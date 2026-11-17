#!/usr/bin/env python3
"""Non-exact drafts in source (user decision 2026-11-13; docs/DECISIONS.md last rows).

    non_exact.py audit                       the checks the strict audit runs (canonical.validate_catalog)
    non_exact.py check [--sources S ...] [--output DIR] [--update] [--config C]
    non_exact.py place LIST.json [--date YYYY-MM-DD]

Exact matching on the unmatched tail stopped. Each unmatched function's best draft sits in its
unit's source at its address-order place inside `#ifdef RA2_NON_MATCHING ... #endif`, with one
marker comment line directly above the `#ifdef`:

    // Not exact (RA2_NON_MATCHING): ParticleClass::Spark_AI at 0x00609540, best score 66.45, 2026-11-13; differs: ...

The default (exact) build never defines the guard, so it never compiles a draft and keeps the
original bytes. manifests/original_target/non_exact.json lists every placed draft (entry, name,
original body bytes, source, the accepted row whose compile group hosts it, the scored symbol and
the `non_exact` flag: score, date, what differs). No .text credit is taken for any of it.

audit (also run by canonical.validate_catalog, the ordinary gate) refuses:
  * a draft whose entry or extent overlaps an accepted region (a guarded function claiming
    acceptance), or a score of 100 (an exact draft is accepted instead and loses its guard);
  * an accepted row's address header comment inside a guarded block;
  * any guard spelling but `#ifdef RA2_NON_MATCHING` (no #if/#ifndef/#elif/#else forms, no
    #define/#undef of the guard, no nested guard);
  * a guarded block holding inline assembly (`__asm`, `_asm`) or instruction arrays: `__emit`/`_emit`,
    a brace initializer of 16 or more byte-sized hex literals, a string of 8 or more \\x escapes;
  * a guarded block in src/recovered without its marker line, a marker without a manifest row (or
    with another score/date than the row), a row whose marker is missing or repeated;
  * a draft-only source file (not a catalog source) with anything outside its guarded blocks but
    comments and blank lines.
Guarded blocks in src/shared headers (declarations a draft needs) get the same guard-form and
instruction-array checks; they need no marker.

check compiles every compile group that hosts a draft the way build.py does, with
/DRA2_NON_MATCHING added (each must compile), and scores each draft's symbol against the original
body with diff_candidate's scorer. --update writes the scores (and resolved symbols) back into the
manifest and the marker lines.

place inserts drafts: LIST items {entry, bytes, draft, name?, symbol?, pre?, anchor?, source?,
differs?, references?}. A draft goes into its unit's Route A module source (unit_map.json ->
modules/<unit>.json) before the header comment of the first accepted row above its entry, or into
`source` (a new draft-only file) when the unit has no module; `anchor` names the accepted row whose
compile group hosts it there. `// @ref` lines move from the text into the row's `references`.
"""
import argparse
import bisect
from collections import OrderedDict
from datetime import datetime, timezone
import json
import os
from pathlib import Path
import re
import sys

REPOSITORY = Path(__file__).resolve().parents[2]
sys.path.insert(0, str(Path(__file__).resolve().parent))

GUARD = 'RA2_NON_MATCHING'
MANIFEST = 'manifests/original_target/non_exact.json'
MARKER = re.compile(r'^//\s*Not exact \(RA2_NON_MATCHING\): (?P<name>.+?) at 0x(?P<entry>[0-9A-Fa-f]{8}), '
                    r'best score (?P<score>\d+(?:\.\d+)?), (?P<date>\d{4}-\d{2}-\d{2})(?:; differs: (?P<differs>.*))?\s*$')
PRELUDE = re.compile(r'^//\s*Not exact \(RA2_NON_MATCHING\) prelude: .+$')
PRELUDE_LINE = re.compile(r'^#\s*define\s+\w+$')
DIRECTIVE = re.compile(r'^[ \t]*#[ \t]*(\w+)(.*)$')
ASM = re.compile(r'(?<![\w$])(?:__asm|_asm|__emit|_emit)(?![\w$])')
BYTE_TABLE = re.compile(r'\{\s*(?:0[xX][0-9A-Fa-f]{1,2}\s*,\s*){15,}0[xX][0-9A-Fa-f]{1,2}\s*,?\s*\}')
STRING = re.compile(r'"(?:\\.|[^"\\\n])*"')
HEX_ESCAPE = re.compile(r'\\x[0-9A-Fa-f]')
REF_LINE = re.compile(r'^\s*//\s*@ref\s+([^\s=]+)=(?:0x)?([0-9a-fA-F]+)\s*$')


def mask(text):
    """Comments blanked (same length, newlines kept); literals kept."""
    tokens = r'"(?:\\.|[^"\\\n])*"|\'(?:\\.|[^\'\\\n])*\'|/\*.*?\*/|//[^\n]*'
    def blank(match):
        value = match[0]
        return re.sub(r'[^\n]', ' ', value) if value.startswith(('/*', '//')) else value
    return re.sub(tokens, blank, text, flags=re.S)


def guarded_blocks(text):
    """([(first, last)], [(line, problem)]): 0-based inclusive line spans of each
    `#ifdef RA2_NON_MATCHING` ... `#endif` block, and guard-form problems."""
    lines = mask(text).split('\n')
    blocks, problems, stack = [], [], []
    for n, line in enumerate(lines):
        m = DIRECTIVE.match(line)
        if not m:
            continue
        directive, rest = m.group(1), m.group(2).strip()
        mentions = re.search(r'\b%s\b' % GUARD, rest) is not None
        if directive in ('if', 'ifdef', 'ifndef'):
            is_guard = directive == 'ifdef' and rest == GUARD
            if mentions and not is_guard:
                problems.append((n, 'guard must be spelled "#ifdef %s"' % GUARD))
            if is_guard and any(g for g, _ in stack):
                problems.append((n, 'nested %s guard' % GUARD))
            stack.append((is_guard, n))
        elif directive in ('elif', 'else'):
            if mentions:
                problems.append((n, '#%s naming %s' % (directive, GUARD)))
            if stack and stack[-1][0]:
                problems.append((n, '#%s inside a %s block (the other branch would build exact)' % (directive, GUARD)))
        elif directive == 'endif':
            if not stack:
                problems.append((n, 'unmatched #endif'))
                continue
            is_guard, start = stack.pop()
            if is_guard:
                blocks.append((start, n))
        elif directive in ('define', 'undef') and mentions:
            problems.append((n, '%s defined or undefined in source (only a guard-on check defines it)' % GUARD))
    if any(g for g, _ in stack):
        problems.append((len(lines) - 1, 'unterminated %s block' % GUARD))
    return blocks, problems


def block_problems(text, first, last):
    """Instruction-array and inline-assembly problems of one guarded block (lines first..last)."""
    raw = '\n'.join(text.split('\n')[first + 1:last])
    masked = mask(raw)
    found = []
    if ASM.search(masked):
        found.append('inline assembly or __emit in a %s block' % GUARD)
    if BYTE_TABLE.search(masked):
        found.append('byte table (16+ byte-sized hex literals) in a %s block' % GUARD)
    for literal in STRING.findall(masked):
        if len(HEX_ESCAPE.findall(literal)) >= 8:
            found.append('string of \\x escapes in a %s block' % GUARD)
            break
    return found


FUNCTION_DECLARATION = re.compile(r'^(?!.*\bvirtual\b)(?!\s*(?:typedef|friend|using|class|struct|union|enum)\b)'
                                  r'[\w\s\*&:<>,~]+\([^;{}]*\)\s*(?:const\s*)?;$')


def header_block_problems(text, first, last):
    """A guarded block in a shared header holds only non-virtual function declarations (a guarded data
    member, virtual function, type or definition would change a class layout or the code under the guard)."""
    body = mask('\n'.join(text.split('\n')[first + 1:last]))
    statements = [' '.join(s.split()) + ';' for s in body.split(';')[:-1]]
    rest = body.split(';')[-1].strip()
    found = []
    if rest or '{' in body or '}' in body:
        found.append('%s block in a shared header holds more than declarations' % GUARD)
    for s in statements:
        if not FUNCTION_DECLARATION.match(s.strip()):
            found.append('%s block in a shared header: not a non-virtual function declaration: %s' % (GUARD, s.strip()[:80]))
    return found


def load_manifest(repository=REPOSITORY):
    path = Path(repository) / MANIFEST
    if not path.exists():
        return dict(about='', rows=[])
    return json.loads(path.read_text())


def write_manifest(manifest, repository=REPOSITORY):
    manifest['rows'].sort(key=lambda r: int(r['entry'], 16))
    (Path(repository) / MANIFEST).write_text(json.dumps(manifest, indent=1) + '\n')


def accepted_header(entry):
    """A `//` line naming an accepted row's address the way module header comments do."""
    return re.compile(r"^\s*//.*\(0x0*%X(?:\)\s*$|[,;] )" % entry, re.I)


def validate(repository, catalog, read=None):
    """Problems (list of strings) of the placed drafts against the tree and the catalog."""
    repository = Path(repository)
    read = read or (lambda path: (repository / path).read_text(errors='replace'))
    manifest = load_manifest(repository)
    problems = []
    regions = sorted((r['entry'], r['entry'] + r['full_extent_bytes'], r) for r in catalog['regions'])
    starts = [a for a, _, _ in regions]
    sources = {r['source'] for r in catalog['regions']}
    by_source = {}
    for r in catalog['regions']:
        by_source.setdefault(r['source'], []).append(r)
    rows, seen = {}, set()
    for row in manifest.get('rows', []):
        try:
            entry = int(row['entry'], 16)
            flag = row['non_exact']
            score, date, bytes_ = float(flag['score']), flag['date'], int(row['bytes'])
            source = row['source']
        except (KeyError, TypeError, ValueError) as error:
            problems.append('non_exact row malformed: %r (%s)' % (row.get('entry'), error)); continue
        if entry in seen:
            problems.append('non_exact row repeated: %08x' % entry); continue
        seen.add(entry)
        if not 0 <= score < 100:
            problems.append('%08x: non_exact score %s (an exact draft is accepted instead and loses its guard)' % (entry, score))
        if not re.fullmatch(r'\d{4}-\d{2}-\d{2}', str(date)):
            problems.append('%08x: non_exact date %r' % (entry, date))
        if bytes_ <= 0:
            problems.append('%08x: non_exact bytes %d' % (entry, bytes_))
        i = bisect.bisect_right(starts, entry + bytes_ - 1)
        for a, b, r in regions[max(0, i - 2):i]:
            if a < entry + bytes_ and entry < b:
                problems.append('%08x: guarded draft overlaps accepted region %08x (claims acceptance)' % (entry, a))
        if not source.startswith('src/recovered/') or not source.endswith('.cpp'):
            problems.append('%08x: draft source outside src/recovered: %s' % (entry, source))
        if row.get('anchor') is not None:
            anchor = int(row['anchor'], 16)
            if not any(a == anchor for a in starts[max(0, bisect.bisect_left(starts, anchor) - 1):bisect.bisect_left(starts, anchor) + 1]):
                problems.append('%08x: anchor %08x is no accepted region' % (entry, anchor))
        rows[entry] = row
    draft_sources = {r['source'] for r in rows.values()}
    files = sorted({p.relative_to(repository).as_posix() for p in (repository / 'src/recovered').rglob('*.cpp')} |
                   draft_sources)
    headers = sorted(p.relative_to(repository).as_posix() for folder in ('src/shared',)
                     for p in (repository / folder).rglob('*') if p.suffix in ('.hpp', '.h', '.inl'))
    markers = {}
    for path in headers:
        text = read(path)
        if GUARD not in text:
            continue
        blocks, form = guarded_blocks(text)
        problems += ['%s:%d: %s' % (path, n + 1, p) for n, p in form]
        for first, last in blocks:
            problems += ['%s:%d: %s' % (path, first + 1, p) for p in block_problems(text, first, last)]
            problems += ['%s:%d: %s' % (path, first + 1, p) for p in header_block_problems(text, first, last)]
    for path in files:
        if not (repository / path).exists():
            problems.append('draft source missing: %s' % path); continue
        text = read(path)
        if GUARD not in text and path not in draft_sources:
            continue
        lines = text.split('\n')
        blocks, form = guarded_blocks(text)
        problems += ['%s:%d: %s' % (path, n + 1, p) for n, p in form]
        inside, preludes = set(), 0
        for first, last in blocks:
            inside.update(range(first, last + 1))
            problems += ['%s:%d: %s' % (path, first + 1, p) for p in block_problems(text, first, last)]
            if first > 0 and PRELUDE.match(lines[first - 1]):
                # Header switches the unit's drafts need, defined only for the guard-on compile.
                body = [l.strip() for l in mask('\n'.join(lines[first + 1:last])).split('\n') if l.strip()]
                if not body or any(not PRELUDE_LINE.match(l) for l in body):
                    problems.append('%s:%d: prelude block holds more than #define SWITCH lines' % (path, first + 1))
                preludes += 1
                if preludes > 1 or path not in sources:
                    problems.append('%s:%d: a second prelude, or a prelude in a draft-only file' % (path, first + 1))
                continue
            m = MARKER.match(lines[first - 1]) if first > 0 else None
            if not m:
                problems.append('%s:%d: %s block without its marker line directly above' % (path, first + 1, GUARD)); continue
            entry = int(m.group('entry'), 16)
            markers.setdefault(entry, []).append(path)
            row = rows.get(entry)
            if row is None:
                problems.append('%s:%d: marker %08x has no non_exact row' % (path, first, entry)); continue
            if row['source'] != path:
                problems.append('%s:%d: marker %08x but the row names %s' % (path, first, entry, row['source']))
            if abs(float(m.group('score')) - float(row['non_exact']['score'])) > 0.005 or m.group('date') != row['non_exact']['date']:
                problems.append('%s:%d: marker %08x score/date differ from the manifest' % (path, first, entry))
        for r in by_source.get(path, ()):
            pattern = accepted_header(r['entry'])
            hits = [n for n, l in enumerate(lines) if pattern.match(l)]
            if hits and all(n in inside for n in hits):
                # The row's header comment only inside guarded text: its body is guarded (claims acceptance).
                problems.append('%s:%d: accepted row %08x inside a %s block' % (path, hits[0] + 1, r['entry'], GUARD))
        if path not in sources:
            # A draft-only file: guarded blocks, markers, comments and blank lines only.
            masked = mask(text).split('\n')
            stray = [n for n, l in enumerate(masked) if l.strip() and n not in inside and not MARKER.match(lines[n])]
            if stray:
                problems.append('%s:%d: draft-only source has text outside its %s blocks' % (path, stray[0] + 1, GUARD))
    for entry, row in rows.items():
        found = markers.get(entry, [])
        if len(found) != 1:
            problems.append('%08x: %d marker(s) for the draft (expected one in %s)' % (entry, len(found), row['source']))
    return problems


def draft_only_sources(repository=REPOSITORY, catalog=None):
    """Draft-only files (sources of non_exact rows that no catalog row names)."""
    manifest = load_manifest(repository)
    names = {r['source'] for r in catalog['regions']} if catalog else set()
    return {r['source'] for r in manifest.get('rows', []) if r['source'] not in names}


def summary(repository=REPOSITORY):
    """(functions, bytes, mean score) of the placed drafts."""
    rows = load_manifest(repository).get('rows', [])
    if not rows:
        return 0, 0, 0.0
    return len(rows), sum(int(r['bytes']) for r in rows), sum(float(r['non_exact']['score']) for r in rows) / len(rows)


# ---------------------------------------------------------------- placing


def unit_source(entry, catalog):
    """(module source, host accepted entry) of the Route A module holding `entry`'s unit, or (None, None)."""
    umap = json.loads((REPOSITORY / 'manifests/original_target/unit_map.json').read_text())['units']
    unit = next((u for u, addrs in umap.items() if '%08x' % entry in {a.lower().replace('0x', '').zfill(8) for a in addrs}), None)
    if unit is None:
        keys = sorted((int(a, 16), u) for u, addrs in umap.items() for a in addrs)
        i = bisect.bisect_left([k for k, _ in keys], entry)
        unit = keys[i - 1][1] if i else None
    path = REPOSITORY / 'manifests/original_target/modules' / ('%s.json' % unit) if unit else None
    if not path or not path.exists():
        return None, None
    source = json.loads(path.read_text())['source']
    host = min((r['entry'] for r in catalog['regions'] if r['source'] == source), default=None)
    return source, host


def insertion_line(text, entry, accepted_entries):
    """Line index where a draft at `entry` goes: before the comment block of the first accepted row
    above `entry` whose header the text names (outside guarded blocks), else the end."""
    lines = text.split('\n')
    blocks, _ = guarded_blocks(text)
    inside = {n for a, b in blocks for n in range(a - 1, b + 1)}
    best = None
    for e in sorted(a for a in accepted_entries if a > entry):
        pattern = accepted_header(e)
        at = next((n for n, l in enumerate(lines) if n not in inside and pattern.match(l)), None)
        if at is None:
            continue
        while at > 0 and lines[at - 1].lstrip().startswith('//') and (at - 1) not in inside:
            at -= 1
        best = at
        break
    if best is None:
        best = len(lines) if lines[-1] else len(lines) - 1
    return best


DEFINITION = re.compile(r'^(?![\s/#}])[^;]*?((?:\w+::)*~?\w+)\s*\(')


def defined_functions(text):
    """Qualified names of top-level function definitions (a column-0 declarator whose body opens before a `;`)."""
    lines, names = text.split('\n'), set()
    for n, line in enumerate(lines):
        m = DEFINITION.match(line)
        if not m or line.rstrip().endswith(';'):
            continue
        tail = '\n'.join(lines[n:n + 4])
        brace, semi = tail.find('{'), tail.find(';')
        if brace >= 0 and (semi < 0 or brace < semi):
            names.add(m.group(1))
    return names


def drop_definitions(text, names):
    """`text` without the top-level function definitions of `names` (and the comment lines just above)."""
    lines, out, n = text.split('\n'), [], 0
    while n < len(lines):
        m = DEFINITION.match(lines[n])
        if m and m.group(1) in names and not lines[n].rstrip().endswith(';'):
            depth, seen, k = 0, False, n
            while k < len(lines):
                depth += lines[k].count('{') - lines[k].count('}')
                seen = seen or '{' in lines[k]
                if seen and depth <= 0:
                    break
                k += 1
            while out and out[-1].lstrip().startswith('//'):
                out.pop()
            n = k + 1
            continue
        out.append(lines[n])
        n += 1
    return '\n'.join(out)


def clean_draft(text, context):
    """Draft text for a guarded block: @ref lines out (returned), lines the unit already has verbatim
    (#include, #define) dropped, function definitions the unit already holds (a lane's stand-in copies
    of accepted helpers) dropped, an `#undef` for each RA2_ switch the draft defines (so it cannot change
    the header state of the accepted code after it), trailing blank lines trimmed."""
    refs, out, switches = {}, [], []
    have = set(l.strip() for l in context.split('\n'))
    for line in text.replace('\r\n', '\n').split('\n'):
        m = REF_LINE.match(line)
        if m:
            refs[m.group(1)] = m.group(2).upper().zfill(8); continue
        if re.match(r'^\s*#\s*(include|define)\b', line) and line.strip() in have:
            continue
        s = re.match(r'^\s*#\s*define\s+(RA2_\w+)\s*$', line)
        if s and s.group(1) not in switches:
            switches.append(s.group(1))
        out.append(line.rstrip())
    body = drop_definitions('\n'.join(out), defined_functions(text) & defined_functions(context))
    out = body.split('\n') + ['#undef %s' % s for s in switches]
    while out and not out[-1].strip():
        out.pop()
    while out and not out[0].strip():
        out.pop(0)
    return '\n'.join(out), refs


def marker_line(name, entry, score, date, differs):
    line = '// Not exact (RA2_NON_MATCHING): %s at 0x%08X, best score %.2f, %s' % (name, entry, score, date)
    return line + ('; differs: %s' % differs if differs else '')


def add_prelude(text, switches):
    """The text with its prelude block (before the first #include) holding the switches as well."""
    lines = text.split('\n')
    blocks, _ = guarded_blocks(text)
    for first, last in blocks:
        if first > 0 and PRELUDE.match(lines[first - 1]):
            have = [l.strip() for l in lines[first + 1:last]]
            lines[last:last] = [s for s in switches if s not in have]
            return '\n'.join(lines)
    at = next((n for n, l in enumerate(lines) if re.match(r'^\s*#\s*include\b', l)), 0)
    block = ['// Not exact (RA2_NON_MATCHING) prelude: header switches the drafts below need (guard-on compile only)',
             '#ifdef %s' % GUARD] + switches + ['#endif', '']
    lines[at:at] = block
    return '\n'.join(lines)


def place(list_path, date):
    import catalog_io
    catalog = catalog_io.load(catalog_io.locate(REPOSITORY), references=False)
    accepted = {r['entry'] for r in catalog['regions']}
    manifest = load_manifest()
    rows = {int(r['entry'], 16): r for r in manifest['rows']}
    texts, preludes = {}, {}
    for item in json.loads(Path(list_path).read_text()):
        entry = int(str(item['entry']), 16)
        if entry in accepted or entry in rows:
            print('skip %08x: accepted or already placed' % entry); continue
        source, host = unit_source(entry, catalog)
        if item.get('source'):
            source, host = item['source'], None
        if item.get('anchor'):
            host = int(str(item['anchor']), 16)
        if source is None or host is None:
            print('skip %08x: no module and no source/anchor given' % entry); continue
        path = REPOSITORY / source
        text = texts.get(source)
        if text is None:
            text = path.read_text() if path.exists() else ''
        draft = Path(item['draft']).read_text(errors='replace')
        pre_switches = []
        if item.get('pre'):
            # A lane PRE file: header switches go to the unit's prelude, anything else heads the draft.
            rest = []
            for line in Path(item['pre']).read_text(errors='replace').split('\n'):
                (pre_switches if PRELUDE_LINE.match(line.strip()) else rest).append(line.strip() if PRELUDE_LINE.match(line.strip()) else line)
            draft = '\n'.join(rest).strip('\n') + '\n' + draft
            if pre_switches and source not in {r['source'] for r in catalog['regions']}:
                # A draft-only file has no include block of its own: the switches head the draft.
                draft = '\n'.join(pre_switches) + '\n' + draft
                pre_switches = []
        preludes.setdefault(source, [])
        preludes[source] += [s for s in pre_switches if s not in preludes[source]]
        body, refs = clean_draft(draft, text)
        refs.update(item.get('references') or {})
        name = item.get('name') or 'FUN_%08x' % entry
        score = float(item.get('score', 0.0))
        block = [marker_line(name, entry, score, date, item.get('differs')), '#ifdef %s' % GUARD, body, '#endif', '']
        lines = text.split('\n') if text else []
        if text:
            at = insertion_line(text, entry, {r['entry'] for r in catalog['regions'] if r['source'] == source})
            if at > 0 and lines[at - 1].strip():
                block.insert(0, '')
            lines[at:at] = block
        else:
            lines = block
        texts[source] = '\n'.join(lines)
        rows[entry] = dict(entry='%08X' % entry, name=name, bytes=int(item['bytes']), source=source,
                           anchor='%08X' % host, symbol=item.get('symbol'), references=refs,
                           non_exact=dict(score=score, date=date, differs=item.get('differs') or ''),
                           draft_origin=item.get('origin') or '')
        print('placed %08x in %s' % (entry, source))
    for source, switches in preludes.items():
        if switches:
            texts[source] = add_prelude(texts[source], switches)
    for source, text in texts.items():
        (REPOSITORY / source).parent.mkdir(parents=True, exist_ok=True)
        (REPOSITORY / source).write_text(text if text.endswith('\n') else text + '\n')
    manifest['rows'] = list(rows.values())
    manifest.setdefault('about', ABOUT)
    write_manifest(manifest)


ABOUT = ('Non-exact drafts placed in source under #ifdef RA2_NON_MATCHING (user decision 2026-11-13): '
         'entry, original body bytes, source, the accepted row whose compile group hosts the draft (anchor), '
         'scored symbol, @ref bindings, non_exact flag (score of the placed text in a guard-on compile, date, '
         'what differs). No .text credit; tools/original_target/non_exact.py audits and checks them.')


# ---------------------------------------------------------------- guard-on check


def groups_for(catalog, manifest, only=None):
    """{group key: (sources in source order, [non_exact rows])} for every compile group hosting a draft."""
    from canonical import compile_group_key, separate_unit_populations
    separate = separate_unit_populations(catalog)
    by_entry = {r['entry']: r for r in catalog['regions']}
    group_rows = OrderedDict()
    for row in sorted(catalog['regions'], key=lambda r: r['source_order']):
        group_rows.setdefault(compile_group_key(row, separate), []).append(row)
    key_of = {r['entry']: k for k, rs in group_rows.items() for r in rs}
    out = OrderedDict()
    for row in manifest['rows']:
        if only and row['source'] not in only:
            continue
        host = by_entry.get(int(row['anchor'], 16))
        if host is None:
            raise SystemExit('%s: anchor %s is not accepted' % (row['entry'], row['anchor']))
        key = key_of[host['entry']]
        if key not in out:
            out[key] = (list(OrderedDict.fromkeys(r['source'] for r in group_rows[key])), [], group_rows[key])
        out[key][1].append(row)
    for key, (sources, rows, members) in out.items():
        for row in rows:
            if row['source'] in sources:
                continue
            # A draft-only file joins the group in address order among the group's sources.
            entry = int(row['entry'], 16)
            first = {s: min(r['entry'] for r in members if r['source'] == s) for s in sources}
            k = next((i for i, s in enumerate(sources) if first.get(s, 0) > entry), len(sources))
            sources.insert(k, row['source'])
    return out


def differs_from(result):
    """A short what-differs phrase from a scored result (instruction counts and shape score)."""
    original, candidate = (result.get('insns') or '?/?').split('/')
    shape = result.get('shape')
    if original == candidate:
        if shape is not None and shape >= 99.995:
            return 'same instructions; registers, stack slots or operands differ'
        return 'same instruction count (%s), shape %.2f: instruction choice and registers differ' % (original, shape or 0)
    return '%s instructions in the original, %s in the draft (shape %.2f)' % (original, candidate, shape or 0)


def check(config_path, output, only=None, update=False):
    import build, canonical, catalog_io, diff_candidate as dc
    from coff import Coff
    import capstone
    from verify_manifest import Audit
    from source_paths import source_path
    config = json.loads(Path(config_path).read_bytes())
    catalog = catalog_io.load(catalog_io.locate(REPOSITORY), references=False)
    manifest = load_manifest()
    accepted_symbols = {r['symbol'] for r in catalog['regions'] if r.get('symbol')}
    groups = groups_for(catalog, manifest, only)
    output = Path(output).resolve()
    output.mkdir(parents=True, exist_ok=True)
    units, plan = [], []
    for n, (key, (sources, rows, members)) in enumerate(groups.items()):
        d = output / ('group-%03d' % n)
        d.mkdir(exist_ok=True)
        for p in ('guarded.obj', 'returncode.txt', 'compile.log'):
            (d / p).unlink(missing_ok=True)
        text = b'\n'.join(source_path(REPOSITORY, s).read_bytes() for s in sources)
        (d / 'guarded.cpp').write_bytes(text)
        args = canonical.compiler_arguments(list(key[1])) + ['/D' + GUARD]
        units.append(dict(cwd=d, source=d / 'guarded.cpp', obj=d / 'guarded.obj', log=d / 'compile.log',
                          returncode=d / 'returncode.txt', args=args))
        plan.append((key, rows, d))
    if units:
        build.compile_units(config, output, units, 'guarded-compile')
    audit = Audit(REPOSITORY, config['executable'])
    decoder = capstone.Cs(capstone.CS_ARCH_X86, capstone.CS_MODE_32)
    refs_base = dc.known_references(catalog)
    for name, value in audit.original_type_descriptors().items():
        refs_base.setdefault(name, value)
    failures, results = [], []
    for key, rows, d in plan:
        obj = d / 'guarded.obj'
        if not obj.exists():
            log = (d / 'compile.log').read_text(errors='replace') if (d / 'compile.log').exists() else ''
            errors = [l.strip()[-220:] for l in log.splitlines() if ' error ' in l or 'fatal error' in l][:6]
            failures.append('%s: guard-on compile failed (%s): %s' % (key[0], ', '.join(r['entry'] for r in rows), errors))
            for row in rows:
                results.append(dict(entry=row['entry'], status='compile-failed', errors=errors))
            continue
        refs = dict(refs_base)
        pj = REPOSITORY / 'manifests/original_target/populations' / ('%s.json' % key[0])
        if pj.exists():
            for k, v in json.loads(pj.read_text()).get('references', {}).items():
                refs[k] = v if isinstance(v, int) else int(v, 16)
        coff = Coff(obj.read_bytes())
        functions = [s['name'] for s in coff.symbols.values() if s['section'] > 0 and s['type'] & 0x20 and
                     s['storage'] == 2 and coff.sections[s['section'] - 1]['flags'] & 0x20]
        claimed = {row['symbol'] for row in rows if row.get('symbol')}
        for row in rows:
            entry, length = int(row['entry'], 16), int(row['bytes'])
            local = dict(refs)
            for k, v in (row.get('references') or {}).items():
                local[k] = int(v, 16)
            prefix = row.get('symbol')
            if prefix:
                # A full mangling may be stale after a type re-spelling: fall back to name and scope.
                unaccepted = [f for f in functions if f not in accepted_symbols]
                candidates = ([f for f in unaccepted if f.startswith(prefix)] or
                              [f for f in unaccepted if f.startswith(prefix.split('@@')[0] + '@@')])
            else:
                candidates = [f for f in functions if f not in accepted_symbols and
                              not any(f.startswith(c) for c in claimed)]
            original_bytes = audit.original(entry, length)
            original = dc.disassemble(decoder, original_bytes, entry)
            best = None
            for c in candidates:
                try:
                    symbol, section, start, end = coff.function(c)
                except Exception:
                    continue
                relocs = [(o - start, s, k) for o, s, k in section['relocs'] if start <= o < end]
                data, unresolved = dc.resolve(section['data'][start:end], relocs, coff.symbols, entry, local)
                body, _ = dc.split_padding(data, length)
                summary_, pairs = dc.score(original, dc.disassemble(decoder, body, entry, unresolved))
                exact = not unresolved and body == original_bytes
                r = dict(symbol=symbol['name'], exact=exact, **summary_)
                if best is None or (r['exact'], r['score']) > (best[0]['exact'], best[0]['score']):
                    best = (r, pairs)
            if best is None:
                failures.append('%s: no scored symbol (%s)' % (row['entry'], prefix or 'any'))
                results.append(dict(entry=row['entry'], status='no-symbol')); continue
            r, pairs = best
            (d / ('diff_%s.txt' % row['entry'])).write_text(dc.render(pairs, context=None))
            results.append(dict(entry=row['entry'], status='EXACT' if r['exact'] else 'miss', symbol=r['symbol'],
                                score=r['score'], shape=r.get('shape_score'), loose=r.get('loose_score'),
                                insns='%s/%s' % (r.get('original_instructions'), r.get('candidate_instructions'))))
            if not row.get('symbol') and update:
                claimed.add(r['symbol'])
    (output / 'results.json').write_text(json.dumps(results, indent=1))
    for r in results:
        print(json.dumps(r))
    if update:
        by_entry = {r['entry']: r for r in results if r.get('score') is not None}
        texts = {}
        for row in manifest['rows']:
            r = by_entry.get(row['entry'])
            if r is None:
                continue
            row['symbol'] = row.get('symbol') or r['symbol']
            old = float(row['non_exact']['score'])
            row['non_exact']['score'] = round(float(r['score']), 2)
            row['non_exact']['differs'] = row['non_exact'].get('differs') or differs_from(r)
            path = row['source']
            text = texts.setdefault(path, (REPOSITORY / path).read_text())
            lines = text.split('\n')
            for n, l in enumerate(lines):
                m = MARKER.match(l)
                if m and int(m.group('entry'), 16) == int(row['entry'], 16):
                    lines[n] = marker_line(m.group('name'), int(row['entry'], 16), row['non_exact']['score'],
                                           row['non_exact']['date'], row['non_exact'].get('differs'))
            texts[path] = '\n'.join(lines)
        for path, text in texts.items():
            (REPOSITORY / path).write_text(text)
        write_manifest(manifest)
    for f in failures:
        print('FAIL', f, file=sys.stderr)
    return not failures


def main():
    parser = argparse.ArgumentParser(description=__doc__, formatter_class=argparse.RawDescriptionHelpFormatter)
    sub = parser.add_subparsers(dest='command', required=True)
    sub.add_parser('audit')
    c = sub.add_parser('check')
    c.add_argument('--sources', nargs='*')
    c.add_argument('--output', type=Path, default=REPOSITORY / 'build/non-exact-check')
    c.add_argument('--update', action='store_true')
    c.add_argument('--config', type=Path, default=REPOSITORY / 'private/original-target.local.json')
    p = sub.add_parser('place')
    p.add_argument('list', type=Path)
    p.add_argument('--date', default=datetime.now(timezone.utc).strftime('%Y-%m-%d'))
    args = parser.parse_args()
    os.chdir(REPOSITORY)
    if args.command == 'audit':
        import catalog_io
        catalog = catalog_io.load(catalog_io.locate(REPOSITORY), references=False)
        problems = validate(REPOSITORY, catalog)
        print('\n'.join(problems) or 'non_exact: %d draft(s), %d bytes, mean score %.2f; no problems' % summary())
        sys.exit(1 if problems else 0)
    if args.command == 'check':
        sys.exit(0 if check(args.config, args.output, args.sources, args.update) else 1)
    if args.command == 'place':
        place(args.list, args.date)


if __name__ == '__main__':
    main()
