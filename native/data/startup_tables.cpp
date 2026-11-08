// Initialised globals the static initialisers reach before WinMain, from
// recovered source text (the shared headers' comments and the code that reads
// them). Each states its evidence; an order or value that is a hypothesis is
// flagged and must be checked when its owner is recovered. No executable bytes.

// 0x007F67F4 / 0x007F67F8 (shared/tab.hpp): the advanced command bar's command
// names, 11 entries from "Team01" to "Cheer", and -1 for an unknown name. The
// names are the ones TabClass::TabClass (0x006A06E0) looks up. ORDER IS A
// HYPOTHESIS beyond the first and last entry (the header gives those two).
int unknown_no_command_7f67f4 = -1;
char const *unknown_command_names_7f67f8[11] = {
    "Team01", "Team02", "Team03", "TypeSelect", "AttackMove", "Guard",
    "Beacon", "Deploy", "Stop", "PlanningMode", "Cheer",
};
