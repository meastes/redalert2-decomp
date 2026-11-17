# Route A: end-of-route report (owner, 2026-11-12)

This report covers Route A, the user's decision of 2026-11-12 ~04:00Z: re-file the tree into
one source per original translation unit, with each unit keeping its own header state. The
work ran from 04:00Z to about 18:00Z on account 2.

The numbers below were read from docs/STATUS.md at main `c7577f385`. Residual detail is in
docs/ROUTE_A_RESIDUALS.md, and per-landing records are in docs/HANDOFF.md.

This report contains no code bytes.

## 1. Totals

| Measure | Value |
|---|---|
| Module form | 404 modules (375 Route A units plus the 29 earlier modules); 12,423 functions, 2,968,994 code bytes, about 90.7% of accepted code |
| Route A units | 375 units, 12,170 functions, 2,943,890 code bytes |
| Units with accepted code not in module form | 31 units, 30,538 B (section 6) |
| Whole-file build | exact (SHA e6418111…) at every landing; Reproduced .text unchanged at 93.46% |
| Per-unit switches kept | 686 instances over about 280 distinct switches; most are kept by only one unit |
| Stand-in references from other units | 3,344, listed per unit in the module maps (the later declaration-cleanup list) |
| Landings | about 50 owner-gated batches. Each gate ran the unit tests, the whole-file exact build, module_check and the native configure, build and ctest 5/5 |
| Played checks | 1, 4, 5 and the final check 6 passed; 2 found the pre-existing missile crash (fixed) |

## 2. Residual rows (rows that stay in their own file)

**Split rows** (held by another frozen population): 3,879 rows, 276,158 B. This is the known
ceiling for module form under the current build model. By cause (STATUS, RouteA5
classifier):

| Cause | Rows | Bytes |
|---|---:|---:|
| Template members (`VECTOR_IMPLEMENT_*` and similar switches, or explicit instantiations) | 1,504 | 102,560 |
| Stand-ins (placeholder form) | 812 | 51,201 |
| Needs another unit's header state (`ghidra_types.hpp` about 428 rows, other includes and switches) | 745 | 46,007 |
| The unit's own second population (a join would empty it) | about 271 | about 50,000 |
| Emitted by another source (inline ctors, dynamic initialisers) | about 327 | about 12,500 |
| Held by a population that compiles each source alone | about 131 | about 7,500 |
| Mixed population with no bound / other option set | about 45 | about 2,600 |

What moves split rows: almost every one (3,651 of 3,715 at the RouteA5 snapshot) was refused
because the unit population's frozen inventory pass has no equal bound for the row. No text or
header change alone moves more than about 34 rows. The rest need either:

- the parked join model (draft/join-trial-ii, reviewed and parked), or
- one shared header state (option b).

**Flipped rows** (not exact when moved): 313 rows, 24,207 B.

| Cause | Rows | Bytes |
|---|---:|---:|
| Header-switch conflict: per-file `VECTOR_IMPLEMENT_*`, all in `globals` | 302 | 23,887 |
| `.CRT$XCU` static-initialiser section in a join (voxdrsys, houstype) | 8 | about 220 |
| Compile errors in joins (ahandle2) | 2 | small |
| Differs under the unit's state (_map 0x40D0F0) | 1 | 16 |
| Declaration-count windows | 0 | 0 |

The join sweep moved 2 rows (190 B). 7 rows could not be retried, because their pour records
were deleted with a finished lane's worktree; only a re-pour of ahandle2 or voxdrsys moves
them.

**Kept by rule:** 6 rows, 3,200 B:

- evidenced inline assembly in conquer and xsurface;
- house's Begin_Production, kept for the native vtable-views heuristic;
- temporal's Release_Target_State, whose native fixup can't be placed inside the unit file;
- others recorded in the maps.

## 3. Cost against the 50–210 lane-hour estimate

| Lane | Model | Work | Lane-hours (wall) |
|---|---|---|---:|
| RouteA1 | Fable | tooling (refile_unit, joins, header-static fix, join command) plus 207 units | about 9.6 |
| RouteA2 | Fable | 145 units plus tool fixes (screens, path scrub, Windows config/LF, auto-keep) | about 13.4 |
| RouteA3 | Opus | 29 units | about 2.0 |
| RouteA4 | Opus | 49 units, vqalib_drawer, join sweep | about 2.8 |
| RouteA5 | Opus | residual classification | about 0.2 |
| JoinTrial1 | Fable | option (ii) trial (parked) | about 0.3 |
| Played checks 1–6, crash classifier | Fable | | about 2.5 |
| Codex (Windows) | Codex | 10 units | about 1.7 worker-hours (plus about 1.9 h on old-tool failures) |
| **Total, Mac lanes** | | | **about 31 lane-hours** |

- The estimate was 50–210 lane-hours; measured is about 31. That is below the low end, because
  large units poured in one include block at the first attempt and gates were batched.
- The rate was about 0.002 h/KB for large units and about 0.1 h/KB for tiny ones.
- Usage, account 2:
  - before the reset: Weekly · Fable 41% → 96% (about 55 points), weekly all models 35% → about 65%;
  - after the reset (Opus lanes): all models 16% → 19%, about 3 points.
- Owner-alone rate: not separately measured. The owner ran continuously alongside the lanes.

## 4. Codex trial (Windows workers)

| Worker | Unit | Rows | Bytes | Exact on Windows | Worker min | Owner replay |
|---|---|---:|---:|---|---:|---|
| codex4 | warhead | 26 | 4,233 | yes | 9.5 | five patches in about 5 min, mechanical modules.json/initterm resolution |
| codex4 | overtype | 23 | 3,272 | yes | 8.8 | |
| codex4 | gauge | 18 | 3,011 | yes | 8.9 | |
| codex4 | quat | 16 | 3,184 | yes | 8.4 | |
| codex4 | walk | 49 | 3,524 | yes | 11.5 | |
| codex1 | weapon | 30 | 4,648 | yes | 9.5 | five patches in 5 s, clean |
| codex1 | smudtype | 32 | 3,880 | yes | 9.8 | |
| codex1 | power | 30 | 3,808 | yes | 9.1 | |
| codex1 | bullettype | 32 | 3,691 | yes | 8.9 | |
| codex1 | terrtype | 19 | 3,274 | yes | 8.6 | |

All 10 units landed after the Mac full gate including native, and all were exact the first
time after the tool fix. Problems found on Windows, all fixed or worked around:

- refile_unit ignored RA2_CONFIG and wrote CRLF (fixed in 7c93c6d8b);
- the pinned VC98 include folder was missing from the mirror (copied over; sync_windows.sh
  should carry it);
- the `.win` config names the RTM C2 build (8168) where the Mac uses SP3 (8447). The Mac replay
  decides acceptance, so this does not affect landed work.

## 5. Inputs for option (b), one shared header state

- Measured in the seeded-tree test: baking one unit's switches (building's 21) across the tree
  flipped 7.1% of accepted bytes, and 11 units stopped compiling.
- Switches behind the most residual bytes are the `VECTOR_IMPLEMENT_*` family, set per file by
  template members and kept by no unit. Together they hold about 450 split rows and 190 flipped
  rows, about 60 KB:

  | Switch | Split rows / bytes | Flipped rows / bytes |
  |---|---:|---:|
  | `VECTOR_IMPLEMENT_VECTOR_RESIZE` | 107 / 20.0 KB | 42 / 7.3 KB |
  | `VECTOR_IMPLEMENT_DYNAMIC_RESIZE` | 102 / 19.5 KB | 41 / 7.2 KB |
  | `VECTOR_IMPLEMENT_VECTOR_EQUAL` | 107 / 7.0 KB | 42 / 2.6 KB |
  | `VECTOR_IMPLEMENT_VECTOR_ID_OBJECT` | 105 / 5.8 KB | 41 / 2.1 KB |
  | `VECTOR_IMPLEMENT_VECTOR_FETCH` | 121 / 2.1 KB | 26 / 0.3 KB |

  In a real codebase these bodies live in the template's header and are instantiated by the
  units that use them. That is the natural first option-(b) experiment: one container header
  state.
- Most-kept unit switches: `VECTOR_IMPLEMENT_ALL` 52 units, `VECTOR_ACCESSORS` 42,
  `DYNAMIC_ACCESSORS` 40, `DYNAMIC_ADD` 31, `SHAPE_FILE_EMPTY_RECT` 21, then the Coord family
  (`RA2_COORD_FROM_CELL_INLINE` 13, `RA2_COORD_AS_CELL` 12, `RA2_COORD_TPOINT3D` 11).
- `shared/ghidra_types.hpp` is the largest single header-state blocker: about 428 split rows,
  plus the ahandle2 compile errors. Replacing its typedefs with the Windows headers' own types,
  where the bodies allow, is exact-preserving cleanup.
- Any option-(b) step that keeps every byte exact can be ordered by the review session. Any
  step that relaxes exact-means-exact is the user's.

## 6. Not in module form

31 units, 30,538 B:

- **No convertible compile group:** txtlabel2 (14.7 KB), msgloop, _astar2, _script, egos,
  _astar, capturemanager, lcw.
- **One-to-three-row units, not attempted:** sendfile, _alpha, obscure, lcwuncmp, audlevel,
  findpath, dllver and others, each in another population's group or a separate-unit
  population.
- **The two vqalib/drawer rows (5 B).**

Converting these would need separate-unit populations handled as one-row units. That is small
work (about 30 KB), and is listed for the next decision.

## 7. Native and played checks

- **Route A native regression, found and fixed:** header-static copies 17 → 46 (ddf0c8bd3),
  with a configure guard.
- **Native fixes outside Route A, landed with the user's go-ahead (15bdc88ae):**
  - nuke-silo index (the crash);
  - three CaptureManagerClass mind-control reads.
  - Two unproven raw offsets are listed in build/routeA/nativefix2_raw_offsets.md.
- **Played checks:**
  - 1 passed.
  - 2 found the missile crash, which turned out to predate Route A.
  - 4 and 5 passed.
  - Final check (6), on c7577f385: all items passed, including the NAMISL nuke scenario (24 missile-mission entries, valid index, no fault); all 14 pre-A saves have now been played across the checks. Logs build/routeA/playcheck6/.

## 8. Open for the next decision (review session's call; the user may override)

1. Option (b), exact-preserving steps first: a container header state for `VECTOR_IMPLEMENT_*`,
   and `ghidra_types.hpp` replacement.
2. The parked join model (draft/join-trial-ii): revisit together with (b).
3. The 31 unconverted units (about 30 KB).
4. Re-pour of ahandle2 and voxdrsys, so their 7 collateral rows can be retried.
5. Native/iOS lane (paused queue), exact-residue lanes, and the Codex workers' next job. Codex
   converts reliably at about 9 minutes per unit; with Route A done, a new kind of job is
   needed.
