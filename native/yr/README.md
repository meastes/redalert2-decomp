# yr-native: the native Yuri's Revenge game (lane Y1)

User decision 2026-11-15 (AGENTS.md); plan 1 of `docs/YR_PORT_SIZING.md`. The game is built from
the pinned ra2-decomp tree in `yr/` (never edited) with native/'s platform layer. RA2's `src/`,
its exact build and the `ra2-native` targets are unchanged; `yr-native` is built only with
`-DRA2_NATIVE_YR=ON` (Mac).

    cmake -DRA2_NATIVE_YR=ON -DRA2_NATIVE_YR_MINGW=<llvm-mingw>/generic-w64-mingw32/include <native build dir>
    cmake --build <native build dir> --target yr-native

## Layout

| Path | What |
| --- | --- |
| `yr/ra2-decomp`, `yr/tibsun` | pinned copies (commits, licences: `yr/README.md`, `THIRD_PARTY.md`) |
| `native/yr/tools/yr_units.py` | one native unit per `src/**/*.cpp` and the merged header directory (TibSun headers, ra2-decomp's `include/` over them, as their build), with `fixups.json` and the RA2 tree's port rewrites (`native_units.port`: long, literal suffixes, ra2_ CRT names, window data, ...) |
| `native/yr/tools/yr_sdk.py` | the Win32/DirectX headers: the local llvm-mingw toolchain's mingw-w64 headers rewritten for the VC6 widths (long -> 32 bits) into the build directory, with the few clashes with the host's C library resolved (BSD socket types renamed, real `__uuidof`, `DLGPROC` returning BOOL, Win32 window-data names kept); never copied into the repository |
| `native/yr/sdk` | the YR prelude (host C types first), the VC6 CRT declarations as the port supplies them, and `comdef.h`/`io.h`/`mbstring.h` wrappers |
| `native/yr/fixups.json` | text fixups where clang refuses VC6 dialect, cut functions (assembly) and left-out units (Westwood Online), each with its reason |
| `native/yr/glue` | native code for what is cut or not matched: the RA2 native tree's C for the TibSun assembly (LCW_Comp, the time-stamp counter), the VC6 CRT names, the host's RA2 hooks, and YR functions written from RA2 twins (`yr_twins.cpp`) |
| `native/yr/yr.cmake` | the `yr_units_objs`, `yr_host_objs` and `yr-native` targets and the link closure |

## Stage 1 result (2026-11-15)

- All 435 YR units compile for arm64 at -O2 (3 Westwood Online units left out: Internet play).
  33 text fixups, 4 cut functions with native glue.
- `yr-native` links (11 MB). Link closure: **2,534** entries (functions 811: 611 C++, 197 C,
  3 Win32 [VariantInit/VariantClear/VariantChangeType]; data 1,723: 1,294 address-named
  `DAT_`/`PTR_` globals, 284 C++ and 139 C globals, 6 vtables). All but 10 are referenced
  by the YR units (9 by the shared host, 1 by the port: RA2 hooks, supplied by
  `glue/yr_host_glue.cpp`). Of the 811 functions, 269 carry ra2-decomp's address names
  (`FUN_xxxxxxxx`, their unmatched and library code); WinMain itself (0x006BB9A0, 10,261
  bytes) is unmatched there and has an RA2 twin (RA2 0x0068E280).
- Run: the process starts and faults in the static initialisers, before main: MapClass's
  constructor chain (TabClass) compares a sidebar name from a `DAT_` global that is still
  zero. The YR globals have no values yet: the original keeps them in gamemd.exe's data.

## The Win32 headers (ruling 2026-11-15)

The Mac YR build needs the mingw-w64 Win32/DirectX headers of the local llvm-mingw toolchain
(`-DRA2_NATIVE_YR_MINGW`, `private/toolchains/llvm-mingw-*`), the same toolchain the Windows
native build uses. `tools/yr_sdk.py` rewrites them into the build directory at configure time;
nothing from them is copied into the repository.

## Stage 2 (lane Y2, 2026-11-15): from the data load into Init_Game

* gamemd.exe 1.001 is recognised by its code sections (the user's copy with its 4 KB overlay;
  the 2-byte no-CD patch at 0x0052F949 is undone for the .text check); `data/globals.tsv`
  (from ra2-decomp's pins, `tools/yr_pins.py`) and the address names give each global its
  address. `tools/yr_data.py` makes storage for every global no unit defines and the table the
  port fills from gamemd.exe (546 globals) before the YR static initialisers, which
  `tools/defer_initializers.py` holds back until then. Class-typed globals no unit defines
  (the INI objects, the random generator, BlubCell) are constructed in `glue/yr_globals.cpp`.
* Address names bound to the definitions at those addresses (`tools/yr_aliases.py`), and the
  few functions the tree defines under one name and calls under another
  (`data/name_aliases.tsv`, `glue/uicmnds2_native.inc`, `glue/ccini_native.inc`).
* Window procedures take full-width message parameters (ruling: WPARAM/LPARAM as the RA2
  tree's host passes them). teleport.cpp's bridge test is `true`: gamemd.exe's 0x00718260 makes
  no call to 0x00483610.
* Native drafts of unmatched functions, appended to their units (`fixups.json` "append"; each
  file is labelled NON-EXACT): WinMain (`glue/startup_winmain.inc`), Allocate_Surfaces,
  ConvertClass::CreateFromFile and FUN_00534E50 (`glue/init_native.inc`), Init_Game
  (`glue/init_game_native.inc`); the import thunks the tree calls by address
  (`glue/yr_import_thunks.cpp`); the CPU probes (`glue/yr_asm.cpp`).
* Fixed on the way: Darwin defines BIG_ENDIAN, so base64.cpp's packet union took the
  big-endian layout and the encryption key decoded to nothing (LANGUAGE.MIX then failed
  intermittently); GUID globals (CLSIDs, IIDs) are now loaded.

Run: `yr-native --data-dir <RA2+YR install> --run-to=menu` creates the "Yuri's Revenge" window
(800x600, 16 bpp), its surfaces and mouse, loads the bootstrap and language mixfiles and the
palettes, and stops in Init_Game at FUN_00754CB0 (3,290 bytes, TS Init_Voxel_Matrices; RA2
twin `src/recovered/voxel.cpp` Unknown_00718060).

## Next (to the main menu)

1. FUN_00754CB0 from its RA2 twin; then Init_Game's remaining callees as they trap
   (Init_Secondary_Mixfiles, Init_Bulk_Data and FUN_0052CD70 are matched upstream), Main_Loop
   (unmatched in YR; RA2 twin), Game::PlayMovie / FUN_005BF260 (movies), Read_PCX_File.
2. Init_Game's CD search when no search path is set (not drafted: the native host always
   passes -CD.).
3. Variant* and the other Win32 imports the port does not yet supply (the run lists them).
