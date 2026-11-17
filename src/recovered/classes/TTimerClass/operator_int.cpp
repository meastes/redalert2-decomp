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
// Adopted from TibSun timer.h, TTimerClass<T>::operator int.
#define RA2_TTIMER_CONVERSION_INT
#include "shared/system_timer.hpp"
#include "shared/t_timer.hpp"
template <>
TTimerClass<SystemTimerClass>::operator int() const
{
    int value = Accumulated;
    if (Started != -1) {
        value += Timer - Started;
    }
    return value;
}
