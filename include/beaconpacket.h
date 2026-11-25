// SPDX-License-Identifier: GPL-3.0-or-later
// Copyright 2026 ra2-decomp contributors
// Copyright 2026 OpenTS contributors
#pragma once

#include "coord.h"

#pragma pack(push, 1)
struct BeaconPacket {
		int Command;
		char Name[43];
		Coord Position;
		unsigned char ID;
		unsigned char House;
		char Rest[455 - 61];
};
#pragma pack(pop)
