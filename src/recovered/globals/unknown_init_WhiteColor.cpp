// Dynamic initializer of the RGBClass constant at 0x00AB6849, (255, 255, 255) (0x0063CBA0; .CRT$XCU 0x007CB450 group of
// the RGB unit, written as a plain function as 0x0063CBC0 is; the constant's name is not recovered).
#include "shared/rgb_class.hpp"
// @ref ?WhiteColor@@3VRGBClass@@A=00ab6849

extern RGBClass WhiteColor;

void unknown_init_WhiteColor_0063cba0()
{
    WhiteColor.Set(255, 255, 255);
}
