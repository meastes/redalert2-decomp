// Behaviour rows (Scout1125, native M15; NOT EXACT, native/behaviour_rewrites.json):
// NoInit constructors the load game reaches (the placement `new (this) X(NoInitClass())`
// after an object's bytes were read back), without a recovered body. Each is a
// plain function under the constructor's symbols, not a C++ constructor body:
// a C++ body would default-construct the members and overwrite what was loaded.

// 0x00423F60 (pass-6a decompile 00423f60: Leaf_423f60::get, `return this`): the
// timers' NoInit constructor (shared/stage.hpp StageTimerClass, shared/techno.hpp
// UnknownTimer423F60: one address) leaves the loaded values as they are.
extern "C" void *ra2_m15_noinit_timer(void *self, void const *) __asm__("__ZN15StageTimerClassC1ERK11NoInitClass");
extern "C" void *ra2_m15_noinit_timer2(void *self, void const *) __asm__("__ZN15StageTimerClassC2ERK11NoInitClass");
extern "C" void *ra2_m15_noinit_timer3(void *self, void const *) __asm__("__ZN18UnknownTimer423F60C1ERK11NoInitClass");
extern "C" void *ra2_m15_noinit_timer4(void *self, void const *) __asm__("__ZN18UnknownTimer423F60C2ERK11NoInitClass");
void *ra2_m15_noinit_timer(void *self, void const *) { return self; }
void *ra2_m15_noinit_timer2(void *self, void const *) { return self; }
void *ra2_m15_noinit_timer3(void *self, void const *) { return self; }
void *ra2_m15_noinit_timer4(void *self, void const *) { return self; }

// ScenarioClass::ScenarioClass(NoInitClass const &) 0x0065B780 (the scenario,
// Westwood's ScenarioClass; pass-6a decompile 0065b780; Scout1133, M16): run
// after ScenarioClass::Load 0x006610C0 read the object's bytes. As the
// original: the generator is rebuilt (Random2Class(0)), the +0x620 countdown
// restarts (Started = now, DelayTime 0), the three allowable-unit TypeLists are
// rebuilt empty (their vtables are this process's; Load then refills them) and
// the mission timer text is fetched again from its label. Everything else
// keeps the loaded bytes, so a save loads in a fresh process.
#define TYPELIST_IMPLEMENT_DEFAULT_CONSTRUCTOR
#include "shared/id_source.hpp"
#include "shared/type_list.hpp"
#include "shared/abstract_type.hpp"
#include "shared/string_table.hpp"
extern "C" void *ra2_m16_noinit_scenario(ScenarioClass *self, void const *) __asm__("__ZN13ScenarioClassC1ERK11NoInitClass");
extern "C" void *ra2_m16_noinit_scenario2(ScenarioClass *self, void const *) __asm__("__ZN13ScenarioClassC2ERK11NoInitClass");
void *ra2_m16_noinit_scenario(ScenarioClass *self, void const *)
{
    new (&self->RandomNumber) Random2Class(0);
    new (&self->unknown_620) CDTimerClass<SystemTimerClass>();
    new (&self->AllowableUnits) TypeList<TechnoTypeClass *>;
    new (&self->AllowableUnitMaximums) TypeList<int>;
    new (&self->AllowableUnitCounts) TypeList<int>;
    if (self->MissionTimerLabel[0] != 0) {
        self->MissionTimerText = (unsigned short const *)StringTable::Load_String(self->MissionTimerLabel, 0, "D:\\Ra2\\Scenario.CPP", 0x154);
    } else {
        self->MissionTimerText = (unsigned short const *)unknown_empty_wide_839d4c;
    }
    return self;
}
void *ra2_m16_noinit_scenario2(ScenarioClass *self, void const *x) { return ra2_m16_noinit_scenario(self, x); }
