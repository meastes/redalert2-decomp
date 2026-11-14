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


// Construct the 16-bit ring stored at original global 00839C6C.
// ZBuffer is the existing project name hypothesis, not an asserted RTTI identity.
ZBuffer::ZBuffer(Rect area)
{
    Width=area.Width;
    Height=area.Height;
    Area=area;
    BufferSize=(int)((unsigned int)Width*(unsigned int)Height*2u);
    Surface=new BSurface(Width,Height,2);
    Surface->Fill_Rect(Rect(0,0,Width,Height),0xffff);
    BufferHead=(char *)Surface->Lock(Point2D(0,0));
    BufferOffset=0;
    MaxValue=0x8000;
    BufferTail=BufferHead+(int)((unsigned int)Width*(unsigned int)Height*2u);
    Surface->Unlock();
}


// Copy an area-sized block of alpha values, starting at the ring's current
// origin, into surface at the area's position (a view of the buffer).
void ZBuffer::Copy_To(::Surface &surface, Rect area)
{
    unsigned short *pixels = (unsigned short *)surface.Lock(Point2D(area.X, area.Y));
    unsigned short *alpha = (unsigned short *)(BufferHead + BufferOffset);
    int skip = surface.Stride() / 2 - area.Width;
    if (pixels && area.Height > 0) {
        for (int row = area.Height; row; --row) {
            for (int column = area.Width; column > 0; --column) {
                *pixels++ = *alpha++;
                alpha = Wrap_Pointer(alpha);
            }
            pixels += skip;
        }
    }
    surface.Unlock();
}


ZBuffer::~ZBuffer() { delete Surface; }


// Store value into count 16-bit entries from pointer: a leading single
// entry when pointer is not 4-byte aligned, then packed pairs, then a final
// single entry. As in the original, the final entry is written only when at
// least one pair was (the tail address stays null otherwise).
void ZBuffer::Set(unsigned short *pointer, int count, unsigned short value)
{
    if (((unsigned int)pointer & 2u) && count) {
        *pointer++=value;
        --count;
    }
    unsigned int pairs=(unsigned int)(count>>1);
    unsigned int *tail=0;
    if(pairs) {
        unsigned int packed=((unsigned int)value<<16)+(unsigned int)value;
        unsigned int *output=(unsigned int *)pointer;
        for(unsigned int remaining=pairs;remaining;--remaining) *output++=packed;
        tail=output;
    }
    if((count&1u) && tail) *(unsigned short *)tail=value;
}


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


// Clear a by-value rectangle to FFFF in the ring at original global00839C6C.
void ZBuffer::Update(Rect area)
{
    char *locked=(char *)Surface->Lock(Point2D(area.X,area.Y));
    Surface->Unlock();
    char *pointer=locked+BufferOffset;
    if(pointer>=BufferTail) pointer-=BufferSize;
    if(area.Height>0) {
        int rows=area.Height;
        do {
            if(pointer+(unsigned int)area.Width*2u>=BufferTail) {
                unsigned int first=(unsigned int)(BufferTail-pointer)>>1;
                Set((unsigned short *)pointer,first,0xffff);
                Set((unsigned short *)BufferHead,area.Width-first,0xffff);
            } else {
                Set((unsigned short *)pointer,area.Width,0xffff);
            }
            pointer+=(unsigned int)Width*2u;
            if(pointer>=BufferTail) pointer-=BufferSize;
        } while(--rows);
    }
}


// Point-by-value getter for the ring at original global00839C6C.
// ZBuffer remains the existing project-name hypothesis.
unsigned short *ZBuffer::Get_Buffer_Offset(Point2D point)
{
    char *pointer=(char *)Surface->Lock(point);
    Surface->Unlock();
    char *result=pointer+BufferOffset;
    if(result>=BufferTail) result-=BufferSize;
    return (unsigned short *)result;
}
