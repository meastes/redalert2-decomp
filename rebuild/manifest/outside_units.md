# Main-exact code outside every rebuild unit (lane R23, 2026-11-19)

`outside_units.csv` lists every row of main's canonical catalog (C/C++ regions) whose entry lies in no
rebuild unit's range (`units.json`, pending units included). First map (at rebuild-trial 7b301a8f4):
2,467 rows, 297,976 bytes. **Now (after lane R23's batches 1-3): 1,446 rows, 217,006 bytes** (full
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

## Remaining, by kind

| Kind | Rows | Bytes |
| --- | ---: | ---: |
| ordinary | 433 | 149,488 |
| template copy | 323 | 28,896 |
| factory (TClassFactory instances) | 445 | 28,560 |
| initializer (`_$E`) | 233 | 9,952 |
| runtime library (>= 0x781000) | 12 | 110 |

## Remaining, by target

| Target | Bytes | State |
| --- | ---: | --- |
| wonline 0x7469C0-0x7683B0 | 128,112 | survey below: one object, needs text |
| startup TClassFactory tail | 29,424 | follows WinMain (guarded; the re-roll pilot's non-refitting row) |
| dropship 0x4A6B90-0x4AA2B0 | 14,112 | positive source; needs main's RA2 bodies reshaped |
| rules TypeList tail | 6,048 | follows the guarded Rules process functions |
| mapgen, session (pending units) | 6,736 | rows between the widened range and the next copy |
| non-adjacent main modules (txtlabel, motlib, wdtmap, score, ...) | 13,658 | held (review ruling) |
| new underscore units (_rules, _isotile, _mixfile, _map), missile, const | 7,507 | need a positive source; provisional if filed |
| getcpu Get_CPU_Type / Get_CPU_Type_Name | 320 | mischa85 text needs `#pragma auto_inline(off)` (budget only falls): held |

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
