// SPDX-License-Identifier: GPL-3.0-or-later
// Copyright 2026 OpenTS contributors
// See LICENSE.md for applicable additional terms and warranty disclaimers.
#pragma once

#include "win.h"

void __fastcall Show_Dialog(HWND dialog, int kind);
void __fastcall Dialog_On_WM_PAINT(HWND dialog);
void __fastcall Dialog_On_WM_DRAWITEM(long item);
