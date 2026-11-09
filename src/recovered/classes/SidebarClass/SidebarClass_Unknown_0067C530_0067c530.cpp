// bool SidebarClass::Unknown_0067C530(int type, int id) (0x0067C530)
// Adds a buildable to its strip: enables the strip's tab, shows the sidebar
// and switches to the first filled strip when every strip was empty.
#include "shared/sidebar.hpp"

bool SidebarClass::Unknown_0067C530(int type, int id)
{
    if (!Debug_Map) {
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
