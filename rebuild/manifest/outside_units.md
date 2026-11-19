# Main-exact code outside every rebuild unit (lane R23, 2026-11-19, refreshed after batch 5; lane R24 batches 1-3)

`outside_units.csv` lists every row of main's canonical catalog (C/C++ regions) whose entry lies in no
rebuild unit's range (`units.json`, pending units included). First map (at rebuild-trial 7b301a8f4):
2,467 rows, 297,976 bytes. **Now (after lane R23's batches 1-5 and lane R24's batches 1-3): 1,005 rows, 176,702 bytes** (full
extent: body + frozen padding). Rows inside pending units' ranges count as inside their unit.

Columns: `main_module` is the module of main's unit map (`manifests/original_target/modules/*.json`)
holding the row; `mischa85_module` its module in mischa85's `config/ra2-1.006/modules.csv`;
`prev_unit`/`next_unit` the neighbouring rebuild units; `target_unit` with `evidence` the unit it most
likely belongs to; `target_object_emits` whether that unit's last checked object defines the row's
symbol; `filing` the precedent that would cover it (DECISIONS, "Filing main-exact code that lies outside
every rebuild unit", 2026-11-18); `counted_elsewhere` any overlap with `library.json`/`assembly.json`.

## Double counting

No outside row overlaps a `library.json` or `assembly.json` region, so nothing here is in the rebuild's
LIBRARY (89,127) or ASSEMBLY (16,999) receipt. The 12 rows at or above 0x781000 (110 bytes, libcmt
seeds) are C rows main credits as matched C; `library.json` skips them because they overlap accepted
rows, so they are in neither receipt: a library-ledger question, not a unit filing.

## Filed by lane R23

| Batch | What | Exact bytes |
| --- | --- | ---: |
| 1 | rule (1), `tools/filetail.py`: 22 units widened up to the last copy their own object defines | 24,606 (10,101 in landed units) |
| 2 | EventClass::Execute to event (rule 2: mischa85's event module, TibSun event.cpp) | 0 (parked, 264 bytes differ) |
| 3 | new unit `globals` 0x4D2BA0-0x4DD590 (TibSun globals.cpp, ra2-decomp twin, .CRT$XCU position) | 39,895 |
| 4 | head rows (R4 rule), trigger/teamtype tails, `filetail.py --link-order` (kept copies fix link order) | 7,638 (5,873 catalogued) |
| 5 | eleven underscore global-object units 0x40ACB0-0x40E970 (TibSun per-global files in alphabetical link order) | 11,628 |
| R24-1 | dropship widened to 0x4AA2B0 (rule 2; Draw_Cost, Draw_Unit_Info exact from main's RA2 bodies; Dropship_Screen parked, see b_log_r24.csv) | 1,309 |
| R24-2 | new units _alpha and _logic (TibSun files; _alpha.h and layer.h include trims) | 953 |
| R24-3 | new unit missile (TibSun file; Projectile_Motion has no main row, guarded) | 356 |

## Remaining, by kind

| Kind | Rows | Bytes |
| --- | ---: | ---: |
| ordinary | 337 | 132,592 |
| factory (TClassFactory instances) | 445 | 28,560 |
| template copy | 127 | 12,160 |
| initializer (`_$E`) | 84 | 3,280 |
| runtime library (>= 0x781000) | 12 | 110 |

## Remaining, by target

| Target | Bytes | State |
| --- | ---: | --- |
| wonline 0x7469C0-0x7683B0 | 128,112 | one object (survey below); held until text (owner ruling) |
| startup TClassFactory tail | 29,424 | follows WinMain (guarded) |
| non-adjacent main modules (motlib, txtlabel, ...) | 5,296 | held (review ruling) |
| _vanim, const | 2,144 | TibSun files in link order; _vanim parked under the owner ruling (no single header gives the second zero Rect to exactly the four objects that have it; b_log_r24.csv); const needs its RA2 tables |
| rules TypeList rest, teamtype, wsproto, movies, hsv, ... | ~12,000 | rows between a range and the next kept copy, or tails needing text |
| getcpu Get_CPU_Type / Get_CPU_Type_Name | 320 | needs `#pragma auto_inline(off)` (budget only falls): held |

The underscore ranges are TibSun's one-global files (`_x.cpp`), linked in alphabetical order through
0x40AC80-0x40F020: _alpha, _astar, _astar2 (ra2-decomp's name; BombList), _command, _convert, _isotile,
_logic, _map, _milsectmr, _mixfile, _palette, _rtti, _rules, _script, _timer, _vanim, _voxel. The whole
image links alphabetically (const between conquer and control, missile between milsectmr and mission).

## wonline 0x7469C0-0x7683B0 boundary survey

- Initializers: the span holds no `.CRT$XCU` entry (the next ones, 0x7683B0-0x768580, open worlddom).
  The two `_$E` rows main names in the span are function-static destructors registered by their
  function: 0x752710 (a 16-element array destructor, referenced from 0x753240) and 0x765980 (`ret`,
  referenced from OnGameStart 0x761570). Under DECISIONS (4) of the .CRT$XCU ruling they move with their
  registering function and are no boundary. mischa85's `unk_00752710` gap is this one function.
- File-name strings: no code in the span refers to a source-file name string.
- mischa85: `unk_007469C0` and `unk_00752730` (code not assigned) are absence, not boundaries.
- Class families: wonline dialog procedures and session functions (0x7469C0-0x75D0D0), then ATL
  (`_AtlAdvise@16`, `_AtlModuleInit@12`), CNetUtilEventSink / CChatEventSink / CDownloadEventSink
  methods and their CComObject wrappers, `_AtlModuleTerm@4`, then the WOL container templates
  (ArrayList, Dictionary, VectorClass<tConnInfoStruct *>). ra2-decomp and mischa85 keep the sinks in
  wonline.cpp.
- Kept copies, which fix link order: the rebuild's wonline object defines `??_GSimpleWonlineDialogControl` (kept at
  0x75D0A0), `??_GWonlineStringDialogControl`, `??0WonlineStringDialogControl`,
  `??_GCreateGameDialogControl` (0x765A60-0x765AC0), `ArrayList<Server>::getPointer` (0x765E60) and
  `Dictionary<Wstring,int>::shrink` (0x767850). A select-any copy is kept in the first defining object in
  link order, and wonline's object precedes the span. So these copies lie inside wonline's own
  contribution, and every row between wonline's current end and 0x7683B0 (worlddom's first row) is
  wonline's object. **No evidenced boundary: the whole span is wonline's.**
- Trial (not landed): widening wonline to 0x7683B0 gives 188/431 rows exact (+13 rows, +1,162 bytes,
  the kept copies) and adds about 125 KB of rows without text to the parked tail. That would take
  parked large functions past the 500 KB checkpoint, so it waits for the owner. The text for the
  remaining rows is main's (class b) or future mischa85 extensions of wonline.cpp.
