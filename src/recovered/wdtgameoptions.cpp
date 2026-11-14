// wdtgameoptions.cpp: the original translation unit re-filed as one source (Route A): the accepted
// bodies of its canonical compile group in address order, under the unit's own header
// state (its switches and includes in their original order). The module map
// manifests/original_target/modules/wdtgameoptions.json carries the addresses and the rows that
// stay in their own files.


#include "shared/shape_file.hpp"
#include "shared/unknown_geometry.hpp"
#include "shared/wdt_game_options.hpp"


// Header-static dynamic initializers of the unit at 0x00728480: the shape_file.hpp empty rect and the
// shared/unknown_geometry.hpp constants (0x00728480-0x00728661); the other
// drafts of this plan are emitted by these includes.


// Dynamic initializer (0x007284A0, _$E) of diagonal from the header(s) included
// by HeaderStatics_empty_rect_init_00728480.cpp; no source of its own.


// Dynamic initializer (0x007284D0, _$E) of b from the header(s) included
// by HeaderStatics_empty_rect_init_00728480.cpp; no source of its own.


// Dynamic initializer (0x007284F0, _$E) of c from the header(s) included
// by HeaderStatics_empty_rect_init_00728480.cpp; no source of its own.


// Dynamic initializer (0x00728510, _$E) of angle45 from the header(s) included
// by HeaderStatics_empty_rect_init_00728480.cpp; no source of its own.


// Dynamic initializer (0x00728530, _$E) of angle60 from the header(s) included
// by HeaderStatics_empty_rect_init_00728480.cpp; no source of its own.


// Dynamic initializer (0x00728550, _$E) of angle90 from the header(s) included
// by HeaderStatics_empty_rect_init_00728480.cpp; no source of its own.


// Dynamic initializer (0x00728570, _$E) of g from the header(s) included
// by HeaderStatics_empty_rect_init_00728480.cpp; no source of its own.


// Dynamic initializer (0x007285B0, _$E) of h from the header(s) included
// by HeaderStatics_empty_rect_init_00728480.cpp; no source of its own.


// Dynamic initializer (0x007285E0, _$E) of i from the header(s) included
// by HeaderStatics_empty_rect_init_00728480.cpp; no source of its own.


// Dynamic initializer (0x00728610, _$E) of j from the header(s) included
// by HeaderStatics_empty_rect_init_00728480.cpp; no source of its own.


// Dynamic initializer (0x00728640, _$E) of k from the header(s) included
// by HeaderStatics_empty_rect_init_00728480.cpp; no source of its own.


// Dynamic initializer (0x00728650, _$E) of l from the header(s) included
// by HeaderStatics_empty_rect_init_00728480.cpp; no source of its own.


// WorldDominationTour::GameOption::~GameOption() (0x00728670)

namespace WorldDominationTour {

GameOption::~GameOption()
{
}

}


// bool WorldDominationTour::GameOption::Is_Forced(rc_ptr<WDTTerritory> const &territory) const (0x00728680)
// Never forced here (ValueGameOption::Get_Value returns 2 when it is).

namespace WorldDominationTour {

bool GameOption::Is_Forced(rc_ptr<WDTTerritory> const &territory) const
{
    return false;
}

}


// WideString WorldDominationTour::RangedGameOption::Describe(rc_ptr<WDTTerritory> const &territory, WideString const &prefix) const (0x00728690)
// The CSF text of Label followed by the territory's range index, after
// prefix and ", " when prefix is not empty.

namespace WorldDominationTour {

WideString RangedGameOption::Describe(rc_ptr<WDTTerritory> const &territory, WideString const &prefix) const
{
    int range = Get_Range(territory);
    Wstring label = Wstring_Join(Label, Wstring_From_Integer(range));
    if (prefix.Length()) {
        WideString separator(L", ");
        return prefix + separator + StringTable::Load_String(label.Get(), 0, "D:\\Ra2\\WDTGameOptions.cpp", 22);
    }
    return WideString(StringTable::Load_String(label.Get(), 0, "D:\\Ra2\\WDTGameOptions.cpp", 25));
}

}


// int WorldDominationTour::RangedGameOption::In_Range_Of(unsigned value) const (0x00728770)
// TibSun shape: the range value falls into (4 is the middle).

namespace WorldDominationTour {

int RangedGameOption::In_Range_Of(unsigned value) const
{
    if (value <= Range0) {
        return 0;
    }
    if (value <= Range1) {
        return 1;
    }
    if (value >= Range3) {
        return 3;
    }
    if (value >= Range2) {
        return 2;
    }
    return 4;
}

}


// int WorldDominationTour::RangedGameOption::Get_Value(rc_ptr<WDTTerritory> const &territory) const (0x007287B0)
// 2 when the territory's value is in the middle range, else 0.

namespace WorldDominationTour {

int RangedGameOption::Get_Value(rc_ptr<WDTTerritory> const &territory) const
{
    if (Get_Range(territory) == 4) {
        return 2;
    }
    return 0;
}

}


// rc_ptr<FlagGameOption> __fastcall WorldDominationTour::FlagGameOption::Create(int flag, Wstring off_label, int off_value, Wstring on_label, int on_value) (0x007287D0)

namespace WorldDominationTour {

rc_ptr<FlagGameOption> __fastcall FlagGameOption::Create(int flag, Wstring off_label, int off_value, Wstring on_label,
                                                         int on_value)
{
    return rc_ptr<FlagGameOption>(new FlagGameOption(flag, off_label, off_value, on_label, on_value));
}

}


// int WorldDominationTour::FlagGameOption::Get_Value(rc_ptr<WDTTerritory> const &territory) const (0x00728850)
// OnValue when the territory's OptionFlags has Flag set, else OffValue.

namespace WorldDominationTour {

int FlagGameOption::Get_Value(rc_ptr<WDTTerritory> const &territory) const
{
    if (territory->OptionFlags & Flag) {
        return OnValue;
    }
    return OffValue;
}

}


// WideString WorldDominationTour::FlagGameOption::Describe(rc_ptr<WDTTerritory> const &territory, WideString const &prefix) const (0x00728870)
// prefix followed by ", " and the CSF text of the label for the flag's
// state (OnLabel or OffLabel); prefix alone when that label is empty.

namespace WorldDominationTour {

WideString FlagGameOption::Describe(rc_ptr<WDTTerritory> const &territory, WideString const &prefix) const
{
    Wstring label = (territory->OptionFlags & Flag) ? OnLabel : OffLabel;
    if (!label.Length()) {
        return prefix;
    }
    if (prefix.Length() > 0) {
        WideString separator(L", ");
        return prefix + separator + StringTable::Load_String(label.Get(), 0, "D:\\Ra2\\WDTGameOptions.cpp", 78);
    }
    return WideString(StringTable::Load_String(label.Get(), 0, "D:\\Ra2\\WDTGameOptions.cpp", 81));
}

}


// 0x00728970: function not yet identified
//
// Body: 5 bytes (frozen inventory bound).
// Referenced from 0 sites.
// Ghidra prototype: undefined4 unknown_return_zero_00728970(void)
//
// Types and names are Ghidra placeholders;
// rename and retype as the purpose becomes known.
//

namespace seed_00728970 {

unsigned int __stdcall unknown_return_zero_00728970(int unused_0)
{
    return 0;
}

}  // namespace seed_00728970


// WideString WorldDominationTour::PlayerCountGameOption::Describe(rc_ptr<WDTTerritory> const &territory, WideString const &prefix) const (0x00728980)
// The CSF text WDT:FourPlayerOnly or WDT:TwoPlayerOnly by the territory's
// PlayerCount, after prefix and ", " when prefix is not empty.

namespace WorldDominationTour {

WideString PlayerCountGameOption::Describe(rc_ptr<WDTTerritory> const &territory, WideString const &prefix) const
{
    Wstring label(territory->PlayerCount == 4 ? "WDT:FourPlayerOnly" : "WDT:TwoPlayerOnly");
    if (prefix.Length() > 0) {
        WideString separator(L", ");
        return prefix + separator + StringTable::Load_String(label.Get(), 0, "D:\\Ra2\\WDTGameOptions.cpp", 104);
    }
    return WideString(StringTable::Load_String(label.Get(), 0, "D:\\Ra2\\WDTGameOptions.cpp", 107));
}

}


// rc_ptr<MapSizeGameOption> __fastcall WorldDominationTour::MapSizeGameOption::Create(int flag) (0x00728A60)

namespace WorldDominationTour {

rc_ptr<MapSizeGameOption> __fastcall MapSizeGameOption::Create(int flag)
{
    return rc_ptr<MapSizeGameOption>(new MapSizeGameOption(flag));
}

}


// SPDX-License-Identifier: GPL-3.0-or-later
// Copyright 2026 ra2-decomp contributors
// Copyright 2025 Electronic Arts Inc.; 2026 OpenTS contributors
// Derived from Electronic Arts source code; EA's GPL Section 7 terms apply (COPYING.md).
// WideString WorldDominationTour::MapSizeGameOption::Describe(rc_ptr<WDTTerritory> const &territory, WideString const &prefix) const (0x00728AB0)
// Adapted from ra2-decomp src/wdtgameoptions.cpp MapSizeGameOption::Get_String (gamemd twin 0x00767740,
// matched there); see THIRD_PARTY.md.

namespace WorldDominationTour {

WideString MapSizeGameOption::Describe(rc_ptr<WDTTerritory> const &territory, WideString const &prefix) const
{
    if (prefix.Length() > 0) {
        return operator+(prefix, L", ") +
               StringTable::Load_String(MapSize_Labels[territory.Get()->unknown_12], 0, "D:\\Ra2\\WDTGameOptions.cpp", 129);
    }
    return WideString(
        StringTable::Load_String(MapSize_Labels[territory.Get()->unknown_13], 0, "D:\\Ra2\\WDTGameOptions.cpp", 131));
}

}


// int __stdcall unknown_table_lookup(UnknownOuter_00728b70 *object) -- 0x00728B70
// Purpose: an entry of the 4-column table at 0x007FBB78, indexed by the
// bytes at +0x12 (row) and +0x13 (column) of the record at object +0x04.
// Assumptions: owner, record and table meaning unknown.

struct UnknownRecord_00728b70 {
    char unknown_00[0x12];
    unsigned char Row;                      // +0x12
    unsigned char Column;                   // +0x13
};
struct UnknownOuter_00728b70 {
    int unknown_00;
    UnknownRecord_00728b70 *Record;         // +0x04
};
extern int unknown_table_7fbb78[][4];

int __stdcall unknown_table_lookup(UnknownOuter_00728b70 *object)
{
    return unknown_table_7fbb78[object->Record->Row][object->Record->Column];
}


// WorldDominationTour::FlagGameOption::FlagGameOption(int, Wstring, int, Wstring, int)
// (0x00728B90): inline in shared/wdt_game_options.hpp; emitted out of line for Create.


// WorldDominationTour::FlagGameOption scalar deleting destructor
// (0x00728BF0): vtable 0x007AF154 slot 0; compiler generated.
