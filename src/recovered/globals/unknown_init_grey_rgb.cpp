// Dynamic initializer of the RGBClass constant at 0x00AB6844, (127, 127, 127) (0x0063CB80; .CRT$XCU 0x007CB450 group of
// the RGB unit, written as a plain function as 0x0063CBC0 is; the constant's name is not recovered).
#include "shared/rgb_class.hpp"

extern RGBClass unknown_rgb_ab6844;

void unknown_init_grey_rgb_0063cb80()
{
    unknown_rgb_ab6844.Set(127, 127, 127);
}
