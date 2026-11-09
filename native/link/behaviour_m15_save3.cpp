// Behaviour rows (Scout1125, native M15; NOT EXACT, native/behaviour_rewrites.json):
// SidebarClass's map-layer save and load slots (see behaviour_m15_save.cpp).
#include "shared/sidebar.hpp"

// Slot 31 0x006822A0 (pass-6a decompile 006822a0: Leaf_6822a0::get) and slot
// 30 0x00682290 (Leaf_682290::get): the sidebar adds nothing of its own, the
// radar layer's save and load run.
HRESULT SidebarClass::unknown_slot31(IStream *stream)
{
    return RadarClass::unknown_slot31(stream);
}

HRESULT SidebarClass::unknown_slot30(IStream *stream)
{
    return RadarClass::unknown_slot30(stream);
}
