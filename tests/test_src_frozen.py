"""src/ and main's catalog are frozen at the tag src-final (user decision 2026-11-22, brought into force by the
user on 2026-11-24 in the owner session: rebuild/ is the exact-match tree of record from that day).

The test is skipped where the tag is absent (a fresh clone, the public snapshot); where it exists, any
difference between src-final and HEAD under the frozen paths fails it. The native ports keep building from
src/ unchanged; their fixes live under native/, never in src/.
"""
import subprocess
import unittest
from pathlib import Path

REPOSITORY = Path(__file__).resolve().parents[1]
TAG = 'src-final'
FROZEN = ('src', 'manifests/original_target')


def git(*args):
    return subprocess.run(['git', '-C', str(REPOSITORY), *args], capture_output=True, text=True)


class SourceFrozen(unittest.TestCase):
    def test_frozen_paths_unchanged_since_src_final(self):
        if git('rev-parse', '-q', '--verify', 'refs/tags/%s' % TAG).returncode != 0:
            self.skipTest('tag %s absent' % TAG)
        committed = git('diff', '--name-only', TAG, 'HEAD', '--', *FROZEN)
        self.assertEqual(committed.returncode, 0, committed.stderr)
        working = git('diff', '--name-only', 'HEAD', '--', *FROZEN)
        changed = sorted(set(committed.stdout.split()) | set(working.stdout.split()))
        self.assertEqual(changed, [], 'src/ and manifests/original_target/ are frozen at %s (read-only reference; '
                                      'land changes in rebuild/): %s' % (TAG, ', '.join(changed[:10])))


if __name__ == '__main__':
    unittest.main()
