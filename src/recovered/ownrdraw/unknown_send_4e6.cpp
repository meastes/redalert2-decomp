// 0x005e1fd0: function not yet identified
//
// Body: 22 bytes (frozen inventory bound).
// Referenced from 0 sites.
// Ghidra prototype: void __fastcall unknown_send_4e6_005e1fd0(HWND param_1,unsigned char param_2)
//
// Types and names are Ghidra placeholders;
// rename and retype as the purpose becomes known.
//
// delete this line when editing it by hand.
#include "shared/win32_types.hpp"

namespace seed_005e1fd0 {

extern "C" __declspec(dllimport) unsigned int __stdcall SendMessageA(int, int, int, int);  // @ref __imp__SendMessageA@16=0079a4a0

void __fastcall unknown_send_4e6_005e1fd0(HWND window, unsigned char flag)
{
                                        /* WARNING: Subroutine does not return */
    SendMessageA((int)(window), (int)(0x4e6), (int)(0), (int)(flag & 0xff));
}

}  // namespace seed_005e1fd0
