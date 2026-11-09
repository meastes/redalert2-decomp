// NON-EXACT DRAFT 0x49B0F0 FUN_0049b0f0
// entry 0x49B0F0; exact-match score 62.84 (misses_index best); plan "review: rewrite likely"
// origin lane: Scout926 (build/class-work/Scout926/w/mu2.cpp)
// Recovered source text by our lanes/workers; never exact-match credit. Tracked by
// Scout1069 (native/drafts.json). Not original bytes.
// void DisplayClass::unknown_slot46(Cell const &, bool, ObjectClass *, int, bool) (0x0049B0F0)
// The mouse over the map with no button held (TibSun DisplayClass::Mouse_Left_Up).
#include "shared/display.hpp"
#include "shared/map_cell.hpp"
#include "shared/object.hpp"
#include "shared/techno.hpp"
#include "shared/techno_type.hpp"
#include "shared/building_type_class.hpp"
#include "shared/house.hpp"
#include "shared/map_view.hpp"
#include "shared/convert.hpp"
#include "shared/hicolor.hpp"
#include "shared/unknown_geometry.hpp"

// @ref ?Unknown_006F6740@@YAHXZ=006f6740
// @ref ?Unknown_006F6740b@@YAHXZ=006f6740
// @ref ?Unknown_006F6730@@YAHXZ=006f6730

extern ConvertClass *unknown_drawer_8323c8;     // 0x008323C8
// Two mouse shapes chosen by functions (0x006F6740 returns 0x16, 0x006F6730 0x47).
int Unknown_006F6740();
int Unknown_006F6740b();
int Unknown_006F6730();

void DisplayClass::unknown_slot46(Cell const &cell, bool shadow, ObjectClass *object, int action, bool small_mouse)
{
    unknown_11cc = false;

    AbstractClass *target = 0;
    if (object != 0) {
        target = object;
    } else {
        if (cell != unknown_geometry_k) {
            target = (AbstractClass *)&Map[cell];
        }
    }
    if (object != 0) {
        TechnoClass *techno = object->As_Techno();
        if (techno != 0 && (techno->Kind_Of() != 6 || !((BuildingClass *)techno)->Type->InvisibleInGame) && !Debug_Map) {
            techno->unknown_380 = true;
        }
    }

    if (unknown_11af && unknown_11b8 != 0) {
        if (unknown_player_a35db4->unknown_func_4ed200(cell) == 0 && Map.Unknown_0055A660(cell, true)) {
            *unknown_11b8 = Cell_Coord(cell);
            unknown_11b8->Z = Map.Unknown_0055A280(*unknown_11b8) + ((Map[cell].unknown_140 & 0x100) ? unknown_geometry_j : 0);
        }
    }

    int waypoint_action = Unknown_0049EAD0(action, cell);

    if (WaypointColor == RGBClass(0, 0, 0)) {
        unsigned short color = ((unsigned short *)unknown_drawer_8323c8->Table)[1];
        WaypointColor = Unpack_Hicolor_Pixel(color);
    }

    switch (waypoint_action) {
    case 0x2A:
    case 0x2B:
    case 0x2F:
        Unknown_0049EB40(unknown_player_a35db4->unknown_154);
        break;

    case 0x2C:
    case 0x2D:
    case 0x2E:
    case 0x31:
    case 0x32:
    case 0x33: {
        int path = -1;
        char id;
        WaypointClass *waypoint = unknown_player_a35db4->unknown_func_4ed200(cell);
        unknown_player_a35db4->unknown_func_4ed2b0(waypoint, &path, &id);
        if (!unknown_11af) {
            unknown_player_a35db4->unknown_154 = path;
        }
        Unknown_0049EB40(path);
        break;
    }

    default: {
        unsigned short *table = (unsigned short *)unknown_drawer_8323c8->Table;
        table[1] = Build_Hicolor_Pixel(WaypointColor.Red, WaypointColor.Green, WaypointColor.Blue);
        Unknown_0049EB40(-1);
        if (!unknown_11af) {
            unknown_player_a35db4->unknown_154 = -1;
        }
        break;
    }
    }

    waypoint_action = Unknown_006D8F60(waypoint_action, (int)&cell, (int)object);

    if (shadow) {
        switch (waypoint_action) {
        case 0x1F:
        case 0x24:
            Set_Default_Mouse(0x1A, small_mouse);
            break;
        case 0x1A:
            Set_Default_Mouse(Unknown_006F6740(), small_mouse);
            break;
        case 10:
        case 0xF:
        case 0x20:
            Set_Default_Mouse(0x23, small_mouse);
            break;
        case 0x32:
            Set_Default_Mouse(0x3C, small_mouse);
        case 0x3C:
            Set_Default_Mouse(0x4E, small_mouse);
            break;
        case 2:
            if (unknown_objects_a40c60.Count() && unknown_objects_a40c60[0] != 0 && unknown_objects_a40c60[0]->AbstractFlags.IsTechno && unknown_objects_a40c60[0]->unknown_slot32()->MoveToShroud) {
                Set_Default_Mouse(0x12, small_mouse);
                break;
            }
            Set_Default_Mouse(0x13, small_mouse);
            break;
        case 1:
        case 5:
            Set_Default_Mouse(0x12, small_mouse);
            break;
        case 0xC:
        case 0xD:
        case 0xE:
            Set_Default_Mouse(0x20, small_mouse);
            break;
        case 0x11:
        case 0x1B:
        case 0x21:
        case 0x22:
        case 0x2A:
        case 0x2B:
        case 0x2C:
        case 0x2D:
        case 0x2E:
        case 0x2F:
        case 0x31:
        case 0x33:
            Set_Default_Mouse(0x3C, small_mouse);
            break;
        case 0x1E:
            Set_Default_Mouse(0x1C, small_mouse);
            break;
        case 0x39:
            Set_Default_Mouse(0x3B, small_mouse);
            break;
        case 0x3D:
            Set_Default_Mouse(0x11, small_mouse);
            break;
        case 0x14:
            Set_Default_Mouse(0x35, small_mouse);
            break;
        case 0x25:
            Set_Default_Mouse(0x39, small_mouse);
            break;
        case 0x26:
            Set_Default_Mouse(0x32, small_mouse);
            break;
        case 0x27:
        case 0x28:
            Set_Default_Mouse(0x3A, small_mouse);
            break;
        case 0x29:
        case 0x41:
            Set_Default_Mouse(0x2F, small_mouse);
            break;
        case 0x3E:
        case 0x3F:
            Set_Default_Mouse(Unknown_006F6730(), small_mouse);
            break;
        case 0:
        case 3:
        case 4:
        case 6:
        case 7:
        case 8:
        case 9:
        case 0xB:
        case 0x10:
        case 0x1C:
        case 0x1D:
        case 0x23:
        case 0x34:
        case 0x35:
        case 0x36:
        case 0x37:
        case 0x38:
        case 0x40:
            Set_Default_Mouse(0, small_mouse);
            break;
        default:
            Set_Default_Mouse(0, small_mouse);
            break;
        }
    } else {
        switch (waypoint_action) {
        case 0x11:
        case 0x1B:
        case 0x21:
        case 0x22:
        case 0x2A:
        case 0x2B:
        case 0x2C:
        case 0x2D:
        case 0x2E:
        case 0x2F:
        case 0x31:
        case 0x32:
        case 0x33:
            Set_Default_Mouse(0x3C, small_mouse);
            break;
        case 0x1E:
            Set_Default_Mouse(0x1C, small_mouse);
            break;
        case 0x39:
            Set_Default_Mouse(0x3B, small_mouse);
            break;
        case 0x1D:
            Set_Default_Mouse(0x21, small_mouse);
            break;
        case 7:
        case 8:
        case 0x3D:
            Set_Default_Mouse(0x11, small_mouse);
            break;
        case 0x1A:
            Set_Default_Mouse(Unknown_006F6740b(), small_mouse);
            break;
        case 1:
            Set_Default_Mouse(0x12, small_mouse);
            break;
        case 5:
            if (target != 0 && unknown_objects_a40c60.Count() == 1 && unknown_objects_a40c60[0] != 0 && unknown_objects_a40c60[0]->AbstractFlags.IsTechno && ((TechnoClass *)unknown_objects_a40c60[0])->unknown_slot213(target)) {
                Set_Default_Mouse(0x14, small_mouse);
                break;
            }
            // fall into the next case
        case 6:
            Set_Default_Mouse(0x15, small_mouse);
            break;
        case 3:
        case 9:
        case 0xB:
        case 0x23:
            Set_Default_Mouse(0x19, small_mouse);
            break;
        case 0xE:
            Set_Default_Mouse(0x20, small_mouse);
            break;
        case 4:
        case 0x34:
            Set_Default_Mouse(0x1B, small_mouse);
            break;
        case 0x35:
            Set_Default_Mouse(0x26, small_mouse);
            break;
        case 2:
        case 0x36:
            Set_Default_Mouse(0x13, small_mouse);
            break;
        case 0x10:
        case 0x37:
        case 0x38:
        case 0x40:
            Set_Default_Mouse(0x34, small_mouse);
            break;
        case 10:
            Set_Default_Mouse(0x22, small_mouse);
            break;
        case 0xD:
            Set_Default_Mouse(0x1F, small_mouse);
            break;
        case 0xC:
            Set_Default_Mouse(0x1E, small_mouse);
            break;
        case 0xF:
        case 0x20:
            Set_Default_Mouse(0x23, small_mouse);
            break;
        case 0x1F:
            Set_Default_Mouse(0x1A, small_mouse);
            break;
        case 0x14:
            Set_Default_Mouse(0x35, small_mouse);
            break;
        case 0x25:
            Set_Default_Mouse(0x39, small_mouse);
            break;
        case 0x26:
            Set_Default_Mouse(0x32, small_mouse);
            break;
        case 0x27:
        case 0x28:
            Set_Default_Mouse(0x3A, small_mouse);
            break;
        case 0x29:
        case 0x41:
            Set_Default_Mouse(0x2F, small_mouse);
            break;
        case 0x3C:
            Set_Default_Mouse(0x4E, small_mouse);
            break;
        case 0x3E:
        case 0x3F:
            Set_Default_Mouse(Unknown_006F6730(), small_mouse);
            break;
        case 0x1C:
            break;
        default:
            Set_Default_Mouse(0, small_mouse);
            break;
        }
    }
}
