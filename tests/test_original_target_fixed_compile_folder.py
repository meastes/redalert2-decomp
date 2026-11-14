"""The fixed compile folder: every compile runs at one path layout; a /GR compile anywhere else is refused."""
from pathlib import Path
import sys
import tempfile
import unittest
from unittest.mock import patch

sys.path.insert(0, str(Path(__file__).resolve().parents[1] / 'tools' / 'original_target'))
import build
import struct
from verify_manifest import hierarchy_bit_only


class FixedCompileFolderTests(unittest.TestCase):

    def unit(self, folder, args):
        return dict(cwd=folder, source=Path(folder) / 'accepted.cpp', obj=Path(folder) / 'accepted.obj',
                    log=Path(folder) / 'compile.log', returncode=Path(folder) / 'returncode.txt', args=args)

    def test_gr_outside_the_fixed_folder_is_refused(self):
        with tempfile.TemporaryDirectory() as scratch:
            unit = self.unit(scratch, ['/c', '/GR', '/O2'])
            with self.assertRaisesRegex(ValueError, 'outside the fixed compile folder'):
                build.unit_lines(unit, build.win, 'SRC', 'CL')
            build.unit_lines(dict(unit, args=['/c', '/O2']), build.win, 'SRC', 'CL')

    def test_staged_units_use_the_fixed_layout(self):
        with tempfile.TemporaryDirectory() as scratch, tempfile.TemporaryDirectory() as root:
            with patch.object(build, 'FIXED_ROOT', Path(root) / 'ra2c'), patch.object(build, 'REPOSITORY', Path(scratch)):
                (Path(scratch) / 'src').mkdir()
                sources = []
                for i in range(2):
                    folder = Path(scratch) / ('u%d' % i)
                    folder.mkdir()
                    (folder / 'accepted.cpp').write_text('int f%d;\n' % i)
                    sources.append(self.unit(folder, ['/c', '/GR']))
                top, staged = build.stage_fixed(sources)
                try:
                    lengths = set()
                    for original, unit in zip(sources, staged):
                        lines = build.unit_lines(unit, build.win, 'IGNORED', 'CL')
                        self.assertIn('/I' + build.fixed_path_text(top / 'src'), lines[1])
                        self.assertIn('/Fo' + build.fixed_path_text(unit['obj']), lines[1])
                        lengths.add(len(build.fixed_path_text(unit['cwd'])))
                        self.assertEqual(unit['source'].read_text(), Path(original['source']).read_text())
                    self.assertEqual(len(lengths), 1)
                    self.assertTrue((top / 'src').is_symlink())
                    staged[0]['obj'].write_bytes(b'obj')
                finally:
                    build.unstage_fixed(top, sources, staged)
                self.assertEqual(Path(sources[0]['obj']).read_bytes(), b'obj')
                self.assertFalse(Path(sources[1]['obj']).exists())
                self.assertFalse(top.exists())

    def test_layout_check(self):
        root = build.fixed_path_text(build.FIXED_ROOT)
        ok = root + '\\0123abcd\\g0042'
        self.assertTrue(build.fixed_layout_ok(ok, ok + '\\accepted.obj', root + '\\0123abcd\\src'))
        self.assertFalse(build.fixed_layout_ok(ok + 'x', ok + 'x\\accepted.obj', root + '\\0123abcd\\src'))
        self.assertFalse(build.fixed_layout_ok(ok, ok + '\\accepted.obj', root + '\\ffffffff\\src'))

    def test_cache_key_names_the_compile_path(self):
        key = build.group_cache_key(b'int x;', ['/c'], [], {}, {'CL.EXE': 'x'})
        with patch.object(build, 'FIXED_LAYOUT', 'another layout'):
            self.assertNotEqual(key, build.group_cache_key(b'int x;', ['/c'], [], {}, {'CL.EXE': 'x'}))


class HierarchyBitTests(unittest.TestCase):
    def chd(self, attributes, count=3, array=0x7c3800):
        return struct.pack('<IIII', 0, attributes, count, array)

    def test_only_the_ambiguous_bit_of_a_hierarchy_descriptor(self):
        self.assertTrue(hierarchy_bit_only('??_R3X@@8', self.chd(5), self.chd(1)))
        self.assertTrue(hierarchy_bit_only('??_R3X@@8', self.chd(1), self.chd(5)))
        self.assertFalse(hierarchy_bit_only('??_R3X@@8', self.chd(3), self.chd(1)))
        self.assertFalse(hierarchy_bit_only('??_R3X@@8', self.chd(5, 4), self.chd(1)))
        self.assertFalse(hierarchy_bit_only('??_R4X@@6B@', self.chd(5), self.chd(1)))
        self.assertFalse(hierarchy_bit_only('??_R3X@@8', self.chd(1), self.chd(1)[:12]))


if __name__ == '__main__':
    unittest.main()
