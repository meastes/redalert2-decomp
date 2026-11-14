// 0x005def80: function not yet identified
//
// Body: 47 bytes (frozen inventory bound).
// Referenced from 5 sites (unconditional call).
// Ghidra prototype: undefined4 __fastcall unknown_focus_next_tab_item_005def80(HWND param_1,uint param_2)
//
// Types and names are Ghidra placeholders;
// rename and retype as the purpose becomes known.
//
// Adjusted from the Ghidra seed by permute_seeds.py (signedness, comparison order, return width) to
// match the original; edit freely, this file is no longer regenerated.

namespace seed_005def80 {

extern "C" __declspec(dllimport) unsigned int __stdcall GetNextDlgTabItem(int, int, int);  // @ref __imp__GetNextDlgTabItem@12=0079a4fc
extern "C" __declspec(dllimport) unsigned int __stdcall GetParent(int);  // @ref __imp__GetParent@4=0079a500
extern "C" __declspec(dllimport) unsigned int __stdcall SetFocus(int);  // @ref __imp__SetFocus@4=0079a38c

bool __fastcall unknown_focus_next_tab_item_005def80(void * window, unsigned int previous)
{
    void * item;

    item = (void *)(GetParent((int)(window)));
    item = (void *)(GetNextDlgTabItem((int)(item), (int)(window), (int)(previous & 0xff)));
    if (item != window) {
        SetFocus((int)(item));
        return 1;
    }
    return 0;
}

}  // namespace seed_005def80
