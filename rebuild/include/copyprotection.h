// SPDX-License-Identifier: GPL-3.0-or-later
// Copyright 2026 ra2-decomp contributors
// RA2: the launcher handshake of the copy protection (copyprotection.cpp).
#pragma once

namespace CopyProtection {
bool IsLauncherRunning(void); // RA2: 0x004918F0, whether the launcher holds its mutex
bool NotifyLauncher(void); // RA2: 0x00491930, tells the launcher the game has started
void ShutDown(void); // RA2: 0x00491B00
}
