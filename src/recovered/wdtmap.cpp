// wdtmap.cpp: the original translation unit re-filed as one source (Route A): the accepted
// bodies of its canonical compile group in address order, under the unit's own header
// state (its switches and includes in their original order). The module map
// manifests/original_target/modules/wdtmap.json carries the addresses and the rows that
// stay in their own files.


#include "shared/wdt_map.hpp"
#define RA2_WDT_MAP_LOAD_LAYOUT
#define RA2_RECT_POINT_INLINE
#include "shared/wdt_voices.hpp"
#include "shared/ms_anim.hpp"
#include "shared/cc_file_class.hpp"
#include "shared/vector_elements.hpp"
#include "shared/cd_file_class.hpp"
#include "shared/ini_class.hpp"
#include "shared/surface.hpp"
#include "shared/rect.hpp"
#include "shared/trect.hpp"
#include "shared/unknown_globals.hpp"
#include "shared/vector_class.hpp"
#include "shared/vector_cursor.hpp"
#include "shared/wstring.hpp"


// MSFrameAnim::MSFrameAnim() (0x007291C0): an MSAnim at the origin with a stopped timer.

MSFrameAnim::MSFrameAnim()
{
}


// 0x007291f0: function not yet identified
//
// Owner: MSFrameAnim, stores the MSFrameAnim vtable (constructor or destructor) (from RTTI/vtables).
// Body: 11 bytes (frozen inventory bound).
// Referenced from 0 sites.
// Ghidra prototype: void __fastcall MSFrameAnim_dtor_007291f0(undefined4 *param_1)
// Callees with accepted source:
//   MSAnim_dtor_005ac690 -> src/recovered/classes/MSAnim/005ac690.cpp
//
// Types and names are Ghidra placeholders;
// rename and retype as the purpose becomes known.
//

namespace seed_007291f0 {

extern void __fastcall MSAnim_dtor_005ac690(void);
extern "C" void* MSFrameAnim_vtable_007af1bc;

void __fastcall MSFrameAnim_dtor_007291f0(unsigned int *self)
{
    *self = (unsigned int)(&MSFrameAnim_vtable_007af1bc);
    MSAnim_dtor_005ac690();
    return;
}

}  // namespace seed_007291f0


// void MSFrameAnim::Draw(Surface *surface, Rect const *clip) (0x00729220, slot 5): while the
// flag 0x007FBD05 is set, draws the interface frame clipped to clip (the whole surface when 0).

void MSFrameAnim::Draw(Surface *surface, Rect const *clip)
{
    if (unknown_flag_7fbd05) {
        int width = surface->Get_Width();
        int height = surface->Get_Height();
        if (clip) {
            Rect rect = *clip;
            Unknown_006F3650(surface, rect, true);
            return;
        }
        Unknown_006F3650(surface, Rect(0, 0, width, height), true);
    }
}


// Rect MSFrameAnim::Get_Rect() (0x007292B0, slot 6): the whole 640x480 screen.

Rect MSFrameAnim::Get_Rect()
{
    return Rect(0, 0, 640, 480);
}


// rc_ptr<Map> __fastcall WorldDominationTour::Map::Create(int, int, int) (0x007292E0)
// A new map, returned counted.

namespace WorldDominationTour {

rc_ptr<Map> __fastcall Map::Create(int unknown_08, int unknown_0c, int unknown_10)
{
    return rc_ptr<Map>(new Map(unknown_08, unknown_0c, unknown_10));
}

}


extern "C" unsigned char vtable_72ba00[]; extern "C" void __cdecl release_buffer_72ba00(void*); extern "C" void __cdecl release_object_72ba00(void*);
class Leaf_72ba00 { public: void* get(unsigned int flags); };
void* Leaf_72ba00::get(unsigned int flags) { unsigned int pointer = *(unsigned int*)((char*)this + 4); *(unsigned int*)this=(unsigned int)vtable_72ba00; if(pointer && *(unsigned char*)((char*)this + 13)){ release_buffer_72ba00((void*)pointer); *(unsigned int*)((char*)this + 4)=0; } *(unsigned char*)((char*)this + 13)=0; *(unsigned int*)((char*)this + 8)=0; if(flags & 1) release_object_72ba00(this); return this; }


extern "C" unsigned char vtable_72ba50[]; extern "C" void __cdecl release_buffer_72ba50(void*); extern "C" void __cdecl release_object_72ba50(void*);
class Leaf_72ba50 { public: void* get(unsigned int flags); };
void* Leaf_72ba50::get(unsigned int flags) { unsigned int pointer = *(unsigned int*)((char*)this + 4); *(unsigned int*)this=(unsigned int)vtable_72ba50; if(pointer && *(unsigned char*)((char*)this + 13)){ release_buffer_72ba50((void*)pointer); *(unsigned int*)((char*)this + 4)=0; } *(unsigned char*)((char*)this + 13)=0; *(unsigned int*)((char*)this + 8)=0; if(flags & 1) release_object_72ba50(this); return this; }


extern "C" unsigned char vtable_72baa0[]; extern "C" void __cdecl release_buffer_72baa0(void*); extern "C" void __cdecl release_object_72baa0(void*);
class Leaf_72baa0 { public: void* get(unsigned int flags); };
void* Leaf_72baa0::get(unsigned int flags) { unsigned int pointer = *(unsigned int*)((char*)this + 4); *(unsigned int*)this=(unsigned int)vtable_72baa0; if(pointer && *(unsigned char*)((char*)this + 13)){ release_buffer_72baa0((void*)pointer); *(unsigned int*)((char*)this + 4)=0; } *(unsigned char*)((char*)this + 13)=0; *(unsigned int*)((char*)this + 8)=0; if(flags & 1) release_object_72baa0(this); return this; }


class Call_72baf0_0 {public: unsigned int target();};
extern "C" unsigned char data_72baf0_7af27c[];
class Leaf_72baf0 { public: unsigned int get(); };
unsigned int Leaf_72baf0::get() { unsigned int r0 = ((Call_72baf0_0*)((unsigned int)this))->target(); *(unsigned int*)(((unsigned int)this)) = (unsigned int)data_72baf0_7af27c; return (unsigned int)this; }


// WorldDominationTour::Map::Map(int, int, int) (0x00729330): every member cleared, the name
// from the shared string at 0x0083C55C and both lists empty. The unit emits the VectorClass<rc_ptr<Territory>>
// constructor 0x0072BB10 (called here out of line, see shared/wdt_map.hpp).

namespace WorldDominationTour {

Map::Map(int unknown_08, int unknown_0c, int unknown_10) :
    unknown_08(unknown_08),
    unknown_0c(unknown_0c),
    unknown_10(unknown_10),
    unknown_14(0),
    unknown_18(0),
    unknown_1c(0),
    unknown_20(0),
    unknown_24(0),
    unknown_28(unknown_string_83c55c),
    Territories(0, 0),
    unknown_44(0, 0),
    unknown_78(0),
    Palettes(0, 0)
{
}

}


// WorldDominationTour::Map::~Map() (0x007293F0): deletes the index surface, both drawers and
// the palettes, clears the two work surfaces and copies the cleared one to the screen.

namespace WorldDominationTour {

Map::~Map()
{
    delete unknown_24;
    delete unknown_14;
    for (int i = 0; i < Palettes.Count(); i++) {
        delete Palettes[i];
    }
    unknown_surface_839958->Fill(0);
    unknown_surface_839954->Fill(0);
    unknown_surface_839950->Copy_From(*unknown_surface_839954, false, true);
    delete unknown_78;
}

}


// Loads the World Domination Tour map layout and animation set (0x00729510).

namespace WorldDominationTour {
void Map::Load(INIClass &ini, MSEngine *engine, int unknown3)
{
    Wstring map_section;
    ini.Get_String("MAPS", Wstring_From_Integer(unknown_08), unknown_string_83c55c, map_section);
    Wstring faction_section;
    ini.Get_String(map_section, Faction_Name(unknown_10), unknown_string_83c55c, faction_section);
    Wstring art;
    bool found = ini.Get_String(faction_section, "Art_" + Wstring_From_Integer(unknown_8523e0), unknown_string_83c55c, art) > 0;
    if (unknown_8523e0 > 800 && !found) {
        ini.Get_String(faction_section, "Art_800", unknown_string_83c55c, art);
    }
    Point2D size(640, 400);
    size = ini.Get_Point(art, "Size", size);
    Point2D sidebar_size(0, 0);
    sidebar_size = ini.Get_Point(art, "SideBarSize", sidebar_size);
    Point2D helpbar_size(0, 0);
    helpbar_size = ini.Get_Point(art, "HelpBarSize", helpbar_size);
    unknown_44 = Point2D((unknown_surface_839954->Get_Width() - sidebar_size.X - size.X) / 2,
                         (unknown_surface_839954->Get_Height() - size.Y - helpbar_size.Y) / 2);
    unknown_4c = Point2D(unknown_44.X, unknown_44.Y + size.Y);
    (INIRect &)DescriptionRect = ini.Get_Rect(art, "TextRect", (INIRect &)DescriptionRect);
    DescriptionRect += unknown_44;
    (INIRect &)TooltipRect = ini.Get_Rect(art, "TooltipRect", (INIRect &)TooltipRect);
    TooltipRect += unknown_44;
    (INIRect &)PlayRect = ini.Get_Rect(art, "TitleRect", (INIRect &)PlayRect);
    PlayRect += unknown_44;
    (INIRect &)BackRect = ini.Get_Rect(art, "BackTextRect", (INIRect &)BackRect);
    BackRect += unknown_44;
    Wstring value;
    ini.Get_String(art, "Opening", unknown_string_83c55c, value);
    if (value.Length() > 0) {
        Rect rectangle(unknown_44, size.X, size.Y);
        unknown_18 = new MSBinkAnim(value, unknown_surface_839958, &engine->Anims, rectangle, false);
        engine->Add_Anim(unknown_18);
    }
    ini.Get_String(art, "Background", unknown_string_83c55c, value);
    unknown_1c = new MSPCXAnim(value, &engine->Anims, unknown_44, true);
    if (unknown_18 && unknown_1c) unknown_1c->Set_Unknown_0C(false);
    engine->Add_Anim(unknown_1c);
    ini.Get_String(art, "ClickMap", 0, value);
    CCFileClass file(value);
    unknown_24 = unknown_load_pcx_60d110(&file, 0, 0, 0);
    file.Close();
    {
        MSAnim *sidebar = 0;
        ini.Get_String(art, "SideBar", unknown_string_83c55c, value);
        if (value.Length() > 0) {
            int const &width = size.X;
            Point2D position(width, (unknown_surface_839954->Get_Height() - sidebar_size.Y) / 2);
            position.X += unknown_44.X;
            sidebar = new MSPCXAnim(value, &engine->Anims, position, true);
        }
        engine->Add_Anim(sidebar);
    }
    ini.Get_String(art, "HelpBar", unknown_string_83c55c, value);
    if (value.Length() > 0) unknown_20 = new MSPCXAnim(value, &engine->Anims, unknown_4c, true);
    engine->Add_Anim(unknown_20);
    (INIRect &)unknown_54 = ini.Get_Rect(art, "BackButtonRectangle", (INIRect &)unknown_54);
    Point2D back_origin = ini.Get_Point(art, "BackButtonOrigin", Point2D(unknown_54.Top_Left()));
    unknown_54 += unknown_44;
    back_origin += unknown_44;
    ini.Get_String(art, "BackButtonPalette", "sdbtnanm.pal", value);
    delete unknown_78;
    unknown_78 = unknown_load_drawer_5ac560(value);
    unknown_6c.unknown_0 = ini.Get_Int(art, "BackButtonNormalFrame", unknown_6c.unknown_0);
    unknown_6c.unknown_4 = ini.Get_Int(art, "BackButtonDepressedFrame", unknown_6c.unknown_4);
    ini.Get_String(art, "BackButton", unknown_string_83c55c, value);
    if (value.Length() > 0) {
        unknown_64.unknown_0 = new MSOverlayAnim(value, back_origin.X, back_origin.Y, unknown_78, 0,
                                               &engine->Anims, true, unknown_6c.unknown_0, false);
        engine->Add_Anim(unknown_64.unknown_0);
    }
    ini.Get_String(art, "BackButtonHighlighted", unknown_string_83c55c, value);
    ini.Get_String(art, "OverlayPrefix", 0, unknown_28);
    ini.Get_String(art, "OverlayPalette", 0, value);
    unknown_14 = unknown_load_drawer_5ac560(value);
    Wstring name_prefix = Wstring("WDT:") + map_section;
    ini.Get_String(faction_section, "NameKeyPrefix", name_prefix, name_prefix);
    {
        ConvertClass *drawer = 0;
        if (ini.Get_String(art, "DefaultPalette", 0, value) > 0) drawer = Get_Drawer(value);
        MSAnim *animation = Load_Anim(ini, art, "Logo", drawer);
        if (animation) engine->Add_Anim(animation);
        for (int index = 0; index < 10; ++index) {
            value = "Animation" + Wstring_From_Integer(index);
            MSAnim *anim = Load_Anim(ini, art, value, drawer);
            if (anim) engine->Add_Anim(anim);
        }
    }
    for (int index = 0; index < 100; ++index) {
        if (ini.Get_String(faction_section, "Territory" + Wstring_From_Integer(index, 2), 0, value) > 0) {
            rc_ptr<Territory> territory = Territory::Create(index, ini, value, unknown_28, name_prefix);
            Territories.Add(territory);
        }
    }
    for (int owner_index = 0; owner_index < Territories.Count(); ++owner_index) {
        Territories[owner_index]->Create_Owner_Anim(engine, unknown_14, unknown_44);
    }
    for (int highlight_index = 0; highlight_index < Territories.Count(); ++highlight_index) {
        Territories[highlight_index]->Create_Highlight_Anim(engine, unknown_14, unknown_44);
    }
}
}


// MSAnim *WorldDominationTour::Map::Load_Anim(INIClass &ini, Wstring const &section, Wstring const &entry,
// ConvertClass *drawer) (0x0072A280): the shape animation an INI entry "file,x,y[,rate[,palette]]"
// describes, placed relative to unknown_44 (the map origin); rate 5 and `drawer` when the entry
// leaves them out or the palette cannot be loaded (Get_Drawer). 0 when the entry is missing or has
// no y, the file is not available or there is no drawer.

namespace WorldDominationTour {

MSAnim *Map::Load_Anim(INIClass &ini, Wstring const &section, Wstring const &entry, ConvertClass *drawer)
{
    Wstring value;
    if (ini.Get_String(section, entry, 0, value) > 0) {
        Wstring filename;
        Wstring x;
        Wstring y;
        Wstring rate;
        Wstring palette;
        int position = value.Get_Token(0, ",", filename);
        position = value.Get_Token(position, ",", x);
        position = value.Get_Token(position, ",", y);
        position = value.Get_Token(position, ",", rate);
        value.Get_Token(position, ",", palette);
        if (y != 0) {
            int xpos = x.As_Integer() + unknown_44.X;
            int ypos = y.As_Integer() + unknown_44.Y;
            int frame_rate = 5;
            if (rate != 0 && rate.Length() > 0) {
                frame_rate = rate.As_Integer();
            }
            ConvertClass *anim_drawer = 0;
            if (palette != 0 && palette.Length() > 0) {
                anim_drawer = Get_Drawer(palette);
            }
            if (anim_drawer == 0) {
                anim_drawer = drawer;
            }
            bool available = CCFileClass(filename).Is_Available();
            if (available && anim_drawer != 0) {
                return new MSShapeAnim(filename, xpos, ypos, anim_drawer, frame_rate, true, 0);
            }
        }
    }
    return 0;
}

}


// ConvertClass *WorldDominationTour::Map::Get_Drawer(Wstring const &name) (0x0072A510): the
// drawer of the palette file name, loaded (0x005AC560) and kept in Palettes on first use.
// Out of line at both of its call sites in the unit (Load_Anim 0x0072A280 and the loader 0x00729510).

namespace WorldDominationTour {

#pragma auto_inline(off)
ConvertClass *Map::Get_Drawer(Wstring const &name)
{
    for (int i = 0; i < Palettes.Count(); i++) {
        if (Palettes[i]->Name == name) {
            return Palettes[i]->Drawer;
        }
    }
    ConvertClass *drawer = unknown_load_drawer_5ac560(name);
    if (drawer) {
        Palettes.Add(new AnimationPalette(name, drawer));
    }
    return drawer;
}
#pragma auto_inline(on)

}


// UnknownNamedValue_0072a5e0::UnknownNamedValue_0072a5e0(Wstring const &name, int value) -- 0x0072A5E0
// Purpose: copy-constructs the Wstring name (+0x00) and stores the value
// (+0x04). In the world-domination map area after MSFrameAnim; the owning
// type is not identified.

struct UnknownNamedValue_0072a5e0 {
    UnknownNamedValue_0072a5e0(Wstring const &name, int value);

    Wstring Name;    // +0x00
    int Value;       // +0x04
};

UnknownNamedValue_0072a5e0::UnknownNamedValue_0072a5e0(Wstring const &name, int value) : Name(name), Value(value)
{
}


// WorldDominationTour::Map::AnimationPalette::~AnimationPalette() (0x0072A600): deletes the drawer.
// Expanded in Map::~Map; this is the unit's out-of-line copy.

namespace WorldDominationTour {

Map::AnimationPalette::~AnimationPalette()
{
    delete Drawer;
}

}


// void WorldDominationTour::Map::unknown_72a620(MSEngine *engine, Voices *voices) (0x0072A620)
// With the +0x18 animation: says voice list 0 and waits for that animation (at most 300000 ms);
// then shows the +0x1C animation.

namespace WorldDominationTour {

void Map::unknown_72a620(MSEngine *engine, Voices *voices)
{
    if (unknown_18) {
        voices->Say_List(0, true, 0);
        engine->Wait_For(unknown_18, 300000);
    }
    if (unknown_1c) {
        unknown_1c->Set_Unknown_0C(true);
    }
}

}


// int WorldDominationTour::Map::Territory_At(Point2D const &point) const (0x0072A660): the
// territory index the index surface holds under point (relative to the map origin unknown_44);
// -1 outside the surface.

namespace WorldDominationTour {

int Map::Territory_At(Point2D const &point) const
{
    Point2D local = point;
    local.X -= unknown_44.X;
    local.Y -= unknown_44.Y;
    if (local.X >= 0 && local.X < unknown_24->Get_Width() && local.Y >= 0 && local.Y < unknown_24->Get_Height()) {
        return unknown_24->Get_Pixel(local);
    }
    return -1;
}

}


// WorldDominationTour::Centroid0072A7B0: empty coordinate sums and count.
namespace WorldDominationTour {
Centroid::Centroid() : unknown_00(0), unknown_04(0), unknown_08(0) {}
}


// 0x0072a770: function not yet identified
//
// Body: 59 bytes (frozen inventory bound).
// Referenced from 3 sites (unconditional call).
// Ghidra prototype: undefined4 __thiscall Is_Inside_0072a770(int param_1,int *param_2)
//
// Types and names are Ghidra placeholders;
// rename and retype as the purpose becomes known.
//
// Adjusted from the Ghidra seed by permute_seeds.py (signedness, comparison order) to
// match the original; edit freely, this file is no longer regenerated.

namespace seed_0072a770 {

struct Owner_0072a770 { unsigned int Is_Inside_0072a770(int *point); };

unsigned int Owner_0072a770::Is_Inside_0072a770(int *point)
{
    int self = (int)this;
    if ((*point >= *(int *)(self + 0x54)) &&
          (*point < *(int *)(self + 0x5c) + *(int *)(self + 0x54))) {
        if ((point[1] >= *(int *)(self + 0x58)) &&
              (point[1] < *(int *)(self + 0x60) + *(int *)(self + 0x58))) {
            return 1;
        }
    }
    return 0;
}

}  // namespace seed_0072a770


// WorldDominationTour::Centroid &WorldDominationTour::Centroid::operator+=(Point2D const &point) (0x0072A7C0):
// adds the point to the coordinate sums and counts it.

namespace WorldDominationTour {

Centroid &Centroid::operator+=(Point2D const &point)
{
    unknown_00 += point.X;
    unknown_04 += point.Y;
    unknown_08++;
    return *this;
}

}


// Point2D WorldDominationTour::Centroid::Average() const (0x0072A7F0): the mean of the added points,
// (0, 0) when none was added.

namespace WorldDominationTour {

Point2D Centroid::Average() const
{
    return unknown_08 > 0 ? Point2D(unknown_00 / unknown_08, unknown_04 / unknown_08) : Point2D(0, 0);
}

}


// 0x0072a840: function not yet identified
//
// Body: 46 bytes (frozen inventory bound).
// Referenced from 0 sites.
// Ghidra prototype: undefined4 __thiscall unknown_equals_0072a840(int *param_1,int *param_2)
//
// Types and names are Ghidra placeholders;
// rename and retype as the purpose becomes known.
//

namespace seed_0072a840 {

struct Owner_0072a840 { unsigned int unknown_equals_0072a840(int *other); };

unsigned int Owner_0072a840::unknown_equals_0072a840(int *other)
{
    int * self = (int *)this;
    if (((*self == *other) && (self[1] == other[1])) && (self[2] == other[2])) {
        return 1;
    }
    return 0;
}

}  // namespace seed_0072a840


// 0x0072a870: function not yet identified
//
// Body: 13 bytes (frozen inventory bound).
// Referenced from 0 sites.
// Ghidra prototype: uint __fastcall unknown_clamp_0072a870(uint param_1)
//
// Types and names are Ghidra placeholders;
// rename and retype as the purpose becomes known.
//

namespace seed_0072a870 {

unsigned int __fastcall unknown_clamp_0072a870(unsigned int value)
{
    if (0x1d < value) {
        value = (unsigned int)(0xff);
    }
    return value;
}

}  // namespace seed_0072a870


// void unknown_write_wdtmap_ini_72a880(Wstring const &prefix, Wstring const &filename) (0x0072A880, __fastcall):
// scans a PCX territory map, collects each colour's bounding rectangle and centroid
// (colours 0-29), and writes the centroids as Target_<width> points of the sections
// <prefix><NN> into WDTMap.ini (next to the game and as ..\code\wdtmap.ini).
// A development tool; no caller is recovered.

using WorldDominationTour::Centroid;

// The INI point of a screen point (taken by value; the copy is the original's temporary).
static inline INIPoint2D To_Point(Point2D point)
{
    return INIPoint2D(point.X, point.Y);
}

void __fastcall unknown_write_wdtmap_ini_72a880(Wstring const &prefix, Wstring const &filename)
{
    CDFileClass file;
    file.Open("WDTMap.ini", FILE_RIGHTS_READ);
    INIClass ini;
    ini.Load(file, false);
    file.Close();

    CDFileClass pcx(filename);
    Surface *surface = unknown_load_pcx_60d110(&pcx, 0, 0, 0);
    int height = surface->Get_Height();
    int width = surface->Get_Width();

    VectorClass<TRect<int> > rects(30);
    VectorClass<Centroid> centroids(30);

    VectorCursor<TRect<int>, VectorClass<TRect<int> > > cursor(rects);
    for (; cursor; ++cursor) {
        (*cursor).Set(0, 0, 0, 0);
    }

    Point2D point;
    for (point.Y = 0; point.Y < height; point.Y++) {
        for (point.X = 0; point.X < width; point.X++) {
            unsigned index = surface->Get_Pixel(point);
            if (index <= 29 && index < 255) {
                if (centroids.Length() < index + 1) {
                    centroids.Resize(index + 1);
                }
                centroids[index] += point;
                if (rects.Length() < index + 1) {
                    int count = rects.Length();
                    rects.Resize(index + 1);
                    VectorCursor<TRect<int>, VectorClass<TRect<int> > > fresh(rects, count);
                    for (; fresh; ++fresh) {
                        (*fresh).Set(0, 0, 0, 0);
                    }
                }
                TRect<int> &rect = rects[index];
                if (rect.Width == 0) {
                    rect.Height = 0;
                }
                if (!rect.Is_Point_Within(point)) {
                    rect = Union(rect, TRect<int>(point, 1, 1));
                }
            }
        }
    }

    {
        VectorCursor<TRect<int>, VectorClass<TRect<int> > > reset(rects, 0);
        cursor = reset;
    }
    int i = 0;
    while (cursor) {
        Wstring section = Wstring_Join(prefix, Wstring_From_Integer(i, 2));
        Wstring number = Wstring_From_Integer(unknown_8523e0);
        ini.Put_Point(section, Wstring("Target_") + number, To_Point(centroids[i].Average()));
        ++cursor;
        i++;
    }

    file.Open("WDTMap.ini", FILE_RIGHTS_WRITE);
    ini.Save(file);
    file.Open("..\\code\\wdtmap.ini", FILE_RIGHTS_WRITE);
    ini.Save(file);
}


// 0x0072af00: function not yet identified
//
// Owner: MSFrameAnim, virtual slot 0 (vtable +0x0) (from RTTI/vtables).
// Body: 36 bytes (frozen inventory bound).
// Referenced from 1 site (data).
// Ghidra prototype: undefined4 * __thiscall scalar_deleting_destructor_0072af00(undefined4 *param_1,byte param_2)
// Callees with accepted source:
//   MSAnim_dtor_005ac690 -> src/recovered/classes/MSAnim/005ac690.cpp
//   operator_delete_00781d3b -> src/recovered/00781d3b.cpp
//
// Types and names are Ghidra placeholders;
// rename and retype as the purpose becomes known.
//

namespace seed_0072af00 {

extern void __fastcall MSAnim_dtor_005ac690(void);
extern void  operator_delete_00781d3b(unsigned int);
extern "C" void* MSFrameAnim_vtable_007af1bc;

struct Owner_0072af00 { unsigned int * scalar_deleting_destructor_0072af00(unsigned char flags); };

unsigned int * Owner_0072af00::scalar_deleting_destructor_0072af00(unsigned char flags)
{
    unsigned int * self = (unsigned int *)this;
    *self = (unsigned int)(&MSFrameAnim_vtable_007af1bc);
    MSAnim_dtor_005ac690();
    if ((flags & 1) != 0) {
        operator_delete_00781d3b((unsigned int)(self));
    }
    return self;
}

}  // namespace seed_0072af00


// 0x0072af30: function not yet identified
//
// Owner: WorldDominationTour::Map, virtual slot 0 (vtable +0x0) (from RTTI/vtables).
// Body: 30 bytes (frozen inventory bound).
// Referenced from 1 site (data).
// Ghidra prototype: undefined4 __thiscall scalar_deleting_destructor_0072af30(undefined4 param_1,byte param_2)
// Callees with accepted source:
//   operator_delete_00781d3b -> src/recovered/00781d3b.cpp
//
// Types and names are Ghidra placeholders;
// rename and retype as the purpose becomes known.
//

namespace seed_0072af30 {

extern void __fastcall Map_dtor_007293f0(void);
extern void  operator_delete_00781d3b(unsigned int);

struct Owner_0072af30 { unsigned int scalar_deleting_destructor_0072af30(unsigned char flags); };

unsigned int Owner_0072af30::scalar_deleting_destructor_0072af30(unsigned char flags)
{
    unsigned int self = (unsigned int)this;
    Map_dtor_007293f0();
    if ((flags & 1) != 0) {
        operator_delete_00781d3b((unsigned int)(self));
    }
    return self;
}

}  // namespace seed_0072af30


// 0x0072af70: function not yet identified
//
// Body: 3 bytes (frozen inventory bound).
// Referenced from 0 sites.
// Ghidra prototype: undefined4 __fastcall unknown_identity_0072af70(undefined4 param_1)
//
// Types and names are Ghidra placeholders;
// rename and retype as the purpose becomes known.
//

namespace seed_0072af70 {

unsigned int __fastcall unknown_identity_0072af70(unsigned int value)
{
    return value;
}

}  // namespace seed_0072af70


// 0x0072b0b0: function not yet identified
//
// Owner: VectorCursor<TRect<int>, VectorClass<TRect<int>>>, virtual slot 1 (vtable +0x4) (from RTTI/vtables).
// Body: 14 bytes (frozen inventory bound).
// Referenced from 0 sites.
// Ghidra prototype: bool __fastcall unknown_in_range_0072b0b0(int param_1)
//
// Types and names are Ghidra placeholders;
// rename and retype as the purpose becomes known.
//

namespace seed_0072b0b0 {

bool __fastcall unknown_in_range_0072b0b0(int self)
{
    return *(unsigned int *)(self + 8) < *(unsigned int *)(*(int *)(self + 4) + 8);
}

}  // namespace seed_0072b0b0


// 0x0072b0c0: function not yet identified
//
// Body: 36 bytes (frozen inventory bound).
// Referenced from 1 site (data).
// Ghidra prototype: undefined4 * __thiscall scalar_deleting_destructor_0072b0c0(undefined4 *param_1,byte param_2)
// Callees with accepted source:
//   operator_delete_00781d3b -> src/recovered/00781d3b.cpp
//
// Types and names are Ghidra placeholders;
// rename and retype as the purpose becomes known.
//

namespace seed_0072b0c0 {

extern void __fastcall rc_ptr_base_dtor_00639510(void);
extern void  operator_delete_00781d3b(unsigned int);
extern "C" void* rc_ptr_vtable_007af1e4;

struct Owner_0072b0c0 { unsigned int * scalar_deleting_destructor_0072b0c0(unsigned char flags); };

unsigned int * Owner_0072b0c0::scalar_deleting_destructor_0072b0c0(unsigned char flags)
{
    unsigned int * self = (unsigned int *)this;
    *self = (unsigned int)(&rc_ptr_vtable_007af1e4);
    rc_ptr_base_dtor_00639510();
    if ((flags & 1) != 0) {
        operator_delete_00781d3b((unsigned int)(self));
    }
    return self;
}

}  // namespace seed_0072b0c0


// 0x0072b0f0: function not yet identified
//
// Owner: VectorCursor<TRect<int>, VectorClass<TRect<int>>>, virtual slot 0 (vtable +0x0) (from RTTI/vtables).
// Body: 32 bytes (frozen inventory bound).
// Referenced from 1 site (data).
// Ghidra prototype: undefined4 * __thiscall scalar_deleting_destructor_0072b0f0(undefined4 *param_1,byte param_2)
// Callees with accepted source:
//   operator_delete_00781d3b -> src/recovered/00781d3b.cpp
//
// Types and names are Ghidra placeholders;
// rename and retype as the purpose becomes known.
//

namespace seed_0072b0f0 {

extern void  operator_delete_00781d3b(unsigned int);
extern "C" void* VectorCursor_vtable_007af284;

struct Owner_0072b0f0 { unsigned int * scalar_deleting_destructor_0072b0f0(unsigned char flags); };

unsigned int * Owner_0072b0f0::scalar_deleting_destructor_0072b0f0(unsigned char flags)
{
    unsigned int * self = (unsigned int *)this;
    *self = (unsigned int)(&VectorCursor_vtable_007af284);
    if ((flags & 1) != 0) {
        operator_delete_00781d3b((unsigned int)(self));
    }
    return self;
}

}  // namespace seed_0072b0f0


// Container members this unit emits by its own use of its lists (bodies: shared/vector_class.hpp):
// VectorClass<rc_ptr<WorldDominationTour::Territory>>::Resize (0x0072B160).
// VectorClass<rc_ptr<WorldDominationTour::Territory>>::Clear (0x0072B280).
// VectorClass<WorldDominationTour::Map::AnimationPalette *>::Resize (0x0072B4A0; VectorClass<WorldDominationTour::Map::AnimationPalette *> vtable 0x007af25c slot 2).
// VectorClass<WorldDominationTour::Map::AnimationPalette *>::Clear (0x0072B550; VectorClass<WorldDominationTour::Map::AnimationPalette *> vtable 0x007af25c slot 3).
// VectorClass<WorldDominationTour::Centroid>::Resize (0x0072B720).
// VectorClass<WorldDominationTour::Centroid>::Clear (0x0072B820; VectorClass<WorldDominationTour::Centroid> vtable 0x007af298 slot 3).
// The element type is left incomplete: Clear does not depend on it.


class Leaf_72b8d0 { public: unsigned int get(); };
unsigned int Leaf_72b8d0::get() { unsigned int self = (unsigned int)this; unsigned int cursor; unsigned int spare; cursor = self; *(unsigned int*)(cursor + 8) = (*(unsigned int*)(cursor + 8)) + 1; return cursor; }


class Leaf_72b8e0 { public: unsigned int get(); };
unsigned int Leaf_72b8e0::get() { unsigned int self = (unsigned int)this; unsigned int cursor; unsigned int spare; cursor = self; *(unsigned int*)(cursor + 8) = (*(unsigned int*)(cursor + 8)) - 1; return cursor; }


// VectorClass<rc_ptr<WorldDominationTour::Territory> >::`scalar deleting destructor' (0x0072b8f0, vtable slot 0):
// compiler-generated, emitted by the Map constructor's unit (WorldDominationTour_Map_constructor_00729330.cpp);
// no source of its own.


// rc_ptr<WorldDominationTour::Territory>::`vector deleting destructor' (0x0072B940, rc_ptr<Territory>
// vtable 0x007AF27C slot 0): compiler-generated, emitted by the Map constructor's unit
// (WorldDominationTour_Map_constructor_00729330.cpp: the VectorClass<rc_ptr<Territory>> constructor's
// new[]); no source of its own.


// DynamicVectorClass<rc_ptr<WorldDominationTour::Territory> >::`scalar deleting destructor' (0x0072b9b0, vtable slot 0):
// compiler-generated, emitted by the Map constructor's unit (WorldDominationTour_Map_constructor_00729330.cpp);
// no source of its own.


// VectorClass<rc_ptr<WorldDominationTour::Territory> >::VectorClass(unsigned, T const *)
// (0x0072BB10): emitted by the Map constructor's unit, WorldDominationTour_Map_constructor_00729330.cpp
// (shared/vector_class.hpp, VECTOR_IMPLEMENT_DYNAMIC_CONSTRUCTOR); no source of its own.
