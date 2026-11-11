// NON-EXACT DRAFT 0x52C230 FUN_0052c230
// entry 0x52C230; exact-match score 81.25 (misses_index best); plan "draft, verify"
// origin lane: Scout978 (build/class-work/Scout978/keep/probe_k6.cpp)
// Recovered source text by our lanes/workers; never exact-match credit. Tracked by
// Scout1069 (native/drafts.json). Not original bytes.
// Native adaptation (Scout1069): callers' unknown_func_52c230 view forwards to the draft's Draw_Tile; each changed line is marked NATIVE.
// Native (Scout1091): IsoTileSet::Tiles as ra2port::IsoTileRef (the image's 4-byte slots; 8-byte pointers misread them);
// IsoBlitState's pointer-holding ints and the ring-end tests at pointer width; the bss state and tables defined here.
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

// Draw_Tile body shape from TibSun isotype.cpp (Draw_Tile, line 1665), rewritten to the RA2 target: global
// cursor loops, 60x30 tiles, tail-call variation walk, IsoHalfbrightMask as its own .bss short (Scout844).
// void IsometricTileTypeClass::Draw_Tile(LightConvertClass *drawer, int subtile, Surface &surface, int x, int y,
//     Rect cliprect, int height, int brightness, bool use_z, int cell_variation, bool fill, bool depth_only, bool fog,
//     int fog_color) const (0x0052C230)
// Draws one sub tile of the tile set: picks the requested variation, builds the iso-diamond span tables once,
// clips the 60x29 diamond and blits it lit (alpha buffer) and depth tested (depth buffer), or stamps a fill,
// depth-only or fog pass; then the cliff's extra image (TibSun Draw_Tile; RA2 60x30 tiles, and a tile without
// the sub tile record returns at once). 0x00839C6C is TibSun's DepthBuffer here, 0x008315B4 its AlphaBuffer.
// @ref ?IsoDrawData@@3UIsoBlitState@@A=00a52f08
// @ref ?IsoHalfbrightMask@@3GA=00a52f70
// @ref ?IsoSpanTablesBuilt@@3DA=00a6e395
// @ref ?IsoTileUseAsmDrawFunc@@3DA=00a6e394
// @ref ?Release@UnknownAlphaTableCache_008315B8@@QAEXPAY0BAA@$$CBG@Z=0041e510
// @ref ?_iso_row_offsets@@3PAGA=00a6cf68
// @ref ?_iso_run_lengths@@3PAY0GMM@EA=00a53398
// @ref ?_iso_start_cols@@3PAEA=00a52594
// @ref _?_iso_row_bases@?1??Draw_Tile@IsometricTileTypeClass@@QBEXPAVLightConvertClass@@HAAVSurface@@HHVRect@@HH_NH333H@Z@4QBHB=007a49b0
// @ref _?_tilemask@?1??Draw_Tile@IsometricTileTypeClass@@QBEXPAVLightConvertClass@@HAAVSurface@@HHVRect@@HH_NH333H@Z@4QBEB=007a4a28
// @ref _Iso_Blit_Asm2=0077f3df
#include "shared/isometric_tile_type.hpp"
#include "shared/surface.hpp"
#include "shared/blit.hpp"
#include "shared/alpha_table_cache.hpp"
#include "shared/hicolor.hpp"
// The tactical area (0x00839630; shared/display.hpp).
extern Rect unknown_rect_839630;
#include "shared/rules.hpp"
#include "shared/always.hpp"

#ifndef NULL
#define NULL 0
#endif
#define ISO_DRAW_WIDTH 60
#define ISO_DRAW_HEIGHT 29
#define ISO_HEIGHT 30
#ifndef MAX
#define MAX(a,b) (((a) > (b)) ? (a) : (b))
#endif
#ifndef LOWORD
#define LOWORD(l) ((unsigned short)(l))
#endif

#pragma pack(4)
struct IsoTileRecord {
    int X;
    int Y;
    int ExtraOffset;
    int ZDataOffset;
    int ExtraZOffset;
    int ExtraX;
    int ExtraY;
    int ExtraWidth;
    int ExtraHeight;
    unsigned int IsHasExtraData:1;
    unsigned int IsHasZData:1;
    unsigned int IsRandomized:1;
    unsigned char Height;
    signed char TileType;
    signed char RampType;
    unsigned char LowColor[3];
    unsigned char HighColor[3];
};
#pragma pack()

struct IsoTileSet {
    IsoTileRecord const * Fetch_Record_Pointer(int index) const
    {
        return (IsoTileRecord const *)Tiles[index % Tile_Count()].get();   // NATIVE: ra2port::IsoTileRef slots (Scout1091)
    }
    int Tile_Count() const { return MapWidth * MapHeight; }
    int MapWidth;
    int MapHeight;
    int unknown_08;
    int Height;
    ra2port::IsoTileRef Tiles[1];   // NATIVE: the file's 4-byte record slots, resolved by Load_Tile_Data/Read_Control_File (Scout1091; was IsoTileRecord *)
};

#pragma pack(push, 1)
struct IsoBlitState {
    intptr_t SrcPixel;   // NATIVE: holds a pointer (was int; Scout1091)
    intptr_t SrcDepth;   // NATIVE: holds a pointer (was int)
    unsigned short *PixelTranslate;
    unsigned short *DepthPtr;
    unsigned short *AlphaPtr;
    unsigned short *DestPtr;
    unsigned short *LightRemap;
    unsigned short *RowSrcOffset;
    unsigned char *RowStartCol;
    unsigned char *RowRunLength;
    unsigned char *ImageBase;
    unsigned char *DepthBase;
    unsigned short *RowDest;
    unsigned short *RowDepth;
    unsigned short *RowAlpha;
    signed int SpanWidth;
    signed int SpanHeight;
    unsigned int ClipLeft;
    unsigned int ClipTop;
    unsigned int BaseDepth;
    unsigned int DepthWidth;
    unsigned int AlphaWidth;
    unsigned int SurfacePitch;
    unsigned int ImageRowStep;
    unsigned int FillDepth;
    unsigned int FogColor;
};
#pragma pack(pop)

unsigned short IsoHalfbrightMask;                                       // 0x00A52F70
// NATIVE (Scout1091): the draft defines the blitter state and span tables (bss in the
// original; the link closure had made them trap functions). IsoTileUseAsmDrawFunc stays
// false as in TibSun isotype.cpp (never enabled): the C++ loops draw, Iso_Blit_Asm2 is not reached.
IsoBlitState IsoDrawData;                                                // 0x00A52F08
unsigned short _iso_row_offsets[ISO_DRAW_WIDTH*ISO_DRAW_HEIGHT];         // 0x00A6CF68
unsigned char _iso_start_cols[ISO_DRAW_WIDTH*ISO_DRAW_HEIGHT];           // 0x00A52594
unsigned char _iso_run_lengths[ISO_DRAW_WIDTH][ISO_DRAW_WIDTH*ISO_DRAW_HEIGHT];  // 0x00A53398
char IsoSpanTablesBuilt;                                                 // 0x00A6E395
char IsoTileUseAsmDrawFunc;                                              // 0x00A6E394
extern "C" void __cdecl Iso_Blit_Asm2(IsoBlitState *state);             // 0x0077F3DF

// The alpha-lighting row for a brightness (0..2000 -> 0..254).
static inline int Light_Level(int brightness)
{
    int level = ((brightness > 0 ? brightness : 0) * 261) >> 11;
    if (level > 254) {
        level = 254;
    }
    return level;
}

void IsometricTileTypeClass::Draw_Tile(LightConvertClass * drawer, int subtile, Surface & surface, int x, int y, Rect cliprect, int height, int brightness, bool use_z, int cell_variation, bool fill, bool depth_only, bool fog, signed int fog_color) const
{
    static int const _iso_row_bases[ISO_HEIGHT] = {
        0, 4, 12, 24, 40, 60, 84, 112, 144, 180, 220, 264, 312, 364, 420, 480, 536, 588, 636, 680, 720, 756, 788, 816, 840, 860, 876, 888, 896, 900
    };
    #define __ "\x20"
    #define XX "\xDB"
    static const unsigned char _tilemask[] = {
        __ __ __ __ __ __ __ __ __ __ __ __ __ __ __ __ __ __ __ __ __ __ __ __ __ __ __ __ XX XX XX XX __ __ __ __ __ __ __ __ __ __ __ __ __ __ __ __ __ __ __ __ __ __ __ __ __ __ __ __
        __ __ __ __ __ __ __ __ __ __ __ __ __ __ __ __ __ __ __ __ __ __ __ __ __ __ XX XX XX XX XX XX XX XX __ __ __ __ __ __ __ __ __ __ __ __ __ __ __ __ __ __ __ __ __ __ __ __ __ __
        __ __ __ __ __ __ __ __ __ __ __ __ __ __ __ __ __ __ __ __ __ __ __ __ XX XX XX XX XX XX XX XX XX XX XX XX __ __ __ __ __ __ __ __ __ __ __ __ __ __ __ __ __ __ __ __ __ __ __ __
        __ __ __ __ __ __ __ __ __ __ __ __ __ __ __ __ __ __ __ __ __ __ XX XX XX XX XX XX XX XX XX XX XX XX XX XX XX XX __ __ __ __ __ __ __ __ __ __ __ __ __ __ __ __ __ __ __ __ __ __
        __ __ __ __ __ __ __ __ __ __ __ __ __ __ __ __ __ __ __ __ XX XX XX XX XX XX XX XX XX XX XX XX XX XX XX XX XX XX XX XX __ __ __ __ __ __ __ __ __ __ __ __ __ __ __ __ __ __ __ __
        __ __ __ __ __ __ __ __ __ __ __ __ __ __ __ __ __ __ XX XX XX XX XX XX XX XX XX XX XX XX XX XX XX XX XX XX XX XX XX XX XX XX __ __ __ __ __ __ __ __ __ __ __ __ __ __ __ __ __ __
        __ __ __ __ __ __ __ __ __ __ __ __ __ __ __ __ XX XX XX XX XX XX XX XX XX XX XX XX XX XX XX XX XX XX XX XX XX XX XX XX XX XX XX XX __ __ __ __ __ __ __ __ __ __ __ __ __ __ __ __
        __ __ __ __ __ __ __ __ __ __ __ __ __ __ XX XX XX XX XX XX XX XX XX XX XX XX XX XX XX XX XX XX XX XX XX XX XX XX XX XX XX XX XX XX XX XX __ __ __ __ __ __ __ __ __ __ __ __ __ __
        __ __ __ __ __ __ __ __ __ __ __ __ XX XX XX XX XX XX XX XX XX XX XX XX XX XX XX XX XX XX XX XX XX XX XX XX XX XX XX XX XX XX XX XX XX XX XX XX __ __ __ __ __ __ __ __ __ __ __ __
        __ __ __ __ __ __ __ __ __ __ XX XX XX XX XX XX XX XX XX XX XX XX XX XX XX XX XX XX XX XX XX XX XX XX XX XX XX XX XX XX XX XX XX XX XX XX XX XX XX XX __ __ __ __ __ __ __ __ __ __
        __ __ __ __ __ __ __ __ XX XX XX XX XX XX XX XX XX XX XX XX XX XX XX XX XX XX XX XX XX XX XX XX XX XX XX XX XX XX XX XX XX XX XX XX XX XX XX XX XX XX XX XX __ __ __ __ __ __ __ __
        __ __ __ __ __ __ XX XX XX XX XX XX XX XX XX XX XX XX XX XX XX XX XX XX XX XX XX XX XX XX XX XX XX XX XX XX XX XX XX XX XX XX XX XX XX XX XX XX XX XX XX XX XX XX __ __ __ __ __ __
        __ __ __ __ XX XX XX XX XX XX XX XX XX XX XX XX XX XX XX XX XX XX XX XX XX XX XX XX XX XX XX XX XX XX XX XX XX XX XX XX XX XX XX XX XX XX XX XX XX XX XX XX XX XX XX XX __ __ __ __
        __ __ XX XX XX XX XX XX XX XX XX XX XX XX XX XX XX XX XX XX XX XX XX XX XX XX XX XX XX XX XX XX XX XX XX XX XX XX XX XX XX XX XX XX XX XX XX XX XX XX XX XX XX XX XX XX XX XX __ __
        XX XX XX XX XX XX XX XX XX XX XX XX XX XX XX XX XX XX XX XX XX XX XX XX XX XX XX XX XX XX XX XX XX XX XX XX XX XX XX XX XX XX XX XX XX XX XX XX XX XX XX XX XX XX XX XX XX XX XX XX
        __ __ XX XX XX XX XX XX XX XX XX XX XX XX XX XX XX XX XX XX XX XX XX XX XX XX XX XX XX XX XX XX XX XX XX XX XX XX XX XX XX XX XX XX XX XX XX XX XX XX XX XX XX XX XX XX XX XX __ __
        __ __ __ __ XX XX XX XX XX XX XX XX XX XX XX XX XX XX XX XX XX XX XX XX XX XX XX XX XX XX XX XX XX XX XX XX XX XX XX XX XX XX XX XX XX XX XX XX XX XX XX XX XX XX XX XX __ __ __ __
        __ __ __ __ __ __ XX XX XX XX XX XX XX XX XX XX XX XX XX XX XX XX XX XX XX XX XX XX XX XX XX XX XX XX XX XX XX XX XX XX XX XX XX XX XX XX XX XX XX XX XX XX XX XX __ __ __ __ __ __
        __ __ __ __ __ __ __ __ XX XX XX XX XX XX XX XX XX XX XX XX XX XX XX XX XX XX XX XX XX XX XX XX XX XX XX XX XX XX XX XX XX XX XX XX XX XX XX XX XX XX XX XX __ __ __ __ __ __ __ __
        __ __ __ __ __ __ __ __ __ __ XX XX XX XX XX XX XX XX XX XX XX XX XX XX XX XX XX XX XX XX XX XX XX XX XX XX XX XX XX XX XX XX XX XX XX XX XX XX XX XX __ __ __ __ __ __ __ __ __ __
        __ __ __ __ __ __ __ __ __ __ __ __ XX XX XX XX XX XX XX XX XX XX XX XX XX XX XX XX XX XX XX XX XX XX XX XX XX XX XX XX XX XX XX XX XX XX XX XX __ __ __ __ __ __ __ __ __ __ __ __
        __ __ __ __ __ __ __ __ __ __ __ __ __ __ XX XX XX XX XX XX XX XX XX XX XX XX XX XX XX XX XX XX XX XX XX XX XX XX XX XX XX XX XX XX XX XX __ __ __ __ __ __ __ __ __ __ __ __ __ __
        __ __ __ __ __ __ __ __ __ __ __ __ __ __ __ __ XX XX XX XX XX XX XX XX XX XX XX XX XX XX XX XX XX XX XX XX XX XX XX XX XX XX XX XX __ __ __ __ __ __ __ __ __ __ __ __ __ __ __ __
        __ __ __ __ __ __ __ __ __ __ __ __ __ __ __ __ __ __ XX XX XX XX XX XX XX XX XX XX XX XX XX XX XX XX XX XX XX XX XX XX XX XX __ __ __ __ __ __ __ __ __ __ __ __ __ __ __ __ __ __
        __ __ __ __ __ __ __ __ __ __ __ __ __ __ __ __ __ __ __ __ XX XX XX XX XX XX XX XX XX XX XX XX XX XX XX XX XX XX XX XX __ __ __ __ __ __ __ __ __ __ __ __ __ __ __ __ __ __ __ __
        __ __ __ __ __ __ __ __ __ __ __ __ __ __ __ __ __ __ __ __ __ __ XX XX XX XX XX XX XX XX XX XX XX XX XX XX XX XX __ __ __ __ __ __ __ __ __ __ __ __ __ __ __ __ __ __ __ __ __ __
        __ __ __ __ __ __ __ __ __ __ __ __ __ __ __ __ __ __ __ __ __ __ __ __ XX XX XX XX XX XX XX XX XX XX XX XX __ __ __ __ __ __ __ __ __ __ __ __ __ __ __ __ __ __ __ __ __ __ __ __
        __ __ __ __ __ __ __ __ __ __ __ __ __ __ __ __ __ __ __ __ __ __ __ __ __ __ XX XX XX XX XX XX XX XX __ __ __ __ __ __ __ __ __ __ __ __ __ __ __ __ __ __ __ __ __ __ __ __ __ __
        __ __ __ __ __ __ __ __ __ __ __ __ __ __ __ __ __ __ __ __ __ __ __ __ __ __ __ __ XX XX XX XX __ __ __ __ __ __ __ __ __ __ __ __ __ __ __ __ __ __ __ __ __ __ __ __ __ __ __ __
        __ __ __ __ __ __ __ __ __ __ __ __ __ __ __ __ __ __ __ __ __ __ __ __ __ __ __ __ __ __ __ __ __ __ __ __ __ __ __ __ __ __ __ __ __ __ __ __ __ __ __ __ __ __ __ __ __ __ __ __
    };
    #undef __
    #undef XX
    if (drawer != NULL) {
        /*
         * If a variation was requested, walk the tile-set list to it and draw that
         * tile instead of this one (a tail call: VC6 turns it into a jump).
         */
        IsoHalfbrightMask = unknown_half_mask_852fd0;
        if (cell_variation != 0) {
            int num = NumTileTypesInSet;
            if (cell_variation > num - 1) {
                cell_variation %= num;
            }
            if (cell_variation != 0) {
                IsometricTileTypeClass const * next = this;
                do {
                    next = (IsometricTileTypeClass const *)next->unknown_28c;
                } while (--cell_variation != 0);
                if (next != this) {
                    next->Draw_Tile(drawer, subtile, surface, x, y, cliprect, height, brightness, use_z, 0, fill, depth_only, fog, fog_color);
                    return;
                }
            }
        }
        IsometricTileTypeClass const * tileptr = this;
        int work_x = x;
        int work_y = y;
        bool clipped_out = false;
        AlphaTableRows aremap = AlphaTableCache_008315B8.Get(drawer->unknown_shadow_count);
        IsoDrawData.LightRemap = (unsigned short *)aremap[Light_Level(brightness)];
        IsoDrawData.PixelTranslate = (unsigned short *)drawer->AlphaPalette;
        /*
         * One-time build of the iso-diamond span tables. For each of the ISO_DRAW_HEIGHT
         * scanlines of the ISO_DRAW_WIDTH-wide tile diamond record the left column of the
         * solid span (_iso_start_cols), the per-pixel source offset (_iso_row_offsets)
         * and, for every possible right-edge clip, the run length (_iso_run_lengths).
         * Driven by the diamond bitmap in _tilemask.
         */
        if (!IsoSpanTablesBuilt) {
            int cell_offset = 0;
            const int * lut = _iso_row_bases;
            int loop_pos = 0;
            do {
                int span_count;
                /*
                 * Find the first non-space column of this mask row and the
                 * length of the solid span that follows it.
                 */
                int first_col;
                for (first_col = 0; first_col < ISO_DRAW_WIDTH; ++first_col) {
                    if (_tilemask[cell_offset + first_col] != 32) {
                        span_count = 1;
                        while (first_col + span_count < ISO_DRAW_WIDTH && _tilemask[cell_offset + first_col + span_count] != 32) {
                            ++span_count;
                        }
                        break;
                    }
                }
                if (first_col >= ISO_DRAW_WIDTH) {
                    first_col = 0;
                    span_count = 0;
                }
                {
                    int col = 0;
                    unsigned short * shortp = (unsigned short *)&((char *)_iso_row_offsets)[loop_pos];
                    int base = *lut;
                    int startcol = first_col;
                    do {
                        _iso_start_cols[cell_offset + col] = startcol < 0 ? 0 : startcol;
                        *shortp++ = (unsigned short)(base + max(0, col - first_col));
                        ++col;
                        --startcol;
                    } while (col < ISO_DRAW_WIDTH);
                }
                {
                    unsigned char * runp = &_iso_run_lengths[ISO_DRAW_WIDTH-1][cell_offset];
                    int left_clip = span_count + first_col - ISO_DRAW_WIDTH;
                    for (int rowcount = ISO_DRAW_WIDTH; rowcount != 0; --rowcount) {
                        for (int i = 0; i < ISO_DRAW_WIDTH; ++i) {
                            int run = span_count - max(0, i - first_col) - max(0, left_clip);
                            runp[i] = max(0, run);
                        }
                        runp -= (ISO_DRAW_WIDTH*ISO_DRAW_HEIGHT);
                        ++left_clip;
                    }
                }
                cell_offset += ISO_DRAW_WIDTH;
                loop_pos += (ISO_DRAW_WIDTH * 2);
                ++lut;
            } while (loop_pos < ((ISO_DRAW_WIDTH*ISO_DRAW_HEIGHT) *2));
            IsoSpanTablesBuilt = 1;
        }
        if (surface.Bytes_Per_Pixel() == 2) {
            const IsoTileSet * set = (const IsoTileSet *)tileptr->unknown_slot39();
            if (set != NULL) {
                if (use_z) {
                    IsoDrawData.BaseDepth = (unsigned short)(LOWORD(DepthBuffer->MaxValue) + LOWORD(DepthBuffer->Area.Y) - y - LOWORD(set->Height));
                    IsoDrawData.BaseDepth += height * set->Height / -2;   // NATIVE: the same sum as integers (the draft formed it through a pointer; Scout1246)
                }
                const IsoTileRecord * record = set->Fetch_Record_Pointer(subtile);
                if (record == NULL) {
                    return;
                }
                int spanw = ISO_DRAW_WIDTH;
                int spanh = ISO_DRAW_HEIGHT;
                IsoDrawData.SpanWidth = spanw;
                IsoDrawData.SpanHeight = spanh;
                IsoDrawData.ClipLeft = 0;
                IsoDrawData.ClipTop = 0;
                IsoDrawData.RowSrcOffset = _iso_row_offsets;
                IsoDrawData.RowStartCol = (unsigned char *)_iso_start_cols;
                IsoDrawData.RowRunLength = (unsigned char *)_iso_run_lengths;
                /*
                 * Clip the ISO_DRAW_WIDTHxISO_DRAW_HEIGHT tile diamond against the clip rectangle, adjusting
                 * the span dimensions and the left/top source offsets. If the tile
                 * clips away entirely, skip straight to the extra-image pass.
                 */
                if (x < cliprect.X) {
                    spanw = x - cliprect.X + IsoDrawData.SpanWidth;
                    IsoDrawData.ClipLeft = cliprect.X - x;
                    IsoDrawData.SpanWidth = spanw;
                    work_x = cliprect.X;
                    if (spanw <= 0) {
                        clipped_out = true;
                    }
                }
                if (y < cliprect.Y) {
                    spanh = y - cliprect.Y + IsoDrawData.SpanHeight;
                    IsoDrawData.ClipTop = cliprect.Y - y;
                    IsoDrawData.SpanHeight = spanh;
                    work_y = cliprect.Y;
                    if (spanh <= 0) {
                        clipped_out = true;
                    }
                }
                int right_clip = ISO_DRAW_WIDTH-1;
                int lock_x = work_x;
                int clip_right = cliprect.Width + cliprect.X;
                if (work_x + spanw > clip_right) {
                    int overhang = spanw - cliprect.Width - cliprect.X + work_x;
                    spanw -= overhang;
                    right_clip -= overhang;
                    IsoDrawData.SpanWidth = spanw;
                    if (spanw <= 0) {
                        clipped_out = true;
                    }
                }
                int clip_bottom = cliprect.Y + cliprect.Height;
                if (IsoDrawData.SpanHeight + work_y > clip_bottom) {
                    IsoDrawData.SpanHeight = cliprect.Height - work_y + cliprect.Y;
                    if (IsoDrawData.SpanHeight <= 0) {
                        clipped_out = true;
                    }
                }
                bool skip_extra = false;
                if (!clipped_out) {
                    {
                        int clip_x = IsoDrawData.ClipLeft;
                        int clip_y = IsoDrawData.ClipTop;
                        if (clip_x > ISO_DRAW_WIDTH-1) {
                            clip_x = ISO_DRAW_WIDTH-1;
                            IsoDrawData.ClipLeft = clip_x;
                        }
                        if (clip_y > ISO_DRAW_HEIGHT-1) {
                            clip_y = ISO_DRAW_HEIGHT-1;
                            IsoDrawData.ClipTop = clip_y;
                        }
                        int table_off = clip_x + ISO_DRAW_WIDTH * clip_y;
                        IsoDrawData.RowSrcOffset = &_iso_row_offsets[table_off];
                        IsoDrawData.RowStartCol = (unsigned char *)&_iso_start_cols[table_off];
                        IsoDrawData.RowRunLength = (unsigned char *)&_iso_run_lengths[MAX(0, right_clip)][table_off];
                    }
                    IsoDrawData.SurfacePitch = surface.Stride();
                    IsoDrawData.DestPtr = (unsigned short *)surface.Lock(Point2D(lock_x, work_y));
                    if (IsoDrawData.DestPtr == NULL) {
                        skip_extra = true;
                    } else {
                        if (!record->IsHasZData) {
                            use_z = false;
                        } else if (use_z) {
                            IsoDrawData.DepthPtr = DepthBuffer->Get_Buffer_Offset(Point2D(lock_x, work_y - unknown_rect_839630.Y));
                            IsoDrawData.DepthWidth = DepthBuffer->Width;
                        }
                        IsoDrawData.AlphaPtr = AlphaBuffer->Get_Buffer_Offset(Point2D(lock_x, work_y - unknown_rect_839630.Y));
                        IsoDrawData.AlphaWidth = AlphaBuffer->Width;
                        IsoDrawData.SrcPixel = (intptr_t)(record + 1);
                        unsigned char const * image = (unsigned char const *)IsoDrawData.SrcPixel;
                        if (fill || fog) {
                            IsoDrawData.FillDepth = 0;
                        } else {
                            IsoDrawData.FillDepth = 0xFFFF;
                        }
                        IsoDrawData.FogColor = IsoHalfbrightMask & (fog_color >> 1);
                        if (use_z && record->IsHasZData) {
                            IsoDrawData.SrcDepth = (intptr_t)record + record->ZDataOffset;
                        }
                        IsoDrawData.ImageBase = (unsigned char *)(record + 1);
                        IsoDrawData.DepthBase = (unsigned char *)IsoDrawData.SrcDepth;
                        if (use_z) {
                            if ((uintptr_t)&IsoDrawData.DepthPtr[IsoDrawData.SpanWidth + 2 + IsoDrawData.SpanHeight * DepthBuffer->Width] >= (uintptr_t)DepthBuffer->BufferTail) {
                                if (fill) {
                                    for (int row = 0; row < IsoDrawData.SpanHeight; ++row) {
                                        int start = *IsoDrawData.RowStartCol;
                                        IsoDrawData.RowDest = IsoDrawData.DestPtr;
                                        IsoDrawData.RowDepth = IsoDrawData.DepthPtr;
                                        IsoDrawData.DestPtr += start;
                                        IsoDrawData.DepthPtr += start;
                                        IsoDrawData.DepthPtr = DepthBuffer->Wrap_Pointer(IsoDrawData.DepthPtr);
                                        int run = *IsoDrawData.RowRunLength;
                                        if (run > 0) {
                                            do {
                                                *IsoDrawData.DepthPtr = IsoDrawData.FillDepth;
                                                *IsoDrawData.DestPtr = (unsigned short)IsoDrawData.FogColor;
                                                ++IsoDrawData.DestPtr;
                                                ++IsoDrawData.DepthPtr;
                                                IsoDrawData.DepthPtr = DepthBuffer->Wrap_Pointer(IsoDrawData.DepthPtr);
                                                --run;
                                            } while (run != 0);
                                        }
                                        IsoDrawData.DestPtr = (unsigned short *)((char *)IsoDrawData.RowDest + IsoDrawData.SurfacePitch);
                                        IsoDrawData.DepthPtr = &IsoDrawData.RowDepth[IsoDrawData.DepthWidth];
                                        IsoDrawData.RowStartCol += ISO_DRAW_WIDTH;
                                        IsoDrawData.RowRunLength += ISO_DRAW_WIDTH;
                                    }
                                } else if (depth_only) {
                                    for (int row = 0; row < IsoDrawData.SpanHeight; ++row) {
                                        int start = *IsoDrawData.RowStartCol;
                                        IsoDrawData.RowDepth = IsoDrawData.DepthPtr;
                                        IsoDrawData.DepthPtr += start;
                                        IsoDrawData.DepthPtr = DepthBuffer->Wrap_Pointer(IsoDrawData.DepthPtr);
                                        int run = *IsoDrawData.RowRunLength;
                                        if (run > 0) {
                                            do {
                                                *IsoDrawData.DepthPtr = IsoDrawData.FillDepth;
                                                ++IsoDrawData.DepthPtr;
                                                IsoDrawData.DepthPtr = DepthBuffer->Wrap_Pointer(IsoDrawData.DepthPtr);
                                                --run;
                                            } while (run != 0);
                                        }
                                        IsoDrawData.DepthPtr = &IsoDrawData.RowDepth[IsoDrawData.DepthWidth];
                                        IsoDrawData.RowStartCol += ISO_DRAW_WIDTH;
                                        IsoDrawData.RowRunLength += ISO_DRAW_WIDTH;
                                    }
                                } else if (fog) {
                                    if (!Rules->BlendedFog) {
                                        for (int row = 0; row < IsoDrawData.SpanHeight; ++row) {
                                            IsoDrawData.SrcPixel = (intptr_t)IsoDrawData.ImageBase + *IsoDrawData.RowSrcOffset;
                                            int start = *IsoDrawData.RowStartCol;
                                            IsoDrawData.RowDest = IsoDrawData.DestPtr;
                                            IsoDrawData.RowDepth = IsoDrawData.DepthPtr;
                                            IsoDrawData.DestPtr += start;
                                            IsoDrawData.DepthPtr += start;
                                            IsoDrawData.DepthPtr = DepthBuffer->Wrap_Pointer(IsoDrawData.DepthPtr);
                                            int checker = ((unsigned char *)IsoDrawData.SrcPixel - image + row + IsoDrawData.ClipTop) & 1;
                                            int run = *IsoDrawData.RowRunLength;
                                            if (run > 0) {
                                                do {
                                                    if (checker) {
                                                        *IsoDrawData.DestPtr = (unsigned short)IsoDrawData.FogColor;
                                                    }
                                                    *IsoDrawData.DepthPtr = 0;
                                                    checker = checker == 0;
                                                    ++IsoDrawData.SrcPixel;
                                                    ++IsoDrawData.DestPtr;
                                                    ++IsoDrawData.DepthPtr;
                                                    IsoDrawData.DepthPtr = DepthBuffer->Wrap_Pointer(IsoDrawData.DepthPtr);
                                                    --run;
                                                } while (run != 0);
                                            }
                                            IsoDrawData.DestPtr = (unsigned short *)((char *)IsoDrawData.RowDest + IsoDrawData.SurfacePitch);
                                            IsoDrawData.DepthPtr = &IsoDrawData.RowDepth[IsoDrawData.DepthWidth];
                                            IsoDrawData.RowStartCol += ISO_DRAW_WIDTH;
                                            IsoDrawData.RowRunLength += ISO_DRAW_WIDTH;
                                        }
                                    } else {
                                        for (int row = 0; row < IsoDrawData.SpanHeight; ++row) {
                                            IsoDrawData.SrcPixel = (intptr_t)IsoDrawData.ImageBase + *IsoDrawData.RowSrcOffset;
                                            int start = *IsoDrawData.RowStartCol;
                                            IsoDrawData.RowDest = IsoDrawData.DestPtr;
                                            IsoDrawData.RowDepth = IsoDrawData.DepthPtr;
                                            IsoDrawData.DestPtr += start;
                                            IsoDrawData.DepthPtr += start;
                                            IsoDrawData.DepthPtr = DepthBuffer->Wrap_Pointer(IsoDrawData.DepthPtr);
                                            int run = *IsoDrawData.RowRunLength;
                                            if (run > 0) {
                                                do {
                                                    if (*IsoDrawData.DepthPtr > (int)IsoDrawData.FillDepth) {
                                                        *IsoDrawData.DestPtr = (unsigned short)IsoDrawData.FogColor + (IsoHalfbrightMask & (*IsoDrawData.DestPtr >> 1));
                                                    }
                                                    *IsoDrawData.DepthPtr = 0;
                                                    ++IsoDrawData.SrcPixel;
                                                    ++IsoDrawData.DestPtr;
                                                    ++IsoDrawData.DepthPtr;
                                                    IsoDrawData.DepthPtr = DepthBuffer->Wrap_Pointer(IsoDrawData.DepthPtr);
                                                    --run;
                                                } while (run != 0);
                                            }
                                            IsoDrawData.DestPtr = (unsigned short *)((char *)IsoDrawData.RowDest + IsoDrawData.SurfacePitch);
                                            IsoDrawData.DepthPtr = &IsoDrawData.RowDepth[IsoDrawData.DepthWidth];
                                            IsoDrawData.RowStartCol += ISO_DRAW_WIDTH;
                                            IsoDrawData.RowRunLength += ISO_DRAW_WIDTH;
                                        }
                                    }
                                } else {
                                    for (int row = 0; row < IsoDrawData.SpanHeight; ++row) {
                                        int offset = *IsoDrawData.RowSrcOffset;
                                        IsoDrawData.SrcPixel = (intptr_t)IsoDrawData.ImageBase + offset;
                                        IsoDrawData.SrcDepth = (intptr_t)IsoDrawData.DepthBase + offset;
                                        int start = *IsoDrawData.RowStartCol;
                                        IsoDrawData.RowDest = IsoDrawData.DestPtr;
                                        IsoDrawData.RowDepth = IsoDrawData.DepthPtr;
                                        IsoDrawData.RowAlpha = IsoDrawData.AlphaPtr;
                                        IsoDrawData.DestPtr += start;
                                        IsoDrawData.DepthPtr += start;
                                        IsoDrawData.DepthPtr = DepthBuffer->Wrap_Pointer(IsoDrawData.DepthPtr);
                                        IsoDrawData.AlphaPtr += start;
                                        IsoDrawData.AlphaPtr = AlphaBuffer->Wrap_Pointer(IsoDrawData.AlphaPtr);
                                        int run = *IsoDrawData.RowRunLength;
                                        if (run > 0) {
                                            do {
                                                unsigned short pixeldepth = (unsigned short)IsoDrawData.BaseDepth + *(unsigned char *)IsoDrawData.SrcDepth;
                                                if (*IsoDrawData.DepthPtr >= pixeldepth) {
                                                    *IsoDrawData.DepthPtr = pixeldepth;
                                                    *IsoDrawData.DestPtr = IsoDrawData.PixelTranslate[*(unsigned char *)IsoDrawData.SrcPixel | IsoDrawData.LightRemap[*IsoDrawData.AlphaPtr]];
                                                }
                                                ++IsoDrawData.SrcDepth;
                                                ++IsoDrawData.SrcPixel;
                                                ++IsoDrawData.DestPtr;
                                                ++IsoDrawData.DepthPtr;
                                                IsoDrawData.DepthPtr = DepthBuffer->Wrap_Pointer(IsoDrawData.DepthPtr);
                                                ++IsoDrawData.AlphaPtr;
                                                IsoDrawData.AlphaPtr = AlphaBuffer->Wrap_Pointer(IsoDrawData.AlphaPtr);
                                                --run;
                                            } while (run != 0);
                                        }
                                        IsoDrawData.DestPtr = (unsigned short *)((char *)IsoDrawData.RowDest + IsoDrawData.SurfacePitch);
                                        IsoDrawData.DepthPtr = &IsoDrawData.RowDepth[IsoDrawData.DepthWidth];
                                        IsoDrawData.AlphaPtr = &IsoDrawData.RowAlpha[IsoDrawData.AlphaWidth];
                                        IsoDrawData.RowSrcOffset += ISO_DRAW_WIDTH;
                                        IsoDrawData.RowStartCol += ISO_DRAW_WIDTH;
                                        IsoDrawData.RowRunLength += ISO_DRAW_WIDTH;
                                    }
                                }
                            } else if (!IsoTileUseAsmDrawFunc || depth_only || fill || fog) {
                                if (fill) {
                                    for (int row = 0; row < IsoDrawData.SpanHeight; ++row) {
                                        int start = *IsoDrawData.RowStartCol;
                                        IsoDrawData.RowDest = IsoDrawData.DestPtr;
                                        IsoDrawData.RowDepth = IsoDrawData.DepthPtr;
                                        IsoDrawData.DestPtr += start;
                                        IsoDrawData.DepthPtr += start;
                                        int run = *IsoDrawData.RowRunLength;
                                        if (run > 0) {
                                            do {
                                                *IsoDrawData.DepthPtr = IsoDrawData.FillDepth;
                                                *IsoDrawData.DestPtr = (unsigned short)IsoDrawData.FogColor;
                                                ++IsoDrawData.DestPtr;
                                                ++IsoDrawData.DepthPtr;
                                                --run;
                                            } while (run != 0);
                                        }
                                        IsoDrawData.DestPtr = (unsigned short *)((char *)IsoDrawData.RowDest + IsoDrawData.SurfacePitch);
                                        IsoDrawData.DepthPtr = &IsoDrawData.RowDepth[IsoDrawData.DepthWidth];
                                        IsoDrawData.RowStartCol += ISO_DRAW_WIDTH;
                                        IsoDrawData.RowRunLength += ISO_DRAW_WIDTH;
                                    }
                                } else if (depth_only) {
                                    for (int row = 0; row < IsoDrawData.SpanHeight; ++row) {
                                        int start = *IsoDrawData.RowStartCol;
                                        IsoDrawData.RowDepth = IsoDrawData.DepthPtr;
                                        IsoDrawData.DepthPtr += start;
                                        int run = *IsoDrawData.RowRunLength;
                                        if (run > 0) {
                                            do {
                                                *IsoDrawData.DepthPtr = IsoDrawData.FillDepth;
                                                ++IsoDrawData.DepthPtr;
                                                --run;
                                            } while (run != 0);
                                        }
                                        IsoDrawData.DepthPtr = &IsoDrawData.RowDepth[IsoDrawData.DepthWidth];
                                        IsoDrawData.RowStartCol += ISO_DRAW_WIDTH;
                                        IsoDrawData.RowRunLength += ISO_DRAW_WIDTH;
                                    }
                                } else if (fog && !Rules->BlendedFog) {
                                    for (int row = 0; row < IsoDrawData.SpanHeight; ++row) {
                                        IsoDrawData.SrcPixel = (intptr_t)IsoDrawData.ImageBase + *IsoDrawData.RowSrcOffset;
                                        int start = *IsoDrawData.RowStartCol;
                                        IsoDrawData.RowDest = IsoDrawData.DestPtr;
                                        IsoDrawData.RowDepth = IsoDrawData.DepthPtr;
                                        IsoDrawData.DestPtr += start;
                                        IsoDrawData.DepthPtr += start;
                                        int checker = ((unsigned char *)IsoDrawData.SrcPixel - image + row + IsoDrawData.ClipTop) & 1;
                                        int run = *IsoDrawData.RowRunLength;
                                        if (run > 0) {
                                            do {
                                                if (checker) {
                                                    *IsoDrawData.DestPtr = (unsigned short)IsoDrawData.FogColor;
                                                }
                                                *IsoDrawData.DepthPtr = 0;
                                                checker = checker == 0;
                                                ++IsoDrawData.SrcPixel;
                                                ++IsoDrawData.DestPtr;
                                                ++IsoDrawData.DepthPtr;
                                                --run;
                                            } while (run != 0);
                                        }
                                        IsoDrawData.DestPtr = (unsigned short *)((char *)IsoDrawData.RowDest + IsoDrawData.SurfacePitch);
                                        IsoDrawData.DepthPtr = &IsoDrawData.RowDepth[IsoDrawData.DepthWidth];
                                        IsoDrawData.RowStartCol += ISO_DRAW_WIDTH;
                                        IsoDrawData.RowRunLength += ISO_DRAW_WIDTH;
                                    }
                                } else if (fog) {
                                    for (int row = 0; row < IsoDrawData.SpanHeight; ++row) {
                                        IsoDrawData.SrcPixel = (intptr_t)IsoDrawData.ImageBase + *IsoDrawData.RowSrcOffset;
                                        int start = *IsoDrawData.RowStartCol;
                                        IsoDrawData.RowDest = IsoDrawData.DestPtr;
                                        IsoDrawData.RowDepth = IsoDrawData.DepthPtr;
                                        IsoDrawData.DestPtr += start;
                                        IsoDrawData.DepthPtr += start;
                                        int run = *IsoDrawData.RowRunLength;
                                        if (run > 0) {
                                            do {
                                                if (*IsoDrawData.DepthPtr > (int)IsoDrawData.FillDepth) {
                                                    *IsoDrawData.DestPtr = (unsigned short)IsoDrawData.FogColor + (IsoHalfbrightMask & (*IsoDrawData.DestPtr >> 1));
                                                }
                                                *IsoDrawData.DepthPtr = 0;
                                                ++IsoDrawData.SrcPixel;
                                                ++IsoDrawData.DestPtr;
                                                ++IsoDrawData.DepthPtr;
                                                --run;
                                            } while (run != 0);
                                        }
                                        IsoDrawData.DestPtr = (unsigned short *)((char *)IsoDrawData.RowDest + IsoDrawData.SurfacePitch);
                                        IsoDrawData.DepthPtr = &IsoDrawData.RowDepth[IsoDrawData.DepthWidth];
                                        IsoDrawData.RowStartCol += ISO_DRAW_WIDTH;
                                        IsoDrawData.RowRunLength += ISO_DRAW_WIDTH;
                                    }
                                } else {
                                    for (int row = 0; row < IsoDrawData.SpanHeight; ++row) {
                                        int offset = *IsoDrawData.RowSrcOffset;
                                        IsoDrawData.SrcPixel = (intptr_t)IsoDrawData.ImageBase + offset;
                                        IsoDrawData.SrcDepth = (intptr_t)IsoDrawData.DepthBase + offset;
                                        int start = *IsoDrawData.RowStartCol;
                                        IsoDrawData.RowDest = IsoDrawData.DestPtr;
                                        IsoDrawData.RowDepth = IsoDrawData.DepthPtr;
                                        IsoDrawData.RowAlpha = IsoDrawData.AlphaPtr;
                                        IsoDrawData.DestPtr += start;
                                        IsoDrawData.DepthPtr += start;
                                        IsoDrawData.AlphaPtr += start;
                                        int run = *IsoDrawData.RowRunLength;
                                        if (run > 0) {
                                            do {
                                                unsigned short pixeldepth = (unsigned short)IsoDrawData.BaseDepth + *(unsigned char *)IsoDrawData.SrcDepth;
                                                if (*IsoDrawData.DepthPtr >= pixeldepth) {
                                                    *IsoDrawData.DepthPtr = pixeldepth;
                                                    *IsoDrawData.DestPtr = IsoDrawData.PixelTranslate[*(unsigned char *)IsoDrawData.SrcPixel | IsoDrawData.LightRemap[*IsoDrawData.AlphaPtr]];
                                                }
                                                ++IsoDrawData.SrcDepth;
                                                ++IsoDrawData.SrcPixel;
                                                ++IsoDrawData.DestPtr;
                                                ++IsoDrawData.DepthPtr;
                                                ++IsoDrawData.AlphaPtr;
                                                --run;
                                            } while (run != 0);
                                        }
                                        IsoDrawData.DestPtr = (unsigned short *)((char *)IsoDrawData.RowDest + IsoDrawData.SurfacePitch);
                                        IsoDrawData.DepthPtr = &IsoDrawData.RowDepth[IsoDrawData.DepthWidth];
                                        IsoDrawData.AlphaPtr = &IsoDrawData.RowAlpha[IsoDrawData.AlphaWidth];
                                        IsoDrawData.RowSrcOffset += ISO_DRAW_WIDTH;
                                        IsoDrawData.RowStartCol += ISO_DRAW_WIDTH;
                                        IsoDrawData.RowRunLength += ISO_DRAW_WIDTH;
                                    }
                                }
                            }
                        } else if ((uintptr_t)&IsoDrawData.AlphaPtr[IsoDrawData.SpanWidth + 2 + IsoDrawData.SpanHeight * AlphaBuffer->Width] >= (uintptr_t)AlphaBuffer->BufferTail) {
                            for (int row = 0; row < IsoDrawData.SpanHeight; ++row) {
                                IsoDrawData.SrcPixel = (intptr_t)IsoDrawData.ImageBase + *IsoDrawData.RowSrcOffset;
                                int start = *IsoDrawData.RowStartCol;
                                IsoDrawData.RowDest = IsoDrawData.DestPtr;
                                IsoDrawData.RowAlpha = IsoDrawData.AlphaPtr;
                                IsoDrawData.DestPtr += start;
                                IsoDrawData.AlphaPtr += start;
                                IsoDrawData.AlphaPtr = AlphaBuffer->Wrap_Pointer(IsoDrawData.AlphaPtr);
                                int run = *IsoDrawData.RowRunLength;
                                if (run > 0) {
                                    do {
                                        *IsoDrawData.DestPtr = IsoDrawData.PixelTranslate[*(unsigned char *)IsoDrawData.SrcPixel | IsoDrawData.LightRemap[*IsoDrawData.AlphaPtr]];
                                        ++IsoDrawData.DestPtr;
                                        ++IsoDrawData.SrcPixel;
                                        ++IsoDrawData.AlphaPtr;
                                        IsoDrawData.AlphaPtr = AlphaBuffer->Wrap_Pointer(IsoDrawData.AlphaPtr);
                                        --run;
                                    } while (run != 0);
                                }
                                IsoDrawData.DestPtr = (unsigned short *)((char *)IsoDrawData.RowDest + IsoDrawData.SurfacePitch);
                                IsoDrawData.AlphaPtr = &IsoDrawData.RowAlpha[IsoDrawData.AlphaWidth];
                                IsoDrawData.RowSrcOffset += ISO_DRAW_WIDTH;
                                IsoDrawData.RowStartCol += ISO_DRAW_WIDTH;
                                IsoDrawData.RowRunLength += ISO_DRAW_WIDTH;
                            }
                        } else {
                            for (int row = 0; row < IsoDrawData.SpanHeight; ++row) {
                                IsoDrawData.SrcPixel = (intptr_t)IsoDrawData.ImageBase + *IsoDrawData.RowSrcOffset;
                                int start = *IsoDrawData.RowStartCol;
                                IsoDrawData.RowDest = IsoDrawData.DestPtr;
                                IsoDrawData.RowAlpha = IsoDrawData.AlphaPtr;
                                IsoDrawData.DestPtr += start;
                                IsoDrawData.AlphaPtr += start;
                                int run = *IsoDrawData.RowRunLength;
                                if (run > 0) {
                                    do {
                                        *IsoDrawData.DestPtr = IsoDrawData.PixelTranslate[*(unsigned char *)IsoDrawData.SrcPixel | IsoDrawData.LightRemap[*IsoDrawData.AlphaPtr]];
                                        ++IsoDrawData.DestPtr;
                                        ++IsoDrawData.SrcPixel;
                                        ++IsoDrawData.AlphaPtr;
                                        --run;
                                    } while (run != 0);
                                }
                                IsoDrawData.DestPtr = (unsigned short *)((char *)IsoDrawData.RowDest + IsoDrawData.SurfacePitch);
                                IsoDrawData.AlphaPtr = &IsoDrawData.RowAlpha[IsoDrawData.AlphaWidth];
                                IsoDrawData.RowSrcOffset += ISO_DRAW_WIDTH;
                                IsoDrawData.RowStartCol += ISO_DRAW_WIDTH;
                                IsoDrawData.RowRunLength += ISO_DRAW_WIDTH;
                            }
                        }
                        surface.Unlock();
                    }
                }
                if (!skip_extra) {
                    if (record->IsHasExtraData && !fill && !depth_only && !fog) {
                        int ex = record->ExtraX - record->X + x;
                        int ey = record->ExtraY - record->Y + y;
                        IsoDrawData.SpanWidth = record->ExtraWidth;
                        IsoDrawData.SpanHeight = record->ExtraHeight;
                        IsoDrawData.ClipLeft = 0;
                        IsoDrawData.ClipTop = 0;
                        bool extra_ok = true;
                        if (ex < cliprect.X) {
                            IsoDrawData.SpanWidth = ex - cliprect.X + record->ExtraWidth;
                            IsoDrawData.ClipLeft = cliprect.X - ex;
                            ex = cliprect.X;
                            if (IsoDrawData.SpanWidth <= 0) {
                                extra_ok = false;
                            }
                        }
                        if (extra_ok && ey < cliprect.Y) {
                            IsoDrawData.SpanHeight = ey - cliprect.Y + IsoDrawData.SpanHeight;
                            IsoDrawData.ClipTop = cliprect.Y - ey;
                            ey = cliprect.Y;
                            if (IsoDrawData.SpanHeight <= 0) {
                                extra_ok = false;
                            }
                        }
                        if (extra_ok && (int)IsoDrawData.SpanWidth + ex > clip_right) {
                            IsoDrawData.SpanWidth = cliprect.Width - ex + cliprect.X;
                            if (IsoDrawData.SpanWidth <= 0) {
                                extra_ok = false;
                            }
                        }
                        if (extra_ok && IsoDrawData.SpanHeight + ey > clip_bottom) {
                            IsoDrawData.SpanHeight = cliprect.Height - ey + cliprect.Y;
                            if (IsoDrawData.SpanHeight <= 0) {
                                extra_ok = false;
                            }
                        }
                        if (extra_ok) {
                            IsoDrawData.ImageRowStep = record->ExtraWidth - IsoDrawData.SpanWidth;
                            IsoDrawData.SurfacePitch = surface.Stride() - IsoDrawData.SpanWidth * surface.Bytes_Per_Pixel();
                            if (use_z) {
                                IsoDrawData.DepthPtr = (unsigned short *)DepthBuffer->Get_Buffer_Offset(Point2D(ex, ey - unknown_rect_839630.Y));
                                IsoDrawData.DepthWidth = DepthBuffer->Width - IsoDrawData.SpanWidth;
                            }
                            IsoDrawData.AlphaPtr = (unsigned short *)AlphaBuffer->Get_Buffer_Offset(Point2D(ex, ey - unknown_rect_839630.Y));
                            IsoDrawData.AlphaWidth = AlphaBuffer->Width - IsoDrawData.SpanWidth;
                            IsoDrawData.DestPtr = (unsigned short *)surface.Lock(Point2D(ex, ey));
                            if (IsoDrawData.DestPtr != NULL) {
                                IsoDrawData.SrcPixel = (intptr_t)record + record->ExtraOffset + IsoDrawData.ClipLeft;
                                IsoDrawData.SrcPixel = IsoDrawData.ClipTop * record->ExtraWidth + IsoDrawData.SrcPixel;
                                if (use_z) {
                                    IsoDrawData.SrcDepth = (intptr_t)record + record->ExtraZOffset + IsoDrawData.ClipLeft;
                                    IsoDrawData.SrcDepth = IsoDrawData.ClipTop * record->ExtraWidth + IsoDrawData.SrcDepth;
                                    if ((uintptr_t)&IsoDrawData.DepthPtr[IsoDrawData.SpanWidth + 2 + IsoDrawData.SpanHeight * DepthBuffer->Width] >= (uintptr_t)DepthBuffer->BufferTail) {
                                        /*
                                         * Depth-tested extra image, Z buffer wrapping.
                                         */
                                        for (int row = 0; row < IsoDrawData.SpanHeight; ++row) {
                                            for (int col = 0; col < (int)IsoDrawData.SpanWidth; ++col) {
                                                if (*(unsigned char *)IsoDrawData.SrcPixel) {
                                                    if (*IsoDrawData.DepthPtr >= (unsigned short)((unsigned short)IsoDrawData.BaseDepth + *(unsigned char *)IsoDrawData.SrcDepth)) {
                                                        *IsoDrawData.DestPtr = IsoDrawData.PixelTranslate[*(unsigned char *)IsoDrawData.SrcPixel | IsoDrawData.LightRemap[*IsoDrawData.AlphaPtr]];
                                                        *IsoDrawData.DepthPtr = (unsigned short)IsoDrawData.BaseDepth + *(unsigned char *)IsoDrawData.SrcDepth;
                                                    }
                                                }
                                                ++IsoDrawData.DestPtr;
                                                ++IsoDrawData.SrcPixel;
                                                ++IsoDrawData.SrcDepth;
                                                ++IsoDrawData.DepthPtr;
                                                IsoDrawData.DepthPtr = DepthBuffer->Wrap_Pointer(IsoDrawData.DepthPtr);
                                                ++IsoDrawData.AlphaPtr;
                                                IsoDrawData.AlphaPtr = AlphaBuffer->Wrap_Pointer(IsoDrawData.AlphaPtr);
                                            }
                                            IsoDrawData.DestPtr = (unsigned short *)((char *)IsoDrawData.DestPtr + IsoDrawData.SurfacePitch);
                                            IsoDrawData.SrcPixel += IsoDrawData.ImageRowStep;
                                            IsoDrawData.SrcDepth += IsoDrawData.ImageRowStep;
                                            IsoDrawData.DepthPtr = &IsoDrawData.DepthPtr[IsoDrawData.DepthWidth];
                                            IsoDrawData.DepthPtr = DepthBuffer->Wrap_Pointer(IsoDrawData.DepthPtr);
                                            IsoDrawData.AlphaPtr = &IsoDrawData.AlphaPtr[IsoDrawData.AlphaWidth];
                                            IsoDrawData.AlphaPtr = AlphaBuffer->Wrap_Pointer(IsoDrawData.AlphaPtr);
                                        }
                                    } else if (IsoTileUseAsmDrawFunc) {
                                        Iso_Blit_Asm2((IsoBlitState *)&IsoDrawData);
                                    } else {
                                        /*
                                         * Depth-tested extra image, no wrapping.
                                         */
                                        for (int row = 0; row < IsoDrawData.SpanHeight; ++row) {
                                            for (int col = 0; col < (int)IsoDrawData.SpanWidth; ++col) {
                                                if (*(unsigned char *)IsoDrawData.SrcPixel) {
                                                    if (*IsoDrawData.DepthPtr >= (unsigned short)((unsigned short)IsoDrawData.BaseDepth + *(unsigned char *)IsoDrawData.SrcDepth)) {
                                                        *IsoDrawData.DestPtr = IsoDrawData.PixelTranslate[*(unsigned char *)IsoDrawData.SrcPixel | IsoDrawData.LightRemap[*IsoDrawData.AlphaPtr]];
                                                        *IsoDrawData.DepthPtr = (unsigned short)IsoDrawData.BaseDepth + *(unsigned char *)IsoDrawData.SrcDepth;
                                                    }
                                                }
                                                ++IsoDrawData.DestPtr;
                                                ++IsoDrawData.SrcPixel;
                                                ++IsoDrawData.SrcDepth;
                                                ++IsoDrawData.DepthPtr;
                                                ++IsoDrawData.AlphaPtr;
                                            }
                                            IsoDrawData.DestPtr = (unsigned short *)((char *)IsoDrawData.DestPtr + IsoDrawData.SurfacePitch);
                                            IsoDrawData.SrcPixel += IsoDrawData.ImageRowStep;
                                            IsoDrawData.SrcDepth += IsoDrawData.ImageRowStep;
                                            IsoDrawData.DepthPtr = &IsoDrawData.DepthPtr[IsoDrawData.DepthWidth];
                                            IsoDrawData.AlphaPtr = &IsoDrawData.AlphaPtr[IsoDrawData.AlphaWidth];
                                        }
                                    }
                                } else if ((uintptr_t)&IsoDrawData.AlphaPtr[IsoDrawData.SpanWidth + 2 + IsoDrawData.SpanHeight * AlphaBuffer->Width] >= (uintptr_t)AlphaBuffer->BufferTail) {
                                    /*
                                     * Plain extra image (no depth), alpha buffer wrapping.
                                     */
                                    for (int row = 0; row < IsoDrawData.SpanHeight; ++row) {
                                        for (int col = 0; col < (int)IsoDrawData.SpanWidth; ++col) {
                                            if (*(unsigned char *)IsoDrawData.SrcPixel) {
                                                *IsoDrawData.DestPtr = IsoDrawData.PixelTranslate[*(unsigned char *)IsoDrawData.SrcPixel | IsoDrawData.LightRemap[*IsoDrawData.AlphaPtr]];
                                            }
                                            ++IsoDrawData.DestPtr;
                                            ++IsoDrawData.AlphaPtr;
                                            IsoDrawData.AlphaPtr = AlphaBuffer->Wrap_Pointer(IsoDrawData.AlphaPtr);
                                            ++IsoDrawData.SrcPixel;
                                        }
                                        IsoDrawData.DestPtr = (unsigned short *)((char *)IsoDrawData.DestPtr + IsoDrawData.SurfacePitch);
                                        IsoDrawData.AlphaPtr = &IsoDrawData.AlphaPtr[IsoDrawData.AlphaWidth];
                                        IsoDrawData.AlphaPtr = AlphaBuffer->Wrap_Pointer(IsoDrawData.AlphaPtr);
                                        IsoDrawData.SrcPixel += IsoDrawData.ImageRowStep;
                                    }
                                } else {
                                    /*
                                     * Plain extra image (no depth), no wrapping.
                                     */
                                    for (int row = 0; row < IsoDrawData.SpanHeight; ++row) {
                                        for (int col = 0; col < (int)IsoDrawData.SpanWidth; ++col) {
                                            if (*(unsigned char *)IsoDrawData.SrcPixel) {
                                                *IsoDrawData.DestPtr = IsoDrawData.PixelTranslate[*(unsigned char *)IsoDrawData.SrcPixel | IsoDrawData.LightRemap[*IsoDrawData.AlphaPtr]];
                                            }
                                            ++IsoDrawData.DestPtr;
                                            ++IsoDrawData.SrcPixel;
                                            ++IsoDrawData.AlphaPtr;
                                        }
                                        IsoDrawData.AlphaPtr = &IsoDrawData.AlphaPtr[IsoDrawData.AlphaWidth];
                                        IsoDrawData.DestPtr = (unsigned short *)((char *)IsoDrawData.DestPtr + IsoDrawData.SurfacePitch);
                                        IsoDrawData.SrcPixel += IsoDrawData.ImageRowStep;
                                    }
                                }
                                surface.Unlock();
                            }
                        }
                    }
                }
            }
        }
        AlphaTableCache_008315B8.Release(aremap);
    }
}

// NATIVE: the callers' view of entry 0x0052C230 (shared/isometric_tile_type.hpp, CellClass::unknown_4754b0)
// forwards to the draft's TibSun spelling. VC6 passed each bool in a 32-bit slot and the body reads its low
// byte; the surface reference and pointer are the same pointer.
void IsometricTileTypeClass::unknown_func_52c230(LightConvertClass *drawer, int arg2, Surface *surface, int x, int y,
    Rect rect, int arg7, int arg8, int arg9, int arg10, int arg11, int arg12, int arg13, int arg14)
{
    Draw_Tile(drawer, arg2, *surface, x, y, rect, arg7, arg8, (unsigned char)arg9 != 0, arg10, (unsigned char)arg11 != 0,
        (unsigned char)arg12 != 0, (unsigned char)arg13 != 0, arg14);
}
