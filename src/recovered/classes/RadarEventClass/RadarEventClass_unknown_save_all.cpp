// bool __fastcall RadarEventClass_unknown_save_all(IStream *stream) -- 0x0063C310
// Purpose: writes the radar events (list 0x00AB67E8): their count, each
// event's 0x40 bytes, then the word at 0x00AB67B8; false when a write
// fails. Called by the save-game writer (0x00656711).
// Assumptions: the events are saved by raw copy (RadarEventClass is 0x40
// bytes); the trailing word's meaning and the names are hypotheses.
#include "shared/com_persist.hpp"
#define VECTOR_IMPLEMENT_ALL
#include "shared/vector_class.hpp"

class RadarEventClass;
extern DynamicVectorClass<RadarEventClass *> RadarEventClass_RadarEvents;
extern int LastRadarEventCell;

bool __fastcall RadarEventClass_unknown_save_all(IStream *stream)
{
    int count = RadarEventClass_RadarEvents.Count();
    if (FAILED(stream->Write(&count, sizeof(count), 0))) {
        return false;
    }
    for (int index = 0; index < count; index++) {
        if (FAILED(stream->Write(RadarEventClass_RadarEvents[index], 0x40, 0))) {
            return false;
        }
    }
    return SUCCEEDED(stream->Write(&LastRadarEventCell, sizeof(LastRadarEventCell), 0)) ? true : false;
}
