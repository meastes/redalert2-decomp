// NON-EXACT DRAFT 0x67CA50 SidebarClass::Unknown_0067CA50 (a strip emptied) (884 B)
// entry 0x67CA50; exact-match score 59.51 (lane best), this text 59.51; plan "draft, verify"
// origin lane: Scout624 (ca50_5, its own score not recorded; lane best 59.51 Scout998, text not kept) (build/class-work/Scout624/w/ca50_5.cpp)
// Recovered source text by our lanes/workers; never exact-match credit. Tracked by
// Scout1103 (native/drafts.json, native M10). Not original bytes.
#include "shared/sidebar.hpp"
#include "shared/shape_file.hpp"

void SidebarClass::Unknown_0067CA50(int index)
{
    if (Column[index].BuildableCount > 0) {
        Unknown_0067C850();
    } else {
        unknown_buttons_ab9508[index].Disable();
        int first = Unknown_00681980();
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
