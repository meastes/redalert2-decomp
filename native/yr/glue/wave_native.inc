
// ---------------------------------------------------------------------------------------------
// NON-EXACT NATIVE DRAFT (lane Y15), appended to the wave unit: WaveClass::Draw_Sonic (gamemd.exe (between 0x0075F9F0 and 0x00760190)), unmatched in
// ra2-decomp. Red Alert 2's text of the same routine (mischa85/RedAlert2 src/wave.cpp, game.exe (its Draw_Sonic); SPDX
// GPL-3.0-or-later, Copyright 2026 OpenTS contributors, LICENSE.md's terms; THIRD_PARTY.md), with the
// YR tree's names where they differ (marked NATIVE). Yuri's Revenge's routine is not compared with it
// instruction by instruction. Behaviour code for the native build only.
// ---------------------------------------------------------------------------------------------
void WaveClass::Draw_Sonic(Point2D const & point, Rect const & cliprect)
{
	int zpix = Tactical::Z_Lepton_To_Pixel(StartCoord.Z);

	unsigned short * surfptr = (unsigned short *)LogicalSurface->Lock();
	if (surfptr != NULL) {

		int top = cliprect.Y + 3;
		int bottom = cliprect.Y + cliprect.Height - 3;
		int left = cliprect.X + 3;
		int right = cliprect.X + cliprect.Width - 3;

		int xoff = point.X - WaveStartMiddle.X;
		int yoff = point.Y - WaveStartMiddle.Y;

		Rasterize_Polygon(WaveShape, DrawData);

		if (DrawData.Points != NULL) {

			int starty = yoff + TacticalRect.Y + DrawData.BaseY;
			int endy = MIN(DrawData.Count + starty - 1, cliprect.Height + cliprect.Y);

			Init_Offset_Tables();

			if (Direction > FACING_NE && Direction < FACING_W) {
				unsigned short base_z = DepthBuffer->Get_Scroll_Delta(zpix);
				unsigned short zval = base_z - starty - 2;
				unsigned int zoffset = DepthBuffer->Get_Buffer_Offset(Point2D(0, starty - TacticalRect.Y));
				int width = LogicalSurface->Get_Width();
				int zwidth = DepthBuffer->Get_Buffer_Width();

				if (zoffset + 2 * (width + zwidth * (endy - starty + 1)) < DepthBuffer->Get_Buffer_End()) {

					int stride = LogicalSurface->Stride() >> 1;
					unsigned short * dest = surfptr + starty * stride;
					Point2D * points = DrawData.Points;

					for (int y = starty; y <= endy; y++) {
						if (y >= top && y <= bottom) {
							int xstop = xoff + points->Y;
							if (xstop >= right) {
								xstop = right;
							}
							int xstart = left;
							if (points->X + xoff > left) {
								xstart = points->X + xoff;
							}

							zoffset += 2 * xstart;
							dest += xstart;

							int x;
							int ypos = abs(y - yoff - WaveStartMiddle.Y);
							for (x = xstart; x <= xstop; x++) {
								if (*(unsigned short *)zoffset > zval) {
									Set_Sonic_Pixel(x, xoff, ypos, dest);
								}
								zoffset += 2;
								dest++;
							}
							zoffset -= 2 * x;
							dest -= x;
						}
						dest += stride;
						zoffset += 2 * zwidth;
						zval--;
						points++;
					}

				} else {

					int index = 0;
					for (int y = starty; y <= endy; y++) {
						if (y >= cliprect.Y + 2 && y <= cliprect.Y + cliprect.Height - 2) {
							Point2D * entry = &DrawData.Points[index];
							int xstop = xoff + entry->Y;
							if (xstop >= cliprect.X + cliprect.Width - 2) {
								xstop = cliprect.X + cliprect.Width - 2;
							}
							int xstart = cliprect.X + 2;
							if (entry->X + xoff > cliprect.X + 2) {
								xstart = entry->X + xoff;
							}

							int xpos = xstart;
							unsigned short * dest = surfptr + xstart + y * (LogicalSurface->Stride() >> 1);
							int zy = y - TacticalRect.Y;
							int ypos = abs(y - yoff - WaveStartMiddle.Y);
							for (int x = xstart; x <= xstop; x++) {
								if (*(unsigned short *)DepthBuffer->Get_Buffer_Offset(Point2D(xpos, zy)) > zval) {
									Set_Sonic_Pixel(x, xoff, ypos, dest);
								}
								xpos++;
								dest++;
							}
						}
						zval--;
						index++;
					}
				}

			} else {

				unsigned int zoffset = DepthBuffer->Get_Buffer_Offset(Point2D(0, starty - TacticalRect.Y));
				int width = LogicalSurface->Get_Width();
				int rows = endy - starty;
				int zwidth = DepthBuffer->Get_Buffer_Width();

				if (zoffset + 2 * (width + zwidth * (rows + 1)) < DepthBuffer->Get_Buffer_End()) {

					int stride = LogicalSurface->Stride() >> 1;
					unsigned short * dest = surfptr + endy * stride;
					Point2D * points = &DrawData.Points[rows];
					unsigned short base_z = DepthBuffer->Get_Scroll_Delta(endy);
					unsigned short zval = base_z - zpix - 2;
					unsigned short * zptr = (unsigned short *)DepthBuffer->Get_Buffer_Offset(Point2D(0, endy - TacticalRect.Y));

					for (int y = endy; y >= starty; y--) {
						if (y >= top && y <= bottom) {
							int xstart = xoff + points->Y;
							if (xstart >= right) {
								xstart = right;
							}
							int xstop = xoff + points->X;
							if (xstop <= left) {
								xstop = left;
							}

							dest += xstart;
							zptr += xstart;

							int x;
							int ypos = abs(y - yoff - WaveStartMiddle.Y);
							for (x = xstart; x >= xstop; x--) {
								if (*zptr > zval) {
									Set_Sonic_Pixel(x, xoff, ypos, dest);
								}
								zptr--;
								dest--;
							}
							zptr -= x;
							dest -= x;
						}
						dest -= stride;
						zval++;
						points--;
						zptr -= zwidth;
					}

				} else {

					unsigned short base_z = DepthBuffer->Get_Scroll_Delta(endy);
					unsigned short zval = base_z - zpix - 2;
					int index = rows;
					for (int y = endy; y >= starty; y--) {
						if (y >= cliprect.Y + 2 && y <= cliprect.Y + cliprect.Height - 2) {
							Point2D * entry = &DrawData.Points[index];
							int xstart = entry->Y + xoff;
							if (xstart >= cliprect.X + cliprect.Width - 2) {
								xstart = cliprect.X + cliprect.Width - 2;
							}
							int xstop = xoff + entry->X;
							if (xstop <= cliprect.X + 2) {
								xstop = cliprect.X + 2;
							}

							unsigned short * dest = surfptr + xstart + y * (LogicalSurface->Stride() >> 1);
							int zy = y - TacticalRect.Y;
							int xpos = xstart;
							int ypos = abs(y - yoff - WaveStartMiddle.Y);
							for (int x = xstart; x >= xstop; x--) {
								if (*(unsigned short *)DepthBuffer->Get_Buffer_Offset(Point2D(xpos, zy)) > zval) {
									Set_Sonic_Pixel(x, xoff, ypos, dest);
								}
								xpos--;
								dest--;
							}
						}
						zval++;
						index--;
					}
				}
			}
		}

		LogicalSurface->Unlock();

		if (DrawData.Points != NULL) {
			delete [] DrawData.Points;
			DrawData.Points = NULL;
		}
	}
}


// ---------------------------------------------------------------------------------------------
// NON-EXACT NATIVE DRAFT (lane Y15), appended to the wave unit: WaveClass::Draw_Laser (gamemd.exe (after 0x007602E0)), unmatched in
// ra2-decomp. Red Alert 2's text of the same routine (mischa85/RedAlert2 src/wave.cpp, game.exe (its Draw_Laser); SPDX
// GPL-3.0-or-later, Copyright 2026 OpenTS contributors, LICENSE.md's terms; THIRD_PARTY.md), with the
// YR tree's names where they differ (marked NATIVE). Yuri's Revenge's routine is not compared with it
// instruction by instruction. Behaviour code for the native build only.
// ---------------------------------------------------------------------------------------------
void WaveClass::Draw_Laser(Point2D const & point, Rect const & cliprect)
{
	if (Options.DetailLevel == 2) {
		int zpix = Tactical::Z_Lepton_To_Pixel(StartCoord.Z);

		unsigned short * buffer = (unsigned short *)LogicalSurface->Lock();
		if (buffer != NULL) {
			int top = cliprect.Y;
			int bottom = cliprect.Y + cliprect.Height;
			int left = cliprect.X;
			int right = cliprect.X + cliprect.Width;

			int xoff = point.X - WaveStartMiddle.X;
			int yoff = point.Y - WaveStartMiddle.Y + TacticalRect.Y;

			Rasterize_Polygon(WaveShape, DrawData);

			if (DrawData.Points != NULL) {
				int ystart = yoff + DrawData.BaseY;
				int yend = MIN(DrawData.Count + ystart - 1, cliprect.Y + cliprect.Height);

				unsigned short base_z = DepthBuffer->Get_Scroll_Delta(ystart); // RA2: the executable takes the depth from the first row, clamps with MIN/MAX, and walks the wrapped spans with a counted do-while
				unsigned short depth = base_z - zpix - 2; // RA2: as above
				unsigned int zoff = DepthBuffer->Get_Buffer_Offset(Point2D(0, ystart - TacticalRect.Y));
				int y; // RA2: as above
				int surfwidth = LogicalSurface->Get_Width();

				if (zoff + 2 * (surfwidth + (yend - ystart + 1) * DepthBuffer->Get_Buffer_Width()) < DepthBuffer->Get_Buffer_End()) { // RA2: as above
					int zwidth = DepthBuffer->Get_Buffer_Width(); // RA2: as above
					int stride = LogicalSurface->Stride() >> 1;
					unsigned short * scrptr = &buffer[ystart * stride];
					Point2D * points = DrawData.Points;

					for (y = ystart; y <= yend; y++) { // RA2: as above
						if (y >= top && y <= bottom) {
							int xstop = MIN(points->Y + xoff, right); // RA2: as above
							int xstart = MAX(points->X + xoff, left); // RA2: as above

							zoff += 2 * xstart;
							scrptr += xstart;

							int x = xstart;
							for (; x <= xstop; x++) {
								if (*(unsigned short *)zoff > depth) {
									Set_Laser_Pixel(scrptr, LaserEC);
								}
								zoff += 2;
								scrptr++;
							}

							zoff -= 2 * x;
							scrptr -= x;
						}
						scrptr += stride;
						zoff += 2 * zwidth;
						depth--;
						points++;
					}
				} else {
					int i = 0;
					for (y = ystart; y <= yend; y++, i++) { // RA2: as above
						if (y >= cliprect.Y + 2 && y <= cliprect.Y + cliprect.Height - 2) {
							Point2D const & span = DrawData.Points[i]; // RA2: as above
							int xstop = MIN(span.Y + xoff, cliprect.Width + cliprect.X - 2); // RA2: as above
							int xstart = MAX(span.X + xoff, cliprect.X + 2); // RA2: as above

							unsigned short * scrptr = &buffer[xstart + y * (LogicalSurface->Stride() >> 1)];
							Point2D zpos(xstart, y - TacticalRect.Y); // RA2: as above

							if (zpos.X <= xstop) { // RA2: as above
								int remaining = xstop - zpos.X + 1; // RA2: as above
								do { // RA2: as above
									if (*(unsigned short *)DepthBuffer->Get_Buffer_Offset(zpos) > depth) { // RA2: as above
										Set_Laser_Pixel(scrptr, LaserEC); // RA2: as above
									}
									zpos.X++; // RA2: as above
									scrptr++; // RA2: as above
								} while (--remaining != 0); // RA2: as above
							}
						}
						depth--;
					}
				}
			}

			LogicalSurface->Unlock();

			if (DrawData.Points != NULL) {
				delete [] DrawData.Points;
				DrawData.Points = NULL;
			}
		}
	}
}

