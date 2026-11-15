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
// From mischa85/RedAlert2 392e904 src/credits.cpp (matching RA2 1.006 decomp; TibSun credits.cpp): names
// and comments; modified (our module form and header state, Route A unit map manifests/original_target/modules/credits.json).

/* $Header: /CounterStrike/CREDITS.CPP 1     3/03/97 10:24a Joe_bostic $ */
/***********************************************************************************************
 ***              C O N F I D E N T I A L  ---  W E S T W O O D  S T U D I O S               ***
 ***********************************************************************************************
 *                                                                                             *
 *                 Project Name : Command & Conquer                                            *
 *                                                                                             *
 *                    File Name : CREDITS.CPP                                                  *
 *                                                                                             *
 *                   Programmer : Joe L. Bostic                                                *
 *                                                                                             *
 *                   Start Date : April 17, 1994                                               *
 *                                                                                             *
 *                  Last Update : March 13, 1995 [JLB]                                         *
 *                                                                                             *
 *---------------------------------------------------------------------------------------------*
 * Functions:                                                                                  *
 *   CreditClass::AI -- Handles updating the credit display.                                   *
 *   CreditClass::CreditClass -- Default constructor for the credit class object.              *
 *   CreditClass::Graphic_Logic -- Handles the credit redraw logic.                            *
 * - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - */

#include "shared/shape_file.hpp"
#include "shared/unknown_geometry.hpp"
#include "shared/tab.hpp"
#include "shared/map_view.hpp"
#include "shared/rules.hpp"
#include "shared/sound_play.hpp"
#include "shared/text_print.hpp"
#include "shared/hicolor_pixel.hpp"
#include "shared/radar.hpp"
#include "shared/unknown_globals.hpp"
#include "shared/id_source.hpp"
#include "shared/crt_memory.hpp"

// The unit's header-static dynamic initializers (0x00494320-0x00494501: the shape_file.hpp empty
// rect and the unknown_geometry.hpp constants) are emitted by these includes; no source of their own.


/***********************************************************************************************
 * CreditClass::CreditClass -- Default constructor for the credit class object.                *
 *                                                                                             *
 *    This is the constructor for the credit class object. It merely sets the credit display   *
 *    state to null.                                                                           *
 *=============================================================================================*/
CreditClass::CreditClass() :
    Credits(0),
    Current(0),
    IsToRedraw(false),
    IsUp(false),
    IsAudible(false),
    Countdown(0)
{
}


/***********************************************************************************************
 * CreditClass::Graphic_Logic -- Handles the credit redraw logic.                              *
 *                                                                                             *
 *    This routine should be called whenever the main game screen is to be updated. It will    *
 *    check to see if the credit display should be redrawn. If so, it will redraw it.          *
 *                                                                                             *
 * INPUT:   forced   -- Should the credit display be redrawn regardless of whether the redraw  *
 *                      flag is set? This is typically the case when the screen needs to be    *
 *                      redrawn from scratch.                                                  *
 *=============================================================================================*/
void CreditClass::Graphic_Logic(bool forced)
{
    if (forced || IsToRedraw) {
        int xx = SidebarSurface->Get_Width() / 2;

        /*
        **	Play a sound effect when the money display changes, but only if a sound
        **	effect was requested.
        */
        if (IsAudible && Rules->CreditTicks.Count() >= 2) {
            if (IsUp) {
                Voc_Play(Rules->CreditTicks[0], 0.5f, 0x2000, 0);
            } else {
                Voc_Play(Rules->CreditTicks[1], 0.5f, 0x2000, 0);
            }
        }

        /*
        **	Display the new current value.
        */
        Map.Draw_Credits_Tab();

        int color = Build_Hicolor_Pixel(TooltipColor.Red, TooltipColor.Green, TooltipColor.Blue);
        Plain_Text_Print(L"%ld", SidebarSurface, &SidebarSurface->Get_Rect(), &Point2D(xx, 2), color, 0, 0x4108, Current);

        IsToRedraw = false;
        IsAudible = false;
    }
}


/***********************************************************************************************
 * CreditClass::AI -- Handles updating the credit display.                                     *
 *                                                                                             *
 *    This routine handles the logic that controls the rate of credit change in the credit     *
 *    display. It doesn't actually redraw the credit display, but will flag it to be redrawn   *
 *    if it detects that a change is to occur.                                                 *
 *                                                                                             *
 * INPUT:   forced   -- Should the credit display immediately reflect the current credit       *
 *                      total for the player? This is usually desired when initially loading   *
 *                      a scenario or saved game.                                              *
 *=============================================================================================*/
void CreditClass::AI(bool forced)
{
    static int _last = 0;

    _last = Frame;

    Credits = unknown_player_a35db4->As_IHouse()->Available_Money();

    /*
    **	Make sure that the credit counter doesn't drop below zero.
    */
    Credits = Credits > 0 ? Credits : 0;

    if (unknown_id_source_a3d290->MissionTimer.Is_Active() || unknown_id_source_a3d290->MissionTimer) {
        IsToRedraw = true;
        Map.Flag_To_Redraw();
    }

    if (Current == Credits) return;

    if (forced) {
        IsAudible = false;
        Current = Credits;
    } else {
        if (Countdown) Countdown--;

        /*
        **	Determine the amount to change the display toward the
        **	desired value.
        */
        int adder = Credits - Current;

        if (adder > 0) {
            Countdown = 1;
        } else {
            Countdown = 3;
        }

        adder = abs(adder);
        adder >>= 3;
        if (adder < 1) {
            adder = 1;
        } else if (adder > 71+72) {
            adder = 71+72;
        }
        if (Current > Credits) adder = -adder;
        Current += adder;
        if (Current - adder != Current) {
            IsAudible = true;
            IsUp = (adder > 0);
        }
    }
    IsToRedraw = true;
    Map.Flag_To_Redraw();
    Map.IsToRedrawCredits = true;
}
