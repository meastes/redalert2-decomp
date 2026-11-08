# Native tree (arm64 macOS)

The shipped game: recovered C++ from `src/recovered` and `src/shared`, compiled
by modern clang for arm64 macOS, on an SDL3 platform layer. No Wine, Rosetta,
emulation, original code or original bytes. Game data is read from the user's
installation at run time (`--data-dir` or `RA2_DATA_DIR`) and never stored here.
This project is separate from the root `CMakeLists.txt` (the exact original-target
build) and never changes `src/`, the catalog or `tools/original_target`.

```
cmake -S native -B build/native -G Ninja && cmake --build build/native
cmake -S native -B build/native -DRA2_NATIVE_SUBSYSTEM=all   # whole tree (3,279 groups) + ra2-native game link
cmake -S native -B build/native -DRA2_NATIVE_SUBSYSTEM=all -DRA2_NATIVE_LINK_CLOSURE=ON   # link with traps; gen/link_closure.json
ctest --test-dir build/native            # native-core-check
build/native/ra2-native --smoke-test     # SDL3 host window
```

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
   writes `gen/drafts.json`; drafts live in private lane scratch
   (`-DRA2_NATIVE_DRAFTS=ON -DRA2_NATIVE_DRAFT_ROOT=<checkout>`).
2. **Dialect** (`port/include/ra2port/prelude.hpp`, forced include, plus flags
   `-fms-extensions -fms-compatibility -fdelayed-template-parsing -Xclang -fno-wchar
   -fshort-wchar -fwrapv -fno-strict-aliasing -Wno-register -Wno-c++11-narrowing`): calling conventions vanish,
   `wchar_t` is VC6's `unsigned short`, signed overflow wraps.
3. **Recovered code** (`ra2_recovered`): accepted sources; unmatched rows enter as
   their best parked draft (flagged non-exact with its score) or a behaviour-written
   implementation, never empty stubs.
4. **Native data** (`data/`): globals the code reads from the original's .data/.rdata.
   Each comes from adopted source text (TibSun / ra2-decomp, with attribution), a
   behaviour-written generator, or the game files at run time; never copied bytes.
   `bss_globals.cpp` (`tools/data_globals.py`) defines the zero-initialised ones as
   storage (original extent doubled for LP64); initialised ones without a permitted
   source are listed in `unsourced_initialised.tsv`, never defined from the executable.
5. **Platform seams** (`port/src`, `ra2_port`): the imported Win32 API (366 imports:
   KERNEL32, USER32, GDI32, WSOCK32, ole32, binkw32, ...) implemented on POSIX and
   SDL3 under the same C names; DirectDraw/DirectSound as C++ classes with the COM
   interface shapes the recovered code calls, backed by SDL3 textures and audio streams.
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
