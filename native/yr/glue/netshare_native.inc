
// ---------------------------------------------------------------------------------------------
// NON-EXACT NATIVE DRAFT (lane YW, LAN), appended to the netshare unit: LBSaveSelections, YR
// 0x005E5F10 (793 bytes, unmatched in ra2-decomp), the counterpart of LBRestoreSelections above:
// before a network list box is refilled, the selected rows' texts go into the dictionary (every
// selected row of a multiple-selection list, else the current row unless the list has none), from
// gamemd.exe's code. Behaviour code for the native build; never part of an exact build.
// ---------------------------------------------------------------------------------------------
void LBSaveSelections(HWND win, Dictionary<WideWstring,bool> & lbdict)
{
	int count = ListBox_GetCount(win);
	WideWstring key;
	wchar_t wide[0x400];
	char narrow[0x400];
	bool value = true;

	if (count) {
		int style = GetWindowLongA(win, GWL_STYLE);
		if (style & LBS_MULTIPLESEL) {
			int i = 0; for(; i < count; i++) {
				if (ListBox_GetSel(win, i)) {
					LBGetText(win, i, wide, narrow);
					key = wide;
					lbdict.add(key, value);
				}
			}
		} else if (!(style & LBS_NOSEL)) {
			int i = ListBox_GetCurSel(win);
			if (i != LB_ERR) {
				wide[0] = 0;
				LBGetText(win, i, wide, narrow);
				key = wide;
				lbdict.add(key, value);
			}
		}
	}
}

// ---------------------------------------------------------------------------------------------
// NON-EXACT NATIVE DRAFT (lane YW, LAN): FUN_005EC9F0, YR 0x005EC9F0 (2,168 bytes, unmatched in
// ra2-decomp), the host screen's "put this entry in this slot" (the RA2 tree's Unknown_005cbf20,
// RA2 0x005CBF20, src/recovered/netshare.cpp), from gamemd.exe's code: a computer is refused (its
// slot's combo box set back, inline as gamemd.exe has it) while an observer is seated or the
// observer mode is on, and an observer boots every computer; then the slot changes, a player
// replaced by a fixed entry is rejected and the slot names sent (mode 1 and 2), the side, colour,
// start and team combo boxes follow an observer or a computer arriving or leaving (YR's five AI
// slot arrays; a computer's team is the game mode's default, vt_entry_30), the slot's controls are
// refreshed, and the computer count is kept, with mode 2 sending the lobby state.
// Behaviour code for the native build; never part of an exact build.
// ---------------------------------------------------------------------------------------------
static bool Yr_Slot_Is_Computer(NodeNameType * node)
{
	return node != NULL && (node == DAT_00AC11A4 || node == DAT_00AC11A8 || node == DAT_00AC11AC);
}

static bool Yr_Slot_Is_Fixed(NodeNameType * node)
{
	return node == NULL || node == DAT_00AC119C || node == DAT_00AC11A0 || node == DAT_00AC11B0 || node == DAT_00AC11B4
		|| Yr_Slot_Is_Computer(node);
}

static bool Yr_Slot_Is_Observer(NodeNameType * node)
{
	return node == DAT_00AC11B4 || (!Yr_Slot_Is_Fixed(node) && node->Slot.SpectatorFlag == -1);
}

// The slot's combo box shows the item whose data is `node` (hidden while it changes).
static void Yr_Slot_Combo_Select(HWND dialog, int id, NodeNameType * node)
{
	HWND combo = GetDlgItem(dialog, id);
	ShowWindow(combo, SW_HIDE);
	int count = SendDlgItemMessage(dialog, id, CB_GETCOUNT, 0, 0);
	int i = 0; for(; i < count; i++) {
		if ((NodeNameType *)SendDlgItemMessage(dialog, id, CB_GETITEMDATA, i, 0) == node) {
			SendDlgItemMessage(dialog, id, CB_SETCURSEL, i, 0);
			break;
		}
	}
	ShowWindow(combo, SW_SHOW);
}

void __fastcall FUN_005EC9F0(HWND dialog, int id, NodeNameType * node, int mode)
{
	int slot = FUN_005E9950(id);
	if (slot < 0) {
		return;
	}
	NodeNameType * previous = Session.Slots[slot];

	if (Yr_Slot_Is_Computer(node) && slot != DAT_008316A4) {
		if (Game::ObserverMode || (DAT_008316A4 > 0
				&& (Session.Slots[DAT_008316A4]->Slot.SpectatorFlag == -1 || Session.Slots[DAT_008316A4] == DAT_00AC11B4))) {
			PMessagePrintf(-1, StringTable::LoadString("TXT_NO_AI_WITH_OBSERVER", NULL, "D:\\ra2mdpost\\netshare.cpp", 0x1806));
			NodeNameType * kept = Yr_Slot_Is_Computer(previous) ? DAT_00AC11A0 : previous;
			previous = node;
			Yr_Slot_Combo_Select(dialog, id, kept);
		}
	} else if (node == DAT_00AC11B4) {
		bool booted = false;
		int i = 0; for(; i < 8; i++) {
			if (i != slot && Yr_Slot_Is_Computer(Session.Slots[i])) {
				int other = FUN_005E98F0(i);
				Yr_Slot_Combo_Select(dialog, other, DAT_00AC11A0);
				FUN_005EC9F0(dialog, other, DAT_00AC11A0, 0);
				booted = true;
			}
		}
		if (booted) {
			PMessagePrintf(-1, StringTable::LoadString("TXT_OBSERVER_BOOTED_AI", NULL, "D:\\ra2mdpost\\netshare.cpp", 0x1823));
		}
	}
	if (node == previous) {
		return;
	}
	Session.Slots[slot] = node;

	if (mode == 1 || mode == 2) {
		if (Yr_Slot_Is_Fixed(node) && !Yr_Slot_Is_Fixed(previous)) {
			if (g_pChat != NULL) {
				FUN_007A28B0(previous);
			} else {
				FUN_005DB600(previous);
			}
		}
		if (g_pChat != NULL) {
			FUN_0077DB90();
		} else {
			FUN_005DAF30();
		}
	}

	bool computer = Yr_Slot_Is_Computer(node);
	bool was_computer = Yr_Slot_Is_Computer(previous);
	bool observer = Yr_Slot_Is_Observer(node);
	bool was_observer = Yr_Slot_Is_Observer(previous);
	if (observer) {
		FUN_004E3B00(dialog, FUN_004E37D0(slot));
		SendMessage(GetDlgItem(dialog, FUN_005E98F0(slot)), 0x4F1, 0, 1);
		int color = FUN_004E41D0(slot);
		FUN_004E4770(dialog, color);
		FUN_004E49A0(dialog, color, 8);
		FUN_004E5260(dialog, FUN_004E4E60(slot));
		FUN_004E5260(dialog, FUN_004E5940(slot));
		FUN_005E9E70(dialog, slot, false);
		Session.Options.AISlots.Difficulties[slot] = -1;
		Session.Options.AISlots.Countries[slot] = -1;
		Session.Options.AISlots.Colors[slot] = -1;
	} else if (was_observer) {
		FUN_004E3A00(dialog, FUN_004E37D0(slot));
		SendMessage(GetDlgItem(dialog, FUN_005E98F0(slot)), 0x4F1, 0, 0);
		int color = FUN_004E41D0(slot);
		FUN_004E4770(dialog, color);
		FUN_004E49A0(dialog, color, -2);
		FUN_004E50C0(dialog, FUN_004E4E60(slot));
		FUN_004E5B60(dialog, FUN_004E5940(slot));
		FUN_005E9E70(dialog, slot, false);
		Session.Options.AISlots.Difficulties[slot] = -1;
		Session.Options.AISlots.Countries[slot] = -1;
		Session.Options.AISlots.Colors[slot] = -1;
		Session.Options.AISlots.Starts[slot] = -1;
		Session.Options.AISlots.Allies[slot] = -1;
	}
	if (computer) {
		FUN_005E9E70(dialog, slot, true);
		int difficulty = -1;
		if (node == DAT_00AC11A4) {
			difficulty = 2;
		} else if (node == DAT_00AC11A8) {
			difficulty = 1;
		} else if (node == DAT_00AC11AC) {
			difficulty = 0;
		}
		Session.Options.AISlots.Difficulties[slot] = difficulty;
		Session.Options.AISlots.Countries[slot] = -2;
		Session.Options.AISlots.Colors[slot] = -2;
		Session.Options.AISlots.Starts[slot] = -2;
		Session.Options.AISlots.Allies[slot] = Session.MPGameMode->vt_entry_30();
	} else if (was_computer) {
		FUN_005E9E70(dialog, slot, false);
		Session.Options.AISlots.Difficulties[slot] = -1;
		Session.Options.AISlots.Countries[slot] = -1;
		Session.Options.AISlots.Colors[slot] = -1;
		Session.Options.AISlots.Starts[slot] = -1;
		Session.Options.AISlots.Allies[slot] = -1;
	}
	if (g_pChat != NULL) {
		FUN_0078F000();
	}
	FUN_005EE510(slot);
	if (computer || was_computer || observer || was_observer) {
		int count = 0;
		int i = 0; for(; i < 8; i++) {
			if (Yr_Slot_Is_Computer(Session.Slots[i])) {
				count++;
			}
		}
		Session.Options.AIPlayers = count;
		if (mode == 2) {
			if (g_pChat != NULL) {
				FUN_00785360();
			} else {
				Net2DisplayUsers();
			}
			PumpGameopts(true, false);
		}
	}
}

// ---------------------------------------------------------------------------------------------
// NON-EXACT NATIVE DRAFT (lane YW, LAN): FUN_005EA3E0, YR 0x005EA3E0 (1,277 bytes, unmatched in
// ra2-decomp), the game-setup screens' refresh of the slots' controls from the session (the host's
// and the guest's MPlayer dialogs call it), from gamemd.exe's code. full: the slot names (FUN_005EB060),
// each slot's country picture and country, colour (the player's own first), start and team combo
// boxes, the slot icons (FUN_005EA2A0's loop), the Internet game's extra and the game mode's own
// controls (vt_entry_78). Otherwise the game mode's controls and the icons, then each part only when
// its summary changed since the last call: the slot names (DAT_00AC0D44 against FUN_005E8DC0's new
// text), the countries ("%d," per slot, DAT_00AC0B10), the colours, starts and teams (FUN_005EAB30,
// FUN_005EAC00, FUN_005EACD0 into their buffers). Behaviour code for the native build.
// ---------------------------------------------------------------------------------------------
void __fastcall FUN_005EA3E0(HWND dialog, bool full)
{
	int i;
	if (full) {
		FUN_005EB060(dialog);
		for (i = 0; i < 8; i++) {
			FUN_00603D30(GetDlgItem(dialog, FUN_004E3320(i)), FUN_004E3560(FUN_005EA920(i)));
		}
		for (i = 0; i < 8; i++) {
			FUN_004E3F70(dialog, FUN_004E37D0(i), FUN_005EA920(i));
		}
		int own = FUN_005E8D40();
		if (own != -1) {
			FUN_004E49A0(dialog, FUN_004E41D0(own), FUN_005EA9B0(own));
		}
		for (i = 0; i < 8; i++) {
			if (i != own) {
				FUN_004E49A0(dialog, FUN_004E41D0(i), FUN_005EA9B0(i));
			}
		}
		for (i = 0; i < 8; i++) {
			FUN_004E5480(dialog, FUN_004E4E60(i), FUN_005EAA10(i));
		}
		for (i = 0; i < 8; i++) {
			FUN_004E5ED0(dialog, FUN_004E5940(i), FUN_005EAA70(i));
		}
		FUN_005EA2A0(dialog);
		if (Session.Type == GAME_INTERNET) {
			FUN_007A0E50();
		}
		if (Session.MPGameMode != NULL) {
			Session.MPGameMode->vt_entry_78(dialog);
		}
		return;
	}

	if (Session.MPGameMode != NULL) {
		Session.MPGameMode->vt_entry_78(dialog);
	}
	FUN_005EA2A0(dialog);
	ra2_wcscpy(DAT_00AC0D44, DAT_00AC0B94);
	FUN_005E8DC0(DAT_00AC0B94);
	if (ra2_wcscmp(DAT_00AC0D44, DAT_00AC0B94) != 0) {
		FUN_005EB060(dialog);
		if (Session.Type == GAME_INTERNET) {
			FUN_007A0E50();
		}
	}
	for (i = 0; i < 8; i++) {
		FUN_00603D30(GetDlgItem(dialog, FUN_004E3320(i)), FUN_004E3560(FUN_005EA920(i)));
	}
	strcpy(DAT_00AC0EA4, DAT_00AC0B10);
	DAT_00AC0B10[0] = 0;
	for (i = 0; i < 8; i++) {
		char number[16];
		ra2_sprintf(number, "%d,", FUN_005EA920(i));
		strcat(DAT_00AC0B10, number);
	}
	if (strcmp(DAT_00AC0EA4, DAT_00AC0B10) != 0) {
		for (i = 0; i < 8; i++) {
			FUN_004E3F70(dialog, FUN_004E37D0(i), FUN_005EA920(i));
		}
	}
	strcpy(DAT_00AC0AC8, DAT_00AC0AEC);
	FUN_005EAB30(DAT_00AC0AEC);
	if (strcmp(DAT_00AC0AC8, DAT_00AC0AEC) != 0) {
		FUN_005EAEE0(dialog);
	}
	strcpy(DAT_00AC0A9C, DAT_00AC0B6C);
	FUN_005EAC00(DAT_00AC0B6C);
	if (strcmp(DAT_00AC0A9C, DAT_00AC0B6C) != 0) {
		FUN_005EADA0(dialog);
	}
	strcpy(DAT_00AC0278, DAT_00AC0B48);
	FUN_005EACD0(DAT_00AC0B48);
	if (strcmp(DAT_00AC0278, DAT_00AC0B48) != 0) {
		FUN_005EAE40(dialog);
	}
}

// ---------------------------------------------------------------------------------------------
// NON-EXACT NATIVE DRAFT (lane YW2, LAN): DecodePubGameopt, YR 0x005E3D10 (8,702 bytes, unmatched in
// ra2-decomp), from gamemd.exe's code; the RA2 tree's twin is Decode_Pub_Gameopt_005c3ff0 (RA2
// 0x005C3FF0, src/recovered/netshare.cpp, a parked draft) and TibSun's netshare.cpp DecodePubGameopt
// the shape. A public options string from another player of the network game, by its first letter:
//   W  the random map's size and start points (Scen's Width .. HouseHomeCells), the setup screen redrawn;
//   O  the observer slot moved: the slot's side, colour and combo boxes follow (DAT_008316A4);
//   A  a player's accept state (Net2SetAccept);
//   K  a joiner's map state (K0 no map, K1 has it, K2 needs a transfer: FUN_005E39C0), host only;
//   G  (Westwood Online's game screen only) the host asks to start;
//   L  the slot names (FUN_005E8EA0);
//   anything else: the host's full options (FUN_005DBB60's encoding): seed, speeds, money, units, the
//   flags, the mode, difficulty, the map (name, players, official, length, file, digest), each player's
//   name, country, colour, start and team, then the five AI slot arrays. A map not present is
//   reported (K0/K2 back to the host), changes clear this player's accept, and the screen is redrawn.
// Returns whether the map changed. Westwood Online's parts are left out, as Westwood Online is not
// built (the Internet game, Session.Type GAME_INTERNET, is never set natively): the NAT state Z, the
// pings P, the co-op campaign C (its game mode's slots take the score blocks as 32-bit words), the
// chat-channel users (g_pChat), the Internet players' addresses in the full options and the
// channel's game screen (dialog 0xC9) in L.
// Behaviour code for the native build; never part of an exact build.
// ---------------------------------------------------------------------------------------------
extern int DAT_00AC1178;
extern int Frame;
int __fastcall FUN_004E37D0(int side);
int __fastcall FUN_004E41D0(int side);
void __fastcall FUN_004E3A00(HWND dialog, int id);
void __fastcall FUN_004E3B00(HWND dialog, int id);
void __fastcall FUN_004E45A0(HWND dialog, int id);
void __fastcall FUN_004E4770(HWND dialog, int id);
void __fastcall FUN_004E49A0(HWND dialog, int id, int wanted);
void __fastcall FUN_004E4FC0(int count);
void __fastcall FUN_004E5310(HWND dialog);
void __fastcall FUN_004E5D60(HWND dialog);
void __fastcall Net2SetAccept(char * who, int status);
int __fastcall Net2SetHouseAndColor(char * who, int house, int color, int start, int team);
void Net2DisplayUsers(void);
void FUN_005E2340(void);
WideWstring __fastcall FUN_007B83E0(Wstring const & text);

// A setup screen's slot control (0x411 .. 0x418 for slots 0 .. 7).
static int Yr_Slot_Control_Id(int slot)
{
	return (slot >= 0 && slot < 8) ? 0x411 + slot : -1;
}

// The options last decoded (gamemd.exe 0x008316EC .. 0x008316F8 and 0x00AC11C3 .. 0x00AC11C7).
static int Yr_Last_Game_Speed = -1;
static int Yr_Last_Credits = -1;
static int Yr_Last_Unit_Count = -1;
static int Yr_Last_Camp_Difficulty = -1;
static bool Yr_Last_Short_Game;
static bool Yr_Last_SW_Allowed;
static bool Yr_Last_Build_Off_Ally;
static bool Yr_Last_MCV_Redeploy;
static bool Yr_Last_Goodies;

// The AI slots' countries, starts and allies as text, to see whether they changed.
static void Yr_AI_Slots_Text(char * out)
{
	out[0] = 0;
	for (int i = 0; i < 8; i++) {
		char number[40];
		sprintf(number, "%d,%d,%d,", Session.Options.AISlots.Countries[i], Session.Options.AISlots.Starts[i],
			Session.Options.AISlots.Allies[i]);
		strcat(out, number);
	}
}

bool DecodePubGameopt(char * options, char * name)
{
	bool changed = false;
	char * copy = strdup(options);
	char * token;

	if (copy[0] == 'Z') {
		FUN_004068E0("Decoding public OPTION_NAT\n");
		free(copy);
		return(false);
	}

	if (copy[0] == 'W') {
		FUN_004068E0("Saw Waypoint info [%s]\n", copy);
		int width = 0, height = 0, x = 0, y = 0, count = 0;
		int px[8] = {}, py[8] = {}, cx[8] = {}, cy[8] = {};
		token = strtok(copy + 1, ",");
		if (token) width = atol(token);
		token = strtok(NULL, ",");
		if (token) height = atol(token);
		token = strtok(NULL, ",");
		if (token) x = atol(token);
		token = strtok(NULL, ",");
		if (token) y = atol(token);
		token = strtok(NULL, ",");
		if (token) count = atol(token);
		for (int i = 0; i < 8; i++) {
			token = strtok(NULL, ",");
			if (token) px[i] = atol(token);
			token = strtok(NULL, ",");
			if (token) py[i] = atol(token);
			token = strtok(NULL, ",");
			if (token) cx[i] = atol(token);
			token = strtok(NULL, ",");
			if (token) cy[i] = atol(token);
		}
		if (token && Scen) {
			Scen->Width = width;
			Scen->Height = height;
			Scen->StartX = x;
			Scen->StartY = y;
			Scen->NumberStartingPoints = count;
			for (int i = 0; i < 8; i++) {
				Scen->StartingPoints[i].X = px[i];
				Scen->StartingPoints[i].Y = py[i];
				Scen->HouseHomeCells[i].X = (short)cx[i];
				Scen->HouseHomeCells[i].Y = (short)cy[i];
			}
			HWND dialog = WS_Find_Dialog(0xC9);
			if (!dialog) dialog = WS_Find_Dialog(IDD_MPLAYER_GUEST);
			if (dialog) InvalidateRect(dialog, NULL, FALSE);
		}
		free(copy);
		return(false);
	}

	if (copy[0] == 'O') {
		HWND dialog = WS_Find_Dialog(0xC9);
		if (!dialog) dialog = WS_Find_Dialog(IDD_MPLAYER_GUEST);
		if (!dialog) {
			free(copy);
			return(false);
		}
		int old = DAT_008316A4;
		DAT_008316A4 = copy[1] == '8' ? -1 : copy[1] - '0';
		if (DAT_008316A4 != old) {
			FUN_005EBA00(dialog);
			wchar_t who[256];
			wcscpy(who, FUN_00735120(copy + 2));
			if (who[0]) {
				NodeNameType * entry;
				{
					WideWstring wide(who);
					entry = FUN_005EDFC0(&wide);
				}
				if (!entry) {
					entry = FUN_005E8BF0(who);
					if (!entry) {
						free(copy);
						return(false);
					}
				}
				int slot = DAT_008316A4;
				if (Yr_Slot_Is_Observer(entry)) {
					Session.Slots[slot] = entry;
					FUN_004E3B00(dialog, FUN_004E37D0(slot));
					SendMessage(GetDlgItem(dialog, Yr_Slot_Control_Id(slot)), 0x4F1, 0, 1);
					int id = FUN_004E41D0(slot);
					FUN_004E4770(dialog, id);
					FUN_004E49A0(dialog, id, 8);
				} else {
					FUN_004E3A00(dialog, FUN_004E37D0(slot));
					SendMessage(GetDlgItem(dialog, Yr_Slot_Control_Id(slot)), 0x4F1, 0, 0);
					int id = FUN_004E41D0(slot);
					FUN_004E45A0(dialog, id);
					FUN_004E49A0(dialog, id, -2);
				}
				Session.Slots[DAT_008316A4] = entry;
			}
		}
		if (DAT_008316A4 >= 0 && DAT_008316A4 != Session.unknown_2854) {
			FUN_005E9E70(dialog, DAT_008316A4, false);
		}
		free(copy);
		return(false);
	}

	if (copy[0] == 'A') {
		Net2SetAccept(name, atol(copy + 1));
		free(copy);
		return(false);
	}

	if (copy[0] == 'C') {
		FUN_004068E0("@@@@ Decoding cooperative campaign game options %s\n", options);
		free(copy);
		return(false);
	}

	if (copy[0] == 'L') {
		WideWstring names = FUN_007B83E0(Wstring(options + 1));
		FUN_005E8EA0(names.get());
		free(copy);
		return(false);
	}

	if (copy[0] == 'G') {
		if (WS_Find_Dialog(0xC9)) {
			Session.unknown_2878 = true;
			FUN_005E2340();
		}
		free(copy);
		return(false);
	}

	if (copy[0] == 'K') {
		if (WS_Find_Dialog(0xC2) || WS_Find_Dialog(IDD_MPLAYER_HOST)) {
			FUN_005E39C0(name, copy[1] == '1' ? 1 : (copy[1] == '2' ? 2 : 0));
		}
		free(copy);
		return(false);
	}

	if (copy[0] == 'P') {
		FUN_004068E0("Got Ping game options string. %s\n", copy);
		free(copy);
		return(false);
	}

	int top = WS_Top_Window_ID();
	if (top != IDD_MPLAYER_HOST && top != IDD_MPLAYER_GUEST) {
		SendDlgItemMessage(GameoptWindow(), 0x53E, 0x49D, 0, 1);
	}
	FUN_004068E0("Decoding game options %s\n", options);

	token = strtok(copy, ",");
	if (token) Seed = atol(token);
	token = strtok(NULL, ",");
	if (token) Options.GameSpeed = atol(token);
	token = strtok(NULL, ",");
	if (token) Session.Options.GameSpeed = atol(token);
	token = strtok(NULL, ",");
	if (token) Session.Options.Credits = atol(token);
	token = strtok(NULL, ",");
	if (token) Session.Options.UnitCount = atol(token);
	token = strtok(NULL, ",");
	if (token) Session.Options.ShortGame = atol(token) != 0;
	token = strtok(NULL, ",");
	if (token) Session.Options.SWAllowed = atol(token) != 0;
	token = strtok(NULL, ",");
	if (token) Session.Options.BuildOffAlly = atol(token) != 0;
	token = strtok(NULL, ",");
	if (token) Session.Options.MCVRedeploy = atoi(token) != 0;
	token = strtok(NULL, ",");
	if (token) Session.Options.Goodies = atoi(token) != 0;
	token = strtok(NULL, ",");
	if (token) Session.Options.MPModeIndex = atol(token);
	token = strtok(NULL, ",");
	if (!token) {
		free(copy);
		return(false);
	}
	Options.CampDifficulty = atol(token);

	bool same = true;
	token = strtok(NULL, ",");
	WideWstring scenario_name(token ? FUN_007B83E0(Wstring(token)) : WideWstring(L""));

	int old_players = Session.unknown_8D4;
	token = strtok(NULL, ",");
	if (token) {
		Session.unknown_8D4 = atoi(token);
		if (Session.unknown_8D4 != old_players) same = false;
	}
	bool old_official = Session.ScenarioIsOfficial;
	token = strtok(NULL, ",");
	if (token) {
		Session.ScenarioIsOfficial = atoi(token) != 0;
		if (Session.ScenarioIsOfficial != old_official) same = false;
	}
	unsigned int old_length = Session.ScenarioFileLength;
	token = strtok(NULL, ",");
	if (token) {
		Session.ScenarioFileLength = atoi(token);
		if (Session.ScenarioFileLength != old_length) same = false;
	}
	token = strtok(NULL, ",");
	if (token) {
		if (stricmp(Session.ScenarioFileName, token) != 0) same = false;
		strncpy(Session.ScenarioFileName, token, 0x202);
		strcpy(Scen->ScenarioName, Session.ScenarioFileName);
	}
	char * digest = strtok(NULL, ":");
	if (digest) {
		if (strcmp(Session.ScenarioDigest, digest) != 0) same = false;
		strncpy(Session.ScenarioDigest, digest, 0x21);
	}

	MultiplayerGameMode * mode = MultiplayerGameMode::Get(Session.Options.MPModeIndex);
	if (mode != Session.MPGameMode) same = false;
	if (top == IDD_MPLAYER_GUEST || top == 0xC9) {
		FUN_005EBDD0(GameoptWindow(), old_players, Session.unknown_8D4, mode);
	}

	bool found = true;
	if (!same || wcslen(Session.Options.ScenarioDescription) == 0) {
		found = false;
		same = false;
		for (int i = 0; i < Session.Scenarios.Count(); i++) {
			MultiMission * scenario = Session.Scenarios[i];
			if (stricmp(Session.ScenarioFileName, scenario->Get_Filename()) != 0) continue;
			CCFileClass file(scenario->Get_Filename());
			if (file.Is_Available(false) && (unsigned int)file.Size() == Session.ScenarioFileLength) {
				INIClass ini;
				ini.Load(file, false);
				char buffer[32];
				ini.Get_String("Digest", "1", "No digest here mate. Nope.", buffer, sizeof(buffer));
				if (Session.ScenarioDigest[0] == 0 || strcmp(Session.ScenarioDigest, buffer) == 0) {
					wcscpy(Session.Options.ScenarioDescription, scenario->Description());
					found = true;
				}
			}
		}
		if (!found && scenario_name.length() > 0) {
			wcscpy(Session.Options.ScenarioDescription, scenario_name.get());
		}
		if (Session.MPGameMode) Session.MPGameMode->vt_entry_24();
	}

	mode = MultiplayerGameMode::Get(Session.Options.MPModeIndex);
	if (mode != Session.MPGameMode) {
		if (Session.MPGameMode) Session.MPGameMode->vt_entry_9C();
		if (mode) mode->vt_entry_20();
	}
	Session.MPGameMode = mode;
	if (mode && !same) {
		mode->UnfixAlliances();
		FUN_005EE3A0();
	}
	HWND dialog = GameoptWindow();
	if (mode) SendMessage(GetDlgItem(dialog, 0x6EC), 0x4B2, 0, (LPARAM)mode->UIName.text());
	SendMessage(GetDlgItem(dialog, 0x5A8), 0x4B2, 0, (LPARAM)Session.Options.ScenarioDescription);
	Scen->Scenario = -1;
	Frame = 0;
	Session.CommProtocol = (CommProtocolType)2;

	if (old_official != Session.ScenarioIsOfficial || old_length != Session.ScenarioFileLength) same = false;
	if (!same) {
		FUN_004068E0("Not same scenario...\n");
		bool random = stricmp(Session.ScenarioFileName, "RandMap.Sed") == 0;
		char const * state;
		if (Session.ScenarioIsOfficial && !found && !random) {
			if (GameoptWindow()) {
				PMessagePrintf(-1, StringTable::LoadString("GUI:JoinerNoMap", NULL, "D:\\ra2mdpost\\netshare.cpp", 0x814),
					Session.Options.ScenarioDescription);
			}
			DAT_00AC11C2 = false;
			EnableWindow(GetDlgItem(GameoptWindow(), 0x59F), FALSE);
			InvalidateRect(GetDlgItem(GameoptWindow(), 0x59F), NULL, FALSE);
			state = "K0";
		} else {
			DAT_00AC11C2 = true;
			EnableWindow(GetDlgItem(GameoptWindow(), 0x59F), TRUE);
			InvalidateRect(GetDlgItem(GameoptWindow(), 0x59F), NULL, FALSE);
			if (!Session.ScenarioIsOfficial && !found && !random) {
				if (GameoptWindow()) {
					PMessagePrintf(-1, StringTable::LoadString("GUI:JoinerMapTransfer", NULL, "D:\\ra2mdpost\\netshare.cpp", 0x824),
						Session.Options.ScenarioDescription);
				}
				state = "K2";
			} else {
				state = "K1";
			}
		}
		SendPublicGameopts(state);
		Update_Network_Dialog_Preview(GameoptWindow());
		int count = Session.unknown_8D4;
		if (Session.MPGameMode && Session.MPGameMode->vt_entry_04() && Scen->NumCoopHumanStartSpots <= count) {
			count = Scen->NumCoopHumanStartSpots;
		}
		FUN_004E4FC0(count);
		if (top == IDD_MPLAYER_GUEST || top == 0xC9) {
			FUN_004E5310(WS_Top_Window());
			FUN_004E5D60(WS_Top_Window());
			FUN_005EE510(-1);
		}
	}
	if (!digest) {
		PMessagePrintf(-1, L"Gameopt parse failed");
	}

	for (char * who = strtok(NULL, ",:"); who && strcmp(who, "@") != 0; who = strtok(NULL, ",:")) {
		token = strtok(NULL, ",:");
		if (!token) break;
		int country = atol(token);
		token = strtok(NULL, ",:");
		if (!token) break;
		int color = atol(token);
		token = strtok(NULL, ",:");
		if (!token) break;
		int start = atol(token);
		token = strtok(NULL, ",:");
		if (!token) break;
		int team = atol(token);
		token = strtok(NULL, ",:");
		if (!token) break;
		// The player's address and two more words: an Internet game's only (the NAT relay).
		strtok(NULL, ",:");
		strtok(NULL, ",:");
		DAT_00AC1178 = 1;
		if (stricmp(who, Session.Handle) == 0 && color != Session.PrefColor) {
			if (DAT_00AC1178 == 0) {
				PMessagePrintf(-1, StringTable::LoadString("TXT_COLOR_IN_USE", NULL, "D:\\ra2mdpost\\netshare.cpp", 0x8b6));
			}
			Session.PrefColor = color;
		}
		if (stricmp(who, Session.Handle) == 0) {
			DAT_00AC1178 = 0;
		}
		if (Net2SetHouseAndColor(who, country, color, start, team)) {
			changed = true;
		}
	}

	char before[200];
	Yr_AI_Slots_Text(before);
	int ai = 0;
	for (int k = 0; k < 8; k++) {
		token = strtok(NULL, ",:");
		if (!token) break;
		Session.Options.AISlots.Difficulties[k] = atol(token);
		token = strtok(NULL, ",:");
		if (!token) break;
		Session.Options.AISlots.Countries[k] = atol(token);
		token = strtok(NULL, ",:");
		if (!token) break;
		Session.Options.AISlots.Colors[k] = atol(token);
		if (Session.Options.AISlots.Countries[k] != -1) ai++;
		token = strtok(NULL, ",:");
		if (!token) break;
		Session.Options.AISlots.Starts[k] = atol(token);
		token = strtok(NULL, ",:");
		if (!token) break;
		Session.Options.AISlots.Allies[k] = atol(token);
	}
	Session.Options.AIPlayers = ai;
	char after[200];
	Yr_AI_Slots_Text(after);
	if (strcmp(before, after) != 0) changed = true;
	free(copy);

	DisplayGameopts(GameoptWindow(), FALSE);

	if (Yr_Last_Game_Speed != Session.Options.GameSpeed) changed = true;
	if (Yr_Last_Credits != Session.Options.Credits) changed = true;
	if (Yr_Last_Unit_Count != Session.Options.UnitCount) changed = true;
	if (Yr_Last_Short_Game != Session.Options.ShortGame) changed = true;
	if (Yr_Last_SW_Allowed != Session.Options.SWAllowed) changed = true;
	if (Yr_Last_Build_Off_Ally != Session.Options.BuildOffAlly) changed = true;
	if (Yr_Last_MCV_Redeploy != Session.Options.MCVRedeploy) changed = true;
	if (Yr_Last_Goodies != Session.Options.Goodies) changed = true;
	if (Yr_Last_Camp_Difficulty != Options.CampDifficulty) changed = true;

	if (!same || changed) {
		FUN_005E3AB0();
	}
	if (top != IDD_MPLAYER_HOST && top != IDD_MPLAYER_GUEST) {
		SendDlgItemMessage(GameoptWindow(), 0x53E, 0x49D, 0, 0);
	}
	Net2DisplayUsers();

	Yr_Last_Game_Speed = Session.Options.GameSpeed;
	Yr_Last_Credits = Session.Options.Credits;
	Yr_Last_Unit_Count = Session.Options.UnitCount;
	Yr_Last_Short_Game = Session.Options.ShortGame;
	Yr_Last_SW_Allowed = Session.Options.SWAllowed;
	Yr_Last_Build_Off_Ally = Session.Options.BuildOffAlly;
	Yr_Last_MCV_Redeploy = Session.Options.MCVRedeploy;
	Yr_Last_Goodies = Session.Options.Goodies;
	Yr_Last_Camp_Difficulty = Options.CampDifficulty;

	return(!same);
}
