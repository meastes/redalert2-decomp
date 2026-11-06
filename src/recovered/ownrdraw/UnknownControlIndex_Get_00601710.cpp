// wlib Dictionary<HWND, WinData>::getPointer out of line for the control registry (keyed by the window's
// value): looks up key; on a hit stores its record's address in value and returns true (0x00601710).
#include "shared/owner_draw_dialog_control.hpp"
#include "shared/wol_dictionary.hpp"
template bool WolDictionary<unsigned int, OwnerDraw::UnknownControlRecord>::Get_Pointer(unsigned int const &key, OwnerDraw::UnknownControlRecord **value) const;
