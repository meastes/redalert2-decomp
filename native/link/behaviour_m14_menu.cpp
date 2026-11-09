// Behaviour rows (Scout1119, native M14; NOT EXACT, native/behaviour_rewrites.json):
// leaves the menus reach without a recovered body, written from the pass-6a
// decompiles with the shared headers' names.
#include "shared/wide_string.hpp"

extern wchar_t const unknown_empty_wide_839d4c[];   // 0x00839D4C (the empty wide string)

// 0x00770430 (decompile: returns the buffer, or the empty wide string 0x00839D4C
// for a null one): the owner-draw status line reads a control's help text with it.
unsigned short const *WideString::unknown_text_770430() const
{
    return Buffer ? Buffer : (unsigned short const *)unknown_empty_wide_839d4c;
}

// 0x00770430 for the in-file view of unknown_set_dialog_wide_text 0x005C3450 (the
// same leaf: the buffer, or the empty wide string).
class UnknownWideText_005c3450 {
public:
    unsigned short const *unknown_text_770430() const;
    unsigned short *Buffer;
};
unsigned short const *UnknownWideText_005c3450::unknown_text_770430() const
{
    return Buffer ? Buffer : (unsigned short const *)unknown_empty_wide_839d4c;
}
