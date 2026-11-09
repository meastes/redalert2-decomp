// Skirmish test harness: a build order (README "Skirmish harness";
// --skirmish-order=build or all). Test scaffolding, never game behaviour: the
// harness queues the events the sidebar and the map queue for a player's
// clicks, for the human player (Scout1111, native M12):
//   * a cameo click: PRODUCE (event 0x0E, kind 7 = BuildingTypeClass, the
//     type's index, naval) as SidebarClass::StripClass::SelectClass::Action
//     0x006809F0 queues it;
//   * the placement click on the map once the factory holds the finished
//     building: PLACE (event 0x0B with the cell) as the placement path does,
//     executed by HouseClass 0x004E6950 (Unlimbo at the cell).
// The order: a power plant (GAPOWR/NAPOWR), then a refinery (GAREFN/NAREFN),
// which brings its free harvester. Cells are tried around the construction
// yard until the building stands. Each step logs harness-build; the last
// frame logs harness-houses (buildings and technos per house).
#define RA2_COORD_AS_CELL   // CoordStruct::As_Cell (shared/coord.hpp)
#include "shared/building_class.hpp"
#include "shared/house.hpp"
#include "shared/factory_class_decl.hpp"
#include "shared/building_type_list.hpp"
#include "shared/event.hpp"
#include "shared/queue.hpp"

extern DynamicVectorClass<TechnoClass *> Technos;   // 0x00A40C20 (shared/logic.hpp)

extern "C" {
int ra2port_skirmish_frames(void);
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
    char digits[16];
    int n = 0;
    unsigned v = value < 0 ? -value : value;
    do { digits[n++] = '0' + v % 10; v /= 10; } while (v);
    char text[20];
    int k = 0;
    if (value < 0) text[k++] = '-';
    while (n) text[k++] = digits[--n];
    text[k] = 0;
    Append(to, text);
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

// The build list: each step is any of its names the player can build.
char const *const steps[][2] = {{"GAPOWR", "NAPOWR"}, {"GAREFN", "NAREFN"}};
int const step_count = sizeof(steps) / sizeof(steps[0]);
// Placement cells (top-left) around the construction yard's centre cell.
signed char const offsets[][2] = {{3, -2}, {3, 1}, {-4, -2}, {-4, 1}, {-1, 3}, {-1, -5}, {5, -2}, {5, 2},
                                  {-6, -2}, {2, 4}, {-3, 4}, {2, -6}};
int const offset_count = sizeof(offsets) / sizeof(offsets[0]);

int state = 0;          // 0 find the yard, 1 produce, 2 wait for the factory, 3 wait for the building
int step = 0;
int type_index = -1;
int try_offset = 0;
int wait_until = 0;
int buildings_before = 0;
Cell yard;

int Player_Buildings()
{
    return unknown_player_a35db4->Buildings.Count();
}

void Note(char const *what, int frame, char const *name, Cell const *cell)
{
    char detail[160] = "frame ";
    Append_Int(detail, frame);
    Append(detail, ": ");
    Append(detail, what);
    if (name) {
        Append(detail, " ");
        Append(detail, name);
    }
    if (cell) {
        Append(detail, " at ");
        Append_Int(detail, cell->X);
        Append(detail, ",");
        Append_Int(detail, cell->Y);
    }
    // One milestone name per note (the run log reports each name once).
    static int notes = 0;
    static char names[32][20];
    if (notes < 32) {
        char *name = names[notes++];
        name[0] = 0;
        Append(name, "harness-build-");
        Append_Int(name, notes);
        ra2port_progress(name, detail);
    }
}

void Houses_Summary(char const *milestone)
{
    char detail[400] = "";
    for (int h = 0; h < Houses.Count(); ++h) {
        HouseClass *house = Houses[h];
        if (house == 0) continue;
        int technos = 0;
        for (int i = 0; i < Technos.Count(); ++i) {
            if (Technos[i] && Technos[i]->Kind_Of() != 6 && Technos[i]->House == house) ++technos;
        }
        Append(detail, house == unknown_player_a35db4 ? "player" : "house");
        Append_Int(detail, h);
        Append(detail, house->Is_Player_Control() ? " (human" : " (computer");
        Append(detail, ")");
        Append(detail, ": buildings ");
        Append_Int(detail, house->Buildings.Count());
        Append(detail, ", units ");
        Append_Int(detail, technos);
        Append(detail, "; ");
        if (detail[340]) break;
    }
    ra2port_progress(milestone, detail);
}
}  // namespace

// Called by the order seam (harness_orders.cpp) after each Main_Loop frame when
// the order kind is build or all.
void Native_Harness_Build(int frame)
{
    HouseClass *player = unknown_player_a35db4;
    if (frame == 100) Houses_Summary("harness-houses-100");
    if (frame == ra2port_skirmish_frames()) Houses_Summary("harness-houses");
    if (player == 0 || step >= step_count || frame < wait_until) return;
    switch (state) {
    case 0:
        for (int i = 0; i < player->Buildings.Count(); ++i) {
            BuildingClass *building = player->Buildings[i];
            if (building && building->Type && building->Type->Factory == 7) {
                yard = building->Center_Coord().As_Cell();
                Note("construction yard", frame, building->Type->ID.Buffer, &yard);
                state = 1;
                break;
            }
        }
        return;
    case 1:
        type_index = -1;
        for (int i = 0; i < BuildingTypes.Count() && type_index < 0; ++i) {
            BuildingTypeClass *type = BuildingTypes[i];
            for (int k = 0; k < 2; ++k) {
                if (Same_Name(type->ID.Buffer, steps[step][k]) && player->Can_Build(type, false, true) > 0) {
                    type_index = i;
                    break;
                }
            }
        }
        if (type_index < 0) {
            wait_until = frame + 30;     // not buildable yet (prerequisites, power): ask again later
            return;
        }
        OutList.Add(EventClass(player->ArrayIndex, 0x0E, 7, type_index, 0));
        Note("produce", frame, BuildingTypes[type_index]->ID.Buffer, 0);
        state = 2;
        try_offset = 0;
        return;
    case 2: {
        FactoryClass *factory = player->unknown_func_4eb580(7, false, HouseClass::unknown_func_4eb550(7, type_index));
        if (factory == 0 || !factory->Is_Complete_004B9900()) return;
        if (try_offset >= offset_count) {
            Note("no cell found for", frame, BuildingTypes[type_index]->ID.Buffer, 0);
            step = step_count;
            return;
        }
        Cell cell = yard;
        cell.X += offsets[try_offset][0];
        cell.Y += offsets[try_offset][1];
        ++try_offset;
        buildings_before = Player_Buildings();
        OutList.Add(EventClass(player->ArrayIndex, 0x0B, 7, type_index, 0, cell));
        Note("place", frame, BuildingTypes[type_index]->ID.Buffer, &cell);
        state = 3;
        wait_until = frame + 20;
        return;
    }
    case 3:
        if (Player_Buildings() > buildings_before) {
            Note("placed", frame, BuildingTypes[type_index]->ID.Buffer, 0);
            ++step;
            state = 1;
            wait_until = frame + 10;
        } else {
            state = 2;                   // still in the factory: the next cell
        }
        return;
    }
}
