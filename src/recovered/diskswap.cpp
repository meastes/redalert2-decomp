// diskswap.cpp: the original translation unit re-filed as one source (Route A): the accepted
// bodies of its canonical compile group in address order, under the unit's own header
// state (its switches and includes in their original order). The module map
// manifests/original_target/modules/diskswap.json carries the addresses and the rows that
// stay in their own files.


#include "shared/cd_file_class.hpp"
#include "shared/cd_timer.hpp"
#include "shared/system_timer.hpp"
#include "shared/win32_api.hpp"
#include "shared/cd.hpp"


WIN32_IMPORT int __cdecl wsprintfA(char *out,char const *format,...);
WIN32_IMPORT BOOL WINAPI GetVolumeInformationA(char const *root,char *name,DWORD size,DWORD *serial,DWORD *max_component,DWORD *flags,char *filesystem,DWORD filesystem_size);
extern "C" int __cdecl sprintf(char *out,char const *format,...);
extern "C" int __cdecl _stricmp(char const *first,char const *second);
// Queries RA1..RA8 volume labels and verifies the corresponding RA<n>.dsk file.
// Format literals are independently checked against the verified target.
int __fastcall Get_CD_Index(int drive,int timeout)
{
    CDTimerClass<SystemTimerClass> timer;
    timer = CDTimerClass<SystemTimerClass>(timeout);
    char path[128];
    char volume[128];
    char expected[32];
    DWORD maximum_component;
    DWORD flags;
    int letter = drive + 'A';
    wsprintfA(path,"%c:\\",letter);
    while (!GetVolumeInformationA(path,volume,127,0,&maximum_component,&flags,0,0)) {
        if (timer.Value() == 0 || GetLastError() != 21) return -1;
        wsprintfA(path,"%c:\\",letter);
    }
    int index=-1;
    for (int candidate=0;candidate<8;++candidate) {
        sprintf(expected,"RA%d",candidate+1);
        if (_stricmp(expected,volume) == 0) {
            index=candidate;
            break;
        }
    }
    if (index > -1) {
        int retries=0;
        for (;;) {
            wsprintfA(path,"%c:\\RA%d.dsk",letter,index+1);
            HANDLE file=CreateFileA(path,GENERIC_READ,FILE_SHARE_READ,0,OPEN_EXISTING,FILE_ATTRIBUTE_NORMAL,0);
            if (file != INVALID_HANDLE_VALUE) {
                CloseHandle(file);
                return index;
            }
            if (++retries >= 2) return -1;
        }
    }
    return -1;
}


// Searches the current, previous and enumerated CD drives, prompting as needed.
// Layout hypothesis: 26 drive indices, count and rotating cursor at +68/+6C.
struct CDDriveList_004985C0 {
    int Drives[26];
    int Count;
    int Cursor;
    int Next() {
        if (Count != 0) {
            if (Cursor == Count) Cursor=0;
            return Drives[Cursor++];
        }
        return -1;
    }
};
extern CDDriveList_004985C0 cd_drives_00a408a8;
extern int loaded_cd_007d5c54;
bool CD::Unknown_004985C0(int cd)
{
    int found_drive = 0;
    if (cd != -2) {
        int drive = CDFileClass::CurrentCDDrive;
        int index = Get_CD_Index(drive,60);
        if (index <= -1 || (cd != index && cd != -1) || (found_drive = drive) == 0) {
            if (CDFileClass::LastCDDrive != 0 && CDFileClass::LastCDDrive != CDFileClass::CurrentCDDrive) {
                index = Get_CD_Index(CDFileClass::LastCDDrive,600);
                if (index > -1) {
                    if (cd == index) goto available;
                    if (cd == -1) goto done;
                }
            }
            int timeout = 120;
            for (;;) {
                for (int i=0;i<cd_drives_00a408a8.Count;++i) {
                    drive = cd_drives_00a408a8.Next();
                    index = Get_CD_Index(drive,timeout);
                    if (index > -1 && (cd == index || cd == -1)) {
                        found_drive = drive;
                        break;
                    }
                }
                if (found_drive > 0) break;
                if (!Insert_Disk(cd)) return false;
                timeout = 300;
            }
            CDFileClass::Set_CD_Drive(found_drive);
            CDFileClass::Refresh_Search_Drives();
        }
    available:
        if (cd > -1 && loaded_cd_007d5c54 != cd) {
            loaded_cd_007d5c54 = cd;
            return Swap_Disk(cd);
        }
    }
done:
    return true;
}
