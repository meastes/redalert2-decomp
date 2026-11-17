import sys
import unittest
from pathlib import Path

sys.path.insert(0, str(Path(__file__).resolve().parents[1] / 'tools/original_target'))
import rename_files as rf


def row(symbol):
    return dict(symbol=symbol, entry=0x401000)


class NamingTests(unittest.TestCase):
    def test_member_outside_class_folder(self):
        self.assertEqual(rf.new_name(row('?Add_Accelerator_005b6130@MessageListClass@@QAEXPAX@Z'), False),
                         ('MessageListClass_Add_Accelerator', None))

    def test_member_in_class_folder_is_the_method(self):
        self.assertEqual(rf.new_name(row('?Save@ThemeClass@@QAEJPAUIStream@@@Z'), True), ('Save', None))
        self.assertEqual(rf.new_name(row('??0ThemeClass@@QAE@XZ'), True), ('Constructor', None))
        self.assertEqual(rf.new_name(row('??_GObjectClass@@UAEPAXI@Z'), True), ('Scalar_Deleting_Destructor', None))
        # a template scope needs no spelling inside its class folder
        self.assertEqual(rf.new_name(row('?Clear@?$VectorClass@PAVAnimTypeClass@@@@QAEXXZ'), True), ('Clear', None))

    def test_template_scope_outside_class_folder_keeps_address(self):
        self.assertEqual(rf.new_name(row('?Clear@?$VectorClass@PAVAnimTypeClass@@@@QAEXXZ'), False),
                         (None, 'no spelling'))

    def test_free_function_and_seed_namespace(self):
        self.assertEqual(rf.new_name(row('?Audio_Event_Type_Range_00405110@@YAHH@Z'), False),
                         ('Audio_Event_Type_Range', None))
        self.assertEqual(rf.new_name(row('?unknown_enum_children_005e86c0@seed_005e86c0@@YIXPAUHWND__@@@Z'), False),
                         ('unknown_enum_children', None))

    def test_seed_and_view_names_keep_address(self):
        for symbol in ('?FUN_004010a0@seed_004010a0@@YIIPAE@Z', '?get@Leaf_4dd090@@QAEPAXI@Z',
                       '?target@Call_6bc210_0@@QAEII@Z', '_$E2', '??B?$TTimerClass@VSystemTimerClass@@@@QBEHXZ',
                       '??_I@YGXPAXIHP6EX0@Z@Z', '?Unknown_0047FCC0@@YGHUCoordStruct@@@Z'):
            with self.subTest(symbol=symbol):
                self.assertEqual(rf.new_name(row(symbol), False), (None, 'seed name'))

    def test_operators_take_the_word_form(self):
        self.assertEqual(rf.new_name(row('??HCoordStruct@@QBE?AU0@ABU0@@Z'), False), ('CoordStruct_operator_plus', None))
        self.assertEqual(rf.new_name(row('??8HSVClass@@QBE_NABV0@@Z'), True), ('operator_equal', None))
        self.assertEqual(rf.new_name(row('??9HSVClass@@QBE_NABV0@@Z'), True), ('operator_not_equal', None))
        self.assertEqual(rf.new_name(row('??4PaletteClass@@QAEAAV0@ABV0@@Z'), True), ('operator_assign', None))
        self.assertEqual(rf.new_name(row('??8?$VectorClass@N@@UBE_NABV0@@Z'), True), ('operator_equal', None))
        self.assertEqual(rf.new_name(row('??A?$VectorClass@N@@QAEAANH@Z'), True), ('operator_index', None))
        # a template class outside the class folders still has no spelling
        self.assertEqual(rf.new_name(row('??8?$VectorClass@N@@UBE_NABV0@@Z'), False), (None, 'no spelling'))

    def test_address_tokens(self):
        self.assertTrue(rf.address_named('src/recovered/00405110.cpp'))
        self.assertTrue(rf.address_named('src/recovered/classes/X/X_Save_006e8310.cpp'))
        self.assertFalse(rf.address_named('src/recovered/classes/X/Save.cpp'))
        self.assertFalse(rf.address_named('src/recovered/house.cpp'))
        self.assertEqual(rf.strip_address('Speech_Save_00716790'), 'Speech_Save')


if __name__ == '__main__':
    unittest.main()
