/*******************************************************************************
 * SPDX-License-Identifier: GPL-3.0-or-later
 * Copyright 2025 Electronic Arts Inc.
 * Copyright 2026 OpenTS contributors
 *
 * Contains material derived from Electronic Arts source code.
 * Modified by OpenTS contributors, 2026.
 * EA's GPLv3 Section 7 additional terms and supplemental warranty
 * disclaimers apply; see LICENSE.md.
 ******************************************************************************/
// Native data: the Random3Class seed tables, adopted from TibSun code/random.cpp
// (OpenTS-Developers/TibSun dbfbfbfaa, see THIRD_PARTY.md). Original data at
// 0x007EE4AC / 0x007EE4FC; the adopted text equals it (checked privately by
// Scout1050 against the supplied executable, 2026-11-08). Adapted only in the
// element type to our shared header (unsigned long -> RA2_ULONG via the port).
#include "shared/random.hpp"

RA2_ULONG Random3Class::Mix1[20] = {
	0x0baa96887, 0x01e17d32c, 0x003bcdc3c, 0x00f33d1b2,
	0x076a6491d, 0x0c570d85d, 0x0e382b1e3, 0x078db4362,
	0x07439a9d4, 0x09cea8ac5, 0x089537c5c, 0x02588f55d,
	0x0415b5e1d, 0x0216e3d95, 0x085c662e7, 0x05e8ab368,
	0x03ea5cc8c, 0x0d26a0f74, 0x0f3a9222b, 0x048aad7e4
};

RA2_ULONG Random3Class::Mix2[20] = {
	0x04b0f3b58, 0x0e874f0c3, 0x06955c5a6, 0x055a7ca46,
	0x04d9a9d86, 0x0fe28a195, 0x0b1ca7865, 0x06b235751,
	0x09a997a61, 0x0aa6e95c8, 0x0aaa98ee1, 0x05af9154c,
	0x0fc8e2263, 0x0390f5e8c, 0x058ffd802, 0x0ac0a5eba,
	0x0ac4874f6, 0x0a9df0913, 0x086be4c74, 0x0ed2c123b
};
