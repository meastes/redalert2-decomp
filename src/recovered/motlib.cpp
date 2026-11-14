// motlib.cpp: the original translation unit re-filed as one source (Route A): the accepted
// bodies of its canonical compile group in address order, under the unit's own header
// state (its switches and includes in their original order). The module map
// manifests/original_target/modules/motlib.json carries the addresses and the rows that
// stay in their own files.


#include "shared/win32_api.hpp"
#include "shared/mono_window.hpp"
#include "shared/win32_gdi.hpp"
#define SOUND_PLAY_DECLARE_HANDLE_INIT
#include "shared/audio.hpp"
#include "shared/voc_class.hpp"
#include "shared/vox_class.hpp"
#define VECTOR_IMPLEMENT_DYNAMIC_ACCESSORS
#include "shared/vector_class.hpp"
#include "shared/mono_class.hpp"


// void Mono_Window_Init(void *instance, HWND__ *parent, bool allowed) (0x0059D780)
// Keeps the dialog's instance, the game window and whether the window may show, builds the
// eight pages (the first is the one on display), clears them and shows the window when it
// was asked for before a parent existed.

void __fastcall Mono_Window_Init(void *instance, HWND__ *parent, bool allowed)
{
    unknown_mono_instance_a70f00 = instance;
    unknown_mono_parent_a70f04 = parent;
    unknown_mono_allowed_a70f08 = allowed;
    for (int page = 0; page < 8; page++) {
        unknown_mono_pages_a70e40[page] = new MonoClass(page);
    }
    unknown_mono_viewed_a70f0c = unknown_mono_pages_a70e40[0];
    Mono_Clear_All_Pages();
    if (unknown_mono_requested_a70f09) {
        Mono_Window_Show();
    }
}


// void Mono_Window_Shutdown() (0x0059D900)
// Frees the eight pages, then deletes the font and destroys the dialog.

void Mono_Window_Shutdown()
{
    for (int i = 0; i < 8; i++) {
        if (unknown_mono_pages_a70e40[i] != 0) {
            delete unknown_mono_pages_a70e40[i];
            unknown_mono_pages_a70e40[i] = 0;
        }
    }
    if (unknown_mono_window_a70f10 != 0) {
        DeleteObject(unknown_mono_font_a70f14);
        Unknown_Remove_Modeless_Dialog_00522b80(unknown_mono_window_a70f10);
        DestroyWindow(unknown_mono_window_a70f10);
        unknown_mono_font_a70f14 = 0;
        unknown_mono_window_a70f10 = 0;
    }
}


// void Mono_Window_Show_Requested() (0x0059D970)
// Without a parent window only records the request; otherwise shows the dialog (creating it first).

void Mono_Window_Show_Requested()
{
    if (unknown_mono_parent_a70f04 == 0) {
        unknown_mono_requested_a70f09 = true;
        return;
    }
    if (unknown_mono_allowed_a70f08) {
        if (unknown_mono_window_a70f10 == 0) {
            Mono_Window_Create();
        }
        unknown_mono_enabled_a70f0a = true;
        ShowWindow(unknown_mono_window_a70f10, 1);
        SetFocus(unknown_mono_parent_a70f04);
    }
}


// void Mono_Window_Show() (0x0059D9C0)

void Mono_Window_Show()
{
    if (unknown_mono_allowed_a70f08) {
        if (unknown_mono_window_a70f10 == 0) {
            Mono_Window_Create();
        }
        unknown_mono_enabled_a70f0a = true;
        ShowWindow(unknown_mono_window_a70f10, 1);
        SetFocus(unknown_mono_parent_a70f04);
    }
}


// void Mono_Window_Hide() (0x0059DA00)

void Mono_Window_Hide()
{
    unknown_mono_requested_a70f09 = false;
    if (unknown_mono_window_a70f10 != 0) {
        unknown_mono_enabled_a70f0a = false;
        ShowWindow(unknown_mono_window_a70f10, 0);
    }
}


// Selects and clears a debug page, then refreshes the selected page.

void __fastcall Mono_View_Page(int page)
{
    unknown_mono_viewed_a70f0c = unknown_mono_pages_a70e40[page];
    unknown_mono_viewed_a70f0c->Clear();
    unknown_mono_viewed_a70f0c->Refresh(0, 0, 80, 25);
}


// 0x0059da20: function not yet identified
//
// Body: 27 bytes (frozen inventory bound).
// Referenced from 0 sites.
// Ghidra prototype: undefined4 Mono_Window_Is_Active_0059da20(void)
//
// Types and names are Ghidra placeholders;
// rename and retype as the purpose becomes known.
//

namespace seed_0059da20 {

extern "C" char mono_requested_00a70f09;
extern "C" char mono_enabled_00a70f0a;

unsigned int  Mono_Window_Is_Active_0059da20(void)
{
    if ((mono_enabled_00a70f0a == '\0') && (mono_requested_00a70f09 == '\0')) {
        return 0;
    }
    return 1;
}

}  // namespace seed_0059da20


// Mono_Viewed_Page_0059da40 (0x0059DA40): defined above in this unit; its definition is the row.


// void Mono_Clear_All_Pages() (0x0059DAD0)

void Mono_Clear_All_Pages()
{
    for (int i = 0; i < 8; i++) {
        unknown_mono_pages_a70e40[i]->Clear();
    }
}


// Build the debug text dialog and size its 80-by-25 character client area.

void __fastcall Mono_Window_Create()
{
    unknown_mono_window_a70f10 = CreateDialogParamA(unknown_mono_instance_a70f00, MAKEINTRESOURCEA(248), unknown_mono_parent_a70f04, Mono_Dialog_Proc_0059dce0, 0);
    Unknown_Add_Modeless_Dialog_00522b20(unknown_mono_window_a70f10);
    HDC dc = GetDC(unknown_mono_window_a70f10);
    LOGFONTA font;
    font.lfHeight = -MulDiv(8, GetDeviceCaps(dc, 90), 72);
    font.lfWidth = 0;
    font.lfEscapement = 0;
    font.lfOrientation = 0;
    font.lfWeight = 0;
    font.lfItalic = 0;
    font.lfUnderline = 0;
    font.lfStrikeOut = 0;
    font.lfCharSet = 0;
    font.lfOutPrecision = 0;
    font.lfClipPrecision = 0;
    font.lfQuality = 0;
    font.lfPitchAndFamily = 0;
    strcpy(font.lfFaceName, "Courier");
    unknown_mono_font_a70f14 = CreateFontIndirectA(&font);
    HGDIOBJ previous = SelectObject(dc, unknown_mono_font_a70f14);
    GetTextMetricsA(dc, &unknown_mono_metrics_a70ea0);
    SelectObject(dc, previous);
    unknown_mono_back_color_a70e00 = GetSysColor(15);
    unknown_mono_text_color_a70de8 = GetSysColor(8);
    RECT frame;
    frame.left = 0;
    frame.top = 0;
    frame.right = unknown_mono_metrics_a70ea0.tmMaxCharWidth * 79 + 20;
    frame.bottom = (unknown_mono_metrics_a70ea0.tmExternalLeading + unknown_mono_metrics_a70ea0.tmHeight) * 25 + 20;
    AdjustWindowRect(&frame, 0x00c00000, 0);
    RECT parent;
    GetWindowRect(unknown_mono_parent_a70f04, &parent);
    MoveWindow(unknown_mono_window_a70f10, parent.right + 5, 0, frame.right - frame.left, frame.bottom - frame.top, 1);
    ShowWindow(unknown_mono_window_a70f10, 0);
    SetFocus(unknown_mono_parent_a70f04);
    ReleaseDC(unknown_mono_window_a70f10, dc);
}


extern DynamicVectorClass<VoxClass *> unknown_VoxClass_list_00ad0930;
struct PAINTSTRUCT { HDC hdc; BOOL erase; RECT paint; BOOL restore; BOOL update; unsigned char reserved[32]; };
WIN32_IMPORT HDC WINAPI BeginPaint(HWND window, PAINTSTRUCT *paint);
WIN32_IMPORT BOOL WINAPI EndPaint(HWND window, PAINTSTRUCT const *paint);
void Audio_Stop_All();
extern "C" {
extern AudioEventHandle mono_test_handle_a70e70;
extern AudioEventHandle *mono_test_handle_a70ef4;
extern int mono_voice_index_a70ef0;
extern int mono_voice_priority_a70efc;
extern int mono_stream_count_a70ef8;
extern int mono_audio_toggle_a70eec;
extern AudioStreamPlayer *mono_test_streams_a70dec[5];
extern int mono_voice_count_ad0940;
}
void __fastcall Mono_View_Page(int page);
MonoClass *__fastcall Mono_Page_0059da50(int page) { return unknown_mono_pages_a70e40[page]; }
namespace seed_0059da40 {
extern "C" unsigned int mono_viewed_00a70f0c;
unsigned int Mono_Viewed_Page_0059da40() { return (unsigned int)unknown_mono_viewed_a70f0c; }
}
BOOL __stdcall Mono_Dialog_Proc_0059dce0(HWND dialog, unsigned int message, unsigned int key, long lparam)
{
    if (message==15) {
        PAINTSTRUCT paint;
        InvalidateRect(dialog,0,1);
        BeginPaint(dialog,&paint);
        MonoClass *page=(MonoClass *)seed_0059da40::Mono_Viewed_Page_0059da40();
        if (page) page->View();
        EndPaint(dialog,&paint);
        return 1;
    }
    if (message==258) {
        if (!mono_test_handle_a70ef4) {
            mono_test_handle_a70ef4=&mono_test_handle_a70e70;
            mono_test_handle_a70ef4->Init();
        }
        switch (key) {
        case '1':case '2':case '3':case '4':mono_voice_priority_a70efc=key-'1';return 0;
        case 'Q':case 'q':
            if (mono_voice_index_a70ef0>16 || mono_voice_index_a70ef0>=unknown_VoxClass_list_00ad0930.Count()) mono_voice_index_a70ef0=0;
            Speech_Play(mono_voice_index_a70ef0,1,mono_voice_priority_a70efc);
            ++mono_voice_index_a70ef0;
            break;
        case 'W':case 'w':
            if (mono_voice_index_a70ef0>16 || mono_voice_index_a70ef0>=unknown_VoxClass_list_00ad0930.Count()) mono_voice_index_a70ef0=0;
            Speech_Play(mono_voice_index_a70ef0,0,mono_voice_priority_a70efc);
            ++mono_voice_index_a70ef0;
            return 0;
        case 'E':case 'e':
            if (mono_voice_index_a70ef0>16 || mono_voice_index_a70ef0>=unknown_VoxClass_list_00ad0930.Count()) mono_voice_index_a70ef0=0;
            Speech_Play(mono_voice_index_a70ef0,2,mono_voice_priority_a70efc);
            ++mono_voice_index_a70ef0;
            return 0;
        case 'O':case 'o':
            if (mono_voice_index_a70ef0>16 || mono_voice_index_a70ef0>=unknown_VoxClass_list_00ad0930.Count()) mono_voice_index_a70ef0=0;
            Speech_Play(mono_voice_index_a70ef0,-1,-1);
            ++mono_voice_index_a70ef0;
            return 0;
        case 'L':case 'l':
            if (Audio_Event_Handle_Event(mono_test_handle_a70ef4)) Audio_Event_Handle_Finish(mono_test_handle_a70ef4);
            else Voc_Play(VocClass::From_Name("TestEnvelopeFShift"),1.0f,0x2000,mono_test_handle_a70ef4);
            return 0;
        case 'T':case 't':Voc_Play(VocClass::From_Name("TestEffect"),1.0f,0x2000,0);return 0;
        case '[':
            if (mono_stream_count_a70ef8<5) {
                AudioStreamPlayer *stream=Audio_Stream_Create(Audio_Get_Device(),5000);
                mono_test_streams_a70dec[mono_stream_count_a70ef8]=stream;
                if (stream) {
                    if (Audio_Stream_Open(stream,"intro.wav",1)) ++mono_stream_count_a70ef8;
                    else Audio_Stream_Destroy(mono_test_streams_a70dec[mono_stream_count_a70ef8]);
                }
            }
            return 0;
        case ']':
            if (mono_stream_count_a70ef8>0) {
                Audio_Stream_Stop(mono_test_streams_a70dec[--mono_stream_count_a70ef8]);
                Audio_Stream_Destroy(mono_test_streams_a70dec[mono_stream_count_a70ef8]);
            }
            return 0;
        case 'N':case 'n':mono_audio_toggle_a70eec=!mono_audio_toggle_a70eec;return 0;
        case 'S':case 's':Audio_Stop_All();return 0;
        case 'A':case 'a':
            Mono_View_Page(5);return 0;
        case 'I':case 'i':
            Mono_View_Page(7);return 0;
        case 'M':case 'm':
            Mono_View_Page(6);return 0;
        case ',':case '<': {
            int page=((MonoClass *)seed_0059da40::Mono_Viewed_Page_0059da40())->Get_Handle()+1;
            if (page==8)page=0;
            unknown_mono_viewed_a70f0c=Mono_Page_0059da50(page);
            ((MonoClass *)seed_0059da40::Mono_Viewed_Page_0059da40())->Clear();
            ((MonoClass *)seed_0059da40::Mono_Viewed_Page_0059da40())->Refresh(0,0,80,25);return 0;
        }
        case '.':case '>': {
            int page=((MonoClass *)seed_0059da40::Mono_Viewed_Page_0059da40())->Get_Handle()-1;
            if (page<0)page=7;
            unknown_mono_viewed_a70f0c=Mono_Page_0059da50(page);
            ((MonoClass *)seed_0059da40::Mono_Viewed_Page_0059da40())->Clear();
            ((MonoClass *)seed_0059da40::Mono_Viewed_Page_0059da40())->Refresh(0,0,80,25);return 0;
        }
        }
    }
    if (message==273) {
        Mono_Window_Hide();
        return 1;
    }
    return 0;
}


// MonoPoint::MonoPoint(int column, int row) (0x0059E160)
// The pixel position of a character cell: the cell size from the font
// metrics, with a margin of ten pixels.

MonoPoint::MonoPoint(int column, int row)
{
    Point2D p(unknown_mono_width_a70eb8 * column + 10,
              (unknown_mono_height_a70ea0 + unknown_mono_leading_a70eb0) * row + 10);
    X = p.X;
    Y = p.Y;
}


// MonoClass::MonoClass(int handle) (0x0059E190)
// Starts a blank 80 x 25 text window at the top left of the screen.

MonoClass::MonoClass(int handle)
{
    Handle = handle;
    X = 0;
    Y = 0;
    WindowX = 0;
    WindowY = 0;
    WindowWidth = 80;
    WindowHeight = 25;
    Clear();
}


void MonoClass::Set_Cursor(int x, int y)
{
    X = x;
    Y = y;
}


void MonoClass::Clear()
{
    for (int y = 0; y < 25; y++) {
        for (int x = 0; x < 80; x++) {
            Characters[x][y] = ' ';
            Attributes[x][y] = 0;
        }
    }
    Changed = true;
    Refresh(0, 0, 80, 25);
}


// 0x0059e220: function not yet identified
//
// Body: 1 bytes (frozen inventory bound).
// Referenced from 0 sites.
// Ghidra prototype: void unknown_empty_0059e220(void)
//
// Types and names are Ghidra placeholders;
// rename and retype as the purpose becomes known.
//

namespace seed_0059e220 {

void  unknown_empty_0059e220(void)
{
    return;
}

}  // namespace seed_0059e220


// 0x0059e230: function not yet identified
//
// Body: 7 bytes (frozen inventory bound).
// Referenced from 0 sites.
// Ghidra prototype: undefined4 __fastcall unknown_get_2718_0059e230(int param_1)
//
// Types and names are Ghidra placeholders;
// rename and retype as the purpose becomes known.
//

namespace seed_0059e230 {

unsigned int __fastcall unknown_get_2718_0059e230(int self)
{
    return *(unsigned int *)(self + 0x2718);
}

}  // namespace seed_0059e230


// 0x0059e260: function not yet identified
//
// Body: 43 bytes (frozen inventory bound).
// Referenced from 0 sites.
// Ghidra prototype: void __thiscall unknown_set_2724_0059e260(int param_1,undefined4 param_2,undefined4 param_3,undefined4 param_4,undefined4 param_5 )
//
// Types and names are Ghidra placeholders;
// rename and retype as the purpose becomes known.
//

namespace seed_0059e260 {

struct Owner_0059e260 { void unknown_set_2724_0059e260(unsigned int a, unsigned int b, unsigned int c, unsigned int d); };

void Owner_0059e260::unknown_set_2724_0059e260(unsigned int a, unsigned int b, unsigned int c, unsigned int d)
{
    int self = (int)this;
    *(unsigned int *)(self + 0x2724) = a;
    *(unsigned int *)(self + 0x2728) = b;
    *(unsigned int *)(self + 0x272c) = c;
    *(unsigned int *)(self + 0x2730) = d;
    return;
}

}  // namespace seed_0059e260


// 0x0059e2e0: function not yet identified
//
// Body: 7 bytes (frozen inventory bound).
// Referenced from 0 sites.
// Ghidra prototype: undefined1 __fastcall unknown_get_2734_0059e2e0(int param_1)
//
// Types and names are Ghidra placeholders;
// rename and retype as the purpose becomes known.
//

namespace seed_0059e2e0 {

unsigned char __fastcall unknown_get_2734_0059e2e0(int self)
{
    return *(unsigned char *)(self + 0x2734);
}

}  // namespace seed_0059e2e0


// void MonoClass::View() (0x0059E2F0)
// Shows this window: a window that was not on display is cleared first.

void MonoClass::View()
{
    if (unknown_mono_enabled_a70f0a) {
        if (unknown_mono_viewed_a70f0c != this) {
            unknown_mono_viewed_a70f0c = this;
            Clear();
        }
        Refresh(0, 0, 80, 25);
    }
}


// 0x0059e370: function not yet identified
//
// Body: 22 bytes (frozen inventory bound).
// Referenced from 0 sites.
// Ghidra prototype: void __thiscall Set_Default_Attribute_0059e370(int param_1,undefined4 param_2)
//
// Types and names are Ghidra placeholders;
// rename and retype as the purpose becomes known.
//

namespace seed_0059e370 {

extern "C" char mono_enabled_00a70f0a;

struct Owner_0059e370 { void Set_Default_Attribute_0059e370(unsigned int attrib); };

void Owner_0059e370::Set_Default_Attribute_0059e370(unsigned int attrib)
{
    int self = (int)this;
    if (mono_enabled_00a70f0a != '\0') {
        *(unsigned int *)(self + 10000) = attrib;
    }
    return;
}

}  // namespace seed_0059e370


// void MonoClass::Fill_Attrib(int x, int y, int width, int height, int attribute) (0x0059E390)
// Sets the attribute of a block of the window's cells (window relative) and redraws the block.

void MonoClass::Fill_Attrib(int x, int y, int width, int height, int attribute)
{
    if (unknown_mono_enabled_a70f0a) {
        int left = WindowX + x;
        int top = WindowY + y;
        int right = left + width;
        int bottom = top + height;
        for (int row = top; row < bottom; row++) {
            for (int column = left; column < right; column++) {
                Attributes[column][row] = attribute;
            }
        }
        Refresh(left, top, width, height);
    }
}


// void MonoClass::Scroll(int lines) (0x0059E410)
// Moves the window's text one line up (positive) or down (negative) and blanks
// the line that comes free.

void MonoClass::Scroll(int lines)
{
    if (unknown_mono_enabled_a70f0a) {
        int x = WindowX;
        int y = WindowY;
        int width = WindowWidth;
        int height = WindowHeight;
        if (lines > 0) {
            for (int column = x; column < x + width; column++) {
                for (int row = y; row < y + height - 1; row++) {
                    Characters[column][row] = Characters[column][row + 1];
                    Attributes[column][row] = Attributes[column][row + 1];
                }
                Characters[column][y + height - 1] = ' ';
                Attributes[column][y + height - 1] = 0;
            }
        } else if (lines < 0) {
            for (int column = x; column < x + width; column++) {
                for (int row = y + height - 1; row > y; row--) {
                    Characters[column][row] = Characters[column][row - 1];
                    Attributes[column][row] = Attributes[column][row - 1];
                }
                Characters[column][y] = ' ';
                Attributes[column][y] = 0;
            }
        }
        Refresh(0, 0, 80, 25);
    }
}


// void MonoClass::Pan(int columns) (0x0059E590)
// Moves the window's text one column left (positive) or right (negative) and
// blanks the column that comes free.

void MonoClass::Pan(int columns)
{
    if (unknown_mono_enabled_a70f0a) {
        int x = WindowX;
        int width = WindowWidth;
        int y = WindowY;
        int height = WindowHeight;
        if (columns > 0) {
            for (int row = y; row < y + height; row++) {
                for (int column = x; column < x + width - 1; column++) {
                    Characters[column][row] = Characters[column + 1][row];
                    Attributes[column][row] = Attributes[column + 1][row];
                }
                Characters[x + width - 1][row] = ' ';
                Attributes[x + width - 1][row] = 0;
            }
        } else if (columns < 0) {
            for (int row = y; row < y + height; row++) {
                for (int column = x + width - 1; column > x; column--) {
                    Characters[column][row] = Characters[column - 1][row];
                    Attributes[column][row] = Attributes[column - 1][row];
                }
                Characters[x][row] = ' ';
                Attributes[x][row] = 0;
            }
        }
        Refresh(0, 0, 80, 25);
    }
}


// SPDX-License-Identifier: GPL-3.0-or-later
// Copyright 2026 ra2-decomp contributors
// Copyright 2025 Electronic Arts Inc.; 2026 OpenTS contributors
// Derived from Electronic Arts source code; EA's GPL Section 7 terms apply (COPYING.md).
// void MonoClass::Print(char const *text) (0x0059E6D0)
// Adopted from ra2-decomp DebugPage::Print_Raw (YR 0x005BD0D0).
// Writes text at the cursor inside the window (clipped to the window width)
// in the current attribute, moving the cursor (a newline returns it to the
// window's left edge one row down), then redraws the written span.

void MonoClass::Print(char const *text)
{
    if (unknown_mono_enabled_a70f0a) {
        int cx = X;
        int start = WindowX + cx;
        int row = WindowY + Y;
        int limit = WindowWidth + WindowX;
        Changed = false;
        int end = (int)strlen(text) + start;
        if (end > limit) {
            end = limit;
        }
        for (int col = start, i = 0; col < end; col++, i++) {
            char c = text[i];
            Characters[col][row] = Map_Character(c);
            Attributes[col][row] = Attribute;
            if (c == '\n') {
                X = 0;
                Y++;
            } else {
                X++;
            }
        }
        Refresh(start, row, end - start, 1);
    }
}


// void MonoClass::Printf(char const *format, ...) (0x0059E7F0)

void MonoClass::Printf(char const *format, ...)
{
    if (unknown_mono_enabled_a70f0a) {
        char buffer[256];
        vsprintf(buffer, format, (char *)(&format + 1));
        buffer[255] = 0;
        Print(buffer);
    }
}


// void MonoClass::Printf(wchar_t const *format, ...) (0x0059E840)

void MonoClass::Printf(wchar_t const *format, ...)
{
    if (unknown_mono_enabled_a70f0a) {
        wchar_t wide[256];
        char buffer[256];
        vswprintf(wide, format, (char *)(&format + 1));
        sprintf(buffer, "%S", wide);
        Print(buffer);
    }
}


// void MonoClass::Text_Print(char const *text, int x, int y, int attribute) (0x0059E8A0)
// Puts the cursor at a place, picks the attribute and prints.

void MonoClass::Text_Print(char const *text, int x, int y, int attribute)
{
    if (unknown_mono_enabled_a70f0a) {
        Set_Cursor(x, y);
        Set_Attribute(attribute);
        Print(text);
    }
}


// 0x0059e8e0: function not yet identified
//
// Body: 114 bytes (frozen inventory bound).
// Referenced from 0 sites.
// Ghidra prototype: void __thiscall Text_Print_0059e8e0(int param_1,undefined4 param_2,undefined4 param_3,undefined4 param_4,undefined4 param_5 )
//
// Types and names are Ghidra placeholders;
// rename and retype as the purpose becomes known.
//

namespace seed_0059e8e0 {

struct Callee_0059e6d0 { void Print_0059e6d0(char *); };
extern unsigned int  sprintf_007820f4(int, int, int);
extern "C" unsigned int text_print_format_007e2af0;
extern "C" char mono_enabled_00a70f0a;

struct Owner_0059e8e0 { void Text_Print_0059e8e0(unsigned int text, unsigned int x, unsigned int y, unsigned int attrib); };

void Owner_0059e8e0::Text_Print_0059e8e0(unsigned int text, unsigned int x, unsigned int y, unsigned int attrib)
{
    int self = (int)this;
    unsigned char buffer [1024];

    if (mono_enabled_00a70f0a != '\0') {
        *(unsigned int *)(self + 0x271c) = x;
        *(unsigned int *)(self + 0x2720) = y;
        if (mono_enabled_00a70f0a != '\0') {
            *(unsigned int *)(self + 10000) = attrib;
        }
        sprintf_007820f4((int)(buffer), (int)(&text_print_format_007e2af0), (int)(text));
        ((Callee_0059e6d0*)(self))->Print_0059e6d0((char *)(buffer));
    }
    return;
}

}  // namespace seed_0059e8e0


// 0x0059e960: function not yet identified
//
// Body: 101 bytes (frozen inventory bound).
// Referenced from 1 site (unconditional call).
// Ghidra prototype: char Map_Character_0059e960(char param_1)
//
// Types and names are Ghidra placeholders;
// rename and retype as the purpose becomes known.
//

namespace seed_0059e960 {

char __stdcall Map_Character_0059e960(char character)
{
    if (character == '\n') {
        return ' ';
    }
    if (character == -0x4d) {
        return '|';
    }
    if (character == -0x3c) {
        return '-';
    }
    if (character == -0x33) {
        return '=';
    }
    if (((((character == -0x26) || (character == -0x3e)) || (character == -0x41)) ||
            ((((character == -0x3a || (character == -0x3d)) ||
                ((character == -0x2f || ((character == -0x3b || (character == -0x28)))))) || (character == -0x4b))))
          || ((((character == -0x40 || (character == -0x3f)) || (character == -0x27)) ||
                  ((character == -0x2b || (character == -0x4c)))))) {
        character = (char)('+');
    }
    return character;
}

}  // namespace seed_0059e960


// void MonoClass::Draw_Character(HDC dc, int x, int y) (0x0059E9D0)
// Draws one cell, switching the device context's colours when its attribute
// differs from the last one drawn (attribute 1 is inverse video).

void MonoClass::Draw_Character(HDC dc, int x, int y)
{
    int attribute = Attributes[x][y];
    if (attribute != DrawnAttribute) {
        if (attribute == 1) {
            SetBkColor(dc, unknown_mono_text_color_a70de8);
            SetTextColor(dc, unknown_mono_back_color_a70e00);
        } else {
            SetBkColor(dc, unknown_mono_back_color_a70e00);
            SetTextColor(dc, unknown_mono_text_color_a70de8);
        }
        DrawnAttribute = attribute;
    }
    char character = Characters[x][y];
    MonoPoint point(x, y);
    TextOutA(dc, point.X, point.Y, &character, 1);
}


// void MonoClass::Refresh(int x, int y, int width, int height) (0x0059EA90)
// Redraws a block of cells on the game window if this is the window on display.

void MonoClass::Refresh(int x, int y, int width, int height)
{
    if (unknown_mono_enabled_a70f0a && this == unknown_mono_viewed_a70f0c) {
        HDC dc = GetDC(unknown_mono_window_a70f10);
        SetBkMode(dc, 2);
        SetBkColor(dc, unknown_mono_back_color_a70e00);
        SetTextColor(dc, unknown_mono_text_color_a70de8);
        if (dc != 0) {
            HGDIOBJ old_font = SelectObject(dc, unknown_mono_font_a70f14);
            for (int row = y; row < y + height; row++) {
                for (int column = x; column < x + width; column++) {
                    Draw_Character(dc, column, row);
                }
            }
            SelectObject(dc, old_font);
            ReleaseDC(unknown_mono_window_a70f10, dc);
        }
    }
}


// 0x0059eba0: function not yet identified
//
// Body: 28 bytes (frozen inventory bound).
// Referenced from 18 sites (unconditional call).
// Ghidra prototype: void __fastcall MotionLibrary_dtor_0059eba0(int param_1)
// Callees with accepted source:
//   operator_delete_00781d3b -> src/recovered/00781d3b.cpp
//
// Types and names are Ghidra placeholders;
// rename and retype as the purpose becomes known.
//

namespace seed_0059eba0 {

extern void  operator_delete_00781d3b(unsigned int);

void __fastcall MotionLibrary_dtor_0059eba0(int self)
{
    if (*(int *)(self + 0xc) != 0) {
        operator_delete_00781d3b((unsigned int)(*(int *)(self + 0xc)));
    }
    *(unsigned int *)(self + 0xc) = 0;
    return;
}

}  // namespace seed_0059eba0


// 0x0059edc0: function not yet identified
//
// Body: 28 bytes (frozen inventory bound).
// Referenced from 0 sites.
// Ghidra prototype: void __fastcall Clear_0059edc0(int param_1)
// Callees with accepted source:
//   operator_delete_00781d3b -> src/recovered/00781d3b.cpp
//
// Types and names are Ghidra placeholders;
// rename and retype as the purpose becomes known.
//

namespace seed_0059edc0 {

extern void  operator_delete_00781d3b(unsigned int);

void __fastcall Clear_0059edc0(int self)
{
    if (*(int *)(self + 0xc) != 0) {
        operator_delete_00781d3b((unsigned int)(*(int *)(self + 0xc)));
    }
    *(unsigned int *)(self + 0xc) = 0;
    return;
}

}  // namespace seed_0059edc0
