// int __fastcall unknown_ini_8397a0_unique_id() -- 0x00652720
// Purpose: the CRC identifier of the INI at 0x008397A0 (the rules INI; hypothesis) (CCINIClass::Get_Unique_ID,
// 0x0046C1C0), a tail call. Callers 0x005960D9, 0x00598EE2, 0x005BD089 and
// others compare it (multiplayer synchronisation; hypothesis).
// Assumptions: the function's name is a placeholder.
#include "shared/ccini_class.hpp"

extern CCINIClass AIINI;      // 0x008397A0

int __fastcall unknown_ini_8397a0_unique_id()
{
    return AIINI.Get_Unique_ID();
}
