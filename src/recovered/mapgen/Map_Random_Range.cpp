#include "shared/random.hpp"
extern Random2Class RMGRandom;
// @ref ?RMGRandom@@3VRandom2Class@@A=00a705c0
// @ref ??RRandom2Class@@QAEHXZ=00638840
// @ref __real@8@3fdf8000000080000000=007a5dd0
unsigned int __fastcall Map_Random_Range_005797a0(unsigned int minimum,unsigned int maximum)
{
    struct Range { double Span; double Origin; };
    Range range={(unsigned int)(maximum-minimum+1),minimum};
    unsigned int result;
    do {
        result=(unsigned int)((unsigned int)RMGRandom()*range.Span*(1.0/4294967295.0)+range.Origin);
    } while(result>maximum);
    return result;
}
