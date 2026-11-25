// SPDX-License-Identifier: GPL-3.0-or-later
// Copyright 2026 ra2-decomp contributors
#pragma once

#include "globals.h" // RA2: the tree declares the conversion function (Wide_To_Narrow 0x6FA120) there, once

// The narrow copy of `text`, valid until eight later conversions have reused its buffer.
inline char * Narrow_Text(wchar_t const * text)
{
	return Wide_To_Narrow(text);
}
