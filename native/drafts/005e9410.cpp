// NON-EXACT DRAFT 0x5E9410 FUN_005e9410
// entry 0x5E9410; exact-match score 100.0 (misses_index best), last lane score 36.27; plan "draft as-is"
// origin lane: Scout888 (build/class-work/Scout888/w/keep/e9_k111A_9979.cpp)
// Recovered source text by our lanes/workers; never exact-match credit. Tracked by
// Scout1061 (native/drafts.json). Not original bytes.
// @ref ?unknown_session_a3d298@@3VUnknownSessionState@@A=00a3d298
// @ref ?Unknown_00672F10@UnknownSessionState@@QAEEXZ=00672f10
// @ref ?unknown_7add6c@OwnerDraw@@3HA=007add6c
// @ref ?unknown_7add78@OwnerDraw@@3HA=007add78
// @ref ?unknown_8523e0@@3HA=008523e0
// @ref ?unknown_screen_heights_7add74@@3PAHA=007add74
// @ref ?Is_Listed_Kind_5e0da0@OwnerDraw@@YI_NH@Z=005e0da0
// @ref __imp__GetParent@4=0079a500
// @ref __imp__GetWindowRect@8=0079a398
// @ref __imp__GetDlgCtrlID@4=0079a3f4
// @ref __imp__MoveWindow@24=0079a3e0
// @ref ?unknown_registry_a73640@OwnerDraw@@3VUnknownControlIndex@1@A=00a73640
#include "shared/owner_draw_sidebar.hpp"
namespace seed_005e0da0 {
unsigned int __fastcall FUN_005e0da0(int param_1);
}
namespace OwnerDraw {


void __fastcall unknown_5e9410(HWND window)
{
    HWND parent = GetParent(window);
    RECT parent_rect;
    RECT rect;
    GetWindowRect(parent, &parent_rect);
    GetWindowRect(window, &rect);
    int x = rect.left - parent_rect.left;
    int y = rect.top - parent_rect.top;
    int width = rect.right - rect.left;
    int height = rect.bottom - rect.top;
    int kind = Kind_Of((unsigned int)parent);
    int id = GetDlgCtrlID(window);
    int old_x = x;
    int old_y = y;
    int old_width = width;
    int old_height = height;
    if ((kind == 0x10c || kind == 0x94 || kind == 0xa3 || kind == 0x73 || kind == 0xd8 || kind == 0xbbb || kind == 0xf5 || kind == 0x105 || kind == 0xe2 || kind == 0xd5 || kind == 0x2b5 || kind == 0xb7 || kind == 0x2b4 || kind == 0xbc7 || kind == 0x101 || kind == 0x129 || kind == 0xbc6 || kind == 0xd7 || kind == 0x108 || kind == 0xbc || kind == 0xbd || kind == 0xbb || kind == 0xb5 || kind == 0xbba || kind == 0xff || kind == 0x6b || kind == 0x100 || kind == 0x102 || kind == 0xb8 || kind == 0xd6 || kind == 0x103 || Is_Listed_Kind_5e0da0(kind)) && id == 0x694) {
        bool c12 = kind == 0x10c || kind == 0x94 || kind == 0xa3 || kind == 0x73 || kind == 0xd8 || kind == 0xbbb || kind == 0xf5 || kind == 0x105 || kind == 0xe2 || kind == 0xd5 || kind == 0x2b5 || kind == 0xb7 || kind == 0x2b4 || kind == 0xbc7 || kind == 0x101 || kind == 0x129 || kind == 0xbc6 || kind == 0xd7 || kind == 0x108 || kind == 0xbc || kind == 0xbd || kind == 0xbb || kind == 0xb5 || kind == 0xbba || kind == 0xff || kind == 0x6b || kind == 0x100 || kind == 0x102 || kind == 0xb8 || kind == 0xd6 || kind == 0x103;
        if (c12) {
            bool c4 = Is_Option_Kind(kind) || kind == 0x105 || kind == 0x6b || kind == 0x113;
            if (c4) {
                if (!unknown_session_a3d298.Unknown_00672F10()) {
                    y += 1;
                }
            } else {
                if (!unknown_session_a3d298.Unknown_00672F10()) {
                    y += 7;
                    height += 1;
                }
            }
        } else if (kind == 0x125 || kind == 0xe7 || kind == 0x116 || kind == 0xc2 || kind == 0xc9 || kind == 0x11d || kind == 0x11c || kind == 0x114 || kind == 700 || kind == 0x113 || kind == 0x109 || kind == 0x10f || kind == 0x117 || kind == 0xe6 || kind == 0xf3 || kind == 0xf4 || kind == 0x10e) {
            if (kind != 0xc2 && kind != 0xc9 && kind != 0x113) {
                y += 2;
            }
        }
    } else if (kind == 0x102) {
        if (id == 0x50c) {
            y -= 1;
        } else if (id == 0x54e || id == 0x693 || id == 0x696 || id == 0x69a) {
            x -= 1;
        }
    } else if (kind == 0xbb) {
        if (id == 0x540) {
            width -= 1;
        }
    } else if (kind == 0xbc || kind == 0xbd) {
        if (id == 0x53f) {
            width += 1;
        } else if (id == 0x54f) {
            y -= 1;
        } else if (id == 0x54e || id == 1000 || id == 0x54d || id == 0x69a) {
            x -= 1;
        }
    } else if (kind == 0x103) {
        if (id == 0x6f3 || id == 0x6f4 || id == 0x69f || id == 0x6f6 || id == 0x6ef || id == 0x6f1 || id == 0x6f0 || id == 0x6f2) {
            if (unknown_8523e0 >= unknown_7add6c) {
                y -= (unknown_7add78 - unknown_screen_heights_7add74[0]) / 2;
            }
        }
    } else if (kind == 0xbc7) {
        if (id == 0x733 || id == 0x735 || id == 0x732 || id == 0x3ea || id == 0x73c || id == 0x411 || id == 0x412 || id == 0x73d || id == 0x419 || id == 0x41a || id == 0x73e || id == 0x6a3 || id == 0x6a4 || id == 0x73f || id == 0x6a5 || id == 0x6a7 || id == 0x740 || id == 0x6a6 || id == 0x6a8 || id == 0x736 || id == 0x73a || id == 0x73b || id == 0x737 || id == 0x739 || id == 0x738) {
            if (unknown_8523e0 >= unknown_7add6c) {
                y -= (unknown_7add78 - unknown_screen_heights_7add74[0]) / 2;
            }
        }
    }
    if (x != old_x || y != old_y || width != old_width || old_height != height) {
        MoveWindow(window, x, y, width, height, FALSE);
    }
}

}
