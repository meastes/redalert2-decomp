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


#pragma once

#include "wonlinestringcontrol.h"

class CreateGameDialogControl : public WonlineStringDialogControl { // RA2: vtable 0x007AF94C, the control of the create game dialog (0xC3); its base constructor 0x00765AA0 is called out of line from Host_Game, the four flags are set where it is used
public: // RA2: as above
	CreateGameDialogControl(bool limit_resolution, bool observer) : IsTournament(false), IsPrivate(false), LimitResolution(limit_resolution), Observer(observer) {} // RA2: the executable stores +0x10 and +0x11 as 0, +0x12 and +0x13 from the arguments
	virtual ~CreateGameDialogControl(void) {} // RA2: expanded where it is used
	bool IsTournament; // RA2: +0x10, the dialog's radio button 0x629 sets it (Host_Game copies it to the channel's tournament member)
	bool IsPrivate; // RA2: +0x11, the radio button 0x66C sets it (the game gets a password and the flag 0x100)
	bool LimitResolution; // RA2: +0x12, the game limits the screen size (flag 0x200 or 0x400)
	bool Observer; // RA2: +0x13, the host is an observer
};
