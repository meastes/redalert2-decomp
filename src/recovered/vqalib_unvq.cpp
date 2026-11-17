// vqalib_unvq.cpp: the original translation unit re-filed as one source (Route A): the accepted
// bodies of its canonical compile group in address order, under the unit's own header
// state (its switches and includes in their original order). The module map
// manifests/original_target/modules/vqalib_unvq.json carries the addresses and the rows that
// stay in their own files.


#include "shared/vqa_player.hpp"


// void __cdecl UnVQ2_C1_4x4(unsigned char *codebook, unsigned char *pointers, unsigned char *buffer,
//                 unsigned long blocksperrow, unsigned long numrows, unsigned long bufwidth) -- 0x0077CBD0
// Purpose: draws a 4x4-block frame into a 16-bit buffer from 16-bit block codes
// (fill, skip, replicated codebook rows, keyed codebook blocks and their single-block
// forms); a block row wraps to the next four pixel rows after blocksperrow blocks.

void __cdecl UnVQ2_C1_4x4(unsigned char *codebook, unsigned char *pointers, unsigned char *buffer,
                          unsigned long blocksperrow, unsigned long numrows, unsigned long bufwidth)
{
    bufwidth *= 2;
    unsigned long rowbytes = bufwidth * 4;
    unsigned char *end = buffer + numrows * bufwidth * 4;
    unsigned char *rowstart = buffer;
    unsigned char *rowend = rowstart + blocksperrow * 8;
    unsigned short *src = (unsigned short *)pointers;
    unsigned char *dst = buffer;

    while (dst < end) {
        unsigned short code = *src;
        int op = code & 0xF000;
        unsigned long count = code;
        count &= 0xFFFF0FFF;
        src++;

        switch (op) {
        case 0x0000: {
            unsigned long n = count * 2;
            unsigned long w = *src;
            unsigned long fill = (w << 16) | w;
            src++;
            {
                unsigned int *p = (unsigned int *)dst;
                unsigned int left = n;
                while (left > 0) { *p++ = fill; left--; }
            }
            {
                unsigned int *p = (unsigned int *)(dst + bufwidth);
                unsigned int left = n;
                while (left > 0) { *p++ = fill; left--; }
            }
            {
                unsigned int *p = (unsigned int *)(dst + bufwidth * 2);
                unsigned int left = n;
                while (left > 0) { *p++ = fill; left--; }
            }
            {
                unsigned int *p = (unsigned int *)(dst + bufwidth * 3);
                unsigned int left = n;
                while (left > 0) { *p++ = fill; left--; }
            }
            dst += count * 8;
        } break;

        case 0x1000:
            dst += count * 8;
            break;

        case 0x2000: {
            unsigned char *cb = codebook + ((unsigned long)*src << 5);
            src++;
            for (unsigned long r = 0; r < 4; r++) {
                unsigned long lo = ((unsigned long)*(unsigned short *)(cb + 2) << 16) | *(unsigned short *)cb;
                cb += 4;
                unsigned long i;
                {
                    unsigned char *p = dst;
                    for (i = 0; i < count; i++) { *(unsigned long *)p = lo; p += 8; }
                }
                unsigned long hi = ((unsigned long)*(unsigned short *)(cb + 2) << 16) | *(unsigned short *)cb;
                cb += 4;
                {
                    unsigned char *p = dst + 4;
                    for (i = 0; i < count; i++) { *(unsigned long *)p = hi; p += 8; }
                }
                dst += bufwidth;
            }
            dst += count * 8 - rowbytes;
        } break;

        case 0x3000: {
            unsigned short index = *src;
            unsigned char *cb = codebook + 32 * (unsigned long)index;
            unsigned long r;
            src++;
            for (r = 0; r < 4; r++) {
                unsigned char *rp = dst;
                unsigned long c;
                for (c = 0; c < 4; c++) {
                    unsigned short px = *(unsigned short *)cb;
                    if ((px & 0x8000) == 0) {
                        unsigned char *p = rp;
                        unsigned long i;
                        for (i = 0; i < count; i++) { *(unsigned short *)p = px; p += 8; }
                    }
                    rp += 2;
                    cb += 2;
                }
                dst += bufwidth;
            }
            dst += count * 8 - rowbytes;
        } break;

        case 0x5000: {
            unsigned long i;
            for (i = 0; i < count; i++) {
                unsigned long step = (bufwidth >> 2) * 4;
                unsigned long j;
                unsigned long fill;
                unsigned long rows = 4;
                unsigned char *p;
                {
                    unsigned long w = *src;
                    fill = (w << 16) | w;
                    src++;
                }
                p = dst;
                for (j = 0; j < rows; j++) {
                    *(unsigned long *)(p + 0) = fill;
                    *(unsigned long *)(p + 4) = fill;
                    p += step;
                }
                dst += 8;
            }
        } break;

        case 0x6000: {
            unsigned long i;
            for (i = 0; i < count; i++) {
                unsigned long step = (bufwidth >> 2) * 4;
                unsigned char *cb = codebook + ((unsigned long)*src << 5);
                src++;
                unsigned char *p = dst;
                for (unsigned long r = 0; r < 4; r++) {
                    unsigned long lo;
                    unsigned long hi;
                    lo = ((unsigned long)*(unsigned short *)(cb + 2) << 16) | *(unsigned short *)cb;
                    cb += 4;
                    *(unsigned long *)(p + 0) = lo;
                    hi = ((unsigned long)*(unsigned short *)(cb + 2) << 16) | *(unsigned short *)cb;
                    cb += 4;
                    *(unsigned long *)(p + 4) = hi;
                    p += step;
                }
                dst += 8;
            }
        } break;

        case 0x7000: {
            unsigned long i;
            for (i = 0; i < count; i++) {
                unsigned short index = *src;
                unsigned char *cb = codebook + 32 * (unsigned long)index;
                src++;
                unsigned char *p = dst;
                for (unsigned long r = 0; r < 4; r++) {
                    unsigned char *q = p;
                    for (unsigned long c = 0; c < 4; c++) {
                        unsigned short px = *(unsigned short *)cb;
                        if ((px & 0x8000) == 0) {
                            *(unsigned short *)q = px;
                        }
                        q += 2;
                        cb += 2;
                    }
                    p += (bufwidth >> 1) * 2;
                }
                dst += 8;
            }
        } break;
        }

        if (dst == rowend) {
            rowstart += rowbytes;
            dst = rowstart;
            rowend = rowstart + blocksperrow * 8;
        }
    }
}


// void __cdecl UnVQ1_C4_4x4(unsigned char *codebook, unsigned char *pointers, unsigned char *buffer,
//                 unsigned long blocksperrow, unsigned long numrows, unsigned long bufwidth) -- 0x0077CF70
// Purpose: draws a 4x4-block frame into a 16-bit buffer from 13-bit codebook
// indices (opaque, keyed on 0x8000 pixels, or a one-block skip).

void __cdecl UnVQ1_C4_4x4(unsigned char *codebook, unsigned char *pointers, unsigned char *buffer,
                          unsigned long blocksperrow, unsigned long numrows, unsigned long bufwidth)
{
    bufwidth *= 2;
    unsigned long rowbytes = bufwidth * 4;
    unsigned char *end = buffer + numrows * bufwidth * 4;
    unsigned short *src = (unsigned short *)pointers;
    unsigned char *dst = buffer;
    unsigned char *rowstart = buffer;
    unsigned char *rowend = rowstart + blocksperrow * 8;

    while (dst < end) {
        unsigned long op = *src & 0xE000;
        unsigned short index = *src & 0x1FFF;
        src++;

        switch (op) {
        case 0x0000: {
            unsigned char *cb = codebook + (index << 5);
            for (unsigned long r = 0; r < 4; r++) {
                unsigned long lo;
                unsigned long hi;
                lo = ((unsigned long)*(unsigned short *)(cb + 2) << 16) | *(unsigned short *)cb;
                cb += 4;
                *(unsigned long *)(dst + 0) = lo;
                hi = ((unsigned long)*(unsigned short *)(cb + 2) << 16) | *(unsigned short *)cb;
                cb += 4;
                *(unsigned long *)(dst + 4) = hi;
                dst += bufwidth;
            }
            dst += 8 - rowbytes;
        } break;

        case 0x2000: {
            unsigned char *cb = codebook + (index << 5);
            for (unsigned long r = 0; r < 4; r++) {
                unsigned char *p = dst;
                for (unsigned long c = 0; c < 4; c++) {
                    unsigned short px = *(unsigned short *)cb;
                    if (px != 0x8000) {
                        *(unsigned short *)p = px;
                    }
                    p += 2;
                    cb += 2;
                }
                dst += bufwidth;
            }
            dst += 8 - rowbytes;
        } break;

        case 0x4000:
            dst += 8;
            break;

        default:
            break;
        }

        if (dst == rowend) {
            rowstart += rowbytes;
            dst = rowstart;
            rowend = rowstart + blocksperrow * 8;
        }
    }
}


// SPDX-License-Identifier: GPL-3.0-or-later
// Copyright 2026 ra2-decomp contributors
// Copyright 2025 Electronic Arts Inc.; 2026 OpenTS contributors
// Derived from Electronic Arts source code; EA's GPL Section 7 terms apply (COPYING.md).
// From ra2-decomp (YR 0x007C3EB0).

typedef signed int int32_t;
typedef unsigned char uint8_t;
typedef unsigned short uint16_t;
typedef unsigned int uint32_t;

void __cdecl UnVQ2_C4_4x4(unsigned char * codebook, unsigned char * pointers, unsigned char * buffer, unsigned long blocksperrow, unsigned long numrows, unsigned long bufwidth)
{
	bufwidth *= 2u;
	uint32_t block_row_stride = bufwidth * 4u;

	uint8_t * end = (uint8_t *)buffer + ((uint32_t)numrows * bufwidth * 4u);
	uint8_t * dst = (uint8_t *)buffer;

	uint8_t * row_base = (uint8_t *)buffer;
	uint8_t * row_end = row_base + ((uint32_t)blocksperrow * 8u);

	uint8_t * src = pointers;

	uint32_t height;
	uint32_t code;
	uint32_t idx;

	while (dst < end) {

		uint32_t command = (*(uint16_t *)src & 0xE000);
		uint16_t cb_index = (*(uint16_t *)src & 0x1FFF);
		src += 2;

		switch (command) {

		case 0x4000u: {
			uint32_t repeat = ((cb_index >> 7) & 0x3Eu) + 2u;
			const uint8_t * cb = codebook + (*(src - 2) << 5);
			uint8_t * rowp = dst;
			uint32_t step = 4u * (bufwidth >> 2);

			for (uint32_t r = 0; r < 4; ++r) {
				uint32_t lo = ((uint32_t)*(uint16_t *)(cb + 2) << 16) | *(uint16_t *)(cb + 0);
				cb += 4;
				*(uint32_t *)(rowp + 0) = lo;

				uint32_t hi = ((uint32_t)*(uint16_t *)(cb + 2) << 16) | *(uint16_t *)(cb + 0);
				cb += 4;
				*(uint32_t *)(rowp + 4) = hi;

				rowp += step;
			}

			uint8_t * cursor = src;
			dst += 8;

			for (int32_t i = 0; i < repeat; i++) {
				uint32_t idx = *cursor;
				cursor += 1;
				uint8_t * p = dst;
				const uint8_t * cb2 = codebook + (idx << 5);

				for (uint32_t r = 0; r < 4; ++r) {
					uint32_t lo = ((uint32_t)*(uint16_t *)(cb2 + 2) << 16) | *(uint16_t *)(cb2 + 0);
					cb2 += 4;
					*(uint32_t *)(p + 0) = lo;

					uint32_t hi = ((uint32_t)*(uint16_t *)(cb2 + 2) << 16) | *(uint16_t *)(cb2 + 0);
					cb2 += 4;
					*(uint32_t *)(p + 4) = hi;

					p += step;
				}

				dst += 8;
			}

			src = cursor;
		} break;

		case 0x2000u: {
			code = cb_index;
			code &= 0xFFFFu;
			height = ((code >> 7) & 0x3Eu) + 2u;
		label_check:
			if (height == 0u) {
				idx = cb_index;
				height = *src;
				src += 1;
				goto label_vertical_fill;
			} else {
				goto label_masked_index;
			}

		}

		case 0x0000u: {
			dst += 8u * (uint8_t)cb_index;
		} break;

		case 0x6000u: {
			const uint8_t * cb = codebook + ((cb_index & 0xFFFFu) << 5);

			for (uint32_t r = 0; r < 4; ++r) {
				uint32_t lo = ((uint32_t)*(uint16_t *)(cb + 2) << 16) | *(uint16_t *)(cb + 0);
				cb += 4;
				*(uint32_t *)(dst + 0) = lo;

				uint32_t hi = ((uint32_t)*(uint16_t *)(cb + 2) << 16) | *(uint16_t *)(cb + 0);
				cb += 4;
				*(uint32_t *)(dst + 4) = hi;

				dst += bufwidth;
			}

			dst += 8u - block_row_stride;
		} break;

		case 0xC000u: {
			height = *src;
			src += 1;

			const uint8_t * cb = codebook + (32u * (uint16_t)cb_index);

			for (uint32_t r = 0; r < 4; ++r) {
				uint8_t * p = dst;

				for (uint32_t c = 0; c < 4; ++c) {
					uint16_t v = *(uint16_t *)cb;

					if (v != 0x8000u) {
						uint8_t * q = p;
						for (uint32_t i = 0; i < height; i++) {
							*(uint16_t *)q = v;
							q += 8;
						}
					}

					p += 2;
					cb += 2;
				}

				dst += bufwidth;
			}

			dst += 8u * height - block_row_stride;
		} break;

		case 0xA000u:
			height = 0;
			goto label_check;

		label_masked_index:
			idx = code & 0xFFu;

		label_vertical_fill:
			{
				const uint8_t * cb = codebook + (32u * idx);

				for (uint32_t r = 0; r < 4; ++r) {

					uint32_t lo = ((uint32_t)*(uint16_t *)(cb + 2) << 16) | *(uint16_t *)(cb + 0);
					cb += 4;

					{
						uint8_t * p = dst;
						uint32_t i;
						for (i = 0; i < height; i++) {
							*(uint32_t *)p = lo;
							p += 8;
						}
					}

					uint32_t hi = ((uint32_t)*(uint16_t *)(cb + 2) << 16) | *(uint16_t *)(cb + 0);
					cb += 4;

					{
						uint8_t * p = dst + 4;
						uint32_t i;
						for (i = 0; i < height; i++) {
							*(uint32_t *)p = hi;
							p += 8;
						}
					}

					dst += bufwidth;
				}

				dst += 8u * height - block_row_stride;
			}
			break;

		case 0x8000u: {
			const uint8_t * cb = codebook + (32u * (uint16_t)cb_index);

			for (uint32_t r = 0; r < 4; ++r) {
				uint8_t * p = dst;

				for (uint32_t c = 0; c < 4; ++c) {
					if (*(uint16_t *)cb != 0x8000u) {
						*(uint16_t *)p = *(uint16_t *)cb;
					}
					p += 2;
					cb += 2;
				}

				dst += bufwidth;
			}

			dst += 8u - block_row_stride;
		} break;
		}

		if (dst == row_end) {
			row_base += block_row_stride;
			dst = row_base;
			row_end = row_base + ((uint32_t)blocksperrow * 8u);
		}
	}
}


// void __cdecl UnVQ1_C4_4x2(unsigned char *codebook, unsigned char *pointers, unsigned char *buffer,
//                 unsigned long blocksperrow, unsigned long numrows, unsigned long bufwidth) -- 0x0077D470
// Purpose: the 4x2-block form of UnVQ1_C4_4x4 (16-byte codebook entries, two
// pixel rows per block).

void __cdecl UnVQ1_C4_4x2(unsigned char *codebook, unsigned char *pointers, unsigned char *buffer,
                          unsigned long blocksperrow, unsigned long numrows, unsigned long bufwidth)
{
    bufwidth *= 2;
    unsigned long rowbytes = bufwidth * 2;
    unsigned char *end = buffer + numrows * bufwidth * 2;
    unsigned short *src = (unsigned short *)pointers;
    unsigned char *dst = buffer;
    unsigned char *rowstart = buffer;
    unsigned char *rowend = rowstart + blocksperrow * 8;

    while (dst < end) {
        unsigned long op = *src & 0xE000;
        unsigned short index = *src & 0x1FFF;
        src++;

        switch (op) {
        case 0x0000: {
            unsigned char *cb = codebook + (index << 4);
            for (unsigned long r = 0; r < 2; r++) {
                unsigned long lo;
                unsigned long hi;
                lo = ((unsigned long)*(unsigned short *)(cb + 2) << 16) | *(unsigned short *)cb;
                cb += 4;
                *(unsigned long *)(dst + 0) = lo;
                hi = ((unsigned long)*(unsigned short *)(cb + 2) << 16) | *(unsigned short *)cb;
                cb += 4;
                *(unsigned long *)(dst + 4) = hi;
                dst += bufwidth;
            }
            dst += 8 - rowbytes;
        } break;

        case 0x2000: {
            unsigned char *cb = codebook + (index << 4);
            for (unsigned long r = 0; r < 2; r++) {
                unsigned char *p = dst;
                for (unsigned long c = 0; c < 4; c++) {
                    unsigned short px = *(unsigned short *)cb;
                    if (px != 0x8000) {
                        *(unsigned short *)p = px;
                    }
                    p += 2;
                    cb += 2;
                }
                dst += bufwidth;
            }
            dst += 8 - rowbytes;
        } break;

        case 0x4000:
            dst += 8;
            break;

        default:
            break;
        }

        if (dst == rowend) {
            rowstart += rowbytes;
            dst = rowstart;
            rowend = rowstart + blocksperrow * 8;
        }
    }
}


// SPDX-License-Identifier: GPL-3.0-or-later
// Copyright 2026 ra2-decomp contributors
// Copyright 2025 Electronic Arts Inc.; 2026 OpenTS contributors
// Derived from Electronic Arts source code; EA's GPL Section 7 terms apply (COPYING.md).
// From ra2-decomp (YR 0x007C43A0).

typedef signed int int32_t;
typedef unsigned char uint8_t;
typedef unsigned short uint16_t;
typedef unsigned int uint32_t;

void __cdecl UnVQ2_C4_4x2(unsigned char * codebook, unsigned char * pointers, unsigned char * buffer, unsigned long blocksperrow, unsigned long numrows, unsigned long bufwidth)
{
	bufwidth *= 2u;
	uint32_t block_row_stride = bufwidth * 2u;

	uint8_t * end = (uint8_t *)buffer + ((uint32_t)numrows * bufwidth * 2u);
	uint8_t * dst = (uint8_t *)buffer;

	uint8_t * row_base = (uint8_t *)buffer;
	uint8_t * row_end = row_base + ((uint32_t)blocksperrow * 8u);

	uint8_t * src = pointers;

	uint32_t height;
	uint32_t code;
	uint32_t idx;

	while (dst < end) {

		uint32_t command = (*(uint16_t *)src & 0xE000);
		uint16_t cb_index = (*(uint16_t *)src & 0x1FFF);
		src += 2;

		switch (command) {

		case 0x4000u: {
			uint32_t repeat = ((cb_index >> 7) & 0x3Eu) + 2u;
			const uint8_t * cb = codebook + (*(src - 2) << 4);
			uint8_t * rowp = dst;
			uint32_t step = 4u * (bufwidth >> 2);

			for (uint32_t r = 0; r < 2; ++r) {
				uint32_t lo = ((uint32_t)*(uint16_t *)(cb + 2) << 16) | *(uint16_t *)(cb + 0);
				cb += 4;
				*(uint32_t *)(rowp + 0) = lo;

				uint32_t hi = ((uint32_t)*(uint16_t *)(cb + 2) << 16) | *(uint16_t *)(cb + 0);
				cb += 4;
				*(uint32_t *)(rowp + 4) = hi;

				rowp += step;
			}

			uint8_t * cursor = src;
			dst += 8;

			for (int32_t i = 0; i < repeat; i++) {
				uint32_t idx = *cursor;
				cursor += 1;
				uint8_t * p = dst;
				const uint8_t * cb2 = codebook + (idx << 4);

				for (uint32_t r = 0; r < 2; ++r) {
					uint32_t lo = ((uint32_t)*(uint16_t *)(cb2 + 2) << 16) | *(uint16_t *)(cb2 + 0);
					cb2 += 4;
					*(uint32_t *)(p + 0) = lo;

					uint32_t hi = ((uint32_t)*(uint16_t *)(cb2 + 2) << 16) | *(uint16_t *)(cb2 + 0);
					cb2 += 4;
					*(uint32_t *)(p + 4) = hi;

					p += step;
				}

				dst += 8;
			}

			src = cursor;
		} break;

		case 0x2000u: {
			code = cb_index;
			code &= 0xFFFFu;
			height = ((code >> 7) & 0x3Eu) + 2u;
		label_check:
			if (height == 0u) {
				idx = cb_index;
				height = *src;
				src += 1;
				goto label_vertical_fill;
			} else {
				goto label_masked_index;
			}

		}

		case 0x0000u: {
			dst += 8u * (uint8_t)cb_index;
		} break;

		case 0x6000u: {
			const uint8_t * cb = codebook + ((cb_index & 0xFFFFu) << 4);

			for (uint32_t r = 0; r < 2; ++r) {
				uint32_t lo = ((uint32_t)*(uint16_t *)(cb + 2) << 16) | *(uint16_t *)(cb + 0);
				cb += 4;
				*(uint32_t *)(dst + 0) = lo;

				uint32_t hi = ((uint32_t)*(uint16_t *)(cb + 2) << 16) | *(uint16_t *)(cb + 0);
				cb += 4;
				*(uint32_t *)(dst + 4) = hi;

				dst += bufwidth;
			}

			dst += 8u - block_row_stride;
		} break;

		case 0xC000u: {
			height = *src;
			src += 1;

			const uint8_t * cb = codebook + (16u * (uint16_t)cb_index);

			for (uint32_t r = 0; r < 2; ++r) {
				uint8_t * p = dst;

				for (uint32_t c = 0; c < 4; ++c) {
					uint16_t v = *(uint16_t *)cb;

					if (v != 0x8000u) {
						uint8_t * q = p;
						for (uint32_t i = 0; i < height; i++) {
							*(uint16_t *)q = v;
							q += 8;
						}
					}

					p += 2;
					cb += 2;
				}

				dst += bufwidth;
			}

			dst += 8u * height - block_row_stride;
		} break;

		case 0xA000u:
			height = 0;
			goto label_check;

		label_masked_index:
			idx = code & 0xFFu;

		label_vertical_fill:
			{
				const uint8_t * cb = codebook + (16u * idx);

				for (uint32_t r = 0; r < 2; ++r) {

					uint32_t lo = ((uint32_t)*(uint16_t *)(cb + 2) << 16) | *(uint16_t *)(cb + 0);
					cb += 4;

					{
						uint8_t * p = dst;
						uint32_t i;
						for (i = 0; i < height; i++) {
							*(uint32_t *)p = lo;
							p += 8;
						}
					}

					uint32_t hi = ((uint32_t)*(uint16_t *)(cb + 2) << 16) | *(uint16_t *)(cb + 0);
					cb += 4;

					{
						uint8_t * p = dst + 4;
						uint32_t i;
						for (i = 0; i < height; i++) {
							*(uint32_t *)p = hi;
							p += 8;
						}
					}

					dst += bufwidth;
				}

				dst += 8u * height - block_row_stride;
			}
			break;

		case 0x8000u: {
			const uint8_t * cb = codebook + (16u * (uint16_t)cb_index);

			for (uint32_t r = 0; r < 2; ++r) {
				uint8_t * p = dst;

				for (uint32_t c = 0; c < 4; ++c) {
					if (*(uint16_t *)cb != 0x8000u) {
						*(uint16_t *)p = *(uint16_t *)cb;
					}
					p += 2;
					cb += 2;
				}

				dst += bufwidth;
			}

			dst += 8u - block_row_stride;
		} break;
		}

		if (dst == row_end) {
			row_base += block_row_stride;
			dst = row_base;
			row_end = row_base + ((uint32_t)blocksperrow * 8u);
		}
	}
}
