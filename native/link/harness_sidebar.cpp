// Skirmish harness: the sidebar for scripted input (README "Playing"; --input-script; lane
// C16). Test scaffolding, never game behaviour. Answers two scripted points for
// harness_input.cpp's resolver:
//   "@tab=N"     the centre of the sidebar's tab button N (SidebarClass's four tab buttons
//                0x00AB9508: 0 buildings, 1 defences, 2 infantry, 3 vehicles);
//   "@cameo=ID"  the centre of the build icon of type ID in the strip shown now (a strip's
//                visible icons are SelectButton[strip * 60 + i], showing Buildables[first
//                row * 2 + i]: SidebarClass::StripClass::SelectClass::Action 0x006809F0);
// and notes the player's production for the harness-input state: "inf=N veh=N", the
// objects of each kind in the house's factory (the one in production and the queue:
// HouseClass::Fetch_Factory 0x004EB580, kinds 0x10 infantry and 0x28 vehicles).
#include "shared/sidebar_screen.hpp"
#include "shared/sidebar.hpp"
#include "shared/house.hpp"
#include "shared/object_type.hpp"
#include "shared/factory_class_decl.hpp"
#include "shared/unknown_globals.hpp"

namespace {
bool Same_Name(char const *a, char const *b)
{
    for (;; ++a, ++b) {
        char x = *a >= 'a' && *a <= 'z' ? *a - 32 : *a;
        char y = *b >= 'a' && *b <= 'z' ? *b - 32 : *b;
        if (x != y) return false;
        if (x == 0) return true;
    }
}
bool Prefix(char const *text, char const *prefix, char const *&rest)
{
    while (*prefix && *text == *prefix) { ++text; ++prefix; }
    if (*prefix) return false;
    rest = text;
    return true;
}
void Centre(GadgetClass const &gadget, int *x, int *y)
{
    *x = gadget.X + gadget.Width / 2;
    *y = gadget.Y + gadget.Height / 2;
}
int In_Factory(int kind)
{
    HouseClass *player = PlayerPtr;
    FactoryClass *factory = player ? player->Fetch_Factory(kind, false, 0) : 0;
    if (factory == 0) return 0;
    return (factory->Object ? 1 : 0) + factory->Queue.Count();
}
}  // namespace

// A sidebar point ("tab=N", "cameo=ID"): 1 and its pixel, 0 when the spec is not one of
// these or names nothing shown.
int Native_Harness_Sidebar_Point(char const *spec, int *x, int *y)
{
    char const *rest;
    if (Prefix(spec, "tab=", rest)) {
        int tab = *rest - '0';
        if (tab < 0 || tab > 3) return 0;
        Centre(SidebarClass::unknown_buttons_ab9508[tab], x, y);
        return 1;
    }
    if (!Prefix(spec, "cameo=", rest)) return 0;
    int shown = Map.Unknown_006820F0();
    for (int strip = 0; strip < 4; ++strip) {
        SidebarClass::StripClass &column = Map.Column[strip];
        if (!column.unknown_18) continue;    // not the strip shown (StripClass::Activate)
        for (int i = 0; i < shown && i < 60; ++i) {
            int index = column.unknown_40 * 2 + i;
            if (index >= column.BuildableCount) break;
            ObjectTypeClass *type = Unknown_00482C00(column.Buildables[index].unknown_04, column.Buildables[index].unknown_00);
            if (type && Same_Name(type->ID.Buffer, rest)) {
                Centre(SidebarClass::SelectButton[strip * 60 + i], x, y);
                return 1;
            }
        }
    }
    return 0;
}

// The player's production as harness-input notes it.
int Native_Harness_Production(int kind) { return In_Factory(kind); }
