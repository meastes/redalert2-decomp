// Behaviour row for native M23 (Scout1187). NOT EXACT.
#include "shared/gadget.hpp"

// 0x00539F30 ListClass::Set_Position (slot 25; pass-6a decompile, offsets of
// gadget.hpp's layout): moves the scroll buttons and the slider to the list's right
// edge for a list at x, y (the list's own position is left as it is, as there).
void ListClass::Set_Position(int x, int y)
{
    UpGadget.X = x + Width - UpGadget.Width;
    UpGadget.Y = y;
    DownGadget.X = x + Width - DownGadget.Width;
    DownGadget.Y = y + Height - DownGadget.Height;
    int width = UpGadget.Width > DownGadget.Width ? UpGadget.Width : DownGadget.Width;
    ScrollGadget.X = x + Width - width;
    ScrollGadget.Y = y + UpGadget.Height;
    ScrollGadget.Height = Height - DownGadget.Height - UpGadget.Height;
    ScrollGadget.Width = width;
}
