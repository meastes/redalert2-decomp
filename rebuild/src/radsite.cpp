/*******************************************************************************
 *                                O P E N  T S
 *******************************************************************************
 * SPDX-License-Identifier: GPL-3.0-or-later
 * Copyright 2026 OpenTS contributors
 *
 * Contains material derived from Electronic Arts source code.
 * Modified by OpenTS contributors, 2026.
 * EA's GPLv3 Section 7 additional terms and supplemental warranty
 * disclaimers apply; see LICENSE.md.
 ******************************************************************************/

/*
**	RA2: the radiation sites (RadSiteClass) that radioactive weapons leave on the map: each site raises the
**	radiation level of the cells around its base cell, lets it decay over time and lights the area green.
**	TibSun has no such file; the code is the executable's from 0x00637030.
*/

#define INCLUDE_COM // RA2: IStream
#include "geometry.h" // RA2: the geometry constants come before the rectangle of rect.h (the executable initializes them first)
#include "always.h" // RA2: the includes this file needs
#include "_map.h" // RA2: the includes this file needs
#include "_rules.h" // RA2: the includes this file needs
#include "cell.h" // RA2: the includes this file needs
#include "crc.h" // RA2: the includes this file needs
#include "globals.h" // RA2: the includes this file needs
#include "light.h" // RA2: the includes this file needs
#include "map.h" // RA2: the includes this file needs
#include "noinit.h" // RA2: the includes this file needs
#include "radsite.h" // RA2: the includes this file needs
#include "rect.h" // RA2: the includes this file needs
#include "rules.h" // RA2: the includes this file needs
#include "scenario.h" // RA2: the includes this file needs
#include "swizzle.h" // RA2: the includes this file needs
#include "tracker.h" // RA2: the includes this file needs
#include "vector.h" // RA2: the includes this file needs



DynamicVectorClass<RadSiteClass *> RadSites; // RA2: 0x00AB6620; every RadSiteClass object, added by its constructor and removed by its destructor


/// <summary>
/// Makes a radiation site that has no position and no radiation yet.
/// The site lists itself in the list of all radiation sites.
/// </summary>
RadSiteClass::RadSiteClass(void) : // RA2: 0x006372A0
	BASECLASS(), // RA2: as above
	LightSource(NULL), // RA2: no light until the site is activated
	RadLevelTimer(), // RA2: both timers start counting from the current frame
	RadLightTimer(), // RA2: as above
	BaseCell(0, 0), // RA2: as above
	Spread(0), // RA2: as above
	SpreadInLeptons(0), // RA2: as above
	RadLevel(0), // RA2: as above
	LevelSteps(0), // RA2: as above
	Intensity(0), // RA2: as above
	IntensitySteps(0), // RA2: as above
	IntensityDecrement(0), // RA2: as above
	RadDuration(0), // RA2: as above
	RadTimeLeft(0) // RA2: as above
{
	RadSites.Add(this); // RA2: the site registers itself in the list of every radiation site
}


/// <summary>
/// Creates a radiation site for the save game loader.
/// The object is filled in afterwards from the stream, so only the timers are made.
/// </summary>
/// <param name="x">The no init marker.</param>
RadSiteClass::RadSiteClass(NoInitClass const & x) : // RA2: 0x00637360
	BASECLASS(x), // RA2: as above
	RadLevelTimer(), // RA2: the timers are rebuilt as new ones
	RadLightTimer() // RA2: as above
{
}


/// <summary>
/// Destroys the site.
/// The radiation site mark of its base cell is cleared, everything that points at the site is
/// detached, its light is deleted and the site leaves the list of all radiation sites.
/// </summary>
RadSiteClass::~RadSiteClass(void) // RA2: 0x006373B0
{
	CellClass & cell = Map[BaseCell]; // RA2: the base cell no longer belongs to this site
	cell.Set_Rad_Site(NULL); // RA2: as above
	Detach_This_From_All(this, true); // RA2: as above
	if (LightSource != NULL) { // RA2: the light goes away with the site
		delete LightSource; // RA2: as above
		LightSource = NULL; // RA2: as above
	}
	RadSites.Delete(this); // RA2: the site leaves the list of every radiation site
}


/// <summary>
/// Fetches the size of this object.
/// </summary>
/// <returns>Returns with the size of a RadSiteClass.</returns>
int RadSiteClass::Fetch_Object_Size(void) const // RA2: 0x00637460
{
	return(sizeof(*this)); // RA2: as above
}


/// <summary>
/// Submits this object to the multiplayer sync check.
/// </summary>
/// <param name="crc">The checksum engine to submit the object state to.</param>
void RadSiteClass::Compute_CRC(CRCEngine & crc) const // RA2: 0x00637470
{
	BASECLASS::Compute_CRC(crc); // RA2: nothing beyond the base class takes part
}


/// <summary>
/// Fetches the run time type of this object.
/// </summary>
/// <returns>Returns with RTTI_RADSITE.</returns>
RTTIType RadSiteClass::Fetch_RTTI(void) const // RA2: 0x00637480
{
	return(RTTI_RADSITE); // RA2: the kind is 0x45
}


/// <summary>
/// Loads this site from the save game stream.
/// The object is rebuilt in place and the light pointer is handed to the swizzler.
/// </summary>
/// <param name="stream">The stream to load from.</param>
/// <returns>Returns with S_OK on success.</returns>
HRESULT STDMETHODCALLTYPE RadSiteClass::Load(IStream * stream) // RA2: 0x00637490
{
	HRESULT result = BASECLASS::Load(stream); // RA2: the base class reads its part of the stream first
	if (SUCCEEDED(result)) { // RA2: as above
		new (this) RadSiteClass(NoInitClass()); // RA2: restores the vtables over the loaded data
		Swizzle_Pointer(&LightSource); // RA2: as above
	}
	return(result); // RA2: as above
}


/// <summary>
/// Saves this site to the save game stream.
/// </summary>
/// <param name="stream">The stream to save to.</param>
/// <param name="cleardirty">Whether the dirty flag is cleared by the save.</param>
/// <returns>Returns with S_OK on success.</returns>
HRESULT STDMETHODCALLTYPE RadSiteClass::Save(IStream * stream, BOOL cleardirty) // RA2: 0x00637510
{
	return(BASECLASS::Save(stream, cleardirty)); // RA2: nothing beyond the base class is saved
}


/// <summary>
/// Fetches the class identifier used to persist this object.
/// </summary>
/// <param name="retval">Pointer to the buffer that will receive the class identifier.</param>
/// <returns>Returns with S_OK, or E_POINTER if no buffer was supplied.</returns>
HRESULT STDMETHODCALLTYPE RadSiteClass::GetClassID(CLSID * retval) // RA2: 0x00637530
{
	if (retval == NULL) { // RA2: as above
		return(E_POINTER); // RA2: as above
	}
	*retval = CLSID_RadSiteClass; // RA2: as above
	return(S_OK); // RA2: as above
}


/// <summary>
/// Fetches how far the radiation reaches, in cells.
/// </summary>
int RadSiteClass::GetSpread(void) // RA2: 0x00637570
{
	return(Spread); // RA2: as above
}


/// <summary>
/// Sets the cell the radiation spreads from.
/// </summary>
/// <param name="cell">The center cell.</param>
void RadSiteClass::SetBaseCell(Cell * cell) // RA2: 0x00637580
{
	BaseCell = *cell; // RA2: as above
}


/// <summary>
/// Sets how far the radiation reaches, in cells; the distance in leptons reaches to the middle of the outermost cell.
/// </summary>
/// <param name="spread">The spread in cells.</param>
void RadSiteClass::SetSpread(int spread) // RA2: 0x00637590
{
	Spread = spread; // RA2: as above
	SpreadInLeptons = spread * 256 + 128; // RA2: the spread in cells times the 256 leptons of a cell, plus half a cell
}


/// <summary>
/// Sets the radiation level; the site lasts the level times the rules' duration multiple.
/// </summary>
/// <param name="level">The radiation level.</param>
void RadSiteClass::SetRadLevel(int level) // RA2: 0x006375B0
{
	RadLevel = level; // RA2: as above
	RadDuration = Rule->RadDurationMultiple * level; // RA2: the rules' RadDurationMultiple times the level
	RadTimeLeft = RadDuration; // RA2: the whole duration is left
}


/// <summary>
/// Fetches the current radiation level: the level scaled by the time that is left.
/// </summary>
/// <returns>Returns with the current level, 0 when the site has no duration.</returns>
int RadSiteClass::GetRadLevel(void) // RA2: 0x006375D0
{
	if (RadDuration <= 0) { // RA2: no duration: no level (also guards the division)
		return(0); // RA2: as above
	}
	return(RadTimeLeft * RadLevel / RadDuration); // RA2: integer product first, then the division
}


/// <summary>
/// Adds radiation to the site.
/// The current level is taken off the cells, the site restarts at the current level plus the new amount and its
/// effect is applied again.
/// </summary>
/// <param name="level">The radiation level to add.</param>
void RadSiteClass::Add(int level) // RA2: 0x006375F0
{
	double current = RadLevel; // RA2: the level now, in floating point
	double left = RadTimeLeft; // RA2: as above
	double duration = RadDuration; // RA2: as above
	int now = (int)(current * left / duration); // RA2: as above
	Deactivate(); // RA2: takes the old effect off the cells
	SetRadLevel(now + level); // RA2: as above
	Activate(); // RA2: applies the new one
}


/// <summary>
/// Starts the site.
/// The timers are set from the rules, the light color and brightness are derived from the level and the rules' light
/// and tint factors (snow is tinted differently), the light is created or changed and the radiation is put on the cells.
/// </summary>
void RadSiteClass::Activate(void) // RA2: 0x00637640
{
	int leveldelay = Rule->RadLevelDelay; // RA2: the frames between two decay steps of the cells
	int lightdelay = Rule->RadLightDelay; // RA2: the frames between two fade steps of the light
	RadLevelTimer = leveldelay; // RA2: the timers count down these frames
	RadLightTimer = lightdelay; // RA2: as above

	int green = Rule->RadColor.Get_Green(); // RA2: the rules' color, read green first
	int blue = Rule->RadColor.Get_Blue(); // RA2: as above
	int red = Rule->RadColor.Get_Red(); // RA2: as above
	if (Scen->Theater == THEATER_SNOW) { // RA2: on snow the red and blue parts are dropped
		red = blue = -128; // RA2: as above
	}
	int r = red * 1000 / 255; // RA2: the color in thousandths
	int g = green * 1000 / 255; // RA2: as above
	int b = blue * 1000 / 255; // RA2: as above
	int visibility = SpreadInLeptons; // RA2: the light reaches as far as the radiation
	double light = RadLevel; // RA2: the brightness, at most 2000
	light *= Rule->RadLightFactor; // RA2: as above
	light = (2000.0 < light) ? 2000.0 : light; // RA2: as above
	int intensity = (int)light; // RA2: as above
	double tr = r * Rule->RadTintFactor; // RA2: the tint of each color, at most 2000
	tr = (2000.0 < tr) ? 2000.0 : tr; // RA2: as above
	int tintr = (int)tr; // RA2: as above
	double tg = g * Rule->RadTintFactor; // RA2: as above
	tg = (2000.0 < tg) ? 2000.0 : tg; // RA2: as above
	int tintg = (int)tg; // RA2: as above
	double tb = b * Rule->RadTintFactor; // RA2: as above
	tb = (2000.0 < tb) ? 2000.0 : tb; // RA2: as above
	int tintb = (int)tb; // RA2: as above

	Tint.Red = tintr; // RA2: kept, so that the light can fade
	Tint.Green = tintg; // RA2: as above
	Tint.Blue = tintb; // RA2: as above
	Intensity = intensity; // RA2: as above
	LevelSteps = RadDuration / leveldelay; // RA2: the number of steps in which the radiation decays
	IntensitySteps = RadDuration / lightdelay; // RA2: the number of steps in which the light fades
	IntensityDecrement = intensity / IntensitySteps; // RA2: as above

	Coord coord = Map[BaseCell].Center_Coord(); // RA2: the light sits in the middle of the base cell
	if (LightSource == NULL) { // RA2: the first activation makes the light
		LightSource = new LightSourceClass(coord, visibility, intensity, tintr, tintg, tintb); // RA2: as above
		LightSource->DetailLevel = 0; // RA2: the light is lit at once, at detail level 0
		LightSource->Enable(false); // RA2: as above
	} else {
		LightSource->ChangeLevels(intensity, tintr, tintg, tintb, false); // RA2: a later one (Add) only changes its levels
	}
	Radiate(); // RA2: puts the radiation on the cells
}


/// <summary>
/// Counts the site down.
/// Every RadLevelDelay frames the radiation of the cells decays, every RadLightDelay frames the light fades, and the
/// site deletes itself when its time is up.
/// </summary>
void RadSiteClass::AI(void) // RA2: 0x006378C0
{
	RadTimeLeft--; // RA2: one frame less to live
	if (RadLevelTimer.Value() == 0) { // RA2: the level timer ran out: one decay step of the cells, then the timer restarts
		DecreaseRadiation(); // RA2: as above
		RadLevelTimer = Rule->RadLevelDelay; // RA2: as above
	}
	if (RadLightTimer.Value() == 0) { // RA2: the light timer ran out: one fade step of the light, then the timer restarts
		DecreaseLight(); // RA2: as above
		RadLightTimer = Rule->RadLightDelay; // RA2: as above
	}
	if (RadTimeLeft <= 0) { // RA2: the site is used up
		delete this; // RA2: through the virtual destructor (scalar deleting destructor at 0x00637F90)
	}
}


/// <summary>
/// Fetches the radiation level of the site at a cell.
/// The level falls off linearly from the base cell to nothing at the edge of the spread, in leptons.
/// </summary>
/// <param name="cell">The cell.</param>
/// <returns>Returns with the level at the cell, 0 outside of the spread.</returns>
double RadSiteClass::GetRadLevelAt(Cell const & cell) // RA2: 0x006379B0
{
	CellClass & here = Map[BaseCell]; // RA2: the distance runs between the centers of the base cell and of the cell
	CellClass & there = Map[cell]; // RA2: as above
	Coord a = here.Center_Coord(); // RA2: as above
	Coord b = there.Center_Coord(); // RA2: as above
	int distance = a.Distance_To(b); // RA2: as above
	int range = SpreadInLeptons; // RA2: as above
	double result = 0.0; // RA2: outside of the spread there is no radiation
	if (distance <= range) { // RA2: as above
		result = (double)(range - distance) / range * RadLevel; // RA2: linear falloff to nothing at the edge
	}
	return(result); // RA2: as above
}


/// <summary>
/// Puts the radiation of the site on every cell of the square around the base cell.
/// </summary>
void RadSiteClass::Radiate(void) // RA2: 0x00637A80
{
	int x0 = BaseCell.X - Spread; // RA2: the square of the spread around the base cell
	int y0 = BaseCell.Y - Spread; // RA2: as above
	int xend = x0 + Spread * 2 + 1; // RA2: as above
	int yend = y0 + Spread * 2 + 1; // RA2: as above
	for (int y = y0; y < yend; y++) { // RA2: as above
		for (int x = x0; x < xend; x++) { // RA2: as above
			Cell cell(x, y); // RA2: as above
			double level = GetRadLevelAt(cell); // RA2: as above
			CellClass & target = Map[cell]; // RA2: as above
			target.RadLevel_Increase(level); // RA2: as above
		}
	}
}


/// <summary>
/// Takes the remaining radiation of the site off the cells of the square around the base cell: the level of each cell is
/// scaled by the decay steps that are left.
/// </summary>
void RadSiteClass::Deactivate(void) // RA2: 0x00637C10
{
	int steps = RadTimeLeft / Rule->RadLevelDelay + 1; // RA2: the decay steps that are left, counting the one that is running
	int x0 = BaseCell.X - Spread; // RA2: as above
	int y0 = BaseCell.Y - Spread; // RA2: as above
	int xend = x0 + Spread * 2 + 1; // RA2: as above
	int yend = y0 + Spread * 2 + 1; // RA2: as above
	for (int y = y0; y < yend; y++) { // RA2: as above
		for (int x = x0; x < xend; x++) { // RA2: as above
			Cell cell(x, y); // RA2: as above
			double level = GetRadLevelAt(cell); // RA2: as above
			level = level / LevelSteps * steps; // RA2: as above
			CellClass & target = Map[cell]; // RA2: as above
			target.RadLevel_Decrease(level); // RA2: as above
		}
	}
}


/// <summary>
/// Takes one decay step off the radiation of the cells of the square around the base cell.
/// </summary>
void RadSiteClass::DecreaseRadiation(void) // RA2: 0x00637DC0
{
	int x0 = BaseCell.X - Spread; // RA2: as above
	int y0 = BaseCell.Y - Spread; // RA2: as above
	int xend = x0 + Spread * 2 + 1; // RA2: as above
	int yend = y0 + Spread * 2 + 1; // RA2: as above
	for (int y = y0; y < yend; y++) { // RA2: as above
		for (int x = x0; x < xend; x++) { // RA2: as above
			Cell cell(x, y); // RA2: as above
			double level = GetRadLevelAt(cell); // RA2: as above
			level /= LevelSteps; // RA2: as above
			CellClass & target = Map[cell]; // RA2: as above
			target.RadLevel_Decrease(level); // RA2: as above
		}
	}
}


/// <summary>
/// Fades the light of the site by one step: the brightness goes down by the decrement and the tint is scaled by the
/// time that is left.
/// </summary>
void RadSiteClass::DecreaseLight(void) // RA2: 0x00637F50
{
	LightSource->ChangeLevels(LightSource->Intensity - IntensityDecrement, Tint.Red * RadTimeLeft / RadDuration, Tint.Green * RadTimeLeft / RadDuration, Tint.Blue * RadTimeLeft / RadDuration, false); // RA2: the light's own intensity less the decrement; each tint part scaled by the share of the time that is left
}
