// bool __fastcall Audio_Priority_From_Name(char const *name, int *priority) -- 0x004067E0
// Purpose: looks the priority name up (case-insensitive) in the table at
// 0x007CCAE8, which ends with a null name; stores its priority (the
// terminator's when the name is not there) and returns whether it was found.
// @ref __stricmp=00781f20
// @ref ?Audio_Priority_Names@@3PAUAudioPriorityName@@A=007ccae8
#include "shared/audio_event.hpp"

extern "C" int __cdecl _stricmp(char const *left, char const *right);

struct AudioPriorityName {
    char const *Name;
    int Priority;
};
extern AudioPriorityName Audio_Priority_Names[];        // 0x007CCAE8

bool __fastcall Audio_Priority_From_Name(char const *name, int *priority)
{
    AudioPriorityName *entry = Audio_Priority_Names;

    while (entry->Name != 0 && _stricmp(name, entry->Name) != 0) {
        entry++;
    }
    if (priority != 0) {
        *priority = entry->Priority;
    }
    return entry->Name != 0;
}
