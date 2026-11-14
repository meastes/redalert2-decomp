// gscreen.cpp: the original translation unit re-filed as one source (Route A): the accepted
// bodies of its canonical compile group in address order, under the unit's own header
// state (its switches and includes in their original order). The module map
// manifests/original_target/modules/gscreen.json carries the addresses and the rows that
// stay in their own files.


#include "shared/shape_file.hpp"
#include "shared/unknown_geometry.hpp"
#include "shared/gscreen.hpp"
#include "shared/map_view.hpp"
#include "shared/mouse.hpp"
#include "shared/keyboard.hpp"
#include "shared/surface.hpp"
#include "shared/keyboard_game.hpp"
#include "shared/tool_tip.hpp"
#include "shared/win32_api.hpp"
#include "shared/crt_memory.hpp"
#include "shared/cd_timer.hpp"


// Header-static dynamic initializers of the unit at 0x004E06E0: the shape_file.hpp empty rect and the
// shared/unknown_geometry.hpp constants (0x004E06E0-0x004E08C1); the other
// drafts of this plan are emitted by these includes.


// Dynamic initializer (0x004E0700, _$E) of diagonal from the header(s) included
// by HeaderStatics_empty_rect_init_004e06e0.cpp; no source of its own.


// Dynamic initializer (0x004E0730, _$E) of b from the header(s) included
// by HeaderStatics_empty_rect_init_004e06e0.cpp; no source of its own.


// Dynamic initializer (0x004E0750, _$E) of c from the header(s) included
// by HeaderStatics_empty_rect_init_004e06e0.cpp; no source of its own.


// Dynamic initializer (0x004E0770, _$E) of angle45 from the header(s) included
// by HeaderStatics_empty_rect_init_004e06e0.cpp; no source of its own.


// Dynamic initializer (0x004E0790, _$E) of angle60 from the header(s) included
// by HeaderStatics_empty_rect_init_004e06e0.cpp; no source of its own.


// Dynamic initializer (0x004E07B0, _$E) of angle90 from the header(s) included
// by HeaderStatics_empty_rect_init_004e06e0.cpp; no source of its own.


// Dynamic initializer (0x004E07D0, _$E) of g from the header(s) included
// by HeaderStatics_empty_rect_init_004e06e0.cpp; no source of its own.


// Dynamic initializer (0x004E0810, _$E) of h from the header(s) included
// by HeaderStatics_empty_rect_init_004e06e0.cpp; no source of its own.


// Dynamic initializer (0x004E0840, _$E) of i from the header(s) included
// by HeaderStatics_empty_rect_init_004e06e0.cpp; no source of its own.


// Dynamic initializer (0x004E0870, _$E) of j from the header(s) included
// by HeaderStatics_empty_rect_init_004e06e0.cpp; no source of its own.


// Dynamic initializer (0x004E08A0, _$E) of k from the header(s) included
// by HeaderStatics_empty_rect_init_004e06e0.cpp; no source of its own.


// Dynamic initializer (0x004E08B0, _$E) of l from the header(s) included
// by HeaderStatics_empty_rect_init_004e06e0.cpp; no source of its own.


// GScreenClass::GScreenClass() (0x004E08D0)
// Clears the shake offsets and asks for a complete first redraw.

GScreenClass::GScreenClass() : ShakeX(0), ShakeY(0), IsToRedraw(2)
{
}


// HRESULT __stdcall GScreenClass::QueryInterface(GUID const &iid, void **object) (0x004E08F0, slot 0)
// IUnknown and IGameMap give the screen itself.

HRESULT __stdcall GScreenClass::QueryInterface(GUID const &iid, void **object)
{
    if (object == 0) {
        return E_POINTER;
    }
    if (!(iid == IID_IUnknown_7afce0) && !(iid == IID_IGameMap)) {
        return E_NOINTERFACE;
    }
    *object = this;
    AddRef();
    return S_OK;
}


// void GScreenClass::One_Time() (0x004E0950, slot 5)
// Starts with an empty button list.

void GScreenClass::One_Time()
{
    Buttons = 0;
}


// void GScreenClass::Init(int theater) (0x004E0960, slot 6)
// Resets the redraw state and the input gadgets.

void GScreenClass::Init(int theater)
{
    Init_Clear();
    Init_IO();
}


// void GScreenClass::Init_Clear() (0x004E0980, slot 7)
// Asks for a complete redraw.

void GScreenClass::Init_Clear()
{
    IsToRedraw = 2;
}


// void GScreenClass::Init_IO() (0x004E0990, slot 8)
// Drops the button list.

void GScreenClass::Init_IO()
{
    Buttons = 0;
}


// void GScreenClass::Flag_To_Redraw(int complete) (0x004E09A0, slot 14)
// Marks the tactical view dirty; a nonzero request is remembered unless a
// complete redraw is already pending, and the screen object is notified.

void GScreenClass::Flag_To_Redraw(int complete)
{
    if (unknown_tactical_83996c != 0) {
        unknown_tactical_83996c->unknown_d7d = true;
    }
    if (complete) {
        if (IsToRedraw != 2) {
            IsToRedraw = complete;
        }
        Map.Unknown_0055ACC0();
    }
}


/*******************************************************************************
 *                                O P E N  T S
 *******************************************************************************
 * SPDX-License-Identifier: GPL-3.0-or-later
 * Copyright 2025 Electronic Arts Inc.
 * Copyright 2026 OpenTS contributors
 *
 * Contains material derived from Electronic Arts source code.
 * Modified by OpenTS contributors, 2026.
 * EA's GPLv3 Section 7 additional terms and supplemental warranty
 * disclaimers apply; see LICENSE.md.
 ******************************************************************************/
// Adapted from TibSun gscreen.cpp Input to the independently bounded RA2 body.
void GScreenClass::Get_Input(int &input, int &x, int &y)
{
    x = WWMouse->Get_Mouse_X();
    y = WWMouse->Get_Mouse_Y();
    if (Buttons != 0) {
        if (Buttons->Is_List_To_Redraw()) Flag_To_Redraw();
        Surface *oldpage = unknown_surface_83995c;
        unknown_surface_83995c = unknown_surface_839954;
        input = Buttons->Input();
        unknown_surface_83995c = oldpage;
    } else {
        input = ((KeyboardClass *)WWKeyboard)->Check();
        if (input != 0) input = ((KeyboardClass *)WWKeyboard)->Get();
    }
    AI(input, Point2D(x, y));
}


// bool GScreenClass::Is_Button_Listed(GadgetClass &gadget) (0x004E0AA0, slot 11)
// The gadget is in the button list when the list starts at Buttons.

bool GScreenClass::Is_Button_Listed(GadgetClass &gadget)
{
    return Buttons == &gadget.Head_Of_List();
}


// bool GScreenClass::Add_A_Button(GadgetClass &gadget) (0x004E0AC0, slot 12)
// Appends gadget to the button list (or starts the list with it); false
// when it is already listed.

bool GScreenClass::Add_A_Button(GadgetClass &gadget)
{
    if (Is_Button_Listed(gadget)) {
        return false;
    }
    if (Buttons != 0) {
        gadget.Add_Tail(*Buttons);
        return true;
    }
    Buttons = &gadget;
    return true;
}


// bool GScreenClass::Remove_A_Button(GadgetClass &gadget) (0x004E0B00, slot 13)
// Unlinks a listed gadget; the list continues at what Remove returns.

bool GScreenClass::Remove_A_Button(GadgetClass &gadget)
{
    if (!Is_Button_Listed(gadget)) {
        return false;
    }
    Buttons = (GadgetClass *)gadget.Remove();
    return true;
}


// void GScreenClass::Render() (0x004E0B30, slot 15)
// Composes the frame on the 0x00839964 surface (mouse removed): the three
// tactical passes around Draw_It, the buttons and the overlays, then puts
// the mouse back and blits the result.

void GScreenClass::Render()
{
    Surface *old = unknown_surface_83995c;
    unknown_surface_83995c = unknown_surface_839964;
    WWMouse->Erase_Mouse(unknown_surface_839964, false);

    bool any = IsToRedraw != 0;
    bool complete = IsToRedraw == 2;
    IsToRedraw = 0;
    unknown_tactical_83996c->Render(unknown_surface_839964, any, 0);
    unknown_tactical_83996c->Render(unknown_surface_839964, any, 1);
    Draw_It(complete);
    unknown_tactical_83996c->Render(unknown_surface_839964, any, 2);

    if (unknown_flag_abcd85 && !Debug_Map) {
        WWMouse->Erase_Mouse(unknown_surface_839948, true);
        unknown_flag_abcd85 = false;
    }
    if (Buttons != 0) {
        Buttons->Draw_All(false);
    }
    unknown_object_a3dc70.Unknown_005B5B30();
    if (ToolTips != 0) {
        ToolTips->Refresh(false);
    }
    if (unknown_flag_a3d8c4) {
        Unknown_005419E0(false);
    }
    WWMouse->Draw_Mouse(unknown_surface_839964, false);
    Blit_Display();
    unknown_surface_83995c = old;
}


// void GScreenClass::Blit_Display() (0x004E0C50, slot 17)
// Flushes the 3D device (an empty scene) when it is in use, then copies the
// composed surface to the screen without the mouse.

void GScreenClass::Blit_Display()
{
    if (unknown_flag_852fd7 && unknown_device_852fe0->BeginScene() == 0) {
        unknown_device_852fe0->EndScene();
    }
    Blit(false, unknown_surface_839964);
}


// void __fastcall GScreenClass::Blit(bool mouse, Surface *surface, Rect const *area) (0x004E0C90)
// Westwood's Update_Visible_Surface: copies area (the whole surface when null) of surface to the window's
// client area, offset by the screen shake, moved past a left sidebar, scaled by the tactical zoom, with
// the strips the shake uncovers cleared; mouse draws the cursor over the copy for 50 ms.

void __fastcall GScreenClass::Blit(bool mouse, Surface *surface, Rect const *area)
{
    Rect rect;
    if (area == 0) {
        rect = surface->Get_Rect();
        area = &rect;
    }

    RECT client_rect;
    if (GetClientRect(MainWindow, &client_rect)) {

    POINT screen_pt;
    screen_pt.x = client_rect.left;
    screen_pt.y = client_rect.top;
    if (ClientToScreen(MainWindow, &screen_pt)) {

    Rect dest_rect(screen_pt.x, screen_pt.y, surface->Get_Width(), surface->Get_Height());

    if (Map.ShakeX == 0 && Map.ShakeY == 0) {
    } else {
        if (Map.ShakeX > 0) {
            dest_rect.X += Map.ShakeX;
            dest_rect.Width -= Map.ShakeX;
        } else if (Map.ShakeX < 0) {
            dest_rect.Width += Map.ShakeX;
        }
        if (Map.ShakeY > 0) {
            dest_rect.Y += Map.ShakeY;
            dest_rect.Height -= Map.ShakeY;
        } else if (Map.ShakeY < 0) {
            dest_rect.Height += Map.ShakeY;
        }
    }

    if (!unknown_flag_a40b30 && !Debug_Map) {
        dest_rect.X += (((unknown_surface_839948->Get_Width() < unknown_int_839648 - dest_rect.Width) ? unknown_surface_839948->Get_Width() : unknown_int_839648 - dest_rect.Width) > 0)
            ? ((unknown_surface_839948->Get_Width() < unknown_int_839648 - dest_rect.Width) ? unknown_surface_839948->Get_Width() : unknown_int_839648 - dest_rect.Width)
            : 0;
    }

    Rect src_rect = *area;
    if (unknown_tactical_83996c != 0 && unknown_tactical_83996c->unknown_c0 != 1.0) {
        int zoom_surface_width = surface->Get_Width();
        int zoom_surface_height = surface->Get_Height();
        Rect tmp;
        double zoomed_width = (double)zoom_surface_width / unknown_tactical_83996c->unknown_c0;
        tmp.X = (int)(((double)zoom_surface_width - zoomed_width) / 2.0);
        double zoomed_height = (double)zoom_surface_height / unknown_tactical_83996c->unknown_c0;
        tmp.Y = (int)(((double)zoom_surface_height - zoomed_height) / 2.0);
        tmp.Width = (int)zoomed_width;
        tmp.Height = (int)zoomed_height;
        src_rect = tmp;
    } else {
        if (src_rect.Width >= dest_rect.Width) {
            src_rect.Width = dest_rect.Width;
        }
        if (src_rect.Height >= dest_rect.Height) {
            src_rect.Height = dest_rect.Height;
        }
    }

    if (Map.ShakeY < 0) {
        src_rect.Y -= Map.ShakeY;
    }
    if (Map.ShakeX < 0) {
        src_rect.X -= Map.ShakeX;
    }

    if (unknown_flag_a40380) {
        bool active = ToolTips != 0 && ToolTips->Is_Active();
        if (!unknown_flag_8373f6 && Map.ShakeY == 0 && !active) {
            dest_rect.Height -= unknown_ac11fc->unknown_0c;
            src_rect.Height -= unknown_ac11fc->unknown_0c;
        }
    }

    if (Map.ShakeX != 0) {
        Rect fill_rect;
        fill_rect.Set(fill_rect.X, screen_pt.y, abs(Map.ShakeX), surface->Get_Height() - unknown_ac11fc->unknown_0c);
        fill_rect.X = Map.ShakeX < 0 ? dest_rect.X + dest_rect.Width : dest_rect.X - Map.ShakeX;
        unknown_surface_839950->Fill_Rect(unknown_surface_839950->Get_Rect(), fill_rect, 0);
    }
    if (Map.ShakeY != 0) {
        Rect fill_rect;
        fill_rect.Set(screen_pt.x, fill_rect.Y, surface->Get_Width(), abs(Map.ShakeY));
        fill_rect.Y = Map.ShakeY < 0 ? dest_rect.Y + dest_rect.Height : dest_rect.Y - Map.ShakeY;
        unknown_surface_839950->Fill_Rect(unknown_surface_839950->Get_Rect(), fill_rect, 0);
    }

    if (mouse && WWMouse != 0) {
        WWMouse->Draw_Mouse(surface, false);
    }

    unknown_surface_839950->Copy_From(dest_rect, *surface, src_rect, false, unknown_int_a40d34 == 0);

    if (mouse && WWMouse != 0) {
        Sleep(50);
        WWMouse->Erase_Mouse(surface, false);
    }
    }
    }
}


// void GScreenClass::AI(int &input, Point2D const &xy) (0x004E10C0, slot 10)
// Damps the screen shake every other frame: each offset swaps sign and
// moves one step toward zero.

void GScreenClass::AI(int &input, Point2D const &xy)
{
    if (!(Frame & 1)) {
        if (ShakeX < 0) {
            ShakeX = -1 - ShakeX;
        } else if (ShakeX > 0) {
            ShakeX = 1 - ShakeX;
        }
        if (ShakeY < 0) {
            ShakeY = -1 - ShakeY;
        } else if (ShakeY > 0) {
            ShakeY = 1 - ShakeY;
        }
    }
}


// GScreenClass scalar deleting destructor (0x004E1110, slot 4)
// Compiler generated after the unit's last member: the inlined empty
// destructor (stores the GScreenClass vtable), then operator delete when
// bit 0 of the flags is set.


// 0x004e1290: function not yet identified
//
// Body: 2 bytes (frozen inventory bound).
// Referenced from 4 sites (unconditional call, unconditional jump).
// Ghidra prototype: void FUN_004e1290(void)
//
// Types and names are Ghidra placeholders;
// rename and retype as the purpose becomes known.
//

namespace seed_004e1290 {

void  FUN_004e1290(void)
{
    do {
                                        /* WARNING: Do nothing block with infinite loop */
    } while( true );
}

}  // namespace seed_004e1290
