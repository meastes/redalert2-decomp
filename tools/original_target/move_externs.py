"""move_externs.py UNIT... [--dry]: conform a unit's stray extern declarations (user, 2026-11-17: externs
belong in headers). Each file-scope `extern` line of src/UNIT.cpp is removed; when no header of the
rebuild tree declares the name yet, the declaration moves to its home header: the header of the unit that
defines the name in src, else the header of the module mischa85's maps put its address in, else
globals.h (the tree's externs header). The unit then includes that header. Lines that are not one complete
declaration are reported and left (lane R2)."""
import csv, re, sys
from pathlib import Path
import rebuild as R
import cleanup_census as C

INC = R.ROOT / 'include'
SRC = R.ROOT / 'src'
LINE = re.compile(r'^extern\s+(?!"C"\s*\{)(.*;)\s*(//.*)?$')


def header_texts():
    return {p: p.read_text(errors='replace') for p in INC.rglob('*.h')}


SIGNATURE_DROP = {'extern', 'static', 'inline', 'virtual', '__fastcall'}    # /Gr: __fastcall is the default


def signature(decl, name, texts):
    """The type shape of one declaration of NAME: builtin words, known type names (rebuild headers or
    Windows SDK style) and the punctuation that changes a type, with NAME, parameter names and storage
    words left out, so `Point2D Lerp(Point2D const & a, ...)` and `RGBClass & Lerp(RGBClass const & a,
    ...)` differ while the same declaration with other parameter names does not (lane R5)."""
    idx = type_index(texts)
    out = []
    for tok in re.findall(r'[A-Za-z_]\w*|[*&(),\[\]]', decl.replace(name, ' ', 1)):
        if tok in SIGNATURE_DROP:
            continue
        if tok == 'wchar_t':                # VC6: a typedef of unsigned short
            out += ['unsigned', 'short']; continue
        if tok in BUILTIN or tok in idx or re.fullmatch(r'(H|LP|DW|W|L|U)?[A-Z][A-Z0-9_]+', tok) or not tok[0].isalpha():
            out.append(tok)
    return ' '.join(out)


def line_scopes(code):
    """For CODE (comments and literals blanked), the enclosing scopes at the start of each line: a tuple
    of namespace names, with None standing for any other block (a class, function or initializer);
    extern "C" blocks are transparent (lane R5)."""
    out, stack, line = [()], [], 0
    for i, ch in enumerate(code):
        if ch == '\n':
            out.append(tuple(stack))
        elif ch == '{':
            head = code[max(0, i - 200):i]
            head = head[max(head.rfind(';'), head.rfind('}'), head.rfind('{')) + 1:]
            m = re.search(r'\bnamespace\s+([A-Za-z_]\w*)\s*$', head)
            stack.append(m.group(1) if m else 'C' if re.search(r'\bextern\s*$', head) else None)
        elif ch == '}' and stack:
            stack.pop()
    return [tuple(x for x in sc if x != 'C') for sc in out]


def declared_in(texts, name, decl=None, scope=()):
    """Headers that declare NAME in namespace SCOPE (file scope by default; a class member is never this
    declaration); with DECL, only those whose declaration has DECL's type shape (a same-named
    declaration of another type, such as an overload, is not this one)."""
    pat = re.compile(r'^(?!\s*//)[^\n;{}()]*\b%s\b\s*(\[[^\]\n]*\]\s*)*(\([^;{}]*\))?\s*(const\s*)?;' % re.escape(name), re.M)
    found = []
    for p, t in texts.items():
        code = C.code_only(t)
        hits = list(pat.finditer(code))
        if not hits and not scope:
            continue
        scopes = line_scopes(code)
        hits = [h.group(0) for h in hits if scopes[code.count('\n', 0, h.start())] == tuple(scope)]
        if len(scope) == 1:                 # the one-line form: namespace N { extern ...; }
            hits += re.findall(r'\bnamespace\s+%s\s*\{([^{}]*\b%s\b[^{}]*;)\s*\}' % (scope[0], re.escape(name)), code)
        if hits and (decl is None or any(signature(h, name, texts) == signature(decl, name, texts) for h in hits)):
            found.append(p)
    return found


def defining_unit(name, skip):
    pat = re.compile(r'^(?!extern)(?!\s)[^\n;()=]*\b%s\s*(\[[^\]\n]*\]\s*)*(=|;|\([^;]*\)\s*(\{|$))' % re.escape(name), re.M)
    for p in sorted(SRC.glob('*.cpp')):
        if p.stem != skip and pat.search(C.code_only(p.read_text(errors='replace'))):
            return p.stem


def defines(text, name):
    pat = re.compile(r'^(?!extern)(?![ \t])[^\n;()=]*\b%s\s*(\[[^\]\n]*\]\s*)*(=|;|\{)' % re.escape(name), re.M)
    return bool(pat.search(C.code_only(text)))


def module_of(name):
    if not hasattr(module_of, 'map'):
        names = {}
        for r in csv.DictReader(open(R.MISCHA / 'config/ra2-1.006/names.csv')):
            if re.match(r'\?[A-Za-z_]\w*@@[23]', r['symbol']):
                continue        # a variable: its data address lies in no code module (lane R5)
            m = re.match(r'\?([A-Za-z_]\w*)@@|_?([A-Za-z_]\w*)$', r['symbol'])
            if m:
                names.setdefault(m.group(1) or m.group(2), int(r['address'], 16))
        mods = [(int(m['start'], 16), int(m['end'], 16), m['unit'])
                for m in csv.DictReader(open(R.MISCHA / 'config/ra2-1.006/modules.csv'))]
        module_of.map = (names, mods)
    names, mods = module_of.map
    a = names.get(name)
    return next((u for s, e, u in mods if a is not None and s <= a < e), None)


BUILTIN = set('const volatile unsigned signed int char short long bool void float double wchar_t __int64 struct class '
              'enum extern static __fastcall __cdecl __stdcall WINAPI CALLBACK inline C'.split())


def type_index(texts):
    """name -> (kind, header, text) for the class/struct/union, template, enum and typedef definitions."""
    if hasattr(type_index, 'cache'):
        return type_index.cache
    idx = {}
    for p in sorted(INC.rglob('*')):
        if p.suffix not in ('.h', '.hh'):
            continue
        code = C.code_only(texts.get(p) or p.read_text(errors='replace'))
        for m in re.finditer(r'(template\s*<[^>]*>\s*)?\b(class|struct|union)\s+([A-Za-z_]\w*)[^;{()]*\{', code):
            idx.setdefault(m.group(3), ('template' if m.group(1) else m.group(2), p, ' '.join(m.group(1).split()) if m.group(1) else ''))
        for m in re.finditer(r'\bnamespace\s+([A-Za-z_]\w*)\s*\{', code):
            idx.setdefault(m.group(1), ('namespace', p, ''))
        for m in re.finditer(r'\benum\s+([A-Za-z_]\w*)\s*(:[^{]*)?\{', code):
            idx.setdefault(m.group(1), ('enum', p, ''))
        for m in re.finditer(r'\btypedef\b[^;{]*?\b([A-Za-z_]\w*)\s*(\[[^\]]*\])?\s*;', code):
            idx.setdefault(m.group(1), ('typedef', p, ''))
    type_index.cache = idx
    return idx


def closure(header, texts, seen=None):
    seen = set() if seen is None else seen
    if header in seen or not header.exists():
        return seen
    seen.add(header)
    for inc in re.findall(r'#include\s+"([^"]+)"', texts.get(header) or header.read_text(errors='replace')):
        closure(INC / inc.replace('\\', '/'), texts, seen)
    return seen


def type_home(decl, name, texts):
    """(header, forward declarations) for a declaration with no home: globals.h with forward declarations
    for its class and template types; for an enum or typedef globals.h does not see, the header that
    defines that type (or globals.h with the enum header included, for a .hh enum header)."""
    idx = type_index(texts)
    glob = INC / 'globals.h'
    seen = closure(glob, texts)
    head = decl.split('(')[0]
    head = head[:head.rfind(name)] if name in head else head
    rest = decl.replace(name, ' ', 1)
    for owner in re.findall(r'\b([A-Za-z_]\w*)\s*::', rest):
        # A nested type (OwnerDraw::WinData) cannot be declared ahead: the header that defines its
        # class is the home (lane R5).
        kind, where, tmpl = idx.get(owner, (None, None, None))
        if kind in ('class', 'struct', 'union', 'template', 'namespace'):
            return where, forwards_for(decl, name, where, texts)
    idents = [w for w in re.findall(r'(?<!::)\b[A-Za-z_]\w*', rest) if w not in BUILTIN]
    forwards = []
    for w in idents:
        kind, where, tmpl = idx.get(w, (None, None, None))
        if kind is None and re.fullmatch(r'(H|LP|DW|W|L|U)?[A-Z][A-Z0-9_]+', w):
            return INC / 'win.h', []        # a Windows SDK type: the header that includes <windows.h>
        if kind is None or where in seen:
            continue
        if kind in ('class', 'struct', 'union'):
            forwards.append('%s %s;' % (kind, w))
        elif kind == 'template':
            forwards.append('%s class %s;' % (tmpl, w))
        elif where.suffix == '.hh':
            forwards.append('#include "%s"' % where.relative_to(INC).as_posix())
        else:
            return where, []
    return glob, forwards


def forwards_for(decl, name, home, texts):
    """Forward declarations a header HOME needs for DECL: its class and template types that HOME and
    its includes do not define (lane R5; type_home does the same for globals.h)."""
    idx = type_index(texts)
    seen = closure(home, texts)
    out = []
    for w in re.findall(r'(?<!::)\b[A-Za-z_]\w*', decl.replace(name, ' ', 1)):
        kind, where, tmpl = idx.get(w, (None, None, None))
        if kind is None or where in seen:
            continue
        if kind in ('class', 'struct', 'union'):
            out.append('%s %s;' % (kind, w))
        elif kind == 'template':
            out.append('%s class %s;' % (tmpl, w))
    return out


def ensure_include(text, header):
    rel = header.relative_to(INC).as_posix()
    if re.search(r'#include\s+"%s"' % re.escape(rel), text):
        return text
    # After the last include of the file's head: an include standing after code (a late include
    # near the end of a file) would come after the uses it is meant to serve (lane R5).
    code = C.code_only(text)                # line structure kept, offsets not
    brace_line = code.count('\n', 0, code.find('{')) if '{' in code else None
    last = [m for m in re.finditer(r'^#include[^\n]*\n', text, re.M)
            if brace_line is None or text.count('\n', 0, m.start()) < brace_line]
    at = last[-1].end() if last else 0
    return text[:at] + '#include "%s"\n' % rel + text[at:]


def main(units, dry):
    texts = header_texts()
    for unit in units:
        path = SRC / (unit + '.cpp')
        text = path.read_text(errors='replace')
        out, moved, left, includes = [], [], [], set()
        scopes = line_scopes(C.code_only(text))
        for number, line in enumerate(text.split('\n')):
            m = LINE.match(line)
            scope = scopes[number] if number < len(scopes) else ()
            if line.startswith('extern') and None in scope:
                left.append(line + '   [inside a block]'); out.append(line); continue
            if not line.startswith('extern') or line.startswith('extern "C" {') or line.startswith('extern "C"{'):
                out.append(line); continue
            if not m:
                left.append(line); out.append(line); continue
            decl = m.group(1)
            hit = C.DECLARED_NAME.search(decl.split('=')[0].rstrip().rstrip(';') + ';')
            if '=' in decl.split('(')[0] or not hit:      # a definition (extern const with a value) keeps its line
                left.append(line); out.append(line); continue
            name = hit.group(1)
            if defines(text, name):
                # A forward declaration of this file's own global (used before its definition): it
                # stays, and the census counts it apart.
                left.append(line + '   [own global]'); out.append(line); continue
            where = declared_in(texts, name, decl, scope)
            if where:
                home = where[0]
            elif scope:
                # A namespace member (namespace OwnerDraw { extern ...; }): the header that opens the
                # namespace takes it in the same one-line form (lane R5).
                kind, home, _ = type_index(texts).get(scope[-1], (None, None, None))
                if kind != 'namespace' or len(scope) != 1:
                    left.append(line + '   [namespace %s]' % '::'.join(scope)); out.append(line); continue
                body = texts[home].rstrip('\n') + '\n'
                if '// Declarations moved from the units that used them' not in body:
                    body += '\n// Declarations moved from the units that used them (one declaration per name).\n'
                texts[home] = body + 'namespace %s { extern %s }' % (scope[0], decl) + (' ' + m.group(2) if m.group(2) else '') + '\n'
            else:
                owner = defining_unit(name, unit)
                cand = [INC / (owner + '.h')] if owner else []
                mod = module_of(name)
                cand += [INC / (mod + '.h')] if mod else []
                home = next((c for c in cand if c.exists() and c.stem != unit), None)
                forwards = []
                if home is None:
                    home, forwards = type_home(decl, name, texts)
                else:
                    forwards = forwards_for(decl, name, home, texts)
                body = texts[home].rstrip('\n') + '\n'
                for fwd in forwards:
                    if fwd not in body:
                        marker = '// Declarations moved from the units that used them'
                        at = body.find(marker)
                        body = (body[:at] + fwd + '\n' + body[at:]) if at >= 0 else body + fwd + '\n'
                if '// Declarations moved from the units that used them' not in body:
                    body += '\n// Declarations moved from the units that used them (one declaration per name).\n'
                texts[home] = body + 'extern ' + decl + (' ' + m.group(2) if m.group(2) else '') + '\n'
            includes.add(home)
            moved.append('%s -> %s%s' % (name, home.relative_to(INC), '' if not where else ' (declared there)'))
        new = '\n'.join(out)
        for h in sorted(includes):
            if h.stem != unit:
                new = ensure_include(new, h)
        print('%s: %d moved, %d left' % (unit, len(moved), len(left)))
        for x in moved: print('   ', x)
        for x in left: print('    left:', x[:100])
        if not dry:
            path.write_text(new)
    if not dry:
        for p, t in texts.items():
            if p.read_text(errors='replace') != t:
                p.write_text(t)


if __name__ == '__main__':
    args = [a for a in sys.argv[1:] if a != '--dry']
    main(args, '--dry' in sys.argv)
