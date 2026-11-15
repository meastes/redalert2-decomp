
// ---------------------------------------------------------------------------------------------
// NON-EXACT NATIVE DRAFT (lane Y3), appended to the surface-cache unit (its caller):
// Read_PCX_File, YR 0x00630310 (1,635 bytes, unmatched in ra2-decomp). The text is TibSun's
// Read_PCX_File (TibSun archive code/pcx.cpp; SPDX GPL-3.0-or-later, Copyright 2025 Electronic
// Arts Inc. and OpenTS contributors, EA's GPL section 7 terms; THIRD_PARTY.md), whose 0x80-byte
// header read, 2,048-byte pool and three paths (24-bit planes to hicolor, padded and packed
// 8-bit lines) gamemd.exe's code repeats; the RA2 tree carries the same text as its draft of
// RA2 0x0060D110. Behaviour code for the native build; never part of an exact build.
// ---------------------------------------------------------------------------------------------
#include "pcx.h"
#include "dsurface.h"
#define	POOL_SIZE 2048
#define	READ_CHAR()  *file_ptr++ ; \
							 if ( file_ptr	>= & pool [ POOL_SIZE ]	) { \
								 file_handle.Read (pool, POOL_SIZE ); \
								 file_ptr = pool ; \
							 }
#define	READ_CHARx()  *file_ptr++ ; \
							 if ( file_ptr	>= & pool [ POOL_SIZE ]	) { \
								 file_handle.Read (pool, POOL_SIZE ); \
							 }


Surface * Read_PCX_File(FileClass & file_handle, PaletteClass * palette, void * Buff, int Size)
{
	unsigned					i, j;
	unsigned char			rle;
	unsigned char			color;
	unsigned					scan_pos;
	char						*file_ptr;
	unsigned					width;
	unsigned					height;
	char						*buffer;
	PCX_HEADER				header;
	char						pool [POOL_SIZE];
	BSurface * pic;

	if (!file_handle.Is_Available()) return(NULL);

	file_handle.Open(FileClass::READ);

	file_handle.Read (&header, sizeof (PCX_HEADER));

	if (header.id != 10 &&  header.version != 5 && header.pixelsize != 8 ) return(NULL);

	width = header.width - header.x + 1;
	height = header.height - header.y + 1;
	unsigned in_line = header.color_planes * header.byte_per_line;
	unsigned bytes_per_pixel = (header.color_planes == 1) ? 1 : 2;

	if (Buff != NULL) {
		i = Size / width;
		height = MIN ((int)(i - 1), (int)height);
		Buffer b(Buff, Size);
		pic = new BSurface(width, height, bytes_per_pixel, &b);
	} else {
		pic = new BSurface(width, height, bytes_per_pixel);
	}
	if (pic == NULL) return(NULL);

	buffer = (char *)pic->Lock();
	if (buffer != NULL) {
		file_ptr = pool ;
		file_handle.Read (pool, POOL_SIZE);

		unsigned short * hibuffer = (unsigned short *)buffer;

		if ( header.color_planes == 3 ) {

			/*
			 * The pcx file is stored as three separate red, green and blue
			 * planes per scanline. Decode each scanline into a temporary
			 * line buffer, then interleave the three planes into hicolor
			 * pixels on the surface.
			 */
			unsigned char * line = new unsigned char [in_line];
			if (line == NULL) {
				pic->Unlock();
				delete pic;
				return(NULL);
			}

			for ( j = 0 ; j < height ; j ++ ) {
				i = 0;
				do {
					rle = READ_CHAR ();
					if ( (rle & 192) == 192 ) {
						rle &= 63 ;
						color = READ_CHAR ();
						do {
							line[i++] = color;
						} while (--rle);
					} else {
						line[i++] = rle;
					}
				} while ( i < in_line );

				i = 0;
				if (i < width) {
					unsigned char * rptr = line;
					unsigned char * gptr = line + header.byte_per_line;
					unsigned char * bptr = line + header.byte_per_line * 2;
					while ( i < width ) {
						*hibuffer++ = (unsigned short)DSurface::Build_Hicolor_Pixel(*rptr++, *gptr++, *bptr++);
						i++;
					}
				}
			}

			delete [] line;
			pic->Unlock();

		} else if ( header.byte_per_line != width ) {

			i = 0;
			rle = 0;
			for ( scan_pos = j = 0 ; j < height ; j ++, scan_pos += width ) {
				for ( i = 0 ; i < header.byte_per_line ; ) {
					rle = READ_CHAR ();
					if ( (rle & 192) == 192 ) {
						rle &= 63 ;
						color =	READ_CHAR (); ;
						for ( unsigned k = 0 ; k < rle ; k ++ ) {
							if ( k + i < width ) {
								*(buffer + scan_pos + k + i) = color;
							}
						}
						i += rle;
					} else {
						if ( i < width ) {
							*(buffer+scan_pos + i++ ) = (char)rle;
						}
					}
				}
			}

			if ( i == width ) {
				rle = READ_CHAR ();
			}
			if ( (rle & 192) == 192 ) {
				READ_CHARx();
			}
			pic->Unlock();

		} else {

			for ( i = 0 ; i < width * height ; ) {
				rle = READ_CHAR ();
				if ( (rle & 192) == 192 ) {
					rle &= 63 ;
					color = READ_CHAR ();
					memset ( buffer + i, color, rle );
					i += rle ;
				} else {
					*(buffer + i++) = (char)rle;
				}
			}
			pic->Unlock();
		}
	}

	if ( palette && header.color_planes == 1 ) {
		file_handle.Seek (- (256 * (int)sizeof(RGB)), SEEK_END );
		file_handle.Read (palette, 256L * sizeof ( RGB ));
	}

	file_handle.Close();
	return(pic);
}
