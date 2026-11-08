// WalkLocomotionClass's movement step (0x0071E220, 4,967 B, unrecovered) for
// the native build (Scout1091, native M8). NOT EXACT and PARTIAL: only the
// idle state is written; every other state reports itself as unrecovered (the
// link closure's trap behaviour), so a moving infantry stops the run instead
// of reading an undefined result. Never original code or bytes.
#include "shared/walk_locomotion.hpp"
#include "shared/foot.hpp"

extern double const unknown_double_79b610;   // 0x0079B610 (0.0)
extern "C" void ra2port_trap_reached(const char *name);
extern "C" char *getenv(char const *);
extern "C" int dprintf(int, char const *, ...);
extern "C" void abort(void);

// 0x0071E220 (TibSun walk.cpp WalkLocomotionClass::Movement_AI; decompile):
// with no head-to coordinate and no destination (both at the unit zero
// 0x00AF9060) a walker that still has speed (+0x468 > 0.0) is stopped through
// FootClass slot 306 (Set_Speed(0.0)); either way its new-navigation flag
// (+0x57A) is cleared. The path, blocked-cell, tube and step branches are not
// written yet.
void WalkLocomotionClass::unknown_71e220(bool first_pass)
{
    bool no_head_to = unknown_28.X == unknown_coord_af9060.X && unknown_28.Y == unknown_coord_af9060.Y
        && unknown_28.Z == unknown_coord_af9060.Z;
    bool no_destination = unknown_1c.X == unknown_coord_af9060.X && unknown_1c.Y == unknown_coord_af9060.Y
        && unknown_1c.Z == unknown_coord_af9060.Z;
    if (no_head_to && no_destination) {
        if (LinkedTo->unknown_468 > unknown_double_79b610) {
            LinkedTo->unknown_slot306(0.0);
            LinkedTo->unknown_57a = false;
            return;
        }
        LinkedTo->unknown_57a = false;
        return;
    }
    static char const name[] = "WalkLocomotionClass::unknown_71e220(bool) (moving states; native partial)";
    dprintf(2, "ra2-native: unrecovered %s reached\n", name);
    ra2port_trap_reached(name);
    if (!getenv("RA2_NATIVE_TRAP_CONTINUE")) abort();
}

#include "shared/techno.hpp"
#include "shared/techno_type.hpp"

// 0x006D8640 TechnoClass::Unknown_006D8640 (decompile: a __thiscall that
// reads only its argument): true when the weapon slot exists and names a
// weapon type (its first field).
bool TechnoClass::Unknown_006D8640(WeaponStruct *weapon)
{
    return weapon != 0 && weapon->WeaponType != 0;
}
