// Native M25 link closure rows (Scout1202), small bodies read from the original's code
// (pass-6a decompiles, which are whole for these). NOT EXACT: behaviour rows.
#define THEME_SOUND_CONTROLS
#define THEME_CONTROL_RECORD
#define RA2_TEAM_AI_HANDLERS
#define RA2_SELECT_ACTION_UNIT
#include "shared/house.hpp"
#include "shared/house_type.hpp"
#include "shared/house_nodes.hpp"
#include "shared/super.hpp"
#include "shared/special.hpp"
#include "shared/theme.hpp"
#include "shared/planning.hpp"
#include "shared/mouse_class.hpp"
#include "shared/map_view.hpp"

// 0x004EEC70: marks every scout node of house as preferred (the scout team's target
// house; the AI's scout script calls it when a team found nothing to attack there).
void HouseClass::unknown_4eec70(HouseClass *house)
{
    for (int i = 0; i < ScoutNodes.Count(); ++i) {
        if (ScoutNodes[i].House == house) {
            ScoutNodes[i].IsPreferred = true;
        }
    }
}

// 0x004F5B30: the owner type's float at +0x148 (UnitClass 0x00707130 scales a harvester's
// load value by it).
float HouseClass::Unknown_004F5B30()
{
    return Type->unknown_0x148;
}

// 0x0069D680 and 0x0068B420: empty bodies in the original (Leaf seeds).
void SuperClass::Unknown_0069D680()
{
}

void SpecialClass::Unknown_0068B420()
{
}

// 0x006E7CB0: a theme's wide display name (+0x200; 0 out of range); the sound controls'
// track list reads it. (Name 0x006E8570 and Base_Name 0x006E7C40 are accepted rows.)

unsigned short const *ThemeClass::Full_Name(unsigned theme) const
{
    if (theme < (unsigned)Themes.Count()) return Themes[theme]->FullName;
    return 0;
}

// 0x006F6750 / 0x006F6760: the first frame of mouse shapes 0x4D and 0x4C (the planning
// mode's cursors).
int PlanningEventTargets::Unknown_006F6750()
{
    return Map.Get_Mouse_Start_Frame(0x4D, false);
}

int PlanningEventTargets::Unknown_006F6760()
{
    return Map.Get_Mouse_Start_Frame(0x4C, false);
}
