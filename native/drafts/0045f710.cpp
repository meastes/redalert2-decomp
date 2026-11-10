// NON-EXACT DRAFT 0x45F710 BulletClass::AI (slot 23)
// entry 0x45F710; exact-match score 87.78 (misses_index best)
// origin lane: Scout949 (build/class-work/Scout949/w/y1.cpp)
// Recovered source text by our lanes/workers; never exact-match credit. Tracked by
// Scout1121 (native/drafts.json). Not original bytes.
// BulletClass::AI 0x0045F710 (slot 23), draft.
// NATIVE (Scout1121): the lane overlay's guards (Scout949 pre3.h; its velocity
// constructor guard is in shared/bullet_velocity.hpp now) and its declarations
// (h3/shared/bullet.hpp tail), mapped onto native names below.
#define RA2_BUILDING_LIGHT_DISTANCE
#define RA2_COORD_TPOINT3D
#define RA2_TPOINT3D_DIFFERENCE
#include "shared/bullet.hpp"
#include "shared/bullet_type.hpp"
#include "shared/warhead_type.hpp"
#include "shared/building_class.hpp"
#include "shared/building_type_class.hpp"
#include "shared/techno.hpp"
#include "shared/techno_type.hpp"
#include "shared/house.hpp"
#include "shared/anim.hpp"
#include "shared/anim_type.hpp"
#include "shared/rules.hpp"
#include "shared/cell.hpp"
#include "shared/map_view.hpp"
#include "shared/fast_math.hpp"
#include "shared/unknown_vector3.hpp"
#include "shared/unknown_geometry.hpp"
#include "shared/matrix3d.hpp"
#include "shared/vector3.hpp"
#include "shared/tactical.hpp"
#include "shared/weapon_type.hpp"
#include "shared/lightning_storm.hpp"
#include "shared/radar_event.hpp"

// NATIVE: 0x00850310 is unknown_geometry.hpp's header static (0, 0, 0).
#define unknown_coord_none_850310 unknown_geometry_l
// NATIVE: Projectile_Motion 0x00593870 is the Scout914 draft (native/drafts/00593870.cpp,
// its own TibSun-shaped types, layout-identical): reached through a C adapter.
extern "C" int ra2_native_projectile_motion_593870(void *coord, void *velocity, void const *target, void const *rot,
                                                   bool aircraft, bool airburst, bool very_high, bool level);
inline int Projectile_Motion_00593870(CoordStruct &coord, TVelocity3D<double> &velocity, CoordStruct const &target,
                                      UnknownDirStruct const &rot, bool aircraft, bool airburst, bool very_high, bool level)
{
    return ra2_native_projectile_motion_593870(&coord, &velocity, &target, &rot, aircraft, airburst, very_high, level);
}
// NATIVE: the slope matrix 0x00718D60 as shared/locomotion.hpp declares it (fills and returns a matrix).
Matrix3D * __fastcall Unknown_00718D60(Matrix3D *matrix, int ramp);
inline Matrix3D Slope_Matrix_00718d60(int ramp)
{
    Matrix3D matrix;
    Unknown_00718D60(&matrix, ramp);
    return matrix;
}

// @ref ?unknown_coord_none_850310@@3UCoordStruct@@A=00850310
// @ref ?Projectile_Motion_00593870@@YIHAAUCoordStruct@@AAU?$TVelocity3D@N@@ABU1@ABUUnknownDirStruct@@_N444@Z=00593870
// @ref ?Slope_Matrix_00718d60@@YI?AVMatrix3D@@H@Z=00718d60
// @ref ?Get_Cell_Ramp_006a7110@Tactical@@QAEHABUCoordStruct@@@Z=006a7110
// @ref ?Proximity_004ceee0@UnknownBulletData@@QAEHABUCoordStruct@@@Z=004ceee0
// @ref ??_C@_04JOHA@NUKE?$AA@=007d1238
// @ref ??_C@_08KHCJ@NUKEBALL?$AA@=007d122c
// @ref __real@8@3ffb8888888888888800=0079d570
// @ref __real@8@3ffefbbbbbbbbbbbb800=0079d560
// @ref __real@8@40069600000000000000=0079d550
inline int Vector_Length_lab(UnknownVector3 vector)
{
    double y = vector.Y;
    double z = vector.Z;
    double x = vector.X;
    return (int)Fast_Sqrt(x * x + z * z + y * y);
}

inline int Coord_Length_lab(CoordStruct const &a, CoordStruct const &b)
{
    return Vector_Length_lab(UnknownVector3(a.X - b.X, a.Y - b.Y, a.Z - b.Z));
}

inline int Coord_Distance_lab(CoordStruct const &a, CoordStruct const &b)
{
    UnknownVector3 d;
    d.X = a.X - b.X;
    d.Y = a.Y - b.Y;
    d.Z = a.Z - b.Z;
    return d.Length();
}

void BulletClass::AI()
{
    ObjectClass::AI();
    if (!unknown_7d) {
        return;
    }
    if (SpawnNextAnim) {
        if (NextAnim != 0) {
            return;
        }
        unknown_abstracts_ac0bf8.Delete(this);
        SpawnNextAnim = false;
        unknown_func_461de0(false);
        unknown_slot60();
        return;
    }

    bool collided = false;
    bool forced = false;
    if (Type->Dropping && !unknown_7a) {
        forced = true;
    }
    if (Type->AnimLow || Type->AnimHigh) {
        if (--AnimRateCounter == 0) {
            AnimRateCounter = Type->AnimRate;
            AnimFrame++;
            if (AnimFrame > Type->AnimHigh) {
                AnimFrame = Type->AnimLow;
            }
        }
    }
    CoordStruct coord = Coord;
    if (Type->Trailer != 0) {
        if (Type->unknown_2b8 != 0) {
            if (Frame % Type->unknown_2b8 == 0) {
                new AnimClass(Type->Trailer, coord, 1, 1, 0x600, 0);
            }
        } else if (Frame % Type->SpawnDelay == 0) {
            new AnimClass(Type->Trailer, coord, 1, 1, 0x600, 0);
        }
    }

    int impact = 0;
    TVelocity3D<double> velocity;
    if (Type->ROT > 0) {
        int max_speed = Speed;
        double speed = Velocity.Speed();
        if (Type->CourseLockDuration == 0) {
            if (Speed >= 40 || speed + 0.5 >= max_speed) {
                unknown_ed = false;
            }
        } else if (unknown_f0 < Type->CourseLockDuration) {
            if (++unknown_f0 >= Type->CourseLockDuration) {
                unknown_ed = false;
            }
        }
        int acceleration = Type->Acceleration;
        if (unknown_ed && Type->CourseLockDuration == 0) {
            acceleration = Frame % 2 == 0;
        }
        if (speed < max_speed) {
            speed += acceleration;
            if (speed >= max_speed) {
                speed = max_speed;
            }
            Velocity.Set_Speed(speed);
        } else if (speed > max_speed) {
            speed -= acceleration / 2;
            if (speed <= 0.0) {
                speed = 0;
            }
            Velocity.Set_Speed(speed);
        }
        
        TVelocity3D<double> velocity = Velocity;
        CoordStruct target_coord = Target != 0 ? Target->unknown_slot22() : unknown_coord_none_850310;
        if (Target != 0 && Target->AbstractFlags.IsObject) {
            target_coord = ((ObjectClass *)Target)->unknown_slot40();
        }
        double &rotvar = Rules->MissileROTVar;
        double phase = ((Fetch_ID() + Frame) % 15) * (1.0 / 15);
        int rot = (int)((Fast_Sin(phase * 6.283185307179586) * rotvar + (rotvar + 1.0)) * Type->ROT);
        if (Coord_Length_lab(Center_Coord(), target_coord) < 256) {
            rot = (int)(rot * 1.5);
        }
        CoordStruct old_coord = coord;
        int aircraft = false;
        if (Target != 0 && Target->Kind_Of() == 2) {
            aircraft = true;
        }
        UnknownDirStruct dir((unsigned short)((unknown_ed ? 0 : rot) << 8));
        int distance = Projectile_Motion_00593870(coord, velocity, target_coord, dir, (bool &)aircraft, Type->Airburst, Type->VeryHigh, Type->Level);
        CellClass *cell = &Map[coord];
        TVelocity3D<double> new_velocity = velocity;
        Velocity = new_velocity;
        if (distance <= Velocity.Speed() * 0.5 || unknown_slot108() <= 0) {
            forced = true;
            impact = 1;
            if (unknown_slot108() > 0 && !Type->Airburst) {
                if (target_coord != unknown_coord_none_850310) {
                    coord = target_coord;
                }
            }
        }
        if (target_coord == unknown_coord_none_850310) {
            if (unknown_slot108() >= Rules->MissileSafetyAltitude) {
                forced = true;
                impact = 1;
            }
        }
        int delta = Coord_Distance_lab(old_coord, target_coord) - Coord_Distance_lab(coord, target_coord);
        if (!unknown_ed) {
            if (unknown_100 < 60) {
                unknown_100++;
                unknown_108 += delta;
            } else {
                unknown_108 = unknown_108 * (59.0 / 60) + delta;
                if (unknown_108 >= 0 && unknown_108 < 60.0 && !Type->Airburst && !Type->VeryHigh) {
                    forced = true;
                    impact = 1;
                    goto flown;
                }
            }
        }
        if (impact == 0 && ((cell->unknown_140 & 0x100) || (Map[old_coord].unknown_140 & 0x100))) {
            int bridge_height = Map.Get_Height_GL(coord) + unknown_geometry_j;
            if (coord.Z > bridge_height && old_coord.Z < bridge_height) {
                impact = 1;
                coord.Z = bridge_height;
                forced = true;
            } else if (coord.Z < bridge_height && old_coord.Z > bridge_height) {
                impact = 1;
                coord.Z = bridge_height;
                forced = true;
            }
        }
    } else {
        TVelocity3D<double> position = Velocity;
        TVelocity3D<double> new_velocity = position;
        velocity = TVelocity3D<double>(position.X, position.Y, position.Z);
        if (position.Speed() < 8.0) {
            impact = 1;
        }
        position.X = coord.X;
        position.Y = coord.Y;
        position.Z = coord.Z;
        double gravity = Rules->unknown_1320;
        if (Type->Floater) {
            gravity = Unknown_0047FCD0();
        }
        if (Type->Vertical) {
            int current = (int)Velocity.Speed();
            if (current < Speed) {
                Velocity.Set_Speed(Type->Acceleration + current);
            }
            TVelocity3D<double> step = Velocity;
            CoordStruct delta((int)step.X, (int)step.Y, (int)step.Z);
            CoordStruct old_coord = coord;
            coord = CoordStruct(coord.X + delta.X, coord.Y + delta.Y, coord.Z + delta.Z);
            if (coord.Z > Type->DetonationAltitude) {
                impact = 1;
                forced = true;
            } else if (unknown_slot108() < 0) {
                impact = 1;
                forced = true;
            } else {
                int bridge_height = Map.Get_Height_GL(coord) + unknown_geometry_j;
                if ((Map[coord].unknown_140 & 0x100) || (Map[old_coord].unknown_140 & 0x100)) {
                    if (coord.Z >= bridge_height) {
                        if (old_coord.Z < bridge_height) {
                            impact = 1;
                            forced = true;
                        }
                    } else if (old_coord.Z >= bridge_height) {
                        impact = 1;
                        forced = true;
                    }
                }
            }
        } else {
            TechnoClass *payback = Owner;
            double elasticity = Type->Elasticity;
            velocity.Z -= gravity;
            UnknownVector3 old_position((int)position.X, (int)position.Y, (int)position.Z);
            position += velocity;
            CoordStruct new_coord;
            new_coord.Set_00434820((int)position.X, (int)position.Y, (int)position.Z);
            int height = Map.Get_Height_GL(new_coord);
            int bridge_height = height + unknown_geometry_j;
            CellClass *cell = &Map[new_coord];
            bool fell = false;
            bool rose = false;
            if ((cell->unknown_140 & 0x100) || (Map[*(CoordStruct *)&old_position].unknown_140 & 0x100)) {
                if (new_coord.Z >= bridge_height) {
                    if (old_position.Z < bridge_height) {
                        rose = true;
                    }
                } else if (old_position.Z >= bridge_height) {
                    fell = true;
                }
            }
            bool hit = false;
            if (!fell && !rose && position.Z >= height && position.Z - 150.0 < height) {
                BuildingClass *building = (BuildingClass *)cell->Cell_Building();
                if (building != 0 || cell->Has_Wall_Or_Gate(-1, -1)) {
                    hit = true;
                    if (building != 0) {
                        if (building == (BuildingClass *)payback || building->Type->LaserFence && building->unknown_4ec >= 8) {
                            hit = false;
                        }
                        if (building->unknown_slot31()) {
                            hit = false;
                        }
                        if (payback != 0 && payback->House->Is_Ally(building)) {
                            hit = false;
                        }
                    }
                }
            }
            if (position.Z < height || fell || rose || hit) {
                if (payback == 0) {
                    if (fell) {
                        position.Z = bridge_height;
                    } else if (rose) {
                        bridge_height -= 20;
                        position.Z = bridge_height;
                    } else {
                        bridge_height = height - 100;
                        if (bridge_height < position.Z) {
                            position.Z = height;
                        }
                    }
                    Matrix3D slope;
                    Matrix3D inverse;
                    inverse = Matrix3D_Rigid_Inverse_00591420(slope = Slope_Matrix_00718d60(unknown_tactical_83996c->Get_Cell_Ramp_006a7110(new_coord)));
                    Vector3 vec;
                    vec.Set_00437690((float)velocity.X, -(float)velocity.Y, (float)velocity.Z);
                    vec = inverse.Rotate_Vector(vec);
                    vec.Scale_004376b0((float)elasticity);
                    vec.Z = -vec.Z;
                    vec = slope.Rotate_Vector(vec);
                    velocity.X = vec.X;
                    velocity.Y = -vec.Y;
                    velocity.Z = vec.Z;
                } else {
                    if (fell) {
                        position.Z = bridge_height;
                    } else if (rose) {
                        bridge_height -= 20;
                        position.Z = bridge_height;
                    } else {
                        goto bounced;
                    }
                }
                impact = 1;
                forced = true;
            }
bounced:
            coord.X = (int)position.X;
            coord.Y = (int)position.Y;
            coord.Z = (int)position.Z;
        }
        Cell here = Coord_Cell_Of(coord);
        Cell there = Coord_Cell_Of(TargetCoords);
        if (here.X == there.X && here.Y == there.Y && !Type->Vertical && unknown_slot108() < unknown_geometry_g * 2) {
            impact = 1;
            forced = true;
            collided = true;
        } else {
            CellClass &cell = Map[here];
            CellClass &target_cell = Map[there];
            ObjectClass *building = cell.Cell_Building();
            ObjectClass *target_building = target_cell.Cell_Building();
            if (building != 0 && !Type->Vertical && building == target_building
                && unknown_slot108() < unknown_geometry_g * 2) {
                impact = 1;
                forced = true;
                collided = true;
            } else {
                TechnoClass *techno = (TechnoClass *)Map[coord].Cell_Techno(Point2D(0, 0), false, 0);
                bool techno_valid = techno != 0;
                bool payback_valid = Owner != 0;
                bool is_owner = payback_valid && techno == Owner;
                bool ally = techno_valid && payback_valid && Owner->House->Is_Ally(techno);
                bool close;
                if (techno_valid && coord.Distance_YZX(techno->Coord) < 0x80) {
                    close = true;
                } else {
                    close = false;
                }
                if (!is_owner && !ally && close) {
                    impact = 1;
                    forced = true;
                    if (!Type->Inaccurate) {
                        coord = techno->Coord;
                    }
                } else if (!Map.Unknown_0054AB00(coord)) {
                    impact = 2;
                    forced = true;
                    coord = Coord;
                } else {
                    new_velocity.X = velocity.X;
                    new_velocity.Y = velocity.Y;
                    if (!Type->Vertical) {
                        Velocity = TVelocity3D<double>(new_velocity.X, new_velocity.Y, velocity.Z);
                    }
                    if (Velocity.Speed() < 10.0 && unknown_slot108() < 10) {
                        impact = 1;
                        forced = true;
                    }
                }
            }
        }
    }
flown:
    switch (impact) {
    case 2:
        Mark(2);
        unknown_slot60();
        break;
    default:
    case 0:
    case 1:
        Mark(2);
        unknown_slot103(coord);
        BuildingClass *building = (BuildingClass *)Map[coord].Cell_Building();
        if (building != 0 && building->Type->FirestormWall && building->House->unknown_142
            && (Owner == 0 || building->House != Owner->House)
            && Class_Of() != 0 && !Class_Of()->unknown_209) {
            building->unknown_450cb0(this, false);
            unknown_slot60();
            break;
        }
        if (!forced) {
            CoordStruct current = Coord;
            forced = Is_Forced_To_Explode_00461c10(current);
            unknown_slot103(current);
        }
        if (forced && unknown_slot108() < 0) {
            unknown_slot109(0);
        }
        int fuse = Type->ROT > 0 || Type->Ranged ? Data.Proximity_004ceee0(coord) : 0;
        if (Owner != 0 && Owner->unknown_slot32()->JumpJet && fuse == 2) {
            fuse = 1;
        }
        if (!forced && (Type->Dropping || fuse == 0)) {
            if (Type->Degenerates && Strength > 5) {
                Strength--;
            }
            break;
        }
        if (Target != 0 && (fuse == 1 || collided) && !Type->Airburst && !Type->Inaccurate) {
            CoordStruct midpoint = coord;
            CoordStruct target_coord = Target->unknown_slot22();
            midpoint.Z = (midpoint.Z + target_coord.Z) / 2;
            int distance = midpoint.Distance_YZX(target_coord);
            if (collided) {
                distance /= 3;
            }
            if (fuse == 1 || distance <= ((128) > (Velocity.Speed() * 2) ? (128) : (Velocity.Speed() * 2))) {
                unknown_slot103(Target->Center_Coord());
            }
        }
        if (*(AbstractTypeClass *)WH == "NUKE") {
            if (unknown_slot108() < 0) {
                unknown_slot109(0);
            }
            Start_Dominator_Lighting_005208A0();
            Unknown_0063B5B0(13, unknown_slot104());
            int index = AnimTypeClass::From_Name("NUKEBALL");
            AnimTypeClass *type;
            if (index != -1 && (type = AnimTypes[index]) != 0) {
                NextAnim = new AnimClass(type, CoordStruct(Coord), 0, 1, 0x2600, Unknown_0047FCC0(Coord));
                SpawnNextAnim = true;
                unknown_abstracts_ac0bf8.Add(this);
                break;
            }
        }
        unknown_func_461de0(forced);
        unknown_slot60();
        break;
    }
    LastMapCoords = Coord_Cell_Of(coord);
}
