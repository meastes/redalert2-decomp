
// ---------------------------------------------------------------------------------------------
// NON-EXACT NATIVE DRAFT (lane Y3): Wait_For_Players_To_Load (gamemd 0x00684370) is unmatched in
// the YR tree. gamemd.exe returns true at once for a campaign or skirmish game, which is all this
// draft covers; the network wait (timers, the load-progress exchange with the other players,
// the serial port check) is not written yet and the network kinds also return true here.
// Behaviour code for the native build only.
// ---------------------------------------------------------------------------------------------
bool Wait_For_Players_To_Load(void)
{
	// gamemd.exe: GAME_NORMAL and GAME_SKIRMISH return true here; NATIVE: so does every other
	// kind until the network wait is written (see above).
	return(true);
}
