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
// Function comment blocks from mischa85/RedAlert2 392e904 src/control.cpp (matching RA2 1.006 decomp), placed above our
// own notes; code unchanged.

// control.cpp: the original translation unit re-filed as one source (Route A): the accepted
// bodies of its canonical compile group in address order, under the unit's own header
// state (its switches and includes in their original order). The module map
// manifests/original_target/modules/control.json carries the addresses and the rows that
// stay in their own files.


#include "shared/shape_file.hpp"
#include "shared/gadget.hpp"


// Header-static dynamic initializer of the unit at 0x00483410: the empty
// rectangle of shared/shape_file.hpp (0x00851130), the unit's first function.


/***********************************************************************************************
 * ControlClass::ControlClass -- Constructor for control class objects.                        *
 *                                                                                             *
 *    This is the normal constructor for control class objects. At this level, it only needs   *
 *    to record the ID number assigned to this button.                                         *
 *                                                                                             *
 * INPUT:   id    -- The ID number for this gadget. If the ID number specified is 0, then      *
 *                   this tells the system that no special ID code should be returned.         *
 *                                                                                             *
 *          x,y   -- Pixel coordinate of upper left corner of gadget's region.                 *
 *                                                                                             *
 *          w,h   -- Pixel dimensions of the gadget's region.                                  *
 *                                                                                             *
 *          flags -- The input event flags that this gadget recognizes.                        *
 *                                                                                             *
 *          sticky-- This this a "sticky" gadget? A sticky gadget is one that takes over the   *
 *                   gadget list while the mouse button is held down, if the mouse button was  *
 *                   initially clicked over its region. This is the behavior of "normal"       *
 *                   buttons in Windows.                                                       *
 *                                                                                             *
 * OUTPUT:  none                                                                               *
 *                                                                                             *
 * WARNINGS:   none                                                                            *
 *                                                                                             *
 * HISTORY:                                                                                    *
 *   01/15/1995 JLB : Created.                                                                 *
 *=============================================================================================*/
// (0x00483430)

ControlClass::ControlClass(unsigned id, int x, int y, int w, int h, unsigned flags, bool sticky) :
    GadgetClass(x, y, w, h, flags, sticky),
    ID(id),
    Peer(0)
{
}


/***********************************************************************************************
 * ControlClass::ControlClass -- Copy constructor for control gadget.                          *
 *                                                                                             *
 *    This copy constructor for a control gadget is used create a duplicate gadget that        *
 *    is functionally similar.                                                                 *
 *                                                                                             *
 * INPUT:   control  -- Reference to the gadget that is to be copied.                          *
 *                                                                                             *
 * OUTPUT:  none                                                                               *
 *                                                                                             *
 * WARNINGS:   none                                                                            *
 *                                                                                             *
 * HISTORY:                                                                                    *
 *   12/05/1995 JLB : Created.                                                                 *
 *=============================================================================================*/
// (0x00483480)

ControlClass::ControlClass(ControlClass const &control) :
    GadgetClass(control),
    ID(control.ID),
    Peer(control.Peer)
{
}


/***********************************************************************************************
 * ControlClass::Action -- Normal action for control gadget objects.                           *
 *                                                                                             *
 *    This function gets called when the input event that this control gadget is looking for   *
 *    occurs. In such a case, the return key code value is changed to the gadget's ID number   *
 *    with the special button bit flag attached.                                               *
 *                                                                                             *
 * INPUT:   flags -- The event that triggered this function call. If this value is NULL, then  *
 *                   this is a forced (probably due to the sticky flag) call and the key code  *
 *                   is not altered.                                                           *
 *                                                                                             *
 *          key   -- Reference to the key code that will be returned by the controlling        *
 *                   Input() function.                                                         *
 *                                                                                             *
 * OUTPUT:  bool; Should further list processing be aborted?                                   *
 *                                                                                             *
 * WARNINGS:   none                                                                            *
 *                                                                                             *
 * HISTORY:                                                                                    *
 *   01/15/1995 JLB : Created.                                                                 *
 *=============================================================================================*/
// (0x004834B0, slot 31)
// Reports the event as button key ID | 0x8000 (| 0x4000 for event 0x40 on a
// gadget that takes event 0x10), tells the peer, then runs GadgetClass::Action.

int ControlClass::Action(unsigned flags, unsigned &key)
{
    if (flags) {
        if (ID) {
            key = ID | KN_BUTTON;
            if ((flags & 0x40) && (Flags & 0x10)) {
                key |= 0x4000;
            }
        } else {
            key = 0;
        }
    }
    if (Peer) {
        Peer->Peer_To_Peer(flags, key, *this);
    }
    return GadgetClass::Action(flags, key);
}


/***********************************************************************************************
 * ControlClass::Make_Peer -- Assigns a peer gadget to this gadget.                            *
 *                                                                                             *
 *    This function will assign another gadget to this one. That other gadget will receive     *
 *    notification of any Action() call to this gadget. Presumably, this is how one gadget     *
 *    can automatically adapt to changes in another. Say for example, a slider bar can affect  *
 *    the list box it is attached to.                                                          *
 *                                                                                             *
 * INPUT:   gadget   -- The gadget to inform when any Action() function is called.             *
 *                                                                                             *
 * OUTPUT:  none                                                                               *
 *                                                                                             *
 * WARNINGS:   none                                                                            *
 *                                                                                             *
 * HISTORY:                                                                                    *
 *   01/16/1995 JLB : Created.                                                                 *
 *=============================================================================================*/
// (0x00483510, slot 33)

void ControlClass::Make_Peer(GadgetClass &gadget)
{
    Peer = &gadget;
}


/***********************************************************************************************
 * ControlClass::Get_ID -- Gets the ID number for this gadget.                                 *
 *                                                                                             *
 *    This function will query and return with the ID number for this gadget. It is primarily  *
 *    used by the Extract_Gadget() function.                                                   *
 *                                                                                             *
 * INPUT:   none                                                                               *
 *                                                                                             *
 * OUTPUT:  Returns with the ID number for this gadget. If zero is returned, this means that   *
 *          no ID was assigned to this gadget. This is a special case since a zero value will  *
 *          never be returned as a pseudo-key as is done with non-zero values.                 *
 *                                                                                             *
 * WARNINGS:   none                                                                            *
 *                                                                                             *
 * HISTORY:                                                                                    *
 *   01/16/1995 JLB : Created.                                                                 *
 *=============================================================================================*/
// (0x00483520, slot 17)

unsigned ControlClass::Get_ID() const
{
    return ID;
}


/***********************************************************************************************
 * ControlClass::Draw_Me -- Draw logic for the control class object.                           *
 *                                                                                             *
 *    This is called when the control object might need to be redrawn or when redrawing is     *
 *    necessary. Since at this level of the class hierarchy, no actual drawing occurs, this    *
 *    routine doesn't perform any rendering. It does, however, inform any peer attached        *
 *    object that a Draw_Me function has been called. Presumably, the attached peer gadget     *
 *    might very well need to be redrawn as a result of some action by this gadget. Since this *
 *    gadget might, more than likely, be of the "sticky" variety, a normal call to Draw_Me     *
 *    for the other gadget will not occur. It must rely on the call by this routine in order   *
 *    to update correctly. A typical example of this would be a slider that is attached to     *
 *    a list box. As the slider is being drug around, the attached list box must be redrawn.   *
 *                                                                                             *
 * INPUT:   forced   -- Should the redraw be forced regardless of the redraw flag?             *
 *                                                                                             *
 * OUTPUT:  bool; Was the gadget redrawn?                                                      *
 *                                                                                             *
 * WARNINGS:   none                                                                            *
 *                                                                                             *
 * HISTORY:                                                                                    *
 *   01/16/1995 JLB : Created.                                                                 *
 *=============================================================================================*/
// (0x00483530, slot 27)
// Draws the peer first.

int ControlClass::Draw_Me(int forced)
{
    if (Peer) {
        Peer->Draw_Me();
    }
    return GadgetClass::Draw_Me(forced);
}


// void GadgetClass::Peer_To_Peer(unsigned flags, unsigned &key, ControlClass &whom) (0x00483560)
// Compiler-emitted from shared/gadget.hpp in this unit (inline, empty); no source of its own.


// void *ControlClass::`scalar deleting destructor'(unsigned int) (0x00483570)
// Compiler-emitted from shared/gadget.hpp in this unit; no source of its own.
