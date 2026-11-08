// Names of members of class-typed globals (link adapters, never original code).
// VC6 binds a global's interior address and the member at that offset to one
// address; natively the class layout differs (LP64), so such a name is bound to
// the member itself here, and the native view of its declaration reads it
// through this reference (port/fixups.json). One storage per original address.
#include "shared/session_state.hpp"
#include "shared/multi_mission.hpp"

// 0x00A3D8D8: unknown_session_a3d298 (0x00A3D298) + 0x640, UnknownSessionState::Scenarios.
DynamicVectorClass<MultiMission *> &unknown_multi_missions_a3d8d8_native = unknown_session_a3d298.Scenarios;
