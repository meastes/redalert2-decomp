// wdtconflict.cpp: the original translation unit re-filed as one source (Route A): the accepted
// bodies of its canonical compile group in address order, under the unit's own header
// state (its switches and includes in their original order). The module map
// manifests/original_target/modules/wdtconflict.json carries the addresses and the rows that
// stay in their own files.


#define RA2_VECTOR_UNSIGNED_INDEX  // migration holdout: unit not yet re-rolled to VectorClass::operator[](int) (docs/HANDOFF.md)
#include "shared/shape_file.hpp"
#include "shared/unknown_geometry.hpp"
#define VECTOR_IMPLEMENT_ALL
#include "shared/wdt_game_options.hpp"
#include "shared/world_domination_tour.hpp"


// Header-static dynamic initializers of the unit at 0x00727100: the shape_file.hpp empty rect and the
// shared/unknown_geometry.hpp constants (0x00727100-0x007272E1); the other
// drafts of this plan are emitted by these includes.


// Dynamic initializer (0x00727120, _$E) of diagonal from the header(s) included
// by HeaderStatics_empty_rect_init_00727100.cpp; no source of its own.


// Dynamic initializer (0x00727150, _$E) of b from the header(s) included
// by HeaderStatics_empty_rect_init_00727100.cpp; no source of its own.


// Dynamic initializer (0x00727170, _$E) of c from the header(s) included
// by HeaderStatics_empty_rect_init_00727100.cpp; no source of its own.


// Dynamic initializer (0x00727190, _$E) of angle45 from the header(s) included
// by HeaderStatics_empty_rect_init_00727100.cpp; no source of its own.


// Dynamic initializer (0x007271B0, _$E) of angle60 from the header(s) included
// by HeaderStatics_empty_rect_init_00727100.cpp; no source of its own.


// Dynamic initializer (0x007271D0, _$E) of angle90 from the header(s) included
// by HeaderStatics_empty_rect_init_00727100.cpp; no source of its own.


// Dynamic initializer (0x007271F0, _$E) of g from the header(s) included
// by HeaderStatics_empty_rect_init_00727100.cpp; no source of its own.


// Dynamic initializer (0x00727230, _$E) of h from the header(s) included
// by HeaderStatics_empty_rect_init_00727100.cpp; no source of its own.


// Dynamic initializer (0x00727260, _$E) of i from the header(s) included
// by HeaderStatics_empty_rect_init_00727100.cpp; no source of its own.


// Dynamic initializer (0x00727290, _$E) of j from the header(s) included
// by HeaderStatics_empty_rect_init_00727100.cpp; no source of its own.


// Dynamic initializer (0x007272C0, _$E) of k from the header(s) included
// by HeaderStatics_empty_rect_init_00727100.cpp; no source of its own.


// Dynamic initializer (0x007272D0, _$E) of l from the header(s) included
// by HeaderStatics_empty_rect_init_00727100.cpp; no source of its own.


// Conflict.cpp (unit 0x00727100-0x0072847F): one battle of the World
// Domination Tour campaign and the shared list of battle options.

// The shared list of battle options (0x00B256C0), filled by Init_Options.
DynamicVectorClass<rc_ptr<WorldDominationTour::GameOption> > unknown_game_options_b256c0;

namespace WorldDominationTour {

rc_ptr<Conflict> __fastcall Conflict::Create(rc_ptr<WDTTerritory> const &territory)
{
    return rc_ptr<Conflict>(new Conflict(territory));
}


Conflict::Conflict(rc_ptr<WDTTerritory> const &territory) :
    Territory(territory)
{
}


bool Conflict::operator==(Conflict const &other) const
{
    return Territory == other.Territory;
}


Conflict::~Conflict()
{
}


bool Conflict::Is_Valid() const
{
    return Territory.Get() != 0;
}


int Conflict::unknown_7273f0() const
{
    if (Territory.Get()) {
        return Territory->unknown_08;
    }
    return 0;
}

}


// Dynamic initializer of unknown_game_options_b256c0 (0x00727400, _$E): constructs the
// option list and registers its destructor; emitted by the definition in
// WorldDominationTour_Conflict_Create_007272f0.cpp, no source of its own.


// The atexit destructor of unknown_game_options_b256c0 (0x00727440, _$E): emitted by the
// definition in WorldDominationTour_Conflict_Create_007272f0.cpp, no source of its own.


// Conflict::Init_Options (0x00727480): builds the shared list of battle options.

int __fastcall WDTTerritory_Get_Unit_Count(WDTTerritory const *territory);
int __fastcall WDTTerritory_Get_Tech_Level(WDTTerritory const *territory);

namespace WorldDominationTour {

void __fastcall Conflict::Init_Options()
{
    Clear_Options();
    unknown_game_options_b256c0.Add(rc_ptr<GameOption>(ValueGameOption<unsigned char>::Create(0x10000, WDTTerritory_Get_Tech_Level, Wstring("WDT:TechLevel"), 0)));
    unknown_game_options_b256c0.Add(rc_ptr<GameOption>(FlagGameOption::Create(0x10, Wstring("WDT:NoBases"), 0, Wstring("WDT:Bases"), 1)));
    unknown_game_options_b256c0.Add(rc_ptr<GameOption>(FlagGameOption::Create(0x400, Wstring("WDT:NotShortGame"), 1, Wstring("WDT:ShortGame"), 0)));
    unknown_game_options_b256c0.Add(rc_ptr<GameOption>(ValueGameOption<unsigned char>::Create(0x8000, WDTTerritory_Get_Unit_Count, Wstring("WDT:UnitCount"), 0)));
    unknown_game_options_b256c0.Add(rc_ptr<GameOption>(FlagGameOption::Create(0x20, Wstring("WDT:NoRedeployMCV"), 1, Wstring("WDT:RedeployableMCV"), 0)));
    unknown_game_options_b256c0.Add(rc_ptr<GameOption>(FlagGameOption::Create(0x100, Wstring("WDT:NoCrates"), 1, Wstring("WDT:Crates"), 1)));
    unknown_game_options_b256c0.Add(MapSizeGameOption::Create(0xc000000));
}

}


// Conflict::Clear_Options (0x00727920): empties the shared battle option list.

namespace WorldDominationTour {

void Conflict::Clear_Options()
{
    unknown_game_options_b256c0.Clear();
}

}


namespace WorldDominationTour {

// The description of this battle's settings: up to six options, first those
// whose value for the territory is 0, then the rest that are not forced (2);
// an option's Flag bit keeps a second option of the same kind out.
WideString Conflict::Describe_Options() const
{
    if (unknown_game_options_b256c0.Count() == 0) {
        Init_Options();
    }
    int remaining = 6;
    WideString text;
    int used = 0;
    VectorCursor<rc_ptr<GameOption>, DynamicVectorClass<rc_ptr<GameOption> > > cursor(unknown_game_options_b256c0);
    for (; remaining != 0 && cursor; ++cursor) {
        rc_ptr<GameOption> option = *cursor;
        if (!(option->Flag & used) && option->Get_Value(Territory) == 0) {
            text = option->Describe(Territory, text);
            remaining--;
            used |= option->Flag;
        }
    }
    cursor.Index = 0;
    for (; remaining != 0 && cursor; ++cursor) {
        rc_ptr<GameOption> option = *cursor;
        if (!(option->Flag & used) && option->Get_Value(Territory) != 2) {
            text = option->Describe(Territory, text);
            remaining--;
            used |= option->Flag;
        }
    }
    return text;
}

}


// WorldDominationTour::Conflict scalar deleting destructor (0x00727B30): emitted with
// Conflict's vtable; no source of its own.


// int __fastcall (WDTTerritory const *) -- 0x00727B70
// The WDT:UnitCount getter ValueGameOption<unsigned char> reads (Conflict.cpp,
// passed to ValueGameOption::Create by Conflict::Init_Options 0x00727480).

int __fastcall WDTTerritory_Get_Unit_Count(WDTTerritory const *territory)
{
    return territory->UnitCount;
}


// int __fastcall (WDTTerritory const *) -- 0x00727B80
// The WDT:TechLevel getter ValueGameOption<unsigned char> reads (Conflict.cpp,
// passed to ValueGameOption::Create by Conflict::Init_Options 0x00727480).

int __fastcall WDTTerritory_Get_Tech_Level(WDTTerritory const *territory)
{
    return territory->TechLevel;
}


// Container members this unit emits by its own use of its lists (bodies: shared/vector_class.hpp):
// DynamicVectorClass<rc_ptr<WorldDominationTour::GameOption>>::Clear (0x00727B90).
// VectorClass<rc_ptr<WorldDominationTour::GameOption>>::Fetch (0x00727BE0).


// WorldDominationTour::ValueGameOption<unsigned char>::Create (0x00727C10, wdtgameoptions.h):
// emitted by Conflict::Init_Options' use of ValueGameOption<unsigned char>::Create; no source of its own.


// rc_ptr<WorldDominationTour::GameOption> destructor (0x00727C80): emitted by Conflict.cpp's use of the class; no source of its own.


// rc_ptr<WorldDominationTour::FlagGameOption> destructor (0x00727C90): emitted by Conflict.cpp's use of the class; no source of its own.


// rc_ptr<WorldDominationTour::MapSizeGameOption> destructor (0x00727CA0): emitted by Conflict.cpp's use of the class; no source of its own.


// VectorCursor<rc_ptr<GameOption>, DynamicVectorClass<rc_ptr<GameOption>>>::Valid (0x00727CB0): emitted by Conflict.cpp's use of the class; no source of its own.


// rc_ptr<WorldDominationTour::Conflict> scalar deleting destructor (this unit's unreferenced copy; the vtable 0x007AEFEC holds 0x00726F60) (0x00727CC0): emitted by Conflict.cpp's use of the class; no source of its own.


// rc_ptr<WorldDominationTour::ValueGameOption<unsigned char>> scalar deleting destructor (0x00727CF0): emitted by Conflict.cpp's use of the class; no source of its own.


// rc_ptr<WorldDominationTour::FlagGameOption> scalar deleting destructor (0x00727D20): emitted by Conflict.cpp's use of the class; no source of its own.


// rc_ptr<WorldDominationTour::MapSizeGameOption> scalar deleting destructor (0x00727D50): emitted by Conflict.cpp's use of the class; no source of its own.


// VectorCursor<rc_ptr<GameOption>, DynamicVectorClass<rc_ptr<GameOption>>> scalar deleting destructor (0x00727D80): emitted by Conflict.cpp's use of the class; no source of its own.


// WorldDominationTour::ValueGameOption<unsigned char>::constructor (0x00727DA0, wdtgameoptions.h):
// emitted by Conflict::Init_Options' use of ValueGameOption<unsigned char>::Create; no source of its own.


// WorldDominationTour::GameOption scalar deleting destructor (0x00727DF0): emitted with
// GameOption's vtable by the inlined GameOption constructor; no source of its own.


// WorldDominationTour::ValueGameOption<unsigned char>::Get_Value (0x00727E10, wdtgameoptions.h):
// emitted by Conflict::Init_Options' use of ValueGameOption<unsigned char>::Create; no source of its own.


// WorldDominationTour::ValueGameOption<unsigned char>::Describe (0x00727E30, wdtgameoptions.h):
// emitted by Conflict::Init_Options' use of ValueGameOption<unsigned char>::Create; no source of its own.


// WorldDominationTour::ValueGameOption<unsigned char>::scalar deleting destructor (0x00727F20, wdtgameoptions.h):
// emitted by Conflict::Init_Options' use of ValueGameOption<unsigned char>::Create; no source of its own.


// Container members this unit emits by its own use of its lists (bodies: shared/vector_class.hpp):
// VectorClass<rc_ptr<WorldDominationTour::GameOption>>::operator== (0x00727F50).
// VectorClass<rc_ptr<WorldDominationTour::GameOption>>::Resize (0x00727FA0).
// VectorClass<rc_ptr<WorldDominationTour::GameOption>>::Clear (0x007280C0).


// VectorClass<rc_ptr<GameOption>>::ID(T const &) (0x007280F0): emitted with the vtable of unknown_game_options_b256c0
// (defined in WorldDominationTour_Conflict_Create_007272f0.cpp); no source of its own.


// VectorClass<rc_ptr<GameOption>>::ID(T const *) (0x00728130): emitted with the vtable of unknown_game_options_b256c0
// (defined in WorldDominationTour_Conflict_Create_007272f0.cpp); no source of its own.


// Container members this unit emits by its own use of its lists (bodies: shared/vector_class.hpp):
// DynamicVectorClass<rc_ptr<WorldDominationTour::GameOption>>::Resize (0x00728150).


// DynamicVectorClass<rc_ptr<GameOption>>::Add (0x00728270): the out-of-line copy Conflict::Init_Options
// calls for its last two options; no source of its own.


// Container members this unit emits by its own use of its lists (bodies: shared/vector_class.hpp):
// DynamicVectorClass<rc_ptr<WorldDominationTour::GameOption>>::ID_object (0x007282D0).


// rc_ptr<WorldDominationTour::GameOption> constructor from another rc_ptr (0x00728300): emitted by Conflict.cpp's use of the class; no source of its own.


// VectorCursor<rc_ptr<GameOption>, DynamicVectorClass<rc_ptr<GameOption>>>::Next (0x00728330): emitted by Conflict.cpp's use of the class; no source of its own.


// VectorCursor<rc_ptr<GameOption>, DynamicVectorClass<rc_ptr<GameOption>>>::Prev (0x00728340): emitted by Conflict.cpp's use of the class; no source of its own.


// VectorClass<rc_ptr<GameOption>> scalar deleting destructor (0x00728350): emitted with the vtable of unknown_game_options_b256c0
// (defined in WorldDominationTour_Conflict_Create_007272f0.cpp); no source of its own.


// rc_ptr<WorldDominationTour::GameOption> vector deleting destructor (0x007283A0): emitted
// with the rc_ptr<GameOption> vtable; no source of its own.


// DynamicVectorClass<rc_ptr<GameOption>> scalar deleting destructor (0x00728410): emitted with the vtable of unknown_game_options_b256c0
// (defined in WorldDominationTour_Conflict_Create_007272f0.cpp); no source of its own.


// rc_ptr<GameOption> default constructor (0x00728460): emitted with the vtable of unknown_game_options_b256c0
// (defined in WorldDominationTour_Conflict_Create_007272f0.cpp); no source of its own.
