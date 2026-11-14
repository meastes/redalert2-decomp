# Route A residuals by cause

Listing only (lane RouteA5, 2026-11-12, base `d6d77b2a8`, 300 Route A units). No source
changes, no compiles: every class is read from the module maps
(`manifests/original_target/modules/*.json`), the catalog, each row's own file and, for the
bound column only, the frozen inventory passes named by the private compiler config.
Machine-readable rows (one per residual, with unit, entry, bytes, population, source, class,
gate, switches): `build/routeA/residuals.json` (ignored). The STATUS Route A line now carries
the same split classes (`refile_unit.classify_split_rows`, read by `status.py`).

## 1. Split rows (3,715 rows, 266,680 B)

A split row is held by another frozen population. Classes in precedence order; the first
three were already in STATUS, the next five replace "not yet classified" (774 rows, 72,465 B,
now 0).

| Class | Rows | Bytes | What decides it |
| --- | ---: | ---: | --- |
| Template members | 1,466 | 99,990 | the file sets a container switch (`VECTOR_IMPLEMENT_*`, class-factory headers) or explicitly instantiates a template; 300 rows / 14,747 B are explicit instantiations with no switch |
| Needs another unit's header state | 681 | 43,887 | a top-level include/define the unit lacks: `shared/ghidra_types.hpp` 428 rows / 13,659 B; another include 225 / 23,162 B; another switch 28 / 7,066 B |
| Stand-ins | 794 | 50,338 | placeholder form (a file-local type or a `seed_` namespace) |
| **Emitted by another source** | 327 | 12,534 | comment-only file: inline constructor, header-static dynamic initialiser or compiler-generated deleting destructor emitted by another source of its population; moves only with its emitter (`globals` 124 rows / 6,424 B; 29 units, 46 populations) |
| **Held by a population that compiles each source alone** | 131 | 7,474 | the holding population is `separate_units` (`startup` 75 rows, `ownrdraw` 4 rows / 2,663 B, `drv_dsnd` 2 / 1,130 B; 30 units, 21 populations) |
| **Other option set** | 6 | 323 | compiled under an option set other than the unit's group (`mapgen` 3, `ownrdraw`, `windlg`, `wonline` 1 each) |
| **The unit's own second population** | 271 | 49,871 | every row of the holding population is a split row of this unit: a second compile group of the same unit that a join would empty (`convert` 100 rows / 21,815 B, `wonline` 66 / 15,076 B, `mapgen` 17 / 2,381 B, `auddev` 23 / 2,132 B, `tactical` 10 / 1,774 B; 21 units, 84 populations) |
| **No bound in the unit's frozen pass** | 39 | 2,263 | a mixed population (other units' rows too); the recorded cause is the frozen-bound refusal and nothing in the text blocks it (`mapgen` 9 / 719 B, `windlg` 3, `wonline` 4; 20 units, 18 populations) |
| Not yet classified | 0 | 0 | |

### The gate that actually refused each split row

`plan_unit` checks the option set, then the frozen bound, then the header-state screen. The
text classes above say what would block a row next; the gate that refused it is almost always
the bound:

| Gate (recorded cause) | Rows |
| --- | ---: |
| Frozen bound (the unit population's inventory pass has no equal bound) | 3,651 |
| Other option set | 30 |
| Header-state screen | 33 |
| Would empty its population | 1 |

Reading the passes themselves: for 3,617 split rows the unit population's pass has **no row at
that entry at all** (most populations carry a narrow composite pass), 58 have a different
inventory row, 6 a different gap row, and only 34 have an equal bound (20 header state, 13
template, 1 own population). So joining any split row needs the parked join model (a bound read
from the row's own population): no text change alone moves more than about 34 rows.

### Split rows by unit (largest 15 of 173 units)

| Unit | Rows | Bytes | Classes (rows/bytes) |
| --- | ---: | ---: | --- |
| convert | 208 | 27,559 | own population 100/21,815; stand-in 54/2,486; header state 44/2,609; template 10/649 |
| startup | 460 | 24,911 | template 307/14,715; stand-in 76/8,813; separate units 75/1,197; frozen bound 2/186 |
| wonline | 136 | 24,037 | own population 66/15,076; header state 24/5,554; template 9/2,430; stand-in 13/529; emitted 15/203; frozen bound 4/149; separate units 4/67; option set 1/29 |
| mapgen | 125 | 14,707 | header state 28/5,288; template 44/4,559; own population 17/2,381; stand-in 21/1,320; frozen bound 9/719; option set 3/216; emitted 2/187; separate units 1/37 |
| ownrdraw | 53 | 14,638 | stand-in 10/7,139; header state 26/4,388; separate units 4/2,663; own population 8/275; emitted 2/64; frozen bound 2/60; option set 1/49 |
| globals | 272 | 14,308 | emitted 124/6,424; stand-in 84/6,072; template 45/1,326; header state 19/486 |
| house | 108 | 7,672 | template 69/5,420; stand-in 21/1,358; header state 18/894 |
| map | 77 | 5,970 | template 52/4,329; stand-in 12/726; emitted 4/504; header state 9/411 |
| rules | 75 | 5,663 | template 52/3,945; stand-in 14/1,086; header state 7/612; separate units 2/20 |
| planmgr | 109 | 5,562 | template 59/3,957; stand-in 18/1,094; header state 31/462; emitted 1/49 |
| tactical | 53 | 5,072 | own population 10/1,774; template 20/1,738; header state 10/941; stand-in 9/494; separate units 2/123; emitted 2/2 |
| session | 74 | 4,975 | template 38/3,208; stand-in 18/1,070; header state 16/616; own population 1/74; separate units 1/7 |
| mpcoop | 63 | 3,885 | template 33/2,455; stand-in 13/580; header state 4/529; emitted 12/316; separate units 1/5 |
| building | 66 | 3,665 | template 37/2,489; stand-in 19/829; header state 10/347 |
| foot | 56 | 3,370 | template 32/2,483; stand-in 16/576; emitted 3/227; header state 5/84 |

### Header state named (the 681 rows, top includes/switches)

| Line the unit lacks | Rows | Bytes |
| --- | ---: | ---: |
| `#include "shared/ghidra_types.hpp"` | 428 | 13,659 |
| `#include "shared/word_sequence_views.hpp"` | 44 | 1,494 |
| `#include "shared/ram_file_class.hpp"` | 18 | 702 |
| `#include "shared/word_storage_layout.hpp"` | 15 | 1,739 |
| `#include "shared/unknown_geometry.hpp"` | 13 | 418 |
| `#include "shared/array_extension_ordinary_views.hpp"` | 11 | 1,306 |
| `#include "shared/coord.hpp"` | 10 | 423 |
| `#include "shared/unknown_static_rect.hpp"` | 8 | 384 |
| `#include "shared/ini_class.hpp"` | 7 | 2,577 |
| `#include "shared/map_cell.hpp"` | 5 | 3,267 |
| `#define RA2_VECTOR_UNSIGNED_INDEX` (migration holdout) | 3 | 72 |

(A row may need several lines; counts are per line.)

## 2. Flipped rows (315 rows, 24,397 B)

| Cause | Rows | Bytes | Units |
| --- | ---: | ---: | --- |
| Header-switch conflict: the file sets its own `VECTOR_IMPLEMENT_*` switch before `shared/vector_class.hpp`; in the unit's text the member is not emitted (symbol missing) | 302 | 23,887 | globals (all 302) |
| Unsupported `.CRT$XCU` static-initialiser section in a join | 8 | 224 | voxdrsys 7 / 150 B; houstype 1 / 74 B |
| Compile error (`ghidra_types.hpp`: C2371 `DWORD` redefinition) | 4 | 270 | ahandle2 |
| Differs under the unit's state | 1 | 16 | _map (`0x0040D0F0`, the COM pointer destroy at 0x8324B0) |
| Declaration-count window | 0 | 0 | none recorded; no flipped row carries `count_calibrated` or `split_declaration` |
| Other | 0 | 0 | |

Header-switch flips in `globals` by switch: `VECTOR_IMPLEMENT_VECTOR_RESIZE` 42 / 7,308 B,
`VECTOR_IMPLEMENT_DYNAMIC_RESIZE` 41 / 7,216 B, `VECTOR_IMPLEMENT_VECTOR_EQUAL` 42 / 2,604 B,
`VECTOR_IMPLEMENT_VECTOR_ID_OBJECT` 41 / 2,091 B, `VECTOR_IMPLEMENT_VECTOR_CLEAR` 42 / 1,638 B,
`VECTOR_IMPLEMENT_DYNAMIC_ID_OBJECT` 41 / 1,558 B, `VECTOR_IMPLEMENT_DYNAMIC_CLEAR` 27 / 1,134 B,
`VECTOR_IMPLEMENT_VECTOR_FETCH` 26 / 338 B. Five of these switches are kept by no Route A unit.

Other flipped rows: voxdrsys `0x0040AED0`, `0x00716EA0`, `0x00716EB0`, `0x00716EC0`,
`0x00716EE0`, `0x00716F10`, `0x00717CD0`; houstype `0x004FB610`; ahandle2 `0x004131D0`,
`0x004134E0`, `0x00413540`, `0x00413590`.

## 3. Switches kept (input for option (b), one shared header state)

636 per-unit switch instances, 278 distinct switches kept by at least one unit: 221 kept by one
unit only, 27 by two, 9 by three, 21 by four or more. By family: `VECTOR_IMPLEMENT_*` 28
switches / 254 unit instances; `RA2_*` 172 / 257; others 87 / 125. "Explains" = split rows
whose file sets the switch (template) or needs it (header state), and flipped rows whose file
sets it; a row that sets several switches counts under each.

| Switch | Units keeping it | Split rows / B it explains | Flipped rows / B |
| --- | ---: | ---: | ---: |
| `VECTOR_IMPLEMENT_ALL` | 52 | 13 / 3,185 | 0 |
| `VECTOR_IMPLEMENT_VECTOR_ACCESSORS` | 42 | 22 / 3,075 | 3 / 267 |
| `VECTOR_IMPLEMENT_DYNAMIC_ACCESSORS` | 40 | 11 / 2,163 | 2 / 190 |
| `VECTOR_IMPLEMENT_DYNAMIC_ADD` | 31 | 12 / 1,583 | 0 |
| `SHAPE_FILE_EMPTY_RECT` | 21 | 1 / 108 | 0 |
| `VECTOR_IMPLEMENT_DYNAMIC_DELETE_INDEX` | 19 | 18 / 1,592 | 1 / 91 |
| `VECTOR_IMPLEMENT_DYNAMIC_CONSTRUCTOR` | 16 | 26 / 2,062 | 0 |
| `VECTOR_IMPLEMENT_VECTOR_DESTRUCTOR` | 15 | 20 / 3,348 | 0 |
| `VECTOR_IMPLEMENT_DYNAMIC_DELETE_OBJECT` | 13 | 8 / 1,094 | 1 / 91 |
| `RA2_COORD_FROM_CELL_INLINE` | 13 | 0 | 0 |
| `RA2_COORD_AS_CELL` | 12 | 2 / 146 | 0 |
| `RA2_COORD_TPOINT3D` | 11 | 2 / 47 | 0 |
| `VECTOR_IMPLEMENT_DYNAMIC_CLEAR` | 7 | 103 / 4,404 | 27 / 1,134 |
| `RA2_RECT_POINT_INLINE` | 7 | 1 / 50 | 0 |
| `RA2_CELL_AS_COORD` | 6 | 0 | 0 |
| `RGB_DECLARE_ACCESSORS` | 6 | 0 | 0 |
| `VECTOR_IMPLEMENT_VECTOR_CLEAR` | 5 | 109 / 4,670 | 42 / 1,638 |
| `RA2_VECTOR_UNSIGNED_INDEX` | 5 | 3 / 72 | 0 |
| `RA2_SHIP_MOVE_TO_VALUE` | 5 | 0 | 0 |
| `RA2_ADJACENT_CELL` | 4 | 0 | 0 |
| `RA2_MATRIX3D_INLINE_COPY` | 4 | 0 | 0 |
| `VECTOR_IMPLEMENT_DYNAMIC_ID_OBJECT` | 3 | 94 / 4,025 | 41 / 1,558 |
| `VECTOR_IMPLEMENT_VECTOR_CONSTRUCTOR` | 3 | 9 / 1,068 | 0 |
| `RA2_COORD_CELL_BASE_INIT`, `RA2_TPOINT3D_DIFFERENCE`, `RA2_POINT_LENGTH` | 3 each | 2 / 47; 1 / 25; 1 / 22 | 0 |
| `COM_PTR_IMPLEMENT_RELEASE`, `RA2_COORD_POINT_Z`, `RA2_RECT_CORNER_INLINE`, `RA2_STAGE_SETTERS` | 3 each | 0 | 0 |

Switches set by residual rows but kept by **no** unit (the per-file template members):
`VECTOR_IMPLEMENT_VECTOR_RESIZE` 107 split / 20,015 B and 42 flipped / 7,308 B;
`VECTOR_IMPLEMENT_DYNAMIC_RESIZE` 102 / 19,505 B and 41 / 7,216 B;
`VECTOR_IMPLEMENT_VECTOR_EQUAL` 107 / 7,023 B and 42 / 2,604 B;
`VECTOR_IMPLEMENT_VECTOR_ID_OBJECT` 105 / 5,801 B and 41 / 2,091 B;
`VECTOR_IMPLEMENT_VECTOR_FETCH` 121 / 2,050 B and 26 / 338 B;
`VECTOR_IMPLEMENT_VECTOR_ASSIGN` 18 / 2,289 B; `VECTOR_IMPLEMENT_VECTOR_COPY_CONSTRUCTOR` 9 / 1,224 B;
`VECTOR_IMPLEMENT_DYNAMIC_NOINIT_CONSTRUCTOR` 4 / 168 B; `VECTOR_IMPLEMENT_DYNAMIC_INSERT_AFTER_INDEX` 1 / 97 B.
The 221 single-unit switches explain 220 split rows / 8,990 B between them.
Full per-switch unit and row lists: `build/routeA/residuals.json` (`switches`).

## 4. Type findings under /GR (data as source, lane D)

Classes whose RTTI the original names differently from our source; the population stays
without /GR until the type is fixed (docs/DECISIONS.md "/GR per population").

| Class | Finding | Evidence | Fix tried | Blocked by | Ruling |
| --- | --- | --- | --- | --- | --- |
| FactoryClass::Queue (house, factory, team) | element type is `TechnoTypeClass const *`, ours `TechnoTypeClass *` | RTTI at vtables 0x007A0F44/0x007A0F64 (type descriptors 0x007D7EB8/0x007D7EF0); TibSun factory.h `DynamicVectorClass<const TechnoTypeClass *> QueuedObjects` | `shared/factory_class_decl.hpp` Queue const; house.cpp `needed` lists and locals, factory.cpp two locals, `TeamClass::unknown_func_6bd870` const; 18 renames (factory copies 0x004BA060-0x004BA3CF, both vtables, constructor 0x004BA3C0, the team function); TechnoTypes (0x00A40AB8, vtable 0x0079DA14/0x007A0B9C) and ccini's copies stay non-const (their RTTI is non-const) | four declaration-count-fragile rows flip: house AI_Build_Defense 0x004F1190 (4 B), sidebar 0x006809F0 (4 B) and 0x006819A0 (2 B), queue Print_CRCs 0x0062A360 (4 B); factory, team, building, logic, init, saveload stay exact | owner 2026-11-13: park house /GR; re-roll the four rows under the corrected header at the end phase |
| DynamicVectorClass<TeamTypeClass const *> (house) | house emits a vtable the original does not hold | no original RTTI for it (??_R2/R3/R4 unbound) | not attempted | | parked with house |

### /GR census (2026-11-13): populations left without /GR

Every module population was compiled under /GR at the fixed compile folder; 356 of 405 passed (all rows exact, every emitted RTTI structure equal to the original with the ??_R3 bit rule) and were switched; ipxconn keeps its option set (`/Gr`, no recorded /Gr+/GR set). The 48 below stay without /GR; the class each refused structure names is the type finding (a template argument that is `T const *` or another type in the original, a class hierarchy that differs, or a vtable the original does not hold). Raw census: lane D build/laneD/gr_all.jsonl.

| Unit | Population | Classes named by the refused RTTI structures |
| --- | --- | --- |
| _map | population-4270 | `??_R1EOOI@?0A@A@INoticeSink@@8` |
| bitfont | population-4371 | `BitFontClass@@@8` |
| bittext | population-4216 | `BitTextClass@@@8` |
| builtype | population-4462 | `?$VectorClass@U?$TPoint3D@H@@@@@8` |
| ccfile | population-4460 | `??_R1A@?0A@A@CD@@8`; `??_R2CD@@8`; `??_R3CD@@8` |
| counter | population-1701 | `??_R1A@?0A@A@?$VectorClass@H@@8`; `??_R2CounterClass@@8`; `??_R2?$VectorClass@H@@8` |
| ddraw | population-4060 | `?$VectorClass@PAUDDSURFACEDESC@@@@@8`; `?$DynamicVectorClass@PAUDDSURFACEDESC@@@@@8` |
| factory | population-4446 | `??_R1CE@?0A@A@StageClass@@8`; `?$VectorClass@PAVTechnoTypeClass@@@@@8`; `?$DynamicVectorClass@PAVTechnoTypeClass@@@@@8` |
| globals | population-1596 | `code rows differ under /GR: 004d69b0, 004d69f0, 004d6aa0, 004d6ad0` |
| house | population-4382 | `?$VectorClass@PAVTechnoTypeClass@@@@@8`; `?$DynamicVectorClass@PAVTechnoTypeClass@@@@@8`; `??_R2?$DynamicVectorClass@PBVTeamTypeClass@@@@8`; `??_R3?$DynamicVectorClass@PBVTeamTypeClass@@@@8`; `??_R4?$DynamicVectorClass@PBVTeamTypeClass@@@@6B@`; `??_7?$DynamicVectorClass@PBVTeamTypeClass@@@@6B@` |
| houstype | population-3625 | `?$VectorClass@PAVUnitTypeClass@@@@@8`; `?$DynamicVectorClass@PAVUnitTypeClass@@@@@8`; `?$TypeList@PAVUnitTypeClass@@@@@8`; `?$VectorClass@PAVAircraftTypeClass@@@@@8`; `?$DynamicVectorClass@PAVAircraftTypeClass@@@@@8`; `?$TypeList@PAVAircraftTypeClass@@@@@8` (+3) |
| infatype | population-3263 | `?$VectorClass@PAVAnimTypeClass@@@@@8`; `?$DynamicVectorClass@PAVAnimTypeClass@@@@@8`; `?$TypeList@PAVAnimTypeClass@@@@@8` |
| init | population-4400 | `??_R1A@?0A@A@CD@@8`; `??_R2CD@@8`; `??_R3CD@@8` |
| ion | population-4379 | `?$VectorClass@PAVAnimClass@@@@@8`; `?$DynamicVectorClass@PAVAnimClass@@@@@8` |
| kamikaze | population-3656 | `?$VectorClass@PAVKamikazeControl@@@@@8`; `?$DynamicVectorClass@PAVKamikazeControl@@@@@8` |
| ldprgmgr | population-4267 | `??_R1A@?0A@A@Unknown535910@@8`; `??_R2Unknown535910@@8`; `??_R3Unknown535910@@8`; `Unknown535910@@@8` |
| list | population-1635 | `?$VectorClass@PAG@@@8`; `?$DynamicVectorClass@PAG@@@8` |
| loaddlg | population-4429 | `??_R1A@?0A@A@CD@@8`; `??_R2CD@@8`; `??_R3CD@@8` |
| mapgen | population-4346 | `?$VectorClass@PAUWIN32_FIND_DATA@@@@@8`; `?$DynamicVectorClass@PAUWIN32_FIND_DATA@@@@@8`; `?$DynamicVectorClass@VRect@@@@@8`; `?$VectorClass@VRect@@@@@8` |
| mixfile | population-3764 | `??_R1A@?0A@A@CD@@8`; `??_R2CD@@8`; `??_R3CD@@8` |
| mouse | population-2666 | `??_R1EOOI@?0A@A@INoticeSink@@8` |
| mpcoop | population-4359 | `?$VectorClass@PAVMPlayerScoreType@@@@@8`; `?$DynamicVectorClass@PAVMPlayerScoreType@@@@@8` |
| mschoice | population-2668 | `?$VectorClass@UPoint2D@@@@@8`; `?$DynamicVectorClass@UPoint2D@@@@@8` |
| msengine | population-4223 | `?$VectorClass@VRect@@@@@8` |
| multiplayer | population-4359 | `?$VectorClass@PAVMPlayerScoreType@@@@@8`; `?$DynamicVectorClass@PAVMPlayerScoreType@@@@@8` |
| netshare | population-4258 | `??_R1A@?0A@A@CD@@8`; `??_R2CD@@8`; `??_R3CD@@8` |
| progress | population-3898 | `??_R1A@?0A@A@ProgressScreenClass@@8`; `??_R2ProgressScreenClass@@8`; `??_R3ProgressScreenClass@@8` |
| radar | population-4448 | `RTacticalClass@@@8`; `?$VectorClass@UPoint2D@@@@@8`; `?$DynamicVectorClass@UPoint2D@@@@@8` |
| rules | population-3626 | `?$VectorClass@PAVBuildingTypeClass@@@@@8`; `?$DynamicVectorClass@PAVBuildingTypeClass@@@@@8`; `?$TypeList@PAVBuildingTypeClass@@@@@8`; `?$VectorClass@PAVVoxelAnimTypeClass@@@@@8`; `?$DynamicVectorClass@PAVVoxelAnimTypeClass@@@@@8`; `?$TypeList@PAVVoxelAnimTypeClass@@@@@8` (+18) |
| scenario | population-4195 | `??_R1A@?0A@A@CD@@8`; `??_R2CD@@8`; `??_R3CD@@8` |
| scroll | population-4218 | `??_R1EOOI@?0A@A@INoticeSink@@8` |
| session | population-4457 | `?$VectorClass@PAUUnknownGlobalPacket@@@@@8`; `?$DynamicVectorClass@PAUUnknownGlobalPacket@@@@@8` |
| sidebar | population-4430 | `?$VectorClass@UPoint2D@@@@@8`; `?$DynamicVectorClass@UPoint2D@@@@@8` |
| smudtype | population-2570 | `?$VectorClass@PAVSmudgeTypeClass@@@@@8`; `?$DynamicVectorClass@PAVSmudgeTypeClass@@@@@8` |
| swizzle | population-2710 | `?$VectorClass@USwizzlePointerClass@@@@@8`; `?$DynamicVectorClass@USwizzlePointerClass@@@@@8` |
| tab | population-4406 | `??_R1EOOI@?0A@A@INoticeSink@@8` |
| techtype | population-4341 | `?$VectorClass@PAVParticleSystemTypeClass@@@@@8`; `?$DynamicVectorClass@PAVParticleSystemTypeClass@@@@@8`; `?$TypeList@PAVParticleSystemTypeClass@@@@@8`; `?$VectorClass@PAVVoxelAnimTypeClass@@@@@8`; `?$DynamicVectorClass@PAVVoxelAnimTypeClass@@@@@8`; `?$TypeList@PAVVoxelAnimTypeClass@@@@@8` (+6) |
| terrain | population-3691 | `??_R1A@?0A@A@TerrainClass@@8`; `??_R2TerrainClass@@8`; `??_R3TerrainClass@@8` |
| tiberium | population-4444 | `?$VectorClass@PAVAnimTypeClass@@@@@8`; `?$DynamicVectorClass@PAVAnimTypeClass@@@@@8`; `?$TypeList@PAVAnimTypeClass@@@@@8` |
| tooltip | population-3429 | `?$VectorClass@PAVToolTip@@@@@8`; `?$DynamicVectorClass@PAVToolTip@@@@@8` |
| txtlabel2 | population-4360 | `code rows differ under /GR: 006f1b60, 006f20b0, 006f22c0, 006f2d00` |
| volinfo | population-3962 | `??_R1A@?0A@A@CD@@8`; `??_R2CD@@8`; `??_R3CD@@8` |
| warhead | population-3520 | `?$VectorClass@PAVAnimTypeClass@@@@@8`; `?$DynamicVectorClass@PAVAnimTypeClass@@@@@8`; `?$TypeList@PAVAnimTypeClass@@@@@8` |
| waypoint | population-3675 | `?$VectorClass@UWaypointClass@@@@@8`; `?$DynamicVectorClass@UWaypointClass@@@@@8` |
| wdtmap | population-4107 | `?$VectorClass@PAVAnimationPalette@Map@WorldDominationTour@@@@@8`; `?$DynamicVectorClass@PAVAnimationPalette@Map@WorldDominationTour@@@@@8`; `?$VectorClass@UCentroid@WorldDominationTour@@@@@8` |
| weapon | population-2437 | `?$VectorClass@PAVAnimTypeClass@@@@@8`; `?$DynamicVectorClass@PAVAnimTypeClass@@@@@8`; `?$TypeList@PAVAnimTypeClass@@@@@8` |
| wonline | population-4248 | `INetUtilEvent@WOL@@@8`; `??_R1A@?0A@A@INetUtilEvent@WOL@@8`; `??_R1A@?0A@A@CNetUtilEventSink@@8`; `??_R1A@?0A@A@?$CComObject@VCNetUtilEventSink@@@ATL@@8`; `??_R2?$CComObject@VCNetUtilEventSink@@@ATL@@8`; `??_R3?$CComObject@VCNetUtilEventSink@@@ATL@@8` (+17) |
| wsproto | population-3655 | `?$VectorClass@PAVWinsockBufferType@WinsockInterfaceClass@@@@@8`; `?$DynamicVectorClass@PAVWinsockBufferType@WinsockInterfaceClass@@@@@8` |
