# Native tree: coupling to this repository's source layout

> **Moved (2026-11-25):** `native/` now lives in github.com/meastes/redalert2-modern (layout move, lane O88); this file is kept as the record of the coupling before the move.

**Since 2026-11-24** `rebuild/` is the exact-match tree of record and `src/` is frozen at the tag
`src-final`. The native ports keep building from `src/recovered` and `src/shared` unchanged (native/build.sh,
native_units.py; `tree.py`'s default stays `main` until the native tools pin `RA2_TREE=main`, step 13 of the
switch). The move onto the rebuild (docs/REBUILD_HANDOVER_PLAN.md plan 2) runs on branch `ports-on-rebuild`
(`native/rb/`: a pinned `rebuild/` commit in `native/rb/PIN` plus `native/rb/fixups.json`); nothing of it lands
on main until it passes what the src-based port passes, and a separate ports repository consuming the rebuild
at a pinned commit is the user's decision. The counts below are lane N1's of 2026-11-17.

What in `native/` would have to be redone if the ports were moved onto a different source base
(another decompilation of the same game.exe, with its own files, headers and names). Counted by
lane N1 on 2026-11-17 at main d8d398b7f plus lane N1's commits. Each line says what the item is
keyed on:

- **address**: a catalog/original address (0x...). Portable: any base for the same game.exe has
  the same addresses.
- **name**: a function, class, member or global name. Portable only through a name map
  (ours -> theirs); placeholder names (`unknown_*`, `Unknown_*`, `*_<hex address>`, `seed_*`,
  `FUN_*`) have no counterpart elsewhere and must be mapped by address.
- **path**: a file under `src/recovered` or `src/shared`, or a compile group of our catalog.
  Not portable: the other base has other files.
- **view**: one of our view headers or per-function owner types (`map_view.hpp`,
  `unknown_globals.hpp`, `Unknown*` structs standing in for real classes). Not portable.
- **draft**: hand-written C++ against our headers. Portable only by rewriting against the other
  base's headers (or replaced by that base's own matched function where it has one).

Placeholder counts use the pattern `[Uu]nknown_|_<6-8 hex digits>|FUN_|seed_`.

## Summary

| Item | Size | Keyed on | Of which placeholder / path-keyed |
|---|---|---|---|
| `tools/native_units.py` (the native view generator) | 976 lines, 24 `RULES` regexes, catalog compile groups | path (catalog groups, `src/` subsystems), name (rules) | 11 `src/` path literals, 6 header names, 12 placeholder identifiers |
| `port/fixups.json` | 499 text fixups over 155 source files, 1 rename | path + exact source text | all 499 keyed to a `src/` file (394 recovered, 105 shared); 257 spell placeholder names |
| `port/view_layouts.json` | 46 view layouts | path + view | all 46 keyed to a `src/` file and a view type |
| `link/*.cpp` (behaviour rewrites, harness, adapters) | 176 files, 15,250 lines | name + view (draft) | 639 includes of `src/shared` headers (200 distinct); 974 distinct placeholder identifiers; 504 address comments; 58 `src/` paths in comments; 64 files define placeholder-named symbols |
| `drafts/*.cpp` + `drafts.json` | 83 files, 14,198 lines; 84 rows | draft + path (host group) + address | 565 shared-header includes, 738 distinct placeholder identifiers; 60 of 84 rows name a `host_source` (`src/` file whose group the draft joins); 62 row names are placeholders |
| `behaviour_rewrites.json` | 311 rows | address (301 rows carry one) + name | 138 placeholder names, 43 placeholder symbols, 86 cite a `src/` file as source |
| `initterm.json` | 3,610 initialiser entries | address + name + path | all 3,610 name a `src/` source file |
| `data/address_aliases.tsv` | 488 rows | name -> name, with address | 400 rows placeholder |
| `data/bss_aliases.tsv` | 68 rows | name -> name, with address | 62 placeholder |
| `data/header_statics.tsv` | 47 rows | name + address | 44 placeholder |
| `data/member_aliases.tsv` | 237 rows | name (global -> owner member, offsets) | 227 placeholder |
| `data/owner_sizes.tsv` | 502 rows | name + type | 218 placeholder |
| `data/unsourced_initialised.tsv` | 78 rows | address + name + path | 74 placeholder, 77 name a `src/` user file |
| `data/vtable_views.tsv` | 44 rows | path (native file) + view class | 42 path-keyed, 17 placeholder |
| `tools/original_layouts.py` + `data/original_save_types.txt` (save layer) | 76 types | name (class names as the port spells them, member paths from our headers) | 1 placeholder type; the field maps pair members by their path in our headers, so every member name and layout of those 76 types is coupled |
| `port/src/original_save.cpp` | run-time class lookup by name | name (type strings such as `UnitClass`, `DynamicVectorClass<int>`) | the names of the 76 save types |
| `tools/data_globals.py` | globals bound to addresses | address, via the catalog's reference tables | catalog-keyed (our catalog format) |
| other tools | `view_audit.py` (9 header names, 2 paths), `vtable_views.py` (4 paths), `member_aliases.py` (2 paths, 3 headers), `original_data.py` (3 paths), `fast_math_tables.py` (1 path, 13 placeholders), `name_rules_check.py` (checks name-keyed rules against `src/`) | path/name | as listed |

## What moves cheaply

- Everything keyed on **addresses**: the behaviour rewrite rows (301 of 311), drafts' entries,
  initterm entries, the alias tables' address column, data_globals, unsourced_initialised. These
  re-bind to another base by address.
- The platform layer (`platform/`, `port/src`): it implements the original's imports (Win32,
  DirectX, CRT, Bink) under their own names and includes no `src/shared` header.

## What has to be redone

1. `native_units.py` and `port/fixups.json`: the 499 fixups are text substitutions in our files;
   each would become a fixup on the other base's text (or disappear where that base already
   compiles portably). The 24 `RULES` and the catalog compile-group logic are specific to our
   catalog.
2. `port/view_layouts.json` (46) and `data/vtable_views.tsv` (44): our view types; none exists
   elsewhere.
3. The hand-written C++ in `link/` and `drafts/` (29,448 lines together) uses our headers and
   placeholder names throughout: 1566 distinct placeholder identifiers between them. Where the
   other base has the function matched, its own text replaces the draft; elsewhere the draft is
   ported by address and renamed.
4. The name-keyed tables (aliases, member aliases, owner sizes, header statics): about 1,000
   rows keyed on placeholder names, which need an address-to-name map to translate.
5. The save layer: the 76 save types' layouts are read from our headers; on another base the
   field maps are regenerated from its headers (the tool is generic; the type list and member
   paths are ours).

## Rule for new port work (coordinator, 2026-11-17)

New port work keys on function names or catalog addresses, never on our file paths or view
headers: a new rewrite row carries its entry address and real name; a new fixup or rule names the
function (and address) it changes rather than relying on the file it sits in; no new view headers.
