// FindSampleIndex_004015c0 (0x004015C0)
#include "shared/ghidra_types.hpp"

extern void __cdecl stricmp_00781f20(void);
extern DWORD  bsearch_00782025(DWORD key, uint entries, uint entry_count, int entry_width, code *comparator);

uint __fastcall FindSampleIndex_004015c0(int *bag_words, DWORD name_address)
{
    int entry_address;

    entry_address = (int)(bsearch_00782025((DWORD)(name_address), (uint)(*bag_words), (uint)(bag_words[1]), (int)(0x24), (code *)(stricmp_00781f20)));
    if (entry_address != 0) {
        return (uint)(entry_address - *bag_words) / 0x24;
    }
    return 0xffffffff;
}

