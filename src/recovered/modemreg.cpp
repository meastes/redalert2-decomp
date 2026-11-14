// modemreg.cpp: the original translation unit re-filed as one source (Route A): the accepted
// bodies of its canonical compile group in address order, under the unit's own header
// state (its switches and includes in their original order). The module map
// manifests/original_target/modules/modemreg.json carries the addresses and the rows that
// stay in their own files.


#include "shared/win_modem.hpp"
#include "shared/crt_string.hpp"
#include "shared/null_modem.hpp"
#include "shared/crt_memory.hpp"
#include "shared/crt_tokens.hpp"
#include "shared/debug_printf.hpp"
#include "shared/unknown_globals.hpp"


// HKEY __fastcall Search_Registry_Key(HKEY key, char const *value_name, char const *value) (0x0059CBA0)

HKEY __fastcall Search_Registry_Key(HKEY key, char const *value_name, char const *value)
{
    DWORD index = 0;
    char *sub_key_name = new char[256];
    DWORD sub_key_name_size = 256;
    FILETIME last_write_time;
    DWORD value_type;
    unsigned char *key_value = new unsigned char[256];
    DWORD key_value_size = 256;
    LONG result;
    do {
        sub_key_name_size = 256;
        result = RegEnumKeyExA(key, index++, sub_key_name, &sub_key_name_size, 0, 0, 0, &last_write_time);
        if (result == 0) {
            HKEY sub_key = Get_Registry_Sub_Key(key, sub_key_name, false);
            if (sub_key) {
                key_value_size = 256;
                if (RegQueryValueExA(sub_key, value_name, 0, &value_type, key_value, &key_value_size) == 0) {
                    if (value_type == REG_SZ && !strcmp((char *)key_value, value)) {
                        delete[] sub_key_name;
                        delete[] key_value;
                        return sub_key;
                    }
                }
                HKEY found = Search_Registry_Key(sub_key, value_name, value);
                RegCloseKey(sub_key);
                if (found) {
                    delete[] sub_key_name;
                    delete[] key_value;
                    return found;
                }
            }
        }
    } while (result == 0);
    delete[] sub_key_name;
    delete[] key_value;
    return 0;
}


// ModemRegistryEntryClass::ModemRegistryEntryClass (0x0059CD00)

ModemRegistryEntryClass::ModemRegistryEntryClass(int modem_number)
{
    HKEY key;
    unsigned char return_buf[256];
    DWORD retbuf_size = sizeof(return_buf);
    int pnp = 0;

    ModemName = 0;
    ModemDeviceName = 0;
    ErrorCorrectionEnable = 0;
    ErrorCorrectionDisable = 0;
    CompressionEnable = 0;
    CompressionDisable = 0;
    HardwareFlowControl = 0;

    char which_modem[5];
    wsprintfA(which_modem, "%04d", modem_number);

    if (unknown_flag_839c4c) {
        if (RegOpenKeyExA(HKEY_LOCAL_MACHINE, "System\\CurrentControlSet\\Services\\Class\\Modem", 0, KEY_READ, &key) != 0) {
            return;
        }
        if (RegOpenKeyExA(key, which_modem, 0, KEY_READ, &key) != 0) {
            Debug_Printf("Unable to find System\\CurrentControlSet\\Services\\Class\\Modem in registry\n");
            return;
        }
    } else {
        if (RegOpenKeyExA(HKEY_LOCAL_MACHINE, "System\\CurrentControlSet\\Control\\Class", 0, KEY_READ, &key) != 0) {
            Debug_Printf("Unable to find System\\CurrentControlSet\\Control\\Class in registry\n");
            return;
        }
        char key_path[256];
        char sub_key_name[256];
        char class_name[256];
        DWORD sub_key_name_size;
        DWORD class_name_size;
        FILETIME last_write_time;
        HKEY class_key;
        for (DWORD index = 0;; index++) {
            sub_key_name_size = 256;
            class_name_size = 256;
            if (RegEnumKeyExA(key, index, sub_key_name, &sub_key_name_size, 0, class_name, &class_name_size, &last_write_time) != 0) {
                return;
            }
            strcpy(key_path, "System\\CurrentControlSet\\Control\\Class\\");
            strcat(key_path, sub_key_name);
            if (RegOpenKeyExA(HKEY_LOCAL_MACHINE, key_path, 0, KEY_READ, &class_key) == 0) {
                retbuf_size = sizeof(return_buf);
                if (RegQueryValueExA(class_key, "Class", 0, 0, return_buf, &retbuf_size) == 0
                    && _stricmp((char *)return_buf, "Modem") == 0) {
                    break;
                }
            }
        }
        strcpy(key_path, "System\\CurrentControlSet\\Control\\Class\\");
        strcat(key_path, sub_key_name);
        strcat(key_path, "\\");
        strcat(key_path, which_modem);
        if (RegOpenKeyExA(HKEY_LOCAL_MACHINE, key_path, 0, KEY_READ, &key) != 0) {
            return;
        }
    }

    retbuf_size = sizeof(return_buf);
    if (RegQueryValueExA(key, "Model", 0, 0, return_buf, &retbuf_size) != 0) {
        RegCloseKey(key);
        return;
    }
    ModemName = new char[retbuf_size + 1];
    memcpy(ModemName, return_buf, retbuf_size);
    Debug_Printf("Modem name is %s\n", ModemName);

    retbuf_size = sizeof(return_buf);
    if (RegQueryValueExA(key, "AttachedTo", 0, 0, return_buf, &retbuf_size) != 0) {
        retbuf_size = sizeof(return_buf);
        if (unknown_flag_839c4c || RegQueryValueExA(key, "PnPAttachedTo", 0, 0, return_buf, &retbuf_size) != 0) {
            pnp = 1;
            ModemDeviceName = new char[strlen(ModemName) + 1];
            strcpy(ModemDeviceName, ModemName);
        } else {
            ModemDeviceName = new char[retbuf_size + 1];
            memcpy(ModemDeviceName, return_buf, retbuf_size);
        }
    } else {
        ModemDeviceName = new char[retbuf_size + 1];
        memcpy(ModemDeviceName, return_buf, retbuf_size);
    }
    Debug_Printf("Modem attached to %s\n", ModemDeviceName);

    key = Get_Registry_Sub_Key(key, "Settings", true);
    if (!key) {
        return;
    }

    retbuf_size = sizeof(return_buf);
    if (RegQueryValueExA(key, "ErrorControl_On", 0, 0, return_buf, &retbuf_size) == 0) {
        ErrorCorrectionEnable = new char[retbuf_size + 1];
        memcpy(ErrorCorrectionEnable, return_buf, retbuf_size);
        Debug_Printf("Modem error control enable string: %s\n", ErrorCorrectionEnable);
    }
    retbuf_size = sizeof(return_buf);
    if (RegQueryValueExA(key, "ErrorControl_Off", 0, 0, return_buf, &retbuf_size) == 0) {
        ErrorCorrectionDisable = new char[retbuf_size + 1];
        memcpy(ErrorCorrectionDisable, return_buf, retbuf_size);
        Debug_Printf("Modem error control disable string: %s\n", ErrorCorrectionDisable);
    }
    retbuf_size = sizeof(return_buf);
    if (RegQueryValueExA(key, "Compression_On", 0, 0, return_buf, &retbuf_size) == 0) {
        CompressionEnable = new char[retbuf_size + 1];
        memcpy(CompressionEnable, return_buf, retbuf_size);
        Debug_Printf("Modem compression enable string: %s\n", CompressionEnable);
    }
    retbuf_size = sizeof(return_buf);
    if (RegQueryValueExA(key, "Compression_Off", 0, 0, return_buf, &retbuf_size) == 0) {
        CompressionDisable = new char[retbuf_size + 1];
        memcpy(CompressionDisable, return_buf, retbuf_size);
        Debug_Printf("Modem compression disable string: %s\n", CompressionDisable);
    }
    retbuf_size = sizeof(return_buf);
    if (RegQueryValueExA(key, "FlowControl_Hard", 0, 0, return_buf, &retbuf_size) == 0) {
        HardwareFlowControl = new char[retbuf_size + 1];
        memcpy(HardwareFlowControl, return_buf, retbuf_size);
        Debug_Printf("Modem hardware flow control enable string: %s\n", HardwareFlowControl);
    }
    RegCloseKey(key);

    if (pnp) {
        char search_string[256] = "Modem\\";
        strcat(search_string, which_modem);
        key = Get_Registry_Sub_Key(HKEY_LOCAL_MACHINE, "Enum", false);
        if (!key) {
            return;
        }
        HKEY port_key = Search_Registry_Key(key, "Driver", search_string);
        if (port_key) {
            retbuf_size = sizeof(return_buf);
            if (RegQueryValueExA(port_key, "PORTNAME", 0, 0, return_buf, &retbuf_size) == 0) {
                if (ModemDeviceName) {
                    delete[] ModemDeviceName;
                }
                ModemDeviceName = new char[retbuf_size + 1];
                memcpy(ModemDeviceName, return_buf, retbuf_size);
                Debug_Printf("PNP Modem attached to %s\n", ModemName);
            }
        } else if (ModemDeviceName && ModemName && strcmp(ModemName, ModemDeviceName) == 0) {
            Debug_Printf("Failed to find port for device %s\n", ModemName);
            delete[] ModemName;
            ModemName = 0;
            delete[] ModemDeviceName;
            ModemDeviceName = 0;
        }
        RegCloseKey(key);
    }
}


// ModemRegistryEntryClass::~ModemRegistryEntryClass (0x0059D4F0)

ModemRegistryEntryClass::~ModemRegistryEntryClass()
{
    Clear_0059d500();
}


// ModemRegistryEntryClass::Clear_0059d500 (0x0059D500)

void ModemRegistryEntryClass::Clear_0059d500()
{
    if (ModemName) {
        delete[] ModemName;
        ModemName = 0;
    }
    if (ModemDeviceName) {
        delete[] ModemDeviceName;
        ModemDeviceName = 0;
    }
    if (ErrorCorrectionEnable) {
        delete[] ErrorCorrectionEnable;
        ErrorCorrectionEnable = 0;
    }
    if (ErrorCorrectionDisable) {
        delete[] ErrorCorrectionDisable;
        ErrorCorrectionDisable = 0;
    }
    if (CompressionEnable) {
        delete[] CompressionEnable;
        CompressionEnable = 0;
    }
    if (CompressionDisable) {
        delete[] CompressionDisable;
        CompressionDisable = 0;
    }
    if (HardwareFlowControl) {
        delete[] HardwareFlowControl;
        HardwareFlowControl = 0;
    }
}
