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

| Tree (numbers side by side, never added) | this tree (`src/`, `include/`; last gated landing 2026-11-28T03:26Z) | src-final (the earlier tree, frozen 2026-11-24; tag src-final) |
| --- | ---: | ---: |
| Reproduced .text (both trees: C/C++ body + evidenced inline asm + frozen padding + linked library members + assembled handwritten asm, the six lines below) | **96.83% whole-unit** (3,650,429 B) (96.87% function-level, 3,651,629 B) | 93.58% (3,527,727 B) |
| Fuzzy (best draft scores): ByteMatch's fuzzy match, in-scope C/C++ bytes with each function not exact counted at its best measured draft score against its image-derived bound, (body - differing) / body; no exact-match credit (bytematch_report.py; progress.csv fuzzy_best_draft) | fuzzy (best draft scores): **97.928%** | |
| Data reproduced from source, authored (file-backed non-code bytes, 711,662 B: headers, .rdata, raw .data, .rsrc; equal to the original where a compiled unit's object or tracked assembly lays them; a report, not in Reproduced .text or acceptance; by kind in docs/DATA_AS_SOURCE.md) | **369,555 B, 51.9%** (gated landing 2026-11-28T03:26Z) | 151,518 B, 21.3% (lane D sizing, 2026-11-13) |
| Data reproduced from source, with generated modules (authored plus the blocks a tracked generator computes entry by entry from formulas, proven whole at their original addresses: the fastmath tables, .data 0x801DC4-0x827E04, 155,712 B; flagged constants: ATAN_STEP_BITS = 0x3CC7FE84 (fastmath_tables, derivation unknown)) | 525,267 B, 73.8% (gated landing 2026-11-28T03:26Z) | n/a |
| C/C++ bodies matched (body bytes: no padding, no evidenced inline asm; the parity line) | **3,396,271 B, 90.09% whole-unit** (3,397,363 B, 90.12% function-level); gap to src-final -121.8 KB (-122.9 KB function-level) | 3,274,492 B, 86.86% |
| Evidenced inline assembly (cited TibSun `__asm`; not C/C++ credit) | 2,555 B in 8 rows (whole-unit) | 2,514 B |
| Frozen padding of accepted rows | 143,769 B whole-unit (143,877 B function-level) | 143,042 B |
| Linked library members (each tree's ledger: the tree's adds the 12 libcmt rows src-final took as C, lane O96 menu R1, and 4 CRT fragments; not linked, no real input reproduces them: 33 import thunks, 198 B (13 BINKW32, 1 COMCTL32, 19 WSOCK32)) | 89,282 B | 89,127 B |
| Assembled handwritten assembly (the same adopted TibSun .asm procedures in both builds) | 16,999 B | 16,999 B |
| Assembled handwritten assembly, revised (binary-derived lines: ra2-decomp src/ra2asm text revising a cited TibSun archive procedure, allowlisted and flagged; Reproduced .text only, never C/C++; user 2026-11-24) | 1,553 B | 1,553 B |
| C/C++ bodies, exact with no judgment call (each tree's judgment rows left out) | **3,083,108 B, 81.784% whole-unit** (3,084,200 B, 81.813% function-level) of .text; left out: judgment calls 155 rows / 313,163 B (of which header-form dependents 8 / 23,273 B), count-sensitive 0 rows / 0 B | 3,245,325 B, 86.09% |
| C/C++ bodies, exact with no soft judgment (only the judgment rows resting on a choice left out: in the rebuild kinds iii author idiom unreferenced at the line, iv-b looked-up local no reference has, vii other, the flagged variation forms and reference-borne forms; main's flags are all choices, so its figure is the line above) | **3,214,038 B, 85.257% whole-unit** (3,215,130 B, 85.286% function-level); left out: soft judgment rows 67 / 182,233 B (kinds iii, iv-b, vii, the variation forms, reference-borne and header-form dependents of such a form), count-sensitive 0 rows / 0 B | 3,245,325 B, 86.09% |
| Rebuild judgment calls by kind (function-level, rows/body bytes; i reference form chosen by caller evidence, ii sibling-image helper form, iii author idiom unreferenced at the line, iv-a twin or reference text at this or a sibling site, iv-b looked-up local no reference has, v image-derived or frozen bound, vi header-form dependent, joined two references joined at one site, vii other; helper, spelling, compiler-local, readability-alias, count-anchor flagged variation forms; reference-borne a refused-if-coined form in adopted reference text; kinds tagged in provenance.csv and header_judgments.csv) | iii 1/747; iv-a 68/99988; iv-b 1/7752; v 7/6073; vi 9/23622; joined 5/12413; vii 2/1407; helper 11/29475; spelling 20/51484; compiler-local 13/39434; reference-borne 16/26056; count-anchor 2/14712 | |
| Judgment calls, one definition for both trees (main's: the manifest flags count_calibrated, split_declaration, declarations_only; in the rebuild, the accepted rows whose provenance.csv judgment note, or a header_judgments.csv form they depend on, names one of those flags; whole-unit, body bytes) | 0 rows / 0 B | 10 rows / 29,167 B |
| Judgment calls, each tree's own count (not comparable row for row: main counts the manifest flags above; the rebuild counts every provenance.csv judgment note, which includes twin-text forms and bounds, plus the rows listed as dependents of a flagged header form; body bytes) | 155 rows / 313,163 B | 10 rows / 29,167 B |
| Matched in src-final (main), not yet in the rebuild (main's accepted entries with no rebuild catalog row; body bytes; stays until it reaches zero) | 28 functions / 31,299 B | |
| Accepted | 17,964 functions in 444 units; function-level 17,978 functions | 17,910 regions |
| Provenance (docs/REBUILD.md; body plus padding) | adopted 17,019 fn / 3,031,759 B; reshaped from src/ 189 / 214,196 B; fresh 163 / 19,920 B; Windows workers 593 / 276,720 B | |
| Rebuild structure | duplicates 0, externs 6 landed / 6 all, address-suffixed 0; parked 62 functions / 118,961 B (>=1 KB: 27 / 113,568 B); judgment 147 rows / 289,890 B; count-sensitive 0 rows / 0 B | |
| Rebuild evidenced inline assembly reached through a file-static helper of its unit | 2 rows / 789 B | |
| Rebuild asm bytes not yet flagged (main flags them; header-template attribution to fix) | 0 rows / 0 B | |

Rebuild history (manifests/progress.csv; C/C++ body bytes whole-unit per gated landing that changed them, and per hour since the previous one):

| Landing (UTC) | Functions | C/C++ body bytes | +Body | +Body/hour |
| --- | ---: | ---: | ---: | ---: |
| 2026-11-26T14:56Z | 17,917 | 3,361,233 | +3,418 | 2,072 |
| 2026-11-26T15:29Z | 17,919 | 3,367,704 | +6,471 | 11,765 |
| 2026-11-26T21:00Z | 17,941 | 3,369,927 | +2,223 | 403 |
| 2026-11-26T22:24Z | 17,942 | 3,373,215 | +3,288 | 2,349 |
| 2026-11-26T23:00Z | 17,950 | 3,374,145 | +930 | 1,550 |
| 2026-11-27T07:16Z | 17,952 | 3,378,207 | +4,062 | 491 |
| 2026-11-27T11:51Z | 17,953 | 3,380,120 | +1,913 | 417 |
| 2026-11-27T13:31Z | 17,954 | 3,386,659 | +6,539 | 3,923 |
| 2026-11-27T15:31Z | 17,955 | 3,387,170 | +511 | 256 |
| 2026-11-27T20:36Z | 17,962 | 3,389,837 | +2,667 | 525 |
| 2026-11-28T01:12Z | 17,963 | 3,394,804 | +4,967 | 1,080 |
| 2026-11-28T03:26Z | 17,964 | 3,396,271 | +1,467 | 657 |

src-final (the earlier tree: `src/recovered`, `src/shared` and the catalog manifests/original_target/, frozen on 2026-11-24): its full table and commit history, which cannot change, are in [history/SRC_FINAL_STATUS.md](history/SRC_FINAL_STATUS.md).

Where the accepted text came from (tools/original_target/provenance_rollup.py over manifests/catalog; body bytes include evidenced inline asm):

| Source of the text | Functions | Body bytes | Share of body bytes | Of the functions: placed by the unit file's adopted origin |
| --- | ---: | ---: | ---: | ---: |
| mischa85 | 14,365 | 2,302,047 | 67.7% | 889 |
| ra2-decomp | 1,042 | 279,441 | 8.2% | 1 |
| tibsun | 758 | 68,554 | 2.0% | 1 |
| robin | 1 | 866 | 0.0% |  |
| renegade | 0 | 0 | 0.0% |  |
| generals | 3 | 2,097 | 0.1% |  |
| reshaped-from-main (this project's earlier tree) | 189 | 212,833 | 6.3% |  |
| fresh (written here from the seed and the class layout) | 163 | 18,563 | 0.5% |  |
| windows-workers (this project's own matching, done on its Windows machines) | 593 | 271,899 | 8.0% |  |
| mixed (template copies and header scans in files not adopted from one source) | 850 | 242,526 | 7.1% |  |
| total | 17,964 | 3,398,826 | 100.0% | 891 |

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
