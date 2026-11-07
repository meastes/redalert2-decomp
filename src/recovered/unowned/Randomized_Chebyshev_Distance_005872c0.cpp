#include "shared/coord.hpp"
#include "shared/random2.hpp"
#include "shared/map_random.hpp"
// @ref ?unknown_map_random_a705c0@@=00a705c0
// @ref ??RRandom2Class@@QAEHXZ=00638840
#include "shared/crt_memory.hpp"
extern double unknown_distance_scale_7a5d50;
// @ref ?unknown_distance_scale_7a5d50@@=007a5d50
// The random term scales by 1/UINT_MAX as a literal (pooled __real at 0x007A5DD0), as ra2-decomp's
// RMG_RANDOM_DOUBLE writes it in MapGeneratorClass::Get_Urban_Score (0x005A5AE0, YR twin); a folded literal,
// not a memory operand, is what lets Create_Urban_Area 0x00586B40 inline this body exactly.
double __stdcall Randomized_Chebyshev_Distance_005872c0(Cell const &a,Cell const &b)
{
    int x = (int)a.X - (int)b.X;
    int y = (int)a.Y - (int)b.Y;
    double distance=abs(x)>abs(y)?abs(x):abs(y);
    return distance + (unsigned int)unknown_map_random_a705c0()*(1.0 / 4294967295.0)*unknown_distance_scale_7a5d50;
}
