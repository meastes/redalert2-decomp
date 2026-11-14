// wdtterr.cpp: the original translation unit re-filed as one source (Route A): the accepted
// bodies of its canonical compile group in address order, under the unit's own header
// state (its switches and includes in their original order). The module map
// manifests/original_target/modules/wdtterr.json carries the addresses and the rows that
// stay in their own files.


#define VECTOR_IMPLEMENT_VECTOR_ACCESSORS
#define VECTOR_IMPLEMENT_DYNAMIC_ACCESSORS
#include "shared/shape_file.hpp"
#include "shared/unknown_geometry.hpp"
#include "shared/world_domination_tour.hpp"
#include "shared/string_table.hpp"
#include "shared/unknown_globals.hpp"
#include "shared/cc_file_class.hpp"
#include "shared/wdt_map.hpp"
#include "shared/vector_cursor.hpp"


// Header-static dynamic initializers of the unit at 0x00730990: the shape_file.hpp empty rect and the
// shared/unknown_geometry.hpp constants (0x00730990-0x00730B71); the other
// drafts of this plan are emitted by these includes. The unit expands the
// vector accessors inline (Find_Territory 0x007311B0).


// Dynamic initializer (0x007309B0, _$E) of diagonal from the header(s) included
// by HeaderStatics_empty_rect_init_00730990.cpp; no source of its own.


// Dynamic initializer (0x007309E0, _$E) of b from the header(s) included
// by HeaderStatics_empty_rect_init_00730990.cpp; no source of its own.


// Dynamic initializer (0x00730A00, _$E) of c from the header(s) included
// by HeaderStatics_empty_rect_init_00730990.cpp; no source of its own.


// Dynamic initializer (0x00730A20, _$E) of angle45 from the header(s) included
// by HeaderStatics_empty_rect_init_00730990.cpp; no source of its own.


// Dynamic initializer (0x00730A40, _$E) of angle60 from the header(s) included
// by HeaderStatics_empty_rect_init_00730990.cpp; no source of its own.


// Dynamic initializer (0x00730A60, _$E) of angle90 from the header(s) included
// by HeaderStatics_empty_rect_init_00730990.cpp; no source of its own.


// Dynamic initializer (0x00730A80, _$E) of g from the header(s) included
// by HeaderStatics_empty_rect_init_00730990.cpp; no source of its own.


// Dynamic initializer (0x00730AC0, _$E) of h from the header(s) included
// by HeaderStatics_empty_rect_init_00730990.cpp; no source of its own.


// Dynamic initializer (0x00730AF0, _$E) of i from the header(s) included
// by HeaderStatics_empty_rect_init_00730990.cpp; no source of its own.


// Dynamic initializer (0x00730B20, _$E) of j from the header(s) included
// by HeaderStatics_empty_rect_init_00730990.cpp; no source of its own.


// Dynamic initializer (0x00730B50, _$E) of k from the header(s) included
// by HeaderStatics_empty_rect_init_00730990.cpp; no source of its own.


// Dynamic initializer (0x00730B60, _$E) of l from the header(s) included
// by HeaderStatics_empty_rect_init_00730990.cpp; no source of its own.


// rc_ptr<Territory> __fastcall WorldDominationTour::Territory::Create(...) (0x00730B80)

namespace WorldDominationTour {

rc_ptr<Territory> __fastcall Territory::Create(int index, INIClass const &ini, Wstring const &section,
                                               Wstring const &shape_prefix, Wstring const &name_prefix)
{
    return rc_ptr<Territory>(new Territory(index, ini, section, shape_prefix, name_prefix));
}

}


// WorldDominationTour::Territory::Territory(...) (0x00730BE0, 541 bytes; the inventory bound of 234 stops at a Ghidra noreturn call)
// Name is the CSF text of <name_prefix><NN>, ShapeName <shape_prefix><NN>.shp, Target/Origin the INI points
// "Target_<width>"/"Origin_<width>" with the _800 keys as fallback on screens wider than 800.
namespace WorldDominationTour {

Territory::Territory(int index, INIClass const &ini, Wstring const &section, Wstring const &shape_prefix,
                     Wstring const &name_prefix) :
    Index(index),
    OwnerAnim(0),
    HighlightAnim(0),
    unknown_2c(0)
{
    ShapeName = Wstring_Join(shape_prefix, Wstring_From_Integer(index, 2)) + ".shp";
    Wstring label = Wstring_Join(name_prefix, Wstring_From_Integer(index, 2));
    Name = StringTable::Load_String(label.Get(), 0, "D:\\Ra2\\WDTTerr.cpp", 39);
    Wstring width = Wstring_From_Integer(unknown_8523e0);
    Target = ini.Get_Point(section, "Target_" + width, Target);
    Origin = ini.Get_Point(section, "Origin_" + width, Origin);
    if (unknown_8523e0 > 800) {
        if (Target.X == 0 && Target.Y == 0) {
            Wstring key("Target_800");
            Target = ini.Get_Point(section, key, Target);
        }
        if (Origin.X == 0 && Origin.Y == 0) {
            Wstring key("Origin_800");
            Origin = ini.Get_Point(section, key, Origin);
        }
    }
}
}


// void WorldDominationTour::Territory::Create_Owner_Anim(MSEngine *engine, ConvertClass *drawer, Point2D const &offset) (0x00730E00)
// When the territory's shape file exists, creates its overlay animation at
// Origin + offset (frame -1), resets its slot-1 flag and adds it to the engine.
// The overlay redraws the engine's other animations over each step.

namespace WorldDominationTour {

void Territory::Create_Owner_Anim(MSEngine *engine, ConvertClass *drawer, Point2D const &offset)
{
    int x = Origin.X + offset.X;
    int y = Origin.Y + offset.Y;
    CCFileClass file(ShapeName);
    if (file.Is_Available()) {
        OwnerAnim = new MSOverlayAnim(ShapeName, x, y, drawer, 0, &engine->Anims, true, -1, false);
        OwnerAnim->Set_Unknown_0C(false);
        engine->Add_Anim(OwnerAnim);
    }
}

}


// void WorldDominationTour::Territory::Create_Highlight_Anim(MSEngine *engine, ConvertClass *drawer, Point2D const &offset) (0x00730EC0)
// When the territory's shape file exists, creates its overlay animation at
// Origin + offset (frame 0), resets its slot-1 flag and adds it to the engine.
// The overlay redraws the engine's other animations over each step.

namespace WorldDominationTour {

void Territory::Create_Highlight_Anim(MSEngine *engine, ConvertClass *drawer, Point2D const &offset)
{
    int x = Origin.X + offset.X;
    int y = Origin.Y + offset.Y;
    CCFileClass file(ShapeName);
    if (file.Is_Available()) {
        HighlightAnim = new MSOverlayAnim(ShapeName, x, y, drawer, 0, &engine->Anims, true, 0, false);
        HighlightAnim->Set_Unknown_0C(false);
        engine->Add_Anim(HighlightAnim);
    }
}

}


// WorldDominationTour::Territory::~Territory() (0x00730F80)

namespace WorldDominationTour {

Territory::~Territory()
{
}

}


// void WorldDominationTour::Territory::Set_Owner_State(int state, MSEngine *engine, bool wait) (0x00730FB0)
// TibSun wdtterr.cpp shape: picks the owner overlay frame (state 1-3 -> 3-1),
// restarts or hides the overlay, redraws the union of the old and new
// overlay rectangles and runs the engine for one frame (wait is unused).

namespace WorldDominationTour {

void Territory::Set_Owner_State(int state, MSEngine *engine, bool wait)
{
    if (OwnerAnim != 0) {
        int frame = -1;
        switch (state) {
            case 1:
                frame = 3;
                break;
            case 2:
                frame = 2;
                break;
            case 3:
                frame = 1;
                break;
        }
        Rect oldrect = OwnerAnim->Get_Rect();
        if (frame >= 0) {
            OwnerAnim->Set_Frame(frame);
            OwnerAnim->Set_Unknown_0C(true);
            OwnerAnim->Restart();
        } else {
            OwnerAnim->Set_Unknown_0C(false);
        }
        Rect newrect = OwnerAnim->Get_Rect();
        Rect rect = Union(newrect, oldrect);
        engine->Broadcast((int)&rect);
        engine->Redraw();
        engine->Run(1);
    }
}

}


// void WorldDominationTour::Territory::Show_Highlight(bool arg, MSEngine *engine) (0x00731120)
// Passes arg to the highlight animation's slot 1, has the engine redraw its
// area and waits up to 300000 for the animation.

namespace WorldDominationTour {

void Territory::Show_Highlight(bool arg, MSEngine *engine)
{
    if (HighlightAnim) {
        HighlightAnim->Set_Unknown_0C(arg);
        Rect rect = HighlightAnim->Get_Rect();
        engine->Broadcast((int)&rect);
        engine->Redraw();
        engine->Wait_For(HighlightAnim, 300000);
    }
}

}


// void WorldDominationTour::Territory::Set_unknown_2c(int value) (0x00731190)

namespace WorldDominationTour {

void Territory::Set_unknown_2c(int value)
{
    unknown_2c = value;
}

}


// int WorldDominationTour::Territory::Take_unknown_2c() (0x007311A0): returns and clears +0x2C.

namespace WorldDominationTour {

int Territory::Take_unknown_2c()
{
    int value = unknown_2c;
    unknown_2c = 0;
    return value;
}

}


// rc_ptr<Territory> WorldDominationTour::Find_Territory(DynamicVectorClass<rc_ptr<Territory> > &territories, int index) (0x007311B0)
// Walks a cursor over territories to the first one whose Index is index;
// an empty pointer when none is. The unit's first source turns on the inline
// vector accessors.

namespace WorldDominationTour {

rc_ptr<Territory> __fastcall Find_Territory(DynamicVectorClass<rc_ptr<Territory> > &territories, int index)
{
    VectorCursor<rc_ptr<Territory>, DynamicVectorClass<rc_ptr<Territory> > > cursor(territories);
    while (cursor && (*cursor)->Index != index) {
        ++cursor;
    }
    return cursor ? *cursor : rc_ptr<Territory>();
}

}


// WorldDominationTour::Territory scalar deleting destructor (0x00731290):
// compiler generated with the unit's vtable; no source of its own.
