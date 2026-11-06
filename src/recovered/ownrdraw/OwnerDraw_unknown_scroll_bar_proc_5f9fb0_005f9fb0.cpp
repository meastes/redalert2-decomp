// SPDX-License-Identifier: GPL-3.0-or-later
// Copyright 2026 ra2-decomp contributors
// Copyright 2025 Electronic Arts Inc.; 2026 OpenTS contributors
// Derived from Electronic Arts source code; EA's GPL Section 7 terms apply (COPYING.md).
// LRESULT OwnerDraw::unknown_scroll_bar_proc_5f9fb0(HWND, UINT, WPARAM, LPARAM) (0x005F9FB0)
// Adopted from ra2-decomp OwnerDraw::ScrollBarHandler (YR 0x0061C690).
// @ref ?unknown_surface_839958@@3PAVSurface@@A=00839958
// @ref ?Window_Rect_In_Main_Client@@YIHPAUHWND__@@PAURECT@@@Z=00735690
// @ref ?unknown_registry_a73640@OwnerDraw@@3VUnknownControlIndex@1@A=00a73640
// @ref ?unknown_window_procs_a73688@OwnerDraw@@3VUnknownWindowProcIndex@1@A=00a73688
// @ref ?unknown_surface_count_a763d8@OwnerDraw@@3HA=00a763d8
// @ref ?unknown_bitmap_writer_a76378@@3VUnknownBitmapWriter@@A=00a76378
// @ref ?Get_Image_0068ca40@UnknownBitmapWriter@@QAEPAVSurface@@PBDH@Z=0068ca40
// @ref ?Draw_0068cce0@UnknownBitmapWriter@@QAE_NABVRect@@PAVSurface@@1HH@Z=0068cce0
// @ref ?unknown_5fdb20@OwnerDraw@@YIXPAVSurface@@ABVRect@@HH@Z=005fdb20
// @ref ?unknown_draw_edge_glows_5fd810@OwnerDraw@@YIXPAVSurface@@ABVRect@@HHHHHH@Z=005fd810
// @ref ?unknown_draw_arrow_5fd960@OwnerDraw@@YIXPAVSurface@@ABVRect@@HH_N@Z=005fd960
// @ref ?unknown_color_a73928@OwnerDraw@@3HA=00a73928
// @ref ?unknown_color_a76158@OwnerDraw@@3HA=00a76158
// @ref ?unknown_color_a73910@OwnerDraw@@3HA=00a73910
// @ref ?unknown_color_a733d0@OwnerDraw@@3HA=00a733d0
// @ref ?unknown_red_mask_a763dc@OwnerDraw@@3GA=00a763dc
// @ref ?unknown_green_mask_a763de@OwnerDraw@@3GA=00a763de
// @ref ?unknown_blue_mask_a763e0@OwnerDraw@@3GA=00a763e0
// @ref ?unknown_shift_852fb8@@3HA=00852fb8
// @ref ?unknown_shift_852fbc@@3HA=00852fbc
// @ref ?unknown_shift_852fc0@@3HA=00852fc0
// @ref ?unknown_shift_852fc4@@3HA=00852fc4
// @ref ?unknown_shift_852fc8@@3HA=00852fc8
// @ref ?unknown_shift_852fcc@@3HA=00852fcc
// @ref ??_7BSurface@@6B@=0079aef8
// @ref ??_7XSurface@@6B@=0079af8c
// @ref ??0Buffer@@QAE@PAXJ@Z=004382c0
// @ref ??2@YAPAXI@Z=00782017
// @ref __ftol=0077f110
// @ref __imp__CallWindowProcA@20=0079a4c0
// @ref __imp__ValidateRect@8=0079a3f8
// @ref __imp__InvalidateRect@12=0079a494
// @ref __imp__GetClientRect@8=0079a4b8
// @ref __imp__GetParent@4=0079a500
// @ref __imp__SendMessageA@16=0079a4a0
// @ref __imp__KillTimer@8=0079a4c4
// @ref __imp__SetTimer@16=0079a4d4
// @ref __imp__GetCursorPos@4=0079a3a8
// @ref __imp__ScreenToClient@8=0079a450
// @ref __imp__SetCapture@4=0079a3f0
// @ref __imp__ReleaseCapture@0=0079a3e8
// @ref ??_C@_0M@IKEK@sbgripm?4pcx?$AA@=007eaf3c
// @ref ??_C@_0N@EMCN@gsbgripm?4pcx?$AA@=007eaf48
// @ref ??_C@_0M@CJBA@sbgript?4pcx?$AA@=007eaf20
// @ref ??_C@_0N@OPHH@gsbgript?4pcx?$AA@=007eaf2c
// @ref ??_C@_0M@OLJK@sbgripb?4pcx?$AA@=007eaf04
// @ref ??_C@_0N@CNPN@gsbgripb?4pcx?$AA@=007eaf10
#include "shared/unknown_globals.hpp"
#include "shared/win32_api.hpp"
#include "shared/win32_window.hpp"
#include "shared/surface.hpp"
#include "shared/hicolor.hpp"
#include "shared/owner_draw_dialog_control.hpp"

// winuser.h SBM_SETSCROLLINFO argument.
struct tagSCROLLINFO {
    UINT cbSize;
    UINT fMask;
    int nMin;
    int nMax;
    UINT nPage;
    int nPos;
    int nTrackPos;
};
typedef tagSCROLLINFO SCROLLINFO;

extern "C" double __cdecl log(double value);
#pragma intrinsic(log)

namespace OwnerDraw {

void __fastcall unknown_draw_edge_glows_5fd810(Surface *surface, Rect const &rect, int raised, int count,
                                               int left_alpha, int top_alpha, int right_alpha, int bottom_alpha);
void __fastcall unknown_draw_arrow_5fd960(Surface *surface, Rect const &rect, int upward, int pressed, bool prefixed);

// A scroll bar's view of its registry record (UnknownControlRecord, 0x200 bytes): the shared fields it
// reads and its state from +0xE8.
struct UnknownScrollBarRecord {
    char unknown_00[0x08];
    HWND owner_08;          // +0x08 told of position changes (WM_VSCROLL)
    char unknown_0c[0x10 - 0x0c];
    Surface *surface_10;    // +0x10 the dimmed background
    char unknown_14[0x20 - 0x14];
    int unknown_20;         // +0x20 nonzero: WM_PAINT draws nothing
    char unknown_24[0xbc - 0x24];
    bool unknown_bc;        // +0xBC WM_PAINT only validates
    char unknown_bd[0xc8 - 0xbd];
    int unknown_c8;         // +0xC8 background dimming alpha
    char unknown_cc;
    bool alternate_cd;      // +0xCD draws the "g" (disabled) art (0x4F1)
    char unknown_ce[0xe8 - 0xce];
    int result;             // +0xE8 the left button is held
    int dragging;           // +0xEC the thumb is dragged
    int range;              // +0xF0
    int position;           // +0xF4
    int up_pressed;         // +0xF8
    int down_pressed;       // +0xFC
    int keep_capture;       // +0x100 give the capture back to owner_08 on release (0x4D2)
};

// The window procedure of the owner-drawn ScrollBar controls.
LRESULT __stdcall unknown_scroll_bar_proc_5f9fb0(HWND window, UINT message, WPARAM wparam, LPARAM lparam)
{
    RECT client_rect;
    RECT display_rect;
    GetClientRect(window, &client_rect);
    Window_Rect_In_Main_Client(window, &display_rect);
    client_rect.bottom -= 2 * unknown_color_a73928;
    client_rect.right -= 2 * unknown_color_a73928;
    display_rect.left += unknown_color_a73928;
    display_rect.right -= unknown_color_a73928;
    display_rect.top += unknown_color_a73928;
    display_rect.bottom -= unknown_color_a73928;
    UnknownScrollBarRecord *data = (UnknownScrollBarRecord *)Get_Record(window);
    if (data == 0) {
        return 0;
    }
    bool keep_parent_capture = false;
    if (data->keep_capture) {
        keep_parent_capture = true;
    }
    int message_result = data->result;
    int dragging = data->dragging;
    int range = data->range;
    int position = data->position;
    int up_pressed = data->up_pressed;
    int down_pressed = data->down_pressed;
    if (!range) {
        range = 100;
    }
    int grip_top = 0;
    int grip_bottom = 0;
    int scroll_code = 0;
    int width = client_rect.right - client_rect.left;
    int travel = client_rect.bottom - client_rect.top - 44;
    int grip_height = (int)((double)travel - log((double)(range + 1)) * (double)travel * 0.2);
    if (grip_height <= 14) {
        grip_height = 14;
    }
    travel -= grip_height;
    if (travel <= 1) {
        travel = 1;
    }
    if (message < 0x400 || message == 0x4a5) {
        if (!dragging) {
            grip_top = position * travel / range + client_rect.top + 22;
            grip_bottom = grip_top + grip_height;
        } else {
            POINT cursor;
            GetCursorPos(&cursor);
            ScreenToClient(window, &cursor);
            grip_top = cursor.y - grip_height / 2;
            if (grip_top < 22) {
                grip_top = 22;
            }
            if (client_rect.bottom - grip_height - 22 < grip_top) {
                grip_top = client_rect.bottom - grip_height - 22;
            }
            grip_bottom = grip_top + grip_height;
            scroll_code = 5;
            position = range * (grip_top - 22) / travel;
        }
    }
    switch (message) {
    case 0xe1:
        return position;
    case 0xe0:
        if ((int)wparam <= range && (int)wparam > 0) {
            position = (int)wparam;
        }
        break;
    case 0xe2:
        range = (int)lparam;
        if (position > range) {
            position = range;
        }
        break;
    case 0xe9: {
        SCROLLINFO *info = (SCROLLINFO *)lparam;
        range = info->nMax;
        position = info->nPos;
        break;
    }
    case 0x84:
    case 0x87: {
        WNDPROC proc = 0;
        WNDPROC *found = 0;
        if (unknown_window_procs_a73688.Get(window, found) && found) {
            proc = *found;
        }
        return CallWindowProcA(proc, window, message, wparam, lparam);
    }
    case 0x14:
        return 0;
    case 0xf: {
        if (data->unknown_bc) {
            ValidateRect(window, 0);
            break;
        }
        if (data->unknown_20) {
            return 0;
        }
        Rect src_rect(0, 0, client_rect.right, client_rect.bottom);
        Rect full_rect(display_rect.left, display_rect.top, client_rect.right, client_rect.bottom);
        HWND parent = GetParent(window);
        UnknownControlRecord *parent_data = 0;
        if (parent) {
            unknown_registry_a73640.Get((unsigned int const &)parent, parent_data);
        }
        RECT parent_display;
        Window_Rect_In_Main_Client(parent, &parent_display);
        Rect source_rect = src_rect;
        if (parent_data && parent_data->surface_10) {
            source_rect.X = display_rect.left + source_rect.X - parent_display.left;
            source_rect.Y += display_rect.top - parent_display.top;
        }
        if (data->surface_10) {
            if (data->surface_10->Get_Width() != client_rect.right || data->surface_10->Get_Height() != client_rect.bottom) {
                delete data->surface_10;
                data->surface_10 = 0;
            }
        }
        if (!data->surface_10) {
            BSurface *background = new BSurface(client_rect.right, client_rect.bottom, 2);
            data->surface_10 = background;
            unknown_surface_count_a763d8++;
            if (parent_data && parent_data->surface_10) {
                background->Copy_From(src_rect, *parent_data->surface_10, source_rect, false, true);
            }
            int count = client_rect.bottom * client_rect.right;
            unsigned short *pixels = (unsigned short *)background->Lock();
            for (int i = 0; i < count; i++) {
                pixels[i] = Blend_Color(pixels[i], 0xffff, data->unknown_c8);
            }
            if (pixels) {
                background->Unlock();
            }
        }
        if (parent_data && parent_data->surface_10) {
            unknown_surface_839958->Copy_From(full_rect, *parent_data->surface_10, source_rect, false, true);
        }
        int color = unknown_5fda40(unknown_color_a76158);
        if (data->alternate_cd) {
            color = unknown_5fda40(unknown_color_a73910);
        }
        unknown_5fdb20(unknown_surface_839958, full_rect, unknown_color_a73928, color);
        Rect grip_rect(display_rect.left, grip_top + display_rect.top, client_rect.right, grip_bottom - grip_top);
        full_rect = grip_rect;
        src_rect = grip_rect;
        src_rect.X = 0;
        src_rect.Y = grip_top;
        Surface *grip_middle;
        if (data->alternate_cd) {
            grip_middle = unknown_bitmap_writer_a76378.Get_Image_0068ca40("gsbgripm.pcx", 0);
        } else {
            grip_middle = unknown_bitmap_writer_a76378.Get_Image_0068ca40("sbgripm.pcx", 0);
        }
        if (grip_middle) {
            grip_rect.Width = grip_middle->Get_Width();
        }
        unknown_bitmap_writer_a76378.Draw_0068cce0(grip_rect, unknown_surface_839958, grip_middle, 0, 0);
        Rect grip_src(0, 0, grip_middle->Get_Height(), grip_middle->Get_Width());
        Surface *grip_top_image;
        if (data->alternate_cd) {
            grip_top_image = unknown_bitmap_writer_a76378.Get_Image_0068ca40("gsbgript.pcx", 0);
        } else {
            grip_top_image = unknown_bitmap_writer_a76378.Get_Image_0068ca40("sbgript.pcx", 0);
        }
        if (grip_top_image) {
            full_rect.Height = grip_top_image->Get_Height();
        }
        unknown_surface_839958->Copy_From(full_rect, *grip_top_image, grip_src, false, true);
        Surface *grip_bottom_image;
        if (data->alternate_cd) {
            grip_bottom_image = unknown_bitmap_writer_a76378.Get_Image_0068ca40("gsbgripb.pcx", 0);
        } else {
            grip_bottom_image = unknown_bitmap_writer_a76378.Get_Image_0068ca40("sbgripb.pcx", 0);
        }
        if (grip_bottom_image) {
            full_rect.Y = display_rect.top + grip_bottom - grip_bottom_image->Get_Height();
        }
        unknown_surface_839958->Copy_From(full_rect, *grip_bottom_image, grip_src, false, true);
        Rect up_rect(display_rect.left, display_rect.top, client_rect.right, 22);
        full_rect = up_rect;
        src_rect = up_rect;
        src_rect.X = 0;
        src_rect.Y = 0;
        unknown_surface_839958->Copy_From(full_rect, *data->surface_10, src_rect, false, true);
        unknown_draw_edge_glows_5fd810(unknown_surface_839958, up_rect, up_pressed == 0, 2, unknown_color_a733d0,
                                       unknown_color_a733d0, unknown_color_a733d0, unknown_color_a733d0);
        unknown_draw_arrow_5fd960(unknown_surface_839958, up_rect, 1, up_pressed, data->alternate_cd);
        Rect down_rect(display_rect.left, display_rect.bottom - 22, client_rect.right, 22);
        full_rect = down_rect;
        src_rect = down_rect;
        src_rect.X = 0;
        src_rect.Y = client_rect.bottom - 22;
        unknown_surface_839958->Copy_From(full_rect, *data->surface_10, src_rect, false, true);
        unknown_draw_edge_glows_5fd810(unknown_surface_839958, down_rect, down_pressed == 0, 2, unknown_color_a733d0,
                                       unknown_color_a733d0, unknown_color_a733d0, unknown_color_a733d0);
        unknown_draw_arrow_5fd960(unknown_surface_839958, down_rect, 0, down_pressed, data->alternate_cd);
        ValidateRect(window, 0);
        break;
    }
    case 0x113: {
        POINT cursor;
        GetCursorPos(&cursor);
        ScreenToClient(window, &cursor);
        up_pressed = 0;
        down_pressed = 0;
        if (message_result && cursor.x > client_rect.right - width) {
            if (cursor.y < 22) {
                if (!dragging) {
                    up_pressed = 1;
                }
                if (position != 0) {
                    scroll_code = 0;
                    position--;
                }
            } else if (cursor.y > client_rect.bottom - 22) {
                if (!dragging) {
                    down_pressed = 1;
                }
                if (position + 1 <= range) {
                    scroll_code = 1;
                    position++;
                }
            }
        }
        SetTimer(window, 0, 0x19, 0);
        break;
    }
    case 0x200:
        if (dragging) {
            RECT area = client_rect;
            area.left = client_rect.right - width;
            InvalidateRect(window, &area, 0);
        }
        if (wparam & 1) {
            break;
        }
    case 0x202:
        message_result = 0;
        dragging = 0;
        if (up_pressed || down_pressed) {
            InvalidateRect(window, 0, 0);
        }
        up_pressed = 0;
        down_pressed = 0;
        KillTimer(window, 0);
        ReleaseCapture();
        if (keep_parent_capture) {
            SetCapture(data->owner_08);
        }
        scroll_code = 8;
        break;
    case 0x201:
    case 0x203: {
        if (message == 0x201) {
            message_result = 1;
            SetCapture(window);
            SetTimer(window, 0, 500, 0);
        } else {
            message_result = 0;
            dragging = 0;
            KillTimer(window, 0);
            ReleaseCapture();
            if (keep_parent_capture) {
                SetCapture(data->owner_08);
            }
        }
        POINT click;
        click.x = (unsigned short)lparam;
        click.y = (unsigned short)(((unsigned long)lparam >> 16) & 0xffff);
        int repeat = 1;
        if (message == 0x203) {
            repeat = 2;
        }
        down_pressed = 0;
        up_pressed = 0;
        while (repeat > 0) {
            if (click.x > client_rect.right - width) {
                if (click.y < 22 && position) {
                    up_pressed = 1;
                    scroll_code = 0;
                    position--;
                } else {
                    bool skip_thumb = false;
                    if (click.y > client_rect.bottom - 22) {
                        if (position + 1 <= range) {
                            down_pressed = 1;
                            scroll_code = 1;
                            position++;
                            skip_thumb = true;
                        }
                    }
                    if (!skip_thumb) {
                        if (click.y < grip_top || click.y >= grip_bottom) {
                            grip_top = click.y - grip_height / 2;
                            if (grip_top < 22) {
                                grip_top = 22;
                            }
                            if (client_rect.bottom - grip_height - 22 < grip_top) {
                                grip_top = client_rect.bottom - grip_height - 22;
                            }
                            grip_bottom = grip_top + grip_height;
                            scroll_code = 5;
                            position = range * (grip_top - 22) / travel;
                        } else if (message == 0x201) {
                            dragging = 1;
                        }
                    }
                }
            }
            repeat--;
        }
        break;
    }
    case 0x4d2:
        data->keep_capture = lparam != 0;
        break;
    case 0x4f1:
        data->alternate_cd = lparam == 1;
        break;
    }
    int send_notify = 0;
    if (position != data->position || range != data->range) {
        if (data->owner_08) {
            send_notify = 1;
        }
    }
    data->result = message_result;
    data->dragging = dragging;
    data->range = range;
    data->position = position;
    data->up_pressed = up_pressed;
    data->down_pressed = down_pressed;
    if (send_notify) {
        SendMessageA(data->owner_08, 0x115, (unsigned short)position << 16 | (unsigned short)scroll_code, (LPARAM)window);
        InvalidateRect(window, 0, 0);
    }
    return 0;
}

}
