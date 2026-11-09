// void Unknown_006F89E0() (0x006F89E0)
// Resets the group and radar-jump repeat state after a load (Scenario.cpp's
// post-load refresh 0x0065D2C0 and Clear_Scenario call it): no group, and both
// press frames far enough back that the next press is not a repeat.
#include "shared/unknown_globals.hpp"
// @ref ?unknown_group_frame_7f8bcc@@3HA=007f8bcc
// @ref ?unknown_group_7f8bd0@@3HA=007f8bd0
// @ref ?unknown_radar_jump_frame_7f8bd4@@3HA=007f8bd4
extern int unknown_group_frame_7f8bcc;          // 0x007F8BCC
extern int unknown_group_7f8bd0;                // 0x007F8BD0
extern int unknown_radar_jump_frame_7f8bd4;     // 0x007F8BD4

void Unknown_006F89E0()
{
    unknown_group_7f8bd0 = -1;
    unknown_group_frame_7f8bcc = -100;
    unknown_radar_jump_frame_7f8bd4 = -100;
}
