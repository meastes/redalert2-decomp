"""RA2_SDK_HEADERS migration switch: the hash-pinned SDK header set and the
angle-include rule (wrapper headers only, inside the switch, pinned files only)."""
import hashlib
import json
from pathlib import Path
import sys
import tempfile
import unittest

sys.path.insert(0, str(Path(__file__).resolve().parents[1]/'tools/original_target'))
import build
from canonical import (SDK_WRAPPERS, sdk_pin, sdk_resolve, sdk_tree_digest, validate_includes,
                       verify_sdk_tree, TOOLCHAIN_MANIFEST)

PIN = {'WINDOWS.H': 'a' * 64, 'COMDEF.H': 'b' * 64, 'GL/GL.H': 'c' * 64}
HEADERS = {'shared/x.hpp': b''}
WRAPPER = 'shared/win.hpp'


def wrap(body):
    return ('#ifndef RA2_SHARED_WIN_HPP\n#define RA2_SHARED_WIN_HPP\n#ifdef RA2_SDK_HEADERS\n%s\n#endif\n#endif\n'
            % body).encode()


class AngleIncludeRule(unittest.TestCase):
    def test_wrapper_inside_switch_accepts_pinned_case_insensitive(self):
        self.assertEqual(validate_includes(wrap('#define WIN32_LEAN_AND_MEAN\n#include <windows.h>\n#include "shared/x.hpp"'),
                                           HEADERS, WRAPPER, PIN), {'shared/x.hpp'})
        validate_includes(b'#if defined(RA2_SDK_HEADERS)\n#include <gl/gl.h>\n#endif\n', HEADERS, WRAPPER, PIN)
        validate_includes(b'#ifdef RA2_SDK_HEADERS\n#ifndef FOO\n#include <comdef.h>\n#endif\n#endif\n',
                          HEADERS, WRAPPER, PIN)




    def test_unbalanced_conditionals_refuse(self):
        for text in [b'#ifdef RA2_SDK_HEADERS\n#include <windows.h>\n', b'#endif\n', b'#else\n']:
            with self.subTest(text=text), self.assertRaisesRegex(ValueError, 'Unbalanced'):
                validate_includes(text, HEADERS, WRAPPER, PIN)


    def test_wrapper_set_is_fixed(self):
        self.assertEqual(set(SDK_WRAPPERS), {'shared/irtti.hpp', 'shared/win.hpp', 'shared/sdk_prelude.hpp'})


class HashPin(unittest.TestCase):
    def test_digest_is_the_sorted_shasum_listing(self):
        listing = ''.join('%s  ./%s\n' % (PIN[n], n) for n in sorted(PIN))
        self.assertEqual(sdk_tree_digest(PIN), hashlib.sha256(listing.encode()).hexdigest())

    def test_manifest_pin_is_consistent(self):
        record = json.loads(Path(TOOLCHAIN_MANIFEST).read_text())['sdk_headers']
        files = sdk_pin()
        self.assertEqual(record['status'], 'hypothesis')
        self.assertEqual(record['file_count'], len(files))
        self.assertEqual(record['tree_sha256'], sdk_tree_digest(files))
        for name in ('windows.h', 'comdef.h', 'mmsystem.h', 'windowsx.h', 'cstddef', 'cmath', 'cstring',
                     'cstdlib', 'new.h'):
            self.assertIsNotNone(sdk_resolve(name, files), name)
        for word in ('Platform SDK', 'DirectX', 'Bink', 'Westwood Online'):
            self.assertIn(word, record['note'])

    def test_tampered_pin_refuses(self):
        with tempfile.TemporaryDirectory() as temp:
            manifest = Path(temp)/'toolchain.json'
            files = dict(PIN)
            manifest.write_text(json.dumps(dict(sdk_headers=dict(files=files, tree_sha256=sdk_tree_digest(files)))))
            self.assertEqual(sdk_pin(manifest), files)
            files['WINDOWS.H'] = 'd' * 64
            manifest.write_text(json.dumps(dict(sdk_headers=dict(files=files, tree_sha256=sdk_tree_digest(PIN)))))
            with self.assertRaisesRegex(ValueError, 'tree digest'):
                sdk_pin(manifest)

    def test_local_tree_must_equal_the_pin(self):
        with tempfile.TemporaryDirectory() as temp:
            root = Path(temp)
            (root/'GL').mkdir()
            (root/'WINDOWS.H').write_bytes(b'w'); (root/'GL/GL.H').write_bytes(b'g')
            files = {'WINDOWS.H': hashlib.sha256(b'w').hexdigest(), 'GL/GL.H': hashlib.sha256(b'g').hexdigest()}
            self.assertEqual(verify_sdk_tree(root, files), sdk_tree_digest(files))
            (root/'EXTRA.H').write_bytes(b'x')
            with self.assertRaisesRegex(ValueError, 'differs'):
                verify_sdk_tree(root, files)
            (root/'EXTRA.H').unlink(); (root/'WINDOWS.H').write_bytes(b'changed')
            with self.assertRaisesRegex(ValueError, 'differs'):
                verify_sdk_tree(root, files)


class BuildSwitch(unittest.TestCase):

    def test_converted_unit_searches_sdk_after_src(self):
        unit = dict(cwd='/r/c', source='/r/c/a.cpp', obj='/r/c/a.obj', log='/r/c/l', returncode='/r/c/rc',
                    args=['/c'])
        plain = build.unit_lines(dict(unit), str, 'SRC', 'CL', 'SDK')[1]
        self.assertNotIn('/ISDK', plain)
        lines = build.unit_lines(dict(unit, sdk=True), str, 'SRC', 'CL', 'SDK')[1]
        self.assertLess(lines.index('"/ISRC"'), lines.index('"/ISDK"'))
        with self.assertRaisesRegex(ValueError, 'SDK header directory'):
            build.unit_lines(dict(unit, sdk=True), str, 'SRC', 'CL')

    def test_cache_key_covers_the_pin_only_for_converted_units(self):
        plain = b'#include "shared/x.hpp"\n'
        converted = b'#define RA2_SDK_HEADERS\n' + plain
        key = lambda text: build.group_cache_key(text, ['/c'], [], {}, {'CL.EXE': 'x'})
        self.assertNotEqual(key(plain), key(converted))
        self.assertEqual(key(plain), build.group_cache_key(plain, ['/c'], [], {}, {'CL.EXE': 'x'}))


if __name__ == '__main__':
    unittest.main()
