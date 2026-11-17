/*******************************************************************************
 *                                O P E N  T S
 *******************************************************************************
 * SPDX-License-Identifier: GPL-3.0-or-later
 * Copyright 2026 OpenTS contributors
 *
 * See LICENSE.md for applicable additional terms and warranty disclaimers.
 ******************************************************************************/

#include "always.h"
#include "geometry.h" // RA2: the header statics, in the order of the unit's initializer table
#include "rect.h"

#include "ovrlight.h"

#include "_map.h"
#include "_rect.h"
#include "_rules.h"
#include "_surface.h"
#include "_tactica.h"
#include "bsurface.h"
#include "dsurface.h"
#include "getcpu.h"
#include "globals.h"
#include "rules.h"
#include "scenario.h"
#include "surface.h"
#include "tactical.h"

namespace FPSCounter { extern unsigned CurrentFrameRate; } // RA2: declared as RA2's code uses it
namespace Detail { unsigned GetMinFrameRate(void); } // RA2: declared as RA2's code uses it

DynamicVectorClass<SpotLightClass *> SpotLights;

int SpotLightDontUseMMX = false;
int SpotLightColorMode = -1;
int *SpotLightMMXBuffer;
BSurface *SpotLightSurfaces[SpotLightClass::SPOTLIGHT_SURFACE_COUNT + SpotLightClass::SPOTLIGHT_EXTRA_SURFACE_COUNT];

extern "C" {
/*
 * Externs to assembly routines from winasm.asm
 */
void __cdecl Adjust_Color_565(void *pal1, void *pal2, int red, int green, int blue, int intensity, char *arg7);
void __cdecl Adjust_Color_555(void *pal1, void *pal2, int red, int green, int blue, int intensity, char *arg7);
void __cdecl Adjust_Color_556(void *pal1, void *pal2, int red, int green, int blue, int intensity, char *arg7);
void __cdecl Adjust_Color_655(void *pal1, void *pal2, int red, int green, int blue, int intensity, char *arg7);

void __cdecl Brighten_Color_565(unsigned char *mul_buffer, unsigned short *color_buffer, int mulbuff_width, int color_buff_width, int width, int height);
void __cdecl Brighten_Color_555(unsigned char *mul_buffer, unsigned short *color_buffer, int mulbuff_width, int color_buff_width, int width, int height);
void __cdecl Brighten_Color_556(unsigned char *mul_buffer, unsigned short *color_buffer, int mulbuff_width, int color_buff_width, int width, int height);
void __cdecl Brighten_Color_655(unsigned char *mul_buffer, unsigned short *color_buffer, int mulbuff_width, int color_buff_width, int width, int height);

void __cdecl MMX_Brighten_Color_565(unsigned char *mul_buffer, unsigned short *color_buffer, int mulbuff_width, int color_buff_width, int dst_width, int dst_height, int *mmx_buffer);
void __cdecl MMX_Brighten_Color_555(unsigned char *mul_buffer, unsigned short *color_buffer, int mulbuff_width, int color_buff_width, int dst_width, int dst_height, int *mmx_buffer);
void __cdecl MMX_Brighten_Color_556(unsigned char *mul_buffer, unsigned short *color_buffer, int mulbuff_width, int color_buff_width, int dst_width, int dst_height, int *mmx_buffer);
void __cdecl MMX_Brighten_Color_655(unsigned char *mul_buffer, unsigned short *color_buffer, int mulbuff_width, int color_buff_width, int dst_width, int dst_height, int *mmx_buffer);
}


/// <summary>
/// Creates a spot light at the location specified.
/// This routine is used by the explosion, particle, and building light code to throw a
/// short lived glow onto the map. The light adds itself to the master list, so it will be
/// updated and drawn until it burns out without the creator having to keep hold of it.
/// </summary>
/// <param name="size">The scale of the light, which governs how wide the glow grows.</param>
SpotLightClass::SpotLightClass(Coord pos, int size) :
	Radius(0),
	Position(pos),
	Size(size), // RA2: SpotLightClass at 0x5DD590 matches only so
	DisableFlags(0) // RA2: as above
{
	SpotLights.Add(this);
}


/// <summary>
/// Destroys the spot light.
/// The light removes itself from the master list, so it stops being updated and drawn.
/// </summary>
SpotLightClass::~SpotLightClass(void)
{
	SpotLights.Delete(this);
}


/// <summary>
/// Advances this spot light by one game frame.
/// The light widens as it ages and burns out once it has grown as far as it may.
/// </summary>
/// <remarks>The light deletes itself when it burns out, so the caller must not touch it
/// again after this routine returns.</remarks>
void SpotLightClass::AI(void)
{
	Radius += SPOTLIGHT_RADIUS_STEP;
	if (Radius >= SPOTLIGHT_MAX_RADIUS) {
		delete this;
	}
}


/// <summary>
/// Processes every active spot light.
/// This routine is called once per game logic loop. Lights that have grown past their
/// limit are disposed of as they are processed.
/// </summary>
void SpotLightClass::Update_All(void)
{
	for (int i = SpotLights.Count() - 1; i >= 0; i--) {
		SpotLights[i]->AI();
	}
}


/// <summary>
/// Performs the one time initialization of the spot light system.
/// This routine builds the brightness ramp surfaces that every spot light is drawn from,
/// and prepares the hicolor acceleration table when the processor can make use of it.
/// </summary>
/// <remarks>It is safe to call this routine again after the video mode changes -- the
/// artwork is built only once, but the color mode is picked up afresh each time.</remarks>
void SpotLightClass::One_Time(void)
{
	int i;
	int j;

	static bool _one_time = false;
	if (!_one_time) {
		for (i = 0; i < SPOTLIGHT_SURFACE_COUNT; i++) {
			if (SpotLightSurfaces[i] == NULL) {
				SpotLightSurfaces[i] = new BSurface(256, 128, 1);
			}
			SpotLightSurfaces[i]->Fill(0);
			BSurface surface(256, 256, 1); // RA2: RA2's form
			surface.Fill(0); // RA2: as above
			Point2D center(128, 128); // RA2: as above
			Rect bounds(0, 0, 255, 255); // RA2: as above
			int radius = i * 2 + 1;
			int ring; // RA2: as above
			if ((ring = radius) > 0) { // RA2: as above
				for (int color = 12; ring > 0; ring -= 2, color += 4) { // RA2: as above
					surface.Draw_Circle(center, ring, bounds, (color <= 255 ? color : 255)); // RA2: as above
				}
			}
			for (j = 0; j < 128; j++) {
				SpotLightSurfaces[i]->Blit_From(Rect(0, j, 255, 1), surface, Rect(0, 2 * j, 255, 1), false, true); // RA2: as above
			}
		}

		for (i = 0; i < SPOTLIGHT_EXTRA_SURFACE_COUNT; i++) {
			if (SpotLightSurfaces[i + SPOTLIGHT_SURFACE_COUNT] == NULL) {
				SpotLightSurfaces[i + SPOTLIGHT_SURFACE_COUNT] = new BSurface(256, 128, 1);
			}
			SpotLightSurfaces[i + SPOTLIGHT_SURFACE_COUNT]->Fill(0);
			BSurface surface(256, 256, 1); // RA2: as above
			surface.Fill(0); // RA2: as above
			Point2D center(128, 128); // RA2: as above
			Rect bounds(0, 0, 255, 255); // RA2: as above
			int radius = 60 * Rule->SpotlightRadius; // RA2: as above
			surface.Draw_Circle(center, i - int(radius / -358.4), bounds, 128 - i * 6); // RA2: as above
			for (j = 0; j < 128; j++) {
				SpotLightSurfaces[i + SPOTLIGHT_SURFACE_COUNT]->Blit_From(Rect(0, j, 255, 1), surface, Rect(0, 2 * j, 255, 1), false, true); // RA2: as above
			}
		}

		_one_time = true;
	}

	SpotLightColorMode = DSurface::Get_Primary_Color_Mode();
	int cpu_type = PROC_PENTIUM_PRO;
	bool mmx = false;
	Get_CPU_Type(cpu_type, mmx);

	if (mmx && SpotLightMMXBuffer == NULL) {
		SpotLightMMXBuffer = new int[65536];
		for (int color = 0; color < 65536; color++) {
			RGBClass rgb = DSurface::Deconstruct_Hicolor_Pixel((unsigned short)color); // RA2: as above
			((int *)SpotLightMMXBuffer)[color] = (rgb.Get_Red() << 16) | (rgb.Get_Green() << 8) | rgb.Get_Blue(); // RA2: as above
		}
	}
}



/// <summary>
/// Frees the artwork the spot lights draw with.
/// This routine is called during shutdown to release the brightness ramp surfaces and the
/// hicolor acceleration table that One_Time built.
/// </summary>
void SpotLightClass::Clear_All(void)
{
	if (SpotLightMMXBuffer != NULL) {
		delete [] SpotLightMMXBuffer;
		SpotLightMMXBuffer = NULL;
	}
	for (int i = 0; i < SPOTLIGHT_SURFACE_COUNT + SPOTLIGHT_EXTRA_SURFACE_COUNT; i++) {
		delete SpotLightSurfaces[i];
		SpotLightSurfaces[i] = NULL;
	}
}


/// <summary>
/// Draws this spot light onto the tactical map.
/// This routine brightens the pixels already on the logical surface through a circular
/// ramp, so the light reads as a glow over whatever has been rendered beneath it. Nothing
/// is drawn if the light lies off screen or under fog of war.
/// </summary>
void SpotLightClass::Draw_It(void)
{
	static char _index_table[SPOTLIGHT_MAX_RADIUS + 10] = {
		5, 10, 15, 20, 25, 30, 35, 40,
		45, 50, 55, 60, 61, 62, 63, 63,
		63, 62, 61, 60, 59, 58, 57, 56,
		55, 54, 53, 52, 51, 50, 49, 48,
		47, 46, 45, 44, 43, 42, 41, 40,
		39, 38, 37, 36, 35, 34, 33, 32,
		31, 30, 29, 28, 27, 26, 25, 24,
		23, 22, 21, 20, 19, 18, 17, 16,
		15, 14, 13, 12, 11, 10, 9, 8,
		7, 6, 5, 4, 3, 2, 1, 0,
		64, 65, 66, 67, 68, 69, 70, 71,
		72, 73,
	};

	unsigned int flags = DisableFlags & 0xF; // RA2: RA2's form
	if ((flags & ~1) == 0) { // RA2: as above
		if (FPSCounter::CurrentFrameRate < Detail::GetMinFrameRate()) { // RA2: as above
			return; // RA2: as above
		}
	}

	Point2D drawpoint;
	if (TacticalMap->Coord_To_Pixel(Position, drawpoint)) {
		if (!Scen->Special.IsFogOfWar || !Map.Is_Fogged(Position)) {
			Surface *dsurf = LogicalSurface;
			Rect drect(drawpoint.X - 128, drawpoint.Y - 64, 255, 127); // RA2: as above
			Rect dcliprect = TacticalRect;
			int index = _index_table[Radius];
			if (index < 64) { // RA2: as above
				index = index * Size / 64; // RA2: as above
			}
			Surface *ssurf = SpotLightSurfaces[index]; // RA2: as above
			int stride = dsurf->Stride();
			bool overlapped = false;
			Rect srect(0, 0, 255, 127);
			Rect scliprect(0, 0, 255, 127);
			void *dbuffer; // RA2: as above
			void *sbuffer; // RA2: as above
			if (XSurface::Prep_For_Blit(*dsurf, dcliprect, drect, *ssurf, scliprect, srect, overlapped, dbuffer, sbuffer)) {

				unsigned short *dptr_row = (unsigned short *)dbuffer; // RA2: as above
				unsigned char *sptr_row = (unsigned char *)sbuffer; // RA2: as above
				unsigned short *dptr = dptr_row; // RA2: as above
				unsigned char *sptr = sptr_row; // RA2: as above
				unsigned char spixel; // RA2: as above
				RGBClass rgb; // RA2: as above

				int colormode = SpotLightColorMode; // RA2: as above
				if (flags == 0 && (unsigned)colormode <= 3) { // RA2: as above
					switch (colormode) { // RA2: as above
					case COLORMODE_555: // RA2: as above
						if (SpotLightMMXBuffer) { // RA2: as above
							MMX_Brighten_Color_555(sptr, dptr, 256, stride, srect.Width, srect.Height, SpotLightMMXBuffer); // RA2: as above
						} else { // RA2: as above
							Brighten_Color_555(sptr, dptr, 256, stride, srect.Width, srect.Height); // RA2: as above
						}
						break; // RA2: as above
					case COLORMODE_556: // RA2: as above
						if (SpotLightMMXBuffer) { // RA2: as above
							MMX_Brighten_Color_556(sptr, dptr, 256, stride, srect.Width, srect.Height, SpotLightMMXBuffer); // RA2: as above
						} else { // RA2: as above
							Brighten_Color_556(sptr, dptr, 256, stride, srect.Width, srect.Height); // RA2: as above
						}
						break; // RA2: as above
					case COLORMODE_565: // RA2: as above
						if (SpotLightMMXBuffer) { // RA2: as above
							MMX_Brighten_Color_565(sptr, dptr, 256, stride, srect.Width, srect.Height, SpotLightMMXBuffer); // RA2: as above
						} else { // RA2: as above
							Brighten_Color_565(sptr, dptr, 256, stride, srect.Width, srect.Height); // RA2: as above
						}
						break; // RA2: as above
					case COLORMODE_655: // RA2: as above
						if (SpotLightMMXBuffer) { // RA2: as above
							MMX_Brighten_Color_655(sptr, dptr, 256, stride, srect.Width, srect.Height, SpotLightMMXBuffer); // RA2: as above
						} else { // RA2: as above
							Brighten_Color_655(sptr, dptr, 256, stride, srect.Width, srect.Height); // RA2: as above
						}
						break; // RA2: as above
					default: // RA2: as above
						goto fallback; // RA2: as above
					}
				} else {
					fallback:
					if (DisableFlags & 1) { // RA2: as above
						for (int y = 0; y < srect.Height; y++) { // RA2: as above
							for (int x = 0; x < srect.Width; x++) { // RA2: as above
								spixel = *sptr++; // RA2: as above
								if (spixel != 0) { // RA2: as above
									int scale = 256 - spixel; // RA2: as above
									rgb = DSurface::Deconstruct_Hicolor_Pixel(*dptr); // RA2: as above
									*dptr = DSurface::Build_Hicolor_Pixel((rgb.Get_Red() * scale) >> 8, (rgb.Get_Green() * scale) >> 8, (rgb.Get_Blue() * scale) >> 8); // RA2: as above
								}
								dptr++; // RA2: as above
							}
							dptr_row = (unsigned short *)((unsigned char *)dptr_row + stride); // RA2: as above
							dptr = dptr_row; // RA2: as above
							sptr_row += 256; // RA2: as above
							sptr = sptr_row; // RA2: as above
						}
					} else if (DisableFlags & 0xE) { // RA2: as above
						for (int x = 0; x < srect.Height; x++) { // RA2: as above
							for (int y = 0; y < srect.Width; y++) { // RA2: as above
								spixel = *sptr++; // RA2: as above
								if (spixel != 0) { // RA2: as above
									rgb = DSurface::Deconstruct_Hicolor_Pixel(*dptr); // RA2: as above
									int scale = spixel + 256; // RA2: as above
									int rr = rgb.Get_Red(); // RA2: as above
									int gg = rgb.Get_Green(); // RA2: as above
									int bb = rgb.Get_Blue(); // RA2: as above
									if (!(DisableFlags & 2)) { // RA2: as above
										rr = (rr * scale) >> 8; // RA2: as above
										if (rr > 255) rr = 255; // RA2: as above
									}
									if (!(DisableFlags & 4)) { // RA2: as above
										gg = (gg * scale) >> 8; // RA2: as above
										if (gg > 255) gg = 255; // RA2: as above
									}
									if (!(DisableFlags & 8)) { // RA2: as above
										bb = (bb * scale) >> 8; // RA2: as above
										if (bb > 255) bb = 255; // RA2: as above
									}
									*dptr = DSurface::Build_Hicolor_Pixel(rr, gg, bb); // RA2: as above
								}
								dptr++; // RA2: as above
							}
							dptr_row = (unsigned short *)((unsigned char *)dptr_row + stride); // RA2: as above
							dptr = dptr_row; // RA2: as above
							sptr_row += 256; // RA2: as above
							sptr = sptr_row; // RA2: as above
						}
					} else { // RA2: as above
						for (int x = 0; x < srect.Height; x++) { // RA2: as above
							for (int y = 0; y < srect.Width; y++) { // RA2: as above
								spixel = *sptr++; // RA2: as above
								if (spixel != 0) { // RA2: as above
									int scale = spixel + 256; // RA2: as above
									rgb = DSurface::Deconstruct_Hicolor_Pixel(*dptr); // RA2: as above
									int rr = (rgb.Get_Red() * scale) >> 8; // RA2: as above
									int gg = (rgb.Get_Green() * scale) >> 8; // RA2: as above
									int bb = (rgb.Get_Blue() * scale) >> 8; // RA2: as above
									*dptr = DSurface::Build_Hicolor_Pixel(MIN(255, rr), MIN(255, gg), MIN(255, bb)); // RA2: as above
								}
								dptr++; // RA2: as above
							}
							dptr_row = (unsigned short *)((unsigned char *)dptr_row + stride); // RA2: as above
							dptr = dptr_row; // RA2: as above
							sptr_row += 256; // RA2: as above
							sptr = sptr_row; // RA2: as above
						}
					}
				}
				dsurf->Unlock();
				ssurf->Unlock();
			}
		}
	}
}



/// <summary>
/// Draws every active spot light.
/// This routine is called by the tactical map render after the scene has been laid down,
/// since a spot light brightens the pixels already present rather than drawing its own.
/// </summary>
void SpotLightClass::Draw_All(void)
{
	for (int i = SpotLights.Count() - 1; i >= 0; i--) {
		SpotLights[i]->Draw_It();
	}
}

