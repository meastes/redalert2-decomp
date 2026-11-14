// winstub.cpp: the original translation unit re-filed as one source (Route A): the accepted
// bodies of its canonical compile group in address order, under the unit's own header
// state (its switches and includes in their original order). The module map
// manifests/original_target/modules/winstub.json carries the addresses and the rows that
// stay in their own files.


#define MAP_VIEW_DECLARE_WINDOW_PROC
#define TOOL_TIP_DECLARE_CREATE
#include "shared/shape_file.hpp"
#include "shared/unknown_geometry.hpp"
#include "shared/win_stub.hpp"
#include "shared/debug_printf.hpp"
#include "shared/mouse.hpp"
#include "shared/unknown_globals.hpp"
#include "shared/win32_api.hpp"
#include "shared/cd_timer.hpp"
#include "shared/dsurface.hpp"
#include "shared/map_view.hpp"
#include "shared/win_util.hpp"
#include "shared/tool_tip.hpp"
#include "shared/wwmouse.hpp"
#include "shared/winsock_interface.hpp"
#include "shared/movie.hpp"
#include "shared/movie_globals.hpp"
#include "shared/exit.hpp"
#include "shared/wstring.hpp"
#include "shared/crt_string.hpp"
#include "shared/msfont.hpp"
#include "shared/crt_tokens.hpp"
#include "shared/cc_file_class.hpp"
#include "shared/surface.hpp"
#include "shared/convert.hpp"
#include "shared/surface_blit.hpp"


// Header-static dynamic initializers of the unit at 0x007371B0: the shape_file.hpp empty rect and the
// shared/unknown_geometry.hpp constants (0x007371B0-0x00737391); the other
// drafts of this plan are emitted by these includes.


// Dynamic initializer (0x007371D0, _$E) of diagonal from the header(s) included
// by HeaderStatics_empty_rect_init_007371b0.cpp; no source of its own.


// Dynamic initializer (0x00737200, _$E) of b from the header(s) included
// by HeaderStatics_empty_rect_init_007371b0.cpp; no source of its own.


// Dynamic initializer (0x00737220, _$E) of c from the header(s) included
// by HeaderStatics_empty_rect_init_007371b0.cpp; no source of its own.


// Dynamic initializer (0x00737240, _$E) of angle45 from the header(s) included
// by HeaderStatics_empty_rect_init_007371b0.cpp; no source of its own.


// Dynamic initializer (0x00737260, _$E) of angle60 from the header(s) included
// by HeaderStatics_empty_rect_init_007371b0.cpp; no source of its own.


// Dynamic initializer (0x00737280, _$E) of angle90 from the header(s) included
// by HeaderStatics_empty_rect_init_007371b0.cpp; no source of its own.


// Dynamic initializer (0x007372A0, _$E) of g from the header(s) included
// by HeaderStatics_empty_rect_init_007371b0.cpp; no source of its own.


// Dynamic initializer (0x007372E0, _$E) of h from the header(s) included
// by HeaderStatics_empty_rect_init_007371b0.cpp; no source of its own.


// Dynamic initializer (0x00737310, _$E) of i from the header(s) included
// by HeaderStatics_empty_rect_init_007371b0.cpp; no source of its own.


// Dynamic initializer (0x00737340, _$E) of j from the header(s) included
// by HeaderStatics_empty_rect_init_007371b0.cpp; no source of its own.


// Dynamic initializer (0x00737370, _$E) of k from the header(s) included
// by HeaderStatics_empty_rect_init_007371b0.cpp; no source of its own.


// Dynamic initializer (0x00737380, _$E) of l from the header(s) included
// by HeaderStatics_empty_rect_init_007371b0.cpp; no source of its own.


// void Focus_Loss() (0x007373A0)
// The game window lost the focus: pauses the radar movie, remembers whether the mouse was captured
// and releases it. The window procedure expands this copy (it is never called).


void Focus_Loss()
{
    Debug_Printf("Focus_Loss()\n");
    Pause_Radar_Movie_005a0a90(true);
    Unknown_00406F50();
    if (WWMouse != 0) {
        unknown_mouse_captured_b263e8 = WWMouse->Is_Captured();
        Debug_Printf("Focus_Loss(): _MouseCaptured = %s\n", unknown_mouse_captured_b263e8 ? "true" : "false");
        WWMouse->Release_Mouse();
    }
}


// void Focus_Restore() (0x00737400)
// The game window regained the focus: re-captures the mouse when it was captured on the loss, restores
// the lost surfaces (clearing them to black unless the first frames after a restart), flags the map for
// a complete redraw, invalidates the window, resumes the radar movie and re-activates the current
// dialog window. Like Focus_Loss, an out-of-line copy of what the window procedure expands.

extern Surface *unknown_surface_839944;
HWND __fastcall Unknown_00735B10();    // 0x00735B10 (the current dialog window)

void Focus_Restore()
{
    Debug_Printf("Focus_Restore()\n");
    Unknown_00406F70();
    Debug_Printf("Focus_Restore(): _MouseCaptured = %s\n", unknown_mouse_captured_b263e8 ? "true" : "false");
    if (WWMouse != 0 && unknown_mouse_captured_b263e8 == true && !Debug_Map) {
        WWMouse->Capture_Mouse();
    }
    if (!unknown_flag_851cb0) {
        if (unknown_surface_839958 != 0) {
            ((DSurface *)unknown_surface_839958)->Restore_Check();
        }
        if (unknown_surface_839954 != 0) {
            static_cast<DSurface *>(unknown_surface_839954)->Restore_Check();
        }
        if (unknown_surface_839964 != 0) {
            if (((DSurface *)unknown_surface_839964)->Restore_Check() && (!unknown_flag_a40d04 || Frame > 0x10)) {
                unknown_surface_839964->Fill(0);
            }
        }
        if (unknown_surface_839944 != 0) {
            if (((DSurface *)unknown_surface_839944)->Restore_Check() && (!unknown_flag_a40d04 || Frame > 0x10)) {
                unknown_surface_839944->Fill(0);
            }
        }
        if (unknown_surface_839948 != 0) {
            if (((DSurface *)unknown_surface_839948)->Restore_Check()) {
                unknown_flag_8373c3 = true;
                if (!unknown_flag_a40d04 || Frame > 0x10) {
                    unknown_surface_839948->Fill(0);
                }
            }
        }
        if (unknown_surface_839950 != 0) {
            if (((DSurface *)unknown_surface_839950)->Restore_Check() && (!unknown_flag_a40d04 || Frame > 0x10)) {
                unknown_surface_839950->Fill_Rect(unknown_window_839640, 0);
            }
        }
    }
    Map.Flag_To_Redraw(2);
    InvalidateRect(MainWindow, 0, FALSE);
    Pause_Radar_Movie_005a0a90(false);
    if (Unknown_00735B10() != 0) {
        SetActiveWindow(Unknown_00735B10());
        SetFocus(Unknown_00735B10());
    }
}


// Main game-window message dispatcher (WinStub.cpp).
extern CCToolTip *ToolTips;
bool __fastcall unknown_moving_736d80(HWND window, unsigned int message, class Rect *rect);
extern bool window_mouse_wheel_b263e9;
extern bool window_scroll_coasting_8373fc;
extern bool unknown_redraw_8523dc;
extern char const ime_processed_007fd18c[], focus_gained_007fd170[], focus_lost_007fd180[];
extern char const sidebar_down_007dd0f8[], sidebar_up_007dcf9c[];
void __fastcall Execute_Command_By_Name(char const *name);
extern "C" unsigned int __cdecl wcslen(wchar_t const *text);
unsigned short __fastcall Char_To_Wide_00738130(unsigned int character, long key_data);
LRESULT WINAPI Windows_Procedure(HWND window, UINT message, WPARAM wparam, LPARAM lparam)
{
    if (PacketTransport && (message == PacketTransport->Protocol_Event_Message(-1) || message == 0x475)) {
        if (PacketTransport->Message_Handler((int)window, message, wparam, lparam))
            return DefWindowProcA(window, message, wparam, lparam);
        return 0;
    }
    Map.Message_Handler(window, message, wparam, lparam);
    if (MainWindow) GetMenu(MainWindow);
    if (IME_Process_Message(window, message, wparam, lparam)) {
        Debug_Printf(ime_processed_007fd18c);
        return IMEDefaultResult;
    }
    switch (message) {
    case 0x18:
        return 0;
    case 0xf:
        if (unknown_int_a40d48 == 0 && (unknown_flag_a40d28 == true || unknown_flag_851cb0 == true)
            && WWMouse && unknown_surface_839950 && unknown_surface_839954 && unknown_surface_839964) {
            if (unknown_flag_a40d04 == true) {
                Unknown_004E0C90(WWMouse->Is_Captured(), unknown_surface_839964, 0);
                Map.Unknown_0067D420(true);
                ValidateRect(window, 0);
                break;
            }
            if ((unsigned char)Is_Movie_Active()) {
                Movie_Handle_Blit();
                ValidateRect(window, 0);
                break;
            }
            Unknown_004E0C90(WWMouse->Is_Captured(), unknown_surface_839954, 0);
        }
        ValidateRect(window, 0);
        break;
    case 0x14:
        return 1;
    case 1:
        ToolTips = new CCToolTip(window);
        break;
    case 3:
        if (unknown_flag_851cb0 == true && WWMouse)
            static_cast<WWMouseClass *>(WWMouse)->Calc_Confining_Rect();
        break;
    case 2:
        if (ToolTips) {
            delete ToolTips;
            ToolTips = 0;
        }
        MainWindow = 0;
        if (ReadyToQuit != 0) ReadyToQuit = 2;
        return 0;
    case 0x1c:
        if (window == MainWindow && unknown_flag_a40d28 != (wparam != 0)) {
            unknown_flag_a40d28 = wparam != 0;
            if (!unknown_flag_a40d28) {
                Focus_Loss();
                Debug_Printf(focus_lost_007fd180);
            } else {
                Focus_Restore();
                Debug_Printf(focus_gained_007fd170);
            }
            unknown_redraw_8523dc = true;
        }
        return 0;
    case 0x205:
        window_scroll_coasting_8373fc = false;
        break;
    case 0x216:
        return unknown_moving_736d80(window, wparam, (Rect *)lparam);
    case 0x20a:
        if (!window_mouse_wheel_b263e9) {
            window_mouse_wheel_b263e9 = true;
            Execute_Command_By_Name((short)(wparam >> 16) < 0 ? sidebar_down_007dd0f8 : sidebar_up_007dcf9c);
            window_mouse_wheel_b263e9 = false;
        }
        break;
    case 0x112:
        switch (wparam) {
        case 0xf060: return 0;
        case 0xf140: return 0;
        }
        break;
    }
    if (WWKeyboard->Message_Handler(window, message, wparam, lparam)) return 0;
    if (message == 0x286) {
        int index = wcslen(IMEResultString);
        IMEResultString[index] = Char_To_Wide_00738130(wparam,lparam);
        IMEResultString[index + 1] = 0;
        return 0;
    }
    return DefWindowProcA(window, message, wparam, lparam);
}


// WinStub.cpp's empty build-information hook (primary reference file order).
void Build_Info_00737a50()
{
}


// void __fastcall Build_Time_Text(char *buffer, int size) (0x00737A60)
// The build time stamp as "MM/DD/YYYY - hh:mm:ss" (buffer untouched if it does not convert).


void __fastcall Build_Time_Text(char *buffer, int size)
{
    SYSTEMTIME time;
    if (FileTimeToSystemTime(&unknown_build_time_7d8940, &time)) {
        sprintf(buffer, "%02d/%02d/%04d - %02d:%02d:%02d", time.wMonth, time.wDay, time.wYear, time.wHour,
                time.wMinute, time.wSecond);
    }
}


// char *Unknown_00737ad0(char *buffer, int size) (0x00737AD0)
// TS winstub.cpp Build_Number_String: the build number (0x007D8900) as decimal text in buffer;
// size is not read.

extern int unknown_build_number_7d8900;         // 0x007D8900 the build number (39)

char *__fastcall Unknown_00737ad0(char *buffer, int size)
{
    sprintf(buffer, "%d", unknown_build_number_7d8900);
    return buffer;
}


// 0x00737b20: function not yet identified
//
// Body: 47 bytes (frozen inventory bound).
// Referenced from 0 sites.
// Ghidra prototype: char * __fastcall FUN_00737b20(char *param_1)
//
// Types and names are Ghidra placeholders;
// rename and retype as the purpose becomes known.
//

namespace seed_00737b20 {

extern unsigned int  FUN_00784130(unsigned int *text_words, char delimiter);
extern "C" char s_gsmallwoo_007d8904[];

char * __fastcall FUN_00737b20(char *destination)
{
    int separator_address;

    *destination = s_gsmallwoo_007d8904[0];
    destination[1] = '\0';
    destination[2] = '\0';
    separator_address = (int)(FUN_00784130((unsigned int *)(s_gsmallwoo_007d8904), (char)(0x5f)));
    if (separator_address != 0) {
        destination[1] = *(char *)(separator_address + 1);
    }
    return destination;
}

}  // namespace seed_00737b20


extern "C" char *__cdecl strchr(char const *, int);
extern int unknown_build_number_7d8900;
extern "C" char s_gsmallwoo_007d8904[];
char *__fastcall Build_Version_String_00737b50(char *buffer, int length)
{
    char number[128];
    char initials[128];
    sprintf(number, "%d", unknown_build_number_7d8900);
    initials[0] = s_gsmallwoo_007d8904[0];
    initials[1] = 0;
    initials[2] = 0;
    char const *separator = strchr(s_gsmallwoo_007d8904, '_');
    if (separator)
        initials[1] = separator[1];
    sprintf(buffer, "%s-%s", initials, number);
    return buffer;
}


// void __fastcall Create_Main_Window(HINSTANCE instance, int command_show, int width, int height) (0x00737BD0)
// Registers the "Red Alert 2" window class and creates the game window: a captionless popup the size
// of the client area in windowed mode, a topmost screen-sized one otherwise; then the IME context.


void __fastcall Create_Main_Window(HINSTANCE instance, int command_show, int width, int height)
{
    WNDCLASSA wndclass;
    RECT rect;

    InitCommonControls();
    unknown_icon_b263d4 = LoadIconA(instance, MAKEINTRESOURCEA(0x5D));
    unknown_cursor_b263d0 = LoadCursorA(ProgramInstance, MAKEINTRESOURCEA(0x68));
    wndclass.style = 3;
    wndclass.lpfnWndProc = Windows_Procedure;
    wndclass.cbClsExtra = 0;
    wndclass.cbWndExtra = 0;
    wndclass.hInstance = instance;
    wndclass.hIcon = unknown_icon_b263d4;
    wndclass.hCursor = unknown_cursor_b263d0;
    wndclass.hbrBackground = 0;
    wndclass.lpszMenuName = 0;
    wndclass.lpszClassName = "Red Alert 2";
    RegisterClassA(&wndclass);
    if (unknown_flag_851cb0) {
        MainWindow = CreateWindowExA(0, "Red Alert 2", "Red Alert 2", 0x2CA0000, 0, 0, 0, 0, 0, 0, instance, 0);
        SetRect(&rect, 0, 0, width, height);
        AdjustWindowRectEx(&rect, GetWindowLongA(MainWindow, -16), GetMenu(MainWindow) != 0,
                           GetWindowLongA(MainWindow, -20));
        MoveWindow(MainWindow, 0, 0, rect.right - rect.left, rect.bottom - rect.top, TRUE);
    } else {
        MainWindow = CreateWindowExA(8, "Red Alert 2", "Red Alert 2", 0x82000000, 0, 0, GetSystemMetrics(0),
                                     GetSystemMetrics(1), 0, 0, instance, 0);
    }
    IMEContext = ImmGetContext(MainWindow);
    ShowWindow(MainWindow, 1);
    unknown_command_show_b263cc = command_show;
    UpdateWindow(MainWindow);
    SetFocus(MainWindow);
    RegisterHotKey(MainWindow, 1, 7, 0x4D);
    SetCursor(LoadCursorA(ProgramInstance, MAKEINTRESOURCEA(0x68)));
}


// void IME_Reset() (0x00737D90)
// Clears the composition and result text and cancels the IME's composition.


void IME_Reset()
{
    IMECompositionString[0] = 0;
    IMECompositionLength = 0;
    IMECursorPosition = 0;
    IMEComposing = 0;
    IMEResultString[0] = 0;
    if (IMEContext != 0) {
        ImmNotifyIME(IMEContext, 0x15, 4, 0);
    }
}


// void IME_Get_Candidate_00737dd0(HIMC context, DWORD index, char *buffer, unsigned size) (0x00737DD0)
// Copies the selected entry of the IME candidate list index into buffer (at most size characters,
// terminated). As in the original, the list is allocated at the reported size but never filled
// (ImmGetCandidateListA is only asked for the size), and the selection's offset is used as the
// string pointer itself.

struct CANDIDATELIST {
    DWORD dwSize;                   // +0x00
    DWORD dwStyle;                  // +0x04
    DWORD dwCount;                  // +0x08
    DWORD dwSelection;              // +0x0C
    DWORD dwPageStart;              // +0x10
    DWORD dwPageSize;               // +0x14
    DWORD dwOffset[1];              // +0x18
};
extern "C" DWORD __stdcall ImmGetCandidateListA(HIMC context, DWORD index, CANDIDATELIST *list, DWORD length);   // 0x00781C2C
extern "C" void *__cdecl malloc(unsigned int size);
extern "C" void __cdecl free(void *block);
extern "C" char *__cdecl strncat(char *destination, char const *source, unsigned int count);
void __fastcall IME_Get_Candidate_00737dd0(HIMC context, DWORD index, char *buffer, unsigned int size);

void __fastcall IME_Get_Candidate_00737dd0(HIMC context, DWORD index, char *buffer, unsigned int size)
{
    if (context) {
        DWORD length = ImmGetCandidateListA(context, index, 0, 0);
        if (length) {
            CANDIDATELIST *list = (CANDIDATELIST *)malloc(length);
            if (list) {
                if (list->dwCount && list->dwSelection < list->dwCount) {
                    strncat(buffer, (char const *)list->dwOffset[list->dwSelection], size);
                    buffer[size - 1] = 0;
                }
                free(list);
            }
        }
    }
}


// void IME_Update_Composition() (0x00737E30)
// Reads the IME's composition string (wide) and caret position (in characters).


void IME_Update_Composition()
{
    char buffer[0x204];
    IMECompositionString[0] = 0;
    IMECompositionLength = 0;
    IMECursorPosition = 0;
    if (IMEContext != 0) {
        int length = ImmGetCompositionStringA(IMEContext, 8, buffer, 0x200);
        if (length > 0) {
            buffer[length] = 0;
            int count = MultiByteToWideChar(0, 0, buffer, -1, IMECompositionString, 0x100);
            unknown_b263a8 = 0;
            if (count < 0) {
                count = 0;
            } else {
                IMECursorPosition = ImmGetCompositionStringA(IMEContext, 0x80, 0, 0) & 0xFFFF;
                count = wcslen(IMECompositionString);
            }
            IMECursorPosition = _mbsnbcnt((unsigned char *)buffer, IMECursorPosition);
            IMECompositionString[count] = 0;
            IMECompositionLength = count;
            if (IMECursorPosition > count) {
                IMECursorPosition = count;
            } else if (IMECursorPosition < 0) {
                IMECursorPosition = 0;
            }
        }
    }
}


// void __fastcall IME_Send_Result(HWND window) (0x00737F20)
// Sends each character of the IME's result string to window as message 0x4D9.


void __fastcall IME_Send_Result(HWND window)
{
    char buffer[0x204];
    unsigned short text[0x102];
    if (IMEContext != 0) {
        int length = ImmGetCompositionStringA(IMEContext, 0x800, buffer, 0x200);
        if (length > 0) {
            buffer[length] = 0;
            int count = MultiByteToWideChar(0, 0, buffer, strlen(buffer), text, 0x100);
            if (count < 0) {
                count = 0;
            }
            text[count] = 0;
            for (int index = 0; index < count; index++) {
                SendMessageA(window, 0x4D9, text[index], text[index]);
            }
        }
    }
}


// BOOL __fastcall IME_Process_Message(HWND window, UINT message, WPARAM wparam, LPARAM lparam) (0x00737FC0)
// WM_IME_STARTCOMPOSITION/ENDCOMPOSITION/COMPOSITION update the composition state; these and the other
// IME messages go to DefWindowProcA (result in IMEDefaultResult) and count as handled.


BOOL __fastcall IME_Process_Message(HWND window, UINT message, WPARAM wparam, LPARAM lparam)
{
    switch (message) {
    case 0x10D:
        IME_Update_Composition();
        IMEResultString[0] = 0;
        IMEComposing = 1;
        IMEDefaultResult = DefWindowProcA(window, message, wparam, lparam);
        return TRUE;
    case 0x10E:
        IME_Update_Composition();
        IMEComposing = 0;
        IMEDefaultResult = DefWindowProcA(window, message, wparam, lparam);
        return TRUE;
    case 0x10F:
        IME_Update_Composition();
        IMEDefaultResult = DefWindowProcA(window, message, wparam, lparam);
        return TRUE;
    case 0x281:
        IMEDefaultResult = DefWindowProcA(window, message, wparam, lparam);
        return TRUE;
    case 0x282:
    case 0x285:
    case 0x290:
    case 0x291:
        IMEDefaultResult = DefWindowProcA(window, message, wparam, lparam);
        return TRUE;
    }
    return FALSE;
}


// void IME_Close_007380c0() (0x007380C0)
// Closes the input method of the game window's IME context (ImmSetOpenStatus FALSE). The window
// unit has two functions with this body (0x007380C0, 0x007380E0).

extern "C" BOOL __stdcall ImmSetOpenStatus(HIMC context, BOOL open);   // 0x00781C38
void IME_Close_007380c0();

void IME_Close_007380c0()
{
    if (IMEContext) {
        ImmSetOpenStatus(IMEContext, 0);
    }
}


// void IME_Close_007380e0() (0x007380E0)
// Closes the input method of the game window's IME context (ImmSetOpenStatus FALSE). The window
// unit has two functions with this body (0x007380C0, 0x007380E0).

void IME_Close_007380e0();

void IME_Close_007380e0()
{
    if (IMEContext) {
        ImmSetOpenStatus(IMEContext, 0);
    }
}


class Leaf_738100 { public: void get(); };
void Leaf_738100::get() {  }


// 0x00738110: function not yet identified
//
// Body: 1 bytes (frozen inventory bound).
// Referenced from 0 sites.
// Ghidra prototype: void FUN_00738110(void)
//
// Types and names are Ghidra placeholders;
// rename and retype as the purpose becomes known.
//

namespace seed_00738110 {

void  FUN_00738110(void)
{
    return;
}

}  // namespace seed_00738110


// 0x00738120: function not yet identified
//
// Body: 1 bytes (frozen inventory bound).
// Referenced from 0 sites.
// Ghidra prototype: void FUN_00738120(void)
//
// Types and names are Ghidra placeholders;
// rename and retype as the purpose becomes known.
//

namespace seed_00738120 {

void  FUN_00738120(void)
{
    return;
}

}  // namespace seed_00738120


// wchar_t Char_To_Wide_00738130(unsigned character, long unused_key_data) (0x00738130)
// The native 005F2977 caller also passes key data in EDX; this body does not read it.
// The wide character of a single or double byte character code (lead byte in bits 8-15);
// 0 when MultiByteToWideChar does not produce exactly one character.

unsigned short __fastcall Char_To_Wide_00738130(unsigned int character, long);

unsigned short __fastcall Char_To_Wide_00738130(unsigned int character, long)
{
    char text[3];
    if (character & 0xFF00) {
        text[0] = (char)(character >> 8);
        text[1] = (char)character;
        text[2] = 0;
    } else {
        text[0] = (char)character;
        text[1] = 0;
    }
    unsigned int wide;
    return (MultiByteToWideChar(0, 0, text, strlen(text), (unsigned short *)&wide, 1) == 1) ? wide : 0;
}


// Loads a title picture, redraws its background and draws the paletted/image surface.
Surface *__fastcall unknown_load_pcx_60d110(FileClass *file, PaletteClass *palette, int unknown3, int unknown4);
void __fastcall Fill_Surface_Rect_006f3a20(Surface *surface, Rect const &window, bool draw);
Rect __fastcall Get_Title_Screen_Area_006f1f30();
extern int const title_width_threshold_007add70;
void __fastcall Load_Title_Screen_00738190(char const *name, Surface *surface, PaletteClass *palette)
{
    CCFileClass file(name);
    Surface *image = unknown_load_pcx_60d110(&file, palette, 0, 0);
    if (image) {
        int width = surface->Get_Width();
        int height = surface->Get_Height();
        Rect background_rect(0, 0, width, height);
        BSurface *background = new BSurface(width, height, 2);
        Fill_Surface_Rect_006f3a20(background, background_rect, false);
        surface->Copy_From(*background, false, true);
        delete background;
        int x = 0;
        int y = 0;
        int image_width = image->Get_Width();
        int image_height = image->Get_Height();
        if (surface->Get_Width() > title_width_threshold_007add70) {
            Rect window = Get_Title_Screen_Area_006f1f30();
            x = (window.Width - image_width) / 2;
            y = (window.Height - image_height) / 2;
        }
        if (palette && image->Bytes_Per_Pixel() == 1) {
            ConvertClass *convert = new ConvertClass(*palette, *palette, surface, 1, false);
            unknown_blit_49f3a0(*surface, convert, *image, image->Get_Rect(),
                RectPoint2D(x, y), surface->Get_Rect(), 0, 0, 0, 0, 1000, 0);
            delete convert;
        } else {
            surface->Copy_From(surface->Get_Rect(), Rect(x, y, image->Get_Width(), image->Get_Height()),
                *image, image->Get_Rect(), image->Get_Rect(), false, true);
        }
        delete image;
    }
}


// 0x00738420: function not yet identified
//
// Owner: CCToolTip, virtual slot 0 (vtable +0x0) (from RTTI/vtables).
// Body: 36 bytes (frozen inventory bound).
// Referenced from 1 site (data).
// Ghidra prototype: undefined4 * __thiscall FUN_00738420(undefined4 *param_1,byte param_2)
// Callees with accepted source:
//   FUN_00781d3b -> src/recovered/00781d3b.cpp
//
// Types and names are Ghidra placeholders;
// rename and retype as the purpose becomes known.
//

namespace seed_00738420 {

extern void __fastcall FUN_006eb390(void);
extern void  FUN_00781d3b(unsigned int allocation_address);
extern "C" void* PTR_FUN_007af51c;

struct Owner_00738420 { unsigned int * FUN_00738420(unsigned char deletion_flags); };

unsigned int * Owner_00738420::FUN_00738420(unsigned char deletion_flags)
{
    unsigned int * object_words = (unsigned int *)this;
    *object_words = (unsigned int)(&PTR_FUN_007af51c);
    FUN_006eb390();
    if ((deletion_flags & 1) != 0) {
        FUN_00781d3b((unsigned int)(object_words));
    }
    return object_words;
}

}  // namespace seed_00738420
