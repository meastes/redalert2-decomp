// drive.cpp: the original translation unit re-filed as one source (Route A): the accepted
// bodies of its canonical compile group in address order, under the unit's own header
// state (its switches and includes in their original order). The module map
// manifests/original_target/modules/drive.json carries the addresses and the rows that
// stay in their own files.


#define RA2_SHIP_SOM
#define RA2_COORD_POINT_Z
#define RA2_TPOINT3D_DIFFERENCE
#define RA2_COORD_TPOINT3D
#define RA2_COORD_CELL_BASE_INIT
#define RA2_COORD_AS_CELL
#define RA2_CELL_AS_COORD
#define RA2_COORD_FROM_CELL_INLINE
#define RA2_TRACK_POINT_STRUCT
#define RA2_SHIP_MOVE_TO_VALUE
#define RA2_SHIP_CELL_ACCESSORS
#include "shared/unknown_geometry.hpp"
#include "shared/unknown_static_rect.hpp"
#include "shared/drive_locomotion.hpp"
#include "shared/com_ptr.hpp"
#include "shared/matrix3d.hpp"
#define Min(a, b) ((a) < (b) ? (a) : (b))
#include "shared/crt_math.hpp"
#include "shared/cell.hpp"
#include "shared/anim.hpp"
#include "shared/rules.hpp"
#include "shared/overlay_type.hpp"
#include "shared/tube.hpp"
#include "shared/adjacent_cell.hpp"
#define MAX(a, b) ((a) > (b) ? (a) : (b))
#define MIN(a, b) ((a) < (b) ? (a) : (b))
#include "shared/house.hpp"
#include "shared/team.hpp"
#include "shared/spawn_manager.hpp"
#include "shared/sound_play.hpp"
#include "shared/unknown_vector3.hpp"
#include "shared/ground_type.hpp"
#include "shared/techno_type.hpp"
#include "shared/facing.hpp"
#include "shared/crt_memory.hpp"
#include "shared/vector_class.hpp"


// Header-static dynamic initializers of the unit at 0x0049F450:  the
// shared/unknown_geometry.hpp constants and unknown_static_rect.hpp
// (0x0049F450-0x0049F636); the other
// drafts of this plan are emitted by these includes.


// Dynamic initializer (0x0049F480, _$E) of b from the header(s) included
// by HeaderStatics_diagonal_init_0049f450.cpp; no source of its own.


// Dynamic initializer (0x0049F4A0, _$E) of c from the header(s) included
// by HeaderStatics_diagonal_init_0049f450.cpp; no source of its own.


// Dynamic initializer (0x0049F4C0, _$E) of angle45 from the header(s) included
// by HeaderStatics_diagonal_init_0049f450.cpp; no source of its own.


// Dynamic initializer (0x0049F4E0, _$E) of angle60 from the header(s) included
// by HeaderStatics_diagonal_init_0049f450.cpp; no source of its own.


// Dynamic initializer (0x0049F500, _$E) of angle90 from the header(s) included
// by HeaderStatics_diagonal_init_0049f450.cpp; no source of its own.


// Dynamic initializer (0x0049F520, _$E) of g from the header(s) included
// by HeaderStatics_diagonal_init_0049f450.cpp; no source of its own.


// Dynamic initializer (0x0049F560, _$E) of h from the header(s) included
// by HeaderStatics_diagonal_init_0049f450.cpp; no source of its own.


// Dynamic initializer (0x0049F590, _$E) of i from the header(s) included
// by HeaderStatics_diagonal_init_0049f450.cpp; no source of its own.


// Dynamic initializer (0x0049F5C0, _$E) of j from the header(s) included
// by HeaderStatics_diagonal_init_0049f450.cpp; no source of its own.


// Dynamic initializer (0x0049F5F0, _$E) of k from the header(s) included
// by HeaderStatics_diagonal_init_0049f450.cpp; no source of its own.


// Dynamic initializer (0x0049F600, _$E) of l from the header(s) included
// by HeaderStatics_diagonal_init_0049f450.cpp; no source of its own.


// Dynamic initializer (0x0049F620, _$E) of static_rect from the header(s) included
// by HeaderStatics_diagonal_init_0049f450.cpp; no source of its own.


// DriveLocomotionClass::DriveLocomotionClass() (0x0049F640)
// No ramps, a fresh slope timer, the destination and head-to coordinate at
// the unit's zero, no track (-1), unlocked (+0x65), no piggybacker.

DriveLocomotionClass::DriveLocomotionClass() :
    unknown_1c(0),
    unknown_20(0),
    unknown_34(unknown_geometry_l),
    unknown_40(unknown_geometry_l),
    unknown_4c(0),
    unknown_50(0),
    unknown_58(-1),
    unknown_5c(-1),
    unknown_60(false),
    unknown_61(false),
    unknown_62(false),
    unknown_63(false),
    unknown_64(false),
    unknown_65(true)
{
}


// DriveLocomotionClass::~DriveLocomotionClass() (0x0049F6E0)
// Releases the carried locomotor.

DriveLocomotionClass::~DriveLocomotionClass()
{
}


// HRESULT __stdcall DriveLocomotionClass::Piggyback_CLSID(GUID *classid) (0x0049F710, IPiggyback 6)
// The class ID of the carried locomotor, or this locomotor's own when none is
// carried; each is queried for IPersist (TibSun Drive.cpp Piggyback_CLSID).

HRESULT __stdcall DriveLocomotionClass::Piggyback_CLSID(GUID *classid)
{
    if (classid == 0) {
        return E_POINTER;
    }
    if (Piggybacker) {
        UnknownComPtr<IPersist, &unknown_uuid_IPersist_7cf208> persist(Piggybacker);
        if (persist == 0) {
            return E_FAIL;
        }
        return persist->GetClassID(classid);
    }
    UnknownHeldComPtr<IPersist, &unknown_uuid_IPersist_7cf208> persist(static_cast<IPersistStream *>(this));
    if (persist == 0) {
        return E_FAIL;
    }
    return persist->GetClassID(classid);
}


// HRESULT __stdcall DriveLocomotionClass::QueryInterface(GUID const &iid, void **object) (0x0049F820)
// The base interfaces, then IPiggyback.

HRESULT __stdcall DriveLocomotionClass::QueryInterface(GUID const &iid, void **object)
{
    HRESULT result = LocomotionClass::QueryInterface(iid, object);
    if (result == E_NOINTERFACE) {
        if (iid == IID_IPiggyback) {
            *object = static_cast<IPiggyback *>(this);
        }
        if (*object == 0) {
            return E_NOINTERFACE;
        }
        AddRef();
        result = S_OK;
    }
    return result;
}


// HRESULT __stdcall DriveLocomotionClass::Load(IStream *stream) (0x0049F880)
// LocomotionClass::Load, rebuilt in place; then a flag and, when set, the
// carried locomotor (OleLoadFromStream).

HRESULT __stdcall DriveLocomotionClass::Load(IStream *stream)
{
    HRESULT result = LocomotionClass::Load(stream);
    if (SUCCEEDED(result)) {
        new (this) DriveLocomotionClass(NoInitClass());
        bool piggybacking;
        stream->Read(&piggybacking, sizeof(piggybacking), 0);
        if (piggybacking) {
            result = OleLoadFromStream(stream, IID_ILocomotion, (void **)&Piggybacker);
        }
    }
    return result;
}


// HRESULT __stdcall DriveLocomotionClass::Save(IStream *stream, BOOL clear_dirty) (0x0049F900)
// LocomotionClass::Save, then whether a locomotor is carried and, if so,
// that locomotor (OleSaveToStream through its IPersistStream).

HRESULT __stdcall DriveLocomotionClass::Save(IStream *stream, BOOL clear_dirty)
{
    HRESULT result = LocomotionClass::Save(stream, clear_dirty);
    if (FAILED(result)) {
        return result;
    }
    bool piggybacking;
    if (Piggybacker != 0) {
        piggybacking = true;
        stream->Write(&piggybacking, sizeof(piggybacking), 0);
        UnknownComPtr<IPersistStream, &unknown_uuid_IPersistStream_7d5f38> persist(static_cast<ILocomotion *>(Piggybacker));
        return OleSaveToStream(persist, stream);
    }
    piggybacking = false;
    stream->Write(&piggybacking, sizeof(piggybacking), 0);
    return result;
}


// HRESULT __stdcall DriveLocomotionClass::Begin_Piggyback(ILocomotion *pointer) (0x0049F9E0, IPiggyback 3)
// Carries the locomotor unless one is already carried.

HRESULT __stdcall DriveLocomotionClass::Begin_Piggyback(ILocomotion *pointer)
{
    if (pointer == 0) {
        return E_POINTER;
    }
    if (Piggybacker == 0) {
        Piggybacker = pointer;
        return S_OK;
    }
    return E_FAIL;
}


// HRESULT __stdcall DriveLocomotionClass::End_Piggyback(ILocomotion **pointer) (0x0049FA30, IPiggyback 4)
// Hands the carried locomotor (and its reference) back.

HRESULT __stdcall DriveLocomotionClass::End_Piggyback(ILocomotion **pointer)
{
    if (pointer == 0) {
        return E_POINTER;
    }
    if (Piggybacker != 0) {
        *pointer = Piggybacker.Pointer;
        Piggybacker.Pointer = 0;
        return S_OK;
    }
    return S_FALSE;
}


// bool __stdcall DriveLocomotionClass::Is_Ok_To_End() (0x0049FA70, IPiggyback 5)
// Once stopped while unlocked and carrying a locomotor.

bool __stdcall DriveLocomotionClass::Is_Ok_To_End()
{
    if (!Is_Moving() && Piggybacker != 0 && unknown_65) {
        return true;
    }
    return false;
}


// Matrix3D *DriveLocomotionClass::unknown_49faa0(Matrix3D *matrix) (0x0049FAA0)
// The slope tilt matrix: while a slope change runs, the blend from the old
// ramp (unknown_20) to the new one by Slope_Ratio; otherwise the new ramp's
// matrix, or identity when level. Draw_Matrix expands it; this copy is
// unreferenced.

Matrix3D *DriveLocomotionClass::unknown_49faa0(Matrix3D *matrix)
{
    Matrix3D tilt;
    tilt.Make_Identity();
    if (!unknown_24.Has_Completed2()) {
        Matrix3D blend;
        double ratio = Slope_Ratio();
        tilt = *Unknown_00718DF0(&blend, unknown_20, unknown_1c, ratio);
    } else if (unknown_1c != 0) {
        Matrix3D ramp;
        tilt = *Unknown_00718D60(&ramp, unknown_1c);
    }
    *matrix = tilt;
    return matrix;
}


// double DriveLocomotionClass::Slope_Ratio() (0x0049FB90)
// How far the slope change has run (1.0 when it has no duration).

double DriveLocomotionClass::Slope_Ratio()
{
    return unknown_24.Progress();
}


// void DriveLocomotionClass::Set_Slope(int ramp) (0x0049FBF0)
// A different ramp starts a three-frame change from the current one.

void DriveLocomotionClass::Set_Slope(int ramp)
{
    if (ramp != unknown_1c) {
        unknown_20 = unknown_1c;
        unknown_1c = ramp;
        unknown_24 = UnknownSlopeTimer(3);
    }
}


// void __stdcall DriveLocomotionClass::Force_New_Slope(int ramp) (0x0049FC30, ILocomotion 31)
// Both ramps become ramp and the slope timer restarts.

void __stdcall DriveLocomotionClass::Force_New_Slope(int ramp)
{
    unknown_20 = ramp;
    unknown_1c = ramp;
    unknown_24 = UnknownSlopeTimer();
}


// bool __stdcall DriveLocomotionClass::Is_Moving() (0x0049FC70, ILocomotion 4)
// A destination is set, or a head-to coordinate other than where the
// object stands (X and Y).

bool __stdcall DriveLocomotionClass::Is_Moving()
{
    if (unknown_34 != unknown_geometry_l) {
        return true;
    }
    if (unknown_40 != unknown_geometry_l) {
        if (unknown_40.X != Object_Coord(LinkedTo).X || unknown_40.Y != Object_Coord(LinkedTo).Y) {
            return true;
        }
    }
    return false;
}


// bool __stdcall DriveLocomotionClass::Is_Moving_Now() (0x0049FD10, ILocomotion 32)
// Turning, or moving toward a head-to coordinate with speed (slot 303).

bool __stdcall DriveLocomotionClass::Is_Moving_Now()
{
    if (LinkedTo->unknown_2e0.Is_Rotating()) {
        return true;
    }
    if (Is_Moving() && unknown_40 != unknown_geometry_l && LinkedTo->unknown_slot303() > 0) {
        return true;
    }
    return false;
}


// CoordStruct __stdcall DriveLocomotionClass::Destination() (0x0049FD80, ILocomotion 5)

CoordStruct __stdcall DriveLocomotionClass::Destination()
{
    return CoordStruct(unknown_34.X, unknown_34.Y, unknown_34.Z);
}


// CoordStruct __stdcall DriveLocomotionClass::Head_To_Coord() (0x0049FDB0, ILocomotion 6)
// The head-to coordinate when set, otherwise where the linked object is.

CoordStruct __stdcall DriveLocomotionClass::Head_To_Coord()
{
    if (unknown_40 != unknown_geometry_l) {
        return CoordStruct(unknown_40.X, unknown_40.Y, unknown_40.Z);
    }
    CoordStruct coord = LinkedTo->Coord;
    return coord;
}


// void __stdcall DriveLocomotionClass::Move_To(COMCoordStruct to) (0x0049FE30, ILocomotion 17)
// Unless the linked object is busy (slots 201, 202, 111, 112), heads for
// the coordinate (raised by the bridge height on flagged cells).

void __stdcall DriveLocomotionClass::Move_To(COMCoordStruct to)
{
    if (!LinkedTo->unknown_slot201() && !LinkedTo->unknown_slot202() && !LinkedTo->unknown_slot111() && !LinkedTo->unknown_slot112()) {
        unknown_34 = CoordStruct(to.X, to.Y, to.Z);
        if (to != unknown_geometry_l) {
            to = CoordStruct(to.X, to.Y, to.Z);
            if (Loco_Cell(to)->Flags & 0x100) {
                unknown_34.Z += unknown_geometry_j;
            }
        }
    }
}


// void __stdcall DriveLocomotionClass::Stop_Moving() (0x0049FEF0, ILocomotion 18)
// With a head-to coordinate, a train's locomotive also stops the cars
// coupled behind it. The speed fraction drops to at most 0.3 and the
// destination is cleared.


void __stdcall DriveLocomotionClass::Stop_Moving()
{
    if (unknown_40 != unknown_geometry_l && LinkedTo->Techno_Type_Class()->IsTrain && !((UnitClass *)LinkedTo)->unknown_5b8) {
        for (UnitClass *car = (UnitClass *)((UnitClass *)LinkedTo)->unknown_5b0; car != 0; ) {
            car->Locomotor.Checked()->Stop_Moving();
            car = (UnitClass *)car->unknown_5b0;
            if (car == 0 || car == car->unknown_5b0) {
                break;
            }
        }
    }
    unknown_50 = Min(unknown_50, 0.3f);
    unknown_34 = unknown_geometry_l;
}


// int DriveLocomotionClass::Is_Level() (0x0049FFC0)
// The slope change has finished and the hull is level (both tilt angles
// under 0.005).

int DriveLocomotionClass::Is_Level()
{
    float ratio = Slope_Ratio();
    if (ratio == 1.0 && fabs(LinkedTo->unknown_280) < 0.005 && fabs(LinkedTo->unknown_284) < 0.005) {
        return true;
    }
    return false;
}


// Not exact (RA2_NON_MATCHING): DriveLocomotionClass::Draw_Matrix at 0x004A0050, best score 77.17, 2026-11-13; differs: 342 instructions in the original, 337 in the draft (shape 94.55)
#ifdef RA2_NON_MATCHING
#include "shared/fast_math.hpp"

// TibSun Get_Slope_Matrix: the slope tilt matrix (0x0049FAA0 here; Draw_Matrix expands it).
static inline Matrix3D Slope_Tilt(DriveLocomotionClass *locomotor)
{
    Matrix3D tilt;
    return *locomotor->unknown_49faa0(&tilt);
}

Matrix3D * __stdcall DriveLocomotionClass::Draw_Matrix(Matrix3D *matrix, int *key)
{
    Matrix3D transform;
    if (!Is_Level()) {
        Matrix3D rotation;
        transform.Make_Identity();
        rotation.Make_Identity();
        float length = LinkedTo->Techno_Type_Class()->unknown_330;
        float width = LinkedTo->Techno_Type_Class()->unknown_338;
        float roll_cos = Fast_Cos(LinkedTo->unknown_284);
        float roll_sin = Fast_Sin(LinkedTo->unknown_284);
        float pitch_cos = Fast_Cos(LinkedTo->unknown_280);
        float pitch_sin = Fast_Sin(LinkedTo->unknown_280);
        int lift = fabs(pitch_sin) * length + fabs(roll_sin) * width;
        int y_shift = (int)(width - (float)(int)(roll_cos * width));
        int y_offset = y_shift;
        int x_shift = (int)(length - (float)(int)(pitch_cos * length));
        if (LinkedTo->unknown_284 < 0.0f) {
            y_offset = -y_shift;
        }
        if (LinkedTo->unknown_280 > 0.0f) {
            x_shift = -x_shift;
        }
        transform.Translate_Z((float)lift);
        rotation.Translate_X((float)y_offset);
        rotation.Translate_Y((float)x_shift);
        rotation.Rotate_X(LinkedTo->unknown_280);
        rotation.Rotate_Y(LinkedTo->unknown_284);
        if (key != 0) {
            *key = -1;
        }
        Matrix3D temp;
        Matrix3D basemtx(*LocomotionClass::Draw_Matrix(&temp, key));
        transform = transform * Slope_Tilt(this) * basemtx * rotation;
    } else {
        if (key != 0) {
            if (*key != -1) {
                int keybase = *key << 6;
                *key = keybase;
                *key += unknown_1c;
            }
        }
        Matrix3D temp;
        Matrix3D basemtx(*LocomotionClass::Draw_Matrix(&temp, key));
        transform = Slope_Tilt(this) * basemtx;
    }
    *matrix = transform;
    return matrix;
}
#endif

// Matrix3D * __stdcall DriveLocomotionClass::Shadow_Matrix(Matrix3D *matrix, int *key) (0x004A0500, ILocomotion 10)
// The base shadow transform; the key is -1 (not cached) unless the unit is
// level.

Matrix3D * __stdcall DriveLocomotionClass::Shadow_Matrix(Matrix3D *matrix, int *key)
{
    if (!Is_Level() && key != 0) {
        *key = -1;
    }
    Matrix3D shadow;
    *matrix = *LocomotionClass::Shadow_Matrix(&shadow, key);
    return matrix;
}


// void __stdcall DriveLocomotionClass::Unlimbo() (0x004A05C0, ILocomotion 20)
// Takes the ramp of the cell the object is placed in.

void __stdcall DriveLocomotionClass::Unlimbo()
{
    Force_New_Slope(((CellClass *)LinkedTo->Get_Cell_Ptr())->unknown_11c);
}


// bool __stdcall DriveLocomotionClass::Process() (0x004A05F0, ILocomotion 16)
// TibSun drive.cpp Process; ShipLocomotionClass::Process 0x006765B0 is the twin. RA2 adds
// the move to a moving navigation target (kind 15) for units, the dust anim every 10
// frames on land type 2 cells and the speed reset once both coordinates are clear.

bool __stdcall DriveLocomotionClass::Process()
{
    Set_Slope(static_cast<CellClass *>(LinkedTo->Get_Cell_Ptr())->unknown_11c);
    if (unknown_58 != -1 && unknown_63) {
        if (While_Moving(false)) {
            return false;
        }
        if (!LinkedTo->unknown_7d) {
            return false;
        }
        if (unknown_58 == -1 && (Is_Moving() || LinkedTo->unknown_4d0 != -1)
            && (LinkedTo->Kind_Of() != 1 || !((UnitClass *)LinkedTo)->unknown_5b9)) {
            bool stop = false;
            if (LinkedTo->Kind_Of() == 1 && LinkedTo->unknown_494 != 0 && LinkedTo->unknown_494->Kind_Of() == 15) {
                CoordStruct coord = LinkedTo->unknown_494->unknown_slot19(LinkedTo);
                if (coord != unknown_34) {
                    Move_To(coord);
                }
            }
            Start_Of_Move(stop, true, false);
            if (stop) {
                return false;
            }
            if (!LinkedTo->unknown_7d) {
                return false;
            }
            While_Moving(true);
            if (!LinkedTo->unknown_7d) {
                return false;
            }
        }
    } else {
        if (LinkedTo->unknown_494 != 0 && LinkedTo->unknown_494->Kind_Of() == 11) {
            CellClass *cell = (CellClass *)LinkedTo->unknown_494;
            if (LinkedTo->unknown_slot104() == cell->Fetch_CellID()) {
                unknown_4a4850();
                return false;
            }
        }
        if (LinkedTo->Mission == 5 && !unknown_63 && unknown_34 != unknown_geometry_l && Object_Coord(LinkedTo) == unknown_34) {
            unknown_4a4850();
            return false;
        }
        if (LinkedTo->unknown_2e0.Is_Rotating()) {
            unknown_62 = true;
        } else {
            if (unknown_62) {
                unknown_62 = false;
                LinkedTo->unknown_slot93(0);
                if (!LinkedTo->unknown_7d || LinkedTo->unknown_76 || LinkedTo->unknown_7a) {
                    return false;
                }
            }
            if ((LinkedTo->Get_Mission() != 5 || Is_Moving()) && LinkedTo->Get_Mission() != 16) {
                if (Is_Moving() || LinkedTo->unknown_4d0 != -1) {
                    if (LinkedTo->unknown_32d && LinkedTo->Get_Mission() != 7 && Is_Moving() && !LinkedTo->unknown_slot171(unknown_34)) {
                        Stop_Driver();
                        if (unknown_4a4850()) {
                            return false;
                        }
                    } else {
                        bool stop = false;
                        Start_Of_Move(stop, true, false);
                        if (stop) {
                            return false;
                        }
                        if (!LinkedTo->unknown_7d) {
                            return false;
                        }
                        While_Moving(false);
                        if (!LinkedTo->unknown_7d) {
                            return false;
                        }
                    }
                } else if (LinkedTo->unknown_325) {
                    Stop_Driver();
                    unknown_50 = 0;
                } else if (LinkedTo->unknown_494 != 0) {
                    Move_To(LinkedTo->unknown_494->unknown_slot19(LinkedTo));
                }
            }
        }
    }
    if (Is_Moving_Now() && Frame % 10 == 0 && !LinkedTo->unknown_79
        && static_cast<CellClass *>(LinkedTo->Get_Cell_Ptr())->Land_Type() == 2 && Rules->unknown_09c != 0) {
        new AnimClass(Rules->unknown_09c, Object_Coord(LinkedTo), 0, 1, 0x600, 0);
    }
    if (unknown_34 == unknown_geometry_l && unknown_40 == unknown_geometry_l && LinkedTo->unknown_4d0 == -1 && LinkedTo->unknown_468 > 0.0) {
        LinkedTo->unknown_slot306(0.0);
    }
    return Is_Moving();
}


// void DriveLocomotionClass::unknown_4a0bc0(CoordStruct const &headto, int mark) (0x004A0BC0)
// Marks (slot 58) or clears (slot 59, mark 0) the cell ahead on the current
// track and the head-to coordinate (Westwood's DriveClass::Mark_Track).

void DriveLocomotionClass::unknown_4a0bc0(CoordStruct const &headto, int mark)
{
    if (headto == unknown_geometry_l) {
        return;
    }
    if (!unknown_60 && unknown_58 != -1) {
        int track = DriveTrackControl[unknown_58].Track;
        if (track) {
            TrackType const *points = DriveRawTracks[track - 1].Track;
            int cell = DriveRawTracks[track - 1].Cell;
            if (cell > -1) {
                int facing = points[cell].Facing;
                if (unknown_5c < cell) {
                    Point2D offset = points[cell].Offset;
                    Point2D point = Smooth_Turn(offset, facing);
                    if (mark == 0) {
                        CoordStruct coord(point.X, point.Y, LinkedTo->Get_Height());
                        LinkedTo->Clear_Occupy_Bit(coord);
                    } else if (mark == 1 || mark == 3) {
                        CoordStruct coord(point.X, point.Y, LinkedTo->Get_Height());
                        LinkedTo->Set_Occupy_Bit(coord);
                    }
                }
            }
        }
    }
    if (mark == 0) {
        LinkedTo->Clear_Occupy_Bit(headto);
    } else if (mark == 1 || mark == 3) {
        LinkedTo->Set_Occupy_Bit(headto);
    }
}


// void __stdcall DriveLocomotionClass::Force_Track(int track, CoordStruct coord) (0x004A0D30, ILocomotion 27)
// Puts the unit on a track at its start; with a coordinate it heads there
// (Start_Driver) and, when that succeeds, it is the destination at full speed.

void __stdcall DriveLocomotionClass::Force_Track(int track, CoordStruct coord)
{
    unknown_58 = track;
    unknown_5c = 0;
    if (coord != unknown_geometry_l) {
        if (Start_Driver(CoordStruct(coord.X, coord.Y, coord.Z))) {
            unknown_34 = coord;
            unknown_50 = 1.0;
        }
    }
}


// bool DriveLocomotionClass::Stop_Driver() (0x004A0E90)
// Clears the head-to coordinate (Westwood's DriveClass::Stop_Driver).
// Expanded where it is used; this out-of-line copy is never called.

bool DriveLocomotionClass::Stop_Driver()
{
    if (unknown_40 != unknown_geometry_l) {
        unknown_40 = unknown_geometry_l;
        unknown_63 = false;
        return true;
    }
    return false;
}


// bool DriveLocomotionClass::Start_Driver(CoordStruct const &headto) (0x004A0EE0)
// Heads for a coordinate: taken (and its track marked) when the object may
// stand in its cell; otherwise dropped again when +0x7D is set (Westwood's
// DriveClass::Start_Driver). Expanded where it is used; this out-of-line
// copy is never called.

bool DriveLocomotionClass::Start_Driver(CoordStruct const &headto)
{
    Stop_Driver();
    if (headto != unknown_geometry_l) {
        unknown_40 = headto;
        unknown_63 = true;
        if (Map[headto].Goodie_Check(LinkedTo) && !LinkedTo->unknown_76) {
            unknown_4a0bc0(headto, 1);
            return true;
        }
        if (LinkedTo->unknown_7d) {
            unknown_40 = unknown_geometry_l;
            unknown_63 = false;
        }
    }
    return false;
}


// void __stdcall DriveLocomotionClass::Do_Turn(UnknownFacingDir dir) (0x004A0FD0, ILocomotion 19)

void __stdcall DriveLocomotionClass::Do_Turn(UnknownFacingDir dir)
{
    LinkedTo->unknown_2e0.Set_Desired(UnknownFacingDir(dir));
}


// bool DriveLocomotionClass::While_Moving(bool just_started) (0x004A1000)
// TibSun DriveClass::While_Moving: accelerates or slows the unit, then moves
// it along its track by the accumulated speed, handling tunnels, crushing,
// track jumps at turns and arrival at the head-to coordinate.

extern Point2D const unknown_adjacent_offsets_00851ac0[8];
CoordStruct __fastcall Lerp(CoordStruct const &from, CoordStruct const &to, float t);

static const double _deaccel = 0.3f;
static const double _sinking = 0.1f;
static const double _sinking_scale = 0.0015f;

#ifndef RA2_ADJACENT_CELL
#define RA2_ADJACENT_CELL
inline Cell Adjacent_Cell(Cell cell, int facing)
{
    Cell const &offset = AdjacentCell[facing & 7];
    return Cell(cell.X + offset.X, cell.Y + offset.Y);
}
#endif

// TibSun Adjacent_Cell(Coord const &, FacingType): the coordinate one cell away (Coord + AdjacentPoint).
inline CoordStruct Adjacent_Coord(CoordStruct const &coord, int facing)
{
    Point2D const &offset = unknown_adjacent_offsets_00851ac0[facing & 7];
    return CoordStruct(coord.X + offset.X, coord.Y + offset.Y, coord.Z);
}

// TibSun TPoint2D - TPoint3D (the coordinate taken as its TPoint2D base).
inline Point2D Point_Difference(Point2D const &point, CoordStruct const &coord)
{
    return Point2D(point.X - coord.X, point.Y - coord.Y);
}

// TibSun Coord(TPoint2D, z): a track point at a height.
inline CoordStruct Point_Coord(Point2D const &point, int z)
{
    return CoordStruct(point.X, point.Y, z);
}

inline CoordStruct::CoordStruct(Point2D const &point, int z) : BASECLASS(point.X, point.Y, z) {}


bool DriveLocomotionClass::While_Moving(bool just_started)
{
    if ((!unknown_63 || unknown_58 == -1) && LinkedTo->Path[0] != 8 || (unknown_62 && !LinkedTo->Techno_Type_Class()->Turret)) {
        unknown_4c = 0;
        return false;
    }
    if (LinkedTo->Techno_Type_Class()->Accelerates) {
        bool not_passive = LinkedTo->Kind_Of() != 1 || !((UnitClass *)LinkedTo)->Type->Passive;
        if (unknown_58 < 0x40 && not_passive) {
            CoordStruct dest_coord = unknown_34;
            dest_coord.Z = Map.Get_Height_GL(dest_coord) + (Map[dest_coord].IsUnderBridge ? unknown_geometry_j : 0);
            int distance = (Object_Coord(LinkedTo) - CoordStruct(dest_coord)).Length();

            bool forced_speed = false;
            double speed = LinkedTo->unknown_468;

            TechnoTypeClass const *tclass = LinkedTo->Techno_Type_Class();
            int maxspeed = LinkedTo->unknown_slot206();

            if (distance < tclass->SlowdownDistance) {
                forced_speed = true;
                speed = MAX(_deaccel, speed - maxspeed * tclass->DeaccelerationFactor);
            } else if (LinkedTo->unknown_325) {
                speed = MAX(_sinking, speed - maxspeed * _sinking_scale);
                forced_speed = true;
            }

            if (LinkedTo->unknown_59f) {
                unknown_50 = MIN(unknown_50, 0.2);
                LinkedTo->unknown_slot306(unknown_50);
            } else if (forced_speed) {
                LinkedTo->unknown_slot306(speed);
            } else if (LinkedTo->unknown_468 < unknown_50) {
                LinkedTo->unknown_slot306(MIN(unknown_50, LinkedTo->unknown_468 + tclass->AccelerationFactor));
            } else if (LinkedTo->unknown_468 > unknown_50) {
                LinkedTo->unknown_slot306(MAX(unknown_50, LinkedTo->unknown_468 - maxspeed * tclass->DeaccelerationFactor));
            }

            if (LinkedTo->Kind_Of() == 1) {
                UnitClass *unit = (UnitClass *)LinkedTo;
                UnitClass *following = (UnitClass *)unit->unknown_5b0;
                if (following != 0) {
                    do {
                        following->unknown_slot306(LinkedTo->unknown_468);
                        following = (UnitClass *)following->unknown_5b0;
                    } while (following && following != following->unknown_5b0);
                }
            }
        }
    } else {
        LinkedTo->unknown_slot306(unknown_50);
    }

    int current_speed = LinkedTo->unknown_slot303();

    int actual;
    actual = unknown_4c + (just_started ? 0 : current_speed);

    int nextface;
    nextface = LinkedTo->Path[0];

    if (nextface == 8 && unknown_58 == -1) {
        LinkedTo->Mark(0);
        Stop_Driver();

        CellClass *cell = (CellClass *)LinkedTo->Get_Cell_Ptr();
        int tubenum = cell->unknown_116;
        if (tubenum >= 0 && tubenum < unknown_TubeClass_list_008661c8.Count()) {
            TubeClass *tube = unknown_TubeClass_list_008661c8[tubenum];
            Cell c = tube->ExitCell;
            unknown_40 = c.As_Coord();
            memcpy((char *)&LinkedTo->Path, (char *)&LinkedTo->Path[1], sizeof(LinkedTo->Path[0]) * 23);
            LinkedTo->Path[23] = -1;
            LinkedTo->unknown_574 = tubenum;
            TPoint2D<short> const &enter = tube->EnterCell;
            LinkedTo->unknown_575 = 0;
            LinkedTo->unknown_45c = Map[Adjacent_Cell(Cell(enter), tube->Directions[0])].unknown_475d60();
            int current_height = Map.Get_Height_GL(Object_Coord(LinkedTo));
            Cell exit = tube->ExitCell;
            int count = tube->unknown_1c0;
            LinkedTo->unknown_45c.Z = current_height + (Map.Get_Height_GL(exit) - current_height) / count;
            unknown_63 = true;
            unknown_58 = -1;
        } else {
            LinkedTo->Path[0] = -1;
            unknown_58 = -1;
            unknown_40 = unknown_geometry_l;
            Stop_Driver();
        }
        return false;
    }

    if (actual > 7) {
        TrackControlStruct const *track;
        TrackType const *ptr;
        int tracknum;
        bool adj;

        track = &DriveTrackControl[unknown_58];
        if (unknown_60) {
            tracknum = track->StartTrack;
        } else {
            tracknum = track->Track;
        }
        ptr = DriveRawTracks[tracknum - 1].Track;

        if (nextface < -1 || nextface > 8) {
            LinkedTo->Path[0] = -1;
            return false;
        }

        adj = false;
        if (nextface != 8 && nextface != -1 && Dir_To_8(UnknownFacingDir((unsigned short)((unsigned char)track->Facing << 8))) != nextface) {
            adj = true;
        }

        while (actual > 7) {
            Point2D offset;
            int dir;

            actual -= 7;

            offset = ptr[unknown_5c].Offset;
            if (offset != Point2D(0, 0) || !unknown_5c) {
                if (LinkedTo->unknown_slot47()) {
                    LinkedTo->Clear_Occupy_Bit(Object_Coord(LinkedTo));
                    LinkedTo->unknown_5a0 = false;
                    LinkedTo->unknown_5a1 = false;
                }

                Cell oldcell;
                if (!unknown_5c) {
                    oldcell = LinkedTo->unknown_slot104();
                } else {
                    dir = ptr[unknown_5c - 1].Facing;
                    Point2D prevoffset = ptr[unknown_5c - 1].Offset;
                    oldcell = CoordStruct(Smooth_Turn(prevoffset, dir), 0).As_Cell();
                }

                dir = ptr[unknown_5c].Facing;
                CoordStruct newcoord = Point_Coord(Smooth_Turn(offset, dir), 0);
                Cell newcoord_cell = newcoord.As_Cell();
                Cell linked_coord_cell = Object_Coord(LinkedTo).As_Cell();
                Cell newcell = newcoord.As_Cell();

                if (newcoord_cell != linked_coord_cell) {
                    LinkedTo->Mark(0);
                    LinkedTo->Set_Coord(newcoord);
                    CellClass *oldcellptr = &Map[oldcell];
                    CellClass *newcellptr = &Map[newcell];
                    if (newcellptr->unknown_11b == oldcellptr->unknown_11b - 4 && newcellptr->IsUnderBridge) {
                        LinkedTo->unknown_79 = true;
                    }
                    if (!newcellptr->IsUnderBridge && oldcellptr->IsUnderBridge) {
                        LinkedTo->unknown_79 = false;
                    }
                    if (LinkedTo->Techno_Type_Class()->IsTrain && !((UnitClass *)LinkedTo)->unknown_5b8) {
                        FootClass *train = LinkedTo;
                        bool is_bridge = train->unknown_79 || (Object_Coord(LinkedTo).Z >= (Map.Get_Height_GL(Object_Coord(LinkedTo)) + unknown_geometry_j));
                        ObjectClass *object = is_bridge ? Map[newcell].AltOccupierPtr : Map[newcell].OccupierPtr;
                        ObjectClass *&ref = object;
                        ObjectClass *occupier = ref;
                        while (occupier != 0) {
                            ObjectClass *next = occupier->unknown_30;
                            if (!occupier->Class_Of()->unknown_1fd) {
                                int strength = 10000;
                                occupier->Take_Damage(&strength, 0, Rules->unknown_c74, 0, true, true);
                                strength = 20;
                                train->Take_Damage(&strength, 0, Rules->unknown_c74, 0, true, false);
                            }
                            occupier = next;
                        }
                    }
                    if (!LinkedTo->unknown_7d) return false;
                    LinkedTo->Mark(1);

                    if (newcellptr->OverlayIndex != -1) {
                        OverlayTypeClass *otype = OverlayTypes[newcellptr->OverlayIndex];
                        if (unknown_64 && (LinkedTo->Techno_Type_Class()->Crusher || LinkedTo->unknown_6d7910(0x11)) && otype->ArrayIndex == 0) {
                            LinkedTo->unknown_59f = true;
                            TechnoTypeClass const *ttype = LinkedTo->Techno_Type_Class();
                            if (ttype->TiltsWhenCrushes) {
                                LinkedTo->unknown_28c = -0.02f;
                            }
                        }
                    }
                } else {
                    bool down = LinkedTo->unknown_74;
                    LinkedTo->unknown_74 = false;
                    LinkedTo->Set_Coord(newcoord);
                    LinkedTo->unknown_74 = down;
                }

                if (!LinkedTo->unknown_7d) {
                    return false;
                }

                bool down = LinkedTo->unknown_74;
                LinkedTo->unknown_74 = false;
                LinkedTo->Set_Height_AGL(0);
                LinkedTo->unknown_74 = down;

                LinkedTo->unknown_2e0.Set(UnknownFacingDir((unsigned short)((unsigned char)dir << 8)));

                if (unknown_5c && DriveRawTracks[tracknum - 1].Cell == unknown_5c) {
                    LinkedTo->Clear_Occupy_Bit(Object_Coord(LinkedTo));
                }

                if (nextface != 8 && nextface != -1 && adj && DriveRawTracks[tracknum - 1].Jump == unknown_5c && unknown_5c) {
                    TrackControlStruct const *newtrack;
                    int tnum;

                    tnum = Dir_To_8(UnknownFacingDir((unsigned short)((unsigned char)track->Facing << 8))) * 8 + nextface;
                    newtrack = &DriveTrackControl[tnum];
                    if (newtrack->Track && DriveRawTracks[newtrack->Track - 1].Entry) {
                        double oldspeed = LinkedTo->unknown_468;

                        CoordStruct c = Adjacent_Coord(unknown_40, nextface);

                        switch (LinkedTo->Can_Enter_Cell(&Map[c.As_Cell()], nextface, LinkedTo->unknown_5d47d0(), 0, true)) {
                            case 0:
                            case 2:
                                if (LinkedTo->Kind_Of() == 1 && !((UnitClass *)LinkedTo)->Type->Passive) break;
                                unknown_60 = false;
                                unknown_58 = tnum;
                                track = newtrack;

                                tracknum = track->Track;
                                unknown_5c = DriveRawTracks[tracknum - 1].Entry - 1;
                                ptr = DriveRawTracks[tracknum - 1].Track;
                                adj = false;

                                Stop_Driver();
                                unknown_63 = true;
                                LinkedTo->unknown_slot93(2);
                                unknown_63 = false;
                                if (!LinkedTo->unknown_7d || LinkedTo->unknown_76 || LinkedTo->unknown_7a) return false;
                                if (Start_Driver(c)) {
                                    LinkedTo->unknown_slot306(oldspeed);
                                    memcpy((char *)&LinkedTo->Path, (char *)&LinkedTo->Path[1], sizeof(LinkedTo->Path[0]) * 23);
                                    LinkedTo->Path[23] = -1;
                                }
                                break;

                            case 1:
                                Map[c].unknown_478740();
                                break;

                            case 3:
                                {
                                    Cell gate = c.As_Cell();
                                    Map.Unknown_0055ACD0(LinkedTo, gate);
                                }
                                break;

                            case 6: {
                                    bool bridge;
                                    if (!Map[c].IsUnderBridge || abs(Object_Coord(LinkedTo).Z / unknown_geometry_g - Map[c].unknown_11b) <= 2) {
                                        bridge = false;
                                    } else {
                                        bridge = true;
                                    }
                                    Map[unknown_40].Incoming(&unknown_geometry_l, true, true, bridge);
                                }
                                break;
                        }
                    }
                }
                unknown_5c++;

            } else {
                CoordStruct dc = unknown_40 - Object_Coord(LinkedTo);
                int d = abs(dc.X) + abs(dc.Y);
                actual += (int)((1.0 - (double)d / 11.0) * 7.0);
                LinkedTo->unknown_5a0 = true;
                LinkedTo->unknown_5a1 = false;

                Cell here = Object_Coord(LinkedTo).As_Cell();
                if (here != unknown_40.As_Cell()) {
                    LinkedTo->Mark(0);
                    LinkedTo->Set_Coord(unknown_40);
                    LinkedTo->Set_Height_AGL(0);
                    LinkedTo->Mark(1);
                } else {
                    bool down = LinkedTo->unknown_74;
                    LinkedTo->unknown_74 = false;
                    LinkedTo->Set_Coord(unknown_40);
                    LinkedTo->Set_Height_AGL(0);
                    LinkedTo->unknown_74 = down;
                }

                Stop_Driver();
                unknown_58 = -1;
                unknown_5c = 0;

                bool arrived = false;
                if (LinkedTo->unknown_494 != 0 && LinkedTo->unknown_slot104() == LinkedTo->unknown_494->unknown_slot19(LinkedTo).As_Cell()) {
                    if (abs(LinkedTo->unknown_slot19(LinkedTo).Z - unknown_34.Z) < 2 * unknown_geometry_g) {
                        arrived = true;
                        unknown_34 = unknown_geometry_l;
                        Stop_Driver();
                        unknown_63 = false;
                    }
                }

                LinkedTo->unknown_slot93(2);
                if (!LinkedTo->unknown_7d || LinkedTo->unknown_76 || LinkedTo->unknown_7a) return true;

                if (arrived) {
                    LinkedTo->unknown_4cd9b0();
                    LinkedTo->Path[0] = -1;
                    if (LinkedTo->Get_Mission() == 2 && LinkedTo->unknown_slot261(false, true)) {
                        return true;
                    }
                }

                if (LinkedTo->unknown_slot292()) {
                    return true;
                }

                if (!LinkedTo->unknown_7d) return false;

                break;
            }
        }
    }

    unknown_4c = actual;

    if (actual > 0 && unknown_58 > -1) {
        TrackControlStruct const *track;
        TrackType const *ptr;
        int tracknum;

        track = &DriveTrackControl[unknown_58];
        if (unknown_60) {
            tracknum = track->StartTrack;
        } else {
            tracknum = track->Track;
        }
        ptr = DriveRawTracks[tracknum - 1].Track;

        int dir;
        Point2D offset = ptr[unknown_5c].Offset;
        if (offset != Point2D(0, 0) || !unknown_5c) {

            CoordStruct oldcoord = Object_Coord(LinkedTo);

            dir = ptr[unknown_5c].Facing;
            CoordStruct movement(Point_Difference(Smooth_Turn(offset, dir), Object_Coord(LinkedTo)), 0);
            CoordStruct stepcoord = movement + Object_Coord(LinkedTo);
            CellClass *stepcellptr = &Map[stepcoord];

            CoordStruct partial(0, 0, 0);
            movement = Lerp(partial, movement, unknown_4c / 7.0);
            CoordStruct partialcoord = oldcoord + movement;

            CellClass *oldcellptr = &Map[oldcoord];
            CellClass *partialcellptr = &Map[partialcoord];

            CoordStruct coord = Object_Coord(LinkedTo);

            if (partialcellptr != stepcellptr && partialcellptr != oldcellptr) {
                if (unknown_4c > 3) {
                    coord = stepcoord;
                }
            } else {
                coord = partialcoord;
            }

            CellClass *newcellptr = &Map[CoordStruct(coord)];

            if (coord.As_Cell() != Object_Coord(LinkedTo).As_Cell()) {
                LinkedTo->Mark(0);
                LinkedTo->Set_Coord(coord);
                if (newcellptr->unknown_11b == oldcellptr->unknown_11b - 4 && newcellptr->IsUnderBridge) {
                    LinkedTo->unknown_79 = true;
                }
                if (!newcellptr->IsUnderBridge && oldcellptr->IsUnderBridge) {
                    LinkedTo->unknown_79 = false;
                }
                LinkedTo->Mark(1);
            } else {
                bool down = LinkedTo->unknown_74;
                LinkedTo->unknown_74 = false;
                LinkedTo->Set_Coord(coord);
                LinkedTo->unknown_74 = down;
            }
        }
    }

    return false;
}


// bool DriveLocomotionClass::Start_Of_Move(bool &stop_processing, bool retry, bool force_straight) (0x004A2670)
// TibSun DriveClass::Start_Of_Move: prepares the path and picks the next
// track, handling blockages at the next cell.

extern Point2D const unknown_adjacent_offsets_00851ac0[8];

#ifndef RA2_ADJACENT_CELL
#define RA2_ADJACENT_CELL
inline Cell Adjacent_Cell(Cell cell, int facing)
{
    Cell const &offset = AdjacentCell[facing & 7];
    return Cell(cell.X + offset.X, cell.Y + offset.Y);
}
#endif

inline TPoint3D<int> const operator+(CoordStruct const &coord, Point2D const &point)
{
    return TPoint3D<int>(coord.X + point.X, coord.Y + point.Y, coord.Z);
}

inline CoordStruct Adjacent_Coord_SOM(CoordStruct const &coord, int facing)
{
    return coord + unknown_adjacent_offsets_00851ac0[facing & 7];
}

bool DriveLocomotionClass::Start_Of_Move(bool &stop_processing, bool retry, bool force_straight)
{
    int facing = LinkedTo->Path[0];

    if (!Is_Moving() && facing == -1) {
        unknown_61 = false;
        Stop_Driver();
        if (LinkedTo->Get_Mission() == 2) {
            stop_processing = LinkedTo->unknown_slot261(false, true);
        }
        return false;
    }

    if (!(unknown_34 != unknown_geometry_l)) return false;

    if (LinkedTo->unknown_slot111() || LinkedTo->unknown_slot112()) return false;
    if (LinkedTo->SpawnManager != 0 && LinkedTo->SpawnManager->Launching_Count() != 0) return true;
    if (LinkedTo->unknown_slot201() || LinkedTo->unknown_slot202()) return true;

    if (facing != -1) {
        if (LinkedTo->unknown_494 != 0 && LinkedTo->unknown_494->Kind_Of() == 1 || LinkedTo->unknown_494 != 0 && LinkedTo->unknown_494->Kind_Of() == 15) {
            int dist = LinkedTo->Distance(unknown_34) / 256;
            if (dist < 24) {
                LinkedTo->Path[dist] = -1;
                facing = LinkedTo->Path[0];
            }
        }
    }

    if (facing == -1) {
        if (LinkedTo->unknown_530 != 0) {
            return false;
        }
        LinkedTo->unknown_530 = CDTimerClass<FrameTimerClass>((int)(Rules->unknown_13c8 * 900.0));

        if (!LinkedTo->unknown_4c3000((unknown_34).As_Cell(), 0, 0)) {
            if (LinkedTo == 0) {
                stop_processing = true;
                return false;
            }
            if (!LinkedTo->unknown_slot171(unknown_34)) {
                LinkedTo->Assign_Destination(0, true);
                return false;
            }

                if (unknown_34 == unknown_geometry_l) return false;

                if (!LinkedTo->unknown_4c8d60() && LinkedTo->Distance(unknown_34) < Rules->unknown_1380 && (LinkedTo->Get_Mission() == 2 || LinkedTo->Get_Mission() == 11)) {
                    Stop_Driver();
                    if (unknown_4a4850()) return true;
                    if (!LinkedTo->unknown_7d) return false;
                } else {
                    Cell cell = Adjacent_Cell((LinkedTo->Center_Coord()).As_Cell(), Dir_To_8(LinkedTo->unknown_2e0.Current()));
                    if (Map.Unknown_0055A660(cell, true)) {
                        int ok = LinkedTo->Can_Enter_Cell(&Map[cell], Dir_To_8(LinkedTo->unknown_2e0.Current()), LinkedTo->unknown_5d47d0(), 0, true);
                        if (ok == 3) {
                            unknown_4a47c0(cell);
                        } else if (ok == 6) {
                            CellClass *cellptr = &Map[cell];
                            TechnoClass *blockage = (TechnoClass *)cellptr->Cell_Techno(Point2D(0, 0), Object_Coord(LinkedTo).Z > Map.Get_Height_GL(CoordStruct(cell.X * 256 + 128, cell.Y * 256 + 128, 0)) + 2 * unknown_geometry_g, 0);
                            if (blockage != 0 && LinkedTo->House->Is_Ally(blockage) && !LinkedTo->Techno_Type_Class()->IsTrain) {
                                if (LinkedTo->Distance(unknown_34) < Rules->unknown_1380 && !LinkedTo->HasAnyLink()) {
                                    if (abs(unknown_34.Z - Object_Coord(LinkedTo).Z) < 2 * unknown_geometry_g) {
                                        if (Map[Object_Coord(LinkedTo)].Land_Type() != 10) {
                                            Stop_Driver();
                                            if (unknown_4a4850()) return true; return false;
                                        }
                                    }
                                }
                                bool is_bridge;
                                if (!(cellptr->unknown_140 & 0x100) || abs(Object_Coord(LinkedTo).Z / unknown_geometry_g - cellptr->unknown_11b) <= 2) {
                                    is_bridge = false;
                                } else {
                                    is_bridge = true;
                                }
                                cellptr->Incoming(&unknown_geometry_l, true, true, is_bridge);
                            }
                        }
                    }

                    if (LinkedTo->unknown_53c > 0) {
                        LinkedTo->unknown_53c--;
                    } else {
                        Stop_Driver();
                        if (unknown_4a4850()) return true;
                        if (!LinkedTo->unknown_7d) return false;
                        if (LinkedTo->unknown_57a) Voc_Play(Rules->unknown_620, 1.0f, 0x2000, 0);
                        LinkedTo->unknown_57a = false;
                    }
                }

                if (!Is_Moving() && LinkedTo->unknown_228 != 0 && !LinkedTo->unknown_slot213(LinkedTo->unknown_228)) {
                    LinkedTo->unknown_578 = true;
                    if (LinkedTo->Team != 0) LinkedTo->Team->unknown_func_6baa30();
                    LinkedTo->unknown_slot220(0);
                }

                Stop_Driver();
                unknown_58 = -1;
                unknown_61 = false;
                return false;
            
        }

        if (LinkedTo->Path[0] == 8) {
            return false;
        }

        Cell cell = Adjacent_Cell((LinkedTo->Center_Coord()).As_Cell(), LinkedTo->Path[0]);
        if (Map.Unknown_0055A660(cell, true)) {
            int ok = LinkedTo->Can_Enter_Cell(&Map[cell], LinkedTo->Path[0], LinkedTo->unknown_5d47d0(), 0, true);
            if (ok == 3) {
                unknown_4a47c0(cell);
            } else if (ok == 6) {
                CellClass *cellptr = &Map[cell];
                TechnoClass *blockage;
                {
                    CoordStruct center(cell.X * 256 + 128, cell.Y * 256 + 128, 0);
                    blockage = (TechnoClass *)cellptr->Cell_Techno(Point2D(0, 0), Object_Coord(LinkedTo).Z > Map.Get_Height_GL(center) + 2 * unknown_geometry_g, 0);
                }
                if (blockage != 0 && LinkedTo->House->Is_Ally(blockage) && !LinkedTo->Techno_Type_Class()->IsTrain) {
                    if (LinkedTo->Distance(unknown_34) < Rules->unknown_1380 && !LinkedTo->HasAnyLink()) {
                        if (abs(unknown_34.Z - Object_Coord(LinkedTo).Z) < 2 * unknown_geometry_g) {
                            if (Map[Object_Coord(LinkedTo)].Land_Type() != 10) {
                                Stop_Driver();
                                return unknown_4a4850();
                            }
                        }
                    }
                    bool is_bridge;
                    if (!(cellptr->unknown_140 & 0x100) || abs(Object_Coord(LinkedTo).Z / unknown_geometry_g - cellptr->unknown_11b) <= 2) {
                        is_bridge = false;
                    } else {
                        is_bridge = true;
                    }
                    cellptr->Incoming(&unknown_geometry_l, true, true, is_bridge);
                }
            }
        }

        LinkedTo->unknown_53c = 10;
        facing = LinkedTo->Path[0];
    }

    if (facing == 8) {
        return false;
    }

    CoordStruct dest = Adjacent_Coord_SOM(Object_Coord(LinkedTo), facing);

    CellClass const &here = Map[Object_Coord(LinkedTo)];
    int cell_height = here.unknown_11b + (LinkedTo->unknown_79 ? 4 : 0);

    CellClass const &next = Map[Adjacent_Coord_SOM(Object_Coord(LinkedTo), facing)];
    if (LinkedTo->unknown_79 ^ next.IsUnderBridge) {
        LinkedTo->unknown_57b = true;
    }
    if (!LinkedTo->unknown_slot159()) {
        return true;
    }
    if (!unknown_4a47c0((dest).As_Cell())) {
        return true;
    }

    if (UnknownFacingDir(DIR_MIN) < (LinkedTo->unknown_2e0.Current() - UnknownFacingDir((FacingType)facing))) {
        Do_Turn(UnknownFacingDir((FacingType)facing));
        return true;
    } else {
        Cell destcell = (dest).As_Cell();
        CellClass *destptr = &Map[destcell];
        bool crushable_dest = false;
        LinkedTo->Mark(0);
        int cando = LinkedTo->Can_Enter_Cell(destptr, facing, cell_height, 0, true);
        LinkedTo->Mark(1);

        if (cando < 7 && LinkedTo->Techno_Type_Class()->IsTrain) {
            cando = 0;
        } else if ((cando == 5 || cando == 4) && LinkedTo->Techno_Type_Class()->Crusher && destptr->OverlayIndex == 0) {
            cando = 0;
        }

        int overlay = destptr->OverlayIndex;
        if (overlay != -1 && cando == 0 && OverlayTypes[overlay]->unknown_1fd) {
            crushable_dest = true;
        }

        if (cando != 0) {
            if (cando == 3) {
                unknown_4a47c0(destcell);
            } else if (cando == 6) {
                if (!LinkedTo->Techno_Type_Class()->IsTrain) {
                    FootClass *unit = LinkedTo;
                    if (retry) {
                        unit->Path[0] = -1;
                        LinkedTo->unknown_530 = CDTimerClass<FrameTimerClass>(0);
                        return Start_Of_Move(stop_processing, false, false);
                    }
                    int dist = unit->Distance(unknown_34);
                    if (dist >= Rules->unknown_1380
                        || abs(unknown_34.Z - Object_Coord(LinkedTo).Z) >= 2 * unknown_geometry_g
                        || Map[Object_Coord(LinkedTo)].Land_Type() == 10) {
                        bool is_bridge;
                        if (!(Map[destcell].unknown_140 & 0x100) || abs(Object_Coord(LinkedTo).Z / unknown_geometry_g - Map[destcell].unknown_11b) <= 2) {
                            is_bridge = false;
                        } else {
                            is_bridge = true;
                        }
                        Map[destcell].Incoming(&unknown_geometry_l, true, true, is_bridge);
                    } else {
                        Stop_Driver();
                        if (unknown_4a4850()) {
                            return true;
                        }
                    }
                }
            } else if (cando == 1) {
                Map[destcell].unknown_478740();
                if (retry) {
                    LinkedTo->Path[0] = -1;
                    return Start_Of_Move(stop_processing, false, false);
                }
                Stop_Driver();
                return unknown_4a4850();
            }

            Stop_Driver();
            if (cando == 2) {
                if (!LinkedTo->unknown_5a1) {
                    LinkedTo->unknown_5a1 = true;
                    LinkedTo->unknown_558 = CDTimerClass<FrameTimerClass>(Rules->unknown_13d0);
                }
                if (LinkedTo->unknown_530 == 0) {
                    bool around_blockage = LinkedTo->unknown_5a1 && LinkedTo->unknown_558 == 0;
                    bool ok = LinkedTo->unknown_4c3000((unknown_34).As_Cell(), 0, (around_blockage != 0) + 1);
                    if (LinkedTo == 0) {
                        stop_processing = true;
                        return false;
                    }
                    if (ok || LinkedTo->unknown_slot171(unknown_34)) {
                        LinkedTo->unknown_530 = CDTimerClass<FrameTimerClass>((int)(Rules->unknown_13c8 * 900.0));
                        return true;
                    }
                    LinkedTo->Assign_Destination(0, true);
                    return false;
                }
            }

            if (cando == 5 || cando == 4) {
                if (retry) {
                    LinkedTo->Path[0] = -1;
                    LinkedTo->unknown_530 = CDTimerClass<FrameTimerClass>(0);
                    return Start_Of_Move(stop_processing, false, false);
                }
                if (Map[destcell].Cell_Object(Point2D(0, 0), 0)) {
                    if (!LinkedTo->House->Is_Ally((TechnoClass *)Map[destcell].Cell_Object(Point2D(0, 0), 0))) {
                        LinkedTo->Override_Mission(1, Map[destcell].Cell_Object(Point2D(0, 0), 0), 0);
                    }
                } else {
                    if (Map[destcell].OverlayIndex != -1 && OverlayTypes[Map[destcell].OverlayIndex]->Wall) {
                        LinkedTo->Override_Mission(1, &Map[destcell], 0);
                    }
                }
            } else {
                if (LinkedTo->unknown_57a) Voc_Play(Rules->unknown_620, 1.0f, 0x2000, 0);
            }

            if (cando == 7 && retry) {
                LinkedTo->Path[0] = -1;
                LinkedTo->unknown_530 = CDTimerClass<FrameTimerClass>(0);
                return Start_Of_Move(stop_processing, false, false);
            } else if (cando == 7 && !retry) {
                Stop_Driver();
                return unknown_4a4850();
            }

            LinkedTo->unknown_57a = false;
            unknown_58 = -1;
            return true;
        }

        int height;
        int ground;
        if (abs(cell_height - Map[destcell].unknown_11b) < 2) {
            height = Map[destcell].unknown_11b;
            ground = Map[destcell].Land_Type();
        } else {
            height = cell_height;
            ground = 1;
        }

        TechnoTypeClass *ttype = LinkedTo->Techno_Type_Class();
        double speed = Ground[ground].Cost[ttype->SpeedType];
        if (speed > 1.0) speed = 1.0;

        int destheight = Map.Get_Height_GL(Map[destcell].unknown_475d60());
        int unitheight = Map.Get_Height_GL(Object_Coord(LinkedTo));
        if (destheight > unitheight && LinkedTo->Kind_Of() == 1) {
            if (LinkedTo->Techno_Type_Class()->SpeedType == 1) {
                speed *= Rules->TrackedUphill;
            } else {
                speed *= Rules->WheeledUphill;
            }
        } else if (destheight < unitheight && LinkedTo->Kind_Of() == 1) {
            if (LinkedTo->Techno_Type_Class()->SpeedType == 1) {
                speed *= Rules->TrackedDownhill;
            } else {
                speed *= Rules->WheeledDownhill;
            }
        }
        if (speed == 0.0) speed = 0.5;

        if (LinkedTo->Health_Ratio() <= Rules->unknown_1368) {
            speed *= 0.75;
        }
        if (unknown_58 < 0x40) {
            unknown_50 = speed;
        } else {
            if (speed != LinkedTo->unknown_468) {
                LinkedTo->unknown_slot306(speed);
            }
        }

        
        LinkedTo->Overrun_Square(dest.As_Cell(), true);

        int nextface = LinkedTo->Path[1];
        if (nextface == -1) {
            if (LinkedTo->Distance(unknown_34) > 0x200) {
                int patharg = LinkedTo->Techno_Type_Class()->IsTrain ? 1 : 0;
                bool found = LinkedTo->unknown_4c3000((unknown_34).As_Cell(), patharg, 0);
                bool &result = found;
                if (!result) {
                    if (LinkedTo == 0) {
                        stop_processing = true;
                        return found;
                    }
                    if (!LinkedTo->unknown_slot171(unknown_34)) {
                        LinkedTo->Assign_Destination(0, true);
                    }
                }
                nextface = LinkedTo->Path[1];
            }
        }
        if (nextface == 8) nextface = facing;
        if (nextface == -1 || force_straight) {
            nextface = facing;
        }

        CellClass const *adjcell;
        if (nextface != -1 && (adjcell = &((CellClass const *)destptr)->Adjacent_Cell(nextface)) != 0 && adjcell->OverlayIndex != -1 && OverlayTypes[adjcell->OverlayIndex]->unknown_1fd || crushable_dest) {
            nextface = facing;
            unknown_64 = true;
        } else {
            unknown_64 = false;
        }

        unknown_60 = false;
        unknown_58 = facing * 8 + nextface;
        if (DriveTrackControl[unknown_58].Track == 0) {
            unknown_58 = facing * 8 + facing;
        }

        if (DriveTrackControl[unknown_58].Flag & 8) {
            int nextcando;
            if (!Map[destcell].Goodie_Check(LinkedTo) && !LinkedTo->unknown_76) {
                nextcando = 7;
                if (!LinkedTo->unknown_7d) return false;
            } else {
                if (!LinkedTo->unknown_7d) return false;
                dest = Adjacent_Coord_SOM(dest, nextface);
                destcell = (dest).As_Cell();
                nextcando = LinkedTo->Can_Enter_Cell(&Map[destcell], nextface, height, 0, true);
                if (nextcando < 7 && LinkedTo->Techno_Type_Class()->IsTrain) {
                    nextcando = 0;
                } else if ((nextcando == 4 || nextcando == 5) && LinkedTo->Techno_Type_Class()->Crusher && Map[destcell].OverlayIndex == 0) {
                    nextcando = 0;
                }
            }
            if (!LinkedTo->unknown_7d) return false;

            if (nextcando != 0) {
                if (nextcando == 3) {
                    unknown_4a47c0(destcell);
                } else if (nextcando == 2) {
                    return Start_Of_Move(stop_processing, retry, true);
                } else if (nextcando == 6) {
                    if (!LinkedTo->Techno_Type_Class()->IsTrain) {
                        if (retry) {
                            LinkedTo->Path[0] = -1;
                            LinkedTo->unknown_530 = CDTimerClass<FrameTimerClass>(0);
                            return Start_Of_Move(stop_processing, false, false);
                        }
                        if (LinkedTo->Distance_YZX(unknown_34) >= Rules->unknown_1380
                            || abs(unknown_34.Z - Object_Coord(LinkedTo).Z) >= 2 * unknown_geometry_g
                            || Map[Object_Coord(LinkedTo)].Land_Type() == 10) {
                            CellClass *cellptr = &Map[destcell];
                            bool is_bridge;
                            if (!(cellptr->unknown_140 & 0x100) || abs(Object_Coord(LinkedTo).Z / unknown_geometry_g - cellptr->unknown_11b) <= 2) {
                                is_bridge = false;
                            } else {
                                is_bridge = true;
                            }
                            cellptr->Incoming(&unknown_geometry_l, true, true, is_bridge);
                        } else {
                            Stop_Driver();
                            if (unknown_4a4850()) {
                                return true;
                            }
                        }
                    }
                } else if (nextcando == 1) {
                    Map[destcell].unknown_478740();
                    if (retry) {
                        LinkedTo->Path[0] = -1;
                        return Start_Of_Move(stop_processing, false, false);
                    }
                    Stop_Driver();
                    return unknown_4a4850();
                } else if (nextcando == 7) {
                    if (retry) {
                        LinkedTo->Path[0] = -1;
                        LinkedTo->unknown_530 = CDTimerClass<FrameTimerClass>(0);
                        return Start_Of_Move(stop_processing, false, false);
                    }
                    Stop_Driver();
                    return unknown_4a4850();
                }

                LinkedTo->Path[0] = -1;
                unknown_58 = -1;
                dest = unknown_geometry_l;
                if (nextcando == 5 || nextcando == 4) {
                    return Start_Of_Move(stop_processing, retry, true);
                }
            } else {
                memcpy(&LinkedTo->Path[0], &LinkedTo->Path[2], sizeof(LinkedTo->Path[0]) * 22);
                LinkedTo->Path[22] = -1;
                LinkedTo->unknown_57b = true;
            }
        } else {
            memcpy(&LinkedTo->Path[0], &LinkedTo->Path[1], sizeof(LinkedTo->Path[0]) * 23);
        }

        LinkedTo->Path[23] = -1;
        LinkedTo->unknown_450 = (dest).As_Cell();
        LinkedTo->unknown_57a = false;
        unknown_5c = 0;
        if (!Start_Driver(dest)) {
            unknown_58 = -1;
            LinkedTo->Path[0] = -1;
            LinkedTo->unknown_slot306(0);
        }
        return false;
    }
    return false;
}


// 0x004a4730: function not yet identified
//
// Body: 1 bytes (frozen inventory bound).
// Referenced from 0 sites.
// Ghidra prototype: void FUN_004a4730(void)
//
// Types and names are Ghidra placeholders;
// rename and retype as the purpose becomes known.
//

namespace seed_004a4730 {

void  FUN_004a4730(void)
{
    return;
}

}  // namespace seed_004a4730


// Point2D DriveLocomotionClass::Smooth_Turn(Point2D const &offset, int &facing) (0x004A4740)
// A track point and facing turned by the current track's flags (swap,
// mirror X, mirror Y), the point placed relative to the head-to coordinate
// (Westwood's DriveClass::Smooth_Turn).

Point2D DriveLocomotionClass::Smooth_Turn(Point2D const &offset, int &facing)
{
    int workdir = facing;
    int flags = DriveTrackControl[unknown_58].Flag;
    int x = offset.X;
    int y = offset.Y;
    if (flags & 1) {
        int temp = x;
        x = y;
        y = temp;
        workdir = (-64 - workdir) & 0xFF;
    }
    if (flags & 2) {
        x = -x;
        workdir = -workdir & 0xFF;
    }
    if (flags & 4) {
        y = -y;
        workdir = (-128 - workdir) & 0xFF;
    }
    facing = workdir;
    return Point2D(unknown_40.X + x, unknown_40.Y + y);
}


// bool DriveLocomotionClass::unknown_4a47c0(Cell cell) (0x004A47C0)
// Whether the unit may enter the cell (MapClass 0x0055ACD0). Unreferenced.

bool DriveLocomotionClass::unknown_4a47c0(Cell cell)
{
    return Map.Unknown_0055ACD0(LinkedTo, cell);
}


// int __stdcall DriveLocomotionClass::Layer() (0x004A47E0, ILocomotion 29)

int __stdcall DriveLocomotionClass::Layer()
{
    return 2;
}


// HRESULT __stdcall DriveLocomotionClass::GetClassID(GUID *clsid) (0x004A47F0)

HRESULT __stdcall DriveLocomotionClass::GetClassID(GUID *clsid)
{
    if (clsid == 0) {
        return E_POINTER;
    }
    *clsid = CLSID_DriveLocomotionClass;
    return S_OK;
}


// int __stdcall DriveLocomotionClass::Z_Adjust() (0x004A4830, ILocomotion 14)

int __stdcall DriveLocomotionClass::Z_Adjust()
{
    return 0;
}


// int __stdcall DriveLocomotionClass::Z_Gradient() (0x004A4840, ILocomotion 15)

int __stdcall DriveLocomotionClass::Z_Gradient()
{
    return LocomotionClass::Z_Gradient();
}


// bool DriveLocomotionClass::unknown_4a4850() (0x004A4850)
// With nothing in the unit's 0x478 list: slot 260 (0, 1) and false; else
// 0x004CD9B0 and slot 261 (false, true). Unreferenced.

bool DriveLocomotionClass::unknown_4a4850()
{
    if (LinkedTo->unknown_478.Count() == 0) {
        LinkedTo->Assign_Destination(0, true);
        return false;
    }
    LinkedTo->unknown_4cd9b0();
    return LinkedTo->unknown_slot261(false, true);
}


// void __stdcall DriveLocomotionClass::Mark_All_Occupation_Bits(int mark) (0x004A4890, ILocomotion 39)
// Marks the cells toward the head-to coordinate when one is set.

void __stdcall DriveLocomotionClass::Mark_All_Occupation_Bits(int mark)
{
    if (unknown_40 != unknown_geometry_l) {
        unknown_4a0bc0(unknown_40, mark);
    }
}


// bool __stdcall DriveLocomotionClass::Is_Moving_Here(CoordStruct to) (0x004A48E0, ILocomotion 40)
// Whether the unit is headed for the coordinate's cell (within the height
// tolerance): the marked point ahead on its track, else the head-to coordinate.

bool __stdcall DriveLocomotionClass::Is_Moving_Here(CoordStruct to)
{
    UnknownCoord434820 head = Head_To_Coord();
    if (head == unknown_geometry_l) {
        return false;
    }
    if (!unknown_60 && unknown_58 != -1) {
        int track = DriveTrackControl[unknown_58].Track;
        if (track) {
            TrackType const *points = DriveRawTracks[track - 1].Track;
            int cell = DriveRawTracks[track - 1].Cell;
            if (cell > -1) {
                int facing = points[cell].Facing;
                if (unknown_5c < cell) {
                    Point2D offset = points[cell].Offset;
                    Point2D point = Smooth_Turn(offset, facing);
                    CoordStruct coord(point.X, point.Y, Object_Coord(LinkedTo).Z);
                    if (Coord_Cell(coord) == Coord_Cell(to) && abs(coord.Z - to.Z) <= unknown_geometry_g) {
                        return true;
                    }
                }
            }
        }
    }
    if (Coord_Cell(head) == Coord_Cell(to) && abs(head.Z - to.Z) <= unknown_geometry_g) {
        return true;
    }
    return false;
}


bool __stdcall DriveLocomotionClass::Will_Jump_Tracks()
{
    int next_face=LinkedTo->Path[0];
    if (next_face>=-1 && next_face<=8) {
        TrackControlStruct const *track=&DriveTrackControl[unknown_58];
        int track_number;
        if (unknown_60) track_number=track->StartTrack;
        else track_number=track->Track;
        bool turn=false;
        if (next_face!=8 && next_face!=-1 && Dir_To_8(UnknownFacingDir((unsigned short)((unsigned char)track->Facing<<8)))!=next_face) turn=true;
        if (next_face!=8 && next_face!=-1 && turn && DriveRawTracks[track_number-1].Jump==unknown_5c && unknown_5c) {
            int next_track=Dir_To_8(UnknownFacingDir((unsigned short)((unsigned char)track->Facing<<8)))*8+next_face;
            TrackControlStruct const *jump=&DriveTrackControl[next_track];
            if (jump->Track && DriveRawTracks[jump->Track-1].Entry) return true;
        }
    }
    return false;
}


// void __stdcall DriveLocomotionClass::Lock() (0x004A4BA0, ILocomotion 44)

void __stdcall DriveLocomotionClass::Lock()
{
    unknown_65 = false;
}


// void __stdcall DriveLocomotionClass::Unlock() (0x004A4BB0, ILocomotion 45)

void __stdcall DriveLocomotionClass::Unlock()
{
    unknown_65 = true;
}


// int __stdcall DriveLocomotionClass::Get_Track_Number() (0x004A4BC0, ILocomotion)

int __stdcall DriveLocomotionClass::Get_Track_Number()
{
    return unknown_58;
}


// int __stdcall DriveLocomotionClass::Get_Track_Index() (0x004A4BD0, ILocomotion)

int __stdcall DriveLocomotionClass::Get_Track_Index()
{
    return unknown_5c;
}


// int __stdcall DriveLocomotionClass::Get_Speed_Accum() (0x004A4BE0, ILocomotion)

int __stdcall DriveLocomotionClass::Get_Speed_Accum()
{
    return unknown_4c;
}


// LocomotionClass::IsDirty (0x004A4BF0): inline in shared/locomotion.hpp; this unit's copy is the one the linker kept. No source of its own.


// LocomotionClass::Is_Really_Moving_Now (0x004A4C10): inline in shared/locomotion.hpp; this unit's copy is the one the linker kept. No source of its own.


// LocomotionClass::Get_Status (0x004A4C20): inline in shared/locomotion.hpp; this unit's copy is the one the linker kept. No source of its own.


// LocomotionClass::Acquire_Hunter_Seeker_Target (0x004A4C30): inline in shared/locomotion.hpp; this unit's copy is the one the linker kept. No source of its own.


// LocomotionClass::Is_Surfacing (0x004A4C40): inline in shared/locomotion.hpp; this unit's copy is the one the linker kept. No source of its own.


// LocomotionClass::Stop_Movement_Animation (0x004A4C50): inline in shared/locomotion.hpp; this unit's copy is the one the linker kept. No source of its own.


// unsigned long __stdcall DriveLocomotionClass::AddRef() (0x004A4C60)

unsigned long __stdcall DriveLocomotionClass::AddRef()
{
    return LocomotionClass::AddRef();
}


// unsigned long __stdcall DriveLocomotionClass::Release() (0x004A4C70)

unsigned long __stdcall DriveLocomotionClass::Release()
{
    return LocomotionClass::Release();
}


// bool __stdcall DriveLocomotionClass::Is_Piggybacking() (0x004A4C80, IPiggyback 7)

bool __stdcall DriveLocomotionClass::Is_Piggybacking()
{
    return Piggybacker != 0;
}


// int DriveLocomotionClass::Size() (0x004A4CA0): inline in shared/drive_locomotion.hpp, emitted with the vtable; no source of its own.


// void *DriveLocomotionClass::`scalar deleting destructor'(unsigned int flags) (0x004A4CB0)
// Compiler-generated with the vtable (the destructor inlined). No source of its own.


// UnknownComPtr<IPersist>::Release_Held at0x004a4d00.
// Existing authored shared template member emitted by the Drive Piggyback_CLSID/Save bodies.

// Original Drive callers retain every Release_Held call out of line; these
// independently bounded copies are never expanded in this unit.
#pragma auto_inline(off)
template <>
void UnknownComPtr<IPersist, &unknown_uuid_IPersist_7cf208>::Release_Held()
{
    if (Pointer != 0) {
        Pointer->Release();
    }
}

#pragma auto_inline(on)


// UnknownComPtr<IPersistStream>::Release_Held at0x004a4d10.
// Existing authored shared template member emitted by the Drive Piggyback_CLSID/Save bodies.

// Original Drive callers retain every Release_Held call out of line; these
// independently bounded copies are never expanded in this unit.
#pragma auto_inline(off)
template <>
void UnknownComPtr<IPersistStream, &unknown_uuid_IPersistStream_7d5f38>::Release_Held()
{
    if (Pointer != 0) {
        Pointer->Release();
    }
}

#pragma auto_inline(on)


// 0x004a4d20: function not yet identified
//
// Body: 28 bytes (frozen inventory bound).
// Referenced from 7 sites (unconditional call).
// Ghidra prototype: int __fastcall CDTimerClass_FrameTimerClass_Value_004a4d20(int *param_1)
//
// Types and names are Ghidra placeholders;
// rename and retype as the purpose becomes known.
//

namespace seed_004a4d20 {

extern "C" unsigned int Frame_00a40d2c;

int __fastcall CDTimerClass_FrameTimerClass_Value_004a4d20(int *timer_words)
{
    int remaining;
    int elapsed;

    remaining = (int)(timer_words[2]);
    if (*timer_words != -1) {
        elapsed = (int)(Frame_00a40d2c - *timer_words);
        if (elapsed < remaining) {
            return remaining - elapsed;
        }
        remaining = (int)(0);
    }
    return remaining;
}

}  // namespace seed_004a4d20


// DriveLocomotionClass::QueryInterface adjustor thunk for ILocomotion (0x004A4D40)
// Compiler generated (this adjusted, then QueryInterface). No source of its own.


// DriveLocomotionClass::AddRef adjustor thunk for ILocomotion (0x004A4D50)
// Compiler generated (this adjusted, then AddRef). No source of its own.


// DriveLocomotionClass::Release adjustor thunk for ILocomotion (0x004A4D60)
// Compiler generated (this adjusted, then Release). No source of its own.


// DriveLocomotionClass::QueryInterface adjustor thunk for IPiggyback (0x004A4D70)
// Compiler generated (this adjusted, then QueryInterface). No source of its own.


// DriveLocomotionClass::AddRef adjustor thunk for IPiggyback (0x004A4D80)
// Compiler generated (this adjusted, then AddRef). No source of its own.


// DriveLocomotionClass::Release adjustor thunk for IPiggyback (0x004A4D90)
// Compiler generated (this adjusted, then Release). No source of its own.
