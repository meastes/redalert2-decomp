// A recorded timeGetTime() value (0x004D0040) and the global one built at
// startup (0x00866074, dynamic initializer 0x004D0030). Owner not identified.
#ifndef RA2_SHARED_UNKNOWN_TIME_STAMP_HPP
#define RA2_SHARED_UNKNOWN_TIME_STAMP_HPP

struct GameTimeClass {
    GameTimeClass();                    // 0x004D0040 records timeGetTime()
    int unknown_elapsed_004d0050() const;           // 0x004D0050 milliseconds since then

    unsigned long Time;     // +0x00
};

extern GameTimeClass unknown_time_stamp_866074;   // 0x00866074

// Milliseconds since the startup time stamp 0x00866074 (0x004D0070; unreferenced).
int unknown_milliseconds_004d0070();

#endif
