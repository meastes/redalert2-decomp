# Yuri's Revenge overlap: what our exact RA2 source could offer ra2-decomp

Measured 2026-11-14 (lane K1, analysis only; nothing was sent, posted or pushed). It refreshes
the 2026-11-06 twin measurement against the current read-only clone at
`private/reference/ra2-decomp` (gibbo101/ra2-decomp `f8d8d53`, 2026-11-15 00:37 UTC) and our
catalog on main. It reverses the question: for each YR function ra2-decomp has **not** matched,
do we already hold an exact RA2 function that is its counterpart? Calibration used: no.

## Method

- **Their unmatched set.** This follows their `tools/gamecode.py`. Game code is every
  `data/functions.csv` function outside the library span (`data/library.csv`). A function
  matches when `data/manifest.csv` lists it (`data/progress.csv` lists the same 19,073 rows, all
  MATCH).
- **YR bytes.** We used the same retail `gamemd.exe` 1.001 copy as the 2026-11-06 measurement
  (SHA-256 `7e839d3b…6077`). Their manifest's per-function SHA-256 agrees with it on 19,068 of
  19,073 matched rows. The 5 that differ are matched rows at patched sites their checker restores, so none of them
  is in the unmatched set.
- **Our bytes.** Every accepted catalog row on main (17,907 rows), read from the target
  `game.exe` at its catalog extent.
- **Masking and twin classes.** These are the same as the 2026-11-06 twin map. We mask the rel32
  field of call/jmp/jcc and every 4-byte immediate or displacement that points inside its own
  image. The classes are:
  - **identical:** equal length and equal bytes outside the masks. Switch-table dwords count as
    equal when they are in-image on both sides.
  - **near, layout:** same length, same instruction boundaries and opcodes; only immediate or
    displacement fields differ, i.e. field offsets, vtable slots, sizes and constants.
  - **near, fuzzy:** length within 10% and mnemonic-sequence similarity of at least 0.90.
- **Address order.** 4,815 unique identical twins form a monotonic RA2-to-YR anchor chain. A
  candidate between the YR twins of its neighbouring anchors is "in order".
- **different:** there is no identical or near twin, but a counterpart is established another
  way. Either their `functions.csv` name equals our symbol's Class::Method, or the function is
  the only unpaired one on both sides of its anchor window.

## The counts

ra2-decomp game code: 19,353 functions, 3,815,998 bytes. Matched: 19,072 functions,
3,254,313 bytes. **Unmatched: 281 functions, 561,685 bytes.** Their README gives the same
figures: 19,072 of 19,353, 85% of the bytes. 273 of the 281 have a recorded attempt in
`data/attempts.csv`.

For those 281:

| Our counterpart | Functions | Bytes | Notes |
| --- | ---: | ---: | --- |
| **Exact RA2 function, byte-identical after relocation masking** | **55** | **37,745** | 47 are unique among our rows and at least 16 bytes: **37,187 B**. Their best attempts on these 47 have a median of 80.9 |
| **Exact RA2 function, near-identical** | **76** | **160,954** | layout 22 / 46,121 B; fuzzy 54 / 114,833 B; 75 of 76 in address order |
| **Exact RA2 function, different** | **7** | **11,078** | counterpart by name or position only; mnemonic similarity 0.01–0.90, so some pairings may be wrong |
| Counterpart found, but not exact on our side | 7 | 17,322 | |
| No counterpart established | 136 | 334,586 | |

What we could offer (identical unique rows plus in-order near rows) is 122 functions, 197,219
bytes:
- **identical:** 37,187 B;
- **layout:** 46,121 B (median token similarity 0.97);
- **fuzzy:** 113,911 B (median token similarity 0.90).

Of the 122 rows, 41 still carry placeholder names in our symbols. One rests on
`count_calibrated` and one on `split_declaration`.

Compared with 2026-11-06:
- ra2-decomp's unmatched game functions fell from 505 to 281.
- Then, 171 of our functions (72 KB) had identical twins they had not matched. Now 47 unique
  identical twins (37 KB) remain on their unmatched list.

## Top 20 units by bytes we could offer

Units are their `data/modules.csv` modules; our source is the Route A unit file holding the RA2
function. Bytes are YR bytes.

| Their module | Our source | Functions | Bytes | Identical | Layout | Fuzzy |
| --- | --- | ---: | ---: | ---: | ---: | ---: |
| Queue | queue.cpp | 5 | 17,606 | 0 | 8,680 | 8,926 |
| MapGen | mapgen.cpp | 8 | 17,022 | 14,004 | 0 | 3,018 |
| wonline | wonline.cpp, classes/CChatEventSink/ | 10 | 16,473 | 0 | 959 | 15,514 |
| map | map.cpp | 10 | 14,453 | 4,398 | 7,669 | 2,386 |
| ownrdraw | ownrdraw.cpp | 9 | 13,173 | 1,174 | 7,636 | 4,363 |
| Unit | unit.cpp | 3 | 10,951 | 0 | 0 | 10,951 |
| Startup | startup.cpp (WinMain) | 1 | 10,261 | 0 | 0 | 10,261 |
| hover | hover.cpp | 3 | 7,894 | 0 | 0 | 7,894 |
| PlanMgr | planmgr.cpp | 9 | 7,618 | 3,464 | 1,915 | 2,239 |
| MainLoop | mainloop.cpp | 3 | 6,487 | 40 | 3,507 | 2,940 |
| WDTSel | wdtsel.cpp | 8 | 6,410 | 3,789 | 1,531 | 1,090 |
| House | house.cpp | 3 | 6,185 | 0 | 0 | 6,185 |
| drive | drive.cpp | 1 | 5,892 | 0 | 0 | 5,892 |
| ship | ship.cpp | 1 | 5,768 | 0 | 0 | 5,768 |
| combat | combat.cpp | 1 | 4,710 | 0 | 0 | 4,710 |
| Techno | techno.cpp | 3 | 4,142 | 0 | 1,645 | 2,497 |
| bullet | bullet.cpp | 2 | 4,061 | 0 | 2,757 | 1,304 |
| wdtmap | wdtmap.cpp | 1 | 3,440 | 0 | 3,440 | 0 |
| voxel | voxel.cpp | 1 | 3,290 | 3,290 | 0 | 0 |
| Sidebar | sidebar.cpp | 2 | 3,239 | 0 | 0 | 3,239 |

The largest identical rows:

| YR address | RA2 entry | Their module | Bytes | Their best attempt |
| --- | --- | --- | ---: | ---: |
| 0x0059E740 | 0x0057FF20 (MapGeneratorClass::Place_Waterfall) | MapGen | 6,472 | 53.5 |
| 0x00571490 | 0x005539B0 | map | 3,474 | 23.2 |
| 0x00754CB0 | 0x00718060 | voxel | 3,290 | 97.0 |
| 0x005EF4E0 | 0x005CDF00 (Select_Serial_Dialog) | NullDlg | 2,580 | 72.6 |
| 0x00755DB0 | 0x00719160 (VoxelLibrary::Read_File) | voxlib | 1,762 | 95.4 |
| 0x00591D80 | 0x00573680 | MapGen | 1,724 | 95.8 |
| 0x0058C800 | 0x0056E100 (MapRegionClass::Create_Region_From_Cell) | MapGen | 1,678 | 84.3 |
| 0x00592AF0 | 0x005743F0 | MapGen | 1,342 | 99.5 |
| 0x005A89D0 | 0x0058A1B0 (MapGeneratorClass::Generate_Rural_Roads) | MapGen | 1,321 | 78.2 |
| 0x005A23A0 | 0x00583B80 (MapGeneratorClass::Create_Tiberium) | MapGen | 1,300 | 80.9 |

## What a contribution must look like (their CONTRIBUTING, README, docs/AGENTS.md, CLAUDE.md, COMMIT_GUARD)

- **Licence.** Game source (`src/`, `include/`) is GPL-3.0-or-later, with Electronic Arts'
  Section 7 terms on files that carry EA or OpenTS material. Tools and data are MIT. Ours is
  GPL-3.0-or-later, so our text can go there. Every file in `src/` and `include/` opens with
  their four-line header:
  - `SPDX-License-Identifier: GPL-3.0-or-later`;
  - "Copyright 2026 ra2-decomp contributors";
  - two EA/OpenTS lines, only when the file derives from that material.

  Code ported from another project names its source and licence in the pull request.
- **Scope.** Only matched functions and the header and tooling work that supports them. No
  fixes, features, refactors or ports. Bugs stay as shipped and are listed in `docs/BUGS.md`.
- **Claims.** Check the pinned "In progress" issue, then claim an address range with a comment
  on it before working. Their batch tools skip claimed ranges and every function in an open
  pull request.
- **File layout.**
  - One module or one closely related set of functions per pull request.
  - Each function goes in the `src/` file of its module (`data/modules.csv`), in address order.
  - Each function carries a reccmp marker, `// FUNCTION: GAMEMD 0x<address>`.
  - Indentation is tabs. Comments are at most two `//` lines and state behaviour only: no
    names, dates, decisions or history.
  - Keep pull requests independent rather than stacked. Do not rebase or force-push once one
    is open.
- **Names and headers.**
  - Callees and globals are named as their `tools/ctx.py` gives them (YRpp or TibSun names).
    Unnamed ones are `FUN_<ADDR>` / `DAT_<ADDR>`.
  - Types come from their `include/` headers, keeping TS member names at YR's offsets. No local
    copies of types the headers already define, and no stand-in structs.
  - Each callee's calling convention is declared as the disassembly shows it. Under `/Gr`, a free
    function is `__fastcall` by default.
  - Each object reached into gets a `data/names.csv` row.
- **Honest matches.**
  - No inline asm, `__emit`, `#pragma optimize`, hard-coded addresses or `volatile` tricks. The
    exceptions are Westwood's own constructs copied unchanged from the TibSun archive (or
    another EA Westwood release).
  - No no-op constructs, and no casts that only make a guessed type compile.
- **Verification.** VC6 SP3 (C2.DLL 12.00.8447) with the fixed flags
  `/O2 /Ob2 /Gr /GR /MT /DWIN32`. `uv run tools/check.py src/<file>` must print MATCH. Maintainers
  rebuild and re-check every pull request; a pull request's own "N of N match" counts only once
  their checker agrees.
- **Tooling and AI.**
  - The project is openly AI-assisted (Claude batches under human direction). Pull requests from
    people go through the same checker and rules, and AI-made work is not barred.
  - Their CLAUDE.md requires the maintainer's explicit opt-in for running agents in parallel (a
    workflow) inside their repository.
  - The commit guard (`tools/hooks/`) must be on. It blocks non-no-reply author emails,
    home-directory and mount paths, secrets, any `MZ` binary and the contributor's listed
    private strings.
  - Never commit game data, executables, disassembly or decompiler output.

## What our source would need to change to fit

1. **Headers.** Our `src/shared` layouts, `ghidra_types.hpp` and per-function view or owner
   classes (`Leaf_*`, `Unknown*`) are stand-ins under their rules. Each function has to be
   rewritten against their `include/` headers, whose types use YR offsets. For layout twins
   this is the whole job: the field offsets and constants that differ come from YR's layouts.
2. **Names.** Our placeholder and address-suffixed names (41 of the 122 rows) become their
   ctx.py names, or `FUN_`/`DAT_` names for callees they have not named.
3. **Conventions.** Their `/Gr` makes free functions `__fastcall` by default. Our free functions
   compiled under `/O2 /Ob2 /G5 /Gy` need explicit `__cdecl` where the exe is cdecl, and lose
   explicit `__fastcall`.
4. **Text form.** Every file needs:
   - reccmp markers instead of our `// signature (0xADDR)` comments;
   - tabs;
   - comments cut to two behaviour lines, dropping our lane, ruling and address notes;
   - their header and attribution lines.
5. **Rulings that would not carry over.** Count-calibrated locals (1 row) and split
   declarations (1 row) fail their "no steering" reading and would have to be re-derived.
6. **Fuzzy rows.** YR changed the code: extra calls, inlined callees, extra instructions. Our
   source is a seed only; each one is ordinary matching work under their checker.

## Effort estimate (lane-hours per KB of YR body, 1 KB = 1,000 bytes)

| Class | Bytes | h/KB | Lane-hours | Basis |
| --- | ---: | ---: | ---: | --- |
| Identical | 37,187 | 0.3 | about 11 | See the note below |
| Layout near | 46,121 | 0.75 | about 35 | Identical port plus re-deriving the differing field offsets and constants from their headers; usually a declaration fix, not a code change |
| Fuzzy near | 113,911 | 2 | about 230 | Matching work seeded by our source. Their own best attempts on these rows already median 82, so expect perhaps half to land within the time box |

Basis for the identical rate: the 2026-11-06 upstream port of identical twins (five pull
requests, 7,864 B) took about 1.6 lane-hours after a one-time checker setup of about
0.6 hours. That is about 0.2 h/KB. We add a margin for review round-trips (two of those five
pull requests needed fixes) and for smaller functions, which carry a fixed cost of about
10 minutes each.

Claims, pull requests and any comment on their issues are publishing on the user's behalf.
Each needs the user's go-ahead, and none was made for this measurement.
