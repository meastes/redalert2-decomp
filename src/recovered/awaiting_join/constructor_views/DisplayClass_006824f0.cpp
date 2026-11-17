class Call_6824f0_0 {public: unsigned int target(unsigned int base_noinit_argument);};
extern "C" unsigned char data_6824f0_79e91c[];
class Leaf_6824f0 { public: unsigned int get(unsigned int noinit_address); };
unsigned int Leaf_6824f0::get(unsigned int noinit_address) { unsigned int unused_base_return = ((Call_6824f0_0*)((unsigned int)this))->target((unsigned int)(noinit_address)); *(unsigned char*)(((unsigned int)this) + 0x11c8) = 0; *(unsigned char*)(((unsigned int)this) + 0x11c9) = 0; *(unsigned char*)(((unsigned int)this) + 0x11ca) = 0; *(unsigned int*)(((unsigned int)this)) = (unsigned int)data_6824f0_79e91c; return (unsigned int)this; }
