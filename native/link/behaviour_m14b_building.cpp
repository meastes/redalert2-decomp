// Behaviour rows (Scout1121, native M14b; NOT EXACT, native/behaviour_rewrites.json).
#include "shared/building_class.hpp"
#include "shared/id_source.hpp"

// 0x00446920 BuildingClass slot 126 (Mission_Attack; pass-6a decompile 00446920
// and the original's jump table at 0x00447214, written out case by case;
// Scout1191 M24 replaces Scout1121's simplified row).
// SAM buildings (Type->SAM) run the two-state launcher (unknown_a8: 0 tracking,
// 1 firing). Others: no target -> drop it, guard; else the weapon slot 174 picks,
// Can_Fire (slot 218); FIRE_FACING on a voxel turret within the type's ROT snaps
// the facing and asks again. Fire results as the table: OK fires (two-shooter
// upgrades both weapons; the prism tower gathers the nearest ready prism tower
// in weapon range as support up to Rules->PrismSupportMax, else charges its own
// delayed shot; IsAnimDelayedFire types charge the delayed shot), FACING/REARM
// turn and return 2, CLOAKED uncloaks, ILLEGAL/CANT/RANGE/AMMO drop the target.
int BuildingClass::Do_MISSION_ATTACK()
{
    if (!Type->SAM) {
        if (unknown_228 == 0) {
            unknown_slot220(0);
            unknown_534 = 0;
            if (unknown_slot242()) {
                unknown_6d1470();
            }
            if (Get_Mission() != 0x1C) {
                unknown_slot116(5, false);
                Commence();
            }
            return 1;
        }
        int weapon = unknown_slot174(unknown_228);
        unknown_5ad = true;
        unsigned fire = (unsigned)unknown_slot218(unknown_228, weapon, true);
        if (fire == 2 && unknown_slot232() && Type->TurretAnimIsVoxel) {
            UnknownFacingDir dir = unknown_slot285(unknown_228);
            bool snap = true;
            int rot = Type->ROT;
            if (rot != 0) {
                UnknownFacingDir current = unknown_2e0.Current();
                int step = (short)(unsigned short)((rot & 0xFF) << 8);
                int diff = (short)(*(short *)&current - *(short *)&dir);
                if ((step < 0 ? -step : step) < (diff < 0 ? -diff : diff)) {
                    snap = false;
                }
            }
            if (snap) {
                unknown_2e0.Set_Current(dir);
                fire = (unsigned)unknown_slot218(unknown_228, weapon, true);
            }
        }
        switch (fire) {
        case 1: case 5: case 6: case 8:         // FIRE_AMMO, FIRE_ILLEGAL, FIRE_CANT, FIRE_RANGE
            unknown_slot220(0);
            unknown_534 = 0;
            if (unknown_slot242()) {
                unknown_6d1470();
            }
            if (Get_Mission() == 0x1C) {
                return 1;
            }
            unknown_slot116(5, false);
            Commence();
            break;
        case 2: case 3:                         // FIRE_FACING, FIRE_REARM
            if (unknown_228 != 0) {
                UnknownFacingDir dir = unknown_slot285(unknown_228);
                unknown_2e0.Set_Desired(dir);
            }
            return 2;
        case 9:                                 // FIRE_CLOAKED
            unknown_slot252(false);
            break;
        case 10:
            return 1;
        case 0: {                               // FIRE_OK
            if (unknown_5c6 != 0 && unknown_4c0[0] != 0) {
                if (unknown_4c0[0]->Is_Two_Shooter()) {
                    unknown_slot221(unknown_228, 0);
                    unknown_slot221(unknown_228, 1);
                }
                return 1;
            }
            if (Type == Rules->PrismType) {
                BuildingClass *support = 0;
                int support_distance = 0x7FFFFFFF;
                CoordStruct center = unknown_slot42();
                if (unknown_534 < Rules->PrismSupportMax) {
                    int count = House->Buildings.Count();
                    for (int index = 0; index < count; ++index) {
                        BuildingClass *other = House->Buildings[index];
                        if (other == 0 || !other->unknown_7d || other->Type != Rules->PrismType) {
                            continue;
                        }
                        if ((long)other->unknown_244 != 0 || other->unknown_5d8 != 0) {
                            continue;
                        }
                        if (other->Get_Mission() == 1 || other == this) {
                            continue;
                        }
                        CoordStruct where = other->unknown_slot42();
                        double dx = center.X - where.X;
                        double dy = center.Y - where.Y;
                        double dz = center.Z - where.Z;
                        int distance = (int)unknown_sqrt_4ba410(dx * dx + dy * dy + dz * dz);
                        if (distance <= Weapon_Range(1) && (support == 0 || distance < support_distance)) {
                            support = other;
                            support_distance = distance;
                        }
                    }
                    if (support != 0) {
                        ++unknown_534;
                        CoordStruct from = unknown_slot43(0);
                        support->unknown_44b870(2, from);
                        return 1;
                    }
                }
                CoordStruct shot = {};
                shot.X = 0;
                unknown_44b870(1, shot);
                return 1;
            }
            if (Type->IsAnimDelayedFire) {
                CoordStruct shot = {};
                shot.X = weapon;
                unknown_44b870(1, shot);
                return 1;
            }
            unknown_slot221(unknown_228, weapon);
            return 1;
        }
        default:                                // FIRE_BUSY, 7, beyond the table
            break;
        }
        if (unknown_228 != 0) {
            UnknownFacingDir dir = unknown_slot285(unknown_228);
            unknown_2e0.Set_Desired(dir);
        }
        return 1;
    }

    // SAM launcher.
    if (unknown_a8 == 0) {                      // tracking
        if (Type->Powered && Type->PowerDrain_c6c > 0 && House->Power_Fraction() < 1.0) {
            return 1;
        }
        AbstractClass *target = unknown_228;
        bool aircraft = target != 0 && target->Kind_Of() == 2 && ((ObjectClass *)target)->Get_Height() != 0;
        if (!aircraft) {
            unknown_slot220(0);
            unknown_a8 = 0;
            unknown_slot116(5, false);
            Commence();
            return 1;
        }
        if (!unknown_2e0.Is_Rotating()) {
            UnknownFacingDir dir = unknown_slot285(unknown_228);
            UnknownFacingDir current = unknown_2e0.Current();
            int diff = (short)(*(short *)&current - *(short *)&dir);
            if ((diff < 0 ? -diff : diff) <= 0x2000) {
                unknown_a8 = 1;
                return 1;
            }
            UnknownFacingDir desired = unknown_slot285(unknown_228);
            unknown_2e0.Set_Desired(desired);
        }
        return 1;
    }
    if (unknown_a8 == 1) {                      // firing
        AbstractClass *target = unknown_228;
        bool aircraft = target != 0 && target->Kind_Of() == 2 && ((ObjectClass *)target)->Get_Height() != 0;
        if (aircraft) {
            int fire = unknown_slot218(unknown_228, 0, true);
            if (fire == 5 || fire == 6 || fire == 8) {
                unknown_slot220(0);
                unknown_a8 = 0;
                return 1;
            }
            if (fire != 2) {
                if (fire != 0) {
                    return 1;
                }
                unknown_slot221(unknown_228, 0);
                unknown_slot221(unknown_228, 1);
                unknown_a8 = 0;
                return 1;
            }
        } else {
            unknown_slot220(0);
        }
        unknown_a8 = 0;
        return 1;
    }
    return (int)(Control()->unknown_18 * 900.0) + unknown_id_source_a3d290->RandomNumber(0, 2);
}
