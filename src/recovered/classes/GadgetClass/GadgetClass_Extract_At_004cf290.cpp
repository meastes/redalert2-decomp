// SPDX-License-Identifier: GPL-3.0-or-later
// Copyright 2026 ra2-decomp contributors
// Copyright 2025 Electronic Arts Inc.; 2026 OpenTS contributors
// Derived from Electronic Arts source code; EA's GPL Section 7 terms apply (COPYING.md).
// GadgetClass *GadgetClass::Extract_At(int x, int y) (0x004CF290): the smallest enabled gadget of the list at x, y.
// Adapted from ra2-decomp src/gadget.cpp GadgetClass::Extract_Gadget_At (gamemd twin 0x004E15A0, matched there);
// see THIRD_PARTY.md.
#include "shared/gadget.hpp"
#include "shared/rect.hpp"
#include "shared/unknown_globals.hpp"

GadgetClass *GadgetClass::Extract_At(int x, int y)
{
    GadgetClass *gadget = this;
    Rect best(0, 0, unknown_screen_widths_7add68[2], unknown_screen_heights_7add74[2]);
    GadgetClass *result = 0;
    while (gadget) {
        if (!gadget->IsDisabled) {
            Rect current(gadget->X, gadget->Y, gadget->Width, gadget->Height);
            if (current.Is_Point_Within(Point2D(x, y)) && current.Width * current.Height <= best.Width * best.Height) {
                best = current;
                result = gadget;
            }
        }
        gadget = (GadgetClass *)gadget->Get_Next();
    }
    return (result);
}
