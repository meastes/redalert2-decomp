// The four menu cheat-code records at 0x007DB500 (Init.CPP): read by the main menu 0x00516F00 and
// cleared by Select_Game 0x005135F0 for multiplayer games (Tiberian Sun's Cheat_Disable).
#ifndef RA2_SHARED_INIT_CHEAT_CODES_HPP
#define RA2_SHARED_INIT_CHEAT_CODES_HPP

#ifdef RA2_INIT_SELECT_GAME  // Init.CPP unit macro (population first row)
struct UnknownCheatCode {
    bool *Flag;                 // +0x0 the flag the code sets
    char const *Code;           // +0x4
    char const *unknown_8;      // +0x8
    bool AllowedInMultiplayer;  // +0xC (a byte; the record is 16 bytes)
};
extern UnknownCheatCode unknown_cheat_codes_7db500[4];

#endif

#endif
