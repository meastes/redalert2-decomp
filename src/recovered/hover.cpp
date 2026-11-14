// hover.cpp: the original translation unit re-filed as one source (Route A): the accepted
// bodies of its canonical compile group in address order, under the unit's own header
// state (its switches and includes in their original order). The module map
// manifests/original_target/modules/hover.json carries the addresses and the rows that
// stay in their own files.


#define RA2_PROCESS_COM_BOOLEAN
#define RA2_COORD_AS_CELL
#define RA2_SHIP_CELL_ACCESSORS
#define RA2_CELL_AS_COORD
#define RA2_COORD_FROM_CELL_INLINE
#include "shared/building_class.hpp"
#include "shared/building_type_class.hpp"
// Header-static dynamic initializers of the unit at 0x004FB6E0:  the
// shared/unknown_geometry.hpp constants and unknown_static_rect.hpp
// (0x004FB6E0-0x004FB8C6); the other
// drafts of this plan are emitted by these includes.
#include "shared/unknown_geometry.hpp"
#include "shared/unknown_static_rect.hpp"


// Dynamic initializer (0x004FB710, _$E) of b from the header(s) included
// by HeaderStatics_diagonal_init_004fb6e0.cpp; no source of its own.


// Dynamic initializer (0x004FB730, _$E) of c from the header(s) included
// by HeaderStatics_diagonal_init_004fb6e0.cpp; no source of its own.


// Dynamic initializer (0x004FB750, _$E) of angle45 from the header(s) included
// by HeaderStatics_diagonal_init_004fb6e0.cpp; no source of its own.


// Dynamic initializer (0x004FB770, _$E) of angle60 from the header(s) included
// by HeaderStatics_diagonal_init_004fb6e0.cpp; no source of its own.


// Dynamic initializer (0x004FB790, _$E) of angle90 from the header(s) included
// by HeaderStatics_diagonal_init_004fb6e0.cpp; no source of its own.


// Dynamic initializer (0x004FB7B0, _$E) of g from the header(s) included
// by HeaderStatics_diagonal_init_004fb6e0.cpp; no source of its own.


// Dynamic initializer (0x004FB7F0, _$E) of h from the header(s) included
// by HeaderStatics_diagonal_init_004fb6e0.cpp; no source of its own.


// Dynamic initializer (0x004FB820, _$E) of i from the header(s) included
// by HeaderStatics_diagonal_init_004fb6e0.cpp; no source of its own.


// Dynamic initializer (0x004FB850, _$E) of j from the header(s) included
// by HeaderStatics_diagonal_init_004fb6e0.cpp; no source of its own.


// Dynamic initializer (0x004FB880, _$E) of k from the header(s) included
// by HeaderStatics_diagonal_init_004fb6e0.cpp; no source of its own.


// Dynamic initializer (0x004FB890, _$E) of l from the header(s) included
// by HeaderStatics_diagonal_init_004fb6e0.cpp; no source of its own.


// Dynamic initializer (0x004FB8B0, _$E) of static_rect from the header(s) included
// by HeaderStatics_diagonal_init_004fb6e0.cpp; no source of its own.


// HoverLocomotionClass::HoverLocomotionClass() (0x004FB8D0)
#include "shared/hover_locomotion.hpp"

HoverLocomotionClass::HoverLocomotionClass() :
    unknown_18(unknown_geometry_l),
    unknown_24(unknown_geometry_l),
    unknown_30(0),
    unknown_48(0),
    unknown_50(0),
    unknown_58(1.0),
    unknown_60(0),
    unknown_68(false),
    unknown_6c(0),
    unknown_70(false)
{
}


// HRESULT __stdcall HoverLocomotionClass::Link_To_Object(void *object) (0x004FB960, ILocomotion 3)
// Links, then turns at twice the type's rate.

HRESULT __stdcall HoverLocomotionClass::Link_To_Object(void *object)
{
    HRESULT result = LocomotionClass::Link_To_Object(object);
    FacingClass facing(LinkedTo->Techno_Type_Class()->ROT * 2);
    unknown_30 = facing;
    return result;
}


// HoverLocomotionClass::~HoverLocomotionClass() (0x004FB9B0)

HoverLocomotionClass::~HoverLocomotionClass()
{
}


#include "shared/rules.hpp"
#include "shared/infantry.hpp"
extern double const unknown_hover_bob_scale_79c9b8;
inline CoordStruct Hover_Adjacent_Coord_004fb9d0(CoordStruct const &coord,int facing)
{
    TPoint2D<int> const &offset=unknown_facing_offsets_851ac0[facing&7];
    return CoordStruct(coord.X+offset.X,coord.Y+offset.Y,coord.Z);
}
void HoverLocomotionClass::Gravity_AI_004fb9d0()
{
    int height=LinkedTo->Get_Height_AGL();
    int clearance=height;
    if (LinkedTo->unknown_4d0!=-1) {
        int ground=Map.Get_Height_GL(Object_Coord(LinkedTo));
        if (Map.Get_Height_GL(Hover_Adjacent_Coord_004fb9d0(Object_Coord(LinkedTo),LinkedTo->unknown_4d0))>ground) clearance-=Rules->HoverHeight;
    }
    height=(int)(height+unknown_60);
    int id=LinkedTo->Fetch_ID();
    double period=(id&1?1.0:1.1)*Rules->HoverBob*900;
    double angle=Fast_Sin((double)((Frame+2*id)%(int)period)*unknown_hover_bob_scale_79c9b8/period);
    height=(int)(2*angle+height);
    if (height<0) { unknown_60=0; height=0; }
    bool wasdown=LinkedTo->unknown_74;
    LinkedTo->unknown_74=false;
    LinkedTo->Set_Height_AGL(height);
    LinkedTo->unknown_74=wasdown;
    if (clearance<Rules->HoverHeight) {
        if (LocomotionClass::Is_Powered()) {
            unknown_60+=((double)Rules->HoverHeight+(double)Rules->HoverHeight-(double)clearance)/(double)Rules->HoverHeight*(double)Rules->unknown_1320;
        }
        if (clearance<Rules->HoverHeight/4) unknown_60+=(double)(Rules->unknown_1320/3);
    }
    unknown_60-=Rules->unknown_1320;
    unknown_60*=Rules->HoverDampen;
}


#include "shared/cell.hpp"
#include "shared/techno_type.hpp"
#include "shared/crt_math.hpp"
extern double const unknown_hover_tilt_min_79d170;
extern double const unknown_hover_facing_step_79d090;
extern float const unknown_hover_zero_79a738;
Matrix3D *__stdcall HoverLocomotionClass::Draw_Matrix(Matrix3D *matrix,int *key)
{
    if (fabs(LinkedTo->unknown_280)>=unknown_hover_tilt_min_79d170 ||
        fabs(LinkedTo->unknown_284)>=unknown_hover_tilt_min_79d170) {
        if (key) *key=-1;
        Matrix3D translation;
        translation.Make_Identity();
        Matrix3D rotation;
        rotation.Make_Identity();
        float width=(float)LinkedTo->Techno_Type_Class()->unknown_330;
        float length=(float)LinkedTo->Techno_Type_Class()->unknown_338;
        float cos_pitch=Fast_Cos(LinkedTo->unknown_284);
        float sin_pitch=Fast_Sin(LinkedTo->unknown_284);
        float cos_roll=Fast_Cos(LinkedTo->unknown_280);
        float sin_roll=Fast_Sin(LinkedTo->unknown_280);
        int height=(int)(fabs(sin_roll)*width+fabs(sin_pitch)*length);
        int shift_x=(int)(length-(int)(cos_pitch*length));
        int shift_y=(int)(width-(int)(cos_roll*width));
        if (LinkedTo->unknown_284<unknown_hover_zero_79a738) shift_x=-shift_x;
        if (LinkedTo->unknown_280>unknown_hover_zero_79a738) shift_y=-shift_y;
        translation.Translate_Z((float)height);
        rotation.Translate_X((float)shift_x);
        rotation.Translate_Y((float)shift_y);
        rotation.Rotate_X(LinkedTo->unknown_280);
        rotation.Rotate_Y(LinkedTo->unknown_284);
        Matrix3D base;
        Matrix3D base_value(*LocomotionClass::Draw_Matrix(&base,key));
        *matrix=translation*base_value*rotation;
    } else {
        if (!Is_Powered()) {
            int ramp=Map[Object_Coord(LinkedTo)].unknown_11c;
            Matrix3D slope;
            Matrix3D result;
            result=*Unknown_00718D60(&slope,ramp);
            result.Rotate_Z((float)((Dir_To_32(LinkedTo->unknown_2e0.Current())-8)*unknown_hover_facing_step_79d090));
            if (key && *key!=-1) {
                *key=32*(ramp+(*key<<6));
                *key|=Dir_To_32(LinkedTo->unknown_2e0.Current());
            }
            *matrix=result;
        } else {
            Matrix3D base;
            *matrix=*LocomotionClass::Draw_Matrix(&base,key);
        }
    }
    return matrix;
}


// Matrix3D * __stdcall HoverLocomotionClass::Shadow_Matrix(Matrix3D *matrix, int *key) (0x004FBF50, ILocomotion 10)
// The base shadow transform; while the hull is tilted (either angle at
// +0x280/+0x284 at least 0.005) the key is -1 (not cached).
#include "shared/matrix3d.hpp"

Matrix3D * __stdcall HoverLocomotionClass::Shadow_Matrix(Matrix3D *matrix, int *key)
{
    if ((fabs(LinkedTo->unknown_280) >= 0.005 || fabs(LinkedTo->unknown_284) >= 0.005) && key != 0) {
        *key = -1;
    }
    Matrix3D shadow;
    *matrix = *LocomotionClass::Shadow_Matrix(&shadow, key);
    return matrix;
}


// unsigned char __stdcall HoverLocomotionClass::Process() (0x004FBFC0, ILocomotion 16)
// TibSun hover.cpp HoverLocomotionClass::Process, statement for statement,
// with RA2's radio-contact arrival test and wake/bridge rules.
#include "shared/cell_class.hpp"
#include "shared/anim.hpp"
#include "shared/fast_math.hpp"
extern double const unknown_facing_origin_79b630;
extern double const unknown_facing_scale_79b628;
extern double const unknown_move_angle_scale_79b620;

inline UnknownFacingDir Hover_Direction_004fbfc0(CoordStruct const &from, CoordStruct const &to)
{
    UnknownFacingDir direction;
    if (from.Equal_XY_004bfb60(to)) direction.Raw = 0;
    direction = UnknownFacingDir((unsigned short)(int)((Fast_Atan2(double(from.Y) - to.Y, double(to.X) - from.X) - unknown_facing_origin_79b630) * unknown_facing_scale_79b628));
    return direction;
}

// TibSun's Point2D(Coord) conversion.
inline TPoint2D<int> Hover_Point_004fbfc0(CoordStruct const &coord)
{
    return TPoint2D<int>(coord.X, coord.Y);
}

// TibSun Point2D::Distance_To.
inline int Hover_Distance_004fbfc0(CoordStruct const &from, CoordStruct const &to)
{
    TPoint2D<int> difference = TPoint2D<int>(from.X, from.Y) - TPoint2D<int>(to.X, to.Y);
    return (int)Fast_Sqrt((double)difference.X * difference.X + (double)difference.Y * difference.Y);
}

inline TPoint2D<int> Hover_Move_Point_004fbfc0(TPoint2D<int> const &start, UnknownFacingDir const &facing, int distance)
{
    double angle = ((short)facing.Raw - 0x3fff) * unknown_move_angle_scale_79b620;
    return TPoint2D<int>((int)(double(start.X) + Fast_Cos(angle) * double(distance)),
        (int)(double(start.Y) - Fast_Sin(angle) * double(distance)));
}
inline UnknownFacingDir operator+(UnknownFacingDir const &left, int right)
{
    return UnknownFacingDir((unsigned short)(left.Raw + right));
}
inline int Dir256_004fbfc0(int facing) { return facing << 8; }
inline unsigned Hover_Under_Bridge_004fbfc0(CellClass const *cell) { return cell->unknown_140 & 0x100; }
inline int Hover_Abs_004fbfc0(int value) { return abs(value); }
inline UnknownFacingDir operator-(UnknownFacingDir const &left, UnknownFacingDir const &right)
{
    return UnknownFacingDir((unsigned short)(left.Raw - right.Raw));
}
inline bool operator>=(UnknownFacingDir const &left, UnknownFacingDir const &right)
{
    return abs((short)left.Raw) >= abs((short)right.Raw);
}
inline bool Is_Complete_Turn(UnknownFacingDir const &current, UnknownFacingDir const &towards, UnknownFacingDir const &by)
{
    UnknownFacingDir diff(current - towards);
    if (by >= diff) {
        return true;
    }
    return false;
}
unsigned char __stdcall HoverLocomotionClass::Process()
{
    if (Is_Moving() && unknown_4fdfc0()) {
        Motion_AI_004fdaf0();
        if (!LinkedTo->unknown_7d) return false;

        int height = LinkedTo->Get_Height();
        int speed = (int)(LinkedTo->unknown_slot303() * unknown_50);
        UnknownFacingDir direction = Hover_Direction_004fbfc0(LinkedTo->Center_Coord(), unknown_24);

        if (speed == 0 && Is_Complete_Turn(unknown_30.Current(), direction, UnknownFacingDir((unsigned short)0))) {
            unknown_4fed20();
            speed = (int)(LinkedTo->unknown_slot303() * unknown_50);
        }

        if (speed >= Hover_Distance_004fbfc0(Object_Coord(LinkedTo), unknown_24)) {
            LinkedTo->unknown_5a0 = true;
            LinkedTo->unknown_5a1 = false;
            unknown_70 = false;

            Start_Of_Move_004fe0f0(0);

            if (LinkedTo->unknown_4d0 == -1 && unknown_18 == unknown_geometry_l) {
                unknown_24 = unknown_geometry_l;
            }

            TechnoClass *contact = LinkedTo->Contact(0);
            Cell cell = unknown_geometry_k;
            if (contact) cell = Coord_Cell(contact->unknown_slot19(LinkedTo));
            bool arrived = LinkedTo->unknown_slot104() == cell;

            if (unknown_18 != unknown_geometry_l || arrived) {
                int ok = While_Moving_004fcbc0(true);
                int const &result = ok;
                if (!LinkedTo->unknown_7d || LinkedTo->unknown_76 || LinkedTo->unknown_7a) return 7;
                if (result == 0 && unknown_24 == unknown_geometry_l) {
                    LinkedTo->unknown_slot93(2);
                    if (!LinkedTo->unknown_7d || LinkedTo->unknown_76 || LinkedTo->unknown_7a) return false;
                }
                if (ok == 7) {
                    if (LinkedTo->unknown_574 >= 0) return false;
                    Stop();
                } else if (ok != 0) {
                    unknown_4fda80();
                    speed = 0;
                }
            }
        }

        if (speed > 0) {
            CoordStruct coord = Object_Coord(LinkedTo);

            if (LinkedTo->unknown_slot47() && unknown_24 != unknown_geometry_l) {
                LinkedTo->Clear_Occupy_Bit(Object_Coord(LinkedTo));
                LinkedTo->unknown_5a0 = false;
                LinkedTo->unknown_5a1 = false;
            }

            TPoint2D<int> moved = Hover_Move_Point_004fbfc0(Hover_Point_004fbfc0(coord), unknown_30.Current(), speed);
            coord.X = moved.X;
            coord.Y = moved.Y;
            if (coord.As_Cell() != Object_Coord(LinkedTo).As_Cell()) {
                LinkedTo->Mark(0);
                LinkedTo->Set_Coord(coord);
                LinkedTo->unknown_func_5d4920(height);
                CellClass *cellptr = &Map[coord];
                if (!LinkedTo->unknown_79 && Hover_Under_Bridge_004fbfc0(cellptr) && LinkedTo->Get_Height_AGL() >= unknown_geometry_j) {
                    LinkedTo->unknown_79 = true;
                }
                if (LinkedTo->unknown_79 == true && !Hover_Under_Bridge_004fbfc0(cellptr)) {
                    LinkedTo->unknown_79 = false;
                }
                LinkedTo->Mark(1);
            } else {
                bool wasdown = LinkedTo->unknown_74;
                LinkedTo->unknown_74 = false;
                LinkedTo->Set_Coord(coord);
                LinkedTo->unknown_func_5d4920(height);
                LinkedTo->unknown_74 = wasdown;
            }
        }
    }

    if (Is_Moving_Now() && (Frame % 10) == 0) {
        if (!LinkedTo->unknown_79 && static_cast<CellClass *>(LinkedTo->Get_Cell_Ptr())->Land_Type() == 2) {
            if (Rules->unknown_09c != 0) {
                new AnimClass(Rules->unknown_09c, Object_Coord(LinkedTo), 0, 1, 0x600, 0);
            }
        }
    }

    Gravity_AI_004fb9d0();

    if (unknown_68) {
        if (Is_Powered()) {
            UnknownFacingDir dir = LinkedTo->unknown_2e0.Current();
            LinkedTo->unknown_2e0.Set(dir + Dir256_004fbfc0(unknown_6c));
            int delta = Hover_Abs_004fbfc0(unknown_6c);
            if (delta >= 1) {
                delta = 1;
            }
            if (unknown_6c < 0) {
                unknown_6c += delta;
            } else {
                unknown_6c -= delta;
            }
            if (unknown_6c == 0) {
                unknown_68 = false;
            }
        } else {
            unknown_68 = false;
        }
        if (!unknown_68) {
            Map[Object_Coord(LinkedTo)].Trigger_Veins();
            if (Map[Object_Coord(LinkedTo)].Land_Type() == 2) {
                if (Object_Coord(LinkedTo).Z < Map.Get_Height_GL(Object_Coord(LinkedTo)) + unknown_geometry_g) {
                    LinkedTo->Fall_From_Height();
                }
            }
        }
    }

    return Is_Moving();
}


// bool __stdcall HoverLocomotionClass::Is_Moving() (0x004FC880, ILocomotion 4)
// A destination or a head-to coordinate is set.

bool __stdcall HoverLocomotionClass::Is_Moving()
{
    return unknown_18 != unknown_geometry_l || unknown_24 != unknown_geometry_l;
}


// bool __stdcall HoverLocomotionClass::Is_Moving_Now() (0x004FC8D0, ILocomotion 32)

bool __stdcall HoverLocomotionClass::Is_Moving_Now()
{
    return Is_Moving() && unknown_48 != 0.0;
}


// CoordStruct __stdcall HoverLocomotionClass::Destination() (0x004FC900, ILocomotion 5)

CoordStruct __stdcall HoverLocomotionClass::Destination()
{
    if (unknown_18 != unknown_geometry_l) {
        return CoordStruct(unknown_18.X, unknown_18.Y, unknown_18.Z);
    }
    return CoordStruct(unknown_geometry_l.X, unknown_geometry_l.Y, unknown_geometry_l.Z);
}


// CoordStruct __stdcall HoverLocomotionClass::Head_To_Coord() (0x004FC960, ILocomotion 6)

CoordStruct __stdcall HoverLocomotionClass::Head_To_Coord()
{
    if (unknown_24 != unknown_geometry_l) {
        return CoordStruct(unknown_24.X, unknown_24.Y, unknown_24.Z);
    }
    CoordStruct coord = LinkedTo->Coord;
    return coord;
}


#include "shared/house.hpp"
void __stdcall HoverLocomotionClass::Move_To(CoordStruct to)
{
    unknown_18=to;
    if (LinkedTo->unknown_slot202()) return;
    if (Is_Powered() && Is_Ion_Sensitive() && LinkedTo->House->Unknown_0051FC80()) Power_Off();
    unknown_18.Z=Map.Get_Height_GL(unknown_18);
    if (Loco_Cell(CoordStruct(to.X,to.Y,to.Z))->Flags&0x100) unknown_18.Z+=unknown_geometry_j;
    if (to!=unknown_geometry_l && Is_Powered()) {
        if (!Is_Moving_Now()) {
            LinkedTo->unknown_slot306(1.0);
            Start_Of_Move_004fe0f0(0);
            While_Moving_004fcbc0(true);
        } else if (unknown_24==unknown_geometry_l) {
            unknown_4fed20();
        }
    }
}


// int HoverLocomotionClass::While_Moving_004fcbc0(bool first_pass) (0x004FCBC0)
// TibSun hover.cpp While_Moving.
#include "shared/unknown_vector3.hpp"
#include "shared/crt_memory.hpp"
#include "shared/overlay_type.hpp"
#include "shared/tube.hpp"

inline CoordStruct Coord_Offset(CoordStruct const &coord, TPoint2D<int> const &offset)
{
    return *(CoordStruct *)&UnknownVector3(coord.X + offset.X, coord.Y + offset.Y, coord.Z);
}

inline CoordStruct Adjacent_Coord(CoordStruct const &coord, int facing)
{
    return Coord_Offset(coord, unknown_facing_offsets_851ac0[facing & 7]);
}

inline int Hover_Distance(CoordStruct const &from, CoordStruct const &to)
{
    UnknownVector3 difference = UnknownVector3(from.X - to.X, from.Y - to.Y, from.Z - to.Z);
    return difference.Length();
}

// The height step per tube direction; the count is read after the caller has built the coordinate.
inline int Tube_Height_Step(CoordStruct const &exit, TubeClass const *tube, int current_height)
{
    int count = tube->unknown_1c0;
    return (Map.Get_Height_GL(exit) - current_height) / count;
}

int HoverLocomotionClass::While_Moving_004fcbc0(bool first_pass)
{
    if (unknown_24 != unknown_geometry_l) {
        LinkedTo->Clear_Occupy_Bit(unknown_24);
        unknown_24 = unknown_geometry_l;
    }

    if (LinkedTo->unknown_slot202()) {
        return 7;
    }

    if (!Is_Moving() || unknown_18 == unknown_geometry_l) {
        return 0;
    }

    int nextface = LinkedTo->Path[0];

    if (nextface == 8) {
        int tubenum = Map[Object_Coord(LinkedTo)].unknown_116;
        if (tubenum >= 0 && tubenum < unknown_TubeClass_list_008661c8.Count()) {
            LinkedTo->Mark(0);
            LinkedTo->Clear_Occupy_Bit(Object_Coord(LinkedTo));
            TubeClass *tube = unknown_TubeClass_list_008661c8[tubenum];
            Cell exit = tube->ExitCell;
            unknown_24 = exit.As_Coord();
            memcpy(&LinkedTo->Path[0], &LinkedTo->Path[1], sizeof(LinkedTo->Path[0]) * 23);
            LinkedTo->unknown_450 = unknown_24.As_Cell();
            LinkedTo->Path[23] = -1;
            LinkedTo->unknown_574 = tubenum;
            LinkedTo->unknown_575 = 0;
            TPoint2D<short> const &enter = tube->EnterCell;
            CoordStruct diff = Map[Adjacent_Cell(Cell(enter), tube->Directions[0])].unknown_475d60() - CoordStruct(Cell(enter));
            LinkedTo->unknown_45c = diff + Object_Coord(LinkedTo);
            int height = LinkedTo->Get_Height();
            int current_height = Map.Get_Height_GL(Object_Coord(LinkedTo));
            Cell exit2 = tube->ExitCell;
            int z = height + Tube_Height_Step(exit2, tube, current_height);
            LinkedTo->unknown_45c.Z = z;
        } else {
            LinkedTo->Path[0] = -1;
            unknown_24 = unknown_geometry_l;
            Stop();
            LinkedTo->Assign_Destination(0, true);
        }
        return 7;
    }

    if (nextface != -1) {
        unknown_24 = Cell(LinkedTo->unknown_slot104() + AdjacentCell[nextface & 7]).As_Coord();
        unknown_24.Z = Map.Get_Height_GL(unknown_24);
        if (Object_Coord(LinkedTo).Z >= unknown_24.Z + 2 * unknown_geometry_g + unknown_geometry_g) {
            unknown_24.Z += unknown_geometry_j;
        }

        if (!Map[unknown_24].Goodie_Check(LinkedTo) && !LinkedTo->unknown_76) {
            if (LinkedTo->unknown_7d && !LinkedTo->unknown_7a) {
                LinkedTo->Path[0] = -1;
                unknown_24 = unknown_geometry_l;
                Stop();
                return 7;
            }
            return 7;
        }

        if (!LinkedTo->unknown_7d || LinkedTo->unknown_76 || LinkedTo->unknown_7a) {
            return 7;
        }

        CellClass &adjacent = Map[Adjacent_Coord(Object_Coord(LinkedTo), nextface)];
        if (adjacent.IsUnderBridge ^ LinkedTo->unknown_79) {
            LinkedTo->unknown_57b = true;
        }

        Cell c = unknown_24.As_Cell();
        int ok = LinkedTo->Can_Enter_Cell(&Map[c], nextface, LinkedTo->unknown_5d47d0(), 0, 1);

        switch (ok) {
        case 7:
        case 1:
            Map[unknown_24].unknown_478740();
            if (first_pass) {
                unknown_4fda80();
                LinkedTo->unknown_530 = 0;
                LinkedTo->Path[0] = -1;
                Start_Of_Move_004fe0f0(0);
                ok = While_Moving_004fcbc0(false);
            }
            if (ok != 0) {
                unknown_24 = unknown_geometry_l;
                unknown_4fda80();
            }
            return ok;

        case 3:
            unknown_24 = unknown_geometry_l;
            Map.Unknown_0055ACD0(LinkedTo, c);
            unknown_4fda80();
            break;

        case 6:
            if (first_pass) {
                unknown_24 = unknown_geometry_l;
                unknown_4fda80();
                LinkedTo->unknown_530 = 0;
                LinkedTo->Path[0] = -1;
                Start_Of_Move_004fe0f0(0);
                ok = While_Moving_004fcbc0(false);
                break;
            } else {
                if (Hover_Distance(LinkedTo->Center_Coord(), unknown_18) < Rules->unknown_1380
                    && abs(unknown_18.Z - Object_Coord(LinkedTo).Z) < 2 * unknown_geometry_g
                    && Map[Object_Coord(LinkedTo)].Land_Type() != 10) {
                    Stop_Moving();
                    LinkedTo->Assign_Destination(0, true);
                    return 7;
                }
                bool bridge;
                if (!Map[unknown_24].IsUnderBridge || abs(Object_Coord(LinkedTo).Z / unknown_geometry_g - Map[unknown_24].unknown_11b) <= 2) {
                    bridge = false;
                } else {
                    bridge = true;
                }
                Map[unknown_24].Incoming(&unknown_geometry_l, true, true, bridge);
                unknown_24 = unknown_geometry_l;
                unknown_4fda80();
                break;
            }

        case 0:
            LinkedTo->unknown_slot93(2);
            if (!LinkedTo->unknown_7d || LinkedTo->unknown_76 || LinkedTo->unknown_7a) {
                return 7;
            }
            if (LinkedTo->unknown_32d) {
                LinkedTo->Set_Occupy_Bit(unknown_24);
            }
            memcpy(&LinkedTo->Path[0], &LinkedTo->Path[1], sizeof(LinkedTo->Path[0]) * 23);
            LinkedTo->Path[23] = -1;
            LinkedTo->unknown_450 = unknown_24.As_Cell();
            break;

        case 4:
        case 5:
            unknown_24 = unknown_geometry_l;
            if (first_pass) {
                unknown_4fda80();
                LinkedTo->unknown_530 = 0;
                LinkedTo->Path[0] = -1;
                Start_Of_Move_004fe0f0(0);
                ok = While_Moving_004fcbc0(false);
                break;
            }
            if (Map[c].Cell_Object(Point2D(0, 0), false) != 0) {
                if (!LinkedTo->House->Is_Ally((TechnoClass *)Map[c].Cell_Object(Point2D(0, 0), false))) {
                    LinkedTo->Override_Mission(1, Map[c].Cell_Object(Point2D(0, 0), false), 0);
                }
            } else {
                if (Map[c].OverlayIndex != -1 && OverlayTypes[Map[c].OverlayIndex]->Wall) {
                    LinkedTo->Override_Mission(1, &Map[c], 0);
                }
            }
            break;

        case 2:
            unknown_24 = unknown_geometry_l;
            unknown_4fda80();
            if (!LinkedTo->unknown_5a1) {
                LinkedTo->unknown_5a1 = true;
                LinkedTo->unknown_558 = Rules->unknown_13d0;
            }
            if (LinkedTo->unknown_530 == 0) {
                LinkedTo->Path[0] = -1;
            }
            bool blocked = LinkedTo->unknown_5a1 && LinkedTo->unknown_558 == 0;
            Start_Of_Move_004fe0f0((blocked != 0) + 1);
            break;
        }
        return ok;
    }

    Start_Of_Move_004fe0f0(0);
    return 0;
}


// void HoverLocomotionClass::Stop() (0x004FDA50)

void HoverLocomotionClass::Stop()
{
    LinkedTo->Assign_Destination(0, true);
    unknown_50 = 0;
    unknown_48 = 0;
    LinkedTo->unknown_slot306(0);
}


// void HoverLocomotionClass::unknown_4fda80() (0x004FDA80)
// Stops: no speed (+0x48, +0x50 and the object's slot 306) and no head-to
// coordinate (its mark cleared, slot 59). Expanded where it is used; this
// out-of-line copy is never called.

void HoverLocomotionClass::unknown_4fda80()
{
    unknown_50 = 0;
    unknown_48 = 0;
    LinkedTo->unknown_slot306(0);
    if (unknown_24 != unknown_geometry_l) {
        LinkedTo->Clear_Occupy_Bit(unknown_24);
        unknown_24 = unknown_geometry_l;
    }
}


extern double const unknown_facing_origin_79b630;
extern double const unknown_facing_scale_79b628;
inline int Hover_Destination_Distance_004fdaf0(CoordStruct const &from,CoordStruct const &to)
{
    CoordStruct difference=from-to;
    return (int)Fast_Sqrt((double)difference.X*difference.X+(double)difference.Z*difference.Z+(double)difference.Y*difference.Y);
}

inline UnknownFacingDir Hover_Assign_Direction_004fdaf0(CoordStruct const &from,CoordStruct const &to)
{
    UnknownFacingDir direction;
    if (from.Equal_XY_004bfb60(to)) direction.Raw=0;
    direction=UnknownFacingDir((unsigned short)(int)((Fast_Atan2(double(from.Y)-to.Y,double(to.X)-from.X)-unknown_facing_origin_79b630)*unknown_facing_scale_79b628));
    return direction;
}
inline UnknownFacingDir Hover_Direction_004fdaf0(CoordStruct const &from,CoordStruct const &to)
{
    int from_x=from.X;
    int to_x=to.X;
    return UnknownFacingDir((unsigned short)(int)((Fast_Atan2(double(from.Y)-to.Y,double(to_x)-from_x)-unknown_facing_origin_79b630)*unknown_facing_scale_79b628));
}
inline UnknownVector3 Hover_Vector_Difference_004fdaf0(CoordStruct const &from,CoordStruct const &to)
{
    UnknownVector3 difference;
    difference.X=from.X-to.X;
    difference.Y=from.Y-to.Y;
    difference.Z=from.Z-to.Z;
    return difference;
}
#define Hover_Min_004fdaf0(a,b) ((a)<(b)?(a):(b))
#define Hover_Max_004fdaf0(a,b) ((a)>(b)?(a):(b))
void HoverLocomotionClass::Motion_AI_004fdaf0()
{
    if (Is_Moving() && unknown_24!=unknown_geometry_l) {
        UnknownFacingDir direction=Hover_Assign_Direction_004fdaf0(LinkedTo->Center_Coord(),unknown_24);
        if (!unknown_70) unknown_30.Set_Desired(direction);
        else unknown_30.Set(direction);
        if ((Is_Powered() && abs(0x2000)>=abs((short)(unknown_30.Current().Raw-direction.Raw))) || unknown_70) {
            if ((unknown_18==unknown_geometry_l && Hover_Vector_Difference_004fdaf0(LinkedTo->Center_Coord(),unknown_24).Length()<256) ||
                (unknown_18!=unknown_geometry_l && Hover_Destination_Distance_004fdaf0(LinkedTo->Center_Coord(),unknown_18)<256)) {
                unknown_48=0.5;
            } else {
                unknown_48=1.0;
                if (unknown_70) unknown_50=1.0;
            }
        } else unknown_48=0.0;
        if (unknown_48>0.0) {
            unknown_58=1.0;
            if (LinkedTo->Path[0]!=-1 && LinkedTo->Path[0]==LinkedTo->Path[1]) unknown_58=Rules->HoverBoost;
        }
        double acceleration=Hover_Min_004fdaf0(1.0,unknown_58*unknown_48);
        if (acceleration>unknown_50) {
            unknown_50+=1.0/(Rules->HoverAcceleration*900);
            unknown_50=Hover_Min_004fdaf0(unknown_50,acceleration);
        }
        if (acceleration<unknown_50) {
            unknown_50-=1.0/(Rules->HoverBrake*900);
            unknown_50=Hover_Max_004fdaf0(unknown_50,0.0);
        }
        if (unknown_48>0.0 && !unknown_70) {
            CoordStruct coord=unknown_24;
            if (LinkedTo->Path[0]!=-1 && LinkedTo->Path[0]!=8)
                coord=Hover_Adjacent_Coord_004fb9d0(coord,LinkedTo->Path[0]);
            UnknownFacingDir facing=Hover_Direction_004fdaf0(LinkedTo->Center_Coord(),coord);
            LinkedTo->unknown_2e0.Set_Desired(facing);
        }
        return;
    }
    if (Is_Moving() && unknown_24==unknown_geometry_l) {
        LinkedTo->unknown_slot306(1.0);
        Start_Of_Move_004fe0f0(0);
        While_Moving_004fcbc0(true);
    }
}
#undef Hover_Min_004fdaf0
#undef Hover_Max_004fdaf0


// void __stdcall HoverLocomotionClass::Stop_Moving() (0x004FDF40, ILocomotion 18)
// Drops a destination that differs from the head-to coordinate.

void __stdcall HoverLocomotionClass::Stop_Moving()
{
    if (unknown_18 != unknown_24) {
        unknown_18 = unknown_geometry_l;
        LinkedTo->unknown_4d0 = -1;
    }
}


// void __stdcall HoverLocomotionClass::Do_Turn(UnknownFacingDir dir) (0x004FDF90, ILocomotion 19)

void __stdcall HoverLocomotionClass::Do_Turn(UnknownFacingDir dir)
{
    LinkedTo->unknown_2e0.Set_Desired(UnknownFacingDir(dir));
}


// bool HoverLocomotionClass::unknown_4fdfc0() (0x004FDFC0)
// Moving (Is_Moving as a bool test). Expanded where it is used; this
// out-of-line copy is never called.

bool HoverLocomotionClass::unknown_4fdfc0()
{
    return Is_Moving() ? true : false;
}


inline int Hover_Distance_004fdfd0(CoordStruct const &from,CoordStruct const &to)
{
    CoordStruct difference=from-to;
    return (int)Fast_Sqrt((double)difference.X*difference.X+(double)difference.Y*difference.Y+(double)difference.Z*difference.Z);
}
bool HoverLocomotionClass::Reduce_Path_Length_004fdfd0()
{
    if (Is_Moving() && LinkedTo->Path[0]!=-1) {
        if ((LinkedTo->unknown_494 && LinkedTo->unknown_494->Kind_Of()==1) ||
            (LinkedTo->unknown_494 && LinkedTo->unknown_494->Kind_Of()==15)) {
            int distance=Hover_Distance_004fdfd0(LinkedTo->Center_Coord(),unknown_18)/256;
            if (distance<24) { LinkedTo->Path[distance]=-1; return true; }
        }
        if (LinkedTo->unknown_228 && LinkedTo->unknown_slot213(LinkedTo->unknown_228)) {
            LinkedTo->Path[0]=-1;
            return true;
        }
    }
    return false;
}


#include "shared/adjacent_cell.hpp"
#include "shared/team.hpp"
#include "shared/sound_play.hpp"
inline int Hover_Close_Distance_004fe0f0(CoordStruct const &from,CoordStruct const &to)
{
    CoordStruct difference=from-to;
    return (int)Fast_Sqrt((double)difference.X*difference.X+(double)difference.Z*difference.Z+(double)difference.Y*difference.Y);
}
inline int Hover_Foot_Distance_004fe0f0(CoordStruct const &from,CoordStruct const &to)
{
    UnknownVector3 difference=UnknownVector3(from.X-to.X,from.Y-to.Y,from.Z-to.Z);
    return (int)Fast_Sqrt((double)difference.Z*difference.Z+(double)difference.Y*difference.Y+(double)difference.X*difference.X);
}
void HoverLocomotionClass::Start_Of_Move_004fe0f0(int num)
{
    Reduce_Path_Length_004fdfd0();
    int facing=LinkedTo->Path[0];
    if (unknown_18!=unknown_geometry_l) {
        if (LinkedTo->unknown_slot104()==Coord_Cell(unknown_18) && abs(LinkedTo->unknown_slot19(0).Z-unknown_18.Z)<2*unknown_geometry_g) {
            Stop_Moving();
            LinkedTo->Assign_Destination(0,true);
            return;
        }
    }
    if (facing!=-1) return;
    if (unknown_18==unknown_geometry_l) return;
    if (LinkedTo->unknown_slot104()==Coord_Cell(unknown_18) && abs(LinkedTo->unknown_slot19(0).Z-unknown_18.Z)<=2*unknown_geometry_g) return;
    if (LinkedTo->unknown_530!=0) return;
    if (!LinkedTo->unknown_4c3000(Coord_Cell(unknown_18),0,num)) {
        LinkedTo->unknown_530=CDTimerClass<FrameTimerClass>((int)(Rules->unknown_13c8*900));
        if (!LinkedTo->unknown_slot171(unknown_18)) {
            LinkedTo->Assign_Destination(0,true);
            return;
        }
        if (!LinkedTo->unknown_4c8d60() && Hover_Foot_Distance_004fe0f0(LinkedTo->Center_Coord(),unknown_18)<Rules->unknown_1380 && (LinkedTo->Get_Mission()==2 || LinkedTo->Get_Mission()==11)) {
            LinkedTo->Assign_Destination(0,true);
            if (!LinkedTo->unknown_7d) return;
        } else {
            if (LinkedTo->unknown_53c>0) --LinkedTo->unknown_53c;
            else {
                LinkedTo->Assign_Destination(0,true);
                if (!LinkedTo->unknown_7d) return;
                if (LinkedTo->unknown_57a) Voc_Play(Rules->unknown_620,1.0f,0x2000,0);
                LinkedTo->unknown_57a=false;
            }
        }
        if (!Is_Moving() && LinkedTo->unknown_228 && !LinkedTo->unknown_slot213(LinkedTo->unknown_228)) {
            LinkedTo->unknown_578=true;
            if (LinkedTo->Team) LinkedTo->Team->unknown_func_6baa30();
            LinkedTo->unknown_slot220(0);
        }
        if (unknown_24!=unknown_geometry_l) {
            LinkedTo->Clear_Occupy_Bit(unknown_24);
            unknown_24=unknown_geometry_l;
        }
        Stop_Moving();
        return;
    } else {
    LinkedTo->unknown_530=CDTimerClass<FrameTimerClass>((int)(Rules->unknown_13c8*900));
    Cell cell=Coord_Cell(LinkedTo->Center_Coord())+AdjacentCell[LinkedTo->Path[0]&7];
    if (Map.Unknown_0055A660(cell,true)) {
        int ok=LinkedTo->Can_Enter_Cell(&Map[cell],LinkedTo->Path[0],LinkedTo->unknown_5d47d0(),0,true);
        if (ok==6) {
            CellClass *cellptr=&Map[cell];
            TechnoClass *blockage=static_cast<TechnoClass *>(cellptr->Cell_Techno(Point2D(0,0),Object_Coord(LinkedTo).Z>Map.Get_Height_GL(CoordStruct(cell.X*256+128,cell.Y*256+128,0))+3*unknown_geometry_g,0));
            if (blockage && LinkedTo->House->Is_Ally(blockage)) {
                if (Hover_Close_Distance_004fe0f0(LinkedTo->Center_Coord(),unknown_18)<Rules->unknown_1380 && !LinkedTo->HasAnyLink() && abs(unknown_18.Z-Object_Coord(LinkedTo).Z)<2*unknown_geometry_g && Map[Object_Coord(LinkedTo)].unknown_ec!=10) {
                    Stop_Moving();
                    LinkedTo->Assign_Destination(0,true);
                    return;
                } else {
                    bool bridge=(cellptr->unknown_140&0x100) && abs(Object_Coord(LinkedTo).Z/unknown_geometry_g-cellptr->unknown_11b)>2;
                    cellptr->Incoming(&unknown_geometry_l,true,true,bridge);
                }
            }
        }
    }
    LinkedTo->unknown_53c=10;
    }
}


// bool __stdcall HoverLocomotionClass::Power_Off() (0x004FE800, ILocomotion 23)
// A powered unit on a mission starts drifting; a moving one stops.

bool __stdcall HoverLocomotionClass::Power_Off()
{
    if (Is_Powered() && LinkedTo->Mission != 0 && LinkedTo->Mission != 7) {
        Start_Drift();
    }
    if (Is_Moving()) {
        Stop_Moving();
    }
    return LocomotionClass::Power_Off();
}


// bool __stdcall HoverLocomotionClass::Is_Powered() (0x004FE880, ILocomotion 24)
// Powered, or still in the air (slot 108 positive).

bool __stdcall HoverLocomotionClass::Is_Powered()
{
    if (LocomotionClass::Is_Powered() || LinkedTo->Get_Height_AGL() > 0) {
        return true;
    }
    return false;
}


bool __stdcall HoverLocomotionClass::Is_Ion_Sensitive()
{
    if (LinkedTo->HasAnyLink()) {
        BuildingClass *contact=static_cast<BuildingClass *>(LinkedTo->Contact(0));
        if (contact->Kind_Of()==6) {
            contact=static_cast<BuildingClass *>(LinkedTo->Contact(0));
            if (contact->Type->WeaponsFactory) return false;
        }
    }
    BuildingClass *building=static_cast<BuildingClass *>(Map[Object_Coord(LinkedTo)].Cell_Building());
    if (building && building->Type->WeaponsFactory) {
        Cell difference=Cell(Object_Coord(LinkedTo))-Cell(Object_Coord(building));
        if (difference.X==0 && difference.Y==1) return false;
        else if (difference.X==2 && difference.Y==1) return false;
        else if (difference.X==3 && difference.Y==1) return false;
    }
    return true;
}


inline Cell Hover_Adjacent_Cell_004fea20(Cell const &cell,int facing)
{
    return cell+AdjacentCell[facing&7];
}
bool __stdcall HoverLocomotionClass::Push(UnknownFacingDir dir)
{
    if (Is_Powered() && !unknown_70) {
        int facing=Dir_To_8(dir);
        Cell cell=Hover_Adjacent_Cell_004fea20(LinkedTo->unknown_slot104(),facing);
        FootClass *linked=LinkedTo;
        if (linked->Can_Enter_Cell(&Map[cell],-1,-1,0,true)==0) {
            unknown_70=true;
            if (Is_Moving()) {
                LinkedTo->Path[0]=-1;
                if (unknown_24!=unknown_geometry_l) LinkedTo->Clear_Occupy_Bit(unknown_24);
                unknown_24=CoordStruct(cell.X*256+128,cell.Y*256+128,0);
                unknown_24.Z=Map.Get_Height_GL(unknown_24);
                if (Object_Coord(LinkedTo).Z>=unknown_24.Z+unknown_geometry_g*3) unknown_24.Z+=unknown_geometry_j;
                LinkedTo->Set_Occupy_Bit(unknown_24);
            } else {
                Move_To(CoordStruct(cell.X*256+128,cell.Y*256+128,0));
            }
            return true;
        }
    }
    return false;
}


// bool __stdcall HoverLocomotionClass::Shove(UnknownFacingDir dir) (0x004FEBD0, ILocomotion 27)
// A successful push also starts a drift.

bool __stdcall HoverLocomotionClass::Shove(UnknownFacingDir dir)
{
    if (Push(dir)) {
        Start_Drift();
        return true;
    }
    return false;
}


// void HoverLocomotionClass::Start_Drift() (0x004FEC30)
// A drift of 20..30, either way at random. Power_Off and Shove expand it;
// this out-of-line copy is never called.

void HoverLocomotionClass::Start_Drift()
{
    unknown_68 = true;
    unknown_6c = unknown_id_source_a3d290->Random()(20, 30);
    if (unknown_id_source_a3d290->Random()(0, 99) < 50) {
        unknown_6c = -unknown_6c;
    }
}


// HRESULT __stdcall HoverLocomotionClass::GetClassID(GUID *clsid) (0x004FEC80)

HRESULT __stdcall HoverLocomotionClass::GetClassID(GUID *clsid)
{
    if (clsid == 0) {
        return E_POINTER;
    }
    *clsid = CLSID_HoverLocomotionClass;
    return S_OK;
}


// HRESULT __stdcall HoverLocomotionClass::Load(IStream *stream) (0x004FECC0)
// LocomotionClass::Load, then rebuilt in place.

HRESULT __stdcall HoverLocomotionClass::Load(IStream *stream)
{
    HRESULT result = LocomotionClass::Load(stream);
    if (SUCCEEDED(result)) {
        new (this) HoverLocomotionClass(NoInitClass());
    }
    return result;
}


// int __stdcall HoverLocomotionClass::Layer() (0x004FED10, ILocomotion 29)

int __stdcall HoverLocomotionClass::Layer()
{
    return 2;
}


// void HoverLocomotionClass::unknown_4fed20() (0x004FED20)
// Full speed (+0x48 and the object's slot 306), the 0x004FE0F0 step, then
// 0x004FCBC0 unless the object already stands in the head-to cell.
// Expanded where it is used; this out-of-line copy is never called.

void HoverLocomotionClass::unknown_4fed20()
{
    bool arrived = unknown_24 == unknown_geometry_l || LinkedTo->unknown_slot104() == unknown_24.As_Cell();
    unknown_48 = 1.0;
    LinkedTo->unknown_slot306(1.0);
    Start_Of_Move_004fe0f0(0);
    if (arrived) {
        While_Moving_004fcbc0(true);
    }
}


void __stdcall HoverLocomotionClass::Mark_All_Occupation_Bits(int mark)
{
    if (mark==0) {
        UnknownCoord434820 coord=Head_To_Coord();
        LinkedTo->Clear_Occupy_Bit(coord);
    }
}


// bool __stdcall HoverLocomotionClass::Is_Moving_Here(CoordStruct to) (0x004FEE20, ILocomotion 40)
// The head-to coordinate is in the cell of the coordinate and within the
// height tolerance of it.

bool __stdcall HoverLocomotionClass::Is_Moving_Here(CoordStruct to)
{
    CoordStruct head = Head_To_Coord();
    if (Coord_Cell(head) == Coord_Cell(to) && abs(head.Z - to.Z) <= unknown_geometry_g) {
        return true;
    }
    return false;
}


// int HoverLocomotionClass::Size() (0x004FEEC0): inline in shared/hover_locomotion.hpp, emitted with the vtable; no source of its own.


// void *HoverLocomotionClass::`scalar deleting destructor'(unsigned int flags) (0x004FEED0)
// Compiler-generated with the vtable (the destructor inlined). No source of its own.


extern "C" void __cdecl release_4fef00(void*);
class Leaf_4fef00 { public: void cleanup();void* get(unsigned int flags); };
void* Leaf_4fef00::get(unsigned int flags) { cleanup(); if (flags & 1) release_4fef00(this); return this; }
