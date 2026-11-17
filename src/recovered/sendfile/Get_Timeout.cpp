// int __fastcall Get_Timeout(int gametype) (0x0066AEA0): the file transfer response timeout in ticks for the
// game type (internet games wait three times longer); the other SendFile.CPP functions inline it.
#include "shared/debug_printf.hpp"

#define RESPONSE_TIMEOUT 60 * 60        // ticks

int __fastcall Get_Timeout(int gametype)
{
    switch (gametype) {
        case 3:
            Debug_Printf("GetTimeout: GAME_IPX (%d)\n", RESPONSE_TIMEOUT);
            return RESPONSE_TIMEOUT;
        case 4:
            Debug_Printf("GetTimeout: GAME_INTERNET (%d)\n", RESPONSE_TIMEOUT * 3);
            return RESPONSE_TIMEOUT * 3;
    }
    Debug_Printf("GetTimeout: RESPONSE_TIMEOUT(%d)\n", RESPONSE_TIMEOUT);
    return RESPONSE_TIMEOUT;
}
