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

// UnknownIDSource (the scenario, Westwood's ScenarioClass) NoInit constructor:
// unrecovered. Simplified: the loaded bytes are kept as read (the NoInit meaning
// for its scalar members); its members' NoInit constructors (timer and vector
// vtables) are not re-run, so this holds only for a game loaded by the process
// that saved it (the loaded vtable pointers are that process's). TibSun
// scenario.cpp ScenarioClass(NoInitClass) is the shape reference for a full row.
extern "C" void *ra2_m15_noinit_scenario(void *self, void const *) __asm__("__ZN15UnknownIDSourceC1ERK11NoInitClass");
extern "C" void *ra2_m15_noinit_scenario2(void *self, void const *) __asm__("__ZN15UnknownIDSourceC2ERK11NoInitClass");
void *ra2_m15_noinit_scenario(void *self, void const *) { return self; }
void *ra2_m15_noinit_scenario2(void *self, void const *) { return self; }
