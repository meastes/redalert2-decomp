#!/usr/bin/env python3
"""Merge names that refer to one original address (link aliases).

The recovered tree names some original functions and globals more than once:
a caller's header declares `unknown_sqrt_4ba410(double)` while the accepted
body is `Fast_Sqrt(double)`, a seed refers to `DAT_00831438` while another
source names it `Audio_Main_Device`. The exact build resolves each to its
address; natively they are different symbols. This tool binds every name to
its original address (the catalog's MSVC reference tables and region entries)
and, for each symbol the native inputs reference but do not define, looks for
a defined symbol at the same address:

  * data: one storage per address. An undefined name at the address of a
    defined object (a recovered source's definition, a generated table, a BSS
    storage) becomes an alias of it.
  * functions: an alias only when the two native ABI signatures are equal
    (LLVM IR declare/define lines of the units: return type, parameter types
    and their extension attributes; `this` is an ordinary pointer, references
    are pointers). Mismatches (float/double, int/pointer...) are reported, not
    aliased: they need a written wrapper (native/link/).

Scaffold targets (seed_*/Leaf_*/recover_* namespaces: truncated pointers,
absolute addresses) are never alias targets.

Output: native/data/address_aliases.tsv (alias symbol, target symbol, address,
kind, evidence), applied at link through ld64 -alias_list after
link_closure.py drops the entries whose target is not linked or whose alias
is defined by then; the report lists mismatches and ambiguities.

    python3 native/tools/address_aliases.py --build build/native [-j6]
      (needs a configured full-tree build with CMAKE_EXPORT_COMPILE_COMMANDS=ON
       and its gen/link_closure.json; compiles each unit to IR once, -O0)
"""
import argparse
import concurrent.futures
import json
import re
import shlex
import subprocess
import sys
from pathlib import Path

ROOT = Path(__file__).resolve().parents[2]
sys.path.insert(0, str(ROOT / 'tools/original_target'))
sys.path.insert(0, str(Path(__file__).resolve().parent))
import catalog_io  # noqa: E402
from data_globals import msvc_key  # noqa: E402

SCAFFOLD = re.compile(r'^(seed_[0-9a-fA-F]+::|Leaf_[0-9a-fA-F]+::|recover_)')
IR_LINE = re.compile(r'^(declare|define)\s+(.*?)@("?)([^"(\s]+)\3\((.*)\)')


def ir_signatures(entry):
    """{symbol: signature} of the declare/define lines of one unit."""
    args = shlex.split(entry['command']) if 'command' in entry else list(entry['arguments'])
    out, keep = [], True
    for i, a in enumerate(args):
        if not keep:
            keep = True
            continue
        if a in ('-o', '-MF', '-MT'):
            keep = False
            continue
        if a in ('-c', '-MD') or a.startswith('-O'):
            continue
        out.append(a)
    out += ['-O0', '-S', '-emit-llvm', '-o', '-']
    proc = subprocess.run(out, cwd=entry['directory'], capture_output=True, text=True)
    sigs = {}
    for line in proc.stdout.splitlines():
        if not (line.startswith('declare') or line.startswith('define')):
            continue
        m = IR_LINE.match(line)
        if m:
            sigs[m.group(4)] = (m.group(1), normalise(m.group(2), m.group(5)))
    return sigs


ATTR_DROP = re.compile(r'\b(noundef|nonnull|noalias|nocapture|readonly|writeonly|returned|dso_local|'
                       r'local_unnamed_addr|unnamed_addr|hidden|internal|linkonce_odr|weak_odr|weak|'
                       r'available_externally|inreg|nofree|immarg)\b|\b(dereferenceable(_or_null)?|align)\(?\s*\d+\)?')


def normalise(ret, params):
    ret = ATTR_DROP.sub('', ret)
    ret = ' '.join(t for t in ret.split() if not t.startswith('#'))
    parts = []
    depth, cur = 0, ''
    for ch in params:
        if ch in '([{<':
            depth += 1
        elif ch in ')]}>':
            depth -= 1
        if ch == ',' and depth == 0:
            parts.append(cur)
            cur = ''
        else:
            cur += ch
    if cur.strip():
        parts.append(cur)
    norm = []
    for p in parts:
        p = ATTR_DROP.sub('', p)
        p = re.sub(r'%[\w.]+', '', p)                 # parameter names
        p = re.sub(r'\b(byval|sret|elementtype)\(([^)]*)\)', lambda m: '%s(%s)' % (m.group(1), m.group(2)), p)
        norm.append(' '.join(p.split()))
    return ' '.join(ret.split()) + ' (' + ', '.join(norm) + ')'


SCALAR = {'double': 'double', 'float': 'float', 'i32': 'int', 'ptr': 'void *', 'i64': 'long long',
          'zeroext i1': 'bool', 'signext i8': 'signed char', 'zeroext i8': 'unsigned char',
          'signext i16': 'short', 'zeroext i16': 'unsigned short', 'void': 'void'}
NUMERIC = ('double', 'float')


def split_sig(sig):
    ret, _, params = sig.partition(' (')
    params = params[:-1]
    return ret.strip(), [p.strip() for p in params.split(', ')] if params else []


def compatible(mine, theirs):
    """'alias' when the caller's view (mine) and the definition (theirs) have the
    same native ABI (a void caller may ignore a returned value); 'wrapper' when
    only a float/double return differs (VC6 returned both in st0, so the caller
    read the callee's value at its own width: a converting wrapper keeps that);
    None otherwise."""
    if mine is None or theirs is None:
        return None
    if mine == theirs:
        return 'alias'
    mr, mp = split_sig(mine)
    tr, tp = split_sig(theirs)
    if mp != tp:
        return None
    if mr == 'void':
        return 'alias'
    if mr in NUMERIC and tr in NUMERIC and all(p in SCALAR for p in mp):
        return 'wrapper'
    return None


def write_wrappers(path, wrappers):
    lines = ['// generated by native/tools/address_aliases.py: one name of an original function',
             '// calling the recovered body of the same address where only a float/double',
             '// return differs (VC6 returned both in st0; the caller read the value at its',
             '// own width). Not original code: a link-level adapter until src/ uses one name.',
             '']
    for k, (sym, name, target, dem, a, mine, theirs) in enumerate(sorted(wrappers, key=lambda w: w[4])):
        mr, mp = split_sig(mine)
        tr, _ = split_sig(theirs)
        params = ', '.join('%s a%d' % (SCALAR[p], i) for i, p in enumerate(mp))
        args = ', '.join('a%d' % i for i in range(len(mp)))
        lines.append('// 0x%08X %s -> %s' % (a, name, dem))
        lines.append('extern "C" %s ra2_alias_target_%04d(%s) __asm__("%s");' % (SCALAR[tr], k, params or 'void', target))
        lines.append('extern "C" %s ra2_alias_%04d(%s) __asm__("%s");' % (SCALAR[mr], k, params or 'void', sym))
        lines.append('extern "C" %s ra2_alias_%04d(%s) { return ra2_alias_target_%04d(%s); }'
                     % (SCALAR[mr], k, params or 'void', k, args))
    path.parent.mkdir(parents=True, exist_ok=True)
    path.write_text('\n'.join(lines) + '\n')


def qualified(demangled):
    """Qualified name of a demangled symbol, without its parameter list."""
    depth = 0
    for i, ch in enumerate(demangled):
        if ch == '<':
            depth += 1
        elif ch == '>':
            depth -= 1
        elif ch == '(' and depth == 0:
            return demangled[:i]
    return demangled


def demangle(symbols):
    out = subprocess.run(['c++filt'], input='\n'.join(symbols), capture_output=True, text=True).stdout
    return out.split('\n')[:len(symbols)]


def main():
    ap = argparse.ArgumentParser(description=__doc__, formatter_class=argparse.RawDescriptionHelpFormatter)
    ap.add_argument('--build', type=Path, required=True)
    ap.add_argument('-j', type=int, default=6)
    ap.add_argument('--out', type=Path, default=ROOT / 'native/data/address_aliases.tsv')
    ap.add_argument('--report', type=Path, help='default: BUILD/gen/address_aliases.json')
    ap.add_argument('--wrappers', type=Path, default=ROOT / 'native/link/address_wrappers.cpp')
    args = ap.parse_args()
    build = args.build.resolve()
    closure = json.loads((build / 'gen/link_closure.json').read_text())['symbols']
    inputs = [p for p in (build / 'gen/link_inputs.txt').read_text().split('\n') if p.strip()]
    nm = subprocess.run(['nm', '-gU'] + [p for p in inputs if p.endswith('.o')], capture_output=True, text=True).stdout
    defined = sorted({l.split()[-1] for l in nm.splitlines() if len(l.split()) == 3})
    # Addresses of every catalog name, by qualified name.
    catalog = catalog_io.load(catalog_io.locate(ROOT), references=True)
    addr_of = {}
    for table in catalog['population_references'].values():
        for msvc, addr in table.items():
            k = msvc_key(msvc)
            if k:
                addr_of.setdefault(k[0], set()).add(addr)
    for r in catalog['regions']:
        k = msvc_key(r['symbol'])
        if k:
            addr_of.setdefault(k[0], set()).add(int(r['entry']))
    # Generated data (tables, BSS storage) carries its address in the name or a comment.
    by_addr = {}
    for sym, dem in zip(defined, demangle(defined)):
        q = qualified(dem if sym.startswith('__Z') else sym[1:])
        if SCAFFOLD.match(q):
            continue
        addrs = set(addr_of.get(q, ()))
        m = re.search(r'_(?:0x)?([0-9a-fA-F]{6,8})$', q)
        if m and not addrs:
            addrs.add(int(m.group(1), 16))
        for a in addrs:
            by_addr.setdefault(a, []).append((sym, dem))
    wanted = []
    for row in closure:
        q = qualified(row['name'])
        addrs = set(addr_of.get(q, ()))
        m = re.search(r'_(?:0x)?([0-9a-fA-F]{6,8})$', q)
        if m and not addrs:
            addrs.add(int(m.group(1), 16))
        cands = [(a, t) for a in sorted(addrs) for t in by_addr.get(a, [])]
        if cands:
            wanted.append((row, cands))
    # IR signatures of the units (functions only).
    sigs = {}
    if any(r['kind'] == 'function' for r, _ in wanted):
        cc = json.loads((build / 'compile_commands.json').read_text())
        units = [e for e in cc if '/gen/units/' in e['file'] or '/native/data/' in e['file']
                 or '/native/link/' in e['file']]
        with concurrent.futures.ThreadPoolExecutor(args.j) as pool:
            for result in pool.map(ir_signatures, units):
                for sym, (kind, sig) in result.items():
                    if kind == 'define' or sym not in sigs:
                        sigs[sym] = sig
    aliases, mismatched, ambiguous, wrappers = [], [], [], []
    for row, cands in wanted:
        sym = row['symbol']
        if row['kind'] == 'data':
            uniq = sorted({(a, t[0]) for a, t in cands})
            if len({t for _, t in uniq}) == 1:
                a, target = uniq[0]
                aliases.append((sym, target, a, 'data', 'same address'))
            else:
                ambiguous.append(dict(symbol=row['name'], candidates=['%s @0x%X' % (t, a) for a, t in uniq]))
            continue
        mine = sigs.get(sym[1:]) or sigs.get(sym)
        good, wrap = [], []
        for a, (target, dem) in cands:
            theirs = sigs.get(target[1:]) or sigs.get(target)
            how = compatible(mine, theirs)
            if how == 'alias':
                good.append((a, target, dem))
            elif how == 'wrapper':
                wrap.append((a, target, dem, theirs))
            else:
                mismatched.append(dict(symbol=row['name'], target=dem, address='0x%08X' % a,
                                       wanted=mine, defined=theirs))
        if len({g[1] for g in good}) == 1:
            a, target, dem = good[0]
            aliases.append((sym, target, a, 'function', 'same address, IR signature %s' % mine))
        elif len(good) > 1:
            ambiguous.append(dict(symbol=row['name'], candidates=[g[2] for g in good]))
        elif len({w[1] for w in wrap}) == 1:
            a, target, dem, theirs = wrap[0]
            wrappers.append((sym, row['name'], target, dem, a, mine, theirs))
        elif len(wrap) > 1:
            ambiguous.append(dict(symbol=row['name'], candidates=[w[2] for w in wrap]))
    lines = ['# generated by native/tools/address_aliases.py: names the native inputs reference that '
             'another name defines at the same original address (catalog references / region entries). '
             'Applied with ld64 -alias_list (link_closure.py filters it to the current link).',
             'alias\ttarget\taddress\tkind\tevidence']
    for sym, target, a, kind, ev in sorted(aliases, key=lambda x: (x[2], x[0])):
        lines.append('%s\t%s\t0x%08X\t%s\t%s' % (sym, target, a, kind, ev))
    args.out.write_text('\n'.join(lines) + '\n')
    write_wrappers(args.wrappers, wrappers)
    report = args.report or build / 'gen/address_aliases.json'
    report.write_text(json.dumps(dict(aliases=len(aliases), mismatched=mismatched, ambiguous=ambiguous), indent=1))
    print('address aliases: %d (%d data, %d functions), %d return-conversion wrappers; '
          '%d signature mismatches, %d ambiguous (%s)' % (
              len(aliases), sum(1 for x in aliases if x[3] == 'data'), sum(1 for x in aliases if x[3] == 'function'),
              len(wrappers), len(mismatched), len(ambiguous), report))


if __name__ == '__main__':
    main()
