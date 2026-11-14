// rocket.cpp: the original translation unit re-filed as one source (Route A): the accepted
// bodies of its canonical compile group in address order, under the unit's own header
// state (its switches and includes in their original order). The module map
// manifests/original_target/modules/rocket.json carries the addresses and the rows that
// stay in their own files.


#define RA2_SHIP_MOVE_TO_VALUE  // ILocomotion::Move_To takes the COM coordinate (shared/ilocomotion.hpp): Move_To 0x0063EA80
#define RA2_COORD_TPOINT3D
#define RA2_COORD_LENGTH
#include "shared/unknown_geometry.hpp"
#include "shared/unknown_static_rect.hpp"
#include "shared/rocket_locomotion.hpp"
#include "shared/fast_math.hpp"
#include "shared/facing.hpp"
#include "shared/rules.hpp"
#include "shared/cell_class.hpp"
#include "shared/point.hpp"
#include "shared/aircraft.hpp"
#include "shared/anim.hpp"
#include "shared/anim_type.hpp"
#include "shared/sound_play.hpp"
#include "shared/foot.hpp"
#include "shared/techno_type.hpp"
#include "shared/infantry.hpp"
#include "shared/techno.hpp"
#include "shared/matrix3d.hpp"


// CoordStruct on TPoint3D (RocketLocomotionClass 0x0063DE10 square sum and Process 0x0063DE90).
// Header-static dynamic initializers of the unit at 0x0063D8D0:  the
// shared/unknown_geometry.hpp constants and unknown_static_rect.hpp
// (0x0063D8D0-0x0063DAB6); the other
// drafts of this plan are emitted by these includes.
// RocketLocomotionClass::Process (0x0063DE90) calls CoordStruct::Length:
// the guard has to be set before this unit first includes shared/coord.hpp.


// Dynamic initializer (0x0063D900, _$E) of b from the header(s) included
// by HeaderStatics_diagonal_init_0063d8d0.cpp; no source of its own.


// Dynamic initializer (0x0063D920, _$E) of c from the header(s) included
// by HeaderStatics_diagonal_init_0063d8d0.cpp; no source of its own.


// Dynamic initializer (0x0063D940, _$E) of angle45 from the header(s) included
// by HeaderStatics_diagonal_init_0063d8d0.cpp; no source of its own.


// Dynamic initializer (0x0063D960, _$E) of angle60 from the header(s) included
// by HeaderStatics_diagonal_init_0063d8d0.cpp; no source of its own.


// Dynamic initializer (0x0063D980, _$E) of angle90 from the header(s) included
// by HeaderStatics_diagonal_init_0063d8d0.cpp; no source of its own.


// Dynamic initializer (0x0063D9A0, _$E) of g from the header(s) included
// by HeaderStatics_diagonal_init_0063d8d0.cpp; no source of its own.


// Dynamic initializer (0x0063D9E0, _$E) of h from the header(s) included
// by HeaderStatics_diagonal_init_0063d8d0.cpp; no source of its own.


// Dynamic initializer (0x0063DA10, _$E) of i from the header(s) included
// by HeaderStatics_diagonal_init_0063d8d0.cpp; no source of its own.


// Dynamic initializer (0x0063DA40, _$E) of j from the header(s) included
// by HeaderStatics_diagonal_init_0063d8d0.cpp; no source of its own.


// Dynamic initializer (0x0063DA70, _$E) of k from the header(s) included
// by HeaderStatics_diagonal_init_0063d8d0.cpp; no source of its own.


// Dynamic initializer (0x0063DA80, _$E) of l from the header(s) included
// by HeaderStatics_diagonal_init_0063d8d0.cpp; no source of its own.


// Dynamic initializer (0x0063DAA0, _$E) of static_rect from the header(s) included
// by HeaderStatics_diagonal_init_0063d8d0.cpp; no source of its own.


// RocketLocomotionClass::RocketLocomotionClass() (0x0063DAC0)

RocketLocomotionClass::RocketLocomotionClass() :
    unknown_18(unknown_geometry_l),
    unknown_40(0),
    unknown_48(0),
    unknown_50(0),
    unknown_54(0)
{
}


// RocketLocomotionClass::~RocketLocomotionClass() (0x0063DB30)

RocketLocomotionClass::~RocketLocomotionClass()
{
}


// bool __stdcall RocketLocomotionClass::Is_Moving() (0x0063DB50, ILocomotion 4)
// A destination is set.

bool __stdcall RocketLocomotionClass::Is_Moving()
{
    return unknown_18 != unknown_geometry_l;
}


// bool __stdcall RocketLocomotionClass::Is_Moving_Now() (0x0063DB90, ILocomotion 32)
// States 3 to 5.

bool __stdcall RocketLocomotionClass::Is_Moving_Now()
{
    if (unknown_40 >= 3 && unknown_40 <= 5) {
        return true;
    }
    return false;
}


// CoordStruct __stdcall RocketLocomotionClass::Destination() (0x0063DBB0, ILocomotion 5)

CoordStruct __stdcall RocketLocomotionClass::Destination()
{
    return CoordStruct(unknown_18.X, unknown_18.Y, unknown_18.Z);
}


// CoordStruct RocketLocomotionClass::unknown_63dbe0(int distance) (0x0063DBE0)
// The point `distance` leptons ahead of the rocket along its facing and
// pitch (+0x50): Westwood's Move_Coord by the ground part of the distance,
// raised by the vertical part.

// Westwood's Move_Coord (TibSun inline.h): a coordinate moved along a direction.
static inline CoordStruct Move_Coord(CoordStruct const &start, UnknownFacingDir const &dir, int distance)
{
    double radians = ((short)dir.Raw - 0x3fff) * -9.587672516830327e-05;
    return CoordStruct(start.X + Fast_Cos(radians) * (double)distance, start.Y - Fast_Sin(radians) * (double)distance, start.Z);
}

CoordStruct RocketLocomotionClass::unknown_63dbe0(int distance)
{
    CoordStruct coord = Object_Coord(LinkedTo);
    CoordStruct result = Move_Coord(coord, LinkedTo->unknown_2e0.Current(), Fast_Cos(unknown_50) * (double)distance);
    result.Z += Fast_Sin(unknown_50) * (double)distance;
    return result;
}


// bool RocketLocomotionClass::unknown_63dcf0(RocketStruct const *rocket) (0x0063DCF0)
// Detonates (0x0063E7D0) when the rocket's nose (BodyLength ahead) is down
// to the destination height, or to the bridge-deck height above it when
// the rocket is over a bridge cell at the destination height. True when it
// detonated. The nested ifs keep it out of line in Process (0x0063DE90),
// as in the original; one && chain compiles the same body but inlines it.

bool RocketLocomotionClass::unknown_63dcf0(RocketStruct const *rocket)
{
    int z = unknown_63dbe0(rocket->BodyLength).Z;
    if (z <= unknown_18.Z) {
        unknown_63e7d0();
        return true;
    }
    CellClass *cell = static_cast<CellClass *>(LinkedTo->Get_Cell_Ptr());
    if (cell != 0) {
        if (cell->unknown_140 & 0x100) {
            if (cell->Center_Coord().Z == unknown_18.Z) {
                if (z <= unknown_18.Z + unknown_geometry_j) {
                    unknown_63e7d0();
                    return true;
                }
            }
        }
    }
    return false;
}


// double RocketLocomotionClass::unknown_63de10() (0x0063DE10)

static inline int Square(int x) { return x * x; }

// double RocketLocomotionClass::unknown_63de10() (0x0063DE10): the pitch from
// the rocket to its destination, the arctangent of the height over the ground
// distance (straight down with no ground distance); TibSun/YR
// RocketLocomotionClass::Pitch_To_Target (ra2-decomp rocket.cpp 0x00662240).
// An ordinary member: this copy has no callers, Process auto-inlines its two
// calls. Its square sum needs CoordStruct on TPoint3D (RA2_COORD_TPOINT3D in
// the unit's first row).
double RocketLocomotionClass::unknown_63de10()
{
    CoordStruct coord = Object_Coord(LinkedTo);
    CoordStruct delta = unknown_18 - coord;
    float distance = Fast_Sqrt((double)(Square(delta.X) + Square(delta.Y)));
    if (distance > 0.0) {
        return Fast_Atan((double)(delta.Z / distance));
    }
    return -1.5707963267948966;
}


// bool RocketLocomotionClass::Process() (0x0063DE90)
// The rocket state machine (+0x3C): 1 pause, 2 tilt from PitchInitial to
// PitchFinal over TiltFrames then launch (take-off anim and AuxSound1),
// 3 accelerate and climb to Altitude, 4 cruise (lazy-curve pitch blend or
// level flight until the dive point), 5 dive toward the destination; then
// trail anim, move along facing and pitch, and detonate on zero strength.
// False when the rocket detonated at the target (0x0063DCF0). Needs
// RA2_COORD_LENGTH (CoordStruct::Length), defined at the top of the unit.
// Its pitch toward the destination (0x0063DE10, the row before) is inlined here.

// The rocket's take-off and trail anim names (char const * variables).
extern char const *unknown_rocket_takeoff_anim_7ee804;  // 0x007EE804
extern char const *unknown_rocket_trail_anim_7ee808;    // 0x007EE808

// Each LinkedTo->Center_Coord() below goes through a file-static helper:
// temporaries in inlined helpers share stack slots, temporaries in different
// cases of one switch do not (docs/VC6_MATCHING_NOTES.md).

// Westwood's Direction (TibSun inline.h): the facing from coord1 to coord2.
static inline UnknownFacingDir Direction_To(CoordStruct const &coord1, CoordStruct const &coord2)
{
    int x1 = coord1.X;
    int x2 = coord2.X;
    return UnknownFacingDir((unsigned short)(int)((Fast_Atan2((double)coord1.Y - (double)coord2.Y, (double)x2 - (double)x1) - 1.5707963267948966) * -10430.060040584269));
}

// Ground (x/y) distance from `to` to `from`. The take-off reading (case 3)
// sums y*y + x*x, the lazy-curve reading x*x + y*y (Ground_Distance2): the
// original's two inlined copies load the components in opposite orders.
static inline int Ground_Distance(TPoint2D<int> const &to, CoordStruct const &from)
{
    TPoint2D<int> delta = TPoint2D<int>(from.X, from.Y) - to;
    return (int)Fast_Sqrt((double)delta.Y * (double)delta.Y + (double)delta.X * (double)delta.X);
}

static inline int Ground_Distance(CoordStruct const &to, ObjectClass *object)
{
    return Ground_Distance(TPoint2D<int>(to.X, to.Y), object->Center_Coord());
}

static inline int Ground_Distance2(TPoint2D<int> const &to, CoordStruct const &from)
{
    TPoint2D<int> delta = TPoint2D<int>(from.X, from.Y) - to;
    return (int)Fast_Sqrt((double)delta.X * (double)delta.X + (double)delta.Y * (double)delta.Y);
}

static inline int Ground_Distance2(CoordStruct const &to, ObjectClass *object)
{
    return Ground_Distance2(TPoint2D<int>(to.X, to.Y), object->Center_Coord());
}

static inline UnknownFacingDir Direction_To(ObjectClass *object, CoordStruct const &to)
{
    return Direction_To(object->Center_Coord(), to);
}

static inline int Distance(CoordStruct const &coord1, CoordStruct const &coord2)
{
    return (coord1 - coord2).Length();
}

static inline int Distance_To(ObjectClass *object, CoordStruct const &to)
{
    return Distance(object->Center_Coord(), to);
}
bool __stdcall RocketLocomotionClass::Process()
{
    RocketStruct const *rocket;
    if (((AircraftClass *)LinkedTo)->Type == Rules->V3Rocket.Type) {
        rocket = &Rules->V3Rocket;
    } else {
        rocket = &Rules->DMisl;
    }
    bool exploded = false;
    switch (unknown_40) {
    case 1:
        unknown_48 = 0;
        if (unknown_24.Progress() == 1.0) {
            unknown_40 = 2;
            unknown_24 = UnknownSlopeTimer(rocket->TiltFrames);
        }
        break;
    case 2:
        unknown_48 = 0;
        if (unknown_24.Progress() == 1.0) {
            unknown_50 = rocket->PitchFinal * 1.5707963267948966;
            unknown_40 = 3;
            if (LinkedTo->unknown_slot178() == unknown_geometry_k) {
                unknown_tracker_839ea0.Unknown_004131E0(LinkedTo);
            }
            new AnimClass(AnimTypes[AnimTypeClass::From_Name(unknown_rocket_takeoff_anim_7ee804)], Object_Coord(LinkedTo), 2, 1, 0x600, -10);
            Voc_Play_At(LinkedTo->Techno_Type_Class()->AuxSound1, Object_Coord(LinkedTo), 0);
        } else {
            float initial = rocket->PitchInitial * 1.5707963267948966;
            float final = rocket->PitchFinal * 1.5707963267948966;
            unknown_50 = initial + (final - initial) * unknown_24.Progress();
        }
        break;
    case 3:
        unknown_48 += rocket->Acceleration;
        if (unknown_48 > LinkedTo->Techno_Type_Class()->Speed) {
            unknown_48 = LinkedTo->Techno_Type_Class()->Speed;
        }
        if (LinkedTo->Get_Height_AGL() >= rocket->Altitude) {
            unknown_40 = 4;
            unknown_54 = Ground_Distance(unknown_18, LinkedTo);
        }
        break;
    case 4:
        unknown_48 += rocket->Acceleration;
        if (unknown_48 > LinkedTo->Techno_Type_Class()->Speed) {
            unknown_48 = LinkedTo->Techno_Type_Class()->Speed;
        }
        if (rocket->LazyCurve && unknown_54 != 0) {
            if (unknown_63dcf0(rocket)) {
                exploded = true;
            break;
            }
            int distance = Ground_Distance2(unknown_18, LinkedTo);
            double ratio = (double)distance / unknown_54;
            double rest = 1.0 - ratio;
            unknown_50 = unknown_63de10() * rest + rocket->PitchFinal * ratio * 1.5707963267948966;
        } else {
            if (unknown_50 > 0.0f) {
                unknown_50 -= rocket->TurnRate;
                if (unknown_50 < 0.0f) {
                    unknown_50 = 0;
                }
            }
            CoordStruct dest = unknown_18;
            CoordStruct coord = Object_Coord(LinkedTo);
            int height = coord.Z - dest.Z;
            dest.Z = Object_Coord(LinkedTo).Z;
            if (Distance_To(LinkedTo, dest) <= height) {
                unknown_40 = 5;
            }
        }
        LinkedTo->unknown_2e0.Set_Desired(Direction_To(LinkedTo, unknown_18));
        break;
    case 5:
        exploded = unknown_63dcf0(rocket);
        if (!exploded) {
            double diff = unknown_63de10() - unknown_50;
            float step = diff;
            if (step < 0.0) {
                step = -step;
            }
            if (step > rocket->TurnRate) {
                if (diff < 0.0) {
                    unknown_50 -= rocket->TurnRate;
                } else {
                    float rate = rocket->TurnRate;
                    unknown_50 += rate;
                }
            } else {
                unknown_50 += diff;
            }
        }
        break;
    }
    if (exploded) {
        return false;
    }
    if (Is_Moving_Now() && unknown_34 == 0) {
        new AnimClass(AnimTypes[AnimTypeClass::From_Name(unknown_rocket_trail_anim_7ee808)], Object_Coord(LinkedTo), 0, 1, 0x600, 0);
        unknown_34 = 3;
    }
    if (unknown_48 > 0.0) {
        CoordStruct coord = unknown_63dbe0((int)unknown_48);
        LinkedTo->Set_Coord(coord);
        Cell cell = LinkedTo->unknown_slot104();
        if (cell != LinkedTo->unknown_458 && LinkedTo->unknown_7d) {
            unknown_tracker_839ea0.Update_00413600(LinkedTo, LinkedTo->unknown_458, cell);
        }
        if (LinkedTo->Strength <= 0) {
            unknown_63e7d0();
        }
    }
    return Is_Moving();
}


// void RocketLocomotionClass::unknown_63e7d0() (0x0063E7D0)
// Detonates the rocket at its nose: leaves the tracker, picks the warhead
// and damage of its kind (V3Rocket for its type, else DMisl; the elite ones
// when the launcher +0x23C is elite), plays the damage animation, the
// explosion effects and the area damage there, then removes the rocket
// (slot 60).

// The land type of a cell (local helper).
static inline int Land_Type_At(Cell const &cell)
{
    return Map[cell].unknown_ec;
}

void RocketLocomotionClass::unknown_63e7d0()
{
    unknown_tracker_839ea0.Unknown_00413310(LinkedTo);
    TechnoClass *spawner = static_cast<TechnoClass *>(LinkedTo->unknown_23c);
    bool elite = spawner != 0 && spawner->unknown_118.Is_Elite();
    RocketStruct const *rocket;
    WarheadTypeClass *warhead;
    if (((AircraftClass *)LinkedTo)->Type == Rules->V3Rocket.Type) {
        rocket = &Rules->V3Rocket;
        warhead = elite ? Rules->V3EliteWarhead : Rules->V3Warhead;
    } else {
        rocket = &Rules->DMisl;
        warhead = elite ? Rules->DMislEliteWarhead : Rules->DMislWarhead;
    }
    CoordStruct coord = unknown_63dbe0(rocket->BodyLength);
    Cell cell = Coord_Cell(coord);
    int damage = elite ? rocket->EliteDamage : rocket->Damage;
    new AnimClass(Select_Damage_Anim(damage, warhead, Land_Type_At(cell), &coord), coord, 0, 1, 0x2600, Get_Explosion_Z_0047fcc0(coord));
    Unknown_0047F600(coord, damage, warhead, 0, 0);
    Unknown_0047E2C0(coord, damage, LinkedTo, warhead, true);
    LinkedTo->Delete_Me();
}


// SPDX-License-Identifier: GPL-3.0-or-later
// Copyright 2026 ra2-decomp contributors
// Copyright 2025 Electronic Arts Inc.; 2026 OpenTS contributors
// Derived from Electronic Arts source code; EA's GPL Section 7 terms apply (COPYING.md).
// void RocketLocomotionClass::Move_To(COMCoordStruct to) (0x0063EA80)
// From ra2-decomp RocketLocomotionClass::Move_To (YR 0x006632E0; RA2 has no
// cruise-missile branch). Picks the V3 or the dreadnought missile's rocket
// data by the aircraft type; when no destination is set yet, starts the
// pause (or, without one, the tilt) state with its frame timer, sets the
// initial pitch (PitchInitial quarter turns) and takes the destination.
// ILocomotion's COM coordinate is stored through CoordStruct's conversion
// (the unit defines RA2_SHIP_MOVE_TO_VALUE in its first row, 0x0063D8D0).

void __stdcall RocketLocomotionClass::Move_To(COMCoordStruct to)
{
    AircraftTypeClass const *type = ((AircraftClass *)LinkedTo)->Type;
    RocketStruct const *rocket;
    if (type == Rules->V3Rocket.Type) {
        rocket = &Rules->V3Rocket;
    } else {
        rocket = &Rules->DMisl;
    }
    if (unknown_18 == unknown_geometry_l) {
        if (rocket->PauseFrames != 0) {
            unknown_40 = 1;
            unknown_24 = rocket->PauseFrames;
        } else {
            unknown_40 = 2;
            unknown_24 = rocket->TiltFrames;
        }
        unknown_50 = rocket->PitchInitial * 1.5707963267948966;
        unknown_18 = to;
    }
}


// void __stdcall RocketLocomotionClass::Stop_Moving() (0x0063EB50, ILocomotion 18)
// A rocket cannot stop.

void __stdcall RocketLocomotionClass::Stop_Moving()
{
}


// HRESULT __stdcall RocketLocomotionClass::GetClassID(GUID *clsid) (0x0063EB60)

HRESULT __stdcall RocketLocomotionClass::GetClassID(GUID *clsid)
{
    if (clsid == 0) {
        return E_POINTER;
    }
    *clsid = CLSID_RocketLocomotionClass;
    return S_OK;
}


// HRESULT __stdcall RocketLocomotionClass::Load(IStream *stream) (0x0063EBA0)
// LocomotionClass::Load, then rebuilt in place (restarting the second timer).

HRESULT __stdcall RocketLocomotionClass::Load(IStream *stream)
{
    HRESULT result = LocomotionClass::Load(stream);
    if (SUCCEEDED(result)) {
        new (this) RocketLocomotionClass(NoInitClass());
    }
    return result;
}


// int __stdcall RocketLocomotionClass::Layer() (0x0063EBF0, ILocomotion 29)
// Layer 3 (hypothesis: the top layer).

int __stdcall RocketLocomotionClass::Layer()
{
    return 3;
}


// Matrix3D * __stdcall RocketLocomotionClass::Draw_Matrix(Matrix3D *matrix, int *key) (0x0063EC00, ILocomotion 9)
// Turned by the facing and pitched (about Y) by +0x50; the key marks the
// rules' initial (0x20) or final (0x40) pitch of the rocket kind (V3Rocket
// for its type, else DMisl) and is -1 between them.

Matrix3D * __stdcall RocketLocomotionClass::Draw_Matrix(Matrix3D *matrix, int *key)
{
    Matrix3D transform;
    transform.Make_Identity();
    transform.Rotate_Z((float)((Dir_To_32(LinkedTo->unknown_2e0.Current()) - 8) * -0.19634954084936207));
    if (unknown_50 != 0.0f) {
        transform.Rotate_Y(-unknown_50);
        RocketStruct const *rocket;
        if (((AircraftClass *)LinkedTo)->Type == Rules->V3Rocket.Type) {
            rocket = &Rules->V3Rocket;
        } else {
            rocket = &Rules->DMisl;
        }
        float final_pitch = rocket->PitchFinal * 1.5707963267948966;
        if (key != 0) {
            if ((float)(rocket->PitchInitial * 1.5707963267948966) == unknown_50) {
                *key |= 0x20;
            } else if (unknown_50 == final_pitch) {
                *key |= 0x40;
            } else {
                *key = -1;
            }
        }
    }
    if (key != 0) {
        *key |= Dir_To_32(LinkedTo->unknown_2e0.Current());
    }
    *matrix = transform;
    return matrix;
}


// int RocketLocomotionClass::Size() (0x0063ED30): inline in shared/rocket_locomotion.hpp, emitted with the vtable; no source of its own.


// void *RocketLocomotionClass::`scalar deleting destructor'(unsigned int flags) (0x0063ED40)
// Compiler-generated with the vtable (the destructor inlined). No source of its own.
