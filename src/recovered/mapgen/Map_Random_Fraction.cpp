#include "shared/random.hpp"
extern Random2Class RMGRandom;
// Random map callback: map the generator's complete unsigned word to [0,1].
double __cdecl Map_Random_Fraction_00579770()
{
    return (unsigned int)RMGRandom() * (1.0 / 4294967295.0);
}
