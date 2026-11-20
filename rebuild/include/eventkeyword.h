// SPDX-License-Identifier: GPL-3.0-or-later
// Copyright 2026 ra2-decomp contributors
// Copyright 2025 Electronic Arts Inc.
// Derived from Electronic Arts source code; EA's GPL Section 7 terms apply (COPYING.md).
#pragma once

// RA2: an audio keyword and its value; each table ends with a NULL name.
struct EventKeyword
{
		char const * Name;
		int Value;
};
