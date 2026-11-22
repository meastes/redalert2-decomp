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

#include "ccfile.h"
#include "ini.h"
#include "wdtnet.h"
#include "wstring.h" // RA2: the includes this file needs

using namespace WorldDominationTour;


/// <summary>
/// Fetches the cycle ID recorded for a campaign.
/// Use this routine to peek at the tour history file without having to build a Campaign
/// object first.
/// </summary>
/// <param name="section">The name of the campaign to look up.</param>
/// <returns>Returns with the recorded campaign ID, or -1 if the history file holds no
/// entry for that campaign.</returns>
int __fastcall WDT_Get_Campaign_ID(Wstring const & section) // RA2: __fastcall (0x00725DE0); nothing in the executable calls it
{
	CDFileClass file("WDTHist.INI");
	if (file.Is_Available()) {
		INIClass ini;
		ini.Load(file);
		file.Close();
		Wstring key("CampaignID"); // RA2: the Wstring overload of Get_Int, the key is a Wstring made on the stack
		return(ini.Get_Int(section, key, -1)); // RA2: as above
	}
	return(-1);
}


/// <summary>
/// Makes a new counted campaign.
/// The campaign needs the tour's properties; without them an empty pointer comes back.
/// </summary>
/// <param name="name">The name of the campaign. This doubles as its history file section.</param>
/// <param name="props">The properties and current world state of the tour.</param>
/// <returns>Returns with the new campaign, or an empty pointer when there are no properties.</returns>
rc_ptr<Campaign> __fastcall Campaign::Create(Wstring const & name, rc_ptr<CampaignProperties> const & props) // RA2: the 0x00725EE0 function every counted class has; the constructor takes counted properties
{
	if (props.Get() == NULL) { // RA2: as above
		return rc_ptr<Campaign>(); // RA2: as above
	}
	return rc_ptr<Campaign>(new Campaign(name, props)); // RA2: as above
}


/// <summary>
/// Constructor for a World Domination Tour campaign.
/// This routine gathers a conflict for every territory currently in dispute, then picks up
/// where the player left off by reading the cycle, the last day seen by each faction and
/// the chosen side out of the tour history file. A campaign whose recorded cycle no longer
/// matches the tour's is treated as a fresh start rather than a continuation.
/// </summary>
/// <param name="name">The name of the campaign. This doubles as its history file section.</param>
/// <param name="props">The properties and current world state of the tour.</param>
Campaign::Campaign(Wstring const & name, rc_ptr<CampaignProperties> const & props) : // RA2: counted properties and a Wstring name (0x00725F40)
	Properties(props),
	CycleHistory(History::Create(props->TourState)), // RA2: the history of the tour state is a counted object
	Conflicts(4),
	PlayerFaction(0), // RA2: a Wstring Name member follows
	Name(name) // RA2: the name is a Wstring member
{
	memset(LastDay, 0, sizeof(LastDay)); // RA2: one last day per faction instead of a single previous tick
	if (props->TourState.Get() != NULL) { // RA2: as above
		rc_ptr<State> state = CycleHistory->Get_State(props->Get_Current_Tick()); // RA2: a counted state
		unsigned int terrnum = props->TourState->NumTerritories; // RA2: as above
		while (terrnum--) {
			if (state->Owner_Of(terrnum) == 1) { // RA2: Owner_Of 1 is a disputed territory
				DynamicVectorClass<rc_ptr<WDTTerritory> > & territories = props->TourState->Territories; // RA2: counted territories
				rc_ptr<WDTTerritory> territory = terrnum < (unsigned)territories.Count() ? territories[terrnum] : rc_ptr<WDTTerritory>(); // RA2: a copy, empty past the end
				if (territory.Get() != NULL) { // RA2: as above
					rc_ptr<Conflict> conflict = Conflict::Create(territory); // RA2: conflicts are counted and made by Create
					Conflicts.Add(conflict); // RA2: as above
				}
			}
		}
	}

	CDFileClass file("WDTHist.INI");
	if (file.Is_Available()) {
		INIClass ini;
		ini.Load(file);
		file.Close();
		int cycle_id; // RA2: as above
		{
			Wstring key("CampaignID"); // RA2: the Wstring overload of Get_Int, the key is built on the stack
			cycle_id = ini.Get_Int(Name, key, -1); // RA2: as above
		}
		CycleID = cycle_id < 0 ? 0 : cycle_id;
		if (CycleID == props->Get_Cycle_ID()) { // RA2: as above
			for (int faction = 0; faction < 4; faction++) { // RA2: every faction has its own last day, saved as LastDay<faction name>
				Wstring key = Wstring("LastDay") + Faction_Code(faction); // RA2: as above
				int day = ini.Get_Int(Name, key, 0); // RA2: as above
				LastDay[faction] = day < 0 ? 0 : day; // RA2: as above
			}
			{
				Wstring key("PlayerFaction"); // RA2: as above
				PlayerFaction = ini.Get_Int(Name, key, PlayerFaction); // RA2: as above
			}
		} else {
			memset(LastDay, 0, sizeof(LastDay)); // RA2: a fresh start forgets every faction's last day
		}
	}
}


/// <summary>
/// Destructor for a World Domination Tour campaign.
/// </summary>
Campaign::~Campaign(void)
{

}


/// <summary>
/// Has the tour started a new cycle?
/// The recorded cycle is the one this campaign last saw; the properties carry the cycle
/// the tour is running now.
/// </summary>
/// <returns>bool; Has the tour moved on to a cycle this campaign has not seen?</returns>
bool Campaign::Is_Different_Cycle(void) const // RA2: const (0x00726350)
{
	int cycle = Properties->Get_Cycle_ID(); // RA2: the properties are a counted pointer; the value is read into a local first
	return(CycleID != cycle); // RA2: as above
}


/// <summary>
/// Has the tour advanced since this faction last saw it?
/// The tour screen uses this routine to decide whether the world map needs bringing up to
/// date before the player is shown it.
/// </summary>
/// <param name="faction">The faction whose last day is compared.</param>
/// <returns>bool; Is either the cycle or the faction's last day out of date?</returns>
bool Campaign::Is_Different(int faction) const // RA2: const, one last day per faction (0x00726370)
{
	int cycle = Properties->Get_Cycle_ID(); // RA2: not a call of Is_Different_Cycle, the test is expanded
	if (CycleID != cycle) { // RA2: as above
		return(true); // RA2: as above
	}
	cycle = Properties->Get_Current_Tick(); // RA2: as above
	return(LastDay[faction] != cycle ? true : false); // RA2: as above
}


/// <summary>
/// Fetches the tick this faction last saw the tour at.
/// </summary>
/// <param name="faction">The faction to look up.</param>
/// <returns>Returns with the faction's last recorded day.</returns>
int Campaign::Get_Last_Day(int faction) const // RA2: 0x007263A0
{
	return(LastDay[faction]); // RA2: as above
}


/// <summary>
/// Records the faction's progress in the tour history file.
/// This routine is called once the player has caught up with the tour, so that the next
/// session knows which cycle and which tick the faction last saw.
/// </summary>
/// <param name="faction">The faction whose progress is saved.</param>
void Campaign::Save_Progress(int faction) // RA2: the day is saved per faction (0x007263B0, TibSun's Write_INI)
{
	CycleID = Properties->Get_Cycle_ID(); // RA2: as above
	LastDay[faction] = Properties->Get_Current_Tick(); // RA2: one last day per faction
	INIClass ini;
	Load_INI(ini);
	Wstring key = Wstring("LastDay") + Faction_Code(faction); // RA2: the entry is named after the faction
	{
		Wstring id_key("CampaignID"); // RA2: the Wstring overload of Put_Int
		ini.Put_Int(Name, id_key, CycleID, 0); // RA2: as above
	}
	ini.Put_Int(Name, key, LastDay[faction], 0); // RA2: as above
	CDFileClass file;
	file.Open("WDTHist.ini", FileClass::WRITE);
	ini.Save(file);
}


/// <summary>
/// Clears the campaign's recorded progress.
/// Use this routine to start the campaign over. The cycle and every faction's last day are
/// zeroed in the tour history file.
/// </summary>
void Campaign::Reset_Progress(void) // RA2: 0x00726580, TibSun's Reset_INI; nothing in the executable calls it
{
	INIClass ini;
	Load_INI(ini);
	{
		Wstring key("CampaignID"); // RA2: the Wstring overload of Put_Int
		ini.Put_Int(Name, key, 0, 0); // RA2: as above
	}
	for (int faction = 0; faction < 4; faction++) { // RA2: every faction's last day is cleared
		Wstring key = Wstring("LastDay") + Faction_Code(faction); // RA2: as above
		ini.Put_Int(Name, key, 0, 0); // RA2: as above
	}
	CDFileClass file;
	file.Open("WDTHist.ini", FileClass::WRITE);
	ini.Save(file);
}


/// <summary>
/// Fetches an iterator to the conflict over a territory.
/// The tour screen uses this routine when the player picks a territory, to reach the
/// conflict that will be fought over it.
/// </summary>
/// <param name="index">The territory to find the conflict for.</param>
/// <returns>Returns with an iterator positioned on the conflict. An exhausted iterator is
/// returned if that territory is not in dispute.</returns>
CONFLICT_ITER Campaign::Get_Conflict_Iterator(int index) // RA2: 0x00726730
{
	CONFLICT_ITER it = Conflicts;
	for (; it; it++) {
		if (it.Cur_Ref()->Get_Territory_Index() == index) { // RA2: the conflicts are counted pointers
			break;
		}
	}
	return(it);
}


/// <summary>
/// Sets the side the player is fighting for.
/// The choice is written straight out to the tour history file so that it survives into
/// the next session. Choosing the side the player is already on does nothing at all.
/// </summary>
/// <param name="faction">The side the player has chosen to fight for.</param>
void Campaign::Set_Faction(int faction) // RA2: 0x007267C0
{
	if (PlayerFaction != faction) {
		PlayerFaction = faction;
		INIClass ini;
		Load_INI(ini);
		{
			Wstring key("PlayerFaction"); // RA2: the Wstring overload of Put_Int
			ini.Put_Int(Name, key, PlayerFaction, 0); // RA2: as above
		}
		CDFileClass file;
		file.Open("WDTHist.ini", FileClass::WRITE);
		ini.Save(file);
	}
}


/// <summary>
/// Loads the tour history file into an INI database.
/// This routine is used by the routines that record campaign progress. The history file is
/// created empty when it is missing, so the caller always ends up with a usable database.
/// </summary>
/// <param name="ini">The INI database to load the history into.</param>
void Campaign::Load_INI(INIClass & ini) // RA2: 0x00726930; the callers expand it, its own copy stays unreferenced
{
	CDFileClass file;
	file.Open("WDTHist.ini");
	if (!file.Is_Available()) {
		file.Create();
		file.Close();
		file.Open("WDTHist.ini");
	}
	ini.Load(file);
	file.Close();
}
