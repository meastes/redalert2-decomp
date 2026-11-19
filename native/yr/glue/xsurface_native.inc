
// ---------------------------------------------------------------------------------------------
// NON-EXACT NATIVE DRAFT (lane Y4), appended to the xsurface unit: XSurface::Plot_Line, unmatched
// in ra2-decomp. TibSun's text (TibSun archive code/xsurface.cpp; SPDX GPL-3.0-or-later,
// Copyright 2025 Electronic Arts Inc. and OpenTS contributors, EA's GPL section 7 terms;
// THIRD_PARTY.md). Behaviour code for the native build only.
// ---------------------------------------------------------------------------------------------
bool XSurface::Plot_Line(Rect const & xcliprect, Point2D & startpoint, Point2D & endpoint, void(*drawer_callback)(Point2D const &))
{
	//assert(xcliprect.Is_Valid());

	/*
	**	Ensure that the clipping rectangle is legal.
	*/
	Rect cliprect = Intersect(xcliprect, Get_Rect());

	/*
	**	High-speed working variables for the clipping rectangle and clipping operation.
	*/
	Point2D start = Bias_To(startpoint, cliprect);
	Point2D end = Bias_To(endpoint, cliprect);

	if (!Clip_Line_To_Rect(start, end, cliprect)) return(false);

	if (start.X > end.X) {
		swap(start, end);
	}

	if (start.Y == end.Y) {

		int x = end.X - start.X;

		/*
		 * Simplest of the blits, straight horizontal line.
		 */
		for (int i = 0; i <= x; i++) {
			drawer_callback(Point2D(start.X + i, end.Y));
		}

	} else if (start.X == end.X) {

		/*
		 * Straight vertical line.
		 */
		int dy = abs(start.Y - end.Y);
		int y = end.Y;
		if (start.Y <= end.Y) {
			y = start.Y;
		}
		for (int i = 0; i <= dy; i++) {
			drawer_callback(Point2D(start.X, y + i));
		}
	} else {
		/*
		 * Distances to x and y.
		 */
		int x2 = end.X;
		int dx = x2 - start.X;
		int dy = end.Y - start.Y;
		/*
		 * The line isn't straight so we need to do some maths.
		 */
		int pitch = 1;
		if (dy < 0) {
			pitch = -pitch;
		}

		dy = abs(dy);
		int dx2 = 2 * dx;
		int dy2 = 2 * dy;

		if (dx > dy) {

			/*
			 * The slope is not steep.
			 */
			int delta = dy2 - dx;
			int y = start.Y;

			/*
			 * Plot low line.
			 */
			for (int i = start.X; i <= x2; i++) {
				drawer_callback(Point2D(i, y));

				if (delta > 0) {
					delta -= dx2;
					y += pitch;
				}

				delta += dy2;
			}
		} else {

			/*
			 * The slope is steep.
			 */
			int delta = dx2 - dy;
			int x = start.X;
			int y = start.Y;
			int k = 0;

			/*
			 * Plot high line.
			 */
			for (int i = 0; i <= dy; i++) {
				drawer_callback(Point2D(x + k, y));

				if (delta > 0) {
					k++;
					delta -= dy2;
				}

				delta += dx2;
				y += pitch;
			}
		}
	}

	/*
	 * This routine plots through the callback and never locks the surface, so this unlock
	 * has nothing to pair with.
	 */
	Unlock();

	return(true);
}

// ---------------------------------------------------------------------------------------------
// NON-EXACT NATIVE STAND-IN (lane N3, 2026-11-19), appended to the xsurface unit: XSurface::Draw_Ellipse,
// unmatched in ra2-decomp (a link-closure trap). Red Alert 2's draft of its twin (game.exe 0x007746C0,
// src/recovered/xsurface.cpp, RA2_NON_MATCHING, best score 22.29: a 16-bit midpoint ellipse, clipped
// pixel by pixel), with YR's names; not compared with gamemd.exe instruction by instruction. Reached by
// BuildingClass::Draw_Radial_Indicator (a selected Gap Generator's or Psychic Sensor's range ring).
// Behaviour code for the native build only.
// ---------------------------------------------------------------------------------------------
bool XSurface::Draw_Ellipse(Point2D point, int radx, int rady, Rect cliprect, int color)
{
    Point2D pixel;
    int pitch = Stride() >> 1;

    int bottom = point.Y + rady;
    if (point.X >= cliprect.X && point.X < cliprect.X + cliprect.Width && bottom >= cliprect.Y && bottom < cliprect.Y + cliprect.Height) {
        pixel.X = point.X;
        pixel.Y = bottom;
        Put_Pixel(pixel, color);
    }

    int top = point.Y - rady;
    if (point.X >= cliprect.X && point.X < cliprect.X + cliprect.Width && top >= cliprect.Y && top < cliprect.Y + cliprect.Height) {
        pixel.X = point.X;
        pixel.Y = top;
        Put_Pixel(pixel, color);
    }

    int rady_squared = rady * rady;
    int radx_squared = radx * radx;
    int product = rady * radx_squared;
    int y = rady;
    int xterm = 0;
    int yterm = 2 * product;
    int decision = radx_squared / 4 - product;

    short *buffer = (short *)Lock();

    int bottom_offset = pitch * bottom;
    int top_offset = pitch * top;
    int xleft = point.X;
    short *bottom_right = &buffer[bottom_offset + point.X];
    short *top_left = &buffer[top_offset + point.X];
    short *top_right = &buffer[top_offset + point.X];
    int xright = point.X;
    short *bottom_left = &buffer[bottom_offset + point.X];

    while (true) {
        decision += rady_squared + xterm;
        if (decision >= 0) {
            yterm -= 2 * radx_squared;
            decision -= yterm;
            y--;

            bottom_right -= pitch;
            top_left += pitch;
            top_right += pitch;
            bottom_left -= pitch;
        }
        xterm += 2 * rady_squared;

        xright++;
        xleft--;

        bottom_right++;
        top_left--;
        top_right++;
        bottom_left--;

        if (xterm >= yterm) {
            break;
        }
        int ypos = y + point.Y;
        if (xright >= cliprect.X && xright < cliprect.X + cliprect.Width && ypos >= cliprect.Y && ypos < cliprect.Y + cliprect.Height) {
            *bottom_right = color;
        }
        ypos = point.Y - y;
        if (xleft >= cliprect.X && xleft < cliprect.X + cliprect.Width && ypos >= cliprect.Y && ypos < cliprect.Y + cliprect.Height) {
            *top_left = color;
        }
        ypos = point.Y - y;
        if (xright >= cliprect.X && xright < cliprect.X + cliprect.Width && ypos >= cliprect.Y && ypos < cliprect.Y + cliprect.Height) {
            *top_right = color;
        }
        ypos = point.Y + y;
        if (xleft >= cliprect.X && xleft < cliprect.X + cliprect.Width && ypos >= cliprect.Y && ypos < cliprect.Y + cliprect.Height) {
            *bottom_left = color;
        }
    }

    xright = point.X + radx;
    if (xright >= cliprect.X && xright < cliprect.X + cliprect.Width && point.Y >= cliprect.Y && point.Y < cliprect.Y + cliprect.Height) {
        pixel.X = xright;
        pixel.Y = point.Y;
        Put_Pixel(pixel, color);
    }

    xleft = point.X - radx;
    if (xleft >= cliprect.X && xleft < cliprect.X + cliprect.Width && point.Y >= cliprect.Y && point.Y < cliprect.Y + cliprect.Height) {
        pixel.X = xleft;
        pixel.Y = point.Y;
        Put_Pixel(pixel, color);
    }

    y = 0;
    yterm = 0;
    product = radx * rady_squared;
    xterm = 2 * product;
    int x = radx;
    decision = rady_squared / 4 - product;

    int left_offset = pitch * point.Y - radx;
    int center_offset = pitch * point.Y;
    xleft = point.X - radx;
    bottom_right = &buffer[point.X + center_offset + radx];
    top_left = &buffer[xleft + center_offset];
    xright = point.X + radx;
    top_right = &buffer[xright + center_offset];
    bottom_left = &buffer[point.X + left_offset];

    while (true) {
        decision += radx_squared + yterm;
        if (decision >= 0) {
            xterm -= 2 * rady_squared;
            decision -= xterm;
            bottom_right--;
            xright--;
            top_left++;
            xleft++;
            top_right--;
            bottom_left++;
        }
        yterm += 2 * radx_squared;
        y++;

        bottom_right += pitch;
        top_left -= pitch;
        top_right -= pitch;
        bottom_left += pitch;

        if (yterm > xterm) {
            break;
        }
        int ypos = y + point.Y;
        if (xright >= cliprect.X && xright < cliprect.X + cliprect.Width && ypos >= cliprect.Y && ypos < cliprect.Y + cliprect.Height) {
            *bottom_right = color;
        }
        ypos = point.Y - y;
        if (xleft >= cliprect.X && xleft < cliprect.X + cliprect.Width && ypos >= cliprect.Y && ypos < cliprect.Y + cliprect.Height) {
            *top_left = color;
        }
        ypos = point.Y - y;
        if (xright >= cliprect.X && xright < cliprect.X + cliprect.Width && ypos >= cliprect.Y && ypos < cliprect.Y + cliprect.Height) {
            *top_right = color;
        }
        ypos = point.Y + y;
        if (xleft >= cliprect.X && xleft < cliprect.X + cliprect.Width && ypos >= cliprect.Y && ypos < cliprect.Y + cliprect.Height) {
            *bottom_left = color;
        }
    }

    return Unlock();
}
