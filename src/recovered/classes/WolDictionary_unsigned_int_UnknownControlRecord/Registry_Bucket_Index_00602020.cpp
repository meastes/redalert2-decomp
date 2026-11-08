// wlib Dictionary::keyHash, the control registry's out-of-line instance (0x00602020): the template's own
// instance (shared/wol_dictionary.hpp), emitted for the call sites /Ob2 refuses (see the template).
#include "shared/wol_dictionary.hpp"
#include "shared/owner_draw_dialog_control.hpp"
template unsigned int WolDictionary<unsigned int, OwnerDraw::UnknownControlRecord>::Key_Hash(unsigned int const &key) const;
