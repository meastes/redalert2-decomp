// Behaviour rows for native M24 (Scout1191). NOT EXACT: nothing here is original
// code; every row stays unrecovered in the exact tree (native/behaviour_rewrites.json).
#include "shared/ini_class.hpp"
#include "shared/wwcrc_engine.hpp"
#include "shared/tiberium.hpp"
#include "shared/fogged_object.hpp"

// 0x00493DF0, 0x00493E80, 0x00493FC0: WWCRCEngine's char, bool and buffer appends
// are the recovered CRCEngine bodies at the same addresses (one class under two
// names: shared/wwcrc_engine.hpp and shared/ini_class.hpp's CRCEngine have the same
// members, CRC, Index and the staging word). Forwarded until src/ uses one name.
static_assert(sizeof(WWCRCEngine) == sizeof(CRCEngine), "one CRC accumulator");
void WWCRCEngine::operator()(char datum)
{
    (*reinterpret_cast<CRCEngine *>(this))(datum);
}
void WWCRCEngine::operator()(bool datum)
{
    (*reinterpret_cast<CRCEngine *>(this))(datum);
}
RA2_LONG WWCRCEngine::operator()(void const *buffer, int length)
{
    return (*reinterpret_cast<CRCEngine *>(this))(buffer, length);
}

// 0x006E9380 TiberiumClass::GetSizeMax (pass-6a decompile 006e9380): the
// AbstractClass size plus a count and one entry per Debris type.
HRESULT __stdcall TiberiumClass::GetSizeMax(ULARGE_INTEGER *size)
{
    HRESULT result = AbstractClass::GetSizeMax(size);
    if (result >= 0) {
        size->LowPart += Debris.Count() * 4 + 4;
        result = 0;
    }
    return result;
}

// 0x006EA9E0 TiberiumClass slot 16 (decompile 006ea9e0): the array index.
int TiberiumClass::Fetch_Heap_ID() const
{
    return ArrayIndex;
}

// 0x004C1F80 FoggedObjectClass::Compute_CRC (decompile 004c1f80): AbstractClass's
// CRC, then the words at +0x24 and +0x2C..+0x58 (the coordinate and the redraw
// rectangle among them) and the draw record count.
void FoggedObjectClass::Compute_CRC(WWCRCEngine &crc) const
{
    AbstractClass::Compute_CRC(crc);
    crc((RA2_LONG)unknown_24);
    crc((RA2_LONG)unknown_2c);
    crc((RA2_LONG)unknown_30);
    crc((RA2_LONG)Coord.X);
    crc((RA2_LONG)Coord.Y);
    crc((RA2_LONG)Coord.Z);
    crc((RA2_LONG)unknown_40.X);
    crc((RA2_LONG)unknown_40.Y);
    crc((RA2_LONG)unknown_40.Width);
    crc((RA2_LONG)unknown_40.Height);
    crc((RA2_LONG)unknown_50);
    crc((RA2_LONG)unknown_54);
    crc((RA2_LONG)unknown_58);
    crc((RA2_LONG)DrawRecords.Count());
}
