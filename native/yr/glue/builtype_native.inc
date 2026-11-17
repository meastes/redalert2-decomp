
// ---------------------------------------------------------------------------------------------
// NON-EXACT NATIVE DRAFTS (lane Y3): functions unmatched in the YR tree, the text TibSun's
// (TibSun archive code/builtype.cpp; SPDX GPL-3.0-or-later, Copyright 2025 Electronic Arts Inc. and
// OpenTS contributors, EA's GPL section 7 terms; THIRD_PARTY.md); native changes are marked
// NATIVE. Behaviour code for the native build; never part of an exact build.
// ---------------------------------------------------------------------------------------------

Rect BuildingTypeClass::Get_Draw_Rect(void)
{
	int i;

	if (DrawRect != RECT_NONE) {
		return(DrawRect);
	}

	ShapeSet const * shape = (ShapeSet const *)Get_Image_Data();
	if (shape == NULL) {
		return(RECT_NONE);
	}

	int frames = shape->Get_Count();
	Rect rect = RECT_NONE;
	int width = shape->Get_Width();
	int height = shape->Get_Height();

	for (i = 0; i < frames; i++) {
		rect = Union(rect, shape->Get_Rect(i));
	}

	bool free_buildup = (BuildupData == NULL);
	ShapeSet const * buildup = (ShapeSet const *)Get_Buildup_Data();

	if (buildup != NULL) {
		frames = buildup->Get_Count();
		for (i = 0; i < frames; i++) {
			rect = Union(rect, buildup->Get_Rect(i));
		}
		if (free_buildup) {
			Free_Buildup_Data();
		}
	}

	if (BibShape != NULL) {
		Rect bibrect = BibShape->Get_Rect(0);
		rect = Union(rect, bibrect);
	}

	int x = rect.X - width / 2;
	int y = rect.Y - height / 2;
	DrawRect = Rect(x, y, rect.Width, rect.Height);

	return(DrawRect);
}
