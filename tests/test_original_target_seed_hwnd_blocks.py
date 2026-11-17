import unittest
import seed_candidate as seed


class HandleAfterBlock(unittest.TestCase):
    def needs_record(self, body):
        return 'struct HWND__ { int unused; };' in seed.sdk_preamble(body, ('HWND window',))

    def test_handle_use_after_control_statement(self):
        for statement in ('if (ok(f(1))) { return 0; }', 'while (ok) { update(); }',
                          'for (int i=0; i<n; ++i) { f(i); }',
                          'switch (i) { default: break; }', 'if (ok) {} else {}'):
            with self.subTest(statement=statement):
                self.assertTrue(self.needs_record('{ '+statement+' window=parent; return window[3].unused; }'))

    def test_nested_control_blocks(self):
        self.assertTrue(self.needs_record('{ if (a) { if (b) { step(); } } window=parent; window[44].unused=2; }'))

    def test_shadow_declaration_and_initializer_list(self):
        self.assertFalse(self.needs_record('{ if (ok) { struct { int v; } window; use(window[1]); } }'))
        self.assertFalse(self.needs_record('{ { struct Named { int a; } *window; use(window[1]); } }'))

    def test_unclassified_block_stays_conservative(self):
        self.assertFalse(self.needs_record('{ { f(); } window=parent; return window[1].unused; }'))

    def test_handle_transport_stays_opaque(self):
        self.assertFalse(self.needs_record('{ if (ready) {} window=parent; return GetParent(window); }'))
