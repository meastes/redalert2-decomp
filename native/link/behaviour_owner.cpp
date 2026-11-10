// Behaviour rows added by the owner from user crash reports (NOT EXACT,
// native/behaviour_rewrites.json).
#include "shared/display.hpp"

// 0x0049AF80 DisplayClass slot 44: accepted only as the leaf Leaf_49af80::get
// (a bare `ret 4`, pass-6a decompile 0049af80), so the typed virtual had no
// native body and ScrollClass::Mouse_Right_Press 0x0066ABE0 trapped on the first
// mouse move over the map (user, 2026-11-09). Empty, as the original.
void DisplayClass::Mouse_Right_Press(Point2D const &point) {}
