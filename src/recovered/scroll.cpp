/*******************************************************************************
 *                                O P E N  T S
 *******************************************************************************
 * SPDX-License-Identifier: GPL-3.0-or-later
 * Copyright 2025 Electronic Arts Inc.
 * Copyright 2026 OpenTS contributors
 *
 * Contains material derived from Electronic Arts source code.
 * Modified by OpenTS contributors, 2026.
 * EA's GPLv3 Section 7 additional terms and supplemental warranty
 * disclaimers apply; see LICENSE.md.
 ******************************************************************************/
// Function comment blocks from mischa85/RedAlert2 392e904 src/scroll.cpp (matching RA2 1.006 decomp), placed above our
// own notes; code unchanged.
// Parameter names from mischa85/RedAlert2 392e904 src/scroll.cpp where both define the function (names only).

// scroll.cpp: the original translation unit re-filed as one source (Route A): the accepted
// bodies of its canonical compile group in address order, under the unit's own header
// state (its switches and includes in their original order). The module map
// manifests/original_target/modules/scroll.json carries the addresses and the rows that
// stay in their own files.

#define RA2_COORD_FROM_CELL_INLINE
#define RA2_TACTICAL_PICK_OBJECT
// Header-static dynamic initializers of the unit at 0x006693A0: the shape_file.hpp empty rect and the
// shared/unknown_geometry.hpp constants (0x006693A0-0x00669581); the other
// drafts of this plan are emitted by these includes.
#include "shared/shape_file.hpp"
#include "shared/unknown_geometry.hpp"


// Dynamic initializer (0x006693C0, _$E) of diagonal from the header(s) included
// by HeaderStatics_empty_rect_init_006693a0.cpp; no source of its own.


// Dynamic initializer (0x006693F0, _$E) of b from the header(s) included
// by HeaderStatics_empty_rect_init_006693a0.cpp; no source of its own.


// Dynamic initializer (0x00669410, _$E) of c from the header(s) included
// by HeaderStatics_empty_rect_init_006693a0.cpp; no source of its own.


// Dynamic initializer (0x00669430, _$E) of angle45 from the header(s) included
// by HeaderStatics_empty_rect_init_006693a0.cpp; no source of its own.


// Dynamic initializer (0x00669450, _$E) of angle60 from the header(s) included
// by HeaderStatics_empty_rect_init_006693a0.cpp; no source of its own.


// Dynamic initializer (0x00669470, _$E) of angle90 from the header(s) included
// by HeaderStatics_empty_rect_init_006693a0.cpp; no source of its own.


// Dynamic initializer (0x00669490, _$E) of g from the header(s) included
// by HeaderStatics_empty_rect_init_006693a0.cpp; no source of its own.


// Dynamic initializer (0x006694D0, _$E) of h from the header(s) included
// by HeaderStatics_empty_rect_init_006693a0.cpp; no source of its own.


// Dynamic initializer (0x00669500, _$E) of i from the header(s) included
// by HeaderStatics_empty_rect_init_006693a0.cpp; no source of its own.


// Dynamic initializer (0x00669530, _$E) of j from the header(s) included
// by HeaderStatics_empty_rect_init_006693a0.cpp; no source of its own.


// Dynamic initializer (0x00669560, _$E) of k from the header(s) included
// by HeaderStatics_empty_rect_init_006693a0.cpp; no source of its own.


// Dynamic initializer (0x00669570, _$E) of l from the header(s) included
// by HeaderStatics_empty_rect_init_006693a0.cpp; no source of its own.




#include "shared/scroll.hpp"
#include "shared/mouse_class.hpp"
#include "shared/map_view.hpp"
#include "shared/cell_class.hpp"
#include "shared/building_class.hpp"
#include "shared/building_type_class.hpp"
#include "shared/techno.hpp"
#include "shared/techno_type.hpp"
#include "shared/house.hpp"
#include "shared/id_source.hpp"
#include "shared/special.hpp"
#include "shared/tactical.hpp"
#include "shared/cell.hpp"
#include "shared/overlay_type.hpp"
#include "shared/super_weapon_type.hpp"
#include "shared/waypoint_path.hpp"
#include "shared/keyboard.hpp"
#include "shared/unknown_geometry.hpp"
#include "shared/display.hpp"
#include "shared/surface.hpp"
#include "shared/fast_math.hpp"
#include "shared/facing.hpp"
#include "shared/options.hpp"
#include "shared/rules.hpp"
#include "shared/mouse.hpp"
#include "shared/planning_mouse.hpp"
#include "shared/particle.hpp"
#include "shared/crt_memory.hpp"


// Dynamic initializer of ScrollClass::Counter (0x00669590, _$E) and its
// guarded atexit destructor (0x006695C0; guard byte 0x00AB7038), emitted by
// this definition (the timer's destructor is empty).

CDTimerClass<SystemTimerClass> ScrollClass::Counter;


// Static-member initializer or guarded atexit destructor (0x006695C0, _$E) emitted by the definition in ScrollClass_Counter_init_00669590.cpp; no source of its own.


/***********************************************************************************************
 * ScrollClass::ScrollClass -- Constructor for the scroll class object.                        *
 *                                                                                             *
 *    This is the constructor for the scroll class object.                                     *
 *                                                                                             *
 * INPUT:   none                                                                               *
 *                                                                                             *
 * OUTPUT:  none                                                                               *
 *                                                                                             *
 * WARNINGS:   none                                                                            *
 *                                                                                             *
 * HISTORY:                                                                                    *
 *   08/10/1995 JLB : Created.                                                                 *
 *=============================================================================================*/
// (0x006695E0)
// Clears the scroll state; +0x4F29 starts true.

ScrollClass::ScrollClass()
    : Inertia(0), IsCoastScrollAllowed(false), RightPressPoint(0, 0), IsDragOperation(false),
      IsEdgeScrollAllowed(true), IsMouseDown(false)
{
}


// void ScrollClass::AI(int &input, Point2D const &xy) (0x00669630, slot 10)
// Scrolls, then runs the tab bar's AI.

void ScrollClass::AI(int &input, Point2D const &xy)
{
    Scroll_AI();
    TabClass::AI(input, xy);
}


/// <summary>
/// Determines what lies underneath a point on the tactical view.
/// This routine is used by the mouse handling before it can decide on an action. Enemy
/// objects the player is not meant to see -- cloaked units and invisible types -- are not
/// reported, and neither is anything hidden behind shroud or fog.
/// </summary>
/// <param name="point">The mouse position, relative to the tactical view.</param>
/// <param name="cell">Receives the cell that lies under the point.</param>
/// <param name="coord">Receives the coordinate that lies under the point.</param>
/// <param name="object">Receives the object under the point, or NULL if there is none.</param>
/// <param name="fog">Receives whether the spot is covered by fog of war.</param>
/// <param name="shadow">Receives whether the spot is covered by shroud.</param>
/// <returns>bool; Does the point lie over the map?</returns>
bool ScrollClass::Resolve_Point(Point2D const &point,Cell &cell,CoordStruct &coord,ObjectClass *&object,bool &fog,bool &shadow)
{
    object=0;
    cell=unknown_geometry_k;
    coord=unknown_geometry_l;
    fog=false;
    shadow=false;
    if (point.X<0 || point.Y<0) return false;
    cell = TacticalMap ? TacticalMap->Pixel_To_Cell(point+Point2D(TacticalRect.X,TacticalRect.Y)) : unknown_geometry_k;
    coord = TacticalMap ? TacticalMap->Pixel_To_Coord(point+Point2D(TacticalRect.X,TacticalRect.Y)) : unknown_geometry_l;
    if (TacticalMap && Map.Unknown_0054AB00(coord)) {
        UnknownCoord434820 adjusted = UnknownCoord434820(cell,Map.Get_Height_GL(UnknownCoord434820(cell)));
        shadow=Map.Is_Shrouded(adjusted) && unknown_b263d8!=0;
        fog=false;
        if (Scen->Special().FogOfWar) {
            BuildingClass *building=static_cast<BuildingClass *>(Map[adjusted].Cell_Building());
            if (building) fog=building->IsFogged;
            else fog=Map.Is_Fogged(adjusted) && unknown_b263d8!=0;
        }
        if (!shadow && !fog) {
            object=TacticalMap->Get_Selectable_Object_006aa990(point);
            TechnoClass *techno=object->As_Techno();
            if (techno && !techno->unknown_372) {
                if ((techno->Cloak==2 && !Map[techno->Center_Coord()].Is_Sensed(PlayerPtr->ArrayIndex)) || techno->Techno_Type_Class()->Invisible) object=0;
            }
            if (object && object->Kind_Of()==6) {
                BuildingClass *building=static_cast<BuildingClass *>(object);
                if (!building->unknown_372 && ((building->TranslucencyLevel==15 && !Map[techno->Center_Coord()].Is_Sensed(PlayerPtr->ArrayIndex)) || building->Type->InvisibleInGame)) object=0;
            }
        }
        return true;
    }
    return false;
}


// int ScrollClass::What_Action(Cell const &cell, ObjectClass *object, bool arg3) (0x00669960)
// The action a left click would take on the object or cell under the mouse (TibSun What_Action):
// the selected object's answer, else the select/beacon/repair/sell/power/waypoint/super/pending modes.
// The modifier globals are ints (pushed as dwords); WWKeyboardClass::Down takes a KeyNumType.
inline bool Scroll_Key_Down(int key)
{
    return WWKeyboard->Down(key);
}

/// <summary>
/// Determines what a left click at this spot would do.
/// This routine decides both the shape of the mouse cursor and what the click will actually
/// perform. When something is selected, the answer comes from the selected object. Otherwise
/// it reflects whichever special mode the map has been put into -- repair, sell, power,
/// waypoint or super weapon targeting -- and falls back to plain selection.
/// </summary>
/// <param name="cell">The cell the mouse is currently over.</param>
/// <param name="object">The object the mouse is over, or NULL if there is none.</param>
/// <param name="check_fog">Should the action be limited by what the fog of war hides?</param>
/// <returns>Returns with the action that a left click would perform.</returns>
int ScrollClass::What_Action(Cell const &cell, ObjectClass *object, bool check_fog)
{
    int action = 0;
    if (CurrentObject.Count()) {
        if (object != 0) {
            action = Best_Selected_Object(0, object)->unknown_slot28(object, false);
        } else {
            action = Best_Selected_Object(&cell, 0)->What_Action(&cell, check_fog, false);
        }
    } else {
        bool visible = true;
        TechnoClass *techno = As_Techno_0040dbd0(object);
        if (techno != 0 && !techno->unknown_372) {
            if ((techno->Cloak == 2 && !Map[techno->Center_Coord()].Is_Sensed(PlayerPtr->ArrayIndex))
                    || techno->Techno_Type_Class()->Invisible) {
                visible = false;
            }
        }

        if (object != 0 && object->Kind_Of() == 6 && !((BuildingClass *)object)->unknown_372
                && ((BuildingClass *)object)->TranslucencyLevel == 15
                && !Map[techno->Center_Coord()].Is_Sensed(PlayerPtr->ArrayIndex)) {
            visible = false;
        }
        if (visible) {
            if (object != 0 && object->CanBeSelected()
                    && (object->Kind_Of() != 6 || !((BuildingClass *)object)->IsFogged)
                    && (techno == 0 || !techno->unknown_32c)) {
                action = 7;
            }
            if (BeaconManager.FindBeaconAt_0042e860(CoordStruct(cell.X * 256 + 128, cell.Y * 256 + 128, Map[cell].Height * unknown_geometry_g))) {
                action = 0x3D;
            }
        }

        if (Map.IsRepairMode) {
            if (object != 0 && object->Owning_House() != 0 && object->Owning_House()->Is_Player_Control() && object->Can_Repair()) {
                action = 10;
            } else {
                action = 15;
            }
        }

        if (Map.IsPowerMode) {
            action = 0x22;
            if (object != 0 && object->Owning_House() != 0 && object->Owning_House()->Is_Player_Control()
                    && object->Kind_Of() == 6 && object->CanBeSelected() && !object->Considered_Vehicle()) {
                BuildingTypeClass *type = ((BuildingClass *)object)->Type;
                if (type->TogglePower && (type->PowerDrain_c6c > 0 || type->Powered)) {
                    action = 0x21;
                }
            }
        }

        WaypointClass *waypoint = PlayerPtr->Waypoint_At(cell);
        if (Map.IsWaypointMode) {
            bool force = Scroll_Key_Down(KeyForceMove1) || Scroll_Key_Down(KeyForceMove2);
            int path = -1;
            char position;
            if (waypoint != 0) {
                PlayerPtr->Fetch_Waypoint_Data(waypoint, &path, &position);
            }
            if (Map.DraggedWaypoint != 0) {
                action = 0x30;
            } else if (force && waypoint != 0 && path == PlayerPtr->SelectedPath
                    && PlayerPtr->Can_Add_Waypoint_To_Path()
                    && PlayerPtr->WaypointPaths[path]->Next_Waypoint(waypoint) != 0) {
                action = 0x2F;
            } else if (waypoint != 0) {
                action = 0x2E;
            } else if (PlayerPtr->SelectedPath != -1 && PlayerPtr->Can_Add_Waypoint_To_Path()
                    && Map.In_Local_Radar(cell, true)) {
                action = 0x2A;
            } else {
                action = 0x2B;
            }
        }

        if (Map.IsBeaconMode) {
            action = 0x3C;
        }

        if (Map.IsSellMode) {
            if (object != 0 && object->Owning_House() != 0 && object->Owning_House()->Is_Player_Control() && object->Can_Demolish()) {
                if (object->Kind_Of() == 6) {
                    action = object->Considered_Vehicle() ? 14 : 12;
                } else {
                    action = 13;
                }
            } else {
                CoordStruct coord(cell.X * 256 + 128, cell.Y * 256 + 128, 0);
                coord.Z = Map.Get_Height_GL(coord);
                if (!Map.Is_Shrouded(coord) && !Map.Is_Fogged(coord)
                        && Map[cell].OverlayIndex != -1 && OverlayTypes[Map[cell].OverlayIndex]->Wall
                        && Map[cell].unknown_50 != -1 && Houses[Map[cell].unknown_50]->Is_Player_Control()) {
                    action = 12;
                } else {
                    action = 14;
                }
            }
        }

        if (Map.IsTargettingMode != -1) {
            int super_action = SuperWeaponTypes[Map.IsTargettingMode]->unknown_slot27(&cell, object);
            if (super_action != 0) {
                action = super_action;
            }
        }
        if (action == 0 && waypoint != 0) {
            action = 0x2C;
        }
        if (Map.PendingObject != 0) {
            action = 0;
        }
    }
    return action;
}


// SPDX-License-Identifier: GPL-3.0-or-later
// Copyright 2026 ra2-decomp contributors
// Copyright 2025 Electronic Arts Inc.; 2026 OpenTS contributors
// Derived from Electronic Arts source code; EA's GPL Section 7 terms apply (COPYING.md).
// void ScrollClass::Edge_Scroll_00669EC0(Point2D const &xy) (0x00669EC0): scrolls the view when the mouse
// is at a screen edge (or with inertia), speeding up with the right button. From ra2-decomp
// ScrollClass::Scroll_Edge (YR 0x00692B60), adapted to our names.

// Westwood's Bound: original clipped to minval..maxval.
static inline int Scroll_Bound(int original, int minval, int maxval)
{
    if (original < minval) return minval;
    if (original > maxval) return maxval;
    return original;
}

void ScrollClass::Edge_Scroll_00669EC0(Point2D const &xy)
{
    if (IsMouseDown != true) {
        Point2D p = Point2D(TacticalRect.X, TacticalRect.Y) + xy;
        int x = p.X;
        int y = p.Y;
        int w = (CompositeSurface->Get_Width() + SidebarSurface->Get_Width()) - 1;
        int h = CompositeSurface->Get_Height() - 1;

        bool at_screen_edge = (y <= 0 || x <= 0 || x >= w || y >= h);

        bool player_scrolled = false;

        static UnknownFacingDir dir;
        static int direction;

        if (Inertia || at_screen_edge) {
            if (at_screen_edge) {
                player_scrolled = true;

                int width = VideoModeWidth;
                int height = VideoModeHeight;

                static double edge_x = 0.16;
                static double edge_y = 0.21;

                int altx = x;
                if (altx < width * edge_x) {
                    altx = 0;
                } else if (altx > width * (1.0 - edge_x)) {
                    altx = width - 1;
                } else {
                    altx = width / 2;
                }

                int alty = y;
                if (alty < height * edge_y) {
                    alty = 0;
                } else if (alty > height * (1.0 - edge_y)) {
                    alty = height - 1;
                } else {
                    alty = height / 2;
                }

                Point2D center(unknown_int_839648 / 2, unknown_int_83964c / 2);
                dir = UnknownFacingDir((unsigned short)((Fast_Atan2((double)center.Y - alty, (double)altx - center.X) - 1.5707963267948966) * -10430.060040584269));
                direction = Dir_To_256(dir);
            }

            int control = Dir_To_8(UnknownFacingDir((unsigned short)(direction << 8)));

            static int _rate[9] = {448, 384, 320, 256, 192, 128, 64, 32, 16};

            int rate = 8 - Inertia;

            if (rate < Options.ScrollRate + 1) {
                rate = Options.ScrollRate + 1;
                Inertia = 8 - rate;
            }

            if (WWKeyboard->Down((KeyNumType)2)) {
                rate = Scroll_Bound(rate + 1, 4, 8);
            }

            int facing = Dir_To_8(UnknownFacingDir((unsigned short)(direction << 8)));

            int distance = 1;

            if (!Scroll_Map(facing, &distance, false)) {
                Override_Mouse_Shape(control + 9, false);
            } else {
                Override_Mouse_Shape(control + 1, false);

                distance = (int)(_rate[rate] * Rules->ScrollMultiplier);
                Scroll_Map(facing, &distance, true);

                if (Counter == 0 && player_scrolled) {
                    Counter = 1;
                    Inertia++;
                }
            }
        }
        if (!player_scrolled) {
            if (!Counter) {
                Inertia--;
                if (Inertia < 0) Inertia++;
                Counter = 1;
            }
        }
    }
}


// The exit-time destructor of Edge_Scroll_00669EC0's function-local static `dir` (0x0066A280, `_$E95`):
// UnknownFacingDir has an empty destructor, so the body is a lone ret. The guarded static's
// initialisation registers it with atexit.
// No source of its own: emitted by this unit (Scroll.cpp) for the static in Edge_Scroll_00669EC0.


/// <summary>
/// Handles the per frame mouse tracking for the tactical map.
/// This routine is called from the scroll AI every game frame. It feeds the current mouse
/// position to the map as a held drag, a coast scroll, or a plain hover that keeps the action
/// cursor up to date, and lets the map scroll when the cursor rests against a screen edge.
/// </summary>
// (0x0066A290)

void ScrollClass::Scroll_AI()
{
    if (IgnoreInput) {
        return;
    }
    int x = TacticalRect.X;
    int y = TacticalRect.Y;
    Point2D mouse = WWMouse->Get_Mouse_XY();
    Point2D xy(mouse.X - x, mouse.Y - y);
    if (Planning_Mouse_Move_00617900(xy.X, xy.Y)) {
        return;
    }
    if (IsMouseDown == true) {
        if (WWKeyboard->Down(1)) {
            Map.Mouse_Left_Held(xy);
        } else if (WWKeyboard->Down(2)) {
            Map.Scroll_Coast(xy);
        }
    } else {
        Cell cell;
        CoordStruct coord;
        ObjectClass *object;
        bool arg5;
        bool arg6;
        if (Resolve_Point(xy, cell, coord, object, arg5, arg6)) {
            Map.unknown_slot46(cell, arg6, object, What_Action(cell, object, true), false);
        }
        if (IsEdgeScrollAllowed && !Debug_Map) {
            Edge_Scroll_00669EC0(xy);
        }
    }
}


/// <summary>
/// Is the tactical map currently in motion?
/// This routine is used to hold off work that would fight with a scroll already under way. A
/// drag operation counts as scrolling just as much as actual map movement does.
/// </summary>
/// <returns>bool; Is the map moving or being dragged about?</returns>
// (0x0066A3C0, slot 26)
// True while a drag select runs, when 0x00A40D44 is set, or when the
// tactical view's +0xD8 value is not 0.

bool ScrollClass::Is_Scrolling()
{
    if (IsMouseDown || IgnoreInput) {
        return true;
    }
    if (TacticalMap != 0 && TacticalMap->MoveSpeed != 0.0f) {
        return true;
    }
    return false;
}


/// <summary>
/// Handles the mouse messages that drive the tactical map.
/// This routine is called from the main window procedure. It turns raw button presses and
/// releases into tactical map actions, and takes and releases the mouse capture so that a
/// drag survives the cursor leaving the window. Messages arriving while the game is not
/// running, or while input is being ignored, are quietly dropped.
/// </summary>
// (0x0066A400)

void ScrollClass::Message_Handler(HWND hwnd, unsigned &message, unsigned &wParam, long &lParam)
{
    if (!TacticalActive) {
        return;
    }
    if (!ScenarioActive && !Debug_Map) {
        return;
    }
    if (!GameActive || TacticalMap == 0 || WWMouse == 0) {
        return;
    }
    if (IgnoreInput && message != 0x215) {
        return;
    }
    Point2D xy;
    Cell cell;
    CoordStruct coord;
    ObjectClass *object;
    bool arg5;
    bool arg6;
    switch (message) {
    case 0x201: {
        long position = lParam;
        xy.X = ((short *)&position)[0] - TacticalRect.X;
        xy.Y = ((short *)&position)[1] - TacticalRect.Y;
        if (!Resolve_Point(xy, cell, coord, object, arg5, arg6)) {
            break;
        }
        if (Planning_Left_Down_00617340(xy.X, xy.Y)) {
            break;
        }
        if (IsMouseDown) {
            break;
        }
        Map.unknown_slot46(cell, arg6, object, What_Action(cell, object, true), false);
        Map.Mouse_Left_Press(xy);
        SetCapture(MainWindow);
        IsMouseDown = true;
        break;
    }
    case 0x202: {
        long position = lParam;
        xy.X = ((short *)&position)[0] - TacticalRect.X;
        xy.Y = ((short *)&position)[1] - TacticalRect.Y;
        if (Planning_Left_Up_00617680(xy)) {
            break;
        }
        if (!IsMouseDown) {
            break;
        }
        Resolve_Point(xy, cell, coord, object, arg5, arg6);
        Map.Mouse_Left_Release(coord, cell, object, What_Action(cell, object, false), false);
        IsMouseDown = false;
        ReleaseCapture();
        break;
    }
    case 0x204: {
        long position = lParam;
        xy.X = ((short *)&position)[0] - TacticalRect.X;
        xy.Y = ((short *)&position)[1] - TacticalRect.Y;
        Planning_Mouse_Down(xy.X, xy.Y);
        if (IsMouseDown) {
            break;
        }
        if (!Resolve_Point(xy, cell, coord, object, arg5, arg6)) {
            break;
        }
        Map.Mouse_Right_Press(xy);
        SetCapture(MainWindow);
        IsMouseDown = true;
        break;
    }
    case 0x205: {
        long position = lParam;
        Point2D point(((short *)&position)[0] - TacticalRect.X, ((short *)&position)[1] - TacticalRect.Y);
        Planning_Mouse_Up(point.X, point.Y);
        if (!IsMouseDown) {
            break;
        }
        Map.Mouse_Right_Release(xy);
        DisplayClass::Abort_Drag_Select();
        IsMouseDown = false;
        ReleaseCapture();
        break;
    }
    case 0x215:
        if (lParam != (long)MainWindow && IsMouseDown == true) {
            Abort_Drag_Select();
        }
        break;
    }
}


/// <summary>
/// Handles coast scrolling while the right mouse button is dragged.
/// Once the mouse has travelled far enough from the press point to count as a drag rather
/// than a click, the map scrolls away from that point at a speed set by how far the mouse has
/// been pulled and by the player's scroll method and rate options. The cursor is updated to
/// show which directions the map can still travel in. Starting a coast scroll cancels any
/// rubber band selection that was under way.
/// </summary>
/// <param name="point">The current mouse position, relative to the tactical view.</param>
// (0x0066A7A0)

void ScrollClass::Scroll_Coast(Point2D const &xy)
{
    static int _direction = -1;
    // Unreferenced by the code (0x007F2C10, between the two statics the code uses).
    static int unknown_7f2c10[8] = { 1, 2, 3, 4, 5, 6, 7, 8 };
    static int _mouse[16] = {
        0x3D, 0x3E, 0x40, 0x3F, 0x42, 0x3D, 0x41, 0x3D, 0x44, 0x45, 0x3D, 0x3D, 0x43, 0x3D, 0x3D, 0x3D
    };

    if (!GameActive) {
        return;
    }
    if (!TacticalActive && !Debug_Map) {
        return;
    }
    if (!IsDragOperation) {
        if (abs(xy.X - RightPressPoint.X) > GetSystemMetrics(0x44) * 2
                || abs(xy.Y - RightPressPoint.Y) > GetSystemMetrics(0x45) * 2) {
            IsDragOperation = true;
        }
    }
    if (!IsDragOperation) {
        return;
    }
    if (!IsCoastScrollAllowed) {
        if (IsRubberBand) {
            Mouse_Right_Release(Point2D(-1, -1));
        } else {
            IsCoastScrollAllowed = true;
        }
    }
    if (IsCoastScrollAllowed) {
        int dx = xy.X - RightPressPoint.X;
        int dy = xy.Y - RightPressPoint.Y;
        int direction = Facing_Between_Points(RightPressPoint, xy);
        if (direction != _direction) {
            _direction = direction;
        }
        if (dx == 0) {
            if (RightPressPoint.X == 0) {
                dx = -1;
            } else if (RightPressPoint.X >= VideoModeWidth - 1) {
                dx = 1;
            }
        }
        if (dy == 0) {
            if (RightPressPoint.Y == 0) {
                dy = -1;
            } else if (RightPressPoint.Y >= VideoModeHeight) {
                dy = 1;
            }
        }
        if ((RightPressPoint.X < 10 && dx < 0) || (RightPressPoint.X > VideoModeWidth - 10 && dx > 0)) {
            if (dx < 0 && dx > -5) {
                dx = -5;
            }
            if (dx > 0 && dx < 5) {
                dx = 5;
            }
            dx *= 4;
        }
        if ((RightPressPoint.Y < 10 && dy < 0) || (RightPressPoint.Y > VideoModeHeight - 10 && dy > 0)) {
            if (dy < 0 && dy > -5) {
                dy = -5;
            }
            if (dy > 0 && dy < 5) {
                dy = 5;
            }
            dy *= 4;
        }

        int distance_x;
        int distance_y;
        switch (Options.Get_Scroll_Method()) {
        case 0: {
            double rate = 1.0 / (Options.ScrollRate + 1);
            distance_x = abs((int)(dx * rate));
            distance_y = abs((int)(dy * rate));
            break;
        }
        case 1: {
            double rate = 12.0 / (Options.ScrollRate + 1);
            distance_x = abs((int)(dx * rate));
            distance_y = abs((int)(dy * rate));
            if (distance_x + distance_y > 0) {
                POINT point;
                point.x = 0;
                point.y = 0;
                ClientToScreen(MainWindow, &point);
                SetCursorPos(point.x + TacticalRect.X + RightPressPoint.X, point.y + TacticalRect.Y + RightPressPoint.Y);
            }
            break;
        }
        case 2: {
            dx = -dx;
            dy = -dy;
            double rate = 12.0 / (Options.ScrollRate + 1);
            distance_x = abs((int)(dx * rate));
            distance_y = abs((int)(dy * rate));
            if (distance_x + distance_y > 0) {
                POINT point;
                point.x = 0;
                point.y = 0;
                ClientToScreen(MainWindow, &point);
                SetCursorPos(point.x + TacticalRect.X + RightPressPoint.X, point.y + TacticalRect.Y + RightPressPoint.Y);
            }
            break;
        }
        }

        int blocked = 0;
        for (int facing = 0; facing < 8; facing += 2) {
            int one = 1;
            if (!Map.Scroll_Map(facing, &one, false)) {
                blocked |= 1 << (facing / 2);
            }
        }
        if (dx > 0) {
            Map.Scroll_Map(2, &distance_x, true);
        } else if (dx < 0) {
            Map.Scroll_Map(6, &distance_x, true);
        }
        if (dy > 0) {
            Map.Scroll_Map(4, &distance_y, true);
        } else if (dy < 0) {
            Map.Scroll_Map(0, &distance_y, true);
        }
        Set_Default_Mouse(_mouse[blocked], true);
    }
}


/// <summary>
/// Handles the right mouse button being released over the tactical map.
/// A release that ends a coast scroll drag merely restores the normal cursor. Otherwise the
/// base class gets the release and treats it as the ordinary deselect click.
/// </summary>
/// <param name="point">The mouse position, relative to the tactical view.</param>
// (0x0066ABA0, slot 49)
// Ends a pending scroll by restoring the default mouse; otherwise the
// display's slot 49.

void ScrollClass::Mouse_Right_Release(Point2D const &point)
{
    if (IsDragOperation) {
        Set_Default_Mouse(0, unknown_flag_83740c);
        IsDragOperation = false;
        return;
    }
    DisplayClass::Mouse_Right_Release(point);
}


/// <summary>
/// Handles the right mouse button being pressed over the tactical map.
/// The press point is remembered so that Scroll_Coast can measure any subsequent drag against
/// it. The mouse is shown again in case something had hidden it.
/// </summary>
/// <param name="point">The mouse position, relative to the tactical view.</param>
// (0x0066ABE0, slot 44)
// Unless 0x00A40D44 is set: the display's slot 44, remembering point, then
// shows the mouse.

void ScrollClass::Mouse_Right_Press(Point2D const &point)
{
    if (!IgnoreInput) {
        DisplayClass::Mouse_Right_Press(point);
        RightPressPoint.X = point.X;
        RightPressPoint.Y = point.Y;
        IsDragOperation = false;
        WWMouse->Show_Mouse();
    }
}


/// <summary>
/// Aborts any drag operation currently in progress.
/// Besides letting the base class abandon the rubber band, this routine clears the mouse down
/// latch and hands back the mouse capture, so a drag interrupted by another window does not
/// leave the tactical map convinced the button is still held.
/// </summary>
// (0x0066AC20, slot 35)
// Aborts a drag select: the display's slot 35, then releases the mouse
// capture when the main window holds it.

void ScrollClass::Abort_Drag_Select()
{
    DisplayClass::Abort_Drag_Select();
    IsMouseDown = false;
    if (GetCapture() == MainWindow) {
        ReleaseCapture();
    }
    Debug_Printf("Scroll: Abort_Drag_Select()\n");
}
