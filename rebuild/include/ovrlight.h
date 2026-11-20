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

#pragma once

#include "coord.h"

template<class T> class DynamicVectorClass;

class SpotLightClass
{
	public:
		SpotLightClass(Coord pos, int size);
		~SpotLightClass(void);

		void Set_Radius(int radius) {Radius=radius;}
		void Disable(unsigned int flags) {DisableFlags |= flags;} // RA2: ORs into the member at +0x14
		void AI(void);
		void Draw_It(void);

		static void One_Time(void);
		static void Update_All(void);
		static void Clear_All(void);
		static void Draw_All(void);

		enum {
			SPOTLIGHT_RADIUS_STEP = 8,
			SPOTLIGHT_MAX_RADIUS = 80,
			SPOTLIGHT_SURFACE_COUNT = 64,
			SPOTLIGHT_EXTRA_SURFACE_COUNT = 10,
		};

	private:
		/*
		 * This is the map coordinate the glow is centered on, fixed when the light is created.
		 * Nothing is drawn if that coordinate lies off screen or under fog of war.
		 */
		Coord Position;

		/*
		 * This is how far the glow has spread, widened by SPOTLIGHT_RADIUS_STEP with
		 * every update until the light burns out at SPOTLIGHT_MAX_RADIUS. It doubles as
		 * the light's age, since it also picks which brightness ramp the glow is drawn from.
		 */
		int Radius;

		/*
		 * This is the scale of the light, specified when it is created. It scales the ramp
		 * picked by the radius, so that a large explosion throws a stronger glow than a
		 * small one of the same age.
		 */
		int Size;

		/*
		**	These flags switch the light off; any set bit keeps it dark.
		*/
		unsigned int DisableFlags; // RA2: the class is 24 bytes (new SpotLightClass pushes 24)
};


extern DynamicVectorClass<SpotLightClass *> SpotLights;

// RA2: the pixel brighten routines used by spot lights
extern "C" {
void __cdecl Brighten_Color_565(unsigned char * mul_buffer, unsigned short * color_buffer, int mulbuff_width, int color_buff_width, int width, int height); // RA2: 0x79813A
void __cdecl Brighten_Color_555(unsigned char * mul_buffer, unsigned short * color_buffer, int mulbuff_width, int color_buff_width, int width, int height); // RA2: 0x7983D1
void __cdecl Brighten_Color_556(unsigned char * mul_buffer, unsigned short * color_buffer, int mulbuff_width, int color_buff_width, int width, int height); // RA2: 0x7982F4
void __cdecl Brighten_Color_655(unsigned char * mul_buffer, unsigned short * color_buffer, int mulbuff_width, int color_buff_width, int width, int height); // RA2: 0x798217
void __cdecl MMX_Brighten_Color_565(unsigned char * mul_buffer, unsigned short * color_buffer, int mulbuff_width, int color_buff_width, int dst_width, int dst_height, int * mmx_buffer); // RA2: 0x7984C0
void __cdecl MMX_Brighten_Color_555(unsigned char * mul_buffer, unsigned short * color_buffer, int mulbuff_width, int color_buff_width, int dst_width, int dst_height, int * mmx_buffer); // RA2: 0x798602
void __cdecl MMX_Brighten_Color_556(unsigned char * mul_buffer, unsigned short * color_buffer, int mulbuff_width, int color_buff_width, int dst_width, int dst_height, int * mmx_buffer); // RA2: 0x798740
void __cdecl MMX_Brighten_Color_655(unsigned char * mul_buffer, unsigned short * color_buffer, int mulbuff_width, int color_buff_width, int dst_width, int dst_height, int * mmx_buffer); // RA2: 0x798880
}
