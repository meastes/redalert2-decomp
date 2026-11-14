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


// StaticBufferClass::StaticBufferClass(int size) (0x0069A1A0; was UnknownBuffer_0069a1a0): size bytes of storage,
// Buffer and Current at their start.

StaticBufferClass::StaticBufferClass(int size)
{
    Capacity = size;
    unsigned char *data = new unsigned char[size];
    Buffer = data;
    Current = data;
}


// StaticBufferClass::~StaticBufferClass() (0x0069A1C0): frees the storage.

StaticBufferClass::~StaticBufferClass()
{
    if (Buffer != 0) {
        delete [] Buffer;
    }
}


// StaticBufferClass::Entry *StaticBufferClass::Add(Surface &surface, SurfaceRegion const &region) (0x0069A1D0; the
// techno voxel draws 0x006D2xxx): appends an Entry header (point, size, data start) and the region's lines RLE
// compressed through the static line buffer 0x00ABD548; null (and nothing kept past the failure) when full. The
// line counter steps before the stride call (a while loop); Reserve tests the end before taking Current.
extern unsigned char unknown_line_buffer_abd548[];     // 0x00ABD548 one compressed line

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
        int length = rle.Line_Compress(source, unknown_line_buffer_abd548, region.Bounds.Width);
        unsigned char *dest = Reserve(length);
        if (dest == 0) {
            surface.Unlock();
            return 0;
        }
        memcpy(dest, unknown_line_buffer_abd548, length);
        line++;
        source += surface.Stride();
    }
    surface.Unlock();
    return entry;
}


// StaticBufferClass::Entry *StaticBufferClass::Add(Surface &surface, Rect const &rect, int x, int y) (0x0069A300;
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
        int length = rle.Line_Compress(source, unknown_line_buffer_abd548, rect.Width);
        unsigned char *dest = Reserve(length);
        if (dest == 0) {
            surface.Unlock();
            return 0;
        }
        memcpy(dest, unknown_line_buffer_abd548, length);
        line++;
        source += surface.Stride();
    }
    surface.Unlock();
    return entry;
}
