// void WolDictionary<unsigned int, OwnerDraw::UnknownControlRecord>::Shrink_Table() (0x006027E0): the
// control registry 0x00A73640 (nodes of 0x208 bytes; called from Remove_Entry 0x00601C50, 0x005EC360 and
// 0x005F8740). wlib Dictionary::shrink out of line, the template's own instance (the registry's atexit
// destructor 0x005DEE00 expands the same body through Shrink_Buckets; RA2_WOL_DICTIONARY_SHRINK_ACCESSORS).
#include "shared/wol_dictionary.hpp"
#include "shared/owner_draw_dialog_control.hpp"

template void WolDictionary<unsigned int, OwnerDraw::UnknownControlRecord>::Shrink_Table();
