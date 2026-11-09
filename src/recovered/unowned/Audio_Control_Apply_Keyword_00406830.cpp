// bool __fastcall Audio_Control_Apply_Keyword(unsigned int *flags, char const *name) -- 0x00406830
// Purpose: looks the control keyword up (case-insensitive) in the table at
// 0x007CCB90, which ends with a null name; turns its flags on (the
// terminator's when the keyword is not there) and returns whether it was found.
// @ref __stricmp=00781f20
// @ref ?Audio_Control_Keywords@@3PAUAudioEventKeyword@@A=007ccb90
#include "shared/audio_event.hpp"

extern "C" int __cdecl _stricmp(char const *left, char const *right);

extern AudioEventKeyword Audio_Control_Keywords[];      // 0x007CCB90

bool __fastcall Audio_Control_Apply_Keyword(unsigned int *flags, char const *name)
{
    AudioEventKeyword *entry = Audio_Control_Keywords;

    while (entry->Name != 0 && _stricmp(name, entry->Name) != 0) {
        entry++;
    }
    if (flags != 0) {
        *flags |= entry->Flags;
    }
    return entry->Name != 0;
}
