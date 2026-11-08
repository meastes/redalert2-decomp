// Behaviour-written bodies of unmatched rows the skirmish harness reaches once
// the multiplayer setup runs as in a real skirmish (Scout1091, native M8;
// --run-to=frames). NOT EXACT; never original code or bytes.
#include "shared/sidebar.hpp"
#include "shared/map_view.hpp"

// 0x006816B0 SidebarClass::SBGadgetClass::Action (slot 31, the class's key
// function: without it the vtable is a zero placeholder and Add_A_Button's
// virtual Head_Of_List call jumps to 0). The accepted text is a seed outside
// the class (src/recovered/classes/SidebarClass__SBGadgetClass/006816b0.cpp):
// Map's MouseClass::Override_Mouse_Shape (0x0059F250, called directly) with
// the normal mouse, then true. TibSun sidebar.cpp has the same body.
int SidebarClass::SBGadgetClass::Action(unsigned flags, unsigned &key)
{
    Map.MouseClass::Override_Mouse_Shape(0, false);
    return 1;
}

#include "shared/gadget.hpp"

// 0x004CF160 GadgetClass::Is_Enabled (seed src/recovered/004cf160.cpp, a
// __fastcall leaf): the gadget's +0x1E IsDisabled byte is clear.
bool GadgetClass::Is_Enabled()
{
    return !IsDisabled;
}
