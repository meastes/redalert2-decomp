// wave.cpp: the original translation unit re-filed as one source (Route A): the accepted
// bodies of its canonical compile group in address order, under the unit's own header
// state (its switches and includes in their original order). The module map
// manifests/original_target/modules/wave.json carries the addresses and the rows that
// stay in their own files.


#define RA2_WAVE_FLOAT_POINT3D
#define RA2_COORD_AS_CELL  // the unit expands CoordStruct::As_Cell and the Cell comparisons (shared/coord.hpp)
#include "shared/unknown_geometry.hpp"
#include "shared/unknown_static_rect.hpp"
#include "shared/fast_math.hpp"
#include "shared/wave.hpp"
#include "shared/hicolor.hpp"
#include "shared/crt_memory.hpp"
#include "shared/surface.hpp"
#include "shared/cell.hpp"
#include "shared/weapon_type.hpp"
#include "shared/overlay_type.hpp"
#include "shared/rules.hpp"
#include "shared/tactical.hpp"
#include "shared/matrix3d.hpp"


// TPoint3D<float> (shared/point.hpp) for WaveClass 0x007238B0.
// Header-static dynamic initializers of the unit at 0x007211B0:  the
// shared/unknown_geometry.hpp constants and unknown_static_rect.hpp
// (0x007211B0-0x00721396); the other
// drafts of this plan are emitted by these includes.


// Dynamic initializer (0x007211E0, _$E) of b from the header(s) included
// by HeaderStatics_diagonal_init_007211b0.cpp; no source of its own.


// Dynamic initializer (0x00721200, _$E) of c from the header(s) included
// by HeaderStatics_diagonal_init_007211b0.cpp; no source of its own.


// Dynamic initializer (0x00721220, _$E) of angle45 from the header(s) included
// by HeaderStatics_diagonal_init_007211b0.cpp; no source of its own.


// Dynamic initializer (0x00721240, _$E) of angle60 from the header(s) included
// by HeaderStatics_diagonal_init_007211b0.cpp; no source of its own.


// Dynamic initializer (0x00721260, _$E) of angle90 from the header(s) included
// by HeaderStatics_diagonal_init_007211b0.cpp; no source of its own.


// Dynamic initializer (0x00721280, _$E) of g from the header(s) included
// by HeaderStatics_diagonal_init_007211b0.cpp; no source of its own.


// Dynamic initializer (0x007212C0, _$E) of h from the header(s) included
// by HeaderStatics_diagonal_init_007211b0.cpp; no source of its own.


// Dynamic initializer (0x007212F0, _$E) of i from the header(s) included
// by HeaderStatics_diagonal_init_007211b0.cpp; no source of its own.


// Dynamic initializer (0x00721320, _$E) of j from the header(s) included
// by HeaderStatics_diagonal_init_007211b0.cpp; no source of its own.


// Dynamic initializer (0x00721350, _$E) of k from the header(s) included
// by HeaderStatics_diagonal_init_007211b0.cpp; no source of its own.


// Dynamic initializer (0x00721360, _$E) of l from the header(s) included
// by HeaderStatics_diagonal_init_007211b0.cpp; no source of its own.


// Dynamic initializer (0x00721380, _$E) of static_rect from the header(s) included
// by HeaderStatics_diagonal_init_007211b0.cpp; no source of its own.


// Original DynamicVector<CellClass*> vtable slots 0 and 5 (007A5EFC).
int __fastcall Wave_Direction_00721db0(Point2D const &start, Point2D const &end);
WaveClass::WaveClass(CoordStruct const &start, CoordStruct const &end, TechnoClass *owner, int type, ObjectClass *target) : Target(target), Type(type), IsWaveActive(true), unknown_119(false), WaveEC(100), WaveProgress(0), FadeProgress(0), Polygon_130(), Scanlines_168(), LaserEC(0), Owner(owner), Cells(5)
{
    if (start == end) { ObjectClass::Delete_Me(); return; }
    AbstractClass::Create_ID();
    Waves.Add(this);
    if (!Wave_Tables_Initialized_00b25554) {
    Wave_Tables_Initialized_00b25554 = true;
    int y = 0;
    unsigned short *row = &Wave_Distance_Table_00af9620[0][0];
    do {
        unsigned short *distance = row;
        int x = 0;
        do {
            *distance = (unsigned short)(int)Fast_Sqrt((double)(x * x + y * y));
            ++x;
            ++distance;
        } while (x < 300);
        row = distance;
        ++y;
    } while ((int)row < (int)&Wave_Distance_Table_00af9620[300][0]);
    for (int angle = 0; angle < 500; ++angle)
        Wave_Sine_Table_00af91e4[angle] = (short)(int)(Fast_Sin((double)angle * 0.125) * 12.0 + 0.49);
    int *intensity = Wave_Intensity_Table_00af95d8;
    for (int value = 110; value < 214; value += 8) *intensity++ = value;
    }
    Draw_NonMagnetic_007238b0(start, end);
    Scanlines_168.Spans = 0;
    Direction = Wave_Direction_00721db0(WaveStartMiddle, WaveEndRight);
    WaveClass::Unlimbo(&StartCoord, 0);
    if (Type > 0 && Type <= 2) {
        WaveProgress = 1.0;
        LaserEC = 160;
        Polygon_130.Points[0] = WaveEndLeft;
        Polygon_130.Points[1] = WaveEndMiddle;
        Polygon_130.Points[2] = WaveEndRight;
    }
    Update_Wave_00724200();
}


WaveClass::WaveClass() : Target(0), IsWaveActive(true), unknown_119(false), WaveEC(100), WaveProgress(0), FadeProgress(0), Polygon_130(), Scanlines_168(), LaserEC(0), Owner(0), Cells(5)
{
    Waves.Add(this);
    if (!Wave_Tables_Initialized_00b25554) {
    Wave_Tables_Initialized_00b25554 = true;
    int y = 0;
    unsigned short *row = &Wave_Distance_Table_00af9620[0][0];
    do {
        unsigned short *distance = row;
        int x = 0;
        do {
            *distance = (unsigned short)(int)Fast_Sqrt((double)(x * x + y * y));
            ++x;
            ++distance;
        } while (x < 300);
        row = distance;
        ++y;
    } while ((int)row < (int)&Wave_Distance_Table_00af9620[300][0]);
    for (int angle = 0; angle < 500; ++angle)
        Wave_Sine_Table_00af91e4[angle] = (short)(int)(Fast_Sin((double)angle * 0.125) * 12.0 + 0.49);
    int *intensity = Wave_Intensity_Table_00af95d8;
    for (int value = 110; value < 214; value += 8) *intensity++ = value;
    }
}


// WaveClass::~WaveClass() (0x00721900)
// Releases references and removes the wave from Waves (the cell list is
// destroyed with it).

WaveClass::~WaveClass()
{
    Unknown_006ECB90(this, true);
    Waves.Delete(this);
    Cells.Clear();
}


// void WaveClass::Brighten_Pixel_007219c0(int x, int offset, int row, unsigned short *pixel) (0x007219C0)
// Sonic wave distortion of one screen pixel (TibSun Set_Sonic_Pixel): the
// ripple amplitude at the pixel's radius picks a neighbour sample along the
// wave's direction, whose green and blue are raised by the intensity table.

// Westwood's always.h MIN (TibSun uses it for both clamps).
#ifndef MIN
#define MIN(a, b) (((a) < (b)) ? (a) : (b))
#endif

void WaveClass::Brighten_Pixel_007219c0(int x, int offset, int row, unsigned short *pixel)
{
    int radius = Wave_Distance_Table_00af9620[row][abs(x - WaveStartMiddle.X - offset)];
    int amplitude = abs(Wave_Sine_Table_00af91e4[WaveEC + radius]);
    unsigned short color = pixel[PixelOffsets_1d0[amplitude]];
    int mult = Wave_Intensity_Table_00af95d8[amplitude];

    RGBClass rgb = Deconstruct_Hicolor_Pixel(color);
    int red = rgb.Red;
    int green = rgb.Green;
    int blue = rgb.Blue;
    green += ((green * mult) >> 8);
    blue += ((blue * mult) >> 8);

    *pixel = Build_Hicolor_Pixel(red, MIN(255, green), MIN(255, blue));
}


// void WaveClass::Set_Laser_Pixel_00721b00(unsigned short *pixel, int mult) const (0x00721B00)
// Brightens one screen pixel under a laser wave (TibSun Set_Laser_Pixel): red and
// blue are raised by the wave's strength, every channel is clamped to 255. No
// caller in the executable; Draw_Laser 0x00722CB0 expands it inline.

// Westwood's always.h MIN.
#ifndef MIN
#define MIN(a, b) (((a) < (b)) ? (a) : (b))
#endif

void WaveClass::Set_Laser_Pixel_00721b00(unsigned short *pixel, int mult) const
{
    RGBClass rgb = Deconstruct_Hicolor_Pixel(*pixel);
    int red = rgb.Red;
    int green = rgb.Green;
    int blue = rgb.Blue;
    red += ((mult * 64) >> 8);
    blue += ((mult * 3 * 32) >> 8);
    *pixel = Build_Hicolor_Pixel(MIN(255, red), MIN(255, green), MIN(255, blue));
}


extern bool Wave_Tables_Initialized_00b25554;
extern unsigned short Wave_Distance_Table_00af9620[300][300];
extern short Wave_Sine_Table_00af91e4[500];
extern int Wave_Intensity_Table_00af95d8[13];
void __fastcall Initialize_Wave_Tables_00721bf0()
{
    Wave_Tables_Initialized_00b25554 = true;
    int y = 0;
    unsigned short *row = &Wave_Distance_Table_00af9620[0][0];
    do {
        unsigned short *distance = row;
        int x = 0;
        do {
            *distance = (unsigned short)(int)Fast_Sqrt((double)(x * x + y * y));
            ++x;
            ++distance;
        } while (x < 300);
        row = distance;
        ++y;
    } while ((int)row < (int)&Wave_Distance_Table_00af9620[300][0]);
    for (int angle = 0; angle < 500; ++angle)
        Wave_Sine_Table_00af91e4[angle] = (short)(int)(Fast_Sin((double)angle * 0.125) * 12.0 + 0.49);
    int *intensity = Wave_Intensity_Table_00af95d8;
    for (int value = 110; value < 214; value += 8) *intensity++ = value;
}


// void WaveClass::Init_Offset_Tables() (0x00721CC0)
// The offsets of the eight neighbours of a pixel on the current surface
// (rows are Stride / 2 pixels), and the offset of the wave's direction
// repeated once, twice and three times.

void WaveClass::Init_Offset_Tables()
{
    int row = unknown_surface_83995c->Stride() >> 1;
    DirectionStrides[0] = -row;
    DirectionStrides[1] = 1 - row;
    DirectionStrides[2] = 1;
    DirectionStrides[3] = row + 1;
    DirectionStrides[4] = row;
    DirectionStrides[5] = row - 1;
    DirectionStrides[6] = -1;
    DirectionStrides[7] = -1 - row;
    PixelOffsets_1d0[0] = 0;
    PixelOffsets_1d0[1] = 0;
    int *step = &DirectionStrides[Direction];
    PixelOffsets_1d0[2] = *step;
    PixelOffsets_1d0[3] = *step;
    PixelOffsets_1d0[4] = *step;
    PixelOffsets_1d0[5] = *step;
    PixelOffsets_1d0[6] = *step;
    PixelOffsets_1d0[7] = *step << 1;
    PixelOffsets_1d0[8] = *step << 1;
    PixelOffsets_1d0[9] = *step << 1;
    PixelOffsets_1d0[10] = *step * 3;
    PixelOffsets_1d0[11] = *step * 3;
    PixelOffsets_1d0[12] = *step * 3;
}


int __fastcall Wave_Direction_00721db0(Point2D const &start, Point2D const &end)
{
    int dx = end.X - start.X;
    int dy = start.Y - end.Y;
    double sine = Fast_Sin(0.39269908169872414);
    double tangent = sine / Fast_Cos(0.39269908169872414);
    if (dx != 0) {
        double slope = (double)dy / dx;
        if (slope < 1.0 / tangent && slope >= tangent) return dx > 0 ? 1 : 5;
        if (slope < tangent && slope >= -tangent) return dx > 0 ? 2 : 6;
        if (slope < -tangent && slope >= -1.0 / tangent) return dx > 0 ? 3 : 7;
    }
    return dy > 0 ? 0 : 4;
}


// Damage eligible cell occupants, then wall/chain-reaction overlays and terrain.
void WaveClass::Damage_Cell_00721eb0(CoordStruct const &coord)
{
    if (Owner) {
        Cell location = Coord_Cell(coord);
        CellClass &cell = Map[location];
        WeaponTypeClass *weapon = Owner->unknown_slot231(0)->WeaponType;
        WarheadTypeClass *warhead = weapon->Warhead;
        int damage = weapon->unknown_98;
        ObjectClass *object;
        if (!(cell.unknown_140 & 0x100) || StartCoord.Z < (cell.unknown_11b + 4) * unknown_geometry_g)
            object = cell.OccupierPtr;
        else object = cell.AltOccupierPtr;
        for (; object; object = object->unknown_30) {
            if (object != Owner && object->unknown_7d && object->unknown_74 && !object->unknown_76 && object->Strength > 0)
                object->Take_Damage(&damage, 0, warhead, Owner, false, 0);
        }
        if (cell.OverlayIndex != -1) {
            OverlayTypeClass *overlay = (OverlayTypeClass *)unknown_overlay_types_a35de8[cell.OverlayIndex];
            if (overlay->ChainReaction) {
                location = cell.MapCoords;
                location.unknown_47e2b0();
            }
            if (overlay->Wall) cell.Reduce_Wall(weapon->unknown_98);
        }
        if (cell.Is_Tile_Destroyable_Cliff()) {
            int chance = Rules->unknown_140c;
            if (unknown_id_source_a3d290->Random()(0, 99) < chance) Map.Unknown_00562F50(&cell);
        }
    }
}


// void WaveClass::Sonic_Add_Cell(Cell const &cell) (0x00722040)
// Adds the cell to the wave's cells unless it is already in the list.

void WaveClass::Sonic_Add_Cell(Cell const &cell)
{
    CellClass *where = &Map[cell];
    if (Cells.ID(where) == -1) {
        Cells.Add(where);
    }
}


// CoordStruct __fastcall Lerp(CoordStruct const &from, CoordStruct const &to, float t) (0x007220C0)
// The coordinate a fraction t of the way from from to to (TibSun Lerp). The result is built as
// UnknownCoord434820 (TibSun's Coord): where Wave_Recalc_Affected_Cells expands Lerp, that
// constructor stays out of line (0x00434820) as in the original.

CoordStruct __fastcall Lerp(CoordStruct const &from, CoordStruct const &to, float t)
{
    return UnknownCoord434820((int)(to.X * t + from.X * (1.0 - t)), (int)(to.Y * t + from.Y * (1.0 - t)), (int)(to.Z * t + from.Z * (1.0 - t)));
}


// Point2D __fastcall Lerp(Point2D const &from, Point2D const &to, float t) (0x00722140)
// The screen point a fraction t of the way from from to to (TibSun Lerp).

Point2D __fastcall Lerp(Point2D const &from, Point2D const &to, float t)
{
    return Point2D((int)(to.X * t + from.X * (1.0 - t)), (int)(to.Y * t + from.Y * (1.0 - t)));
}


// void WaveClass::Detach(AbstractClass *target, bool all) (0x00722190)

void WaveClass::Detach(AbstractClass *target, bool all)
{
    ObjectClass::Detach(target, all);
    if (Owner == target) {
        Owner = 0;
    }
    if (Target == target) {
        Target = 0;
    }
}


// Complete WaveClass save-game load,007221D0:349body+3NOP.

HRESULT __stdcall WaveClass::Load(IStream *stream)
{
    HRESULT result=ObjectClass::Load(stream);
    if(SUCCEEDED(result)) {
        new(this) WaveClass(NoInitClass());
        Polygon_130.Points=&Polygon_130.InlinePoint0;
        Swizzle_Pointer(Target);
        Swizzle_Pointer(Owner);
        int count;
        result=stream->Read(&count,sizeof(count),0);
        if(FAILED(result)) return result;
        int index;
        for(index=0;index<count;++index) {
            CellClass *cell;
            result=stream->Read(&cell,sizeof(cell),0);
            if(FAILED(result)) return result;
            Cells.Add(cell);
        }
        for(index=0;index<count;++index) {
            Swizzle_Pointer(Cells[index]);
        }
    }
    return result;
}


// HRESULT __stdcall WaveClass::Save(IStream *stream, BOOL clear_dirty) (0x00722330)
// AbstractClass::Save (its result is not used), then the cell count and the cell pointers.

HRESULT __stdcall WaveClass::Save(IStream *stream, BOOL clear_dirty)
{
    AbstractClass::Save(stream, clear_dirty);
    int count = Cells.Count();
    HRESULT result = stream->Write(&count, sizeof(count), 0);
    if (SUCCEEDED(result)) {
        for (int index = 0; index < count; index++) {
            result = stream->Write(&Cells[index], sizeof(CellClass *), 0);
            if (FAILED(result)) {
                break;
            }
        }
    }
    return result;
}


// HRESULT __stdcall WaveClass::GetClassID(GUID *clsid) (0x007223A0)

HRESULT __stdcall WaveClass::GetClassID(GUID *clsid)
{
    if (clsid == 0) {
        return E_POINTER;
    }
    *clsid = CLSID_WaveClass;
    return S_OK;
}


// WaveClass::Size_Of (0x007223E0)
// Compiler-emitted from shared/wave.hpp in this unit; no source of its own.


// WaveClass::In_Which_Layer (0x007223F0)
// Compiler-emitted from shared/wave.hpp in this unit; no source of its own.


// WaveClass::Class_Of (0x00722400)
// Compiler-emitted from shared/wave.hpp in this unit; no source of its own.


// bool WaveClass::Unlimbo(CoordStruct const *coord, int dir) (0x00722410)
// Places the wave at the coordinate (while the game runs, when it was
// taken off the map), marks it mapped and submits it to the layer lists.

bool WaveClass::Unlimbo(CoordStruct const *coord, int dir)
{
    if (unknown_flag_a40958 && unknown_76 && !unknown_74) {
        if (unknown_a40774 == 0 && Can_Enter_Cell(&Map[*coord], -1, coord->Z, 0, 1) != 0) {
            return false;
        }
        unknown_76 = false;
        unknown_75 = false;
        Set_Coord(*coord);
        if (Mark(1)) {
            if (unknown_7d) {
                if (In_Which_Layer() != -1) {
                    Map.Submit(this);
                }
                unknown_list_832470.Submit(this, false);
            }
            return true;
        }
    }
    return false;
}


// bool WaveClass::Limbo() (0x007224D0, slot 52)
// While the game runs (0x00A40958) and the wave is placed: slots 54(1) and
// 71(0), off the map and the 0x00832470 list, slot 69; then marks it removed.

bool WaveClass::Limbo()
{
    if (unknown_flag_a40958 && !unknown_76) {
        unknown_slot54(1);
        Mark(0);
        Map.Remove(this);
        unknown_list_832470.Remove(this);
        Hidden();
        unknown_76 = true;
        unknown_75 = false;
        return true;
    }
    return false;
}


// void WaveClass::unknown_slot67(Point2D const &point, Rect const &window) (0x00722530)
// Draws the wave by its type unless both ends are hidden (tunnels).

void WaveClass::unknown_slot67(Point2D const &point, Rect const &window)
{
    if ((unknown_id_source_a3d290->unknown_000 & 0x1000) && Map.Is_Fogged(StartCoord) && Map.Is_Fogged(EndCoord)) {
        return;
    }
    switch (Type) {
    case 0:
        Draw_Sonic_007225b0(point, window);
        break;
    case 1:
    case 2:
        Draw_Laser_00722cb0(point, window);
        break;
    }
}


// Not exact (RA2_NON_MATCHING): FUN_007225b0 at 0x007225B0, best score 86.89, 2026-11-13; differs: same instructions; registers, stack slots or operands differ
#ifdef RA2_NON_MATCHING
#include "shared/blit.hpp"
void __fastcall Rasterize_Polygon_0061be40(WavePolygonInput const &shape, WaveScanlineBuffer &raster);
extern Rect unknown_rect_839630;
#define MIN(a,b) (((a) < (b)) ? (a) : (b))
#define MAX(a,b) (((a) > (b)) ? (a) : (b))

void WaveClass::Draw_Sonic_007225b0(Point2D const &point, Rect const &cliprect)
{
    int zpix = Unknown_006A2970(StartCoord.Z);

    unsigned short *surfptr = (unsigned short *)unknown_surface_83995c->Lock();
    if (surfptr != 0) {
        int top = cliprect.Y + 3;
        int bottom = cliprect.Y + cliprect.Height - 3;
        int left = cliprect.X + 3;
        int right = cliprect.X + cliprect.Width - 3;

        int xoff = point.X - WaveStartMiddle.X;
        int yoff = point.Y - WaveStartMiddle.Y;

        Rasterize_Polygon_0061be40(Polygon_130, Scanlines_168);

        if (Scanlines_168.Spans != 0) {
            int starty = yoff + unknown_rect_839630.Y + Scanlines_168.FirstY;
            int endy = (Scanlines_168.Count + starty - 1 < cliprect.Height + cliprect.Y) ? Scanlines_168.Count + starty - 1 : cliprect.Height + cliprect.Y;

            int y;
            unsigned short zval;
            Point2D *points;

            Init_Offset_Tables();

            if (Direction > 1 && Direction < 6) {
                int xstop;
                int xstart;
                zval = (unsigned short)(DepthBuffer->MaxValue - zpix) - starty - 2;
                unsigned short *zptr = DepthBuffer->Get_Buffer_Offset(Point2D(0, starty - unknown_rect_839630.Y));
                int width = unknown_surface_83995c->Get_Width();
                int zwidth = DepthBuffer->Width;

                if (zptr + (width + zwidth * (endy - starty + 1)) < (unsigned short *)DepthBuffer->BufferTail) {
                    int stride = unknown_surface_83995c->Stride() >> 1;
                    unsigned short *dest = surfptr + starty * stride;
                    points = Scanlines_168.Spans;

                    for (y = starty; y <= endy; y++) {
                        if (y >= top && y <= bottom) {
                            xstop = MIN(xoff + points->Y, right);
                            xstart = MAX(points->X + xoff, left);

                            zptr += xstart;
                            dest += xstart;

                            int x;
                            int ypos = abs(y - yoff - WaveStartMiddle.Y);
                            for (x = xstart; x <= xstop; x++) {
                                if (*zptr > zval) {
                                    Brighten_Pixel_007219c0(x, xoff, ypos, dest);
                                }
                                zptr++;
                                dest++;
                            }
                            zptr -= x;
                            dest -= x;
                        }
                        dest += stride;
                        zptr += zwidth;
                        zval--;
                        points++;
                    }
                } else {
                    int index = 0;
                    for (y = starty; y <= endy; y++, index++) {
                        if (y >= cliprect.Y + 2 && y <= cliprect.Y + cliprect.Height - 2) {
                            Point2D *entry = &Scanlines_168.Spans[index];
                            xstop = MIN(xoff + entry->Y, cliprect.X + cliprect.Width - 2);
                            xstart = MAX(entry->X + xoff, cliprect.X + 2);

                            unsigned short *dest = surfptr + xstart + y * (unknown_surface_83995c->Stride() >> 1);
                            int zy = y - unknown_rect_839630.Y;
                            Point2D zpos(xstart, zy);
                            int ypos = abs(y - yoff - WaveStartMiddle.Y);
                            for (int x = xstart; x <= xstop; x++, dest++) {
                                if (*DepthBuffer->Get_Buffer_Offset(zpos) > zval) {
                                    Brighten_Pixel_007219c0(x, xoff, ypos, dest);
                                }
                                zpos.X++;
                            }
                        }
                        zval--;
                    }
                }
            } else {
                int xstop;
                int xstart;
                unsigned int zoffset = (unsigned int)DepthBuffer->Get_Buffer_Offset(Point2D(0, starty - unknown_rect_839630.Y));
                int width = unknown_surface_83995c->Get_Width();
                int zwidth = DepthBuffer->Width;

                if (zoffset + 2 * (width + zwidth * (endy - starty + 1)) < (unsigned int)DepthBuffer->BufferTail) {
                    int stride = unknown_surface_83995c->Stride() >> 1;
                    unsigned short *dest = surfptr + endy * stride;
                    points = &Scanlines_168.Spans[endy - starty];
                    zval = (unsigned short)(DepthBuffer->MaxValue - endy) - zpix - 2;
                    unsigned short *zptr = DepthBuffer->Get_Buffer_Offset(Point2D(0, endy - unknown_rect_839630.Y));

                    for (y = endy; y >= starty; y--) {
                        if (y >= top && y <= bottom) {
                            xstart = MIN(xoff + points->Y, right);
                            xstop = MAX(xoff + points->X, left);

                            dest += xstart;
                            zptr += xstart;

                            int x;
                            int ypos = abs(y - yoff - WaveStartMiddle.Y);
                            for (x = xstart; x >= xstop; x--) {
                                if (*zptr > zval) {
                                    Brighten_Pixel_007219c0(x, xoff, ypos, dest);
                                }
                                zptr--;
                                dest--;
                            }
                            zptr -= x;
                            dest -= x;
                        }
                        zptr -= zwidth;
                        dest -= stride;
                        zval++;
                        points--;
                    }
                } else {
                    zval = (unsigned short)(DepthBuffer->MaxValue - endy) - zpix - 2;
                    int index = endy - starty;
                    for (y = endy; y >= starty; y--, index--) {
                        if (y >= cliprect.Y + 2 && y <= cliprect.Y + cliprect.Height - 2) {
                            Point2D *entry = &Scanlines_168.Spans[index];
                            xstart = MIN(entry->Y + xoff, cliprect.X + cliprect.Width - 2);
                            xstop = MAX(xoff + entry->X, cliprect.X + 2);

                            unsigned short *dest = surfptr + xstart + y * (unknown_surface_83995c->Stride() >> 1);
                            int zy = y - unknown_rect_839630.Y;
                            Point2D zpos(xstart, zy);
                            int ypos = abs(y - yoff - WaveStartMiddle.Y);
                            for (int x = xstart; x >= xstop; x--, dest--) {
                                if (*DepthBuffer->Get_Buffer_Offset(zpos) > zval) {
                                    Brighten_Pixel_007219c0(x, xoff, ypos, dest);
                                }
                                zpos.X--;
                            }
                        }
                        zval++;
                    }
                }
            }
        }

        unknown_surface_83995c->Unlock();

        if (Scanlines_168.Spans != 0) {
            delete [] Scanlines_168.Spans;
            Scanlines_168.Spans = 0;
        }
    }
}
#endif

// Not exact (RA2_NON_MATCHING): FUN_00722cb0 at 0x00722CB0, best score 61.27, 2026-11-13; differs: 394 instructions in the original, 396 in the draft (shape 92.41)
#ifdef RA2_NON_MATCHING
extern Rect unknown_rect_839630;
#ifndef MIN
#endif
void __fastcall Rasterize_Polygon_0061be40(WavePolygonInput const &shape, WaveScanlineBuffer &raster);



void WaveClass::Draw_Laser_00722cb0(Point2D const &point, Rect const &cliprect)
{

    if (unknown_a40b2c == 2) {
        int zpix = Unknown_006A2970(StartCoord.Z);

        unsigned short *buffer = (unsigned short *)unknown_surface_83995c->Lock();
        if (buffer != 0) {

            int top = cliprect.Y;
            int bottom = cliprect.Y + cliprect.Height;
            int left = cliprect.X;
            int right = cliprect.X + cliprect.Width;

            int xoff = point.X - WaveStartMiddle.X;
            int yoff = point.Y - WaveStartMiddle.Y + unknown_rect_839630.Y;

            Rasterize_Polygon_0061be40(Polygon_130, Scanlines_168);

            if (Scanlines_168.Spans != 0) {
                unsigned short depth;
                unsigned short *zoff;
                int zwidth;
                int stride;
                unsigned short *scrptr;
                Point2D *points;
                int xstop;
                int xstart;
                int x;
                int i;
                int y;

                int ystart = yoff + Scanlines_168.FirstY;
                int yend = MIN(Scanlines_168.Count + ystart - 1, cliprect.Y + cliprect.Height);

                depth = (short)DepthBuffer->Get_Scroll() - zpix;
                depth -= ystart;
                depth -= 2;
                zoff = DepthBuffer->Get_Buffer_Offset(Point2D(0, ystart - unknown_rect_839630.Y));
                if (zoff + (unknown_surface_83995c->Get_Width() + (yend - ystart + 1) * DepthBuffer->Width) < (unsigned short *)DepthBuffer->BufferTail) {

                    stride = unknown_surface_83995c->Stride() >> 1;
                    zwidth = DepthBuffer->Width;
                    scrptr = &buffer[ystart * stride];
                    points = Scanlines_168.Spans;

                    for (y = ystart; y <= yend; y++) {
                        if (y >= top && y <= bottom) {

                            xstop = right;
                            if (points->Y + xoff < right) {
                                xstop = points->Y + xoff;
                            }
                            xstart = points->X + xoff;
                            if (xstart <= left) {
                                xstart = left;
                            }

                            zoff += xstart;
                            scrptr += xstart;

                            x = xstart;
                            for (; x <= xstop; x++) {
                                if (*zoff > depth) {
                                    Set_Laser_Pixel_00721b00(scrptr, LaserEC);
                                }
                                zoff++;
                                scrptr++;
                            }

                            zoff -= x;
                            scrptr -= x;
                        }
                        scrptr += stride;
                        zoff += zwidth;
                        depth--;
                        points++;
                    }

                } else {

                    i = 0;
                    for (y = ystart; y <= yend; y++) {
                        if (y >= cliprect.Y + 2 && y <= cliprect.Y + cliprect.Height - 2) {

                            xstop = Scanlines_168.Spans[i].Y + xoff;
                            if (xstop >= cliprect.Width + cliprect.X - 2) {
                                xstop = cliprect.Width + cliprect.X - 2;
                            }
                            xstart = cliprect.X + 2;
                            if (Scanlines_168.Spans[i].X + xoff > cliprect.X + 2) {
                                xstart = Scanlines_168.Spans[i].X + xoff;
                            }

                            scrptr = &buffer[xstart + y * (unknown_surface_83995c->Stride() >> 1)];
                            Point2D zpos(xstart, y - unknown_rect_839630.Y);

                            for (x = xstart; x <= xstop; x++) {
                                if (*DepthBuffer->Get_Buffer_Offset(zpos) > depth) {
                                    Set_Laser_Pixel_00721b00(scrptr, LaserEC);
                                }
                                zpos.X++;
                                scrptr++;
                            }
                        }
                        depth--;
                        i++;
                    }
                }
            }

            unknown_surface_83995c->Unlock();

            if (Scanlines_168.Spans != 0) {
                delete [] Scanlines_168.Spans;
                Scanlines_168.Spans = 0;
            }
        }
    }
}
#endif

// void WaveClass::AI() (0x00723220)
// A type 0 wave steps through its cells, types 1 and 2 fade.

void WaveClass::AI()
{
    switch (Type) {
    case 0:
        Sonic_AI();
        break;
    case 1:
    case 2:
        Laser_AI();
        break;
    }
}


// void WaveClass::Sonic_AI() (0x007232D0; expanded into AI, also kept out of line)
// Type 0 step: runs the cell animation, then the base AI or the end.

void WaveClass::Sonic_AI()
{
    Update_Wave_00724200();
    for (int index = 0; index < Cells.Count(); index++) {
        Damage_Cell_00721eb0(Cells[index]->unknown_475d60());
    }
    if (--WaveEC < 0) {
        Delete_Me();
    } else {
        ObjectClass::AI();
    }
}


// void WaveClass::Laser_AI() (0x00723340; expanded into AI, also kept out of line)
// Types 1 and 2 step: fade out.

void WaveClass::Laser_AI()
{
    LaserEC -= 6;
    if (LaserEC < 0x20) {
        Delete_Me();
    }
}


// void WaveClass::Wave_Recalc_Affected_Cells() (0x00723360)
// Rebuilds the list of cells the wave covers (TibSun Wave_Recalc_Affected_Cells): StartCoord/EndCoord are
// TibSun's StartCoord/EndCoord, IsWaveActive IsWaveActive, WaveProgress/FadeProgress WaveProgress/FadeProgress,
// Direction the direction. The unit sees CoordStruct::As_Cell and the Cell comparisons (RA2_COORD_AS_CELL,
// defined by the unit's first draft); their out-of-line copies are 0x00413770 and 0x0058DB50, and the budget
// runs out exactly where the original's does (the middle cell test calls them; the right edge also calls Lerp).

extern float const Wave_Advance_Step_007aec88;

void WaveClass::Wave_Recalc_Affected_Cells()
{
    Cells.Clear();

    if (IsWaveActive) {
        Cell start = StartCoord.As_Cell();

        for (double t = Wave_Advance_Step_007aec88; t <= ((Wave_Advance_Step_007aec88 / 2.0) + WaveProgress); t += Wave_Advance_Step_007aec88) {
            CoordStruct coord = Lerp(StartCoord, EndCoord, t);
            if (start != coord.As_Cell()) {
                start = coord.As_Cell();
                Sonic_Add_Cell(start);
            }
        }
    } else {
        Cell left = WaveEndLeftCoord.As_Cell();
        Cell right = WaveEndRightCoord.As_Cell();
        Cell middle = EndCoord.As_Cell();

        for (double t = FadeProgress; t <= ((Wave_Advance_Step_007aec88 / 2.0) + WaveProgress); t += Wave_Advance_Step_007aec88) {
            CoordStruct coord = unknown_geometry_l;

            if (Direction < 4) {
                coord = Lerp(WaveStartLeftCoord, WaveEndLeftCoord, t);
                if (left != coord.As_Cell()) {
                    left = coord.As_Cell();
                    Sonic_Add_Cell(left);
                }
            }

            coord = Lerp(StartCoord, EndCoord, t);
            if (middle != coord.As_Cell()) {
                middle = coord.As_Cell();
                Sonic_Add_Cell(middle);
            }

            if (Direction >= 4) {
                coord = Lerp(WaveStartRightCoord, WaveEndRightCoord, t);
                if (right != coord.As_Cell()) {
                    right = coord.As_Cell();
                    Sonic_Add_Cell(right);
                }
            }
        }
    }
}


// SPDX-License-Identifier: GPL-3.0-or-later
// Copyright 2026 ra2-decomp contributors
// Copyright 2025 Electronic Arts Inc.
// Derived from Electronic Arts source code; EA's GPL Section 7 terms apply (COPYING.md).
// void WaveClass::Draw_NonMagnetic_007238b0(CoordStruct const &source, CoordStruct const &target) -- 0x007238B0
// From ra2-decomp WaveClass::Draw_NonMagnetic (YR 0x00761640), without its YR-only
// fourth wave type; Scout801's draft for the rest.
// Purpose: places the wave between the two coordinates (shortened by the type's
// length factor) and builds its six-point outline polygon in screen pixels.
extern float const Wave_Length_Factors_007aec8c[3];

void WaveClass::Draw_NonMagnetic_007238b0(CoordStruct const &source, CoordStruct const &target)
{
    static Vector3 sizes[3][4] = {
        {Vector3(-30,-100,0),Vector3(-30,100,0),Vector3(30,-100,0),Vector3(30,100,0)},
        {Vector3(-34,-44,0),Vector3(-34,44,0),Vector3(34,-44,0),Vector3(34,44,0)},
        {Vector3(-27,-34,0),Vector3(-27,34,0),Vector3(27,-34,0),Vector3(27,34,0)}
    };
    CoordStruct start = source;
    CoordStruct end = Lerp(start,target,Wave_Length_Factors_007aec8c[Type]);
    if(Type==0)end.Z+=50;
    start=Lerp(target,start,Wave_Length_Factors_007aec8c[Type]);
    TPoint3D<float> planar_delta(start.X-end.X,start.Y-end.Y,0);
    int distance=Fast_Sqrt((double)planar_delta.Y*planar_delta.Y+(double)planar_delta.X*planar_delta.X);
    StartCoord=start;
    Set_Coord(StartCoord);
    EndCoord=end;
    Polygon_130.Count=6;
    Polygon_130.Points=&Polygon_130.InlinePoint0;
    unknown_tactical_83996c->Coord_To_Pixel(start,WaveStartMiddle);
    unknown_tactical_83996c->Coord_To_Pixel(end,WaveEndMiddle);
    Polygon_130.Points[1]=WaveEndMiddle;
    Polygon_130.Points[4]=WaveStartMiddle;
    double distance_f=distance;
    Vector3 end_left=sizes[Type][0]+Vector3(distance_f,0,0);
    Vector3 end_right=sizes[Type][1]+Vector3(distance_f,0,0);
    Vector3 start_left=sizes[Type][2]+Vector3(0,0,0);
    Vector3 start_right=sizes[Type][3]+Vector3(0,0,0);
    double dz=end.Z-start.Z;
    end_left.Z=end_left.X*dz/distance_f+start.Z;
    end_right.Z=end_right.X*dz/distance_f+start.Z;
    start_left.Z=start_left.X*dz/distance_f+start.Z;
    start_right.Z=start_right.X*dz/distance_f+start.Z;
    TPoint3D<float> heading(end.X-start.X,end.Y-start.Y,0);
    double dx=heading.X;
    double length=heading.Length();
    if(length<dx)dx=length;
    if(-dx>length)dx=-length;
    double theta=Fast_Acos_004ba580(dx/length);
    if(start.Y>end.Y)theta=-theta;
    Matrix3D matrix;
    matrix.Make_Identity();
    matrix.Rotate_Z(theta);
    end_left=matrix*end_left;
    end_right=matrix*end_right;
    start_left=matrix*start_left;
    start_right=matrix*start_right;
    WaveEndLeftCoord=CoordStruct(start.X+end_left.X,start.Y+end_left.Y,end_left.Z);
    WaveEndRightCoord=CoordStruct(start.X+end_right.X,start.Y+end_right.Y,end_right.Z);
    WaveStartLeftCoord=CoordStruct(start.X+start_left.X,start.Y+start_left.Y,start_left.Z);
    WaveStartRightCoord=CoordStruct(start.X+start_right.X,start.Y+start_right.Y,start_right.Z);
    unknown_tactical_83996c->Coord_To_Pixel(WaveEndLeftCoord,WaveEndLeft);
    unknown_tactical_83996c->Coord_To_Pixel(WaveEndRightCoord,WaveEndRight);
    unknown_tactical_83996c->Coord_To_Pixel(WaveStartLeftCoord,WaveStartLeft);
    unknown_tactical_83996c->Coord_To_Pixel(WaveStartRightCoord,WaveStartRight);
    Polygon_130.Points[0]=WaveEndLeft;
    Polygon_130.Points[2]=WaveEndRight;
    Polygon_130.Points[3]=WaveStartRight;
    Polygon_130.Points[5]=WaveStartLeft;
    Facing=Owner->unknown_2f8;
}


// The exit-time destructor of Draw_NonMagnetic_007238b0 0x007238B0's function-local static `sizes` (0x007241F0, `_$E44`):
// Vector3 has no destructor work, so the body is a lone ret. The guarded static's initialisation registers it
// with atexit.
// No source of its own: emitted by this unit (Wave.cpp) for the static in Draw_NonMagnetic_007238b0.


// Advances the wave's head/tail, updates its six-point outline, and retires it.
// QWORD progress additions and the original X^2+Z^2+Y^2 sum order are intentional.
extern float const Wave_Advance_Step_007aec88;
inline Point2D Interpolate_Wave_Pixel(Point2D const &a, Point2D const &b, float amount)
{
    return Point2D((int)(a.X * amount + b.X * (1.0 - amount)), (int)(a.Y * amount + b.Y * (1.0 - amount)));
}
void WaveClass::Update_Wave_00724200()
{
    if (WaveProgress - FadeProgress > 0.1 && Type == 0) unknown_119 = true;
    if (Target && Owner && WaveEC != (int)(1.0 / Wave_Advance_Step_007aec88) && Owner->unknown_228 == Target) {
        CoordStruct source = Owner->Coord;
        CoordStruct delta = source - Target->unknown_slot22();
        int distance = (int)Fast_Sqrt((double)delta.X * delta.X + (double)delta.Z * delta.Z + (double)delta.Y * delta.Y);
        if (distance <= unknown_geometry_diagonal * 6.0) goto update_geometry;
    }
    IsWaveActive = false;
    unknown_119 = true;
update_geometry:
    if (IsWaveActive) {
        TechnoClass *owner = Owner;
        Draw_NonMagnetic_007238b0(owner->unknown_slot43(0), Target->unknown_slot22());
    }
    if (WaveProgress < 1.0 && IsWaveActive) {
        WaveProgress += Wave_Advance_Step_007aec88;
        if (WaveProgress > 0.98) WaveProgress = 1.0;
        Polygon_130.Points[0] = Interpolate_Wave_Pixel(WaveEndLeft, WaveStartLeft, WaveProgress);
        Polygon_130.Points[1] = Interpolate_Wave_Pixel(WaveEndMiddle, WaveStartMiddle, WaveProgress);
        Polygon_130.Points[2] = Interpolate_Wave_Pixel(WaveEndRight, WaveStartRight, WaveProgress);
    }
    if (unknown_119 && FadeProgress <= Wave_Advance_Step_007aec88 * 0.5 + WaveProgress) {
        FadeProgress += Wave_Advance_Step_007aec88;
        if (FadeProgress >= WaveProgress) {
            Delete_Me();
            return;
        }
        Polygon_130.Points[3] = Interpolate_Wave_Pixel(WaveEndRight, WaveStartRight, FadeProgress);
        Polygon_130.Points[4] = Interpolate_Wave_Pixel(WaveEndMiddle, WaveStartMiddle, FadeProgress);
        Polygon_130.Points[5] = Interpolate_Wave_Pixel(WaveEndLeft, WaveStartLeft, FadeProgress);
    }
    Wave_Recalc_Affected_Cells();
}


// WaveClass::Kind_Of (0x007245F0)
// Compiler-emitted from shared/wave.hpp in this unit; no source of its own.


// WaveClass::scalar_deleting_destructor (0x00724600)
// Compiler-emitted from shared/wave.hpp in this unit; no source of its own.


// Original region: 0x007246d0 (93 body bytes, 3 independently frozen NOP bytes).
// Exact target SHA256: e6418111c2c36081a1795fb000e0927beea1630170b334c5013d12102602e70f.
// Purpose: initialize an inferred four-byte-element storage wrapper.
// Class/API identity and the three unknown field purposes remain unresolved.

extern "C" void* __cdecl allocate_storage_007246d0(unsigned int byte_count);
extern "C" unsigned char base_dispatch_007246d0[];
extern "C" unsigned char derived_dispatch_007246d0[];

class WordStorage_007246d0 {
    unsigned char* dispatch_table;
    unsigned int* storage;
    unsigned int capacity;
    unsigned char unknown_0x0c;
    unsigned char owns_storage;
    // Bytes 0x0e..0x0f are alignment padding and are not written.
    unsigned int unknown_0x10;
    unsigned int unknown_0x14;

public:
    WordStorage_007246d0* initialize(unsigned int requested_capacity,
                                unsigned int* supplied_storage);
};

typedef char WordStorage_007246d0_size_check[(sizeof(WordStorage_007246d0) == 24) ? 1 : -1];

WordStorage_007246d0* WordStorage_007246d0::initialize(
    unsigned int requested_capacity,
    unsigned int* supplied_storage)
{
    storage = 0;
    capacity = requested_capacity;
    unknown_0x0c = 1;
    owns_storage = 0;
    dispatch_table = base_dispatch_007246d0;

    // A zero capacity ignores even a nonnull supplied pointer.
    if (requested_capacity != 0) {
        if (supplied_storage != 0) {
            storage = supplied_storage;
        } else {
            // Preserve 32-bit wrapping and ownership even on a null result.
            storage = (unsigned int*)allocate_storage_007246d0(requested_capacity << 2);
            owns_storage = 1;
        }
    }

    dispatch_table = derived_dispatch_007246d0;
    unknown_0x14 = 10;
    unknown_0x10 = 0;
    return this;
}


// Container members this unit emits by its own use of its lists (bodies: shared/vector_class.hpp):
// DynamicVectorClass<CellClass *>::Add (0x00724730): emitted by this unit's use of its list.
