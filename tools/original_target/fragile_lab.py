"""Fragile-row lab (lane E1b): apply identifier renames (word-boundary) to the lab tree's src, compile the
populations of the fragile rows, report each row's status; restore the tree.
  lab.py RENAMES.json [--keep]   RENAMES: [[old, new], ...]"""
import json, os, re, subprocess, sys, tempfile, shutil
sys.path.insert(0, 'tools/original_target')
os.environ['RA2_OBJECT_CACHE'] = 'off'
import catalog_io
from pathlib import Path
from build import compile_sources
from compile_candidates import compare_candidate
from coff import Coff
ROWS = {0x62A360: 'Print_CRCs', 0x5E5150: 'OwnerDraw 5e5150', 0x6819A0: 'Reposition_Sidebar', 0x68E280: 'WinMain'}
pairs = json.load(open(sys.argv[1])) if len(sys.argv) > 1 and sys.argv[1] != '-' else []
if subprocess.run(['git', 'status', '--porcelain', 'src'], capture_output=True, text=True).stdout.strip():
    sys.exit('fragile_lab: src/ has uncommitted changes; run it in a clean scratch worktree (it restores src with git checkout)')
files = subprocess.run(['git', 'ls-files', 'src'], capture_output=True, text=True).stdout.split()
changed = []
for f in files:
    t = open(f, encoding='latin-1').read(); n = t
    for o, w in pairs:
        n = re.sub(r'(?<![\w$?@])%s(?!\w)' % re.escape(o), w, n)
    if n != t:
        open(f, 'w', encoding='latin-1').write(n); changed.append(f)
try:
    cat = catalog_io.load(catalog_io.locate(Path('.')))
    config = json.load(open('private/original-target.local.json'))
    pops = {r['population'] for r in cat['regions'] if r['entry'] in ROWS}
    rows = [r for r in cat['regions'] if r['population'] in pops]
    out = Path(tempfile.mkdtemp(dir='build/lab'))
    compiled = compile_sources(config, dict(cat, regions=rows), out)
    import pefile
    pe = pefile.PE(config['executable']); base = pe.OPTIONAL_HEADER.ImageBase
    class A:
        def original(self, va, n): return pe.get_data(va - base, n)
    res = {}
    for group, obj in compiled:
        coff = Coff(Path(obj).read_bytes())
        for row in group:
            if row['entry'] in ROWS:
                refs = {}
                for k, v in (row.get('references') or {}).items():
                    for o, w in pairs:
                        if k.startswith('?%s@' % o): k = '?%s@' % w + k[len(o) + 2:]
                        elif k == '_' + o: k = '_' + w
                    refs.setdefault(k, v)
                cand = dict(row, references=refs, method='%08x' % row['entry'], region_bytes=row['full_extent_bytes'], source_sha256='')
                r = compare_candidate(A(), coff, cand, Path(obj), row['options'], 0)
                res[ROWS[row['entry']]] = r['status'] + ('' if r['status'] == 'extent-exact' else ' %s' % r.get('differing_bytes'))
    print(json.dumps(res), 'changed files', len(changed))
    shutil.rmtree(out, ignore_errors=True)
finally:
    if '--keep' not in sys.argv and changed:
        subprocess.run(['git', 'checkout', '--'] + changed)
