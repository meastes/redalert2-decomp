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
    are pointers). A float/double return difference gets a converting wrapper
    (native/link/address_wrappers.cpp); integral widths, a Win32 handle carried
    as an int (port handles are small tokens) or an ignored result get a
    converting adapter (native/link/signature_adapters.cpp). The rest (int for
    pointer, parameter count, struct by value, a read result the definition
    does not return) are reported with a reason, not linked: one side's
    declaration is wrong and src/ must settle it.

Scaffold targets (seed_*/Leaf_*/recover_* namespaces: truncated pointers,
absolute addresses) are never alias targets.

Output: native/data/address_aliases.tsv (alias symbol, target symbol, address,
kind, evidence), applied at link through ld64 -alias_list after
link_closure.py drops the entries whose target is not linked or whose alias
is defined by then; the report lists mismatches and ambiguities.

    python3 native/tools/address_aliases.py --build build/native [-j6]
      (needs a built full tree with CMAKE_EXPORT_COMPILE_COMMANDS=ON and
       RA2_NATIVE_LINK_CLOSURE=ON; recomputes the undefined queue from
       gen/link_inputs.txt without its own outputs, so it is idempotent;
       compiles each unit to IR once, -O0)
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
import link_closure  # noqa: E402

SCAFFOLD = re.compile(r'^(seed_[0-9a-fA-F]+::|Leaf_[0-9a-fA-F]+::|recover_)')
IR_LINE = re.compile(r'^(declare|define)\s+(.*?)@("?)([^"(\s]+)\3\((.*)\)')


def structor(q):
    """A constructor or destructor (Class::Class, Class::~Class): its last name repeats the
    class name, so an address suffix there names the class, not the member's address."""
    parts = q.split('::')
    return len(parts) > 1 and parts[-1].lstrip('~') == parts[-2]


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


# Canonical scalar kinds of IR parameter/return spellings.
CANON = {'i32': 'int', 'i64': 'i64', 'ptr': 'ptr',
         'zeroext i1': 'bool', 'i1 zeroext': 'bool', 'signext i8': 'schar', 'i8 signext': 'schar',
         'zeroext i8': 'uchar', 'i8 zeroext': 'uchar', 'signext i16': 'short', 'i16 signext': 'short',
         'zeroext i16': 'ushort', 'i16 zeroext': 'ushort'}
CTYPE = {'int': 'int', 'i64': 'long long', 'ptr': 'void *', 'bool': 'bool', 'schar': 'signed char',
         'uchar': 'unsigned char', 'short': 'short', 'ushort': 'unsigned short', 'void': 'void'}
SMALL = {'bool', 'schar', 'uchar', 'short', 'ushort', 'int'}
# Win32 handle types: the port's handles are small integer tokens (port/src), so
# a handle carried in a 32-bit int survives LP64 unchanged.
HANDLE = re.compile(r'^(HWND__|HINSTANCE__|HDC__|HMENU__|HICON__|HBITMAP__|HFONT__|HBRUSH__|HKEY__|'
                    r'HPALETTE__|HRGN__|HACCEL__|HMONITOR__|_IMAGELIST|HKL__)\s*(const\s*)?\*$')
INT_TYPES = {'int', 'unsigned int', 'long', 'unsigned long', 'unsigned'}
# Scaffold views of a callee (seed glue classes, Ghidra seeds): their declared
# result is Ghidra's undefined4, not a value the original callee produced.
SEED_VIEW = re.compile(r'^(seed_[0-9a-fA-F]+::|Leaf_[0-9a-fA-F]+::|recover_|(Call|Target)_[0-9a-fA-F]+(_\d+)?::)')


def demangled_params(dem):
    """Top-level parameter types of a demangled function name ([] for '()')."""
    depth, start = 0, None
    for i, ch in enumerate(dem):
        if ch == '<':
            depth += 1
        elif ch == '>':
            depth -= 1
        elif ch == '(' and depth == 0:
            start = i
            break
    if start is None:
        return None
    depth, cur, out = 0, '', []
    for ch in dem[start + 1:]:
        if ch in '(<[':
            depth += 1
        elif ch in ')>]':
            if depth == 0:
                break
            depth -= 1
        if ch == ',' and depth == 0:
            out.append(cur.strip())
            cur = ''
        else:
            cur += ch
    if cur.strip() and cur.strip() != 'void':
        out.append(cur.strip())
    return out


def convert(frm, to, frm_type, to_type):
    """C expression template ('{}' = the value) taking a value of IR kind frm
    (C++ type frm_type) to kind to, as the VC6 callee would read it; (template,
    kind) or (None, reason)."""
    if frm == to:
        return '{}', 'same'
    if frm in SMALL and to in SMALL:
        slot = '(unsigned int)(int){}'      # VC6: a 32-bit stack slot / eax
        if to == 'bool':
            return '((%s & 0xFFu) != 0)' % slot, 'width'
        return '(%s)%s' % (CTYPE[to], slot), 'width'
    if {frm, to} == {'ptr', 'int'}:
        ptype, itype = (frm_type, to_type) if frm == 'ptr' else (to_type, frm_type)
        if HANDLE.match(ptype or '') and (itype or '') in INT_TYPES:
            if frm == 'ptr':
                return '(int)(uintptr_t){}', 'handle'
            return '(void *)(uintptr_t)(unsigned int){}', 'handle'
        return None, 'int-for-pointer (%s vs %s): a 32-bit int cannot carry an LP64 pointer' % (frm_type, to_type)
    if {frm, to} == {'ptr', 'i64'}:          # pointer-sized LPARAM/LRESULT: lossless
        return ('(void *)(intptr_t){}' if to == 'ptr' else '(long long)(intptr_t){}'), 'pointer-sized'
    if frm in SMALL and to == 'i64':          # widening (Win64 WPARAM/LPARAM side)
        unsigned = (frm_type or '').startswith('unsigned') or frm in ('bool', 'uchar', 'ushort')
        return ('(long long)(unsigned int){}' if unsigned else '(long long)(int){}'), 'widen'
    if frm == 'i64' and to in SMALL and (frm_type or '') == 'unsigned long':
        return '(%s)(unsigned int){}' % CTYPE[to], 'wparam'   # WPARAM: VC6 passed 32 bits
    return None, '%s (%s) vs %s (%s)' % (frm, frm_type, to, to_type)


def adaptable(mine, theirs, name, dem):
    """A converting adapter when the two views differ only in ways a native
    wrapper carries without losing VC6 meaning: integral widths (VC6 passed a
    32-bit slot; the callee read its own width: bool = low byte nonzero, char =
    low byte, short = low word), a Win32 handle carried in an int (port handles
    are small tokens), pointer-sized vs pointer/32-bit message parameters, a
    result the caller ignores, or a seed view's undefined4 result of a void
    callee (0). Returns (plan, None) or (None, reason)."""
    if mine is None or theirs is None:
        return None, 'no IR signature'
    mr, mp = split_sig(mine)
    tr, tp = split_sig(theirs)
    pair_ret = mr == tr == '[2 x i64]'       # a 16-byte struct returned in x0/x1 on both sides
    if any('sret' in x or 'byval' in x or x.startswith('[') for x in mp + tp) or (
            not pair_ret and any(x.startswith('[') or 'sret' in x for x in (mr, tr))):
        return None, 'struct passed or returned by value differs'
    if '...' in mp or '...' in tp:
        return None, 'variadic'
    md, td = demangled_params(name), demangled_params(dem)
    if md is None or td is None:
        return None, 'unparsed name'
    moff, toff = len(mp) - len(md), len(tp) - len(td)
    convs, kinds = [], []
    drop_this = False
    if len(mp) == len(tp) + 1 and moff == 1 and toff == 0 and mp[0] == 'ptr':
        # The caller's view is a member (VC6 __thiscall: this in ecx), the
        # definition a free function that never reads ecx: the adapter drops it.
        drop_this = True
        convs.append(None)
        kinds.append('this dropped')
        mp_rest = mp[1:]
    elif len(mp) != len(tp):
        return None, 'parameter count differs (%d vs %d)' % (len(mp), len(tp))
    else:
        mp_rest = mp
    if any(x not in CANON for x in mp + tp) or any(x not in CANON and x != 'void' and not pair_ret for x in (mr, tr)):
        return None, 'unhandled IR type'
    base = 1 if drop_this else 0
    for i, (m, th) in enumerate(zip(mp_rest, tp), start=base):
        if drop_this:
            mt = md[i - moff] if 0 <= i - moff < len(md) else None
            tt = td[i - 1] if 0 <= i - 1 < len(td) else None
            tmpl, kind = convert(CANON[m], CANON[th], mt, tt)
            if tmpl is None:
                return None, 'parameter %d: %s' % (i, kind)
            convs.append(tmpl)
            kinds.append(kind)
            continue
        mt = md[i - moff] if 0 <= i - moff < len(md) else ('this' if m == 'ptr' else None)
        tt = td[i - toff] if 0 <= i - toff < len(td) else ('this' if th == 'ptr' else None)
        tmpl, kind = convert(CANON[m], CANON[th], mt, tt)
        if tmpl is None:
            return None, 'parameter %d: %s' % (i, kind)
        convs.append(tmpl)
        kinds.append(kind)
    structor = re.search(r'::(~?)(\w+)::\1?\2\(|::~\w+\(', name) is not None or re.match(r'^(?:.*::)?(\w+)::~?\1\(', name) is not None
    if pair_ret:
        ret = '{}'
    elif mr == 'void':
        ret = None
        if tr != 'void':
            kinds.append('result ignored')
    elif mr == 'ptr' and tr == 'void' and drop_this and structor:
        ret = 'this'                         # Apple arm64: constructors/destructors return this
        kinds.append('structor returns this')
    elif tr == 'void':
        if SEED_VIEW.match(name):
            ret = '0'
            kinds.append('seed view result 0')
        else:
            return None, 'caller reads a value the definition does not return (%s vs void)' % mr
    else:
        ret, kind = convert(CANON[tr], CANON[mr], None, None)
        if ret is None:
            return None, 'return %s (caller) vs %s (definition): %s' % (mr, tr, kind.split(' (')[0])
        if kind != 'same':
            kinds.append('return ' + kind)
    return (convs, ret, kinds), None


def write_adapters(path, adapters):
    lines = ['// generated by native/tools/address_aliases.py: one name of an original function',
             '// calling the recovered body of the same address where the two declarations',
             '// differ only in ways a converting call keeps VC6 meaning: integral widths (VC6',
             '// passed a 32-bit slot and the callee read its own width: bool = low byte',
             '// nonzero, char = low byte, short = low word), a Win32 handle carried as an int',
             '// (port handles are small tokens), pointer-sized message parameters, a result',
             '// the caller ignores, a seed view\'s undefined4 result of a void callee (0).',
             '// Not original code: a link-level adapter until src/ declares one signature',
             '// (gen/address_aliases.json lists the mismatches no adapter can carry).',
             '#include <stdint.h>',
             'struct ra2_pair { long long lo, hi; };   // a 16-byte struct returned in x0/x1', '']
    for k, (sym, name, target, dem, a, mine, theirs, plan) in enumerate(sorted(adapters, key=lambda w: (w[4], w[0]))):
        convs, ret, kinds = plan
        mr, mp = split_sig(mine)
        tr, tp = split_sig(theirs)
        ctype = lambda x: CTYPE['void'] if x == 'void' else ('ra2_pair' if x == '[2 x i64]' else CTYPE[CANON[x]])
        params = ', '.join('%s a%d' % (ctype(x), i) for i, x in enumerate(mp))
        tparams = ', '.join(ctype(x) for x in tp)
        call = 'ra2_adapter_target_%04d(%s)' % (k, ', '.join(c.format('a%d' % i) for i, c in enumerate(convs) if c is not None))
        lines.append('// 0x%08X %s -> %s [%s]' % (a, name, dem, ', '.join(sorted(set(kinds) - {'same'}))))
        lines.append('extern "C" %s ra2_adapter_target_%04d(%s) __asm__("%s");' % (ctype(tr), k, tparams or 'void', target))
        lines.append('extern "C" %s ra2_adapter_%04d(%s) __asm__("%s");' % (ctype(mr), k, params or 'void', sym))
        if mr == 'void':
            body = '%s;' % call
        elif ret == '0':
            body = '%s; return 0;' % call
        elif ret == 'this':
            body = '%s; return a0;' % call
        else:
            body = 'return %s;' % ret.format(call)
        lines.append('extern "C" %s ra2_adapter_%04d(%s) { %s }' % (ctype(mr), k, params or 'void', body))
    path.parent.mkdir(parents=True, exist_ok=True)
    path.write_text('\n'.join(lines) + '\n')


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


# A derived class's vtable in the exact tree may reference an inherited slot
# under the base class's name at the derived override's address (population
# reference tables: UnitClass's unit maps ?unknown_slot176@TechnoClass to
# FootClass's 0x004C9930, while TechnoClass's own body is 0x006CFEA0). Binding
# the base name to the derived override makes the override's base call recurse
# (Scout1091: FootClass::unknown_slot176 -> TechnoClass::unknown_slot176 ->
# itself). A candidate that is the same member of a class derived from the
# wanted name's class is therefore not an alias (RTTI bases,
# manifests/rtti_classes.json).
_RTTI_BASES = None


def rtti_bases():
    global _RTTI_BASES
    if _RTTI_BASES is None:
        _RTTI_BASES = {}
        for v in json.loads((ROOT / 'manifests/rtti_classes.json').read_text())['vtables']:
            _RTTI_BASES.setdefault(v['class_name'], set()).update(b['name'] for b in v.get('bases', []))
    return _RTTI_BASES


def derived_override(wanted, target):
    """True when target is wanted's member as a class derived from wanted's class."""
    w = re.match(r'^(.*)::([^:(]+)(\(.*)?$', wanted or '')
    t = re.match(r'^(.*)::([^:(]+)(\(.*)?$', target or '')
    if not w or not t or w.group(2) != t.group(2) or w.group(1) == t.group(1):
        return False
    return w.group(1) in rtti_bases().get(t.group(1), ())


def demangle(symbols):
    out = subprocess.run(['c++filt'], input='\n'.join(symbols), capture_output=True, text=True).stdout
    return out.split('\n')[:len(symbols)]


def other_overload(q, wanted, addrs, target):
    """An overloaded name (the catalog gives it several addresses) is bound by its
    qualified name only: a defined overload with other parameters is a different
    function (Tactical::Draw_Tiles(Rect, Rect) 0x006A7BA0 is not Draw_Tiles(Cell, Rect)
    0x006A8560; Scout1138, M17)."""
    sym, dem = target
    if len(addrs) < 2 or not sym.startswith('__Z'):
        return False
    return qualified(dem) == q and dem != wanted


def main():
    ap = argparse.ArgumentParser(description=__doc__, formatter_class=argparse.RawDescriptionHelpFormatter)
    ap.add_argument('--build', type=Path, required=True)
    ap.add_argument('-j', type=int, default=6)
    ap.add_argument('--out', type=Path, default=ROOT / 'native/data/address_aliases.tsv')
    ap.add_argument('--report', type=Path, help='default: BUILD/gen/address_aliases.json')
    ap.add_argument('--wrappers', type=Path, default=ROOT / 'native/link/address_wrappers.cpp')
    ap.add_argument('--adapters', type=Path, default=ROOT / 'native/link/signature_adapters.cpp')
    args = ap.parse_args()
    build = args.build.resolve()
    # The queue: what the link misses without this tool's own outputs (the
    # aliases, wrappers and adapters it wrote before), so a re-run regenerates
    # every entry rather than only new ones. data/bss_aliases.tsv still applies.
    own = ('/link/%s.o' % args.wrappers.name, '/link/%s.o' % args.adapters.name)
    inputs = [p for p in (build / 'gen/link_inputs.txt').read_text().split('\n')
              if p.strip() and not p.endswith(own)]
    hosts = sorted(str(h) for h in build.glob('CMakeFiles/ra2_host_objs.dir/**/*.o'))
    missing, _ = link_closure.missing_symbols(inputs + hosts, [ROOT / 'native/data/bss_aliases.tsv'])
    closure = link_closure.classify(missing)
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
    # Included drafts (native/drafts.json) define their entry under the name they record.
    for d in json.loads((ROOT / 'native/drafts.json').read_text())['drafts']:
        if d.get('draft') and d.get('native') == 'compiles' and not d.get('name', 'FUN_').startswith('FUN_'):
            addr_of.setdefault(d['name'], set()).add(int(d['entry'], 16))
    # Generated data (tables, BSS storage) carries its address in the name or a comment.
    by_addr = {}
    for sym, dem in zip(defined, demangle(defined)):
        q = qualified(dem if sym.startswith('__Z') else sym[1:])
        if SCAFFOLD.match(q):
            continue
        addrs = set(addr_of.get(q, ()))
        m = re.search(r'_(?:0x)?([0-9a-fA-F]{6,8})$', q)
        if m and not addrs and not structor(q):
            addrs.add(int(m.group(1), 16))
        for a in addrs:
            by_addr.setdefault(a, []).append((sym, dem))
    wanted = []
    for row in closure:
        q = qualified(row['name'])
        addrs = set(addr_of.get(q, ()))
        m = re.search(r'_(?:0x)?([0-9a-fA-F]{6,8})$', q)
        if m and not addrs and not structor(q):
            addrs.add(int(m.group(1), 16))
        cands = [(a, t) for a in sorted(addrs) for t in by_addr.get(a, [])
                 if not derived_override(q, qualified(t[1] if t[0].startswith('__Z') else t[0][1:]))
                 and not other_overload(q, row['name'], addrs, t)]
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
    aliases, mismatched, ambiguous, wrappers, adapters = [], [], [], [], []
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
        good, wrap, adapt, misses = [], [], [], []
        for a, (target, dem) in cands:
            theirs = sigs.get(target[1:]) or sigs.get(target)
            how = compatible(mine, theirs)
            if how == 'alias':
                good.append((a, target, dem))
            elif how == 'wrapper':
                wrap.append((a, target, dem, theirs))
            else:
                plan, reason = adaptable(mine, theirs, row['name'], dem)
                if plan:
                    adapt.append((a, target, dem, theirs, plan))
                misses.append(dict(symbol=row['name'], target=dem, address='0x%08X' % a,
                                   wanted=mine, defined=theirs, reason=reason or 'adaptable'))
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
        elif len({w[1] for w in adapt}) == 1:
            a, target, dem, theirs, plan = adapt[0]
            adapters.append((sym, row['name'], target, dem, a, mine, theirs, plan))
        elif len(adapt) > 1:
            ambiguous.append(dict(symbol=row['name'], candidates=[w[2] for w in adapt]))
        else:
            mismatched += misses
    lines = ['# generated by native/tools/address_aliases.py: names the native inputs reference that '
             'another name defines at the same original address (catalog references / region entries). '
             'Applied with ld64 -alias_list (link_closure.py filters it to the current link).',
             'alias\ttarget\taddress\tkind\tevidence']
    for sym, target, a, kind, ev in sorted(aliases, key=lambda x: (x[2], x[0])):
        lines.append('%s\t%s\t0x%08X\t%s\t%s' % (sym, target, a, kind, ev))
    args.out.write_text('\n'.join(lines) + '\n')
    write_wrappers(args.wrappers, wrappers)
    write_adapters(args.adapters, adapters)
    report = args.report or build / 'gen/address_aliases.json'
    report.write_text(json.dumps(dict(aliases=len(aliases), adapters=[dict(symbol=x[1], target=x[3], address='0x%08X' % x[4],
                                      kinds=sorted(set(x[7][2]) - {'same'})) for x in adapters],
                                      mismatched=mismatched, ambiguous=ambiguous), indent=1))
    print('address aliases: %d (%d data, %d functions), %d return-conversion wrappers, %d signature adapters; '
          '%d signature mismatches (%d names), %d ambiguous (%s)' % (
              len(aliases), sum(1 for x in aliases if x[3] == 'data'), sum(1 for x in aliases if x[3] == 'function'),
              len(wrappers), len(adapters), len(mismatched), len({m['symbol'] for m in mismatched}), len(ambiguous), report))


if __name__ == '__main__':
    main()
