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


bool FrameAdvanceDraws = true; // RA2: 0x007FBD04
bool FrameRedrawDraws = true; // RA2: 0x007FBD05


/// <summary>
/// RA2: an inactive anim at the origin.
/// </summary>
MSFrameAnim::MSFrameAnim(void) : // RA2: an inactive anim at the origin
	MSAnim(0, 0, false)
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
	if (FrameAdvanceDraws) {
		Draw_Interface_Frame(surface, rect, true);
	}
	return false;
}


/// <summary>
/// RA2: draws the interface frame, clipped to the rectangle (the whole surface without one), while FrameRedrawDraws is set.
/// </summary>
void MSFrameAnim::Redraw(Surface * surface, Rect const * rect) // RA2: draws the interface frame, clipped to the rectangle (the whole surface without one), while FrameRedrawDraws is set
{
	if (FrameRedrawDraws) {
		int width = surface->Get_Width();
		int height = surface->Get_Height();
		if (rect != NULL) {
			Rect clip = *rect;
			Draw_Interface_Frame(surface, clip, true);
			return;
		}
		Draw_Interface_Frame(surface, Rect(0, 0, width, height), true);
	}
}


/// <summary>
/// RA2: the whole 640 x 480 screen.
/// </summary>
Rect MSFrameAnim::Get_Rect(void) const // RA2: the whole 640 x 480 screen
{
	return Rect(0, 0, 640, 480);
}


/// <summary>
/// RA2: a new map, counted.
/// </summary>
rc_ptr<Map> __fastcall Map::Create(int map, int faction, int player_faction) // RA2: a new map, counted
{
	return rc_ptr<Map>(new Map(map, faction, player_faction));
}


/// <summary>
/// RA2: every member cleared, the overlay prefix empty and both lists empty.
/// </summary>
Map::Map(int map, int faction, int player_faction) : // RA2: every member cleared, the overlay prefix empty and both lists empty
	MapNumber(map),
	Faction(faction),
	PlayerFaction(player_faction),
	OverlayDrawer(0),
	OpeningAnim(0),
	BackgroundAnim(0),
	HelpBarAnim(0),
	ClickMap(0),
	OverlayPrefix(""),
	Territories(0, 0),
	Origin(0, 0),
	BackButtonDrawer(0),
	Palettes(0, 0)
{
	//nothing
}


/// <summary>
/// Destroys the map: the click map, both drawers and the palettes go, and the screen is cleared.
/// </summary>
Map::~Map(void) // RA2: Destroys the map: the click map, both drawers and the palettes go, and the screen is cleared
{
	delete ClickMap;
	delete OverlayDrawer;
	for (int i = 0; i < Palettes.Count(); i++) {
		delete Palettes[i];
	}
	AlternateSurface->Fill(0);
	HiddenSurface->Fill(0);
	VisibleSurface->Blit_From(*HiddenSurface, false, true);
	delete BackButtonDrawer;
}


/// <summary>
/// RA2: reads the map's layout and animation set from WDTMap.ini.
/// </summary>
/// <param name="ini">The tour's map INI.</param>
/// <param name="engine">The presentation engine the anims are added to.</param>
/// <param name="differs">Passed on by the selection screen (whether the campaign progress differs); not used here.</param>
void Map::Load(INIClass & ini, MSEngine * engine, bool differs) // RA2: reads the map's layout and animation set from WDTMap.ini
{
	Wstring map_section;
	ini.Get_String("MAPS", Wstring_FromInt(MapNumber), "", map_section);
	Wstring faction_section;
	ini.Get_String(map_section, Faction_Name(PlayerFaction), "", faction_section);
	Wstring art;
	bool found = ini.Get_String(faction_section, "Art_" + Wstring_FromInt(VideoModeWidth), "", art) > 0;
	if (VideoModeWidth > 800 && !found) {
		ini.Get_String(faction_section, "Art_800", "", art);
	}
	Point2D size(640, 400);
	size = ini.Get_Point(art, "Size", size);
	Point2D sidebar_size(0, 0);
	sidebar_size = ini.Get_Point(art, "SideBarSize", sidebar_size);
	Point2D helpbar_size(0, 0);
	helpbar_size = ini.Get_Point(art, "HelpBarSize", helpbar_size);
	Origin = Point2D((HiddenSurface->Get_Width() - sidebar_size.X - size.X) / 2, (HiddenSurface->Get_Height() - size.Y - helpbar_size.Y) / 2);
	HelpBarOrigin = Point2D(Origin.X, Origin.Y + size.Y);
	DescriptionRect = ini.Get_Rect(art, "TextRect", DescriptionRect);
	DescriptionRect += Origin;
	TooltipRect = ini.Get_Rect(art, "TooltipRect", TooltipRect);
	TooltipRect += Origin;
	PlayRect = ini.Get_Rect(art, "TitleRect", PlayRect);
	PlayRect += Origin;
	BackRect = ini.Get_Rect(art, "BackTextRect", BackRect);
	BackRect += Origin;
	Wstring value;
	ini.Get_String(art, "Opening", "", value);
	if (value.length() > 0) {
		Rect rectangle(Origin, size.X, size.Y);
		OpeningAnim = new MSBinkAnim(value, AlternateSurface, engine->Get_Anims(), rectangle, false);
		engine->Add_Animation(OpeningAnim);
	}
	ini.Get_String(art, "Background", "", value);
	BackgroundAnim = new MSPCXAnim(value, engine->Get_Anims(), Origin, true);
	if (OpeningAnim && BackgroundAnim) {
		BackgroundAnim->Set_Active(false);
	}
	engine->Add_Animation(BackgroundAnim);
	ini.Get_String(art, "ClickMap", NULL, value);
	CCFileClass file(value);
	ClickMap = Read_PCX_File(file, NULL, NULL, 0);
	file.Close();
	{
		MSAnim * sidebar = NULL;
		ini.Get_String(art, "SideBar", "", value);
		if (value.length() > 0) {
			int const & width = size.X;
			Point2D position(width, (HiddenSurface->Get_Height() - sidebar_size.Y) / 2);
			position.X += Origin.X;
			sidebar = new MSPCXAnim(value, engine->Get_Anims(), position, true);
		}
		engine->Add_Animation(sidebar);
	}
	ini.Get_String(art, "HelpBar", "", value);
	if (value.length() > 0) {
		HelpBarAnim = new MSPCXAnim(value, engine->Get_Anims(), HelpBarOrigin, true);
	}
	engine->Add_Animation(HelpBarAnim);
	BackButtonRect = ini.Get_Rect(art, "BackButtonRectangle", BackButtonRect);
	Point2D back_origin = ini.Get_Point(art, "BackButtonOrigin", Point2D(BackButtonRect.Top_Left()));
	BackButtonRect += Origin;
	back_origin += Origin;
	ini.Get_String(art, "BackButtonPalette", "sdbtnanm.pal", value);
	delete BackButtonDrawer;
	BackButtonDrawer = Create_Drawer(value);
	BackFrames.Normal = ini.Get_Int(art, "BackButtonNormalFrame", BackFrames.Normal);
	BackFrames.Depressed = ini.Get_Int(art, "BackButtonDepressedFrame", BackFrames.Depressed);
	ini.Get_String(art, "BackButton", "", value);
	if (value.length() > 0) {
		BackButton.Anim = new MSOverlayAnim(value, back_origin.X, back_origin.Y, BackButtonDrawer, 0, engine->Get_Anims(), true, BackFrames.Normal, false);
		engine->Add_Animation(BackButton.Anim);
	}
	ini.Get_String(art, "BackButtonHighlighted", "", value);
	ini.Get_String(art, "OverlayPrefix", NULL, OverlayPrefix);
	ini.Get_String(art, "OverlayPalette", NULL, value);
	OverlayDrawer = Create_Drawer(value);
	Wstring name_prefix = Wstring("WDT:") + map_section;
	ini.Get_String(faction_section, "NameKeyPrefix", name_prefix, name_prefix);
	{
		ConvertClass * drawer = NULL;
		if (ini.Get_String(art, "DefaultPalette", NULL, value) > 0) {
			drawer = Get_Drawer(value);
		}
		MSAnim * animation = Load_Anim(ini, art, "Logo", drawer);
		if (animation) {
			engine->Add_Animation(animation);
		}
		for (int index = 0; index < 10; ++index) {
			value = "Animation" + Wstring_FromInt(index);
			MSAnim * anim = Load_Anim(ini, art, value, drawer);
			if (anim) {
				engine->Add_Animation(anim);
			}
		}
	}
	for (int index = 0; index < MAX_WDT_TERRITORIES; ++index) {
		if (ini.Get_String(faction_section, "Territory" + Wstring_FromInt(index, 2), NULL, value) > 0) {
			rc_ptr<Territory> territory = Territory::Create(index, ini, value, OverlayPrefix, name_prefix);
			Territories.Add(territory);
		}
	}
	for (int owner_index = 0; owner_index < Territories.Count(); ++owner_index) {
		Territories[owner_index]->Create_Owner_Anim(engine, OverlayDrawer, Origin);
	}
	for (int highlight_index = 0; highlight_index < Territories.Count(); ++highlight_index) {
		Territories[highlight_index]->Create_Highlight_Anim(engine, OverlayDrawer, Origin);
	}
}


/// <summary>
/// RA2: the shape anim an INI entry "file,x,y[,rate[,palette]]" describes, placed relative to the map origin.
/// The rate is 5 and the drawer is the one given when the entry leaves them out or the palette cannot be loaded.
/// Returns NULL when the entry is missing or has no y, the file is not there, or there is no drawer.
/// </summary>
MSAnim * Map::Load_Anim(INIClass & ini, Wstring const & section, Wstring const & entry, ConvertClass * drawer) // RA2: the shape anim an INI entry "file,x,y[,rate[,palette]]" describes, placed relative to the map origin
{
	Wstring value;
	if (ini.Get_String(section, entry, NULL, value) > 0) {
		Wstring filename;
		Wstring x;
		Wstring y;
		Wstring rate;
		Wstring palette;
		int position = value.getToken(0, ",", filename);
		position = value.getToken(position, ",", x);
		position = value.getToken(position, ",", y);
		position = value.getToken(position, ",", rate);
		value.getToken(position, ",", palette);
		if (y != 0) {
			int xpos = x.toInt() + Origin.X;
			int ypos = y.toInt() + Origin.Y;
			int frame_rate = 5;
			if (rate != 0 && rate.length() > 0) {
				frame_rate = rate.toInt();
			}
			ConvertClass * anim_drawer = NULL;
			if (palette != 0 && palette.length() > 0) {
				anim_drawer = Get_Drawer(palette);
			}
			if (anim_drawer == NULL) {
				anim_drawer = drawer;
			}
			bool available = CCFileClass(filename).Is_Available();
			if (available && anim_drawer != NULL) {
				return new MSShapeAnim(filename, xpos, ypos, anim_drawer, frame_rate, true, SHAPE_NORMAL);
			}
		}
	}
	return NULL;
}


/// <summary>
/// RA2: the drawer of a palette file, loaded and kept on first use. Out of line at both call sites.
/// </summary>
#pragma auto_inline(off)
ConvertClass * Map::Get_Drawer(Wstring const & name) // RA2: the drawer of a palette file, loaded and kept on first use. Out of line at both call sites
{
	for (int i = 0; i < Palettes.Count(); i++) {
		if (Palettes[i]->Name == name) {
			return Palettes[i]->Drawer;
		}
	}
	ConvertClass * drawer = Create_Drawer(name);
	if (drawer) {
		Palettes.Add(new AnimationPalette(name, drawer));
	}
	return drawer;
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
	Name(name),
	Value(value)
{
	//nothing
}


/// <summary>
/// RA2: deletes the drawer.
/// </summary>
Map::AnimationPalette::~AnimationPalette(void) // RA2: deletes the drawer
{
	delete Drawer;
}


/// <summary>
/// RA2: with an opening movie, says the first voice list and waits for the movie (at most 300000 ms); then shows the background.
/// </summary>
void Map::Start_Intro(MSEngine * engine, Voices * voices) // RA2: with an opening movie, says the first voice list and waits for the movie (at most 300000 ms); then shows the background
{
	if (OpeningAnim) {
		voices->Say_List(0, true, 0);
		engine->Wait_For_Anim(OpeningAnim, 300000);
	}
	if (BackgroundAnim) {
		BackgroundAnim->Set_Active(true);
	}
}


/// <summary>
/// RA2: the number the click map holds under a screen point, -1 outside it.
/// </summary>
int Map::Territory_At(Point2D const & point) const // RA2: the number the click map holds under a screen point, -1 outside it
{
	Point2D local = point;
	local.X -= Origin.X;
	local.Y -= Origin.Y;
	if (local.X >= 0 && local.X < ClickMap->Get_Width() && local.Y >= 0 && local.Y < ClickMap->Get_Height()) {
		return ClickMap->Get_Pixel(local);
	}
	return -1;
}


/// <summary>
/// RA2: when the state changes, shows the back button's frame for it and has the engine redraw.
/// </summary>
void Map::Set_Back_Hot(MSEngine * engine, bool hot) // RA2: when the state changes, shows the back button's frame for it and has the engine redraw
{
	if (BackFrames.IsHot != hot) {
		BackFrames.IsHot = hot;
		BackButton.Anim->Set_Frame(BackFrames.IsHot && BackButton.IsDown ? BackFrames.Depressed : BackFrames.Normal);
		engine->Broadcast(&BackButtonRect);
		engine->Redraw();
	}
}


/// <summary>
/// RA2: as Set_Back_Hot, for whether the button is down.
/// </summary>
void Map::Set_Back_Down(MSEngine * engine, bool down) // RA2: as Set_Back_Hot, for whether the button is down
{
	if (BackButton.IsDown != down) {
		BackButton.IsDown = down;
		BackButton.Anim->Set_Frame(BackFrames.IsHot && BackButton.IsDown ? BackFrames.Depressed : BackFrames.Normal);
		engine->Broadcast(&BackButtonRect);
		engine->Redraw();
	}
}


/// <summary>
/// RA2: whether a point is inside the back button's rectangle.
/// </summary>
int Map::Is_Over_Back(Point2D const & point) const // RA2: whether a point is inside the back button's rectangle
{
	if (point.X >= BackButtonRect.X && point.X < BackButtonRect.Width + BackButtonRect.X) {
		if (point.Y >= BackButtonRect.Y && point.Y < BackButtonRect.Height + BackButtonRect.Y) {
			return 1;
		}
	}
	return 0;
}


/// <summary>
/// RA2: no points yet.
/// </summary>
Centroid::Centroid(void) : // RA2: no points yet
	Position(0, 0),
	Count(0)
{
	//nothing
}


/// <summary>
/// RA2: adds a point to the sums and counts it.
/// </summary>
Centroid const & Centroid::operator+=(Point2D const & pt) // RA2: adds a point to the sums and counts it
{
	Position.X += pt.X;
	Position.Y += pt.Y;
	Count++;
	return *this;
}


/// <summary>
/// RA2: the mean of the points added, (0, 0) when there are none.
/// </summary>
Point2D Centroid::Center_Point(void) const // RA2: the mean of the points added, (0, 0) when there are none
{
	return Count > 0 ? Point2D(Position.X / Count, Position.Y / Count) : Point2D(0, 0);
}


/// <summary>
/// RA2: the sums and counts are the same.
/// </summary>
bool Centroid::operator==(Centroid const & that) const // RA2: the sums and counts are the same
{
	return Position == that.Position && Count == that.Count;
}


/// <summary>
/// RA2: the point as the INI class takes it, a copy made by value.
/// </summary>
static inline Point2D To_Point(Point2D point) // RA2: the point as the INI class takes it, a copy made by value
{
	return Point2D(point.X, point.Y);
}


/// <summary>
/// RA2: the territory a click map color stands for: colors 0 to 29, and 255 (no territory) for any other.
/// </summary>
unsigned __fastcall Territory_Of_Color(unsigned color) // RA2: 0x0072A870, the executable also has this out of line, though no function calls it; WDT_Write_Map_Ini tests the color with the same two comparisons
{
	return color > 29 ? 255 : color;
}


/// <summary>
/// RA2: a development tool that no code calls. It scans a click map, finds each territory's bounding rectangle and
/// center point (colors 0 to 29), and writes the center points to WDTMap.ini, next to the game and as ..\code\wdtmap.ini,
/// as the Target_(width) entries of the sections (prefix)(NN).
/// </summary>
/// <param name="prefix">The prefix of the territory sections.</param>
/// <param name="filename">The PCX file of the click map.</param>
#ifdef RA2_NON_MATCHING // not exact: 7 bytes differ (one lea operand order in the expanded VectorClass<Rect>::Resize; the VectorClass<Centroid> vtable then binds; VectorCursor::operator= at 0x72AF50 matches with plain member copies)
void __fastcall WDT_Write_Map_Ini(Wstring const & prefix, Wstring const & filename) // RA2: a development tool that no code calls. It scans a click map, finds each territory's bounding rectangle and
{
	Wstring __fastcall operator+(Wstring const & left, Wstring const & right); // RA2: 0x0076F7D0, the free join of two strings (wstring.h has only the member), declared here because file scope makes the other joins in this file ambiguous
	CDFileClass file;
	file.Open("WDTMap.ini", FileClass::READ);
	INIClass ini;
	ini.Load(file, false);
	file.Close();

	CDFileClass pcx(filename);
	Surface * surface = Read_PCX_File(pcx, NULL, NULL, 0);
	int height = surface->Get_Height();
	int width = surface->Get_Width();

	VectorClass<Rect> rects(30);
	VectorClass<Centroid> centroids(30);

	VectorCursor<Rect, VectorClass<Rect>> cursor(rects);
	for (; cursor; ++cursor) {
		(*cursor).Set(0, 0, 0, 0);
	}

	Point2D point;
	for (point.Y = 0; point.Y < height; point.Y++) {
		for (point.X = 0; point.X < width; point.X++) {
			unsigned index = surface->Get_Pixel(point);
			if (index <= 29 && index < 255) {
				if (centroids.Length() < index + 1) {
					centroids.Resize(index + 1);
				}
				centroids[index] += point;
				if (rects.Length() < index + 1) {
					int count = rects.Length();
					rects.Resize(index + 1);
					VectorCursor<Rect, VectorClass<Rect>> fresh(rects, count);
					for (; fresh; ++fresh) {
						(*fresh).Set(0, 0, 0, 0);
					}
				}
				Rect & rect = rects[index];
				if (rect.Width == 0) {
					rect.Height = 0;
				}
				if (!rect.Is_Point_Within(point)) {
					rect = Union(rect, Rect(point, 1, 1));
				}
			}
		}
	}

	{
		VectorCursor<Rect, VectorClass<Rect>> reset(rects, 0);
		cursor = reset;
	}
	int i = 0;
	while (cursor) {
		Wstring section = operator+(prefix, Wstring_FromInt(i, 2)); // RA2: the free join, prefix in edx
		Wstring number = Wstring_FromInt(VideoModeWidth);
		ini.Put_Point(section, Wstring("Target_").operator+(number), To_Point(centroids[i].Center_Point()));
		++cursor;
		i++;
	}

	file.Open("WDTMap.ini", FileClass::WRITE);
	ini.Save(file);
	file.Open("..\\code\\wdtmap.ini", FileClass::WRITE);
	ini.Save(file);
}
#endif
