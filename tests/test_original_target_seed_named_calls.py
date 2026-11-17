import unittest
from types import SimpleNamespace

from seed_candidate import named_calls


class NamedCallsTests(unittest.TestCase):
    def setUp(self):
        self.rows = {1: {'name': 'Build'}, 2: {'name': 'Build'}, 3: {'name': 'Read'}}
        self.inventory = SimpleNamespace(passes=['first', 'second'], rows=lambda directory: self.rows)

    def resolve(self, body, targets=(), excluded=()):
        return named_calls(body, self.inventory, targets, excluded)

    def test_only_matching_target_is_bound(self):
        self.assertEqual(self.resolve('Build(); Read();', [2]), {'Build': 2, 'Read': 3})

    def test_repeated_address_across_passes_is_one_candidate(self):
        self.assertEqual(self.resolve('Read();'), {'Read': 3})

    def test_ambiguous_names_refuse_even_when_cached(self):
        self.resolve('Build();', [1])
        for targets in ([], [7], [1, 2]):
            with self.subTest(targets=targets), self.assertRaisesRegex(ValueError, 'Ambiguous.*Build'):
                self.resolve('Build();', targets)

    def test_qualified_target_cannot_receive_free_function_prototype(self):
        with self.assertRaisesRegex(ValueError, 'member prototype'):
            self.resolve('Build :: Build (buffer);', [2])

    def test_imports_and_quoted_or_commented_calls_are_skipped(self):
        self.assertEqual(self.resolve('Build(); /*Build();*/ "Read()";', [], {'Build'}), {})


if __name__ == '__main__':
    unittest.main()
