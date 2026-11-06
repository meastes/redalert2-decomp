// @ref ?Shrink_Table@?$WolDictionary@IUUnknownControlRecord@OwnerDraw@@@@QAEXXZ=006027e0
// wlib Dictionary<HWND, WinData>::remove(key, value) out of line for the control registry: removes key,
// passing its record back; the table shrinks (shrink() out of line, 0x006027E0) once its load falls to the
// shrink threshold (0x00601C50).
#include "shared/owner_draw_dialog_control.hpp"
#include "shared/wol_dictionary.hpp"
template bool WolDictionary<unsigned int, OwnerDraw::UnknownControlRecord>::Remove_Entry(unsigned int const &key, OwnerDraw::UnknownControlRecord *old_value);
