// Native layout check (Scout1089), as check_light_views.cpp: the light
// HouseClass of shared/house_screen.hpp against the full class's arm64 offsets
// (behaviour_scenario.cpp asserts the full side). Compile-time only.
#include "shared/house_screen.hpp"
#include <cstddef>

static_assert(offsetof(HouseClass, ArrayIndex) == 0x50, "house_screen.hpp ArrayIndex");
static_assert(offsetof(HouseClass, Type) == 0x58, "house_screen.hpp Type");
static_assert(offsetof(HouseClass, Buildings) == 0xA0, "house_screen.hpp Buildings");
static_assert(offsetof(HouseClass, unknown_130) == 0x188, "house_screen.hpp unknown_130");
static_assert(offsetof(HouseClass, unknown_13d) == 0x195, "house_screen.hpp unknown_13d");
static_assert(offsetof(HouseClass, FirestormActive) == 0x19A, "house_screen.hpp FirestormActive");
static_assert(offsetof(HouseClass, WaypointPaths) == 0x1B0, "house_screen.hpp WaypointPaths");
static_assert(offsetof(HouseClass, Supers) == 0x228, "house_screen.hpp Supers");
static_assert(offsetof(HouseClass, unknown_207) == 0x29B, "house_screen.hpp unknown_207");
static_assert(offsetof(HouseClass, unknown_230) == 0x2C4, "house_screen.hpp unknown_230");
static_assert(offsetof(HouseClass, PowerOutput) == 0x5364, "house_screen.hpp PowerOutput");
static_assert(offsetof(HouseClass, unknown_5364) == 0x5428, "house_screen.hpp unknown_5364");
static_assert(offsetof(HouseClass, BaseCenter) == 0x5480, "house_screen.hpp BaseCenter");
static_assert(offsetof(HouseClass, unknown_5414) == 0x54E0, "house_screen.hpp unknown_5414");
static_assert(offsetof(HouseClass, unknown_56d0) == 0x58A0, "house_screen.hpp unknown_56d0");
static_assert(offsetof(HouseClass, IniName) == 0x160B0, "house_screen.hpp IniName");
static_assert(offsetof(HouseClass, UIName) == 0x160E6, "house_screen.hpp UIName");
static_assert(offsetof(HouseClass, ColorSchemeIndex) == 0x16110, "house_screen.hpp ColorSchemeIndex");
