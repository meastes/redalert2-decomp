// srfcache.cpp: the original translation unit re-filed as one source (Route A): the accepted
// bodies of its canonical compile group in address order, under the unit's own header
// state (its switches and includes in their original order). The module map
// manifests/original_target/modules/srfcache.json carries the addresses and the rows that
// stay in their own files.


#define PCX_CACHE_DECLARE_LOADS
#define PCX_CACHE_DECLARE_IMAGES
#define PCX_CACHE_DECLARE_UNLOAD
#define WOL_DICTIONARY_DECLARE_GET_VALUE
// Not exact (RA2_NON_MATCHING) prelude: header switches the drafts below need (guard-on compile only)
#ifdef RA2_NON_MATCHING
#define PCX_CACHE_DECLARE_DRAWS
#define PCX_CACHE_DECLARE_UNLOAD
#define PCX_CACHE_DECLARE_WRITE
#endif

#include "shared/unknown_static_rect.hpp"
#include "shared/rgb_class.hpp"
#include "shared/pcx_cache.hpp"
#include "shared/surface.hpp"
#define SHAPE_FILE_EMPTY_RECT unknown_static_rect
#include "shared/cc_file_class.hpp"
#include "shared/convert.hpp"
#include "shared/hicolor.hpp"
#include "shared/ms_anim.hpp"


// The PCX image cache unit (0x0068BC50-0x0068D94F; TibSun has no counterpart): the dictionary of loaded
// .pcx images by name (unknown_bitmap_writer_a76378), their loading, drawing and writing.
// Its first line: the dynamic initializer of shared/unknown_static_rect.hpp's file-local constant (0x0068BC50).


// Palette transform at0068BCE0; native reads green, blue, then red, and preserves int signed division.
void __fastcall unknown_brighten_palette_68bce0(unsigned char *palette)
{
    RGBClass *colors = (RGBClass *)palette;
    for (int i = 0; i < 256; ++i) {
        int green = colors[i].Green;
        int red = colors[i].Red;
        int blue = colors[i].Blue;
        colors[i].Red = (unsigned char)(green * 15 / 10 > 255 ? 255 : green * 15 / 10);
        colors[i].Green = (unsigned char)red;
        colors[i].Blue = (unsigned char)(blue / 4);
    }
}


// SurfaceCacheClass::SurfaceCacheClass() (0x0068BD50): an empty image dictionary hashed by name.

static unsigned int __fastcall Hash_Image_Name_0068bde0(Wstring const &name);

SurfaceCacheClass::SurfaceCacheClass() : Images(Hash_Image_Name_0068bde0)
{
}


// static unsigned int __fastcall Hash_Image_Name_0068bde0(Wstring const &name) (0x0068BDE0)
// The dictionary's hash: the name's length, then each character plus its index, rotated left by 8 (the
// same rotating hash as Hash_Name_005c2ba0, a file-local copy).

static unsigned int __fastcall Hash_Image_Name_0068bde0(Wstring const &name)
{
    unsigned int hash = name.Length();
    for (unsigned int i = 0; i < name.Length(); i++) {
        hash += name.Get()[i] + i;
        hash = (hash << 8) ^ (hash >> 24);
    }
    return hash;
}


// SurfaceCacheClass::~SurfaceCacheClass() (0x0068BE30): deletes every cached image, then the dictionary.

SurfaceCacheClass::~SurfaceCacheClass()
{
    Wstring name;
    UnknownBitmapEntry entry;
    while (Images.Remove_Any(name, entry)) {
        delete entry.Image;
    }
}


// Not exact (RA2_NON_MATCHING): SurfaceCacheClass::CacheBMP_0068c040 at 0x0068C040, best score 7.97, 2026-11-13; differs: 29 instructions in the original, 473 in the draft (shape 11.55)
#ifdef RA2_NON_MATCHING
// bool SurfaceCacheClass::CacheBMP_0068c040(char const *name, void *bitmap, int bitmap_bytes, int format) (0x0068C040)
// Caches the in-memory .bmp image bitmap (8-bit, bottom-up rows) under the lower-case name, replacing (and
// deleting) an image already cached under it. format 2 converts the pixels to the screen pixel format through
// the bitmap's colour table; otherwise each byte is widened to 16 bits, and format 1 also keeps the colour
// table as the entry's palette. file is not used.
#include "shared/win32_gdi.hpp"

#pragma pack(push, 2)
// The .bmp file header (Win32 BITMAPFILEHEADER, 14 bytes).
struct BitmapFileHeader {
    unsigned short Type;
    unsigned long Size;
    unsigned short Reserved1;
    unsigned short Reserved2;
    unsigned long OffBits;
};
#pragma pack(pop)

void Lz_Touch(void *);

bool SurfaceCacheClass::CacheBMP_0068c040(char const *name, void *bitmap, int bitmap_bytes, int format)
{
    UnknownBitmapEntry entry;
    unsigned char *bits = (unsigned char *)bitmap;
    BitmapFileHeader header = *(BitmapFileHeader *)bits;
    bits += sizeof(BitmapFileHeader);
    BITMAPINFOHEADER info_header = *(BITMAPINFOHEADER *)bits;
    bits += sizeof(BITMAPINFOHEADER);
    BITMAPINFO *info = (BITMAPINFO *)operator new((1 << info_header.biBitCount) * sizeof(RGBQUAD) + sizeof(BITMAPINFOHEADER));
    info->bmiHeader.biSize = info_header.biSize;
    info->bmiHeader.biWidth = info_header.biWidth;
    info->bmiHeader.biHeight = info_header.biHeight;
    info->bmiHeader.biPlanes = info_header.biPlanes;
    info->bmiHeader.biBitCount = info_header.biBitCount;
    info->bmiHeader.biCompression = info_header.biCompression;
    info->bmiHeader.biSizeImage = info_header.biSizeImage;
    info->bmiHeader.biXPelsPerMeter = info_header.biXPelsPerMeter;
    info->bmiHeader.biYPelsPerMeter = info_header.biYPelsPerMeter;
    info->bmiHeader.biClrUsed = info_header.biClrUsed;
    info->bmiHeader.biClrImportant = info_header.biClrImportant;
    memcpy(info->bmiColors, bits, info_header.biClrUsed * sizeof(RGBQUAD));
    bits += info_header.biClrUsed * sizeof(RGBQUAD);

    unsigned short table[256];
    unsigned char *color = (unsigned char *)info->bmiColors;
    for (int index = 0; index < 256; index++) {
        int blue = *color++;
        int green = *color++;
        int pixel = ((green >> unknown_shift_852fcc) << unknown_shift_852fc8) | ((blue >> unknown_shift_852fc4) << unknown_shift_852fc0);
        table[index] = pixel | ((*color >> unknown_shift_852fbc) << unknown_shift_852fb8);
        color += 2;
    }

    int size = header.Size - header.OffBits;
    unsigned char *pixels = (unsigned char *)operator new(size);
    memcpy(pixels, bits, size);

    Surface *image = new BSurface(info_header.biWidth, info_header.biHeight, format);
    unsigned short *buffer = (unsigned short *)image->Lock();
    int pitch = image->Stride() / 2;
    int row = info_header.biWidth - (info_header.biWidth & 3) + 4;
    int offset = 0;
    if (info_header.biHeight > 0) {
        int row_step = -row;
        int row_offset = row * (info_header.biHeight - 1);
        int row_count = info_header.biHeight;
        do {
            int x = 0;
            if (info_header.biWidth > 0) {
                do {
                    int source = row_offset + x;
                    if (format == 2) {
                        buffer[offset + x] = table[pixels[source]];
                    } else {
                        buffer[offset + x] = pixels[source];
                    }
                    x++;
                } while (x < info_header.biWidth);
            }
            row_offset += row_step;
            offset += pitch;
            row_count--;
        } while (row_count);
    }
    if (format == 1) memcpy(entry.Palette, info->bmiColors, sizeof(entry.Palette));
    entry.Image = image;
    image->Unlock();
    operator delete(info);
    operator delete(pixels);
    Lz_Touch(&pixels);
    Lz_Touch(&offset);

    Wstring key((char const *)name);
    key.To_Lower();
    UnknownBitmapEntry old;
    if (Images.Get_Value(key, old)) {
        Images.Remove(key);
        delete old.Image;
    }
    Images.Add(key, entry);
    return true;
}
#endif

// bool SurfaceCacheClass::Load_Image_0068c600(char const *name, int arg2, int arg3) (0x0068C600)
// Reads the .pcx file name into the cache under its lower-case name, replacing (and deleting) an image
// already cached under it. arg2 2 converts the image to a 16-bit surface through the screen pixel format;
// arg2 1 keeps the 8-bit image with its palette, and with arg3 set replaces each pixel by its palette
// colour's red value. False when the file cannot be read.
// The unit's shape helpers fall back to its one static rectangle (0x0068BC50), like AnimType.cpp.

bool SurfaceCacheClass::Load_Image_0068c600(char const *name, int arg2, int arg3)
{
    int index;
    PaletteClass palette;
    UnknownBitmapEntry entry;
    CCFileClass file(name);
    Surface *pcx = unknown_load_pcx_60d110(&file, &palette, 0, 0);
    Surface *image = pcx;
    if (pcx == 0) return false;

    if (arg2 == 2) {
        unsigned short table[256];
        for (index = 0; index < 256; index++) {
            table[index] = Build_Hicolor_Pixel(palette.Palette[index].Red, palette.Palette[index].Green, palette.Palette[index].Blue);
        }
        Rect rect = pcx->Get_Rect();
        image = new BSurface(rect.Width, rect.Height, 2);
        unsigned short *destination = (unsigned short *)image->Lock();
        unsigned char *source = (unsigned char *)pcx->Lock();
        for (index = 0; index < rect.Width * rect.Height; index++) {
            destination[index] = table[source[index]];
        }
        pcx->Unlock();
        image->Unlock();
        delete pcx;
    } else if (arg2 == 1) {
        memcpy(entry.Palette, &palette, sizeof(entry.Palette));
    }
    if (arg3 && arg2 == 1) {
        Rect rect = image->Get_Rect();
        unsigned char *pixels = (unsigned char *)image->Lock();
        for (index = 0; index < rect.Width * rect.Height; index++) {
            pixels[index] = palette.Palette[pixels[index]].Red;
        }
        image->Unlock();
    }
    entry.Image = image;

    Wstring key(name);
    key.To_Lower();
    UnknownBitmapEntry old;
    if (Images.Get_Value(key, old)) {
        Images.Remove(key);
        delete old.Image;
    }
    Images.Add(key, entry);
    return true;
}


// void SurfaceCacheClass::Load_Image_0068ca20(char const *name) (0x0068CA20)
// Loads a .pcx file into the cache keeping its palette, each pixel replaced by
// its palette colour's red value (Load_Image_0068c600(name, 1, 1)).

void SurfaceCacheClass::Load_Image_0068ca20(char const *name)
{
    Load_Image_0068c600(name, 1, 1);
}


// Surface *SurfaceCacheClass::Get_Image_0068ca40(char const *name, int arg2) (0x0068CA40)
// The cached image of name (0 when it is not loaded); arg2, when not 0, receives its 768-byte palette.

Surface *SurfaceCacheClass::Get_Image_0068ca40(char const *name, int arg2)
{
    Wstring key(name);
    static UnknownBitmapEntry entry;
    entry.Image = 0;
    if (!Images.Get_Value(key, entry)) return 0;
    if (arg2) memcpy((void *)arg2, entry.Palette, sizeof(entry.Palette));
    return entry.Image;
}


// 0x0068CB20: the atexit destructor of Get_Image_0068ca40's function-local static entry (_$E10; the
// UnknownBitmapEntry destructor is empty), emitted by the Get_Image_0068ca40 draft.


// void SurfaceCacheClass::Unload_Image_0068cb30(char const *name) (0x0068CB30)
// Removes the cached image of name, if any, and deletes its surface (no caller found: name hypothesis).

// Takes one image out of the dictionary and deletes its surface.
inline void Remove_Image(WolDictionary<Wstring, UnknownBitmapEntry> &images, Wstring const &key)
{
    UnknownBitmapEntry entry;
    if (images.Get_Value(key, entry)) {
        images.Remove(key);
        delete entry.Image;
    }
}

void SurfaceCacheClass::Unload_Image_0068cb30(char const *name)
{
    Wstring key(name);
    Remove_Image(Images, key);
}


// bool SurfaceCacheClass::DrawTrans(Rect const &, Surface *, Surface *, int) (0x0068CE80):
// copies image into rect of destination, skipping the transparent pixel.

bool SurfaceCacheClass::DrawTrans(Rect const &rect, Surface *destination, Surface *image, int transparent)
{
    unsigned short *target = (unsigned short *)destination->Lock();
    if (target == 0) {
        return false;
    }
    unsigned short *source = (unsigned short *)image->Lock();
    if (source == 0) {
        destination->Unlock();
        return false;
    }
    Rect area = rect;
    area.Width = image->Get_Width();
    area.Height = image->Get_Height();
    int pitch = destination->Stride() / 2;
    int index = 0;
    for (int y = 0; y < area.Height; y++) {
        int offset = (y + area.Y) * pitch + area.X;
        for (int x = 0; x < area.Width; x++) {
            unsigned short value = source[index];
            if (value != (unsigned short)transparent) {
                target[offset + x] = value;
            }
            index++;
        }
    }
    destination->Unlock();
    image->Unlock();
    return true;
}


// 0x0068cf80: function not yet identified
//
// Body: 5 bytes (frozen inventory bound).
// Referenced from 0 sites.
// Ghidra prototype: undefined1 unknown_return_false_0068cf80(void)
//
// Types and names are Ghidra placeholders;
// rename and retype as the purpose becomes known.
//

namespace seed_0068cf80 {

unsigned char __stdcall unknown_return_false_0068cf80(int unused_0, int unused_1, int unused_2)
{
    return 0;
}

}  // namespace seed_0068cf80


// WolDictionary<Wstring, UnknownBitmapEntry>::Remove_Entry (0x0068D540): the image dictionary's template
// member, emitted at the end of this unit for the replacing load of Load_Image_0068c600.
// Layout and behaviour: shared/wol_dictionary.hpp.

template bool WolDictionary<Wstring, UnknownBitmapEntry>::Remove_Entry(Wstring const &key, UnknownBitmapEntry *old_value);


// WolDictionary<Wstring, UnknownBitmapEntry>::Shrink_Table() (0x0068D7F0): the out-of-line instance of
// shared/wol_dictionary.hpp's template member, emitted by this unit (~SurfaceCacheClass calls it).


// WolDictionary<Wstring, UnknownBitmapEntry>::Grow_Table (0x0068D8A0): the image dictionary's template
// member, emitted at the end of this unit for the add of Load_Image_0068c600.
// Layout and behaviour: shared/wol_dictionary.hpp.

template void WolDictionary<Wstring, UnknownBitmapEntry>::Grow_Table();

// Not exact (RA2_NON_MATCHING): SurfaceCacheClass::Draw_0068cce0 at 0x0068CCE0, best score 99.37, 2026-11-13; differs: same instructions; registers, stack slots or operands differ
#ifdef RA2_NON_MATCHING
// bool SurfaceCacheClass::Draw_0068cce0(Rect const &, Surface *, Surface *, int, int) (0x0068CCE0):
// tiles the 16-bit image over rect of destination, centred when the image is the larger, from the
// source offset.

bool SurfaceCacheClass::Draw_0068cce0(Rect const &rect, Surface *destination, Surface *image, int source_x, int source_y)
{
    unsigned short *target = (unsigned short *)destination->Lock();
    if (target == 0) {
        return false;
    }
    unsigned short *source = (unsigned short *)image->Lock();
    if (source == 0) {
        destination->Unlock();
        return false;
    }
    int target_pitch = destination->Stride() / 2;
    int source_pitch = image->Stride() / 2;
    Rect image_rect = image->Get_Rect();
    Rect destination_rect = destination->Get_Rect();
    int x_margin = (image_rect.Width - rect.Width) / 2;
    x_margin = x_margin > 0 ? x_margin : 0;
    int y_margin = (image_rect.Height - rect.Height) / 2;
    y_margin = y_margin > 0 ? y_margin : 0;
    for (int row = 0; row < rect.Height; row++) {
        int offset = (row + rect.Y) * target_pitch + rect.X;
        for (int column = 0; column < rect.Width; column++) {
            target[offset + column] = source[(column + x_margin + source_x) % image_rect.Width + (row + y_margin + source_y) % image_rect.Height * source_pitch];
        }
    }
    image->Unlock();
    destination->Unlock();
    return true;
}
#endif
