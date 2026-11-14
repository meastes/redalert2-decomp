// waypoint.cpp: the original translation unit re-filed as one source (Route A): the accepted
// bodies of its canonical compile group in address order, under the unit's own header
// state (its switches and includes in their original order). The module map
// manifests/original_target/modules/waypoint.json carries the addresses and the rows that
// stay in their own files.


// Header-static dynamic initializers of the unit at 0x00724780 and the
// unit's WaypointPathClass list; the other drafts of this plan are emitted by this file.
#include "shared/unknown_geometry.hpp"
#include "shared/unknown_static_rect.hpp"
#ifndef VECTOR_IMPLEMENT_ALL
#define VECTOR_IMPLEMENT_ALL
#endif
#include "shared/waypoint_path.hpp"

// The unit's list of WaypointPathClass objects (0x00B25590; name is a hypothesis).
DynamicVectorClass<WaypointPathClass *> unknown_WaypointPathClass_list_00b25590;


// Dynamic initializer (0x007247B0, _$E) emitted by HeaderStatics_diagonal_init_00724780.cpp; no source of its own.


// Dynamic initializer (0x007247D0, _$E) emitted by HeaderStatics_diagonal_init_00724780.cpp; no source of its own.


// Dynamic initializer (0x007247F0, _$E) emitted by HeaderStatics_diagonal_init_00724780.cpp; no source of its own.


// Dynamic initializer (0x00724810, _$E) emitted by HeaderStatics_diagonal_init_00724780.cpp; no source of its own.


// Dynamic initializer (0x00724830, _$E) emitted by HeaderStatics_diagonal_init_00724780.cpp; no source of its own.


// Dynamic initializer (0x00724850, _$E) emitted by HeaderStatics_diagonal_init_00724780.cpp; no source of its own.


// Dynamic initializer (0x00724890, _$E) emitted by HeaderStatics_diagonal_init_00724780.cpp; no source of its own.


// Dynamic initializer (0x007248C0, _$E) emitted by HeaderStatics_diagonal_init_00724780.cpp; no source of its own.


// Dynamic initializer (0x007248F0, _$E) emitted by HeaderStatics_diagonal_init_00724780.cpp; no source of its own.


// Dynamic initializer (0x00724920, _$E) emitted by HeaderStatics_diagonal_init_00724780.cpp; no source of its own.


// Dynamic initializer (0x00724930, _$E) emitted by HeaderStatics_diagonal_init_00724780.cpp; no source of its own.


// Dynamic initializer (0x00724950, _$E) emitted by HeaderStatics_diagonal_init_00724780.cpp; no source of its own.


// Dynamic initializer (0x00724970, _$E) emitted by HeaderStatics_diagonal_init_00724780.cpp; no source of its own.


// Dynamic initializer (0x007249B0, _$E) emitted by HeaderStatics_diagonal_init_00724780.cpp; no source of its own.


// Waypoint As String (0x7249F0)

char const *__fastcall Waypoint_As_String(int index)
{
    static char name[8];
    if (index == -1) {
        name[0] = '\0';
        return name;
    }
    if (index < 26) {
        wsprintfA(name, "%c", index + 'A');
        return name;
    }
    wsprintfA(name, "%c%c", index / 26 + '@', index % 26 + 'A');
    return name;
}


// Waypoint From String (0x724A70)

int __fastcall Waypoint_From_String(char const *string)
{
    int index = -1;
    if (isalpha(string[0])) {
        index = toupper(string[0]) - 'A';
        if (isalpha(string[1])) {
            return toupper(string[1]) - 0x27 + index * 26;
        }
    }
    return index;
}


// WaypointClass constructor (0x724AD0)

WaypointClass::WaypointClass()
    : Coord(0, 0, 0)
{
}


// WaypointClass copy constructor (0x724AE0)

WaypointClass::WaypointClass(WaypointClass const &waypoint)
    : Coord(waypoint.Coord)
{
}


// WaypointClass destructor (0x724B00)

WaypointClass::~WaypointClass()
{
}


// WaypointPathClass::WaypointPathClass() (0x00724B10): an empty path with no
// selected waypoint, added to the unit's path list.

WaypointPathClass::WaypointPathClass() :
    Selected(-1)
{
    unknown_WaypointPathClass_list_00b25590.Add(this);
}


// WaypointPathClass::WaypointPathClass(NoInitClass const &noinit) (0x00724BB0): the vtables only.

WaypointPathClass::WaypointPathClass(NoInitClass const &noinit) :
    AbstractClass(noinit),
    Waypoints(noinit)
{
}


// WaypointPathClass::WaypointPathClass(int index) (0x00724BF0): an empty path
// that joins the unit's path list only for path numbers 0..11.

WaypointPathClass::WaypointPathClass(int index) :
    Selected(-1)
{
    if (index >= 0 && index < 12) {
        unknown_WaypointPathClass_list_00b25590.Add(this);
    }
}


// WaypointPathClass destructor (0x724CA0)

WaypointPathClass::~WaypointPathClass()
{
    Unknown_006ECB90(this, true);
    unknown_WaypointPathClass_list_00b25590.Delete(this);
}


// WaypointPathClass Get Waypoint (0x724D60)

WaypointClass *WaypointPathClass::Get_Waypoint(int index)
{
    if (index >= 0 && index < Waypoints.Count()) {
        return &Waypoints[index];
    }
    return 0;
}


// bool WaypointPathClass::Add_Waypoint(CoordStruct const &coord) (0x00724D80):
// adds the centre of coord's cell (at coord's height) unless a waypoint is
// selected; always answers true.
#include "shared/cell.hpp"
#include "shared/object.hpp"

bool WaypointPathClass::Add_Waypoint(CoordStruct const &coord)
{
    WaypointClass waypoint;
    waypoint.Coord = Cell_Center(Coord_Cell(coord));
    waypoint.Coord.Z = coord.Z;
    if (Selected != -1) {
        return true;
    }
    Waypoints.Add(waypoint);
    return true;
}


// WaypointPathClass Select Waypoint (0x724E30)

bool WaypointPathClass::Select_Waypoint(CoordStruct const &coord)
{
    for (int index = 0; index < Waypoints.Count(); index++) {
        if (Cell((short)(Waypoints[index].Coord.X / 256), (short)(Waypoints[index].Coord.Y / 256))
            == Cell((short)(coord.X / 256), (short)(coord.Y / 256))) {
            Selected = index;
            return true;
        }
    }
    return false;
}


// WaypointPathClass Set Waypoint (0x724ED0)

void WaypointPathClass::Set_Waypoint(int index, WaypointClass const &waypoint)
{
    if (index >= 0 && index < Waypoints.Count()) {
        new (&Waypoints[index]) WaypointClass(waypoint);
    }
}


// WaypointPathClass Remove Waypoint (0x724F10)

void WaypointPathClass::Remove_Waypoint(int index)
{
    if (index >= 0 && index < Waypoints.Count()) {
        if (index == Selected || index == Waypoints.Count() - 1) {
            Selected = -1;
        }
        Waypoints.Delete(index);
    }
}


// WaypointPathClass Next Waypoint (0x724F80)

WaypointClass *WaypointPathClass::Next_Waypoint(WaypointClass const *waypoint)
{
    int index = Waypoints.ID(waypoint) + 1;
    if (Selected != -1 && index == Waypoints.Count()) {
        index = Selected;
    }
    if (index >= 0 && index < Waypoints.Count()) {
        return &Waypoints[index];
    }
    return 0;
}


// WaypointPathClass Clear Waypoints (0x724FC0)

void WaypointPathClass::Clear_Waypoints()
{
    Waypoints.Clear();
    Selected = -1;
}


// WaypointPathClass Compute CRC (0x724FE0)

void WaypointPathClass::Compute_CRC(WWCRCEngine &crc) const
{
    AbstractClass::Compute_CRC(crc);
    crc((long)Selected);
    crc((long)Waypoints.Count());
}


// WaypointPathClass GetClassID (0x725010)

HRESULT __stdcall WaypointPathClass::GetClassID(GUID *clsid)
{
    if (clsid == 0) {
        return E_POINTER;
    }
    *clsid = CLSID_WaypointPathClass;
    return S_OK;
}


// HRESULT __stdcall WaypointPathClass::Load(IStream *stream) (0x00725050):
// restores the path in place, rebuilds the waypoint list and reads the
// waypoints back (a count and 12-byte records).

HRESULT __stdcall WaypointPathClass::Load(IStream *stream)
{
    HRESULT result = AbstractClass::Load(stream);
    if (FAILED(result)) {
        return result;
    }
    new (this) WaypointPathClass(NoInitClass());
    int count;
    result = stream->Read(&count, sizeof(count), 0);
    if (FAILED(result)) {
        return result;
    }
    new (&Waypoints) DynamicVectorClass<WaypointClass>;
    for (int index = 0; index < count; index++) {
        WaypointClass waypoint;
        result = stream->Read(&waypoint, sizeof(waypoint), 0);
        if (FAILED(result)) {
            return result;
        }
        Waypoints.Add(waypoint);
    }
    return S_OK;
}


// HRESULT __stdcall WaypointPathClass::Save(IStream *stream, BOOL clear_dirty) (0x00725170)
// Writes the waypoint count and then each waypoint (12 bytes); TibSun waypoint.cpp shape (index
// declared first, early return when the count write fails).

HRESULT __stdcall WaypointPathClass::Save(IStream *stream, BOOL clear_dirty)
{
    HRESULT result = AbstractClass::Save(stream, clear_dirty);
    if (SUCCEEDED(result)) {
        int index;

        int count = Waypoints.Count();
        result = stream->Write(&count, sizeof(count), 0);
        if (FAILED(result)) {
            return result;
        }

        for (index = 0; index < count; index++) {
            result = stream->Write(&Waypoints[index], sizeof(Waypoints[index]), 0);
            if (FAILED(result)) {
                return result;
            }
        }

        result = S_OK;
    }
    return result;
}


// WaypointPathClass Size Of (0x7251E0)

int WaypointPathClass::Size_Of() const
{
    return sizeof(WaypointPathClass);
}


// WaypointPathClass Kind Of (0x7251F0)

int WaypointPathClass::Kind_Of() const
{
    return 0x33;
}


// WaypointPathClass scalar deleting destructor (0x00725200): compiler generated, with the destructor expanded inline;
// emitted by the vtable the constructors write. No source of its own.


// DynamicVectorClass<WaypointClass>::Clear (0x7252D0): compiler generated; emitted by the vtable the constructors of this unit store; no source of its own.


// DynamicVectorClass<WaypointClass>::ID(pointer) (0x725310): compiler generated; emitted by the vtable the constructors of this unit store; no source of its own.


// VectorClass<WaypointClass>::Fetch (0x725340): compiler generated; emitted by the vtables and member uses of this unit (see
// WaypointPathClass_constructor_00724b10.cpp and the WaypointClass definitions); no source of its own.


// emitted container member (manifest DynamicVectorClass_WaypointPathClass_ptr_Clear_00725370.cpp) (0x725370): compiler generated; emitted by the vtables and member uses of this unit (see
// WaypointPathClass_constructor_00724b10.cpp and the WaypointClass definitions); no source of its own.


// emitted container member (manifest DynamicVectorClass_WaypointPathClass_ptr_ID_pointer_007253a0.cpp) (0x7253A0): compiler generated; emitted by the vtables and member uses of this unit (see
// WaypointPathClass_constructor_00724b10.cpp and the WaypointClass definitions); no source of its own.


// emitted container member (manifest VectorClass_WaypointPathClass_ptr_Fetch_007253c0.cpp) (0x7253C0): compiler generated; emitted by the vtables and member uses of this unit (see
// WaypointPathClass_constructor_00724b10.cpp and the WaypointClass definitions); no source of its own.


// DynamicVectorClass<WaypointClass> vector deleting destructor (0x7253D0): compiler generated; emitted by the vtable the constructors of this unit store; no source of its own.


// VectorClass<WaypointClass> scalar deleting destructor (0x725430): compiler generated; emitted by the vtables and member uses of this unit (see
// WaypointPathClass_constructor_00724b10.cpp and the WaypointClass definitions); no source of its own.


// VectorClass<WaypointClass>::operator== (0x725490): compiler generated; emitted by the vtables and member uses of this unit (see
// WaypointPathClass_constructor_00724b10.cpp and the WaypointClass definitions); no source of its own.


// VectorClass<WaypointClass>::Resize (0x7254F0): compiler generated; emitted by the vtables and member uses of this unit (see
// WaypointPathClass_constructor_00724b10.cpp and the WaypointClass definitions); no source of its own.


// VectorClass<WaypointClass>::Clear (0x725600): compiler generated; emitted by the vtables and member uses of this unit (see
// WaypointPathClass_constructor_00724b10.cpp and the WaypointClass definitions); no source of its own.


// VectorClass<WaypointClass>::ID(object) (0x725640): compiler generated; emitted by the vtables and member uses of this unit (see
// WaypointPathClass_constructor_00724b10.cpp and the WaypointClass definitions); no source of its own.


// VectorClass<WaypointClass>::ID(ptr) (0x725690): compiler generated; emitted by the vtables and member uses of this unit (see
// WaypointPathClass_constructor_00724b10.cpp and the WaypointClass definitions); no source of its own.


// DynamicVectorClass<WaypointClass>::Resize (0x7256C0): compiler generated; emitted by the vtable the constructors of this unit store; no source of its own.
// The heap new[] loop calls the out-of-line UnknownCoord434820(int, int, int) (0x00434820); the placement new[] loop
// expands it and calls the CoordStruct(int, int, int) it wraps (0x0041B630); see shared/coord.hpp.


// DynamicVectorClass<WaypointClass>::ID(object) (0x725800): compiler generated; emitted by the vtable the constructors of this unit store; no source of its own.


// emitted container member (manifest VectorClass_WaypointPathClass_ptr_operator_equal_00725840.cpp) (0x725840): compiler generated; emitted by the vtables and member uses of this unit (see
// WaypointPathClass_constructor_00724b10.cpp and the WaypointClass definitions); no source of its own.


// emitted container member (manifest VectorClass_WaypointPathClass_ptr_Resize_00725880.cpp) (0x725880): compiler generated; emitted by the vtables and member uses of this unit (see
// WaypointPathClass_constructor_00724b10.cpp and the WaypointClass definitions); no source of its own.


// emitted container member (manifest VectorClass_WaypointPathClass_ptr_Clear_00725930.cpp) (0x725930): compiler generated; emitted by the vtables and member uses of this unit (see
// WaypointPathClass_constructor_00724b10.cpp and the WaypointClass definitions); no source of its own.


// emitted container member (manifest VectorClass_WaypointPathClass_ptr_ID_object_00725960.cpp) (0x725960): compiler generated; emitted by the vtables and member uses of this unit (see
// WaypointPathClass_constructor_00724b10.cpp and the WaypointClass definitions); no source of its own.


// emitted container member (manifest VectorClass_WaypointPathClass_ptr_ID_pointer_007259a0.cpp) (0x7259A0): compiler generated; emitted by the vtables and member uses of this unit (see
// WaypointPathClass_constructor_00724b10.cpp and the WaypointClass definitions); no source of its own.


// emitted container member (manifest DynamicVectorClass_WaypointPathClass_ptr_Resize_007259c0.cpp) (0x7259C0): compiler generated; emitted by the vtables and member uses of this unit (see
// WaypointPathClass_constructor_00724b10.cpp and the WaypointClass definitions); no source of its own.


// emitted container member (manifest DynamicVectorClass_WaypointPathClass_ptr_ID_object_00725a70.cpp) (0x725A70): compiler generated; emitted by the vtables and member uses of this unit (see
// WaypointPathClass_constructor_00724b10.cpp and the WaypointClass definitions); no source of its own.


// VectorClass<WaypointPathClass *> scalar deleting destructor (0x725AA0): compiler generated; emitted by the vtables and member uses of this unit (see
// WaypointPathClass_constructor_00724b10.cpp and the WaypointClass definitions); no source of its own.


// DynamicVectorClass<WaypointPathClass *> scalar deleting destructor (0x725AF0): compiler generated; emitted by the vtables and member uses of this unit (see
// WaypointPathClass_constructor_00724b10.cpp and the WaypointClass definitions); no source of its own.


// VectorClass<WaypointClass>::VectorClass(unsigned size, WaypointClass const *array) (0x725B40): compiler generated; emitted out of line by WaypointPathClass::Load (0x00725050); no source of its own.
