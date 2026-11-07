#define RA2_COORD_EQUAL_MEMBER
// SPDX-License-Identifier: GPL-3.0-or-later
// Copyright 2026 ra2-decomp contributors
// Copyright 2025 Electronic Arts Inc.; 2026 OpenTS contributors
// Derived from Electronic Arts source code; EA's GPL Section 7 terms apply (COPYING.md).
// void PlanningTokenClass::Destroy() (0x006130B0)
// From ra2-decomp (YR 0x00636310).
// Drops the path, forgets the owner and deletes the token (TechnoClass's
// destructor 0x006C20B0 calls it on the techno's token). Cancel_Owner and the
// destructor (with Planning_Unlist_Token and a second Cancel_Owner) expand inline.
#include "shared/planning.hpp"

void PlanningTokenClass::Destroy()
{
    Cancel_Owner();
    Owner = 0;
    delete this;
}
