// int RGBClass::Difference(RGBClass const &other) const (0x0063CFB0)
// The weighted distance 2|dRed| + 4|dGreen| + 3|dBlue|.
#include "shared/rgb_class.hpp"

int RGBClass::Difference(RGBClass const &other) const
{
    int red = (int)Red - (int)other.Red;
    if (red < 0) red = -red;
    int green = (int)Green - (int)other.Green;
    if (green < 0) green = -green;
    int blue = (int)Blue - (int)other.Blue;
    if (blue < 0) blue = -blue;
    return blue * 3 + (red + green * 2) * 2;
}
