// void RadarEventClass_unknown_delete_all_63b890() -- 0x0063B890
// Purpose: deletes every radar event of the list at 0x00AB67E8, last
// first, removing each from the list before freeing it.
// Assumptions: RadarEventClass needs no destructor here (the memory is
// freed directly); 0x0063B890 and 0x0063C640 are identical copies (two
// units). Name hypothesis.
#define VECTOR_IMPLEMENT_ALL
#include "shared/vector_class.hpp"
// @ref ?RadarEventClass_RadarEvents@@3V?$DynamicVectorClass@PAVRadarEventClass@@@@A=00ab67e8
// @ref ??3@YAXPAX@Z=00781d3b

class RadarEventClass {
public:
    int unknown_00;
};
extern DynamicVectorClass<RadarEventClass *> RadarEventClass_RadarEvents;

void RadarEventClass_unknown_delete_all_63b890()
{
    for (int index = RadarEventClass_RadarEvents.Count() - 1; index >= 0; index--) {
        RadarEventClass *event = RadarEventClass_RadarEvents[index];
        if (event != 0) {
            RadarEventClass *pointer = event;
            RadarEventClass_RadarEvents.Delete(pointer);
            delete event;
        }
    }
}
