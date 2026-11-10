// bool BinkClass::Render_Frame() (0x00430800): draws the frame onto the target.
#include "shared/movie.hpp"

bool BinkClass::Render_Frame()
{
    return Draw_Frame(Target, Area.X, Area.Y);
}
// @ref ?Open@BinkClass@@QAE_NPBD@Z=0042ff30
// @ref ?Draw_Frame@BinkClass@@QAE_NPAVSurface@@HH@Z=00430600
