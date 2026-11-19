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

// The ATL 3.0 headers of Visual C++ 6.0, as wonline.cpp includes them (TibSun's wonline.cpp):
// the module object is declared between atlbase.h and atlcom.h, as ATL requires.

#include <atlbase.h>
extern CComModule _Module;
#include <atlcom.h>
