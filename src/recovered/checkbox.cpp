// checkbox.cpp: the original translation unit re-filed as one source (Route A): the accepted
// bodies of its canonical compile group in address order, under the unit's own header
// state (its switches and includes in their original order). The module map
// manifests/original_target/modules/checkbox.json carries the addresses and the rows that
// stay in their own files.


#include "shared/gadget.hpp"
#include "shared/rect.hpp"
#include "shared/convert.hpp"
#include "shared/mouse.hpp"
#include "shared/text_print.hpp"
#include "shared/unknown_globals.hpp"


// int CheckBoxClass::Draw_Me(int forced) (0x0047D150; TibSun checkbox.cpp): when the
// control draws, hides the mouse, draws the sunken box, fills its face grey (palette
// index 13) and, when on, green (index 4), then shows the mouse again.

int CheckBoxClass::Draw_Me(int forced)
{
    if (ControlClass::Draw_Me(forced)) {
        WWMouse->Hide_Mouse();
        Draw_Box(Rect(X, Y, Width, Height), 0, false);
        unknown_surface_83995c->Fill_Rect(Rect(X + 1, Y + 1, Width - 1, Height - 1), unknown_drawer_8323c4->Convert(13));
        if (IsOn) {
            unknown_surface_83995c->Fill_Rect(Rect(X + 1, Y + 1, Width - 1, Height - 1), unknown_drawer_8323c4->Convert(4));
        }
        WWMouse->Show_Mouse();
        return true;
    }
    return false;
}


// int CheckBoxClass::Action(unsigned flags, unsigned &key) (0x0047D270; TibSun
// checkbox.cpp): a left release (flag 4) flips the state, then ToggleClass::Action.

int CheckBoxClass::Action(unsigned flags, unsigned &key)
{
    if (flags & 4) {
        if (IsOn) {
            Turn_Off();
        } else {
            Turn_On();
        }
    }
    return ToggleClass::Action(flags, key);
}
