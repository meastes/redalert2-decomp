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

#include "ionblast.h"

#include "_rect.h"
#include "_rules.h"
#include "_surface.h"
#include "_tactica.h"
#include "_zbuffer.h"
#include "anim.h"
#include "bsurface.h"
#include "building.h"
#include "cell.h"
#include "combat.h"
#include "foot.h"
#include "globals.h"
#include "goptions.h"
#include "infantry.h"
#include "rules.h"
#include "tactical.h"
#include "unit.h"
#include "vector.h"
#include "vector3.h"
#include "zbuffer.h"

int SpiralIndexToSurfaceLUT[289];
int BlastSurfaceStride = 0;

BSurface * BlastSurfaces[80];

/*
 * Parameters of the ion blast shockwave ripple. The ripple is an elliptical
 * band that sweeps outward from the blast center over the lifetime of the
 * blast, displacing the pixels it covers by the height of a decaying sine
 * wave. The band starts as [WaveStartDistance - WaveBandWidth, WaveStartDistance]
 * and both edges advance by (a multiple of) WaveSpeed every frame. WaveRockScale
 * turns the slope of the wave into the angle a voxel unit riding it is rocked by.
 */
const double WaveStartDistance = 0.0;
const double WaveBandWidth = 57.0;
const double WaveFrequency = 0.11;
const double WaveAmplitudeFalloff = 25.0;
const double WaveRockScale = DEG_TO_RAD(360);
const double WaveSpeedScale = 0.0125;

double WaveSpeed = ((256.0 - WaveStartDistance) + WaveBandWidth) * WaveSpeedScale;
double WavePhaseSpeed = WaveSpeed;

DynamicVectorClass<IonBlastClass *> IonBlastClass::IonBlasts;

int Point_To_Spiral_Index(Point2D point);
void Calculate_Index_To_Surface_LUT(int stride);
Point2D Spiral_Index_To_Point(int index);


/// <summary>
/// Creates an ion cannon ground blast at the specified location.
/// The blast adds itself to the list of blasts in progress and is looked after from
/// there until its shockwave has expired. The ion cannon superweapon and the trigger
/// action of the same name are the two things that set one off.
/// </summary>
IonBlastClass::IonBlastClass(Coord coord) :
	Lifetime(0),
	Position(coord), // RA2: as above
	DisableIonBeam(0) // RA2: the constructor also clears the word at +0x10
{
	IonBlasts.Add(this);
}


/// <summary>
/// Removes this blast from the list of blasts in progress.
/// </summary>
IonBlastClass::~IonBlastClass(void)
{
	IonBlasts.Delete(this);
}


/// <summary>
/// Advances this ion blast by a single frame.
/// On the frame the blast lands it throws up the explosion animations, damages whatever
/// is beneath it and lights the scene. From then on the expanding shockwave takes hold
/// of the infantry and vehicles it rolls over, bringing them to a halt and rocking them
/// away from the center. Once the wave has run its course the blast deletes itself.
/// </summary>
void IonBlastClass::AI(void)
{
	if (Lifetime >= 79) {
		delete this;
		return;
	}

	if (DisableIonBeam != 1) { // RA2: a blast with the word at +0x10 set only ages; the body below sits one level deeper
		Point2D point; // RA2: as above
		TacticalMap->Coord_To_Pixel(Position, point); // RA2: the result is not tested and Map.ScreenY is not touched

		if (Lifetime == 0) { // RA2: as above
			if (Map[Position].Land_Type() == LAND_WATER) { // RA2: as above
				new AnimClass(Rule->SplashList[Rule->SplashList.Count() - 1], Position + Coord(0,0,5), 0, 1, (ShapeFlags_Type)(SHAPE_CENTER|SHAPE_WIN_REL), 0); // RA2: as above
			} else {
				new AnimClass(Rule->IonBlast, Position + Coord(0,0,5), 0, 1, (ShapeFlags_Type)(SHAPE_CENTER|SHAPE_WIN_REL), 0); // RA2: as above
			}
			new AnimClass(Rule->IonBeam, Position + Coord(0,0,5), 0, 1, (ShapeFlags_Type)(SHAPE_CENTER|SHAPE_WIN_REL), 0); // RA2: as above

			if (Map[Position].IsUnderBridge) { // RA2: as above
				Coord coord; // RA2: declared where it is used
				coord.X = Position.X; // RA2: as above
				coord.Z = BRIDGE_LEPTON_HEIGHT + Position.Z; // RA2: as above
				coord.Y = Position.Y; // RA2: as above
				Explosion_Damage(coord, Rule->IonCannonDamage, NULL, Rule->IonCannonWarhead, true); // RA2: as above
			}

			Explosion_Damage(Position, Rule->IonCannonDamage, NULL, Rule->IonCannonWarhead, true); // RA2: as above
			Combat_Lighting(Position, Rule->IonCannonDamage, Rule->IonCannonWarhead, 0, 0); // RA2: the five argument form
		}

		Cell center(Position); // RA2: as above

		for (int x_offset = -3; x_offset < 4; x_offset++) { // RA2: as above
			for (int y_offset = -3; y_offset < 4; y_offset++) { // RA2: as above

				FootClass * occupier = (FootClass *)Map[center + Cell(x_offset, y_offset)].Cell_Occupier(); // RA2: as above

				while (occupier != NULL) { // RA2: as above

					if (occupier->Fetch_RTTI() == RTTI_INFANTRY || occupier->Fetch_RTTI() == RTTI_UNIT) { // RA2: the exe tests the RTTI (15 then 1), not dynamic_cast

						Point2D occupier_point; // RA2: as above
						TacticalMap->Coord_To_Pixel(occupier->Get_Coord(), occupier_point); // RA2: as above

						Point2D diff = occupier_point - point; // RA2: as above
						int screen_dist = (int)fastmath::sqrt((double)diff.Y * diff.Y + (double)diff.X * diff.X) + 8; // RA2: Y squared first, written out instead of Point2D::Length

						if (screen_dist < 128) { // RA2: as above

							char * displacement = (char *)BlastSurfaces[Lifetime]->Lock(Point2D(screen_dist + 128, 64)); // RA2: as above

							if (*displacement > 0) { // RA2: as above

								occupier->Set_Speed(0.0); // RA2: no weapons factory exemption; the blast always stops the unit

								int draw_offset = Spiral_Index_To_Point(*displacement).Y; // RA2: as above
								occupier->IonBlastYDrawOffset = 2 * draw_offset; // RA2: as above
							}

							if (occupier->Techno_Type_Class()->Voxel.VoxLib != NULL // RA2: as above
								&& !occupier->Techno_Type_Class()->Voxel.VoxLib->Load_Failed() // RA2: as above
								&& *displacement >= 0) { // RA2: as above

								/*
								 * Rock the voxel unit away from the blast: build the vector
								 * from the unit to the blast center, rotate it into the unit's
								 * facing frame, and scale it by the slope of the ripple at the
								 * unit's distance from the blast. The wave height there is
								 * height/decay, so its slope comes out by the quotient rule.
								 */
								Coord blast_coord = Position; // RA2: as above
								Coord occupier_coord = occupier->Get_Coord(); // RA2: as above

								Vector3 to_blast((float)(blast_coord.X - occupier_coord.X), (float)(occupier_coord.Y - blast_coord.Y), (float)(blast_coord.Z - occupier_coord.Z)); // RA2: as above
								Vector3 flat = to_blast; // RA2: as above

								double facing_rad = occupier->PrimaryFacing.Current().As_Radian(); // RA2: as above
								float facing_sin = (float)fastmath::sin(facing_rad); // RA2: as above
								float facing_cos = (float)fastmath::cos(facing_rad); // RA2: as above

								float blast_dist = fastmath::sqrt((double)to_blast.X * to_blast.X + (double)to_blast.Y * to_blast.Y + (double)to_blast.Z * to_blast.Z); // RA2: written out instead of Vector3::Length

								if (fabs(blast_dist) > 0.00002) { // RA2: as above

									flat.Z = 0.0; // RA2: as above

									double flat_len = fastmath::sqrt(flat.Y * flat.Y + flat.X * flat.X); // RA2: as above

									Vector3 normal; // RA2: as above
									if (flat_len != 0.0) { // RA2: as above
										normal = flat / flat_len; // RA2: as above
									} else {
										normal = flat; // RA2: as above
									}

									float forwards = normal.X * facing_cos + normal.Y * facing_sin; // RA2: as above
									float sideways_zsin = -(normal.Z * facing_sin); // RA2: as above
									double sideways_zcos = normal.Z * facing_cos; // RA2: as above
									double sideways_y = normal.X * facing_sin - normal.Y * facing_cos; // RA2: as above
									float sideways = (float)fastmath::sqrt(sideways_zsin * sideways_zsin + sideways_zcos * sideways_zcos + sideways_y * sideways_y); // RA2: as above
									if (fabs(facing_cos * forwards - facing_sin * sideways - normal.X) > 0.0002 // RA2: as above
										|| fabs(facing_cos * sideways + facing_sin * forwards - normal.Y) > 0.0002) { // RA2: as above
										sideways = -sideways; // RA2: as above
									}

									double decay = blast_dist + WaveAmplitudeFalloff; // RA2: as above
									double height = (fastmath::sin((blast_dist - (double)Lifetime * WavePhaseSpeed + 38.0) * WaveFrequency) * 3.5 + 3.0) * WaveAmplitudeFalloff; // RA2: as above
									double phase_cos = fastmath::cos((blast_dist - (double)Lifetime * WavePhaseSpeed + 38.0) * WaveFrequency); // RA2: as above
									double slope = ((phase_cos * WaveFrequency * WaveAmplitudeFalloff * 3.5) * decay - height) / (decay * decay); // RA2: as above

									occupier->AngleRotatedSideways = (float)(sideways * slope * WaveRockScale); // RA2: as above
									occupier->AngleRotatedForwards = (float)(-(forwards * slope * WaveRockScale)); // RA2: as above
								}
							}
						}
					}

					occupier = (FootClass *)occupier->Next; // RA2: as above
				}
			}
		}
	}

	Lifetime++;
}


/// <summary>
/// Processes the logic of every ion blast in progress.
/// This routine is called by the main game logic once per frame. Blasts that have
/// outlived their shockwave delete themselves along the way.
/// </summary>
void IonBlastClass::Update_All(void)
{
	for (int i = IonBlasts.Count() - 1; i >= 0; i--) {
		IonBlasts[i]->AI();
	}
}


/// <summary>
/// Precalculates the shockwave displacement surfaces.
/// One surface is built for each frame of the blast animation, holding the direction
/// the ripple pushes every pixel around the blast center. This is far too expensive to
/// work out while the game is running, so it is done once during startup and the
/// surfaces are kept until the game exits.
/// </summary>
/// <remarks>Harmless to call more than once -- only the first call does any work.</remarks>
void IonBlastClass::One_Time(void)
{
	static bool one_time = false;
	if (one_time) {
		return;
	}

	double min_dist = WaveStartDistance - WaveBandWidth;
	double max_dist = WaveStartDistance;

	for (int frame = 0; frame < ARRAY_SIZE(BlastSurfaces); frame++) {
		BlastSurfaces[frame] = new BSurface(256, 128, 1);
		BlastSurfaces[frame]->Fill(-1);

		/*
		 * Rasterize this frame's ripple band into the displacement surface.
		 * The surface is 256x128 with the blast center in the middle; only one
		 * quadrant is computed and the result is mirrored into the other three.
		 * The vertical axis counts double toward the distance because the
		 * shockwave ellipse is half as tall as it is wide.
		 */
		char * center = (char *)BlastSurfaces[frame]->Lock() + 0x4080;
		for (int y = 63; y >= 0; y--) {
			int y_dist_sq = y * y * 4;
			for (int x = 127; x >= 0; x--) {
				double dist = fastmath::sqrt(x * x + y_dist_sq);
				if (dist >= min_dist && dist <= max_dist) {
					double wave_height = (fastmath::sin((dist - frame * WavePhaseSpeed + 38.0) * WaveFrequency) * 3.5 + 3.0) / (dist / WaveAmplitudeFalloff + 1.0) + 0.5;
					char spiral_index = Point_To_Spiral_Index(Point2D(0, wave_height));
					int row = y << 8;
					center[row + x] = spiral_index;  /// Quadrant I
					center[row - x] = spiral_index;  /// Quadrant II
					char * mirror = center - row;
					mirror[x] = spiral_index;        /// Quadrant IV
					mirror[-x] = spiral_index;       /// Quadrant III
				}
			}
		}

		if (128.0 - WaveSpeed > max_dist) {
			max_dist += WaveSpeed;
		}

		min_dist = WaveSpeed * 1.2 + min_dist;

		if (min_dist > max_dist) {
			min_dist = max_dist;
		}
	}

	one_time = true;
}


/// <summary>
/// Frees the precalculated shockwave surfaces.
/// This routine is called as the game shuts down, to give back the memory One_Time
/// claimed for the wave animation.
/// </summary>
void IonBlastClass::Clear_All(void)
{
	for (int i = 0; i < ARRAY_SIZE(BlastSurfaces); i++) {
		delete BlastSurfaces[i];
		BlastSurfaces[i] = NULL;
	}
}


/// <summary>
/// Draws this blast's shockwave onto the tactical view.
/// Nothing of the wave is actually drawn; instead the scene already on the surface is
/// warped, each pixel the ripple band covers being replaced by a neighbor pulled from
/// the direction the wave is pushing. The depth buffer is consulted so that anything
/// standing nearer than the wave is left undisturbed. The effect is a luxury, so it
/// only appears at the highest detail level.
/// </summary>
/// <remarks>Call this only after the scene has been rendered, and only once the wave
/// surfaces have been built by One_Time.</remarks>
void IonBlastClass::Draw_It(void)
{
	Point2D point;
	if (Options.DetailLevel == 2 && TacticalMap->Coord_To_Pixel(Position, point)) {
		Surface * dest_surface = LogicalSurface;
		Rect dcliprect = TacticalRect;
		dcliprect.Height -= 7;
		Surface * source_surface = BlastSurfaces[Lifetime];
		int stride = LogicalSurface->Stride();
		Rect srect(0, 0, 256, 128);
		Rect drect(point.X - 128, point.Y - 64, 256, 128);
		Rect scliprect(0, 0, 256, 128);
		bool overlapped = false;
		short * dbuffer;
		char * sbuffer;

		if (XSurface::Prep_For_Blit(*dest_surface, dcliprect, drect, *source_surface, scliprect, srect, overlapped, (void*&)dbuffer, (void*&)sbuffer)) {
			char * sptr = sbuffer;
			short * dest_row = dbuffer;
			char * source_row = sbuffer;

			int depth_z = TacticalMap->Z_Lepton_To_Pixel(Position.Z);
			short base_z = (short)DepthBuffer->Get_Scroll() - depth_z;
			unsigned short draw_z = base_z - drect.Y - 3;

			unsigned short * zbuffer_base = (unsigned short *)DepthBuffer->Get_Buffer_Offset(Point2D(0, drect.Y));
			int height = srect.Height + 1;
			int surface_width = LogicalSurface->Get_Width();
			int buffer_width = DepthBuffer->Get_Buffer_Width();

			if (zbuffer_base + surface_width + height * buffer_width < (unsigned short *)DepthBuffer->Get_Buffer_End()) {
				unsigned short * zptr = zbuffer_base + drect.X;
				int x;
				for (int y = 0; y < srect.Height; ++y) {
					short * dptr = dest_row;
					for (x = 0; x < srect.Width; ++x) {
						int index = *sptr++;
						if (index > 0 && *zptr > draw_z) {
							*dptr = dptr[SpiralIndexToSurfaceLUT[index]];
						}
						dptr++;
						zptr++;
					}
					dest_row = (short *)((char *)dest_row + stride);
					source_row += 256;
					draw_z--;
					sptr = source_row;
					zptr += buffer_width - x;
				}
			} else {
				for (int y = 0; y < srect.Height; ++y) {
					short * dptr = dest_row;
					for (int x = 0; x < srect.Width; ++x) {
						int index = *sptr++;
						if (index > 0 && *(unsigned short *)DepthBuffer->Get_Buffer_Offset(Point2D(drect.X + x, drect.Y + y)) > draw_z) {
							*dptr = dptr[SpiralIndexToSurfaceLUT[index]];
						}
						dptr++;
					}
					dest_row = (short *)((char *)dest_row + stride);
					source_row += 256;
					draw_z--;
					sptr = source_row;
				}
			}

			dest_surface->Unlock();
			source_surface->Unlock();
		}
	}
}


/// <summary>
/// Draws the shockwave of every ion blast in progress.
/// The tactical map calls this routine after the scene has been rendered, since the
/// blast distorts what is already on the surface rather than drawing anything of its
/// own.
/// </summary>
void IonBlastClass::Draw_All(void)
{
	if (LogicalSurface->Stride() != BlastSurfaceStride) {
		Calculate_Index_To_Surface_LUT(LogicalSurface->Stride());
	}
	for (int i = IonBlasts.Count() - 1; i >= 0; i--) {
		IonBlasts[i]->Draw_It();
	}
}


/// <summary>
/// Converts a spiral index back into a grid point.
/// Displacement offsets are kept as spiral indices so that each one fits into a single
/// byte of the wave surface. This routine unpacks one back into the offset it stands
/// for, counting outward from the center in concentric square rings.
/// </summary>
/// <returns>Returns with the grid point that the index refers to.</returns>
Point2D Spiral_Index_To_Point(int index)
{
	index--; /// Skip the center point, which corresponds to index 0.

	int layer = 1; /// Start at the first spiral layer.

	/*
	 * Determine which concentric layer the index falls into.
	 * Each layer has 8 * radius points.
	 */
	if (index >= 8) {
		for (int i = 8; index >= i; i += 8) {
			index -= i;
			layer++;
		}
	}

	/*
	 * Map the index within the layer to a point on one of its four sides:
	 * Right (X = +radius), Top (Y = +radius),
	 * Left (X = -radius), Bottom (Y = -radius).
	 */
	if (index < 2 * layer + 1) {
		return(Point2D(layer, index - layer)); /// Right edge
	} else if (index < 4 * layer + 1) {
		return(Point2D(3 * layer - index, layer)); // Top edge
	} else if (index < 6 * layer + 1) {
		return(Point2D(-layer, 5 * layer - index)); /// Left edge
	} else {
		return(Point2D(index - 7 * layer, -layer)); // Bottom edge
	}
}


/// <summary>
/// Converts a grid point into its index within the spiral.
/// This routine is the inverse of Spiral_Index_To_Point. The shockwave rasterizer uses
/// it to fold a displacement offset down into the single byte that is stored per pixel
/// of the wave surface.
/// </summary>
/// <returns>Returns with the spiral index of the point, zero being the center.</returns>
int Point_To_Spiral_Index(Point2D point)
{
	/*
	 * The center point (0, 0) is the origin of the spiral and has index 0.
	 */
	if (point == Point2D(0,0)) {
		return(0);
	}

	/*
	 * Determine which concentric square layer the point lies in.
	 * Each layer `n` surrounds the previous one and spans from -n to +n in both X and Y.
	 */
	int layer = MAX(abs(point.X), abs(point.Y));

	int index = 1;

	for (int i = 1; i < layer; i++) {
		index += 8 * i;
	}

	/*
	 * Add the offset of the point within its current layer's perimeter.
	 * Each layer has 8 * n positions, arranged clockwise starting from (n, -n+1).
	 * The positions proceed in the following order:
	 * - From (n, -n+1) up to (n, n)          -> Right edge (ascending Y)
	 * - From (n-1, n) to (-n, n)             -> Top edge (descending X)
	 * - From (-n, n-1) to (-n, -n)           -> Left edge (descending Y)
	 * - From (-n+1, -n) to (n, -n)           -> Bottom edge (ascending X)
	 */
	if (point.X == layer) {
		/// Right edge
		index += 1 * layer + point.Y;
	} else if (point.Y == layer) {
		// Top edge
		index += 3 * layer - point.X;
	} else if (point.X == -layer) {
		/// Left edge
		index += 5 * layer - point.Y;
	} else {
		// Bottom edge
		index += 7 * layer + point.X;
	}

	return(index);
}


/// <summary>
/// Rebuilds the spiral index to buffer offset table.
/// The blast drawer displaces a pixel by fetching a neighbor a short way off in the
/// direction the ripple is pushing. This routine works out where each of those
/// neighbors lives in a surface of the given stride, so that the drawer only has to
/// add. Draw_All calls it whenever the logical surface stride has changed.
/// </summary>
/// <param name="stride">The row stride of the surface the blast will be drawn onto.</param>
void Calculate_Index_To_Surface_LUT(int stride)
{
	/*
	 * Spiral center offset is always zero.
	 */
	SpiralIndexToSurfaceLUT[0] = 0;

	/*
	 * Store the stride used.
	 */
	BlastSurfaceStride = stride;

	for (int index = 1; index < ARRAY_SIZE(SpiralIndexToSurfaceLUT); index++) {
		Point2D point = Spiral_Index_To_Point(index);
		SpiralIndexToSurfaceLUT[index] = point.X + point.Y * stride;
	}
}
