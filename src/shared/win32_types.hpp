// Win32 type names spelled as the pinned VC6 SDK spells them (WINDEF.H/WINNT.H with STRICT, the SDK default):
// typedefs only, no declarations, so a unit may use the original's names without the SDK headers and without
// conflicting with stand-in import declarations. A unit converted to the SDK (RA2_SDK_HEADERS) gets them from
// windows.h instead. HINSTANCE/HMODULE are left out: shared/win32_api.hpp spells them as HANDLE (not the SDK's
// STRICT struct pointer), and both headers must agree in a unit that includes both.
#ifndef RA2_SHARED_WIN32_TYPES_HPP
#define RA2_SHARED_WIN32_TYPES_HPP
#ifdef RA2_SDK_HEADERS
#include "shared/win.hpp"
#else
typedef unsigned long DWORD;
typedef int BOOL;
typedef unsigned char BYTE;
typedef unsigned short WORD;
typedef unsigned int UINT;
typedef long LONG;
typedef char CHAR;
typedef void *PVOID;
typedef void *LPVOID;
typedef void *HANDLE;
typedef unsigned int WPARAM;
typedef long LPARAM;
typedef long LRESULT;
typedef DWORD LCID;
typedef CHAR *LPSTR;
typedef const CHAR *LPCSTR;
typedef DWORD *LPDWORD;
typedef struct HWND__ *HWND;
typedef struct HDC__ *HDC;
typedef struct HKEY__ *HKEY;
#endif
#endif
