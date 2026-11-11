// bool __stdcall HoverLocomotionClass::Is_Powered() (0x004FE880, ILocomotion 24)
// Powered, or still in the air (slot 108 positive).
#include "shared/hover_locomotion.hpp"
// @ref ?Is_Powered@LocomotionClass@@UAG_NXZ=0053d230

bool __stdcall HoverLocomotionClass::Is_Powered()
{
    if (LocomotionClass::Is_Powered() || LinkedTo->Get_Height_AGL() > 0) {
        return true;
    }
    return false;
}
