"""Tests for rebuild/tools/image_bounds.py: one synthetic image per refusal of the review session's ruling
(2026-11-22, "Image-derived bound"; docs/DECISIONS.md) and the accepted forms they are refused against.

Each image is a .text section at 0x401000 filled with padding (0xCC) plus hand-assembled code, and an optional
.data section; bound rows play the part of main's exact rows.

Run: RA2_TREE=rebuild python -m unittest discover -s rebuild/tests"""
import struct
import sys
import unittest
from pathlib import Path

sys.path.insert(0, str(Path(__file__).resolve().parents[1] / 'tools'))
import image_bounds as B     # noqa: E402

TEXT = 0x401000
DATA = 0x500000
RET = b'\xc3'
FRAME = b'\x55\x8b\xec\x5d\xc3'          # push ebp; mov ebp, esp; pop ebp; ret


def call(site, target):
    return b'\xe8' + struct.pack('<i', target - (site + 5))


def jmp(site, target):
    return b'\xe9' + struct.pack('<i', target - (site + 5))


def image(code, size=0x100, data=None):
    """CODE: {address: bytes} placed in a padded .text; DATA: {address: bytes} in .data."""
    text = bytearray(b'\xcc' * size)
    for a, b in code.items():
        text[a - TEXT:a - TEXT + len(b)] = b
    sections = [('.text', TEXT, bytes(text), True)]
    d = bytearray(0x40)
    for a, b in (data or {}).items():
        d[a - DATA:a - DATA + len(b)] = b
    sections.append(('.data', DATA, bytes(d), False))
    return B.Image(sections)


def derive(code, rows, entry, data=None, strict=True):
    """Derive ENTRY with ROWS [(start, end)] bound; strict as in the tool's final passes."""
    bounds = B.Bounds(image(code, data=data), [(s, e, 'exact', 'row%x' % s) for s, e in rows])
    bounds.strict = strict
    return bounds.derive(entry)


CALLER = TEXT                    # bound row: call F; ret
F = TEXT + 0x10
NEXT = TEXT + 0x20               # bound row after F


def caller_of(target):
    return {CALLER: call(CALLER, target) + RET}


class Accepted(unittest.TestCase):
    def test_called_function_ends_at_the_next_bound_start_minus_padding(self):
        code = {**caller_of(F), F: FRAME, NEXT: RET}
        r = derive(code, [(CALLER, CALLER + 6), (NEXT, NEXT + 1)], F)
        self.assertEqual(r['status'], 'bound', r.get('reason'))
        self.assertEqual((r['end_exclusive'], r['padding_bytes'], r['next_start']),
                         ('%08x' % (F + 5), 11, '%08x' % NEXT))
        self.assertTrue(r['start_evidence'].startswith('call@%08x' % CALLER))

    def test_data_slot_is_start_evidence(self):
        code = {F: FRAME, NEXT: RET}
        r = derive(code, [(NEXT, NEXT + 1)], F, data={DATA + 8: struct.pack('<I', F)})
        self.assertEqual(r['status'], 'bound', r.get('reason'))
        self.assertEqual(r['start_evidence'], 'dataslot@%08x' % (DATA + 8))

    def test_bounded_switch_table_is_attributed_inside_the_range(self):
        # cmp eax, 1; ja L; jmp [eax*4 + T]; L: ret; c0: ret; c1: ret; T (aligned): dd c0, c1
        t = F + 0x10
        body = b'\x83\xf8\x01' + b'\x77\x07' + b'\xff\x24\x85' + struct.pack('<I', t) + RET + RET + RET
        c0, c1 = F + 13, F + 14
        code = {**caller_of(F), F: body, t: struct.pack('<II', c0, c1), TEXT + 0x30: RET}
        r = derive(code, [(CALLER, CALLER + 6), (TEXT + 0x30, TEXT + 0x31)], F)
        self.assertEqual(r['status'], 'bound', r.get('reason'))
        self.assertEqual(r['tables'], 'dword@%08x*2' % t)
        self.assertEqual(r['end_exclusive'], '%08x' % (t + 8))


class Refusals(unittest.TestCase):
    def assertRefused(self, r, reason):
        self.assertEqual(r['status'], 'refused', r)
        self.assertTrue(r['reason'].startswith(reason), r['reason'])

    def test_shared_tail(self):
        # F jumps to the ret inside the next bound row, which is no evidenced start: a shared tail
        g = TEXT + 0x40
        code = {**caller_of(F), F: b'\x55' + jmp(F + 1, g + 2), g: b'\x31\xc0' + RET}
        self.assertRefused(derive(code, [(CALLER, CALLER + 6), (g, g + 3)], F), 'shared tail')

    def test_jump_into_the_range_from_outside(self):
        # the bound caller also jumps to F+3 (pop ebp), an instruction inside F's range
        code = {CALLER: call(CALLER, F) + jmp(CALLER + 5, F + 3), F: FRAME, NEXT: RET}
        self.assertRefused(derive(code, [(CALLER, CALLER + 10), (NEXT, NEXT + 1)], F), 'jump into the range')

    def test_jump_into_the_range_from_bytes_nobody_decoded(self):
        # an E9 site in unbound bytes is unverifiable: the strict pass refuses rather than guess
        code = {**caller_of(F), F: FRAME, NEXT: RET, TEXT + 0x30: jmp(TEXT + 0x30, F + 3)}
        self.assertRefused(derive(code, [(CALLER, CALLER + 6), (NEXT, NEXT + 1)], F), 'jump into the range')

    def test_unreferenced_start(self):
        # nothing calls or points at F and no bound row ends right before it
        code = {TEXT: FRAME, F: RET}
        self.assertRefused(derive(code, [(F, F + 1)], TEXT), 'unreferenced start')

    def test_unverified_call_alone_is_not_a_reference(self):
        # the only "call" to F is an E8 inside bytes nobody decoded (8B E8 is mov ebp, eax)
        code = {TEXT + 0x30: b'\x8b' + call(TEXT + 0x31, F), F: FRAME, NEXT: RET}
        self.assertRefused(derive(code, [(NEXT, NEXT + 1)], F), 'unreferenced start')

    def test_overlap_start_inside_a_bound_row(self):
        code = {**caller_of(F), F: FRAME, NEXT: RET}
        self.assertRefused(derive(code, [(CALLER, CALLER + 6), (F, F + 5)], F + 3), 'overlap')

    def test_overlap_body_runs_into_a_bound_row(self):
        # F's code falls through into the next bound row with no return or padding between them
        code = {**caller_of(F), F: b'\x55\x8b\xec', F + 3: RET}
        self.assertRefused(derive(code, [(CALLER, CALLER + 6), (F + 3, F + 4)], F), 'overlap')

    def test_return_only_end(self):
        # after F's ret come bytes that are neither padding nor an evidenced start (an unreferenced function):
        # the ret alone does not end F
        code = {**caller_of(F), F: FRAME + b'\x33\xc0' + RET, NEXT: RET}
        self.assertRefused(derive(code, [(CALLER, CALLER + 6), (NEXT, NEXT + 1)], F), 'return-only end')

    def test_unattributed_switch_table(self):
        # an indirect jump through a table with no bound on its index on the path into it
        t = F + 0x10
        body = b'\xff\x24\x85' + struct.pack('<I', t) + RET + RET
        code = {**caller_of(F), F: body, t: struct.pack('<II', F + 7, F + 8), TEXT + 0x30: RET}
        self.assertRefused(derive(code, [(CALLER, CALLER + 6), (TEXT + 0x30, TEXT + 0x31)], F),
                           'unattributed switch table')

    def test_switch_bound_that_does_not_hold_on_the_path(self):
        # cmp eax, 1; jbe L (not taken on the fallthrough path to the jump): the index is unbounded there
        t = F + 0x10
        body = b'\x83\xf8\x01' + b'\x76\x07' + b'\xff\x24\x85' + struct.pack('<I', t) + RET + RET + RET
        code = {**caller_of(F), F: body, t: struct.pack('<II', F + 13, F + 14), TEXT + 0x30: RET}
        self.assertRefused(derive(code, [(CALLER, CALLER + 6), (TEXT + 0x30, TEXT + 0x31)], F),
                           'unattributed switch table')

    def test_derived_rows_that_overlap_refuse_each_other(self):
        rows = [{'entry': '%08x' % F, 'status': 'bound', 'next_start': '%08x' % (F + 0x20),
                 'end_exclusive': '%08x' % (F + 0x18), '_body': B.Body.__new__(B.Body)},
                {'entry': '%08x' % (F + 0x10), 'status': 'bound', 'next_start': '%08x' % (F + 0x20),
                 'end_exclusive': '%08x' % (F + 0x15), '_body': B.Body.__new__(B.Body)}]
        for r in rows:
            r['_body'].insns = {}
        out = B.cross_check(rows)
        self.assertEqual([r['status'] for r in out], ['refused', 'refused'])
        self.assertTrue(out[0]['reason'].startswith('overlap with derived row'))


if __name__ == '__main__':
    unittest.main()
