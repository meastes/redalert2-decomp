"""Batch CLI failures preserve refusals and independent later work."""
import contextlib
import io
import json
import sys
import tempfile
import unittest
from pathlib import Path
from unittest import mock

import seed_candidate as seed


class SeedBatchRefusalTests(unittest.TestCase):
    entries = ['00401000', '00402000']

    def setUp(self):
        self.temp = tempfile.TemporaryDirectory()
        self.addCleanup(self.temp.cleanup)
        self.root = Path(self.temp.name)

    def source(self, entry):
        return 'int seed_%08x() { return 0; }\n' % entry

    def candidate(self, entry):
        return self.root / entry / 'candidate.cpp'

    def run_cli(self, render=None, choose=None, force=False, config_text='{}', dumps_error=None):
        args = ['seed_candidate.py', *self.entries, '--output', str(self.root)]
        if choose is None:
            args.append('--no-compile-check')
        else:
            config = self.root / 'compiler.json'
            config.write_text(config_text)
            args += ['--config', str(config)]
        if force:
            args.append('--force')
        stdout, stderr = io.StringIO(), io.StringIO()
        with contextlib.ExitStack() as stack:
            stack.enter_context(mock.patch.object(sys, 'argv', args))
            stack.enter_context(mock.patch('inventory.Inventory', return_value=object()))
            stack.enter_context(mock.patch('seed_fallback.frozen_inventory', return_value=object()))
            stack.enter_context(mock.patch('seed_fallback.has_fallback', return_value=False))
            stack.enter_context(mock.patch.object(seed.catalog_io, 'locate', return_value=self.root))
            stack.enter_context(mock.patch.object(seed.catalog_io, 'load', return_value={}))
            stack.enter_context(mock.patch.object(seed, 'accepted_sources', return_value={}))
            stack.enter_context(mock.patch.object(seed, 'seed', side_effect=render))
            stack.enter_context(mock.patch('seed_fallback.choose_seed', side_effect=choose))
            if dumps_error is not None:
                stack.enter_context(mock.patch.object(seed.json, 'dumps', side_effect=dumps_error))
            stack.enter_context(contextlib.redirect_stdout(stdout))
            stack.enter_context(contextlib.redirect_stderr(stderr))
            status = seed.main()
        return status, stdout.getvalue(), stderr.getvalue()

    def test_refused_first_entry_does_not_block_later_source(self):
        def render(entry, *args):
            if entry == 0x401000:
                raise ValueError('unsupported scalar slice width 3')
            return self.source(entry)
        status, stdout, stderr = self.run_cli(render=render)
        self.assertEqual(status, 1)
        self.assertFalse(self.candidate(self.entries[0]).exists())
        self.assertEqual(self.candidate(self.entries[1]).read_text(), self.source(0x402000))
        self.assertIn('00401000 refused: unsupported scalar slice width 3', stderr)
        self.assertIn(str(self.candidate(self.entries[1])), stdout)

    def test_later_refusal_preserves_successful_first_entry(self):
        def render(entry, *args):
            if entry == 0x402000:
                raise ValueError('unresolved export')
            return self.source(entry)
        status, _, stderr = self.run_cli(render=render)
        self.assertEqual(status, 1)
        self.assertEqual(self.candidate(self.entries[0]).read_text(), self.source(0x401000))
        self.assertFalse(self.candidate(self.entries[1]).exists())
        self.assertIn('00402000 refused: unresolved export', stderr)

    def test_all_refusals_are_reported_without_fabricating_candidates(self):
        status, _, stderr = self.run_cli(render=mock.Mock(side_effect=ValueError('unresolved export')))
        self.assertEqual(status, 1)
        for entry in self.entries:
            self.assertIn(entry + ' refused:', stderr)
            self.assertFalse(self.candidate(entry).exists())

    def test_failed_force_retains_existing_source_and_choice(self):
        candidate = self.candidate(self.entries[0])
        candidate.parent.mkdir()
        candidate.write_text('// existing reviewed scratch source\n')
        choice = candidate.parent / 'seed-choice.json'
        choice.write_text('{"previous": true}\n')
        def render(entry, *args):
            if entry == 0x401000:
                raise ValueError('refused replacement')
            return self.source(entry)
        status, _, _ = self.run_cli(render=render, force=True)
        self.assertEqual(status, 1)
        self.assertEqual(candidate.read_text(), '// existing reviewed scratch source\n')
        self.assertEqual(choice.read_text(), '{"previous": true}\n')
        self.assertEqual(self.candidate(self.entries[1]).read_text(), self.source(0x402000))

    def test_validated_generation_refusal_does_not_block_next_entry(self):
        def choose(entry, *args):
            if entry == 0x401000:
                raise ValueError('No compilable or reviewable seed')
            return dict(source=self.source(entry), compiled=True, seed_mode='typed', attempts=[])
        status, _, stderr = self.run_cli(choose=choose)
        self.assertEqual(status, 1)
        self.assertIn('00401000 refused:', stderr)
        self.assertFalse((self.candidate(self.entries[0]).parent / 'seed-choice.json').exists())
        self.assertEqual(self.candidate(self.entries[1]).read_text(), self.source(0x402000))
        result = json.loads((self.candidate(self.entries[1]).parent / 'seed-choice.json').read_text())
        self.assertTrue(result['compiled'])
        self.assertNotIn('source', result)

    def test_failed_compilation_still_keeps_its_reviewable_source(self):
        def choose(entry, *args):
            return dict(source=self.source(entry), compiled=entry != 0x401000, seed_mode='pass5', attempts=[])
        status, _, stderr = self.run_cli(choose=choose)
        self.assertEqual(status, 1)
        self.assertEqual(stderr, '')
        for entry in self.entries:
            self.assertEqual(self.candidate(entry).read_text(), self.source(int(entry, 16)))
        result = json.loads((self.candidate(self.entries[0]).parent / 'seed-choice.json').read_text())
        self.assertFalse(result['compiled'])

    def test_all_successes_still_return_zero(self):
        status, _, stderr = self.run_cli(render=lambda entry, *args: self.source(entry))
        self.assertEqual(status, 0)
        self.assertEqual(stderr, '')
        for entry in self.entries:
            self.assertEqual(self.candidate(entry).read_text(), self.source(int(entry, 16)))

    def test_invalid_configuration_is_not_misreported_as_export_refusal(self):
        with self.assertRaises(json.JSONDecodeError):
            self.run_cli(choose=lambda *args: None, config_text='{invalid')

    def test_unexpected_generation_failure_is_not_silently_skipped(self):
        with self.assertRaisesRegex(RuntimeError, 'unexpected bug'):
            self.run_cli(render=mock.Mock(side_effect=RuntimeError('unexpected bug')))
        self.assertFalse(self.candidate(self.entries[1]).exists())

    def test_metadata_serialization_failure_is_not_an_export_refusal(self):
        with self.assertRaisesRegex(ValueError, 'bad metadata'):
            self.run_cli(choose=lambda entry, *args: dict(source=self.source(entry), compiled=True),
                         dumps_error=ValueError('bad metadata'))


if __name__ == '__main__':
    unittest.main()
