// restate.cpp: the original translation unit re-filed as one source (Route A): the accepted
// bodies of its canonical compile group in address order, under the unit's own header
// state (its switches and includes in their original order). The module map
// manifests/original_target/modules/restate.json carries the addresses and the rows that
// stay in their own files.


#define GENERIC_LIST_IMPLEMENT_CONSTRUCTOR  // the unit calls GenericList::GenericList 0x00490E80 (shared/ini_class.hpp)
#define INDEX_DECLARE_CONSTRUCTOR          // and IndexClass<int, INISection *>::IndexClass 0x004910C0
#include "shared/cc_file_class.hpp"
#include "shared/ccini_class.hpp"
#include "shared/ms_message_box.hpp"
#include "shared/dialog_create.hpp"
#include "shared/string_table.hpp"
#include "shared/keyboard.hpp"
#include "shared/unknown_globals.hpp"
#include "shared/id_source.hpp"
#include "shared/map_view.hpp"
#include "shared/win32_api.hpp"


// void __fastcall Unknown_0063B090(ScenarioClass *scenario) -- 0x0063B090 (Restate.cpp; TibSun Restate_Mission)
// Restates the mission: shows the scenario's [Briefing] text from MISSION.INI (or Brief:Error)
// in dialog 0x10C and waits until it is closed or the game must quit.

extern char const *const Restate_Ini_Name_007ee578;        // "MISSION.INI"
extern char const *const Restate_Section_Entry_007ee57c;   // "Briefing"
extern wchar_t const *Restate_Text_00ab66e8;               // the text dialog 0x10C shows

int __stdcall Restate_Dialog_Proc_0063b270(HWND window, UINT message, WPARAM wparam, LPARAM lparam);

void __fastcall Unknown_0063B090(ScenarioClass *scenario)
{
    bool old = unknown_flag_a40d04;
    unknown_flag_a40d04 = false;

    CCFileClass file;
    CCINIClass ini;

    file.Close();
    file.Set_Name(Restate_Ini_Name_007ee578);
    if (file.Is_Available()) {
        ini.Load(file, false, false);

        if (ini.Is_Present(scenario->FileName, Restate_Section_Entry_007ee57c)) {
            char buffer[256];
            ini.Get_String(scenario->FileName, Restate_Section_Entry_007ee57c, unknown_string_83c55c, buffer, sizeof(buffer));
            Restate_Text_00ab66e8 = StringTable::Load_String(buffer, 0, "D:\\Ra2\\Restate.cpp", 0x87);
        } else {
            Restate_Text_00ab66e8 = StringTable::Load_String("Brief:Error", 0, "D:\\Ra2\\Restate.cpp", 0x8A);
        }

        HWND dialog = unknown_create_dialog_5ff610(0x10C, Restate_Dialog_Proc_0063b270, 0);
        unknown_flag_a40d44 = true;
        WWKeyboard->Clear();
        if (dialog) {
            int result;
            SetWindowLongA(dialog, 8, (LONG)&result);
            unknown_dialog_5ff7c0(dialog);
            result = 0;
            while (true) {
                if (unknown_6000d0()) {
                    result = 0x423;
                    break;
                }
                if (result != 0) {
                    break;
                }
            }
            unknown_dialog_5ff6e0(dialog);
        }
        WWKeyboard->Clear();
        unknown_flag_a40d44 = unknown_id_source_a3d290->unknown_2306;
        Map.Flag_To_Redraw(2);
    }

    unknown_flag_a40d04 = old;
}


// int __stdcall Restate_Dialog_Proc_0063b270(HWND window, UINT message, WPARAM wparam, LPARAM lparam)
// -- 0x0063B270 (Restate.cpp): the briefing dialog: 0x497 loads the text into control 0x6E7;
// button 0x423 stores its id through the DWL_USER pointer.

extern wchar_t const *Restate_Text_00ab66e8;

int __stdcall Restate_Dialog_Proc_0063b270(HWND window, UINT message, WPARAM wparam, LPARAM lparam)
{
    int handled = unknown_dialog_proc_5ffb00(window, message, wparam, lparam);
    if (handled != 0) {
        return handled;
    }
    switch (message) {
        case 0x497: {
            HWND item = GetDlgItem(window, 0x6E7);
            SendMessageA(item, 0x4B2, 0, (LPARAM)Restate_Text_00ab66e8);
            return 0;
        }
        case 0x111:
            if (GetDlgCtrlID((HWND)lparam) == 0x423 && (unsigned short)(wparam >> 16) == 0) {   // HIWORD(wparam) == BN_CLICKED
                *(int *)GetWindowLongA(window, 8) = 0x423;
            }
            break;
    }
    return 0;
}
