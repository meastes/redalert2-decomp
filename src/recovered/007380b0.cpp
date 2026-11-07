// IME_Get_Default_Result_007380b0: result returned after IME_Process_Message handles a message.
// Original getter reads only the global, with no object/this access.
// @ref ?IMEDefaultResult@@3JA=00b263f8
#include "shared/win_stub.hpp"
LRESULT __fastcall IME_Get_Default_Result_007380b0()
{
    return IMEDefaultResult;
}
