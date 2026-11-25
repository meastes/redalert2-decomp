// SPDX-License-Identifier: GPL-3.0-or-later
// Copyright 2026 ra2-decomp contributors

#include "always.h"
#include <cstddef>
#include <cstdlib>
#include "command.h"
#include "index.h"
#include "_command.h"
#include "keyboard.h" // RA2: the original unit includes this header

bool __fastcall Command_Allows_Modified_Key(CommandClass const & command, unsigned key)
{
	key &= ~0x800u;
	if (key != (key & ~0x1700u)) {
		return !command.PreventCombinationOverride(key);
	}
	return true;
}

bool __fastcall Command_Allows_Key(unsigned key)
{
	unsigned combination = key & ~0x800u;
	int unmodified_key = combination & ~0x1700u;
	CommandClass const * command = HotkeyCommands[unmodified_key];
	if (command == 0) {
		return true;
	}
	return !command->PreventCombinationOverride(combination);
}
