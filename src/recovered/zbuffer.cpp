/*******************************************************************************
 *                                O P E N  T S
 *******************************************************************************
 * SPDX-License-Identifier: GPL-3.0-or-later
 * Copyright 2026 OpenTS contributors
 *
 * See LICENSE.md for applicable additional terms and warranty disclaimers.
 ******************************************************************************/
// Function comment blocks from mischa85/RedAlert2 392e904 src/zbuffer.cpp (matching RA2 1.006 decomp), placed above our
// own notes; code unchanged.
// Parameter names from mischa85/RedAlert2 392e904 src/zbuffer.cpp where both define the function (names only).
// Local variable names from mischa85/RedAlert2 392e904 src/zbuffer.cpp where both functions declare the same local types (names only).

// zbuffer.cpp: the original translation unit re-filed as one source (Route A): the accepted
// bodies of its canonical compile group in address order, under the unit's own header
// state (its switches and includes in their original order). The module map
// manifests/original_target/modules/zbuffer.json carries the addresses and the rows that
// stay in their own files.


#include "shared/unknown_static_rect.hpp"
#include "shared/blit.hpp"


// Header-static dynamic initializer of the unit at 0x00775B60 (the alpha
// buffer, abuffer.cpp): the file-local all-zero Rect of
// shared/unknown_static_rect.hpp (0x00B2AF80). The ZBuffer members follow.


/// <summary>
/// Creates a depth buffer covering the area specified.
/// The buffer is a 16 bit surface that the depth aware blitters test and write against. It
/// is treated as a ring rather than a plain bitmap, so that the tactical map can scroll it
/// by sliding its origin. It starts out cleared to maximum depth, so that the first objects
/// drawn always pass the depth test.
/// </summary>
/// <param name="rect">The area the buffer is to cover.</param>
// Construct the 16-bit ring stored at original global 00839C6C.
// ZBuffer is the existing project name hypothesis, not an asserted RTTI identity.
ZBuffer::ZBuffer(Rect rect)
{
    Width=rect.Width;
    Height=rect.Height;
    Area=rect;
    BufferSize=(int)((unsigned int)Width*(unsigned int)Height*2u);
    Surface=new BSurface(Width,Height,2);
    Surface->Fill_Rect(Rect(0,0,Width,Height),0xffff);
    BufferHead=(char *)Surface->Lock(Point2D(0,0));
    BufferOffset=0;
    MaxValue=0x8000;
    BufferTail=BufferHead+(int)((unsigned int)Width*(unsigned int)Height*2u);
    Surface->Unlock();
}


/// <summary>
/// Copies the depth buffer out to a surface.
/// The buffer is unwrapped as it is copied, so the destination receives the region laid out
/// the way it is displayed rather than the way it happens to be stored.
/// </summary>
/// <param name="surface">The surface to copy into.</param>
/// <param name="rect">The region of the destination surface to copy into.</param>
// Copy an area-sized block of alpha values, starting at the ring's current
// origin, into surface at the area's position (a view of the buffer).
void ZBuffer::Copy_To(::Surface &surface, Rect rect)
{
    unsigned short *surfbuffptr = (unsigned short *)surface.Lock(Point2D(rect.X, rect.Y));
    unsigned short *pixptr = (unsigned short *)(BufferHead + BufferOffset);
    int steps = surface.Stride() / 2 - rect.Width;
    if (surfbuffptr && rect.Height > 0) {
        for (int row = rect.Height; row; --row) {
            for (int column = rect.Width; column > 0; --column) {
                *surfbuffptr++ = *pixptr++;
                pixptr = Wrap_Pointer(pixptr);
            }
            surfbuffptr += steps;
        }
    }
    surface.Unlock();
}


ZBuffer::~ZBuffer() { delete Surface; }


/// <summary>
/// Fills a run of depth entries with the value specified.
/// This is the low level fill routine that the pan and update code use once they have
/// worked out where in the buffer to write. It works in raw buffer addresses rather than
/// in coordinates.
/// </summary>
/// <param name="dst">The buffer address to begin filling at.</param>
/// <param name="size">The number of depth entries to fill.</param>
/// <param name="value">The depth value to fill with.</param>
/// <remarks>The run is not wrapped. The caller must split any fill that would otherwise
/// run off the end of the buffer.</remarks>
// Store value into count 16-bit entries from pointer: a leading single
// entry when pointer is not 4-byte aligned, then packed pairs, then a final
// single entry. As in the original, the final entry is written only when at
// least one pair was (the tail address stays null otherwise).
void ZBuffer::Set(unsigned short *dst, int size, unsigned short value)
{
    if (((unsigned int)dst & 2u) && size) {
        *dst++=value;
        --size;
    }
    unsigned int pairs=(unsigned int)(size>>1);
    unsigned int *tail=0;
    if(pairs) {
        unsigned int packed=((unsigned int)value<<16)+(unsigned int)value;
        unsigned int *output=(unsigned int *)dst;
        for(unsigned int remaining=pairs;remaining;--remaining) *output++=packed;
        tail=output;
    }
    if((size&1u) && tail) *(unsigned short *)tail=value;
}


/// <summary>
/// Scrolls the depth buffer by the amount specified.
/// This routine is what makes the buffer cheap for the tactical map to scroll. The depth
/// entries are never moved; the buffer's origin slides instead, and only the rows and
/// columns that have just come into view are reset. A scroll far enough to leave nothing
/// worth keeping resets the whole buffer instead.
/// </summary>
/// <param name="x">The distance to scroll horizontally, in pixels.</param>
/// <param name="y">The distance to scroll vertically, in pixels.</param>
/// <param name="value">The depth value to reset the newly exposed area to.</param>
// Scroll the ring by dx columns and dy rows (the view moved): move the ring
// origin and clear the newly exposed columns and rows to value. A move
// larger than the buffer clears all of it and resets MaxValue.
void ZBuffer::Pan(int dx, int dy, unsigned short value)
{
    int x = (BufferOffset / 2) % Width;
    if (abs(dx) > Width || abs(dy) > Height) {
        Fill(value);
        MaxValue = 0x8000;
        return;
    }
    int right = x + dx;
    if (dx) {
        BufferOffset += dx * 2;
        char *pointer = BufferHead + BufferOffset;
        if (pointer < BufferHead) pointer += BufferSize;
        pointer = (char *)Wrap_Pointer((unsigned short *)pointer);
        BufferOffset = pointer - BufferHead;
        if (dx < 0) {
            if (right < 0) {
                Clear_Region_007761A0(value, Rect(0, 0, x, Height));
                right += Width;
                Clear_Region_007761A0(value, Rect(right, 0, Width - right, Height));
            } else {
                Clear_Region_007761A0(value, Rect(right, 0, -dx, Height));
            }
        } else {
            if (right >= Width) {
                Clear_Region_007761A0(value, Rect(x, 0, Width - x, Height));
                right -= Width;
                Clear_Region_007761A0(value, Rect(0, 0, right, Height));
            } else {
                Clear_Region_007761A0(value, Rect(x, 0, dx, Height));
            }
        }
    }
    if (dy) {
        int row = (BufferOffset / 2) / Width;
        int old_offset = BufferOffset;
        MaxValue -= dy;
        BufferOffset += Width * dy * 2;
        row += dy;
        char *pointer = BufferHead + BufferOffset;
        if (pointer < BufferHead) pointer += BufferSize;
        pointer = (char *)Wrap_Pointer((unsigned short *)pointer);
        BufferOffset = pointer - BufferHead;
        if (dy < 0) {
            if (row < 0) {
                Set((unsigned short *)BufferHead, old_offset / 2, value);
                Set((unsigned short *)(BufferHead + BufferOffset), (Height * Width * 2 - BufferOffset) / 2, value);
            } else {
                Set((unsigned short *)(BufferHead + BufferOffset), (old_offset - BufferOffset) / 2, value);
            }
        } else {
            if (row >= Height) {
                Set((unsigned short *)(BufferHead + old_offset), (Height * Width * 2 - old_offset) / 2, value);
                Set((unsigned short *)BufferHead, BufferOffset / 2, value);
            } else {
                Set((unsigned short *)(BufferHead + old_offset), (BufferOffset - old_offset) / 2, value);
            }
        }
    }
}


void ZBuffer::Fill(unsigned short color) { Surface->Fill_Rect(Rect(0,0,Width,Height),color); }


void ZBuffer::Clear_Region_007761A0(unsigned short color, Rect area) { Surface->Fill_Rect(area,color); }


/// <summary>
/// Resets a region of the depth buffer back to maximum depth.
/// The tactical map calls this routine for every region it is about to redraw, so that the
/// depth compares there start from a clean slate. Unlike Fill, the region is located
/// through the current pan offset and is allowed to straddle the wrap point.
/// </summary>
/// <param name="rect">The region of the buffer to reset.</param>
// Clear a by-value rectangle to FFFF in the ring at original global00839C6C.
void ZBuffer::Update(Rect rect)
{
    char *locked=(char *)Surface->Lock(Point2D(rect.X,rect.Y));
    Surface->Unlock();
    char *pointer=locked+BufferOffset;
    if(pointer>=BufferTail) pointer-=BufferSize;
    if(rect.Height>0) {
        int rows=rect.Height;
        do {
            if(pointer+(unsigned int)rect.Width*2u>=BufferTail) {
                unsigned int first=(unsigned int)(BufferTail-pointer)>>1;
                Set((unsigned short *)pointer,first,0xffff);
                Set((unsigned short *)BufferHead,rect.Width-first,0xffff);
            } else {
                Set((unsigned short *)pointer,rect.Width,0xffff);
            }
            pointer+=(unsigned int)Width*2u;
            if(pointer>=BufferTail) pointer-=BufferSize;
        } while(--rows);
    }
}


/// <summary>
/// Fetches the address of a point within the depth buffer.
/// The point is given in buffer coordinates. The current pan offset is applied to it and
/// the result is wrapped, so the address returned is where that entry actually lives.
/// </summary>
/// <param name="pos">The point within the buffer to locate.</param>
/// <returns>Returns with the address of the entry within the depth buffer.</returns>
// Point-by-value getter for the ring at original global00839C6C.
// ZBuffer remains the existing project-name hypothesis.
unsigned short *ZBuffer::Get_Buffer_Offset(Point2D pos)
{
    char *pointer=(char *)Surface->Lock(pos);
    Surface->Unlock();
    char *result=pointer+BufferOffset;
    if(result>=BufferTail) result-=BufferSize;
    return (unsigned short *)result;
}
