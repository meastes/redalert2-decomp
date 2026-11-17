# Header-static initialisers by unit

Each folder holds the dynamic initialisers (and a few compiler-generated members) that one original
translation unit emits for the file statics of the shared headers it includes (unknown_geometry.hpp's
angles and rects, the type lists). The folder is named after that unit. The original address of the
unit's first initialiser is given for reference; the catalog keys every row by its own address.

Evidence for the unit: "unit map" = manifests/original_target/unit_map.json names the rows;
".CRT$XCU table" = docs/CRT_XCU_TABLE.md / tools/original_target/xcu_units.py; "population" = the
rows compile in one population with that unit's module rows. Three units are not in the unit map
(hidden units, docs/CRT_XCU_TABLE.md); their folders say where they sit in link order.

| Folder | First initialiser | Evidence |
| --- | --- | --- |
| after_map | 0x0056CEF0 | a unit the unit map does not list (hidden unit, docs/CRT_XCU_TABLE.md): directly after map |
| after_warhead | 0x0040F720 | a unit the unit map does not list (hidden unit, docs/CRT_XCU_TABLE.md): after _warhead, before _winfix; its population also holds the WeaponTypeClass list initialiser |
| aitrig | 0x0041C3D0 | .CRT$XCU table (xcu-exclusion) and the population shares aitrig's module rows |
| _alpha | 0x00406900 | unit map |
| _astar | 0x0040ACB0 | unit map |
| _astar2 | 0x0040B280 | unit map |
| before_mapgen | 0x0056D0E0 | a second unlisted unit (one unit cannot instantiate these statics twice): directly before mapgen |
| capturemanager | 0x00467B40 | population shared with capturemanager's rows (CaptureManagerClass list); .CRT$XCU candidates include it |
| cargo | 0x00468770 | population shared with cargo's module rows |
| coopcamp | 0x0048DC70 | unit map |
| egos | 0x004B35A0 | unit map |
| facing | 0x004B7A00 | unit map |
| findpath | 0x004BB160 | population shared with findpath's row (FootClass::Find_Path) |
| findpath2 | 0x004BB6A0 | population shared with findpath2's module rows |
| globals | 0x004D2BA0 | unit map |
| isotype | 0x00528380 | .CRT$XCU table (xcu-exclusion) and the population shares isotype's module rows |
| keyboard | 0x00531F30 | unit map |
| _logic | 0x0040C4A0 | its one row is LogicClass's scalar deleting destructor (0x0040CB10), right after _logic's rows in _logic's population |
| milsectmr | 0x00593340 | unit map |
| mpcoop | 0x005A2610 | unit map |
| mpsiege | 0x005AB3B0 | .CRT$XCU table (xcu-exclusion) and the population shares mpsiege's module rows |
| netdlg2 | 0x005BBB20 | unit map |
| ovrlight | 0x005DD320 | .CRT$XCU table (xcu-exclusion) and the population shares ovrlight's module rows |
| partsys | 0x0060A880 | .CRT$XCU table (xcu-exclusion) and the population shares partsys's module rows |
| radbeam | 0x006351B0 | .CRT$XCU table (xcu-exclusion) and the population shares radbeam's module rows |
| revent | 0x0063B300 | unit map |
| _rtti | 0x0040D950 | population shared with _rtti's module rows; .CRT$XCU candidates _rect/_rtti |
| _script | 0x0040E3C0 | unit map |
| sendfile | 0x0066ACB0 | population shared with sendfile's rows |
| teleport | 0x006DFAB0 | .CRT$XCU table (xcu-exclusion) and the population shares teleport's module rows |
| terrtype | 0x006E4B70 | .CRT$XCU table (xcu-exclusion) and the population shares terrtype's module rows |
| txtlabel | 0x006F13D0 | unit map |
| txtlabel2 | 0x006F1810 | unit map (12 of 13 rows; the unit map gives the 13th to txtlabel) |
| voxdrsys | 0x00716D80 | unit map |
| _voxel | 0x0040EA30 | between _timer and _voxel's first function (units begin with their initialisers); the population's other row is the VoxelAnimClass list initialiser |
| _warhead | 0x0040F0A0 | population shared with _warhead's module rows |
| wdtmap | 0x00728FD0 | .CRT$XCU table (xcu-exclusion) and the population shares wdtmap's module rows |
