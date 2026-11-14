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
// WolDictionary<unsigned int, WinData> (one storage natively), and
// 0x00601710 is that template's getPointer: the UnknownControlIndex name
// forwards to it (pass-6a decompile 00601710: false when empty or absent,
// else the record's address stored and true).
extern "C" bool ra2_owner_draw_index_get(void const *self, unsigned int const &key, OwnerDraw::WinData *&value)
    __asm__("__ZNK9OwnerDraw19UnknownControlIndex3GetERKjRPNS_7WinDataE");
bool ra2_owner_draw_index_get(void const *, unsigned int const &key, OwnerDraw::WinData *&value)
{
    OwnerDraw::WinData *record = 0;
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
// ScenarioClass +0x940) are set, then Dialog 0x0053B770 runs; the names are
// the string table's labels (0x0053C970..0x0053C9D0).
#include "shared/map_seed.hpp"
#include "shared/id_source.hpp"

#include "shared/load_dlg.hpp"   // WWMouse, the dialog helpers
#include "shared/dialog_create.hpp"   // unknown_create_dialog_5ff610
#include <stdio.h>
#include "shared/cd.hpp"
#include "shared/campaign.hpp"
extern "C" const char *ra2port_take_resume(void);   // port/src/port_core.cpp

char LoadOptionsClass::Load_Dialog_0053b190()
{
    unknown_04 = 1;
    Description = 0;
    // Resume at start (lane I, NATIVE, not original behaviour): the host asked for a
    // checkpoint to be loaded (ra2port::request_resume). Dialog 0x0053B770's own steps
    // for a load, with the row already chosen: the load dialog 0xB7 is made, filled and
    // shown, then hidden; the disk the row's game needs; the mouse hidden; Load
    // (0x0053C680); the list freed and the dialog closed (0x005FF6E0). True goes back to
    // Select_Game as after a load from the list; a failed load shows the dialog as usual.
    char const *file = ra2port_take_resume();
    if (file) {
        HWND dialog = unknown_create_dialog_5ff610(0xB7, (DialogProc)Load_Dialog_Proc, 0);
        if (dialog) {
            ra2port_set_dialog_user(dialog, this);   // DWL_USER, pointer-sized natively
            if (HWND list = GetDlgItem(dialog, 0x525)) Fill_List_0053c040(list);
            unknown_dialog_5ff7c0(dialog);
            ShowWindow(dialog, 0);
        }
        // The disk the row's game needs, as Dialog does before its Load.
        for (int i = 0; i < Files.Count(); ++i) {
            if (_stricmp(Files[i]->Filename, file) == 0) {
                Unknown_0046E4C0(0x3c);
                CD::Set_Required_CD(Files[i]->unknown_1a8 == -1 ? -1 : Campaigns[Files[i]->unknown_1a8]->unknown_98);
                CD().Force_Available(CD::RequiredCD);
                break;
            }
        }
        WWMouse->Hide_Mouse();
        bool loaded = Load(file);
        for (int i = 0; i < Files.Count(); ++i) {
            delete Files[i];
        }
        Files.Clear();
        if (dialog) unknown_dialog_5ff6e0(dialog);
        if (loaded) {
            fprintf(stderr, "resume: loaded %s\n", file);
            return true;
        }
        WWMouse->Show_Mouse();
        fprintf(stderr, "resume: could not load %s\n", file);
    }
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
