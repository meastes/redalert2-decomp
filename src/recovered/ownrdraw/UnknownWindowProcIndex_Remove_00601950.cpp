// wlib Dictionary<HWND, WNDPROC>::remove(key, value) out of line: removes key, passing its window procedure
// back; the table shrinks (shrink() inline) once its load falls to the shrink threshold (0x00601950).
#include "shared/win32_api.hpp"
#include "shared/wol_dictionary.hpp"
template bool WolDictionary<HWND, WNDPROC>::Remove_Entry(HWND const &key, WNDPROC *old_value);
