// ---------------------------------------------------------------------------------------------
// NON-EXACT NATIVE STAND-IN (lane N5), appended to the line-trail unit: LineTrail::LineTrail (gamemd.exe
// 0x00556A20, 164 bytes), unmatched in the YR tree. Written from gamemd.exe's instructions (yr_dis.py),
// not compared byte for byte. Behaviour code for the native build; never part of an exact build.
//
// ObjectClass::Unlimbo makes one for a type with UseLineTrail=yes (a link-closure trap before).
// ---------------------------------------------------------------------------------------------

LineTrail::LineTrail(void) :
	Color(0x80, 0x80, 0x80),
	Owner(NULL),
	Decrement(16),
	ActiveSlot(0)
{
	for (int i = 0; i < 32; i++) {
		Trails[i].Position = Coord(0, 0, 0);	// the unit's zero coordinate (gamemd.exe 0x00ABCB50)
		Trails[i].Value = 0;
	}
	LineTrailArray.Add(this);
}
