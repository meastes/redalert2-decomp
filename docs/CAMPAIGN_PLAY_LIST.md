# Red Alert 2 campaign: missions to play by hand

Lane N2, 2026-11-17. Which base-game missions the native port has been won in a real game, and
which are left for a person to play. Real campaign wins are the user's (AGENTS.md, 2026-11-17).

## What automation has done

- **Forced wins, all 24 missions:** ctests `native-campaign-allied` and `native-campaign-soviet`
  start each side's campaign from the menus, end each mission as a win with
  `RA2_NATIVE_FORCE_END`, go through every score screen and both endings and back to the main
  menu (Mac, and the Windows build under Wine). This proves the chain (briefings, score screens,
  next-mission saves, the ending movies) but plays nothing.
- **All 24 start and play:** each mission started and ran about 5,000 frames with a save and
  load (lane C3 start sweep, 2026-11-12; C17w audit 2026-11-15).
- **Won in a real game (scripted orders, Mac):** 9 of 24.

| Mission | Map | How it was won | Record |
|---|---|---|---|
| Allied 1 | ALL01T | nukes on the Soviet base via `RA2_NATIVE_ORDERS super 0`, Tanya kept alive | r17 0cb047cbb, r39 4c7771d86 (from the menus) |
| Allied 2 | ALL02S | scripted order set plus nukes; chapel captured, Tanya alive | r40 c3bb43b90 |
| Allied 5 | ALL05S | both missile silos destroyed, frame 847 from its save | r42 7b23ba06c |
| Allied 6 | ALL06U | from its save, frame 8,725 | r43b bde4e1367 |
| Allied 7 | ALL07T | from its save, frame 621 | r43b bde4e1367 |
| Allied 8 | ALL08U | from its save, frame 6,941 | r43b bde4e1367 |
| Allied 9 | ALL09T | from its save, frame 16,221 | r43b bde4e1367 |
| Soviet 1 | SOV01T | scripted; also won after a save and reload in a fresh process | r17 0cb047cbb, r42 7b23ba06c |
| Soviet 2 | SOV02T | from its save, frame 5,221 | r42 7b23ba06c |

These wins used test scaffolding (scripted orders, granted super weapons), so none of them is a
normal playthrough either.

## Missions for the user to play (15)

No automated real win exists for these. Each needs a person to play it to its win screen in the
port. Each starts from the menus, or from the previous mission's start save
(`ALLnnX.sav` / `SOVnnX.sav`) that the score screen's Continue writes.

| # | Mission | Map | Why automation has not won it |
|---|---|---|---|
| 1 | Allied 3 | ALL03U | never attempted with scripted orders (start save ALL03U.sav exists) |
| 2 | Allied 4 | ALL04U | never attempted |
| 3 | Allied 10 | ALL10S | never attempted |
| 4 | Allied 11 | ALL11T | never attempted |
| 5 | Allied 12 (final) | ALL12S | never attempted; the Allied ending has only been reached by the forced chain |
| 6 | Soviet 3 | SOV03U | tried three times (lane C3, 2026-11-12): the objective is a unit entering the Allied Battle Lab across the map, and the scripted engineers died to the lab garrison (15-37 units). Owner ruling: a tactics problem, not a port defect |
| 7 | Soviet 4 | SOV04S | never attempted |
| 8 | Soviet 5 | SOV05U | never attempted |
| 9 | Soviet 6 | SOV06T | never attempted |
| 10 | Soviet 7 | SOV07S | never attempted |
| 11 | Soviet 8 | SOV08U | never attempted |
| 12 | Soviet 9 | SOV09U | never attempted |
| 13 | Soviet 10 | SOV10T | never attempted |
| 14 | Soviet 11 | SOV11S | never attempted |
| 15 | Soviet 12 (final) | SOV12S | never attempted; the Soviet ending has only been reached by the forced chain |

Suggested order, highest value first: the two finals (Allied 12 and Soviet 12, because no
ending has been reached in real play anywhere), then Soviet 3 (the only mission automation tried
and failed), then the rest in campaign order. Worth also playing one mission each on the iPhone and
on Windows: so far those platforms have played only saves and skirmishes.

"Never attempted" means only that no scripted run tried it. It does not mean the mission is hard
to script. Super weapon orders (`super <index> <x> <y>`) could likely win several of the
destroy-the-base missions. That would be lane work, and it would still not be a normal
playthrough.
