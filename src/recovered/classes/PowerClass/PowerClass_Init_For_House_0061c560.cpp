// void PowerClass::Init_For_House() (0x0061C560, slot 53)
// After the radar's shapes, loads the power bar shape "POWERP.SHP".
#include "shared/power.hpp"
// @ref ?Init_For_House@RadarClass@@UAEXXZ=0062f310
// @ref ?Unknown_Load_Shape_00495270@@YIPBXPBDAA_N@Z=00495270
// @ref ?PowerPipShape@PowerClass@@2PBXB=00a7691c
// @ref ?PowerPipShapeAllocated@PowerClass@@2_NA=00a76920

void PowerClass::Init_For_House()
{
    RadarClass::Init_For_House();
    PowerPipShape = Unknown_Load_Shape_00495270("POWERP.SHP", PowerPipShapeAllocated);
}
// @ref ??_C@_0L@IPOE@POWERP?4SHP?$AA@=007ebf0c
