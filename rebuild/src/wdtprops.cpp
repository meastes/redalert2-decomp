/*******************************************************************************
 *                                O P E N  T S
 *******************************************************************************
 * SPDX-License-Identifier: GPL-3.0-or-later
 * Copyright 2026 OpenTS contributors
 *
 * See LICENSE.md for applicable additional terms and warranty disclaimers.
 ******************************************************************************/

#include "always.h"

#include "rect.h"
#include "geoconst1.h" // RA2: this unit holds its own copy of the geometry constants (its first initializers)

#include "globals.h" // RA2: the includes this file needs
#include "wdtnet.h"
#include "geoconst2.h" // RA2: the slope and bridge constants (after globals.h)

using namespace WorldDominationTour;


/// <summary>
/// Creates the counted descriptions of a tour state.
/// </summary>
/// <param name="tour">The tour state to describe.</param>
/// <returns>Returns with the new properties.</returns>
rc_ptr<CampaignProperties> __fastcall CampaignProperties::Create(rc_ptr<WDTState> const & tour) // RA2: a counted object, made by a static Create (0x0072D180)
{
	return(rc_ptr<CampaignProperties>(new CampaignProperties(tour)));
}


/// <summary>
/// Destroys the campaign properties.
/// The tour state they describe is shared, so only this object's hold on it is released.
/// </summary>
CampaignProperties::~CampaignProperties(void) // RA2: counted (0x0072D1C0)
{
	/// nothing
}


/// <summary>
/// Creates the descriptions of a tour state.
/// The short and long descriptions are the string table entries the ladder server named
/// for the cycle. With no tour state the campaign is described by the default entries.
/// </summary>
/// <param name="tour">The tour state to describe. It may be empty.</param>
CampaignProperties::CampaignProperties(rc_ptr<WDTState> const & tour) : // RA2: 0x0072D1F0
	TourState(tour),
	ShortDescription(tour.Get() != NULL ? StringTable::LoadString(tour->ShortDesc, NULL, "D:\\Ra2\\WDTProps.cpp", 20) : StringTable::LoadString("\"WDT:DefaultShortDescription\"", NULL, "D:\\Ra2\\WDTProps.cpp", 20)), // RA2: the default labels carry their quotes
	LongDescription(tour.Get() != NULL ? StringTable::LoadString(tour->LongDesc, NULL, "D:\\Ra2\\WDTProps.cpp", 21) : StringTable::LoadString("\"WDT:DefaultLongDescription\"", NULL, "D:\\Ra2\\WDTProps.cpp", 21))
{
	/// nothing
}


/// <summary>
/// Fetches the identifier of the tour cycle being described.
/// </summary>
/// <returns>Returns with the cycle identifier, or zero when there is no tour state.</returns>
int CampaignProperties::Get_Cycle_ID(void) const // RA2: 0x0072D290
{
	if (TourState.Get() != NULL) {
		return(TourState->CycleID);
	}
	return(0);
}


/// <summary>
/// Fetches the layout of the selection screen.
/// </summary>
/// <returns>Returns with the key into the layouts the selection screen reads; there is only one.</returns>
int CampaignProperties::Get_Layout(void) const // RA2: 0x0072D2A0
{
	return(1);
}


/// <summary>
/// Fetches the world map the tour cycle is fought over.
/// </summary>
/// <returns>Returns with the map identifier, or the first map when there is no tour state.</returns>
int CampaignProperties::Get_Map_ID(void) const // RA2: 0x0072D2B0
{
	if (TourState.Get() != NULL) {
		return(TourState->MapID);
	}
	return(1);
}


/// <summary>
/// Fetches the tick the tour cycle has reached.
/// </summary>
/// <returns>Returns with the last tick of the ownership history, or zero when there is no
/// tour state.</returns>
int CampaignProperties::Get_Current_Tick(void) const // RA2: 0x0072D2D0
{
	if (TourState.Get() != NULL) {
		return(TourState->NumTicks - 1);
	}
	return(0);
}


/// <summary>
/// Fetches the address of the web page where the standings of the cycle are published.
/// </summary>
/// <returns>Returns with the address, or an empty string when there is no tour state.</returns>
Wstring CampaignProperties::Get_Web_URL(void) const // RA2: 0x0072D2E0
{
	return(Wstring(TourState.Get() != NULL ? TourState->WebURL : ""));
}
