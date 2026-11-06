// wlib Dictionary<HWND, WinData>::expand out of line for the control registry: doubles the bucket count and
// rehashes every node; a fixed-size table is left alone (0x00601F70).
#include "shared/owner_draw_dialog_control.hpp"
#include "shared/wol_dictionary.hpp"
template void WolDictionary<unsigned int, OwnerDraw::UnknownControlRecord>::Grow_Table();
