// ---------------------------------------------------------------------------------------------
// NON-EXACT NATIVE DRAFT (lane Y6): AircraftTrackerClass::FillCurrentVector (gamemd.exe 0x00412B40,
// 2,373 B), unmatched in ra2-decomp; written from gamemd.exe's code. The tracker splits the map
// into 20 x 20 regions; this collects into CurrentVector the flying objects of every region
// within range (in cells, at least 1) of the cell: the cell's own region, then the four arms
// (east, north, south, west: from the far region back toward the centre), then the four
// quadrants between them. Behaviour code for the native build only.
// ---------------------------------------------------------------------------------------------
void AircraftTrackerClass::FillCurrentVector(CellClass * cell, int range)
{
	if (range < 1) range = 1;
	Cell here = cell->Center_Coord().As_Cell();
	int center = GetVectorIndex(here);
	int cx = center % 20;
	int cy = center / 20;
	FUN_004129C0(center);

	int east = GetVectorIndex(Cell(here.X + range, here.Y)) % 20 - cx;
	int north = cy - GetVectorIndex(Cell(here.X, here.Y - range)) / 20;
	int south = GetVectorIndex(Cell(here.X, here.Y + range)) / 20 - cy;
	int west = cx - GetVectorIndex(Cell(here.X - range, here.Y)) % 20;

	int i, j;
	for (i = east; i > 0; i--) FUN_004129C0(center + i);
	for (j = north; j > 0; j--) FUN_004129C0(center - 20 * j);
	for (j = south; j > 0; j--) FUN_004129C0(center + 20 * j);
	for (i = west; i > 0; i--) FUN_004129C0(center - i);

	for (i = 1; i <= east; i++) {
		for (j = 1; j <= north; j++) FUN_004129C0(FUN_00412A40(center, Point2D(i, -j)));
		for (j = 1; j <= south; j++) FUN_004129C0(FUN_00412A40(center, Point2D(i, j)));
	}
	for (i = 1; i <= west; i++) {
		for (j = 1; j <= north; j++) FUN_004129C0(FUN_00412A40(center, Point2D(-i, -j)));
		for (j = 1; j <= south; j++) FUN_004129C0(FUN_00412A40(center, Point2D(-i, j)));
	}
}
