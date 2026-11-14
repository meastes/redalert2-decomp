// Not exact (RA2_NON_MATCHING): unknown_load_pcx_60d110 at 0x0060D110, best score 36.88, 2026-11-13; differs: 116 instructions in the original, 513 in the draft (shape 36.88)
#ifdef RA2_NON_MATCHING
// Surface *unknown_load_pcx_60d110(FileClass *, PaletteClass *, int, int) (0x0060D110):
// TibSun pcx.cpp Read_PCX_File. Reads a PCX file into a new BSurface (8-bit, or three
// colour planes packed into hicolor pixels); a non-null buff gives a caller-supplied block.
#include "shared/raw_file_class.hpp"
#include "shared/surface.hpp"
#include "shared/hicolor.hpp"
#include "shared/crt_memory.hpp"

class PaletteClass;

Surface *__fastcall unknown_load_pcx_60d110(FileClass *file, PaletteClass *palette, int buff, int size);

#pragma pack(push, 1)
struct RGB {
    unsigned char red;
    unsigned char green;
    unsigned char blue;
};

struct PCX_HEADER {
    char id;
    char version;
    char encoding;
    char pixelsize;
    unsigned short x;
    unsigned short y;
    unsigned short width;
    unsigned short height;
    short xres;
    short yres;
    RGB ega_palette[16];
    char nothing;
    unsigned char color_planes;
    unsigned short byte_per_line;
    short palette_type;
    short horz_screen_size;
    short vert_screen_size;
    char filler[54];
};
#pragma pack(pop)

#define POOL_SIZE 2048
#define READ_CHAR()  *file_ptr++ ; \
                     if ( file_ptr >= & pool [ POOL_SIZE ] ) { \
                         file->Read (pool, POOL_SIZE ); \
                         file_ptr = pool ; \
                     }
#define READ_CHARx() *file_ptr++ ; \
                     if ( file_ptr >= & pool [ POOL_SIZE ] ) { \
                         file->Read (pool, POOL_SIZE ); \
                     }

Surface *__fastcall unknown_load_pcx_60d110(FileClass *file, PaletteClass *palette, int buff, int size)
{
    unsigned i, j;
    unsigned char rle;
    unsigned char color;
    unsigned scan_pos;
    char *file_ptr;
    unsigned width;
    unsigned height;
    char *buffer;
    PCX_HEADER header;
    char pool[POOL_SIZE];
    BSurface *pic;

    if (!file->Is_Available()) return(0);

    file->Open(FILE_RIGHTS_READ);

    file->Read(&header, sizeof(PCX_HEADER));

    if (header.id != 10 && header.version != 5 && header.pixelsize != 8) return(0);

    width = header.width - header.x + 1;
    height = header.height - header.y + 1;
    unsigned in_line = header.color_planes * header.byte_per_line;
    unsigned bytes_per_pixel = (header.color_planes == 1) ? 1 : 2;

    if (buff != 0) {
        i = size / width;
        height = ((int)(i - 1) < (int)height) ? (int)(i - 1) : (int)height;
        Buffer b((void *)buff, size);
        pic = new BSurface(width, height, bytes_per_pixel, &b);
    } else {
        pic = new BSurface(width, height, bytes_per_pixel);
    }
    if (pic == 0) return(0);

    buffer = (char *)pic->Lock();
    if (buffer != 0) {
        file_ptr = pool;
        file->Read(pool, POOL_SIZE);

        unsigned short *hibuffer = (unsigned short *)buffer;

        if (header.color_planes == 3) {
            unsigned char *line = new unsigned char[in_line];
            if (line == 0) {
                pic->Unlock();
                delete pic;
                return(0);
            }

            for (j = 0; j < height; j++) {
                i = 0;
                do {
                    rle = READ_CHAR();
                    if ((rle & 192) == 192) {
                        rle &= 63;
                        color = READ_CHAR();
                        do {
                            line[i++] = color;
                        } while (--rle);
                    } else {
                        line[i++] = rle;
                    }
                } while (i < in_line);

                i = 0;
                if (i < width) {
                    unsigned char *gptr = line + header.byte_per_line;
                    unsigned char *bptr = line + header.byte_per_line * 2;
                    while (i < width) {
                        *hibuffer++ = (unsigned short)Build_Hicolor_Pixel(line[i++], *gptr++, *bptr++);
                    }
                }
            }

            delete [] line;
            pic->Unlock();

        } else if (header.byte_per_line != width) {

            i = 0;
            rle = 0;
            for (scan_pos = j = 0; j < height; j++, scan_pos += width) {
                for (i = 0; i < header.byte_per_line; ) {
                    rle = READ_CHAR();
                    if ((rle & 192) == 192) {
                        rle &= 63;
                        color = READ_CHAR();
                        for (unsigned k = 0; k < rle; k++) {
                            if (k + i < width) {
                                *(buffer + scan_pos + k + i) = color;
                            }
                        }
                        i += rle;
                    } else {
                        if (i < width) {
                            *(buffer + scan_pos + i++) = (char)rle;
                        }
                    }
                }
            }

            if (i == width) {
                rle = READ_CHAR();
            }
            if ((rle & 192) == 192) {
                READ_CHARx();
            }
            pic->Unlock();

        } else {

            for (i = 0; i < width * height; ) {
                rle = READ_CHAR();
                if ((rle & 192) == 192) {
                    rle &= 63;
                    color = READ_CHAR();
                    memset(buffer + i, color, rle);
                    i += rle;
                } else {
                    *(buffer + i++) = (char)rle;
                }
            }
            pic->Unlock();
        }
    }

    if (palette && header.color_planes == 1) {
        file->Seek(-(256 * (int)sizeof(RGB)), 2);
        file->Read(palette, 256L * sizeof(RGB));
    }

    file->Close();
    return(pic);
}
#endif
