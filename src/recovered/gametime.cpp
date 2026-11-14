// gametime.cpp: the original translation unit re-filed as one source (Route A): the accepted
// bodies of its canonical compile group in address order, under the unit's own header
// state (its switches and includes in their original order). The module map
// manifests/original_target/modules/gametime.json carries the addresses and the rows that
// stay in their own files.


#include "shared/unknown_time_stamp.hpp"
#include "shared/win32_api.hpp"


// The global GameTimeClass at 0x00866074; its dynamic
// initializer 0x004D0030 is the constructor 0x004D0040 expanded inline.

GameTimeClass unknown_time_stamp_866074;


// GameTimeClass::GameTimeClass() -- 0x004D0040
// Purpose: records the current system time (timeGetTime) at +0x00. No
// direct callers (the out-of-line copy of the constructor the global
// 0x00866074's initializer expands). Owner not identified.

GameTimeClass::GameTimeClass()
{
    Time = timeGetTime();
}


// int GameTimeClass::unknown_elapsed_004d0050() const -- 0x004D0050
// Purpose: milliseconds since the recorded time (+0x00), computed across a
// timeGetTime wrap (one less when the clock has wrapped). No direct callers.

int GameTimeClass::unknown_elapsed_004d0050() const
{
    unsigned long now = timeGetTime();
    if (now <= Time) {
        return now - Time - 1;
    }
    return now - Time;
}


// int unknown_milliseconds_004d0070() (0x004D0070)
// Milliseconds since the startup time stamp 0x00866074 (its elapsed time expanded inline).
// Unreferenced; no inventory row.

int unknown_milliseconds_004d0070()
{
    return unknown_time_stamp_866074.unknown_elapsed_004d0050();
}


// unsigned long unknown_ticks_004d0090() (0x004D0090)
// Game ticks (1/60 s) elapsed since the startup time stamp 0x00866074.

unsigned long unknown_ticks_004d0090()
{
    return (unsigned long)unknown_time_stamp_866074.unknown_elapsed_004d0050() * 3 / 50;
}
