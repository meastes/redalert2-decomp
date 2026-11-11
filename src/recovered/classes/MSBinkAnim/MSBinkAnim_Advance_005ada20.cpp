// bool MSBinkAnim::Advance(Surface *surface, Rect *dirty) (0x005ADA20, slot 4)
// Shows each new movie frame: draws it, reports the movie rectangle and
// redraws every other animation of the list inside it. True once the movie
// finished unless unknown_38.
#include "shared/ms_anim.hpp"
// @ref ?Render_Frame@BinkClass@@QAE_NXZ=00430800

bool MSBinkAnim::Advance(Surface *surface, Rect *dirty)
{
    if (surface == 0) {
        return false;
    }
    if (Movie == 0) {
        return true;
    }
    if (!Movie->Is_Finished() && Movie->Render_Frame()) {
        Draw(surface, 0);
        *dirty = MovieRect;
        int index = Anims->ID(this);
        for (int i = 0; i < Anims->Count(); i++) {
            if (i != index) {
                (*Anims)[i]->Draw(surface, &MovieRect);
            }
        }
    }
    return Movie->Is_Finished() && !unknown_38;
}
