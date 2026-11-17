
// ---------------------------------------------------------------------------------------------
// NON-EXACT NATIVE DRAFT (lane Y3): IsometricTileTypeClass::Draw_Tile (YR 0x00547CF0, 6,967
// bytes) is unmatched in the YR tree. The text is the RA2 tree's native draft of its twin
// (native/drafts/0052c230.cpp, RA2 0x0052C230: TibSun's Draw_Tile rewritten to RA2's 60x30
// tiles, with its native pointer-width changes), here with the YR tree's names: TibSun's
// AlphaLightingRemapInit / IntensityTranslator / Rule->IsBlendedFog / NextTileTypeInSet /
// DSurface::Get_Halfbright_Mask, the YR IsoBlitState's HalfbrightMask, TacticalRect, and the
// pointer-width ABuffer/ZBuffer (Get_Buffer_End, Wrap_Overflow). Source: OpenTS / TibSun archive
// code/isotype.cpp (SPDX GPL-3.0-or-later, Copyright 2025 Electronic Arts Inc. and OpenTS
// contributors, EA's GPL section 7 terms; THIRD_PARTY.md). Behaviour code for the native build.
// ---------------------------------------------------------------------------------------------
#include "alphalighting.h"
#include "convert.h"

#define ISO60_DRAW_W 60     // RA2/YR tiles: 60 pixels wide,
#define ISO60_DRAW_H 29     // a 29-line diamond,
#define ISO60_H 30          // 30 row bases
#ifndef LOWORD
#define LOWORD(l) ((unsigned short)(l))
#endif

#pragma pack(push, 1)
struct IsoBlitState {
	intptr_t SrcPixel;   // NATIVE: holds a pointer (was int)
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
	unsigned short HalfbrightMask;
};
#pragma pack(pop)

IsoBlitState IsoDrawData;
unsigned short _iso_row_offsets[ISO60_DRAW_W*ISO60_DRAW_H];
unsigned char _iso_start_cols[ISO60_DRAW_W*ISO60_DRAW_H];
unsigned char _iso_run_lengths[ISO60_DRAW_W][ISO60_DRAW_W*ISO60_DRAW_H];
char IsoSpanTablesBuilt;
char IsoTileUseAsmDrawFunc;     // never set (TibSun): the C++ loops draw
extern "C" void __cdecl Iso_Blit_Asm2(IsoBlitState *state);

void IsometricTileTypeClass::Draw_Tile(LightConvertClass * drawer, int subtile, Surface & surface, int x, int y, Rect cliprect, int height, int brightness, bool use_z, int cell_variation, bool fill, bool depth_only, bool fog, signed int fog_color) const
{
    static int const _iso_row_bases[ISO60_H] = {
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
        IsoDrawData.HalfbrightMask = DSurface::Get_Halfbright_Mask();
        if (cell_variation != 0) {
            int num = NumTileTypesInSet;
            if (cell_variation > num - 1) {
                cell_variation %= num;
            }
            if (cell_variation != 0) {
                IsometricTileTypeClass const * next = this;
                do {
                    next = next->NextTileTypeInSet;
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
        AlphaLightingRemapClass * aremap = AlphaLightingRemapInit.Init(drawer->IntensityLevels);   // NATIVE: YR's names (TibSun)
        IsoDrawData.LightRemap = aremap->Get_Table(brightness);
        IsoDrawData.PixelTranslate = (unsigned short *)drawer->IntensityTranslator;
        /*
         * One-time build of the iso-diamond span tables. For each of the ISO60_DRAW_H
         * scanlines of the ISO60_DRAW_W-wide tile diamond record the left column of the
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
                for (first_col = 0; first_col < ISO60_DRAW_W; ++first_col) {
                    if (_tilemask[cell_offset + first_col] != 32) {
                        span_count = 1;
                        while (first_col + span_count < ISO60_DRAW_W && _tilemask[cell_offset + first_col + span_count] != 32) {
                            ++span_count;
                        }
                        break;
                    }
                }
                if (first_col >= ISO60_DRAW_W) {
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
                    } while (col < ISO60_DRAW_W);
                }
                {
                    unsigned char * runp = &_iso_run_lengths[ISO60_DRAW_W-1][cell_offset];
                    int left_clip = span_count + first_col - ISO60_DRAW_W;
                    for (int rowcount = ISO60_DRAW_W; rowcount != 0; --rowcount) {
                        for (int i = 0; i < ISO60_DRAW_W; ++i) {
                            int run = span_count - max(0, i - first_col) - max(0, left_clip);
                            runp[i] = max(0, run);
                        }
                        runp -= (ISO60_DRAW_W*ISO60_DRAW_H);
                        ++left_clip;
                    }
                }
                cell_offset += ISO60_DRAW_W;
                loop_pos += (ISO60_DRAW_W * 2);
                ++lut;
            } while (loop_pos < ((ISO60_DRAW_W*ISO60_DRAW_H) *2));
            IsoSpanTablesBuilt = 1;
        }
        if (surface.Bytes_Per_Pixel() == 2) {
            const IsoTileSet * set = (const IsoTileSet *)tileptr->Get_Image_Data();
            if (set != NULL) {
                if (use_z) {
                    IsoDrawData.BaseDepth = (unsigned short)(LOWORD(DepthBuffer->Bounds.Y) + LOWORD(DepthBuffer->ScrollOffset) - y - LOWORD(set->Height));   // NATIVE: TibSun's names for the RA2 draft's MaxValue/Area
                    IsoDrawData.BaseDepth += height * set->Height / -2;   // NATIVE: the same sum as integers (the draft formed it through a pointer; Scout1246)
                }
                const IsoTileRecord * record = set->Fetch_Record_Pointer(subtile);
                if (record == NULL) {
                    return;
                }
                int spanw = ISO60_DRAW_W;
                int spanh = ISO60_DRAW_H;
                IsoDrawData.SpanWidth = spanw;
                IsoDrawData.SpanHeight = spanh;
                IsoDrawData.ClipLeft = 0;
                IsoDrawData.ClipTop = 0;
                IsoDrawData.RowSrcOffset = _iso_row_offsets;
                IsoDrawData.RowStartCol = (unsigned char *)_iso_start_cols;
                IsoDrawData.RowRunLength = (unsigned char *)_iso_run_lengths;
                /*
                 * Clip the ISO60_DRAW_WxISO60_DRAW_H tile diamond against the clip rectangle, adjusting
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
                int right_clip = ISO60_DRAW_W-1;
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
                        if (clip_x > ISO60_DRAW_W-1) {
                            clip_x = ISO60_DRAW_W-1;
                            IsoDrawData.ClipLeft = clip_x;
                        }
                        if (clip_y > ISO60_DRAW_H-1) {
                            clip_y = ISO60_DRAW_H-1;
                            IsoDrawData.ClipTop = clip_y;
                        }
                        int table_off = clip_x + ISO60_DRAW_W * clip_y;
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
                            IsoDrawData.DepthPtr = (unsigned short *)DepthBuffer->Get_Buffer_Offset(Point2D(lock_x, work_y - TacticalRect.Y));
                            IsoDrawData.DepthWidth = DepthBuffer->Get_Buffer_Width();
                        }
                        IsoDrawData.AlphaPtr = (unsigned short *)AlphaBuffer->Get_Buffer_Offset(Point2D(lock_x, work_y - TacticalRect.Y));
                        IsoDrawData.AlphaWidth = AlphaBuffer->Get_Buffer_Width();
                        IsoDrawData.SrcPixel = (intptr_t)(record + 1);
                        unsigned char const * image = (unsigned char const *)IsoDrawData.SrcPixel;
                        if (fill || fog) {
                            IsoDrawData.FillDepth = 0;
                        } else {
                            IsoDrawData.FillDepth = 0xFFFF;
                        }
                        IsoDrawData.FogColor = IsoDrawData.HalfbrightMask & (fog_color >> 1);
                        if (use_z && record->IsHasZData) {
                            IsoDrawData.SrcDepth = (intptr_t)record + record->ZDataOffset;
                        }
                        IsoDrawData.ImageBase = (unsigned char *)(record + 1);
                        IsoDrawData.DepthBase = (unsigned char *)IsoDrawData.SrcDepth;
                        if (use_z) {
                            if ((uintptr_t)&IsoDrawData.DepthPtr[IsoDrawData.SpanWidth + 2 + IsoDrawData.SpanHeight * DepthBuffer->Get_Buffer_Width()] >= DepthBuffer->Get_Buffer_End()) {
                                if (fill) {
                                    for (int row = 0; row < IsoDrawData.SpanHeight; ++row) {
                                        int start = *IsoDrawData.RowStartCol;
                                        IsoDrawData.RowDest = IsoDrawData.DestPtr;
                                        IsoDrawData.RowDepth = IsoDrawData.DepthPtr;
                                        IsoDrawData.DestPtr += start;
                                        IsoDrawData.DepthPtr += start;
                                        IsoDrawData.DepthPtr = (unsigned short *)DepthBuffer->Wrap_Overflow((uintptr_t)IsoDrawData.DepthPtr);
                                        int run = *IsoDrawData.RowRunLength;
                                        if (run > 0) {
                                            do {
                                                *IsoDrawData.DepthPtr = IsoDrawData.FillDepth;
                                                *IsoDrawData.DestPtr = (unsigned short)IsoDrawData.FogColor;
                                                ++IsoDrawData.DestPtr;
                                                ++IsoDrawData.DepthPtr;
                                                IsoDrawData.DepthPtr = (unsigned short *)DepthBuffer->Wrap_Overflow((uintptr_t)IsoDrawData.DepthPtr);
                                                --run;
                                            } while (run != 0);
                                        }
                                        IsoDrawData.DestPtr = (unsigned short *)((char *)IsoDrawData.RowDest + IsoDrawData.SurfacePitch);
                                        IsoDrawData.DepthPtr = &IsoDrawData.RowDepth[IsoDrawData.DepthWidth];
                                        IsoDrawData.RowStartCol += ISO60_DRAW_W;
                                        IsoDrawData.RowRunLength += ISO60_DRAW_W;
                                    }
                                } else if (depth_only) {
                                    for (int row = 0; row < IsoDrawData.SpanHeight; ++row) {
                                        int start = *IsoDrawData.RowStartCol;
                                        IsoDrawData.RowDepth = IsoDrawData.DepthPtr;
                                        IsoDrawData.DepthPtr += start;
                                        IsoDrawData.DepthPtr = (unsigned short *)DepthBuffer->Wrap_Overflow((uintptr_t)IsoDrawData.DepthPtr);
                                        int run = *IsoDrawData.RowRunLength;
                                        if (run > 0) {
                                            do {
                                                *IsoDrawData.DepthPtr = IsoDrawData.FillDepth;
                                                ++IsoDrawData.DepthPtr;
                                                IsoDrawData.DepthPtr = (unsigned short *)DepthBuffer->Wrap_Overflow((uintptr_t)IsoDrawData.DepthPtr);
                                                --run;
                                            } while (run != 0);
                                        }
                                        IsoDrawData.DepthPtr = &IsoDrawData.RowDepth[IsoDrawData.DepthWidth];
                                        IsoDrawData.RowStartCol += ISO60_DRAW_W;
                                        IsoDrawData.RowRunLength += ISO60_DRAW_W;
                                    }
                                } else if (fog) {
                                    if (!Rule->IsBlendedFog) {
                                        for (int row = 0; row < IsoDrawData.SpanHeight; ++row) {
                                            IsoDrawData.SrcPixel = (intptr_t)IsoDrawData.ImageBase + *IsoDrawData.RowSrcOffset;
                                            int start = *IsoDrawData.RowStartCol;
                                            IsoDrawData.RowDest = IsoDrawData.DestPtr;
                                            IsoDrawData.RowDepth = IsoDrawData.DepthPtr;
                                            IsoDrawData.DestPtr += start;
                                            IsoDrawData.DepthPtr += start;
                                            IsoDrawData.DepthPtr = (unsigned short *)DepthBuffer->Wrap_Overflow((uintptr_t)IsoDrawData.DepthPtr);
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
                                                    IsoDrawData.DepthPtr = (unsigned short *)DepthBuffer->Wrap_Overflow((uintptr_t)IsoDrawData.DepthPtr);
                                                    --run;
                                                } while (run != 0);
                                            }
                                            IsoDrawData.DestPtr = (unsigned short *)((char *)IsoDrawData.RowDest + IsoDrawData.SurfacePitch);
                                            IsoDrawData.DepthPtr = &IsoDrawData.RowDepth[IsoDrawData.DepthWidth];
                                            IsoDrawData.RowStartCol += ISO60_DRAW_W;
                                            IsoDrawData.RowRunLength += ISO60_DRAW_W;
                                        }
                                    } else {
                                        for (int row = 0; row < IsoDrawData.SpanHeight; ++row) {
                                            IsoDrawData.SrcPixel = (intptr_t)IsoDrawData.ImageBase + *IsoDrawData.RowSrcOffset;
                                            int start = *IsoDrawData.RowStartCol;
                                            IsoDrawData.RowDest = IsoDrawData.DestPtr;
                                            IsoDrawData.RowDepth = IsoDrawData.DepthPtr;
                                            IsoDrawData.DestPtr += start;
                                            IsoDrawData.DepthPtr += start;
                                            IsoDrawData.DepthPtr = (unsigned short *)DepthBuffer->Wrap_Overflow((uintptr_t)IsoDrawData.DepthPtr);
                                            int run = *IsoDrawData.RowRunLength;
                                            if (run > 0) {
                                                do {
                                                    if (*IsoDrawData.DepthPtr > (int)IsoDrawData.FillDepth) {
                                                        *IsoDrawData.DestPtr = (unsigned short)IsoDrawData.FogColor + (IsoDrawData.HalfbrightMask & (*IsoDrawData.DestPtr >> 1));
                                                    }
                                                    *IsoDrawData.DepthPtr = 0;
                                                    ++IsoDrawData.SrcPixel;
                                                    ++IsoDrawData.DestPtr;
                                                    ++IsoDrawData.DepthPtr;
                                                    IsoDrawData.DepthPtr = (unsigned short *)DepthBuffer->Wrap_Overflow((uintptr_t)IsoDrawData.DepthPtr);
                                                    --run;
                                                } while (run != 0);
                                            }
                                            IsoDrawData.DestPtr = (unsigned short *)((char *)IsoDrawData.RowDest + IsoDrawData.SurfacePitch);
                                            IsoDrawData.DepthPtr = &IsoDrawData.RowDepth[IsoDrawData.DepthWidth];
                                            IsoDrawData.RowStartCol += ISO60_DRAW_W;
                                            IsoDrawData.RowRunLength += ISO60_DRAW_W;
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
                                        IsoDrawData.DepthPtr = (unsigned short *)DepthBuffer->Wrap_Overflow((uintptr_t)IsoDrawData.DepthPtr);
                                        IsoDrawData.AlphaPtr += start;
                                        IsoDrawData.AlphaPtr = (unsigned short *)AlphaBuffer->Wrap_Overflow((uintptr_t)IsoDrawData.AlphaPtr);
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
                                                IsoDrawData.DepthPtr = (unsigned short *)DepthBuffer->Wrap_Overflow((uintptr_t)IsoDrawData.DepthPtr);
                                                ++IsoDrawData.AlphaPtr;
                                                IsoDrawData.AlphaPtr = (unsigned short *)AlphaBuffer->Wrap_Overflow((uintptr_t)IsoDrawData.AlphaPtr);
                                                --run;
                                            } while (run != 0);
                                        }
                                        IsoDrawData.DestPtr = (unsigned short *)((char *)IsoDrawData.RowDest + IsoDrawData.SurfacePitch);
                                        IsoDrawData.DepthPtr = &IsoDrawData.RowDepth[IsoDrawData.DepthWidth];
                                        IsoDrawData.AlphaPtr = &IsoDrawData.RowAlpha[IsoDrawData.AlphaWidth];
                                        IsoDrawData.RowSrcOffset += ISO60_DRAW_W;
                                        IsoDrawData.RowStartCol += ISO60_DRAW_W;
                                        IsoDrawData.RowRunLength += ISO60_DRAW_W;
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
                                        IsoDrawData.RowStartCol += ISO60_DRAW_W;
                                        IsoDrawData.RowRunLength += ISO60_DRAW_W;
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
                                        IsoDrawData.RowStartCol += ISO60_DRAW_W;
                                        IsoDrawData.RowRunLength += ISO60_DRAW_W;
                                    }
                                } else if (fog && !Rule->IsBlendedFog) {
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
                                        IsoDrawData.RowStartCol += ISO60_DRAW_W;
                                        IsoDrawData.RowRunLength += ISO60_DRAW_W;
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
                                                    *IsoDrawData.DestPtr = (unsigned short)IsoDrawData.FogColor + (IsoDrawData.HalfbrightMask & (*IsoDrawData.DestPtr >> 1));
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
                                        IsoDrawData.RowStartCol += ISO60_DRAW_W;
                                        IsoDrawData.RowRunLength += ISO60_DRAW_W;
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
                                        IsoDrawData.RowSrcOffset += ISO60_DRAW_W;
                                        IsoDrawData.RowStartCol += ISO60_DRAW_W;
                                        IsoDrawData.RowRunLength += ISO60_DRAW_W;
                                    }
                                }
                            }
                        } else if ((uintptr_t)&IsoDrawData.AlphaPtr[IsoDrawData.SpanWidth + 2 + IsoDrawData.SpanHeight * AlphaBuffer->Get_Buffer_Width()] >= AlphaBuffer->Get_Buffer_End()) {
                            for (int row = 0; row < IsoDrawData.SpanHeight; ++row) {
                                IsoDrawData.SrcPixel = (intptr_t)IsoDrawData.ImageBase + *IsoDrawData.RowSrcOffset;
                                int start = *IsoDrawData.RowStartCol;
                                IsoDrawData.RowDest = IsoDrawData.DestPtr;
                                IsoDrawData.RowAlpha = IsoDrawData.AlphaPtr;
                                IsoDrawData.DestPtr += start;
                                IsoDrawData.AlphaPtr += start;
                                IsoDrawData.AlphaPtr = (unsigned short *)AlphaBuffer->Wrap_Overflow((uintptr_t)IsoDrawData.AlphaPtr);
                                int run = *IsoDrawData.RowRunLength;
                                if (run > 0) {
                                    do {
                                        *IsoDrawData.DestPtr = IsoDrawData.PixelTranslate[*(unsigned char *)IsoDrawData.SrcPixel | IsoDrawData.LightRemap[*IsoDrawData.AlphaPtr]];
                                        ++IsoDrawData.DestPtr;
                                        ++IsoDrawData.SrcPixel;
                                        ++IsoDrawData.AlphaPtr;
                                        IsoDrawData.AlphaPtr = (unsigned short *)AlphaBuffer->Wrap_Overflow((uintptr_t)IsoDrawData.AlphaPtr);
                                        --run;
                                    } while (run != 0);
                                }
                                IsoDrawData.DestPtr = (unsigned short *)((char *)IsoDrawData.RowDest + IsoDrawData.SurfacePitch);
                                IsoDrawData.AlphaPtr = &IsoDrawData.RowAlpha[IsoDrawData.AlphaWidth];
                                IsoDrawData.RowSrcOffset += ISO60_DRAW_W;
                                IsoDrawData.RowStartCol += ISO60_DRAW_W;
                                IsoDrawData.RowRunLength += ISO60_DRAW_W;
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
                                IsoDrawData.RowSrcOffset += ISO60_DRAW_W;
                                IsoDrawData.RowStartCol += ISO60_DRAW_W;
                                IsoDrawData.RowRunLength += ISO60_DRAW_W;
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
                                IsoDrawData.DepthPtr = (unsigned short *)DepthBuffer->Get_Buffer_Offset(Point2D(ex, ey - TacticalRect.Y));
                                IsoDrawData.DepthWidth = DepthBuffer->Get_Buffer_Width() - IsoDrawData.SpanWidth;
                            }
                            IsoDrawData.AlphaPtr = (unsigned short *)AlphaBuffer->Get_Buffer_Offset(Point2D(ex, ey - TacticalRect.Y));
                            IsoDrawData.AlphaWidth = AlphaBuffer->Get_Buffer_Width() - IsoDrawData.SpanWidth;
                            IsoDrawData.DestPtr = (unsigned short *)surface.Lock(Point2D(ex, ey));
                            if (IsoDrawData.DestPtr != NULL) {
                                IsoDrawData.SrcPixel = (intptr_t)record + record->ExtraOffset + IsoDrawData.ClipLeft;
                                IsoDrawData.SrcPixel = IsoDrawData.ClipTop * record->ExtraWidth + IsoDrawData.SrcPixel;
                                if (use_z) {
                                    IsoDrawData.SrcDepth = (intptr_t)record + record->ExtraZOffset + IsoDrawData.ClipLeft;
                                    IsoDrawData.SrcDepth = IsoDrawData.ClipTop * record->ExtraWidth + IsoDrawData.SrcDepth;
                                    if ((uintptr_t)&IsoDrawData.DepthPtr[IsoDrawData.SpanWidth + 2 + IsoDrawData.SpanHeight * DepthBuffer->Get_Buffer_Width()] >= DepthBuffer->Get_Buffer_End()) {
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
                                                IsoDrawData.DepthPtr = (unsigned short *)DepthBuffer->Wrap_Overflow((uintptr_t)IsoDrawData.DepthPtr);
                                                ++IsoDrawData.AlphaPtr;
                                                IsoDrawData.AlphaPtr = (unsigned short *)AlphaBuffer->Wrap_Overflow((uintptr_t)IsoDrawData.AlphaPtr);
                                            }
                                            IsoDrawData.DestPtr = (unsigned short *)((char *)IsoDrawData.DestPtr + IsoDrawData.SurfacePitch);
                                            IsoDrawData.SrcPixel += IsoDrawData.ImageRowStep;
                                            IsoDrawData.SrcDepth += IsoDrawData.ImageRowStep;
                                            IsoDrawData.DepthPtr = &IsoDrawData.DepthPtr[IsoDrawData.DepthWidth];
                                            IsoDrawData.DepthPtr = (unsigned short *)DepthBuffer->Wrap_Overflow((uintptr_t)IsoDrawData.DepthPtr);
                                            IsoDrawData.AlphaPtr = &IsoDrawData.AlphaPtr[IsoDrawData.AlphaWidth];
                                            IsoDrawData.AlphaPtr = (unsigned short *)AlphaBuffer->Wrap_Overflow((uintptr_t)IsoDrawData.AlphaPtr);
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
                                } else if ((uintptr_t)&IsoDrawData.AlphaPtr[IsoDrawData.SpanWidth + 2 + IsoDrawData.SpanHeight * AlphaBuffer->Get_Buffer_Width()] >= AlphaBuffer->Get_Buffer_End()) {
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
                                            IsoDrawData.AlphaPtr = (unsigned short *)AlphaBuffer->Wrap_Overflow((uintptr_t)IsoDrawData.AlphaPtr);
                                            ++IsoDrawData.SrcPixel;
                                        }
                                        IsoDrawData.DestPtr = (unsigned short *)((char *)IsoDrawData.DestPtr + IsoDrawData.SurfacePitch);
                                        IsoDrawData.AlphaPtr = &IsoDrawData.AlphaPtr[IsoDrawData.AlphaWidth];
                                        IsoDrawData.AlphaPtr = (unsigned short *)AlphaBuffer->Wrap_Overflow((uintptr_t)IsoDrawData.AlphaPtr);
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
        AlphaLightingRemapInit.Deinit(aremap);   // NATIVE: TibSun's name
    }
}

