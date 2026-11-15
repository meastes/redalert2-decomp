
// ---------------------------------------------------------------------------------------------
// NON-EXACT NATIVE DRAFTS (lane Y3): functions unmatched in the YR tree, the text TibSun's
// (TibSun archive code/alphashp.cpp; SPDX GPL-3.0-or-later, Copyright 2025 Electronic Arts Inc. and
// OpenTS contributors, EA's GPL section 7 terms; THIRD_PARTY.md); native changes are marked
// NATIVE. Behaviour code for the native build; never part of an exact build.
// ---------------------------------------------------------------------------------------------

void AlphaShapeClass::Draw_All(Rect const & cliprect)
{
	for (int i = 0; i < AlphaShapes.Count(); i++) {
		{
			Rect r1 = AlphaShapes[i]->DrawRect;
			r1 -= Point2D(TacticalMap->TacPixelX, TacticalMap->TacPixelY);
			r1 += TacticalRect.Top_Left();

			Rect clip = cliprect;
			Rect r2 = Intersect(r1, clip);

			if (r2.Is_Valid()) {
				int x1 = r1.X;
				int y1 = r1.Y;

				SHPStruct const * shape = AlphaShapes[i]->ImageData;	// NATIVE: YR's SHPStruct for TibSun's ShapeSet
				if (shape == NULL) {
					AlphaShapes[i]->ImageData = (SHPStruct const *)AlphaShapes[i]->Owner->Class_Of()->AlphaImageData;
					shape = AlphaShapes[i]->ImageData;
				}
				Rect shape_rect = shape->GetFrameBounds(0);

				int top = MAX(r2.Y, shape_rect.Y + y1);
				int src_y = top - shape_rect.Y - y1;
				int bottom = MIN(clip.Y + clip.Height, shape_rect.Y + shape_rect.Height + y1);
				int left = MAX(r2.X, shape_rect.X + x1);
				int src_x = left - shape_rect.X - x1;
				int right = MIN(clip.X + clip.Width, shape_rect.X + shape_rect.Width + x1);

				int shape_skip = shape_rect.Width - right + shape_rect.X + x1 + src_x;
				int alpha_skip = AlphaBuffer->Get_Buffer_Width() - right + left;

				unsigned char * shapedata = (unsigned char *)shape->GetPixels(0);

				unsigned short * alphaptr = (unsigned short *)AlphaBuffer->Get_Buffer_Offset(Point2D(left - TacticalRect.X, top - TacticalRect.Y));

				unsigned char * shapeptr = (unsigned char *)&shapedata[src_x + src_y * shape_rect.Width];
				if (&alphaptr[right - left + (bottom - top) * AlphaBuffer->Get_Buffer_Width() + 2] >= (unsigned short *)AlphaBuffer->Get_Buffer_End()) {
					for (int i = top; i < bottom; i++) {
						for (int j = left; j < right; j++) {
							unsigned char pixel = *shapeptr++;
							*alphaptr = BrightnessTable[pixel][*alphaptr];
							alphaptr++;
							alphaptr = (unsigned short *)AlphaBuffer->Wrap_Overflow((uintptr_t)alphaptr);
						}
						shapeptr += shape_skip;
						alphaptr += alpha_skip;
						alphaptr = (unsigned short *)AlphaBuffer->Wrap_Overflow((uintptr_t)alphaptr);
					}
				} else {
					for (int i = top; i < bottom; i++) {
						for (int j = left; j < right; j++) {
							unsigned char pixel = *shapeptr++;
							*alphaptr = BrightnessTable[pixel][*alphaptr];
							++alphaptr;
						}
						shapeptr += shape_skip;
						alphaptr += alpha_skip;
					}
				}
			}
		}
	}
}
