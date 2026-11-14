#!/usr/bin/env python3
"""Unit evidence for the `$E` rows from the original `.CRT$XCU` table (lane X1, docs/CRT_XCU_TABLE.md;
docs/DECISIONS.md "$E initialiser/atexit rows placed by the original .CRT$XCU table").

    xcu_units.py [--config CONFIG] [--tier TIER ...] [--json OUT] [--summary]

Read-only report: no compiles, nothing written except --json. Prints one tab-separated line per
address-named source file holding a `_$E` row (an initialiser, a chain function or an atexit
destructor reached from the table): tier, unit (the pick), candidates, table entries, file.

Tiers (first that applies):
  single          one evidenced unit: the entries lie between two anchors of one unit (table/text
                  order), or their data targets lie inside one unit's data block, or the same
                  re-applied with those as anchors (no other unit's code between);
  group           a true/open group: a third unit's code lies between the anchors, or the unit map
                  names a third unit for the entries (stays in its header_statics/globals group);
  stay            two candidates but no pick: the previous unit is one of those that end with an
                  initialiser, the unit map names the other candidate, or (prefix tier only) the
                  next unit is one that does not start with its initialisers; also the
                  function-local static destructors registered outside the table (they move with
                  their registering function);
  xcu-exclusion   two candidates; the previous unit already owns an initialiser of the same
                  distinctive shape (one unit cannot instantiate a header static twice): the next;
  xcu-prefix+map  two candidates; prefix rule (units start with their initialisers, the previous
                  ends with ordinary code) and the unit map agree on the next unit;
  xcu-prefix      two candidates; prefix rule alone.
The rows of one compile group with one pick (a header-statics group: one include emits them all)
take the strongest flagged tier among them.

Anchors: every function of a re-filed unit's module map (manifests/original_target/modules/*.json
`code`), else the unit whose module source holds the row. The table order equals .text order, so
the segmentation reads anchors in .text order. Byte-free output (addresses, unit and file names).
"""
import argparse
import bisect
import collections
import json
from pathlib import Path
import re
import struct
import sys

REPOSITORY = Path(__file__).resolve().parents[2]
sys.path.insert(0, str(Path(__file__).resolve().parent))

TIERS = ('single', 'xcu-exclusion', 'xcu-prefix+map', 'xcu-prefix', 'stay', 'group')
FLAG_TIERS = ('xcu-exclusion', 'xcu-prefix+map', 'xcu-prefix')
TEXT_START = 0x401000
TEXT_LIMIT = 0x7A0000          # code addresses pushed before an atexit call
DATA_LIMIT = 0xB30000          # .data/.bss targets of an initialiser
DATA_FIRST = 0x7CC870          # first .data address after the initialiser tables
ATEXIT = 0x0078298A            # the CRT atexit
DEFAULT_CONFIG = REPOSITORY / 'private/original-target.interactive.json'
MEMORY = re.compile(r'\[(0x[0-9a-f]+)\]')
HEX = re.compile(r'0x[0-9a-f]+')
WRITES = ('mov', 'fstp', 'fst', 'fistp', 'fist', 'inc', 'dec', 'or', 'and', 'add', 'sub', 'movsx', 'movzx')


def norm(unit):
    return unit[:-4] if unit and unit.endswith('.cpp') else unit


# ----------------------------------------------------------------------------- image

class Image:
    def __init__(self, data):
        self.data = data
        pe = struct.unpack_from('<I', data, 0x3C)[0]
        count = struct.unpack_from('<H', data, pe + 6)[0]
        optional = struct.unpack_from('<H', data, pe + 20)[0]
        base = struct.unpack_from('<I', data, pe + 24 + 28)[0]
        self.sections = []
        for i in range(count):
            o = pe + 24 + optional + 40 * i
            name = data[o:o + 8].rstrip(b'\0').decode()
            vsize, va, rsize, raw = struct.unpack_from('<IIII', data, o + 8)
            self.sections.append((name, base + va, vsize, rsize, raw))
        import capstone
        self.decoder = capstone.Cs(capstone.CS_ARCH_X86, capstone.CS_MODE_32)

    def offset(self, address):
        for _, va, _, rsize, raw in self.sections:
            if va <= address < va + rsize:
                return raw + address - va
        raise ValueError('%08x outside the image' % address)

    def u32(self, address):
        return struct.unpack_from('<I', self.data, self.offset(address))[0]

    def section(self, name):
        return next(va for n, va, *_ in self.sections if n == name)

    def disassemble(self, address, size):
        o = self.offset(address)
        return [(i.address, i.mnemonic, i.op_str) for i in self.decoder.disasm(self.data[o:o + size], address)]


def table_slots(image, entries):
    """[(slot, initialiser)] of the C++ initialiser table: from the null slot __xc_a at the start
    of .data to the slot before __xi_a (a null followed by a pointer that is no catalog row)."""
    start = image.section('.data')
    assert image.u32(start) == 0, '__xc_a is not the first .data slot'
    slots, address = [], start + 4
    while True:
        value = image.u32(address)
        if value == 0:
            following = image.u32(address + 4)
            if following == 0 or following not in entries:
                break
        else:
            slots.append((address, value))
        address += 4
    return slots


# ----------------------------------------------------------------------------- facts per entry

def entry_facts(image, slots, regions):
    """Per table entry: chain members (the `_$E` functions it calls or jumps to), registered
    destructors (the code address pushed before the atexit call), data targets and body shape."""
    def body(address, default=128):
        row = regions.get(address)
        return image.disassemble(address, (row or {}).get('body_bytes') or default)

    def is_e(address):
        row = regions.get(address)
        return bool(row and (row.get('symbol') or '').startswith('_$E'))

    out = []
    for index, (slot, entry) in enumerate(slots):
        members, dtors, todo = [entry], [], [entry]
        while todo:
            code = body(todo.pop())
            for k, (_, mnemonic, ops) in enumerate(code):
                if mnemonic in ('call', 'jmp') and ops.startswith('0x'):
                    target = int(ops, 16)
                    if target == ATEXIT:
                        pushed = [int(o, 16) for _, m, o in code[:k] if m == 'push' and o.startswith('0x')
                                  and TEXT_START <= int(o, 16) < TEXT_LIMIT]
                        if pushed:
                            dtors.append(pushed[-1])
                    elif is_e(target) and target not in members:
                        members.append(target)
                        todo.append(target)
        targets = set()
        for address in members:
            for _, mnemonic, ops in body(address):
                first = ops.split(',')[0]
                match = MEMORY.search(first)
                if match and mnemonic in WRITES and DATA_FIRST <= int(match.group(1), 16) < DATA_LIMIT:
                    targets.add(int(match.group(1), 16))
                if mnemonic == 'mov' and first.strip() == 'ecx' and ops.split(',')[1].strip().startswith('0x'):
                    value = int(ops.split(',')[1], 16)
                    if DATA_FIRST <= value < DATA_LIMIT:
                        targets.add(value)
                if mnemonic == 'push' and ops.startswith('0x') and DATA_FIRST <= int(ops, 16) < DATA_LIMIT:
                    targets.add(int(ops, 16))
        shape = []
        for _, mnemonic, ops in body(entry, 64):
            def mask(match, mnemonic=mnemonic):
                value = int(match.group(0), 16)
                if DATA_FIRST <= value < DATA_LIMIT:
                    return 'D'
                if TEXT_START <= value < 0x79A000 and mnemonic not in ('call', 'jmp'):
                    return 'T'
                if mnemonic.startswith('j'):
                    return 'L'
                return match.group(0)
            shape.append(mnemonic + ' ' + HEX.sub(mask, ops))
        out.append(dict(index=index, slot=slot, entry=entry, members=members, dtors=dtors, targets=targets,
                        shape=tuple(shape)))
    return out


# ----------------------------------------------------------------------------- classification

def distinctive(shape):
    return any(x.startswith(('call', 'fld', 'fdiv', 'fmul')) for x in shape)


def classify_entries(facts, code_owner, source_owner, unit_of, regions):
    """Adds to each fact: owner, cls (owned | single:table | single:data | single:propagated |
    two | open | open:unit-map third), cands (tuple), pick, hyp (unit-map units), prev, next
    (the text anchors' units) and excluded (the previous unit already owns this shape)."""
    def owner(address):
        if address in code_owner:
            return code_owner[address]
        row = regions.get(address)
        return source_owner.get(row['source']) if row else None

    for f in facts:
        owners = {owner(a) for a in f['members'] + f['dtors']} - {None}
        if len(owners) > 1:
            raise SystemExit('table entry %08x has two owners: %s' % (f['entry'], sorted(owners)))
        f['owner'] = next(iter(owners)) if owners else None
        f['hyp'] = {norm(unit_of[a]) for a in f['members'] + f['dtors'] if a in unit_of}
    members = {a for f in facts for a in f['members'] + f['dtors']}
    units = collections.defaultdict(list)
    for address, unit in code_owner.items():
        units[unit].append(address)
    anchors = sorted(code_owner)
    mapped = sorted(unit_of)
    ends_init = {u for u, s in units.items() if max(s) in members}
    starts_other = {u for u in {f['owner'] for f in facts if f['owner']} if min(units[u]) not in members}
    # text anchors
    for f in facts:
        if f['owner']:
            continue
        a = f['entry']
        j = bisect.bisect_left(anchors, a)
        f['prev'] = code_owner[anchors[j - 1]] if j > 0 else None
        f['next'] = code_owner[anchors[j]] if j < len(anchors) else None
        lo = anchors[j - 1] if j > 0 else 0
        hi = anchors[j] if j < len(anchors) else 1 << 40
        between = {norm(unit_of[x]) for x in mapped[bisect.bisect_right(mapped, lo):bisect.bisect_left(mapped, hi)]
                   if x not in members}
        if f['prev'] == f['next']:
            f['text'] = 'same'
            f['text_cands'] = {f['prev']}
        else:
            others = between - {norm(f['prev']), norm(f['next'])}
            f['text'] = 'two' if not others else 'open'
            f['text_cands'] = {f['prev'], f['next']} | others
    # data blocks per owned unit (non-overlapping units only)
    span = {}
    for f in facts:
        if f['owner'] and f['targets']:
            lo, hi = span.get(f['owner'], (1 << 40, 0))
            span[f['owner']] = (min(lo, min(f['targets'])), max(hi, max(f['targets'])))
    ordered = sorted((lo, hi, u) for u, (lo, hi) in span.items())
    overlapping = {x for a, b in zip(ordered, ordered[1:]) if b[0] <= a[1] for x in (a[2], b[2])}
    for f in facts:
        if f['owner'] or not f['targets']:
            f['data_unit'] = None
            continue
        inside = {u for v in f['targets'] for lo, hi, u in ordered if lo <= v <= hi}
        f['data_unit'] = next(iter(inside)) if len(inside) == 1 and not inside & overlapping else None
    # final class
    for f in facts:
        f['excluded'] = False
        if f['owner']:
            f.update(cls='owned', cands=(f['owner'],), pick=f['owner'])
        elif f['text'] == 'same':
            f.update(cls='single:table', cands=(f['prev'],), pick=f['prev'])
        elif f['data_unit']:
            f.update(cls='single:data', cands=(f['data_unit'],), pick=f['data_unit'])
        elif f['text'] == 'two':
            cands = (f['prev'], f['next'])
            if f['hyp'] and not f['hyp'] & {norm(x) for x in cands}:
                f.update(cls='open:unit-map third', cands=cands + tuple(sorted(f['hyp'])), pick=None)
            else:
                f.update(cls='two', cands=cands, pick=f['next'] if f['prev'] not in ends_init else None)
        else:
            k = bisect.bisect_right(mapped, f['entry'])
            nxt = None
            while k < len(mapped):
                if mapped[k] not in members:
                    nxt = norm(unit_of[mapped[k]])
                    break
                k += 1
            f.update(cls='open', cands=tuple(sorted(f['text_cands'])), pick=nxt)
    # propagation: single entries become anchors when no other unit's code lies between
    n = len(facts)
    changed = True
    while changed:
        changed = False
        known = [f['cands'][0] if f['cls'] == 'owned' or f['cls'].startswith('single') else None for f in facts]
        for i in range(n):
            if known[i]:
                continue
            jp = next((j for j in range(i - 1, -1, -1) if known[j]), None)
            jq = next((j for j in range(i + 1, n) if known[j]), None)
            if jp is None or jq is None or known[jp] != known[jq]:
                continue
            unit = known[jp]
            lo, hi = facts[jp]['entry'], facts[jq]['entry']
            if all(code_owner[a] == unit for a in anchors[bisect.bisect_right(anchors, lo):bisect.bisect_left(anchors, hi)]):
                facts[i].update(cls='single:propagated', cands=(unit,), pick=unit)
                changed = True
    # duplicate header static: the previous unit already owns this distinctive shape
    shapes = collections.defaultdict(set)
    for f in facts:
        if f['owner']:
            shapes[f['owner']].add(f['shape'])
    for f in facts:
        if f['cls'] == 'two' and distinctive(f['shape']) and f['shape'] in shapes[f['prev']]:
            f['excluded'] = True
    return dict(ends_init=ends_init, starts_other=starts_other)


def classify_files(facts, regions_by_source, unit_extremes, local_destructors=(), group_of=None):
    """[{file, tier, unit, candidates, entries, reason}] for the address-named files with `_$E`
    rows (regions_by_source: {source: [row]}; unit_extremes: dict(ends_init, starts_other))."""
    index_of = {}
    for f in facts:
        for a in f['members'] + f['dtors']:
            index_of.setdefault(a, f['index'])
    out = []
    for source, rows in sorted(regions_by_source.items()):
        ids = sorted({index_of[r['entry']] for r in rows if r['entry'] in index_of})
        record = dict(file=source, entries=['%08x' % facts[i]['entry'] for i in ids], candidates=[], unit=None, reason='')
        if not ids:
            record.update(tier='stay', reason='not reached from the table (function-local static destructor: moves with '
                          'its registering function)' if any(r['entry'] in local_destructors for r in rows)
                          else 'not reached from the table')
            out.append(record)
            continue
        group = [facts[i] for i in ids]
        classes = {f['cls'] for f in group}
        cands = set(x for f in group for x in f['cands'] if x)
        picks = {f['pick'] for f in group}
        record['candidates'] = sorted(cands)
        if all(c == 'owned' or c.startswith('single') for c in classes) and len(cands) == 1:
            record.update(tier='single', unit=next(iter(cands)), reason='/'.join(sorted(c.split(':')[-1] for c in classes)))
        elif classes <= {'owned', 'single:table', 'single:data', 'single:propagated', 'two'} and len(cands) == 2:
            hyp = set().union(*[f['hyp'] for f in group])
            pick = next(iter(picks)) if len(picks) == 1 else None
            twos = [f for f in group if f['cls'] == 'two']
            prev = twos[0]['prev'] if twos else None
            if pick is None:
                record.update(tier='stay', reason='no prefix pick (%s ends with an initialiser)' % (prev or '?'))
            elif hyp and norm(pick) not in hyp:
                record.update(tier='stay', reason='the unit map names the other candidate (%s)' % ', '.join(sorted(hyp)))
            elif any(f['excluded'] for f in group):
                record.update(tier='xcu-exclusion', unit=pick, reason='%s already owns the shape' % prev)
            elif norm(pick) in hyp:
                record.update(tier='xcu-prefix+map', unit=pick, reason='prefix rule; unit map agrees')
            elif prev in unit_extremes['ends_init'] or pick in unit_extremes['starts_other']:
                record.update(tier='stay', reason='prefix rule does not apply (%s ends with an initialiser or %s does '
                              'not start with its initialisers)' % (prev, pick))
            else:
                record.update(tier='xcu-prefix', unit=pick, reason='prefix rule alone')
        else:
            record.update(tier='group', reason=', '.join(sorted(classes - {'owned'})))
        out.append(record)
    return lift_groups(out, group_of or {})


def lift_groups(records, group_of):
    """Rows one compile group emits from one include (a header-statics group: one emitter, the
    other rows comment-only) are one unit's or none: they cannot be split between tiers. Every
    flagged record of a group whose records share one pick takes the strongest tier among them
    (xcu-exclusion, then xcu-prefix+map, then xcu-prefix); `group_of` {file: group key}."""
    rank = {t: i for i, t in enumerate(FLAG_TIERS)}
    groups = collections.defaultdict(list)
    for r in records:
        if r['file'] in group_of:
            groups[group_of[r['file']]].append(r)
    for members in groups.values():
        flagged = [r for r in members if r['tier'] in rank]
        if len(flagged) < 2 or len({r['unit'] for r in flagged}) != 1:
            continue
        best = min(flagged, key=lambda r: rank[r['tier']])['tier']
        for r in flagged:
            if r['tier'] != best:
                r['reason'] = '%s; lifted from %s: one compile group, one include' % (r['reason'], r['tier'])
                r['tier'] = best
    return records


# ----------------------------------------------------------------------------- inputs

def load_inputs(repository, executable):
    import catalog_io
    import rename_files
    catalog = catalog_io.load(catalog_io.locate(repository), references=False)
    regions = {r['entry']: r for r in catalog['regions']}
    code_owner = {}
    for path in sorted((repository / 'manifests/original_target/modules').glob('*.json')):
        document = json.loads(path.read_text())
        for item in document.get('code', []):
            code_owner[int(item['entry'], 16)] = document['module']
    units = json.loads((repository / 'manifests/original_target/unit_map.json').read_text())['units']
    unit_of = {int(e, 16): u for u, entries in units.items() for e in entries}
    modules = json.loads((repository / 'manifests/original_target/modules.json').read_text())
    source_owner = {v['source']: k for k, v in modules.items() if v.get('source')}
    by_source = collections.defaultdict(list)
    for row in catalog['regions']:
        by_source[row['source']].append(row)
    e_sources = {s: rows for s, rows in by_source.items()
                 if rename_files.address_named(s) and any((r.get('symbol') or '').startswith('_$E') for r in rows)}
    image = Image(Path(executable).read_bytes())
    return catalog, regions, code_owner, source_owner, unit_of, e_sources, image


def local_static_destructors(image, regions, reached):
    """`_$E` rows no table entry reaches whose address an ordinary function pushes (a function-local
    static's destructor registered at first use)."""
    o = image.offset(TEXT_START)
    text = image.data[o:o + 0x39A000]
    entries = sorted(regions)
    out = set()
    for entry, row in regions.items():
        if not (row.get('symbol') or '').startswith('_$E') or entry in reached:
            continue
        pattern = b'\x68' + struct.pack('<I', entry)
        if text.find(pattern) >= 0:
            out.add(entry)
    return out


def report(repository=REPOSITORY, config=DEFAULT_CONFIG):
    executable = json.loads(Path(config).read_text())['executable']
    catalog, regions, code_owner, source_owner, unit_of, e_sources, image = load_inputs(repository, executable)
    slots = table_slots(image, regions)
    facts = entry_facts(image, slots, regions)
    extremes = classify_entries(facts, code_owner, source_owner, unit_of, regions)
    reached = {a for f in facts for a in f['members'] + f['dtors']}
    local = local_static_destructors(image, regions, reached)
    group_of = {source: rows[0]['population'] for source, rows in e_sources.items()}
    files = classify_files(facts, e_sources, extremes, local, group_of)
    for record in files:
        record['file'] = record['file'].replace('src/recovered/', '', 1)
    return dict(slots=len(slots), entries=collections.Counter(f['cls'] for f in facts), files=files,
                ends_init=sorted(extremes['ends_init'] & {f['owner'] for f in facts if f['owner']}),
                starts_other=sorted(extremes['starts_other']))


def main():
    parser = argparse.ArgumentParser(description=__doc__.split('\n\n')[0])
    parser.add_argument('--config', type=Path, default=DEFAULT_CONFIG)
    parser.add_argument('--tier', action='append', choices=TIERS, help='only these tiers')
    parser.add_argument('--json', type=Path, help='also write the records as JSON')
    parser.add_argument('--summary', action='store_true', help='counts only')
    args = parser.parse_args()
    result = report(config=args.config)
    files = [r for r in result['files'] if not args.tier or r['tier'] in args.tier]
    if args.json:
        args.json.write_text(json.dumps(dict(result, files=files, entries=dict(result['entries'])), indent=1) + '\n')
    if not args.summary:
        for r in files:
            print('\t'.join([r['tier'], r['unit'] or '-', ','.join(r['candidates']) or '-', ','.join(r['entries']) or '-',
                             r['file'], r['reason']]))
    tiers = collections.Counter(r['tier'] for r in result['files'])
    print('# table %d entries (%s); %d address-named $E files: %s' % (
        result['slots'], ', '.join('%s %d' % kv for kv in sorted(result['entries'].items())), len(result['files']),
        ', '.join('%s %d' % (t, tiers[t]) for t in TIERS)), file=sys.stderr)


if __name__ == '__main__':
    main()
