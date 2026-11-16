// Behaviour-written ThemeClass row the skirmish harness reaches with audio
// (Scout1098, native M9). NOT EXACT; never original code or bytes.
#define THEME_CONTROL_RECORD
#include "shared/theme.hpp"
#include "shared/house.hpp"
#include "shared/house_type.hpp"
#include "shared/id_source.hpp"
#include "shared/unknown_globals.hpp"

// 0x006E8440 ThemeClass::Is_Allowed (seed decompile; TibSun
// ThemeClass::Is_Allowed): the special values -3 and -2 are always allowed;
// otherwise the theme must exist, be available (its file found) and normal,
// belong to the player's side (Owner -1: any side) and, outside multiplayer,
// not be above the scenario's level (+0x280 against 0x00A3D290 +0x834).
bool ThemeClass::Is_Allowed(unsigned theme)
{
    if (theme != 0xFFFFFFFDu && theme != 0xFFFFFFFEu) {
        if (theme >= (unsigned)Themes.Count()) {
            return false;
        }
        ThemeControl *control = Themes[theme];
        if (!control->Available) {
            return false;
        }
        if (!control->unknown_288) {
            return false;
        }
        if (PlayerPtr != 0 && control->Owner != -1 && PlayerPtr->Type->Side != control->Owner) {
            return false;
        }
        if (unknown_game_mode_a3d298 == 0 && Scen->TechLevel < control->Scenario) {
            return false;
        }
    }
    return true;
}
