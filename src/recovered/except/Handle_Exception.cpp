#include "shared/surface.hpp"
#include "shared/win32_api.hpp"
#include "shared/debug_printf.hpp"
#include "shared/dsurface.hpp"
WIN32_IMPORT int WINAPI lstrlenA(LPCSTR string);
WIN32_IMPORT DWORD WINAPI GetCurrentThreadId();
void __cdecl Unknown_00522e10();
struct _EXCEPTION_POINTERS;
void __fastcall Dump_Exception_004b7df0(_EXCEPTION_POINTERS *information);
int __fastcall Show_Exception_Dialog_004b7cf0();
void __fastcall Destroy_Exception_Window_005ff6e0(HWND window);
extern int exception_depth_007d6af8;
extern bool exception_flag_007d6afc;
extern char exception_text_00855ba8[];
extern DWORD main_thread_00b25f78;
extern HWND exception_window_00b25dd4;
extern bool unknown_flag_839c4c;
extern Surface *VisibleSurface;
extern Surface *CompositeSurface;
extern Surface *TileSurface;
extern Surface *SidebarSurface;
extern Surface *AlternateSurface;
extern char const exception_log_007d7ea0[];
extern char const exception_filename_007d7e94[];
extern char const show_dialog_log_007d7e38[];
extern char const dialog_unavailable_log_007d7e4c[];
// Original filter shims0068E270 and00690891 pass the exception code in ECX
// and the actual EXCEPTION_POINTERS in EDX. Code is unused by this body.
int __fastcall Handle_Exception_004b87f0(unsigned int exceptionCode, _EXCEPTION_POINTERS *information)
{
    Debug_Printf(exception_log_007d7ea0);
    Unknown_00522e10();
    ++exception_depth_007d6af8;
    if(exception_depth_007d6af8 == 0) {
        Dump_Exception_004b7df0(information);
        HANDLE file=CreateFileA(exception_filename_007d7e94,GENERIC_WRITE,0,0,CREATE_ALWAYS,FILE_ATTRIBUTE_NORMAL,0);
        if(file!=INVALID_HANDLE_VALUE) {
            DWORD written;
            WriteFile(file,exception_text_00855ba8,lstrlenA(exception_text_00855ba8),&written,0);
            CloseHandle(file);
        }
    }
    if (((VisibleSurface && VisibleSurface->Is_Locked())
      || (HiddenSurface && HiddenSurface->Is_Locked())
      || (CompositeSurface && CompositeSurface->Is_Locked())
      || (TileSurface && TileSurface->Is_Locked())
      || (SidebarSurface && SidebarSurface->Is_Locked())
      || (AlternateSurface && AlternateSurface->Is_Locked())) && unknown_flag_839c4c)
        goto unavailable;
    if (GetCurrentThreadId()!=main_thread_00b25f78) {
unavailable:
        Debug_Printf(dialog_unavailable_log_007d7e4c);
        return 0;
    }
    Debug_Printf(show_dialog_log_007d7e38);
    if(Show_Exception_Dialog_004b7cf0()!=0x481) return 0;
    exception_flag_007d6afc=false;
    if(exception_depth_007d6af8==0) exception_depth_007d6af8=-1;
    else if(exception_depth_007d6af8==2) return 0;
    if(exception_window_00b25dd4) Destroy_Exception_Window_005ff6e0(exception_window_00b25dd4);
    return 1;
}
