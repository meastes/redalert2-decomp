/*******************************************************************************
 *                                O P E N  T S
 *******************************************************************************
 * SPDX-License-Identifier: GPL-3.0-or-later
 * Copyright 2026 OpenTS contributors
 *
 * See LICENSE.md for applicable additional terms and warranty disclaimers.
 ******************************************************************************/

#include "always.h"
#include "rect.h" // RA2: the header statics, in the order of the unit's initializer table
#include "geometry.h"

#include "ccfile.h" // RA2: the click map is read through the game's file system
#include "cdfile.h"
#include "convert.h"
#include "_surface.h" // RA2: the screen surfaces
#include "ini.h"
#include "misc.h" // RA2: VideoModeWidth
#include "pcx.h"
#include "surface.h"
#include "wdtnet.h"
#include "wstring.h"

using namespace WorldDominationTour;


bool FrameAdvanceDraws; // RA2: 0x007FBD04
bool FrameRedrawDraws; // RA2: 0x007FBD05


/// <summary>
/// RA2: an inactive anim at the origin.
/// </summary>
MSFrameAnim::MSFrameAnim(void) : // RA2: an inactive anim at the origin
	MSAnim(0, 0, false) // RA2: as above
{
	//nothing
}


/// <summary>
/// RA2: sets the vtable and runs the base destructor; nothing of its own.
/// </summary>
MSFrameAnim::~MSFrameAnim(void) // RA2: sets the vtable and runs the base destructor
{
	//nothing
}


/// <summary>
/// RA2: draws the interface frame while FrameAdvanceDraws is set; the anim never ends.
/// </summary>
bool MSFrameAnim::Advance(Surface * surface, Rect & rect) // RA2: draws the interface frame while FrameAdvanceDraws is set; the anim never ends
{
	if (FrameAdvanceDraws) { // RA2: as above
		Draw_Interface_Frame(surface, rect, true); // RA2: as above
	}
	return false; // RA2: as above
}


/// <summary>
/// RA2: draws the interface frame, clipped to the rectangle (the whole surface without one), while FrameRedrawDraws is set.
/// </summary>
void MSFrameAnim::Redraw(Surface * surface, Rect const * rect) // RA2: draws the interface frame, clipped to the rectangle (the whole surface without one), while FrameRedrawDraws is set
{
	if (FrameRedrawDraws) { // RA2: as above
		int width = surface->Get_Width(); // RA2: as above
		int height = surface->Get_Height(); // RA2: as above
		if (rect != NULL) { // RA2: as above
			Rect clip = *rect; // RA2: as above
			Draw_Interface_Frame(surface, clip, true); // RA2: as above
			return; // RA2: as above
		}
		Draw_Interface_Frame(surface, Rect(0, 0, width, height), true); // RA2: as above
	}
}


/// <summary>
/// RA2: the whole 640 x 480 screen.
/// </summary>
Rect MSFrameAnim::Get_Rect(void) const // RA2: the whole 640 x 480 screen
{
	return Rect(0, 0, 640, 480); // RA2: as above
}


/// <summary>
/// RA2: a new map, counted.
/// </summary>
rc_ptr<Map> __fastcall Map::Create(int map, int faction, int player_faction) // RA2: a new map, counted
{
	return rc_ptr<Map>(new Map(map, faction, player_faction)); // RA2: as above
}


/// <summary>
/// RA2: every member cleared, the overlay prefix empty and both lists empty.
/// </summary>
Map::Map(int map, int faction, int player_faction) : // RA2: every member cleared, the overlay prefix empty and both lists empty
	MapNumber(map), // RA2: as above
	Faction(faction), // RA2: as above
	PlayerFaction(player_faction), // RA2: as above
	OverlayDrawer(0), // RA2: as above
	OpeningAnim(0), // RA2: as above
	BackgroundAnim(0), // RA2: as above
	HelpBarAnim(0), // RA2: as above
	ClickMap(0), // RA2: as above
	OverlayPrefix(""), // RA2: as above
	Territories(0, 0), // RA2: as above
	Origin(0, 0), // RA2: as above
	BackButtonDrawer(0), // RA2: as above
	Palettes(0, 0) // RA2: as above
{
	//nothing
}


/// <summary>
/// Destroys the map: the click map, both drawers and the palettes go, and the screen is cleared.
/// </summary>
Map::~Map(void) // RA2: Destroys the map: the click map, both drawers and the palettes go, and the screen is cleared
{
	delete ClickMap; // RA2: as above
	delete OverlayDrawer; // RA2: as above
	for (int i = 0; i < Palettes.Count(); i++) { // RA2: as above
		delete Palettes[i]; // RA2: as above
	}
	AlternateSurface->Fill(0); // RA2: as above
	HiddenSurface->Fill(0); // RA2: as above
	VisibleSurface->Blit_From(*HiddenSurface, false, true); // RA2: as above
	delete BackButtonDrawer; // RA2: as above
}


/// <summary>
/// RA2: reads the map's layout and animation set from WDTMap.ini.
/// </summary>
/// <param name="ini">The tour's map INI.</param>
/// <param name="engine">The presentation engine the anims are added to.</param>
/// <param name="differs">Passed on by the selection screen (whether the campaign progress differs); not used here.</param>
void Map::Load(INIClass & ini, MSEngine * engine, int differs) // RA2: reads the map's layout and animation set from WDTMap.ini
{
	Wstring map_section; // RA2: as above
	ini.Get_String("MAPS", Wstring_FromInt(MapNumber), "", map_section); // RA2: as above
	Wstring faction_section; // RA2: as above
	ini.Get_String(map_section, Faction_Name(PlayerFaction), "", faction_section); // RA2: as above
	Wstring art; // RA2: as above
	bool found = ini.Get_String(faction_section, "Art_" + Wstring_FromInt(VideoModeWidth), "", art) > 0; // RA2: as above
	if (VideoModeWidth > 800 && !found) { // RA2: as above
		ini.Get_String(faction_section, "Art_800", "", art); // RA2: as above
	}
	Point2D size(640, 400); // RA2: as above
	size = ini.Get_Point(art, "Size", size); // RA2: as above
	Point2D sidebar_size(0, 0); // RA2: as above
	sidebar_size = ini.Get_Point(art, "SideBarSize", sidebar_size); // RA2: as above
	Point2D helpbar_size(0, 0); // RA2: as above
	helpbar_size = ini.Get_Point(art, "HelpBarSize", helpbar_size); // RA2: as above
	Origin = Point2D((HiddenSurface->Get_Width() - sidebar_size.X - size.X) / 2, (HiddenSurface->Get_Height() - size.Y - helpbar_size.Y) / 2); // RA2: as above
	HelpBarOrigin = Point2D(Origin.X, Origin.Y + size.Y); // RA2: as above
	DescriptionRect = ini.Get_Rect(art, "TextRect", DescriptionRect); // RA2: as above
	DescriptionRect += Origin; // RA2: as above
	TooltipRect = ini.Get_Rect(art, "TooltipRect", TooltipRect); // RA2: as above
	TooltipRect += Origin; // RA2: as above
	PlayRect = ini.Get_Rect(art, "TitleRect", PlayRect); // RA2: as above
	PlayRect += Origin; // RA2: as above
	BackRect = ini.Get_Rect(art, "BackTextRect", BackRect); // RA2: as above
	BackRect += Origin; // RA2: as above
	Wstring value; // RA2: as above
	ini.Get_String(art, "Opening", "", value); // RA2: as above
	if (value.length() > 0) { // RA2: as above
		Rect rectangle(Origin, size.X, size.Y); // RA2: as above
		OpeningAnim = new MSBinkAnim(value, AlternateSurface, engine->Get_Anims(), rectangle, false); // RA2: as above
		engine->Add_Animation(OpeningAnim); // RA2: as above
	}
	ini.Get_String(art, "Background", "", value); // RA2: as above
	BackgroundAnim = new MSPCXAnim(value, engine->Get_Anims(), Origin, true); // RA2: as above
	if (OpeningAnim && BackgroundAnim) { // RA2: as above
		BackgroundAnim->Set_Active(false); // RA2: as above
	}
	engine->Add_Animation(BackgroundAnim); // RA2: as above
	ini.Get_String(art, "ClickMap", NULL, value); // RA2: as above
	CCFileClass file(value); // RA2: as above
	ClickMap = Read_PCX_File(file, NULL, NULL, 0); // RA2: as above
	file.Close(); // RA2: as above
	{
		MSAnim * sidebar = NULL; // RA2: as above
		ini.Get_String(art, "SideBar", "", value); // RA2: as above
		if (value.length() > 0) { // RA2: as above
			int const & width = size.X; // RA2: as above
			Point2D position(width, (HiddenSurface->Get_Height() - sidebar_size.Y) / 2); // RA2: as above
			position.X += Origin.X; // RA2: as above
			sidebar = new MSPCXAnim(value, engine->Get_Anims(), position, true); // RA2: as above
		}
		engine->Add_Animation(sidebar); // RA2: as above
	}
	ini.Get_String(art, "HelpBar", "", value); // RA2: as above
	if (value.length() > 0) { // RA2: as above
		HelpBarAnim = new MSPCXAnim(value, engine->Get_Anims(), HelpBarOrigin, true); // RA2: as above
	}
	engine->Add_Animation(HelpBarAnim); // RA2: as above
	BackButtonRect = ini.Get_Rect(art, "BackButtonRectangle", BackButtonRect); // RA2: as above
	Point2D back_origin = ini.Get_Point(art, "BackButtonOrigin", Point2D(BackButtonRect.Top_Left())); // RA2: as above
	BackButtonRect += Origin; // RA2: as above
	back_origin += Origin; // RA2: as above
	ini.Get_String(art, "BackButtonPalette", "sdbtnanm.pal", value); // RA2: as above
	delete BackButtonDrawer; // RA2: as above
	BackButtonDrawer = Create_Drawer(value); // RA2: as above
	BackFrames.Normal = ini.Get_Int(art, "BackButtonNormalFrame", BackFrames.Normal); // RA2: as above
	BackFrames.Depressed = ini.Get_Int(art, "BackButtonDepressedFrame", BackFrames.Depressed); // RA2: as above
	ini.Get_String(art, "BackButton", "", value); // RA2: as above
	if (value.length() > 0) { // RA2: as above
		BackButton.Anim = new MSOverlayAnim(value, back_origin.X, back_origin.Y, BackButtonDrawer, 0, engine->Get_Anims(), true, BackFrames.Normal, false); // RA2: as above
		engine->Add_Animation(BackButton.Anim); // RA2: as above
	}
	ini.Get_String(art, "BackButtonHighlighted", "", value); // RA2: as above
	ini.Get_String(art, "OverlayPrefix", NULL, OverlayPrefix); // RA2: as above
	ini.Get_String(art, "OverlayPalette", NULL, value); // RA2: as above
	OverlayDrawer = Create_Drawer(value); // RA2: as above
	Wstring name_prefix = Wstring("WDT:") + map_section; // RA2: as above
	ini.Get_String(faction_section, "NameKeyPrefix", name_prefix, name_prefix); // RA2: as above
	{
		ConvertClass * drawer = NULL; // RA2: as above
		if (ini.Get_String(art, "DefaultPalette", NULL, value) > 0) { // RA2: as above
			drawer = Get_Drawer(value); // RA2: as above
		}
		MSAnim * animation = Load_Anim(ini, art, "Logo", drawer); // RA2: as above
		if (animation) { // RA2: as above
			engine->Add_Animation(animation); // RA2: as above
		}
		for (int index = 0; index < 10; ++index) { // RA2: as above
			value = "Animation" + Wstring_FromInt(index); // RA2: as above
			MSAnim * anim = Load_Anim(ini, art, value, drawer); // RA2: as above
			if (anim) { // RA2: as above
				engine->Add_Animation(anim); // RA2: as above
			}
		}
	}
	for (int index = 0; index < MAX_WDT_TERRITORIES; ++index) { // RA2: as above
		if (ini.Get_String(faction_section, "Territory" + Wstring_FromInt(index, 2), NULL, value) > 0) { // RA2: as above
			rc_ptr<Territory> territory = Territory::Create(index, ini, value, OverlayPrefix, name_prefix); // RA2: as above
			Territories.Add(territory); // RA2: as above
		}
	}
	for (int owner_index = 0; owner_index < Territories.Count(); ++owner_index) { // RA2: as above
		Territories[owner_index].Get()->Create_Owner_Anim(engine, OverlayDrawer, Origin); // RA2: as above
	}
	for (int highlight_index = 0; highlight_index < Territories.Count(); ++highlight_index) { // RA2: as above
		Territories[highlight_index].Get()->Create_Highlight_Anim(engine, OverlayDrawer, Origin); // RA2: as above
	}
}


/// <summary>
/// RA2: the shape anim an INI entry "file,x,y[,rate[,palette]]" describes, placed relative to the map origin.
/// The rate is 5 and the drawer is the one given when the entry leaves them out or the palette cannot be loaded.
/// Returns NULL when the entry is missing or has no y, the file is not there, or there is no drawer.
/// </summary>
MSAnim * Map::Load_Anim(INIClass & ini, Wstring const & section, Wstring const & entry, ConvertClass * drawer) // RA2: the shape anim an INI entry "file,x,y[,rate[,palette]]" describes, placed relative to the map origin
{
	Wstring value; // RA2: as above
	if (ini.Get_String(section, entry, NULL, value) > 0) { // RA2: as above
		Wstring filename; // RA2: as above
		Wstring x; // RA2: as above
		Wstring y; // RA2: as above
		Wstring rate; // RA2: as above
		Wstring palette; // RA2: as above
		int position = value.getToken(0, ",", filename); // RA2: as above
		position = value.getToken(position, ",", x); // RA2: as above
		position = value.getToken(position, ",", y); // RA2: as above
		position = value.getToken(position, ",", rate); // RA2: as above
		value.getToken(position, ",", palette); // RA2: as above
		if (y != 0) { // RA2: as above
			int xpos = x.toInt() + Origin.X; // RA2: as above
			int ypos = y.toInt() + Origin.Y; // RA2: as above
			int frame_rate = 5; // RA2: as above
			if (rate != 0 && rate.length() > 0) { // RA2: as above
				frame_rate = rate.toInt(); // RA2: as above
			}
			ConvertClass * anim_drawer = NULL; // RA2: as above
			if (palette != 0 && palette.length() > 0) { // RA2: as above
				anim_drawer = Get_Drawer(palette); // RA2: as above
			}
			if (anim_drawer == NULL) { // RA2: as above
				anim_drawer = drawer; // RA2: as above
			}
			bool available = CCFileClass(filename).Is_Available(); // RA2: as above
			if (available && anim_drawer != NULL) { // RA2: as above
				return new MSShapeAnim(filename, xpos, ypos, anim_drawer, frame_rate, true, SHAPE_NORMAL); // RA2: as above
			}
		}
	}
	return NULL; // RA2: as above
}


/// <summary>
/// RA2: the drawer of a palette file, loaded and kept on first use. Out of line at both call sites.
/// </summary>
#pragma auto_inline(off)
ConvertClass * Map::Get_Drawer(Wstring const & name) // RA2: the drawer of a palette file, loaded and kept on first use. Out of line at both call sites
{
	for (int i = 0; i < Palettes.Count(); i++) { // RA2: as above
		if (Palettes[i]->Name == name) { // RA2: as above
			return Palettes[i]->Drawer; // RA2: as above
		}
	}
	ConvertClass * drawer = Create_Drawer(name); // RA2: as above
	if (drawer) { // RA2: as above
		Palettes.Add(new AnimationPalette(name, drawer)); // RA2: as above
	}
	return drawer; // RA2: as above
}
#pragma auto_inline(on)


/// <summary>
/// RA2: 0x0072A5E0 is the constructor of a name and a number, out of line, which nothing calls. It has the shape of
/// AnimationPalette's constructor (a Wstring copy, then the second member), which is expanded in Get_Drawer, so the
/// copy is made here by a type of its own.
/// </summary>
struct NamedValue
{
	NamedValue(Wstring const & name, int value);

	Wstring Name; // RA2: +0x00
	int Value; // RA2: +0x04
};

NamedValue::NamedValue(Wstring const & name, int value) : // RA2: 0x0072A5E0 is the constructor of a name and a number, out of line, which nothing calls. It has the shape of
	Name(name), // RA2: as above
	Value(value) // RA2: as above
{
	//nothing
}


/// <summary>
/// RA2: deletes the drawer.
/// </summary>
Map::AnimationPalette::~AnimationPalette(void) // RA2: deletes the drawer
{
	delete Drawer; // RA2: as above
}


/// <summary>
/// RA2: with an opening movie, says the first voice list and waits for the movie (at most 300000 ms); then shows the background.
/// </summary>
void Map::Start_Intro(MSEngine * engine, Voices * voices) // RA2: with an opening movie, says the first voice list and waits for the movie (at most 300000 ms); then shows the background
{
	if (OpeningAnim) { // RA2: as above
		voices->Say_List(0, true, 0); // RA2: as above
		engine->Wait_For_Anim(OpeningAnim, 300000); // RA2: as above
	}
	if (BackgroundAnim) { // RA2: as above
		BackgroundAnim->Set_Active(true); // RA2: as above
	}
}


/// <summary>
/// RA2: the number the click map holds under a screen point, -1 outside it.
/// </summary>
int Map::Territory_At(Point2D const & point) const // RA2: the number the click map holds under a screen point, -1 outside it
{
	Point2D local = point; // RA2: as above
	local.X -= Origin.X; // RA2: as above
	local.Y -= Origin.Y; // RA2: as above
	if (local.X >= 0 && local.X < ClickMap->Get_Width() && local.Y >= 0 && local.Y < ClickMap->Get_Height()) { // RA2: as above
		return ClickMap->Get_Pixel(local); // RA2: as above
	}
	return -1; // RA2: as above
}


/// <summary>
/// RA2: when the state changes, shows the back button's frame for it and has the engine redraw.
/// </summary>
void Map::Set_Back_Hot(MSEngine * engine, bool hot) // RA2: when the state changes, shows the back button's frame for it and has the engine redraw
{
	if (BackFrames.IsHot != hot) { // RA2: as above
		BackFrames.IsHot = hot; // RA2: as above
		BackButton.Anim->Set_Frame(BackFrames.IsHot && BackButton.IsDown ? BackFrames.Depressed : BackFrames.Normal); // RA2: as above
		engine->Broadcast(&BackButtonRect); // RA2: as above
		engine->Redraw(); // RA2: as above
	}
}


/// <summary>
/// RA2: as Set_Back_Hot, for whether the button is down.
/// </summary>
void Map::Set_Back_Down(MSEngine * engine, bool down) // RA2: as Set_Back_Hot, for whether the button is down
{
	if (BackButton.IsDown != down) { // RA2: as above
		BackButton.IsDown = down; // RA2: as above
		BackButton.Anim->Set_Frame(BackFrames.IsHot && BackButton.IsDown ? BackFrames.Depressed : BackFrames.Normal); // RA2: as above
		engine->Broadcast(&BackButtonRect); // RA2: as above
		engine->Redraw(); // RA2: as above
	}
}


/// <summary>
/// RA2: whether a point is inside the back button's rectangle.
/// </summary>
int Map::Is_Over_Back(Point2D const & point) const // RA2: whether a point is inside the back button's rectangle
{
	if (point.X >= BackButtonRect.X && point.X < BackButtonRect.Width + BackButtonRect.X) { // RA2: as above
		if (point.Y >= BackButtonRect.Y && point.Y < BackButtonRect.Height + BackButtonRect.Y) { // RA2: as above
			return 1; // RA2: as above
		}
	}
	return 0; // RA2: as above
}


/// <summary>
/// RA2: no points yet.
/// </summary>
Centroid::Centroid(void) : // RA2: no points yet
	Position(0, 0), // RA2: as above
	Count(0) // RA2: as above
{
	//nothing
}


/// <summary>
/// RA2: adds a point to the sums and counts it.
/// </summary>
Centroid const & Centroid::operator+=(Point2D const & pt) // RA2: adds a point to the sums and counts it
{
	Position.X += pt.X; // RA2: as above
	Position.Y += pt.Y; // RA2: as above
	Count++; // RA2: as above
	return *this; // RA2: as above
}


/// <summary>
/// RA2: the mean of the points added, (0, 0) when there are none.
/// </summary>
Point2D Centroid::Center_Point(void) const // RA2: the mean of the points added, (0, 0) when there are none
{
	return Count > 0 ? Point2D(Position.X / Count, Position.Y / Count) : Point2D(0, 0); // RA2: as above
}


/// <summary>
/// RA2: the sums and counts are the same.
/// </summary>
bool Centroid::operator==(Centroid const & that) const // RA2: the sums and counts are the same
{
	return Position == that.Position && Count == that.Count; // RA2: as above
}


/// <summary>
/// RA2: the point as the INI class takes it, a copy made by value.
/// </summary>
static inline Point2D To_Point(Point2D point) // RA2: the point as the INI class takes it, a copy made by value
{
	return Point2D(point.X, point.Y); // RA2: as above
}


/// <summary>
/// RA2: the territory a click map color stands for: colors 0 to 29, and 255 (no territory) for any other.
/// </summary>
unsigned __fastcall Territory_Of_Color(unsigned color) // RA2: 0x0072A870, the executable also has this out of line, though no function calls it; WDT_Write_Map_Ini tests the color with the same two comparisons
{
	return color > 29 ? 255 : color; // RA2: as above
}


/// <summary>
/// RA2: a development tool that no code calls. It scans a click map, finds each territory's bounding rectangle and
/// center point (colors 0 to 29), and writes the center points to WDTMap.ini, next to the game and as ..\code\wdtmap.ini,
/// as the Target_(width) entries of the sections (prefix)(NN).
/// </summary>
/// <param name="prefix">The prefix of the territory sections.</param>
/// <param name="filename">The PCX file of the click map.</param>
void __fastcall WDT_Write_Map_Ini(Wstring const & prefix, Wstring const & filename) // RA2: a development tool that no code calls. It scans a click map, finds each territory's bounding rectangle and
{
	Wstring __fastcall operator+(Wstring const & left, Wstring const & right); // RA2: 0x0076F7D0, the free join of two strings (wstring.h has only the member), declared here because file scope makes the other joins in this file ambiguous
	CDFileClass file; // RA2: as above
	file.Open("WDTMap.ini", FileClass::READ); // RA2: as above
	INIClass ini; // RA2: as above
	ini.Load(file, false); // RA2: as above
	file.Close(); // RA2: as above

	CDFileClass pcx(filename); // RA2: as above
	Surface * surface = Read_PCX_File(pcx, NULL, NULL, 0); // RA2: as above
	int height = surface->Get_Height(); // RA2: as above
	int width = surface->Get_Width(); // RA2: as above

	VectorClass<Rect> rects(30); // RA2: as above
	VectorClass<Centroid> centroids(30); // RA2: as above

	VectorCursor<Rect, VectorClass<Rect> > cursor(rects); // RA2: as above
	for (; cursor; ++cursor) { // RA2: as above
		(*cursor).Set(0, 0, 0, 0); // RA2: as above
	}

	Point2D point; // RA2: as above
	for (point.Y = 0; point.Y < height; point.Y++) { // RA2: as above
		for (point.X = 0; point.X < width; point.X++) { // RA2: as above
			unsigned index = surface->Get_Pixel(point); // RA2: as above
			if (index <= 29 && index < 255) { // RA2: as above
				if (centroids.Length() < index + 1) { // RA2: as above
					centroids.Resize(index + 1); // RA2: as above
				}
				centroids[index] += point; // RA2: as above
				if (rects.Length() < index + 1) { // RA2: as above
					int count = rects.Length(); // RA2: as above
					rects.Resize(index + 1); // RA2: as above
					VectorCursor<Rect, VectorClass<Rect> > fresh(rects, count); // RA2: as above
					for (; fresh; ++fresh) { // RA2: as above
						(*fresh).Set(0, 0, 0, 0); // RA2: as above
					}
				}
				Rect & rect = rects[index]; // RA2: as above
				if (rect.Width == 0) { // RA2: as above
					rect.Height = 0; // RA2: as above
				}
				if (!rect.Is_Point_Within(point)) { // RA2: as above
					rect = Union(rect, Rect(point, 1, 1)); // RA2: as above
				}
			}
		}
	}

	{
		VectorCursor<Rect, VectorClass<Rect> > reset(rects, 0); // RA2: as above
		cursor = reset; // RA2: as above
	}
	int i = 0; // RA2: as above
	while (cursor) { // RA2: as above
		Wstring section = operator+(prefix, Wstring_FromInt(i, 2)); // RA2: the free join, prefix in edx
		Wstring number = Wstring_FromInt(VideoModeWidth); // RA2: as above
		ini.Put_Point(section, Wstring("Target_").operator+(number), To_Point(centroids[i].Center_Point())); // RA2: as above
		++cursor; // RA2: as above
		i++; // RA2: as above
	}

	file.Open("WDTMap.ini", FileClass::WRITE); // RA2: as above
	ini.Save(file); // RA2: as above
	file.Open("..\\code\\wdtmap.ini", FileClass::WRITE); // RA2: as above
	ini.Save(file); // RA2: as above
}
