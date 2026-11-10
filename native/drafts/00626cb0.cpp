// void __fastcall Unknown_00626CB0(HWND dialog) (0x00626CB0): the reconnect dialog's response-time bars:
// green, yellow past 240 ticks, red past 480 (blue while the last ping is under 420 ticks old), shorter
// as the time grows.
#include "shared/session_state.hpp"   // NATIVE: for Session (Scout1209 r10)
#include "shared/queue.hpp"
#include "shared/unknown_player_entry.hpp"
#include "shared/hicolor.hpp"
#include "shared/surface.hpp"
#include "shared/rect.hpp"
#include "shared/win32_window.hpp"
#include "shared/rgb_class.hpp"
// @ref ?unknown_ipx_manager_a40978@@3VIPXManagerClass@@A=00a40978
// @ref ?unknown_timing_bars_7ec418@@3PAHA=007ec418
// @ref ?unknown_aabe90@@3HA=00aabe90
// @ref ?unknown_bar_color_ab6831@@3VRGBClass@@A=00ab6831

extern int unknown_aabe90;     // 0x00AABE90
extern RGBClass unknown_bar_color_ab6831;     // 0x00AB6831

void __fastcall Unknown_00626CB0(HWND dialog)
{
    for (int i = 0; i < unknown_session_a3d298.Players.Count(); i++) {
        RECT bar_winrect;
        Window_Rect_In_Main_Client(GetDlgItem(dialog, unknown_timing_bars_7ec418[i]), &bar_winrect);

        Rect bar_rect;
        bar_rect.X = bar_winrect.left;
        bar_rect.Y = bar_winrect.top;
        bar_rect.Width = bar_winrect.right - bar_winrect.left;
        bar_rect.Height = bar_winrect.bottom - bar_winrect.top;
        Rect outline(bar_rect.X, bar_rect.Y, bar_rect.Width, bar_rect.Height);

        int id = unknown_session_a3d298.Players[i]->ID;
        int index;
        if (unknown_session_a3d298.Type == 1 || unknown_session_a3d298.Type == 2) {
            index = 0;
        } else {
            index = unknown_ipx_manager_a40978.Connection_Index(id);
        }
        unsigned int progress = 0;
        if (i != 0) {
            progress = unknown_aabe90 - unknown_timing_aabcf0[index].unknown_0c;
        }

        unsigned short color = Build_Hicolor_Pixel(0, 200, 0);
        if (progress > 240) {
            color = Build_Hicolor_Pixel(200, 200, 0);
            if (progress > 480) {
                color = Build_Hicolor_Pixel(200, 0, 0);
                unsigned int age = (timeGetTime() * 60 - unknown_timing_aabcf0[index].unknown_14 * 60) / 1000;
                if (age < 420) {
                    progress = age;
                    color = Build_Hicolor_Pixel(0, 0, 200);
                }
            }
        }

        int w = (100 - (int)(100 * progress / 1200) > 0 ? 100 - (int)(100 * progress / 1200) : 0) * bar_rect.Width;
        bar_rect.Width = (6 > w / 100 ? 6 : w / 100);

        int outline_color = Build_Hicolor_Pixel(unknown_bar_color_ab6831.Red, unknown_bar_color_ab6831.Green, unknown_bar_color_ab6831.Blue);
        unknown_surface_839958->Draw_Rect(unknown_surface_839958->Get_Rect(), outline, outline_color);
        Rect inside = bar_rect;
        inside.X += 2;
        inside.Y += 2;
        inside.Width -= 4;
        inside.Height -= 4;
        unknown_surface_839958->Fill_Rect(unknown_surface_839958->Get_Rect(), inside, color);

    }
}
