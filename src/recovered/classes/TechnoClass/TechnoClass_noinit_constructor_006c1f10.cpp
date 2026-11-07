// SPDX-License-Identifier: GPL-3.0-or-later
// Copyright 2026 ra2-decomp contributors
// Copyright 2025 Electronic Arts Inc.; 2026 OpenTS contributors
// Derived from Electronic Arts source code; EA's GPL Section 7 terms apply (COPYING.md).
// TechnoClass(NoInitClass const &) (0x006C1F10)
// The load constructor: every member with a NoInitClass constructor is built from noinit, the stage at
// +0x1BC and the three target lists default-construct, then the object re-registers in Technos unless it
// is already there. From ra2-decomp (YR 0x006F4300): the target lists' `(0, 0)` initializers. The
// DynamicVectorClass constructors expand inline but their VectorClass base constructors (0x0046D020,
// 0x004CEB90) stay out of line, as in the original, under the unit's RA2_CDTIMER_BASIC_BASE and
// RA2_STAGE_PLAIN_TIMER (first row 0x0049ED90).
#include "shared/techno.hpp"
TechnoClass::TechnoClass(NoInitClass const &noinit) :
    RadioClass(noinit), FlasherClass(noinit), StageClass(noinit),
    Cargo(noinit), unknown_118(noinit),
    unknown_130(noinit), unknown_13c(noinit), unknown_148(noinit), unknown_154(noinit),
    unknown_160(noinit), unknown_178(noinit), unknown_184(noinit), unknown_194(noinit),
    unknown_1d4(noinit),
    unknown_244(noinit), unknown_294(noinit), unknown_2a8(noinit),
    unknown_2c8(noinit), unknown_2e0(noinit), unknown_2f8(noinit), unknown_314(noinit),
    unknown_390(0, 0), unknown_3a8(0, 0), unknown_3c0(0, 0)
{
    bool found = false;
    for (int i = 0; i < Technos.Count(); i++) {
        if (Technos[i] == this) {
            found = true;
            break;
        }
    }
    unknown_3d8.Init();
    if (!found) {
        Technos.Add(this);
    }
}
