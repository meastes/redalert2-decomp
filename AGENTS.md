# Red Alert 2 original-target decompilation

Reproduce the exact supplied base RA2 executable from recovered C/C++, replacing
original machine-code scaffolding function by function. Exact compiler output is
the acceptance test; everything else exists to raise matched bytes per hour.

Read `docs/HANDOFF.md` (current work), `docs/STATUS.md` (generated numbers) and
`docs/ORIGINAL_TARGET_PROCESS.md` (workflow and completion criteria). Use
`docs/ORIGINAL_TARGET_BUILD.md` for tools/commands, `docs/ARCHITECTURE.md` and
`docs/RECOVERY.md` for build layers and evidence reuse, `docs/VC6_MATCHING_NOTES.md`
for source-shape rules that reproduce VC6 output, `docs/REFERENCES.md` for
external material. `docs/history/` holds per-batch records: consult, don't reload.

## Fixed constraints

- **Target:** 4,485,392 bytes, SHA256
  `e6418111c2c36081a1795fb000e0927beea1630170b334c5013d12102602e70f`. Hash before
  using addresses. The exact decompilation is base RA2 only. User decision (2026-11-15):
  a native Yuri's Revenge port proceeds as a separate native target built from
  ra2-decomp's tree (GPL-3.0-or-later, pinned, notices kept, `THIRD_PARTY.md`), sharing
  our native platform layer, staged per `docs/YR_PORT_SIZING.md` (plan 1, a unified
  menu with per-game match logic as the later target); it earns no exact-match credit
  and never edits `src/`. (Since the layout move of 2026-11-25 the native ports, the YR port among
  them, live in github.com/meastes/redalert2-modern; this repository keeps only the pinned `yr/` copies
  that repository vendors.)
- **Exact means exact:** a region is accepted only when the fresh VC6 object
  reproduces the complete independently bounded original body, references and
  frozen padding. No masks, prefix/tail trimming or candidate-derived bounds.
  Unknown extents, references or constants refuse acceptance.
- **No fake source:** no instruction arrays or original inline assembly in C
  candidates. Original-byte scaffolding, original data, identified library code
  and evidenced handwritten assembly are separate ledger categories. Exception
  (user, 2026-11-07): "evidenced inline assembly" — an `__asm` block only where
  the TibSun archive shows the author's own `__asm` at that point (cited), with the
  archive's instructions only, flagged `inline_asm: evidenced` in the manifest,
  enforced by the strict audit and reported as its own category. Exception (user, 2026-11-24,
  in the owner session): "revised, binary-derived lines" — a MASM procedure whose TibSun archive parent
  is cited, carrying RA2's revisions of it as derived from the binary (ra2-decomp `src/ra2asm`, attributed,
  shown as a diff against the archive), is credited to Reproduced .text only, never C/C++; its own STATUS
  sub-line, strict-audit flag and per-source allowlist; both trees' ledgers. Exception (user,
  2026-11-24, in the owner session): flagged variation forms land in the rebuild, each flagged by kind and
  counted on the judgment line and the STATUS kinds line: inline helpers where the image's expansion depth
  shows a helper existed; alternative statement or condition spellings; named locals chosen for the
  compiler; readability aliases (a copy with a more meaningful name or type than what it copies, one copy
  per value used from then on in place of the original, no copy of a copy). Still refused: any header form
  under which another function stops being exact; copies with an equivalent or meaningless name, several
  copies of one value, or a once-used copy where the original reads the same; dummy variables, dead code,
  pass-through wrappers and second names for an existing function. Every line does real work; matching
  reference text replaces a flagged form whenever it appears; acceptance is unchanged (one definition,
  whole-file exact, no other function stops matching). Exception
  (user, 2026-11-13, "Let's go with option 2 then", confirmed in the owner
  session): exact matching on the unmatched tail stops; the best drafts of
  unmatched functions are tracked in their unit's source under a non-matching
  guard (`RA2_NON_MATCHING`), each labelled not exact with its score; the exact
  build never compiles them and keeps the original bytes; flagged `non_exact`
  with score in the manifest, no .text credit, refused by the strict audit if they
  claim acceptance or contain instruction arrays or inline asm, and reported as
  their own category. Exception (user, 2026-11-13, confirmed in the owner
  session): bit 4 of the attributes field of each class hierarchy descriptor
  (`??_R3`) is accepted as a recorded known difference, because VC6 SP3 fills it
  from uninitialised memory per compile; every other byte is still compared,
  nothing is credited for it, and STATUS counts the affected descriptors.
- **Fidelity:** preserve widths, wrapping, signed division, FP stores, layouts and
  ordered reads/writes/calls. Type/ABI/class hypotheses stay explicit until evidenced.
- **Private material:** executables, assets, compiler/media, extracted data,
  dumps, disassembly, Ghidra exports, scaffolding, experiments and build outputs
  stay ignored under `private/` or `build/`; never stage them. Original
  installations and `outpost2-native` are read-only. Tracked: accepted source in
  `src/`, `include/` and `asm/`, authored tools/tests, byte-free manifests, docs.
- **External references:** public/recomp/YRpp material supplies hypotheses only;
  verify on the exact target and check licenses (`THIRD_PARTY.md`). Exception
  (user, 2026-11-06; extended 2026-11-09): text from ra2-decomp, the TibSun archive,
  mischa85/RedAlert2, electronicarts/CnC_Renegade, (2026-11-23) Generals' GameText.cpp and (2026-11-25)
  RobinDigitalPro/ra2-1001-decomp (repack baseline; text partly shared with mischa85's and ours, so not independent
  evidence there) may be adopted with attribution
  (`THIRD_PARTY.md`);
  acceptance is unchanged. Standing pulls (user, 2026-11-09): at each landing
  point pull `private/reference/ra2-decomp` and `private/reference/mischa85-RedAlert2`
  (at most hourly) and report functions they match that we have not accepted.
  Do not adopt a lifted runtime or OpenRA gameplay.
- **One tree (user decision 2026-11-24/25: layout move at port parity).** When the new ports in
  github.com/meastes/redalert2-modern reached parity (the Windows recheck passed on the real PC on
  2026-11-24; the user's Mac/LAN checks are not a gate), the repository made one layout move
  (2026-11-25, lane O88): the earlier `src/` tree was deleted (the tag `src-final` keeps it), `native/`
  (the old ports) was deleted, and the rebuild was promoted to the root. The exact-match tree is now:
  one source per original translation unit in `src/`, one header per class in `include/`,
  handwritten assembly in `asm/`, the catalog `manifests/catalog/` with the tree's other manifests
  beside it in `manifests/`, the tools in `tools/original_target/`, the tests in `tests/`. There is no
  tree switch (`RA2_TREE` is gone) and one gate: catalog, structure gate (census), whole-file build.
  The earlier tree's catalog `manifests/original_target/` stays, read-only and never edited, as the
  evidence the catalog rows cite (bounds, hashes, `bound_population`) and for its library ledger; its
  source is read from the tag `src-final` where a tool needs it. Acceptance, STATUS, ByteMatch and the
  public snapshot follow this tree. The native ports, the YR port and every native lane live in
  redalert2-modern, which vendors `src/`, `include/`, `LICENSE.md` and `manifests/adopted.csv` at a pinned
  commit (its `tools/update_rebuild.sh`).
- **Earlier decisions, kept as recorded** (paths updated: `rebuild/` is now the repository root and the
  exact tree is `src/` + `include/`; `native/` is in redalert2-modern):
  No native/SDL/arm64/gameplay check per recovery batch.
  `historical/` was removed on 2026-11-17 at the user's request (confirmed by the user
  2026-11-17); tag `historical-final`. The shipped game will be
  native arm64 with no Wine/CrossOver, Rosetta, VM, emulation or original code.
  User decision (2026-11-08): one dedicated native lane runs alongside the exact
  lanes; at five or six lanes, two native lanes, provided they work on separate parts of
  `native/` (each brief names its area, one lands at a time, neither edits `src/`); otherwise
  the extra lane is an exact lane (user, 2026-11-09). User decision (2026-11-17): while
  the ports are being finished (Red Alert 2 on Mac, iPhone and Windows; full Yuri's
  Revenge; real campaign wins left to the user), every lane may be a native lane, each on a
  separate part of `native/`, one landing at a time, none editing `src/`. Exact-match and
  structure work has no lanes for now; this is not a goal change, main keeps every matched
  byte, and the work can resume. User decision (2026-11-17, amended 2026-11-18): the
  rebuild is the standing matching effort (one Mac lane plus the Windows workers; structure
  checks are landing gates): a tree from the Tiberian Sun layout, mischa85's matched modules
  as the unit map, main's accepted source as the answer key, text adopted with attribution.
  User decision (2026-11-19): The rebuild lives on main in its own folder (then `rebuild/`, now the
  root) beside the earlier `src/`, built and gated separately (superseded by the one gate above).
  Rebuild-only commits publish at most hourly.
  User decision (2026-11-22; brought into force by the user, 2026-11-24, in the owner session:
  the rebuild is the tree of record from today): the rebuild is the exact-match tree of record.
  Acceptance, STATUS, ByteMatch and the public snapshot follow it. The earlier `src/` and main's catalog
  (`manifests/original_target/`) are frozen at the tag `src-final`: read-only, never edited, kept as
  reference text and as evidence the rebuild's rows cite. The exceptions recorded above apply to the
  tree of record as they stand,
  including the 2026-11-24 flagged variation forms (each flagged by kind, counted on the judgment line
  and the STATUS kinds line, replaced whenever matching reference text appears). The native ports keep
  building from `src/` until a separate ports repository consumes the rebuild at a pinned commit;
  creating or publishing that repository is the user's decision (done: redalert2-modern consumes this
  tree at a pinned commit).
  From every commit: one source file per original unit, one header and one definition
  per class, no per-unit views or `RA2_` switches, members as members, address-suffixed names
  minimal and never growing silently, no tool data inside sources. Acceptance is main's exact
  standard; no native-compile requirement. No separate assessment reports: the normal status
  carries a checkpoint (rate by provenance, structure line, parked tail, lane-hours, a
  continue/change recommendation) at 25%, 50% and parity of main's matched bytes, or when
  parked large functions pass 500 KB; the owner reports a stall unasked (two consecutive days
  where parked bytes grow faster than matched bytes outside adopted text). Native work (now in
  redalert2-modern) carries unmatched functions as their best
  parked draft (flagged non-exact with its score) or a behaviour-written
  implementation, never empty stubs. Native work earns no exact-match credit.
  User decision (2026-11-08): the native port may read data the original stores
  (file names, dialog templates, tables, COM class ids) from the user's installed
  `game.exe` and `Ra2.tlb` at run time, like other game files; never copied into
  the repo or shipped. The port reads either the rebuilt target game.exe or the
  user's installed game.exe (a different build: locate data per build, e.g. by
  signature or a per-build address map), and skips the CD check (user, 2026-11-08).
  User decision (2026-11-09): the port may also read the Bink tables from the user's
  installed `BINKW32.DLL` at run time on the same terms (never copied or shipped).

## How to work

1. **Pick by unit or family, not by single function.** `tools/original_target/tail.py` and the
   twin map (`tools/original_target/twinmap.py`) rank parked functions; the earlier tree at the tag
   `src-final` is the answer key. A class's layout lives once in its header under `include/` (one
   header per class); sweep its unit with those types.
2. **Iterate.** Edit the unit in `src/<unit>.cpp` and run `tools/original_target/rebuild.py
   check <unit>`, which compiles it with the real compiler and compares every row.
   Iterating on near misses (statement order, temporaries, types, expression shape, inline
   splits, option variant) is the normal work, not a forbidden "steering". A
   miss is never "closed": record its best score and move on when the time box
   (about 30 minutes per KB of body, capped at four hours) runs out; anyone may resume it.
3. **Land in bulk.** Exact units go through the landing: `rebuild.py check` /
   `catalog` (the structure gate), then the owner's landing script (house format, catalog,
   whole-file build, progress.csv). No separate reviewer is required for source, tools or
   build changes; the catalog audit and the exact whole-file build are the checks.
4. **Build periodically.** Run the full `./build.command` before each commit that changes
   accepted source or build logic, not for every candidate.
5. **Record briefly.** Commit accepted source with a short message. Regenerate
   numbers with `tools/original_target/status.py --write`; update `HANDOFF.md`
   with what is in flight and the next target. No per-batch cost receipts, hash
   tables or prose ledgers; the manifest and git history are the record.

## Readable source

The executable carries MSVC RTTI: original class names and inheritance for
every polymorphic class (`docs/CLASS_HIERARCHY.md`, `manifests/rtti_classes.json`).
Sources live one per original unit in `src/`, one header per class in
`include/`; use the original class names for shared layouts and members.

Name understood operations, parameters and fields as you match them, with the
original address as a suffix; keep honest `unknown_0xNN` names for the rest.
Prefer real shared types over per-function owner classes and register-named
locals. Existing transliterated sources (`unsigned int ecx = ...`) may be
replaced whenever a class sweep touches them, provided the replacement still
matches exactly. Formatting and renaming earn no matched-byte credit but are
welcome. Canonical source in `src/` and `include/` is authoritative.

Retired with the earlier `src/` tree (at the tag `src-final`; kept here as the record of how it was built): the
standing rename lane (user, 2026-11-09: one lane, counted within the lane total, renames toward
Westwood/TibSun names first, then ra2-decomp, then mischa85 names.csv, then RTTI names, order
changed by the user 2026-11-15; YRpp-only names stay until better evidenced; one class per round;
no source-shape change; whole-file exact build gates every round), the seed tools
(`seed_candidate.py`, `sweep_seeds.py`, `class_inventory.py`, `regenerate_seeds.py` and the
`seed-generated` marker), `compile_candidates.py` acceptance into `src/recovered/`, and Ghidra type
names from `src/shared/ghidra_types.hpp` (the seed and rename tools stay as text in
`tools/original_target/`; they need the earlier tree from `src-final` to run). Renames toward those
sources continue in the tree under the same order.

## Ownership and workers

One owner session integrates: canonical manifest, shared headers, build logic,
`STATUS.md`/`HANDOFF.md`, commits. Workers (up to six, raised from two by the user on 2026-10-21; when the user starts the
recovery goal) take a class or sweep slice, iterate in private scratch
directories and hand back exact candidates plus best-score misses. Serialize
only the compiler/linker bottle (`shared/compiler.lock`, handled by the tools);
analysis and seeding run concurrently. Before shared writes check HEAD/status
and preserve unexpected changes rather than overwriting them.

## Decision policy (user, 2026-11-09)

- Technical rulings belong to the owner: whether a source form is acceptable, which landing option
  to take, whether to revert a row. Decide, record, report; never ask the user these.
- The test, in order: (a) could one real codebase contain this? (b) does every line do real work? No
  dummy variables, dead code or pass-through aliases (renames of existing names). A row whose named
  values fail this is reverted to its clean form and only the dependents that then fail are reverted
  (Is_At); real computed or looked-up values are kept and flagged, with the dependents that fail under
  the natural form recorded in the flag note, whoever the values help ("caller-tuning" alone decides
  nothing); (c) evidence first: TibSun, YR, ra2-decomp twins and real inline helpers before any
  calibration; (d) mark what cannot be proven with a manifest flag the strict audit and STATUS count;
  (e) when a ruling costs bytes, take the option that removes the objection and keeps everything still
  legitimate. In doubt between flagging and parking with no second opinion at hand: park.
- Precedents first: match each case to `docs/DECISIONS.md` (pattern, ruling, reason, example row) and
  decide; a large cost under a fitting precedent is still the owner's call.
- Second opinion (user, 2026-11-09): only when no precedent fits or the case changes the build or
  acceptance model. Send such cases to the Recovery process review session at the next status or
  landing point, bundled with anything else queued (not one at a time; about one message per three
  hours at most; a result that decides several lanes' next work may go at once), each with evidence
  paths and a recommendation; hold the status quo for a queued row
  (no landing, no revert) while lanes work other rows; add each answer to `docs/DECISIONS.md`. Show
  the "Weekly · Fable" usage in each status; above about 15% in a week, tell the user and send only
  build-model cases until the reset.
- Only these go to the user, in plain words with what each answer costs, never blocking work:
  downloads, licences, reading or adopting third-party files; publishing or messaging anyone; usage
  beyond the pacing plan, worker counts or Windows worker spend; changes to the goal, the fixed
  constraints or this policy; anything irreversible.
- Every status carries a plain decision log (one line per ruling since the last status: what, cost or
  gain, reviewed or not) and the judgment-call bytes from STATUS beside Reproduced .text. Every ruling
  stays reversible (drafts kept, flags set). Every lane hand-back states "Calibration used: yes/no".

## Status format (user, 2026-11-09)

Every status (each landing and lane hand-back) uses this order, in plain words, with a
function's name rather than its address where it has one:
1. What happened since the last status.
2. Measures table (user, 2026-11-22), this tree first, src-final as the frozen reference column:
   Reproduced .text % (like for like); C/C++ bodies matched, padding separate
   (bytes, %, gap to main in KB: the parity line, kept until "matched in src-final, not yet in
   the rebuild" is zero); exact with no judgment call (bytes, %);
   judgment calls (rows, bytes, one named definition for both trees); tail (parked functions and
   bytes, count and bytes of those >= 1 KB; variations held for the user's list, rows and bytes);
   since the last status (+rows/+bytes split by Fable lanes, Opus lanes, Windows workers;
   lane-hours; KB per Fable lane-hour); structure (duplicates, externs, address-suffixed, one
   line, any rise flagged); usage (all models % and Fable % with reading time, projected day of
   the Fable stop; Codex credits against the 5,000 floor); disk free and swap only when past a
   guard; the Public snapshot line. Main's in-scope C/C++ % and provenance a/b/c appear in
   checkpoints only. Any figure compared across the two trees names its measure once in STATUS;
   a measure change is a marked series break in progress.csv, never a silent one.
3. Decision log: one line per ruling, with cost or gain and reviewed or not.
4. Lanes running, one line each; Windows deliveries pending.
5. Anything that needs the user, last and numbered.

Between statuses, write to the conversation only when something significant happens: a failure
that changes the plan, a ruling, or something that needs the user. No running commentary on
commands or tool steps. When the conversation is summarized, copy
the latest full status into the summary word for word.

## Controls

The prepared goal text lives in `docs/ORIGINAL_TARGET_GOAL.md`; do not create,
start, resume or change a goal or automation unless the user asks. Honor
pauses. Routine continuation does not authorize downloads, publication,
messages, system/Xcode/DRM changes, license acceptance or global tool changes.
Honor `DEVELOPER_DIR`; use the existing local toolchain and Wine host (see the build doc).
