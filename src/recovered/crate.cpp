// crate.cpp: the original translation unit re-filed as one source (Route A): the accepted
// bodies of its canonical compile group in address order, under the unit's own header
// state (its switches and includes in their original order). The module map
// manifests/original_target/modules/crate.json carries the addresses and the rows that
// stay in their own files.


#include "shared/shape_file.hpp"
#include "shared/unknown_geometry.hpp"
#include "shared/crate.hpp"
#include "shared/cell.hpp"
#include "shared/map_view.hpp"
#include "shared/rules.hpp"
#include "shared/tactical.hpp"
#include "shared/id_source.hpp"
#include "shared/overlay.hpp"
#include "shared/unknown_globals.hpp"
#include "shared/overlay_type.hpp"


// Header-static dynamic initializers of the unit at 0x00493740: the shape_file.hpp empty rect and the
// shared/unknown_geometry.hpp constants (0x00493740-0x00493921); the other
// drafts of this plan are emitted by these includes.


// Dynamic initializer (0x00493760, _$E) of diagonal from the header(s) included
// by HeaderStatics_empty_rect_init_00493740.cpp; no source of its own.


// Dynamic initializer (0x00493790, _$E) of b from the header(s) included
// by HeaderStatics_empty_rect_init_00493740.cpp; no source of its own.


// Dynamic initializer (0x004937B0, _$E) of c from the header(s) included
// by HeaderStatics_empty_rect_init_00493740.cpp; no source of its own.


// Dynamic initializer (0x004937D0, _$E) of angle45 from the header(s) included
// by HeaderStatics_empty_rect_init_00493740.cpp; no source of its own.


// Dynamic initializer (0x004937F0, _$E) of angle60 from the header(s) included
// by HeaderStatics_empty_rect_init_00493740.cpp; no source of its own.


// Dynamic initializer (0x00493810, _$E) of angle90 from the header(s) included
// by HeaderStatics_empty_rect_init_00493740.cpp; no source of its own.


// Dynamic initializer (0x00493830, _$E) of g from the header(s) included
// by HeaderStatics_empty_rect_init_00493740.cpp; no source of its own.


// Dynamic initializer (0x00493870, _$E) of h from the header(s) included
// by HeaderStatics_empty_rect_init_00493740.cpp; no source of its own.


// Dynamic initializer (0x004938A0, _$E) of i from the header(s) included
// by HeaderStatics_empty_rect_init_00493740.cpp; no source of its own.


// Dynamic initializer (0x004938D0, _$E) of j from the header(s) included
// by HeaderStatics_empty_rect_init_00493740.cpp; no source of its own.


// Dynamic initializer (0x00493900, _$E) of k from the header(s) included
// by HeaderStatics_empty_rect_init_00493740.cpp; no source of its own.


// Dynamic initializer (0x00493910, _$E) of l from the header(s) included
// by HeaderStatics_empty_rect_init_00493740.cpp; no source of its own.


// bool CrateClass::Remove_It() -- 0x00493930
// Removes the crate this object tracks (if any) and stops its timer.

bool CrateClass::Remove_It()
{
    if (Is_Valid()) {
        Location.Unknown_00493C80();
        Make_Invalid();
        return true;
    }
    return false;
}


// bool CrateClass::Create_Crate(Cell const &cell) -- 0x004939A0
// Replaces the tracked crate with one placed at (or near) cell and starts its regeneration timer:
// a random time between 0.5 and 2 times Rules CrateRegen minutes.


inline double Random_Double(double a, double b)
{
    int num = Random_Pick(0, 0x7FFFFFFE);
    return (b - a) * (num * (1.0 / 2147483646.0)) + a;
}

bool CrateClass::Create_Crate(Cell const &cell)
{
    Remove_It();
    Cell loc = cell;
    if (Put_Crate(loc)) {
        unknown_tactical_83996c->Flag_Cell(&Map[cell]);
        Location = loc;
        Timer = int(Random_Double(Rules->CrateRegen * 450.0, Rules->CrateRegen * 1800.0));
        Timer.Start();
        return true;
    }
    return false;
}


// bool __fastcall CrateClass::Put_Crate(Cell &cell) -- 0x00493AD0
// Places a wood crate (a water crate on water) at cell when the cell is on the map and has no overlay,
// and redraws the area it covers.

bool __fastcall CrateClass::Put_Crate(Cell &cell)
{
    int old = unknown_a40774;
    unknown_a40774 = 0;
    if (!Map.Unknown_0055A660(cell, true)) {
        unknown_a40774 = old;
        return false;
    }
    CellClass *cellptr = &Map[cell];
    if (cellptr->OverlayIndex == -1) {
        if (Map[cell].unknown_ec == 2) {
            new OverlayClass(Rules->unknown_f8, cell);
        } else {
            new OverlayClass(Rules->unknown_f0, cell);
        }
        Rect rect = Union(cellptr->Overlay_Render_Rect(), cellptr->Overlay_Shadow_Render_Rect());
        Point2D pt(unknown_int_839630, unknown_int_839634);
        unknown_tactical_83996c->Register_Dirty_Area(Rect(rect.X - pt.X, rect.Y - pt.Y, rect.Width, rect.Height), false);
        unknown_a40774 = old;
        return true;
    }
    unknown_a40774 = old;
    return false;
}


// bool Cell::Unknown_00493C80() (0x00493C80)
// Removes the crate overlay (one of Rules' three crate images) from the cell,
// redraws the area it covered and clears the cell's overlay data.

bool Cell::Unknown_00493C80()
{
    if (Map.In_Radar(*this)) {
        CellClass &cell = Map[*this];
        if (cell.OverlayIndex != -1) {
            OverlayTypeClass *type = OverlayTypes[cell.OverlayIndex];
            if (type == Rules->unknown_f0 || type == Rules->unknown_f4 || type == Rules->unknown_f8) {
                Rect rect = Union(cell.Overlay_Render_Rect(), cell.Overlay_Shadow_Render_Rect());
                rect.Y -= unknown_int_839634;
                unknown_tactical_83996c->Register_Dirty_Area(rect, 0);
                cell.OverlayIndex = -1;
                cell.OverlayData = 0;
                return true;
            }
        }
    }
    return false;
}
