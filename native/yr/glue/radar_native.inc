// ---------------------------------------------------------------------------------------------
// NON-EXACT NATIVE STAND-IN (lane N5), appended to the radar unit: RadarClass::FUN_00658330 (gamemd.exe
// 0x00658330, 775 bytes), unmatched in the YR tree. Written from gamemd.exe's instructions (yr_dis.py),
// not compared byte for byte. Behaviour code for the native build; never part of an exact build.
//
// The Diplomacy dialog (Tab in a skirmish or a network game: Keyboard_Process -> FUN_006ABC40 sets
// SpecialDialog 8 -> RadarClass::FUN_006586D0) fills one player's row through FUN_00658640 when it opens
// and on its 250 ms timer: the name in the house's colours, the country's flag, the alliance and chat
// check boxes, the kill count. It was a link-closure trap: Tab in a Yuri's Revenge skirmish stopped the
// game.
// ---------------------------------------------------------------------------------------------

Surface * __fastcall FUN_004E3560(int index);
void __fastcall FUN_00603D30(HWND item, Surface * surface);

void RadarClass::FUN_00658330(HWND dialog, int index, HouseClass * house)
{
	int name_id = DiplomacyNameIDs[index];
	int kills_id = DiplomacyKillsIDs[index];
	wchar_t text[92];
	text[0] = 0;

	if (house == NULL || !house->Class->IsMultiplay) {
		SendMessage(GetDlgItem(dialog, name_id), 0x4b2, 0, (LPARAM)text);
		SendMessage(GetDlgItem(dialog, kills_id), 0x4b2, 0, (LPARAM)text);
		return;
	}

	wcscpy(text, house->UIName);

	/*
	**	The row's colour: the observer's light grey, a defeated player's grey, else the house's scheme.
	*/
	ColorScheme * scheme;
	if (house == DAT_00AC1198) {
		scheme = Fetch_Scheme_By_Name("LightGrey", 1);
	} else if (house->IsDefeated) {
		scheme = Fetch_Scheme_By_Name("Grey", 1);
	} else {
		scheme = ColorSchemes[house->Scheme];
	}
	RGBClass rgb = (RGBClass)scheme->HSV;
	LPARAM color = (LPARAM)rgb.Get_Red() | ((LPARAM)rgb.Get_Green() << 8) | ((LPARAM)rgb.Get_Blue() << 16);

	HWND item = GetDlgItem(dialog, name_id);
	SendMessage(item, 0x498, 0, color);
	SendMessage(item, 0x4b2, 0, (LPARAM)text);

	/*
	**	The country's flag (the observer's own picture).
	*/
	HWND flag = GetDlgItem(dialog, FUN_004E3320(index));
	Surface * picture = FUN_004E3560(house->Class->Side);
	if (house == DAT_00AC1198) {
		picture = FUN_004E3560(-3);
	}
	FUN_00603D30(flag, picture);

	int alliance_id = DiplomacyAllianceIDs[index];
	if (alliance_id != -1) {
		HWND alliance = GetDlgItem(dialog, alliance_id);
		if (Unsorted::MuteSWLaunches) {
			EnableWindow(alliance, FALSE);
		}
		if (house->IsDefeated) {
			ShowWindow(alliance, SW_HIDE);
		} else {
			SendMessage(alliance, BM_SETCHECK, PlayerPtr->Is_Ally(house), 0);
		}
		FUN_00603DD0(alliance, house->Is_Ally(PlayerPtr));
	}

	if (house->IsGiverUpper) {
		ShowWindow(GetDlgItem(dialog, DiplomacyChatIDs[index]), SW_HIDE);
	} else if (Unsorted::MuteSWLaunches) {
		HWND chat = GetDlgItem(dialog, DiplomacyChatIDs[index]);
		SendMessage(chat, BM_SETCHECK, 1, 0);
		EnableWindow(chat, FALSE);
	}

	/*
	**	The kills: units and buildings, every kind of house counted.
	*/
	int kills = 0;
	for (int i = 0; i < 20; i++) {
		kills += house->UnitsKilled[i] + house->BuildingsKilled[i];
	}
	swprintf(text, L"%d", kills);
	item = GetDlgItem(dialog, kills_id);
	SendMessage(item, 0x498, 0, color);
	SendMessage(item, 0x4b2, 0, (LPARAM)text);
}
