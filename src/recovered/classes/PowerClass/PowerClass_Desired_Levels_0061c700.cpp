// int PowerClass::Desired_Levels(int &green, int &yellow, int &red) (0x0061C700)
// The three power bar counters' targets: the segments 0x0061C5F0 leaves are
// split by the shares of the player's drain (red), of the surplus
// output up to 100 (yellow) and of the rest of
// the surplus (green); what the truncations lose goes to
// red. Returns the bar's segment count (0x0061C6E0).
#include "shared/power.hpp"
// @ref ?unknown_player_a35db4@@3PAVHouseClass@@A=00a35db4
// @ref ?unknown_int_abcd70@@3HA=00abcd70
// @ref ?Desired_Power_Height@PowerClass@@QAEHXZ=0061c5f0
// @ref __real@8@00000000000000000000=0079b610
// @ref __real@8@4005c800000000000000=0079b7f0
// @ref __real@8@3ff8a3d70a3d70a3d800=0079c518
// @ref __ftol=0077f110

int PowerClass::Desired_Levels(int &green, int &yellow, int &red)
{
    int segments = Max_Power_Height();
    int count = Desired_Power_Height();
    int surplus = unknown_player_a35db4->Power_Output() - unknown_player_a35db4->Power_Drain();
    double value = surplus;
    double first = 100.0;
    double second;
    if (value < 0.0) {
        first = 0.0;
        second = 0.0;
    } else {
        if (value < 100.0) {
            first = value;
        }
        second = value - first;
    }
    double drain_share = 1.0;
    double second_share = 0.0;
    double first_share = 0.0;
    double total = unknown_player_a35db4->Power_Drain();
    total += first;
    total += second;
    if (total > 0.0) {
        drain_share = unknown_player_a35db4->Power_Drain() / total;
        second_share = second / total;
        first_share = first / total;
    }
    double bar = count;
    double drain_part = bar * drain_share;
    red = (int)drain_part;
    double first_part = bar * first_share;
    yellow = (int)first_part;
    double second_part = bar * second_share;
    green = (int)second_part;
    red = (int)((first_part - yellow) + (second_part - green) + (drain_part - red) + red + 0.01);
    return segments;
}
