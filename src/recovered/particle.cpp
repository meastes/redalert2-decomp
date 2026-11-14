// particle.cpp: the original translation unit re-filed as one source (Route A): the accepted
// bodies of its canonical compile group in address order, under the unit's own header
// state (its switches and includes in their original order). The module map
// manifests/original_target/modules/particle.json carries the addresses and the rows that
// stay in their own files.

#define RA2_VECTOR_UNSIGNED_INDEX  // migration holdout: unit not yet re-rolled to VectorClass::operator[](int) (docs/HANDOFF.md)
#define RA2_MATRIX3D_INLINE_COPY
// Header-static dynamic initializers of the unit at 0x00608250:  the
// shared/unknown_geometry.hpp constants and unknown_static_rect.hpp
// (0x00608250-0x00608436); the other
// drafts of this plan are emitted by these includes.
#include "shared/unknown_geometry.hpp"
#include "shared/unknown_static_rect.hpp"


// Dynamic initializer (0x00608280, _$E) of b from the header(s) included
// by HeaderStatics_diagonal_init_00608250.cpp; no source of its own.


// Dynamic initializer (0x006082A0, _$E) of c from the header(s) included
// by HeaderStatics_diagonal_init_00608250.cpp; no source of its own.


// Dynamic initializer (0x006082C0, _$E) of angle45 from the header(s) included
// by HeaderStatics_diagonal_init_00608250.cpp; no source of its own.


// Dynamic initializer (0x006082E0, _$E) of angle60 from the header(s) included
// by HeaderStatics_diagonal_init_00608250.cpp; no source of its own.


// Dynamic initializer (0x00608300, _$E) of angle90 from the header(s) included
// by HeaderStatics_diagonal_init_00608250.cpp; no source of its own.


// Dynamic initializer (0x00608320, _$E) of g from the header(s) included
// by HeaderStatics_diagonal_init_00608250.cpp; no source of its own.


// Dynamic initializer (0x00608360, _$E) of h from the header(s) included
// by HeaderStatics_diagonal_init_00608250.cpp; no source of its own.


// Dynamic initializer (0x00608390, _$E) of i from the header(s) included
// by HeaderStatics_diagonal_init_00608250.cpp; no source of its own.


// Dynamic initializer (0x006083C0, _$E) of j from the header(s) included
// by HeaderStatics_diagonal_init_00608250.cpp; no source of its own.


// Dynamic initializer (0x006083F0, _$E) of k from the header(s) included
// by HeaderStatics_diagonal_init_00608250.cpp; no source of its own.


// Dynamic initializer (0x00608400, _$E) of l from the header(s) included
// by HeaderStatics_diagonal_init_00608250.cpp; no source of its own.


// Dynamic initializer (0x00608420, _$E) of static_rect from the header(s) included
// by HeaderStatics_diagonal_init_00608250.cpp; no source of its own.




#include "shared/particle.hpp"
#include "shared/map_view.hpp"
#include "shared/cd_timer.hpp"
#include "shared/crt_memory.hpp"
#include "shared/map_cell_view.hpp"
#include "shared/rules.hpp"
#include "shared/matrix3d.hpp"
#include "shared/bounce.hpp"
#include "shared/tunnel_locomotion.hpp"
#include "shared/anim.hpp"
#include "shared/building_type.hpp"
#define VECTOR_IMPLEMENT_DYNAMIC_ACCESSORS
#include "shared/particle_system.hpp"
#include "shared/logic.hpp"
#include "shared/unknown_globals.hpp"
#include "shared/id_source.hpp"
#include "shared/special.hpp"
#include "shared/tactical.hpp"
#include "shared/unknown_static_rect.hpp"
#define SHAPE_FILE_EMPTY_RECT unknown_static_rect
#include "shared/palette.hpp"
#include "shared/blit.hpp"
#include "shared/surface.hpp"
#include "shared/hicolor.hpp"
#define MIN(a,b) (((b)<(a))?(b):(a))


// ParticleClass::ParticleClass(ParticleTypeClass *type, CoordStruct const &coord, CoordStruct const &target, ParticleSystemClass *system) (0x00608440)
// A particle of a type at a coordinate heading for a target: the direction
// is the normalized difference, the lifetime and colour come from the type
// (with some randomness), and it is placed above the ground.

// The game's uniform random value in [a, b].
static inline double Random_Double(double a, double b)
{
    int value = unknown_id_source_a3d290->Random()(0, 0x7ffffffe);
    return (b - a) * (value * 4.656612877414201e-10) + a;
}

static inline Vector3 Normalized(Vector3 const &v)
{
    double length = unknown_sqrt_4ba410(v.X * v.X + v.Y * v.Y + v.Z * v.Z);
    if (length != 0.0) {
        return Vector3((float)(v.X / length), (float)(v.Y / length), (float)(v.Z / length));
    }
    return v;
}

ParticleClass::ParticleClass(ParticleTypeClass *type, CoordStruct const &coord, CoordStruct const &target,
                             ParticleSystemClass *system) :
    Class(type),
    unknown_b0(0, 0, 0),
    GasVelocity(0, 0, -1.0f),
    UnusedCoord1(0, 0, 0),
    Velocity(Class->Velocity),
    unknown_d8(target),
    unknown_e4(coord),
    unknown_f0(unknown_geometry_l),
    unknown_fc((float)(target.X - coord.X), (float)(target.Y - coord.Y), (float)(target.Z - coord.Z)),
    unknown_108((float)coord.X, (float)coord.Y, (float)coord.Z),
    System(system),
    unknown_11c(Class->StateAIAdvance),
    unknown_11d(0),
    unknown_11e(Class->StartStateAI),
    unknown_11f(Class->Translucency),
    unknown_120(false),
    unknown_121(false)
{
    AbstractClass::Create_ID();
    unknown_ParticleClass_list_00a35e28.Add(this);
    if (Class->BehavesLike == 1) {
        Velocity += (float)(unknown_id_source_a3d290->Random()() % 2);
    }
    unknown_11a = (short)Class->MaxDC;
    if (Class->BehavesLike == 4) {
        Lifetime = (unsigned short)(Class->MaxEC + abs(unknown_id_source_a3d290->Random()() % 10));
    } else {
        int random = unknown_id_source_a3d290->Random()();
        Lifetime = (unsigned short)(Class->MaxEC + abs(random % Class->MaxEC));
    }
    CoordStruct place(coord.X, coord.Y, coord.Z);
    if (place.Z <= Map.Get_Height_GL(coord)) {
        place.Z = Map.Get_Height_GL(coord);
    }
    Set_Coord(place);
    unknown_fc = Normalized(unknown_fc);
    if (Class->Normalized) {
        CoordStruct delta(coord.X - target.X, coord.Y - target.Y, coord.Z - target.Z);
        float ax = (float)abs((int)(unknown_fc.X * Velocity));
        float ay = (float)abs((int)(unknown_fc.Y * Velocity));
        float steps = 9999.0f;
        if (ax > 1e-6) {
            steps = (float)(abs(delta.X) / ax);
        }
        if (ay > 1e-6) {
            float other = (float)(abs(delta.Y) / ay);
            if (!(steps < other)) {
                steps = other;
            }
        }
        unknown_11c = (char)(steps / (Class->FinalDamageState + 1) + 1.0);
    }
    if (Class->ColorList.Count() > 0) {
        if (Class->StartColor1.Red == 0 && Class->StartColor1.Green == 0 && Class->StartColor1.Blue == 0
            && Class->StartColor2.Red == 0 && Class->StartColor2.Green == 0 && Class->StartColor2.Blue == 0) {
            Color = Class->ColorList[0];
        } else {
            Color = RGBClass(Class->StartColor1, Class->StartColor2, (float)Random_Double(0.0, 1.0));
        }
    }
    ObjectClass::Unlimbo(&place, 0);
}


// ParticleClass::ParticleClass() (0x00608980)
// An empty particle registered in the particle list.

ParticleClass::ParticleClass() :
    Class(0),
    unknown_b0(0, 0, 0),
    GasVelocity(0, 0, -1.0f),
    UnusedCoord1(0, 0, 0),
    Velocity(0),
    unknown_d8(unknown_geometry_l),
    unknown_e4(unknown_geometry_l),
    unknown_f0(unknown_geometry_l),
    unknown_fc(0, 0, 0),
    unknown_108(0, 0, 0),
    System(0),
    unknown_11c(0),
    unknown_11d(0),
    unknown_11e(0),
    unknown_11f(0),
    unknown_120(false),
    unknown_121(false)
{
    unknown_ParticleClass_list_00a35e28.Add(this);
}


// ParticleClass::~ParticleClass() (0x00608B20)
// Leaves the map (ObjectClass slot 52, qualified), then the particle list.

ParticleClass::~ParticleClass()
{
    ObjectClass::Limbo();
    Unknown_006ECB90(this, true);
    Class = 0;
    unknown_ParticleClass_list_00a35e28.Delete(this);
}


// void ParticleClass::Gas_Behavior_AI() (0x00608BB0)
// Smoke behaviour (TibSun ParticleClass::Gas_Behavior_AI): every other frame the
// drift jitters within +-5 and the smoke sinks, glancing off the ground, bridge
// decks and walls; then it damages what shares its cell and advances its state.

// TibSun Slope_Vector: reflects a vector off the slope of the cell at coord.
static inline Vector3 Slope_Vector(Vector3 &vector, CoordStruct const &coord, float length)
{
    Matrix3D slope_matrix;
    slope_matrix = *Unknown_00718D60(&Matrix3D(), unknown_tactical_83996c->Get_Cell_Ramp_006a7110(coord));
    Matrix3D slope_matrix_inv = Matrix3D_Rigid_Inverse_00591420(slope_matrix);
    Vector3 vec;
    vec.Set_00437690(vector.X, -vector.Y, vector.Z);
    vec = slope_matrix_inv.Rotate_Vector(vec);
    vec.Scale_004376b0(length);
    vec.Z = -vec.Z;
    vec = slope_matrix.Rotate_Vector(vec);
    vector.X = vec.X;
    vector.Y = -vec.Y;
    vector.Z = vec.Z;
    return vector;
}

// The height of a bridge deck over coord; ground receives the ground height.
static inline int Bridge_Height(CoordStruct const &coord, int &ground)
{
    int height = Map.Get_Height_GL(coord);
    ground = height;
    return height + unknown_geometry_j;
}

// Keeps a value within [low, high].
static inline void Limit(int &value, int low, int high)
{
    if (value < low) {
        value = low;
    } else if (value > high) {
        value = high;
    }
}

void ParticleClass::Gas_Behavior_AI()
{
    if ((Frame & 1) == 0) {
        CoordStruct coord = Coord;
        if ((abs(unknown_id_source_a3d290->Random()()) & 7) == 0) {
            int dx = 0;
            int dy = 0;
            if ((abs(unknown_id_source_a3d290->Random()()) & 1) == 0) {
                dx = abs(unknown_id_source_a3d290->Random()()) % 3 - 1;
            } else {
                dy = abs(unknown_id_source_a3d290->Random()()) % 3 - 1;
            }
            unknown_b0.X += dx;
            unknown_b0.Y += dy;
            Limit(unknown_b0.X, -5, 5);
            Limit(unknown_b0.Y, -5, 5);
        }

        Vector3 position(coord.X, coord.Y, coord.Z);
        Vector3 &velocity = GasVelocity;
        velocity = Vector3(0, 0, -2);
        double gravity = Rules->unknown_1320;
        velocity.Z -= gravity;

        CoordStruct old_coord;
        old_coord.Set_00434820(position.X, position.Y, position.Z);
        position.Add_004376e0(velocity);
        CoordStruct coord2;
        coord2.Set_00434820(position.X, position.Y, position.Z);

        int height_gl;
        int height_bridge = Bridge_Height(coord2, height_gl);

        CellClass *cellptr = &Map[coord2];
        bool start_under_bridge = false;
        bool end_under_bridge = false;
        if ((cellptr->unknown_140 & 0x100) || (Map[old_coord].unknown_140 & 0x100)) {
            if (coord2.Z >= height_bridge) {
                if (old_coord.Z < height_bridge) {
                    end_under_bridge = true;
                }
            } else {
                if (old_coord.Z >= height_bridge) {
                    start_under_bridge = true;
                }
            }
        }

        bool blocked = false;
        if (!start_under_bridge && !end_under_bridge) {
            if (position.Z >= (double)height_gl && position.Z - 150.0f < (double)height_gl) {
                BuildingClass *building = static_cast<BuildingClass *>(cellptr->Cell_Building());
                if (building || cellptr->Has_Wall_Or_Gate(-1, -1)) {
                    blocked = true;
                    if (building) {
                        if (building->Type->LaserFence && building->unknown_4ec >= 8) {
                            blocked = false;
                        }
                        if (building->unknown_slot31()) {
                            blocked = false;
                        }
                    }
                } else {
                    blocked = false;
                }
            }
        }

        if (position.Z < (float)height_gl || start_under_bridge || end_under_bridge || blocked) {
            if (start_under_bridge) {
                position.Z = height_bridge;
            } else if (end_under_bridge) {
                position.Z = height_bridge - 20;
            } else if ((float)(height_gl - 100) < position.Z) {
                position.Z = (float)height_gl;
            }
            Slope_Vector(velocity, coord2, 1);
        }

        if (Get_Height_AGL() > 5 && (Frame % 2) == 0) {
            unknown_b0.Z = (-5 > unknown_b0.Z - 1) ? -5 : unknown_b0.Z - 1;
        } else {
            unknown_b0.Z = (unknown_b0.Z > 0) ? unknown_b0.Z : 0;
        }
    }

    unknown_11a--;

    if (unknown_11a == 0 && Class->Damage) {
        unknown_11a = Class->MaxDC;
        ObjectClass *occupier = Map[Position_Coord_004f3f40()].OccupierPtr;
        while (occupier) {
            CoordStruct delta = Position_Coord_004f3f40() - occupier->Position_Coord_004f3f40();
            ObjectClass *next = occupier->unknown_30;
            if (occupier->unknown_7d) {
                if (occupier->Strength > 0) {
                    int damage = Class->Damage;
                    occupier->Take_Damage(&damage, Unknown_006A2970(abs(delta.X) + abs(delta.Y)), Class->Warhead, 0, 0, 0);
                }
            }
            occupier = next;
        }
    }

    if ((Fetch_ID() + (Class->MaxEC - Lifetime)) % ((Fetch_ID() & 1) + Class->StateAIAdvance) == 0) {
        unknown_11e++;
    }
    if (unknown_11e == Class->EndStateAI) {
        if (Class->DeleteOnStateLimit) {
            unknown_121 = true;
            return;
        }
        unknown_11e = 0;
    }
}


// Original immutable double at0x007A10F8: independently decoded value0.05.
extern "C" double const Particle_Color_Step_007a10f8;
static inline void Advance_Particle_Position(Vector3 &position, Vector3 const &step) { position.X = step.X + position.X; position.Y = step.Y + position.Y; position.Z = step.Z + position.Z; }
void ParticleClass::Railgun_Behavior_AI()
{
    Vector3 direction(unknown_fc.X, unknown_fc.Y, unknown_fc.Z);
    float speed = Velocity;
    Vector3 step; step.X = direction.X * speed; step.Y = direction.Y * speed; step.Z = direction.Z * speed;
    Velocity += (unknown_id_source_a3d290->Random()(0, 0x7ffffffe) * 4.656612877414201e-10 - 0.5) * 0.1;
    Advance_Particle_Position(unknown_108, step);
    CoordStruct coord; coord.X = (int)unknown_108.X; coord.Y = (int)unknown_108.Y; coord.Z = (int)unknown_108.Z;
    Set_Coord(coord);
    double color_change = unknown_id_source_a3d290->Random()(0, 0x7ffffffe) * 4.656612877414201e-10;
    color_change *= Particle_Color_Step_007a10f8;
    color_change += Class->ColorSpeed;
    ColorState.Fraction = color_change + ColorState.Fraction;
    if (ColorState.Fraction > 1.0) {
        if (ColorState.ColorIndex < Class->ColorList.Count() - 2) {
            ++ColorState.ColorIndex;
            ColorState.Fraction = 0;
            return;
        }
        ColorState.Fraction = 1.0;
    }
}


// void ParticleClass::Smoke_Behavior_AI() (0x006093A0)
// Behaviour 1 (gas): every other frame the drift jitters within +-5, the
// state frame advances and the speed decays.

// Limit: defined by the 0x00608BB0 source earlier in the unit.
void ParticleClass::Smoke_Behavior_AI()
{
    if (Frame & 1) {
        if ((abs(unknown_id_source_a3d290->Random()()) & 3) == 0) {
            int dx = 0;
            int dy = 0;
            if ((abs(unknown_id_source_a3d290->Random()()) & 1) == 0) {
                dx = abs(unknown_id_source_a3d290->Random()()) % 3 - 1;
            } else {
                dy = abs(unknown_id_source_a3d290->Random()()) % 3 - 1;
            }
            unknown_b0.X += dx;
            unknown_b0.Y += dy;
            Limit(unknown_b0.X, -5, 5);
            Limit(unknown_b0.Y, -5, 5);
        }
        unknown_b0.Z = 0;
    }
    if (unknown_11e < Class->EndStateAI) {
        if ((Fetch_ID() + (Class->MaxEC - Lifetime)) % ((Fetch_ID() & 1) + Class->StateAIAdvance) == 0) {
            unknown_11e++;
        }
        if (unknown_11e == Class->EndStateAI && Class->DeleteOnStateLimit) {
            unknown_121 = true;
        }
    }
    if (Velocity > 3.0) {
        Velocity -= Class->Deacc;
    }
}


static inline ObjectClass *Fire_Content_At_Height(CellClass &cell, CoordStruct coord) { if (coord.Z >= (cell.unknown_11b + 4) * unknown_geometry_g) return cell.AltOccupierPtr; return cell.OccupierPtr; }
static inline int Particle_Distance(CoordStruct const &delta) { return (int)unknown_sqrt_4ba410((double)delta.Y * delta.Y + (double)delta.Z * delta.Z + (double)delta.X * delta.X); }
void ParticleClass::Fire_Behavior_AI()
{
    if (Velocity > 0.0) {
    float jitter = (unknown_id_source_a3d290->Random()() % 10 - 5) * 0.02 + 1.0;
    Vector3 step(unknown_fc.X * jitter, unknown_fc.Y * jitter, unknown_fc.Z * jitter);
    int dx = (int)(step.X * Velocity);
    int dy = (int)(step.Y * Velocity);
    int dz = (int)(step.Z * Velocity);
    unknown_f0 = CoordStruct(dx, dy, dz);
    if (unknown_11e < Class->EndStateAI) {
        if ((Fetch_ID() + (Class->MaxEC - Lifetime)) % ((Fetch_ID() & 1) + unknown_11c) == 0) {
            ++unknown_11e;
            if (unknown_11e == Class->Translucent25State) unknown_11f = 25;
            if (unknown_11e == Class->Translucent50State) unknown_11f = 50;
        }
        if (unknown_11e == Class->EndStateAI && Class->DeleteOnStateLimit) {
            goto particle_done;
        }
    }
    Velocity -= Class->Deacc;
    unknown_11a--;
    if (unknown_11a == 0 && Class->Damage != 0 && unknown_11e <= Class->FinalDamageState) {
        unknown_11a = (short)Class->MaxDC;
        CellClass &cell = Map[CoordStruct(Coord)];
        ObjectClass *target;
        if (cell.unknown_140 & 0x100) target = Fire_Content_At_Height(cell, Coord);
        else target = cell.OccupierPtr;
        while (target) {
            CoordStruct delta = target->Coord;
            delta = Center_Coord() - delta;
            int distance = Particle_Distance(delta);
            int damage = Class->Damage;
            if (target->Strength > 0 && target->unknown_7d && target != System->Owner) {
                target->Take_Damage(&damage, distance / 10, Class->Warhead, 0, false, 0);
            }
            target = target->unknown_30;
        }
    }
        return;
    }
    Velocity = 0;
particle_done:
    unknown_121 = true;
}


// Not exact (RA2_NON_MATCHING): ParticleClass::unknown_func_609540 at 0x00609540, best score 66.45, 2026-11-13; differs: 306 instructions in the original, 308 in the draft (shape 96.74)
#ifdef RA2_NON_MATCHING
// void ParticleClass::unknown_func_609540() (0x00609540)
// Spark behaviour (TibSun ParticleClass::Spark_Behavior_AI): the spark falls
// under gravity until it meets the ground, a bridge deck or a wall, where it
// glances off along the cell slope and is flagged done; its colour then moves
// a step further along the type's colour list.
#include "shared/unknown_vector3.hpp"
// Original immutable double at 0x007A10F8 (0.05).
extern "C" double const Particle_Color_Step_007a10f8;

void ParticleClass::unknown_func_609540()
{
    CoordStruct coord = Coord;
    unknown_fc.Z -= Rules->unknown_1320;
    Vector3 velocity(unknown_fc.X, unknown_fc.Y, unknown_fc.Z);
    Vector3 position(coord.X, coord.Y, coord.Z);
    double gravity = Rules->unknown_1320;
    velocity.Z -= gravity;

    CoordStruct old_coord;
    old_coord.Set_00434820(position.X, position.Y, position.Z);
    position.Add_004376e0(velocity);
    CoordStruct coord2;
    coord2.Set_00434820(position.X, position.Y, position.Z);

    int height_gl;
    int height_bridge = Bridge_Height(coord2, height_gl);

    CellClass *cellptr = &Map[coord2];
    bool start_under_bridge = false;
    bool end_under_bridge = false;
    if ((cellptr->unknown_140 & 0x100) || (Map[old_coord].unknown_140 & 0x100)) {
        if (coord2.Z >= height_bridge) {
            if (old_coord.Z < height_bridge) {
                end_under_bridge = true;
            }
        } else {
            if (old_coord.Z >= height_bridge) {
                start_under_bridge = true;
            }
        }
    }

    bool blocked = false;
    if (!start_under_bridge && !end_under_bridge) {
        if (position.Z >= (double)height_gl && position.Z - 150.0f < (double)height_gl) {
            BuildingClass *building = static_cast<BuildingClass *>(cellptr->Cell_Building());
            if (building || cellptr->Has_Wall_Or_Gate(-1, -1)) {
                blocked = true;
                if (building) {
                    if (building->Type->LaserFence && building->unknown_4ec >= 8) {
                        blocked = false;
                    }
                    if (building->unknown_slot31()) {
                        blocked = false;
                    }
                }
            } else {
                blocked = false;
            }
        }
    }

    float ground_z = (float)height_gl;
    bool sloped = true;
    if (position.Z >= ground_z) {
        if (!start_under_bridge && !end_under_bridge && !blocked) {
            sloped = false;
        }
    }
    if (sloped) {
        if (start_under_bridge) {
            position.Z = height_bridge;
        } else if (end_under_bridge) {
            position.Z = (float)(height_bridge - 20);
        } else if ((float)(height_gl - 100) < position.Z) {
            position.Z = ground_z;
        }
        velocity = Slope_Vector(velocity, coord2, 1);
        unknown_121 = true;
    }

    Set_Coord((CoordStruct const &)UnknownVector3(position.X, position.Y, position.Z));
    double color_change = unknown_id_source_a3d290->Random()(0, 0x7ffffffe) * 4.656612877414201e-10;
    color_change *= Particle_Color_Step_007a10f8;
    color_change += Class->ColorSpeed;
    ColorState.Fraction = color_change + ColorState.Fraction;
    if (ColorState.Fraction > 1.0) {
        if (ColorState.ColorIndex < Class->ColorList.Count() - 2) {
            ColorState.ColorIndex++;
            ColorState.Fraction = 0;
        } else {
            ColorState.Fraction = 1.0;
        }
    }
}
#endif

// void ParticleClass::Behavior_AI() (0x00609C90)
// Runs the behaviour routine of the type's BehavesLike value, then counts
// the lifetime down and flags the particle done at zero.

void ParticleClass::Behavior_AI()
{
    switch (Class->BehavesLike) {
    case 0:
        Gas_Behavior_AI();
        break;
    case 1:
        Smoke_Behavior_AI();
        break;
    case 2:
        Fire_Behavior_AI();
        break;
    case 3:
        unknown_func_609540();
        break;
    case 4:
        Railgun_Behavior_AI();
        break;
    }
    Lifetime--;
    if (Lifetime == 0) {
        unknown_121 = true;
    }
}


// void ParticleClass::unknown_slot67(Point2D const &point, Rect const &cliprect) (0x00609D10, slot 67)
// Draws the particle (TibSun ParticleClass::Draw_It) at the screen point point
// within the window cliprect, unless the frame rate is too low or it is hidden
// under fog. Shape particles draw their frame lifted by their height, with
// the type's translucency at full detail; sparks and railgun particles put
// one pixel blended along the type's colour list, dimmed by the alpha buffer
// and hidden behind closer depth.

void ParticleClass::unknown_slot67(Point2D const &point, Rect const &cliprect)
{
    if (unknown_a6eb1c < Unknown_0053D840()) {
        return;
    }
    if (unknown_a40b2c != 0 || Class->BehavesLike != 1 && Class->BehavesLike != 3) {
        if (!Debug_Map && unknown_b263d8 != 0 && unknown_id_source_a3d290->Special().FogOfWar && Map.Is_Fogged(Position_Coord_004f3f40())) {
            return;
        }

        if (Class->BehavesLike != 3 && Class->BehavesLike != 4) {
            int height_offset = -15 - Unknown_006A2970(Get_Height());
            ShapeFileStruct const *shape = (ShapeFileStruct const *)Get_Image_Data();
            if (shape != 0) {
                int flags = 0x2800;
                int shapenum = Shape_Number();
                if (unknown_a40b2c == 2) {
                    if (unknown_11f == 25) {
                        flags = 0x2802;
                    } else if (unknown_11f == 50) {
                        flags = 0x2804;
                    } else if (unknown_11f >= 75) {
                        flags = 0x2806;
                    }
                }
                CC_Draw_Shape(unknown_surface_83995c, Unit_Draw_Convert_008323C0, shape, shapenum, point, cliprect, flags | 0xE00, 0, height_offset, 2, 1000, 0, 0, Point2D(0, 0));
            }
        } else {
            Point2D pixel;
            unknown_tactical_83996c->Coord_To_Pixel(Position_Coord_004f3f40(), pixel);
            pixel.Y += unknown_int_839634;
            if (cliprect.Is_Point_Within(pixel)) {
                Point2D alpha_point = pixel - Point2D(0, AlphaBuffer->Area.Y);
                unsigned short *alpha_ptr = AlphaBuffer->Get_Buffer_Offset(alpha_point);
                int alpha = *alpha_ptr;
                if (alpha != 0) {
                    Point2D depth_point = pixel - Point2D(0, DepthBuffer->Area.Y);
                    int zdepth = (unsigned short)(DepthBuffer->Area.Y + DepthBuffer->MaxValue - pixel.Y) - Unknown_006A2970(Position_Coord_004f3f40().Z) - 50;
                    int depth = *DepthBuffer->Get_Buffer_Offset(depth_point);
                    if (zdepth < depth) {
                        RGBClass color1 = ColorState.ColorIndex == 0 ? Color : Class->ColorList[ColorState.ColorIndex];
                        RGBClass color2 = Class->ColorList[ColorState.ColorIndex + 1];
                        int red = color1.Red * (1.0 - ColorState.Fraction) + color2.Red * ColorState.Fraction;
                        int green = color1.Green * (1.0 - ColorState.Fraction) + color2.Green * ColorState.Fraction;
                        int blue = color1.Blue * (1.0 - ColorState.Fraction) + color2.Blue * ColorState.Fraction;
                        if (alpha < 127) {
                            red = (int)(red * alpha) >> 7;
                            green = (int)(green * alpha) >> 7;
                            blue = (int)(blue * alpha) >> 7;
                        }
                        unknown_surface_83995c->Put_Pixel(pixel, (unsigned short)Build_Hicolor_Pixel(red, green, blue));
                    }
                }
            }
        }
    }
}


static inline Point2D Smoke_Wind_Offset(int const &direction) { Point2D offset; offset.X = unknown_wind_7eb7e4[0][direction]; offset.Y = unknown_wind_7eb7e4[1][direction]; return offset; }
void ParticleClass::Smoke_Motion_AI()
{
    int frame = Frame;
    CoordStruct next = Coord;
    if (Class->WindEffect > 0 && frame % (10 / Class->WindEffect) == 0) {
        Point2D offset = Smoke_Wind_Offset(Rules->unknown_6d4);
        next.X += offset.X; next.Y += offset.Y;
    }
    if (frame & 1) {
        next.X = (int)(next.X + GasVelocity.X);
        next.Y = (int)(next.Y + GasVelocity.Y);
        int height = Map.Get_Height_GL(next);
        if (next.Z > height + 5) {
            next.Z -= MIN(2, next.Z - height - 5);
        }
        next.X += unknown_b0.X;
        next.Y += unknown_b0.Y;
        next.Z += unknown_b0.Z;
        if (next.Z < Map.Get_Height_GL(next) + 5) next.Z = Map.Get_Height_GL(next) + 5;
    }
    Set_Coord(next);
}


static inline Point2D Wind_Offset(int const &direction, int effect) { Point2D offset; offset.X = unknown_wind_7eb824[0][direction] * effect; offset.Y = unknown_wind_7eb824[1][direction] * effect; return offset; }
void ParticleClass::Gas_Motion_AI()
{
    CoordStruct next = Coord;
    CoordStruct coord = Coord;
    int effect = Class->WindEffect;
    
    Point2D offset = Wind_Offset(Rules->unknown_6d4, effect);
    next.X += offset.X; next.Y += offset.Y;
    next.Z += (int)Velocity;
    next.X += unknown_b0.X;
    next.Y += unknown_b0.Y;
    next.Z += unknown_b0.Z;
    if (Map[coord].unknown_140 & 0x100) {
        int height = Map.Get_Height_GL(coord) + unknown_geometry_j;
        if (coord.Z < height && next.Z >= (double)height - unknown_geometry_g * 2.5) {
            unknown_121 = true;
            return;
        }
    }
    Set_Coord(next);
}


// void ParticleClass::Fire_Motion_AI() (0x0060A350; inlined into
// 0x0060A420, never called)
// Movement of behaviour 2: a moving particle (Velocity > 0) steps by
// unknown_f0 and is marked done (unknown_11d, unknown_121) when the ground
// under the new position is higher than under the old one; then the
// position is set (slot 103).

void ParticleClass::Fire_Motion_AI()
{
    CoordStruct next = Coord;
    CoordStruct coord = Coord;
    if (Velocity > 0.0) {
        next.X += unknown_f0.X;
        next.Y += unknown_f0.Y;
        next.Z += unknown_f0.Z;
    }
    if (Velocity > 0.0) {
        int height = Map.Get_Height_GL(coord);
        if (Map.Get_Height_GL(next) > height) {
            unknown_11d = 1;
            unknown_121 = true;
        }
    }
    Set_Coord(next);
}


// void ParticleClass::Motion_AI() (0x0060A420)
// Moves the particle by its type's behaviour (0, 1 or 2; others do not
// move here).

void ParticleClass::Motion_AI()
{
    switch (Class->BehavesLike) {
    case 0:
        Smoke_Motion_AI();
        break;
    case 1:
        Gas_Motion_AI();
        break;
    case 2:
        Fire_Motion_AI();
        break;
    }
}


// bool ParticleClass::Mark(int arg1) (0x0060A530)
// The ObjectClass result as a truth value.

bool ParticleClass::Mark(int arg1)
{
    return ObjectClass::Mark(arg1) ? true : false;
}


// Cell const *ParticleClass::Occupy_List(int arg1) (0x0060A550)
// No cell: a function-local static (0x7FFF, 0x7FFF).
// (the guard's number follows the unit's header contents: 42 with the
// proposed abstract.hpp, 40 with the current one)

Cell const *ParticleClass::Occupy_List(int arg1)
{
    static UnknownStaticCell const unknown_none(Cell(0x7FFF, 0x7FFF));
    return &unknown_none.Cell;
}


// Exit thunk of the function-local static in ParticleClass::Occupy_List
// (0x0060A5A0)
// Compiler generated ($E) right after unknown_slot64: UnknownStaticCell's empty
// destructor, registered with atexit on first use.


// int ParticleClass::In_Which_Layer() const (0x0060A5B0)
// The layer (3).

int ParticleClass::In_Which_Layer() const
{
    return 3;
}


// int __fastcall Unknown_0060A5C0(Point2D const &point) (0x0060A5C0)
// Half the sum of the absolute coordinates (never called: expanded wherever
// the unit uses it).

int __fastcall Unknown_0060A5C0(Point2D const &point)
{
    return (abs(point.X) + abs(point.Y)) >> 1;
}


// HRESULT __stdcall ParticleClass::Load(IStream *stream) (0x0060A5E0)

HRESULT __stdcall ParticleClass::Load(IStream *stream)
{
    HRESULT result = ObjectClass::Load(stream);
    if (FAILED(result)) {
        return result;
    }
    new (this) ParticleClass(NoInitClass());
    Swizzler.Swizzle((void **)&Class);
    Swizzler.Swizzle((void **)&System);
    return S_OK;
}


// HRESULT __stdcall ParticleClass::Save(IStream *stream, BOOL clear_dirty) (0x0060A650)
// Saves through AbstractClass::Save and marks the particle saved (+0x120).

HRESULT __stdcall ParticleClass::Save(IStream *stream, BOOL clear_dirty)
{
    HRESULT result = AbstractClass::Save(stream, clear_dirty);
    unknown_120 = true;
    return result;
}


// int ParticleClass::Shape_Number() (0x0060A670)
// The shape frame: behaviours 0 and 1 use unknown_11e; behaviour 2 adds the
// type's per-direction frame count (+0x2D9) times 0..3 by the screen
// direction from the start (unknown_e4) to the target (unknown_d8).

int ParticleClass::Shape_Number()
{
    switch (Class->BehavesLike) {
    case 0:
    case 1:
        return unknown_11e;
    case 2:
        {
            Point2D from;
            Point2D to;
            unknown_tactical_83996c->Coord_To_Pixel(unknown_e4, from);
            unknown_tactical_83996c->Coord_To_Pixel(unknown_d8, to);
            int direction = Unknown_00721DB0(from, to);
            if (direction == 2 || direction == 6) {
                return unknown_11e + Class->EndStateAI * 2;
            }
            if (direction == 4 || direction == 0) {
                return unknown_11e;
            }
            if (direction == 1 || direction == 5) {
                return Class->EndStateAI + unknown_11e;
            }
            if (direction == 3 || direction == 7) {
                return Class->EndStateAI * 3 + unknown_11e;
            }
        }
        break;
    }
    return 0;
}


// HRESULT __stdcall ParticleClass::GetClassID(GUID *clsid) (0x0060A770)
// Compiler-emitted from shared/particle.hpp in this unit; no source of its own.


// int ParticleClass::Size_Of() const (0x0060A7B0)
// Compiler-emitted from shared/particle.hpp in this unit; no source of its own.


// int ParticleClass::Kind_Of() const (0x0060A7C0)
// Compiler-emitted from shared/particle.hpp in this unit; no source of its own.


// ObjectTypeClass *ParticleClass::Class_Of() const (0x0060A7D0)
// Compiler-emitted from shared/particle.hpp in this unit; no source of its own.
