// NON-EXACT DRAFT 0x6CA890 FUN_006ca890
// entry 0x6CA890; exact-match score 99.14 (misses_index best); plan "draft as-is"
// origin lane: Scout708 (build/class-work/Scout708/w/keep/fm_b_7834_s9926.cpp)
// Recovered source text by our lanes/workers; never exact-match credit. Tracked by
// Scout1069 (native/drafts.json). Not original bytes.
// Native adaptation (Scout1069): unknown_slot71 renamed Mark (slot 71, object.hpp); each changed line is marked NATIVE.
// BulletClass *TechnoClass::unknown_slot221(AbstractClass *target, int which) (0x006CA890)
// TibSun TechnoClass::Fire_At.
#include "shared/techno.hpp"
#include "shared/foot.hpp"
#include "shared/bullet.hpp"
#include "shared/bullet_type.hpp"
#include "shared/weapon_type.hpp"
#include "shared/warhead_type.hpp"
#include "shared/techno_type.hpp"
#include "shared/spawn_manager.hpp"
#include "shared/particle_system.hpp"
#include "shared/anim.hpp"
#include "shared/wave.hpp"
#include "shared/ebolt.hpp"
#include "shared/map_view.hpp"
#include "shared/house.hpp"
#include "shared/rules.hpp"
#include "shared/team.hpp"
#include "shared/fast_math.hpp"
#include "shared/building_class.hpp"
#include "shared/laser_draw.hpp"
#include "shared/sound_play.hpp"
#include "shared/id_source.hpp"
#include "shared/vector3.hpp"
inline int Length_006ca890(CoordStruct const &c) { return (int)unknown_sqrt_4ba410((double)c.X * (double)c.X + ((double)c.Z * (double)c.Z + (double)c.Y * (double)c.Y)); }
struct Point2F_006ca890 { Point2F_006ca890() {} Point2F_006ca890(float x, float y) : X(x), Y(y) {} float X; float Y; };
struct Point3F_006ca890 : public Point2F_006ca890 {
    Point3F_006ca890(float x, float y, float z) : Point2F_006ca890(x, y), Z(z) {}
    float Length() const { return (float)unknown_sqrt_4ba410((double)Z * (double)Z + (double)X * (double)X + (double)Y * (double)Y); }
    float Z;
};
inline int Planar_006ca890(CoordStruct const &c) { double y = c.Y; return (int)unknown_sqrt_4ba410(y * y + (double)c.X * (double)c.X); }
extern bool Debug_Map;
extern CoordStruct const unknown_coord_none_ac0188;
extern double const unknown_facing_scale_79b628;
extern double const unknown_facing_origin_79b630;
extern double const unknown_tau_79d158;
extern double const unknown_twenty_79d150;
struct UnknownUnitView_006ca890 { char unknown_000[0x5C4]; EBolt *unknown_5c4; };
extern double const unknown_facing_radians_79b620;
extern double const unknown_random_unit_79c280;
extern double const unknown_two_pi_79c9b8;
inline unsigned short Random_Direction_006ca890()
{
    return (unsigned short)(int)(((double)unknown_id_source_a3d290->Random()(0, 0x7FFFFFFE) * unknown_random_unit_79c280 * unknown_two_pi_79c9b8 - unknown_facing_origin_79b630) * unknown_facing_scale_79b628);
}
extern double const unknown_small_7a10f8;
float __stdcall Fast_Atan2_Float_004baba0(float y, float x);
struct AircraftVectorXY;
double __fastcall Aircraft_XY_Length_0041B750(AircraftVectorXY const *vector);
inline double Radian_006ca890(UnknownFacingDir const &dir)
{
    return ((short)dir.Raw - 0x3FFF) * unknown_facing_radians_79b620;
}
inline void Set_Yaw_006ca890(BulletVelocity &velocity, UnknownFacingDir const &yaw)
{
    if (velocity.X == 0.0 && velocity.Y == 0.0) {
        velocity.X = 100.0;
    }
    double horizontal = unknown_sqrt_4ba410(velocity.X * velocity.X + velocity.Y * velocity.Y);
    double angle = Radian_006ca890(yaw);
    velocity.X = unknown_cos_4ba4d0(angle) * horizontal;
    velocity.Y = -(unknown_sin_4ba480(angle) * horizontal);
}
inline void Set_Pitch_006ca890(BulletVelocity &velocity, UnknownFacingDir const &pitch)
{
    double current = Radian_006ca890(UnknownFacingDir((unsigned short)((unknown_math_4ba600(velocity.Z, Aircraft_XY_Length_0041B750((AircraftVectorXY const *)&velocity)) - 1.5707963267948966) * -10430.060040584269)));
    double speed = velocity.Speed();
    if (current != unknown_double_79b610) {
        velocity.X /= unknown_cos_4ba4d0(current);
        velocity.Y /= unknown_cos_4ba4d0(current);
    }
    velocity.X *= unknown_cos_4ba4d0(Radian_006ca890(pitch));
    velocity.Y *= unknown_cos_4ba4d0(Radian_006ca890(pitch));
    velocity.Z = speed * unknown_sin_4ba480(Radian_006ca890(pitch));
}
inline CoordStruct Move_Coord_006ca890(CoordStruct const &start, UnknownFacingDir const &dir, int distance)
{
    double radians = Radian_006ca890(dir);
    return CoordStruct((int)(start.X + unknown_cos_4ba4d0(radians) * distance), (int)(start.Y - unknown_sin_4ba480(radians) * distance), start.Z);
}
inline void Recoil_Start_006ca890(UnknownTechnoBlock &block)
{
    if (block.unknown_00 != 0) {
        block.unknown_18 = 1;
        block.unknown_1c = block.unknown_04;
        if (block.unknown_04 < 1) {
            block.unknown_1c = 1;
        }
        block.unknown_10 = (float)block.unknown_00 / (float)block.unknown_1c;
    }
}

// @ref ?Aircraft_XY_Length_0041B750@@YINPBUAircraftVectorXY@@@Z=0041b750
// @ref ?unknown_small_7a10f8@@=007a10f8
// @ref ?Fast_Atan2_Float_004baba0@@=004baba0
// @ref ?unknown_6c9df0@TechnoClass@@=006c9df0
// @ref ?unknown_6ca340@TechnoClass@@=006ca340
// @ref ?unknown_random_unit_79c280@@=0079c280
// @ref ?unknown_two_pi_79c9b8@@=0079c9b8
// @ref ?unknown_tau_79d158@@=0079d158
// @ref ?unknown_twenty_79d150@@=0079d150
// @ref ?unknown_coord_none_ac0188@@=00ac0188

BulletClass *TechnoClass::unknown_slot221(AbstractClass *target, int which)
{
    WeaponTypeClass *weapon = unknown_slot231(which)->WeaponType;
    if (weapon == 0) {
        return 0;
    }
    ObjectClass *object = target == 0 ? 0 : (target->AbstractFlags.IsObject ? static_cast<ObjectClass *>(target) : 0);
    BulletTypeClass *btype = weapon->Projectile;
    if (Debug_Map || target == 0 || (object != 0 && object->unknown_76)) {
        return 0;
    }
    if (weapon->unknown_11c) {
        int damage = unknown_slot32()->MaxStrength;
        unknown_slot86(&damage, 0, Rules->unknown_c74, 0, true, false);
        return 0;
    }
    if ((weapon->unknown_105 && unknown_25c[0] != 0) || (weapon->unknown_109 && unknown_25c[4] != 0)
        || (weapon->unknown_106 && unknown_25c[1] != 0) || (weapon->unknown_10c && unknown_27c != 0)) {
        return 0;
    }
    if (weapon->unknown_10d) {
        SpawnManager->Set_Target(target);
        if (unknown_372 || unknown_373) {
            if (!Map.Unknown_00568140(Center_Coord()) && !Map.Unknown_005683C0(Center_Coord())) {
                return 0;
            }
            if (Kind_Of() == 2 && unknown_372) {
                return 0;
            }
        }
        if (object != 0) {
            HouseClass *owner = object->Owning_House();
            if (owner != 0 && owner->Unknown_004F52B0() && weapon->unknown_113) {
                Map.Unknown_00549C50(Center_Coord(), 3, owner, false, false, false, true, false);
            }
            Map.Unknown_0054A550(Center_Coord(), 0, 4, 0);
        }
        return 0;
    }
    BulletVelocity velocity;
    CoordStruct target_coord = object != 0 ? object->unknown_slot40() : target->unknown_slot22();
    CoordStruct fire_coord = unknown_slot43(which);
    UnknownFacingDir dir;
    if (btype->ROT == 0 && !btype->Dropping) {
        dir = UnknownFacingDir((unsigned short)(int)((Fast_Atan2((double)fire_coord.Y - (double)target_coord.Y, (double)target_coord.X - (double)fire_coord.X) - unknown_facing_origin_79b630) * unknown_facing_scale_79b628));
    } else {
        dir = unknown_slot183();
        if (btype->Dropping) {
            fire_coord = Center_Coord();
        }
    }
    int attack = weapon->unknown_a4;
    int firepower = attack;
    if (weapon->unknown_10c || weapon->unknown_105) {
        firepower = 0;
    }
    if (firepower > 0) {
        firepower = (int)(House->unknown_d0 * unknown_128 * attack);
        if (unknown_118.unknown_7133a0() || unknown_118.unknown_713420()) {
            TechnoTypeClass *type = unknown_slot32();
            if ((unknown_118.unknown_7133a0() && type->VeteranAbilities.Ability[2]) || (unknown_118.unknown_713420() && (type->VeteranAbilities.Ability[2] || type->EliteAbilities.Ability[2]))) {
                firepower = (int)(firepower * Rules->VeteranCombat);
            }
        }
    }
    int dx = target_coord.X - fire_coord.X;
    int dy = target_coord.Y - fire_coord.Y;
    int speed = weapon->unknown_func_733f70((int)Fast_Sqrt((double)(dy * dy + dx * dx)));
    BulletClass *bullet = Create_Bullet(weapon->Projectile, target, this, firepower, weapon->Warhead, speed, weapon->unknown_10b);
    if (bullet != 0) {
        bullet->Set_Weapon_Type(weapon);
        bullet->unknown_slot52();
        bool is_foot;
        if (this != 0) {
            is_foot = AbstractFlags.IsFoot;
        }
        if (this != 0 && is_foot) {
            if (static_cast<FootClass *>(this)->Locomotor.Checked()->Is_Moving() && !unknown_slot32()->JumpJet) {
                bullet->unknown_a0 = true;
            }
        }
        if (!bullet->unknown_a0 && !btype->Inaccurate) {
            TechnoClass *techno = unknown_228 == 0 ? 0 : (unknown_228->AbstractFlags.IsTechno ? static_cast<TechnoClass *>(unknown_228) : 0);
            WeaponTypeClass *bullet_weapon = bullet->Get_Weapon_Type();
            if (techno != 0 && bullet_weapon != 0) {
                techno->unknown_70 -= unknown_6ca6c0(techno, bullet_weapon);
            }
        }
        CoordStruct displacement;
        displacement = unknown_6d65b0() - fire_coord;
        if (btype->Inaccurate && btype->Arcing) {
            if (!btype->FlakScatter || btype->Inviso) {
                int spread = unknown_id_source_a3d290->Random()(Rules->unknown_139c / 2, Rules->unknown_139c);
                UnknownFacingDir facing(Random_Direction_006ca890());
                displacement = Move_Coord_006ca890(displacement, facing, spread);
            } else {
                float length = Point3F_006ca890((float)displacement.X, (float)displacement.Y, (float)displacement.Z).Length();
                int spread = unknown_id_source_a3d290->Random()(0, Rules->unknown_139c) * (int)length / unknown_slot85(which);
                UnknownFacingDir facing(Random_Direction_006ca890());
                displacement = Move_Coord_006ca890(displacement, facing, spread);
            }
        }
        if (btype->ROT == 0 && !btype->Dropping) {
            dir = UnknownFacingDir((unsigned short)(int)((Fast_Atan2((double)-displacement.Y, (double)displacement.X) - unknown_facing_origin_79b630) * unknown_facing_scale_79b628));
        } else {
            dir = unknown_slot183();
            if (btype->Dropping) {
                fire_coord = Center_Coord();
            }
        }
        if (speed > Length_006ca890(displacement) / 2) {
            speed = Length_006ca890(displacement) / 2;
        }
        int segments = unknown_slot32()->RadialFireSegments;
        if (weapon->Projectile->ROT > 0 || weapon->Projectile->Vertical) {
            if (segments == 0) {
                speed = 1;
            }
            bullet->Speed = weapon->unknown_a8;
        }
        double full_speed = (double)speed;
        velocity.Y = 0.0;
        velocity.Z = 0.0;
        velocity.X = 100.0;
        double ratio = full_speed / unknown_sqrt_4ba410(velocity.X * velocity.X + velocity.Y * velocity.Y + velocity.Z * velocity.Z);
        velocity.X *= ratio;
        velocity.Y *= ratio;
        velocity.Z *= ratio;
        if (segments > 0) {
            double angle = unknown_tau_79d158 / (double)segments * unknown_38c - unknown_facing_origin_79b630;
            UnknownFacingDir current = unknown_2e0.Current();
            UnknownFacingDir facing((unsigned short)(int)((((short)current.Raw - 0x3FFF) * unknown_facing_radians_79b620 + angle - unknown_facing_origin_79b630) * unknown_facing_scale_79b628));
            Set_Yaw_006ca890(velocity, facing);
            unknown_38c++;
            if (unknown_38c >= segments) {
                unknown_38c = 0;
            }
        } else {
            Set_Yaw_006ca890(velocity, dir);
        }
        int height = displacement.Z;
        UnknownFacingDir pitch(0x3FFF);
        bool valid = true;
        if (bullet->Type->Arcing) {
            double gravity = (double)Rules->unknown_1320;
            if (bullet->Type->Floater) {
                gravity = Unknown_0047FCD0();
            }
            int planar = Planar_006ca890(displacement);
            valid = Unknown_0047F8B0(unknown_6d7d20(which), speed, planar, height, gravity, &pitch);
        } else if (bullet->Type->unknown_206) {
            if (target->Center_Coord().Z < Center_Coord().Z) {
                pitch = UnknownFacingDir(0x8000);
            } else {
                pitch = UnknownFacingDir(0x4000);
            }
        } else {
            int abs_z = abs(height);
            if (abs_z > 200) {
                double y = 20.0;
                BuildingClass *building = (BuildingClass *)unknown_228;
                if (building != 0 && building->Kind_Of() == 6) {
                    CoordStruct turret = unknown_slot181(which);
                    height = building->Type->Height * 200 - turret.Z;
                    abs_z = abs(height);
                    if ((double)abs_z < unknown_twenty_79d150) {
                        y = 0.0;
                    }
                }
                double planar = (double)Planar_006ca890(displacement);
                if (planar < unknown_small_7a10f8) {
                    planar = unknown_small_7a10f8;
                }
                double angle = Fast_Atan2_Float_004baba0((float)((double)abs_z - y), (float)planar);
                if (height < 0) {
                    angle = -angle;
                }
                pitch = UnknownFacingDir((unsigned short)(int)((angle - unknown_facing_origin_79b630) * unknown_facing_scale_79b628));
            }
        }
        Set_Pitch_006ca890(velocity, pitch);
        if (valid) {
          if (!bullet->unknown_slot118(fire_coord, velocity)) {
            bullet->Release();
            bullet = 0;
          } else {
            if (bullet->Type->Inviso && object != 0 && object->unknown_79) {
                bullet->unknown_79 = true;
            }
            if (unknown_slot232() && unknown_slot32()->TurretRecoil) {
                Recoil_Start_006ca890(unknown_330);
                Recoil_Start_006ca890(unknown_350);
            }
            if (weapon->unknown_105 && unknown_25c[0] == 0) {
                unknown_25c[0] = new ParticleSystemClass(weapon->unknown_f8, fire_coord, target, this, unknown_coord_none_ac0188);
            }
            if (weapon->unknown_106 && unknown_25c[1] == 0) {
                unknown_25c[1] = new ParticleSystemClass(weapon->unknown_f8, fire_coord, target, this, unknown_coord_none_ac0188);
            }
            if (weapon->unknown_109 && unknown_25c[4] == 0) {
                CoordStruct end = Railgun_Beam_Damage(fire_coord, target, weapon);
                unknown_25c[4] = new ParticleSystemClass(weapon->unknown_f8, fire_coord, 0, this, end);
            }
            unknown_310++;
            int delay = unknown_slot187(which);
            unknown_250 = delay;
            unknown_244 = delay;
            unknown_310 %= weapon->Burst;
            AnimTypeClass *anim_type = 0;
            if (weapon->Anim.Count() == 8) {
                anim_type = weapon->Anim[(Dir_To_8(unknown_slot183()) + 1) % 8];
            } else if (weapon->Anim.Count() > 0) {
                anim_type = weapon->Anim[0];
            }
            if (weapon->unknown_bc.Count() > 0) {
                Voc_Play_At(weapon->unknown_bc[(unsigned short)unknown_320 % (unsigned)weapon->unknown_bc.Count()], fire_coord, 0);
            }
            if (anim_type != 0) {
                AnimClass *anim = new AnimClass(anim_type, fire_coord, 0, 1, 0x600, 0);
                if (Kind_Of() == 6) {
                    int zadjust = -((fire_coord.Y - unknown_slot42().Y) / 4);
                    anim->ZAdjust = zadjust < 0 ? zadjust : 0;
                    if (static_cast<BuildingClass *>(this)->unknown_4532f0() > 0) {
                        anim->ZAdjust = -200;
                        static_cast<BuildingClass *>(this)->unknown_56c++;
                        static_cast<BuildingClass *>(this)->unknown_56c %= static_cast<BuildingClass *>(this)->unknown_4532f0();
                    }
                }
                if (anim != 0 && Kind_Of() != 6) {
                    anim->Attach_To(this);
                }
            }
            if (weapon->unknown_10c) {
                unknown_27c = new WaveClass(fire_coord, target_coord, this, 0, (ObjectClass *)target);
            }
            if (unknown_slot32()->TargetLaser && House->Unknown_004F52B0()) {
                unknown_314 = 15;
            }
            if (weapon->unknown_121) {
                if (Kind_Of() == 6) {
                    WeaponTypeClass *primary = unknown_slot230()->WeaponType;
                    LaserDrawClass *laser = unknown_6c9df0(target, which, primary, unknown_coord_none_ac0188);
                    if (laser != 0 && static_cast<BuildingClass *>(this)->Type == Rules->PrismType) {
                        laser->unknown_18 = 3;
                        if (static_cast<BuildingClass *>(this)->unknown_534 > 0) {
                            laser->unknown_1d = true;
                            laser->unknown_18 = 5;
                        }
                    }
                } else {
                    LaserDrawClass *laser = unknown_6c9df0(target, which, weapon, unknown_coord_none_ac0188);
                    if (laser != 0 && weapon->unknown_123) {
                        laser->unknown_18 = 2;
                    }
                }
            } else if (weapon->unknown_127) {
                int index = unknown_slot174(target);
                WeaponTypeClass *bolt_weapon = unknown_slot231(index)->WeaponType;
                CoordStruct source = unknown_slot43(index);
                EBolt *bolt = unknown_6ca020(target, bolt_weapon, source);
                if (bolt != 0 && Kind_Of() == 1 && unknown_7d && reinterpret_cast<UnknownUnitView_006ca890 *>(this)->unknown_5c4 == 0) {
                    reinterpret_cast<UnknownUnitView_006ca890 *>(this)->unknown_5c4 = bolt;
                    bolt->Attach_004B2580(this, index);
                }
            } else if (weapon->unknown_12a) {
                if (weapon->Warhead == 0 || !weapon->Warhead->Temporal) {
                    unknown_6ca1d0(target, 0);
                } else {
                    unknown_6ca1d0(target, 1);
                }
            } else if (weapon->unknown_12b) {
                unknown_6ca340((int)weapon->Warhead->CellSpread);
            }
            unknown_slot207();
            Mark(2);
            if (!(unknown_372 || unknown_373)
                || ((Map.Unknown_00568140(Center_Coord()) || Map.Unknown_005683C0(Center_Coord())) && !(Kind_Of() == 2 && unknown_372))) {
                if (target->AbstractFlags.IsObject) {
                    HouseClass *owner = static_cast<ObjectClass *>(target)->Owning_House();
                    if (owner != 0 && owner->Unknown_004F52B0() && weapon->unknown_113) {
                        Map.Unknown_00549C50(Center_Coord(), 3, owner, false, false, false, true, false);
                        Map.Unknown_0054A550(Center_Coord(), 0, 4, 0);
                    }
                }
            }
          }
        } else {
            bullet->Release();
            bullet = 0;
        }
    }
    if (weapon->unknown_10e) {
        if (unknown_slot32()->ReselectIfLimboed && unknown_77) {
            if (Owning_House()->Unknown_004F52B0() && target->Kind_Of() == 15) {
                unknown_381 = true;
            }
        }
        if (unknown_slot32()->RejoinTeamIfLimboed && this != 0 && AbstractFlags.IsFoot && static_cast<FootClass *>(this)->Team != 0 && target->Kind_Of() == 15) {
            unknown_384 = (AbstractFlags.IsFoot ? static_cast<FootClass *>(this) : 0)->Team;
        }
        unknown_slot52();
        if (weapon->Warhead->Parasite) {
            if (target->AbstractFlags.IsFoot) {
                static_cast<FootClass *>(target)->unknown_584 = Frame + 20;
            }
            bullet->unknown_slot52();
            bullet->Construct(weapon->Projectile, target, this, firepower, weapon->Warhead, weapon->unknown_a8, weapon->unknown_10b);
            bullet->unknown_slot118(fire_coord, velocity);
        }
    }
    if (unknown_slot32()->DistributedFire && (Mission != 1 || Kind_Of() == 6)) {
        unknown_3c0.Add(target);
        unknown_slot220(0);
    }
    if (weapon->unknown_111) {
        if (this != 0 && AbstractFlags.IsFoot && static_cast<FootClass *>(this)->Team != 0) {
            static_cast<FootClass *>(this)->Team->unknown_func_6b79e0(0);
            static_cast<FootClass *>(this)->Team->StepCompleted = true;
        }
        unknown_slot220(0);
    }
    return bullet;
}
