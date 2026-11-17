"""move_externs.py UNIT... [--dry]: conform a unit's stray extern declarations (user, 2026-11-17: externs
belong in headers). Each file-scope `extern` line of rebuild/src/UNIT.cpp is removed; when no header of the
rebuild tree declares the name yet, the declaration moves to its home header: the header of the unit that
defines the name in rebuild/src, else the header of the module mischa85's maps put its address in, else
globals.h (the tree's externs header). The unit then includes that header. Lines that are not one complete
declaration are reported and left (lane R2)."""
import csv, re, sys
from pathlib import Path
import rebuild as R
import cleanup_census as C

INC = R.REBUILD / 'include'
SRC = R.REBUILD / 'src'
LINE = re.compile(r'^extern\s+(?!"C"\s*\{)(.*;)\s*(//.*)?$')


def header_texts():
    return {p: p.read_text(errors='replace') for p in INC.rglob('*.h')}


def declared_in(texts, name):
    pat = re.compile(r'^(?!\s*//)[^\n;{}()]*\b%s\b\s*(\[[^\]\n]*\]\s*)*(\([^;{}]*\))?\s*(const\s*)?;' % re.escape(name), re.M)
    return [p for p, t in texts.items() if pat.search(C.code_only(t))]


def defining_unit(name, skip):
    pat = re.compile(r'^(?!extern)(?!\s)[^\n;()=]*\b%s\s*(\[[^\]\n]*\]\s*)*(=|;|\([^;]*\)\s*(\{|$))' % re.escape(name), re.M)
    for p in sorted(SRC.glob('*.cpp')):
        if p.stem != skip and pat.search(C.code_only(p.read_text(errors='replace'))):
            return p.stem


def module_of(name):
    if not hasattr(module_of, 'map'):
        names = {}
        for r in csv.DictReader(open(R.MISCHA / 'config/ra2-1.006/names.csv')):
            m = re.match(r'\?([A-Za-z_]\w*)@@|_?([A-Za-z_]\w*)$', r['symbol'])
            if m:
                names.setdefault(m.group(1) or m.group(2), int(r['address'], 16))
        mods = [(int(m['start'], 16), int(m['end'], 16), m['unit'])
                for m in csv.DictReader(open(R.MISCHA / 'config/ra2-1.006/modules.csv'))]
        module_of.map = (names, mods)
    names, mods = module_of.map
    a = names.get(name)
    return next((u for s, e, u in mods if a is not None and s <= a < e), None)


def ensure_include(text, header):
    rel = header.relative_to(INC).as_posix()
    if re.search(r'#include\s+"%s"' % re.escape(rel), text):
        return text
    last = list(re.finditer(r'^#include[^\n]*\n', text, re.M))
    at = last[-1].end() if last else 0
    return text[:at] + '#include "%s"\n' % rel + text[at:]


def main(units, dry):
    texts = header_texts()
    for unit in units:
        path = SRC / (unit + '.cpp')
        text = path.read_text(errors='replace')
        out, moved, left, includes = [], [], [], set()
        for line in text.split('\n'):
            m = LINE.match(line)
            if not line.startswith('extern') or line.startswith('extern "C" {') or line.startswith('extern "C"{'):
                out.append(line); continue
            if not m:
                left.append(line); out.append(line); continue
            decl = m.group(1)
            hit = C.DECLARED_NAME.search(decl.split('=')[0].rstrip().rstrip(';') + ';')
            if '=' in decl.split('(')[0] or not hit:      # a definition (extern const with a value) keeps its line
                left.append(line); out.append(line); continue
            name = hit.group(1)
            where = declared_in(texts, name)
            if where:
                home = where[0]
            else:
                owner = defining_unit(name, unit)
                cand = [INC / (owner + '.h')] if owner else []
                mod = module_of(name)
                cand += [INC / (mod + '.h')] if mod else []
                home = next((c for c in cand if c.exists() and c.stem != unit), INC / 'globals.h')
                body = texts[home].rstrip('\n') + '\n'
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
