#!/usr/bin/env python3
"""Plain-virtual views against the real classes' native vtables (Scout1288).

Recovered sources often call a virtual function through a "view": a class that
lists the original's VC6 vtable slots as plain virtuals (unknown_slot0 ...) and
is cast onto an object of another, real class. VC6 and the native arm64
(Itanium) ABI lay vtables out differently: a virtual destructor takes one VC6
slot but two native entries, and a class with secondary bases (AbstractClass:
IRTTITypeInfo, INoticeSink, INoticeSource) also gets their overriders in its
primary native vtable. A view that does not mirror those differences calls the
wrong function natively, silently (RadSiteCell's Center_Coord reached
CellClass::Size_Of; r36/r37).

This tool
  1. scans every polymorphic class definition in src/shared, src/recovered,
     native/link and native/drafts;
  2. calls a definition a view when no vtable of that name is emitted in the
     native binary (a view is never constructed), or when it is a shorter
     second definition of a real class's name with another virtual list;
  3. requires every view to be classified in native/data/vtable_views.tsv:
       of <Real>     the view stands for objects of <Real> (evidence: the call
                     site's object, the vtable the original code loads); the
                     native entry of each view slot must be the real class's
                     native entry for the same VC6 slot (clang
                     -fdump-vtable-layouts, Itanium for both, MSVC for the
                     real class's VC6 slot order);
       dead          nothing outside its own object references a function the
                     file defines (checked against the build's objects);
       interface     an interface the native code implements with the same
                     entries (no destructor, no secondary bases; checked);
  4. exits 1 on an unclassified view, a failed check or a stale row.

    python3 native/tools/vtable_views.py --build <full native build> [--list]
"""
import argparse
import json
import re
import shlex
import subprocess
import sys
import tempfile
from pathlib import Path

HERE = Path(__file__).resolve().parent
ROOT = HERE.parents[1]
TSV = ROOT / 'native/data/vtable_views.tsv'
SCAN_DIRS = ('src/shared', 'src/recovered', 'native/link', 'native/drafts')
CLASS_RX = re.compile(r'\b(class|struct)\s+(\w+)\s*(:[^{;()]*)?\{')


def strip_comments(text):
    text = re.sub(r'/\*.*?\*/', lambda m: re.sub(r'[^\n]', ' ', m.group(0)), text, flags=re.S)
    return re.sub(r'//[^\n]*', '', text)


def scan():
    """Polymorphic class definitions: (path, name, bases, virtuals, line)."""
    out = []
    for d in SCAN_DIRS:
        for f in sorted((ROOT / d).rglob('*')):
            if f.suffix not in ('.hpp', '.h', '.cpp'):
                continue
            raw = f.read_text(errors='replace')
            if 'virtual' not in raw:
                continue
            t = strip_comments(raw)
            # only the native branch of `#if defined(__LP64__) ... #else ... #endif`
            t = re.sub(r'#if defined\(__LP64__\)\n(.*?)#else\n.*?#endif', r'\1', t, flags=re.S)
            for m in CLASS_RX.finditer(t):
                name, base = m.group(2), (m.group(3) or '').lstrip(':')
                i, depth = m.end(), 1
                while i < len(t) and depth:
                    depth += {'{': 1, '}': -1}.get(t[i], 0)
                    i += 1
                body = t[m.end():i - 1]
                flat = body
                for _ in range(4):
                    flat = re.sub(r'\{[^{}]*\}', ';', flat)
                virts = [' '.join(v.split()) for v in re.findall(r'\bvirtual\b([^;]*)', flat)]
                if not virts:
                    continue
                bases = [b for b in re.findall(r'(?:public|private|protected|virtual|\s)*\s*([\w:]+)\s*(?:<[^>]*>)?\s*(?:,|$)', base) if b]
                out.append(dict(path=str(f.relative_to(ROOT)), name=name, bases=bases, virtuals=virts,
                                line=raw[:m.start()].count('\n') + 1))
    return out


def rtti_names():
    """Classes the original's RTTI names (manifests/rtti_classes.json): real whatever the build emits."""
    path = ROOT / 'manifests/rtti_classes.json'
    if not path.exists():
        return set()
    return {re.sub(r'<.*', '', v['class_name']).split('::')[-1] for v in json.load(open(path))['vtables']}


def emitted_vtables(build):
    nm = subprocess.run(['nm', str(build / 'ra2-native')], capture_output=True, text=True).stdout
    syms = [l.split()[-1] for l in nm.splitlines() if '__ZTV' in l]
    dem = subprocess.run(['c++filt', '-_'], input='\n'.join(syms), capture_output=True, text=True).stdout
    names = set()
    for l in dem.splitlines():
        l = l.replace('vtable for ', '')
        names.add(re.sub(r'<.*', '', l).split('::')[-1])
    return names


def views(defs, real_names):
    by = {}
    for d in defs:
        by.setdefault(d['name'], []).append(d)
    out = []
    for name, ds in by.items():
        if name not in real_names:
            out += ds
            continue
        shared = [d for d in ds if d['path'].startswith('src/shared/')]
        longest = max(shared or ds, key=lambda d: len(d['virtuals']))
        for d in ds:
            if d is not longest and d['path'] != longest['path'] and d['virtuals'] != longest['virtuals'][:len(d['virtuals'])]:
                out.append(dict(d, real_def=longest))
    return out


def read_tsv():
    rows = {}
    if TSV.exists():
        for line in TSV.read_text().splitlines():
            if not line or line.startswith('#') or line.startswith('path\t'):
                continue
            f = line.split('\t') + [''] * 6
            rows[(f[0], f[1])] = dict(verdict=f[2], real=f[3], header=f[4], evidence=f[5])
    return rows


# ---- clang vtable dumps -------------------------------------------------------------

def compile_command(build):
    cc = json.load(open(build / 'compile_commands.json'))
    e = next(e for e in cc if '/gen/units/' in e['file'])
    cmd = shlex.split(e['command'])
    i = cmd.index('-o')
    del cmd[i:i + 2]
    return [c for c in cmd if c != '-c' and c != e['file']], e['directory']


def unit_of(build, path):
    marker = '#line 1 "%s"' % (ROOT / path)
    for u in sorted((build / 'gen/units').glob('*.cpp')):
        if marker in u.read_text(errors='replace'):
            return u
    return None


def dump(cmd, cwd, source):
    p = subprocess.run(cmd + ['-S', '-o', '/dev/null', '-Xclang', '-fdump-vtable-layouts', str(source)],
                       cwd=cwd, capture_output=True, text=True)
    return p.stdout, p.returncode, p.stderr


def msvc_dump(build, source):
    cmd = ['clang++', '-target', 'i686-pc-windows-msvc', '-DRA2PORT_ALLOW_HOST', '-std=c++17', '-fms-extensions',
           '-fms-compatibility', '-fdelayed-template-parsing', '-Xclang', '-fno-wchar', '-fshort-wchar', '-w',
           '-I', str(build / 'gen/overlay'), '-include', str(ROOT / 'native/port/include/ra2port/prelude.hpp')]
    return dump(cmd, str(ROOT), source)


def method_key(text):
    """'void RadSiteCell::unknown_slot6() const' -> 'unknown_slot6()'; destructors '~'."""
    text = re.sub(r'\s*\[.*?\]\s*$', '', text.strip())
    text = re.sub(r'\s*\[(?:this|return) adjustment.*', '', text)
    m = re.search(r'([\w~]+)\s*\((.*)\)\s*(const)?', text)
    if not m:
        return text
    name = m.group(1)
    if name.startswith('~'):
        return '~'
    params = re.sub(r'\s+', '', re.sub(r'\bconst\b', '', m.group(2)))
    return '%s(%s)%s' % (name, params, ' const' if m.group(3) else '')


def decl_key(decl):
    """A declaration's key as the dumps print it: parameter names and defaults dropped."""
    decl = re.sub(r'=\s*[^,)]+', '', decl)
    m = re.search(r'([\w~]+)\s*\((.*)\)\s*(const)?', decl)
    if not m:
        return decl
    name = m.group(1)
    if name.startswith('~'):
        return '~'
    params = []
    for p in [x for x in m.group(2).split(',') if x.strip() and x.strip() != 'void']:
        p = p.split('=')[0].strip()
        toks = re.findall(r'[\w:]+|[*&]+|<[^>]*>', p)
        if len(toks) >= 2 and re.match(r'^[A-Za-z_]\w*$', toks[-1]) and toks[-1] not in (
                'int', 'char', 'short', 'long', 'bool', 'float', 'double', 'unsigned', 'signed', 'const'):
            toks = toks[:-1]
        params.append(''.join(t for t in toks if t != 'const'))
    return '%s(%s)%s' % (name, ','.join(params), ' const' if m.group(3) else '')


def itanium_entries(text, cls):
    """method key -> primary vtable index (first entry: the complete destructor)."""
    lines = text.splitlines()
    head = re.compile(r"^Vtable for '%s' \(\d+ entries\)\.$" % re.escape(cls))
    for i, l in enumerate(lines):
        if head.match(l):
            out = {}
            for l2 in lines[i + 1:]:
                if not l2.strip():
                    break
                m = re.match(r'^\s+(\d+) \| (.*)$', l2)
                if not m:
                    continue
                if 'offset_to_top' in m.group(2) and int(m.group(1)) > 1:
                    break   # a secondary vtable starts
                if 'RTTI' in m.group(2) or 'offset_to_top' in m.group(2):
                    continue
                out.setdefault(method_key(m.group(2)), int(m.group(1)) - 2)
            return out
    return None


def msvc_slots(text, cls):
    """VC6 slot order of the primary vftable of cls."""
    lines = text.splitlines()
    head = re.compile(r"^VFTable for (?:.* in )?'%s' \(\d+ entries\)\.$" % re.escape(cls))
    for i, l in enumerate(lines):
        if head.match(l):
            out = []
            for l2 in lines[i + 1:]:
                if not l2.strip():
                    break
                m = re.match(r'^\s+(\d+) \| (.*)$', l2)
                if m and not m.group(2).rstrip().endswith('RTTI'):
                    out.append(method_key(m.group(2)))
            return out
    return None


def vc6_model(view, defs_by_name):
    """VC6 slot order of a view from its declarations (bases first; one slot per destructor)."""
    slots = []
    for b in view['bases']:
        for d in defs_by_name.get(b.split('::')[-1], []):
            slots = vc6_model(d, defs_by_name)
            break
        break
    for v in view['virtuals']:
        key = method_key(v.split('=')[0])
        if key in slots:
            continue   # an override keeps its base slot
        slots.append(key)
    return slots


def check_of(build, view, row, defs_by_name, tmp, cmd, cwd):
    real = row['real']
    # the view's native entries: from a unit that includes its file, or a probe including its header
    # the view's native entries and VC6 order: from a source that calls through it (its own
    # file for a file-local view; for a header view, a recovered source that includes the
    # header and calls one of its virtuals), compiled as its native unit and as VC6 (MSVC ABI)
    user = view_user(view)
    if user is None:
        return None   # unused: no source calls a virtual through it, nothing can misdispatch
    unit = unit_of(build, user)
    if unit is None:
        return 'no native unit includes %s' % user
    text, rc, err = dump(cmd, cwd, unit)
    ent_v = itanium_entries(text, view['name'])
    if ent_v is None:
        return None   # unused: no virtual call through it is compiled
    msvc_v, _, _ = msvc_dump(build, ROOT / user)
    vc6_v = msvc_slots(msvc_v, view['name'])
    if vc6_v is None:
        vc6_v = []
        if view['bases']:
            # a derived view: its base's VC6 slots (an MSVC dump of the base's header), then its new virtuals
            base = view['bases'][0].split('::')[-1]
            bdefs = [d for d in defs_by_name.get(base, []) if d['path'].startswith('src/shared/')]
            if not bdefs:
                return 'no VC6 vftable of the derived view (base %s has no shared header)' % base
            bdef = max(bdefs, key=lambda d: len(d['virtuals']))
            bprobe = tmp / ('b_%s.cpp' % base)
            bprobe.write_text('#include "%s"\nvoid ra2_probe(%s *p) { p->~%s(); }\n' % (bdef['path'][len('src/'):], base, base))
            vc6_v = msvc_slots(msvc_dump(build, bprobe)[0], base)
            if vc6_v is None:
                return 'no VC6 vftable of the base %s' % base
            vc6_v = list(vc6_v)
        for v in view['virtuals']:   # declaration order, one slot each; an override keeps its slot
            k = decl_key(v)
            if k == '~' and '~' in vc6_v:
                continue
            if k not in vc6_v:
                vc6_v.append(k)
    # the real class: native and VC6 orders from one probe including its header
    probe = tmp / ('r_%s.cpp' % re.sub(r'\W', '_', real))
    probe.write_text(''.join(('#define %s\n' % h.strip()[7:]) if h.strip().startswith('define:') else ('#include "%s"\n' % h.strip())
                             for h in row['header'].split(',')) +
                     'void ra2_probe(%s *p) { p->~%s(); }\n' % (real, re.sub(r'<.*', '', real).split('::')[-1]))
    text_r, rc, err = dump(cmd, cwd, probe)
    ent_r = itanium_entries(text_r, real) or itanium_entries(text_r, re.sub(r'<.*', '', real))
    msvc_text, rc2, err2 = msvc_dump(build, probe)
    vc6_r = msvc_slots(msvc_text, real) or msvc_slots(msvc_text, re.sub(r'<.*', '', real))
    if ent_r is None or vc6_r is None:
        return 'real class %s not dumped (%s)' % (real, (err or err2).strip().splitlines()[-1:] or rc)
    # an override of a secondary base's virtual (What_Am_I ...) has no VC6 primary slot
    vc6_v = [k for k in vc6_v if not (k in ent_r and k not in vc6_r and k not in ent_v_primary_only(vc6_r))]
    called = called_methods(view)
    bad = []
    for k, key in enumerate(vc6_v):
        if key.split('(')[0] not in called:
            continue
        if k >= len(vc6_r):
            bad.append('slot %d %s: past the real vftable (%d slots)' % (k, key, len(vc6_r)))
            continue
        nv = ent_v.get(key)
        nr = ent_r.get(vc6_r[k])
        if nv is None or nr is None or nv != nr:
            bad.append('slot %d: view %s at native %s, real %s at native %s' % (k, key, nv, vc6_r[k], nr))
    return '; '.join(bad[:4]) + (' (+%d)' % (len(bad) - 4) if len(bad) > 4 else '') if bad else None


_FILES = None


def called_methods(view):
    """Names of the view's virtuals that the files using it call (`->name(` / `.name(`); a
    file-local view only in its own file. Over-approximates (another class's member of the
    same name), never misses a call written in the source."""
    global _FILES
    if _FILES is None:
        _FILES = {}
        for d in SCAN_DIRS:
            for f in (ROOT / d).rglob('*'):
                if f.suffix in ('.hpp', '.h', '.cpp'):
                    _FILES[f] = strip_comments(f.read_text(errors='replace'))
    names = {re.search(r'([\w~]+)\s*\(', v).group(1) for v in view['virtuals'] if re.search(r'([\w~]+)\s*\(', v)}
    if view['path'].endswith('.cpp'):
        files = [_FILES[ROOT / view['path']]]
    else:
        files = [t for t in _FILES.values() if re.search(r'\b%s\b' % re.escape(view['name']), t)]
    called = set()
    for t in files:
        for n in names:
            if n.startswith('~'):
                if re.search(r'->\s*%s\s*\(' % re.escape(n), t):
                    called.add(n)
            elif re.search(r'(?:\.|->)\s*%s\s*\(' % re.escape(n), t):
                called.add(n)
    return called


def ent_v_primary_only(vc6_r):
    return set(vc6_r)


def view_user(view):
    """A recovered source that calls a virtual through the view (its own file when file-local)."""
    called_methods(view)
    if view['path'].endswith('.cpp'):
        return view['path']
    header = view['path'][len('src/'):]
    names = [n for n in called_methods(view)]
    for f, t in sorted(_FILES.items()):
        rel = str(f.relative_to(ROOT))
        if not rel.startswith('src/recovered/') or '"%s"' % header not in t:
            continue
        if any(re.search(r'(?:\.|->)\s*%s\s*\(' % re.escape(n), t) for n in names if not n.startswith('~')):
            return rel
    return None


def vcall_text(view):
    v = view['virtuals'][-1]
    m = re.search(r'([\w~]+)\s*\(([^)]*)\)', v)
    name, params = m.group(1), m.group(2).strip()
    if name.startswith('~'):
        return '%s()' % name
    n = 0 if params in ('', 'void') else params.count(',') + 1
    return '%s(%s)' % (name, ', '.join(['{}'] * n))


def check_dead(build, view, undefined):
    """No member function the file defines out of line (Class::name) is referenced by an object."""
    text = strip_comments((ROOT / view['path']).read_text(errors='replace'))
    defined = set(re.findall(r'\b(\w+)::(\w+)\s*\([^;{]*\)\s*(?:const\s*)?\{', text))
    frags = {'%d%s%d%s' % (len(c), c, len(f), f) for c, f in defined}
    frags |= {'_Z%d%s' % (len(f), f) for f in re.findall(r'^[\w\s\*&<>:,]*?\b(\w+)\s*\([^;{]*\)\s*\{', text, re.M)
              if len(f) > 6 and f not in ('if', 'while', 'switch', 'for')}
    hits = sorted({u for u in undefined for fr in frags if fr in u})
    return ('referenced: %s' % ', '.join(hits[:3])) if hits else None


def undefined_symbols(build):
    objs = [str(p) for p in build.rglob('*.o')]
    out = set()
    for i in range(0, len(objs), 400):
        p = subprocess.run(['nm', '-u'] + objs[i:i + 400], capture_output=True, text=True)
        out |= {l.split()[-1] for l in p.stdout.splitlines() if l.strip() and not l.endswith(':')}
    alias = build / 'gen/alias_list.txt'
    if alias.exists():
        out |= {w for l in alias.read_text().splitlines() for w in l.split()}
    return out


def main():
    ap = argparse.ArgumentParser(description=__doc__, formatter_class=argparse.RawDescriptionHelpFormatter)
    ap.add_argument('--build', type=Path, required=True)
    ap.add_argument('--list', action='store_true', help='print every view and its row, then check')
    args = ap.parse_args()
    build = args.build.resolve()
    defs = scan()
    # Real: a vtable of the name is emitted natively, or the original's RTTI names the class
    # (a class whose vtable has no key function, Blitter's inline destructor, is emitted only
    # where something constructs one, which differs between builds).
    real = emitted_vtables(build) | rtti_names()
    vs = views(defs, real)
    rows = read_tsv()
    defs_by_name = {}
    for d in defs:
        defs_by_name.setdefault(d['name'], []).append(d)
    cmd, cwd = compile_command(build)
    undefined = None
    failures, seen = [], set()
    with tempfile.TemporaryDirectory() as td:
        tmp = Path(td)
        for v in sorted(vs, key=lambda d: (d['path'], d['line'])):
            key = (v['path'], v['name'])
            if key in seen:
                continue
            seen.add(key)
            row = rows.get(key)
            if row is None and v.get('real_def') and v['real_def']['path'].startswith('src/shared/'):
                row = dict(verdict='of', real=v['name'], header=v['real_def']['path'][len('src/'):],
                           evidence='a second definition of the real class %s (%s)' % (v['name'], v['real_def']['path']))
            if row is None:
                failures.append('%s:%d %s: unclassified view (first virtual: %s)' % (v['path'], v['line'], v['name'], v['virtuals'][0][:60]))
                continue
            verdict = row['verdict']
            problem = None
            if verdict == 'of':
                problem = check_of(build, v, row, defs_by_name, tmp, cmd, cwd)
            elif verdict == 'dtor-only':
                if not all('~' in x for x in v['virtuals']):
                    problem = 'declares virtuals other than its destructor'
            elif verdict == 'iunknown-calls':
                # only IUnknown's slots 0-2 are called, and the view declares them first: every COM
                # object's vtable starts with them on both ABIs
                first = [re.search(r'(\w+)\s*\(', x).group(1) for x in v['virtuals'][:3]]
                called = called_methods(v)
                if first != ['QueryInterface', 'AddRef', 'Release'] or not called <= set(first):
                    problem = 'calls %s beyond IUnknown (first virtuals %s)' % (sorted(called), first)
            elif verdict == 'unused-header':
                called_methods(v)
                inc = '"%s"' % v['path'][len('src/'):]
                users = [str(f.relative_to(ROOT)) for f, t in _FILES.items() if inc in t]
                if users:
                    problem = 'included by %s' % ', '.join(sorted(users)[:3])
            elif verdict == 'dead' and not v['path'].endswith('.cpp'):
                problem = 'a header view cannot be dead (classify it as of, interface or open)'
            elif verdict == 'dead':
                # dead: nothing outside its own object references a function the file defines
                if undefined is None:
                    undefined = undefined_symbols(build)
                problem = check_dead(build, v, undefined)
            elif verdict == 'open':
                problem = None
                print('open %s:%d %s: %s' % (v['path'], v['line'], v['name'], row['evidence']))
            elif verdict == 'interface':
                if any('~' in x for x in v['virtuals']):
                    problem = 'an interface with a destructor'
            else:
                problem = 'unknown verdict %r' % verdict
            if not row['evidence'].strip():
                problem = (problem + '; ' if problem else '') + 'no evidence'
            if args.list or problem:
                print('%s %s:%d %s: %s %s%s' % ('FAIL' if problem else 'ok  ', v['path'], v['line'], v['name'], verdict,
                                                row['real'], (' -- ' + problem) if problem else ''))
            if problem:
                failures.append('%s %s: %s' % (v['path'], v['name'], problem))
    # a row is stale when its class is no longer defined in that file (a row for a class this
    # build happens to emit a vtable for is kept: another build may not)
    defined = {(d['path'], d['name']) for d in defs}
    stale = [k for k in rows if k not in defined]
    for k in stale:
        failures.append('%s %s: stale row (no such view)' % k)
    print('vtable views: %d views, %d rows, %d failures' % (len(seen), len(rows), len(failures)))
    for f in failures:
        print('  ' + f)
    return 1 if failures else 0


if __name__ == '__main__':
    sys.exit(main())
