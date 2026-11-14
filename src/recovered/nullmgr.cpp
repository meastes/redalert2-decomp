// nullmgr.cpp: the original translation unit re-filed as one source (Route A): the accepted
// bodies of its canonical compile group in address order, under the unit's own header
// state (its switches and includes in their original order). The module map
// manifests/original_target/modules/nullmgr.json carries the addresses and the rows that
// stay in their own files.


#include "shared/shape_file.hpp"
#include "shared/unknown_geometry.hpp"
#include "shared/null_modem.hpp"
#include "shared/crt_string.hpp"
#include "shared/debug_printf.hpp"
#define VECTOR_IMPLEMENT_VECTOR_ACCESSORS
#include "shared/ms_message_box.hpp"
#include "shared/ww_message_box.hpp"
#include "shared/string_table.hpp"
#include "shared/session_state.hpp"
#include "shared/vector_class.hpp"
#include "shared/crt_tokens.hpp"
#include "shared/campaign.hpp"
#include "shared/serial_progress.hpp"


// Header-static dynamic initializers of the unit at 0x005CFDD0: the shape_file.hpp empty rect and the
// shared/unknown_geometry.hpp constants (0x005CFDD0-0x005CFFB1); the other
// drafts of this plan are emitted by these includes.


// Dynamic initializer (0x005CFDF0, _$E) of diagonal from the header(s) included
// by HeaderStatics_empty_rect_init_005cfdd0.cpp; no source of its own.


// Dynamic initializer (0x005CFE20, _$E) of b from the header(s) included
// by HeaderStatics_empty_rect_init_005cfdd0.cpp; no source of its own.


// Dynamic initializer (0x005CFE40, _$E) of c from the header(s) included
// by HeaderStatics_empty_rect_init_005cfdd0.cpp; no source of its own.


// Dynamic initializer (0x005CFE60, _$E) of angle45 from the header(s) included
// by HeaderStatics_empty_rect_init_005cfdd0.cpp; no source of its own.


// Dynamic initializer (0x005CFE80, _$E) of angle60 from the header(s) included
// by HeaderStatics_empty_rect_init_005cfdd0.cpp; no source of its own.


// Dynamic initializer (0x005CFEA0, _$E) of angle90 from the header(s) included
// by HeaderStatics_empty_rect_init_005cfdd0.cpp; no source of its own.


// Dynamic initializer (0x005CFEC0, _$E) of g from the header(s) included
// by HeaderStatics_empty_rect_init_005cfdd0.cpp; no source of its own.


// Dynamic initializer (0x005CFF00, _$E) of h from the header(s) included
// by HeaderStatics_empty_rect_init_005cfdd0.cpp; no source of its own.


// Dynamic initializer (0x005CFF30, _$E) of i from the header(s) included
// by HeaderStatics_empty_rect_init_005cfdd0.cpp; no source of its own.


// Dynamic initializer (0x005CFF60, _$E) of j from the header(s) included
// by HeaderStatics_empty_rect_init_005cfdd0.cpp; no source of its own.


// Dynamic initializer (0x005CFF90, _$E) of k from the header(s) included
// by HeaderStatics_empty_rect_init_005cfdd0.cpp; no source of its own.


// Dynamic initializer (0x005CFFA0, _$E) of l from the header(s) included
// by HeaderStatics_empty_rect_init_005cfdd0.cpp; no source of its own.


// NullModemClass::NullModemClass(int numsend, int numreceive, int maxlen, unsigned short magicnum) (0x005CFFC0)

// The modem command results by status, for the debug log (0x007E78F0; the first
// data of the unit, before Init's com_ids).
char ModemCmdNames[4][20] = {
    "MODEM_CMD_TIMEOUT",
    "MODEM_CMD_ERROR",
    "MODEM_CMD_OK",
    "MODEM_CMD_0"
};

NullModemClass::NullModemClass(int numsend, int numreceive, int maxlen, unsigned short magicnum) : ConnManClass()
{
    PortHandle = -1;
    Connection = 0;
    NumSend = numsend;
    NumReceive = numreceive;
    MaxLen = maxlen;
    MagicNum = magicnum;
    RXBuf = 0;
    BuildBuf = 0;
    ReceiveLen = 500;
    ReceiveBuf = 0;
    unknown_18 = -1;
    unknown_1c = 0;
    unknown_20 = 0;
    unknown_24 = 3000;
    unknown_28 = 1200;
    unknown_2c = 60;
    unknown_30 = '+';
    SendOverflows = 0;
    ReceiveOverflows = 0;
    CRCErrors = 0;
    NumConnections = 0;
    RetryDelta = 60;
    MaxRetries = -1;
    Timeout = 1200;
    ModemEntry = 0;
}


// NullModemClass::~NullModemClass() (0x005D0050)
// Releases the modem entry, the connection and the serial port.

NullModemClass::~NullModemClass()
{
    if (ModemEntry) {
        delete ModemEntry;
    }
    Delete_Connection();
    Shutdown();
    if (SerialPort) {
        delete SerialPort;
        SerialPort = 0;
    }
}


// int NullModemClass::Init(int port, int irq, char *dev_name, int baud, char parity, int wordlen, int stopbits) (0x005D00E0)
// Creates the connection and its buffers once, then opens the port (COM1-COM4
// by address, or the registry modem named dev_name): 1 when it opened.

int NullModemClass::Init(int port, int irq, char *dev_name, int baud, char parity, int wordlen, int stopbits)
{
    int i;
    static char com_ids[9][5] = {
        "COM1", "COM2", "COM3", "COM4", "COM5", "COM6", "COM7", "COM8", "COM9"
    };
    char *device;

    Debug_Printf("NullModemClass::Init\n");
    if (SerialPort) {
        PortHandle = (int)SerialPort->Get_Port_Handle();
    } else {
        PortHandle = -1;
    }

    if (!Connection) {
        Debug_Printf("Creating connection class\n");
        Connection = new NullModemConnClass(NumSend, NumReceive, MaxLen, MagicNum);
        Connection->RetryDelta = RetryDelta;
        Connection->MaxRetries = MaxRetries;
        Connection->Timeout = Timeout;
        RXSize = Connection->Actual_Max_Packet() * NumReceive;
        RXBuf = new char[RXSize];
        BuildBuf = new char[MaxLen];
        ReceiveBuf = new char[ReceiveLen];
    } else {
        Debug_Printf("Connection already exists\n");
    }

    RXCount = 0;
    ReceiveIndex = 0;

    if (!SerialPort) {
        SerialPort = new WinModemClass;
    }

    switch (port) {
        case 0x3F8:
            device = com_ids[0];
            break;
        case 0x2F8:
            device = com_ids[1];
            break;
        case 0x3E8:
            device = com_ids[2];
            break;
        case 0x2E8:
            device = com_ids[3];
            break;
        case 1:
            device = dev_name;
            if (ModemEntry) {
                delete ModemEntry;
                ModemEntry = 0;
            }
            for (i = 0; i < 10; i++) {
                ModemEntry = new ModemRegistryEntryClass(i);
                if (ModemEntry->ModemName && !strcmp(dev_name, ModemEntry->ModemName)) {
                    device = ModemEntry->ModemDeviceName;
                    break;
                }
                delete ModemEntry;
                ModemEntry = 0;
            }
            break;
        default:
            device = 0;
            break;
    }

    PortHandle = (int)SerialPort->Serial_Port_Open(device, baud, parity, wordlen, stopbits);
    if (PortHandle == -1) {
        return 0;
    }

    Debug_Printf("Ready to initialise connection\n");
    Connection->Init(PortHandle);
    NumConnections = 1;
    return 1;
}


// int NullModemClass::Num_Connections() (0x005D0350)

int NullModemClass::Num_Connections()
{
    return NumConnections;
}


// int NullModemClass::Delete_Connection() (0x005D0360)
// Deletes the connection and the buffers.

int NullModemClass::Delete_Connection()
{
    if (Connection) {
        delete Connection;
        Connection = 0;
    }
    if (RXBuf) {
        delete RXBuf;
        RXBuf = 0;
    }
    if (BuildBuf) {
        delete BuildBuf;
        BuildBuf = 0;
    }
    if (ReceiveBuf) {
        delete ReceiveBuf;
        ReceiveBuf = 0;
    }
    NumConnections = 0;
    return 1;
}


// int NullModemClass::Init_Send_Queue() (0x005D03C0)

int NullModemClass::Init_Send_Queue()
{
    if (Connection) {
        Connection->Queue->Init_Send_Queue();
    }
    return 1;
}


// int NullModemClass::Detect_Port(SerialSettingsType *settings) (0x005D03E0)
// Opens the settings' port (COM1-COM4 by address, or the registry modem
// named in the settings) to see that it works, and closes it again: 0 when
// it opened, 1 otherwise.

int NullModemClass::Detect_Port(SerialSettingsType *settings)
{
    if (!SerialPort) {
        SerialPort = new WinModemClass;
    } else {
        SerialPort->Serial_Port_Close();
    }

    int baud = settings->Baud;
    char const *com;
    switch (settings->Port) {
        case 0x3F8:
            com = "COM1";
            break;
        case 0x2F8:
            com = "COM2";
            break;
        case 0x3E8:
            com = "COM3";
            break;
        case 0x2E8:
            com = "COM4";
            break;
        case 1: {
            com = settings->ModemName;
            if (ModemEntry) {
                delete ModemEntry;
                ModemEntry = 0;
            }
            for (int i = 0; i < 10; i++) {
                ModemEntry = new ModemRegistryEntryClass(i);
                if (ModemEntry->ModemName && !strcmp(settings->ModemName, ModemEntry->ModemName)) {
                    com = ModemEntry->ModemDeviceName;
                    break;
                }
                delete ModemEntry;
                ModemEntry = 0;
            }
            break;
        }
        default:
            return 1;
    }

    if (SerialPort->Serial_Port_Open(com, baud, 0, 8, 1) == INVALID_HANDLE_VALUE) {
        return 1;
    }
    SerialPort->Serial_Port_Close();
    return 0;
}


// void NullModemClass::Shutdown() (0x005D0570)
// Closes and deletes the serial port, then the connection.

void NullModemClass::Shutdown()
{
    if (PortHandle != -1 && SerialPort) {
        SerialPort->Serial_Port_Close();
        delete SerialPort;
        SerialPort = 0;
        PortHandle = -1;
        Delete_Connection();
    }
}


// void NullModemClass::Set_Timing(unsigned long retrydelta, unsigned long maxretries, unsigned long timeout, bool global) (0x005D05C0, slot 13)

void NullModemClass::Set_Timing(unsigned long retrydelta, unsigned long maxretries, unsigned long timeout, bool global)
{
    RetryDelta = retrydelta;
    MaxRetries = maxretries;
    Timeout = timeout;
    Connection->RetryDelta = RetryDelta;
    Connection->MaxRetries = MaxRetries;
    Connection->Timeout = Timeout;
}


// int NullModemClass::Send_Message(void *buf, int buflen, int ack_req) (0x005D05F0)

int NullModemClass::Send_Message(void *buf, int buflen, int ack_req)
{
    if (!NumConnections) {
        return 0;
    }
    int result = Connection->Send_Packet(buf, buflen, ack_req, 0);
    if (!result) {
        SendOverflows++;
    }
    return result;
}


// int NullModemClass::Get_Message(void *buf, int *buflen) (0x005D0630)

int NullModemClass::Get_Message(void *buf, int *buflen)
{
    if (!NumConnections) {
        return 0;
    }
    return Connection->Get_Packet(buf, buflen);
}


// int NullModemClass::Service() (0x005D0660, slot 1)
// Reads what the port has received into RXBuf, hands one complete frame with
// a good CRC to the connection, drops bytes up to the next start code, and
// services the connection's queues.

int NullModemClass::Service()
{
    int pos;
    int i;
    unsigned short length;
    SerialHeaderType *header;
    SerialCRCType *crc;

    if (NumConnections == 0) {
        return 0;
    }

    RXCount += SerialPort->Read_From_Serial_Port((unsigned char *)(RXBuf + RXCount), RXSize - RXCount);

    if (RXCount < PACKET_SERIAL_OVERHEAD_SIZE + 1) {
        return Connection->Service();
    }

    pos = -1;
    for (i = 0; i <= RXCount - 2; i++) {
        if (*((unsigned short *)(RXBuf + i)) == 0xDABD) {
            pos = i;
            break;
        }
    }

    if (pos == -1) {
        memmove(RXBuf, RXBuf + i, sizeof(short) - 1);
        RXCount = sizeof(short) - 1;
        return Connection->Service();
    }

    if ((RXCount - pos) < sizeof(SerialHeaderType)) {
        memmove(RXBuf, RXBuf + pos, RXCount - pos);
        RXCount -= pos;
        return Connection->Service();
    }

    header = (SerialHeaderType *)(RXBuf + pos);
    if (header->MagicNumber2 != 0xDEAF) {
        pos += sizeof(short);
        memmove(RXBuf, RXBuf + pos, RXCount - pos);
        RXCount -= pos;
        return Connection->Service();
    }

    length = header->Length;
    if (length > MaxLen) {
        Debug_Printf("Serial packet length too long by %d bytes\n", length - MaxLen);
        pos += sizeof(short);
        memmove(RXBuf, RXBuf + pos, RXCount - pos);
        RXCount -= pos;
        return Connection->Service();
    }

    if ((pos + length + PACKET_SERIAL_OVERHEAD_SIZE) > RXCount) {
        if (pos) {
            memmove(RXBuf, RXBuf + pos, RXCount - pos);
            RXCount -= pos;
        }
        return Connection->Service();
    }

    crc = (SerialCRCType *)(RXBuf + pos + sizeof(SerialHeaderType) + length);
    if (NullModemConnClass::Compute_CRC(RXBuf + pos + sizeof(SerialHeaderType), length) != crc->SerialCRC) {
        CRCErrors++;
        Debug_Printf("Serial packet failed CRC check\n");
        pos += sizeof(short);
        memmove(RXBuf, RXBuf + pos, RXCount - pos);
        RXCount -= pos;
        return Connection->Service();
    }

    if (!Connection->Receive_Packet(RXBuf + pos + sizeof(SerialHeaderType), length)) {
        ReceiveOverflows++;
    }

    pos += (PACKET_SERIAL_OVERHEAD_SIZE + length);
    memmove(RXBuf, RXBuf + pos, RXCount - pos);
    RXCount -= pos;
    return Connection->Service();
}


// int NullModemClass::Num_Send() (0x005D08A0)

int NullModemClass::Num_Send()
{
    if (Connection) {
        return Connection->Queue->SendCount;
    }
    return 0;
}


// int NullModemClass::Num_Receive() (0x005D08C0)

int NullModemClass::Num_Receive()
{
    if (Connection) {
        return Connection->Queue->ReceiveCount;
    }
    return 0;
}


// unsigned long NullModemClass::Response_Time() (0x005D08E0, slot 12)

unsigned long NullModemClass::Response_Time()
{
    if (Connection) {
        return Connection->Queue->Avg_Delay();
    }
    return 0;
}


// void NullModemClass::Reset_Response_Time(bool clear) (0x005D0900, slot 11)

void NullModemClass::Reset_Response_Time(bool clear)
{
    if (Connection) {
        Connection->Queue->Reset_Delay(true);
    }
}


// void *NullModemClass::Oldest_Send() (0x005D0920)
// The buffer of the oldest unacknowledged data packet that needs an ACK.

void *NullModemClass::Oldest_Send()
{
    void *buf = 0;
    for (int i = 0; i < Connection->Queue->SendCount; i++) {
        SendQueueType *send = Connection->Queue->Get_Send(i);
        if (send && ((CommHeaderType *)send->Buffer)->Code == 0 && !send->IsACK) {
            buf = send->Buffer;
            break;
        }
    }
    return buf;
}


// void NullModemClass::Configure_Debug(int index, int offset, int size, int names, int namestart, int namecount) (0x005D0970, slot 15)

void NullModemClass::Configure_Debug(int index, int offset, int size, int names, int namestart, int namecount)
{
    if (Connection) {
        Connection->Queue->Configure_Debug(offset, size, names, namestart, namecount);
    }
}


// bool NullModemClass::Detect_Modem(SerialSettingsType *settings, bool reconnect) (0x005D09A0)
// Initialises the modem in failsafe mode under an "initializing" dialog: echo,
// result codes and verbose results on, AT, the session's init string (each
// '|' part), the registry modem's flow control, compression and error
// correction strings, auto-answer off; three errors are too many. Then checks
// AT again at the normal settings.

bool NullModemClass::Detect_Modem(SerialSettingsType *settings, bool reconnect)
{
    int status;
    int error_count = 0;
    char buffer[240];

    Debug_Printf("Detecting modem\n");
    if (SerialPort->Enter_Failsafe_Mode()) {
        HWND dialog = unknown_cancel_dialog_6001e0(StringTable::Load_String("TXT_INITIALIZING_MODEM", 0, "D:\\Ra2\\NullMgr.CPP", 0x46E), 0, 0);
        if (dialog) {
            unknown_dialog_5ff7c0(dialog);

            Debug_Printf("Sending echo on command\n");
            Debug_Printf("Command response was %s\n", ModemCmdNames[SerialPort->Send_Command_To_Modem("ATE1", '\r', buffer, 81, 90, 2, -1)]);
            Debug_Printf("Sending result codes on command\n");
            Debug_Printf("Command response was %s\n", ModemCmdNames[SerialPort->Send_Command_To_Modem("ATQ0", '\r', buffer, 81, 90, 2, -1)]);
            Debug_Printf("Sending verbose result codes command\n");
            Debug_Printf("Command response was %s\n", ModemCmdNames[SerialPort->Send_Command_To_Modem("ATV1", '\r', buffer, 81, 90, 2, -1)]);
            unknown_1c = 1;
            unknown_20 = 1;

            status = SerialPort->Send_Command_To_Modem("AT", '\r', buffer, 81, 180, 2, -1);
            if (status < 2) {
                Debug_Printf("Modem failed to respond to 'AT' command\n");
            } else {
            if (settings->InitStringIndex == -1) {
                status = SerialPort->Send_Command_To_Modem("", '\r', buffer, 81, 300, 1, -1);
            } else {
                Debug_Printf("Sending init strings\n");
                char *istr = new char[2 + strlen(unknown_session_a3d298.InitStrings[settings->InitStringIndex])];
                strcpy(istr, unknown_session_a3d298.InitStrings[settings->InitStringIndex]);
                char *tokenptr = strtok(istr, "|");
                while (tokenptr) {
                    Debug_Printf("Sending %s\n", tokenptr);
                    status = SerialPort->Send_Command_To_Modem(tokenptr, '\r', buffer, 81, 90, 1, -1);
                    Debug_Printf("Command response was %s\n", ModemCmdNames[status]);
                    if (status < 2) {
                        if (WWMessageBox(L" ").Process(StringTable::Load_String("TXT_ERROR_NO_INIT", 0, "D:\\Ra2\\NullMgr.CPP", 0x4BF),
                                StringTable::Load_String("TXT_IGNORE", 0, "D:\\Ra2\\NullMgr.CPP", 0x4BF),
                                StringTable::Load_String("TXT_CANCEL", 0, "D:\\Ra2\\NullMgr.CPP", 0x4BF), 0, false)) {
                            delete istr;
                            unknown_dialog_5ff6e0(dialog);
                            SerialPort->Leave_Failsafe_Mode();
                            return false;
                        }
                        error_count++;
                        break;
                    }
                    tokenptr = strtok(0, "|");
                }
                delete istr;
            }

            if (settings->Port == 1 && ModemEntry) {
                char send_string[256] = "AT";
                if (ModemEntry->HardwareFlowControl) {
                    Debug_Printf("Sending flow control init\n");
                    strcpy(&send_string[2], ModemEntry->HardwareFlowControl);
                    status = SerialPort->Send_Command_To_Modem(send_string, '\r', buffer, 81, 60, 1, -1);
                    Debug_Printf("Command response was %s\n", ModemCmdNames[status]);
                    if (status != 2 && status != 3) {
                        if (WWMessageBox(L" ").Process(StringTable::Load_String("TXT_NO_FLOW_CONTROL_RESPONSE", 0, "D:\\Ra2\\NullMgr.CPP", 0x4DF),
                                StringTable::Load_String("TXT_IGNORE", 0, "D:\\Ra2\\NullMgr.CPP", 0x4DF),
                                StringTable::Load_String("TXT_CANCEL", 0, "D:\\Ra2\\NullMgr.CPP", 0x4DF), 0, false)) {
                            unknown_dialog_5ff6e0(dialog);
                            SerialPort->Leave_Failsafe_Mode();
                            return false;
                        }
                    }
                }
                if (settings->Compression) {
                    if (ModemEntry->CompressionEnable) {
                        Debug_Printf("Sending data compression enable init\n");
                        strcpy(&send_string[2], ModemEntry->CompressionEnable);
                        SerialPort->Send_Command_To_Modem(send_string, '\r', buffer, 81, 60, 1, -1);
                        Debug_Printf("Command response was %s\n", ModemCmdNames[status]);
                        if (status != 2 && status != 3) {
                            if (WWMessageBox(L" ").Process(StringTable::Load_String("TXT_NO_COMP_ENABLE_RESPONSE", 0, "D:\\Ra2\\NullMgr.CPP", 0x4F2),
                                    StringTable::Load_String("TXT_IGNORE", 0, "D:\\Ra2\\NullMgr.CPP", 0x4F2),
                                    StringTable::Load_String("TXT_CANCEL", 0, "D:\\Ra2\\NullMgr.CPP", 0x4F2), 0, false)) {
                                unknown_dialog_5ff6e0(dialog);
                                SerialPort->Leave_Failsafe_Mode();
                                return false;
                            }
                        }
                    }
                } else {
                    if (ModemEntry->CompressionDisable) {
                        Debug_Printf("Sending data compression disable init\n");
                        strcpy(&send_string[2], ModemEntry->CompressionDisable);
                        SerialPort->Send_Command_To_Modem(send_string, '\r', buffer, 81, 60, 1, -1);
                        Debug_Printf("Command response was %s\n", ModemCmdNames[status]);
                        if (status != 2 && status != 3) {
                            if (WWMessageBox(L" ").Process(StringTable::Load_String("TXT_NO_COMP_DISABLE_RESPONSE", 0, "D:\\Ra2\\NullMgr.CPP", 0x501),
                                    StringTable::Load_String("TXT_IGNORE", 0, "D:\\Ra2\\NullMgr.CPP", 0x501),
                                    StringTable::Load_String("TXT_CANCEL", 0, "D:\\Ra2\\NullMgr.CPP", 0x501), 0, false)) {
                                unknown_dialog_5ff6e0(dialog);
                                SerialPort->Leave_Failsafe_Mode();
                                return false;
                            }
                        }
                    }
                }
                if (settings->ErrorCorrection) {
                    if (ModemEntry->ErrorCorrectionEnable) {
                        Debug_Printf("Sending error correction enable init\n");
                        strcpy(&send_string[2], ModemEntry->ErrorCorrectionEnable);
                        SerialPort->Send_Command_To_Modem(send_string, '\r', buffer, 81, 60, 1, -1);
                        Debug_Printf("Command response was %s\n", ModemCmdNames[status]);
                        if (status != 2 && status != 3) {
                            if (WWMessageBox(L" ").Process(StringTable::Load_String("TXT_NO_EC_ENABLE_RESPONSE", 0, "D:\\Ra2\\NullMgr.CPP", 0x515),
                                    StringTable::Load_String("TXT_IGNORE", 0, "D:\\Ra2\\NullMgr.CPP", 0x515),
                                    StringTable::Load_String("TXT_CANCEL", 0, "D:\\Ra2\\NullMgr.CPP", 0x515), 0, false)) {
                                unknown_dialog_5ff6e0(dialog);
                                SerialPort->Leave_Failsafe_Mode();
                                return false;
                            }
                        }
                    }
                } else {
                    if (ModemEntry->ErrorCorrectionDisable) {
                        Debug_Printf("Sending error correction disable init\n");
                        strcpy(&send_string[2], ModemEntry->ErrorCorrectionDisable);
                        SerialPort->Send_Command_To_Modem(send_string, '\r', buffer, 81, 60, 1, -1);
                        Debug_Printf("Command response was %s\n", ModemCmdNames[status]);
                        if (status != 2 && status != 3) {
                            if (WWMessageBox(L" ").Process(StringTable::Load_String("TXT_NO_EC_DISABLE_RESPONSE", 0, "D:\\Ra2\\NullMgr.CPP", 0x524),
                                    StringTable::Load_String("TXT_IGNORE", 0, "D:\\Ra2\\NullMgr.CPP", 0x524),
                                    StringTable::Load_String("TXT_CANCEL", 0, "D:\\Ra2\\NullMgr.CPP", 0x524), 0, false)) {
                                unknown_dialog_5ff6e0(dialog);
                                SerialPort->Leave_Failsafe_Mode();
                                return false;
                            }
                        }
                    }
                }
            }

            Debug_Printf("Sending disable auto-answer\n");
            status = SerialPort->Send_Command_To_Modem("ATS0=0", '\r', buffer, 81, 90, 2, -1);
            Debug_Printf("Command response was %s\n", ModemCmdNames[status]);
            if (status != 2) {
                if (WWMessageBox(L" ").Process(StringTable::Load_String("TXT_ERROR_NO_DISABLE", 0, "D:\\Ra2\\NullMgr.CPP", 0x536),
                        StringTable::Load_String("TXT_IGNORE", 0, "D:\\Ra2\\NullMgr.CPP", 0x536),
                        StringTable::Load_String("TXT_CANCEL", 0, "D:\\Ra2\\NullMgr.CPP", 0x536), 0, false)) {
                    unknown_dialog_5ff6e0(dialog);
                    SerialPort->Leave_Failsafe_Mode();
                    return false;
                }
                error_count++;
            }

            if (error_count >= 3) {
                WWMessageBox(L" ").Process(StringTable::Load_String("TXT_ERROR_TOO_MANY", 0, "D:\\Ra2\\NullMgr.CPP", 0x543),
                    StringTable::Load_String("TXT_OK", 0, "D:\\Ra2\\NullMgr.CPP", 0x543), 0, 0, false);
                unknown_dialog_5ff6e0(dialog);
                SerialPort->Leave_Failsafe_Mode();
                return false;
            }

            unknown_dialog_5ff6e0(dialog);
            Debug_Printf("Modem detected OK\n");
            SerialPort->Leave_Failsafe_Mode();
            status = SerialPort->Send_Command_To_Modem("AT", '\r', buffer, 81, 90, 2, -1);
            if (status < 2) {
                Debug_Printf("Modem failed to respond to 'AT' command\n");
                return false;
            }
            return true;
            }
            unknown_dialog_5ff6e0(dialog);
        }
        SerialPort->Leave_Failsafe_Mode();
    }
    return false;
}


// DialStatusType NullModemClass::Dial_Modem(char *string, int method, bool reconnect) (0x005D1350)
// Dials under a cancel dialog and waits for the modem's answer: CONNECT
// (copied to ModemRXString), BUSY, NO CARRIER, NO DIALTONE or ERROR; the
// wait ends on a timeout, the cancel button or ESC (the modem gets a CR).
extern "C" char *__cdecl strstr(char const *string, char const *search);
extern "C" char *__cdecl strncpy(char *destination, char const *source, unsigned int count);

DialStatusType NullModemClass::Dial_Modem(char *string, int method, bool reconnect)
{
    bool process = true;
    int delay;
    DialStatusType dialstatus = DIAL_ERROR;
    char buffer[240];
    wchar_t const *text;

    Debug_Printf("Dial_Modem\n");
    if (reconnect == true) {
        text = StringTable::Load_String("TXT_MODEM_CONNERR_REDIALING", 0, "D:\\Ra2\\NullMgr.CPP", 0x581);
    } else {
        text = StringTable::Load_String("TXT_DIALING", 0, "D:\\Ra2\\NullMgr.CPP", 0x583);
    }
    unknown_modem_aborted_a72ec4 = false;
    HWND dialog = unknown_cancel_dialog_6001e0(text, StringTable::Load_String("TXT_CANCEL", 0, "D:\\Ra2\\NullMgr.CPP", 0x587),
                                               &unknown_modem_aborted_a72ec4);
    if (dialog) {
        unknown_dialog_5ff7c0(dialog);

        Debug_Printf("Setting modem dial type\n");
        SerialPort->Set_Modem_Dial_Type(method);
        Debug_Printf("Clearing results buffer\n");
        SerialPort->Get_Modem_Result(60, buffer, 81, -1);
        Debug_Printf("Calling dial function\n");
        SerialPort->Dial_Modem(string);
        SerialPort->Set_Abort_Function(Abort_Modem);
        unknown_modem_aborted_a72ec4 = false;

        delay = unknown_24;
        Debug_Printf("Waiting for dial result\n");
        while (process == true) {
            if (unknown_6000d0() == true || unknown_modem_aborted_a72ec4) {
                dialstatus = DIAL_CANCELED;
                if (SerialPort) {
                    char abort[3] = "\r";
                    SerialPort->Write_To_Serial_Port((unsigned char *)abort, 2);
                }
                Debug_Printf("Dialling cancelled\n");
                break;
            }
            unknown_517370(false);
            memset(buffer, 0, sizeof(buffer));
            delay = SerialPort->Get_Modem_Result(delay, buffer, 81, 15);
            if (delay == -1000) {
                dialstatus = DIAL_CANCELED;
                if (SerialPort) {
                    char abort[3] = "\r";
                    SerialPort->Write_To_Serial_Port((unsigned char *)abort, 2);
                }
                Debug_Printf("Dialling cancelled\n");
                break;
            }
            _strupr(buffer);
            if (strlen(buffer)) {
                Debug_Printf("From Modem: %s\n", buffer);
            }
            if (strstr(buffer, "CON")) {
                memset(ModemRXString, 0, 80);
                strncpy(ModemRXString, buffer, 79);
                dialstatus = DIAL_CONNECTED;
                Debug_Printf("Dial result is %s\n", buffer);
                process = false;
            } else if (strncmp(buffer, "BUSY", 4) == 0) {
                dialstatus = DIAL_BUSY;
                Debug_Printf("Dial result is %s\n", buffer);
                process = false;
            } else if (strncmp(buffer, "NO C", 4) == 0) {
                dialstatus = DIAL_NO_CARRIER;
                Debug_Printf("Dial result is %s\n", buffer);
                process = false;
            } else if (strncmp(buffer, "NO D", 4) == 0) {
                dialstatus = DIAL_NO_DIAL_TONE;
                Debug_Printf("Dial result is %s\n", buffer);
                process = false;
            } else if (strncmp(buffer, "ERRO", 4) == 0) {
                dialstatus = DIAL_ERROR;
                Debug_Printf("Dial result is %s\n", buffer);
                process = false;
            }
            if (delay <= 0) {
                Debug_Printf("NullModemClass::Dial_Modem has timed out\n");
                if (strlen(buffer)) {
                    Debug_Printf("Dial result is %s\n", buffer);
                }
                if (SerialPort) {
                    char abort[3] = "\r";
                    SerialPort->Write_To_Serial_Port((unsigned char *)abort, 2);
                }
                dialstatus = DIAL_TIMEOUT;
                break;
            }
        }

        SerialPort->Set_Abort_Function(0);
        unknown_modem_aborted_a72ec4 = false;
        unknown_dialog_5ff6e0(dialog);
    }
    return dialstatus;
}


// DialStatusType NullModemClass::Answer_Modem(bool reconnect) (0x005D16E0)
// Waits under a cancel dialog for the modem to ring, answers with ATA and
// waits for CONNECT (copied to ModemRXString), BUSY, NO CARRIER or ERROR;
// after the ring a timeout still connects while the modem has carrier.
extern "C" char *__cdecl strstr(char const *string, char const *search);
extern "C" char *__cdecl strncpy(char *destination, char const *source, unsigned int count);
extern "C" int __cdecl sprintf(char *buffer, char const *format, ...);

DialStatusType NullModemClass::Answer_Modem(bool reconnect)
{
    bool process;
    bool ring = false;
    int delay;
    DialStatusType dialstatus = DIAL_ERROR;
    char buffer[240];
    wchar_t const *text;

    if (reconnect == true) {
        text = StringTable::Load_String("TXT_MODEM_CONNERR_WAITING", 0, "D:\\Ra2\\NullMgr.CPP", 0x62C);
    } else {
        text = StringTable::Load_String("TXT_WAITING_FOR_CALL", 0, "D:\\Ra2\\NullMgr.CPP", 0x62E);
    }
    Debug_Printf("Waiting for ring\n");
    process = true;
    unknown_modem_aborted_a72ec4 = false;
    HWND dialog = unknown_cancel_dialog_6001e0(text, StringTable::Load_String("TXT_CANCEL", 0, "D:\\Ra2\\NullMgr.CPP", 0x635),
                                               &unknown_modem_aborted_a72ec4);
    if (dialog) {
        unknown_dialog_5ff7c0(dialog);
        SerialPort->Set_Abort_Function(Abort_Modem);
        unknown_modem_aborted_a72ec4 = false;

        delay = 3600;
        while (process == true) {
            if (unknown_6000d0() == true || unknown_modem_aborted_a72ec4) {
                dialstatus = DIAL_CANCELED;
                Debug_Printf("Answering cancelled\n");
                if (SerialPort) {
                    char abort[3] = "\r";
                    SerialPort->Write_To_Serial_Port((unsigned char *)abort, 2);
                }
                break;
            }
            unknown_517370(false);
            memset(buffer, 0, sizeof(buffer));
            delay = SerialPort->Get_Modem_Result(delay, buffer, 81, ring ? 15 : -1);
            Debug_Printf("Get_Modem_Result returned %d\n", delay);
            if (delay == -1000) {
                dialstatus = DIAL_CANCELED;
                Debug_Printf("Answering cancelled\n");
                if (SerialPort) {
                    char abort[3] = "\r";
                    SerialPort->Write_To_Serial_Port((unsigned char *)abort, 2);
                }
                break;
            }
            _strupr(buffer);
            if (strlen(buffer)) {
                Debug_Printf("Get_Modem_Result returned string:%s\n", buffer);
            }
            if (strncmp(buffer, "RING", 4) == 0) {
                Debug_Printf("RING\n");
                unknown_set_dialog_item_5b0(dialog, (LPARAM)StringTable::Load_String("TXT_ANSWERING", 0, "D:\\Ra2\\NullMgr.CPP", 0x66F));
                Debug_Printf("Answering with ATA\n");
                SerialPort->Write_To_Serial_Port((unsigned char *)"ATA\r", strlen("ATA\r"));
                ring = true;
                delay = unknown_24;
            } else if (strstr(buffer, "CONNECT")) {
                memset(ModemRXString, 0, 80);
                strncpy(ModemRXString, buffer, 79);
                dialstatus = DIAL_CONNECTED;
                Debug_Printf("Result is %s\n", buffer);
                process = false;
            } else if (strncmp(buffer, "BUSY", 4) == 0) {
                dialstatus = DIAL_BUSY;
                Debug_Printf("Result is %s\n", buffer);
                process = false;
            } else if (strncmp(buffer, "NO C", 4) == 0) {
                dialstatus = DIAL_NO_CARRIER;
                Debug_Printf("Result is %s\n", buffer);
                process = false;
            } else if (strncmp(buffer, "ERRO", 4) == 0) {
                dialstatus = DIAL_ERROR;
                Debug_Printf("Result is %s\n", buffer);
                WWMessageBox(L" ").Process(StringTable::Load_String("TXT_ERROR_ERROR", 0, "D:\\Ra2\\NullMgr.CPP", 0x687),
                    StringTable::Load_String("TXT_OK", 0, "D:\\Ra2\\NullMgr.CPP", 0x687), 0, 0, false);
                process = false;
            }
            if (delay <= 0) {
                if (ring) {
                    if (SerialPort->Get_Modem_Status() & 0x80) {
                        sprintf(ModemRXString, "%s", "Connected");
                        dialstatus = DIAL_CONNECTED;
                        Debug_Printf("Connected\n");
                    } else {
                        Debug_Printf("NullModemClass::Answer_Modem has timed out\n");
                        if (strlen(buffer)) {
                            Debug_Printf("Answer result is %s\n", buffer);
                        }
                        WWMessageBox(L" ").Process(StringTable::Load_String("TXT_ERROR_TIMEOUT", 0, "D:\\Ra2\\NullMgr.CPP", 0x697),
                            StringTable::Load_String("TXT_OK", 0, "D:\\Ra2\\NullMgr.CPP", 0x697), 0, 0, false);
                        Debug_Printf("Not connected\n");
                        dialstatus = DIAL_TIMEOUT;
                    }
                    process = false;
                } else {
                    delay = 3600;
                }
            }
        }

        SerialPort->Set_Abort_Function(0);
        unknown_modem_aborted_a72ec4 = false;
        unknown_dialog_5ff6e0(dialog);
    }
    return dialstatus;
}


// bool NullModemClass::Hangup_Modem() (0x005D1B70)
// Hangs the modem up: drops DTR, sends the escape code, then ATH and ATZ.
// Modem servicing (bit 0 of serial_session_flags_00a3fac0) is off meanwhile.

bool NullModemClass::Hangup_Modem()
{
    int status;
    int delay;
    char buffer[81];
    char escape[4];

    if (PortHandle == -1 || !SerialPort) {
        return false;
    }

    serial_session_flags_00a3fac0 &= ~1;

    status = SerialPort->Send_Command_To_Modem("AT", '\r', buffer, 81, 90, 1, -1);
    if (status == 2) {
        serial_session_flags_00a3fac0 |= 1;
        return true;
    }

    SerialPort->Set_Serial_DTR(false);
    Wait_Timer_00496A20(3200 / 60);
    SerialPort->Set_Serial_DTR(true);

    status = SerialPort->Send_Command_To_Modem("AT", '\r', buffer, 81, 90, 1, -1);
    if (status == 2) {
        serial_session_flags_00a3fac0 |= 1;
        return true;
    }

    delay = unknown_2c;
    while (delay > 0) {
        delay = SerialPort->Get_Modem_Result(delay, buffer, 81, -1);
    }

    escape[0] = unknown_30;
    escape[1] = unknown_30;
    escape[2] = unknown_30;
    escape[3] = 0;
    SerialPort->Write_To_Serial_Port((unsigned char *)escape, 3);

    delay = unknown_2c;
    while (delay > 0) {
        delay = SerialPort->Get_Modem_Result(delay, buffer, 81, -1);
        if (strncmp(buffer, "OK", 2) == 0) {
            break;
        }
    }

    status = SerialPort->Send_Command_To_Modem("ATH", '\r', buffer, 81, unknown_28, 1, -1);
    if (status == 2) {
    } else {
        serial_session_flags_00a3fac0 |= 1;
        return false;
    }

    status = SerialPort->Send_Command_To_Modem("ATZ", '\r', buffer, 81, 60, 1, -1);
    if (status != 2) {
        serial_session_flags_00a3fac0 |= 1;
        return false;
    }

    serial_session_flags_00a3fac0 |= 1;
    return true;
}


// void NullModemClass::Setup_Modem_Echo(void (__fastcall *func)(char c)) (0x005D1D20)

void NullModemClass::Setup_Modem_Echo(void (__fastcall *func)(char c))
{
    SerialPort->Set_Echo_Function(func);
}


// void NullModemClass::unknown_5d1d40() (0x005D1D40)
// Does nothing.

void NullModemClass::unknown_5d1d40()
{
}


// void NullModemClass::Translate_Line_Ends() (0x005D1D50)
// Replaces CR by 1 and LF by 2 in the serial manager's receive buffer.

extern "C" unsigned int __cdecl strlen(char const *string);
#pragma intrinsic(strlen)

void NullModemClass::Translate_Line_Ends()
{
    for (unsigned i = 0; i < strlen(serial_manager_00a40a28.ReceiveBuf); i++) {
        if (serial_manager_00a40a28.ReceiveBuf[i] == '\r') {
            serial_manager_00a40a28.ReceiveBuf[i] = 1;
        } else if (serial_manager_00a40a28.ReceiveBuf[i] == '\n') {
            serial_manager_00a40a28.ReceiveBuf[i] = 2;
        }
    }
}


// void NullModemClass::Clear_Receive_Buffer() (0x005D1DA0)

void NullModemClass::Clear_Receive_Buffer()
{
    ReceiveBuf[0] = 0;
    ReceiveIndex = 0;
}


// int NullModemClass::Abort_Modem() (0x005D1DB0)
// The serial port's abort function: services the game, and returns -1000
// once ESC was pressed (it stays set).

int NullModemClass::Abort_Modem()
{
    Service_Game_IO_00481fb0();
    Unknown_005B5EE0();
    int key = WWKeyboard->Check();
    if (key) {
        WWKeyboard->Clear();
        if (key == 0x1B) {
            unknown_modem_aborted_a72ec4 = true;
            return -1000;
        }
    }
    return unknown_modem_aborted_a72ec4 ? -1000 : 0;
}


// void NullModemClass::Setup_Abort_Modem() (0x005D1E00)

void NullModemClass::Setup_Abort_Modem()
{
    SerialPort->Set_Abort_Function(Abort_Modem);
    unknown_modem_aborted_a72ec4 = false;
}


// void NullModemClass::Remove_Abort_Modem() (0x005D1E20)

void NullModemClass::Remove_Abort_Modem()
{
    SerialPort->Set_Abort_Function(0);
    unknown_modem_aborted_a72ec4 = false;
}


// int NullModemClass::Change_IRQ_Priority(int irq) (0x005D1E40)
// Nothing to do under Windows.

int NullModemClass::Change_IRQ_Priority(int irq)
{
    return 0;
}


// int NullModemClass::Get_Modem_Status() (0x005D1E50)
// The port's modem status, without carrier detect (0x80) when the modem
// answers "AT" (it is in command mode).

int NullModemClass::Get_Modem_Status()
{
    int modemstatus;
    int status;
    char buffer[81];

    modemstatus = SerialPort->Get_Modem_Status();
    status = SerialPort->Send_Command_To_Modem("AT", '\r', buffer, 81, 90, 1, -1);
    if (status == 2) {
        modemstatus &= ~0x80;
    }
    return modemstatus;
}


// int NullModemClass::Send_Modem_Command(char *command, char terminator, char *buffer, int buflen, int delay, int retries) (0x005D1EA0)

int NullModemClass::Send_Modem_Command(char *command, char terminator, char *buffer, int buflen, int delay, int retries)
{
    return SerialPort->Send_Command_To_Modem(command, terminator, buffer, buflen, delay, retries, -1);
}


// int NullModemClass::Number_Of(char *text) (0x005D1ED0)
// The value of an all-digit string; -1 when any character is not a digit.

extern "C" int __cdecl isdigit(int character);
extern "C" int __cdecl atoi(char const *string);
extern "C" unsigned int __cdecl strlen(char const *string);
#pragma intrinsic(strlen)

int NullModemClass::Number_Of(char *text)
{
    int length = strlen(text);
    for (int i = 0; i < length; i++) {
        if (!isdigit(text[i])) {
            return -1;
        }
    }
    return atoi(text);
}


// int NullModemClass::Connection_ID(int index) (0x005D1F20, slot 5)

int NullModemClass::Connection_ID(int index)
{
    return 0;
}


// int NullModemClass::Connection_Index(int id) (0x005D1F30, slot 6)

int NullModemClass::Connection_Index(int id)
{
    return 0;
}


// void NullModemClass::unknown_slot14(unsigned long retrydelta, unsigned long maxretries, unsigned long timeout) (0x005D1F40, slot 14)

void NullModemClass::unknown_slot14(unsigned long retrydelta, unsigned long maxretries, unsigned long timeout)
{
    Set_Timing(retrydelta, maxretries, timeout, true);
}


// int NullModemClass::Send_Private_Message(void *buf, int buflen, int ack_req, int conn_id) (0x005D1F60, slot 2)

int NullModemClass::Send_Private_Message(void *buf, int buflen, int ack_req, int conn_id)
{
    if (!NumConnections) {
        return 0;
    }
    int result = Connection->Send_Packet(buf, buflen, ack_req, 0);
    if (!result) {
        SendOverflows++;
    }
    return result;
}


// int NullModemClass::Get_Private_Message(void *buf, int *buflen, int *conn_id) (0x005D1FA0, slot 3)

int NullModemClass::Get_Private_Message(void *buf, int *buflen, int *conn_id)
{
    if (!NumConnections) {
        return 0;
    }
    return Connection->Get_Packet(buf, buflen);
}


// int NullModemClass::Global_Num_Send() (0x005D1FD0, slot 7)

int NullModemClass::Global_Num_Send()
{
    if (Connection) {
        return Connection->Queue->SendCount;
    }
    return 0;
}


// int NullModemClass::Global_Num_Receive() (0x005D1FF0, slot 8)

int NullModemClass::Global_Num_Receive()
{
    if (Connection) {
        return Connection->Queue->ReceiveCount;
    }
    return 0;
}


// int NullModemClass::Private_Num_Send(int id) (0x005D2010, slot 9)

int NullModemClass::Private_Num_Send(int id)
{
    if (Connection) {
        return Connection->Queue->SendCount;
    }
    return 0;
}


// int NullModemClass::Private_Num_Receive(int id) (0x005D2030, slot 10)

int NullModemClass::Private_Num_Receive(int id)
{
    if (Connection) {
        return Connection->Queue->ReceiveCount;
    }
    return 0;
}


// void *NullModemClass::`scalar deleting destructor'(unsigned int flags) (0x005D2050)
// Compiler-generated with the vtable (the destructor inlined). No source of its own.
