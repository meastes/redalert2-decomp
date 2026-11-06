// The campaign picker shared by Select_Game 0x005135F0 and its dialog procedure 0x005147A0 (Init.CPP).
#ifndef RA2_SHARED_CAMPAIGN_CHOICE_HPP
#define RA2_SHARED_CAMPAIGN_CHOICE_HPP

#ifdef RA2_INIT_SELECT_GAME  // Init.CPP unit macro (population first row)
#include "shared/win32_api.hpp"

// The dialog's DWL_USER payload: the chosen campaign (-1 none) and whether a choice was made.
struct UnknownCampaignChoice {
    int Campaign;
    bool Chosen;
};

int __stdcall Unknown_005147A0(HWND, UINT, WPARAM, LPARAM);

struct AudioEventHandle;
extern AudioEventHandle unknown_side_sound_a411e8;  // 0x00A411E8 the hovered side's sound
extern __int64 unknown_hover_time_a41680;           // 0x00A41680 when the hovered side's sound plays
extern int unknown_hover_id_a41688;                 // 0x00A41688 the hovered button
extern int unknown_side_voc_7db4f8;                 // 0x007DB4F8 the side sound to play (-1 none)

#endif

#endif
