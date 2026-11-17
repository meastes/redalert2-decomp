# Data as source: sizing and pilot (lane D, 2026-11-13)

Byte-free counts. Every non-code byte of the image (headers, `.rdata`, `.data` with its
uninitialised tail, `.rsrc`) gets one kind, and each kind is split by proof state. Nothing
here is .text credit.

## Method

- Kinds come from the PE directories (headers, import descriptors, IAT, lookup and hint/name
  tables, debug directory, resources), from the RTTI graph of the 770 vtables in
  `manifests/rtti_classes.json` (vtable with its locator slot; COL, CHD, BCA, BCD, TD), from
  Ghidra's string types (string, unicode, TerminatedCString, char[N]), and from the section a
  byte sits in: other `.rdata` is a constant table, other raw `.data` is an initialised global
  (runs of 4 or more zero bytes inside raw `.data` are "zero-valued"), `.data` past its raw size
  is zero-initialised (bss). Float constants are the `__real@` targets of accepted rows. Library
  data is data that only identified library code (CRT >= 0x781000, library.json) references.
- "Proven (module maps)": the union of every section a module's object emits that
  `module_ledger.py` laid at its original address and found equal (manifests/original_target/
  modules/*.json, data and kept_elsewhere; deduplicated by address).
- "Proven (other objects)": the same proof run over every compile group of a kept build
  (`build.command --keep-intermediates`): sections emitted by non-module objects (residual rows'
  files) that prove at their bound addresses.
- "Referenced by matched code, not yet emitted": bytes from each relocation target of an
  accepted row to the next known item start (Ghidra item, relocation target or proven copy;
  at most 4 KB). Accepted source names them as `extern` (or as a compiler-numbered static it
  does not define); a definition (or, for a string, the literal itself) makes them emitted.
- "Rest": no accepted row references the item's start. It is reached through pointers in
  data, from residual scaffold, or not at all.

## Table

| Kind | Bytes | Proven (module maps) | Proven (other objects) | Referenced by matched code, not yet emitted | Rest |
| --- | ---: | ---: | ---: | ---: | ---: |
| String literals | 126,489 | 89,290 | 6,152 | 14,353 | 16,694 |
| Float constants (`__real@`) | 1,436 | 1,260 | 176 | 0 | 0 |
| Other constant tables (.rdata) | 80,664 | 12,246 | 548 | 17,780 | 50,090 |
| Vtables (with locator slot) | 44,844 | 24,128 | 4,252 | 1,256 | 15,208 |
| RTTI (COL, CHD, BCA, BCD, TD) | 95,236 | 593 | 0 | 0 | 94,643 |
| Initialised globals (non-zero) | 228,102 | 11,118 | 95 | 47,145 | 169,744 |
| Initialised globals (zero-valued runs in .data) | 25,705 | 1,652 | 8 | 9,741 | 14,304 |
| Zero-initialised globals (bss) | 3,127,748 | 0 | 0 | 108,920 | 3,018,828 |
| Library data (CRT only) | 5,490 | 0 | 0 | 4 | 5,486 |
| Import (descriptors, IAT, ILT, names) | 9,532 | 0 | 0 | 1,104 | 8,428 |
| Resources (.rsrc) | 90,040 | 0 | 0 | 0 | 90,040 |
| Headers, debug directory | 4,124 | 0 | 0 | 0 | 4,124 |
| **Total** | **3,839,410** | **140,287** | **11,231** | **200,303** | **3,487,589** |

File-backed part (everything but bss): 711,662 bytes, of which 151,518 (21.3%) are proven at
their original addresses today. The file also carries a 272-byte overlay.

## The three questions, per kind

| Kind | Already emitted and proven | Provable mechanically from accepted source | Needs authored definitions |
| --- | --- | --- | --- |
| String literals | 95,442 B (75%) | 14,353 B: strings our source spells as `extern char const x[]` at the literal's address; the literal in place of the extern is a rename (the pilot did two) | 16,694 B reached only from pointer tables, scaffold or nothing |
| Float constants | all 1,436 B | - | - |
| Constant tables | 12,794 B | part of 17,780 B (tables the code indexes; values from the original, layout from the code) | 50,090 B incl. switch-free lookup tables, CRT tables, unreferenced |
| Vtables | 28,380 B | 1,256 B (vtables named by matched code but emitted nowhere) | 15,208 B: classes whose vtable no compiled object emits yet |
| RTTI | 593 B | ~94.6 KB is emitted by `/GR` for every class whose vtable an object emits; it proves only if our class names, template arguments and base lists equal the original's (see pilot) | none in principle |
| Initialised globals | 12,873 B | 56,886 B referenced (definitions with the original values) | 184,048 B unreferenced by matched code |
| Bss | no byte proof possible (zero bytes); address/size layout only | 108,920 B referenced | rest: extents unknown without types |
| Library data | - | 5,490 B belongs with the linked library members (library ledger), not authored source | - |
| Import, resources, headers | - | linker/resource compiler outputs (import libraries, an .rc script) | the .rc script (90,040 B of resources) |

## Pilot (house, campaign, terrtype)

The existing module data check: `modules.json` `data` ranges, tiled completely by sections the
module's object emits, each equal to the original at the address its first symbol is bound to
(`module_ledger.py`; part of `refile_unit.py maps`). A unit's `.data` block is bounded by its
neighbours' items (the previous unit's last string, the next unit's first type descriptor).

- **campaign** (range 0x7D1410-0x7D1480, 112 B): population set to `/GR`; the CampaignClass
  type descriptor and six literals tile it. Landed on the branch (apply_rename `options`, strict
  audit exact, whole-file exact).
- **terrtype** (0x7F81B8-0x7F825F, 167 B): `/GR`; type descriptor plus nine literals. Landed the
  same way.
- **house** (0x7D9FA0-0x7DAA31, 2,705 B): source changes make every byte emitted: the two
  Adjust_Threat tables as function-local statics `_val`/`_thr` (TibSun house.cpp
  HouseClass::Adjust_Threat), `unknown_flag_7d9fe8 = true`, two strings as literals instead of
  externs. Under `/GR` the range is complete in a screen compile (2,548 B in 77 sections plus
  alignment fill, all code rows exact), but the strict audit refuses the object: house's source
  says `VectorClass<TechnoTypeClass *>` where the original RTTI names
  `VectorClass<TechnoTypeClass const *>` (TDs at 0x7D7EB8/0x7D7EF0 differ), and it emits a
  `DynamicVectorClass<TeamTypeClass const *>` vtable the original does not hold. Not landed;
  draft and screens under build/laneD.
- **tagtype** (tried as a small unit): `/GR` refused: our TagTypeClass hierarchy descriptor
  differs from the original's (base list).

Tool change: `module_ledger.coverage` accepts zero fill below the next section's alignment
(8-aligned type descriptors after odd-length ones), not only below 4 bytes.

Cost: about 1.2 lane-hours for the pilot: campaign and terrtype took minutes each once the
screen existed (0.28 KB together); house took about 40 minutes to complete in screen (2.7 KB)
and is blocked on types. About 0.3 h per KB when the class types are right.

## What blocks the rest

1. RTTI needs `/GR` per population, and `/GR` turns every wrong class name, template argument
   (const-ness) or base list into a refusal. That is the largest kind (95 KB) and a type audit
   for the whole tree: two of four pilot units failed it. The refusals are evidence, not noise.
2. Initialised globals (228 KB) are mostly unreferenced by matched code or declared `extern` in
   one unit and defined nowhere; each definition needs its original unit (the block it sits
   in) and its values; header-defined statics repeat per unit (each unit's own
   `unknown_geometry_*` block in bss).
3. Bss cannot be proven by bytes; a layout proof (address and size of every uninitialised
   symbol per unit) is a new check.
4. COMDAT copies (vtables, TDs, `__real@`, strings with /Gf) are kept from the first object in
   link order; a unit's block holds only the copies it was first to emit, so ranges must come
   from the original's neighbours, never from what our object emits.
5. Resources, imports and headers are linker and resource-compiler products: an .rc script and
   the import libraries, not C++ definitions.

Correction (2026-11-13): the first table counted 5,486 bss bytes as library data (the CRT-only rule ran over
the uninitialised tail too); the file-backed total is 711,662, not 717,148.

## State after the /GR batch (2026-11-13)

Proven at the original addresses (module maps, deduplicated by address; `status.py`), after
the strings batch (43 literals, 1,581 B) and /GR for 354 of 675 populations:

| Kind (module ledger) | Bytes proven | Of the kind in the image |
| --- | ---: | ---: |
| String literals | 93,311 | 126,489 |
| RTTI (COL, CHD, BCA, BCD, TD) | 76,027 | 95,236 |
| Vtables | 36,524 | 44,844 |
| Constant tables and float constants (`table`) | 13,205 | 82,100 |
| Function statics (`static`) | 3,199 | in initialised globals |
| **Total** | **222,266 of 711,662 file-backed (31.2%)** | |

14 class hierarchy descriptors carry the original's ambiguous bit (docs/DECISIONS.md "Class
hierarchy descriptor ambiguous bit") and count without their attributes word. Initialised
globals still prove only per whole unit block (one `.data` section per object). The first
sizing (about 354 KB of per-unit blocks, "about 256 KB to author") ran each unit's span to its
next owned item and so swallowed other objects' data; the corrected census is in the lane AG
section below (318.5 KB of unit blocks, of which 155.7 KB is the fastmath generated module and
about 50 KB is left to author).

### Type findings that keep populations without /GR

48 units (docs/ROUTE_A_RESIDUALS.md section 4 lists each with the refused classes):

- **Const element types of the type lists.** The original names `TypeList`/`VectorClass`/
  `DynamicVectorClass` of `AnimTypeClass const *`, `BuildingTypeClass const *`,
  `VoxelAnimTypeClass const *`, `UnitTypeClass const *`, `AircraftTypeClass const *`,
  `InfantryTypeClass const *`, `SmudgeTypeClass const *`, `TerrainTypeClass const *`,
  `ParticleSystemTypeClass const *` (infatype, rules, techtype, tiberium, warhead, weapon,
  houstype, smudtype), and `TechnoTypeClass const *` for FactoryClass's queue (factory, house).
  Probe (AnimTypeClass only): the change reaches the shared reader `TypeListReader<T>::Get`
  (ccini_class.hpp), which returns `TypeList<T *>` for every type, and locals in aircraft and
  infantry; infatype and warhead compile but their rows lose bindings that a name-based rename
  cannot split (the non-const instance at 0x004D55B0-0x004DD090 shares every decorated name with
  the const one at 0x0050B100-0x0050B7D0, so `apply_rename` needs per-population renames). It is
  one tree-wide batch for all type lists, not one list at a time. The FactoryClass batch flipped
  the declaration-count-fragile rows (AI_Build_Defense, Print_CRCs, two sidebar rows): parked
  for the end-phase re-roll.
- **Class hierarchies:** CD (ccfile, init, loaddlg, mixfile, netshare, scenario, volinfo); an
  INoticeSink base descriptor (_map, mouse, scroll, tab); BitFontClass, BitTextClass, TerrainClass,
  ProgressScreenClass, RTacticalClass, StageClass, CounterClass, Unknown535910.
- **Other vector element types:** Point2D (mschoice, radar, sidebar), Rect (mapgen, msengine),
  MPlayerScoreType (mpcoop, multiplayer), TPoint3D<int>, WaypointClass, ToolTip, DDSURFACEDESC,
  KamikazeControl, SwizzlePointerClass, AnimClass, WinsockBufferType, `unsigned short *`, `int`,
  WIN32_FIND_DATA, UnknownGlobalPacket, the WorldDominationTour palettes and centroids.
- **wonline:** the WOL COM event sinks (CComObject, ATL layouts).
- **house:** an extra `DynamicVectorClass<TeamTypeClass const *>` vtable.
- **globals, txtlabel2:** code rows differ under /GR.
- **ipxconn:** option set `/Gr` (no recorded `/Gr` + `/GR` set).
## Authored-globals pilot (lane AG, 2026-11-13)

Rule: author only definitions whose type is evidenced (accepted use site, TibSun/Renegade/YR
twin, constructor); credit a block only when the unit's whole .data section proves at the
original addresses (docs/DECISIONS.md "Data-only symbols bound by section layout / bss by
elimination", "fastmath tables as a generated data module").

- **Unit blocks.** A unit's raw .data is its object's one non-COMDAT `.data` section
  (definition order) followed by its COMDATs (literals, type descriptors) in emission order
  (smartdeform, smudge). The first census ran spans to the next single-unit item and
  swallowed other objects: smartdeform's "26 KB" is 508 B (0x7F4034-0x7F4230); the four
  undo ints at 0x8286E0 are TibSun's `extern "C"` DeformPointXAdd/YAdd/Width/Height of the
  ripple assembly module, followed by other late-linked objects' tables.
- **smartdeform: 508 B proven (landed).** TibSun smartdeform.cpp `_ramp_corners0..20` and
  `_ramp_corners[21]` (values equal the original; full height 30). The arrays are bound by
  the enclosed layout of their section (slider's BTN-PLUS.SHP literal below, the
  VectorClass<UndoInfoStruct> type descriptor above, the code-bound table inside), the
  all-zero `_ramp_corners0` by bss elimination (0xABCE9C-0xABCEAC, the only free hole of the
  unit's bss); flags `data_bound_by` (tools/original_target/data_bindings.py).
- **wonline: open.** Blocker: its `.data` section (0x7FD2D8-0x7FD538, 608 B) holds a 12 x
  32-byte TCP state-name table ("CLOSED" ... "DELETE_TCB", 384 B) that nothing in the image
  references and no twin has, besides about 150 B typed by use and about 45 B of int arrays
  whose extent no use fixes. Its COMDAT region (to 0x8004BC, about 12 KB) also holds literals
  of 130 split residual rows (CChatEventSink, CNetUtilEventSink, CDownloadEventSink,
  Handle_User_Leave, WOL settings) that compile in other populations.
- **fastmath: 155,712 B as a generated data module.** The five tables of TibSun
  fastmathtab.asm (sin 10,244, asin 4,100, tan 4,100, atan 4,098, sqrt 16,386 dwords) at
  0x801DC4-0x827E04, generated by tools/original_target/generate_fastmath.py with integer
  and exact rational arithmetic: x87 transcendental results rounded to nearest at the 64-bit
  extended mantissa (fsin/fptan reduce by the x87's 66-bit pi, so sin(pi) is not zero and
  sin(pi/2) is exactly 1), then chopped to float (the game's control word); square roots
  chop directly; atan steps accumulate in float. Every entry is reproduced; the atan step
  `ATAN_STEP_BITS` = 0x3CC7FE84 is a flagged constant (derivation unknown; TibSun's
  pi/size/pi * tan_tab[1017] and RA2's Remap_Atan_Table tan_tab[1017]/4096 give other
  values). build.py writes the module under the build folder, assembles it with the pinned
  MASM, proves the one .data section as a whole block (all seven decorated names bound by
  accepted references to one base) and places it over the scaffold
  (manifests/original_target/generated_data.json).
- **Corrected census** (raw .data 0x7C9000-0x831000, 425,984 B; item starts from Ghidra,
  accepted relocations, proven copies and every pointer in the image; a unit's run ends at
  the end of its last owned item):

  | | Bytes |
  | --- | ---: |
  | Unit blocks (271 units) | 318,533 |
  | of which proven before this lane | 112,927 |
  | of which the fastmath generated module | 155,712 |
  | of which typed, not yet proven: strings with an accepted use / other items with an accepted use / type descriptors | 20,719 / 6,609 / 986 |
  | of which untyped: strings with no accepted use / items referenced only by residual code or data / unreferenced | 11,885 / 6,877 / 2,818 |
  | Outside any single-unit block (shared or unowned items) | 107,451 |
  | of which proven | 18,791 |
  | of which typed (strings with an accepted use / other items / type descriptors) | 4,051 / 740 / 1,711 |
  | of which untyped (strings with no accepted use / referenced only by residual code or data / unreferenced) | 17,452 / 39,874 / 24,832 |

  So the first sizing's "about 256 KB to author" is: 155.7 KB fastmath (generated, now
  proven), about 28.3 KB typed and about 21.6 KB untyped inside unit blocks, and about
  88.7 KB of unproven items no single unit owns (6.5 KB typed, 82.2 KB untyped). 100 blocks
  hold no untyped byte (161.4 KB: fastmath plus 5.7 KB in 99 small blocks, 1.5 KB of that unproven). Authoring globals stops here
  (review session's call).
