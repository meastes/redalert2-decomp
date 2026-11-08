// NON-EXACT DRAFT 0x5BA280 FirewallSettings::Load_005ba280
// entry 0x5BA280; exact-match score 100.00 (/O2 /Ob2 /G5, byte-exact in diff_candidate; not yet through the
// strict audit or the canonical tree: build/class-work/Scout1076/d/exact/); plan "draft as-is"
// origin lane: Scout1076
// Recovered source text by our lanes/workers; never exact-match credit until accepted. Tracked by
// native/drafts.json. Not original bytes.
// Reads [MultiPlayer] FirewallSettings (default 0) and FirewallDelta (default 1) of the INI
// object 0x00839740 into +0x04 and +0x0C (the Save pair 0x005BA210 writes +0x00/+0x08).
void FirewallSettings::Load_005ba280()
{
    unknown_04 = unknown_ini_839740.Get_Int("MultiPlayer", "FirewallSettings", 0);
    unknown_0c = unknown_ini_839740.Get_Int("MultiPlayer", "FirewallDelta", 1);
}
