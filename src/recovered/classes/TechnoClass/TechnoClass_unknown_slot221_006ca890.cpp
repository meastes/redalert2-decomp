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
    float Length() const
    {
        double sum = (double)Z * (double)Z;
        sum += (double)X * (double)X + (double)Y * (double)Y;
        float length = (float)unknown_sqrt_4ba410(sum);
        return length;
    }
    float Z;
};
inline int Planar_006ca890(CoordStruct const &c) { float y = (float)c.Y; return (int)unknown_sqrt_4ba410((double)y * (double)y + (double)c.X * (double)c.X); }
extern bool unknown_flag_a40d13;
extern CoordStruct const unknown_coord_none_ac0188;
extern double const unknown_facing_scale_79b628;
extern double const unknown_facing_origin_79b630;
extern double const unknown_tau_79d158;
extern double const unknown_twenty_79d150;
struct UnknownUnitView_006ca890 { char unknown_000[0x5C4]; EBolt *unknown_5c4; };
extern double const unknown_facing_radians_79b620;
extern double const unknown_two_pi_79c9b8;
inline double Random_Double_006ca890(double low, double high)
{
    int sample = unknown_id_source_a3d290->Random()(0, 0x7FFFFFFE);
    return (high - low) * ((double)sample * unknown_random_scale_79c280) + low;
}
inline unsigned short Random_Direction_006ca890()
{
    return (unsigned short)(int)((Random_Double_006ca890(0.0, 1.0) * unknown_two_pi_79c9b8 - unknown_facing_origin_79b630) * unknown_facing_scale_79b628);
}
extern double const unknown_small_7a10f8;
float __stdcall Fast_Atan2_Float_004baba0(float y, float x);
struct AircraftVectorXY;
double __fastcall Aircraft_XY_Length_0041B750(AircraftVectorXY const *vector);
inline double Radian_006ca890(UnknownFacingDir const &dir)
{
    return ((short)dir.Raw - 0x3FFF) * unknown_facing_radians_79b620;
}
inline void Set_Yaw_006ca890(TVelocity3D<double> &velocity, UnknownFacingDir const &yaw)
{
    if (velocity.X == 0.0 && velocity.Y == 0.0) {
        velocity.X = 100.0;
    }
    double horizontal = unknown_sqrt_4ba410(velocity.X * velocity.X + velocity.Y * velocity.Y);
    double angle = Radian_006ca890(yaw);
    velocity.X = unknown_cos_4ba4d0(angle) * horizontal;
    velocity.Y = -(unknown_sin_4ba480(angle) * horizontal);
}
inline void Set_Pitch_006ca890(TVelocity3D<double> &velocity, UnknownFacingDir const &pitch)
{
    double current = Radian_006ca890(UnknownFacingDir((unsigned short)((unknown_math_4ba600(velocity.Z, Aircraft_XY_Length_0041B750((AircraftVectorXY const *)&velocity)) - 1.5707963267948966) * -10430.060040584269)));
    double speed = unknown_sqrt_4ba410((velocity.X * velocity.X + velocity.Y * velocity.Y) + velocity.Z * velocity.Z);
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
inline bool Is_Foot_006ca890(ObjectClass const *object)
{
    if (object == 0) {
        return false;
    }
    unsigned char flag = object->AbstractFlags.IsFoot;
    return flag != 0;
}
inline bool Has_Ability_006ca890(TechnoClass *techno, int ability)
{
    if (techno->unknown_118.unknown_7133a0() || techno->unknown_118.unknown_713420()) {
        TechnoTypeClass *type = techno->unknown_slot32();
        if ((techno->unknown_118.unknown_7133a0() && type->VeteranAbilities.Ability[ability])
            || (techno->unknown_118.unknown_713420() && (type->VeteranAbilities.Ability[ability] || type->EliteAbilities.Ability[ability]))) {
            return true;
        }
    }
    return false;
}
template<class T> inline T MIN_006ca890(T a, T b) { return a < b ? a : b; }
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
// @ref ?unknown_flag_a40d13@@3_NA=00a40d13
// @ref ?Fast_Atan2_Float_004baba0@@=004baba0
// @ref ?unknown_6c9df0@TechnoClass@@=006c9df0
// @ref ?unknown_6ca340@TechnoClass@@=006ca340
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
    if (unknown_flag_a40d13 || target == 0 || (object != 0 && object->unknown_76)) {
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
            if (!Map.Is_Shrouded(Center_Coord()) && !Map.Is_Fogged(Center_Coord())) {
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
    TVelocity3D<double> velocity;
    CoordStruct target_coord;
    if (object != 0) {
        target_coord = object->unknown_slot40();
    } else {
        target_coord = target->unknown_slot22();
    }
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
        if (Has_Ability_006ca890(this, 2)) {
            firepower = (int)(firepower * Rules->VeteranCombat);
        }
    }
    Point2D delta = Point2D(target_coord.X, target_coord.Y) - Point2D(fire_coord.X, fire_coord.Y);
    int distance = (int)Fast_Sqrt((double)(delta.X * delta.X + delta.Y * delta.Y));
    int speed = weapon->Get_Speed(distance);
    BulletClass *bullet = Create_Bullet(weapon->Projectile, target, this, firepower, weapon->Warhead, speed, weapon->unknown_10b);
    if (bullet != 0) {
        bullet->Set_Weapon_Type(weapon);
        bullet->unknown_slot52();
        if (Is_Foot_006ca890(this)) {
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
                int spread = Random_Pick(0, Rules->unknown_139c);
                spread *= (int)length;
                spread /= unknown_slot85(which);
                UnknownFacingDir facing(Random_Direction_006ca890());
                displacement = Move_Coord_006ca890(displacement, facing, spread);
            }
        }
        if (btype->ROT == 0 && !btype->Dropping) {
            double angle = Fast_Atan2((double)-displacement.Y, (double)displacement.X);
            dir = UnknownFacingDir((unsigned short)(int)((angle - unknown_facing_origin_79b630) * unknown_facing_scale_79b628));
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
        BulletTypeClass *projectile = weapon->Projectile;
        if (projectile->ROT > 0 || projectile->Vertical) {
            if (segments == 0) {
                speed = 1;
            }
            bullet->Speed = weapon->unknown_a8;
        }
        double full_speed = (double)speed;
        velocity.Y = 0.0;
        velocity.Z = 0.0;
        velocity.X = 100.0;
        double length = unknown_sqrt_4ba410(velocity.X * velocity.X + velocity.Y * velocity.Y + velocity.Z * velocity.Z);
        double ratio = full_speed / length;
        velocity.X *= ratio;
        velocity.Y *= ratio;
        velocity.Z *= ratio;
        if (segments > 0) {
            double angle = unknown_tau_79d158 / (double)segments * unknown_38c - unknown_facing_origin_79b630;
            UnknownFacingDir current = unknown_2e0.Current();
            UnknownFacingDir facing((unsigned short)(int)((((short)current.Raw - 0x3FFF) * unknown_facing_radians_79b620 + angle - unknown_facing_origin_79b630) * unknown_facing_scale_79b628));
            velocity.Set_Yaw(facing);
            unknown_38c++;
            if (unknown_38c >= segments) {
                unknown_38c = 0;
            }
        } else {
            velocity.Set_Yaw(dir);
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
                TechnoClass *tarcom = (TechnoClass *)unknown_228;
                BuildingClass *building = (BuildingClass *)tarcom;
                int kind = building != 0 ? building->Kind_Of() : 0;
                if (kind == 6) {
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
            int anim_count = weapon->Anim.Count();
            if (anim_count == 8) {
                anim_type = weapon->Anim[(Dir_To_8(unknown_slot183()) + 1) % 8];
            } else if (anim_count > 0) {
                anim_type = weapon->Anim[0];
            }
            if (weapon->unknown_bc.Count() > 0) {
                int voc = weapon->unknown_bc.Pick((unsigned short)unknown_320);
                Voc_Play_At(voc, fire_coord, 0);
            }
            if (anim_type != 0) {
                AnimClass *anim = new AnimClass(anim_type, fire_coord, 0, 1, 0x600, 0);
                if (Kind_Of() == 6) {
                    int dy = (fire_coord - unknown_slot42()).Y;
                    int zadjust = dy / -4;
                    anim->ZAdjust = MIN_006ca890(zadjust, 0);
                    BuildingClass *building = static_cast<BuildingClass *>(this);
                    if (building->unknown_4532f0() > 0) {
                        anim->ZAdjust = -200;
                        building->unknown_56c++;
                        building->unknown_56c %= building->unknown_4532f0();
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
                    BuildingClass *building = static_cast<BuildingClass *>(this);
                    if (laser != 0 && building->Type == Rules->PrismType) {
                        laser->unknown_18 = 3;
                        if (building->unknown_534 > 0) {
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
                WarheadTypeClass *warhead = weapon->Warhead;
                if (warhead == 0 || !warhead->Temporal) {
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
                || ((Map.Is_Shrouded(Center_Coord()) || Map.Is_Fogged(Center_Coord())) && !(Kind_Of() == 2 && unknown_372))) {
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
            HouseClass *house = Owning_House();
            if (house->Unknown_004F52B0() && target->Kind_Of() == 15) {
                unknown_381 = true;
            }
        }
        if (unknown_slot32()->RejoinTeamIfLimboed && this != 0 && AbstractFlags.IsFoot && static_cast<FootClass *>(this)->Team != 0 && target->Kind_Of() == 15) {
            unknown_384 = (this != 0 && AbstractFlags.IsFoot ? static_cast<FootClass *>(this) : 0)->Team;
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
