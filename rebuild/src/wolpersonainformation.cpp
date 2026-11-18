/*******************************************************************************
 *                                O P E N  T S
 *******************************************************************************
 * SPDX-License-Identifier: GPL-3.0-or-later
 * Copyright 2026 OpenTS contributors
 *
 * Contains material derived from Electronic Arts source code.
 * Modified by OpenTS contributors, 2026.
 * EA's GPLv3 Section 7 additional terms and supplemental warranty
 * disclaimers apply; see LICENSE.md.
 ******************************************************************************/

/*
**	RA2: the profile of the player in Westwood Online (WOLPersonaInformation.cpp): the record of a
**	persona (locale, country, color, ranking) that wolinfo.ini keeps under the persona's name, the
**	settings of the [Ask] and [DefaultPersona] sections, and the filling of the profile dialog.
**	TibSun has no such file; the code is the executable's from 0x00738450.
*/

#define GEOMETRY_H // RA2: geometry.h follows the includes, as its constants take their initializers after the one of the rectangle below
#include "always.h" // RA2: the includes this file needs
#include "cdfile.h" // RA2: as above
#include "ini.h" // RA2: as above
#include "rect.h" // RA2: as above
#include "rcptr.h" // RA2: as above
#include "playerprofile.h" // RA2: as above
#include "wolapi.h" // RA2: as above
#include "wolopt.h" // RA2: as above
#undef LoadString // RA2: windows.h maps it to LoadStringA
#include "stringtable.h" // RA2: as above
#include <stdlib.h> // RA2: as above
#include <wchar.h> // RA2: as above


#include "geometry.h" // RA2: this module holds its own copy of the geometry constants (initialized after the rectangle, from 0x00738470)
#include "widewstring.h"
#include "wstring.h"

enum PersonaControl // RA2: the controls of the profile dialog this file fills, by the settings they show
{
	PERSONA_NICKNAMES = 0x628, // RA2: the persona's nickname (an owner drawn combo box that holds narrow or wide text)
	PERSONA_SERVER = 0x629, // RA2: the server the persona plays on
	PERSONA_LOCALE = 0x62A, // RA2: the locale; the item data of each entry is a locale
	PERSONA_COUNTRY = 0x6A1, // RA2: the country; the item data of each entry is a country
	PERSONA_COLOR = 0x6A2, // RA2: the color; the item data of each entry is a color
	PERSONA_TITLE = 0x6EF, // RA2: the title text "profile of ..."
	PERSONA_WINS = 0x6F0, // RA2: the wins
	PERSONA_LOSSES = 0x6F1, // RA2: the losses
	PERSONA_RANK = 0x6F4, // RA2: the rank
	PERSONA_POINTS = 0x6F5, // RA2: the points
	PERSONA_DISCONNECTS = 0x701 // RA2: the disconnects
};

enum ProfileMessage // RA2: the messages of the owner drawn combo box and static text, named for the text they take
{
	PM_SET_TEXT_WIDE = 0x4B2, // RA2: sets the text from a wide string
	PM_SET_TEXT = 0x4B4, // RA2: sets the text from a narrow string
	PM_FIND_STRING = 0x4B9, // RA2: finds an entry by its narrow text (wParam is the entry to start after)
	PM_FIND_STRING_WIDE = 0x4BF, // RA2: finds an entry by its wide text
	PM_HAS_WIDE_TEXT = 0x4CE // RA2: whether the combo box holds wide text
};


Wstring DefaultProfileSection("_Default_"); // RA2: 0x00B26418, the section of wolinfo.ini that holds the profile of no persona

static inline int Profile_Get_Int(INIClass & ini, Wstring const & section, char const * key, int defvalue) // RA2: the locale is read through a helper (without it the executable's function and ours differ in what the compiler expands of the INIClass constructor)
{
	return ini.Get_Int(section, key, defvalue); // RA2: as above
}

static inline void Profile_Put_String(INIClass & ini, Wstring const & section, char const * key, Wstring const & value) // RA2: the server name is written through a helper, for the same reason as above
{
	Wstring name(key); // RA2: as above
	ini.Put_String(section, name, value); // RA2: as above
}

/// <summary>
/// Reads the profile of no persona: the _Default_ section of wolinfo.ini, or empty values when the file or the section is missing.
/// </summary>
rc_ptr<PlayerProfile> PlayerProfile::Load_Default(void) // RA2: 0x00738670
{
	Wstring server; // RA2: the server name starts empty
	int locale = 0; // RA2: no locale
	int country = -2; // RA2: an unset country and color are -2 (the statistics are -1)
	int color = -2; // RA2: as above
	CDFileClass file("wolinfo.ini"); // RA2: the file of the profiles; made when it is not there
	if (!file.Is_Available()) { // RA2: as above
		file.Create(); // RA2: as above
		file.Close(); // RA2: as above
		file.Open("wolinfo.ini", 1); // RA2: as above
	}
	if (file.Is_Available()) { // RA2: as above
		INIClass ini; // RA2: as above
		ini.Load(file, false); // RA2: as above
		file.Close(); // RA2: as above
		if (ini.Find_Section(DefaultProfileSection.text())) { // RA2: the entries are only read when the _Default_ section exists
			ini.Get_String(DefaultProfileSection, "Server", server, server); // RA2: as above
			locale = Profile_Get_Int(ini, DefaultProfileSection, "Locale", 0); // RA2: through the helper
			country = ini.Get_Int(DefaultProfileSection, "Country", -2); // RA2: as above
			color = ini.Get_Int(DefaultProfileSection, "Color", -2); // RA2: as above
		}
	}
	return rc_ptr<PlayerProfile>(new PlayerProfile(server, locale, false, 0, country, color, -1, -1, -1, -1, -1)); // RA2: the profile is 0x38 bytes; not verified, status 0, no statistics (-1)
}

/// <summary>
/// Reads the profile of a persona from its section of wolinfo.ini (the section is named by the persona in lower case); the default profile when there is no such section.
/// </summary>
rc_ptr<PlayerProfile> PlayerProfile::Load(Wstring const & name) // RA2: 0x00738880
{
	CDFileClass file("wolinfo.ini"); // RA2: as above
	if (!file.Is_Available()) { // RA2: as above
		file.Create(); // RA2: as above
		file.Close(); // RA2: as above
		file.Open("wolinfo.ini", 1); // RA2: as above
	}
	if (!file.Is_Available()) { // RA2: as above
		return Load_Default(); // RA2: as above
	}
	INIClass ini; // RA2: as above
	ini.Load(file, false); // RA2: as above
	file.Close(); // RA2: as above
	Wstring section(name); // RA2: the section is named by the persona in lower case
	section.toLower(); // RA2: as above
	if (!ini.Find_Section(section.text())) { // RA2: a persona without a section gets the default profile
		return Load_Default(); // RA2: as above
	}
	Wstring server; // RA2: as above
	ini.Get_String(section, "Server", server, server); // RA2: as above
	int locale = Profile_Get_Int(ini, section, "Locale", 0); // RA2: through the helper
	bool verified = ini.Get_Bool(section.get(), "LocaleVerified", false); // RA2: this entry is read with the narrow overload (no Wstring temporaries)
	int country = ini.Get_Int(section, "Country", -2); // RA2: as above
	int color = ini.Get_Int(section, "Color", -2); // RA2: as above
	int rank = ini.Get_Int(section, "Rank", -1); // RA2: as above
	int points = ini.Get_Int(section, "Points", -1); // RA2: as above
	int wins = ini.Get_Int(section, "Wins", -1); // RA2: as above
	int losses = ini.Get_Int(section, "Losses", -1); // RA2: as above
	int disconnects = ini.Get_Int(section, "Disconnects", -1); // RA2: as above
	return rc_ptr<PlayerProfile>(new PlayerProfile(server, locale, verified, verified ? 1 : 3, country, color, rank, points, wins, losses, disconnects)); // RA2: a verified locale has status 1, a guessed one 3
}

/// <summary>
/// Makes a profile from its parts; nothing is dirty yet.
/// </summary>
PlayerProfile::PlayerProfile(Wstring const & name, int locale, bool verified, int status, int country, int color, int rank, int points, int wins, int losses, int disconnects) : // RA2: 0x00738C40
	IsDirty(false), // RA2: +0x08, nothing to save yet
	Name(name),
	Locale(locale),
	IsLocaleVerified(verified),
	LocaleStatus(status),
	Country(country),
	Color(color),
	Rank(rank),
	Points(points),
	Wins(wins),
	Losses(losses),
	Disconnects(disconnects)
{
}

/// <summary>
/// Whether the player is logged in automatically: [Ask] AutoLogin of wolinfo.ini, true when the file exists and has no such entry, false without the file.
/// </summary>
bool __fastcall WOL_Get_Auto_Login(void) // RA2: 0x00738CB0
{
	CDFileClass file("wolinfo.ini"); // RA2: as above
	if (!file.Is_Available()) { // RA2: as above
		return false; // RA2: as above
	}
	INIClass ini; // RA2: as above
	ini.Load(file, false); // RA2: as above
	file.Close(); // RA2: as above
	return ini.Get_Bool("Ask", "AutoLogin", true); // RA2: read with the narrow overload; true when the entry is missing
}

/// <summary>
/// Writes [Ask] AutoLogin of wolinfo.ini, making the file when it is missing.
/// </summary>
void __fastcall WOL_Set_Auto_Login(bool value) // RA2: 0x00738DA0
{
	CDFileClass file("wolinfo.ini"); // RA2: as above
	if (!file.Is_Available()) { // RA2: as above
		file.Create(); // RA2: as above
		file.Close(); // RA2: as above
		file.Open("wolinfo.ini", 1); // RA2: as above
	}
	INIClass ini; // RA2: as above
	if (file.Is_Available()) { // RA2: as above
		ini.Load(file, false); // RA2: as above
		ini.Put_Bool("Ask", "AutoLogin", value); // RA2: written with the narrow overload
		ini.Save(file); // RA2: as above
	}
}

/// <summary>
/// Whether the player's matches add their opponents as buddies: [Ask] AddMatchBuddies of wolinfo.ini, true when the file exists and has no such entry, false without the file.
/// </summary>
bool __fastcall WOL_Get_Add_Match_Buddies(void) // RA2: 0x00738EB0
{
	CDFileClass file("wolinfo.ini"); // RA2: as above
	if (!file.Is_Available()) { // RA2: as above
		return false; // RA2: as above
	}
	INIClass ini; // RA2: as above
	ini.Load(file, false); // RA2: as above
	file.Close(); // RA2: as above
	return ini.Get_Bool("Ask", "AddMatchBuddies", true); // RA2: as above, for the other [Ask] entry
}

/// <summary>
/// Writes [Ask] AddMatchBuddies of wolinfo.ini, making the file when it is missing.
/// </summary>
void __fastcall WOL_Set_Add_Match_Buddies(bool value) // RA2: 0x00738FA0
{
	CDFileClass file("wolinfo.ini"); // RA2: as above
	if (!file.Is_Available()) { // RA2: as above
		file.Create(); // RA2: as above
		file.Close(); // RA2: as above
		file.Open("wolinfo.ini", 1); // RA2: as above
	}
	INIClass ini; // RA2: as above
	if (file.Is_Available()) { // RA2: as above
		ini.Load(file, false); // RA2: as above
		ini.Put_Bool("Ask", "AddMatchBuddies", value); // RA2: as above, for the other [Ask] entry
		ini.Save(file); // RA2: as above
	}
}

/// <summary>
/// The persona that logs in automatically: [DefaultPersona] AutoLogin of wolinfo.ini, empty without the file.
/// </summary>
Wstring __fastcall WOL_Auto_Login_Persona(void) // RA2: 0x007390B0
{
	CDFileClass file("wolinfo.ini"); // RA2: as above
	if (!file.Is_Available()) { // RA2: as above
		return Wstring((char const *)0); // RA2: as above
	}
	INIClass ini; // RA2: as above
	ini.Load(file, false); // RA2: as above
	file.Close(); // RA2: as above
	Wstring persona; // RA2: as above
	{
		Wstring key("AutoLogin"); // RA2: as above
		Wstring section("DefaultPersona"); // RA2: as above
		ini.Get_String(section, key, persona, persona); // RA2: as above
	}
	return persona; // RA2: as above
}

/// <summary>
/// Writes [DefaultPersona] AutoLogin of wolinfo.ini, making the file when it is missing.
/// </summary>
void __fastcall WOL_Set_Auto_Login_Persona(Wstring const & persona) // RA2: 0x007391F0
{
	CDFileClass file("wolinfo.ini"); // RA2: as above
	if (!file.Is_Available()) { // RA2: as above
		file.Create(); // RA2: as above
		file.Close(); // RA2: as above
		file.Open("wolinfo.ini", 1); // RA2: as above
	}
	INIClass ini; // RA2: as above
	if (file.Is_Available()) { // RA2: as above
		ini.Load(file, false); // RA2: as above
		{
			Wstring key("AutoLogin"); // RA2: as above
			Wstring section("DefaultPersona"); // RA2: as above
			ini.Put_String(section, key, persona); // RA2: as above
		}
		ini.Save(file); // RA2: as above
	}
}

/// <summary>
/// The persona that logged in last: [DefaultPersona] LastValid of wolinfo.ini, empty without the file.
/// </summary>
Wstring __fastcall WOL_Last_Valid_Persona(void) // RA2: 0x00739330
{
	CDFileClass file("wolinfo.ini"); // RA2: as above
	if (!file.Is_Available()) { // RA2: as above
		return Wstring((char const *)0); // RA2: as above
	}
	INIClass ini; // RA2: as above
	ini.Load(file, false); // RA2: as above
	file.Close(); // RA2: as above
	Wstring persona; // RA2: as above
	{
		Wstring key("LastValid"); // RA2: as above
		Wstring section("DefaultPersona"); // RA2: as above
		ini.Get_String(section, key, persona, persona); // RA2: as above
	}
	return persona; // RA2: as above
}

/// <summary>
/// Writes [DefaultPersona] LastValid of wolinfo.ini, making the file when it is missing.
/// </summary>
void __fastcall WOL_Set_Last_Valid_Persona(Wstring const & persona) // RA2: 0x00739470
{
	CDFileClass file("wolinfo.ini"); // RA2: as above
	if (!file.Is_Available()) { // RA2: as above
		file.Create(); // RA2: as above
		file.Close(); // RA2: as above
		file.Open("wolinfo.ini", 1); // RA2: as above
	}
	INIClass ini; // RA2: as above
	if (file.Is_Available()) { // RA2: as above
		ini.Load(file, false); // RA2: as above
		{
			Wstring key("LastValid"); // RA2: as above
			Wstring section("DefaultPersona"); // RA2: as above
			ini.Put_String(section, key, persona); // RA2: as above
		}
		ini.Save(file); // RA2: as above
	}
}

/// <summary>
/// Frees the profile; the name is a member with a destructor of its own.
/// </summary>
PlayerProfile::~PlayerProfile(void) // RA2: 0x007395B0
{
}

/// <summary>
/// Writes the profile into wolinfo.ini when it changed: under the given persona name in lower case, or under _Default_ without a name. A named profile is only written when the options allow it.
/// </summary>
void PlayerProfile::Save(Wstring const * name) // RA2: 0x007395D0
{
	WOLOptionsClass options; // RA2: the registry options are read first, whether or not anything is saved
	options.Load(); // RA2: as above; the constructor only clears the two members
	if (IsDirty) { // RA2: only a changed profile is written
		Wstring section(*name); // RA2: as above
		if (!name->length()) { // RA2: as above
			section = DefaultProfileSection; // RA2: as above
		} else {
			section.toLower(); // RA2: as above
			if (!options.Is_Bit1_Clear()) { // RA2: a persona's section is only written when bit 1 of the options is clear
				return; // RA2: as above
			}
		}
		CDFileClass file("wolinfo.ini"); // RA2: as above
		if (!file.Is_Available()) { // RA2: as above
			file.Create(); // RA2: as above
			file.Close(); // RA2: as above
			file.Open("wolinfo.ini", 1); // RA2: as above
		}
		if (file.Is_Available()) { // RA2: as above
			INIClass ini; // RA2: as above
			ini.Load(file, false); // RA2: as above
			Profile_Put_String(ini, section, "Server", Name); // RA2: through the helper
			{ Wstring k("Locale"); ini.Put_Int(section, k, Locale, 0); } // RA2: as above
			{ Wstring k("LocaleVerified"); ini.Put_Bool(section, k, IsLocaleVerified); } // RA2: as above
			{ Wstring k("Country"); ini.Put_Int(section, k, Country, 0); } // RA2: as above
			{ Wstring k("Color"); ini.Put_Int(section, k, Color, 0); } // RA2: as above
			{ Wstring k("Rank"); ini.Put_Int(section, k, Rank, 0); } // RA2: as above
			{ Wstring k("Points"); ini.Put_Int(section, k, Points, 0); } // RA2: as above
			{ Wstring k("Wins"); ini.Put_Int(section, k, Wins, 0); } // RA2: as above
			{ Wstring k("Losses"); ini.Put_Int(section, k, Losses, 0); } // RA2: as above
			{ Wstring k("Disconnects"); ini.Put_Int(section, k, Disconnects, 0); } // RA2: as above
			ini.Save(file); // RA2: as above
			file.Close(); // RA2: as above
			IsDirty = false; // RA2: as above
		}
	}
}

/// <summary>
/// Sets the name the server knows the persona by.
/// </summary>
void PlayerProfile::SetName(Wstring const * name) // RA2: 0x00739920
{
	if (!(*name == Name)) { // RA2: as above
		Name = *name; // RA2: as above
		IsDirty = true; // RA2: as above
	}
}

/// <summary>
/// Sets the locale when the new one is allowed to replace the old: the same locale only upgrades the verified flag (state 2), another one must not have a lower status than the one held, and 0 is never taken.
/// </summary>
void PlayerProfile::SetLocale(int locale, int state) // RA2: 0x00739950
{
	if (locale == Locale) { // RA2: as above
		if (state == 2 && !IsLocaleVerified) { // RA2: state 2 (the server's answer) verifies the locale already held
			IsLocaleVerified = true; // RA2: as above
			IsDirty = true; // RA2: as above
		}
		return; // RA2: as above
	}
	bool newer = state >= LocaleStatus; // RA2: as above
	if (Locale == 0) { // RA2: as above
		newer = true; // RA2: as above
	} else if (!newer) { // RA2: as above
		return; // RA2: as above
	}
	if (locale == 0 || !newer) { // RA2: as above
		return; // RA2: as above
	}
	if (state == 3) { // RA2: status 3 is a guessed locale, which is not verified
		IsLocaleVerified = false; // RA2: as above
	}
	Locale = locale; // RA2: as above
	LocaleStatus = state; // RA2: as above
	IsDirty = true; // RA2: as above
}

/// <summary>
/// Sets the country.
/// </summary>
void PlayerProfile::SetCountry(int country) // RA2: 0x007399B0
{
	if (country != Country) { // RA2: as above
		Country = country; // RA2: as above
		IsDirty = true; // RA2: as above
	}
}

/// <summary>
/// Sets the color.
/// </summary>
void PlayerProfile::SetColor(int color) // RA2: 0x007399D0
{
	if (color != Color) { // RA2: as above
		Color = color; // RA2: as above
		IsDirty = true; // RA2: as above
	}
}

/// <summary>
/// Takes rank, points, wins, losses and disconnects over from a ladder record.
/// </summary>
void PlayerProfile::UpdateStats(Ladder const * record) // RA2: 0x007399F0
{
	if (record->rung != Rank) { // RA2: the rank is the ladder's rung (+0x1C of the record)
		Rank = record->rung; // RA2: as above
		IsDirty = true; // RA2: as above
	}
	if (record->points != Points) { // RA2: as above
		Points = record->points; // RA2: as above
		IsDirty = true; // RA2: as above
	}
	if (record->wins != Wins) { // RA2: as above
		Wins = record->wins; // RA2: as above
		IsDirty = true; // RA2: as above
	}
	if (record->losses != Losses) { // RA2: as above
		Losses = record->losses; // RA2: as above
		IsDirty = true; // RA2: as above
	}
	if (record->disconnects != Disconnects) { // RA2: as above
		Disconnects = record->disconnects; // RA2: as above
		IsDirty = true; // RA2: as above
	}
}

/// <summary>
/// Puts the "unknown" text into the statistics of the profile dialog (title, wins, losses, disconnects, rank and points) and, when asked, empties the nickname list.
/// </summary>
void __fastcall WOL_Unknown_Stats_Text(HWND dialog, bool clearNames) // RA2: 0x00739A50
{
	HWND item; // RA2: as above
	if (clearNames) { // RA2: as above
		item = GetDlgItem(dialog, PERSONA_NICKNAMES); // RA2: as above
		SendMessageA(item, PM_SET_TEXT, 0, (LPARAM)""); // RA2: as above
		InvalidateRect(item, NULL, FALSE); // RA2: as above
	}
	item = GetDlgItem(dialog, PERSONA_TITLE); // RA2: as above
	SendMessageA(item, PM_SET_TEXT_WIDE, 0, (LPARAM)L""); // RA2: as above
	WideWstring unknown(StringTable::LoadString("GUI:UnknownStats", NULL, "D:\\Ra2\\WOLPersonaInformation.cpp", 639)); // RA2: as above
	item = GetDlgItem(dialog, PERSONA_WINS); // RA2: as above
	SendMessageA(item, PM_SET_TEXT_WIDE, 0, (LPARAM)unknown.text()); // RA2: as above
	item = GetDlgItem(dialog, PERSONA_LOSSES); // RA2: as above
	SendMessageA(item, PM_SET_TEXT_WIDE, 0, (LPARAM)unknown.text()); // RA2: as above
	item = GetDlgItem(dialog, PERSONA_DISCONNECTS); // RA2: as above
	SendMessageA(item, PM_SET_TEXT_WIDE, 0, (LPARAM)unknown.text()); // RA2: as above
	item = GetDlgItem(dialog, PERSONA_RANK); // RA2: as above
	SendMessageA(item, PM_SET_TEXT_WIDE, 0, (LPARAM)unknown.text()); // RA2: as above
	item = GetDlgItem(dialog, PERSONA_POINTS); // RA2: as above
	SendMessageA(item, PM_SET_TEXT_WIDE, 0, (LPARAM)unknown.text()); // RA2: as above
}

static inline void Profile_Set_Text(HWND dialog, int id, WideWstring const & text) // RA2: the statistics are set through a helper (as in the YR decompilation, which matches the temporaries of the conditional expressions of its caller)
{
	HWND item = GetDlgItem(dialog, id); // RA2: as above
	SendMessageA(item, PM_SET_TEXT_WIDE, 0, (LPARAM)text.text()); // RA2: as above
}

/// <summary>
/// Fills the profile dialog from the profile: the nickname list (when asked), the server, the locale, country and color lists, the statistics and the title. Statistics that are not known show the "unknown" text.
/// </summary>
void PlayerProfile::PopulateDialog(HWND dialog, Wstring const * name, bool update) // RA2: 0x00739B70
{
	if (update) { // RA2: as above
		HWND item = GetDlgItem(dialog, PERSONA_NICKNAMES); // RA2: as above
		bool wide = SendMessageA(item, PM_HAS_WIDE_TEXT, 0, 0) != 0; // RA2: as above
		if (name->length() != 0 && !wide) { // RA2: as above
			int selected = SendMessageA(item, PM_FIND_STRING, -1, (LPARAM)name->text()); // RA2: as above
			SendMessageA(item, CB_SETCURSEL, selected, 0); // RA2: as above
			if (selected == -1) { // RA2: as above
				SendMessageA(item, PM_SET_TEXT, 0, (LPARAM)name->text()); // RA2: as above
			}
		} else {
			WideWstring nickname = Wstring_ToWide(*name); // RA2: as above
			int selected = -1; // RA2: as above
			if (name->length() > 0) { // RA2: as above
				selected = SendMessageA(item, PM_FIND_STRING_WIDE, -1, (LPARAM)nickname.text()); // RA2: as above
			}
			if (selected == -1) { // RA2: as above
				nickname = StringTable::LoadString("GUI:Default", NULL, "D:\\Ra2\\WOLPersonaInformation.cpp", 668); // RA2: as above
				selected = SendMessageA(item, PM_FIND_STRING_WIDE, -1, (LPARAM)nickname.text()); // RA2: as above
			}
			SendMessageA(item, CB_SETCURSEL, selected, 0); // RA2: as above
			if (selected == -1) { // RA2: as above
				SendMessageA(item, PM_SET_TEXT_WIDE, 0, (LPARAM)nickname.text()); // RA2: as above
			}
		}
	}

	HWND item = GetDlgItem(dialog, PERSONA_SERVER); // RA2: as above
	int selected = SendMessageA(item, PM_FIND_STRING, -1, (LPARAM)Name.text()); // RA2: as above
	SendMessageA(item, CB_SETCURSEL, selected, 0); // RA2: as above

	item = GetDlgItem(dialog, PERSONA_LOCALE); // RA2: as above
	if (item) { // RA2: as above
		int count = SendMessageA(item, CB_GETCOUNT, 0, 0); // RA2: as above
		for (int i = 0; i < count; i++) { // RA2: as above
			LRESULT data = SendMessageA(item, CB_GETITEMDATA, i, 0); // RA2: as above
			if (data == Locale) { // RA2: as above
				SendMessageA(item, CB_SETCURSEL, i, 0); // RA2: as above
				break; // RA2: as above
			}
		}
	}

	item = GetDlgItem(dialog, PERSONA_COUNTRY); // RA2: as above
	if (item) { // RA2: as above
		int count = SendMessageA(item, CB_GETCOUNT, 0, 0); // RA2: as above
		for (int i = 0; i < count; i++) { // RA2: as above
			LRESULT data = SendMessageA(item, CB_GETITEMDATA, i, 0); // RA2: as above
			if (data == Country) { // RA2: as above
				SendMessageA(item, CB_SETCURSEL, i, 0); // RA2: as above
				break; // RA2: as above
			}
		}
	}

	item = GetDlgItem(dialog, PERSONA_COLOR); // RA2: as above
	if (item) { // RA2: as above
		int count = SendMessageA(item, CB_GETCOUNT, 0, 0); // RA2: as above
		for (int i = 0; i < count; i++) { // RA2: as above
			LRESULT data = SendMessageA(item, CB_GETITEMDATA, i, 0); // RA2: as above
			if (data == Color) { // RA2: as above
				SendMessageA(item, CB_SETCURSEL, i, 0); // RA2: as above
				break; // RA2: as above
			}
		}
	}

	WideWstring unknown(StringTable::LoadString("GUI:UnknownStats", NULL, "D:\\Ra2\\WOLPersonaInformation.cpp", 729)); // RA2: as above

	Profile_Set_Text(dialog, PERSONA_RANK, Rank == -1 ? unknown : WideWstring_FromInt(Rank)); // RA2: as above
	Profile_Set_Text(dialog, PERSONA_POINTS, Points == -1 ? unknown : WideWstring_FromInt(Points)); // RA2: as above
	Profile_Set_Text(dialog, PERSONA_WINS, Wins == -1 ? unknown : WideWstring_FromInt(Wins)); // RA2: as above
	Profile_Set_Text(dialog, PERSONA_LOSSES, Losses == -1 ? unknown : WideWstring_FromInt(Losses)); // RA2: as above
	Profile_Set_Text(dialog, PERSONA_DISCONNECTS, Disconnects == -1 ? unknown : WideWstring_FromInt(Disconnects)); // RA2: as above

	if (name->length() > 0) { // RA2: as above
		WideWstring label(StringTable::LoadString("GUI:UserProfileLabel", NULL, "D:\\Ra2\\WOLPersonaInformation.cpp", 737)); // RA2: as above
		wchar_t * title = new wchar_t[label.length() + name->length() + 1]; // RA2: as above
		swprintf(title, label.get(), name->text()); // RA2: as above
		HWND titleItem = GetDlgItem(dialog, PERSONA_TITLE); // RA2: as above
		SendMessageA(titleItem, PM_SET_TEXT_WIDE, 0, (LPARAM)title); // RA2: as above
		delete [] title; // RA2: as above
	} else {
		HWND titleItem = GetDlgItem(dialog, PERSONA_TITLE); // RA2: as above
		SendMessageA(titleItem, PM_SET_TEXT_WIDE, 0, (LPARAM)L""); // RA2: as above
	}
}
