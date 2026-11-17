// 0x007348b0: function not yet identified
//
// Owner: WinModemClass, virtual slot 3 (vtable +0xc) (from RTTI/vtables).
// Body: 65 bytes (frozen inventory bound).
// Referenced from 1 site (data).
// Ghidra prototype: uint __fastcall Get_Modem_Status_007348b0(int param_1)
//
// Types and names are Ghidra placeholders;
// rename and retype as the purpose becomes known.
//

#include "shared/win32_types.hpp"

namespace seed_007348b0 {

extern "C" __declspec(dllimport) unsigned int __stdcall GetCommModemStatus(int, int);  // @ref __imp__GetCommModemStatus@8=0079a174

unsigned int __fastcall Get_Modem_Status_007348b0(int self)
{
    unsigned int status;
    DWORD modem_status;

    status = (unsigned int)(0);
    modem_status = (DWORD)(0);
    GetCommModemStatus((int)(*(HANDLE *)(self + 0x5c)), (int)(&modem_status));
    if ((modem_status & 0x10) != 0) {
        status = (unsigned int)(0x10);
    }
    if ((modem_status & 0x20) != 0) {
        status = (unsigned int)(status | 0x20);
    }
    if ((modem_status & 0x40) != 0) {
        status = (unsigned int)(status | 0x40);
    }
    if ((modem_status & 0x80) != 0) {
        status = (unsigned int)(status | 0x80);
    }
    return status;
}

}  // namespace seed_007348b0
