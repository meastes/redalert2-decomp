// Behaviour rows (Scout1209 r9; NOT EXACT, native/behaviour_rewrites.json): the LAN lobby's
// session accessors.
#include "shared/session_state.hpp"

// 0x006729F0: the session player's country (+0x124), -2 (random) while +0x128 holds -2
// (inventory/decompiled/006729f0.c; Net_Join_Dialog 0x005BD070 seats the host with it).
int UnknownSessionState::Value_0124_006729f0()
{
    return unknown_128 != -2 ? unknown_124 : -2;
}

// 0x00672A10: the session player's colour (+0x11C), -2 (random) while +0x120 holds -2
// (inventory/decompiled/00672a10.c).
int UnknownSessionState::Value_011c_00672a10()
{
    return unknown_120 != -2 ? unknown_11c : -2;
}

#include "shared/lobby.hpp"
#include "shared/unknown_player_entry.hpp"
#include "shared/unknown_globals.hpp"
#include "shared/owner_talk.hpp"

void __fastcall Get_Join_Responses_005c08a0();
WIN32_IMPORT HWND WINAPI GetNextDlgTabItem(HWND dialog, HWND control, BOOL previous);
namespace ra2port { void *original_data_pointer(unsigned int va); }

// 0x005C8CD0 (accepted as the seed seed_005c8cd0::FUN_005c8cd0, whose int-typed globals
// cannot carry native pointers): the slot index becomes the session's own slot and that slot
// holds the first session player (the host seats itself; Get_Join_Responses seats a joiner).
void __fastcall Seat_First_Player_005c8cd0(int slot)
{
    unknown_lobby_a3fa94 = slot;
    unknown_player_slots_a3fa98[slot] = unknown_players_a3fa7c[0];
}

// 0x005C8CC0 (accepted seed Leaf_5c8cc0::get): a player into a lobby slot.
void __fastcall Set_Player_Slot_005c8cc0(int slot, NodeNameType *player)
{
    unknown_player_slots_a3fa98[slot] = player;
}

// 0x005BBD10 (accepted seed seed_005bbd10::FUN_005bbd10): the lobby message boxes' service
// callback: services the network and reads the join responses; never ends the box.
bool Unknown_005bbd10()
{
    Service_Game_IO_00481fb0();
    Get_Join_Responses_005c08a0();
    return false;
}

// 0x005DEF80 (accepted seed seed_005def80::FUN_005def80): moves the focus to the next (or,
// with flag, the previous) tab stop of the control's dialog; false when there is none.
bool __fastcall Unknown_005def80(int lparam, bool flag)   // long in src/: 32-bit, as the native units read it
{
    HWND control = (HWND)lparam;
    HWND next = GetNextDlgTabItem(GetParent(control), control, flag);
    if (next != control) {
        SetFocus(next);
        return true;
    }
    return false;
}

// 0x005C9320 (accepted seed seed_005c9320::FUN_005c9320): a real player (not empty, not one
// of the seven fixed slot entries).
bool __fastcall Unknown_005c9320(NodeNameType *entry)
{
    return entry && entry != unknown_open_entry_a72d2c && entry != unknown_closed_entry_a72d30
        && entry != unknown_waiting_entry_a72d40 && entry != unknown_player_a72d44
        && entry != unknown_easy_ai_entry_a72d34 && entry != unknown_medium_ai_entry_a72d38
        && entry != unknown_hard_ai_entry_a72d3c;
}

// 0x005C9370 (accepted seed seed_005c9370::FUN_005c9370): a computer player's entry.
bool __fastcall Unknown_005c9370(NodeNameType const *entry)
{
    return entry && (entry == unknown_easy_ai_entry_a72d34 || entry == unknown_medium_ai_entry_a72d38
        || entry == unknown_hard_ai_entry_a72d3c);
}

// 0x005C93A0 (accepted seed seed_005c93a0::FUN_005c93a0): a player, human or computer (not
// empty, open, closed, waiting or the open observer slot).
bool __fastcall Unknown_005c93a0(NodeNameType *entry)
{
    return entry && entry != unknown_open_entry_a72d2c && entry != unknown_closed_entry_a72d30
        && entry != unknown_player_a72d44 && entry != unknown_waiting_entry_a72d40;
}

// 0x007837D3 (the CRT's wcscmp; the seeds 0x005C8660 and 0x005C8680 name it in their own
// namespaces): -1, 0 or 1 by the first differing 16-bit unit.
static int Compare_Wide(unsigned short const *a, unsigned short const *b)
{
    int diff;
    while ((diff = (int)*a - (int)*b) == 0 && *b) {
        ++a;
        ++b;
    }
    return diff < 0 ? -1 : diff > 0 ? 1 : 0;
}
namespace seed_005c8660 { unsigned int FUN_007837d3(unsigned short *a, unsigned short *b) { return (unsigned int)Compare_Wide(a, b); } }
namespace seed_005c8680 { unsigned int FUN_007837d3(unsigned short *a, unsigned short *b) { return (unsigned int)Compare_Wide(a, b); } }

// The name of session player index (native-only helper for the seed 0x005C8680, whose
// int-typed DAT_00a3fa80 cannot carry the native vector pointer; port/fixups.json).
unsigned short *Native_Session_Player_Name(int index)
{
    return (unsigned short *)unknown_players_a3fa7c[index]->Name;
}

// 0x00672F20 (accepted seed Owner_00672f20::FUN_00672f20): removes entry index of the eight
// words at +0x3022, moving the later ones down (Destroy_Connection).
void UnknownSessionState::Unknown_00672F20(int index)
{
    for (int i = index; i < 7; ++i) unknown_3022.unknown[i] = unknown_3022.unknown[i + 1];
}

// 0x0063FB40 (accepted seed Owner_0063fb40::FUN_0063fb40): the name of connection index, null
// past the end.
char *OwnerTalkClass::Unknown_0063FB40(int index)
{
    return index < Connections.Count() ? Connections[index]->Name : 0;
}

// 0x00737B00 (accepted seed seed_00737b00::FUN_00737b00): the builder's name for the CRC dump
// header (the original's string at 0x007D8904, read from game.exe at run time).
char *__fastcall Unknown_00737b00(char *buffer, int size)
{
    (void)size;
    char const *name = (char const *)ra2port::original_data_pointer(0x007D8904u);
    int i = 0;                                  // strncpy(buffer, name, 0x20)
    for (; name && i < 0x20 && name[i]; ++i) buffer[i] = name[i];
    for (; i < 0x20; ++i) buffer[i] = 0;
    return buffer;
}

// 0x00738100 (accepted seed Leaf_738100::get): does nothing.
void Unknown_00738100()
{
}

// 0x005C2940 and 0x0075CA00 (accepted seeds seed_005c2940 / seed_0075ca00, whose int-typed
// handles the native units cannot pass): message 0x4DC to control 0x59F of the active dialog.
WIN32_IMPORT HWND WINAPI GetDlgItem(HWND dialog, int id);
void Unknown_005c2940()
{
    SendMessageA(GetDlgItem(Unknown_00735B10(), 0x59F), 0x4DC, 0, 0);
}
void Unknown_0075ca00()
{
    SendMessageA(GetDlgItem(Unknown_00735B10(), 0x59F), 0x4DC, 0, 0);
}

// 0x0073BDB0 (accepted seed seed_0073bdb0): the online session state 0x00A3D2A4 is 1 or 2.
extern int unknown_a3d2a4;
bool Unknown_0073bdb0()
{
    return unknown_a3d2a4 == 2 || unknown_a3d2a4 == 1;
}
