// SPDX-License-Identifier: GPL-3.0-or-later
// Copyright 2026 ra2-decomp contributors
// Copyright 2025 Electronic Arts Inc.; 2026 OpenTS contributors
// Derived from Electronic Arts source code; EA's GPL Section 7 terms apply (COPYING.md).
// void PowerClass::Draw_It(bool complete) (0x0061C8C0, slot 16)
// Draws the power bar when it needs it: one shape frame per segment, down
// the sidebar's edge, three pixels apart. The segments the counters do not
// cover come first (frame 0), then a blinking one (frame 4) while the first
// counter is odd... and the three counters' segments (frames 1, 2, 3).
// Then the radar's drawing. Draft: Display245/Power338; the side-dependent
// x and y offsets take the shape of ra2-decomp PowerClass::Draw_It (YR
// 0x0063FB20, a fuzzy twin): the side is read again for the y offset, whose
// arms agree, as in the twin (Scout836).
#include "shared/power.hpp"
#include "shared/surface.hpp"
#include "shared/abstract.hpp"
#include "shared/palette.hpp"
// @ref ?Draw_It@RadarClass@@UAEX_N@Z=0062f580
// @ref ?unknown_id_source_a3d290@@3PAVUnknownIDSource@@A=00a3d290
// @ref ?unknown_flag_8373c1@@3_NA=008373c1
// @ref ?unknown_flag_abcd84@@3_NA=00abcd84
// @ref ?unknown_int_839624@@3HA=00839624
// @ref ?unknown_int_abcd70@@3HA=00abcd70
// @ref ?unknown_surface_839948@@3PAVSurface@@A=00839948
// @ref ?unknown_drawer_8323cc@@3PAVConvertClass@@A=008323cc
// @ref ?PowerShape@PowerClass@@2PBXB=00a7691c
// @ref ?CC_Draw_Shape@@YIXPAVSurface@@PAVConvertClass@@PBUShapeFileStruct@@HABUPoint2D@@ABVRect@@HPBEHHH2HU4@@Z=0049eea0

void PowerClass::Draw_It(bool complete)
{
    if ((complete || unknown_1508) && unknown_flag_8373c1) {
        unknown_1508 = false;
        unknown_flag_abcd84 = true;
        bool side = unknown_id_source_a3d290->unknown_2239;
        int shift;
        if (side) {
            shift = 5;
        } else {
            shift = 0;
        }
        side = unknown_id_source_a3d290->unknown_2239;
        int yoff;
        if (side) {
            yoff = 0x45;
        } else {
            yoff = 0x45;
        }
        int y = unknown_int_839624 + yoff;
        Rect window = unknown_surface_839948->Get_Rect();
        int unlit = Unknown_0061C6E0() - unknown_1530 - unknown_152c - unknown_1528;

        for (int index = 0; index < unlit; index++) {
            CC_Draw_Shape(unknown_surface_839948, unknown_drawer_8323cc, (ShapeFileStruct const *)PowerShape, 0,
                          Point2D(shift, y), window, 0x400, 0, 0, 0, 1000, 0, 0, Point2D(0, 0));
            y += 3;
        }
        int first = 0;
        if (unknown_1518 > 0 && unknown_1518 % 2 == 0) {
            CC_Draw_Shape(unknown_surface_839948, unknown_drawer_8323cc, (ShapeFileStruct const *)PowerShape, 4,
                          Point2D(shift, y), window, 0x400, 0, 0, 0, 1000, 0, 0, Point2D(0, 0));
            y += 3;
            first = 1;
        }
        if (unknown_1528 > 0) {
            for (; first < unknown_1528; first++) {
                CC_Draw_Shape(unknown_surface_839948, unknown_drawer_8323cc, (ShapeFileStruct const *)PowerShape, 1,
                              Point2D(shift, y), window, 0x400, 0, 0, 0, 1000, 0, 0, Point2D(0, 0));
                y += 3;
            }
            first = 0;
        }
        if (unknown_152c > 0) {
            for (; first < unknown_152c; first++) {
                CC_Draw_Shape(unknown_surface_839948, unknown_drawer_8323cc, (ShapeFileStruct const *)PowerShape, 2,
                              Point2D(shift, y), window, 0x400, 0, 0, 0, 1000, 0, 0, Point2D(0, 0));
                y += 3;
            }
            first = 0;
        }
        if (unknown_1530 > 0) {
            for (; first < unknown_1530; first++) {
                CC_Draw_Shape(unknown_surface_839948, unknown_drawer_8323cc, (ShapeFileStruct const *)PowerShape, 3,
                              Point2D(shift, y), window, 0x400, 0, 0, 0, 1000, 0, 0, Point2D(0, 0));
                y += 3;
            }
        }
    }
    RadarClass::Draw_It(complete);
}
