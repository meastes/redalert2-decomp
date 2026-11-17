#!/usr/bin/env python3
# SPDX-License-Identifier: GPL-3.0-or-later
"""Compare a save the port wrote in the original's format with the save it was loaded from
(lane C17, stage E): decoded streams, not files.

The two compound files are read stream by stream (the container's own layout, sector order,
slack and times are one class, "container", and not compared). The summary streams are
compared byte for byte. CONTENTS is decompressed (CStreamClass blocks: compressed size,
decompressed size, LZO1X data; the compressor's match choices are one class, "lzo", and not
compared) and compared object by object along the index the port logged while it read the
source and wrote the copy (RA2_NATIVE_SAVE_INDEX: R and W lines, the position in the
decompressed stream, the bytes, the kind and the map). Each differing byte is put in a class:
  vptr      a vtable pointer (the port writes the installed game's vtable)
  pointer   a pointer member or a pointer read on its own (swizzle ids)
  padding   bytes no member of the map covers (the original wrote whatever was there)
  member    a member the game reads back (a bug to fix, unless it is named)
  untyped   bytes between the typed reads (untyped reads and writes)
  state     where the two streams part (a list the load leaves with another count): the
            bytes up to the next object both hold
  summary   a summary stream
and the report gives the count per class and a list per class and member.

  original_save_compare.py --source <original save> --written <port's save> --index <index file>
                           --members <gen/original_layouts_members.json> [--json <report.json>] [--limit N]
"""
import argparse
import collections
import json
import struct
import sys

COMPOUND = bytes([0xD0, 0xCF, 0x11, 0xE0, 0xA1, 0xB1, 0x1A, 0xE1])


def streams(path):
    """{name: bytes} of the root storage's streams of a compound file (version 3 or 4)."""
    b = open(path, 'rb').read()
    if b[:8] != COMPOUND:
        raise ValueError('%s: not a compound file' % path)
    sector = 1 << struct.unpack_from('<H', b, 30)[0]
    mini = 1 << struct.unpack_from('<H', b, 32)[0]
    nfat, dir_start = struct.unpack_from('<II', b, 44)
    cutoff, minifat_start = struct.unpack_from('<II', b, 56)
    difat_start, ndifat = struct.unpack_from('<II', b, 68)
    b += bytes(-len(b) % sector)
    at = lambda s: 512 + s * sector if sector == 512 else (s + 1) * sector
    difat = list(struct.unpack_from('<109I', b, 76))
    s = difat_start
    while s < 0xFFFFFFFA:
        vals = struct.unpack_from('<%dI' % (sector // 4), b, at(s))
        difat += vals[:-1]
        s = vals[-1]
    fat = []
    for s in difat[:nfat]:
        fat += struct.unpack_from('<%dI' % (sector // 4), b, at(s))

    def chain(s):
        out = bytearray()
        while s < 0xFFFFFFFA:
            out += b[at(s):at(s) + sector]
            s = fat[s]
        return bytes(out)
    d = chain(dir_start)
    entries = []
    for i in range(len(d) // 128):
        e = d[i * 128:(i + 1) * 128]
        n = struct.unpack_from('<H', e, 64)[0]
        entries.append((e[:max(n - 2, 0)].decode('utf-16le', 'replace'), e[66], struct.unpack_from('<I', e, 116)[0],
                        struct.unpack_from('<I', e, 120)[0]))
    ministream = chain(entries[0][2])
    minifat = struct.unpack('<%dI' % (len(chain(minifat_start)) // 4), chain(minifat_start)) if minifat_start < 0xFFFFFFFA else ()
    out = {}
    for name, kind, start, size in entries[1:]:
        if kind != 2:
            continue
        if size < cutoff:
            data, s = bytearray(), start
            while s < 0xFFFFFFFA and len(data) < size:
                data += ministream[s * mini:(s + 1) * mini]
                s = minifat[s]
            out[name] = bytes(data[:size])
        else:
            out[name] = chain(start)[:size]
    return out


def lzo1x_decompress(src):
    """LZO1X (the 1 and 999 compressors' format) decompression."""
    out = bytearray()
    ip = 0
    n = len(src)

    def copy_match(dist, length):
        start = len(out) - dist
        if start < 0:
            raise ValueError('lzo: match before the start')
        for k in range(length):
            out.append(out[start + k])
    state = 'loop'
    t = 0
    if src[0] > 17:
        t = src[0] - 17
        ip = 1
        if t < 4:
            state = 'next'
        else:
            out += src[ip:ip + t]
            ip += t
            state = 'first'
    while True:
        if state == 'loop':
            t = src[ip]
            ip += 1
            if t >= 16:
                state = 'match'
                continue
            if t == 0:
                while src[ip] == 0:
                    t += 255
                    ip += 1
                t += 15 + src[ip]
                ip += 1
            out += src[ip:ip + t + 3]
            ip += t + 3
            state = 'first'
        if state == 'first':
            t = src[ip]
            ip += 1
            if t >= 16:
                state = 'match'
                continue
            dist = 1 + 0x0800 + (t >> 2) + (src[ip] << 2)
            ip += 1
            copy_match(dist, 3)
            state = 'done'
        if state == 'match':
            if t >= 64:
                dist = 1 + ((t >> 2) & 7) + (src[ip] << 3)
                ip += 1
                copy_match(dist, (t >> 5) - 1 + 2)
            elif t >= 32:
                t &= 31
                if t == 0:
                    while src[ip] == 0:
                        t += 255
                        ip += 1
                    t += 31 + src[ip]
                    ip += 1
                dist = 1 + ((src[ip] | src[ip + 1] << 8) >> 2)
                ip += 2
                copy_match(dist, t + 2)
            elif t >= 16:
                high = (t & 8) << 11
                t &= 7
                if t == 0:
                    while src[ip] == 0:
                        t += 255
                        ip += 1
                    t += 7 + src[ip]
                    ip += 1
                dist = high + ((src[ip] | src[ip + 1] << 8) >> 2)
                ip += 2
                if dist == 0:
                    break                       # the end marker
                copy_match(dist + 0x4000, t + 2)
            else:
                dist = 1 + (t >> 2) + (src[ip] << 2)
                ip += 1
                copy_match(dist, 2)
            state = 'done'
        if state == 'done':
            t = src[ip - 2] & 3
            if t == 0:
                state = 'loop'
                continue
            state = 'next'
        if state == 'next':
            out += src[ip:ip + t]
            ip += t
            t = src[ip]
            ip += 1
            state = 'match'
        if ip > n:
            raise ValueError('lzo: past the input')
    return bytes(out)


def contents(data):
    """CONTENTS decompressed: CStreamClass blocks (u32 compressed size, u32 size, LZO1X data)."""
    out = bytearray()
    at = 0
    while at + 8 <= len(data):
        csize, size = struct.unpack_from('<II', data, at)
        at += 8
        block = lzo1x_decompress(data[at:at + csize])
        # The last block records the buffer's 65,536 bytes whatever it holds (the
        # original's flush; its reader stops where the game's reads end).
        if len(block) != size and at + csize < len(data):
            raise ValueError('a block decompressed to %d bytes, %d recorded' % (len(block), size))
        out += block
        at += csize
    return bytes(out)


def read_index(path):
    """[(R|W, position, bytes, kind, name, count)] of the CONTENTS streams: per direction the
    stream with the most entries."""
    rows = collections.defaultdict(list)
    for line in open(path, errors='replace'):
        p = line.split()
        if len(p) != 7:
            continue
        rows[(p[0], p[1])].append((p[0], int(p[2]), int(p[3]), p[4], p[5], int(p[6])))
    best = {}
    for (rw, stream), r in rows.items():
        if rw not in best or len(r) > len(best[rw]):
            best[rw] = r
    return best.get('R', []), best.get('W', [])


class Classifier:
    def __init__(self, members):
        self.members = members
        self.cache = {}

    def member_at(self, name, offset):
        """(class, member path) of byte offset in one object of map name."""
        m = self.members.get(name)
        if not m:
            return 'member', name + '.?'
        size = m['i686_size'] or 1
        off = offset % size
        table = self.cache.get(name)
        if table is None:
            table = [None] * size
            for path, at, n, kind in m['members']:
                cls = {'vptr': 'vptr', 'pointer': 'pointer'}.get(kind, 'member')
                label = name + '.' + (path if kind != 'bits' else path.rsplit('.', 1)[0] + '.(bit-field unit)')
                for k in range(at, min(at + max(n, 1), size)):
                    table[k] = (cls, label)
            self.cache[name] = table
        hit = table[off] if off < len(table) else None
        return hit if hit else ('padding', name + '+%d' % off)


def compare(a, b, base_a, base_b, length, tag, classify, found):
    """Differing bytes of a[base_a:] and b[base_b:] over length, into found."""
    for k in range(length):
        if base_a + k >= len(a) or base_b + k >= len(b):
            found[('untyped', 'past the end')] += 1
            break
        if a[base_a + k] != b[base_b + k]:
            found[classify(k) if classify else (tag, tag)] += 1


def main():
    ap = argparse.ArgumentParser(description=__doc__, formatter_class=argparse.RawDescriptionHelpFormatter)
    ap.add_argument('--source', required=True)
    ap.add_argument('--written', required=True)
    ap.add_argument('--index', required=True)
    ap.add_argument('--members', required=True)
    ap.add_argument('--json')
    ap.add_argument('--limit', type=int, default=40)
    a = ap.parse_args()
    sa, sb = streams(a.source), streams(a.written)
    found = collections.Counter()           # (class, member) -> differing bytes
    notes = []
    for name in sorted(set(sa) | set(sb)):
        if name == 'CONTENTS':
            continue
        if name not in sa or name not in sb:
            notes.append('summary stream %s only in the %s' % (name, 'source' if name in sa else 'written save'))
            continue
        if sa[name] != sb[name]:
            found[('summary', name)] += sum(1 for x, y in zip(sa[name], sb[name]) if x != y) + abs(len(sa[name]) - len(sb[name]))
    ca, cb = contents(sa['CONTENTS']), contents(sb['CONTENTS'])
    reads, writes = read_index(a.index)
    members = json.load(open(a.members))
    cl = Classifier(members)
    pa = pb = 0                             # positions after the last compared entry
    paired = 0
    i = j = 0
    last = 'the start'
    same = lambda e: (e[2], 'value' if e[3] in ('signed', 'wide', 'pointer') else e[3], e[4], e[5])
    while i < len(reads) and j < len(writes):
        r, w = reads[i], writes[j]
        if same(r) != same(w):   # (a load may read as an integer what its save writes as a pointer)
            # The streams part (a list the load leaves with another count, ...): go on from the
            # nearest object entry both have again; the bytes between are one difference.
            best = None
            for di in range(0, 400):
                for dj in range(0, 400 - di):
                    if i + di < len(reads) and j + dj < len(writes):
                        x, y = reads[i + di], writes[j + dj]
                        if x[3] == 'object' and same(x) == same(y):
                            best = (di, dj)
                            break
                if best:
                    break
            if not best:
                notes.append('the indexes part at entry %d (after %s) and do not meet again' % (paired, last))
                break
            di, dj = best
            na, nb = reads[i + di][1] - pa, writes[j + dj][1] - pb
            notes.append('entries part after %s: %d entries (%d bytes) read, %d entries (%d bytes) written'
                         % (last, di, na, dj, nb))
            found[('state', 'parted after ' + last)] += max(na, nb)
            pa, pb = reads[i + di][1], writes[j + dj][1]
            i, j = i + di, j + dj
            continue
        # The untyped bytes before this entry.
        gap_a, gap_b = r[1] - pa, w[1] - pb
        if gap_a != gap_b:
            notes.append('untyped gap before entry %d (after %s): %d bytes read, %d written' % (paired, last, gap_a, gap_b))
        compare(ca, cb, pa, pb, min(gap_a, gap_b), 'untyped', None, found)
        if r[3] == 'object':
            name = r[4]
            compare(ca, cb, r[1], w[1], r[2], None, lambda k, n=name: cl.member_at(n, k), found)
            last = name
        else:
            compare(ca, cb, r[1], w[1], r[2], 'pointer' if r[3] in ('pointer', 'wide', 'signed') else 'member', None, found)
        pa, pb = r[1] + r[2], w[1] + w[2]
        paired += 1
        i += 1
        j += 1
    if len(reads) != len(writes):
        notes.append('%d typed reads, %d typed writes' % (len(reads), len(writes)))
    compare(ca, cb, pa, pb, min(len(ca) - pa, len(cb) - pb), 'untyped', None, found)
    if len(ca) - pa != len(cb) - pb:
        notes.append('after the last entry: %d bytes read, %d written' % (len(ca) - pa, len(cb) - pb))
    by_class = collections.Counter()
    for (cls, _), n in found.items():
        by_class[cls] += n
    print('compare: CONTENTS %d bytes decompressed in the source, %d written; %d typed entries paired'
          % (len(ca), len(cb), paired))
    for n in notes:
        print('compare: NOTE: ' + n)
    for cls in ('member', 'pointer', 'vptr', 'padding', 'untyped', 'state', 'summary'):
        print('compare: %-8s %d bytes' % (cls, by_class.get(cls, 0)))
        for (c, m), n in sorted(((k, v) for k, v in found.items() if k[0] == cls), key=lambda kv: -kv[1])[:a.limit]:
            print('    %6d  %s' % (n, m))
    if a.json:
        json.dump({'contents': [len(ca), len(cb)], 'paired': paired, 'notes': notes, 'by_class': by_class,
                   'members': [[c, m, n] for (c, m), n in sorted(found.items(), key=lambda kv: -kv[1])]},
                  open(a.json, 'w'), indent=1)
    return 0


if __name__ == '__main__':
    sys.exit(main())
