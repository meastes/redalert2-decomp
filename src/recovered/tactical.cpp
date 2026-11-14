// tactical.cpp: the original translation unit re-filed as one source (Route A): the accepted
// bodies of its canonical compile group in address order, under the unit's own header
// state (its switches and includes in their original order). The module map
// manifests/original_target/modules/tactical.json carries the addresses and the rows that
// stay in their own files.


// Tactical::Draw_Tile_Shadows(int, Rect const &) 0x006A88A0 is declared for this unit.
#define RA2_TACTICAL_TILE_SHADOWS_AREA
#define RA2_TACTICAL_SHADOW_COORD_CTOR_DECL
#define CELL_DECLARE_DRAW_IT
#define RA2_COORD_AS_CELL
#define RA2_TACTICAL_CELL_PROJECT_GEOMETRY
#define RA2_COORD_POINT_Z
#define RA2_COORD_TPOINT3D
#define RA2_RECT_CORNER_INLINE
// The Rect/RectPoint2D corner arithmetic expands inline in this unit where /Ob2 allows (Debug_Draw_Occupation_Flags
// 0x006ABC20 calls the RectPoint2D sum 0x00435540 and the corner constructor 0x00453E40 at six of its eight sites).
#define RA2_RECT_POINT_INLINE
#define RA2_POINT_ADD_IN_PLACE
#define RA2_MOUSE_FRAME_QUERIES
#define RA2_TACTICAL_PLACEMENT
#define RA2_TACTICAL_PICK_OBJECT
// Tactical::Draw_Objects 0x006A93F0 is declared for this unit.
#define RA2_TACTICAL_DRAW_OBJECTS
// Tactical::Debug_Draw_Occupation_Flags 0x006ABC20 is declared for this unit.
#define RA2_TACTICAL_OCCUPATION_FLAGS
// Header-static dynamic initializers of the unit at 0x006A20E0:  the
// shared/unknown_geometry.hpp constants and unknown_static_rect.hpp
// (0x006A20E0-0x006A22C6); the other
// drafts of this plan are emitted by these includes.
// Not exact (RA2_NON_MATCHING) prelude: header switches the drafts below need (guard-on compile only)
#ifdef RA2_NON_MATCHING
#define RA2_POINT_LENGTH
#endif

#include "shared/unknown_geometry.hpp"
#include "shared/unknown_static_rect.hpp"


// Dynamic initializer (0x006A2110, _$E) of b from the header(s) included
// by HeaderStatics_diagonal_init_006a20e0.cpp; no source of its own.


// Dynamic initializer (0x006A2130, _$E) of c from the header(s) included
// by HeaderStatics_diagonal_init_006a20e0.cpp; no source of its own.


// Dynamic initializer (0x006A2150, _$E) of angle45 from the header(s) included
// by HeaderStatics_diagonal_init_006a20e0.cpp; no source of its own.


// Dynamic initializer (0x006A2170, _$E) of angle60 from the header(s) included
// by HeaderStatics_diagonal_init_006a20e0.cpp; no source of its own.


// Dynamic initializer (0x006A2190, _$E) of angle90 from the header(s) included
// by HeaderStatics_diagonal_init_006a20e0.cpp; no source of its own.


// Dynamic initializer (0x006A21B0, _$E) of g from the header(s) included
// by HeaderStatics_diagonal_init_006a20e0.cpp; no source of its own.


// Dynamic initializer (0x006A21F0, _$E) of h from the header(s) included
// by HeaderStatics_diagonal_init_006a20e0.cpp; no source of its own.


// Dynamic initializer (0x006A2220, _$E) of i from the header(s) included
// by HeaderStatics_diagonal_init_006a20e0.cpp; no source of its own.


// Dynamic initializer (0x006A2250, _$E) of j from the header(s) included
// by HeaderStatics_diagonal_init_006a20e0.cpp; no source of its own.


// Dynamic initializer (0x006A2280, _$E) of k from the header(s) included
// by HeaderStatics_diagonal_init_006a20e0.cpp; no source of its own.


// Dynamic initializer (0x006A2290, _$E) of l from the header(s) included
// by HeaderStatics_diagonal_init_006a20e0.cpp; no source of its own.


// Dynamic initializer (0x006A22B0, _$E) of static_rect from the header(s) included
// by HeaderStatics_diagonal_init_006a20e0.cpp; no source of its own.


// Tactical.CPP file statics after the header statics: the tactical area
// rectangle (0x00ABE560, initializer 0x006A22D0), the dirty areas (0x00ABE5B0,
// 0x006A22F0, exit 0x006A2330), the object pixel table (0x00ABE5F8, empty
// initializer 0x006A2390), the shadow controls (0x00ABE528, 0x006A23A0, exit
// 0x006A23E0) and three scale factors from the cell diagonal and the 60 degree
// sine (0x006A2440, 0x006A2480; names hypotheses).
#ifndef VECTOR_IMPLEMENT_ALL
#define VECTOR_IMPLEMENT_ALL
#endif
#include "shared/tactical.hpp"
#include "shared/tactical_lists.hpp"
#include "shared/fast_math.hpp"

Rect unknown_rect_abe560(0, 0, 0, 0);
DynamicVectorClass<DirtyAreaStruct> TacticalLists::DirtyAreas;
TacticalObjectPixel unknown_tactical_pixels_abe5f8[500];
DynamicVectorClass<ShadowControlClass *> TacticalLists::ShadowControls;
static double unknown_scale_abe550 = 60.0 / unknown_geometry_diagonal, unknown_scale_abe490 = Fast_Sin(unknown_geometry_angle60) * unknown_scale_abe550;
static double unknown_scale_abe520 = unknown_geometry_diagonal / (Fast_Sin(unknown_geometry_angle60) * 60.0);


// the dirty-area list initializer (0x006A22F0, _$E49) is emitted by the draft of 0x006A22D0 (Tactical.CPP file statics); no source of its own.


// the dirty-area list exit destructor (guard bit 0x00ABE498) (0x006A2330, _$E46) is emitted by the draft of 0x006A22D0 (Tactical.CPP file statics); no source of its own.


// the object pixel table initializer (empty: Point2D has an empty constructor) (0x006A2390, _$E52) is emitted by the draft of 0x006A22D0 (Tactical.CPP file statics); no source of its own.


// the shadow-control list initializer (0x006A23A0, _$E58) is emitted by the draft of 0x006A22D0 (Tactical.CPP file statics); no source of its own.


// the shadow-control list exit destructor (guard bit 0x00ABE499) (0x006A23E0, _$E55) is emitted by the draft of 0x006A22D0 (Tactical.CPP file statics); no source of its own.


// the initializer of the two scale factors at 0x00ABE550 and 0x00ABE490 (0x006A2440, _$E61) is emitted by the draft of 0x006A22D0 (Tactical.CPP file statics); no source of its own.


// the initializer of the scale factor at 0x00ABE520 (0x006A2480, _$E64) is emitted by the draft of 0x006A22D0 (Tactical.CPP file statics); no source of its own.


// Tactical::Tactical() (0x006A24B0): the new view (Read_Scenario_INI). The
// members start cleared, the scroll points at the zero coordinate, the view
// rectangle as the empty header rectangle; the body clears the cells to
// redraw and the caption and builds the two projection matrices (TibSun
// Tactical::Tactical).
// @ref ?unknown_abe540@@3UPoint2D@@A=00abe540/00abe544

extern Point2D unknown_abe540;    // 0x00ABE540 (the header static unknown_geometry_l, read as a point)

Tactical::Tactical()
    : unknown_a8(-1),
      TacPixelX(0),
      TacPixelY(0),
      LastTacPixelX(0),
      LastTacPixelY(0),
      unknown_c0(1.0),
      unknown_c8(unknown_abe540),
      unknown_d0(unknown_abe540),
      unknown_d8(0),
      unknown_dc(0),
      unknown_e0(0),
      unknown_d64(0, 0),
      unknown_d6c(0, 0),
      unknown_d74(0, 0),
      unknown_d7c(true),
      unknown_d7d(false),
      unknown_d7e(false),
      unknown_d80(unknown_static_rect),
      RubberBandStart(0, 0),
      RubberBandEnd(0, 0),
      unknown_da0(0),
      unknown_da4(),
      unknown_db0(0)
{
    for (int i = 0; i < 800; i++) {
        unknown_e4[i] = 0;
    }

    unknown_24 = 0;
    unknown_a4 = -1;

    unknown_db4.Make_Identity();
    unknown_db4.Rotate_X(unknown_geometry_angle60);
    unknown_db4.Rotate_Z(unknown_geometry_angle45);
    unknown_db4.Scale(60.0f / (float)unknown_geometry_diagonal);

    unknown_de4.Set(4.2667f, 8.5334f, 0.0f, 0.0f,
                    -4.2667f, 8.5334f, 0.0f, 0.0f,
                    0.0f, 0.0f, 1.0f, 0.0f);

    unknown_tactical_83996c = this;
}


// Tactical::Tactical(NoInitClass const &) (0x006A26C0): Load's placement
// constructor. Only the AbstractClass base, the flag pair at +0xAC (its NoInitClass
// constructor does nothing) and the timer at +0xDA4 (it has no
// NoInitClass constructor of its own) are constructed; then the vptrs. The
// unit emits the vtables; their slots are bound below.

Tactical::Tactical(NoInitClass const &noinit)
    : AbstractClass(noinit),
      unknown_ac(noinit)
{
}


// Tactical::~Tactical() (0x006A2710)
// Forgets the global tactical view, then the AbstractClass part.

Tactical::~Tactical()
{
    unknown_tactical_83996c = 0;
}


// SPDX-License-Identifier: GPL-3.0-or-later
// Copyright 2026 ra2-decomp contributors
// Copyright 2025 Electronic Arts Inc.; 2026 OpenTS contributors
// Derived from Electronic Arts source code; EA's GPL Section 7 terms apply (COPYING.md).
// Point2D __fastcall Rectangular_To_Isometric(Point2D point) (0x006A2740): a rectangular point rotated onto
// the isometric axes (TibSun Tactical::Rectangular_To_Isometric; written as ra2-decomp's
// Tactical::Rectangular_To_Isometric, YR 0x006D1EB0, the same arithmetic as 0x006A6800). The hidden result
// comes in ecx and the point on the stack, which is why the old TacticalProjection constructor spelling also fit.

Point2D __fastcall Rectangular_To_Isometric(Point2D point)
{
    Point2D iso;
    iso.X = point.X * 60 / 2;
    iso.Y = point.X * 30 / 2;
    iso.X += point.Y * 60 / -2;
    iso.Y += point.Y * 30 / 2;
    return iso;
}


// Point2D Tactical::Coord_To_Pixel_Absolute(CoordStruct const &coord) (0x006A27A0): the absolute screen pixel of a
// coordinate: its X/Y rotated onto the isometric axes, leptons to pixels (/256), raised by the screen
// height of its Z (0x006A2970) (TibSun Tactical::Coord_To_Pixel_Absolute(Coord const &)).

Point2D Tactical::Coord_To_Pixel_Absolute(CoordStruct const &coord)
{
    Point2D point(coord.X, coord.Y);
    Point2D pixel = Rectangular_To_Isometric(point);
    pixel.X /= 256;
    pixel.Y /= 256;
    int lift = Unknown_006A2970(coord.Z);
    pixel.Y -= lift;
    return pixel;
}


// Point2D Tactical::Coord_To_Pixel_Absolute(Point2D const &point) (0x006A2870): the absolute screen pixel of a
// flat lepton point: rotated onto the isometric axes, leptons to pixels (/256) (TibSun
// Tactical::Coord_To_Pixel_Absolute(Point2D const &), without its unused height lift).

Point2D Tactical::Coord_To_Pixel_Absolute(Point2D const &point)
{
    Point2D pixel = Rectangular_To_Isometric(point);
    pixel.X /= 256;
    pixel.Y /= 256;
    return pixel;
}


// Point2D Tactical::Unknown_006A2900(Point2D const &) (0x006A2900): a screen
// point run through the projection matrix at +0xDE4 (the techno types' smoke
// offsets use it).
#include "shared/unknown_globals.hpp"

Point2D Tactical::Unknown_006A2900(Point2D const &point)
{
    Vector3 position((float)point.X,(float)point.Y,0.0f);
    position = unknown_de4 * position;
    return Point2D((int)position.X, (int)position.Y);
}


// int __fastcall Unknown_006A2970(int z) (0x006A2970): the screen rise of a
// height in leptons, rounded, one pixel more from 728 up (hypothesis). Tactical
// 0x006A27A0 expands it inline.
// unknown_scale_abe490: Tactical.CPP's file static (pixels per lepton of height),
// defined with its initialiser 0x006A22D0 earlier in this unit.

int __fastcall Unknown_006A2970(int z)
{
    int bias = 0;
    if (z >= 728) {
        bias = 1;
    }
    return (int)(z * unknown_scale_abe490 + bias + 0.5);
}


// int __fastcall Unknown_006A29B0(int pixel) (0x006A29B0): a screen rise back
// to leptons of height (the inverse of 0x006A2970; hypothesis).
// unknown_scale_abe520: Tactical.CPP's file static (leptons of height per pixel),
// defined with its initialiser 0x006A22D0 earlier in this unit.

int __fastcall Unknown_006A29B0(int pixel)
{
    return (int)((pixel - 0.5) * unknown_scale_abe520);
}


// bool Tactical::Coord_To_Pixel(Coord const &, Point2D &) (0x006A29D0).

bool Tactical::Coord_To_Pixel(CoordStruct const &coord, Point2D &pixel)
{
    pixel = Coord_To_Pixel_Absolute(coord);
    pixel -= Point2D(TacPixelX, TacPixelY);

    if (pixel.X < -360) {
        return false;
    }

    if (pixel.X > unknown_rect_abe560.Width + 360) {
        return false;
    }

    if (pixel.Y < -180) {
        return false;
    }

    if (pixel.Y > unknown_rect_abe560.Height + 180) {
        return false;
    }

    return true;
}


// CoordStruct Tactical::Pixel_To_Coord(Point2D const &point) (0x006A2B10): the
// coordinate under a screen point: the point plus the view position through the
// projection matrix at +0xDE4, Z 0; the unit's zero coordinate when the point
// lies right of or below the rectangle at 0x00ABE560 (hypothesis).

CoordStruct Tactical::Pixel_To_Coord(Point2D const &point)
{
    if (point.X < unknown_rect_abe560.Width && point.Y < unknown_rect_abe560.Height + unknown_rect_abe560.Y) {
        Point2D view(TacPixelX, TacPixelY);
        Point2D pixel(point.X + view.X, point.Y + view.Y);
        Vector3 position;
        position.X = (float)pixel.X;
        position.Y = (float)pixel.Y;
        position.Z = 0.0f;
        position = unknown_de4 * position;
        return CoordStruct((int)position.X, (int)position.Y, 0);
    }
    return unknown_geometry_l;
}


// CoordStruct Tactical::Unknown_006A2BF0(Point2D const &point) (0x006A2BF0): a point inside the
// screen rectangle 0x00ABE560 run through the projection +0xDE4 to a ground coordinate; otherwise
// the unit's empty coordinate (header static unknown_geometry_l).

CoordStruct Tactical::Unknown_006A2BF0(Point2D const &point)
{
    int x = point.X;
    if (x < unknown_rect_abe560.Width) {
        int y = point.Y;
        if (y < unknown_rect_abe560.Height) {
            Vector3 position;
            position.X = (float)x;
            position.Y = (float)y;
            position.Z = 0.0f;
            position = unknown_de4 * position;
            return CoordStruct((int)position.X, (int)position.Y, 0);
        }
    }
    return unknown_geometry_l;
}


// void Tactical::Setup_Trigger_Scroll(CoordStruct const &coord, int speed) (0x006A2CB0): starts a trigger scroll
// to a coordinate: the progress cleared, the step from the speed table (0x007F6914), the target the
// coordinate's absolute pixel (Coord_To_Pixel plus the view position), the start the current view position
// (TibSun Tactical::Setup_Trigger_Scroll; TeamClass script handler 0x006BC4C0).

extern float unknown_scroll_speeds_7f6914[5];      // 0x007F6914 (TibSun's static _speeds)

void Tactical::Setup_Trigger_Scroll(CoordStruct const &coord, int speed)
{
    unknown_dc = 0;
    unknown_d8 = unknown_scroll_speeds_7f6914[speed];

    Point2D to;
    Coord_To_Pixel(coord, to);
    to.X += TacPixelX;
    to.Y += TacPixelY;
    unknown_d0 = to;
    unknown_c8 = unknown_d64;
}


// void Tactical::Register_Dirty_Area(Rect area, bool refresh_shroud) (0x006A3020): adds a
// screen area to the dirty areas. An area inside a registered one is dropped
// (unless only it refreshes the shroud), registered areas inside it are removed,
// and otherwise the registered area whose union with it is smallest (and smaller
// than both apart) grows to that union (TibSun Tactical::Register_Dirty_Area).

void Tactical::Register_Dirty_Area(Rect area, bool refresh_shroud)
{
    int best_index = -1;
    int best = 0x7FFFFFFF;

    for (int i = 0; i < TacticalLists::DirtyAreas.Count(); i++) {
        Rect rect = TacticalLists::DirtyAreas[i].Area;
        Rect inter = Intersect(rect, area);
        Rect whole = Union(rect, area);

        if (inter.Is_Valid()) {
            if (inter == area) {
                if (!refresh_shroud || TacticalLists::DirtyAreas[i].unknown_10) {
                    return;
                }
            } else if (inter == rect) {
                if (refresh_shroud || !TacticalLists::DirtyAreas[i].unknown_10) {
                    TacticalLists::DirtyAreas.Delete(i);
                    i--;
                }
            } else {
                int size = whole.Size();
                if (size < (rect.Size() + area.Size()) && size < best) {
                    best = size;
                    best_index = i;
                }
            }
        }
    }

    if (best_index == -1) {
        TacticalLists::DirtyAreas.Add(DirtyAreaStruct(area, refresh_shroud));
    } else {
        TacticalLists::DirtyAreas[best_index] = DirtyAreaStruct(Union(area, TacticalLists::DirtyAreas[best_index].Area), refresh_shroud || TacticalLists::DirtyAreas[best_index].unknown_10);
    }
}


// 0x006a33e0: function not yet identified
//
// Body: 3 bytes (frozen inventory bound).
// Referenced from 0 sites.
// Ghidra prototype: void unknown_empty_006a33e0(void)
//
// Types and names are Ghidra placeholders;
// rename and retype as the purpose becomes known.
//

namespace seed_006a33e0 {

void __stdcall unknown_empty_006a33e0(int unused_0, int unused_1)
{
    return;
}

}  // namespace seed_006a33e0


// void Tactical::Wipe_Depth(bool fullredraw, int xoff, int yoff, Rect const &cliprect) (0x006A33F0): unless the
// whole view is redrawn, moves every dirty area by the scroll offset and clips it to the view (dropping those
// left empty, clearing the alpha buffer under the rest), then wipes the depth of every cell flagged for redraw
// (TibSun Tactical::Wipe_Depth).
#include "shared/cell.hpp"
#include "shared/display.hpp"
#include "shared/blit.hpp"
#include "shared/win32_window.hpp"

// Point quotient by a scalar, zero giving the origin (TibSun RectPoint2D::operator/; file-local: as a Point2D
// member it changes another unit's output). The sum is rect.hpp's Point2D::operator+.
inline Point2D operator/(Point2D const &point, int value)
{
    if (value == 0) {
        return Point2D(0, 0);
    }
    return Point2D(point.X / value, point.Y / value);
}

// The rectangle moved back by a point and its corner (TibSun TRect::operator- and Top_Left, which this unit
// expands; the other units call their out-of-line copies 0x0047D000 and 0x00435560).
inline RectPoint2D Corner(Rect const &rect)
{
    RectPoint2D point;
    point.Y = rect.Y;
    point.X = rect.X;
    return point;
}

inline Rect Moved_Back(Rect const &rect, RectPoint2D const &point)
{
    return Rect(Corner(rect) - point, rect.Width, rect.Height);
}

void Tactical::Wipe_Depth(bool fullredraw, int xoff, int yoff, Rect const &cliprect)
{
    if (!fullredraw && TacticalLists::DirtyAreas.Count()) {
        for (int i = TacticalLists::DirtyAreas.Count() - 1; i >= 0; i--) {
            Rect redraw = TacticalLists::DirtyAreas[i].Area;
            redraw.X += xoff;
            redraw.Y += yoff;
            redraw = Intersect(Moved_Back(unknown_rect_839630, Corner(unknown_rect_839630)), redraw);
            TacticalLists::DirtyAreas[i].Area = redraw;

            if (!redraw.Is_Valid()) {
                TacticalLists::DirtyAreas.Delete(i);
            } else {
                DepthBuffer->Update(redraw);
            }
        }
    }

    if (unknown_e0 > 0 && !fullredraw) {
        for (int i = 0; i < unknown_e0; i++) {
            CoordStruct coord = unknown_e4[i]->unknown_475d60();
            coord.Z = 0;
            Point2D pixel;
            Coord_To_Pixel(coord, pixel);
            Point2D draw = pixel + Point2D(60, 30) / -2;
            draw = draw + Point2D(unknown_rect_839630.X, unknown_rect_839630.Y);

            if (MainWindow) {
                unknown_e4[i]->unknown_4754b0(draw, cliprect);
            }
        }
    }
}


// void Tactical::Render_Tiles(Rect const &xpanrect, Rect const &ypanrect, bool fullredraw)
// (0x006A3670): draws the iso tiles (0x006A7BA0) of the whole view, or of the strips the pan exposed, then of every
// cell flagged for redraw that is in view and of every dirty area, last first
// (TibSun Tactical::Render_Tiles).

// The cell's map position as a copy (TibSun's CellClass::Fetch_CellID accessor; a CellClass
// member changes another unit's output, so it stays file-local here).
inline Cell Fetch_CellID(CellClass const *cell)
{
    return cell->MapCoords;
}

void Tactical::Render_Tiles(Rect const &xpanrect, Rect const &ypanrect, bool fullredraw)
{
    if (fullredraw) {
        Draw_Tiles(unknown_rect_839630, unknown_rect_839630);
    } else {
        if (xpanrect.Is_Valid()) {
            Draw_Tiles(xpanrect, xpanrect);
        }
        if (ypanrect.Is_Valid()) {
            Draw_Tiles(ypanrect, ypanrect);
        }
    }

    if (unknown_e0 > 0 && !fullredraw) {
        for (int i = 0; i < unknown_e0; i++) {
            CoordStruct coord = Coord_Snap(Cell_Center(unknown_e4[i]->MapCoords));
            Point2D pixel;
            Coord_To_Pixel(coord, pixel);
            Rect rect = Rect(pixel.X - 30, pixel.Y, 60, 30);
            rect.Y += unknown_rect_839630.Y;

            Rect inter = Intersect(unknown_rect_839630, rect);

            if (inter.Is_Valid()) {
                Draw_Tiles(Fetch_CellID(unknown_e4[i]), rect);
            }
        }
    }

    if (TacticalLists::DirtyAreas.Count() && !fullredraw) {
        for (int j = TacticalLists::DirtyAreas.Count() - 1; j >= 0; j--) {
            Rect redraw = TacticalLists::DirtyAreas[j].Area;
            redraw.Y += unknown_rect_839630.Y;
            redraw = Intersect(unknown_rect_839630, redraw);

            if (redraw.Is_Valid()) {
                Draw_Tiles(redraw, redraw);
            }
        }
    }
}


// void Tactical::Render_Tile_Shadows(Rect const &xpanrect, Rect const &ypanrect, bool fullredraw)
// (0x006A38D0): draws the tile shadows (0x006A8240) of the whole view, or of the strips the pan exposed, then of every
// cell flagged for redraw that is in view and of every dirty area, last first
// (TibSun Tactical::Render_Tile_Shadows).

void Tactical::Render_Tile_Shadows(Rect const &xpanrect, Rect const &ypanrect, bool fullredraw)
{
    if (fullredraw) {
        Draw_Tile_Shadows(unknown_rect_839630, unknown_rect_839630);
    } else {
        if (xpanrect.Is_Valid()) {
            Draw_Tile_Shadows(xpanrect, xpanrect);
        }
        if (ypanrect.Is_Valid()) {
            Draw_Tile_Shadows(ypanrect, ypanrect);
        }
    }

    if (unknown_e0 > 0 && !fullredraw) {
        for (int i = 0; i < unknown_e0; i++) {
            CoordStruct coord = Coord_Snap(Cell_Center(unknown_e4[i]->MapCoords));
            Point2D pixel;
            Coord_To_Pixel(coord, pixel);
            Rect rect = Rect(pixel.X - 30, pixel.Y, 60, 30);
            rect.Y += unknown_rect_839630.Y;

            Rect inter;
            inter = Intersect(unknown_rect_839630, rect);

            if (inter.Is_Valid()) {
                Draw_Tile_Shadows(inter, inter);
            }
        }
    }

    if (TacticalLists::DirtyAreas.Count()) {
        for (int j = TacticalLists::DirtyAreas.Count() - 1; j >= 0; j--) {
            Rect redraw = TacticalLists::DirtyAreas[j].Area;
            redraw.Y += unknown_rect_839630.Y;
            redraw = Intersect(unknown_rect_839630, redraw);

            if (redraw.Is_Valid()) {
                Draw_Tile_Shadows(redraw, redraw);
            }
        }
    }
}


// void Tactical::Render_Overlays(Rect const &xpanrect, Rect const &ypanrect, bool fullredraw)
// (0x006A3B20): draws the overlays (0x006A7350) of the whole view, or of the
// strips the pan exposed, then of every cell flagged for redraw and of every
// dirty area, last first (TibSun Tactical::Render_Overlays).

void Tactical::Render_Overlays(Rect const &xpanrect, Rect const &ypanrect, bool fullredraw)
{
    if (fullredraw) {
        Draw_Overlays(unknown_rect_839630);
    } else {
        if (xpanrect.Is_Valid()) {
            Draw_Overlays(xpanrect);
        }
        if (ypanrect.Is_Valid()) {
            Draw_Overlays(ypanrect);
        }
    }

    if (unknown_e0 > 0 && !fullredraw) {
        for (int i = 0; i < unknown_e0; i++) {
            CoordStruct coord = Coord_Snap(Cell_Center(unknown_e4[i]->MapCoords));
            Point2D pixel;
            Coord_To_Pixel(coord, pixel);
            Rect rect = Rect(pixel.X - 30, pixel.Y, 60, 30);
            rect.Y += unknown_rect_839630.Y;

            Draw_Overlays(rect);
        }
    }

    if (TacticalLists::DirtyAreas.Count()) {
        for (int i = TacticalLists::DirtyAreas.Count() - 1; i >= 0; i--) {
            Rect redraw = TacticalLists::DirtyAreas[i].Area;
            redraw.Y += unknown_rect_839630.Y;
            redraw = Intersect(unknown_rect_839630, redraw);

            if (redraw.Is_Valid()) {
                Draw_Overlays(redraw);
            }
        }
    }
}


// void Tactical::Render_Fogged_Objects(Rect const &xpanrect, Rect const &ypanrect, bool fullredraw)
// (0x006A3D00): draws the fogged objects (0x006A9F50, expanded) of the whole view, or of the
// strips the pan exposed, then of every cell flagged for redraw and of every
// dirty area, last first (TibSun Tactical::Render_Fogged_Objects).

void Tactical::Render_Fogged_Objects(Rect const &xpanrect, Rect const &ypanrect, bool fullredraw)
{
    if (fullredraw) {
        Draw_Fogged_Objects(unknown_rect_839630);
    } else {
        if (xpanrect.Is_Valid()) {
            Draw_Fogged_Objects(xpanrect);
        }
        if (ypanrect.Is_Valid()) {
            Draw_Fogged_Objects(ypanrect);
        }
    }

    if (unknown_e0 > 0 && !fullredraw) {
        for (int i = 0; i < unknown_e0; i++) {
            CoordStruct coord = Coord_Snap(Cell_Center(unknown_e4[i]->MapCoords));
            Point2D pixel;
            Coord_To_Pixel(coord, pixel);
            Rect rect = Rect(pixel.X - 30, pixel.Y, 60, 30);
            rect.Y += unknown_rect_839630.Y;

            Draw_Fogged_Objects(rect);
        }
    }

    if (TacticalLists::DirtyAreas.Count()) {
        for (int i = TacticalLists::DirtyAreas.Count() - 1; i >= 0; i--) {
            Rect redraw = TacticalLists::DirtyAreas[i].Area;
            redraw.Y += unknown_rect_839630.Y;
            redraw = Intersect(unknown_rect_839630, redraw);

            if (redraw.Is_Valid()) {
                Draw_Fogged_Objects(redraw);
            }
        }
    }
}


// void Tactical::Render_Shroud(Rect const &xpanrect, Rect const &ypanrect, Rect const &cliprect, bool fullredraw)
// (0x006A3EF0): redraws the shroud and fog of every cell flagged for redraw (then the alpha shapes over it), or
// of the whole view, then of the strips the pan exposed and of every dirty area that refreshes the shroud,
// clearing the depth buffer there first (TibSun Tactical::Render_Shroud).
#include "shared/alpha_shape.hpp"

void Tactical::Render_Shroud(Rect const &xpanrect, Rect const &ypanrect, Rect const &cliprect, bool fullredraw)
{
    if (unknown_e0 > 0 && !fullredraw) {
        for (int i = 0; i < unknown_e0; i++) {
            CoordStruct coord = unknown_e4[i]->unknown_475d60();
            coord.Z = 0;
            Point2D pixel;
            Coord_To_Pixel(coord, pixel);
            Point2D draw = pixel + Point2D(60, 30) / -2 + Point2D(unknown_rect_839630.X, unknown_rect_839630.Y);

            if (MainWindow) {
                unknown_e4[i]->Draw_Shroud_And_Fog(draw, cliprect);
            }
            AlphaShapeClass::Draw_In_Area(draw, cliprect);
        }
    } else if (fullredraw) {
        Draw_Shroud(unknown_rect_839630);
    }

    if (xpanrect.Is_Valid() && !fullredraw) {
        Draw_Shroud(xpanrect);
    }
    if (ypanrect.Is_Valid() && !fullredraw) {
        Draw_Shroud(ypanrect);
    }

    for (int j = 0; j < TacticalLists::DirtyAreas.Count(); j++) {
        Point2D corner = Point2D(TacticalLists::DirtyAreas[j].Area.X, TacticalLists::DirtyAreas[j].Area.Y) + Point2D(unknown_rect_839630.X, unknown_rect_839630.Y);
        Rect redraw = Rect(corner.X, corner.Y, TacticalLists::DirtyAreas[j].Area.Width, TacticalLists::DirtyAreas[j].Area.Height);
        Rect inter = Intersect(redraw, cliprect);

        if (TacticalLists::DirtyAreas[j].unknown_10) {
            AlphaBuffer->Update(inter);
            Draw_Shroud(inter);
        }
    }
}


// void Tactical::Render_Buildings(Rect const &xpanrect, Rect const &ypanrect, bool fullredraw)
// (0x006A4100): redraws the buildings (0x006A9F60) of every cell flagged
// for redraw, of every dirty area (last first), then of the whole view or of
// the strips the pan exposed (TibSun Tactical::Render_Buildings).

void Tactical::Render_Buildings(Rect const &xpanrect, Rect const &ypanrect, bool fullredraw)
{
    int i;

    if (unknown_e0 > 0) {
        for (i = 0; i < unknown_e0; i++) {
            CoordStruct coord = Coord_Snap(Cell_Center(unknown_e4[i]->MapCoords));
            Point2D pixel;
            Coord_To_Pixel(coord, pixel);

            Rect rect = Rect(pixel.X - 30, pixel.Y, 60, 30);
            rect.Y += unknown_rect_839630.Y;
            Rect cliprect = rect;

            Unknown_006A9F60(true, rect, cliprect);
        }
    }

    if (TacticalLists::DirtyAreas.Count()) {
        for (i = TacticalLists::DirtyAreas.Count() - 1; i >= 0; i--) {
            Rect redraw = TacticalLists::DirtyAreas[i].Area;
            redraw.Y += unknown_rect_839630.Y;
            redraw = Intersect(unknown_rect_839630, redraw);

            if (redraw.Is_Valid()) {
                Rect cliprect = redraw;
                Unknown_006A9F60(true, redraw, cliprect);
            }
        }
    }

    if (fullredraw) {
        Unknown_006A9F60(true, unknown_rect_839630, unknown_rect_839630);
    } else {
        if (xpanrect.Is_Valid()) {
            Unknown_006A9F60(true, xpanrect, xpanrect);
        }
        if (ypanrect.Is_Valid()) {
            Unknown_006A9F60(true, ypanrect, ypanrect);
        }
    }
}


// void Tactical::Render_Terrain(Rect const &xpanrect, Rect const &ypanrect, bool fullredraw)
// (0x006A4350): redraws the terrain objects (0x006A9E10) of every cell flagged
// for redraw, of every dirty area (last first), then of the whole view or of
// the strips the pan exposed (TibSun Tactical::Render_Terrain).

void Tactical::Render_Terrain(Rect const &xpanrect, Rect const &ypanrect, bool fullredraw)
{
    int i;

    if (unknown_e0 > 0) {
        for (i = 0; i < unknown_e0; i++) {
            CoordStruct coord = Coord_Snap(Cell_Center(unknown_e4[i]->MapCoords));
            Point2D pixel;
            Coord_To_Pixel(coord, pixel);

            Rect rect = Rect(pixel.X - 30, pixel.Y, 60, 30);
            rect.Y += unknown_rect_839630.Y;
            Rect cliprect = rect;

            Unknown_006A9E10(true, rect, cliprect);
        }
    }

    if (TacticalLists::DirtyAreas.Count()) {
        for (i = TacticalLists::DirtyAreas.Count() - 1; i >= 0; i--) {
            Rect redraw = TacticalLists::DirtyAreas[i].Area;
            redraw.Y += unknown_rect_839630.Y;
            redraw = Intersect(unknown_rect_839630, redraw);

            if (redraw.Is_Valid()) {
                Rect cliprect = redraw;
                Unknown_006A9E10(true, redraw, cliprect);
            }
        }
    }

    if (fullredraw) {
        Unknown_006A9E10(true, unknown_rect_839630, unknown_rect_839630);
    } else {
        if (xpanrect.Is_Valid()) {
            Unknown_006A9E10(true, xpanrect, xpanrect);
        }
        if (ypanrect.Is_Valid()) {
            Unknown_006A9E10(true, ypanrect, ypanrect);
        }
    }
}


// void __fastcall Draw_Timer(int line, ColorScheme *scheme, int value,
// wchar_t const *text, unsigned __int64 *flash_time, bool *flash) (0x006A5280): the
// timer captions Render draws above the bottom right corner of the tactical
// view, one per line: the text and the time left (seconds at 60 a minute,
// hours shown only when present). At zero the caption flashes white, its
// state flipping once a second (name and parameters hypotheses).
#include "shared/text_print.hpp"
#include "shared/color_scheme.hpp"
#include "shared/audio.hpp"
#include "shared/string_table.hpp"

void __fastcall Draw_Timer(int line, ColorScheme *scheme, int value, wchar_t const *text,
                                  unsigned __int64 *flash_time, bool *flash)
{
    wchar_t buffer[256];
    BitFontClass *font = Font_For_Style(0x4618);

    int hours = value / 60 / 60;
    int seconds = value % 60;
    int minutes = value / 60 % 60;
    if (hours) {
        swprintf(buffer, L"%s  %d:%02d:%02d", text, hours, minutes, seconds);
    } else {
        swprintf(buffer, L"%s  %02d:%02d", text, minutes, seconds);
    }

    if (value == 0 && flash_time && flash) {
        if (*flash_time <= Audio_Now()) {
            *flash_time = Audio_Now() + 1000;
            *flash = !*flash;
        }
        if (*flash) {
            scheme = ColorSchemes[0];
        }
    }

    Fancy_Text_Printf(buffer, unknown_surface_839964, &unknown_surface_839964->Get_Rect(),
        &Point2D(unknown_rect_839630.Width - 3, unknown_rect_839630.Height - (font->Height + 2) * (line + 1)), scheme, 0, 0x4618);
}


// 0x006a5440: function not yet identified
//
// Body: 17 bytes (frozen inventory bound).
// Referenced from 0 sites.
// Ghidra prototype: void __fastcall Clear_Caption_Text_006a5440(int param_1)
//
// Types and names are Ghidra placeholders;
// rename and retype as the purpose becomes known.
//

namespace seed_006a5440 {

void __fastcall Clear_Caption_Text_006a5440(int self)
{
    *(unsigned int *)(self + 0xa4) = 0xffffffff;
    *(unsigned short *)(self + 0x24) = 0;
    return;
}

}  // namespace seed_006a5440


// void Tactical::Draw_Screen_Text(wchar_t const *text) (0x006A5460): unless images are being reloaded, draws the
// caption over the middle of the tactical view on a DirectDraw surface: frame unknown_a4 of GRFXTXT.SHP when one is
// set, else the text in white, centred in the bitmap font (TibSun Tactical::Draw_Screen_Text).
// The unit keeps no empty-rectangle copy of its own: the shape helpers fall back on its header static rectangle.
#define SHAPE_FILE_EMPTY_RECT unknown_static_rect
#include "shared/surface.hpp"
#include "shared/palette.hpp"
#include "shared/hicolor.hpp"
#include "shared/tag_rect.hpp"
#include "shared/msfont.hpp"

void Tactical::Draw_Screen_Text(wchar_t const *text)
{
    if (Debug_Map) {
        return;
    }
    if ((text == 0 || !wcslen(text)) && unknown_a4 == -1) {
        return;
    }
    if (unknown_surface_839964->Is_Direct_Draw()) {
        int frame = unknown_a4;
        if (frame != -1 && unknown_grfxtxt_drawer_a41678 != 0 && unknown_grfxtxt_shape_a41674 != 0
            && ((ShapeFileStruct const *)unknown_grfxtxt_shape_a41674)->FrameCount > frame) {
            ShapeFileStruct const *shape = (ShapeFileStruct const *)unknown_grfxtxt_shape_a41674;
            Point2D point((unknown_rect_839630.Width - shape->Width) / 2, (unknown_rect_839630.Height - shape->Height) / 2);
            CC_Draw_Shape(unknown_surface_839964, unknown_grfxtxt_drawer_a41678, shape, frame, point,
                unknown_rect_839630, 0, 0, 0, 0, 1000, 0, 0, Point2D(0, 0));
            return;
        }
        if (text != 0 && wcslen(text) > 0) {
            BitFontClass *font = BitFont;
            Rect rect = unknown_rect_839630;
            unsigned short color = Build_Hicolor_Pixel(255, 255, 255);
            int width;
            int height;
            font->Get_Text_Area(text, &width, &height, rect.Width);
            int x = (rect.Width - width) / 2 + rect.X;
            int y = rect.Y + (rect.Height - height) / 2;
            tagRECT box;
            box.left = rect.X;
            box.top = rect.Y;
            box.right = rect.X + rect.Width;
            box.bottom = rect.Height + rect.Y;
            font->Set_Clipping(true);
            font->Set_Clip_Box(&box);
            font->Set_Color(color);
            BitText->Print(font, unknown_surface_839964, text, x, y, 0, 0);
        }
    }
}


// void Tactical::Draw_Placement(bool drawtrans) (0x006A5670): marks the footprint of the object waiting for placement
// and of the secondary cell list, and previews the run of a wall, fence or firestorm post (TibSun Tactical::Draw_Placement).
#include "shared/map_view.hpp"
#include "shared/building.hpp"
#include "shared/house.hpp"
#include "shared/mouse.hpp"
#include "shared/building_type_class.hpp"
#include "shared/overlay_type.hpp"

void Tactical::Draw_Placement(bool drawtrans)
{
    Cell start = Cell(0, 0);
    Cell dims;

    if (Map.unknown_11a4 != 0) {
        Point2D mouse = WWMouse->Get_Mouse_XY();
        if (!unknown_rect_839630.Is_Point_Within(mouse)) {
            return;
        }

        if (Map.unknown_11a4->Kind_Of() == 7 && ((BuildingTypeClass *)Map.unknown_11a4)->Gate) {
            dims = Cell(3, 3);
            start = Cell(-1, -1);
        } else {
            dims = Map.Unknown_00499810(Map.unknown_1178);
        }

        bool all_valid = true;
        for (int cx = start.X; cx < dims.X; cx++) {
            for (int cy = start.Y; cy < dims.Y; cy++) {
                Cell cell = Cell(cx, cy) + Map.unknown_1170 + Map.unknown_1174;
                CoordStruct coord = Coord_Snap(CoordStruct(cell.X * 256 + 128, cell.Y * 256 + 128, 0));
                Point2D pixel;
                Coord_To_Pixel(coord, pixel);
                pixel.X = pixel.X - 30;
                if (Map.In_Radar(cell)) {
                    CellClass *cellptr = &Map[cell];
                    all_valid = cellptr->Draw_Placement_Cursor(pixel, unknown_rect_839630, drawtrans) && all_valid;
                }
            }
        }

        int rtti = Map.unknown_11a4->Kind_Of();

        if (Debug_Map && Map.unknown_11a0 != 0) {
            Cell cell = start + Map.unknown_1170 + Map.unknown_1174;
            CoordStruct coord = CoordStruct(cell.X * 256 + 128, cell.Y * 256 + 128, 0);
            if (rtti == 7) {
                coord = Coord_Snap(coord);
            }
            coord.Z = Map.Get_Height_GL(coord);
            Map.unknown_11a0->Set_Coord(coord);
            Point2D pixel;
            Coord_To_Pixel(coord, pixel);
            if (rtti != 0x12) {
                Map.unknown_11a0->Editor_Draw_It(pixel, unknown_rect_839630);
            }
        }

        if (all_valid && Map.unknown_11a0 != 0 && Map.unknown_11a0->Kind_Of() == 6) {
            BuildingClass *bptr = (BuildingClass *)Map.unknown_11a0;
            if (bptr->Type->LaserFencePost) {
                Draw_Fence_Placement(drawtrans, Cell(start.X, start.Y) + Map.unknown_1170 + Map.unknown_1174);
            } else if (bptr->Type->FirestormWall) {
                Draw_Firestorm_Wall_Placement(drawtrans, Cell(start.X, start.Y) + Map.unknown_1170 + Map.unknown_1174);
            } else if (bptr->Type->ToOverlay != 0 && bptr->Type->ToOverlay->Wall) {
                Draw_Wall_Placement(drawtrans, Cell(start.X, start.Y) + Map.unknown_1170 + Map.unknown_1174);
            }
        }
    }

    if (!Debug_Map && Map.unknown_1190 != 0) {
        if (((ObjectTypeClass *)Map.unknown_1190)->Kind_Of() == 7 && ((BuildingTypeClass *)Map.unknown_1190)->Gate) {
            dims = Cell(3, 3);
            start = Cell(-1, -1);
        } else {
            dims = Map.Unknown_00499810(Map.unknown_1188);
        }
        for (int cx = start.X; cx < dims.X; cx++) {
            for (int cy = start.Y; cy < dims.Y; cy++) {
                Cell cell = Cell(cx, cy) + Map.unknown_117e + Map.unknown_1182;
                CoordStruct coord = Coord_Snap(CoordStruct(cell.X * 256 + 128, cell.Y * 256 + 128, 0));
                Point2D pixel;
                Coord_To_Pixel(coord, pixel);
                pixel.X = pixel.X - 30;
                if (Map.In_Radar(cell)) {
                    CellClass *cellptr = &Map[cell];
                    cellptr->Draw_Placement_Cursor(pixel, unknown_rect_839630, drawtrans);
                }
            }
        }
    }
}


// Complete laser-fence placement preview, original 0x006A5D70.
void Tactical::Draw_Fence_Placement(bool drawtrans, Cell cell)
{
    for (int index = 0; index < 8; index += 2) {
        Cell scan = cell;
        int count = 0;
        int range = ((BuildingClass *)Map.unknown_11a0)->Type->GuardRange.Value >> 8;
        if (range <= 0) continue;
        int facing = index;
        BuildingClass *building;
        bool found = false;
        while (true) {
            scan = Adjacent_Cell(scan, facing);
            building = (BuildingClass *)Map[scan].Cell_Building();
            if (building) {
                found = true;
                break;
            }
            CellClass *nearby = &Map[scan];
            if (nearby->Cell_Terrain(false) || nearby->unknown_11c > 0 || nearby->unknown_4c != 0 || ++count >= range) break;
        }
        if (found && building->Type->LaserFencePost && building->House->Is_Player_Control() && count > 0) {
            int flags = drawtrans ? 0x10606 : 0x20606;
            Cell drawcell = Adjacent_Cell(cell, facing);
            for (int i = 0; i < count; ++i) {
                int height = 15 * Map[drawcell].unknown_11b;
                int zheight = 15 * Map[drawcell].unknown_11b;
                Point2D offset(0, -1 - height);
                int zoff = -2 - zheight;
                CoordStruct coord = Coord_Snap(Cell_Center(drawcell));
                Point2D pixel;
                Coord_To_Pixel(coord, pixel);
                pixel.X -= 30;
                int px = pixel.X + 30;
                int py = offset.Y + pixel.Y;
                Point2D point(px, py);
                CC_Draw_Shape(unknown_surface_83995c, unknown_drawer_8323c4, (ShapeFileStruct const *)DisplayClass::TransIconset, 0, point,
                    unknown_rect_839630, flags, 0, zoff, 0, 1000, 0, 0, Point2D(0, 0));
                drawcell = Adjacent_Cell(drawcell, facing);
            }
        }
    }
}


// void Tactical::Draw_Firestorm_Wall_Placement(bool drawtrans, Cell cell) (0x006A6010): from the pending firestorm
// wall's cell, walks each of the four directions while the cells are clear to build, up to the wall's guard range; when
// the run ends at a player's firestorm wall, draws the placement shape over every cell walked (TibSun
// Tactical::Draw_Firestorm_Wall_Placement).
#include "shared/gadget.hpp"

void Tactical::Draw_Firestorm_Wall_Placement(bool drawtrans, Cell cell)
{
    for (int index = 0; index < 8; index += 2) {
        Cell scan = cell;
        int count = 0;
        BuildingTypeClass *type = ((BuildingClass *)Map.unknown_11a0)->Type;
        int range = type->GuardRange.Value >> 8;
        int facing = index;
        bool found = false;
        for (; count < range; count++) {
            scan = Adjacent_Cell(scan, facing);
            BuildingClass *building = (BuildingClass *)Map[scan].Cell_Building();
            if (building != 0 && building->Type->FirestormWall && building->House->Is_Player_Control()) {
                found = true;
                break;
            }
            if (!Map[scan].Is_Clear_To_Build(type->SpeedType, type, unknown_player_a35db4)) {
                break;
            }
        }

        if (found && count > 0) {
            int flags = drawtrans ? 0x10606 : 0x20606;
            Cell drawcell = Adjacent_Cell(cell, facing);
            for (int i = 0; i < count; i++) {
                int height = 15 * Map[drawcell].unknown_11b;
                int zheight = 15 * Map[drawcell].unknown_11b;
                Point2D offset(0, -1 - height);
                int zoff = -2 - zheight;
                CoordStruct coord = Coord_Snap(Cell_Center(drawcell));
                Point2D pixel;
                Coord_To_Pixel(coord, pixel);
                pixel.X -= 30;
                int px = pixel.X + 30;
                int py = offset.Y + pixel.Y;
                Point2D point(px, py);
                CC_Draw_Shape(unknown_surface_83995c, unknown_drawer_8323c4, (ShapeFileStruct const *)DisplayClass::TransIconset, 0, point,
                    unknown_rect_839630, flags, 0, zoff, 0, 1000, 0, 0, Point2D(0, 0));
                drawcell = Adjacent_Cell(drawcell, facing);
            }
        }
    }
}


// void Tactical::Draw_Wall_Placement(bool drawtrans, Cell cell) (0x006A6290): when the pending building builds a wall
// overlay, walks each of the four directions from its cell while the cells are clear to build, up to its guard range;
// when the run ends at the player's own wall overlay of that type, draws the placement shape over every cell walked
// (TibSun Tactical::Draw_Wall_Placement).

void Tactical::Draw_Wall_Placement(bool drawtrans, Cell cell)
{
    OverlayTypeClass const *to_overlay = ((BuildingClass *)Map.unknown_11a0)->Type->ToOverlay;
    if (to_overlay == 0) {
        return;
    }

    for (int index = 0; index < 8; index += 2) {
        Cell scan = cell;
        int count = 0;
        BuildingTypeClass *type = ((BuildingClass *)Map.unknown_11a0)->Type;
        int range = type->GuardRange.Value >> 8;
        if (range <= 0) {
            continue;
        }
        bool found = false;
        for (; count < range; count++) {
            scan = Adjacent_Cell(scan, index);
            CellClass *cellptr = &Map[scan];
            if (cellptr->OverlayIndex != -1 && to_overlay->ArrayIndex == cellptr->OverlayIndex && unknown_player_a35db4->ArrayIndex == Map[scan].unknown_50) {
                found = true;
                break;
            }
            if (!Map[scan].Is_Clear_To_Build(type->SpeedType, type, unknown_player_a35db4)) {
                break;
            }
        }

        if (found && count > 0) {
            int flags = drawtrans ? 0x10606 : 0x20606;
            Cell drawcell = cell;
            drawcell = Adjacent_Cell(drawcell, index);
            for (int i = 0; i < count; i++) {
                int height = 15 * Map[drawcell].unknown_11b;
                int zoff = 15 * Map[drawcell].unknown_11b;
                Point2D offset(0, -1 - height);
                zoff = -2 - zoff;
                CoordStruct coord = Coord_Snap(Cell_Center(drawcell));
                Point2D pixel;
                Coord_To_Pixel(coord, pixel);
                pixel.X -= 30;
                Point2D point;
                point.X = pixel.X + 30;
                point.Y = pixel.Y + offset.Y;
                CC_Draw_Shape(unknown_surface_83995c, unknown_drawer_8323c4, (ShapeFileStruct const *)DisplayClass::TransIconset, 0, point,
                    unknown_rect_839630, flags, 0, zoff, 0, 1000, 0, 0, Point2D(0, 0));
                drawcell = Adjacent_Cell(drawcell, index);
            }
        }
    }
}


// bool Tactical::Blit_Tiles() (0x006A6570): copies the tactical area of the tile
// surface (0x00839944) onto the drawing surface (0x0083995C) (TibSun
// Tactical::Blit_Tiles).

bool Tactical::Blit_Tiles()
{
    return unknown_surface_83995c->Copy_From(unknown_rect_839630, *unknown_surface_839944, unknown_rect_839630, false, true);
}


// Point2D Tactical::Get_Tactical_Position() (0x006A67B0): the view position (+0xD64).

Point2D Tactical::Get_Tactical_Position()
{
    return unknown_d64;
}


// Point2D Tactical::Unknown_006A67D0() (0x006A67D0): the view position less the
// tactical pixel origin (TibSun Tactical::Get_Relative_Tactical_Position).

Point2D Tactical::Unknown_006A67D0()
{
    Point2D offset(TacPixelX, TacPixelY);
    return unknown_d64 - offset;
}


// void __fastcall Unknown_006A6800(int x, int y, int *pixel_x, int *pixel_y)
// (0x006A6800): a lepton offset to its screen offset before the division by
// 256. Never called: the Tactical unit expands it inline (0x006A2740 and the
// coordinate-to-pixel family).

void __fastcall Unknown_006A6800(int x, int y, int *pixel_x, int *pixel_y)
{
    *pixel_x = x * 60 / 2;
    *pixel_y = x * 30 / 2;
    *pixel_x += y * -60 / 2;
    *pixel_y += y * 30 / 2;
}


// 0x006a6860: function not yet identified
//
// Body: 81 bytes (frozen inventory bound).
// Referenced from 0 sites.
// Ghidra prototype: void __fastcall Isometric_To_Rectangular_006a6860(int param_1,int param_2,int *param_3,int *param_4)
//
// Types and names are Ghidra placeholders;
// rename and retype as the purpose becomes known.
//

namespace seed_006a6860 {

void __fastcall Isometric_To_Rectangular_006a6860(int x, int y, int *out_x, int *out_y)
{
    *out_x = ((y * 2 + x) * 0xf) / 0x240 + -0x10000;
    *out_y = ((y * 2 - x) * 0xf) / 0x240 + 0x10000;
    return;
}

}  // namespace seed_006a6860


// void __stdcall Unknown_006A69B0(int x, int y, int *cell_x, int *cell_y) (0x006A69B0): a screen
// point back to lepton coordinates relative to an origin pair (0x007F6928/0x007F692C, -1 in the
// data until first use, then (-0x10000, 0x10000)). Never called (inlined everywhere); hypothesis.
extern int unknown_origin_x_7f6928;
extern int unknown_origin_y_7f692c;

void __stdcall Unknown_006A69B0(int x, int y, int *cell_x, int *cell_y)
{
    x *= 256;
    y *= 256;
    int lepton_x = (y * 2 + x) * 15 / 576 - 0x10000;
    int lepton_y = (y * 2 - x) * 15 / 576 + 0x10000;
    if (unknown_origin_x_7f6928 == -1 && unknown_origin_y_7f692c == -1) {
        unknown_origin_y_7f692c = 0x10000;
        unknown_origin_x_7f6928 = -0x10000;
    }
    lepton_x -= unknown_origin_x_7f6928;
    lepton_y -= unknown_origin_y_7f692c;
    *cell_x = lepton_x;
    *cell_y = lepton_y;
}


// Cell Tactical::Coord_To_Cell(CoordStruct const &coord) (0x006A6A50): the cell
// drawn over a coordinate: from six cells down the view diagonal, steps back 8
// leptons at a time until a cell, lowered by its height difference to the
// coordinate's cell (plus four levels under a bridge when that cell is a
// bridge surface), lies at or before the cell (TibSun Tactical::Coord_To_Cell).
#include "shared/object.hpp"

Cell Tactical::Coord_To_Cell(CoordStruct const &coord)
{
    Cell cell = Coord_Cell(coord);
    CoordStruct center = Cell_Coord(cell);

    bool isbridge = (Map[cell].unknown_140 >> 12) & 1;
    int baseheight = Map[cell].unknown_11b;

    CoordStruct scan = CoordStruct(center.X + 256 * 6, center.Y + 256 * 6, 0);

    Cell scancell = cell;
    while (true) {
        scan -= CoordStruct(8, 8, 0);
        scancell = Coord_Cell(scan);

        CellClass *cellptr = &Map[scancell];
        int heightdiff = cellptr->unknown_11b - baseheight;
        if (isbridge) {
            heightdiff += 4 * ((cellptr->unknown_140 >> 8) & 1);
        }

        Cell projected = Coord_Cell(scan - CoordStruct(heightdiff * 128, heightdiff * 128, 0));
        if (projected.X <= cell.X && projected.Y <= cell.Y) {
            return scancell;
        }
        if (scancell == cell) {
            return cell;
        }
    }
}


// Cell Tactical::Pixel_To_Cell(Point2D const &point) (0x006A6BD0): the cell under a
// screen point: scans up to 180 pixels below the point for the first cell whose
// lifted top lies at or above it, allowing for bridges (TibSun Tactical::Pixel_To_Cell).
// Unlike TibSun's early returns, the found cell is stored into the fallback and the scan
// breaks to one shared return: the original copies every result through one register.

Cell Tactical::Pixel_To_Cell(Point2D const &point)
{
    Point2D lepton = Unknown_006A2900(Point2D(TacPixelX, TacPixelY) + point - Point2D(unknown_rect_839630.X, unknown_rect_839630.Y));

    Cell fallback(lepton.X / 256, lepton.Y / 256);

    // The pixel of the point's offset within its cell (TibSun Coord_Fraction); never used.
    Point2D abspixel;
    Coord_To_Pixel(CoordStruct(lepton.X & 255, lepton.Y & 255, 0), abspixel);
    Point2D xy = point - Point2D(unknown_rect_839630.X, unknown_rect_839630.Y);

    Point2D scan = xy;
    scan.Y += 180;

    int count = 0;
    while (count < 180) {
        abspixel = scan + Point2D(TacPixelX, TacPixelY);

        Point2D scanlepton = Unknown_006A2900(abspixel);
        Cell cell(scanlepton.X / 256, scanlepton.Y / 256);

        CellClass *cellptr = &Map[cell];
        int celly = scan.Y - cellptr->unknown_11b * 15;

        if (cellptr->unknown_140 & 0x100) {
            CellClass &east = cellptr->Adjacent_Cell(2);
            CellClass &south = cellptr->Adjacent_Cell(4);

            bool bridge1 = (cellptr->unknown_140 & 0x800) && !(cellptr->Adjacent_Cell(0).unknown_140 & 0x100);
            bool bridge2 = !(cellptr->unknown_140 & 0x800) && !(cellptr->Adjacent_Cell(6).unknown_140 & 0x100);
            bool bridge3 = (cellptr->unknown_140 & 0x800) && !(south.unknown_140 & 0x100);
            bool bridge4 = !(cellptr->unknown_140 & 0x800) && !(east.unknown_140 & 0x100);
            bool bridge5 = (cellptr->unknown_140 & 0x800) && abs(cellptr->unknown_11b - east.unknown_11b) <= 1 && !(east.unknown_140 & 0x100);
            bool bridge6 = !(cellptr->unknown_140 & 0x800) && abs(cellptr->unknown_11b - south.unknown_11b) <= 1 && !(south.unknown_140 & 0x100);

            CoordStruct topcoord = Cell_Coord(cellptr->MapCoords) - CoordStruct(128, 128, 0);
            Point2D top;
            Coord_To_Pixel(topcoord, top);
            top.Y += 30 * cellptr->unknown_11b / -2;

            if (top.Y <= xy.Y) {
                if (bridge3 || bridge6) {
                    fallback = Cell(cell.X, cell.Y + 1);
                    break;
                }
                if (bridge4 || bridge5) {
                    fallback = Cell(cell.X + 1, cell.Y);
                    break;
                }
            }

            if (bridge1 || bridge2) {
                int dx = xy.X - top.X;
                int dy = xy.Y - top.Y;
                if ((bridge1 && dy - dx / 2 > 15) || (bridge2 && dx / 2 + dy > 15)) {
                    celly -= ((cellptr->unknown_140 & 0x100) ? 60 : 0);
                }
            } else {
                celly -= ((cellptr->unknown_140 & 0x100) ? 60 : 0);
            }
        }

        if (celly <= xy.Y) {
            fallback = cell;
            break;
        }

        scan.Y--;
        count++;
    }

    return fallback;
}


// int Tactical::Get_Cell_Height(CoordStruct const &coord) (0x006A70D0): the height
// of the ground under a coordinate, asked of its cell (TibSun
// Tactical::Get_Cell_Height).

int Tactical::Get_Cell_Height(CoordStruct const &coord)
{
    CellClass *cellptr = &Map[coord];
    return cellptr->unknown_4706c0(Point2D(coord.X, coord.Y));
}


// void Tactical::Add_Shadow_Control(ShadowControlClass control) (0x006A7130): adds
// a heap copy of the control to the shadow controls (TibSun
// Tactical::Add_Shadow_Control).

void Tactical::Add_Shadow_Control(ShadowControlClass control)
{
    ShadowControlClass *ctrl = new ShadowControlClass(control);
    TacticalLists::ShadowControls.Add(ctrl);
}


// CellClass *Tactical::Find_Bridge_Owner_Cell(Cell const &cell) (0x006A71F0): the
// bridge deck cell (+0x140 bit 0x80, an overlay) whose span covers the cell:
// east-west decks (bit 0x800) two and one cells east, scanning five cells
// south, then north-south decks two and one cells south, scanning east (TibSun
// Tactical::Find_Bridge_Owner_Cell; nothing calls it).

CellClass *Tactical::Find_Bridge_Owner_Cell(Cell const &cell)
{
    CellClass *cellptr;
    Cell scan;
    int i;

    scan = cell + Cell(2, 0);
    for (i = 0; i < 5; ++i) {
        cellptr = &Map[scan];
        if ((cellptr->unknown_140 & 0x80) && cellptr->OverlayIndex && (cellptr->unknown_140 & 0x800)) {
            return cellptr;
        }
        scan.Y++;
    }

    scan = cell + Cell(1, 0);
    for (i = 0; i < 5; ++i) {
        cellptr = &Map[scan];
        if ((cellptr->unknown_140 & 0x80) && cellptr->OverlayIndex && (cellptr->unknown_140 & 0x800)) {
            return cellptr;
        }
        scan.Y++;
    }

    scan = cell + Cell(0, 2);
    for (i = 0; i < 5; ++i) {
        cellptr = &Map[scan];
        if ((cellptr->unknown_140 & 0x80) && cellptr->OverlayIndex && !(cellptr->unknown_140 & 0x800)) {
            return cellptr;
        }
        scan.X++;
    }

    scan = cell + Cell(0, 1);
    for (i = 0; i < 5; ++i) {
        cellptr = &Map[scan];
        if ((cellptr->unknown_140 & 0x80) && cellptr->OverlayIndex && !(cellptr->unknown_140 & 0x800)) {
            return cellptr;
        }
        scan.X++;
    }

    return 0;
}


// void Tactical::Draw_Overlays(Rect const &area) (0x006A7350): draws the overlay
// of every cell of the diagonal band under area whose overlay rectangle meets the
// clipped area, then the overlay shadows over the same band (TibSun
// Tactical::Draw_Overlays).

void Tactical::Draw_Overlays(Rect const &area)
{
    Point2D leptons = Unknown_006A2900(Point2D(area.X, area.Y) + Point2D(TacPixelX, TacPixelY) - Point2D(unknown_rect_839630.X, unknown_rect_839630.Y));
    CoordStruct lepton(leptons.X, leptons.Y, 0);
    if (lepton.Y < 0) lepton.Y = 0;
    if (lepton.X < 0) lepton.X = 0;

    Cell origin = Map[lepton].MapCoords;
    Cell base = origin;

    int ycount = area.Height / 15 + 20;
    int xcount = area.Width / 60 + 4;

    Rect cliprect = Intersect(unknown_rect_839630, area);
    base += Cell(-4, -2);

    int ix, iy;
    for (iy = ycount + 1; iy >= 0; iy--) {
        Cell step(iy / 2, (iy + 1) / 2);
        Cell cell = base + step;

        for (ix = xcount; ix > 0; ix--) {
            if (Map.In_Radar(cell)) {
                CellClass *cellptr = &Map[cell];
                if (cellptr->OverlayIndex != -1) {
                    Rect render = cellptr->Overlay_Render_Rect();
                    Rect visible = cliprect.Intersect(render);
                    if (visible.Is_Valid()) {
                        Point2D pixel;
                        Coord_To_Pixel(Coord_Snap(Cell_Center(cell)), pixel);
                        pixel.X += -30;
                        cellptr->Draw_Overlay(pixel, cliprect);
                    }
                }
            }
            cell += Cell(1, -1);
        }
    }

    for (iy = ycount + 1; iy >= 0; iy--) {
        Cell step(iy / 2, (iy + 1) / 2);
        Cell cell = base + step;

        for (ix = xcount; ix > 0; ix--) {
            if (Map.In_Radar(cell)) {
                CellClass *cellptr = &Map[cell];
                if (cellptr->OverlayIndex != -1) {
                    Rect shadow = cellptr->Overlay_Shadow_Render_Rect();
                    Rect visible = cliprect.Intersect(shadow);
                    if (visible.Is_Valid()) {
                        Point2D pixel;
                        Coord_To_Pixel(Coord_Snap(Cell_Center(cell)), pixel);
                        pixel.X += -30;
                        cellptr->Draw_Overlay_Shadow(pixel, cliprect);
                    }
                }
            }
            cell.X++;
            cell.Y--;
        }
    }
}


// Draw cell shroud/fog and then the alpha-shape layer (0x006A7820).
// Real unaccepted alpha-layer entry: area in ECX, no stack arguments, plain RET.
void __fastcall Unknown_0041F5E0(Rect const &area);

inline CoordStruct::CoordStruct(Point2D const &point,int z) : BASECLASS(point.X,point.Y,z) {}

void Tactical::Draw_Shroud(Rect const &area)
{
    Point2D const viewport(TacPixelX,TacPixelY);
    CoordStruct lepton(Unknown_006A2900(viewport+area.Top_Left()-unknown_rect_839630.Top_Left()),0);
    if(lepton.Y<0) lepton.Y=0;
    if(lepton.X<0) lepton.X=0;
    Cell origin=Map[lepton].MapCoords;
    int rows=area.Height/15+17;
    int columns=area.Width/60+4;
    Cell base=origin;
    base.X-=2;
    Cell scan=base;
    if(unknown_b263d8) {
        int x,y;
        for(y=0;y<rows;++y) {
            Cell step(y/2,(y+1)/2);
            scan=base+step;
            for(x=columns;x>0;--x) {
                if(Map.In_Radar(scan)) {
                    CellClass *cell=&Map[scan];
                    CoordStruct coord=Coord_Snap(Cell_Center(scan));
                    Point2D pixel;
                    Coord_To_Pixel(coord,pixel);
                    pixel.X-=30;
                    Rect cell_rect(pixel.X,unknown_rect_839630.Y+pixel.Y,60,30);
                    Rect intersection=cell_rect.Intersect(area);
                    Rect draw_rect=unknown_rect_839630.Intersect(intersection);
                    if(draw_rect.Is_Valid()) {
                        Point2D origin(unknown_rect_839630.X,unknown_rect_839630.Y);
                        cell->Draw_Shroud_And_Fog(pixel+origin,draw_rect);
                    }
                }
                scan+=Cell(1,-1);
            }
        }
    }
    Unknown_0041F5E0(area);
}


// Not exact (RA2_NON_MATCHING): Tactical::Render at 0x006A45A0, best score 54.57, 2026-11-13; differs: 904 instructions in the original, 903 in the draft (shape 94.30)
#ifdef RA2_NON_MATCHING
// NON-EXACT DRAFT 0x6A45A0 FUN_006a45a0
// entry 0x6A45A0; exact-match score 64.69 (misses_index best), this text 56.45; plan "review: rewrite likely"
// origin lane: Scout958 (build/class-work/Scout958/w/keep/render_b3_5645.cpp)
// Recovered source text by our lanes/workers; never exact-match credit. Tracked by
// Scout1069 (native/drafts.json). Not original bytes.
// Native adaptation (Scout1069): RA2_OBJECT_IS_FOOT inline spelled out; each changed line is marked NATIVE.
// void Tactical::Render(Surface *surface, bool fullredraw, int drawpass) (0x006A45A0): draws one pass of the
// tactical view (TibSun Tactical::Render).
#include "shared/super.hpp"
#include "shared/veinhole.hpp"
#include "shared/spot_light.hpp"
#include "shared/ebolt.hpp"
#include "shared/line_trail.hpp"
#include "shared/capture_manager.hpp"

// Every techno (0x00A40C20; declared in shared/planning.hpp, which this unit cannot include).
extern DynamicVectorClass<TechnoClass *> unknown_list_a40c20;
extern bool unknown_scrolled_off_screen_abfd6c;     // 0x00ABFD6C
void Unknown_00521FB0();                            // 0x00521FB0 (TibSun IonBlastClass::Draw_All; hypothesis)
void Unknown_005334D0();                            // 0x005334D0 (TibSun LaserDrawClass::Draw_All; hypothesis)
void Unknown_006354E0();                            // 0x006354E0 (RadBeam drawing; hypothesis)
bool Planning_Active();                             // 0x00614840
void Planning_Redraw();                             // 0x00617E40
void Unknown_00617EF0();                            // 0x00617EF0 (planning nodes; hypothesis)
void Unknown_00618090();                            // 0x00618090 (planning nodes; hypothesis)
bool __fastcall Unknown_004386F0(TechnoClass *techno);  // 0x004386F0 (hypothesis)
void __fastcall Draw_Timer(int line, ColorScheme *scheme, int value, wchar_t const *text,
    unsigned __int64 *flash_time, bool *flash);                         // 0x006A5280
void __fastcall Unknown_004CAB90(TechnoClass *techno);  // 0x004CAB90 (hypothesis)

void Tactical::Render(Surface *surface, bool fullredraw, int drawpass)
{
    unknown_db0 = 0;
    bool has_selectable_buildings = Contains_Selectable_Buildings(unknown_rect_839630);

    int xoffset = LastTacPixelX - TacPixelX;
    int yoffset = LastTacPixelY - TacPixelY;

    if (drawpass == 0) {
        if (abs(xoffset) < unknown_rect_839630.Width && abs(yoffset) < unknown_rect_839630.Height) {
            unknown_scrolled_off_screen_abfd6c = false;
        } else {
            fullredraw = true;
            unknown_scrolled_off_screen_abfd6c = true;
        }
    } else {
        if (unknown_scrolled_off_screen_abfd6c) {
            fullredraw = true;
        }
    }

    if (fullredraw) {
        xoffset = 0;
        yoffset = 0;
    }

    if (drawpass != 2) {
    bool render;
    if (fullredraw) {
        render = true;
    } else if (!unknown_d7c && !has_selectable_buildings && TacticalLists::DirtyAreas.Count() == 0 && !unknown_d7d && unknown_d64.X == unknown_d6c.X && unknown_d64.Y == unknown_d6c.Y) {
        render = false;
    } else if (has_selectable_buildings) {
        render = true;
    } else {
        render = unknown_d7d || TacticalLists::DirtyAreas.Count() != 0 || TacPixelX != LastTacPixelX || TacPixelY != LastTacPixelY;
    }

    if (render) {
        unsigned panwidth = abs(xoffset);
        unsigned panheight = abs(yoffset);
        int width = unknown_rect_abe560.Width - panwidth;
        int height = unknown_rect_abe560.Height - panheight;
        int ystart = (yoffset < 0) ? -yoffset : 0;
        int xstart = (xoffset < 0) ? -xoffset : 0;

        Rect sourcerect(unknown_rect_abe560.X + xstart, unknown_rect_abe560.Y + ystart,
            width, height);
        Rect destrect(unknown_rect_abe560.X + ((xoffset <= 0) ? 0 : xoffset),
            unknown_rect_abe560.Y + ((yoffset <= 0) ? 0 : yoffset),
            width, height);

        if (xoffset != 0 || yoffset != 0) {
            if (drawpass == 0 || drawpass == 3) {
                unknown_surface_839964->Copy_From(destrect, *unknown_surface_839944, sourcerect, false, true);
            }
            if (drawpass == 0 || drawpass == 3) {
                DepthBuffer->Pan(-xoffset, -yoffset, 0xFFFF);
            }
            if (drawpass == 0 || drawpass == 3) {
                AlphaBuffer->Pan(-xoffset, -yoffset, 0x7F);
            }
            if (drawpass == 0 || drawpass == 3) {
                Surface *oldcomp = unknown_surface_839964;
                unknown_surface_839964 = unknown_surface_839944;
                unknown_surface_839944 = oldcomp;
                surface = unknown_surface_839964;
                unknown_surface_83995c = unknown_surface_839964;
            }
        } else {
            if (fullredraw && (drawpass == 0 || drawpass == 3)) {
                DepthBuffer->Fill(0xFFFF);
                AlphaBuffer->Fill(0x7F);
            }
        }

        if (drawpass == 0) {
            return;
        }

        if (!Debug_Map) {
            Register_Dirty_Buildings(Rect(unknown_rect_839630.X - 64, unknown_rect_839630.Y - 64,
                unknown_rect_839630.Width + 128, unknown_rect_839630.Height + 128));
        }

        Surface *prevlogical = unknown_surface_83995c;
        unknown_surface_83995c = unknown_surface_839944;
        unknown_surface_83995c->Lock();

        if (drawpass == 1 || drawpass == 3) {
            bool scrolled_left = LastTacPixelX > TacPixelX;
            bool scrolled_right = LastTacPixelX < TacPixelX;
            bool scrolled_up = LastTacPixelY > TacPixelY;
            bool scrolled_down = LastTacPixelY < TacPixelY;

            Rect leftstrip(unknown_rect_839630.X, unknown_rect_839630.Y, panwidth, unknown_rect_839630.Height);
            Rect rightstrip(unknown_rect_839630.X + unknown_rect_839630.Width - panwidth, unknown_rect_839630.Y, panwidth, unknown_rect_839630.Height);
            Rect leftclip = Intersect(leftstrip, unknown_rect_839630);
            Rect rightclip = Intersect(rightstrip, unknown_rect_839630);

            Rect topstrip;
            Rect bottomstrip;
            if (scrolled_left) {
                topstrip = Rect(unknown_rect_839630.X + panwidth, unknown_rect_839630.Y, unknown_rect_839630.Width - panwidth, panheight);
                bottomstrip = Rect(unknown_rect_839630.X + panwidth, unknown_rect_839630.Y + unknown_rect_839630.Height - panheight, unknown_rect_839630.Width - panwidth, panheight);
            } else if (scrolled_right) {
                topstrip = Rect(unknown_rect_839630.X, unknown_rect_839630.Y, unknown_rect_839630.Width - panwidth, panheight);
                bottomstrip = Rect(unknown_rect_839630.X, unknown_rect_839630.Y + unknown_rect_839630.Height - panheight, unknown_rect_839630.Width - panwidth, panheight);
            } else {
                topstrip = Rect(unknown_rect_839630.X, unknown_rect_839630.Y, unknown_rect_839630.Width, panheight);
                bottomstrip = Rect(unknown_rect_839630.X, unknown_rect_839630.Y + unknown_rect_839630.Height - panheight, unknown_rect_839630.Width, panheight);
            }
            Rect topclip = Intersect(topstrip, unknown_rect_839630);
            Rect bottomclip = Intersect(bottomstrip, unknown_rect_839630);

            Rect bounds(unknown_rect_839630.X + (scrolled_left ? panwidth : 0),
                unknown_rect_839630.Y + (scrolled_up ? panheight : 0),
                unknown_rect_839630.Width - panwidth, unknown_rect_839630.Height - panheight);

            Rect xpanrect(0, 0, 0, 0);
            Rect ypanrect(0, 0, 0, 0);
            if (scrolled_left) {
                xpanrect = leftclip;
            } else if (scrolled_right) {
                xpanrect = rightclip;
            }
            if (scrolled_up) {
                ypanrect = topclip;
            } else if (scrolled_down) {
                ypanrect = bottomclip;
            }

            Wipe_Depth(fullredraw, xoffset, yoffset, bounds);
            Render_Shroud(xpanrect, ypanrect, bounds, fullredraw);
            Render_Tiles(xpanrect, ypanrect, fullredraw);
            Render_Fogged_Objects(xpanrect, ypanrect, fullredraw);
            Render_Overlays(xpanrect, ypanrect, fullredraw);
            Render_Terrain(xpanrect, ypanrect, fullredraw);
            Render_Tile_Shadows(xpanrect, ypanrect, fullredraw);
            Render_Buildings(xpanrect, ypanrect, fullredraw);

            for (int index = TacticalLists::DirtyAreas.Count() - 1; index >= 0; index--) {
                TacticalLists::DirtyAreas.Delete(index);
            }

            unknown_surface_83995c->Unlock();
            unknown_surface_83995c = prevlogical;
            unknown_e0 = 0;
        }
    }
    }

    unknown_d6c.X = unknown_d64.X;
    unknown_d6c.Y = unknown_d64.Y;
    LastTacPixelX = TacPixelX;
    LastTacPixelY = TacPixelY;

    if (drawpass == 1 || drawpass == 3) {
        unknown_surface_83995c->Copy_From(unknown_rect_839630, *unknown_surface_839944, unknown_rect_839630, false, true);
    }

    if (drawpass == 1) {
        return;
    }

    if (drawpass == 2 || drawpass == 3) {
        int i;

        Add_Buildings_To_Selectable(unknown_rect_839630);
        surface->Lock();
        Surface *savedlogical = unknown_surface_83995c;
        unknown_surface_83995c = surface;
        Draw_Waypoints(false);
        Draw_Rally_Points(false);
        Draw_Placement(false);
        VeinholeMonsterClass::Draw_All();
        Unknown_00521FB0();
        Draw_Objects(true);
        SpotLightClass::Draw_All();
        Unknown_005334D0();
        EBolt::Update_All_004B2210();
        LineTrail::Update_All();
        Unknown_006354E0();
        Draw_Radial_Indicators();
        unknown_object_84e8d0.Unknown_0042E290(unknown_surface_83995c, unknown_rect_839630);
        Unknown_006AA790();
        Draw_Waypoints(true);
        Draw_Rally_Points(true);
        Draw_Placement(true);

        bool planning = Planning_Active();
        if (planning) {
            Planning_Redraw();
            Unknown_00617EF0();
        }

        int count = unknown_list_a40c20.Count();
        for (i = 0; i < count; i++) {
            TechnoClass *techno = unknown_list_a40c20[i];
            if (techno->House->Is_Player_Control()) {
                if (!planning && techno->unknown_77) {
                    if (unknown_bool_7f7110) {
                        techno->unknown_slot243(false, 0);
                    }
                    UnknownTechno230 *capture = (UnknownTechno230 *)techno->unknown_230;
                    if (capture != 0 && capture->unknown_24 != 0) {
                        ((CaptureManagerClass *)capture)->Draw_Link();
                    }
                }
            } else if (Unknown_004386F0(techno) && techno != 0 && techno->AbstractFlags.IsFoot && !techno->unknown_76 /* NATIVE: RA2_OBJECT_IS_FOOT inline spelled out (techno is non-null here) */) {
                Unknown_004CAB90(techno);
            }
        }

        if (planning) {
            Unknown_00618090();
        }

        Draw_Pixel_Effects(unknown_rect_839630, unknown_rect_839630);
        unknown_surface_83995c = savedlogical;
        surface->Unlock();

        int line = 0;
        if (unknown_id_source_a3d290->MissionTimer.Started != -1) {
            Draw_Timer(0, ColorSchemes[unknown_player_a35db4->ColorSchemeIndex], unknown_id_source_a3d290->MissionTimer / 15,
                unknown_id_source_a3d290->MissionTimerText, 0, 0);
            line = 1;
        }

        count = unknown_supers_a35db8.Count();
        for (i = 0; i < count; i++) {
            if (!unknown_supers_a35db8[i]->unknown_0x58 || unknown_game_mode_a3d298 != 0
                    || unknown_supers_a35db8[i]->RechargeTimer != unknown_supers_a35db8[i]->Type->RechargeTime) {
                SuperClass *super = unknown_supers_a35db8[i];
                Draw_Timer(line, ColorSchemes[super->House->ColorSchemeIndex], super->RechargeTimer / 15, super->Type->UIName,
                    (unsigned __int64 *)&super->unknown_0x3C.unknown_4, &super->unknown_0x3C.unknown_0);
                line++;
            }
        }

        count = Houses.Count();
        for (i = 0; i < count; i++) {
            int time = Houses[i]->unknown_1ec;
            if (time > 0) {
                Draw_Timer(line, ColorSchemes[Houses[i]->ColorSchemeIndex], time / 15,
                    StringTable::Load_String("MSG:BlackoutTimer", 0, "D:\\Ra2\\Tactical.CPP", 444), 0, 0);
                line++;
            }
        }

        Draw_Screen_Text((wchar_t const *)&unknown_24);
        unknown_d7c = false;
        unknown_d7d = false;
    }
}
#endif

// SPDX-License-Identifier: GPL-3.0-or-later
// Copyright 2026 ra2-decomp contributors
// Copyright 2025 Electronic Arts Inc.; 2026 OpenTS contributors
// Derived from Electronic Arts source code; EA's GPL Section 7 terms apply (COPYING.md).
// void Tactical::Draw_Tiles(Rect const &area, Rect const &cliprect) (0x006A7BA0)
// Adopted from ra2-decomp Tactical::Draw_Tiles (YR 0x006D7560).
// Draws the tile of every cell of the diagonal band under area whose render
// rectangle is valid and meets area.

static inline Point2D Pixel_To_Ground_Point(Matrix3D const &matrix, Point2D const &pixel)
{
    Vector3 ground;
    ground.X = pixel.X;
    ground.Y = pixel.Y;
    ground.Z = 0;
    ground = matrix * ground;
    return Point2D(ground.X, ground.Y);
}

static inline void Project_Tile_Left(Tactical &view, CoordStruct const &coord, Point2D &pixel)
{
    view.Coord_To_Pixel(coord, pixel);
    pixel.X -= 60 / 2;
}

void Tactical::Draw_Tiles(Rect const &area, Rect const &cliprect)
{
    Point2D point = Pixel_To_Ground_Point(unknown_de4, Point2D(area.Top_Left()) + Point2D(TacPixelX, TacPixelY) - unknown_rect_839630.Top_Left());
    CoordStruct lepton(point.X, point.Y, 0);
    Cell origin(lepton.X / 256, lepton.Y / 256);
    Cell base = origin;

    int ycount = area.Height / (30 / 2) + 17;
    int xcount = area.Width / 60 + 4;

    base.X -= 2;
    int ix, iy;
    for (iy = 0; iy < ycount; iy++) {
        Cell step(iy / 2, (iy + 1) / 2);
        Cell cell = base + step;

        for (ix = xcount; ix > 0; ix--) {
            if (Map.In_Radar(cell)) {
                CellClass *cellptr = &Map[cell];
                Rect render = cellptr->Cell_Render_Rect();
                Rect inter = Intersect(area, render);
                Point2D pixel;
                Project_Tile_Left(*this, Coord_Snap(Cell_Center(cell)), pixel);
                if (render.Is_Valid() && inter.Is_Valid()) {
                    cellptr->Draw_It(pixel, cliprect, 0);
                }
            }
            cell += Cell(1, -1);
        }
    }
}


// Tactical tile-shadow scan (006A8240); two original inverse projections are evidenced.

void Tactical::Draw_Tile_Shadows(Rect const &area,Rect const &cliprect)
{
    Point2D const viewport(TacPixelX,TacPixelY);
    CoordStruct lepton(Unknown_006A2900(viewport+area.Top_Left()-unknown_rect_839630.Top_Left()),0);
    Cell origin=lepton.As_Cell();
    Cell base=origin;
    base.X-=2;
    Cell scan=base;
    lepton=CoordStruct(Unknown_006A2900(Point2D(area.Top_Left())+Point2D(TacPixelX,TacPixelY)-unknown_rect_839630.Top_Left()),0);
    origin=Map[lepton].MapCoords;
    base=origin;
    int rows=area.Height/15+20;
    int columns=area.Width/60+7;
    base.X-=2;
    base.Y+=3;
    int y,x;
    for(y=0;y<rows;++y) {
        Cell step(y/2,(y+1)/2);
        scan=base+step;
        for(x=columns;x>0;--x) {
            if(Map.In_Radar(scan)) {
                int x=scan.X*256+128;
                int y=scan.Y*256+128;
                CoordStruct coord=UnknownCoord434820(x/256*256,y/256*256,0);
                Point2D pixel;
                Coord_To_Pixel(coord,pixel);
                pixel.X-=30;
                Map[scan].Draw_Shadow_Cast(pixel,cliprect);
            }
            scan+=Cell(1,-1);
        }
    }
}


// SPDX-License-Identifier: GPL-3.0-or-later
// Copyright 2026 ra2-decomp contributors
// Copyright 2025 Electronic Arts Inc.
// Derived from Electronic Arts source code; EA's GPL Section 7 terms apply (COPYING.md).
// void Tactical::Draw_Tile_Shadows(int, Rect const &area) -- 0x006A88A0
// From ra2-decomp Tactical::Draw_Tile_Shadows(int, Rect const &) (YR 0x006D8260).
// Purpose: draws the shadow casts of the cells inside area, clipped to it;
// the scan starts at the area's map corner moved back onto the map.

// TibSun inline.h Coord_Whole: the coordinate without its sub-cell part.
static inline CoordStruct Coord_Whole(CoordStruct const &coord)
{
    int x = coord.X / 256;
    int y = coord.Y / 256;

    return CoordStruct(x * 256, y * 256, coord.Z);
}

static inline void Set_Ground_Position(CoordStruct &coord, Point2D const &point)
{
    coord.X = point.X;
    coord.Y = point.Y;
    coord.Z = 0;
}

void Tactical::Draw_Tile_Shadows(int, Rect const &area)
{
    Point2D sample = Point2D(area.Top_Left()) + Point2D(TacPixelX, TacPixelY) - unknown_rect_839630.Top_Left();
    CoordStruct lepton;
    Set_Ground_Position(lepton, Unknown_006A2900(sample));
    if (lepton.Y < 0) {
        lepton.X -= lepton.Y;
        lepton.Y = 0;
    }
    if (lepton.X < 0) {
        lepton.Y -= lepton.X;
        lepton.X = 0;
    }

    Cell base = Map[lepton].Fetch_CellID();
    int ycount = area.Height / 15 + 20;
    int xcount = area.Width / 60 + 7;
    base.X -= 2;
    base.Y += 3;

    int ix, iy;
    for (iy = 0; iy < ycount; iy++) {
        Cell step(iy / 2, (iy + 1) / 2);
        Cell cell = base + step;

        for (ix = xcount; ix > 0; ix--) {
            if (Map.In_Radar(cell)) {
                Point2D pixel;
                Coord_To_Pixel(Coord_Whole(CoordStruct(cell.X * 256 + 128, cell.Y * 256 + 128, 0)), pixel);
                pixel.X -= 30;
                Map[cell].Draw_Shadow_Cast(pixel, area);
            }
            ++cell.X;
            --cell.Y;
        }
    }
}


// Point2D Tactical::Clamp_Pixel_To_Tactical(Point2D const &pixel) (0x006A8B10): the
// pixel clamped to the view (0x006A8C80) unless the map editor flag 0x00A40D13
// is set (TibSun Tactical::Clamp_Pixel_To_Tactical).

Point2D Tactical::Clamp_Pixel_To_Tactical(Point2D const &pixel)
{
    Point2D clamped = pixel;
    if (Unknown_006A8C80(&clamped) && !Debug_Map) {
        return clamped;
    } else {
        return pixel;
    }
}


// Not exact (RA2_NON_MATCHING): Tactical::Draw_Pixel_Effects at 0x006A7E80, best score 86.26, 2026-11-13; differs: 287 instructions in the original, 288 in the draft (shape 98.78)
#ifdef RA2_NON_MATCHING
// Tactical pixel effects (0x006A7E80): the glints over ore and gem cells.
#include "shared/frame_rate.hpp"
#include "shared/pixel_fx.hpp"
#include "shared/hicolor_pixel.hpp"
#include "shared/win32_api.hpp"
unsigned int Unknown_0053D840();
extern unsigned long unknown_last_time_abfd70;

void Tactical::Draw_Pixel_Effects(Rect const &area, Rect const &cliprect)
{
    if (unknown_a6eb1c < Unknown_0053D840()) {
        return;
    }
    if (unknown_surface_83995c->Bytes_Per_Pixel() != 2) {
        return;
    }
    if (unknown_a40b2c == 0) {
        return;
    }
    char *pixels = (char *)unknown_surface_83995c->Lock();
    if (pixels == 0) {
        return;
    }
    DWORD now = timeGetTime();
    unsigned int frames = now - unknown_last_time_abfd70;
    unknown_last_time_abfd70 = now;
    if (frames > 1000) {
        frames = 1000;
    }
    int pitch = unknown_surface_83995c->Stride();
    Point2D lepton = Unknown_006A2900(Point2D(area.Top_Left()) + Point2D(TacPixelX, TacPixelY) - unknown_rect_839630.Top_Left());
    Cell origin(lepton.X / 256, lepton.Y / 256);
    Cell base = origin;
    int rows = area.Height / 15 + 17;
    int columns = area.Width / 60 + 4;
    base.X -= 2;
    Cell scan;
    for (int y = 0; y < rows; ++y) {
        Cell step(y / 2, (y + 1) / 2);
        scan = base + step;
        for (int x = columns; x > 0; --x) {
            if (Map.In_Radar(scan)) {
                CellClass &cell = Map[scan];
                int value = cell.Get_Tiberium_Value();
                if (cell.Flags_12c & 0x10 && !cell.unknown_47cb50()
                    && (cell.unknown_ec == 2 || value != 0)
                    && cell.OccupierPtr == 0
                    && !(cell.unknown_140 & 0x1000)) {
                    PixelFXClass *effect = (PixelFXClass *)cell.unknown_fc;
                    int type = value != 0;
                    if (effect == 0) {
                        effect = new PixelFXClass(type);
                        cell.unknown_fc = (UnknownCellOwned *)effect;
                    } else if (effect->Phase >= 0x2000) {
                        effect->Init(type);
                    }
                    if (effect != 0 && effect->Reduce_Life(frames)) {
                        Rect rect = cell.Cell_Render_Rect();
                        Point2D point(rect.Width / 2, rect.Height / 2);
                        point.X += rect.X;
                        point.Y += rect.Y;
                        point.X += effect->XOffset;
                        point.Y += effect->YOffset;
                        if (point.X >= cliprect.X && point.X < cliprect.Width + cliprect.X
                            && point.Y >= cliprect.Y && point.Y < cliprect.Height + cliprect.Y) {
                            effect->Update(frames);
                            *(unsigned short *)(pixels + pitch * point.Y + point.X * 2) = (unsigned short)RGB_To_Pixel(effect->Current.Red, effect->Current.Green, effect->Current.Blue);
                        }
                    }
                }
            }
            scan += Cell(1, -1);
        }
    }
    unknown_surface_83995c->Unlock();
}
#endif

// Not exact (RA2_NON_MATCHING): Tactical::Draw_Tiles(Cell const &, Rect const &) at 0x006A8560, best score 61.48, 2026-11-13; differs: 255 instructions in the original, 259 in the draft (shape 93.39)
#ifdef RA2_NON_MATCHING
// void Tactical::Draw_Tiles(Cell const &cell, Rect const &cliprect) (0x006A8560): draws a cell's iso tile and the taller
// tiles that overhang it (TibSun Tactical::Draw_Tiles).
#define CELL_DECLARE_DRAW_IT
#include "shared/isometric_tile_type.hpp"

static inline bool Has_Extra_Data(CellClass const *cellptr)
{
    int subtile = (unsigned char)cellptr->unknown_11a;
    int ittype = cellptr->IsoTileIndex;
    if (ittype == 0xFFFF || ittype == 0xFF) {
        return false;
    }
    IsometricTileTypeClass *type = IsoTileTypes[ittype];
    if (type == 0) {
        return false;
    }
    IsoTileImageStruct *set = (IsoTileImageStruct *)type->Get_Image_Data();
    if (set == 0) {
        return false;
    }
    int last = set->unknown_00[1] * set->unknown_00[0] - 1;     // the image's cell count (header +0x00 x +0x04)
    if (subtile >= last) {
        subtile = last;
    }
    IsoTileCellStruct const *record = (IsoTileCellStruct const *)set->Tiles[subtile];
    if (record == 0 || !record->IsHasExtraData) {
        return false;
    }
    return true;
}

static inline CoordStruct Tile_Center(Cell const &cell)
{
    CoordStruct center;
    center.X = cell.X * 256 + 256 / 2;
    center.Y = cell.Y * 256 + 256 / 2;
    center.Z = 0;
    return center;
}

void Tactical::Draw_Tiles(Cell const &cell, Rect const &cliprect)
{
    CellClass *basecell = &Map[cell];

    CoordStruct coord = Coord_Snap(Tile_Center(cell));
    Point2D screen;
    Coord_To_Pixel(coord, screen);
    Point2D pixel;
    if (basecell->unknown_11b < 2) {
        pixel.X = screen.X - 30;
        pixel.Y = screen.Y;
        basecell->Draw_It(pixel, unknown_rect_839630, 0);
    }

    int x = cell.X;
    int y = cell.Y;
    for (int level = 0; level < 24; level += 2) {
        for (int col = 0; col < 3; col++) {
            int cx = x + ((col + 1) >> 1);
            int cy = y - (col >> 1) + 1;
            if (Map.In_Radar(Cell(cx, cy))) {
                CellClass *cellptr = &Map[Cell(cx, cy)];
                bool overlap = false;
                bool draw = false;
                int height = cellptr->unknown_11b;

                if (col != 1) {
                    if (height == level + 1) {
                        draw = true;
                        overlap = true;
                    }
                } else if (height > level && height < level + 4) {
                    draw = true;
                    overlap = true;
                }

                if (cellptr->unknown_11d > 0 && !draw) {
                    int diff = height - level;
                    if (col != 1) {
                        if (diff < 0 && cellptr->unknown_11d + diff >= 0) {
                            draw = true;
                        } else {
                            draw = overlap;
                        }
                    } else if (diff + 1 > 0) {
                        draw = overlap;
                    } else {
                        draw = true;
                        if (cellptr->unknown_11d + diff + 1 <= 0) {
                            draw = overlap;
                        }
                    }
                }

                if (height == level && !draw) {
                    if (!Has_Extra_Data(cellptr)) {
                        continue;
                    }
                } else if (!draw) {
                    continue;
                }

                Cell above(cx, cy);
                coord = Coord_Snap(Tile_Center(above));
                Point2D tilepixel;
                Project_Tile_Left(*this, coord, tilepixel);
                Map[above].Draw_It(tilepixel, unknown_rect_839630, 0);
            }
        }
        x++;
        y++;
    }
}
#endif

// bool Tactical::Unknown_006A8C80(Point2D *point) (0x006A8C80)
// Clamps a view position to the scrollable area derived from the map size and the tactical window; true when
// the point was changed.

extern int unknown_int_8325d0;
extern int unknown_int_8325d8;
extern int unknown_int_8325dc;
extern int unknown_int_8325e0;
extern int unknown_int_8325e4;
extern int unknown_int_83963c;

bool Tactical::Unknown_006A8C80(Point2D *point)
{
    int min_x = unknown_int_839638 / 2 + (unknown_int_8325d8 * 2 - unknown_int_8325d0) * 30;
    int max_x = (unknown_int_8325e0 * 60 - unknown_int_839638) + min_x;
    int min_y = (unknown_int_8325d0 - 5 + unknown_int_8325dc * 2) * 15 + unknown_int_83963c / 2;
    int max_y = ((unknown_int_8325e4 * 60 + 270) / 2 - unknown_int_83963c) + min_y;
    bool changed = false;
    if (point->Y < min_y) {
        changed = true;
        point->Y = min_y;
    } else if (point->Y > max_y) {
        point->Y = max_y;
        changed = true;
    }
    if (point->X < min_x) {
        point->X = min_x;
        return true;
    }
    if (point->X > max_x) {
        point->X = max_x;
        return true;
    }
    return changed;
}


// int Tactical::Cell_Shadow(Cell const &cell, bool fog) (0x006A8D40): the shroud (or, with fog, the fog)
// edge frame of a cell from which of its eight neighbours are unrevealed: -2 for solid black, -1 for none
// (TibSun Tactical::Cell_Shadow).

// The edge frame of each neighbour mask (0x007AC3BC; TibSun Cell_Shadow's _shadow table).
extern char const unknown_shadow_7ac3bc[256];

int Tactical::Cell_Shadow(Cell const &cell, bool fog)
{
    int index = 0;
    int value = -1;

    CellClass const *cellptr = &Map[cell];

    if (fog) {
        if (!(cellptr->unknown_140 & 3)) value = -2;

        if (cellptr->unknown_140 & 2) {
            Cell c;
            c = Cell(cell.X - 1, cell.Y - 1);
            if (!(Map[c].unknown_140 & 2)) index |= 0x40;
            c = Cell(cell.X, cell.Y - 1);
            if (!(Map[c].unknown_140 & 2)) index |= 0x80;
            c = Cell(cell.X + 1, cell.Y - 1);
            if (!(Map[c].unknown_140 & 2)) index |= 0x01;
            c = Cell(cell.X - 1, cell.Y);
            if (!(Map[c].unknown_140 & 2)) index |= 0x20;
            c = Cell(cell.X + 1, cell.Y);
            if (!(Map[c].unknown_140 & 2)) index |= 0x02;
            c = Cell(cell.X - 1, cell.Y + 1);
            if (!(Map[c].unknown_140 & 2)) index |= 0x10;
            c = Cell(cell.X, cell.Y + 1);
            if (!(Map[c].unknown_140 & 2)) index |= 0x08;
            c = Cell(cell.X + 1, cell.Y + 1);
            if (!(Map[c].unknown_140 & 2)) index |= 0x04;

            value = unknown_shadow_7ac3bc[index];
        }
    } else {
        if (!(cellptr->unknown_12c & 0x18)) value = -2;

        if (cellptr->unknown_12c & 8) {
            Cell c;
            c = Cell(cell.X - 1, cell.Y - 1);
            if (!(Map[c].unknown_12c & 8)) index |= 0x40;
            c = Cell(cell.X, cell.Y - 1);
            if (!(Map[c].unknown_12c & 8)) index |= 0x80;
            c = Cell(cell.X + 1, cell.Y - 1);
            if (!(Map[c].unknown_12c & 8)) index |= 0x01;
            c = Cell(cell.X - 1, cell.Y);
            if (!(Map[c].unknown_12c & 8)) index |= 0x20;
            c = Cell(cell.X + 1, cell.Y);
            if (!(Map[c].unknown_12c & 8)) index |= 0x02;
            c = Cell(cell.X - 1, cell.Y + 1);
            if (!(Map[c].unknown_12c & 8)) index |= 0x10;
            c = Cell(cell.X, cell.Y + 1);
            if (!(Map[c].unknown_12c & 8)) index |= 0x08;
            c = Cell(cell.X + 1, cell.Y + 1);
            if (!(Map[c].unknown_12c & 8)) index |= 0x04;

            value = unknown_shadow_7ac3bc[index];
        }
    }
    return value;
}


// bool Tactical::Is_Cell_Visible(Cell const &cell) (0x006A9120): the cell lies in
// the visible cell rectangle (+0xD80) (TibSun Tactical::Is_Cell_Visible).

bool Tactical::Is_Cell_Visible(Cell const &cell)
{
    Point2D point = Point2D(cell.X, cell.Y);
    if (unknown_d80.Is_Point_Within(point)) {
        return true;
    }
    return false;
}


// void Tactical::Unknown_006A9170() (0x006A9170): after the view position
// (+0xD64) changed: the view origin +0xB0 is centred on the screen rectangle
// 0x00ABE560, projected through +0xDE4, and the visible cell rectangle +0xD80
// is derived from it. The 60 x 30 offset is converted through a Point2D whose
// members feed the two fild temporaries.


void Tactical::Unknown_006A9170()
{
    Vector3 position;
    position.Z = 0.0f;
    int x = unknown_d64.X;
    int y = unknown_d64.Y;
    TacPixelX = x;
    TacPixelY = y;
    TacPixelX = x - unknown_rect_abe560.Width / 2;
    TacPixelY = y - unknown_rect_abe560.Height / 2;
    Point2D offset(TacPixelX - 60, TacPixelY - 30);
    position.X = (float)offset.X;
    position.Y = (float)offset.Y;
    position = unknown_de4 * position;
    Point2D cell((int)position.X, (int)position.Y);
    unknown_d80.X = cell.X / 256;
    unknown_d80.Y = cell.Y / 256;
    unknown_d80.Width = unknown_rect_abe560.Width / 60 + 2;
    unknown_d80.Height = unknown_rect_abe560.Height / 15 + 4;
}


// void Tactical::Draw_Render_Hooks(ObjectClass *object, Rect const &cliprect) (0x006A92A0): an object's two
// decoration passes around its pixel, when it lies near the view (TibSun Tactical::Draw_Render_Hooks).

void __fastcall Tactical::Draw_Render_Hooks(ObjectClass *object, Rect const &cliprect)
{
    Point2D pixel;
    if (unknown_tactical_83996c->Coord_To_Pixel(object->Center_Coord(), pixel)) {
        object->unknown_slot65((int)&pixel, (int)&cliprect);
        object->unknown_slot66(pixel, cliprect);
    }
}


// SPDX-License-Identifier: GPL-3.0-or-later
// Copyright 2026 ra2-decomp contributors
// Copyright 2025 Electronic Arts Inc.; 2026 OpenTS contributors
// Derived from Electronic Arts source code; EA's GPL Section 7 terms apply (COPYING.md).
// void Tactical::Draw_Objects(bool forced) (0x006A93F0): from ra2-decomp Tactical::Draw_Objects
// (YR 0x006D8DB0), adapted to our names. Clears every layer object's visible flag (+0x85), draws
// those whose position lies in the view diamond (fogged foot objects, fog-removing animations and
// animating terrain under fog skipped), the ground layer's selected buildings' overlays, then the
// post-render pass of every visible techno.
#include "shared/id_source.hpp"
#include "shared/special.hpp"
#include "shared/anim.hpp"
#include "shared/anim_type.hpp"
#include "shared/terrain.hpp"
#include "shared/terrain_type.hpp"
#include "shared/building_class.hpp"
#include "shared/techno.hpp"

static inline bool Is_Foot_Object(ObjectClass const *object)
{
    if (object == 0) {
        return false;
    }
    unsigned char flag = object->AbstractFlags.IsFoot;
    return flag != 0;
}

static inline bool Is_Techno_Object(ObjectClass const *object)
{
    if (object == 0) {
        return false;
    }
    unsigned char flag = object->AbstractFlags.IsTechno;
    return flag != 0;
}

static inline bool Is_Animating(TerrainClass const *terrain)
{
    return terrain->Class->IsAnimated || terrain->unknown_b5;
}

void Tactical::Draw_Objects(bool forced)
{
    int width = unknown_rect_839630.Width;
    int pixelx = TacPixelX;
    int pixely = TacPixelY;
    int height = unknown_rect_839630.Height;
    Point2D top = Unknown_006A2900(Point2D(pixelx + width / 2, pixely - height));
    Point2D right = Unknown_006A2900(Point2D(pixelx + width * 2, pixely + height / 2));
    Point2D left = Unknown_006A2900(Point2D(pixelx - width, pixely + height / 2));
    Rect view(top.X, top.Y, (right.X - top.X) * 2, (left.Y - top.Y) * 2);
    CoordStruct position;

    for (int layer = 0; layer < 5; layer++) {
        for (int index = 0; index < DisplayClass::Layer[layer].Count(); index++) {
            ObjectClass *obj = DisplayClass::Layer[layer][index];
            obj->unknown_85 = false;

            if (Is_Foot_Object(obj)) {
                position = obj->Position_Coord_004f3f40();
                if (view.Is_Point_Within(Point2D(position.X, position.Y))) {
                    obj->unknown_slot19(0);
                    if (unknown_id_source_a3d290->Special().FogOfWar && Map.Is_Fogged(position)) {
                        continue;
                    }
                    Point2D pixel;
                    if (unknown_tactical_83996c->Coord_To_Pixel(obj->Center_Coord(), pixel)) {
                        obj->unknown_85 = true;
                        obj->unknown_slot65((int)&pixel, (int)&unknown_rect_839630);
                        obj->Render(&unknown_rect_839630, forced, false);
                    }
                }
            } else if (Is_Techno_Object(obj)) {
                position = obj->Position_Coord_004f3f40();
                if (view.Is_Point_Within(Point2D(position.X, position.Y))) {
                    obj->unknown_85 = true;
                    Draw_Render_Hooks(obj, unknown_rect_839630);
                    obj->Render(&unknown_rect_839630, forced, true);
                }
            } else {
                int rtti = obj->Kind_Of();
                if (rtti == 4) {
                    position = obj->Center_Coord();
                    if (view.Is_Point_Within(Point2D(position.X, position.Y))) {
                        AnimClass *anim = (AnimClass *)obj;
                        obj->unknown_85 = true;
                        if (!anim->unknown_100 && anim->Class->ShouldFogRemove && Map.Is_Fogged(obj->Center_Coord())) {
                            continue;
                        }
                        obj->Render(&unknown_rect_839630, forced, false);
                    }
                } else if (rtti == 0x24) {
                    if (Is_Animating((TerrainClass *)obj)) {
                        position = obj->Position_Coord_004f3f40();
                        if (view.Is_Point_Within(Point2D(position.X, position.Y))) {
                            if (Map.Is_Fogged(position)) {
                                continue;
                            }
                            obj->unknown_85 = true;
                            obj->Render(&unknown_rect_839630, forced, false);
                        }
                    }
                } else {
                    position = obj->Position_Coord_004f3f40();
                    if (view.Is_Point_Within(Point2D(position.X, position.Y))) {
                        Point2D pixel;
                        if (unknown_tactical_83996c->Coord_To_Pixel(obj->Center_Coord(), pixel)) {
                            obj->unknown_85 = true;
                            obj->unknown_slot65((int)&pixel, (int)&unknown_rect_839630);
                            obj->Render(&unknown_rect_839630, forced, false);
                        }
                    }
                }
            }
        }

        if (layer == 2) {
            for (int bindex = 0; bindex < unknown_BuildingClass_list_00a40af8.Count(); bindex++) {
                BuildingClass *bptr = unknown_BuildingClass_list_00a40af8[bindex];
                if (bptr->unknown_74 && bptr->unknown_85) {
                    Point2D pixel;
                    if (unknown_tactical_83996c->Coord_To_Pixel(bptr->unknown_slot42(), pixel)) {
                        Rect rect = unknown_rect_839630;
                        if (rect.X > unknown_rect_839630.X) pixel.X += unknown_rect_839630.X - rect.X;
                        if (rect.Y > unknown_rect_839630.Y) pixel.Y += unknown_rect_839630.Y - rect.Y;
                        bptr->Draw_Selection_Overlay_0043b660(pixel, rect);
                    }
                }
            }
        }
    }

    for (int layer2 = 0; layer2 < 5; layer2++) {
        int count = DisplayClass::Layer[layer2].Count();
        for (int index2 = 0; index2 < count; index2++) {
            ObjectClass *obj2 = DisplayClass::Layer[layer2][index2];
            if (obj2->unknown_85 && obj2 != 0 && Is_Techno_Object(obj2)) {
                if (Is_Foot_Object(obj2)) {
                    obj2->unknown_slot19(0);
                    if (unknown_b263d8 && !Debug_Map && unknown_id_source_a3d290->Special().FogOfWar && Map.Is_Fogged(obj2->Position_Coord_004f3f40())) {
                        continue;
                    }
                }
                Point2D pixel;
                if (unknown_tactical_83996c->Coord_To_Pixel(obj2->Center_Coord(), pixel)) {
                    obj2->unknown_slot66(pixel, unknown_rect_839630);
                }
            }
        }
    }
}


// void Tactical::Unknown_006A9E10(bool arg1, Rect const &rect1, Rect const &rect2) (0x006A9E10):
// redraws the terrain objects of display layer 2 (not animated, slot 17 false,
// +0xB5 clear) whose drawn area, moved onto the tactical area, overlaps the
// intersection of the two rectangles: slot 63 with a copy of rect2, then +0x75
// cleared. Last object first (hypothesis).
// display.hpp's map chain emits VectorClass<SubzoneConnectionStruct>'s vtable:

void Tactical::Unknown_006A9E10(bool arg1, Rect const &rect1, Rect const &rect2)
{
    Rect clip = Intersect(rect1, rect2);
    for (int index = DisplayClass::Layer[2].Count() - 1; index >= 0; index--) {
        ObjectClass *object = DisplayClass::Layer[2][index];
        if (object->Kind_Of() == 0x24 && !object->Is_Inactive()) {
            TerrainClass *terrain = (TerrainClass *)object;
            if (!terrain->Class->IsAnimated && !terrain->unknown_b5) {
                Point2D offset(unknown_rect_839630.X, unknown_rect_839630.Y);
                Rect area = Offset_Rect(object->Get_Render_Rect(), offset.X, offset.Y);
                if (Rects_Overlap(area, clip)) {
                    Rect bounds = rect2;
                    object->Render(&bounds, arg1, false);
                    object->unknown_75 = false;
                }
            }
        }
    }
}


// void Tactical::Draw_Fogged_Objects(Rect const &area) (0x006A9F50): the
// fogged objects inside area (TibSun Tactical::Draw_Fogged_Objects; the
// render pass 0x006A3D00 expands it).

void Tactical::Draw_Fogged_Objects(Rect const &area)
{
    ::Draw_Fogged_Objects(area);
}


// void Tactical::Unknown_006A9F60(bool arg1, Rect const &rect1, Rect const &rect2) (0x006A9F60):
// the buildings of display layer 2 (+0x5B7 clear, or every one while the
// reload flag 0x00A40D13 is set) redrawn like 0x006A9E10's terrain objects,
// first object first (hypothesis).

void Tactical::Unknown_006A9F60(bool arg1, Rect const &rect1, Rect const &rect2)
{
    Rect clip = Intersect(rect1, rect2);
    for (int index = 0; index < DisplayClass::Layer[2].Count(); index++) {
        ObjectClass *object = DisplayClass::Layer[2][index];
        if (object->Kind_Of() == 6) {
            BuildingClass *building = (BuildingClass *)object;
            if (!building->unknown_5b7 || Debug_Map) {
                Point2D offset(unknown_rect_839630.X, unknown_rect_839630.Y);
                Rect area = Offset_Rect(object->Get_Render_Rect(), offset.X, offset.Y);
                if (Rects_Overlap(area, clip)) {
                    Rect bounds = rect2;
                    object->Render(&bounds, arg1, false);
                    object->unknown_75 = false;
                }
            }
        }
    }
}


// void Tactical::Register_Dirty_Buildings(Rect area) (0x006AA090): every building
// of display layer 2 still waiting to be drawn (+0x75) whose drawn area,
// moved onto the screen, overlaps area goes to the dirty areas (0x006A3020)
// and is marked drawn (TibSun Tactical::Register_Dirty_Buildings).

void Tactical::Register_Dirty_Buildings(Rect area)
{
    for (int index = 0; index < DisplayClass::Layer[2].Count(); index++) {
        ObjectClass *object = DisplayClass::Layer[2][index];
        if (object->Kind_Of() == 6 && object->unknown_75) {
            Rect const &render = object->Get_Render_Rect();
            if (area.Is_Overlapping(render + Point2D(unknown_rect_839630.X, unknown_rect_839630.Y))) {
                Register_Dirty_Area(render, false);
                object->unknown_75 = false;
            }
        }
    }
}


// bool Tactical::Contains_Selectable_Buildings(Rect rect) (0x006AA180): whether any building on the map shows within the view
// rectangle (TibSun Tactical::Contains_Selectable_Buildings).

bool Tactical::Contains_Selectable_Buildings(Rect rect)
{
    Point2D top_left;
    Point2D bottom_right;
    top_left.X = rect.X + TacPixelX;
    top_left.Y = rect.Y + TacPixelY;
    bottom_right.X = rect.X + TacPixelX + rect.Width;
    bottom_right.Y = rect.Y + TacPixelY + rect.Height;

    for (int i = 0; i < unknown_BuildingClass_list_00a40af8.Count(); i++) {
        BuildingClass *bptr = unknown_BuildingClass_list_00a40af8[i];
        if (bptr->unknown_7d && bptr->unknown_75 && bptr->unknown_74) {
            Point2D point = Coord_To_Pixel_Absolute(bptr->unknown_slot42());
            if (point.X >= top_left.X && point.X <= bottom_right.X && point.Y >= top_left.Y && point.Y <= bottom_right.Y) {
                return true;
            }
        }
    }
    return false;
}


// void Tactical::Add_Buildings_To_Selectable(Rect rect) (0x006AA300): lists every building of the map that shows within the
// view rectangle in the object pixels table (TibSun Tactical::Add_Buildings_To_Selectable).

void Tactical::Add_Buildings_To_Selectable(Rect rect)
{
    Point2D top_left;
    Point2D bottom_right;
    top_left.X = rect.X + TacPixelX;
    top_left.Y = rect.Y + TacPixelY;
    bottom_right.X = rect.X + TacPixelX + rect.Width;
    bottom_right.Y = rect.Y + TacPixelY + rect.Height;

    for (int i = 0; i < unknown_BuildingClass_list_00a40af8.Count(); i++) {
        BuildingClass *bptr = unknown_BuildingClass_list_00a40af8[i];
        if (bptr->unknown_7d && bptr->unknown_74 && !bptr->Type->InvisibleInGame) {
            CoordStruct coord = bptr->Coord;
            Point2D point = Coord_To_Pixel_Absolute(coord);
            if (point.X >= top_left.X && point.X <= bottom_right.X && point.Y >= top_left.Y && point.Y <= bottom_right.Y) {
                Point2D view(TacPixelX, TacPixelY);
                point = point - view;
                Add_To_Selectables(bptr, point);
            }
        }
    }
}


// bool Tactical::Add_To_Selectables(ObjectClass *object, Point2D point) (0x006AA510): lists an
// object with its pixel (point plus the view position) in the table at 0x00ABE5F8
// when the table has room and the point is within 32 pixels of the tactical area
// (hypothesis; 0x006AA300 expands it).

bool Tactical::Add_To_Selectables(ObjectClass *object, Point2D point)
{
    if (unknown_db0 != 500 && point.X >= -32 && point.X <= unknown_rect_839630.Width + 32 && point.Y >= -32 && point.Y <= unknown_rect_839630.Height + 32) {
        unknown_tactical_pixels_abe5f8[unknown_db0].Object = object;
        Point2D view(TacPixelX, TacPixelY);
        unknown_tactical_pixels_abe5f8[unknown_db0].Pixel = Point2D(point.X + view.X, point.Y + view.Y);
        unknown_db0++;
        return true;
    }
    return false;
}


// void Tactical::Start_Rubber_Band(Point2D const &point) (0x006AA5A0): starts the selection band at point
// unless one is open.

void Tactical::Start_Rubber_Band(Point2D const &point)
{
    if (RubberBandStart.X == 0 && RubberBandStart.Y == 0) {
        RubberBandStart = point;
        RubberBandEnd = point;
    }
}


// void Tactical::Modify_Rubber_Band(Point2D const &point) (0x006AA5E0): drags an open selection band to point.

void Tactical::Modify_Rubber_Band(Point2D const &point)
{
    if (RubberBandStart.X != 0 || RubberBandStart.Y != 0) {
        RubberBandEnd = point;
    }
}


// void Tactical::Unknown_006AA610(void (__fastcall *callback)(ObjectClass *object)) (0x006AA610): ends the selection band: the
// rectangle spanned by its two corners (+0xD90, +0xD98) goes to 0x006AABC0 with the callback, then the band is cleared.

void Tactical::Unknown_006AA610(void (__fastcall *callback)(ObjectClass *object))
{
    if (RubberBandStart != Point2D(0, 0)) {
        int left = RubberBandStart.X;
        int right = RubberBandEnd.X;
        int bottom = RubberBandEnd.Y;
        int top = RubberBandStart.Y;
        if (left > right) {
            int swap = left;
            left = right;
            right = swap;
        }
        if (top > bottom) {
            int swap = top;
            top = bottom;
            bottom = swap;
        }
        Rect rect(left, top, right - left + 1, bottom - top + 1);
        Unknown_006AABC0(&rect, callback);
        RubberBandStart = Point2D(0, 0);
    }
}


// int Tactical::Unknown_006AA6A0() (0x006AA6A0): 1 when the pixel of a listed
// object (the table at 0x00ABE5F8, +0x7D set) lies inside the selection band
// spanned by +0xD90 and +0xD98, relative to the view position (hypothesis).

int Tactical::Unknown_006AA6A0()
{
    int left = RubberBandStart.X;
    int right = RubberBandEnd.X;
    int bottom = RubberBandEnd.Y;
    int top = RubberBandStart.Y;
    if (left > right) {
        int swap = left;
        left = right;
        right = swap;
    }
    if (top > bottom) {
        int swap = top;
        top = bottom;
        bottom = swap;
    }
    Rect rect(left, top, right - left + 1, bottom - top + 1);
    if (rect.Is_Valid()) {
        for (int index = 0; index < unknown_db0; index++) {
            TacticalObjectPixel &entry = unknown_tactical_pixels_abe5f8[index];
            if (entry.Object != 0 && entry.Object->unknown_7d) {
                Point2D view(TacPixelX, TacPixelY);
                Point2D point(entry.Pixel.X - view.X, entry.Pixel.Y - view.Y);
                if (rect.Is_Point_Within(point)) {
                    return 1;
                }
            }
        }
    }
    return 0;
}


// void Tactical::Unknown_006AA770() (0x006AA770): ends the selection band, both
// corners back to (0, 0) (TibSun Tactical::End_Rubber_Band).

void Tactical::Unknown_006AA770()
{
    RubberBandStart = Point2D(0, 0);
    RubberBandEnd = Point2D(0, 0);
}


// void Tactical::Unknown_006AA790() (0x006AA790): draws the selection band rectangle onto the current
// surface (clipped to the tactical area 0x00839630) in GUI color 15. The width, height and color are
// locals computed before the rectangle (they stay in callee-saved registers).

extern Rect unknown_rect_839630;

void Tactical::Unknown_006AA790()
{
    if (RubberBandStart != Point2D(0, 0)) {
        int left = RubberBandStart.X;
        int right = RubberBandEnd.X;
        int top = RubberBandStart.Y;
        int bottom = RubberBandEnd.Y;
        if (right < left) {
            int swap = left;
            left = right;
            right = swap;
        }
        if (bottom < top) {
            int swap = top;
            top = bottom;
            bottom = swap;
        }
        int width = right - left + 1;
        int height = bottom - top + 1;
        int color = unknown_drawer_8323c4->Convert(15);
        Rect rect(left, top, width, height);
        unknown_surface_83995c->Draw_Rect(unknown_rect_839630, rect, color);
    }
}


// Cell Tactical::Clamp_Cursor_To_Tactical(Cell const &cell, Cell const *list)
// (0x006AA970): the cell unchanged (TibSun Tactical::Clamp_Cursor_To_Tactical).

Cell Tactical::Clamp_Cursor_To_Tactical(Cell const &cell, Cell const *list)
{
    return cell;
}


// 0x006aa980: function not yet identified
//
// Body: 3 bytes (frozen inventory bound).
// Referenced from 0 sites.
// Ghidra prototype: void unknown_empty_006aa980(void)
//
// Types and names are Ghidra placeholders;
// rename and retype as the purpose becomes known.
//

namespace seed_006aa980 {

void __stdcall unknown_empty_006aa980(int unused_0)
{
    return;
}

}  // namespace seed_006aa980


// ObjectClass *Tactical::Get_Selectable_Object_006aa990(Point2D const &point) (0x006AA990): of the objects drawn this frame
// (the table at 0x00ABE5F8), the nearest to the view point within 200 (distance x*x + y*y/2) that may be picked:
// a techno that is active and not in limbo, the player's own, uncloaked or over a cell the player senses, or a
// veinhole terrain; failing that, the occupier of the cell under the point (TibSun Tactical::Get_Selectable_Object;
// the view pixel is two ints and the cell fallback sits under `if (best == 0)`: both set the original frame slots).
#include "shared/techno_map.hpp"
#include "shared/house_screen.hpp"

// Original RTTI-kind checked conversion: kinds 1, 2, 6 and 15, all TechnoClass descendants.
TechnoClass *__fastcall Infantry_As_Techno_0040dbd0(ObjectClass *object);

ObjectClass *Tactical::Get_Selectable_Object_006aa990(Point2D const &point)
{
    ObjectClass *best = 0;
    unsigned int bestdist = (unsigned int)-1;

    int pixelx = point.X + TacPixelX;
    int pixely = point.Y + TacPixelY;

    for (int index = 0; index < unknown_db0; index++) {
        TacticalObjectPixel &sel = unknown_tactical_pixels_abe5f8[index];
        ObjectClass *obj = sel.Object;

        if (obj != 0) {
            TechnoClass *tech = Infantry_As_Techno_0040dbd0(obj);
            bool pass = false;

            if (tech != 0) {
                if (obj->unknown_7d && !tech->unknown_76) {
                    if (tech->unknown_372 || tech->unknown_1b8 != 2 || ((UnknownCellClass *)&Map[tech->Center_Coord()])->Is_Sensed(unknown_player_a35db4->ArrayIndex)) {
                        pass = true;
                    }
                }
            }

            if (tech == 0) {
                if (obj->Class_Of() != 0 && obj->Class_Of()->Kind_Of() == 0x25) {
                    TerrainTypeClass *type = (TerrainTypeClass *)obj->Class_Of();
                    if (type->IsVeinhole) {
                        pass = true;
                    }
                }
            }

            if (pass) {
                Point2D pos = sel.Pixel - Point2D(pixelx, pixely);
                unsigned int dist = int(double(pos.X * pos.X) + double(pos.Y * pos.Y) * 0.5);
                if (dist < bestdist && dist < 200) {
                    bestdist = dist;
                    best = obj;
                }
            }
        }
    }
    if (best == 0) {
        Cell cell = Pixel_To_Cell(Point2D(unknown_rect_839630.X, unknown_rect_839630.Y) + point);
        best = Map[cell].OccupierPtr;
    }
    return best;
}


// void Tactical::Detach(AbstractClass *target, bool all) (0x006AAB60, slot 10)
// Forgets a target: clears it from the selectable object table and, when it is
// the object waiting for placement, cancels the placement cursor (TibSun
// Tactical::Detach; the placement part is RA2's).

void Tactical::Detach(AbstractClass *target, bool all)
{
    for (int index = 0; index < 500; index++) {
        TacticalObjectPixel &sel = unknown_tactical_pixels_abe5f8[index];
        if (sel.Object == (ObjectClass *)target) {
            sel.Object = 0;
        }
    }
    if (target == Map.unknown_118c) {
        Map.Set_Cursor_Shape(0);
        Map.unknown_118c = 0;
        Map.unknown_1190 = 0;
        Map.unknown_1194 = -1;
    }
}


// void Tactical::Unknown_006AABC0(Rect const *rect, void (__fastcall *callback)(ObjectClass *))
// (0x006AABC0): every listed object (the table at 0x00ABE5F8) whose pixel lies
// in rect: its type ID to 0x006F71E0 while the 0x006F76B0 flag is set, else the
// callback, else the default selection test that clears 0x007D88D6 (as
// 0x0049C3F0 does). 0x007D88D6 is set before and after (hypothesis).

void Tactical::Unknown_006AABC0(Rect const *rect, void (__fastcall *callback)(ObjectClass *object))
{
    unknown_sound_enabled_7d88d6 = true;
    if (rect->Is_Valid()) {
        for (int index = 0; index < unknown_db0; index++) {
            TacticalObjectPixel &entry = unknown_tactical_pixels_abe5f8[index];
            if (entry.Object != 0 && entry.Object->unknown_7d) {
                ObjectClass *object = entry.Object;
                Point2D view(TacPixelX, TacPixelY);
                Point2D point(entry.Pixel.X - view.X, entry.Pixel.Y - view.Y);
                if (rect->Is_Point_Within(point)) {
                    if (Unknown_006F76B0()) {
                        Unknown_006F71E0(object->Class_Of()->ID);
                    } else if (callback == 0) {
                        bool undeploys = false;
                        if (object->Kind_Of() == 6) {
                            BuildingTypeClass *type = ((BuildingClass *)object)->Type;
                            if (type->UndeploysInto != 0 && !type->ConstructionYard) {
                                undeploys = true;
                            }
                        }
                        HouseClass *house = object->Owning_House();
                        if (house != 0 && house->Is_Player_Control() && object->CanBeSelected()
                            && (object->Kind_Of() != 6 || undeploys) && object->unknown_slot80()) {
                            unknown_sound_enabled_7d88d6 = false;
                        }
                    } else {
                        callback(object);
                    }
                }
            }
        }
    }
    unknown_sound_enabled_7d88d6 = true;
}


// void Tactical::Unknown_006AAD40() (0x006AAD40): unselects every selected object
// (slot 81 of the first until none is left), then clears the pending command
// (0x006F6780 with 0; hypothesis).
#include "shared/vector_class.hpp"

void __fastcall Unknown_006f6780(unsigned int value);

void Tactical::Unknown_006AAD40()
{
    while (unknown_objects_a40c60.Count()) {
        unknown_objects_a40c60[0]->Unselect();
    }
    Unknown_006f6780(0);
}


// bool Tactical::Unknown_006AAD70() (0x006AAD70): with exactly one object selected,
// owned by a house that is not the player's (+0x135 in the campaign,
// 0x004F52B0 otherwise), unselects it (slot 81) and returns true (hypothesis).

bool Tactical::Unknown_006AAD70()
{
    if (unknown_objects_a40c60.Count() == 1) {
        ObjectClass *object = unknown_objects_a40c60[0];
        if (object->Owning_House() != 0) {
            if (unknown_game_mode_a3d298 == 0) {
                if (!object->Owning_House()->unknown_135) {
                    object->Unselect();
                    return true;
                }
            } else if (!object->Owning_House()->Is_Player_Control()) {
                object->Unselect();
                return true;
            }
        }
    }
    return false;
}


// SPDX-License-Identifier: GPL-3.0-or-later
// Copyright 2026 ra2-decomp contributors
// Copyright 2025 Electronic Arts Inc.; 2026 OpenTS contributors
// Derived from Electronic Arts source code; EA's GPL Section 7 terms apply (COPYING.md).
// void Tactical::Flag_Cell(CellClass *cell) (0x006AADD0)
// Adopted from ra2-decomp Tactical::Flag_Cell (YR 0x006DA7D0).
// During a dirty redraw, lists a mapped (or newly revealed) cell not yet
// flagged this frame for redrawing when its ground pixel lies near the
// tactical area, and marks the view to redraw.
#include "shared/cd_timer.hpp"

void Tactical::Flag_Cell(CellClass *cell)
{
    if (Map.IsToRedraw == 0 && cell->unknown_5c != Frame && ((cell->unknown_12c & 8) || cell->unknown_138)) {
        cell->unknown_5c = Frame;

        CoordStruct coord = cell->unknown_475d60();

        CoordStruct ground = CoordStruct(coord.X, coord.Y, 0);

        Point2D pixel;
        if ((cell->unknown_12c & 8) && (cell->unknown_12c & 0x10)) {
            Coord_To_Pixel(ground, pixel);
        } else {
            Coord_To_Pixel(ground, pixel);
        }

        if (pixel.X >= unknown_rect_839630.X - 30
            && pixel.Y >= -30
            && pixel.X <= unknown_rect_839630.X + unknown_rect_839630.Width + 30
            && pixel.Y <= unknown_rect_839630.Y + unknown_rect_839630.Height + 15) {

            if (unknown_e0 < 799) {
                unknown_e4[unknown_e0] = cell;
                unknown_e0++;
            }
            cell->unknown_138 = false;
            unknown_d7d = true;
        }
    }
}


// void Tactical::Reset_Dirty_Rectangles() (0x006AAF80): deletes the dirty areas
// (TacticalLists::DirtyAreas) one by one from the front (TibSun Tactical::Reset_Dirty_Rectangles).

void Tactical::Reset_Dirty_Rectangles()
{
    while (TacticalLists::DirtyAreas.Count() > 0) {
        TacticalLists::DirtyAreas.Delete(0);
    }
}


// void Tactical::Draw_Rally_Points(bool inshroud) (0x006AAFD0): for every selected factory of the player's house
// with a rally target (+0x1B0), a marching dashed line from the building to the target (ground height, lifted
// onto a bridge): one line 2 pixels low in the planning colour, then two in the house colour (TibSun Draw_Rally_Points). The
// `offset` local and the two sums it feeds count as inline sites: they push Coord_To_Pixel_Absolute out of line
// at the first Coord_To_Pixel and Point2D::operator-= out of line at the second.
#include "shared/planning.hpp"

extern bool Draw_Rally_Pattern[];

void Tactical::Draw_Rally_Points(bool inshroud)
{
    int phase = (0x7FFFFFFF - Frame) % 15;
    int shadow = Build_Hicolor_Pixel(unknown_rgb_ab6841.Red, unknown_rgb_ab6841.Green, unknown_rgb_ab6841.Blue);
    for (int index = unknown_objects_a40c60.Count() - 1; index >= 0; index--) {
        ObjectClass *object = unknown_objects_a40c60[index];
        if (object->Kind_Of() == 6 && object->unknown_7d && object->unknown_77) {
            BuildingClass *building = (BuildingClass *)object;
            if (building->House == unknown_player_a35db4 && building->unknown_slot153() && building->unknown_1b0 != 0) {
                CoordStruct center = building->Center_Coord();
 Point2D start;
 Coord_To_Pixel(center, start);
 start += Point2D(unknown_rect_839630.X, unknown_rect_839630.Y);
                CoordStruct rally = building->unknown_1b0->Center_Coord();
                rally.Z = Map.Get_Height_GL(rally);
                if (Map[rally].unknown_140 & 0x100) {
                    rally.Z += unknown_geometry_j;
                }
                Point2D end;
                Coord_To_Pixel(rally, end);
                end += Point2D(unknown_rect_839630.X, unknown_rect_839630.Y);
                Point2D offset(0, 2);
 start += offset;
 end += offset;
                if (Clip_Line(start, end, unknown_rect_839630)) {
                    unknown_surface_83995c->Draw_Dashed_Line_Depth_Masked(start, end, shadow, Draw_Rally_Pattern, phase, inshroud);
                }
                RGBClass &rgb = building->House->unknown_55e9;
 int color = Build_Hicolor_Pixel(rgb.Red, rgb.Green, rgb.Blue);
                start.Y--;
                end.Y--;
                if (Clip_Line(start, end, unknown_rect_839630)) {
                    unknown_surface_83995c->Draw_Dashed_Line_Depth_Masked(start, end, color, Draw_Rally_Pattern, phase, inshroud);
                }
                start.Y--;
                end.Y--;
                if (Clip_Line(start, end, unknown_rect_839630)) {
                    unknown_surface_83995c->Draw_Dashed_Line_Depth_Masked(start, end, color, Draw_Rally_Pattern, phase, inshroud);
                }
              }
        }
    }
}


// void Tactical::Draw_Waypoints(bool inshroud) (0x006AB360): the player's waypoint paths over the view: the cursor
// and the index of every waypoint on screen on the requested side of the shroud, and a line to the next waypoint,
// marching dashes on the selected path. The four table entries the dashed line cycles are saved around the drawing
// (TibSun Draw_Waypoints). `offset` holds the rectangle's Y only, in a Point2D. Print_Label is a file-local wrapper:
// it charges the inline budget that keeps the second Coord_To_Pixel's Rectangular_To_Isometric out of line.
#include "shared/waypoint_path.hpp"
#include "shared/convert.hpp"
#include "shared/mix_file_class.hpp"

extern ConvertClass *unknown_drawer_8323c8;

// The four table entries the dashed line cycles (the table at +2 as a block).
struct Dash_Entries {
    int Value[4];
};
extern bool Waypoint_Line_Style[];
extern void *Waypoint_Mouse_Shapes;
extern wchar_t Waypoint_Text[];

static inline void Print_Label(Point2D *point, ColorScheme *scheme)
{
    Fancy_Text_Print(Waypoint_Text, unknown_surface_83995c, &unknown_rect_839630, point, scheme, 0, 0x109);
}
void Tactical::Draw_Waypoints(bool inshroud)
{
    int phase = (0x7FFFFFFF - Frame) % 15;
    int start_frame = Map.Get_Mouse_Start(0x3C);
    int frame = unknown_da0 % Map.Get_Mouse_Count(0x3C) + start_frame;
    if (Debug_Map) {
        return;
    }
    Dash_Entries *dashtable = (Dash_Entries *)((unsigned short *)unknown_drawer_8323c8->Table + 1);
    Dash_Entries dashsave = *(Dash_Entries *)((unsigned short *)unknown_drawer_8323c8->Table + 1);
    if (Waypoint_Mouse_Shapes == 0) {
        Waypoint_Mouse_Shapes = (void *)MixFileClass::Retrieve("MOUSE.SHP");
    }
    Point2D offset(0, unknown_rect_839630.Y + 1);
    int coloridx = unknown_id_source_a3d290->Theater != 1 ? 14 : 12;
    for (int path = 0; path < 12; path++) {
        unknown_player_a35db4->Make_Waypoint_Path(path);
        int count = unknown_player_a35db4->WaypointPaths[path]->Waypoints.Count();
        if (count != 0) {
            Map.Update_Waypoint_Color(path);
        }
        for (int index = 0; index < count; index++) {
            WaypointClass *waypoint = unknown_player_a35db4->WaypointPaths[path]->Get_Waypoint(index);
            Point2D pixel;
            bool onscreen;
            if (Coord_To_Pixel(waypoint->Coord, pixel) && ((inshroud && Map.Is_Shrouded(waypoint->Coord)) || (!inshroud && !Map.Is_Shrouded(waypoint->Coord)))) {
                onscreen = true;
            } else {
                onscreen = false;
            }
            pixel.Y += unknown_rect_839630.Y;
            if (onscreen) {
                CC_Draw_Shape(unknown_surface_83995c, unknown_drawer_8323c8, (ShapeFileStruct const *)Waypoint_Mouse_Shapes, frame, pixel - offset, unknown_rect_839630, 0x600, 0, 0, 0, 1000, 0, 0, Point2D(0, 0));
                swprintf(Waypoint_Text, L"%d", index);
                Point2D label = pixel - offset + Point2D(-1, -25);
                Print_Label(&label, ColorSchemes[coloridx]);
            }
            WaypointClass *next = unknown_player_a35db4->WaypointPaths[path]->Next_Waypoint(waypoint);
            if (next != 0) {
                Point2D nextpixel;
                Coord_To_Pixel(next->Coord, nextpixel);
                nextpixel.Y += unknown_rect_839630.Y;
                if (Clip_Line(pixel, nextpixel, unknown_rect_839630)) {
                    if (path == unknown_player_a35db4->unknown_154) {
                        phase = unknown_surface_83995c->Draw_Dashed_Line_Depth_Masked(pixel, nextpixel, unknown_drawer_8323c8->Convert(3), Waypoint_Line_Style, phase, inshroud);
                    } else {
                        unknown_surface_83995c->Draw_Line_Depth_Masked(pixel, nextpixel, unknown_drawer_8323c8->Convert(3), inshroud);
                    }
                }
            }
        }
    }
    *dashtable = dashsave;
}


// Tactical occupier-link diagnostic, complete original 0x006AB810 (1036+4).
#include "shared/cell_class.hpp"
#include "shared/object_lists.hpp"
extern Rect unknown_rect_839630;

void Tactical::Debug_Draw_Occupier_Links()
{
    Map.Reset_Iterator();
    CellClass *cell = Map.Iterate();
    int ground_color = Build_Hicolor_Pixel(255,255,0);
    int link_color = Build_Hicolor_Pixel(255,0,255);
    int bridge_color = Build_Hicolor_Pixel(0,255,255);
    while (cell) {
        ObjectClass *object = cell->OccupierPtr;
        if (object) {
            CoordStruct cell_coord = cell->unknown_475d60();
            Point2D start;
            Coord_To_Pixel(cell_coord,start);
            CoordStruct object_coord = object->Center_Coord();
            Point2D end;
            Coord_To_Pixel(object_coord,end);
            Rect rect(start.X-1,start.Y-1,3,3);
            unknown_surface_83995c->Fill_Rect(unknown_rect_839630,rect,ground_color);
            unknown_surface_83995c->Draw_Line(unknown_rect_839630,start,end,ground_color);
        }
        object = cell->AltOccupierPtr;
        if (object) {
            CoordStruct cell_coord = cell->unknown_475d60();
            Point2D start;
            Coord_To_Pixel(cell_coord,start);
            start.Y -= 60;
            CoordStruct object_coord = object->Center_Coord();
            Point2D end;
            Coord_To_Pixel(object_coord,end);
            Rect rect(start.X-1,start.Y-1,3,3);
            unknown_surface_83995c->Fill_Rect(unknown_rect_839630,rect,bridge_color);
            unknown_surface_83995c->Draw_Line(unknown_rect_839630,start,end,bridge_color);
        }
        cell = Map.Iterate();
    }
    for (int index=unknown_objects_a40368.Count()-1;index>=0;--index) {
        ObjectClass *object = unknown_objects_a40368[index];
        if (object->unknown_30) {
            CoordStruct first_coord = object->Center_Coord();
            Point2D start;
            Coord_To_Pixel(first_coord,start);
            CoordStruct next_coord = object->unknown_30->Center_Coord();
            Point2D end;
            Coord_To_Pixel(next_coord,end);
            unknown_surface_83995c->Draw_Line(unknown_rect_839630,start,end,link_color);
        }
    }
}


#include "shared/swizzle.hpp"


// void Tactical::Debug_Draw_Occupation_Flags() (0x006ABC20): for every cell in view, a 4x4 block in yellow for each
// occupation bit it carries, the ground's (Occupy_124) beside the cell centre and the bridge deck's (Occupy_128) 60
// pixels higher (TibSun Tactical::Debug_Draw_Occupation_Flags). The centre goes through the inlined Coord_To_Pixel,
// as in Debug_Draw_Occupier_Links: its Coord_To_Pixel_Absolute 0x006A27A0 stays out of line one level down.
extern Rect unknown_rect_839630;

void Tactical::Debug_Draw_Occupation_Flags()
{
    Map.Reset_Iterator();
    CellClass *cell = Map.Iterate();
    int color = Build_Hicolor_Pixel(255, 255, 0);
    while (cell) {
        Rect render = cell->Cell_Render_Rect();
        Rect inter = Intersect(render, unknown_rect_839630);
        if (inter.Is_Valid()) {
            CoordStruct coord = cell->unknown_475d60();
            Point2D pixel;
            Coord_To_Pixel(coord, pixel);
            Rect rect = Rect(pixel.X - 2, unknown_rect_839630.Y + pixel.Y - 2, 4, 4);
            if (cell->Occupy_124.NE) {
                unknown_surface_83995c->Fill_Rect(rect + RectPoint2D(10, 0), color);
            }
            if (cell->Occupy_124.SE) {
                unknown_surface_83995c->Fill_Rect(rect + RectPoint2D(0, 6), color);
            }
            if (cell->Occupy_124.SW) {
                unknown_surface_83995c->Fill_Rect(rect + RectPoint2D(-10, 0), color);
            }
            if (cell->Occupy_124.Building || cell->Occupy_124.Monolith || cell->Occupy_124.Vehicle) {
                unknown_surface_83995c->Fill_Rect(rect, color);
            }
            if (cell->Occupy_128.NE) {
                unknown_surface_83995c->Fill_Rect(rect + RectPoint2D(10, -60), color);
            }
            if (cell->Occupy_128.SE) {
                unknown_surface_83995c->Fill_Rect(rect + RectPoint2D(0, -66), color);
            }
            if (cell->Occupy_128.SW) {
                unknown_surface_83995c->Fill_Rect(rect + RectPoint2D(-10, -60), color);
            }
            if (cell->Occupy_128.Building || cell->Occupy_128.Monolith || cell->Occupy_128.Vehicle) {
                unknown_surface_83995c->Fill_Rect(rect + Point2D(0, -60), color);
            }
        }
        cell = Map.Iterate();
    }
}


// Tactical depth-shaded world line, original006AC160 (374+10).
extern Rect unknown_rect_839630;

bool Tactical::unknown_slot24(CoordStruct const &from, CoordStruct const &to, int color, bool write_depth)
{
    Point2D start;
    Coord_To_Pixel(from,start);
    Point2D end;
    Coord_To_Pixel(to,end);
    unknown_surface_83995c->Draw_Depth_Shaded_Line(unknown_rect_839630,start,end,color,
        14-Unknown_006A2970(from.Z),14-Unknown_006A2970(to.Z),write_depth);
    return true;
}


// HRESULT __stdcall Tactical::GetClassID(GUID *clsid) (0x006AC2E0, slot 3)
// Reports the tactical view's class ID (TibSun Tactical::GetClassID).

extern "C" GUID const CLSID_Tactical;   // 0x007A1E90

HRESULT __stdcall Tactical::GetClassID(GUID *clsid)
{
    if (clsid == 0) {
        return E_POINTER;
    }
    *clsid = CLSID_Tactical;
    return S_OK;
}


// HRESULT Tactical::Load(IStream *) (0x006AC320): the base load, the
// placement NoInit constructor, the pointer table at +0xE4 swizzled, then the
// view position clamped (0x006A8C80) unless 0x00A40D13 is set and stored as
// both the current and the requested position.

HRESULT __stdcall Tactical::Load(IStream *stream)
{
    HRESULT result = AbstractClass::Load(stream);
    if (SUCCEEDED(result)) {
        new (this) Tactical(NoInitClass());
        unknown_db0 = 0;
        for (int i = 0; i < unknown_e0; i++) {
            Swizzler.Swizzle((void **)&unknown_e4[i]);
        }
        Point2D point = unknown_d64;
        int x;
        int y;
        if (Unknown_006A8C80(&point) && !Debug_Map) {
            x = point.X;
            y = point.Y;
        } else {
            x = unknown_d64.X;
            y = unknown_d64.Y;
        }
        unknown_d64 = Point2D(x, y);
        unknown_d74 = Point2D(x, y);
        Unknown_006A9170();
        unknown_d7d = true;
        result = S_OK;
    }
    return result;
}


// Not exact (RA2_NON_MATCHING): Tactical::Draw_Radial_Indicators at 0x006AC420, best score 77.41, 2026-11-13; differs: 269 instructions in the original, 271 in the draft (shape 94.44)
#ifdef RA2_NON_MATCHING
// void Tactical::Draw_Radial_Indicators() (0x006AC420): TibSun's loop over the current objects whose type
// has a radial indicator, then RA2's placement preview: while a building is waiting for placement and has a
// detection radius, its house-coloured radius is drawn pulsing at the cursor cell, and the nearest of the
// player's buildings of the same type (within twice the radius) draws its own indicator. Names are
// hypotheses: the placement flag (0x00A40B32), the pulse level (0x00ABFD78) and its direction (0x007F6974).
#include "shared/rgb_class.hpp"
void __fastcall Draw_Radial_Indicator_00451BF0(unsigned int radius, unsigned char mode,
    CoordStruct center, RGBClass color, bool filled, bool radius_is_pixels);
extern bool unknown_a40b32;        // 0x00A40B32 (the placement preview is shown)
extern int unknown_pulse_abfd78;   // 0x00ABFD78 (the pulse level, 0..100)
extern int unknown_pulse_7f6974;   // 0x007F6974 (the pulse direction, +1/-1)

void Tactical::Draw_Radial_Indicators()
{
    int count = unknown_objects_a40c60.Count();
    for (int i = 0; i < count; i++) {
        ObjectClass *obj = unknown_objects_a40c60[i];
        if (obj->Class_Of() && obj->Class_Of()->unknown_208) {
            obj->unknown_slot74(1);
        }
    }

    if (unknown_a40b32) {
        BuildingClass *pending = (BuildingClass *)Map.unknown_11a0;
        if (pending != 0 && pending->Kind_Of() == 6) {
            if (pending->unknown_451930() > 0) {
                Cell cell = Map.unknown_1170;
                CoordStruct coord = Map[cell].unknown_475d60();
                static RGBClass black(0, 0, 0);
                unknown_pulse_abfd78 += unknown_pulse_7f6974 * 10;
                if (unknown_pulse_abfd78 >= 100) {
                    unknown_pulse_7f6974 = -1;
                } else if (unknown_pulse_abfd78 <= 0) {
                    unknown_pulse_7f6974 = 1;
                }
                RGBClass color = pending->House->unknown_55e9;
                color.Adjust(unknown_pulse_abfd78, black);
                int radius = pending->unknown_451930();
                Draw_Radial_Indicator_00451BF0(radius, 0, coord, color, true, false);

                int closest = 0x7FFFFFFF;
                int count = unknown_player_a35db4->Buildings.Count();
                for (int index = 0; index < count; index++) {
                    BuildingClass *building = unknown_player_a35db4->Buildings[index];
                    if (building != 0 && building->unknown_7d && building != pending && building->Type == pending->Type) {
                        Cell zone = Map.unknown_1170;
                        Cell bcell = Cell(building->Center_Coord());
                        int dist = zone.Distance_To(bcell);
                        if (dist <= radius * 2 && dist < closest) {
                            closest = dist;
                        }
                    }
                }
                for (index = 0; index < count; index++) {
                    BuildingClass *building = unknown_player_a35db4->Buildings[index];
                    if (building != 0 && building->unknown_7d && building != pending && building->Type == pending->Type) {
                        Cell zone = Map.unknown_1170;
                        Cell bcell = Cell(building->Center_Coord());
                        int dist = zone.Distance_To(bcell);
                        if (dist == closest) {
                            building->unknown_slot74(0);
                        }
                    }
                }
            }
        }
    }
}
#endif

// 0x006ac400: function not yet identified
//
// Owner: Tactical, virtual slot 6 (vtable +0x18) (from RTTI/vtables).
// Body: 29 bytes (frozen inventory bound).
// Referenced from 1 site (data).
// Ghidra prototype: int Save_006ac400(undefined4 param_1,undefined4 param_2,undefined4 param_3)
//
// Types and names are Ghidra placeholders;
// rename and retype as the purpose becomes known.
//

namespace seed_006ac400 {

extern unsigned int __stdcall AbstractClass_Save_004100c0(int, int, int);

int __stdcall Save_006ac400(unsigned int self, unsigned int stream, unsigned int clear_dirty)
{
    int result;

    result = (int)(AbstractClass_Save_004100c0((int)(self), (int)(stream), (int)(clear_dirty)));
    if (result >= 0) {
        result = (int)(0);
    }
    return result;
}

}  // namespace seed_006ac400


// 0x006ac7a0: function not yet identified
//
// Body: 1 bytes (frozen inventory bound).
// Referenced from 1 site (data).
// Ghidra prototype: void unknown_empty_006ac7a0(void)
//
// Types and names are Ghidra placeholders;
// rename and retype as the purpose becomes known.
//

namespace seed_006ac7a0 {

void  unknown_empty_006ac7a0(void)
{
    return;
}

}  // namespace seed_006ac7a0


class Leaf_6ac7b0 { public: unsigned int get(); };
unsigned int Leaf_6ac7b0::get() { return *(unsigned int*)((char*)this + 0xdb0); }


// 0x006ac7c0: function not yet identified
//
// Body: 17 bytes (frozen inventory bound).
// Referenced from 1 site (unconditional call).
// Ghidra prototype: undefined4 unknown_get_006ac7c0(int param_1)
//
// Types and names are Ghidra placeholders;
// rename and retype as the purpose becomes known.
//

namespace seed_006ac7c0 {

extern "C" unsigned int SelectableObjects_00abe5f8;

unsigned int __stdcall unknown_get_006ac7c0(int index)
{
    return (&SelectableObjects_00abe5f8)[index * 3];
}

}  // namespace seed_006ac7c0


// int Tactical::Kind_Of() const (0x006AC7E0)

int Tactical::Kind_Of() const
{
    return 0x38;
}


// int Tactical::Size_Of() const (0x006AC7F0)

int Tactical::Size_Of() const
{
    return sizeof(Tactical);
}


// 0x006ac800: function not yet identified
//
// Owner: Tactical, virtual slot 8 (vtable +0x20) (from RTTI/vtables).
// Body: 67 bytes (frozen inventory bound).
// Referenced from 1 site (data).
// Ghidra prototype: undefined4 * __thiscall scalar_deleting_destructor_006ac800(undefined4 *param_1,byte param_2)
// Callees with accepted source:
//   AbstractClass_dtor_0040ff90 -> src/recovered/classes/AbstractClass/0040ff90.cpp
//   operator_delete_00781d3b -> src/recovered/00781d3b.cpp
//
// Types and names are Ghidra placeholders;
// rename and retype as the purpose becomes known.
//

namespace seed_006ac800 {

extern void __fastcall AbstractClass_dtor_0040ff90(void);
extern void  operator_delete_00781d3b(unsigned int);
extern "C" unsigned int TacticalMap_0083996c;
extern "C" void* Tactical_INoticeSource_vtable_007ac544;
extern "C" void* Tactical_INoticeSink_vtable_007ac54c;
extern "C" void* Tactical_IRTTITypeInfo_vtable_007ac554;
extern "C" void* Tactical_vtable_007ac570;

struct Owner_006ac800 { unsigned int * scalar_deleting_destructor_006ac800(unsigned char flags); };

unsigned int * Owner_006ac800::scalar_deleting_destructor_006ac800(unsigned char flags)
{
    unsigned int * self = (unsigned int *)this;
    *self = (unsigned int)(&Tactical_vtable_007ac570);
    self[1] = (unsigned int)(&Tactical_IRTTITypeInfo_vtable_007ac554);
    self[2] = (unsigned int)(&Tactical_INoticeSink_vtable_007ac54c);
    self[3] = (unsigned int)(&Tactical_INoticeSource_vtable_007ac544);
    TacticalMap_0083996c = 0;
    AbstractClass_dtor_0040ff90();
    if ((flags & 1) != 0) {
        operator_delete_00781d3b((unsigned int)(self));
    }
    return self;
}

}  // namespace seed_006ac800


// Point2D &Point2D::operator-=(Point2D const &point) (0x006AC850): the out-of-line copy of the inline in
// shared/rect.hpp, emitted by the Tactical unit where /Ob2 refuses it (Draw_Rally_Points, Draw_Waypoints).


// Container members this unit emits by its own use of its lists (bodies: shared/vector_class.hpp):
// DynamicVectorClass<DirtyAreaStruct>::Clear (0x006AC870; DynamicVectorClass<DirtyAreaStruct> vtable 0x007ac4c4 slot 3).
// The element type is left incomplete: Clear does not depend on it.
// DynamicVectorClass<DirtyAreaStruct>::ID_pointer (0x006AC8A0; DynamicVectorClass<DirtyAreaStruct> vtable 0x007ac4c4 slot 5).
// DirtyAreaStruct is a size-only stand-in (20 bytes, from the ID(T const *) divisor);
// its real fields are not recovered.
// VectorClass<DirtyAreaStruct>::Fetch (0x006AC8D0).
// DynamicVectorClass<ShadowControlClass *>::Clear (0x006AC910; DynamicVectorClass<ShadowControlClass *> vtable 0x007ac504 slot 3).
// DynamicVectorClass<ShadowControlClass *>::ID_pointer (0x006AC940; DynamicVectorClass<ShadowControlClass *> vtable 0x007ac504 slot 5).
// VectorClass<ShadowControlClass *>::Fetch (0x006AC960; VectorClass<ShadowControlClass *> vtable 0x007ac524 slot 6).
// VectorClass<DirtyAreaStruct>::operator== (0x006AC970).
// VectorClass<DirtyAreaStruct>::Resize (0x006AC9F0).
// VectorClass<DirtyAreaStruct>::Clear (0x006ACB10; VectorClass<DirtyAreaStruct> vtable 0x007ac4e4 slot 3).
// The element type is left incomplete: Clear does not depend on it.
// VectorClass<DirtyAreaStruct>::ID_object (0x006ACB40; virtual slot 4).
// Layout and behaviour: shared/vector_class.hpp; element comparison: shared/vector_elements.hpp.
// VectorClass<DirtyAreaStruct>::ID_pointer (0x006ACBB0; VectorClass<DirtyAreaStruct> vtable 0x007ac4e4 slot 5).
// DirtyAreaStruct is a size-only stand-in (20 bytes, from the ID(T const *) divisor);
// its real fields are not recovered.
// DynamicVectorClass<DirtyAreaStruct>::Resize (0x006ACBE0).
// DynamicVectorClass<DirtyAreaStruct>::ID_object (0x006ACD00; virtual slot 4).
// Layout and behaviour: shared/vector_class.hpp; element comparison: shared/vector_elements.hpp.
// VectorClass<ShadowControlClass *>::operator equal (0x006ACD60; VectorClass<ShadowControlClass *> vtable 0x007ac524 slot 1).
// VectorClass<ShadowControlClass *>::Resize (0x006ACDA0; VectorClass<ShadowControlClass *> vtable 0x007ac524 slot 2).
// VectorClass<ShadowControlClass *>::Clear (0x006ACE50; VectorClass<ShadowControlClass *> vtable 0x007ac524 slot 3).
// VectorClass<ShadowControlClass *>::ID_object (0x006ACE80; VectorClass<ShadowControlClass *> vtable 0x007ac524 slot 4).
// VectorClass<ShadowControlClass *>::ID_pointer (0x006ACEC0; VectorClass<ShadowControlClass *> vtable 0x007ac524 slot 5).
// DynamicVectorClass<ShadowControlClass *>::Resize (0x006ACEE0; DynamicVectorClass<ShadowControlClass *> vtable 0x007ac504 slot 2).
// DynamicVectorClass<ShadowControlClass *>::ID_object (0x006ACF90; DynamicVectorClass<ShadowControlClass *> vtable 0x007ac504 slot 4).


extern "C" unsigned char vtable_6acfc0[]; extern "C" void __cdecl release_buffer_6acfc0(void*); extern "C" void __cdecl release_object_6acfc0(void*);
class Leaf_6acfc0 { public: void* get(unsigned int flags); };
void* Leaf_6acfc0::get(unsigned int flags) { unsigned int pointer = *(unsigned int*)((char*)this + 4); *(unsigned int*)this=(unsigned int)vtable_6acfc0; if(pointer && *(unsigned char*)((char*)this + 13)){ release_buffer_6acfc0((void*)pointer); *(unsigned int*)((char*)this + 4)=0; } *(unsigned char*)((char*)this + 13)=0; *(unsigned int*)((char*)this + 8)=0; if(flags & 1) release_object_6acfc0(this); return this; }


extern "C" unsigned char vtable_6ad010[]; extern "C" void __cdecl release_buffer_6ad010(void*); extern "C" void __cdecl release_object_6ad010(void*);
class Leaf_6ad010 { public: void* get(unsigned int flags); };
void* Leaf_6ad010::get(unsigned int flags) { unsigned int pointer = *(unsigned int*)((char*)this + 4); *(unsigned int*)this=(unsigned int)vtable_6ad010; if(pointer && *(unsigned char*)((char*)this + 13)){ release_buffer_6ad010((void*)pointer); *(unsigned int*)((char*)this + 4)=0; } *(unsigned char*)((char*)this + 13)=0; *(unsigned int*)((char*)this + 8)=0; if(flags & 1) release_object_6ad010(this); return this; }


extern "C" unsigned char vtable_6ad060[]; extern "C" void __cdecl release_buffer_6ad060(void*); extern "C" void __cdecl release_object_6ad060(void*);
class Leaf_6ad060 { public: void* get(unsigned int flags); };
void* Leaf_6ad060::get(unsigned int flags) { unsigned int pointer = *(unsigned int*)((char*)this + 4); *(unsigned int*)this=(unsigned int)vtable_6ad060; if(pointer && *(unsigned char*)((char*)this + 13)){ release_buffer_6ad060((void*)pointer); *(unsigned int*)((char*)this + 4)=0; } *(unsigned char*)((char*)this + 13)=0; *(unsigned int*)((char*)this + 8)=0; if(flags & 1) release_object_6ad060(this); return this; }


extern "C" unsigned char vtable_6ad0b0[]; extern "C" void __cdecl release_buffer_6ad0b0(void*); extern "C" void __cdecl release_object_6ad0b0(void*);
class Leaf_6ad0b0 { public: void* get(unsigned int flags); };
void* Leaf_6ad0b0::get(unsigned int flags) { unsigned int pointer = *(unsigned int*)((char*)this + 4); *(unsigned int*)this=(unsigned int)vtable_6ad0b0; if(pointer && *(unsigned char*)((char*)this + 13)){ release_buffer_6ad0b0((void*)pointer); *(unsigned int*)((char*)this + 4)=0; } *(unsigned char*)((char*)this + 13)=0; *(unsigned int*)((char*)this + 8)=0; if(flags & 1) release_object_6ad0b0(this); return this; }
