#define RA2_WOL_PRIVATE_OPTIONS
#include "shared/wol_sink.hpp"
HRESULT __stdcall CNetUtilEventSink::OnPing(HRESULT result, int time, unsigned long ip, int handle)
{
    if (g_PlayingNetGame && unknown_b2a630) {
        if (result == 0 && time >= 0 && time < 1000) {
            ++unknown_b2aaf8;
            return 0;
        }
    } else if (Is_Online_Session()) {
        Debug_Printf("Received Ping\n");
        ++unknown_b26568;
        if (unknown_b26568 >= unknown_b29828) {
            if (unknown_b26898 == 4 || unknown_b26898 == 5) Unknown_0075B590();
            Unknown_0075B2D0();
            return 0;
        }
    } else if (unknown_b299c0) {
        Unknown_0073FEE0(0, false);
    }
    return 0;
}

