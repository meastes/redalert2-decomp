
// ---------------------------------------------------------------------------------------------
// NON-EXACT NATIVE DRAFTS (lane Y3), appended to the surface-cache unit: SurfaceCacheClass::Draw
// (YR 0x006BA3E0, 408 bytes) and SurfaceCacheClass::DrawMasked, unmatched in ra2-decomp. The text
// is TibSun's (TibSun archive code/srfcache.cpp; SPDX GPL-3.0-or-later, Copyright 2025 Electronic
// Arts Inc. and OpenTS contributors, EA's GPL section 7 terms; THIRD_PARTY.md); the tree's matched
// DrawTrans beside them is the same file's. Behaviour code for the native build; never part of an
// exact build.
// ---------------------------------------------------------------------------------------------
bool SurfaceCacheClass::Draw(Rect const & rect, Surface & tosurface, Surface & fromsurface, int x, int y)
{
	unsigned short *tbuf = (unsigned short *)tosurface.Lock();
	if (tbuf == NULL) {
		return(false);
	}

	unsigned short *fbuf = (unsigned short *)fromsurface.Lock();
	if (fbuf == NULL) {
		tosurface.Unlock();
		return(false);
	}

	/*
	 * When the source is larger than the rectangle, start sampling at its
	 * centering margin so the middle of the image shows through.
	 */
	int twidth = tosurface.Stride() / 2;
	int fwidth = fromsurface.Stride() / 2;
	Rect frect = fromsurface.Get_Rect();
	Rect trect = tosurface.Get_Rect();
	int ox = (frect.Width - rect.Width) / 2 <= 0 ? 0 : (frect.Width - rect.Width) / 2;
	int oy = (frect.Height - rect.Height) / 2 <= 0 ? 0 : (frect.Height - rect.Height) / 2;
	for (int ny = 0; ny < rect.Height; ny++) {
		unsigned short *t = tbuf + rect.X + twidth * (rect.Y + ny);
		for (int nx = 0; nx < rect.Width; nx++) {
			*t = fbuf[fwidth * ((oy + y + ny) % frect.Height) + (ox + x + nx) % frect.Width];
			t++;
		}
	}
	fromsurface.Unlock();
	tosurface.Unlock();
	return(true);
}

bool SurfaceCacheClass::DrawMasked(Rect const & rect, Surface & tosurface, Surface & fromsurface, Surface & masksurface, void * palette, int center, int x_offset, int y_offset)
{
	unsigned short pal16[256];

	Rect image_rect = fromsurface.Get_Rect();
	int src_y = 0;
	int src_x = 0;
	int image_w = image_rect.Width;
	int top_clip = 0;
	int image_width = image_w;
	int right_clip = 0;
	int image_h = image_rect.Height;
	int image_height = image_h;
	int src_y2 = 0;
	int rect_width = rect.Width;
	int dst_x_center = 0;
	int dst_y_center = 0;

	/*
	 * Work out the horizontal fit: an image wider than the rectangle is
	 * clipped (split between both sides when centering), a narrower one is
	 * optionally centered in the destination.
	 */
	if (image_w > rect_width) {
		if (center) {
			int diff = image_w - rect_width;
			src_x = (diff + 1) / 2;
			right_clip = diff / 2;
		} else {
			right_clip = image_w - rect_width;
		}
	} else if (center) {
		dst_x_center = (rect_width - image_w) / 2;
	}

	/*
	 * The same for the vertical fit.
	 */
	int rect_height = rect.Height;
	if (image_h > rect_height) {
		if (center) {
			top_clip = (image_h - rect_height + 1) / 2;
			src_y = (image_h - rect_height) / 2;
			src_y2 = src_y;
		} else {
			src_y = image_h - rect_height;
			src_y2 = image_h - rect_height;
		}
	} else if (center) {
		dst_y_center = (rect_height - image_h) / 2;
	}

	/*
	 * Scroll offsets shift the visible window further into the source.
	 */
	if (x_offset > src_x) {
		int newclip = src_x - x_offset + right_clip;
		src_x = x_offset;
		right_clip = newclip;
	}
	if (y_offset > top_clip) {
		int delta = top_clip - y_offset;
		top_clip = y_offset;
		src_y += delta;
		src_y2 = src_y;
	}

	unsigned short * dest = (unsigned short *)tosurface.Lock();
	if (dest == NULL) {
		return(false);
	}

	unsigned char * source = NULL;
	unsigned char * palsource = NULL;
	if (palette != NULL) {
		palsource = (unsigned char *)fromsurface.Lock();
		if (palsource == NULL) {
			tosurface.Unlock();
			return(false);
		}
	} else {
		source = (unsigned char *)fromsurface.Lock();
		if (source == NULL) {
			tosurface.Unlock();
			return(false);
		}
	}

	unsigned char * mask = (unsigned char *)masksurface.Lock();

	/*
	 * For a paletted source, convert the supplied palette into a table of
	 * ready-made 16-bit pixels and restart from the unscrolled clip values.
	 */
	if (palette != NULL) {
		unsigned char * pal = (unsigned char *)palette;
		for (int i = 0; i < 256; i++) {
			pal16[i] = DSurface::Build_Hicolor_Pixel(pal[0], pal[1], pal[2]);
			pal += 3;
		}
		src_y = src_y2;
		image_h = image_height;
	}

	int dst_stride = tosurface.Stride() / 2;
	unsigned int src_stride = fromsurface.Stride();
	if (palette == NULL) {
		src_stride >>= 1;
	}

	/*
	 * Blend the visible rows. Each source pixel is alpha-blended onto the
	 * destination with the mask byte as its alpha; zero mask bytes are
	 * skipped entirely. The paletted arm reads the source pixel through its
	 * fixed distance from the mask pointer and converts it via the table.
	 */
	int cur_y = top_clip;
	int row_count = image_h - src_y;
	if (top_clip < row_count) {
		unsigned char * mask_row = mask + src_x + top_clip * src_stride;
		unsigned short * source_row = (unsigned short *)(source + 2 * (src_x + top_clip * src_stride));
		do {
			int dst_index = dst_stride * (rect.Y - top_clip + cur_y + dst_y_center) + rect.X + dst_x_center;
			if (palette == NULL) {
				if (src_x < image_width - right_clip) {
					unsigned char * mptr = mask_row;
					unsigned short * sptr = source_row;
					unsigned short * dptr = dest + dst_index;
					int count = image_width - right_clip - src_x;
					do {
						if (*mptr != 0) {
							*dptr = OD_Blend_Color(*dptr, *sptr, *mptr);
						}
						dptr++;
						mptr++;
						sptr++;
						count--;
					} while (count != 0);
				}
			} else if (src_x < image_width - right_clip) {
				unsigned char * mptr = mask_row;
				int source_delta = (int)palsource - (int)mask;
				unsigned short * dptr = dest + dst_index;
				int count = image_width - right_clip - src_x;
				do {
					if (*mptr != 0) {
						*dptr = OD_Blend_Color(*dptr, pal16[(unsigned char)mptr[source_delta]], *mptr);
					}
					dptr++;
					mptr++;
					count--;
				} while (count != 0);
			}
			mask_row += src_stride;
			source_row += src_stride;
			cur_y++;
		} while (cur_y < row_count);
	}

	tosurface.Unlock();
	fromsurface.Unlock();
	masksurface.Unlock();
	return(true);
}
