// SPDX-License-Identifier: GPL-3.0-or-later
// RA2: not in TibSun. The detail settings that decide the frame rate below which effects are thinned
// out (YRpp's names, namespace Detail); logic.cpp defines GetMinFrameRate.
#pragma once

namespace Detail {
	extern unsigned MinFrameRate;
	extern unsigned BufferZoneWidth;
	unsigned GetMinFrameRate(void);
}
