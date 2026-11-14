// bullet.cpp: the original translation unit re-filed as one source (Route A): the accepted
// bodies of its canonical compile group in address order, under the unit's own header
// state (its switches and includes in their original order). The module map
// manifests/original_target/modules/bullet.json carries the addresses and the rows that
// stay in their own files.


#define RA2_DIR_MISSILE_ANGLES
#define RA2_DIR_RADIAN32
#define RA2_BULLET_VELOCITY_DIRS
#define SHAPE_FILE_EMPTY_RECT unknown_static_rect
#define RA2_BUILDING_FULL
// Not exact (RA2_NON_MATCHING) prelude: header switches the drafts below need (guard-on compile only)
#ifdef RA2_NON_MATCHING
#define RA2_BUILDING_LIGHT_DISTANCE
#define RA2_COORD_TPOINT3D
#define RA2_TPOINT3D_DIFFERENCE
#define RA2_BULLET_VELOCITY_TIBSUN_CTOR
#endif

#include "shared/unknown_geometry.hpp"
#include "shared/unknown_static_rect.hpp"
#include "shared/bullet.hpp"
#include "shared/bullet_type.hpp"
#include "shared/matrix3d.hpp"
#include "shared/map_view.hpp"
#include "shared/cell.hpp"
#include "shared/house.hpp"
#include "shared/palette.hpp"
#include "shared/color_scheme.hpp"
#include "shared/techno.hpp"
#include "shared/anim.hpp"
#include "shared/weapon_type.hpp"
#include "shared/building.hpp"
#include "shared/rules.hpp"
#include "shared/random.hpp"
#include "shared/fast_math.hpp"
#include "shared/vector3.hpp"
#include "shared/warhead_type.hpp"
#include "shared/building_type.hpp"
#include "shared/unknown_vector3.hpp"
#include "shared/building_class.hpp"
#include "shared/unit.hpp"
#include "shared/infantry.hpp"
#include "shared/radsite.hpp"
#include "shared/foot.hpp"
#include "shared/capture_manager.hpp"
#include "shared/parasite.hpp"
#include "shared/tag.hpp"
#include "shared/house_type.hpp"
#include "shared/random2.hpp"
#include "shared/sound_play.hpp"
#include "shared/map_cell_view.hpp"
#include "shared/text_print.hpp"
#include "shared/surface_blit.hpp"


// Select the actual full BuildingClass before shared/techno.hpp.
// Bullet.cpp: the unit's header-static dynamic initializers and
// BulletClass::Array (a static data member: its atexit thunk 0x0045F350
// tests and sets a guard bit, as VC6 does for class statics).
#ifndef VECTOR_IMPLEMENT_ALL
#define VECTOR_IMPLEMENT_ALL
#endif

DynamicVectorClass<BulletClass *> BulletClass::Array;


// Dynamic initializer (0x0045F150, _$E5) emitted by HeaderStatics_diagonal_init_0045f120.cpp; no source of its own.


// Dynamic initializer (0x0045F170, _$E8) emitted by HeaderStatics_diagonal_init_0045f120.cpp; no source of its own.


// Dynamic initializer (0x0045F190, _$E11) emitted by HeaderStatics_diagonal_init_0045f120.cpp; no source of its own.


// Dynamic initializer (0x0045F1B0, _$E14) emitted by HeaderStatics_diagonal_init_0045f120.cpp; no source of its own.


// Dynamic initializer (0x0045F1D0, _$E17) emitted by HeaderStatics_diagonal_init_0045f120.cpp; no source of its own.


// Dynamic initializer (0x0045F1F0, _$E20) emitted by HeaderStatics_diagonal_init_0045f120.cpp; no source of its own.


// Dynamic initializer (0x0045F230, _$E23) emitted by HeaderStatics_diagonal_init_0045f120.cpp; no source of its own.


// Dynamic initializer (0x0045F260, _$E26) emitted by HeaderStatics_diagonal_init_0045f120.cpp; no source of its own.


// Dynamic initializer (0x0045F290, _$E29) emitted by HeaderStatics_diagonal_init_0045f120.cpp; no source of its own.


// Dynamic initializer (0x0045F2C0, _$E32) emitted by HeaderStatics_diagonal_init_0045f120.cpp; no source of its own.


// Dynamic initializer (0x0045F2D0, _$E35) emitted by HeaderStatics_diagonal_init_0045f120.cpp; no source of its own.


// Dynamic initializer (0x0045F2F0, _$E38) emitted by HeaderStatics_diagonal_init_0045f120.cpp; no source of its own.


// Dynamic initializer (0x0045F310, _$E45) emitted by HeaderStatics_diagonal_init_0045f120.cpp; no source of its own.


// atexit destructor (0x0045F350, _$E42) emitted by HeaderStatics_diagonal_init_0045f120.cpp; no source of its own.


// BulletClass::BulletClass() (0x0045F3B0)
// An empty bullet registered in Bullets.

BulletClass::BulletClass() :
    Type(0),
    Owner(0),
    unknown_a0(false),
    Bright(false),
    unknown_e8(0),
    unknown_ec(true),
    unknown_ed(true),
    unknown_f0(0),
    Target(0),
    Speed(0),
    InheritedColor(-1),
    unknown_100(0),
    unknown_108(0),
    WH(0),
    AnimFrame(0),
    AnimRateCounter(0),
    WeaponType(0),
    LastMapCoords(-1, -1),
    NextAnim(0),
    SpawnNextAnim(false)
{
    Create_ID();
    SourceCoords.X = 0;
    SourceCoords.Y = 0;
    SourceCoords.Z = 0;
    TargetCoords.X = 0;
    TargetCoords.Y = 0;
    TargetCoords.Z = 0;
    unknown_BulletClass_list_00a40ce8.Add(this);
}


// void BulletClass::Construct(...) (0x0045F4F0)
// Sets up a new bullet; the inherited color comes from the owner's house
// when the type asks for it.

void BulletClass::Construct(BulletTypeClass *type, AbstractClass *target, TechnoClass *owner,
                            int damage, WarheadTypeClass *warhead, int speed, bool bright)
{
    Type = type;
    Target = target;
    Speed = speed;
    WH = warhead;
    Bright = bright;
    Strength = damage;
    Owner = owner;
    AnimFrame = 0;
    AnimRateCounter = Type_View(type)->unknown_2c6;
    if (Type_View(type)->unknown_279 && owner != 0) {
        InheritedColor = Techno_View(owner)->House->unknown_15f40;
    } else {
        InheritedColor = -1;
    }
    DamageMultiplier = 0x100;
    NextAnim = 0;
    SpawnNextAnim = false;
}


// BulletClass::~BulletClass() (0x0045F590)

BulletClass::~BulletClass()
{
    Unknown_006ECB90(this, true);
    if (SpawnNextAnim) {
        unknown_abstracts_ac0bf8.Delete(this);
    }
    if (unknown_flag_a40958) {
        ObjectClass::Limbo();
    }
    Type = 0;
    Owner = 0;
    NextAnim = 0;
    unknown_BulletClass_list_00a40ce8.Delete(this);
}


// Cell const *BulletClass::Occupy_List(int arg1) (0x0045F690)
// Always the "no cell" value (0x7FFF, 0x7FFF), a function-local static like
// ObjectClass::Occupy_List's.

Cell const *BulletClass::Occupy_List(int arg1)
{
    static UnknownStaticCell const unknown_none(Cell(0x7FFF, 0x7FFF));
    return &unknown_none.Cell;
}


// Exit thunk of BulletClass::Occupy_List's local static (0x0045F6E0),
// emitted by BulletClass_unknown_slot64_0045f690.cpp; no source of its own.


// bool BulletClass::Mark(int arg1) (0x0045F6F0)

bool BulletClass::Mark(int arg1)
{
    return ObjectClass::Mark(arg1) ? true : false;
}


// Not exact (RA2_NON_MATCHING): BulletClass::AI at 0x0045F710, best score 60.04, 2026-11-13; differs: 1775 instructions in the original, 1766 in the draft (shape 94.89)
#ifdef RA2_NON_MATCHING
// BulletClass::AI 0x0045F710 (slot 23), draft.
#include "shared/building_type_class.hpp"
#include "shared/techno_type.hpp"
#include "shared/anim_type.hpp"
#include "shared/tactical.hpp"
#include "shared/lightning_storm.hpp"
#include "shared/radar_event.hpp"
Matrix3D * __fastcall Unknown_00718D60(Matrix3D *matrix, int ramp);
int __fastcall Projectile_Motion_00593870(CoordStruct &coord, TVelocity3D<double> &velocity, CoordStruct const &target, UnknownDirStruct const &dir, bool aircraft, bool airburst, bool very_high, bool level);
// The offset and its length (inline, defined further down in this unit).
inline UnknownVector3 Coord_Offset(CoordStruct const &from, CoordStruct const &to);
inline int Vector_Length(UnknownVector3 vector);

// The distance between two coordinates through the out-of-line length 0x0041B780 (TibSun bullet.cpp `::Distance`).
inline int Distance(CoordStruct const &a, CoordStruct const &b)
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
        Delete_Me();
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
        CoordStruct target_coord = Target != 0 ? Target->unknown_slot22() : unknown_geometry_l;
        if (Target != 0 && Target->AbstractFlags.IsObject) {
            target_coord = ((ObjectClass *)Target)->unknown_slot40();
        }
        double &rotvar = Rules->MissileROTVar;
        double phase = ((Fetch_ID() + Frame) % 15) * (1.0 / 15);
        int rot = (int)((Fast_Sin(phase * 6.283185307179586) * rotvar + (rotvar + 1.0)) * Type->ROT);
        if (Vector_Length(Coord_Offset(Center_Coord(), target_coord)) < 256) {
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
        if (distance <= Velocity.Speed() * 0.5 || Get_Height_AGL() <= 0) {
            forced = true;
            impact = 1;
            if (Get_Height_AGL() > 0 && !Type->Airburst) {
                if (target_coord != unknown_geometry_l) {
                    coord = target_coord;
                }
            }
        }
        if (target_coord == unknown_geometry_l) {
            if (Get_Height_AGL() >= Rules->MissileSafetyAltitude) {
                forced = true;
                impact = 1;
            }
        }
        int delta = ::Distance(old_coord, target_coord) - ::Distance(coord, target_coord);
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
            } else if (Get_Height_AGL() < 0) {
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
                    inverse = Matrix3D_Rigid_Inverse_00591420(slope = *Unknown_00718D60(&Matrix3D(), unknown_tactical_83996c->Get_Cell_Ramp_006a7110(new_coord)));
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
        if (here.X == there.X && here.Y == there.Y && !Type->Vertical && Get_Height_AGL() < unknown_geometry_g * 2) {
            impact = 1;
            forced = true;
            collided = true;
        } else {
            CellClass &cell = Map[here];
            CellClass &target_cell = Map[there];
            ObjectClass *building = cell.Cell_Building();
            ObjectClass *target_building = target_cell.Cell_Building();
            if (building != 0 && !Type->Vertical && building == target_building
                && Get_Height_AGL() < unknown_geometry_g * 2) {
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
                    if (Velocity.Speed() < 10.0 && Get_Height_AGL() < 10) {
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
        Delete_Me();
        break;
    default:
    case 0:
    case 1:
        Mark(2);
        Set_Coord(coord);
        BuildingClass *building = (BuildingClass *)Map[coord].Cell_Building();
        if (building != 0 && building->Type->FirestormWall && building->House->unknown_142
            && (Owner == 0 || building->House != Owner->House)
            && Class_Of() != 0 && !Class_Of()->unknown_209) {
            building->unknown_450cb0(this, false);
            Delete_Me();
            break;
        }
        if (!forced) {
            CoordStruct current = Coord;
            forced = Is_Forced_To_Explode_00461c10(current);
            Set_Coord(current);
        }
        if (forced && Get_Height_AGL() < 0) {
            Set_Height_AGL(0);
        }
        int fuse = Type->ROT > 0 || Type->Ranged ? Data.Proximity_004ceee0(coord) : 0;
        if (Owner != 0 && Owner->Techno_Type_Class()->JumpJet && fuse == 2) {
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
                Set_Coord(Target->Center_Coord());
            }
        }
        if (*(AbstractTypeClass *)WH == "NUKE") {
            if (Get_Height_AGL() < 0) {
                Set_Height_AGL(0);
            }
            Start_Dominator_Lighting_005208A0();
            Unknown_0063B5B0(13, unknown_slot104());
            int index = AnimTypeClass::From_Name("NUKEBALL");
            AnimTypeClass *type;
            if (index != -1 && (type = AnimTypes[index]) != 0) {
                NextAnim = new AnimClass(type, CoordStruct(Coord), 0, 1, 0x2600, Get_Explosion_Z_0047fcc0(Coord));
                SpawnNextAnim = true;
                unknown_abstracts_ac0bf8.Add(this);
                break;
            }
        }
        unknown_func_461de0(forced);
        Delete_Me();
        break;
    }
    LastMapCoords = Coord_Cell_Of(coord);
}
#endif

// int BulletClass::unknown_slot116() (0x00461040)
// The shape frame: by the velocity's heading (one of 32 facings), or the
// animation frame for animated types.

int BulletClass::unknown_slot116()
{
    int frame = 0;
    if (!Type_View(Type)->unknown_271) {
        UnknownDirStruct dir((unsigned short)((unknown_math_4ba600(-Velocity.Y, Velocity.X) - 1.5707963267948966) * -10430.060040584269));
        frame = unknown_facing_frames_7acab0[Dir_Facing32(dir)];
    }
    if (Type_View(Type)->unknown_2c4 || Type_View(Type)->unknown_2c5) {
        frame = AnimFrame;
    }
    return frame;
}


// BulletClass slot 67 0x004610D0 (Draw_It: TibSun bullet.cpp shape; Velocity Get_Yaw/Get_Pitch DirType accessors); ABI legacy address arguments.
extern ConvertClass *bullet_normal_drawer;
extern ConvertClass *bullet_anim_drawer;
Matrix3D __fastcall Bullet_Isometric_View();
int __fastcall Bullet_Height_Pixel(int height);
void BulletClass::unknown_slot67(Point2D const &point, Rect const &clip)
{
    if ((unknown_id_source_a3d290->unknown_000 & 0x1000) && Map.Is_Fogged(CoordStruct(Coord))) return;
    if (Type->Inviso || SpawnNextAnim) return;
    if (Type->unknown_206) {
        Matrix3D matrix;
        matrix.Make_Identity();
        matrix.Rotate_Z(Velocity.Get_Yaw().As_Radian32());
        matrix.Rotate_Y(-Velocity.Get_Pitch().As_Radian32());
        Draw_Voxel_00463a00(Type->unknown_b0, Bullet_Isometric_View() * matrix, *(RectPoint2D const *)&point, clip, 0, 0x800, 1000);
        return;
    }
    ShapeFileStruct const *image = (ShapeFileStruct const *)Get_Image_Data();
    if (!image) return;
    int shape = unknown_slot116();
    int height_agl = Get_Height_AGL();
    int ground_height = Map.Get_Height_GL(CoordStruct(Coord));
    if (!unknown_79 && (Map[CoordStruct(Coord)].unknown_140 & 0x100) && height_agl >= unknown_geometry_j) {
        height_agl -= unknown_geometry_j;
        ground_height += unknown_geometry_j;
    }
    if (height_agl > 0 && Type->Shadow) {
        Point2D shadow = point + Point2D(0, Bullet_Height_Pixel(height_agl));
        CC_Draw_Shape(unknown_surface_83995c, bullet_normal_drawer, image, shape, shadow, clip, 0x2601, 0, -10 - Bullet_Height_Pixel(ground_height), 0, 1000, 0, 0, Point2D(0, 0));
    }
    ConvertClass *drawer = bullet_normal_drawer;
    if (Type->AnimPalette) drawer = bullet_anim_drawer;
    else if (Type->FirersPalette) {
        if (InheritedColor != -1) drawer = ColorSchemes[InheritedColor]->Converter;
        else drawer = ColorSchemes[unknown_player_a35db4->ColorSchemeIndex]->Converter;
    }
    CC_Draw_Shape(unknown_surface_83995c, drawer, image, shape, point, clip, 0x2e00, 0, -30 - Bullet_Height_Pixel(Get_Height()), 0, 1000, 0, 0, Point2D(0, 0));
}


// void BulletClass::unknown_func_461470() (0x00461470)
// Retargets the bullet at the cell under its target (cleared when the
// target is in limbo-like state or the cell is the zero cell).

void BulletClass::unknown_func_461470()
{
    Cell cell = Coord_Cell(Target->Center_Coord());
    if (unknown_a40774 == 0 && !Target->In_Air() && (cell.X != unknown_geometry_k.X || cell.Y != unknown_geometry_k.Y)) {
        Target = (AbstractClass *)&Map[cell];
        return;
    }
    Target = 0;
}


// void BulletClass::Detach(AbstractClass *target, bool all) (0x00461520)
// Forgets a target that is being removed: the owner, the target (re-aimed at the
// cell under it when it is a plain object), the type, weapon and next animation;
// a bullet being removed with all is also dropped from the bullet list.

// The cell under an object's centre (inline; its Cell construction sits three
// levels deep in Detach, where /Ob2's divided budget leaves 0x0042AD30 out of
// line as in the original; Inline248).
inline Cell Cell_Of(AbstractClass *object)
{
    return Coord_Cell_Of(object->Center_Coord());
}

// Re-aims a bullet whose target is going away at the cell under that target,
// or clears it (inline helper of Detach).
inline void Retarget(BulletClass *self)
{
    Cell cell = Cell_Of(self->Target);
    if (unknown_a40774 == 0 && !self->Target->In_Air() && cell != unknown_geometry_k) {
        self->Target = &Map[cell];
    } else {
        self->Target = 0;
    }
}

void BulletClass::Detach(AbstractClass *target, bool all)
{
    ObjectClass::Detach(target, all);
    if (Owner == target) {
        Owner = 0;
    }
    if (Target == target) {
        Retarget(this);
    }
    if (Type == target) {
        Type = 0;
    }
    if (WeaponType == target) {
        WeaponType = 0;
    }
    if (NextAnim == target) {
        NextAnim = 0;
    }
    if (all && target->Kind_Of() == 8) {
        BulletClass *bullet = (BulletClass *)target;
        if (Type_View(bullet->Type)->unknown_2bc) {
            for (int i = 0; i < Array.Count(); i++) {
                if (bullet == Array[i]) {
                    Array.Delete(bullet);
                    return;
                }
            }
        }
    }
}


// Bullet launch, typed member declarations authorized by owner0200Z.
// Bullet launch004616B0: independently bounded1343+1, twoRET8 sites.
CellClass *__fastcall Trace_Projectile_Line_004bb890(CoordStruct const &origin,
    CoordStruct const &destination, BulletTypeClass const *projectile, HouseClass const *house);
inline void Set_Projectile_Speed(TVelocity3D<double> &velocity,double speed)
{
    if (velocity.X == 0.0 && velocity.Y == 0.0 && velocity.Z == 0.0) velocity.X = 100.0;
    double factor=speed/Fast_Sqrt(velocity.X*velocity.X+velocity.Y*velocity.Y+velocity.Z*velocity.Z);
    velocity.X*=factor; velocity.Y*=factor; velocity.Z*=factor;
}
inline float Projectile_Launch_Distance(CoordStruct const &source,CoordStruct const &target)
{
    Vector3 v((float)(source.X-target.X),(float)(target.Y-source.Y),(float)(source.Z-target.Z));
    float distance=Fast_Sqrt((double)v.X*v.X+(double)v.Y*v.Y+(double)v.Z*v.Z);
    return distance;
}
bool BulletClass::unknown_slot118(CoordStruct const &source, TVelocity3D<double> const &velocity)
{
    if (ObjectClass::Unlimbo(&source, 0)) {
        Velocity = velocity;
        SourceCoords = source;
        LastMapCoords = Coord_Cell(SourceCoords);
        Map.Remove(this);
        CoordStruct target = Target->unknown_slot22();
        TargetCoords = target;
        if (Type->FlakScatter && Type->Inviso) {
            float distance=Projectile_Launch_Distance(source,target);
            int maximum=Rules->unknown_139c*2;
            int scatter=unknown_id_source_a3d290->Random()(0,maximum);
            int product = (int)((unsigned int)scatter * (unsigned int)(int)distance); // original32-bit IMUL before signed IDIV
            int radius = product / WeaponType->unknown_b4;
            int random_direction = unknown_id_source_a3d290->Random()(0, 0x7ffffffe);
            // Full original conversion payloads verified in R1415 launch-data-proof.json.
            double random_angle = random_direction * 4.656612877414201e-10;
            random_angle *= 6.283185307179586;
            short raw_direction = (short)(int)((random_angle - 1.5707963267948966) * -10430.060040584269);
            int z=target.Z;
            double angle = ((int)raw_direction - 0x3fff) * -9.587672516830327e-05;
            int y = (int)((double)target.Y - Fast_Sin(angle) * (double)radius);
            int x = (int)(Fast_Cos(angle) * (double)radius + target.X);
            target = CoordStruct(x, y, z);
        }
        if (Type->Inviso) {
            HouseClass *house = Owner != 0 ? Owner->House : 0;
            CoordStruct collision = Map.Unknown_00569FB0(source, target, house);
            if (collision != unknown_geometry_l) {
                collision.Z = Map.Get_Height_GL(collision);
                Set_Coord(collision);
                BuildingClass *building = static_cast<BuildingClass *>(Map[collision].Cell_Building());
                building->Unknown_00450CB0(this, false);
                Delete_Me();
            } else {
                CellClass *blocking = Trace_Projectile_Line_004bb890(source, target, Type, house);
                if (blocking != 0) Set_Coord(blocking->Center_Coord());
                else Set_Coord(target);
                // Original calls again even after the blocking-cell branch.
                Set_Coord(target);
                Speed = 0;
                Set_Projectile_Speed(Velocity,0.0);
            }
        }
        int arm = Target != 0 && Target->Kind_Of() == 2 ? 0 : Type->Arm;
        Data.Initialize_004cee20(CoordStruct(Coord), target, arm, 0x7fffffff);
        if (Type->ROT > 0) {
            Set_Projectile_Speed(Velocity,1.0);
        }
        if (unknown_7d) Map.Submit(this);
        return true;
    }
    return false;
}


// int BulletClass::In_Which_Layer() const (0x00461BF0)
// The display layer: 1 for types with +0x2C7 set, else 3.

int BulletClass::In_Which_Layer() const
{
    return Type_View(Type)->unknown_2c7 ? 1 : 3;
}


// Forced-explosion decision00461C10. Fresh real-type source; no steering casts.
bool BulletClass::Is_Forced_To_Explode_00461c10(CoordStruct &coord)
{
    coord = Coord;
    CellClass *cell = &Map[coord];
    if (Type->SubjectToCliffs || Type->SubjectToWalls) {
        CellClass *source = &Map[SourceCoords];
        CellClass *destination = &Map[TargetCoords];
        CellClass *previous = &Map[LastMapCoords];
        if (Unknown_004BBAF0(source, destination, previous, coord, Type, Owner == 0 ? 0 : Owner->House)) return true;
    }
    if (Get_Height_AGL() <= -unknown_geometry_g * 4) return true;
    if (Target != 0 && Type->FlakScatter) {
        CoordStruct current = Coord;
        if (current.Z < Target->unknown_slot22().Z && Get_Height_AGL() < 0) return true;
    }
    if (Type->Level && cell->On_Ground()) return true;
    if (Type->AA && Target != 0 && Target->In_Air() && unknown_5d4c00(Target) < 0x80) return true;
    return false;
}


// void BulletClass::unknown_func_461de0(bool forced) (0x00461DE0)
// See shared/bullet.hpp. The aim point starts at the bullet; an accurate
// bullet close to its target (within 32) takes the target's centre, and
// unless its warhead is an EMP or it bursts in the air, the remembered point
// (Data +0x18) of a straight, unforced shot, an object target's slot 40
// point (within 0x80) or a target's slot 22 point (within 0x2A; a building
// whose type has an offset at +0xC48 uses slot 40). Then Cluster
// detonations (0x00462110), the later ones scattered 256-511 leptons around
// the aim point, until the bullet is gone; an air burst detonates once.

// A target building as this function reads it (BuildingClass +0x418 Type).
struct UnknownBulletTargetBuilding {
    char unknown_000[0x418];
    BuildingTypeClass *Type;            // +0x418
};

// The length of an offset taken by value (the terms loaded Y, Z, X and
// summed X, Z, Y, as foot.hpp's Offset_Length).
inline UnknownVector3 Coord_Offset(CoordStruct const &from, CoordStruct const &to)
{
    return UnknownVector3(from.X - to.X, from.Y - to.Y, from.Z - to.Z);
}

inline int Vector_Length(UnknownVector3 vector)
{
    double y = vector.Y;
    double z = vector.Z;
    double x = vector.X;
    return (int)unknown_sqrt_4ba410(x * x + z * z + y * y);
}

void BulletClass::unknown_func_461de0(bool forced)
{
    ObjectClass *object = 0;
    CoordStruct coord = Coord;
    if (Target != 0 && Target->In_Air()) {
        object = (ObjectClass *)Target;
    }
    if (!Type->Inaccurate) {
        if (Target != 0) {
            if (Vector_Length(Coord_Offset(coord, Target->Center_Coord())) < 32
                && !Type->Airburst && !Type->Inaccurate) {
                coord = Target->Center_Coord();
            }
        }
        if (!WH->EMEffect && !Type->Airburst) {
            if (!forced && !Type->Arcing && Type->ROT <= 0) {
                CoordStruct aim = Data.unknown_18;
                if (aim != unknown_geometry_l) {
                    coord = Data.unknown_18;
                }
            }
            if (object != 0 && object->In_Which_Layer() != 2) {
                if (unknown_5d4c00(Target) < 0x80) {
                    coord = object->unknown_slot40();
                }
            } else if (Target != 0 && unknown_5d4c00(Target) < 0x2A) {
                coord = Target->unknown_slot22();
                if (Target->Kind_Of() == 6) {
                    CoordStruct const &offset = ((UnknownBulletTargetBuilding *)Target)->Type->TargetCoordOffset;
                    if (offset.X != 0 || offset.Y != 0 || offset.Z != 0) {
                        coord = ((ObjectClass *)Target)->unknown_slot40();
                    }
                }
            }
        }
    }
    if (!Type->Airburst) {
        CoordStruct origin = coord;
        for (int index = 0; index < Type->Cluster; index++) {
            unknown_func_462110(coord);
            if (!unknown_7d) {
                return;
            }
            int distance = unknown_id_source_a3d290->Random()(0x100, 0x200);
            coord = Coord_Scatter(origin, distance, false);
        }
    } else {
        unknown_func_462110(coord);
    }
}


// void BulletClass::unknown_func_462110(CoordStruct const &coord) (0x00462110)
// The detonation at coord (hypothesis: Westwood's BulletClass::Detonate):
// shakes the screen by the warhead's ranges, leaves the weapon's radiation
// in the cell (unknown_func_463720, expanded here), then applies the
// warhead's special: MindControl captures the target (springing its tag
// events 6 and 0x2C first, and playing YuriMindControlSound when a player's
// unit is involved), IvanBomb plants a bomb, ElectricAssault charges a
// building from an infantry owner, Parasite, Temporal, BombDisarm,
// MakesDisguise, NukeMaker (unknown_func_463c50); any other warhead fires
// the ShrapnelWeapon (unknown_func_462c50) and damages the area. Then the
// impact animation (scattered for Inviso), the combat light of a Bright
// bullet, and for an Airburst projectile nine AirburstWeapon bullets: one
// into each neighbour cell of the bullet's cell and one into the cell
// itself, each launched straight down with a random facing. A nullified
// hit (the area damage returned 2) shows Rules' WeaponNullifyAnim instead.

// The impact animation of a warhead hitting a land type (-1 above the
// ground) at a coordinate (0x0047F4D0, __fastcall; hypothesis).
AnimTypeClass *__fastcall Unknown_0047F4D0(int damage, WarheadTypeClass *warhead, int land, CoordStruct const &coord);
// The height adjustment an animation at a coordinate gets (0x0047FCC0;
// hypothesis: the AnimClass constructor's z adjustment).
int __stdcall Get_Explosion_Z_0047fcc0(CoordStruct coord);

// A value in [a, b] from the generator at 0x00839218 (the other generator's
// form of Random_Pick in shared/id_source.hpp: the result is a variable).
template<class T> inline T Random2_Pick(T a, T b)
{
    return T(unknown_random_839218((int)a, (int)b));
}

// The target as a techno (flag bit 0), or 0.
inline TechnoClass *Bullet_Target_Techno(AbstractClass *target)
{
    if (target == 0) return 0;
    return target->AbstractFlags.IsTechno ? static_cast<TechnoClass *>(target) : 0;
}

void BulletClass::unknown_func_462110(CoordStruct const &position)
{
    int damage_result = 1;
    bool special_impact = false;
    if (WH->ShakeXlo != 0 || WH->ShakeXhi != 0)
        Map.ShakeX = Random2_Pick(WH->ShakeXlo, WH->ShakeXhi);
    if (WH->ShakeYlo != 0 || WH->ShakeYhi != 0)
        Map.ShakeY = Random2_Pick(WH->ShakeYlo, WH->ShakeYhi);
    if (WeaponType != 0) {
        int level = WeaponType->unknown_12c;
        if (level > 0) {
            unknown_func_463720(Coord_Cell(position), (int)WeaponType->Warhead->CellSpread, level);
        }
    }
    if (WH->MindControl) {
        if (Owner != 0 && Owner->unknown_230 != 0) {
            ObjectClass *object = Target == 0 ? 0 :
                (Target->AbstractFlags.IsObject ? static_cast<ObjectClass *>(Target) : 0);
            bool target_house = Target != 0 && Target->AbstractFlags.IsTechno &&
                static_cast<TechnoClass *>(Target)->House->Is_Player_Control();
            if (object != 0 && object->unknown_7d && object->unknown_34 != 0) {
                object->unknown_34->Spring(6, object, unknown_geometry_k, false, (int)Owner);
                object->unknown_34->Spring(0x2c, object, unknown_geometry_k, false, (int)Owner);
            }
            if (static_cast<CaptureManagerClass *>(Owner->unknown_230)->Capture(static_cast<TechnoClass *>(Target))
                && Rules->YuriMindControlSound != -1
                && (Owner->House->Is_Player_Control() || target_house)) {
                Voc_Play_At(Rules->YuriMindControlSound, CoordStruct(object->Coord), 0);
            }
        }
    } else if (WH->IvanBomb) {
        TechnoClass *target = Target == 0 ? 0 :
            (Target->AbstractFlags.IsTechno ? static_cast<TechnoClass *>(Target) : 0);
        unknown_list_8322e0.Add_Bomb(Owner, target);
    } else if (WH->ElectricAssault) {
        if (Target != 0 && Target->Kind_Of() == 6 && Owner != 0 && Owner->Kind_Of() == 15)
            static_cast<BuildingClass *>(Target)->unknown_44da20(static_cast<InfantryClass *>(Owner));
    } else if (WH->Parasite) {
        if (Owner != 0) {
            FootClass *target = Target == 0 ? 0 :
                (Target->AbstractFlags.IsFoot ? static_cast<FootClass *>(Target) : 0);
            static_cast<FootClass *>(Owner)->unknown_588->Unknown_00607980(target);
        }
    } else if (WH->Temporal) {
        if (Owner != 0 && Target != 0) {
            TechnoClass *target = Bullet_Target_Techno(Target);
            Owner->unknown_208->Unknown_006E27B0(target);
        }
    } else if (WH->BombDisarm) {
        if (Target != 0 && Target->AbstractFlags.IsObject) {
            ObjectClass *object = static_cast<ObjectClass *>(Target);
            if (object->unknown_38 != 0) object->unknown_38->Unknown_00435FB0();
        }
    } else if (WH->MakesDisguise) {
        if (Owner != 0) Owner->unknown_slot255(static_cast<TechnoClass *>(Target));
    } else if (WH->NukeMaker) {
        unknown_func_463c50();
    } else {
        if (Type->ShrapnelWeapon != 0) unknown_func_462c50();
        int damage = DamageMultiplier * Strength;
        damage_result = Unknown_0047E2C0(position, damage >> 8, Owner, WH, true);
        if (!unknown_7d) return;
        if (damage_result == 2) special_impact = true;
    }
    CoordStruct impact = position;
    if (Type->Inviso) impact = Coord_Scatter(impact, 32, false);
    int land = -1;
    if (Get_Height_AGL() < unknown_geometry_g * 2) {
        land = Map[CoordStruct(Coord)].unknown_ec;
        if (land == 2 && damage_result == 0) {
            ObjectClass *occupier = Map[CoordStruct(Coord)].OccupierPtr;
            if (occupier != 0 && occupier->Kind_Of() == 1) {
                UnitTypeClass *type = static_cast<UnitClass *>(occupier)->Type;
                if (type->Naval && !type->Underwater) land = -1;
            }
        }
    }
    AnimTypeClass *animation = Unknown_0047F4D0(Strength, WH, land, CoordStruct(Coord));
    if (Bright && !special_impact) {
        unsigned flags = 0;
        if (WH->CLDisableRed) flags = 2;
        if (WH->CLDisableGreen) flags |= 4;
        if (WH->CLDisableBlue) flags |= 8;
        Unknown_0047F600(impact, Strength, WH, 1, flags);
    }
    if (special_impact) {
        AnimTypeClass *nullify = Rules->WeaponNullifyAnim;
        if (nullify != 0)
            new AnimClass(nullify, impact, 0, 1, 0x2600, Get_Explosion_Z_0047fcc0(impact));
    } else {
        if (animation != 0) {
            AnimClass *created = new AnimClass(animation, impact, 0, 1, 0x2600, Get_Explosion_Z_0047fcc0(impact));
            if (created == 0 && WH == Rules->unknown_c60) {
                int house_type = -1;
                if (Owner != 0) house_type = Owner->House->Type->House;
                Unknown_00422BA0(house_type, Coord_Cell(impact));
            }
        }
        if (Type->Airburst) {
            WeaponTypeClass *weapon = Type->AirburstWeapon;
            BulletTypeClass *projectile = weapon->Projectile;
            unsigned facing = 0;
            CellClass *center = static_cast<CellClass *>(Get_Cell_Ptr());
            int remaining = 8;
            do {
                CellClass &neighbor = center->Adjacent_Cell(facing);
                facing = (facing + 1) & 7;
                BulletClass *fragment = Create_Bullet(projectile, &neighbor, Owner, weapon->unknown_a4, weapon->Warhead, 50, false);
                if (fragment != 0) {
                    int random_direction = unknown_id_source_a3d290->Random()(0, 32);
                    short raw = (short)((unsigned short)(unsigned char)random_direction << 8);
                    double speed = (double)(weapon->unknown_a8 / 10);
                    double angle = ((int)raw - 0x3fff) * -9.587672516830327e-05;
                    TVelocity2D<double> planar(unknown_cos_4ba4d0(angle) * unknown_cos_4ba4d0(4.712436918747274) * speed, unknown_sin_4ba480(angle) * unknown_cos_4ba4d0(4.712436918747274) * speed);
                    TVelocity3D<double> velocity;
                    velocity.XY() = planar;
                    velocity.Z = unknown_sin_4ba480(4.712436918747274) * speed;
                    fragment->unknown_slot118(CoordStruct(Coord), velocity);
                }
            } while (--remaining != 0);
            BulletClass *fragment = Create_Bullet(projectile, center, Owner, weapon->unknown_a4, weapon->Warhead, 50, false);
            if (fragment != 0) {
                int random_direction = unknown_id_source_a3d290->Random()(0, 32);
                short raw = (short)((unsigned short)(unsigned char)random_direction << 8);
                double speed = (double)(weapon->unknown_a8 / 10);
                double angle = ((int)raw - 0x3fff) * -9.587672516830327e-05;
                TVelocity2D<double> planar(unknown_cos_4ba4d0(angle) * unknown_cos_4ba4d0(4.712436918747274) * speed, unknown_sin_4ba480(angle) * unknown_cos_4ba4d0(4.712436918747274) * speed);
                TVelocity3D<double> velocity;
                velocity.XY() = planar;
                velocity.Z = unknown_sin_4ba480(4.712436918747274) * speed;
                fragment->unknown_slot118(CoordStruct(Coord), velocity);
            }
        }
    }
}


// BulletClass 0x00462C50 (shrapnel)

// The first index of each ring in unknown_cells_a6f248 (0x007A5908).
extern int const unknown_radius_starts_7a5908[];
// Cell offsets in rings of growing radius (0x00A6F248; MapClass unit).
extern Cell unknown_cells_a6f248[];

// Hypothesis (Inline156): an inline accessor returning the object coordinate by value; its sites give
// the original inline pattern (each Coord copy is a pending site under the depth-first rule).
inline CoordStruct Coord_Of_462c50(ObjectClass const *o) { return o->Coord; }

// Westwood's Direction(Coord const &, Coord const &) (TibSun inline.h): only the X members get locals,
// and the angle becomes a DirType.
inline DirType Coord_Direction_462c50(CoordStruct const &coord1, CoordStruct const &coord2)
{
    int x1 = coord1.X;
    int x2 = coord2.X;
    return(DirType(Fast_Atan2((double)coord1.Y - (double)coord2.Y, (double)x2 - (double)x1)));
}

void BulletClass::unknown_func_462c50()
{
    int shrapnel = Type->ShrapnelCount;
    int count;
    if (shrapnel < 0) {
        if (Owner == 0) {
            count = 3;
        } else {
            CoordStruct coord = Coord_Of_462c50(this);
            UnknownVector3 offset = Coord_Offset(Owner->Center_Coord(), coord);
            count = -shrapnel - offset.Length() / 256;
            if (count <= 0) return;
        }
    } else {
        count = shrapnel;
    }
    if (static_cast<CellClass *>(Get_Cell_Ptr())->OccupierPtr == 0) return;
    CellClass *center = static_cast<CellClass *>(Get_Cell_Ptr());
    if (center->OccupierPtr->Kind_Of() == 6) return;
    WeaponTypeClass *weapon = Type->ShrapnelWeapon;
    BulletTypeClass *projectile = weapon->Projectile;
    Cell origin = unknown_slot104();
    int range = weapon->unknown_b4 / 256;
    int fired = 0;
    // One velocity for both loops, zeroed with Set() before each fragment (TibSun building.cpp idiom).
    TVelocity3D<double> velocity;
    for (int radius = 1; radius <= range; radius++) {
        int end = unknown_radius_starts_7a5908[radius];
        for (int index = unknown_radius_starts_7a5908[radius - 1]; index < end; index++) {
            CellClass *cellptr = &Map[unknown_cells_a6f248[index] + origin];
            ObjectClass *target = cellptr->OccupierPtr;
            if (target != 0 && target != Owner && Owner != 0 && !Owner->House->Is_Ally((TechnoClass *)target)) {
                BulletClass *fragment = Create_Bullet(projectile, target, Owner, weapon->unknown_a4, weapon->Warhead, weapon->unknown_a8, weapon->unknown_10b);
                if (fragment != 0) {
                    velocity.Set(0, 0, 0);
                    velocity.Set_Yaw(Coord_Direction_462c50(Coord_Of_462c50(this), target->Center_Coord()));
                    velocity.Set_Pitch(DirType(0x2000));
                    velocity.Set_Speed(weapon->unknown_a8);
                    fragment->unknown_slot118(Coord_Of_462c50(this), velocity);
                    CoordStruct source = Coord_Of_462c50(this);
                    if (weapon->unknown_121 && Owner != 0) Owner->unknown_6c9df0(target, 0, weapon, source);
                    if (weapon->unknown_127 && Owner != 0) Owner->unknown_6ca020(target, weapon, source);
                }
                if (++fired == count) return;
            }
        }
    }
    if (fired < count) {
        int remaining = count - fired;
        do {
            Cell cell = unknown_slot104();
            cell.X += Random_Pick(-2, 2);
            cell.Y += Random_Pick(-2, 2);
            BulletClass *fragment = Create_Bullet(projectile, &Map[cell], Owner, weapon->unknown_a4, weapon->Warhead, weapon->unknown_a8, weapon->unknown_10b);
            if (fragment != 0) {
                velocity.Set(0, 0, 0);
                velocity.Set_Yaw(Coord_Direction_462c50(Coord_Of_462c50(this), Map_Cell(cell).Center_Coord()));
                velocity.Set_Speed(weapon->unknown_a8);
                velocity.Set_Pitch(DirType(0x2000));
                fragment->unknown_slot118(Coord_Of_462c50(this), velocity);
                CoordStruct source = Coord_Of_462c50(this);
                if (weapon->unknown_121 && Owner != 0) Owner->unknown_6c9df0(&Map_Cell(cell), 0, weapon, source);
                if (weapon->unknown_127 && Owner != 0) Owner->unknown_6ca020(&Map_Cell(cell), weapon, source);
            }
        } while (--remaining != 0);
    }
}


// void BulletClass::unknown_func_463720(Cell cell, int spread, int level) (0x00463720)
// See shared/bullet.hpp: a new radiation site (0x74 bytes) is based at the
// cell with the spread and level and activated (0x00637640), then entered in
// the cell (0x0047CE30); a cell that has one gets the level added.

void BulletClass::unknown_func_463720(Cell cell, int spread, int level)
{
    RadSiteCell &where = (RadSiteCell &)Map[cell];
    RadSiteClass *site = where.Unknown_0047CE40();
    if (site == 0) {
        site = new RadSiteClass();
        site->Set_Base_Cell(cell);
        site->Set_Spread(spread);
        site->Set_Rad_Level(level);
        site->Unknown_00637640();
        where.Unknown_0047CE30(site);
    } else {
        site->Increase_Radiation(level);
    }
}


// HRESULT __stdcall BulletClass::Load(IStream *stream) (0x004637B0, slot 5)
// Loads the object, rebuilds itself in place, re-registers a pending next
// animation and fixes up its pointers.

HRESULT __stdcall BulletClass::Load(IStream *stream)
{
    HRESULT result = ObjectClass::Load(stream);
    if (SUCCEEDED(result)) {
        new (this) BulletClass(NoInitClass());
        if (NextAnim) {
            unknown_abstracts_ac0bf8.Add(this);
        }
        Swizzle_Pointer(Type);
        Swizzle_Pointer(Owner);
        Swizzle_Pointer(WH);
        Swizzle_Pointer(Target);
        Swizzle_Pointer(WeaponType);
        Swizzle_Pointer(NextAnim);
    }
    return result;
}


// HRESULT __stdcall BulletClass::Save(IStream *stream, BOOL clear_dirty) (0x004638F0)

HRESULT __stdcall BulletClass::Save(IStream *stream, BOOL clear_dirty)
{
    return AbstractClass::Save(stream, clear_dirty);
}


// unsigned long __stdcall BulletClass::AddRef() (0x00463910)
// Bullets are reference counted COM objects.

unsigned long __stdcall BulletClass::AddRef()
{
    unknown_com_objects_a6eb14++;
    return InterlockedIncrement(&RefCount);
}


// unsigned long __stdcall BulletClass::Release() (0x00463930)

unsigned long __stdcall BulletClass::Release()
{
    unknown_com_objects_a6eb14--;
    long count = InterlockedDecrement(&RefCount);
    if (count == 0) {
        delete this;
    }
    return count;
}


// bool BulletClass::unknown_func_463970() (0x00463970)

bool BulletClass::unknown_func_463970()
{
    return Type_View(Type)->unknown_2ac > 0 ? true : false;
}


// BulletClass *__fastcall Create_Bullet(...) (0x00463990)

BulletClass *__fastcall Create_Bullet(BulletTypeClass *type, AbstractClass *target, TechnoClass *owner,
                                      int damage, WarheadTypeClass *warhead, int speed, bool bright)
{
    void *object = 0;
    if (FAILED(CoCreateInstance(CLSID_BulletClass, 0, 7, IID_IUnknown_7afce0, &object))) {
        return 0;
    }
    BulletClass *bullet = (BulletClass *)object;
    bullet->Construct(type, target, owner, damage, warhead, speed, bright);
    return bullet;
}


// Bullet voxel drawing00463A00: complete original408+8, ABI/layout hypotheses.
struct BulletVoxelBounds {
    RectPoint2D offset; // first8, original463B0C/463B10
    Rect rectangle; // +08, original463B6C LEA passes this field
};
void __fastcall Prepare_Voxel_Lighting_00717050(VoxelLibrary *voxel, int section, int arg3, Matrix3D const &matrix, Vector3 const &light);
void __cdecl Reset_Voxel_Renderer_007171d0();
void __fastcall Add_Voxel_Section_007174c0(VoxelLibrary *voxel, unsigned section, int arg3, Matrix3D const &matrix);
BulletVoxelBounds __fastcall Voxel_Render_Bounds_007178e0();
Surface &__cdecl Voxel_Render_Surface_00717040();
extern Matrix3D unknown_voxel_transform_839a70;
extern Vector3 unknown_voxel_light_839ab0;
extern int unknown_flat_voxel_7d11a4;
void BulletClass::Draw_Voxel_00463a00(UnknownObjectPair const &pair, Matrix3D const &matrix,
                                   RectPoint2D const &point, Rect const &window, unsigned frame,
                                   unsigned flags, int brightness)
{
    VoxelLibrary *voxel = pair.unknown_00;
    UnknownPairSecond *motion = pair.unknown_04;
    Prepare_Voxel_Lighting_00717050(voxel, 0, 0, matrix, unknown_voxel_light_839ab0);
    Reset_Voxel_Renderer_007171d0();
    // Original leaves this uninitialized if sections exist but motion is null;
    // valid caller inputs/precondition remain to be evidenced.
    Matrix3D transformed;
    for (unsigned section = 0; section < (unsigned)voxel->LayerCount; ++section) {
        if (motion != 0) {
            Matrix3D section_matrix;
            section_matrix = motion->unknown_transforms[(frame % motion->unknown_frame_count) * motion->unknown_section_count + section];
            transformed = matrix * section_matrix;
        }
        Add_Voxel_Section_007174c0(voxel, section, 0, unknown_voxel_transform_839a70 * transformed);
    }
    BulletVoxelBounds bounds = Voxel_Render_Bounds_007178e0();
    int flat = 0;
    unsigned draw_flags = flags & ~0x10;
    if (Type->Flat) {
        flat = unknown_flat_voxel_7d11a4;
        draw_flags |= 0x2000;
    }
    unknown_blit_49f3a0(*unknown_surface_83995c, ColorSchemes[Type->Color]->Converter, Voxel_Render_Surface_00717040(),
                       bounds.rectangle, point + bounds.offset, window, 0,
                       ColorSchemes[Type->Color]->Converter->Select_Blitter_00485AA0(draw_flags), flat,
                       Type->Flat ? 0 : 2, brightness, 0);
}

// Genuine shared point-sum operation; complete emitted copy binds435540.
RectPoint2D const RectPoint2D::operator+(RectPoint2D const &point) const { return RectPoint2D(X + point.X, Y + point.Y); }


// void BulletClass::Set_Weapon_Type(WeaponTypeClass *weapon) (0x00463BA0)

void BulletClass::Set_Weapon_Type(WeaponTypeClass *weapon)
{
    WeaponType = weapon;
}


// WeaponTypeClass *BulletClass::Get_Weapon_Type() (0x00463BB0)

WeaponTypeClass *BulletClass::Get_Weapon_Type()
{
    return WeaponType;
}


// void BulletClass::unknown_func_463bc0() (0x00463BC0)
// Tracks the bullet in BulletClass::Array and updates the type's value by
// how many are tracked.

void BulletClass::unknown_func_463bc0()
{
    Array.Add(this);
    int count = Array.Count() - 5;
    if (count < 0) {
        count = 0;
    }
    Type_View(Type)->Unknown_00465150(Type_View(Type)->unknown_2b4 + count / 3);
}


// void BulletClass::unknown_func_463c50() (0x00463C50)
// A NukeMaker warhead's payload (hypothesis): a bullet of the weapon named
// NukePayload, bright, aimed at the target and owned by this bullet's owner,
// is placed above the centre of the target's cell (this bullet's weapon's
// projectile DetonationAltitude higher) and launched straight down (the
// direction and pitch 0x3FFF steps, speed -1).

void BulletClass::unknown_func_463c50()
{
    AbstractClass *target = Target;
    CellClass *target_cell = &Map[Coord_Cell_Of(target->Center_Coord())];
    WeaponTypeClass *payload = unknown_WeaponTypeClass_list_00839b90[WeaponTypeClass::Index_From_Name("NukePayload")];
    BulletTypeClass *projectile = payload->Projectile;
    WarheadTypeClass *warhead = payload->Warhead;
    int speed = payload->unknown_a8;
    int damage = payload->unknown_a4;
    TechnoClass *owner = Owner;
    BulletClass *bullet = Create_Bullet(projectile, target, owner, damage, warhead, speed, true);
    if (bullet != 0) {
        bullet->Limbo();
        bullet->Set_Weapon_Type(payload);
        CoordStruct launch = target_cell->Center_Coord();
        launch = launch + CoordStruct(0, 0, WeaponType->Projectile->DetonationAltitude);
        double y = unknown_sin_4ba480(1.5707483884323126);
        y *= unknown_cos_4ba4d0(1.5707483884323126);
        y *= -1.0;
        TVelocity2D<double> planar(unknown_cos_4ba4d0(1.5707483884323126) * unknown_cos_4ba4d0(1.5707483884323126) * -1.0, y);
        TVelocity3D<double> velocity;
        velocity.XY() = planar;
        velocity.Z = unknown_sin_4ba480(1.5707483884323126) * -1.0;
        bullet->unknown_slot118(launch, velocity);
    }
}


// int BulletClass::Size_Of() const (0x00463E80): inline in shared/bullet.hpp, emitted with the unit's vtable; no source of its own.


// int BulletClass::Kind_Of() const (0x00463E90): inline in shared/bullet.hpp, emitted with the unit's vtable; no source of its own.


// HRESULT __stdcall BulletClass::GetClassID(GUID *clsid) (0x00463EA0): inline in shared/bullet.hpp, emitted with the unit's vtable; no source of its own.


// void BulletClass::Set_Target(AbstractClass *target) (0x00463EE0): inline in shared/bullet.hpp, emitted with the unit's vtable; no source of its own.


// ObjectTypeClass *BulletClass::Class_Of() const (0x00463EF0): inline in shared/bullet.hpp, emitted with the unit's vtable; no source of its own.


// BulletClass scalar deleting destructor (0x00463F00): compiler generated; no source of its own.


// Container members this unit emits by its own use of its lists (bodies: shared/vector_class.hpp):
// DynamicVectorClass<BulletClass *>::Clear (0x00463F20; DynamicVectorClass<BulletClass *> vtable 0x0079d308 slot 3).
// DynamicVectorClass<BulletClass *>::ID_pointer (0x00463F50; DynamicVectorClass<BulletClass *> vtable 0x0079d308 slot 5).
// VectorClass<BulletClass *>::Fetch (0x00463F70; VectorClass<BulletClass *> vtable 0x0079d328 slot 6).


// CDTimerClass<FrameTimerClass>::CDTimerClass(long set) (0x00463F80)
// Instantiated from shared/cd_timer.hpp; BulletClass::Load's in-place
// rebuild (UnknownBulletData's NoInitClass constructor) calls it out of line.
// @symbol ??0?$CDTimerClass@VFrameTimerClass@@@@QAE@J@Z


// Existing three-double vector operation; original4642A0 full33+15.

// The existing shared vector operation, with an ordinary out-of-class definition.
// No alias, inlining pragma, auxiliary operation, or forced call is introduced.
TVelocity3D<double> &TVelocity3D<double>::operator+=(TVelocity3D<double> const &other)
{
    X+=other.X;
    Y+=other.Y;
    Z+=other.Z;
    return *this;
}


// Container members this unit emits by its own use of its lists (bodies: shared/vector_class.hpp):
// VectorClass<BulletClass *>::operator equal (0x00463FA0; VectorClass<BulletClass *> vtable 0x0079d328 slot 1).
// VectorClass<BulletClass *>::Resize (0x00463FE0; VectorClass<BulletClass *> vtable 0x0079d328 slot 2).
// VectorClass<BulletClass *>::Clear (0x00464090; VectorClass<BulletClass *> vtable 0x0079d328 slot 3).
// VectorClass<BulletClass *>::ID_object (0x004640C0; VectorClass<BulletClass *> vtable 0x0079d328 slot 4).
// VectorClass<BulletClass *>::ID_pointer (0x00464100; VectorClass<BulletClass *> vtable 0x0079d328 slot 5).
// DynamicVectorClass<BulletClass *>::Resize (0x00464120; DynamicVectorClass<BulletClass *> vtable 0x0079d308 slot 2).
// DynamicVectorClass<BulletClass *>::ID_object (0x004641D0; DynamicVectorClass<BulletClass *> vtable 0x0079d308 slot 4).


extern "C" unsigned char vtable_464200[]; extern "C" void __cdecl release_buffer_464200(void*); extern "C" void __cdecl release_object_464200(void*);
class Leaf_464200 { public: void* get(unsigned int flags); };
void* Leaf_464200::get(unsigned int flags) { unsigned int pointer = *(unsigned int*)((char*)this + 4); *(unsigned int*)this=(unsigned int)vtable_464200; if(pointer && *(unsigned char*)((char*)this + 13)){ release_buffer_464200((void*)pointer); *(unsigned int*)((char*)this + 4)=0; } *(unsigned char*)((char*)this + 13)=0; *(unsigned int*)((char*)this + 8)=0; if(flags & 1) release_object_464200(this); return this; }


extern "C" unsigned char vtable_464250[]; extern "C" void __cdecl release_buffer_464250(void*); extern "C" void __cdecl release_object_464250(void*);
class Leaf_464250 { public: void* get(unsigned int flags); };
void* Leaf_464250::get(unsigned int flags) { unsigned int pointer = *(unsigned int*)((char*)this + 4); *(unsigned int*)this=(unsigned int)vtable_464250; if(pointer && *(unsigned char*)((char*)this + 13)){ release_buffer_464250((void*)pointer); *(unsigned int*)((char*)this + 4)=0; } *(unsigned char*)((char*)this + 13)=0; *(unsigned int*)((char*)this + 8)=0; if(flags & 1) release_object_464250(this); return this; }


// BulletClass::AddRef adjustor thunk for IRTTITypeInfo (0x004642D0): compiler generated (this - 4, then AddRef); no source of its own.


// BulletClass::Release adjustor thunk for IRTTITypeInfo (0x004642E0): compiler generated (this - 4, then Release); no source of its own.
