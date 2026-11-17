// Behaviour rows for native M22 (Scout1177): the release_buffer_<addr> / release_object_<addr>
// callees of the seed-generated vector destructors (the scalar deleting destructors of the
// VectorClass/DynamicVectorClass instantiations under src/recovered/classes/*Vector*). In every
// one the original calls the CRT's free (0x00781D3B; checked against each address's pass-6a
// decompile), the buffer when the vector owns it and the object itself for a deleting call.
// NOT EXACT. Natively the vectors are the vector_class.hpp templates; these seed leaves are not
// in any native vtable. One line per name the link closure listed (generated from
// gen/link_closure.json; a name an accepted source later defines is weakened as a duplicate).
void Free_Memory_00781d3b(void *block);   // link/behaviour_m22_options.cpp
extern "C" {
void release_buffer_40b230(void *block) { Free_Memory_00781d3b(block); }
void release_buffer_40bbc0(void *block) { Free_Memory_00781d3b(block); }
void release_buffer_40ce20(void *block) { Free_Memory_00781d3b(block); }
void release_buffer_40e8d0(void *block) { Free_Memory_00781d3b(block); }
void release_buffer_40e920(void *block) { Free_Memory_00781d3b(block); }
void release_buffer_40ef80(void *block) { Free_Memory_00781d3b(block); }
void release_buffer_40efd0(void *block) { Free_Memory_00781d3b(block); }
void release_buffer_40fc30(void *block) { Free_Memory_00781d3b(block); }
void release_buffer_40fc80(void *block) { Free_Memory_00781d3b(block); }
void release_buffer_4686d0(void *block) { Free_Memory_00781d3b(block); }
void release_buffer_468720(void *block) { Free_Memory_00781d3b(block); }
void release_buffer_491340(void *block) { Free_Memory_00781d3b(block); }
void release_buffer_491390(void *block) { Free_Memory_00781d3b(block); }
void release_buffer_4b28b0(void *block) { Free_Memory_00781d3b(block); }
void release_buffer_4b2900(void *block) { Free_Memory_00781d3b(block); }
void release_buffer_4b45f0(void *block) { Free_Memory_00781d3b(block); }
void release_buffer_4b4640(void *block) { Free_Memory_00781d3b(block); }
void release_buffer_4b5770(void *block) { Free_Memory_00781d3b(block); }
void release_buffer_4ba320(void *block) { Free_Memory_00781d3b(block); }
void release_buffer_4ba370(void *block) { Free_Memory_00781d3b(block); }
void release_buffer_4dbbf0(void *block) { Free_Memory_00781d3b(block); }
void release_buffer_4dbc40(void *block) { Free_Memory_00781d3b(block); }
void release_buffer_4dbc90(void *block) { Free_Memory_00781d3b(block); }
void release_buffer_4dbce0(void *block) { Free_Memory_00781d3b(block); }
void release_buffer_4dbd30(void *block) { Free_Memory_00781d3b(block); }
void release_buffer_4dbd80(void *block) { Free_Memory_00781d3b(block); }
void release_buffer_4dbdd0(void *block) { Free_Memory_00781d3b(block); }
void release_buffer_4dbe20(void *block) { Free_Memory_00781d3b(block); }
void release_buffer_4dbe70(void *block) { Free_Memory_00781d3b(block); }
void release_buffer_4dbec0(void *block) { Free_Memory_00781d3b(block); }
void release_buffer_4dbf10(void *block) { Free_Memory_00781d3b(block); }
void release_buffer_4dbf60(void *block) { Free_Memory_00781d3b(block); }
void release_buffer_4dbfb0(void *block) { Free_Memory_00781d3b(block); }
void release_buffer_4dc000(void *block) { Free_Memory_00781d3b(block); }
void release_buffer_4dc050(void *block) { Free_Memory_00781d3b(block); }
void release_buffer_4dc0a0(void *block) { Free_Memory_00781d3b(block); }
void release_buffer_4dc0f0(void *block) { Free_Memory_00781d3b(block); }
void release_buffer_4dc140(void *block) { Free_Memory_00781d3b(block); }
void release_buffer_4dc190(void *block) { Free_Memory_00781d3b(block); }
void release_buffer_4dc1e0(void *block) { Free_Memory_00781d3b(block); }
void release_buffer_4dc230(void *block) { Free_Memory_00781d3b(block); }
void release_buffer_4dc280(void *block) { Free_Memory_00781d3b(block); }
void release_buffer_4dc2d0(void *block) { Free_Memory_00781d3b(block); }
void release_buffer_4dc320(void *block) { Free_Memory_00781d3b(block); }
void release_buffer_4dc370(void *block) { Free_Memory_00781d3b(block); }
void release_buffer_4dc3c0(void *block) { Free_Memory_00781d3b(block); }
void release_buffer_4dc410(void *block) { Free_Memory_00781d3b(block); }
void release_buffer_4dc460(void *block) { Free_Memory_00781d3b(block); }
void release_buffer_4dc4b0(void *block) { Free_Memory_00781d3b(block); }
void release_buffer_4dc500(void *block) { Free_Memory_00781d3b(block); }
void release_buffer_4dc550(void *block) { Free_Memory_00781d3b(block); }
void release_buffer_4dc5a0(void *block) { Free_Memory_00781d3b(block); }
void release_buffer_4dc5f0(void *block) { Free_Memory_00781d3b(block); }
void release_buffer_4dc640(void *block) { Free_Memory_00781d3b(block); }
void release_buffer_4dc690(void *block) { Free_Memory_00781d3b(block); }
void release_buffer_4dc6e0(void *block) { Free_Memory_00781d3b(block); }
void release_buffer_4dc730(void *block) { Free_Memory_00781d3b(block); }
void release_buffer_4dc780(void *block) { Free_Memory_00781d3b(block); }
void release_buffer_4dc7d0(void *block) { Free_Memory_00781d3b(block); }
void release_buffer_4dc820(void *block) { Free_Memory_00781d3b(block); }
void release_buffer_4dc870(void *block) { Free_Memory_00781d3b(block); }
void release_buffer_4dc8c0(void *block) { Free_Memory_00781d3b(block); }
void release_buffer_4dc910(void *block) { Free_Memory_00781d3b(block); }
void release_buffer_4dc960(void *block) { Free_Memory_00781d3b(block); }
void release_buffer_4dc9b0(void *block) { Free_Memory_00781d3b(block); }
void release_buffer_4dca00(void *block) { Free_Memory_00781d3b(block); }
void release_buffer_4dca50(void *block) { Free_Memory_00781d3b(block); }
void release_buffer_4dcaa0(void *block) { Free_Memory_00781d3b(block); }
void release_buffer_4dcaf0(void *block) { Free_Memory_00781d3b(block); }
void release_buffer_4dcb40(void *block) { Free_Memory_00781d3b(block); }
void release_buffer_4dcb90(void *block) { Free_Memory_00781d3b(block); }
void release_buffer_4dcbe0(void *block) { Free_Memory_00781d3b(block); }
void release_buffer_4dcc30(void *block) { Free_Memory_00781d3b(block); }
void release_buffer_4dcc80(void *block) { Free_Memory_00781d3b(block); }
void release_buffer_4dccd0(void *block) { Free_Memory_00781d3b(block); }
void release_buffer_4dcd20(void *block) { Free_Memory_00781d3b(block); }
void release_buffer_4dcd70(void *block) { Free_Memory_00781d3b(block); }
void release_buffer_4dcdc0(void *block) { Free_Memory_00781d3b(block); }
void release_buffer_4dce10(void *block) { Free_Memory_00781d3b(block); }
void release_buffer_4dce60(void *block) { Free_Memory_00781d3b(block); }
void release_buffer_4dceb0(void *block) { Free_Memory_00781d3b(block); }
void release_buffer_4dcf00(void *block) { Free_Memory_00781d3b(block); }
void release_buffer_4dcf50(void *block) { Free_Memory_00781d3b(block); }
void release_buffer_4dcfa0(void *block) { Free_Memory_00781d3b(block); }
void release_buffer_4dcff0(void *block) { Free_Memory_00781d3b(block); }
void release_buffer_4dd040(void *block) { Free_Memory_00781d3b(block); }
void release_buffer_4dd090(void *block) { Free_Memory_00781d3b(block); }
void release_buffer_4dd0e0(void *block) { Free_Memory_00781d3b(block); }
void release_buffer_4dd130(void *block) { Free_Memory_00781d3b(block); }
void release_buffer_4dd180(void *block) { Free_Memory_00781d3b(block); }
void release_buffer_4dd1d0(void *block) { Free_Memory_00781d3b(block); }
void release_buffer_4dd220(void *block) { Free_Memory_00781d3b(block); }
void release_buffer_4dd270(void *block) { Free_Memory_00781d3b(block); }
void release_buffer_4dd2c0(void *block) { Free_Memory_00781d3b(block); }
void release_buffer_4dd310(void *block) { Free_Memory_00781d3b(block); }
void release_buffer_4dd360(void *block) { Free_Memory_00781d3b(block); }
void release_buffer_4dd3b0(void *block) { Free_Memory_00781d3b(block); }
void release_buffer_4dd400(void *block) { Free_Memory_00781d3b(block); }
void release_buffer_4dd450(void *block) { Free_Memory_00781d3b(block); }
void release_buffer_4dd4a0(void *block) { Free_Memory_00781d3b(block); }
void release_buffer_4dd4f0(void *block) { Free_Memory_00781d3b(block); }
void release_buffer_4dd540(void *block) { Free_Memory_00781d3b(block); }
void release_buffer_4dd590(void *block) { Free_Memory_00781d3b(block); }
void release_buffer_531e40(void *block) { Free_Memory_00781d3b(block); }
void release_buffer_531e90(void *block) { Free_Memory_00781d3b(block); }
void release_buffer_539b30(void *block) { Free_Memory_00781d3b(block); }
void release_buffer_539b80(void *block) { Free_Memory_00781d3b(block); }
void release_buffer_56c870(void *block) { Free_Memory_00781d3b(block); }
void release_buffer_56c8c0(void *block) { Free_Memory_00781d3b(block); }
void release_buffer_58eca0(void *block) { Free_Memory_00781d3b(block); }
void release_buffer_58ecf0(void *block) { Free_Memory_00781d3b(block); }
void release_buffer_58ed40(void *block) { Free_Memory_00781d3b(block); }
void release_buffer_58ed90(void *block) { Free_Memory_00781d3b(block); }
void release_buffer_5b6870(void *block) { Free_Memory_00781d3b(block); }
void release_buffer_5b68c0(void *block) { Free_Memory_00781d3b(block); }
void release_buffer_5b6910(void *block) { Free_Memory_00781d3b(block); }
void release_buffer_5b6960(void *block) { Free_Memory_00781d3b(block); }
void release_buffer_5cd900(void *block) { Free_Memory_00781d3b(block); }
void release_buffer_5cd950(void *block) { Free_Memory_00781d3b(block); }
void release_buffer_6365a0(void *block) { Free_Memory_00781d3b(block); }
void release_buffer_6365f0(void *block) { Free_Memory_00781d3b(block); }
void release_buffer_63cac0(void *block) { Free_Memory_00781d3b(block); }
void release_buffer_63cb10(void *block) { Free_Memory_00781d3b(block); }
void release_buffer_6401d0(void *block) { Free_Memory_00781d3b(block); }
void release_buffer_640220(void *block) { Free_Memory_00781d3b(block); }
void release_buffer_674960(void *block) { Free_Memory_00781d3b(block); }
void release_buffer_6749b0(void *block) { Free_Memory_00781d3b(block); }
void release_buffer_68afa0(void *block) { Free_Memory_00781d3b(block); }
void release_buffer_68aff0(void *block) { Free_Memory_00781d3b(block); }
void release_buffer_68b040(void *block) { Free_Memory_00781d3b(block); }
void release_buffer_68b090(void *block) { Free_Memory_00781d3b(block); }
void release_buffer_69c180(void *block) { Free_Memory_00781d3b(block); }
void release_buffer_69c310(void *block) { Free_Memory_00781d3b(block); }
void release_buffer_6e2d70(void *block) { Free_Memory_00781d3b(block); }
void release_buffer_6e2dc0(void *block) { Free_Memory_00781d3b(block); }
void release_buffer_766800(void *block) { Free_Memory_00781d3b(block); }
void release_buffer_766850(void *block) { Free_Memory_00781d3b(block); }
void release_object_40b230(void *block) { Free_Memory_00781d3b(block); }
void release_object_40bbc0(void *block) { Free_Memory_00781d3b(block); }
void release_object_40ce20(void *block) { Free_Memory_00781d3b(block); }
void release_object_40e8d0(void *block) { Free_Memory_00781d3b(block); }
void release_object_40e920(void *block) { Free_Memory_00781d3b(block); }
void release_object_40ef80(void *block) { Free_Memory_00781d3b(block); }
void release_object_40efd0(void *block) { Free_Memory_00781d3b(block); }
void release_object_40fc30(void *block) { Free_Memory_00781d3b(block); }
void release_object_40fc80(void *block) { Free_Memory_00781d3b(block); }
void release_object_4686d0(void *block) { Free_Memory_00781d3b(block); }
void release_object_468720(void *block) { Free_Memory_00781d3b(block); }
void release_object_491340(void *block) { Free_Memory_00781d3b(block); }
void release_object_491390(void *block) { Free_Memory_00781d3b(block); }
void release_object_4b28b0(void *block) { Free_Memory_00781d3b(block); }
void release_object_4b2900(void *block) { Free_Memory_00781d3b(block); }
void release_object_4b45f0(void *block) { Free_Memory_00781d3b(block); }
void release_object_4b4640(void *block) { Free_Memory_00781d3b(block); }
void release_object_4b5770(void *block) { Free_Memory_00781d3b(block); }
void release_object_4ba320(void *block) { Free_Memory_00781d3b(block); }
void release_object_4ba370(void *block) { Free_Memory_00781d3b(block); }
void release_object_4dbbf0(void *block) { Free_Memory_00781d3b(block); }
void release_object_4dbc40(void *block) { Free_Memory_00781d3b(block); }
void release_object_4dbc90(void *block) { Free_Memory_00781d3b(block); }
void release_object_4dbce0(void *block) { Free_Memory_00781d3b(block); }
void release_object_4dbd30(void *block) { Free_Memory_00781d3b(block); }
void release_object_4dbd80(void *block) { Free_Memory_00781d3b(block); }
void release_object_4dbdd0(void *block) { Free_Memory_00781d3b(block); }
void release_object_4dbe20(void *block) { Free_Memory_00781d3b(block); }
void release_object_4dbe70(void *block) { Free_Memory_00781d3b(block); }
void release_object_4dbec0(void *block) { Free_Memory_00781d3b(block); }
void release_object_4dbf10(void *block) { Free_Memory_00781d3b(block); }
void release_object_4dbf60(void *block) { Free_Memory_00781d3b(block); }
void release_object_4dbfb0(void *block) { Free_Memory_00781d3b(block); }
void release_object_4dc000(void *block) { Free_Memory_00781d3b(block); }
void release_object_4dc050(void *block) { Free_Memory_00781d3b(block); }
void release_object_4dc0a0(void *block) { Free_Memory_00781d3b(block); }
void release_object_4dc0f0(void *block) { Free_Memory_00781d3b(block); }
void release_object_4dc140(void *block) { Free_Memory_00781d3b(block); }
void release_object_4dc190(void *block) { Free_Memory_00781d3b(block); }
void release_object_4dc1e0(void *block) { Free_Memory_00781d3b(block); }
void release_object_4dc230(void *block) { Free_Memory_00781d3b(block); }
void release_object_4dc280(void *block) { Free_Memory_00781d3b(block); }
void release_object_4dc2d0(void *block) { Free_Memory_00781d3b(block); }
void release_object_4dc320(void *block) { Free_Memory_00781d3b(block); }
void release_object_4dc370(void *block) { Free_Memory_00781d3b(block); }
void release_object_4dc3c0(void *block) { Free_Memory_00781d3b(block); }
void release_object_4dc410(void *block) { Free_Memory_00781d3b(block); }
void release_object_4dc460(void *block) { Free_Memory_00781d3b(block); }
void release_object_4dc4b0(void *block) { Free_Memory_00781d3b(block); }
void release_object_4dc500(void *block) { Free_Memory_00781d3b(block); }
void release_object_4dc550(void *block) { Free_Memory_00781d3b(block); }
void release_object_4dc5a0(void *block) { Free_Memory_00781d3b(block); }
void release_object_4dc5f0(void *block) { Free_Memory_00781d3b(block); }
void release_object_4dc640(void *block) { Free_Memory_00781d3b(block); }
void release_object_4dc690(void *block) { Free_Memory_00781d3b(block); }
void release_object_4dc6e0(void *block) { Free_Memory_00781d3b(block); }
void release_object_4dc730(void *block) { Free_Memory_00781d3b(block); }
void release_object_4dc780(void *block) { Free_Memory_00781d3b(block); }
void release_object_4dc7d0(void *block) { Free_Memory_00781d3b(block); }
void release_object_4dc820(void *block) { Free_Memory_00781d3b(block); }
void release_object_4dc870(void *block) { Free_Memory_00781d3b(block); }
void release_object_4dc8c0(void *block) { Free_Memory_00781d3b(block); }
void release_object_4dc910(void *block) { Free_Memory_00781d3b(block); }
void release_object_4dc960(void *block) { Free_Memory_00781d3b(block); }
void release_object_4dc9b0(void *block) { Free_Memory_00781d3b(block); }
void release_object_4dca00(void *block) { Free_Memory_00781d3b(block); }
void release_object_4dca50(void *block) { Free_Memory_00781d3b(block); }
void release_object_4dcaa0(void *block) { Free_Memory_00781d3b(block); }
void release_object_4dcaf0(void *block) { Free_Memory_00781d3b(block); }
void release_object_4dcb40(void *block) { Free_Memory_00781d3b(block); }
void release_object_4dcb90(void *block) { Free_Memory_00781d3b(block); }
void release_object_4dcbe0(void *block) { Free_Memory_00781d3b(block); }
void release_object_4dcc30(void *block) { Free_Memory_00781d3b(block); }
void release_object_4dcc80(void *block) { Free_Memory_00781d3b(block); }
void release_object_4dccd0(void *block) { Free_Memory_00781d3b(block); }
void release_object_4dcd20(void *block) { Free_Memory_00781d3b(block); }
void release_object_4dcd70(void *block) { Free_Memory_00781d3b(block); }
void release_object_4dcdc0(void *block) { Free_Memory_00781d3b(block); }
void release_object_4dce10(void *block) { Free_Memory_00781d3b(block); }
void release_object_4dce60(void *block) { Free_Memory_00781d3b(block); }
void release_object_4dceb0(void *block) { Free_Memory_00781d3b(block); }
void release_object_4dcf00(void *block) { Free_Memory_00781d3b(block); }
void release_object_4dcf50(void *block) { Free_Memory_00781d3b(block); }
void release_object_4dcfa0(void *block) { Free_Memory_00781d3b(block); }
void release_object_4dcff0(void *block) { Free_Memory_00781d3b(block); }
void release_object_4dd040(void *block) { Free_Memory_00781d3b(block); }
void release_object_4dd090(void *block) { Free_Memory_00781d3b(block); }
void release_object_4dd0e0(void *block) { Free_Memory_00781d3b(block); }
void release_object_4dd130(void *block) { Free_Memory_00781d3b(block); }
void release_object_4dd180(void *block) { Free_Memory_00781d3b(block); }
void release_object_4dd1d0(void *block) { Free_Memory_00781d3b(block); }
void release_object_4dd220(void *block) { Free_Memory_00781d3b(block); }
void release_object_4dd270(void *block) { Free_Memory_00781d3b(block); }
void release_object_4dd2c0(void *block) { Free_Memory_00781d3b(block); }
void release_object_4dd310(void *block) { Free_Memory_00781d3b(block); }
void release_object_4dd360(void *block) { Free_Memory_00781d3b(block); }
void release_object_4dd3b0(void *block) { Free_Memory_00781d3b(block); }
void release_object_4dd400(void *block) { Free_Memory_00781d3b(block); }
void release_object_4dd450(void *block) { Free_Memory_00781d3b(block); }
void release_object_4dd4a0(void *block) { Free_Memory_00781d3b(block); }
void release_object_4dd4f0(void *block) { Free_Memory_00781d3b(block); }
void release_object_4dd540(void *block) { Free_Memory_00781d3b(block); }
void release_object_4dd590(void *block) { Free_Memory_00781d3b(block); }
void release_object_531e40(void *block) { Free_Memory_00781d3b(block); }
void release_object_531e90(void *block) { Free_Memory_00781d3b(block); }
void release_object_539b30(void *block) { Free_Memory_00781d3b(block); }
void release_object_539b80(void *block) { Free_Memory_00781d3b(block); }
void release_object_56c870(void *block) { Free_Memory_00781d3b(block); }
void release_object_56c8c0(void *block) { Free_Memory_00781d3b(block); }
void release_object_58eca0(void *block) { Free_Memory_00781d3b(block); }
void release_object_58ecf0(void *block) { Free_Memory_00781d3b(block); }
void release_object_58ed40(void *block) { Free_Memory_00781d3b(block); }
void release_object_58ed90(void *block) { Free_Memory_00781d3b(block); }
void release_object_5b6870(void *block) { Free_Memory_00781d3b(block); }
void release_object_5b68c0(void *block) { Free_Memory_00781d3b(block); }
void release_object_5b6910(void *block) { Free_Memory_00781d3b(block); }
void release_object_5b6960(void *block) { Free_Memory_00781d3b(block); }
void release_object_5cd900(void *block) { Free_Memory_00781d3b(block); }
void release_object_5cd950(void *block) { Free_Memory_00781d3b(block); }
void release_object_6365a0(void *block) { Free_Memory_00781d3b(block); }
void release_object_6365f0(void *block) { Free_Memory_00781d3b(block); }
void release_object_63cac0(void *block) { Free_Memory_00781d3b(block); }
void release_object_63cb10(void *block) { Free_Memory_00781d3b(block); }
void release_object_6401d0(void *block) { Free_Memory_00781d3b(block); }
void release_object_640220(void *block) { Free_Memory_00781d3b(block); }
void release_object_674960(void *block) { Free_Memory_00781d3b(block); }
void release_object_6749b0(void *block) { Free_Memory_00781d3b(block); }
void release_object_68afa0(void *block) { Free_Memory_00781d3b(block); }
void release_object_68aff0(void *block) { Free_Memory_00781d3b(block); }
void release_object_68b040(void *block) { Free_Memory_00781d3b(block); }
void release_object_68b090(void *block) { Free_Memory_00781d3b(block); }
void release_object_69c180(void *block) { Free_Memory_00781d3b(block); }
void release_object_69c310(void *block) { Free_Memory_00781d3b(block); }
void release_object_6e2d70(void *block) { Free_Memory_00781d3b(block); }
void release_object_6e2dc0(void *block) { Free_Memory_00781d3b(block); }
void release_object_766800(void *block) { Free_Memory_00781d3b(block); }
void release_object_766850(void *block) { Free_Memory_00781d3b(block); }
// release_<addr>: the deleting destructors' free of the object (0x00781D3B).
void release_40e2e0(void *block) { Free_Memory_00781d3b(block); }
void release_42ae60(void *block) { Free_Memory_00781d3b(block); }
void release_453de0(void *block) { Free_Memory_00781d3b(block); }
void release_47fcf0(void *block) { Free_Memory_00781d3b(block); }
void release_496470(void *block) { Free_Memory_00781d3b(block); }
void release_4a9b00(void *block) { Free_Memory_00781d3b(block); }
void release_4a9b60(void *block) { Free_Memory_00781d3b(block); }
void release_4b5600(void *block) { Free_Memory_00781d3b(block); }
void release_4f6e00(void *block) { Free_Memory_00781d3b(block); }
void release_56ace0(void *block) { Free_Memory_00781d3b(block); }
void release_58da50(void *block) { Free_Memory_00781d3b(block); }
void release_58dd30(void *block) { Free_Memory_00781d3b(block); }
void release_58dd90(void *block) { Free_Memory_00781d3b(block); }
void release_58de60(void *block) { Free_Memory_00781d3b(block); }
void release_595c40(void *block) { Free_Memory_00781d3b(block); }
void release_59cb80(void *block) { Free_Memory_00781d3b(block); }
void release_5a62b0(void *block) { Free_Memory_00781d3b(block); }
void release_6738d0(void *block) { Free_Memory_00781d3b(block); }
void release_673930(void *block) { Free_Memory_00781d3b(block); }
void release_673990(void *block) { Free_Memory_00781d3b(block); }
void release_6d9750(void *block) { Free_Memory_00781d3b(block); }
void release_6ea9f0(void *block) { Free_Memory_00781d3b(block); }
void release_72ca00(void *block) { Free_Memory_00781d3b(block); }
void release_730290(void *block) { Free_Memory_00781d3b(block); }
void release_765e00(void *block) { Free_Memory_00781d3b(block); }
void release_7667a0(void *block) { Free_Memory_00781d3b(block); }
void release_7667c0(void *block) { Free_Memory_00781d3b(block); }
void release_76bb00(void *block) { Free_Memory_00781d3b(block); }
void release_76d5b0(void *block) { Free_Memory_00781d3b(block); }
// storage_<addr>, allocate_storage_<addr>, allocate_words_<addr>: the seed vectors' buffer
// allocation through the CRT operator new (byte count).
void *allocate_storage_0050b480(unsigned int byte_count) { return ::operator new(byte_count); }
void *allocate_storage_0058e170(unsigned int byte_count) { return ::operator new(byte_count); }
void *allocate_storage_005b95c0(unsigned int byte_count) { return ::operator new(byte_count); }
void *allocate_storage_005cd770(unsigned int byte_count) { return ::operator new(byte_count); }
void *allocate_storage_0061ac80(unsigned int byte_count) { return ::operator new(byte_count); }
void *allocate_storage_0061af90(unsigned int byte_count) { return ::operator new(byte_count); }
void *allocate_storage_00653730(unsigned int byte_count) { return ::operator new(byte_count); }
void *allocate_storage_00653a40(unsigned int byte_count) { return ::operator new(byte_count); }
void *allocate_storage_006634e0(unsigned int byte_count) { return ::operator new(byte_count); }
void *allocate_storage_007246d0(unsigned int byte_count) { return ::operator new(byte_count); }
void *storage_42b510(unsigned int byte_count) { return ::operator new(byte_count); }
void *storage_454ad0(unsigned int byte_count) { return ::operator new(byte_count); }
void *storage_454ba0(unsigned int byte_count) { return ::operator new(byte_count); }
void *storage_46dbd0(unsigned int byte_count) { return ::operator new(byte_count); }
void *storage_47d0e0(unsigned int byte_count) { return ::operator new(byte_count); }
void *storage_480050(unsigned int byte_count) { return ::operator new(byte_count); }
void *storage_4913e0(unsigned int byte_count) { return ::operator new(byte_count); }
void *storage_4967d0(unsigned int byte_count) { return ::operator new(byte_count); }
void *storage_4aa210(unsigned int byte_count) { return ::operator new(byte_count); }
void *storage_4aa260(unsigned int byte_count) { return ::operator new(byte_count); }
void *storage_4ba3c0(unsigned int byte_count) { return ::operator new(byte_count); }
void *storage_4bfbd0(unsigned int byte_count) { return ::operator new(byte_count); }
void *storage_4ceb90(unsigned int byte_count) { return ::operator new(byte_count); }
void *storage_4df2e0(unsigned int byte_count) { return ::operator new(byte_count); }
void *storage_4f8900(unsigned int byte_count) { return ::operator new(byte_count); }
void *storage_4f8950(unsigned int byte_count) { return ::operator new(byte_count); }
void *storage_4f89f0(unsigned int byte_count) { return ::operator new(byte_count); }
void *storage_4f8a40(unsigned int byte_count) { return ::operator new(byte_count); }
void *storage_4f8a90(unsigned int byte_count) { return ::operator new(byte_count); }
void *storage_4fb5c0(unsigned int byte_count) { return ::operator new(byte_count); }
void *storage_4fb660(unsigned int byte_count) { return ::operator new(byte_count); }
void *storage_50b860(unsigned int byte_count) { return ::operator new(byte_count); }
void *storage_51e710(unsigned int byte_count) { return ::operator new(byte_count); }
void *storage_531ee0(unsigned int byte_count) { return ::operator new(byte_count); }
void *storage_58ef60(unsigned int byte_count) { return ::operator new(byte_count); }
void *storage_58f040(unsigned int byte_count) { return ::operator new(byte_count); }
void *storage_58f210(unsigned int byte_count) { return ::operator new(byte_count); }
void *storage_58f2d0(unsigned int byte_count) { return ::operator new(byte_count); }
void *storage_5a6d80(unsigned int byte_count) { return ::operator new(byte_count); }
void *storage_5b9840(unsigned int byte_count) { return ::operator new(byte_count); }
void *storage_5b9890(unsigned int byte_count) { return ::operator new(byte_count); }
void *storage_5b98e0(unsigned int byte_count) { return ::operator new(byte_count); }
void *storage_5cdaf0(unsigned int byte_count) { return ::operator new(byte_count); }
void *storage_60d0a0(unsigned int byte_count) { return ::operator new(byte_count); }
void *storage_654960(unsigned int byte_count) { return ::operator new(byte_count); }
void *storage_6549b0(unsigned int byte_count) { return ::operator new(byte_count); }
void *storage_654a00(unsigned int byte_count) { return ::operator new(byte_count); }
void *storage_674ba0(unsigned int byte_count) { return ::operator new(byte_count); }
void *storage_68b0e0(unsigned int byte_count) { return ::operator new(byte_count); }
void *storage_6f8b90(unsigned int byte_count) { return ::operator new(byte_count); }
void *storage_730470(unsigned int byte_count) { return ::operator new(byte_count); }
void *storage_7304c0(unsigned int byte_count) { return ::operator new(byte_count); }
void *storage_7680c0(unsigned int byte_count) { return ::operator new(byte_count); }
}
