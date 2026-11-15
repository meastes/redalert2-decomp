
// ---------------------------------------------------------------------------------------------
// NATIVE NAME BINDING (lane Y3), appended to the conquer unit: Game::CallBack, YR 0x0048D080.
// The tree defines the function as Call_Back (void, matched) and calls it as Game::CallBack
// (bool __cdecl: the palette-fade callbacks and FUN_0052CB50 return it). gamemd.exe's code
// leaves Session.Type in eax on every path but the Internet poll (time()), so the bool the
// callers see is Session.Type's low byte; natively it is returned explicitly.
// Behaviour code for the native build; never part of an exact build.
// ---------------------------------------------------------------------------------------------
namespace Game { bool __cdecl CallBack(void); }

bool __cdecl Game::CallBack(void)
{
	Call_Back();
	return (unsigned char)Session.Type != 0;
}
