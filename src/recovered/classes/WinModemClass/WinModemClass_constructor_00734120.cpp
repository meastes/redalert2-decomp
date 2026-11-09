// WinModemClass::WinModemClass() (0x00734120)
// An idle serial modem: allocates the 8 KB receive buffer, clears the buffer
// pointers, flags, hooks and error counters, and marks the port as not open.
// Defined here, the wincomm unit emits the vtable 0x007AF4E0 and the scalar
// deleting destructor 0x00735610 with it.
#include "shared/win_modem.hpp"
#include "shared/debug_printf.hpp"
// @ref ??_7WinModemClass@@6B@=007af4e0
// @ref ??2@YAPAXI@Z=00782017

WinModemClass::WinModemClass()
{
    SerialBuffer = new unsigned char[SIZE_OF_WINDOWS_SERIAL_BUFFER];
    SerialBufferReadPtr = 0;
    SerialBufferWritePtr = 0;
    WaitingForSerialCharRead = false;
    WaitingForSerialCharWrite = false;
    AbortFunction = 0;
    EchoFunction = 0;
    FramingErrors = 0;
    IOErrors = 0;
    BufferOverruns = 0;
    InBufferOverflows = 0;
    ParityErrors = 0;
    OutBufferOverflows = 0;
    PortHandle = INVALID_HANDLE_VALUE;
}
