// void __fastcall Unknown_005cbf20(HWND dialog, int id, NodeNameType *entry, int update) (0x005CBF20)
// Puts an entry into the slot of a slot combo box: a computer is refused (and the combo box reset) while an
// observer is seated or the observer mode is on, an observer boots every computer; then the slot's side and
// colour combo boxes and computer settings follow the change, update 1 and 2 send the slot names and 2 also
// the lobby state.
#include "shared/lobby.hpp"
#include "shared/gdlg_supp.hpp"
#include "shared/session_state.hpp"
#include "shared/string_table.hpp"
// NATIVE (Scout1209 r9): the callees under their accepted names and declarations.
void __fastcall unknown_combo_select_by_data_005c9950(HWND dialog, int id, int data);
int Native_Entry_Handle(void const *entry);   // NATIVE: native/link/native_event_data.cpp (32-bit combo data)
void __fastcall Kick_Channel_User_Wide_0075cae0(wchar_t const *name);
void __fastcall Unknown_0073c480();
void __fastcall Update_Channel_Player_Limit_00749f20();

void __fastcall Unknown_005cbf20(HWND dialog, int id, NodeNameType *entry, int update)
{
    int slot = Control_Slot_005c9800(id);
    NodeNameType *previous = unknown_player_slots_a3fa98[slot];
    if (entry != 0 && (entry == unknown_easy_ai_entry_a72d34 || entry == unknown_medium_ai_entry_a72d38
            || entry == unknown_hard_ai_entry_a72d3c) && slot != unknown_lobby_flag_7e6d90) {
        if (unknown_a72c68 != 0 || (unknown_lobby_flag_7e6d90 > 0
                && (unknown_player_slots_a3fa98[unknown_lobby_flag_7e6d90]->Team == -1
                    || unknown_player_slots_a3fa98[unknown_lobby_flag_7e6d90] == unknown_player_a72d44))) {
            Unknown_005C2C80(-1, StringTable::Load_String("TXT_NO_AI_WITH_OBSERVER", 0, "D:\\Ra2\\netshare.cpp", 0x15A2));
            NodeNameType *kept = previous;
            if (previous != 0 && (previous == unknown_easy_ai_entry_a72d34 || previous == unknown_medium_ai_entry_a72d38
                    || previous == unknown_hard_ai_entry_a72d3c)) {
                kept = unknown_closed_entry_a72d30;
            }
            previous = entry;
            unknown_combo_select_by_data_005c9950(dialog, id, Native_Entry_Handle(kept));   // NATIVE: handle
        }
    } else if (entry == unknown_player_a72d44) {
        bool booted = false;
        for (int i = 0; i < 8; i++) {
            NodeNameType *other = unknown_player_slots_a3fa98[i];
            if (i != slot && other != 0 && (other == unknown_easy_ai_entry_a72d34 || other == unknown_medium_ai_entry_a72d38
                    || other == unknown_hard_ai_entry_a72d3c)) {
                int other_id = Slot_Control_Id_005c97a0(i);
                unknown_combo_select_by_data_005c9950(dialog, other_id, Native_Entry_Handle(unknown_closed_entry_a72d30));   // NATIVE: handle
                Unknown_005cbf20(dialog, other_id, unknown_closed_entry_a72d30, 0);
                booted = true;
            }
        }
        if (booted) {
            Unknown_005C2C80(-1, StringTable::Load_String("TXT_OBSERVER_BOOTED_AI", 0, "D:\\Ra2\\netshare.cpp", 0x15BF));
        }
    }
    if (entry == previous) {
        return;
    }
    unknown_player_slots_a3fa98[slot] = entry;
    if (update == 1 || update == 2) {
        if ((entry == 0 || entry == unknown_open_entry_a72d2c || entry == unknown_closed_entry_a72d30
                || entry == unknown_waiting_entry_a72d40 || entry == unknown_player_a72d44
                || entry == unknown_easy_ai_entry_a72d34 || entry == unknown_medium_ai_entry_a72d38
                || entry == unknown_hard_ai_entry_a72d3c)
            && previous != 0 && previous != unknown_open_entry_a72d2c && previous != unknown_closed_entry_a72d30
            && previous != unknown_waiting_entry_a72d40 && previous != unknown_player_a72d44
            && previous != unknown_easy_ai_entry_a72d34 && previous != unknown_medium_ai_entry_a72d38
            && previous != unknown_hard_ai_entry_a72d3c) {
            if (unknown_chat_a72cfc != 0) {
                Kick_Channel_User_Wide_0075cae0(previous->Name);
            } else {
                Reject_Join_005bc3e0(previous);
            }
        }
        if (unknown_chat_a72cfc != 0) {
            Unknown_0073c480();
        } else {
            Send_Slot_Names_005bbd20();
        }
    }
    bool computer = entry != 0 && (entry == unknown_easy_ai_entry_a72d34 || entry == unknown_medium_ai_entry_a72d38
        || entry == unknown_hard_ai_entry_a72d3c);
    bool was_computer = previous != 0 && (previous == unknown_easy_ai_entry_a72d34
        || previous == unknown_medium_ai_entry_a72d38 || previous == unknown_hard_ai_entry_a72d3c);
    bool observer = entry == unknown_player_a72d44 || (entry != 0 && entry != unknown_open_entry_a72d2c
        && entry != unknown_closed_entry_a72d30 && entry != unknown_waiting_entry_a72d40
        && entry != unknown_easy_ai_entry_a72d34 && entry != unknown_medium_ai_entry_a72d38
        && entry != unknown_hard_ai_entry_a72d3c && entry->Team == -1);
    bool was_observer = previous == unknown_player_a72d44 || (previous != 0 && previous != unknown_open_entry_a72d2c
        && previous != unknown_closed_entry_a72d30 && previous != unknown_waiting_entry_a72d40
        && previous != unknown_easy_ai_entry_a72d34 && previous != unknown_medium_ai_entry_a72d38
        && previous != unknown_hard_ai_entry_a72d3c && previous->Team == -1);
    if (observer) {
        Fill_Observer_Combo_4d1640(dialog, Player_Side_Combo_ID_4d1350(slot));
        SendMessageA(GetDlgItem(dialog, Slot_Control_Id_005c97a0(slot)), 0x4F1, 0, 1);
        int color = Player_Color_Combo_ID_4d1cb0(slot);
        Fill_Random_Color_Combo_4d2250(dialog, color);
        Select_Color_Combo_4d2480(dialog, color, 8);
        Unknown_005c9b90(dialog, slot, false);
        unknown_lobby_a3d2dc[slot] = -1;
        unknown_lobby_a3d2fc[slot] = -1;
        unknown_lobby_a3d31c[slot] = -1;
    } else if (was_observer) {
        Fill_Side_Combo_4d1560(dialog, Player_Side_Combo_ID_4d1350(slot));
        SendMessageA(GetDlgItem(dialog, Slot_Control_Id_005c97a0(slot)), 0x4F1, 0, 0);
        int color = Player_Color_Combo_ID_4d1cb0(slot);
        Fill_Random_Color_Combo_4d2250(dialog, color);
        Select_Color_Combo_4d2480(dialog, color, -2);
        Unknown_005c9b90(dialog, slot, false);
        unknown_lobby_a3d2dc[slot] = -1;
        unknown_lobby_a3d2fc[slot] = -1;
        unknown_lobby_a3d31c[slot] = -1;
    }
    if (computer) {
        Unknown_005c9b90(dialog, slot, true);
        int difficulty = -1;
        if (entry == unknown_easy_ai_entry_a72d34) {
            difficulty = 2;
        } else if (entry == unknown_medium_ai_entry_a72d38) {
            difficulty = 1;
        } else if (entry == unknown_hard_ai_entry_a72d3c) {
            difficulty = 0;
        }
        unknown_lobby_a3d2dc[slot] = difficulty;
        unknown_lobby_a3d2fc[slot] = -2;
        unknown_lobby_a3d31c[slot] = -2;
    } else if (was_computer) {   // NATIVE (Scout1274): else-if as in the original (0x005CC608 jmp 0x005CC632); computer to computer keeps the new difficulty
        Unknown_005c9b90(dialog, slot, false);
        unknown_lobby_a3d2dc[slot] = -1;
        unknown_lobby_a3d2fc[slot] = -1;
        unknown_lobby_a3d31c[slot] = -1;
    }
    if (unknown_chat_a72cfc != 0) {
        Update_Channel_Player_Limit_00749f20();
    }
    bool computers_changed = computer || was_computer;
    bool observers_changed = observer || was_observer;
    if (computers_changed || observers_changed) {
        int count = 0;
        for (NodeNameType **pointer = unknown_player_slots_a3fa98; pointer < &unknown_player_slots_a3fa98[8]; pointer++) {
            NodeNameType *other = *pointer;
            if (other != 0 && (other == unknown_easy_ai_entry_a72d34 || other == unknown_medium_ai_entry_a72d38
                    || other == unknown_hard_ai_entry_a72d3c)) {
                count++;
            }
        }
        unknown_session_a3d298.AICount = count;
        if (update == 2) {
            if (unknown_chat_a72cfc != 0) {
                unknown_call_73fee0_default();
            } else {
                Unknown_005bbdc0();
            }
            Unknown_005c3810(true, false);
        }
    }
}
