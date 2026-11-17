// 0x00663220: function not yet identified
//
// Body: 26 bytes (frozen inventory bound).
// Referenced from 1 site (unconditional call).
// Ghidra prototype: void __fastcall Clear_All_Waypoints_00663220(int param_1)
//
// Types and names are Ghidra placeholders;
// rename and retype as the purpose becomes known.
//

namespace seed_00663220 {

extern "C" unsigned int _waypoint_none_00ab6e88;

void __fastcall Clear_All_Waypoints_00663220(int scenario)
{
    unsigned int *waypoint;
    int remaining;

    waypoint = (unsigned int *)(scenario + 0x632);
    remaining = (int)(0x65);
    do {
        *waypoint = _waypoint_none_00ab6e88;
        waypoint = (unsigned int *)(waypoint + 1);
        remaining = (int)(remaining + -1);
    } while (remaining != 0);
    return;
}

}  // namespace seed_00663220
