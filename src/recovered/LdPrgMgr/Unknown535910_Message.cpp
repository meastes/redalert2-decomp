// bool Unknown535910::Message_00536ea0(int message, void *data) -- 0x00536EA0 (slot 0)
// Message 0x11AE (the progress screen's percent) copies the surface to the screen.
#include "shared/unknown_globals.hpp"
#include "shared/dsurface.hpp"

bool Unknown535910::Message_00536ea0(int message, void *data)
{
    if (message == 0x11AE) {
        Unknown_00536E00(HiddenSurface);
        return true;
    }
    return false;
}
