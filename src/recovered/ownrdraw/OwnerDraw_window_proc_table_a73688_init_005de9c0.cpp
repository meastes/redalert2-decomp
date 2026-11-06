// The ownrdraw window table at 0x00A73688 as wlib's chained dictionary (shared/wol_dictionary.hpp:
// thresholds 0.2/0.8, 128 buckets), keyed by window: its dynamic initializer (0x005DE9C0) inlines the
// constructor and registers the atexit destructor (0x005DEA60, the next draft), which inlines the
// destructor with clear() and shrink(). The unit's window procedures reach it through this object
// (RA2_OWNERDRAW_WNDPROC_DICTIONARY); its out-of-line members are getPointer 0x006018E0, remove 0x00601950,
// expand 0x00601B90, keyHash 0x00601C30 and shrink 0x00602730.
// @ref ?unknown_window_proc_table_a73688@@3V?$WolDictionary@PAUHWND__@@P6GJPAU1@IIJ@Z@@A=00a73688
// @ref ?unknown_hash_window_5de970@@YIIABQAUHWND__@@@Z=005de970
#include "shared/wol_dictionary.hpp"

// The window's handle value as its hash (0x005DE970).
unsigned int __fastcall unknown_hash_window_5de970(HWND const &key);

WolDictionary<HWND, WNDPROC> unknown_window_proc_table_a73688(unknown_hash_window_5de970);
