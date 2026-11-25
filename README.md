# Red Alert 2 decompilation

This project rebuilds the source code of Command & Conquer: Red Alert 2 (the 2000 Westwood
game, version 1.006 of its `game.exe`) so that, compiled with the compiler Westwood used, it
produces the original program again, byte for byte, in one source file per original source file,
laid out like Westwood's own code. Most of the code is adopted from other projects, with credit; the
largest source is mischa85's RedAlert2 (the split by source is below). What is not yet rebuilt from
source is still taken from the original; [docs/STATUS.md](docs/STATUS.md) says how much. It is a
decompilation (source code worked back out of a compiled program by hand and with tools), not a
remake: the game logic is the original's.

**The native ports** (Mac, iPhone, Windows, and Yuri's Revenge) live in their own repository,
[redalert2-modern](https://github.com/meastes/redalert2-modern), which builds this tree at a pinned
commit with a modern compiler. They moved there on 25 November 2026; this repository's earlier
`native/` folder and its earlier `src/` tree are kept in its history (tag `src-final` for the old tree).

What it is not: it contains no game files. There are no graphics, sounds, movies, maps or
executables from the game here, and no downloads; you need your own copy of Red Alert 2. The
exact rebuild covers base Red Alert 2 1.006 only.

## Where it stands

Numbers are generated into [docs/STATUS.md](docs/STATUS.md); the headline ones (25 November 2026):

| Measure | Value |
| --- | ---: |
| Reproduced .text (built from real inputs, not copied from the original) | 94.34% (94.52% function by function) |
| C/C++ function bodies matched exactly | 3,303,651 bytes (87.63%) |
| Functions matched exactly | 17,819 in 441 original source files |
| Functions not yet exact (kept as labelled drafts) | 102 (84,118 bytes) |

22 of the matched functions (55,650 bytes, 1.5% of the code) use a form chosen by this project
rather than one found in Westwood's code or a reference decompilation, allowed since 24 November
and flagged by kind: 7 keep a small inline helper whose existence the machine code shows, 11 spell a
statement or condition differently, 4 name a local variable the compiler needs. Matching reference
text replaces any of them when it turns up. The earlier tree (`src-final`) reached 93.58% on the same
measure; its frozen table is [docs/history/SRC_FINAL_STATUS.md](docs/history/SRC_FINAL_STATUS.md).

- **Reproduced**: the share of the program's machine code (its `.text` section) that the build
  makes from real inputs (source code, identified compiler library code, and handwritten
  assembly) instead of copying it from the original file. 100% is the finish line.
- **Fuzzy (best draft scores)**: the figure ByteMatch shows as "Fuzzy match": each function not
  yet exact counts at the share of its bytes its best measured draft already reproduces. It earns
  no exact-match credit; [docs/STATUS.md](docs/STATUS.md) carries it.
- **Matched exactly**: a function counts only when the compiler, given its recovered source,
  emits exactly the original bytes.
- **Non-exact draft**: a readable best attempt at a function that does not yet compile to the
  exact bytes. Drafts are kept in the source behind a switch, labelled with their score; the
  exact build never uses them.

The whole-file build already reproduces the original executable exactly (4,485,392 bytes,
SHA256 `e6418111c2c36081a1795fb000e0927beea1630170b334c5013d12102602e70f`), because whatever is
not yet rebuilt from source is still taken from the original. STATUS counts exactly what that is.

Most of the code is adopted from other projects, with credit: mischa85's RedAlert2 decompilation (the largest
source), gibbo101's ra2-decomp, the Tiberian Sun reconstruction, RobinDigitalPro's 1.001 rebuild, and EA's Renegade
and Generals sources; the rest is this project's own work. These sources build on one another, so the split by
source in [docs/STATUS.md](docs/STATUS.md) records where each function's text was taken from, not who first wrote it.
Every accepted function's provenance is in `manifests/provenance.csv`; the
working notes (provenance classes, how the parked tail is worked, rulings) are in
[docs/REBUILD.md](docs/REBUILD.md).

### Still rough

- **Placeholder names.** Many fields and variables, and some classes, still have honest
  stand-in names where nobody has worked out what they mean yet: a field such as `unknown_0x0c`,
  a class such as `UnknownAnimTechno`.
- **102 functions not yet exact** (about 84 KB of machine code) are kept as labelled drafts behind
  a switch; the exact build takes their bytes from the original.

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
| `src/` | One source file per original translation unit (`src/<unit>.cpp`) |
| `include/` | One header per class, plus the units' shared headers |
| `asm/` | The handwritten assembly sources (adopted Tiberian Sun `.asm`, and the revised RA2 procedures in `asm/ra2asm/`) |
| `manifests/` | Byte-free records: the catalog (`manifests/catalog/`: which source rebuilds which part of the original), the units, provenance, adopted files, progress; `manifests/original_target/` is the earlier tree's catalog, kept read-only as the evidence the rows cite |
| `tools/original_target/` | The build, gate, census and analysis tools |
| `tests/` | Tests for the tools |
| `yr/` | Pinned, unedited copies of the ra2-decomp and Tiberian Sun sources that redalert2-modern's Yuri's Revenge build vendors |
| `docs/` | Process, status, build instructions and decisions |

At the top level, `build.command` runs the exact whole-file build and `test.command` the tool tests.

## Build the exact target

Follow [docs/ORIGINAL_TARGET_BUILD.md](docs/ORIGINAL_TARGET_BUILD.md). You must supply, from your
own copies: Visual C++ 6.0 with Service Pack 3 (the compiler and linker), Wine or CrossOver to run
it, the original `game.exe` 1.006 (its SHA256 is checked), and a Python environment from
`requirements-research.txt`. The build also reads per-file inventories made from the original
executable, which are not in the repository, and a build configuration the gate writes under
`build/r/`, so a fresh clone is not yet a one-command build. With those in place:

```sh
./test.command
./build.command    # fresh compile, strict checks, link, and compare with the original
```

## Contributing and following along

Current work and the next targets are in [docs/HANDOFF.md](docs/HANDOFF.md); the generated numbers
in [docs/STATUS.md](docs/STATUS.md); the workflow in
[docs/ORIGINAL_TARGET_PROCESS.md](docs/ORIGINAL_TARGET_PROCESS.md).

How a function gets accepted: its source, in its original source file under `src/`, is
compiled with the original compiler and compared with the original instruction by instruction,
and reworked until it matches. The unit is then catalogued (every row exact, no hidden machine
code, no copied instructions, every reference and padding byte accounted for, the structure
budget kept), and the whole-file build must still produce the identical executable before the
commit lands.

## FAQ

**What do I need to own?** A legitimate copy of Red Alert 2 (version 1.006) and Visual C++ 6.0 SP3.

**Where are the Mac, iPhone and Windows games?** In
[redalert2-modern](https://github.com/meastes/redalert2-modern).

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

The project's own tools and documents are licensed under the GNU General Public License, version 3
or later ([LICENSE](LICENSE)). The sources in `src/`, `include/` and `asm/` are under the licence in
[LICENSE.md](LICENSE.md) (GPL-3.0-or-later, with Electronic Arts' additional terms under GPL section 7
for the material derived from their releases; much of the text is adopted from mischa85's RedAlert2).
The recovered source is reconstructed from the original program and makes no new grant of rights in
the game or the compiler. Adopted third-party text keeps its own licence and attribution, listed in
[THIRD_PARTY.md](THIRD_PARTY.md).

Command & Conquer and Red Alert are trademarks of Electronic Arts. This project is not
affiliated with or endorsed by Electronic Arts, and grants no rights to the game or its assets.
