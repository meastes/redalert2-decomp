/*******************************************************************************
 *                                O P E N  T S
 *******************************************************************************
 * SPDX-License-Identifier: GPL-3.0-or-later
 * Copyright 2026 OpenTS contributors
 *
 * See LICENSE.md for applicable additional terms and warranty disclaimers.
 ******************************************************************************/
// Function comment blocks from mischa85/RedAlert2 392e904 src/stbuffer.cpp (matching RA2 1.006 decomp), placed above our
// own notes; code unchanged.

// stbuffer.cpp: the original translation unit re-filed as one source (Route A): the accepted
// bodies of its canonical compile group in address order, under the unit's own header
// state (its switches and includes in their original order). The module map
// manifests/original_target/modules/stbuffer.json carries the addresses and the rows that
// stay in their own files.


#include "shared/shape_file.hpp"
#define STATIC_BUFFER_DECLARE_LIFETIME
#include "shared/rect.hpp"
#include "shared/surface_region.hpp"
#include "shared/static_buffer.hpp"
#include "shared/crt_memory.hpp"
#include "shared/surface.hpp"
#include "shared/rle_engine.hpp"


// Header-static dynamic initializer of the StaticBufferClass unit 0x0069A180-0x0069A42F: the shape_file.hpp
// empty rect (0x00ABD538).


/// <summary>
/// Creates a static buffer of the capacity requested.
/// This routine will allocate the whole block up front. Space is only ever handed out from
/// the front of it, since individual entries are never released -- Reset is what reclaims
/// the buffer for reuse.
/// </summary>
/// <param name="size">The capacity of the buffer, in bytes.</param>
// (0x0069A1A0; was UnknownBuffer_0069a1a0)
// Buffer and Current at their start.

StaticBufferClass::StaticBufferClass(int size)
{
    Capacity = size;
    unsigned char *data = new unsigned char[size];
    Buffer = data;
    Current = data;
}


/// <summary>
/// Frees the memory held by the static buffer.
/// Every entry handed out by Add points into this one block, so none of them outlive the
/// buffer they were cached in.
/// </summary>
// (0x0069A1C0)

StaticBufferClass::~StaticBufferClass()
{
    if (Buffer != 0) {
        delete [] Buffer;
    }
}


/// <summary>
/// Adds a compressed copy of a surface region to the buffer.
/// This routine is used by the voxel drawing cache to stash a freshly rendered voxel so
/// that later frames can blit the image back instead of rendering it again. The image is
/// run length compressed as it is copied out of the surface.
/// </summary>
/// <param name="surface">The surface to capture the image from.</param>
/// <param name="region">The area of the surface to capture, along with the draw offset to
/// remember with it.</param>
/// <returns>Returns with a pointer to the entry describing the cached image. Otherwise,
/// NULL is returned if the buffer has no room left.</returns>
// (0x0069A1D0)
// techno voxel draws 0x006D2xxx): appends an Entry header (point, size, data start) and the region's lines RLE
// compressed through the static line buffer 0x00ABD548; null (and nothing kept past the failure) when full. The
// line counter steps before the stride call (a while loop); Reserve tests the end before taking Current.
extern unsigned char CompressionBuffer[];     // 0x00ABD548 one compressed line

StaticBufferClass::Entry *StaticBufferClass::Add(Surface &surface, SurfaceRegion const &region)
{
    Entry *entry = (Entry *)Reserve(sizeof(Entry));
    if (entry == 0) {
        return 0;
    }
    entry->X = region.Point.X;
    entry->Y = region.Point.Y;
    entry->Width = region.Bounds.Width;
    entry->Height = region.Bounds.Height;
    entry->Data = Current;
    unsigned char *source = (unsigned char *)surface.Lock(region.Bounds.Top_Left());
    RLEEngine rle;
    int line = 0;
    while (line < region.Bounds.Height) {
        int length = rle.Line_Compress(source, CompressionBuffer, region.Bounds.Width);
        unsigned char *dest = Reserve(length);
        if (dest == 0) {
            surface.Unlock();
            return 0;
        }
        memcpy(dest, CompressionBuffer, length);
        line++;
        source += surface.Stride();
    }
    surface.Unlock();
    return entry;
}


/// <summary>
/// Adds a compressed copy of a clipped surface rectangle to the buffer.
/// This routine serves the same purpose as the region flavor above, except that the caller
/// states the draw offset to remember with the image rather than taking it from a surface
/// region.
/// </summary>
/// <param name="surface">The surface to capture the image from.</param>
/// <param name="cliprect">The area of the surface to capture.</param>
/// <param name="x">The horizontal draw offset to remember with the cached image.</param>
/// <param name="y">The vertical draw offset to remember with the cached image.</param>
/// <returns>Returns with a pointer to the entry describing the cached image. Otherwise,
/// NULL is returned if the buffer has no room left.</returns>
// (0x0069A300)
// no direct callers): the same for a rectangle stored with the point x, y.

StaticBufferClass::Entry *StaticBufferClass::Add(Surface &surface, Rect const &rect, int x, int y)
{
    Entry *entry = (Entry *)Reserve(sizeof(Entry));
    if (entry == 0) {
        return 0;
    }
    entry->X = x;
    entry->Y = y;
    entry->Width = rect.Width;
    entry->Height = rect.Height;
    entry->Data = Current;
    unsigned char *source = (unsigned char *)surface.Lock(rect.Top_Left());
    RLEEngine rle;
    int line = 0;
    while (line < rect.Height) {
        int length = rle.Line_Compress(source, CompressionBuffer, rect.Width);
        unsigned char *dest = Reserve(length);
        if (dest == 0) {
            surface.Unlock();
            return 0;
        }
        memcpy(dest, CompressionBuffer, length);
        line++;
        source += surface.Stride();
    }
    surface.Unlock();
    return entry;
}
