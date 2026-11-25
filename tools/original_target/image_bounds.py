#!/usr/bin/env python3
"""image_bounds.py: function bounds derived from the image alone (review session ruling, 2026-11-22; lane O17;
docs/DECISIONS.md "Image-derived bound").

A bound counts as independently bounded when this tool regenerates it from the original image and the existing
manifests, with no draft, candidate or worker ledger as input. Inputs: game.exe (SHA-256 checked), main's
manifests/function_map.csv (rows flagged exact are the bound rows; the non-exact rows' entry addresses are
queries only, their recorded lengths are never read), manifests/original_target/assembly.json regions and the
library region of manifests/original_target/library.json (both bound). A worker's ledger is only the expected
output (--expect).

Rules, one per condition of the ruling:
 (2) start: a direct call (E8 rel32) from a decoded instruction (a bound row decoded linearly, the row's own
     descent, or a row an earlier pass derived), an aligned pointer in .rdata/.data (vtable or data slot; the value
     16-aligned and not amid unaligned .text values, see slot_pointer), a code-address operand of a decoded
     instruction, a call from the row's own body (recursion), or the end of a bound row followed only by padding.
     An opcode site in bytes nobody decoded is unverified (8B E8 is mov ebp, eax): a lenient first pass uses it to
     find candidate bodies, the strict passes then verify it against those bodies and otherwise drop it.
 (3) end: recursive descent from the start (jumps, conditional jumps, switch tables, the row's own code-address
     operands and its C++ EH funclets read from its FuncInfo) gives the code and tables; switch tables are
     attributed only with their bound (a `cmp idx, n` on the path into the indirect jump, through a byte remap
     table where there is one); the end is the next independently evidenced start (a bound row or a referenced
     address) minus bytes verified to be padding (0x90/0xCC). A return alone never ends a row: bytes between the
     last instruction and the next evidenced start that are not padding refuse it. Holes inside the range must
     be padding or VC6 npad forms.
 (4) refusals: a jump out of the range to anything but an evidenced start (shared tail), a jump, call or code
     pointer into the range from outside it (from bytes nobody decoded too), a data pointer to an instruction our
     flow reaches only as a call's fallthrough, a start with no reference, overlap with a bound row, another
     derived row or data, an indirect jump through a table without its bound, undecodable bytes, a body that runs
     into an evidenced start.
 (5) the frozen output is manifests/image_bounds.csv (status `bound` rows); a unit row may use one only as
     `bound: image-derived` once this file's commit precedes the candidate's comparison. This tool binds nothing.

Usage (the matching-pilot python):
  image_bounds.py --write            regenerate manifests/image_bounds.csv
  image_bounds.py --check            regenerate and compare with the committed file (exit 1 on any difference)
  image_bounds.py --entry 0x440520   derive one row and print the evidence
  image_bounds.py --expect LEDGER    compare a worker ledger (entry,end_exclusive,padding_bytes,next_start,bound)
  image_bounds.py --validate 1       hold out each of main's exact rows in turn and compare the derived extent
"""
import argparse
import bisect
import collections
import csv
import hashlib
import json
import os
import struct
import sys
from pathlib import Path

import capstone
from capstone import x86_const as X

TOOLS = Path(__file__).resolve().parent
ROOT = TOOLS.parents[1]
OUT = ROOT / 'manifests' / 'image_bounds.csv'
FUNCTION_MAP = ROOT / 'manifests' / 'function_map.csv'
ASSEMBLY = ROOT / 'manifests' / 'assembly.json'     # the rebuild's ledger (since 2026-11-24; same rows as main's)
LIBRARY = ROOT / 'manifests' / 'original_target' / 'library.json'
CONFIG = ROOT / 'private' / 'original-target.interactive.json'
SHA = 'e6418111c2c36081a1795fb000e0927beea1630170b334c5013d12102602e70f'
PAD = (0x90, 0xCC)
# VC6 npad fill inside a function (alignment before a switch table), exact byte forms only.
NPAD = (bytes.fromhex('8da42400000000'), bytes.fromhex('8d9b00000000'), bytes.fromhex('8d642400'),
        bytes.fromhex('8d4900'), bytes.fromhex('8bc0'), bytes.fromhex('8bff'))
EH_MAGIC = 0x19930520
FIELDS = ['entry', 'end_exclusive', 'body_bytes', 'padding_bytes', 'next_start', 'status', 'reason',
          'start_evidence', 'end_evidence', 'tables', 'eh_roots', 'noreturn_calls', 'body_sha256', 'catalog_symbol']

JUMPS = {X.X86_INS_JMP}
CONDS = {X.X86_INS_JAE, X.X86_INS_JA, X.X86_INS_JBE, X.X86_INS_JB, X.X86_INS_JCXZ, X.X86_INS_JECXZ, X.X86_INS_JE,
         X.X86_INS_JGE, X.X86_INS_JG, X.X86_INS_JLE, X.X86_INS_JL, X.X86_INS_JNE, X.X86_INS_JNO, X.X86_INS_JNP,
         X.X86_INS_JNS, X.X86_INS_JO, X.X86_INS_JP, X.X86_INS_JS, X.X86_INS_LOOP, X.X86_INS_LOOPE,
         X.X86_INS_LOOPNE}
RETS = {X.X86_INS_RET, X.X86_INS_RETF, X.X86_INS_IRET, X.X86_INS_IRETD}
FAMILY = {}
for _fam, _regs in {'eax': ('EAX', 'AX', 'AL', 'AH'), 'ebx': ('EBX', 'BX', 'BL', 'BH'), 'ecx': ('ECX', 'CX', 'CL', 'CH'),
                    'edx': ('EDX', 'DX', 'DL', 'DH'), 'esi': ('ESI', 'SI'), 'edi': ('EDI', 'DI'),
                    'ebp': ('EBP', 'BP'), 'esp': ('ESP', 'SP')}.items():
    for _r in _regs:
        FAMILY[getattr(X, 'X86_REG_' + _r)] = _fam


class Refused(Exception):
    pass


class Image:
    """Sections of a PE image (or a synthetic one for tests): [(name, start, bytes, executable)]."""

    def __init__(self, sections):
        self.sections = sorted(sections, key=lambda s: s[1])
        texts = [s for s in self.sections if s[3]]
        if len(texts) != 1:
            raise SystemExit('image needs exactly one executable section')
        self.text_lo, self.text = texts[0][1], texts[0][2]
        self.text_hi = self.text_lo + len(self.text)

    @classmethod
    def from_pe(cls, path, sha=SHA):
        import pefile
        raw = Path(path).read_bytes()
        if hashlib.sha256(raw).hexdigest() != sha:
            raise SystemExit('hash mismatch: %s' % path)
        pe = pefile.PE(data=raw, fast_load=True)
        base = pe.OPTIONAL_HEADER.ImageBase
        out = []
        for s in pe.sections:
            data = s.get_data()[:min(s.Misc_VirtualSize, s.SizeOfRawData)]
            out.append((s.Name.rstrip(b'\0').decode(), base + s.VirtualAddress, data,
                        bool(s.Characteristics & 0x20000000)))
        return cls(out)

    def in_text(self, a):
        return self.text_lo <= a < self.text_hi

    def read(self, a, n):
        for name, lo, data, exe in self.sections:
            if lo <= a and a + n <= lo + len(data):
                return data[a - lo:a - lo + n]
        return None

    def byte(self, a):
        return self.text[a - self.text_lo] if self.in_text(a) else None

    def dword(self, a):
        b = self.read(a, 4)
        return struct.unpack('<I', b)[0] if b else None


class Bounds:
    def __init__(self, image, bound_rows, catalog=None):
        """bound_rows: [(start, end, kind, name)] already bound (exact rows, assembly, library). catalog:
        {entry: symbol} of main's non-exact rows, queries and labels only."""
        self.img = image
        self.rows = sorted(bound_rows)
        self.row_starts = [r[0] for r in self.rows]
        self.bound_starts = set(self.row_starts)
        self.catalog = catalog or {}
        self.md = capstone.Cs(capstone.CS_ARCH_X86, capstone.CS_MODE_32)
        self.md.detail = True
        self._linear = {}
        self.derived, self.derived_starts = {}, []
        self.strict = False
        self._index()

    # ----- reference index -------------------------------------------------------------------------------
    def _index(self):
        img, t, lo = self.img, self.img.text, self.img.text_lo
        self.rel = collections.defaultdict(list)          # target -> [(site, kind)]
        n = len(t)
        for op, kind in ((0xE8, 'call'), (0xE9, 'jmp')):
            i = t.find(bytes([op]))
            while i != -1:
                if i + 5 <= n:
                    tgt = (lo + i + 5 + struct.unpack_from('<i', t, i + 1)[0]) & 0xFFFFFFFF
                    if img.in_text(tgt):
                        self.rel[tgt].append((lo + i, kind))
                i = t.find(bytes([op]), i + 1)
        i = t.find(b'\x0f')
        while i != -1:
            if i + 6 <= n and 0x80 <= t[i + 1] <= 0x8F:
                tgt = (lo + i + 6 + struct.unpack_from('<i', t, i + 2)[0]) & 0xFFFFFFFF
                if img.in_text(tgt):
                    self.rel[tgt].append((lo + i, 'jcc'))
            i = t.find(b'\x0f', i + 1)
        self.textptr = collections.defaultdict(list)      # value -> [address of the dword inside .text]
        for k in range(4):
            usable = (n - k) // 4 * 4
            for j, (v,) in enumerate(struct.iter_unpack('<I', t[k:k + usable])):
                if lo <= v < img.text_hi:
                    self.textptr[v].append(lo + k + 4 * j)
        self.dataptr = collections.defaultdict(list)      # value -> [aligned slot in a data section]
        for name, slo, data, exe in img.sections:
            if exe:
                continue
            start = (-slo) % 4
            usable = (len(data) - start) // 4 * 4
            for j, (v,) in enumerate(struct.iter_unpack('<I', data[start:start + usable])):
                if lo <= v < img.text_hi:
                    self.dataptr[v].append(slo + start + 4 * j)
        self.rel_keys = sorted(self.rel)
        self.textptr_keys = sorted(self.textptr)
        self.dataptr_keys = sorted(self.dataptr)

    # ----- bound rows ------------------------------------------------------------------------------------
    def row_at(self, a):
        """The bound row containing A, or None (bound rows are merged, so they do not overlap)."""
        i = bisect.bisect_right(self.row_starts, a) - 1
        return self.rows[i] if i >= 0 and a < self.rows[i][1] else None

    def next_row_start(self, a):
        i = bisect.bisect_right(self.row_starts, a)
        return self.row_starts[i] if i < len(self.row_starts) else self.img.text_hi

    def prev_row(self, a):
        i = bisect.bisect_left(self.row_starts, a) - 1
        return self.rows[i] if i >= 0 else None

    def linear(self, row):
        """(instruction starts, 4-byte operand field addresses, branch targets) of a bound row, decoded linearly."""
        key = row[0]
        if key not in self._linear:
            starts, fields, branches = set(), {}, []
            lo, hi = row[0], row[1]
            code = self.img.text[lo - self.img.text_lo:hi - self.img.text_lo]
            off = 0
            while off < len(code):
                got = next(self.md.disasm(code[off:off + 16], lo + off, 1), None)
                if got is None:
                    off += 1
                    continue
                starts.add(got.address)
                if got.imm_offset and got.size - got.imm_offset >= 4:
                    fields[got.address + got.imm_offset] = got
                if got.disp_offset and got.disp_size == 4:
                    fields[got.address + got.disp_offset] = got
                if got.id in JUMPS | CONDS or got.id == X.X86_INS_CALL:
                    if got.operands and got.operands[0].type == X.X86_OP_IMM:
                        branches.append((got.address, got.operands[0].imm, got.mnemonic))
                off += got.size
            self._linear[key] = (starts, fields, branches)
        return self._linear[key]

    def derived_at(self, a, own=None):
        """The derived body (an earlier pass's bound row, not OWN) containing A, or None."""
        i = bisect.bisect_right(self.derived_starts, a) - 1
        if i >= 0:
            body = self.derived[self.derived_starts[i]]
            if body is not own and a < body.hi:
                return body
        return None

    def site_real(self, site, own=None):
        """Whether a rel32 opcode site is a real instruction: True or False when the site lies in the row's own
        decode, a bound row (decoded linearly) or a row an earlier pass derived; None for bytes nobody decoded (a
        lone E8/E9 there may be a modrm byte: 8B E8 is mov ebp, eax)."""
        if own is not None and own.lo <= site < own.hi:
            return site in own.insns
        row = self.row_at(site)
        if row is not None:
            return site in self.linear(row)[0]
        body = self.derived_at(site, own)
        if body is not None:
            return site in body.insns
        return None

    def field_real(self, q, own=None):
        """Whether a 4-byte value at Q in .text is an operand of a real instruction (True/False), or None for
        bytes nobody decoded that follow a push/mov-immediate opcode with a 16-aligned value (else False)."""
        if own is not None and own.lo <= q < own.hi:
            return q in own.fields
        row = self.row_at(q)
        if row is not None:
            return q in self.linear(row)[1]
        body = self.derived_at(q, own)
        if body is not None:
            return q in body.fields
        op = self.img.byte(q - 1)
        if self.img.dword(q) % 16 == 0 and (op == 0x68 or (op is not None and 0xB8 <= op <= 0xBF)):
            return None
        return False

    def slot_pointer(self, slot, v):
        """Whether an aligned data slot holding V reads as a code pointer: V is 16-aligned (VC6 /Gy places every
        function on 16; 17,898 of main's 17,910 exact rows) and neither neighbouring slot holds an unaligned .text
        value (a numeric table such as 0x6cc2f0, 0x6cc51a, ... at 0x8258f4 is not a pointer table)."""
        if v % 16:
            return False
        for n in (slot - 4, slot + 4):
            w = self.img.dword(n)
            if w is not None and self.img.in_text(w) and w % 16:
                return False
        return True

    def start_refs(self, a, own=None):
        """Evidence that A starts a function: [(strength, description)]. strong: a call or code-address operand
        of a decoded instruction; data: a pointer slot (strong unless own flow contradicts it, see Body);
        unverified: an opcode site in bytes nobody decoded (counted only in the lenient first pass)."""
        ev = []
        for site, kind in self.rel.get(a, ()):
            if kind == 'call':
                real = self.site_real(site, own)
                if real:
                    ev.append(('strong', 'call@%08x' % site))
                elif real is None:
                    ev.append(('unverified', 'call?@%08x' % site))
        for slot in self.dataptr.get(a, ()):
            if self.slot_pointer(slot, a):
                ev.append(('data', 'dataslot@%08x' % slot))
        for q in self.textptr.get(a, ()):
            if own is not None and own.lo <= q < own.hi and own.tables_cover(q):
                continue
            real = self.field_real(q, own)
            if real:
                ev.append(('strong', 'codeptr@%08x' % q))
            elif real is None:
                ev.append(('unverified', 'codeptr?@%08x' % q))
        if not self.strict:
            return ev
        return [e for e in ev if e[0] != 'unverified']

    def evidenced_start(self, a, own=None):
        """'strong', 'weak' (data slots or unverified sites only) or None."""
        if a in self.bound_starts:
            return 'strong'
        kinds = {k for k, d in self.start_refs(a, own)}
        return 'strong' if 'strong' in kinds else ('weak' if kinds else None)

    # ----- one row -----------------------------------------------------------------------------------------
    def derive(self, s):
        try:
            return self._derive(s)
        except Refused as e:
            return {'entry': '%08x' % s, 'status': 'refused', 'reason': str(e),
                    'catalog_symbol': self.catalog.get(s, '')}

    def _derive(self, s):
        img = self.img
        if not img.in_text(s):
            raise Refused('start outside .text')
        row = self.row_at(s)
        if row is not None:
            raise Refused(('already bound: %s row' % row[2]) if row[0] == s else
                          'overlap: start inside bound %s row %08x' % (row[2], row[0]))
        limit = self.next_row_start(s)
        body = Body(self, s, limit)
        body.descend()
        # end: skip padding to the next evidenced start
        end = body.code_end
        p = end
        while p < limit and img.byte(p) in PAD:
            p += 1
        body.lo, body.hi = s, end
        if p >= limit:
            p = limit
            end_ev = 'bound row' if p in self.bound_starts else 'end of .text'
        elif p in self.bound_starts:
            end_ev = 'bound row'
        else:
            refs = [d for k, d in self.start_refs(p, body)]
            if not refs:
                raise Refused('return-only end: %d bytes at %08x after the last instruction %08x are neither padding '
                              'nor an evidenced start' % (self._run(p, limit), p, end))
            end_ev = refs[0] + ('' if len(refs) == 1 else ' +%d' % (len(refs) - 1))
        body.holes()
        # start evidence
        ev = [d for k, d in self.start_refs(s, body)]
        if not ev:
            prev = self.prev_row(s)
            if prev and prev[1] <= s and all(img.byte(a) in PAD for a in range(prev[1], s)):
                ev = ['end of bound %s row %08x' % (prev[2], prev[0])]
        if not ev:
            raise Refused('unreferenced start')
        body.interior_refs()
        data = img.text[s - img.text_lo:end - img.text_lo]
        return {'entry': '%08x' % s, 'end_exclusive': '%08x' % end, 'body_bytes': end - s,
                'padding_bytes': p - end, 'next_start': '%08x' % p, 'status': 'bound', 'reason': '',
                'start_evidence': ev[0] + ('' if len(ev) == 1 else ' +%d' % (len(ev) - 1)),
                'end_evidence': end_ev, 'tables': ';'.join(body.table_text()),
                'eh_roots': len(body.eh_roots), 'noreturn_calls': len(body.noreturn),
                'body_sha256': hashlib.sha256(data).hexdigest(), 'catalog_symbol': self.catalog.get(s, ''),
                '_body': body}

    def _run(self, p, limit):
        n = 0
        while p + n < limit and self.img.byte(p + n) not in PAD:
            n += 1
        return n


class Body:
    """Recursive descent of one candidate row in [start, limit)."""

    def __init__(self, bounds, start, limit):
        self.b, self.img = bounds, bounds.img
        self.start, self.limit = start, limit
        self.lo, self.hi = start, limit
        self.insns = {}            # address -> capstone instruction
        self.ends = {}             # end address -> instruction address (fallthrough predecessor)
        self.preds = collections.defaultdict(list)
        self.fields = {}           # 4-byte operand field address -> instruction
        self.switches = {}         # jmp address -> (index register family, table address) unresolved
        self.tables = []           # (kind, address, count)
        self.attributed = set()    # data-section slots read as this row's EH tables
        self.eh_roots = set()
        self.noreturn = []
        self.stops = set()         # instructions that end a path without ret/jmp (noreturn calls)
        self.targets = set()       # internal branch, table and code-pointer targets
        self.contradicted = set()  # weakly evidenced addresses our own flow reaches (not starts)
        self.after_call = set()    # instructions reached as the fallthrough of a call

    def tables_cover(self, a):
        for kind, t, n in self.tables:
            if t <= a < t + n * (4 if kind == 'dword' else 1):
                return True
        return False

    def table_text(self):
        return ['%s@%08x*%d' % (k, t, n) for k, t, n in sorted(self.tables, key=lambda x: x[1])]

    @property
    def code_end(self):
        ends = [a + i.size for a, i in self.insns.items()]
        ends += [t + n * (4 if k == 'dword' else 1) for k, t, n in self.tables]
        return max(ends)

    def internal(self, t, why):
        """Branch target T: True to follow it inside the range; False for a transfer to an evidenced start (a tail
        call); refuse a transfer outside the range to anything else (shared tail). Inside the range a target
        evidenced only weakly (data slots, unverified sites) is ours: our own branch contradicts a start there."""
        ev = self.b.evidenced_start(t, self) if t != self.start else None
        if not (self.start <= t < self.limit):
            if ev:
                return False
            raise Refused('shared tail: %s to %08x outside the range' % (why, t))
        return ev != 'strong'

    def descend(self):
        todo = [self.start]
        while True:
            while todo:
                self._path(todo.pop(), todo)
            progress = False
            for j, (fam, table) in sorted(self.switches.items()):
                got = self._switch(j, fam, table)
                if got is None:
                    continue
                del self.switches[j]
                progress = True
                for t in got:
                    if self.internal(t, 'switch entry'):
                        self.targets.add(t)
                        self.preds[t].append(j)
                        todo.append(t)
                break
            if not progress and not todo:
                break
        if self.switches:
            j = min(self.switches)
            raise Refused('unattributed switch table %08x at indirect jump %08x (no bound on its index)'
                          % (self.switches[j][1], j))
        for k, t, n in self.tables:
            size = n * (4 if k == 'dword' else 1)
            if not (self.start <= t and t + size <= self.limit):
                raise Refused('switch table %08x outside the range' % t)
            for a in self.insns:
                if a < t + size and t < a + self.insns[a].size:
                    raise Refused('switch table %08x overlaps code at %08x' % (t, a))

    def _decode(self, a):
        code = self.img.text[a - self.img.text_lo:a - self.img.text_lo + 16]
        return next(self.b.md.disasm(code, a, 1), None)

    def _path(self, a, todo):
        b = self.b
        prev = None
        while a not in self.insns:
            if not (self.start <= a < self.limit):
                raise Refused('overlap: body runs into bound row at %08x' % a if a >= self.limit else
                              'shared tail: flow to %08x outside the range' % a)
            if a != self.start:
                ev = b.evidenced_start(a, self)
                if ev == 'strong':
                    raise Refused('body runs into evidenced start %08x' % a)
                if ev and prev is not None and prev.id == X.X86_INS_CALL:
                    raise Refused('ambiguous: pointer to %08x, the fallthrough of a call (noreturn?)' % a)
                if ev:
                    self.contradicted.add(a)
            if prev is not None and prev.id == X.X86_INS_CALL:
                self.after_call.add(a)
            i = self._decode(a)
            if i is None:
                raise Refused('undecodable bytes at %08x' % a)
            if a + i.size > self.limit:
                raise Refused('overlap: instruction at %08x crosses bound row %08x' % (a, self.limit))
            self.insns[a] = i
            self.ends[a + i.size] = a
            self._operands(i, todo)
            if i.id in RETS or i.id == X.X86_INS_INT3 or i.id == X.X86_INS_HLT:
                return
            op0 = i.operands[0] if i.operands else None
            if i.id in JUMPS:
                if op0.type == X.X86_OP_IMM:
                    t = op0.imm
                    if self.internal(t, 'jump'):
                        self.preds[t].append(a)
                        self.targets.add(t)
                        todo.append(t)
                elif (op0.type == X.X86_OP_MEM and op0.mem.index and op0.mem.scale == 4 and not op0.mem.base
                      and self.img.in_text(op0.mem.disp & 0xFFFFFFFF)):
                    self.switches[a] = (FAMILY.get(op0.mem.index), op0.mem.disp & 0xFFFFFFFF)
                return
            if i.id in CONDS:
                t = op0.imm
                if self.internal(t, 'conditional jump'):
                    self.preds[t].append(a)
                    self.targets.add(t)
                    todo.append(t)
            nxt = a + i.size
            if i.id == X.X86_INS_CALL and nxt < self.limit and self.img.byte(nxt) in PAD:
                self.noreturn.append(a)    # a call followed by padding: noreturn (CxxThrowException and kin)
                self.stops.add(a)
                return
            prev = i
            a = nxt

    def _operands(self, i, todo):
        if i.imm_offset and i.size - i.imm_offset >= 4:
            self.fields[i.address + i.imm_offset] = i
        if i.disp_offset and i.disp_size == 4:
            self.fields[i.address + i.disp_offset] = i
        if i.id in JUMPS | CONDS or i.id == X.X86_INS_CALL:
            return
        for op in i.operands:
            if op.type != X.X86_OP_IMM:
                continue
            v = op.imm & 0xFFFFFFFF
            if self.img.in_text(v) and self.start < v < self.limit and not self.b.evidenced_start(v, self) \
                    and i.id in (X.X86_INS_PUSH, X.X86_INS_MOV):
                self.targets.add(v)          # own code pointer (the __ehhandler stub)
                todo.append(v)
            elif i.id == X.X86_INS_MOV and self.img.dword(v) == EH_MAGIC:
                self._funcinfo(v, todo)

    def _funcinfo(self, fi, todo):
        """C++ EH FuncInfo: unwind actions and catch handlers are roots of this row; their slots attributed."""
        img = self.img
        states, unwind, ntry, trymap = (img.dword(fi + 4), img.dword(fi + 8), img.dword(fi + 12),
                                        img.dword(fi + 16))
        roots = []
        for k in range(states or 0):
            slot = unwind + 8 * k + 4
            act = img.dword(slot)
            self.attributed.add(slot)
            if act:
                roots.append(act)
        for k in range(ntry or 0):
            tb = trymap + 20 * k
            n, handlers = img.dword(tb + 12), img.dword(tb + 16)
            for h in range(n or 0):
                slot = handlers + 16 * h + 12
                self.attributed.add(slot)
                roots.append(img.dword(slot))
        for r in roots:
            if not (self.start < r < self.limit):
                raise Refused('EH funclet %08x outside the range' % r)
            self.eh_roots.add(r)
            self.targets.add(r)
            todo.append(r)

    def _preds(self, a):
        """Every instruction our flow reaches A from: the straight-line predecessor and every branch to A."""
        out = list(self.preds.get(a, []))
        f = self.ends.get(a)
        if f is not None:
            i = self.insns[f]
            if not (i.id in RETS or i.id in JUMPS or f in self.stops):
                out.append(f)
        return out

    def _bound(self, cur, flag, fam, byte_table, depth, seen):
        """(n, byte table) bounding the index FAM on every path into CUR, or None when some path has none.
        Walking back along each path: FLAG is the conditional jump that consumes the flags at this point and how
        the path left it (jump id, taken), cleared by any other flag-writing instruction. The bound is the
        nearest `cmp fam, imm` whose consuming jump bounds the index the way the path leaves it (unsigned: ja,
        jae or jne not taken, jbe, jb or je taken; n = imm + 1). A byte remap load (movzx/mov r8, byte ptr
        [base + table]) switches the tracked register to its base. Any other write to the tracked register
        between the cmp and the jump (a call clobbers eax/ecx/edx) leaves that path unbounded."""
        preds = self._preds(cur)
        if not preds or depth > 96:
            return None
        found = []
        for p in preds:
            key = (p, cur, flag, fam, byte_table)
            if key in seen:
                found.append((0, byte_table))   # an edge already searched (or a loop back): no new path
                continue
            seen.add(key)
            i = self.insns[p]
            ops = i.operands
            w = FAMILY.get(ops[0].reg) if ops and ops[0].type == X.X86_OP_REG else None
            written = {FAMILY.get(r, r) for r in i.regs_access()[1]}
            if i.id == X.X86_INS_CALL:
                written |= {'eax', 'ecx', 'edx', X.X86_REG_EFLAGS}
            if i.id == X.X86_INS_CMP and w == fam and ops[1].type == X.X86_OP_IMM:
                if flag is None:
                    return None
                jid, taken = flag
                if not ((jid in (X.X86_INS_JA, X.X86_INS_JAE, X.X86_INS_JNE) and not taken) or
                        (jid in (X.X86_INS_JBE, X.X86_INS_JB, X.X86_INS_JE) and taken)):
                    return None
                got = ((ops[1].imm & 0xFFFFFFFF) + 1, byte_table)
            elif i.id in CONDS:
                got = self._bound(p, (i.id, cur != p + i.size), fam, byte_table, depth + 1, seen)
            elif (w == fam and i.id in (X.X86_INS_MOV, X.X86_INS_MOVZX) and byte_table is None and
                  ops[1].type == X.X86_OP_MEM and ops[1].size == 1 and ops[1].mem.base and not ops[1].mem.index
                  and self.img.in_text(ops[1].mem.disp & 0xFFFFFFFF)):
                got = self._bound(p, flag, FAMILY.get(ops[1].mem.base), ops[1].mem.disp & 0xFFFFFFFF, depth + 1,
                                  seen)
            elif fam in written:
                return None                  # the index changes after its bound (or is clobbered): unbounded
            else:
                got = self._bound(p, None if X.X86_REG_EFLAGS in written else flag, fam, byte_table, depth + 1,
                                  seen)
            if got is None:
                return None
            found.append(got)
        if len({t for n, t in found}) != 1:
            return None
        return max(n for n, t in found), found[0][1]

    def _switch(self, j, fam, table):
        """Entries of the switch at J once its index is bounded on every path into it."""
        got = self._bound(j, None, fam, None, 0, set())
        if got is None:
            return None
        n, byte_table = got
        if n > 0x10000:
            return None
        if byte_table is not None:
            data = self.img.read(byte_table, n)
            if data is None:
                return None
            count = max(data) + 1
            self.tables.append(('byte', byte_table, n))
        else:
            count = n
        self.tables.append(('dword', table, count))
        return [self.img.dword(table + 4 * k) for k in range(count)]

    def holes(self):
        """Bytes of [start, code_end) that neither code nor tables cover must be padding or npad forms."""
        covered = bytearray(self.code_end - self.start)
        for a, i in self.insns.items():
            covered[a - self.start:a - self.start + i.size] = b'\1' * i.size
        for k, t, n in self.tables:
            size = n * (4 if k == 'dword' else 1)
            covered[t - self.start:t - self.start + size] = b'\1' * size
        off = 0
        while off < len(covered):
            if covered[off]:
                off += 1
                continue
            a = self.start + off
            if self.img.byte(a) in PAD:
                off += 1
                continue
            got = next((f for f in NPAD if self.img.read(a, len(f)) == f), None)
            if got is None:
                raise Refused('unattributed bytes inside the range at %08x' % a)
            off += len(got)

    def interior_refs(self):
        """References into (start, hi) from outside it refuse the row: a call or jump from a decoded instruction
        (or, in the strict pass, from bytes nobody decoded), a code-address operand, and a data pointer to an
        instruction our flow reaches only as a call's fallthrough (a pointer to an address our own branch or
        straight-line flow reaches, or into the middle of an instruction, is a value, not a start)."""
        b, lo, hi = self.b, self.lo, self.hi
        for keys, kind in ((b.rel_keys, 'rel'), (b.textptr_keys, 'text'), (b.dataptr_keys, 'data')):
            i = bisect.bisect_right(keys, lo)
            while i < len(keys) and keys[i] < hi:
                v = keys[i]
                i += 1
                if kind == 'rel':
                    for site, how in b.rel[v]:
                        if lo <= site < hi:
                            continue
                        real = b.site_real(site, self)
                        if real or (real is None and b.strict):
                            raise Refused('%s into the range at %08x from %08x%s' % (
                                'jump' if how != 'call' else 'call', v, site, '' if real else ' (bytes nobody decoded)'))
                elif kind == 'text':
                    for q in b.textptr[v]:
                        if lo <= q < hi:
                            continue
                        real = b.field_real(q, self)
                        if real or (real is None and b.strict):
                            raise Refused('code pointer into the range at %08x from %08x%s' % (
                                v, q, '' if real else ' (bytes nobody decoded)'))
                else:
                    for slot in b.dataptr[v]:
                        if slot in self.attributed or not b.slot_pointer(slot, v):
                            continue
                        if v in self.insns and v in self.after_call and not self.preds.get(v):
                            raise Refused('data pointer into the range at %08x from %08x' % (v, slot))
        # short jumps from the bound neighbours
        for row in (b.prev_row(lo), b.row_at(self.hi) or None):
            if row is None:
                continue
            for site, t, how in b.linear(row)[2]:
                if lo < t < hi:
                    raise Refused('jump into the range at %08x from bound row %08x' % (t, site))


def cross_check(rows):
    """Derived rows refuse each other on overlap or on a branch from one into another's interior."""
    good = sorted((int(r['entry'], 16), r) for r in rows if r['status'] == 'bound')
    starts = [a for a, r in good]
    bad = {}
    for k in range(len(good) - 1):
        a, r = good[k]
        b2, r2 = good[k + 1]
        if int(r['next_start'], 16) > b2:
            bad[a] = 'overlap with derived row %08x' % b2
            bad[b2] = 'overlap with derived row %08x' % a
    for a, r in good:
        body = r['_body']
        for site, i in body.insns.items():
            if i.id in JUMPS | CONDS or i.id == X.X86_INS_CALL:
                if i.operands and i.operands[0].type == X.X86_OP_IMM:
                    t = i.operands[0].imm
                    j = bisect.bisect_right(starts, t) - 1
                    if j >= 0 and starts[j] != a and starts[j] < t < int(good[j][1]['end_exclusive'], 16):
                        bad[starts[j]] = 'jump into the range at %08x from derived row %08x' % (t, site)
    for a, r in good:
        if a in bad:
            r.update({'status': 'refused', 'reason': bad[a]})
            for f in ('end_exclusive', 'body_bytes', 'padding_bytes', 'next_start', 'start_evidence',
                      'end_evidence', 'tables', 'eh_roots', 'noreturn_calls', 'body_sha256'):
                r.pop(f, None)
    return rows


# ----- the real image ---------------------------------------------------------------------------------------
def image_path():
    if os.environ.get('RA2_GAME_EXE'):
        return os.environ['RA2_GAME_EXE']
    return json.loads(CONFIG.read_text())['executable']


def load_bound_rows():
    rows, catalog = [], {}
    for r in csv.DictReader(open(FUNCTION_MAP)):
        a = int(r['address'], 16)
        if r['flags'].split(';')[0].split(' ')[0] == 'exact':
            rows.append((a, a + int(r['size']), 'exact', r['symbol']))
        else:
            catalog[a] = r['symbol']        # query and label only; its recorded length is never read
    for r in json.loads(ASSEMBLY.read_text())['regions']:
        a = int(r['entry'], 16)
        rows.append((a, a + int(r['bytes']), 'assembly', r['symbol']))
    lo, hi = (int(x, 16) for x in json.loads(LIBRARY.read_text())['region'])
    rows.append((lo, hi, 'library', 'library region'))
    covered = sorted(rows)
    merged = []
    for r in covered:
        if merged and r[0] < merged[-1][1]:
            if r[1] > merged[-1][1]:
                merged[-1] = (merged[-1][0], r[1], merged[-1][2], merged[-1][3])
            continue
        merged.append(r)
    return merged, catalog


def targets(bounds):
    """Queries: main's non-exact entries, the first non-padding byte of every gap between bound rows, and every
    referenced call target and 16-aligned data slot in those gaps. A refused query is still listed."""
    img, out = bounds.img, set(bounds.catalog)
    rows = bounds.rows
    gaps = []
    prev = img.text_lo
    for r in rows:
        if r[0] > prev:
            gaps.append((prev, r[0]))
        prev = max(prev, r[1])
    if prev < img.text_hi:
        gaps.append((prev, img.text_hi))
    for lo, hi in gaps:
        a = lo
        while a < hi and img.byte(a) in PAD:
            a += 1
        if a < hi:
            out.add(a)
        for keys, kind in ((bounds.rel_keys, 'rel'), (bounds.dataptr_keys, 'data')):
            i = bisect.bisect_left(keys, a)
            while i < len(keys) and keys[i] < hi:
                v = keys[i]
                i += 1
                if kind == 'rel' and any(k == 'call' and bounds.site_real(s) for s, k in bounds.rel[v]):
                    out.add(v)
                elif kind == 'data' and v % 16 == 0:
                    out.add(v)
    return sorted(a for a in out if bounds.row_at(a) is None)


def derive_all(bounds, queries, chain):
    todo, done = sorted(queries), {}
    while todo:
        s = todo.pop(0)
        if s in done:
            continue
        r = done[s] = bounds.derive(s)
        if chain and r['status'] == 'bound':
            p = int(r['next_start'], 16)
            if p not in done and bounds.img.in_text(p) and bounds.row_at(p) is None:
                todo.append(p)
    return cross_check([done[a] for a in sorted(done)])


def run(bounds, passes=12):
    """A lenient pass finds the queries and candidate bodies; strict passes then verify every opcode site in
    bytes nobody decoded against the previous pass's bodies (sites there that are not instructions are values),
    refusing what stays unverifiable, until the bound set is stable."""
    bounds.strict = False
    rows = derive_all(bounds, targets(bounds), chain=True)
    queries = [int(r['entry'], 16) for r in rows]
    bounds.strict = True
    key = None
    for _ in range(passes):
        bodies = {int(r['entry'], 16): r['_body'] for r in rows if r['status'] == 'bound'}
        bounds.derived, bounds.derived_starts = bodies, sorted(bodies)
        rows = derive_all(bounds, queries, chain=False)
        new = tuple((r['entry'], r.get('next_start')) for r in rows if r['status'] == 'bound')
        if new == key:
            return rows
        key = new
    raise SystemExit('strict passes did not converge')


def validate(bounds, every):
    """Hold out every EVERY-th exact row of main (its bytes become unbound), derive it, and compare the derived
    extent with main's independently bounded one: a check of the method on known answers."""
    out, lines = collections.Counter(), []
    exact = [r for r in bounds.rows if r[2] == 'exact']
    bounds.strict = True
    for r in exact[::every]:
        k = bounds.row_starts.index(r[0])
        del bounds.rows[k], bounds.row_starts[k]
        bounds.bound_starts.discard(r[0])
        bounds._linear.pop(r[0], None)
        got = bounds.derive(r[0])
        bounds.rows.insert(k, r)
        bounds.row_starts.insert(k, r[0])
        bounds.bound_starts.add(r[0])
        if got['status'] != 'bound':
            out['refused: ' + got['reason'].split(' at ')[0].split(':')[0].split(' %')[0]] += 1
        elif int(got['end_exclusive'], 16) == r[1]:
            out['agrees'] += 1
        elif int(got['end_exclusive'], 16) < r[1] <= int(got['next_start'], 16):
            out['agrees; main\'s extent also takes %s padding' % ('all the' if r[1] == int(got['next_start'], 16)
                                                                   else 'part of the')] += 1
        else:
            out['DISAGREES'] += 1
            lines.append('%08x main end %08x, derived %s (%s)' % (r[0], r[1], got['end_exclusive'], r[3]))
    return out, lines


def write(rows, path):
    with open(path, 'w', newline='') as f:
        w = csv.DictWriter(f, FIELDS, extrasaction='ignore', lineterminator='\n')
        w.writeheader()
        for r in rows:
            w.writerow(r)


def expect(rows, ledger, bounds):
    """Compare a worker ledger's established rows with the tool's output. An entry outside the query set is
    derived for the report only (never written); an entry that is already a bound row is compared with it."""
    mine = {r['entry']: r for r in rows}
    out = collections.Counter()
    lines = []
    for x in csv.DictReader(open(ledger)):
        if not x.get('entry') or x.get('bound', 'established') != 'established':
            out['ledger rows not established (no entry)'] += 1
            continue
        e = x['entry'].lower()
        r = mine.get(e)
        row = bounds.row_at(int(e, 16))
        if r is None and row is not None and row[0] == int(e, 16):
            same = '%08x' % row[1] == x['end_exclusive'].lower()
            out['already a bound %s row, ledger end %s' % (row[2], 'agrees' if same else 'differs')] += 1
            if not same:
                lines.append('%s bound %s row ends %08x; ledger end %s (%s)' % (
                    e, row[2], row[1], x['end_exclusive'], x.get('function', '')))
            continue
        if r is None:
            r = bounds.derive(int(e, 16))
            out['outside the query set'] += 1
            lines.append('%s outside the query set, derived for the report: %s (%s)' % (
                e, r['status'] if r['status'] == 'bound' else r['reason'], x.get('function', '')))
        if r['status'] != 'bound':
            reason = r['reason'].split(' at ')[0].split(':')[0]
            out['refused: ' + reason] += 1
            lines.append('%s refused: %s (%s)' % (e, r['reason'], x.get('function', '')))
        elif (r['end_exclusive'], str(r['padding_bytes']), r['next_start']) == (
                x['end_exclusive'].lower(), x['padding_bytes'], x['next_start'].lower()):
            out['reproduced'] += 1
        else:
            out['disagree'] += 1
            lines.append('%s disagree: tool end %s pad %s next %s; ledger end %s pad %s next %s (%s)' % (
                e, r['end_exclusive'], r['padding_bytes'], r['next_start'], x['end_exclusive'],
                x['padding_bytes'], x['next_start'], x.get('function', '')))
    return out, lines


def main(argv=None):
    ap = argparse.ArgumentParser(description=__doc__.split('\n')[0])
    ap.add_argument('--write', action='store_true')
    ap.add_argument('--check', action='store_true')
    ap.add_argument('--entry', action='append', default=[])
    ap.add_argument('--expect')
    ap.add_argument('--validate', type=int, metavar='EVERY', help='hold out every EVERY-th exact row of main')
    args = ap.parse_args(argv)
    rows_bound, catalog = load_bound_rows()
    bounds = Bounds(Image.from_pe(image_path()), rows_bound, catalog)
    if args.validate:
        out, lines = validate(bounds, args.validate)
        for k, v in sorted(out.items()):
            print('validate %5d  %s' % (v, k))
        for line in lines:
            print('  ' + line)
        return 1 if out['DISAGREES'] else 0
    if args.entry:
        for e in args.entry:
            r = bounds.derive(int(e, 16))
            print(json.dumps({k: v for k, v in r.items() if k != '_body'}, indent=1))
        return 0
    rows = run(bounds)
    c = collections.Counter(r['status'] for r in rows)
    print('queries %d: bound %d (%d bytes), refused %d' % (
        len(rows), c['bound'], sum(r['body_bytes'] for r in rows if r['status'] == 'bound'), c['refused']))
    reasons = collections.Counter(r['reason'].split(' at ')[0].split(':')[0].split(' %')[0]
                                  for r in rows if r['status'] == 'refused')
    for k, v in reasons.most_common():
        print('  refused %4d  %s' % (v, k))
    if args.write:
        write(rows, OUT)
        print('wrote %s' % OUT.relative_to(ROOT))
    if args.check:
        tmp = OUT.with_suffix('.check')
        write(rows, tmp)
        same = tmp.read_bytes() == OUT.read_bytes()
        tmp.unlink()
        print('check: %s' % ('reproduced' if same else 'DIFFERS from the committed file'))
        if not same:
            return 1
    if args.expect:
        out, lines = expect(rows, args.expect, bounds)
        for k, v in sorted(out.items()):
            print('expect %4d  %s' % (v, k))
        for line in lines:
            print('  ' + line)
    return 0


if __name__ == '__main__':
    sys.exit(main())
