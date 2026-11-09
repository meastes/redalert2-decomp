// void __fastcall Unknown_006f6780(unsigned int state) (0x006F6780): sets the command
// state word 0x00AC132C (the combatant/experience commands test 1 and 4) and clears
// the flag at 0x00AC1330.
#include "shared/uicmnds.hpp"
// @ref ?unknown_state_ac132c@@3HA=00ac132c

extern int unknown_state_ac132c;

void __fastcall Unknown_006f6780(unsigned int state)
{
    unknown_state_ac132c = state;
    unknown_flag_ac1330 = false;
}
