# Red Alert 2 decompilation and native ports

This project rebuilds the source code of Command & Conquer: Red Alert 2 (the 2000 Westwood
game, version 1.006 of its `game.exe`) so that, compiled with the compiler Westwood used, it
produces the original program again, byte for byte. About 94.1% of the code is rebuilt from source
in the current tree, `rebuild/` (94.3% counting each function checked on its own): one source file
per original source file, laid out like Westwood's own code. The earlier tree, `src/`, reached 93.6%
on the same measure and is kept unchanged as a reference (tag `src-final`); the native ports still
build from it while they move to the new tree. The figure is expected to move in the coming days as
held forms are decided. The rest of the code is still taken from the original. The recovered source
is also compiled with a modern compiler into native games for the Mac, iPhone and Windows. It is a
decompilation (source code worked back out of a compiled program by hand and with tools), not a
remake: the game logic is the original's.

What it is not: it contains no game files. There are no graphics, sounds, movies, maps or
executables from the game here, and no downloads; you need your own copy of Red Alert 2. The
exact rebuild covers base Red Alert 2 1.006 only. Yuri's Revenge, the expansion, is a separate
native build made from the [ra2-decomp](https://github.com/gibbo101/ra2-decomp) project's
GPL-licensed source; it is playable but not part of the exact rebuild.

## What you can do today

All of this needs your own installed copy of the game; the ports read its files at run time.
Details, and how each claim was checked, are in [docs/PORTS_FACTS.md](docs/PORTS_FACTS.md).

- **Mac** (Apple silicon, macOS 26 or later): campaigns, skirmish against the computer, LAN
  games, saving and loading, movies, sound and music. All 24 campaign missions start; played to a win:
  Allied 7 of 12, Soviet 2 of 12. A test forces each mission's win in turn and reaches every end
  screen.
- **iPhone** (iOS 17 or later): one app holds both Red Alert 2 and Yuri's Revenge, switched with
  a "Play Yuri's Revenge" / "Play Red Alert 2" button on the main menu. Touch controls: tap to
  select and move, box select, pan, pinch zoom, an on-screen toolbar and keyboard. It copies the
  game files from a folder you pick on first start. iPad is not verified.
- **Windows** (10 or later, 64-bit): cross-compiled on a Mac; played in LAN games against the Mac
  on a real Windows PC.
- **LAN play** has been played Mac with a Windows PC, and Mac with iPhone. iPhone with Windows
  has not been tried. Internet play is switched off.
- **Saves** are written in the original game's own format: the original Windows game loads saves
  the port wrote, and the port loads the original game's saves.
- **Optional enhancements**, all off by default (with them off, test frames are identical to the
  original's): map zoom, menus that fill the window, a larger interface, modern RTS controls,
  queueing five units at once, higher-detail vehicles, and mods.
- **Yuri's Revenge** (Mac and iPhone): skirmishes play through to the score screen and save in the
  original format. It is rougher than Red Alert 2: the loading screen is incomplete, campaigns
  have not been tried, there is no Windows build yet, and in longer games the fighting reaches a
  function not yet written for it, which stops the game. See
  [docs/PORTS_FACTS.md](docs/PORTS_FACTS.md) for the open list.

There are no signed builds: macOS and Windows will warn when you open what you build.

## Where it stands

Numbers are generated into [docs/STATUS.md](docs/STATUS.md); the headline ones:

| Measure | `rebuild/` (tree of record) | `src/` at `src-final` (frozen reference) |
| --- | ---: | ---: |
| Reproduced .text (built from real inputs, not copied from the original) | 94.13% (94.31% function by function) | 93.58% |
| C/C++ function bodies matched exactly | 3,295,764 bytes (87.43%) | 3,274,492 bytes (86.86%) |
| Functions matched exactly | 17,807 in 440 original source files | 17,910 |
| Functions not yet exact (kept as labelled drafts) | 110 (88,956 bytes) | |

21 of the rebuild's matched functions (53,972 bytes, 1.4% of the code) use a form chosen by this
project rather than one found in Westwood's code or a reference decompilation, allowed since 24
November and flagged by kind: 6 keep a small inline helper whose existence the machine code shows
(21,121 bytes), 11 spell a statement or condition differently (22,386 bytes), 4 name a local
variable the compiler needs (10,465 bytes), and none yet uses a readability alias. Without them the
rebuild stands at about 92.7% (92.9% counting each function on its own), and matching reference
text replaces any of them when it turns up.

- **Reproduced**: the share of the program's machine code (its `.text` section) that the build
  makes from real inputs (source code, identified compiler library code, and handwritten
  assembly) instead of copying it from the original file. 100% is the finish line.
- **Matched exactly**: a function counts only when the compiler, given its recovered source,
  emits exactly the original bytes. "In scope" leaves out the compiler's own runtime library and
  the game's handwritten assembly.
- **Non-exact draft**: a readable best attempt at a function that does not yet compile to the
  exact bytes. Drafts are kept in the source behind a switch, labelled with their score; the
  exact build never uses them (the native games do).

The whole-file build already reproduces the original executable exactly (4,485,392 bytes,
SHA256 `e6418111c2c36081a1795fb000e0927beea1630170b334c5013d12102602e70f`), because whatever is
not yet rebuilt from source is still taken from the original. STATUS counts exactly what that is.

### Still rough

- **Placeholder names.** Many fields and variables, and some classes, still have honest
  stand-in names where nobody has worked out what they mean yet: a field such as `unknown_0x0c`,
  a class such as `UnknownAnimTechno`. Decompiler-generated names (such as `FUN_` followed by an
  address) have been removed from the code.
- **Structure.** `rebuild/` is organised like the original: one source file per original source
  file, one header per class, no per-file switches. Its census (STATUS, "Rebuild structure") counts
  0 classes defined twice, 6 stray `extern` declarations in landed files and 0 names carrying an
  address suffix. The frozen `src/` tree is organised by function, as it was matched (about 1,960
  one-function files, 121 of them named after addresses, and about 7,200 address-suffixed names);
  it is kept unchanged as a reference and as the base the native ports still build from.
- **110 functions not yet exact** in the rebuild (about 89 KB of machine code) are kept as labelled
  drafts behind a switch; the exact build takes their bytes from the original.

## Two trees

The repository holds two source trees for the same `game.exe`. They are built separately, and their
numbers are never added together.

### Tree of record: `rebuild/`

Since 24 November 2026 the exact-match work, the numbers and the public snapshot follow
`rebuild/`: one source file per original source file, one header per class, members as members, no
per-file switches. Every function is checked byte for byte with the original compiler, then the
whole-file build must reproduce the original executable. Most of its text is adopted, with credit,
from mischa85's RedAlert2 decompilation (16,867 functions); 185 were reshaped from `src/`, 163
written fresh, and 592 came from the Windows workers. Details, provenance and licences:
[rebuild/README.md](rebuild/README.md). `./build.command` builds it.

### Frozen reference: `src/` (tag `src-final`)

`src/` is the earlier exact-match record (93.6% reproduced), kept unchanged at the tag
`src-final`: it is read-only, still reproduces the original (`./build.command --tree src`), and is
the evidence many of the rebuild's rows cite. The native ports build from it until a separate
ports repository takes the rebuild at a pinned commit.

## How exact matching works

Red Alert 2 was compiled with Microsoft Visual C++ 6.0, Service Pack 3. This project uses that
same compiler (run on a Mac under Wine) and the same options. For each function, the recovered
C++ is compiled and the result compared, byte for byte, with the function in the original
`game.exe`, including every call, data reference and padding byte. Only an exact match is
accepted, with no masking of differences. Because a compiler turns source into machine code
deterministically, source that produces the identical bytes behaves identically: the same
arithmetic, the same order of reads, writes and calls, the same data layouts. The final check
links everything into a whole executable and confirms it is identical to the original file.

What the match proves is behaviour, not wording. The original names and comments did not
survive compilation; the names used here are chosen from evidence, such as the class names the
executable itself carries (its RTTI, the run-time type information the compiler stores for
classes with virtual functions) and the names in sister projects (the Yuri's Revenge and
Tiberian Sun reconstructions).

## Repository map

| Folder | What it holds |
| --- | --- |
| `rebuild/` | The tree of record: one source per original unit (`rebuild/src/`), one header per class (`rebuild/include/`), the assembly sources (`rebuild/asm/`), its manifests and tools ([rebuild/README.md](rebuild/README.md)) |
| `src/` | The frozen earlier tree (tag `src-final`): `src/recovered/` (exact functions) and `src/shared/` (class layouts and headers); the native ports build from it |
| `native/` | The native Mac, iPhone and Windows ports: build files, the SDL3 platform layer, touch controls, enhancements, tests |
| `native/yr/` | The Yuri's Revenge native build: glue and fixes applied to the pinned ra2-decomp source |
| `yr/` | Pinned, unedited copies of the ra2-decomp and Tiberian Sun reference sources the Yuri's Revenge build uses |
| `tools/` | The build, acceptance and analysis tools (`tools/original_target/` for the exact build) |
| `manifests/` | Byte-free records: which source rebuilds which part of the original, class names, toolchain pins |
| `tests/` | Tests for the tools |
| `docs/` | Process, status, build instructions and decisions |

At the top level, `build.command` runs the exact whole-file build of `rebuild/` (`--tree src` for
the frozen tree) and `test.command` the tool tests.

## Getting started

### Play a port

Building any port currently needs a Mac: there is no way to build on Windows yet (the Windows
version is cross-compiled on the Mac). Follow [native/README.md](native/README.md). You need:

- a Mac with Apple silicon and macOS 26 or later (the Windows build is cross-compiled on the
  Mac too);
- Xcode or its command-line tools (clang), CMake, Ninja and Python 3;
- the SDL3 library: installed for the Mac build, or its 3.4.2 source for the iPhone and Windows
  builds (nothing is downloaded by the build);
- for Windows, the llvm-mingw cross toolchain; for a real iPhone, your own Apple developer
  signing;
- your own installed Red Alert 2. Yuri's Revenge files are needed only if you want to play that
  game (the iPhone app asks for them only when you switch to it).

### Build the exact target

Follow [docs/ORIGINAL_TARGET_BUILD.md](docs/ORIGINAL_TARGET_BUILD.md). You must supply, from your
own copies: Visual C++ 6.0 with Service Pack 3 (the compiler and linker), Wine or CrossOver to run
it, the original `game.exe` 1.006 (its SHA256 is checked), and a Python environment from
`requirements-research.txt`. The build also reads per-file inventories made from the original
executable, which are not in the repository, so a fresh clone is not yet a one-command build.
With those in place:

```sh
./test.command
./build.command             # rebuild/: fresh compile, strict checks, link, and compare with the original
./build.command --tree src  # the frozen src/ tree, the same way
```

The rebuild's build also reads a build configuration its landing gate writes under `build/r/`.

## Contributing and following along

Current work and the next targets are in [docs/HANDOFF.md](docs/HANDOFF.md); the generated numbers
in [docs/STATUS.md](docs/STATUS.md); the workflow in
[docs/ORIGINAL_TARGET_PROCESS.md](docs/ORIGINAL_TARGET_PROCESS.md).

How a function gets accepted: its source, in its original source file under `rebuild/src/`, is
compiled with the original compiler and compared with the original instruction by instruction,
and reworked until it matches. The unit is then catalogued (every row exact, no hidden machine
code, no copied instructions, every reference and padding byte accounted for, the structure
budget kept), and the whole-file build must still produce the identical executable before the
commit lands.

## FAQ

**What do I need to own?** A legitimate copy of Red Alert 2 (version 1.006 for the exact
build), and Yuri's Revenge if you want to play it. For the exact build, also Visual C++ 6.0 SP3.

**Does multiplayer work?** LAN games, yes: played Mac with a Windows PC, and Mac with iPhone
(iPhone with Windows has not been tried). Internet play is disabled.

**What about Yuri's Revenge?** It is a separate native build from ra2-decomp's GPL source,
sharing the ports' platform layer. It plays skirmishes on the Mac and iPhone but is less complete
than Red Alert 2, and it is not part of the exact rebuild.

**Where can I download it?** Nowhere: there are no binary downloads. You build from source and
supply your own game files.

**Why Visual C++ 6?** It is the compiler the original was built with. Only that compiler, with the
same options, can reproduce the original bytes, and the exact match is what proves the recovered
source is right.

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

## Licence

The project's own tools, native port code and documents are licensed under the GNU General
Public License, version 3 or later ([LICENSE](LICENSE)). The recovered source in `rebuild/` and `src/` is
reconstructed from the original program and makes no new grant of rights in the game or the
compiler. Adopted third-party text keeps its own licence and attribution, listed in
[THIRD_PARTY.md](THIRD_PARTY.md).

Command & Conquer and Red Alert are trademarks of Electronic Arts. This project is not
affiliated with or endorsed by Electronic Arts, and grants no rights to the game or its assets.
