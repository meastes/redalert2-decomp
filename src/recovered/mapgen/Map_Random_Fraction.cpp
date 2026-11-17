#include "shared/random.hpp"
extern Random2Class RMGRandom;
// @ref ?RMGRandom@@3VRandom2Class@@A=00a705c0
// @ref ??RRandom2Class@@QAEHXZ=00638840
// @ref __real@8@3fdf8000000080000000=007a5dd0
// Random map callback: map the generator's complete unsigned word to [0,1].
double __cdecl Map_Random_Fraction_00579770()
{
    return (unsigned int)RMGRandom() * (1.0 / 4294967295.0);
}
