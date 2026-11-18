
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
