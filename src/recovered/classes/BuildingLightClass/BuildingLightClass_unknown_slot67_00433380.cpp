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
// @ref ?unknown_id_source_a3d290@@3PAVUnknownIDSource@@A=00a3d290
// @ref ?Is_Fogged@MapClass@@QAE_NABUCoordStruct@@@Z=005683c0
// @ref ??0SpotLightClass@@QAE@UCoordStruct@@H@Z=005dd590
// @ref ??1SpotLightClass@@QAE@XZ=005dd610
// @ref ?unknown_func_5ddb90@SpotLightClass@@QAEXXZ=005ddb90
// @ref ??0?$TPoint3D@H@@QAE@HHH@Z=0041b630
// @ref ??G?$TPoint3D@H@@QBE?BV0@ABV0@@Z=004348f0
// @ref ?Sweep_Stage@BuildingLightClass@@QBEHXZ=00434610
// @ref ?Length@?$TPoint3D@H@@QBEHXZ=0041b780
// @ref _?_scale@?1??Detection_Radius@BuildingLightClass@@QBEHXZ@4NB=0079c7a8

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
