// Behaviour rows (Scout1125, native M15; NOT EXACT, native/behaviour_rewrites.json):
// leaves the menu-to-score loop reaches that have no recovered body under the
// name the caller uses.
#define RA2_OWNERDRAW_CONTROL_DICTIONARY
#include "shared/unknown_globals.hpp"
#include "shared/win32_api.hpp"
#include "shared/win32_window.hpp"
#include "shared/owner_draw_dialog_control.hpp"

// 0x00601710: OwnerDraw::UnknownControlIndex::Get(key, record *&) const, the
// control registry 0x00A73640's out-of-line lookup that Get_Record
// (0x005FC020 inline) calls, e.g. Default_Dialog_Proc 0x005FFB00's WM_PAINT.
// The accepted ownrdraw rows spell the same registry as wlib's
// WolDictionary<unsigned int, UnknownControlRecord> (one storage natively), and
// 0x00601710 is that template's getPointer: the UnknownControlIndex name
// forwards to it (pass-6a decompile 00601710: false when empty or absent,
// else the record's address stored and true).
extern "C" bool ra2_owner_draw_index_get(void const *self, unsigned int const &key, OwnerDraw::UnknownControlRecord *&value)
    __asm__("__ZNK9OwnerDraw19UnknownControlIndex3GetERKjRPNS_20UnknownControlRecordE");
bool ra2_owner_draw_index_get(void const *, unsigned int const &key, OwnerDraw::UnknownControlRecord *&value)
{
    OwnerDraw::UnknownControlRecord *record = 0;
    if (!unknown_control_table_a73640.Get_Pointer(key, &record) || record == 0) {
        return false;
    }
    value = record;
    return true;
}

// 0x00482E00: UnitClass::Explode 0x006FD3A0 calls it with the shake count
// (at most 6) when a vehicle blows up. The original body is a shared empty
// leaf (pass-6a decompile 00482e00: Leaf_482e00::get, `return;`; the screen
// shake was compiled out of the release build).
void __fastcall Unit_Unknown_Shake_00482e00(int)
{
}

// LoadOptionsClass's dialog entry points and menu names (LoadDlg.CPP; the
// accepted rows 0x0053B190/0x0053B1B0/0x0053B1E0 are seeds under their own
// names, the four name functions are unrecovered). From the pass-6a
// decompiles: the style (+0x04: 1 load, 2 save, 3 delete) and the
// description (+0x0C: the save's, by default the scenario's name
// UnknownIDSource +0x940) are set, then Dialog 0x0053B770 runs; the names are
// the string table's labels (0x0053C970..0x0053C9D0).
#include "shared/map_seed.hpp"
#include "shared/id_source.hpp"

char LoadOptionsClass::Load_Dialog_0053b190()
{
    unknown_04 = 1;
    Description = 0;
    return Dialog();
}

char LoadOptionsClass::Save_Dialog_0053b1b0(wchar_t *description)
{
    unknown_04 = 2;
    if (description == 0) {
        description = (wchar_t *)unknown_id_source_a3d290->Name;
    }
    Description = description;
    return Dialog();
}

char LoadOptionsClass::Delete_Dialog_0053b1e0()
{
    unknown_04 = 3;
    Description = 0;
    return Dialog();
}

wchar_t const *LoadOptionsClass::Load_Menu_Name() const
{
    return StringTable::Load_String("GUI:LoadMissionMenu", 0, "D:\\Ra2\\LoadDlg.CPP", 0x4C0);
}

wchar_t const *LoadOptionsClass::Save_Menu_Name() const
{
    return StringTable::Load_String("GUI:SaveMissionMenu", 0, "D:\\Ra2\\LoadDlg.CPP", 0x4C6);
}

wchar_t const *LoadOptionsClass::Delete_Menu_Name() const
{
    return StringTable::Load_String("GUI:DeleteMissionMenu", 0, "D:\\Ra2\\LoadDlg.CPP", 0x4CC);
}

wchar_t const *LoadOptionsClass::Saved_Message() const
{
    return StringTable::Load_String("TXT_GAME_WAS_SAVED", 0, "D:\\Ra2\\LoadDlg.CPP", 0x4D2);
}
