/*******************************************************************************
 *                                O P E N  T S
 *******************************************************************************
 * SPDX-License-Identifier: GPL-3.0-or-later
 * Copyright 2025 Electronic Arts Inc.
 * Copyright 2026 OpenTS contributors
 *
 * Contains material derived from Electronic Arts source code.
 * Modified by OpenTS contributors, 2026.
 * EA's GPLv3 Section 7 additional terms and supplemental warranty
 * disclaimers apply; see LICENSE.md.
 ******************************************************************************/
// Function comment blocks from mischa85/RedAlert2 392e904 src/blit.cpp (matching RA2 1.006 decomp), placed above our
// own notes; code unchanged.

// blit.cpp: the original translation unit re-filed as one source (Route A): the accepted
// bodies of its canonical compile group in address order, under the unit's own header
// state (its switches and includes in their original order). The module map
// manifests/original_target/modules/blit.json carries the addresses and the rows that
// stay in their own files.


#include "shared/blit.hpp"
#include "shared/rle_blitter.hpp"


/***********************************************************************************************
 * Buffer_Size -- Determines size of buffer for given dimensions.                              *
 *                                                                                             *
 *    This routine will determine the byte size of a buffer if it were to hold the pixels      *
 *    of the dimensions specified. It takes into account the bytes per pixel.                  *
 *                                                                                             *
 * INPUT:   surface  -- The surface to base the buffer size calculation upon.                  *
 *                                                                                             *
 *          width    -- Pixel width of a graphic region.                                       *
 *                                                                                             *
 *          height   -- Pixel height of a graphic region.                                      *
 *                                                                                             *
 * OUTPUT:  Returns with the number of bytes such a region would consume if it were linearly   *
 *          packed into a memory buffer.                                                       *
 *                                                                                             *
 * WARNINGS:   none                                                                            *
 *                                                                                             *
 * HISTORY:                                                                                    *
 *   02/07/1997 JLB : Created.                                                                 *
 *=============================================================================================*/
// (0x00434940)
// Bytes needed for a width x height block of the surface's pixels.

int __fastcall Buffer_Size(Surface &surface, int width, int height)
{
    return surface.Bytes_Per_Pixel() * width * height;
}


/***********************************************************************************************
 * To_Buffer -- Copies a graphic region into a linear RAM buffer.                              *
 *                                                                                             *
 *    This routine will copy the graphic rectangle specified, into a RAM buffer. The size of   *
 *    the RAM buffer must be big enough to hold the pixel data. Use the Buffer_Size() function *
 *    to determine how big it must be.                                                         *
 *                                                                                             *
 * INPUT:   surface  -- The surface to copy the pixel data from.                               *
 *                                                                                             *
 *          rect     -- The graphic rectangle to copy from.                                    *
 *                                                                                             *
 *          buffer   -- Reference to the buffer that will be filled with the pixel data.       *
 *                                                                                             *
 * OUTPUT:  bool; Was the data copy performed without error?                                   *
 *                                                                                             *
 * WARNINGS:   none                                                                            *
 *                                                                                             *
 * HISTORY:                                                                                    *
 *   02/07/1997 JLB : Created.                                                                 *
 *=============================================================================================*/
// (0x00434960)
// Copies rect of surface into buffer (packed rows) through a temporary
// BSurface over the buffer memory; false for an empty rect.
// Unresolved: the meaning of the last Copy_From flag (passed true).

bool __fastcall To_Buffer(Surface &surface, Rect const &rect, Buffer &buffer)
{
    if (!rect.Is_Valid()) {
        return false;
    }
    BSurface to(rect.Width, rect.Height, surface.Bytes_Per_Pixel(), buffer.Get_Buffer());
    return to.Copy_From(Rect(0, 0, rect.Width, rect.Height), surface, rect, false, true);
}


/***********************************************************************************************
 * From_Buffer -- Copy graphic data from a buffer to a surface.                                *
 *                                                                                             *
 *    This routine will take pixel data and move it from the specified buffer and into the     *
 *    surface rectangle specified. It is the counterpart routine of To_Buffer().               *
 *                                                                                             *
 * INPUT:   surface  -- The surface to store the pixel data to.                                *
 *                                                                                             *
 *          rect     -- The destination rectangle to store the pixel data to.                  *
 *                                                                                             *
 *          buffer   -- Reference to the buffer that contains the pixel data.                  *
 *                                                                                             *
 * OUTPUT:  bool; Was the pixel data copy performed without error?                             *
 *                                                                                             *
 * WARNINGS:   none                                                                            *
 *                                                                                             *
 * HISTORY:                                                                                    *
 *   02/07/1997 JLB : Created.                                                                 *
 *=============================================================================================*/
// (0x00434A20)
// Copies buffer (packed rows) into rect of surface through a temporary
// BSurface over the buffer memory; false for an empty rect.

bool __fastcall From_Buffer(Surface &surface, Rect const &rect, Buffer &buffer)
{
    if (!rect.Is_Valid()) {
        return false;
    }
    BSurface from(rect.Width, rect.Height, surface.Bytes_Per_Pixel(), buffer.Get_Buffer());
    return surface.Copy_From(rect, from, Rect(0, 0, rect.Width, rect.Height), false, true);
}


/***********************************************************************************************
 * Bit_Blit -- Blits data to a surface w/ clipping.                                            *
 *                                                                                             *
 *    This routine will take source pixel data and blit it to the surface specified while      *
 *    also performing clipping on both the source and the destination data. Typical users of   *
 *    this routine would be to draw shape (sprite) data.                                       *
 *                                                                                             *
 * INPUT:   dest     -- Destintaion surface rect. This specifies the destination surface and   *
 *                      any coordinate clipping rectangle.                                     *
 *                                                                                             *
 *          destrect -- The destination rectangle of the blit. The coordinates are relative    *
 *                      to the destination clipping rectangle.                                 *
 *                                                                                             *
 *          source   -- Source surface rect. This specifies the source surface as well as any  *
 *                      clipping rectangle it may contain.                                     *
 *                                                                                             *
 *          srcrect  -- The rectange, relative to the source clipping rectangle, that          *
 *                      specifies the source blit data. It is presumed that the dimensions of  *
 *                      the source rectangle are the same as the destination rectangle.        *
 *                                                                                             *
 *          blitter  -- The blitter to use for moving the source pixels to the destination     *
 *                      surface.                                                               *
 *                                                                                             *
 * OUTPUT:  bool; Was the blit performed even if it was for only a single pixel. Failure would *
 *                indicate that the blit was completely clipped away.                          *
 *                                                                                             *
 * WARNINGS:   none                                                                            *
 *                                                                                             *
 * HISTORY:                                                                                    *
 *   05/19/1997 JLB : Created.                                                                 *
 *=============================================================================================*/
// (0x00434AE0)
// Blits with the surfaces' own rectangles as clipping windows.

bool __fastcall Bit_Blit(Surface &dest, Rect const &destrect, Surface &source, Rect const &sourcerect,
                         Blitter const &blitter, int unknown1, int unknown2, int unknown3, int unknown4)
{
    return Bit_Blit(dest, dest.Get_Rect(), destrect, source, source.Get_Rect(), sourcerect, blitter,
                    unknown1, unknown2, unknown3, unknown4);
}


/***********************************************************************************************
 * Bit_Blit -- Blit a block of pixels to the destination surface.                              *
 *                                                                                             *
 *    This routine will blit a block of pixels and perform clipping on the blit as controlled  *
 *    by the clipping rectangles.                                                              *
 *                                                                                             *
 * INPUT:   dest     -- Surface to blit to.                                                    *
 *                                                                                             *
 *          dcliprect-- The destination surface clipping rectangle.                            *
 *                                                                                             *
 *          ddrect   -- The destination rect of the blit. It is relative to the clipping       *
 *                      rectangle and will be clipped against same.                            *
 *                                                                                             *
 *          source   -- The source surface to blit from.                                       *
 *                                                                                             *
 *          scliprect-- The source surface clipping rectangle.                                 *
 *                                                                                             *
 *          ssrect   -- The source rectangle of the blit. It is relative to the source         *
 *                      clipping rectangle and will be clipped against same.                   *
 *                                                                                             *
 *          blitter  -- The blitter to use for blitting of this rectangle.                     *
 *                                                                                             *
 * OUTPUT:  bool; Was the blit performed? A 'false' return value would indicate that the       *
 *                blit was clipped into nothing.                                               *
 *                                                                                             *
 * WARNINGS:   none                                                                            *
 *                                                                                             *
 * HISTORY:                                                                                    *
 *   05/27/1997 JLB : Created.                                                                 *
 *=============================================================================================*/
// Windowed pixel-run blit, full original00434B40..00435057.
// Native839C6C carries the interpolated depth values; names of buffer classes remain hypotheses.
bool __fastcall Bit_Blit(Surface &dest, Rect const &destwindow, Rect const &destarea,
    Surface &source, Rect const &sourcewindow, Rect const &sourcearea,
    Blitter const &blitter, int depth, int ramp_index, int alpha, int unknown)
{
    Rect src = sourcearea;
    Rect dst = destarea;
    bool overlap = false;
    void *dest_pixels = 0;
    void *source_pixels = 0;
    unsigned int depth_pointer = 0;
    unsigned int alpha_pointer = 0;
    int depth_level = 0;
    int depth_pitch = 0;
    int alpha_pitch = 0;
    int fraction = 0;
    int increment = 0;
    int limit = 0;
    int ratio = 0;
    BlitRamp *ramp = &BlitRamps[ramp_index];
    if (!Prep_For_Blit(dest, destwindow, dst, source, sourcewindow, src, overlap, dest_pixels, source_pixels)) return false;
    if (AlphaBuffer) {
        Point2D point;
        point = destwindow.Top_Left() + dst.Top_Left();
        point.Y -= AlphaBuffer->Area.Y;
        alpha_pointer = (unsigned int)AlphaBuffer->Get_Buffer_Offset(point);
    }
    if (DepthBuffer) {
        Point2D point;
        point = destwindow.Top_Left() + dst.Top_Left();
        point.Y -= DepthBuffer->Area.Y;
        depth_pointer = (unsigned int)DepthBuffer->Get_Buffer_Offset(point);
        fraction = 0;
        increment = ramp->Numerator;
        limit = ramp->Denominator;
        ratio = limit / increment;
        if (ramp->Flag) {
            depth_level = (unsigned short)DepthBuffer->Get_Scroll_Delta(destwindow.Y + dst.Y - DepthBuffer->Area.Y);
            depth_level += depth;
            depth_level = (depth_level / ramp->Divisor) * ramp->Divisor;
        } else {
            depth_level = (unsigned short)DepthBuffer->Get_Scroll_Delta(destwindow.Y + dst.Y + dst.Height - 1 - DepthBuffer->Area.Y);
            depth_level += depth;
            depth_level = (depth_level / ratio) * ratio;
            depth_level -= dst.Height / ratio;
            fraction = limit - dst.Height % ratio;
            if (fraction == limit) {
                fraction = 0;
                depth_level += ramp->Step;
            }
        }
    }
    if (dst.Width * dest.Bytes_Per_Pixel() == dest.Stride() && dest.Stride() == source.Stride()) {
        int count = src.Height * src.Width < dst.Height * dst.Width ? src.Height * src.Width : dst.Height * dst.Width;
        if (overlap) blitter.BlitBackward(dest_pixels, source_pixels, count, 0, 0, 0, 1000);
        else blitter.BlitForward(dest_pixels, source_pixels, count, 0, 0, 0, 1000, 0);
    } else {
        int source_pitch = source.Stride();
        int dest_pitch = dest.Stride();
        if (DepthBuffer) depth_pitch = DepthBuffer->Width;
        if (AlphaBuffer) alpha_pitch = AlphaBuffer->Width;
        if (overlap) {
            source_pitch = -source_pitch;
            dest_pitch = -dest_pitch;
            depth_pitch = -depth_pitch;
            alpha_pitch = -alpha_pitch;
            source_pixels = (char *)source_pixels + (src.Height - 1) * source.Stride();
            dest_pixels = (char *)dest_pixels + (dst.Height - 1) * dest.Stride();
            if (DepthBuffer) {
                depth_pointer = DepthBuffer->Wrap_Overflow(depth_pointer + 2 * src.Height - 2 * DepthBuffer->Width);
                depth_level -= src.Height - 1;
            }
            if (AlphaBuffer) alpha_pointer = AlphaBuffer->Wrap_Overflow(alpha_pointer + 2 * src.Height - 2 * AlphaBuffer->Width);
        }
        int rows = src.Height < dst.Height ? src.Height : dst.Height;
        if (overlap) {
            for (int row = 0; row < rows; ++row) {
                blitter.BlitBackward(dest_pixels, source_pixels, src.Width, depth_level, (void *)depth_pointer, (void *)alpha_pointer, alpha);
                dest_pixels = (char *)dest_pixels + dest_pitch;
                source_pixels = (char *)source_pixels + source_pitch;
                if (DepthBuffer) {
                    depth_pointer += 2 * depth_pitch;
                    depth_pointer = DepthBuffer->Wrap_Underflow(depth_pointer);
                    fraction += increment;
                    if (fraction >= limit) {
                        depth_level += ramp->Step;
                        fraction -= limit;
                    }
                }
                if (AlphaBuffer) {
                    alpha_pointer += 2 * alpha_pitch;
                    alpha_pointer = AlphaBuffer->Wrap_Underflow(alpha_pointer);
                }
            }
        } else {
            for (int row = 0; row < rows; ++row) {
                blitter.BlitForward(dest_pixels, source_pixels, src.Width, depth_level, (void *)depth_pointer, (void *)alpha_pointer, alpha, 0);
                dest_pixels = (char *)dest_pixels + dest_pitch;
                source_pixels = (char *)source_pixels + source_pitch;
                if (DepthBuffer) {
                    depth_pointer += 2 * depth_pitch;
                    depth_pointer = DepthBuffer->Wrap_Overflow(depth_pointer);
                    fraction += increment;
                    if (fraction >= limit) {
                        depth_level += ramp->Step;
                        fraction -= limit;
                    }
                }
                if (AlphaBuffer) {
                    alpha_pointer += 2 * alpha_pitch;
                    alpha_pointer = AlphaBuffer->Wrap_Overflow(alpha_pointer);
                }
            }
        }
    }
    dest.Unlock();
    source.Unlock();
    return true;
}


/***********************************************************************************************
 * RLE_Blit -- Blits RLE compressed data without extra clipping.                               *
 *                                                                                             *
 *    This routine will blit a rectangle of RLE compressed data to the specified surface. It   *
 *    is functionally similar to the other RLE blit routine, but does not use any sub          *
 *    clipping rectangles. The blit is naturally clipped to the edge of the destination        *
 *    surface.                                                                                 *
 *                                                                                             *
 * INPUT:   dest     -- Reference to the destination surface.                                  *
 *                                                                                             *
 *          destrect -- The destination rectangle to draw the pixels to.                       *
 *                                                                                             *
 *          source   -- Reference to the source RLE surface data.                              *
 *                                                                                             *
 *          sourcerect  -- The source rectangle of from the RLE surface to blit from.          *
 *                                                                                             *
 *          blitter  -- Reference to the blitter to perform the blit operation with.           *
 *                                                                                             *
 * OUTPUT:  bool; Was a blit performed? A 'false' value would mean that the blit has been      *
 *                clipped into nothing.                                                        *
 *                                                                                             *
 * WARNINGS:   The dimensions of the source and destination rectangles should be the same      *
 *             until such time that the blitter can support scaling (as of this writing, it    *
 *             cannot).                                                                        *
 *                                                                                             *
 * HISTORY:                                                                                    *
 *   05/27/1997 JLB : Created.                                                                 *
 *=============================================================================================*/
// (0x00435060)
// RLE blit with the surfaces' own rectangles as clipping windows.

bool __fastcall RLE_Blit(Surface &dest, Rect const &destrect, Surface &source, Rect const &sourcerect,
                         RLEBlitter const &blitter, int unknown1, int unknown2, int unknown3, int unknown4)
{
    return RLE_Blit(dest, dest.Get_Rect(), destrect, source, source.Get_Rect(), sourcerect, blitter,
                    unknown1, unknown2, unknown3, unknown4, 0, BlitPair(0, 0));
}


// Compressed scanline blit, native004350C0..00435530 plus15 frozenNOP.
extern unsigned char Blit_RLE_Default_Depth[];
/***********************************************************************************************
 * RLE_Blit -- Blits a rectangle of RLE compressed data to a surface.                          *
 *                                                                                             *
 *    This routine will blit a rectangle of REL compressed pixel data from a sprite to the     *
 *    surface specified. Appropriate clipping and coordinate adjustments will occur as         *
 *    controlled by the parameters. This is the workhorse RLE blit dispatcher routine.         *
 *                                                                                             *
 * INPUT:   dest     -- The destination surface to blit to.                                    *
 *                                                                                             *
 *          dcliprect-- The clipping rectangle to use on the destination. Pixels won't be      *
 *                      drawn outside of this rectangle and the destination rect coordinates   *
 *                      are biased to this clipping rectange.                                  *
 *                                                                                             *
 *          ddrect   -- The destination rectangle of the blit. The upper left coordinates are  *
 *                      biased to the destination clipping rectangle when blitted. Thus,       *
 *                      a dest X,Y position of 0,0 really means the upper left corner of the   *
 *                      destination clipping rectangle.                                        *
 *                                                                                             *
 *          source   -- The source surface of the RLE compressed data.                         *
 *                                                                                             *
 *          scliprect-- It is quite likely that this will be the full size of the source       *
 *                      surface.                                                               *
 *                                                                                             *
 *          ssrect   -- The source rectangle to blit from within the source surface. It        *
 *                      behaves similarly to the ddrect parameter, but for the source. The     *
 *                      width and height of this rectangle should match the width and height   *
 *                      of the ddrect parameter (scaling is not yet supported).                *
 *                                                                                             *
 *          blitter  -- The blitter to use for this pixel copy. It must be an RLE blitter.     *
 *                                                                                             *
 * OUTPUT:  bool; Did the blit draw at least one pixel?                                        *
 *                                                                                             *
 * WARNINGS:   none                                                                            *
 *                                                                                             *
 * HISTORY:                                                                                    *
 *   05/24/1997 JLB : Created.                                                                 *
 *=============================================================================================*/
bool __fastcall RLE_Blit(Surface &dest, Rect const &destwindow, Rect const &destarea,
    Surface &source, Rect const &sourcewindow, Rect const &sourcearea,
    RLEBlitter const &blitter, int depth, int ramp_index, int alpha, int unknown,
    Surface *depth_shape, BlitPair depth_point)
{
    unsigned int depth_pointer = 0;
    unsigned int alpha_pointer = 0;
    char *shape_pixels = 0;
    int depth_pitch = 0;
    int alpha_pitch = 0;
    int depth_level = 0;
    Rect src = sourcearea;
    Rect dst = destarea;
    Rect shape_area;
    int fraction = 0;
    int increment = 0;
    int limit = 0;
    int ratio = 0;
    int shape_width = 0;
    BlitRamp *ramp = &BlitRamps[ramp_index];
    if (!Clip_Blit_Rects(dst, destwindow, src, sourcewindow)) return false;
    int left = src.X - sourcewindow.X;
    int top = src.Y - sourcewindow.Y;
    void *dest_pixels = dest.Lock(destwindow.Top_Left() + dst.Top_Left());
    if (!dest_pixels) return false;
    if (DepthBuffer) {
        Point2D point;
        point = destwindow.Top_Left() + dst.Top_Left();
        point.Y -= DepthBuffer->Area.Y;
        depth_pointer = (unsigned int)DepthBuffer->Get_Buffer_Offset(point);
        depth_pitch = DepthBuffer->Width;
        fraction = 0;
        increment = ramp->Numerator;
        limit = ramp->Denominator;
        ratio = limit / increment;
        if (ramp->Flag) {
            depth_level = (unsigned short)DepthBuffer->Get_Scroll_Delta(point.Y);
            depth_level += depth;
            depth_level = (depth_level / ramp->Divisor) * ramp->Divisor;
        } else {
            depth_level = (unsigned short)DepthBuffer->Get_Scroll_Delta(destwindow.Y + destarea.Y + destarea.Height - 1 - DepthBuffer->Area.Y);
            depth_level += depth;
            if (!depth_shape) {
                depth_level = (depth_level / ratio) * ratio;
                int lines = destarea.Y + destarea.Height - dst.Y;
                depth_level -= lines / ratio;
                fraction = limit - lines % ratio;
                if (fraction == limit) {
                    fraction = 0;
                    depth_level += ramp->Step;
                }
            }
        }
        if (depth_shape) {
            shape_area = depth_shape->Get_Rect();
            shape_width = shape_area.Width;
            shape_pixels = (char *)depth_shape->Lock(Point2D(shape_area.X, shape_area.Y) + Point2D(depth_point.X, depth_point.Y) + Point2D(left, top));
        } else shape_pixels = (char *)Blit_RLE_Default_Depth;
    }
    if (AlphaBuffer) {
        Point2D point;
        point = destwindow.Top_Left() + dst.Top_Left();
        point.Y -= AlphaBuffer->Area.Y;
        alpha_pointer = (unsigned int)AlphaBuffer->Get_Buffer_Offset(point);
        alpha_pitch = AlphaBuffer->Width;
    }
    void *source_pixels = source.Lock();
    if (!source_pixels) {
        dest.Unlock();
        return false;
    }
    while (top > 0) {
        source_pixels = (unsigned char *)source_pixels + *(unsigned short *)source_pixels;
        --top;
    }
    int dest_pitch = dest.Stride();
    int rows = src.Height < dst.Height ? src.Height : dst.Height;
    for (int row = 0; row < rows; ++row) {
        blitter.Blit(dest_pixels, (unsigned short *)source_pixels + 1, src.Width, left,
            depth_level, (void *)depth_pointer, (void *)alpha_pointer, alpha, 0, shape_pixels);
        source_pixels = (unsigned char *)source_pixels + *(unsigned short *)source_pixels;
        dest_pixels = (char *)dest_pixels + dest_pitch;
        if (DepthBuffer) {
            depth_pointer += 2 * depth_pitch;
            depth_pointer = DepthBuffer->Wrap_Overflow(depth_pointer);
            if (depth_shape) shape_pixels += shape_width;
            else {
                fraction += increment;
                if (fraction >= limit) {
                    depth_level += ramp->Step;
                    fraction -= limit;
                }
            }
            alpha_pointer += 2 * alpha_pitch;
            alpha_pointer = AlphaBuffer->Wrap_Overflow(alpha_pointer);
        }
    }
    dest.Unlock();
    source.Unlock();
    return true;
}
