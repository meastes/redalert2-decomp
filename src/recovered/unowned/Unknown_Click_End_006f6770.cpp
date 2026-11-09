// void Unknown_Click_End_006F6770() (0x006F6770): clears the flag at 0x00AC1330
// (Mouse_Left_Release 0x0049BB20 calls it after a placement or selection click).
#include "shared/uicmnds.hpp"

void Unknown_Click_End_006F6770()
{
    unknown_flag_ac1330 = false;
}
