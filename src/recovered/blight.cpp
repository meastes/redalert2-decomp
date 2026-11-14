// blight.cpp: the original translation unit re-filed as one source (Route A): the accepted
// bodies of its canonical compile group in address order, under the unit's own header
// state (its switches and includes in their original order). The module map
// manifests/original_target/modules/blight.json carries the addresses and the rows that
// stay in their own files.


#define RA2_TPOINT3D_LENGTH_TIBSUN
#define RA2_RECT_CORNER_INLINE
#define RA2_BUILDING_LIGHT_DISTANCE_TO
#define RA2_BUILDING_LIGHT_DISTANCE
#define RA2_COORD_TPOINT3D
#define RA2_TPOINT3D_DIFFERENCE
#define RA2_CELL_OCCUPIER
#include "shared/unknown_geometry.hpp"
#include "shared/unknown_static_rect.hpp"
#include "shared/building_light.hpp"
#include "shared/map_view.hpp"
#include "shared/spot_light.hpp"
#include "shared/rules.hpp"
#include "shared/matrix3d.hpp"
#include "shared/tactical.hpp"
#include "shared/unknown_globals.hpp"
#include "shared/tool_tip.hpp"
#include "shared/surface.hpp"
#include "shared/fast_math.hpp"
#include "shared/always.hpp"
#include "shared/house.hpp"
#include "shared/map_cell_view.hpp"
#include "shared/tag.hpp"


// Header-static dynamic initializers of the unit at 0x00432DD0:  the
// shared/unknown_geometry.hpp constants and unknown_static_rect.hpp
// (0x00432DD0-0x00432FB6); the other
// drafts of this plan are emitted by these includes.


// Dynamic initializer (0x00432E00, _$E) of b from the header(s) included
// by HeaderStatics_diagonal_init_00432dd0.cpp; no source of its own.


// Dynamic initializer (0x00432E20, _$E) of c from the header(s) included
// by HeaderStatics_diagonal_init_00432dd0.cpp; no source of its own.


// Dynamic initializer (0x00432E40, _$E) of angle45 from the header(s) included
// by HeaderStatics_diagonal_init_00432dd0.cpp; no source of its own.


// Dynamic initializer (0x00432E60, _$E) of angle60 from the header(s) included
// by HeaderStatics_diagonal_init_00432dd0.cpp; no source of its own.


// Dynamic initializer (0x00432E80, _$E) of angle90 from the header(s) included
// by HeaderStatics_diagonal_init_00432dd0.cpp; no source of its own.


// Dynamic initializer (0x00432EA0, _$E) of g from the header(s) included
// by HeaderStatics_diagonal_init_00432dd0.cpp; no source of its own.


// Dynamic initializer (0x00432EE0, _$E) of h from the header(s) included
// by HeaderStatics_diagonal_init_00432dd0.cpp; no source of its own.


// Dynamic initializer (0x00432F10, _$E) of i from the header(s) included
// by HeaderStatics_diagonal_init_00432dd0.cpp; no source of its own.


// Dynamic initializer (0x00432F40, _$E) of j from the header(s) included
// by HeaderStatics_diagonal_init_00432dd0.cpp; no source of its own.


// Dynamic initializer (0x00432F70, _$E) of k from the header(s) included
// by HeaderStatics_diagonal_init_00432dd0.cpp; no source of its own.


// Dynamic initializer (0x00432F80, _$E) of l from the header(s) included
// by HeaderStatics_diagonal_init_00432dd0.cpp; no source of its own.


// Dynamic initializer (0x00432FA0, _$E) of static_rect from the header(s) included
// by HeaderStatics_diagonal_init_00432dd0.cpp; no source of its own.


BuildingLightClass::BuildingLightClass(TechnoClass *owner) :
    unknown_98(0),
    unknown_a0(unknown_geometry_l),
    unknown_ac(unknown_geometry_l),
    unknown_b8(0),
    unknown_c0(false),
    Behavior(0),
    Following(0),
    Owner(owner)
{
    BuildingLights.Add(this);
    if (owner != 0) {
        Init_Rotation_Arc(owner);
        BuildingLightClass::Unlimbo(&CoordStruct(Coord), 0);
        Set_Behavior(1);
        unknown_c0 = BuildingLights.ID(this) % 2 != 0;
    }
}


// BuildingLightClass::~BuildingLightClass() (0x004332F0)
// Takes the light off the map (slot 52, called directly) and out of BuildingLights.

BuildingLightClass::~BuildingLightClass()
{
    Limbo();
    BuildingLights.Delete(this);
}


// void BuildingLightClass::unknown_slot67(Point2D const &point, Rect const &window) (0x00433380, slot 67)
// Draws a building's search light: a spot light at the light's coordinate
// sized by its distance from the owner, and, past the owner's reach, the two
// edges of the beam from the owner's top to the points rotated either side of
// the light (names are hypotheses). Statement shape adopted from the TibSun
// reconstruction's blight.cpp BuildingLightClass::Draw_It (OpenTS;
// SPDX-License-Identifier: GPL-3.0-or-later; see THIRD_PARTY.md). RA2
// differences, from the bytes: the early return when the owner is within
// detection range (TibSun wraps the beam code in an if), the radius clamp
// through min, here from Position_Coord, the caster point as
// there + CoordStruct(0, 0, 430), the end depth from the light's coordinate
// plus CoordStruct(0, 0, 250), and a second named Vector3 (vec2) for the
// other edge where TibSun reuses vec (the frame groups vec with the early
// temporaries and there with the operator- buffers, as in the original).
// Needs RA2_TPOINT3D_LENGTH_TIBSUN and RA2_RECT_CORNER_INLINE (defined at
// line 1 of the unit's first row 0x00432DD0).

void BuildingLightClass::unknown_slot67(Point2D const &point, Rect const &window)
{
    if (Behavior == 0 || Owner == 0 || Owner->Kind_Of() != 6) {
        return;
    }
    BuildingClass *building = (BuildingClass *)Owner;
    if (building == 0 || !building->unknown_7d || !building->unknown_slot195() || building->unknown_5b7) {
        return;
    }
    if ((unknown_id_source_a3d290->unknown_000 & 0x1000) && Map.Is_Fogged(Position_Coord_004f3f40())) {
        return;
    }

    SpotLightClass *spotlight = new SpotLightClass(Position_Coord_004f3f40(), 0x10);

    int planar_dist = Distance(Owner->Center_Coord());
    int stage = Sweep_Stage();

    if (planar_dist > Rules->unknown_6b4 && Behavior == 3) {
        int radius = stage + 80;
        radius = (radius > 0) ? radius : 0;
        spotlight->Set_Radius(min(radius, 89));
    } else {
        spotlight->Set_Radius(0x50);
    }

    spotlight->unknown_func_5ddb90();
    delete spotlight;

    CoordStruct here = Position_Coord_004f3f40();
    CoordStruct there = Owner->Center_Coord();

    int distance = Distance(there);
    int detection = Detection_Radius();

    if (distance < detection) {
        return;
    }

    double angle = Fast_Asin_004ba550((double)detection / (double)distance);

    Matrix3D mat;
    mat.Make_Identity();
    mat.Rotate_Z(angle);
    Vector3 v(here.X - there.X, here.Y - there.Y, here.Z - there.Z);
    Vector3 vec = mat * v;
    CoordStruct arc1 = CoordStruct(there.X + vec.X, there.Y + vec.Y, there.Z + vec.Z);

    mat.Make_Identity();
    mat.Rotate_Z(-angle);
    Vector3 vec2 = mat * v;
    CoordStruct arc2 = CoordStruct(there.X + vec2.X, there.Y + vec2.Y, there.Z + vec2.Z);

    Point2D arc1_px;
    Point2D arc2_px;
    Point2D caster_px;
    unknown_tactical_83996c->Coord_To_Pixel(arc1, arc1_px);
    unknown_tactical_83996c->Coord_To_Pixel(arc2, arc2_px);
    unknown_tactical_83996c->Coord_To_Pixel(there + CoordStruct(0, 0, 430), caster_px);

    arc1_px += unknown_rect_839630.Top_Left();
    arc2_px += unknown_rect_839630.Top_Left();
    caster_px += unknown_rect_839630.Top_Left();
    Point2D caster_px2 = caster_px;

    int zstart = -Unknown_006A2970(there.Z + 400);
    int zend = -Unknown_006A2970((Position_Coord_004f3f40() + CoordStruct(0, 0, 250)).Z);

    if (Clip_Line(caster_px, arc1_px, unknown_rect_839630)) {
        unknown_surface_83995c->Draw_Depth_Glow_Line(unknown_surface_83995c->Get_Rect(), caster_px, arc1_px, 75 - 6 * stage, zstart, zend);
    }
    if (Clip_Line(caster_px2, arc2_px, unknown_rect_839630)) {
        unknown_surface_83995c->Draw_Depth_Glow_Line(unknown_surface_83995c->Get_Rect(), caster_px2, arc2_px, 75 - 6 * stage, zstart, zend);
    }
}


// void BuildingLightClass::AI() (0x00433970, slot 23)
// Moves the light: behavior 3 drifts a quarter of the way toward the followed
// object while it is within the far range (else falls back to behavior 1),
// behavior 2 sweeps a full circle around the owner, behavior 1 swings back
// and forth within the rules' angle. A building owner in behavior 1 then
// springs its tag (event 0x23) when a non-allied object of kind 15 or 1 in
// the 3x3 cells around the light is within the light's radius. Lights whose
// owner is gone or inactive remove themselves (names are hypotheses).
// Statement shape adopted from the TibSun reconstruction's blight.cpp
// BuildingLightClass::AI (OpenTS; SPDX-License-Identifier: GPL-3.0-or-later;
// see THIRD_PARTY.md). RA2 differences, from the bytes: the cached Following,
// one Spring, case 3 after cases 1 and 2, and the follow test measuring
// Distance_YZX(Owner->Center_Coord()) into a local inside the target test (the
// owner's Center_Coord temporary in its own top frame slot 0xC4, the y, z, x
// sum as in the locomotors' Start_Of_Move). Needs RA2_BUILDING_LIGHT_DISTANCE,
// RA2_COORD_TPOINT3D, RA2_TPOINT3D_DIFFERENCE and RA2_CELL_OCCUPIER (defined
// at line 1 of the unit's first row 0x00432DD0).
CoordStruct __fastcall Lerp(CoordStruct const &from, CoordStruct const &to, float t); // 0x007220C0 (Wave unit)

void BuildingLightClass::AI()
{
    if (Owner == 0 || !Owner->unknown_7d) {
        Delete_Me();
        return;
    }

    CoordStruct coord;
    ObjectClass *following;
    switch (Behavior) {
    default:
        coord = Coord;
        break;

    case 1: {
        unknown_98 += unknown_b8;
        if (unknown_c0) {
            if (unknown_98 > Rules->SpotlightAngle / 2) {
                unknown_b8 -= Rules->SpotlightAcceleration;
                if (unknown_b8 < 0) {
                    unknown_b8 = 0;
                    unknown_c0 = false;
                }
            } else if (unknown_b8 < Rules->SpotlightSpeed) {
                unknown_b8 += Rules->SpotlightAcceleration;
            }
        } else {
            if (unknown_98 < Rules->SpotlightAngle / -2) {
                unknown_b8 += Rules->SpotlightAcceleration;
                if (unknown_b8 > 0.0) {
                    unknown_b8 = 0.0;
                    unknown_c0 = true;
                }
            } else if (-Rules->SpotlightSpeed < unknown_b8) {
                unknown_b8 -= Rules->SpotlightAcceleration;
            }
        }

        Matrix3D mtx;
        mtx.Make_Identity();
        mtx.Rotate_Z(unknown_98);

        Vector3 v(unknown_ac.X - unknown_a0.X, unknown_ac.Y - unknown_a0.Y, unknown_ac.Z - unknown_a0.Z);
        v = mtx * v;
        coord = CoordStruct(unknown_a0.X + v.X, unknown_a0.Y + v.Y, unknown_a0.Z + v.Z);
    }
    break;
    case 2: {
        CoordStruct owner_coord = Owner->Coord;
        unknown_98 += Rules->SpotlightSpeed * 4;

        if (unknown_98 > 6.283185307179586) {
            unknown_98 -= 6.283185307179586;
        }

        Matrix3D mtx;
        mtx.Make_Identity();
        mtx.Rotate_Z(unknown_98);

        Vector3 v(unknown_ac.X - owner_coord.X, unknown_ac.Y - owner_coord.Y, unknown_ac.Z - owner_coord.Z);
        v = mtx * v;
        coord = CoordStruct(owner_coord.X + v.X, owner_coord.Y + v.Y, owner_coord.Z + v.Z);
    }
    break;

    case 3:
        following = Following;
        if (following != 0 && following->unknown_7d) {
            int dist = following->Distance_YZX(Owner->Center_Coord());
            if (dist < Rules->unknown_6b0) {
                coord = Lerp(Position_Coord_004f3f40(), following->Position_Coord_004f3f40(), 0.25f);
                break;
            }
        }
        Set_Behavior(1);
        break;

    }

    Set_Coord(coord);

    BuildingClass *owner_building = (Owner->Kind_Of() == 6) ? (BuildingClass *)Owner : 0;

    if (Behavior == 1) {
        if (owner_building != 0 && owner_building->unknown_7d && owner_building->unknown_slot195() && owner_building->unknown_34 != 0) {
            bool found = false;
            Cell cell = unknown_slot104();
            int radius = Detection_Radius();
            int detection = radius + 30;
            HouseClass *house = Owner->House;

            for (int x = -1; x < 2; x++) {
                for (int y = -1; y < 2; y++) {
                    ObjectClass *occupier = Map[cell + Cell(x, y)].Cell_Occupier();
                    while (occupier != 0) {
                        TechnoClass *techno = (TechnoClass *)occupier;
                        if ((occupier->Kind_Of() == 0xf || occupier->Kind_Of() == 1) && !house->Is_Ally(techno)) {
                            int dist = occupier->Center_Coord().Distance_To(Position_Coord_004f3f40());
                            if (dist < detection) {
                                found = true;
                            }
                        }
                        occupier = occupier->unknown_30;
                    }
                }
            }

            if (found) {
                owner_building->unknown_34->Spring(0x23, owner_building, unknown_geometry_k);
            }
        }
    }
}


// BuildingLightClass::Size_Of (0x00434090)
// Compiler-emitted from shared/building_light.hpp in this unit; no source of its own.


// HRESULT __stdcall BuildingLightClass::GetClassID(GUID *clsid) (0x004340A0)

HRESULT __stdcall BuildingLightClass::GetClassID(GUID *clsid)
{
    if (clsid == 0) {
        return E_POINTER;
    }
    *clsid = CLSID_BuildingLightClass;
    return S_OK;
}


// HRESULT __stdcall BuildingLightClass::Load(IStream *stream) (0x004340E0)
// Restores the light in place and swizzles its owner and followed object.

HRESULT __stdcall BuildingLightClass::Load(IStream *stream)
{
    HRESULT result = ObjectClass::Load(stream);
    if (SUCCEEDED(result)) {
        new (this) BuildingLightClass(NoInitClass());
        Swizzler.Swizzle((void **)&Owner);
        Swizzler.Swizzle((void **)&Following);
    }
    return result;
}


// HRESULT __stdcall BuildingLightClass::Save(IStream *stream, BOOL clear_dirty) (0x00434150)

HRESULT __stdcall BuildingLightClass::Save(IStream *stream, BOOL clear_dirty)
{
    return AbstractClass::Save(stream, clear_dirty);
}


// BuildingLightClass::Class_Of (0x00434170)
// Compiler-emitted from shared/building_light.hpp in this unit; no source of its own.


// BuildingLightClass::In_Which_Layer (0x00434180)
// Compiler-emitted from shared/building_light.hpp in this unit; no source of its own.


// void BuildingLightClass::Detach(AbstractClass *target, bool all) (0x00434190)
// Forgets target as the followed object or the owner.

void BuildingLightClass::Detach(AbstractClass *target, bool all)
{
    ObjectClass::Detach(target, all);
    if (Following == target) {
        Following = 0;
    }
    if (Owner == target) {
        Owner = 0;
    }
}


// The point DISTANCE leptons from COORD along FACING (in the ground plane;
// Z is kept). Expanded twice by 0x004341D0 and, through it, the constructor.
static inline CoordStruct Coord_Along_Facing(UnknownFacingDir facing, int distance, CoordStruct coord)
{
    double angle = ((short)facing.Raw - 0x3fff) * unknown_facing_radians_79b620;
    double y = coord.Y;
    return UnknownCoord434820((int)(unknown_cos_4ba4d0(angle) * distance + coord.X), (int)(y - unknown_sin_4ba480(angle) * distance), coord.Z);
}

// Moves LIGHT to COORD (virtual slot 103).
static inline void Move_To(BuildingLightClass *light, CoordStruct const &coord)
{
    light->Set_Coord(coord);
}

// Places the light's two points in front of and behind OBJECT (Rules
// +0x6B4 ahead, +0x6B0 behind) and moves the light to the front one.
void BuildingLightClass::Init_Rotation_Arc(TechnoClass *object)
{
    CoordStruct near_point = Coord_Along_Facing(object->unknown_2e0.Current(), Rules->unknown_6b4, object->Coord);
    unknown_ac = near_point;
    unknown_a0 = Coord_Along_Facing(object->unknown_2e0.Current(), -Rules->unknown_6b0, object->Position_Coord_004f3f40());
    Move_To(this, near_point);
}


// void BuildingLightClass::Set_Behavior(int behavior) (0x00434370, name hypothesis)
// Clears the sweep angle; behavior 3 follows the nearest non-allied object of
// kind 15 or 1 in the 3x3 cells around the light.
// Uses the member CoordStruct::operator- of shared/coord.hpp (out-of-line copy 0x004348F0).

void BuildingLightClass::Set_Behavior(int behavior)
{
    Behavior = behavior;
    unknown_98 = 0;
    if (behavior == 3) {
        Cell cell = unknown_slot104();
        int best_distance = 9999999;
        ObjectClass *best = 0;
        HouseClass *house = Owner->House;
        for (int x = -1; x < 2; x++) {
            for (int y = -1; y < 2; y++) {
                Cell neighbor = Cell(cell.X + x, cell.Y + y);
                for (ObjectClass *object = Map[neighbor].OccupierPtr; object != 0; object = object->unknown_30) {
                    if ((object->Kind_Of() == 0xf || object->Kind_Of() == 1) && !house->Is_Ally((TechnoClass *)object)) {
                        CoordStruct here = Coord;
                        CoordStruct diff = object->Center_Coord() - here;
                        int distance = (int)unknown_sqrt_4ba410((double)diff.X * diff.X + (double)diff.Z * diff.Z + (double)diff.Y * diff.Y);
                        if (distance < best_distance) {
                            best = object;
                            best_distance = distance;
                        }
                    }
                }
            }
        }
        if (best != 0) {
            Following = best;
        }
    }
}


// int BuildingLightClass::Detection_Radius() const (0x00434530)
// The distance step (Sweep_Stage) scaled by 5.973333333333333 and
// offset by the rules value at +0x6D0. TibSun blight.cpp Detection_Radius
// shape (OpenTS; SPDX-License-Identifier: GPL-3.0-or-later; see
// THIRD_PARTY.md): the scale is its function-local `static const double
// _scale` (0x0079C7A8, read from memory) and the result is a named radius;
// the cast-and-add initializer (TibSun: `radius += ...` as a second
// statement) gives slot 67's register order for the inlined add.

int BuildingLightClass::Detection_Radius() const
{
    static const double _scale = 5.973333333333333;
    int radius = (int)(Sweep_Stage() * _scale) + Rules->unknown_6d0;
    return(radius);
}


// int BuildingLightClass::Sweep_Stage() const (0x00434610)
// The light's distance from its owner as a step: 0 inside the rules' near
// range (+0x6B4), then tenths of the span up to the far range (+0x6B0).
// TibSun blight.cpp Sweep_Stage shape (OpenTS; SPDX-License-Identifier:
// GPL-3.0-or-later; see THIRD_PARTY.md) with the distance written as the
// Yuri's Revenge twin's `Distance_To(Owner)` (ra2-decomp src/blight.cpp
// banked form). ObjectClass::Distance_To(AbstractClass const *) is TibSun's
// object.h inline (RA2_BUILDING_LIGHT_DISTANCE_TO, defined in the unit's
// first row 0x00432DD0); its TPoint3D::Length (RA2_TPOINT3D_LENGTH_TIBSUN)
// is the 0x0041B780 copy that slot 67's inlined copy of this function calls.

int BuildingLightClass::Sweep_Stage() const
{
    int distance = Distance_To(Owner);
    int near_range = Rules->unknown_6b4;
    if (distance < near_range) {
        return 0;
    }
    return (distance - near_range) / ((Rules->unknown_6b0 - near_range) / 10);
}


// void BuildingLightClass::Compute_CRC(WWCRCEngine &crc) const (0x004346D0)

void BuildingLightClass::Compute_CRC(WWCRCEngine &crc) const
{
    ObjectClass::Compute_CRC(crc);
    crc(unknown_98);
    crc((long)unknown_a0.X);
    crc((long)unknown_a0.Y);
    crc((long)unknown_a0.Z);
    crc((long)unknown_ac.X);
    crc((long)unknown_ac.Y);
    crc((long)unknown_ac.Z);
    crc(unknown_b8);
    crc(unknown_c0);
    crc((long)Behavior);
    if (Following != 0) {
        crc(Following->Fetch_ID());
    }
    if (Owner != 0) {
        crc(Owner->Fetch_ID());
    }
}


// bool BuildingLightClass::Limbo() (0x004347C0, slot 52)
// ObjectClass's slot 52, then removes the light from the list at 0x00832470.

bool BuildingLightClass::Limbo()
{
    if (ObjectClass::Limbo()) {
        unknown_list_832470.Remove(this);
        return true;
    }
    return false;
}


// bool BuildingLightClass::Unlimbo(CoordStruct const *coord, int dir) (0x004347E0, slot 53)
// ObjectClass's slot 53, then submits the light to the list at 0x00832470.

bool BuildingLightClass::Unlimbo(CoordStruct const *coord, int dir)
{
    if (ObjectClass::Unlimbo(coord, dir)) {
        unknown_list_832470.Submit(this, false);
        return true;
    }
    return false;
}


// BuildingLightClass::Kind_Of (0x00434840)
// Compiler-emitted from shared/building_light.hpp in this unit; no source of its own.


// void *BuildingLightClass::`scalar deleting destructor'(unsigned int) (0x00434850)
// Compiler-emitted from shared/building_light.hpp in this unit (destructor inlined); no source of its own.
