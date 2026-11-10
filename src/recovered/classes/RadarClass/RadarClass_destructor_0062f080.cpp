// RadarClass::~RadarClass() (0x0062F080)
// Releases the radar's list at +0x14BC and its tracking table (0x00631E40);
// the member vectors and the map layers follow.
#include "shared/radar.hpp"
// @ref ?Unknown_00405C10@UnknownList14BC@@QAEXXZ=00405c10
// @ref ?Clear_Radar@RadarClass@@QAEXXZ=00631e40
// @ref ??1MapClass@@UAE@XZ=00547b90
// @ref ??3@YAXPAX@Z=00781d3b

RadarClass::~RadarClass()
{
    unknown_14bc.Unknown_00405C10();
    Clear_Radar();
}
