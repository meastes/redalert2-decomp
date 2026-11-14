// bar.cpp: the original translation unit re-filed as one source (Route A): the accepted
// bodies of its canonical compile group in address order, under the unit's own header
// state (its switches and includes in their original order). The module map
// manifests/original_target/modules/bar.json carries the addresses and the rows that
// stay in their own files.


#include "shared/shape_file.hpp"
#include "shared/progress_bar.hpp"
#include "shared/gadget.hpp"
#include "shared/mouse.hpp"
#include "shared/surface.hpp"
#include "shared/unknown_globals.hpp"


// Dynamic initializer (0x0042B950, _$E2) of the file-local empty Rect that
// shared/shape_file.hpp defines in BAR.CPP; the unit's first source.


// ProgressBarClass::ProgressBarClass (0x0042B970), Westwood's BAR.CPP.

ProgressBarClass::ProgressBarClass(int x, int y, int width, int height, int forecolor, int backcolor,
                                   int bordercolor) :
    X(x),
    Y(y),
    Width(width),
    Height(height),
    BarColor(forecolor),
    BackColor(backcolor),
    BorderColor(bordercolor),
    CurrentValue(0),
    LastDisplayCurrent(0),
    IsDrawn(false)
{
}


// ProgressBarClass::Is_Horizontal (0x0042B9C0): the bar fills along its longer side.

bool ProgressBarClass::Is_Horizontal() const
{
    if (Width > Height) return true;
    return false;
}


// ProgressBarClass::Update (0x0042B9D0): redraws when the value moved by a tenth or more.

bool ProgressBarClass::Update(double value)
{
    CurrentValue = value;
    if (!IsDrawn || value - LastDisplayCurrent >= 0.1) {
        Redraw();
        return true;
    }
    return false;
}


// ProgressBarClass::Outline (0x0042BA10): the border rectangle, when the bar has one.

void ProgressBarClass::Outline() const
{
    if (Is_Outlined()) {
        unknown_surface_83995c->Draw_Rect(Rect(X, Y, Width, Height), unknown_drawer_8323c4->Convert(BorderColor));
    }
}


// ProgressBarClass::Redraw (0x0042BA80): border, filled part, then the rest.

void ProgressBarClass::Redraw() const
{
    WWMouse->Hide_Mouse();
    Outline();

    int x = X;
    int y = Y;
    int w = Width;
    int h = Height;
    if (Is_Outlined()) {
        x += 1;
        y += 1;
        w -= 2;
        h -= 2;
    }

    int size = Is_Horizontal() ? w : h;
    int fill = CurrentValue * size;

    if (fill > 0) {
        if (Is_Horizontal()) {
            unknown_surface_83995c->Fill_Rect(Rect(x, y, fill, h), unknown_drawer_8323c4->Convert(BarColor));
        } else {
            unknown_surface_83995c->Fill_Rect(Rect(x, y + fill, w, h), unknown_drawer_8323c4->Convert(BarColor));
        }
    }

    if (w - fill > 0) {
        if (Is_Horizontal()) {
            unknown_surface_83995c->Fill_Rect(Rect(x + fill, y, w, h), unknown_drawer_8323c4->Convert(BackColor));
        } else {
            unknown_surface_83995c->Fill_Rect(Rect(x, y, w, fill - 1), unknown_drawer_8323c4->Convert(BackColor));
        }
    }
    WWMouse->Show_Mouse();

    ProgressBarClass *me = (ProgressBarClass *)this;
    me->LastDisplayCurrent = CurrentValue;
    me->IsDrawn = true;
}
