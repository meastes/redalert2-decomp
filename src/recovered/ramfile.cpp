// ramfile.cpp: the original translation unit re-filed as one source (Route A): the accepted
// bodies of its canonical compile group in address order, under the unit's own header
// state (its switches and includes in their original order). The module map
// manifests/original_target/modules/ramfile.json carries the addresses and the rows that
// stay in their own files.

#define RA2_RADSITE_COORD_DISTANCE_DIFFERENCE
// Header-static dynamic initializers of the unit at 0x00637030 and the
// unit's RadSiteClass list; the other drafts of this plan are emitted by this file.
#include "shared/unknown_geometry.hpp"
#include "shared/unknown_static_rect.hpp"
#ifndef VECTOR_IMPLEMENT_ALL
#define VECTOR_IMPLEMENT_ALL
#endif
#include "shared/vector_class.hpp"
#include "shared/radsite.hpp"

// The unit's list of RadSiteClass objects (0x00AB6620; name is a hypothesis).
DynamicVectorClass<RadSiteClass *> unknown_RadSiteClass_list_00ab6620;


// Dynamic initializer (0x00637060, _$E) emitted by HeaderStatics_diagonal_init_00637030.cpp; no source of its own.


// Dynamic initializer (0x00637080, _$E) emitted by HeaderStatics_diagonal_init_00637030.cpp; no source of its own.


// Dynamic initializer (0x006370A0, _$E) emitted by HeaderStatics_diagonal_init_00637030.cpp; no source of its own.


// Dynamic initializer (0x006370C0, _$E) emitted by HeaderStatics_diagonal_init_00637030.cpp; no source of its own.


// Dynamic initializer (0x006370E0, _$E) emitted by HeaderStatics_diagonal_init_00637030.cpp; no source of its own.


// Dynamic initializer (0x00637100, _$E) emitted by HeaderStatics_diagonal_init_00637030.cpp; no source of its own.


// Dynamic initializer (0x00637140, _$E) emitted by HeaderStatics_diagonal_init_00637030.cpp; no source of its own.


// Dynamic initializer (0x00637170, _$E) emitted by HeaderStatics_diagonal_init_00637030.cpp; no source of its own.


// Dynamic initializer (0x006371A0, _$E) emitted by HeaderStatics_diagonal_init_00637030.cpp; no source of its own.


// Dynamic initializer (0x006371D0, _$E) emitted by HeaderStatics_diagonal_init_00637030.cpp; no source of its own.


// Dynamic initializer (0x006371E0, _$E) emitted by HeaderStatics_diagonal_init_00637030.cpp; no source of its own.


// Dynamic initializer (0x00637200, _$E) emitted by HeaderStatics_diagonal_init_00637030.cpp; no source of its own.


// Dynamic initializer (0x00637220, _$E) emitted by HeaderStatics_diagonal_init_00637030.cpp; no source of its own.


// Dynamic initializer (0x00637260, _$E) emitted by HeaderStatics_diagonal_init_00637030.cpp; no source of its own.




#include "shared/radsite.hpp"
#include "shared/map_view.hpp"
#include "shared/id_source.hpp"
#include "shared/rgb_class.hpp"
#include "shared/coord_distance.hpp"


// ??0RadSiteClass@@QAE@XZ (0x006372A0)
// Clears the site and lists it in the unit list.

RadSiteClass::RadSiteClass() :
    LightSource(0),
    BaseCell(0, 0),
    Spread(0),
    SpreadInLeptons(0),
    RadLevel(0),
    unknown_50(0),
    unknown_54(0),
    unknown_64(0),
    unknown_68(0),
    RadDuration(0),
    RadTimeLeft(0)
{
    unknown_RadSiteClass_list_00ab6620.Add(this);
}


// ??0RadSiteClass@@QAE@ABVNoInitClass@@@Z (0x00637360)
// Load path: the vtables and fresh timers.

RadSiteClass::RadSiteClass(NoInitClass const &noinit) : AbstractClass(noinit)
{
}


// ??1RadSiteClass@@UAE@XZ (0x006373B0)
// Clears the radiation of the base cell, detaches the site, deletes its light and unlists it.

RadSiteClass::~RadSiteClass()
{
    RadSiteCell &cell = (RadSiteCell &)Map[BaseCell];
    cell.Unknown_0047CE30(0);
    Detach_This_From_All(this, true);
    if (LightSource != 0) {
        delete LightSource;
        LightSource = 0;
    }
    unknown_RadSiteClass_list_00ab6620.Delete(this);
}


// ?Size_Of@RadSiteClass@@UBEHXZ (0x00637460)
// Object size.

int RadSiteClass::Size_Of() const
{
    return sizeof(RadSiteClass);
}


// ?Compute_CRC@RadSiteClass@@UBEXAAVWWCRCEngine@@@Z (0x00637470)
// Only the AbstractClass part.

void RadSiteClass::Compute_CRC(WWCRCEngine &crc) const
{
    AbstractClass::Compute_CRC(crc);
}


// ?Kind_Of@RadSiteClass@@UBEHXZ (0x00637480)
// RTTI kind 0x45.

int RadSiteClass::Kind_Of() const
{
    return 0x45;
}


// ?Load@RadSiteClass@@UAGJPAUIStream@@@Z (0x00637490)
// AbstractClass load, vtables, light pointer fixup.

HRESULT __stdcall RadSiteClass::Load(IStream *stream)
{
    HRESULT result = AbstractClass::Load(stream);
    if (SUCCEEDED(result)) {
        new (this) RadSiteClass(NoInitClass());
        Swizzler.Swizzle((void **)&LightSource);
    }
    return result;
}


// ?Save@RadSiteClass@@UAGJPAUIStream@@H@Z (0x00637510)
// Only the AbstractClass part.

HRESULT __stdcall RadSiteClass::Save(IStream *stream, BOOL clear_dirty)
{
    return AbstractClass::Save(stream, clear_dirty);
}


// ?GetClassID@RadSiteClass@@UAGJPAUGUID@@@Z (0x00637530)
// The class identifier.

HRESULT __stdcall RadSiteClass::GetClassID(GUID *classid)
{
    if (classid == 0) {
        return E_POINTER;
    }
    *classid = CLSID_RadSiteClass;
    return S_OK;
}


// ?Get_Spread@RadSiteClass@@QBEHXZ (0x00637570)
// Spread in cells.

int RadSiteClass::Get_Spread() const
{
    return Spread;
}


// ?Set_Base_Cell@RadSiteClass@@QAEXABVCell@@@Z (0x00637580)
// Centre cell.

void RadSiteClass::Set_Base_Cell(Cell const &cell)
{
    BaseCell = cell;
}


// ?Set_Spread@RadSiteClass@@QAEXH@Z (0x00637590)
// Spread in cells and in leptons (cell centre).

void RadSiteClass::Set_Spread(int spread)
{
    Spread = spread;
    SpreadInLeptons = spread * 256 + 128;
}


// ?Set_Rad_Level@RadSiteClass@@QAEXH@Z (0x006375B0)
// Radiation level; the duration is level times the rules multiple.

void RadSiteClass::Set_Rad_Level(int level)
{
    RadLevel = level;
    RadTimeLeft = RadDuration = Rules->RadDurationMultiple * level;
}


// ?Get_Current_Level@RadSiteClass@@QBEHXZ (0x006375D0)
// Level scaled by the time left.

int RadSiteClass::Get_Current_Level() const
{
    if (RadDuration <= 0) {
        return 0;
    }
    return RadTimeLeft * RadLevel / RadDuration;
}


// void RadSiteClass::Increase_Radiation(int amount) (0x006375F0)
// Takes the site's current (decayed) level, clears its effect
// (0x00637C10), restarts it at that level plus amount and reapplies it
// (0x00637640). Names are hypotheses.

void RadSiteClass::Increase_Radiation(int amount)
{
    double level_now = RadLevel;
    double left = RadTimeLeft;
    double duration = RadDuration;
    int level = (int)(level_now * left / duration);
    Unknown_00637C10();
    Set_Rad_Level(level + amount);
    Unknown_00637640();
}


// void RadSiteClass::Unknown_00637640() (0x00637640)
// Starts the site's timers, derives its decay steps and light from the rules
// (RadColor, RadLightFactor, RadTintFactor), then creates or retints the
// light and fills the cells (0x00637A80).
extern double const unknown_double_7a6020;  // 0x007A6020 (2000.0)

void RadSiteClass::Unknown_00637640()
{
    int level_delay = Rules->RadLevelDelay;
    int light_delay = Rules->RadLightDelay;
    unknown_28 = CDTimerClass<FrameTimerClass>(level_delay);
    unknown_34 = CDTimerClass<FrameTimerClass>(light_delay);
    int red = Rules->RadColor.Red;
    int green = Rules->RadColor.Green;
    int blue = Rules->RadColor.Blue;
    if (unknown_id_source_a3d290->Theater == 1) {
        red = blue = -128;
    }
    red = red * 1000 / 255;
    green = green * 1000 / 255;
    blue = blue * 1000 / 255;
    int visibility = SpreadInLeptons;
    double v = RadLevel;
    v = v * Rules->RadLightFactor;
    int intensity = (int)(unknown_double_7a6020 < v ? unknown_double_7a6020 : v);
    v = red * Rules->RadTintFactor;
    int r = (int)(unknown_double_7a6020 < v ? unknown_double_7a6020 : v);
    v = green * Rules->RadTintFactor;
    int g = (int)(unknown_double_7a6020 < v ? unknown_double_7a6020 : v);
    v = blue * Rules->RadTintFactor;
    int b = (int)(unknown_double_7a6020 < v ? unknown_double_7a6020 : v);
    unknown_58 = r;
    unknown_5c = g;
    unknown_60 = b;
    unknown_54 = intensity;
    unknown_50 = RadDuration / level_delay;
    unknown_64 = RadDuration / light_delay;
    unknown_68 = intensity / unknown_64;
    RadSiteCell &cell = (RadSiteCell &)Map[BaseCell];
    CoordStruct coord = cell.Center_Coord();
    if (LightSource == 0) {
        LightSource = new LightSourceClass(coord, visibility, intensity, r, g, b);
        LightSource->DetailLevel = 0;
        LightSource->Activate(0);
    } else {
        LightSource->Change(intensity, r, g, b, 0);
    }
    Unknown_00637A80();
}


// void RadSiteClass::AI() (0x006378C0, slot 23)
// Counts the site down; every RadLevelDelay frames updates the cells
// (0x00637DC0), every RadLightDelay frames dims the light, and deletes
// the site when its time is up.

void RadSiteClass::AI()
{
    RadTimeLeft--;
    if (unknown_28.Value() == 0) {
        Unknown_00637DC0();
        unknown_28 = CDTimerClass<FrameTimerClass>(Rules->RadLevelDelay);
    }
    if (unknown_34.Value() == 0) {
        Update_Light();
        unknown_34 = CDTimerClass<FrameTimerClass>(Rules->RadLightDelay);
    }
    if (RadTimeLeft <= 0) {
        delete this;
    }
}


// double RadSiteClass::Get_Rad_Level_At(Cell const &cell) (0x006379B0)
// The site's level at a cell: falls off linearly from the base cell to 0 at
// the spread's edge (leptons); 0 outside it.

double RadSiteClass::Get_Rad_Level_At(Cell const &cell)
{
    RadSiteCell &base = (RadSiteCell &)Map[BaseCell];
    RadSiteCell &target = (RadSiteCell &)Map[cell];
    CoordStruct base_coord = base.Center_Coord();
    CoordStruct target_coord = target.Center_Coord();
    int distance = Coord_Distance(base_coord, target_coord);
    double level = 0.0;
    if (distance <= SpreadInLeptons) {
        level = (double)(SpreadInLeptons - distance) / SpreadInLeptons * RadLevel;
    }
    return level;
}


// RadSiteClass cell radiation update, reconstructed from complete original.
void RadSiteClass::Unknown_00637A80()
{
    int xstart = BaseCell.X - Spread;
    int ystart = BaseCell.Y - Spread;
    int xend = xstart + Spread * 2 + 1;
    int yend = ystart + Spread * 2 + 1;
    for (int y = ystart; y < yend; y++) {
        for (int x = xstart; x < xend; x++) {
            Cell cell(x, y);
            double level = Get_Rad_Level_At(cell);
            RadSiteCell &target = (RadSiteCell &)Map[cell];
            target.Unknown_0047CEA0(level);
        }
    }
}


// void RadSiteClass::Unknown_00637C10() (0x00637C10)
// Takes the site's remaining effect off every cell of the spread square: each
// cell's level (Get_Rad_Level_At) scaled by the decay steps left,
// RadTimeLeft / RadLevelDelay + 1, over unknown_50.

void RadSiteClass::Unknown_00637C10()
{
    int steps = RadTimeLeft / Rules->RadLevelDelay + 1;
    int xstart = BaseCell.X - Spread;
    int ystart = BaseCell.Y - Spread;
    int xend = xstart + Spread * 2 + 1;
    int yend = ystart + Spread * 2 + 1;
    for (int y = ystart; y < yend; y++) {
        for (int x = xstart; x < xend; x++) {
            Cell cell(x, y);
            double level = Get_Rad_Level_At(cell);
            level = level / unknown_50 * steps;
            RadSiteCell &target = (RadSiteCell &)Map[cell];
            target.Unknown_0047CEC0(level);
        }
    }
}


// RadSiteClass cell radiation update, reconstructed from complete original.
void RadSiteClass::Unknown_00637DC0()
{
    int xstart = BaseCell.X - Spread;
    int ystart = BaseCell.Y - Spread;
    int xend = xstart + Spread * 2 + 1;
    int yend = ystart + Spread * 2 + 1;
    for (int y = ystart; y < yend; y++) {
        for (int x = xstart; x < xend; x++) {
            Cell cell(x, y);
            double level = Get_Rad_Level_At(cell);
            level /= unknown_50;
            RadSiteCell &target = (RadSiteCell &)Map[cell];
            target.Unknown_0047CEC0(level);
        }
    }
}


// ?Update_Light@RadSiteClass@@QAEXXZ (0x00637F50)
// Scales the light colour by the remaining time and applies it.

void RadSiteClass::Update_Light()
{
    LightSource->Change(LightSource->Intensity - unknown_68,
                        unknown_58 * RadTimeLeft / RadDuration,
                        unknown_5c * RadTimeLeft / RadDuration,
                        unknown_60 * RadTimeLeft / RadDuration, 0);
}


// RadSiteClass::`scalar deleting destructor' (0x00637F90)
// Compiler generated from the virtual destructor (0x006373B0).


// 0x00637fb0: function not yet identified
//
// Owner: DynamicVectorClass<RadSiteClass *>, virtual slot 3 (vtable +0xc) (from RTTI/vtables).
// Body: 42 bytes (frozen inventory bound).
// Referenced from 1 site (data).
// Ghidra prototype: void __fastcall FUN_00637fb0(int param_1)
// Callees with accepted source:
//   FUN_00781d3b -> src/recovered/00781d3b.cpp
//
// Types and names are Ghidra placeholders;
// rename and retype as the purpose becomes known.
//

namespace seed_00637fb0 {

extern void  operator_delete_00781d3b(unsigned int allocation_address);

void __fastcall Clear_00637fb0(int vector_address)
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

}  // namespace seed_00637fb0


// DynamicVectorClass<RadSiteClass *>::ID(RadSiteClass * const *) (0x00637FE0)
// Template member instantiated from shared/vector_class.hpp.
// No source of its own: emitted by this unit (population-2333).


// Container members this unit emits by its own use of its lists (bodies: shared/vector_class.hpp):
// VectorClass<RadSiteClass *>::Fetch (0x00638000; VectorClass<RadSiteClass *> vtable 0x007a8b84 slot 6).
// VectorClass<RadSiteClass *>::operator equal (0x00638010; VectorClass<RadSiteClass *> vtable 0x007a8b84 slot 1).
// VectorClass<RadSiteClass *>::Resize (0x00638050; VectorClass<RadSiteClass *> vtable 0x007a8b84 slot 2).
// VectorClass<RadSiteClass *>::Clear (0x00638100; VectorClass<RadSiteClass *> vtable 0x007a8b84 slot 3).
// VectorClass<RadSiteClass *>::ID_object (0x00638130; VectorClass<RadSiteClass *> vtable 0x007a8b84 slot 4).
// VectorClass<RadSiteClass *>::ID_pointer (0x00638170; VectorClass<RadSiteClass *> vtable 0x007a8b84 slot 5).
// DynamicVectorClass<RadSiteClass *>::Resize (0x00638190; DynamicVectorClass<RadSiteClass *> vtable 0x007a8b64 slot 2).
// DynamicVectorClass<RadSiteClass *>::ID_object (0x00638240; DynamicVectorClass<RadSiteClass *> vtable 0x007a8b64 slot 4).


extern "C" unsigned char vtable_638270[]; extern "C" void __cdecl release_buffer_638270(void*); extern "C" void __cdecl release_object_638270(void*);
class Leaf_638270 { public: void* get(unsigned int flags); };
void* Leaf_638270::get(unsigned int flags) { unsigned int pointer = *(unsigned int*)((char*)this + 4); *(unsigned int*)this=(unsigned int)vtable_638270; if(pointer && *(unsigned char*)((char*)this + 13)){ release_buffer_638270((void*)pointer); *(unsigned int*)((char*)this + 4)=0; } *(unsigned char*)((char*)this + 13)=0; *(unsigned int*)((char*)this + 8)=0; if(flags & 1) release_object_638270(this); return this; }


extern "C" unsigned char vtable_6382c0[]; extern "C" void __cdecl release_buffer_6382c0(void*); extern "C" void __cdecl release_object_6382c0(void*);
class Leaf_6382c0 { public: void* get(unsigned int flags); };
void* Leaf_6382c0::get(unsigned int flags) { unsigned int pointer = *(unsigned int*)((char*)this + 4); *(unsigned int*)this=(unsigned int)vtable_6382c0; if(pointer && *(unsigned char*)((char*)this + 13)){ release_buffer_6382c0((void*)pointer); *(unsigned int*)((char*)this + 4)=0; } *(unsigned char*)((char*)this + 13)=0; *(unsigned int*)((char*)this + 8)=0; if(flags & 1) release_object_6382c0(this); return this; }
