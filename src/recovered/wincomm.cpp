// wincomm.cpp: the original translation unit re-filed as one source (Route A): the accepted
// bodies of its canonical compile group in address order, under the unit's own header
// state (its switches and includes in their original order). The module map
// manifests/original_target/modules/wincomm.json carries the addresses and the rows that
// stay in their own files.


#include "shared/win_modem.hpp"
#include "shared/debug_printf.hpp"
#include "shared/crt_string.hpp"
#include "shared/crt_memory.hpp"
#include "shared/cd_timer.hpp"
#include "shared/system_timer.hpp"
#include "shared/unknown_globals.hpp"
#include "shared/crt_tokens.hpp"


// void WinModemClass::Set_Echo_Function(void (__fastcall *func)(char c)) (0x007354A0, slot 8).

void WinModemClass::Set_Echo_Function(void (__fastcall *func)(char c))
{
    EchoFunction = func;
}


// void WinModemClass::Set_Abort_Function(int (*func)()) (0x007354B0, slot 9).

void WinModemClass::Set_Abort_Function(int (*func)())
{
    AbortFunction = func;
}


// HANDLE WinModemClass::Get_Port_Handle() (0x007354C0).

HANDLE WinModemClass::Get_Port_Handle()
{
    return PortHandle;
}


// 0x007354d0: function not yet identified
//
// Body: 317 bytes (frozen inventory bound).
// Referenced from 4 sites (unconditional call).
// Ghidra prototype: void __thiscall Process_Errors_007354d0(int param_1,uint param_2,byte param_3)
// Callees with accepted source:
//   Debug_Log_004068f0 -> src/recovered/004068f0.cpp
//
// Types and names are Ghidra placeholders;
// rename and retype as the purpose becomes known.
//

namespace seed_007354d0 {

extern void  Debug_Log_004068f0(int);
extern "C" char s_Comm_status___Character_waiting_f_007fcdd0[];
extern "C" char s_Comm_status___EOF_character_sent_007fcdf8[];
extern "C" char s_Comm_status___Tx_waiting__XOFF_c_007fce1c[];
extern "C" char s_Comm_status___Tx_waiting__XOFF_c_007fce48[];
extern "C" char s_Comm_status___Tx_waiting_for_CTS_007fced0[];
extern "C" char s_Comm_status___Tx_waiting_for_DSR_007fcea4[];
extern "C" char s_Comm_status___Tx_waiting_for_RLS_007fce78[];
extern "C" char s_Communications__Break__detected__007fcff8[];
extern "C" char s_Communications__Framing__error_d_007fd01c[];
extern "C" char s_Communications__IO__error_detect_007fcfd0[];
extern "C" char s_Communications__Parity__error_de_007fcf34[];
extern "C" char s_Communications__Receive_Overrun__007fcf9c[];
extern "C" char s_Communications__Receive_Queue_Ov_007fcf60[];
extern "C" char s_Communications__Transmit_Queue_F_007fcefc[];

struct Owner_007354d0 { void Process_Errors_007354d0(unsigned int errors, unsigned char status, int unused_2, int unused_3); };

void Owner_007354d0::Process_Errors_007354d0(unsigned int errors, unsigned char status, int unused_2, int unused_3)
{
    int self = (int)this;
    if (errors != 0) {
        if ((errors & 8) != 0) {
            Debug_Log_004068f0((int)(s_Communications__Framing__error_d_007fd01c));
            *(int *)(self + 4) = *(int *)(self + 4) + 1;
        }
        if ((errors & 0x10) != 0) {
            Debug_Log_004068f0((int)(s_Communications__Break__detected__007fcff8));
        }
        if ((errors & 0x400) != 0) {
            Debug_Log_004068f0((int)(s_Communications__IO__error_detect_007fcfd0));
            *(int *)(self + 8) = *(int *)(self + 8) + 1;
        }
        if ((errors & 2) != 0) {
            Debug_Log_004068f0((int)(s_Communications__Receive_Overrun__007fcf9c));
            *(int *)(self + 0xc) = *(int *)(self + 0xc) + 1;
        }
        if ((errors & 1) != 0) {
            Debug_Log_004068f0((int)(s_Communications__Receive_Queue_Ov_007fcf60));
            *(int *)(self + 0x10) = *(int *)(self + 0x10) + 1;
        }
        if ((errors & 4) != 0) {
            Debug_Log_004068f0((int)(s_Communications__Parity__error_de_007fcf34));
            *(int *)(self + 0x14) = *(int *)(self + 0x14) + 1;
        }
        if ((errors & 0x100) != 0) {
            Debug_Log_004068f0((int)(s_Communications__Transmit_Queue_F_007fcefc));
            *(int *)(self + 0x18) = *(int *)(self + 0x18) + 1;
        }
    }
    if ((status & 1) != 0) {
        Debug_Log_004068f0((int)(s_Comm_status___Tx_waiting_for_CTS_007fced0));
    }
    if ((status & 2) != 0) {
        Debug_Log_004068f0((int)(s_Comm_status___Tx_waiting_for_DSR_007fcea4));
    }
    if ((status & 4) != 0) {
        Debug_Log_004068f0((int)(s_Comm_status___Tx_waiting_for_RLS_007fce78));
    }
    if ((status & 8) != 0) {
        Debug_Log_004068f0((int)(s_Comm_status___Tx_waiting__XOFF_c_007fce48));
    }
    if ((status & 0x10) != 0) {
        Debug_Log_004068f0((int)(s_Comm_status___Tx_waiting__XOFF_c_007fce1c));
    }
    if ((status & 0x20) != 0) {
        Debug_Log_004068f0((int)(s_Comm_status___EOF_character_sent_007fcdf8));
    }
    if ((status & 0x40) != 0) {
        Debug_Log_004068f0((int)(s_Comm_status___Character_waiting_f_007fcdd0));
    }
    return;
}

}  // namespace seed_007354d0


// WinModemClass::WinModemClass() (0x00734120)
// An idle serial modem: allocates the 8 KB receive buffer, clears the buffer
// pointers, flags, hooks and error counters, and marks the port as not open.
// Defined here, the wincomm unit emits the vtable 0x007AF4E0 and the scalar
// deleting destructor 0x00735610 with it.

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


// WinModemClass::~WinModemClass() (0x00734170)
// Closes the port if it is open (Serial_Port_Close's steps, as Serial_Port_Open
// 0x00734410 writes them) and frees the receive buffer.

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


// HKEY __fastcall Get_Registry_Sub_Key(HKEY base_key, char const *search_key, bool close) (0x007341C0)

HKEY __fastcall Get_Registry_Sub_Key(HKEY base_key, char const *search_key, bool close)
{
    char class_buffer[1024];
    DWORD class_buffer_size = sizeof(class_buffer);
    DWORD num_sub_keys;
    DWORD longest_sub_key_name;
    DWORD longest_class_string;
    DWORD num_value_entries;
    DWORD longest_value_name_length;
    DWORD longest_value_data_length;
    DWORD security_descriptor_length;
    FILETIME last_write_time;
    if (RegQueryInfoKeyA(base_key, class_buffer, &class_buffer_size, 0, &num_sub_keys, &longest_sub_key_name,
                         &longest_class_string, &num_value_entries, &longest_value_name_length,
                         &longest_value_data_length, &security_descriptor_length, &last_write_time) != 0) {
        return 0;
    }
    DWORD sub_key_buffer_size = longest_sub_key_name + 16;
    char *sub_key_buffer = new char[sub_key_buffer_size];
    char *sub_key_class = new char[longest_class_string + 1];
    for (DWORD key_num = 0; key_num < num_sub_keys; key_num++) {
        *sub_key_buffer = 0;
        longest_sub_key_name = sub_key_buffer_size;
        RegEnumKeyExA(base_key, key_num, sub_key_buffer, &longest_sub_key_name, 0, sub_key_class, &longest_class_string,
                      &last_write_time);
        if (_stricmp(search_key, sub_key_buffer) == 0) {
            HKEY result_key;
            if (RegOpenKeyExA(base_key, sub_key_buffer, 0, KEY_READ, &result_key) == 0) {
                if (close) {
                    RegCloseKey(base_key);
                }
                delete[] sub_key_buffer;
                delete[] sub_key_class;
                return result_key;
            }
        }
    }
    if (close) {
        RegCloseKey(base_key);
    }
    delete[] sub_key_buffer;
    delete[] sub_key_class;
    return 0;
}


// bool __fastcall Get_Modem_Name_From_Registry(char *buffer, int buffer_len) (0x00734310)
// Copies the FriendlyName of HKEY_LOCAL_MACHINE\Enum\Root\Modem\0000.

bool __fastcall Get_Modem_Name_From_Registry(char *buffer, int buffer_len)
{
    char value[256];
    DWORD value_size = sizeof(value);
    HKEY key = Get_Registry_Sub_Key(HKEY_LOCAL_MACHINE, "Enum", false);
    if (key == 0) {
        return false;
    }
    key = Get_Registry_Sub_Key(key, "Root", true);
    if (key == 0) {
        return false;
    }
    key = Get_Registry_Sub_Key(key, "Modem", true);
    if (key == 0) {
        return false;
    }
    key = Get_Registry_Sub_Key(key, "0000", true);
    if (key == 0) {
        return false;
    }
    if (RegQueryValueExA(key, "FriendlyName", 0, 0, (BYTE *)value, &value_size) != 0) {
        RegCloseKey(key);
        return false;
    }
    RegCloseKey(key);
    memcpy(buffer, value, buffer_len < (int)value_size ? buffer_len : value_size);
    return true;
}


// HANDLE WinModemClass::Serial_Port_Open(char const *port_name, int baud, int parity, int wordlen, int stopbits) (0x00734410)
// Opens \\.\port_name for overlapped I/O with 8 KB queues and RTS/CTS flow
// control, records the settings and raises DTR; INVALID_HANDLE_VALUE on
// failure.

HANDLE WinModemClass::Serial_Port_Open(char const *port_name, int baud, int parity, int wordlen, int stopbits)
{
    char device_name[266] = "\\\\.\\";
    strcat(device_name, port_name);
    if (PortHandle != INVALID_HANDLE_VALUE) {
        Debug_Printf("Closing serial port\n");
        CloseHandle(PortHandle);
        PortHandle = INVALID_HANDLE_VALUE;
    }
    Debug_Printf("Opening serial port %s\n", device_name);
    HANDLE com_handle = CreateFileA(device_name, GENERIC_READ | GENERIC_WRITE, 0, 0, OPEN_EXISTING,
                                    FILE_FLAG_OVERLAPPED, 0);
    if (com_handle == INVALID_HANDLE_VALUE) {
        Debug_Printf("Failed to get valid port handle. Error: %s\n", 0);
        return INVALID_HANDLE_VALUE;
    }
    Debug_Printf("Port opened OK\n");
    PortHandle = com_handle;
    SetupComm(com_handle, SIZE_OF_WINDOWS_SERIAL_BUFFER, SIZE_OF_WINDOWS_SERIAL_BUFFER);
    PurgeComm(PortHandle, 0x0F);
    ReadOverlap.Internal = 0;
    ReadOverlap.InternalHigh = 0;
    ReadOverlap.Offset = 0;
    ReadOverlap.OffsetHigh = 0;
    ReadOverlap.hEvent = CreateEventA(0, true, true, 0);
    WriteOverlap.Internal = 0;
    WriteOverlap.InternalHigh = 0;
    WriteOverlap.Offset = 0;
    WriteOverlap.OffsetHigh = 0;
    WriteOverlap.hEvent = CreateEventA(0, true, true, 0);
    DCB dcb;
    if (GetCommState(com_handle, &dcb)) {
        dcb.BaudRate = baud;
        dcb.Parity = parity;
        dcb.ByteSize = wordlen;
        dcb.StopBits = stopbits - 1;
        dcb.fBinary = 1;
        dcb.fParity = 0;
        dcb.fOutxCtsFlow = 1;
        dcb.fOutxDsrFlow = 0;
        dcb.fDtrControl = 1;
        dcb.fOutX = 0;
        dcb.fInX = 0;
        dcb.fErrorChar = 0;
        dcb.fRtsControl = 2;
        dcb.fAbortOnError = 0;
        if (!SetCommState(com_handle, &dcb)) {
            Debug_Printf("SetCommState returned error %s.\n", 0);
        } else {
            SerialSettings = dcb;
        }
    } else {
        Debug_Printf("GetCommState returned error code %s.\n", 0);
    }
    COMMTIMEOUTS timeouts;
    timeouts.ReadIntervalTimeout = 1000;
    timeouts.ReadTotalTimeoutMultiplier = 0;
    timeouts.ReadTotalTimeoutConstant = 3000;
    timeouts.WriteTotalTimeoutMultiplier = 500;
    timeouts.WriteTotalTimeoutConstant = 1000;
    if (!SetCommTimeouts(com_handle, &timeouts)) {
        Debug_Printf("SetCommTimeouts returned error code %s.\n", 0);
    }
    Set_Serial_DTR(true);
    DWORD errors;
    COMSTAT status;
    if (ClearCommError(PortHandle, &errors, &status)) {
        InQueue = status.cbInQue;
        OutQueue = status.cbOutQue;
        if (errors) {
            Process_Errors(errors, status);
        }
    }
    return com_handle;
}


// bool WinModemClass::Enter_Failsafe_Mode() (0x00734670)
// Drops pending I/O and sets the port to 9600 baud, 8 data bits, no parity,
// one stop bit, no flow control; false when the settings are refused.

bool WinModemClass::Enter_Failsafe_Mode()
{
    DCB dcb;
    DWORD mask;
    DWORD errors;
    COMSTAT status;
    Debug_Printf("WinModemClass - Entering failsafe mode\n");
    PurgeComm(PortHandle, 0x0F);
    ResetEvent(ReadOverlap.hEvent);
    ResetEvent(WriteOverlap.hEvent);
    WaitingForSerialCharRead = false;
    WaitingForSerialCharWrite = false;
    if (GetCommState(PortHandle, &dcb)) {
        dcb.BaudRate = 9600;
        dcb.Parity = 0;
        dcb.ByteSize = 8;
        dcb.StopBits = 0;
        dcb.fBinary = 1;
        dcb.fParity = 0;
        dcb.fOutxCtsFlow = 0;
        dcb.fOutxDsrFlow = 0;
        dcb.fDtrControl = 1;
        dcb.fOutX = 0;
        dcb.fInX = 0;
        dcb.fErrorChar = 0;
        dcb.fRtsControl = 1;
        dcb.fAbortOnError = 0;
        if (!SetCommState(PortHandle, &dcb)) {
            Debug_Printf("SetCommState returned error %s.\n", 0);
            return false;
        }
    } else {
        Debug_Printf("GetCommState returned error %s.\n", 0);
    }
    GetCommMask(PortHandle, &mask);
    Set_Serial_DTR(true);
    Get_Modem_Status();
    if (!EscapeCommFunction(PortHandle, 3)) {
        Debug_Printf("Enter_Failsafe_Mode - EscapeCommFunction failed to set state of RTS line. Error code %d, %s\n",
                     GetLastError(), 0);
    }
    if (ClearCommError(PortHandle, &errors, &status)) {
        InQueue = status.cbInQue;
        OutQueue = status.cbOutQue;
        if (errors) {
            Process_Errors(errors, status);
        }
    }
    return true;
}


// void WinModemClass::Leave_Failsafe_Mode() (0x007347C0)
// Drops pending I/O and restores the settings Serial_Port_Open made.

void WinModemClass::Leave_Failsafe_Mode()
{
    DWORD errors;
    COMSTAT status;
    Debug_Printf("WinModemClass - Leaving failsafe mode\n");
    PurgeComm(PortHandle, 0x0F);
    ResetEvent(ReadOverlap.hEvent);
    ResetEvent(WriteOverlap.hEvent);
    WaitingForSerialCharRead = false;
    WaitingForSerialCharWrite = false;
    if (!SetCommState(PortHandle, &SerialSettings)) {
        Debug_Printf("SetCommState returned error %s.\n", 0);
    }
    if (!EscapeCommFunction(PortHandle, 3)) {
        Debug_Printf("Leave_Failsafe_Mode - EscapeCommFunction failed to set state of RTS line. Error code %d, %s\n",
                     GetLastError(), 0);
    }
    if (ClearCommError(PortHandle, &errors, &status)) {
        InQueue = status.cbInQue;
        OutQueue = status.cbOutQue;
        if (errors) {
            Process_Errors(errors, status);
        }
    }
    Set_Serial_DTR(true);
}


// void WinModemClass::Set_Modem_Dial_Type(int method) (0x007348A0, slot 2).

void WinModemClass::Set_Modem_Dial_Type(int method)
{
    DialingMethod = method;
}


// void WinModemClass::Set_Serial_DTR(bool state) (0x00734900; slot 4)
// Raises (SETDTR) or drops (CLRDTR) the port's DTR line; reports a failure.

void WinModemClass::Set_Serial_DTR(bool state)
{
    if (state) {
        if (EscapeCommFunction(PortHandle, 5)) {   // SETDTR
            return;
        }
    } else {
        if (EscapeCommFunction(PortHandle, 6)) {   // CLRDTR
            return;
        }
    }
    Debug_Printf("Set_Serial_DTR - EscapeCommFunction failed with error code %d, %s\n", GetLastError(), 0);
}


// bool WinModemClass::Read_Serial_Chars() (0x00734980)
// Collects a finished overlapped read, then reads what the port holds (at
// most the buffer, at least one byte) into the circular buffer until a read
// stays pending. True when anything was stored.

bool WinModemClass::Read_Serial_Chars()
{
    DWORD bytes_read;
    int total = 0;
    if (WaitingForSerialCharRead) {
        bytes_read = 0;
        if (GetOverlappedResult(PortHandle, &ReadOverlap, &bytes_read, false)) {
            WaitingForSerialCharRead = false;
            if (bytes_read) {
                for (unsigned index = 0; index < bytes_read; index++) {
                    SerialBuffer[SerialBufferWritePtr] = TempSerialBuffer[index];
                    SerialBufferWritePtr = (SerialBufferWritePtr + 1) & (SIZE_OF_WINDOWS_SERIAL_BUFFER - 1);
                }
                total = bytes_read;
            }
        } else {
            if (GetLastError() == ERROR_IO_INCOMPLETE) {
                return false;
            }
            Debug_Printf("GetOverlappedResult in WinModemClass::Read_Serial_Chars returned error %s\n", 0);
        }
    }
    ResetEvent(ReadOverlap.hEvent);
    DWORD errors;
    COMSTAT status;
    int bytes_to_read = 1;
    if (ClearCommError(PortHandle, &errors, &status)) {
        InQueue = status.cbInQue;
        OutQueue = status.cbOutQue;
        if (errors) {
            Process_Errors(errors, status);
        }
        bytes_to_read = status.cbInQue;
        if (bytes_to_read >= SIZE_OF_WINDOWS_SERIAL_BUFFER) {
            bytes_to_read = SIZE_OF_WINDOWS_SERIAL_BUFFER;
        } else if (bytes_to_read == 0) {
            bytes_to_read = 1;
        }
    }
    bool read_result;
    while (true) {
        bytes_read = 0;
        read_result = ReadFile(PortHandle, TempSerialBuffer, bytes_to_read, &bytes_read, &ReadOverlap) != 0;
        if (!read_result) {
            if (GetLastError() == ERROR_IO_PENDING) {
                WaitingForSerialCharRead = true;
                return total != 0;
            }
            Debug_Printf("Reading from port handle failed with error %s\n", 0);
            break;
        }
        if (bytes_read) {
            for (unsigned index = 0; index < bytes_read; index++) {
                SerialBuffer[SerialBufferWritePtr] = TempSerialBuffer[index];
                SerialBufferWritePtr = (SerialBufferWritePtr + 1) & (SIZE_OF_WINDOWS_SERIAL_BUFFER - 1);
            }
            total += bytes_read;
        }
        bytes_to_read = 1;
        if (read_result != true) {
            break;
        }
    }
    return total != 0;
}


// int WinModemClass::Read_From_Serial_Port(unsigned char *dest_ptr, int buffer_len) (0x00734B30)
// Copies at most buffer_len received bytes out of the circular buffer, echoing
// every byte but carriage returns; returns the count copied (0 when nothing arrived).

int WinModemClass::Read_From_Serial_Port(unsigned char *dest_ptr, int buffer_len)
{
    int bytes_to_copy = 0;
    if (Read_Serial_Chars()) {
        bytes_to_copy = SerialBufferWritePtr - SerialBufferReadPtr;
        if (bytes_to_copy < 0) {
            bytes_to_copy += SIZE_OF_WINDOWS_SERIAL_BUFFER;
        }
        if (bytes_to_copy > buffer_len) {
            bytes_to_copy = buffer_len;
        }
        for (int index = 0; index < bytes_to_copy; index++) {
            if (EchoFunction && SerialBuffer[SerialBufferReadPtr] != '\r') {
                EchoFunction(SerialBuffer[SerialBufferReadPtr]);
            }
            *dest_ptr++ = SerialBuffer[SerialBufferReadPtr];
            SerialBufferReadPtr = (SerialBufferReadPtr + 1) & (SIZE_OF_WINDOWS_SERIAL_BUFFER - 1);
        }
    }
    return bytes_to_copy;
}


// void WinModemClass::Wait_For_Serial_Write() (0x00734BC0)
// Waits up to 300 ticks for the pending overlapped write to finish. Out of line at
// all six call sites in the unit (Write_To_Serial_Port and its expansions).

#pragma auto_inline(off)
void WinModemClass::Wait_For_Serial_Write()
{
    DWORD bytes_written;
    CDTimerClass<SystemTimerClass> timer;
    if (WaitingForSerialCharWrite) {
        timer = 300;
        while (!GetOverlappedResult(PortHandle, &WriteOverlap, &bytes_written, false)
               && GetLastError() == ERROR_IO_INCOMPLETE && timer) {
        }
        WaitingForSerialCharWrite = false;
    }
    ResetEvent(WriteOverlap.hEvent);
}
#pragma auto_inline(on)


// void WinModemClass::Write_To_Serial_Port(unsigned char *buffer, int length) (0x00734C70)
// Starts an overlapped write after the previous one finished.

void WinModemClass::Write_To_Serial_Port(unsigned char *buffer, int length)
{
    DWORD bytes_written;
    Wait_For_Serial_Write();
    if (!WriteFile(PortHandle, buffer, length, &bytes_written, &WriteOverlap)) {
        if (GetLastError() == ERROR_IO_PENDING) {
            WaitingForSerialCharWrite = true;
            return;
        }
        Debug_Printf("Writing to port handle failed with error %s\n", 0);
    }
}


// int WinModemClass::Get_Modem_Result(int delay, char *buffer, int buffer_len, int timeout)
// (0x00734CD0; slot 5)
// Collects the modem reply into buffer until delay runs out or a line ends and the
// timeout passes, drops echoed "AT" lines and leading line breaks; returns the time
// left of delay, 0 when the application lost focus, or the abort function's result.
extern "C" char *__cdecl strchr(char const *string, int character);
extern "C" char *__cdecl strrchr(char const *string, int character);
extern "C" char *__cdecl strstr(char const *string, char const *search);

int WinModemClass::Get_Modem_Result(int delay, char *buffer, int buffer_len, int timeout)
{
    CDTimerClass<SystemTimerClass> timer;
    CDTimerClass<SystemTimerClass> cr_timer;
    int pos = 0;
    char *cr_ptr;

    memset(buffer, 0, buffer_len);
    cr_timer = timeout;
    do {
        timer = delay;
        for (;;) {
            if (AbortFunction) {
                int abort = AbortFunction();
                if (abort) {
                    return abort;
                }
            }
            if (!unknown_flag_a40d28) {
                return 0;
            }
            int bytes = Read_From_Serial_Port((unsigned char *)buffer + pos, (buffer_len - pos < 0 ? 0 : buffer_len - pos));
            if (bytes) {
                pos += bytes;
                cr_timer = timeout;
            }
            if (!timer || pos >= buffer_len) {
                break;
            }
            if (strchr(buffer, '\r')) {
                if (timeout == -1 || !cr_timer) {
                    break;
                }
            }
        }

        buffer[buffer_len - 1] = '\0';
        cr_ptr = strstr(buffer, "AT");
        if (cr_ptr) {
            if (buffer[0] == 'A' && buffer[1] == 'T' && strchr(buffer, '\r')) {
                cr_ptr = strchr(buffer, '\r');
            }
        }
        if (!cr_ptr) {
            if (buffer[0] != '\r' && buffer[0] != '\n') {
                break;
            }
            cr_ptr = strchr(buffer, '\r');
            char *lf_ptr = strchr(buffer, '\n');
            if (!cr_ptr || (lf_ptr && lf_ptr < cr_ptr)) {
                cr_ptr = strchr(buffer, '\n');
            }
            if (!cr_ptr) {
                break;
            }
        }
        while (*cr_ptr == '\r' || *cr_ptr == '\n') {
            cr_ptr++;
        }
        if (cr_ptr != buffer) {
            int skip = cr_ptr - buffer;
            memmove(buffer, cr_ptr, buffer_len - skip);
            pos -= skip;
        }
    } while (cr_ptr);

    if (strrchr(buffer, '\r')) {
        *strrchr(buffer, '\r') = '\0';
    }
    return timer;
}


// void WinModemClass::Dial_Modem(char const *dial_number) (0x00734F50; slot 6)
// Sends "ATDT<number>\r" (ATDP for pulse dialling) and waits for the write.

void WinModemClass::Dial_Modem(char const *dial_number)
{
    char dial_string[80];
    Debug_Printf("Creating dial string\n");
    strcpy(dial_string, "ATD");
    if (DialingMethod == 0) {
        strcat(dial_string, "T");
        Debug_Printf("Dial type is tone\n");
    } else {
        Debug_Printf("Dial type is pulse\n");
        strcat(dial_string, "P");
    }
    Debug_Printf("Appending dial string\n");
    strcat(dial_string, dial_number);
    Debug_Printf("Appending carriage return\n");
    strcat(dial_string, "\r");
    Debug_Printf("Send to modem: %s\n", dial_string);
    Write_To_Serial_Port((unsigned char *)dial_string, strlen(dial_string));
    Wait_For_Serial_Write();
}


// int WinModemClass::Send_Command_To_Modem(char const *command, char terminator, char *buffer,
//     int buffer_len, int delay, int retries, int timeout) (0x00735110; slot 7)
// Sends command plus terminator up to retries times and reads the reply: 3 for "0",
// 2 for "OK" or a three digit code (copied to buffer), 1 for "ERROR", 0 on timeout.
extern "C" char *__cdecl strstr(char const *string, char const *search);
extern "C" char *__cdecl strncpy(char *destination, char const *source, unsigned int count);

int WinModemClass::Send_Command_To_Modem(char const *command, char terminator, char *buffer, int buffer_len,
                                         int delay, int retries, int timeout)
{
    char term_string[2];
    char send_string[80];
    char flush_buffer[80];

    term_string[0] = terminator;
    term_string[1] = '\0';
    strcpy(send_string, command);
    strcat(send_string, term_string);

    Read_From_Serial_Port((unsigned char *)flush_buffer, 80);
    Sleep(100);
    Read_From_Serial_Port((unsigned char *)flush_buffer, 80);

    for (int times = 0; times < retries; times++) {
        CDTimerClass<SystemTimerClass> timer(delay);
        Write_To_Serial_Port((unsigned char *)send_string, strlen(send_string));
        Wait_For_Serial_Write();
        if (timer.Value() <= 0) {
            return 0;
        }
        int wait = timer.Value() < 1 ? 1 : timer.Value();
        char reply[80];
        memset(reply, 0, 80);
        Get_Modem_Result(wait, reply, 80, timeout);
        if (strcmp(reply, "0") == 0) {
            return 3;
        }
        if (strstr(reply, "OK")) {
            return 2;
        }
        if (strstr(reply, "ERROR")) {
            return 1;
        }
        if (strlen(reply) == 3 && reply[0] >= '0' && reply[0] <= '9' && reply[1] >= '0' && reply[1] <= '9'
            && reply[2] >= '0' && reply[2] <= '9') {
            strncpy(buffer, reply, buffer_len < 80 ? buffer_len : 80);
            return 2;
        }
        strncpy(buffer, reply, buffer_len < 80 ? buffer_len : 80);
        Sleep(100);
        Write_To_Serial_Port((unsigned char *)"\r", 1);
        Wait_For_Serial_Write();
        Sleep(100);
    }
    return 0;
}


// void *WinModemClass::`scalar deleting destructor'(unsigned int flags) (0x00735610, slot 0)
// Compiler-generated with the vtable that the constructor's definition in this
// unit emits; the destructor body is inlined. No source of its own.
