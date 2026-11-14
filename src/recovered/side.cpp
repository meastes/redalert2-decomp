// side.cpp: the original translation unit re-filed as one source (Route A): the accepted
// bodies of its canonical compile group in address order, under the unit's own header
// state (its switches and includes in their original order). The module map
// manifests/original_target/modules/side.json carries the addresses and the rows that
// stay in their own files.


#include "shared/side.hpp"


// SideClass::SideClass(char const *ini_name) (0x0067AE10)
// An empty side named ini_name; registers it in Sides.

SideClass::SideClass(char const *ini_name) :
    AbstractTypeClass(ini_name)
{
    Create_ID();
    Sides.Add(this);
}


// SideClass::~SideClass() (0x0067AED0)
// Leaves the dying side's notice lists and Sides; ~TypeList and ~AbstractTypeClass follow. Defined
// here, not inline: the scalar deleting destructor 0x0067B1F0 expands this body.

SideClass::~SideClass()
{
    Unknown_006ECB90(this, true);
    Sides.Delete(this);
}


// int __fastcall SideClass::Find_Index(char const *name) (0x0067AF90)

int __fastcall SideClass::Find_Index(char const *name)
{
    for (int index = 0; index < Sides.Count(); index++) {
        if (_stricmp(Sides[index]->ID, name) == 0) {
            return index;
        }
    }
    return -1;
}


// void SideClass::Compute_CRC(WWCRCEngine &crc) const (0x0067AFD0)
// The AbstractTypeClass data and the number of houses.

void SideClass::Compute_CRC(WWCRCEngine &crc) const
{
    AbstractTypeClass::Compute_CRC(crc);
    crc((long)Houses.Count());
}


// HRESULT __stdcall SideClass::GetClassID(GUID *classid) (0x0067B000)

HRESULT __stdcall SideClass::GetClassID(GUID *classid)
{
    if (classid == 0) {
        return E_POINTER;
    }
    *classid = CLSID_SideClass;
    return S_OK;
}


// HRESULT __stdcall SideClass::Load(IStream *stream) (0x0067B040)
// Restores the side in place and rebuilds its house index list from the
// saved count and entries.

// Rebuilds a list in place from its saved count and elements.
template <class T>
static inline void Load_List(IStream *stream, TypeList<T> &list)
{
    new (&list) TypeList<T>;
    int count;
    stream->Read(&count, sizeof(count), 0);
    for (int index = 0; index < count; index++) {
        T value;
        stream->Read(&value, sizeof(value), 0);
        list.Add(value);
    }
}

HRESULT __stdcall SideClass::Load(IStream *stream)
{
    HRESULT result = AbstractTypeClass::Load(stream);
    if (FAILED(result)) {
        return result;
    }
    new (this) SideClass(NoInitClass());
    Load_List(stream, Houses);
    return S_OK;
}


// HRESULT __stdcall SideClass::Save(IStream *stream, BOOL clear_dirty) (0x0067B160)
// The AbstractClass data, the house count and the houses.

HRESULT __stdcall SideClass::Save(IStream *stream, BOOL clear_dirty)
{
    HRESULT result = AbstractClass::Save(stream, clear_dirty);
    if (FAILED(result)) {
        return result;
    }
    int count = Houses.Count();
    stream->Write(&count, sizeof(count), 0);
    for (int index = 0; index < count; index++) {
        stream->Write(&Houses[index], sizeof(int), 0);
    }
    return S_OK;
}


// int SideClass::Size_Of() const (0x0067B1D0)

int SideClass::Size_Of() const
{
    return sizeof(SideClass);
}


// int SideClass::Kind_Of() const (0x0067B1E0): the side RTTI kind.

int SideClass::Kind_Of() const
{
    return 0x1C;
}


// SideClass scalar deleting destructor (0x0067B1F0): emitted with the vtable;
// it expands the inline ~SideClass (shared/side.hpp), ~TypeList and calls
// ~AbstractTypeClass. No source of its own.
