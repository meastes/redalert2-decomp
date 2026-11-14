// volinfo.cpp: the original translation unit re-filed as one source (Route A): the accepted
// bodies of its canonical compile group in address order, under the unit's own header
// state (its switches and includes in their original order). The module map
// manifests/original_target/modules/volinfo.json carries the addresses and the rows that
// stay in their own files.


#include "shared/cd.hpp"
#include "shared/cd_drive.hpp"
#include "shared/crt_string.hpp"
#include "shared/debug_printf.hpp"
#include "shared/win32_find.hpp"
#include "shared/crt_tokens.hpp"


// Game disk volume check -- 0x00715090,407body+9NOP independently bounded.
WIN32_IMPORT DWORD WINAPI GetLogicalDriveStringsA(DWORD size,char *buffer);
WIN32_IMPORT UINT WINAPI GetDriveTypeA(LPCSTR root);
extern char volume_label_format_007d5c68[];
bool __fastcall VolumeCheck()
{
    char roots[260];
    DriveInfo info;
    if(!GetLogicalDriveStringsA(sizeof(roots),roots)) return false;
    char const *root=roots;
    while(strlen(root)!=0) {
        if(GetDriveTypeA(root)==5) {
            if(!Drive_Info_00715230(root,&info)) {
                Debug_Printf("VolumeCheck() - GetVolInfo failed\n");
                root+=strlen(root)+1;
                continue;
            } else {
                char label[32];
                for(int index=0;index<8;++index) {
                    sprintf(label,volume_label_format_007d5c68,index+1);
                    if(!_stricmp(label,info.label)) return true;
                }
            }
        }
        root+=strlen(root)+1;
    }
    if(CD().Force_Available(CD::RequiredCD)) return true;
    return false;
}


// Drive_Info_00715230 (0x00715230): queries a drive root for the disk search. A drive that reports free clusters is
// refused (hypothesis: the disk must be full, as a game disk is). Debug strings are independently checked.
WIN32_IMPORT UINT WINAPI GetDriveTypeA(LPCSTR root);
WIN32_IMPORT BOOL WINAPI GetDiskFreeSpaceA(LPCSTR root,DWORD *sectors,DWORD *bytes,DWORD *free_clusters,DWORD *clusters);
WIN32_IMPORT BOOL WINAPI GetVolumeInformationA(LPCSTR root,char *name,DWORD size,DWORD *serial,DWORD *max_component,DWORD *flags,char *filesystem,DWORD filesystem_size);

bool __fastcall Drive_Info_00715230(LPCSTR root,DriveInfo *info)
{
    DWORD sectors,bytes,free_clusters,clusters,flags,max_component;
    memset(info,0,sizeof(*info));
    info->type=GetDriveTypeA(root);
    if (!GetDiskFreeSpaceA(root,&sectors,&bytes,&free_clusters,&clusters)) {
        Debug_Printf("GetDiskFreeSpace() failed: %s\n",0);
        return false;
    }
    info->used_bytes=sectors*bytes*free_clusters;
    if (free_clusters) return false;
    info->total_bytes=sectors*bytes*clusters;
    if (!GetVolumeInformationA(root,info->label,64,&info->serial,&max_component,&flags,info->filesystem,64)) {
        Debug_Printf("GetVolumeInformation() - failed: %s\n",0);
        return false;
    }
    return true;
}


// Directory_Totals_00715300 (0x00715300): walks a FindFirstFile pattern ("X:\dir\*") recursively, counting
// subdirectories (skipping "." and ".."), files and the low 32 bits of the file sizes. Caller and purpose unconfirmed.

void __fastcall Directory_Totals_00715300(LPCSTR path,int *dirs,int *files,int *bytes)
{
    WIN32_FIND_DATA data;
    char sub[260];
    HANDLE handle=FindFirstFileA(path,&data);
    if (handle==INVALID_HANDLE_VALUE) return;
    do {
        if (data.dwFileAttributes & 0x10) {
            if (_stricmp(data.cFileName,".") && _stricmp(data.cFileName,"..")) {
                ++*dirs;
                strcpy(sub,path);
                sub[strlen(sub)-1]=0;
                strcat(sub,data.cFileName);
                strcat(sub,"\\*");
                Directory_Totals_00715300(sub,dirs,files,bytes);
            }
        } else {
            ++*files;
            *bytes+=data.nFileSizeLow;
        }
    } while (FindNextFileA(handle,&data));
    FindClose(handle);
}
