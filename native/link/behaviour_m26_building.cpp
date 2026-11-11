// Behaviour rows (Scout1208, native M26; NOT EXACT, native/behaviour_rewrites.json).
#include "shared/building_class.hpp"
#include "shared/building_type_class.hpp"
#include "shared/house.hpp"
#include "shared/rules.hpp"
#include "shared/anim.hpp"
#include "shared/anim_type.hpp"
#include "shared/bullet.hpp"
#include "shared/weapon_type.hpp"
#include "shared/super_weapon_type.hpp"
#include "shared/techno_type.hpp"
#include "shared/cell.hpp"
#include "shared/map_view.hpp"
#include "shared/mission.hpp"
#include "shared/facing.hpp"
#include "shared/random2.hpp"
#include "shared/sound_play.hpp"
#include "shared/unknown_globals.hpp"

AnimClass *__fastcall Create_Animation_At_Object_00438b80(HouseClass *house, AbstractClass *object,
    AbstractClass *attachment, char const *name);                         // 0x00438B80 (recovered)

// 0x00448400 BuildingClass slot 142 Do_MISSION_MISSILE (TibSun building.cpp Do_MISSION_MISSILE;
// behaviour from the 99.77 parked draft build/class-work/Scout697/w/keep/mm.cpp (codex2/Scout697),
// rewritten over the current headers and with a local velocity in place of the draft's
// file-local TVelocity3D view). States in unknown_a8 (MissionClass Status):
//   nuke silo: 0 open the doors and put PSIWARN on the target cell, 1 wait for the doors
//   (unknown_5ad), 2 launch the type's super weapon bullet straight up (NukeTakeOff anim), 3 close,
//   4 done -> guard.
//   any other building with a super weapon (unknown_4cc) fires it at NukeDest and guards.
//   EM pulse cannon: 0 turn the turret then the barrel to EMPDest, 1 PULSBALL anim, 2 fire a
//   ballistic bullet (pitch from the arc solver 0x0047F8B0, 10/8 speed retry, else 0xFA00),
//   3 lower the barrel -> guard.
namespace {
double Dir_Radian_m26(UnknownFacingDir const &dir)
{
    return ((short)dir.Raw - 0x3fff) * -9.587672516830327e-05;
}
UnknownFacingDir Radian_Dir_m26(double radian)
{
    return UnknownFacingDir((unsigned short)(int)((radian - 1.5707963267948966) * -10430.060040584269));
}
bool Facing_Turning_m26(UnknownFacingDir const &current, UnknownFacingDir const &toward)
{
    short difference = (short)(unsigned short)(current.Raw - toward.Raw);
    return difference != 0;
}
struct Velocity_m26 {
    double X, Y, Z;
    double Horizontal() const { return unknown_sqrt_4ba410(X * X + Y * Y); }
    double Speed() const { return unknown_sqrt_4ba410(X * X + Y * Y + Z * Z); }
    void Set_Yaw(UnknownFacingDir const &yaw)
    {
        if (X == 0 && Y == 0) {
            X = 100;
        }
        double horizontal = Horizontal();
        X = horizontal * unknown_cos_4ba4d0(Dir_Radian_m26(yaw));
        Y = -horizontal * unknown_sin_4ba480(Dir_Radian_m26(yaw));
    }
    void Set_Speed(double speed)
    {
        if (X == 0 && Y == 0 && Z == 0) {
            X = 100;
        }
        double ratio = speed / Speed();
        X *= ratio;
        Y *= ratio;
        Z *= ratio;
    }
    void Set_Pitch(UnknownFacingDir const &pitch)
    {
        double current = Dir_Radian_m26(Radian_Dir_m26(unknown_math_4ba600(Z, Horizontal())));
        double speed = Speed();
        if (current != 0) {
            X /= unknown_cos_4ba4d0(current);
            Y /= unknown_cos_4ba4d0(current);
        }
        X *= unknown_cos_4ba4d0(Dir_Radian_m26(pitch));
        Y *= unknown_cos_4ba4d0(Dir_Radian_m26(pitch));
        Z = speed * unknown_sin_4ba480(Dir_Radian_m26(pitch));
    }
};
}

int BuildingClass::Do_MISSION_MISSILE()
{
    if (Type->NukeSilo) {
        switch (unknown_a8) {
        case 0:
            unknown_5ad = false;
            unknown_443d60(2);
            unknown_a8 = 1;
            unknown_444 = Create_Animation_At_Object_00438b80(House, &Map[House->NukeDest], 0, "PSIWARN");
            // fall through
        case 1:
            if (unknown_5ad) {
                unknown_443d60(4);
                unknown_a8 = 2;
            }
            // fall through
        case 2: {
            AbstractClass *target = &Map[House->NukeDest];
            WeaponTypeClass *weapon = SuperWeaponTypes[unknown_4cc]->WeaponType;
            BulletClass *bullet = Create_Bullet(weapon->Projectile, target, this, weapon->unknown_a4,
                weapon->Warhead, 255, true);
            if (bullet) {
                bullet->Set_Weapon_Type(weapon);
                if (unknown_444 != 0) {
                    ((AnimClass *)unknown_444)->unknown_func_422680(bullet);
                    unknown_444 = 0;
                }
                bullet->Limbo();
                CoordStruct launch = unknown_slot43(0);
                // Straight up at speed 10 (TibSun: Velocity(DIR_N, DIR_N, 10)): yaw 0, pitch 0 of the
                // RA2 direction view gives the vector below.
                double yaw = Dir_Radian_m26(UnknownFacingDir(0));
                double pitch = Dir_Radian_m26(UnknownFacingDir(0));
                TVelocity3D<double> velocity(unknown_cos_4ba4d0(yaw) * unknown_cos_4ba4d0(pitch) * 10.0,
                    unknown_sin_4ba480(yaw) * unknown_cos_4ba4d0(pitch) * 10.0,
                    unknown_sin_4ba480(pitch) * 10.0);
                if (!bullet->unknown_slot118(launch, velocity)) {
                    delete bullet;
                    unknown_a8 = 3;
                    return 1;
                }
                AnimClass *anim = new AnimClass(Rules->NukeTakeOff, launch, 0, 1, 0x600, 0);
                anim->ZAdjust = -100;
                unknown_a8 = 3;
            }
            return 1;
        }
        case 3:
            unknown_443d60(5);
            unknown_a8 = 4;
            return 6;
        case 4:
            unknown_443d60(1);
            unknown_slot116(5, false);
            return 60;
        default:
            break;
        }
    }

    if (unknown_4cc != -1) {
        unknown_slot221(&Map[House->NukeDest], 0);
        unknown_slot116(5, false);
    }

    if (Type->EMPulseCannon) {
        switch (unknown_a8) {
        case 0: {
            UnknownFacingDir aim = unknown_slot285(&Map[House->EMPDest]);
            UnknownFacingDir barrel = unknown_slot170(&Map[House->EMPDest]);
            if (Facing_Turning_m26(unknown_2e0.Current(), aim)) {
                unknown_2e0.Set_Desired(aim);
            } else if (Facing_Turning_m26(unknown_2c8.Current(), barrel)) {
                unknown_2c8.Set_Desired(barrel);
            } else {
                unknown_a8 = 1;
            }
            return 1;
        }
        case 1:
            new AnimClass(AnimTypes[Anim_From_Name("PULSBALL")], unknown_slot43(0), 0, 1, 0x600, 0);
            unknown_a8 = 2;
            return 32;
        case 3:
            unknown_2c8.Set_Desired(UnknownFacingDir(0x4000));
            unknown_443d60(1);
            unknown_slot116(5, false);
            return 60;
        case 2: {
            WeaponTypeClass *weapon = unknown_slot231(0)->WeaponType;
            CellClass *target = &Map[House->EMPDest];
            if (Debug_Map || *(int const *)&House->EMPDest == unknown_84ed10) {
                unknown_443d60(1);
                unknown_slot116(5, false);
                return 60;
            }
            unknown_2c8.Set_Desired(unknown_slot170(target));

            CoordStruct fire = unknown_slot43(0);
            Cell const &destination = House->EMPDest;
            CoordStruct position(destination.X * 256 + 128, destination.Y * 256 + 128, 0);
            Map.Get_Height_GL(position);
            CoordStruct to(House->EMPDest.X * 256 + 128, House->EMPDest.Y * 256 + 128, 0);
            UnknownFacingDir yaw = Radian_Dir_m26(unknown_math_4ba600(double(fire.Y) - to.Y, double(to.X) - fire.X));
            int speed = Unknown_0047FB70(unknown_5d4c00(target), Rules->unknown_1320);

            BulletClass *bullet = Create_Bullet(weapon->Projectile, target, this, weapon->unknown_a4,
                weapon->Warhead, 3 * speed / 4, weapon->unknown_10b);

            Velocity_m26 velocity = {0, 0, 0};
            velocity.Set_Yaw(yaw);
            velocity.Set_Speed(speed);

            CoordStruct source = unknown_slot181(0);
            CoordStruct centre = target->Center_Coord();
            int dx = centre.X - source.X;
            int dy = centre.Y - source.Y;
            int dz = centre.Z - source.Z;
            int distance = (int)unknown_sqrt_4ba410((double)(dx * dx + dy * dy));

            UnknownFacingDir pitch;
            bool valid = Unknown_0047F8B0(unknown_6d7d20(0), speed, distance, dz, Rules->unknown_1320, &pitch);
            if (!valid) {
                valid = Unknown_0047F8B0(unknown_6d7d20(0), speed * 10 / 8, distance, dz, Rules->unknown_1320, &pitch);
                if (!valid) {
                    pitch = UnknownFacingDir(0xFA00);
                    valid = true;
                }
            }
            velocity.Set_Pitch(pitch);

            if (bullet && valid && bullet->unknown_slot118(source, TVelocity3D<double>(velocity.X, velocity.Y, velocity.Z))) {
                if (unknown_slot232() && Techno_Type_Class()->TurretRecoil) {
                    unknown_330.unknown_6d8b90();
                    unknown_350.unknown_6d8b90();
                }
                int random_value = unknown_random_839218();
                Voc_Play_At(weapon->unknown_bc.Pick(random_value), unknown_slot43(0), 0);
                unknown_a8 = 3;
                return 1;
            }
            if (bullet) {
                bullet->Release();
            }
            unknown_443d60(1);
            unknown_slot116(5, false);
            return 60;
        }
        default:
            break;
        }
    }

    return Control()->Normal_Delay();
}
