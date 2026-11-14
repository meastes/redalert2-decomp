# Red Alert 2 decompilation and native port

This project recovers C/C++ source for the base Red Alert 2 executable
(`game.exe` 1.006, not Yuri's Revenge) that recompiles with the original
compiler (Visual C++ 6 SP3) into the exact original file, byte for byte. The same
source also builds a native game for macOS (arm64), iOS and Windows.

## Where it stands

Numbers are generated into [docs/STATUS.md](docs/STATUS.md); the headline ones:

| Measure | Value |
| --- | ---: |
| Reproduced .text (built from real inputs, not copied from the original) | 93.48% |
| In-scope C/C++ bytes matched exactly | 93.08% |
| In-scope functions matched exactly | 17,889 of 17,985 (99.47%) |
| In-scope functions with source (exact or draft) | 17,982 of 17,985 |

The whole-file build reproduces the original executable exactly (4,485,392
bytes, SHA256 `e6418111c2c36081a1795fb000e0927beea1630170b334c5013d12102602e70f`).
What it still takes from the original rather than from source is counted in
STATUS.

## What is in the repo

- **Exact source** (`src/recovered/`, `src/shared/`): every accepted function
  compiles to the original bytes, checked by a strict audit and the whole-file
  build. Most units are in their original translation-unit form (module form).
- **Non-exact drafts**: the best drafts of the remaining functions, kept in
  their units under `#ifdef RA2_NON_MATCHING`, each labelled with its score. The
  exact build never compiles them.
- **Native port** (`native/`): the recovered source built with modern clang on
  an SDL3 platform layer, with touch controls on iOS and optional enhancements
  (all off by default). It reads game data from your own installation at run
  time. See [native/README.md](native/README.md).
- **Tools and tests** (`tools/original_target/`, `tests/`), **manifests**
  (`manifests/`) and **docs** (`docs/`).

Never in the repo: the game, its data or executables, the compiler, extracted
bytes, disassembly or build outputs. You need your own copy of the game.

## Building

See [docs/ORIGINAL_TARGET_BUILD.md](docs/ORIGINAL_TARGET_BUILD.md) for the
exact build (VC6 under Wine) and [native/README.md](native/README.md) for the
native one. The exact build:

```sh
./test.command
./build.command     # fresh compile, strict checks, link and package
```

The process and completion criteria are in
[docs/ORIGINAL_TARGET_PROCESS.md](docs/ORIGINAL_TARGET_PROCESS.md); current work
is in [docs/HANDOFF.md](docs/HANDOFF.md).

## Credits

This work stands on other people's. Text adopted from these projects carries
attribution; licences and pinned commits are in [THIRD_PARTY.md](THIRD_PARTY.md).

- **Luke Gibson ([gibbo101](https://github.com/gibbo101))** and the
  [ra2-decomp](https://github.com/gibbo101/ra2-decomp) contributors: the Yuri's
  Revenge matching decompilation. Luke gave permission to reuse its source and
  sent the twin leads in
  [pull request 1](https://github.com/meastes/redalert2-decomp/pull/1); functions
  adopted from it say "from ra2-decomp" in the commit subject.
- **The OpenTS developers**: the
  [Tiberian Sun reconstruction](https://github.com/OpenTS-Developers/TibSun).
- **Marcel Bierling ([mischa85](https://github.com/mischa85))**:
  [RedAlert2](https://github.com/mischa85/RedAlert2), a matching decompilation of
  the same executable.
- **Electronic Arts**: the GPL source releases of Red Alert, Tiberian Dawn and
  Renegade.

Red Alert 2 is a trademark of Electronic Arts. This project grants no rights to
the game or its assets.
