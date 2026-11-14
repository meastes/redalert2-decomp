// The real classes behind check_light_views_techno.cpp's numbers (Scout1103):
// if a src/shared change moves one of these, both files need the new value.
#include "shared/tube.hpp"
#include "shared/fogged_object.hpp"
#include "shared/capture_manager.hpp"
#include <cstddef>

static_assert(offsetof(TubeClass, EnterCell) == 0x32, "TubeClass EnterCell");
static_assert(offsetof(TubeClass, ExitCell) == 0x36, "TubeClass ExitCell");
static_assert(offsetof(TubeClass, Facing) == 0x3C, "TubeClass Facing");
static_assert(offsetof(FoggedObjectClass, unknown_24) == 0x34, "FoggedObjectClass +0x24");
static_assert(offsetof(FoggedObjectClass, unknown_28) == 0x38, "FoggedObjectClass +0x28");
static_assert(offsetof(FoggedObjectClass, unknown_30) == 0x44, "FoggedObjectClass +0x30");
static_assert(offsetof(FoggedObjectClass, unknown_74) == 0x90, "FoggedObjectClass +0x74");
static_assert(offsetof(CaptureManagerClass, unknown_24) == 0x38, "CaptureManagerClass +0x24 (techno.hpp UnknownTechno230, unit.cpp radio 15; NativeFix2)");
static_assert(offsetof(CaptureManagerClass, unknown_28) == 0x40, "CaptureManagerClass +0x28 (house.cpp UnknownTechno230View; NativeFix2)");
