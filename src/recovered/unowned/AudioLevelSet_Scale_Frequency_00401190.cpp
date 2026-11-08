// int AudioLevelSet::Scale_Frequency(int value) (0x00401190): value scaled by the frequency level's
// percent (Frequency.Current, 16.16); the product is unsigned, the division by 100 signed.
#include "shared/audio.hpp"

int __fastcall AudioLevelSet::Scale_Frequency(int value)
{
    return (int)((Frequency.Current >> 16) * value) / 100;
}
