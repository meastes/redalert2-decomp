/*******************************************************************************
 *                                O P E N  T S
 *******************************************************************************
 * SPDX-License-Identifier: GPL-3.0-or-later
 * Copyright 2026 OpenTS contributors
 *
 * See LICENSE.md for applicable additional terms and warranty disclaimers.
 ******************************************************************************/

#pragma once

#include "alphalighting.h"

class AlphaLightingRemapInitClass; // RA2: abuffer.h is not included here (its rect.h would give _alpha a RECT_NONE initializer the original lacks)
class ABuffer;

extern AlphaLightingRemapInitClass AlphaLightingRemapInit;
extern ABuffer * AlphaBuffer;
