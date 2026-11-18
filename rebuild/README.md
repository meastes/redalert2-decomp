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

"Not yet exact" (parked) counts the functions that are not exact inside processed units: units whose
current text has a check here. Units with no text yet, or a worker's unit not merged, are not counted.
The newest numbers are always the last `rebuild-trial` line of `manifest/progress.csv`, written only at a
gated landing (column `tree`; older rows from worker branches are marked as such).

## Rulings

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

## Layout

- `src/`: one file per original unit.
- `include/`: headers, one per class.
- `manifest/`: units and their rows (`units.json`), the catalog the build reads, provenance,
  adoption index, progress, time log and worker rows.
- `tools/`: adopt, check, catalog and census tools. They need this repository's private inputs (the
  original executable and the compiler), which are not published.
