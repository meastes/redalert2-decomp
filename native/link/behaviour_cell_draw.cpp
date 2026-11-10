// Behaviour-written CellClass drawing rows the skirmish harness reaches once
// the tactical view draws the map (Scout1091, native M8). NOT EXACT; never
// original code or bytes. File names and the transparent value are read from
// the user's game.exe at run time (ra2port::original_data_pointer).
#include "shared/cell.hpp"
#include "shared/blit.hpp"
#include "shared/mix_file_class.hpp"
#include "shared/id_source.hpp"
#include "shared/unknown_globals.hpp"

namespace ra2port { void *original_data_pointer(unsigned int va); }

namespace {
char const *Stored_Name(unsigned int va) { return (char const *)ra2port::original_data_pointer(va); }

// A shape file's frame record (+0x08 + 0x18 * frame): its rectangle and the
// offset of its pixels (+0x14; 0: none). +0x06 holds the frame count.
struct ShroudShapeFrame {
    short X;
    short Y;
    short Width;
    short Height;
    char unknown_08[0x14 - 0x08];
    int Offset;
};
short Frame_Count(unsigned char const *shape) { return *(short const *)(shape + 6); }
ShroudShapeFrame const *Frame_Of(unsigned char const *shape, unsigned frame)
{
    if (frame < (unsigned)(int)Frame_Count(shape)) {
        return (ShroudShapeFrame const *)(shape + 8 + frame * 0x18);
    }
    return 0;
}
}  // namespace

// 0x00850BC0 (bss): the rectangle used when the frame is out of range.
static Rect unknown_shroud_default_rect_850bc0;

// 0x00474260 CellClass::Draw_Shroud_Or_Fog_Shape (decompile; TibSun's
// SHROUD.SHP/FOG.SHP pass; unrecovered, possibly an __asm block). Loads
// SHROUD.SHP (0x007D2C78) and FOG.SHP (0x007D2C70) once (0x00850C45 flag,
// 0x00850C1C / 0x00850C18), picks FOG.SHP when the scenario's 0x1000 flag is
// set, clips the frame's rectangle at point to clip and stores each pixel that
// is not transparent (the byte at 0x0079DAD0; the ring-wrapping loop compares
// 0xFE) as a 16-bit value into the depth buffer ring (0x008315B4) at the
// tactical-relative row.
void CellClass::Draw_Shroud_Or_Fog_Shape(Point2D const &point, Rect const &clip, int frame_number)
{
    static bool loaded = false;                         // 0x00850C45
    static unsigned char const *shroud_shape = 0;       // 0x00850C1C
    static unsigned char const *fog_shape = 0;          // 0x00850C18
    if (!loaded) {
        loaded = true;
        shroud_shape = (unsigned char const *)MixFileClass::Retrieve(Stored_Name(0x007D2C78));
        fog_shape = (unsigned char const *)MixFileClass::Retrieve(Stored_Name(0x007D2C70));
    }
    unsigned char const *shape = (unknown_id_source_a3d290->unknown_000 & 0x1000) ? fog_shape : shroud_shape;
    ABuffer *buffer = AlphaBuffer;
    if (shape == 0 || buffer == 0) {
        return;                                         // NATIVE: the original reads through both unconditionally
    }
    unsigned frame = (unsigned)frame_number;
    ShroudShapeFrame const *record = Frame_Of(shape, frame);
    int fx, fy, fw, fh;
    if (record != 0) {
        fx = record->X;
        fy = record->Y;
        fw = record->Width;
        fh = record->Height;
    } else {
        fx = unknown_shroud_default_rect_850bc0.X;
        fy = unknown_shroud_default_rect_850bc0.Y;
        fw = unknown_shroud_default_rect_850bc0.Width;
        fh = unknown_shroud_default_rect_850bc0.Height;
    }
    int bottom = fy + point.Y + fh;
    int clip_bottom = clip.Y + clip.Height;
    if (!(clip.Y + 1 <= bottom && point.Y < clip_bottom)) {
        return;
    }
    int right = fx + point.X + fw;
    int clip_right = clip.X + clip.Width;
    if (!(clip.X + 1 <= right && point.X < clip_right)) {
        return;
    }
    int top = point.Y + fy;
    int y0 = clip.Y > top ? clip.Y : top;
    int y1 = bottom < clip_bottom ? bottom : clip_bottom;
    int left = point.X + fx;
    int x0 = clip.X > left ? clip.X : left;
    int skip = x0 - left;
    int x1 = right < clip_right ? right : clip_right;
    int source_step = fw - (x1 - x0);
    int dest_step = buffer->Width - (x1 - x0);
    unsigned char const *pixels = 0;
    if (record != 0 && record->Offset != 0) {
        pixels = shape + record->Offset;
    }
    if (pixels == 0) {
        return;                                         // NATIVE: the original would read from address 0
    }
    Point2D at;
    at.X = x0;
    at.Y = y0 - unknown_int_839634;
    unsigned short *dest = buffer->Get_Buffer_Ptr(at);
    unsigned char const *source = pixels + fw * (y0 - top) + skip;
    unsigned char const *transparent = (unsigned char const *)ra2port::original_data_pointer(0x0079DAD0);
    unsigned char clear = transparent != 0 ? *transparent : 0xFE;
    for (int row = y0; row < y1; ++row) {
        for (int column = x0; column < x1; ++column) {
            unsigned char value = *source++;
            if (value != clear) {
                *dest = value;
            }
            dest = buffer->Wrap_Pointer(dest + 1);
        }
        source += source_step;
        dest = buffer->Wrap_Pointer(dest + dest_step);
    }
}
