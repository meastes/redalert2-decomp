// gauge.cpp: the original translation unit re-filed as one source (Route A): the accepted
// bodies of its canonical compile group in address order, under the unit's own header
// state (its switches and includes in their original order). The module map
// manifests/original_target/modules/gauge.json carries the addresses and the rows that
// stay in their own files.


#include "shared/shape_file.hpp"
#include "shared/gadget.hpp"
#include "shared/mouse.hpp"
#include "shared/text_print.hpp"
#include "shared/color_scheme.hpp"
#include "shared/convert.hpp"
#include "shared/surface.hpp"
#include "shared/unknown_globals.hpp"


// Header-static dynamic initializer of the unit at 0x004D00D0: the empty
// rectangle of shared/shape_file.hpp (0x00866078), the unit's first function.


// GaugeClass::GaugeClass(unsigned id, int x, int y, int w, int h) (0x004D00F0)
// A sticky 0..255 gauge, horizontal when wider than tall, with a thumb.

GaugeClass::GaugeClass(unsigned id, int x, int y, int w, int h) :
    ControlClass(id, x, y, w, h, LEFTPRESS | LEFTHELD | LEFTRELEASE, true)
{
    Set_Maximum(255);
    Set_Value(0);
    HasThumb = true;
    IsHorizontal = (w > h);
    IsColorized = true;
    ClickDiff = 0;
}


// int GaugeClass::Set_Maximum(int value) (0x004D0170, slot 34)

int GaugeClass::Set_Maximum(int value)
{
    if (value != MaxValue) {
        MaxValue = value;
        Flag_To_Redraw();
        return true;
    }
    return false;
}


// int GaugeClass::Set_Value(int value) (0x004D0190, slot 35)
// Clamps value to 0..MaxValue.

int GaugeClass::Set_Value(int value)
{
    int maximum = MaxValue;
    if (value < 0) {
        value = 0;
    } else if (value > maximum) {
        value = maximum;
    }
    if (value != CurValue) {
        CurValue = value;
        Flag_To_Redraw();
        return true;
    }
    return false;
}


// int GaugeClass::Pixel_To_Value(int pixel) (0x004D01C0, slot 40)
// The value at a screen coordinate along the gauge (inside the 1-pixel border).

int GaugeClass::Pixel_To_Value(int pixel)
{
    int maximum;
    if (IsHorizontal) {
        pixel -= X + 1;
        maximum = Width;
    } else {
        pixel -= Y + 1;
        maximum = Height;
    }
    maximum -= 2;
    if (pixel < 0) {
        pixel = 0;
    } else if (pixel > maximum) {
        pixel = maximum;
    }
    return (int)((double)pixel / maximum * MaxValue);
}


// int GaugeClass::Value_To_Pixel(int value) (0x004D0240, slot 41)
// The screen coordinate of a value along the gauge.

int GaugeClass::Value_To_Pixel(int value)
{
    int start;
    int maximum;
    if (IsHorizontal) {
        maximum = Width;
        start = X;
    } else {
        maximum = Height;
        start = Y;
    }
    maximum -= 2;
    return (int)((double)value / MaxValue * maximum + start);
}


// int GaugeClass::Draw_Me(int forced) (0x004D0280, slot 27)
// A box; a colorized gauge fills it up to the value in the gadget colour
// scheme's colour; then the thumb.

int GaugeClass::Draw_Me(int forced)
{
    if (ControlClass::Draw_Me(forced)) {
        WWMouse->Conditional_Hide_Mouse(Rect(X, Y, Width, Height));
        Draw_Box(Rect(X, Y, Width, Height), 0, true);
        if (IsColorized) {
            int middle = Value_To_Pixel(CurValue);
            int color = ColorSchemes[Gadget_Color_Scheme()]->unknown_color_0x330;
            if (IsHorizontal) {
                if (middle >= X + 1) {
                    unknown_surface_83995c->Fill_Rect(Rect(X + 1, Y + 1, middle - X - 1, Height - 2), unknown_drawer_8323c4->Convert(color));
                }
            } else {
                if (middle >= Y + 1) {
                    unknown_surface_83995c->Fill_Rect(Rect(X + 1, Y + 1, Width - 2, middle - Y - 1), unknown_drawer_8323c4->Convert(color));
                }
            }
        }
        if (HasThumb) {
            Draw_Thumb();
        }
        WWMouse->Conditional_Show_Mouse();
        return true;
    }
    return false;
}


// int GaugeClass::Action(unsigned flags, unsigned &key) (0x004D0420, slot 31)
// Dragging the thumb sets the value under the mouse (minus the grab offset
// within the thumb). An unchanged value consumes the input.

int GaugeClass::Action(unsigned flags, unsigned &key)
{
    if (!HasThumb) {
        return false;
    }
    Sticky_Process(flags);
    if ((flags & LEFTPRESS) || ((flags & LEFTHELD) && StuckOn == this)) {
        if (flags & LEFTPRESS) {
            int curpix = Value_To_Pixel(CurValue);
            int clickpix = IsHorizontal ? WWMouse->Get_Mouse_X() : WWMouse->Get_Mouse_Y();
            if (clickpix > curpix && clickpix - curpix < Thumb_Pixels()) {
                ClickDiff = clickpix - curpix;
            } else {
                ClickDiff = 0;
            }
            int testval = Pixel_To_Value(IsHorizontal ? WWMouse->Get_Mouse_X() - ClickDiff : WWMouse->Get_Mouse_Y() - ClickDiff);
            while (testval < CurValue && ClickDiff > 0) {
                ClickDiff--;
                testval = Pixel_To_Value(IsHorizontal ? WWMouse->Get_Mouse_X() - ClickDiff : WWMouse->Get_Mouse_Y() - ClickDiff);
            }
        }
        if (!Set_Value(Pixel_To_Value(IsHorizontal ? WWMouse->Get_Mouse_X() - ClickDiff : WWMouse->Get_Mouse_Y() - ClickDiff))) {
            ControlClass::Action(0, key);
            key = 0;
            return true;
        }
    } else {
        flags &= ~LEFTHELD;
    }
    return ControlClass::Action(flags, key);
}


// void GaugeClass::Draw_Thumb() (0x004D0580, slot 39)
// A raised box four pixels long at the current value, kept inside the gauge.

void GaugeClass::Draw_Thumb()
{
    int x = Value_To_Pixel(CurValue);
    if (x + 4 > Value_To_Pixel(MaxValue)) {
        x = Value_To_Pixel(MaxValue) - 2;
    }
    if (x < X) {
        x = X;
    }
    if (IsHorizontal) {
        Draw_Box(Rect(x, Y, 4, Height), 1, true);
    } else {
        Draw_Box(Rect(X, x, Width, 4), 1, true);
    }
}


// TriColorGaugeClass::TriColorGaugeClass(unsigned id, int x, int y, int w, int h) (0x004D0630)

TriColorGaugeClass::TriColorGaugeClass(unsigned id, int x, int y, int w, int h) :
    GaugeClass(id, x, y, w, h),
    RedLimit(0),
    YellowLimit(0)
{
}


// int TriColorGaugeClass::Set_Red_Limit(int value) (0x004D06D0, slot 42)

int TriColorGaugeClass::Set_Red_Limit(int value)
{
    if (value >= 0 && value < MaxValue) {
        RedLimit = value;
        Flag_To_Redraw();
        return true;
    }
    return false;
}


// int TriColorGaugeClass::Set_Yellow_Limit(int value) (0x004D0700, slot 43)

int TriColorGaugeClass::Set_Yellow_Limit(int value)
{
    if (value >= 0 && value < MaxValue) {
        YellowLimit = value;
        Flag_To_Redraw();
        return true;
    }
    return false;
}


// int TriColorGaugeClass::Draw_Me(int forced) (0x004D0730, slot 27)
// Redraws the box (raised when disabled) and fills it in up to three colours:
// up to the red limit, the yellow limit and the value. Several rectangles
// keep the original's mixed corner/size arithmetic.

int TriColorGaugeClass::Draw_Me(int forced)
{
    if (GaugeClass::Draw_Me(forced)) {
        WWMouse->Conditional_Hide_Mouse(Rect(X, Y, Width, Height));
        Draw_Box(Rect(X, Y, Width, Height), IsDisabled ? 1 : 0, true);
        int red = Value_To_Pixel(RedLimit);
        int yellow = Value_To_Pixel(YellowLimit);
        int middle = Value_To_Pixel(CurValue);
        if (CurValue <= RedLimit) {
            if (IsHorizontal) {
                unknown_surface_83995c->Fill_Rect(Rect(X + 1, Y + 1, middle - X + 1, Height - 2), unknown_drawer_8323c4->Convert(6));
            } else {
                unknown_surface_83995c->Fill_Rect(Rect(X + 1, Y + 1, Width - 2, middle - Y + 1), unknown_drawer_8323c4->Convert(6));
            }
        } else if (CurValue > RedLimit && CurValue <= YellowLimit) {
            if (IsHorizontal) {
                unknown_surface_83995c->Fill_Rect(Rect(X + 1, Y + 1, red - X + 1, Height - 2), unknown_drawer_8323c4->Convert(6));
                unknown_surface_83995c->Fill_Rect(Rect(red - X + 1, Y + 1, middle - X - 1, Height - 2), unknown_drawer_8323c4->Convert(5));
            } else {
                unknown_surface_83995c->Fill_Rect(Rect(X + 1, Y + 1, Width - 2, red - Y + 1), unknown_drawer_8323c4->Convert(6));
                unknown_surface_83995c->Fill_Rect(Rect(X + 1, red - Y + 1, X + Width - 2, middle - Y + 1), unknown_drawer_8323c4->Convert(5));
            }
        } else if (CurValue > YellowLimit && CurValue <= MaxValue) {
            if (IsHorizontal) {
                unknown_surface_83995c->Fill_Rect(Rect(X + 1, Y + 1, red - X + 1, Height - 2), unknown_drawer_8323c4->Convert(6));
                unknown_surface_83995c->Fill_Rect(Rect(red, Y + 1, yellow - X + 1, Height - 2), unknown_drawer_8323c4->Convert(5));
                unknown_surface_83995c->Fill_Rect(Rect(yellow, Y + 1, middle - X + 1, Height - 2), unknown_drawer_8323c4->Convert(3));
            } else {
                unknown_surface_83995c->Fill_Rect(Rect(X + 1, Y + 1, Width - 2, red - Y + 1), unknown_drawer_8323c4->Convert(6));
                unknown_surface_83995c->Fill_Rect(Rect(X + 1, red, Width - 2, yellow - Y + 1), unknown_drawer_8323c4->Convert(5));
                unknown_surface_83995c->Fill_Rect(Rect(X + 1, yellow, Width - 2, middle - Y + 1), unknown_drawer_8323c4->Convert(3));
            }
        }
        if (HasThumb) {
            Draw_Thumb();
        }
        WWMouse->Conditional_Show_Mouse();
        return true;
    }
    return false;
}


// GaugeClass::Get_Value (0x004D0C80)
// Compiler-emitted from shared/gadget.hpp in this unit; no source of its own.


// GaugeClass::Use_Thumb (0x004D0C90)
// Compiler-emitted from shared/gadget.hpp in this unit; no source of its own.


// GaugeClass::Thumb_Pixels (0x004D0CA0)
// Compiler-emitted from shared/gadget.hpp in this unit; no source of its own.


// GaugeClass::scalar_deleting_destructor (0x004D0CB0)
// Compiler-emitted from shared/gadget.hpp in this unit; no source of its own.


// TriColorGaugeClass::scalar_deleting_destructor (0x004D0CD0)
// Compiler-emitted from shared/gadget.hpp in this unit; no source of its own.
