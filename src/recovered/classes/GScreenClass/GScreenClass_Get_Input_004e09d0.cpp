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
#include "shared/gscreen.hpp"
#include "shared/mouse.hpp"
#include "shared/keyboard.hpp"
#include "shared/surface.hpp"
#include "shared/keyboard_game.hpp"
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
