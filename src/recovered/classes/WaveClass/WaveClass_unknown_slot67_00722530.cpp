// void WaveClass::unknown_slot67(Point2D const &point, Rect const &window) (0x00722530)
// Draws the wave by its type unless both ends are hidden (tunnels).
#include "shared/wave.hpp"
// @ref ?unknown_id_source_a3d290@@3PAVUnknownIDSource@@A=00a3d290
// @ref ?Is_Fogged@MapClass@@QAE_NABUCoordStruct@@@Z=005683c0
// @ref ?Draw_Sonic_007225b0@WaveClass@@QAEXABUPoint2D@@ABVRect@@@Z=007225b0
// @ref ?Draw_Laser_00722cb0@WaveClass@@QAEXABUPoint2D@@ABVRect@@@Z=00722cb0

void WaveClass::unknown_slot67(Point2D const &point, Rect const &window)
{
    if ((unknown_id_source_a3d290->unknown_000 & 0x1000) && Map.Is_Fogged(StartCoord) && Map.Is_Fogged(EndCoord)) {
        return;
    }
    switch (Type) {
    case 0:
        Draw_Sonic_007225b0(point, window);
        break;
    case 1:
    case 2:
        Draw_Laser_00722cb0(point, window);
        break;
    }
}
