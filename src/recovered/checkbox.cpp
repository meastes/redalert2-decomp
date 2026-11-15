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
// Function comment blocks from mischa85/RedAlert2 392e904 src/checkbox.cpp (matching RA2 1.006 decomp), placed above our
// own notes; code unchanged.

// checkbox.cpp: the original translation unit re-filed as one source (Route A): the accepted
// bodies of its canonical compile group in address order, under the unit's own header
// state (its switches and includes in their original order). The module map
// manifests/original_target/modules/checkbox.json carries the addresses and the rows that
// stay in their own files.


#include "shared/gadget.hpp"
#include "shared/rect.hpp"
#include "shared/convert.hpp"
#include "shared/mouse.hpp"
#include "shared/text_print.hpp"
#include "shared/unknown_globals.hpp"


/***********************************************************************************************
 * CheckBoxClass::Draw_Me -- Draws the checkbox imagery.                                       *
 *                                                                                             *
 *    This routine will draw the checkbox either filled or empty as necessary.                 *
 *                                                                                             *
 * INPUT:   forced   -- Should the check box be drawn even if it doesn't think it needs to?    *
 *                                                                                             *
 * OUTPUT:  Was the check box rendered?                                                        *
 *                                                                                             *
 * WARNINGS:   none                                                                            *
 *                                                                                             *
 * HISTORY:                                                                                    *
 *   07/01/1995 JLB : Created.                                                                 *
 *=============================================================================================*/
// (0x0047D150; TibSun checkbox.cpp)
// control draws, hides the mouse, draws the sunken box, fills its face grey (palette
// index 13) and, when on, green (index 4), then shows the mouse again.

int CheckBoxClass::Draw_Me(int forced)
{
    if (ControlClass::Draw_Me(forced)) {
        WWMouse->Hide_Mouse();
        Draw_Box(Rect(X, Y, Width, Height), 0, false);
        LogicalSurface->Fill_Rect(Rect(X + 1, Y + 1, Width - 1, Height - 1), NormalDrawer->Convert(13));
        if (IsOn) {
            LogicalSurface->Fill_Rect(Rect(X + 1, Y + 1, Width - 1, Height - 1), NormalDrawer->Convert(4));
        }
        WWMouse->Show_Mouse();
        return true;
    }
    return false;
}


/***********************************************************************************************
 * CheckBoxClass::Action -- Handles a button action on a checkbox object.                      *
 *                                                                                             *
 *    This routine will detect if the mouse has been clicked on the checkbox object. If so,    *
 *    the check box state will be toggled.                                                     *
 *                                                                                             *
 * INPUT:   flags -- The event flags that resulted in this routine being called.               *
 *                                                                                             *
 *          key   -- The key that resulted in this routine being called.                       *
 *                                                                                             *
 * OUTPUT:  bool; Should normal processing occur?                                              *
 *                                                                                             *
 * WARNINGS:   none                                                                            *
 *                                                                                             *
 * HISTORY:                                                                                    *
 *   07/06/1996 JLB : Created.                                                                 *
 *=============================================================================================*/
// (0x0047D270)
// checkbox.cpp): a left release (flag 4) flips the state, then ToggleClass::Action.

int CheckBoxClass::Action(unsigned flags, unsigned &key)
{
    if (flags & 4) {
        if (IsOn) {
            Turn_Off();
        } else {
            Turn_On();
        }
    }
    return ToggleClass::Action(flags, key);
}
