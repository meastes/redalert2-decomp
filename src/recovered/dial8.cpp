// dial8.cpp: the original translation unit re-filed as one source (Route A): the accepted
// bodies of its canonical compile group in address order, under the unit's own header
// state (its switches and includes in their original order). The module map
// manifests/original_target/modules/dial8.json carries the addresses and the rows that
// stay in their own files.


#include "shared/dial8.hpp"
#include "shared/techno.hpp"
#include "shared/mouse.hpp"
#include "shared/fast_math.hpp"


Dial8Class::Dial8Class(unsigned id, int x, int y, int width, int height, unsigned direction)
 : ControlClass(id, x, y, width, height, 7, true)
{
    Center.X = Width / 2 + X;
    Center.Y = Height / 2 + Y;
    Direction = direction;
    Selected = Dir_To_8(UnknownFacingDir((unsigned short)(direction << 8)));
    Previous = Selected;
    OuterPoints[0].X = Center.X;
    OuterPoints[0].Y = Center.Y - height * 8 / 2 / 10;
    OuterPoints[1].X = Center.X + width * 56 / 2 / 100;
    OuterPoints[1].Y = Center.Y - height * 56 / 2 / 100;
    OuterPoints[2].X = Center.X + width * 8 / 2 / 10;
    OuterPoints[2].Y = Center.Y;
    OuterPoints[3].X = Center.X + width * 56 / 2 / 100;
    OuterPoints[3].Y = Center.Y + height * 56 / 2 / 100;
    OuterPoints[4].X = Center.X;
    OuterPoints[4].Y = Center.Y + height * 8 / 2 / 10;
    OuterPoints[5].X = Center.X - width * 56 / 2 / 100;
    OuterPoints[5].Y = Center.Y + height * 56 / 2 / 100;
    OuterPoints[6].X = Center.X - width * 8 / 2 / 10;
    OuterPoints[6].Y = Center.Y;
    OuterPoints[7].X = Center.X - width * 56 / 2 / 100;
    OuterPoints[7].Y = Center.Y - height * 56 / 2 / 100;
    InnerPoints[0].X = Center.X;
    InnerPoints[0].Y = Center.Y - height * 6 / 2 / 10;
    InnerPoints[1].X = Center.X + width * 42 / 2 / 100;
    InnerPoints[1].Y = Center.Y - height * 42 / 2 / 100;
    InnerPoints[2].X = Center.X + width * 6 / 2 / 10;
    InnerPoints[2].Y = Center.Y;
    InnerPoints[3].X = Center.X + width * 42 / 2 / 100;
    InnerPoints[3].Y = Center.Y + height * 42 / 2 / 100;
    InnerPoints[4].X = Center.X;
    InnerPoints[4].Y = Center.Y + height * 6 / 2 / 10;
    InnerPoints[5].X = Center.X - width * 42 / 2 / 100;
    InnerPoints[5].Y = Center.Y + height * 42 / 2 / 100;
    InnerPoints[6].X = Center.X - width * 6 / 2 / 10;
    InnerPoints[6].Y = Center.Y;
    InnerPoints[7].X = Center.X - width * 42 / 2 / 100;
    InnerPoints[7].Y = Center.Y - height * 42 / 2 / 100;
}


extern int unknown_dial_drag_00852478;
inline UnknownFacingDir Dial_Direction(Point2D from, Point2D to)
{
    return UnknownFacingDir((unsigned short)((Fast_Atan2((double)from.Y - to.Y, (double)to.X - from.X) - 1.5707963267948966) * -10430.060040584269));
}
inline void Dial8Class::Update_Direction(unsigned direction)
{
    Direction = direction;
    Selected = Dir_To_8(UnknownFacingDir((unsigned short)(direction << 8)));
}
int Dial8Class::Action(unsigned flags, unsigned &key)
{
    Sticky_Process(flags);
    if (flags & LEFTPRESS) unknown_dial_drag_00852478 = 1;
    if ((flags & LEFTPRESS) || ((flags & LEFTHELD) && unknown_dial_drag_00852478)) {
        Update_Direction(Dir_To_256(Dial_Direction(Center, WWMouse->Get_Mouse_XY())));
        if (Selected != Previous) {
            Previous = Selected;
            ControlClass::Action(flags, key);
        } else {
            key = 0;
            ControlClass::Action(0, key);
        }
        return 1;
    }
    if (flags & LEFTRELEASE) {
        key = 0;
        unknown_dial_drag_00852478 = 0;
    }
    return ControlClass::Action(0, key);
}


int Dial8Class::Draw_Me(int forced)
{
    int scheme = Gadget_Color_Scheme();
    if (ControlClass::Draw_Me(forced)) {
        WWMouse->Hide_Mouse();
        Draw_Box(Rect(X, Y, Width, Height), 0, true);
        for (int i = 0; i < 8; ++i) {
            Draw_Box(Rect(OuterPoints[i].X - 1, OuterPoints[i].Y - 1, 3, 3), 1, false);
        }
        int color = unknown_drawer_8323c4->Convert(ColorSchemes[scheme]->unknown_color_0x31c);
        unknown_surface_83995c->Draw_Line(Point2D(Center.X + 1, Center.Y + 1), Point2D(InnerPoints[Selected].X + 1, InnerPoints[Selected].Y + 1), color);
        color = unknown_drawer_8323c4->Convert(ColorSchemes[scheme]->unknown_color_0x328);
        unknown_surface_83995c->Draw_Line(Point2D(Center.X, Center.Y), Point2D(InnerPoints[Selected].X, InnerPoints[Selected].Y), color);
        WWMouse->Show_Mouse();
        return 1;
    }
    return 0;
}


void Dial8Class::Set_Direction(unsigned direction)
{
    Update_Direction(direction);
    Previous = Selected;
    Flag_To_Redraw();
}


// 0x004972d0: function not yet identified
//
// Body: 7 bytes (frozen inventory bound).
// Referenced from 0 sites.
// Ghidra prototype: undefined4 __fastcall FUN_004972d0(int param_1)
//
// Types and names are Ghidra placeholders;
// rename and retype as the purpose becomes known.
//

namespace seed_004972d0 {

unsigned int __fastcall FUN_004972d0(int object_address)
{
    return *(unsigned int *)(object_address + 0xb4);
}

}  // namespace seed_004972d0
