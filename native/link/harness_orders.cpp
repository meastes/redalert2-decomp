// Skirmish test harness orders (README "Skirmish harness";
// --skirmish-order=<kind>[@<frame>]). Test scaffolding, never game behaviour:
// at the given Main_Loop frame (default 10) the harness queues the orders a
// player's click would, as MEGAMISSION events (Queue_Mission 0x006233A0, the
// call TechnoClass slot 200 0x006CC2A0 ends in), for the human player's
// start units. Kinds: "move" (every infantry and vehicle moves 6 cells east),
// "deploy" (vehicles whose type deploys into a building, the MCV, unload:
// mission 16), "both" (the MCV deploys, the others move).
#define RA2_COORD_AS_CELL   // CoordStruct::As_Cell (shared/coord.hpp)
#include "shared/techno.hpp"
#include "shared/techno_type.hpp"
#include "shared/house.hpp"
#include "shared/target.hpp"
#include "shared/queue.hpp"

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

// Called by the frame seam (harness_skirmish.cpp) after each Main_Loop frame.
void Native_Harness_Orders(int frame)
{
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
    if (frame != at) return;
    int len = (int)(sep - order);
    bool move = (len == 4 && Same(order, "move", 4)) || (len == 4 && Same(order, "both", 4));
    bool deploy = (len == 6 && Same(order, "deploy", 6)) || (len == 4 && Same(order, "both", 4));
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
