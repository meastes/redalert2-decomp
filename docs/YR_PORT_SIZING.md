# Yuri's Revenge native port: sizing

Read-only sizing, 2026-11-15 (lane C17y; review's call, user-approved order). Nothing was
downloaded, built, posted or pushed. The installation was only listed and hashed. Starting YR
work needs the user's word and an AGENTS.md change ("Base RA2 only; Yuri's Revenge is a later
separate target"). Calibration used: no.

Every estimate below has two numbers: the **raw estimate**, as we would have sized it before, and
the **expected actual**, the raw estimate times our measured estimate-to-actual ratio.

## Our estimate-to-actual record

From HANDOFF "Estimates vs actuals":

| Job | Estimate (h) | Actual (h) | Actual / estimate |
| --- | ---: | ---: | ---: |
| HD vehicles 2x | 30–45 | 4–5 | ~0.12 |
| Modern controls + Queue Five | 30–40 | ~3 | ~0.09 |
| Smooth zoom 0.5–3x | 15–25 | ~8 | ~0.40 |
| YR identical-twin batch | ~11 | ~1.3 | ~0.12 |
| Performance (lane P1) | 4 | ~1.5 | ~0.38 |
| Original-save writing, cut A–E+G (still open) | 45–55 | ~11 so far | ≤0.22 so far |

The ratio over the five finished jobs, by midpoint sums, is **about 0.18**. The median is
**0.12**.

These were feature jobs on a pipeline that already worked. A port to a new tree carries more
unknowns: another project's source conventions, a new address map, unmatched code we have
never seen. Below we use **0.3**, with 0.18 as the optimistic case. The expected actual is
given as a range: raw × 0.18 to raw × 0.3.

One scale reference comes from git history. The RA2 native port started from recovered source
on 2026-11-08 (Native M1). It reached a played menu → skirmish → win → score → menu loop on
2026-11-09 (M15, Scout1125). That is about two days of one owner plus one or two native lanes,
roughly 50–70 lane-hours. The tools it built then (native_units.py, the link closure,
original_data, the layout field maps) would not have to be written again.

## (a) Which gamemd.exe, and does the user have it

**ra2-decomp's target** is `gamemd.exe` **1.001** (`private/reference/ra2-decomp/README.md`).
It accepts three packagings of the same code:

| Build | Size | MD5 |
| --- | ---: | --- |
| Retail CD with the official 1.001 patch | 4,813,072 | `937b14db32f07ee47e92a8114ebdbdde` |
| The First Decade | 4,813,072 | `6604fd6f4ab71a11c708ea423dc0a29e` |
| Steam | 5,286,208 | `2a1307f6f57c9c492f985b4e3afb88e6` |

Its checker restores the few bytes the later builds changed.

**The user's installation** (`/Volumes/Lesalia/Command and Conquer Red Alert II`, one folder for
both games) has Yuri's Revenge:
- the executable: `gamemd.exe`, `gamemd.exe.bak`;
- data and media: `ra2md.mix`, `expandmd01.mix`, `langmd.mix`, `mapsmd03.mix`, `movmd03.mix`,
  `multimd.mix`, `thememd.mix`;
- the launchers `YURI.exe` and `RA2MD.exe`.

Its two executables:

| File | Size | SHA-256 | MD5 |
| --- | ---: | --- | --- |
| `gamemd.exe` | 4,816,864 | `7e839d3bb42f2989cea5d01884fb340d26a09f9184d47453943d9552852b6077` | `65ea7cd87363843ca71e8eda2cf22a96` |
| `gamemd.exe.bak` | 4,816,864 | `7cd005d263fde203d9c84548200a057a8df61d724da3c6bd1e521eeb61cd0747` | `56d582a1d6f3c144d3adc867d7a4d91b` |

- **It is 1.001 code in a size ra2-decomp does not list.** The PE timestamp is 0x3BDF544E. The
  four sections end at 4,812,800 bytes, followed by a 4,064-byte overlay (an installer or
  launcher signature), which accounts for the size.
- **Its code matches ra2-decomp's.** Their manifest's per-function SHA-256 agrees with
  `gamemd.exe` on 19,068 of 19,073 matched functions. The 5 that differ are at sites their
  checker restores, so the code is theirs. (Measured 2026-11-14 and again today, 1,999 of the
  first 2,000.)
- **`gamemd.exe` differs from its `.bak` by 2 bytes**, at file offset 0x12F94A. That is a local
  patch, most likely the CD check. The port would read either one. Our RA2 rule already skips
  the CD check (user decision, 2026-11-08).
- **Next to the retail 1.001 hash, it fails.** Their checker compares against an `orig/gamemd.exe`
  with the retail MD5 and size. Ours would fail a strict size/MD5 check on the overlay alone, so
  a native YR build should identify the build by its code sections, as our RA2 per-build loader
  does.

## (b) ra2-decomp's coverage, and what we hold

ra2-decomp is at `f8d8d53` (2026-11-15 00:37 UTC; today's standing pull found no new commits).
`build/yr-overlap-20261114/overlap.py` was rerun today against our catalog on main.

| | Functions | Bytes |
| --- | ---: | ---: |
| YR game code (outside the library span) | 19,353 | 3,815,998 |
| Matched (their manifest) | 19,072 | 3,254,313 (85%) |
| **Unmatched** | **281** | **561,685** |

Their README adds that, with their best attempts counted, the fuzzy match is 95%. Their whole-
executable link (`tools/exelink`) fills the unmatched 15% with the original's bytes. A native
build cannot do that.

Of the 281 unmatched, what we hold as exact RA2 counterparts (unchanged from the 2026-11-14
measurement in `docs/YR_OVERLAP.md`):

| Our counterpart | Functions | Bytes |
| --- | ---: | ---: |
| Byte-identical after relocation masking | 55 | 37,745 |
| Near-identical (22 layout, 54 fuzzy) | 76 | 160,954 |
| Different text, same function by name or position | 7 | 11,078 |
| **Exact RA2 counterpart, any kind** | **138** | **209,777** |
| Counterpart, but not exact on our side | 7 | 17,322 |
| No counterpart (mostly YR-only features) | 136 | 334,586 |

Our four upstream PRs (#258–#260; 6.3 KB, identical twins) are open, not merged, so they are
still counted as unmatched above.

**For a native YR build:**
- **138 functions (210 KB)** can start from our exact RA2 source, adapted to YR's layouts. Byte-
  identical twins need only renaming.
- **143 functions (352 KB)** need behaviour drafts written fresh, as the RA2 native tree did for
  its 133 unmatched functions.
- **The 48 MASM objects** (Westwood's blitters, LCW and the like) are reassembled, not
  decompiled, in ra2-decomp. Natively they need C. Our RA2 native tree already has C for the same
  routines, so most of these carry over.

## (c) What a "combined port" means

Two concrete meanings, which are different jobs.

**1. One app, both games, each running its own code.**
- The RA2 tree (ours, `src/recovered`) and the YR tree (ra2-decomp's `src`) are each compiled
  natively. RA2 plays RA2's campaign with RA2's rules; YR plays YR.
- Faithful to both games. This is the natural reading of "port both".
- It doubles the game code: about 19.4k plus 19.3k functions, mostly the same names. One
  *binary* needs one tree's symbols renamed: a per-game prefix or namespace emitted by
  native_units.py, plus per-game globals. One *app bundle* holding two executables behind a
  shared launcher needs none of that.

**2. YR's code running RA2's campaign and rules.**
- Only the YR tree is built, and RA2's content is fed to it: `ra2.mix`, RA2 maps, `rules.ini`
  instead of `rulesmd.ini`.
- This is not RA2. YR changed the engine's behaviour, not only its data:
  - new logic in shared classes (mind control, garrison and Battle Fortress, cloning, the
    Boomer and subterranean movement, extra super-weapon kinds);
  - balance and engine constants that live in code;
  - RA2-only paths YR dropped.
- RA2's campaign maps reference RA2 rules sections and triggers. Some would run, some would not.
- It conflicts with the project's acceptance model: an exact RA2 that plays as RA2. It would
  also throw away the RA2 native tree.
- **Not recommended.** The RA2 campaign on YR's engine is a mod-shaped project, like those built
  on Ares/Phobos, which the user has excluded.

**The differences that matter** (YR against RA2):

- **Class layouts grew.** Object sizes taken from the `push size; call operator new` in each
  type's `Create_One_Of`, read from the two executables today:

  | Class | RA2 bytes | YR bytes | Growth |
  | --- | ---: | ---: | ---: |
  | BuildingClass | 1,504 (0x5E0) | 1,824 (0x720) | +320 |
  | UnitClass | 2,000 (0x7D0) | 2,280 (0x8E8) | +280 |
  | OverlayClass | 156 | 176 | +20 |

  The shared bases (TechnoClass and its type classes, HouseClass, RulesClass) also grew. This is
  why 22 of the near twins differ only in field offsets.
  - Every shared header needs a YR layout. ra2-decomp has its own headers (OpenTS/TibSun-based,
    named from YRpp).
  - Our pointer-width field maps (`tools/original_layouts.py`, generated from our headers) would
    be generated again from theirs. The generator carries over; the maps do not.
- **New types and code.** YR-only classes, mission kinds, locomotors, super weapons and events
  (the 136 unmatched functions with no RA2 counterpart are mostly here).
- **Rules and art files.** `rulesmd.ini`, `artmd.ini` and YR's `.mix` set (`ra2md.mix`,
  `expandmd01.mix`, `langmd.mix`, …). The native file layer reads them as it reads RA2's (case-
  insensitive lookup, mix search order, mod layer). No new mechanism is needed.
- **Save format.** YR saves are the same COM structured storage with YR's layouts and class IDs.
  - Our original-format reader and writer (lane C17) are generic over the field maps, so YR
    saves need YR maps. RA2 and YR saves do not cross-load in the original game either.
- **Network protocol.** YR's LAN game is the same IPX-style layer with its own version, event
  set and game-option packets. RA2 and YR do not play each other in the original.
  - Our IPX-over-UDP shim (`port/src/wsock32.cpp`) is protocol-agnostic and carries over
    unchanged. A combined app keeps two separate lobbies, as the originals did.
- **Unmatched code.** 281 YR functions (above) against our 133 RA2 drafts.

## (d) How much of `native/` carries over

Line counts of the native tree today (C/C++/ObjC/Python/CMake sources):

| Part | Lines | Carries over to YR |
| --- | ---: | --- |
| `platform/sdl` (host: window, picture, input, touch, menus' look, enhancements page shell, settings, iOS/Mac app) | 6,099 | **Unchanged**, apart from a game choice and app name |
| `platform/win` (Win32 POSIX layer, crash handler) | 1,252 | **Unchanged** |
| `port/src` + `port/include` (Win32, DirectDraw, DirectSound, WinSock/IPX-over-UDP, ole32 storage, timers, file layer and mods, run progress, per-build image reader) | 10,838 | **Unchanged** (the per-build loader is already build-keyed) |
| `tools` (native_units.py, link_closure.py, data_globals.py, original_data.py, address/member aliases) | 5,917 | **Adapted**: same pipeline, new input format (their `src` tree, `functions.csv`/`manifest.csv`/`names.csv` instead of our catalog and manifests) |
| `tests` | 4,720 | **Mostly reused**: same harness, per-game coordinates, IDs and maps |
| `link` (107 behaviour rows, harness, native hooks for HD vehicles, map zoom, Queue Five, modern controls, saves) | 14,917 | **Per game**. Hook logic carries over; the hook points (fixups) and the RA2 behaviour rows do not. About a third have YR twins |
| `drafts` (RA2's unmatched functions as behaviour code) | 14,198 | **Per game.** YR needs its own for 281, about 138 starting from ours |
| `data` (generated globals and original data) | 12,200 | **Regenerated** for gamemd.exe by the same tools |
| `port/fixups.json` (492 source-text fixups for clang, LP64 and native hooks) | (JSON) | **Per game.** The patterns carry over (pointer-width casts, signed pitch, combo handles, swizzle ids); the text matches ra2-decomp's spelling, not ours |

In round numbers, about 28k of 70k lines (40%) carry over unchanged, about 11k are adapted, and
about 30k are per-game code that is generated or written again.

**By feature:**
- **Carry over:** the enhancements (picture scaling, menu fill, Map zoom and zoom gestures, HD
  vehicles 2x/3x, Queue Five, modern controls, vsync, mods), saves (container and original
  format), LAN, iOS and Windows packaging.
- **Need re-hooking, about 30–60 hook points:** the enhancements and save hooks, which attach
  through fixups to specific functions.
- **Need YR field maps:** saves.

## (e) Licence

- ra2-decomp's game source (`src/`, `include/`) is **GPL-3.0-or-later**.
  - Files carrying Electronic Arts or OpenTS material add EA's GPL section 7 terms (`COPYING.md`).
  - Their tooling and data are **MIT**.
- Our repository is already **GPL-3.0-or-later** (`LICENSE`, `THIRD_PARTY.md`). Building the YR
  port on their source is therefore licence-compatible, and the combined work stays GPL-3.0-or-
  later.
- What that means concretely:
  - Their files keep their SPDX line and copyright lines, including the EA and OpenTS lines and
    the section 7 terms where present. THIRD_PARTY.md gets a pinned entry. This is the same rule
    the user approved for adopted ra2-decomp text on 2026-11-06.
  - Anyone receiving a native YR binary gets the corresponding source under GPL-3.0. That
    already holds for our RA2 binaries.
  - EA's section 7 terms add trademark/attribution conditions (no use of EA marks, keep their
    notices). They do not restrict distributing the code.
  - Their MIT tools (checker, exelink) are not needed for a native build. If any were copied,
    their MIT notice would be kept.
  - YRpp is used by ra2-decomp only as a naming guide; they ship nothing derived from it. A YR
    lane would follow the same line, under our existing rule that YRpp-only names stay
    hypotheses.
  - No game data or executable is copied or shipped: the port reads the user's installed
    `gamemd.exe` and mix files at run time, under the same 2026-11-08 user decision as for RA2.
- Two points for the user to confirm, since they are licence/adoption questions:
  1. Building a YR binary from ra2-decomp's source is adopting third-party source as a whole
     tree, not function by function. The 2026-11-06 decision covered functions adopted into our
     `src/`.
  2. The dependency on an external, moving project: we would pin a commit and pull at most
     hourly, as for the standing references.

## (f) Staging plans

**Plan 1: YR as a second native game in its own executable (recommended).**
Build ra2-decomp's tree with our native pipeline into `yr-native`, a separate executable linked
with the same `platform/` and `port/`. One Mac app bundle (and one iOS app) holds both games
behind a game chooser.
- **Stage 1, pipeline on their tree.**
  - native_units.py and link_closure.py read their `src`, `functions.csv` and `manifest.csv`;
    data_globals and original_data read `gamemd.exe` (their `names.csv` gives proven globals);
    a build-keyed loader recognises the user's 1.001 by its sections.
  - The LP64 fixup sweep applies our patterns to their spelling.
  - The 281 unmatched start as traps.
  - Raw 60–90 h; expected 18–27 h (×0.3), 11–16 h (×0.18).
- **First playable milestone (stage 2): YR main menu → skirmish on a YR map → played to the score
  screen on the Mac.**
  - Traps reached on that path are filled first, starting from our RA2 twins.
  - Raw 50–80 h; expected 15–24 h.
- **Stage 3, the rest of the 281** (138 from RA2 twins, 143 YR-only drafts), the campaign start
  sweep, saves with YR field maps, LAN (YR to YR).
  - Raw 120–200 h; expected 36–60 h.
- **Stage 4, enhancements re-hooked; Windows and iOS packaging.**
  - Raw 30–50 h; expected 9–15 h.
- **Total to a played YR with saves, LAN and enhancements:** raw 260–420 h; expected about
  80–125 h at ×0.3, 45–75 h at ×0.18.

**Plan 2: one binary running both games.** Plan 1's stages, plus one tree's symbols put in a
per-game namespace or prefix by native_units.py (about 19k functions and their globals, the
link closure and data tables per game), plus a shared startup that picks the game before
WinMain.
- Its only gain over Plan 1's two executables in one bundle is a single executable, which no
  platform requires (an iOS app may hold two executables only as extensions, so iOS would want
  this or a build-time choice).
- First playable milestone: as Plan 1, inside the combined binary.
- Extra: raw 40–70 h; expected 12–21 h on top of Plan 1. **Do only if iOS packaging needs it;**
  otherwise ship two iOS apps or one app built per game.

**Plan 3: YR's engine running RA2's campaign** (combined port meaning 2).
- First playable milestone: RA2 Allied 1 started from YR's engine with RA2 rules and maps.
- Raw 80–150 h; expected 24–45 h for a first mission, open-ended for parity.
- **Not recommended:**
  - it is not RA2's behaviour;
  - it drops the exact RA2 tree the project exists for;
  - it is the Ares/Phobos-style direction the user excluded ("Do not adopt a lifted runtime").

**Cheapest first step if the user says yes to YR:** Plan 1, stage 1 only, boxed at about
20 lane-hours. It shows whether ra2-decomp's tree goes through our native pipeline cleanly before
any commitment to a playable target. Report at the box with the menu or the first traps.

## What starting needs

- The user's word, and an AGENTS.md change. The fixed constraint reads "Base RA2 only; Yuri's
  Revenge is a later separate target". The native-phase rules name RA2 only.
- The user's confirmation on adopting ra2-decomp's tree as a whole (licence point 1 above).
- No downloads: the user's `gamemd.exe` and YR mix files are already installed and readable.
  ra2-decomp is already a standing-pull reference clone.
