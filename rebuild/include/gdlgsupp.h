/*******************************************************************************
 *                                O P E N  T S
 *******************************************************************************
 * SPDX-License-Identifier: GPL-3.0-or-later
 * Copyright 2026 OpenTS contributors
 *
 * See LICENSE.md for applicable additional terms and warranty disclaimers.
 ******************************************************************************/

/*
**	RA2: the player rows shared by the multiplayer setup dialogs (gdlgsupp.cpp): each player
**	slot has a row control, a side box, a color box and a flag picture.
*/

#pragma once

#include <windows.h>

int __fastcall Player_Control_ID(int player);
int __fastcall Player_Side_Combo_ID(int player);
int __fastcall Side_Combo_Player(int id);
wchar_t const * __fastcall Side_Name(int side);
void __fastcall Fill_Side_Combos(HWND dialog);
void __fastcall Select_Player_Side(HWND dialog, int player);
void __fastcall Select_Side_Combo(HWND dialog, int id, int side);
int __fastcall Side_Combo_Data(HWND dialog, int id, int index);
int __fastcall Player_Color_Combo_ID(int player);
int __fastcall Color_Combo_Player(int id);
wchar_t const * __fastcall Color_Name(int color);
void __fastcall Init_Color_Entries(void);
void __fastcall Fill_Color_Combos(HWND dialog);
void __fastcall Select_Player_Color(HWND dialog, int player);
void __fastcall Select_Color_Combo(HWND dialog, int id, int color);
void __fastcall Color_Combo_Changed(HWND dialog, int id);
int __fastcall Color_Combo_Data(HWND dialog, int id, int index);
void __fastcall Show_Player_Flag(HWND dialog, int player);
