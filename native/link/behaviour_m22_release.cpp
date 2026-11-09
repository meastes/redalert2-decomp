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
void release_buffer_40bad0(void *block) { Free_Memory_00781d3b(block); }
void release_buffer_40bb20(void *block) { Free_Memory_00781d3b(block); }
void release_buffer_40bb70(void *block) { Free_Memory_00781d3b(block); }
void release_buffer_40bbc0(void *block) { Free_Memory_00781d3b(block); }
void release_buffer_40c9d0(void *block) { Free_Memory_00781d3b(block); }
void release_buffer_40ca20(void *block) { Free_Memory_00781d3b(block); }
void release_buffer_40ce20(void *block) { Free_Memory_00781d3b(block); }
void release_buffer_40d6d0(void *block) { Free_Memory_00781d3b(block); }
void release_buffer_40d720(void *block) { Free_Memory_00781d3b(block); }
void release_buffer_40e8d0(void *block) { Free_Memory_00781d3b(block); }
void release_buffer_40e920(void *block) { Free_Memory_00781d3b(block); }
void release_buffer_40ef80(void *block) { Free_Memory_00781d3b(block); }
void release_buffer_40efd0(void *block) { Free_Memory_00781d3b(block); }
void release_buffer_40f680(void *block) { Free_Memory_00781d3b(block); }
void release_buffer_40f6d0(void *block) { Free_Memory_00781d3b(block); }
void release_buffer_40fc30(void *block) { Free_Memory_00781d3b(block); }
void release_buffer_40fc80(void *block) { Free_Memory_00781d3b(block); }
void release_buffer_41e8d0(void *block) { Free_Memory_00781d3b(block); }
void release_buffer_41e920(void *block) { Free_Memory_00781d3b(block); }
void release_buffer_41fd50(void *block) { Free_Memory_00781d3b(block); }
void release_buffer_41fda0(void *block) { Free_Memory_00781d3b(block); }
void release_buffer_42b3d0(void *block) { Free_Memory_00781d3b(block); }
void release_buffer_42b420(void *block) { Free_Memory_00781d3b(block); }
void release_buffer_42b470(void *block) { Free_Memory_00781d3b(block); }
void release_buffer_42b4c0(void *block) { Free_Memory_00781d3b(block); }
void release_buffer_42d160(void *block) { Free_Memory_00781d3b(block); }
void release_buffer_42d340(void *block) { Free_Memory_00781d3b(block); }
void release_buffer_42d390(void *block) { Free_Memory_00781d3b(block); }
void release_buffer_42d3e0(void *block) { Free_Memory_00781d3b(block); }
void release_buffer_438270(void *block) { Free_Memory_00781d3b(block); }
void release_buffer_454710(void *block) { Free_Memory_00781d3b(block); }
void release_buffer_454760(void *block) { Free_Memory_00781d3b(block); }
void release_buffer_4547b0(void *block) { Free_Memory_00781d3b(block); }
void release_buffer_454800(void *block) { Free_Memory_00781d3b(block); }
void release_buffer_454850(void *block) { Free_Memory_00781d3b(block); }
void release_buffer_4549e0(void *block) { Free_Memory_00781d3b(block); }
void release_buffer_454a30(void *block) { Free_Memory_00781d3b(block); }
void release_buffer_454a80(void *block) { Free_Memory_00781d3b(block); }
void release_buffer_464200(void *block) { Free_Memory_00781d3b(block); }
void release_buffer_464250(void *block) { Free_Memory_00781d3b(block); }
void release_buffer_4686d0(void *block) { Free_Memory_00781d3b(block); }
void release_buffer_468720(void *block) { Free_Memory_00781d3b(block); }
void release_buffer_46ceb0(void *block) { Free_Memory_00781d3b(block); }
void release_buffer_46cf00(void *block) { Free_Memory_00781d3b(block); }
void release_buffer_46cf50(void *block) { Free_Memory_00781d3b(block); }
void release_buffer_46da70(void *block) { Free_Memory_00781d3b(block); }
void release_buffer_46dac0(void *block) { Free_Memory_00781d3b(block); }
void release_buffer_46db10(void *block) { Free_Memory_00781d3b(block); }
void release_buffer_47d090(void *block) { Free_Memory_00781d3b(block); }
void release_buffer_47ffb0(void *block) { Free_Memory_00781d3b(block); }
void release_buffer_480000(void *block) { Free_Memory_00781d3b(block); }
void release_buffer_48dbd0(void *block) { Free_Memory_00781d3b(block); }
void release_buffer_48dc20(void *block) { Free_Memory_00781d3b(block); }
void release_buffer_491340(void *block) { Free_Memory_00781d3b(block); }
void release_buffer_491390(void *block) { Free_Memory_00781d3b(block); }
void release_buffer_496730(void *block) { Free_Memory_00781d3b(block); }
void release_buffer_496780(void *block) { Free_Memory_00781d3b(block); }
void release_buffer_4aa080(void *block) { Free_Memory_00781d3b(block); }
void release_buffer_4aa0d0(void *block) { Free_Memory_00781d3b(block); }
void release_buffer_4aa120(void *block) { Free_Memory_00781d3b(block); }
void release_buffer_4aa170(void *block) { Free_Memory_00781d3b(block); }
void release_buffer_4aa1c0(void *block) { Free_Memory_00781d3b(block); }
void release_buffer_4b28b0(void *block) { Free_Memory_00781d3b(block); }
void release_buffer_4b2900(void *block) { Free_Memory_00781d3b(block); }
void release_buffer_4b45f0(void *block) { Free_Memory_00781d3b(block); }
void release_buffer_4b4640(void *block) { Free_Memory_00781d3b(block); }
void release_buffer_4b5770(void *block) { Free_Memory_00781d3b(block); }
void release_buffer_4ba320(void *block) { Free_Memory_00781d3b(block); }
void release_buffer_4ba370(void *block) { Free_Memory_00781d3b(block); }
void release_buffer_4c2480(void *block) { Free_Memory_00781d3b(block); }
void release_buffer_4c24d0(void *block) { Free_Memory_00781d3b(block); }
void release_buffer_4cdf30(void *block) { Free_Memory_00781d3b(block); }
void release_buffer_4cdf90(void *block) { Free_Memory_00781d3b(block); }
void release_buffer_4ce9f0(void *block) { Free_Memory_00781d3b(block); }
void release_buffer_4cea40(void *block) { Free_Memory_00781d3b(block); }
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
void release_buffer_4df240(void *block) { Free_Memory_00781d3b(block); }
void release_buffer_4df290(void *block) { Free_Memory_00781d3b(block); }
void release_buffer_4f6ee0(void *block) { Free_Memory_00781d3b(block); }
void release_buffer_4f8490(void *block) { Free_Memory_00781d3b(block); }
void release_buffer_4f84e0(void *block) { Free_Memory_00781d3b(block); }
void release_buffer_4f8530(void *block) { Free_Memory_00781d3b(block); }
void release_buffer_4f8580(void *block) { Free_Memory_00781d3b(block); }
void release_buffer_4f85d0(void *block) { Free_Memory_00781d3b(block); }
void release_buffer_4f8620(void *block) { Free_Memory_00781d3b(block); }
void release_buffer_4f8670(void *block) { Free_Memory_00781d3b(block); }
void release_buffer_4f86c0(void *block) { Free_Memory_00781d3b(block); }
void release_buffer_4f8710(void *block) { Free_Memory_00781d3b(block); }
void release_buffer_4f8760(void *block) { Free_Memory_00781d3b(block); }
void release_buffer_4f87b0(void *block) { Free_Memory_00781d3b(block); }
void release_buffer_50b260(void *block) { Free_Memory_00781d3b(block); }
void release_buffer_50b2b0(void *block) { Free_Memory_00781d3b(block); }
void release_buffer_51e670(void *block) { Free_Memory_00781d3b(block); }
void release_buffer_51e6c0(void *block) { Free_Memory_00781d3b(block); }
void release_buffer_520f00(void *block) { Free_Memory_00781d3b(block); }
void release_buffer_520f50(void *block) { Free_Memory_00781d3b(block); }
void release_buffer_522460(void *block) { Free_Memory_00781d3b(block); }
void release_buffer_5224b0(void *block) { Free_Memory_00781d3b(block); }
void release_buffer_52ec50(void *block) { Free_Memory_00781d3b(block); }
void release_buffer_52eca0(void *block) { Free_Memory_00781d3b(block); }
void release_buffer_52ecf0(void *block) { Free_Memory_00781d3b(block); }
void release_buffer_52ed40(void *block) { Free_Memory_00781d3b(block); }
void release_buffer_531e40(void *block) { Free_Memory_00781d3b(block); }
void release_buffer_531e90(void *block) { Free_Memory_00781d3b(block); }
void release_buffer_534610(void *block) { Free_Memory_00781d3b(block); }
void release_buffer_534660(void *block) { Free_Memory_00781d3b(block); }
void release_buffer_538260(void *block) { Free_Memory_00781d3b(block); }
void release_buffer_5382b0(void *block) { Free_Memory_00781d3b(block); }
void release_buffer_538300(void *block) { Free_Memory_00781d3b(block); }
void release_buffer_538350(void *block) { Free_Memory_00781d3b(block); }
void release_buffer_539b30(void *block) { Free_Memory_00781d3b(block); }
void release_buffer_539b80(void *block) { Free_Memory_00781d3b(block); }
void release_buffer_53aaf0(void *block) { Free_Memory_00781d3b(block); }
void release_buffer_53cd30(void *block) { Free_Memory_00781d3b(block); }
void release_buffer_53cd80(void *block) { Free_Memory_00781d3b(block); }
void release_buffer_56ae40(void *block) { Free_Memory_00781d3b(block); }
void release_buffer_56ae90(void *block) { Free_Memory_00781d3b(block); }
void release_buffer_56c1b0(void *block) { Free_Memory_00781d3b(block); }
void release_buffer_56c200(void *block) { Free_Memory_00781d3b(block); }
void release_buffer_56c870(void *block) { Free_Memory_00781d3b(block); }
void release_buffer_56c8c0(void *block) { Free_Memory_00781d3b(block); }
void release_buffer_56c910(void *block) { Free_Memory_00781d3b(block); }
void release_buffer_56c960(void *block) { Free_Memory_00781d3b(block); }
void release_buffer_58ec00(void *block) { Free_Memory_00781d3b(block); }
void release_buffer_58ec50(void *block) { Free_Memory_00781d3b(block); }
void release_buffer_58eca0(void *block) { Free_Memory_00781d3b(block); }
void release_buffer_58ecf0(void *block) { Free_Memory_00781d3b(block); }
void release_buffer_58ed40(void *block) { Free_Memory_00781d3b(block); }
void release_buffer_58ed90(void *block) { Free_Memory_00781d3b(block); }
void release_buffer_58ede0(void *block) { Free_Memory_00781d3b(block); }
void release_buffer_58ee30(void *block) { Free_Memory_00781d3b(block); }
void release_buffer_58ee80(void *block) { Free_Memory_00781d3b(block); }
void release_buffer_5a2280(void *block) { Free_Memory_00781d3b(block); }
void release_buffer_5a6360(void *block) { Free_Memory_00781d3b(block); }
void release_buffer_5a6bf0(void *block) { Free_Memory_00781d3b(block); }
void release_buffer_5a6c40(void *block) { Free_Memory_00781d3b(block); }
void release_buffer_5a6c90(void *block) { Free_Memory_00781d3b(block); }
void release_buffer_5a6ce0(void *block) { Free_Memory_00781d3b(block); }
void release_buffer_5a6d30(void *block) { Free_Memory_00781d3b(block); }
void release_buffer_5b6870(void *block) { Free_Memory_00781d3b(block); }
void release_buffer_5b68c0(void *block) { Free_Memory_00781d3b(block); }
void release_buffer_5b6910(void *block) { Free_Memory_00781d3b(block); }
void release_buffer_5b6960(void *block) { Free_Memory_00781d3b(block); }
void release_buffer_5b9700(void *block) { Free_Memory_00781d3b(block); }
void release_buffer_5b9750(void *block) { Free_Memory_00781d3b(block); }
void release_buffer_5b97a0(void *block) { Free_Memory_00781d3b(block); }
void release_buffer_5b97f0(void *block) { Free_Memory_00781d3b(block); }
void release_buffer_5cd900(void *block) { Free_Memory_00781d3b(block); }
void release_buffer_5cd950(void *block) { Free_Memory_00781d3b(block); }
void release_buffer_5d82f0(void *block) { Free_Memory_00781d3b(block); }
void release_buffer_5d8340(void *block) { Free_Memory_00781d3b(block); }
void release_buffer_5de5c0(void *block) { Free_Memory_00781d3b(block); }
void release_buffer_5de610(void *block) { Free_Memory_00781d3b(block); }
void release_buffer_6055a0(void *block) { Free_Memory_00781d3b(block); }
void release_buffer_6055f0(void *block) { Free_Memory_00781d3b(block); }
void release_buffer_6081b0(void *block) { Free_Memory_00781d3b(block); }
void release_buffer_608200(void *block) { Free_Memory_00781d3b(block); }
void release_buffer_61ba10(void *block) { Free_Memory_00781d3b(block); }
void release_buffer_61ba60(void *block) { Free_Memory_00781d3b(block); }
void release_buffer_61bab0(void *block) { Free_Memory_00781d3b(block); }
void release_buffer_61bb00(void *block) { Free_Memory_00781d3b(block); }
void release_buffer_61bb50(void *block) { Free_Memory_00781d3b(block); }
void release_buffer_61bba0(void *block) { Free_Memory_00781d3b(block); }
void release_buffer_61bbf0(void *block) { Free_Memory_00781d3b(block); }
void release_buffer_61bc40(void *block) { Free_Memory_00781d3b(block); }
void release_buffer_61bc90(void *block) { Free_Memory_00781d3b(block); }
void release_buffer_61bce0(void *block) { Free_Memory_00781d3b(block); }
void release_buffer_622040(void *block) { Free_Memory_00781d3b(block); }
void release_buffer_635040(void *block) { Free_Memory_00781d3b(block); }
void release_buffer_6365a0(void *block) { Free_Memory_00781d3b(block); }
void release_buffer_6365f0(void *block) { Free_Memory_00781d3b(block); }
void release_buffer_638270(void *block) { Free_Memory_00781d3b(block); }
void release_buffer_6382c0(void *block) { Free_Memory_00781d3b(block); }
void release_buffer_63cac0(void *block) { Free_Memory_00781d3b(block); }
void release_buffer_63cb10(void *block) { Free_Memory_00781d3b(block); }
void release_buffer_6401d0(void *block) { Free_Memory_00781d3b(block); }
void release_buffer_640220(void *block) { Free_Memory_00781d3b(block); }
void release_buffer_654390(void *block) { Free_Memory_00781d3b(block); }
void release_buffer_6543e0(void *block) { Free_Memory_00781d3b(block); }
void release_buffer_654430(void *block) { Free_Memory_00781d3b(block); }
void release_buffer_654480(void *block) { Free_Memory_00781d3b(block); }
void release_buffer_6544d0(void *block) { Free_Memory_00781d3b(block); }
void release_buffer_654520(void *block) { Free_Memory_00781d3b(block); }
void release_buffer_6739f0(void *block) { Free_Memory_00781d3b(block); }
void release_buffer_674790(void *block) { Free_Memory_00781d3b(block); }
void release_buffer_6748c0(void *block) { Free_Memory_00781d3b(block); }
void release_buffer_674910(void *block) { Free_Memory_00781d3b(block); }
void release_buffer_674960(void *block) { Free_Memory_00781d3b(block); }
void release_buffer_6749b0(void *block) { Free_Memory_00781d3b(block); }
void release_buffer_674a00(void *block) { Free_Memory_00781d3b(block); }
void release_buffer_674a50(void *block) { Free_Memory_00781d3b(block); }
void release_buffer_687330(void *block) { Free_Memory_00781d3b(block); }
void release_buffer_687380(void *block) { Free_Memory_00781d3b(block); }
void release_buffer_68afa0(void *block) { Free_Memory_00781d3b(block); }
void release_buffer_68aff0(void *block) { Free_Memory_00781d3b(block); }
void release_buffer_68b040(void *block) { Free_Memory_00781d3b(block); }
void release_buffer_68b090(void *block) { Free_Memory_00781d3b(block); }
void release_buffer_6986a0(void *block) { Free_Memory_00781d3b(block); }
void release_buffer_6986f0(void *block) { Free_Memory_00781d3b(block); }
void release_buffer_69c180(void *block) { Free_Memory_00781d3b(block); }
void release_buffer_69c310(void *block) { Free_Memory_00781d3b(block); }
void release_buffer_6acfc0(void *block) { Free_Memory_00781d3b(block); }
void release_buffer_6ad010(void *block) { Free_Memory_00781d3b(block); }
void release_buffer_6ad060(void *block) { Free_Memory_00781d3b(block); }
void release_buffer_6ad0b0(void *block) { Free_Memory_00781d3b(block); }
void release_buffer_6b3560(void *block) { Free_Memory_00781d3b(block); }
void release_buffer_6b35b0(void *block) { Free_Memory_00781d3b(block); }
void release_buffer_6b51b0(void *block) { Free_Memory_00781d3b(block); }
void release_buffer_6b5200(void *block) { Free_Memory_00781d3b(block); }
void release_buffer_6e2d70(void *block) { Free_Memory_00781d3b(block); }
void release_buffer_6e2dc0(void *block) { Free_Memory_00781d3b(block); }
void release_buffer_6e7180(void *block) { Free_Memory_00781d3b(block); }
void release_buffer_6e71d0(void *block) { Free_Memory_00781d3b(block); }
void release_buffer_6e86e0(void *block) { Free_Memory_00781d3b(block); }
void release_buffer_6eacd0(void *block) { Free_Memory_00781d3b(block); }
void release_buffer_6ead20(void *block) { Free_Memory_00781d3b(block); }
void release_buffer_6ec1d0(void *block) { Free_Memory_00781d3b(block); }
void release_buffer_70f3c0(void *block) { Free_Memory_00781d3b(block); }
void release_buffer_712a40(void *block) { Free_Memory_00781d3b(block); }
void release_buffer_712a90(void *block) { Free_Memory_00781d3b(block); }
void release_buffer_714ff0(void *block) { Free_Memory_00781d3b(block); }
void release_buffer_715040(void *block) { Free_Memory_00781d3b(block); }
void release_buffer_716ce0(void *block) { Free_Memory_00781d3b(block); }
void release_buffer_716d30(void *block) { Free_Memory_00781d3b(block); }
void release_buffer_72ba00(void *block) { Free_Memory_00781d3b(block); }
void release_buffer_72ba50(void *block) { Free_Memory_00781d3b(block); }
void release_buffer_72baa0(void *block) { Free_Memory_00781d3b(block); }
void release_buffer_766800(void *block) { Free_Memory_00781d3b(block); }
void release_buffer_766850(void *block) { Free_Memory_00781d3b(block); }
void release_buffer_76bdc0(void *block) { Free_Memory_00781d3b(block); }
void release_buffer_76be10(void *block) { Free_Memory_00781d3b(block); }
void release_buffer_76d870(void *block) { Free_Memory_00781d3b(block); }
void release_buffer_76d8c0(void *block) { Free_Memory_00781d3b(block); }
void release_object_40b230(void *block) { Free_Memory_00781d3b(block); }
void release_object_40bad0(void *block) { Free_Memory_00781d3b(block); }
void release_object_40bb20(void *block) { Free_Memory_00781d3b(block); }
void release_object_40bb70(void *block) { Free_Memory_00781d3b(block); }
void release_object_40bbc0(void *block) { Free_Memory_00781d3b(block); }
void release_object_40c9d0(void *block) { Free_Memory_00781d3b(block); }
void release_object_40ca20(void *block) { Free_Memory_00781d3b(block); }
void release_object_40ce20(void *block) { Free_Memory_00781d3b(block); }
void release_object_40d6d0(void *block) { Free_Memory_00781d3b(block); }
void release_object_40d720(void *block) { Free_Memory_00781d3b(block); }
void release_object_40e8d0(void *block) { Free_Memory_00781d3b(block); }
void release_object_40e920(void *block) { Free_Memory_00781d3b(block); }
void release_object_40ef80(void *block) { Free_Memory_00781d3b(block); }
void release_object_40efd0(void *block) { Free_Memory_00781d3b(block); }
void release_object_40f680(void *block) { Free_Memory_00781d3b(block); }
void release_object_40f6d0(void *block) { Free_Memory_00781d3b(block); }
void release_object_40fc30(void *block) { Free_Memory_00781d3b(block); }
void release_object_40fc80(void *block) { Free_Memory_00781d3b(block); }
void release_object_41e8d0(void *block) { Free_Memory_00781d3b(block); }
void release_object_41e920(void *block) { Free_Memory_00781d3b(block); }
void release_object_41fd50(void *block) { Free_Memory_00781d3b(block); }
void release_object_41fda0(void *block) { Free_Memory_00781d3b(block); }
void release_object_42b3d0(void *block) { Free_Memory_00781d3b(block); }
void release_object_42b420(void *block) { Free_Memory_00781d3b(block); }
void release_object_42b470(void *block) { Free_Memory_00781d3b(block); }
void release_object_42b4c0(void *block) { Free_Memory_00781d3b(block); }
void release_object_42d160(void *block) { Free_Memory_00781d3b(block); }
void release_object_42d340(void *block) { Free_Memory_00781d3b(block); }
void release_object_42d390(void *block) { Free_Memory_00781d3b(block); }
void release_object_42d3e0(void *block) { Free_Memory_00781d3b(block); }
void release_object_438270(void *block) { Free_Memory_00781d3b(block); }
void release_object_454710(void *block) { Free_Memory_00781d3b(block); }
void release_object_454760(void *block) { Free_Memory_00781d3b(block); }
void release_object_4547b0(void *block) { Free_Memory_00781d3b(block); }
void release_object_454800(void *block) { Free_Memory_00781d3b(block); }
void release_object_454850(void *block) { Free_Memory_00781d3b(block); }
void release_object_4549e0(void *block) { Free_Memory_00781d3b(block); }
void release_object_454a30(void *block) { Free_Memory_00781d3b(block); }
void release_object_454a80(void *block) { Free_Memory_00781d3b(block); }
void release_object_464200(void *block) { Free_Memory_00781d3b(block); }
void release_object_464250(void *block) { Free_Memory_00781d3b(block); }
void release_object_4686d0(void *block) { Free_Memory_00781d3b(block); }
void release_object_468720(void *block) { Free_Memory_00781d3b(block); }
void release_object_46ceb0(void *block) { Free_Memory_00781d3b(block); }
void release_object_46cf00(void *block) { Free_Memory_00781d3b(block); }
void release_object_46cf50(void *block) { Free_Memory_00781d3b(block); }
void release_object_46da70(void *block) { Free_Memory_00781d3b(block); }
void release_object_46dac0(void *block) { Free_Memory_00781d3b(block); }
void release_object_46db10(void *block) { Free_Memory_00781d3b(block); }
void release_object_47d090(void *block) { Free_Memory_00781d3b(block); }
void release_object_47ffb0(void *block) { Free_Memory_00781d3b(block); }
void release_object_480000(void *block) { Free_Memory_00781d3b(block); }
void release_object_48dbd0(void *block) { Free_Memory_00781d3b(block); }
void release_object_48dc20(void *block) { Free_Memory_00781d3b(block); }
void release_object_491340(void *block) { Free_Memory_00781d3b(block); }
void release_object_491390(void *block) { Free_Memory_00781d3b(block); }
void release_object_496730(void *block) { Free_Memory_00781d3b(block); }
void release_object_496780(void *block) { Free_Memory_00781d3b(block); }
void release_object_4aa080(void *block) { Free_Memory_00781d3b(block); }
void release_object_4aa0d0(void *block) { Free_Memory_00781d3b(block); }
void release_object_4aa120(void *block) { Free_Memory_00781d3b(block); }
void release_object_4aa170(void *block) { Free_Memory_00781d3b(block); }
void release_object_4aa1c0(void *block) { Free_Memory_00781d3b(block); }
void release_object_4b28b0(void *block) { Free_Memory_00781d3b(block); }
void release_object_4b2900(void *block) { Free_Memory_00781d3b(block); }
void release_object_4b45f0(void *block) { Free_Memory_00781d3b(block); }
void release_object_4b4640(void *block) { Free_Memory_00781d3b(block); }
void release_object_4b5770(void *block) { Free_Memory_00781d3b(block); }
void release_object_4ba320(void *block) { Free_Memory_00781d3b(block); }
void release_object_4ba370(void *block) { Free_Memory_00781d3b(block); }
void release_object_4c2480(void *block) { Free_Memory_00781d3b(block); }
void release_object_4c24d0(void *block) { Free_Memory_00781d3b(block); }
void release_object_4cdf30(void *block) { Free_Memory_00781d3b(block); }
void release_object_4cdf90(void *block) { Free_Memory_00781d3b(block); }
void release_object_4ce9f0(void *block) { Free_Memory_00781d3b(block); }
void release_object_4cea40(void *block) { Free_Memory_00781d3b(block); }
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
void release_object_4df240(void *block) { Free_Memory_00781d3b(block); }
void release_object_4df290(void *block) { Free_Memory_00781d3b(block); }
void release_object_4f6ee0(void *block) { Free_Memory_00781d3b(block); }
void release_object_4f8490(void *block) { Free_Memory_00781d3b(block); }
void release_object_4f84e0(void *block) { Free_Memory_00781d3b(block); }
void release_object_4f8530(void *block) { Free_Memory_00781d3b(block); }
void release_object_4f8580(void *block) { Free_Memory_00781d3b(block); }
void release_object_4f85d0(void *block) { Free_Memory_00781d3b(block); }
void release_object_4f8620(void *block) { Free_Memory_00781d3b(block); }
void release_object_4f8670(void *block) { Free_Memory_00781d3b(block); }
void release_object_4f86c0(void *block) { Free_Memory_00781d3b(block); }
void release_object_4f8710(void *block) { Free_Memory_00781d3b(block); }
void release_object_4f8760(void *block) { Free_Memory_00781d3b(block); }
void release_object_4f87b0(void *block) { Free_Memory_00781d3b(block); }
void release_object_50b260(void *block) { Free_Memory_00781d3b(block); }
void release_object_50b2b0(void *block) { Free_Memory_00781d3b(block); }
void release_object_51e670(void *block) { Free_Memory_00781d3b(block); }
void release_object_51e6c0(void *block) { Free_Memory_00781d3b(block); }
void release_object_520f00(void *block) { Free_Memory_00781d3b(block); }
void release_object_520f50(void *block) { Free_Memory_00781d3b(block); }
void release_object_522460(void *block) { Free_Memory_00781d3b(block); }
void release_object_5224b0(void *block) { Free_Memory_00781d3b(block); }
void release_object_52ec50(void *block) { Free_Memory_00781d3b(block); }
void release_object_52eca0(void *block) { Free_Memory_00781d3b(block); }
void release_object_52ecf0(void *block) { Free_Memory_00781d3b(block); }
void release_object_52ed40(void *block) { Free_Memory_00781d3b(block); }
void release_object_531e40(void *block) { Free_Memory_00781d3b(block); }
void release_object_531e90(void *block) { Free_Memory_00781d3b(block); }
void release_object_534610(void *block) { Free_Memory_00781d3b(block); }
void release_object_534660(void *block) { Free_Memory_00781d3b(block); }
void release_object_538260(void *block) { Free_Memory_00781d3b(block); }
void release_object_5382b0(void *block) { Free_Memory_00781d3b(block); }
void release_object_538300(void *block) { Free_Memory_00781d3b(block); }
void release_object_538350(void *block) { Free_Memory_00781d3b(block); }
void release_object_539b30(void *block) { Free_Memory_00781d3b(block); }
void release_object_539b80(void *block) { Free_Memory_00781d3b(block); }
void release_object_53aaf0(void *block) { Free_Memory_00781d3b(block); }
void release_object_53cd30(void *block) { Free_Memory_00781d3b(block); }
void release_object_53cd80(void *block) { Free_Memory_00781d3b(block); }
void release_object_56ae40(void *block) { Free_Memory_00781d3b(block); }
void release_object_56ae90(void *block) { Free_Memory_00781d3b(block); }
void release_object_56c1b0(void *block) { Free_Memory_00781d3b(block); }
void release_object_56c200(void *block) { Free_Memory_00781d3b(block); }
void release_object_56c870(void *block) { Free_Memory_00781d3b(block); }
void release_object_56c8c0(void *block) { Free_Memory_00781d3b(block); }
void release_object_56c910(void *block) { Free_Memory_00781d3b(block); }
void release_object_56c960(void *block) { Free_Memory_00781d3b(block); }
void release_object_58ec00(void *block) { Free_Memory_00781d3b(block); }
void release_object_58ec50(void *block) { Free_Memory_00781d3b(block); }
void release_object_58eca0(void *block) { Free_Memory_00781d3b(block); }
void release_object_58ecf0(void *block) { Free_Memory_00781d3b(block); }
void release_object_58ed40(void *block) { Free_Memory_00781d3b(block); }
void release_object_58ed90(void *block) { Free_Memory_00781d3b(block); }
void release_object_58ede0(void *block) { Free_Memory_00781d3b(block); }
void release_object_58ee30(void *block) { Free_Memory_00781d3b(block); }
void release_object_58ee80(void *block) { Free_Memory_00781d3b(block); }
void release_object_5a2280(void *block) { Free_Memory_00781d3b(block); }
void release_object_5a6360(void *block) { Free_Memory_00781d3b(block); }
void release_object_5a6bf0(void *block) { Free_Memory_00781d3b(block); }
void release_object_5a6c40(void *block) { Free_Memory_00781d3b(block); }
void release_object_5a6c90(void *block) { Free_Memory_00781d3b(block); }
void release_object_5a6ce0(void *block) { Free_Memory_00781d3b(block); }
void release_object_5a6d30(void *block) { Free_Memory_00781d3b(block); }
void release_object_5b6870(void *block) { Free_Memory_00781d3b(block); }
void release_object_5b68c0(void *block) { Free_Memory_00781d3b(block); }
void release_object_5b6910(void *block) { Free_Memory_00781d3b(block); }
void release_object_5b6960(void *block) { Free_Memory_00781d3b(block); }
void release_object_5b9700(void *block) { Free_Memory_00781d3b(block); }
void release_object_5b9750(void *block) { Free_Memory_00781d3b(block); }
void release_object_5b97a0(void *block) { Free_Memory_00781d3b(block); }
void release_object_5b97f0(void *block) { Free_Memory_00781d3b(block); }
void release_object_5cd900(void *block) { Free_Memory_00781d3b(block); }
void release_object_5cd950(void *block) { Free_Memory_00781d3b(block); }
void release_object_5d82f0(void *block) { Free_Memory_00781d3b(block); }
void release_object_5d8340(void *block) { Free_Memory_00781d3b(block); }
void release_object_5de5c0(void *block) { Free_Memory_00781d3b(block); }
void release_object_5de610(void *block) { Free_Memory_00781d3b(block); }
void release_object_6055a0(void *block) { Free_Memory_00781d3b(block); }
void release_object_6055f0(void *block) { Free_Memory_00781d3b(block); }
void release_object_6081b0(void *block) { Free_Memory_00781d3b(block); }
void release_object_608200(void *block) { Free_Memory_00781d3b(block); }
void release_object_61ba10(void *block) { Free_Memory_00781d3b(block); }
void release_object_61ba60(void *block) { Free_Memory_00781d3b(block); }
void release_object_61bab0(void *block) { Free_Memory_00781d3b(block); }
void release_object_61bb00(void *block) { Free_Memory_00781d3b(block); }
void release_object_61bb50(void *block) { Free_Memory_00781d3b(block); }
void release_object_61bba0(void *block) { Free_Memory_00781d3b(block); }
void release_object_61bbf0(void *block) { Free_Memory_00781d3b(block); }
void release_object_61bc40(void *block) { Free_Memory_00781d3b(block); }
void release_object_61bc90(void *block) { Free_Memory_00781d3b(block); }
void release_object_61bce0(void *block) { Free_Memory_00781d3b(block); }
void release_object_622040(void *block) { Free_Memory_00781d3b(block); }
void release_object_635040(void *block) { Free_Memory_00781d3b(block); }
void release_object_6365a0(void *block) { Free_Memory_00781d3b(block); }
void release_object_6365f0(void *block) { Free_Memory_00781d3b(block); }
void release_object_638270(void *block) { Free_Memory_00781d3b(block); }
void release_object_6382c0(void *block) { Free_Memory_00781d3b(block); }
void release_object_63cac0(void *block) { Free_Memory_00781d3b(block); }
void release_object_63cb10(void *block) { Free_Memory_00781d3b(block); }
void release_object_6401d0(void *block) { Free_Memory_00781d3b(block); }
void release_object_640220(void *block) { Free_Memory_00781d3b(block); }
void release_object_654390(void *block) { Free_Memory_00781d3b(block); }
void release_object_6543e0(void *block) { Free_Memory_00781d3b(block); }
void release_object_654430(void *block) { Free_Memory_00781d3b(block); }
void release_object_654480(void *block) { Free_Memory_00781d3b(block); }
void release_object_6544d0(void *block) { Free_Memory_00781d3b(block); }
void release_object_654520(void *block) { Free_Memory_00781d3b(block); }
void release_object_6739f0(void *block) { Free_Memory_00781d3b(block); }
void release_object_674790(void *block) { Free_Memory_00781d3b(block); }
void release_object_6748c0(void *block) { Free_Memory_00781d3b(block); }
void release_object_674910(void *block) { Free_Memory_00781d3b(block); }
void release_object_674960(void *block) { Free_Memory_00781d3b(block); }
void release_object_6749b0(void *block) { Free_Memory_00781d3b(block); }
void release_object_674a00(void *block) { Free_Memory_00781d3b(block); }
void release_object_674a50(void *block) { Free_Memory_00781d3b(block); }
void release_object_687330(void *block) { Free_Memory_00781d3b(block); }
void release_object_687380(void *block) { Free_Memory_00781d3b(block); }
void release_object_68afa0(void *block) { Free_Memory_00781d3b(block); }
void release_object_68aff0(void *block) { Free_Memory_00781d3b(block); }
void release_object_68b040(void *block) { Free_Memory_00781d3b(block); }
void release_object_68b090(void *block) { Free_Memory_00781d3b(block); }
void release_object_6986a0(void *block) { Free_Memory_00781d3b(block); }
void release_object_6986f0(void *block) { Free_Memory_00781d3b(block); }
void release_object_69c180(void *block) { Free_Memory_00781d3b(block); }
void release_object_69c310(void *block) { Free_Memory_00781d3b(block); }
void release_object_6acfc0(void *block) { Free_Memory_00781d3b(block); }
void release_object_6ad010(void *block) { Free_Memory_00781d3b(block); }
void release_object_6ad060(void *block) { Free_Memory_00781d3b(block); }
void release_object_6ad0b0(void *block) { Free_Memory_00781d3b(block); }
void release_object_6b3560(void *block) { Free_Memory_00781d3b(block); }
void release_object_6b35b0(void *block) { Free_Memory_00781d3b(block); }
void release_object_6b51b0(void *block) { Free_Memory_00781d3b(block); }
void release_object_6b5200(void *block) { Free_Memory_00781d3b(block); }
void release_object_6e2d70(void *block) { Free_Memory_00781d3b(block); }
void release_object_6e2dc0(void *block) { Free_Memory_00781d3b(block); }
void release_object_6e7180(void *block) { Free_Memory_00781d3b(block); }
void release_object_6e71d0(void *block) { Free_Memory_00781d3b(block); }
void release_object_6e86e0(void *block) { Free_Memory_00781d3b(block); }
void release_object_6eacd0(void *block) { Free_Memory_00781d3b(block); }
void release_object_6ead20(void *block) { Free_Memory_00781d3b(block); }
void release_object_6ec1d0(void *block) { Free_Memory_00781d3b(block); }
void release_object_70f3c0(void *block) { Free_Memory_00781d3b(block); }
void release_object_712a40(void *block) { Free_Memory_00781d3b(block); }
void release_object_712a90(void *block) { Free_Memory_00781d3b(block); }
void release_object_714ff0(void *block) { Free_Memory_00781d3b(block); }
void release_object_715040(void *block) { Free_Memory_00781d3b(block); }
void release_object_716ce0(void *block) { Free_Memory_00781d3b(block); }
void release_object_716d30(void *block) { Free_Memory_00781d3b(block); }
void release_object_72ba00(void *block) { Free_Memory_00781d3b(block); }
void release_object_72ba50(void *block) { Free_Memory_00781d3b(block); }
void release_object_72baa0(void *block) { Free_Memory_00781d3b(block); }
void release_object_766800(void *block) { Free_Memory_00781d3b(block); }
void release_object_766850(void *block) { Free_Memory_00781d3b(block); }
void release_object_76bdc0(void *block) { Free_Memory_00781d3b(block); }
void release_object_76be10(void *block) { Free_Memory_00781d3b(block); }
void release_object_76d870(void *block) { Free_Memory_00781d3b(block); }
void release_object_76d8c0(void *block) { Free_Memory_00781d3b(block); }
// release_<addr>: the deleting destructors' free of the object (0x00781D3B).
void release_40e2e0(void *block) { Free_Memory_00781d3b(block); }
void release_42ae60(void *block) { Free_Memory_00781d3b(block); }
void release_453de0(void *block) { Free_Memory_00781d3b(block); }
void release_47cfa0(void *block) { Free_Memory_00781d3b(block); }
void release_47fcf0(void *block) { Free_Memory_00781d3b(block); }
void release_496470(void *block) { Free_Memory_00781d3b(block); }
void release_4a5780(void *block) { Free_Memory_00781d3b(block); }
void release_4a57a0(void *block) { Free_Memory_00781d3b(block); }
void release_4a9b00(void *block) { Free_Memory_00781d3b(block); }
void release_4a9b60(void *block) { Free_Memory_00781d3b(block); }
void release_4b54b0(void *block) { Free_Memory_00781d3b(block); }
void release_4b5600(void *block) { Free_Memory_00781d3b(block); }
void release_4c2080(void *block) { Free_Memory_00781d3b(block); }
void release_4cde10(void *block) { Free_Memory_00781d3b(block); }
void release_4deef0(void *block) { Free_Memory_00781d3b(block); }
void release_4def10(void *block) { Free_Memory_00781d3b(block); }
void release_4f6e00(void *block) { Free_Memory_00781d3b(block); }
void release_4fef00(void *block) { Free_Memory_00781d3b(block); }
void release_51e0a0(void *block) { Free_Memory_00781d3b(block); }
void release_52e6b0(void *block) { Free_Memory_00781d3b(block); }
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
void *allocate_storage_0046d1f0(unsigned int byte_count) { return ::operator new(byte_count); }
void *allocate_storage_004faa90(unsigned int byte_count) { return ::operator new(byte_count); }
void *allocate_storage_004fae60(unsigned int byte_count) { return ::operator new(byte_count); }
void *allocate_storage_004fb230(unsigned int byte_count) { return ::operator new(byte_count); }
void *allocate_storage_0050b480(unsigned int byte_count) { return ::operator new(byte_count); }
void *allocate_storage_0058e170(unsigned int byte_count) { return ::operator new(byte_count); }
void *allocate_storage_005b95c0(unsigned int byte_count) { return ::operator new(byte_count); }
void *allocate_storage_005cd770(unsigned int byte_count) { return ::operator new(byte_count); }
void *allocate_storage_0061ac80(unsigned int byte_count) { return ::operator new(byte_count); }
void *allocate_storage_0061af90(unsigned int byte_count) { return ::operator new(byte_count); }
void *allocate_storage_00653340(unsigned int byte_count) { return ::operator new(byte_count); }
void *allocate_storage_00653730(unsigned int byte_count) { return ::operator new(byte_count); }
void *allocate_storage_00653a40(unsigned int byte_count) { return ::operator new(byte_count); }
void *allocate_storage_006634e0(unsigned int byte_count) { return ::operator new(byte_count); }
void *allocate_storage_006c03e0(unsigned int byte_count) { return ::operator new(byte_count); }
void *allocate_storage_007246d0(unsigned int byte_count) { return ::operator new(byte_count); }
void *allocate_words_0046ce20(unsigned int byte_count) { return ::operator new(byte_count); }
void *allocate_words_004f6f30(unsigned int byte_count) { return ::operator new(byte_count); }
void *allocate_words_004fb4a0(unsigned int byte_count) { return ::operator new(byte_count); }
void *allocate_words_004fb530(unsigned int byte_count) { return ::operator new(byte_count); }
void *allocate_words_005a62d0(unsigned int byte_count) { return ::operator new(byte_count); }
void *allocate_words_006547b0(unsigned int byte_count) { return ::operator new(byte_count); }
void *allocate_words_00654840(unsigned int byte_count) { return ::operator new(byte_count); }
void *allocate_words_006548d0(unsigned int byte_count) { return ::operator new(byte_count); }
void *storage_40bc10(unsigned int byte_count) { return ::operator new(byte_count); }
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
void *storage_4fb610(unsigned int byte_count) { return ::operator new(byte_count); }
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
