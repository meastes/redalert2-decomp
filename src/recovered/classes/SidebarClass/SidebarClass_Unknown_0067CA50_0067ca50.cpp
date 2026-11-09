// void SidebarClass::Unknown_0067CA50(int index) (0x0067CA50)
// A strip changed: refreshes the scroll buttons while it still has buildables; otherwise
// disables its tab and switches to the first filled strip; with none left it refreshes the
// scroll buttons and resets +0x4ED4/+0x4ED0 from shape 4's frame count.
#include "shared/sidebar.hpp"
#include "shared/shape_file.hpp"

void SidebarClass::Unknown_0067CA50(int index)
{
    if (Column[index].BuildableCount > 0) {
        Unknown_0067C850();
    } else {
        unknown_buttons_ab9508[index].Disable();
        int first = -1;
        for (int i = 0; i < 4; i++) {
            if (Column[i].BuildableCount > 0) {
                first = i;
                break;
            }
        }
        if (first == -1) {
            Unknown_0067C850();
            if (unknown_shapes_abcd20[4]) {
                unknown_4ed4 = -1;
                unknown_4ed0 = ((ShapeFileStruct const *)unknown_shapes_abcd20[4])->FrameCount;
            }
        } else {
            Select_Tab(first);
        }
    }
}
