# Original-target status

Generated numbers below come from `tools/original_target/status.py --write`
(canonical manifest plus its git history); edit only the text outside the
markers. The decompilation is not finished: the table says how much of the
original is reproduced from source and what is still taken from the original;
the native port is in progress. Per-batch evidence through A15 is in
[history/](history/); current work is in [HANDOFF.md](HANDOFF.md).

<!-- generated:status -->

| Tree (built and gated separately; numbers side by side, never added) | `src/` (exact-match record) | `rebuild/` (clean tree in progress; last gated landing 2026-11-22T23:22Z) |
| --- | ---: | ---: |
| Reproduced .text (both trees: C/C++ body + evidenced inline asm + frozen padding + linked library members + assembled handwritten asm, the five lines below) | 93.54% (3,526,174 B) | **90.93% function-level** (3,428,049 B) (90.34% whole-unit, 3,405,521 B) |
| C/C++ bodies matched (body bytes: no padding, no evidenced inline asm; the parity line) | 3,274,492 B, 86.86% | **3,178,277 B, 84.31% function-level** (3,156,290 B, 83.73% whole-unit); gap to main 96.2 KB (118.2 KB whole-unit) |
| Evidenced inline assembly (cited TibSun `__asm`; not C/C++ credit) | 2,514 B | 2,555 B in 8 rows (whole-unit) |
| Frozen padding of accepted rows | 143,042 B | 141,091 B function-level (140,550 B whole-unit) |
| Linked library members (the same archive members in both builds) | 89,127 B | 89,127 B |
| Assembled handwritten assembly (the same adopted TibSun .asm procedures in both builds) | 16,999 B | 16,999 B |
| C/C++ bodies, exact with no judgment call (each tree's judgment rows left out) | 3,245,325 B, 86.09% | **3,057,112 B, 81.095% function-level** (3,041,261 B, 80.674% whole-unit) of .text; left out: judgment calls 65 rows / 115,029 B (of which header-form dependents 7 / 23,173 B), count-sensitive 3 rows / 6,136 B |
| Judgment calls, one definition for both trees (main's: the manifest flags count_calibrated, split_declaration, declarations_only; in the rebuild, the accepted rows whose provenance.csv judgment note, or a header_judgments.csv form they depend on, names one of those flags; whole-unit, body bytes) | 10 rows / 29,167 B | 1 rows / 4,617 B |
| Judgment calls, each tree's own count (not comparable row for row: main counts the manifest flags above; the rebuild counts every provenance.csv judgment note, which includes twin-text forms and bounds, plus the rows listed as dependents of a flagged header form; body bytes) | 10 rows / 29,167 B | 65 rows / 115,029 B |
| Accepted | 17,910 regions | 17,560 functions in 438 units; function-level 17,644 functions |
| Rebuild provenance (rebuild/README.md; body plus padding) | | adopted 16,669 fn / 2,844,975 B; reshaped from src/ 178 / 181,396 B; fresh 163 / 19,920 B; Windows workers 550 / 253,104 B |
| Rebuild structure | | duplicates 0, externs 6 landed / 6 all, address-suffixed 0; parked 169 functions / 167,155 B (>=1 KB: 47 / 151,359 B); judgment 58 rows / 91,856 B; count-sensitive 3 rows / 6,136 B |
| Rebuild evidenced inline assembly reached through a file-static helper of its unit | | 2 rows / 789 B |
| Rebuild asm bytes not yet flagged (main flags them; header-template attribution to fix) | | 0 rows / 0 B |

| Measure | Value |
| --- | ---: |
| **Reproduced .text** (built from real inputs, not copied from the original: C/C++ source + evidenced inline asm + frozen padding + linked library members + assembled handwritten asm; 100% when done) | **3,526,174 of 3,769,805 (93.54%)** |
| Judgment-call bytes (accepted rows resting on a flagged, unprovable choice: count_calibrated, split_declaration, declarations_only; read from the manifest) | 29,167 in 10 regions (0.77% of .text) |
| Accepted regions | 17,910 |
| Matched C/C++ body bytes | 3,274,492 (86.86% of .text) |
| Count-calibrated C/C++ bytes (rows whose named locals were chosen by count calibration under the user's three conditions, 2026-11-09; own category, included in the matched C/C++ bytes above) | 14,606 in 7 regions (0.39% of .text) |
| Evidenced inline assembly body bytes (cited TibSun `__asm`; own category beside library code, not C/C++ credit) | 2,514 (0.07% of .text) |
| Frozen padding bytes | 143,042 |
| Module form (sources in original translation-unit form, no addresses; manifests/original_target/modules/) | 406 module(s): 15,609 functions, 3,186,578 code bytes; **2,086 data bytes compiler-emitted and proven at their original addresses** in 43 sections, 345,465 bytes of vtable/RTTI copies kept elsewhere; 38 header gaps (withheld template details, modules.json header_gaps; no bytes) in 17 module(s); own ledger, no .text credit (_command, _isotile, _logic, _map, _mixfile, _palette, _rect, _rtti, _rules, _warhead, abstract, abstype, abuffer, ahandle, ahandle2, aircraft, airctype, aitrig, alphalighting, alphashp, anim, animfile, animtype, app, astar, audattr, audbag, audcache, audchanl, auddev, audevent, audio, audlists, audplay, audsrc, audstrm, audtimer, b64pipe, b64straw, bar, base, base64, beacon, bench, bfiofile, bfiofile2, bitfont, bittext, blight, blit, blowfish, blowpipe, blwstraw, bomb, bounce, brain, buff, buffer, building, builtype, bullet, bullettype, campaign, campscor, cargo, carry, category, ccfile, ccini, cctooltip, cd, cdcntrl, cdfile, cell, checkbox, cheklist, colrlist, combat, combuf, combuf2, connect, conquer, const, control, convert, coopcamp, coord, copyprotection, counter, cpoint, crate, crc, crcpipe, crcstraw, credits, cstraw, cstream, data, ddraw, delay, dial8, dialog, dib, diskswap, display, draw, drive, drop, droppod, dropship, drv_dsnd, dsurface, ebolt, edit, effects, emember, empulse, enviro, event, facing, factory, fastmath, field, findpath2, flasher, fly, fog, foot, fuse, gadget, gamedlg, gametime, gauge, gdlgsupp, getcd, getcpu, globals, goptions, grphmenu, grphmenu2, grphmimg, grphmitm, grphmsct, gscreen, hicolor, house, houstype, hover, hsv, infantry, infatype, ini, init, interpal, intro, ion, ionblast, ipxaddr, ipxconn, ipxgconn, ipxmgr, isotile, isotype, jshell, jumpjet, kamikaze, keyboard, laser, layer, lcwpipe, lcwstraw, ldprgmgr, light, lightcalc, lightcon, link, link2, list, loaddlg, loco, logic, lzo1x_c, lzo1x_d, lzopipe, lzostraw, mainloop, mainopt, map, mapgen, mapsel, matrix3d, mech, milsectmr, missile, mission, mixfile, modemgst, modemhst, modemreg, motlib, mouse, movie, movies, mpcoop, mpfreeforall, mplayer, mpmath, mpobserver, mpscore, mpsiege, mpsiegeteam, mpu, mpunholyalliance, msanim, mschoice, msengine, msfont, msgbox, msglist, mstimer, multiplayer, multiplayerteam, multiplayerteam2, netdlg, netdlg2, netshare, nullconn, nulldlg, nullmgr, object, objtype, options, overlay, overtype, ovrlight, ownrdraw, packet, palette, parasite, particle, partsys, phoneed, pipe, pixelfx, pk, pkpipe, pkstraw, planmgr, polygon, power, preview, progress, psystype, ptype, quat, queue, radar, radbeam, radio, ramfile, random, rawfile, rc_ptr_base, readline, reinf, restate, revent, rgb, rle, rndstraw, rocket, rocket2, rules, saveload, savever, scenario, scheme, score, script, scroll, serialed, session, sha, shapebtn, shapipe, shastraw, ship, side, sidebar, skirmish, slider, smartdeform, smudge, smudtype, sounddlg, spawnmanager, special, sprite, srfcache, startup, statbtn, stats, stbuffer, stimer, stlscore, storage, straw, subtitle, super, suprtype, swizzle, tab, tactical, taction, tag, tagtype, target, taskforc, team, teamtype, techno, techtype, teleport, temporal, terrain, terrtype, tevent, textbtn, theme, tiberium, tmission, toggle, tooltip, tracker, trigger, trigtype, trim, tube, tunnel, txtlabel, txtlabel2, uicmnds, uicmnds2, unit, unittype, unvqtblc, utracker, vanim, vanimtype, vector, vein, version, veteran, voc, volinfo, vox, voxdrsys, voxel, voxlib, voxpal, vqa, vqalib_audio, vqalib_buffer_, vqalib_drawer, vqalib_dstream, vqalib_loader, vqalib_task, vqalib_timer, vqalib_unvq, vqalib_unvqtran, vqoption, walk, warhead, wave, waypoint, wdtcampaign, wdtconflict, wdtgameoptions, wdthistory, wdtmap, wdtnet, wdtprops, wdtsel, wdtstate, wdtterr, wdtvoice, weapon, wincomm, windlg, winfix, winstub, wolpersonainformation, wolpersonainformation2, wonline, worlddom, writepcx, wspipx, wsproto, wspudp, wstring, wwmous, wwmouse, xpipe, xstraw, xsurface, zbuffer) |
| Route A units (one source per original translation unit under its own header state, refile_unit.py; part of the Module form line) | 376 unit(s): 15,355 functions, 3,161,442 code bytes; residual rows in their own files: **split 1,246 rows, 118,909 bytes** (held by another frozen population; the known ceiling of what joining can still move), **flipped 15 rows, 1,367 bytes** (not exact when moved), kept by rule 13 rows, 4,078 bytes; 686 per-unit switches kept; 5,762 stand-in references from other units. Split rows by cause (read from each row's file and its holding population, no compile): template members 399 rows, 32,983 bytes (the file sets a container implementation switch such as VECTOR_IMPLEMENT_* or instantiates a template); needs another unit's header state 272 rows, 34,450 bytes (another switch, another include or ghidra_types.hpp); stand-ins 85 rows, 12,915 bytes (placeholder-form rows); emitted by another source 250 rows, 10,619 bytes (comment-only files: an inline constructor, dynamic initialiser or compiler-generated destructor another source emits; moves only with its emitter); held by a population that compiles each source alone 92 rows, 7,323 bytes (the holding population compiles each source alone); other option set 88 rows, 8,523 bytes (compiled under another option set than the unit's group); the unit's own second population 34 rows, 8,811 bytes (every row of the holding population is this unit's: a second compile group a join would empty); no bound in the unit's frozen pass 26 rows, 3,285 bytes (a mixed population; the unit's frozen inventory pass gives no equal bound); not yet classified 0 rows, 0 bytes (nothing in the text or the holding population decides). |
| Rows bound by their own frozen population (joined into a unit's translation unit; bound, gap padding and provenance from `bound_population`'s pass) | 3,040 rows, 195,941 bytes, 167 donor populations |
| Rows joined under the unit's /GR (frozen without /GR; every RTTI structure the unit emits still proven at its original address; manifest note "joined under the unit's /GR") / under the unit's options (frozen under another option set; the unit's set evidenced by its own rows; manifest note "joined under the unit's options (frozen under X)") | /GR: 1,051 rows, 64,229 bytes, 141 units; options: 3 rows, 182 bytes, 2 units |
| `$E` rows joined to the one unit the .CRT$XCU table evidences (no flag; manifest note "xcu single") | 81 rows, 2,031 bytes, 9 units |
| `$E` rows joined to a .CRT$XCU boundary pick, flag `unit_evidence: xcu-exclusion` (the previous unit already owns that header static; bytes cannot tell the two candidates apart) | 172 rows, 4,857 bytes |
| `$E` rows joined to a .CRT$XCU boundary pick, flag `unit_evidence: xcu-prefix+map` (prefix rule and unit map agree; bytes cannot tell the two candidates apart) | 77 rows, 2,113 bytes |
| `$E` rows joined to a .CRT$XCU boundary pick, flag `unit_evidence: xcu-prefix` (prefix rule alone; bytes cannot tell the two candidates apart) | 195 rows, 5,340 bytes |
| Address-named source files remaining (collision / seed name; rename_files.py) | 121 files: collision 12, seed name 91, not yet renamed 0, other 18 (no unit or folder spelling, not one accepted row) |
| Files depending on VECTOR_DECLARATIONS_ONLY (container members left undefined; per file, no bytes counted) | 4 (Constructor.cpp, Resize.cpp, gamedlg.cpp, ownrdraw.cpp) |
| Data bound by enclosed section layout (flag data_bound_by: layout; the split into declarations is a hypothesis) | 20 symbols in 1 source(s); 404 bytes in the proven sections that hold them |
| Data bound by bss elimination (flag data_bound_by: elimination; earns nothing by itself) | 1 symbols in 1 source(s) |
| Generated data modules (tracked generator, no original bytes in the repository; each proven at build time as one whole block at its original addresses; not C/C++ credit) | 1 module(s), 155,712 bytes; 1 flagged constant(s) (fastmath_tables) |
| C runtime / library bytes (.text >= 0x781000; library ledger, not source credit) | 87,989 (2.33% of .text; 110 matched) |
| Linked library bytes (identified archive members built from the pinned SP3 LIBCMT/COMSUPP and VC98 import libraries; manifests/original_target/library.json; not source credit) | 89,127 in 899 sections (2.36% of .text) |
| Handwritten assembly reproduced from adopted TibSun .asm sources (manifests/original_target/assembly.json; own ledger) | 16,999 in 51 procedures |
| Handwritten blitter/codec assembly bytes (9 owner-ruled ranges plus evidenced inline asm; own ledger) | 23,156 (0.61% of .text) |
| In-scope matched C/C++ bytes (of .text minus CRT, handwritten assembly and in-scope padding) | 3,274,382 of 3,515,642 (93.14%) |
| In-scope functions matched (game code below 0x781000, handwritten ranges and known false rows excluded) | 17,892 of 17,986 (99.48%) |
| Partial-match average (unmatched functions at best recorded score, 0 if none, sized by inventory bounds; 90 of 94 scored) | 98.92% by bytes, 99.87% per function |
| In-scope functions with source (exact or draft: accepted rows plus non-exact drafts in source) | 17,983 of 17,986 (99.98%) |
| Non-exact drafts in source (#ifdef RA2_NON_MATCHING, manifests/original_target/non_exact.json; never compiled by the exact build; no .text credit) | 96 functions, 181,989 bytes, mean score 68.49 |
| Structure: class/struct names defined more than once under src/ (cleanup_census.py --structure (a)) | 155 of 1,986 names (276 extra definitions) |
| Structure: files under src/recovered/ outside their unit's file ((c); a one-function file holds one catalog row) | 1,967 (1,962 one-function) beside 406 unit files |
| Structure: functions free or on a stand-in class where a reference has a class member ((d); mischa85 by address, ra2-decomp/TibSun by unit-scoped name) | 490 rows, 56,890 bytes (free 275, stand-in 215) |
| Structure: address-suffixed identifiers ((e)) | 7,194 distinct, 36,015 occurrences in 1,033 files |
| Structure: RA2_ preprocessor switches ((b); include guards excluded) | 260 distinct in 199 files |
| Structure: "// @ref" comment lines ((f)) | 0 in 0 files |
| Structure: extern declarations in .cpp files ((g)) | 4,741 lines, 2,614 names, 486 files |
| Original .text scaffold bytes | 243,631 |
| Logical .text bytes | 3,769,805 |
| Compiler populations | 603 |
| Compile path (build environment, with the pinned VC6 SP3 8447 compiler; docs/ORIGINAL_TARGET_BUILD.md "Fixed compile folder") | `<drive>:\private\tmp\ra2c\XXXXXXXX\gNNNN\accepted.cpp; /I <drive>:\private\tmp\ra2c\XXXXXXXX\src; TMP <drive>:\private\tmp\ra2c\XXXXXXXX\tmp-N` |
| Populations under /GR (docs/DECISIONS.md "/GR per population") | 342 of 603 |
| Non-code bytes proven at their original addresses (module maps, deduplicated by address; not .text credit) | 224,129 of 711,662 file-backed (31.5%); RTTI 76,039, vtables 37,244, strings 93,930; 12 class hierarchy descriptor(s) differ from the original only in the ambiguous bit (known difference; the attributes word is not counted) |

Manifest history (body bytes added per commit, and per hour since the previous commit):

| Commit | Time | Regions | Body bytes | +Body | +Body/hour |
| --- | --- | ---: | ---: | ---: | ---: |
| `d190902d63` | 2026-10-20 13:39 -0500 | 6,763 | 215,781 | +0 |  |
| `d7358c5179` | 2026-10-20 13:55 -0500 | 6,780 | 217,362 | +1,581 | 6,016 |
| `836edc7bc3` | 2026-10-20 14:35 -0500 | 6,780 | 217,362 | +0 | 0 |
| `f0f7e6f1df` | 2026-10-20 14:49 -0500 | 6,780 | 217,362 | +0 | 0 |
| `b2719d3514` | 2026-10-20 15:33 -0500 | 6,799 | 219,720 | +2,358 | 3,240 |
| `6663b8487a` | 2026-10-20 15:57 -0500 | 6,806 | 220,115 | +395 | 989 |
| `b0278f4ab4` | 2026-10-20 16:49 -0500 | 6,823 | 222,182 | +2,067 | 2,352 |
| `2fab41ad08` | 2026-10-20 17:30 -0500 | 6,834 | 222,597 | +415 | 616 |
| `dcf425b57c` | 2026-10-20 19:35 -0500 | 6,862 | 223,218 | +621 | 298 |
| `e121360434` | 2026-10-20 20:58 -0500 | 7,199 | 235,303 | +12,085 | 8,693 |
| `187f802c0f` | 2026-10-20 21:22 -0500 | 7,254 | 237,333 | +2,030 | 5,136 |
| `9d73334884` | 2026-10-20 21:25 -0500 | 7,254 | 237,333 | +0 | 0 |
| `dbf699de16` | 2026-10-20 21:39 -0500 | 7,254 | 237,333 | +0 | 0 |
| `d18cabe4ef` | 2026-10-20 21:44 -0500 | 7,377 | 241,610 | +4,277 | 60,145 |
| `65583dfbbd` | 2026-10-20 21:47 -0500 | 7,377 | 241,610 | +0 | 0 |
| `6c40be4ab4` | 2026-10-20 21:51 -0500 | 7,377 | 241,610 | +0 | 0 |
| `4cf175c85b` | 2026-10-20 22:04 -0500 | 7,383 | 242,123 | +513 | 2,332 |
| `57909998f3` | 2026-10-20 22:32 -0500 | 7,389 | 243,551 | +1,428 | 3,033 |
| `7612578ccb` | 2026-10-20 22:49 -0500 | 7,399 | 246,026 | +2,475 | 8,804 |
| `d95eb5bf34` | 2026-10-20 23:07 -0500 | 7,404 | 246,336 | +310 | 1,017 |
| `4580f4ccb0` | 2026-10-20 23:13 -0500 | 7,408 | 246,801 | +465 | 5,264 |
| `85f9025f2a` | 2026-10-20 23:15 -0500 | 7,418 | 247,296 | +495 | 11,208 |
| `9e5ac425eb` | 2026-10-20 23:20 -0500 | 7,422 | 247,687 | +391 | 5,393 |
| `0a5b2876f7` | 2026-10-20 23:24 -0500 | 7,424 | 247,910 | +223 | 2,867 |
| `5a114bb5d5` | 2026-10-20 23:26 -0500 | 7,452 | 249,040 | +1,130 | 47,302 |
| `5cca26510d` | 2026-10-20 23:28 -0500 | 7,456 | 249,206 | +166 | 4,208 |
| `a35fc6ecaf` | 2026-10-20 23:35 -0500 | 7,457 | 249,246 | +40 | 337 |
| `65aebb75ee` | 2026-10-20 23:53 -0500 | 7,468 | 250,441 | +1,195 | 3,972 |
| `44241b663c` | 2026-10-20 23:58 -0500 | 7,773 | 259,703 | +9,262 | 133,373 |
| `79af10501b` | 2026-10-21 00:08 -0500 | 7,780 | 260,022 | +319 | 1,876 |
| `6682b883a5` | 2026-10-21 00:14 -0500 | 7,785 | 260,325 | +303 | 2,972 |
| `d76493d708` | 2026-10-21 00:52 -0500 | 7,821 | 266,958 | +6,633 | 10,418 |
| `018c92fa63` | 2026-10-21 02:11 -0500 | 8,152 | 320,686 | +53,728 | 41,040 |
| `936080b1fa` | 2026-10-21 02:36 -0500 | 8,180 | 324,224 | +3,538 | 8,418 |
| `8a5a33e3a6` | 2026-10-21 03:02 -0500 | 8,205 | 327,618 | +3,394 | 7,919 |
| `76ccacd03c` | 2026-10-21 03:14 -0500 | 8,714 | 337,925 | +10,307 | 51,392 |
| `6149997bc5` | 2026-10-21 03:28 -0500 | 8,714 | 337,925 | +0 | 0 |
| `4a437520ea` | 2026-10-21 04:21 -0500 | 8,715 | 338,363 | +438 | 496 |
| `0827c550fd` | 2026-10-21 04:50 -0500 | 8,914 | 352,652 | +14,289 | 29,907 |
| `33a2ba5c53` | 2026-10-21 05:04 -0500 | 8,957 | 360,863 | +8,211 | 34,133 |
| `a5590df0b6` | 2026-10-21 05:35 -0500 | 9,001 | 367,151 | +6,288 | 11,971 |
| `e1e85080f6` | 2026-10-21 05:41 -0500 | 9,110 | 372,926 | +5,775 | 59,570 |
| `597623b073` | 2026-10-21 06:00 -0500 | 9,113 | 373,927 | +1,001 | 3,218 |
| `dfdf7b74ef` | 2026-10-21 06:38 -0500 | 9,167 | 383,078 | +9,151 | 14,342 |
| `452ad1e335` | 2026-10-21 06:47 -0500 | 9,228 | 384,605 | +1,527 | 10,592 |
| `63658ec6b2` | 2026-10-21 07:06 -0500 | 9,240 | 386,488 | +1,883 | 5,804 |
| `db83372a09` | 2026-10-21 07:25 -0500 | 9,256 | 388,554 | +2,066 | 6,635 |
| `33a6599985` | 2026-10-21 07:42 -0500 | 9,265 | 390,420 | +1,866 | 6,560 |
| `c002ae63f7` | 2026-10-21 07:50 -0500 | 9,283 | 393,199 | +2,779 | 22,085 |
| `f5184674e8` | 2026-10-21 07:51 -0500 | 9,307 | 396,794 | +3,595 | 147,068 |
| `5d9524effb` | 2026-10-21 07:57 -0500 | 9,307 | 396,794 | +0 | 0 |
| `4e6db4d83a` | 2026-10-21 08:21 -0500 | 9,341 | 398,363 | +1,569 | 4,043 |
| `902a2564ab` | 2026-10-21 08:52 -0500 | 9,358 | 403,906 | +5,543 | 10,688 |
| `8407b496ea` | 2026-10-21 09:06 -0500 | 9,358 | 403,906 | +0 | 0 |
| `11b32cffa0` | 2026-10-21 09:15 -0500 | 9,367 | 405,481 | +1,575 | 10,539 |
| `a27ece80d5` | 2026-10-21 09:24 -0500 | 9,391 | 409,993 | +4,512 | 28,749 |
| `b6fc74acbe` | 2026-10-21 09:45 -0500 | 9,425 | 412,492 | +2,499 | 7,073 |
| `fe4107b2fe` | 2026-10-21 10:19 -0500 | 9,440 | 413,363 | +871 | 1,554 |
| `c5fcf9a5c0` | 2026-10-21 10:28 -0500 | 9,445 | 413,894 | +531 | 3,527 |
| `6cdb69eda9` | 2026-10-21 11:01 -0500 | 9,567 | 417,526 | +3,632 | 6,564 |
| `6ad4a1d077` | 2026-10-21 11:29 -0500 | 9,625 | 423,150 | +5,624 | 12,315 |
| `e76f1b36b2` | 2026-10-21 13:37 -0500 | 9,655 | 424,812 | +1,662 | 779 |
| `573bf032dd` | 2026-10-21 13:49 -0500 | 9,675 | 426,328 | +1,516 | 7,446 |
| `906a2d3ba5` | 2026-10-21 14:02 -0500 | 9,753 | 433,200 | +6,872 | 32,046 |
| `cbf491aed3` | 2026-10-21 16:22 -0500 | 9,803 | 438,792 | +5,592 | 2,401 |
| `0fd8a82df5` | 2026-10-21 16:34 -0500 | 9,860 | 449,855 | +11,063 | 54,408 |
| `b9e76d9a7e` | 2026-10-21 18:46 -0500 | 10,037 | 472,010 | +22,155 | 10,056 |
| `81b25a9197` | 2026-10-21 21:03 -0500 | 10,265 | 504,641 | +32,631 | 14,308 |
| `56486634f5` | 2026-10-22 00:45 -0500 | 10,610 | 550,916 | +46,275 | 12,508 |
| `20c716a73c` | 2026-10-22 02:15 -0500 | 10,890 | 578,428 | +27,512 | 18,206 |
| `e6294233a8` | 2026-10-22 04:29 -0500 | 11,011 | 597,742 | +19,314 | 8,671 |
| `2bed1e625c` | 2026-10-22 06:49 -0500 | 11,048 | 604,311 | +6,569 | 2,808 |
| `7fdfe50077` | 2026-10-22 07:42 -0500 | 11,315 | 618,802 | +14,491 | 16,598 |
| `3e5e170f80` | 2026-10-22 08:46 -0500 | 11,460 | 633,466 | +14,664 | 13,798 |
| `0ea2fad033` | 2026-10-22 13:26 -0500 | 11,580 | 676,828 | +43,362 | 9,289 |
| `e20cc85f90` | 2026-10-22 14:06 -0500 | 11,632 | 684,982 | +8,154 | 12,030 |
| `e79fdf0cbc` | 2026-10-22 16:38 -0500 | 11,676 | 689,804 | +4,822 | 1,909 |
| `a417a47097` | 2026-10-22 20:52 -0500 | 11,835 | 716,005 | +26,201 | 6,194 |
| `9d2c78d7b0` | 2026-10-22 21:31 -0500 | 11,865 | 719,903 | +3,898 | 5,881 |
| `c20aeffed7` | 2026-10-22 22:06 -0500 | 11,873 | 722,074 | +2,171 | 3,741 |
| `a75c4968ac` | 2026-10-22 22:44 -0500 | 11,910 | 727,907 | +5,833 | 9,379 |
| `a3c43c6b78` | 2026-10-22 23:36 -0500 | 11,920 | 729,055 | +1,148 | 1,321 |
| `7135160356` | 2026-10-23 01:03 -0500 | 11,936 | 732,118 | +3,063 | 2,107 |
| `1f96d72fc8` | 2026-10-23 04:24 -0500 | 11,961 | 735,310 | +3,192 | 951 |
| `d33284dea5` | 2026-10-23 04:59 -0500 | 11,973 | 737,767 | +2,457 | 4,214 |
| `265d007fb1` | 2026-10-23 05:24 -0500 | 11,974 | 737,810 | +43 | 104 |
| `4b1024e502` | 2026-10-23 05:52 -0500 | 11,984 | 739,885 | +2,075 | 4,457 |
| `531e0d71e3` | 2026-10-23 10:42 -0500 | 12,078 | 760,540 | +20,655 | 4,272 |
| `7ac82c9093` | 2026-10-23 12:07 -0500 | 12,136 | 767,946 | +7,406 | 5,232 |
| `43a1e0eb48` | 2026-10-23 13:50 -0500 | 12,138 | 768,887 | +941 | 548 |
| `89a0c72120` | 2026-10-23 15:30 -0500 | 12,139 | 769,101 | +214 | 128 |
| `87866beab9` | 2026-10-23 17:15 -0500 | 12,176 | 775,397 | +6,296 | 3,605 |
| `b9e09bef33` | 2026-10-23 18:30 -0500 | 12,220 | 781,206 | +5,809 | 4,632 |
| `9efbd23cc8` | 2026-10-23 20:38 -0500 | 12,242 | 786,740 | +5,534 | 2,598 |
| `11ccc7f3bc` | 2026-10-23 21:26 -0500 | 12,253 | 788,477 | +1,737 | 2,174 |
| `2673879eab` | 2026-10-23 22:15 -0500 | 12,265 | 790,147 | +1,670 | 2,072 |
| `f55afac9c7` | 2026-10-23 22:48 -0500 | 12,312 | 798,763 | +8,616 | 15,571 |
| `ccf4843b26` | 2026-10-23 23:07 -0500 | 12,324 | 801,628 | +2,865 | 9,063 |
| `46c8cdb4aa` | 2026-10-24 02:13 -0500 | 12,421 | 814,363 | +12,735 | 4,112 |
| `c1dd64d18e` | 2026-10-24 06:41 -0500 | 12,427 | 816,336 | +1,973 | 442 |
| `00a5f0beb6` | 2026-10-24 07:24 -0500 | 12,575 | 844,713 | +28,377 | 39,186 |
| `15fdd70ce9` | 2026-10-24 13:36 -0500 | 12,576 | 845,313 | +600 | 97 |
| `811a804acc` | 2026-10-24 14:43 -0500 | 12,576 | 845,313 | +0 | 0 |
| `1d952a232e` | 2026-10-24 15:04 -0500 | 12,584 | 846,287 | +974 | 2,783 |
| `f5e00e6c68` | 2026-10-24 17:30 -0500 | 12,584 | 846,287 | +0 | 0 |
| `347a386110` | 2026-10-24 18:23 -0500 | 12,845 | 906,375 | +60,088 | 67,117 |
| `b8e76d5f6b` | 2026-10-24 19:02 -0500 | 12,868 | 916,800 | +10,425 | 16,135 |
| `ea4ad3c0e3` | 2026-10-24 19:48 -0500 | 12,875 | 918,595 | +1,795 | 2,337 |
| `5a4e2a2f2c` | 2026-10-24 20:25 -0500 | 12,881 | 919,689 | +1,094 | 1,773 |
| `deeb4e8a4d` | 2026-10-24 21:42 -0500 | 12,884 | 920,111 | +422 | 329 |
| `d0e52aa0c8` | 2026-10-24 22:42 -0500 | 12,894 | 923,206 | +3,095 | 3,108 |
| `7570d6d076` | 2026-10-24 23:14 -0500 | 12,897 | 926,403 | +3,197 | 5,976 |
| `efe2154aa9` | 2026-10-24 23:49 -0500 | 12,909 | 929,122 | +2,719 | 4,648 |
| `b688a677d4` | 2026-10-25 00:24 -0500 | 12,910 | 929,639 | +517 | 891 |
| `9250019332` | 2026-10-25 00:54 -0500 | 12,911 | 930,116 | +477 | 949 |
| `484687375b` | 2026-10-25 01:47 -0500 | 12,912 | 930,800 | +684 | 778 |
| `1a258ca446` | 2026-10-25 02:46 -0500 | 12,913 | 931,430 | +630 | 642 |
| `55d6da51d1` | 2026-10-25 03:26 -0500 | 12,914 | 931,679 | +249 | 374 |
| `aec7efa4b3` | 2026-10-25 04:44 -0500 | 12,928 | 936,980 | +5,301 | 4,059 |
| `059a996140` | 2026-10-25 05:24 -0500 | 12,930 | 937,365 | +385 | 578 |
| `bcfcd20e8a` | 2026-10-25 06:05 -0500 | 12,934 | 938,879 | +1,514 | 2,200 |
| `7f847ca0d1` | 2026-10-25 06:39 -0500 | 12,935 | 939,674 | +795 | 1,421 |
| `dfa6721358` | 2026-10-25 07:44 -0500 | 12,938 | 941,233 | +1,559 | 1,428 |
| `b76a2aa615` | 2026-10-25 08:25 -0500 | 12,939 | 941,590 | +357 | 526 |
| `a61bd64997` | 2026-10-25 10:20 -0500 | 12,948 | 942,928 | +1,338 | 697 |
| `59331472ae` | 2026-10-25 10:46 -0500 | 12,956 | 943,434 | +506 | 1,202 |
| `120ed5d30b` | 2026-10-25 11:28 -0500 | 12,959 | 944,255 | +821 | 1,161 |
| `ee8101fb49` | 2026-10-25 13:03 -0500 | 12,959 | 944,255 | +0 | 0 |
| `c854c71812` | 2026-10-25 13:39 -0500 | 12,961 | 945,193 | +938 | 1,534 |
| `e8ee708ea6` | 2026-10-25 14:31 -0500 | 12,961 | 945,193 | +0 | 0 |
| `5d9681ab38` | 2026-10-25 15:30 -0500 | 12,963 | 945,369 | +176 | 177 |
| `29c31b59e3` | 2026-10-25 16:07 -0500 | 12,966 | 946,105 | +736 | 1,189 |
| `cec6c563cb` | 2026-10-25 16:30 -0500 | 12,971 | 946,656 | +551 | 1,490 |
| `937689aadd` | 2026-10-25 17:26 -0500 | 12,972 | 947,042 | +386 | 411 |
| `d4cd695d66` | 2026-10-25 18:01 -0500 | 12,982 | 949,799 | +2,757 | 4,673 |
| `5626a9db9e` | 2026-10-25 18:32 -0500 | 12,995 | 953,751 | +3,952 | 7,653 |
| `23e8d3513f` | 2026-10-25 19:34 -0500 | 12,996 | 953,934 | +183 | 179 |
| `c932a60b1b` | 2026-10-25 20:04 -0500 | 13,015 | 956,633 | +2,699 | 5,410 |
| `8c09e37e83` | 2026-10-25 20:15 -0500 | 13,015 | 956,633 | +0 | 0 |
| `85e2f1a8a9` | 2026-10-25 22:06 -0500 | 13,020 | 958,810 | +2,177 | 1,177 |
| `ac81aa9a7f` | 2026-10-25 22:26 -0500 | 13,030 | 960,090 | +1,280 | 3,876 |
| `b37c0c0785` | 2026-10-25 23:59 -0500 | 13,030 | 960,090 | +0 | 0 |
| `65bfbda570` | 2026-10-26 00:14 -0500 | 13,038 | 962,170 | +2,080 | 8,329 |
| `de517344be` | 2026-10-26 00:55 -0500 | 13,040 | 962,905 | +735 | 1,063 |
| `2e1c59daaa` | 2026-10-26 01:04 -0500 | 13,044 | 963,394 | +489 | 3,452 |
| `dcbe3ca885` | 2026-10-26 01:17 -0500 | 13,045 | 963,427 | +33 | 143 |
| `24b54499db` | 2026-10-26 01:25 -0500 | 13,047 | 963,738 | +311 | 2,347 |
| `b27899991a` | 2026-10-26 01:46 -0500 | 13,047 | 963,738 | +0 | 0 |
| `517a1652f8` | 2026-10-26 02:01 -0500 | 13,051 | 966,333 | +2,595 | 10,221 |
| `18b43eb0e7` | 2026-10-26 02:33 -0500 | 13,054 | 967,311 | +978 | 1,827 |
| `1eaa74a963` | 2026-10-26 02:46 -0500 | 13,064 | 968,916 | +1,605 | 7,370 |
| `6dad0bc672` | 2026-10-26 03:39 -0500 | 13,065 | 969,118 | +202 | 232 |
| `92e3907903` | 2026-10-26 05:12 -0500 | 13,067 | 969,128 | +10 | 6 |
| `33fa2414d8` | 2026-10-26 10:15 -0500 | 13,070 | 970,735 | +1,607 | 318 |
| `44bba1d120` | 2026-10-26 11:31 -0500 | 13,097 | 971,796 | +1,061 | 837 |
| `83620d0aae` | 2026-10-26 11:40 -0500 | 13,105 | 972,635 | +839 | 5,934 |
| `a024a4ab78` | 2026-10-26 12:00 -0500 | 13,111 | 972,937 | +302 | 913 |
| `f1099558fa` | 2026-10-26 12:21 -0500 | 13,212 | 985,985 | +13,048 | 36,300 |
| `81bae84720` | 2026-10-26 12:38 -0500 | 13,212 | 985,985 | +0 | 0 |
| `a229af544c` | 2026-10-26 12:55 -0500 | 13,215 | 987,377 | +1,392 | 5,072 |
| `95782e060b` | 2026-10-26 13:26 -0500 | 13,220 | 988,647 | +1,270 | 2,450 |
| `20f2ff9ee3` | 2026-10-26 13:43 -0500 | 13,224 | 991,736 | +3,089 | 11,076 |
| `27d4d89a10` | 2026-10-26 14:20 -0500 | 13,238 | 993,150 | +1,414 | 2,284 |
| `83a2a7ec1a` | 2026-10-26 16:55 -0500 | 13,239 | 1,005,809 | +12,659 | 4,881 |
| `ed9aed8829` | 2026-10-26 17:40 -0500 | 13,239 | 1,005,809 | +0 | 0 |
| `400d917e18` | 2026-10-26 18:03 -0500 | 13,240 | 1,008,770 | +2,961 | 7,730 |
| `4bd27d6fcc` | 2026-10-26 18:21 -0500 | 13,240 | 1,008,770 | +0 | 0 |
| `40b7ff9bba` | 2026-10-26 18:54 -0500 | 13,240 | 1,008,770 | +0 | 0 |
| `90c39c9d9b` | 2026-10-26 19:28 -0500 | 13,240 | 1,008,770 | +0 | 0 |
| `f9a15c4820` | 2026-10-26 20:11 -0500 | 13,240 | 1,008,770 | +0 | 0 |
| `5a172f61bc` | 2026-10-26 20:26 -0500 | 13,241 | 1,009,052 | +282 | 1,100 |
| `90e10d247c` | 2026-10-26 20:53 -0500 | 13,244 | 1,010,920 | +1,868 | 4,256 |
| `43ae89e1fa` | 2026-10-26 21:17 -0500 | 13,321 | 1,018,482 | +7,562 | 18,710 |
| `16fc37734e` | 2026-10-26 21:31 -0500 | 13,334 | 1,018,739 | +257 | 1,067 |
| `382a315c41` | 2026-10-26 22:14 -0500 | 13,366 | 1,035,403 | +16,664 | 23,702 |
| `5f785a3181` | 2026-10-26 22:29 -0500 | 13,367 | 1,036,940 | +1,537 | 6,054 |
| `e88d9f44a0` | 2026-10-26 22:53 -0500 | 13,369 | 1,036,952 | +12 | 30 |
| `628dd35fd6` | 2026-10-26 23:13 -0500 | 13,383 | 1,041,698 | +4,746 | 14,566 |
| `1083794933` | 2026-10-26 23:22 -0500 | 13,384 | 1,041,766 | +68 | 459 |
| `72d5f1d90f` | 2026-10-26 23:48 -0500 | 13,384 | 1,041,766 | +0 | 0 |
| `ec38556c35` | 2026-10-27 00:01 -0500 | 13,384 | 1,041,766 | +0 | 0 |
| `24a106aad7` | 2026-10-27 00:25 -0500 | 13,391 | 1,075,489 | +33,723 | 83,900 |
| `09fd610fd5` | 2026-10-27 00:33 -0500 | 13,402 | 1,076,695 | +1,206 | 9,459 |
| `d04d742392` | 2026-10-27 00:58 -0500 | 13,403 | 1,078,806 | +2,111 | 5,083 |
| `1c4ead347d` | 2026-10-27 01:08 -0500 | 13,403 | 1,078,806 | +0 | 0 |
| `8053b5f696` | 2026-10-27 01:15 -0500 | 13,409 | 1,079,536 | +730 | 6,272 |
| `8242b369c8` | 2026-10-27 01:28 -0500 | 13,418 | 1,080,211 | +675 | 3,275 |
| `47965153a4` | 2026-10-27 01:47 -0500 | 13,424 | 1,080,973 | +762 | 2,402 |
| `226b7d26d7` | 2026-10-27 02:06 -0500 | 13,433 | 1,082,338 | +1,365 | 4,262 |
| `7e294e9202` | 2026-10-27 02:14 -0500 | 13,436 | 1,082,916 | +578 | 4,145 |
| `53858b3295` | 2026-10-27 02:22 -0500 | 13,441 | 1,084,235 | +1,319 | 10,505 |
| `9e88000a31` | 2026-10-27 02:32 -0500 | 13,442 | 1,100,148 | +15,913 | 93,149 |
| `9057e3e090` | 2026-10-27 02:49 -0500 | 13,447 | 1,101,472 | +1,324 | 4,628 |
| `dee81b8211` | 2026-10-27 03:09 -0500 | 13,451 | 1,102,840 | +1,368 | 4,145 |
| `a35fd348ee` | 2026-10-27 03:24 -0500 | 13,453 | 1,103,458 | +618 | 2,584 |
| `6c00d43fee` | 2026-10-27 03:46 -0500 | 13,453 | 1,103,458 | +0 | 0 |
| `73b1467e08` | 2026-10-27 03:58 -0500 | 13,457 | 1,105,861 | +2,403 | 11,834 |
| `494e184a2f` | 2026-10-27 04:22 -0500 | 13,458 | 1,106,821 | +960 | 2,407 |
| `3da1879572` | 2026-10-27 05:18 -0500 | 13,458 | 1,106,821 | +0 | 0 |
| `15e5ca1746` | 2026-10-27 05:27 -0500 | 13,468 | 1,112,564 | +5,743 | 38,145 |
| `2d28b33392` | 2026-10-27 08:51 -0500 | 13,470 | 1,114,103 | +1,539 | 452 |
| `4aee080453` | 2026-10-27 09:00 -0500 | 13,471 | 1,114,614 | +511 | 3,375 |
| `7601c573f5` | 2026-10-27 10:25 -0500 | 13,493 | 1,123,928 | +9,314 | 6,597 |
| `59bd377bc5` | 2026-10-27 11:11 -0500 | 13,511 | 1,131,925 | +7,997 | 10,469 |
| `15066627c5` | 2026-10-27 12:10 -0500 | 13,512 | 1,132,961 | +1,036 | 1,044 |
| `523dd76739` | 2026-10-27 12:32 -0500 | 13,512 | 1,132,961 | +0 | 0 |
| `a650190752` | 2026-10-27 12:35 -0500 | 13,512 | 1,132,961 | +0 | 0 |
| `e751024e02` | 2026-10-27 12:43 -0500 | 13,513 | 1,135,685 | +2,724 | 21,411 |
| `f79e248834` | 2026-10-27 13:21 -0500 | 13,528 | 1,139,355 | +3,670 | 5,769 |
| `c0cabcc7c0` | 2026-10-27 13:25 -0500 | 13,529 | 1,140,051 | +696 | 12,528 |
| `1e891ab44b` | 2026-10-27 13:28 -0500 | 13,529 | 1,140,051 | +0 | 0 |
| `28db575f5b` | 2026-10-27 13:40 -0500 | 13,538 | 1,142,592 | +2,541 | 12,794 |
| `8d6990bf3a` | 2026-10-27 13:47 -0500 | 13,543 | 1,145,001 | +2,409 | 21,900 |
| `d1f622cc10` | 2026-10-27 13:56 -0500 | 13,544 | 1,145,288 | +287 | 1,842 |
| `33173d4d1d` | 2026-10-27 14:14 -0500 | 13,552 | 1,149,010 | +3,722 | 12,293 |
| `aed7652694` | 2026-10-27 14:26 -0500 | 13,552 | 1,149,010 | +0 | 0 |
| `5bc4514800` | 2026-10-27 14:43 -0500 | 13,552 | 1,149,010 | +0 | 0 |
| `015c520928` | 2026-10-27 15:01 -0500 | 13,553 | 1,149,924 | +914 | 3,204 |
| `250dbb2692` | 2026-10-27 15:10 -0500 | 13,554 | 1,152,450 | +2,526 | 16,123 |
| `35f71717dd` | 2026-10-27 15:19 -0500 | 13,558 | 1,153,653 | +1,203 | 7,720 |
| `0d21140a32` | 2026-10-27 15:47 -0500 | 13,565 | 1,169,662 | +16,009 | 35,292 |
| `7f98f528c1` | 2026-10-27 15:58 -0500 | 13,565 | 1,169,662 | +0 | 0 |
| `8e1c1e1c51` | 2026-10-27 16:07 -0500 | 13,565 | 1,169,662 | +0 | 0 |
| `d7d8b95cfa` | 2026-10-27 16:16 -0500 | 13,601 | 1,181,348 | +11,686 | 77,907 |
| `831ac01e99` | 2026-10-27 16:18 -0500 | 13,601 | 1,181,348 | +0 | 0 |
| `9df19d0a22` | 2026-10-27 16:25 -0500 | 13,602 | 1,181,528 | +180 | 1,525 |
| `cb948b55c2` | 2026-10-27 16:36 -0500 | 13,602 | 1,181,528 | +0 | 0 |
| `1721ac6da3` | 2026-10-27 16:46 -0500 | 13,602 | 1,181,528 | +0 | 0 |
| `3bdd6c59cd` | 2026-10-27 16:50 -0500 | 13,602 | 1,181,528 | +0 | 0 |
| `2afa62f785` | 2026-10-27 16:55 -0500 | 13,620 | 1,184,384 | +2,856 | 36,203 |
| `db713c5028` | 2026-10-27 17:04 -0500 | 13,620 | 1,184,384 | +0 | 0 |
| `df3f91a505` | 2026-10-27 17:11 -0500 | 13,622 | 1,186,109 | +1,725 | 15,258 |
| `6f1930f1ba` | 2026-10-27 17:22 -0500 | 13,623 | 1,186,994 | +885 | 5,065 |
| `70f46f009b` | 2026-10-27 17:27 -0500 | 13,624 | 1,187,754 | +760 | 9,151 |
| `afa0224cd8` | 2026-10-27 17:33 -0500 | 13,640 | 1,191,425 | +3,671 | 34,326 |
| `e6cb31fb8c` | 2026-10-27 17:50 -0500 | 13,649 | 1,191,802 | +377 | 1,289 |
| `19345188b1` | 2026-10-27 18:07 -0500 | 13,650 | 1,192,072 | +270 | 986 |
| `8c4d5f851b` | 2026-10-27 18:13 -0500 | 13,650 | 1,192,072 | +0 | 0 |
| `1b048f93a4` | 2026-10-27 18:15 -0500 | 13,651 | 1,194,028 | +1,956 | 64,015 |
| `c0d4062943` | 2026-10-27 18:19 -0500 | 13,651 | 1,194,028 | +0 | 0 |
| `9069f9e069` | 2026-10-27 18:22 -0500 | 13,701 | 1,197,485 | +3,457 | 69,526 |
| `2db7072539` | 2026-10-27 18:23 -0500 | 13,704 | 1,197,802 | +317 | 10,973 |
| `5ccf5f9348` | 2026-10-27 18:36 -0500 | 13,707 | 1,206,330 | +8,528 | 41,713 |
| `4d1aeed764` | 2026-10-27 18:41 -0500 | 13,723 | 1,208,373 | +2,043 | 22,984 |
| `38bb945591` | 2026-10-27 18:57 -0500 | 13,727 | 1,209,411 | +1,038 | 3,980 |
| `5c3caa380d` | 2026-10-27 19:18 -0500 | 13,727 | 1,209,411 | +0 | 0 |
| `1e655a88e9` | 2026-10-27 19:21 -0500 | 13,744 | 1,211,577 | +2,166 | 46,414 |
| `aaf9d2ee03` | 2026-10-27 19:23 -0500 | 13,744 | 1,211,577 | +0 | 0 |
| `5368fbb25a` | 2026-10-27 19:25 -0500 | 13,747 | 1,215,149 | +3,572 | 149,526 |
| `9fc374f6a5` | 2026-10-27 19:31 -0500 | 13,748 | 1,215,405 | +256 | 2,276 |
| `3e507f297d` | 2026-10-27 19:36 -0500 | 13,758 | 1,216,706 | +1,301 | 15,877 |
| `44b1a28f24` | 2026-10-27 19:44 -0500 | 13,763 | 1,217,215 | +509 | 3,770 |
| `4897cd68c4` | 2026-10-27 19:53 -0500 | 13,763 | 1,217,215 | +0 | 0 |
| `3f22f99713` | 2026-10-27 19:55 -0500 | 13,764 | 1,220,857 | +3,642 | 163,890 |
| `11fe989217` | 2026-10-27 20:00 -0500 | 13,764 | 1,220,857 | +0 | 0 |
| `94ef02fbf5` | 2026-10-27 20:01 -0500 | 13,765 | 1,221,932 | +1,075 | 47,778 |
| `0fd277a7f2` | 2026-10-27 20:19 -0500 | 13,796 | 1,228,082 | +6,150 | 21,558 |
| `1f46ed03c9` | 2026-10-27 20:21 -0500 | 13,797 | 1,228,231 | +149 | 3,352 |
| `ed8763b7fc` | 2026-10-27 20:26 -0500 | 13,810 | 1,230,140 | +1,909 | 23,376 |
| `d7d9d2e231` | 2026-10-27 20:41 -0500 | 13,814 | 1,230,666 | +526 | 2,104 |
| `81dd34ce94` | 2026-10-27 20:49 -0500 | 13,830 | 1,232,302 | +1,636 | 12,776 |
| `8cb8eeca67` | 2026-10-27 20:58 -0500 | 13,845 | 1,234,267 | +1,965 | 13,500 |
| `b0eda182bc` | 2026-10-27 21:01 -0500 | 13,865 | 1,234,949 | +682 | 13,950 |
| `5e9031089e` | 2026-10-27 21:02 -0500 | 13,871 | 1,236,007 | +1,058 | 37,711 |
| `45e1196e6d` | 2026-10-27 21:06 -0500 | 13,880 | 1,236,873 | +866 | 13,496 |
| `dea0d0a1b4` | 2026-10-27 21:09 -0500 | 13,887 | 1,237,620 | +747 | 16,007 |
| `5691d7a97b` | 2026-10-27 21:14 -0500 | 13,891 | 1,238,600 | +980 | 12,000 |
| `7c228d54aa` | 2026-10-27 21:15 -0500 | 13,893 | 1,238,855 | +255 | 11,195 |
| `fc71502417` | 2026-10-27 21:27 -0500 | 13,897 | 1,240,377 | +1,522 | 7,941 |
| `b0d5c11330` | 2026-10-27 21:31 -0500 | 13,897 | 1,240,377 | +0 | 0 |
| `e416e78a02` | 2026-10-27 21:32 -0500 | 13,901 | 1,241,488 | +1,111 | 39,212 |
| `8c5285e1a9` | 2026-10-27 21:37 -0500 | 13,904 | 1,241,767 | +279 | 3,512 |
| `01dd3c0e15` | 2026-10-27 21:42 -0500 | 13,918 | 1,243,652 | +1,885 | 23,481 |
| `608339c66d` | 2026-10-27 21:49 -0500 | 13,934 | 1,247,924 | +4,272 | 35,193 |
| `997401e6ab` | 2026-10-27 21:53 -0500 | 13,938 | 1,249,005 | +1,081 | 16,774 |
| `1ee6811f97` | 2026-10-27 22:10 -0500 | 13,938 | 1,249,005 | +0 | 0 |
| `c8bae9b6b5` | 2026-10-27 22:11 -0500 | 13,939 | 1,251,055 | +2,050 | 76,875 |
| `1721fdca33` | 2026-10-27 22:18 -0500 | 13,948 | 1,253,065 | +2,010 | 18,000 |
| `ba0c7d49b2` | 2026-10-27 22:20 -0500 | 13,948 | 1,253,065 | +0 | 0 |
| `120fccc442` | 2026-10-27 22:22 -0500 | 13,948 | 1,253,065 | +0 | 0 |
| `6d9d698cc5` | 2026-10-27 22:24 -0500 | 13,952 | 1,254,771 | +1,706 | 58,491 |
| `638790f9aa` | 2026-10-27 22:36 -0500 | 13,960 | 1,255,302 | +531 | 2,559 |
| `fc55cb3f78` | 2026-10-27 22:38 -0500 | 13,960 | 1,255,302 | +0 | 0 |
| `6d395518d8` | 2026-10-27 22:40 -0500 | 13,965 | 1,255,726 | +424 | 13,273 |
| `4d4687928f` | 2026-10-27 22:42 -0500 | 13,967 | 1,256,618 | +892 | 25,285 |
| `397ad4c74d` | 2026-10-27 23:15 -0500 | 13,968 | 1,257,590 | +972 | 1,778 |
| `fccc4d8f63` | 2026-10-27 23:25 -0500 | 13,971 | 1,261,229 | +3,639 | 21,944 |
| `5ecfcceaf6` | 2026-10-27 23:47 -0500 | 13,971 | 1,261,229 | +0 | 0 |
| `08e381dce0` | 2026-10-27 23:49 -0500 | 13,983 | 1,263,633 | +2,404 | 73,969 |
| `74b3d8613f` | 2026-10-27 23:57 -0500 | 13,985 | 1,263,941 | +308 | 2,364 |
| `93df62b34b` | 2026-10-28 00:04 -0500 | 13,985 | 1,263,941 | +0 | 0 |
| `26d29af8d3` | 2026-10-28 00:06 -0500 | 13,986 | 1,264,125 | +184 | 5,430 |
| `89c9744c6f` | 2026-10-28 00:08 -0500 | 13,986 | 1,264,125 | +0 | 0 |
| `9c6515def0` | 2026-10-28 00:17 -0500 | 13,991 | 1,264,288 | +163 | 1,128 |
| `561868c3d7` | 2026-10-28 00:21 -0500 | 13,992 | 1,265,536 | +1,248 | 16,702 |
| `c78bcc6fb9` | 2026-10-28 00:23 -0500 | 13,999 | 1,267,764 | +2,228 | 90,121 |
| `b44dbb1dfa` | 2026-10-28 00:25 -0500 | 14,000 | 1,268,941 | +1,177 | 31,621 |
| `73a6d621c0` | 2026-10-28 00:34 -0500 | 14,001 | 1,269,375 | +434 | 2,760 |
| `0a2eac027c` | 2026-10-28 00:53 -0500 | 14,003 | 1,270,139 | +764 | 2,440 |
| `a0a9a58f5f` | 2026-10-28 00:55 -0500 | 14,005 | 1,271,012 | +873 | 32,400 |
| `3875f1bbd7` | 2026-10-28 01:05 -0500 | 14,006 | 1,271,375 | +363 | 2,026 |
| `8686c9c471` | 2026-10-28 01:11 -0500 | 14,007 | 1,271,808 | +433 | 4,667 |
| `2ea788770b` | 2026-10-28 01:21 -0500 | 14,007 | 1,271,808 | +0 | 0 |
| `0866505f9c` | 2026-10-28 01:23 -0500 | 14,007 | 1,271,808 | +0 | 0 |
| `c5b1200973` | 2026-10-28 01:24 -0500 | 14,010 | 1,273,660 | +1,852 | 68,033 |
| `6138ca3c4d` | 2026-10-28 01:28 -0500 | 14,010 | 1,273,660 | +0 | 0 |
| `cbb5f3f34c` | 2026-10-28 01:34 -0500 | 14,013 | 1,276,018 | +2,358 | 25,416 |
| `a6d3048c7a` | 2026-10-28 01:36 -0500 | 14,014 | 1,276,067 | +49 | 1,082 |
| `87a36b317e` | 2026-10-28 01:38 -0500 | 14,016 | 1,276,253 | +186 | 6,143 |
| `76285271d9` | 2026-10-28 01:41 -0500 | 14,016 | 1,276,253 | +0 | 0 |
| `2ea76682ef` | 2026-10-28 01:42 -0500 | 14,019 | 1,276,414 | +161 | 5,573 |
| `b3e207bb46` | 2026-10-28 01:44 -0500 | 14,025 | 1,278,499 | +2,085 | 73,588 |
| `e353bdfa1e` | 2026-10-28 01:50 -0500 | 14,027 | 1,279,861 | +1,362 | 14,507 |
| `2071e2706e` | 2026-10-28 01:51 -0500 | 14,028 | 1,279,897 | +36 | 1,364 |
| `8d879a60dc` | 2026-10-28 02:01 -0500 | 14,029 | 1,280,583 | +686 | 4,229 |
| `ddb23a7151` | 2026-10-28 02:05 -0500 | 14,029 | 1,280,583 | +0 | 0 |
| `1d27300a60` | 2026-10-28 02:07 -0500 | 14,044 | 1,281,881 | +1,298 | 39,938 |
| `bf3dc046e3` | 2026-10-28 02:11 -0500 | 14,045 | 1,283,184 | +1,303 | 16,934 |
| `5b5adedacd` | 2026-10-28 02:17 -0500 | 14,046 | 1,283,613 | +429 | 4,624 |
| `8aa9ec1dd7` | 2026-10-28 02:18 -0500 | 14,046 | 1,283,613 | +0 | 0 |
| `e9ba236340` | 2026-10-28 02:20 -0500 | 14,048 | 1,284,317 | +704 | 30,535 |
| `469a191678` | 2026-10-28 02:22 -0500 | 14,056 | 1,285,589 | +1,272 | 36,057 |
| `afd79eb26f` | 2026-10-28 02:26 -0500 | 14,056 | 1,285,589 | +0 | 0 |
| `937419e44d` | 2026-10-28 02:29 -0500 | 14,056 | 1,285,589 | +0 | 0 |
| `8821222ca2` | 2026-10-28 02:31 -0500 | 14,061 | 1,287,907 | +2,318 | 56,384 |
| `6403a6ff05` | 2026-10-28 02:33 -0500 | 14,061 | 1,287,907 | +0 | 0 |
| `5e133cc142` | 2026-10-28 02:45 -0500 | 14,063 | 1,289,071 | +1,164 | 5,828 |
| `bceffdafdb` | 2026-10-28 02:47 -0500 | 14,063 | 1,289,071 | +0 | 0 |
| `e1b8cc1a30` | 2026-10-28 02:49 -0500 | 14,063 | 1,289,071 | +0 | 0 |
| `b21293ee90` | 2026-10-28 02:51 -0500 | 14,064 | 1,290,163 | +1,092 | 29,558 |
| `70cbcff077` | 2026-10-28 02:53 -0500 | 14,080 | 1,291,365 | +1,202 | 46,529 |
| `905765f86a` | 2026-10-28 02:57 -0500 | 14,080 | 1,291,365 | +0 | 0 |
| `aba2b6cd16` | 2026-10-28 03:12 -0500 | 14,119 | 1,294,119 | +2,754 | 10,730 |
| `471ece77d8` | 2026-10-28 03:14 -0500 | 14,123 | 1,295,029 | +910 | 27,074 |
| `4a854932c9` | 2026-10-28 03:21 -0500 | 14,124 | 1,295,042 | +13 | 123 |
| `878787fefe` | 2026-10-28 03:22 -0500 | 14,124 | 1,295,042 | +0 | 0 |
| `102c1fa512` | 2026-10-28 03:25 -0500 | 14,140 | 1,295,925 | +883 | 21,334 |
| `ab678c840d` | 2026-10-28 03:26 -0500 | 14,144 | 1,296,459 | +534 | 21,600 |
| `a2a38700e6` | 2026-10-28 03:28 -0500 | 14,178 | 1,299,880 | +3,421 | 110,951 |
| `1472983862` | 2026-10-28 03:31 -0500 | 14,185 | 1,302,057 | +2,177 | 47,498 |
| `820c5974bf` | 2026-10-28 03:35 -0500 | 14,192 | 1,302,750 | +693 | 9,745 |
| `1660a0b5a4` | 2026-10-28 03:37 -0500 | 14,203 | 1,304,516 | +1,766 | 69,104 |
| `7a37897aa2` | 2026-10-28 03:44 -0500 | 14,221 | 1,307,208 | +2,692 | 20,931 |
| `04ed6693e9` | 2026-10-28 03:50 -0500 | 14,231 | 1,309,442 | +2,234 | 22,528 |
| `d91941a23c` | 2026-10-28 03:55 -0500 | 14,231 | 1,309,442 | +0 | 0 |
| `bd236b8d84` | 2026-10-28 04:01 -0500 | 14,231 | 1,309,442 | +0 | 0 |
| `cf2bf22b35` | 2026-10-28 04:02 -0500 | 14,237 | 1,310,106 | +664 | 22,766 |
| `b8fd65fa3a` | 2026-10-28 04:04 -0500 | 14,241 | 1,311,675 | +1,569 | 49,117 |
| `d7781f28af` | 2026-10-28 04:06 -0500 | 14,241 | 1,311,675 | +0 | 0 |
| `c1338df6b4` | 2026-10-28 04:08 -0500 | 14,252 | 1,312,324 | +649 | 29,954 |
| `08c110f28f` | 2026-10-28 04:10 -0500 | 14,256 | 1,314,622 | +2,298 | 72,568 |
| `38edb0bd32` | 2026-10-28 04:12 -0500 | 14,256 | 1,314,622 | +0 | 0 |
| `b33c2e4c3d` | 2026-10-28 04:17 -0500 | 14,257 | 1,316,149 | +1,527 | 21,901 |
| `079cd055d8` | 2026-10-28 04:19 -0500 | 14,259 | 1,316,267 | +118 | 3,124 |
| `c16c4c9e41` | 2026-10-28 04:20 -0500 | 14,260 | 1,316,326 | +59 | 2,529 |
| `e3ec9bc19f` | 2026-10-28 04:23 -0500 | 14,264 | 1,316,809 | +483 | 9,009 |
| `d53db629a3` | 2026-10-28 04:25 -0500 | 14,265 | 1,318,920 | +2,111 | 65,514 |
| `d602d94707` | 2026-10-28 04:29 -0500 | 14,265 | 1,318,920 | +0 | 0 |
| `403683962b` | 2026-10-28 04:30 -0500 | 14,275 | 1,319,977 | +1,057 | 44,247 |
| `77f1b3b626` | 2026-10-28 04:35 -0500 | 14,277 | 1,320,407 | +430 | 4,793 |
| `b9c7c5658d` | 2026-10-28 04:37 -0500 | 14,282 | 1,321,053 | +646 | 23,256 |
| `7cd405aef3` | 2026-10-28 04:39 -0500 | 14,282 | 1,321,053 | +0 | 0 |
| `7ebaf3cbb2` | 2026-10-28 04:40 -0500 | 14,283 | 1,323,093 | +2,040 | 74,182 |
| `e4577e2a51` | 2026-10-28 04:42 -0500 | 14,290 | 1,324,412 | +1,319 | 49,462 |
| `82e639470c` | 2026-10-28 04:44 -0500 | 14,293 | 1,324,857 | +445 | 16,688 |
| `df552c2388` | 2026-10-28 04:47 -0500 | 14,294 | 1,326,685 | +1,828 | 36,560 |
| `b13148f602` | 2026-10-28 04:50 -0500 | 14,294 | 1,326,685 | +0 | 0 |
| `6440d922a9` | 2026-10-28 04:53 -0500 | 14,297 | 1,327,237 | +552 | 14,830 |
| `f4b88ee61a` | 2026-10-28 04:54 -0500 | 14,300 | 1,327,622 | +385 | 13,075 |
| `c47078d49b` | 2026-10-28 04:59 -0500 | 14,300 | 1,327,622 | +0 | 0 |
| `0237544f81` | 2026-10-28 05:01 -0500 | 14,302 | 1,328,250 | +628 | 16,624 |
| `cf93a469d8` | 2026-10-28 05:03 -0500 | 14,302 | 1,328,250 | +0 | 0 |
| `32eee130b5` | 2026-10-28 05:04 -0500 | 14,304 | 1,331,554 | +3,304 | 154,473 |
| `c1cf26b0e6` | 2026-10-28 05:11 -0500 | 14,315 | 1,334,543 | +2,989 | 25,620 |
| `9dd0c2f879` | 2026-10-28 05:13 -0500 | 14,315 | 1,334,543 | +0 | 0 |
| `b0f817f3e3` | 2026-10-28 05:17 -0500 | 14,315 | 1,334,543 | +0 | 0 |
| `930713dcda` | 2026-10-28 05:26 -0500 | 14,315 | 1,334,543 | +0 | 0 |
| `0606537840` | 2026-10-28 05:27 -0500 | 14,320 | 1,334,927 | +384 | 15,709 |
| `dc5a430a12` | 2026-10-28 05:29 -0500 | 14,327 | 1,336,267 | +1,340 | 44,667 |
| `2b834b2915` | 2026-10-28 05:34 -0500 | 14,330 | 1,337,534 | +1,267 | 15,621 |
| `65a9e47450` | 2026-10-28 05:40 -0500 | 14,331 | 1,338,212 | +678 | 7,580 |
| `a0bf379f78` | 2026-10-28 05:41 -0500 | 14,341 | 1,339,604 | +1,392 | 57,600 |
| `9b5f95bd6f` | 2026-10-28 05:42 -0500 | 14,342 | 1,342,116 | +2,512 | 110,283 |
| `9b11fa61ff` | 2026-10-28 05:44 -0500 | 14,342 | 1,342,116 | +0 | 0 |
| `c7a14d2c18` | 2026-10-28 05:49 -0500 | 14,347 | 1,343,279 | +1,163 | 13,772 |
| `a65722aff0` | 2026-10-28 05:51 -0500 | 14,347 | 1,343,279 | +0 | 0 |
| `f2cd7d6d77` | 2026-10-28 05:53 -0500 | 14,348 | 1,343,381 | +102 | 3,464 |
| `67683735e6` | 2026-10-28 05:55 -0500 | 14,348 | 1,343,381 | +0 | 0 |
| `eae7c896c0` | 2026-10-28 05:56 -0500 | 14,351 | 1,343,829 | +448 | 15,658 |
| `c5e6413bf7` | 2026-10-28 05:58 -0500 | 14,356 | 1,344,305 | +476 | 18,831 |
| `160a0001fc` | 2026-10-28 06:00 -0500 | 14,363 | 1,345,102 | +797 | 30,202 |
| `eeba26fdf2` | 2026-10-28 06:01 -0500 | 14,365 | 1,345,400 | +298 | 9,933 |
| `1aebe7bdd0` | 2026-10-28 06:03 -0500 | 14,378 | 1,348,035 | +2,635 | 85,459 |
| `0a831acf7d` | 2026-10-28 06:05 -0500 | 14,379 | 1,349,822 | +1,787 | 80,415 |
| `546d313086` | 2026-10-28 06:06 -0500 | 14,381 | 1,350,450 | +628 | 24,574 |
| `39cc18ba30` | 2026-10-28 06:23 -0500 | 14,387 | 1,352,109 | +1,659 | 5,844 |
| `cff4d3cc0a` | 2026-10-28 06:25 -0500 | 14,387 | 1,352,109 | +0 | 0 |
| `511a77a823` | 2026-10-28 06:28 -0500 | 14,389 | 1,352,614 | +505 | 9,989 |
| `b9f1b442d5` | 2026-10-28 06:30 -0500 | 14,390 | 1,353,909 | +1,295 | 42,382 |
| `c548a8dae3` | 2026-10-28 07:08 -0500 | 14,390 | 1,353,909 | +0 | 0 |
| `29340e6808` | 2026-10-28 07:09 -0500 | 14,392 | 1,354,450 | +541 | 24,345 |
| `6f05b9be55` | 2026-10-28 07:12 -0500 | 14,393 | 1,356,148 | +1,698 | 39,953 |
| `83cd55ca7a` | 2026-10-28 07:15 -0500 | 14,393 | 1,356,148 | +0 | 0 |
| `07fe7c066f` | 2026-10-28 07:17 -0500 | 14,417 | 1,360,559 | +4,411 | 132,330 |
| `a69cac22a8` | 2026-10-28 07:22 -0500 | 14,442 | 1,366,405 | +5,846 | 68,552 |
| `49075a00f5` | 2026-10-28 07:51 -0500 | 14,446 | 1,367,065 | +660 | 1,352 |
| `433b0647b4` | 2026-10-28 07:54 -0500 | 14,453 | 1,368,739 | +1,674 | 40,446 |
| `b998ffe24a` | 2026-10-28 08:07 -0500 | 14,458 | 1,369,850 | +1,111 | 5,082 |
| `98d7d428bf` | 2026-10-28 08:09 -0500 | 14,462 | 1,370,596 | +746 | 18,024 |
| `e9e2755c80` | 2026-10-28 08:11 -0500 | 14,462 | 1,370,596 | +0 | 0 |
| `8470d42fd4` | 2026-10-28 08:13 -0500 | 14,463 | 1,370,661 | +65 | 2,962 |
| `2b8044ad8a` | 2026-10-28 08:14 -0500 | 14,463 | 1,370,661 | +0 | 0 |
| `9d2056239c` | 2026-10-28 08:16 -0500 | 14,465 | 1,371,194 | +533 | 15,600 |
| `e9e7a361c9` | 2026-10-28 08:18 -0500 | 14,468 | 1,372,859 | +1,665 | 66,600 |
| `a5b398ec4a` | 2026-10-28 08:26 -0500 | 14,468 | 1,372,859 | +0 | 0 |
| `2ece68cff0` | 2026-10-28 08:27 -0500 | 14,468 | 1,372,859 | +0 | 0 |
| `b23149ea48` | 2026-10-28 08:29 -0500 | 14,468 | 1,372,859 | +0 | 0 |
| `7ac24e50af` | 2026-10-28 08:39 -0500 | 14,471 | 1,373,512 | +653 | 3,978 |
| `68043e12ee` | 2026-10-28 08:41 -0500 | 14,474 | 1,374,244 | +732 | 28,335 |
| `ad85c74f6d` | 2026-10-28 08:43 -0500 | 14,474 | 1,374,356 | +112 | 3,200 |
| `8fa99decd8` | 2026-10-28 08:45 -0500 | 14,474 | 1,374,356 | +0 | 0 |
| `be6eaae1f9` | 2026-10-28 08:46 -0500 | 14,475 | 1,374,711 | +355 | 14,690 |
| `9f5e8bba7c` | 2026-10-28 08:52 -0500 | 14,476 | 1,375,028 | +317 | 3,386 |
| `cb2b5bae4e` | 2026-10-28 09:00 -0500 | 14,477 | 1,375,582 | +554 | 4,403 |
| `a619371a56` | 2026-10-28 09:08 -0500 | 14,478 | 1,376,141 | +559 | 3,848 |
| `5c26c736bf` | 2026-10-28 09:13 -0500 | 14,481 | 1,376,330 | +189 | 2,456 |
| `b267113261` | 2026-10-28 09:18 -0500 | 14,482 | 1,376,540 | +210 | 2,400 |
| `4c8ea0e703` | 2026-10-28 09:23 -0500 | 14,483 | 1,376,640 | +100 | 1,314 |
| `c09846b63e` | 2026-10-28 09:27 -0500 | 14,484 | 1,376,946 | +306 | 4,173 |
| `c8f23f7857` | 2026-10-28 09:36 -0500 | 14,484 | 1,376,946 | +0 | 0 |
| `46aba1d502` | 2026-10-28 09:41 -0500 | 14,485 | 1,377,140 | +194 | 2,217 |
| `28bd0db382` | 2026-10-28 09:48 -0500 | 14,485 | 1,377,140 | +0 | 0 |
| `a9574dff29` | 2026-10-28 09:49 -0500 | 14,493 | 1,378,784 | +1,644 | 68,819 |
| `c9adad1ea0` | 2026-10-28 09:54 -0500 | 14,495 | 1,380,348 | +1,564 | 20,474 |
| `908227aea5` | 2026-10-28 10:10 -0500 | 14,501 | 1,381,569 | +1,221 | 4,490 |
| `5acaf50467` | 2026-10-28 10:15 -0500 | 14,503 | 1,381,822 | +253 | 3,544 |
| `c0e038c2e3` | 2026-10-28 10:17 -0500 | 14,505 | 1,382,030 | +208 | 4,959 |
| `c21649df44` | 2026-10-28 10:19 -0500 | 14,510 | 1,383,730 | +1,700 | 68,764 |
| `d9d80a6321` | 2026-10-28 10:30 -0500 | 14,511 | 1,383,849 | +119 | 638 |
| `f680af1596` | 2026-10-28 10:37 -0500 | 14,514 | 1,384,512 | +663 | 5,590 |
| `816832457b` | 2026-10-28 10:39 -0500 | 14,517 | 1,385,070 | +558 | 13,573 |
| `f81a2a0833` | 2026-10-28 10:51 -0500 | 14,519 | 1,385,406 | +336 | 1,699 |
| `d5395d7bd7` | 2026-10-28 10:56 -0500 | 14,521 | 1,385,995 | +589 | 7,796 |
| `cc1099cd76` | 2026-10-28 11:00 -0500 | 14,522 | 1,386,180 | +185 | 2,571 |
| `ba89f419a4` | 2026-10-28 11:05 -0500 | 14,525 | 1,386,692 | +512 | 6,703 |
| `41ee04eba4` | 2026-10-28 11:07 -0500 | 14,530 | 1,387,676 | +984 | 24,600 |
| `0920a9cbe8` | 2026-10-28 11:18 -0500 | 14,531 | 1,387,777 | +101 | 570 |
| `9015ae7745` | 2026-10-28 11:20 -0500 | 14,532 | 1,388,039 | +262 | 5,858 |
| `700d16f577` | 2026-10-28 11:23 -0500 | 14,533 | 1,388,233 | +194 | 4,850 |
| `29eb72d3fd` | 2026-10-28 11:25 -0500 | 14,533 | 1,388,233 | +0 | 0 |
| `08cacaef22` | 2026-10-28 11:27 -0500 | 14,534 | 1,388,819 | +586 | 20,091 |
| `fc67085609` | 2026-10-28 11:29 -0500 | 14,535 | 1,388,964 | +145 | 4,500 |
| `ba180ee6fe` | 2026-10-28 11:31 -0500 | 14,537 | 1,389,898 | +934 | 22,121 |
| `496f1e17e9` | 2026-10-28 11:33 -0500 | 14,537 | 1,389,898 | +0 | 0 |
| `78c85a522e` | 2026-10-28 11:35 -0500 | 14,539 | 1,390,230 | +332 | 12,196 |
| `79febaadf6` | 2026-10-28 11:37 -0500 | 14,542 | 1,390,729 | +499 | 12,220 |
| `3101f8f5a0` | 2026-10-28 11:39 -0500 | 14,543 | 1,391,141 | +412 | 15,291 |
| `f2f3550cd9` | 2026-10-28 11:44 -0500 | 14,545 | 1,391,326 | +185 | 2,075 |
| `3e2f74bbb5` | 2026-10-28 11:49 -0500 | 14,546 | 1,392,142 | +816 | 10,060 |
| `563052c38a` | 2026-10-28 11:54 -0500 | 14,548 | 1,392,387 | +245 | 3,173 |
| `7a658cd225` | 2026-10-28 11:55 -0500 | 14,551 | 1,392,889 | +502 | 21,773 |
| `8c8b4c4fea` | 2026-10-28 11:57 -0500 | 14,558 | 1,394,305 | +1,416 | 36,153 |
| `3bd1c756a5` | 2026-10-28 11:59 -0500 | 14,559 | 1,394,425 | +120 | 3,963 |
| `0be38d9715` | 2026-10-28 12:01 -0500 | 14,560 | 1,394,676 | +251 | 7,996 |
| `a38454a7d4` | 2026-10-28 12:03 -0500 | 14,562 | 1,394,943 | +267 | 10,335 |
| `6f89abe5ad` | 2026-10-28 12:04 -0500 | 14,564 | 1,395,188 | +245 | 9,284 |
| `ba691ee702` | 2026-10-28 12:06 -0500 | 14,565 | 1,395,303 | +115 | 4,452 |
| `6915e1b59b` | 2026-10-28 12:10 -0500 | 14,567 | 1,395,541 | +238 | 3,631 |
| `745ec9aebe` | 2026-10-28 12:12 -0500 | 14,569 | 1,396,147 | +606 | 14,259 |
| `e28dbb4a07` | 2026-10-28 12:14 -0500 | 14,570 | 1,396,398 | +251 | 9,315 |
| `f55b02636a` | 2026-10-28 12:15 -0500 | 14,571 | 1,396,702 | +304 | 13,853 |
| `03c86d5f3a` | 2026-10-28 12:22 -0500 | 14,573 | 1,397,146 | +444 | 3,861 |
| `bc362052c9` | 2026-10-28 12:26 -0500 | 14,577 | 1,398,235 | +1,089 | 16,683 |
| `f623a52b33` | 2026-10-28 12:34 -0500 | 14,577 | 1,398,235 | +0 | 0 |
| `310cbb812b` | 2026-10-28 12:39 -0500 | 14,578 | 1,398,471 | +236 | 3,218 |
| `63dfa134f5` | 2026-10-28 12:42 -0500 | 14,582 | 1,399,560 | +1,089 | 19,800 |
| `cea65806c2` | 2026-10-28 12:46 -0500 | 14,582 | 1,399,560 | +0 | 0 |
| `c9b7874034` | 2026-10-28 12:49 -0500 | 14,587 | 1,401,129 | +1,569 | 37,909 |
| `8237f0a0eb` | 2026-10-28 12:51 -0500 | 14,590 | 1,402,420 | +1,291 | 40,414 |
| `c53e5a7aa3` | 2026-10-28 12:52 -0500 | 14,596 | 1,403,261 | +841 | 28,033 |
| `99659d0791` | 2026-10-28 12:56 -0500 | 14,596 | 1,403,261 | +0 | 0 |
| `ecca99d246` | 2026-10-28 13:00 -0500 | 14,599 | 1,404,667 | +1,406 | 20,410 |
| `d8df3337d9` | 2026-10-28 13:05 -0500 | 14,601 | 1,406,051 | +1,384 | 15,868 |
| `fb23122f6f` | 2026-10-28 13:07 -0500 | 14,606 | 1,406,902 | +851 | 27,112 |
| `dc614461a9` | 2026-10-28 13:10 -0500 | 14,606 | 1,406,902 | +0 | 0 |
| `92ba6a954e` | 2026-10-28 13:13 -0500 | 14,607 | 1,407,638 | +736 | 12,557 |
| `f2d2bf3c49` | 2026-10-28 13:15 -0500 | 14,609 | 1,407,945 | +307 | 11,634 |
| `79f643fdc3` | 2026-10-28 13:16 -0500 | 14,609 | 1,407,945 | +0 | 0 |
| `1dbbf0cbfa` | 2026-10-28 13:25 -0500 | 14,611 | 1,409,518 | +1,573 | 10,974 |
| `d9b1b01681` | 2026-10-28 13:39 -0500 | 14,613 | 1,412,334 | +2,816 | 12,348 |
| `2eb52aebfb` | 2026-10-28 13:43 -0500 | 14,618 | 1,413,224 | +890 | 13,810 |
| `ce04c0223f` | 2026-10-28 13:50 -0500 | 14,618 | 1,413,224 | +0 | 0 |
| `c7038cee05` | 2026-10-28 13:52 -0500 | 14,621 | 1,413,947 | +723 | 20,494 |
| `22604bff0c` | 2026-10-28 13:55 -0500 | 14,628 | 1,415,123 | +1,176 | 18,985 |
| `60d1ca1684` | 2026-10-28 13:57 -0500 | 14,629 | 1,415,346 | +223 | 8,822 |
| `ed524a44e0` | 2026-10-28 14:07 -0500 | 14,629 | 1,415,346 | +0 | 0 |
| `0da3001375` | 2026-10-28 14:09 -0500 | 14,631 | 1,417,117 | +1,771 | 52,691 |
| `45581fc914` | 2026-10-28 14:20 -0500 | 14,632 | 1,418,356 | +1,239 | 6,969 |
| `be0d32c996` | 2026-10-28 14:22 -0500 | 14,634 | 1,419,189 | +833 | 23,800 |
| `92799d752a` | 2026-10-28 14:29 -0500 | 14,636 | 1,420,139 | +950 | 8,465 |
| `5feb588f1b` | 2026-10-28 14:39 -0500 | 14,636 | 1,420,139 | +0 | 0 |
| `41aea13b45` | 2026-10-28 14:40 -0500 | 14,648 | 1,422,590 | +2,451 | 103,807 |
| `5a8bfd7bbc` | 2026-10-28 14:42 -0500 | 14,648 | 1,422,590 | +0 | 0 |
| `dd110c331c` | 2026-10-28 14:44 -0500 | 14,649 | 1,422,789 | +199 | 6,396 |
| `4a37ce3a32` | 2026-10-28 14:58 -0500 | 14,649 | 1,422,789 | +0 | 0 |
| `ad8421ed4a` | 2026-10-28 14:59 -0500 | 14,649 | 1,422,789 | +0 | 0 |
| `3e8a22967e` | 2026-10-28 15:01 -0500 | 14,651 | 1,423,146 | +357 | 13,388 |
| `d72e526678` | 2026-10-28 15:03 -0500 | 14,655 | 1,424,305 | +1,159 | 29,591 |
| `cafa4aeb06` | 2026-10-28 15:06 -0500 | 14,656 | 1,425,259 | +954 | 22,896 |
| `b751801e97` | 2026-10-28 15:12 -0500 | 14,664 | 1,426,124 | +865 | 8,822 |
| `be2cb5b62b` | 2026-10-28 15:16 -0500 | 14,664 | 1,426,124 | +0 | 0 |
| `c05ae2346b` | 2026-10-28 15:17 -0500 | 14,680 | 1,428,105 | +1,981 | 68,573 |
| `5cb380d628` | 2026-10-28 15:21 -0500 | 14,682 | 1,428,791 | +686 | 12,106 |
| `3634dc8d1b` | 2026-10-28 15:24 -0500 | 14,682 | 1,428,791 | +0 | 0 |
| `5ae9171eeb` | 2026-10-28 15:26 -0500 | 14,690 | 1,429,105 | +314 | 10,184 |
| `ac5dbc667a` | 2026-10-28 15:28 -0500 | 14,690 | 1,429,105 | +0 | 0 |
| `d560991511` | 2026-10-28 15:30 -0500 | 14,691 | 1,429,141 | +36 | 1,108 |
| `e265fb5de6` | 2026-10-28 15:32 -0500 | 14,706 | 1,430,381 | +1,240 | 31,660 |
| `c5cffb1da4` | 2026-10-28 15:37 -0500 | 14,707 | 1,430,440 | +59 | 650 |
| `c9d73dfec0` | 2026-10-28 15:43 -0500 | 14,707 | 1,430,440 | +0 | 0 |
| `7591b95d05` | 2026-10-28 15:45 -0500 | 14,710 | 1,431,013 | +573 | 21,714 |
| `d1b9252225` | 2026-10-28 15:56 -0500 | 14,711 | 1,431,575 | +562 | 2,862 |
| `6602e52df5` | 2026-10-28 15:58 -0500 | 14,712 | 1,432,660 | +1,085 | 42,000 |
| `a66ca0ee3e` | 2026-10-28 16:03 -0500 | 14,713 | 1,432,861 | +201 | 2,428 |
| `385942f7ab` | 2026-10-28 16:05 -0500 | 14,713 | 1,432,861 | +0 | 0 |
| `e248f1cf1c` | 2026-10-28 16:07 -0500 | 14,715 | 1,433,487 | +626 | 20,121 |
| `010862ad58` | 2026-10-28 16:14 -0500 | 14,716 | 1,433,856 | +369 | 2,959 |
| `fa551ab551` | 2026-10-28 16:19 -0500 | 14,718 | 1,434,525 | +669 | 9,228 |
| `801b278907` | 2026-10-28 17:45 -0500 | 14,720 | 1,435,137 | +612 | 427 |
| `56f34e9ed5` | 2026-10-28 17:54 -0500 | 14,720 | 1,435,137 | +0 | 0 |
| `4a4ec0fb55` | 2026-10-28 17:58 -0500 | 14,721 | 1,435,619 | +482 | 8,590 |
| `c2225a7b75` | 2026-10-28 17:59 -0500 | 14,722 | 1,436,190 | +571 | 19,033 |
| `34b07773d3` | 2026-10-28 18:03 -0500 | 14,723 | 1,437,294 | +1,104 | 16,912 |
| `8f8247fe4a` | 2026-10-28 18:06 -0500 | 14,724 | 1,437,734 | +440 | 10,490 |
| `9d6af74f60` | 2026-10-28 18:08 -0500 | 14,724 | 1,437,734 | +0 | 0 |
| `9cf21dde01` | 2026-10-28 18:19 -0500 | 14,725 | 1,438,386 | +652 | 3,696 |
| `04b2a015bf` | 2026-10-28 18:23 -0500 | 14,725 | 1,438,386 | +0 | 0 |
| `750ec8180f` | 2026-10-28 18:24 -0500 | 14,726 | 1,438,774 | +388 | 17,460 |
| `cd3c37b027` | 2026-10-28 18:26 -0500 | 14,730 | 1,439,494 | +720 | 18,783 |
| `3d74ceff9b` | 2026-10-28 18:35 -0500 | 14,731 | 1,440,025 | +531 | 3,712 |
| `85219c6caf` | 2026-10-28 18:37 -0500 | 14,740 | 1,440,700 | +675 | 16,309 |
| `236c5ee56b` | 2026-10-28 18:39 -0500 | 14,743 | 1,441,521 | +821 | 27,367 |
| `a1ef0abf6d` | 2026-10-28 18:41 -0500 | 14,749 | 1,442,277 | +756 | 26,947 |
| `56ba72daf2` | 2026-10-28 18:44 -0500 | 14,752 | 1,442,932 | +655 | 14,646 |
| `a95ecc72f1` | 2026-10-28 18:48 -0500 | 14,757 | 1,443,828 | +896 | 12,406 |
| `a2c3f81fec` | 2026-10-28 18:51 -0500 | 14,761 | 1,444,417 | +589 | 11,912 |
| `3388db2f47` | 2026-10-28 19:10 -0500 | 14,762 | 1,444,900 | +483 | 1,527 |
| `2b9f1372d5` | 2026-10-28 19:24 -0500 | 14,763 | 1,446,052 | +1,152 | 5,009 |
| `f6864f8a69` | 2026-10-28 19:27 -0500 | 14,765 | 1,447,583 | +1,531 | 27,558 |
| `16fb935e3e` | 2026-10-28 19:29 -0500 | 14,766 | 1,447,594 | +11 | 309 |
| `a0c1d4df0a` | 2026-10-28 19:36 -0500 | 14,768 | 1,447,888 | +294 | 2,714 |
| `33eb78293d` | 2026-10-28 19:37 -0500 | 14,769 | 1,448,241 | +353 | 12,836 |
| `886814a2b5` | 2026-10-28 19:51 -0500 | 14,772 | 1,448,949 | +708 | 3,086 |
| `491dc0cc33` | 2026-10-28 19:53 -0500 | 14,773 | 1,448,979 | +30 | 831 |
| `481092c1ff` | 2026-10-28 20:04 -0500 | 14,774 | 1,451,569 | +2,590 | 14,021 |
| `23f37d6074` | 2026-10-28 20:09 -0500 | 14,775 | 1,451,796 | +227 | 2,679 |
| `f1285a0275` | 2026-10-28 20:38 -0500 | 14,776 | 1,451,930 | +134 | 285 |
| `a758ee980d` | 2026-10-28 20:41 -0500 | 14,776 | 1,451,930 | +0 | 0 |
| `4294b9258a` | 2026-10-28 20:45 -0500 | 14,777 | 1,453,489 | +1,559 | 25,395 |
| `fa993b5d70` | 2026-10-28 20:53 -0500 | 14,778 | 1,456,368 | +2,879 | 20,770 |
| `b6effca06e` | 2026-10-28 20:55 -0500 | 14,779 | 1,456,925 | +557 | 17,286 |
| `ff1d1f6917` | 2026-10-28 20:59 -0500 | 14,780 | 1,458,575 | +1,650 | 25,939 |
| `b24cc60c85` | 2026-10-28 21:05 -0500 | 14,780 | 1,458,575 | +0 | 0 |
| `affe6b1174` | 2026-10-28 21:46 -0500 | 14,781 | 1,460,985 | +2,410 | 3,546 |
| `bd9e68f803` | 2026-10-28 21:48 -0500 | 14,781 | 1,460,985 | +0 | 0 |
| `aab538ee5a` | 2026-10-28 21:52 -0500 | 14,782 | 1,462,569 | +1,584 | 24,474 |
| `7c67dcc15d` | 2026-10-28 21:54 -0500 | 14,787 | 1,463,191 | +622 | 15,443 |
| `081666b9d9` | 2026-10-28 22:18 -0500 | 14,788 | 1,463,671 | +480 | 1,198 |
| `a003cd36af` | 2026-10-28 22:34 -0500 | 14,788 | 1,463,671 | +0 | 0 |
| `ac72ca15b8` | 2026-10-28 22:38 -0500 | 14,789 | 1,464,359 | +688 | 10,630 |
| `6b689d39fc` | 2026-10-28 22:40 -0500 | 14,789 | 1,464,359 | +0 | 0 |
| `ffbc269a24` | 2026-10-28 23:00 -0500 | 14,794 | 1,464,823 | +464 | 1,408 |
| `8a1fda9755` | 2026-10-28 23:02 -0500 | 14,799 | 1,466,869 | +2,046 | 51,508 |
| `18b48d945c` | 2026-10-28 23:04 -0500 | 14,802 | 1,467,895 | +1,026 | 30,275 |
| `70bb48e46f` | 2026-10-28 23:06 -0500 | 14,802 | 1,467,895 | +0 | 0 |
| `4be9e10f0a` | 2026-10-28 23:08 -0500 | 14,804 | 1,469,472 | +1,577 | 45,784 |
| `858ffa6d2f` | 2026-10-28 23:10 -0500 | 14,805 | 1,469,541 | +69 | 1,533 |
| `d6ee796035` | 2026-10-29 01:50 -0500 | 14,807 | 1,470,574 | +1,033 | 388 |
| `ffb0cfe32c` | 2026-10-29 02:10 -0500 | 14,808 | 1,470,747 | +173 | 514 |
| `29ac03106f` | 2026-10-29 02:34 -0500 | 14,809 | 1,470,883 | +136 | 338 |
| `449cc53dc9` | 2026-10-29 02:37 -0500 | 14,809 | 1,470,883 | +0 | 0 |
| `88976e2299` | 2026-10-29 02:41 -0500 | 14,820 | 1,477,589 | +6,706 | 107,775 |
| `caf3371eaf` | 2026-10-29 03:17 -0500 | 14,822 | 1,477,867 | +278 | 457 |
| `4a1c9a7aae` | 2026-10-29 04:01 -0500 | 14,822 | 1,477,867 | +0 | 0 |
| `02275f25eb` | 2026-10-29 04:05 -0500 | 14,829 | 1,482,160 | +4,293 | 63,339 |
| `1e43d299f1` | 2026-10-29 04:27 -0500 | 14,829 | 1,482,160 | +0 | 0 |
| `d13cf6ee64` | 2026-10-29 04:29 -0500 | 14,848 | 1,485,713 | +3,553 | 106,590 |
| `69723218b8` | 2026-10-29 04:52 -0500 | 14,850 | 1,488,200 | +2,487 | 6,493 |
| `ae0fd6b442` | 2026-10-29 05:07 -0500 | 14,851 | 1,489,120 | +920 | 3,742 |
| `d774418004` | 2026-10-29 05:24 -0500 | 14,859 | 1,493,793 | +4,673 | 15,901 |
| `0c02215dce` | 2026-10-29 05:42 -0500 | 14,870 | 1,494,989 | +1,196 | 3,972 |
| `6cf09db5ed` | 2026-10-29 05:46 -0500 | 14,871 | 1,495,075 | +86 | 1,447 |
| `930a67d58c` | 2026-10-29 06:02 -0500 | 14,900 | 1,498,647 | +3,572 | 13,175 |
| `2bba96fd36` | 2026-10-29 06:05 -0500 | 14,900 | 1,498,647 | +0 | 0 |
| `e254dd5245` | 2026-10-29 06:08 -0500 | 14,909 | 1,500,968 | +2,321 | 53,220 |
| `08c35ab39a` | 2026-10-29 06:30 -0500 | 14,910 | 1,502,055 | +1,087 | 3,022 |
| `6aa0a600fb` | 2026-10-29 06:37 -0500 | 14,918 | 1,504,190 | +2,135 | 17,389 |
| `603595bda2` | 2026-10-29 06:39 -0500 | 14,923 | 1,508,836 | +4,646 | 133,805 |
| `28084ed7cb` | 2026-10-29 06:41 -0500 | 14,927 | 1,513,361 | +4,525 | 171,474 |
| `9a6068e0cc` | 2026-10-29 06:53 -0500 | 14,927 | 1,513,361 | +0 | 0 |
| `094cdba6f8` | 2026-10-29 06:59 -0500 | 14,941 | 1,517,463 | +4,102 | 37,865 |
| `a1dac521dd` | 2026-10-29 07:19 -0500 | 14,941 | 1,517,463 | +0 | 0 |
| `d7bd73dbd1` | 2026-10-29 07:21 -0500 | 14,948 | 1,520,828 | +3,365 | 77,159 |
| `92487c5c75` | 2026-10-29 07:42 -0500 | 14,958 | 1,521,977 | +1,149 | 3,410 |
| `b7ffa415c3` | 2026-10-29 07:44 -0500 | 14,959 | 1,524,633 | +2,656 | 83,874 |
| `b0ab9a15f3` | 2026-10-29 07:54 -0500 | 14,999 | 1,531,046 | +6,413 | 36,472 |
| `440c1b0036` | 2026-10-29 08:06 -0500 | 15,005 | 1,535,619 | +4,573 | 22,459 |
| `af9ff82f7d` | 2026-10-29 08:17 -0500 | 15,033 | 1,540,393 | +4,774 | 26,604 |
| `c24aaa324a` | 2026-10-29 08:23 -0500 | 15,045 | 1,542,324 | +1,931 | 18,198 |
| `54ec289501` | 2026-10-29 08:47 -0500 | 15,066 | 1,547,263 | +4,939 | 12,408 |
| `0f0028b87f` | 2026-10-29 08:59 -0500 | 15,072 | 1,549,348 | +2,085 | 10,708 |
| `4b74187b5b` | 2026-10-29 09:02 -0500 | 15,072 | 1,549,348 | +0 | 0 |
| `e399a00ecd` | 2026-10-29 09:04 -0500 | 15,072 | 1,549,348 | +0 | 0 |
| `975bdc8496` | 2026-10-29 09:07 -0500 | 15,089 | 1,555,426 | +6,078 | 136,755 |
| `5881010589` | 2026-10-29 09:21 -0500 | 15,090 | 1,555,780 | +354 | 1,528 |
| `48916c52cc` | 2026-10-29 09:41 -0500 | 15,106 | 1,563,840 | +8,060 | 24,383 |
| `56d65ba502` | 2026-10-29 09:43 -0500 | 15,107 | 1,565,367 | +1,527 | 42,286 |
| `fc66ee4d9f` | 2026-10-29 09:45 -0500 | 15,109 | 1,566,116 | +749 | 23,447 |
| `9d539345d8` | 2026-10-29 09:53 -0500 | 15,109 | 1,566,116 | +0 | 0 |
| `efa0864aac` | 2026-10-29 09:56 -0500 | 15,112 | 1,568,439 | +2,323 | 54,304 |
| `1d7206dbdc` | 2026-10-29 09:58 -0500 | 15,115 | 1,571,875 | +3,436 | 97,398 |
| `ebe49dad7b` | 2026-10-29 10:00 -0500 | 15,126 | 1,573,806 | +1,931 | 62,068 |
| `ae23f095ca` | 2026-10-29 10:37 -0500 | 15,131 | 1,580,233 | +6,427 | 10,220 |
| `cc317643c7` | 2026-10-29 10:43 -0500 | 15,131 | 1,580,233 | +0 | 0 |
| `f0fafc38b7` | 2026-10-29 10:45 -0500 | 15,131 | 1,580,233 | +0 | 0 |
| `1757e659b4` | 2026-10-29 10:48 -0500 | 15,132 | 1,580,410 | +177 | 4,364 |
| `4caf8b5b59` | 2026-10-29 10:53 -0500 | 15,143 | 1,582,986 | +2,576 | 27,765 |
| `efc9522247` | 2026-10-29 11:06 -0500 | 15,151 | 1,587,124 | +4,138 | 20,379 |
| `9c25c06966` | 2026-10-29 11:39 -0500 | 15,154 | 1,589,176 | +2,052 | 3,681 |
| `de57331141` | 2026-10-29 11:43 -0500 | 15,154 | 1,589,176 | +0 | 0 |
| `a69a63d269` | 2026-10-29 11:44 -0500 | 15,170 | 1,595,437 | +6,261 | 223,164 |
| `2d085b1f76` | 2026-10-29 11:48 -0500 | 15,170 | 1,595,437 | +0 | 0 |
| `f634e4a5bc` | 2026-10-29 11:54 -0500 | 15,187 | 1,598,450 | +3,013 | 28,469 |
| `fa782878c3` | 2026-10-29 12:00 -0500 | 15,188 | 1,599,288 | +838 | 7,735 |
| `7db4fae877` | 2026-10-29 12:12 -0500 | 15,195 | 1,600,674 | +1,386 | 6,920 |
| `ad7e0badc6` | 2026-10-29 12:16 -0500 | 15,196 | 1,601,091 | +417 | 8,071 |
| `010fc11781` | 2026-10-29 12:33 -0500 | 15,197 | 1,601,919 | +828 | 2,928 |
| `35165a70a3` | 2026-10-29 12:35 -0500 | 15,200 | 1,602,883 | +964 | 25,148 |
| `f11b3f3603` | 2026-10-29 12:48 -0500 | 15,206 | 1,604,285 | +1,402 | 6,381 |
| `2bc5a64a3c` | 2026-10-29 12:51 -0500 | 15,211 | 1,605,032 | +747 | 15,367 |
| `5f5262bf97` | 2026-10-29 13:04 -0500 | 15,211 | 1,605,032 | +0 | 0 |
| `45bd7c4e28` | 2026-10-29 13:11 -0500 | 15,213 | 1,605,495 | +463 | 4,375 |
| `40fe92cea6` | 2026-10-29 13:20 -0500 | 15,216 | 1,607,458 | +1,963 | 12,290 |
| `af5c87ac66` | 2026-10-29 13:23 -0500 | 15,216 | 1,607,458 | +0 | 0 |
| `628d68cbe8` | 2026-10-29 13:26 -0500 | 15,217 | 1,608,157 | +699 | 18,235 |
| `014234f441` | 2026-10-29 13:27 -0500 | 15,218 | 1,609,571 | +1,414 | 45,859 |
| `01ad75be37` | 2026-10-29 13:48 -0500 | 15,221 | 1,612,453 | +2,882 | 8,442 |
| `a0e84b6ecd` | 2026-10-29 13:53 -0500 | 15,221 | 1,612,453 | +0 | 0 |
| `414196d811` | 2026-10-29 13:56 -0500 | 15,222 | 1,612,900 | +447 | 10,058 |
| `44e05dd4e7` | 2026-10-29 14:14 -0500 | 15,242 | 1,617,009 | +4,109 | 13,509 |
| `77c8f068f1` | 2026-10-29 14:25 -0500 | 15,243 | 1,617,184 | +175 | 998 |
| `053fb33772` | 2026-10-29 14:35 -0500 | 15,246 | 1,617,419 | +235 | 1,330 |
| `2caee6c5df` | 2026-10-29 14:49 -0500 | 15,246 | 1,617,419 | +0 | 0 |
| `79303828ec` | 2026-10-29 14:51 -0500 | 15,247 | 1,618,074 | +655 | 19,983 |
| `c3581d9f27` | 2026-10-29 14:56 -0500 | 15,258 | 1,622,315 | +4,241 | 54,527 |
| `67162140e2` | 2026-10-29 15:14 -0500 | 15,268 | 1,623,273 | +958 | 3,226 |
| `30672f5a9c` | 2026-10-29 15:15 -0500 | 15,281 | 1,624,062 | +789 | 30,542 |
| `c3f831d334` | 2026-10-29 15:17 -0500 | 15,284 | 1,625,032 | +970 | 32,943 |
| `7bfdfef428` | 2026-10-29 15:18 -0500 | 15,286 | 1,625,823 | +791 | 29,975 |
| `7a52d49827` | 2026-10-29 15:25 -0500 | 15,287 | 1,627,665 | +1,842 | 16,056 |
| `a90ce7bf1a` | 2026-10-29 15:30 -0500 | 15,288 | 1,628,310 | +645 | 8,829 |
| `34ea938d47` | 2026-10-29 15:32 -0500 | 15,289 | 1,628,725 | +415 | 13,105 |
| `55ee9019b6` | 2026-10-29 15:56 -0500 | 15,289 | 1,628,725 | +0 | 0 |
| `0037b994f2` | 2026-10-29 15:58 -0500 | 15,292 | 1,634,527 | +5,802 | 164,466 |
| `a1b4936773` | 2026-10-29 16:00 -0500 | 15,293 | 1,635,213 | +686 | 23,977 |
| `6cb8688ae8` | 2026-10-29 16:03 -0500 | 15,294 | 1,635,720 | +507 | 10,370 |
| `698556b811` | 2026-10-29 16:07 -0500 | 15,294 | 1,635,720 | +0 | 0 |
| `cb46259c56` | 2026-10-29 16:09 -0500 | 15,296 | 1,641,298 | +5,578 | 145,513 |
| `7666077f76` | 2026-10-29 16:15 -0500 | 15,350 | 1,650,717 | +9,419 | 99,731 |
| `1e06923d91` | 2026-10-29 16:45 -0500 | 15,361 | 1,653,218 | +2,501 | 4,994 |
| `5e73095727` | 2026-10-29 16:47 -0500 | 15,361 | 1,653,218 | +0 | 0 |
| `831321f69f` | 2026-10-29 16:48 -0500 | 15,364 | 1,655,767 | +2,549 | 88,235 |
| `088a8b5c0d` | 2026-10-29 16:51 -0500 | 15,366 | 1,655,938 | +171 | 4,397 |
| `117a5c85d2` | 2026-10-29 17:11 -0500 | 15,369 | 1,657,413 | +1,475 | 4,429 |
| `32bf6ffc75` | 2026-10-29 17:21 -0500 | 15,369 | 1,657,413 | +0 | 0 |
| `8e4b0f2c78` | 2026-10-29 17:23 -0500 | 15,380 | 1,667,478 | +10,065 | 272,436 |
| `2290c443e6` | 2026-10-29 17:27 -0500 | 15,425 | 1,673,423 | +5,945 | 112,052 |
| `96a106573f` | 2026-10-29 17:41 -0500 | 15,426 | 1,673,570 | +147 | 632 |
| `a2585d38f2` | 2026-10-29 17:56 -0500 | 15,439 | 1,677,831 | +4,261 | 16,113 |
| `c7d9b92aa2` | 2026-10-29 18:02 -0500 | 15,455 | 1,678,310 | +479 | 4,844 |
| `52766719d5` | 2026-10-29 18:05 -0500 | 15,461 | 1,678,584 | +274 | 5,735 |
| `4372b441a7` | 2026-10-29 18:11 -0500 | 15,462 | 1,679,123 | +539 | 5,360 |
| `1c89e6c4db` | 2026-10-29 18:21 -0500 | 15,474 | 1,681,907 | +2,784 | 17,340 |
| `45e8767838` | 2026-10-29 18:26 -0500 | 15,474 | 1,681,907 | +0 | 0 |
| `7e227c2896` | 2026-10-29 18:28 -0500 | 15,475 | 1,683,303 | +1,396 | 34,900 |
| `ed2788dc7d` | 2026-10-29 18:39 -0500 | 15,477 | 1,683,339 | +36 | 202 |
| `50a4b90db3` | 2026-10-29 18:43 -0500 | 15,493 | 1,687,815 | +4,476 | 64,974 |
| `4ab41c98ef` | 2026-10-29 18:46 -0500 | 15,494 | 1,689,546 | +1,731 | 30,698 |
| `41caaf86ad` | 2026-10-29 18:49 -0500 | 15,497 | 1,689,674 | +128 | 3,114 |
| `f695fe4a73` | 2026-10-29 18:50 -0500 | 15,498 | 1,689,976 | +302 | 10,982 |
| `2781314867` | 2026-10-29 18:53 -0500 | 15,498 | 1,689,976 | +0 | 0 |
| `6a7094d5bb` | 2026-10-29 18:55 -0500 | 15,500 | 1,690,053 | +77 | 2,349 |
| `bf234731d3` | 2026-10-29 19:00 -0500 | 15,502 | 1,691,363 | +1,310 | 13,360 |
| `80cd9e2ca7` | 2026-10-29 19:23 -0500 | 15,506 | 1,694,005 | +2,642 | 7,151 |
| `78b992c68f` | 2026-10-29 19:28 -0500 | 15,519 | 1,696,164 | +2,159 | 22,081 |
| `8b523d86fb` | 2026-10-29 19:31 -0500 | 15,520 | 1,696,334 | +170 | 3,778 |
| `9a80211d8b` | 2026-10-29 19:34 -0500 | 15,521 | 1,696,907 | +573 | 14,033 |
| `5505cdab0d` | 2026-10-29 19:57 -0500 | 15,522 | 1,697,299 | +392 | 1,000 |
| `c48aabaee5` | 2026-10-29 20:04 -0500 | 15,527 | 1,698,149 | +850 | 7,083 |
| `1024976389` | 2026-10-29 20:11 -0500 | 15,527 | 1,698,149 | +0 | 0 |
| `de4ce8b561` | 2026-10-29 20:13 -0500 | 15,527 | 1,698,149 | +0 | 0 |
| `9eefc6968b` | 2026-10-29 20:15 -0500 | 15,527 | 1,698,149 | +0 | 0 |
| `b330c8db4c` | 2026-10-29 20:17 -0500 | 15,527 | 1,698,149 | +0 | 0 |
| `e7a6aa6794` | 2026-10-29 20:19 -0500 | 15,530 | 1,700,392 | +2,243 | 63,581 |
| `bde9b9be8e` | 2026-10-29 20:24 -0500 | 15,531 | 1,700,427 | +35 | 465 |
| `04b6cdab48` | 2026-10-29 20:26 -0500 | 15,532 | 1,700,537 | +110 | 3,194 |
| `691db9bd90` | 2026-10-29 20:28 -0500 | 15,535 | 1,701,121 | +584 | 14,600 |
| `b609e3c19d` | 2026-10-29 20:38 -0500 | 15,535 | 1,701,121 | +0 | 0 |
| `77e089224d` | 2026-10-29 20:46 -0500 | 15,537 | 1,702,624 | +1,503 | 12,583 |
| `ccb8b4cc93` | 2026-10-29 21:08 -0500 | 15,538 | 1,702,945 | +321 | 877 |
| `f81b7ca462` | 2026-10-29 21:10 -0500 | 15,539 | 1,703,490 | +545 | 15,092 |
| `4dd84c0620` | 2026-10-29 21:13 -0500 | 15,546 | 1,704,717 | +1,227 | 20,262 |
| `402d15ab16` | 2026-10-29 21:16 -0500 | 15,546 | 1,704,717 | +0 | 0 |
| `f167fc31c9` | 2026-10-29 21:19 -0500 | 15,553 | 1,705,282 | +565 | 14,028 |
| `800af09bb4` | 2026-10-29 21:26 -0500 | 15,554 | 1,705,606 | +324 | 2,586 |
| `50540dfe9e` | 2026-10-29 21:29 -0500 | 15,554 | 1,705,606 | +0 | 0 |
| `a51b80b215` | 2026-10-29 21:33 -0500 | 15,554 | 1,705,606 | +0 | 0 |
| `260a35a46c` | 2026-10-29 21:36 -0500 | 15,565 | 1,708,428 | +2,822 | 64,708 |
| `ec9fd5dd7f` | 2026-10-29 21:44 -0500 | 15,566 | 1,708,884 | +456 | 3,456 |
| `dbea467fbe` | 2026-10-29 21:46 -0500 | 15,572 | 1,709,315 | +431 | 12,217 |
| `b78c0d65cf` | 2026-10-29 21:49 -0500 | 15,576 | 1,720,390 | +11,075 | 188,066 |
| `e64c427dc1` | 2026-10-29 21:51 -0500 | 15,601 | 1,729,036 | +8,646 | 227,194 |
| `5d73b27954` | 2026-10-29 22:09 -0500 | 15,618 | 1,734,721 | +5,685 | 19,851 |
| `fbd8b1b556` | 2026-10-29 22:11 -0500 | 15,627 | 1,736,591 | +1,870 | 61,200 |
| `bb7adcdf51` | 2026-10-29 22:12 -0500 | 15,628 | 1,736,935 | +344 | 12,900 |
| `f9ce21938c` | 2026-10-29 22:14 -0500 | 15,630 | 1,737,168 | +233 | 9,117 |
| `01c161e9ff` | 2026-10-29 22:16 -0500 | 15,630 | 1,737,168 | +0 | 0 |
| `45ab5c13e1` | 2026-10-29 22:17 -0500 | 15,642 | 1,737,937 | +769 | 26,619 |
| `5b0a0a116c` | 2026-10-29 22:20 -0500 | 15,643 | 1,738,178 | +241 | 4,930 |
| `f76a4d394c` | 2026-10-29 22:23 -0500 | 15,659 | 1,740,851 | +2,673 | 70,239 |
| `289989e9f2` | 2026-10-29 22:56 -0500 | 15,662 | 1,744,363 | +3,512 | 6,247 |
| `8da04169c2` | 2026-10-29 22:59 -0500 | 15,664 | 1,744,818 | +455 | 12,409 |
| `acfd26d39d` | 2026-10-29 23:00 -0500 | 15,678 | 1,748,783 | +3,965 | 130,954 |
| `a3a6995a22` | 2026-10-29 23:13 -0500 | 15,680 | 1,749,237 | +454 | 2,206 |
| `e009bfe3d8` | 2026-10-29 23:17 -0500 | 15,680 | 1,749,237 | +0 | 0 |
| `b8c3ef94fa` | 2026-10-29 23:19 -0500 | 15,682 | 1,749,951 | +714 | 15,965 |
| `e87219b024` | 2026-10-29 23:29 -0500 | 15,686 | 1,751,979 | +2,028 | 11,969 |
| `d3d9d09f42` | 2026-10-29 23:31 -0500 | 15,692 | 1,755,205 | +3,226 | 109,562 |
| `e231148cfb` | 2026-10-29 23:37 -0500 | 15,699 | 1,758,871 | +3,666 | 39,279 |
| `d517454890` | 2026-10-29 23:40 -0500 | 15,701 | 1,759,655 | +784 | 16,602 |
| `413f350969` | 2026-10-30 00:05 -0500 | 15,702 | 1,759,925 | +270 | 640 |
| `a2a7b7a606` | 2026-10-30 00:15 -0500 | 15,710 | 1,762,845 | +2,920 | 18,000 |
| `c6d018aa09` | 2026-10-30 00:18 -0500 | 15,710 | 1,762,845 | +0 | 0 |
| `4d73111dca` | 2026-10-30 00:26 -0500 | 15,713 | 1,765,208 | +2,363 | 18,413 |
| `ece48e7465` | 2026-10-30 00:27 -0500 | 15,720 | 1,766,535 | +1,327 | 43,429 |
| `3bdf977044` | 2026-10-30 00:36 -0500 | 15,721 | 1,771,711 | +5,176 | 34,635 |
| `04d8963ff1` | 2026-10-30 00:40 -0500 | 15,724 | 1,773,126 | +1,415 | 25,858 |
| `800095bd10` | 2026-10-30 00:50 -0500 | 15,726 | 1,773,867 | +741 | 4,324 |
| `56c09b1841` | 2026-10-30 00:52 -0500 | 15,726 | 1,773,867 | +0 | 0 |
| `eff65f6df0` | 2026-10-30 00:54 -0500 | 15,728 | 1,775,599 | +1,732 | 47,597 |
| `ba934a7cd8` | 2026-10-30 00:57 -0500 | 15,738 | 1,780,733 | +5,134 | 99,368 |
| `96ee3cc480` | 2026-10-30 01:03 -0500 | 15,741 | 1,781,621 | +888 | 9,806 |
| `13ac85dc0e` | 2026-10-30 01:16 -0500 | 15,744 | 1,782,797 | +1,176 | 5,578 |
| `2a788a4a6f` | 2026-10-30 01:22 -0500 | 15,759 | 1,788,004 | +5,207 | 51,077 |
| `36fe0c4c0e` | 2026-10-30 01:25 -0500 | 15,764 | 1,790,496 | +2,492 | 41,533 |
| `9ff2f591dc` | 2026-10-30 01:33 -0500 | 15,776 | 1,793,958 | +3,462 | 26,349 |
| `2a53bf70ff` | 2026-10-30 01:41 -0500 | 15,779 | 1,799,649 | +5,691 | 43,406 |
| `951ee2631c` | 2026-10-30 01:43 -0500 | 15,788 | 1,803,939 | +4,290 | 143,000 |
| `f516cc7b8f` | 2026-10-30 01:46 -0500 | 15,790 | 1,805,635 | +1,696 | 33,183 |
| `48efaf9fcc` | 2026-10-30 01:54 -0500 | 15,792 | 1,806,250 | +615 | 4,546 |
| `0017dd39e4` | 2026-10-30 02:07 -0500 | 15,795 | 1,807,811 | +1,561 | 7,327 |
| `4a7917ca23` | 2026-10-30 02:13 -0500 | 15,799 | 1,812,533 | +4,722 | 49,560 |
| `0c831f48fc` | 2026-10-30 02:16 -0500 | 15,801 | 1,813,756 | +1,223 | 20,196 |
| `f0b8f3b5f7` | 2026-10-30 02:20 -0500 | 15,817 | 1,815,360 | +1,604 | 24,365 |
| `3495bf8e3d` | 2026-10-30 02:39 -0500 | 15,817 | 1,815,360 | +0 | 0 |
| `d669728c69` | 2026-10-30 02:42 -0500 | 15,818 | 1,816,076 | +716 | 18,411 |
| `4249e7a26e` | 2026-10-30 02:44 -0500 | 15,819 | 1,816,883 | +807 | 19,498 |
| `6993bf0e1c` | 2026-10-30 02:52 -0500 | 15,821 | 1,818,300 | +1,417 | 11,567 |
| `d29400c2db` | 2026-10-30 02:56 -0500 | 15,823 | 1,819,008 | +708 | 9,103 |
| `b02c5062b2` | 2026-10-30 03:12 -0500 | 15,824 | 1,819,381 | +373 | 1,399 |
| `2184b642c7` | 2026-10-30 03:21 -0500 | 15,824 | 1,819,381 | +0 | 0 |
| `5d81cb8f68` | 2026-10-30 03:24 -0500 | 15,825 | 1,820,634 | +1,253 | 30,896 |
| `1bf1823abf` | 2026-10-30 03:27 -0500 | 15,827 | 1,821,267 | +633 | 10,083 |
| `2479c57914` | 2026-10-30 03:51 -0500 | 15,840 | 1,824,331 | +3,064 | 7,829 |
| `645a15700a` | 2026-10-30 04:00 -0500 | 15,844 | 1,825,987 | +1,656 | 10,780 |
| `1ee362963d` | 2026-10-30 04:02 -0500 | 15,849 | 1,828,827 | +2,840 | 74,628 |
| `f813b6bf5d` | 2026-10-30 04:07 -0500 | 15,850 | 1,829,747 | +920 | 13,355 |
| `5e8c559a70` | 2026-10-30 04:38 -0500 | 15,853 | 1,831,150 | +1,403 | 2,665 |
| `b26e81a420` | 2026-10-30 04:43 -0500 | 15,853 | 1,831,150 | +0 | 0 |
| `cbf2ffe2d9` | 2026-10-30 04:49 -0500 | 15,853 | 1,831,150 | +0 | 0 |
| `c3cc28234f` | 2026-10-30 04:55 -0500 | 15,886 | 1,841,288 | +10,138 | 93,822 |
| `911c4d0268` | 2026-10-30 05:03 -0500 | 15,889 | 1,842,359 | +1,071 | 8,493 |
| `dd64d3c83d` | 2026-10-30 05:20 -0500 | 15,891 | 1,843,791 | +1,432 | 5,044 |
| `7d270e221e` | 2026-10-30 06:14 -0500 | 15,896 | 1,846,010 | +2,219 | 2,456 |
| `791257b679` | 2026-10-30 06:22 -0500 | 15,899 | 1,851,265 | +5,255 | 38,373 |
| `b5ee977aed` | 2026-10-30 06:29 -0500 | 15,899 | 1,851,265 | +0 | 0 |
| `83e5cbafca` | 2026-10-30 06:34 -0500 | 15,918 | 1,857,316 | +6,051 | 70,044 |
| `84bbedf4f7` | 2026-10-30 06:39 -0500 | 15,919 | 1,858,941 | +1,625 | 19,697 |
| `29d3eb4804` | 2026-10-30 06:43 -0500 | 15,920 | 1,859,893 | +952 | 12,463 |
| `d40f2e2ae4` | 2026-10-30 06:54 -0500 | 15,927 | 1,864,222 | +4,329 | 24,389 |
| `9020f4bfdf` | 2026-10-30 07:04 -0500 | 15,937 | 1,871,027 | +6,805 | 42,384 |
| `2cb52b5521` | 2026-10-30 07:45 -0500 | 15,937 | 1,871,027 | +0 | 0 |
| `dced5a78cb` | 2026-10-30 07:53 -0500 | 15,939 | 1,874,684 | +3,657 | 26,436 |
| `7f810672e3` | 2026-10-30 07:59 -0500 | 15,940 | 1,875,892 | +1,208 | 11,010 |
| `5962b639bf` | 2026-10-30 08:06 -0500 | 15,948 | 1,881,512 | +5,620 | 53,666 |
| `86e4f472c2` | 2026-10-30 08:16 -0500 | 15,952 | 1,884,540 | +3,028 | 18,539 |
| `4a6254603d` | 2026-10-30 08:20 -0500 | 15,952 | 1,884,540 | +0 | 0 |
| `85548365e4` | 2026-10-30 08:31 -0500 | 15,953 | 1,885,169 | +629 | 3,516 |
| `4248f141dc` | 2026-10-30 08:36 -0500 | 15,953 | 1,885,169 | +0 | 0 |
| `c91f05700f` | 2026-10-30 08:42 -0500 | 15,955 | 1,885,927 | +758 | 6,856 |
| `e858cb23be` | 2026-10-30 08:47 -0500 | 15,955 | 1,885,927 | +0 | 0 |
| `c5465f2e4f` | 2026-10-30 08:53 -0500 | 15,969 | 1,887,839 | +1,912 | 19,499 |
| `31e7368f09` | 2026-10-30 10:28 -0500 | 15,979 | 1,890,519 | +2,680 | 1,690 |
| `60e3278b19` | 2026-10-30 10:33 -0500 | 15,983 | 1,891,174 | +655 | 7,731 |
| `e5579ce84b` | 2026-10-30 10:39 -0500 | 15,985 | 1,892,161 | +987 | 10,702 |
| `1b55639527` | 2026-10-30 10:49 -0500 | 15,990 | 1,893,928 | +1,767 | 10,655 |
| `150e13e1fd` | 2026-10-30 10:55 -0500 | 15,997 | 1,894,936 | +1,008 | 9,353 |
| `9bc9bf2bea` | 2026-10-30 11:01 -0500 | 15,999 | 1,895,162 | +226 | 2,285 |
| `6cebe854d0` | 2026-10-30 11:11 -0500 | 15,999 | 1,895,162 | +0 | 0 |
| `d36dd0804b` | 2026-10-30 11:18 -0500 | 16,000 | 1,895,674 | +512 | 4,142 |
| `38548270f5` | 2026-10-30 11:24 -0500 | 16,004 | 1,897,320 | +1,646 | 17,126 |
| `b743d3fe29` | 2026-10-30 11:30 -0500 | 16,004 | 1,897,320 | +0 | 0 |
| `9964f9c3a6` | 2026-10-30 11:36 -0500 | 16,005 | 1,898,152 | +832 | 8,343 |
| `c78d20b35c` | 2026-10-30 11:42 -0500 | 16,006 | 1,898,199 | +47 | 466 |
| `68decd9e97` | 2026-10-30 11:59 -0500 | 16,009 | 1,899,077 | +878 | 2,957 |
| `2dce9388dd` | 2026-10-30 12:05 -0500 | 16,011 | 1,901,180 | +2,103 | 21,508 |
| `f2f511a307` | 2026-10-30 12:20 -0500 | 16,013 | 1,902,147 | +967 | 3,988 |
| `1af3b47495` | 2026-10-30 12:32 -0500 | 16,015 | 1,903,271 | +1,124 | 5,558 |
| `d95c86987b` | 2026-10-30 12:50 -0500 | 16,018 | 1,904,566 | +1,295 | 4,337 |
| `fd7cf8bccb` | 2026-10-30 12:57 -0500 | 16,019 | 1,907,951 | +3,385 | 27,323 |
| `00ec61a7c3` | 2026-10-30 13:08 -0500 | 16,020 | 1,909,235 | +1,284 | 7,234 |
| `adbe284b32` | 2026-10-30 13:19 -0500 | 16,020 | 1,909,235 | +0 | 0 |
| `285fbaffb3` | 2026-10-30 13:29 -0500 | 16,021 | 1,910,695 | +1,460 | 8,546 |
| `5291eb4d1d` | 2026-10-30 13:34 -0500 | 16,023 | 1,911,221 | +526 | 6,836 |
| `1278cff2ed` | 2026-10-30 13:42 -0500 | 16,023 | 1,911,221 | +0 | 0 |
| `ce20c593f5` | 2026-10-30 13:50 -0500 | 16,027 | 1,914,034 | +2,813 | 20,376 |
| `779d257cac` | 2026-10-30 13:55 -0500 | 16,029 | 1,914,789 | +755 | 9,438 |
| `79a6bcd853` | 2026-10-30 14:01 -0500 | 16,031 | 1,916,289 | +1,500 | 14,286 |
| `c8f50087c4` | 2026-10-30 14:06 -0500 | 16,032 | 1,917,116 | +827 | 10,826 |
| `0d1adf6360` | 2026-10-30 14:15 -0500 | 16,033 | 1,917,977 | +861 | 5,698 |
| `21b8db14bc` | 2026-10-30 14:34 -0500 | 16,035 | 1,918,685 | +708 | 2,199 |
| `da9c152a52` | 2026-10-30 14:44 -0500 | 16,036 | 1,919,084 | +399 | 2,597 |
| `76071a8a4a` | 2026-10-30 15:07 -0500 | 16,036 | 1,919,084 | +0 | 0 |
| `474b26d3c9` | 2026-10-30 15:36 -0500 | 16,037 | 1,920,548 | +1,464 | 3,095 |
| `979703601b` | 2026-10-30 15:42 -0500 | 16,039 | 1,922,755 | +2,207 | 20,268 |
| `a56e307a52` | 2026-10-30 15:47 -0500 | 16,039 | 1,922,755 | +0 | 0 |
| `23865e60cc` | 2026-10-30 15:52 -0500 | 16,039 | 1,922,755 | +0 | 0 |
| `e1f3f0a530` | 2026-10-30 16:15 -0500 | 16,040 | 1,924,290 | +1,535 | 4,016 |
| `a65e6fb6b3` | 2026-10-30 16:20 -0500 | 16,041 | 1,925,371 | +1,081 | 12,554 |
| `5ab07c72ee` | 2026-10-30 16:25 -0500 | 16,042 | 1,925,417 | +46 | 569 |
| `cba016610a` | 2026-10-30 16:30 -0500 | 16,043 | 1,926,265 | +848 | 9,661 |
| `5604983d2d` | 2026-10-30 16:36 -0500 | 16,044 | 1,927,239 | +974 | 11,131 |
| `92e8d69a43` | 2026-10-30 16:43 -0500 | 16,047 | 1,927,538 | +299 | 2,340 |
| `aee9d02dde` | 2026-10-30 16:52 -0500 | 16,048 | 1,927,993 | +455 | 3,205 |
| `a7f208a9e5` | 2026-10-30 16:57 -0500 | 16,049 | 1,928,670 | +677 | 7,408 |
| `5ed6e82ac8` | 2026-10-30 17:09 -0500 | 16,050 | 1,929,298 | +628 | 3,230 |
| `24a8a4f903` | 2026-10-30 17:26 -0500 | 16,051 | 1,931,414 | +2,116 | 7,468 |
| `ccdc91d1c6` | 2026-10-30 17:38 -0500 | 16,053 | 1,933,083 | +1,669 | 8,265 |
| `628f4e82ac` | 2026-10-30 17:53 -0500 | 16,054 | 1,934,038 | +955 | 3,925 |
| `17f9b255be` | 2026-10-30 17:58 -0500 | 16,054 | 1,934,038 | +0 | 0 |
| `cae971cd18` | 2026-10-30 18:08 -0500 | 16,055 | 1,934,766 | +728 | 4,241 |
| `bd96b99541` | 2026-10-30 18:32 -0500 | 16,056 | 1,935,794 | +1,028 | 2,661 |
| `9ff899d3de` | 2026-10-30 18:38 -0500 | 16,057 | 1,936,382 | +588 | 5,176 |
| `9d001c6d0d` | 2026-10-30 18:44 -0500 | 16,060 | 1,937,587 | +1,205 | 12,084 |
| `880f312611` | 2026-10-30 18:51 -0500 | 16,063 | 1,939,067 | +1,480 | 12,808 |
| `2b47f4dc8c` | 2026-10-30 18:58 -0500 | 16,064 | 1,940,074 | +1,007 | 8,757 |
| `284f9308c4` | 2026-10-30 19:07 -0500 | 16,065 | 1,941,093 | +1,019 | 7,308 |
| `4c9f2814bb` | 2026-10-30 19:48 -0500 | 16,066 | 1,941,483 | +390 | 562 |
| `25bccf8d31` | 2026-10-30 19:59 -0500 | 16,067 | 1,945,027 | +3,544 | 19,128 |
| `8d40d6046f` | 2026-10-30 20:06 -0500 | 16,068 | 1,945,211 | +184 | 1,752 |
| `af3414edea` | 2026-10-30 20:12 -0500 | 16,069 | 1,946,064 | +853 | 8,554 |
| `623675d1d8` | 2026-10-30 20:19 -0500 | 16,071 | 1,947,984 | +1,920 | 16,037 |
| `5d244d45aa` | 2026-10-30 20:31 -0500 | 16,072 | 1,948,801 | +817 | 3,860 |
| `9fd2b24683` | 2026-10-30 20:38 -0500 | 16,073 | 1,950,136 | +1,335 | 12,954 |
| `a6502306eb` | 2026-10-30 20:42 -0500 | 16,073 | 1,950,136 | +0 | 0 |
| `1780a7433c` | 2026-10-30 20:53 -0500 | 16,073 | 1,950,136 | +0 | 0 |
| `3a169a023b` | 2026-10-30 21:00 -0500 | 16,077 | 1,953,805 | +3,669 | 30,575 |
| `f50a4a60c2` | 2026-10-30 21:07 -0500 | 16,079 | 1,955,345 | +1,540 | 14,706 |
| `c12d805eb7` | 2026-10-30 21:13 -0500 | 16,079 | 1,955,345 | +0 | 0 |
| `4a1d3e9922` | 2026-10-30 21:19 -0500 | 16,080 | 1,956,400 | +1,055 | 10,237 |
| `6e5aa2956e` | 2026-10-30 21:24 -0500 | 16,084 | 1,957,561 | +1,161 | 12,514 |
| `bfac31c189` | 2026-10-30 21:33 -0500 | 16,086 | 1,958,250 | +689 | 4,707 |
| `2eadc68e47` | 2026-10-30 21:41 -0500 | 16,087 | 1,960,254 | +2,004 | 15,821 |
| `e16ef9393a` | 2026-10-30 21:48 -0500 | 16,089 | 1,961,592 | +1,338 | 11,635 |
| `d4dcc522b9` | 2026-10-30 21:54 -0500 | 16,090 | 1,963,383 | +1,791 | 16,791 |
| `58e14225e1` | 2026-10-30 21:59 -0500 | 16,090 | 1,963,383 | +0 | 0 |
| `ce17aa59a2` | 2026-10-30 22:05 -0500 | 16,091 | 1,963,828 | +445 | 4,249 |
| `815779f024` | 2026-10-30 22:10 -0500 | 16,091 | 1,963,828 | +0 | 0 |
| `22ef6ae4a7` | 2026-10-30 22:16 -0500 | 16,092 | 1,965,041 | +1,213 | 11,372 |
| `68ec8c1f18` | 2026-10-30 22:34 -0500 | 16,094 | 1,968,294 | +3,253 | 10,874 |
| `191aa29f25` | 2026-10-30 22:41 -0500 | 16,095 | 1,971,989 | +3,695 | 32,365 |
| `ff2524857a` | 2026-10-30 22:54 -0500 | 16,097 | 1,975,600 | +3,611 | 17,426 |
| `06e0db2474` | 2026-10-30 23:07 -0500 | 16,098 | 1,976,559 | +959 | 4,289 |
| `10cea28299` | 2026-10-30 23:15 -0500 | 16,102 | 1,982,281 | +5,722 | 44,781 |
| `9b751607b3` | 2026-10-30 23:22 -0500 | 16,103 | 1,985,611 | +3,330 | 27,622 |
| `3ec4de5cda` | 2026-10-30 23:28 -0500 | 16,105 | 1,988,855 | +3,244 | 33,367 |
| `3cc2197604` | 2026-10-31 03:42 -0500 | 16,106 | 1,989,358 | +503 | 119 |
| `15bdd2fb2a` | 2026-10-31 03:54 -0500 | 16,106 | 1,989,358 | +0 | 0 |
| `3750b12e3d` | 2026-10-31 03:59 -0500 | 16,108 | 1,990,860 | +1,502 | 17,111 |
| `38d44b18b5` | 2026-10-31 04:19 -0500 | 16,110 | 1,994,224 | +3,364 | 10,289 |
| `34b11da6a1` | 2026-10-31 04:26 -0500 | 16,108 | 1,992,114 | -2,110 | -17,342 |
| `5dbf1a0197` | 2026-10-31 04:33 -0500 | 16,108 | 1,992,114 | +0 | 0 |
| `238c38b550` | 2026-10-31 04:39 -0500 | 16,109 | 1,992,231 | +117 | 1,048 |
| `e86321ad23` | 2026-10-31 04:50 -0500 | 16,110 | 1,992,718 | +487 | 2,727 |
| `09104e0ddc` | 2026-10-31 05:02 -0500 | 16,111 | 1,993,024 | +306 | 1,601 |
| `01cc32918e` | 2026-10-31 05:14 -0500 | 16,112 | 1,993,339 | +315 | 1,573 |
| `2ea436ffef` | 2026-10-31 05:25 -0500 | 16,114 | 1,993,541 | +202 | 1,107 |
| `e886945897` | 2026-10-31 05:30 -0500 | 16,115 | 1,993,765 | +224 | 2,688 |
| `9d743a3081` | 2026-10-31 05:35 -0500 | 16,116 | 1,994,307 | +542 | 6,136 |
| `3d7ee4d1d4` | 2026-10-31 05:39 -0500 | 16,117 | 1,995,669 | +1,362 | 17,895 |
| `515ea9854c` | 2026-10-31 05:44 -0500 | 16,118 | 1,995,953 | +284 | 3,466 |
| `6002fb0d5c` | 2026-10-31 05:50 -0500 | 16,118 | 1,995,953 | +0 | 0 |
| `52772632ea` | 2026-10-31 05:55 -0500 | 16,119 | 1,996,899 | +946 | 11,203 |
| `b368edfc25` | 2026-10-31 05:59 -0500 | 16,122 | 1,999,150 | +2,251 | 28,138 |
| `bb5bba54b6` | 2026-10-31 06:04 -0500 | 16,123 | 1,999,956 | +806 | 10,181 |
| `4f73755549` | 2026-10-31 06:09 -0500 | 16,125 | 2,000,779 | +823 | 9,843 |
| `f2d9f57c51` | 2026-10-31 06:14 -0500 | 16,125 | 2,000,779 | +0 | 0 |
| `bf58687657` | 2026-10-31 06:20 -0500 | 16,128 | 2,001,682 | +903 | 10,159 |
| `a495922fe3` | 2026-10-31 06:24 -0500 | 16,129 | 2,002,150 | +468 | 5,953 |
| `0f7e209c1b` | 2026-10-31 06:29 -0500 | 16,130 | 2,002,589 | +439 | 5,584 |
| `f1a40ea928` | 2026-10-31 06:34 -0500 | 16,132 | 2,003,493 | +904 | 11,540 |
| `78d2d4a8c6` | 2026-10-31 06:39 -0500 | 16,136 | 2,004,414 | +921 | 11,970 |
| `8c5d6ec133` | 2026-10-31 06:44 -0500 | 16,137 | 2,005,052 | +638 | 7,555 |
| `cab647123c` | 2026-10-31 06:49 -0500 | 16,137 | 2,005,052 | +0 | 0 |
| `ff7d9a951f` | 2026-10-31 06:54 -0500 | 16,137 | 2,005,052 | +0 | 0 |
| `9636ffa3d7` | 2026-10-31 07:04 -0500 | 16,138 | 2,005,488 | +436 | 2,499 |
| `ca12734cc3` | 2026-10-31 07:09 -0500 | 16,138 | 2,005,488 | +0 | 0 |
| `ec2b054779` | 2026-10-31 07:14 -0500 | 16,139 | 2,007,683 | +2,195 | 25,006 |
| `47879d16ee` | 2026-10-31 07:20 -0500 | 16,139 | 2,007,683 | +0 | 0 |
| `92cfff3e85` | 2026-10-31 07:27 -0500 | 16,150 | 2,012,869 | +5,186 | 44,136 |
| `4c77891b5c` | 2026-10-31 07:32 -0500 | 16,150 | 2,012,869 | +0 | 0 |
| `331314743a` | 2026-10-31 07:37 -0500 | 16,151 | 2,013,663 | +794 | 9,528 |
| `fa0e2dc8f6` | 2026-10-31 07:42 -0500 | 16,152 | 2,013,929 | +266 | 3,203 |
| `50f5448e4f` | 2026-10-31 07:58 -0500 | 16,155 | 2,014,653 | +724 | 2,845 |
| `92a6dcf8af` | 2026-10-31 08:03 -0500 | 16,157 | 2,015,929 | +1,276 | 14,866 |
| `72a61834b6` | 2026-10-31 08:08 -0500 | 16,157 | 2,015,929 | +0 | 0 |
| `9229f8beb3` | 2026-10-31 08:15 -0500 | 16,158 | 2,016,654 | +725 | 6,574 |
| `37f80cc43b` | 2026-10-31 08:17 -0500 | 16,172 | 2,019,813 | +3,159 | 112,598 |
| `c0cb810179` | 2026-10-31 08:18 -0500 | 16,173 | 2,021,141 | +1,328 | 54,327 |
| `96eaa3c0ee` | 2026-10-31 08:20 -0500 | 16,174 | 2,026,273 | +5,132 | 132,915 |
| `41768ab5b5` | 2026-10-31 08:22 -0500 | 16,175 | 2,027,024 | +751 | 25,033 |
| `1a15a5407f` | 2026-10-31 08:24 -0500 | 16,175 | 2,027,024 | +0 | 0 |
| `483c8397e2` | 2026-10-31 08:26 -0500 | 16,175 | 2,027,024 | +0 | 0 |
| `0d46ea6cbd` | 2026-10-31 08:27 -0500 | 16,178 | 2,028,926 | +1,902 | 65,211 |
| `7dc927a381` | 2026-10-31 08:29 -0500 | 16,179 | 2,029,245 | +319 | 11,150 |
| `514668cf5a` | 2026-10-31 08:30 -0500 | 16,179 | 2,029,245 | +0 | 0 |
| `1e4e34c16f` | 2026-10-31 08:33 -0500 | 16,180 | 2,029,452 | +207 | 4,516 |
| `3b40ffe901` | 2026-10-31 08:35 -0500 | 16,181 | 2,029,576 | +124 | 4,464 |
| `3d61e84933` | 2026-10-31 08:36 -0500 | 16,182 | 2,029,720 | +144 | 6,480 |
| `81a69294d1` | 2026-10-31 08:38 -0500 | 16,184 | 2,029,966 | +246 | 10,933 |
| `4a931372ae` | 2026-10-31 08:39 -0500 | 16,187 | 2,030,299 | +333 | 12,488 |
| `281d1586fe` | 2026-10-31 08:41 -0500 | 16,188 | 2,030,471 | +172 | 7,551 |
| `3e349788ee` | 2026-10-31 08:42 -0500 | 16,189 | 2,030,644 | +173 | 7,884 |
| `2ca821c1a4` | 2026-10-31 08:43 -0500 | 16,190 | 2,030,724 | +80 | 3,692 |
| `3e270413cc` | 2026-10-31 08:44 -0500 | 16,191 | 2,030,831 | +107 | 5,003 |
| `cac27ef52d` | 2026-10-31 08:46 -0500 | 16,192 | 2,030,928 | +97 | 4,595 |
| `d6854f305d` | 2026-10-31 08:47 -0500 | 16,193 | 2,031,117 | +189 | 8,953 |
| `d8752f2c6b` | 2026-10-31 08:48 -0500 | 16,194 | 2,031,302 | +185 | 8,649 |
| `57c08c36b2` | 2026-10-31 08:50 -0500 | 16,195 | 2,031,376 | +74 | 3,330 |
| `c18cdda9f2` | 2026-10-31 08:51 -0500 | 16,196 | 2,031,456 | +80 | 3,789 |
| `701c711377` | 2026-10-31 08:52 -0500 | 16,198 | 2,031,570 | +114 | 5,622 |
| `7639b4e38e` | 2026-10-31 08:53 -0500 | 16,199 | 2,031,627 | +57 | 2,736 |
| `ad8ab31f26` | 2026-10-31 08:55 -0500 | 16,201 | 2,032,401 | +774 | 30,620 |
| `0258deabe9` | 2026-10-31 08:57 -0500 | 16,202 | 2,033,920 | +1,519 | 51,589 |
| `5aadfb7432` | 2026-10-31 08:58 -0500 | 16,202 | 2,033,920 | +0 | 0 |
| `5f27ff2de6` | 2026-10-31 09:04 -0500 | 16,219 | 2,035,334 | +1,414 | 14,884 |
| `3df912d619` | 2026-10-31 09:13 -0500 | 16,220 | 2,038,310 | +2,976 | 20,101 |
| `ec9be1db00` | 2026-10-31 09:23 -0500 | 16,222 | 2,039,187 | +877 | 5,167 |
| `b409ede3c7` | 2026-10-31 09:26 -0500 | 16,223 | 2,039,434 | +247 | 4,136 |
| `a94dcebe85` | 2026-10-31 09:28 -0500 | 16,223 | 2,039,434 | +0 | 0 |
| `f8bc23f835` | 2026-10-31 10:11 -0500 | 16,224 | 2,042,005 | +2,571 | 3,603 |
| `4315ab58de` | 2026-10-31 10:13 -0500 | 16,225 | 2,043,891 | +1,886 | 53,044 |
| `9312a79c84` | 2026-10-31 10:15 -0500 | 16,226 | 2,045,358 | +1,467 | 47,154 |
| `fce5b8c7bb` | 2026-10-31 10:31 -0500 | 16,226 | 2,045,358 | +0 | 0 |
| `903022f98a` | 2026-10-31 10:33 -0500 | 16,227 | 2,045,994 | +636 | 24,619 |
| `795a795456` | 2026-10-31 10:35 -0500 | 16,228 | 2,047,337 | +1,343 | 38,678 |
| `9c3feff91f` | 2026-10-31 10:37 -0500 | 16,229 | 2,047,409 | +72 | 2,469 |
| `4f49a3ccd8` | 2026-10-31 10:40 -0500 | 16,229 | 2,047,409 | +0 | 0 |
| `8b613f4422` | 2026-10-31 10:43 -0500 | 16,230 | 2,047,515 | +106 | 1,759 |
| `9dffdb6c48` | 2026-10-31 10:55 -0500 | 16,233 | 2,048,992 | +1,477 | 7,395 |
| `c3a146acf8` | 2026-10-31 11:02 -0500 | 16,236 | 2,049,413 | +421 | 3,886 |
| `3731bd8aa2` | 2026-10-31 11:04 -0500 | 16,236 | 2,049,413 | +0 | 0 |
| `ea6e668fa7` | 2026-10-31 11:07 -0500 | 16,237 | 2,050,068 | +655 | 15,826 |
| `a54c4b02de` | 2026-10-31 11:08 -0500 | 16,239 | 2,050,881 | +813 | 26,368 |
| `18904bb641` | 2026-10-31 11:12 -0500 | 16,241 | 2,051,543 | +662 | 10,098 |
| `dff1439c77` | 2026-10-31 11:14 -0500 | 16,241 | 2,051,543 | +0 | 0 |
| `cf677d88f2` | 2026-10-31 11:16 -0500 | 16,242 | 2,052,118 | +575 | 21,789 |
| `525a58f1de` | 2026-10-31 11:18 -0500 | 16,242 | 2,052,118 | +0 | 0 |
| `f628157d8f` | 2026-10-31 11:19 -0500 | 16,244 | 2,052,818 | +700 | 28,966 |
| `9f461b7ef3` | 2026-10-31 11:40 -0500 | 16,250 | 2,056,919 | +4,101 | 11,839 |
| `7b2fc977be` | 2026-10-31 11:46 -0500 | 16,251 | 2,058,013 | +1,094 | 11,971 |
| `3369c5bb4e` | 2026-10-31 11:53 -0500 | 16,251 | 2,058,013 | +0 | 0 |
| `34bcce60df` | 2026-10-31 11:54 -0500 | 16,253 | 2,058,709 | +696 | 26,375 |
| `31c2e9f977` | 2026-10-31 12:13 -0500 | 16,254 | 2,058,754 | +45 | 149 |
| `6f82ddd435` | 2026-10-31 12:15 -0500 | 16,254 | 2,058,754 | +0 | 0 |
| `63f563b5d9` | 2026-10-31 12:17 -0500 | 16,254 | 2,058,754 | +0 | 0 |
| `cc5191dfa3` | 2026-10-31 12:19 -0500 | 16,255 | 2,060,774 | +2,020 | 53,471 |
| `b98a6001e7` | 2026-10-31 12:21 -0500 | 16,257 | 2,062,499 | +1,725 | 50,081 |
| `7f2661471d` | 2026-10-31 12:29 -0500 | 16,261 | 2,066,206 | +3,707 | 28,515 |
| `9b0ab6ea77` | 2026-10-31 12:36 -0500 | 16,261 | 2,066,206 | +0 | 0 |
| `de8b070609` | 2026-10-31 12:37 -0500 | 16,262 | 2,067,002 | +796 | 33,713 |
| `4501c3d35d` | 2026-10-31 13:00 -0500 | 16,262 | 2,067,002 | +0 | 0 |
| `d1014880a3` | 2026-10-31 13:02 -0500 | 16,262 | 2,067,002 | +0 | 0 |
| `2f132ec293` | 2026-10-31 13:04 -0500 | 16,262 | 2,067,002 | +0 | 0 |
| `a9c3488827` | 2026-10-31 13:06 -0500 | 16,266 | 2,069,175 | +2,173 | 57,521 |
| `cada865753` | 2026-10-31 13:07 -0500 | 16,267 | 2,069,514 | +339 | 15,849 |
| `2d48c08700` | 2026-10-31 13:09 -0500 | 16,272 | 2,072,836 | +3,322 | 129,991 |
| `ea07004df8` | 2026-10-31 13:30 -0500 | 16,273 | 2,077,339 | +4,503 | 13,042 |
| `1ff984dab8` | 2026-10-31 13:33 -0500 | 16,273 | 2,077,339 | +0 | 0 |
| `e0661de38b` | 2026-10-31 13:34 -0500 | 16,273 | 2,077,339 | +0 | 0 |
| `2eb7c5f3fe` | 2026-10-31 13:36 -0500 | 16,276 | 2,081,184 | +3,845 | 101,036 |
| `70e864c71b` | 2026-10-31 13:38 -0500 | 16,278 | 2,081,525 | +341 | 13,060 |
| `94c1f51ecb` | 2026-10-31 13:46 -0500 | 16,279 | 2,082,279 | +754 | 5,386 |
| `2a8262742e` | 2026-10-31 13:48 -0500 | 16,279 | 2,082,279 | +0 | 0 |
| `f09a78bc41` | 2026-10-31 13:50 -0500 | 16,280 | 2,082,909 | +630 | 22,680 |
| `60269a521b` | 2026-10-31 14:24 -0500 | 16,282 | 2,083,216 | +307 | 545 |
| `5f067b427f` | 2026-10-31 14:25 -0500 | 16,283 | 2,083,261 | +45 | 1,620 |
| `e79f84e2e1` | 2026-10-31 14:36 -0500 | 16,283 | 2,083,261 | +0 | 0 |
| `3ae4b91151` | 2026-10-31 14:40 -0500 | 16,306 | 2,087,693 | +4,432 | 60,898 |
| `5b3d9e4e10` | 2026-10-31 14:51 -0500 | 16,308 | 2,089,732 | +2,039 | 11,523 |
| `b2949383dc` | 2026-10-31 14:53 -0500 | 16,309 | 2,089,940 | +208 | 7,131 |
| `baf5beb9b9` | 2026-10-31 14:55 -0500 | 16,309 | 2,089,940 | +0 | 0 |
| `757834ffe8` | 2026-10-31 15:14 -0500 | 16,311 | 2,091,952 | +2,012 | 6,244 |
| `4b854418b3` | 2026-10-31 15:19 -0500 | 16,311 | 2,091,952 | +0 | 0 |
| `98c8ec886f` | 2026-10-31 15:21 -0500 | 16,318 | 2,092,578 | +626 | 15,227 |
| `a39cb973be` | 2026-10-31 15:23 -0500 | 16,318 | 2,092,578 | +0 | 0 |
| `1fa523ddb1` | 2026-10-31 15:24 -0500 | 16,319 | 2,092,693 | +115 | 5,241 |
| `d8ef35f2f8` | 2026-10-31 15:28 -0500 | 16,319 | 2,092,693 | +0 | 0 |
| `8e770d4cdf` | 2026-10-31 15:30 -0500 | 16,320 | 2,095,033 | +2,340 | 82,588 |
| `c8d61f32f5` | 2026-10-31 15:44 -0500 | 16,322 | 2,096,287 | +1,254 | 5,262 |
| `ca5eb7b473` | 2026-10-31 15:55 -0500 | 16,326 | 2,097,391 | +1,104 | 5,794 |
| `3fa8c075ac` | 2026-10-31 16:00 -0500 | 16,327 | 2,098,101 | +710 | 8,875 |
| `a2b922b7d3` | 2026-10-31 16:32 -0500 | 16,327 | 2,098,101 | +0 | 0 |
| `bfe9eec9ae` | 2026-10-31 16:42 -0500 | 16,327 | 2,098,101 | +0 | 0 |
| `8f55aa9221` | 2026-10-31 16:44 -0500 | 16,337 | 2,104,325 | +6,224 | 183,659 |
| `7068ace6b1` | 2026-10-31 16:52 -0500 | 16,339 | 2,105,369 | +1,044 | 8,427 |
| `f7e0967f1e` | 2026-10-31 16:54 -0500 | 16,339 | 2,105,369 | +0 | 0 |
| `d158afa1ca` | 2026-10-31 16:56 -0500 | 16,346 | 2,110,889 | +5,520 | 191,077 |
| `bfbe923e97` | 2026-10-31 17:08 -0500 | 16,347 | 2,111,013 | +124 | 607 |
| `8a5292a6ba` | 2026-10-31 17:17 -0500 | 16,347 | 2,111,013 | +0 | 0 |
| `44a1faba99` | 2026-10-31 17:24 -0500 | 16,354 | 2,116,160 | +5,147 | 44,649 |
| `c3c9de2ba5` | 2026-10-31 17:26 -0500 | 16,355 | 2,119,667 | +3,507 | 112,725 |
| `9ea2bac2bb` | 2026-10-31 17:28 -0500 | 16,356 | 2,120,372 | +705 | 15,862 |
| `f71456fae9` | 2026-10-31 17:30 -0500 | 16,357 | 2,120,889 | +517 | 21,393 |
| `f9a8a84279` | 2026-10-31 17:36 -0500 | 16,357 | 2,120,889 | +0 | 0 |
| `62be56a9a0` | 2026-10-31 17:42 -0500 | 16,359 | 2,121,776 | +887 | 8,725 |
| `1cc2c0378b` | 2026-10-31 17:51 -0500 | 16,361 | 2,123,565 | +1,789 | 12,061 |
| `4551375966` | 2026-10-31 17:53 -0500 | 16,363 | 2,125,036 | +1,471 | 50,434 |
| `9af71ea70a` | 2026-10-31 18:02 -0500 | 16,364 | 2,126,346 | +1,310 | 8,274 |
| `ab1745c122` | 2026-10-31 18:29 -0500 | 16,375 | 2,128,198 | +1,852 | 4,167 |
| `cefda30fd2` | 2026-10-31 18:55 -0500 | 16,376 | 2,129,094 | +896 | 2,117 |
| `f15a5f7bc3` | 2026-10-31 19:19 -0500 | 16,377 | 2,129,359 | +265 | 662 |
| `860568df45` | 2026-10-31 19:26 -0500 | 16,401 | 2,134,601 | +5,242 | 41,024 |
| `7c666c8cae` | 2026-10-31 19:30 -0500 | 16,402 | 2,134,973 | +372 | 6,630 |
| `9b40fcf915` | 2026-10-31 19:46 -0500 | 16,402 | 2,134,973 | +0 | 0 |
| `a0a749cc9f` | 2026-10-31 19:58 -0500 | 16,411 | 2,137,047 | +2,074 | 11,029 |
| `97eca56dc3` | 2026-10-31 19:59 -0500 | 16,413 | 2,137,436 | +389 | 14,437 |
| `bbd296faa6` | 2026-10-31 20:10 -0500 | 16,431 | 2,141,320 | +3,884 | 20,593 |
| `29a00220ae` | 2026-10-31 20:12 -0500 | 16,432 | 2,141,827 | +507 | 23,104 |
| `53bf6b5f58` | 2026-10-31 20:14 -0500 | 16,432 | 2,141,827 | +0 | 0 |
| `851a1582e7` | 2026-10-31 20:27 -0500 | 16,433 | 2,142,364 | +537 | 2,491 |
| `274224c3b9` | 2026-10-31 20:34 -0500 | 16,433 | 2,142,364 | +0 | 0 |
| `2d66e211d9` | 2026-10-31 20:41 -0500 | 16,435 | 2,144,363 | +1,999 | 16,933 |
| `1f5fbfbbb8` | 2026-10-31 20:43 -0500 | 16,435 | 2,144,363 | +0 | 0 |
| `951135308f` | 2026-10-31 20:45 -0500 | 16,443 | 2,146,249 | +1,886 | 63,454 |
| `5ae64f4ffb` | 2026-10-31 20:46 -0500 | 16,450 | 2,146,985 | +736 | 34,410 |
| `de5320b7e2` | 2026-10-31 21:19 -0500 | 16,450 | 2,146,985 | +0 | 0 |
| `dd76bc7125` | 2026-10-31 21:30 -0500 | 16,450 | 2,146,985 | +0 | 0 |
| `5d42d50471` | 2026-10-31 21:32 -0500 | 16,451 | 2,147,871 | +886 | 24,348 |
| `48caca217f` | 2026-10-31 21:38 -0500 | 16,456 | 2,150,068 | +2,197 | 21,319 |
| `5c6127897d` | 2026-10-31 21:41 -0500 | 16,456 | 2,150,068 | +0 | 0 |
| `b617d0f6cd` | 2026-10-31 21:44 -0500 | 16,458 | 2,150,164 | +96 | 1,889 |
| `216cad093c` | 2026-10-31 21:46 -0500 | 16,476 | 2,155,647 | +5,483 | 199,382 |
| `ed8c257da8` | 2026-10-31 21:51 -0500 | 16,477 | 2,156,291 | +644 | 7,832 |
| `02bec0f431` | 2026-10-31 22:00 -0500 | 16,478 | 2,156,717 | +426 | 2,700 |
| `1926df8187` | 2026-10-31 22:11 -0500 | 16,478 | 2,156,717 | +0 | 0 |
| `10b20491b3` | 2026-10-31 22:15 -0500 | 16,483 | 2,157,742 | +1,025 | 15,702 |
| `770e176ec5` | 2026-10-31 22:19 -0500 | 16,484 | 2,160,145 | +2,403 | 30,038 |
| `829d118b0c` | 2026-10-31 22:47 -0500 | 16,485 | 2,161,504 | +1,359 | 2,926 |
| `9f393e99fd` | 2026-10-31 23:03 -0500 | 16,486 | 2,161,774 | +270 | 1,031 |
| `495a74fdab` | 2026-10-31 23:12 -0500 | 16,491 | 2,162,332 | +558 | 3,901 |
| `499cb147b0` | 2026-10-31 23:13 -0500 | 16,492 | 2,162,353 | +21 | 879 |
| `52bb7d15bf` | 2026-10-31 23:29 -0500 | 16,494 | 2,162,586 | +233 | 892 |
| `648dad18d0` | 2026-10-31 23:30 -0500 | 16,496 | 2,163,257 | +671 | 29,104 |
| `44e182804c` | 2026-10-31 23:34 -0500 | 16,497 | 2,163,546 | +289 | 4,504 |
| `419cc99a19` | 2026-10-31 23:54 -0500 | 16,502 | 2,164,664 | +1,118 | 3,324 |
| `7320d34964` | 2026-11-01 00:02 -0500 | 16,507 | 2,165,268 | +604 | 4,331 |
| `263506c781` | 2026-11-01 00:07 -0500 | 16,511 | 2,166,300 | +1,032 | 14,920 |
| `cc4a16e8e2` | 2026-11-01 00:08 -0500 | 16,518 | 2,167,676 | +1,376 | 56,291 |
| `0d5dfbbb05` | 2026-11-01 00:10 -0500 | 16,519 | 2,169,223 | +1,547 | 39,220 |
| `8ebf06d4a9` | 2026-11-01 00:17 -0500 | 16,519 | 2,169,223 | +0 | 0 |
| `39a2b88971` | 2026-11-01 00:19 -0500 | 16,519 | 2,169,223 | +0 | 0 |
| `965ea7f52f` | 2026-11-01 00:20 -0500 | 16,522 | 2,171,115 | +1,892 | 86,218 |
| `e5a0069e84` | 2026-11-01 00:32 -0500 | 16,527 | 2,172,117 | +1,002 | 5,031 |
| `b170c57200` | 2026-11-01 00:40 -0500 | 16,530 | 2,173,038 | +921 | 7,224 |
| `65af04f6e4` | 2026-11-01 00:41 -0500 | 16,536 | 2,175,276 | +2,238 | 99,467 |
| `b24d448603` | 2026-11-01 00:46 -0500 | 16,537 | 2,176,207 | +931 | 11,098 |
| `8ab828b123` | 2026-11-01 01:02 -0500 | 16,537 | 2,176,207 | +0 | 0 |
| `cea6ef0180` | 2026-11-01 01:04 -0500 | 16,547 | 2,178,487 | +2,280 | 80,471 |
| `29f2c405fe` | 2026-11-01 01:06 -0500 | 16,551 | 2,178,816 | +329 | 11,964 |
| `1ea7bd34cb` | 2026-11-01 01:33 -0500 | 16,572 | 2,182,360 | +3,544 | 7,832 |
| `6cc72ae57e` | 2026-11-01 01:34 -0500 | 16,573 | 2,182,415 | +55 | 2,475 |
| `ac7cc664b3` | 2026-11-01 01:36 -0500 | 16,573 | 2,182,415 | +0 | 0 |
| `67d8ea646f` | 2026-11-01 01:49 -0500 | 16,575 | 2,182,713 | +298 | 1,460 |
| `9ca7377a5b` | 2026-11-01 01:50 -0500 | 16,576 | 2,183,773 | +1,060 | 40,596 |
| `6fa6420e61` | 2026-11-01 01:10 -0600 | 16,576 | 2,183,773 | +0 | 0 |
| `3c9d9516bc` | 2026-11-01 01:20 -0600 | 16,578 | 2,185,815 | +2,042 | 12,942 |
| `aaa28479bb` | 2026-11-01 01:33 -0600 | 16,578 | 2,185,815 | +0 | 0 |
| `2db205a68e` | 2026-11-01 01:35 -0600 | 16,592 | 2,189,004 | +3,189 | 91,843 |
| `a801c9e6c9` | 2026-11-01 01:36 -0600 | 16,603 | 2,192,430 | +3,426 | 158,123 |
| `e71249fa23` | 2026-11-01 01:56 -0600 | 16,604 | 2,192,546 | +116 | 351 |
| `5826512571` | 2026-11-01 02:06 -0600 | 16,604 | 2,192,546 | +0 | 0 |
| `3ebd6ad21e` | 2026-11-01 02:08 -0600 | 16,615 | 2,195,260 | +2,714 | 81,420 |
| `c980b70946` | 2026-11-01 02:09 -0600 | 16,619 | 2,195,616 | +356 | 16,431 |
| `1b2941ae65` | 2026-11-01 02:11 -0600 | 16,621 | 2,196,227 | +611 | 21,996 |
| `d9171d0df8` | 2026-11-01 02:13 -0600 | 16,625 | 2,197,455 | +1,228 | 39,827 |
| `f12161744d` | 2026-11-01 02:14 -0600 | 16,626 | 2,198,076 | +621 | 23,783 |
| `33cb69e64f` | 2026-11-01 02:24 -0600 | 16,627 | 2,198,754 | +678 | 4,179 |
| `8555515b26` | 2026-11-01 04:55 -0600 | 16,628 | 2,199,102 | +348 | 138 |
| `55780bdf01` | 2026-11-01 04:57 -0600 | 16,628 | 2,199,102 | +0 | 0 |
| `764c13ccdc` | 2026-11-01 04:59 -0600 | 16,630 | 2,199,455 | +353 | 10,769 |
| `56629d9108` | 2026-11-01 05:01 -0600 | 16,630 | 2,199,455 | +0 | 0 |
| `4e4b98f649` | 2026-11-01 05:05 -0600 | 16,630 | 2,199,455 | +0 | 0 |
| `9c6bdf9514` | 2026-11-01 05:07 -0600 | 16,630 | 2,199,455 | +0 | 0 |
| `7f0d30eb25` | 2026-11-01 05:09 -0600 | 16,631 | 2,200,772 | +1,317 | 30,008 |
| `c8adf2e068` | 2026-11-01 05:21 -0600 | 16,632 | 2,200,930 | +158 | 799 |
| `5949ead843` | 2026-11-01 05:26 -0600 | 16,635 | 2,204,580 | +3,650 | 41,582 |
| `a888af0e82` | 2026-11-01 05:29 -0600 | 16,637 | 2,205,743 | +1,163 | 27,912 |
| `865ee239d1` | 2026-11-01 05:45 -0600 | 16,637 | 2,205,743 | +0 | 0 |
| `3a30df99da` | 2026-11-01 05:57 -0600 | 16,637 | 2,205,743 | +0 | 0 |
| `0fcc85e434` | 2026-11-01 06:06 -0600 | 16,638 | 2,206,812 | +1,069 | 6,934 |
| `0a0153653b` | 2026-11-01 06:08 -0600 | 16,640 | 2,207,424 | +612 | 25,036 |
| `f7aacbf3f3` | 2026-11-01 06:17 -0600 | 16,642 | 2,215,280 | +7,856 | 48,098 |
| `8395f62033` | 2026-11-01 06:32 -0600 | 16,643 | 2,219,284 | +4,004 | 16,492 |
| `6d65471f9c` | 2026-11-01 06:49 -0600 | 16,646 | 2,221,183 | +1,899 | 6,605 |
| `380221778d` | 2026-11-01 06:51 -0600 | 16,652 | 2,223,204 | +2,021 | 67,367 |
| `036399e3d0` | 2026-11-01 06:52 -0600 | 16,653 | 2,223,304 | +100 | 4,235 |
| `436875e345` | 2026-11-01 08:04 -0600 | 16,656 | 2,225,496 | +2,192 | 1,825 |
| `09af173c1e` | 2026-11-01 08:06 -0600 | 16,657 | 2,225,629 | +133 | 5,911 |
| `3d59a84b55` | 2026-11-01 08:08 -0600 | 16,657 | 2,225,629 | +0 | 0 |
| `00c25d1862` | 2026-11-01 08:10 -0600 | 16,662 | 2,227,850 | +2,221 | 96,333 |
| `ab94418e90` | 2026-11-01 08:11 -0600 | 16,665 | 2,228,419 | +569 | 24,680 |
| `e96c81b679` | 2026-11-01 08:13 -0600 | 16,665 | 2,228,419 | +0 | 0 |
| `14a107172a` | 2026-11-01 08:14 -0600 | 16,667 | 2,229,817 | +1,398 | 54,704 |
| `4751427bbf` | 2026-11-01 08:16 -0600 | 16,667 | 2,229,817 | +0 | 0 |
| `e9e2792429` | 2026-11-01 08:18 -0600 | 16,667 | 2,229,817 | +0 | 0 |
| `88cbb02655` | 2026-11-01 08:20 -0600 | 16,677 | 2,233,944 | +4,127 | 118,858 |
| `b625f39bbc` | 2026-11-01 08:43 -0600 | 16,685 | 2,238,117 | +4,173 | 11,120 |
| `f34995a0d2` | 2026-11-01 08:44 -0600 | 16,686 | 2,238,142 | +25 | 1,125 |
| `f2642bca3b` | 2026-11-01 08:45 -0600 | 16,687 | 2,238,194 | +52 | 2,255 |
| `cf96b3969b` | 2026-11-01 08:59 -0600 | 16,687 | 2,238,194 | +0 | 0 |
| `83f6d7a196` | 2026-11-01 09:14 -0600 | 16,687 | 2,238,194 | +0 | 0 |
| `0689ee174c` | 2026-11-01 09:16 -0600 | 16,691 | 2,238,951 | +757 | 22,710 |
| `49922d288e` | 2026-11-01 09:17 -0600 | 16,692 | 2,239,435 | +484 | 21,249 |
| `5155c12eb7` | 2026-11-01 09:19 -0600 | 16,693 | 2,239,843 | +408 | 17,486 |
| `951c832162` | 2026-11-01 09:20 -0600 | 16,694 | 2,241,207 | +1,364 | 55,173 |
| `0e3b799d66` | 2026-11-01 09:31 -0600 | 16,694 | 2,241,207 | +0 | 0 |
| `f69e4797f9` | 2026-11-01 09:34 -0600 | 16,697 | 2,242,987 | +1,780 | 43,592 |
| `46261b6f84` | 2026-11-01 09:35 -0600 | 16,698 | 2,243,161 | +174 | 7,369 |
| `6da761d410` | 2026-11-01 09:36 -0600 | 16,699 | 2,243,278 | +117 | 4,955 |
| `3e08c599e3` | 2026-11-01 09:57 -0600 | 16,706 | 2,244,439 | +1,161 | 3,349 |
| `21d8a9a6bb` | 2026-11-01 09:59 -0600 | 16,708 | 2,245,128 | +689 | 22,146 |
| `1de207fd0a` | 2026-11-01 10:10 -0600 | 16,708 | 2,245,128 | +0 | 0 |
| `65e26efe0d` | 2026-11-01 10:13 -0600 | 16,713 | 2,247,125 | +1,997 | 51,351 |
| `7f038f5e76` | 2026-11-01 10:14 -0600 | 16,720 | 2,248,822 | +1,697 | 75,422 |
| `295cc2caed` | 2026-11-01 13:36 -0600 | 16,725 | 2,249,856 | +1,034 | 308 |
| `8b225cd131` | 2026-11-01 13:38 -0600 | 16,727 | 2,251,746 | +1,890 | 52,338 |
| `1d01cb64c5` | 2026-11-01 13:40 -0600 | 16,728 | 2,252,788 | +1,042 | 32,905 |
| `3ec94b023b` | 2026-11-01 13:44 -0600 | 16,728 | 2,252,788 | +0 | 0 |
| `3076676563` | 2026-11-01 13:45 -0600 | 16,729 | 2,253,895 | +1,107 | 46,885 |
| `685010b181` | 2026-11-01 13:48 -0600 | 16,729 | 2,253,895 | +0 | 0 |
| `d796a12000` | 2026-11-01 13:54 -0600 | 16,730 | 2,255,718 | +1,823 | 16,085 |
| `bc4133275f` | 2026-11-01 13:56 -0600 | 16,731 | 2,255,855 | +137 | 6,089 |
| `e1e5d86f4b` | 2026-11-01 13:57 -0600 | 16,735 | 2,259,473 | +3,618 | 138,562 |
| `f38ea37548` | 2026-11-01 14:03 -0600 | 16,735 | 2,259,473 | +0 | 0 |
| `372058e347` | 2026-11-01 14:06 -0600 | 16,738 | 2,262,294 | +2,821 | 58,366 |
| `53f898fb60` | 2026-11-01 14:07 -0600 | 16,739 | 2,263,571 | +1,277 | 52,841 |
| `6e7a1bf575` | 2026-11-01 14:10 -0600 | 16,741 | 2,263,922 | +351 | 6,943 |
| `9130d26823` | 2026-11-01 14:16 -0600 | 16,743 | 2,264,283 | +361 | 3,868 |
| `423982d9cc` | 2026-11-01 14:34 -0600 | 16,747 | 2,265,921 | +1,638 | 5,385 |
| `ecbc120b5b` | 2026-11-01 14:36 -0600 | 16,748 | 2,266,449 | +528 | 19,596 |
| `136943f020` | 2026-11-01 14:44 -0600 | 16,749 | 2,266,854 | +405 | 2,864 |
| `2843914c75` | 2026-11-01 14:57 -0600 | 16,751 | 2,268,678 | +1,824 | 8,686 |
| `647fd96544` | 2026-11-01 14:58 -0600 | 16,752 | 2,269,778 | +1,100 | 46,047 |
| `51909ed5ca` | 2026-11-01 15:03 -0600 | 16,754 | 2,270,675 | +897 | 11,411 |
| `ff6d055c40` | 2026-11-01 15:12 -0600 | 16,756 | 2,271,153 | +478 | 3,222 |
| `1df0439145` | 2026-11-01 15:16 -0600 | 16,756 | 2,271,153 | +0 | 0 |
| `0bae24234f` | 2026-11-01 15:18 -0600 | 16,757 | 2,272,583 | +1,430 | 44,000 |
| `1b5c71e223` | 2026-11-01 15:20 -0600 | 16,758 | 2,273,459 | +876 | 27,186 |
| `052b53adf3` | 2026-11-01 15:22 -0600 | 16,759 | 2,274,296 | +837 | 25,536 |
| `f383daa0f9` | 2026-11-01 15:31 -0600 | 16,759 | 2,274,296 | +0 | 0 |
| `80fa21abda` | 2026-11-01 15:43 -0600 | 16,762 | 2,275,523 | +1,227 | 6,101 |
| `355cf37274` | 2026-11-01 15:44 -0600 | 16,763 | 2,275,555 | +32 | 1,200 |
| `40abc51b9f` | 2026-11-01 15:52 -0600 | 16,765 | 2,276,369 | +814 | 5,993 |
| `e2a06d8035` | 2026-11-01 16:03 -0600 | 16,765 | 2,276,369 | +0 | 0 |
| `1ee4fad2e8` | 2026-11-01 16:05 -0600 | 16,766 | 2,278,134 | +1,765 | 62,911 |
| `df8fd727f9` | 2026-11-01 16:14 -0600 | 16,768 | 2,278,851 | +717 | 4,736 |
| `95f097bd8e` | 2026-11-01 16:18 -0600 | 16,770 | 2,281,091 | +2,240 | 30,545 |
| `ee37f32c9e` | 2026-11-01 16:21 -0600 | 16,770 | 2,281,091 | +0 | 0 |
| `04174c6d13` | 2026-11-01 16:28 -0600 | 16,771 | 2,282,003 | +912 | 7,911 |
| `ca50413b7a` | 2026-11-01 16:34 -0600 | 16,773 | 2,283,955 | +1,952 | 18,941 |
| `33ae105739` | 2026-11-01 16:36 -0600 | 16,774 | 2,285,846 | +1,891 | 66,093 |
| `1d4a88ee92` | 2026-11-01 16:38 -0600 | 16,775 | 2,297,012 | +11,166 | 287,126 |
| `ea7ea45db0` | 2026-11-01 16:40 -0600 | 16,776 | 2,298,195 | +1,183 | 34,624 |
| `076ee7c58c` | 2026-11-01 16:46 -0600 | 16,776 | 2,298,195 | +0 | 0 |
| `072d03bbeb` | 2026-11-01 16:48 -0600 | 16,778 | 2,302,299 | +4,104 | 167,891 |
| `1d7dea91c7` | 2026-11-01 16:58 -0600 | 16,779 | 2,302,545 | +246 | 1,493 |
| `361cc9ebcf` | 2026-11-01 17:04 -0600 | 16,780 | 2,302,833 | +288 | 2,728 |
| `13b09c815b` | 2026-11-01 17:08 -0600 | 16,783 | 2,303,674 | +841 | 13,888 |
| `b87dbdac37` | 2026-11-01 17:09 -0600 | 16,784 | 2,303,856 | +182 | 6,552 |
| `4001161100` | 2026-11-01 17:16 -0600 | 16,787 | 2,308,132 | +4,276 | 36,478 |
| `3f1c2be72d` | 2026-11-01 17:18 -0600 | 16,788 | 2,308,240 | +108 | 3,927 |
| `52519e16d6` | 2026-11-01 17:20 -0600 | 16,789 | 2,308,772 | +532 | 20,160 |
| `9b8e8be069` | 2026-11-01 17:21 -0600 | 16,793 | 2,310,263 | +1,491 | 60,310 |
| `37ecf04731` | 2026-11-01 17:22 -0600 | 16,795 | 2,310,888 | +625 | 25,000 |
| `8417be190c` | 2026-11-01 17:24 -0600 | 16,799 | 2,312,252 | +1,364 | 57,769 |
| `4a50f6f043` | 2026-11-01 17:30 -0600 | 16,800 | 2,314,039 | +1,787 | 18,276 |
| `464f5cb412` | 2026-11-01 17:40 -0600 | 16,801 | 2,315,191 | +1,152 | 6,625 |
| `462c617ebc` | 2026-11-01 17:43 -0600 | 16,802 | 2,315,427 | +236 | 4,968 |
| `661a996874` | 2026-11-01 17:48 -0600 | 16,803 | 2,317,983 | +2,556 | 28,936 |
| `de3e6a0d3e` | 2026-11-01 17:50 -0600 | 16,808 | 2,318,231 | +248 | 10,504 |
| `7b22ff6bb1` | 2026-11-01 17:52 -0600 | 16,809 | 2,319,238 | +1,007 | 34,858 |
| `2b4d187826` | 2026-11-01 17:58 -0600 | 16,810 | 2,320,923 | +1,685 | 15,797 |
| `d24e8327dc` | 2026-11-01 18:12 -0600 | 16,813 | 2,321,882 | +959 | 4,115 |
| `fa55471928` | 2026-11-01 18:15 -0600 | 16,814 | 2,324,250 | +2,368 | 49,276 |
| `cff82f1530` | 2026-11-01 18:17 -0600 | 16,814 | 2,324,250 | +0 | 0 |
| `e04ea5105a` | 2026-11-01 18:20 -0600 | 16,816 | 2,325,224 | +974 | 19,589 |
| `12cc077c33` | 2026-11-01 18:22 -0600 | 16,817 | 2,325,261 | +37 | 1,148 |
| `0ce50770bc` | 2026-11-01 18:24 -0600 | 16,819 | 2,326,793 | +1,532 | 52,526 |
| `85b0e7cd3b` | 2026-11-01 18:26 -0600 | 16,820 | 2,327,315 | +522 | 12,363 |
| `ca4bbd4bad` | 2026-11-01 18:28 -0600 | 16,823 | 2,327,663 | +348 | 11,819 |
| `be607c4b67` | 2026-11-01 18:37 -0600 | 16,824 | 2,327,907 | +244 | 1,577 |
| `7988d0c00e` | 2026-11-01 18:39 -0600 | 16,826 | 2,329,399 | +1,492 | 58,383 |
| `f95f7f9c58` | 2026-11-01 18:41 -0600 | 16,829 | 2,330,586 | +1,187 | 34,186 |
| `97b11c5a0e` | 2026-11-01 18:43 -0600 | 16,832 | 2,333,703 | +3,117 | 80,151 |
| `a9141cff90` | 2026-11-01 18:45 -0600 | 16,841 | 2,335,573 | +1,870 | 65,359 |
| `986bcdc693` | 2026-11-01 18:48 -0600 | 16,841 | 2,335,573 | +0 | 0 |
| `f0c9ce2205` | 2026-11-01 18:52 -0600 | 16,841 | 2,335,573 | +0 | 0 |
| `08672678c4` | 2026-11-01 18:54 -0600 | 16,842 | 2,335,798 | +225 | 6,532 |
| `c6dbb2eec8` | 2026-11-01 18:56 -0600 | 16,846 | 2,336,738 | +940 | 35,250 |
| `81bb03f806` | 2026-11-01 18:57 -0600 | 16,847 | 2,338,644 | +1,906 | 76,240 |
| `e0e597519c` | 2026-11-01 19:05 -0600 | 16,849 | 2,340,589 | +1,945 | 15,950 |
| `9b7c103495` | 2026-11-01 19:08 -0600 | 16,851 | 2,341,092 | +503 | 8,231 |
| `879d290014` | 2026-11-01 19:10 -0600 | 16,852 | 2,341,383 | +291 | 9,791 |
| `f7527b22ee` | 2026-11-01 19:17 -0600 | 16,852 | 2,341,383 | +0 | 0 |
| `91b80afe9c` | 2026-11-01 19:19 -0600 | 16,853 | 2,342,267 | +884 | 31,824 |
| `533ca78bcb` | 2026-11-01 19:20 -0600 | 16,855 | 2,342,855 | +588 | 18,900 |
| `8eb25f4362` | 2026-11-01 19:22 -0600 | 16,856 | 2,343,217 | +362 | 15,153 |
| `837185e8da` | 2026-11-01 19:24 -0600 | 16,856 | 2,343,217 | +0 | 0 |
| `ee424be935` | 2026-11-01 19:31 -0600 | 16,858 | 2,344,434 | +1,217 | 10,003 |
| `fec3001e60` | 2026-11-01 19:37 -0600 | 16,859 | 2,346,237 | +1,803 | 18,598 |
| `921050eb1a` | 2026-11-01 19:43 -0600 | 16,869 | 2,348,001 | +1,764 | 17,494 |
| `631b95d5fb` | 2026-11-01 19:46 -0600 | 16,870 | 2,348,773 | +772 | 13,827 |
| `473aae1fd5` | 2026-11-01 19:50 -0600 | 16,870 | 2,348,773 | +0 | 0 |
| `a9f60161da` | 2026-11-01 19:54 -0600 | 16,872 | 2,349,342 | +569 | 8,194 |
| `2e01f14cc2` | 2026-11-01 19:55 -0600 | 16,876 | 2,349,853 | +511 | 22,164 |
| `cdb0d9da80` | 2026-11-01 19:57 -0600 | 16,879 | 2,350,165 | +312 | 11,949 |
| `0f62cc0b50` | 2026-11-01 19:59 -0600 | 16,880 | 2,350,493 | +328 | 9,225 |
| `075192f5e9` | 2026-11-01 20:01 -0600 | 16,881 | 2,351,512 | +1,019 | 39,026 |
| `60c27970dc` | 2026-11-01 20:02 -0600 | 16,882 | 2,352,111 | +599 | 19,427 |
| `8705b584eb` | 2026-11-01 20:06 -0600 | 16,883 | 2,354,179 | +2,068 | 32,369 |
| `2cccd3b148` | 2026-11-01 20:08 -0600 | 16,884 | 2,354,395 | +216 | 7,200 |
| `e299c22637` | 2026-11-01 20:13 -0600 | 16,886 | 2,355,563 | +1,168 | 14,651 |
| `a9d432c7ce` | 2026-11-01 20:17 -0600 | 16,888 | 2,355,715 | +152 | 2,465 |
| `6c202f0cac` | 2026-11-01 20:20 -0600 | 16,889 | 2,355,920 | +205 | 3,167 |
| `4c1ae54c1c` | 2026-11-01 20:25 -0600 | 16,893 | 2,356,380 | +460 | 6,419 |
| `fe8be4bec8` | 2026-11-01 20:27 -0600 | 16,895 | 2,356,799 | +419 | 10,056 |
| `4ece3b3a6e` | 2026-11-01 20:29 -0600 | 16,898 | 2,358,202 | +1,403 | 39,153 |
| `9779cfb3e7` | 2026-11-01 20:31 -0600 | 16,900 | 2,358,579 | +377 | 12,804 |
| `3e51b6f4f3` | 2026-11-01 20:33 -0600 | 16,901 | 2,358,708 | +129 | 4,788 |
| `88f54e0866` | 2026-11-01 20:34 -0600 | 16,902 | 2,358,741 | +33 | 1,212 |
| `265a538f5d` | 2026-11-01 20:39 -0600 | 16,911 | 2,359,698 | +957 | 13,511 |
| `6c18d31e9d` | 2026-11-01 20:41 -0600 | 16,912 | 2,360,222 | +524 | 14,854 |
| `581a6ac08b` | 2026-11-01 20:43 -0600 | 16,916 | 2,360,861 | +639 | 20,913 |
| `ece4e6d9b5` | 2026-11-01 20:45 -0600 | 16,918 | 2,361,112 | +251 | 7,996 |
| `535d558b3e` | 2026-11-01 20:46 -0600 | 16,919 | 2,361,525 | +413 | 13,275 |
| `0d6e557858` | 2026-11-01 20:51 -0600 | 16,920 | 2,361,860 | +335 | 4,173 |
| `5e05e59528` | 2026-11-01 20:53 -0600 | 16,921 | 2,361,887 | +27 | 972 |
| `83914ef8ca` | 2026-11-01 20:55 -0600 | 16,924 | 2,362,149 | +262 | 8,815 |
| `98577d36fe` | 2026-11-01 20:57 -0600 | 16,925 | 2,362,545 | +396 | 12,185 |
| `db34d88e34` | 2026-11-01 20:58 -0600 | 16,926 | 2,363,123 | +578 | 18,916 |
| `bf75dc4563` | 2026-11-01 21:02 -0600 | 16,928 | 2,364,013 | +890 | 13,751 |
| `0a6f26fbd2` | 2026-11-01 21:10 -0600 | 16,933 | 2,364,159 | +146 | 1,109 |
| `de27a3e6c3` | 2026-11-01 21:16 -0600 | 16,934 | 2,364,692 | +533 | 5,996 |
| `e55cf71f23` | 2026-11-01 21:21 -0600 | 16,934 | 2,364,692 | +0 | 0 |
| `6b673ea250` | 2026-11-01 21:23 -0600 | 16,936 | 2,366,246 | +1,554 | 44,400 |
| `24ee0f0aaf` | 2026-11-01 21:27 -0600 | 16,938 | 2,368,369 | +2,123 | 28,099 |
| `c8daa37317` | 2026-11-01 21:30 -0600 | 16,938 | 2,368,369 | +0 | 0 |
| `a4370b3993` | 2026-11-01 21:32 -0600 | 16,940 | 2,369,146 | +777 | 27,972 |
| `049465031e` | 2026-11-01 21:34 -0600 | 16,942 | 2,371,229 | +2,083 | 62,490 |
| `d43ddce5a8` | 2026-11-01 21:37 -0600 | 16,943 | 2,371,734 | +505 | 10,156 |
| `8c400e00c0` | 2026-11-01 21:38 -0600 | 16,944 | 2,372,385 | +651 | 21,903 |
| `9919cd4957` | 2026-11-01 21:40 -0600 | 16,945 | 2,372,823 | +438 | 12,614 |
| `4b7d805521` | 2026-11-01 21:44 -0600 | 16,946 | 2,373,749 | +926 | 14,949 |
| `f1a3f52b9f` | 2026-11-01 21:58 -0600 | 16,946 | 2,373,749 | +0 | 0 |
| `a881c01084` | 2026-11-01 22:01 -0600 | 16,947 | 2,374,084 | +335 | 8,739 |
| `573ed23975` | 2026-11-01 22:03 -0600 | 16,948 | 2,374,408 | +324 | 8,514 |
| `37436d21c0` | 2026-11-01 22:07 -0600 | 16,949 | 2,374,886 | +478 | 6,829 |
| `9ab9d8fac5` | 2026-11-01 22:09 -0600 | 16,950 | 2,376,082 | +1,196 | 35,880 |
| `807849c7d6` | 2026-11-01 22:12 -0600 | 16,951 | 2,376,756 | +674 | 16,395 |
| `a45b141f14` | 2026-11-01 22:17 -0600 | 16,953 | 2,376,864 | +108 | 1,161 |
| `b7b983d3a3` | 2026-11-01 22:34 -0600 | 16,953 | 2,376,864 | +0 | 0 |
| `3e36613873` | 2026-11-01 22:38 -0600 | 16,953 | 2,376,864 | +0 | 0 |
| `810c87f18f` | 2026-11-01 22:40 -0600 | 16,953 | 2,376,864 | +0 | 0 |
| `ae39a8b850` | 2026-11-01 22:45 -0600 | 16,954 | 2,377,640 | +776 | 9,129 |
| `62cb2149e7` | 2026-11-01 22:50 -0600 | 16,955 | 2,378,012 | +372 | 4,198 |
| `d57050a57a` | 2026-11-01 22:52 -0600 | 16,955 | 2,378,012 | +0 | 0 |
| `f5764441de` | 2026-11-01 22:54 -0600 | 16,956 | 2,378,439 | +427 | 15,220 |
| `b6ae7b583c` | 2026-11-01 22:56 -0600 | 16,957 | 2,378,865 | +426 | 13,336 |
| `72bb2f6861` | 2026-11-01 23:14 -0600 | 16,958 | 2,378,953 | +88 | 293 |
| `993e15cbb8` | 2026-11-01 23:16 -0600 | 16,959 | 2,380,138 | +1,185 | 34,967 |
| `82d2d85634` | 2026-11-01 23:29 -0600 | 16,962 | 2,381,563 | +1,425 | 6,645 |
| `4992c514b2` | 2026-11-01 23:31 -0600 | 16,963 | 2,381,991 | +428 | 13,058 |
| `fea6edfe73` | 2026-11-01 23:37 -0600 | 16,963 | 2,381,991 | +0 | 0 |
| `e98678b0a9` | 2026-11-01 23:39 -0600 | 16,964 | 2,382,430 | +439 | 15,196 |
| `be7cc771d3` | 2026-11-01 23:42 -0600 | 16,964 | 2,382,430 | +0 | 0 |
| `6ec4db557b` | 2026-11-02 00:01 -0600 | 16,965 | 2,384,339 | +1,909 | 5,986 |
| `744127d4e7` | 2026-11-02 00:05 -0600 | 16,966 | 2,384,478 | +139 | 2,042 |
| `508885cb7e` | 2026-11-02 00:07 -0600 | 16,968 | 2,385,544 | +1,066 | 38,376 |
| `a38e65be39` | 2026-11-02 00:09 -0600 | 16,968 | 2,385,544 | +0 | 0 |
| `ffff8e5f86` | 2026-11-02 00:21 -0600 | 16,969 | 2,385,549 | +5 | 24 |
| `243c095204` | 2026-11-02 00:24 -0600 | 16,970 | 2,386,637 | +1,088 | 23,040 |
| `17d3464a2a` | 2026-11-02 00:29 -0600 | 16,970 | 2,386,637 | +0 | 0 |
| `b7bb25cdfa` | 2026-11-02 00:31 -0600 | 16,971 | 2,387,912 | +1,275 | 48,316 |
| `688ee11ec6` | 2026-11-02 00:42 -0600 | 16,971 | 2,387,912 | +0 | 0 |
| `3bf83b2e2f` | 2026-11-02 00:44 -0600 | 16,972 | 2,388,127 | +215 | 6,789 |
| `2372470024` | 2026-11-02 00:46 -0600 | 16,972 | 2,388,127 | +0 | 0 |
| `20d714a05a` | 2026-11-02 00:48 -0600 | 16,972 | 2,388,127 | +0 | 0 |
| `880c211614` | 2026-11-02 00:50 -0600 | 16,973 | 2,388,836 | +709 | 20,921 |
| `212d0fe15c` | 2026-11-02 00:54 -0600 | 16,974 | 2,390,823 | +1,987 | 38,252 |
| `f708f54714` | 2026-11-02 01:04 -0600 | 16,974 | 2,390,823 | +0 | 0 |
| `2011fe3bbb` | 2026-11-02 01:06 -0600 | 16,975 | 2,390,987 | +164 | 6,634 |
| `22bad5223c` | 2026-11-02 01:07 -0600 | 16,976 | 2,391,359 | +372 | 13,950 |
| `00e45ddde7` | 2026-11-02 01:14 -0600 | 16,978 | 2,391,664 | +305 | 2,725 |
| `4b1f35fa51` | 2026-11-02 01:16 -0600 | 16,980 | 2,391,888 | +224 | 7,536 |
| `a890dae09c` | 2026-11-02 01:18 -0600 | 16,981 | 2,392,153 | +265 | 6,814 |
| `24ba6f2027` | 2026-11-02 01:28 -0600 | 16,983 | 2,392,962 | +809 | 5,083 |
| `e392e9b0ea` | 2026-11-02 01:32 -0600 | 16,985 | 2,394,131 | +1,169 | 17,107 |
| `f09f8dea15` | 2026-11-02 01:34 -0600 | 16,985 | 2,394,131 | +0 | 0 |
| `c18599600f` | 2026-11-02 01:36 -0600 | 16,989 | 2,394,621 | +490 | 17,126 |
| `1653ed9790` | 2026-11-02 01:44 -0600 | 16,989 | 2,394,621 | +0 | 0 |
| `619076c6fa` | 2026-11-02 01:46 -0600 | 16,990 | 2,394,968 | +347 | 13,012 |
| `f92ff5cb07` | 2026-11-02 01:48 -0600 | 16,991 | 2,395,379 | +411 | 12,646 |
| `427bc0e9b5` | 2026-11-02 01:51 -0600 | 16,992 | 2,395,451 | +72 | 1,098 |
| `17e8e5b0ab` | 2026-11-02 01:59 -0600 | 16,992 | 2,395,451 | +0 | 0 |
| `02e7daaf5d` | 2026-11-02 02:00 -0600 | 16,993 | 2,395,796 | +345 | 13,800 |
| `a4fcbc2c9c` | 2026-11-02 02:09 -0600 | 16,994 | 2,395,887 | +91 | 607 |
| `63230d1a3a` | 2026-11-02 02:16 -0600 | 16,998 | 2,396,788 | +901 | 8,232 |
| `b51f82272b` | 2026-11-02 02:19 -0600 | 16,999 | 2,396,899 | +111 | 1,998 |
| `d04e1edf65` | 2026-11-02 02:21 -0600 | 17,000 | 2,397,385 | +486 | 13,155 |
| `cd2e61e81a` | 2026-11-02 02:24 -0600 | 17,004 | 2,398,188 | +803 | 18,531 |
| `0f828e0f3f` | 2026-11-02 02:26 -0600 | 17,006 | 2,398,281 | +93 | 3,282 |
| `8734608fc1` | 2026-11-02 02:27 -0600 | 17,006 | 2,398,281 | +0 | 0 |
| `90bf01620b` | 2026-11-02 02:29 -0600 | 17,007 | 2,398,590 | +309 | 9,508 |
| `0b21367ee5` | 2026-11-02 02:32 -0600 | 17,007 | 2,398,590 | +0 | 0 |
| `eec36f8b38` | 2026-11-02 02:35 -0600 | 17,008 | 2,399,475 | +885 | 19,309 |
| `aa7d9a6575` | 2026-11-02 02:39 -0600 | 17,008 | 2,399,475 | +0 | 0 |
| `fe6b8d4dca` | 2026-11-02 02:40 -0600 | 17,009 | 2,399,542 | +67 | 2,594 |
| `167c487f12` | 2026-11-02 02:47 -0600 | 17,009 | 2,399,542 | +0 | 0 |
| `18b2280f69` | 2026-11-02 02:48 -0600 | 17,010 | 2,400,219 | +677 | 23,435 |
| `8082a5154b` | 2026-11-02 02:53 -0600 | 17,014 | 2,400,603 | +384 | 4,670 |
| `0ac57838fe` | 2026-11-02 02:58 -0600 | 17,014 | 2,400,603 | +0 | 0 |
| `b36ac80671` | 2026-11-02 03:00 -0600 | 17,015 | 2,400,843 | +240 | 8,229 |
| `e68ea3705c` | 2026-11-02 03:24 -0600 | 17,020 | 2,401,144 | +301 | 756 |
| `b403bd36f1` | 2026-11-02 03:25 -0600 | 17,021 | 2,401,769 | +625 | 22,059 |
| `fb27dba8c4` | 2026-11-02 03:28 -0600 | 17,022 | 2,401,878 | +109 | 2,437 |
| `83bebd9a8f` | 2026-11-02 03:30 -0600 | 17,023 | 2,402,002 | +124 | 3,434 |
| `9aa8e167c5` | 2026-11-02 03:39 -0600 | 17,024 | 2,404,430 | +2,428 | 16,247 |
| `8000db057c` | 2026-11-02 03:42 -0600 | 17,028 | 2,404,690 | +260 | 6,985 |
| `04ee1fc596` | 2026-11-02 03:43 -0600 | 17,029 | 2,405,004 | +314 | 11,535 |
| `7d1812f9f3` | 2026-11-02 03:51 -0600 | 17,030 | 2,405,416 | +412 | 3,129 |
| `8e961a6fff` | 2026-11-02 03:58 -0600 | 17,031 | 2,405,745 | +329 | 3,045 |
| `bd6b5dee43` | 2026-11-02 04:13 -0600 | 17,038 | 2,405,910 | +165 | 644 |
| `cb67673fe6` | 2026-11-02 04:18 -0600 | 17,039 | 2,406,189 | +279 | 3,512 |
| `3ec68fc4fa` | 2026-11-02 04:30 -0600 | 17,042 | 2,406,453 | +264 | 1,264 |
| `320baa2790` | 2026-11-02 04:32 -0600 | 17,043 | 2,406,481 | +28 | 826 |
| `0f14dec5d5` | 2026-11-02 04:36 -0600 | 17,044 | 2,407,317 | +836 | 14,400 |
| `697365b192` | 2026-11-02 04:43 -0600 | 17,045 | 2,407,637 | +320 | 2,803 |
| `e09bbd1876` | 2026-11-02 04:47 -0600 | 17,047 | 2,408,153 | +516 | 7,490 |
| `fea61dc9ae` | 2026-11-02 04:57 -0600 | 17,047 | 2,408,153 | +0 | 0 |
| `904274009b` | 2026-11-02 04:59 -0600 | 17,048 | 2,408,704 | +551 | 16,530 |
| `da6224f55c` | 2026-11-02 05:01 -0600 | 17,057 | 2,409,516 | +812 | 23,766 |
| `9515a2ce9b` | 2026-11-02 05:03 -0600 | 17,058 | 2,413,337 | +3,821 | 87,615 |
| `0bc4427b5b` | 2026-11-02 05:06 -0600 | 17,062 | 2,421,707 | +8,370 | 215,229 |
| `4dbe1b5788` | 2026-11-02 05:08 -0600 | 17,062 | 2,421,707 | +0 | 0 |
| `8df1881148` | 2026-11-02 05:10 -0600 | 17,065 | 2,421,749 | +42 | 900 |
| `a77d4ec7f9` | 2026-11-02 05:17 -0600 | 17,070 | 2,422,866 | +1,117 | 9,666 |
| `5fe57c534a` | 2026-11-02 05:19 -0600 | 17,076 | 2,424,068 | +1,202 | 35,180 |
| `62084f337e` | 2026-11-02 05:23 -0600 | 17,081 | 2,424,295 | +227 | 4,148 |
| `a347723fd0` | 2026-11-02 05:25 -0600 | 17,082 | 2,424,456 | +161 | 4,712 |
| `51711a589a` | 2026-11-02 05:31 -0600 | 17,083 | 2,425,537 | +1,081 | 10,750 |
| `5c3205d541` | 2026-11-02 05:33 -0600 | 17,091 | 2,427,392 | +1,855 | 58,070 |
| `a640d82bee` | 2026-11-02 05:35 -0600 | 17,092 | 2,427,658 | +266 | 6,514 |
| `4bcd0ebd3b` | 2026-11-02 05:40 -0600 | 17,094 | 2,430,060 | +2,402 | 30,341 |
| `1e03045f62` | 2026-11-02 05:43 -0600 | 17,095 | 2,431,216 | +1,156 | 26,339 |
| `76fd1ab834` | 2026-11-02 05:48 -0600 | 17,096 | 2,431,278 | +62 | 643 |
| `ad5b85a534` | 2026-11-02 05:50 -0600 | 17,097 | 2,431,733 | +455 | 16,887 |
| `e32cf31ab8` | 2026-11-02 05:54 -0600 | 17,099 | 2,431,874 | +141 | 1,887 |
| `062cee63d9` | 2026-11-02 05:56 -0600 | 17,102 | 2,432,233 | +359 | 12,796 |
| `79c0f546ad` | 2026-11-02 06:02 -0600 | 17,102 | 2,432,233 | +0 | 0 |
| `08f0b5822c` | 2026-11-02 06:04 -0600 | 17,103 | 2,432,339 | +106 | 3,053 |
| `a0700fc358` | 2026-11-02 06:06 -0600 | 17,108 | 2,432,640 | +301 | 10,520 |
| `be2680cee0` | 2026-11-02 06:10 -0600 | 17,117 | 2,432,893 | +253 | 3,530 |
| `587c3ae9db` | 2026-11-02 06:13 -0600 | 17,125 | 2,433,300 | +407 | 10,617 |
| `ef3007a60a` | 2026-11-02 06:16 -0600 | 17,129 | 2,433,672 | +372 | 7,200 |
| `1e2fe5d1ca` | 2026-11-02 06:18 -0600 | 17,176 | 2,435,176 | +1,504 | 37,341 |
| `7913387024` | 2026-11-02 06:25 -0600 | 17,178 | 2,435,249 | +73 | 667 |
| `3d444e978e` | 2026-11-02 06:27 -0600 | 17,179 | 2,435,271 | +22 | 535 |
| `fcfbf9fd28` | 2026-11-02 06:29 -0600 | 17,180 | 2,435,419 | +148 | 4,800 |
| `bdf99e9256` | 2026-11-02 06:32 -0600 | 17,181 | 2,435,458 | +39 | 735 |
| `add6d1e9a7` | 2026-11-02 06:35 -0600 | 17,182 | 2,435,491 | +33 | 656 |
| `f5523e9afe` | 2026-11-02 06:37 -0600 | 17,184 | 2,436,103 | +612 | 19,326 |
| `7aadb7f57d` | 2026-11-02 06:39 -0600 | 17,185 | 2,436,811 | +708 | 20,555 |
| `9773e79e73` | 2026-11-02 06:41 -0600 | 17,188 | 2,437,019 | +208 | 7,131 |
| `3b00a25661` | 2026-11-02 06:43 -0600 | 17,189 | 2,437,206 | +187 | 5,301 |
| `0d430afc49` | 2026-11-02 06:45 -0600 | 17,190 | 2,437,296 | +90 | 3,560 |
| `8e818d13ce` | 2026-11-02 06:49 -0600 | 17,191 | 2,437,385 | +89 | 1,116 |
| `74baa4b892` | 2026-11-02 06:51 -0600 | 17,194 | 2,437,476 | +91 | 2,978 |
| `e9a98e36bd` | 2026-11-02 06:53 -0600 | 17,195 | 2,437,487 | +11 | 305 |
| `e204db5bbe` | 2026-11-02 06:56 -0600 | 17,196 | 2,437,505 | +18 | 510 |
| `ab0328d8bd` | 2026-11-02 06:58 -0600 | 17,197 | 2,437,521 | +16 | 414 |
| `3848b0e506` | 2026-11-02 07:01 -0600 | 17,197 | 2,437,521 | +0 | 0 |
| `ce6c00d980` | 2026-11-02 07:03 -0600 | 17,198 | 2,437,590 | +69 | 2,179 |
| `4bd001535e` | 2026-11-02 07:05 -0600 | 17,199 | 2,437,605 | +15 | 607 |
| `7ee979e48d` | 2026-11-02 07:06 -0600 | 17,201 | 2,438,738 | +1,133 | 43,391 |
| `11e104dc17` | 2026-11-02 07:11 -0600 | 17,202 | 2,438,902 | +164 | 1,981 |
| `deb6499918` | 2026-11-02 07:19 -0600 | 17,213 | 2,442,607 | +3,705 | 28,439 |
| `9be6b935e1` | 2026-11-02 07:21 -0600 | 17,214 | 2,442,681 | +74 | 2,960 |
| `c6974ca20a` | 2026-11-02 07:26 -0600 | 17,215 | 2,442,780 | +99 | 1,200 |
| `fd57a6ed20` | 2026-11-02 07:31 -0600 | 17,222 | 2,444,570 | +1,790 | 19,468 |
| `c7794f7aaa` | 2026-11-02 07:37 -0600 | 17,226 | 2,444,929 | +359 | 3,522 |
| `58c612ff72` | 2026-11-02 07:44 -0600 | 17,228 | 2,447,592 | +2,663 | 24,332 |
| `0ecefb3610` | 2026-11-02 07:49 -0600 | 17,231 | 2,448,048 | +456 | 4,975 |
| `1f3ae5b662` | 2026-11-02 07:56 -0600 | 17,241 | 2,449,259 | +1,211 | 11,178 |
| `12e40f723b` | 2026-11-02 07:58 -0600 | 17,243 | 2,451,750 | +2,491 | 66,922 |
| `0798acf0d6` | 2026-11-02 08:00 -0600 | 17,244 | 2,451,777 | +27 | 917 |
| `916ea7c406` | 2026-11-02 08:02 -0600 | 17,251 | 2,452,061 | +284 | 8,380 |
| `404e931965` | 2026-11-02 08:04 -0600 | 17,253 | 2,452,354 | +293 | 9,253 |
| `0b681586c6` | 2026-11-02 08:06 -0600 | 17,254 | 2,452,364 | +10 | 308 |
| `fc24a0499b` | 2026-11-02 08:08 -0600 | 17,260 | 2,452,825 | +461 | 14,307 |
| `56b6d9429c` | 2026-11-02 08:12 -0600 | 17,262 | 2,453,065 | +240 | 3,212 |
| `931b216870` | 2026-11-02 08:14 -0600 | 17,264 | 2,453,480 | +415 | 15,091 |
| `ce88c56592` | 2026-11-02 08:15 -0600 | 17,265 | 2,454,127 | +647 | 23,292 |
| `b9dacc4ecf` | 2026-11-02 08:18 -0600 | 17,266 | 2,454,181 | +54 | 1,080 |
| `f29bdf3d47` | 2026-11-02 08:21 -0600 | 17,267 | 2,454,396 | +215 | 4,230 |
| `f0520e30c2` | 2026-11-02 08:23 -0600 | 17,269 | 2,455,125 | +729 | 21,870 |
| `a9ed145e84` | 2026-11-02 08:29 -0600 | 17,269 | 2,455,125 | +0 | 0 |
| `e95d50543d` | 2026-11-02 08:36 -0600 | 17,271 | 2,455,821 | +696 | 5,909 |
| `9418941134` | 2026-11-02 08:37 -0600 | 17,272 | 2,455,937 | +116 | 4,094 |
| `4c6cc4ff4e` | 2026-11-02 08:42 -0600 | 17,273 | 2,456,093 | +156 | 2,194 |
| `0b655086e9` | 2026-11-02 08:43 -0600 | 17,275 | 2,456,701 | +608 | 21,250 |
| `f8dc96fcc3` | 2026-11-02 08:51 -0600 | 17,275 | 2,456,701 | +0 | 0 |
| `7e0c79a44d` | 2026-11-02 08:53 -0600 | 17,276 | 2,460,263 | +3,562 | 88,436 |
| `a595498b89` | 2026-11-02 08:55 -0600 | 17,277 | 2,460,812 | +549 | 18,300 |
| `03cb967df2` | 2026-11-02 08:59 -0600 | 17,279 | 2,461,459 | +647 | 9,392 |
| `435f88d044` | 2026-11-02 09:02 -0600 | 17,279 | 2,461,459 | +0 | 0 |
| `946ad8878d` | 2026-11-02 09:04 -0600 | 17,279 | 2,461,459 | +0 | 0 |
| `93ba244b48` | 2026-11-02 09:11 -0600 | 17,281 | 2,461,570 | +111 | 914 |
| `06e4c037db` | 2026-11-02 09:21 -0600 | 17,282 | 2,461,593 | +23 | 137 |
| `2c897d8ec4` | 2026-11-02 09:24 -0600 | 17,283 | 2,462,897 | +1,304 | 30,682 |
| `4f96efcd4e` | 2026-11-02 09:26 -0600 | 17,284 | 2,462,943 | +46 | 1,465 |
| `82bc1f5793` | 2026-11-02 09:28 -0600 | 17,284 | 2,462,943 | +0 | 0 |
| `827003056d` | 2026-11-02 09:31 -0600 | 17,284 | 2,462,943 | +0 | 0 |
| `06b28c2f85` | 2026-11-02 09:33 -0600 | 17,285 | 2,462,984 | +41 | 1,025 |
| `0cebd1fa60` | 2026-11-02 09:36 -0600 | 17,286 | 2,463,000 | +16 | 329 |
| `ac10c07964` | 2026-11-02 09:39 -0600 | 17,287 | 2,463,005 | +5 | 94 |
| `6dc939a9c5` | 2026-11-02 09:41 -0600 | 17,288 | 2,463,008 | +3 | 115 |
| `400ae17470` | 2026-11-02 09:43 -0600 | 17,290 | 2,463,054 | +46 | 1,403 |
| `e95b09761d` | 2026-11-02 09:46 -0600 | 17,299 | 2,465,817 | +2,763 | 59,562 |
| `1ec47b258d` | 2026-11-02 09:48 -0600 | 17,299 | 2,465,817 | +0 | 0 |
| `a675584457` | 2026-11-02 09:50 -0600 | 17,300 | 2,465,833 | +16 | 524 |
| `00ce5905f3` | 2026-11-02 09:52 -0600 | 17,300 | 2,465,833 | +0 | 0 |
| `f59ce902ab` | 2026-11-02 09:54 -0600 | 17,301 | 2,465,859 | +26 | 678 |
| `f339b1dfb5` | 2026-11-02 09:57 -0600 | 17,302 | 2,467,535 | +1,676 | 34,478 |
| `f8fff202bc` | 2026-11-02 10:04 -0600 | 17,303 | 2,467,594 | +59 | 484 |
| `bcfc45c2d4` | 2026-11-02 10:09 -0600 | 17,305 | 2,468,469 | +875 | 10,642 |
| `7f6a4215a0` | 2026-11-02 10:22 -0600 | 17,306 | 2,468,573 | +104 | 492 |
| `130f906eb9` | 2026-11-02 10:24 -0600 | 17,307 | 2,468,605 | +32 | 922 |
| `da83bd6b32` | 2026-11-02 10:27 -0600 | 17,307 | 2,468,605 | +0 | 0 |
| `2fcba8fcd2` | 2026-11-02 10:29 -0600 | 17,307 | 2,468,605 | +0 | 0 |
| `7ae46e32c9` | 2026-11-02 10:32 -0600 | 17,307 | 2,468,605 | +0 | 0 |
| `02e43c9054` | 2026-11-02 10:35 -0600 | 17,307 | 2,468,605 | +0 | 0 |
| `341f5b9236` | 2026-11-02 10:37 -0600 | 17,307 | 2,468,605 | +0 | 0 |
| `6023df56aa` | 2026-11-02 10:39 -0600 | 17,307 | 2,468,605 | +0 | 0 |
| `796428ff6b` | 2026-11-02 10:41 -0600 | 17,308 | 2,469,360 | +755 | 19,141 |
| `6b440721dd` | 2026-11-02 10:43 -0600 | 17,309 | 2,469,434 | +74 | 2,960 |
| `b3d31b9e9b` | 2026-11-02 10:44 -0600 | 17,310 | 2,469,965 | +531 | 21,479 |
| `993d523513` | 2026-11-02 10:46 -0600 | 17,312 | 2,470,198 | +233 | 8,738 |
| `0e7a6d59bb` | 2026-11-02 10:47 -0600 | 17,313 | 2,470,270 | +72 | 2,757 |
| `c1a92be38b` | 2026-11-02 10:49 -0600 | 17,315 | 2,470,801 | +531 | 15,798 |
| `d7b18f1288` | 2026-11-02 10:51 -0600 | 17,316 | 2,470,857 | +56 | 1,884 |
| `27723fc3a8` | 2026-11-02 10:53 -0600 | 17,317 | 2,471,046 | +189 | 5,194 |
| `e4f71414fb` | 2026-11-02 10:55 -0600 | 17,318 | 2,471,052 | +6 | 206 |
| `402c6e6a72` | 2026-11-02 10:57 -0600 | 17,320 | 2,471,364 | +312 | 10,119 |
| `2dc3f526ce` | 2026-11-02 10:59 -0600 | 17,324 | 2,472,242 | +878 | 28,476 |
| `3480d176f1` | 2026-11-02 11:01 -0600 | 17,325 | 2,472,268 | +26 | 900 |
| `be6efe1ac3` | 2026-11-02 11:03 -0600 | 17,326 | 2,472,948 | +680 | 17,612 |
| `c99e5724fa` | 2026-11-02 11:05 -0600 | 17,327 | 2,472,953 | +5 | 149 |
| `9c73b62b45` | 2026-11-02 11:07 -0600 | 17,329 | 2,473,479 | +526 | 17,059 |
| `5eadbca9af` | 2026-11-02 11:08 -0600 | 17,330 | 2,474,000 | +521 | 19,953 |
| `f756bd799e` | 2026-11-02 11:11 -0600 | 17,332 | 2,474,038 | +38 | 999 |
| `1005097f26` | 2026-11-02 11:13 -0600 | 17,341 | 2,475,342 | +1,304 | 36,111 |
| `9022a6eeac` | 2026-11-02 11:15 -0600 | 17,342 | 2,476,335 | +993 | 29,790 |
| `b15118a0be` | 2026-11-02 11:17 -0600 | 17,343 | 2,476,956 | +621 | 15,105 |
| `7a11db6291` | 2026-11-02 11:22 -0600 | 17,345 | 2,477,180 | +224 | 3,114 |
| `9bbfd7f18b` | 2026-11-02 11:23 -0600 | 17,347 | 2,477,197 | +17 | 523 |
| `dae71b91f8` | 2026-11-02 11:26 -0600 | 17,348 | 2,477,705 | +508 | 14,748 |
| `30a6ca0655` | 2026-11-02 11:28 -0600 | 17,349 | 2,477,804 | +99 | 2,995 |
| `e285cafedd` | 2026-11-02 11:29 -0600 | 17,350 | 2,477,901 | +97 | 3,715 |
| `5f17f789e5` | 2026-11-02 11:32 -0600 | 17,350 | 2,477,901 | +0 | 0 |
| `e7fa878108` | 2026-11-02 11:33 -0600 | 17,351 | 2,478,086 | +185 | 5,842 |
| `3ecad7a3ed` | 2026-11-02 12:04 -0600 | 17,353 | 2,478,204 | +118 | 235 |
| `e2283a1fa1` | 2026-11-02 12:06 -0600 | 17,357 | 2,483,184 | +4,980 | 134,797 |
| `18c9a881f9` | 2026-11-02 12:08 -0600 | 17,358 | 2,485,631 | +2,447 | 84,704 |
| `60af656e74` | 2026-11-02 12:10 -0600 | 17,367 | 2,487,192 | +1,561 | 44,600 |
| `f32971dd7d` | 2026-11-02 12:15 -0600 | 17,367 | 2,487,192 | +0 | 0 |
| `4d575e846a` | 2026-11-02 12:17 -0600 | 17,370 | 2,487,262 | +70 | 2,312 |
| `4fc56e2603` | 2026-11-02 12:20 -0600 | 17,371 | 2,487,299 | +37 | 709 |
| `427ac7d49d` | 2026-11-02 12:30 -0600 | 17,372 | 2,488,555 | +1,256 | 7,850 |
| `0fe178e2ca` | 2026-11-02 12:32 -0600 | 17,373 | 2,489,073 | +518 | 16,076 |
| `a39bcd663f` | 2026-11-02 12:42 -0600 | 17,374 | 2,491,281 | +2,208 | 12,738 |
| `7b933f7477` | 2026-11-02 12:48 -0600 | 17,375 | 2,491,669 | +388 | 4,220 |
| `edd4a6c61f` | 2026-11-02 12:49 -0600 | 17,376 | 2,491,809 | +140 | 4,893 |
| `be2709b443` | 2026-11-02 12:57 -0600 | 17,377 | 2,491,835 | +26 | 217 |
| `99b3f75525` | 2026-11-02 13:04 -0600 | 17,378 | 2,492,371 | +536 | 4,405 |
| `975333fe5f` | 2026-11-02 13:12 -0600 | 17,379 | 2,492,825 | +454 | 3,289 |
| `f43a963d11` | 2026-11-02 13:19 -0600 | 17,380 | 2,493,039 | +214 | 1,921 |
| `7cddc26db1` | 2026-11-02 13:29 -0600 | 17,380 | 2,493,039 | +0 | 0 |
| `fe37b2b266` | 2026-11-02 13:31 -0600 | 17,381 | 2,494,051 | +1,012 | 25,656 |
| `b683355b62` | 2026-11-02 13:34 -0600 | 17,382 | 2,494,315 | +264 | 7,146 |
| `18c5c90a81` | 2026-11-02 13:44 -0600 | 17,383 | 2,494,347 | +32 | 178 |
| `fe43879ef3` | 2026-11-02 13:54 -0600 | 17,384 | 2,495,483 | +1,136 | 7,422 |
| `ec84100bb6` | 2026-11-02 14:09 -0600 | 17,386 | 2,496,000 | +517 | 2,045 |
| `577ddbd222` | 2026-11-02 14:19 -0600 | 17,387 | 2,496,303 | +303 | 1,809 |
| `ad66e7d56d` | 2026-11-02 14:33 -0600 | 17,388 | 2,496,364 | +61 | 254 |
| `6afe4f72f6` | 2026-11-02 14:40 -0600 | 17,389 | 2,496,823 | +459 | 4,110 |
| `9811b2fa0c` | 2026-11-02 14:48 -0600 | 17,390 | 2,497,838 | +1,015 | 7,676 |
| `67928cc45c` | 2026-11-02 14:50 -0600 | 17,390 | 2,497,838 | +0 | 0 |
| `aa7dd138e0` | 2026-11-02 14:52 -0600 | 17,391 | 2,498,541 | +703 | 24,103 |
| `d93f47fce5` | 2026-11-02 15:03 -0600 | 17,392 | 2,499,148 | +607 | 3,271 |
| `44064797a6` | 2026-11-02 15:06 -0600 | 17,395 | 2,500,161 | +1,013 | 17,202 |
| `c0a5d7bede` | 2026-11-02 15:09 -0600 | 17,396 | 2,502,498 | +2,337 | 58,834 |
| `ac8799157d` | 2026-11-02 15:11 -0600 | 17,397 | 2,503,196 | +698 | 19,182 |
| `8760a238f1` | 2026-11-02 15:16 -0600 | 17,399 | 2,504,687 | +1,491 | 16,670 |
| `65d1a18cb6` | 2026-11-02 15:19 -0600 | 17,400 | 2,504,765 | +78 | 2,065 |
| `eabf729f46` | 2026-11-02 15:23 -0600 | 17,401 | 2,504,783 | +18 | 241 |
| `1dce53880d` | 2026-11-02 15:30 -0600 | 17,404 | 2,505,707 | +924 | 8,316 |
| `b3b834c991` | 2026-11-02 15:32 -0600 | 17,405 | 2,506,907 | +1,200 | 31,765 |
| `ba3aa5e4dc` | 2026-11-02 15:35 -0600 | 17,408 | 2,508,122 | +1,215 | 24,712 |
| `ee6edf6221` | 2026-11-02 15:38 -0600 | 17,409 | 2,508,762 | +640 | 12,590 |
| `1e71408f74` | 2026-11-02 15:40 -0600 | 17,411 | 2,508,804 | +42 | 1,112 |
| `c0e7b4438c` | 2026-11-02 15:55 -0600 | 17,412 | 2,509,198 | +394 | 1,571 |
| `a0454392ce` | 2026-11-02 15:58 -0600 | 17,413 | 2,509,724 | +526 | 13,430 |
| `dcd812448a` | 2026-11-02 15:59 -0600 | 17,413 | 2,509,724 | +0 | 0 |
| `da8db31f89` | 2026-11-02 16:09 -0600 | 17,415 | 2,509,782 | +58 | 379 |
| `c9d95c1a56` | 2026-11-02 16:19 -0600 | 17,415 | 2,509,782 | +0 | 0 |
| `a161c77795` | 2026-11-02 16:21 -0600 | 17,417 | 2,510,426 | +644 | 22,729 |
| `a5aede80eb` | 2026-11-02 16:36 -0600 | 17,418 | 2,510,926 | +500 | 2,000 |
| `f7712f17c9` | 2026-11-02 16:40 -0600 | 17,419 | 2,512,753 | +1,827 | 29,103 |
| `b09ca77185` | 2026-11-02 16:42 -0600 | 17,420 | 2,512,873 | +120 | 3,789 |
| `6169afbedf` | 2026-11-02 16:47 -0600 | 17,421 | 2,514,769 | +1,896 | 20,873 |
| `f7a4a030d9` | 2026-11-02 17:35 -0600 | 17,422 | 2,517,223 | +2,454 | 3,103 |
| `ef3ab38edb` | 2026-11-02 17:43 -0600 | 17,427 | 2,519,297 | +2,074 | 14,903 |
| `6a2dccff51` | 2026-11-02 17:52 -0600 | 17,428 | 2,519,388 | +91 | 595 |
| `6dc80663b8` | 2026-11-02 18:00 -0600 | 17,428 | 2,519,388 | +0 | 0 |
| `0439cc1c55` | 2026-11-02 18:02 -0600 | 17,429 | 2,519,694 | +306 | 8,813 |
| `a1d9601ace` | 2026-11-02 18:04 -0600 | 17,430 | 2,520,332 | +638 | 19,972 |
| `1ddd30bbe7` | 2026-11-02 18:06 -0600 | 17,431 | 2,521,306 | +974 | 32,770 |
| `c4c9643c66` | 2026-11-02 18:10 -0600 | 17,432 | 2,523,851 | +2,545 | 34,444 |
| `a66f69407d` | 2026-11-02 18:13 -0600 | 17,432 | 2,523,851 | +0 | 0 |
| `9c8de179a1` | 2026-11-02 18:23 -0600 | 17,433 | 2,524,089 | +238 | 1,354 |
| `cf596c91e0` | 2026-11-02 18:29 -0600 | 17,435 | 2,524,771 | +682 | 6,782 |
| `80bc8d3333` | 2026-11-02 18:35 -0600 | 17,437 | 2,525,628 | +857 | 8,691 |
| `9f4cf64cd4` | 2026-11-02 18:47 -0600 | 17,438 | 2,527,703 | +2,075 | 10,795 |
| `f00e66c408` | 2026-11-02 18:55 -0600 | 17,439 | 2,529,039 | +1,336 | 9,486 |
| `3cbbd01928` | 2026-11-02 19:01 -0600 | 17,440 | 2,540,262 | +11,223 | 122,433 |
| `bb8093368a` | 2026-11-02 19:32 -0600 | 17,441 | 2,541,759 | +1,497 | 2,874 |
| `0c4bab601c` | 2026-11-02 19:59 -0600 | 17,442 | 2,543,817 | +2,058 | 4,607 |
| `3ec8331b7f` | 2026-11-02 20:18 -0600 | 17,443 | 2,544,781 | +964 | 2,979 |
| `57d3f53b88` | 2026-11-02 20:24 -0600 | 17,444 | 2,546,737 | +1,956 | 20,650 |
| `29e2a57f3d` | 2026-11-02 21:08 -0600 | 17,444 | 2,546,737 | +0 | 0 |
| `0eb8176066` | 2026-11-02 21:10 -0600 | 17,445 | 2,551,652 | +4,915 | 163,833 |
| `1f0be6fc63` | 2026-11-02 21:20 -0600 | 17,446 | 2,553,444 | +1,792 | 10,934 |
| `60dae9b0a6` | 2026-11-02 21:36 -0600 | 17,447 | 2,560,242 | +6,798 | 25,387 |
| `fed11ffed3` | 2026-11-02 21:38 -0600 | 17,448 | 2,562,496 | +2,254 | 69,354 |
| `4fd05f8524` | 2026-11-02 21:45 -0600 | 17,448 | 2,562,496 | +0 | 0 |
| `25b851c4f4` | 2026-11-02 22:00 -0600 | 17,449 | 2,564,354 | +1,858 | 7,541 |
| `7c93319250` | 2026-11-02 22:20 -0600 | 17,450 | 2,565,360 | +1,006 | 2,907 |
| `bca04fa374` | 2026-11-02 22:42 -0600 | 17,451 | 2,568,624 | +3,264 | 9,245 |
| `08f22eeec2` | 2026-11-02 22:54 -0600 | 17,453 | 2,572,965 | +4,341 | 20,428 |
| `654bac9e98` | 2026-11-02 22:56 -0600 | 17,454 | 2,573,358 | +393 | 14,737 |
| `356294726e` | 2026-11-02 23:10 -0600 | 17,455 | 2,574,174 | +816 | 3,384 |
| `f1629b40ec` | 2026-11-02 23:16 -0600 | 17,455 | 2,574,174 | +0 | 0 |
| `6709e26c3e` | 2026-11-02 23:18 -0600 | 17,456 | 2,574,578 | +404 | 15,309 |
| `ee3ce06436` | 2026-11-02 23:20 -0600 | 17,457 | 2,577,047 | +2,469 | 84,651 |
| `eb38a12480` | 2026-11-02 23:22 -0600 | 17,458 | 2,579,801 | +2,754 | 69,820 |
| `6abefd4f63` | 2026-11-02 23:29 -0600 | 17,458 | 2,579,801 | +0 | 0 |
| `0001456606` | 2026-11-02 23:32 -0600 | 17,458 | 2,579,801 | +0 | 0 |
| `92d228ac4c` | 2026-11-02 23:49 -0600 | 17,459 | 2,582,536 | +2,735 | 9,350 |
| `f347a7ac78` | 2026-11-02 23:56 -0600 | 17,460 | 2,583,922 | +1,386 | 11,768 |
| `4d8903a6c6` | 2026-11-02 23:59 -0600 | 17,461 | 2,585,598 | +1,676 | 41,611 |
| `a20b82d7ed` | 2026-11-03 00:12 -0600 | 17,462 | 2,587,309 | +1,711 | 7,633 |
| `ee34a16539` | 2026-11-03 00:14 -0600 | 17,462 | 2,587,309 | +0 | 0 |
| `27c1db8344` | 2026-11-03 00:16 -0600 | 17,463 | 2,587,970 | +661 | 24,788 |
| `3e7f221bb7` | 2026-11-03 01:52 -0600 | 17,464 | 2,589,619 | +1,649 | 1,029 |
| `08a71c167a` | 2026-11-03 02:16 -0600 | 17,465 | 2,589,644 | +25 | 62 |
| `bd18a5b68d` | 2026-11-03 02:38 -0600 | 17,466 | 2,590,006 | +362 | 1,013 |
| `674c9e7b1e` | 2026-11-03 02:49 -0600 | 17,467 | 2,590,469 | +463 | 2,364 |
| `4087650c0a` | 2026-11-03 05:24 -0600 | 17,468 | 2,592,654 | +2,185 | 849 |
| `999e293da0` | 2026-11-03 05:26 -0600 | 17,469 | 2,592,816 | +162 | 4,942 |
| `0bad42613d` | 2026-11-03 05:38 -0600 | 17,470 | 2,592,838 | +22 | 108 |
| `b74b191a0b` | 2026-11-03 05:49 -0600 | 17,471 | 2,596,364 | +3,526 | 18,722 |
| `2e4bf5ae5b` | 2026-11-03 06:28 -0600 | 17,472 | 2,596,959 | +595 | 911 |
| `282eaf9f20` | 2026-11-03 06:30 -0600 | 17,473 | 2,597,271 | +312 | 9,360 |
| `224772b2be` | 2026-11-03 07:03 -0600 | 17,473 | 2,597,271 | +0 | 0 |
| `28781032ff` | 2026-11-03 07:39 -0600 | 17,474 | 2,597,709 | +438 | 747 |
| `d83a78bf77` | 2026-11-03 07:51 -0600 | 17,475 | 2,597,877 | +168 | 798 |
| `341f3fa72d` | 2026-11-03 08:56 -0600 | 17,476 | 2,605,776 | +7,899 | 7,301 |
| `ae3d31ddae` | 2026-11-03 08:58 -0600 | 17,477 | 2,605,805 | +29 | 768 |
| `c33c0cae8e` | 2026-11-03 09:02 -0600 | 17,477 | 2,605,805 | +0 | 0 |
| `5da18baa37` | 2026-11-03 09:04 -0600 | 17,478 | 2,606,207 | +402 | 12,695 |
| `2a828c39a1` | 2026-11-03 09:11 -0600 | 17,478 | 2,606,207 | +0 | 0 |
| `882d3be628` | 2026-11-03 09:13 -0600 | 17,479 | 2,607,003 | +796 | 23,110 |
| `5c76eb7fc7` | 2026-11-03 09:16 -0600 | 17,481 | 2,607,906 | +903 | 17,384 |
| `116fcee33d` | 2026-11-03 09:49 -0600 | 17,482 | 2,610,264 | +2,358 | 4,274 |
| `8365399851` | 2026-11-03 09:59 -0600 | 17,483 | 2,610,521 | +257 | 1,563 |
| `1283f87e34` | 2026-11-03 10:08 -0600 | 17,483 | 2,610,521 | +0 | 0 |
| `c851b2ba3d` | 2026-11-03 10:45 -0600 | 17,484 | 2,610,982 | +461 | 753 |
| `9c4945b186` | 2026-11-03 10:56 -0600 | 17,484 | 2,610,982 | +0 | 0 |
| `a4fea42dc3` | 2026-11-03 11:07 -0600 | 17,484 | 2,610,982 | +0 | 0 |
| `cfdb2818f5` | 2026-11-03 11:30 -0600 | 17,485 | 2,615,157 | +4,175 | 10,883 |
| `253e380d15` | 2026-11-03 11:43 -0600 | 17,485 | 2,615,157 | +0 | 0 |
| `57a92ec167` | 2026-11-03 11:45 -0600 | 17,486 | 2,616,583 | +1,426 | 41,069 |
| `f272d0651a` | 2026-11-03 11:51 -0600 | 17,490 | 2,617,458 | +875 | 8,161 |
| `04b88e54e1` | 2026-11-03 11:53 -0600 | 17,491 | 2,618,539 | +1,081 | 31,898 |
| `9dedffc066` | 2026-11-03 12:12 -0600 | 17,492 | 2,621,378 | +2,839 | 8,880 |
| `1590726b57` | 2026-11-03 12:24 -0600 | 17,494 | 2,621,692 | +314 | 1,566 |
| `c8050da343` | 2026-11-03 12:38 -0600 | 17,499 | 2,621,916 | +224 | 989 |
| `a5f5df5e2c` | 2026-11-03 12:43 -0600 | 17,500 | 2,622,156 | +240 | 2,796 |
| `12799928b8` | 2026-11-03 12:56 -0600 | 17,500 | 2,622,156 | +0 | 0 |
| `a1c350ae72` | 2026-11-03 12:58 -0600 | 17,500 | 2,622,156 | +0 | 0 |
| `34b0e3b49b` | 2026-11-03 13:01 -0600 | 17,502 | 2,622,380 | +224 | 6,018 |
| `18741ba192` | 2026-11-03 13:24 -0600 | 17,503 | 2,623,028 | +648 | 1,626 |
| `93083e5611` | 2026-11-03 13:27 -0600 | 17,504 | 2,623,142 | +114 | 3,206 |
| `d4358cd927` | 2026-11-03 13:29 -0600 | 17,504 | 2,623,142 | +0 | 0 |
| `b1e09c9d31` | 2026-11-03 13:31 -0600 | 17,505 | 2,623,631 | +489 | 15,718 |
| `ae91120957` | 2026-11-03 13:49 -0600 | 17,507 | 2,624,072 | +441 | 1,450 |
| `5395837997` | 2026-11-03 13:51 -0600 | 17,508 | 2,625,555 | +1,483 | 46,424 |
| `e3675bec67` | 2026-11-03 14:02 -0600 | 17,508 | 2,625,555 | +0 | 0 |
| `588c0f16a5` | 2026-11-03 14:04 -0600 | 17,510 | 2,627,438 | +1,883 | 51,355 |
| `7853e61d79` | 2026-11-03 14:28 -0600 | 17,511 | 2,628,733 | +1,295 | 3,335 |
| `29f2ac8a61` | 2026-11-03 14:29 -0600 | 17,512 | 2,631,055 | +2,322 | 72,062 |
| `aaeb9105cd` | 2026-11-03 14:31 -0600 | 17,513 | 2,633,747 | +2,692 | 82,129 |
| `6725ce259a` | 2026-11-03 14:59 -0600 | 17,513 | 2,633,747 | +0 | 0 |
| `fceebc8cf1` | 2026-11-03 15:01 -0600 | 17,514 | 2,635,146 | +1,399 | 39,042 |
| `8eb27200a1` | 2026-11-03 15:03 -0600 | 17,515 | 2,637,474 | +2,328 | 74,166 |
| `7e47d8ba3c` | 2026-11-03 15:05 -0600 | 17,518 | 2,638,422 | +948 | 29,421 |
| `e08e495c43` | 2026-11-03 15:17 -0600 | 17,519 | 2,640,672 | +2,250 | 10,561 |
| `5e4001e243` | 2026-11-03 15:31 -0600 | 17,520 | 2,641,264 | +592 | 2,596 |
| `18c47bd2d3` | 2026-11-03 15:35 -0600 | 17,521 | 2,644,704 | +3,440 | 47,448 |
| `f1b2f24f32` | 2026-11-03 15:37 -0600 | 17,522 | 2,645,792 | +1,088 | 32,640 |
| `d1ec6e7792` | 2026-11-03 15:48 -0600 | 17,523 | 2,646,465 | +673 | 3,895 |
| `00271f5d51` | 2026-11-03 16:07 -0600 | 17,525 | 2,647,183 | +718 | 2,226 |
| `305010c3de` | 2026-11-03 17:04 -0600 | 17,526 | 2,648,940 | +1,757 | 1,851 |
| `2cc9366f5b` | 2026-11-03 17:06 -0600 | 17,527 | 2,650,006 | +1,066 | 28,854 |
| `7bfa0ec43c` | 2026-11-03 17:26 -0600 | 17,528 | 2,650,958 | +952 | 2,939 |
| `380238dcf8` | 2026-11-03 17:36 -0600 | 17,529 | 2,651,075 | +117 | 652 |
| `25b5d5aaa5` | 2026-11-03 17:39 -0600 | 17,529 | 2,651,075 | +0 | 0 |
| `abe8e25c8a` | 2026-11-03 17:55 -0600 | 17,531 | 2,654,163 | +3,088 | 11,437 |
| `eb8d6529d9` | 2026-11-03 18:20 -0600 | 17,532 | 2,654,678 | +515 | 1,205 |
| `d2e99f6cc7` | 2026-11-03 18:33 -0600 | 17,533 | 2,655,541 | +863 | 4,297 |
| `8ff54558ee` | 2026-11-03 18:38 -0600 | 17,535 | 2,658,793 | +3,252 | 34,637 |
| `0df11ae78a` | 2026-11-03 18:52 -0600 | 17,535 | 2,658,793 | +0 | 0 |
| `cca399bc8f` | 2026-11-03 18:56 -0600 | 17,536 | 2,658,929 | +136 | 1,706 |
| `3df4668d70` | 2026-11-03 19:07 -0600 | 17,537 | 2,662,657 | +3,728 | 20,365 |
| `156b75805f` | 2026-11-03 19:10 -0600 | 17,538 | 2,662,705 | +48 | 1,122 |
| `7dbe26924b` | 2026-11-03 19:25 -0600 | 17,539 | 2,664,934 | +2,229 | 8,799 |
| `8475881f55` | 2026-11-03 19:33 -0600 | 17,540 | 2,665,554 | +620 | 4,873 |
| `f2665830d2` | 2026-11-03 19:36 -0600 | 17,540 | 2,665,554 | +0 | 0 |
| `21ced14fd7` | 2026-11-03 19:42 -0600 | 17,541 | 2,666,775 | +1,221 | 11,945 |
| `da766738b8` | 2026-11-03 19:56 -0600 | 17,541 | 2,666,775 | +0 | 0 |
| `6eceaad02e` | 2026-11-03 20:03 -0600 | 17,542 | 2,668,118 | +1,343 | 11,218 |
| `b3a27d9063` | 2026-11-03 20:13 -0600 | 17,543 | 2,671,426 | +3,308 | 21,040 |
| `7932683a53` | 2026-11-03 20:15 -0600 | 17,545 | 2,673,572 | +2,146 | 56,806 |
| `f7803f7da2` | 2026-11-03 20:17 -0600 | 17,546 | 2,674,313 | +741 | 20,363 |
| `0bd321b0ec` | 2026-11-03 20:19 -0600 | 17,547 | 2,675,211 | +898 | 26,283 |
| `ed24527643` | 2026-11-03 20:23 -0600 | 17,547 | 2,675,211 | +0 | 0 |
| `aa2300d957` | 2026-11-03 20:42 -0600 | 17,548 | 2,675,618 | +407 | 1,291 |
| `e575ce31b6` | 2026-11-03 20:44 -0600 | 17,549 | 2,677,522 | +1,904 | 64,664 |
| `c84e605fc3` | 2026-11-03 20:56 -0600 | 17,550 | 2,678,547 | +1,025 | 5,034 |
| `c999f78863` | 2026-11-03 21:06 -0600 | 17,551 | 2,679,521 | +974 | 5,592 |
| `5d81c6cd15` | 2026-11-03 21:27 -0600 | 17,552 | 2,680,771 | +1,250 | 3,597 |
| `fd2467d388` | 2026-11-03 21:35 -0600 | 17,553 | 2,681,937 | +1,166 | 9,066 |
| `29f648fb9a` | 2026-11-03 22:02 -0600 | 17,553 | 2,681,937 | +0 | 0 |
| `13fa2a104d` | 2026-11-03 22:12 -0600 | 17,554 | 2,683,010 | +1,073 | 6,180 |
| `47afcc8afd` | 2026-11-03 22:17 -0600 | 17,555 | 2,684,702 | +1,692 | 22,070 |
| `b7568ef001` | 2026-11-03 22:36 -0600 | 17,557 | 2,687,347 | +2,645 | 8,187 |
| `df268c1f76` | 2026-11-03 22:45 -0600 | 17,558 | 2,687,873 | +526 | 3,600 |
| `23c7d29cbd` | 2026-11-03 22:56 -0600 | 17,559 | 2,689,194 | +1,321 | 7,294 |
| `34a5fe6ee5` | 2026-11-03 23:16 -0600 | 17,560 | 2,689,987 | +793 | 2,354 |
| `b8064409fb` | 2026-11-03 23:43 -0600 | 17,561 | 2,691,585 | +1,598 | 3,596 |
| `4f07c22632` | 2026-11-03 23:45 -0600 | 17,562 | 2,693,285 | +1,700 | 48,960 |
| `5d828cb0a1` | 2026-11-03 23:51 -0600 | 17,564 | 2,694,510 | +1,225 | 13,086 |
| `9c4ad689fc` | 2026-11-04 00:02 -0600 | 17,565 | 2,696,174 | +1,664 | 8,981 |
| `b90a6f19d5` | 2026-11-04 00:04 -0600 | 17,566 | 2,698,420 | +2,246 | 69,108 |
| `b2daf02f29` | 2026-11-04 00:12 -0600 | 17,567 | 2,699,560 | +1,140 | 8,274 |
| `2a840588ce` | 2026-11-04 00:23 -0600 | 17,568 | 2,700,860 | +1,300 | 7,405 |
| `40541c02a9` | 2026-11-04 00:29 -0600 | 17,569 | 2,703,380 | +2,520 | 24,519 |
| `d28282f6d9` | 2026-11-04 00:36 -0600 | 17,570 | 2,705,331 | +1,951 | 15,927 |
| `a669d9f9a2` | 2026-11-04 00:44 -0600 | 17,571 | 2,706,515 | +1,184 | 8,936 |
| `73aaf38e64` | 2026-11-04 00:46 -0600 | 17,571 | 2,706,515 | +0 | 0 |
| `8b6d0e16bd` | 2026-11-04 00:56 -0600 | 17,572 | 2,708,604 | +2,089 | 13,429 |
| `834eeee68b` | 2026-11-04 01:02 -0600 | 17,573 | 2,711,072 | +2,468 | 24,078 |
| `e64f2cb890` | 2026-11-04 01:04 -0600 | 17,574 | 2,711,668 | +596 | 20,831 |
| `c7acefd0c5` | 2026-11-04 01:23 -0600 | 17,576 | 2,715,008 | +3,340 | 10,520 |
| `f01a80fce5` | 2026-11-04 01:25 -0600 | 17,576 | 2,715,008 | +0 | 0 |
| `d5e6272e41` | 2026-11-04 01:41 -0600 | 17,577 | 2,717,616 | +2,608 | 10,031 |
| `e83e84d8ff` | 2026-11-04 01:43 -0600 | 17,578 | 2,721,711 | +4,095 | 118,887 |
| `d12e696011` | 2026-11-04 01:50 -0600 | 17,579 | 2,722,581 | +870 | 7,547 |
| `2bc4237703` | 2026-11-04 02:06 -0600 | 17,580 | 2,723,505 | +924 | 3,367 |
| `54d863ec1a` | 2026-11-04 02:15 -0600 | 17,581 | 2,725,987 | +2,482 | 16,425 |
| `0abb60e8ce` | 2026-11-04 02:21 -0600 | 17,582 | 2,727,966 | +1,979 | 20,355 |
| `d8ba1a0a1d` | 2026-11-04 02:29 -0600 | 17,583 | 2,728,449 | +483 | 3,600 |
| `fc272ab02d` | 2026-11-04 02:57 -0600 | 17,584 | 2,732,835 | +4,386 | 9,315 |
| `e45e7d6822` | 2026-11-04 02:59 -0600 | 17,585 | 2,733,214 | +379 | 11,276 |
| `bafc23152b` | 2026-11-04 03:17 -0600 | 17,586 | 2,735,945 | +2,731 | 9,426 |
| `7b41c9b8d4` | 2026-11-04 03:27 -0600 | 17,587 | 2,736,190 | +245 | 1,425 |
| `86c44ba84f` | 2026-11-04 04:15 -0600 | 17,587 | 2,736,190 | +0 | 0 |
| `cfbc80a871` | 2026-11-04 04:17 -0600 | 17,588 | 2,742,662 | +6,472 | 166,423 |
| `638e30f291` | 2026-11-04 04:23 -0600 | 17,589 | 2,745,067 | +2,405 | 25,023 |
| `2473f9abb4` | 2026-11-04 04:38 -0600 | 17,589 | 2,745,067 | +0 | 0 |
| `7d898b1296` | 2026-11-04 04:44 -0600 | 17,590 | 2,745,416 | +349 | 3,621 |
| `fc92b06f0b` | 2026-11-04 05:04 -0600 | 17,591 | 2,745,610 | +194 | 584 |
| `22bd31f4d3` | 2026-11-04 05:06 -0600 | 17,592 | 2,748,239 | +2,629 | 68,089 |
| `f27b693a41` | 2026-11-04 05:09 -0600 | 17,593 | 2,751,081 | +2,842 | 70,077 |
| `a4e5506b58` | 2026-11-04 05:24 -0600 | 17,594 | 2,752,602 | +1,521 | 5,794 |
| `9fd2560b72` | 2026-11-04 05:26 -0600 | 17,595 | 2,759,364 | +6,762 | 227,507 |
| `3c61ea423a` | 2026-11-04 05:34 -0600 | 17,596 | 2,759,476 | +112 | 865 |
| `36f46f6492` | 2026-11-04 05:36 -0600 | 17,596 | 2,759,476 | +0 | 0 |
| `e9006282c7` | 2026-11-04 05:38 -0600 | 17,598 | 2,760,410 | +934 | 30,021 |
| `fe91609cb0` | 2026-11-04 06:16 -0600 | 17,599 | 2,761,606 | +1,196 | 1,907 |
| `38f6428386` | 2026-11-04 06:29 -0600 | 17,600 | 2,765,003 | +3,397 | 15,286 |
| `146eeb0827` | 2026-11-04 06:37 -0600 | 17,601 | 2,767,145 | +2,142 | 16,619 |
| `1caa137071` | 2026-11-04 06:44 -0600 | 17,602 | 2,768,157 | +1,012 | 8,394 |
| `de1031c97d` | 2026-11-04 06:58 -0600 | 17,603 | 2,768,889 | +732 | 3,171 |
| `dfbe3ee34f` | 2026-11-04 07:28 -0600 | 17,604 | 2,769,808 | +919 | 1,823 |
| `dccbeeec23` | 2026-11-04 07:38 -0600 | 17,605 | 2,771,589 | +1,781 | 10,210 |
| `86c399b399` | 2026-11-04 07:44 -0600 | 17,605 | 2,771,589 | +0 | 0 |
| `0fd5fd3356` | 2026-11-04 08:05 -0600 | 17,606 | 2,772,225 | +636 | 1,759 |
| `7b676230d1` | 2026-11-04 08:07 -0600 | 17,607 | 2,777,945 | +5,720 | 162,142 |
| `6793a698f6` | 2026-11-04 08:13 -0600 | 17,608 | 2,783,681 | +5,736 | 59,854 |
| `8ac383cd1f` | 2026-11-04 08:37 -0600 | 17,610 | 2,784,584 | +903 | 2,272 |
| `661df1cf9d` | 2026-11-04 08:46 -0600 | 17,610 | 2,784,584 | +0 | 0 |
| `4e98cf5cf4` | 2026-11-04 09:02 -0600 | 17,610 | 2,784,584 | +0 | 0 |
| `fb0fb9ff6c` | 2026-11-04 09:04 -0600 | 17,611 | 2,786,064 | +1,480 | 35,285 |
| `62fbe47f2b` | 2026-11-04 09:10 -0600 | 17,612 | 2,786,393 | +329 | 3,374 |
| `cdc29552c9` | 2026-11-04 09:32 -0600 | 17,612 | 2,786,393 | +0 | 0 |
| `d9b342c302` | 2026-11-04 09:36 -0600 | 17,613 | 2,786,527 | +134 | 2,412 |
| `8bc69c81b1` | 2026-11-04 09:48 -0600 | 17,614 | 2,794,900 | +8,373 | 41,865 |
| `691a02a5df` | 2026-11-04 09:53 -0600 | 17,615 | 2,803,273 | +8,373 | 91,899 |
| `3528f3e22d` | 2026-11-04 10:26 -0600 | 17,617 | 2,807,585 | +4,312 | 7,993 |
| `a845cf5a14` | 2026-11-04 10:33 -0600 | 17,618 | 2,808,456 | +871 | 6,846 |
| `be49138f4f` | 2026-11-04 10:39 -0600 | 17,619 | 2,809,308 | +852 | 9,323 |
| `9fd5d546b9` | 2026-11-04 11:16 -0600 | 17,620 | 2,811,544 | +2,236 | 3,642 |
| `9d639785c4` | 2026-11-04 11:18 -0600 | 17,621 | 2,814,205 | +2,661 | 70,960 |
| `3ee0cf98b1` | 2026-11-04 11:23 -0600 | 17,622 | 2,814,279 | +74 | 865 |
| `a012ec4dd2` | 2026-11-04 11:25 -0600 | 17,623 | 2,815,064 | +785 | 25,459 |
| `b0a6b17aa1` | 2026-11-04 11:36 -0600 | 17,624 | 2,817,956 | +2,892 | 15,266 |
| `fcb716260c` | 2026-11-04 11:48 -0600 | 17,625 | 2,818,280 | +324 | 1,669 |
| `2594374b88` | 2026-11-04 11:56 -0600 | 17,626 | 2,819,785 | +1,505 | 11,479 |
| `704a10af11` | 2026-11-04 12:07 -0600 | 17,627 | 2,822,478 | +2,693 | 13,909 |
| `e1001cafc6` | 2026-11-04 12:33 -0600 | 17,628 | 2,825,866 | +3,388 | 7,793 |
| `1e5819b080` | 2026-11-04 12:49 -0600 | 17,629 | 2,826,778 | +912 | 3,434 |
| `2afe6374c6` | 2026-11-04 12:51 -0600 | 17,630 | 2,827,489 | +711 | 24,147 |
| `ec92369298` | 2026-11-04 13:18 -0600 | 17,631 | 2,828,008 | +519 | 1,174 |
| `03b73c5be9` | 2026-11-04 13:37 -0600 | 17,632 | 2,829,155 | +1,147 | 3,581 |
| `1ceb1cc7fc` | 2026-11-04 13:39 -0600 | 17,633 | 2,829,547 | +392 | 11,859 |
| `31f4b46bd8` | 2026-11-04 13:41 -0600 | 17,635 | 2,830,149 | +602 | 17,911 |
| `8bd3fdcb9f` | 2026-11-04 13:43 -0600 | 17,636 | 2,830,638 | +489 | 11,582 |
| `613d6dbb20` | 2026-11-04 14:10 -0600 | 17,638 | 2,832,434 | +1,796 | 4,087 |
| `3b6997f213` | 2026-11-04 14:48 -0600 | 17,639 | 2,832,774 | +340 | 535 |
| `351c75d859` | 2026-11-04 14:50 -0600 | 17,640 | 2,833,875 | +1,101 | 38,112 |
| `fbcf35c348` | 2026-11-04 15:16 -0600 | 17,641 | 2,836,827 | +2,952 | 6,799 |
| `ce223b53c7` | 2026-11-04 15:18 -0600 | 17,642 | 2,837,854 | +1,027 | 30,059 |
| `d270b5ec7c` | 2026-11-04 15:25 -0600 | 17,643 | 2,838,500 | +646 | 5,472 |
| `13c18bd8ed` | 2026-11-04 15:41 -0600 | 17,644 | 2,839,253 | +753 | 2,792 |
| `55bbfe9717` | 2026-11-04 15:47 -0600 | 17,644 | 2,839,253 | +0 | 0 |
| `e236ccbbcc` | 2026-11-04 16:17 -0600 | 17,645 | 2,842,517 | +3,264 | 6,485 |
| `5d564d84b5` | 2026-11-04 16:26 -0600 | 17,646 | 2,844,076 | +1,559 | 10,670 |
| `71c082a8ed` | 2026-11-04 16:48 -0600 | 17,646 | 2,844,076 | +0 | 0 |
| `e5841bccd7` | 2026-11-04 16:52 -0600 | 17,646 | 2,844,076 | +0 | 0 |
| `6e3b067962` | 2026-11-04 16:55 -0600 | 17,647 | 2,844,656 | +580 | 9,198 |
| `4d84f867c3` | 2026-11-04 17:00 -0600 | 17,648 | 2,844,757 | +101 | 1,415 |
| `def9db4439` | 2026-11-04 17:02 -0600 | 17,649 | 2,845,190 | +433 | 11,547 |
| `59091f3c88` | 2026-11-04 17:04 -0600 | 17,650 | 2,846,791 | +1,601 | 42,379 |
| `a25fdaa61e` | 2026-11-04 17:11 -0600 | 17,654 | 2,848,376 | +1,585 | 13,850 |
| `b3f7b51e4e` | 2026-11-04 17:25 -0600 | 17,655 | 2,849,041 | +665 | 2,920 |
| `8efa0d87d4` | 2026-11-04 17:31 -0600 | 17,657 | 2,849,617 | +576 | 5,574 |
| `74c3533ce3` | 2026-11-04 17:37 -0600 | 17,659 | 2,850,214 | +597 | 6,037 |
| `2028c9ad2e` | 2026-11-04 17:42 -0600 | 17,661 | 2,850,558 | +344 | 3,753 |
| `f06821456e` | 2026-11-04 18:17 -0600 | 17,662 | 2,851,403 | +845 | 1,488 |
| `5078eca96e` | 2026-11-04 18:32 -0600 | 17,663 | 2,855,527 | +4,124 | 16,120 |
| `f7321fcd56` | 2026-11-04 19:27 -0600 | 17,664 | 2,855,990 | +463 | 506 |
| `9d44367a86` | 2026-11-04 19:29 -0600 | 17,665 | 2,856,356 | +366 | 11,764 |
| `4d0e8798ad` | 2026-11-04 19:39 -0600 | 17,666 | 2,856,946 | +590 | 3,431 |
| `1b0f91e3d7` | 2026-11-04 20:00 -0600 | 17,668 | 2,858,190 | +1,244 | 3,532 |
| `60af4cf89f` | 2026-11-04 20:13 -0600 | 17,669 | 2,858,729 | +539 | 2,422 |
| `8c2e39be43` | 2026-11-04 20:20 -0600 | 17,669 | 2,858,729 | +0 | 0 |
| `18f32abe07` | 2026-11-04 20:27 -0600 | 17,670 | 2,861,190 | +2,461 | 19,386 |
| `489d2a61d6` | 2026-11-04 20:29 -0600 | 17,671 | 2,861,419 | +229 | 6,595 |
| `6eff88afd4` | 2026-11-04 20:35 -0600 | 17,671 | 2,861,419 | +0 | 0 |
| `a49b55acc8` | 2026-11-04 20:41 -0600 | 17,672 | 2,861,665 | +246 | 2,426 |
| `f72d7f4c36` | 2026-11-04 20:47 -0600 | 17,673 | 2,862,211 | +546 | 6,104 |
| `20330cdc43` | 2026-11-04 20:49 -0600 | 17,674 | 2,863,005 | +794 | 20,130 |
| `dcf72a1d0c` | 2026-11-04 20:51 -0600 | 17,675 | 2,864,941 | +1,936 | 48,066 |
| `50209b0c1c` | 2026-11-04 20:59 -0600 | 17,676 | 2,865,413 | +472 | 3,810 |
| `13608603f2` | 2026-11-04 21:23 -0600 | 17,677 | 2,867,881 | +2,468 | 6,231 |
| `823386590c` | 2026-11-04 21:44 -0600 | 17,678 | 2,867,985 | +104 | 300 |
| `de74afb03b` | 2026-11-04 21:46 -0600 | 17,679 | 2,869,266 | +1,281 | 36,893 |
| `595cb50ee0` | 2026-11-04 21:48 -0600 | 17,680 | 2,869,309 | +43 | 1,138 |
| `701356f3d8` | 2026-11-04 21:58 -0600 | 17,681 | 2,870,460 | +1,151 | 7,144 |
| `7e13444e9d` | 2026-11-04 22:14 -0600 | 17,682 | 2,870,865 | +405 | 1,468 |
| `21da65f4b2` | 2026-11-04 22:20 -0600 | 17,683 | 2,871,413 | +548 | 5,653 |
| `087994487f` | 2026-11-04 22:32 -0600 | 17,683 | 2,871,413 | +0 | 0 |
| `2ef2a60b33` | 2026-11-04 22:35 -0600 | 17,684 | 2,871,914 | +501 | 13,561 |
| `48f9d826d8` | 2026-11-04 22:38 -0600 | 17,684 | 2,871,914 | +0 | 0 |
| `a9d16f9357` | 2026-11-04 22:47 -0600 | 17,685 | 2,872,370 | +456 | 3,068 |
| `2a61ec4ad0` | 2026-11-04 22:56 -0600 | 17,686 | 2,873,284 | +914 | 6,071 |
| `1ed799143d` | 2026-11-04 23:02 -0600 | 17,687 | 2,878,420 | +5,136 | 50,936 |
| `f1b0b51143` | 2026-11-04 23:27 -0600 | 17,688 | 2,879,299 | +879 | 2,068 |
| `e25b5a86b6` | 2026-11-04 23:36 -0600 | 17,689 | 2,887,081 | +7,782 | 52,267 |
| `248aa1e77e` | 2026-11-04 23:39 -0600 | 17,690 | 2,887,508 | +427 | 8,540 |
| `7c367ea5e7` | 2026-11-04 23:56 -0600 | 17,691 | 2,887,933 | +425 | 1,507 |
| `5bf6469b39` | 2026-11-05 00:02 -0600 | 17,693 | 2,888,783 | +850 | 8,793 |
| `eb3c246d11` | 2026-11-05 00:14 -0600 | 17,694 | 2,889,257 | +474 | 2,387 |
| `6975bc0532` | 2026-11-05 00:27 -0600 | 17,694 | 2,889,257 | +0 | 0 |
| `f689bfb42e` | 2026-11-05 00:37 -0600 | 17,695 | 2,889,629 | +372 | 2,232 |
| `90b7e691ba` | 2026-11-05 00:49 -0600 | 17,696 | 2,897,265 | +7,636 | 37,969 |
| `651411fb90` | 2026-11-05 00:50 -0600 | 17,697 | 2,897,353 | +88 | 3,017 |
| `020816df67` | 2026-11-05 01:06 -0600 | 17,698 | 2,898,247 | +894 | 3,487 |
| `f28c652eac` | 2026-11-05 01:08 -0600 | 17,700 | 2,903,815 | +5,568 | 177,388 |
| `9a0b9e5444` | 2026-11-05 01:35 -0600 | 17,701 | 2,904,600 | +785 | 1,702 |
| `ac3bce86be` | 2026-11-05 01:41 -0600 | 17,702 | 2,908,383 | +3,783 | 39,247 |
| `4a41c7ccaa` | 2026-11-05 01:47 -0600 | 17,703 | 2,910,145 | +1,762 | 17,474 |
| `3edd3b8072` | 2026-11-05 01:53 -0600 | 17,704 | 2,910,208 | +63 | 646 |
| `73813fe68b` | 2026-11-05 02:12 -0600 | 17,705 | 2,910,843 | +635 | 2,056 |
| `0e1cfac53f` | 2026-11-05 02:13 -0600 | 17,706 | 2,913,891 | +3,048 | 94,593 |
| `74ad01b915` | 2026-11-05 02:18 -0600 | 17,707 | 2,915,543 | +1,652 | 19,628 |
| `66fea2ec09` | 2026-11-05 02:26 -0600 | 17,708 | 2,919,583 | +4,040 | 30,300 |
| `801766f7cb` | 2026-11-05 02:35 -0600 | 17,709 | 2,921,216 | +1,633 | 11,460 |
| `e683bd5112` | 2026-11-05 02:46 -0600 | 17,710 | 2,922,747 | +1,531 | 8,652 |
| `ec44a33276` | 2026-11-05 02:47 -0600 | 17,712 | 2,923,788 | +1,041 | 37,476 |
| `138247c2c3` | 2026-11-05 03:05 -0600 | 17,712 | 2,923,788 | +0 | 0 |
| `9ed25184eb` | 2026-11-05 03:08 -0600 | 17,714 | 2,923,941 | +153 | 4,371 |
| `4cb7f1ead3` | 2026-11-05 03:10 -0600 | 17,715 | 2,925,013 | +1,072 | 32,705 |
| `87caca7ee2` | 2026-11-05 03:11 -0600 | 17,716 | 2,925,206 | +193 | 6,374 |
| `d0cab48f2a` | 2026-11-05 03:13 -0600 | 17,717 | 2,925,321 | +115 | 4,059 |
| `58496662d8` | 2026-11-05 03:18 -0600 | 17,718 | 2,925,853 | +532 | 6,023 |
| `4230278d71` | 2026-11-05 03:26 -0600 | 17,719 | 2,926,326 | +473 | 3,497 |
| `82a3e56a87` | 2026-11-05 03:35 -0600 | 17,720 | 2,927,023 | +697 | 4,844 |
| `15ce972ace` | 2026-11-05 03:48 -0600 | 17,720 | 2,927,023 | +0 | 0 |
| `695dbf348b` | 2026-11-05 03:51 -0600 | 17,721 | 2,929,253 | +2,230 | 51,462 |
| `508697d652` | 2026-11-05 03:53 -0600 | 17,722 | 2,930,405 | +1,152 | 38,759 |
| `3238acfa6f` | 2026-11-05 04:03 -0600 | 17,723 | 2,930,489 | +84 | 491 |
| `b7fb34b48f` | 2026-11-05 04:18 -0600 | 17,724 | 2,930,541 | +52 | 202 |
| `50b307bbac` | 2026-11-05 04:20 -0600 | 17,725 | 2,932,617 | +2,076 | 59,789 |
| `943245a900` | 2026-11-05 04:35 -0600 | 17,726 | 2,934,630 | +2,013 | 8,016 |
| `5b87ddf1a8` | 2026-11-05 04:45 -0600 | 17,726 | 2,934,630 | +0 | 0 |
| `106c7ef741` | 2026-11-05 04:47 -0600 | 17,728 | 2,936,705 | +2,075 | 71,827 |
| `9fd35b8032` | 2026-11-05 04:49 -0600 | 17,728 | 2,936,705 | +0 | 0 |
| `aa8d9b8433` | 2026-11-05 04:51 -0600 | 17,730 | 2,938,551 | +1,846 | 51,919 |
| `800318c1d9` | 2026-11-05 05:09 -0600 | 17,730 | 2,938,551 | +0 | 0 |
| `f70307290b` | 2026-11-05 05:11 -0600 | 17,731 | 2,939,108 | +557 | 14,323 |
| `f73e2213de` | 2026-11-05 05:13 -0600 | 17,733 | 2,940,364 | +1,256 | 39,318 |
| `bb46974843` | 2026-11-05 05:15 -0600 | 17,734 | 2,940,757 | +393 | 13,222 |
| `e8729b04b7` | 2026-11-05 05:17 -0600 | 17,735 | 2,941,365 | +608 | 17,941 |
| `d1aceb82bb` | 2026-11-05 05:24 -0600 | 17,736 | 2,942,232 | +867 | 6,967 |
| `41752c26ea` | 2026-11-05 05:26 -0600 | 17,737 | 2,942,437 | +205 | 5,811 |
| `eca346caa7` | 2026-11-05 05:28 -0600 | 17,738 | 2,942,688 | +251 | 8,367 |
| `bdf812c209` | 2026-11-05 05:30 -0600 | 17,738 | 2,945,014 | +2,326 | 70,366 |
| `c77288c1a9` | 2026-11-05 05:35 -0600 | 17,740 | 2,945,526 | +512 | 5,637 |
| `b7af828910` | 2026-11-05 05:39 -0600 | 17,741 | 2,946,569 | +1,043 | 18,316 |
| `65a4851dc1` | 2026-11-05 05:58 -0600 | 17,742 | 2,947,087 | +518 | 1,622 |
| `d0733f65e5` | 2026-11-05 06:00 -0600 | 17,744 | 2,948,258 | +1,171 | 32,934 |
| `44c1028ad6` | 2026-11-05 06:25 -0600 | 17,744 | 2,948,258 | +0 | 0 |
| `2055f9a71c` | 2026-11-05 06:27 -0600 | 17,745 | 2,950,115 | +1,857 | 51,032 |
| `4679c0ffe7` | 2026-11-05 06:34 -0600 | 17,747 | 2,950,673 | +558 | 4,817 |
| `44233dcd14` | 2026-11-05 06:43 -0600 | 17,748 | 2,950,967 | +294 | 1,942 |
| `d941fa4a2f` | 2026-11-05 06:49 -0600 | 17,749 | 2,951,374 | +407 | 4,681 |
| `b2a456787f` | 2026-11-05 06:54 -0600 | 17,750 | 2,951,505 | +131 | 1,488 |
| `a039fd6e12` | 2026-11-05 07:00 -0600 | 17,751 | 2,952,703 | +1,198 | 12,537 |
| `32ba0c2d64` | 2026-11-05 07:14 -0600 | 17,751 | 2,952,703 | +0 | 0 |
| `a8860b4d78` | 2026-11-05 07:32 -0600 | 17,752 | 2,953,892 | +1,189 | 4,038 |
| `679b29b615` | 2026-11-05 07:37 -0600 | 17,753 | 2,953,983 | +91 | 964 |
| `140220a389` | 2026-11-05 07:48 -0600 | 17,756 | 2,963,743 | +9,760 | 53,725 |
| `f02ee7418d` | 2026-11-05 08:17 -0600 | 17,757 | 2,965,946 | +2,203 | 4,506 |
| `5aa816bd16` | 2026-11-05 08:31 -0600 | 17,758 | 2,966,916 | +970 | 4,322 |
| `2e816a2a12` | 2026-11-05 08:33 -0600 | 17,760 | 2,967,701 | +785 | 20,478 |
| `a4f7b5e635` | 2026-11-05 09:10 -0600 | 17,762 | 2,968,914 | +1,213 | 2,002 |
| `ec50c60de2` | 2026-11-05 09:42 -0600 | 17,764 | 2,969,380 | +466 | 853 |
| `2ed84f83b3` | 2026-11-05 09:55 -0600 | 17,765 | 2,969,747 | +367 | 1,807 |
| `65a78b8632` | 2026-11-05 09:56 -0600 | 17,766 | 2,970,555 | +808 | 25,971 |
| `53fa01898b` | 2026-11-05 10:02 -0600 | 17,766 | 2,970,555 | +0 | 0 |
| `58da2c07ac` | 2026-11-05 10:08 -0600 | 17,767 | 2,971,232 | +677 | 6,983 |
| `f3f45c41d9` | 2026-11-05 10:31 -0600 | 17,768 | 2,971,487 | +255 | 672 |
| `ac1f745e9c` | 2026-11-05 10:35 -0600 | 17,769 | 2,973,165 | +1,678 | 21,196 |
| `07be223200` | 2026-11-05 10:49 -0600 | 17,769 | 2,973,165 | +0 | 0 |
| `0669dd1d44` | 2026-11-05 10:51 -0600 | 17,770 | 2,976,100 | +2,935 | 75,471 |
| `e84be609fd` | 2026-11-05 11:04 -0600 | 17,771 | 2,976,474 | +374 | 1,689 |
| `2d6b7c5526` | 2026-11-05 11:28 -0600 | 17,772 | 2,978,676 | +2,202 | 5,594 |
| `88696866c5` | 2026-11-05 11:40 -0600 | 17,773 | 2,979,273 | +597 | 2,989 |
| `3bfca40cc0` | 2026-11-05 12:16 -0600 | 17,774 | 2,979,712 | +439 | 733 |
| `1ab70810c4` | 2026-11-05 12:35 -0600 | 17,775 | 2,979,945 | +233 | 720 |
| `7e78acb7fd` | 2026-11-05 12:47 -0600 | 17,776 | 2,982,632 | +2,687 | 13,491 |
| `a956ef8118` | 2026-11-05 12:56 -0600 | 17,777 | 2,983,239 | +607 | 4,047 |
| `6387f1fd24` | 2026-11-05 13:31 -0600 | 17,779 | 2,985,679 | +2,440 | 4,239 |
| `97c5df3079` | 2026-11-05 14:14 -0600 | 17,779 | 2,985,679 | +0 | 0 |
| `d18f2132a3` | 2026-11-05 14:16 -0600 | 17,780 | 2,988,171 | +2,492 | 66,453 |
| `ebb0bf9ec3` | 2026-11-05 14:23 -0600 | 17,781 | 2,989,497 | +1,326 | 10,703 |
| `7b2eeb5af9` | 2026-11-05 14:36 -0600 | 17,782 | 2,989,874 | +377 | 1,839 |
| `e05e2aae4d` | 2026-11-05 14:39 -0600 | 17,782 | 2,989,874 | +0 | 0 |
| `eb23d9b162` | 2026-11-05 14:48 -0600 | 17,783 | 2,991,329 | +1,455 | 9,772 |
| `36ccd2f62a` | 2026-11-05 15:12 -0600 | 17,784 | 2,991,608 | +279 | 698 |
| `644d02b431` | 2026-11-05 15:51 -0600 | 17,785 | 2,992,147 | +539 | 834 |
| `723315f95f` | 2026-11-05 15:54 -0600 | 17,786 | 2,993,444 | +1,297 | 23,702 |
| `f3497a76d4` | 2026-11-05 15:56 -0600 | 17,787 | 2,993,887 | +443 | 12,657 |
| `893b01449a` | 2026-11-05 16:10 -0600 | 17,787 | 2,993,887 | +0 | 0 |
| `7619c91610` | 2026-11-05 16:13 -0600 | 17,788 | 2,995,005 | +1,118 | 23,675 |
| `6d37e870da` | 2026-11-05 16:15 -0600 | 17,788 | 2,995,005 | +0 | 0 |
| `8f6a8dbae5` | 2026-11-05 16:21 -0600 | 17,789 | 2,998,088 | +3,083 | 29,597 |
| `9fa8a02012` | 2026-11-05 16:23 -0600 | 17,790 | 2,999,460 | +1,372 | 39,200 |
| `5fd93e4009` | 2026-11-05 16:31 -0600 | 17,791 | 3,000,045 | +585 | 4,649 |
| `963e1f8abe` | 2026-11-05 17:12 -0600 | 17,792 | 3,001,021 | +976 | 1,438 |
| `3c94cecf19` | 2026-11-05 17:27 -0600 | 17,793 | 3,001,818 | +797 | 3,129 |
| `e5a947d207` | 2026-11-05 17:36 -0600 | 17,794 | 3,002,544 | +726 | 4,950 |
| `9b0cfea890` | 2026-11-05 17:46 -0600 | 17,796 | 3,002,684 | +140 | 820 |
| `00400a94ba` | 2026-11-05 17:48 -0600 | 17,797 | 3,002,959 | +275 | 8,250 |
| `f868626fa7` | 2026-11-05 17:51 -0600 | 17,797 | 3,002,959 | +0 | 0 |
| `4b165f31c3` | 2026-11-05 18:06 -0600 | 17,801 | 3,003,798 | +839 | 3,294 |
| `e473ef8f12` | 2026-11-05 18:09 -0600 | 17,801 | 3,003,798 | +0 | 0 |
| `dd597c2a57` | 2026-11-05 18:12 -0600 | 17,801 | 3,003,798 | +0 | 0 |
| `e9da38c876` | 2026-11-05 18:21 -0600 | 17,802 | 3,004,048 | +250 | 1,596 |
| `e8136b7ece` | 2026-11-05 18:29 -0600 | 17,802 | 3,004,048 | +0 | 0 |
| `caef739901` | 2026-11-05 18:31 -0600 | 17,803 | 3,004,514 | +466 | 12,245 |
| `240f48d459` | 2026-11-05 18:33 -0600 | 17,804 | 3,005,164 | +650 | 20,893 |
| `3b92aecb4d` | 2026-11-05 18:45 -0600 | 17,805 | 3,006,345 | +1,181 | 5,753 |
| `8d9ff540f2` | 2026-11-05 18:47 -0600 | 17,806 | 3,007,121 | +776 | 24,943 |
| `3765678c85` | 2026-11-05 19:07 -0600 | 17,807 | 3,008,673 | +1,552 | 4,546 |
| `05c7623299` | 2026-11-05 19:34 -0600 | 17,807 | 3,008,673 | +0 | 0 |
| `270491bbe3` | 2026-11-05 19:49 -0600 | 17,808 | 3,009,096 | +423 | 1,648 |
| `2b6dd46f6a` | 2026-11-05 20:13 -0600 | 17,809 | 3,015,768 | +6,672 | 16,808 |
| `e8e8182eb6` | 2026-11-05 20:30 -0600 | 17,811 | 3,023,718 | +7,950 | 27,706 |
| `feaacfdb10` | 2026-11-05 20:50 -0600 | 17,811 | 3,023,718 | +0 | 0 |
| `d811de015e` | 2026-11-05 21:06 -0600 | 17,812 | 3,024,452 | +734 | 2,773 |
| `c28ff299b2` | 2026-11-05 21:17 -0600 | 17,813 | 3,025,472 | +1,020 | 5,980 |
| `29367a57df` | 2026-11-05 21:25 -0600 | 17,814 | 3,026,008 | +536 | 3,620 |
| `c5e62c1bc8` | 2026-11-05 21:32 -0600 | 17,815 | 3,026,439 | +431 | 4,228 |
| `d101a32047` | 2026-11-05 22:49 -0600 | 17,816 | 3,027,079 | +640 | 499 |
| `d8cba06837` | 2026-11-05 22:55 -0600 | 17,817 | 3,029,678 | +2,599 | 26,135 |
| `dda7fd7766` | 2026-11-05 23:09 -0600 | 17,818 | 3,030,580 | +902 | 3,829 |
| `4689d51fd5` | 2026-11-05 23:25 -0600 | 17,819 | 3,031,975 | +1,395 | 5,172 |
| `0988fa01c5` | 2026-11-05 23:32 -0600 | 17,819 | 3,031,975 | +0 | 0 |
| `9bcae7776a` | 2026-11-05 23:43 -0600 | 17,820 | 3,033,352 | +1,377 | 7,333 |
| `b5a55131a5` | 2026-11-06 00:04 -0600 | 17,821 | 3,034,584 | +1,232 | 3,557 |
| `dfa8d381b9` | 2026-11-06 00:35 -0600 | 17,822 | 3,035,500 | +916 | 1,745 |
| `0fb20a930e` | 2026-11-06 01:02 -0600 | 17,823 | 3,036,648 | +1,148 | 2,532 |
| `bed0bdd629` | 2026-11-06 02:04 -0600 | 17,824 | 3,037,307 | +659 | 639 |
| `39661fa3a5` | 2026-11-06 02:51 -0600 | 17,825 | 3,038,584 | +1,277 | 1,633 |
| `23a7d385a7` | 2026-11-06 02:57 -0600 | 17,826 | 3,039,620 | +1,036 | 10,748 |
| `ccf02f4cda` | 2026-11-06 03:57 -0600 | 17,827 | 3,041,550 | +1,930 | 1,927 |
| `d21f76282c` | 2026-11-06 05:10 -0600 | 17,827 | 3,041,550 | +0 | 0 |
| `cada1abea2` | 2026-11-06 06:56 -0600 | 17,827 | 3,041,550 | +0 | 0 |
| `f67b6a9c82` | 2026-11-06 07:17 -0600 | 17,828 | 3,042,335 | +785 | 2,252 |
| `a7d1bb2f04` | 2026-11-06 07:24 -0600 | 17,829 | 3,044,081 | +1,746 | 14,030 |
| `0ee287d54d` | 2026-11-06 07:26 -0600 | 17,830 | 3,046,665 | +2,584 | 72,112 |
| `a15e7da04e` | 2026-11-06 07:28 -0600 | 17,831 | 3,048,824 | +2,159 | 68,782 |
| `2976e30fcd` | 2026-11-06 07:30 -0600 | 17,832 | 3,050,684 | +1,860 | 48,522 |
| `88776a4175` | 2026-11-06 07:32 -0600 | 17,833 | 3,052,328 | +1,644 | 56,366 |
| `1538cfbc88` | 2026-11-06 07:34 -0600 | 17,834 | 3,053,317 | +989 | 32,664 |
| `6a53ea7a02` | 2026-11-06 07:36 -0600 | 17,835 | 3,054,267 | +950 | 32,264 |
| `d3790b05eb` | 2026-11-06 07:38 -0600 | 17,836 | 3,055,465 | +1,198 | 39,207 |
| `89fed92a0f` | 2026-11-06 07:39 -0600 | 17,838 | 3,057,502 | +2,037 | 71,196 |
| `50b899ac96` | 2026-11-06 07:42 -0600 | 17,839 | 3,058,471 | +969 | 23,893 |
| `b2b833461f` | 2026-11-06 07:44 -0600 | 17,839 | 3,058,471 | +0 | 0 |
| `9e1b775577` | 2026-11-06 07:52 -0600 | 17,840 | 3,058,854 | +383 | 2,837 |
| `442d471ca7` | 2026-11-06 08:03 -0600 | 17,841 | 3,059,504 | +650 | 3,508 |
| `44646883d6` | 2026-11-06 08:06 -0600 | 17,843 | 3,060,839 | +1,335 | 29,851 |
| `397f1859c8` | 2026-11-06 08:08 -0600 | 17,844 | 3,061,569 | +730 | 16,024 |
| `c4a6ce9705` | 2026-11-06 08:10 -0600 | 17,845 | 3,062,182 | +613 | 19,190 |
| `243af825ee` | 2026-11-06 08:21 -0600 | 17,845 | 3,062,182 | +0 | 0 |
| `cdaec816bf` | 2026-11-06 08:25 -0600 | 17,845 | 3,062,182 | +0 | 0 |
| `78acdb391d` | 2026-11-06 08:38 -0600 | 17,846 | 3,065,671 | +3,489 | 15,603 |
| `42b8389831` | 2026-11-06 08:41 -0600 | 17,847 | 3,075,208 | +9,537 | 256,218 |
| `caf1bc6ea5` | 2026-11-06 08:47 -0600 | 17,849 | 3,077,038 | +1,830 | 18,049 |
| `c3813742d2` | 2026-11-06 08:48 -0600 | 17,850 | 3,077,422 | +384 | 13,042 |
| `a95ad91034` | 2026-11-06 08:50 -0600 | 17,851 | 3,077,440 | +18 | 611 |
| `603395bca3` | 2026-11-06 08:52 -0600 | 17,852 | 3,077,864 | +424 | 12,936 |
| `2bc1a170c2` | 2026-11-06 08:54 -0600 | 17,853 | 3,078,194 | +330 | 10,703 |
| `282f1f4221` | 2026-11-06 08:56 -0600 | 17,854 | 3,078,487 | +293 | 9,172 |
| `733db3b0f0` | 2026-11-06 08:58 -0600 | 17,855 | 3,078,774 | +287 | 9,225 |
| `26c936a27b` | 2026-11-06 09:00 -0600 | 17,856 | 3,078,901 | +127 | 3,779 |
| `3ec1ed98d8` | 2026-11-06 09:02 -0600 | 17,857 | 3,078,976 | +75 | 1,901 |
| `f41b7234bb` | 2026-11-06 09:24 -0600 | 17,857 | 3,078,976 | +0 | 0 |
| `839559fffb` | 2026-11-06 09:26 -0600 | 17,858 | 3,083,128 | +4,152 | 102,378 |
| `f0db0104c4` | 2026-11-06 09:42 -0600 | 17,859 | 3,083,610 | +482 | 1,808 |
| `9a13bb3df3` | 2026-11-06 09:52 -0600 | 17,860 | 3,086,348 | +2,738 | 16,994 |
| `a1b5f2ba12` | 2026-11-06 09:57 -0600 | 17,861 | 3,086,454 | +106 | 1,192 |
| `f26bc5a9c6` | 2026-11-06 10:23 -0600 | 17,862 | 3,088,516 | +2,062 | 4,707 |
| `c23d9a6e30` | 2026-11-06 10:26 -0600 | 17,863 | 3,090,928 | +2,412 | 62,023 |
| `525259e3fe` | 2026-11-06 10:38 -0600 | 17,863 | 3,090,928 | +0 | 0 |
| `01e09f6a1e` | 2026-11-06 10:41 -0600 | 17,863 | 3,090,928 | +0 | 0 |
| `48956b2e63` | 2026-11-06 10:43 -0600 | 17,864 | 3,090,959 | +31 | 915 |
| `fd370adaa2` | 2026-11-06 10:49 -0600 | 17,865 | 3,097,144 | +6,185 | 58,905 |
| `50b65cb642` | 2026-11-06 11:14 -0600 | 17,866 | 3,097,407 | +263 | 637 |
| `70f09a94ad` | 2026-11-06 11:30 -0600 | 17,867 | 3,099,809 | +2,402 | 9,180 |
| `c311b6073a` | 2026-11-06 11:32 -0600 | 17,868 | 3,099,871 | +62 | 1,539 |
| `762e8e4c18` | 2026-11-06 11:34 -0600 | 17,868 | 3,099,871 | +0 | 0 |
| `7a827db7e3` | 2026-11-06 11:36 -0600 | 17,869 | 3,101,709 | +1,838 | 58,556 |
| `e2fe2e0794` | 2026-11-06 11:39 -0600 | 17,870 | 3,103,033 | +1,324 | 23,365 |
| `858bb53628` | 2026-11-06 11:57 -0600 | 17,871 | 3,105,471 | +2,438 | 8,311 |
| `819464c585` | 2026-11-06 12:20 -0600 | 17,872 | 3,106,215 | +744 | 1,961 |
| `46210cf3b3` | 2026-11-06 12:22 -0600 | 17,873 | 3,106,819 | +604 | 15,757 |
| `e5748ef2d1` | 2026-11-06 12:24 -0600 | 17,874 | 3,107,186 | +367 | 11,197 |
| `9d14040b8f` | 2026-11-06 12:52 -0600 | 17,875 | 3,107,842 | +656 | 1,389 |
| `3c0853a0d9` | 2026-11-06 13:06 -0600 | 17,876 | 3,109,522 | +1,680 | 7,513 |
| `35caab263e` | 2026-11-06 13:29 -0600 | 17,877 | 3,109,750 | +228 | 585 |
| `6d735597ac` | 2026-11-06 13:31 -0600 | 17,878 | 3,110,134 | +384 | 11,815 |
| `3200bf8b67` | 2026-11-06 13:34 -0600 | 17,879 | 3,110,753 | +619 | 14,470 |
| `9755878e9d` | 2026-11-06 13:36 -0600 | 17,880 | 3,113,111 | +2,358 | 66,319 |
| `99510b2137` | 2026-11-06 13:47 -0600 | 17,880 | 3,113,111 | +0 | 0 |
| `1d84e75734` | 2026-11-06 14:19 -0600 | 17,881 | 3,114,099 | +988 | 1,894 |
| `4c687d7e58` | 2026-11-06 14:40 -0600 | 17,882 | 3,114,561 | +462 | 1,278 |
| `e53cef1f35` | 2026-11-06 14:45 -0600 | 17,883 | 3,114,962 | +401 | 5,774 |
| `973dad5aaf` | 2026-11-06 14:48 -0600 | 17,885 | 3,115,772 | +810 | 15,429 |
| `852772cd47` | 2026-11-06 14:50 -0600 | 17,886 | 3,116,376 | +604 | 15,313 |
| `f974dbd271` | 2026-11-06 14:52 -0600 | 17,887 | 3,116,570 | +194 | 6,021 |
| `90d13d68f0` | 2026-11-06 14:54 -0600 | 17,888 | 3,116,736 | +166 | 5,242 |
| `dbfcdbceb8` | 2026-11-06 14:56 -0600 | 17,889 | 3,117,280 | +544 | 16,883 |
| `53ec79ccc7` | 2026-11-06 14:58 -0600 | 17,890 | 3,118,309 | +1,029 | 26,843 |
| `65c837b614` | 2026-11-06 15:17 -0600 | 17,893 | 3,118,507 | +198 | 618 |
| `c612850670` | 2026-11-06 15:19 -0600 | 17,894 | 3,119,158 | +651 | 21,114 |
| `076f872649` | 2026-11-06 15:21 -0600 | 17,895 | 3,119,355 | +197 | 6,389 |
| `90c44cd04d` | 2026-11-06 15:28 -0600 | 17,897 | 3,121,072 | +1,717 | 15,688 |
| `72e99f31c1` | 2026-11-06 15:30 -0600 | 17,898 | 3,121,786 | +714 | 22,747 |
| `d229cefb9c` | 2026-11-06 15:31 -0600 | 17,899 | 3,122,023 | +237 | 7,170 |
| `34910577ce` | 2026-11-06 15:33 -0600 | 17,900 | 3,122,072 | +49 | 1,561 |
| `ef6be5ea7d` | 2026-11-06 15:35 -0600 | 17,901 | 3,122,384 | +312 | 10,400 |
| `a9d534415a` | 2026-11-06 16:24 -0600 | 17,902 | 3,122,554 | +170 | 208 |
| `34dc5c9f10` | 2026-11-06 16:26 -0600 | 17,903 | 3,122,835 | +281 | 8,430 |
| `72e364efc8` | 2026-11-06 16:28 -0600 | 17,903 | 3,122,835 | +0 | 0 |
| `69b8757b4d` | 2026-11-06 16:31 -0600 | 17,904 | 3,123,440 | +605 | 15,338 |
| `dae5ec4877` | 2026-11-06 16:33 -0600 | 17,904 | 3,123,440 | +0 | 0 |
| `272c14ef4b` | 2026-11-06 16:36 -0600 | 17,905 | 3,124,671 | +1,231 | 31,430 |
| `923c867005` | 2026-11-06 16:54 -0600 | 17,906 | 3,129,187 | +4,516 | 15,123 |
| `53c5566d3c` | 2026-11-06 16:56 -0600 | 17,907 | 3,130,182 | +995 | 24,703 |
| `a430267c83` | 2026-11-06 17:06 -0600 | 17,907 | 3,130,182 | +0 | 0 |
| `f1cd3d5eb2` | 2026-11-06 17:15 -0600 | 17,908 | 3,130,754 | +572 | 4,030 |
| `1f8ddb4e18` | 2026-11-06 17:34 -0600 | 17,909 | 3,131,480 | +726 | 2,261 |
| `fb8e7e1378` | 2026-11-06 17:58 -0600 | 17,910 | 3,132,451 | +971 | 2,455 |
| `093e1046eb` | 2026-11-06 18:35 -0600 | 17,911 | 3,133,783 | +1,332 | 2,137 |
| `976fb4283e` | 2026-11-06 19:00 -0600 | 17,911 | 3,133,783 | +0 | 0 |
| `f2b20e81ee` | 2026-11-06 19:03 -0600 | 17,912 | 3,135,696 | +1,913 | 37,026 |
| `3e60a6d4fc` | 2026-11-06 19:05 -0600 | 17,913 | 3,137,510 | +1,814 | 53,970 |
| `6d54ed9450` | 2026-11-06 20:00 -0600 | 17,913 | 3,137,510 | +0 | 0 |
| `84c628c8ef` | 2026-11-06 20:35 -0600 | 17,914 | 3,141,287 | +3,777 | 6,438 |
| `df005d5e9e` | 2026-11-06 21:22 -0600 | 17,914 | 3,141,287 | +0 | 0 |
| `7bda0dcba8` | 2026-11-06 21:25 -0600 | 17,914 | 3,141,287 | +0 | 0 |
| `89ceb01a21` | 2026-11-06 22:23 -0600 | 17,915 | 3,142,858 | +1,571 | 1,627 |
| `a6435ecb88` | 2026-11-06 22:44 -0600 | 17,916 | 3,144,083 | +1,225 | 3,568 |
| `b9562d5200` | 2026-11-06 22:46 -0600 | 17,917 | 3,144,210 | +127 | 3,687 |
| `2239792d8f` | 2026-11-06 23:26 -0600 | 17,918 | 3,145,773 | +1,563 | 2,321 |
| `7f103deb53` | 2026-11-06 23:38 -0600 | 17,919 | 3,147,497 | +1,724 | 8,514 |
| `3baeeb23d8` | 2026-11-06 23:55 -0600 | 17,919 | 3,147,497 | +0 | 0 |
| `3957557bd1` | 2026-11-06 23:57 -0600 | 17,920 | 3,148,072 | +575 | 15,682 |
| `2742976cba` | 2026-11-07 00:00 -0600 | 17,922 | 3,151,928 | +3,856 | 77,120 |
| `b60e81bde9` | 2026-11-07 00:14 -0600 | 17,923 | 3,152,763 | +835 | 3,613 |
| `973a17f3c5` | 2026-11-07 00:26 -0600 | 17,924 | 3,152,965 | +202 | 956 |
| `4b94aac651` | 2026-11-07 00:29 -0600 | 17,925 | 3,153,374 | +409 | 9,949 |
| `41e7997518` | 2026-11-07 01:01 -0600 | 17,926 | 3,164,171 | +10,797 | 20,139 |
| `4ef0d7beb4` | 2026-11-07 01:39 -0600 | 17,927 | 3,165,089 | +918 | 1,466 |
| `8745c05cfa` | 2026-11-07 01:48 -0600 | 17,928 | 3,169,521 | +4,432 | 27,894 |
| `36906a233b` | 2026-11-07 01:51 -0600 | 17,929 | 3,170,902 | +1,381 | 27,020 |
| `905de7d57c` | 2026-11-07 02:23 -0600 | 17,929 | 3,170,902 | +0 | 0 |
| `c19176ef07` | 2026-11-07 02:25 -0600 | 17,930 | 3,171,214 | +312 | 8,986 |
| `3042c2929b` | 2026-11-07 02:47 -0600 | 17,931 | 3,173,982 | +2,768 | 7,607 |
| `498e48194b` | 2026-11-07 02:54 -0600 | 17,932 | 3,174,099 | +117 | 1,012 |
| `b77a1d163c` | 2026-11-07 03:09 -0600 | 17,932 | 3,174,099 | +0 | 0 |
| `401014fc0e` | 2026-11-07 03:11 -0600 | 17,933 | 3,174,612 | +513 | 13,480 |
| `443c5deeb9` | 2026-11-07 03:24 -0600 | 17,934 | 3,174,729 | +117 | 528 |
| `3b1705d371` | 2026-11-07 03:52 -0600 | 17,935 | 3,175,112 | +383 | 814 |
| `0c1b9736fd` | 2026-11-07 04:13 -0600 | 17,936 | 3,175,121 | +9 | 27 |
| `403be3d4eb` | 2026-11-07 04:32 -0600 | 17,936 | 3,175,121 | +0 | 0 |
| `c49030307a` | 2026-11-07 05:47 -0600 | 17,937 | 3,176,939 | +1,818 | 1,465 |
| `8d3bddcfc4` | 2026-11-07 06:02 -0600 | 17,938 | 3,179,299 | +2,360 | 9,367 |
| `7a18bf4eb8` | 2026-11-07 07:36 -0600 | 17,938 | 3,179,299 | +0 | 0 |
| `5fb7390d50` | 2026-11-07 07:54 -0600 | 17,938 | 3,179,299 | +0 | 0 |
| `0073a015f1` | 2026-11-07 08:45 -0600 | 17,938 | 3,179,299 | +0 | 0 |
| `6ffacf5f05` | 2026-11-07 09:12 -0600 | 17,939 | 3,183,687 | +4,388 | 9,510 |
| `6b0b5d8b7b` | 2026-11-07 10:02 -0600 | 17,939 | 3,183,687 | +0 | 0 |
| `713ff31ec1` | 2026-11-07 10:50 -0600 | 17,939 | 3,183,687 | +0 | 0 |
| `eb78b23877` | 2026-11-07 11:54 -0600 | 17,940 | 3,184,268 | +581 | 545 |
| `9e6a5064df` | 2026-11-07 12:16 -0600 | 17,940 | 3,184,268 | +0 | 0 |
| `571e127fce` | 2026-11-07 12:28 -0600 | 17,941 | 3,185,214 | +946 | 4,790 |
| `ac6640f542` | 2026-11-07 12:48 -0600 | 17,942 | 3,186,062 | +848 | 2,611 |
| `be5a89f792` | 2026-11-07 13:23 -0600 | 17,943 | 3,186,062 | +0 | 0 |
| `2cebbb01c0` | 2026-11-07 13:39 -0600 | 17,944 | 3,191,713 | +5,651 | 21,191 |
| `6e9c9dfe38` | 2026-11-07 13:42 -0600 | 17,944 | 3,191,713 | +0 | 0 |
| `0ecf38b24b` | 2026-11-07 14:02 -0600 | 17,945 | 3,193,094 | +1,381 | 4,088 |
| `f79b01d95d` | 2026-11-07 14:17 -0600 | 17,945 | 3,193,094 | +0 | 0 |
| `96245f7ed9` | 2026-11-07 14:20 -0600 | 17,945 | 3,193,094 | +0 | 0 |
| `2dbd8ba79f` | 2026-11-07 14:45 -0600 | 17,945 | 3,193,094 | +0 | 0 |
| `50eb21daa7` | 2026-11-07 14:53 -0600 | 17,946 | 3,193,299 | +205 | 1,587 |
| `c2f913337f` | 2026-11-07 15:14 -0600 | 17,947 | 3,193,299 | +0 | 0 |
| `b5325bc26e` | 2026-11-07 15:16 -0600 | 17,948 | 3,194,807 | +1,508 | 35,252 |
| `34386123d9` | 2026-11-07 15:22 -0600 | 17,949 | 3,197,377 | +2,570 | 25,210 |
| `92f3eb5ba2` | 2026-11-07 15:38 -0600 | 17,949 | 3,197,377 | +0 | 0 |
| `5298300bee` | 2026-11-07 15:53 -0600 | 17,952 | 3,197,377 | +0 | 0 |
| `8058f3a7d1` | 2026-11-07 16:00 -0600 | 17,954 | 3,197,830 | +453 | 3,874 |
| `ee452133cf` | 2026-11-07 16:08 -0600 | 17,955 | 3,200,410 | +2,580 | 18,502 |
| `e0ec2e5877` | 2026-11-07 17:00 -0600 | 17,956 | 3,203,945 | +3,535 | 4,136 |
| `7498401baa` | 2026-11-07 17:16 -0600 | 17,957 | 3,204,404 | +459 | 1,704 |
| `e5ff2a3b74` | 2026-11-07 17:53 -0600 | 17,957 | 3,204,404 | +0 | 0 |
| `59f5a9195f` | 2026-11-07 18:18 -0600 | 17,958 | 3,204,616 | +212 | 503 |
| `2c6e903135` | 2026-11-07 18:20 -0600 | 17,959 | 3,206,126 | +1,510 | 44,195 |
| `7225107031` | 2026-11-07 19:37 -0600 | 17,960 | 3,206,523 | +397 | 310 |
| `7aa048d433` | 2026-11-07 19:39 -0600 | 17,960 | 3,206,523 | +0 | 0 |
| `92bb78b03a` | 2026-11-07 20:28 -0600 | 17,962 | 3,206,535 | +12 | 15 |
| `70ec9b4bac` | 2026-11-07 20:33 -0600 | 17,963 | 3,207,269 | +734 | 10,612 |
| `ab585efa2d` | 2026-11-07 20:36 -0600 | 17,892 | 3,204,135 | -3,134 | -58,762 |
| `d12d1d9d09` | 2026-11-07 20:38 -0600 | 17,892 | 3,204,135 | +0 | 0 |
| `3e9a7d531b` | 2026-11-07 21:07 -0600 | 17,893 | 3,205,402 | +1,267 | 2,633 |
| `3e177d4221` | 2026-11-07 21:09 -0600 | 17,893 | 3,205,402 | +0 | 0 |
| `45192abcb3` | 2026-11-07 21:12 -0600 | 17,874 | 3,204,999 | -403 | -7,009 |
| `3dd859a9d9` | 2026-11-07 21:15 -0600 | 17,875 | 3,207,742 | +2,743 | 56,107 |
| `dd120165c5` | 2026-11-07 21:21 -0600 | 17,875 | 3,207,742 | +0 | 0 |
| `5edb4c29fe` | 2026-11-07 21:24 -0600 | 17,870 | 3,207,701 | -41 | -1,032 |
| `24743e8f0b` | 2026-11-08 00:53 -0600 | 17,871 | 3,209,087 | +1,386 | 397 |
| `4f17146fbe` | 2026-11-08 02:41 -0600 | 17,871 | 3,209,087 | +0 | 0 |
| `3439108934` | 2026-11-08 02:43 -0600 | 17,871 | 3,209,087 | +0 | 0 |
| `f75d3c9592` | 2026-11-08 04:17 -0600 | 17,872 | 3,211,847 | +2,760 | 1,763 |
| `c79924a35e` | 2026-11-08 05:02 -0600 | 17,872 | 3,211,847 | +0 | 0 |
| `f3b9ab65a9` | 2026-11-08 05:04 -0600 | 17,872 | 3,211,847 | +0 | 0 |
| `a700045433` | 2026-11-08 05:08 -0600 | 17,872 | 3,211,847 | +0 | 0 |
| `e8920ed7e1` | 2026-11-08 05:10 -0600 | 17,872 | 3,211,847 | +0 | 0 |
| `9e771f7174` | 2026-11-08 05:13 -0600 | 17,872 | 3,211,847 | +0 | 0 |
| `ac34106325` | 2026-11-08 05:16 -0600 | 17,872 | 3,211,847 | +0 | 0 |
| `a4a476fa43` | 2026-11-08 05:22 -0600 | 17,873 | 3,213,112 | +1,265 | 11,890 |
| `0b3289c271` | 2026-11-08 05:28 -0600 | 17,874 | 3,214,426 | +1,314 | 13,439 |
| `8a57ab050c` | 2026-11-08 06:21 -0600 | 17,875 | 3,214,885 | +459 | 521 |
| `42d821f5d0` | 2026-11-08 15:48 -0600 | 17,875 | 3,214,885 | +0 | 0 |
| `2fc293365b` | 2026-11-08 15:51 -0600 | 17,875 | 3,214,885 | +0 | 0 |
| `33a8c2b84c` | 2026-11-08 15:54 -0600 | 17,875 | 3,214,885 | +0 | 0 |
| `08aa729b68` | 2026-11-08 16:00 -0600 | 17,876 | 3,215,794 | +909 | 9,513 |
| `c6452038ac` | 2026-11-08 16:56 -0600 | 17,876 | 3,215,794 | +0 | 0 |
| `516fe774e3` | 2026-11-08 16:59 -0600 | 17,876 | 3,215,794 | +0 | 0 |
| `c5eb8ba605` | 2026-11-08 17:02 -0600 | 17,876 | 3,215,794 | +0 | 0 |
| `519c414523` | 2026-11-08 17:33 -0600 | 17,876 | 3,215,794 | +0 | 0 |
| `a23c72ead6` | 2026-11-08 17:36 -0600 | 17,876 | 3,215,794 | +0 | 0 |
| `e58cfa3e0e` | 2026-11-08 17:41 -0600 | 17,876 | 3,215,794 | +0 | 0 |
| `4ba1ec48cd` | 2026-11-08 17:49 -0600 | 17,876 | 3,215,794 | +0 | 0 |
| `93fa95f26b` | 2026-11-08 17:57 -0600 | 17,876 | 3,215,794 | +0 | 0 |
| `7a8ea23499` | 2026-11-08 18:52 -0600 | 17,876 | 3,215,794 | +0 | 0 |
| `c7c6a26bdf` | 2026-11-08 18:57 -0600 | 17,876 | 3,215,794 | +0 | 0 |
| `479491dc3f` | 2026-11-08 19:04 -0600 | 17,876 | 3,215,794 | +0 | 0 |
| `239931e7f1` | 2026-11-08 19:31 -0600 | 17,876 | 3,215,794 | +0 | 0 |
| `963e247e6a` | 2026-11-08 19:35 -0600 | 17,876 | 3,215,794 | +0 | 0 |
| `74183a8657` | 2026-11-08 19:37 -0600 | 17,876 | 3,215,794 | +0 | 0 |
| `945a325da6` | 2026-11-08 19:40 -0600 | 17,876 | 3,215,794 | +0 | 0 |
| `5234a59126` | 2026-11-08 19:42 -0600 | 17,876 | 3,215,794 | +0 | 0 |
| `4653894024` | 2026-11-08 21:11 -0600 | 17,876 | 3,215,794 | +0 | 0 |
| `688370cf76` | 2026-11-08 21:14 -0600 | 17,876 | 3,215,794 | +0 | 0 |
| `473e3eb370` | 2026-11-08 21:16 -0600 | 17,876 | 3,215,794 | +0 | 0 |
| `62004d1cf7` | 2026-11-08 21:43 -0600 | 17,876 | 3,215,794 | +0 | 0 |
| `0f4cc80227` | 2026-11-08 21:46 -0600 | 17,876 | 3,215,794 | +0 | 0 |
| `5fbd8bed92` | 2026-11-08 21:48 -0600 | 17,876 | 3,215,794 | +0 | 0 |
| `f87c5bd441` | 2026-11-08 21:50 -0600 | 17,876 | 3,215,794 | +0 | 0 |
| `248540058c` | 2026-11-08 22:05 -0600 | 17,876 | 3,215,794 | +0 | 0 |
| `010640d78a` | 2026-11-08 22:25 -0600 | 17,876 | 3,215,794 | +0 | 0 |
| `2d96d3184b` | 2026-11-08 22:28 -0600 | 17,876 | 3,215,794 | +0 | 0 |
| `75a0e8d72e` | 2026-11-08 22:30 -0600 | 17,876 | 3,215,794 | +0 | 0 |
| `e9c023221a` | 2026-11-08 23:29 -0600 | 17,876 | 3,215,794 | +0 | 0 |
| `e382dc4868` | 2026-11-09 00:18 -0600 | 17,876 | 3,215,794 | +0 | 0 |
| `dce4188f13` | 2026-11-09 00:21 -0600 | 17,876 | 3,215,794 | +0 | 0 |
| `7db81a0f5d` | 2026-11-09 00:27 -0600 | 17,876 | 3,215,794 | +0 | 0 |
| `cf1cc7bf6a` | 2026-11-09 00:30 -0600 | 17,876 | 3,215,794 | +0 | 0 |
| `caeb3ff0cf` | 2026-11-09 01:28 -0600 | 17,876 | 3,215,794 | +0 | 0 |
| `18a4e7fd7f` | 2026-11-09 01:31 -0600 | 17,876 | 3,215,794 | +0 | 0 |
| `b42a83ade3` | 2026-11-09 01:41 -0600 | 17,878 | 3,217,466 | +1,672 | 9,569 |
| `2d533018fc` | 2026-11-09 03:13 -0600 | 17,878 | 3,217,466 | +0 | 0 |
| `de9a94ad6c` | 2026-11-09 03:16 -0600 | 17,879 | 3,218,781 | +1,315 | 26,898 |
| `59d91da3ab` | 2026-11-09 05:07 -0600 | 17,880 | 3,218,823 | +42 | 23 |
| `3d40136795` | 2026-11-09 05:10 -0600 | 17,881 | 3,221,324 | +2,501 | 46,651 |
| `b104180494` | 2026-11-09 05:36 -0600 | 17,882 | 3,231,400 | +10,076 | 23,297 |
| `0408221bd4` | 2026-11-09 05:36 -0600 | 17,882 | 3,231,400 | +0 | 0 |
| `ee4462e468` | 2026-11-09 06:24 -0600 | 17,883 | 3,233,045 | +1,645 | 2,061 |
| `0a4e027b93` | 2026-11-09 06:32 -0600 | 17,884 | 3,239,064 | +6,019 | 48,259 |
| `8f57ea38bf` | 2026-11-09 07:04 -0600 | 17,885 | 3,242,689 | +3,625 | 6,703 |
| `c57022d71d` | 2026-11-09 08:13 -0600 | 17,885 | 3,242,689 | +0 | 0 |
| `9815872da9` | 2026-11-09 08:20 -0600 | 17,885 | 3,242,689 | +0 | 0 |
| `c6945a7e06` | 2026-11-09 08:23 -0600 | 17,885 | 3,242,689 | +0 | 0 |
| `1feea9ba18` | 2026-11-09 08:26 -0600 | 17,885 | 3,242,689 | +0 | 0 |
| `e90e481e46` | 2026-11-09 08:29 -0600 | 17,885 | 3,242,689 | +0 | 0 |
| `b4d97fc521` | 2026-11-09 08:37 -0600 | 17,887 | 3,249,617 | +6,928 | 53,406 |
| `09e27b3a0a` | 2026-11-09 09:17 -0600 | 17,888 | 3,252,976 | +3,359 | 5,047 |
| `9f3563929d` | 2026-11-09 09:28 -0600 | 17,888 | 3,252,976 | +0 | 0 |
| `0449c1a79b` | 2026-11-09 10:33 -0600 | 17,890 | 3,253,303 | +327 | 303 |
| `7d8f8a2e34` | 2026-11-09 11:04 -0600 | 17,890 | 3,253,303 | +0 | 0 |
| `ca72aecc1b` | 2026-11-09 11:05 -0600 | 17,890 | 3,253,303 | +0 | 0 |
| `25ad91425a` | 2026-11-09 11:07 -0600 | 17,891 | 3,253,723 | +420 | 10,013 |
| `fd3202f821` | 2026-11-09 11:19 -0600 | 17,892 | 3,256,480 | +2,757 | 14,532 |
| `365929b228` | 2026-11-09 12:04 -0600 | 17,890 | 3,255,512 | -968 | -1,270 |
| `d6989b647e` | 2026-11-09 12:26 -0600 | 17,890 | 3,255,512 | +0 | 0 |
| `c562812bfb` | 2026-11-09 12:49 -0600 | 17,891 | 3,256,122 | +610 | 1,529 |
| `ca79f8e093` | 2026-11-09 13:15 -0600 | 17,892 | 3,257,303 | +1,181 | 2,754 |
| `93452981bb` | 2026-11-09 13:59 -0600 | 17,892 | 3,257,303 | +0 | 0 |
| `7f6b7e9936` | 2026-11-09 14:41 -0600 | 17,892 | 3,257,303 | +0 | 0 |
| `d9fdd6f337` | 2026-11-09 14:46 -0600 | 17,892 | 3,257,303 | +0 | 0 |
| `5e3089f269` | 2026-11-09 15:45 -0600 | 17,892 | 3,257,303 | +0 | 0 |
| `649a1823ac` | 2026-11-09 16:04 -0600 | 17,892 | 3,257,303 | +0 | 0 |
| `b5e7063db2` | 2026-11-09 16:12 -0600 | 17,892 | 3,257,303 | +0 | 0 |
| `6987fba11f` | 2026-11-09 16:19 -0600 | 17,892 | 3,257,303 | +0 | 0 |
| `16103350bb` | 2026-11-09 16:27 -0600 | 17,892 | 3,257,303 | +0 | 0 |
| `6edc6ff64d` | 2026-11-09 16:43 -0600 | 17,895 | 3,257,310 | +7 | 26 |
| `840c35bfe9` | 2026-11-09 17:42 -0600 | 17,895 | 3,257,310 | +0 | 0 |
| `868eae5b99` | 2026-11-09 18:06 -0600 | 17,896 | 3,258,098 | +788 | 1,980 |
| `8c6388e0d7` | 2026-11-09 18:10 -0600 | 17,896 | 3,258,098 | +0 | 0 |
| `fc2ee1ee85` | 2026-11-09 18:13 -0600 | 17,897 | 3,258,206 | +108 | 2,015 |
| `050a3b5240` | 2026-11-09 18:27 -0600 | 17,897 | 3,258,206 | +0 | 0 |
| `e6aa37dc36` | 2026-11-09 18:41 -0600 | 17,897 | 3,258,206 | +0 | 0 |
| `d21eab23cd` | 2026-11-09 18:48 -0600 | 17,897 | 3,258,206 | +0 | 0 |
| `7272c11af7` | 2026-11-09 18:53 -0600 | 17,897 | 3,258,206 | +0 | 0 |
| `e5e2a92989` | 2026-11-09 19:03 -0600 | 17,897 | 3,258,206 | +0 | 0 |
| `426cf96125` | 2026-11-09 19:06 -0600 | 17,897 | 3,258,206 | +0 | 0 |
| `4b90d6034f` | 2026-11-09 19:20 -0600 | 17,898 | 3,261,157 | +2,951 | 12,769 |
| `c88ab820b9` | 2026-11-09 19:22 -0600 | 17,899 | 3,261,339 | +182 | 4,020 |
| `c3b71d6ac5` | 2026-11-09 19:31 -0600 | 17,899 | 3,261,339 | +0 | 0 |
| `1005f206ea` | 2026-11-09 19:34 -0600 | 17,899 | 3,261,339 | +0 | 0 |
| `c59ded1bcc` | 2026-11-09 19:40 -0600 | 17,899 | 3,261,339 | +0 | 0 |
| `b72163023d` | 2026-11-09 19:43 -0600 | 17,899 | 3,261,339 | +0 | 0 |
| `c7562ed3a8` | 2026-11-09 20:30 -0600 | 17,899 | 3,261,339 | +0 | 0 |
| `da62461e8b` | 2026-11-09 20:40 -0600 | 17,899 | 3,261,339 | +0 | 0 |
| `0d80a41497` | 2026-11-09 20:44 -0600 | 17,899 | 3,261,339 | +0 | 0 |
| `3482f1078f` | 2026-11-09 20:48 -0600 | 17,899 | 3,261,339 | +0 | 0 |
| `1328086e49` | 2026-11-09 20:51 -0600 | 17,899 | 3,261,339 | +0 | 0 |
| `02bac133f0` | 2026-11-09 20:57 -0600 | 17,899 | 3,261,339 | +0 | 0 |
| `322379c001` | 2026-11-09 21:34 -0600 | 17,899 | 3,261,339 | +0 | 0 |
| `2b1978ef88` | 2026-11-09 22:12 -0600 | 17,899 | 3,261,339 | +0 | 0 |
| `bbfcbab078` | 2026-11-09 22:24 -0600 | 17,899 | 3,261,339 | +0 | 0 |
| `665e864834` | 2026-11-09 22:29 -0600 | 17,899 | 3,261,339 | +0 | 0 |
| `80a71ac738` | 2026-11-09 23:03 -0600 | 17,899 | 3,261,339 | +0 | 0 |
| `474d1d4bd7` | 2026-11-09 23:06 -0600 | 17,899 | 3,261,339 | +0 | 0 |
| `1c73b1da0e` | 2026-11-09 23:08 -0600 | 17,899 | 3,261,339 | +0 | 0 |
| `c884edbcb2` | 2026-11-09 23:15 -0600 | 17,899 | 3,261,339 | +0 | 0 |
| `f091c71909` | 2026-11-09 23:19 -0600 | 17,899 | 3,261,339 | +0 | 0 |
| `ac18652cc3` | 2026-11-09 23:22 -0600 | 17,899 | 3,261,339 | +0 | 0 |
| `7788021147` | 2026-11-09 23:46 -0600 | 17,899 | 3,261,339 | +0 | 0 |
| `ec777e8b43` | 2026-11-09 23:49 -0600 | 17,899 | 3,261,339 | +0 | 0 |
| `6fca13a7a7` | 2026-11-09 23:51 -0600 | 17,899 | 3,261,339 | +0 | 0 |
| `93fd49cdec` | 2026-11-09 23:53 -0600 | 17,899 | 3,261,339 | +0 | 0 |
| `aa83d8f5f2` | 2026-11-09 23:55 -0600 | 17,899 | 3,261,339 | +0 | 0 |
| `1208d01fbb` | 2026-11-10 01:07 -0600 | 17,899 | 3,261,339 | +0 | 0 |
| `49d1898872` | 2026-11-10 13:43 -0600 | 17,900 | 3,262,475 | +1,136 | 90 |
| `de7411e3d2` | 2026-11-10 14:56 -0600 | 17,901 | 3,263,307 | +832 | 682 |
| `660a9eced4` | 2026-11-10 15:49 -0600 | 17,901 | 3,263,307 | +0 | 0 |
| `c4f583ddb9` | 2026-11-10 16:24 -0600 | 17,902 | 3,264,423 | +1,116 | 1,940 |
| `62c8e0e8ac` | 2026-11-10 16:57 -0600 | 17,902 | 3,264,423 | +0 | 0 |
| `dbe67335ca` | 2026-11-10 17:15 -0600 | 17,902 | 3,264,423 | +0 | 0 |
| `d660ee4ded` | 2026-11-10 17:21 -0600 | 17,902 | 3,264,423 | +0 | 0 |
| `e81c9ee163` | 2026-11-10 17:26 -0600 | 17,902 | 3,264,423 | +0 | 0 |
| `4d0efe6dd0` | 2026-11-10 17:32 -0600 | 17,902 | 3,264,423 | +0 | 0 |
| `c14f9c6958` | 2026-11-10 17:37 -0600 | 17,902 | 3,264,423 | +0 | 0 |
| `354c6d96b4` | 2026-11-10 18:14 -0600 | 17,902 | 3,264,423 | +0 | 0 |
| `1f83893b5e` | 2026-11-10 18:19 -0600 | 17,902 | 3,264,423 | +0 | 0 |
| `bb38a21e78` | 2026-11-10 18:24 -0600 | 17,902 | 3,264,423 | +0 | 0 |
| `87ad033452` | 2026-11-10 18:29 -0600 | 17,902 | 3,264,423 | +0 | 0 |
| `1118c4bd6b` | 2026-11-10 18:33 -0600 | 17,902 | 3,264,423 | +0 | 0 |
| `451f8db717` | 2026-11-10 19:30 -0600 | 17,903 | 3,269,034 | +4,611 | 4,872 |
| `e5f5ae395a` | 2026-11-10 19:59 -0600 | 17,903 | 3,269,034 | +0 | 0 |
| `99141f1a22` | 2026-11-10 20:04 -0600 | 17,903 | 3,269,034 | +0 | 0 |
| `5a792e7268` | 2026-11-10 20:11 -0600 | 17,903 | 3,269,034 | +0 | 0 |
| `39b6058dde` | 2026-11-10 20:15 -0600 | 17,903 | 3,269,034 | +0 | 0 |
| `679a22e6ab` | 2026-11-10 20:20 -0600 | 17,903 | 3,269,034 | +0 | 0 |
| `8021c5aba7` | 2026-11-10 20:24 -0600 | 17,903 | 3,269,034 | +0 | 0 |
| `ce5bfa564b` | 2026-11-10 20:28 -0600 | 17,903 | 3,269,034 | +0 | 0 |
| `7f5d6ad1c0` | 2026-11-10 20:36 -0600 | 17,903 | 3,269,034 | +0 | 0 |
| `4982628bfd` | 2026-11-10 21:28 -0600 | 17,903 | 3,269,034 | +0 | 0 |
| `1113f8435f` | 2026-11-10 23:13 -0600 | 17,903 | 3,269,034 | +0 | 0 |
| `d12735e809` | 2026-11-10 23:18 -0600 | 17,903 | 3,269,034 | +0 | 0 |
| `74a9490def` | 2026-11-10 23:22 -0600 | 17,903 | 3,269,034 | +0 | 0 |
| `12bbb5b51d` | 2026-11-10 23:27 -0600 | 17,903 | 3,269,034 | +0 | 0 |
| `4906726acd` | 2026-11-10 23:47 -0600 | 17,903 | 3,269,034 | +0 | 0 |
| `77525d526c` | 2026-11-10 23:51 -0600 | 17,903 | 3,269,034 | +0 | 0 |
| `128341e02c` | 2026-11-10 23:55 -0600 | 17,903 | 3,269,034 | +0 | 0 |
| `f6e67bff5e` | 2026-11-10 23:59 -0600 | 17,903 | 3,269,034 | +0 | 0 |
| `0571462405` | 2026-11-11 00:03 -0600 | 17,903 | 3,269,034 | +0 | 0 |
| `d085e807a8` | 2026-11-11 00:08 -0600 | 17,903 | 3,269,034 | +0 | 0 |
| `5be69695a3` | 2026-11-11 00:27 -0600 | 17,903 | 3,269,034 | +0 | 0 |
| `d57333d460` | 2026-11-11 00:34 -0600 | 17,903 | 3,269,034 | +0 | 0 |
| `888a0606bb` | 2026-11-11 00:38 -0600 | 17,903 | 3,269,034 | +0 | 0 |
| `6406ccdcc4` | 2026-11-11 00:41 -0600 | 17,904 | 3,270,592 | +1,558 | 33,993 |
| `ef0407731a` | 2026-11-11 00:54 -0600 | 17,904 | 3,270,592 | +0 | 0 |
| `7870e25b51` | 2026-11-11 00:59 -0600 | 17,904 | 3,270,592 | +0 | 0 |
| `4a8c6029b1` | 2026-11-11 01:03 -0600 | 17,904 | 3,270,592 | +0 | 0 |
| `1f0e3c3b3f` | 2026-11-11 01:07 -0600 | 17,904 | 3,270,592 | +0 | 0 |
| `5700f31fe7` | 2026-11-11 01:22 -0600 | 17,904 | 3,270,592 | +0 | 0 |
| `2a5f87308f` | 2026-11-11 01:27 -0600 | 17,904 | 3,270,592 | +0 | 0 |
| `b9a9c9e4f3` | 2026-11-11 01:31 -0600 | 17,904 | 3,270,592 | +0 | 0 |
| `21d685f7be` | 2026-11-11 01:46 -0600 | 17,904 | 3,270,592 | +0 | 0 |
| `5c4372a32e` | 2026-11-11 01:50 -0600 | 17,904 | 3,270,592 | +0 | 0 |
| `3a3a53e49e` | 2026-11-11 01:54 -0600 | 17,904 | 3,270,592 | +0 | 0 |
| `4d26f0fa4d` | 2026-11-11 01:58 -0600 | 17,904 | 3,270,592 | +0 | 0 |
| `036e40916a` | 2026-11-11 02:02 -0600 | 17,904 | 3,270,592 | +0 | 0 |
| `cfbf729f98` | 2026-11-11 02:08 -0600 | 17,904 | 3,270,592 | +0 | 0 |
| `3a1b0593ea` | 2026-11-11 02:30 -0600 | 17,904 | 3,270,592 | +0 | 0 |
| `d5afc235eb` | 2026-11-11 02:35 -0600 | 17,904 | 3,270,592 | +0 | 0 |
| `db2235611c` | 2026-11-11 02:40 -0600 | 17,904 | 3,270,592 | +0 | 0 |
| `5187fc1d3f` | 2026-11-11 02:45 -0600 | 17,904 | 3,270,592 | +0 | 0 |
| `f94552b7d1` | 2026-11-11 02:49 -0600 | 17,904 | 3,270,592 | +0 | 0 |
| `9039fb3ffd` | 2026-11-11 02:53 -0600 | 17,904 | 3,270,592 | +0 | 0 |
| `df21927a31` | 2026-11-11 02:57 -0600 | 17,904 | 3,270,592 | +0 | 0 |
| `b2672c886e` | 2026-11-11 03:01 -0600 | 17,904 | 3,270,592 | +0 | 0 |
| `343055cee9` | 2026-11-11 03:13 -0600 | 17,904 | 3,270,592 | +0 | 0 |
| `f112a76086` | 2026-11-11 03:28 -0600 | 17,904 | 3,270,592 | +0 | 0 |
| `fdca11e73b` | 2026-11-11 03:44 -0600 | 17,904 | 3,270,592 | +0 | 0 |
| `2d8c61008d` | 2026-11-11 03:54 -0600 | 17,904 | 3,270,592 | +0 | 0 |
| `0ee059823c` | 2026-11-11 04:03 -0600 | 17,904 | 3,270,592 | +0 | 0 |
| `2c6345eeab` | 2026-11-11 04:37 -0600 | 17,904 | 3,270,592 | +0 | 0 |
| `6eb7f5f04d` | 2026-11-11 05:02 -0600 | 17,904 | 3,270,592 | +0 | 0 |
| `8f7ead8394` | 2026-11-11 06:35 -0600 | 17,904 | 3,270,592 | +0 | 0 |
| `d4fe8d1883` | 2026-11-11 09:43 -0600 | 17,904 | 3,270,592 | +0 | 0 |
| `61bd12e93e` | 2026-11-11 11:06 -0600 | 17,904 | 3,270,592 | +0 | 0 |
| `11bbfff525` | 2026-11-11 11:06 -0600 | 17,904 | 3,270,592 | +0 | 0 |
| `1fa84e26eb` | 2026-11-11 12:23 -0600 | 17,904 | 3,270,592 | +0 | 0 |
| `f00492f1f0` | 2026-11-11 12:23 -0600 | 17,904 | 3,270,592 | +0 | 0 |
| `344f67ea7a` | 2026-11-11 12:23 -0600 | 17,904 | 3,270,592 | +0 |  |
| `1a037ba0c9` | 2026-11-11 12:23 -0600 | 17,904 | 3,270,592 | +0 |  |
| `fa09f00a96` | 2026-11-11 12:23 -0600 | 17,904 | 3,270,592 | +0 |  |
| `5da5c86ad8` | 2026-11-11 12:23 -0600 | 17,904 | 3,270,592 | +0 |  |
| `c45d7ad25f` | 2026-11-11 12:23 -0600 | 17,904 | 3,270,592 | +0 |  |
| `27d31cbe97` | 2026-11-11 12:23 -0600 | 17,904 | 3,270,592 | +0 |  |
| `e7b947a362` | 2026-11-11 12:23 -0600 | 17,904 | 3,270,592 | +0 |  |
| `60bd3544de` | 2026-11-11 12:23 -0600 | 17,904 | 3,270,592 | +0 |  |
| `ee9f830e90` | 2026-11-11 12:23 -0600 | 17,904 | 3,270,592 | +0 |  |
| `27994ad045` | 2026-11-11 12:23 -0600 | 17,904 | 3,270,592 | +0 |  |
| `e8520582e6` | 2026-11-11 12:23 -0600 | 17,904 | 3,270,592 | +0 |  |
| `d60e81a681` | 2026-11-11 12:23 -0600 | 17,904 | 3,270,592 | +0 |  |
| `bbcf0f1c08` | 2026-11-11 12:23 -0600 | 17,904 | 3,270,592 | +0 |  |
| `481c5d38dd` | 2026-11-11 13:02 -0600 | 17,904 | 3,270,592 | +0 | 0 |
| `742911e9ea` | 2026-11-11 13:02 -0600 | 17,904 | 3,270,592 | +0 |  |
| `b83737898e` | 2026-11-11 13:02 -0600 | 17,904 | 3,270,592 | +0 | 0 |
| `6f77f76833` | 2026-11-11 13:02 -0600 | 17,904 | 3,270,592 | +0 |  |
| `44eca944e6` | 2026-11-11 13:02 -0600 | 17,904 | 3,270,592 | +0 |  |
| `c78539a90e` | 2026-11-11 13:02 -0600 | 17,904 | 3,270,592 | +0 |  |
| `3f44f198c7` | 2026-11-11 13:02 -0600 | 17,904 | 3,270,592 | +0 |  |
| `2aa74e6043` | 2026-11-11 13:22 -0600 | 17,904 | 3,270,592 | +0 | 0 |
| `7cd194d33d` | 2026-11-11 13:22 -0600 | 17,904 | 3,270,592 | +0 |  |
| `59819903f8` | 2026-11-11 13:22 -0600 | 17,904 | 3,270,592 | +0 |  |
| `e7cd52c954` | 2026-11-11 13:22 -0600 | 17,904 | 3,270,592 | +0 | 0 |
| `4ab9bb4060` | 2026-11-11 13:33 -0600 | 17,904 | 3,270,592 | +0 | 0 |
| `15a33fc07a` | 2026-11-11 13:33 -0600 | 17,904 | 3,270,592 | +0 |  |
| `38faed6861` | 2026-11-11 13:33 -0600 | 17,904 | 3,270,592 | +0 |  |
| `a5653484c3` | 2026-11-11 14:13 -0600 | 17,904 | 3,270,592 | +0 | 0 |
| `e2159f4c92` | 2026-11-11 14:32 -0600 | 17,904 | 3,270,592 | +0 | 0 |
| `f39386a59f` | 2026-11-11 14:53 -0600 | 17,904 | 3,270,592 | +0 | 0 |
| `e450f08e6c` | 2026-11-11 14:56 -0600 | 17,905 | 3,271,575 | +983 | 21,844 |
| `1aa381e6bf` | 2026-11-11 15:53 -0600 | 17,905 | 3,271,575 | +0 | 0 |
| `1105e6ca06` | 2026-11-11 16:01 -0600 | 17,905 | 3,271,575 | +0 | 0 |
| `84752da8e8` | 2026-11-11 17:35 -0600 | 17,905 | 3,271,575 | +0 | 0 |
| `b86f7bf6fb` | 2026-11-11 17:49 -0600 | 17,905 | 3,271,575 | +0 | 0 |
| `7b66240890` | 2026-11-11 17:57 -0600 | 17,905 | 3,271,575 | +0 | 0 |
| `401d927f2b` | 2026-11-11 18:46 -0600 | 17,905 | 3,271,575 | +0 | 0 |
| `d0bb3cd810` | 2026-11-11 19:01 -0600 | 17,905 | 3,271,575 | +0 | 0 |
| `09dfb6518d` | 2026-11-11 19:15 -0600 | 17,905 | 3,271,575 | +0 | 0 |
| `89263278dc` | 2026-11-11 19:38 -0600 | 17,905 | 3,271,575 | +0 | 0 |
| `3a5e362d92` | 2026-11-11 22:18 -0600 | 17,905 | 3,271,575 | +0 | 0 |
| `39d820fc39` | 2026-11-11 22:18 -0600 | 17,905 | 3,271,575 | +0 | 0 |
| `418f6355ac` | 2026-11-11 22:37 -0600 | 17,905 | 3,271,575 | +0 | 0 |
| `82e7323591` | 2026-11-11 22:44 -0600 | 17,905 | 3,271,575 | +0 | 0 |
| `7e564a3c46` | 2026-11-11 22:53 -0600 | 17,905 | 3,271,575 | +0 | 0 |
| `80db6c9cc9` | 2026-11-11 22:53 -0600 | 17,905 | 3,271,575 | +0 | 0 |
| `f74ac16592` | 2026-11-11 22:53 -0600 | 17,905 | 3,271,575 | +0 |  |
| `3ce6dbff81` | 2026-11-11 22:53 -0600 | 17,905 | 3,271,575 | +0 |  |
| `23a3e37973` | 2026-11-11 22:53 -0600 | 17,905 | 3,271,575 | +0 |  |
| `7efc21bebe` | 2026-11-11 23:18 -0600 | 17,905 | 3,271,575 | +0 | 0 |
| `1c19133271` | 2026-11-11 23:18 -0600 | 17,905 | 3,271,575 | +0 |  |
| `73315b4dd8` | 2026-11-11 23:18 -0600 | 17,905 | 3,271,575 | +0 |  |
| `3c751c3285` | 2026-11-11 23:52 -0600 | 17,905 | 3,271,575 | +0 | 0 |
| `4f506864c6` | 2026-11-11 23:52 -0600 | 17,905 | 3,271,575 | +0 |  |
| `dafdfd433d` | 2026-11-11 23:52 -0600 | 17,905 | 3,271,575 | +0 |  |
| `b058ca7cde` | 2026-11-12 00:08 -0600 | 17,905 | 3,271,575 | +0 | 0 |
| `29bd49150a` | 2026-11-12 00:08 -0600 | 17,905 | 3,271,575 | +0 |  |
| `9cd744abf5` | 2026-11-12 00:08 -0600 | 17,905 | 3,271,575 | +0 |  |
| `1e068d3a44` | 2026-11-12 00:08 -0600 | 17,905 | 3,271,575 | +0 | 0 |
| `1b18e77392` | 2026-11-12 00:08 -0600 | 17,905 | 3,271,575 | +0 |  |
| `78d133ea03` | 2026-11-12 00:08 -0600 | 17,905 | 3,271,575 | +0 |  |
| `87a07f2b9a` | 2026-11-12 00:08 -0600 | 17,905 | 3,271,575 | +0 |  |
| `6055b8ffb1` | 2026-11-12 00:30 -0600 | 17,905 | 3,271,575 | +0 | 0 |
| `e61b8fd8fd` | 2026-11-12 00:30 -0600 | 17,905 | 3,271,575 | +0 |  |
| `fa2683c7c7` | 2026-11-12 00:30 -0600 | 17,905 | 3,271,575 | +0 |  |
| `28b847e928` | 2026-11-12 00:39 -0600 | 17,905 | 3,271,575 | +0 | 0 |
| `c51b3b4c81` | 2026-11-12 00:39 -0600 | 17,905 | 3,271,575 | +0 | 0 |
| `2d1d9bae07` | 2026-11-12 00:39 -0600 | 17,905 | 3,271,575 | +0 |  |
| `5df3cbd80d` | 2026-11-12 00:39 -0600 | 17,905 | 3,271,575 | +0 |  |
| `d63cb5f6fa` | 2026-11-12 00:39 -0600 | 17,905 | 3,271,575 | +0 |  |
| `271a055cef` | 2026-11-12 00:39 -0600 | 17,905 | 3,271,575 | +0 |  |
| `0b19f9991a` | 2026-11-12 00:39 -0600 | 17,905 | 3,271,575 | +0 |  |
| `fdcf3ed2f4` | 2026-11-12 00:56 -0600 | 17,905 | 3,271,575 | +0 | 0 |
| `061c95beb7` | 2026-11-12 00:56 -0600 | 17,905 | 3,271,575 | +0 |  |
| `9ac88fa6e9` | 2026-11-12 01:19 -0600 | 17,905 | 3,271,575 | +0 | 0 |
| `90cf9b4a1d` | 2026-11-12 01:19 -0600 | 17,905 | 3,271,575 | +0 |  |
| `7e4013fa1d` | 2026-11-12 01:19 -0600 | 17,905 | 3,271,575 | +0 |  |
| `f374db1571` | 2026-11-12 01:19 -0600 | 17,905 | 3,271,575 | +0 |  |
| `1e5bbd8d0d` | 2026-11-12 01:19 -0600 | 17,905 | 3,271,575 | +0 |  |
| `a0cc276211` | 2026-11-12 01:19 -0600 | 17,905 | 3,271,575 | +0 |  |
| `ef68aa9234` | 2026-11-12 03:07 -0600 | 17,905 | 3,271,575 | +0 | 0 |
| `f37938f477` | 2026-11-12 03:07 -0600 | 17,905 | 3,271,575 | +0 | 0 |
| `67bfb8ec88` | 2026-11-12 03:07 -0600 | 17,905 | 3,271,575 | +0 |  |
| `215b394750` | 2026-11-12 03:07 -0600 | 17,905 | 3,271,575 | +0 |  |
| `161f4ca1f1` | 2026-11-12 03:07 -0600 | 17,905 | 3,271,575 | +0 |  |
| `b4b93cb973` | 2026-11-12 03:07 -0600 | 17,905 | 3,271,575 | +0 |  |
| `a74b68f20c` | 2026-11-12 03:07 -0600 | 17,905 | 3,271,575 | +0 |  |
| `c9375d1cc9` | 2026-11-12 03:07 -0600 | 17,905 | 3,271,575 | +0 |  |
| `59cc4aa2ea` | 2026-11-12 03:07 -0600 | 17,905 | 3,271,575 | +0 |  |
| `85a1536055` | 2026-11-12 03:07 -0600 | 17,905 | 3,271,575 | +0 |  |
| `2b55d3a611` | 2026-11-12 03:07 -0600 | 17,905 | 3,271,575 | +0 |  |
| `804273165f` | 2026-11-12 03:07 -0600 | 17,905 | 3,271,575 | +0 |  |
| `010fe92413` | 2026-11-12 03:07 -0600 | 17,905 | 3,271,575 | +0 |  |
| `b951f644e0` | 2026-11-12 03:07 -0600 | 17,905 | 3,271,575 | +0 |  |
| `f1b17e0cd1` | 2026-11-12 03:07 -0600 | 17,905 | 3,271,575 | +0 |  |
| `9efca93018` | 2026-11-12 03:07 -0600 | 17,905 | 3,271,575 | +0 | 0 |
| `1df01c92d1` | 2026-11-12 03:07 -0600 | 17,905 | 3,271,575 | +0 |  |
| `1c077f6244` | 2026-11-12 03:07 -0600 | 17,905 | 3,271,575 | +0 |  |
| `76f683f969` | 2026-11-12 03:07 -0600 | 17,905 | 3,271,575 | +0 |  |
| `6e0ee83422` | 2026-11-12 03:07 -0600 | 17,905 | 3,271,575 | +0 |  |
| `da0115461a` | 2026-11-12 03:07 -0600 | 17,905 | 3,271,575 | +0 |  |
| `27292d207d` | 2026-11-12 03:07 -0600 | 17,905 | 3,271,575 | +0 |  |
| `7cfea35b32` | 2026-11-12 03:07 -0600 | 17,905 | 3,271,575 | +0 |  |
| `1984c0ecbd` | 2026-11-12 03:07 -0600 | 17,905 | 3,271,575 | +0 |  |
| `fca42354e4` | 2026-11-12 03:07 -0600 | 17,905 | 3,271,575 | +0 |  |
| `5c23c0e34f` | 2026-11-12 03:07 -0600 | 17,905 | 3,271,575 | +0 |  |
| `0d9345bfa2` | 2026-11-12 03:07 -0600 | 17,905 | 3,271,575 | +0 |  |
| `4e7ceed1fb` | 2026-11-12 03:07 -0600 | 17,905 | 3,271,575 | +0 |  |
| `faf638d5e3` | 2026-11-12 03:07 -0600 | 17,905 | 3,271,575 | +0 |  |
| `63ad1554d6` | 2026-11-12 03:07 -0600 | 17,905 | 3,271,575 | +0 |  |
| `b9d4e8b738` | 2026-11-12 03:07 -0600 | 17,905 | 3,271,575 | +0 |  |
| `bc6c0b0d48` | 2026-11-12 03:07 -0600 | 17,905 | 3,271,575 | +0 | 0 |
| `48038630d2` | 2026-11-12 03:07 -0600 | 17,905 | 3,271,575 | +0 |  |
| `6131306b96` | 2026-11-12 03:07 -0600 | 17,905 | 3,271,575 | +0 |  |
| `a57ae45409` | 2026-11-12 03:07 -0600 | 17,905 | 3,271,575 | +0 |  |
| `0f61f71cf7` | 2026-11-12 03:32 -0600 | 17,905 | 3,271,575 | +0 | 0 |
| `9dd8b56e7c` | 2026-11-12 03:32 -0600 | 17,905 | 3,271,575 | +0 |  |
| `d7a63418b5` | 2026-11-12 03:32 -0600 | 17,905 | 3,271,575 | +0 |  |
| `5e7dee630e` | 2026-11-12 03:32 -0600 | 17,905 | 3,271,575 | +0 |  |
| `416f45f156` | 2026-11-12 03:32 -0600 | 17,905 | 3,271,575 | +0 |  |
| `3ecf5949ce` | 2026-11-12 03:32 -0600 | 17,905 | 3,271,575 | +0 |  |
| `b243897245` | 2026-11-12 03:32 -0600 | 17,905 | 3,271,575 | +0 |  |
| `6680620d66` | 2026-11-12 03:32 -0600 | 17,905 | 3,271,575 | +0 |  |
| `45192b0423` | 2026-11-12 03:32 -0600 | 17,905 | 3,271,575 | +0 |  |
| `2d2c44e60f` | 2026-11-12 03:32 -0600 | 17,905 | 3,271,575 | +0 |  |
| `0127b83abd` | 2026-11-12 03:32 -0600 | 17,905 | 3,271,575 | +0 |  |
| `072cb4b113` | 2026-11-12 03:32 -0600 | 17,905 | 3,271,575 | +0 |  |
| `2e43cebd2a` | 2026-11-12 03:32 -0600 | 17,905 | 3,271,575 | +0 |  |
| `42b266bd0e` | 2026-11-12 03:32 -0600 | 17,905 | 3,271,575 | +0 | 0 |
| `e194d0774e` | 2026-11-12 03:32 -0600 | 17,905 | 3,271,575 | +0 |  |
| `73f5e85d88` | 2026-11-12 03:32 -0600 | 17,905 | 3,271,575 | +0 |  |
| `4cb71ccac7` | 2026-11-12 03:32 -0600 | 17,905 | 3,271,575 | +0 |  |
| `62a59d61c2` | 2026-11-12 03:32 -0600 | 17,905 | 3,271,575 | +0 |  |
| `6472e5d429` | 2026-11-12 03:32 -0600 | 17,905 | 3,271,575 | +0 |  |
| `2c565ee5d1` | 2026-11-12 03:32 -0600 | 17,905 | 3,271,575 | +0 |  |
| `90ac588609` | 2026-11-12 03:32 -0600 | 17,905 | 3,271,575 | +0 |  |
| `3b04e4477b` | 2026-11-12 03:32 -0600 | 17,905 | 3,271,575 | +0 |  |
| `e18b751973` | 2026-11-12 03:32 -0600 | 17,905 | 3,271,575 | +0 |  |
| `9324c7c62f` | 2026-11-12 03:32 -0600 | 17,905 | 3,271,575 | +0 |  |
| `96f98cb85e` | 2026-11-12 03:32 -0600 | 17,905 | 3,271,575 | +0 |  |
| `ec002152fe` | 2026-11-12 03:32 -0600 | 17,905 | 3,271,575 | +0 |  |
| `24390fee73` | 2026-11-12 03:32 -0600 | 17,905 | 3,271,575 | +0 |  |
| `94ada538e8` | 2026-11-12 03:32 -0600 | 17,905 | 3,271,575 | +0 |  |
| `ef7907ec1e` | 2026-11-12 03:32 -0600 | 17,905 | 3,271,575 | +0 | 0 |
| `55af835e6f` | 2026-11-12 03:32 -0600 | 17,905 | 3,271,575 | +0 |  |
| `523e145246` | 2026-11-12 03:32 -0600 | 17,905 | 3,271,575 | +0 |  |
| `19f8224721` | 2026-11-12 03:32 -0600 | 17,905 | 3,271,575 | +0 |  |
| `7af91b7e0a` | 2026-11-12 03:32 -0600 | 17,905 | 3,271,575 | +0 |  |
| `b37ce026ba` | 2026-11-12 03:32 -0600 | 17,905 | 3,271,575 | +0 |  |
| `587fbce9b4` | 2026-11-12 03:32 -0600 | 17,905 | 3,271,575 | +0 |  |
| `ba65524e2b` | 2026-11-12 03:32 -0600 | 17,905 | 3,271,575 | +0 |  |
| `465f0ecd68` | 2026-11-12 03:32 -0600 | 17,905 | 3,271,575 | +0 |  |
| `af47e6c1ee` | 2026-11-12 03:32 -0600 | 17,905 | 3,271,575 | +0 |  |
| `751250128a` | 2026-11-12 03:32 -0600 | 17,905 | 3,271,575 | +0 |  |
| `fc73e4ccd4` | 2026-11-12 03:32 -0600 | 17,905 | 3,271,575 | +0 |  |
| `3fee14a8c0` | 2026-11-12 03:32 -0600 | 17,905 | 3,271,575 | +0 |  |
| `9e67fb64a4` | 2026-11-12 03:32 -0600 | 17,905 | 3,271,575 | +0 |  |
| `a2da7d89d3` | 2026-11-12 03:32 -0600 | 17,905 | 3,271,575 | +0 |  |
| `519a94dbd1` | 2026-11-12 03:32 -0600 | 17,905 | 3,271,575 | +0 |  |
| `d13a2bbf24` | 2026-11-12 03:32 -0600 | 17,905 | 3,271,575 | +0 | 0 |
| `a46d2d3bc7` | 2026-11-12 03:32 -0600 | 17,905 | 3,271,575 | +0 |  |
| `737eb5e282` | 2026-11-12 03:32 -0600 | 17,905 | 3,271,575 | +0 |  |
| `0ae6f1fe00` | 2026-11-12 03:32 -0600 | 17,905 | 3,271,575 | +0 |  |
| `be1817e34b` | 2026-11-12 03:32 -0600 | 17,905 | 3,271,575 | +0 |  |
| `2e5fa05c8f` | 2026-11-12 03:32 -0600 | 17,905 | 3,271,575 | +0 |  |
| `2950b79997` | 2026-11-12 03:32 -0600 | 17,905 | 3,271,575 | +0 |  |
| `bde236147f` | 2026-11-12 03:32 -0600 | 17,905 | 3,271,575 | +0 |  |
| `60875c2ca8` | 2026-11-12 03:51 -0600 | 17,905 | 3,271,575 | +0 | 0 |
| `55d08ee121` | 2026-11-12 03:51 -0600 | 17,905 | 3,271,575 | +0 |  |
| `1b3a50b3ec` | 2026-11-12 03:51 -0600 | 17,905 | 3,271,575 | +0 |  |
| `46c8259f6b` | 2026-11-12 03:51 -0600 | 17,905 | 3,271,575 | +0 |  |
| `0c7479e4e4` | 2026-11-12 03:51 -0600 | 17,905 | 3,271,575 | +0 |  |
| `c394d95c1b` | 2026-11-12 03:51 -0600 | 17,905 | 3,271,575 | +0 |  |
| `4a7191d9d6` | 2026-11-12 03:51 -0600 | 17,905 | 3,271,575 | +0 |  |
| `4bafebdade` | 2026-11-12 03:51 -0600 | 17,905 | 3,271,575 | +0 |  |
| `7f861d2c35` | 2026-11-12 03:51 -0600 | 17,905 | 3,271,575 | +0 |  |
| `4f2747bd71` | 2026-11-12 03:51 -0600 | 17,905 | 3,271,575 | +0 |  |
| `5f05019053` | 2026-11-12 04:03 -0600 | 17,905 | 3,271,575 | +0 | 0 |
| `067c8ebb2e` | 2026-11-12 04:03 -0600 | 17,905 | 3,271,575 | +0 |  |
| `271f34e1d6` | 2026-11-12 04:03 -0600 | 17,905 | 3,271,575 | +0 |  |
| `fc38b35e68` | 2026-11-12 04:03 -0600 | 17,905 | 3,271,575 | +0 |  |
| `6bac69b37a` | 2026-11-12 04:03 -0600 | 17,905 | 3,271,575 | +0 |  |
| `929e37a760` | 2026-11-12 04:03 -0600 | 17,905 | 3,271,575 | +0 |  |
| `5842cceb25` | 2026-11-12 04:31 -0600 | 17,905 | 3,271,575 | +0 | 0 |
| `35270f6d99` | 2026-11-12 04:31 -0600 | 17,905 | 3,271,575 | +0 |  |
| `5004a52c7e` | 2026-11-12 04:31 -0600 | 17,905 | 3,271,575 | +0 | 0 |
| `1a2e016d05` | 2026-11-12 04:31 -0600 | 17,905 | 3,271,575 | +0 |  |
| `f1ecfe50fd` | 2026-11-12 04:31 -0600 | 17,905 | 3,271,575 | +0 |  |
| `e7caeefb87` | 2026-11-12 04:31 -0600 | 17,905 | 3,271,575 | +0 |  |
| `f39462801d` | 2026-11-12 04:31 -0600 | 17,905 | 3,271,575 | +0 |  |
| `9bd1ff6b22` | 2026-11-12 04:31 -0600 | 17,905 | 3,271,575 | +0 |  |
| `ee4aad602a` | 2026-11-12 04:31 -0600 | 17,905 | 3,271,575 | +0 |  |
| `474a7603fd` | 2026-11-12 04:31 -0600 | 17,905 | 3,271,575 | +0 |  |
| `5f1b9fbdef` | 2026-11-12 04:59 -0600 | 17,905 | 3,271,575 | +0 | 0 |
| `789d5bea53` | 2026-11-12 04:59 -0600 | 17,905 | 3,271,575 | +0 |  |
| `5ad136b735` | 2026-11-12 04:59 -0600 | 17,905 | 3,271,575 | +0 |  |
| `98f3bd16d8` | 2026-11-12 04:59 -0600 | 17,905 | 3,271,575 | +0 |  |
| `f446d9498f` | 2026-11-12 04:59 -0600 | 17,905 | 3,271,575 | +0 |  |
| `67ddf7cb24` | 2026-11-12 04:59 -0600 | 17,905 | 3,271,575 | +0 |  |
| `d3067037df` | 2026-11-12 04:59 -0600 | 17,905 | 3,271,575 | +0 |  |
| `eb7f0cb2f9` | 2026-11-12 04:59 -0600 | 17,905 | 3,271,575 | +0 |  |
| `e7ef159c35` | 2026-11-12 04:59 -0600 | 17,905 | 3,271,575 | +0 |  |
| `eb28d1ec4d` | 2026-11-12 04:59 -0600 | 17,905 | 3,271,575 | +0 | 0 |
| `d194597cf0` | 2026-11-12 04:59 -0600 | 17,905 | 3,271,575 | +0 |  |
| `7c11d77f50` | 2026-11-12 04:59 -0600 | 17,905 | 3,271,575 | +0 |  |
| `cde2b70439` | 2026-11-12 04:59 -0600 | 17,905 | 3,271,575 | +0 |  |
| `e1a02895a0` | 2026-11-12 04:59 -0600 | 17,905 | 3,271,575 | +0 |  |
| `259a6c6e54` | 2026-11-12 04:59 -0600 | 17,905 | 3,271,575 | +0 |  |
| `4581567865` | 2026-11-12 04:59 -0600 | 17,905 | 3,271,575 | +0 |  |
| `dc9683e414` | 2026-11-12 04:59 -0600 | 17,905 | 3,271,575 | +0 |  |
| `2f8aa3c0ff` | 2026-11-12 04:59 -0600 | 17,905 | 3,271,575 | +0 |  |
| `acf48076d8` | 2026-11-12 04:59 -0600 | 17,905 | 3,271,575 | +0 |  |
| `b675a04ac1` | 2026-11-12 04:59 -0600 | 17,905 | 3,271,575 | +0 |  |
| `3e201e9f38` | 2026-11-12 04:59 -0600 | 17,905 | 3,271,575 | +0 |  |
| `d60426dc1a` | 2026-11-12 04:59 -0600 | 17,905 | 3,271,575 | +0 |  |
| `27b8e75256` | 2026-11-12 04:59 -0600 | 17,905 | 3,271,575 | +0 |  |
| `018359a900` | 2026-11-12 04:59 -0600 | 17,905 | 3,271,575 | +0 |  |
| `ed6743c184` | 2026-11-12 04:59 -0600 | 17,905 | 3,271,575 | +0 |  |
| `ffc39ae8c8` | 2026-11-12 04:59 -0600 | 17,905 | 3,271,575 | +0 |  |
| `11d999a9a9` | 2026-11-12 04:59 -0600 | 17,905 | 3,271,575 | +0 | 0 |
| `11a18fe41f` | 2026-11-12 04:59 -0600 | 17,905 | 3,271,575 | +0 |  |
| `7c3f398cbd` | 2026-11-12 05:16 -0600 | 17,905 | 3,271,575 | +0 | 0 |
| `2b6636c4f7` | 2026-11-12 05:16 -0600 | 17,905 | 3,271,575 | +0 |  |
| `79f35dfb6c` | 2026-11-12 05:16 -0600 | 17,905 | 3,271,575 | +0 |  |
| `f7c91d4d33` | 2026-11-12 05:16 -0600 | 17,905 | 3,271,575 | +0 |  |
| `d589a9e44c` | 2026-11-12 05:16 -0600 | 17,905 | 3,271,575 | +0 |  |
| `5e039bcb20` | 2026-11-12 05:16 -0600 | 17,905 | 3,271,575 | +0 |  |
| `8bb1f0e910` | 2026-11-12 05:16 -0600 | 17,905 | 3,271,575 | +0 |  |
| `fb4b0ec0fe` | 2026-11-12 05:16 -0600 | 17,905 | 3,271,575 | +0 |  |
| `a1cfae4274` | 2026-11-12 05:16 -0600 | 17,905 | 3,271,575 | +0 |  |
| `5494f53990` | 2026-11-12 05:16 -0600 | 17,905 | 3,271,575 | +0 |  |
| `e122cc3567` | 2026-11-12 05:16 -0600 | 17,905 | 3,271,575 | +0 |  |
| `7d14fed671` | 2026-11-12 05:16 -0600 | 17,905 | 3,271,575 | +0 |  |
| `f6997d4bcd` | 2026-11-12 05:16 -0600 | 17,905 | 3,271,575 | +0 |  |
| `de829d2aee` | 2026-11-12 05:16 -0600 | 17,905 | 3,271,575 | +0 |  |
| `6390715a14` | 2026-11-12 05:16 -0600 | 17,905 | 3,271,575 | +0 |  |
| `e7bfaa3e37` | 2026-11-12 05:29 -0600 | 17,905 | 3,271,575 | +0 | 0 |
| `7fcae77daf` | 2026-11-12 05:29 -0600 | 17,905 | 3,271,575 | +0 |  |
| `ae89eec1eb` | 2026-11-12 05:29 -0600 | 17,905 | 3,271,575 | +0 |  |
| `f9fbfbea04` | 2026-11-12 05:29 -0600 | 17,905 | 3,271,575 | +0 |  |
| `7ee873579c` | 2026-11-12 05:29 -0600 | 17,905 | 3,271,575 | +0 |  |
| `8573cbaaf4` | 2026-11-12 05:29 -0600 | 17,905 | 3,271,575 | +0 |  |
| `52393968bc` | 2026-11-12 05:59 -0600 | 17,905 | 3,271,575 | +0 | 0 |
| `f37a3c8b93` | 2026-11-12 05:59 -0600 | 17,905 | 3,271,575 | +0 |  |
| `58b776f859` | 2026-11-12 05:59 -0600 | 17,905 | 3,271,575 | +0 |  |
| `519fa8c177` | 2026-11-12 05:59 -0600 | 17,905 | 3,271,575 | +0 |  |
| `e3c549fa4e` | 2026-11-12 05:59 -0600 | 17,905 | 3,271,575 | +0 | 0 |
| `c30a6e086e` | 2026-11-12 05:59 -0600 | 17,905 | 3,271,575 | +0 |  |
| `4583fc7253` | 2026-11-12 05:59 -0600 | 17,905 | 3,271,575 | +0 |  |
| `45d578200e` | 2026-11-12 05:59 -0600 | 17,905 | 3,271,575 | +0 |  |
| `b838d9b5e4` | 2026-11-12 05:59 -0600 | 17,905 | 3,271,575 | +0 |  |
| `82feb551f0` | 2026-11-12 05:59 -0600 | 17,905 | 3,271,575 | +0 |  |
| `12d93104bd` | 2026-11-12 06:15 -0600 | 17,905 | 3,271,575 | +0 | 0 |
| `8e24a8c8db` | 2026-11-12 06:15 -0600 | 17,905 | 3,271,575 | +0 |  |
| `06c6261dc6` | 2026-11-12 06:15 -0600 | 17,905 | 3,271,575 | +0 |  |
| `d0411133c5` | 2026-11-12 06:15 -0600 | 17,905 | 3,271,575 | +0 |  |
| `0cddd76cd5` | 2026-11-12 06:15 -0600 | 17,905 | 3,271,575 | +0 |  |
| `2549f1d314` | 2026-11-12 06:33 -0600 | 17,905 | 3,271,575 | +0 | 0 |
| `08db1e8665` | 2026-11-12 06:33 -0600 | 17,905 | 3,271,575 | +0 | 0 |
| `2bc7e6e85e` | 2026-11-12 06:33 -0600 | 17,905 | 3,271,575 | +0 |  |
| `0f35f23ff5` | 2026-11-12 06:33 -0600 | 17,905 | 3,271,575 | +0 |  |
| `7e5a614316` | 2026-11-12 06:33 -0600 | 17,905 | 3,271,575 | +0 |  |
| `66d6d379a2` | 2026-11-12 06:48 -0600 | 17,905 | 3,271,575 | +0 | 0 |
| `428cdf3a06` | 2026-11-12 06:48 -0600 | 17,905 | 3,271,575 | +0 |  |
| `59148cd0f9` | 2026-11-12 06:48 -0600 | 17,905 | 3,271,575 | +0 |  |
| `b9362b552c` | 2026-11-12 06:48 -0600 | 17,905 | 3,271,575 | +0 |  |
| `082bd374a5` | 2026-11-12 06:48 -0600 | 17,905 | 3,271,575 | +0 |  |
| `70d7216170` | 2026-11-12 06:48 -0600 | 17,905 | 3,271,575 | +0 |  |
| `5acfaab489` | 2026-11-12 06:48 -0600 | 17,905 | 3,271,575 | +0 |  |
| `fbb7f1c7d2` | 2026-11-12 06:48 -0600 | 17,905 | 3,271,575 | +0 |  |
| `518878ac2c` | 2026-11-12 06:48 -0600 | 17,905 | 3,271,575 | +0 |  |
| `d1fbcd33dc` | 2026-11-12 06:48 -0600 | 17,905 | 3,271,575 | +0 |  |
| `7df4611617` | 2026-11-12 06:48 -0600 | 17,905 | 3,271,575 | +0 |  |
| `a39e3d441c` | 2026-11-12 06:48 -0600 | 17,905 | 3,271,575 | +0 |  |
| `444df63e88` | 2026-11-12 06:48 -0600 | 17,905 | 3,271,575 | +0 |  |
| `d95ac8a7bb` | 2026-11-12 06:48 -0600 | 17,905 | 3,271,575 | +0 |  |
| `2dd0300678` | 2026-11-12 06:48 -0600 | 17,905 | 3,271,575 | +0 | 0 |
| `ad5f77cb25` | 2026-11-12 06:48 -0600 | 17,905 | 3,271,575 | +0 |  |
| `479f96dc9a` | 2026-11-12 06:48 -0600 | 17,905 | 3,271,575 | +0 |  |
| `edd13e3d26` | 2026-11-12 06:48 -0600 | 17,905 | 3,271,575 | +0 |  |
| `144652db85` | 2026-11-12 06:48 -0600 | 17,905 | 3,271,575 | +0 |  |
| `74642df8c3` | 2026-11-12 06:48 -0600 | 17,905 | 3,271,575 | +0 |  |
| `0d503b6385` | 2026-11-12 06:48 -0600 | 17,905 | 3,271,575 | +0 |  |
| `dc461e3dce` | 2026-11-12 06:48 -0600 | 17,905 | 3,271,575 | +0 |  |
| `afbe6cd753` | 2026-11-12 06:48 -0600 | 17,905 | 3,271,575 | +0 |  |
| `e26d4a3fac` | 2026-11-12 06:48 -0600 | 17,905 | 3,271,575 | +0 |  |
| `07de22155b` | 2026-11-12 06:48 -0600 | 17,905 | 3,271,575 | +0 |  |
| `0a2cf95821` | 2026-11-12 06:48 -0600 | 17,905 | 3,271,575 | +0 |  |
| `b45099c9ae` | 2026-11-12 06:48 -0600 | 17,905 | 3,271,575 | +0 |  |
| `5ecb90f221` | 2026-11-12 06:48 -0600 | 17,905 | 3,271,575 | +0 |  |
| `5e2f733825` | 2026-11-12 06:48 -0600 | 17,905 | 3,271,575 | +0 |  |
| `a1509f5109` | 2026-11-12 06:48 -0600 | 17,905 | 3,271,575 | +0 |  |
| `1c72fc4406` | 2026-11-12 06:48 -0600 | 17,905 | 3,271,575 | +0 |  |
| `3fccc2f300` | 2026-11-12 06:48 -0600 | 17,905 | 3,271,575 | +0 |  |
| `66a396561a` | 2026-11-12 06:48 -0600 | 17,905 | 3,271,575 | +0 |  |
| `9fe40e46de` | 2026-11-12 06:48 -0600 | 17,905 | 3,271,575 | +0 |  |
| `233a6c824f` | 2026-11-12 06:48 -0600 | 17,905 | 3,271,575 | +0 |  |
| `069e5bbe10` | 2026-11-12 06:48 -0600 | 17,905 | 3,271,575 | +0 |  |
| `1f85cfe7b9` | 2026-11-12 06:48 -0600 | 17,905 | 3,271,575 | +0 | 0 |
| `39b9452fdc` | 2026-11-12 06:48 -0600 | 17,905 | 3,271,575 | +0 |  |
| `c146cc427b` | 2026-11-12 06:48 -0600 | 17,905 | 3,271,575 | +0 |  |
| `14670f4e77` | 2026-11-12 06:48 -0600 | 17,905 | 3,271,575 | +0 |  |
| `ffb40b377d` | 2026-11-12 06:48 -0600 | 17,905 | 3,271,575 | +0 |  |
| `460c755a8e` | 2026-11-12 06:48 -0600 | 17,905 | 3,271,575 | +0 |  |
| `87d1dafec1` | 2026-11-12 06:48 -0600 | 17,905 | 3,271,575 | +0 |  |
| `54711fd21a` | 2026-11-12 06:48 -0600 | 17,905 | 3,271,575 | +0 |  |
| `870b980a89` | 2026-11-12 06:48 -0600 | 17,905 | 3,271,575 | +0 |  |
| `d002db2d64` | 2026-11-12 06:48 -0600 | 17,905 | 3,271,575 | +0 |  |
| `03c86d38f3` | 2026-11-12 06:48 -0600 | 17,905 | 3,271,575 | +0 |  |
| `56bd3b4605` | 2026-11-12 06:48 -0600 | 17,905 | 3,271,575 | +0 |  |
| `7dc3e424ff` | 2026-11-12 07:08 -0600 | 17,905 | 3,271,575 | +0 | 0 |
| `cd7a3c86ba` | 2026-11-12 07:08 -0600 | 17,905 | 3,271,575 | +0 |  |
| `e03b3ecc24` | 2026-11-12 07:08 -0600 | 17,905 | 3,271,575 | +0 |  |
| `b34b9efca4` | 2026-11-12 07:08 -0600 | 17,905 | 3,271,575 | +0 |  |
| `b9409e75d9` | 2026-11-12 07:08 -0600 | 17,905 | 3,271,575 | +0 |  |
| `6215056be3` | 2026-11-12 07:08 -0600 | 17,905 | 3,271,575 | +0 |  |
| `b5fed89d78` | 2026-11-12 07:08 -0600 | 17,905 | 3,271,575 | +0 |  |
| `82e681659e` | 2026-11-12 07:08 -0600 | 17,905 | 3,271,575 | +0 |  |
| `c89ae0b716` | 2026-11-12 07:08 -0600 | 17,905 | 3,271,575 | +0 |  |
| `6a6a10f024` | 2026-11-12 07:08 -0600 | 17,905 | 3,271,575 | +0 |  |
| `22b5054668` | 2026-11-12 07:08 -0600 | 17,905 | 3,271,575 | +0 |  |
| `5789d7cc75` | 2026-11-12 07:30 -0600 | 17,905 | 3,271,575 | +0 | 0 |
| `3c770c542e` | 2026-11-12 07:30 -0600 | 17,905 | 3,271,575 | +0 |  |
| `38d7e1e12f` | 2026-11-12 07:30 -0600 | 17,905 | 3,271,575 | +0 |  |
| `e00af66c5b` | 2026-11-12 07:30 -0600 | 17,905 | 3,271,575 | +0 |  |
| `41bd596888` | 2026-11-12 07:30 -0600 | 17,905 | 3,271,575 | +0 |  |
| `f07e163cbc` | 2026-11-12 07:30 -0600 | 17,905 | 3,271,575 | +0 |  |
| `f8c95f961b` | 2026-11-12 07:30 -0600 | 17,905 | 3,271,575 | +0 |  |
| `5db987fea6` | 2026-11-12 07:30 -0600 | 17,905 | 3,271,575 | +0 |  |
| `1a15765993` | 2026-11-12 07:30 -0600 | 17,905 | 3,271,575 | +0 |  |
| `e5b419e92e` | 2026-11-12 07:30 -0600 | 17,905 | 3,271,575 | +0 | 0 |
| `29d3817699` | 2026-11-12 07:30 -0600 | 17,905 | 3,271,575 | +0 |  |
| `5fcb00f9f4` | 2026-11-12 07:30 -0600 | 17,905 | 3,271,575 | +0 |  |
| `1301ad500d` | 2026-11-12 07:30 -0600 | 17,905 | 3,271,575 | +0 |  |
| `e9557ba847` | 2026-11-12 07:42 -0600 | 17,905 | 3,271,575 | +0 | 0 |
| `5af289f990` | 2026-11-12 07:42 -0600 | 17,905 | 3,271,575 | +0 | 0 |
| `dfe5543cd9` | 2026-11-12 07:42 -0600 | 17,905 | 3,271,575 | +0 |  |
| `f96dc8ebac` | 2026-11-12 07:42 -0600 | 17,905 | 3,271,575 | +0 |  |
| `b8c53bdc91` | 2026-11-12 07:42 -0600 | 17,905 | 3,271,575 | +0 |  |
| `0fd3133441` | 2026-11-12 07:42 -0600 | 17,905 | 3,271,575 | +0 |  |
| `3feea4835f` | 2026-11-12 07:42 -0600 | 17,905 | 3,271,575 | +0 |  |
| `2efb14572c` | 2026-11-12 07:42 -0600 | 17,905 | 3,271,575 | +0 |  |
| `62c74e5e77` | 2026-11-12 07:42 -0600 | 17,905 | 3,271,575 | +0 |  |
| `e4cb4e8d90` | 2026-11-12 07:42 -0600 | 17,905 | 3,271,575 | +0 |  |
| `75895e278e` | 2026-11-12 07:42 -0600 | 17,905 | 3,271,575 | +0 |  |
| `4b3c2aafa5` | 2026-11-12 07:57 -0600 | 17,905 | 3,271,575 | +0 | 0 |
| `ea114b4b82` | 2026-11-12 07:57 -0600 | 17,905 | 3,271,575 | +0 |  |
| `06a90b4d74` | 2026-11-12 07:57 -0600 | 17,905 | 3,271,575 | +0 |  |
| `4e8fdbb7f5` | 2026-11-12 07:57 -0600 | 17,905 | 3,271,575 | +0 |  |
| `82d2b7f1cc` | 2026-11-12 07:57 -0600 | 17,905 | 3,271,575 | +0 |  |
| `1718aa64f8` | 2026-11-12 08:06 -0600 | 17,905 | 3,271,575 | +0 | 0 |
| `cb3747723f` | 2026-11-12 08:06 -0600 | 17,905 | 3,271,575 | +0 |  |
| `44bcb8f5fd` | 2026-11-12 08:06 -0600 | 17,905 | 3,271,575 | +0 |  |
| `9c521bf46d` | 2026-11-12 08:06 -0600 | 17,905 | 3,271,575 | +0 |  |
| `dfef4fc88b` | 2026-11-12 08:06 -0600 | 17,905 | 3,271,575 | +0 |  |
| `10b3a1ed42` | 2026-11-12 08:06 -0600 | 17,905 | 3,271,575 | +0 |  |
| `4570fe60fd` | 2026-11-12 08:06 -0600 | 17,905 | 3,271,575 | +0 | 0 |
| `14a3011de8` | 2026-11-12 08:06 -0600 | 17,905 | 3,271,575 | +0 |  |
| `80c590fcd8` | 2026-11-12 08:06 -0600 | 17,905 | 3,271,575 | +0 |  |
| `61af9984b1` | 2026-11-12 08:06 -0600 | 17,905 | 3,271,575 | +0 |  |
| `df0a5cd57e` | 2026-11-12 08:06 -0600 | 17,905 | 3,271,575 | +0 |  |
| `ce313114e6` | 2026-11-12 08:06 -0600 | 17,905 | 3,271,575 | +0 |  |
| `c642385b84` | 2026-11-12 08:17 -0600 | 17,905 | 3,271,575 | +0 | 0 |
| `258313bcb7` | 2026-11-12 08:17 -0600 | 17,905 | 3,271,575 | +0 |  |
| `7a0edb628b` | 2026-11-12 08:17 -0600 | 17,905 | 3,271,575 | +0 |  |
| `df0ed5d03d` | 2026-11-12 08:17 -0600 | 17,905 | 3,271,575 | +0 |  |
| `0093dd9d81` | 2026-11-12 08:17 -0600 | 17,905 | 3,271,575 | +0 |  |
| `58fc8b8e24` | 2026-11-12 08:31 -0600 | 17,905 | 3,271,575 | +0 | 0 |
| `a9f9d4b946` | 2026-11-12 08:31 -0600 | 17,905 | 3,271,575 | +0 |  |
| `085f88d429` | 2026-11-12 08:31 -0600 | 17,905 | 3,271,575 | +0 |  |
| `6a26f74133` | 2026-11-12 08:31 -0600 | 17,905 | 3,271,575 | +0 |  |
| `9448973756` | 2026-11-12 08:31 -0600 | 17,905 | 3,271,575 | +0 |  |
| `9cc12d21b3` | 2026-11-12 08:40 -0600 | 17,905 | 3,271,575 | +0 | 0 |
| `96a68f4533` | 2026-11-12 08:40 -0600 | 17,905 | 3,271,575 | +0 | 0 |
| `27efaac8dc` | 2026-11-12 08:40 -0600 | 17,905 | 3,271,575 | +0 |  |
| `1efa943aeb` | 2026-11-12 08:40 -0600 | 17,905 | 3,271,575 | +0 |  |
| `6f3d6c15d2` | 2026-11-12 08:40 -0600 | 17,905 | 3,271,575 | +0 |  |
| `7e44306382` | 2026-11-12 08:40 -0600 | 17,905 | 3,271,575 | +0 |  |
| `3f0bf34b4d` | 2026-11-12 08:40 -0600 | 17,905 | 3,271,575 | +0 |  |
| `9a66be5595` | 2026-11-12 08:40 -0600 | 17,905 | 3,271,575 | +0 |  |
| `d1c3c17b09` | 2026-11-12 08:40 -0600 | 17,905 | 3,271,575 | +0 |  |
| `45cfe678b6` | 2026-11-12 08:40 -0600 | 17,905 | 3,271,575 | +0 |  |
| `89d57302d7` | 2026-11-12 08:40 -0600 | 17,905 | 3,271,575 | +0 | 0 |
| `fb3826137d` | 2026-11-12 08:40 -0600 | 17,905 | 3,271,575 | +0 |  |
| `574354a7e7` | 2026-11-12 08:55 -0600 | 17,905 | 3,271,575 | +0 | 0 |
| `d43eebd932` | 2026-11-12 08:55 -0600 | 17,905 | 3,271,575 | +0 |  |
| `213973feb9` | 2026-11-12 08:55 -0600 | 17,905 | 3,271,575 | +0 |  |
| `7181f2f5aa` | 2026-11-12 08:55 -0600 | 17,905 | 3,271,575 | +0 |  |
| `903b0d10fc` | 2026-11-12 08:55 -0600 | 17,905 | 3,271,575 | +0 |  |
| `ff16e53841` | 2026-11-12 09:03 -0600 | 17,905 | 3,271,575 | +0 | 0 |
| `a1429777ca` | 2026-11-12 09:03 -0600 | 17,905 | 3,271,575 | +0 |  |
| `01454a9359` | 2026-11-12 09:04 -0600 | 17,905 | 3,271,575 | +0 | 0 |
| `8d037c9a79` | 2026-11-12 09:04 -0600 | 17,905 | 3,271,575 | +0 |  |
| `65ab5f422f` | 2026-11-12 09:04 -0600 | 17,905 | 3,271,575 | +0 |  |
| `4c780f6918` | 2026-11-12 09:04 -0600 | 17,905 | 3,271,575 | +0 |  |
| `6581a0631a` | 2026-11-12 09:04 -0600 | 17,905 | 3,271,575 | +0 |  |
| `d08d2ce856` | 2026-11-12 09:04 -0600 | 17,905 | 3,271,575 | +0 |  |
| `13870af43d` | 2026-11-12 09:04 -0600 | 17,905 | 3,271,575 | +0 |  |
| `6af681618c` | 2026-11-12 09:04 -0600 | 17,905 | 3,271,575 | +0 |  |
| `d3c79f7413` | 2026-11-12 09:04 -0600 | 17,905 | 3,271,575 | +0 |  |
| `aaf488c765` | 2026-11-12 09:13 -0600 | 17,905 | 3,271,575 | +0 | 0 |
| `2e331bd0aa` | 2026-11-12 09:13 -0600 | 17,905 | 3,271,575 | +0 |  |
| `0ad3269f42` | 2026-11-12 09:13 -0600 | 17,905 | 3,271,575 | +0 | 0 |
| `8ca9a8439e` | 2026-11-12 09:13 -0600 | 17,905 | 3,271,575 | +0 |  |
| `d3670c41dd` | 2026-11-12 09:14 -0600 | 17,905 | 3,271,575 | +0 | 0 |
| `6754205257` | 2026-11-12 09:14 -0600 | 17,905 | 3,271,575 | +0 | 0 |
| `905381be6a` | 2026-11-12 09:14 -0600 | 17,905 | 3,271,575 | +0 |  |
| `e7823bcbf6` | 2026-11-12 09:14 -0600 | 17,905 | 3,271,575 | +0 |  |
| `38f0e90753` | 2026-11-12 09:14 -0600 | 17,905 | 3,271,575 | +0 |  |
| `3cc229b2a3` | 2026-11-12 09:14 -0600 | 17,905 | 3,271,575 | +0 |  |
| `745159aafb` | 2026-11-12 09:27 -0600 | 17,905 | 3,271,575 | +0 | 0 |
| `d683b85ebd` | 2026-11-12 09:27 -0600 | 17,905 | 3,271,575 | +0 |  |
| `c240be5d9e` | 2026-11-12 09:27 -0600 | 17,905 | 3,271,575 | +0 |  |
| `460e5c0073` | 2026-11-12 09:27 -0600 | 17,905 | 3,271,575 | +0 |  |
| `a4a3c27dc7` | 2026-11-12 09:27 -0600 | 17,905 | 3,271,575 | +0 |  |
| `be364af880` | 2026-11-12 09:27 -0600 | 17,905 | 3,271,575 | +0 |  |
| `d4e47d5e64` | 2026-11-12 09:27 -0600 | 17,905 | 3,271,575 | +0 |  |
| `c8927d430a` | 2026-11-12 09:27 -0600 | 17,905 | 3,271,575 | +0 |  |
| `71fda33662` | 2026-11-12 09:27 -0600 | 17,905 | 3,271,575 | +0 |  |
| `705001355e` | 2026-11-12 09:27 -0600 | 17,905 | 3,271,575 | +0 | 0 |
| `a24fe7f915` | 2026-11-12 09:39 -0600 | 17,905 | 3,271,575 | +0 | 0 |
| `860fe0144d` | 2026-11-12 09:39 -0600 | 17,905 | 3,271,575 | +0 |  |
| `8535901be0` | 2026-11-12 09:39 -0600 | 17,905 | 3,271,575 | +0 |  |
| `2ad4fc855e` | 2026-11-12 09:39 -0600 | 17,905 | 3,271,575 | +0 |  |
| `9df2160156` | 2026-11-12 09:39 -0600 | 17,905 | 3,271,575 | +0 |  |
| `68bb7388ed` | 2026-11-12 09:50 -0600 | 17,905 | 3,271,575 | +0 | 0 |
| `e52554673e` | 2026-11-12 09:50 -0600 | 17,905 | 3,271,575 | +0 |  |
| `7facb3f979` | 2026-11-12 09:50 -0600 | 17,905 | 3,271,575 | +0 |  |
| `d16a00166d` | 2026-11-12 09:50 -0600 | 17,905 | 3,271,575 | +0 |  |
| `87038d12de` | 2026-11-12 09:50 -0600 | 17,905 | 3,271,575 | +0 |  |
| `e85b3fd8f2` | 2026-11-12 09:50 -0600 | 17,905 | 3,271,575 | +0 |  |
| `6d65985117` | 2026-11-12 09:50 -0600 | 17,905 | 3,271,575 | +0 |  |
| `0b32911851` | 2026-11-12 09:50 -0600 | 17,905 | 3,271,575 | +0 |  |
| `95af9ed558` | 2026-11-12 09:50 -0600 | 17,905 | 3,271,575 | +0 |  |
| `cb66605c05` | 2026-11-12 10:07 -0600 | 17,905 | 3,271,575 | +0 | 0 |
| `3fe912783e` | 2026-11-12 10:07 -0600 | 17,905 | 3,271,575 | +0 |  |
| `68371888ca` | 2026-11-12 10:07 -0600 | 17,905 | 3,271,575 | +0 | 0 |
| `5000f83d02` | 2026-11-12 10:07 -0600 | 17,905 | 3,271,575 | +0 |  |
| `c798c277aa` | 2026-11-12 10:07 -0600 | 17,905 | 3,271,575 | +0 |  |
| `828c3e0c53` | 2026-11-12 10:17 -0600 | 17,905 | 3,271,575 | +0 | 0 |
| `148e4176e2` | 2026-11-12 10:17 -0600 | 17,905 | 3,271,575 | +0 |  |
| `f32cc1624e` | 2026-11-12 10:17 -0600 | 17,905 | 3,271,575 | +0 |  |
| `8ed1e262a6` | 2026-11-12 10:17 -0600 | 17,905 | 3,271,575 | +0 |  |
| `01c494e8b5` | 2026-11-12 10:17 -0600 | 17,905 | 3,271,575 | +0 |  |
| `09cf1c67f1` | 2026-11-12 10:26 -0600 | 17,905 | 3,271,575 | +0 | 0 |
| `525d548e27` | 2026-11-12 10:26 -0600 | 17,905 | 3,271,575 | +0 |  |
| `78b6cc7bfe` | 2026-11-12 10:26 -0600 | 17,905 | 3,271,575 | +0 |  |
| `fb2514c327` | 2026-11-12 10:26 -0600 | 17,905 | 3,271,575 | +0 |  |
| `49aef00120` | 2026-11-12 10:26 -0600 | 17,905 | 3,271,575 | +0 |  |
| `548d8234fc` | 2026-11-12 10:26 -0600 | 17,905 | 3,271,575 | +0 |  |
| `f9d1040862` | 2026-11-12 10:26 -0600 | 17,905 | 3,271,575 | +0 | 0 |
| `6bd774add4` | 2026-11-12 10:26 -0600 | 17,905 | 3,271,575 | +0 |  |
| `b4600bddb5` | 2026-11-12 10:26 -0600 | 17,905 | 3,271,575 | +0 |  |
| `c4dec48454` | 2026-11-12 10:42 -0600 | 17,905 | 3,271,575 | +0 | 0 |
| `a31db97e31` | 2026-11-12 10:42 -0600 | 17,905 | 3,271,575 | +0 |  |
| `3935b49c23` | 2026-11-12 10:42 -0600 | 17,905 | 3,271,575 | +0 |  |
| `b4037bd15e` | 2026-11-12 10:42 -0600 | 17,905 | 3,271,575 | +0 |  |
| `644929f5a3` | 2026-11-12 10:42 -0600 | 17,905 | 3,271,575 | +0 | 0 |
| `790360a559` | 2026-11-12 10:42 -0600 | 17,905 | 3,271,575 | +0 |  |
| `e9d0ff736b` | 2026-11-12 10:42 -0600 | 17,905 | 3,271,575 | +0 |  |
| `7d5ddf9614` | 2026-11-12 10:42 -0600 | 17,905 | 3,271,575 | +0 |  |
| `be44ed1e25` | 2026-11-12 10:42 -0600 | 17,905 | 3,271,575 | +0 |  |
| `56707c428d` | 2026-11-12 10:42 -0600 | 17,905 | 3,271,575 | +0 |  |
| `d3bd6a7982` | 2026-11-12 10:56 -0600 | 17,905 | 3,271,575 | +0 | 0 |
| `122e66af48` | 2026-11-12 10:56 -0600 | 17,905 | 3,271,575 | +0 |  |
| `f81234e5fe` | 2026-11-12 10:56 -0600 | 17,905 | 3,271,575 | +0 |  |
| `05f0463cea` | 2026-11-12 10:56 -0600 | 17,905 | 3,271,575 | +0 |  |
| `8b5e3a9a6a` | 2026-11-12 10:56 -0600 | 17,905 | 3,271,575 | +0 |  |
| `cea10ca4d3` | 2026-11-12 10:56 -0600 | 17,905 | 3,271,575 | +0 |  |
| `8b67a1eea8` | 2026-11-12 11:05 -0600 | 17,905 | 3,271,575 | +0 | 0 |
| `5a41fe2892` | 2026-11-12 11:05 -0600 | 17,905 | 3,271,575 | +0 |  |
| `4e5d52a823` | 2026-11-12 11:05 -0600 | 17,905 | 3,271,575 | +0 |  |
| `4ba2431017` | 2026-11-12 11:05 -0600 | 17,905 | 3,271,575 | +0 |  |
| `c701d9119a` | 2026-11-12 11:05 -0600 | 17,905 | 3,271,575 | +0 |  |
| `4d10665760` | 2026-11-12 11:05 -0600 | 17,905 | 3,271,575 | +0 | 0 |
| `41bf6e8c2e` | 2026-11-12 11:05 -0600 | 17,905 | 3,271,575 | +0 |  |
| `a5e7254075` | 2026-11-12 11:05 -0600 | 17,905 | 3,271,575 | +0 |  |
| `b0c088e0d6` | 2026-11-12 11:05 -0600 | 17,905 | 3,271,575 | +0 |  |
| `0399d22edc` | 2026-11-12 11:05 -0600 | 17,905 | 3,271,575 | +0 |  |
| `b167250099` | 2026-11-12 11:14 -0600 | 17,905 | 3,271,575 | +0 | 0 |
| `144fb489b9` | 2026-11-12 11:14 -0600 | 17,905 | 3,271,575 | +0 | 0 |
| `31ec576983` | 2026-11-12 11:14 -0600 | 17,905 | 3,271,575 | +0 | 0 |
| `62609e9caf` | 2026-11-12 11:14 -0600 | 17,905 | 3,271,575 | +0 |  |
| `0102125a40` | 2026-11-12 11:14 -0600 | 17,905 | 3,271,575 | +0 | 0 |
| `7d639d74d3` | 2026-11-12 11:28 -0600 | 17,905 | 3,271,575 | +0 | 0 |
| `dd58efb5d4` | 2026-11-12 11:28 -0600 | 17,905 | 3,271,575 | +0 |  |
| `1b25f1c69b` | 2026-11-12 11:28 -0600 | 17,905 | 3,271,575 | +0 |  |
| `f535f989bf` | 2026-11-12 11:28 -0600 | 17,905 | 3,271,575 | +0 |  |
| `249355c211` | 2026-11-12 11:28 -0600 | 17,905 | 3,271,575 | +0 |  |
| `591b281663` | 2026-11-12 11:28 -0600 | 17,905 | 3,271,575 | +0 |  |
| `1c2020a0bc` | 2026-11-12 11:28 -0600 | 17,905 | 3,271,575 | +0 |  |
| `f64fb0b4ea` | 2026-11-12 11:28 -0600 | 17,905 | 3,271,575 | +0 |  |
| `c41657c4b1` | 2026-11-12 11:28 -0600 | 17,905 | 3,271,575 | +0 |  |
| `ef15a50288` | 2026-11-12 11:40 -0600 | 17,905 | 3,271,575 | +0 | 0 |
| `f223417a9c` | 2026-11-12 11:40 -0600 | 17,905 | 3,271,575 | +0 |  |
| `4e5d280057` | 2026-11-12 11:40 -0600 | 17,905 | 3,271,575 | +0 |  |
| `63657365dd` | 2026-11-12 11:40 -0600 | 17,905 | 3,271,575 | +0 |  |
| `27b61d1b0c` | 2026-11-12 11:49 -0600 | 17,905 | 3,271,575 | +0 | 0 |
| `fae0c51a46` | 2026-11-12 11:56 -0600 | 17,905 | 3,271,575 | +0 | 0 |
| `5d13ed4b47` | 2026-11-12 12:58 -0600 | 17,905 | 3,271,575 | +0 | 0 |
| `bea9ae1b0c` | 2026-11-12 12:58 -0600 | 17,905 | 3,271,575 | +0 |  |
| `0e03d39b5b` | 2026-11-12 12:58 -0600 | 17,905 | 3,271,575 | +0 |  |
| `2a9b95694a` | 2026-11-12 12:58 -0600 | 17,905 | 3,271,575 | +0 |  |
| `8adb4c64c4` | 2026-11-12 13:15 -0600 | 17,905 | 3,271,575 | +0 | 0 |
| `e89060496a` | 2026-11-12 13:15 -0600 | 17,905 | 3,271,575 | +0 |  |
| `18587f6f77` | 2026-11-12 13:15 -0600 | 17,905 | 3,271,575 | +0 |  |
| `e873771964` | 2026-11-12 14:24 -0600 | 17,905 | 3,271,575 | +0 | 0 |
| `ec3d50b72a` | 2026-11-12 15:25 -0600 | 17,905 | 3,271,575 | +0 | 0 |
| `4695653f1b` | 2026-11-12 15:41 -0600 | 17,905 | 3,271,575 | +0 | 0 |
| `6d3aa02e7b` | 2026-11-12 16:39 -0600 | 17,905 | 3,271,575 | +0 | 0 |
| `0f9f52d034` | 2026-11-12 17:10 -0600 | 17,905 | 3,271,575 | +0 | 0 |
| `c30d45f869` | 2026-11-12 17:35 -0600 | 17,905 | 3,271,575 | +0 | 0 |
| `9d5710c537` | 2026-11-12 18:49 -0600 | 17,905 | 3,271,575 | +0 | 0 |
| `18f393726f` | 2026-11-12 18:59 -0600 | 17,905 | 3,271,575 | +0 | 0 |
| `c9a48112ad` | 2026-11-12 19:11 -0600 | 17,905 | 3,271,575 | +0 | 0 |
| `bd109482eb` | 2026-11-12 19:58 -0600 | 17,905 | 3,271,575 | +0 | 0 |
| `78f7eecf2a` | 2026-11-12 20:11 -0600 | 17,905 | 3,271,575 | +0 | 0 |
| `0db5c68668` | 2026-11-12 20:26 -0600 | 17,905 | 3,271,575 | +0 | 0 |
| `5a6c27faab` | 2026-11-12 20:35 -0600 | 17,905 | 3,271,575 | +0 | 0 |
| `0ca549bbfb` | 2026-11-12 21:01 -0600 | 17,905 | 3,271,575 | +0 | 0 |
| `35b5779b49` | 2026-11-12 21:39 -0600 | 17,905 | 3,271,575 | +0 | 0 |
| `4915c3ee3a` | 2026-11-12 22:15 -0600 | 17,905 | 3,271,575 | +0 | 0 |
| `d0e004d7b7` | 2026-11-12 23:19 -0600 | 17,905 | 3,271,575 | +0 | 0 |
| `2d28a94e1d` | 2026-11-12 23:19 -0600 | 17,905 | 3,271,575 | +0 |  |
| `3bac800b9e` | 2026-11-13 00:14 -0600 | 17,905 | 3,271,575 | +0 | 0 |
| `6eccb78159` | 2026-11-13 00:16 -0600 | 17,905 | 3,271,575 | +0 | 0 |
| `4cfd624d0d` | 2026-11-13 00:18 -0600 | 17,905 | 3,271,575 | +0 | 0 |
| `a620ed5c61` | 2026-11-13 00:21 -0600 | 17,905 | 3,271,575 | +0 | 0 |
| `78c550f0a8` | 2026-11-13 00:23 -0600 | 17,905 | 3,271,575 | +0 | 0 |
| `a842cd542e` | 2026-11-13 00:25 -0600 | 17,905 | 3,271,575 | +0 | 0 |
| `18aa79dda9` | 2026-11-13 00:28 -0600 | 17,905 | 3,271,575 | +0 | 0 |
| `ae65e9e882` | 2026-11-13 00:30 -0600 | 17,905 | 3,271,575 | +0 | 0 |
| `4d85542497` | 2026-11-13 03:27 -0600 | 17,905 | 3,271,575 | +0 | 0 |
| `c25851952b` | 2026-11-13 03:27 -0600 | 17,905 | 3,271,575 | +0 | 0 |
| `356056bd56` | 2026-11-13 04:05 -0600 | 17,905 | 3,271,575 | +0 | 0 |
| `035c06c00e` | 2026-11-13 04:30 -0600 | 17,905 | 3,271,575 | +0 | 0 |
| `7d390138ef` | 2026-11-13 04:46 -0600 | 17,905 | 3,271,575 | +0 | 0 |
| `fbf96dab91` | 2026-11-13 05:07 -0600 | 17,905 | 3,271,575 | +0 | 0 |
| `7f8def052d` | 2026-11-13 05:10 -0600 | 17,905 | 3,271,575 | +0 | 0 |
| `38741fc2c2` | 2026-11-13 05:16 -0600 | 17,905 | 3,271,575 | +0 | 0 |
| `fef3d36bb2` | 2026-11-13 05:19 -0600 | 17,905 | 3,271,575 | +0 | 0 |
| `5992f1ab24` | 2026-11-13 05:45 -0600 | 17,905 | 3,271,575 | +0 | 0 |
| `281c3f71ba` | 2026-11-13 06:06 -0600 | 17,905 | 3,271,575 | +0 | 0 |
| `edf8dd8a00` | 2026-11-13 06:53 -0600 | 17,905 | 3,271,575 | +0 | 0 |
| `a9aaf1b545` | 2026-11-13 06:53 -0600 | 17,905 | 3,271,575 | +0 |  |
| `47e05bbd34` | 2026-11-13 07:09 -0600 | 17,905 | 3,271,575 | +0 | 0 |
| `b0e19ac2c9` | 2026-11-13 07:27 -0600 | 17,905 | 3,271,575 | +0 | 0 |
| `1396ccf8a4` | 2026-11-13 08:16 -0600 | 17,905 | 3,271,575 | +0 | 0 |
| `5710649651` | 2026-11-13 08:50 -0600 | 17,905 | 3,271,575 | +0 | 0 |
| `a5b67762f2` | 2026-11-13 09:16 -0600 | 17,905 | 3,271,575 | +0 | 0 |
| `51bac5b45a` | 2026-11-13 09:57 -0600 | 17,905 | 3,271,575 | +0 | 0 |
| `e8daeb6a87` | 2026-11-13 10:03 -0600 | 17,905 | 3,271,575 | +0 | 0 |
| `8266bedc34` | 2026-11-13 10:05 -0600 | 17,905 | 3,271,575 | +0 | 0 |
| `3498bb7952` | 2026-11-13 10:10 -0600 | 17,905 | 3,271,575 | +0 | 0 |
| `7757479b99` | 2026-11-13 10:13 -0600 | 17,905 | 3,271,575 | +0 | 0 |
| `a195b1215c` | 2026-11-13 10:31 -0600 | 17,905 | 3,271,575 | +0 | 0 |
| `63212984af` | 2026-11-13 10:39 -0600 | 17,905 | 3,271,575 | +0 | 0 |
| `622c5c95f4` | 2026-11-13 10:45 -0600 | 17,905 | 3,271,575 | +0 | 0 |
| `58b01c21ed` | 2026-11-13 10:57 -0600 | 17,905 | 3,271,575 | +0 | 0 |
| `b99007f216` | 2026-11-13 11:05 -0600 | 17,905 | 3,271,575 | +0 | 0 |
| `354b800ce3` | 2026-11-13 11:16 -0600 | 17,905 | 3,271,575 | +0 | 0 |
| `1e9f1b4ed0` | 2026-11-13 11:26 -0600 | 17,905 | 3,271,575 | +0 | 0 |
| `2a3ff9bf43` | 2026-11-13 11:39 -0600 | 17,905 | 3,271,575 | +0 | 0 |
| `a897af1b4c` | 2026-11-13 11:54 -0600 | 17,905 | 3,271,575 | +0 | 0 |
| `38ed212718` | 2026-11-13 13:08 -0600 | 17,905 | 3,271,575 | +0 | 0 |
| `b4a8fc4bd4` | 2026-11-13 13:12 -0600 | 17,905 | 3,271,575 | +0 | 0 |
| `cf77b4e943` | 2026-11-13 13:34 -0600 | 17,905 | 3,271,575 | +0 | 0 |
| `a7ade0bd70` | 2026-11-13 14:15 -0600 | 17,905 | 3,271,575 | +0 | 0 |
| `51156dc78f` | 2026-11-13 14:15 -0600 | 17,905 | 3,271,575 | +0 | 0 |
| `5e33bb5982` | 2026-11-13 14:15 -0600 | 17,905 | 3,271,575 | +0 | 0 |
| `fb72c225ff` | 2026-11-13 14:15 -0600 | 17,905 | 3,271,575 | +0 |  |
| `25000d038d` | 2026-11-13 14:15 -0600 | 17,905 | 3,271,575 | +0 |  |
| `b6e8a3bb8f` | 2026-11-13 14:15 -0600 | 17,905 | 3,271,575 | +0 | 0 |
| `59979df377` | 2026-11-13 14:49 -0600 | 17,905 | 3,271,575 | +0 | 0 |
| `ef98a6ece2` | 2026-11-13 14:49 -0600 | 17,905 | 3,271,575 | +0 |  |
| `fd35174c8b` | 2026-11-13 14:49 -0600 | 17,905 | 3,271,575 | +0 |  |
| `0a98d896e0` | 2026-11-13 15:27 -0600 | 17,905 | 3,271,575 | +0 | 0 |
| `a82fa4e9f8` | 2026-11-13 15:27 -0600 | 17,905 | 3,271,575 | +0 |  |
| `a6b81649a5` | 2026-11-13 15:27 -0600 | 17,905 | 3,271,575 | +0 | 0 |
| `be735980f0` | 2026-11-13 15:27 -0600 | 17,905 | 3,271,575 | +0 | 0 |
| `4adb063276` | 2026-11-13 15:27 -0600 | 17,905 | 3,271,575 | +0 |  |
| `cad6acf7f1` | 2026-11-13 15:27 -0600 | 17,905 | 3,271,575 | +0 | 0 |
| `051bb98b89` | 2026-11-13 17:31 -0600 | 17,905 | 3,271,575 | +0 | 0 |
| `192931c77f` | 2026-11-13 18:29 -0600 | 17,905 | 3,271,575 | +0 | 0 |
| `f433f73fd4` | 2026-11-13 19:24 -0600 | 17,905 | 3,271,575 | +0 | 0 |
| `7b2901e026` | 2026-11-13 19:24 -0600 | 17,905 | 3,271,575 | +0 |  |
| `18c080d3c6` | 2026-11-13 22:12 -0600 | 17,905 | 3,271,575 | +0 | 0 |
| `bd20eee17e` | 2026-11-14 04:00 -0600 | 17,905 | 3,271,575 | +0 | 0 |
| `b49aef9f97` | 2026-11-14 04:21 -0600 | 17,905 | 3,271,575 | +0 | 0 |
| `8611d2d8c9` | 2026-11-14 04:25 -0600 | 17,905 | 3,271,575 | +0 | 0 |
| `23659b1551` | 2026-11-14 05:00 -0600 | 17,905 | 3,271,575 | +0 | 0 |
| `4afe94f967` | 2026-11-14 06:22 -0600 | 17,907 | 3,272,459 | +884 | 648 |
| `50bc8deeec` | 2026-11-14 06:22 -0600 | 17,907 | 3,272,459 | +0 |  |
| `424f884855` | 2026-11-14 07:35 -0600 | 17,907 | 3,272,459 | +0 | 0 |
| `3eb2bee59a` | 2026-11-14 07:35 -0600 | 17,907 | 3,272,459 | +0 | 0 |
| `fd6ad1a1c7` | 2026-11-14 08:23 -0600 | 17,907 | 3,272,459 | +0 | 0 |
| `e369c1619c` | 2026-11-14 08:26 -0600 | 17,907 | 3,272,459 | +0 | 0 |
| `47960f17c3` | 2026-11-14 08:29 -0600 | 17,907 | 3,272,459 | +0 | 0 |
| `674b96f707` | 2026-11-14 08:38 -0600 | 17,907 | 3,272,459 | +0 | 0 |
| `97a48e66c2` | 2026-11-14 09:19 -0600 | 17,907 | 3,272,459 | +0 | 0 |
| `e35458b2e6` | 2026-11-14 10:19 -0600 | 17,907 | 3,272,459 | +0 | 0 |
| `8e8d276e60` | 2026-11-14 10:19 -0600 | 17,907 | 3,272,459 | +0 |  |
| `6a5dc13296` | 2026-11-14 10:19 -0600 | 17,907 | 3,272,459 | +0 |  |
| `7fe957f488` | 2026-11-14 10:19 -0600 | 17,907 | 3,272,459 | +0 | 0 |
| `5bce7c3941` | 2026-11-14 11:12 -0600 | 17,907 | 3,272,459 | +0 | 0 |
| `878403b126` | 2026-11-14 11:12 -0600 | 17,907 | 3,272,459 | +0 |  |
| `8f5c50af9f` | 2026-11-14 11:12 -0600 | 17,907 | 3,272,459 | +0 | 0 |
| `499ff76bd2` | 2026-11-14 11:31 -0600 | 17,907 | 3,272,459 | +0 | 0 |
| `6cd2f83b05` | 2026-11-14 11:34 -0600 | 17,907 | 3,272,459 | +0 | 0 |
| `2712cc6522` | 2026-11-14 11:59 -0600 | 17,907 | 3,272,459 | +0 | 0 |
| `574028e82b` | 2026-11-14 14:13 -0600 | 17,907 | 3,272,459 | +0 | 0 |
| `b8af88c33d` | 2026-11-14 15:04 -0600 | 17,907 | 3,272,459 | +0 | 0 |
| `f9c6a0f50f` | 2026-11-14 15:04 -0600 | 17,907 | 3,272,459 | +0 | 0 |
| `fb02ecaec2` | 2026-11-14 15:04 -0600 | 17,907 | 3,272,459 | +0 |  |
| `6649f8308f` | 2026-11-14 15:54 -0600 | 17,907 | 3,272,459 | +0 | 0 |
| `48cdbc0185` | 2026-11-14 15:54 -0600 | 17,907 | 3,272,459 | +0 |  |
| `b85b8439f7` | 2026-11-14 15:54 -0600 | 17,907 | 3,272,459 | +0 |  |
| `7b5da6880f` | 2026-11-14 18:02 -0600 | 17,907 | 3,272,459 | +0 | 0 |
| `b0bcd3950a` | 2026-11-14 18:02 -0600 | 17,907 | 3,272,459 | +0 | 0 |
| `3a0c96565b` | 2026-11-14 19:38 -0600 | 17,907 | 3,272,459 | +0 | 0 |
| `706deea171` | 2026-11-14 19:38 -0600 | 17,907 | 3,272,459 | +0 | 0 |
| `3bcd74ca52` | 2026-11-14 20:17 -0600 | 17,907 | 3,272,459 | +0 | 0 |
| `6df40b9b61` | 2026-11-14 21:23 -0600 | 17,907 | 3,272,459 | +0 | 0 |
| `dd25ceeede` | 2026-11-14 22:31 -0600 | 17,907 | 3,272,459 | +0 | 0 |
| `5734208dce` | 2026-11-15 00:19 -0600 | 17,907 | 3,272,459 | +0 | 0 |
| `9d7c9d8c57` | 2026-11-15 00:19 -0600 | 17,907 | 3,272,459 | +0 | 0 |
| `07e3f2a177` | 2026-11-15 00:19 -0600 | 17,907 | 3,272,459 | +0 |  |
| `c04895b403` | 2026-11-15 00:19 -0600 | 17,907 | 3,272,459 | +0 |  |
| `4222af151a` | 2026-11-15 00:19 -0600 | 17,907 | 3,272,459 | +0 |  |
| `bd1fce8b0c` | 2026-11-15 00:19 -0600 | 17,907 | 3,272,459 | +0 |  |
| `115d58956b` | 2026-11-15 02:09 -0600 | 17,907 | 3,272,459 | +0 | 0 |
| `8e4f2ae9b6` | 2026-11-15 02:09 -0600 | 17,907 | 3,272,459 | +0 | 0 |
| `992143a107` | 2026-11-15 02:09 -0600 | 17,907 | 3,272,459 | +0 |  |
| `610ab86aa3` | 2026-11-15 02:09 -0600 | 17,907 | 3,272,459 | +0 | 0 |
| `815efeabfa` | 2026-11-15 02:10 -0600 | 17,907 | 3,272,459 | +0 | 0 |
| `5d22bdc1f1` | 2026-11-15 02:10 -0600 | 17,907 | 3,272,459 | +0 |  |
| `fbc781fec0` | 2026-11-15 02:10 -0600 | 17,907 | 3,272,459 | +0 |  |
| `643d0aad70` | 2026-11-15 02:10 -0600 | 17,907 | 3,272,459 | +0 | 0 |
| `c6fa216bfe` | 2026-11-15 02:57 -0600 | 17,907 | 3,272,459 | +0 | 0 |
| `9af930ab09` | 2026-11-15 02:57 -0600 | 17,907 | 3,272,459 | +0 | 0 |
| `bfc92ff99f` | 2026-11-15 03:27 -0600 | 17,907 | 3,272,459 | +0 | 0 |
| `abea1af975` | 2026-11-15 03:27 -0600 | 17,907 | 3,272,459 | +0 |  |
| `9b81f5089b` | 2026-11-15 03:44 -0600 | 17,907 | 3,272,459 | +0 | 0 |
| `322fa2bde7` | 2026-11-15 03:44 -0600 | 17,907 | 3,272,459 | +0 | 0 |
| `f40420fbe5` | 2026-11-15 03:44 -0600 | 17,907 | 3,272,459 | +0 |  |
| `3cf156d39e` | 2026-11-15 03:57 -0600 | 17,907 | 3,272,459 | +0 | 0 |
| `77e5eb6719` | 2026-11-15 03:57 -0600 | 17,907 | 3,272,459 | +0 |  |
| `9e854311b3` | 2026-11-15 05:14 -0600 | 17,907 | 3,272,459 | +0 | 0 |
| `9648ce4521` | 2026-11-15 05:14 -0600 | 17,907 | 3,272,459 | +0 |  |
| `76aab072a3` | 2026-11-15 05:14 -0600 | 17,907 | 3,272,459 | +0 |  |
| `2a243945ca` | 2026-11-15 05:14 -0600 | 17,907 | 3,272,459 | +0 |  |
| `3f300d93b1` | 2026-11-15 06:17 -0600 | 17,907 | 3,272,459 | +0 | 0 |
| `40cb6bd623` | 2026-11-15 06:17 -0600 | 17,907 | 3,272,459 | +0 | 0 |
| `50c9ae41b0` | 2026-11-15 06:29 -0600 | 17,907 | 3,272,459 | +0 | 0 |
| `7567bc238f` | 2026-11-15 06:29 -0600 | 17,907 | 3,272,459 | +0 | 0 |
| `ce35e49dbd` | 2026-11-15 07:15 -0600 | 17,907 | 3,272,459 | +0 | 0 |
| `8518c8b674` | 2026-11-15 08:12 -0600 | 17,907 | 3,272,459 | +0 | 0 |
| `f0e55af69b` | 2026-11-15 08:12 -0600 | 17,907 | 3,272,459 | +0 |  |
| `8fa9c42e5f` | 2026-11-15 09:26 -0600 | 17,907 | 3,272,459 | +0 | 0 |
| `eb0835bf6d` | 2026-11-15 11:09 -0600 | 17,907 | 3,272,459 | +0 | 0 |
| `2616a1a152` | 2026-11-15 13:41 -0600 | 17,907 | 3,272,459 | +0 | 0 |
| `16f292be87` | 2026-11-15 14:38 -0600 | 17,907 | 3,272,459 | +0 | 0 |
| `dce30fbf4d` | 2026-11-15 17:17 -0600 | 17,907 | 3,272,459 | +0 | 0 |
| `5ed06f87fb` | 2026-11-15 17:17 -0600 | 17,907 | 3,272,459 | +0 |  |
| `1722427e3b` | 2026-11-15 17:17 -0600 | 17,907 | 3,272,459 | +0 | 0 |
| `a7242baa80` | 2026-11-15 18:28 -0600 | 17,907 | 3,272,459 | +0 | 0 |
| `e51eebac1e` | 2026-11-15 18:29 -0600 | 17,907 | 3,272,459 | +0 | 0 |
| `5b3d4397e1` | 2026-11-15 19:21 -0600 | 17,907 | 3,272,459 | +0 | 0 |
| `89cafd0707` | 2026-11-15 19:21 -0600 | 17,907 | 3,272,459 | +0 | 0 |
| `2ed696fff2` | 2026-11-15 19:21 -0600 | 17,907 | 3,272,459 | +0 |  |
| `223548916b` | 2026-11-15 21:03 -0600 | 17,907 | 3,272,459 | +0 | 0 |
| `0d65d9378a` | 2026-11-15 23:30 -0600 | 17,907 | 3,272,459 | +0 | 0 |
| `70037a4f69` | 2026-11-15 23:30 -0600 | 17,907 | 3,272,459 | +0 | 0 |
| `cf0817a779` | 2026-11-15 23:31 -0600 | 17,907 | 3,272,459 | +0 | 0 |
| `b8789d8833` | 2026-11-16 00:36 -0600 | 17,907 | 3,272,459 | +0 | 0 |
| `06688192cc` | 2026-11-16 02:08 -0600 | 17,907 | 3,272,459 | +0 | 0 |
| `14369c2a69` | 2026-11-16 02:08 -0600 | 17,907 | 3,272,459 | +0 |  |
| `9ebb02e039` | 2026-11-16 02:41 -0600 | 17,907 | 3,272,459 | +0 | 0 |
| `7d6f09cef7` | 2026-11-16 02:41 -0600 | 17,907 | 3,272,459 | +0 |  |
| `80e04cd357` | 2026-11-16 03:07 -0600 | 17,907 | 3,272,459 | +0 | 0 |
| `3cef4df3e1` | 2026-11-16 03:40 -0600 | 17,907 | 3,272,459 | +0 | 0 |
| `dd74577971` | 2026-11-16 03:40 -0600 | 17,907 | 3,272,459 | +0 | 0 |
| `1fc418da03` | 2026-11-16 04:28 -0600 | 17,907 | 3,272,459 | +0 | 0 |
| `48dced696f` | 2026-11-16 04:28 -0600 | 17,907 | 3,272,459 | +0 |  |
| `0827b9412b` | 2026-11-16 06:25 -0600 | 17,907 | 3,272,459 | +0 | 0 |
| `a350af3da4` | 2026-11-16 06:25 -0600 | 17,907 | 3,272,459 | +0 | 0 |
| `cd6aa7d410` | 2026-11-16 07:05 -0600 | 17,907 | 3,272,459 | +0 | 0 |
| `40d59042d9` | 2026-11-16 07:31 -0600 | 17,907 | 3,272,459 | +0 | 0 |
| `3aeefb8bc0` | 2026-11-16 07:37 -0600 | 17,907 | 3,272,459 | +0 | 0 |
| `3bd0538fd2` | 2026-11-16 07:39 -0600 | 17,907 | 3,272,459 | +0 | 0 |
| `dbd6bf4115` | 2026-11-16 07:42 -0600 | 17,907 | 3,272,459 | +0 | 0 |
| `dcd3b99cf2` | 2026-11-16 07:46 -0600 | 17,907 | 3,272,459 | +0 | 0 |
| `d3d27291a2` | 2026-11-16 07:51 -0600 | 17,907 | 3,272,459 | +0 | 0 |
| `f3b37d1a6a` | 2026-11-16 07:53 -0600 | 17,907 | 3,272,459 | +0 | 0 |
| `83c829cb2a` | 2026-11-16 08:24 -0600 | 17,907 | 3,272,459 | +0 | 0 |
| `7c1f0f52b4` | 2026-11-16 09:14 -0600 | 17,907 | 3,272,459 | +0 | 0 |
| `c985675395` | 2026-11-16 09:14 -0600 | 17,907 | 3,272,459 | +0 |  |
| `6fa5d71e39` | 2026-11-16 09:39 -0600 | 17,907 | 3,272,459 | +0 | 0 |
| `5ce62e2c42` | 2026-11-16 10:05 -0600 | 17,907 | 3,272,459 | +0 | 0 |
| `cf30ba056b` | 2026-11-16 10:12 -0600 | 17,907 | 3,272,459 | +0 | 0 |
| `9cf7f4f829` | 2026-11-16 11:10 -0600 | 17,907 | 3,272,459 | +0 | 0 |
| `14ef980f74` | 2026-11-16 12:22 -0600 | 17,907 | 3,272,459 | +0 | 0 |
| `8a28ea7b15` | 2026-11-16 13:24 -0600 | 17,907 | 3,272,459 | +0 | 0 |
| `a442883cf6` | 2026-11-16 13:24 -0600 | 17,907 | 3,272,459 | +0 | 0 |
| `5d4e44baea` | 2026-11-16 13:24 -0600 | 17,907 | 3,272,459 | +0 | 0 |
| `3bfd2ec2fa` | 2026-11-16 13:24 -0600 | 17,907 | 3,272,459 | +0 |  |
| `60724cdd3c` | 2026-11-16 15:21 -0600 | 17,907 | 3,272,459 | +0 | 0 |
| `e8bc45cbba` | 2026-11-16 15:21 -0600 | 17,907 | 3,272,459 | +0 | 0 |
| `a5f4709638` | 2026-11-16 15:26 -0600 | 17,907 | 3,272,459 | +0 | 0 |
| `ccf390ca95` | 2026-11-16 15:29 -0600 | 17,907 | 3,272,459 | +0 | 0 |
| `bd8bf8fefc` | 2026-11-16 15:34 -0600 | 17,907 | 3,272,459 | +0 | 0 |
| `da5e273095` | 2026-11-16 15:38 -0600 | 17,907 | 3,272,459 | +0 | 0 |
| `3ffa3cfc25` | 2026-11-16 15:38 -0600 | 17,907 | 3,272,459 | +0 |  |
| `3734dc7981` | 2026-11-16 16:19 -0600 | 17,907 | 3,272,459 | +0 | 0 |
| `62a2031294` | 2026-11-16 16:28 -0600 | 17,907 | 3,272,459 | +0 | 0 |
| `a02be401c7` | 2026-11-16 16:34 -0600 | 17,907 | 3,272,459 | +0 | 0 |
| `3adc1d0ef6` | 2026-11-16 17:36 -0600 | 17,907 | 3,272,459 | +0 | 0 |
| `a7c1b94ff0` | 2026-11-16 18:34 -0600 | 17,907 | 3,272,459 | +0 | 0 |
| `30bb9678ca` | 2026-11-16 18:35 -0600 | 17,907 | 3,272,459 | +0 | 0 |
| `44d1b5db4b` | 2026-11-16 20:05 -0600 | 17,907 | 3,272,459 | +0 | 0 |
| `d91c20d7f3` | 2026-11-16 20:08 -0600 | 17,907 | 3,272,459 | +0 | 0 |
| `11fe81474e` | 2026-11-16 20:08 -0600 | 17,907 | 3,272,459 | +0 | 0 |
| `578feb0e59` | 2026-11-16 20:12 -0600 | 17,907 | 3,272,459 | +0 | 0 |
| `179227ab9e` | 2026-11-16 20:12 -0600 | 17,907 | 3,272,459 | +0 |  |
| `091b462102` | 2026-11-16 20:44 -0600 | 17,907 | 3,272,459 | +0 | 0 |
| `823530877b` | 2026-11-16 21:43 -0600 | 17,907 | 3,272,459 | +0 | 0 |
| `32cdb55e2c` | 2026-11-16 22:25 -0600 | 17,907 | 3,272,459 | +0 | 0 |
| `051e9ed049` | 2026-11-16 23:28 -0600 | 17,907 | 3,272,459 | +0 | 0 |
| `5d227d9e6a` | 2026-11-17 00:06 -0600 | 17,907 | 3,272,459 | +0 | 0 |
| `ab6bf547a3` | 2026-11-17 00:13 -0600 | 17,907 | 3,272,459 | +0 | 0 |
| `81529372ef` | 2026-11-17 00:19 -0600 | 17,907 | 3,272,459 | +0 | 0 |
| `59a5cfa196` | 2026-11-17 00:23 -0600 | 17,907 | 3,272,459 | +0 | 0 |
| `7b0efb5c0b` | 2026-11-17 00:26 -0600 | 17,907 | 3,272,459 | +0 | 0 |
| `2a99d86771` | 2026-11-17 01:11 -0600 | 17,907 | 3,272,459 | +0 | 0 |
| `a3a67e68fa` | 2026-11-17 01:20 -0600 | 17,907 | 3,272,459 | +0 | 0 |
| `d4e8b56ccd` | 2026-11-17 02:09 -0600 | 17,907 | 3,272,459 | +0 | 0 |
| `03f1843747` | 2026-11-17 05:28 -0600 | 17,910 | 3,274,492 | +2,033 | 614 |
| `f1a29f3e73` | 2026-11-17 05:54 -0600 | 17,910 | 3,274,492 | +0 | 0 |
| `cce4442a17` | 2026-11-17 10:29 -0600 | 17,910 | 3,274,492 | +0 | 0 |
| `e8d96811b6` | 2026-11-17 10:36 -0600 | 17,910 | 3,274,492 | +0 | 0 |
| `796f2d8f90` | 2026-11-17 10:41 -0600 | 17,910 | 3,274,492 | +0 | 0 |
| `0d09178d79` | 2026-11-17 11:59 -0600 | 17,910 | 3,274,492 | +0 | 0 |
| `36cdf2bded` | 2026-11-17 12:45 -0600 | 17,910 | 3,274,492 | +0 | 0 |

<!-- /generated:status -->

## Notes that the numbers do not show

- **Target:** 4,485,392 bytes, SHA256
  `e6418111c2c36081a1795fb000e0927beea1630170b334c5013d12102602e70f`. The latest
  fresh complete build (`build/class-work/Owner/C1Ordinary-29c31b59-v1/landing/fresh-build/`)
  compiles all 12,971 accepted sources and reproduces every supplied byte; raw linker output
  differs and original PE metadata is retained explicitly.
- **Headline:** quote **Reproduced .text** (everything the build produces from real inputs) with
  **in-scope matched C/C++** (Westwood's C/C++ recovered). "Matched C/C++ body bytes" alone cannot reach
  100%: padding, the linked C runtime and handwritten assembly are never C/C++ source.
- **Not source credit:** linked library members (SP3 LIBCMT/COMSUPP) and evidenced inline assembly are
  their own ledgers; handwritten ranges not yet assembled from TibSun .asm (Iso_Blit_Asm1, Detect_MMX_Availability,
  RA2-only MMX code at 0x7989C1, blitters) and the unexplained CRT runs remain scaffold.
- **Inventory is not a denominator:** baseline Ghidra found 8,077 function
  hypotheses, the seeded pass 16,331; region counts are not verified functions.
- **Readability debt:** early accepted sources include register-transliterated
  leaf functions and per-address owner classes. Replace them with typed source
  during class sweeps when the replacement stays exact; the current directory
  and naming counts are in [RECOVERED_SOURCE_INDEX.md](RECOVERED_SOURCE_INDEX.md).
- **Rebuild "declared, never defined" 60 -> 31 (lane O14, 2026-11-21): 21 false listings and 8 real
  removals, reported apart** (review session's condition on the measure fix). *False listings (measure
  correction, no source change, 21):* 0f85b91f4 read 16 unqualified friend declarations as class members
  (DSAudio Audio_Available, Direct_Sound_Object, Direct_Sound_Primary_Buffer; Restore_Surface; _Graphic_Menu;
  GM_Read_Image_Item; Int Gcd, Generate_Prime; Modem_Guest_Dialog_Proc; ModemHostCallback, Modem_Host_Dialog_Proc;
  Phone_Dialog_Proc; Edit_Phone_Dialog_Proc; Com_Settings_Dialog_Proc; VocClass_From_Name, Voc_Name), 57 -> 41;
  b26573803 read 4 friends after an access label (Dropship_Screen, Recalc_House_Factories, Message_Input,
  Main_Loop) and one spaced template qualifier (PriorityQueueClass <T>::Remove_Matching), 41 -> 36. Tests for
  the three forms: `rebuild/tests/test_declared_never_defined.py` (14 of its 15 fail on the tool before
  0f85b91f4). *Real removals (declarations with no RA2 body and no caller taken out of their headers, every
  includer unchanged, 8 list entries):* SmudgeTypeClass::Prep_For_Add, OverlayTypeClass::Prep_For_Add,
  HouseTypeClass::One_Time (60 -> 57); StripClass::Init_Clear and Init_IO, ShapeSet::Get_Size(int),
  WWMessageBox::_Process (its three overloads, with the six narrow Process forwarders that call them; the wide
  Process stays), WindowsVersionInfo::As_String (36 -> 31). CreditClass::Update stays listed: removing it flips
  wonline Encode_Game_Options by declaration count. Evidence: `rebuild/manifest/b_log_o14.csv`.
