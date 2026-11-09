// Behaviour-written bodies for native M22 (Scout1177): interface leaves the
// coverage ranking found under reached callers. NOT EXACT: each follows its
// pass-6a decompile against the shared headers.
#define VECTOR_IMPLEMENT_VECTOR_ACCESSORS
#define VECTOR_IMPLEMENT_DYNAMIC_ACCESSORS
#include "shared/vector_class.hpp"
#include "shared/command.hpp"
#include "shared/mouse_class.hpp"
#include "shared/map_view.hpp"
#include "shared/object.hpp"
#include "shared/tab.hpp"
#include "shared/unknown_globals.hpp"
#include "shared/main_loop.hpp"
#include "shared/session_state.hpp"

// 0x006A17B0: flags the tab bar for a redraw (+0x4F15) and the whole map (slot
// 14, Flag_To_Redraw(false)). Reached by TActionClass::Execute.
void TabClass::Unknown_006A17B0()
{
    unknown_4f15 = true;
    Flag_To_Redraw(false);
}

// 0x006F64B0: outside a network game (+0x2B0 of the session, 0x00A3D548) the
// radar mode switch is reset (Map.Unknown_0049CAC0(-1)). Reached by TabClass::AI.
void Unknown_006F64B0()
{
    if (unknown_int_a3d548 == 0) {
        Map.Unknown_0049CAC0(-1);
    }
}

// 0x006818B0 (the sidebar tab key): in a multiplayer session the radar buttons
// select mode 8. Reached by Keyboard_Process.
void __fastcall Sidebar_Tab_Key_006818b0(SidebarClass *sidebar)
{
    (void)sidebar;
    if (unknown_session_a3d298.Type != 0) {
        unknown_int_a40d48 = 8;
    }
}

// 0x0040D120 (DisplayClass slot 43): does nothing.
void DisplayClass::unknown_slot43()
{
}

// 0x00681940: the sidebar column of a buildable kind (RTTI): infantry 2, units and
// aircraft 3, buildings 1 for kind 5 (defences) else 0, otherwise -1.
// Reached by HouseClass 0x004F3170.
int __fastcall SidebarClass::Which_Column(int kind, int arg3, bool naval)
{
    (void)naval;
    if (kind == 0x10) return 2;
    if (kind == 0x28 || kind == 3) return 3;
    if (kind == 7) return arg3 == 5;
    return -1;
}
