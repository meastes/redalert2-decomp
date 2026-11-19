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
// <frame> hunt; <frame> health; <frame> shoot <ID> (C22); <frame> capture <ID> <x> <y>; <frame> sabotage <ID> <x> <y>; <frame> train <ID>; <frame> super <index> <x> <y>;
// <frame> amove <ID|*> <x> <y> (attack-move, C3); <frame> grant <index>; <frame> super <index> enemy [<house>] [units]; <frame> super <index> home [<dx> <dy>] (C17); <frame> board <ID> <TRANSPORT>; <frame> unload <TRANSPORT>" (board/unload, train of a vehicle: Scout1295) (Scout1153, M19; super/grant Scout1170, M21: the human house's SuperWeapons[index];
// attack/capture: the building in cell x,y, mission 1 / 8 (`attack <ID> <TYPE>`: the first enemy
// object of that type; Tanya only when named); train: the barracks' cameo click;
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
#include "shared/unit_type.hpp"
#include "shared/super.hpp"
#include "shared/super_weapon_type.hpp"
#include "shared/laser_draw.hpp"
#include "shared/wave.hpp"
#include "shared/rules.hpp"

#include <stdio.h>
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
// `spawn <ID> [<dx> <dy>]` and `kill <ID>` (lane N4): a vehicle for the player beside its first techno, and the
// player's first techno of a type destroyed through Take_Damage (coverage_runs.py n4-deaths).
// A super weapon's target for `super <index> enemy`: a building, or with `units` a vehicle or
// infantry standing on the map (kind 1 or 15; aircraft and anything in limbo at cell 0,0 are not).
bool Super_Target(TechnoClass *techno, bool units)
{
    int kind = techno->Kind_Of();
    if (!units) return kind == 6;
    if (kind != 1 && kind != 15) return false;
    Cell cell = techno->Center_Coord().As_Cell();
    return cell.X != 0 || cell.Y != 0;
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
    if (Same(word[1], "list", 5)) {
        // `list` (Scout1223, test scaffolding): logs every techno not the player's as
        // `harness-list h<house> <ID> k<kind> @x,y` to stderr (a campaign map's targets).
        int listed = 0;
        for (int i = 0; i < Technos.Count(); ++i) {
            TechnoClass *techno = Technos[i];
            if (techno == 0 || techno->House == PlayerPtr) continue;
            TechnoTypeClass *type = techno->Techno_Type_Class();
            Cell cell = techno->Center_Coord().As_Cell();
            fprintf(stderr, "harness-list h%d %s k%d @%d,%d\n", techno->House ? techno->House->ArrayIndex : -1,
                    type ? type->ID.Buffer : "?", techno->Kind_Of(), cell.X, cell.Y);
            ++listed;
        }
        return listed;
    }
    if (Same(word[1], "health", 7)) {
        // `health` (lane C22, test scaffolding): logs every vehicle's and infantry's strength as
        // `harness-health h<house> <ID> k<kind> s<strength> @x,y #<index>` (weapon damage checks;
        // index: the object's place in Technos).
        int listed = 0;
        for (int i = 0; i < Technos.Count(); ++i) {
            TechnoClass *techno = Technos[i];
            if (techno == 0) continue;
            int kind = techno->Kind_Of();
            if (kind != 1 && kind != 15) continue;
            TechnoTypeClass *type = techno->Techno_Type_Class();
            Cell cell = techno->Center_Coord().As_Cell();
            fprintf(stderr, "harness-health h%d %s k%d s%d @%d,%d #%d &%p\n", techno->House ? techno->House->ArrayIndex : -1,
                    type ? type->ID.Buffer : "?", kind, techno->Strength, cell.X, cell.Y, i, (void *)techno);
            ++listed;
        }
        return listed;
    }
    if (Same(word[1], "money", 6)) {
        // `money` (lane P2, test scaffolding): logs `harness-money <credits>`, the player's
        // Available_Money (a harvester's delivery raises it; ctest native-basics).
        if (PlayerPtr == 0) return -1;
        fprintf(stderr, "harness-money %ld\n", (long)PlayerPtr->Available_Money());
        return 1;
    }
    if ((Same(word[1], "scout", 6) && words >= 5) || Same(word[1], "mapped", 7)) {
        // `scout <ID> <dx> <dy>` (lane P2, test scaffolding): up to three of the player's units of
        // type ID are sent max(|dx|, |dy|) cells away (the move a click queues), each in another of
        // the eight directions, the ones whose destination's 7x7 cells are all on the map
        // (MapClass::In_Radar) and reachable (MapClass::Zone of the type's MovementZone, lane Y16), least
        // mapped first; the player's mapped cells (CellClass +0x12C
        // bit 3, which Unshroud sets) around each start are kept. `mapped` logs, for each scout,
        // `harness-mapped s<n> moved <cells> now <mapped>/<cells> before <mapped>`: the 7x7 cells
        // around where it stands now, mapped now and when it was sent (ctest native-basics: a unit
        // that moves lifts the shroud around it).
        enum { R = 20, W = 2 * R + 1, SCOUTS = 3 };
        static TechnoClass *scout[SCOUTS];
        static Cell origin[SCOUTS];
        static unsigned char before[SCOUTS][W][W];
        static int scouts = 0;
        if (Same(word[1], "scout", 6)) {
            int reach = abs(atoi(word[3])) > abs(atoi(word[4])) ? abs(atoi(word[3])) : abs(atoi(word[4]));
            static int const dirs[8][2] = {{1, 0}, {0, 1}, {-1, 0}, {0, -1}, {1, 1}, {-1, 1}, {-1, -1}, {1, -1}};
            bool used[8] = {};
            for (int i = 0; i < Technos.Count() && scouts < SCOUTS; ++i) {
                TechnoClass *techno = Technos[i];
                if (techno == 0 || techno->House != PlayerPtr || techno->Techno_Type_Class() == 0) continue;
                if (!Same_Type(techno->Techno_Type_Class()->ID.Buffer, word[2])) continue;
                Cell at = techno->Center_Coord().As_Cell();
                // Only a destination the scout can reach (the same movement zone as where it stands): a
                // destination in another zone (a cliff top, water) is refused by the path finder and the
                // scout never moves. The start position is random, so before lane Y16 a game whose three
                // nearest-to-unmapped destinations were all unreachable failed the shroud check.
                int mzone = techno->Techno_Type_Class()->MovementZone;
                int zone = Map.Zone(at, mzone, false);
                int best = -1, best_mapped = 0;
                for (int d = 0; d < 8; ++d) {
                    if (used[d]) continue;
                    int mapped = 0;
                    bool inside = true;
                    for (int dy = -3; dy <= 3 && inside; ++dy) {
                        for (int dx = -3; dx <= 3 && inside; ++dx) {
                            Cell cell;
                            cell.X = (short)(at.X + dirs[d][0] * reach + dx);
                            cell.Y = (short)(at.Y + dirs[d][1] * reach + dy);
                            if (!Map.In_Radar(cell)) inside = false;
                            else if (Map[cell].unknown_12c & 0x8) ++mapped;
                        }
                    }
                    if (inside) {
                        Cell to;
                        to.X = (short)(at.X + dirs[d][0] * reach);
                        to.Y = (short)(at.Y + dirs[d][1] * reach);
                        inside = Map.Zone(to, mzone, false) == zone;
                    }
                    if (inside && (best < 0 || mapped < best_mapped)) best = d, best_mapped = mapped;
                }
                if (best < 0) continue;
                used[best] = true;
                for (int y = 0; y < W; ++y) {
                    for (int x = 0; x < W; ++x) {
                        Cell cell;
                        cell.X = (short)(at.X - R + x);
                        cell.Y = (short)(at.Y - R + y);
                        before[scouts][y][x] = Map.In_Radar(cell) && (Map[cell].unknown_12c & 0x8) ? 1 : 0;
                    }
                }
                Cell to;
                to.X = (short)(at.X + dirs[best][0] * reach);
                to.Y = (short)(at.Y + dirs[best][1] * reach);
                Queue_Mission(TargetClass(techno), 2, TargetClass(), TargetClass(to));
                fprintf(stderr, "harness-scout s%d #%d @%d,%d -> @%d,%d (%d of 49 mapped, zone %d)\n", scouts, i, at.X, at.Y,
                        to.X, to.Y, best_mapped, zone);
                scout[scouts] = techno;
                origin[scouts++] = at;
            }
            return scouts ? scouts : -1;
        }
        int listed = 0;
        for (int s = 0; s < scouts; ++s) {
            bool alive = false;     // still in Technos (it may have died)
            for (int i = 0; i < Technos.Count() && !alive; ++i) alive = Technos[i] == scout[s];
            if (!alive) continue;
            Cell at = scout[s]->Center_Coord().As_Cell();
            int moved = abs(at.X - origin[s].X) > abs(at.Y - origin[s].Y) ? abs(at.X - origin[s].X) : abs(at.Y - origin[s].Y);
            int now = 0, was = 0, cells = 0;
            for (int dy = -3; dy <= 3; ++dy) {
                for (int dx = -3; dx <= 3; ++dx) {
                    Cell cell;
                    cell.X = (short)(at.X + dx);
                    cell.Y = (short)(at.Y + dy);
                    if (!Map.In_Radar(cell)) continue;
                    ++cells;
                    if (Map[cell].unknown_12c & 0x8) ++now;
                    int x = cell.X - origin[s].X + R, y = cell.Y - origin[s].Y + R;
                    if (x >= 0 && x < W && y >= 0 && y < W) was += before[s][y][x];
                }
            }
            fprintf(stderr, "harness-mapped s%d moved %d now %d/%d before %d\n", s, moved, now, cells, was);
            ++listed;
        }
        return listed;
    }
    if (Same(word[1], "spawn", 6) && words >= 3) {
        // `spawn <ID> [<dx> <dy>]` (lane N4, test scaffolding): a vehicle of type ID for the player, on the first free
        // cell in rings around dx,dy cells from the player's first techno (UnitTypeClass::Create_One_Of, Unlimbo).
        // Logs `harness-spawn <ID> @x,y &<addr>`.
        UnitTypeClass const *type = 0;
        for (int i = 0; i < UnitTypes.Count() && type == 0; ++i) {
            if (Same_Type(UnitTypes[i]->ID.Buffer, word[2])) type = UnitTypes[i];
        }
        TechnoClass *home = 0;
        for (int i = 0; i < Technos.Count() && home == 0; ++i) {
            if (Technos[i] && Technos[i]->House == PlayerPtr) home = Technos[i];
        }
        if (type == 0 || home == 0) return -1;
        ObjectClass *object = type->Create_One_Of(PlayerPtr);
        if (object == 0) return -1;
        Cell base = home->Center_Coord().As_Cell();
        int dx = words >= 5 ? atoi(word[3]) : 3, dy = words >= 5 ? atoi(word[4]) : 0;
        for (int ring = 0; ring <= 8; ++ring) {
            for (int y = -ring; y <= ring; ++y) {
                for (int x = -ring; x <= ring; ++x) {
                    if (abs(x) != ring && abs(y) != ring) continue;
                    CoordStruct at;
                    at.X = (base.X + dx + x) * 256 + 128;
                    at.Y = (base.Y + dy + y) * 256 + 128;
                    at.Z = 0;
                    at.Z = Map.Get_Height_GL(at);
                    if (object->Unlimbo(&at, 0)) {
                        fprintf(stderr, "harness-spawn %s @%d,%d &%p\n", word[2], at.X / 256, at.Y / 256, (void *)object);
                        return 1;
                    }
                }
            }
        }
        fprintf(stderr, "harness-spawn: no room for %s\n", word[2]);
        return -1;
    }
    if (Same(word[1], "kill", 5) && words >= 3) {
        // `kill <ID>` (lane N4, test scaffolding): the player's first techno of type ID takes its full strength in
        // damage from the C4 warhead (TechnoClass::Take_Damage, forced): it dies the way it dies in battle (a
        // demolition truck's explosion, a vehicle's wreck). Logs `harness-kill <ID> &<addr>`.
        for (int i = 0; i < Technos.Count(); ++i) {
            TechnoClass *techno = Technos[i];
            if (techno == 0 || techno->House != PlayerPtr || techno->Techno_Type_Class() == 0) continue;
            if (!Same_Type(techno->Techno_Type_Class()->ID.Buffer, word[2])) continue;
            int damage = techno->Strength;
            fprintf(stderr, "harness-kill %s &%p\n", word[2], (void *)techno);
            techno->Take_Damage(&damage, 0, Rules->C4Warhead, 0, true, 0);
            return 1;
        }
        return -1;
    }
    if (Same(word[1], "shoot", 6) && words >= 3) {
        // `shoot <ID>` (lane C22, test scaffolding): the player's first unit of type ID attacks the
        // player's second one (mission Attack with it as the target, the order a force-fire click
        // queues), so a weapon's damage is seen at once, with no enemy to find. Logs
        // `harness-shoot #<attacker> #<target>` (Technos indexes, as `health` prints them).
        int first = -1, second = -1;
        for (int i = 0; i < Technos.Count() && second < 0; ++i) {
            TechnoClass *techno = Technos[i];
            if (techno == 0 || techno->House != PlayerPtr || techno->Techno_Type_Class() == 0) continue;
            if (!Same_Type(techno->Techno_Type_Class()->ID.Buffer, word[2])) continue;
            if (first < 0) first = i;
            else second = i;
        }
        if (second < 0) return -1;
        Queue_Mission(TargetClass(Technos[first]), 1, TargetClass(Technos[second]), TargetClass());
        fprintf(stderr, "harness-shoot #%d #%d &%p &%p\n", first, second, (void *)Technos[first], (void *)Technos[second]);
        return 1;
    }
    // `amove <ID|*> <x> <y>` (C3): an attack-move, the MEGAMISSION 0x1D a Ctrl+Shift ground
    // click queues (TechnoClass slot 200 0x006CC2A0 turns a move into 0x1D while the keys are down).
    bool amove = Same(word[1], "amove", 6) && words >= 5;
    bool go = (Same(word[1], "goto", 5) || amove) && words >= 5;
    bool hunt = Same(word[1], "hunt", 5);
    bool attack = (Same(word[1], "attack", 7) || Same(word[1], "capture", 8) || Same(word[1], "sabotage", 9)) && words >= 5;
    // `attack <ID> <TYPE>` (Scout1209 r6): the first enemy object of that type, wherever it is
    // (Allied 1's Dreadnoughts for Tanya).
    bool attack_type = (Same(word[1], "attack", 7) || Same(word[1], "sabotage", 9)) && words == 4;
    if (attack_type) attack = true;
    // Capture (an engineer into a building), Sabotage (Tanya's C4: the building is the destination,
    // as a player's click gives it; Scout1231) or Attack.
    int attack_mission = Same(word[1], "capture", 8) ? 8 : Same(word[1], "sabotage", 9) ? 17 : 1;
    if (Same(word[1], "train", 6) && words >= 3) {
        // The barracks' cameo click: PRODUCE (0x0E) of InfantryTypeClass (kind 0x10) by its ID.
        HouseClass *player = PlayerPtr;
        for (int i = 0; player && i < InfantryTypes.Count(); ++i) {
            if (Same_Type(InfantryTypes[i]->ID.Buffer, word[2])) {
                OutList.Add(EventClass(player->ArrayIndex, 0x0E, 0x10, i, 0));
                return 1;
            }
        }
        // A vehicle (Scout1295): the war factory's cameo click, PRODUCE of UnitTypeClass
        // (kind 0x28), as harness_army.cpp queues its tanks.
        for (int i = 0; player && i < UnitTypes.Count(); ++i) {
            if (Same_Type(UnitTypes[i]->ID.Buffer, word[2])) {
                OutList.Add(EventClass(player->ArrayIndex, 0x0E, 0x28, i, 0));
                return 1;
            }
        }
        return -1;
    }
    if ((Same(word[1], "board", 6) && words >= 4) || (Same(word[1], "unload", 7) && words >= 3)) {
        // `board <ID> <TRANSPORT>` / `unload <TRANSPORT>` (Scout1295, test scaffolding): the
        // player's units of type ID enter the first of the player's transports of that type
        // (mission Enter 7, the transport as the destination, as the click on it queues);
        // the transports unload (mission 16, the deploy key's order).
        HouseClass *player = PlayerPtr;
        bool board = Same(word[1], "board", 6);
        TechnoClass *transport = 0;
        for (int i = 0; board && i < Technos.Count() && transport == 0; ++i) {
            TechnoClass *techno = Technos[i];
            if (techno && techno->House == player && techno->Techno_Type_Class()
                && Same_Type(techno->Techno_Type_Class()->ID.Buffer, word[3])) transport = techno;
        }
        if (board && transport == 0) return -1;
        int count = 0;
        for (int i = 0; i < Technos.Count(); ++i) {
            TechnoClass *techno = Technos[i];
            if (techno == 0 || techno->House != player || techno->Techno_Type_Class() == 0) continue;
            if (!Same_Type(techno->Techno_Type_Class()->ID.Buffer, word[2])) continue;
            if (board) Queue_Mission(TargetClass(techno), 7, TargetClass(), TargetClass(transport));
            else Queue_Mission(TargetClass(techno), 16, TargetClass(), TargetClass());
            ++count;
        }
        return count;
    }
    if (Same(word[1], "super", 6) && (words >= 4 && words <= 6) && Same(word[3], "enemy", 6)) {
        // `super <index> enemy [<house>] [units]` (Scout1187, M23): the cell of an enemy building,
        // the next one at each such order (the skirmish AIs' bases in turn); with a house
        // index, that house's buildings only (Scout1209 r17: a campaign's civilian city is
        // not a target, Allied 1 is lost when it falls); with `units` (Scout1236) that house's
        // vehicles and infantry on the map instead (Soviet 2's destroyers at sea, which no land
        // unit reaches), each order the next one.
        static int next_target = 0;
        int only = words >= 5 ? atoi(word[4]) : -1;
        bool units = words == 6 && Same(word[5], "units", 6);
        int buildings = 0;
        for (int i = 0; i < Technos.Count(); ++i) {
            TechnoClass *techno = Technos[i];
            if (techno && techno->House != PlayerPtr && Super_Target(techno, units)
                && (only < 0 || (techno->House && techno->House->ArrayIndex == only))) ++buildings;
        }
        if (buildings == 0) return -1;
        int pick = (next_target++ * 7) % buildings;
        for (int i = 0; i < Technos.Count(); ++i) {
            TechnoClass *techno = Technos[i];
            if (techno && techno->House != PlayerPtr && Super_Target(techno, units)
                && (only < 0 || (techno->House && techno->House->ArrayIndex == only)) && pick-- == 0) {
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
    if (Same(word[1], "super", 6) && (words == 4 || words == 6) && Same(word[3], "home", 5)) {
        // `super <index> home [<dx> <dy>]` (C17, HD vehicles: a paradrop's cargo plane over the
        // player's own view): the cell of the player's first building, else first unit, moved by
        // dx, dy cells.
        HouseClass *player = PlayerPtr;
        if (player == 0) return -1;
        TechnoClass *home = 0;
        for (int i = 0; i < Technos.Count() && home == 0; ++i)
            if (Technos[i] && Technos[i]->House == player && Technos[i]->Kind_Of() == 6) home = Technos[i];
        for (int i = 0; i < Technos.Count() && home == 0; ++i)
            if (Technos[i] && Technos[i]->House == player && Technos[i]->Kind_Of() == 1) home = Technos[i];
        if (home == 0) return -1;
        Cell cell = home->Center_Coord().As_Cell();
        char text[64] = "0 super ";
        Append(text, word[2]);
        Append(text, " ");
        Append_Int(text, cell.X + (words == 6 ? atoi(word[4]) : 0));
        Append(text, " ");
        Append_Int(text, cell.Y + (words == 6 ? atoi(word[5]) : 0));
        return Scripted_Order(text);
    }
    if (Same(word[1], "super", 6) && words >= 5) {
        // A super weapon (Scout1170, M21): the human house's SuperWeapons[index] is
        // granted once and charged as a campaign trigger's one-time grant does (Grant
        // 0x0069CCF0, Forced_Charge 0x0069D690), then fired at cell x,y with the
        // event the map click queues once the cameo is armed (SPECIAL_PLACE 0x12,
        // executed as HouseClass 0x004E68D0). An index past the list: -1.
        HouseClass *player = PlayerPtr;
        int index = atoi(word[2]);
        if (player == 0 || index < 0 || index >= player->SuperWeapons.Count()) return -1;
        SuperClass *super = player->SuperWeapons[index];
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
        HouseClass *player = PlayerPtr;
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
    if (Same(word[1], "wave", 5)) {
        // `wave` (Scout1208, M26; test scaffolding): a sonic wave (type 0, the dolphin's)
        // from the player's first unit at its first building, the unit's target set to the
        // building as a firing unit's would be (the wave follows while the target holds),
        // and a laser wave (type 1, same owner) from the building 10 cells east: covers
        // WaveClass::Draw_Sonic 0x007225B0 and Draw_Laser 0x00722CB0.
        HouseClass *player = PlayerPtr;
        if (player == 0 || player->Buildings.Count() == 0) return -1;
        ObjectClass *building = player->Buildings[0];
        TechnoClass *unit = 0;
        for (int i = 0; i < Technos.Count() && unit == 0; ++i) {
            TechnoClass *techno = Technos[i];
            if (techno && techno->House == player && techno->Kind_Of() == 1) unit = techno;
        }
        if (unit == 0) return -1;
        unit->TarCom = building;
        new WaveClass(unit->Coord, building->Coord, unit, 0, building);
        CoordStruct end = building->Coord;
        end.X += 2560;
        new WaveClass(building->Coord, end, unit, 1, 0);     // a wave's owner is never null (Draw_NonMagnetic reads its facing)
        return 1;
    }
    if (Same(word[1], "grant", 6) && words >= 3) {
        // Grant and charge only (the cameo is then armed from the sidebar by a click).
        HouseClass *player = PlayerPtr;
        int index = atoi(word[2]);
        if (player == 0 || index < 0 || index >= player->SuperWeapons.Count()) return -1;
        player->SuperWeapons[index]->Grant(false, false, false);
        player->SuperWeapons[index]->Forced_Charge(true);
        return 1;
    }
    ObjectClass *target = 0;
    if (attack_type) {
        for (int i = 0; i < Technos.Count() && target == 0; ++i) {
            TechnoClass *techno = Technos[i];
            if (techno == 0 || techno->House == PlayerPtr || techno->Techno_Type_Class() == 0) continue;
            if (Same_Type(techno->Techno_Type_Class()->ID.Buffer, word[3])) target = techno;
        }
        if (target == 0) return -1;
    } else if (attack) {
        Cell at;
        at.X = (short)atoi(word[3]);
        at.Y = (short)atoi(word[4]);
        target = Map[at].Cell_Building();      // the building in the cell
        if (target == 0) return -1;
    }
    int count = 0;
    for (int i = 0; i < Technos.Count(); ++i) {
        TechnoClass *techno = Technos[i];
        if (techno == 0 || techno->House != PlayerPtr) continue;
        int kind = techno->Kind_Of();
        if (kind != 1 && kind != 0x0F) continue;     // UnitClass, InfantryClass
        TechnoTypeClass *type = techno->Techno_Type_Class();
        char const *id = type ? type->ID.Buffer : "";
        if (attack && Same_Type(id, word[2]) && (!Same_Type(id, "TANY") || Same_Type(word[2], "TANY"))) {
            if (attack_mission == 17) Queue_Mission(TargetClass(techno), attack_mission, TargetClass(), TargetClass(target));
            else Queue_Mission(TargetClass(techno), attack_mission, TargetClass(target), TargetClass());
            ++count;
        } else if (go && Same_Type(id, word[2])) {
            Cell cell;
            cell.X = (short)atoi(word[3]);
            cell.Y = (short)atoi(word[4]);
            Queue_Mission(TargetClass(techno), amove ? 0x1D : 2, TargetClass(), TargetClass(cell));
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
        fprintf(stderr, "harness-order %s\n", detail);   // the app's log too (Scout1288)
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
            if (techno == 0 || techno->House != PlayerPtr) continue;
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
        if (techno == 0 || techno->House != PlayerPtr) continue;
        int kind = techno->Kind_Of();
        if (kind != 1 && kind != 0x0F) continue;     // UnitClass, InfantryClass
        TechnoTypeClass *type = techno->Techno_Type_Class();
        if (deploy && kind == 1 && type != 0 && type->DeploysInto != 0) {
            Queue_Mission(TargetClass(techno), 16, TargetClass(), TargetClass());
            ++deployed;
            continue;
        }
        if (rush && kind == 1 && type != 0 && type->DeploysInto != 0) {
            // The MCV cannot hunt: it drives to the first enemy techno's cell.
            for (int k = 0; k < Technos.Count(); ++k) {
                TechnoClass *enemy = Technos[k];
                if (enemy == 0 || enemy->House == PlayerPtr || enemy->House == 0 || enemy->House->Is_Player_Control()) continue;
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

// RA2_NATIVE_ALLY_LOG=1 (Scout1288, test diagnostics): whenever the alliances between the
// playing houses change, or a house is defeated, a line on stderr:
//   alliances frame <n>: <i>:<name>[ defeated] ... allies <i>><j> ...
// (i>j: house i counts house j as an ally, HouseClass::Is_Ally 0x004E5540), so a scripted
// game can confirm an alliance the diplomacy dialog formed and the end of a house.
// Not game behaviour.
#include "shared/house_type.hpp"
void Native_Alliance_Log(int frame)
{
    static int on = -1;
    if (on < 0) on = getenv("RA2_NATIVE_ALLY_LOG") ? 1 : 0;
    if (!on) return;
    static char last[512];
    char now[512];
    now[0] = 0;
    int count = Houses.Count();
    size_t n = 0;
    for (int i = 0; i < count && i < 16 && n + 64 < sizeof now; ++i) {
        HouseClass *h = Houses[i];
        if (!h || !h->Type || !h->Type->Multiplay) continue;
        n += snprintf(now + n, sizeof now - n, " %d:%s%s", i, h->IniName.Buffer, h->IsDefeated ? " defeated" : "");
    }
    n += snprintf(now + n, sizeof now - n, " allies");
    for (int i = 0; i < count && i < 16; ++i) {
        HouseClass *h = Houses[i];
        if (!h || !h->Type || !h->Type->Multiplay) continue;
        for (int j = 0; j < count && j < 16 && n + 8 < sizeof now; ++j) {
            HouseClass *o = Houses[j];
            if (i == j || !o || !o->Type || !o->Type->Multiplay) continue;
            if (h->Is_Ally(o)) n += snprintf(now + n, sizeof now - n, " %d>%d", i, j);
        }
    }
    bool same = true;
    for (size_t k = 0; same && (now[k] || last[k]); ++k) same = now[k] == last[k];
    if (!same) {
        fprintf(stderr, "alliances frame %d:%s\n", frame, now);
        for (size_t k = 0; (last[k] = now[k]) != 0; ++k) {}
    }
}
