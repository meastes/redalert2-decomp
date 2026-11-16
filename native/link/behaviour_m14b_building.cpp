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
        if (TarCom == 0) {
            unknown_slot220(0);
            SupportingPrisms = 0;
            if (IsNotWarpingIn()) {
                Next_Planning_Node();
            }
            if (Get_Mission() != 0x1C) {
                QueueMission(5, false);
                Commence();
            }
            return 1;
        }
        int weapon = What_Weapon_Should_I_Use(TarCom);
        IsReadyToCommence = true;
        unsigned fire = (unsigned)unknown_slot218(TarCom, weapon, true);
        if (fire == 2 && unknown_slot232() && Type->TurretAnimIsVoxel) {
            UnknownFacingDir dir = unknown_slot285(TarCom);
            bool snap = true;
            int rot = Type->ROT;
            if (rot != 0) {
                UnknownFacingDir current = PrimaryFacing.Current();
                int step = (short)(unsigned short)((rot & 0xFF) << 8);
                int diff = (short)(*(short *)&current - *(short *)&dir);
                if ((step < 0 ? -step : step) < (diff < 0 ? -diff : diff)) {
                    snap = false;
                }
            }
            if (snap) {
                PrimaryFacing.Set(dir);
                fire = (unsigned)unknown_slot218(TarCom, weapon, true);
            }
        }
        switch (fire) {
        case 1: case 5: case 6: case 8:         // FIRE_AMMO, FIRE_ILLEGAL, FIRE_CANT, FIRE_RANGE
            unknown_slot220(0);
            SupportingPrisms = 0;
            if (IsNotWarpingIn()) {
                Next_Planning_Node();
            }
            if (Get_Mission() == 0x1C) {
                return 1;
            }
            QueueMission(5, false);
            Commence();
            break;
        case 2: case 3:                         // FIRE_FACING, FIRE_REARM
            if (TarCom != 0) {
                UnknownFacingDir dir = unknown_slot285(TarCom);
                PrimaryFacing.Set_Desired(dir);
            }
            return 2;
        case 9:                                 // FIRE_CLOAKED
            unknown_slot252(false);
            break;
        case 10:
            return 1;
        case 0: {                               // FIRE_OK
            if (UpgradeLevel != 0 && unknown_4c0[0] != 0) {
                if (unknown_4c0[0]->Is_Two_Shooter()) {
                    Fire_At(TarCom, 0);
                    Fire_At(TarCom, 1);
                }
                return 1;
            }
            if (Type == Rules->PrismType) {
                BuildingClass *support = 0;
                int support_distance = 0x7FFFFFFF;
                CoordStruct center = unknown_slot42();
                if (SupportingPrisms < Rules->PrismSupportMax) {
                    int count = House->Buildings.Count();
                    for (int index = 0; index < count; ++index) {
                        BuildingClass *other = House->Buildings[index];
                        if (other == 0 || !other->IsActive || other->Type != Rules->PrismType) {
                            continue;
                        }
                        if ((long)other->Arm != 0 || other->DelayBeforeFiring != 0) {
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
                        ++SupportingPrisms;
                        CoordStruct from = unknown_slot43(0);
                        support->Start_Delayed_Fire(2, from);
                        return 1;
                    }
                }
                CoordStruct shot = {};
                shot.X = 0;
                Start_Delayed_Fire(1, shot);
                return 1;
            }
            if (Type->IsAnimDelayedFire) {
                CoordStruct shot = {};
                shot.X = weapon;
                Start_Delayed_Fire(1, shot);
                return 1;
            }
            Fire_At(TarCom, weapon);
            return 1;
        }
        default:                                // FIRE_BUSY, 7, beyond the table
            break;
        }
        if (TarCom != 0) {
            UnknownFacingDir dir = unknown_slot285(TarCom);
            PrimaryFacing.Set_Desired(dir);
        }
        return 1;
    }

    // SAM launcher.
    if (Status == 0) {                      // tracking
        if (Type->Powered && Type->PowerDrain_c6c > 0 && House->Power_Fraction() < 1.0) {
            return 1;
        }
        AbstractClass *target = TarCom;
        bool aircraft = target != 0 && target->Kind_Of() == 2 && ((ObjectClass *)target)->Get_Height() != 0;
        if (!aircraft) {
            unknown_slot220(0);
            Status = 0;
            QueueMission(5, false);
            Commence();
            return 1;
        }
        if (!PrimaryFacing.Is_Rotating()) {
            UnknownFacingDir dir = unknown_slot285(TarCom);
            UnknownFacingDir current = PrimaryFacing.Current();
            int diff = (short)(*(short *)&current - *(short *)&dir);
            if ((diff < 0 ? -diff : diff) <= 0x2000) {
                Status = 1;
                return 1;
            }
            UnknownFacingDir desired = unknown_slot285(TarCom);
            PrimaryFacing.Set_Desired(desired);
        }
        return 1;
    }
    if (Status == 1) {                      // firing
        AbstractClass *target = TarCom;
        bool aircraft = target != 0 && target->Kind_Of() == 2 && ((ObjectClass *)target)->Get_Height() != 0;
        if (aircraft) {
            int fire = unknown_slot218(TarCom, 0, true);
            if (fire == 5 || fire == 6 || fire == 8) {
                unknown_slot220(0);
                Status = 0;
                return 1;
            }
            if (fire != 2) {
                if (fire != 0) {
                    return 1;
                }
                Fire_At(TarCom, 0);
                Fire_At(TarCom, 1);
                Status = 0;
                return 1;
            }
        } else {
            unknown_slot220(0);
        }
        Status = 0;
        return 1;
    }
    return (int)(Control()->unknown_18 * 900.0) + unknown_id_source_a3d290->RandomNumber(0, 2);
}
