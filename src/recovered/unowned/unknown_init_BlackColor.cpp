// Dynamic initializer of the RGBClass constant at 0x00AB6841, (0, 0, 0) (0x0063CB60; .CRT$XCU 0x007CB450 group of
// the RGB unit, written as a plain function as 0x0063CBC0 is; the constant's name is not recovered).
#include "shared/rgb_class.hpp"
// @ref ?BlackColor@@3VRGBClass@@A=00ab6841

extern RGBClass BlackColor;

void unknown_init_BlackColor_0063cb60()
{
    BlackColor.Set(0, 0, 0);
}
