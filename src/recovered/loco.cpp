/*******************************************************************************
 *                                O P E N  T S
 *******************************************************************************
 * SPDX-License-Identifier: GPL-3.0-or-later
 * Copyright 2026 OpenTS contributors
 *
 * See LICENSE.md for applicable additional terms and warranty disclaimers.
 ******************************************************************************/
// Function comment blocks from mischa85/RedAlert2 392e904 src/loco.cpp (matching RA2 1.006 decomp), placed above our
// own notes; code unchanged.
// Parameter names from mischa85/RedAlert2 392e904 src/loco.cpp where both define the function (names only).
// Local variable names from mischa85/RedAlert2 392e904 src/loco.cpp where both functions declare the same local types (names only).

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


/// <summary>
/// Constructs a locomotor that is not yet attached to anything.
/// The locomotor comes up powered and with no object to drive.
/// </summary>
/// <remarks>Link_To_Object must be called before the locomotor can be asked to do anything
/// useful.</remarks>
// (0x0053CFC0)
// Unlinked, powered and dirty, no references.

LocomotionClass::LocomotionClass() :
    Owner(0),
    LinkedTo(0),
    Powered(true),
    Dirty(true),
    RefCount(0)
{
}


/// <summary>
/// Destroys the locomotor and severs the link to the object it drove.
/// </summary>
// (0x0053CFF0)
// Forgets the linked object.

LocomotionClass::~LocomotionClass()
{
    LinkedTo = 0;
}


/// <summary>
/// Attaches this locomotor to the object it is to drive.
/// This is called as the object is being created, and every other service the locomotor
/// offers depends on it having been called first.
/// </summary>
/// <param name="pointer">Pointer to the foot class object this locomotor will carry about.</param>
/// <returns>Returns with S_OK, since the attachment cannot fail.</returns>
// (0x0053D010, ILocomotion 3)
// The object becomes both the owner and the linked object.

HRESULT __stdcall LocomotionClass::Link_To_Object(void *pointer)
{
    LinkedTo = (FootClass *)pointer;
    Owner = (FootClass *)pointer;
    return S_OK;
}


/// <summary>
/// Builds the transform matrix to draw the object with.
/// The base locomotor merely turns the object to its body facing, snapped to one of the
/// facings the voxel art is drawn at. Locomotors that tilt, bank, or bob their object
/// will override this routine.
/// </summary>
/// <param name="key">Optional cache key for the voxel renderer, which the facing is folded
/// into. May be NULL, and a key of -1 means the drawing is not to be cached.</param>
/// <returns>Returns with the matrix to transform the object by.</returns>
// (0x0053D030, ILocomotion 9)
// The body transform: the facing as one of 32 directions turned about Z;
// a key not -1 gains the facing as five more bits.

Matrix3D * __stdcall LocomotionClass::Draw_Matrix(Matrix3D *matrix, int *key)
{
    Matrix3D draw_matrix;
    draw_matrix.Make_Identity();
    draw_matrix.Rotate_Z((float)((Dir_To_32(LinkedTo->PrimaryFacing.Current()) - 8) * -0.19634954084936207));
    if (key != 0 && *key != -1) {
        *key <<= 5;
        *key |= Dir_To_32(LinkedTo->PrimaryFacing.Current());
    }
    *matrix = draw_matrix;
    return matrix;
}


/// <summary>
/// Builds the transform matrix to draw the object's shadow with.
/// A shadow lies flat on the ground, so the object's facing is applied on top of the
/// slope of the cell it is currently over.
/// </summary>
/// <param name="key">Optional cache key for the voxel renderer, which the slope and facing are
/// folded into. May be NULL, and a key of -1 means the shadow is not to be cached.</param>
/// <returns>Returns with the matrix to transform the shadow by.</returns>
// (0x0053D0D0, ILocomotion 10)
// The shadow transform: the slope matrix of the cell's ramp turned about Z
// by the facing; a key not -1 gains the ramp (six bits) and the facing
// (five bits).

Matrix3D * __stdcall LocomotionClass::Shadow_Matrix(Matrix3D *matrix, int *key)
{
    CoordStruct coord = LinkedTo->Coord;
    int ramp = Map[coord].unknown_11c;
    Matrix3D transform;
    Matrix3D ramp_matrix;
    transform = *Get_Slope_Matrix(&ramp_matrix, ramp);
    transform.Rotate_Z((float)((Dir_To_32(LinkedTo->PrimaryFacing.Current()) - 8) * -0.19634954084936207));
    if (key != 0 && *key != -1) {
        *key = ((*key << 6) + ramp) << 5;
        *key |= Dir_To_32(LinkedTo->PrimaryFacing.Current());
    }
    *matrix = transform;
    return matrix;
}


/// <summary>
/// Fetches the pixel offset to draw the object's shadow at.
/// The shadow belongs on the ground rather than with the object, so the offset carries it
/// down by however far the object is flying above the terrain.
/// </summary>
/// <returns>Returns with the pixel offset to shift the shadow by when drawing.</returns>
// (0x0053D1C0, ILocomotion 12)
// The shadow is drawn the object's height (in pixels) lower.

Point2D __stdcall LocomotionClass::Shadow_Point()
{
    return Point2D(0, Tactical_Z_Lepton_To_Pixel(LinkedTo->Get_Height_AGL()));
}


/// <summary>
/// Restores power to the locomotor.
/// This undoes an earlier Power_Off and lets the object travel under its own means once
/// more.
/// </summary>
/// <returns>bool; Is the locomotor powered after the change?</returns>
// (0x0053D1F0, ILocomotion 22)

bool __stdcall LocomotionClass::Power_On()
{
    Powered = true;
    return Is_Powered();
}


/// <summary>
/// Removes power from the locomotor.
/// This routine is used to strand the object where it stands, such as when it is caught
/// by an EMP pulse or its owner loses base power.
/// </summary>
/// <returns>bool; Is the locomotor powered after the change?</returns>
// (0x0053D210, ILocomotion 23)

bool __stdcall LocomotionClass::Power_Off()
{
    Powered = false;
    return Is_Powered();
}


/// <summary>
/// Does the locomotor currently have power?
/// An unpowered locomotor will not carry its object anywhere, which is how an EMP or a
/// loss of base power leaves a unit stranded.
/// </summary>
/// <returns>bool; Is the locomotor powered?</returns>
// (0x0053D230, ILocomotion 24)

bool __stdcall LocomotionClass::Is_Powered()
{
    return Powered;
}


/// <summary>
/// Is this locomotor disrupted by an ion storm?
/// Locomotors that rely on flight or some other fragile means of travel will override
/// this routine so that a storm can bring their objects down.
/// </summary>
/// <returns>bool; Is the locomotor sensitive to ion storms?</returns>
// (0x0053D240, ILocomotion 25)

bool __stdcall LocomotionClass::Is_Ion_Sensitive()
{
    return false;
}


/// <summary>
/// Adds a reference to this locomotor.
/// Anything that holds on to a locomotor takes a reference first, which keeps the
/// locomotor alive until that holder releases it again.
/// </summary>
/// <returns>Returns with the number of references now outstanding.</returns>
// (0x0053D250)
// Counts a live COM object and a reference.

unsigned long __stdcall LocomotionClass::AddRef()
{
    unknown_com_objects_a6eb14++;
    return InterlockedIncrement(&RefCount);
}


/// <summary>
/// Releases a reference to this locomotor.
/// When the last reference goes away the locomotor destroys itself, so the caller must
/// not touch its pointer afterward.
/// </summary>
/// <returns>Returns with the number of references still outstanding.</returns>
// (0x0053D270)
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


/// <summary>
/// Fetches one of the interfaces this locomotor implements.
/// A locomotor answers to IUnknown, IPersist, IPersistStream, and ILocomotion. Any other
/// interface asked for is refused.
/// </summary>
/// <param name="riid">The identifier of the interface being asked for.</param>
/// <param name="ppvObject">Pointer to the location to store the interface pointer in.</param>
/// <returns>Returns with S_OK, or E_NOINTERFACE if the interface is not supported.</returns>
/// <remarks>An interface fetched successfully carries a reference. The caller must release
/// it when finished with it.</remarks>
// (0x0053D2B0)
// IUnknown and ILocomotion give the +4 interface, IPersistStream and
// IPersist the object.

HRESULT __stdcall LocomotionClass::QueryInterface(GUID const &riid, void **ppvObject)
{
    if (ppvObject == 0) {
        return E_POINTER;
    }
    *ppvObject = 0;
    if (riid == IID_IUnknown_7afce0) {
        *ppvObject = static_cast<ILocomotion *>(this);
    }
    if (riid == IID_IPersistStream) {
        *ppvObject = static_cast<IPersistStream *>(this);
    }
    if (riid == IID_ILocomotion) {
        *ppvObject = static_cast<ILocomotion *>(this);
    }
    if (riid == IID_IPersist) {
        *ppvObject = static_cast<IPersistStream *>(this);
    }
    if (*ppvObject == 0) {
        return E_NOINTERFACE;
    }
    AddRef();
    return S_OK;
}


/// <summary>
/// Saves the locomotor out to a save game stream.
/// The locomotor's address is written ahead of its data, which is what lets the swizzle
/// manager remap every pointer to it when the game is loaded again.
/// </summary>
/// <param name="cleardirty">Should the locomotor be marked as no longer needing a save?</param>
/// <returns>Returns with the result of the write, or E_POINTER if no stream was supplied.</returns>
// (0x0053D360, IPersistStream)
// Writes the object's own address (the swizzle key Load registers), then
// the whole object (Size() bytes).

HRESULT __stdcall LocomotionClass::Save(IStream *stream, BOOL cleardirty)
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
    if (cleardirty) {
        Dirty = false;
    }
    return result;
}


/// <summary>
/// Loads the locomotor back from a save game stream.
/// The locomotor announces its new address to the swizzle manager before its data is
/// read in, so that every saved pointer to it can be remapped and its link back to the
/// object it drives can be restored. The reference count belongs to the running session
/// rather than to the saved state, so it survives the load untouched.
/// </summary>
/// <param name="stream">The stream to read the locomotor back from.</param>
/// <returns>Returns with the result of the read, or E_POINTER if no stream was supplied.</returns>
// (0x0053D3C0)
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


/// <summary>
/// Fetches the number of bytes needed to save this locomotor.
/// This is the IPersistStream service the save system uses to size the storage it must
/// set aside before asking the locomotor to write itself out.
/// </summary>
/// <param name="pcbSize">Pointer to the value to fill in with the required byte count.</param>
/// <returns>Returns with S_OK, or E_POINTER if no destination was supplied.</returns>
// (0x0053D440)
// Size() plus the saved address.

HRESULT __stdcall LocomotionClass::GetSizeMax(ULARGE_INTEGER *pcbSize)
{
    if (pcbSize == 0) {
        return E_POINTER;
    }
    pcbSize->QuadPart = (unsigned)(Size() + sizeof(LocomotionClass *));
    return S_OK;
}


/// <summary>
/// Asks the object to step out of the way in the direction specified.
/// This routine is used when another object needs the cell this one happens to be
/// occupying. The base locomotor cannot be moved and declines.
/// </summary>
/// <returns>bool; Did the object step out of the way?</returns>
// (0x0053D470, ILocomotion)
// Refused here.

bool __stdcall LocomotionClass::Push(UnknownFacingDir dir)
{
    return false;
}


/// <summary>
/// Shoves the object aside in the direction specified.
/// This is the more forceful companion to Push, used when a loitering object must be
/// displaced to clear the way. The base locomotor will not budge.
/// </summary>
/// <returns>bool; Was the object shoved out of the way?</returns>
// (0x0053D480, ILocomotion)
// Refused here.

bool __stdcall LocomotionClass::Shove(UnknownFacingDir dir)
{
    return false;
}


/// <summary>
/// Handles the per frame settling of the object's tilt and pitch.
/// Locomotors that rock their object about -- over bumps, on landing, or when it takes a
/// hit -- use this routine to ease the body back toward level.
/// </summary>
// (0x0053D490, ILocomotion)
// Nothing here.

void __stdcall LocomotionClass::Tilt_Pitch_AI()
{

}


/// <summary>
/// Fetches the depth bias to draw the object with.
/// This nudges the object forward or back in the depth buffer so that it sorts correctly
/// against the terrain it is traveling over. The base locomotor needs no such favor.
/// </summary>
/// <returns>Returns with the depth adjustment to apply when drawing the object.</returns>
// (0x0053D4A0, ILocomotion)
// No adjustment.

int __stdcall LocomotionClass::Z_Adjust()
{
    return 0;
}


/// <summary>
/// Fetches the Z gradient to draw the object with.
/// The gradient tells the depth shaded renderer how the object's depth varies across its
/// shape. The base locomotor reports the upright case.
/// </summary>
/// <returns>Returns with the Z gradient to render the object with.</returns>
// (0x0053D4B0, ILocomotion)
// Gradient 2 (hypothesis: ground level).

int __stdcall LocomotionClass::Z_Gradient()
{
    return 2;
}


/// <summary>
/// Fetches the visual character to draw the object with.
/// This is how a locomotor fades or hides its object as it burrows, submerges, or
/// otherwise leaves plain sight. The base locomotor never alters the appearance.
/// </summary>
/// <returns>Returns with the visual character to render the object with.</returns>
// (0x0053D4C0, ILocomotion)
// Normal (0).

int __stdcall LocomotionClass::Visual_Character(bool flag)
{
    return 0;
}


/// <summary>
/// Fetches the pixel offset to draw the object at.
/// The render code adds this to the object's normal screen position, which is how a
/// locomotor makes its object bob, hop, or sink. The base locomotor draws in place.
/// </summary>
/// <returns>Returns with the pixel offset to shift the object by when drawing.</returns>
// (0x0053D4D0, ILocomotion)
// No offset.

Point2D __stdcall LocomotionClass::Draw_Point()
{
    return Point2D(0, 0);
}


/// <summary>
/// Should the object cast a shadow?
/// Locomotors that take their object out of the light -- underground, submerged, or
/// otherwise hidden -- will override this routine to suppress the shadow.
/// </summary>
/// <returns>bool; Should a shadow be drawn for the object?</returns>
// (0x0053D4E0, ILocomotion)
// Always.

bool __stdcall LocomotionClass::Is_To_Have_Shadow()
{
    return true;
}


/// <summary>
/// Determines if the object may move into the cell specified.
/// The movement code asks the locomotor rather than the object, since what counts as
/// passable depends entirely on how the object travels. The base locomotor is
/// unrestricted and welcomes every cell.
/// </summary>
/// <returns>Returns with the move legality of the cell.</returns>
// (0x0053D4F0, ILocomotion)
// Always allowed (0).

int __stdcall LocomotionClass::Can_Enter_Cell(Cell cell)
{
    return 0;
}


/// <summary>
/// Forces the object's immediate destination to the coordinate specified.
/// This overrides whatever short term move the locomotor had in mind, and is used when
/// outside code must dictate exactly where the object ends up next.
/// </summary>
/// <param name="coord">The coordinate the object should head to immediately.</param>
// (0x0053D500, ILocomotion)
// Nothing here.

void __stdcall LocomotionClass::Force_Immediate_Destination(CoordStruct coord)
{

}


/// <summary>
/// Forces the object onto a specific movement track.
/// This routine is used when outside code must dictate the exact motion the object
/// performs, rather than letting the locomotor plan one for itself.
/// </summary>
/// <param name="track">The track number the object should be placed onto.</param>
/// <param name="coord">The coordinate to treat as the start of the track.</param>
// (0x0053D510, ILocomotion)
// Nothing here.

void __stdcall LocomotionClass::Force_Track(int track, CoordStruct coord)
{

}


/// <summary>
/// Notifies the locomotor that its object has been placed onto the map.
/// This gives derived locomotors their chance to pick up a starting facing, slope, or
/// altitude from the ground the object has just arrived on.
/// </summary>
// (0x0053D520, ILocomotion)
// Nothing here.

void __stdcall LocomotionClass::Unlimbo()
{

}


/// <summary>
/// Turns the object to face the direction specified.
/// The base locomotor has no body of its own to rotate, so the request goes unheeded.
/// </summary>
/// <param name="coord">The direction that the object should come to face.</param>
// (0x0053D530, ILocomotion)
// Nothing here.

void __stdcall LocomotionClass::Do_Turn(UnknownFacingDir coord)
{

}


/// <summary>
/// Halts the object's travel.
/// This routine is called when the object must give up on wherever it was going. Derived
/// locomotors use it to abandon their journey and bring the object to a legal rest.
/// </summary>
// (0x0053D540, ILocomotion)
// Nothing here.

void __stdcall LocomotionClass::Stop_Moving()
{

}


/// <summary>
/// Commands the locomotor to carry the object to the coordinate specified.
/// This is how the object hands its locomotor a new place to go. The base locomotor
/// cannot move anything, so the request is quietly ignored.
/// </summary>
// (0x0053D550, ILocomotion)
// Nothing here.

void __stdcall LocomotionClass::Move_To(CoordStruct to)
{

}


/// <summary>
/// Performs one game frame of locomotion for the object.
/// The linked object calls this from its own AI so that the locomotor can carry it
/// further along its travel. The base locomotor has nothing to carry and reports that it
/// is already at rest.
/// </summary>
/// <returns>bool; Is the locomotor at rest, with nothing further to do?</returns>
// (0x0053D560, ILocomotion)
// Nothing to do; reports success.

bool __stdcall LocomotionClass::Process()
{
    return true;
}


/// <summary>
/// Fetches the final destination of the object's travel.
/// The base locomotor never has a journey pending, so it reports that there is no
/// destination at all.
/// </summary>
/// <returns>Returns with the destination coordinate, or COORD_NONE if there is none.</returns>
// (0x0053D570, ILocomotion)
// None: the zero coordinate.

CoordStruct __stdcall LocomotionClass::Destination()
{
    CoordStruct coord = unknown_geometry_l;
    return coord;
}


/// <summary>
/// Fetches the coordinate the object is immediately heading toward.
/// This is the next step of the journey rather than its end. Since the base locomotor
/// has nowhere to go, it reports the object's own position.
/// </summary>
/// <returns>Returns with the coordinate currently being moved toward.</returns>
// (0x0053D5A0, ILocomotion)
// Where the linked object is.

CoordStruct __stdcall LocomotionClass::Head_To_Coord()
{
    CoordStruct coord = LinkedTo->Coord;
    return coord;
}


/// <summary>
/// Is the object currently in motion?
/// The mission and display code uses this to tell a traveling object from a parked one.
/// The base locomotor never carries its object anywhere, so it always answers no.
/// </summary>
/// <returns>bool; Is the object moving?</returns>
// (0x0053D5D0, ILocomotion)
// Never here.

bool __stdcall LocomotionClass::Is_Moving()
{
    return false;
}


/// <summary>
/// Forces the object onto a new ground slope.
/// This routine is called when the terrain beneath the object changes out from under it.
/// Only locomotors that tilt their object with the ground need to act on it.
/// </summary>
/// <param name="ramp">The ramp type of the slope the object should now conform to.</param>
// (0x0053D5E0, ILocomotion)
// Nothing here.

void __stdcall LocomotionClass::Force_New_Slope(int ramp)
{

}


/// <summary>
/// Fetches the drawing code the locomotor wants the object rendered with.
/// The render code uses this to pick a presentation that suits how the object is
/// currently traveling. The base locomotor has no preference.
/// </summary>
/// <returns>Returns with the drawing code, or zero for the ordinary presentation.</returns>
// (0x0053D5F0, ILocomotion)
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


/// <summary>
/// Fetches the speed that the object appears to be traveling at.
/// This is the plain answer -- the linked object's own current speed. Locomotors whose
/// visible motion differs from the object's logical speed will override this routine.
/// </summary>
/// <returns>Returns with the apparent speed of the linked object.</returns>
// (0x0053D610, ILocomotion)
// The linked object's speed (FootClass slot 303).

int __stdcall LocomotionClass::Apparent_Speed()
{
    return LinkedTo->Current_Speed();
}
