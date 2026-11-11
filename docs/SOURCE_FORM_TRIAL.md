# Source-form trial: one module in its final shape

A one-module trial of the form the whole tree is meant to end in, done at the 2026-11-11
account switch (the user's 2026-11-10 decision on the source-form end phase, recorded in
docs/HANDOFF.md, pulled forward for one module on 2026-11-11). The question it answers:
will this decompilation end in something a person can read, change and rebuild, rather than
a pile of one-function files pinned to addresses with the data carried as raw bytes?

## What the end state looks like

- **One source file per original translation unit**, named after Westwood's file, in the
  shape of its Tiberian Sun twin where one exists. The file holds the unit's functions *and
  its data*: tables, static buffers, string literals, the class's vtable and RTTI, all as
  ordinary C++ that the compiler emits.
- **One header per class**, declaring the class as the original did.
- **No addresses in source.** Functions and data have their original names (or evidenced
  ones); nothing is called `Unknown_007131b0` or `version_object_00a40c88`. The addresses
  live in a per-module map the build writes (`manifests/original_target/modules/<name>.json`):
  every function's entry and size, every data item's address and size, the references into the
  module and out of it.
- **No stand-ins.** Other units call the module's functions by their real declarations from
  its header, not through locally invented prototypes. (None remain for this module; the
  ordinary gate now refuses one.)
- **The exact build still reproduces the original byte for byte**, and a second build mode
  proves the module is an ordinary object: linked where the linker puts it, behind padding,
  reached only through symbols, and the game still runs.

## The module: `version` (Westwood's version.cpp)

Chosen because it is small, fully matched, self-contained and owns real data: twelve
functions (2,016 bytes at 0x712B20-0x713300), one contiguous data block (632 bytes at
0x7F9700-0x7F9978: the `VersionProtocol` table, `Version_Name`'s static buffer and flag,
four string literals), a vtable with RTTI, 29 references in from 20 other units and 15
references out. Tiberian Sun's `version.cpp` is its twin and gave the file layout, names,
types and comments.

Before: 12 files under `src/recovered/classes/VersionClass/`, names such as
`VersionClass_Internal_Version_Name_00712ed0.cpp`, 47 `// @ref NAME=ADDRESS` lines, the
class declared inside `src/shared/lobby.hpp` behind a per-unit macro.

After: `src/recovered/version.cpp` and `src/shared/version.hpp`. The names are
`VerNum`, `Version_Number`, `Major_Version`, `Minor_Version`, `Version_Name` (member and
free function), `Read_Text_String`, `Version_Protocol`, `Init_Clipping`, `Clip_Version`,
`Min_Version`, `Max_Version`; the data is `VersionProtocol[]`, `buffer`, `empty` and the
literals. No address appears in either file. Four callers that had declared the module's
functions under invented prototypes (`Unknown_Version_ID_00712b50(VersionClass *)`,
`unknown_version_text_7131d0()` as `cdecl`, `unknown_internal_version_text_712ed0`) now call
the real declarations (the Init unit 0x516C40 through lobby.hpp's declaration, since another
file of its translation unit already includes lobby.hpp and version.hpp cannot be included
beside it). `module_check.py` refuses any reference to the module under another symbol.

## What the trial proved

1. **Code (exact).** The module file compiles to the twelve original bodies byte for byte
   under the strict audit, and the whole-file build reproduces the target
   (SHA256 e6418111...). The renames reached 401 populations (14,202 regions re-verified
   exact in one batch, 123 s). Population 3705 now records `/GR`, so the compiler emits the
   class's vtable and RTTI and the audit compares them with the original copies.
2. **Data (own ledger line).** `module_ledger.py` lays every initialized data section the
   module's object emits at the original address bound to its first symbol, resolves its
   relocations and compares the bytes: 623 of the 632 bytes of 0x7F9700-0x7F9978 are
   compiler-emitted and equal (5 sections: the table with the two statics, four string
   literals); the other 9 bytes are the linker's 4-byte alignment fill between them. The
   compiler's section order also reproduces the original layout. The vtable and its five
   RTTI structures (100 bytes) are emitted and equal too, at the copies the original linker
   kept from the globals object (0x7A2AB8, 0x7BA060-0x7BA0A4, 0x7D9B00). None of this is
   .text credit; STATUS.md carries it as "Module form".
3. **Shift build (`build.py --shift-module version`).** The module's object is linked as it
   is, its sections renamed `.text$zversion`, `.data$zversion`, `.rdata$zversion` (LINK sorts
   `$` suffixes after the fixed fragments), behind 512 bytes of padding; the original bytes
   of its code and data ranges become `int3` fill; the 29 references into it from accepted
   fragments become COFF relocations against its symbols (the placement container carries a
   symbol table for the first time); its 14 references out are symbols the scaffold defines at
   the original addresses. The module landed at 0x7997D0 (code), 0x7C8C38 (vtable, RTTI) and
   0xB2C9C8 (data); the static initializer at 0x4D45F0 calls the constructor there, the
   launcher check at 0x491B70 calls `Version_Number` there, the constructor stores the new
   vtable, whose slot points at the module's own deleting destructor.
   Run test (`tools/original_target/shift_run/`, repeatable), under the project's Wine host
   with the four test MIX files, the installed game's BINKW32.DLL beside the image, a helper
   holding the launcher's mutex (`CopyProtection_Check_Already_Running` bails without it)
   and the launcher's registry value `HKCU\WChat\SysID\ID` = 0x10006: the exact image and
   the shifted image behave the same for about 530,000 traced API calls each, past WinMain's
   checks, the COM class registrations and window creation, to the same stop, a message box
   `***FATAL*** String Manager failed to initilaized properly`. That stop is the test data
   set's doing, not the module's: the four MIX files carry no string table, and the native
   lane's harness stops at the same place with the same data. Direct evidence that the moved
   module ran:
   winedbg attached to the live process reads `VerNum` (0xA40C88) as `007c8c80 00000000 ...`
   in the shifted image, the module's relocated vtable written by its constructor at
   0x7997D0 from the static initializer, against `007a2abc` in the exact image. Both runner
   scripts and the traces are under `build/class-work/Scout1279/scratch/`; nothing of the
   installation was written to (the registry value and the helper were removed afterwards).
   Raw-address references the shift cannot redirect: one `call 0x713160`
   (`Init_Clipping`) at 0x5BF624 inside code that is still original bytes (a netdlg2
   function); four words at 0x826130 and one at 0x827D2C hold values inside the module's
   ranges at no symbol boundary (data coincidences, not pointers). The only tool change that
   removes the first is recovering that function; the shift build reports both.

## What it cost

Lane hours: about 1.3 (one Fable lane, 14:13-15:30 UTC on 2026-11-11; the batch tooling,
the data ledger, the shift mode, the run test and this document included). Mechanical: writing the module file from the twelve accepted bodies and
the TibSun shape; the rename batch; the ledger and map. Judgment: the header split (below),
the signatures to adopt from TibSun (all but `Version_Protocol`'s, which lobby.hpp's users
would not take), what counts as the module's data (the vtable copies the original kept from
another object are reported, not claimed), and how to prove a run under Wine.

### The one thing that did not go as planned

`lobby.hpp` cannot include `version.hpp`. Replacing its VersionClass block by the include
changes the instruction order (two loads swapped, 4 bytes) in `Print_CRCs` 0x62A360 and
`HouseClass::AI_Build_Defense` 0x4F1190, units that include lobby.hpp and never touch the
version object. Nine header variants bisected it: renames alone are harmless; adding any new
type to the declaration set lobby.hpp shows those units (the `CommProtocolType` enum, the
`VersionProtocolType` struct, even the class's private enums) flips them; the bit-fields are
not the cause. So lobby.hpp keeps a second declaration of VersionClass with the same member
signatures (and the flags word), and `CommProtocolType` lives in version.cpp. This is the
first concrete cost of the count-sensitive units to the end state. It is a flagged judgment
call (review 2026-11-11): the two flipping rows carry `split_declaration` with a note naming
both headers, the strict audit refuses a malformed flag and STATUS counts the rows; and
`module_check.py` runs in the ordinary gate (validate_catalog): every reference to the
module from another unit must use the decorated symbol the module defines, and the two
declarations must give the same object size, offsets, virtual functions and member-function
signatures. Known differences it reports rather than hides: the data members are
`unsigned long` in lobby.hpp and `unsigned int` in version.hpp (same width); lobby.hpp has
the flags word where version.hpp has the four bit-fields; version.hpp alone declares
`Major_Version`, `Minor_Version`, `Version_Text`, `Get_Clipped_Version` and the private
`Read_Text_String`.

## Whole-executable estimate (measured 2026-11-11)

- Data to define as source: `.rdata` 191,542 bytes and the initialized part of `.data`,
  425,984 file bytes (its 3,553,732-byte virtual size is mostly zero-initialized storage that
  needs declarations, not initializers).
- Accepted code references 13,846 distinct data addresses: 2,481 in `.rdata` (1,174 vtables,
  268 import slots; 2,341 under a real name, 140 under an address-named stand-in) and 11,365
  in `.data` (5,186 string literals; 8,900 named, 2,465 stand-ins).
- 377 functions are called under more than one symbol name by different units (351 of them
  accepted): each is a stand-in declaration to replace with the real header, as this trial did
  for three of the version module's callers.
- The tree holds 17,904 regions in 17,904 files; mischa85's unit map puts the code in about
  300 named modules plus unassigned ranges, so the end state is on the order of 400-500
  module files, as in ra2-decomp.
- The version module took about one lane-hour of mechanical work for 2 KB of code and
  0.6 KB of data once the tooling existed; the renames and the header question took the rest.
  At that rate the conversion is weeks of lane time, dominated not by typing but by the
  count-sensitive units: every header consolidation is a whole-tree build, and each
  flipped unit costs a bisection like the one above.

## Lane practices to stop now

1. Declaring another unit's function locally with an invented name, type or convention
   (`unknown_version_text_7131d0` declared `cdecl` for a `__fastcall` function;
   `Unknown_Version_ID_00712b50(VersionClass *)` for a member). Declare it in the owner's
   header; if the header cannot be included yet, declare the real signature.
2. Per-unit macros that show different member sets of one class to different units
   (`RA2_VERSION_CLASS_MEMBERS`). They hide exactly the count sensitivity the end state has
   to pay for.
3. (A proposal for the user, since AGENTS.md asks for the address suffix on understood
   names today.) Address-suffixed names for understood functions and globals: the catalog
   is the map; the suffix only has to be renamed away later across every population that
   mentions it.
4. Accepting a unit whose bytes depend on the declaration set of a shared header without
   recording which header and which declarations (the lobby.hpp cases were known only from a
   comment).
5. Writing a table or string in source once and leaving its neighbours as original bytes: a
   module's data range should be claimed whole (`modules.json`) so the ledger can refuse gaps.

## Tools added

- `apply_rename.py`: `modules` (several accepted regions become one source file) and
  `options` (a population's recorded option set, e.g. `/GR`).
- `module_ledger.py BUILD_DIR [--write]`: proves a module's compiler-emitted data against
  the original and writes its address map.
- `build.py --shift-module NAME`: the shift build (`shift_build.py`: section renames, a
  container with symbolic relocations, scaffold symbol definitions, a runnable image).
- `module_check.py`: the agreement checks (symbols, declarations) in the ordinary gate.
- `shift_run/`: the Wine runner, the memory peek and the launcher-mutex helper.
- `status.py`: the "Module form" ledger line (outside Reproduced .text and in-scope) and the
  `split_declaration` judgment flag.
- Tools that take one file per region now handle a module source or refuse it clearly:
  `native_units.py` reads it once per unit; `organize_by_class.py` never moves it;
  `lane_tools.py plan` and `accept_sources.py` refuse a population that holds one (it changes
  through `apply_rename.py`); `regenerate_seeds.py` only touches seed-generated rows;
  `callsite_rename.py` already grouped by file; `misses_index.py`/rescore and
  `sync_windows.sh` do not read sources.

## Measured cost after 25 modules (2026-11-11)

- 9.45 lane-hours for 25 modules (171 functions, 18,256 code bytes, 1,567 data bytes proven at their original addresses): 0.38 h per module, 3.3 min per function, 0.52 h per KB of code; about 9 weekly usage points (estimated, not metered).
- These were small leaf modules (730 B on average against about 7.8 KB for the average unit). Eleven took about 2 minutes each; medium ones (1-2.7 KB) took 0.4-1.1 h and left 1-6 header gaps each. The time goes into header attempts, each of which recompiles every unit that includes the header.
- 36 template details are withheld (modules.json header_gaps) because they shift a small set of declaration-count-fragile functions elsewhere (Reposition_Sidebar, Print_CRCs, AI_Build_Defense, WinMain, an owner-draw trackbar, Dropship_Screen, and initializer renumbering). Gaps grow with how widely a module's header is included, not with its size. The template headers (ra2-decomp, mischa85) did not shift fewer functions than ours.
- Whole tree (estimate): about 420 units, 500-1,300 lane-hours (central about 800), with the large class units (Techno, Building, Map, Unit, House) at 8-25 h each, plus 20-60 h at the end to re-roll the fragile functions under final headers (some as flagged calibrated forms). Parallel converters shorten the calendar time, not the total; four bring it to about 200-600 wall-clock hours, and the class hierarchy keeps the large units mostly serial.
