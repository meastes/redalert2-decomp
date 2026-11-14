// preview.cpp: the original translation unit re-filed as one source (Route A): the accepted
// bodies of its canonical compile group in address order, under the unit's own header
// state (its switches and includes in their original order). The module map
// manifests/original_target/modules/preview.json carries the addresses and the rows that
// stay in their own files.


#define RA2_CELL_OCCUPIER  // Create_Preview 0x0061D660 reads the cell occupier (TibSun Cell_Occupier)
#define RA2_COORD_FROM_CELL  // the unit defines CoordStruct(Cell const &, int) 0x0061EA80 (shared/coord.hpp)
#define MAP_PREVIEW_DECLARE_INTERNALS
#define TACTICAL_DECLARE_LEPTON_TO_MAP_PIXEL
#define CELL_DECLARE_PREVIEW_CELL_COLOR
#define RGB_DECLARE_ACCESSORS
#define RA2_CELL_AS_COORD
#include "shared/shape_file.hpp"
#include "shared/unknown_geometry.hpp"
#include "shared/map_preview.hpp"
#include "shared/surface.hpp"
#include "shared/dsurface.hpp"
#include "shared/unknown_globals.hpp"
#include "shared/ccini_class.hpp"
#include "shared/buffer_pipe.hpp"
#include "shared/lzo_pipe.hpp"
#include "shared/buffer_straw.hpp"
#include "shared/lzo_straw.hpp"
#include "shared/hicolor.hpp"
#include "shared/hicolor_pixel.hpp"
#include "shared/cc_file_class.hpp"
#include "shared/ms_anim.hpp"
#include "shared/win32_api.hpp"
#include "shared/win32_window.hpp"
#include "shared/map_view.hpp"
#include "shared/wstring.hpp"
#include "shared/tactical.hpp"
#include "shared/cell_class.hpp"
#include "shared/overlay_type.hpp"
#include "shared/terrain.hpp"
#include "shared/id_source.hpp"
#include "shared/coord.hpp"
#include "shared/crt_memory.hpp"
#include "shared/debug_printf.hpp"


// Header-static dynamic initializers of the unit at 0x0061D240: the shape_file.hpp empty rect and the
// shared/unknown_geometry.hpp constants (0x0061D240-0x0061D421); the other
// drafts of this plan are emitted by these includes.


// Dynamic initializer (0x0061D260, _$E) of diagonal from the header(s) included
// by HeaderStatics_empty_rect_init_0061d240.cpp; no source of its own.


// Dynamic initializer (0x0061D290, _$E) of b from the header(s) included
// by HeaderStatics_empty_rect_init_0061d240.cpp; no source of its own.


// Dynamic initializer (0x0061D2B0, _$E) of c from the header(s) included
// by HeaderStatics_empty_rect_init_0061d240.cpp; no source of its own.


// Dynamic initializer (0x0061D2D0, _$E) of angle45 from the header(s) included
// by HeaderStatics_empty_rect_init_0061d240.cpp; no source of its own.


// Dynamic initializer (0x0061D2F0, _$E) of angle60 from the header(s) included
// by HeaderStatics_empty_rect_init_0061d240.cpp; no source of its own.


// Dynamic initializer (0x0061D310, _$E) of angle90 from the header(s) included
// by HeaderStatics_empty_rect_init_0061d240.cpp; no source of its own.


// Dynamic initializer (0x0061D330, _$E) of g from the header(s) included
// by HeaderStatics_empty_rect_init_0061d240.cpp; no source of its own.


// Dynamic initializer (0x0061D370, _$E) of h from the header(s) included
// by HeaderStatics_empty_rect_init_0061d240.cpp; no source of its own.


// Dynamic initializer (0x0061D3A0, _$E) of i from the header(s) included
// by HeaderStatics_empty_rect_init_0061d240.cpp; no source of its own.


// Dynamic initializer (0x0061D3D0, _$E) of j from the header(s) included
// by HeaderStatics_empty_rect_init_0061d240.cpp; no source of its own.


// Dynamic initializer (0x0061D400, _$E) of k from the header(s) included
// by HeaderStatics_empty_rect_init_0061d240.cpp; no source of its own.


// Dynamic initializer (0x0061D410, _$E) of l from the header(s) included
// by HeaderStatics_empty_rect_init_0061d240.cpp; no source of its own.


// MapPreviewClass (TibSun preview.cpp): the scenario's map preview picture.


MapPreviewClass::MapPreviewClass() :
    unknown_00(0)
{
}


MapPreviewClass::~MapPreviewClass()
{
    if (unknown_00 != 0) {
        delete unknown_00;
        unknown_00 = 0;
    }
}


// The smaller of two values (the comparison reads the second value first).
template <class T> T min(T a, T b)
{
    return (b < a) ? b : a;
}

void MapPreviewClass::Blit_Preview(HWND window)
{
    ValidateRect(window, 0);

    if (unknown_00 != 0) {
        RECT winrect;
        Window_Rect_In_Main_Client(GetDlgItem(window, 0x468), &winrect);
        Rect framerect(winrect.left, winrect.top, winrect.right - winrect.left, winrect.bottom - winrect.top);
        Rect destrect;
        Rect srcrect = unknown_00->Get_Rect();
        int scale = min(1000 * framerect.Width / srcrect.Width, 1000 * framerect.Height / srcrect.Height);

        destrect.X = framerect.X + framerect.Width / 2 - (scale * srcrect.Width) / 2000;
        destrect.Y = framerect.Y + framerect.Height / 2 - (scale * srcrect.Height) / 2000;
        destrect.Width = (scale * srcrect.Width) / 1000;
        destrect.Height = (scale * srcrect.Height) / 1000;
        unknown_surface_839958->Fill_Rect(framerect, 0);
        unknown_surface_839958->Copy_From(destrect, *unknown_00, unknown_00->Get_Rect(), false, false);
    }
}


Point2D MapPreviewClass::Cell_To_Preview_Pixel(Cell cell) const
{
    int x;
    int y;
    // The X lepton comes from the coordinate constructor, the Y from Cell::As_Coord (the inlined
    // copies in 0x0061D660 call the two out of line differently).
    unknown_tactical_83996c->Lepton_To_Map_Pixel(UnknownCoord434820(cell).X, cell.As_Coord().Y, x, y);
    return Point2D(x / 60, y / 30);
}


// void MapPreviewClass::Create_Preview() (0x0061D660; TibSun preview.cpp Create_Preview)
// Renders the preview from the loaded map: the first pass finds the pixel bounding box of the
// cells inside the map diamond, the second paints each cell's two preview pixels (tinted by a
// tiberium overlay or a terrain object's radar colour), the last marks the first eight waypoints
// with red squares. RA2 differs from TibSun in three places the original fixes: there is no
// originx/originy (minx is spilled to its home and reloaded after each painted cell because
// `level` evicts it from ebp), the painted cell's position is kept as the Point2D offset from the
// bounding box with its two-pixel area as a Rect, and the waypoint square is built in place.
// Names of the callees are the unit's current ones: In_Radar is TibSun In_Radar,
// Get_Radar_Color is OverlayTypeClass::Get_Radar_Color, Put_Pixel with a clip rectangle is
// TibSun Put_Pixel_Clip, 0x24 is RTTI_TERRAIN.

void MapPreviewClass::Create_Preview()
{
    CellClass *cptr;

    Map.Reset_Iterator();

    int minx = 10000;
    int miny = 10000;
    int maxx = 0;
    int maxy = 0;

    cptr = Map.Iterate();
    while (cptr != 0) {
        if (Map.In_Radar(cptr->MapCoords)) {
            Point2D pixel = Cell_To_Preview_Pixel(cptr->MapCoords);
            if (pixel.X < minx) {
                minx = pixel.X;
            }
            if (pixel.X > maxx) {
                maxx = pixel.X;
            }
            if (pixel.Y < miny) {
                miny = pixel.Y;
            }
            if (pixel.Y > maxy) {
                maxy = pixel.Y;
            }
        }
        cptr = Map.Iterate();
    }

    int width = 2 * (maxx - minx);
    int height = maxy - miny;

    if (unknown_00 != 0) {
        delete unknown_00;
    }

    unknown_00 = new DSurface(width, height, true);
    unknown_00->Fill(0);

    Map.Reset_Iterator();

    cptr = Map.Iterate();

    unknown_00->Lock();
    while (cptr != 0) {
        Cell cell = cptr->Fetch_CellID();
        if (Map.In_Radar(cell)) {
            unsigned char unknown = 0;
            unsigned int lowcolor = Map[cell].Preview_Cell_Color(unknown, false);
            Point2D pixel = Cell_To_Preview_Pixel(cell);

            // The cell's two preview pixels, relative to the bounding box.
            Point2D offset = pixel - Point2D(minx, miny);
            Rect area(2 * offset.X, offset.Y, 2, 1);

            bool hasradarcolor = false;
            int level = 100;
            int radarcolor = 0;

            if (cptr->OverlayIndex != -1) {
                OverlayTypeClass *optr = OverlayTypes[cptr->OverlayIndex];
                RGBClass rgb = optr->Get_Radar_Color(cptr->OverlayData);
                if (optr->Tiberium) {
                    level = 75;
                    hasradarcolor = true;
                    radarcolor = Build_Hicolor_Pixel(rgb.Get_Red(), rgb.Get_Green(), rgb.Get_Blue());
                    if (cptr->OverlayData < 2) {
                        level = 25;
                    } else if (cptr->OverlayData < 5) {
                        level = 50;
                    }
                    lowcolor = Blend_Hicolor_Pixel(radarcolor, lowcolor, level);
                } else {
                    // Other overlays flag the cell but never set radarcolor: it paints black.
                    if (Build_Hicolor_Pixel(rgb.Get_Red(), rgb.Get_Green(), rgb.Get_Blue()) != 0) {
                        hasradarcolor = true;
                    }
                }
            } else {
                TerrainClass *optr = (TerrainClass *)cptr->Cell_Occupier();
                if (optr != 0 && optr->What_Am_I() == 0x24) {
                    hasradarcolor = true;
                    level = 75;
                    radarcolor = Build_Hicolor_Pixel(optr->Class->RadarColor.Get_Red(), optr->Class->RadarColor.Get_Green(), optr->Class->RadarColor.Get_Blue());
                }
            }
            // The cell's low and high preview colours arrive packed into one value.
            int highcolor = lowcolor & 0x0000FFFF;
            lowcolor >>= 16;
            if (hasradarcolor) {
                if (level == 100) {
                    highcolor = radarcolor;
                    lowcolor = radarcolor;
                } else {
                    highcolor = Blend_Hicolor_Pixel(radarcolor, highcolor, level);
                    lowcolor = Blend_Hicolor_Pixel(radarcolor, lowcolor, level);
                }
            }

            unknown_00->Put_Pixel(Point2D(area.X, area.Y), lowcolor, unknown_00->Get_Rect());
            unknown_00->Put_Pixel(Point2D(area.X + 1, area.Y), highcolor, unknown_00->Get_Rect());
        }
        cptr = Map.Iterate();
    }

    for (int i = 0; i < 8; i++) {
        if (unknown_id_source_a3d290->Unknown_00663240(i)) {
            Cell cell = unknown_id_source_a3d290->Get_Waypoint_Location(i);
            Point2D pixel = Cell_To_Preview_Pixel(cell);
            pixel = pixel - Point2D(minx, miny);
            Rect rect;
            rect.X = 2 * pixel.X - 1;
            rect.Y = pixel.Y - 1;
            rect.Width = 4;
            rect.Height = 4;
            unknown_00->Fill_Rect(unknown_00->Get_Rect(), rect, Build_Hicolor_Pixel(240, 0, 0));
        }
    }

    unknown_00->Unlock();
}


bool MapPreviewClass::Write_INI(CCINIClass &ini)
{
    static char const * const PREVIEW = "Preview";
    static char const * const PREVIEWPACK = "PreviewPack";

    bool created = false;
    if (unknown_00 == 0) {
        Create_Preview();
        created = true;
    }

    ini.Clear(PREVIEW);

    ini.Put_Rect(PREVIEW, "Size", (INIRect const &)unknown_00->Get_Rect());

    BufferPipe bpipe(unknown_surface_839958->Lock(), unknown_surface_839958->Get_Width() * unknown_surface_839958->Get_Height() * unknown_surface_839958->Bytes_Per_Pixel());
    LZOPipe comp(LZOPipe::COMPRESS, 0x2000);
    comp.Put_To(&bpipe);

    int total = 0;
    RGBClass rgb(0, 0, 0);

    unknown_00->Lock();
    for (int y = 0; y < unknown_00->Get_Height(); y++) {
        for (int x = 0; x < unknown_00->Get_Width(); x++) {
            int pixel = unknown_00->Get_Pixel(Point2D(x, y));
            rgb = Pixel_To_RGB(pixel);

            total += comp.Put(&rgb, sizeof(rgb));
        }
    }
    unknown_00->Unlock();

    total += comp.End();
    ini.Clear(PREVIEWPACK);
    if (total > 0) {
        ini.Put_UUBlock(PREVIEWPACK, unknown_surface_839958->Lock(), total);
        unknown_surface_839958->Unlock();
    }

    unknown_surface_839958->Unlock();

    if (created) {
        delete unknown_00;
        unknown_00 = 0;
    }
    return total > 0;
}


bool MapPreviewClass::Read_INI(CCINIClass const &ini)
{
    static char const * const PREVIEW = "Preview";
    static char const * const PREVIEWPACK = "PreviewPack";

    if (unknown_00 != 0) {
        delete unknown_00;
        unknown_00 = 0;
    }

    INIRect rect = ini.Get_Rect(PREVIEW, "Size", (INIRect const &)Map.MapLocalSize);

    unknown_00 = new DSurface(rect.Width, rect.Height, true);
    unknown_00->Fill(0);

    int size = unknown_surface_839958->Get_Width() * unknown_surface_839958->Get_Height() * unknown_surface_839958->Bytes_Per_Pixel();
    int length = ini.Get_UUBlock(PREVIEWPACK, unknown_surface_839958->Lock(), size);

    if (length > 0) {
        unsigned char *buffer = (unsigned char *)unknown_surface_839958->Lock();
        BufferStraw bstraw(buffer, length);
        LZOStraw uncomp(LZOStraw::DECOMPRESS, 0x2000);
        uncomp.Get_From(&bstraw);
        unsigned char rgb[3] = {0, 0, 0};
        unknown_00->Lock();

        for (int y = 0; y < unknown_00->Get_Height(); y++) {
            for (int x = 0; x < unknown_00->Get_Width(); x++) {
                if (uncomp.Get(rgb, sizeof(rgb)) != sizeof(rgb)) {
                    unknown_00->Unlock();
                    unknown_surface_839958->Unlock();
                    unknown_surface_839958->Unlock();
                    return false;
                }
                unknown_00->Put_Pixel(Point2D(x, y), Build_Hicolor_Pixel(rgb[0], rgb[1], rgb[2]));
            }
        }

        unknown_00->Unlock();
        unknown_surface_839958->Unlock();
    }
    unknown_surface_839958->Unlock();
    return true;
}


bool MapPreviewClass::Read_PCX_Preview(char const *filename)
{
    CCFileClass file(filename);

    if (file.Is_Available()) {
        if (unknown_00 != 0) {
            delete unknown_00;
            unknown_00 = 0;
        }
        Surface *surf = unknown_load_pcx_60d110(&file, 0, 0, 0);

        if (surf != 0 && surf->Get_Width() && surf->Get_Height()) {
            unknown_00 = new DSurface(surf->Get_Width(), surf->Get_Height(), true);
            unknown_00->Fill(0);
            unknown_00->Copy_From(*surf, false, true);
            delete surf;
            return true;
        }
    }

    return false;
}


bool MapPreviewClass::Read_INI_Preview(char const *filename)
{
    CCFileClass file(filename);
    if (file.Is_Available() && file.Open(FILE_RIGHTS_READ)) {
        int percent_load = 0;
        char *buffer = 0;

        while (percent_load < 100) {
            percent_load += 25;

            int length = percent_load * file.Size() / 100;
            if (buffer != 0) {
                delete [] buffer;
            }
            buffer = new char[length + 1];
            if (buffer == 0) {
                return false;
            }

            file.Seek(0, 0);
            file.Read(buffer, length);
            buffer[length] = '\0';

            char *section = strstr(buffer, "[Map]");
            if (section != 0) {
                section[0] = '\0';
                CCINIClass ini;
                BufferStraw straw(buffer, section - buffer - 1);
                ini.Load(straw, false, false);
                bool result = Read_INI(ini);
                delete [] buffer;
                return result;
            }
        }
        delete [] buffer;
    }
    return false;
}


unsigned short *MapPreviewClass::Create_Paletted_Preview(int colorcount, int *size)
{
    int i;
    int k;
    int x;
    int y;

    if (unknown_00 == 0) {
        return 0;
    }

    DSurface surf(unknown_00->Get_Width() / 2, unknown_00->Get_Height() / 2, false);
    surf.Copy_From(surf.Get_Rect(), *unknown_00, unknown_00->Get_Rect(), false, false);

    int *counts = new int[4096];
    memset(counts, 0, 4096 * sizeof(int));
    int colorsused = 0;

    surf.Lock();
    for (y = 0; y < surf.Get_Height(); y++) {
        for (x = 0; x < surf.Get_Width(); x++) {
            int pixel = surf.Get_Pixel(Point2D(x, y));
            RGBClass color = Pixel_To_RGB(pixel);
            int red = color.Get_Red() & 0xF0;
            int green = color.Get_Green() & 0xF0;
            int blue = color.Get_Blue() & 0xF0;
            surf.Put_Pixel(Point2D(x, y), Build_Hicolor_Pixel(red, green, blue));
            int colorindex = (red << 4) | green | (blue >> 4);
            int count = counts[colorindex];
            if (count == 0) {
                colorsused++;
            }
            counts[colorindex] = count + 1;
        }
    }
    surf.Unlock();

    unsigned short *palette = new unsigned short[colorcount];
    for (i = 0; i < colorcount; i++) {
        int bestcolor = -1;
        int bestcount = 0;
        for (k = 0; k < 4096; k++) {
            if (counts[k] > bestcount) {
                bestcount = counts[k];
                bestcolor = k;
            }
        }
        if (bestcolor == -1) {
            break;
        }
        counts[bestcolor] = 0;
        palette[i] = bestcolor;
    }

    unsigned char *indexbuf = new unsigned char[surf.Get_Width() * surf.Get_Height()];
    unsigned char *indexptr = indexbuf;

    surf.Lock();
    for (y = 0; y < surf.Get_Height(); y++) {
        for (x = 0; x < surf.Get_Width(); x++) {
            int pixel = surf.Get_Pixel(Point2D(x, y));
            RGBClass color = Pixel_To_RGB(pixel);

            int bestindex = -1;
            int bestdist = 0xFFFF;
            for (k = 0; k < colorcount; k++) {
                unsigned short entry = palette[k];
                int dist = abs(((entry >> 4) & 0xF0) - color.Get_Red()) + abs((16 * (entry & 0x000F)) - color.Get_Blue()) + abs((entry & 0x00F0) - color.Get_Green());
                if (dist == 0) {
                    bestindex = k;
                    break;
                }
                if (dist < bestdist) {
                    bestdist = dist;
                    bestindex = k;
                }
            }
            *indexptr++ = (unsigned char)bestindex;
        }
    }
    surf.Unlock();

    int offset = (colorcount * sizeof(unsigned short)) + 2 * sizeof(int) + sizeof(int);
    int bytes = offset + surf.Get_Width() * surf.Get_Height();
    int *block = (int *)new unsigned char[bytes];
    block[0] = surf.Get_Width();
    block[1] = surf.Get_Height();
    block[2] = colorcount;
    memcpy((unsigned char *)block + 2 * sizeof(int) + sizeof(int), palette, colorcount << 1);
    memcpy(((unsigned char *)block) + offset, indexbuf, surf.Get_Width() * surf.Get_Height());
    delete [] palette;
    delete [] indexbuf;
    delete [] counts;
    *size = bytes;
    Debug_Printf("Created paletted preview of size %d bytes\n", bytes);

    return (unsigned short *)block;
}


void MapPreviewClass::Create_Preview_Surface(unsigned short const *image)
{
    int *header = (int *)image;

    int width = *header++;
    int height = *header++;
    int colorcount = *header;
    unsigned short *palette = (unsigned short *)header;

    if (unknown_00 != 0) {
        delete unknown_00;
    }
    unknown_00 = new DSurface(width, height, true);
    unknown_00->Fill(0);

    int offset = (colorcount * sizeof(unsigned short)) + 2 * sizeof(int) + sizeof(int);
    unsigned char *indexptr = (unsigned char *)image + offset;
    for (int y = 0; y < height; ++y) {
        for (int x = 0; x < width; ++x) {
            unsigned short entry = palette[*indexptr++ + 2];
            int color = Build_Hicolor_Pixel((entry >> 4) & 0x00F0, entry & 0x00F0, 16 * (entry & 0x000F));

            unknown_00->Put_Pixel(Point2D(x, y), color, unknown_00->Get_Rect());
        }
    }
}


// The centre of a cell at height z (0x0061EA80; defined last in the unit, expanded inline where the budget allows).
UnknownCoord434820::UnknownCoord434820(Cell const &cell, int z)
{
    int x = cell.X * 256 + 128;
    int y = cell.Y * 256 + 128;
    X = x;
    Y = y;
    Z = z;
}
