# Pinned Yuri's Revenge source (native YR port, lane Y1)

User decision (2026-11-15, AGENTS.md): the native Yuri's Revenge port is built from
ra2-decomp's tree, pinned here and never edited. Native-only changes live in `native/yr/`
(text fixups applied to generated copies, glue, the SDK layer); `src/` and the RA2 exact build
are untouched.

| Copy | Source | Commit | Licence |
| --- | --- | --- | --- |
| `ra2-decomp/` | gibbo101/ra2-decomp (local clone `private/reference/ra2-decomp`) | `8719494727dd9d5db16f92087e3cc13ea99f4dd3` (2026-11-15; moved from `f8d8d53` by lane Y3) | `src/`, `include/`: GPL-3.0-or-later (`ra2-decomp/COPYING.md`), EA's section 7 terms on files that carry EA or OpenTS material; `data/` (the function map, manifest and names used by the native pipeline): MIT (`ra2-decomp/LICENSE`) |
| `tibsun/` | OpenTS-Developers/TibSun (local clone `private/reference/TibSun`), headers only (`code/**/*.h`), which ra2-decomp's build falls back on | `dbfbfbfaac40364ce7e1cd8fad7bcade5c6fb0d4` (2026-08-27; ra2-decomp's pinned `TIBSUN_COMMIT`) | GPL-3.0-or-later with EA's section 7 terms (`tibsun/LICENSE.md`) |

Copied with `git archive` at those commits; nothing was downloaded. To move the pin, archive the
new commit over these folders in one change and record it here and in `THIRD_PARTY.md`.
