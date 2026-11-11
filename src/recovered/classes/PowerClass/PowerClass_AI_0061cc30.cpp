// void PowerClass::AI(int &input, Point2D const &xy) (0x0061CC30, slot 10)
// While the power bar is shown: counts the first counter down once per
// expiry of the first timer (redrawing the bar), restarts it when the
// player's power output or drain changed, then once per expiry of the
// second timer moves one of the three counters a step toward its target
// and restarts that timer for the share of the bar they fill. Ends with
// the radar's AI.
#include "shared/power.hpp"
#include "shared/tool_tip.hpp"
#include "shared/map_view.hpp"
// @ref ?Map@@3VMouseClass@@A=008324e0
// @ref ?unknown_flag_8373c1@@3_NA=008373c1
// @ref ?unknown_flag_8373c2@@3_NA=008373c2
// @ref ?unknown_player_a35db4@@3PAVHouseClass@@A=00a35db4
// @ref ?Flag_To_Redraw@GScreenClass@@UAEXH@Z=004e09a0
// @ref ?AI@RadarClass@@UAEXAAHABUPoint2D@@@Z=0062fcc0
// @ref ?Desired_Levels@PowerClass@@QAEHAAH00@Z=0061c700
// @ref ?Desired_Power_Height@PowerClass@@QAEHXZ=0061c5f0
// @ref ??BSystemTimerClass@@QBEJXZ=0069a430
// @ref __real@8@4001a000000000000000=0079c278
// @ref __ftol=0077f110

void PowerClass::AI(int &input, Point2D const &xy)
{
    if (unknown_flag_8373c1) {
        if (!HasChanged && FlashCount > 0 && FlashTimer.Value() == 0) {
            IsToRedraw = true;
            FlashCount--;
            unknown_flag_8373c2 = true;
            Map.Flag_To_Redraw(false);
            FlashTimer = 3;
        }
        if (unknown_player_a35db4->Power_Drain() != RecordedDrain || unknown_player_a35db4->Power_Output() != RecordedPower || HasChanged) {
            IsToRedraw = true;
            unknown_flag_8373c2 = true;
            Map.Flag_To_Redraw(false);
            if (unknown_player_a35db4->Power_Drain() != RecordedDrain || unknown_player_a35db4->Power_Output() != RecordedPower) {
                HasChanged = true;
                FlashCount = 10;
                FlashTimer = 3;
            }
            RecordedDrain = unknown_player_a35db4->Power_Drain();
            RecordedPower = unknown_player_a35db4->Power_Output();
            int green;
            int yellow;
            int red;
            Desired_Levels(green, yellow, red);
            if (UpdateTimer.Value() == 0) {
                HasChanged = false;
                if (RedPipCount != red) {
                    HasChanged = true;
                    if (RedPipCount > red) {
                        RedPipCount--;
                        Add_Pip();
                    } else {
                        RedPipCount++;
                        Remove_Pip();
                    }
                } else if (GreenPipCount != green) {
                    HasChanged = true;
                    if (GreenPipCount > green) {
                        GreenPipCount--;
                        Add_Pip();
                    } else {
                        GreenPipCount++;
                        Remove_Pip();
                    }
                } else if (YellowPipCount != yellow) {
                    HasChanged = true;
                    if (YellowPipCount > yellow) {
                        YellowPipCount--;
                        Add_Pip();
                    } else {
                        YellowPipCount++;
                        Remove_Pip();
                    }
                }
                if (HasChanged) {
                    int segments = Desired_Power_Height();
                    int total = YellowPipCount + GreenPipCount + RedPipCount;
                    if (total > segments) {
                        total = segments;
                    }
                    UpdateTimer = (int)((double)total / segments * 5.0);
                }
            }
        }
    }
    RadarClass::AI(input, xy);
}
