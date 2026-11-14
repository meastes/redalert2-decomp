// NON-EXACT DRAFT 0x5FFB00 unknown_dialog_proc_5ffb00 (Default_Dialog_Proc, ownrdraw.cpp)
// entry 0x5FFB00; exact-match score 53.32 (OwnrDraw406 natural; OPEN_MISSES best 66.88 used invented helpers)
// origin lane: OwnrDraw389/OwnrDraw406 (build/class-work/OwnrDraw406/c/5ffb00a.cpp)
// Recovered source text by our lanes/workers; never exact-match credit. Tracked by
// Scout1081 (native/drafts.json). Not original bytes.
// @ref ?unknown_5ecc90@@YIXPAUHWND__@@H@Z=005ecc90
// @ref ?unknown_int_a40d34@@3HA=00a40d34
// @ref ?MainWindow@@3PAUHWND__@@A=00b263d8
// @ref ?unknown_empty_wide_839d4c@@3QBGB=00839d4c
#include "shared/win32_api.hpp"
#include "shared/win32_window.hpp"
#include "shared/unknown_globals.hpp"
#include "shared/session_state.hpp"
#include "shared/campaign_score.hpp"
#include "shared/queue.hpp"
#include "shared/gscreen.hpp"
#include "shared/lobby.hpp"
#include "shared/main_opt.hpp"
#include "shared/ms_message_box.hpp"
#include "shared/string_table.hpp"
#include "shared/ipx_global_conn.hpp"
#include "shared/rect.hpp"
#include "shared/owner_draw_dialog_control.hpp"

void __fastcall unknown_735ba0(HWND window);
void __fastcall unknown_737060(HWND window);

// NATIVE (Scout1081 adaptation): declarations OwnrDraw406's scratch overlay
// supplied, as the accepted rows declare them (OwnerDraw_unknown_5e21c0/5e2160/
// 5e2260 and unknown_show_dialog_5ff7e0's inline unknown_prepare_dialog, copied
// from src/recovered/ownrdraw/unknown_show_dialog_5ff7e0_005ff7e0.cpp).
namespace OwnerDraw {
WideString __fastcall unknown_5e21c0(HWND window, HWND item, int x, int y);
WideString __fastcall unknown_5e2160(HWND window, int a);
WideString __fastcall unknown_5e2260(unsigned int key, HWND window);
BOOL __stdcall unknown_5e8720(HWND window, LPARAM lparam);
BOOL __stdcall unknown_5e7eb0(HWND window, LPARAM lparam);
BOOL __stdcall unknown_5e8130(HWND window, LPARAM lparam);
bool __fastcall unknown_5e9fa0(HWND dialog);
void __fastcall unknown_5eaaa0(HWND control, int value);
void __fastcall unknown_set_control_art_5ea960(unsigned int key);
bool __fastcall unknown_5ea550(HWND control);
bool __fastcall unknown_5ea390(HWND control);
bool __fastcall unknown_5ea720(HWND control);
bool __fastcall unknown_5ea810(HWND control);
bool __fastcall unknown_5ea230(HWND control);
BOOL __stdcall unknown_5e86d0(HWND window, LPARAM lparam);

inline void unknown_prepare_dialog(HWND dialog, int kind)
{
    unknown_735ba0(dialog);
    unknown_5eaaa0(dialog, kind);
    if (!unknown_session_a3d298.Unknown_00672F10()) {
        unknown_set_control_art_5ea960((unsigned int)dialog);
    }
    unknown_5ea550(dialog);
    unknown_5ea390(dialog);
    unknown_5ea720(dialog);
    unknown_5ea810(dialog);
    EnumChildWindows(dialog, unknown_5e8720, 0);
    if (unknown_5e9fa0(dialog)) {
        Point2D size;
        size.X = unknown_screen_widths_7add68[0];
        size.Y = unknown_screen_heights_7add74[0];
        Unknown_005E9F00(dialog, &size);
    } else {
        unknown_5ea230(dialog);
    }
    EnumChildWindows(dialog, unknown_5e7eb0, 0);
    EnumChildWindows(dialog, unknown_5e8130, 0);
    unknown_737060(dialog);
}
}
void __fastcall unknown_5ecc90(HWND window, int unknown2);

int __fastcall unknown_dialog_proc_5ffb00(HWND window, UINT message, WPARAM wparam, LPARAM lparam)
{
    switch (message) {
    case 0x2:
        Unknown_005B6060(window);
        unknown_int_a40d34--;
        SetFocus(MainWindow);
        return 0;
    case 0x2b:
        Unknown_005FE380(lparam);
        return 1;
    case 0xf: {
        OwnerDraw::WinData *record = OwnerDraw::unknown_registry_a73640.Find((unsigned int)window);
        if (record) {
            if (record->unknown_bc) {
                ValidateRect(window, 0);
                return 1;
            }
            Unknown_005FEE70(window);
            record = OwnerDraw::Get_Record(window);
            if (record && record->unknown_be) {
                HWND item = GetDlgItem(window, 0x71a);
                if (item) {
                    SendMessageA(item, 0x4e2, 0, 0);
                }
                OwnerDraw::unknown_5e5150(window, false);
                record->unknown_be = 0;
            }
            ValidateRect(window, 0);
            return 0;
        }
        break;
    }
    case 0x14:
        return 1;
    case 0x84: {
        HWND tip = GetDlgItem(window, 0x695);
        if (tip) {
            int x = lparam & 0xffff;
            int y = (unsigned int)lparam >> 16;
            RECT rect;
            GetWindowRect(window, &rect);
            POINT point;
            point.x = x - rect.left;
            point.y = y - rect.top;
            WideString text;
            HWND child = ChildWindowFromPointEx(window, point, 1);
            if (child) {
                text = OwnerDraw::unknown_5e21c0(window, child, x, y);
                if (!Compare_Wide_007837d3(text.Get(), unknown_empty_wide_839d4c)) {
                    text = OwnerDraw::unknown_5e2160(window, (int)child);
                    if (!Compare_Wide_007837d3(text.Get(), unknown_empty_wide_839d4c)) {
                        text = OwnerDraw::unknown_5e2260((unsigned int)window, child);
                    }
                }
            }
            SendMessageA(tip, 0x4b2, 0, (LPARAM)text.unknown_text_770430());
        }
        break;
    }
    case 0x110:
        unknown_int_a40d34++;
        if (lparam) {
            unknown_show_dialog_5ff7e0(window, *(unsigned short *)lparam);
        } else {
            unknown_5ecc90(window, 0);
            OwnerDraw::unknown_prepare_dialog(window, 0);
        }
        SetFocus(window);
        return 0;
    case 0x132:
    case 0x133:
    case 0x134:
    case 0x135:
    case 0x136:
    case 0x137:
    case 0x138:
        return (int)GetStockObject(4);
    case 0x497:
        SendMessageA(window, 0x4a9, (WPARAM)window, 1);
        return 0;
    case 0x4ec:
        EnumChildWindows(window, OwnerDraw::unknown_5e86d0, 0);
        break;
    }
    return 0;
}
