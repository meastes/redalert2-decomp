// Dynamic initializer of the RGBClass constant at 0x00AB682C, (0, 0, 255) (0x0063CC00; .CRT$XCU 0x007CB450 group of
// the RGB unit, written as a plain function as 0x0063CBC0 is; the constant's name is not recovered).
#include "shared/rgb_class.hpp"

extern RGBClass unknown_rgb_ab682c;

void unknown_init_blue_rgb_0063cc00()
{
    unknown_rgb_ab682c.Set(0, 0, 255);
}
