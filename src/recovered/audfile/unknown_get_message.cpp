// BOOL __fastcall Get_Windows_Message(MSG *, HWND, UINT, UINT) -- 0x00522E90
// Purpose: the game's GetMessage: polls Peek_Windows_Message (remove)
// until it yields a message; FALSE for WM_QUIT (shared/movie.hpp).
#include "shared/win32_api.hpp"
#include "shared/movie.hpp"
// @ref ?Peek_Windows_Message@@YIHPAUtagMSG@@PAUHWND__@@III@Z=00522ed0

BOOL __fastcall Get_Windows_Message(MSG *message, HWND window, UINT first, UINT last)
{
    BOOL result;
    do {
        result = Peek_Windows_Message(message, window, first, last, 1);
    } while (!result);
    if (message->message == 0x12) {
        result = 0;
    }
    return result;
}
