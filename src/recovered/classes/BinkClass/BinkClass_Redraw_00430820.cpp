// void BinkClass::Redraw() (0x00430820)
// Copies the current frame into the target surface again at the movie area
// (offset by the window client area on the screen surface).
#include "shared/movie.hpp"

void BinkClass::Redraw()
{
    if (Target == unknown_surface_839950) {
        RECT client;
        GetClientRect(MainWindow, &client);
        ClientToScreen(MainWindow, (POINT *)&client);
        int x = Area.X + client.left;
        int y = Area.Y + client.top;
        Redraw_Frame(Target, x, y);
    } else {
        Redraw_Frame(Target, Area.X, Area.Y);
    }
}
