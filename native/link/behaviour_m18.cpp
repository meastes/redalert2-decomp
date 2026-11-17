// Native helpers (Scout1142, native M18; NOT EXACT, no original row of their own).
#include "shared/object_type.hpp"

// ObjectTypeClass slot 39 (the type's image, +0xA4) through the real class's
// vtable, for the unowned IsometricTileTypeClass views of Land_Type 0x00529300
// and 0x0052DFA0, whose hand-listed VC6 slots do not match the native vtable
// (port/fixups.json).
void *Native_Object_Type_Image(void *object)
{
    return ((ObjectTypeClass *)object)->Get_Image_Data();
}
