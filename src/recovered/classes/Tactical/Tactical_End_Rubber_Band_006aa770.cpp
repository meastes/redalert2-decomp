// void Tactical::Unknown_006AA770() (0x006AA770): ends the selection band, both
// corners back to (0, 0) (TibSun Tactical::End_Rubber_Band).
#include "shared/tactical.hpp"

void Tactical::Unknown_006AA770()
{
    RubberBandStart = Point2D(0, 0);
    RubberBandEnd = Point2D(0, 0);
}
