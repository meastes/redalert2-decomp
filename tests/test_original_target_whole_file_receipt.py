"""whole_file_receipt.py: fixed-path receipt in the checkout, shared history in the main checkout."""
from datetime import datetime, timezone
import json
import pathlib
import subprocess
import sys
import tempfile
import unittest

sys.path.insert(0, str(pathlib.Path(__file__).resolve().parents[1] / 'tools' / 'original_target'))
import whole_file_receipt


def git(cwd, *args):
    subprocess.run(['git', '-C', str(cwd), *args], check=True, capture_output=True)


class ReceiptTests(unittest.TestCase):
    def setUp(self):
        self.tmp = tempfile.TemporaryDirectory()
        self.main = pathlib.Path(self.tmp.name) / 'main'
        self.main.mkdir()
        git(self.main, 'init', '-q')
        git(self.main, '-c', 'user.name=t', '-c', 'user.email=t@example.invalid', 'commit', '-q', '--allow-empty', '-m', 'a')
        (self.main / 'tracked.txt').write_text('x\n')
        git(self.main, 'add', 'tracked.txt')
        git(self.main, '-c', 'user.name=t', '-c', 'user.email=t@example.invalid', 'commit', '-q', '-m', 'b')
        self.worktree = pathlib.Path(self.tmp.name) / 'wt'
        git(self.main, 'worktree', 'add', '-q', '--detach', str(self.worktree))

    def tearDown(self):
        self.tmp.cleanup()

    def test_shape(self):
        when = datetime(2026, 11, 14, 17, 0, 5, tzinfo=timezone.utc)
        r = whole_file_receipt.receipt('build.command', 1, 'ab' * 32, 'c' * 40, False, '/w', now=when, build_receipt='x')
        self.assertEqual(r, dict(commit='c' * 40, whole_file_exact=True, output_sha256='ab' * 32,
                                 time_utc='2026-11-14T17:00:05Z', gate='build.command', worktree='/w', build_receipt='x'))
        self.assertTrue(whole_file_receipt.receipt('g', False, None, 'c', True, '/w', now=when)['dirty'])

    def test_worktree_writes_fixed_file_and_main_history(self):
        head = subprocess.run(['git', '-C', str(self.worktree), 'rev-parse', 'HEAD'], capture_output=True,
                              text=True, check=True).stdout.strip()
        whole_file_receipt.record('build.command', True, 'aa', repository=self.worktree)
        whole_file_receipt.record('join_gate.py', False, None, repository=self.worktree, base='HEAD~1')
        fixed = json.loads((self.worktree / 'build/whole-file-receipt.json').read_text())
        self.assertEqual((fixed['gate'], fixed['whole_file_exact'], fixed['commit']), ('join_gate.py', False, head))
        self.assertNotIn('dirty', fixed)
        lines = (self.main / 'build/whole-file-receipts.jsonl').read_text().splitlines()
        self.assertEqual([json.loads(l)['gate'] for l in lines], ['build.command', 'join_gate.py'])
        self.assertFalse((self.main / 'build/whole-file-receipt.json').exists())

    def test_dirty_tracked_change(self):
        (self.worktree / 'tracked.txt').write_text('changed\n')
        (self.worktree / 'untracked.txt').write_text('ignored for dirtiness\n')
        r = whole_file_receipt.record('build.command', True, 'aa', repository=self.worktree)
        self.assertTrue(r['dirty'])

    def test_failure_never_raises(self):
        self.assertIsNone(whole_file_receipt.record('build.command', True, 'aa',
                                                    repository=pathlib.Path(self.tmp.name) / 'not-a-repo'))


if __name__ == '__main__':
    unittest.main()
