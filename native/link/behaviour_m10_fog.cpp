// Behaviour row (Scout1103, native M10). NOT EXACT; never original code or bytes.
#define VECTOR_IMPLEMENT_DYNAMIC_ACCESSORS
#include "shared/techno.hpp"
#include "shared/fogged_object.hpp"

// 0x004C2030 (seed decompile): a fogged object's first drawn type (the first
// record of its DrawRecords vector, +0x5C; the count at +0x6C), or null.
// techno.hpp's UnknownCellEntry28 is the FoggedObjectClass as TechnoClass
// slot 27 sees it.
UnknownCellEntryType *__fastcall Unknown_004C2030(UnknownCellEntry28 *entry)
{
    FoggedObjectClass *fogged = (FoggedObjectClass *)entry;
    if (fogged->DrawRecords.Count() > 0) {
        return (UnknownCellEntryType *)fogged->DrawRecords[0].Type;
    }
    return 0;
}
