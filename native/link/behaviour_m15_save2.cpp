// Behaviour rows (Scout1125, native M15; NOT EXACT, native/behaviour_rewrites.json):
// Tactical's and SidebarClass's save members (see behaviour_m15_save.cpp).
#include "shared/tactical.hpp"

// 0x006AC400 (pass-6a decompile 006ac400): the base save, success as S_OK
// (Tactical::Load 0x006AC320 reads it back).
HRESULT __stdcall Tactical::Save(IStream *stream, BOOL clear_dirty)
{
    HRESULT result = AbstractClass::Save(stream, clear_dirty);
    if (result >= 0) result = S_OK;
    return result;
}
