
// ---------------------------------------------------------------------------------------------
// NON-EXACT NATIVE DRAFT (lane Y15), appended to the dsurface unit: DSurface::Draw_Depth_Antialiased_Line (gamemd.exe (dsurface.cpp)), unmatched in
// ra2-decomp. Red Alert 2's text of the same routine (mischa85/RedAlert2 src/dsurface.cpp, game.exe 0x004AC650; SPDX
// GPL-3.0-or-later, Copyright 2026 OpenTS contributors, LICENSE.md's terms; THIRD_PARTY.md), with the
// YR tree's names where they differ (marked NATIVE). Yuri's Revenge's routine is not compared with it
// instruction by instruction. Behaviour code for the native build only.
// ---------------------------------------------------------------------------------------------
bool DSurface::Draw_Depth_Antialiased_Line(Rect const & cliprect, Point2D const & startpoint, Point2D const & endpoint, RGBClass & color, int start_depth, int end_depth, bool write_depth, bool blend_red, bool blend_green, bool blend_blue, float intensity)
{
	static int GradientTable[257];
	static bool onetime = false;

	/*
	 * One-time build of the gradient/falloff lookup table.
	 */
	if (!onetime) {
		onetime = true;
		for (int i = 0; i <= 256; i++) {
			GradientTable[i] = (int)((pow((double)i / 128.0 - 1.0, 3.0) + 1.0) * 128.0);
		}
	}

	/*
	 * Shrink the surface rectangle by one pixel on every edge and clip the supplied
	 * clipping rectangle against it.
	 */
	Rect surfrect = Get_Rect();
	surfrect.X += 1;
	surfrect.Y += 1;
	surfrect.Width -= 2;
	surfrect.Height -= 2;

	Rect clip = Intersect(cliprect, surfrect);
	if (clip.Width == 0 && clip.Height == 0) {
		return(false);
	}

	/*
	 * Cache the requested gun values.
	 */
	int red = color.Get_Red();
	int green = color.Get_Green();
	int blue = color.Get_Blue();

	/*
	 * Bias both endpoints into the clipping rectangle, then order them left to right,
	 * carrying the alpha values with them.
	 */
	Point2D start = Bias_To(startpoint, clip);
	Point2D end = Bias_To(endpoint, clip);
	int astart = start_depth;
	int aend = end_depth;
	if (start.X > end.X) {
		swap(start, end);
		astart = end_depth;
		aend = start_depth;
	}

	Point2D origend = end;
	int px = start.X;
	int py = start.Y;

	if (Clip_Line_To_Rect(start, end, clip)) {

	int alpha_end = aend;
	int alpha_start = astart;

	/*
	 * Re-interpolate the alpha values across the portion of the line that survived clipping.
	 */
	if (end.X != origend.X || end.Y != origend.Y) {
		Point2D clipped(px - end.X, py - end.Y);
		Point2D full(px - origend.X, py - origend.Y);
		int fulllen = full.Length();
		unsigned int amount = abs((int)((double)clipped.Length() / (double)fulllen * (double)(astart - aend)));
		if (astart < aend) {
			alpha_end = astart + amount;
		} else {
			alpha_end = astart - amount;
		}
	}

	if (start.X != px || start.Y != py) {
		Point2D clipped(start.X - origend.X, start.Y - origend.Y);
		Point2D full(px - origend.X, py - origend.Y);
		int fulllen = full.Length();
		unsigned int amount = abs((int)((double)clipped.Length() / (double)fulllen * (double)(astart - aend)));
		if (aend > astart) {
			alpha_start = aend - amount;
		} else {
			alpha_start = aend + amount;
		}
	}

	void * buffer = Lock(start);

	if (DepthBuffer == NULL) {
		Unlock();
		return(false);
	}

	unsigned short * zbuffer = (unsigned short *)DepthBuffer->Get_Buffer_Offset(Point2D(start.X, start.Y - DepthBuffer->Get_Bounds().Y));
	unsigned short * abuffer = (unsigned short *)AlphaBuffer->Get_Buffer_Offset(Point2D(start.X - AlphaBuffer->Get_Bounds().X, start.Y - AlphaBuffer->Get_Bounds().Y));

	int zwidth = DepthBuffer->Get_Buffer_Width();
	int zwrap = -1;
	unsigned short z = (unsigned short)(alpha_start + (short)(DepthBuffer->Get_Bounds().Y + DepthBuffer->Get_Scroll()) - start.Y - clip.Y);

	if (buffer == NULL) {
		Unlock();
		return(false);
	}

	/*
	 * Distances to x, y and alpha.
	 */
	int dy = end.Y - start.Y;
	int dx = end.X - start.X;
	int xcount = end.X - start.X;
	int adelta = alpha_end - alpha_start;

	int pitch = Stride();
	if (dy < 0) {
		pitch = -pitch;
		zwidth = -zwidth;
		zwrap = 1;
	}

	int astep = 1;
	int abs_dy = abs(dy);
	int half_pitch = pitch / 2;
	if (adelta < 0) {
		astep = -1;
	}

	int dy2 = 2 * abs_dy;
	int abs_adelta = abs(adelta);
	int adelta2 = 2 * abs_adelta;

	if (dx != 0 || abs_dy != 0) {

		if (abs_adelta <= dx || abs_adelta <= abs_dy) {

			/*
			 * Geometric stepping -- one of the two axes drives the loop while the alpha
			 * gradient is advanced through a secondary DDA accumulator.
			 */
			int weight = 256;
			if (dx > abs_dy) {

				/*
				 * The slope is not steep -- step the X axis.
				 */
				int adda = adelta2 - dx;
				int grad = (int)((double)abs_dy / (double)xcount * 256.0);
				int xoff = 0;
				if (dx > 0) {
					int neg_grad = -grad;
					int neighoff = 2 * half_pitch;
					int otherweight = 0;
					do {
						int cov = *abuffer;
						if (z < *zbuffer) {
							if (cov) {
								int lidx = (int)((double)weight * intensity);
								int oidx = (int)((double)otherweight * intensity);
								int lbk = GradientTable[256 - lidx];
								int lf = GradientTable[lidx];
								int nbk = GradientTable[256 - oidx];
								int nf = GradientTable[oidx];

								RGBClass line = Deconstruct_Hicolor_Pixel(*(unsigned short *)((char *)buffer + 2 * xoff));
								RGBClass neighbor = Deconstruct_Hicolor_Pixel(*(unsigned short *)((char *)buffer + neighoff));

								int outnr = neighbor.Get_Red();
								int outlr = line.Get_Red();
								int outng = neighbor.Get_Green();
								int outlg = line.Get_Green();
								int outnb = neighbor.Get_Blue();
								int outlb = line.Get_Blue();
								if (blend_red) {
									outlr = (cov * (red * nf + line.Get_Red() * nbk)) >> 15;
									outnr = (cov * (red * lf + neighbor.Get_Red() * lbk)) >> 15;
								}
								if (blend_green) {
									outlg = (cov * (green * nf + line.Get_Green() * nbk)) >> 15;
									outng = (cov * (green * lf + neighbor.Get_Green() * lbk)) >> 15;
								}
								if (blend_blue) {
									outlb = (cov * (blue * nf + line.Get_Blue() * nbk)) >> 15;
									outnb = (cov * (blue * lf + neighbor.Get_Blue() * lbk)) >> 15;
								}

								int wb = 255;
								if (outnb <= 255) {
									wb = outnb;
								}
								if (outng > 255) {
									outng = 255;
								}
								if (outnr > 255) {
									outnr = 255;
								}
								*(unsigned short *)((char *)buffer + 2 * xoff) = (unsigned short)Build_Hicolor_Pixel(outnr, outng, wb);
								if (outlb > 255) {
									outlb = 255;
								}
								if (outlg > 255) {
									outlg = 255;
								}
								if (outlr > 255) {
									outlr = 255;
								}
								*(unsigned short *)((char *)buffer + neighoff) = (unsigned short)Build_Hicolor_Pixel(outlr, outlg, outlb);
								if (write_depth) {
									*zbuffer = z;
								}
							}
						}

						weight -= grad;
						otherweight -= neg_grad;
						if (otherweight > 256) {
							otherweight -= 256;
							weight += 256;
							buffer = (unsigned char *)buffer + pitch;
							zbuffer += zwidth;
							if (zwidth > 0) {
								zbuffer = (unsigned short *)DepthBuffer->Wrap_Overflow((unsigned int)zbuffer);
							} else {
								zbuffer = (unsigned short *)DepthBuffer->Wrap_Underflow((unsigned int)zbuffer);
							}
							abuffer += zwidth;
							if (zwidth > 0) {
								abuffer = (unsigned short *)AlphaBuffer->Wrap_Overflow((unsigned int)abuffer);
							} else {
								abuffer = (unsigned short *)AlphaBuffer->Wrap_Underflow((unsigned int)abuffer);
							}
							z += zwrap;
						}

						if (adda > 0) {
							z += astep;
							adda -= 2 * xcount;
						}

						adda += adelta2;
						zbuffer++;
						zbuffer = (unsigned short *)DepthBuffer->Wrap_Overflow((unsigned int)zbuffer);
						abuffer++;
						abuffer = (unsigned short *)AlphaBuffer->Wrap_Overflow((unsigned int)abuffer);
						xoff++;
						neighoff += 2;
					} while (xoff < xcount);
				}
			} else {

				/*
				 * The slope is steep -- step the Y axis.
				 */
				int adda = adelta2 - abs_dy;
				int grad = (int)((double)xcount / (double)abs_dy * 256.0);
				if (abs_dy > 0) {
					int byteoff = 0;
					int neg_grad = -grad;
					int otherweight = 0;
					int i = abs_dy;
					do {
						int cov = *abuffer;
						if (z < *zbuffer) {
							if (cov) {
								int lidx = (int)((double)weight * intensity);
								int oidx = (int)((double)otherweight * intensity);
								int nbk = GradientTable[256 - oidx];
								int lbk = GradientTable[256 - lidx];
								int nf = GradientTable[oidx];
								int lf = GradientTable[lidx];

								RGBClass line = Deconstruct_Hicolor_Pixel(*(unsigned short *)((char *)buffer + byteoff));
								RGBClass neighbor = Deconstruct_Hicolor_Pixel(*(unsigned short *)((char *)buffer + byteoff + 2));

								int outlb = line.Get_Blue();
								int outnb = neighbor.Get_Blue();
								int outlr = line.Get_Red();
								int outnr = neighbor.Get_Red();
								int outng = neighbor.Get_Green();
								int outlg = line.Get_Green();
								if (blend_red) {
									outlr = (cov * (red * lf + line.Get_Red() * lbk)) >> 15;
									outnr = (cov * (red * nf + neighbor.Get_Red() * nbk)) >> 15;
								}
								if (blend_green) {
									outlg = (cov * (green * lf + line.Get_Green() * lbk)) >> 15;
									outng = (cov * (green * nf + neighbor.Get_Green() * nbk)) >> 15;
								}
								if (blend_blue) {
									outlb = (cov * (blue * lf + line.Get_Blue() * lbk)) >> 15;
									outnb = (cov * (blue * nf + neighbor.Get_Blue() * nbk)) >> 15;
								}

								int wb = MIN(255, outlb);
								if (outlg > 255) {
									outlg = 255;
								}
								if (outlr > 255) {
									outlr = 255;
								}
								*(unsigned short *)((char *)buffer + byteoff) = (unsigned short)Build_Hicolor_Pixel(outlr, outlg, wb);
								if (outnb > 255) {
									outnb = 255;
								}
								if (outng > 255) {
									outng = 255;
								}
								if (outnr > 255) {
									outnr = 255;
								}
								*(unsigned short *)((char *)buffer + byteoff + 2) = (unsigned short)Build_Hicolor_Pixel(outnr, outng, outnb);
								if (write_depth) {
									*zbuffer = z;
								}
							}
						}

						weight -= grad;
						otherweight -= neg_grad;
						if (otherweight > 256) {
							otherweight -= 256;
							weight += 256;
							byteoff += 2;
							zbuffer++;
							zbuffer = (unsigned short *)DepthBuffer->Wrap_Overflow((unsigned int)zbuffer);
							abuffer++;
							abuffer = (unsigned short *)AlphaBuffer->Wrap_Overflow((unsigned int)abuffer);
						}

						if (adda > 0) {
							z += astep;
							adda -= dy2;
						}

						adda += adelta2;
						buffer = (unsigned char *)buffer + pitch;
						z += zwrap;
						zbuffer += zwidth;
						if (zwidth > 0) {
							zbuffer = (unsigned short *)DepthBuffer->Wrap_Overflow((unsigned int)zbuffer);
						} else {
							zbuffer = (unsigned short *)DepthBuffer->Wrap_Underflow((unsigned int)zbuffer);
						}
						abuffer += zwidth;
						if (zwidth > 0) {
							abuffer = (unsigned short *)AlphaBuffer->Wrap_Overflow((unsigned int)abuffer);
						} else {
							abuffer = (unsigned short *)AlphaBuffer->Wrap_Underflow((unsigned int)abuffer);
						}
						i--;
					} while (i != 0);
				}

			}

		} else {

			/*
			 * The alpha gradient changes faster than either axis -- alpha drives the loop.
			 */
			int weight = 256;
			if (dx > abs_dy) {

				/*
				 * Alpha-dominant and shallow.
				 */
				int grad = (int)((double)abs_dy / (double)xcount * 256.0);
				int xoff = 0;
				if (dx > 0) {
					int otherweight = 0;
					int neg_grad = -grad;
					int neighoff = 2 * half_pitch;
					do {
						int cov = *abuffer;
						if (z < *zbuffer) {
							if (cov) {
								int lidx = (int)((double)weight * intensity);
								int oidx = (int)((double)otherweight * intensity);
								int nf = GradientTable[oidx];
								int nbk = GradientTable[256 - oidx];
								int lf = GradientTable[lidx];
								int lbk = GradientTable[256 - lidx];

								RGBClass line = Deconstruct_Hicolor_Pixel(*(unsigned short *)((char *)buffer + 2 * xoff));
								RGBClass neighbor = Deconstruct_Hicolor_Pixel(*(unsigned short *)((char *)buffer + neighoff));

								int outlg = line.Get_Green();
								int outng = neighbor.Get_Green();
								int outlr = line.Get_Red();
								int outnr = neighbor.Get_Red();
								int outlb = line.Get_Blue();
								int outnb = neighbor.Get_Blue();
								if (blend_red) {
									outlr = (cov * (red * nf + line.Get_Red() * nbk)) >> 15;
									outnr = (cov * (red * lf + neighbor.Get_Red() * lbk)) >> 15;
								}
								if (blend_green) {
									outlg = (cov * (green * nf + line.Get_Green() * nbk)) >> 15;
									outng = (cov * (green * lf + neighbor.Get_Green() * lbk)) >> 15;
								}
								if (blend_blue) {
									outlb = (cov * (blue * nf + line.Get_Blue() * nbk)) >> 15;
									outnb = (cov * (blue * lf + neighbor.Get_Blue() * lbk)) >> 15;
								}

								int wb = MIN(255, outnb);
								if (outng > 255) {
									outng = 255;
								}
								if (outnr > 255) {
									outnr = 255;
								}
								*(unsigned short *)((char *)buffer + 2 * xoff) = (unsigned short)Build_Hicolor_Pixel(outnr, outng, wb);
								if (outlb > 255) {
									outlb = 255;
								}
								if (outlg > 255) {
									outlg = 255;
								}
								if (outlr > 255) {
									outlr = 255;
								}
								*(unsigned short *)((char *)buffer + neighoff) = (unsigned short)Build_Hicolor_Pixel(outlr, outlg, outlb);
								if (write_depth) {
									*zbuffer = z;
								}
							}
						}

						weight -= grad;
						otherweight -= neg_grad;
						if (otherweight > 256) {
							otherweight -= 256;
							buffer = (unsigned char *)buffer + pitch;
							weight += 256;
							zbuffer += zwidth;
							if (zwidth > 0) {
								zbuffer = (unsigned short *)DepthBuffer->Wrap_Overflow((unsigned int)zbuffer);
							} else {
								zbuffer = (unsigned short *)DepthBuffer->Wrap_Underflow((unsigned int)zbuffer);
							}
							abuffer += zwidth;
							if (zwidth > 0) {
								abuffer = (unsigned short *)AlphaBuffer->Wrap_Overflow((unsigned int)abuffer);
							} else {
								abuffer = (unsigned short *)AlphaBuffer->Wrap_Underflow((unsigned int)abuffer);
							}
							z += zwrap;
						}

						z += astep;
						zbuffer++;
						zbuffer = (unsigned short *)DepthBuffer->Wrap_Overflow((unsigned int)zbuffer);
						abuffer++;
						abuffer = (unsigned short *)AlphaBuffer->Wrap_Overflow((unsigned int)abuffer);
						xoff += 2;
						neighoff += 4;
					} while (xoff < xcount);
				}
			} else {

				/*
				 * Alpha-dominant and steep.
				 */
				int grad = (int)((double)xcount / (double)abs_dy * 256.0);
				if (abs_dy > 0) {
					int otherweight = 0;
					int neg_grad = -grad;
					int i = abs_dy;
					int byteoff = 0;
					int yadjust = zwrap + astep;
					do {
						int cov = *abuffer;
						if (z < *zbuffer) {
							if (cov) {
								int lidx = (int)((double)weight * intensity);
								int oidx = (int)((double)otherweight * intensity);
								int lf = GradientTable[lidx];
								int lbk = GradientTable[256 - lidx];
								int nbk = GradientTable[256 - oidx];
								int nf = GradientTable[oidx];

								RGBClass line = Deconstruct_Hicolor_Pixel(*(unsigned short *)((char *)buffer + byteoff));
								RGBClass neighbor = Deconstruct_Hicolor_Pixel(*(unsigned short *)((char *)buffer + byteoff + 2));

								int outlb = line.Get_Blue();
								int outnb = neighbor.Get_Blue();
								int outnr = neighbor.Get_Red();
								int outlr = line.Get_Red();
								int outng = neighbor.Get_Green();
								int outlg = line.Get_Green();
								if (blend_red) {
									outlr = (cov * (red * lf + line.Get_Red() * lbk)) >> 15;
									outnr = (cov * (red * nf + neighbor.Get_Red() * nbk)) >> 15;
								}
								if (blend_green) {
									outlg = (cov * (green * lf + line.Get_Green() * lbk)) >> 15;
									outng = (cov * (green * nf + neighbor.Get_Green() * nbk)) >> 15;
								}
								if (blend_blue) {
									outlb = (cov * (blue * lf + line.Get_Blue() * lbk)) >> 15;
									outnb = (cov * (blue * nf + neighbor.Get_Blue() * nbk)) >> 15;
								}

								int wb = 255;
								if (outlb <= 255) {
									wb = outlb;
								}
								if (outlg > 255) {
									outlg = 255;
								}
								if (outlr > 255) {
									outlr = 255;
								}
								*(unsigned short *)((char *)buffer + byteoff) = (unsigned short)Build_Hicolor_Pixel(outlr, outlg, wb);
								if (outnb > 255) {
									outnb = 255;
								}
								if (outng > 255) {
									outng = 255;
								}
								if (outnr > 255) {
									outnr = 255;
								}
								*(unsigned short *)((char *)buffer + byteoff + 2) = (unsigned short)Build_Hicolor_Pixel(outnr, outng, outnb);
								if (write_depth) {
									*zbuffer = z;
								}
							}
						}

						weight -= grad;
						otherweight -= neg_grad;
						if (otherweight > 256) {
							otherweight -= 256;
							weight += 256;
							byteoff += 2;
							zbuffer++;
							zbuffer = (unsigned short *)DepthBuffer->Wrap_Overflow((unsigned int)zbuffer);
							abuffer++;
							abuffer = (unsigned short *)AlphaBuffer->Wrap_Overflow((unsigned int)abuffer);
						}

						buffer = (unsigned char *)buffer + pitch;
						zbuffer += zwidth;
						if (zwidth > 0) {
							zbuffer = (unsigned short *)DepthBuffer->Wrap_Overflow((unsigned int)zbuffer);
						} else {
							zbuffer = (unsigned short *)DepthBuffer->Wrap_Underflow((unsigned int)zbuffer);
						}
						abuffer += zwidth;
						if (zwidth > 0) {
							abuffer = (unsigned short *)AlphaBuffer->Wrap_Overflow((unsigned int)abuffer);
						} else {
							abuffer = (unsigned short *)AlphaBuffer->Wrap_Underflow((unsigned int)abuffer);
						}
						z += yadjust;
						i--;
					} while (i != 0);
				}

			}
		}

		Unlock();
		return(true);
	}

	Unlock();
	return(false);

	}

	return(false);
}

