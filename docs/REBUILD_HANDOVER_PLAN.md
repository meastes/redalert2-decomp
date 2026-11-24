# Rebuild handover plan

**Done on 2026-11-24 (lane O83; the user brought the date forward in the owner session: "Switch today once all the
criteria are met").** Rows 0 (dry run), 2-6, 8 and 12 are committed on the switch branch; row 1 (tag `src-final`),
7 (private landing scripts, `private/tools/retire_src_scripts.sh`), 9 (Windows workers) and 10 (public snapshot) are
the owner's steps at the fast-forward. Rows 11 (tool default, one week later, after pinning `RA2_TREE=main` in the
native tools) and 13 (the rebuild owns its evidence) remain. Figures below are those of 2026-11-22; the rebuild
since passed `src/` (94.13% vs 93.58% Reproduced .text, whole-unit, 2026-11-24 17:00Z). The 16 assembly files of row
12 are 15 (12 `.asm`, 3 `.inc`) plus the 3 revised `ra2asm` files, with their own ledger `rebuild/manifest/assembly.json`.

Lane O48, 2026-11-22 (main at 9f34da05a). A planning document; nothing here is done yet. It
implements the user's decision of 2026-11-22 (AGENTS.md): from 2026-11-29 `rebuild/` is the
exact-match tree of record, `src/` retires at the tag `src-final` (read-only, kept as reference
text), and the native ports keep building from `src/` until a separate ports repository consumes the
rebuild at a pinned commit. Creating or publishing that repository is the user's decision alone.

Revised the same day after the review session approved it with seven changes: whole-unit headline,
the variation list first on 2026-11-29, decisions regrouped, assembly moved into the switch day,
step 13 (the rebuild owns its evidence), a split-first ordering for the ports, and the STATUS line
for what `src-final` matches and the rebuild does not yet.

Three plans:

1. [Retiring `src/` on 2026-11-29](#1-retiring-src-on-2026-11-29): ordered steps, each with a check
   and a rollback.
2. [Re-costing the move of the native ports onto the rebuild](#2-moving-the-native-ports-onto-the-rebuild-re-cost).
3. [A ports repository, for the user's decision](#3-the-ports-repository-a-design-for-the-users-decision).

Every step stays reversible until the user's go. Text the user must approve is marked
**[user approval]**. The user decisions are at the end, in three groups: the Thursday 26 bundle (the
only items gating 2026-11-29), defaults taken, and a later ports bundle.

## Starting point (measured today)

| Measure | `src/` (main) | `rebuild/` |
| --- | ---: | ---: |
| Reproduced .text (STATUS.md) | 93.54% | 90.93% function-level, 90.34% whole-unit |
| Catalogued functions (regions) | 17,910 | 17,560 |
| Catalogued body bytes | 3,277,006 | 3,158,845 (96.4% of main's) |
| Source files | 2,369 in the catalog (2,373 `.cpp` under `src/recovered`) | 438 units (`rebuild/src`) |
| Headers | 569 (`src/shared`) | 652 (`rebuild/include`) |
| Function symbols that are placeholders (`Unknown_`, `_<hex>`, `FUN_`, `seed_`) | 3,353 | 121 |

Both catalogs share 17,523 entries (3,105,692 B). 387 functions (171,314 B) are exact in main and not
yet in the rebuild's catalog: 164 of them (159,726 B) are parked rows of rebuild units, and 223 small
ones (11,588 B: template copies, header statics, class files) sit in no rebuild row yet. 37
functions (53,153 B) are exact in the rebuild only, 27 of them main's never-matched functions
(TActionClass::operator(), Init_Game and others).

The like-for-like headline is the **whole-unit** figure (what the whole-file exact build accepts,
the measure main's 93.54% uses): 90.34% today. The function-level figure (90.93%) is shown beside it
and labelled, never in its place. So on 2026-11-29 the headline will fall by about three points
unless the rebuild reaches main's matched bytes first. The README has to say so plainly (step 8).

The first item on 2026-11-29 is the user's answer on the variation list
(`rebuild/manifest/variation_candidates.csv`, `flex_menu.csv`). The held variations total about
145 KB, against a gap to main of about 123 KB, so the figure is expected to move after that
decision. The switch does not wait for it.

Until the gap is zero, STATUS carries the line "matched in src-final, not yet in the rebuild: N
functions / B bytes" (lane O50 is adding it).

---

## 1. Retiring `src/` on 2026-11-29

### What changes, in one table

| Area | Today | After 2026-11-29 |
| --- | --- | --- |
| Acceptance | Two trees, two gates. `src/` lands through `land.sh` (strict audit, whole-file build). `rebuild/` lands through `land_rebuild_lane.sh` | Only `rebuild/` takes new rows. `src/` and main's catalog are frozen at `src-final`. Acceptance standard unchanged |
| STATUS (`status.py`) | Main's numbers are the headline; the rebuild is a side column | The rebuild's like-for-like whole-unit figure is the headline, with function-level beside it and labelled. `src-final` is a frozen reference column, plus the line "matched in src-final, not yet in the rebuild: N functions / B bytes" until it reaches zero (lane O50) |
| `build.command` | Main's whole-file build (`build.py`, main's config and catalog) | The rebuild's whole-file build. `build.command --tree src` builds the frozen record |
| `test.command` | `tests/` (tool tests) | `tests/` plus `rebuild/tests/` |
| `gate_scope.py` | Scopes `exact`, `native`, `rebuild`, `none` | A change under `src/` or `manifests/original_target/` is refused (scope `frozen`). `rebuild` becomes the acceptance scope. `native` is unchanged |
| Public snapshot | `publish_github.sh` pushes a snapshot of main. Rebuild-only commits publish at most hourly. `publish_rebuild.sh` (branch `rebuild-trial`) is no longer used since the merge | `publish_github.sh` unchanged. Cadence and the fate of `rebuild-trial` are user decisions |
| ByteMatch | Main's series plus a rebuild series (`bytematch_report.py --tree rebuild`) | Main's series ends at `src-final`. The rebuild series is the live one |
| Native ports | Generated from `src/recovered` and `src/shared` | Unchanged: still generated from `src/`, now identical to `src-final` |

### What stays depending on `src/` and main's catalog, and must not be deleted

On the day the retirement freezes `src/`; it cannot remove it yet. The intended end state (owner
ruling, matching the user's "kept as reference text") is that `src/` and main's catalog are reference
only and nothing the tree of record builds or lands depends on them: permanent dependence is not
accepted. Step 12 removes the assembly dependency on the day; step 13 removes the rest. Measured
dependencies:

- **The rebuild's whole-file build** assembles the 16 handwritten assembly sources from
  `src/recovered/asm` (`tools/original_target/assembly_link.py`, not switched by `RA2_TREE`). Cut
  on the day by step 12. Since 2026-11-24 (lane O73) it also assembles the revised, binary-derived
  sources in `src/recovered/asm/ra2asm/` (allowlist `assembly_tools.REVISED_ALLOWLIST`, a hard-coded
  path too): step 12 moves them to `rebuild/asm/ra2asm/` with the allowlist keys.
- **The rebuild's catalog** keeps main's frozen evidence on every row: bounds, body and padding
  hashes, and the `bound_population` (main's population that froze the row). Its build config is
  main's private config plus one entry per rebuild population pointing at main's inventory
  (`rebuild/tools/rebuild_catalog.py`). So `manifests/original_target/` and the private
  inventory are inputs of the tree of record. They stay as read-only evidence until step 13.
- **Rebuild tools that read main as the answer key**: `rebuild.py` and `assess.py` (main's catalog);
  `mainhist.py`, `filetail.py`, `keptcopy.py`, `nodraft_queue.py`, `tail.py` (main's catalog, module map,
  unit map, non-exact list); `image_bounds.py` (main's `assembly.json` and `library.json`);
  `declared_never_defined.py` (main's `src/` definitions); `provenance.py` (class b, "reshaped from
  main"); `rebuild_census.py` (main's `cleanup_census.py` structure checks).
- **The native ports**: `native/tools/native_units.py` (11 `src/` path literals, compile groups
  from main's catalog), `port/fixups.json` (all 499 fixups keyed to a `src/` file),
  `initterm.json` (3,610 entries naming `src/` files), `view_audit.py`, `vtable_views.py`,
  `member_aliases.py`, `original_data.py`, `fast_math_tables.py`, `name_rules_check.py`, and
  `native/CMakeLists.txt` (`src/recovered/asm` for the asm check). See NATIVE_COUPLING.md.

None of these needs to change on the day: the files stay where they are, unchanged.

### Which tools read `src/` today, and what each should read after

`grep` of `tools/`, `rebuild/tools/`, `private/tools/` and `native/tools/` for `src/` paths and main's
catalog, sorted by what happens to each.

| Tool | Reads today | After |
| --- | --- | --- |
| `tools/original_target/tree.py` | `RA2_TREE` picks `src/recovered`+`src/shared` (default) or `rebuild/` | Default stays `main` on the day. Flip the default to `rebuild` one week later (step 11), once the landing scripts have run on `RA2_TREE=rebuild` alone |
| `build.py`, `canonical.py`, `generated_data.py`, `data_bindings.py`, `source_paths.py` (tree-aware through `tree.py`) | Either tree | Unchanged; the rebuild via `RA2_TREE=rebuild` |
| `catalog_io.py`, `verify_manifest.py`, `coff.py`, `cleanup_census.py` (shared, used by rebuild tools) | Either catalog | Unchanged |
| `assembly_link.py`, `assembly_tools.py` | `src/recovered/asm`, `src/recovered/asm/ra2asm` (hard-coded) | `rebuild/asm/` through `tree.py`, on the day (step 12) |
| `status.py` | Main's catalog (headline) plus the rebuild's catalog and `progress.csv` | Headline: the rebuild's whole-unit figure, function-level beside it and labelled. `src-final` figures computed from the frozen catalog (they cannot change), and the "matched in src-final, not yet in the rebuild" line |
| `bytematch_report.py` | Main by default, `--tree rebuild` for the rebuild series | Default `--tree rebuild`. Main's series stops |
| `gate_scope.py` | Path prefixes | Adds the frozen scope (step 3) |
| Main-only matching tools: `accept_sources.py`, `apply_rename.py`, `callsite_rename.py`, `member_rename.py`, `rename_files.py`, `organize_by_class.py`, `refile_unit.py`, `promote_sweep.py`, `regenerate_seeds.py`, `repair_seeds.py`, every `seed_*.py`, `sweep_seeds.py`, `permute_seeds.py`, `class_seed.py`, `generate_*.py`, `module_ledger.py`, `non_exact.py`, `placeholder_census.py`, `source_index.py`, `treemap.py`, `xcu_units.py`, `lane_tools.py`, `check_plans.py/.sh`, `fragile_lab.py`, `remote_compile.py`, `diff_candidate.py` (38 files) | `src/recovered`, `src/shared`, main's catalog | Kept as text, not deleted. Any that writes under `src/` or main's catalog is refused by the frozen gate. Their tests in `tests/` keep running: they test fixtures and shared code |
| `rebuild/tools/*` listed above | Main's catalog and `src/` as answer key | Unchanged. They read the frozen files at HEAD, which equal `src-final`. If `src/` is ever moved out of the tree, they must read `git archive src-final` output under `build/` instead |
| `private/tools/land.sh`, `land_round.sh`, `land_status.sh`, `accept.sh`, `merge_headers.sh`, `rehash_catalog.py`, `rehead_plan.py` | `src/`, main's catalog | Retired: each exits with "src/ retired at src-final; land through land_rebuild_lane.sh" (step 7) |
| `private/tools/land_rebuild_lane.sh` | Rebuild catalog, main's private config | The only landing script. Still sets `RA2_TREE=rebuild` |
| `private/tools/publish_github.sh` | HEAD of main | Unchanged |
| `private/tools/publish_rebuild.sh` | Branch `rebuild-trial` | Retired (it already refuses any commit not on `rebuild-trial`) |
| `private/tools/native_main.sh`, `native/tools/*`, `native/CMakeLists.txt` | `src/` | Unchanged until the ports move (plan 2) |

### Ordered steps

Steps 1 to 10 and 12 are the switch day (step 12 is rehearsed in the dry run, so the tree of record
builds without `src/` from day one). Step 11 follows a week later; step 13 has no date. Each step is
one commit (or one private-script edit), so each reverts on its own.

| # | Step | Check | Rollback |
| --- | --- | --- | --- |
| 0 | **Before the day.** Owner reads this plan. The Thursday 26 bundle goes to the user (variation list, AGENTS.md wording, README wording). A dry run of steps 2 to 6 and 12 on a branch `retire-src-dryrun` in its own worktree, run in a fresh worktree as the 2026-11-19 clean-reproduction check was | Fresh-worktree `build.command` (rebuild) and `build.command --tree src` both print `"whole_file_exact": true`, the rebuild one also with `src/recovered/asm` renamed away; `test.command` passes | Delete the branch |
| 1 | **Freeze point.** First, apply the user's answer on the variation list (Thursday 26 bundle) as rebuild landings; the switch does not wait for it if it has not come. Land the last `src/` delivery, if any. Run `./build.command` (main) and `status.py --write`. Commit. Make the annotated local tag `src-final` on that commit | `git rev-parse src-final` resolves; the build log at that commit is whole-file exact; `git diff --quiet src-final HEAD -- src manifests/original_target` | `git tag -d src-final` (local only, default A) |
| 2 | **Tests cover the frozen tree.** `tests/test_src_frozen.py`: fails when `git diff src-final HEAD -- src manifests/original_target` is non-empty | `test.command` passes; a scratch edit under `src/` makes it fail | Revert the commit |
| 3 | **Gate.** `gate_scope.py`: paths under `src/` or `manifests/original_target/` give scope `frozen`, which the landing scripts refuse. `rebuild/` is the acceptance scope. A shared-tool change still runs the rebuild gate, the native gate, and `build.command --tree src` so `src-final` stays reproducible | `gate_scope.py --base src-final` on a scratch `src/` edit prints `frozen`; on a `rebuild/` edit prints `rebuild`; `tests/test_original_target_gate_scope*` pass | Revert the commit |
| 4 | **`build.command` and `test.command`.** `build.command` runs the rebuild's whole-file build: write the config under `build/r/` without touching tracked files, then `build.py --config build/r/rebuild-config.json --manifest rebuild/manifest/catalog`. `--tree src` runs today's command. `test.command` discovers `tests/` and `rebuild/tests/`. (Today the config is written by `rebuild.py catalog`, which also rewrites `units.json`. The landing script restores that file afterwards; `build.command` needs a config-only mode or that restore) | Both builds whole-file exact from a fresh worktree; `git status` clean after each | Revert the two files |
| 5 | **STATUS.** `status.py`: the headline is the rebuild's like-for-like whole-unit figure, with function-level beside it and labelled; judgment-call bytes, parked tail and the "matched in src-final, not yet in the rebuild: N functions / B bytes" line (lane O50) until that reaches zero. Main's table becomes "src-final (frozen 2026-11-29)", computed from the frozen catalog. Matched bytes per hour from the rebuild's `progress.csv` | `status.py --write`: the headline equals the last `progress.csv` row's whole-unit `reproduced_text`; the src-final numbers equal step 1's STATUS | Revert `status.py` and `STATUS.md` |
| 6 | **ByteMatch.** `bytematch_report.py` defaults to `--tree rebuild`; one final main upload at `src-final` | `--dry-run` (or no `--upload`) writes the rebuild report | Revert the default |
| 7 | **Landing scripts (private, untracked).** `land.sh`, `land_round.sh`, `land_status.sh`, `accept.sh`, `merge_headers.sh`, `rehash_catalog.py`, `rehead_plan.py` and `publish_rebuild.sh` exit with the retirement message. Keep a `.pre-retire` copy of each. `land_rebuild_lane.sh` keeps setting the `rebuild-trial` pointer the Windows workers fetch until step 9 is done. Its hourly publish gate stays (default C) | `land.sh x` prints the refusal; one real rebuild landing goes through end to end (gate, whole-file build, `progress.csv`, commit, mirror sync, publish) | Restore the `.pre-retire` copies |
| 8 | **Docs.** AGENTS.md **[user approval]**, README.md **[user approval]**, ORIGINAL_TARGET_PROCESS.md ("Two trees, two gates" becomes "Tree of record"), ORIGINAL_TARGET_BUILD.md (commands), ARCHITECTURE.md (canonical source in `rebuild/`), STATUS.md (generated), HANDOFF.md, RECOVERED_SOURCE_INDEX.md (frozen header line), `rebuild/README.md` ("How it relates": `src/` is now the `src-final` reference), DECISIONS.md (the ruling), NATIVE_COUPLING.md (points at plan 2). THIRD_PARTY.md needs no change | Doc-only commit; `gate_scope.py` prints `none` | Revert the commit |
| 9 | **Windows workers.** Their brief and assignments fetch `main`, not `rebuild-trial`; the mirror sync is unchanged. Once both workers have fetched main, drop the `rebuild-trial` pointer line from `land_rebuild_lane.sh` | Next delivery from each worker is based on main | Restore the pointer line |
| 10 | **Public snapshot.** The first publish after the switch carries the new README. `src-final` stays a local tag (default A); pushed as a public tag only if the user asks, mirroring `historical-final-public` | `owner_checks.sh` "Public snapshot" line shows the commit; the public README shows the rebuild's numbers | A further snapshot restores the text (published history itself is not reversible: that is why the wording is approved first) |
| 11 | **One week later: tool default.** `tree.py` default becomes `rebuild`; `RA2_TREE=main` reaches the frozen tree. A shared-tool change, so both gates | Both builds whole-file exact; `test.command` passes | Revert `tree.py` |
| 12 | **On the day (rehearsed in step 0): assembly into the rebuild.** Copy the 16 `src/recovered/asm` files (`.asm`, `.inc`) to `rebuild/asm/` unchanged, with their notices, and have `assembly_link.py` read the tree's directory (`tree.py`). The rebuild then no longer needs `src/` to build | Rebuild whole-file build exact with `src/recovered/asm` temporarily renamed in a scratch worktree | Revert; `src/recovered/asm` is untouched |
| 13 | **No date: the rebuild owns its evidence.** The rebuild catalog carries its own bounds, hashes and inventory, after which `src/` and main's catalog are reference only. Dependencies to cut, each one commit: (a) the catalog rows' `bound_population` and frozen evidence (bound, body, padding and full-extent hashes, padding provenance) copied into rebuild-owned populations, so `rebuild_catalog.py` no longer reads main's catalog; (b) the build config generated from the rebuild's own populations and an inventory folder the rebuild names (copied or re-pointed from main's private inventory), not main's config plus overrides; (c) `image_bounds.py`'s use of main's `assembly.json` and `library.json` moved to rebuild manifests; (d) the answer-key readers (`rebuild.py`, `assess.py`, `mainhist.py`, `filetail.py`, `keptcopy.py`, `nodraft_queue.py`, `tail.py`, `declared_never_defined.py`, `provenance.py`) reading `src-final` through `git archive` under `build/` or a generated rebuild-side index, and optional for landing; (e) `rebuild_census.py`'s structure checks from `cleanup_census.py` made tree-aware; (f) shared tools default to the rebuild (step 11) | Rebuild gate passes in a fresh worktree with `src/` and `manifests/original_target/` renamed away | Revert the commit for the item; main's files were never changed |

### What keeps building from `src/` meanwhile

- The native ports, all six `native/build.sh` targets (mac, mac-one, ios, windows, yr-windows,
  windows-one), `native_main.sh` and the native ctest gate. The `native` gate scope is unchanged,
  and native lanes never edited `src/` anyway.
- `build.command --tree src`, run whenever a shared tool changes, so the frozen record keeps
  reproducing the original exactly.
- The Yuri's Revenge native build reads `yr/ra2-decomp` and `yr/tibsun`, plus the RA2 native tree's
  port rewrites (`native_units.port`). The retirement does not touch it.

### Wording the user must approve

**[user approval] AGENTS.md** (no figures in it, so it does not go stale), replacing the "Fixed constraints" paragraph that begins "User decision
(2026-11-22, confirmed in the owner session)":

> User decision (2026-11-22; in force since 2026-11-29): `rebuild/` is the exact-match tree of record.
> Acceptance, STATUS, ByteMatch and the public snapshot follow it. `src/` and main's catalog
> (`manifests/original_target/`) are frozen at the tag `src-final`: read-only, never edited, kept as
> reference text and as evidence the rebuild's rows cite. `build.command --tree src` still reproduces
> the original from them. The native ports keep building from `src/` until a separate ports repository
> consumes the rebuild at a pinned commit; creating or publishing that repository is the user's
> decision.

And in "How to work" and "Readable source": `src/recovered/` becomes `rebuild/src/` (one file per
original unit), `src/shared/` becomes `rebuild/include/` (one header per class), and
`compile_candidates.py` becomes the rebuild landing (`rebuild.py check` / `catalog`, then the owner's
landing script). The rename-lane, seed and class-sweep paragraphs describe `src/` work, which stops;
they move to a "Retired with src/" note rather than being deleted.

**[user approval] README.md**, the opening paragraph's second sentence and the "Where it stands" lead:

> About 90.3% of the code is rebuilt from source in the current tree, `rebuild/` (90.9% counting each
> function checked on its own): one source file per original source file, laid out like Westwood's own
> code. The earlier tree, `src/`, reached 93.5% on the same measure and is kept unchanged as a
> reference (tag `src-final`); the native ports still build from it while they move to the new tree.
> The figure is expected to move in the coming days as held forms are decided.

(90.3% is the like-for-like whole-unit reproduced share on 2026-11-22, the measure of main's 93.5%;
90.9% is function-level, labelled. Step 5 regenerates both; the last sentence stays until the
variation-list answer has landed.)

---

## 2. Moving the native ports onto the rebuild: re-cost

### Summary

The earlier owner estimate was 25 to 35 lane-days. Re-measured today: **17 to 28 lane-days, about 21
expected**. Add 1 to 2 lane-days to set up the ports repository (plan 3), and the user's device checks
on top. Three findings cut the cost:

1. **A pipeline for this kind of tree already exists.** The Yuri's Revenge port compiles ra2-decomp's
   tree (TibSun layout, one source per unit, the family the rebuild's text comes from) with
   `native/yr/tools/yr_units.py`, 2,157 lines of tools, 327 fixups and 93 glue files.
   - **Probe (scratch only):** `yr_units.py` pointed at `rebuild/` with no fixups at all. Then
     `clang -fsyntax-only` with the YR build's flags on all 438 units.
   - **Result:** 227 units compile clean as they stand, and 375 of the 438 errors come from two
     header lines. One is `int.h:175`, a friend declaration with a default argument. The other is
     `tag.h`, a static `Init` that hides a virtual (main's port already renames this one in
     `aircraft_type.hpp`).
   - **What is left:** after those two fixes, 26 units still have errors: `except.cpp`'s x86
     `CONTEXT`, which the port already replaces; four MS inline-asm sites; MIDL `_i.c` includes the
     probe left out; and about 20 one-off dialect errors.
2. **The rebuild names almost everything the port has to name by placeholder today.**
   - Of main's 3,353 placeholder function symbols, 3,086 have a real name in the rebuild.
   - Of the 6,503 placeholder identifiers in `native/link`, `drafts`, `data`, `fixups.json` and
     `view_layouts.json` that carry an image address, 6,134 (94%) resolve mechanically to a real
     rebuild name through the rebuild catalog's reference tables.
   - Another 330 are bound only in main, 35 by neither, and 4 still have placeholder names.
3. **Every save type and almost every RTTI class is already defined.**
   - All 76 save types (`native/data/original_save_types.txt`) have a class definition in
     `rebuild/include`.
   - Of the 295 RTTI classes with vtables, 280 are defined in rebuild headers, against 291 in
     `src/shared`. The 11 missing are command and Westwood Online classes, which the rebuild defines
     in unit sources.

### Coupling, re-measured against the rebuild

| Native item (NATIVE_COUPLING.md, recounted today) | Size | On the rebuild |
| --- | --- | --- |
| `native_units.py` (view generator) | 976 lines, 24 rules, catalog compile groups | Replaced by a `yr_units.py`-style generator: one TU per `rebuild/src` unit, one header directory. The port rewrites (`native_units.port`: long width, literal suffixes, CRT names, window data) are reused as they are, as YR does |
| `port/fixups.json` | 499 fixups over 155 `src/` files: about 246 pointer-width, 161 hooks, enhancements and saves, 27 dialect, 65 other (classified by their `why` text) | Re-keyed to rebuild text by function name. Dialect: about 25 (probe). Pointer-width patterns re-applied (many become generic `port()` rules). Hooks re-attached to the rebuild's function names, as YR's glue already does for 30 to 60 hook points |
| `port/view_layouts.json` (46), `data/vtable_views.tsv` (44) | Our view types | Dropped: the rebuild has real classes and vtables (the structure gate holds zero duplicate class definitions and no views) |
| `link/*.cpp` | 178 files, 15,941 lines | Rewritten against rebuild headers. A tool renames by address (function names from `units.json` `main_symbol` → `symbol` for 17,690 rows; data names from the reference tables). Members are renamed by offset; there are 3,092 placeholder identifiers with no address, mostly member and owner-type names |
| `drafts/*.cpp` + `drafts.json` | 83 files, 14,198 lines; 84 rows | 24 drafts sit at addresses the rebuild now has exact (21 of them, 33 KB, exact only in the rebuild): dropped, their exact text replaces them. The rest move by address and are renamed |
| `behaviour_rewrites.json` | 311 rows (298 with an address) | Address-keyed, so they carry over. The C++ bodies are renamed as `link/` is |
| `initterm.json` | 3,610 entries naming `src/` files | Re-keyed by address to rebuild units (`units.json` ranges); mechanical |
| `data/*.tsv` (address, bss, member aliases, header statics, owner sizes, unsourced initialised) | About 1,420 rows, most placeholder-named | Regenerated from rebuild names by address; member aliases by offset in rebuild layouts |
| Save layer (`original_layouts.py`, 76 types) | Field maps from our headers' member paths | Regenerated from rebuild headers. Every type's size is checked against the sizes the type list records |
| Platform layer (`platform/`, `port/src`, `port/include`) | About 18k lines | Unchanged: it includes no `src/shared` header |

### Functions the rebuild does not yet carry as exact

A port needs behaviour-correct text for every function it links.

| Group | Functions | Bytes | Source for the port |
| --- | ---: | ---: | --- |
| Parked in the rebuild, draft exact with its guard open (`exact_when_open`) | 81 | 15,851 | The rebuild's own draft |
| Parked, only code generation differs (register order, inlining, not emitted) | 45 | 120,155 | The rebuild's draft; behaviour checked against main's exact text |
| Parked, calls differ, unbound or no draft | 35 | 17,584 | Main's exact text, reshaped to rebuild names, in a port-side overlay (as `drafts/`) |
| Main-exact, in no rebuild row (template copies, header statics) | 223 | 11,588 | The rebuild's headers emit most template copies; the rest from main's text through the overlay |
| Never matched in either tree | about 69 | | The existing native drafts and behaviour rewrites, by address |

(Bucket counts from `rebuild/manifest/tail_buckets.csv`; a few main-exact entries are counted under
no bucket.) These numbers fall with every rebuild landing. At parity the overlay is close to empty.

### Work items and lane-days

| # | Item | Lane-days |
| --- | --- | ---: |
| 1 | Generator for the rebuild tree (from `yr_units.py`), dialect fixups, all 438 units compiling, Mac link closure at today's level | 2–3 |
| 2 | Data globals, address and bss aliases, header statics, initterm re-keyed by address; the per-build image reader unchanged | 2–3 |
| 3 | `link/` and `drafts/` (about 30k lines) renamed and re-typed against rebuild headers, tool-assisted; drafts the rebuild now matches dropped | 5–8 |
| 4 | Overlay for main-exact functions the rebuild still parks (35 plus the small unfiled ones); behaviour check of the 45 codegen-only drafts | 1–2 |
| 5 | Enhancement and save hooks re-attached (zoom, menu fill, HD vehicles, Queue Five, modern controls, mods, saves), following YR's glue | 2–3 |
| 6 | Save field maps regenerated; cross-load with the original game both ways | 1–2 |
| 7 | Pointer-width and run-time faults the new text exposes (the old 246 width fixups as patterns), found by the played tests | 2–4 |
| 8 | Windows, iOS and the combined RA2+YR app; full native ctest tier, campaign start sweep (24 missions), frame-identity tests, LAN Mac–Windows | 2–3 |
| | **Total** | **17–28** |

Calibration: YR stage 1 (its whole tree compiling and linking) was boxed at about 20 lane-hours and
took one lane session; YR went from nothing to a skirmish played to the score screen with saves in
about five calendar days. The RA2 move starts better placed: every behaviour question already has
a working answer in today's port, addressed by entry.

### Order of work (for the later ports bundle)

Start after the flex week. Two orderings; the cheaper one first:

1. **Split first, then move (recommended).** Create the ports repository with `src-final` vendored
   (`vendor/ra2-src-final/`: `src/recovered` and `src/shared` at the tag, unchanged, under a `PIN`
   list). Today's `native_units.py`, fixups and tables point at that folder instead of `../src`. This
   costs 1 to 2 lane-days, and the played port stays byte-for-byte the same program. The move onto
   the rebuild (items 1 to 8, 17 to 28 lane-days) then happens inside the new repository, with
   `vendor/ra2-rebuild/` beside the old folder until the new port passes the full tier and the
   user's device checks. This repository stops carrying ports work from the split onwards.
2. **Move first, then split.** Items 1 to 8 here in `native/`, then the split. Same total, but two
   generated trees and both ports live in this repository for the whole move.

Creating the repository stays the user's decision alone (decision P2).

### Main risks

- **Behaviour drift where two exact texts differ in source.** Both trees give the same VC6 bytes, but
  clang may treat them differently where the code leans on undefined or implementation behaviour
  (signed overflow, evaluation order, uninitialised locals, bit-field layout). The frame-identity
  and played tests are the net; budgeted in item 7.
- **A moving target.** The rebuild lands daily and the user plans rule flexes after 2026-11-29. Pin a
  commit, re-vendor deliberately, and start after the flex week, so the overlay (item 4) is small.
- **Parked drafts are non-exact.** The 45 codegen-only drafts should behave like main's exact text,
  but nobody has proven it. Each gets a behaviour check against main's text before it replaces it.
- **Layouts under clang.** `-mms-bitfields` and the TibSun headers' packing must reproduce the sizes
  the save layer records. Item 6 checks all 76 types. A mismatch shows up as a save that does not
  cross-load.
- **Inline assembly in the rebuild.** 46 `__asm` lines in 7 files, including the user-approved header
  specialisations (blitters). The port already has C for these routines; they need wiring by name.
- **Memory and disk on the Mac.** Two generated native trees side by side during the move. Run one
  native configure at a time, under the disk guard (20 GiB).
- **Losing what the port has proven.** The src-based port is the one the user has played
  (campaign wins, LAN, saves). Keep it buildable from `src-final` until the rebuild-based port passes
  the full tier and the user's device checks.

---

## 3. The ports repository: a design for the user's decision

### What goes in, what stays out

- **In:** today's `native/` (platform layer, port, link, drafts, data tools, tests, packaging, CMake),
  `native/build.sh`, the YR port (`native/yr`), and the pinned sources it builds from: the rebuild's
  `src/` and `include/`, `yr/ra2-decomp`, `yr/tibsun`.
- **Out:**
  - All game data. No executables, mix files, movies, `Ra2.tlb`, `BINKW32.DLL`, maps, extracted
    tables or saves. The apps keep reading the user's installed `game.exe`, `Ra2.tlb`,
    `BINKW32.DLL`, the mix files and `gamemd.exe` at run time, located per build by signature or a
    per-build address map, with the CD check skipped (AGENTS.md decisions of 2026-11-08 and
    2026-11-09).
  - The exact-match tooling, `private/`, `build/`, the VC6 compiler and the catalogs.

### Three ways to consume the rebuild

| | A. Git submodule | B. Pinned commit + fetch script | C. Vendored snapshot |
| --- | --- | --- | --- |
| What it is | `third_party/redalert2-decomp` points at a commit of the public repository | `REBUILD.lock` names a commit and a SHA-256 list. `tools/fetch_rebuild.sh` fetches only `rebuild/src`, `rebuild/include`, the licence and the indexes into an ignored folder and checks the list | `vendor/ra2-rebuild/` holds a copy of those files, unchanged, with a `PIN` file (local main commit, public snapshot commit, SHA-256 list). `tools/update_rebuild.sh <commit>` re-vendors and writes the derived symbol map |
| Fits the public history? | Poorly. The public repository holds one snapshot commit per publish, not main's commits, so the submodule pins snapshot ids, which differ from the local ones. A checkout also brings the 298 MB of manifests and every other folder | Yes: it fetches files at the snapshot commit | Yes: no network needed after the copy |
| GPL "corresponding source" | Satisfied only while the public repository keeps that snapshot | A source release must bundle the fetched files | The repository is complete on its own |
| CI | Recursive checkout of a large repository | A fetch per run (cacheable). Fails if the source repository is unreachable or private | Plain checkout |
| Updating | Move the submodule pointer | Edit the lock file | Large but mechanical diff (about 1,100 files, about 21 MB); review reads the `PIN` change and the script's report |
| Risk of local edits to the pinned text | Low | Low (checked against the list) | Guarded: a CI check fails when a vendored file differs from the `PIN` list. Precedent: `yr/ra2-decomp` is vendored this way today |
| Matches the existing YR setup | No | No | Yes |

**Recommendation: C, a vendored snapshot,** for the rebuild and, unchanged, for ra2-decomp and the
TibSun headers.
- The repository is complete on its own, which is the plain way to meet the GPL for any binary the
  user publishes.
- CI needs no access to this repository.
- It is the mechanism the YR port already uses.

The vendor script also writes a small derived `symbols.tsv` (address, symbol, unit; about 2 to 3 MB)
from the rebuild catalog, so the port's address-keyed tools do not need the 55 MB catalog.

### Licences and notices

- **The ports repository:** GPL-3.0-or-later (`LICENSE`), as the native tree is today.
- **Rebuild text:** adopted mostly from mischa85/RedAlert2, which builds on the TibSun
  reconstruction, ra2-decomp and EA's Renegade Westwood Online headers. It is GPL-3.0-or-later, with
  Electronic Arts' section 7 terms on files carrying an EA copyright line.
  - Each file keeps its SPDX and copyright lines, as adopted.
  - `vendor/ra2-rebuild/LICENSE.md` is `rebuild/LICENSE.md` (mischa85's: the GPL text plus EA's
    section 7 terms), copied unchanged.
  - `adopted.csv` is vendored too, as the per-file origin index.
- **ra2-decomp:** game source GPL-3.0-or-later with EA's section 7 terms (`COPYING.md`); `data/` and
  tooling text MIT, notice kept.
- **TibSun headers:** GPL-3.0-or-later with EA's section 7 terms (`LICENSE.md`).
- **Renegade WOLAPI headers** (`rebuild/include/wolapi.h`, `wolapi_i.c`): EA's GPL release, notice
  kept, marked modified.
- **`THIRD_PARTY.md`:** carries over the entries for these four, the native linked libraries (SDL3
  zlib, OpenSSL Apache-2.0, LZO GPL-2.0-or-later, zlib) and the Windows toolchain runtime that ends
  up in the binaries: llvm-mingw's mingw-w64 runtime (ZPL / public domain) and libc++ (Apache-2.0
  with LLVM exception). Packaged builds include SDL's licence file, as `build.sh windows-one`
  already does.
- **EA's section 7 terms forbid use of EA's trademarks.** App and bundle names (`RA2.app`,
  `RA2YRNative.exe`) and store-style descriptions are the user's call in that light (decision P6).
- **No new grant of rights** in the game, as THIRD_PARTY.md says today.

### Layout

```
ra2-native-ports/            (name: user decision)
  README.md  LICENSE  THIRD_PARTY.md
  build.sh                   (today's native/build.sh; paths to toolchains and SDL by variable)
  CMakeLists.txt  cmake/
  platform/  port/           (unchanged)
  ra2/                       (link/, drafts/, data/, tools/, fixups.json for the rebuild tree)
  yr/                        (today's native/yr)
  combined/                  (the one-game RA2+YR host)
  tests/  packaging/  docs/
  vendor/
    ra2-rebuild/   src/ include/ LICENSE.md adopted.csv symbols.tsv PIN
    ra2-decomp/    (today's yr/ra2-decomp, unchanged)
    tibsun/        (today's yr/tibsun, unchanged)
  .github/workflows/ci.yml   (if decision P5 says yes)
```

During the move (plan 2) a second vendor folder, `vendor/ra2-src-final/` (`src/recovered`,
`src/shared` at `src-final`), lets the old port build side by side until the new one passes. It is
removed after that.

### CI expectations

CI has no game files and no VC6, so it proves building, packaging and hygiene, not play.

| Check | Where |
| --- | --- |
| Vendored files equal their `PIN` lists (no edits to pinned text) | Every push |
| No game data: refuse known game file names and extensions (`.mix`, `.bik`, `.vqa`, `.tlb`, `game.exe`, `gamemd.exe`, `BINKW32.DLL`, saves) and any file over a size limit | Every push |
| Every source file keeps an SPDX line; THIRD_PARTY entries exist for each vendor folder | Every push |
| Mac build (arm64, Homebrew SDL3), static tests (link closure count, name rules, asm check) | Every push, macOS runner |
| Windows cross build (llvm-mingw 20261020, SDL 3.4.2 source checked against its SHA-256) | Every push or nightly |
| iOS simulator build, compile and link only | Nightly |
| Played tests, frame identity, campaign start sweep, LAN, saves | Not in CI: they need the user's installed game. Run on the owner's Mac (today's native gate, full tier) before any build for the user's devices and before a re-vendor lands |

CI downloads the toolchain, SDL and Homebrew packages; that needs the user's word (decision P5).

### History and the move itself

- Start the new repository either with a fresh history (one commit naming this repository's commit),
  or with `native/`'s history carried over by a subdirectory filter. The decision is the user's
  (decision P4).
- This repository keeps `native/` until the ports repository builds and passes the full tier. Then
  `native/` and `yr/` either stay frozen here or are removed under a tag `native-final`, like
  `historical-final`. Removal is the user's decision (decision P7).

---

## Decisions for the user

Three groups. None blocks the rebuild lanes.

### Thursday 26 bundle: the only things gating 2026-11-29

(a) **The variation list** (`rebuild/manifest/variation_candidates.csv`, totals in `flex_menu.csv`):
    which held hand-made forms may land. About 145 KB is held against a gap to main of about 123 KB,
    so this answer can move the headline past main's figure. It is applied first on 2026-11-29
    (step 1); the switch is not delayed if it has not come. [Default meanwhile: nothing held lands.]

(b) **AGENTS.md wording** for the retirement (section 1, "Wording the user must approve"; no figures
    in it). [Default: the 2026-11-22 text stands, and the owner runs the switch under it.]

(c) **README wording**: the public lead becomes the rebuild's like-for-like whole-unit figure (about
    90.3% today, function-level 90.9% beside it and labelled) against `src/`'s 93.5%, with a sentence
    that the figure is expected to move as held forms are decided. [Default: no README change until
    approved.]

### Defaults taken: say if you want otherwise

- **A.** `src-final` stays a local tag; it is not pushed as a public tag (as `historical-final-public`
  was) unless asked.
- **B.** The public `rebuild-trial` branch is left frozen at its last snapshot, not deleted (deleting
  cannot be undone for anyone who linked to it).
- **C.** Rebuild commits keep the hourly publish cap after the switch.
- **D.** ByteMatch: main's uploads stop at `src-final`; the rebuild series continues as a second
  series, not renamed or merged.

### Later bundle: the ports (sent when the ports move is proposed, after the flex week)

- **P1. Whether and when to move the ports onto the rebuild.** Two orderings (plan 2, "Order of
  work"): split the repository first with `src-final` vendored (1 to 2 lane-days, the played port
  unchanged), then move onto the rebuild inside it (17 to 28 lane-days, about 21); or move first,
  then split. [Recommended: split first, start after the flex week.]
- **P2. Create the ports repository**: yes or no, its name, host and account, and whether it is public
  or private. Publishing it also redistributes the adopted GPL text as a whole tree. This is the
  user's decision alone. [Default: not created.]
- **P3. How it consumes the rebuild**: A submodule, B fetch script, or C vendored snapshot.
  [Recommended: C.]
- **P4. History**: fresh, or `native/`'s history carried over. [Recommended: fresh, with a pointer
  commit; smaller and free of this repository's large blobs.]
- **P5. CI**: whether to use a hosted CI (macOS runner minutes cost money on private repositories), and
  approval for CI to download llvm-mingw, SDL 3.4.2 source and Homebrew packages.
  [Default: no hosted CI; the owner's Mac runs the checks.]
- **P6. Binary releases and names**: whether built apps are published at all, and under which names,
  given EA's section 7 trademark term. [Default: no binary releases; builds stay local, as today.]
- **P7. After the ports repository passes**: keep `native/` and `yr/` frozen in this repository, or
  remove them under a tag `native-final`. [Default: keep them frozen.]
