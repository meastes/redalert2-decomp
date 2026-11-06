// wlib Dictionary<HWND, WNDPROC>::expand out of line: doubles the bucket count and rehashes every node; a
// fixed-size table is left alone (0x00601B90).
#include "shared/win32_api.hpp"
#include "shared/wol_dictionary.hpp"
template void WolDictionary<HWND, WNDPROC>::Grow_Table();
