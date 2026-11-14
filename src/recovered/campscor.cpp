// campscor.cpp: the original translation unit re-filed as one source (Route A): the accepted
// bodies of its canonical compile group in address order, under the unit's own header
// state (its switches and includes in their original order). The module map
// manifests/original_target/modules/campscor.json carries the addresses and the rows that
// stay in their own files.


#define VECTOR_IMPLEMENT_DYNAMIC_DELETE_OBJECT
#define VECTOR_IMPLEMENT_DYNAMIC_DELETE_INDEX
#include "shared/unknown_geometry.hpp"
#include "shared/unknown_static_rect.hpp"
#define VECTOR_IMPLEMENT_VECTOR_ACCESSORS
#define VECTOR_IMPLEMENT_DYNAMIC_ACCESSORS
#define VECTOR_IMPLEMENT_VECTOR_DESTRUCTOR
#include "shared/campaign_score.hpp"
#include "shared/string_table.hpp"
#include "shared/win32_api.hpp"
#define SHAPE_FILE_EMPTY_RECT unknown_static_rect
#include "shared/house.hpp"
#include "shared/debug_printf.hpp"
#include "shared/session_state.hpp"
#include "shared/ms_message_box.hpp"
#include "shared/queue.hpp"
#include "shared/color_scheme.hpp"
#include "shared/ipx_manager.hpp"
#include "shared/lobby.hpp"
#include "shared/unknown_globals.hpp"
#include "shared/keyboard.hpp"
#include "shared/id_source.hpp"
#include "shared/mplayer_score_type.hpp"


// Header-static dynamic initializers of the unit at 0x00465A00:  the
// shared/unknown_geometry.hpp constants and unknown_static_rect.hpp
// (0x00465A00-0x00465BE6); the other
// drafts of this plan are emitted by these includes.
// The unit expands DynamicVectorClass::Delete inline (Presentation 0x00466A50).


// Dynamic initializer (0x00465A30, _$E) of b from the header(s) included
// by HeaderStatics_diagonal_init_00465a00.cpp; no source of its own.


// Dynamic initializer (0x00465A50, _$E) of c from the header(s) included
// by HeaderStatics_diagonal_init_00465a00.cpp; no source of its own.


// Dynamic initializer (0x00465A70, _$E) of angle45 from the header(s) included
// by HeaderStatics_diagonal_init_00465a00.cpp; no source of its own.


// Dynamic initializer (0x00465A90, _$E) of angle60 from the header(s) included
// by HeaderStatics_diagonal_init_00465a00.cpp; no source of its own.


// Dynamic initializer (0x00465AB0, _$E) of angle90 from the header(s) included
// by HeaderStatics_diagonal_init_00465a00.cpp; no source of its own.


// Dynamic initializer (0x00465AD0, _$E) of g from the header(s) included
// by HeaderStatics_diagonal_init_00465a00.cpp; no source of its own.


// Dynamic initializer (0x00465B10, _$E) of h from the header(s) included
// by HeaderStatics_diagonal_init_00465a00.cpp; no source of its own.


// Dynamic initializer (0x00465B40, _$E) of i from the header(s) included
// by HeaderStatics_diagonal_init_00465a00.cpp; no source of its own.


// Dynamic initializer (0x00465B70, _$E) of j from the header(s) included
// by HeaderStatics_diagonal_init_00465a00.cpp; no source of its own.


// Dynamic initializer (0x00465BA0, _$E) of k from the header(s) included
// by HeaderStatics_diagonal_init_00465a00.cpp; no source of its own.


// Dynamic initializer (0x00465BB0, _$E) of l from the header(s) included
// by HeaderStatics_diagonal_init_00465a00.cpp; no source of its own.


// Dynamic initializer (0x00465BD0, _$E) of static_rect from the header(s) included
// by HeaderStatics_diagonal_init_00465a00.cpp; no source of its own.


// CampaignScoreClass::CampaignScoreClass() -- 0x00465BF0
// Clears the score screen's field and the shown map's time and difficulty (-1: unknown).
// The unit's first include of shared/vector_class.hpp: the inline accessors and destructor
// the later functions use are enabled here.

CampaignScoreClass::CampaignScoreClass()
{
    unknown_04 = 0;
    unknown_score_time_850508 = 0;
    unknown_score_difficulty_7d1480 = -1;
}


// CampaignScoreClass::~CampaignScoreClass() -- 0x00465C10

CampaignScoreClass::~CampaignScoreClass()
{
}


// void __fastcall unknown_format_time(wchar_t *buffer, int time) -- 0x00465C20
// Formats a game time in seconds with TXT_TIME_FORMAT_HOURS as hours, minutes and seconds;
// times of 100 hours or more show as 99:59:59. Does nothing without a buffer.

void __fastcall unknown_format_time_465c20(wchar_t *buffer, int time)
{
    if (buffer) {
        if (time / 3600 > 99) {
            time = 359999;
        }
        int hours = time / 3600;
        time -= hours * 3600;
        int minutes = time / 60;
        time -= minutes * 60;
        swprintf(buffer, StringTable::Load_String("TXT_TIME_FORMAT_HOURS", 0, "D:\\Ra2\\CampScor.cpp", 102), hours, minutes, time);
    }
}


// void __fastcall unknown_set_dialog_item_number(HWND dialog, int id, int value) -- 0x00465CC0
// Purpose: formats a number as wide text (L"%d") and sends it to a dialog
// control with the custom message 0x4B2 (WM_USER + 0xB2). Referenced from
// the campaign score area; bound by linear sweep to the final ret.

WIN32_IMPORT HWND __stdcall GetDlgItem(HWND dialog, int id);
WIN32_IMPORT long __stdcall SendMessageA(HWND window, unsigned int message, unsigned int wparam, long lparam);

void __fastcall unknown_set_dialog_item_number_00465cc0(HWND dialog, int id, int value)
{
    wchar_t text[32];
    swprintf(text, L"%d", value);
    HWND item = GetDlgItem(dialog, id);
    SendMessageA(item, 0x4b2, 0, (LPARAM)text);
}


// void CampaignScoreClass::unknown_465d10() -- 0x00465D10
// Fills the session's score records from every house that is not multiplayer-passive: name,
// colour scheme, whether it won (and the last winner's record), the kills of each house's
// units and buildings, the totals lost and built, and the score; prints each record to the
// debug output. The records belong to the session object (the scheduling of the record and
// winner stores needs one object).
// The unit's shape helpers fall back to unknown_static_rect (no empty rect of their own).

extern "C" wchar_t *__cdecl wcscpy(wchar_t *destination, wchar_t const *source);

void CampaignScoreClass::unknown_465d10()
{
    unknown_session_a3d298.ScoreCount = 0;
    int count = Houses.Count();
    for (int i = 0; i < count; i++) {
        HouseClass *house = Houses[i];
        if (house != 0 && house->Type->MultiplayPassive != true) {
            int index = unknown_session_a3d298.ScoreCount++;
            unknown_session_a3d298.Scores[index].Winner = 0;
            wcscpy(unknown_session_a3d298.Scores[index].Name, house->UIName);
            unknown_session_a3d298.Scores[index].Lost = 0;
            unknown_session_a3d298.Scores[index].Kills = 0;
            unknown_session_a3d298.Scores[index].Built = 0;
            unknown_session_a3d298.Scores[index].Score = 0;
            unknown_session_a3d298.Scores[index].Scheme = house->ColorSchemeIndex;
            if (!house->unknown_13d) {
                unknown_session_a3d298.Scores[index].Winner++;
                unknown_session_a3d298.ScoreWinner = index;
            }
            int kills = 0;
            int j;
            for (j = 0; j < 20; j++) {
                kills += house->unknown_5310[j];
            }
            for (j = 0; j < count; j++) {
                kills += house->unknown_5364[j];
            }
            unknown_session_a3d298.Scores[index].Kills = kills;
            unknown_session_a3d298.Scores[index].Lost = house->unknown_5360 + house->unknown_53b4;
            int built = house->unknown_5460.Sum();
            built += house->unknown_5470.Sum();
            built += house->unknown_5480.Sum();
            built += house->unknown_5490.Sum();
            unknown_session_a3d298.Scores[index].Built = house->unknown_5360 + house->unknown_53b4 + built;
            if (house->unknown_5418 > 0) {
                unknown_session_a3d298.Scores[index].Score += house->unknown_5418;
            }
            Debug_Printf("%s: %s\n Scheme: %d\n Lost = %d\n Kills = %d\n Built = %d\n Score = %d\n",
                unknown_session_a3d298.Scores[index].Name,
                unknown_session_a3d298.Scores[index].Winner > 0 ? "Winner" : "Loser",
                unknown_session_a3d298.Scores[index].Scheme, unknown_session_a3d298.Scores[index].Lost,
                unknown_session_a3d298.Scores[index].Kills, unknown_session_a3d298.Scores[index].Built,
                unknown_session_a3d298.Scores[index].Score);
        }
    }
}


// void unknown_sort_session_scores() -- 0x00465EE0
// Points the score list at the session's records and sorts them best first.

extern "C" void __cdecl qsort(void *base, unsigned int count, unsigned int width, int (__cdecl *compare)(void const *, void const *));
int __cdecl unknown_compare_descending_60_00465f20(void const *left, void const *right);

void unknown_sort_session_scores_465ee0()
{
    int count = unknown_session_a3d298.ScoreCount;
    for (int i = 0; i < count; i++) {
        unknown_score_list_850448[i] = &unknown_session_a3d298.Scores[i];
    }
    if (count > 1) {
        qsort(unknown_score_list_850448, count, sizeof(MPlayerScoreType *), unknown_compare_descending_60_00465f20);
    }
}


// int __cdecl unknown_compare_descending_60(void const *, void const *) -- 0x00465F20
// Purpose: qsort comparator over an array of object pointers, ordering by the
// int at +0x60 from largest to smallest (0 when equal). Referenced from 4
// data sites (qsort calls). The pointee type is not yet identified.
struct UnknownSortItem_00465f20 {
    char unknown_00[0x60];
    int unknown_60;
};

int __cdecl unknown_compare_descending_60_00465f20(void const *left, void const *right)
{
    UnknownSortItem_00465f20 const *a = *(UnknownSortItem_00465f20 const * const *)left;
    UnknownSortItem_00465f20 const *b = *(UnknownSortItem_00465f20 const * const *)right;
    if (a->unknown_60 == b->unknown_60) {
        return 0;
    }
    return a->unknown_60 > b->unknown_60 ? -1 : 1;
}


// void __fastcall unknown_reset_ready(int count) -- 0x00465F50
// Marks all eight player rows ready, then the first count rows not ready.

void __fastcall unknown_reset_ready_465f50(int count)
{
    memset(unknown_score_ready_8504c8, true, sizeof(unknown_score_ready_8504c8));
    for (int i = 0; i < count; i++) {
        unknown_score_ready_8504c8[i] = false;
    }
}


// bool unknown_all_ready() -- 0x00465F80 (29 bytes; the inventory row 0x00465F96 is a false entry
// from a number table at 0x008217B0)
// Whether every player row is ready.

bool unknown_all_ready_465f80()
{
    int i;
    for (i = 0; i < 8; i++) {
        if (!unknown_score_ready_8504c8[i]) {
            break;
        }
    }
    return i == 8;
}


// void __fastcall unknown_set_ready(int index, bool ready) -- 0x00465FA0 (7 bytes)

void __fastcall unknown_set_ready_465fa0(int index, bool ready)
{
    unknown_score_ready_8504c8[index] = ready;
}


// bool __fastcall unknown_is_ready(int index) -- 0x00465FB0

bool __fastcall unknown_is_ready_465fb0(int index)
{
    return unknown_score_ready_8504c8[index];
}


// int __stdcall unknown_score_dialog_proc(HWND window, UINT message, WPARAM wparam, LPARAM lparam) -- 0x00465FC0
// (2,656 bytes; the inventory splits it at false entries 0x0046622A, 0x00466754 and 0x004669E8 from the number
// table at 0x008217B0)
// The cooperative campaign score screen: after the common dialog handler, WM_INITDIALOG fills the title,
// difficulty, campaign name, time and one row per score (name, kills, lost, built, score in the player's
// colour; computers start ready; a player no longer connected shows TXT_ABORTED and disables continue);
// the continue button 0x6D1 toggles ready (TXT_READY / TXT_WAITING to the other players, commands 0x23 / 0x25),
// the cancel button 0x5C0 ends with 2 (command 0x24), and 0x4ED hides the dialog.

extern "C" int __cdecl wcscmp(wchar_t const *left, wchar_t const *right);

// Sends a score-screen message (command) to every other player.
static inline void Send_Score_Packet(int command)
{
    UnknownScorePacket packet;
    memset(&packet, 0, sizeof(packet));
    packet.Command = command;
    strcpy(packet.Name, Wide_To_Narrow_006fa120(unknown_players_a3fa7c[0]->Name));
    for (int i = 1; i < unknown_players_a3fa7c.Count(); i++) {
        unknown_ipx_manager_a40978.unknown_5259a0(&packet, 0x2F, 1, unknown_players_a3fa7c[i]->Name);
        unknown_ipx_manager_a40978.Service();
    }
}

int __stdcall unknown_score_dialog_proc_465fc0(HWND window, UINT message, WPARAM wparam, LPARAM lparam)
{
    int result = unknown_dialog_proc_5ffb00(window, message, wparam, lparam);
    if (result != 0) {
        return result;
    }
    int *done = (int *)GetWindowLongA(window, 8);
    switch (message) {
    case 0x110: {
        wchar_t text[32];
        HWND item = GetDlgItem(window, 0x71B);
        SendMessageA(item, 0x4B2, 0, (LPARAM)unknown_text_a3d342);
        swprintf(text, L"%s: ", StringTable::Load_String("GUI:Difficulty", 0, "D:\\Ra2\\CampScor.cpp", 285));
        if (unknown_score_difficulty_7d1480 == 0) {
            swprintf(text, StringTable::Load_String("GUI:DifficultyString", 0, "D:\\Ra2\\CampScor.cpp", 287),
                StringTable::Load_String("TXT_DIFFICULTY_EASY", 0, "D:\\Ra2\\CampScor.cpp", 287));
        } else if (unknown_score_difficulty_7d1480 == 1) {
            swprintf(text, StringTable::Load_String("GUI:DifficultyString", 0, "D:\\Ra2\\CampScor.cpp", 289),
                StringTable::Load_String("TXT_DIFFICULTY_MEDIUM", 0, "D:\\Ra2\\CampScor.cpp", 289));
        } else if (unknown_score_difficulty_7d1480 == 2) {
            swprintf(text, StringTable::Load_String("GUI:DifficultyString", 0, "D:\\Ra2\\CampScor.cpp", 291),
                StringTable::Load_String("TXT_DIFFICULTY_BRUTAL", 0, "D:\\Ra2\\CampScor.cpp", 291));
        }
        item = GetDlgItem(window, 0x732);
        SendMessageA(item, 0x4B2, 0, (LPARAM)text);
        item = GetDlgItem(window, 0x733);
        SendMessageA(item, 0x4B2, 0, (LPARAM)unknown_score_name_850514);
        unknown_format_time_465c20(text, unknown_score_time_850508 / 60);
        item = GetDlgItem(window, 0x3EA);
        SendMessageA(item, 0x4B2, 0, (LPARAM)text);
        memset(unknown_score_ready_8504c8, true, sizeof(unknown_score_ready_8504c8));
        for (int i = 0; i < unknown_session_a3d298.ScoreCount; i++) {
            if (wcscmp(unknown_score_list_850448[i]->Name, StringTable::Load_String("TXT_COMPUTER", 0, "D:\\Ra2\\CampScor.cpp", 307)) != 0) {
                unknown_score_ready_8504c8[i] = false;
            }
            ColorScheme *scheme = ColorSchemes[unknown_score_list_850448[i]->Scheme];
            RGBClass color = scheme->BaseColor.To_RGB();
            unsigned long rgb = color.Red | (color.Green << 8) | (color.Blue << 16);
            int id = unknown_score_controls_7d1484[i][0];
            wchar_t const *name = unknown_score_list_850448[i]->Name;
            item = GetDlgItem(window, id);
            SendMessageA(item, 0x4B2, 0, (LPARAM)name);
            item = GetDlgItem(window, id);
            SendMessageA(item, 0x498, 0, rgb);
            id = unknown_score_controls_7d1484[i][1];
            unknown_set_dialog_item_number_00465cc0(window, id, unknown_score_list_850448[i]->Kills);
            item = GetDlgItem(window, id);
            SendMessageA(item, 0x498, 0, rgb);
            id = unknown_score_controls_7d1484[i][2];
            unknown_set_dialog_item_number_00465cc0(window, id, unknown_score_list_850448[i]->Lost);
            item = GetDlgItem(window, id);
            SendMessageA(item, 0x498, 0, rgb);
            id = unknown_score_controls_7d1484[i][3];
            unknown_set_dialog_item_number_00465cc0(window, id, unknown_score_list_850448[i]->Built);
            item = GetDlgItem(window, id);
            SendMessageA(item, 0x498, 0, rgb);
            id = unknown_score_controls_7d1484[i][4];
            unknown_set_dialog_item_number_00465cc0(window, id, unknown_score_list_850448[i]->Score);
            item = GetDlgItem(window, id);
            SendMessageA(item, 0x498, 0, rgb);
            int count = unknown_players_a3fa7c.Count();
            bool found = false;
            for (int j = 0; j < count; j++) {
                if (wcscmp(unknown_players_a3fa7c[j]->Name, unknown_score_list_850448[i]->Name) == 0) {
                    found = true;
                    break;
                }
            }
            id = unknown_score_controls_7d1484[i][5];
            if (found) {
                wchar_t const *status = StringTable::Load_String("TXT_WAITING", 0, "D:\\Ra2\\CampScor.cpp", 367);
                item = GetDlgItem(window, id);
                SendMessageA(item, 0x4B2, 0, (LPARAM)status);
            } else if (!unknown_score_ready_8504c8[i]) {
                wchar_t const *status = StringTable::Load_String("TXT_ABORTED", 0, "D:\\Ra2\\CampScor.cpp", 370);
                item = GetDlgItem(window, id);
                SendMessageA(item, 0x4B2, 0, (LPARAM)status);
                EnableWindow(GetDlgItem(window, 0x6D1), FALSE);
            }
            item = GetDlgItem(window, id);
            SendMessageA(item, 0x498, 0, rgb);
        }
        unknown_score_flag_850510 = false;
        return 0;
    }
    case 0x0F:
        Unknown_005FEE70(window);
        unknown_dialog_5e5f20(window);
        return 0;
    case 0x111:
        switch (wparam & 0xFFFF) {
        case 0x6D1:
            if (!unknown_score_flag_850510) {
                if (unknown_game_mode_a3d298 == 3 || unknown_game_mode_a3d298 == 4) {
                    Send_Score_Packet(0x23);
                }
                for (int i = 0; i < unknown_session_a3d298.ScoreCount; i++) {
                    int id = unknown_score_controls_7d1484[i][5];
                    if (wcscmp(unknown_players_a3fa7c[0]->Name, unknown_score_list_850448[i]->Name) == 0) {
                        wchar_t const *status = StringTable::Load_String("TXT_READY", 0, "D:\\Ra2\\CampScor.cpp", 415);
                        HWND item = GetDlgItem(window, id);
                        SendMessageA(item, 0x4B2, 0, (LPARAM)status);
                        break;
                    }
                }
                wchar_t const *label = StringTable::Load_String("GUI:Cancel", 0, "D:\\Ra2\\CampScor.cpp", 419);
                HWND item = GetDlgItem(window, 0x6D1);
                SendMessageA(item, 0x4B2, 0, (LPARAM)label);
                EnableWindow(GetDlgItem(window, 0x5C0), FALSE);
                *done = 1;
                unknown_score_flag_850510 = true;
                InvalidateRect(window, 0, FALSE);
                return 0;
            } else {
                if (unknown_game_mode_a3d298 == 3 || unknown_game_mode_a3d298 == 4) {
                    Send_Score_Packet(0x25);
                }
                for (int i = 0; i < unknown_session_a3d298.ScoreCount; i++) {
                    int id = unknown_score_controls_7d1484[i][5];
                    if (wcscmp(unknown_players_a3fa7c[0]->Name, unknown_score_list_850448[i]->Name) == 0) {
                        wchar_t const *status = StringTable::Load_String("TXT_WAITING", 0, "D:\\Ra2\\CampScor.cpp", 434);
                        HWND item = GetDlgItem(window, id);
                        SendMessageA(item, 0x4B2, 0, (LPARAM)status);
                        break;
                    }
                }
                wchar_t const *label = StringTable::Load_String("GUI:Continue", 0, "D:\\Ra2\\CampScor.cpp", 439);
                HWND item = GetDlgItem(window, 0x6D1);
                SendMessageA(item, 0x4B2, 0, (LPARAM)label);
                EnableWindow(GetDlgItem(window, 0x5C0), TRUE);
                *done = 1;
                unknown_score_flag_850510 = false;
                InvalidateRect(window, 0, FALSE);
                return 0;
            }
        case 0x5C0:
            *done = 2;
            unknown_score_850504 = false;
            if (unknown_game_mode_a3d298 == 3 || unknown_game_mode_a3d298 == 4) {
                Send_Score_Packet(0x24);
            }
            return 1;
        }
        break;
    case 0x4ED:
        unknown_5159b0(0, true);
        unknown_dialog_5ecf40(window, 1);
        ShowWindow(window, 0);
        break;
    }
    return 0;
}


// bool __cdecl unknown_score_service_466a20() (0x00466A20)
// The score dialogs' idle service: the game mode's network service (modem 0x005BBD10,
// internet 0x0073E100), otherwise the plain game I/O service; false then.

bool Unknown_005bbd10();                                // 0x005BBD10 (shared/lobby.hpp)
bool Unknown_0073e100();                                // 0x0073E100 (shared/lobby.hpp)

bool unknown_score_service_466a20()
{
    if (unknown_game_mode_a3d298 == 3) {
        return Unknown_005bbd10();
    }
    if (unknown_game_mode_a3d298 == 4) {
        return Unknown_0073e100();
    }
    Service_Game_IO_00481fb0();
    return false;
}


// void CampaignScoreClass::Presentation(DynamicVectorClass<MPlayerScoreType *> scores, int time,
//     bool more, int difficulty, wchar_t const *name) -- 0x00466A50
// The DynamicVectorClass Delete members this function expands inline are enabled by the unit's first
// include (header statics 0x00465A00).

extern "C" wchar_t *__cdecl wcscpy(wchar_t *destination, wchar_t const *source);
extern "C" int __cdecl wcscmp(wchar_t const *left, wchar_t const *right);

// Tells the other players to continue (command 0x26).
static inline void Send_Continue()
{
    UnknownScorePacket packet;
    memset(&packet, 0, sizeof(packet));
    unknown_score_850504 = true;
    packet.Command = 0x26;
    strcpy(packet.Name, Wide_To_Narrow_006fa120(unknown_players_a3fa7c[0]->Name));
    for (int i = 1; i < unknown_players_a3fa7c.Count(); i++) {
        unknown_ipx_manager_a40978.unknown_5259a0(&packet, 0x2F, 1, unknown_players_a3fa7c[i]->Name);
        unknown_ipx_manager_a40978.Service();
    }
}

extern int unknown_score_count_85050c;
extern int unknown_score_controls_7d1498[8][6];
class IPXAddressClass;
extern UnknownScorePacket unknown_global_packet_a3f640;
extern int unknown_global_length_a3f808;
extern IPXAddressClass unknown_global_address_a3f80c;
extern unsigned short unknown_global_product_a3f818;
bool __fastcall unknown_5c05f0(UnknownScorePacket *packet, IPXAddressClass *address);

// Every GetDlgItem call goes through this code-free wrapper: it puts the GetDlgItem import in edi and
// EnableWindow in esi in the 0x24 branch (the Get_Item rule, VC6_MATCHING_NOTES).
static inline HWND Get_Item(HWND window, int id)
{
    return GetDlgItem(window, id);
}

void CampaignScoreClass::Presentation(DynamicVectorClass<MPlayerScoreType *> scores, int time, bool more,
    int difficulty, wchar_t const *name)
{
    unknown_score_count_85050c = scores.Count();
    unknown_score_time_850508 = time;
    unknown_score_difficulty_7d1480 = difficulty;
    wcscpy(unknown_score_name_850514, name);
    unknown_reset_ready_465f50(8);
    int i;
    for (i = 0; i < unknown_score_count_85050c; i++) {
        unknown_score_list_850448[i] = scores[i];
    }
    unknown_465d10();
    unknown_sort_session_scores_465ee0();
    Unknown_Load_Art_Once_006F2710(unknown_id_source_a3d290->unknown_2239 == false);
    unknown_5159b0(0, true);
    if (unknown_game_mode_a3d298 == 4) {
        unknown_ipx_manager_a40978.unknown_5277f0();
    }
    HWND window = unknown_create_dialog_5ff610(0xBC6, unknown_score_dialog_proc_465fc0, 0);
    int result = 0;
    if (window) {
        SetWindowLongA(window, 8, (LONG)&result);
        unknown_dialog_5ff7c0(window);
        while (result == 0 || result == 1) {
            Unknown_005B5EE0();
            if (result == 1) {
                for (i = 0; i < unknown_session_a3d298.ScoreCount; i++) {
                    if (wcscmp(unknown_players_a3fa7c[0]->Name, unknown_score_list_850448[i]->Name) == 0) {
                        break;
                    }
                }
                if (unknown_score_flag_850510) {
                    unknown_score_ready_8504c8[i] = true;
                } else {
                    unknown_score_ready_8504c8[i] = false;
                }
                if (unknown_all_ready_465f80() && more) {
                    unknown_score_850504 = true;
                    Send_Score_Packet(0x26);
                    break;
                }
                if (!more) {
                    unknown_score_850504 = false;
                    break;
                }
                result = 0;
            }
            unknown_ipx_manager_a40978.Service();
            if (!unknown_ipx_manager_a40978.Get_Global_Message(&unknown_global_packet_a3f640, &unknown_global_length_a3f808,
                    &unknown_global_address_a3f80c, &unknown_global_product_a3f818)
                || unknown_global_product_a3f818 != 0xAA03
                || unknown_5c05f0(&unknown_global_packet_a3f640, &unknown_global_address_a3f80c)) {
                continue;
            }
            if (unknown_global_packet_a3f640.Command == 0x23) {
                for (i = 0; i < unknown_session_a3d298.ScoreCount; i++) {
                    if (strcmp(unknown_global_packet_a3f640.Name, Wide_To_Narrow_006fa120(unknown_score_list_850448[i]->Name)) == 0) {
                        int id = unknown_score_controls_7d1498[i][0];
                        wchar_t const *text = StringTable::Load_String("TXT_READY", 0, "D:\\Ra2\\CampScor.cpp", 582);
                        HWND item = Get_Item(window, id);
                        SendMessageA(item, 0x4B2, 0, (LPARAM)text);
                        unknown_score_ready_8504c8[i] = true;
                        break;
                    }
                }
                if (unknown_all_ready_465f80() && more) {
                    unknown_score_850504 = true;
                    Send_Score_Packet(0x26);
                    break;
                }
            } else if (unknown_global_packet_a3f640.Command == 0x24) {
                for (i = 0; i < unknown_session_a3d298.ScoreCount; i++) {
                    if (strcmp(unknown_global_packet_a3f640.Name, Wide_To_Narrow_006fa120(unknown_score_list_850448[i]->Name)) == 0) {
                        int id = unknown_score_controls_7d1498[i][0];
                        wchar_t const *text = StringTable::Load_String("TXT_ABORTED", 0, "D:\\Ra2\\CampScor.cpp", 602);
                        HWND item = Get_Item(window, id);
                        SendMessageA(item, 0x4B2, 0, (LPARAM)text);
                        unknown_score_ready_8504c8[i] = false;
                        break;
                    }
                }
                for (i = 0; i < unknown_players_a3fa7c.Count(); i++) {
                    if (strcmp(unknown_global_packet_a3f640.Name, Wide_To_Narrow_006fa120(unknown_players_a3fa7c[i]->Name)) == 0) {
                        delete unknown_players_a3fa7c[i];
                        unknown_players_a3fa7c.Delete(unknown_players_a3fa7c[i]);
                        break;
                    }
                }
                EnableWindow(Get_Item(window, 0x6D1), FALSE);
                EnableWindow(Get_Item(window, 0x5C0), TRUE);
            } else if (unknown_global_packet_a3f640.Command == 0x25) {
                for (i = 0; i < unknown_session_a3d298.ScoreCount; i++) {
                    if (strcmp(unknown_global_packet_a3f640.Name, Wide_To_Narrow_006fa120(unknown_score_list_850448[i]->Name)) == 0) {
                        int id = unknown_score_controls_7d1498[i][0];
                        wchar_t const *text = StringTable::Load_String("TXT_WAITING", 0, "D:\\Ra2\\CampScor.cpp", 625);
                        HWND item = Get_Item(window, id);
                        SendMessageA(item, 0x4B2, 0, (LPARAM)text);
                        unknown_score_ready_8504c8[i] = false;
                        break;
                    }
                }
            } else if (unknown_global_packet_a3f640.Command == 0x26) {
                unknown_score_850504 = true;
                break;
            } else if (unknown_all_ready_465f80() && more) {
                unknown_score_850504 = true;
                    Send_Score_Packet(0x26);
                break;
            }
        }
        unknown_5159b0(0, true);
        unknown_dialog_5ff6e0(window);
        unknown_5159b0(0, true);
    }
    if (unknown_game_mode_a3d298 == 4) {
        unknown_ipx_manager_a40978.unknown_527800();
    }
    Unknown_Free_Art_Flagged_006F2750();
}


// CampaignEndScoreClass::CampaignEndScoreClass() -- 0x00467200
// Clears the end screen's difficulty (-1: unknown), score count and time.

CampaignEndScoreClass::CampaignEndScoreClass()
{
    unknown_end_score_difficulty_8504d0 = -1;
    unknown_end_score_count_8504e8 = 0;
    unknown_end_score_time_85049c = 0;
}


// CampaignEndScoreClass::~CampaignEndScoreClass() -- 0x00467230

CampaignEndScoreClass::~CampaignEndScoreClass()
{
}


// void __fastcall unknown_set_dialog_item_number(HWND dialog, int id, int value) -- 0x00467240
// The campaign end screen's copy of 0x00465CC0: sends value as decimal text (L"%d") to the
// dialog control id with the custom message 0x4B2.

void __fastcall unknown_set_dialog_item_number_467240(HWND dialog, int id, int value)
{
    wchar_t text[32];
    swprintf(text, L"%d", value);
    HWND item = GetDlgItem(dialog, id);
    SendMessageA(item, 0x4b2, 0, (LPARAM)text);
}


// void __fastcall unknown_format_time(wchar_t *buffer, int time) -- 0x00467290
// The campaign end screen's copy of unknown_format_time_465c20 (its own source line).

void __fastcall unknown_format_time_467290(wchar_t *buffer, int time)
{
    if (buffer) {
        if (time / 3600 > 99) {
            time = 359999;
        }
        int hours = time / 3600;
        time -= hours * 3600;
        int minutes = time / 60;
        time -= minutes * 60;
        swprintf(buffer, StringTable::Load_String("TXT_TIME_FORMAT_HOURS", 0, "D:\\Ra2\\CampScor.cpp", 700), hours, minutes, time);
    }
}


// int __stdcall unknown_end_score_dialog_proc(HWND window, UINT message, WPARAM wparam, LPARAM lparam) -- 0x00467330
// Dialog procedure of the campaign end score screen: fills the campaign, difficulty, time and per-player
// score rows on WM_INITDIALOG; button 0x6D1 sets the caller's done flag. Every case breaks to the one
// final return (separate returns sink the ebp save into WM_INITDIALOG).

int __stdcall unknown_end_score_dialog_proc_467330(HWND window, UINT message, WPARAM wparam, LPARAM lparam)
{
    int result = unknown_dialog_proc_5ffb00(window, message, wparam, lparam);
    if (result != 0) {
        return result;
    }
    int *done = (int *)GetWindowLongA(window, 8);
    switch (message) {
    case 0x0F:
        Unknown_005FEE70(window);
        unknown_dialog_5e5f20(window);
        break;
    case 0x110: {
        HWND item;
        wchar_t text[32];
        text[0] = 0;
        item = GetDlgItem(window, 0x733);
        SendMessageA(item, 0x4B2, 0, (LPARAM)unknown_end_score_name_850554);
        swprintf(text, StringTable::Load_String("GUI:CampaignLabel", 0, "D:\\Ra2\\CampScor.cpp", 722),
            unknown_end_score_campaign_850488);
        item = GetDlgItem(window, 0x735);
        SendMessageA(item, 0x4B2, 0, (LPARAM)text);
        if (unknown_end_score_difficulty_8504d0 == 0) {
            swprintf(text, StringTable::Load_String("GUI:DifficultyString", 0, "D:\\Ra2\\CampScor.cpp", 726),
                StringTable::Load_String("TXT_DIFFICULTY_EASY", 0, "D:\\Ra2\\CampScor.cpp", 726));
        } else if (unknown_end_score_difficulty_8504d0 == 1) {
            swprintf(text, StringTable::Load_String("GUI:DifficultyString", 0, "D:\\Ra2\\CampScor.cpp", 728),
                StringTable::Load_String("TXT_DIFFICULTY_MEDIUM", 0, "D:\\Ra2\\CampScor.cpp", 728));
        } else if (unknown_end_score_difficulty_8504d0 == 2) {
            swprintf(text, StringTable::Load_String("GUI:DifficultyString", 0, "D:\\Ra2\\CampScor.cpp", 730),
                StringTable::Load_String("TXT_DIFFICULTY_BRUTAL", 0, "D:\\Ra2\\CampScor.cpp", 730));
        } else {
            swprintf(text, StringTable::Load_String("GUI:Blank", 0, "D:\\Ra2\\CampScor.cpp", 732));
        }
        item = GetDlgItem(window, 0x732);
        SendMessageA(item, 0x4B2, 0, (LPARAM)text);
        unknown_format_time_467290(text, unknown_end_score_time_85049c / 60);
        item = GetDlgItem(window, 0x3EA);
        SendMessageA(item, 0x4B2, 0, (LPARAM)text);
        for (int i = 0; i < unknown_end_score_count_8504e8; i++) {
            ColorScheme *scheme = ColorSchemes[unknown_end_score_list_8504a8[i]->Scheme];
            RGBClass color = scheme->BaseColor.To_RGB();
            unsigned long rgb = color.Red | (color.Green << 8) | (color.Blue << 16);
            int id = unknown_score_controls_7d1484[i][0];
            wchar_t const *name = unknown_end_score_list_8504a8[i]->Name;
            item = GetDlgItem(window, id);
            SendMessageA(item, 0x4B2, 0, (LPARAM)name);
            item = GetDlgItem(window, id);
            SendMessageA(item, 0x498, 0, rgb);
            id = unknown_score_controls_7d1484[i][1];
            unknown_set_dialog_item_number_467240(window, id, unknown_end_score_list_8504a8[i]->Kills);
            item = GetDlgItem(window, id);
            SendMessageA(item, 0x498, 0, rgb);
            id = unknown_score_controls_7d1484[i][2];
            unknown_set_dialog_item_number_467240(window, id, unknown_end_score_list_8504a8[i]->Lost);
            item = GetDlgItem(window, id);
            SendMessageA(item, 0x498, 0, rgb);
            id = unknown_score_controls_7d1484[i][3];
            unknown_set_dialog_item_number_467240(window, id, unknown_end_score_list_8504a8[i]->Built);
            item = GetDlgItem(window, id);
            SendMessageA(item, 0x498, 0, rgb);
            id = unknown_score_controls_7d1484[i][4];
            unknown_set_dialog_item_number_467240(window, id, unknown_end_score_list_8504a8[i]->Score);
            item = GetDlgItem(window, id);
            SendMessageA(item, 0x498, 0, rgb);
            if (i == 0) {
                name = unknown_end_score_list_8504a8[0]->Name;
                item = GetDlgItem(window, 0x73A);
                SendMessageA(item, 0x4B2, 0, (LPARAM)name);
                item = GetDlgItem(window, 0x73A);
                SendMessageA(item, 0x498, 0, rgb);
                swprintf(text, L"%d", unknown_end_score_list_8504a8[0]->Score);
                item = GetDlgItem(window, 0x73B);
                SendMessageA(item, 0x4B2, 0, (LPARAM)text);
                item = GetDlgItem(window, 0x73B);
                SendMessageA(item, 0x498, 0, rgb);
            } else if (i == 1) {
                name = unknown_end_score_list_8504a8[1]->Name;
                item = GetDlgItem(window, 0x739);
                SendMessageA(item, 0x4B2, 0, (LPARAM)name);
                item = GetDlgItem(window, 0x739);
                SendMessageA(item, 0x498, 0, rgb);
                swprintf(text, L"%d", unknown_end_score_list_8504a8[1]->Score);
                item = GetDlgItem(window, 0x738);
                SendMessageA(item, 0x4B2, 0, (LPARAM)text);
                item = GetDlgItem(window, 0x738);
                SendMessageA(item, 0x498, 0, rgb);
            }
        }
        break;
    }
    case 0x111:
        if ((unsigned short)wparam == 0x6D1) {
            *done = 1;
            break;
        }
        break;
    case 0x497:
        {
            HWND item = GetDlgItem(window, 0x72B);
            PostMessageA(item, 0x4D7, 0, (LPARAM)window);
        }
        break;
    case 0x4D8:
        if (wparam == 0) {
            SendDlgItemMessageA(window, 0x72B, 0x4D5, wparam, 0x1D);
        }
        break;
    case 0x4EC:
        SendDlgItemMessageA(window, 0x72B, 0x4D3, 0, 0);
        break;
    case 0x4ED:
        unknown_5159b0(0, true);
        ShowWindow(window, 0);
        unknown_dialog_5ecf40(window, 1);
        break;
    }
    return 0;
}


// void unknown_sort_end_scores() -- 0x00467A00
// Sorts the end screen's score list best first.

extern "C" void __cdecl qsort(void *base, unsigned int count, unsigned int width, int (__cdecl *compare)(void const *, void const *));
int __cdecl unknown_compare_descending_60_00465f20(void const *left, void const *right);

void unknown_sort_end_scores_467a00()
{
    if (unknown_end_score_count_8504e8 > 1) {
        qsort(unknown_end_score_list_8504a8, unknown_end_score_count_8504e8, sizeof(MPlayerScoreType *),
            unknown_compare_descending_60_00465f20);
    }
}


// void CampaignEndScoreClass::Presentation(DynamicVectorClass<MPlayerScoreType *> scores, int time,
//     int difficulty, int campaign, wchar_t const *name) -- 0x00467A20
// Copies the scores (sorted best first), time, difficulty, campaign and name for the end
// screen, loads its art and runs the dialog 0xBC7, then releases the art.

extern "C" wchar_t *__cdecl wcscpy(wchar_t *destination, wchar_t const *source);
extern "C" void __cdecl qsort(void *base, unsigned int count, unsigned int width, int (__cdecl *compare)(void const *, void const *));
int __cdecl unknown_compare_descending_60_00465f20(void const *left, void const *right);

void CampaignEndScoreClass::Presentation(DynamicVectorClass<MPlayerScoreType *> scores, int time, int difficulty,
    int campaign, wchar_t const *name)
{
    unknown_end_score_count_8504e8 = scores.Count();
    wcscpy(unknown_end_score_name_850554, name);
    for (int i = 0; i < unknown_end_score_count_8504e8; i++) {
        unknown_end_score_list_8504a8[i] = scores[i];
    }
    if (unknown_end_score_count_8504e8 > 1) {
        qsort(unknown_end_score_list_8504a8, unknown_end_score_count_8504e8, sizeof(MPlayerScoreType *),
            unknown_compare_descending_60_00465f20);
    }
    unknown_end_score_campaign_850488 = campaign;
    unknown_end_score_difficulty_8504d0 = difficulty;
    unknown_end_score_time_85049c = time;
    int alternate = unknown_id_source_a3d290->unknown_2239 == false;
    Unknown_Load_Art_Once_006F2710(alternate);
    Unknown_Load_Art_Pair_006F2A30(alternate);
    unknown_5159b0(0, true);
    unknown_dialog_box_5eab60(0xBC7, unknown_end_score_dialog_proc_467330, 0);
    Unknown_006F2AB0();
    Unknown_Free_Art_Flagged_006F2750();
}


// ??_GCampaignScoreClass@@UAEPAXI@Z (0x00467B00): compiler generated with the vtable 0x0079D724; emitted by
// CampaignScoreClass_constructor_00465bf0.cpp (it inlines ~CampaignScoreClass). No source of its own.


// ??_GCampaignEndScoreClass@@UAEPAXI@Z (0x00467B20): compiler generated with the vtable 0x0079D730; emitted by
// CampaignEndScoreClass_constructor_00467200.cpp (it inlines ~CampaignEndScoreClass). No source of its own.
