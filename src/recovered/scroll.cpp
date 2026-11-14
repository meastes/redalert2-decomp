// scroll.cpp: the original translation unit re-filed as one source (Route A): the accepted
// bodies of its canonical compile group in address order, under the unit's own header
// state (its switches and includes in their original order). The module map
// manifests/original_target/modules/scroll.json carries the addresses and the rows that
// stay in their own files.


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


// ScrollClass::ScrollClass() (0x006695E0)
// Clears the scroll state; +0x4F29 starts true.

ScrollClass::ScrollClass()
    : unknown_4f18(0), unknown_4f1c(false), unknown_4f20(0, 0), unknown_4f28(false),
      unknown_4f29(true), unknown_4f2a(false)
{
}


// void ScrollClass::AI(int &input, Point2D const &xy) (0x00669630, slot 10)
// Scrolls, then runs the tab bar's AI.

void ScrollClass::AI(int &input, Point2D const &xy)
{
    Scroll_AI();
    TabClass::AI(input, xy);
}


bool ScrollClass::Resolve_Point(Point2D const &xy,Cell &cell,CoordStruct &coord,ObjectClass *&object,bool &fog,bool &shadow)
{
    object=0;
    cell=unknown_geometry_k;
    coord=unknown_geometry_l;
    fog=false;
    shadow=false;
    if (xy.X<0 || xy.Y<0) return false;
    cell = unknown_tactical_83996c ? unknown_tactical_83996c->Pixel_To_Cell(xy+Point2D(unknown_rect_839630.X,unknown_rect_839630.Y)) : unknown_geometry_k;
    coord = unknown_tactical_83996c ? unknown_tactical_83996c->Pixel_To_Coord(xy+Point2D(unknown_rect_839630.X,unknown_rect_839630.Y)) : unknown_geometry_l;
    if (unknown_tactical_83996c && Map.Unknown_0054AB00(coord)) {
        UnknownCoord434820 adjusted = UnknownCoord434820(cell,Map.Get_Height_GL(UnknownCoord434820(cell)));
        shadow=Map.Is_Shrouded(adjusted) && unknown_b263d8!=0;
        fog=false;
        if (unknown_id_source_a3d290->Special().FogOfWar) {
            BuildingClass *building=static_cast<BuildingClass *>(Map[adjusted].Cell_Building());
            if (building) fog=building->unknown_5b7;
            else fog=Map.Is_Fogged(adjusted) && unknown_b263d8!=0;
        }
        if (!shadow && !fog) {
            object=unknown_tactical_83996c->Get_Selectable_Object_006aa990(xy);
            TechnoClass *techno=object->As_Techno();
            if (techno && !techno->unknown_372) {
                if ((techno->unknown_1b8==2 && !Map[techno->Center_Coord()].Is_Sensed(unknown_player_a35db4->ArrayIndex)) || techno->Techno_Type_Class()->Invisible) object=0;
            }
            if (object && object->Kind_Of()==6) {
                BuildingClass *building=static_cast<BuildingClass *>(object);
                if (!building->unknown_372 && ((building->unknown_5bd==15 && !Map[techno->Center_Coord()].Is_Sensed(unknown_player_a35db4->ArrayIndex)) || building->Type->InvisibleInGame)) object=0;
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

int ScrollClass::What_Action(Cell const &cell, ObjectClass *object, bool arg3)
{
    int action = 0;
    if (unknown_objects_a40c60.Count()) {
        if (object != 0) {
            action = Unknown_0051A690(0, object)->unknown_slot28(object, false);
        } else {
            action = Unknown_0051A690(&cell, 0)->unknown_slot27(&cell, arg3, false);
        }
    } else {
        bool visible = true;
        TechnoClass *techno = As_Techno_0040dbd0(object);
        if (techno != 0 && !techno->unknown_372) {
            if ((techno->unknown_1b8 == 2 && !Map[techno->Center_Coord()].Is_Sensed(unknown_player_a35db4->ArrayIndex))
                    || techno->Techno_Type_Class()->Invisible) {
                visible = false;
            }
        }

        if (object != 0 && object->Kind_Of() == 6 && !((BuildingClass *)object)->unknown_372
                && ((BuildingClass *)object)->unknown_5bd == 15
                && !Map[techno->Center_Coord()].Is_Sensed(unknown_player_a35db4->ArrayIndex)) {
            visible = false;
        }
        if (visible) {
            if (object != 0 && object->CanBeSelected()
                    && (object->Kind_Of() != 6 || !((BuildingClass *)object)->unknown_5b7)
                    && (techno == 0 || !techno->unknown_32c)) {
                action = 7;
            }
            if (unknown_object_84e8d0.FindBeaconAt_0042e860(CoordStruct(cell.X * 256 + 128, cell.Y * 256 + 128, Map[cell].unknown_11b * unknown_geometry_g))) {
                action = 0x3D;
            }
        }

        if (Map.unknown_11ac) {
            if (object != 0 && object->Owning_House() != 0 && object->Owning_House()->Is_Player_Control() && object->Can_Repair()) {
                action = 10;
            } else {
                action = 15;
            }
        }

        if (Map.unknown_11ae) {
            action = 0x22;
            if (object != 0 && object->Owning_House() != 0 && object->Owning_House()->Is_Player_Control()
                    && object->Kind_Of() == 6 && object->CanBeSelected() && !object->unknown_slot31()) {
                BuildingTypeClass *type = ((BuildingClass *)object)->Type;
                if (type->TogglePower && (type->PowerDrain_c6c > 0 || type->Powered)) {
                    action = 0x21;
                }
            }
        }

        WaypointClass *waypoint = unknown_player_a35db4->Waypoint_At(cell);
        if (Map.unknown_11af) {
            bool force = Scroll_Key_Down(KeyForceMove1) || Scroll_Key_Down(KeyForceMove2);
            int path = -1;
            char position;
            if (waypoint != 0) {
                unknown_player_a35db4->Fetch_Waypoint_Data(waypoint, &path, &position);
            }
            if (Map.unknown_11b8 != 0) {
                action = 0x30;
            } else if (force && waypoint != 0 && path == unknown_player_a35db4->unknown_154
                    && unknown_player_a35db4->Unknown_004F3120()
                    && unknown_player_a35db4->WaypointPaths[path]->Next_Waypoint(waypoint) != 0) {
                action = 0x2F;
            } else if (waypoint != 0) {
                action = 0x2E;
            } else if (unknown_player_a35db4->unknown_154 != -1 && unknown_player_a35db4->Unknown_004F3120()
                    && Map.Unknown_0055A660(cell, true)) {
                action = 0x2A;
            } else {
                action = 0x2B;
            }
        }

        if (Map.unknown_11b0) {
            action = 0x3C;
        }

        if (Map.unknown_11ad) {
            if (object != 0 && object->Owning_House() != 0 && object->Owning_House()->Is_Player_Control() && object->Can_Demolish()) {
                if (object->Kind_Of() == 6) {
                    action = object->unknown_slot31() ? 14 : 12;
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

        if (Map.unknown_11b4 != -1) {
            int super_action = SuperWeaponTypes[Map.unknown_11b4]->unknown_slot27(&cell, object);
            if (super_action != 0) {
                action = super_action;
            }
        }
        if (action == 0 && waypoint != 0) {
            action = 0x2C;
        }
        if (Map.unknown_11a4 != 0) {
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
    if (unknown_4f2a != true) {
        Point2D p = Point2D(unknown_rect_839630.X, unknown_rect_839630.Y) + xy;
        int x = p.X;
        int y = p.Y;
        int w = (unknown_surface_839964->Get_Width() + unknown_surface_839948->Get_Width()) - 1;
        int h = unknown_surface_839964->Get_Height() - 1;

        bool at_screen_edge = (y <= 0 || x <= 0 || x >= w || y >= h);

        bool player_scrolled = false;

        static UnknownFacingDir dir;
        static int direction;

        if (unknown_4f18 || at_screen_edge) {
            if (at_screen_edge) {
                player_scrolled = true;

                int width = unknown_8523e0;
                int height = unknown_8523e4;

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

            int rate = 8 - unknown_4f18;

            if (rate < Options.ScrollRate + 1) {
                rate = Options.ScrollRate + 1;
                unknown_4f18 = 8 - rate;
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
                    unknown_4f18++;
                }
            }
        }
        if (!player_scrolled) {
            if (!Counter) {
                unknown_4f18--;
                if (unknown_4f18 < 0) unknown_4f18++;
                Counter = 1;
            }
        }
    }
}


// The exit-time destructor of Edge_Scroll_00669EC0's function-local static `dir` (0x0066A280, `_$E95`):
// UnknownFacingDir has an empty destructor, so the body is a lone ret. The guarded static's
// initialisation registers it with atexit.
// No source of its own: emitted by this unit (Scroll.cpp) for the static in Edge_Scroll_00669EC0.


// void ScrollClass::Scroll_AI() (0x0066A290)

void ScrollClass::Scroll_AI()
{
    if (unknown_flag_a40d44) {
        return;
    }
    int x = unknown_rect_839630.X;
    int y = unknown_rect_839630.Y;
    Point2D mouse = WWMouse->Get_Mouse_XY();
    Point2D xy(mouse.X - x, mouse.Y - y);
    if (Planning_Mouse_Move_00617900(xy.X, xy.Y)) {
        return;
    }
    if (unknown_4f2a == true) {
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
        if (unknown_4f29 && !Debug_Map) {
            Edge_Scroll_00669EC0(xy);
        }
    }
}


// bool ScrollClass::Is_Scrolling() (0x0066A3C0, slot 26)
// True while a drag select runs, when 0x00A40D44 is set, or when the
// tactical view's +0xD8 value is not 0.

bool ScrollClass::Is_Scrolling()
{
    if (unknown_4f2a || unknown_flag_a40d44) {
        return true;
    }
    if (unknown_tactical_83996c != 0 && unknown_tactical_83996c->unknown_d8 != 0.0f) {
        return true;
    }
    return false;
}


// void ScrollClass::Message_Handler(HWND window, unsigned &message, unsigned &wparam, long &lparam) (0x0066A400)

void ScrollClass::Message_Handler(HWND window, unsigned &message, unsigned &wparam, long &lparam)
{
    if (!unknown_flag_a40380) {
        return;
    }
    if (!unknown_flag_a40d04 && !Debug_Map) {
        return;
    }
    if (!unknown_flag_a40958 || unknown_tactical_83996c == 0 || WWMouse == 0) {
        return;
    }
    if (unknown_flag_a40d44 && message != 0x215) {
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
        long position = lparam;
        xy.X = ((short *)&position)[0] - unknown_rect_839630.X;
        xy.Y = ((short *)&position)[1] - unknown_rect_839630.Y;
        if (!Resolve_Point(xy, cell, coord, object, arg5, arg6)) {
            break;
        }
        if (Planning_Left_Down_00617340(xy.X, xy.Y)) {
            break;
        }
        if (unknown_4f2a) {
            break;
        }
        Map.unknown_slot46(cell, arg6, object, What_Action(cell, object, true), false);
        Map.Mouse_Left_Press(xy);
        SetCapture(MainWindow);
        unknown_4f2a = true;
        break;
    }
    case 0x202: {
        long position = lparam;
        xy.X = ((short *)&position)[0] - unknown_rect_839630.X;
        xy.Y = ((short *)&position)[1] - unknown_rect_839630.Y;
        if (Planning_Left_Up_00617680(xy)) {
            break;
        }
        if (!unknown_4f2a) {
            break;
        }
        Resolve_Point(xy, cell, coord, object, arg5, arg6);
        Map.Mouse_Left_Release(coord, cell, object, What_Action(cell, object, false), false);
        unknown_4f2a = false;
        ReleaseCapture();
        break;
    }
    case 0x204: {
        long position = lparam;
        xy.X = ((short *)&position)[0] - unknown_rect_839630.X;
        xy.Y = ((short *)&position)[1] - unknown_rect_839630.Y;
        Planning_Mouse_Down(xy.X, xy.Y);
        if (unknown_4f2a) {
            break;
        }
        if (!Resolve_Point(xy, cell, coord, object, arg5, arg6)) {
            break;
        }
        Map.Mouse_Right_Press(xy);
        SetCapture(MainWindow);
        unknown_4f2a = true;
        break;
    }
    case 0x205: {
        long position = lparam;
        Point2D point(((short *)&position)[0] - unknown_rect_839630.X, ((short *)&position)[1] - unknown_rect_839630.Y);
        Planning_Mouse_Up(point.X, point.Y);
        if (!unknown_4f2a) {
            break;
        }
        Map.Mouse_Right_Release(xy);
        DisplayClass::Abort_Drag_Select();
        unknown_4f2a = false;
        ReleaseCapture();
        break;
    }
    case 0x215:
        if (lparam != (long)MainWindow && unknown_4f2a == true) {
            Abort_Drag_Select();
        }
        break;
    }
}


// void ScrollClass::Scroll_Coast(Point2D const &xy) (0x0066A7A0)

void ScrollClass::Scroll_Coast(Point2D const &xy)
{
    static int _direction = -1;
    // Unreferenced by the code (0x007F2C10, between the two statics the code uses).
    static int unknown_7f2c10[8] = { 1, 2, 3, 4, 5, 6, 7, 8 };
    static int _mouse[16] = {
        0x3D, 0x3E, 0x40, 0x3F, 0x42, 0x3D, 0x41, 0x3D, 0x44, 0x45, 0x3D, 0x3D, 0x43, 0x3D, 0x3D, 0x3D
    };

    if (!unknown_flag_a40958) {
        return;
    }
    if (!unknown_flag_a40380 && !Debug_Map) {
        return;
    }
    if (!unknown_4f28) {
        if (abs(xy.X - unknown_4f20.X) > GetSystemMetrics(0x44) * 2
                || abs(xy.Y - unknown_4f20.Y) > GetSystemMetrics(0x45) * 2) {
            unknown_4f28 = true;
        }
    }
    if (!unknown_4f28) {
        return;
    }
    if (!unknown_4f1c) {
        if (unknown_11cb) {
            Mouse_Right_Release(Point2D(-1, -1));
        } else {
            unknown_4f1c = true;
        }
    }
    if (unknown_4f1c) {
        int dx = xy.X - unknown_4f20.X;
        int dy = xy.Y - unknown_4f20.Y;
        int direction = Unknown_00721DB0(unknown_4f20, xy);
        if (direction != _direction) {
            _direction = direction;
        }
        if (dx == 0) {
            if (unknown_4f20.X == 0) {
                dx = -1;
            } else if (unknown_4f20.X >= unknown_8523e0 - 1) {
                dx = 1;
            }
        }
        if (dy == 0) {
            if (unknown_4f20.Y == 0) {
                dy = -1;
            } else if (unknown_4f20.Y >= unknown_8523e4) {
                dy = 1;
            }
        }
        if ((unknown_4f20.X < 10 && dx < 0) || (unknown_4f20.X > unknown_8523e0 - 10 && dx > 0)) {
            if (dx < 0 && dx > -5) {
                dx = -5;
            }
            if (dx > 0 && dx < 5) {
                dx = 5;
            }
            dx *= 4;
        }
        if ((unknown_4f20.Y < 10 && dy < 0) || (unknown_4f20.Y > unknown_8523e4 - 10 && dy > 0)) {
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
                SetCursorPos(point.x + unknown_rect_839630.X + unknown_4f20.X, point.y + unknown_rect_839630.Y + unknown_4f20.Y);
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
                SetCursorPos(point.x + unknown_rect_839630.X + unknown_4f20.X, point.y + unknown_rect_839630.Y + unknown_4f20.Y);
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


// void ScrollClass::Mouse_Right_Release(Point2D const &xy) (0x0066ABA0, slot 49)
// Ends a pending scroll by restoring the default mouse; otherwise the
// display's slot 49.

void ScrollClass::Mouse_Right_Release(Point2D const &xy)
{
    if (unknown_4f28) {
        Set_Default_Mouse(0, unknown_flag_83740c);
        unknown_4f28 = false;
        return;
    }
    DisplayClass::Mouse_Right_Release(xy);
}


// void ScrollClass::Mouse_Right_Press(Point2D const &point) (0x0066ABE0, slot 44)
// Unless 0x00A40D44 is set: the display's slot 44, remembering point, then
// shows the mouse.

void ScrollClass::Mouse_Right_Press(Point2D const &point)
{
    if (!unknown_flag_a40d44) {
        DisplayClass::Mouse_Right_Press(point);
        unknown_4f20.X = point.X;
        unknown_4f20.Y = point.Y;
        unknown_4f28 = false;
        WWMouse->Show_Mouse();
    }
}


// void ScrollClass::Abort_Drag_Select() (0x0066AC20, slot 35)
// Aborts a drag select: the display's slot 35, then releases the mouse
// capture when the main window holds it.

void ScrollClass::Abort_Drag_Select()
{
    DisplayClass::Abort_Drag_Select();
    unknown_4f2a = false;
    if (GetCapture() == MainWindow) {
        ReleaseCapture();
    }
    Debug_Printf("Scroll: Abort_Drag_Select()\n");
}
