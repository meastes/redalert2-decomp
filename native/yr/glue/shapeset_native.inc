
// ---------------------------------------------------------------------------------------------
// NON-EXACT NATIVE DRAFT (lane Y3): ShapeSet::Get_Color is unmatched in the YR tree (its header
// declares it out of line). The text is TibSun's inline (TibSun archive code/shapeset.h;
// SPDX GPL-3.0-or-later, Copyright 2025 Electronic Arts Inc. and OpenTS contributors, EA's GPL
// section 7 terms; THIRD_PARTY.md). Behaviour code for the native build; never part of an exact
// build.
// ---------------------------------------------------------------------------------------------
RGBClass ShapeSet::Get_Color(int shape) const
{
	ShapeRecord const * record = Fetch_Record_Pointer(shape);
	if (record != NULL) {
		return(RGBClass(record->Color[0], record->Color[1], record->Color[2]));
	}
	return(RGBClass(0, 0, 0));
}
