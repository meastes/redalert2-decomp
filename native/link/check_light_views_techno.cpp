// Native layout check (Scout1103), as check_light_views.cpp: shared/techno.hpp's
// stand-ins for TubeClass (UnknownTube47A130), FoggedObjectClass
// (UnknownCellEntry28) and ObjectTypeClass's +0x201 flag (UnknownCellEntryType),
// whose VC6 padding port/fixups.json respells, against the real classes' arm64
// offsets (shared/tube.hpp, fogged_object.hpp, object_type.hpp, measured with
// offsetof under the dialect flags). Compile-time only.
#include "shared/techno.hpp"
#include <cstddef>

static_assert(offsetof(UnknownTube47A130, unknown_24) == 0x32, "techno.hpp UnknownTube47A130 EnterCell");
static_assert(offsetof(UnknownTube47A130, unknown_28) == 0x36, "techno.hpp UnknownTube47A130 ExitCell");
static_assert(offsetof(UnknownTube47A130, unknown_2c) == 0x3C, "techno.hpp UnknownTube47A130 Facing");
static_assert(offsetof(UnknownCellEntryType, unknown_201) == 0x321, "techno.hpp UnknownCellEntryType +0x201");
static_assert(offsetof(UnknownCellEntry28, unknown_24) == 0x34, "techno.hpp UnknownCellEntry28 +0x24");
static_assert(offsetof(UnknownCellEntry28, unknown_28) == 0x38, "techno.hpp UnknownCellEntry28 owner");
static_assert(offsetof(UnknownCellEntry28, unknown_30) == 0x44, "techno.hpp UnknownCellEntry28 kind");
static_assert(offsetof(UnknownCellEntry28, unknown_74) == 0x90, "techno.hpp UnknownCellEntry28 active");
static_assert(offsetof(UnknownTechno230, unknown_24) == 0x38, "techno.hpp UnknownTechno230 +0x24 (CaptureManagerClass; NativeFix2)");
