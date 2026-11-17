// Int<64> (BigInt) statics (Westwood's INT.CPP): Error 0x00A417C0, Carry 0x00A417C4,
// Borrow 0x00A417C5 and Remainder 0x00A416C0, whose dynamic initializer (0x0051E7F0, _$E)
// runs the inline Int() constructor (XMP_Init of 64 digits). The unit then has the
// file-local unknown_static_rect (0x0051E800) and Tiberian Sun's interpal.cpp functions.
#include "shared/int.hpp"

int BigInt::Error = 0;
bool BigInt::Carry = false;
bool BigInt::Borrow = false;
BigInt BigInt::Remainder;
