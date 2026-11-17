#!/usr/bin/env python3
"""Is this native build the same game as an earlier one? (lane P2, 2026-11-16)

    native_identity.py snapshot --build DIR [--output JSON]   the identity record of DIR's ra2-native
    native_identity.py compare OLD.json NEW.json              {"verdict": ..., "reasons": [...]}
    native_identity.py detail OLD_DIR NEW_DIR                 the differing atoms, with symbol names

The native gate (native_gate.py) takes a record after each native compile and compares it with the
record of the last build whose played tests passed. Equal records mean the played tests would run
the same program with the same test scripts, so they are not run again; the receipt names the
passing receipt the result rests on.

Two builds compare equal at one of two levels:

* "identical": every section of the linked ra2-native outside __LINKEDIT (code, constants, strings,
  data, zero-fill sizes, unwind tables, init offsets) has the same bytes at the same address, and
  the image loads the same dylibs. __LINKEDIT holds the symbol and string tables, the export trie,
  binding names and the code signature: names live there and nowhere else in a pure rename.
* "equivalent": the image bytes differ, but the link is the same up to names: the same link flags
  and libraries (object paths and the build folder aside), the same number of inputs in the same
  order, and each input object has the same sections, the same bytes and the same relocations once
  every symbol is replaced by the place that defines it. A place is (input index, section,
  atom index, offset in the atom); an atom runs from one symbol's address to the next one's, as
  ld64 cuts them. A name resolves the way ld64 resolves it: the first strong definition in link
  order, else the first weak one, through gen/alias_list.txt; names nothing defines (the system's)
  stay names. The symbol table enters as the set of (place, type bits, desc bits) of every defined
  symbol, so a merge of two names for one place compares equal and a split does not.

Bytes that carry a name, and only these, are compared by kind instead of content:

1. RTTI type names: an atom whose symbol is __ZTS<mangled> and whose bytes are exactly
   "<mangled>\\0" (the class's own mangled name: a renamed class renames it).
2. The link closure (gen/link_closure.c, never shipped): its traps and zero placeholders exist only
   as names (unrecovered code, unsourced data; ordered by name). Each enters as (kind, size, the
   first place that references it); the rest of the file is compared as text with the trap and
   placeholder lines removed.

3. The source folder's path in strings (__FILE__ in native/link, e.g. native_enhancements.cpp's
   Load_String calls; the path of an unnamed type below): a lane worktree and the main checkout
   build the same game.
4. An unnamed or anonymous type's (or a lambda's) line and column in its compiler-made name,
   "(unnamed struct at <file>:<line>:<col>)": a rename that adds or removes lines above it moves
   them (vox.cpp's in 0d65d9378).
5. The type in ra2port::original_type_name<T>()'s __PRETTY_FUNCTION__ text, "... [T = <type>]"
   (port/include/ra2port/prelude.hpp: the static type name original saves look layouts up by;
   the layout table, gen/original_layouts.cpp, is generated from the same sources, so a rename
   renames both).

Carriers 3-5 are replaced inside string atoms; when one changes length inside an atom that holds
other referenced strings after it (a merged block), the later references move and the builds
compare different: the comparison errs towards "different", never towards "equal".

Nothing else is excluded: the recovered sources are built with -DNDEBUG and have no __func__ or
stringised identifiers; any other string that spells a renamed name compares different (and would
be listed here once evidenced). The record also carries the digest of the test scripts and of the
ctest registrations: a changed test never rests on an older pass.
"""
import argparse
import hashlib
import json
import os
from pathlib import Path
import re
import struct
import sys

REPOSITORY = Path(__file__).resolve().parents[2]
VERSION = 1

LC_SEGMENT_64, LC_SYMTAB, LC_LOAD_DYLIB, LC_LOAD_WEAK_DYLIB, LC_REEXPORT_DYLIB = 0x19, 0x2, 0xC, 0x80000018, 0x8000001F
N_STAB, N_TYPE, N_SECT, N_UNDF, N_EXT = 0xE0, 0x0E, 0x0E, 0x0, 0x01
N_WEAK_DEF = 0x0080
ZEROFILL = {0x1, 0xC, 0x12}             # S_ZEROFILL, S_GB_ZEROFILL, S_THREAD_LOCAL_ZEROFILL
ARM64_RELOC_UNSIGNED, ARM64_RELOC_ADDEND = 0, 10
CLOSURE_NAME = 'link_closure.c.o'


def sha(data):
    return hashlib.sha256(data).hexdigest()


class MachO:
    """The parts of a 64-bit Mach-O file the comparison reads."""

    def __init__(self, data, name='?'):
        self.name, self.data = name, data
        magic, _, _, self.filetype, ncmds, _, _, _ = struct.unpack_from('<IiiIIIII', data, 0)
        if magic != 0xFEEDFACF:
            raise ValueError('%s: not a 64-bit Mach-O file' % name)
        self.sections, self.symbols, self.dylibs = [], [], []
        offset = 32
        for _ in range(ncmds):
            cmd, size = struct.unpack_from('<II', data, offset)
            if cmd == LC_SEGMENT_64:
                nsects = struct.unpack_from('<I', data, offset + 64)[0]
                for i in range(nsects):
                    base = offset + 72 + 80 * i
                    sect, seg = (data[base:base + 16].rstrip(b'\0').decode(), data[base + 16:base + 32].rstrip(b'\0').decode())
                    addr, sz, off, align, reloff, nreloc, flags = struct.unpack_from('<QQIIIII', data, base + 32)
                    self.sections.append(dict(seg=seg, sect=sect, addr=addr, size=sz, offset=off, align=align,
                                              reloff=reloff, nreloc=nreloc, flags=flags))
            elif cmd == LC_SYMTAB:
                symoff, nsyms, stroff, _ = struct.unpack_from('<IIII', data, offset + 8)
                for i in range(nsyms):
                    strx, ntype, nsect, ndesc, value = struct.unpack_from('<IBBHQ', data, symoff + 16 * i)
                    end = data.index(b'\0', stroff + strx)
                    self.symbols.append((data[stroff + strx:end].decode(errors='replace'), ntype, nsect, ndesc, value))
            elif cmd in (LC_LOAD_DYLIB, LC_LOAD_WEAK_DYLIB, LC_REEXPORT_DYLIB):
                name_off = struct.unpack_from('<I', data, offset + 8)[0]
                end = data.index(b'\0', offset + name_off)
                self.dylibs.append(data[offset + name_off:end].decode())
            offset += size

    def content(self, s):
        if (s['flags'] & 0xFF) in ZEROFILL or not s['offset']:
            return None
        return self.data[s['offset']:s['offset'] + s['size']]

    def relocations(self, s):
        out = []
        for i in range(s['nreloc']):
            address, info = struct.unpack_from('<iI', self.data, s['reloff'] + 8 * i)
            out.append((address, info & 0xFFFFFF, (info >> 24) & 1, (info >> 25) & 3, (info >> 27) & 1, info >> 28))
        return out


def archive_members(path):
    """[(name, bytes)] of a BSD ar archive (the symbol table member skipped)."""
    data = Path(path).read_bytes()
    if not data.startswith(b'!<arch>\n'):
        raise ValueError('%s: not an ar archive' % path)
    out, offset = [], 8
    while offset + 60 <= len(data):
        header = data[offset:offset + 60]
        name, size = header[:16].decode().strip(), int(header[48:58].decode().strip())
        body = data[offset + 60:offset + 60 + size]
        if name.startswith('#1/'):
            n = int(name[3:])
            name, body = body[:n].rstrip(b'\0').decode(), body[n:]
        if not name.startswith('__.SYMDEF'):
            out.append((name, body))
        offset += 60 + size + (size & 1)
    return out


def link_statement(build):
    """(inputs, archives, flags) of the ra2-native link in DIR/build.ninja."""
    text = (Path(build) / 'build.ninja').read_text(errors='replace')
    match = re.search(r'^build ra2-native: (\S+) (.*?)(?: \| (.*?))?(?: \|\| .*)?\n((?:  .*\n)*)', text, re.M)
    if not match:
        raise ValueError('%s/build.ninja: no ra2-native link statement' % build)
    inputs = match.group(2).split()
    variables = dict(re.findall(r'^  (\w+) = (.*)$', match.group(4), re.M))
    implicit = (match.group(3) or '').split()
    archives = [p for p in implicit + variables.get('LINK_LIBRARIES', '').split() if p.endswith('.a')]
    seen = []
    for p in archives:
        if p not in seen:
            seen.append(p)
    root = str(Path(build).resolve())
    # The build folder's own path (also as written in a copied folder's build.ninja) is not compared.
    flags = {k: re.sub(r'\S*\.(?:o|a)\b', '<input>', re.sub(r'/\S+/(gen/alias_list\.txt)', r'<build>/\1', v.replace(root, '<build>')))
             for k, v in variables.items() if k in ('FLAGS', 'LINK_FLAGS', 'LINK_LIBRARIES', 'LINK_PATH')}
    return inputs, seen, flags


def image_record(path):
    image = MachO(Path(path).read_bytes(), str(path))
    sections = {}
    for s in image.sections:
        if s['seg'] == '__LINKEDIT':
            continue
        body = image.content(s)
        sections['%s,%s' % (s['seg'], s['sect'])] = dict(addr=s['addr'], size=s['size'],
                                                         sha256=sha(body) if body is not None else None)
    return dict(sections=sections, dylibs=image.dylibs)


class Linked:
    """Every input of the link, parsed, with places for symbols."""

    def __init__(self, build):
        self.build = Path(build)
        inputs, archives, self.flags = link_statement(build)
        self.objects = []                      # [(label, MachO)]
        for p in inputs:
            self.objects.append((p, MachO((self.build / p).read_bytes(), p)))
        for a in archives:
            for name, body in archive_members(self.build / a):
                self.objects.append(('%s(%s)' % (Path(a).name, name), MachO(body, name)))
        self.aliases = {}
        alias_file = self.build / 'gen/alias_list.txt'
        if alias_file.exists():
            for line in alias_file.read_text().splitlines():
                parts = line.split()
                if len(parts) == 2:
                    self.aliases[parts[1]] = parts[0]
        found = re.search(r'(/\S+)/native/CMakeLists\.txt', (self.build / 'build.ninja').read_text(errors='replace'))
        self.root = found.group(1).encode() if found else None
        self.closure = next((i for i, (label, _) in enumerate(self.objects) if label.endswith(CLOSURE_NAME)), None)
        self._atoms()
        self._definitions()

    def _atoms(self):
        # per object: per section index (1-based) the sorted atom starts
        self.starts = []
        for _, o in self.objects:
            per = {i + 1: set() for i in range(len(o.sections))}
            for name, ntype, nsect, ndesc, value in o.symbols:
                if not ntype & N_STAB and ntype & N_TYPE == N_SECT and nsect in per:
                    per[nsect].add(value)
            self.starts.append({k: sorted(v) for k, v in per.items()})

    def place(self, obj, nsect, address):
        """(section key, atom index, offset in the atom) of an address in section nsect of obj."""
        import bisect
        starts = self.starts[obj].get(nsect, [])
        s = self.objects[obj][1].sections[nsect - 1]
        i = bisect.bisect_right(starts, address) - 1
        base = starts[i] if i >= 0 else s['addr']
        return ('%s,%s' % (s['seg'], s['sect']), i, address - base)

    def _definitions(self):
        strong, weak, common = {}, {}, {}
        for obj, (_, o) in enumerate(self.objects):
            for name, ntype, nsect, ndesc, value in o.symbols:
                if ntype & N_STAB or not ntype & N_EXT:
                    continue
                if ntype & N_TYPE == N_SECT:
                    table = weak if ndesc & N_WEAK_DEF else strong
                    table.setdefault(name, (obj, nsect, value))
                elif ntype & N_TYPE == N_UNDF and value:
                    if name not in common or value > common[name][1]:
                        common[name] = (obj, value)
        self.strong, self.weak, self.common = strong, weak, common

    def resolve(self, name, depth=0):
        """('def', obj, place) | ('common', obj, size, name) | ('ext', name)."""
        found = self.strong.get(name) or self.weak.get(name)
        if found:
            obj, nsect, value = found
            return ('def', obj, self.place(obj, nsect, value))
        if name in self.aliases and depth < 8:
            return self.resolve(self.aliases[name], depth + 1)
        if name in self.common:
            obj, size = self.common[name]
            return ('common', obj, size, name)
        return ('ext', name)


def object_digests(linked):
    """Each input's name-free digest (the closure's by kind), and the closure's own record."""
    targets = []                     # per object: per section: [(reloc tuple, raw target)]
    closure_refs = {}                # closure key -> smallest referencing place
    for obj, (_, o) in enumerate(linked.objects):
        local = {}
        for i, (name, ntype, nsect, ndesc, value) in enumerate(o.symbols):
            local[i] = (name, ntype, nsect, value)
        per_section = []
        for si, s in enumerate(o.sections):
            rows = []
            for address, symbolnum, pcrel, length, extern, rtype in o.relocations(s):
                source = linked.place(obj, si + 1, s['addr'] + address)
                if rtype == ARM64_RELOC_ADDEND:
                    target = ('addend', symbolnum - (1 << 24) if symbolnum & 0x800000 else symbolnum)
                elif extern:
                    name, ntype, nsect, value = local[symbolnum]
                    if ntype & N_TYPE == N_SECT and not ntype & N_EXT:
                        target = ('def', obj, linked.place(obj, nsect, value))
                    else:
                        target = linked.resolve(name)
                else:
                    target = ('sect', symbolnum)
                rows.append(((source, rtype, length, pcrel, extern), target, address))
                key = closure_key(linked, target) if obj != linked.closure else None
                if key is not None:
                    where = (obj,) + source
                    if key not in closure_refs or where < closure_refs[key]:
                        closure_refs[key] = where
            per_section.append(rows)
        targets.append(per_section)

    def canon(target):
        key = closure_key(linked, target)
        if key is not None:
            return ['closure', key[0], key[1], list(closure_refs.get(key, ()))]
        if target[0] == 'common':
            return ['common', target[2]]
        return list(target)

    digests, carriers = [], dict(rtti=0, source_root=0, unnamed_line=0, type_name=0)
    for obj, (label, o) in enumerate(linked.objects):
        if obj == linked.closure:
            digests.append(None)
            continue
        h = hashlib.sha256()
        sect_digests = {}
        for si, s in enumerate(o.sections):
            hs = hashlib.sha256()
            key = '%s,%s' % (s['seg'], s['sect'])
            hs.update(json.dumps([key, s['flags'], s['align']]).encode())
            body = o.content(s)
            rows = targets[obj][si]
            # non-extern pointers hold a section address: replace it by its place
            patched = bytearray(body) if body is not None else None
            relocs = []
            for (source, rtype, length, pcrel, extern), target, address in rows:
                if target[0] == 'sect' and rtype == ARM64_RELOC_UNSIGNED and length in (2, 3) and patched is not None:
                    width = 4 if length == 2 else 8
                    value = int.from_bytes(patched[address:address + width], 'little')
                    tsect = target[1]
                    target = ('def', obj, linked.place(obj, tsect, value))
                    patched[address:address + width] = b'\0' * width
                relocs.append([list(source), rtype, length, pcrel, extern, canon(target)])
            if patched is None:
                hs.update(b'zerofill %d' % s['size'])
            else:
                starts = linked.starts[obj][si + 1]
                cuts = [s['addr']] + [a for a in starts if a > s['addr']] + [s['addr'] + s['size']]
                names = {}
                for name, ntype, nsect, ndesc, value in o.symbols:
                    if nsect == si + 1 and not ntype & N_STAB and ntype & N_TYPE == N_SECT:
                        names.setdefault(value, []).append(name)
                for a, b in zip(cuts, cuts[1:]):
                    if b < a:
                        continue
                    chunk = bytes(patched[a - s['addr']:b - s['addr']])
                    rtti = [n for n in names.get(a, []) if n.startswith('__ZTS') and chunk == n[5:].encode() + b'\0']
                    if rtti:
                        carriers['rtti'] += 1
                        chunk = b'<RTTI type name>'
                    elif linked.root:
                        chunk = scrub(chunk, linked.root, carriers)
                    hs.update(struct.pack('<I', len(chunk)) + chunk)
            relocs.sort()
            hs.update(json.dumps(relocs, separators=(',', ':')).encode())
            sect_digests[key + '#%d' % si] = hs.hexdigest()
            h.update(hs.digest())
        defined = set()
        for name, ntype, nsect, ndesc, value in o.symbols:
            if not ntype & N_STAB and ntype & N_TYPE == N_SECT:
                defined.add((linked.place(obj, nsect, value), ntype, ndesc))
        h.update(json.dumps(sorted(defined)).encode())
        digests.append(dict(input=label, sha256=h.hexdigest(), sections=sect_digests))
    closure = None
    if linked.closure is not None:
        label, o = linked.objects[linked.closure]
        items = sorted([k[0], k[1] or 0, list(v)] for k, v in closure_refs.items())
        source = linked.build / 'gen/link_closure.c'
        template = '\n'.join(l for l in source.read_text(errors='replace').splitlines()
                             if not re.match(r'(?:void ra2_trap_|unsigned char ra2_hole_)', l)) if source.exists() else ''
        closure = dict(input=label, template_sha256=sha(template.encode()), items=len(items),
                       items_sha256=sha(json.dumps(items).encode()), list=items)
    return digests, closure, carriers


TYPE_NAME = re.compile(rb'(original_type_name\(\) \[T = )[^\]\0]*(\])')
UNNAMED = re.compile(rb'(\((?:unnamed|anonymous) \w+ at |\(lambda at )([^\0)]*?):\d+:\d+\)')


def scrub(chunk, root, carriers):
    """A string atom with the name carriers 3-5 replaced (see the module comment)."""
    if root in chunk:
        carriers['source_root'] += chunk.count(root)
        chunk = chunk.replace(root, b'<root>')
    chunk, n = UNNAMED.subn(rb'\1\2)', chunk)
    carriers['unnamed_line'] += n
    chunk, n = TYPE_NAME.subn(rb'\1<type>\2', chunk)
    carriers['type_name'] += n
    return chunk


def closure_key(linked, target):
    """(kind, size, ident) for a symbol the link closure defines (a trap or a placeholder), else
    None. ident tells the closure's symbols apart within one build (its atom or its name); only
    kind, size and the first referencing place enter the record."""
    if linked.closure is None:
        return None
    if target[0] == 'def' and target[1] == linked.closure:
        key, atom, delta = target[2]
        cache = linked.__dict__.setdefault('closure_starts', {})
        if key not in cache:
            o = linked.objects[linked.closure][1]
            cache[key] = sorted(v for n, s in linked.starts[linked.closure].items()
                                if '%s,%s' % (o.sections[n - 1]['seg'], o.sections[n - 1]['sect']) == key for v in s)
        starts = cache[key]
        size = starts[atom + 1] - starts[atom] if 0 <= atom < len(starts) - 1 else None
        return ('trap' if key == '__TEXT,__text' else 'data:' + key, size, (key, atom, delta))
    if target[0] == 'common' and target[1] == linked.closure:
        return ('placeholder', target[2], target[3])
    return None


def tests_digest(build):
    """The test scripts (native/tests) and the ctest registrations of DIR."""
    h = hashlib.sha256()
    for p in sorted((REPOSITORY / 'native/tests').rglob('*')):
        if p.is_file() and '__pycache__' not in p.parts:
            h.update(p.relative_to(REPOSITORY).as_posix().encode() + b'\0' + p.read_bytes())
    ctest = Path(build) / 'CTestTestfile.cmake'
    if ctest.exists():
        h.update(ctest.read_text(errors='replace').replace(str(Path(build).resolve()), '<build>').encode())
    return h.hexdigest()


def snapshot(build):
    build = Path(build).resolve()
    linked = Linked(build)
    digests, closure, carriers = object_digests(linked)
    return dict(version=VERSION, image=image_record(build / 'ra2-native'), link_flags=linked.flags,
                inputs=len(digests), objects=digests, closure=closure, carriers=carriers,
                tests_sha256=tests_digest(build))


def compare(old, new):
    """{'verdict': 'identical' | 'equivalent' | 'different', 'reasons': [...], 'tests_same': bool}."""
    reasons = []
    tests_same = old.get('tests_sha256') == new.get('tests_sha256')
    if old.get('version') != new.get('version'):
        return dict(verdict='different', reasons=['record versions differ'], tests_same=tests_same)
    if old['image'] == new['image']:
        return dict(verdict='identical', reasons=[], tests_same=tests_same)
    for key in sorted(set(old['image']['sections']) | set(new['image']['sections'])):
        a, b = old['image']['sections'].get(key), new['image']['sections'].get(key)
        if a != b:
            reasons.append('image %s: %s' % (key, 'size %s -> %s' % (a and a['size'], b and b['size'])
                                              if (a and a['size']) != (b and b['size']) else 'bytes or address'))
    level_b = []
    if old['link_flags'] != new['link_flags']:
        level_b.append('link flags or libraries differ')
    if old['image']['dylibs'] != new['image']['dylibs']:
        level_b.append('dylibs differ')
    if old['inputs'] != new['inputs']:
        level_b.append('input count %d -> %d' % (old['inputs'], new['inputs']))
    else:
        for i, (a, b) in enumerate(zip(old['objects'], new['objects'])):
            if a is None or b is None:
                if (a is None) != (b is None):
                    level_b.append('input %d: link closure position moved' % i)
                continue
            if a['sha256'] != b['sha256']:
                sects = sorted(k for k in set(a['sections']) | set(b['sections']) if a['sections'].get(k) != b['sections'].get(k))
                level_b.append('input %d %s: %s' % (i, b['input'], ', '.join(sects) or 'symbol table (places or flags)'))
    if (old.get('closure') or {}) != (new.get('closure') or {}):
        a, b = old.get('closure') or {}, new.get('closure') or {}
        what = [k for k in ('template_sha256', 'items', 'items_sha256') if a.get(k) != b.get(k)]
        if what:
            gone = [x for x in a.get('list', []) if x not in b.get('list', [])]
            new = [x for x in b.get('list', []) if x not in a.get('list', [])]
            refs = sorted({x[2][0] for x in gone + new if x[2]})
            level_b.append('link closure: %s (items %s -> %s; %d changed, first referenced from inputs %s)'
                           % (', '.join(what), a.get('items'), b.get('items'), len(new), refs[:8]))
    if not level_b:
        return dict(verdict='equivalent', reasons=reasons, tests_same=tests_same)
    return dict(verdict='different', reasons=level_b + reasons, tests_same=tests_same)


def detail(old_dir, new_dir, limit=40):
    """The differing atoms of two build folders, with their symbol names."""
    a, b = Linked(old_dir), Linked(new_dir)
    da, _, _ = object_digests(a)
    db, _, _ = object_digests(b)
    out = []
    for i, (x, y) in enumerate(zip(da, db)):
        if x is None or y is None or x['sha256'] == y['sha256']:
            continue
        oa, ob = a.objects[i][1], b.objects[i][1]
        for key in sorted(set(x['sections']) | set(y['sections'])):
            if x['sections'].get(key) == y['sections'].get(key):
                continue
            si = int(key.split('#')[1])
            def atoms(linked, o, obj):
                s = o.sections[si]
                body = o.content(s) or b''
                starts = linked.starts[obj][si + 1]
                names = {}
                for name, ntype, nsect, ndesc, value in o.symbols:
                    if nsect == si + 1 and not ntype & N_STAB and ntype & N_TYPE == N_SECT:
                        names.setdefault(value, []).append(name)
                cuts = [s['addr']] + [v for v in starts if v > s['addr']] + [s['addr'] + s['size']]
                return [(names.get(p, ['?'])[0], body[p - s['addr']:q - s['addr']]) for p, q in zip(cuts, cuts[1:])]
            xa, xb = atoms(a, oa, i), atoms(b, ob, i)
            diffs = [(n1, n2, len(c1), len(c2)) for (n1, c1), (n2, c2) in zip(xa, xb) if c1 != c2]
            out.append(dict(input=y['input'], section=key, atoms=(len(xa), len(xb)), differing_bytes=diffs[:limit]))
    return out


def main():
    parser = argparse.ArgumentParser(description=__doc__, formatter_class=argparse.RawDescriptionHelpFormatter)
    sub = parser.add_subparsers(dest='command', required=True)
    p = sub.add_parser('snapshot')
    p.add_argument('--build', type=Path, required=True)
    p.add_argument('--output', type=Path)
    p = sub.add_parser('compare')
    p.add_argument('old', type=Path)
    p.add_argument('new', type=Path)
    p = sub.add_parser('detail')
    p.add_argument('old', type=Path)
    p.add_argument('new', type=Path)
    args = parser.parse_args()
    if args.command == 'snapshot':
        record = snapshot(args.build)
        text = json.dumps(record, indent=None, separators=(',', ':')) + '\n'
        if args.output:
            args.output.parent.mkdir(parents=True, exist_ok=True)
            args.output.write_text(text)
        print(json.dumps(dict(inputs=record['inputs'], carriers=record['carriers'], closure=record['closure'],
                              image_text=record['image']['sections'].get('__TEXT,__text'))))
    elif args.command == 'compare':
        print(json.dumps(compare(json.loads(args.old.read_text()), json.loads(args.new.read_text())), indent=1))
    else:
        print(json.dumps(detail(args.old, args.new), indent=1))


if __name__ == '__main__':
    main()
