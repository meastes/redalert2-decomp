// WinModemClass::~WinModemClass() (0x00734170)
// Closes the port if it is open (Serial_Port_Close's steps, as Serial_Port_Open
// 0x00734410 writes them) and frees the receive buffer.
#include "shared/win_modem.hpp"
#include "shared/debug_printf.hpp"
// @ref ??_C@_0BF@GLCD@Closing?5serial?5port?6?$AA@=007fca0c
// @ref ??3@YAXPAX@Z=00781d3b

WinModemClass::~WinModemClass()
{
    if (PortHandle != INVALID_HANDLE_VALUE) {
        Debug_Printf("Closing serial port\n");
        CloseHandle(PortHandle);
        PortHandle = INVALID_HANDLE_VALUE;
    }
    if (SerialBuffer) {
        delete [] SerialBuffer;
    }
}
