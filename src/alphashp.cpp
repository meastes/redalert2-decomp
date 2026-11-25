/*******************************************************************************
 *                                O P E N  T S
 *******************************************************************************
 * SPDX-License-Identifier: GPL-3.0-or-later
 * Copyright 2026 OpenTS contributors
 *
 * See LICENSE.md for applicable additional terms and warranty disclaimers.
 ******************************************************************************/

#define INCLUDE_COM
#include "always.h"
#include "rect.h"
#include "geometry.h"

#include "alphashp.h"

#include "_alpha.h"
#include "_tactica.h"
#include "crc.h"
#include "globals.h"
#include "object.h"
#include "objtype.h"
#include "shapeset.h"
#include "sun.h"
#include "swizzle.h"
#include "tactical.h"
#include "tracker.h"
#include "vector.h"
#include "abuffer.h"

DynamicVectorClass<AlphaShapeClass *> AlphaShapes;

unsigned char AlphaShapeClass::BrightnessTable[256][256];
static bool BrightnessCalculated = false;


/// <summary>
/// Creates an alpha shape for the object specified.
/// This routine attaches the light casting shape declared by the owner's type at the
/// screen position given. The shape adds itself to the global list, so the tactical map
/// will blend it into the alpha buffer from here on.
/// </summary>
/// <param name="owner">The object that this alpha shape belongs to.</param>
/// <param name="x">The horizontal pixel position to place the shape at.</param>
/// <param name="y">The vertical pixel position to place the shape at.</param>
AlphaShapeClass::AlphaShapeClass(ObjectClass * owner, int x, int y) :
	BASECLASS(),
	Owner(owner),
	ImageData(NULL),
	IsToDelete(false)
{
	ShapeSet const * image = (ShapeSet const *)owner->Class_Of()->AlphaImageData;
	ImageData = image;
	DrawRect = Rect(x, y, image->Get_Width(), image->Get_Height());
	AlphaShapes.Add(this);
	ObjectPtrTracker.Add(this);

	if (!BrightnessCalculated) {
		Calculate_Brightness_Table();
	}
}


/// <summary>
/// Creates a blank alpha shape.
/// This routine is used by the save/load system, which needs an empty object to read the
/// saved state into.
/// </summary>
AlphaShapeClass::AlphaShapeClass(void) :
	BASECLASS(),
	Owner(NULL),
	DrawRect(0, 0, 0, 0),
	ImageData(NULL),
	IsToDelete(false)
{
	AlphaShapes.Add(this);
	ObjectPtrTracker.Add(this);

	if (!BrightnessCalculated) {
		Calculate_Brightness_Table();
	}
}


/// <summary>
/// Creates an alpha shape without initializing it.
/// This routine is used by the load process, which is about to overwrite every member
/// with the values read out of the save stream.
/// </summary>
AlphaShapeClass::AlphaShapeClass(NoInitClass const & x) :
	BASECLASS(x)
{
}


/// <summary>
/// Destroys the alpha shape.
/// This routine takes the shape out of the global list so that it stops contributing
/// light to the tactical map.
/// </summary>
AlphaShapeClass::~AlphaShapeClass(void)
{
	AlphaShapes.Delete(this);
	ObjectPtrTracker.Delete(this);
}


/// <summary>
/// Fetches the class identifier used to persist this object.
/// The save system writes this identifier ahead of the object data so that the loader
/// knows what kind of object to reconstruct.
/// </summary>
/// <param name="retval">Pointer to the buffer that will receive the class identifier.</param>
/// <returns>Returns with S_OK, or E_POINTER if no buffer was supplied.</returns>
HRESULT STDMETHODCALLTYPE AlphaShapeClass::GetClassID(CLSID * retval)
{
	if (retval == NULL) return(E_POINTER);
	*retval = CLSID_AlphaShapeClass;
	return(S_OK);
}


/// <summary>
/// Fetches the run time type identifier of this object.
/// </summary>
/// <returns>Returns with RTTI_ALPHASHAPE.</returns>
RTTIType AlphaShapeClass::Fetch_RTTI(void) const
{
	return(RTTI_ALPHASHAPE);
}


/// <summary>
/// Fetches the size of this object in bytes.
/// This routine is used by the save system to determine how much of the object must be
/// committed to the save stream.
/// </summary>
/// <returns>Returns with the byte size of this object.</returns>
int AlphaShapeClass::Fetch_Object_Size(void) const
{
	return(sizeof(*this));
}


/// <summary>
/// Adds the state of this alpha shape to the running game checksum.
/// This routine is used by the multiplayer sync check to prove that every machine holds
/// an identical copy of this object.
/// </summary>
/// <param name="crc">The checksum engine to submit the object state to.</param>
void AlphaShapeClass::Compute_CRC(CRCEngine & crc) const
{
	BASECLASS::Compute_CRC(crc);
	crc(DrawRect.X);
	crc(DrawRect.Y);
	crc(DrawRect.Width);
	crc(DrawRect.Height);
}


/// <summary>
/// Loads this alpha shape from the save game stream.
/// The object is rebuilt in place and its owner pointer is handed to the swizzler. The
/// shape image itself is dropped and fetched again from the owner's type data the next
/// time the shape is drawn.
/// </summary>
/// <returns>Returns with S_OK if the object was read successfully.</returns>
HRESULT STDMETHODCALLTYPE AlphaShapeClass::Load(IStream * stream)
{
	HRESULT result = BASECLASS::Load(stream);
	if (SUCCEEDED(result)) {
		new (this) AlphaShapeClass(NoInitClass());

		Swizzle_Pointer(&Owner);
		ImageData = NULL;
	}
	return(result);
}


/// <summary>
/// Saves this alpha shape to the save game stream.
/// </summary>
/// <param name="cleardirty">Should the dirty flag be cleared once the object is written?</param>
/// <returns>Returns with S_OK if the object was written successfully.</returns>
HRESULT STDMETHODCALLTYPE AlphaShapeClass::Save(IStream * stream, BOOL cleardirty)
{
	HRESULT result = BASECLASS::Save(stream, cleardirty);
	if (SUCCEEDED(result)) {
		return(S_OK);
	}
	return(result);
}


/// <summary>
/// Removes any reference this shape has to the object specified.
/// When the object going away is this shape's owner, the shape flags itself for deletion
/// rather than lingering with a dangling owner pointer.
/// </summary>
/// <param name="target">The object that is about to be removed from the game.</param>
void AlphaShapeClass::Detach(AbstractClass const * target, bool all)
{
	if (target == Owner) {
		IsToDelete = true;
	}
}


/// <summary>
/// Purges the alpha shapes that have been flagged for deletion.
/// This routine is used by the game logic to clean up the shapes whose owning object has
/// gone away, and to ensure the brightness table exists before any drawing is attempted.
/// </summary>
void AlphaShapeClass::Update_All(void)
{
	if (!BrightnessCalculated) {
		Calculate_Brightness_Table();
	}

	for (int i = AlphaShapes.Count() - 1; i >= 0; i--) {
		if (AlphaShapes[i]->IsToDelete) {
			delete AlphaShapes[i];
		}
	}
}


/// <summary>
/// Draws the alpha shapes that fall across a single map tile.
/// This routine is the per cell counterpart to Draw_All. Each shape's light is blended
/// into the alpha buffer as usual, but only where it lands inside the diamond of the
/// tile being drawn.
/// </summary>
/// <param name="point">The pixel position of the tile being drawn.</param>
/// <param name="cliprect">The clipping rectangle to draw within.</param>
void AlphaShapeClass::Draw_In_Area(Point2D const & point, Rect const & cliprect)
{
#define ALPHA_BLOCK4 "\xDB\xDB\xDB\xDB"
	static const unsigned char _tilemask[] =
		"                                                            "
		"                            " ALPHA_BLOCK4 "                            "
		"                          " ALPHA_BLOCK4 ALPHA_BLOCK4 "                          "
		"                        " ALPHA_BLOCK4 ALPHA_BLOCK4 ALPHA_BLOCK4 "                        "
		"                      " ALPHA_BLOCK4 ALPHA_BLOCK4 ALPHA_BLOCK4 ALPHA_BLOCK4 "                      "
		"                    " ALPHA_BLOCK4 ALPHA_BLOCK4 ALPHA_BLOCK4 ALPHA_BLOCK4 ALPHA_BLOCK4 "                    "
		"                  " ALPHA_BLOCK4 ALPHA_BLOCK4 ALPHA_BLOCK4 ALPHA_BLOCK4 ALPHA_BLOCK4 ALPHA_BLOCK4 "                  "
		"                " ALPHA_BLOCK4 ALPHA_BLOCK4 ALPHA_BLOCK4 ALPHA_BLOCK4 ALPHA_BLOCK4 ALPHA_BLOCK4 ALPHA_BLOCK4 "                "
		"              " ALPHA_BLOCK4 ALPHA_BLOCK4 ALPHA_BLOCK4 ALPHA_BLOCK4 ALPHA_BLOCK4 ALPHA_BLOCK4 ALPHA_BLOCK4 ALPHA_BLOCK4 "              "
		"            " ALPHA_BLOCK4 ALPHA_BLOCK4 ALPHA_BLOCK4 ALPHA_BLOCK4 ALPHA_BLOCK4 ALPHA_BLOCK4 ALPHA_BLOCK4 ALPHA_BLOCK4 ALPHA_BLOCK4 "            "
		"          " ALPHA_BLOCK4 ALPHA_BLOCK4 ALPHA_BLOCK4 ALPHA_BLOCK4 ALPHA_BLOCK4 ALPHA_BLOCK4 ALPHA_BLOCK4 ALPHA_BLOCK4 ALPHA_BLOCK4 ALPHA_BLOCK4 "          "
		"        " ALPHA_BLOCK4 ALPHA_BLOCK4 ALPHA_BLOCK4 ALPHA_BLOCK4 ALPHA_BLOCK4 ALPHA_BLOCK4 ALPHA_BLOCK4 ALPHA_BLOCK4 ALPHA_BLOCK4 ALPHA_BLOCK4 ALPHA_BLOCK4 "        "
		"      " ALPHA_BLOCK4 ALPHA_BLOCK4 ALPHA_BLOCK4 ALPHA_BLOCK4 ALPHA_BLOCK4 ALPHA_BLOCK4 ALPHA_BLOCK4 ALPHA_BLOCK4 ALPHA_BLOCK4 ALPHA_BLOCK4 ALPHA_BLOCK4 ALPHA_BLOCK4 "      "
		"    " ALPHA_BLOCK4 ALPHA_BLOCK4 ALPHA_BLOCK4 ALPHA_BLOCK4 ALPHA_BLOCK4 ALPHA_BLOCK4 ALPHA_BLOCK4 ALPHA_BLOCK4 ALPHA_BLOCK4 ALPHA_BLOCK4 ALPHA_BLOCK4 ALPHA_BLOCK4 ALPHA_BLOCK4 "    "
		"  " ALPHA_BLOCK4 ALPHA_BLOCK4 ALPHA_BLOCK4 ALPHA_BLOCK4 ALPHA_BLOCK4 ALPHA_BLOCK4 ALPHA_BLOCK4 ALPHA_BLOCK4 ALPHA_BLOCK4 ALPHA_BLOCK4 ALPHA_BLOCK4 ALPHA_BLOCK4 ALPHA_BLOCK4 ALPHA_BLOCK4 "  "
		ALPHA_BLOCK4 ALPHA_BLOCK4 ALPHA_BLOCK4 ALPHA_BLOCK4 ALPHA_BLOCK4 ALPHA_BLOCK4 ALPHA_BLOCK4 ALPHA_BLOCK4 ALPHA_BLOCK4 ALPHA_BLOCK4 ALPHA_BLOCK4 ALPHA_BLOCK4 ALPHA_BLOCK4 ALPHA_BLOCK4 ALPHA_BLOCK4
		"  " ALPHA_BLOCK4 ALPHA_BLOCK4 ALPHA_BLOCK4 ALPHA_BLOCK4 ALPHA_BLOCK4 ALPHA_BLOCK4 ALPHA_BLOCK4 ALPHA_BLOCK4 ALPHA_BLOCK4 ALPHA_BLOCK4 ALPHA_BLOCK4 ALPHA_BLOCK4 ALPHA_BLOCK4 ALPHA_BLOCK4 "  "
		"    " ALPHA_BLOCK4 ALPHA_BLOCK4 ALPHA_BLOCK4 ALPHA_BLOCK4 ALPHA_BLOCK4 ALPHA_BLOCK4 ALPHA_BLOCK4 ALPHA_BLOCK4 ALPHA_BLOCK4 ALPHA_BLOCK4 ALPHA_BLOCK4 ALPHA_BLOCK4 ALPHA_BLOCK4 "    "
		"      " ALPHA_BLOCK4 ALPHA_BLOCK4 ALPHA_BLOCK4 ALPHA_BLOCK4 ALPHA_BLOCK4 ALPHA_BLOCK4 ALPHA_BLOCK4 ALPHA_BLOCK4 ALPHA_BLOCK4 ALPHA_BLOCK4 ALPHA_BLOCK4 ALPHA_BLOCK4 "      "
		"        " ALPHA_BLOCK4 ALPHA_BLOCK4 ALPHA_BLOCK4 ALPHA_BLOCK4 ALPHA_BLOCK4 ALPHA_BLOCK4 ALPHA_BLOCK4 ALPHA_BLOCK4 ALPHA_BLOCK4 ALPHA_BLOCK4 ALPHA_BLOCK4 "        "
		"          " ALPHA_BLOCK4 ALPHA_BLOCK4 ALPHA_BLOCK4 ALPHA_BLOCK4 ALPHA_BLOCK4 ALPHA_BLOCK4 ALPHA_BLOCK4 ALPHA_BLOCK4 ALPHA_BLOCK4 ALPHA_BLOCK4 "          "
		"            " ALPHA_BLOCK4 ALPHA_BLOCK4 ALPHA_BLOCK4 ALPHA_BLOCK4 ALPHA_BLOCK4 ALPHA_BLOCK4 ALPHA_BLOCK4 ALPHA_BLOCK4 ALPHA_BLOCK4 "            "
		"              " ALPHA_BLOCK4 ALPHA_BLOCK4 ALPHA_BLOCK4 ALPHA_BLOCK4 ALPHA_BLOCK4 ALPHA_BLOCK4 ALPHA_BLOCK4 ALPHA_BLOCK4 "              "
		"                " ALPHA_BLOCK4 ALPHA_BLOCK4 ALPHA_BLOCK4 ALPHA_BLOCK4 ALPHA_BLOCK4 ALPHA_BLOCK4 ALPHA_BLOCK4 "                "
		"                  " ALPHA_BLOCK4 ALPHA_BLOCK4 ALPHA_BLOCK4 ALPHA_BLOCK4 ALPHA_BLOCK4 ALPHA_BLOCK4 "                  "
		"                    " ALPHA_BLOCK4 ALPHA_BLOCK4 ALPHA_BLOCK4 ALPHA_BLOCK4 ALPHA_BLOCK4 "                    "
		"                      " ALPHA_BLOCK4 ALPHA_BLOCK4 ALPHA_BLOCK4 ALPHA_BLOCK4 "                      "
		"                        " ALPHA_BLOCK4 ALPHA_BLOCK4 ALPHA_BLOCK4 "                        "
		"                          " ALPHA_BLOCK4 ALPHA_BLOCK4 "                          "
		"                            " ALPHA_BLOCK4 "                            ";
#undef ALPHA_BLOCK4

	for (int i = 0; i < AlphaShapes.Count(); i++) {
		if (!AlphaShapes[i]->IsToDelete) {
			Rect r1 = AlphaShapes[i]->DrawRect;
			r1 -= Point2D(TacticalMap->TacPixelX, TacticalMap->TacPixelY);
			r1 += TacticalRect.Top_Left();

			Rect r2 = Intersect(r1, cliprect);
			Rect r3(point.X, point.Y, ISO_TILE_PIXEL_W, ISO_TILE_PIXEL_H);
			Rect r4 = Intersect(r2, r3);

			if (r4.Is_Valid()) {
				Point2D offset = r4.Top_Left() - r3.Top_Left();

				Point2D origin = r1.Top_Left();
				ShapeSet const * shape = AlphaShapes[i]->ImageData;
				Rect shape_rect = shape->Get_Rect(0);

				int top = MAX(r4.Y, shape_rect.Y + origin.Y);
				int src_y = top - origin.Y - shape_rect.Y;
				int bottom = MIN(
					r4.Y + r4.Height,
					origin.Y + shape_rect.Height + shape_rect.Y);
				int left = MAX(r4.X, shape_rect.X + origin.X);
				int src_x = left - origin.X - shape_rect.X;
				int right = MIN(
					r4.X + r4.Width,
					origin.X + shape_rect.Width + shape_rect.X);

				int shape_skip =
					origin.X - right + shape_rect.Width
					+ src_x + shape_rect.X;
				int alpha_skip =
					AlphaBuffer->Get_Buffer_Width() - right + left;
				int mask_skip = ISO_TILE_PIXEL_W - r4.Width;

				const unsigned char * maskptr =
					&_tilemask[ISO_TILE_PIXEL_W * offset.Y + offset.X];
				unsigned char * shapeptr =
					(unsigned char *)shape->Get_Data(0);

				unsigned short * alphaptr =
					(unsigned short *)AlphaBuffer->Get_Buffer_Offset(
						Point2D(left, top - TacticalRect.Y));

				shapeptr += src_x + src_y * shape_rect.Width;
				unsigned char pixel;

				if (&alphaptr[
						right - left
						+ (bottom - top) * AlphaBuffer->Get_Buffer_Width()
						+ 2]
					>= (unsigned short *)AlphaBuffer->Get_Buffer_End()) {
					for (int i = top; i < bottom; i++) {
						for (int j = left; j < right; j++) {
							pixel = *shapeptr++;
							if (*maskptr++ != '\x20') {
								*alphaptr = BrightnessTable[pixel][*alphaptr];
							}
							alphaptr++;
							alphaptr = (unsigned short *)
										   AlphaBuffer->Wrap_Overflow((unsigned)alphaptr);
						}
						shapeptr += shape_skip;
						maskptr += mask_skip;
						alphaptr += alpha_skip;
						alphaptr = (unsigned short *)
									   AlphaBuffer->Wrap_Overflow((unsigned)alphaptr);
					}
				} else {
					for (int i = top; i < bottom; i++) {
						for (int j = left; j < right; j++) {
							pixel = *shapeptr++;
							if (*maskptr++ != '\x20') {
								*alphaptr = BrightnessTable[pixel][*alphaptr];
							}
							++alphaptr;
						}
						shapeptr += shape_skip;
						alphaptr += alpha_skip;
						maskptr += mask_skip;
					}
				}
			}
		}
	}
}


/// <summary>
/// Draws every alpha shape into the alpha buffer.
/// This routine is used by the tactical map to lay down the light contribution of all
/// registered shapes in one pass, blending each shape against whatever light has already
/// been recorded for that spot.
/// </summary>
/// <param name="cliprect">The clipping rectangle to draw within.</param>
void AlphaShapeClass::Draw_All(Rect const & cliprect) // RA2: 0x0041F5E0; RobinDigitalPro/ra2-1001-decomp's text (1.001, after ra2-decomp's Yuri's Revenge twin form) in this tree's names
{
	unsigned char pixel;

	for (int i = 0; i < AlphaShapes.Count(); i++) {
		{
			Rect r1 = AlphaShapes[i]->DrawRect;
			r1 -= Point2D(TacticalMap->TacPixelX, TacticalMap->TacPixelY);
			r1 += TacticalRect.Top_Left();

			Rect clip = cliprect;
			Rect r2 = Intersect(r1, clip);

			if (r2.Is_Valid()) {
				Point2D origin = r1.Top_Left();

				ShapeSet const * shape = AlphaShapes[i]->ImageData;
				if (shape == NULL) {
					AlphaShapes[i]->ImageData = (ShapeSet const *)AlphaShapes[i]->Owner->Class_Of()->AlphaImageData;
					shape = AlphaShapes[i]->ImageData;
				}
				Rect shape_rect = shape->Get_Rect(0);

				int top = MAX(r2.Y, origin.Y + shape_rect.Y);
				int src_y = top - origin.Y - shape_rect.Y;
				int bottom = MIN(clip.Y + clip.Height, origin.Y + shape_rect.Height + shape_rect.Y);
				int left = MAX(r2.X, origin.X + shape_rect.X);
				int src_x = left - origin.X - shape_rect.X;
				int right = MIN(clip.X + clip.Width, origin.X + shape_rect.Width + shape_rect.X);

				int shape_skip = origin.X - right + shape_rect.Width + src_x + shape_rect.X;
				int alpha_skip = AlphaBuffer->Get_Buffer_Width() - right + left;

				unsigned char * shapeptr = (unsigned char *)shape->Get_Data(0);

				unsigned short * alphaptr = (unsigned short *)AlphaBuffer->Get_Buffer_Offset(Point2D(left, top) - TacticalRect.Top_Left());

				shapeptr += src_x + src_y * shape_rect.Width;
				if (&alphaptr[right - left + (bottom - top) * AlphaBuffer->Get_Buffer_Width() + 2] >= (unsigned short *)AlphaBuffer->Get_Buffer_End()) {
					for (int i = top; i < bottom; i++) {
						for (int j = left; j < right; j++) {
							pixel = *shapeptr++;
							*alphaptr = BrightnessTable[pixel][*alphaptr];
							alphaptr++;
							alphaptr = (unsigned short *)AlphaBuffer->Wrap_Overflow((unsigned)alphaptr);
						}
						shapeptr += shape_skip;
						alphaptr += alpha_skip;
						alphaptr = (unsigned short *)AlphaBuffer->Wrap_Overflow((unsigned)alphaptr);
					}
				} else {
					for (int i = top; i < bottom; i++) {
						for (int j = left; j < right; j++) {
							pixel = *shapeptr++;
							*alphaptr = BrightnessTable[pixel][*alphaptr];
							++alphaptr;
						}
						shapeptr += shape_skip;
						alphaptr += alpha_skip;
					}
				}
			}
		}
	}
}


/// <summary>
/// Builds the alpha blending brightness table.
/// The table pairs a shape pixel with the light level already recorded in the alpha
/// buffer and yields the resulting brightness, so that the draw routines can blend with
/// a single lookup. It is built once, the first time an alpha shape is needed.
/// </summary>
void AlphaShapeClass::Calculate_Brightness_Table(void)
{
	BrightnessCalculated = true;
	for (int i = 0; i < 256*256; i++) {
		int low = i % 256;
		int high = i / 256;
		int brightness = MAX(0, MIN(255, low * high / 127));
		BrightnessTable[0][i] = brightness;
	}
}


/// <summary>
/// Draws the alpha shapes that fall across a single map tile.
/// This routine is the per cell counterpart to Draw_All. Each shape's light is blended
/// into the alpha buffer as usual, but only where it lands inside the diamond of the
/// tile being drawn.
/// </summary>
/// <param name="point">The pixel position of the tile being drawn.</param>


/// <summary>
/// Draws every alpha shape into the alpha buffer.
/// This routine is used by the tactical map to lay down the light contribution of all
/// registered shapes in one pass, blending each shape against whatever light has already
/// been recorded for that spot.
/// </summary>
