
// ---------------------------------------------------------------------------------------------
// NON-EXACT NATIVE DRAFT (lane Y3, partial): FUN_0063AB60 (YR 0x0063AB60), the planning-mode
// test ScrollClass::Scroll_AI makes for the mouse position, is unmatched in the YR tree.
// gamemd.exe returns false at once unless planning mode is on, which is what this draft writes;
// its planning-mode body (the waypoint under the cursor) is not written yet and returns false
// too. Behaviour code for the native build only.
// ---------------------------------------------------------------------------------------------
bool __stdcall FUN_0063AB60(int x, int y)
{
	// gamemd.exe: false unless PlanningNodeClass::PlanningModeActive; NATIVE: false in planning
	// mode as well until its body is written (see above).
	return(false);
}

// FUN_0063A5A0 (YR 0x0063A5A0, partial, as above): the planning-mode handling of a left button
// press at the mouse position (ScrollClass::Message_Handler). gamemd.exe returns false unless
// planning mode is on; NATIVE: false in planning mode too until its body is written.
bool __stdcall FUN_0063A5A0(int x, int y)
{
	return(false);
}
