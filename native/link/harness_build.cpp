// Skirmish test harness: a build order (README "Skirmish harness";
// --skirmish-order=build, all or win). Test scaffolding, never game behaviour: the
// harness queues the events the sidebar and the map queue for a player's
// clicks, for the human player (Scout1111, native M12):
//   * a cameo click: PRODUCE (event 0x0E, kind 7 = BuildingTypeClass, the
//     type's index, naval) as SidebarClass::StripClass::SelectClass::Action
//     0x006809F0 queues it;
//   * the placement click on the map once the factory holds the finished
//     building: PLACE (event 0x0B with the cell) as the placement path does,
//     executed by HouseClass 0x004E6950 (Unlimbo at the cell).
// The order: a power plant (GAPOWR/NAPOWR), then a refinery (GAREFN/NAREFN),
// which brings its free harvester. The placement cell is the nearest one to
// the construction yard that the original's own placement tests accept:
// DisplayClass::Passes_Proximity_Check 0x004991D0 (the foundation touches one
// of the house's base buildings) and CellClass::Is_Clear_To_Build 0x004718B0
// (draft) for every cell of the type's occupy list (ObjectTypeClass slot 36),
// as the placement cursor checks them (Scout1115, M13); a cell whose PLACE
// event did not take is skipped next time. Each step logs harness-build; the last
// frame logs harness-houses (buildings and technos per house).
#define RA2_COORD_AS_CELL   // CoordStruct::As_Cell (shared/coord.hpp)
#include "shared/display.hpp"     // before map_view.hpp (full DisplayClass)
#include "shared/map_view.hpp"
#include "shared/map_cell.hpp"
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

// The build list: each step is any of its names the player can build. The
// order kind win (Scout1125, M15) goes on to a barracks, a war factory, a
// second refinery (its harvester) and a second power plant (harness_army.cpp
// builds tanks once the war factory stands).
char const *const steps[][2] = {{"GAPOWR", "NAPOWR"}, {"GAREFN", "NAREFN"}, {"GAPILE", "NAHAND"}, {"GAWEAP", "NAWEAP"},
    {"GAREFN", "NAREFN"}, {"GAPOWR", "NAPOWR"}};
int const short_step_count = 2;
int step_count = short_step_count;
Cell yard;              // the construction yard's centre cell

// Cells already tried for the current step (a PLACE that did not take).
Cell tried[64];
int tried_count = 0;
int const search_radius = 12;

bool Was_Tried(Cell const &cell)
{
    for (int i = 0; i < tried_count; ++i) {
        if (tried[i].X == cell.X && tried[i].Y == cell.Y) return true;
    }
    return false;
}

// Whether the original's placement tests accept the type's top-left cell.
bool Legal_Cell(BuildingTypeClass *type, HouseClass *house, Cell const &cell)
{
    Cell const *list = type->Occupy_List(true);
    if (!Map.Passes_Proximity_Check_004991D0(type, house->ArrayIndex, list, cell)) return false;
    if (list == 0) return false;
    for (; list->X != 0x7FFF || list->Y != 0x7FFF; ++list) {
        Cell at = cell;
        at.X += list->X;
        at.Y += list->Y;
        if (at.X < 1 || at.Y < 1 || at.X > 510 || at.Y > 510) return false;
        if (!Map[at].Is_Clear_To_Build(type->SpeedType, type, house)) return false;
    }
    return true;
}

// The legal cell nearest the construction yard (rings of growing radius).
bool Find_Cell(BuildingTypeClass *type, HouseClass *house, Cell &found)
{
    for (int r = 1; r <= search_radius; ++r) {
        for (int dy = -r; dy <= r; ++dy) {
            for (int dx = -r; dx <= r; ++dx) {
                if (dx != -r && dx != r && dy != -r && dy != r) continue;
                Cell cell = yard;
                cell.X += dx;
                cell.Y += dy;
                if (Was_Tried(cell) || !Legal_Cell(type, house, cell)) continue;
                found = cell;
                return true;
            }
        }
    }
    return false;
}

int state = 0;          // 0 find the yard, 1 produce, 2 wait for the factory, 3 wait for the building
int step = 0;
int type_index = -1;
int wait_until = 0;
int buildings_before = 0;

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
        if (house == unknown_player_a35db4) {
            Append(detail, ", money ");
            Append_Int(detail, (int)house->Available_Money());
        }
        Append(detail, "; ");
        if (detail[340]) break;
    }
    ra2port_progress(milestone, detail);
}
}  // namespace

// The house summary for harness_army.cpp's periodic notes.
void Native_Harness_Houses(char const *milestone)
{
    Houses_Summary(milestone);
}

// Called by the order seam (harness_orders.cpp) after each Main_Loop frame when
// the order kind is build, all or win (army: the long build list).
void Native_Harness_Build(int frame, bool army)
{
    HouseClass *player = unknown_player_a35db4;
    if (army && frame == 1) step_count = sizeof(steps) / sizeof(steps[0]);
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
        tried_count = 0;
        return;
    case 2: {
        FactoryClass *factory = player->unknown_func_4eb580(7, false, HouseClass::unknown_func_4eb550(7, type_index));
        if (factory == 0 || !factory->Is_Complete_004B9900()) return;
        Cell cell;
        if (tried_count >= 64 || !Find_Cell(BuildingTypes[type_index], player, cell)) {
            Note("no cell found for", frame, BuildingTypes[type_index]->ID.Buffer, 0);
            step = step_count;
            return;
        }
        tried[tried_count++] = cell;
        buildings_before = Player_Buildings();
        OutList.Add(EventClass(player->ArrayIndex, 0x0B, 7, type_index, 0, cell));
        Note("place", frame, BuildingTypes[type_index]->ID.Buffer, &cell);
        state = 3;
        wait_until = frame + 20;
        return;
    }
    case 3: {
        // Placed when the player's building count rose, or when the house's
        // factory no longer holds the finished building (Scout1153: in a campaign
        // the count can stay flat while other buildings of the player die).
        FactoryClass *factory = player->unknown_func_4eb580(7, false, HouseClass::unknown_func_4eb550(7, type_index));
        if (Player_Buildings() > buildings_before || factory == 0 || !factory->Is_Complete_004B9900()) {
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
}
