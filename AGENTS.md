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
  using addresses. Base RA2 only; Yuri's Revenge is a later separate target.
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
  enforced by the strict audit and reported as its own category. Exception
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
  `src/`, authored tools/tests, byte-free manifests, docs.
- **External references:** public/recomp/YRpp material supplies hypotheses only;
  verify on the exact target and check licenses (`THIRD_PARTY.md`). Exception
  (user, 2026-11-06; extended 2026-11-09): text from ra2-decomp, the TibSun archive,
  mischa85/RedAlert2 and electronicarts/CnC_Renegade may be adopted with attribution
  (`THIRD_PARTY.md`);
  acceptance is unchanged. Standing pulls (user, 2026-11-09): at each landing
  point pull `private/reference/ra2-decomp` and `private/reference/mischa85-RedAlert2`
  (at most hourly) and report functions they match that we have not accepted.
  Do not adopt a lifted runtime or OpenRA gameplay.
- **Native phase:** no native/SDL/arm64/gameplay check per recovery batch.
  Preserve `historical/native-reconstruction/`. The shipped game will be
  native arm64 with no Wine/CrossOver, Rosetta, VM, emulation or original code.
  User decision (2026-11-08): one dedicated native lane runs alongside the exact
  lanes; at five or six lanes, two native lanes, provided they work on separate parts of
  `native/` (each brief names its area, one lands at a time, neither edits `src/`); otherwise
  the extra lane is an exact lane (user, 2026-11-09). It builds the native tree from
  `src/recovered` and `src/shared`; unmatched functions are carried as their best
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

1. **Pick by class or family, not by single function.** `class_inventory.py`
   ranks vtables by remaining bytes; `sweep_seeds.py` ranks every unrecovered
   function by how close its Ghidra seed already compiles. Recover a class's
   layout once from its constructors/vptr stores, put it in `src/shared/`, then
   sweep its slots and constructors with those types.
2. **Seed, then iterate.** `seed_candidate.py` turns the Ghidra export into a
   compilable scratch candidate; `diff_candidate.py [--watch]` compiles it with
   the real compiler and shows an aligned, scored instruction diff. Iterating on
   near misses (statement order, temporaries, types, expression shape, inline
   splits, option variant) is the normal work, not a forbidden "steering". A
   miss is never "closed": record its best score and move on when the time box
   (about 30 minutes per KB of body, capped at four hours) runs out; anyone may resume it.
3. **Accept in bulk.** Promote exact candidates through `compile_candidates.py`
   (strict audit) into `src/recovered/` and the canonical manifest. No
   separate reviewer is required for source, tools or build changes; the
   strict audit and the exact whole-file build are the checks.
4. **Build periodically.** Run the full `./build.command` before each commit
   that changes accepted source or build logic, not for every candidate.
5. **Record briefly.** Commit accepted source with a short message. Regenerate
   numbers with `tools/original_target/status.py --write`; update `HANDOFF.md`
   with what is in flight and the next target. No per-batch cost receipts, hash
   tables or prose ledgers; the manifest and git history are the record.

## Readable source

The executable carries MSVC RTTI: original class names and inheritance for
every polymorphic class (`docs/CLASS_HIERARCHY.md`, `manifests/rtti_classes.json`).
Class-owned sources live in `src/recovered/classes/<Class>/`; use the original
class names for shared layouts and members.

Name understood operations, parameters and fields as you match them, with the
original address as a suffix; keep honest `unknown_0xNN` names for the rest.
Prefer real shared types over per-function owner classes and register-named
locals. Existing transliterated sources (`unsigned int ecx = ...`) may be
replaced whenever a class sweep touches them, provided the replacement still
matches exactly. Formatting and renaming earn no matched-byte credit but are
welcome. Standing rename lane (user, 2026-11-09): one lane, counted within the lane
total, renames toward Westwood/TibSun names first, then mischa85 names.csv, ra2-decomp
and RTTI names; YRpp-only names stay until better evidenced; one class per round; no
source-shape change; whole-file exact build gates every round. Canonical source in `src/recovered/` is authoritative. Accepted seed sources
carry a `seed-generated` marker line: `regenerate_seeds.py` may rewrite marked
files (the build proves they still match); delete the marker when editing a
file by hand and it is never regenerated. Ghidra type names come from
`src/shared/ghidra_types.hpp` until real types replace them.

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
2. Measures table: Reproduced .text %, in-scope C/C++ %, judgment-call bytes and rows, usage
   per account with Weekly · Fable %.
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
