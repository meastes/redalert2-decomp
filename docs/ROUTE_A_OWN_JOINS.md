# Own-bound joins of stand-ins and second populations (lane F)

Rulings (review session, adopted by the owner 2026-11-13; docs/DECISIONS.md "Own-bound joins
widened"): the join covers stand-ins and a unit's own second population. Rows held by another
real unit's population stay out. Rows that join only by taking the unit's `/GR` are approved,
on three conditions:
- every RTTI structure the unit emits is still proven at its original address
  (`join_gate.py` checks each joined unit's module map);
- the manifest keeps the note;
- STATUS counts these rows on their own line.

Each landing batch goes through `tools/original_target/join_gate.py --base REV`. The gate checks:
- the whole-file build is exact;
- RTTI is still proven per joined unit;
- module_check passes;
- the native configure, build and ctest pass, with the link closure at its baseline.

A join is undone with `refile_unit.py unjoin UNIT [ENTRY...] --base REV`. A round trip (join
then unjoin) gives back the pre-join tree byte for byte; this was proven on isotype (10 rows,
8 native fixups, 2 vtable views) and on theme. The failures below are left as they are.

## Landing batches (on main 0ea0739ee)

| Batch | Base | Units joined (tried) | Rows | Bytes | Of which under the unit's /GR | Address-named .cpp after |
| --- | --- | ---: | ---: | ---: | ---: | ---: |
| 1 | 4de9dc74b | 96 (100) | 1,051 | 80,852 | 691 / 54,227 | 3,907 -> 2,856 |
| 2 | aceec3d01 | 73 (79) | 191 | 2,999 | 183 / 2,836 | 2,856 -> 2,665 |
| **Total** | | **169** | **1,242** | **83,851** | **874 / 57,063** | |

Each batch passed `join_gate.py`:
- the whole-file build is exact;
- every joined unit still proves its RTTI (96 and 73 units);
- module_check passes;
- the native build passes, with ctest 8/8 and link closure 290.

17,905 rows before and after.

Refused before compiling:
- `005c8cd0` (netshare) and `005ead00` (ownrdraw), with `--refuse`: each has a declaration
  that conflicts with another declaration of the same name in the unit, which the native
  build refuses;
- smartdeform's rows: they carry `data_bound_by`, which would have to cover a joining row too;
- files holding a vtable view classed `dead`.

More rows join here than in the measurement. codex1's Ghidra-header removal (r1986) took
away the header-state cause from rows that were stand-ins or second populations.

Left after both batches (by `refile_unit.classify_split_rows`): 308 rows / 34,646 B
(stand-ins 196 / 18,281; own second populations 112 / 16,365). Their causes are in
modules.json. The measurement table below gives the breakdown by cause.

An earlier run of the same batches on 585e696a1 (branch `lane-f-joins`) found a bug: a batch
apply draft kept the text of rows that had left the batch. It is fixed in e56ac87ae.

## Measurement (step 1, base 4915c3ee3)

Measurement for the review session (its call 2026-11-13, step 1 of the address-free file
names item). Branch only; nothing lands until the review answers. Base `4915c3ee3`.

Tool: `refile_unit.py join-own UNIT... | all [--adopt-gr] [--dry-run]`. The container-join
machinery (`joins_bound: own`, `bound_population`, both populations recompiled and strict
audited by `apply_rename.py`) applied to split rows that bring their own code. The row's
text goes into the unit's file in address order as it is (only directives the unit already
holds are dropped); a row whose file needs header state the unit lacks, another option set,
a closed compiler-output population, `__asm`, a binding conflict, a missing own frozen bound
or an unplaceable native fixup never tries. Screens compile the unit with the rows and halve
the set when the unit's own rows differ; survivors go into one `apply_rename` batch.

`--adopt-gr` (`apply_rename` `joins_options: "unit_gr"`): a row whose option set is the
unit's minus `/GR` joins under the unit's options. Needed because the /GR landing
(`4915c3ee3`) moved 353 unit populations to `/GR` while their stand-in and second-population
rows stayed frozen without it: 901 of the 1,376 rows were refused only for that. Such rows
carry `"options": "joined under the unit's /GR (frozen without it)"` in their `joined` entry.
This is the review question.

### Result (whole-file build exact, SHA e6418111..., zero rows lost, module_check 405 pass, native ctest 7/7, link closure 290)

| Class | Rows before | Bytes before | Joined rows | Joined bytes |
| --- | ---: | ---: | ---: | ---: |
| Stand-ins | 1,075 | 62,343 | 870 | 43,780 |
| The unit's own second population | 301 | 52,780 | 185 | 35,716 |
| **Total** | 1,376 | 115,123 | **1,055** | **79,496** |

Of the joined rows, 335 rows / 26,026 B have the unit's option set unchanged and 720 rows /
53,470 B joined under the unit's `/GR`. 17,905 accepted rows before and after;
`bound_population` rows 1,540 / 97,964 B -> 2,590 / 176,785 B. No joined row's symbol changed.

76 rows were joined exact, then taken out by the native gate. 74 rows / 4,053 B (52 in convert)
hold a seed call view that `native/data/vtable_views.tsv` classes `dead`. That check reads the
file's own object, so once the view is in the unit's object it fails. The precedent is
`run --keep` for native checks that read per file; `join-own` now refuses these rows before
compiling. Two more rows were taken out because the native build (clang) refuses a conflicting
redeclaration that VC6 accepts: `005c8cd0` (netshare, `extern "C" unsigned int *
unknown_lobby_players_00a3fa80` against another stand-in's `unsigned int`) and `005ead00`
(ownrdraw, `DefWindowProcA(int, int, int, int)` against the unit's win32 header). Neither
passes "one real codebase"; the tool does not screen for this yet (the native build does).

### What does not join (321 rows, 35,627 B)

| Class | Cause | Rows | Bytes |
| --- | --- | ---: | ---: |
| own population | differs under the unit's state (bytes differ) | 22 | 7,337 |
| own population | does not compile in the unit (C2653/C2084/C2143/C2039/C2838/C2628/C2228: names the unit spells differently or defines twice) | 37 | 2,993 |
| own population | rows of the unit differ when it joins | 15 | 2,291 |
| own population | its population changes when it leaves (another row of the donor differs) | 19 | 1,823 |
| own population | differs in the batch apply | 8 | 1,100 |
| own population | another option set (`/Gr`, `/Ob1`) | 10 | 806 |
| own population | the joined unit fails the strict audit (an extra `.CRT$XCU` dynamic initialiser) | 5 | 714 |
| stand-in | does not compile: C2039 (member not in the unit's class) | 4 | 6,830 |
| stand-in | its file holds a vtable view classed `dead` (native check reads per object) | 74 | 4,053 |
| stand-in | does not compile: C2011 (the stand-in type redefines the unit's real class) | 30 | 2,830 |
| stand-in | another option set (`/Gr`, `/O1`, `/Ob1`, `/G6`) | 31 | 1,570 |
| stand-in | differs in the batch apply | 26 | 1,428 |
| stand-in | rows of the unit differ when it joins (alone or with another row) | 13 | 948 |
| stand-in | the joined unit fails the strict audit (`.CRT$XCU`) | 16 | 646 |
| stand-in | other compile errors (C2084, C2371) and the two native conflicts | 8 | 222 |
| stand-in | differs under the unit's state | 3 | 36 |

Rows needing another unit's header state were not tried (they are the separate "header
state" class). The C2011 stand-ins need their placeholder type replaced by the real class
first (a declaration cleanup, not a join).

### Is a tree-wide run mechanical?

Mostly. `join-own all` then `join-own all --adopt-gr --attempts 80` ran unattended (12 and
about 60 minutes). The batch apply converged in 2-3 attempts, the whole-file build stayed
exact, and native fixups were re-keyed automatically. Three things were found only at the
gate and were handled by hand on this branch:
- the dead-view rows (now screened by the tool);
- the two conflicting declarations, which only the native build finds, so the native build
  and ctest must stay in the landing gate;
- the screen budget: the default of 24 was too small for wonline and mapgen (80 worked).
Undoing a join has no tool command yet (scratch script); add `unjoin` before any tree-wide
landing. The whole branch is one tree-wide run, so step 1 for these two classes is the
landing itself.

## For codex1: the 30 placeholder redefinitions (declaration cleanup)

Each row is a stand-in whose file defines its own placeholder for a type its unit already
defines for real. Inside the unit file that is error C2011 (type redefinition), so the row
cannot join. The cleanup: rewrite the row on the unit's real type, keeping it exact in its
own file. The row can then join (`refile_unit.py join-own UNIT --adopt-gr`). Byte counts are
body bytes.

| Unit | Entry | File | Redefined type | Bytes |
| --- | --- | --- | --- | ---: |
| ownrdraw | 00600e50 | `src/recovered/ownrdraw/unknown_get_window_wide_text.cpp` | class `WideString` | 96 |
| ownrdraw | 00600f10 | `src/recovered/ownrdraw/unknown_get_list_item_wide_text.cpp` | class `WideString` | 95 |
| ownrdraw | 00600fd0 | `src/recovered/ownrdraw/unknown_get_combo_item_wide_text.cpp` | class `WideString` | 95 |
| startup | 00691eb0 | `src/recovered/classes/TClassFactory_Tactical/CreateInstance.cpp` | class `Tactical` | 117 |
| startup | 006933b0 | `src/recovered/classes/TClassFactory_BombClass/CreateInstance.cpp` | class `BombClass` | 114 |
| startup | 006936b0 | `src/recovered/classes/TClassFactory_TEventClass/CreateInstance.cpp` | class `TEventClass` | 114 |
| startup | 00693b30 | `src/recovered/classes/TClassFactory_TActionClass/CreateInstance.cpp` | class `TActionClass` | 114 |
| startup | 00693cb0 | `src/recovered/classes/TClassFactory_TriggerClass/CreateInstance.cpp` | class `TriggerClass` | 116 |
| startup | 00693e30 | `src/recovered/classes/TClassFactory_TriggerTypeClass/CreateInstance.cpp` | class `TriggerTypeClass` | 119 |
| startup | 006942b0 | `src/recovered/classes/TClassFactory_TagClass/CreateInstance.cpp` | class `TagClass` | 116 |
| startup | 00694430 | `src/recovered/classes/TClassFactory_TagTypeClass/CreateInstance.cpp` | class `TagTypeClass` | 119 |
| startup | 00694a30 | `src/recovered/classes/TClassFactory_UnitTypeClass/CreateInstance.cpp` | class `UnitTypeClass` | 119 |
| startup | 00694bb0 | `src/recovered/classes/TClassFactory_BuildingTypeClass/CreateInstance.cpp` | class `BuildingTypeClass` | 119 |
| startup | 00695030 | `src/recovered/classes/TClassFactory_BulletTypeClass/CreateInstance.cpp` | class `BulletTypeClass` | 119 |
| startup | 00695630 | `src/recovered/classes/TClassFactory_UnitClass/CreateInstance.cpp` | class `UnitClass` | 121 |
| startup | 00695f30 | `src/recovered/classes/TClassFactory_HouseTypeClass/CreateInstance.cpp` | class `HouseTypeClass` | 119 |
| startup | 006960b0 | `src/recovered/classes/TClassFactory_HouseClass/CreateInstance.cpp` | class `HouseClass` | 119 |
| startup | 006975b0 | `src/recovered/classes/TClassFactory_WarheadTypeClass/CreateInstance.cpp` | class `WarheadTypeClass` | 119 |
| startup | 00697730 | `src/recovered/classes/TClassFactory_WeaponTypeClass/CreateInstance.cpp` | class `WeaponTypeClass` | 119 |
| subtitle | 0069bef0 | `src/recovered/subtitle/unknown_subtitle_parse_color.cpp` | class `SubTitle` | 120 |
| subtitle | 0069bf70 | `src/recovered/subtitle/unknown_subtitle_parse_text.cpp` | class `SubTitle` | 9 |
| tactical | 006a53f0 | `src/recovered/classes/Tactical/Set_Caption_Text.cpp` | class `Tactical` | 66 |
| theme | 006e8150 | `src/recovered/classes/ThemeClass/unknown_theme_length.cpp` | struct `ThemeControl` | 36 |
| theme | 006e8180 | `src/recovered/classes/ThemeClass/Fade_Stream.cpp` | class `ThemeClass` | 21 |
| theme | 006e8270 | `src/recovered/classes/ThemeClass/Suspend.cpp` | class `ThemeClass` | 85 |
| theme | 006e8310 | `src/recovered/classes/ThemeClass/Save.cpp` | class `ThemeClass` | 45 |
| theme | 006e84e0 | `src/recovered/classes/ThemeClass/unknown_is_flagged.cpp` | struct `ThemeControl` | 44 |
| wdtmap | 0072a6d0 | `src/recovered/unowned/UnknownToggle_Set_Unknown_74_0072a6d0.cpp` | class `MSEngine` | 69 |
| wdtmap | 0072a720 | `src/recovered/unowned/UnknownToggle_Set_Unknown_68_0072a720.cpp` | class `UnknownShape_72a6d0` | 69 |
| wonline | 0073c0b0 | `src/recovered/wonline/unknown_can_send_chat.cpp` | class `WideString` | 97 |

30 rows, 2,830 B.

## Lane S1: stand-in declaration cleanup (batch 1, base 705ebac92)

Stand-ins outside codex1's 30, rewritten on the unit's real class and header (the name the
original would have used), then joined with `refile_unit.py join-own UNIT --adopt-gr`. Body
shapes unchanged; no calibration, split declarations or dummies.

| Unit | Rows | Bytes | What |
| --- | ---: | ---: | --- |
| convert | 32 | 1,440 | Blitter `BlitBackward` slot-2 entries: the seed call views (classed dead) became explicit specializations `X<T>::BlitBackward`, the form convert.cpp already has for `BlitTransLucent50ZRead<unsigned short>` (0x00487570); class and slot read from each vtable's RTTI. The 25 native behaviour rewrites in `native/link/behaviour_m22_blit.cpp` are retired. |
| foot | 1 | 56 | `FootClass::Override_Mission` 0x004C7AC0 (TibSun shape). foot.hpp emits header static initializers in a file of its own, so its rename and own-bound join ran as one `apply_rename` batch (renames + `modules` joins; modules.json entry written as join-own writes it). Native rewrite retired. |
| netshare | 1 | 21 | `Seat_First_Player_005c8cd0` on lobby.hpp's session globals (wonline's caller re-declared without the seed namespace). Native rewrite retired. |
| ownrdraw | 2 | 10 | 0x005DE970, 0x005DE980: no change, now join (lane F: rows of the unit differed). |
| mapgen, queue, wonline | 25 | 1,412 | No change, now join (lane F: differed in the batch apply or flipped unit rows). |
| **Total** | **61** | **2,939** | 37 rows / 1,593 B joined under the unit's /GR (noted per `joined` entry). |

Rewritten but not joined:
- ownrdraw 0x005EAD00 (29 B): now `OwnerDraw::unknown_new_edit_proc_5ead00` defined by qualified name
  (own file exact, native rewrite retired); in the unit it flips 0x005FB270 (the same row that
  0x005DEF80, 0x005DF660 and 0x005E0DA0 flip). Natively its population-67 group meets the seed
  0x005C2940's int-typed `GetDlgItem`/`SendMessageA`; a native fixup points that seed at the SDK
  declarations (`native/port/fixups.json`).

Left, with causes (stand-ins after this batch: 139 rows / 15,373 B; 0x005EAD00 now classes as unclassified):
- 41 rows / 2,580 B, a dead vtable view:
  - convert, 20 rows `X<unsigned short>::BlitBackward` (Alpha, ZRead/Warp/ReadWrite Alpha
    families): their headers define `BlitBackward` inline and the classes' destructors and
    vtables are still seed rows in convert.cpp, so the real row is compiler-emitted with the class
    (moves with a convert class sweep, not a declaration cleanup);
  - 21 rows `Append_*` (`DynamicVectorClass<T>::Add` seed views in map, init, foot, mapgen,
    rules, planmgr, scenario, astar, netshare, ccini, uicmnds, wave, houstype, partsys,
    infatype, ahandle2): their real form is a vector_class.hpp member the unit emits (container
    join), not a declaration cleanup.
- 36 rows / 1,659 B: another option set (unchanged).
- wdtsel 0x00730310-0x007303F0, 6 rows / 117 B (stand-ins after codex1 r2015): frozen in another
  population's inventory pass (no own frozen bound for the join).
- ownrdraw 0x005DEFB0, 0x005DF8E0, 0x005E0010, 0x005E0760 (6,830 B): use the stand-in index's
  inline `Find`; the unit's registry is wlib's `WolDictionary` (`Get_Pointer`, another statement
  shape). Joining needs a body change: left.
- 11 rows / 1,003 B flip unit rows when joined (mapgen 00589AC0/00587CF0, ownrdraw 005E5150/005FB270).
- mapgen 0x0056DAC0, 0x0056DBA0, scenario 0x00663220, motlib 0x0059DA40 (C2084): the unit already
  defines the function (an inline copy for its callers); the row is its out-of-line emission.
- uicmnds 0x006F6490: `BeaconManagerClass::Unknown_0042E990` is declared only under
  `UNKNOWN_BEACONS_DECLARE_MEMBERS`, which uicmnds.cpp does not define (another header state).
- uicmnds2 0x006F9FE0: the unit declares the observer with its own placeholder struct in
  uicmnds2.cpp (`UnknownObserver_006f9fb0`); one declaration for both needs a shared header for a
  class not yet identified.
- ownrdraw 0x005EAC10/0x005EAC20 (1 B each) and techno 0x006D98D0: placed inside the unit's
  `namespace` block (ownrdraw) or symbol not found under the unit's state.
- smartdeform 0x00687330/0x00687380: the unit's rows carry `data_bound_by`.
- codex1's 30 placeholder redefinitions (C2011): not touched.

## Lane S1 batch 2: convert class sweep and the Append_* container joins (base 65862f634)

| What | Rows | Bytes |
| --- | ---: | ---: |
| convert: 40 seed scalar deleting destructors (20 `Blit*Alpha`, 18 `RLEBlit*Alpha`, `BlitTransXlat`, `RLEBlitTransXlat`, all `<unsigned short>`) become the compiler-generated `??_G` of each class's inline virtual destructor, emitted with its vtable (unit rows renamed, seed text removed; no join) | 40 | (in the unit already) |
| convert: the 20 `BlitBackward` stand-ins whose headers define the member inline join as the COMDATs convert.cpp emits (DECISIONS "Header inline member the original emits as a COMDAT"; `emitted` note on each `joined` entry) | 20 | 900 |
| `Append_*` seed views as `DynamicVectorClass<T>::Add`, emitted by the unit's own use of its list (container join; join-template's text: the unit without member switches, a description per member; only wave and ahandle2 had switches) | 18 | 1,440 |
| **Joined** | **38** | **2,340** |

29 of the joined rows (1,620 B) are under the unit's /GR. Every `Add<T*>` has the same bytes, so `T` was named from each row's callers inside its unit (the relocation of the original call site in the unit's object), never from the bytes.
The rename and the join went into one `apply_rename` batch per step (renames + `modules` joins, own frozen
bound). This is because the member has no source of its own, so it cannot be made exact in its own file first.

Left (3 dead-view rows):
- mapgen 0x0058EFF0: mapgen emits no `Add`; its callers are in rules.cpp and techtype.cpp (the row
  is held for another unit);
- netshare 0x005CD880: its callers are outside accepted rows (unit's use not provable yet);
- uicmnds 0x006F8B40: the one caller in an accepted row does not relocate to an `Add` (inlined or
  another call form); two candidate instantiations (`TechnoClass *`, `char const *`).

Stand-ins after batch 2: 101 rows / 13,033 B.

## Lane S1 batch 3 and the final stand-in residual (base 60c170b16)

- The link-closure baseline was re-recorded (289 -> 251 undefined, 38 symbols gone, none added).
  Each removed symbol was declared only inside a removed seed view: 37 are
  `seed_0048xxxx::Callee_0041e510::Deinit_0041e510`, the placeholder callee of the blitters'
  seed destructors. The 38th is `Call_4c7ac0_0::target`, the FootClass::Override_Mission view.
- Three stand-ins duplicated a definition their unit already holds, with the same symbol: motlib
  0x0059DA40, mapgen 0x0056DAC0 and 0x0056DBA0 (79 B). The unit's compile reproduces each of them,
  so they join on their own frozen bound and the duplicate file goes. This was lane F's C2084 cause.
  I probed every other remaining stand-in against its unit compiled unchanged; none is emitted.

Stand-ins left (98 rows / 12,954 B). No existing ruling reaches these:

| Cause | Rows | Bytes | Rows / note |
| --- | ---: | ---: | --- |
| C2011 placeholder redefinitions (codex1's list) | 30 | 2,830 | not touched |
| Another option set (/O1 /Ob2 ...) | 23 | 448 | only a /GR difference is approved for joining |
| Another option set (/O2 /Ob2 /G5 /Gy ..., not the unit's) | 13 | 1,211 | same |
| Flips unit rows when joined | 11 | 1,003 | mapgen 0056D5D0/D620/D640/D660/DB40/DBC0/E9D0, 005794D0 (flip 00589AC0/00587CF0); ownrdraw 005DEF80, 005DF660, 005E0DA0 (flip 005E5150/005FB270) |
| Does not compile in the unit | 7 | 6,923 | ownrdraw 005DEFB0, 005DF8E0, 005E0010, 005E0760 (the stand-in index's `Find`; the unit's `WolDictionary` needs a body change); scenario 00663220 (the unit's copy is `inline`, no out-of-line copy is emitted); uicmnds 006F6490 (`UNKNOWN_BEACONS_DECLARE_MEMBERS`); uicmnds2 006F9FE0 (the observer placeholder struct is defined in the unit) |
| Frozen in another population's inventory pass | 6 | 117 | wdtsel 00730310-007303F0 |
| Dead vtable view | 3 | 240 | mapgen 0058EFF0, netshare 005CD880, uicmnds 006F8B40 (see batch 2) |
| Differs under the unit's state | 3 | 36 | ownrdraw 005EAC10/005EAC20 (placed inside the unit's `namespace OwnerDraw` block), techno 006D98D0 |
| The unit's rows carry `data_bound_by` | 2 | 146 | smartdeform 00687330, 00687380 |

Not counted above: ownrdraw 0x005EAD00 (29 B), on its real declaration since batch 1, now
classes as unclassified; it flips 0x005FB270.

## Lane J1: stand-ins frozen under another option set (base 6534e862e)

`refile_unit.py join-own --adopt-options` (DECISIONS "Stand-ins frozen under another optimisation
option than their unit"): a stand-in frozen under another option set joins under the unit's set
(`apply_rename` `"joins_options": "unit"`, note "joined under the unit's options (frozen under X)",
STATUS on the /GR-join line in its own count). Before screening, the unit is compiled unchanged
under each frozen set X; the unit's set counts as evidenced against X only when some of its own
rows then differ (a row whose X has no such evidence stays out). No `#pragma optimize`. Only the
stand-in class adopts options; own-population rows keep the /GR rule.

Result: **0 of the 36 rows (1,659 B) join**. Every unit's set is evidenced except wonline's:

| Unit | Frozen set X | Unit rows that differ under X |
| --- | --- | ---: |
| _map, ahandle, animfile, blowfish, checkbox, combuf, dial8, dib, empulse, gscreen, hsv, statbtn, subtitle, team, techno, tooltip, winfix, winstub, wstring | /O1 /Ob2 /G5 /Gy | all (19, 15, 20, 5, 2, 17, 5, 3, 36, 30, 6, 8, 23, 111, 256, 41, 19, 35, 108) |
| map, planmgr | /O1 /Ob2 /G5 /Gy | 300 of 301, 319 of 320 |
| mapgen | /O1 /Ob2 /G5 /Gy; /O2 /Ob1 /G5 /Gy | 181 of 181; 79 of 181 |
| sidebar | /O2 /Ob2 /G6 /Gy | 59 of 110 |
| txtlabel2 (unit /O2 /Ob2 /G5 /Gy /Gr) | /O2 /Ob2 /G5 /Gy; the same with /GR | 10 of 38 each |
| wonline (unit without /GR) | /O2 /Ob2 /G5 /Gy /GR | 0 of 356: not evidenced, row stays out |

Rows left (causes recorded in modules.json):
- 23 rows / 478 B do not reproduce under the unit's set (3-14 bytes differ): the 22 /O1 stand-ins
  (incl. the eleven 23-byte `Leaf_` zero-fill initialisers, map 0055AA50 and mapgen 0056E9A0) and
  sidebar 006824F0 (/G6). A stand-in that reproduces only
  under /O1 is evidence that its function belongs to another (an /O1) translation unit than the
  unit map says, not a candidate for this unit.
- txtlabel2, 10 rows / 1,035 B: 8 do not compile in the unit (C2011 `UnknownArtData_006f2390`
  placeholder redefinition, C2371 `unknown_color_ac10f4`: codex1's placeholder cleanup);
  006F1A60/006F1AA0 are not emitted under the names the unit gives them.
- mapgen 005797A0 (97 B): C2084, the unit already defines `Map_Random_Range_005797a0`; the unit's
  copy is not the out-of-line row (S1 batch 3 probe).
- tab 006A17B0 (15 B): dead vtable view.
- wonline 00768260 (34 B): the unit's non-/GR set is not evidenced against the row's /GR set.

Seen on the way (not joined, a case for the owner): txtlabel2 has 7 own-population rows
(006F24A0, 006F2980, 006F2A30, 006F2B50, 006F2BD0, 006F4400, 006F4C60; 590 B) frozen under
/O2 /Ob2 /G5 /Gy that screen exact under the unit's /Gr set. The ruling covers stand-ins only.

### Address-named files: what holds the remainder (lane J1, 1,680 at base, 1,678 after)

STATUS splits them into collision / seed name / no spelling / not one accepted row. Finer, at base:

| Cause | Files | Where they sit |
| --- | ---: | --- |
| `$E` initialisers and atexit destructors (`_$E<n>` symbols) | 1,113 | 654 in no unit (mostly `header_statics/<addr>/` groups and the globals population-3698), 357 split residuals, 96 in units not re-filed, 6 flipped. 298 of the split ones are comment-only rows emitted by another file of their population |
| Placeholder class (address in the class name, e.g. `UnknownToggle_0072a6d0`) | 98 | 89 split residuals |
| `Leaf_`/`Append_`/`Storage_` views | 77 | 29 no unit, 23 split, 21 units not re-filed |
| Operators (`??8` ==, `??9` !=, `??4` =, `??H` +, `??G` -, `??B` conversion, `??_I`, `??_H`) | 65 | 47 are `operator==`; no spelling rule for operators |
| Name is only `Unknown`/`unknown` plus an address | 34 | |
| `FUN_` with no source name | 12 | |
| Collision (two rows want one name in one folder) | 250 | 179 split residuals; they clear only when a partner file goes |
| No spelling (template class outside the class folders, or no unit) | 24 | |
| Not one accepted row | 5 | 4 orphan files with no row, 1 file with 339 rows |

What an existing ruling reached:
- The comment-only rows of a unit's own second population now join with their emitter
  (own-population ruling), and the joining rows' `_$E<n>`/`_$S<n>` names take placeholder numbers
  past the unit's, so the re-binding after the compile meets no clash (`joins_renumber`).
- planmgr 0x0061BD30 joined; its collision partner 0x0061AD90 then took its address-free name
  (`Delete.cpp`): 2 files fewer.
- wonline's global-string initializers and destructors (25 rows, 0x0073BCC0-0x0073C450, one
  population per string) reproduce byte for byte in wonline.cpp, but the strict audit then refuses the unit's
  second `.CRT$XCU` section: the initializer pointers are not one contiguous run in the original
  table. They stay. (Corrected by lane X1/E1, docs/CRT_XCU_TABLE.md: this is not evidence they are
  another unit's. The original table puts them inside wonline's own run, interleaved with two
  entries wonline already owns, 0x0073BD20 and 0x0073BFC0, and their data inside wonline's data
  block: they are wonline's, in population-4248. Lane E1 found what keeps the run broken in our
  object: 0x0073BD20 and 0x0073BFC0 are not compiled as dynamic initializers (0x0073BD20 is an /O1
  stand-in in population-08, 0x0073BFC0 a seed function that calls atexit itself). Also,
  wonline.cpp redeclares several of these globals `extern` after the point where their
  definitions go. In worlddom.cpp and wolpersonainformation.cpp, a redeclaration like that makes
  VC6 emit a second initializer for the object.)
- The header-statics groups (audevent, keyboard, lightcon, msglist, netdlg2, revent, theme,
  txtlabel, wdtnet, coopcamp, 12-16 rows each) are not emitted under the unit's state (no `_$E`
  function binds); motlib and mpcoop bind the `unknown_geometry` statics at other addresses.
  worlddom, wolpersonainformation and tactical rows: frozen under another option set, not emitted,
  or their population changes when they leave.

## Lane J1 batch 2 (base 07c6443ee; owner rulings 2026-11-13)

- Own-population rows under another option set (`--adopt-options` now reaches the unit's own second
  population): txtlabel2 0x006F2980, 0x006F2BD0 join under the unit's /Gr set (153 B; note "joined
  under the unit's options (frozen under /O2 /Ob2 /G5 /Gy)"). The other five (0x006F24A0, 0x006F2A30,
  0x006F2B50, 0x006F4400, 0x006F4C60; 437 B) stay: their population-4337 also holds eight
  header-state rows of txtlabel2, and 0x006F4620 differs once they leave. mapgen's three
  own-population option rows do not reproduce under the unit's set; auddev's 0x00402990 differs
  under the unit's state (168 B), so its population's other five stay too.
- wonline under /GR (population-4248, "/GR per population"): not switched. With the unit's text
  re-bound (so its RTTI references enter the table), `??_R0?AVDialogControl@OwnerDraw@@@8` proves;
  `??_R0?AUIUnknown@@@8` does not bind through the original's RTTI graph from wonline's object
  (type finding: a COM interface class of wonline whose hierarchy the original's RTTI does not
  hold). 0x00768260 stays out.
- Operator files: `rename_files.py` names operators in the word form (`OPERATORS`); conversion
  operators (`??B`) and the compiler's array iterators (`??_H`, `??_I`) keep their address. 60 files
  renamed (operator_equal 48, operator_assign 6, operator_plus 3, operator_not_equal 2,
  operator_minus 1); 2 collide in their folder and keep the address.

Address-named files: 1,678 -> 1,615 on main b6e8a3bb8 (seed name 1,335, collision 252, no spelling 23, not one
accepted row 5). The `$E` comment-only split rows and globals population-3698 wait for the review
session.
