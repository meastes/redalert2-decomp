// unknown_type_flag_aab_0041b3f0 (0x0041B3F0): no name evidence; named by what it does
#include "shared/ghidra_types.hpp"

BYTE __fastcall unknown_type_flag_aab_0041b3f0(int *aircraft_words)
{
    int type_address;

    type_address = (int)((**(code **)(*aircraft_words + 0x80))());
    return *(BYTE *)(type_address + 0xaab);
}

