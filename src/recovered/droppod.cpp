// droppod.cpp: the original translation unit re-filed as one source (Route A): the accepted
// bodies of its canonical compile group in address order, under the unit's own header
// state (its switches and includes in their original order). The module map
// manifests/original_target/modules/droppod.json carries the addresses and the rows that
// stay in their own files.


#include "shared/unknown_geometry.hpp"
#include "shared/unknown_static_rect.hpp"
#include "shared/droppod_locomotion.hpp"
#include "shared/foot.hpp"
#include "shared/rules.hpp"
#include "shared/anim.hpp"
#include "shared/anim_type.hpp"
#include "shared/infantry.hpp"
#include "shared/bullet.hpp"
#include "shared/cell.hpp"
#include "shared/map_view.hpp"
#include "shared/house.hpp"
#include "shared/weapon_type.hpp"
#include "shared/sound_play.hpp"
#include "shared/com_ptr.hpp"


// Header-static dynamic initializers of the unit at 0x004A5850:  the
// shared/unknown_geometry.hpp constants and unknown_static_rect.hpp
// (0x004A5850-0x004A5A36); the other
// drafts of this plan are emitted by these includes.


// Dynamic initializer (0x004A5880, _$E) of b from the header(s) included
// by HeaderStatics_diagonal_init_004a5850.cpp; no source of its own.


// Dynamic initializer (0x004A58A0, _$E) of c from the header(s) included
// by HeaderStatics_diagonal_init_004a5850.cpp; no source of its own.


// Dynamic initializer (0x004A58C0, _$E) of angle45 from the header(s) included
// by HeaderStatics_diagonal_init_004a5850.cpp; no source of its own.


// Dynamic initializer (0x004A58E0, _$E) of angle60 from the header(s) included
// by HeaderStatics_diagonal_init_004a5850.cpp; no source of its own.


// Dynamic initializer (0x004A5900, _$E) of angle90 from the header(s) included
// by HeaderStatics_diagonal_init_004a5850.cpp; no source of its own.


// Dynamic initializer (0x004A5920, _$E) of g from the header(s) included
// by HeaderStatics_diagonal_init_004a5850.cpp; no source of its own.


// Dynamic initializer (0x004A5960, _$E) of h from the header(s) included
// by HeaderStatics_diagonal_init_004a5850.cpp; no source of its own.


// Dynamic initializer (0x004A5990, _$E) of i from the header(s) included
// by HeaderStatics_diagonal_init_004a5850.cpp; no source of its own.


// Dynamic initializer (0x004A59C0, _$E) of j from the header(s) included
// by HeaderStatics_diagonal_init_004a5850.cpp; no source of its own.


// Dynamic initializer (0x004A59F0, _$E) of k from the header(s) included
// by HeaderStatics_diagonal_init_004a5850.cpp; no source of its own.


// Dynamic initializer (0x004A5A00, _$E) of l from the header(s) included
// by HeaderStatics_diagonal_init_004a5850.cpp; no source of its own.


// Dynamic initializer (0x004A5A20, _$E) of static_rect from the header(s) included
// by HeaderStatics_diagonal_init_004a5850.cpp; no source of its own.


// DropPodLocomotionClass::DropPodLocomotionClass() (0x004A5A40)

DropPodLocomotionClass::DropPodLocomotionClass() :
    unknown_1c(false),
    unknown_20(unknown_geometry_l)
{
}


// DropPodLocomotionClass::~DropPodLocomotionClass() (0x004A5A90)
// Releases the carried locomotor.

DropPodLocomotionClass::~DropPodLocomotionClass()
{
}


// bool __stdcall DropPodLocomotionClass::Is_Moving() (0x004A5AC0, ILocomotion 4)
// Always.

bool __stdcall DropPodLocomotionClass::Is_Moving()
{
    return true;
}


// CoordStruct __stdcall DropPodLocomotionClass::Destination() (0x004A5AD0, ILocomotion 5)

CoordStruct __stdcall DropPodLocomotionClass::Destination()
{
    return CoordStruct(unknown_20.X, unknown_20.Y, unknown_20.Z);
}


// bool __stdcall DropPodLocomotionClass::Process() (0x004A5B00, ILocomotion 16)
// TibSun DropPod.cpp Process: the pod falls at DropPodAngle (X toward the chosen side)
// at least DropPodSpeed; on the ground the object is unlimboed (or destroyed with a C4
// explosion when that fails) and the piggybacked locomotor restored; in the air every
// sixth frame leaves SMOKEY and every third the DropPodWeapon strafes near the target.
// The body runs to 0x004A5FBB (Ghidra splits it at 0x004A5F5A/0x004A5F70).

bool __stdcall DropPodLocomotionClass::Process()
{
    CoordStruct coord = LinkedTo->Coord;
    CoordStruct smoke = coord;
    int speed = LinkedTo->Get_Height_AGL() / 10 + 2;
    if (Rules->DropPodSpeed > speed) {
        speed = Rules->DropPodSpeed;
    }
    if (unknown_1c) {
        coord.X += unknown_cos_4ba4d0(Rules->DropPodAngle) * speed;
    } else {
        coord.X -= unknown_cos_4ba4d0(Rules->DropPodAngle) * speed;
    }
    coord.Z -= unknown_sin_4ba480(Rules->DropPodAngle) * speed;
    int height = LinkedTo->Get_Height_AGL();
    FootClass *linked = LinkedTo;
    if (height <= 0) {
        linked->Set_Height_AGL(0);
        coord = linked->Coord;
        linked->Limbo();
        AddRef();
        UnknownComPtr<ILocomotion, &unknown_uuid_ILocomotion_7ce600> &locomotor = LinkedTo->Locomotor;
        locomotor.Attach(0);
        End_Piggyback(&locomotor.Pointer);
        if (!linked->Unlimbo(&coord, 0)) {
            Unknown_0047E2C0(coord, 100, LinkedTo, Rules->unknown_c74, true);
            new AnimClass(Select_Damage_Anim(100, Rules->unknown_c74, 0, &coord), coord, 0, 1, 0x2600, Get_Explosion_Z_0047fcc0(coord));
        } else {
            new AnimClass(Rules->DropPodPuff, coord, 0, 1, 0x600, 0);
            if (unknown_1c) {
                new AnimClass(Rules->DropPod[1], coord, 0, 1, 0x600, 0);
            } else {
                new AnimClass(Rules->DropPod[0], coord, 0, 1, 0x600, 0);
            }
            linked->Mark(1);
            linked->Set_Height_AGL(0);
            linked->unknown_slot261(false, true);
            linked->Commence();
            linked->Scatter(unknown_geometry_l, false, false);
        }
        Release();
    } else {
        linked->Set_Coord(coord);
        WeaponTypeClass *weapon = Rules->DropPodWeapon;
        if (weapon != 0) {
            if (Frame % 6 == 0) {
                new AnimClass(AnimTypes[AnimTypeClass::From_Name("SMOKEY")], smoke, 0, 1, 0x600, 0);
            }
            if (Frame % 3 == 0) {
                if (!LinkedTo->House->Is_Ally((TechnoClass *)Map[unknown_20].Cell_Techno(Point2D(0, 0), false, 0))) {
                    CoordStruct scattered = Coord_Scatter(unknown_20, 0x55, false);
                    Voc_Play_At(weapon->unknown_bc.Pick(unknown_random_839218()), coord, 0);
                    Unknown_0047E2C0(scattered, weapon->unknown_a4 * 2, LinkedTo, weapon->Warhead, true);
                    new AnimClass(Select_Damage_Anim(weapon->unknown_a4 * 2, weapon->Warhead, 0, &scattered), scattered, 0, 1, 0x2600, Get_Explosion_Z_0047fcc0(scattered));
                }
            }
        }
    }
    return true;
}


// void __stdcall DropPodLocomotionClass::Move_To(CoordStruct to) (0x004A5FC0, ILocomotion 17)
// A pod without a destination lands at this one (on the ground): it starts
// DropPodHeight above it, DropPodHeight / tan(DropPodAngle) to the east, or
// as far west when that start is off the map (+0x1C). Placed there facing
// south, it gets an atmosphere entry animation.

void __stdcall DropPodLocomotionClass::Move_To(CoordStruct to)
{
    if (unknown_20 == unknown_geometry_l) {
        unknown_20 = CoordStruct(to.X, to.Y, to.Z);
        unknown_20.Z = Map.Get_Height_GL(unknown_20);
        CoordStruct start = unknown_20;
        start.Z += Rules->DropPodHeight;
        start.X = (int)(start.X + Rules->DropPodHeight / unknown_math_4ba520(Rules->DropPodAngle));
        if (!Map.Unknown_0055A7F0(start)) {
            unknown_1c = true;
            start.X = (int)(unknown_20.X - Rules->DropPodHeight / unknown_math_4ba520(Rules->DropPodAngle));
        }
        LinkedTo->Set_Coord(start);
        if (LinkedTo->Unlimbo(&start, 0x80)) {
            LinkedTo->Set_Coord(start);
            LinkedTo->unknown_2e0.Set(UnknownFacingDir(0x8000));
            new AnimClass(Rules->AtmosphereEntry, start, 0, 1, 0x600, 0);
        } else {
            LinkedTo->Unlimbo(&start, 0x80);
        }
    }
}


// HRESULT __stdcall DropPodLocomotionClass::GetClassID(GUID *clsid) (0x004A6180)

HRESULT __stdcall DropPodLocomotionClass::GetClassID(GUID *clsid)
{
    if (clsid == 0) {
        return E_POINTER;
    }
    *clsid = CLSID_DropPodLocomotionClass;
    return S_OK;
}


// HRESULT __stdcall DropPodLocomotionClass::Load(IStream *stream) (0x004A61C0)
// LocomotionClass::Load, rebuilt in place; then a flag and, when set, the
// carried locomotor (OleLoadFromStream).

HRESULT __stdcall DropPodLocomotionClass::Load(IStream *stream)
{
    HRESULT result = LocomotionClass::Load(stream);
    if (SUCCEEDED(result)) {
        new (this) DropPodLocomotionClass(NoInitClass());
        bool piggybacking;
        stream->Read(&piggybacking, sizeof(piggybacking), 0);
        if (piggybacking) {
            result = OleLoadFromStream(stream, IID_ILocomotion, (void **)&Piggybacker);
        }
    }
    return result;
}


// HRESULT __stdcall DropPodLocomotionClass::Save(IStream *stream, BOOL clear_dirty) (0x004A6240)
// LocomotionClass::Save, then whether a locomotor is carried and, if so,
// that locomotor (OleSaveToStream through its IPersistStream); otherwise
// the result of writing the flag.

HRESULT __stdcall DropPodLocomotionClass::Save(IStream *stream, BOOL clear_dirty)
{
    HRESULT result = LocomotionClass::Save(stream, clear_dirty);
    if (SUCCEEDED(result)) {
        bool piggybacking;
        if (Piggybacker != 0) {
            piggybacking = true;
            stream->Write(&piggybacking, sizeof(piggybacking), 0);
            UnknownComPtr<IPersistStream, &unknown_uuid_IPersistStream_7d5f38> persist(static_cast<ILocomotion *>(Piggybacker));
            result = OleSaveToStream(persist, stream);
        } else {
            piggybacking = false;
            result = stream->Write(&piggybacking, sizeof(piggybacking), 0);
        }
    }
    return result;
}


// void __stdcall DropPodLocomotionClass::Stop_Moving() (0x004A6320, ILocomotion 18)
// A drop pod cannot stop.

void __stdcall DropPodLocomotionClass::Stop_Moving()
{
}


// HRESULT __stdcall DropPodLocomotionClass::Begin_Piggyback(ILocomotion *pointer) (0x004A6330, IPiggyback 3)
// Carries the locomotor unless one is already carried.

HRESULT __stdcall DropPodLocomotionClass::Begin_Piggyback(ILocomotion *pointer)
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


// HRESULT __stdcall DropPodLocomotionClass::End_Piggyback(ILocomotion **pointer) (0x004A6380, IPiggyback 4)
// Hands the carried locomotor (and its reference) back.

HRESULT __stdcall DropPodLocomotionClass::End_Piggyback(ILocomotion **pointer)
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


// bool __stdcall DropPodLocomotionClass::Is_Ok_To_End() (0x004A63C0, IPiggyback 5)
// Once landed while carrying a locomotor.

bool __stdcall DropPodLocomotionClass::Is_Ok_To_End()
{
    if (!Is_Moving() && Piggybacker != 0) {
        return true;
    }
    return false;
}


// HRESULT __stdcall DropPodLocomotionClass::QueryInterface(GUID const &iid, void **object) (0x004A63F0)
// The base interfaces, then IPiggyback.

HRESULT __stdcall DropPodLocomotionClass::QueryInterface(GUID const &iid, void **object)
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


// int __stdcall DropPodLocomotionClass::Layer() (0x004A6450, ILocomotion 29)

int __stdcall DropPodLocomotionClass::Layer()
{
    return 3;
}


// HRESULT __stdcall DropPodLocomotionClass::Piggyback_CLSID(GUID *classid) (0x004A6460, IPiggyback 6)
// The class ID of the carried locomotor, or this locomotor's own when none is
// carried; each is queried for IPersist (TibSun Drive.cpp Piggyback_CLSID).

HRESULT __stdcall DropPodLocomotionClass::Piggyback_CLSID(GUID *classid)
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


// int __stdcall DropPodLocomotionClass::Drawing_Code() (0x004A6570, ILocomotion 34)

int __stdcall DropPodLocomotionClass::Drawing_Code()
{
    return unknown_1c != 0;
}


// LocomotionClass::Is_Moving_Now (0x004A6590): inline in shared/locomotion.hpp; this unit's copy is the one the linker kept. No source of its own.


// LocomotionClass::Mark_All_Occupation_Bits (0x004A65A0): inline in shared/locomotion.hpp; this unit's copy is the one the linker kept. No source of its own.


// LocomotionClass::Is_Moving_Here (0x004A65B0): inline in shared/locomotion.hpp; this unit's copy is the one the linker kept. No source of its own.


// LocomotionClass::Will_Jump_Tracks (0x004A65C0): inline in shared/locomotion.hpp; this unit's copy is the one the linker kept. No source of its own.


// LocomotionClass::Lock (0x004A65D0): inline in shared/locomotion.hpp; this unit's copy is the one the linker kept. No source of its own.


// LocomotionClass::Unlock (0x004A65E0): inline in shared/locomotion.hpp; this unit's copy is the one the linker kept. No source of its own.


// LocomotionClass::Get_Track_Number (0x004A65F0): inline in shared/locomotion.hpp; this unit's copy is the one the linker kept. No source of its own.


// LocomotionClass::Get_Track_Index (0x004A6600): inline in shared/locomotion.hpp; this unit's copy is the one the linker kept. No source of its own.


// LocomotionClass::Get_Speed_Accum (0x004A6610): inline in shared/locomotion.hpp; this unit's copy is the one the linker kept. No source of its own.


// unsigned long __stdcall DropPodLocomotionClass::AddRef() (0x004A6620)

unsigned long __stdcall DropPodLocomotionClass::AddRef()
{
    return LocomotionClass::AddRef();
}


// unsigned long __stdcall DropPodLocomotionClass::Release() (0x004A6630)

unsigned long __stdcall DropPodLocomotionClass::Release()
{
    return LocomotionClass::Release();
}


// bool __stdcall DropPodLocomotionClass::Is_Piggybacking() (0x004A6640, IPiggyback 7)

bool __stdcall DropPodLocomotionClass::Is_Piggybacking()
{
    return Piggybacker != 0;
}


// int DropPodLocomotionClass::Size() (0x004A6660): inline in shared/droppod_locomotion.hpp, emitted with the vtable; no source of its own.


// void *DropPodLocomotionClass::`scalar deleting destructor'(unsigned int flags) (0x004A6670)
// Compiler-generated with the vtable (the destructor inlined). No source of its own.


// DropPodLocomotionClass::QueryInterface adjustor thunk for ILocomotion (0x004A66C0)
// Compiler generated (this adjusted, then QueryInterface). No source of its own.


// DropPodLocomotionClass::AddRef adjustor thunk for ILocomotion (0x004A66D0)
// Compiler generated (this adjusted, then AddRef). No source of its own.


// DropPodLocomotionClass::Release adjustor thunk for ILocomotion (0x004A66E0)
// Compiler generated (this adjusted, then Release). No source of its own.


// DropPodLocomotionClass::QueryInterface adjustor thunk for IPiggyback (0x004A66F0)
// Compiler generated (this adjusted, then QueryInterface). No source of its own.


// DropPodLocomotionClass::AddRef adjustor thunk for IPiggyback (0x004A6700)
// Compiler generated (this adjusted, then AddRef). No source of its own.


// DropPodLocomotionClass::Release adjustor thunk for IPiggyback (0x004A6710)
// Compiler generated (this adjusted, then Release). No source of its own.
