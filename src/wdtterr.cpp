/*******************************************************************************
 *                                O P E N  T S
 *******************************************************************************
 * SPDX-License-Identifier: GPL-3.0-or-later
 * Copyright 2026 OpenTS contributors
 *
 * See LICENSE.md for applicable additional terms and warranty disclaimers.
 ******************************************************************************/

#include "always.h" // RA2: the includes this file needs

#include "rect.h"
#include "geoconst1.h" // RA2: this unit holds its own copy of the geometry constants (the first initializer, at 0x00730990, is RECT_NONE from rect.h)

#include "ccfile.h" // RA2: the shape of a territory is looked up through the game's file system
#include "convert.h" // RA2: the includes this file needs
#include "globals.h" // RA2: the includes this file needs
#include "ini.h" // RA2: the includes this file needs
#include "misc.h" // RA2: VideoModeWidth
#include "msengine.h" // RA2: the includes this file needs
#include "stringtable.h" // RA2: the territory name comes from the string table
#include "vector.h" // RA2: the includes this file needs
#include "wdtnet.h" // RA2: the includes this file needs
#include "wstring.h" // RA2: the includes this file needs
#include "geoconst2.h" // RA2: the slope and bridge constants (after globals.h)

using namespace WorldDominationTour;


Wstring __fastcall operator+(Wstring const & left, Wstring const & right); // RA2: 0x0076F7D0, the prefix and the number joined


/// <summary>
/// Creates a counted territory from a section of the map INI.
/// </summary>
/// <param name="index">The number of the territory.</param>
/// <param name="ini">The map INI.</param>
/// <param name="section">The section that holds the territory's points.</param>
/// <param name="shape_prefix">The start of the shape file name of the territory's overlays.</param>
/// <param name="name_prefix">The start of the string table label of the territory's name.</param>
/// <returns>Returns with the new territory.</returns>
rc_ptr<Territory> __fastcall Territory::Create(int index, INIClass const & ini, Wstring const & section, Wstring const & shape_prefix, Wstring const & name_prefix) // RA2: a counted territory, made by a static Create (0x00730B80)
{
	return(rc_ptr<Territory>(new Territory(index, ini, section, shape_prefix, name_prefix)));
}


/// <summary>
/// Creates a territory of the map.
/// The name comes from the string table, the shape file from the prefix and the number,
/// and the target and origin points from the INI entries of the video mode's width.
/// </summary>
/// <param name="id">The number of the territory.</param>
/// <param name="ini">The map INI.</param>
/// <param name="section">The section that holds the territory's points.</param>
/// <param name="shape_prefix">The start of the shape file name of the territory's overlays.</param>
/// <param name="name_prefix">The start of the string table label of the territory's name.</param>
Territory::Territory(int id, INIClass const & ini, Wstring const & section, Wstring const & shape_prefix, Wstring const & name_prefix) : // RA2: 0x00730BE0
	ID(id), // RA2: the number is stored first, as the executable does
	Target(0, 0), // RA2: the points start at zero, set before the shape name is made
	Origin(0, 0),
	OwnerAnim(NULL),
	HighlightAnim(NULL),
	TargetAnim(NULL)
{
	ShapeName = operator+(shape_prefix, Wstring_FromInt(id, 2)) + ".shp"; // RA2: the overlay shape is the prefix, the number in two digits and ".shp"
	Wstring label = operator+(name_prefix, Wstring_FromInt(id, 2)); // RA2: the string table label is the prefix and the number in two digits
	Name = StringTable::LoadString(label.get(), NULL, "D:\\Ra2\\WDTTerr.cpp", 39); // RA2: RA2 string table text (label, source path and line as the executable passes them)
	Wstring width = Wstring_FromInt(VideoModeWidth); // RA2: the points are keyed by the width of the video mode
	Target = ini.Get_Point(section, "Target_" + width, Target);
	Origin = ini.Get_Point(section, "Origin_" + width, Origin);
	if (VideoModeWidth > 800) { // RA2: wider modes fall back to the points of the 800 mode
		if (Target.X == 0 && Target.Y == 0) {
			Wstring key("Target_800");
			Target = ini.Get_Point(section, key, Target);
		}
		if (Origin.X == 0 && Origin.Y == 0) {
			Wstring key("Origin_800");
			Origin = ini.Get_Point(section, key, Origin);
		}
	}
}


/// <summary>
/// Creates the animation that shows who owns the territory.
/// Nothing is created when the territory's shape file does not exist.
/// </summary>
/// <param name="engine">The engine that runs the animation.</param>
/// <param name="drawer">The palette the shape is drawn with.</param>
/// <param name="offset">The map's position on the screen.</param>
void Territory::Create_Owner_Anim(MSEngine * engine, ConvertClass * drawer, Point2D const & offset) // RA2: 0x00730E00
{
	int x = Origin.X + offset.X; // RA2: the animation sits at the territory's origin on the map
	int y = Origin.Y + offset.Y;
	CCFileClass file(ShapeName);
	if (file.Is_Available()) {
		OwnerAnim = new MSOverlayAnim(ShapeName, x, y, drawer, 0, engine->Get_Anims(), true, -1, false); // RA2: no frame is shown until an owner is set
		OwnerAnim->Set_Active(false);
		engine->Add_Animation(OwnerAnim);
	}
}


/// <summary>
/// Creates the animation that highlights the territory.
/// Nothing is created when the territory's shape file does not exist.
/// </summary>
/// <param name="engine">The engine that runs the animation.</param>
/// <param name="drawer">The palette the shape is drawn with.</param>
/// <param name="offset">The map's position on the screen.</param>
void Territory::Create_Highlight_Anim(MSEngine * engine, ConvertClass * drawer, Point2D const & offset) // RA2: 0x00730EC0
{
	int x = Origin.X + offset.X; // RA2: the animation sits at the territory's origin on the map
	int y = Origin.Y + offset.Y;
	CCFileClass file(ShapeName);
	if (file.Is_Available()) {
		HighlightAnim = new MSOverlayAnim(ShapeName, x, y, drawer, 0, engine->Get_Anims(), true, 0, false); // RA2: frame 0 is the highlight
		HighlightAnim->Set_Active(false);
		engine->Add_Animation(HighlightAnim);
	}
}


/// <summary>
/// Destroys the territory.
/// </summary>
Territory::~Territory(void) // RA2: counted (0x00730F80)
{
	//nothing
}


/// <summary>
/// Shows which side owns the territory.
/// The overlay's frame follows the owner, the screen area of the old and the new frame
/// is redrawn, and the engine runs one step.
/// </summary>
/// <param name="state">The owner: 1 (disputed), 2 or 3 (the two sides), anything else hides the overlay.</param>
/// <param name="engine">The engine that runs the animation.</param>
/// <param name="wait">Not used.</param>
void Territory::Set_Owner_State(int state, MSEngine * engine, bool wait) // RA2: 0x00730FB0
{
	if (OwnerAnim != NULL) { // RA2: nothing to show without a shape
		int frame = -1; // RA2: no frame hides the overlay
		switch (state) { // RA2: the owner picks the frame: 1 gives 3, 2 gives 2, 3 gives 1
			case 1:
				frame = 3;
				break;
			case 2:
				frame = 2;
				break;
			case 3:
				frame = 1;
				break;
		}
		Rect oldrect = OwnerAnim->Get_Rect(); // RA2: the area the old frame covered
		if (frame >= 0) {
			OwnerAnim->Set_Frame(frame);
			OwnerAnim->Set_Active(true);
			OwnerAnim->Restart();
		} else {
			OwnerAnim->Set_Active(false);
		}
		Rect newrect = OwnerAnim->Get_Rect();
		Rect rect = Union(newrect, oldrect);
		engine->Restore_Anims(rect);
		engine->Restore_And_Advance();
		engine->Wait_Delay(1);
	}
}


/// <summary>
/// Turns the territory's highlight on or off.
/// </summary>
/// <param name="active">True to show the highlight.</param>
/// <param name="engine">The engine that runs the animation.</param>
void Territory::Set_Highlight(bool active, MSEngine * engine) // RA2: 0x00731120
{
	if (HighlightAnim != NULL) {
		HighlightAnim->Set_Active(active);
		Rect rect = HighlightAnim->Get_Rect();
		engine->Restore_Anims(rect);
		engine->Restore_And_Advance();
		engine->Wait_For_Anim(HighlightAnim, 300000);
	}
}


/// <summary>
/// Remembers the animation of the target marker.
/// </summary>
/// <param name="anim">The animation.</param>
void Territory::Set_Target_Anim(MSAnim * anim) // RA2: 0x00731190
{
	TargetAnim = anim;
}


/// <summary>
/// Takes the animation of the target marker away from the territory.
/// </summary>
/// <returns>Returns with the animation, NULL when there was none.</returns>
MSAnim * Territory::Remove_Target_Anim(void) // RA2: 0x007311A0
{
	MSAnim * anim = TargetAnim;
	TargetAnim = NULL;
	return(anim);
}


/// <summary>
/// Finds a territory by its number.
/// </summary>
/// <param name="territories">The territories to search.</param>
/// <param name="index">The number to look for.</param>
/// <returns>Returns with the territory, empty when none has the number.</returns>
rc_ptr<Territory> __fastcall WorldDominationTour::Find_Territory(TERRITORY_LIST & territories, int index) // RA2: 0x007311B0
{
	TERRITORY_ITER cursor(territories); // RA2: the vector accessors are expanded inline in this unit
	while (cursor && (*cursor)->ID != index) {
		++cursor;
	}
	return(cursor ? *cursor : rc_ptr<Territory>());
}
