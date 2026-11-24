// Initialised globals the static initialisers reach before WinMain, from
// recovered source text (the shared headers' comments and the code that reads
// them). Each states its evidence; an order or value that is a hypothesis is
// flagged and must be checked when its owner is recovered. No executable bytes.

// 0x007F67F4 / 0x007F67F8 (shared/tab.hpp): the advanced command bar's command
// names, 11 entries from "Team01" to "Cheer", and -1 for an unknown name. The
// names are the ones TabClass::TabClass (0x006A06E0) looks up. Order read from
// the image's pointer table at 0x007F67F8 (Deploy is index 4, before AttackMove,
// Guard and Beacon; UI.INI's command bar slots index this table).
int NoAdvancedCommand = -1;
char const *CommandNames[11] = {
    "Team01", "Team02", "Team03", "TypeSelect", "Deploy", "AttackMove",
    "Guard", "Beacon", "Stop", "PlanningMode", "Cheer",
};
