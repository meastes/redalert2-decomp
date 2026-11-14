// loco.cpp: the original translation unit re-filed as one source (Route A): the accepted
// bodies of its canonical compile group in address order, under the unit's own header
// state (its switches and includes in their original order). The module map
// manifests/original_target/modules/loco.json carries the addresses and the rows that
// stay in their own files.


#include "shared/shape_file.hpp"
#include "shared/unknown_geometry.hpp"
#include "shared/locomotion.hpp"
#include "shared/matrix3d.hpp"
#include "shared/cell.hpp"


// Header-static dynamic initializers of the unit at 0x0053CDD0: the shape_file.hpp empty rect and the
// shared/unknown_geometry.hpp constants (0x0053CDD0-0x0053CFB1); the other
// drafts of this plan are emitted by these includes.


// Dynamic initializer (0x0053CDF0, _$E) of diagonal from the header(s) included
// by HeaderStatics_empty_rect_init_0053cdd0.cpp; no source of its own.


// Dynamic initializer (0x0053CE20, _$E) of b from the header(s) included
// by HeaderStatics_empty_rect_init_0053cdd0.cpp; no source of its own.


// Dynamic initializer (0x0053CE40, _$E) of c from the header(s) included
// by HeaderStatics_empty_rect_init_0053cdd0.cpp; no source of its own.


// Dynamic initializer (0x0053CE60, _$E) of angle45 from the header(s) included
// by HeaderStatics_empty_rect_init_0053cdd0.cpp; no source of its own.


// Dynamic initializer (0x0053CE80, _$E) of angle60 from the header(s) included
// by HeaderStatics_empty_rect_init_0053cdd0.cpp; no source of its own.


// Dynamic initializer (0x0053CEA0, _$E) of angle90 from the header(s) included
// by HeaderStatics_empty_rect_init_0053cdd0.cpp; no source of its own.


// Dynamic initializer (0x0053CEC0, _$E) of g from the header(s) included
// by HeaderStatics_empty_rect_init_0053cdd0.cpp; no source of its own.


// Dynamic initializer (0x0053CF00, _$E) of h from the header(s) included
// by HeaderStatics_empty_rect_init_0053cdd0.cpp; no source of its own.


// Dynamic initializer (0x0053CF30, _$E) of i from the header(s) included
// by HeaderStatics_empty_rect_init_0053cdd0.cpp; no source of its own.


// Dynamic initializer (0x0053CF60, _$E) of j from the header(s) included
// by HeaderStatics_empty_rect_init_0053cdd0.cpp; no source of its own.


// Dynamic initializer (0x0053CF90, _$E) of k from the header(s) included
// by HeaderStatics_empty_rect_init_0053cdd0.cpp; no source of its own.


// Dynamic initializer (0x0053CFA0, _$E) of l from the header(s) included
// by HeaderStatics_empty_rect_init_0053cdd0.cpp; no source of its own.


// LocomotionClass::LocomotionClass() (0x0053CFC0)
// Unlinked, powered and dirty, no references.

LocomotionClass::LocomotionClass() :
    Owner(0),
    LinkedTo(0),
    Powered(true),
    Dirty(true),
    RefCount(0)
{
}


// LocomotionClass::~LocomotionClass() (0x0053CFF0)
// Forgets the linked object.

LocomotionClass::~LocomotionClass()
{
    LinkedTo = 0;
}


// HRESULT __stdcall LocomotionClass::Link_To_Object(void *object) (0x0053D010, ILocomotion 3)
// The object becomes both the owner and the linked object.

HRESULT __stdcall LocomotionClass::Link_To_Object(void *object)
{
    LinkedTo = (FootClass *)object;
    Owner = (FootClass *)object;
    return S_OK;
}


// Matrix3D * __stdcall LocomotionClass::Draw_Matrix(Matrix3D *matrix, int *key) (0x0053D030, ILocomotion 9)
// The body transform: the facing as one of 32 directions turned about Z;
// a key not -1 gains the facing as five more bits.

Matrix3D * __stdcall LocomotionClass::Draw_Matrix(Matrix3D *matrix, int *key)
{
    Matrix3D transform;
    transform.Make_Identity();
    transform.Rotate_Z((float)((Dir_To_32(LinkedTo->unknown_2e0.Current()) - 8) * -0.19634954084936207));
    if (key != 0 && *key != -1) {
        *key <<= 5;
        *key |= Dir_To_32(LinkedTo->unknown_2e0.Current());
    }
    *matrix = transform;
    return matrix;
}


// Matrix3D * __stdcall LocomotionClass::Shadow_Matrix(Matrix3D *matrix, int *key) (0x0053D0D0, ILocomotion 10)
// The shadow transform: the slope matrix of the cell's ramp turned about Z
// by the facing; a key not -1 gains the ramp (six bits) and the facing
// (five bits).

Matrix3D * __stdcall LocomotionClass::Shadow_Matrix(Matrix3D *matrix, int *key)
{
    CoordStruct coord = LinkedTo->Coord;
    int ramp = Map[coord].unknown_11c;
    Matrix3D transform;
    Matrix3D ramp_matrix;
    transform = *Unknown_00718D60(&ramp_matrix, ramp);
    transform.Rotate_Z((float)((Dir_To_32(LinkedTo->unknown_2e0.Current()) - 8) * -0.19634954084936207));
    if (key != 0 && *key != -1) {
        *key = ((*key << 6) + ramp) << 5;
        *key |= Dir_To_32(LinkedTo->unknown_2e0.Current());
    }
    *matrix = transform;
    return matrix;
}


// Point2D __stdcall LocomotionClass::Shadow_Point() (0x0053D1C0, ILocomotion 12)
// The shadow is drawn the object's height (in pixels) lower.

Point2D __stdcall LocomotionClass::Shadow_Point()
{
    return Point2D(0, Unknown_006A2970(LinkedTo->Get_Height_AGL()));
}


// bool __stdcall LocomotionClass::Power_On() (0x0053D1F0, ILocomotion 22)

bool __stdcall LocomotionClass::Power_On()
{
    Powered = true;
    return Is_Powered();
}


// bool __stdcall LocomotionClass::Power_Off() (0x0053D210, ILocomotion 23)

bool __stdcall LocomotionClass::Power_Off()
{
    Powered = false;
    return Is_Powered();
}


// bool __stdcall LocomotionClass::Is_Powered() (0x0053D230, ILocomotion 24)

bool __stdcall LocomotionClass::Is_Powered()
{
    return Powered;
}


// bool __stdcall LocomotionClass::Is_Ion_Sensitive() (0x0053D240, ILocomotion 25)

bool __stdcall LocomotionClass::Is_Ion_Sensitive()
{
    return false;
}


// unsigned long __stdcall LocomotionClass::AddRef() (0x0053D250)
// Counts a live COM object and a reference.

unsigned long __stdcall LocomotionClass::AddRef()
{
    unknown_com_objects_a6eb14++;
    return InterlockedIncrement(&RefCount);
}


// unsigned long __stdcall LocomotionClass::Release() (0x0053D270)
// Drops a reference; the last one deletes the locomotor.

unsigned long __stdcall LocomotionClass::Release()
{
    unknown_com_objects_a6eb14--;
    long count = InterlockedDecrement(&RefCount);
    if (count == 0) {
        delete this;
    }
    return count;
}


// HRESULT __stdcall LocomotionClass::QueryInterface(GUID const &iid, void **object) (0x0053D2B0)
// IUnknown and ILocomotion give the +4 interface, IPersistStream and
// IPersist the object.

HRESULT __stdcall LocomotionClass::QueryInterface(GUID const &iid, void **object)
{
    if (object == 0) {
        return E_POINTER;
    }
    *object = 0;
    if (iid == IID_IUnknown_7afce0) {
        *object = static_cast<ILocomotion *>(this);
    }
    if (iid == IID_IPersistStream) {
        *object = static_cast<IPersistStream *>(this);
    }
    if (iid == IID_ILocomotion) {
        *object = static_cast<ILocomotion *>(this);
    }
    if (iid == IID_IPersist) {
        *object = static_cast<IPersistStream *>(this);
    }
    if (*object == 0) {
        return E_NOINTERFACE;
    }
    AddRef();
    return S_OK;
}


// HRESULT __stdcall LocomotionClass::Save(IStream *stream, BOOL clear_dirty) (0x0053D360, IPersistStream)
// Writes the object's own address (the swizzle key Load registers), then
// the whole object (Size() bytes).

HRESULT __stdcall LocomotionClass::Save(IStream *stream, BOOL clear_dirty)
{
    if (!stream) {
        return E_POINTER;
    }
    LocomotionClass *self = this;
    HRESULT result = stream->Write(&self, sizeof(self), 0);
    if (FAILED(result)) {
        return result;
    }
    result = stream->Write(this, Size(), 0);
    if (FAILED(result)) {
        return result;
    }
    if (clear_dirty) {
        Dirty = false;
    }
    return result;
}


// HRESULT __stdcall LocomotionClass::Load(IStream *stream) (0x0053D3C0)
// Reads the saved address (registered with the swizzle manager) and the
// object's Size() bytes, keeping the reference count; then fixes up the
// owner and linked object.

HRESULT __stdcall LocomotionClass::Load(IStream *stream)
{
    if (stream == 0) {
        return E_POINTER;
    }
    LONG_PTR id;
    HRESULT result = stream->Read(&id, sizeof(id), 0);
    if (FAILED(result)) {
        return result;
    }
    Swizzler.Here_I_Am(id, this);
    long count = RefCount;
    result = stream->Read(this, Size(), 0);
    RefCount = count;
    Swizzler.Swizzle((void **)&LinkedTo);
    Swizzler.Swizzle((void **)&Owner);
    return result;
}


// HRESULT __stdcall LocomotionClass::GetSizeMax(ULARGE_INTEGER *size) (0x0053D440)
// Size() plus the saved address.

HRESULT __stdcall LocomotionClass::GetSizeMax(ULARGE_INTEGER *size)
{
    if (size == 0) {
        return E_POINTER;
    }
    size->QuadPart = (unsigned)(Size() + sizeof(LocomotionClass *));
    return S_OK;
}


// bool __stdcall LocomotionClass::Push(UnknownFacingDir dir) (0x0053D470, ILocomotion)
// Refused here.

bool __stdcall LocomotionClass::Push(UnknownFacingDir dir)
{
    return false;
}


// bool __stdcall LocomotionClass::Shove(UnknownFacingDir dir) (0x0053D480, ILocomotion)
// Refused here.

bool __stdcall LocomotionClass::Shove(UnknownFacingDir dir)
{
    return false;
}


// void __stdcall LocomotionClass::Tilt_Pitch_AI() (0x0053D490, ILocomotion)
// Nothing here.

void __stdcall LocomotionClass::Tilt_Pitch_AI()
{

}


// int __stdcall LocomotionClass::Z_Adjust() (0x0053D4A0, ILocomotion)
// No adjustment.

int __stdcall LocomotionClass::Z_Adjust()
{
    return 0;
}


// int __stdcall LocomotionClass::Z_Gradient() (0x0053D4B0, ILocomotion)
// Gradient 2 (hypothesis: ground level).

int __stdcall LocomotionClass::Z_Gradient()
{
    return 2;
}


// int __stdcall LocomotionClass::Visual_Character(bool raw) (0x0053D4C0, ILocomotion)
// Normal (0).

int __stdcall LocomotionClass::Visual_Character(bool raw)
{
    return 0;
}


// Point2D __stdcall LocomotionClass::Draw_Point() (0x0053D4D0, ILocomotion)
// No offset.

Point2D __stdcall LocomotionClass::Draw_Point()
{
    return Point2D(0, 0);
}


// bool __stdcall LocomotionClass::Is_To_Have_Shadow() (0x0053D4E0, ILocomotion)
// Always.

bool __stdcall LocomotionClass::Is_To_Have_Shadow()
{
    return true;
}


// int __stdcall LocomotionClass::Can_Enter_Cell(Cell cell) (0x0053D4F0, ILocomotion)
// Always allowed (0).

int __stdcall LocomotionClass::Can_Enter_Cell(Cell cell)
{
    return 0;
}


// void __stdcall LocomotionClass::Force_Immediate_Destination(CoordStruct coord) (0x0053D500, ILocomotion)
// Nothing here.

void __stdcall LocomotionClass::Force_Immediate_Destination(CoordStruct coord)
{

}


// void __stdcall LocomotionClass::Force_Track(int track, CoordStruct coord) (0x0053D510, ILocomotion)
// Nothing here.

void __stdcall LocomotionClass::Force_Track(int track, CoordStruct coord)
{

}


// void __stdcall LocomotionClass::Unlimbo() (0x0053D520, ILocomotion)
// Nothing here.

void __stdcall LocomotionClass::Unlimbo()
{

}


// void __stdcall LocomotionClass::Do_Turn(UnknownFacingDir dir) (0x0053D530, ILocomotion)
// Nothing here.

void __stdcall LocomotionClass::Do_Turn(UnknownFacingDir dir)
{

}


// void __stdcall LocomotionClass::Stop_Moving() (0x0053D540, ILocomotion)
// Nothing here.

void __stdcall LocomotionClass::Stop_Moving()
{

}


// void __stdcall LocomotionClass::Move_To(CoordStruct to) (0x0053D550, ILocomotion)
// Nothing here.

void __stdcall LocomotionClass::Move_To(CoordStruct to)
{

}


// bool __stdcall LocomotionClass::Process() (0x0053D560, ILocomotion)
// Nothing to do; reports success.

bool __stdcall LocomotionClass::Process()
{
    return true;
}


// CoordStruct __stdcall LocomotionClass::Destination() (0x0053D570, ILocomotion)
// None: the zero coordinate.

CoordStruct __stdcall LocomotionClass::Destination()
{
    CoordStruct coord = unknown_geometry_l;
    return coord;
}


// CoordStruct __stdcall LocomotionClass::Head_To_Coord() (0x0053D5A0, ILocomotion)
// Where the linked object is.

CoordStruct __stdcall LocomotionClass::Head_To_Coord()
{
    CoordStruct coord = LinkedTo->Coord;
    return coord;
}


// bool __stdcall LocomotionClass::Is_Moving() (0x0053D5D0, ILocomotion)
// Never here.

bool __stdcall LocomotionClass::Is_Moving()
{
    return false;
}


// void __stdcall LocomotionClass::Force_New_Slope(int ramp) (0x0053D5E0, ILocomotion)
// Nothing here.

void __stdcall LocomotionClass::Force_New_Slope(int ramp)
{

}


// int __stdcall LocomotionClass::Drawing_Code() (0x0053D5F0, ILocomotion)
// Code 0.

int __stdcall LocomotionClass::Drawing_Code()
{
    return 0;
}


// int __stdcall LocomotionClass::State() (0x0053D600, ILocomotion)
// State 0.

int __stdcall LocomotionClass::State()
{
    return 0;
}


// int __stdcall LocomotionClass::Apparent_Speed() (0x0053D610, ILocomotion)
// The linked object's speed (FootClass slot 303).

int __stdcall LocomotionClass::Apparent_Speed()
{
    return LinkedTo->unknown_slot303();
}
