// void Unknown_Free_Screen_Surfaces_0068E1C0() -- 0x0068E1C0
// Purpose: once (guarded by the flag at 0x00ABD48D), shuts the 3D device down
// when it is in use (0x004B1470), deletes the six screen surfaces and restores
// the video mode (0x00495E90), then sets the flag (hypothesis: the final
// shutdown of the display).
// @ref ?Unknown_004B1470@@YAXXZ=004b1470
// @ref ?Restore_Video_Mode_00495e90@@YAXXZ=00495e90
// @ref ?unknown_flag_abd48d@@3_NA=00abd48d
// @ref ?unknown_surface_839944@@3PAVSurface@@A=00839944
#include "shared/dsurface.hpp"
#include "shared/unknown_globals.hpp"

extern Surface *unknown_surface_839944;
extern bool unknown_flag_abd48d;
void Unknown_004B1470();
void __cdecl Restore_Video_Mode_00495e90();

void Unknown_Free_Screen_Surfaces_0068E1C0()
{
    if (UseDirect3D) {
        Unknown_004B1470();
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
        if (unknown_surface_839944 != 0) {
            delete unknown_surface_839944;
            unknown_surface_839944 = 0;
        }
        if (SidebarSurface != 0) {
            delete SidebarSurface;
            SidebarSurface = 0;
        }
        if (unknown_surface_839964 != 0) {
            delete unknown_surface_839964;
            unknown_surface_839964 = 0;
        }
        if (VisibleSurface != 0) {
            delete VisibleSurface;
            VisibleSurface = 0;
        }
        Restore_Video_Mode_00495e90();
        unknown_flag_abd48d = true;
    }
}
