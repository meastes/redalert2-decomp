# Ruling precedents

Owner decisions under the AGENTS.md decision policy. Match a new case here first; send it to the
review session only when no entry fits or it changes the build or acceptance model. One entry per
ruling: pattern, ruling, reason, example.

| Pattern | Ruling | Reason | Example |
|---|---|---|---|
| Source that could not coexist in one tree (a declaration guarded so only one unit sees it, to land a tie) | Refuse | Fails test (a): not one real codebase | WinMain 0x68E280 (unevidenced `~WolRegistryOptions()` under a unit guard, 2026-11-09) |
| Named locals that rename existing names, dead or dummy | Refuse; find the clean form, revert only the dependents that fail with it | Fails test (b); test (e) keeps the legitimate rest | Is_At 0x611310 kept natural, Execute 0x614BA0 reverted (-968 B, 2026-11-09) |
| Named locals holding real computed or looked-up values whose natural form is contradicted by its dependents | Keep, flagged `count_calibrated`; record failing dependents and scores in the note | Passes (b); evidence shows the plain form is wrong; flagged under (d) | Unknown_006820F0, Select_Tab 0x67D990 (2026-11-09) |
| A twin-evidenced idiom extended by analogy inside the same function | Pass, flagged; note which uses are evidenced | Evidence first (c), unprovable rest marked (d) | Select_Tab 0x67D990 (`StripClass &current` from YR Activate_Tab 0x6A74A0) |
| "Caller-tuning" (locals that only set a caller's inline budget) | Decides nothing alone; apply (b) to the values themselves. No exceptions to a test | A row passes or fails a test | Review session, 2026-11-09 |
| Include or header choices | Fixed by evidence, identical for every unit; never chosen per unit to land a tie | Per-unit choice is steering one level up | Scout1172 / SDK-header lab (2026-11-09) |
| A third-party file read at run time by the native port | User question | Third-party file use belongs to the user | BINKW32.DLL tables (approved 2026-11-09) |
| A declaration guarded to keep a fragile unit's count, when the declared members are real and evidenced | Accepted (existing pattern) | The members are real; the guard selects declarations a unit needs | RA2_CELL_SET_CELLID (Scout1110), RA2_BUILDING_CLOAK_GENERATOR (Scout1159) |
| A tie that follows whole-unit declaration load (class-index window) | Park the row. The fix is the original's declarations (complete class declarations first, SDK headers second), fixed by evidence and the same for every unit. Filler declarations are diagnostic only | SDK prelude alone shifts but does not reach the window (Scout1179); twin sits just inside with complete class declarations | Get_Obstacle 0x4BBAF0 (Scout1172/1179, 2026-11-09) |
| Migration switch: a shared declaration is evidenced wrong | A per-unit switch is allowed while the tree migrates: two states only, units move only toward the evidenced side, end state is every unit on it and the switch deleted; holdouts tracked in HANDOFF | Moving toward evidence is migration, not steering | RA2_VECTOR_SIGNED_INDEX (TibSun vector.h:81, ra2-decomp vector.h:25 `operator[](int)`), Place_Tiberium 0x47C3A0 (2026-11-09) |
| A matching function in a third-party decomp the user approved for adoption (ra2-decomp, TibSun archive, mischa85/RedAlert2) | Adopt with attribution (SPDX/copyright kept, provenance line, commit subject "from <repo> <address>"); acceptance unchanged | User decisions 2026-11-06 and 2026-11-09 | Scout1168 (ra2-decomp), mischa85 from 2026-11-09 |
