// void TriggerClass::Enable() (0x006EDB60)
#include "shared/object.hpp"

void TriggerClass::Enable()
{
    IsActive = true;
    Reset_All_Timed_Events();
}
