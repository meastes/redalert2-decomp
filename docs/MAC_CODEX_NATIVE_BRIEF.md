# Mac Codex native lane: brief (prepared 2026-11-11, not started)

Prepared by the owner on the user's decision of 2026-11-11 ~04:00Z (HANDOFF). The lane starts only
when the user says so; until then this file is the brief, nothing runs.

## Goal

Bug fixing and audits in the Mac native port (`native/`, see `native/README.md`): crashes, traps,
wrong behaviour against the original, 64-bit and layout problems, and applying the behaviour patches
the Windows draft pilot delivers (`native/drafts.json` behaviour_verdict fields). Native work earns no
exact-match credit and never changes the exact build.

## Area

The lane works only on parts of `native/` the iOS lane (Scout1247: `native/platform/sdl/` host layer,
`native/packaging/`, the port_core paths in `native/port/src/port_core.cpp`, `os_apple.cpp`,
`storage.cpp`, and the iOS build/CMake parts) does not touch:

- `native/link/` (behaviour adapters, harness scaffolding, link wrappers),
- `native/drafts/` and the draft fields of `native/drafts.json` (draft text, behaviour patches),
- `native/behaviour_rewrites.json`, `native/port/fixups.json`, `native/port/view_layouts.json`,
- `native/tools/` and `native/tests/` (census, audits),
- the Win32 shims in `native/port/src/` other than the iOS lane's files (user32, gdi_winmm_advapi,
  ddraw_dsound, kernel32, wsock32, ole, bink, crt, ...) when the unit brief names them.

Each unit brief from the owner names its area. Anything outside it (the iOS lane's files, the
top-level `native/CMakeLists.txt` beyond a one-line source list entry, `src/`, `tools/original_target/`,
manifests, the catalog) is off limits; if a fix needs it, stop and hand back the finding.

## Rules

- Work in a git worktree of main (`git worktree add build/class-work/<lane>/wt main`), never in the
  main checkout. Never edit `src/` (the exact tree); recovered code is changed natively only through
  `native/` fixups, rewrites or adapters, each marked NATIVE.
- One landing at a time: hand back one unit, wait for the owner to land it, rebase, then the next.
  The owner checks no landing is running (`pgrep -f land.sh`) before applying.
- No game bytes, assets, game.exe data or disassembly in tracked files; game data is read from the
  user's installation at run time only (AGENTS.md native phase).
- Parallel game instances stall the native-census gate on this Mac: run one game at a time, and none
  while another lane's census or UI crawler runs.

## Gates (all, in order, before every hand-back)

1. `cmake --build build/<lane>-native` (configured `-DRA2_NATIVE_SUBSYSTEM=all
   -DRA2_NATIVE_LINK_CLOSURE=ON`, census data dir and game.exe set) builds with no new warnings
   in the touched files.
2. `ctest --test-dir build/<lane>-native` passes 4/4: native-core-check, native-asm-check,
   native-view-audit, native-census (600 skirmish frames, no trap or fault).
3. `ra2-native --smoke-test` opens and closes the host window.
4. `native/packaging/make_app.sh build/<lane>-native/ra2-native build/<lane>-native` rebuilds
   RA2.app without errors.
5. A windowed skirmish game in that RA2.app: start from the main menu, play at least two minutes
   (build, harvest, fight), quit to the menu; no error window, no crash report
   (`~/Library/Logs/DiagnosticReports/ra2-native-*.ips`).

## Hand-back

- One patch per unit against the main commit it was built on:
  `build/class-work/<lane>/d/rNN/<unit>.patch`, plus a short note: what was wrong (input or state
  that shows it), what changed, files touched, gate results 1-5 with the census log line, the base
  commit, and "Calibration used: no" (native work never calibrates).
- For a pilot behaviour patch: the draft entry, the Windows delivery it came from, and the
  drafts.json fields to set (behaviour_verdict, behaviour_checked_by, behaviour_checked_date,
  behaviour_checked_head).
- Status in the AGENTS.md status format. Never wait on the owner: when a unit is blocked, record it
  and take the next one from the unit list.

## First units (proposal, owner picks at start)

1. Apply the Windows pilot's "differs" patches to `native/drafts/<entry>.cpp` (keeping NATIVE lines),
   one draft per landing, census + skirmish gate each.
2. Audit `native/link/` adapters for pointer-width and layout assumptions not covered by the
   64-bit audit (Scout1242/Scout1246 patch).
3. Census traps and crash reports from user games, newest first.
