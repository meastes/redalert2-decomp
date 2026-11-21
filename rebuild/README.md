# Rebuild tree

This folder holds the standing rebuild effort (started 2026-11-17 as a three-day trial; the branch keeps
the name `rebuild-trial`). It rebuilds the Red Alert 2 1.006 `game.exe`
from a new source tree laid out like the original: one source file per original translation unit
(`src/<unit>.cpp`), one header per class (`include/`), and no per-unit views or feature switches. A
function counts only when the original compiler (VC6) turns its source into the exact original bytes,
and the tree as a whole must pass the whole-file exact build.

## How it relates to the rest of this repository

- **Main tree** (`src/` on `main`): this project's exact-match record, and the code the native ports build
  from. The rebuild never edits it. It serves as the answer key: function bodies taken from it are
  provenance class b.
- **mischa85/RedAlert2**: a separate matching decompilation of the same executable. Most of the text here
  is adopted from it, whole files, credited per file. Its module map decides where each unit's functions
  lie. `manifest/adopted.csv` lists every adopted file with origin and commit; licences are in
  `LICENSE.md` and the repository's `THIRD_PARTY.md`.

**Compiler version.** Everything here is checked with VC6 SP3 (compiler build 8447), the compiler of the
original. mischa85's project used the RTM code generator by accident and is moving to SP3 8447. Adopted
text that matched there under RTM is proven again here under 8447. A function mischa85 claims that fails
here under 8447 is recorded as an RTM-only claim, not as one of our parked functions.

## Where the bytes come from

Every accepted function has a provenance class in `manifest/provenance.csv`:

| Class | Meaning |
| --- | --- |
| a | text adopted from mischa85 (or the TibSun archive / ra2-decomp it builds on) |
| b | the body from this repository's main tree, reshaped to this tree's names and headers |
| c | written fresh from the Ghidra seed and the class layout, without either reference |

The percentage this tree reproduces is mostly adopted work (class a). Read it together with the split
below.

<!-- numbers: updated at each gated landing -->
**At the last gated landing (2026-11-18 04:22Z):** 262 units, 6,347 functions exact; 24.61% of `.text`
built from source, 27.42% reproduced with the linked library and assembled bytes.

| Class | Functions | Bytes |
| --- | ---: | ---: |
| a, adopted | 5,582 | 825,651 |
| b, reshaped from main | 10 | 13,456 |
| c, fresh | 82 | 10,400 |
| Windows workers (mostly adopted) | 673 | 78,144 |

Structure: 0 duplicate class definitions, 0 stray externs in landed sources (11 in held units), 4 address-suffixed names
(budgeted: an increase fails the gate), 122 units held, 670 functions not yet exact (443,661 bytes; 115 functions of
1 KB or more, 361,777 bytes).

"Not yet exact" (parked; the checkpoint's measure) is the census definition, `rebuild_census.parked_tail`,
which `progress.py` also prints (lane R43, 2026-11-19): every row of every unit in `units.json` that a check
of the unit's current text and current rows does not find exact, count-sensitive rows apart. Rows with no
text (tailmap's no_draft bucket), rows of a unit whose source is missing and rows of a unit that does not
compile all count. A check is current only when it is of the same source and the same rows (entry and
symbol, `rebuild.last_check`); before this the test was the source alone, so a unit given rows after its
check counted them as parked (2,006 rows / 560 KB on the progress line of 2026-11-19 21:51Z; 1,168 rows /
497 KB, tailmap's count, once the 113 units with such checks were checked again). Functions inside a unit's range with no row at all
(main has no bound for them) are not parked rows: `tools/nodraft_queue.py` lists them as `no_main_row` in
`manifest/no_draft_queue.csv` beside the no_draft rows main has an exact body for (`main_exact`).
Two counts of exact functions sit side by side (review session, 2026-11-20; no rule or credit change;
`rebuild_census.function_level`, the census line "Exact functions", progress.py's `function_level_*` columns):
**whole-unit** (catalogued, whole-file exact) is the catalog, the count every percentage above uses;
**function-level** (each function checked on its own, as ra2-decomp reports) adds every row its unit's current
check finds exact in a unit not yet catalogued (held units), the count-sensitive rows (exact in their probed
window) and the rows tailmap records as exact with their guard open (`exact_when_open`). At the first
measurement (2026-11-20): whole-unit 16,457 rows, 2,917,311 bytes; function-level 16,940 rows, 3,028,147 bytes
(+314 rows / 76,500 B in held units, +2 / 14,320 count-sensitive, +167 / 20,016 exact with the guard open).
The newest numbers are always the last `rebuild-trial` line of `manifest/progress.csv`, written only at a
gated landing (column `tree`; older rows from worker branches are marked as such).
Series break: before 2026-11-18T16:07Z `bytes_exact` counted only the lane's catalogued rows, while
`source_share_of_text` and `reproduced_text` already used every catalogued row (workers included); from that
row on, `bytes_exact` counts every catalogued row and divides out to the share. The two percentages are
comparable across the whole series; `bytes_exact` is comparable only from 16:07Z.
Series break (lane ASM1, 2026-11-20): from the first row with the `inline_asm_*` columns, `source_share_of_text` leaves out the body bytes of evidenced inline assembly (its own category, as on main; conquer Main_Game and lcw LCW_Comp, 1,187 B, were in it before; the row's `note` says so); `reproduced_text` still counts them. `helper_asm_*` counts the rows reached through a file-static helper of their unit (DECISIONS, 2026-11-20).

## Working the parked tail

Lane guidance (lane L1, 2026-11-19). Before working a parked row, read three things:

1. Its bucket in `manifest/tail_buckets.csv` (`tools/tailmap.py`; `tools/tailmap.py --notes` prints the summary
   without compiling) and the compiler notes for that bucket from `manifest/bucket_notes.csv`. Each bucket lists
   the sections of main's `docs/VC6_MATCHING_NOTES.md` that address it; read only those, one at a time:
   `python3 tools/original_target/notes_index.py --section "Register allocation"` (the index of every section is
   `docs/VC6_MATCHING_INDEX.md`). In short: inline_call and not_emitted are inlining questions ("Inlining and
   translation units", "/Ob2 inline decision rule", "Free and charged inline callees", "Named locals and the
   inline budget", "Out-of-line helpers", "Inline constructors and template members"); register_order is
   "Register allocation", "Call-argument scheduling" and the declaration-count sections; stack_layout is "Stack
   frame layout (Study42)" and the frame-slot sections; unbound is binding ("Literals, function statics and data
   order", "Select-any COMDAT copies"); calls_differ, no_draft and text_if0 start from the reference texts
   ("Westwood source idioms", "From the ra2-decomp guide", "YR twin adoption").
2. Its history on main in `manifest/tail_history.csv` (`tools/mainhist.py`, no compile): main's accepting commit
   and date, flags, miss-index tries, the sessions whose commits name it, the notes sections that cite it, and
   "first-try on main" or "hard on main". A hard row's main source (`main_source`) and the sessions named are the
   first evidence; a first-try row that is hard here points at this tree's context (headers, unit order, a
   missing neighbour), not at the row's text.
3. For main's 96 never-matched functions, the guard label of the rebuild draft carries main's best score, tries
   and causes ("main: best N, tried N times (...)"); a draft that does not beat it is not a finding.
4. Its Yuri's Revenge twin in `manifest/tail_twins.csv` (`tools/twinmap.py --write`, lane L2, no compile):
   identical or near twin in gamemd.exe 1.001, whether ra2-decomp matched it, their path, line and symbol, and
   how close our text already is to theirs. `manifest/twin_queue.csv` lists the rows with a matched twin by
   bytes; `tools/twinmap.py --headers` writes `manifest/twin_header_diffs.csv` (classes named by, and headers
   included by, rows whose text is already the identical twin's; their header set is the pinned TibSun archive
   with their `include/` over it). For an identical twin, YR-only members and field differences are expected:
   the bodies equal means every offset the row uses already agrees.

Compiler behaviours found here go back to main's notes, tagged "found in the rebuild (lane R...)", with the
index regenerated (`python3 tools/original_target/notes_index.py --write docs/VC6_MATCHING_INDEX.md`).

## Rulings

- **Rows main matched only through hand-made variations stay parked** (user's preference, 2026-11-20).
  `manifest/variation_candidates.csv` lists the parked rows main reached only through coined unit-local helpers,
  wrappers, named constants, aliases or count-calibrated specialisations (unit, entry, function, bytes, main's
  variation, routes tried, notes). They are not worked toward main's variation during normal work. They land
  only at the end, on the user's word, flagged as judgment calls.

- **Alignment padding from the aligned split of the row's own pass** (lane R4, 2026-11-18). A row that
  main froze without padding, whose object carries alignment NOPs, takes them as padding when the
  aligned split of the same frozen pass (`<pass>-aligned`) holds the same bound and a gap row classed
  `alignment_nop_padding_split_from_unassigned_gap` right after the body; the row is then bound to that
  population (`gap_population` in `units.json`, `bound_population` in the catalog). Precedent:
  `docs/DECISIONS.md`, "Rewritten initialisers, stale table names, coopcamp, aligned bounds" (4). The
  bound and the padding are independent of the candidate; the audit compares the full extent.
- **The head of a unit's object contribution** (lane R4). `rebuild.py init --auto` also takes the rows
  before mischa85's range that its map leaves to no named module and that main files under the unit's
  source (initializers, then class-factory or constructor code), never reaching another unit's range.
  Drafts guarded as unplaced are tried again without their guard (`tools/unguard.py`) and kept unguarded
  only when the unit gains exact rows and loses none.
- **`MapClass::Place_Firestorm_Wall` and `Place_Wall` take a non-const `BuildingTypeClass *`** (lane R4).
  The original's mangled names say so (`...PAVBuildingTypeClass@@@Z`, mischa85 names.csv at 0x56A480 and
  0x56A660), so `map.h` follows them; house.cpp's caller drops the `const` from its local (`btype`),
  which leaves its code unchanged.

- **A function our object emits that the image lacks takes the linkage that does not emit it** (lane R6,
  owner via the review session, 2026-11-18). The original link kept unreferenced code: main's accepted
  "uncalled copies" rows are /Gy COMDATs with no reference anywhere in the image (0x56D620/0x56D640/0x56D660,
  0x5E5130/0x5E5140/0x5E8120/0x5E86C0), and byte-identical ones sit at distinct addresses (0x56D620 and
  0x56D660; 0x63B890 and 0x63C640), so neither /OPT:REF nor folding ran (the debug directory is a MISC
  .dbg record, a /DEBUG link). A helper that every exact row expands inline and that no row of the unit's
  fully covered range matches was therefore never emitted out of line: it is declared `inline` (in its class
  where ra2-decomp or the TibSun archive has it there, else at its definition), never kept as an external
  function. Applied with every row of the unit unchanged: ownrdraw's 19 helpers, `TerrainClass::Is_Animating`
  (in the class, as ra2-decomp has it), `~AlphaLightingRemapClass`.
- **subtitle and subtitle2 are two objects** (lane R9, owner ruling on R8's open item). mischa85 matches
  them as two modules (modules.csv: subtitle 0x69B190-0x69B680, subtitle2 0x69B680-0x69C180), but its
  module check leaves initializers to unassigned units, so it does not test the missing RECT_NONE
  initializer at 0x69B680. Tested here under 8447: one translation unit (subtitle.cpp, then subtitle2's
  text) makes `MovieSubtitles::Load_Sub_Titles` (0x69B300) expand the parser constructor inline, which the
  original calls out of line (35/36 rows), so the parser is a separate object, as mischa85 says. Its
  object therefore does not include rect.h: `SubTitleParserClass` moved from movies.h into its own header
  (`subtitleparser.h`, which movies.h includes), subtitle2.cpp includes that instead of movies.h, and the
  extra initializer is gone (27/27 rows, no stray function).
- **Explicit template instantiations that emit functions the image lacks are dropped** (lane R9, the
  select-any ruling). subtitle2's `template class DynamicVectorClass<SubTitle *>` / `VectorClass` and
  savever's `template class _com_ptr_t<...IStream...>` emitted every member (Add_Head, Insert,
  GetActiveObject, ...), several with no copy anywhere in the image (keptcopy.py: "a source question").
  Without them the units emit only what their code uses (the vtables pull the virtual functions in), all
  rows stay exact, and no unbound function remains.
- **`AbstractClass::operator=` (0x56A9F0) is emitted by map** (lane R9). Its only callers (0x54863F,
  0x548E51) lie in `MapClass::Set_Map_Dimensions` (0x5484E0, mischa85's map module, main's map.cpp), and
  the copy lies inside map's object contribution (map's object binds its own template copies from 0x56AA70
  through 0x56CAF0). map's range now runs to 0x56AA70: 0x56A9F0 and `TPoint2D<short>::operator+=`
  (0x56AA20, the same caller) are map rows reported missing until Set_Map_Dimensions (no independent
  bound yet, guarded) compiles, and `TPoint2D<short>::operator-` (0x56AA40) is exact.
- **infatype's SequenceName is RA2's 38-entry table** (lane R9). The original's table at 0x7DAF60 holds
  38 keys (TibSun's first 16, then Tread, Swim, WetIdle1/2, WetDie1/2, WetAttack, Hover, Fly, Tumble,
  FireFly, Deploy, Deployed, DeployedFire, DeployedIdle, Undeploy, Cheer, Paradrop, AirDeath*, Panic),
  sized by the unit's DOINFO_COUNT; ra2-decomp's YR list is the same with four more. TibSun's 21-entry
  array was the build-refused data section.

- **Byte-identical template copies are bound by the order the unit emits them** (lane R11).
  `TVelocity2D<double>::Speed` and `TVelocity3D<double>::Horizontal_Speed` compile to the same bytes, and
  aircraft's object carries one copy of each (0x41B750, 0x41B830). The compiler emits a copy right after the
  first function that calls it, in reverse call order, as the 3D pair shows (Set_Speed calls Nonzero_Check,
  then Speed: Speed 0x41B7C0, Nonzero_Check 0x41B7F0): Get_Pitch (0x41B6E0) is followed by Horizontal_Speed
  (0x41B750) and Set_Yaw's Nonzero_Check (0x41B860) is preceded by Speed (0x41B830). Main's population names
  them the other way round (its own header); `manifest/symbols.csv` binds them by this order. No exact row
  elsewhere calls either.

- **A unit defines its own globals** (lane R13). wonline's text declared about ninety globals `extern` that
  the original keeps in wonline's own data (TibSun's wonline.cpp defines the same names); they are defined in
  wonline.cpp, with TibSun's initial values, and the dynamic-initialized ones in the order of the unit's
  initializer table (ra2-decomp's wonline.cpp gives the order, main's population confirms it). Names already
  declared in globals.h are not redeclared. The chat event sinks, Startup_Chat and Shutdown_Chat need the ATL
  headers, which this tree does not pin (DECISIONS row 22), and stay guarded.

- **wonline's g_OLEInitializer stays unplaced** (user decision 2026-11-18, `docs/DECISIONS.md` on main, last
  row; lane R14). TibSun's empty OLE object exists in the original: its initializer 0x73A8E0 (OleInitialize,
  atexit) and destructor 0x73A900 (OleUninitialize) are exact rows. Nothing in the image refers to the
  1-byte object, and VC6's hash-ordered .bss gives no unique slot for it among wonline's unreferenced gaps.
  The unit declares it `data_bound_by: unplaced-unreferenced` with the evidencing rows
  (`data_bound_by_rows` in `units.json`, copied to the catalog rows); the audit
  (`tools/original_target/data_bindings.py`, `unplaced`) refuses unless no relocation targets it, the rows
  are accepted rows of the source, it lies in .bss, and the unit's unclaimed, unreferenced bss gaps
  (recomputed at every build) hold all such symbols jointly. It earns nothing; the census and the progress
  line count it. Each further use needs the owner.
- **`TVelocity3D(T x, T y, T z)` builds its base from x and y directly** (lane R14; ra2-decomp's
  include/velocity.h:74). TibSun's form, through a `TVelocity2D<T>(x, y)` temporary, leaves
  `AircraftClass::Fire_At` (0x415790) 8 bytes of frame short; every other row of the tree is unchanged.

- **DoType has RA2's 38 sequences; rows that flip with it by declaration count are "exact form,
  count-sensitive"** (review session, 2026-11-18; lane R20). `do.hh` lists the 38 keys of SequenceName
  (Tread .. Panic); infantry spells its raw sequence numbers by these names. Two unflagged rows whose exact
  text depends only on the number of declarations before them (wonline's Draw_Player_List, sidebar's
  SelectClass::Action) are guarded with the label "exact form, count-sensitive" and listed in their unit's
  `count_sensitive` in `units.json` with the probed windows; the census and the progress line count them
  apart from the parked tail. Headers are never padded to move a count: `tools/countsens.py [--write]`
  tries every such row again at a landing whose headers changed and takes the guard off when the row is
  exact again with nothing lost. Encode_Game_Options (judgment-flagged) is parked as an ordinary draft.
- **BlitTransLucent50<unsigned short>::BlitForward is the original's inline-assembly copy** (owner, lane R20).
  The member is declared in the class, TibSun's loop is a guarded out-of-class draft, and the vtable slot
  binds to 0x487080; likewise the voxel draw functions the VoxelDrawFunctions table points to bind from its
  entries.
- **Data no exact code refers to: layout by run, pending ranges** (review session 2026-11-19, `docs/DECISIONS.md`
  "Rebuild data no exact code refers to"; lane R25, mapgen). A: `data_bound_by: layout` falls back, when the whole
  section has no one base, to its nearest reference-bound neighbours in the same section (both sides, one base, no
  pending range between them, nothing else bound inside the derived extent); the bytes are compared as before.
  B: `data_pending` [{address, size, owner}] records data the original holds only because a parked function emits
  it (mapgen: Read_RMG_INI's static pointer at 0x7E0BE4); never defined, no credit, refused once its owner is an
  accepted row; the census prints the count. No compare cut: the 4-byte gap after Init_Map's float
  `_waterfall_chance` was declaration order. VC6 emits a unit's initialised constants (file-scope and
  function-local alike) in text order, one run per declaration group; with `_waterfall_chance` at file scope just
  before Init_Map (ra2-decomp's mapgen.cpp:3254) it lies directly before Init_Map's size tables as in the image,
  and MapRegionClass::Get_Angle_Score declares its three weights as the image holds them (random 2.5, length,
  angle; main's three statics). The image orders the runs differently from our object (Init_Map's run before
  MapRegionClass's), which the per-symbol compare does not depend on.
  mapgen also takes main's `1.0 / 4294967295.0`: VC6 folds `1.0 / UINT_MAX` to 2^-32, a different literal at the
  same bound address (the row compare binds literals by address; the build compares their bytes).

## Structure gate

`tools/rebuild.py catalog` refuses to land a tree that breaks the structure budget in
`manifest/structure_budget.json`. The tree-wide checks: zero duplicate class definitions (counted by
qualified name), no source outside a unit, no free functions standing in for class members, and no tool
data in sources. Each unit is also checked on its own: a new unit may not add stray `extern` lines or
address-suffixed names.

Budgets that may only fall (review session, 2026-11-18; the census prints them on two `Gate` lines):
process chatter in rebuild/src and rebuild/include (lane, worker, manifest or tool mentions: zero),
`Unknown*` names (distinct identifiers that begin with `Unknown` or `unknown`, outside comments, in src
and include; COM names such as `IUnknown` are not counted), `vt_entry_NN` slots, `field_NN` and `FUN_`
names in headers, and the inherited constructs still to be evidenced: `#define GEOMETRY_H` in sources,
`#pragma auto_inline(off)` and hand-written `extern "C"` blocks in sources.

`RA2_NON_MATCHING` draft guards are not a budget (owner ruling, 2026-11-18): they are the parked tail,
and a new guarded draft is a legitimate parked attempt, so a merge that adds guarded drafts is never
refused for it. The census reports the guard count on its own line beside the parked tail (rows not
exact and their body bytes).

Judgment calls are flagged as main flags them (AGENTS.md Decision policy; owner ruling, 2026-11-18): a real
computed or looked-up value kept in the form that matches carries a `judgment` note in
`manifest/provenance.csv` naming its evidence (a twin's text or main's accepted form); a new flagged form
without such evidence is not landed. `tools/provenance.py --judgment=ENTRY:NOTE` sets the flag and keeps it
when the unit's lines are rewritten. The census reports the flagged rows and their body bytes on the
"Judgment calls" line, and the progress line carries them.
A hypothesis in a shared class sits on the header member, not on a row that depends on it (review session,
2026-11-19): `manifest/header_judgments.csv` (header, member, judgment, dependents) lists such members
(RulesClass::EMPulseSparkles and OreTwinkle), and the census names them on the "Judgment calls" line. A control-flow form (goto,
loop shape, early return) chosen because it reproduces the original's branches is matching iteration, not a
judgment call: no flag, the note stays in provenance.

Unit-local copies of a shared helper (review session, 2026-11-19) are a budget that may not grow
(`helper_copies`): the census counts inline functions in a unit source whose name holds, as whole words, the
name of an inline free function a header defines (Aircraft_Adjacent_Cell, Adjacent_Cell_L, Tunnel_Move_Coord),
and lists them on their own Gate line.

Statements kept only to steer the inliner (review session, 2026-11-19) are a budget that may not grow
(`inliner_statements`, 0): an explicit trailing `return;` or a like statement that does no work and only stops or
forces an expansion is a last resort, after definition order, the caller's inline budget (`tools/inline_probe.py`)
and the reference texts are checked; it carries a judgment note starting `inliner:` that records those checks,
and the census counts such rows on their own Gate line. The first one (Reset_Buddy_Ladders) is gone: its cause
was upstream, main's helper standing in for `Request_Game_Channel_List(1, false)` (lane R33).
An uncalled out-of-line copy whose body other rows expand is one ordinary definition at the place its address
gives (review session, 2026-11-19): /Ob2 expands a function of the unit at its call sites, even one defined later
in the file, and the link keeps the unreferenced body. Inline twins of such a function are dropped
(wonline's Set_Match_State, Reset_Wait_Events, Is_Tournament_Session, Select_WDT_Location; lane R33).

Names the project coined (descriptive, not from TibSun, ra2-decomp, mischa85 or RTTI; main's own descriptive
names included) are listed in `manifest/provisional_names.csv` (address, name, unit, reason) until an evidenced
name replaces them; the census prints the count on the "Provisional names" line (review session, 2026-11-19).
mapgen's six uncalled helpers 0x56D5D0-0x56D680 (ra2-decomp's FUN_0058BCD0..BD80, ordinary functions there too)
are defined once at their addresses with such names (owner ruling, lane R33).

Functions the tree declares that RA2 never defines (review session, 2026-11-19; lane R45's In_Radio_Contact,
Sound_Effect and Speak) are a budget that may only fall (`declared_never_defined`): `tools/declared_never_defined.py
--write` lists in `manifest/declared_never_defined.csv` every declaration with no definition in rebuild/, no symbol
the image is known to hold (main's catalog and symbols, mischa85 names.csv, symbols.csv, units.json) and no
import, CRT, pure-virtual, operator or constructor exemption, with its users (exact, parked, count-sensitive) and
what the image calls instead where known; one main defines or whose comment cites its address is "not yet carried"
(`--not-carried`), not listed. A declaration leaves its header when its last user calls the image's routine.

## House format

New text is written in the layout of the adopted Westwood/Tiberian Sun text: `rebuild/.clang-format` describes
it, and `tools/fmtnew.py` applies it at the owner's landing to the lines a change adds or modifies, never to
whole files, so the adopted text keeps its own layout. Only whitespace edits that keep every line where it
was are applied, because code that reads `__LINE__` (64 places) compiles differently when a line moves. Each
touched unit is checked again; a unit whose rows change is restored unformatted and its lines are listed in
`manifest/format_exempt.csv`. The census line "new lines not in house format" is gated (zero) when
`RA2_FMT_BASE` names the landing's base, and reported otherwise.

Habits the formatter does not enforce (lane and worker briefs): `return(x);` with no space;
`if (x) {` with the brace on the statement line, functions and classes with the brace on its own line;
`Type * name` and `Type & name`; `operator == (`; `NULL` for pointers, `(void)` for empty parameter lists;
comments in sentences, `//` at the end of a line, no column alignment by spaces; split long calls only where
the adopted text does.

## Layout

- `src/`: one file per original unit.
- `include/`: headers, one per class.
- `manifest/`: units and their rows (`units.json`), the catalog the build reads, provenance,
  adoption index, progress, time log and worker rows.
- `tools/`: adopt, check, catalog and census tools. They need this repository's private inputs (the
  original executable and the compiler), which are not published.
  `tools/ptrbind.py UNIT` binds an unbound data symbol of the unit (.bss/.data) that a bound data section of
  the same object points to, from the original pointer at that field (every pointer must agree; the audit
  compares it again); first used for auddev's `_audioDataStruct` (codex2 landing, 2026-11-18).
  `tools/keptcopy.py` also binds a select-any template copy when the image holds several copies byte-identical
  to ours (every relocation bound and agreeing), in the unit's own table with the count in `kept_copies`.
  `tools/tblbind.py UNIT` binds data tables made only of pointers to string literals (which databind cannot
  place): each literal where its own bytes occur exactly once, then the table filled with those addresses where
  it occurs exactly once; anything not unique is refused (owner ruling, 2026-11-18). First used for mainopt's
  label tables and options' HotkeyName (codex2 landing).
  `tools/held.py CATALOG_LOG [--write] [--note UNIT=KIND:TEXT]` refreshes `held_reason` and `held_note` on
  every pending unit of `units.json` from a catalog run's "left out" lines (worker, header, tooling,
  mismatch, binding, owner) and prints the breakdown by kind with bytes; header and tooling notes are set by
  hand with `--note` and kept until the unit lands.
  `tools/order_search.py UNIT ENTRY [--split] [--alt ALTS.json]` searches ordinary matching iteration for one
  function in a scratch copy of its unit (the tree's file is not written): the order of adjacent independent
  statements and declarations, declared-then-assigned locals, pre/post increment, for/while form, and statement
  texts from the twin, the archive or main given in ALTS. Each variant is compiled under the compiler lock and
  scored; the ranked table goes to `build/r/order_search/`. It never adds a statement, a local, an alias or a
  cast (lane O12, 2026-11-21).
  The catalog also refuses a unit whose compiler-output rows (`_$` atexit destructors, which are not in the
  .CRT$XCU table and cannot be guarded) differ, the test `tools/held_units.py` makes: the whole-file build would
  refuse their section as unaccounted code (lane B4's finding; lane M1, 2026-11-20).
