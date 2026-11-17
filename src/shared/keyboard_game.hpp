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
// Game-facing keyboard interface from TibSun keyboard.h, adapted to RA2.
// Layout: WWKeyboardClass base0x31C plus the game-facing 32-bit flag at0x31C.
// Target allocates0x320, calls532130, storesDWORD[object+31C], then installs832468
// at68EB6B..68EB8B. Flag name from TibSun keyboard.h; initialvalue remains unevidenced.
// The enum range preserves the 16-bit key/button values in a 32-bit game input.
#ifndef RA2_SHARED_KEYBOARD_GAME_HPP
#define RA2_SHARED_KEYBOARD_GAME_HPP
#include "shared/keyboard.hpp"
// Partial game-facing interface evidenced in TibSun keyboard.h lines528..670.
// Constructor/layout evidence: target68EB6B..68EB8B; interface from TibSun keyboard.h.
enum GScreenKeyboardKey { GScreenKeyboardNone = 0, GScreenKeyboardButton = 0x8000 };
struct KeyboardClass : public WWKeyboardClass {
    unsigned unknown_31c; // +0x31C, total0x320; IsLibrary is a reference-name hypothesis.
    GScreenKeyboardKey Check() { return (GScreenKeyboardKey)WWKeyboardClass::Check(); }
    GScreenKeyboardKey Get() { return (GScreenKeyboardKey)WWKeyboardClass::Get(); }
};
#endif
