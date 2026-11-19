#include "random.h"

RandomClass::RandomClass(unsigned seed) : Seed(seed)
{
}

int RandomClass::operator()()
{
	Seed = Seed * MULT_CONSTANT + ADD_CONSTANT;
	return (Seed >> THROW_AWAY_BITS) & ((1 << SIGNIFICANT_BITS) - 1);
}

int RandomClass::operator()(int minval, int maxval)
{
	return Pick_Random_Number(*this, minval, maxval);
}

Random2Class::Random2Class(unsigned seed) : Index1(0), Index2(103)
{
	Random3Class generator(seed);
	for (int i = 0; i < TABLE_SIZE; ++i) {
		Table[i] = generator();
	}
	IsLocked = false;
}

int Random2Class::operator()()
{
	if (IsLocked) return 0;
	Table[Index1] ^= Table[Index2];
	int value = Table[Index1];
	++Index1;
	++Index2;
	if (Index1 >= TABLE_SIZE) Index1 = 0;
	if (Index2 >= TABLE_SIZE) Index2 = 0;
	return value;
}

int Random2Class::operator()(int minval, int maxval)
{
	return Pick_Random_Number(*this, minval, maxval);
}

Random3Class::Random3Class(unsigned seed1, unsigned seed2) : Seed(seed1), Index(seed2)
{
}

int Random3Class::operator()()
{
	int left = Seed;
	int right = Index++;
	for (int round = 0; round < 4; ++round) {
		int saved = right;
		int mixed = Mix1[round] ^ right;
		int low = mixed & 0xffff;
		int high = mixed >> 16;
		int square = ~(high * high) + low * low;
		right = (((square >> 16 | square << 16) ^ Mix2[round]) + high * low) ^ left;
		left = saved;
	}
	return right;
}

int Random3Class::operator()(int minval, int maxval)
{
	return Pick_Random_Number(*this, minval, maxval);
}
