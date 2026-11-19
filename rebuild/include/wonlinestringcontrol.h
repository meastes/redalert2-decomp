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

#include "wonlinecontrol.h"
#include "wstring.h"

class WonlineStringDialogControl : public SimpleWonlineDialogControl { // RA2: vtable 0x007AF934, the control of the dialogs that ask for a text
public: // RA2: as above
	WonlineStringDialogControl(void) {} // RA2: expanded where it is used
	virtual ~WonlineStringDialogControl(void) {} // RA2: expanded where it is used
	Wstring Text; // RA2: +0x0C, the text the player typed
};
