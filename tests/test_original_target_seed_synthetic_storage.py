"""Unverified synthetic storage must not become invented C++ locals."""
from pathlib import Path
import sys
import unittest
sys.path.insert(0,str(Path(__file__).resolve().parents[1]/'tools/original_target'))
import seed_candidate as seed

class SyntheticStorageTests(unittest.TestCase):
    def test_unique_read_and_write_refuse_without_declared_storage(self):
        for expression in ('return unique0x100001dc;', 'unique0x10000514=2;'):
            with self.subTest(expression=expression):
                with self.assertRaisesRegex(ValueError, 'Undeclared Ghidra unique storage.*width and storage'):
                    seed.validate_synthetic_storage('{'+expression+'}')

    def test_widened_byte_parameter_and_word_local_refuse(self):
        with self.assertRaisesRegex(ValueError, 'widened.*_param_4.*extent and overlap'):
            seed.validate_synthetic_storage('{_param_4 = CONCAT22(x,y);}', ['char param_4'])
        with self.assertRaisesRegex(ValueError, 'widened.*_local_20'):
            seed.validate_synthetic_storage('{\n short local_20;\n _local_20=CONCAT22(x,y);\n}')

    def test_each_known_narrow_type_is_not_widened_by_name(self):
        for kind in ('char','unsigned char','short','ushort','undefined1','undefined2','bool','const short','volatile char'):
            with self.subTest(kind=kind):
                with self.assertRaisesRegex(ValueError, 'Undeclared widened'):
                    seed.validate_synthetic_storage('{return _param_1;}', [kind+' param_1'])

    def test_explicit_local_parameter_and_extern_declarations_are_retained(self):
        for declaration in ('int unique0x100001dc;', 'double unique0x100001dc;', 'extern int unique0x100001dc;', 'int unique0x100001dc[2];'):
            seed.validate_synthetic_storage('{\n'+declaration+'\n use(unique0x100001dc);\n}')
        seed.validate_synthetic_storage('{return unique0x100001dc;}', ['int unique0x100001dc'])
        seed.validate_synthetic_storage('{\n char param_4;\n int _param_4;\n return _param_4;\n}')

    def test_declared_function_pointers_are_not_unknown_storage(self):
        seed.validate_synthetic_storage('{\n int (*unique0x100001dc)(int);\n use(unique0x100001dc);\n}')

    def test_comments_strings_and_character_literals_do_not_refuse(self):
        seed.validate_synthetic_storage('{\n char param_4;\n // _param_4\n char *s="unique0x100001dc";\n return 0;\n}')

    def test_members_labels_and_function_calls_keep_existing_handling(self):
        for expression in ('object.unique0x100001dc=1;', 'object->_param_4=1;',
                           'ns::unique0x100001dc=1;', 'unique0x100001dc();',
                           'goto unique0x100001dc; unique0x100001dc: return 0;'):
            seed.validate_synthetic_storage('{\n char param_4;\n '+expression+'\n}')

    def test_ordinary_underscored_names_and_unknown_or_wide_bases_are_not_inferred(self):
        for declaration in ('', 'int param_4;', 'double param_4;', 'short param_4[2];', 'short *param_4;'):
            seed.validate_synthetic_storage('{\n'+declaration+'\n return _param_4;\n}')
        seed.validate_synthetic_storage('{return _ordinary_name;}')

    def test_ambiguous_declarations_are_not_reconstructed(self):
        seed.validate_synthetic_storage('{\n short local_20;\n {int local_20; _local_20=1;}\n}')
        seed.validate_synthetic_storage('{\n int other,unique0x100001dc;\n return unique0x100001dc;\n}')

    def test_preprocessor_and_local_type_scope_are_left_alone(self):
        seed.validate_synthetic_storage('{\n#define unique0x100001dc 1\n return unique0x100001dc;\n}')
        seed.validate_synthetic_storage('{typedef int unique0x100001dc;}')

    def test_check_does_not_modify_the_input_or_supply_a_value(self):
        body='{\n int local_20;\n return local_20;\n}'
        self.assertIsNone(seed.validate_synthetic_storage(body))
        self.assertEqual(body,'{\n int local_20;\n return local_20;\n}')

if __name__=='__main__':unittest.main()
