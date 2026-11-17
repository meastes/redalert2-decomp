// void Unknown_Free_Screen_Surfaces_0068E1C0() -- 0x0068E1C0
// Purpose: once (guarded by the flag at 0x00ABD48D), shuts the 3D device down
// when it is in use (0x004B1470), deletes the six screen surfaces and restores
// the video mode (0x00495E90), then sets the flag (hypothesis: the final
// shutdown of the display).
// @ref ?Release_Direct3D@@YAXXZ=004b1470
// @ref ?Restore_Video_Mode_00495e90@@YAXXZ=00495e90
// @ref ?unknown_flag_abd48d@@3_NA=00abd48d
// @ref ?TileSurface@@3PAVSurface@@A=00839944
#include "shared/dsurface.hpp"
#include "shared/unknown_globals.hpp"

extern Surface *TileSurface;
extern bool unknown_flag_abd48d;
void Release_Direct3D();
void __cdecl Restore_Video_Mode_00495e90();

void Unknown_Free_Screen_Surfaces_0068E1C0()
{
    if (UseDirect3D) {
        Release_Direct3D();
    }
    if (!unknown_flag_abd48d) {
        if (HiddenSurface != 0) {
            delete HiddenSurface;
            HiddenSurface = 0;
        }
        if (AlternateSurface != 0) {
            delete AlternateSurface;
            AlternateSurface = 0;
        }
        if (TileSurface != 0) {
            delete TileSurface;
            TileSurface = 0;
        }
        if (SidebarSurface != 0) {
            delete SidebarSurface;
            SidebarSurface = 0;
        }
        if (CompositeSurface != 0) {
            delete CompositeSurface;
            CompositeSurface = 0;
        }
        if (VisibleSurface != 0) {
            delete VisibleSurface;
            VisibleSurface = 0;
        }
        Restore_Video_Mode_00495e90();
        unknown_flag_abd48d = true;
    }
}
