// bool AbstractClass::Is_Techno() const (0x006F8A70)
// Whether this is a TechnoClass; false for a null object (ra2-decomp include/abstract.h:92).
// Out-of-line until the UICmnds unit (0x006F6D00) emits it as the header inline's COMDAT.
// SPDX-License-Identifier: GPL-3.0-or-later
// Copyright 2026 ra2-decomp contributors
#define RA2_ABSTRACT_IS_TECHNO_DECL
#include "shared/abstract_class.hpp"

bool AbstractClass::Is_Techno() const
{
    if (this == 0) {
        return false;
    }
    return AbstractFlags.IsTechno;
}
