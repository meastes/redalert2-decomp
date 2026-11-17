// MapClass_Reset_Local_Iterator (0x0055AA50): name from mischa85 names.csv

void __fastcall MapClass_Reset_Local_Iterator(int map_address)
{
    *(int *)(map_address + 0x118) = *(int *)(map_address + 0xf8) + 1 + *(int *)(map_address + 0xfc);
    *(int *)(map_address + 0x11c) =
              (*(int *)(map_address + 0xf0) - *(int *)(map_address + 0xf8)) + *(int *)(map_address + 0xfc);
    return;
}

