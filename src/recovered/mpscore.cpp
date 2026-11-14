// mpscore.cpp: the original translation unit re-filed as one source (Route A): the accepted
// bodies of its canonical compile group in address order, under the unit's own header
// state (its switches and includes in their original order). The module map
// manifests/original_target/modules/mpscore.json carries the addresses and the rows that
// stay in their own files.

// Header-static dynamic initializers of the unit at 0x005AA830: the shape_file.hpp empty rect and the
// shared/unknown_geometry.hpp constants (0x005AA830-0x005AAA11); the other
// drafts of this plan are emitted by these includes.
#include "shared/shape_file.hpp"
#include "shared/unknown_geometry.hpp"


// Dynamic initializer (0x005AA850, _$E) of diagonal from the header(s) included
// by HeaderStatics_empty_rect_init_005aa830.cpp; no source of its own.


// Dynamic initializer (0x005AA880, _$E) of b from the header(s) included
// by HeaderStatics_empty_rect_init_005aa830.cpp; no source of its own.


// Dynamic initializer (0x005AA8A0, _$E) of c from the header(s) included
// by HeaderStatics_empty_rect_init_005aa830.cpp; no source of its own.


// Dynamic initializer (0x005AA8C0, _$E) of angle45 from the header(s) included
// by HeaderStatics_empty_rect_init_005aa830.cpp; no source of its own.


// Dynamic initializer (0x005AA8E0, _$E) of angle60 from the header(s) included
// by HeaderStatics_empty_rect_init_005aa830.cpp; no source of its own.


// Dynamic initializer (0x005AA900, _$E) of angle90 from the header(s) included
// by HeaderStatics_empty_rect_init_005aa830.cpp; no source of its own.


// Dynamic initializer (0x005AA920, _$E) of g from the header(s) included
// by HeaderStatics_empty_rect_init_005aa830.cpp; no source of its own.


// Dynamic initializer (0x005AA960, _$E) of h from the header(s) included
// by HeaderStatics_empty_rect_init_005aa830.cpp; no source of its own.


// Dynamic initializer (0x005AA990, _$E) of i from the header(s) included
// by HeaderStatics_empty_rect_init_005aa830.cpp; no source of its own.


// Dynamic initializer (0x005AA9C0, _$E) of j from the header(s) included
// by HeaderStatics_empty_rect_init_005aa830.cpp; no source of its own.


// Dynamic initializer (0x005AA9F0, _$E) of k from the header(s) included
// by HeaderStatics_empty_rect_init_005aa830.cpp; no source of its own.


// Dynamic initializer (0x005AAA00, _$E) of l from the header(s) included
// by HeaderStatics_empty_rect_init_005aa830.cpp; no source of its own.




#define VECTOR_IMPLEMENT_VECTOR_ACCESSORS
#define VECTOR_IMPLEMENT_DYNAMIC_ACCESSORS
#define VECTOR_IMPLEMENT_VECTOR_DESTRUCTOR
#include "shared/house.hpp"
#include "shared/debug_printf.hpp"
#include "shared/session_state.hpp"
#include "shared/id_source.hpp"
#include "shared/ms_message_box.hpp"
#include "shared/color_scheme.hpp"
#include "shared/string_table.hpp"
#include "shared/campaign_score.hpp"
#include "shared/theme.hpp"
#include "shared/win32_window.hpp"
#include "shared/movie_globals.hpp"
#include "shared/crt_string.hpp"
#include "shared/dsurface.hpp"
#include "shared/mpscore.hpp"


// UnknownScore_005AAA20::UnknownScore_005AAA20() (0x005AAA20): clears the screen's statics.

extern "C" wchar_t *__cdecl wcscpy(wchar_t *destination, wchar_t const *source);
void __fastcall Unknown_6F3CD0(Surface *surface, Rect const &window);
void __fastcall Unknown_Rect_Shrink_006F1B30(Rect *rect, int x, int y);

UnknownScore_005AAA20::UnknownScore_005AAA20()
{
    unknown_score_a71684 = 0;
    unknown_score_a71688 = 0;
    memset(unknown_sorted_scores_a715f0, 0, sizeof(unknown_sorted_scores_a715f0));
}


// UnknownScore_005AAA20::Presentation_005AAA40() (0x005AAA40): loads the score art, draws the backdrop,
// runs the score dialog 0x108, draws the backdrop again and frees the art.

void UnknownScore_005AAA20::Presentation_005AAA40()
{
    Unknown_Load_Art_Pair_006F2A30(unknown_id_source_a3d290->unknown_2239 == false);
    Rect rect(0, 0, unknown_8523e0, unknown_8523e4);
    Unknown_6F3CD0(unknown_surface_839954, rect);
    Unknown_004E0C90(true, unknown_surface_839954, 0);
    unknown_dialog_box_5eab60(0x108, MP_Score_Dialog_Proc_005AAE40, 0);
    Unknown_6F3CD0(unknown_surface_839954, rect);
    Unknown_004E0C90(true, unknown_surface_839954, 0);
    Unknown_006F2AB0();
}


// MP_Format_Time_005AAAE0 (0x005AAAE0): CampScor.cpp's unknown_format_time_467290 with its own source line.

void __fastcall MP_Format_Time_005AAAE0(wchar_t *buffer, int time)
{
    if (buffer) {
        if (time / 3600 > 99) {
            time = 359999;
        }
        int hours = time / 3600;
        time -= hours * 3600;
        int minutes = time / 60;
        time -= minutes * 60;
        swprintf(buffer, StringTable::Load_String("TXT_TIME_FORMAT_HOURS", 0, "D:\\Ra2\\MPScore.cpp", 129), hours, minutes, time);
    }
}


// MP_Set_Dialog_Item_Number_005AAB80 (0x005AAB80): CampScor.cpp's unknown_set_dialog_item_number_467240.

void __fastcall MP_Set_Dialog_Item_Number_005AAB80(HWND dialog, int id, int value)
{
    wchar_t text[32];
    swprintf(text, L"%d", value);
    HWND item = GetDlgItem(dialog, id);
    SendMessageA(item, 0x4b2, 0, (LPARAM)text);
}


// MP_Tally_Scores_005AABD0 (0x005AABD0): fills the session's score records from every house that is not
// multiplayer-passive or the player without a team (CampaignScoreClass::unknown_465d10's copy; built is the
// sum of the four counters at +0x54A0, and a house that did not lose gets a random bonus).

void MP_Tally_Scores_005AABD0()
{
    unknown_session_a3d298.ScoreCount = 0;
    int count = Houses.Count();
    for (int i = 0; i < count; i++) {
        HouseClass *house = Houses[i];
        if (house != 0 && house->Type->MultiplayPassive != true && house != unknown_house_a72d28) {
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
            unknown_session_a3d298.Scores[index].Lost = house->unknown_53b4 + house->unknown_5360;
            int built = house->unknown_54a0.Sum();
            built += house->unknown_54b0.Sum();
            built += house->unknown_54c0.Sum();
            built += house->unknown_54d0.Sum();
            unknown_session_a3d298.Scores[index].Built = built;
            if (house->unknown_5418 > 0) {
                unknown_session_a3d298.Scores[index].Score += house->unknown_5418;
            }
            if (!house->unknown_13d) {
                int score = unknown_session_a3d298.Scores[index].Score;
                unknown_session_a3d298.Scores[index].Score = Random_Pick(score / 2, score) + score / 2 + score;
            }
            Debug_Printf("%hs: %s\n Scheme: %d\n Lost = %d\n Kills = %d\n Built = %d\n Score = %d\n",
                unknown_session_a3d298.Scores[index].Name,
                unknown_session_a3d298.Scores[index].Winner > 0 ? "Winner" : "Loser",
                unknown_session_a3d298.Scores[index].Scheme, unknown_session_a3d298.Scores[index].Lost,
                unknown_session_a3d298.Scores[index].Kills, unknown_session_a3d298.Scores[index].Built,
                unknown_session_a3d298.Scores[index].Score);
        }
    }
}


// void Unknown_005AADD0() (0x005AADD0)
// Sorts the session's used score records by their +0x60 value, largest first: fills the pointer table at
// 0x00A715F0 with &Scores[0..ScoreCount) and qsorts it with 0x005AAE10.
extern MPlayerScoreType *unknown_sorted_scores_a715f0[8];
int __cdecl unknown_compare_descending_60_005aae10(void const *left, void const *right);

void Unknown_005AADD0()
{
    int count = unknown_session_a3d298.ScoreCount;
    for (int index = 0; index < count; index++) {
        unknown_sorted_scores_a715f0[index] = &unknown_session_a3d298.Scores[index];
    }
    if (count > 1) {
        qsort(unknown_sorted_scores_a715f0, count, sizeof(MPlayerScoreType *), unknown_compare_descending_60_005aae10);
    }
}


// int __cdecl unknown_compare_descending_60(void const *, void const *) -- 0x005AAE10
// Purpose: qsort comparator over an array of object pointers, ordering by the
// int at +0x60 from largest to smallest (0 when equal). Referenced from 1
// data site (a qsort call). The pointee type is not yet identified.
struct UnknownSortItem_005aae10 {
    char unknown_00[0x60];
    int unknown_60;
};

int __cdecl unknown_compare_descending_60_005aae10(void const *left, void const *right)
{
    UnknownSortItem_005aae10 const *a = *(UnknownSortItem_005aae10 const * const *)left;
    UnknownSortItem_005aae10 const *b = *(UnknownSortItem_005aae10 const * const *)right;
    if (a->unknown_60 == b->unknown_60) {
        return 0;
    }
    return a->unknown_60 > b->unknown_60 ? -1 : 1;
}
