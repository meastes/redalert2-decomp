#!/usr/bin/env python3
"""Generate the native (arm64, clang) view of the pinned Yuri's Revenge tree (lane Y1).

The tree is ra2-decomp's (yr/ra2-decomp, pinned, GPL-3.0-or-later; THIRD_PARTY.md). Its
build compiles each src/*.cpp as one translation unit against ONE header directory: the
TibSun archive's headers (yr/tibsun/code), ra2-decomp's include/ over them (their
tools/check.py prepare_headers). This tool never edits yr/; it writes:

  OUT/hdr/...             that header directory after the port rewrites
  OUT/units/<name>.cpp    one native TU per yr/ra2-decomp/src/**/*.cpp, port-rewritten
  OUT/units.cmake         YR_NATIVE_UNITS for CMake
  OUT/report.json         the units and what was skipped

The port rewrites are the RA2 native tree's (native/tools/native_units.py port(): long ->
RA2_LONG, literal L suffixes, operator new sizes, pointer-width Win32 typedefs, ...).

    python3 native/yr/tools/yr_units.py --out build/yr-native/gen
"""
import argparse
import json
import re
import sys
from pathlib import Path

ROOT = Path(__file__).resolve().parents[3]
sys.path.insert(0, str(ROOT / 'native/tools'))
import native_units  # noqa: E402

# Window and dialog procedures (and the handlers they pass their parameters to) take the message
# parameters pointer-sized (WPARAM/LPARAM), as the RA2 tree's (native_units.py --win64-params): WM_CREATE,
# WM_NOTIFY, WM_DRAWITEM and dialog init parameters carry pointers. The YR tree also spells the
# last one LONG, which the RA2 pattern does not take.
native_units.WIN64_PARAMS = False   # the RA2 pattern's uintptr_t/intptr_t are not mingw's WPARAM/LPARAM types
YR_MSG_PARAMS = re.compile(r'\(\s*((?:struct\s+)?HWND(?:__)?\s*\*?\s*\w*)\s*,'
                           r'\s*(?:unsigned\s+int|unsigned|UINT)\b(\s*&?\s*\w*)\s*,'
                           r'\s*(?:unsigned\s+int|unsigned|UINT|WPARAM)\b(\s*&?\s*\w*)\s*,'
                           r'\s*(?:LONG|long|LPARAM)\b(\s*&?\s*\w*)\s*\)')

TREE = ROOT / 'yr/ra2-decomp'
TIBSUN = ROOT / 'yr/tibsun/code'
HEADER_SUFFIXES = ('.h', '.hh', '.hpp', '.inl')


def read(path):
    return path.read_text(encoding='latin-1')


FIXUPS_PATH = ROOT / 'native/yr/fixups.json'


def load_fixups():
    rows = json.loads(FIXUPS_PATH.read_text())['fixups']
    for r in rows:
        r['hits'] = 0
        if r.get('skip_unit') or 'append' in r:
            r['rx'] = None
            continue
        if 'cut' in r:
            r['rx'] = re.compile(r'^[ \t]*//[ \t]*FUNCTION:[ \t]*GAMEMD[ \t]+%s\b[\s\S]*?(?=^[ \t]*//[ \t]*(?:FUNCTION|TEMPLATE|SYNTHETIC|LIBRARY):|\Z)'
                                 % re.escape(r['cut']), re.M | re.I)
            r['new'] = '// native: %s cut (native/yr/fixups.json; native/yr/glue)\n' % r['cut']
        else:
            r['rx'] = re.compile(r['old'] if r.get('regex') else re.escape(r['old']))
        r['hits'] = 0
    return rows


# The YR tree calls the Win32 macros SetWindowLong/GetWindowLong (the A forms); the RA2 tree's port
# rewrites (native_units.py) recognise the A spelling when they make window-data storage
# pointer-sized.
WINDOW_LONG = re.compile(r'\b(Set|Get)WindowLong(?=\s*\()')


# VC6 (/Ze) puts a for-init declaration in the enclosing scope (native_units.py FOR_INIT hoists it
# when the for follows ; { or } on the same or the next line); the YR tree also has blank lines
# and comment lines between them.
FOR_INIT = re.compile(r'([;{}](?:[ \t]*(?://[^\n]*)?\n)*[ \t]*)for[ \t]*\([ \t]*'
                      r'((?:const[ \t]+)?(?:unsigned[ \t]+|signed[ \t]+)?[A-Za-z_][\w:]*(?:<[^;()]*?>)?[ \t\*&]+'
                      r'[A-Za-z_]\w*[ \t]*=[^;()]*(?:\([^;()]*\)[^;()]*)*);')


def apply_fixups(key, text, fixups):
    text = WINDOW_LONG.sub(r'\1WindowLongA', text)
    text = YR_MSG_PARAMS.sub(lambda m: '(%s, UINT%s, WPARAM%s, LPARAM%s)' % m.groups(), text)
    text = FOR_INIT.sub(lambda m: '%s%s; for(;' % (m.group(1), m.group(2)), text)
    """key: 'hdr/<name>' or 'src/<path>'"""
    for r in fixups:
        if r['rx'] is None or (r['file'] != '*' and r['file'].lower() != key.lower()):
            continue
        text, n = r['rx'].subn((lambda m, r=r: m.expand(r['new'])) if r.get('regex') else (lambda m, r=r: r['new']), text)
        r['hits'] += n
    # "append": a native glue file (native/yr/glue) added to the end of the unit, so it sees the
    # unit's includes and declarations (a native draft of a function the tree leaves unmatched).
    for r in fixups:
        if 'append' in r and r['file'].lower() == key.lower():
            text += '\n#line 1 "native/yr/glue/%s"\n' % r['append'] + (ROOT / 'native/yr/glue' / r['append']).read_text(encoding='latin-1')
            r['hits'] += 1
    return text


def emit(target, text):
    target.parent.mkdir(parents=True, exist_ok=True)
    if not target.exists() or target.read_text(encoding='latin-1') != text:
        target.write_text(text, encoding='latin-1')


def main():
    ap = argparse.ArgumentParser(description=__doc__, formatter_class=argparse.RawDescriptionHelpFormatter)
    ap.add_argument('--out', required=True)
    ap.add_argument('--only', default='', help='comma-separated src names (probe runs)')
    a = ap.parse_args()
    out = Path(a.out)
    headers = {}
    for base in (TIBSUN, TREE / 'include'):
        for h in base.rglob('*'):
            if h.is_file() and h.suffix.lower() in HEADER_SUFFIXES:
                headers[h.relative_to(base).as_posix().lower()] = h
    # case-insensitive includes (VC6 on Windows): the overlay keeps each header under its
    # lower-case name and the original spelling (a symlink would not survive every checkout).
    fixups = load_fixups()
    texts = {k: native_units.port(apply_fixups('hdr/' + k, read(p), fixups)) for k, p in headers.items()}
    for k, text in texts.items():
        emit(out / 'hdr' / k, text)
        spelled = headers[k].relative_to(TIBSUN if str(headers[k]).startswith(str(TIBSUN)) else TREE / 'include').as_posix()
        if spelled != k:
            emit(out / 'hdr' / spelled, text)
    only = set(x for x in a.only.split(',') if x)
    units, skipped = [], []
    for src in sorted((TREE / 'src').rglob('*.cpp')):
        rel = src.relative_to(TREE / 'src').as_posix()
        if only and rel not in only and src.stem not in only:
            continue
        if any(r.get('skip_unit') and r['file'].lower() == ('src/' + rel).lower() for r in fixups):
            skipped.append(rel)
            for r in fixups:
                if r.get('skip_unit') and r['file'].lower() == ('src/' + rel).lower():
                    r['hits'] += 1
            continue
        name = rel[:-4].replace('/', '__')
        target = out / 'units' / (name + '.cpp')
        emit(target, '#line 1 "yr/ra2-decomp/src/%s"\n' % rel + native_units.port(apply_fixups('src/' + rel, read(src), fixups)))
        units.append(target)
    unused = [r.get('old', r.get('cut', r['file'])) for r in fixups if not r['hits'] and (not only or r['file'] == '*' or r['file'].startswith('hdr/'))]
    if unused:
        raise SystemExit('yr_units: fixups that match nothing: %s' % unused)
    emit(out / 'units.cmake', 'set(YR_NATIVE_UNITS\n%s)\n' % ''.join('  "%s"\n' % u for u in units))
    emit(out / 'report.json', json.dumps({'headers': len(headers), 'units': len(units), 'skipped': skipped}, indent=1) + '\n')
    print('yr_units: %d headers, %d units (%d left out) -> %s' % (len(headers), len(units), len(skipped), out))


if __name__ == '__main__':
    main()
