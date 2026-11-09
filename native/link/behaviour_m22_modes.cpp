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

// 0x00537B50: a light source adds only the AbstractClass fields to the CRC.
void LightSourceClass::Compute_CRC(WWCRCEngine &crc) const
{
    AbstractClass::Compute_CRC(crc);
}

// MPCooperative (multiplayer_game_mode.hpp), pass-6a decompiles.
bool MPCooperative::unknown_slot1() { return true; }                       // 0x005A6280
void MPCooperative::unknown_slot9() { unknown_344 = true; }                // 0x005A31F0
bool MPCooperative::unknown_slot11() { return false; }                     // 0x005A46B0
void MPCooperative::unknown_slot16(HWND combo, int team)                   // 0x005A3F70
{
    MultiplayerGameMode::unknown_slot16(combo, team);
}
int MPCooperative::unknown_slot32() { return 2; }                          // 0x005A5340
bool MPCooperative::unknown_slot40() { return false; }                     // 0x005A62A0

// 0x005AC390: with multiplayer bases on (session +0x20, 0x00A3D2B8) the base mode's test.
bool UnholyAlliance::unknown_slot5(WideString *message)
{
    return unknown_flag_a3d2b8 && MultiplayerGameMode::unknown_slot5(message);
}
