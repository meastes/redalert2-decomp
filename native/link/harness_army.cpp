// Skirmish test harness: an army (README "Skirmish harness";
// --skirmish-order=win[@<frame>]). Test scaffolding, never game behaviour
// (Scout1125, native M15): once the build order of harness_build.cpp has put up
// a war factory, the harness queues the events a player's clicks would:
//   * the tank cameo click: PRODUCE (event 0x0E, kind 0x28 = UnitTypeClass,
//     the type's index in UnitTypes, not naval), one tank at a time while the
//     house's vehicle factory (HouseClass::unknown_func_4eb580 0x004EB580,
//     kinds 1/0x28) is idle;
//   * the attack: once twelve tanks stand (or at the frame given by
//     RA2_NATIVE_ARMY_ATTACK, default 9,000) every combat unit of the player
//     (vehicles and infantry that neither deploy into a building nor harvest)
//     gets the hunt mission 15 (Queue_Mission 0x006233A0, as harness_orders'
//     hunt), and every later tank joins as it leaves the factory.
// Notes harness-army-<n> per event.
// RA2_NATIVE_ARMY_INFANTRY=<frame> (Scout1153, M19; a campaign whose tech level
// has no war factory, Allied 1): from that frame, while no tank is buildable,
// the barracks trains one rifleman (GI or Conscript: PRODUCE kind 0x10 =
// InfantryTypeClass) at a time instead, and they attack when RA2_NATIVE_ARMY_SIZE
// (default 12) stand or at RA2_NATIVE_ARMY_ATTACK.
#include "shared/techno.hpp"
#include "shared/techno_type.hpp"
#include "shared/unit_type.hpp"
#include "shared/infantry_type.hpp"
#include "shared/house.hpp"
#include "shared/target.hpp"
#include "shared/event.hpp"
#include "shared/queue.hpp"

#include <stdlib.h>

extern DynamicVectorClass<TechnoClass *> Technos;   // 0x00A40C20 (shared/logic.hpp)

extern "C" {
void ra2port_progress(const char *milestone, const char *detail);
}

namespace {
void Append(char *to, char const *text)
{
    while (*to) ++to;
    while (*text) *to++ = *text++;
    *to = 0;
}
void Append_Int(char *to, int value)
{
    while (*to) ++to;
    char digits[16];
    int n = 0;
    unsigned v = value < 0 ? -value : value;
    do { digits[n++] = '0' + v % 10; v /= 10; } while (v);
    if (value < 0) *to++ = '-';
    while (n) *to++ = digits[--n];
    *to = 0;
}
bool Same_Name(char const *a, char const *b)
{
    while (*a && *b) {
        char x = *a >= 'a' && *a <= 'z' ? *a - 32 : *a;
        char y = *b >= 'a' && *b <= 'z' ? *b - 32 : *b;
        if (x != y) return false;
        ++a, ++b;
    }
    return *a == *b;
}

char const *const tank_names[] = {"MTNK", "HTNK"};   // Grizzly (Allies), Rhino (Soviets)
char const *const harvester_names[] = {"CMIN", "HARV"};
int const attack_size = 12;

int tank_index = -1;
int infantry_index = -1;
char const *const infantry_names[] = {"E1", "E2"};   // GI (Allies), Conscript (Soviets)
bool attacking = false;
int next_check = 0;
int produced = 0;

void Note(char const *what, int frame, int value)
{
    static int notes = 0;
    static char names[40][20];
    if (notes >= 40) return;
    char detail[160] = "frame ";
    Append_Int(detail, frame);
    Append(detail, ": ");
    Append(detail, what);
    Append(detail, " ");
    Append_Int(detail, value);
    char *name = names[notes++];
    name[0] = 0;
    Append(name, "harness-army-");
    Append_Int(name, notes);
    ra2port_progress(name, detail);
}

bool Is_Combat(TechnoClass *techno)
{
    int kind = techno->Kind_Of();
    if (kind != 1 && kind != 0x0F) return false;           // UnitClass, InfantryClass
    TechnoTypeClass *type = techno->unknown_slot32();
    if (type == 0 || type->DeploysInto != 0) return false;  // the MCV
    for (int i = 0; i < 2; ++i) {
        if (Same_Name(type->ID.Buffer, harvester_names[i])) return false;
    }
    // Tanya stays back: a campaign loses when she dies (Allied 1's Enable Failure;
    // Scout1153, M19, the win order in a campaign).
    if (Same_Name(type->ID.Buffer, "TANY")) return false;
    if (Same_Name(type->ID.Buffer, "ENGINEER") || Same_Name(type->ID.Buffer, "SENGINEER")) return false;   // engineers repair/capture by script
    return true;
}

int Hunt_All()
{
    int sent = 0;
    for (int i = 0; i < Technos.Count(); ++i) {
        TechnoClass *techno = Technos[i];
        if (techno == 0 || techno->House != unknown_player_a35db4 || !Is_Combat(techno)) continue;
        if (techno->Mission == 15) continue;
        Queue_Mission(TargetClass(techno), 15, TargetClass(), TargetClass());
        ++sent;
    }
    return sent;
}

int Riflemen()
{
    int count = 0;
    for (int i = 0; i < Technos.Count(); ++i) {
        TechnoClass *techno = Technos[i];
        if (techno == 0 || techno->House != unknown_player_a35db4 || techno->Kind_Of() != 0x0F) continue;
        TechnoTypeClass *type = techno->unknown_slot32();
        if (type != 0 && infantry_index >= 0 && type == InfantryTypes[infantry_index]) ++count;
    }
    return count;
}
int Tanks()
{
    int count = 0;
    for (int i = 0; i < Technos.Count(); ++i) {
        TechnoClass *techno = Technos[i];
        if (techno == 0 || techno->House != unknown_player_a35db4 || techno->Kind_Of() != 1) continue;
        TechnoTypeClass *type = techno->unknown_slot32();
        if (type != 0 && tank_index >= 0 && type == UnitTypes[tank_index]) ++count;
    }
    return count;
}
}  // namespace

void Native_Harness_Houses(char const *milestone);   // harness_build.cpp

// Called by the order seam (harness_orders.cpp) after each Main_Loop frame when
// the order kind is win.
void Native_Harness_Army(int frame)
{
    HouseClass *player = unknown_player_a35db4;
    if (frame % 2000 == 0 && frame <= 60000) {
        // Buildings and units per house every 2,000 frames (harness-houses-<k>k).
        static char names[31][24];
        char *name = names[frame / 2000 - 1];
        name[0] = 0;
        Append(name, "harness-houses-");
        Append_Int(name, frame / 1000);
        Append(name, "k");
        Native_Harness_Houses(name);
    }
    if (player == 0 || frame < next_check) return;
    next_check = frame + 30;
    static int infantry_frame = -2;
    if (infantry_frame == -2) {
        char const *env = getenv("RA2_NATIVE_ARMY_INFANTRY");
        infantry_frame = env ? atoi(env) : -1;
    }
    if (tank_index < 0 && infantry_frame >= 0 && frame >= infantry_frame) {
        if (infantry_index < 0) {
            for (int i = 0; i < InfantryTypes.Count() && infantry_index < 0; ++i) {
                for (int k = 0; k < 2; ++k) {
                    if (Same_Name(InfantryTypes[i]->ID.Buffer, infantry_names[k]) && player->Can_Build(InfantryTypes[i], false, true) > 0) {
                        infantry_index = i;
                        Note("rifleman type buildable, index", frame, i);
                        break;
                    }
                }
            }
        }
        if (infantry_index >= 0 && player->unknown_func_4eb580(0x10, false, 0) == 0
            && player->Can_Build(InfantryTypes[infantry_index], false, true) > 0) {
            OutList.Add(EventClass(player->ArrayIndex, 0x0E, 0x10, infantry_index, 0));
            ++produced;
            if (produced <= 3 || produced % 5 == 0) Note("train rifleman", frame, produced);
        }
        int riflemen = Riflemen();
        static int attack_frame_i = -1;
        if (attack_frame_i < 0) {
            char const *env = getenv("RA2_NATIVE_ARMY_ATTACK");
            attack_frame_i = env ? atoi(env) : 9000;
        }
        static int rifle_size = -1;     // RA2_NATIVE_ARMY_SIZE (default the tanks' 12)
        if (rifle_size < 0) {
            char const *env = getenv("RA2_NATIVE_ARMY_SIZE");
            rifle_size = env ? atoi(env) : attack_size;
        }
        if (!attacking && (riflemen >= rifle_size || frame >= attack_frame_i)) {
            attacking = true;
            Note("attack: hunt, units", frame, Hunt_All());
            return;
        }
        if (attacking && frame % 150 < 30) {
            int sent = Hunt_All();
            if (sent) Note("reinforce: hunt, units", frame, sent);
        }
    }
    if (tank_index < 0) {
        for (int i = 0; i < UnitTypes.Count() && tank_index < 0; ++i) {
            UnitTypeClass *type = UnitTypes[i];
            for (int k = 0; k < 2; ++k) {
                if (Same_Name(type->ID.Buffer, tank_names[k]) && player->Can_Build(type, false, true) > 0) {
                    tank_index = i;
                    Note("tank type buildable, index", frame, i);
                    break;
                }
            }
        }
        if (tank_index < 0) return;
    }
    if (player->unknown_func_4eb580(0x28, false, 0) == 0 && player->Can_Build(UnitTypes[tank_index], false, true) > 0) {
        OutList.Add(EventClass(player->ArrayIndex, 0x0E, 0x28, tank_index, 0));
        ++produced;
        if (produced <= 3 || produced % 5 == 0) Note("produce tank", frame, produced);
    }
    static int attack_frame = -1;
    if (attack_frame < 0) {
        char const *env = getenv("RA2_NATIVE_ARMY_ATTACK");
        attack_frame = env ? atoi(env) : 9000;
    }
    int tanks = Tanks();
    if (!attacking && (tanks >= attack_size || frame >= attack_frame)) {
        attacking = true;
        Note("attack: hunt, units", frame, Hunt_All());
        return;
    }
    if (attacking && frame % 150 < 30) {
        int sent = Hunt_All();
        if (sent) Note("reinforce: hunt, units", frame, sent);
    }
}
