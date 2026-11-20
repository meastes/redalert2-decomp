// SPDX-License-Identifier: GPL-3.0-or-later
// Copyright 2026 ra2-decomp contributors
// Copyright 2026 OpenTS contributors
#pragma once

#include <stddef.h>

#pragma pack(push, 1)
struct BeaconTextPacket {
		int Command;
		char Name[43];
		char Slot;
		char House;
		wchar_t Text[128];
		char Rest[455 - 305];
};
#pragma pack(pop)
