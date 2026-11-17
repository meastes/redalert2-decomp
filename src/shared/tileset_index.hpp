// Isometric tile set start indices (set from the theater's tile sets;
// the cell and map predicates test IsoTileIndex against [start, start +
// count)). One declaration each, for shared/cell.hpp and shared/map.hpp.
#ifndef RA2_SHARED_TILESET_INDEX_HPP
#define RA2_SHARED_TILESET_INDEX_HPP

extern int IsometricTileTypeClass_WaterSet;
extern int IsometricTileTypeClass_WaterfallEast;
extern int unknown_tileset_a52590;
extern int IsometricTileTypeClass_GreenTile;
extern int IsometricTileTypeClass_Medians;
extern int unknown_tileset_a52c70;
extern int IsometricTileTypeClass_BridgeSet;
extern int IsometricTileTypeClass_BridgeBottomRight2;
extern int unknown_tileset_a52e64;
extern int IsometricTileTypeClass_CliffSet;
extern int unknown_tileset_a52ee8;
extern int unknown_tileset_a52eec;
extern int IsometricTileTypeClass_ClearTile;
extern int IsometricTileTypeClass_BridgeMiddle2;
extern int IsometricTileTypeClass_WaterfallSouth;
extern int unknown_tileset_a52ea8;
extern int unknown_tileset_a52ee4;
extern int IsometricTileTypeClass_WaterfallNorth;
extern int IsometricTileTypeClass_MiscPaveTile;
extern int unknown_tileset_a52efc;
extern int IsometricTileTypeClass_BridgeBottomLeft2;
extern int IsometricTileTypeClass_TrainBridgeSet;
extern int IsometricTileTypeClass_DirtRoadJunction;
extern int unknown_tileset_a6cb70;
extern int IsometricTileTypeClass_ShorePieces;
extern int IsometricTileTypeClass_BridgeMiddle1;
extern int IsometricTileTypeClass_WaterfallWest;
extern int IsometricTileTypeClass_CliffRamps;
extern int IsometricTileTypeClass_PavedRoadEnds;
extern int IsometricTileTypeClass_PavedRoads;
extern int IsometricTileTypeClass_BridgeBottomLeft1;
extern int IsometricTileTypeClass_DirtRoadCurve;
extern int IsometricTileTypeClass_RampStart;
extern int IsometricTileTypeClass_BridgeBottomRight1;
extern int IsometricTileTypeClass_SlopeSetPieces;
extern int IsometricTileTypeClass_PaveTile;
extern int IsometricTileTypeClass_DestroyableCliffs;

#ifdef RA2_CELL_RECALC_ATTRIBUTES
// The four tunnel tile sets (TibSun Tunnels, TrackTunnels, DirtTunnels, DirtTrackTunnels;
// CellClass Recalc_Attributes 0x00472540).
extern int unknown_tileset_a52ea4;
extern int unknown_tileset_a6cf50;
extern int unknown_tileset_a52f04;
extern int unknown_tileset_a6cb78;
#endif

#endif
