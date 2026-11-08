# Native tree (arm64 macOS)

The shipped game: recovered C++ from `src/recovered` and `src/shared`, compiled
by modern clang for arm64 macOS, on an SDL3 platform layer. No Wine, Rosetta,
emulation, original code or original bytes. Game data is read from the user's
installation at run time (`--data-dir` or `RA2_DATA_DIR`) and never stored here.
This project is separate from the root `CMakeLists.txt` (the exact original-target
build) and never changes `src/`, the catalog or `tools/original_target`.

```
cmake -S native -B build/native -G Ninja && cmake --build build/native
cmake -S native -B build/native -DRA2_NATIVE_SUBSYSTEM=all   # whole tree (3,279 groups) + drafts + ra2-native game link
cmake -S native -B build/native -DRA2_NATIVE_SUBSYSTEM=all -DRA2_NATIVE_LINK_CLOSURE=ON   # link with traps; gen/link_closure.json
ctest --test-dir build/native            # native-core-check, native-asm-check
build/native/ra2-native --smoke-test     # SDL3 host window
build/native/ra2-native --data-dir <installation> --run-to=menu   # start-up test run (below)
```

Full-tree smoke test: all 3,610 original static initialisers run (in `_initterm`
order), main is reached and prints `initterm: main reached ...`, and exit-time
destructors complete. Diagnostics: `RA2_NATIVE_INIT_TRACE=1` prints each unit's
first original initterm index as it starts, `RA2_NATIVE_CRASH_TRACE=1` prints the
last index and a backtrace on a fault, `RA2_NATIVE_TRAP_CONTINUE=1` lists every
link-closure trap on the path instead of stopping at the first.

## Start-up test runs

`ra2-native --run-to=<milestone> [--run-seconds=N] [--write-dir=D] --data-dir <installation>`
starts the game (WinMain, `native/drafts.json` 0x68E280) and logs each start-up
milestone to stderr as the port sees it: `main`, `winmain`, `ini` (RA2.INI opened),
`mix` (first MIX), `strings` (ra2.csf as a loose file; from a MIX it is implied by
`window`), `window`, `ddraw`, `primary` (first DirectDraw primary surface),
`surfaces` (off-screen surfaces after it), `dialog` (first dialog requested: the
templates are the open question), `present` (first frame presented), `menu` (not
detected yet: the main menu is a dialog, blocked with the templates; use it as the
run-as-far-as-possible target). The
run ends by itself at the requested milestone (exit 0), the first link-closure
trap (3), a fault (4, with a backtrace), WinMain returning, or the time limit
(default 5 s; exit 2, with the main thread's backtrace), and prints a summary:
milestones with times, files opened/written, windows, surfaces, dialogs, message
boxes, unported imports and traps. The installation is never written: every
create/write/delete goes to `--write-dir` (default a fresh `$TMPDIR/ra2-native-run-*`)
and reads look there first. Message boxes print to stderr; no modal UI is shown.
The host plays the launcher's part (the launch mutex and event RA2.EXE created).

## Layers

1. **Native view** (`tools/native_units.py`, run at configure time). Reads the
   catalog (read-only, via `tools/original_target/catalog_io.py`) and writes,
   under the build directory, one translation unit per catalog compile group in
   source order (the exact build concatenates a group the same way, and later
   files rely on earlier ones), plus a port-rewritten copy of every `src/shared`
   header. Rewrites are mechanical and keep VC6 x86 meaning on LP64:
   `long` to a 32-bit `RA2_LONG`, `L` literal suffixes dropped, `operator new(unsigned int`
   to `size_t`, 32-bit layout size checks disabled, VC6 for-init scope hoisted,
   VC6 wide CRT calls renamed `ra2_*`, opaque enums given VC6's fixed `int`
   (`enum X;` and its definition become `enum X : int`), reference casts of
   rvalues wrapped in `ra2port::as_lvalue` (VC6 reinterprets a temporary).
   Rows with `__asm` are skipped and listed. `port/fixups.json` holds the
   per-file substitutions clang needs (sources and `src/shared` headers, each
   with the VC6 meaning it keeps and its evidence) plus qualified-name
   `renames`; each should move into `src/` once the exact build proves the
   edit byte-neutral. `--win64-params` (on: `RA2_NATIVE_WIN64_PARAMS`) makes WPARAM/LPARAM/LRESULT/*_PTR
   pointer-sized and respells the (HWND, unsigned, unsigned, long) procedure shape.
   Calls whose libc namesake differs (Winsock, rand, fopen, time, printf/scanf
   family, wide CRT) are renamed `ra2_*` and supplied by the port.
   **Static initialisers**: `initterm.json` (from `tools/initterm_manifest.py`,
   addresses and names only) gives the original `_initterm` order; units are
   emitted in that order and linked as objects, so `__mod_init_func` runs as the
   original did; explicit init functions are called from generated
   `units/initterm_NNNN.cpp` at their positions (seed/scaffold ones skipped, listed
   in `gen/report.json`).
   `--drafts` appends the non-exact launch-path drafts listed in `drafts.json`
   (entry, score, draft path, host group; only those marked `compiles`) and
   writes `gen/drafts.json`; a draft with `host_source: null` is its own unit,
   linked after the initialiser units (WinMain: no accepted Startup.CPP row hosts
   it); an `adapted` draft is a native adaptation of the scored draft (its private
   headers inlined, non-portable statements marked NATIVE). Each included draft is
   tracked as `native/drafts/<entry>.cpp` (header: entry, score, origin lane or
   worker, NON-EXACT DRAFT; `origin`/`author` in drafts.json are provenance only),
   so the build reads no ignored path. Recovered source text by our lanes, never
   original bytes; raw Ghidra seeds are not tracked. On by default with
   `RA2_NATIVE_SUBSYSTEM=all` (`-DRA2_NATIVE_DRAFTS=OFF` builds without them).
   Refresh a draft by copying the lane's newer text over the file, keeping the header.
2. **Dialect** (`port/include/ra2port/prelude.hpp`, forced include, plus flags
   `-fms-extensions -fms-compatibility -fdelayed-template-parsing -Xclang -fno-wchar
   -fshort-wchar -fwrapv -fno-strict-aliasing -fwritable-strings -Wno-register -Wno-c++11-narrowing`):
   calling conventions vanish, `wchar_t` is VC6's `unsigned short`, signed overflow
   wraps, string literals are writable (below).

   **Writable string literals.** VC6 put string literals in .data, one copy per
   occurrence (the exact build has no /GF), and recovered code writes through
   pointers that callers pass literals to. Clang's `-fwritable-strings` (Apple clang
   21 accepts it) gives every narrow and wide literal its own `__DATA,__data` object,
   the same meaning; native-core-check proves it (an in-place upper-casing of a
   literal and a store into `L""`). A static scan (Scout1061, 2026-11-08: functions
   that write through a char/wide pointer parameter, directly, through an alias,
   through a CRT mutator or transitively, joined with the call sites that pass
   literals there) found 71 literal sites, all one chain: `MixFileClass::Offset`
   0x595AA0 upper-cases its name in place (`_strupr`), reached by
   `MixFileClass::Retrieve` 0x5956C0 (53 sites: palettes, shapes, sounds in
   Init_Game, the One_Time functions, the sidebar), `Load_Cached_Or_Allocated_File`
   0x495270 (12: MOUSE.SHP, sidebar shapes), `ProgressScreenClass::Load_Shape`
   0x61EF40 (4), plus `_strupr("TUR")` twice in
   `BuildingTypeClass::Load_Turret_Voxels` 0x459640. CCFileClass names are copied
   (`Set_Name`) before `Offset` sees them. Literals stored in pointers and written
   later, and name tables in native data, are covered by the same flag (it applies
   to every unit with the dialect, `native/data` and `native/link` included); the
   platform layer (`ra2_port`) keeps read-only literals.
3. **Recovered code** (`ra2_recovered`): accepted sources; unmatched rows enter as
   their best parked draft (flagged non-exact with its score) or a behaviour-written
   implementation, never empty stubs.
   **Link adapters** (`link/`, compiled with the dialect, never original code):
   `address_wrappers.cpp` and `data/address_aliases.tsv` (`tools/address_aliases.py`)
   bind a name to the recovered body at the same original address (catalog
   references and region entries) when the native ABI signatures (LLVM IR of the
   units) agree, or through a converting wrapper when only a float/double return
   differs (x87 st0), or through a converting adapter (`signature_adapters.cpp`)
   when the declarations differ only in integral widths (VC6 passed a 32-bit slot;
   the callee read its own width), a Win32 handle carried in an int (port handles are
   small tokens), pointer-sized message parameters, a `this` the free definition
   never reads, or a result the caller ignores; the remaining mismatches (int for
   pointer, parameter count, struct by value) are listed with a reason in
   `gen/address_aliases.json`: one side's declaration is wrong and src/ settles it.
   The generators are idempotent (each recomputes its queue from the build's link
   inputs without its own outputs, or keeps what it wrote before); after a landing
   re-run `data_globals.py --build <dir>`, rebuild, `address_aliases.py --build <dir>`,
   rebuild, `template_instances.py --build <dir>`, rebuild.
   `template_instances*.cpp` (`tools/template_instances.py`)
   explicitly instantiate the recovered VectorClass/DynamicVectorClass bodies for
   element types whose owner unit is missing; `initterm_rewrites.cpp` rewrites the
   seed/scaffold explicit initialisers (`Initterm_<entry>`, called at their
   positions); `behaviour_*.cpp` hold behaviour-written bodies of unmatched rows on
   the start-up/exit path, each citing its evidence (`behaviour_winmain.cpp`,
   `behaviour_surface.cpp`, `behaviour_map.cpp`: the WinMain path). Fixups run
   before the inline-assembly check, so a fixup can give an evidenced `__asm` block
   its native meaning (Main_Game's exception-return capture).
   **Handwritten assembly** (`behaviour_asm_vqa.cpp`, `behaviour_asm_draw.cpp`,
   `behaviour_asm_lib.cpp`; listed in `behaviour_rewrites.json`): portable C++
   for the functions the original implements in assembly (the archive procedures
   in `src/recovered/asm`: AudioUnzap, VQA_LCW_Uncompress, the VQA SOS ADPCM
   decoder, the four ASM_UnVQ drawers, Brighten_Color/MMX_Brighten_Color in four
   pixel formats, the three Asm_Interpolate copies, Get_CPU_Clock) and for the
   accepted rows whose body is evidenced `__asm` (LCW_Comp, BlitTransXlat<unsigned
   short>::BlitForward, RDTSC, Get_CPU_Speed). Each is written from the adopted
   archive text, not exact; the TSC is a virtual 1 GHz counter (Get_CPU_Speed
   returns 1000). `native-asm-check` (tests/asm_check.cpp) runs each on synthetic
   inputs against references built from the algorithm (encoders, per-pixel
   formulas, the textbook IMA ADPCM decoder) and compares the ADPCM tables with
   the adopted `.inc` text. Aliases are applied with ld64
   `-alias_list` (link_closure.py filters them to the current link).
4. **Native data** (`data/`): globals the code reads from the original's .data/.rdata.
   Each comes from adopted source text (TibSun / ra2-decomp, with attribution), a
   behaviour-written generator, or the game files at run time; never copied bytes.
   `bss_globals.cpp` (`tools/data_globals.py`) defines the zero-initialised ones as
   storage (original extent doubled for LP64); initialised ones without a permitted
   source are listed in `unsourced_initialised.tsv`, never defined from the executable.
   One storage per original address: other names of a BSS address are assembler
   aliases (`.set`), names whose address another input defines go to `bss_aliases.tsv`.
   WinMain path data: `com_guids.cpp` (COM class/interface ids: TibSun MIDL output
   and public Microsoft ids), `startup_winmain.cpp` (key text, launcher names,
   flags; TibSun / ra2-decomp text), `base64_tables.cpp` (TibSun base64.cpp),
   `crc32_table.cpp` (CRC-32 table generated at compile time); each compared
   privately with the pinned target, nothing copied. Data files are not
   port-rewritten: spell VC6 `long` as `int` there.
   **Generated data**: `tools/fast_math_tables.py` computes the fast-math tables
   (sin/cos 10,241, asin 4,097, tan 4,097, atan 4,097 + step, sqrt mantissas 16,384)
   and their scale constants at configure time: TibSun fastmath.cpp's init loops run
   as on the chopping x87 (FSIN/FPTAN reduced by the FPU's 66-bit pi and rounded to
   64 bits, every float store truncated, arctangent limit tan(M_FPI/2 - 0.01)).
   Checked privately against the pinned target: all 38,927 words identical.
5. **Platform seams** (`port/src`, `ra2_port`): the imported Win32 API (366 imports:
   KERNEL32, USER32, GDI32, WSOCK32, ole32, binkw32, ...) implemented on POSIX and
   SDL3 under the same C names; DirectDraw/DirectSound as C++ classes with the COM
   interface shapes the recovered code calls, backed by SDL3 textures and audio streams.
   Blowfish.dll's COM server (IBlockCipher, MIX header decryption) is native
   (`port/src/blowfish.cpp`, TibSun blowfish.cpp); comctl32's DllGetVersion
   reports 5.81; FindWindowA, OpenMutexA, RegisterActiveObject,
   MessageBoxIndirectA and `_controlfp` are ported; showing the first top-level
   window activates it (WM_ACTIVATEAPP, WM_ACTIVATE) as Windows does.
   Unported imports are logged no-ops returning their Win32 failure value
   (`port/src/win32_unimplemented.cpp` and the `ra2port::unimplemented` calls).
6. **Host** (`platform/sdl`, `ra2-native`): process, window, event pump
   (`host.cpp`: SDL input to Win32 messages, primary-surface presentation); with
   `-DRA2_NATIVE_SUBSYSTEM=all` it links every recovered object and calls `WinMain`
   (`--smoke-test` only opens the window). Repeated definitions across units get
   VC6 select-any semantics at link (`tools/weaken_duplicates.py`). Until the tree
   is closed, `RA2_NATIVE_LINK_CLOSURE` adds traps/placeholders for what is still
   unrecovered (`tools/link_closure.py`, never shipped).

`historical/native-reconstruction/` stays preserved as reference (SDL3 choice,
MIX/SHP readers usable as test oracles); its own simulation is not reused.
