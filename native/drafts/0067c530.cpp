// NON-EXACT DRAFT 0x67C530 SidebarClass::Unknown_0067C530 (Add: a buildable to its strip) (788 B)
// entry 0x67C530; exact-match score 51.87 (lane best), this text 51.87; plan "draft, verify"
// origin lane: Scout624 (lane best 72.12 Scout998, text not kept) (build/class-work/Scout624/w/c530_0.cpp)
// Recovered source text by our lanes/workers; never exact-match credit. Tracked by
// Scout1103 (native/drafts.json, native M10). Not original bytes.
// bool SidebarClass::Unknown_0067C530(int type, int id) (0x0067C530)
// Adds a buildable to its strip: enables the strip's tab, shows the sidebar
// and switches to the first filled strip when every strip was empty.
#include "shared/sidebar.hpp"

bool SidebarClass::Unknown_0067C530(int type, int id)
{
    if (!Debug_Map) {   // NATIVE: Debug_Map (0x00A40D13; unknown_flag_a40d13 is its unused former name, its own storage natively)
        int first = Unknown_00681980();
        int column = Which_Column(type, id);
        if (Column[column].Unknown_0067EDF0(type, id)) {
            Unknown_0067C910(column);
            unknown_slot54(1);
            if (first == -1) {
                Select_Tab(Unknown_00681980());
            }
            unknown_4ee2 = true;
            Flag_To_Redraw(false);
            if (column == unknown_4ed8) {
                unknown_flag_abcd84 = true;
            }
            return true;
        }
    }
    return false;
}
