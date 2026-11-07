// SPDX-License-Identifier: GPL-3.0-or-later
// Copyright 2026 ra2-decomp contributors
// Copyright 2025 Electronic Arts Inc.; 2026 OpenTS contributors
// Derived from Electronic Arts source code; EA's GPL Section 7 terms apply (COPYING.md).
// From ra2-decomp 0x005A1190 MapGeneratorClass::Get_Angle_Score (src/mapgen.cpp, YR twin): the angular
// score of a cell seen from another against a reference heading, plus a random jitter. The pi constants and
// 1/UINT_MAX are literals (pooled __real constants at 0x79B630/0x79D158/0x79C9B8/0x7A5DD0); the two
// weights at 0x7A5CC8 (2.0, the twin's _random_weight) and 0x7A5CD0 (1.5, _angle_weight) are real data.
#include "shared/unknown_map_generator.hpp"
#include "shared/fast_math.hpp"
#include "shared/random2.hpp"
#include "shared/crt_math.hpp"
#include "shared/map_random.hpp"
extern double const angle_weight_7a5cd0,jitter_weight_7a5cc8;
// @ref ?angle_weight_7a5cd0@@=007a5cd0
// @ref ?jitter_weight_7a5cc8@@=007a5cc8
// @ref ?unknown_map_random_a705c0@@=00a705c0
// @ref ??RRandom2Class@@QAEHXZ=00638840
// @ref __real@8@3fffc90fdaa22168c000=0079b630
// @ref __real@8@4000c90fdaa22168c000=0079d158
// @ref __real@8@4001c90fdaa22168c000=0079c9b8
// @ref __real@8@3fdf8000000080000000=007a5dd0
// ECX is unused, RET16/ST0 are physical.
double UnknownMapGenerator::Angle_Score_00582970(Cell const &cell1,Cell const &cell2,double ref_angle)
{
    int dx=cell2.X-cell1.X;
    int dy=cell2.Y-cell1.Y;

    double angle=(dx!=0) ? Fast_Atan(-((double)dy/(double)dx)) : 1.5707963267948966;
    if(dx<0) {
        angle+=3.141592653589793;
    }

    double angle_diff=fabs(angle-ref_angle);
    while(angle_diff>=3.141592653589793*2) {
        angle_diff-=3.141592653589793*2;
    }
    if(angle_diff>3.141592653589793) {
        angle_diff=3.141592653589793*2-angle_diff;
    }

    return(((unsigned)unknown_map_random_a705c0()*(1.0 / 4294967295.0))*jitter_weight_7a5cc8+angle_weight_7a5cd0*angle_diff);
}
