# Pool A: matched in src-final, not yet in this tree (priced menu)

Lane O95, 2026-11-25/26. Analysis only: nothing was relaxed or landed. Scratch tests ran in a
worktree and were reverted. Calibration used: no.

**The pool:** src-final's accepted rows whose entry has no row in this tree's catalog.
That is `status.main_only()`: `manifests/original_target` against `manifests/catalog`.

## Reconciling the count

The pool is now **165 rows / 68,738 B**. STATUS says 168 / 77,368 because STATUS was last written at
f4502e4fa9 (2026-11-25 15:52Z). Three rows have landed since then, and STATUS is regenerated only at
the next gated landing:

- Wonline_Main_Dialog_Proc 0x7469C0 (7,172 B)
- SidebarClass::AI 0x67DC90 (1,315 B)
- WolIntArray::Grow 0x768290 (143 B)

Together that is 3 rows / 8,630 B, and 77,368 − 8,630 = 68,738. No row has left the catalog.

## In plain words

Most of the pool is not blocked by a rule. Here is where the 68.7 KB sits:

- **33.3 KB is what the old tree's own extra forms bought:**
  - main's coined helper functions (21.1 KB)
  - per-function header switches or header forms (22.0 KB, mostly Dropship_Screen)
  - copies made for a declaration count (10.4 KB)
  - inline-budget or dead-code tricks (6.7 KB)

  These add up to more than 33.3 KB on purpose. Re-tested in this tree, most of those forms release
  nothing, because the rows stay off even with main's text. The ones that do release something are
  in the table below.
- **6.1 KB is template copies and small helpers (69 rows)** that VC6 emits only beside a parent
  function that is still parked. They land on their own the day their parent lands. The parent that
  brings the most is WDT_Write_Map_Ini: it is 1 byte off and brings 14 rows.
- **2.5 KB is in no unit at all (70 rows).** This is bookkeeping and filing:
  - libcmt code main credited as C
  - header-static initializers of two code-less objects
  - the held `_vanim` unit
  - four tiny rows with coined names
- **0.1 KB has simply not been written yet.**

The options that can be taken with no loss elsewhere come to about 10.3 KB. Most of that is one
row, the WOL game-options dialog (7.9 KB), which needs you to allow one form the 11-24 rules refuse
today. The TechnoTypeClass constructor (3.0 KB) is not free: re-measured now, its header form costs
12.9 KB elsewhere.

## The menu (decide in one sitting)

"Side cost" means exact rows elsewhere that stop matching under the form. "Who" means who may
decide: you, or the owner under an existing precedent.

| # | Relaxation | Rows | Bytes released (net) | Side cost | Structure / flag cost | Tested | Recommendation | Who |
| --- | --- | ---: | ---: | --- | --- | --- | --- | --- |
| R1 | Credit the 12 libcmt rows main took as C (seed_/Leaf_ wrappers over srand, unlink, errno, getc...) to the library ledger instead, and leave them out of the pool line | 12 | 110 (to *linked library*, no C/C++ credit) | none | none | no | **Yes** (bookkeeping) | owner |
| R2 | File the two code-less header-static objects between map and mapgen (0x56CEF0-0x56D2B0) as provisional new units, under the 2026-11-18 filing precedent | 26 | 712 | none expected (new objects only) | +2 provisional units, counted on the structure line | no | **Yes** | owner (review if wanted) |
| R3 | `_vanim`: ra2-decomp's file-scope `const Rect SecondRectNone(0, 0, 0, 0);` in the four objects that build a second zero Rect (_rtti, _vanim, abstract, anim). This creates the new unit `_vanim` | 28 (+2 outside the pool: abstract 0x40FEF0, _rtti 0x40DB40) | 1,294 | none expected (one initializer in each of the 4 objects) | 4 file-scope constants equal to RECT_NONE. Adopted text, so flag iv-a. Not a second name: the image has two objects | no | **Yes**, flagged | you (2026-11-19 ruling parked it: "placeholder constant, no precedent") |
| R4 | Raise the address-suffixed budget (0) by 4 and take main's coined owners: UnknownBuffer::Is_Present_0070a350, Is_Archive_Same_0070a3f0, Pixel_To_RGB_00536ed0, Storage_5cdaf0 | 4 | 340 | none | +4 address-suffixed, +2 coined classes | no | **No**: give them real names instead (about 1 lane-hour of ordinary work) | you |
| R5 | Satellites without their parent: an explicit template instantiation in the unit, so the copies are emitted while the parent stays parked | up to 61 of 69 | ~5,500 | wdtmap test: 10 of 13 exact (+628 B), but the object gains 18 template members the image does not have there. Whole-file link not tried | a line whose only job is to force emission (dummy under the decision test) | partly | **No**: they land free with their parents (G5 below) | you |
| R6 | Main's coined unit-local helpers (main_forms A, 9 rows / 21,078 B) | 2 net | **+7,153** (Fire_At 6,019; StringTable::LoadFile 1,134) | Set_Slot_Names is also exact, but flips wonline OnPrivateGameOptions (−1,440, net −520). The other 6 rows stay 1–1,084 B off even with main's helpers | Fire_At: about 5 coined helpers (Set_Pitch through the out-of-line 0x41B750, Point3F length, the radial velocity and electric bolt written out, a Set_Yaw local). LoadFile: 2 pass-through wrappers (refused by name on 11-24) | O51/O55, 11-22 | **Fire_At: hold** (F66 has it 6 B off with legitimate forms). **LoadFile: no.** Rest: nothing to release | you |
| R7a | TechnoTypeClass::ThreatRange as a one-member class with an inline constructor (main's `Leptons`, a YRpp name) | 1 | **+2,961** | **−9,908 net**: MapClass::Zone_Reset 0x54EBD0 (1,646 B) and queue Print_CRCs 0x62A360 (11,223 B) each go 4 B off (O95 re-measure, below) | +1 class header; 18 use sites read `.Value` | yes (O50/O51 11-22, O95 now) | **No** | you |
| R7b | Unsigned VectorClass index, operator[] (Dropship_Screen) | 1 + 7 satellites | 11,154 gross, **−36,231 net** | 26 exact rows / 47,385 B in 14 units (O68, 11-24) | header form | yes | **No** | you |
| R7c | Key_Hash per-key specialisation (OnFind 2,867; Dialog_On_WM_PAINT 1,281) | 2 | 4,148 | Main's form is an `RA2_` switch, which is forbidden outright. The switch-free generic form turned 20+ wonline rows out of line (F16 probe) | switch / header form | probe only | **No** | — |
| R7d | Is_Listed_Kind through a declared-only second name, or `#pragma auto_inline(off)` (Enable_Animated_Control 2,468; Is_Tested_Control 1,594) | 2 | **0** (still 2,216 and 1,444 B off with the pragma, O50) | — | second name / pragma | yes | **No**: releases nothing | — |
| R8a | Six reference locals with equivalent names (`int & unit_count = Session.Options.UnitCount;` ...) in Wonline_Game_Options_Dialog_Proc | 1 | **+7,899** | none (wonline 417→418 of 435, O50) | a new alias kind: equivalent-name references, which 11-24 refuses. 1 flag | yes | **Yes, flagged**, *if F66's legitimate attempt fails* (F66 is on this row now) | you |
| R8b | ra2-decomp's pass-through `_Move_Coord` in Coord_Scatter | 1 | +289 | none (coord 17/17) | 1 pass-through (it is reference text, so flag iv-a) | yes | **Yes**, as reference text | you |
| R8c | Start_Quick_Match reads arraylist.h's private Count | 1 | **0** (exact only with an unbound probe) | — | header access change | yes | **No** | — |
| R9 | Budget and dummy forms: an extra empty inline site (Search_For_Tiberium_Weighted 1,467), probe declarations in abstract.h (Create_Urban_Area 1,913, count-sensitive), an unread local (Palette_Scheme_Count 592, **you already said no on 11-24**), main's single-caller helpers (Run_Online_Menu 2,687: still 2,391 B off) | 4 | 3,380 possible; 0 for Run_Online_Menu | abstract.h probe: a count window for every includer | dummy declarations or sites, dead code | probes | **No** | — |
| — | Not a rule: no draft yet (TVelocity3D::operator+= 0x4642A0, the rc_ptr<Conflict> deleting dtor 0x727CC0, fly's 0x4BFB30 helper, an empty function 0x6AC7A0) | 4 | 106 | — | — | — | ordinary work | lanes |

**If you take every "Yes":** R1 + R2 + R3 + R8a + R8b release 68 rows / 10,304 B, with no row lost
elsewhere. R1's 110 B go to the library ledger, not to C/C++, so the C/C++ gain is 10,194 B. Of that,
7,899 B depend on R8a, and only if F66's legitimate attempt on that row fails.

### R7a note (O95 re-measure, 2026-11-26)

Earlier records disagree:

- O51 (11-22) measured every includer. Only queue's Print_CRCs 0x62A360 lost one load order.
- O80 (11-24) records Zone_Reset and Print_CRCs flipping.

Print_CRCs has been parked count-sensitive since 11-23, so it no longer counts as a loss. O95 re-ran
the six units that touch the member, before and after the form: techtype, building, map, tactical,
techno and queue. The result is in the line below.

**Result: the form costs more than it releases.** Under the form, with the 18 use sites reading
`.Value`:

- techtype, building, tactical and techno are unchanged.
- map drops from 330 to 329 of 332: MapClass::Zone_Reset 0x54EBD0 (1,646 B) goes 4 B off.
- queue drops from 70 to 69 of 70: Print_CRCs 0x62A360 (11,223 B) goes 4 B off. Print_CRCs is an
  exact catalog row again, so its loss now counts.

The constructor's own guard was not opened. Even if the constructor matched, the net is
2,961 − 12,869 = **−9,908 B**. This confirms O80's record.

Scratch: the worktree lane-o95, reverted. Logs: `o95_lep_{base,new}_<unit>.txt` in the lane
scratchpad.

## Causes, row by row (groups)

| Group | Cause: what blocks src-final's text here | Rows | Bytes |
| --- | --- | ---: | ---: |
| 1 CRT seed | Outside every unit. libcmt code src-final credited as C through `seed_`/`Leaf_` per-function owners. library.json skipped these rows because they overlapped accepted rows | 12 | 110 |
| 2 statics filing | Outside every unit. Header-static `_$E` initializers of two code-less objects after map (mischa85 unk_0056A9D0). src-final filed them under txtlabel, globals and cargo, which are not adjacent units: a filing ruling | 26 | 712 |
| 3 _vanim Rect | Outside every unit. New unit `_vanim` (TibSun file) is held: its object initializes a second zero Rect, and no header in any reference gives exactly the four objects that have one (b_log_r24) | 28 | 1,294 |
| 4 coined owner | Range widening not done. src-final's text is a per-function owner class with an address-suffixed name | 4 | 340 |
| 5 satellite | A template copy or inline helper that VC6 emits only with a parked parent (guardprobe, O95). src-final emitted them through per-function views | 69 | 6,081 |
| 6 main helpers | src-final's coined unit-local helpers, views or wrappers (main_forms A) | 9 | 21,078 |
| 7 header form | A shared header form that src-final had under per-unit switches; under one header per class it flips other rows (main_forms B) | 6 | 21,968 |
| 8 count shape | Locals, aliases or a private-member read chosen for a count window (main_forms C) | 3 | 10,390 |
| 9 budget/dummy | An inline-budget site, probe declarations, an unread local, or coined single-caller helpers | 4 | 6,659 |
| 10 no draft | No text yet; no rule involved | 4 | 106 |
| **Total** | | **165** | **68,738** |

None of the 165 rows is blocked by an `RA2_` switch in this tree, or by a duplicate class definition.
Those were src-final's means. Here they appear as groups 4–7.

### Group 5: parents that bring the satellites (guardprobe, O95)

When each parent below lands, it brings its satellite rows with it.

| Parent (parked) | Off now | Satellite rows | Bytes |
| --- | ---: | ---: | ---: |
| WDT_Write_Map_Ini (wdtmap 0x72A880) | 1 B | 14 | 716 |
| Scenario_DlgProc (netshare 0x5C69E0): MultiMission vector copies, ctor and dtors | 1,219 B | 14 | 968 |
| Control_Proc (ownrdraw 0x5EE460): Dictionary<MessageKey> members, ArrayList<HWND> grow | 4,863 B | 5 | 1,103 |
| List_Proc (ownrdraw): WolArrayList grow/shrink/clear, Leaf_601410 | 13,791 B | 6 | 630 |
| Projectile_Motion (missile 0x593870): DirType operators, Set_Pitch | 179 B | 6 | 566 |
| MapGeneratorClass::Read_Settings (mapgen 0x579960): TypeList / DynamicVectorClass copies | 127 B | 4 | 434 |
| Dropship_Screen (dropship 0x4A6B90) | 4,453 B | 7 | 357 |
| CChatEventSink::OnChannelJoin (wonline): ArrayList<Channel>/<User> shrink and grow | 17 B | 2 | 298 |
| Run_Online_Menu (wonline 0x759370): Dictionary getPointer, Key_Hash | 2,579 B | 2 | 139 |
| Combo_Proc (ownrdraw) | 6,538 B | 1 | 138 |
| TechnoClass::Draw_Post_Render (techno 0x6C2C40) | 3,138 B | 1 | 77 |
| MapClass::Set_Map_Dimensions (map 0x5484E0) | 3,665 B | 1 | 39 |
| StripClass::Recalc (sidebar 0x6802D0) | 1,660 B | 1 | 29 |
| No single parent opens them (two ownrdraw atexit destructors of statics, WolArrayList<int>::Grow_Vector, TPoint2D<short>::operator+=, WolNameTable::Get_Pointer) | — | 5 | 587 |

## Rows F66 closed while this ran

None so far. At hand-back (main at 66bccbbfdd), F66 had one uncommitted change in its worktree: it
was working on Wonline_Game_Options_Dialog_Proc 0x74A710, with the guard opened for testing. That row
is the one R8a would release. No pool row has landed during this lane: the pool is still
165 / 68,738. Read F66's result before deciding R8a.

## The .text bytes in no inventoried function (second task)

**Where the "about 50 KB" comes from.** It is the difference between three STATUS lines: unreproduced
.text 213.3 KB − parked 84.1 KB − pool 77.4 KB = 51.8 KB. Those lines overlap, so the subtraction
misleads. 94 of the 99 parked rows (64.4 KB) are pool rows.

Measured byte by byte on the image now (.text 3,769,805 B), the **204,635 B not reproduced** split
like this:

| Where the bytes are | Bytes | What they are |
| --- | ---: | --- |
| Rows of `units.json` not in the catalog | 78,128 | parked rows: 94 pool + 5 other = 75,488 body; count-sensitive Create_Urban_Area 1,913; their padding |
| src-final rows outside every unit | 2,990 | groups 1–3 above plus the four widening rows (2,456 body + padding) |
| **Parked drafts with no row** | **122,372** | 31 functions src-final carried as non-exact (92,594 B: BulletClass::AI, BuildingClass::Exit_Object, IsometricTileTypeClass::Draw_Tile, Decode_Pub_Gameopt, Control_Proc, DSurface::Draw_Depth_Antialiased_Line, Set_Map_Dimensions ...), plus 7 image-derived bounds with no src-final row (29,348 B: Combo_Proc + List_Proc 21,520, Static_Proc 4,512, Start_Talking_To 2,464, a cell bound 688, a convert bound 96, a sidebar bound 64, a 4 B bound), plus 430 B of tails where the image bound runs past src-final's size |
| **In no inventory at all** | **1,145** | 845 B int3 alignment fill in 112 runs (460 B among game code, 385 B in the CRT); 198 B of 33 linker import thunks (`jmp [IAT]` at 0x781B66-0x781C8B) that library.json does not list; about 80 B of CRT code in 7 fragments (0x782146, 0x782196, 0x7829CB, 0x7841EC, 0x7879F2, 0x794AD8 ...); 20 B lea-nop alignment |

What this means:

- No real game code lies outside the inventory. Nothing there is library or handwritten assembly
  that has been overlooked.
- The 122 KB of parked drafts with no row is counted on **neither** STATUS line. The census counts
  only `units.json` rows ("Functions inside a unit's range with no row at all are not parked rows",
  docs/REBUILD.md). So the parked line understates the tail by 122 KB / 38 functions.
- Suggested measure fix (a marked series break): count these drafts on the parked line, and list the
  33 thunks and the CRT fragments in the library ledger.

## Appendix: every row

Groups as above. For satellites, the last column names the parent function. src-final source paths
are relative to `src/recovered/` at the tag.

| Entry | Bytes | Function (src-final symbol, demangled) | src-final source | Group | Parent / note |
| --- | ---: | --- | --- | --- | --- |
| 0x792AD4 | 17 | `seed_00792ad4::unknown_set_exception_filter_00792ad4` | libcmt/unknown_set_exception_filter.cpp | 1 CRT seed |  |
| 0x78B1E7 | 13 | `Leaf_78b1e7::get` | unidentified/seed_views/exception_0078b1e7.cpp | 1 CRT seed |  |
| 0x78469D | 13 | `seed_0078469d::srand_0078469d` | libcmt/srand.cpp | 1 CRT seed |  |
| 0x781D3B | 11 | `seed_00781d3b::operator_delete_00781d3b` | libcmt/operator_delete.cpp | 1 CRT seed |  |
| 0x784A3C | 11 | `seed_00784a3c::Unknown_getc_00784a3c` | libcmt/Unknown_getc.cpp | 1 CRT seed |  |
| 0x7856A2 | 11 | `seed_007856a2::unlink_007856a2` | libcmt/unlink.cpp | 1 CRT seed |  |
| 0x78991E | 9 | `seed_0078991e::errno_0078991e` | libcmt/errno.cpp | 1 CRT seed |  |
| 0x789927 | 9 | `seed_00789927::doserrno_00789927` | libcmt/doserrno.cpp | 1 CRT seed |  |
| 0x7905E2 | 9 | `seed_007905e2::fptrap_007905e2` | libcmt/fptrap.cpp | 1 CRT seed |  |
| 0x78F3F5 | 3 | `Leaf_78f3f5::get` | unidentified/seed_views/0078f3f5.cpp | 1 CRT seed |  |
| 0x793976 | 3 | `Leaf_793976::get` | unidentified/seed_views/00793976.cpp | 1 CRT seed |  |
| 0x78215D | 1 | `Leaf_78215d::get` | unidentified/seed_views/0078215d.cpp | 1 CRT seed |  |
| 0x56CFE0 | 49 | `_$E23` | header_statics/after_map/HeaderStatics_g_init.cpp | 2 statics filing |  |
| 0x56D1D0 | 49 | `_$E20` | header_statics/before_mapgen/HeaderStatics_g_init.cpp | 2 statics filing |  |
| 0x56D050 | 44 | `_$E29` | header_statics/after_map/HeaderStatics_i_init.cpp | 2 statics filing |  |
| 0x56D240 | 44 | `_$E26` | header_statics/before_mapgen/HeaderStatics_i_init.cpp | 2 statics filing |  |
| 0x56CF10 | 40 | `_$E5` | header_statics/after_map/HeaderStatics_diagonal_init.cpp | 2 statics filing |  |
| 0x56D100 | 40 | `_$E2` | header_statics/before_mapgen/HeaderStatics_diagonal_init.cpp | 2 statics filing |  |
| 0x56D080 | 39 | `_$E32` | header_statics/after_map/HeaderStatics_j_init.cpp | 2 statics filing |  |
| 0x56D270 | 39 | `_$E29` | header_statics/before_mapgen/HeaderStatics_j_init.cpp | 2 statics filing |  |
| 0x56D020 | 33 | `_$E26` | header_statics/after_map/HeaderStatics_h_init.cpp | 2 statics filing |  |
| 0x56D210 | 33 | `_$E23` | header_statics/before_mapgen/HeaderStatics_h_init.cpp | 2 statics filing |  |
| 0x56CEF0 | 23 | `_$E2` | header_statics/after_map/HeaderStatics_empty_rect_init.cpp | 2 statics filing |  |
| 0x56D0E0 | 23 | `_$E41` | header_statics/after_map/HeaderStatics_static_rect_init.cpp | 2 statics filing |  |
| 0x56CF40 | 19 | `_$E8` | header_statics/after_map/HeaderStatics_b_init.cpp | 2 statics filing |  |
| 0x56CF60 | 19 | `_$E11` | header_statics/after_map/HeaderStatics_c_init.cpp | 2 statics filing |  |
| 0x56CF80 | 19 | `_$E14` | header_statics/after_map/HeaderStatics_angle45_init.cpp | 2 statics filing |  |
| 0x56CFA0 | 19 | `_$E17` | header_statics/after_map/HeaderStatics_angle60_init.cpp | 2 statics filing |  |
| 0x56CFC0 | 19 | `_$E20` | header_statics/after_map/HeaderStatics_angle90_init.cpp | 2 statics filing |  |
| 0x56D130 | 19 | `_$E5` | header_statics/before_mapgen/HeaderStatics_b_init.cpp | 2 statics filing |  |
| 0x56D150 | 19 | `_$E8` | header_statics/before_mapgen/HeaderStatics_c_init.cpp | 2 statics filing |  |
| 0x56D170 | 19 | `_$E11` | header_statics/before_mapgen/HeaderStatics_angle45_init.cpp | 2 statics filing |  |
| 0x56D190 | 19 | `_$E14` | header_statics/before_mapgen/HeaderStatics_angle60_init.cpp | 2 statics filing |  |
| 0x56D1B0 | 19 | `_$E17` | header_statics/before_mapgen/HeaderStatics_angle90_init.cpp | 2 statics filing |  |
| 0x56D0C0 | 18 | `_$E38` | header_statics/after_map/HeaderStatics_l_init.cpp | 2 statics filing |  |
| 0x56D2B0 | 18 | `_$E35` | header_statics/before_mapgen/HeaderStatics_l_init.cpp | 2 statics filing |  |
| 0x56D0B0 | 15 | `_$E35` | header_statics/after_map/HeaderStatics_k_init.cpp | 2 statics filing |  |
| 0x56D2A0 | 15 | `_$E32` | header_statics/before_mapgen/HeaderStatics_k_init.cpp | 2 statics filing |  |
| 0x40EEA0 | 176 | `?Resize@?$DynamicVectorClass@PAVVoxelAnimClass@@@@UAE_NHPBQAVVoxelA...` | classes/DynamicVectorClass_VoxelAnimClass_ptr/Resize.cpp | 3 _vanim Rect |  |
| 0x40ED60 | 174 | `?Resize@?$VectorClass@PAVVoxelAnimClass@@@@UAE_NHPBQAVVoxelAnimClas...` | classes/VectorClass_VoxelAnimClass_ptr/Resize.cpp | 3 _vanim Rect |  |
| 0x40EF80 | 73 | `Leaf_40ef80::get` | awaiting_join/deleting_destructors/VectorClass_VoxelAnimClass_ptr_0040ef80.cpp | 3 _vanim Rect |  |
| 0x40EFD0 | 73 | `Leaf_40efd0::get` | awaiting_join/deleting_destructors/DynamicVectorClass_VoxelAnimClass_ptr_0040efd0.cpp | 3 _vanim Rect |  |
| 0x40ED20 | 62 | `??8?$VectorClass@PAVVoxelAnimClass@@@@UBE_NABV0@@Z` | classes/VectorClass_VoxelAnimClass_ptr/operator_equal.cpp | 3 _vanim Rect |  |
| 0x40EC40 | 61 | `_$E46` | classes/VoxelAnimClass/VoxelAnimClass_list_init.cpp | 3 _vanim Rect |  |
| 0x40EC80 | 59 | `_$E44` | classes/VoxelAnimClass/VoxelAnimClass_list_atexit_destructor.cpp | 3 _vanim Rect |  |
| 0x40EE40 | 51 | `?ID@?$VectorClass@PAVVoxelAnimClass@@@@UAEHABQAVVoxelAnimClass@@@Z` | classes/VectorClass_VoxelAnimClass_ptr/ID_object.cpp | 3 _vanim Rect |  |
| 0x40EB20 | 49 | `_$E23` | header_statics/_voxel/HeaderStatics_g_init.cpp | 3 _vanim Rect |  |
| 0x40EB90 | 44 | `_$E29` | header_statics/_voxel/HeaderStatics_i_init.cpp | 3 _vanim Rect |  |
| 0x40ECC0 | 42 | `?Clear@?$DynamicVectorClass@PAVVoxelAnimClass@@@@UAEXXZ` | classes/DynamicVectorClass_VoxelAnimClass_ptr/Clear.cpp | 3 _vanim Rect |  |
| 0x40EA50 | 40 | `_$E5` | header_statics/_voxel/HeaderStatics_diagonal_init.cpp | 3 _vanim Rect |  |
| 0x40EBC0 | 39 | `_$E32` | header_statics/_voxel/HeaderStatics_j_init.cpp | 3 _vanim Rect |  |
| 0x40EE10 | 39 | `?Clear@?$VectorClass@PAVVoxelAnimClass@@@@UAEXXZ` | classes/VectorClass_VoxelAnimClass_ptr/Clear.cpp | 3 _vanim Rect |  |
| 0x40EF50 | 38 | `?ID@?$DynamicVectorClass@PAVVoxelAnimClass@@@@UAEHABQAVVoxelAnimCla...` | classes/DynamicVectorClass_VoxelAnimClass_ptr/ID_object.cpp | 3 _vanim Rect |  |
| 0x40EB60 | 33 | `_$E26` | header_statics/_voxel/HeaderStatics_h_init.cpp | 3 _vanim Rect |  |
| 0x40ECF0 | 27 | `?ID@?$DynamicVectorClass@PAVVoxelAnimClass@@@@UAEHPBQAVVoxelAnimCla...` | classes/DynamicVectorClass_VoxelAnimClass_ptr/ID_pointer.cpp | 3 _vanim Rect |  |
| 0x40EE80 | 27 | `?ID@?$VectorClass@PAVVoxelAnimClass@@@@UAEHPBQAVVoxelAnimClass@@@Z` | classes/VectorClass_VoxelAnimClass_ptr/ID_pointer.cpp | 3 _vanim Rect |  |
| 0x40EA30 | 23 | `_$E2` | header_statics/_voxel/HeaderStatics_empty_rect_init.cpp | 3 _vanim Rect |  |
| 0x40EC20 | 23 | `_$E41` | header_statics/_voxel/HeaderStatics_static_rect_init.cpp | 3 _vanim Rect |  |
| 0x40EA80 | 19 | `_$E8` | header_statics/_voxel/HeaderStatics_b_init.cpp | 3 _vanim Rect |  |
| 0x40EAA0 | 19 | `_$E11` | header_statics/_voxel/HeaderStatics_c_init.cpp | 3 _vanim Rect |  |
| 0x40EAC0 | 19 | `_$E14` | header_statics/_voxel/HeaderStatics_angle45_init.cpp | 3 _vanim Rect |  |
| 0x40EAE0 | 19 | `_$E17` | header_statics/_voxel/HeaderStatics_angle60_init.cpp | 3 _vanim Rect |  |
| 0x40EB00 | 19 | `_$E20` | header_statics/_voxel/HeaderStatics_angle90_init.cpp | 3 _vanim Rect |  |
| 0x40EC00 | 18 | `_$E38` | header_statics/_voxel/HeaderStatics_l_init.cpp | 3 _vanim Rect |  |
| 0x40EBF0 | 15 | `_$E35` | header_statics/_voxel/HeaderStatics_k_init.cpp | 3 _vanim Rect |  |
| 0x40ED10 | 13 | `?Fetch@?$VectorClass@PAVVoxelAnimClass@@@@UBEPAVVoxelAnimClass@@H@Z` | classes/VectorClass_VoxelAnimClass_ptr/Fetch.cpp | 3 _vanim Rect |  |
| 0x70A350 | 151 | `UnknownBuffer::Is_Present_0070a350` | unit.cpp | 4 coined owner |  |
| 0x536ED0 | 88 | `Pixel_To_RGB_00536ed0` | unowned/Pixel_To_RGB.cpp | 4 coined owner |  |
| 0x5CDAF0 | 74 | `Storage_5cdaf0::get` | netshare.cpp | 4 coined owner |  |
| 0x70A3F0 | 27 | `UnknownBuffer::Is_Archive_Same_0070a3f0` | unit.cpp | 4 coined owner |  |
| 0x602040 | 607 | `?Remove_Entry@?$WolDictionary@UUnknownPairKey@OwnerDraw@@_N@@QAE_NA...` | ownrdraw.cpp | 5 satellite | Control_Proc (ownrdraw 0x5EE460, 4,863 B off) |
| 0x5941B0 | 342 | `?Set_Pitch@?$TVelocity3D@N@@QAEXABVDirType@@@Z` | missile.cpp | 5 satellite | Projectile_Motion (missile 0x593870, 179 B off) |
| 0x5F00E0 | 291 | `unknown_active_messages_atexit_5f00e0` | ownrdraw.cpp | 5 satellite | base missing (no parent alone) |
| 0x72B720 | 249 | `?Resize@?$VectorClass@UCentroid@WorldDominationTour@@@@UAE_NHPBUCen...` | wdtmap.cpp | 5 satellite | WDT_Write_Map_Ini (wdtmap 0x72A880, 1 B off) |
| 0x5CD7D0 | 176 | `?Resize@?$DynamicVectorClass@PAVMultiMission@@@@UAE_NHPBQAVMultiMis...` | unidentified/multimission_instances/DynamicVectorClass_MultiMission_ptr_Resize_005cd7d0.cpp | 5 satellite | Scenario_DlgProc (netshare 0x5C69E0, 1,219 B off) |
| 0x5CD630 | 174 | `?Resize@?$VectorClass@PAVMultiMission@@@@UAE_NHPBQAVMultiMission@@@Z` | unidentified/multimission_instances/VectorClass_MultiMission_ptr_Resize_005cd630.cpp | 5 satellite | Scenario_DlgProc (netshare 0x5C69E0, 1,219 B off) |
| 0x602890 | 168 | `?Shrink_Table@?$WolDictionary@UUnknownPairKey@OwnerDraw@@H@@QAEXXZ` | ownrdraw.cpp | 5 satellite | Control_Proc (ownrdraw 0x5EE460, 4,863 B off) |
| 0x6022A0 | 156 | `?Grow_Table@?$WolDictionary@UUnknownPairKey@OwnerDraw@@H@@QAEXXZ` | ownrdraw.cpp | 5 satellite | Control_Proc (ownrdraw 0x5EE460, 4,863 B off) |
| 0x766DC0 | 150 | `WolChannelArray::Shrink_00766dc0` | wonline.cpp | 5 satellite | CChatEventSink::OnChannelJoin (wonline, 17 B off) |
| 0x766E60 | 148 | `WolUserArray::Grow_00766e60` | wonline/WolUserArray_Grow.cpp | 5 satellite | CChatEventSink::OnChannelJoin (wonline, 17 B off) |
| 0x602360 | 143 | `?Grow_Vector@?$WolArrayList@H@@QAE_NXZ` | ownrdraw.cpp | 5 satellite | base missing (no parent alone) |
| 0x602940 | 143 | `?Grow_Vector@?$WolArrayList@PAUHWND__@@@@QAE_NXZ` | ownrdraw.cpp | 5 satellite | Control_Proc (ownrdraw 0x5EE460, 4,863 B off) |
| 0x602510 | 138 | `UnknownListOwner_00602510::Release_00602510` | awaiting_join/nested_class_members/UnknownListOwner_Release_00602510.cpp | 5 satellite | Combo_Proc (ownrdraw 0x5F2B50.., 6,538 B off) |
| 0x58EED0 | 136 | `??0?$TypeList@PAVBuildingTypeClass@@@@QAE@ABV0@@Z` | classes/TypeList_BuildingTypeClass_ptr/Copy_Constructor.cpp | 5 satellite | MapGeneratorClass::Read_Settings (mapgen 0x579960, 127 B off) |
| 0x601850 | 136 | `?Shrink_Vector@?$WolArrayList@UUnknownListEntry_6017c0@@@@QAE_NXZ` | ownrdraw.cpp | 5 satellite | List_Proc (ownrdraw, 13,791 B off) |
| 0x602480 | 136 | `?Shrink_Vector@?$WolArrayList@UUnknownListEntry_602480@@@@QAE_NXZ` | ownrdraw.cpp | 5 satellite | List_Proc (ownrdraw, 13,791 B off) |
| 0x6029D0 | 135 | `?Grow_Vector@?$WolArrayList@UUnknownListEntry_602480@@@@QAE_NXZ` | ownrdraw.cpp | 5 satellite | List_Proc (ownrdraw, 13,791 B off) |
| 0x6023F0 | 134 | `?Shrink_Vector@?$WolArrayList@H@@QAE_NXZ` | ownrdraw.cpp | 5 satellite | List_Proc (ownrdraw, 13,791 B off) |
| 0x58DB80 | 132 | `??4?$DynamicVectorClass@H@@QAEAAV0@ABV0@@Z` | classes/DynamicVectorClass_int/operator_assign.cpp | 5 satellite | MapGeneratorClass::Read_Settings (mapgen 0x579960, 127 B off) |
| 0x58DCA0 | 132 | `??4?$DynamicVectorClass@PAVBuildingTypeClass@@@@QAEAAV0@ABV0@@Z` | classes/DynamicVectorClass_BuildingTypeClass_ptr/operator_assign.cpp | 5 satellite | MapGeneratorClass::Read_Settings (mapgen 0x579960, 127 B off) |
| 0x767CF0 | 110 | `WolChannelDictionary::Get_Pointer_00767cf0` | classes/WolChannelDictionary/Get_Pointer.cpp | 5 satellite | Run_Online_Menu (wonline 0x759370, 2,579 B off) |
| 0x767BB0 | 107 | `WolNameTable::Get_Pointer_00767bb0` | classes/WolNameTable/Get_Pointer.cpp | 5 satellite | base missing (no parent alone) |
| 0x594140 | 100 | `DirType::Turn` | missile.cpp | 5 satellite | Projectile_Motion (missile 0x593870, 179 B off) |
| 0x5CD770 | 93 | `WordStorage_005cd770::initialize` | netshare.cpp | 5 satellite | Scenario_DlgProc (netshare 0x5C69E0, 1,219 B off): vector ctor/deleting dtor, also needs the vtable bound |
| 0x72B6C0 | 92 | `??8?$VectorClass@UCentroid@WorldDominationTour@@@@UBE_NABV0@@Z` | classes/VectorClass_WorldDominationTour__Centroid/operator_equal.cpp | 5 satellite | WDT_Write_Map_Ini (wdtmap 0x72A880, 1 B off) |
| 0x5CD880 | 80 | `Append_5cd880::get` | unidentified/seed_views/005cd880.cpp | 5 satellite | Scenario_DlgProc (netshare 0x5C69E0, 1,219 B off) |
| 0x72B850 | 80 | `?ID@?$VectorClass@UCentroid@WorldDominationTour@@@@UAEHABUCentroid@...` | classes/VectorClass_WorldDominationTour__Centroid/ID_object.cpp | 5 satellite | WDT_Write_Map_Ini (wdtmap 0x72A880, 1 B off) |
| 0x6D9770 | 77 | `??KCoordStruct@@QBE?AU0@H@Z` | techno.cpp | 5 satellite | TechnoClass::Draw_Post_Render (techno 0x6C2C40, 3,138 B off) |
| 0x4AA080 | 73 | `??_G?$DynamicVectorClass@PAVTechnoTypeClass@@@@UAEPAXI@Z` | dropship.cpp | 5 satellite | Dropship_Screen (dropship 0x4A6B90, 4,453 B off) |
| 0x4AA120 | 73 | `??_G?$DynamicVectorClass@PAUButtonFadeEffect@@@@UAEPAXI@Z` | dropship.cpp | 5 satellite | Dropship_Screen (dropship 0x4A6B90, 4,453 B off) |
| 0x4AA1C0 | 73 | `??_G?$DynamicVectorClass@PAUCrossDissolveEffect@@@@UAEPAXI@Z` | dropship.cpp | 5 satellite | Dropship_Screen (dropship 0x4A6B90, 4,453 B off) |
| 0x5CD900 | 73 | `Leaf_5cd900::get` | netshare.cpp | 5 satellite | Scenario_DlgProc (netshare 0x5C69E0, 1,219 B off): vector ctor/deleting dtor, also needs the vtable bound |
| 0x5CD950 | 73 | `Leaf_5cd950::get` | netshare.cpp | 5 satellite | Scenario_DlgProc (netshare 0x5C69E0, 1,219 B off): vector ctor/deleting dtor, also needs the vtable bound |
| 0x72BAA0 | 73 | `??_G?$VectorClass@UCentroid@WorldDominationTour@@@@UAEPAXI@Z` | wdtmap.cpp | 5 satellite | WDT_Write_Map_Ini (wdtmap 0x72A880, 1 B off): deleting dtor, vtable reference unbound |
| 0x601490 | 68 | `?Clear_Vector@?$WolArrayList@UUnknownListEntry_6017c0@@@@QAEXXZ` | ownrdraw.cpp | 5 satellite | List_Proc (ownrdraw, 13,791 B off) |
| 0x5CD5F0 | 62 | `??8?$VectorClass@PAVMultiMission@@@@UBE_NABV0@@Z` | unidentified/multimission_instances/VectorClass_MultiMission_ptr_operator_equal_005cd5f0.cpp | 5 satellite | Scenario_DlgProc (netshare 0x5C69E0, 1,219 B off) |
| 0x5CD710 | 51 | `?ID@?$VectorClass@PAVMultiMission@@@@UAEHABQAVMultiMission@@@Z` | unidentified/multimission_instances/VectorClass_MultiMission_ptr_ID_object_005cd710.cpp | 5 satellite | Scenario_DlgProc (netshare 0x5C69E0, 1,219 B off) |
| 0x4A9B00 | 42 | `Cleanup_4a9b00::get` | dropship.cpp | 5 satellite | Dropship_Screen (dropship 0x4A6B90, 4,453 B off) |
| 0x4A9B60 | 42 | `Cleanup_4a9b60::get` | dropship.cpp | 5 satellite | Dropship_Screen (dropship 0x4A6B90, 4,453 B off) |
| 0x5CD590 | 42 | `?Clear@?$DynamicVectorClass@PAVMultiMission@@@@UAEXXZ` | classes/DynamicVectorClass_MultiMission_ptr/Clear.cpp | 5 satellite | Scenario_DlgProc (netshare 0x5C69E0, 1,219 B off) |
| 0x594110 | 42 | `DirType::Is_Complete_Turn` | missile.cpp | 5 satellite | Projectile_Motion (missile 0x593870, 179 B off) |
| 0x72B060 | 40 | `?Fetch@?$VectorClass@UCentroid@WorldDominationTour@@@@UBE?AUCentroi...` | classes/VectorClass_WorldDominationTour__Centroid/Fetch.cpp | 5 satellite | WDT_Write_Map_Ini (wdtmap 0x72A880, 1 B off) |
| 0x5CD6E0 | 39 | `?Clear@?$VectorClass@PAVMultiMission@@@@UAEXXZ` | unidentified/multimission_instances/VectorClass_MultiMission_ptr_Clear_005cd6e0.cpp | 5 satellite | Scenario_DlgProc (netshare 0x5C69E0, 1,219 B off) |
| 0x72B820 | 39 | `?Clear@?$VectorClass@UCentroid@WorldDominationTour@@@@UAEXXZ` | wdtmap.cpp | 5 satellite | WDT_Write_Map_Ini (wdtmap 0x72A880, 1 B off) |
| 0x56A9F0 | 39 | `??4AbstractClass@@QAEAAV0@ABV0@@Z` | map.cpp | 5 satellite | MapClass::Set_Map_Dimensions (map 0x5484E0; opening also loses 3 rows) |
| 0x5CD8D0 | 38 | `?ID@?$DynamicVectorClass@PAVMultiMission@@@@UAEHABQAVMultiMission@@@Z` | classes/DynamicVectorClass_MultiMission_ptr/ID_object.cpp | 5 satellite | Scenario_DlgProc (netshare 0x5C69E0, 1,219 B off) |
| 0x72B8A0 | 36 | `?ID@?$VectorClass@UCentroid@WorldDominationTour@@@@UAEHPBUCentroid@...` | classes/VectorClass_WorldDominationTour__Centroid/ID_pointer.cpp | 5 satellite | WDT_Write_Map_Ini (wdtmap 0x72A880, 1 B off) |
| 0x58DC70 | 34 | `??0?$TypeList@PAVBuildingTypeClass@@@@QAE@XZ` | classes/TypeList_BuildingTypeClass_ptr/Default_Constructor.cpp | 5 satellite | MapGeneratorClass::Read_Settings (mapgen 0x579960, 127 B off) |
| 0x72B0F0 | 32 | `??_G?$VectorCursor@V?$TRect@H@@V?$VectorClass@V?$TRect@H@@@@@@UAEPA...` | wdtmap.cpp | 5 satellite | WDT_Write_Map_Ini (wdtmap 0x72A880, 1 B off) |
| 0x682520 | 29 | `Leaf_682520::get` | sidebar.cpp | 5 satellite | StripClass::Recalc (sidebar 0x6802D0, 1,660 B off) |
| 0x767CD0 | 29 | `WolNameTable::Key_Hash_00767cd0` | classes/WolNameTable/Key_Hash.cpp | 5 satellite | Run_Online_Menu (wonline 0x759370, 2,579 B off) |
| 0x602340 | 29 | `?Key_Hash@?$WolDictionary@UUnknownPairKey@OwnerDraw@@_N@@QBEIABUUnk...` | ownrdraw.cpp | 5 satellite | Control_Proc (ownrdraw 0x5EE460, 4,863 B off) |
| 0x5940B0 | 28 | `??HDirType@@QBE?AV0@ABV0@@Z` | missile.cpp | 5 satellite | Projectile_Motion (missile 0x593870, 179 B off) or another missile caller |
| 0x5940D0 | 28 | `??GDirType@@QBE?AV0@ABV0@@Z` | missile.cpp | 5 satellite | Projectile_Motion (missile 0x593870, 179 B off) |
| 0x4A9B30 | 27 | `?ID@?$DynamicVectorClass@PAUButtonFadeEffect@@@@UAEHPBQAUButtonFade...` | classes/DynamicVectorClass_ButtonFadeEffect_ptr/ID_pointer.cpp | 5 satellite | Dropship_Screen (dropship 0x4A6B90, 4,453 B off) |
| 0x4A9B90 | 27 | `?ID@?$DynamicVectorClass@PAUCrossDissolveEffect@@@@UAEHPBQAUCrossDi...` | classes/DynamicVectorClass_CrossDissolveEffect_ptr/ID_pointer.cpp | 5 satellite | Dropship_Screen (dropship 0x4A6B90, 4,453 B off) |
| 0x5CD5C0 | 27 | `?ID@?$DynamicVectorClass@PAVMultiMission@@@@UAEHPBQAVMultiMission@@@Z` | classes/DynamicVectorClass_MultiMission_ptr/ID_pointer.cpp | 5 satellite | Scenario_DlgProc (netshare 0x5C69E0, 1,219 B off) |
| 0x5CD750 | 27 | `?ID@?$VectorClass@PAVMultiMission@@@@UAEHPBQAVMultiMission@@@Z` | unidentified/multimission_instances/VectorClass_MultiMission_ptr_ID_pointer_005cd750.cpp | 5 satellite | Scenario_DlgProc (netshare 0x5C69E0, 1,219 B off) |
| 0x5940F0 | 26 | `??KDirType@@QBE?AV0@H@Z` | missile.cpp | 5 satellite | Projectile_Motion (missile 0x593870, 179 B off) |
| 0x72B090 | 25 | `??0?$VectorCursor@V?$TRect@H@@V?$VectorClass@V?$TRect@H@@@@@@QAE@AA...` | classes/VectorCursor_TRect_int_VectorClass_TRect_int/Constructor.cpp | 5 satellite | WDT_Write_Map_Ini (wdtmap 0x72A880, 1 B off) |
| 0x56AA20 | 23 | `??Y?$TPoint2D@F@@QAEABV0@ABV0@@Z` | map.cpp | 5 satellite | base missing (no parent alone) |
| 0x5F00C0 | 23 | `unknown_modal_list_atexit_5f00c0` | ownrdraw.cpp | 5 satellite | base missing (no parent alone) |
| 0x601410 | 21 | `Leaf_601410::get` | ownrdraw.cpp | 5 satellite | List_Proc (ownrdraw, 13,791 B off) |
| 0x72AF50 | 21 | `??4?$VectorCursor@V?$TRect@H@@V?$VectorClass@V?$TRect@H@@@@@@QAEAAV...` | classes/VectorCursor_TRect_int_VectorClass_TRect_int/operator_assign.cpp | 5 satellite | WDT_Write_Map_Ini (wdtmap 0x72A880, 1 B off) |
| 0x72B0B0 | 14 | `seed_0072b0b0::unknown_in_range_0072b0b0` | wdtmap.cpp | 5 satellite | WDT_Write_Map_Ini (wdtmap 0x72A880, 1 B off) |
| 0x5CD5E0 | 13 | `?Fetch@?$VectorClass@PAVMultiMission@@@@UBEPAVMultiMission@@H@Z` | unidentified/multimission_instances/VectorClass_MultiMission_ptr_Fetch_005cd5e0.cpp | 5 satellite | Scenario_DlgProc (netshare 0x5C69E0, 1,219 B off) |
| 0x72B8D0 | 6 | `Leaf_72b8d0::get` | wdtmap.cpp | 5 satellite | WDT_Write_Map_Ini (wdtmap 0x72A880, 1 B off) |
| 0x72B8E0 | 6 | `Leaf_72b8e0::get` | wdtmap.cpp | 5 satellite | WDT_Write_Map_Ini (wdtmap 0x72A880, 1 B off) |
| 0x72AF70 | 3 | `seed_0072af70::unknown_identity_0072af70` | wdtmap.cpp | 5 satellite | WDT_Write_Map_Ini (wdtmap 0x72A880, 1 B off) |
| 0x6CA890 | 6,019 | `TechnoClass::Fire_At` | techno.cpp | 6 main helpers |  |
| 0x6C2C40 | 3,341 | `TechnoClass::unknown_slot66` | techno.cpp | 6 main helpers |  |
| 0x67F800 | 2,760 | `SidebarClass::StripClass::Unknown_0067F800` | sidebar.cpp | 6 main helpers |  |
| 0x43EE70 | 2,501 | `BuildingClass::Take_Damage` | building.cpp | 6 main helpers |  |
| 0x72A880 | 1,652 | `unknown_write_wdtmap_ini_72a880` | wdtmap.cpp | 6 main helpers |  |
| 0x4BE720 | 1,633 | `FlyLocomotionClass::unknown_4be720` | fly.cpp | 6 main helpers |  |
| 0x6F8E30 | 1,134 | `String_Table_Init_006f8e30` | uicmnds2.cpp | 6 main helpers |  |
| 0x519500 | 1,118 | `Allocate_Surfaces` | init.cpp | 6 main helpers |  |
| 0x5C8DD0 | 920 | `Set_Slot_Names_005c8dd0` | netshare.cpp | 6 main helpers |  |
| 0x4A6B90 | 10,797 | `Dropship_Screen_004A6B90` | dropship.cpp | 7 header form |  |
| 0x6D9B40 | 2,961 | `TechnoTypeClass::TechnoTypeClass` | classes/TechnoTypeClass/Constructor_Name.cpp | 7 header form |  |
| 0x762F70 | 2,867 | `CChatEventSink::OnFind` | wonline.cpp | 7 header form |  |
| 0x5E4780 | 2,468 | `OwnerDraw::unknown_5e4780` | ownrdraw.cpp | 7 header form |  |
| 0x5E0760 | 1,594 | `OwnerDraw_Control_Test_005e0760` | unidentified/uncalled_copies/OwnerDraw_Control_Test_005e0760.cpp | 7 header form |  |
| 0x5FEE70 | 1,281 | `OwnerDraw::unknown_draw_dialog_back_5fee70` | ownrdraw.cpp | 7 header form |  |
| 0x74A710 | 7,899 | `WOL_Game_Options_Proc_0074a710` | wonline.cpp | 8 count shape |  |
| 0x759E20 | 2,202 | `Start_Quick_Match_00759e20` | wonline.cpp | 8 count shape |  |
| 0x491750 | 289 | `Coord_Scatter` | coord.cpp | 8 count shape |  |
| 0x759370 | 2,687 | `Unknown_00759370` | wonline.cpp | 9 budget/dummy |  |
| 0x586B40 | 1,913 | `MapGeneratorClass::Create_Urban_Area` | mapgen.cpp | 9 budget/dummy |  |
| 0x4CBB40 | 1,467 | `FootClass::unknown_4cbb40` | foot.cpp | 9 budget/dummy |  |
| 0x603D50 | 592 | `Unknown_00603D50` | palette.cpp | 9 budget/dummy |  |
| 0x727CC0 | 36 | `??_G?$rc_ptr@VConflict@WorldDominationTour@@@@UAEPAXI@Z` | wdtconflict.cpp | 10 no draft | no draft / emitted by a parked caller (no parent found alone) |
| 0x4BFB30 | 36 | `UnknownFacingDir::Magnitude_At_Least_004bfb30` | classes/UnknownFacingDir/Magnitude_At_Least.cpp | 10 no draft | no draft / emitted by a parked caller (no parent found alone) |
| 0x4642A0 | 33 | `??Y?$TVelocity3D@N@@QAEAAU0@ABU0@@Z` | bullet.cpp | 10 no draft | no draft / emitted by a parked caller (no parent found alone) |
| 0x6AC7A0 | 1 | `seed_006ac7a0::unknown_empty_006ac7a0` | tactical.cpp | 10 no draft | no draft / emitted by a parked caller (no parent found alone) |

