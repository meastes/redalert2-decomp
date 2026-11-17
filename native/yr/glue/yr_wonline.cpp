// The two Westwood Online pieces the rest of the game reaches while wonline.cpp is cut from the
// native build (native/yr/fixups.json skip_unit). The text is ra2-decomp's (src/wonline.cpp;
// SPDX GPL-3.0-or-later, Copyright 2026 ra2-decomp contributors, EA's GPL section 7 terms;
// THIRD_PARTY.md), copied here unchanged because its unit is not compiled (lane Y3). Select_Game
// sets the "away" auto-reply text on loading a game; only Internet chat reads it.
#include "widewstring.h"

WideWstring DAT_00B76230;
extern bool DAT_00B77E40;

// FUNCTION: GAMEMD 0x0078ABF0
void __fastcall FUN_0078ABF0(bool flag, wchar_t const * text)
{
	DAT_00B77E40 = flag;
	DAT_00B76230.set(text);
}
