// Skirmish test harness orders (README "Skirmish harness";
// --skirmish-order=<kind>[@<frame>]). Test scaffolding, never game behaviour:
// at the given Main_Loop frame (default 10) the harness queues the orders a
// player's click would, as MEGAMISSION events (Queue_Mission 0x006233A0, the
// call TechnoClass slot 200 0x006CC2A0 ends in), for the human player's
// start units. Kinds: "move" (every infantry and vehicle moves 6 cells east),
// "deploy" (vehicles whose type deploys into a building, the MCV, unload:
// mission 16), "both" (the MCV deploys, the others move), "build" (the MCV
// deploys, then the build order of harness_build.cpp), "all" (both and build),
// "hunt" (the MCV deploys, every other unit gets the hunt mission 15, as the
// AI's attack teams do: scripted aggression for the menu-to-score loop,
// Scout1121), "rush" (every unit hunts, the MCV too: no base, so the game ends
// either way and shows the score screen), "win" (Scout1125, M15: the MCV
// deploys, the long build order of harness_build.cpp, then harness_army.cpp's
// tanks hunt the enemy; the start units wait and join the attack).
// RA2_NATIVE_ORDERS="<frame> goto <ID|*> <x> <y>; <frame> attack <ID|*> <x> <y>;
// <frame> hunt; <frame> capture <ID> <x> <y>; <frame> train <ID>; <frame> super <index> <x> <y>;
// <frame> grant <index>; <frame> super <index> enemy" (Scout1153, M19; super/grant Scout1170, M21: the human house's Supers[index];
// attack/capture: the building in cell x,y, mission 1 / 8; train: the barracks' cameo click;
// units -1 when the cell holds none) (Scout1153,
// M19; any game, the campaign too): at that frame the player's technos of type
// ID ('*': every infantry and vehicle) get a move order to cell x,y, or every
// combat unit but Tanya hunts, as a player's move click / the AI's attack teams
// queue them (Queue_Mission, mission 2 / 15). Test scaffolding: the scripted
// campaign run walks Tanya to Allied 1's fort.
#define RA2_COORD_AS_CELL   // CoordStruct::As_Cell (shared/coord.hpp)
#include "shared/display.hpp"     // before map_view.hpp (full DisplayClass)
#include "shared/map_view.hpp"
#include "shared/map_cell.hpp"
#include "shared/techno.hpp"
#include "shared/techno_type.hpp"
#include "shared/house.hpp"
#include "shared/target.hpp"
#include "shared/queue.hpp"
#include "shared/event.hpp"
#include "shared/infantry_type.hpp"
#include "shared/super.hpp"
#include "shared/super_weapon_type.hpp"
#include "shared/laser_draw.hpp"

#include <stdlib.h>

class TechnoClass;
extern DynamicVectorClass<TechnoClass *> Technos;   // 0x00A40C20 (shared/logic.hpp)

extern "C" {
const char *ra2port_skirmish_order(void);
int ra2port_skirmish_frames(void);
void ra2port_progress(const char *milestone, const char *detail);
}

namespace {
bool Same(char const *a, char const *b, int n)
{
    for (int i = 0; i < n; ++i) if (a[i] != b[i]) return false;
    return true;
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
void Append(char *to, char const *text)
{
    while (*to) ++to;
    while (*text) *to++ = *text++;
    *to = 0;
}
}

namespace {
bool Same_Type(char const *id, char const *want)
{
    if (want[0] == '*' && want[1] == 0) return true;
    for (; *id && *want; ++id, ++want) {
        char x = *id >= 'a' && *id <= 'z' ? *id - 32 : *id;
        char y = *want >= 'a' && *want <= 'z' ? *want - 32 : *want;
        if (x != y) return false;
    }
    return *id == 0 && *want == 0;
}
// One RA2_NATIVE_ORDERS command (see the file comment); returns the units ordered.
int Scripted_Order(char const *command)
{
    char word[8][24];
    int words = 0;
    for (char const *p = command; *p && words < 8;) {
        while (*p == ' ') ++p;
        if (!*p) break;
        int n = 0;
        while (*p && *p != ' ') { if (n < 23) word[words][n++] = *p; ++p; }
        word[words++][n] = 0;
    }
    if (words < 2) return 0;
    bool go = Same(word[1], "goto", 5) && words >= 5;
    bool hunt = Same(word[1], "hunt", 5);
    bool attack = (Same(word[1], "attack", 7) || Same(word[1], "capture", 8)) && words >= 5;
    int attack_mission = Same(word[1], "capture", 8) ? 8 : 1;   // Capture (an engineer into a building) or Attack
    if (Same(word[1], "train", 6) && words >= 3) {
        // The barracks' cameo click: PRODUCE (0x0E) of InfantryTypeClass (kind 0x10) by its ID.
        HouseClass *player = unknown_player_a35db4;
        for (int i = 0; player && i < InfantryTypes.Count(); ++i) {
            if (Same_Type(InfantryTypes[i]->ID.Buffer, word[2])) {
                OutList.Add(EventClass(player->ArrayIndex, 0x0E, 0x10, i, 0));
                return 1;
            }
        }
        return -1;
    }
    if (Same(word[1], "super", 6) && words == 4 && Same(word[3], "enemy", 6)) {
        // `super <index> enemy` (Scout1187, M23): the cell of an enemy building,
        // the next one at each such order (the skirmish AIs' bases in turn).
        static int next_target = 0;
        int buildings = 0;
        for (int i = 0; i < Technos.Count(); ++i) {
            TechnoClass *techno = Technos[i];
            if (techno && techno->House != unknown_player_a35db4 && techno->Kind_Of() == 6) ++buildings;
        }
        if (buildings == 0) return -1;
        int pick = (next_target++ * 7) % buildings;
        for (int i = 0; i < Technos.Count(); ++i) {
            TechnoClass *techno = Technos[i];
            if (techno && techno->House != unknown_player_a35db4 && techno->Kind_Of() == 6 && pick-- == 0) {
                Cell cell = techno->Center_Coord().As_Cell();
                char text[64] = "0 super ";
                Append(text, word[2]);
                Append(text, " ");
                Append_Int(text, cell.X);
                Append(text, " ");
                Append_Int(text, cell.Y);
                return Scripted_Order(text);
            }
        }
        return -1;
    }
    if (Same(word[1], "super", 6) && words >= 5) {
        // A super weapon (Scout1170, M21): the human house's Supers[index] is
        // granted once and charged as a campaign trigger's one-time grant does (Grant
        // 0x0069CCF0, Forced_Charge 0x0069D690), then fired at cell x,y with the
        // event the map click queues once the cameo is armed (SPECIAL_PLACE 0x12,
        // executed as HouseClass 0x004E68D0). An index past the list: -1.
        HouseClass *player = unknown_player_a35db4;
        int index = atoi(word[2]);
        if (player == 0 || index < 0 || index >= player->Supers.Count()) return -1;
        SuperClass *super = player->Supers[index];
        super->Grant(true, false, false);   // one time (a nuke then needs no silo), charged
        super->Forced_Charge(true);
        Cell at;
        at.X = (short)atoi(word[3]);
        at.Y = (short)atoi(word[4]);
        OutList.Add(EventClass(player->ArrayIndex, (unsigned char)0x12, index, at));
        static char names[8][20];
        static int named = 0;
        if (named < 8) {
            char *name = names[named++];
            name[0] = 0;
            Append(name, "harness-super-");
            Append_Int(name, index);
            ra2port_progress(name, super->Type ? super->Type->ID.Buffer : "?");
        }
        return 1;
    }
    if (Same(word[1], "laser", 6)) {
        // `laser` (Scout1202, M25; test scaffolding): four laser beams from
        // the player's first building (east, south, north-east and a steep one from 8 cells
        // up), drawn by LaserDrawClass::Draw_All through DSurface::Draw_Depth_Antialiased_Line
        // for 90 frames: covers the x-major, y-major and depth-major bodies.
        HouseClass *player = unknown_player_a35db4;
        if (player == 0 || player->Buildings.Count() == 0) return -1;
        CoordStruct from = player->Buildings[0]->Coord;
        from.Z += 200;
        static int const offsets[4][3] = {{2560, 0, 0}, {0, 2560, 0}, {1800, -1800, -100}, {256, 256, -2000}};
        for (int i = 0; i < 4; ++i) {
            CoordStruct start = from, end = from;
            if (i == 3) start.Z += 2048;
            end.X += offsets[i][0];
            end.Y += offsets[i][1];
            end.Z += offsets[i][2];
            // Beams 0 and 2 blink (inner and glow lines through slot 15), 1 and 3 have no outer
            // colour (the inner line through slot 15); a glowing steady beam would use slot 16.
            bool blinks = (i & 1) == 0;
            new LaserDrawClass(start, end, 0, false, RGBClass(255, 64, 64), blinks ? RGBClass(80, 80, 255) : RGBClass(0, 0, 0),
                               RGBClass(40, 40, 120), 90, blinks, true, 1.0f, 0.2f);
        }
        return 1;
    }
    if (Same(word[1], "grant", 6) && words >= 3) {
        // Grant and charge only (the cameo is then armed from the sidebar by a click).
        HouseClass *player = unknown_player_a35db4;
        int index = atoi(word[2]);
        if (player == 0 || index < 0 || index >= player->Supers.Count()) return -1;
        player->Supers[index]->Grant(false, false, false);
        player->Supers[index]->Forced_Charge(true);
        return 1;
    }
    ObjectClass *target = 0;
    if (attack) {
        Cell at;
        at.X = (short)atoi(word[3]);
        at.Y = (short)atoi(word[4]);
        target = Map[at].unknown_4717b0();      // the building in the cell
        if (target == 0) return -1;
    }
    int count = 0;
    for (int i = 0; i < Technos.Count(); ++i) {
        TechnoClass *techno = Technos[i];
        if (techno == 0 || techno->House != unknown_player_a35db4) continue;
        int kind = techno->Kind_Of();
        if (kind != 1 && kind != 0x0F) continue;     // UnitClass, InfantryClass
        TechnoTypeClass *type = techno->unknown_slot32();
        char const *id = type ? type->ID.Buffer : "";
        if (attack && Same_Type(id, word[2]) && !Same_Type(id, "TANY")) {
            Queue_Mission(TargetClass(techno), attack_mission, TargetClass(target), TargetClass());
            ++count;
        } else if (go && Same_Type(id, word[2])) {
            Cell cell;
            cell.X = (short)atoi(word[3]);
            cell.Y = (short)atoi(word[4]);
            Queue_Mission(TargetClass(techno), 2, TargetClass(), TargetClass(cell));
            ++count;
        } else if (hunt && !Same_Type(id, "TANY") && !Same_Type(id, "ENGINEER") && (type == 0 || type->DeploysInto == 0)) {
            Queue_Mission(TargetClass(techno), 15, TargetClass(), TargetClass());
            ++count;
        }
    }
    return count;
}
void Scripted_Orders(int frame)
{
    static char const *text = 0;
    static bool read = false;
    if (!read) { read = true; text = getenv("RA2_NATIVE_ORDERS"); }
    if (!text) return;
    for (char const *p = text; *p;) {
        char command[96];
        int n = 0;
        while (*p && *p != ';') { if (n < 95) command[n++] = *p; ++p; }
        command[n] = 0;
        if (*p == ';') ++p;
        char const *q = command;
        while (*q == ' ') ++q;
        if (atoi(q) != frame) continue;
        int count = Scripted_Order(q);
        char detail[160] = "";
        Append(detail, q);
        Append(detail, ": units ");
        Append_Int(detail, count);
        static char names[16][24];
        static int named = 0;
        if (named < 16) {
            char *name = names[named++];
            name[0] = 0;
            Append(name, "harness-orders-");
            Append_Int(name, named);
            ra2port_progress(name, detail);
        }
    }
}
}  // namespace

void Native_Harness_Build(int frame, bool army);   // harness_build.cpp
void Native_Harness_Army(int frame);               // harness_army.cpp

// Called by the frame seam (harness_skirmish.cpp) after each Main_Loop frame.
void Native_Harness_Orders(int frame)
{
    Scripted_Orders(frame);
    char const *order = ra2port_skirmish_order();
    if (order == 0) return;
    int at = 10;
    char const *sep = order;
    while (*sep && *sep != '@') ++sep;
    if (*sep == '@') {
        at = 0;
        for (char const *p = sep + 1; *p >= '0' && *p <= '9'; ++p) at = at * 10 + (*p - '0');
    }
    if (frame == ra2port_skirmish_frames()) {
        // The player's units at the end: kind, mission and cell (harness-units).
        char detail[400] = "";
        for (int i = 0; i < Technos.Count(); ++i) {
            TechnoClass *techno = Technos[i];
            if (techno == 0 || techno->House != unknown_player_a35db4) continue;
            Cell cell = techno->Center_Coord().As_Cell();
            Append_Int(detail, techno->Kind_Of());
            Append(detail, ":m");
            Append_Int(detail, techno->Mission);
            Append(detail, "@");
            Append_Int(detail, cell.X);
            Append(detail, ",");
            Append_Int(detail, cell.Y);
            Append(detail, " ");
            if (detail[300]) break;
        }
        ra2port_progress("harness-units", detail);
    }
    int len = (int)(sep - order);
    bool all = len == 3 && Same(order, "all", 3);
    bool win = len == 3 && Same(order, "win", 3);
    bool build = all || win || (len == 5 && Same(order, "build", 5));
    if (build) Native_Harness_Build(frame, win);
    if (win) Native_Harness_Army(frame);
    if (frame != at) return;
    bool move = (len == 4 && Same(order, "move", 4)) || (len == 4 && Same(order, "both", 4)) || all;
    bool rush = len == 4 && Same(order, "rush", 4);
    bool hunt = (len == 4 && Same(order, "hunt", 4)) || rush;
    bool deploy = (len == 6 && Same(order, "deploy", 6)) || (len == 4 && Same(order, "both", 4)) || build || (hunt && !rush);
    int moved = 0, deployed = 0;
    for (int i = 0; i < Technos.Count(); ++i) {
        TechnoClass *techno = Technos[i];
        if (techno == 0 || techno->House != unknown_player_a35db4) continue;
        int kind = techno->Kind_Of();
        if (kind != 1 && kind != 0x0F) continue;     // UnitClass, InfantryClass
        TechnoTypeClass *type = techno->unknown_slot32();
        if (deploy && kind == 1 && type != 0 && type->DeploysInto != 0) {
            Queue_Mission(TargetClass(techno), 16, TargetClass(), TargetClass());
            ++deployed;
            continue;
        }
        if (rush && kind == 1 && type != 0 && type->DeploysInto != 0) {
            // The MCV cannot hunt: it drives to the first enemy techno's cell.
            for (int k = 0; k < Technos.Count(); ++k) {
                TechnoClass *enemy = Technos[k];
                if (enemy == 0 || enemy->House == unknown_player_a35db4 || enemy->House == 0 || enemy->House->Is_Player_Control()) continue;
                Queue_Mission(TargetClass(techno), 2, TargetClass(), TargetClass(enemy->Center_Coord().As_Cell()));
                break;
            }
            ++moved;
            continue;
        }
        if (hunt) {
            Queue_Mission(TargetClass(techno), 15, TargetClass(), TargetClass());
            ++moved;
            continue;
        }
        if (move) {
            Cell cell = techno->Center_Coord().As_Cell();
            cell.X += 6;
            Queue_Mission(TargetClass(techno), 2, TargetClass(), TargetClass(cell));
            ++moved;
        }
    }
    char detail[96] = "frame ";
    Append_Int(detail, frame);
    Append(detail, ": moved ");
    Append_Int(detail, moved);
    Append(detail, ", deployed ");
    Append_Int(detail, deployed);
    ra2port_progress("harness-order", detail);
}
