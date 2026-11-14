// abstract.cpp: the original translation unit re-filed as one source (Route A): the accepted
// bodies of its canonical compile group in address order, under the unit's own header
// state (its switches and includes in their original order). The module map
// manifests/original_target/modules/abstract.json carries the addresses and the rows that
// stay in their own files.


#include "shared/abstract.hpp"
#include "shared/vector_class.hpp"


// AbstractClass::AbstractClass() (0x0040FF10)
// No unique ID yet (-1), clean, reference count 0.

AbstractClass::AbstractClass() :
    UniqueID(-1),
    unknown_18(0),
    RefCount(0),
    Dirty(false)
{
    AbstractFlags.IsTechno = 0;
    AbstractFlags.IsObject = 0;
    AbstractFlags.IsFoot = 0;
}


// AbstractClass::AbstractClass(NoInitClass const &noinit) (0x0040FF60)
// Installs the vtables only (the save-game load path).

AbstractClass::AbstractClass(NoInitClass const &noinit)
{
}


// AbstractClass::~AbstractClass() (0x0040FF90)
// Restores the AbstractClass vtables.

AbstractClass::~AbstractClass()
{
}


// int __stdcall AbstractClass::What_Am_I() (0x0040FFB0)
// IRTTITypeInfo: the object kind (Kind_Of); `this` is the +4 interface.

int __stdcall AbstractClass::What_Am_I()
{
    return Kind_Of();
}


// long __stdcall AbstractClass::Fetch_ID() (0x0040FFC0)
// IRTTITypeInfo: the unique ID.

long __stdcall AbstractClass::Fetch_ID()
{
    return UniqueID;
}


// void __stdcall AbstractClass::Create_ID() (0x0040FFD0)
// IRTTITypeInfo: takes a new unique ID from the ID source (0 without one).

void __stdcall AbstractClass::Create_ID()
{
    UniqueID = unknown_id_source_a3d290 == 0 ? 0 : unknown_id_source_a3d290->Next_ID();
}


// HRESULT __stdcall AbstractClass::QueryInterface(GUID const &iid, void **object) (0x00410000)
// IUnknown: IUnknown, IPersistStream and IPersist give the object, IRTTITypeInfo its +4 interface.

HRESULT __stdcall AbstractClass::QueryInterface(GUID const &iid, void **object)
{
    if (object == 0) {
        return E_POINTER;
    }
    *object = 0;
    if (iid == IID_IUnknown_7afce0) {
        *object = static_cast<IPersistStream *>(this);
    }
    if (iid == IID_IPersistStream) {
        *object = static_cast<IPersistStream *>(this);
    }
    if (iid == IID_IPersist) {
        *object = static_cast<IPersistStream *>(this);
    }
    if (iid == IID_IRTTITypeInfo) {
        *object = static_cast<IRTTITypeInfo *>(this);
    }
    if (*object == 0) {
        return E_NOINTERFACE;
    }
    AddRef();
    return S_OK;
}


// unsigned long __stdcall AbstractClass::AddRef() (0x004100A0)
// IUnknown: no reference counting (always 1).

unsigned long __stdcall AbstractClass::AddRef()
{
    return 1;
}


// unsigned long __stdcall AbstractClass::Release() (0x004100B0)
// IUnknown: no reference counting (always 1).

unsigned long __stdcall AbstractClass::Release()
{
    return 1;
}


// HRESULT __stdcall AbstractClass::Save(IStream *stream, BOOL clear_dirty) (0x004100C0)
// IPersistStream: writes the object pointer (for swizzling), then Size_Of() bytes of the object.

HRESULT __stdcall AbstractClass::Save(IStream *stream, BOOL clear_dirty)
{
    if (stream == 0) {
        return E_POINTER;
    }
    AbstractClass *self = this;
    HRESULT result = stream->Write(&self, sizeof(self), 0);
    if (FAILED(result)) {
        return result;
    }
    result = stream->Write(this, Size_Of(), 0);
    if (SUCCEEDED(result) && clear_dirty) {
        Dirty = false;
    }
    return result;
}


// HRESULT __stdcall AbstractClass::Load(IStream *stream) (0x00410120)
// IPersistStream: reads the saved pointer, registers it with the swizzle manager, then reads the object over itself keeping RefCount.

HRESULT __stdcall AbstractClass::Load(IStream *stream)
{
    if (stream == 0) {
        return E_POINTER;
    }
    LONG_PTR id;
    HRESULT result = stream->Read(&id, sizeof(id), 0);
    if (SUCCEEDED(result)) {
        Swizzler.Here_I_Am(id, this);
        long refcount = RefCount;
        result = stream->Read(this, Size_Of(), 0);
        RefCount = refcount;
    }
    return result;
}


// HRESULT __stdcall AbstractClass::GetSizeMax(ULARGE_INTEGER *size) (0x00410180)
// IPersistStream: Size_Of() plus the saved pointer.

HRESULT __stdcall AbstractClass::GetSizeMax(ULARGE_INTEGER *size)
{
    if (size == 0) {
        return E_POINTER;
    }
    size->QuadPart = Size_Of() + sizeof(AbstractClass *);
    return S_OK;
}


// void AbstractClass::Compute_CRC(WWCRCEngine &crc) const (0x004101B0)
// Adds the unique ID and the dirty flag to the CRC.

void AbstractClass::Compute_CRC(WWCRCEngine &crc) const
{
    crc(UniqueID);
    crc(Dirty);
}


// bool AbstractClass::Is_Inactive() const (0x004101E0)
// Virtual slot 17: true.

bool AbstractClass::Is_Inactive() const
{
    return true;
}


// HRESULT __stdcall AbstractClass::IsDirty() (0x004101F0)
// IPersistStream: S_OK when changed since the last clearing save, else S_FALSE.

HRESULT __stdcall AbstractClass::IsDirty()
{
    return !Dirty;
}


// void AbstractClass::Init() (0x00410210)
// Virtual slot 9: nothing.

void AbstractClass::Init()
{
}


// void AbstractClass::Detach(AbstractClass *target, bool all) (0x00410220)
// Virtual slot 10: nothing to detach.

void AbstractClass::Detach(AbstractClass *target, bool all)
{
}


// int AbstractClass::Owner() const (0x00410230)
// Virtual slot 14: no owning house (-1).

int AbstractClass::Owner() const
{
    return -1;
}


// HouseClass *AbstractClass::Owning_House() const (0x00410240)
// Virtual slot 15: no owning house.

HouseClass *AbstractClass::Owning_House() const
{
    return 0;
}


// int AbstractClass::Fetch_Heap_ID() const (0x00410250)
// Virtual slot 16: 0.

int AbstractClass::Fetch_Heap_ID() const
{
    return 0;
}


// CoordStruct AbstractClass::Center_Coord() const (0x00410260)
// Virtual slot 18: the zero coordinate (Abstract.cpp's copy of
// unknown_geometry_l, 0x00839CA8).

CoordStruct AbstractClass::Center_Coord() const
{
    return unknown_geometry_l;
}


// CoordStruct AbstractClass::unknown_slot19(TechnoClass *techno) const (0x00410290)
// Virtual slot 19: Center_Coord (the techno argument is unused).

CoordStruct AbstractClass::unknown_slot19(TechnoClass *techno) const
{
    return Center_Coord();
}


// bool AbstractClass::On_Ground() const (0x004102C0)
// Virtual slot 20: false.

bool AbstractClass::On_Ground() const
{
    return false;
}


// bool AbstractClass::In_Air() const (0x004102D0)
// Virtual slot 21: false.

bool AbstractClass::In_Air() const
{
    return false;
}


// CoordStruct AbstractClass::unknown_slot22() const (0x004102E0)
// Virtual slot 22: Center_Coord.

CoordStruct AbstractClass::unknown_slot22() const
{
    return Center_Coord();
}


// void AbstractClass::AI() (0x00410310)
// Virtual slot 23: nothing per frame.

void AbstractClass::AI()
{
}


// bool AbstractClass::INoticeSink_Unknown(int code, int arg) (0x00410320)
// INoticeSink slot 0: false.

bool AbstractClass::INoticeSink_Unknown(int code, int arg)
{
    return false;
}


// void __stdcall AbstractClass::INoticeSource_Unknown() (0x00410330)
// INoticeSource slot 0: nothing.

void __stdcall AbstractClass::INoticeSource_Unknown()
{
}


// AbstractClass scalar deleting destructor (0x00410340)
// Compiler generated after the unit's last member: the inlined destructor,
// then operator delete when bit 0 of the flags is set.


// AbstractClass::QueryInterface adjustor thunk for IRTTITypeInfo (0x00410380)
// Compiler generated (this - 4, then QueryInterface) at the end of the unit.


// AbstractClass::AddRef adjustor thunk for IRTTITypeInfo (0x00410390)
// Compiler generated (this - 4, then AddRef) at the end of the unit.


// AbstractClass::Release adjustor thunk for IRTTITypeInfo (0x004103A0)
// Compiler generated (this - 4, then Release) at the end of the unit.
