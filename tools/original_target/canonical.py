"""Canonical tracked-source input validation and fresh-object V4 acceptance.

Historical manifests remain evidence only. Runtime inputs are the catalog, its
tracked sources, pinned private boundary inventories, and the supplied executable.
"""
import hashlib
import json
import re
from pathlib import Path

import catalog_io
from coff import Coff
from compiler_output import classify
from source_paths import source_path
from verify_manifest import Audit, require, TARGET_BYTES, TARGET_SHA256


# Compiler option sets a canonical region may be accepted under. /Gr, /Ob1 and
# /GR are explicit-only (a plan names them; compile_candidates never tries
# them by default). /GR enables RTTI (the original was built with it); every
# RTTI structure it emits is bound to the original's copy by the strict audit.
RECORDED_OPTIONS = [['/O2', '/Ob2', '/G5', '/Gy'], ['/O2', '/Ob2', '/G6', '/Gy'],
                    ['/O1', '/Ob2', '/G5', '/Gy'], ['/O2', '/Ob2', '/G5', '/Gy', '/Gr'],
                    ['/O2', '/Ob1', '/G5', '/Gy'], ['/O2', '/Ob2', '/G5', '/Gy', '/GR']]
BASE_COMPILER_ARGUMENTS = ['/nologo', '/c', '/TP', '/Zp4', '/GX-', '/GR-', '/W3']


def compiler_arguments(options):
    """Base compiler arguments followed by an option set. An option set
    naming /GR replaces the base /GR- in place (never both on one line)."""
    options = list(options)
    if '/GR' in options:
        return ['/GR' if a == '/GR-' else a for a in BASE_COMPILER_ARGUMENTS] + [o for o in options if o != '/GR']
    return BASE_COMPILER_ARGUMENTS + options


def sha(data):
    return hashlib.sha256(data).hexdigest()


def validate_headers(repository, catalog, read=None):
    headers = catalog.get('headers', [])
    paths, blobs = set(), {}
    read = {} if read is None else read
    for header in headers:
        value = header['path']
        path = Path(value)
        require(not path.is_absolute() and path.as_posix() == value and '..' not in path.parts,
                'Noncanonical header path')
        require(path.parts[:2] == ('src', 'shared') and path.suffix in ('.h', '.hpp'),
                'Header outside canonical shared tree')
        resolved = (repository / path).resolve()
        require(resolved.is_relative_to((repository / 'src/shared').resolve()), 'Header escapes tree')
        require(value not in paths, 'Duplicate canonical header')
        data = read[resolved] = resolved.read_bytes()
        require(sha(data) == header['sha256'], 'Header hash drift')
        paths.add(value)
        blobs[path.relative_to('src').as_posix()] = data
    actual = {p.relative_to(repository).as_posix() for p in (repository/'src/shared').rglob('*')
              if p.is_file() and p.suffix in ('.h', '.hpp')}
    require(paths == actual, 'Uncatalogued/missing shared header')
    return blobs


# Evidenced inline assembly (user decision 2026-11-07, AGENTS.md "No fake source"):
# a C/C++ candidate may hold an `__asm` block only where the TibSun archive shows
# the author's own `__asm` at that point. The row carries `inline_asm: evidenced`
# and `inline_asm_citation` ("TibSun conquer.cpp:346", or a list, one per block in
# source order), the source quotes each citation, and each block holds exactly the
# cited archive block's instructions. Only these archive files are eligible.
INLINE_ASM_FILES = ('conquer.cpp', 'xsurface.cpp', 'keyboard.cpp', 'mpu.cpp', 'lcw.cpp', 'blitblit.h', 'rlerle.h')
INLINE_ASM_CITATION = re.compile(r'TibSun (%s):([1-9][0-9]*)' % '|'.join(map(re.escape, INLINE_ASM_FILES)))
TIBSUN_ARCHIVE = Path('private/reference/TibSun/code')
_ASM_TOKEN = re.compile(r'(?<![A-Za-z0-9_$])(?:__asm|_asm)(?![A-Za-z0-9_$])')
_ASM_MACRO = re.compile(r'[ \t]*#[ \t]*define[ \t]+(\w+)[ \t]+([^\n]*\S)[ \t]*')


def _mask_comments_and_literals(text):
    """`text` with comments blanked and literal contents blanked (same length, newlines kept)."""
    tokens = r'"(?:\\.|[^"\\\n])*"|\'(?:\\.|[^\'\\\n])*\'|/\*.*?\*/|//[^\n]*'
    def blank(match):
        value = match[0]
        if value.startswith(('/*', '//')):
            return re.sub(r'[^\n]', ' ', value)
        return value[0] + ' ' * (len(value) - 2) + value[-1]
    return re.sub(tokens, blank, text, flags=re.S)


def _asm_instructions(body):
    lines = []
    for line in body.splitlines():
        line = line.split(';', 1)[0]                  # MASM comment inside an __asm block
        line = re.sub(r'\s*,\s*', ', ', ' '.join(line.split()))
        if line:
            lines.append(line)
    return lines


def inline_asm_blocks(text, start=0):
    """[(offset, [instruction, ...])] of every `__asm`/`_asm` block from `start`.
    Comments are ignored (a commented-out `__asm` is no block)."""
    masked = _mask_comments_and_literals(text)
    blocks, position = [], start
    while True:
        match = _ASM_TOKEN.search(masked, position)
        if match is None:
            return blocks
        cursor = match.end()
        while cursor < len(masked) and masked[cursor] in ' \t\r\n':
            cursor += 1
        if not masked.startswith('{', cursor):
            cursor = match.end()
        if masked.startswith('{', cursor):
            depth, end = 0, cursor
            while end < len(masked):
                depth += {'{': 1, '}': -1}.get(masked[end], 0)
                if depth == 0:
                    break
                end += 1
            require(depth == 0, 'Unterminated __asm block')
            body, position = masked[cursor + 1:end], end + 1
        else:
            # Single-line form: up to the end of the line or the next __asm on it.
            end = masked.find('\n', cursor)
            end = len(masked) if end < 0 else end
            following = _ASM_TOKEN.search(masked, cursor, end)
            end = following.start() if following else end
            body, position = masked[cursor:end], end
        blocks.append((match.start(), _asm_instructions(body)))


_SCOPE_KEYWORD = re.compile(r'^\s*(?:template\s*<[^>]*>\s*)?(?:class|struct|union|enum|namespace)\b(?:\s+(\w+))?')
_DECLARATOR_NAME = re.compile(r'((?:\w+\s*::\s*)*(?:~\s*)?\w+|(?:\w+\s*::\s*)*operator\s*[^\s(]+)\s*$')


def _row_function_name(symbol_prefix):
    """The qualified name a row's decorated symbol names (`?Get@Straw@@` -> `Straw::Get`,
    `??0X@@` -> `X::X`, `??1X@@` -> `X::~X`); None for operators and other special names."""
    if not symbol_prefix.startswith('?'):
        return symbol_prefix.lstrip('_') or None
    special = None
    body = symbol_prefix[1:]
    if body.startswith('?'):
        if body[1:2] not in ('0', '1'):
            return None
        special, body = body[1], body[2:]
    parts = body.split('@@', 1)[0].split('@')
    if not parts or not all(re.fullmatch(r'\w+', p) for p in parts):
        return None
    if special:
        scope = parts[::-1]
        return '::'.join(scope + [('~' if special == '1' else '') + parts[0]])
    return '::'.join(parts[::-1])


def module_inline_asm_blocks(text, rows):
    """Per-function attribution of the `__asm` blocks of a module source (several
    rows in one file; review 2026-11-11): {entry: [(offset, instructions), ...]}.

    Every block must lie in the body of exactly one function definition the file
    holds, and that function must be exactly one row's (its decorated name); a
    block on a preprocessor line (a macro), outside any function, in a function
    no row names (an inline helper) or in a function two rows could name refuses
    the whole file."""
    masked = _mask_comments_and_literals(text)
    blocks = inline_asm_blocks(text)
    if not blocks:
        return {row['entry']: [] for row in rows}
    directives = {m.start(): m.end() for m in re.finditer(r'(?m)^[ \t]*#(?:[^\n]*\\\n)*[^\n]*', masked)}
    # Asm-only archive macros (amendment, reviewed 2026-11-11): `#define NAME <__asm statements>`
    # counts as expanded at each invocation; any other __asm in a directive refuses.
    macros = {}
    for start, end in directives.items():
        if not _ASM_TOKEN.search(masked, start, end):
            continue
        macro = _ASM_MACRO.fullmatch(masked[start:end])
        require(macro is not None,
                'Module source: an __asm block in a macro cannot be attributed to one function')
        name, body = macro.group(1), macro.group(2)
        require(_ASM_TOKEN.match(body.strip()) is not None and '\\' not in body,
                'Module source: macro %s holds a non-asm token (asm-only macros only)' % name)
        expansion = inline_asm_blocks(body)
        require(name not in macros, 'Module source: macro %s defined twice' % name)
        macros[name] = [instructions for _, instructions in expansion]
    blocks = [b for b in blocks if not any(start <= b[0] < end for start, end in directives.items())]
    for name, expansion in macros.items():
        for match in re.finditer(r'(?<![A-Za-z0-9_$])%s(?![A-Za-z0-9_$])' % re.escape(name), masked):
            if any(start <= match.start() < end for start, end in directives.items()):
                continue
            # The invocation, then one block per __asm statement of the macro.
            blocks.append((match.start(), None, name))
            blocks.extend((match.start(), instructions, name) for instructions in expansion)
    blocks.sort(key=lambda b: (b[0], len(b) > 2 and b[1] is not None))
    # Function bodies: a `{` whose header (since the previous `;`, `}` or `{`) holds a
    # parameter list and is no class/struct/union/enum/namespace head.
    functions, stack, header_start = [], [], 0
    index = 0
    while index < len(masked):
        char = masked[index]
        if index in directives:
            index = header_start = directives[index]
            continue
        if char == '{':
            header = masked[header_start:index]
            scope = _SCOPE_KEYWORD.match(header)
            inside_function = any(kind == 'function' for kind, _, _ in stack)
            if inside_function:
                kind, name = 'block', None
            elif scope or '(' not in header:
                kind, name = 'scope', (scope.group(1) if scope and header.lstrip().startswith('namespace') else None)
            else:
                paren = header.index('(')
                found = _DECLARATOR_NAME.search(header[:paren])
                kind, name = 'function', (re.sub(r'\s+', '', found.group(1)) if found else None)
            stack.append((kind, name, index))
            header_start = index + 1
        elif char == '}':
            require(stack, 'Module source: unbalanced braces')
            kind, name, start = stack.pop()
            if kind == 'function':
                namespaces = [n for k, n, _ in stack if k == 'scope' and n]
                functions.append(('::'.join(namespaces + [name]) if name else None, start, index))
            header_start = index + 1
        elif char == ';' and not any(kind == 'function' for kind, _, _ in stack):
            header_start = index + 1
        index += 1
    names = {}
    for row in rows:
        name = _row_function_name(row['symbol_prefix'])
        if name:
            names.setdefault(name, []).append(row['entry'])
    attributed = {row['entry']: [] for row in rows}
    for block in blocks:
        owners = [f for f in functions if f[1] < block[0] < f[2]]
        require(len(owners) == 1, 'Module source: an __asm block outside a function body')
        name = owners[0][0]
        entries = names.get(name or '', [])
        require(len(entries) == 1,
                'Module source: the __asm block in %s is not one row\'s function (inline helper or overload)' % name)
        attributed[entries[0]].append(block)
    return attributed


def validate_inline_asm(data, row, archive=None, require_archive=False, blocks=None, unit=None):
    """Refuse `__asm` in a candidate unless `row` is flagged as evidenced inline
    assembly with eligible TibSun citations; with `archive` (the TibSun code
    directory), each block must equal the archive block that starts on its cited
    line. Returns the number of blocks. `blocks`, for a row of a module source,
    are the blocks attributed to its own function (module_inline_asm_blocks):
    the flag is then required on rows with blocks and refused on rows without.

    An asm-only macro invocation in a module source (amendment, reviewed 2026-11-11)
    takes one citation for the invocation (the archive line where the author invokes
    the macro) and one per __asm statement of the macro (its archive definition line;
    the repeat count restarts per invocation). `unit` is the module source's file
    name: the macro must be the archive's own in that unit (TibSun <unit>)."""
    text = data.decode(errors='replace') if isinstance(data, bytes) else data
    if blocks is None:
        blocks = inline_asm_blocks(text)
    flag, citations = row.get('inline_asm'), row.get('inline_asm_citation')
    if not blocks:
        require(flag is None and citations is None, 'inline_asm flag on a source without an __asm block')
        return 0
    require(flag == 'evidenced', 'Inline assembly (__asm) in a C/C++ candidate without inline_asm: evidenced')
    citations = [citations] if isinstance(citations, str) else citations
    require(isinstance(citations, list) and citations and
            all(isinstance(c, str) and INLINE_ASM_CITATION.fullmatch(c) for c in citations),
            'inline_asm_citation must be "TibSun <file>:<line>" with <file> one of ' + ', '.join(INLINE_ASM_FILES))
    require(len(citations) == len(blocks), 'One inline_asm_citation per __asm block, in source order')
    for citation in citations:
        require(citation in text, 'Inline-assembly citation %s not quoted in the source' % citation)
    for block, citation in zip(blocks, citations):
        if len(block) > 2:
            require(unit is not None and INLINE_ASM_CITATION.fullmatch(citation).group(1) == unit,
                    'Asm macro %s cited outside its own unit (%s): %s' % (block[2], unit, citation))
    if archive is None or not Path(archive).is_dir():
        require(not require_archive, 'TibSun archive unavailable: evidenced inline assembly cannot be verified')
        return len(blocks)
    plain, invocations, expansion = [], set(), 0
    for block, citation in zip(blocks, citations):
        name, line = INLINE_ASM_CITATION.fullmatch(citation).groups()
        path = Path(archive) / name
        require(path.is_file(), 'Cited TibSun archive file missing: %s' % name)
        source = path.read_text(errors='replace')
        lines = source.splitlines(keepends=True)
        require(int(line) <= len(lines), 'Cited TibSun line out of range: %s' % citation)
        first = sum(len(l) for l in lines[:int(line) - 1])
        cited_line = _mask_comments_and_literals(source)[first:first + len(lines[int(line) - 1])]
        if len(block) > 2 and block[1] is None:
            # The invocation: the author invokes this macro on the cited line, once per citation.
            require(re.match(r'[ \t]*#', cited_line) is None and
                    re.search(r'(?<![A-Za-z0-9_$])%s(?![A-Za-z0-9_$])' % re.escape(block[2]), cited_line),
                    'Asm macro %s has no archive invocation on the cited line %s' % (block[2], citation))
            require(citation not in invocations, 'Asm macro invocation cited twice: %s' % citation)
            invocations.add(citation)
            expansion = 0
            continue
        if len(block) > 2:
            # One __asm statement of the macro: the archive's own definition, same instructions.
            definition = _ASM_MACRO.fullmatch(cited_line.rstrip('\r\n'))
            require(definition is not None and definition.group(1) == block[2],
                    'Asm macro %s is not defined on the cited TibSun line %s' % (block[2], citation))
            cited = inline_asm_blocks(definition.group(2))
            require(len(cited) > expansion and block[1] == cited[expansion][1],
                    'Asm macro %s differs from the archive definition %s' % (block[2], citation))
            expansion += 1
            continue
        # A repeated citation names the next block on the same archive line
        # (`__asm _emit 0x0f __asm _emit 0x31`).
        repeat = plain.count(citation)
        plain.append(citation)
        cited = [b for b in inline_asm_blocks(source, first) if b[0] < first + len(lines[int(line) - 1])]
        require(len(cited) > repeat, 'No __asm block starts on the cited TibSun line: %s' % citation)
        require(block[1] == cited[repeat][1],
                'Inline-assembly block differs from the cited TibSun archive block %s' % citation)
    return len(blocks)


# Count-calibrated rows (user decision 2026-11-09, docs/HANDOFF.md): a row whose
# named locals or helper forms were chosen by a symbol-count or inline-budget
# calibration is accepted only when every named local holds a real, used value,
# twin forms and real inline helpers were checked first, and the row says so:
# `count_calibrated: true` plus a non-empty `count_calibrated_note` naming the
# chosen forms and the evidence checked first. status.py counts these rows as
# their own category inside matched C/C++. A plan item may say
# `count_calibrated: false` to clear an existing region's flag on re-acceptance
# (accept_sources); a candidate or canonical row never stores false.
COUNT_CALIBRATED_KEYS = ('count_calibrated', 'count_calibrated_note')


def validate_count_calibrated(row):
    """Refuse a malformed count-calibration flag on a candidate or canonical row;
    return whether the row is flagged."""
    flag, note = row.get('count_calibrated'), row.get('count_calibrated_note')
    if flag is None:
        require(note is None, 'count_calibrated_note without count_calibrated: true')
        return False
    require(flag is True, 'count_calibrated must be true (omit both keys on an uncalibrated row)')
    require(isinstance(note, str) and note.strip() != '',
            'count_calibrated: true needs a non-empty count_calibrated_note (chosen forms and evidence checked first)')
    return True


def validate_split_declaration(row):
    """Refuse a malformed split-declaration flag (docs/DECISIONS.md "split declaration": a row
    whose bytes depend on a header keeping a second declaration of a module's class); return
    whether the row is flagged. The note names both headers and what flips."""
    flag, note = row.get('split_declaration'), row.get('split_declaration_note')
    if flag is None:
        require(note is None, 'split_declaration_note without split_declaration: true')
        return False
    require(flag is True, 'split_declaration must be true (omit both keys on an unflagged row)')
    require(isinstance(note, str) and note.strip() != '' and note.count('.hpp') >= 2,
            'split_declaration: true needs a split_declaration_note naming both headers')
    return True


# VECTOR_DECLARATIONS_ONLY (docs/DECISIONS.md "Container member joins on their own frozen
# bound"): a source that keeps every shared/vector_class.hpp member undefined because its text
# does not reproduce the original unit's list uses. A judgment call only for the rows whose body
# references a container member the switch leaves undefined (an accepted relocation naming a
# VectorClass/DynamicVectorClass symbol, the class's vtable included): exactly those rows carry
# `declarations_only: true` and a non-empty `declarations_only_note`, and no other row may
# (owner ruling 2026-11-12).
DECLARATIONS_ONLY_SWITCH = re.compile(rb'^[ \t]*#[ \t]*define[ \t]+VECTOR_DECLARATIONS_ONLY\b', re.M)
DECLARATIONS_ONLY_KEYS = ('declarations_only', 'declarations_only_note')
CONTAINER_MEMBER_SYMBOL = re.compile(r'\?\$(?:Dynamic)?VectorClass@')


def references_container_member(row):
    """Whether the row's accepted relocations name a container member or vtable."""
    return any(CONTAINER_MEMBER_SYMBOL.search(r['symbol']) for r in row.get('relocations') or [])


# Data-only bindings (review 2026-11-13, docs/DECISIONS.md "Data-only symbols bound by section
# layout / bss by elimination"): `data_bound_by` {symbol: "layout" | "elimination"} with a
# non-empty `data_bound_by_note` (the split into declarations is a hypothesis), the same on
# every row of the source; the strict audit (data_bindings.py) checks the conditions.
DATA_BOUND_BY_KEYS = ('data_bound_by', 'data_bound_by_note')


def validate_data_bound_by(row, source_rows):
    import data_bindings
    if not any(row.get(k) is not None for k in DATA_BOUND_BY_KEYS):
        require(not any(r.get('data_bound_by') for r in source_rows),
                'data_bound_by must be on every row of its source: %08x' % row['entry'])
        return
    try:
        data_bindings.declared([row])
    except data_bindings.BindingRefused as error:
        raise ValueError('%s: %08x' % (error, row['entry']))
    require(all(r.get('data_bound_by') == row['data_bound_by'] and
                r.get('data_bound_by_note') == row['data_bound_by_note'] for r in source_rows),
            'data_bound_by differs between rows of one source: %08x' % row['entry'])


def validate_declarations_only(row, data):
    """Refuse a declarations-only flag that disagrees with the row's source text and body;
    return whether the row is flagged."""
    expected = bool(DECLARATIONS_ONLY_SWITCH.search(data)) and references_container_member(row)
    flag, note = row.get('declarations_only'), row.get('declarations_only_note')
    if not expected:
        require(flag is None and note is None,
                'declarations_only flag on a row that does not reference a container member left undefined by '
                'VECTOR_DECLARATIONS_ONLY: %08x' % row['entry'])
        return False
    require(flag is True and isinstance(note, str) and note.strip() != '',
            'a row referencing a container member that VECTOR_DECLARATIONS_ONLY leaves undefined needs '
            'declarations_only: true and a note: %08x' % row['entry'])
    return True


# RA2_SDK_HEADERS migration switch (docs/DECISIONS.md "SDK headers for a unit"):
# the one pinned system header set (manifests/toolchain.json "sdk_headers") and
# the only tracked headers that may name it with angle includes, each inside an
# `#ifdef RA2_SDK_HEADERS` (or `#if defined(RA2_SDK_HEADERS)`) block. A unit
# converts by defining RA2_SDK_HEADERS at line 1; every other source sees none
# of these includes. Each wrapper is the src/shared counterpart of an original
# header carrying system includes (TibSun archive / mischa85 chain):
SDK_SWITCH = 'RA2_SDK_HEADERS'
SDK_WRAPPERS = {
    'shared/irtti.hpp': 'irtti.h:12 <comdef.h> (TibSun code/irtti.h, mischa85 include/irtti.h)',
    'shared/win.hpp': 'win.h:35-43 WIN32_LEAN_AND_MEAN + windows/mmsystem/windowsx/winnt/winuser (TibSun code/win.h)',
    'shared/sdk_prelude.hpp': 'flat prelude for the first landing: rect.h:40, fastmath.h:16, sun.h:19, irtti.h, win.h, vector.h:55-57',
}
TOOLCHAIN_MANIFEST = Path(__file__).resolve().parents[2] / 'manifests/toolchain.json'


def sdk_tree_digest(files):
    """Digest of a header tree: sha256 of `shasum -a 256` lines sorted by path."""
    return sha(''.join('%s  ./%s\n' % (files[name], name) for name in sorted(files)).encode())


def sdk_pin(manifest=None):
    """The pinned SDK header set {path: sha256} (paths relative to the tree root,
    as extracted); validates its recorded tree digest."""
    path = Path(manifest or TOOLCHAIN_MANIFEST)
    key = (str(path), path.stat().st_mtime_ns)
    if key not in _SDK_PIN_CACHE:
        _SDK_PIN_CACHE[key] = _load_sdk_pin(path)
    return _SDK_PIN_CACHE[key]


_SDK_PIN_CACHE = {}


def sdk_pin_record(manifest=None):
    """The whole sdk_headers record of the toolchain manifest."""
    return json.loads(Path(manifest or TOOLCHAIN_MANIFEST).read_text())['sdk_headers']


def _load_sdk_pin(path):
    document = json.loads(path.read_text())
    pin = document.get('sdk_headers')
    require(isinstance(pin, dict) and isinstance(pin.get('files'), dict) and pin['files'],
            'No pinned SDK header set (manifests/toolchain.json sdk_headers)')
    require(sdk_tree_digest(pin['files']) == pin.get('tree_sha256'), 'SDK pin tree digest differs from its files')
    return pin['files']


def sdk_resolve(name, files):
    """Pinned path an angle include names (case-insensitive, as on the Windows
    compiler host), or None."""
    wanted = name.replace('\\', '/').lower()
    for path in files:
        if path.lower() == wanted:
            return path
    return None


def verify_sdk_tree(directory, files):
    """Refuse a local SDK tree whose files differ from the pin (missing, changed
    or extra); return its tree digest."""
    directory = Path(directory)
    require(directory.is_dir(), 'SDK header directory missing: %s' % directory)
    actual = {p.relative_to(directory).as_posix(): sha(p.read_bytes())
              for p in sorted(directory.rglob('*')) if p.is_file()}
    require(actual == files, 'SDK header tree differs from manifests/toolchain.json sdk_headers')
    return sdk_tree_digest(actual)


def angle_includes(data):
    """Angle-include names in a (directive-normalized) text; for build tools."""
    return set(re.findall(r'^\s*#\s*include\s*<([^>\n]+)>', data.decode(errors='replace'), flags=re.M))


def validate_includes(data, headers, name=None, sdk=None):
    """Quoted includes a tracked text has (all must be catalogued shared headers).
    Angle includes are refused except in an SDK_WRAPPERS header (`name`), inside
    an RA2_SDK_HEADERS block, naming a pinned SDK file (`sdk`, default the pin)."""
    # Apply preprocessing character/line/comment phases before recognizing
    # directives, so comments or continuations cannot hide a dependency.
    text = data.decode()
    for old, new in [('??=', '#'), ('??/', chr(92)), ('??\'', '^'), ('??(', '['),
                     ('??)', ']'), ('??!', '|'), ('??<', '{'), ('??>', '}'), ('??-', '~')]:
        text = text.replace(old, new)
    text = re.sub(r'\\\r?\n', '', text)
    tokens = r'"(?:\\.|[^"\\])*"|\'(?:\\.|[^\'\\])*\'|/\*.*?\*/|//[^\n]*'
    text = re.sub(tokens, lambda m: (' ' + '\n' * m[0].count('\n'))
                  if m[0].startswith(('/*', '//')) else m[0], text, flags=re.S)
    text = re.sub(r'^(\s*)%:', r'\1#', text, flags=re.M)
    includes = set()
    switch = []    # one entry per open conditional: inside an RA2_SDK_HEADERS-true branch
    for line in text.splitlines():
        directive = re.match(r'^\s*#\s*(\w+)', line)
        if not directive: continue
        kind = directive[1]
        require(kind not in ('import', 'using') and
                not (kind == 'pragma' and 'include_alias' in line), 'Untracked compiler dependency directive')
        if kind in ('if', 'ifdef', 'ifndef'):
            switch.append(re.fullmatch(r'\s*#\s*(?:ifdef\s+%s|if\s+defined\s*\(?\s*%s\s*\)?)\s*'
                                       % (SDK_SWITCH, SDK_SWITCH), line) is not None)
        elif kind in ('else', 'elif'):
            require(bool(switch), 'Unbalanced conditional directive')
            switch[-1] = False
        elif kind == 'endif':
            require(bool(switch), 'Unbalanced conditional directive')
            switch.pop()
        elif kind.startswith('include'):
            angle = re.fullmatch(r'\s*#\s*include\s*<([^<>"\n]+)>\s*', line)
            if angle is not None:
                require(name in SDK_WRAPPERS, 'Angle include outside an SDK wrapper header (canonical.SDK_WRAPPERS)')
                require(any(switch), 'SDK include outside an #ifdef %s block' % SDK_SWITCH)
                require(sdk_resolve(angle[1], sdk_pin() if sdk is None else sdk) is not None,
                        'SDK include does not resolve to a pinned header: <%s>' % angle[1])
                continue
            match = re.fullmatch(r'\s*#\s*include\s+"([^"\n]+)"\s*', line)
            require(match is not None and match[1] in headers, 'Untracked/noncanonical include dependency')
            includes.add(match[1])
    require(not switch, 'Unbalanced conditional directive')
    return includes


def dependency_headers(audit, sources):
    location = catalog_io.locate(audit.repository)
    stored = audit.document(location / catalog_io.INDEX if catalog_io.is_sharded(location) else location)
    headers = validate_headers(audit.repository, catalog_io.header_document(stored))
    pending = set()
    for source in sources:
        pending.update(validate_includes(audit.read(source['source'], source['source_sha256']), headers))
    seen = set()
    while pending:
        name = pending.pop()
        if name in seen: continue
        seen.add(name)
        pending.update(validate_includes(headers[name], headers, name) - seen)
    return [dict(path='src/' + name, sha256=sha(headers[name])) for name in sorted(seen)]


def compile_group_key(row, separate_populations):
    """Translation unit a canonical source compiles in.

    Sources of one population and option set normally form one unit (in
    source order). A population marked separate_units compiles every source
    alone (e.g. explicit template instantiations of single members).
    """
    key = (row['population'], tuple(row['options']))
    return key + (row['source'],) if row['population'] in separate_populations else key


def bound_population(row):
    """Population whose independently frozen inventory pass holds a row's bound.

    Normally the row's own population. A row that joined another unit's translation unit
    (docs/DECISIONS.md "Route A split rows", option (ii)) compiles in that unit's population
    (`population`: compile group, option set, reference table) and keeps the population it
    was frozen and accepted under as `bound_population`: its bound, gap padding and
    provenance are still read from that population's inventory pass, never from the unit's
    pass and never from the candidate."""
    return row.get('bound_population', row['population'])


def separate_unit_populations(catalog):
    return {p['id'] for p in catalog.get('populations', []) if p.get('separate_units')}


def validate_catalog(repository, catalog, modules=True):
    """Validate the catalog against the tree; return {path: bytes} of every
    source and header read (resolved paths), for callers that check the same
    files again."""
    require(catalog['schema'] == 'ra2-canonical-source-v1', 'Unsupported catalog')
    require(catalog['target'] == dict(size=TARGET_BYTES, sha256=TARGET_SHA256), 'Wrong catalog target')
    read = {}
    headers = validate_headers(repository, catalog, read)
    for name, data in headers.items():
        validate_includes(data, headers, name)
        # The switch is per unit (line 1 of a converted unit), never set by a shared header.
        require(re.search(r'^\s*#\s*define\s+%s\b' % SDK_SWITCH, data.decode(errors='replace'), flags=re.M) is None,
                '%s defined in a shared header' % SDK_SWITCH)
        require(not inline_asm_blocks(data.decode(errors='replace')), 'Inline assembly (__asm) in a shared header')
    ids = [p['id'] for p in catalog['populations']]
    require(len(ids) == len(set(ids)), 'Duplicate populations')
    entries, paths, orders = set(), {}, set()
    cursor = 0
    # Module sources (one file, several rows): `__asm` blocks are attributed per function.
    module_rows, module_blocks = {}, {}
    for row in catalog['regions']:
        module_rows.setdefault(row['source'], []).append(row)
    policies = {p['id'] for p in catalog['populations'] if p.get('compiler_output_policy')}
    for row in sorted(catalog['regions'], key=lambda r: r['entry']):
        va = row['entry']
        # One region, one unit: a second row at an accepted entry (a function claimed by the
        # population that froze it and by a unit it joined, or by two units) refuses.
        require(va not in entries, 'Region claimed by two units: %08x' % va)
        require(va >= cursor and row['body_bytes'] > 0 and 0 <= row['padding_bytes'] <= 15,
                'Overlapping/invalid canonical extent')
        require(row['full_extent_bytes'] == row['body_bytes'] + row['padding_bytes'], 'Extent size differs')
        require(row['entry'] not in entries and row['source_order'] not in orders,
                'Duplicate canonical entry/source/order')
        # A module source (one file per original translation unit, several
        # regions) is shared only by rows of one population: one compile unit.
        require(paths.setdefault(row['source'], row['population']) == row['population'],
                'Source shared across populations: ' + row['source'])
        if row.get('semantic_name'):
            # (address-free file names, docs/DECISIONS.md: the name no longer carries the entry)
            require(Path(row['source']).stem == row['semantic_name'] and
                    len(Path(row['source']).parts) > 3,
                    'Understood operation requires descriptive filename and purpose directory')
        require(row['population'] in ids, 'Missing bound population')
        if 'bound_population' in row:
            # A row compiled in another unit's translation unit keeps its own frozen population.
            require(row['bound_population'] in ids and row['bound_population'] != row['population'],
                    'Joined row without its own frozen population: %08x' % va)
            require(row['population'] not in policies and row['bound_population'] not in policies,
                    'Joined row in a closed compiler-output population: %08x' % va)
        require(row['options'] in RECORDED_OPTIONS, 'Unsupported accepted options')
        resolved = source_path(repository, row['source'])
        data = read[resolved] = resolved.read_bytes()
        require(sha(data) == row['source_sha256'], 'Source hash drift')
        validate_includes(data, headers)
        if len(module_rows.get(row['source'], ())) > 1:
            if row['source'] not in module_blocks:
                module_blocks[row['source']] = module_inline_asm_blocks(
                    data.decode(errors='replace'), module_rows[row['source']])
            validate_inline_asm(data, row, repository / TIBSUN_ARCHIVE, blocks=module_blocks[row['source']][va],
                                unit=Path(row['source']).name)
        else:
            validate_inline_asm(data, row, repository / TIBSUN_ARCHIVE)
        validate_count_calibrated(row)
        validate_split_declaration(row)
        validate_declarations_only(row, data)
        validate_data_bound_by(row, module_rows.get(row['source'], ()))
        entries.add(va); orders.add(row['source_order'])
        cursor = va + row['full_extent_bytes']
    require(bool(entries), 'Empty canonical source catalog')
    # Draft-only files (non_exact.json: a non-exact draft of a unit without a module, guarded by
    # RA2_NON_MATCHING) are the only uncatalogued sources; the exact build never compiles them.
    import non_exact
    drafts = non_exact.draft_only_sources(repository, catalog)
    require(set(paths) | drafts == {p.relative_to(repository).as_posix() for p in (repository/'src/recovered').rglob('*.cpp')},
            'Uncatalogued/missing canonical sources')
    problems = non_exact.validate(repository, catalog)
    require(not problems, 'Non-exact drafts: ' + '; '.join(problems[:5]))
    if modules:
        validate_modules(repository, catalog)
    return read


def validate_modules(repository, catalog):
    """Source-form modules (manifests/original_target/modules.json): every reference into a
    module names the module's own symbols, and a split class declaration agrees with the
    module's (module_check.py). The build runs it at its start and end; a batch that is
    still re-verifying its relocation receipts (apply_rename.py) runs it once they are fresh."""
    import module_check
    modules = module_check.load_modules(repository / 'manifests/original_target/modules.json')
    if modules:
        problems, _ = module_check.check(catalog, modules, lambda path: (repository / path).read_text(errors='replace'))
        require(not problems, 'Module agreement: ' + '; '.join(problems[:5]))


class CanonicalAudit(Audit):
    """Supply V4 with canonical rows and private independently frozen bounds.

    The inherited verifier retains all decoding, section-coverage, reference,
    common-symbol, operand-boundary and full-extent checks. No candidate search,
    historical object, aggregate source, report, or ignored Python is executed.
    """
    def __init__(self, repository, catalog, config):
        super().__init__(repository, config['executable'])
        validate_catalog(repository, catalog, modules=False)    # the build and the batch run the module check themselves
        self.catalog = catalog
        self.bounds = {}
        self.groups = {}
        # Most populations share a few inventory passes: parse each verified
        # file content (by hash) once. The rows are only read afterwards.
        parsed = {}
        def rows(path, digest, key):
            if (digest, key) not in parsed:
                data = self.read(path, digest)
                items = [json.loads(line) for line in data.splitlines() if line.strip()]
                table = {int(r[key], 16): r for r in items}
                require(len(table) == len(items), 'Duplicate original boundary rows')
                parsed[digest, key] = table
            else:
                self.read(path, digest)
            return parsed[digest, key]
        for population in catalog['populations']:
            pid = population['id']
            inputs = config['populations'][pid]
            self.bounds[pid] = (rows(inputs['inventory'], population['inventory_sha256'], 'entry'),
                                rows(inputs['gaps'], population['gaps_sha256'], 'start'))

    def population(self, manifest):
        key = manifest['canonical_compile_group']
        rows = self.groups[key]
        pid = rows[0]['population']
        inventory, gaps = self.bounds[pid]
        if any(bound_population(row) != pid for row in rows):
            # Rows joined from other populations: each is checked against the bound and the
            # gap row of its OWN frozen pass. The unit's pass says nothing about them (its
            # rows at those addresses, if any, are replaced for this group's audit); a
            # joined row whose own pass holds no bound refuses.
            inventory, gaps = dict(inventory), dict(gaps)
            for row in rows:
                own = bound_population(row)
                if own == pid:
                    continue
                require(own in self.bounds, 'Joined row names an unknown frozen population')
                own_inventory, own_gaps = self.bounds[own]
                require(row['entry'] in own_inventory,
                        'Joined row has no frozen bound in its own population %s: %08x' % (own, row['entry']))
                inventory[row['entry']] = own_inventory[row['entry']]
                end = row['entry'] + row['body_bytes']
                gaps.pop(end, None)
                if end in own_gaps:
                    gaps[end] = own_gaps[end]
        candidates, cases = {}, {}
        for row in rows:
            body = self.original(row['entry'], row['body_bytes'])
            pad = self.original(row['entry'] + row['body_bytes'], row['padding_bytes']) if row['padding_bytes'] else b''
            require(sha(pad) == row['padding_sha256'], 'Frozen padding hash differs')
            candidate = dict(row, original_inventory_row=inventory[row['entry']],
                             region_bytes=row['full_extent_bytes'], region_sha256=row['full_extent_sha256'],
                             padding_hex=pad.hex(), padding_provenance=gaps.get(row['entry']+len(body)) if pad else row['padding_provenance'])
            candidates[row['method']] = candidate
            cases[(row['method'], row['variant'])] = dict(row, status='extent-exact')
        return candidates, inventory, gaps, cases, key

    def compiler_output(self, owner, candidates, case, coff, inventory, gaps):
        pid = next(iter(candidates.values()))['population']
        populations = self.catalog.get('populations', [])
        population = next((p for p in populations if p['id'] == pid), {})
        reference = population.get('compiler_output_policy')
        if reference is None: return []
        require(all(bound_population(c) == pid for c in candidates.values()),
                'Joined row in a closed compiler-output population')
        compiler = self.document('manifests/toolchain.json')['compiler']
        return classify(self, coff, reference, list(candidates.values()), case['options'], compiler,
                        population['inventory_sha256'], population['gaps_sha256'],
                        dependency_headers(self, list(candidates.values())), (inventory, gaps))

    def verify_fresh(self, destination, compiled_groups, complete=True, jobs=None):
        # Full-file equality can hide omitted source behind original scaffold.
        # Require the complete canonical plan before any object is accepted.
        # A partial check (apply_rename.py) still compiles whole canonical units.
        expected = {}
        separate = separate_unit_populations(self.catalog)
        for row in sorted(self.catalog['regions'], key=lambda r: r['source_order']):
            expected.setdefault(compile_group_key(row, separate), []).append(row)
        if complete:
            require([rows for rows, _ in compiled_groups] == list(expected.values()),
                    'Fresh compile groups differ from complete canonical source plan')
        else:
            require(compiled_groups and all(rows == expected.get(compile_group_key(rows[0], separate))
                                            for rows, _ in compiled_groups),
                    'Fresh compile groups are not whole canonical units')
        runtime, objects, parsed = [], {}, {}
        for index, (rows, object_path) in enumerate(compiled_groups):
            data = object_path.read_bytes()
            object_hash = sha(data)
            coff = Coff(data)
            objects[self.path(object_path)] = data
            parsed[self.path(object_path)] = coff
            key = 'fresh-%03d' % index
            group_manifest = destination / (key + '.json')
            group_manifest.write_text(json.dumps(dict(canonical_compile_group=key)))
            group = []
            for canonical in rows:
                symbol, section, start, end = coff.function(canonical['symbol_prefix'])
                own = bound_population(canonical)
                require(own in self.bounds, 'Row names an unknown frozen population: %s' % own)
                bound, gaps = self.bounds[own]
                require(canonical['entry'] in bound,
                        ('Joined row has no frozen bound in its own population %s: %08x' if own != canonical['population']
                         else 'Row has no frozen bound in %s: %08x') % (own, canonical['entry']))
                pad = self.original(canonical['entry']+canonical['body_bytes'], canonical['padding_bytes']) if canonical['padding_bytes'] else b''
                # Empty padding provenance has multiple historical representations;
                # absence is canonical here, consistently passed to candidate and row.
                provenance = gaps.get(canonical['entry']+canonical['body_bytes']) if pad else None
                row = dict(canonical, object=str(object_path), object_sha256=object_hash,
                           variant=0, original_bound=bound[canonical['entry']], padding_hex=pad.hex(),
                           padding_provenance=provenance, accepted_batch_manifest=str(group_manifest),
                           section_index=symbol['section'], section_start=start, section_end=end,
                           raw_section_sha256=sha(section['data'][start:end]))
                require(row['symbol'] == symbol['name'], 'Fresh symbol differs')
                group.append(row)
            self.groups[key] = group
            runtime.extend(group)
        manifest = dict(executable_sha256=TARGET_SHA256, accepted=runtime,
                        matched_source_regions=len(runtime), body_bytes=sum(r['body_bytes'] for r in runtime),
                        frozen_padding_bytes=sum(r['padding_bytes'] for r in runtime),
                        full_extent_bytes=sum(r['full_extent_bytes'] for r in runtime))
        path = destination/'fresh-accepted.json'
        digest, spans = write_manifest(path, manifest)
        # The audit reads every row back from the written file. Each compile
        # group (one object) is audited completely by one task; tasks run in
        # a process pool and their results are merged in the serial audit's
        # order, so the report is byte-identical to Audit.audit(path).
        tasks, offset = [], 0
        for index, (rows, object_path) in enumerate(compiled_groups):
            group = spans[offset:offset+len(rows)]; offset += len(rows)
            tasks.append((index, group[0][1], group[-1][1]+group[-1][2]-group[0][1],
                          sorted((r['entry'], start, length) for r, (_, start, length) in zip(rows, group))))
        result = self.audit_parallel(path, manifest, digest, tasks, jobs)
        # Later steps (placement transfer, final input check) use the objects
        # parsed above; the workers re-read and re-parsed them from disk.
        self.blobs.update(objects)
        self.coffs.update(parsed)
        (destination/'fresh-verification.json').write_text(json.dumps(result, indent=2)+'\n')
        return manifest

    def audit_parallel(self, path, manifest, digest, tasks, jobs=None):
        """Audit the written manifest's rows group by group (see _audit_group)."""
        require(manifest.get('executable_sha256') == TARGET_SHA256, 'Manifest executable identity differs')
        saved = {name: self.__dict__.pop(name) for name in RECORDED if name in self.__dict__}
        jobs = verify_jobs(jobs, len(tasks))
        # Largest groups first, so one long group does not finish last.
        order = sorted(tasks, key=lambda t: -len(t[3]))
        global _WORKER
        _WORKER = (self, path)
        try:
            if jobs > 1:
                import gc
                import multiprocessing
                gc.collect(); gc.freeze()       # keep forked copy-on-write pages shared
                try:
                    with multiprocessing.get_context('fork').Pool(jobs) as pool:
                        outcomes = list(pool.imap_unordered(_audit_group, order, chunksize=4))
                finally:
                    gc.unfreeze()
            else:
                outcomes = [_audit_group(task) for task in order]
        finally:
            _WORKER = None
            for name in RECORDED:
                self.__dict__.pop(name, None)
            self.__dict__.update(saved)
        refused = sorted(r for outcome in outcomes for r in outcome['refused'])
        require(not refused, '%d fresh row(s) refused:\n%s' % (len(refused), '\n'.join(m for _, m in refused)))
        # Replay what the tasks recorded in the serial audit's order (rows by
        # entry, then the order within the row): dict insertion order and the
        # last value bound to a name are then the serial path's.
        events = sorted((event for outcome in outcomes for event in outcome['events']), key=lambda e: (e[0], e[1]))
        for _, _, name, key, value in events:
            if name == 'kept_elsewhere_omitted':
                self.__dict__.setdefault(name, []).append(value)
            else:
                self.__dict__.setdefault(name, {})[key] = value
        digests = self.__dict__.setdefault('blob_digests', {})
        for outcome in outcomes:
            for key, value in outcome['blobs']:
                if Path(key) not in self.blobs:
                    require(digests.setdefault(Path(key), value) == value, 'Proof input read with two contents: '+key)
        digests[self.path(path)] = digest
        result, cursor = [], 0
        for row in sorted((row for outcome in outcomes for row in outcome['rows']), key=lambda r: r['entry']):
            require(row['entry'] >= cursor, 'Overlapping accepted extents')
            result.append(row)
            cursor = row['entry'] + row['full_extent_bytes']
        require(len(result) == len(manifest['accepted']), 'Audited row count differs from the manifest')
        return self.audit_result(path, manifest, result, digest)


# Audit state that rows add to and that the report or later steps read.
RECORDED = ('discarded_outputs', 'rtti_original_copies', 'data_original_copies', 'rtti_known_differences',
            'kept_elsewhere_omitted')
_WORKER = None


def verify_jobs(jobs, tasks):
    """Verification processes: RA2_VERIFY_JOBS, else min(4, CPUs). Forked
    workers inherit the loaded audit; without fork the tasks run in-process."""
    import multiprocessing
    import os
    if jobs is None:
        jobs = int(os.environ.get('RA2_VERIFY_JOBS') or min(4, os.cpu_count() or 1))
    if 'fork' not in multiprocessing.get_all_start_methods():
        return 1
    return max(1, min(jobs, tasks))


class _Recorder(dict):
    """A dict that logs each assignment with the row being audited."""
    def __init__(self, name, log, audit):
        super().__init__(); self.name, self.log, self.audit = name, log, audit

    def __setitem__(self, key, value):
        self.log.append((self.audit._current_entry, len(self.log), self.name, key, value))
        super().__setitem__(key, value)

    def update(self, other=(), **more):
        for key, value in dict(other, **more).items():
            self[key] = value


class _RecordedList(list):
    def __init__(self, name, log, audit):
        super().__init__(); self.name, self.log, self.audit = name, log, audit

    def append(self, value):
        self.log.append((self.audit._current_entry, len(self.log), self.name, None, value))
        super().append(value)


class _LoggedBlobs(dict):
    """Audit.blobs that remembers which paths were read since the last task."""
    def __init__(self, data):
        super().__init__(data); self.added = []

    def __setitem__(self, key, value):
        if key not in self:
            self.added.append(key)
        super().__setitem__(key, value)


def _audit_group(task):
    """Audit one compile group's rows from the written manifest (in a worker
    process, or in-process). Returns the result rows, the audit state they
    recorded (tagged with entry and order) and the inputs read with hashes."""
    audit, path = _WORKER
    index, start, length, rows = task
    log = []
    for name in RECORDED[:4]:
        setattr(audit, name, _Recorder(name, log, audit))
    audit.kept_elsewhere_omitted = _RecordedList('kept_elsewhere_omitted', log, audit)
    if not isinstance(audit.blobs, _LoggedBlobs):
        audit.blobs = _LoggedBlobs(audit.blobs)
    audit.blobs.added = []
    with open(path, 'rb') as handle:
        handle.seek(start)
        data = handle.read(length)
    require(len(data) == length, 'Short fresh manifest read')
    populations, results, refused = {}, [], []
    for entry, offset, size in rows:
        row = json.loads(data[offset-start:offset-start+size])
        require(row['entry'] == entry, 'Fresh manifest row placement differs')
        audit._current_entry = entry
        # A refused row is reported with every other refused row of the
        # build (audit_parallel raises them together), so one header change
        # shows all the units and regions it moved.
        try:
            results.append(audit.audit_row(row, path, populations))
        except ValueError as error:
            refused.append((entry, str(error)))
    blobs = [(str(key), audit.sha(audit.blobs[key])) for key in audit.blobs.added]
    return dict(group=index, rows=results, events=log, blobs=blobs, refused=refused)


def write_manifest(path, manifest):
    """Write json.dumps(manifest)+'\\n' to `path`, streamed; return its SHA-256
    and each accepted row's (index, byte offset, byte length).

    Every row of a population carries the same reference table object, and
    encoding those tables was most of the cost: each distinct table is
    encoded once and spliced into its rows. A compact JSON value encodes the
    same wherever it appears, so the bytes equal json.dumps(manifest).
    """
    head = json.dumps(dict(manifest, accepted=[]))
    marker = '"accepted": []'
    require(head.count(marker) == 1, 'Unexpected manifest layout')
    before, after = head.split(marker)
    tables, spans = {}, []
    digest = hashlib.sha256()
    position = 0
    with path.open('wb') as handle:
        def emit(text):
            nonlocal position
            data = text.encode()
            handle.write(data); digest.update(data)
            position += len(data)
        emit(before + '"accepted": [')
        for index, row in enumerate(manifest['accepted']):
            if index:
                emit(', ')
            references = row.get('references')
            if isinstance(references, dict) and references:
                if id(references) not in tables:
                    tables[id(references)] = (references, json.dumps(references))
                token = '\0references-%d\0' % index
                text = json.dumps(dict(row, references=token))
                quoted = json.dumps(token)
                require(text.count(quoted) == 1, 'Unexpected manifest row layout')
                text = text.replace(quoted, tables[id(references)][1])
            else:
                text = json.dumps(row)
            start = position
            emit(text)
            spans.append((index, start, position-start))
        emit(']' + after + '\n')
    return digest.hexdigest(), spans
