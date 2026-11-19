
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
