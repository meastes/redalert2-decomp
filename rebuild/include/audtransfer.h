// SPDX-License-Identifier: GPL-3.0-or-later
// Copyright 2026 ra2-decomp contributors
// Copyright 2025 Electronic Arts Inc.
// Derived from Electronic Arts source code; EA's GPL Section 7 terms apply (COPYING.md).
#pragma once

#include "imadata.h"

struct AudioDriverChannel;

typedef int(__fastcall CB_DECODE)(AudioDriverChannel *);

struct TRANSFER
{
		int state;
		int next_state;
		unsigned char * in;
		int in_bytes;
		int in_bytes_left;
		int in_size;
		int in_size_needed;
		unsigned char * out;
		int out_bytes;
		int out_bytes_left;
		int out_size;
		int out_size_needed;
		int channels;
		int pending;
		int block_size;
		CB_DECODE * decode;
		IMA_DATA ima[2];
		unsigned gap0[2];
};
