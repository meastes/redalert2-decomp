# S2 CellClass one-definition pilot (stopped 2026-11-17)

Short record of lane S2's pilot, copied here because worktrees get pruned. The draft is on branch
`lane-s2`, commit d836ea3ee "S2 CellClass one-definition pilot: stopped draft". It is **not exact and
must never land**. The lane's own note was `.claude/worktrees/lane-s2/build/s2/PILOT_NOTE.md`; its lab
script `build/s2/lab.py` (in that worktree) compiled the 125 affected compile groups and compared every
row (baseline 7,035 of 7,035 exact, 34 s).

Why it stopped: the owner redirected S2 to the mischa85 cost measurement, and then the user decided the
readable source will probably be mischa85/RedAlert2's tree, so one-definition class work in this tree
stopped (see HANDOFF "Stopped work").

What the draft did:
- `cell_class.hpp`: one unconditional CellClass, the union of every switch-selected declaration, with one
  form where they disagreed (Fetch_CellID returns Cell; Fixup_LAT returns bool; TibSun's Cell_Occupier
  ternary; `ushort[24]` at +0xAC; +0x12C and +0x140 kept as union word views with the YR/TibSun bit names,
  "option B").
- Removed 13 cell-only `RA2_` switches (cell.cpp, tactical, preview, blight, combat, smartdeform,
  hover/drive/ship, isotype, findpath2, Map Constructor.cpp, map_cell.hpp). Kept RA2_CELL_GOODIE_CHECK,
  RA2_CELL_RECALC_ATTRIBUTES, RA2_MAP_BRIDGE_PIECES/CLOSE_OBJECT and RA2_TACTICAL_PLACEMENT (other
  headers read them).
- aircraft.hpp's stand-in and startup.cpp's factory view replaced by `#include "shared/cell.hpp"`; Map
  aliases merged (unknown_473000 -> Destroy_Bridge, unknown_47a270 -> Is_Tile_Water, Unknown_004712c0
  -> Cell_Color).

Lab result (first run):
- 5 groups (585 rows) failed to compile: TibSun `operator++` templates clash with shared/incdec.hpp
  (reinf, tiberium, UnknownBuffer); display.cpp and the Map unit still call
  Unknown_00478760/Unknown_00471660 on CellClass (renames to Is_Clear_To_Move/Cell_Techno pending);
  tiberium.cpp picked the const Adjacent_Cell(FacingType) overload.
- Of the rows that compiled, 7 were not exact (22,407 bytes): ShipLocomotionClass::Process and
  DriveLocomotionClass::Process (Fetch_CellID by value instead of `Cell const &`), the
  declaration-count-fragile Print_CRCs 0x62A360 and AI_Build_Defense 0x4F1190 (4 bytes each), one cell
  row and combat's Explosion_Damage (Cell_Occupier if-form), and AircraftClass::Do_MISSION_ENTER
  (comparison refused: a reference). Not diagnosed further.

Not started: the cast views standing in for CellClass (UnknownCellClass, UnknownPlacementCellView,
UnknownAircraftCell, TerrainCellView and about ten more), the plain-bitfield option A, the native ctest,
the split-row count, HouseClass. Time used about 1 h of a 4 h estimate. Calibration used: no.

To resume: rebase `lane-s2` onto main, fix the 5 compile failures first (the two pending renames and the
incdec clash), then diagnose the 7 rows with the lab; the fragile rows fall under DECISIONS "Module
conversion adopts template headers and a declaration-count tie row flips".
