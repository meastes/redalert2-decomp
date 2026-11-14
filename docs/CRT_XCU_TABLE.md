# The original `.CRT$XCU` table as unit evidence for the `$E` rows (lane X1)

Analysis only (DECISIONS "$E initialisers / atexit destructors ..."): no compiles, no source or
manifest changes. Base: main a4ed83025; target hash checked
(e6418111c2c36081a1795fb000e0927beea1630170b334c5013d12102602e70f). Addresses, counts and unit
names only. Calibration used: no.

## The table

- `__xc_a` is the null slot at the start of .data, 0x007C9000; the C++ initialiser pointers run
  from 0x007C9004 to 0x007CC868 (3,610 entries; the slot before `__xi_a` ends it), the same table
  `native/tools/initterm_manifest.py` reads. Every entry is a catalog row.
- Each entry was followed through the `_$E` functions it calls or jumps to; a `push` of a code
  address before a call to the CRT `atexit` (0x0078298A) names the destructor it registers.
  250 entries register a destructor (243 `atexit` call sites sit in `_$E` functions; 3 more
  destructors are registered from ordinary functions, below).
- Owner of an entry: the re-filed unit whose module map (`manifests/original_target/modules/*.json`,
  `code`) holds the entry, one of its chain functions or its destructor, else the unit whose
  source file the catalog row names. 2,565 entries are owned by 245 units; no entry has two owners.
  1,045 entries are unowned.

## Cross-checks

| Check | Result |
| --- | --- |
| Table order against .text order of the initialisers | strictly increasing, 3,609 of 3,609 steps: the table is laid down in .text (object) order |
| Owned units contiguous in the table | 245 of 245 units occupy one unbroken run |
| Owned entries against the Route A unit map (`unit_map.json`) | 2,565 of 2,565 agree |
| Data written by owned initialisers (.data/.bss targets), per unit | 244 unit blocks, none overlapping, ordered as in the table (0 inversions): data is also laid down per object in link order |
| Where a unit's initialisers sit in its .text | 244 of 245 units start with an initialiser (exception: overlay, whose module hull overlaps drive's); 238 end with ordinary code; 7 end with an initialiser (_command, _isotile, _logic, _mixfile, _palette, _rect, const) |
| Module-map text hulls | 5 overlapping neighbour pairs (cdfile/ccfile/audcache, overlay/drive, rawfile/random, wwmous/wwmouse); none touches a classification below |
| Final link map under build/ | `build/*/linked.map` is our relink, one `.text$<address>` section per pinned address: it restates the original addresses and is not independent link-order evidence. No original map exists |

Consequence: the table carries the same order as .text, so "between two entries of one unit" in
the table is the same statement as "between two functions of one unit" in .text. The
segmentation below therefore uses every module-map function as an anchor, not only the owned
initialisers. The data blocks are a second, independent order.

## Segmentation rules used

1. **Single, table**: the nearest owned anchors before and after (in table/.text order) are the
   same unit.
2. **Single, data**: the entry's data targets lie inside exactly one unit's data block (blocks do
   not overlap). All 36 such entries also lie between that unit's text anchors or next to them.
3. **Single, propagated**: rule 1 re-applied with the entries of rules 1-2 as anchors, provided
   no other unit's code lies between.
4. **Two**: the anchors are two different units and no non-initialiser function in between is
   given to a third unit by the unit map.
5. **Open (true group)**: a third unit has code between the anchors, or the unit map names a
   third unit for the entries themselves.

Within "two", three further facts are recorded, none of them byte evidence:
- *Prefix rule*: units start with their initialisers (244/245) and the previous unit ends in
  ordinary code, so the run is the next unit's prefix, unless the gap holds a whole
  initialiser-only unit (7 such units exist, so this cannot be excluded from the table).
- *Duplicate header static*: the previous unit already owns an initialiser of the same distinctive
  shape (a call or x87 load, addresses masked); one translation unit cannot instantiate a header
  static twice, so the previous unit is excluded.
- *Unit map*: whether `unit_map.json` names the pick for the entry.

A join cannot tell the two candidates apart by bytes: placed at the end of the previous unit or
at the start of the next, the initialiser lands at the same address, and both objects then give
one contiguous `.CRT$XCU` run and one contiguous data block. The audit accepts either. Only the
"single" bucket is byte evidence.

## Results

Entries (1,045 unowned): single 79 (table 26, data 36, propagated 17), two 783, open 183 (165 with
a third unit's code in the gap, 18 where the unit map names a third unit).

Address-named source files with `_$E` rows: **1,114** on a4ed83025 (J1 counted 1,113 at its base).
A file takes the class of the table entries its rows belong to (initialiser, chain function or
registered destructor).

| Class | Files | Where |
| --- | ---: | --- |
| Single evidenced unit | **94** | header_statics 51, unowned 36, globals 3, classes 2, wonline 2 (by rule: table 40, data 36, propagated 17, row already owned 1) |
| Two candidates | **828** | header_statics 679, classes 137, globals 6, unowned 4, top level 1, interpal 1 |
| True groups / open | **189** | header_statics 156, classes 15, unowned 12, globals 4, top level 2; 22 runs, 14 distinct candidate sets |
| Not reached from the table | 3 | function-local static destructors registered by ordinary functions: Tactical::Scroll_Map (0x006A8C70), Tactical::Scroll_Dir (0x006AA960), CChatEventSink::OnPublicGameOptions (0x00765980); unit map: tactical, wonline |

The 828 two-candidate files, split further:

| Sub-class | Files |
| --- | ---: |
| Previous unit excluded by a duplicate header static (pick = next unit) | 108 (unit map agrees 24, silent 84) |
| Prefix rule pick, unit map agrees | 293 |
| Prefix rule pick, unit map silent | 423 |
| Unit map names the other candidate (cd/cdcntrl 0x0046EA40, combuf/combuf2 0x00480B00, init/interpal 0x0051E7F0, txtlabel/txtlabel2 0x006F1810) | 4 |
| Of all 828, files with a single prefix pick | 822 (the 6 others: previous unit _command, _palette or _rect ends with an initialiser) |

Single-evidenced files by unit: wonline 28, mapgen 13, tracker 13, txtlabel2 12, isotype 11,
worlddom 8, _isotile 3, _map 2, wolpersonainformation 2, _mixfile 1, wolpersonainformation2 1.

The prefix picks for the 13-entry header-statics groups are the units J1 tried and found
"not emitted under the unit's state" (audevent, keyboard, lightcon, msglist, netdlg2, revent,
theme, txtlabel, wdtnet, coopcamp). The table agrees with that membership; what blocks them is
the unit's header state (the header defining these statics is not included), not membership.

## The wonline finding

J1: wonline's global-string initialisers (0x0073BCC0-0x0073C450) reproduce in wonline.cpp, but
the audit refuses the unit's second `.CRT$XCU` section because the pointers are not one
contiguous run, "evidence they are not wonline's". The table says the opposite:

- They are 14 initialisers (slots 0x007CC738-0x007CC774) with their 14 `atexit` destructors (28
  address-named files on this base).
- Before them: wonline's own run, 46 owned entries from 0x0073A6F0 (slot 0x007CC688) to 0x0073BCA0.
  Among them:
  wonline-owned 0x0073BD20 (slot 0x007CC73C) and 0x0073BFC0 (slot 0x007CC76C). After them: the
  next unit, worlddom, at 0x007683B0 (slot 0x007CC778). No other unit's entry or code is inside.
- Their data targets (0x00B269A8-0x00B2A724) lie inside wonline's data block (0x00B26570-0x00B2ABC8).

So the hypothesis does not fit: they do not fall inside someone else's run. They are inside
wonline's own run, interleaved with two entries wonline already owns. A second population is a
second object, and its `.CRT$XCU` contribution is one block. It cannot interleave with the
unit's own entries, so the audit refusal shows only that they belong in wonline's **own**
population (population-4248), in table order around 0x0073BD20 and 0x0073BFC0, not that they
belong to another unit. J1's note in ROUTE_A_OWN_JOINS.md should be corrected.

## Runs

Generated from the table at a4ed83025. "Files" counts address-named `_$E` files whose rows fall
in the run; a run with 0 files holds rows that already have address-free names.

### Single evidenced unit (15 runs)

| Initialisers | Entries | Files | Candidates | Pick |
| --- | ---: | ---: | --- | --- |
| 0x0040C670-0x0040C690 | 2 | 3 | _isotile | _isotile |
| 0x0040D0D0-0x0040D0D0 | 1 | 2 | _map | _map |
| 0x00483390-0x00483390 | 1 | 0 | const | const |
| 0x005283D0-0x00528550 | 11 | 11 | isotype | isotype |
| 0x0056D2D0-0x0056D4A0 | 13 | 13 | mapgen | mapgen |
| 0x005EDF60-0x005EDF60 | 1 | 0 | ownrdraw | ownrdraw |
| 0x006EC3A0-0x006EC570 | 13 | 13 | tracker | tracker |
| 0x006F1830-0x006F19E0 | 12 | 12 | txtlabel2 | txtlabel2 |
| 0x006F1A20-0x006F1AA0 | 5 | 0 | txtlabel2 | txtlabel2 |
| 0x00738640-0x00738640 | 1 | 2 | wolpersonainformation | wolpersonainformation |
| 0x0073A0F0-0x0073A0F0 | 1 | 1 | wolpersonainformation2 | wolpersonainformation2 |
| 0x0073BCC0-0x0073BCC0 | 1 | 2 | wonline | wonline |
| 0x0073BD80-0x0073BF80 | 11 | 22 | wonline | wonline |
| 0x0073C420-0x0073C450 | 2 | 4 | wonline | wonline |
| 0x00769410-0x007694D0 | 4 | 8 | worlddom | worlddom |


### Two candidates (76 runs)

| Initialisers | Entries | Files | Candidates (before, after) | Prefix pick |
| --- | ---: | ---: | --- | --- |
| 0x00403CE0-0x00403E90 | 12 | 12 | auddev, audevent | audevent |
| 0x0040BCE0-0x0040C0C0 | 2 | 4 | _command, _isotile |  |
| 0x0040D8D0-0x0040D8D0 | 1 | 1 | _palette, _rect |  |
| 0x0040D950-0x0040D950 | 1 | 1 | _rect, _rtti |  |
| 0x0040F720-0x0040F8F0 | 13 | 14 | _warhead, abstract | abstract |
| 0x0040FD00-0x0040FEF0 | 14 | 13 | _warhead, abstract | abstract |
| 0x00411400-0x00411400 | 1 | 0 | abuffer, ahandle | ahandle |
| 0x0041BAA0-0x0041BC70 | 13 | 13 | aircraft, airctype | airctype |
| 0x0041C3D0-0x0041C5A0 | 13 | 13 | airctype, aitrig | aitrig |
| 0x00423F70-0x00423F70 | 1 | 0 | anim, animfile | animfile |
| 0x00424C40-0x00424E10 | 13 | 13 | animfile, animtype | animtype |
| 0x00426910-0x00426AE0 | 13 | 13 | animtype, app | app |
| 0x00468AC0-0x00468C90 | 13 | 13 | cargo, carry | carry |
| 0x00468F30-0x00468F30 | 1 | 0 | category, ccfile | ccfile |
| 0x0046EA40-0x0046EA40 | 1 | 2 | cd, cdcntrl | cdcntrl |
| 0x0047D130-0x0047D130 | 1 | 0 | cell, checkbox | checkbox |
| 0x00480B00-0x00480B00 | 1 | 0 | combuf, combuf2 | combuf2 |
| 0x0048DC70-0x0048DE40 | 13 | 13 | convert, coopcamp | coopcamp |
| 0x00496D10-0x00496D10 | 1 | 0 | delay, dial8 | dial8 |
| 0x004981A0-0x004981A0 | 1 | 0 | dialog, dib | dib |
| 0x004A4DA0-0x004A4DA0 | 1 | 1 | drive, drop | drop |
| 0x004D2BA0-0x004D4920 | 78 | 136 | getcpu, globals | globals |
| 0x004F8F00-0x004F90D0 | 13 | 13 | house, houstype | houstype |
| 0x0051E7F0-0x0051E7F0 | 1 | 1 | init, interpal | interpal |
| 0x00522540-0x00522540 | 1 | 0 | ionblast, ipxaddr | ipxaddr |
| 0x00527890-0x00527A60 | 13 | 13 | ipxmgr, isotile | isotile |
| 0x00528380-0x005283A0 | 2 | 2 | isotile, isotype | isotype |
| 0x0052EF70-0x0052F140 | 13 | 13 | jshell, jumpjet | jumpjet |
| 0x00531360-0x00531530 | 13 | 13 | jumpjet, kamikaze | kamikaze |
| 0x00531F30-0x00532100 | 13 | 13 | kamikaze, keyboard | keyboard |
| 0x00538670-0x00538840 | 13 | 13 | lightcalc, lightcon | lightcon |
| 0x0056CEF0-0x0056D2B0 | 26 | 26 | map, mapgen | mapgen |
| 0x00593340-0x00593510 | 13 | 13 | mech, milsectmr | milsectmr |
| 0x0059D590-0x0059D760 | 13 | 13 | modemreg, motlib | motlib |
| 0x0059EDE0-0x0059EFB0 | 13 | 13 | motlib, mouse | mouse |
| 0x005A0060-0x005A0230 | 13 | 13 | mouse, movie | movie |
| 0x005A2610-0x005A27E0 | 13 | 13 | movies, mpcoop | mpcoop |
| 0x005AA830-0x005AAA00 | 13 | 13 | mpobserver, mpscore | mpscore |
| 0x005AB3B0-0x005AB580 | 13 | 13 | mpscore, mpsiege | mpsiege |
| 0x005B49E0-0x005B4BB0 | 13 | 13 | msgbox, msglist | msglist |
| 0x005BBB20-0x005BBCF0 | 13 | 13 | netdlg, netdlg2 | netdlg2 |
| 0x005DC3B0-0x005DC580 | 13 | 13 | overlay, overtype | overtype |
| 0x005DD320-0x005DD4F0 | 13 | 13 | overtype, ovrlight | ovrlight |
| 0x00608250-0x00608420 | 13 | 13 | parasite, particle | particle |
| 0x0060A880-0x0060AA50 | 13 | 13 | particle, partsys | partsys |
| 0x0060D0F0-0x0060D950 | 14 | 13 | partsys, phoneed | phoneed |
| 0x006351B0-0x006353A0 | 14 | 15 | radar, radbeam | radbeam |
| 0x00637030-0x00637220 | 14 | 15 | radio, ramfile | ramfile |
| 0x00639770-0x00639940 | 13 | 13 | readline, reinf | reinf |
| 0x0063AEA0-0x0063B070 | 13 | 13 | reinf, restate | restate |
| 0x0063B300-0x0063B590 | 15 | 15 | restate, revent | revent |
| 0x0063CB60-0x0063CB60 | 1 | 0 | revent, rgb | rgb |
| 0x0063CBC0-0x0063CC60 | 6 | 0 | revent, rgb | rgb |
| 0x006693A0-0x00669570 | 13 | 13 | script, scroll | scroll |
| 0x00675410-0x006755C0 | 12 | 12 | shastraw, ship | ship |
| 0x0067AC40-0x0067ADF0 | 12 | 12 | ship, side | side |
| 0x006846C0-0x006846C0 | 1 | 1 | skirmish, slider | slider |
| 0x00698740-0x00698740 | 1 | 0 | startup, statbtn | statbtn |
| 0x0069B170-0x0069B170 | 1 | 1 | straw, subtitle | subtitle |
| 0x006D9950-0x006D9B20 | 13 | 13 | techno, techtype | techtype |
| 0x006DFAB0-0x006DFC80 | 13 | 13 | techtype, teleport | teleport |
| 0x006E2E10-0x006E2FE0 | 13 | 13 | temporal, terrain | terrain |
| 0x006E4B70-0x006E4D40 | 13 | 13 | terrain, terrtype | terrtype |
| 0x006E7550-0x006E7720 | 13 | 13 | textbtn, theme | theme |
| 0x006EB2C0-0x006EB2C0 | 1 | 0 | toggle, tooltip | tooltip |
| 0x006ED030-0x006ED200 | 13 | 13 | tracker, trigger | trigger |
| 0x006F13D0-0x006F15A0 | 13 | 13 | tunnel, txtlabel | txtlabel |
| 0x006F1810-0x006F1810 | 1 | 1 | txtlabel, txtlabel2 | txtlabel2 |
| 0x0070C610-0x0070C7E0 | 13 | 13 | utracker, vanim | vanim |
| 0x0070DFB0-0x0070E180 | 13 | 13 | vanim, vanimtype | vanimtype |
| 0x00716D80-0x00716F10 | 8 | 10 | vox, voxdrsys | voxdrsys |
| 0x00728C60-0x00728E30 | 13 | 13 | wdtgameoptions, wdthistory | wdthistory |
| 0x00728FD0-0x007291A0 | 13 | 13 | wdthistory, wdtmap | wdtmap |
| 0x0072BBD0-0x0072BDA0 | 13 | 13 | wdtmap, wdtnet | wdtnet |
| 0x00734100-0x00734100 | 1 | 0 | weapon, wincomm | wincomm |
| 0x00736210-0x00736210 | 1 | 0 | windlg, winfix | winfix |


### Open: true groups (22 runs)

| Initialisers | Entries | Files | Candidates | Next unit (unit map) | Note |
| --- | ---: | ---: | --- | --- | --- |
| 0x00406900-0x00406AB0 | 12 | 12 | _alpha, audevent, audio | audio | unit map names a third unit for the entries |
| 0x0040AC80-0x0040AEA0 | 15 | 17 | _astar, _astar2, _command, drv_dsnd, voxdrsys | voxdrsys | |
| 0x0040B280-0x0040B450 | 13 | 14 | _astar, _astar2, _command, drv_dsnd, voxdrsys | _astar2 | |
| 0x0040BC60-0x0040BC60 | 1 | 0 | _astar, _astar2, _command, drv_dsnd, voxdrsys | _logic | |
| 0x0040D1E0-0x0040D1E0 | 1 | 2 | _map, _milsectmr, _mixfile | _mixfile | unit map names a third unit for the entries |
| 0x0040E3C0-0x0040E590 | 13 | 14 | _rules, _script, _warhead | _script | |
| 0x0040E970-0x0040F270 | 36 | 38 | _rules, _script, _warhead | _warhead | |
| 0x0040FCD0-0x0040FCD0 | 1 | 2 | _warhead, _winfix, abstract | abstract | unit map names a third unit for the entries |
| 0x0042FE50-0x0042FE50 | 1 | 1 | bfiofile, bfiofile2, binkhandle | bfiofile2 | unit map names a third unit for the entries |
| 0x00434920-0x00434920 | 1 | 0 | blight, blit, blowfish | blit | unit map names a third unit for the entries |
| 0x00467B40-0x00467D30 | 14 | 15 | campscor, capturemanager, cargo, disklaser | disklaser | |
| 0x00468770-0x00468940 | 13 | 13 | campscor, capturemanager, cargo, disklaser | cargo | |
| 0x0049EE80-0x0049EE80 | 1 | 1 | display, dllver, draw | draw | |
| 0x004B3580-0x004B35A0 | 2 | 3 | effects, egos, emember | egos | |
| 0x004B7A00-0x004B7BD0 | 13 | 13 | event, expand, facing | facing | |
| 0x004BB160-0x004BB330 | 13 | 13 | field, findpath, findpath2 | findpath | |
| 0x004BB6A0-0x004BB870 | 13 | 13 | field, findpath, findpath2 | findpath2 | |
| 0x004BBEE0-0x004BBEE0 | 1 | 0 | findpath2, flasher, fly | flasher | |
| 0x004FEF50-0x004FEF50 | 1 | 1 | house, hover, hsv | hsv | |
| 0x005B5DC0-0x005B5E60 | 3 | 4 | msglist, msgloop, mssfx, mstimer | msgloop | |
| 0x0063CB80-0x0063CBA0 | 2 | 0 | globals, revent, rgb | rgb | unit map names a third unit for the entries |
| 0x0066ACB0-0x0066AE80 | 13 | 13 | scroll, sendfile, serialed | sendfile | |

## Recommendation (for the new DECISIONS row)

1. **Single evidenced (94 files): join now** as ordinary own-population joins, under the
   existing rules (unit's options, exact whole-file build, strict audit). wonline's 28 go into
   population-4248 in table order around 0x0073BD20 and 0x0073BFC0, not as a second population.
   The /GR question J1 left open (00768260) is separate.
2. **Two candidates (828 files): join to the prefix-rule pick only with a manifest flag**
   (for example `unit_evidence: "xcu prefix"`, counted by STATUS and the strict audit under
   DECISIONS (d)), because bytes cannot tell the end of the previous unit from the start of the
   next. Order: the 108 where a duplicate header static excludes the previous unit, then the 293
   where the unit map agrees, then the 423 where it is silent. The 4 where the unit map names
   the other candidate and the 6 with no pick stay. For the 13-entry header-statics groups,
   the join is a source-state task: give the unit the header that defines these statics, as one
   include-block change per unit. It is not a membership question.
3. **Open (189 files, 22 runs): stay** in their header_statics/unowned groups as true groups.
   Re-run this classification after each unit is re-filed: a re-filed third unit turns its
   gap into "single" or "two".
4. **The 3 function-local destructors** are outside the table. They move with the function that
   registers them (tactical, wonline) when it joins.
5. Correct the wonline note in ROUTE_A_OWN_JOINS.md (Lane J1): the table puts those initialisers
   inside wonline's run.

The classification is cheap (seconds) and read-only. If the row adopts it, it can be made a
`tools/original_target` report (byte-free output) and re-run at each landing.
