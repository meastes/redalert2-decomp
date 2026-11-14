// mpunholyalliance.cpp: the original translation unit re-filed as one source (Route A): the accepted
// bodies of its canonical compile group in address order, under the unit's own header
// state (its switches and includes in their original order). The module map
// manifests/original_target/modules/mpunholyalliance.json carries the addresses and the rows that
// stay in their own files.


// Header-static dynamic initializers of the unit at 0x005AC140:  the
// shared/unknown_geometry.hpp constants and unknown_static_rect.hpp
// (0x005AC140-0x005AC326); the other
// drafts of this plan are emitted by these includes.
#include "shared/unknown_geometry.hpp"
#include "shared/unknown_static_rect.hpp"


// Dynamic initializer (0x005AC170, _$E) of b from the header(s) included
// by HeaderStatics_diagonal_init_005ac140.cpp; no source of its own.


// Dynamic initializer (0x005AC190, _$E) of c from the header(s) included
// by HeaderStatics_diagonal_init_005ac140.cpp; no source of its own.


// Dynamic initializer (0x005AC1B0, _$E) of angle45 from the header(s) included
// by HeaderStatics_diagonal_init_005ac140.cpp; no source of its own.


// Dynamic initializer (0x005AC1D0, _$E) of angle60 from the header(s) included
// by HeaderStatics_diagonal_init_005ac140.cpp; no source of its own.


// Dynamic initializer (0x005AC1F0, _$E) of angle90 from the header(s) included
// by HeaderStatics_diagonal_init_005ac140.cpp; no source of its own.


// Dynamic initializer (0x005AC210, _$E) of g from the header(s) included
// by HeaderStatics_diagonal_init_005ac140.cpp; no source of its own.


// Dynamic initializer (0x005AC250, _$E) of h from the header(s) included
// by HeaderStatics_diagonal_init_005ac140.cpp; no source of its own.


// Dynamic initializer (0x005AC280, _$E) of i from the header(s) included
// by HeaderStatics_diagonal_init_005ac140.cpp; no source of its own.


// Dynamic initializer (0x005AC2B0, _$E) of j from the header(s) included
// by HeaderStatics_diagonal_init_005ac140.cpp; no source of its own.


// Dynamic initializer (0x005AC2E0, _$E) of k from the header(s) included
// by HeaderStatics_diagonal_init_005ac140.cpp; no source of its own.


// Dynamic initializer (0x005AC2F0, _$E) of l from the header(s) included
// by HeaderStatics_diagonal_init_005ac140.cpp; no source of its own.


// Dynamic initializer (0x005AC310, _$E) of static_rect from the header(s) included
// by HeaderStatics_diagonal_init_005ac140.cpp; no source of its own.


// UnholyAlliance::UnholyAlliance(...) -- 0x005AC330 (the UnholyAlliance unit)
// Forwards its six arguments to the MultiplayerGameMode constructor.
#ifndef VECTOR_IMPLEMENT_ALL
#define VECTOR_IMPLEMENT_ALL
#endif
#include "shared/multiplayer_game_mode.hpp"

// A game option the Unholy Alliance mode switches on (0x00A3D2B8; its
// meaning is not identified).
extern bool unknown_flag_a3d2b8;

UnholyAlliance::UnholyAlliance(WideString const &name, WideString const &description, Wstring const &ini_name,
    Wstring const &unknown_4, bool unknown_5, int id) :
    MultiplayerGameMode(name, description, ini_name, unknown_4, unknown_5, id)
{
}


// UnholyAlliance::~UnholyAlliance() -- 0x005AC370

UnholyAlliance::~UnholyAlliance()
{
}


// bool UnholyAlliance::AssignTeams() -- 0x005AC380
// Slot 4: switches the option at 0x00A3D2B8 on; true.

bool UnholyAlliance::AssignTeams()
{
    unknown_flag_a3d2b8 = true;
    return true;
}


// 0x005ac390: function not yet identified
//
// Owner: UnholyAlliance, virtual slot 5 (vtable +0x14) (from RTTI/vtables).
// Body: 36 bytes (frozen inventory bound).
// Referenced from 1 site (data).
// Ghidra prototype: undefined4 FUN_005ac390(undefined4 param_1)
// Callees with accepted source:
//   FUN_005b73a0 -> src/recovered/classes/MultiplayerGameMode/005b73a0.cpp
//
// Types and names are Ghidra placeholders;
// rename and retype as the purpose becomes known.
//

namespace seed_005ac390 {

extern unsigned int __stdcall CheckStartConditions_005b73a0(int);
extern "C" char DAT_00a3d2b8;

unsigned int __stdcall FUN_005ac390(unsigned int message_address)
{
    char result_byte;

    if (DAT_00a3d2b8 != '\0') {
        result_byte = (char)(CheckStartConditions_005b73a0((int)(message_address)));
        if (result_byte != '\0') {
            return 1;
        }
    }
    return 0;
}

}  // namespace seed_005ac390


// bool UnholyAlliance::AssignTeamsAtStart() -- 0x005AC3C0
// Slot 6: switches the option at 0x00A3D2B8 on; true.

bool UnholyAlliance::AssignTeamsAtStart()
{
    unknown_flag_a3d2b8 = true;
    return true;
}


// bool UnholyAlliance::SpawnBaseUnits(HouseClass *house, int *money) -- 0x005AC3D0
// Puts one unit of each of the first two Rules BaseUnit types near the house's BaseCenter:
// the first at the cell three cells down and right of it if it can be unlimboed there, else
// (and the second always) placed near the BaseCenter cell; false when any of them fails.
#include "shared/techno.hpp"
#include "shared/id_source.hpp"
#include "shared/unknown_globals.hpp"
#include "shared/unit_type.hpp"
#include "shared/infantry_type.hpp"
#include "shared/house.hpp"
#include "shared/rules.hpp"

bool UnholyAlliance::SpawnBaseUnits(HouseClass *house, int *money)
{
    bool failed = false;
    for (int i = 0; i < 2; i++) {
        UnitTypeClass *type = Rules->BaseUnit[i];
        if (type == 0) {
            failed = true;
            continue;
        }
        TechnoClass *unit = (TechnoClass *)new UnitClass(type, house);
        if (unit == 0) {
            failed = true;
            continue;
        }
        Cell cell = Coord_Cell(house->BaseCenter);
        Cell offset = cell + Cell(3, 3);
        if (i == 0 && unit->Unlimbo(&CoordStruct(offset.X * 256 + 128, offset.Y * 256 + 128, 0), 0)) {
            continue;
        }
        Cell spot = Cell(house->BaseCenter.X / 256, house->BaseCenter.Y / 256);
        if (!Unknown_00660b60(unit, spot, 3)) {
            failed = true;
            delete unit;
        }
    }
    return !failed;
}


// UnholyAlliance scalar deleting destructor (0x005AC510, ??_G): emitted with the
// vtable; the destructor is expanded inline.
