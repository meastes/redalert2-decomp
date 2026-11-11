// RadarClass::Toggle_Radar (0x00633140): gains or loses radar (TibSun RadarClass::Toggle_Radar).
#include "shared/radar.hpp"
#include "shared/debug_printf.hpp"
// @ref ??_C@_0CH@IIEN@Radar?3?5TacticalMap?5availability?5@=007ee324
// @ref ??_C@_02PHBM@on?$AA@=007ee350
// @ref ??_C@_03OJPB@off?$AA@=007ee34c

void RadarClass::Toggle_Radar(bool on)
{
    if (unknown_14d4 != on) {
        unknown_14d4 = on;
        char const *txt = on ? "on" : "off";
        Debug_Printf("Radar: TacticalMap availability is %s\n", txt);
        if (unknown_14ac == 1) {
            Set_Radar_State(on, true);
        } else {
            Radar_Activate(1, true);
        }
    }
}
