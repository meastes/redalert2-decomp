// light.cpp: the original translation unit re-filed as one source (Route A): the accepted
// bodies of its canonical compile group in address order, under the unit's own header
// state (its switches and includes in their original order). The module map
// manifests/original_target/modules/light.json carries the addresses and the rows that
// stay in their own files.


#define RA2_LIGHT_SOURCE_FLAT_TINT
// Header-static dynamic initializers of the unit at 0x00536F30 and its
// list globals (LightSourceClass, LightSourceClass_PendingCellClass); the other drafts of this plan are emitted by this file.
// A list whose atexit thunk tests and sets a guard bit is a static data
// member (VC6 guards their destruction); its owner class is a hypothesis.
#include "shared/unknown_geometry.hpp"
#include "shared/unknown_static_rect.hpp"
#ifndef VECTOR_IMPLEMENT_ALL
#define VECTOR_IMPLEMENT_ALL
#endif
#include "shared/vector_class.hpp"

#include "shared/light_source.hpp"

#include "shared/map.hpp"
#include "shared/map_view.hpp"
#include "shared/cell_class.hpp"
#include "shared/options.hpp"

#include "shared/milsectmr.hpp"

// A cell waiting to be relit (TibSun LightSourceClass::PendingCellClass): the drawer and values picked
// for it, applied together when every pending cell has them (0x14 bytes).
class LightSourceClass::PendingCellClass {
public:
    PendingCellClass(Cell cell) :
        Converter(0),
        Intensity(0x10000),
        Ambient(0),
        Brightness(1000),
        TileBrightness(1000),
        AltBrightness(1000),
        CellID(cell)
    {
    }
    static DynamicVectorClass<PendingCellClass *> Array;   // 0x00A6E848

    LightConvertClass *Converter;   // +0x00
    int Intensity;                  // +0x04
    unsigned short Ambient;                // +0x08
    unsigned short Brightness;             // +0x0A
    unsigned short TileBrightness;         // +0x0C
    unsigned short AltBrightness;          // +0x0E
    Cell CellID;                    // +0x10
};

// The unit's list of LightSourceClass (0x00A6E818; name is a hypothesis).
DynamicVectorClass<LightSourceClass *> unknown_LightSourceClass_list_00a6e818;
DynamicVectorClass<LightSourceClass::PendingCellClass *> LightSourceClass::PendingCellClass::Array;


// Dynamic initializer (0x00536F60, _$E5) emitted by HeaderStatics_diagonal_init_00536f30.cpp; no source of its own.


// Dynamic initializer (0x00536F80, _$E8) emitted by HeaderStatics_diagonal_init_00536f30.cpp; no source of its own.


// Dynamic initializer (0x00536FA0, _$E11) emitted by HeaderStatics_diagonal_init_00536f30.cpp; no source of its own.


// Dynamic initializer (0x00536FC0, _$E14) emitted by HeaderStatics_diagonal_init_00536f30.cpp; no source of its own.


// Dynamic initializer (0x00536FE0, _$E17) emitted by HeaderStatics_diagonal_init_00536f30.cpp; no source of its own.


// Dynamic initializer (0x00537000, _$E20) emitted by HeaderStatics_diagonal_init_00536f30.cpp; no source of its own.


// Dynamic initializer (0x00537040, _$E23) emitted by HeaderStatics_diagonal_init_00536f30.cpp; no source of its own.


// Dynamic initializer (0x00537070, _$E26) emitted by HeaderStatics_diagonal_init_00536f30.cpp; no source of its own.


// Dynamic initializer (0x005370A0, _$E29) emitted by HeaderStatics_diagonal_init_00536f30.cpp; no source of its own.


// Dynamic initializer (0x005370D0, _$E32) emitted by HeaderStatics_diagonal_init_00536f30.cpp; no source of its own.


// Dynamic initializer (0x005370E0, _$E35) emitted by HeaderStatics_diagonal_init_00536f30.cpp; no source of its own.


// Dynamic initializer (0x00537100, _$E38) emitted by HeaderStatics_diagonal_init_00536f30.cpp; no source of its own.


// Dynamic initializer (0x00537120, _$E49) emitted by HeaderStatics_diagonal_init_00536f30.cpp; no source of its own.


// atexit destructor (0x00537160, _$E47) emitted by HeaderStatics_diagonal_init_00536f30.cpp; no source of its own.


// Dynamic initializer (0x005371A0, _$E55) emitted by HeaderStatics_diagonal_init_00536f30.cpp; no source of its own.


// atexit destructor (0x005371E0, _$E52) emitted by HeaderStatics_diagonal_init_00536f30.cpp; no source of its own.


// LightSourceClass::LightSourceClass (0x00537240): create and register an
// initially inactive light with the supplied position, radius and tint.

LightSourceClass::LightSourceClass(CoordStruct coord, int visibility, int intensity, int red, int green, int blue) :
    Intensity(intensity), RedTint(red), GreenTint(green), BlueTint(blue), DetailLevel(2), Location(coord), Visibility(visibility), Activated(false)
{
    unknown_LightSourceClass_list_00a6e818.Add(this);
}


// LightSourceClass::LightSourceClass() (0x00537310): an unlit light at the
// zero coordinate; added to the light list.

LightSourceClass::LightSourceClass() :
    Intensity(0),
    RedTint(0), GreenTint(0), BlueTint(0),
    Location(unknown_geometry_l),
    Visibility(0),
    Activated(false)
{
    unknown_LightSourceClass_list_00a6e818.Add(this);
}


// LightSourceClass::LightSourceClass(NoInitClass const &noinit) (0x005373C0): installs the vtables only
// (the load path restores the object in place; Load 0x00537BA0 expands it).

LightSourceClass::LightSourceClass(NoInitClass const &noinit) :
    AbstractClass(noinit)
{
}


// LightSourceClass::~LightSourceClass() (0x005373F0): detaches the light from
// everything, leaves the light list and, when active, turns it off.
#include "shared/radsite.hpp"

LightSourceClass::~LightSourceClass()
{
    Detach_This_From_All(this, true);
    unknown_LightSourceClass_list_00a6e818.Delete(this);
    if (Activated) {
        Activated = false;
        Unknown_005375D0(0);
    }
}


// void LightSourceClass::Clear_All() (0x00537480): deletes every light and
// every pending cell, with cell updates suspended meanwhile.

void LightSourceClass::Clear_All()
{
    unknown_flag_7df630 = false;
    while (unknown_LightSourceClass_list_00a6e818.Count() != 0) {
        LightSourceClass *light = unknown_LightSourceClass_list_00a6e818[0];
        if (light != 0) {
            delete light;
        }
        unknown_LightSourceClass_list_00a6e818.Delete(0);
    }
    while (PendingCellClass::Array.Count() != 0) {
        delete PendingCellClass::Array[0];
        PendingCellClass::Array.Delete(0);
    }
    unknown_flag_7df630 = true;
}


// void LightSourceClass::Activate(bool defer) (0x00537540): turns the light on
// and lights its cells.

void LightSourceClass::Activate(bool defer)
{
    if (!Activated) {
        Activated = true;
        Unknown_005375D0(defer);
    }
}


// void LightSourceClass::Deactivate(bool defer) (0x00537560): turns the light off
// and relights its cells.

void LightSourceClass::Deactivate(bool defer)
{
    if (Activated) {
        Activated = false;
        Unknown_005375D0(defer);
    }
}


// void LightSourceClass::Change(int intensity, int red, int green, int blue, bool defer) (0x00537580):
// takes the new intensity and colour unless they look unchanged, and relights
// the cells when active. (The test compares the stored red with both red and
// blue and never looks at green: as in the original.)

void LightSourceClass::Change(int intensity, int red, int green, int blue, bool defer)
{
    if (Intensity != intensity || RedTint != red || RedTint != blue) {
        Intensity = intensity;
        GreenTint = green;
        RedTint = red;
        BlueTint = blue;
        if (Activated) {
            Unknown_005375D0(defer);
        }
    }
}


// LightSourceClass::Unknown_005375D0(bool defer) (0x005375D0): TibSun's Recalculate_Affected_Cells with
// RA2's detail-level test.

void LightSourceClass::Unknown_005375D0(bool defer)
{
    if (unknown_flag_7df630 && Options.DetailLevel >= DetailLevel) {
        if (defer && PendingCellClass::Array.Count() != 0) {
            Unknown_00537830(0, true);
        }

        Cell center = Cell(Location);
        int radius = Visibility / 256 + 1;

        for (int y = -radius; y <= radius; y++) {
            for (int x = -radius; x <= radius; x++) {
                Cell test_cell = center + Cell(x, y);
                if (test_cell.X >= 0 && test_cell.X < 512 && test_cell.Y >= 0 && test_cell.Y < 512 && Map.Has_Cell(test_cell)) {
                    CoordStruct cell_coord = Cell_Coord(test_cell);
                    int dx = cell_coord.X - Location.X;
                    int dy = cell_coord.Y - Location.Y;
                    int dist = dx * dx + dy * dy;

                    if ((int)unknown_sqrt_4ba410(dist) <= Visibility) {
                        if (defer) {
                            PendingCellClass::Array.Add(new PendingCellClass(test_cell));
                        } else {
                            Map[test_cell].Init_Drawer(0, 0x10000, 0, 1000, 1000, 1000);
                        }
                    }
                }
            }
        }

        Map.Flag_To_Redraw(true);
    }
}


// LightSourceClass::Unknown_00537830(int time, bool force) (0x00537830): TibSun's Process_Lighting. Picks
// the drawers of the pending cells (newest first, checking the time budget every 16 cells unless forced);
// when every pending cell has its values and there is time (or forced), applies them all and frees the list;
// with time left, frees unused tile drawers every _update_interval frames.

void __fastcall LightSourceClass::Unknown_00537830(int time, bool force)
{
    static unsigned _update_frames = 0;

    double update_start_time = MillisecondTimer;

    static unsigned _update_interval = 50;
    static unsigned _last_update_frame = 0;
    static int _pending_index = 0;
    static int _pending_remaining = 0;
    static bool _commit_ready = false;

    _update_frames++;

    if (PendingCellClass::Array.Count() && !_commit_ready) {
        if (_pending_index <= 0) {
            _pending_remaining = PendingCellClass::Array.Count();
            _pending_index = _pending_remaining - 1;
        }
        for (; _pending_index >= 0; _pending_index--) {
            if (!force && ((unsigned)_pending_index % 16) == 15) {
                if (MillisecondTimer - update_start_time >= time) {
                    break;
                }
            }

            if (PendingCellClass::Array[_pending_index]->Converter != 0) {
                _pending_remaining--;
            } else {
                LightConvertClass *drawer;
                int intensity, ambient, brightness, tile_brightness, alt_brightness;
                Map[PendingCellClass::Array[_pending_index]->CellID].Pick_Drawer(drawer, intensity, ambient, brightness, tile_brightness, alt_brightness);
                PendingCellClass::Array[_pending_index]->Converter = drawer;
                PendingCellClass::Array[_pending_index]->Intensity = intensity;
                PendingCellClass::Array[_pending_index]->Ambient = ambient;
                PendingCellClass::Array[_pending_index]->Brightness = brightness;
                PendingCellClass::Array[_pending_index]->TileBrightness = tile_brightness;
                PendingCellClass::Array[_pending_index]->AltBrightness = alt_brightness;
                _pending_remaining--;
            }
        }
        if (_pending_remaining <= 0) {
            _commit_ready = true;
        }
    }

    double time_left = time - (MillisecondTimer - update_start_time);

    if (_commit_ready) {
        if (force || time_left >= time - 1) {
            for (int i = 0; i < PendingCellClass::Array.Count(); i++) {
                PendingCellClass *pending = PendingCellClass::Array[i];
                Map[pending->CellID].Init_Drawer(pending->Converter, pending->Intensity, pending->Ambient, pending->Brightness, pending->TileBrightness, pending->AltBrightness);
                delete pending;
            }
            PendingCellClass::Array.Clear();
            _commit_ready = false;
            Map.Flag_To_Redraw(true);
        }
    }

    time_left = time - (MillisecondTimer - update_start_time);

    if (time_left > 0.0 && _update_frames > (_last_update_frame + _update_interval)) {
        bool more_to_free = Unknown_00529710(time_left * 0.5 < 1.0 ? time_left * 0.5 : 1.0, false);
        _last_update_frame = _update_frames;
        _update_interval = more_to_free ? 1 : 50;
    }
}


// void LightSourceClass::Compute_CRC(WWCRCEngine &crc) const (0x00537B50, slot 13): only the AbstractClass part.

void LightSourceClass::Compute_CRC(WWCRCEngine &crc) const
{
    AbstractClass::Compute_CRC(crc);
}


// HRESULT LightSourceClass::GetClassID(GUID *clsid) (0x00537B60, slot 3)

HRESULT __stdcall LightSourceClass::GetClassID(GUID *clsid)
{
    if (!clsid) {
        return E_POINTER;
    }
    *clsid = CLSID_LightSource;
    return S_OK;
}


// HRESULT __stdcall LightSourceClass::Load(IStream *stream) (0x00537BA0):
// restores the light in place.

HRESULT __stdcall LightSourceClass::Load(IStream *stream)
{
    HRESULT result = AbstractClass::Load(stream);
    if (SUCCEEDED(result)) {
        new (this) LightSourceClass(NoInitClass());
        result = S_OK;
    }
    return result;
}


// 0x00537bf0: function not yet identified
//
// Owner: LightSourceClass, virtual slot 6 (vtable +0x18) (from RTTI/vtables).
// Body: 29 bytes (frozen inventory bound).
// Referenced from 1 site (data).
// Ghidra prototype: int Save_00537bf0(undefined4 param_1,undefined4 param_2,undefined4 param_3)
//
// Types and names are Ghidra placeholders;
// rename and retype as the purpose becomes known.
//

namespace seed_00537bf0 {

extern unsigned int __stdcall Save_004100c0(int, int, int);

int __stdcall Save_00537bf0(unsigned int object_address, unsigned int stream_address, unsigned int clear_dirty)
{
    int result;

    result = (int)(Save_004100c0((int)(object_address), (int)(stream_address), (int)(clear_dirty)));
    if (result >= 0) {
        result = (int)(0);
    }
    return result;
}

}  // namespace seed_00537bf0


class Leaf_537c10 { public: unsigned int get(); };
unsigned int Leaf_537c10::get() { return 0x4c; }


class Leaf_537c20 { public: unsigned int get(); };
unsigned int Leaf_537c20::get() { return 0x36; }


// LightSourceClass scalar deleting destructor (0x00537C30): compiler generated (the destructor expanded inline); no source of its own.


// Container members this unit emits by its own use of its lists (bodies: shared/vector_class.hpp):
// DynamicVectorClass<LightSourceClass *>::Clear (0x00537CE0; DynamicVectorClass<LightSourceClass *> vtable 0x007a54b4 slot 3).
// DynamicVectorClass<LightSourceClass *>::ID_pointer (0x00537D10; DynamicVectorClass<LightSourceClass *> vtable 0x007a54b4 slot 5).
// VectorClass<LightSourceClass *>::Fetch (0x00537D30; VectorClass<LightSourceClass *> vtable 0x007a54d4 slot 6).


// 0x00537d40: function not yet identified
//
// Owner: DynamicVectorClass<LightSourceClass::PendingCellClass *>, virtual slot 3 (vtable +0xc) (from RTTI/vtables).
// Body: 42 bytes (frozen inventory bound).
// Referenced from 1 site (data).
// Ghidra prototype: void __fastcall Clear_00537d40(int param_1)
// Callees with accepted source:
//   FUN_00781d3b -> src/recovered/00781d3b.cpp
//
// Types and names are Ghidra placeholders;
// rename and retype as the purpose becomes known.
//

namespace seed_00537d40 {

extern void  operator_delete_00781d3b(unsigned int allocation_address);

void __fastcall Clear_00537d40(int vector_address)
{
    *(unsigned int *)(vector_address + 0x10) = 0;
    if ((*(int *)(vector_address + 4) != 0) && (*(char *)(vector_address + 0xd) != '\0')) {
        operator_delete_00781d3b((unsigned int)(*(int *)(vector_address + 4)));
        *(unsigned int *)(vector_address + 4) = 0;
    }
    *(unsigned char *)(vector_address + 0xd) = 0;
    *(unsigned int *)(vector_address + 8) = 0;
    return;
}

}  // namespace seed_00537d40


// DynamicVectorClass<LightSourceClass::PendingCellClass *>::ID(PendingCellClass * const *) (0x00537D70)
// Template member instantiated from shared/vector_class.hpp.
// No source of its own: emitted by this unit (population-2563).


// Container members this unit emits by its own use of its lists (bodies: shared/vector_class.hpp):
// VectorClass<LightSourceClass::PendingCellClass *>::Fetch (0x00537D90; VectorClass<LightSourceClass::PendingCellClass *> vtable 0x007a5514 slot 6).
// VectorClass<LightSourceClass *>::operator equal (0x00537DA0; VectorClass<LightSourceClass *> vtable 0x007a54d4 slot 1).
// VectorClass<LightSourceClass *>::Resize (0x00537DE0; VectorClass<LightSourceClass *> vtable 0x007a54d4 slot 2).
// VectorClass<LightSourceClass *>::Clear (0x00537E90; VectorClass<LightSourceClass *> vtable 0x007a54d4 slot 3).
// VectorClass<LightSourceClass *>::ID_object (0x00537EC0; VectorClass<LightSourceClass *> vtable 0x007a54d4 slot 4).
// VectorClass<LightSourceClass *>::ID_pointer (0x00537F00; VectorClass<LightSourceClass *> vtable 0x007a54d4 slot 5).
// DynamicVectorClass<LightSourceClass *>::Resize (0x00537F20; DynamicVectorClass<LightSourceClass *> vtable 0x007a54b4 slot 2).
// DynamicVectorClass<LightSourceClass *>::ID_object (0x00537FD0; DynamicVectorClass<LightSourceClass *> vtable 0x007a54b4 slot 4).
// VectorClass<LightSourceClass::PendingCellClass *>::operator equal (0x00538000; VectorClass<LightSourceClass::PendingCellClass *> vtable 0x007a5514 slot 1).
// VectorClass<LightSourceClass::PendingCellClass *>::Resize (0x00538040; VectorClass<LightSourceClass::PendingCellClass *> vtable 0x007a5514 slot 2).
// VectorClass<LightSourceClass::PendingCellClass *>::Clear (0x005380F0; VectorClass<LightSourceClass::PendingCellClass *> vtable 0x007a5514 slot 3).
// VectorClass<LightSourceClass::PendingCellClass *>::ID_object (0x00538120; VectorClass<LightSourceClass::PendingCellClass *> vtable 0x007a5514 slot 4).
// VectorClass<LightSourceClass::PendingCellClass *>::ID_pointer (0x00538160; VectorClass<LightSourceClass::PendingCellClass *> vtable 0x007a5514 slot 5).
// DynamicVectorClass<LightSourceClass::PendingCellClass *>::Resize (0x00538180; DynamicVectorClass<LightSourceClass::PendingCellClass *> vtable 0x007a54f4 slot 2).
// DynamicVectorClass<LightSourceClass::PendingCellClass *>::ID_object (0x00538230; DynamicVectorClass<LightSourceClass::PendingCellClass *> vtable 0x007a54f4 slot 4).


extern "C" unsigned char vtable_538260[]; extern "C" void __cdecl release_buffer_538260(void*); extern "C" void __cdecl release_object_538260(void*);
class Leaf_538260 { public: void* get(unsigned int flags); };
void* Leaf_538260::get(unsigned int flags) { unsigned int pointer = *(unsigned int*)((char*)this + 4); *(unsigned int*)this=(unsigned int)vtable_538260; if(pointer && *(unsigned char*)((char*)this + 13)){ release_buffer_538260((void*)pointer); *(unsigned int*)((char*)this + 4)=0; } *(unsigned char*)((char*)this + 13)=0; *(unsigned int*)((char*)this + 8)=0; if(flags & 1) release_object_538260(this); return this; }


extern "C" unsigned char vtable_5382b0[]; extern "C" void __cdecl release_buffer_5382b0(void*); extern "C" void __cdecl release_object_5382b0(void*);
class Leaf_5382b0 { public: void* get(unsigned int flags); };
void* Leaf_5382b0::get(unsigned int flags) { unsigned int pointer = *(unsigned int*)((char*)this + 4); *(unsigned int*)this=(unsigned int)vtable_5382b0; if(pointer && *(unsigned char*)((char*)this + 13)){ release_buffer_5382b0((void*)pointer); *(unsigned int*)((char*)this + 4)=0; } *(unsigned char*)((char*)this + 13)=0; *(unsigned int*)((char*)this + 8)=0; if(flags & 1) release_object_5382b0(this); return this; }


extern "C" unsigned char vtable_538300[]; extern "C" void __cdecl release_buffer_538300(void*); extern "C" void __cdecl release_object_538300(void*);
class Leaf_538300 { public: void* get(unsigned int flags); };
void* Leaf_538300::get(unsigned int flags) { unsigned int pointer = *(unsigned int*)((char*)this + 4); *(unsigned int*)this=(unsigned int)vtable_538300; if(pointer && *(unsigned char*)((char*)this + 13)){ release_buffer_538300((void*)pointer); *(unsigned int*)((char*)this + 4)=0; } *(unsigned char*)((char*)this + 13)=0; *(unsigned int*)((char*)this + 8)=0; if(flags & 1) release_object_538300(this); return this; }


extern "C" unsigned char vtable_538350[]; extern "C" void __cdecl release_buffer_538350(void*); extern "C" void __cdecl release_object_538350(void*);
class Leaf_538350 { public: void* get(unsigned int flags); };
void* Leaf_538350::get(unsigned int flags) { unsigned int pointer = *(unsigned int*)((char*)this + 4); *(unsigned int*)this=(unsigned int)vtable_538350; if(pointer && *(unsigned char*)((char*)this + 13)){ release_buffer_538350((void*)pointer); *(unsigned int*)((char*)this + 4)=0; } *(unsigned char*)((char*)this + 13)=0; *(unsigned int*)((char*)this + 8)=0; if(flags & 1) release_object_538350(this); return this; }
