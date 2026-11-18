# Rebuild trial tree

This folder holds a three-day trial (started 2026-11-17). It rebuilds the Red Alert 2 1.006 `game.exe`
from a new source tree laid out like the original: one source file per original translation unit
(`src/<unit>.cpp`), one header per class (`include/`), and no per-unit views or feature switches. A
function counts only when the original compiler (VC6) turns its source into the exact original bytes,
and the tree as a whole must pass the whole-file exact build.

## How it relates to the rest of this repository

- **Main tree** (`src/` on `main`): this project's exact-match record, and the code the native ports build
  from. The trial never edits it. It serves as the answer key: function bodies taken from it are
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
**At the last gated landing (2026-11-18 00:28Z):** 190 units, 3,951 functions exact; 14.86% of `.text`
built from source, 17.68% reproduced with the linked library and assembled bytes.

| Class | Functions | Bytes |
| --- | ---: | ---: |
| a, adopted | 3,363 | 481,676 |
| b, reshaped from main | 8 | 11,120 |
| c, fresh | 63 | 6,944 |
| Windows workers (mostly adopted) | 517 | 60,464 |

Structure: 0 duplicate class definitions, 0 stray externs in landed sources (12 forward declarations of
a unit's own globals in units still held, counted apart), 5 address-suffixed names, 187 units held,
662 functions not yet exact (433,926 bytes; 116 functions of 1 KB or more, 352,579 bytes).
The newest numbers are always the last line of `manifest/progress.csv`.

## Structure gate

`tools/rebuild.py catalog` refuses to land a tree that breaks the structure budget in
`manifest/structure_budget.json`. The tree-wide checks: zero duplicate class definitions (counted by
qualified name), no source outside a unit, no free functions standing in for class members, and no tool
data in sources. Each unit is also checked on its own: a new unit may not add stray `extern` lines or
address-suffixed names.

## Layout

- `src/`: one file per original unit.
- `include/`: headers, one per class.
- `manifest/`: units and their rows (`units.json`), the catalog the build reads, provenance,
  adoption index, progress, time log and worker rows.
- `tools/`: adopt, check, catalog and census tools. They need this repository's private inputs (the
  original executable and the compiler), which are not published.
