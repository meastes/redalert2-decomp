// Behaviour rows (Scout1281 r32; NOT EXACT, native/behaviour_rewrites.json): MapSeedClass's
// LoadOptionsClass slots, which the random map dialog's Load Map, Save Map and Delete Map
// (Map_Seed_Dialog_Proc 0x00577BD0 -> LoadOptionsClass::Dialog 0x0053B770) call. The accepted
// rows at these addresses are seed scaffolding (src/recovered/classes/MapSeedClass/005794d0.cpp,
// 005796f0.cpp ...), so the virtual slots were link-closure traps.
#include "shared/map_seed.hpp"
#include "shared/string_table.hpp"
#include "shared/load_dlg.hpp"

// 0x005794D0: mov eax, [esp+4]; push eax; call 0x0053C7D0 (Delete_Save_File); ret 4.
bool MapSeedClass::Delete_File(char const *filename)
{
    return Delete_Save_File(filename);
}

// 0x005796F0, 0x00579710, 0x00579730, 0x00579750: Load_String(label, 0, "D:\Ra2\MapGen.cpp",
// line) with the labels at 0x007E16F4, 0x007E1704, 0x007E1714, 0x007E1728 and the lines pushed.
wchar_t const *MapSeedClass::Load_Menu_Name() const
{
    return StringTable::Load_String("GUI:LoadMapMenu", 0, "D:\\Ra2\\MapGen.cpp", 0x1526);
}

wchar_t const *MapSeedClass::Save_Menu_Name() const
{
    return StringTable::Load_String("GUI:SaveMapMenu", 0, "D:\\Ra2\\MapGen.cpp", 0x152C);
}

wchar_t const *MapSeedClass::Delete_Menu_Name() const
{
    return StringTable::Load_String("GUI:DeleteMapMenu", 0, "D:\\Ra2\\MapGen.cpp", 0x1532);
}

wchar_t const *MapSeedClass::Saved_Message() const
{
    return StringTable::Load_String("GUI:MapSaved", 0, "D:\\Ra2\\MapGen.cpp", 0x1538);
}
