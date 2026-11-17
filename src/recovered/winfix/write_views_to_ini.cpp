// void __fastcall write_views_to_ini(char const *section, HWND window, INIClass &ini) -- 0x00736CD0
// Purpose: writes window's screen position as X and Y under section of
// ini, remembers section for the child callback (0x00B25F28) and saves
// every child window through 0x00736B40.
// Assumptions: the callback's behaviour is only inferred from its use.
#include "shared/win32_api.hpp"
#include "shared/ini_class.hpp"

typedef BOOL (WINAPI *WNDENUMPROC)(HWND window, long parameter);
WIN32_IMPORT BOOL WINAPI GetWindowRect(HWND window, RECT *rect);
WIN32_IMPORT BOOL WINAPI EnumChildWindows(HWND parent, WNDENUMPROC callback, long parameter);

extern char const *last_view_ini_section_name;
BOOL __stdcall write_view_to_ini(HWND window, long parameter);

void __fastcall write_views_to_ini(char const *section, HWND window, INIClass &ini)
{
    if (window != 0) {
        RECT rect;
        GetWindowRect(window, &rect);
        ini.Put_Int(section, "X", rect.left);
        ini.Put_Int(section, "Y", rect.top);
        last_view_ini_section_name = section;
        EnumChildWindows(window, write_view_to_ini, (long)&ini);
    }
}
