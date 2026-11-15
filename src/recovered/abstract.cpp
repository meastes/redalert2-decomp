/*******************************************************************************
 *                                O P E N  T S
 *******************************************************************************
 * SPDX-License-Identifier: GPL-3.0-or-later
 * Copyright 2025 Electronic Arts Inc.
 * Copyright 2026 OpenTS contributors
 *
 * Contains material derived from Electronic Arts source code.
 * Modified by OpenTS contributors, 2026.
 * EA's GPLv3 Section 7 additional terms and supplemental warranty
 * disclaimers apply; see LICENSE.md.
 ******************************************************************************/
// Function comment blocks from mischa85/RedAlert2 392e904 src/abstract.cpp (matching RA2 1.006 decomp), placed above our
// own notes; code unchanged.

// abstract.cpp: the original translation unit re-filed as one source (Route A): the accepted
// bodies of its canonical compile group in address order, under the unit's own header
// state (its switches and includes in their original order). The module map
// manifests/original_target/modules/abstract.json carries the addresses and the rows that
// stay in their own files.

// Header-static dynamic initializers of the unit at 0x0040FD00: the shape_file.hpp empty rect and the
// shared/unknown_geometry.hpp constants (0x0040FD00-0x0040FEE1); the other
// drafts of this plan are emitted by these includes.
#include "shared/shape_file.hpp"
#include "shared/unknown_geometry.hpp"


// Dynamic initializer (0x0040FD20, _$E) of diagonal from the header(s) included
// by HeaderStatics_empty_rect_init_0040fd00.cpp; no source of its own.


// Dynamic initializer (0x0040FD50, _$E) of b from the header(s) included
// by HeaderStatics_empty_rect_init_0040fd00.cpp; no source of its own.


// Dynamic initializer (0x0040FD70, _$E) of c from the header(s) included
// by HeaderStatics_empty_rect_init_0040fd00.cpp; no source of its own.


// Dynamic initializer (0x0040FD90, _$E) of angle45 from the header(s) included
// by HeaderStatics_empty_rect_init_0040fd00.cpp; no source of its own.


// Dynamic initializer (0x0040FDB0, _$E) of angle60 from the header(s) included
// by HeaderStatics_empty_rect_init_0040fd00.cpp; no source of its own.


// Dynamic initializer (0x0040FDD0, _$E) of angle90 from the header(s) included
// by HeaderStatics_empty_rect_init_0040fd00.cpp; no source of its own.


// Dynamic initializer (0x0040FDF0, _$E) of g from the header(s) included
// by HeaderStatics_empty_rect_init_0040fd00.cpp; no source of its own.


// Dynamic initializer (0x0040FE30, _$E) of h from the header(s) included
// by HeaderStatics_empty_rect_init_0040fd00.cpp; no source of its own.


// Dynamic initializer (0x0040FE60, _$E) of i from the header(s) included
// by HeaderStatics_empty_rect_init_0040fd00.cpp; no source of its own.


// Dynamic initializer (0x0040FE90, _$E) of j from the header(s) included
// by HeaderStatics_empty_rect_init_0040fd00.cpp; no source of its own.


// Dynamic initializer (0x0040FEC0, _$E) of k from the header(s) included
// by HeaderStatics_empty_rect_init_0040fd00.cpp; no source of its own.


// Dynamic initializer (0x0040FED0, _$E) of l from the header(s) included
// by HeaderStatics_empty_rect_init_0040fd00.cpp; no source of its own.




#include "shared/abstract.hpp"
#include "shared/vector_class.hpp"


/// <summary>
/// Default constructor for abstract objects.
/// This routine puts the object into its neutral state: no unique ID assigned
/// yet, and nothing pending for the save system.
/// </summary>
// (0x0040FF10)
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


/// <summary>
/// No-initialization constructor for abstract objects.
/// Use this constructor when the object is about to be overwritten wholesale by
/// the load process. It deliberately leaves every member untouched.
/// </summary>
// (0x0040FF60)
// Installs the vtables only (the save-game load path).

AbstractClass::AbstractClass(NoInitClass const &noinit)
{
}


/// <summary>
/// Destructor for abstract objects.
/// </summary>
// (0x0040FF90)
// Restores the AbstractClass vtables.

AbstractClass::~AbstractClass()
{
}


/// <summary>
/// Fetches the RTTI type of this object.
/// The engine carries its own run-time type scheme in place of the C++ feature.
/// Only instantiable classes report a type of their own; the intermediate layers
/// of the hierarchy report RTTI_NONE.
/// </summary>
/// <returns>Returns with the RTTI type constant for this object.</returns>
// (0x0040FFB0)
// IRTTITypeInfo: the object kind (Kind_Of); `this` is the +4 interface.

int __stdcall AbstractClass::What_Am_I()
{
    return Kind_Of();
}


/// <summary>
/// Fetches this object's unique identifier.
/// </summary>
/// <returns>Returns with the unique ID, or -1 if none has been assigned yet.</returns>
// (0x0040FFC0)
// IRTTITypeInfo: the unique ID.

long __stdcall AbstractClass::Fetch_ID()
{
    return UniqueID;
}


/// <summary>
/// Assigns this object a unique identifier.
/// The ID is drawn from the scenario's running counter, so objects created
/// before a scenario exists are all given an ID of zero.
/// </summary>
// (0x0040FFD0)
// IRTTITypeInfo: takes a new unique ID from the ID source (0 without one).

void __stdcall AbstractClass::Create_ID()
{
    UniqueID = unknown_id_source_a3d290 == 0 ? 0 : unknown_id_source_a3d290->Next_ID();
}


/// <summary>
/// Fetches a COM interface pointer from this object.
/// This is the IUnknown implementation shared by every game object. Abstract
/// objects expose IUnknown, IPersistStream, IPersist and IRTTITypeInfo; the save
/// game system reaches the whole object hierarchy through them.
/// </summary>
/// <param name="riid">The identifier of the interface being asked for.</param>
/// <param name="ppvObject">Receives the interface pointer, or NULL when the
/// interface is not supported.</param>
/// <returns>
/// Returns with S_OK when the interface was supplied. Otherwise E_NOINTERFACE is
/// returned for an unsupported interface, or E_POINTER when no output pointer was given.
/// </returns>
// (0x00410000)
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


/// <summary>
/// Satisfies the IUnknown reference count contract.
/// The game owns its objects outright and they outlive any interface pointer
/// handed out, so nothing is actually counted.
/// </summary>
/// <returns>Returns with the reference count, which is always one.</returns>
// (0x004100A0)
// IUnknown: no reference counting (always 1).

unsigned long __stdcall AbstractClass::AddRef()
{
    return 1;
}


/// <summary>
/// Satisfies the IUnknown release contract.
/// Releasing an interface never destroys a game object -- see AddRef.
/// </summary>
/// <returns>Returns with the reference count, which is always one.</returns>
// (0x004100B0)
// IUnknown: no reference counting (always 1).

unsigned long __stdcall AbstractClass::Release()
{
    return 1;
}


/// <summary>
/// Writes this object to the save stream.
/// The object's address goes out first as its swizzle identity, followed by the
/// raw object image. Load reverses the process.
/// </summary>
/// <param name="stream">The stream to write to.</param>
/// <param name="cleardirty">Should the object be marked clean once it has been written?</param>
/// <returns>
/// Returns with S_OK when the object was written, E_POINTER when no stream was
/// supplied, or the stream's own failure code.
/// </returns>
// (0x004100C0)
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


/// <summary>
/// Reads this object back from the save stream.
/// The saved address is handed to the swizzle system so that pointers elsewhere
/// in the save game can be remapped onto this object's new location, and then the
/// raw object image is read back over this one.
/// </summary>
/// <param name="stream">The stream to read from.</param>
/// <returns>
/// Returns with S_OK when the object was read, E_POINTER when no stream was
/// supplied, or the stream's own failure code.
/// </returns>
// (0x00410120)
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


/// <summary>
/// Fetches the number of bytes that Save will write.
/// </summary>
/// <param name="pcbSize">Receives the maximum size, in bytes.</param>
/// <returns>Returns with S_OK, or E_POINTER when no output pointer was given.</returns>
// (0x00410180)
// IPersistStream: Size_Of() plus the saved pointer.

HRESULT __stdcall AbstractClass::GetSizeMax(ULARGE_INTEGER *size)
{
    if (size == 0) {
        return E_POINTER;
    }
    size->QuadPart = Size_Of() + sizeof(AbstractClass *);
    return S_OK;
}


/// <summary>
/// Folds this object's state into a running CRC.
/// The multiplayer sync check walks every object each frame and accumulates its
/// state; a CRC that differs between machines means the simulations have drifted
/// apart. Derived classes add their own state on top of this.
/// </summary>
/// <param name="crc">The running CRC to fold this object's state into.</param>
// (0x004101B0)
// Adds the unique ID and the dirty flag to the CRC.

void AbstractClass::Compute_CRC(WWCRCEngine &crc) const
{
    crc(UniqueID);
    crc(Dirty);
}


/// <summary>
/// Determines if this object is inactive.
/// The bare abstract object is never a participant in the game, so it always
/// reports inactive. Objects that can be placed in the world override this.
/// </summary>
/// <returns>bool; Is the object inactive?</returns>
// (0x004101E0)
// Virtual slot 17: true.

bool AbstractClass::Is_Inactive() const
{
    return true;
}


/// <summary>
/// Determines if this object has changed since it was last saved.
/// </summary>
/// <returns>Returns with S_OK when the object is dirty, or S_FALSE when it is not.</returns>
// (0x004101F0)
// IPersistStream: S_OK when changed since the last clearing save, else S_FALSE.

HRESULT __stdcall AbstractClass::IsDirty()
{
    return !Dirty;
}


/// <summary>
/// Resets this object to its start of scenario state.
/// The bare abstract object carries no scenario state, so there is nothing to do.
/// </summary>
// (0x00410210)
// Virtual slot 9: nothing.

void AbstractClass::Init()
{
}


/// <summary>
/// Severs any references this object holds to the target.
/// This routine is called on every object when one of them is about to be
/// destroyed, so that nothing is left pointing at the corpse.
/// </summary>
/// <param name="target">The object that is about to disappear.</param>
/// <param name="all">Should every reference be severed, including cargo and radio links?</param>
// (0x00410220)
// Virtual slot 10: nothing to detach.

void AbstractClass::Detach(AbstractClass *target, bool all)
{
}


/// <summary>
/// Fetches the house that owns this object.
/// The bare abstract object belongs to nobody. Ownable objects override this.
/// </summary>
/// <returns>Returns with the owning house, or HOUSE_NONE when the object is unowned.</returns>
// (0x00410230)
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


/// <summary>
/// Fetches this object's index within its heap.
/// </summary>
/// <returns>Returns with the heap index, or zero for an object that does not
/// live in a heap.</returns>
// (0x00410250)
// Virtual slot 16: 0.

int AbstractClass::Fetch_Heap_ID() const
{
    return 0;
}


/***********************************************************************************************
 * ObjectClass::Center_Coord -- Fetches the center coordinate for the object.                  *
 *                                                                                             *
 *    This routine will return the center coordinate for the object. The center coordinate is  *
 *    typically the coordinate recorded in the object structure. Exceptions to this include    *
 *    the trees and other terrain elements.                                                    *
 *                                                                                             *
 * INPUT:   none                                                                               *
 *                                                                                             *
 * OUTPUT:  Returns the coordinate that is considered the center point of this object.         *
 *                                                                                             *
 * WARNINGS:   none                                                                            *
 *                                                                                             *
 * HISTORY:                                                                                    *
 *   09/21/1995 JLB : Created.                                                                 *
 *=============================================================================================*/
// (0x00410260)
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


/// <summary>
/// Determines if this object is resting on the ground.
/// </summary>
/// <returns>bool; Is the object on the ground?</returns>
// (0x004102C0)
// Virtual slot 20: false.

bool AbstractClass::On_Ground() const
{
    return false;
}


/// <summary>
/// Determines if this object is airborne.
/// </summary>
/// <returns>bool; Is the object in the air?</returns>
// (0x004102D0)
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


/// <summary>
/// Handles the per frame logic for this object.
/// This routine is called once per game frame for every active object. The bare
/// abstract object has no logic of its own.
/// </summary>
// (0x00410310)
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
