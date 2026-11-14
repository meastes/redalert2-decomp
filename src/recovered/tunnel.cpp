// tunnel.cpp: the original translation unit re-filed as one source (Route A): the accepted
// bodies of its canonical compile group in address order, under the unit's own header
// state (its switches and includes in their original order). The module map
// manifests/original_target/modules/tunnel.json carries the addresses and the rows that
// stay in their own files.


#define RA2_COORD_FROM_CELL_INLINE
#include "shared/unknown_geometry.hpp"
#include "shared/unknown_static_rect.hpp"
#include "shared/tunnel_locomotion.hpp"
#include "shared/foot.hpp"
#include "shared/map_view.hpp"
#include "shared/rules.hpp"
#include "shared/house.hpp"
#include "shared/special.hpp"
#include "shared/id_source.hpp"
#include "shared/anim.hpp"
#include "shared/fast_math.hpp"
#include "shared/cell.hpp"
#include "shared/tactical.hpp"


// Header-static dynamic initializers of the unit at 0x006EF9F0:  the
// shared/unknown_geometry.hpp constants and unknown_static_rect.hpp
// (0x006EF9F0-0x006EFBD6); the other
// drafts of this plan are emitted by these includes.


// Dynamic initializer (0x006EFA20, _$E) of b from the header(s) included
// by HeaderStatics_diagonal_init_006ef9f0.cpp; no source of its own.


// Dynamic initializer (0x006EFA40, _$E) of c from the header(s) included
// by HeaderStatics_diagonal_init_006ef9f0.cpp; no source of its own.


// Dynamic initializer (0x006EFA60, _$E) of angle45 from the header(s) included
// by HeaderStatics_diagonal_init_006ef9f0.cpp; no source of its own.


// Dynamic initializer (0x006EFA80, _$E) of angle60 from the header(s) included
// by HeaderStatics_diagonal_init_006ef9f0.cpp; no source of its own.


// Dynamic initializer (0x006EFAA0, _$E) of angle90 from the header(s) included
// by HeaderStatics_diagonal_init_006ef9f0.cpp; no source of its own.


// Dynamic initializer (0x006EFAC0, _$E) of g from the header(s) included
// by HeaderStatics_diagonal_init_006ef9f0.cpp; no source of its own.


// Dynamic initializer (0x006EFB00, _$E) of h from the header(s) included
// by HeaderStatics_diagonal_init_006ef9f0.cpp; no source of its own.


// Dynamic initializer (0x006EFB30, _$E) of i from the header(s) included
// by HeaderStatics_diagonal_init_006ef9f0.cpp; no source of its own.


// Dynamic initializer (0x006EFB60, _$E) of j from the header(s) included
// by HeaderStatics_diagonal_init_006ef9f0.cpp; no source of its own.


// Dynamic initializer (0x006EFB90, _$E) of k from the header(s) included
// by HeaderStatics_diagonal_init_006ef9f0.cpp; no source of its own.


// Dynamic initializer (0x006EFBA0, _$E) of l from the header(s) included
// by HeaderStatics_diagonal_init_006ef9f0.cpp; no source of its own.


// Dynamic initializer (0x006EFBC0, _$E) of static_rect from the header(s) included
// by HeaderStatics_diagonal_init_006ef9f0.cpp; no source of its own.


// TunnelLocomotionClass::TunnelLocomotionClass() (0x006EFBE0)
// Idle, no destination, the timer started.

TunnelLocomotionClass::TunnelLocomotionClass() :
    unknown_18(0),
    DestinationCoord(unknown_geometry_l),
    IsUnderground(false)
{
}


// bool __stdcall TunnelLocomotionClass::Is_Moving() (0x006EFC30, ILocomotion 4)
// Any state but idle.

bool __stdcall TunnelLocomotionClass::Is_Moving()
{
    return unknown_18 ? true : false;
}


// bool __stdcall TunnelLocomotionClass::Is_Moving_Now() (0x006EFC40, ILocomotion 32)
// Moving and not in state 1.

bool __stdcall TunnelLocomotionClass::Is_Moving_Now()
{
    if (Is_Moving() && unknown_18 != 1) {
        return true;
    }
    return false;
}


// CoordStruct __stdcall TunnelLocomotionClass::Destination() (0x006EFC70, ILocomotion 5)
// The destination while moving, otherwise where the linked object is.

CoordStruct __stdcall TunnelLocomotionClass::Destination()
{
    if (Is_Moving()) {
        return CoordStruct(DestinationCoord.X, DestinationCoord.Y, DestinationCoord.Z);
    }
    CoordStruct coord = LinkedTo->Coord;
    return coord;
}


// void __stdcall TunnelLocomotionClass::Move_To(CoordStruct to) (0x006EFCD0, ILocomotion 17)

void __stdcall TunnelLocomotionClass::Move_To(CoordStruct to)
{
    if (!LinkedTo->unknown_slot201() && !LinkedTo->unknown_slot202() && !LinkedTo->unknown_slot111()
        && !LinkedTo->unknown_slot112()) {
        CoordStruct from = to;
        CoordStruct copy = from;
        DestinationCoord = LinkedTo->Class_Of()->Coord_Fixup(copy);
        if (from != unknown_geometry_l) {
            if (LinkedTo->Get_Height_AGL() < 0 && unknown_18 != 3 && unknown_18 != 5 && unknown_18 != 1
                && unknown_18 != 6 && unknown_18 != 2) {
                LinkedTo->Mark(0);
                unknown_18 = 4;
            }
        }
        if (unknown_18 == 0) {
            unknown_18 = 1;
        }
    }
}


// SPDX-License-Identifier: GPL-3.0-or-later
// Copyright 2026 ra2-decomp contributors
// Copyright 2025 Electronic Arts Inc.
// Derived from Electronic Arts source code; EA's GPL Section 7 terms apply (COPYING.md).
// void __stdcall TunnelLocomotionClass::Stop_Moving() (0x006EFDE0, ILocomotion 18)
// TibSun Tunnel.cpp Stop_Moving: a tunneling unit more than a cell diagonal from its destination
// heads for the nearest free cell (in its own zone first); with none it is destroyed.
// from ra2-decomp 0x00728C00 (TunnelLocomotionClass::Stop_Moving, src/tunnel.cpp)

// The twin's Point2D::Length(): the double square root of the summed squares.
static inline int Point_Length(Point2D const &point)
{
    double y = point.Y;
    double x = point.X;
    return (int)unknown_sqrt_4ba410(x * x + y * y);
}

void __stdcall TunnelLocomotionClass::Stop_Moving()
{
    switch (unknown_18) {
    case 1:
        DestinationCoord = unknown_geometry_l;
        break;
    case 5:
        DestinationCoord = unknown_geometry_l;
        break;
    case 6:
        DestinationCoord = unknown_geometry_l;
        break;
    case 7:
        DestinationCoord = unknown_geometry_l;
        break;
    case 4: {
        CoordStruct pos = LinkedTo->Coord;
        if (Point_Length(Point2D(pos.X, pos.Y) - Point2D(DestinationCoord.X, DestinationCoord.Y)) > unknown_geometry_diagonal) {
            Cell cell = LinkedTo->unknown_slot104();
            Cell nearby = Map.Nearby_Location(cell, 1, Map.Zone(cell, 0, false), 0, false, Point2D(1, 1), 0, 0, 1, 1, Cell(0, 0), 0);
            if (nearby == unknown_geometry_k) {
                nearby = Map.Nearby_Location(cell, 1, -1, 0, false, Point2D(1, 1), 0, 0, 1, 1, Cell(0, 0), 0);
            }
            if (nearby == unknown_geometry_k) {
                LinkedTo->Take_Damage(&LinkedTo->Strength, 0, Rules->unknown_c74, 0, true, true);
                DestinationCoord = unknown_geometry_l;
            } else {
                DestinationCoord = Cell_Coord(nearby);
            }
        }
        break;
    }
    case 3:
        unknown_18 = 5;
        DestinationCoord = unknown_geometry_l;
        break;
    case 2:
        unknown_18 = 7;
        DestinationCoord = unknown_geometry_l;
        break;
    }
}


// bool __stdcall TunnelLocomotionClass::Process() (0x006F0000, ILocomotion 16)
// TibSun Tunnel.cpp Process: runs the dig cycle's state, drops an enemy unit's selection once
// it is shrouded or fogged and clears its damage particles. The state handlers expand inline
// (state 4's 0x006F0730 stays a call); their timer ratios then call the out-of-line
// CDTimerClass::Value 0x004A4D20 (inline budget: the two Object_Coord accessors are pending sites).

bool __stdcall TunnelLocomotionClass::Process()
{
    if (Is_Moving()) {
        int agl = LinkedTo->Get_Height_AGL();
        int height = LinkedTo->Get_Height();
        switch (unknown_18) {
        case 1:
            if (agl < 0) {
                unknown_18 = 3;
            } else {
                Process_Turning();
            }
            break;
        case 2:
            if (agl < 0) {
                unknown_18 = 3;
            } else {
                Process_Digging_In();
            }
            break;
        case 3:
            if (height == -0x100) {
                LinkedTo->Mark(0);
                unknown_18 = 4;
            } else {
                Process_Descending();
            }
            break;
        case 4:
            Process_Tunneling();
            break;
        case 5:
            Process_Ascending();
            break;
        case 6:
            Process_Emerging();
            break;
        case 7:
            Process_Aborting();
            break;
        }
        if (LinkedTo->unknown_77) {
            if (!LinkedTo->House->Is_Player_Control()) {
                if (Map.Is_Shrouded(Object_Coord(LinkedTo)) || (unknown_id_source_a3d290->Special().FogOfWar && Map.Is_Fogged(Object_Coord(LinkedTo)))) {
                    LinkedTo->Unselect();
                }
            }
        }
        LinkedTo->unknown_6d7f30();
        return true;
    }
    return false;
}


// int __stdcall TunnelLocomotionClass::Visual_Character(bool raw) (0x006F0390, ILocomotion 13)
// Underground (state 4): 4, or 5 for the raw view; otherwise normal.

int __stdcall TunnelLocomotionClass::Visual_Character(bool raw)
{
    if (unknown_18 == 4) {
        return (raw != 0) + 4;
    }
    return 0;
}


// The direction from one coordinate to another (hypothesis: Westwood's Direction()).
static inline UnknownFacingDir Coord_Direction(CoordStruct const &from, CoordStruct const &to)
{
    CoordStruct delta = from;
    delta.X = to.X - delta.X;
    delta.Y = to.Y - delta.Y;
    double angle = Fast_Atan2((double)-delta.Y, (double)delta.X);
    return UnknownFacingDir((unsigned short)(int)((angle - 1.5707963267948966) * -10430.060040584269));
}

void TunnelLocomotionClass::Process_Turning()
{
    if (!LinkedTo->unknown_2e0.Is_Rotating()) {
        UnknownFacingDir dir = Coord_Direction(LinkedTo->Coord, DestinationCoord);
        if (dir.Raw != LinkedTo->unknown_2e0.Current().Raw) {
            Do_Turn(dir);
            return;
        }
        int delay = (int)(64.0 / LinkedTo->Techno_Type_Class()->ROT / Rules->unknown_20);
        DigTimer = UnknownTunnelTimer(delay);
        unknown_18 = 2;
        Voc_Play_At(Rules->unknown_16c, CoordStruct(LinkedTo->Coord), 0);
        new AnimClass(Rules->unknown_228, CoordStruct(LinkedTo->Coord), 0, 1, 0x600, 0);
        IsUnderground = false;
    }
}


void TunnelLocomotionClass::Process_Digging_In()
{
    if (DigTimer.Ratio() == 1.0) {
        LinkedTo->unknown_slot306(1.0);
        unknown_18 = 3;
        Voc_Play_At(Rules->unknown_16c, CoordStruct(LinkedTo->Coord), 0);
        new AnimClass(Rules->unknown_228, CoordStruct(LinkedTo->Coord), 0, 1, 0x600, 0);
        LinkedTo->unknown_slot54(false);
    }
}


void TunnelLocomotionClass::Process_Aborting()
{
    if (DigTimer.Ratio() == 1.0) {
        unknown_18 = 0;
        LinkedTo->unknown_4cd9b0();
    }
}


void TunnelLocomotionClass::Process_Descending()
{
    CoordStruct coord = LinkedTo->Coord;
    if (coord.Z > -0x100) {
        int rate = (int)(LinkedTo->unknown_slot303() * Rules->unknown_20);
        if (rate <= 5) {
            rate = 5;
        }
        coord.Z -= rate;
        if (coord.Z < -0x100) {
            coord.Z = -0x100;
        }
        LinkedTo->Set_Coord(coord);
    } else {
        LinkedTo->Mark(0);
        unknown_18 = 4;
    }
}


// Tunnel state4: choose a surface exit, or advance toward the underground destination.
extern "C" double const tunnel_radian_scale;
static inline UnknownFacingDir Tunnel_Direction_To(CoordStruct const &from, CoordStruct const &to)
{
 Point2D delta(to.X-from.X,to.Y-from.Y);
 double angle=Fast_Atan2((double)-delta.Y,(double)delta.X);
 return UnknownFacingDir((unsigned short)(int)((angle-1.5707963267948966)*-10430.060040584269));
}
static inline CoordStruct Tunnel_Move_Coord(CoordStruct const &start, UnknownFacingDir const &dir, int distance)
{
 Point2D point(start.X,start.Y);
 double radians=((short)dir.Raw-0x3fff)*tunnel_radian_scale;
 return CoordStruct((int)(point.X+Fast_Cos(radians)*(double)distance),
                    (int)(point.Y-Fast_Sin(radians)*(double)distance),start.Z);
}
void TunnelLocomotionClass::Process_Tunneling()
{
 CoordStruct coord=LinkedTo->Coord;
 if((Point2D(DestinationCoord.X,DestinationCoord.Y)-Point2D(coord.X,coord.Y)).Length()<20) {
  coord=DestinationCoord;
  coord.Z=-256;
  if(LinkedTo->Can_Enter_Cell(&Map[CoordStruct(LinkedTo->Coord)],-1,-1,0,true) && Map.Unknown_0055A7F0(CoordStruct(LinkedTo->Coord))) {
   Cell cell=LinkedTo->unknown_slot104();
   cell=Map.Nearby_Location(cell,1,Map.Zone(cell,0,false),0,false,Point2D(1,1),false,false,true,true,Cell(0,0),0);
   if(cell==unknown_geometry_k) cell=Map.Nearby_Location(LinkedTo->unknown_slot104(),1,-1,6,false,Point2D(1,1),false,false,true,true,Cell(0,0),0);
   if(cell==unknown_geometry_k) {
    LinkedTo->Take_Damage(&LinkedTo->Strength,0,Rules->unknown_c74,0,true,true);
    return;
   }
   Move_To(CoordStruct(cell,0));
  } else {
   if(LinkedTo->Get_Height_AGL()>-50) Voc_Play_At(Rules->unknown_16c,CoordStruct(LinkedTo->Coord),0);
   unknown_18=5;
   LinkedTo->Mark(1);
   IsUnderground=false;
  }
 } else {
  coord=Tunnel_Move_Coord(coord,Tunnel_Direction_To(coord,DestinationCoord),19);
 }
 LinkedTo->Set_Coord(coord);
}


void TunnelLocomotionClass::Process_Ascending()
{
    CoordStruct coord = LinkedTo->Coord;
    int ground = Map.Get_Height_GL(coord);
    if (coord.Z < ground) {
        int height = LinkedTo->Get_Height_AGL();
        int rate = (int)(LinkedTo->unknown_slot303() * Rules->unknown_20);
        if (rate <= 5) {
            rate = 5;
        }
        coord.Z += rate;
        if (coord.Z > ground) {
            coord.Z = ground;
        }
        LinkedTo->Set_Coord(coord);
        if (height <= -0x32 && LinkedTo->Get_Height_AGL() > -0x32) {
            Voc_Play_At(Rules->unknown_16c, CoordStruct(LinkedTo->Coord), 0);
            new AnimClass(Rules->unknown_228, CoordStruct(LinkedTo->Coord), 0, 1, 0x600, 0);
        }
    } else {
        LinkedTo->Look(0, 0);
        int delay = (int)(64.0 / LinkedTo->Techno_Type_Class()->ROT);
        DigTimer = UnknownTunnelTimer(delay);
        LinkedTo->unknown_slot306(0.0);
        unknown_18 = 6;
    }
}


void TunnelLocomotionClass::Process_Emerging()
{
    if (DigTimer.Ratio() == 1.0) {
        LinkedTo->unknown_slot93(2);
        if (LinkedTo->unknown_7d && !LinkedTo->unknown_76 && !LinkedTo->unknown_7a) {
            Stop_Moving();
            unknown_18 = 0;
            LinkedTo->unknown_4cd9b0();
        }
    }
}


// Matrix3D * __stdcall TunnelLocomotionClass::Draw_Matrix(Matrix3D *matrix, int *key) (0x006F0CE0, ILocomotion 9)
// Above ground (state 0): the base transform, tilted by the cell's slope
// matrix; the key gains the ramp. Otherwise the base transform pitched about
// Y by the state's dig angle (the key gains its 32nds; -1 while the angle
// changes). Each base result goes to a buffer that lives only as long as the
// original's by-value return temporary: a block local in the two branches, a
// temporary in the declaration that copies it on the dig path (the original's
// frame gives the three their own lifetimes; a function-scope buffer does not).

Matrix3D * __stdcall TunnelLocomotionClass::Draw_Matrix(Matrix3D *matrix, int *key)
{
    if (unknown_18 == 0) {
        CoordStruct coord = LinkedTo->Coord;
        int ramp = Map[coord].unknown_11c;
        if (key != 0 && *key != -1) {
            *key = (*key << 6) + ramp;
        }
        Matrix3D transform;
        transform.Make_Identity();
        if (ramp != 0) {
            Matrix3D temp;
            Matrix3D body(*LocomotionClass::Draw_Matrix(&temp, key));
            Matrix3D ramp_matrix;
            transform = *Unknown_00718D60(&ramp_matrix, ramp) * body;
        } else {
            Matrix3D temp;
            Matrix3D body(*LocomotionClass::Draw_Matrix(&temp, key));
            transform = body;
        }
        *matrix = transform;
        return matrix;
    }

    double angle;
    switch (unknown_18) {
    case 3:
        angle = 1.5707963267948966;
        break;
    case 5:
        angle = -1.5707963267948966;
        break;
    case 2:
        if (key != 0) {
            *key = -1;
        }
        angle = DigTimer.Ratio() * 1.5707963267948966;
        break;
    case 6:
        if (key != 0) {
            *key = -1;
        }
        angle = (1.0 - DigTimer.Ratio()) * -1.5707963267948966;
        break;
    case 7:
        if (key != 0) {
            *key = -1;
        }
        angle = (1.0 - DigTimer.Ratio()) * 1.5707963267948966;
        break;
    default:
        angle = 0;
        break;
    }
    Matrix3D transform(*LocomotionClass::Draw_Matrix(&Matrix3D(), key));
    if (key != 0 && *key != -1) {
        *key = (*key << 8) | ((int)(angle * 32.0) & 0x3F);
    }
    transform.Rotate_Y((float)angle);
    *matrix = transform;
    return matrix;
}


// int __stdcall TunnelLocomotionClass::Z_Adjust() (0x006F0FF0, ILocomotion 14)
extern int unknown_7f8660;
extern int unknown_7f8664;

int __stdcall TunnelLocomotionClass::Z_Adjust()
{
    CoordStruct coord = LinkedTo->Coord;
    Point2D from;
    unknown_tactical_83996c->Coord_To_Pixel(coord, from);
    coord.Z = Map.Get_Height_GL(coord);
    Point2D to;
    unknown_tactical_83996c->Coord_To_Pixel(coord, to);
    int height = from.Y - to.Y;
    double ratio = DigTimer.Ratio();
    int facing = Dir_To_256(LinkedTo->unknown_2e0.Current());
    switch (unknown_18) {
    case 4:
        return unknown_7f8664 + height;
    case 6:
        return (int)((1.0 - ratio) * unknown_7f8660 + height);
    case 2:
        if (ratio > 0.4 && !IsUnderground) {
            IsUnderground = true;
        }
        if (facing > 0x46 && facing < 0x8c) {
            height += unknown_7f8664;
        } else {
            height = (int)(unknown_7f8664 * ratio + height);
        }
        return height;
    case 3:
        return unknown_7f8664 + height;
    case 5:
        return unknown_7f8660 + height;
    default:
        return LocomotionClass::Z_Adjust();
    }
}


// int __stdcall TunnelLocomotionClass::Z_Gradient() (0x006F11C0, ILocomotion 15)
// Gradient 2 while digging or surfacing (states 2, 3, 5, 6, 7).

int __stdcall TunnelLocomotionClass::Z_Gradient()
{
    if (unknown_18 == 3 || unknown_18 == 2 || unknown_18 == 7 || unknown_18 == 6 || unknown_18 == 5) {
        return 2;
    }
    return LocomotionClass::Z_Gradient();
}


// bool __stdcall TunnelLocomotionClass::Is_To_Have_Shadow() (0x006F1200, ILocomotion 8)
// Only above ground (states 0, 1, 2, 6, 7).

bool __stdcall TunnelLocomotionClass::Is_To_Have_Shadow()
{
    if (unknown_18 == 0 || unknown_18 == 1 || unknown_18 == 7 || unknown_18 == 2 || unknown_18 == 6) {
        return true;
    }
    return false;
}


// int __stdcall TunnelLocomotionClass::Can_Enter_Cell(Cell cell) (0x006F1230, ILocomotion 7)
// Refused (7) where the cell object's 0x0047C200 fails, unless the flag at
// 0x00A40D13 is set.

int __stdcall TunnelLocomotionClass::Can_Enter_Cell(Cell cell)
{
    if (!Debug_Map) {
        Cell copy(cell.X, cell.Y);
        if (!Loco_Cell_At(copy)->Can_Burrow_Here()) {
            return 7;
        }
    }
    return 0;
}


// void __stdcall TunnelLocomotionClass::Do_Turn(UnknownFacingDir dir) (0x006F1280, ILocomotion 19)
// Turns the linked object's body facing.

void __stdcall TunnelLocomotionClass::Do_Turn(UnknownFacingDir dir)
{
    LinkedTo->unknown_2e0.Set_Desired(UnknownFacingDir(dir));
}


// HRESULT __stdcall TunnelLocomotionClass::GetClassID(GUID *clsid) (0x006F12B0)

HRESULT __stdcall TunnelLocomotionClass::GetClassID(GUID *clsid)
{
    if (clsid == 0) {
        return E_POINTER;
    }
    *clsid = CLSID_TunnelLocomotionClass;
    return S_OK;
}


// HRESULT __stdcall TunnelLocomotionClass::Load(IStream *stream) (0x006F12F0)
// LocomotionClass::Load, then rebuilt in place.

HRESULT __stdcall TunnelLocomotionClass::Load(IStream *stream)
{
    HRESULT result = LocomotionClass::Load(stream);
    if (SUCCEEDED(result)) {
        new (this) TunnelLocomotionClass(NoInitClass());
    }
    return result;
}


// int __stdcall TunnelLocomotionClass::Layer() (0x006F1340, ILocomotion 29)
// Underground (state 4) is layer 0, otherwise 2.

int __stdcall TunnelLocomotionClass::Layer()
{
    return unknown_18 != 4 ? 2 : 0;
}


// int __stdcall TunnelLocomotionClass::State() (0x006F1360, ILocomotion 35)
// The base state, or 7 while tunnelling.

int __stdcall TunnelLocomotionClass::State()
{
    int state = LocomotionClass::State();
    if (state == 0 && unknown_18) {
        state = 7;
    }
    return state;
}


// bool __stdcall TunnelLocomotionClass::Is_Surfacing() (0x006F1380, ILocomotion 38)
// States 5 and 6.

bool __stdcall TunnelLocomotionClass::Is_Surfacing()
{
    return unknown_18 == 5 || unknown_18 == 6;
}


// int TunnelLocomotionClass::Size() (0x006F13A0): inline in shared/tunnel_locomotion.hpp, emitted with the vtable; no source of its own.


// void *TunnelLocomotionClass::`scalar deleting destructor'(unsigned int flags) (0x006F13B0)
// Compiler-generated with the vtable (the destructor inlined). No source of its own.
