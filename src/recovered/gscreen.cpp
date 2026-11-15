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
// Function comment blocks from mischa85/RedAlert2 392e904 src/gscreen.cpp (matching RA2 1.006 decomp), placed above our
// own notes; code unchanged.

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


/***********************************************************************************************
 * GScreenClass::GScreenClass -- Default constructor for GScreenClass.                         *
 *                                                                                             *
 *    This constructor merely sets the display system, so that it will redraw the first time   *
 *    the render function is called.                                                           *
 *                                                                                             *
 * INPUT:   none                                                                               *
 *                                                                                             *
 * OUTPUT:  none                                                                               *
 *                                                                                             *
 * WARNINGS:   none                                                                            *
 *                                                                                             *
 * HISTORY:                                                                                    *
 *   12/15/1994 JLB : Created.                                                                 *
 *=============================================================================================*/
// (0x004E08D0)
// Clears the shake offsets and asks for a complete first redraw.

GScreenClass::GScreenClass() : ShakeX(0), ShakeY(0), IsToRedraw(2)
{
}


/// <summary>
/// Fetches a requested interface from the game screen.
/// The game screen publishes itself as IGameMap so that the scenario and save game code can
/// reach the map through COM. Any other interface is refused.
/// </summary>
/// <param name="riid">The identifier of the interface being asked for.</param>
/// <param name="ppvObject">Pointer to the interface pointer to fill in.</param>
/// <returns>Returns with S_OK and a counted reference on success. Otherwise, E_POINTER or
/// E_NOINTERFACE is returned.</returns>
// (0x004E08F0, slot 0)
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


/***********************************************************************************************
 * GScreenClass::One_Time -- Handles one time class setups.                                    *
 *                                                                                             *
 * This routine (and all those that overload it) must perform truly one-time initialization.   *
 * Such init's would normally be done in the constructor, but other aspects of the game may    *
 * not have been initialized at the time the constructors are called (such as the file system, *
 * the display, or other WWLIB subsystems), so many initializations should be deferred to the  *
 * One_Time init's.                                                                            *
 *                                                                                             *
 * Any variables set in this routine should be declared as static, so they won't be modified   *
 * by the load/save process.  Non-static variables will be over-written by a loaded game.      *
 *                                                                                             *
 * This function allocates the shadow buffer that is used for quick screen updates. If         *
 * there were any data files to load, they would be loaded at this time as well.               *
 *                                                                                             *
 * INPUT:   none                                                                               *
 *                                                                                             *
 * OUTPUT:  none                                                                               *
 *                                                                                             *
 * WARNINGS:   Call this routine only ONCE at the beginning of the game.                       *
 *                                                                                             *
 * HISTORY:                                                                                    *
 *   12/15/1994 JLB : Created.                                                                 *
 *=============================================================================================*/
// (0x004E0950, slot 5)
// Starts with an empty button list.

void GScreenClass::One_Time()
{
    Buttons = 0;
}


/***********************************************************************************************
 * GScreenClass::Init -- Init's the entire display hierarchy by calling all Init routines.     *
 *                                                                                             *
 * This routine shouldn't be overloaded.  It's the main map initialization routine, and will   *
 * perform a complete map initialization, from mixfiles to clearing the buffers.  Calling this *
 * routine results in calling every initialization routine in the entire map hierarchy.        *
 *                                                                                             *
 * INPUT:                                                                                      *
 *      theater      theater to initialize to                                                  *
 *                                                                                             *
 * OUTPUT:                                                                                     *
 *      none.                                                                                  *
 *                                                                                             *
 * WARNINGS:                                                                                   *
 *      none.                                                                                  *
 *                                                                                             *
 * HISTORY:                                                                                    *
 *   12/28/1994 BR : Created.                                                                  *
 *=============================================================================================*/
// (0x004E0960, slot 6)
// Resets the redraw state and the input gadgets.

void GScreenClass::Init(int theater)
{
    Init_Clear();
    Init_IO();
}


/***********************************************************************************************
 * GScreenClass::Init_Clear -- Sets the map to a known state.                                  *
 *                                                                                             *
 * This routine (and those that overload it) clears any buffers and variables to a known       *
 * state.  It assumes that all buffers are allocated & valid.  The map should be displayable   *
 * after calling this function, and should draw basically an empty display.                    *
 *                                                                                             *
 * INPUT:                                                                                      *
 *      none.                                                                                  *
 *                                                                                             *
 * OUTPUT:                                                                                     *
 *      none.                                                                                  *
 *                                                                                             *
 * WARNINGS:                                                                                   *
 *      none.                                                                                  *
 *                                                                                             *
 * HISTORY:                                                                                    *
 *   12/28/1994 BR : Created.                                                                  *
 *=============================================================================================*/
// (0x004E0980, slot 7)
// Asks for a complete redraw.

void GScreenClass::Init_Clear()
{
    IsToRedraw = 2;
}


/***********************************************************************************************
 * GScreenClass::Init_IO -- Initializes the Button list ('Buttons').                           *
 *                                                                                             *
 * INPUT:                                                                                      *
 *      none.                                                                                  *
 *                                                                                             *
 * OUTPUT:                                                                                     *
 *      none.                                                                                  *
 *                                                                                             *
 * WARNINGS:                                                                                   *
 *      none.                                                                                  *
 *                                                                                             *
 * HISTORY:                                                                                    *
 *   12/28/1994 BR : Created.                                                                  *
 *=============================================================================================*/
// (0x004E0990, slot 8)
// Drops the button list.

void GScreenClass::Init_IO()
{
    Buttons = 0;
}


/***********************************************************************************************
 * GScreenClass::Flag_To_Redraw -- Flags the display to be redrawn.                            *
 *                                                                                             *
 *    This function is used to flag the display system whether any rendering is needed. The    *
 *    parameter tells the system either to redraw EVERYTHING, or just that something somewhere *
 *    has changed and the individual Draw_It functions must be called. When a sub system       *
 *    determines that it needs to render something local to itself, it would call this routine *
 *    with a false parameter. If the entire screen gets trashed or needs to be rebuilt, then   *
 *    this routine will be called with a true parameter.                                       *
 *                                                                                             *
 * INPUT:   complete -- bool; Should the ENTIRE screen be redrawn?                             *
 *                                                                                             *
 * OUTPUT:  none                                                                               *
 *                                                                                             *
 * WARNINGS:   This doesn't actually draw the screen, it merely sets flags so that when the    *
 *             Render() function is called, the appropriate drawing steps will be performed.   *
 *                                                                                             *
 * HISTORY:                                                                                    *
 *   12/15/1994 JLB : Created.                                                                 *
 *=============================================================================================*/
// (0x004E09A0, slot 14)
// Marks the tactical view dirty; a nonzero request is remembered unless a
// complete redraw is already pending, and the screen object is notified.

void GScreenClass::Flag_To_Redraw(int complete)
{
    if (TacticalMap != 0) {
        TacticalMap->unknown_d7d = true;
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
        unknown_surface_83995c = HiddenSurface;
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


/***********************************************************************************************
 * GScreenClass::Add_A_Button -- Add a gadget to the game input system.                        *
 *                                                                                             *
 *    This will add a gadget to the game input system. The gadget will be processed in         *
 *    subsequent calls to the GScreenClass::Input() function.                                  *
 *                                                                                             *
 * INPUT:   gadget   -- Reference to the gadget that will be added to the input system.        *
 *                                                                                             *
 * OUTPUT:  none                                                                               *
 *                                                                                             *
 * WARNINGS:   none                                                                            *
 *                                                                                             *
 * HISTORY:                                                                                    *
 *   01/19/1995 JLB : Created.                                                                 *
 *=============================================================================================*/
// (0x004E0AC0, slot 12)
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


/***********************************************************************************************
 * GScreenClass::Remove_A_Button -- Removes a gadget from the game input system.               *
 *                                                                                             *
 * INPUT:   gadget   -- Reference to the gadget that will be removed from the input system.    *
 *                                                                                             *
 * OUTPUT:  none                                                                               *
 *                                                                                             *
 * WARNINGS:   'gadget' MUST be already a part of 'Buttons', or the new value of 'Buttons'     *
 *               will be invalid!                                                              *
 *                                                                                             *
 * HISTORY:                                                                                    *
 *   01/19/1995 JLB : Created.                                                                 *
 *=============================================================================================*/
// (0x004E0B00, slot 13)
// Unlinks a listed gadget; the list continues at what Remove returns.

bool GScreenClass::Remove_A_Button(GadgetClass &gadget)
{
    if (!Is_Button_Listed(gadget)) {
        return false;
    }
    Buttons = (GadgetClass *)gadget.Remove();
    return true;
}


/***********************************************************************************************
 * GScreenClass::Render -- General drawing dispatcher an display update function.              *
 *                                                                                             *
 *    This routine should be called in the main game loop (once every game frame). It will     *
 *    call the Draw_It() function if necessary. All rendering is performed to the LogicPage    *
 *    which is set to the HIDPAGE. After rendering has been performed, the HIDPAGE is          *
 *    copied to the visible page.                                                              *
 *                                                                                             *
 * INPUT:   none                                                                               *
 *                                                                                             *
 * OUTPUT:  none                                                                               *
 *                                                                                             *
 * WARNINGS:   This actually updates the graphic display. As a result it can take quite a      *
 *             while to perform.                                                               *
 *                                                                                             *
 * HISTORY:                                                                                    *
 *   12/15/1994 JLB : Created.                                                                 *
 *=============================================================================================*/
// (0x004E0B30, slot 15)
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
    TacticalMap->Render(unknown_surface_839964, any, 0);
    TacticalMap->Render(unknown_surface_839964, any, 1);
    Draw_It(complete);
    TacticalMap->Render(unknown_surface_839964, any, 2);

    if (unknown_flag_abcd85 && !Debug_Map) {
        WWMouse->Erase_Mouse(SidebarSurface, true);
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


/***********************************************************************************************
 * GScreenClass::Blit_Display -- Redraw the display from the hidpage to the seenpage.          *
 *                                                                                             *
 *    This routine is used to copy the correct display from the HIDPAGE                        *
 *    to the SEENPAGE.                                                                         *
 *                                                                                             *
 * INPUT:   none                                                                               *
 *                                                                                             *
 * OUTPUT:  none                                                                               *
 *                                                                                             *
 * WARNINGS:   none                                                                            *
 *                                                                                             *
 * HISTORY:                                                                                    *
 *   02/14/1994 JLB : Created.                                                                 *
 *   05/01/1994 JLB : Converted to member function.                                            *
 *=============================================================================================*/
// (0x004E0C50, slot 17)
// Flushes the 3D device (an empty scene) when it is in use, then copies the
// composed surface to the screen without the mouse.

void GScreenClass::Blit_Display()
{
    if (UseDirect3D && unknown_device_852fe0->BeginScene() == 0) {
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
        dest_rect.X += (((SidebarSurface->Get_Width() < unknown_int_839648 - dest_rect.Width) ? SidebarSurface->Get_Width() : unknown_int_839648 - dest_rect.Width) > 0)
            ? ((SidebarSurface->Get_Width() < unknown_int_839648 - dest_rect.Width) ? SidebarSurface->Get_Width() : unknown_int_839648 - dest_rect.Width)
            : 0;
    }

    Rect src_rect = *area;
    if (TacticalMap != 0 && TacticalMap->unknown_c0 != 1.0) {
        int zoom_surface_width = surface->Get_Width();
        int zoom_surface_height = surface->Get_Height();
        Rect tmp;
        double zoomed_width = (double)zoom_surface_width / TacticalMap->unknown_c0;
        tmp.X = (int)(((double)zoom_surface_width - zoomed_width) / 2.0);
        double zoomed_height = (double)zoom_surface_height / TacticalMap->unknown_c0;
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

    if (TacticalActive) {
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

    unknown_surface_839950->Copy_From(dest_rect, *surface, src_rect, false, _dialog_count == 0);

    if (mouse && WWMouse != 0) {
        Sleep(50);
        WWMouse->Erase_Mouse(surface, false);
    }
    }
    }
}


/// <summary>
/// Performs the per frame logic for the game screen.
/// This routine eases the screen shake offset back toward its resting position, so that a
/// jolt requested by an explosion or a superweapon settles down of its own accord. Derived
/// screens call this routine from their own logic pass.
/// </summary>
/// <param name="xy">The current mouse position.</param>
// (0x004E10C0, slot 10)
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
