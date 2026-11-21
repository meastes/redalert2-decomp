// Mission soak (lane N25; README "Mission soak", ctest label `soak`). Test scaffolding,
// never game behaviour: with RA2_NATIVE_SOAK=<period> each Main_Loop pass the harness
// watches the scenario that is playing and writes to stderr
//   soak-mission <file> loop <n> triggers <live> types <n>   when a scenario starts (Frame
//                         went back, or another scenario file);
//   soak-trigger <frame> gone <ID> <name>                   a live trigger left Triggers (a
//                         volatile trigger sprung and deleted, or one an action destroyed);
//   soak-trigger <frame> new <ID> <name>                    a trigger made after the start
//                         (a tag an action attached, Find_Or_Make);
//   soak-trigger <frame> tripped <ID> <bits>                its tripped-events bits changed
//                         (TriggerClass::IsTripped +0x3C);
//   soak-timer <frame> started|stopped <left>               the mission timer (Scen +0x7C8);
//   soak-pause <frame> paused <0|1> held <n> loop <n>       the scenario's pause (Scen +0x630, +0x62C);
//   soak-house <frame> <i>:<name> defeated|won              a house's end;
//   soak-digest <frame> <hash> technos <n> player <n> credits <n> rng <hash>   every <period>
//                         game frames: a hash of every techno's type, house, cell, height,
//                         strength and mission, the houses' balances and the scenario's random
//                         generator, so two runs of one mission with the same seed can be compared
//                         frame by frame (a desync shows as the first digest that differs).
// The game frame is Frame (0x00A40D2C, from the scenario's start). The soak's orders use it
// too: RA2_NATIVE_ORDERS "+<n>[/<p>] <order>" (harness_orders.cpp).
// RA2_NATIVE_SOAK_SEED=<n> sets CustomSeed (0x00A40D40), which Init_Random 0x00515710 then
// gives the scenario instead of GetTickCount, so a run's random numbers do not come from the
// clock (see Native_Soak_Seed).
// RA2_NATIVE_SOAK_SPEED=<0-6>: a single-player game's frame delay (Main_Loop keeps it at 2 ticks,
// 30 frames per second, unless -SPEEDCONTROL set GameControlsShown 0x00A40D84; the harness sets
// that flag and the delay unknown_a40b18 as a network game's speed would), so a soak runs as fast
// as the machine draws (0). unknown_a40b18 is Options.GameSpeed (0x00A40B18): normalized animations
// take their rate from it, so a run at another speed is another game (lane N25: Allied 1 parts at frame 62).
#include "shared/trigger.hpp"
#include "shared/techno.hpp"
#include "shared/techno_type.hpp"
#include "shared/house.hpp"
#include "shared/house_type.hpp"
#include "shared/id_source.hpp"
#include "shared/queue.hpp"
#include "shared/unknown_globals.hpp"
#include "shared/main_loop.hpp"
#include "shared/frame_rate.hpp"
#include "shared/display.hpp"
#include "shared/tactical.hpp"
#include "shared/spawn_manager.hpp"
#include "shared/logic.hpp"
#include "shared/anim.hpp"

#include <stdio.h>
#include <stdlib.h>

extern "C" void ra2port_progress(const char *milestone, const char *detail);

class TechnoClass;
extern DynamicVectorClass<TechnoClass *> Technos;   // 0x00A40C20 (shared/logic.hpp)

extern "C" { void *volatile soak_watch_address = 0; void *volatile soak_watch_object = 0; }
namespace {
// Copies the scenario's file name; true when it differs from the one kept (no <string.h> here:
// the port's prelude declares the C library's string functions with the game's types).
bool Take_Name(char *kept, char const *name, int size)
{
    bool differ = false;
    for (int k = 0; k < size - 1; ++k) {
        if (kept[k] != name[k]) differ = true;
        kept[k] = name[k];
        if (name[k] == 0) break;
    }
    return differ;
}
enum { kTriggers = 4096 };
struct Seen { TriggerClass *trigger; TriggerTypeClass *type; int tripped; };
Seen seen[kTriggers];
int seen_count = 0;
char scenario[0x104] = "";
int last_frame = -1;
bool timer_active = false;
bool house_done[64];

unsigned Mix(unsigned hash, unsigned value)
{
    for (int i = 0; i < 4; ++i) {
        hash ^= (value >> (8 * i)) & 0xFF;
        hash *= 16777619u;
    }
    return hash;
}
unsigned Mix_Text(unsigned hash, char const *text)
{
    for (; text && *text; ++text) hash = Mix(hash, (unsigned char)*text);
    return hash;
}
char const *Id_Of(TriggerTypeClass *type) { return type ? type->ID.Buffer : "?"; }
char const *Name_Of(TriggerTypeClass *type) { return type ? type->Name.Buffer : "?"; }

void Take_Triggers(bool report, int frame)
{
    static Seen now[kTriggers];
    int count = 0;
    for (int i = 0; i < Triggers.Count() && count < kTriggers; ++i) {
        TriggerClass *trigger = Triggers[i];
        if (trigger == 0) continue;
        now[count].trigger = trigger;
        now[count].type = trigger->Class;
        now[count].tripped = trigger->IsTripped;
        ++count;
    }
    if (report) {
        for (int i = 0; i < seen_count; ++i) {
            int k = 0;
            while (k < count && !(now[k].trigger == seen[i].trigger && now[k].type == seen[i].type)) ++k;
            if (k == count) {
                fprintf(stderr, "soak-trigger %d gone %s %s\n", frame, Id_Of(seen[i].type), Name_Of(seen[i].type));
            } else if (now[k].tripped != seen[i].tripped) {
                fprintf(stderr, "soak-trigger %d tripped %s %x\n", frame, Id_Of(now[k].type), (unsigned)now[k].tripped);
            }
        }
        for (int k = 0; k < count; ++k) {
            int i = 0;
            while (i < seen_count && !(now[k].trigger == seen[i].trigger && now[k].type == seen[i].type)) ++i;
            if (i == seen_count) fprintf(stderr, "soak-trigger %d new %s %s\n", frame, Id_Of(now[k].type), Name_Of(now[k].type));
        }
    }
    for (int k = 0; k < count; ++k) seen[k] = now[k];
    seen_count = count;
}

void Digest(int frame)
{
    unsigned hash = 2166136261u;
    int player = 0;
    for (int i = 0; i < Technos.Count(); ++i) {
        TechnoClass *techno = Technos[i];
        if (techno == 0) continue;
        TechnoTypeClass *type = techno->Techno_Type_Class();
        hash = Mix_Text(hash, type ? type->ID.Buffer : "?");
        hash = Mix(hash, techno->House ? techno->House->ArrayIndex : -1);
        hash = Mix(hash, techno->Coord.X);
        hash = Mix(hash, techno->Coord.Y);
        hash = Mix(hash, techno->Coord.Z);
        hash = Mix(hash, techno->Strength);
        hash = Mix(hash, techno->Mission);
        if (techno->House == PlayerPtr) ++player;
    }
    for (int i = 0; i < Houses.Count(); ++i) {
        if (Houses[i]) hash = Mix(hash, Houses[i]->Balance);
    }
    unsigned rng = 2166136261u;
    unsigned char const *bytes = (unsigned char const *)&Scen->RandomNumber;
    for (unsigned i = 0; i < sizeof Scen->RandomNumber; ++i) rng = Mix(rng, bytes[i]);
    Point2D view = TacticalMap ? TacticalMap->Get_Relative_Tactical_Position() : Point2D();
    fprintf(stderr, "soak-digest %d %08x technos %d player %d credits %d rng %08x view %d,%d\n", frame, hash, Technos.Count(),
            player, PlayerPtr ? PlayerPtr->Balance : -1, rng, view.X, view.Y);
}
void Dump(int frame)
{
    {
        // the logic list's objects in order (kind, and the type of animations): its loop skips the object after
        // one that leaves the list during its AI
        char kinds[4096];
        int n = snprintf(kinds, sizeof kinds, "soak-logic %d %d:", frame, Logic.Count());
        for (int i = 0; i < Logic.Count() && n < (int)sizeof kinds - 40; ++i) {
            ObjectClass *object = Logic[i];
            int k = object ? object->Kind_Of() : -1;
            if (k == 4) n += snprintf(kinds + n, sizeof kinds - n, " a:%s", ((AnimClass *)object)->Class ? ((AnimClass *)object)->Class->ID.Buffer : "?");
            else n += snprintf(kinds + n, sizeof kinds - n, " %d", k);
        }
        fprintf(stderr, "%s\n", kinds);
    }
    // RA2_NATIVE_SOAK_TRAP=<frame>,<index> (diagnostics, a debugger's watchpoint): at that frame the address of
    // techno #index's TarCom goes to soak_watch_address and the process stops with SIGTRAP.
    static int trap_frame = -2, trap_index = -1;
    if (trap_frame == -2) {
        trap_frame = -1;
        if (char const *env = getenv("RA2_NATIVE_SOAK_TRAP")) {
            trap_frame = atoi(env);
            for (char const *p = env; *p; ++p) if (*p == ',') trap_index = atoi(p + 1);
        }
    }
    if (frame == trap_frame && trap_index >= 0 && trap_index < Technos.Count() && Technos[trap_index]) {
        soak_watch_address = &Technos[trap_index]->TarCom;
        soak_watch_object = Technos[trap_index];
        fprintf(stderr, "soak-trap %p\n", (void *)soak_watch_address);
        __builtin_debugtrap();
    }
    for (int i = 0; i < Technos.Count(); ++i) {
        TechnoClass *techno = Technos[i];
        if (techno == 0) continue;
        TechnoTypeClass *type = techno->Techno_Type_Class();
        fprintf(stderr, "soak-techno %d #%d %s h%d %d,%d,%d s%d m%d k%d q%d st%d t%ld\n", frame, i, type ? type->ID.Buffer : "?",
                techno->House ? techno->House->ArrayIndex : -1, techno->Coord.X, techno->Coord.Y, techno->Coord.Z,
                techno->Strength, techno->Mission, techno->Kind_Of(), techno->MissionQueue, techno->Status,
                (long)techno->Timer);
        if (techno->TarCom) {
            AbstractClass *target = techno->TarCom;
            int k = target->Kind_Of();
            char const *id = "";
            if (k == 1 || k == 2 || k == 6 || k == 15) {
                TechnoTypeClass *tt = ((TechnoClass *)target)->Techno_Type_Class();
                id = tt ? tt->ID.Buffer : "?";
            }
            int weapon = techno->What_Weapon_Should_I_Use(target);
            fprintf(stderr, "soak-tarcom %d #%d k%d %s %p fire %d w%d air %d p%d%d%d%d%d\n", frame, i, k, id, (void *)target,
                    techno->unknown_slot217(target, weapon), weapon, target->In_Air(), techno->unknown_25c[0] != 0,
                    techno->unknown_25c[1] != 0, techno->unknown_25c[2] != 0, techno->unknown_25c[3] != 0,
                    techno->unknown_25c[4] != 0);
        }
        if (SpawnManagerClass *spawn = techno->SpawnManager) {
            char nodes[256] = "";
            int n = 0;
            for (int k = 0; k < spawn->SpawnedNodes.Count() && n < 200; ++k) {
                SpawnControl *node = spawn->SpawnedNodes[k];
                n += snprintf(nodes + n, sizeof nodes - n, " %d/%ld", node->Status, (long)node->Timer);
            }
            fprintf(stderr, "soak-spawn %d #%d status %d update %ld spawn %ld dest %d target %d nodes%s\n", frame, i,
                    spawn->Status, (long)spawn->UpdateTimer, (long)spawn->SpawnTimer, spawn->Destination != 0,
                    spawn->Target != 0, nodes);
        }
    }
}
}  // namespace

// CustomSeed from RA2_NATIVE_SOAK_SEED (Select_Game's seam, before any scenario starts).
void Native_Soak_Seed()
{
    char const *env = getenv("RA2_NATIVE_SOAK_SEED");
    if (env && *env) CustomSeed = (unsigned)atoi(env);
}

// After each Main_Loop pass (harness_skirmish.cpp's frame seam).
void Native_Harness_Soak(int loop_frame)
{
    static int period = -2;
    if (period == -2) {
        char const *env = getenv("RA2_NATIVE_SOAK");
        period = env ? atoi(env) : -1;
        if (period == 0) period = 500;
    }
    static int dump_from = -1, dump_to = -2;
    static bool dump_read = false;
    if (!dump_read) {
        dump_read = true;
        // RA2_NATIVE_SOAK_DUMP=<from>[-<to>]: every techno at those frames (a desync's first frame).
        if (char const *env = getenv("RA2_NATIVE_SOAK_DUMP")) {
            dump_from = dump_to = atoi(env);
            for (char const *p = env; *p; ++p) if (*p == '-') dump_to = atoi(p + 1);
        }
    }
    static int speed = -2;
    if (speed == -2) {
        char const *env = getenv("RA2_NATIVE_SOAK_SPEED");
        speed = env && *env ? atoi(env) : -1;
    }
    if (speed >= 0) {
        GameControlsShown = true;
        unknown_a40b18 = speed;
    }
    static int fps = -2;
    if (fps == -2) {
        // RA2_NATIVE_SOAK_FPS=<n> (diagnostics): the measured frame rate (FPSCounter_CurrentFrameRate 0x00A6EB1C,
        // which Sync_Delay sets once a second) held at n.
        char const *env = getenv("RA2_NATIVE_SOAK_FPS");
        fps = env && *env ? atoi(env) : -1;
    }
    if (fps >= 0) FPSCounter_CurrentFrameRate = (unsigned)fps;
    if (period < 0 || Scen == 0) return;
    int frame = Frame;
    bool renamed = Take_Name(scenario, Scen->FileName, sizeof scenario);
    static int end_frames = -2, end_mission = 0, missions = 0;
    if (end_frames == -2) {
        // RA2_NATIVE_SOAK_END=<frames>[@<n>]: the n-th scenario of the run (default the first) ends the
        // run once Frame reaches <frames>, or when it is won or lost: milestone soak-end (--run-to=soak-end).
        char const *env = getenv("RA2_NATIVE_SOAK_END");
        end_frames = env ? atoi(env) : -1;
        end_mission = 1;
        for (char const *p = env; p && *p; ++p) if (*p == '@') end_mission = atoi(p + 1);
    }
    if (frame < last_frame || renamed) {
        ++missions;
        Take_Triggers(false, frame);
        fprintf(stderr, "soak-mission %s loop %d triggers %d types %d\n", scenario, loop_frame, seen_count,
                TriggerTypes.Count());
        timer_active = false;
        for (int k = 0; k < 64; ++k) house_done[k] = false;
    } else if (frame != last_frame) {
        Take_Triggers(true, frame);
    }
    {
        // A scenario pause (the trigger action's, timed in wall-clock ticks: Scen->PauseTimer) and the hold
        // flag Main_Loop then runs only the view and the drawing under (+0x62C).
        static int paused = 0, held = 0;
        if ((int)Scen->IsGamePaused != paused || Scen->unknown_62c != held) {
            paused = Scen->IsGamePaused;
            held = Scen->unknown_62c;
            fprintf(stderr, "soak-pause %d paused %d held %d loop %d\n", frame, paused, held, loop_frame);
        }
    }
    if (frame != last_frame) {
        bool active = Scen->MissionTimer.Is_Active();
        if (active != timer_active) {
            fprintf(stderr, "soak-timer %d %s %ld\n", frame, active ? "started" : "stopped", (long)Scen->MissionTimer);
            timer_active = active;
        }
        for (int i = 0; i < Houses.Count() && i < 64; ++i) {
            HouseClass *house = Houses[i];
            if (house == 0 || house_done[i]) continue;
            if (house->IsDefeated) {
                house_done[i] = true;
                fprintf(stderr, "soak-house %d %d:%s defeated\n", frame, i, house->IniName.Buffer);
            }
        }
        if (frame % period == 0) Digest(frame);
        if (frame >= dump_from && frame <= dump_to) Dump(frame);
    }
    last_frame = frame;
    if (end_frames >= 0 && missions == end_mission) {
        char const *how = 0;
        if (player_wins_a35db1) how = "won";
        else if (player_loses_a40c78 || (PlayerPtr && PlayerPtr->IsDefeated)) how = "lost";
        else if (frame >= end_frames) how = "played";
        if (how) {
            char detail[0x140];
            snprintf(detail, sizeof detail, "%s %s at frame %d", scenario, how, frame);
            fprintf(stderr, "soak-end %s\n", detail);
            ra2port_progress("soak-end", detail);
            end_frames = -1;
        }
    }
}
