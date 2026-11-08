// NON-EXACT DRAFT 0x5A74B0 MultiplayerManBattle::MultiplayerManBattle
// entry 0x5A74B0; exact-match score 100.00 (/O2 /Ob2 /G5, byte-exact in diff_candidate; not yet through the
// strict audit or the canonical tree: build/class-work/Scout1076/d/exact/); plan "draft as-is"
// origin lane: Scout1076
// Recovered source text by our lanes/workers; never exact-match credit until accepted. Tracked by
// native/drafts.json. Not original bytes.
// Forwards its six arguments to the MultiplayerGameMode constructor (0x005B6CD0), as
// MultiplayerBattle's (0x005A2320) does.
MultiplayerManBattle::MultiplayerManBattle(WideString const &name, WideString const &description, Wstring const &ini_name,
    Wstring const &unknown_4, bool unknown_5, int id) :
    MultiplayerGameMode(name, description, ini_name, unknown_4, unknown_5, id)
{
}
