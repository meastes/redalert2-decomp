// int AudioLevel::Get_Target() (0x004071E0): the target level, whole part (16.16 value).
// mischa85 audlevel.cpp AudioLevelGetVal.

#include "shared/audio.hpp"

int __fastcall AudioLevel::Get_Target()
{
    return Target >> 16;
}
