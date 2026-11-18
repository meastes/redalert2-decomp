# Main-exact code outside every rebuild unit (lane R23, 2026-11-18, at rebuild-trial 7b301a8f4)

`outside_units.csv` lists every row of main's canonical catalog (C/C++ regions, 17,910 rows) whose entry
lies in no rebuild unit's range (`units.json`, all 421 units, pending included): **2,467 rows, 297,976
bytes** (full extent: body + frozen padding). Rows inside the 24 pending units' ranges (1,994 rows,
638,863 bytes) are not counted here; they are inside a unit.

Columns: `main_module` is the module of main's unit map (`manifests/original_target/modules/*.json`, code,
joined and residual rows) that holds the row; `mischa85_module` its module in mischa85's
`config/ra2-1.006/modules.csv`; `prev_unit`/`next_unit` the neighbouring rebuild units;
`target_unit` with `evidence` the unit it most likely belongs to; `target_object_emits` whether the
target unit's last checked object (`build/r/last/<unit>.json`) defines the row's symbol; `filing` the
precedent that would cover it; `counted_elsewhere` any overlap with `library.json`/`assembly.json`.

Target evidence, in order: main's module map (278,565 bytes), main's file (16,029), mischa85's module
(768), nearest neighbour in a mischa85 `unk_` gap (2,614). One override: `?Execute@EventClass@@QAEXXZ`
(0x4B65C0, 5,184 bytes) goes to `event` (mischa85 lists it as event.cpp's unplaced function; main's module
map files it under `init`).

## Double counting

No outside row overlaps a `library.json` or `assembly.json` region, so nothing here is in the rebuild's
LIBRARY (89,127) or ASSEMBLY (16,999) receipt. The 12 rows at or above 0x781000 (110 bytes; libcmt
seeds such as `operator delete`, `srand`, `errno`) are C rows main credits as matched C; `library.json`
skips them because they overlap accepted rows, so they are in neither receipt. They are a library-ledger
question, not a unit filing.

## By kind

| Kind | Rows | Bytes |
| --- | ---: | ---: |
| ordinary | 609 | 167,322 |
| template copy | 1,019 | 83,680 |
| factory (TClassFactory CreateInstance etc.) | 458 | 29,312 |
| initializer (`_$E`) | 369 | 17,552 |
| runtime library (>= 0x781000) | 12 | 110 |
| handwritten asm (inline_asm flag) | 0 | 0 |

## By filing

| Filing | Rows | Bytes |
| --- | ---: | ---: |
| tail, neighbour object already emits the symbol (radsite precedent) | 263 | 23,484 |
| tail, main files it under the preceding unit; range widening, text to add | 1,148 | 207,187 |
| head of the next unit (`init --auto` head rule) | 55 | 2,203 |
| new unit (main module with no rebuild unit): needs ruling | 775 | 51,091 |
| main module not adjacent to the row: needs ruling | 214 | 13,901 |
| library ledger | 12 | 110 |

## By target unit (largest)

| Target | Bytes | Note |
| --- | ---: | --- |
| wonline | 128,112 | 0x7469C0-0x768320, main module wonline; mischa85 leaves it as unk_007469C0/unk_00752730 (may be several original units) |
| new unit globals | 43,584 | 0x4D2BA0-0x4DD590, main's globals module (header statics, DVC instances, initializers); mischa85 unk_004D2BA0 |
| startup | 29,424 | 0x691450-0x6986F0, 445 TClassFactory CreateInstance rows main files in startup.cpp |
| dropship | 14,112 | 0x4A6B90-0x4AA260 |
| map | 8,672 | 6,144 of it already emitted by map's object |
| planmgr (pending) | 8,256 | 5,360 emitted |
| mapgen (pending) | 6,416 | 2,208 emitted |
| rules | 6,048 | template copies |
| house (non-adjacent) | 5,200 | template copies placed away from house |
| event | 5,184 | EventClass::Execute |
| building | 3,648 | 1,536 emitted |

## First filings (radsite precedent, catalogued targets, object already emits)

map 6,144 (50 rows), building 1,536, coopcamp 1,088, startup 768, wdtnet 496, trigger 288, vanimtype 256,
teamtype 240, wonline 208, wsproto 192, blight 160, foot 160: about 11.4 KB. The same rule on pending
units (planmgr 5,360, mapgen 2,208, session 1,408, bullet 900, ebolt 688) adds about 10.9 KB when those
units land.
