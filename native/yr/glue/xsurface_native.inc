
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
