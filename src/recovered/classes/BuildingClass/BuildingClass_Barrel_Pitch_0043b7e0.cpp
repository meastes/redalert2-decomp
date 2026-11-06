// UnknownFacingDir BuildingClass::unknown_slot170(AbstractClass *target) 0x0043B7E0 (slot 170; TibSun
// building.cpp Barrel_Pitch): a laser weapon elevates toward the predicted target point from its turret
// fire point, the EM pulse cannon solves the ballistic pitch, anything else uses TechnoClass's.
// The laser arm is ra2-decomp's text (single return; DirType(int) and DirType::operator- from the unit's
// RA2_FACING_LEFT90/RA2_FACING_DIFFERENCE profile, defined in the first row 0x00438500).
// From ra2-decomp 0x0043E940 (YR twin, src/building.cpp BuildingClass::Barrel_Pitch):
// SPDX-License-Identifier: GPL-3.0-or-later
// Copyright 2026 ra2-decomp contributors
// Copyright 2025 Electronic Arts Inc.; 2026 OpenTS contributors
// Derived from Electronic Arts source code; EA's GPL Section 7 terms apply (COPYING.md).
#include "shared/building.hpp"
#include "shared/rules.hpp"
#include "shared/weapon_type.hpp"
#include "shared/tactical.hpp"
#include "shared/fast_math.hpp"
// @ref ?unknown_slot170@TechnoClass@@UAE?AUUnknownFacingDir@@PAVAbstractClass@@@Z=006ca540
// @ref ?Unknown_006A2900@Tactical@@QAE?AVPoint2D@@ABV2@@Z=006a2900
// @ref ?unknown_tactical_83996c@@3PAVTactical@@A=0083996c
// @ref ?Fast_Asin_004ba550@@YAMN@Z=004ba550
// @ref ?Fast_Atan2@@YAMNN@Z=004ba600
// @ref ?Fast_Sqrt@@YAMN@Z=004ba410
// @ref ?Rules@@3PAVRulesClass@@A=00839848
// @ref __real@8@3fff8000000000000000=0079a708
// @ref __real@8@400d8000000000000000=0079d0a0
// @ref __real@8@3ffea2f9836e4e441800=0079d098
// @ref __real@8@3fffc90fdaa22168c000=0079b630
// @ref __real@8@c00ca5f9836e4e441800=0079b628

// TibSun TPoint3D::Length.
inline int CoordStruct::Length() const
{
    return (int)Fast_Sqrt((double)X * X + (double)Y * Y + (double)Z * Z);
}
// TibSun ::Direction(Coord, Coord) (the twin discards its result).
inline UnknownFacingDir Direction_0043b7e0(CoordStruct const &from, CoordStruct const &to)
{
    return UnknownFacingDir((unsigned short)(int)((Fast_Atan2(double(from.Y) - to.Y, double(to.X) - from.X)
        - 1.5707963267948966) * -10430.060040584269));
}

// The twin's Point3D(TPoint2D, z): a coordinate's X and Y at height z.
inline CoordStruct XY_0043b7e0(CoordStruct const &from, int z)
{
    CoordStruct result;
    result.X=from.X;
    result.Y=from.Y;
    result.Z=z;
    return result;
}

UnknownFacingDir BuildingClass::unknown_slot170(AbstractClass *target)
{
    if (unknown_slot231(0)->WeaponType->unknown_121) {
        unknown_slot32();
        if (target != 0) {
            CoordStruct predicted = unknown_6d65b0();
            WeaponStruct *weap = unknown_slot231(0);
            int z = weap->FLH.Z;
            CoordStruct coord = unknown_slot42() + CoordStruct(0, 0, z);
            CoordStruct pt(unknown_tactical_83996c->Unknown_006A2900(Point2D(Type->Anims[9].Offset.X, Type->Anims[9].Offset.Y)), 0);
            coord += pt;
            CoordStruct fire = predicted - coord;

            if (fire.Z == 0) {
                return static_cast<Dir256>(64);
            }

            double dr;
            int d;

            d = fire.X * fire.X + fire.Y * fire.Y;
            dr = Fast_Sqrt(d);
            d = fire.Length();
            dr = dr / d;
            if (dr > 1) {
                dr = 1;
            }

            UnknownFacingDir dir = (int)((Fast_Asin_004ba550(dr) * 16384.0) * 0.6366197723675814);
            if (fire.Z < 0) {
                dir = UnknownFacingDir(static_cast<Dir256>(128)) - dir;
            }

            return dir;

        } else {
            return unknown_2c8.Current();
        }

    } else if (Type->EMPulseCannon) {
        CoordStruct tcoord = XY_0043b7e0(target->Center_Coord(), Map.Unknown_0055A280(target->Center_Coord()));
        CoordStruct fcoord = unknown_slot43(0);
        Direction_0043b7e0(fcoord, tcoord);
        int max_speed = Unknown_0047FB70(unknown_5d4c00(target), Rules->unknown_1320);

        CoordStruct ucoord = unknown_slot181(0);
        CoordStruct disp = target->Center_Coord() - ucoord;
        int distance2 = disp.X * disp.X + disp.Y * disp.Y;

        UnknownFacingDir pitch;
        bool valid = Unknown_0047F8B0(unknown_6d7d20(0), max_speed, (int)Fast_Sqrt(distance2), disp.Z, Rules->unknown_1320, &pitch);
        if (!valid) {
            valid = Unknown_0047F8B0(unknown_6d7d20(0), max_speed * 10 / 8, (int)Fast_Sqrt(distance2), disp.Z, Rules->unknown_1320, &pitch);
        }
        return pitch;
    } else {
        return TechnoClass::unknown_slot170(target);
    }
}
