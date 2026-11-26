# Original-target status

Generated numbers below come from `tools/original_target/status.py --write`
(the tree's catalog `manifests/catalog`, `manifests/progress.csv`, and src-final's frozen catalog);
edit only the text outside the markers. Since the layout move of 2026-11-25 there is one tree at
the repository root (`src/`, `include/`, `asm/`; first column); the earlier tree is the tag
`src-final` (reference column; its full frozen table is in
[history/SRC_FINAL_STATUS.md](history/SRC_FINAL_STATUS.md)). The decompilation is not finished: the
table says how much of the original is reproduced from source and what is still taken from the
original. The native ports live in github.com/meastes/redalert2-modern. Per-batch evidence through
A15 is in [history/](history/); current work is in [HANDOFF.md](HANDOFF.md).

<!-- generated:status -->

| Tree (numbers side by side, never added) | this tree (`src/`, `include/`; last gated landing 2026-11-26T18:41Z) | src-final (the earlier tree, frozen 2026-11-24; tag src-final) |
| --- | ---: | ---: |
| Reproduced .text (both trees: C/C++ body + evidenced inline asm + frozen padding + linked library members + assembled handwritten asm, the six lines below) | **96.07% whole-unit** (3,621,597 B) (96.21% function-level, 3,627,101 B) | 93.58% (3,527,727 B) |
| Fuzzy (best draft scores): ByteMatch's fuzzy match, in-scope C/C++ bytes with each function not exact counted at its best measured draft score against its image-derived bound, (body - differing) / body; no exact-match credit (bytematch_report.py; progress.csv fuzzy_best_draft) | fuzzy (best draft scores): **97.415%** | |
| Data reproduced from source, authored (file-backed non-code bytes, 711,662 B: headers, .rdata, raw .data, .rsrc; equal to the original where a compiled unit's object or tracked assembly lays them; a report, not in Reproduced .text or acceptance; by kind in docs/DATA_AS_SOURCE.md) | **369,212 B, 51.9%** (gated landing 2026-11-26T18:41Z) | 151,518 B, 21.3% (lane D sizing, 2026-11-13) |
| Data reproduced from source, with generated modules (authored plus the blocks a tracked generator computes entry by entry from formulas, proven whole at their original addresses: the fastmath tables, .data 0x801DC4-0x827E04, 155,712 B; flagged constants: ATAN_STEP_BITS = 0x3CC7FE84 (fastmath_tables, derivation unknown)) | from the next gated landing | n/a |
| C/C++ bodies matched (body bytes: no padding, no evidenced inline asm; the parity line) | **3,367,704 B, 89.33% whole-unit** (3,372,995 B, 89.47% function-level); gap to src-final -93.2 KB (-98.5 KB function-level) | 3,274,492 B, 86.86% |
| Evidenced inline assembly (cited TibSun `__asm`; not C/C++ credit) | 2,555 B in 8 rows (whole-unit) | 2,514 B |
| Frozen padding of accepted rows | 143,504 B whole-unit (143,717 B function-level) | 143,042 B |
| Linked library members (each tree's ledger: the tree's adds the 12 libcmt rows src-final took as C, lane O96 menu R1, and 4 CRT fragments; not linked, no real input reproduces them: 33 import thunks, 198 B (13 BINKW32, 1 COMCTL32, 19 WSOCK32)) | 89,282 B | 89,127 B |
| Assembled handwritten assembly (the same adopted TibSun .asm procedures in both builds) | 16,999 B | 16,999 B |
| Assembled handwritten assembly, revised (binary-derived lines: ra2-decomp src/ra2asm text revising a cited TibSun archive procedure, allowlisted and flagged; Reproduced .text only, never C/C++; user 2026-11-24) | 1,553 B | 1,553 B |
| C/C++ bodies, exact with no judgment call (each tree's judgment rows left out) | **3,093,555 B, 82.061% whole-unit** (3,096,933 B, 82.151% function-level) of .text; left out: judgment calls 143 rows / 274,149 B (of which header-form dependents 7 / 23,173 B), count-sensitive 1 rows / 1,913 B | 3,245,325 B, 86.09% |
| C/C++ bodies, exact with no soft judgment (only the judgment rows resting on a choice left out: in the rebuild kinds iii author idiom unreferenced at the line, iv-b looked-up local no reference has, vii other, the flagged variation forms and reference-borne forms; main's flags are all choices, so its figure is the line above) | **3,224,385 B, 85.532% whole-unit** (3,227,763 B, 85.621% function-level); left out: soft judgment rows 56 / 143,319 B (kinds iii, iv-b, vii, the variation forms, reference-borne and header-form dependents of such a form), count-sensitive 1 rows / 1,913 B | 3,245,325 B, 86.09% |
| Rebuild judgment calls by kind (function-level, rows/body bytes; i reference form chosen by caller evidence, ii sibling-image helper form, iii author idiom unreferenced at the line, iv-a twin or reference text at this or a sibling site, iv-b looked-up local no reference has, v image-derived or frozen bound, vi header-form dependent, joined two references joined at one site, vii other; helper, spelling, compiler-local, readability-alias flagged variation forms; reference-borne a refused-if-coined form in adopted reference text; kinds tagged in provenance.csv and header_judgments.csv) | iii 1/747; iv-a 68/99988; iv-b 1/7752; v 7/6073; vi 8/23522; joined 5/12413; vii 2/1407; helper 8/23946; spelling 18/39978; compiler-local 12/36146; reference-borne 13/22177 | |
| Judgment calls, one definition for both trees (main's: the manifest flags count_calibrated, split_declaration, declarations_only; in the rebuild, the accepted rows whose provenance.csv judgment note, or a header_judgments.csv form they depend on, names one of those flags; whole-unit, body bytes) | 0 rows / 0 B | 10 rows / 29,167 B |
| Judgment calls, each tree's own count (not comparable row for row: main counts the manifest flags above; the rebuild counts every provenance.csv judgment note, which includes twin-text forms and bounds, plus the rows listed as dependents of a flagged header form; body bytes) | 143 rows / 274,149 B | 10 rows / 29,167 B |
| Matched in src-final (main), not yet in the rebuild (main's accepted entries with no rebuild catalog row; body bytes; stays until it reaches zero) | 68 functions / 42,460 B | |
| Accepted | 17,919 functions in 444 units; function-level 17,954 functions | 17,910 regions |
| Provenance (docs/REBUILD.md; body plus padding) | adopted 16,974 fn / 3,002,927 B; reshaped from src/ 189 / 214,196 B; fresh 163 / 19,920 B; Windows workers 593 / 276,720 B | |
| Rebuild structure | duplicates 0, externs 6 landed / 6 all, address-suffixed 0; parked 113 functions / 146,249 B (>=1 KB: 34 / 135,992 B); judgment 136 rows / 250,976 B; count-sensitive 1 rows / 1,913 B | |
| Rebuild evidenced inline assembly reached through a file-static helper of its unit | 2 rows / 789 B | |
| Rebuild asm bytes not yet flagged (main flags them; header-template attribution to fix) | 0 rows / 0 B | |

Rebuild history (manifests/progress.csv; C/C++ body bytes whole-unit per gated landing that changed them, and per hour since the previous one):

| Landing (UTC) | Functions | C/C++ body bytes | +Body | +Body/hour |
| --- | ---: | ---: | ---: | ---: |
| 2026-11-26T05:45Z | 17,904 | 3,339,162 | +2,368 | 14,208 |
| 2026-11-26T06:29Z | 17,905 | 3,341,271 | +2,109 | 2,876 |
| 2026-11-26T06:49Z | 17,906 | 3,341,344 | +73 | 219 |
| 2026-11-26T06:59Z | 17,907 | 3,342,085 | +741 | 4,446 |
| 2026-11-26T07:21Z | 17,908 | 3,343,554 | +1,469 | 4,006 |
| 2026-11-26T07:33Z | 17,909 | 3,344,688 | +1,134 | 5,670 |
| 2026-11-26T12:43Z | 17,910 | 3,349,200 | +4,512 | 873 |
| 2026-11-26T12:55Z | 17,911 | 3,351,701 | +2,501 | 12,505 |
| 2026-11-26T13:06Z | 17,914 | 3,357,779 | +6,078 | 33,153 |
| 2026-11-26T13:17Z | 17,915 | 3,357,815 | +36 | 196 |
| 2026-11-26T14:56Z | 17,917 | 3,361,233 | +3,418 | 2,072 |
| 2026-11-26T15:29Z | 17,919 | 3,367,704 | +6,471 | 11,765 |

src-final (the earlier tree: `src/recovered`, `src/shared` and the catalog manifests/original_target/, frozen on 2026-11-24): its full table and commit history, which cannot change, are in [history/SRC_FINAL_STATUS.md](history/SRC_FINAL_STATUS.md).

Where the accepted text came from (tools/original_target/provenance_rollup.py over manifests/catalog; body bytes include evidenced inline asm):

| Source of the text | Functions | Body bytes | Share of body bytes | Of the functions: placed by the unit file's adopted origin |
| --- | ---: | ---: | ---: | ---: |
| mischa85 | 14,346 | 2,286,127 | 67.8% | 872 |
| ra2-decomp | 1,034 | 275,930 | 8.2% | 1 |
| tibsun | 752 | 65,987 | 2.0% |  |
| robin | 1 | 866 | 0.0% |  |
| renegade | 0 | 0 | 0.0% |  |
| generals | 3 | 2,097 | 0.1% |  |
| reshaped-from-main (this project's earlier tree) | 189 | 212,833 | 6.3% |  |
| fresh (written here from the seed and the class layout) | 163 | 18,563 | 0.6% |  |
| windows-workers (this project's own matching, done on its Windows machines) | 593 | 271,899 | 8.1% |  |
| mixed (template copies and header scans in files not adopted from one source) | 838 | 235,957 | 7.0% |  |
| total | 17,919 | 3,370,259 | 100.0% | 873 |

The sources build on one another (mischa85's on the TibSun reconstruction; the Yuri's Revenge and 1.001 projects on each other and on this one), so this records where each function's text was taken from, not who first wrote it.

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
  the three forms: `tests/test_declared_never_defined.py` (14 of its 15 fail on the tool before
  0f85b91f4). *Real removals (declarations with no RA2 body and no caller taken out of their headers, every
  includer unchanged, 8 list entries):* SmudgeTypeClass::Prep_For_Add, OverlayTypeClass::Prep_For_Add,
  HouseTypeClass::One_Time (60 -> 57); StripClass::Init_Clear and Init_IO, ShapeSet::Get_Size(int),
  WWMessageBox::_Process (its three overloads, with the six narrow Process forwarders that call them; the wide
  Process stays), WindowsVersionInfo::As_String (36 -> 31). CreditClass::Update stays listed: removing it flips
  wonline Encode_Game_Options by declaration count. Evidence: `manifests/b_log_o14.csv`.
