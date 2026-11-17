// bool RadarEventClass_unknown_none() -- 0x0063C070
// Purpose: whether the radar event list (0x00AB67E8) is empty.
// Assumptions: name hypothesis.
#define VECTOR_IMPLEMENT_ALL
#include "shared/vector_class.hpp"
// @ref ?RadarEventClass_RadarEvents@@3V?$DynamicVectorClass@PAVRadarEventClass@@@@A=00ab67e8

class RadarEventClass;
extern DynamicVectorClass<RadarEventClass *> RadarEventClass_RadarEvents;

bool RadarEventClass_unknown_none()
{
    return RadarEventClass_RadarEvents.Count() == 0 ? true : false;
}
