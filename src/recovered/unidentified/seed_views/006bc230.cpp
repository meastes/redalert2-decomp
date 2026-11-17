extern "C" unsigned int global_6bc230_a35db4;
class Call_6bc230_0 {public: unsigned int Flag_To_Lose_004e80d0(unsigned int flag_argument);};
class Leaf_6bc230 { public: unsigned int TMission_LOSE_006bc230(unsigned int unused_script_argument_1, unsigned int unused_script_argument_2); };
unsigned int Leaf_6bc230::TMission_LOSE_006bc230(unsigned int unused_script_argument_1, unsigned int unused_script_argument_2) { unsigned int flag_result = ((Call_6bc230_0*)(global_6bc230_a35db4))->Flag_To_Lose_004e80d0((unsigned int)(0)); *(unsigned char*)(((unsigned int)this) + 0x80) = 1; return flag_result; }
