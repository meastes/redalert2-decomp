// wlib Dictionary<HWND, WNDPROC>::getPointer out of line: looks up key; on a hit stores its window procedure
// slot in value and returns true (0x006018E0).
#include "shared/win32_api.hpp"
#include "shared/wol_dictionary.hpp"
template bool WolDictionary<HWND, WNDPROC>::Get_Pointer(HWND const &key, WNDPROC **value) const;
