/*******************************************************************************
 *                                O P E N  T S
 *******************************************************************************
 * SPDX-License-Identifier: GPL-3.0-or-later
 * Copyright 2026 OpenTS contributors
 *
 * See LICENSE.md for applicable additional terms and warranty disclaimers.
 ******************************************************************************/

#pragma once

template<class T> class TRect;
typedef TRect<int> Rect;

extern Rect SidebarRect;
extern Rect TacticalRect;
extern Rect VisibleRect;
extern Rect * TabBarRect; // RA2: the area of the tab bar above the tactical view

// Declarations moved from the units that used them (one declaration per name).
extern Rect BriefingButtonRect; // RA2: 0x00AB6468
extern Rect OptionsButtonRect; // RA2: 0x00AB6338
