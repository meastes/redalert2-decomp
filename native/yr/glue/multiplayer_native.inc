
// ---------------------------------------------------------------------------------------------
// NATIVE BINDINGS (lane Y3), appended to the multiplayer unit: the game-mode initialisers'
// Allocate. In gamemd.exe each initialiser's vtable (0x007EEEBC, 0x007EEEB0, 0x007EEEA4,
// 0x007EEE98, 0x007EEE8C, 0x007EEE80) holds its __stdcall Allocate_MP* function in the Allocate
// slot (the x86 this-call's ecx goes unused, the six arguments and `ret 0x18` agree); the tree
// defines those functions and declares the members. Natively each member calls the function its
// vtable slot holds. Behaviour code for the native build; never part of an exact build.
// ---------------------------------------------------------------------------------------------
MultiplayerGameMode * MPBattleInit::Allocate(WideWstring const & title, WideWstring const & tooltip, Wstring const & inifile, Wstring const & mapfilter, bool aiallowed, int modeindex)
{
	return(Allocate_MPBattle(title, tooltip, inifile, mapfilter, aiallowed, modeindex));
}

MultiplayerGameMode * MPManBattleInit::Allocate(WideWstring const & title, WideWstring const & tooltip, Wstring const & inifile, Wstring const & mapfilter, bool aiallowed, int modeindex)
{
	return(Allocate_MPManBattle(title, tooltip, inifile, mapfilter, aiallowed, modeindex));
}

MultiplayerGameMode * MPSiegeInit::Allocate(WideWstring const & title, WideWstring const & tooltip, Wstring const & inifile, Wstring const & mapfilter, bool aiallowed, int modeindex)
{
	return(Allocate_MPSiege(title, tooltip, inifile, mapfilter, aiallowed, modeindex));
}

MultiplayerGameMode * MPUnholyAllianceInit::Allocate(WideWstring const & title, WideWstring const & tooltip, Wstring const & inifile, Wstring const & mapfilter, bool aiallowed, int modeindex)
{
	return(Allocate_MPUnholyAlliance(title, tooltip, inifile, mapfilter, aiallowed, modeindex));
}

MultiplayerGameMode * MPFreeForAllInit::Allocate(WideWstring const & title, WideWstring const & tooltip, Wstring const & inifile, Wstring const & mapfilter, bool aiallowed, int modeindex)
{
	return(Allocate_MPFreeForAll(title, tooltip, inifile, mapfilter, aiallowed, modeindex));
}

MultiplayerGameMode * MPMegawealthInit::Allocate(WideWstring const & title, WideWstring const & tooltip, Wstring const & inifile, Wstring const & mapfilter, bool aiallowed, int modeindex)
{
	return(Allocate_MPCoop(title, tooltip, inifile, mapfilter, aiallowed, modeindex));
}

