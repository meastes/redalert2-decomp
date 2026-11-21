// SPDX-License-Identifier: GPL-3.0-or-later
// Copyright 2026 ra2-decomp contributors
// Copyright 2025 Electronic Arts Inc.; 2026 OpenTS contributors
// Derived from Electronic Arts source code; EA's GPL Section 7 terms apply (COPYING.md).
// Changed to RA2 1.006's layout by RedAlert2 contributors, 2026.
#pragma once

#include <windows.h>
#include "refcount.h"
#include "rcptr.h" // RA2: the profile is handed around in an rc_ptr
#include "widewstring.h"
#include "wstring.h"

namespace WOL { struct Ladder; }

class PlayerProfile : public ReferenceCounted
{
	public:
		PlayerProfile(Wstring const & name, int locale, bool verified, int status, int country, int color, int rank, int points, int wins, int losses, int disconnects); // RA2: 0x00738C40
		virtual ~PlayerProfile(void); // RA2: 0x007395B0, the scalar deleting destructor is at 0x0073A090
		static rc_ptr<PlayerProfile> Load_Default(void); // RA2: 0x00738670, the profile of no persona (the _Default_ section of wolinfo.ini)
		static rc_ptr<PlayerProfile> Load(Wstring const & name); // RA2: 0x00738880, the profile of a persona (its section of wolinfo.ini)
		void SetName(Wstring const * name); // RA2: 0x00739920
		void SetLocale(int locale, int state); // RA2: 0x00739950
		void SetCountry(int country); // RA2: 0x007399B0
		void SetColor(int color); // RA2: 0x007399D0, renamed from the YR decompilation's SetField20: the member is the color
		void UpdateStats(WOL::Ladder const * record); // RA2: 0x007399F0
		void Save(Wstring const * name); // RA2: 0x007395D0
		void PopulateDialog(HWND window, Wstring const & name, bool update); // RA2: 0x00739B70, fills the profile dialog's controls from the profile

		bool IsDirty; // RA2: +0x08, set by every change; Save writes only then and clears it
		Wstring Name; // RA2: +0x0C, the Server key of wolinfo.ini
		int Locale; // RA2: +0x10
		bool IsLocaleVerified; // RA2: +0x14
		int LocaleStatus; // RA2: +0x18, 1 when verified, 3 when not; a lower status does not replace the locale
		int Country; // RA2: +0x1C
		int Color; // RA2: +0x20
		int Rank; // RA2: +0x24
		int Points; // RA2: +0x28
		int Wins; // RA2: +0x2C
		int Losses; // RA2: +0x30
		int Disconnects; // RA2: +0x34
};

void __fastcall WOL_Unknown_Stats_Text(HWND dialog, bool clearNames); // RA2: 0x00739A50, writes the "unknown" text into the statistics controls of the profile dialog (and empties the name list when asked)
bool __fastcall WOL_Get_Auto_Login(void); // RA2: 0x00738CB0, the AutoLogin setting of the [Ask] section of wolinfo.ini (true when the file is there and says nothing)
void __fastcall WOL_Set_Auto_Login(bool value); // RA2: 0x00738DA0
bool __fastcall WOL_Get_Add_Match_Buddies(void); // RA2: 0x00738EB0, the AddMatchBuddies setting of the [Ask] section
void __fastcall WOL_Set_Add_Match_Buddies(bool value); // RA2: 0x00738FA0
Wstring __fastcall WOL_Auto_Login_Persona(void); // RA2: 0x007390B0, the persona logged in automatically (AutoLogin of [DefaultPersona])
void __fastcall WOL_Set_Auto_Login_Persona(Wstring const & persona); // RA2: 0x007391F0
Wstring __fastcall WOL_Last_Valid_Persona(void); // RA2: 0x00739330, the persona that logged in last (LastValid of [DefaultPersona])
void __fastcall WOL_Set_Last_Valid_Persona(Wstring const & persona); // RA2: 0x00739470
