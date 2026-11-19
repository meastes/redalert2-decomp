
// ---------------------------------------------------------------------------------------------
// NON-EXACT NATIVE STAND-IN (lane N3, 2026-11-19): a function unmatched in the YR tree, written
// from gamemd.exe's instructions (read with native/yr/tools/yr_dis.py) in the shape of its
// matched sibling FUN_00732280 (select all combatants) above. Behaviour code for the native
// build; never part of an exact build.
// ---------------------------------------------------------------------------------------------

// gamemd.exe 0x00732950 (729 bytes): "select all units of this type", called by the command
// bar's TypeSelect button (TabClass::AI) and by the TypeSelect key's quick press
// (FUN_00732CC0). The names are the selected objects' types (and what they deploy or undeploy
// into, FUN_00732D10). First on the screen; when every such unit on the screen is already
// selected, or on a second press, across the map (DAT_00B0FE64, NavCycleMode 2).
void __fastcall FUN_00732950(void)
{
	wchar_t text[260];
	if (Unsorted::MuteSWLaunches) {
		return;
	}
	FUN_006DA770(TacticalMap);
	if (Unsorted::NavCycleMode != 2) {
		DAT_00B0FE64 = false;
	}
	NameList * names = FUN_00732D10();

	TechnoList * list;
	if (DAT_00B0FE64) {
		list = FUN_00731EB0(FUN_00732580, 0);
	} else {
		list = Collect_Owned_Visible();
		bool unselected = false;
		int count = list->Count();
		for (int i = 0; i < count; ++i) {
			TechnoClass * techno = (*list)[i];
			if (Is_Named_Selectable(techno, names) && !techno->IsSelected) {
				unselected = true;
				break;
			}
		}
		if (!unselected) {
			DAT_00B0FE64 = true;
		}
		if (DAT_00B0FE64) {
			delete list;
			list = FUN_00731EB0(FUN_00732580, 0);
		}
	}

	int count = list->Count();
	for (int i = 0; i < count; ++i) {
		TechnoClass * techno = (*list)[i];
		if (Is_Named_Selectable(techno, names)) {
			techno->Select();
		}
	}
	FUN_00731D10(text);
	FUN_00730A90(text, 240);
	Unsorted::NavCycleMode = 2;
	DAT_00B0FE58 = false;
	delete names;
	delete list;
}

// gamemd.exe 0x00733380 (614 bytes) and 0x007336C0 (624 bytes): the HealthNav and VeterancyNav keys
// (init.cpp's HealthNavCommandClass/VeterancyNavCommandClass; Shift keeps the selection). The first
// press takes the selected units (or, with none, the player's units on the screen: FUN_007331B0) and
// selects those in the worst health (the least experience) state; each further press (NavCycleMode
// 3, 4) moves to the next state (FUN_007336A0, FUN_00733990). The message names the state, or
// "mixed" when the selection is not all in it. Written from gamemd.exe's instructions with the
// matched helpers they inline (FUN_007331B0, FUN_00733640/FUN_00733930, FUN_007336A0/FUN_00733990).
// gamemd.exe's last states DAT_00845560/DAT_00845564 (initialised -1) are this file's storage: no
// other unit reads them.
static int HealthNavLast_00845560 = -1;
static int VeterancyNavLast_00845564 = -1;

static int Nav_Health_Of(TechnoClass * techno) { return(techno->GetHealthStatus()); }
static int Nav_Veterancy_Of(TechnoClass * techno) { return(techno->Veterancy.Remaining_Levels()); }

static void Nav_Cycle(bool clear, int mode, int & last, int (*state_of)(TechnoClass *),
	wchar_t const * (__fastcall * state_name)(int), int (__fastcall * next_state)(int))
{
	wchar_t text[260];
	if (Unsorted::MuteSWLaunches) {
		return;
	}
	int state;
	if (Unsorted::NavCycleMode != mode) {
		FUN_007331B0();
		FUN_006DA740(TacticalMap);
		state = 0;
	} else {
		state = next_state(last);
		if (clear) {
			FUN_006DA740(TacticalMap);
		}
	}
	TechnoList * list = DAT_00B0FE6C;
	int count = list->Count();
	for (int i = 0; i < count; ++i) {
		TechnoClass * techno = (*list)[i];
		if (state_of(techno) == state) {
			techno->Select();
		}
	}
	wchar_t const * name = state_name(state);
	TechnoList * selected = FUN_00732050();
	count = selected->Count();
	for (int k = 0; k < count; ++k) {
		if (state_of((*selected)[k]) != state) {
			name = StringTable::LoadString("MSG:Mixed", NULL, "D:\\ra2mdpost\\UICmnds.cpp", 0x62f);
			break;
		}
	}
	delete selected;
	FUN_00731D90(text, name);
	FUN_00730A90(text, 240);
	last = state;
	Unsorted::NavCycleMode = DAT_00B0FE6C->Count() ? mode : 0;
	DAT_00B0FE58 = false;
}

void __fastcall FUN_00733380(bool clear)
{
	Nav_Cycle(clear, 3, HealthNavLast_00845560, Nav_Health_Of, FUN_00733640, FUN_007336A0);
}

void __fastcall FUN_007336C0(bool clear)
{
	Nav_Cycle(clear, 4, VeterancyNavLast_00845564, Nav_Veterancy_Of, FUN_00733930, FUN_00733990);
}
