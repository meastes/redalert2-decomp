// Behaviour-written bodies for native M22 (Scout1177): COM persistence slots of
// the bomb, EMP pulse and overlay objects (a save while one exists), the
// cooperative game mode's slots and a few defaults in live vtables. NOT EXACT:
// each follows its pass-6a decompile.
#include "shared/object.hpp"
#include "shared/empulse_class.hpp"
#include "shared/overlay.hpp"
#include "shared/light_source.hpp"
#include "shared/multiplayer_game_mode.hpp"
#include "shared/unknown_globals.hpp"

// 0x00436100: the class id (E_POINTER for a null out pointer).
HRESULT __stdcall BombClass::GetClassID(GUID *clsid)
{
    if (clsid == 0) return (HRESULT)0x80004003;
    *clsid = CLSID_BombClass;
    return 0;
}

// 0x004361D0 / 0x004B53D0 / 0x005DBCE0: the object is AbstractClass::Save's
// (EMPulseClass answers S_OK for any success).
HRESULT __stdcall BombClass::Save(IStream *stream, BOOL clear_dirty)
{
    return AbstractClass::Save(stream, clear_dirty);
}

HRESULT __stdcall EMPulseClass::Save(IStream *stream, BOOL clear_dirty)
{
    HRESULT result = AbstractClass::Save(stream, clear_dirty);
    return result < 0 ? result : 0;
}

HRESULT __stdcall OverlayClass::Save(IStream *stream, BOOL clear_dirty)
{
    return AbstractClass::Save(stream, clear_dirty);
}

// 0x004B5400 / 0x004B53F0: RTTI kind 0x37; the size (0x34 in the original, the
// native object here: AbstractClass::Save writes Size_Of() bytes).
int EMPulseClass::Kind_Of() const { return 0x37; }
int EMPulseClass::Size_Of() const { return sizeof(EMPulseClass); }

// MPCooperative (multiplayer_game_mode.hpp), pass-6a decompiles.
bool MPCooperative::unknown_slot1() { return true; }                       // 0x005A6280
void MPCooperative::unknown_slot9() { unknown_344 = true; }                // 0x005A31F0
bool MPCooperative::IsAIAllowed() { return false; }                     // 0x005A46B0
void MPCooperative::PopulateTeamDropdownForPlayer(HWND combo, int team)                   // 0x005A3F70
{
    MultiplayerGameMode::PopulateTeamDropdownForPlayer(combo, team);
}
int MPCooperative::unknown_slot32() { return 2; }                          // 0x005A5340
bool MPCooperative::unknown_slot40() { return false; }                     // 0x005A62A0

// 0x005AC390: with multiplayer bases on (session +0x20, 0x00A3D2B8) the base mode's test.
bool UnholyAlliance::CheckStartConditions(WideString *message)
{
    return unknown_flag_a3d2b8 && MultiplayerGameMode::CheckStartConditions(message);
}

// MultiplayerSiege (lane C16, link-closure classification): MPMODES.INI's Siege modes are made at
// start-up (MultiplayerGameMode::unknown_init_modes_5b8bd0, Read_Game_Modes "Siege"); the
// installed game lists none, a mod's MPMODES.INI may. The constructor (0x005AB5A0) forwards its
// six arguments to the MultiplayerGameMode constructor and the destructor (0x005AB5E0) is the
// base's, as FreeForAll's; slot 11 (0x005AB750, folded with other "false" leaves) allows no AI.
MultiplayerSiege::MultiplayerSiege(WideString const &name, WideString const &description, Wstring const &ini_name,
    Wstring const &unknown_4, bool unknown_5, int id) :
    MultiplayerGameMode(name, description, ini_name, unknown_4, unknown_5, id)
{
}
MultiplayerSiege::~MultiplayerSiege()
{
}
bool MultiplayerSiege::IsAIAllowed() { return false; }
