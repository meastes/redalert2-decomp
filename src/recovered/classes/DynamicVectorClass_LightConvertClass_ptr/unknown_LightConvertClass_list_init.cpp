// The light converter list's unit (0x0040C0C0-0x0040C49F): its only global,
// TileDrawers (declared in shared/isometric_tile_type.hpp),
// with its initializer (0x0040C0C0) and atexit destructor (0x0040C100); the
// list's template members and vtables (0x0079A85C, 0x0079A87C) follow as this
// unit's COMDAT copies (0x0040C140-0x0040C3D0 accepted as standalone members;
// the two deleting destructors 0x0040C400/0x0040C450 are claimed here).
#ifndef VECTOR_IMPLEMENT_ALL
#define VECTOR_IMPLEMENT_ALL
#endif
#include "shared/vector_class.hpp"

class LightConvertClass;

DynamicVectorClass<LightConvertClass *> TileDrawers;
