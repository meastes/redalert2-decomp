# Native tree (arm64 macOS)

The shipped game: recovered C++ from `src/recovered` and `src/shared`, compiled
by modern clang for arm64 macOS, on an SDL3 platform layer. No Wine, Rosetta,
emulation, original code or original bytes. Game data is read from the user's
installation at run time (`--data-dir` or `RA2_DATA_DIR`) and never stored here.
This project is separate from the root `CMakeLists.txt` (the exact original-target
build) and never changes `src/`, the catalog or `tools/original_target`.

```
cmake -S native -B build/native -G Ninja && cmake --build build/native
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
   VC6 wide CRT calls renamed `ra2_*`. Rows with `__asm` are skipped and listed.
   `port/fixups.json` holds the few per-file substitutions clang needs (each
   should move into `src/` once the exact build proves the edit byte-neutral).
2. **Dialect** (`port/include/ra2port/prelude.hpp`, forced include, plus flags
   `-fms-extensions -fms-compatibility -fdelayed-template-parsing -Xclang -fno-wchar
   -fshort-wchar -fwrapv -fno-strict-aliasing`): calling conventions vanish,
   `wchar_t` is VC6's `unsigned short`, signed overflow wraps.
3. **Recovered code** (`ra2_recovered`): accepted sources; unmatched rows enter as
   their best parked draft (flagged non-exact with its score) or a behaviour-written
   implementation, never empty stubs.
4. **Native data** (`data/`): globals the code reads from the original's .data/.rdata.
   Each comes from adopted source text (TibSun / ra2-decomp, with attribution), a
   behaviour-written generator, or the game files at run time; never copied bytes.
5. **Platform seams** (`port/src`, `ra2_port`): the imported Win32 API (366 imports:
   KERNEL32, USER32, GDI32, WSOCK32, ole32, binkw32, ...) implemented on POSIX and
   SDL3 under the same C names; DirectDraw/DirectSound as C++ classes with the COM
   interface shapes the recovered code calls, backed by SDL3 textures and audio streams.
6. **Host** (`platform/sdl`, `ra2-native`): process, window, event pump; it will
   call the recovered `WinMain` once the tree links.

`historical/native-reconstruction/` stays preserved as reference (SDL3 choice,
MIX/SHP readers usable as test oracles); its own simulation is not reused.
