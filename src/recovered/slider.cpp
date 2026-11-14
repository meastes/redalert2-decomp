// slider.cpp: the original translation unit re-filed as one source (Route A): the accepted
// bodies of its canonical compile group in address order, under the unit's own header
// state (its switches and includes in their original order). The module map
// manifests/original_target/modules/slider.json carries the addresses and the rows that
// stay in their own files.


#include "shared/gadget.hpp"
#include "shared/mix_file_class.hpp"
#include "shared/rect.hpp"
#include "shared/mouse.hpp"


// SliderClass::SliderClass(unsigned id, int x, int y, int w, int h, int no_buttons) (0x006846E0)
// A gauge with a thumb; unless no_buttons, a plus button right of it and a minus button left of it
// become its peers. The thumb starts one value wide.

SliderClass::SliderClass(unsigned id, int x, int y, int w, int h, int no_buttons)
    : GaugeClass(id, x, y, w, h)
{
    NoButtons = no_buttons ? true : false;

    PlusGadget = 0;
    MinusGadget = 0;
    if (!NoButtons) {
        PlusGadget = new ShapeButtonClass(id, MixFileClass::Retrieve("BTN-PLUS.SHP"), X + Width + 2, Y);
        MinusGadget = new ShapeButtonClass(id, MixFileClass::Retrieve("BTN-MINS.SHP"), X - 6, Y);

        if (PlusGadget) {
            PlusGadget->Make_Peer(*this);
            PlusGadget->Add(*this);
            PlusGadget->Flag_To_Redraw();
        }
        if (MinusGadget) {
            MinusGadget->Make_Peer(*this);
            MinusGadget->Add(*this);
            MinusGadget->Flag_To_Redraw();
        }
    }
    Set_Thumb_Size(1);
    Recalc_Thumb();

    IsColorized = 0;
}


// SliderClass::~SliderClass() (0x006848C0)
// Deletes the plus and minus buttons.

SliderClass::~SliderClass()
{
    if (PlusGadget != 0) {
        delete PlusGadget;
        PlusGadget = 0;
    }
    if (MinusGadget != 0) {
        delete MinusGadget;
        MinusGadget = 0;
    }
}


// int SliderClass::Set_Maximum(int value) (0x00684900, slot 34)

int SliderClass::Set_Maximum(int value)
{
    if (GaugeClass::Set_Maximum(value)) {
        Recalc_Thumb();
        return true;
    }
    return false;
}


// void SliderClass::Set_Thumb_Size(int value) (0x00684980, slot 42)
// Thumb becomes value, at most MaxValue and at least 1.

void SliderClass::Set_Thumb_Size(int value)
{
    Thumb = (value < MaxValue) ? value : MaxValue;
    Thumb = (Thumb > 1) ? Thumb : 1;
    Flag_To_Redraw();
    Recalc_Thumb();
}


// int SliderClass::Set_Value(int value) (0x00684A10, slot 35)
// The value is kept within MaxValue - Thumb.

int SliderClass::Set_Value(int value)
{
    int maximum = MaxValue - Thumb;
    if (value >= maximum) {
        value = maximum;
    }
    if (GaugeClass::Set_Value(value)) {
        Recalc_Thumb();
        return true;
    }
    return false;
}


// void SliderClass::Recalc_Thumb() (0x00684AA0)
// The thumb's pixel size (at least 4) and start for the current value.

void SliderClass::Recalc_Thumb()
{
    int length = IsHorizontal ? Width : Height;
    int size = (int)((double)Thumb / MaxValue * length);
    ThumbSize = (size > 4) ? size : 4;
    int start = (int)((double)CurValue / MaxValue * length);
    ThumbStart = (start < length - ThumbSize) ? start : length - ThumbSize;
}


// int SliderClass::Action(unsigned flags, unsigned &key) (0x00684B10, slot 31)
// A left press before the thumb bumps the value down a thumb, one after it bumps it up;
// a press on the thumb itself goes to the gauge. The key is consumed either way.

int SliderClass::Action(unsigned flags, unsigned &key)
{
    if (flags & LEFTPRESS) {
        int mouse;
        int edge;

        if (IsHorizontal) {
            mouse = WWMouse->Get_Mouse_X();
            edge = X;
        } else {
            mouse = WWMouse->Get_Mouse_Y();
            edge = Y;
        }
        edge += 1;

        if (mouse < edge + ThumbStart) {
            Bump(true);
            GaugeClass::Action(0, key);
            key = 0;
            return true;
        } else {
            if (mouse > edge + ThumbStart + ThumbSize) {
                Bump(false);
                GaugeClass::Action(0, key);
                key = 0;
                return true;
            } else {
                GaugeClass::Action(flags, key);
                key = 0;
                return true;
            }
        }
    }

    return GaugeClass::Action(flags, key);
}


// int SliderClass::Bump(int up) (0x00684BB0, slot 43)
// Moves the value by a whole thumb.

int SliderClass::Bump(int up)
{
    if (up) {
        return Set_Value(CurValue - Thumb);
    } else {
        return Set_Value(Thumb + CurValue);
    }
}


// int SliderClass::Step(int up) (0x00684BF0, slot 44)
// Moves the value by one.

int SliderClass::Step(int up)
{
    if (up) {
        return Set_Value(CurValue - 1);
    } else {
        return Set_Value(CurValue + 1);
    }
}


// void SliderClass::Draw_Thumb() (0x00684C20, slot 39)

void SliderClass::Draw_Thumb()
{
    if (IsHorizontal) {
        Draw_Box(Rect(X + ThumbStart, Y, ThumbSize, Height), 1, true);
    } else {
        Draw_Box(Rect(X, Y + ThumbStart, Width, ThumbSize), 1, true);
    }
}


// int SliderClass::Draw_Me(int forced) (0x00684CA0, slot 27)

int SliderClass::Draw_Me(int forced)
{
    if (NoButtons && ControlClass::Draw_Me(forced)) {
        Draw_Box(Rect(X, Y, Width, Height), 0, true);
        Draw_Thumb();
        return true;
    }
    return GaugeClass::Draw_Me(forced);
}


// void SliderClass::Peer_To_Peer(unsigned flags, unsigned &key, ControlClass &whom) (0x00684D10, slot 19)
// Releasing the plus button steps up the value, the minus button down.

void SliderClass::Peer_To_Peer(unsigned flags, unsigned &key, ControlClass &whom)
{
    if (flags & LEFTRELEASE) {
        if (&whom == PlusGadget) {
            Step(false);
        }
        if (&whom == MinusGadget) {
            Step(true);
        }
    }
}
