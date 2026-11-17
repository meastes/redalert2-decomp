// copyprotection.cpp: the original translation unit re-filed as one source (Route A): the accepted
// bodies of its canonical compile group in address order, under the unit's own header
// state (its switches and includes in their original order). The module map
// manifests/original_target/modules/copyprotection.json carries the addresses and the rows that
// stay in their own files.


#include "shared/win32_api.hpp"
#include "shared/debug_printf.hpp"
#include "shared/launcher_validation.hpp"
#include "shared/crt_string.hpp"
#include "shared/version.hpp"


extern "C" char const launch_check_log_007d5184[];
extern "C" char const launch_mutex_name_007d515c[];
bool __cdecl CopyProtection_Check_Already_Running_004918f0()
{
    Debug_Printf(launch_check_log_007d5184);
    HANDLE mutex=CreateMutexA(0,0,launch_mutex_name_007d515c);
    bool already_running=GetLastError()==183;
    if(mutex) CloseHandle(mutex);
    return already_running;
}


bool __cdecl Notify_Launcher_00491930()
{
    Debug_Printf(launcher_notify_log_007d5218);
    MSG message;
    PeekMessageA(&message,0,0x400,0x400,0);
    HANDLE event=OpenEventA(2,1,launcher_event_007d51f0);
    if(event) {
        SetEvent(event);
        CloseHandle(event);
        Debug_Printf(launcher_notified_log_007d51dc);
        return true;
    }
    Debug_Printf(launcher_notify_failed_log_007d51b8);
    return false;
}


void __fastcall Receive_Launcher_Message_004919b0(MSG const *message)
{
    if(message->message==0xbeef) {
        Debug_Printf(launcher_received_log_007d5274);
        launcher_message_00851b3c=(char const *)MapViewOfFileEx((HANDLE)message->lParam,0xf001f,0,0,0,0);
        if(!launcher_message_00851b3c) {
            Debug_Printf(launcher_map_failed_log_007d5254);
            return;
        }
        Debug_Printf(launcher_message_log_007d523c,launcher_message_00851b3c);
    }
}


extern char const launcher_validate_log_007d52ec[];
extern char const launcher_expected_message_007d52b8[];
bool __fastcall Probe_Launcher_Registry_00491b70();
// Zero-argument convention and bool spelling remain source/ABI hypotheses.
bool __fastcall Validate_Launcher_00491a10()
{
    Debug_Printf(launcher_validate_log_007d52ec);
    if (launcher_message_00851b3c)
        return strcmp(launcher_message_00851b3c,launcher_expected_message_007d52b8)==0;
    return Probe_Launcher_Registry_00491b70();
}


void __cdecl CopyProtection_Shutdown_00491b00()
{
    Debug_Printf(launcher_shutdown_log_007d530c);
    if(launcher_message_00851b3c) {
        UnmapViewOfFile(launcher_message_00851b3c);
        launcher_message_00851b3c=0;
    }
    HKEY key;
    if(RegOpenKeyExA((HKEY)0x80000000,launcher_registry_key_007d52ac,0,0xf003f,&key)==0) {
        RegDeleteValueA(key,launcher_registry_value_007d52a8);
        RegCloseKey(key);
    }
}


WIN32_IMPORT LONG WINAPI RegQueryValueExA(HKEY key,LPCSTR name,DWORD *reserved,DWORD *type,unsigned char *data,DWORD *size);
// Original zero-argument convention and AL result spelling remain hypotheses.
bool __fastcall Probe_Launcher_Registry_00491b70()
{
    HKEY key;
    if(RegOpenKeyExA((HKEY)0x80000000,launcher_registry_key_007d52ac,0,0x20019,&key)==0) {
        DWORD size=4;
        DWORD type,value;
        LONG status=RegQueryValueExA(key,launcher_registry_value_007d52a8,0,&type,(unsigned char *)&value,&size);
        RegCloseKey(key);
        if(status==0 && type==4) return value==VerNum.Version_Number();
    }
    return false;
}
