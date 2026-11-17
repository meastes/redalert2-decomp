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
// Function comment blocks from mischa85/RedAlert2 392e904 src/toggle.cpp (matching RA2 1.006 decomp), placed above our
// own notes; code unchanged.

// toggle.cpp: the original translation unit re-filed as one source (Route A): the accepted
// bodies of its canonical compile group in address order, under the unit's own header
// state (its switches and includes in their original order). The module map
// manifests/original_target/modules/toggle.json carries the addresses and the rows that
// stay in their own files.


#include "shared/shape_file.hpp"
#include "shared/gadget.hpp"


// Header-static dynamic initializer of the unit at 0x006EB100: the empty
// rectangle of shared/shape_file.hpp (0x00AC0BD8), the unit's first function.


/***********************************************************************************************
 * ToggleClass::ToggleClass -- Normal constructor for toggle button gadgets.                   *
 *                                                                                             *
 *    This is the normal constructor for toggle buttons. A toggle button is one that most      *
 *    closely resembles the Windows style. It has an up and down state as well as an on        *
 *    and off state.                                                                           *
 *                                                                                             *
 * INPUT:   id    -- ID number for this button.                                                *
 *                                                                                             *
 *          x,y   -- Pixel coordinate of upper left corner of this button.                     *
 *                                                                                             *
 *          w,h   -- Width and height of the button.                                           *
 *                                                                                             *
 * OUTPUT:  none                                                                               *
 *                                                                                             *
 * WARNINGS:   none                                                                            *
 *                                                                                             *
 * HISTORY:                                                                                    *
 *   01/15/1995 JLB : Created.                                                                 *
 *=============================================================================================*/
// (0x006EB120)
// A sticky, released, switched-off button.

ToggleClass::ToggleClass(unsigned id, int x, int y, int w, int h) :
    ControlClass(id, x, y, w, h, LEFTPRESS | LEFTRELEASE, true),
    IsPressed(false),
    IsOn(false),
    ToggleType(0)
{
}


/***********************************************************************************************
 * ToggleClass::Turn_On -- Turns the toggle button to the "ON" state.                          *
 *                                                                                             *
 *    This routine will turn the button on. The button will also be flagged to be redrawn      *
 *    at the next opportunity.                                                                 *
 *                                                                                             *
 * INPUT:   none                                                                               *
 *                                                                                             *
 * OUTPUT:  none                                                                               *
 *                                                                                             *
 * WARNINGS:   none                                                                            *
 *                                                                                             *
 * HISTORY:                                                                                    *
 *   01/15/1995 JLB : Created.                                                                 *
 *=============================================================================================*/
// (0x006EB160)

void ToggleClass::Turn_On()
{
    IsOn = true;
    Flag_To_Redraw();
}


/***********************************************************************************************
 * ToggleClass::Turn_Off -- Turns the toggle button to the "OFF" state.                        *
 *                                                                                             *
 *    This routine will turn the toggle button "off". It will also be flagged to be redrawn    *
 *    at the next opportunity.                                                                 *
 *                                                                                             *
 * INPUT:   none                                                                               *
 *                                                                                             *
 * OUTPUT:  none                                                                               *
 *                                                                                             *
 * WARNINGS:   none                                                                            *
 *                                                                                             *
 * HISTORY:                                                                                    *
 *   01/15/1995 JLB : Created.                                                                 *
 *=============================================================================================*/
// (0x006EB170)

void ToggleClass::Turn_Off()
{
    IsOn = false;
    Flag_To_Redraw();
}
