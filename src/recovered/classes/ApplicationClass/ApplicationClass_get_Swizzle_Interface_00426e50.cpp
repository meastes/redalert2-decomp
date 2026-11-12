// HRESULT ApplicationClass::get_Swizzle_Interface(ISwizzle **swizzle) (0x00426E50): the save-game
// swizzler's ISwizzle interface (name from mischa85/RedAlert2 src/app.cpp).
#include "shared/application.hpp"
#include "shared/swizzle.hpp"

HRESULT __stdcall ApplicationClass::get_Swizzle_Interface(ISwizzle **swizzle)
{
    return Swizzler.QueryInterface(IID_ISwizzle, (void **)swizzle);
}
