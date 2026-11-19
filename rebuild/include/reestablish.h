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

class TechnoClass;

struct ReestablishRecord { // RA2: the 8-byte record Captured allocates per captured contact (contact, was-tethered flag)
		TechnoClass * Contact; // RA2: as above
		bool WasTethered; // RA2: as above
		ReestablishRecord(TechnoClass * contact, bool tethered) : Contact(contact), WasTethered(tethered) {} // RA2: as above
};
