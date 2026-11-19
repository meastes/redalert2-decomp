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

#include "wdtnet.h"

#include "packet.h"
#include "session.h"

#include <cstdio>
#include <new>

#define PEEK(data) (*((unsigned char*)(data)))
#define PEEK_AT(data, pos) (*((unsigned char*)(data) + pos))

/// <summary>
/// A battle setting byte of the tour packet with its high bit dropped.
/// </summary>
static inline unsigned char WDT_Unbias(unsigned char const &value) // RA2: no such routine in TibSun, whose triples treat a byte above 0x7F as the default
{
	return(value > 0x7F ? value - 0x80 : value); // RA2: as above
}







/// <summary>
/// Makes a new territory record, counted.
/// </summary>
/// <returns>The counted pointer to the new territory.</returns>
rc_ptr<WDTTerritory> __fastcall WDTTerritory::Create(void) // RA2: 0x0072BDC0; no such routine in TibSun, which makes its territories with new
{
	return(rc_ptr<WDTTerritory>(new WDTTerritory())); // RA2: as above
}


/// <summary>
/// Creates a territory record carrying default settings.
/// This routine gives the map generator settings and the game option ranges workable values
/// for the case where the ladder server supplies none of its own.
/// </summary>
WDTTerritory::WDTTerritory(void)
{
	Index = 0;
	UnitCount = 10;
	TechLevel = 10;
	StartingCredits = 10000;
	Seed = 0;
	Width = 1;
	Height = 1;
	NumPlayers = 2;
	MapType = 1; // RA2: the map type is +0x1C, a byte, one here (TibSun: Biome 3)
	Theater = 0; // RA2: the byte after it, which TibSun does not have
	Time = 0;
	CliffsMin = 0;
	CliffsMax = 100;
	CliffsDefault = 50;
	AccessibilityMin = 0;
	AccessibilityMax = 100;
	AccessibilityDefault = 50;
	HillsMin = 0;
	HillsMax = 100;
	HillsDefault = 50;
	WaterMin = 0;
	WaterMax = 100;
	WaterDefault = 50;
	TiberiumAmountMin = 1;
	TiberiumAmountMax = 100;
	TiberiumAmountDefault = 50;
	TiberiumFieldsMin = 0;
	TiberiumFieldsMax = 100;
	TiberiumFieldsDefault = 50;
	VegetationMin = 0;
	VegetationMax = 100;
	VegetationDefault = 50;
	CitiesMin = 0;
	CitiesMax = 100;
	CitiesDefault = 50;
	Booleans = -1;
	UserModBooleans = -1;
}


/// <summary>
/// Destroys the territory record.
/// </summary>
WDTTerritory::~WDTTerritory(void)
{
	//nothing
}





/// <summary>
/// Fetches the game options this territory is fought under.
/// This routine is used to set up the rules for the conflict over this territory. Options
/// the tour never varies are forced to their tour values rather than taken from the
/// territory.
/// </summary>
/// <returns>Returns with a newly allocated option block. The caller is responsible for
/// deleting it.</returns>
/// <param name="data">The raw packet received from the ladder server.</param>
/// <returns>The counted pointer to the new state.</returns>
rc_ptr<WDTState> __fastcall WDTState::Create(void *data) // RA2: 0x0072BEB0; no such routine in TibSun
{
	return(rc_ptr<WDTState>(new WDTState(data))); // RA2: as above
}


/// <summary>
/// Creates an empty campaign state.
/// This routine is used when no tour data has been received, so the state is left holding
/// no territories and no ownership history.
/// </summary>
WDTState::WDTState(void)
{
	MapID=0;
	CycleID=0;
	NumTerritories=0;
	NumTicks=0;
	TickTime=1000000;
	Territories = DynamicVectorClass<rc_ptr<WDTTerritory> >(); // RA2: the list is reset by assigning an empty one (TibSun: NULL)
	OwnerHistory = NULL;
}


/// <summary>
/// Creates a campaign state from a World Domination Tour packet.
/// This routine is used to unpack the tour state that the ladder server sends. The
/// descriptions, the cycle identifiers, the tick timing, the territory records and the
/// ownership history all come out of the packet fields. A packet reporting the previous
/// cycle brings history alone, and one bearing a negative acknowledgement is ignored.
/// </summary>
/// <param name="data">The raw packet received from the ladder server.</param>
WDTState::WDTState(void *data) :
	Territories()
{
	PacketClass *packet = new PacketClass((char *)data);

	int size_mpvc = 0;
	int size_hsvc = 0;
	int size_sdsc = 0;
	int size_ldsc = 0;
	int size_wurl = 0;
	int size_temp = 0;

	ShortDesc = NULL;
	LongDesc = NULL;
	WebURL = NULL;
	MapID = 0;
	CycleID = 0;
	CycleLength = 0;
	NumTerritories = 0;
	NumTicks = 0;
	TickTime = 0;
	OwnerHistory = NULL;

	if (packet->Get_Field("NACK", NULL, size_temp)) {
		/// NACK is not implemented
		delete packet;
	} else {
		if (packet->Get_Field("TOTK", CycleLength)) {
			IsPreviousCycle = true;
			CycleLength++;
			NumTicks = CycleLength;
		} else {
			IsPreviousCycle = false;
			packet->Get_Field("CRTK", NumTicks);
			NumTicks++;
			packet->Get_Field("CYLN", CycleLength);
			CycleLength++;
		}

		packet->Get_Field("WURL", NULL, size_wurl);
		packet->Get_Field("SDSC", NULL, size_sdsc);
		packet->Get_Field("LDSC", NULL, size_ldsc);

		if (!IsPreviousCycle) {
			packet->Get_Field("MPVC", NULL, size_mpvc);
		}
		packet->Get_Field("HSVC", NULL, size_hsvc);

		ShortDesc = new char[size_sdsc];
		LongDesc = new char[size_ldsc];
		WebURL = new char[size_wurl];

		unsigned char *hsvc_ptr = new unsigned char[size_hsvc];
		unsigned char *mpvc_ptr = NULL;

		if (!IsPreviousCycle) {
			mpvc_ptr = new unsigned char[size_mpvc];
			memset(mpvc_ptr, 0, size_mpvc);
		}

		memset(ShortDesc, 0, size_sdsc);
		memset(LongDesc, 0, size_ldsc);
		memset(WebURL, 0, size_wurl);
		memset(hsvc_ptr, 0, size_hsvc);

		packet->Get_Field("CMID", MapID);
		packet->Get_Field("CYID", CycleID);

		if (IsPreviousCycle) {
			TickTime = 0;
		} else {
			packet->Get_Field("TICC", TickTime);
		}

		packet->Get_Field("SDSC", ShortDesc, size_sdsc);
		packet->Get_Field("LDSC", LongDesc, size_ldsc);
		packet->Get_Field("WURL", WebURL, size_wurl);

		if (!IsPreviousCycle) {
			packet->Get_Field("MPVC", mpvc_ptr, size_mpvc);
		}

		packet->Get_Field("HSVC", hsvc_ptr, size_hsvc);

		if (!IsPreviousCycle) {
			Create_Territories(mpvc_ptr, size_mpvc);
		} else {
			NumTerritories = 30;
		}

		Create_History(hsvc_ptr, size_hsvc);

		delete [] hsvc_ptr;

		if (!IsPreviousCycle) {
			delete [] mpvc_ptr;
		}

		delete packet;
	}
}


/// <summary>
/// Destroys the campaign state.
/// This routine releases the territory records, the description strings, and the ownership
/// history that were built out of the server packet.
/// </summary>
WDTState::~WDTState(void)
{
	Territories.Clear(); // RA2: the territories are counted pointers and Clear releases them (TibSun deletes each one and calls Delete_All first)

	if (LongDesc != NULL) {
		delete [] LongDesc;
	}
	if (ShortDesc != NULL) {
		delete [] ShortDesc;
	}
	if (WebURL != NULL) {
		delete [] WebURL;
	}

	if (OwnerHistory != NULL) {
		for (int i = 0; i < (int)NumTicks; i++) {
			if (OwnerHistory[i] != NULL) {
				delete [] OwnerHistory[i];
			}
		}
		delete [] OwnerHistory;
	}
}


/// <summary>
/// Determines if an ownership history came with this state.
/// </summary>
/// <returns>bool; Is there a per tick ownership record to examine?</returns>
bool WDTState::Has_Owner_History(void)
{
	if (OwnerHistory != NULL) {
		return(true);
	}
	return(false);
}


/// <summary>
/// Is this state a report on the tour cycle that has already ended?
/// Such a state carries the finished ownership history only -- it has no territory records
/// and no tick timing of its own.
/// </summary>
/// <returns>bool; Does this state describe the previous cycle?</returns>
bool WDTState::Is_Previous_Cycle(void)
{
	return(IsPreviousCycle);
}


/// <summary>
/// Creates the territory ownership history from the packed server data.
/// This routine is used to unpack the history field of a World Domination Tour state
/// packet. It yields the side that held each territory at each tick of the cycle, which is
/// what the campaign presentation replays.
/// </summary>
/// <param name="data">The packed ownership records from the server packet.</param>
/// <param name="count">The length of the packed data in bytes.</param>
/// <returns>bool; Was there enough data to cover every tick and territory?</returns>
bool WDTState::Create_History(void *data, int count)
{
	int i;

	/// Four ownership entries are packed into each byte.
	if (4 * count < NumTicks * NumTerritories) {
		return(false);
	}

	OwnerHistory = new unsigned char *[NumTicks];

	for (i = 0; i < NumTicks; i++) {
		OwnerHistory[i] = new unsigned char[NumTerritories];
	}

	int pos = 0;
	int index = 0;

	for (int tick = 0; tick < NumTicks; tick++) {
		for (unsigned int territory = 0; territory < NumTerritories; territory++) {
			switch (index) {
				case CONTESTED:
					OwnerHistory[tick][territory] = (PEEK_AT(data, pos)) & 3;
					break;

				case GDI:
					OwnerHistory[tick][territory] = (PEEK_AT(data, pos) >> 2) & 3;
					break;

				case NOD:
					OwnerHistory[tick][territory] = (PEEK_AT(data, pos) >> 4) & 3;
					break;

				default: /// UNASSIGNED
					OwnerHistory[tick][territory] = (PEEK_AT(data, pos) >> 6) & 3;
					break;
			}

			index++;
			if (index == 4) {
				index = 0;
				pos++;
			}
		}

		if (index) {
			index = 0;
			pos++;
		}
	}
	return(true);
}


/// <summary>
/// Creates the territory list from the packed server data.
/// This routine is used to unpack the map variation field of a World Domination Tour state
/// packet. A territory is added for every record the block holds, each carrying the map
/// generator settings and the game options for the conflict fought over it.
/// </summary>
/// <param name="data">The packed territory records from the server packet.</param>
/// <param name="size">The length of the packed data in bytes.</param>
/// <returns>bool; Was the whole block unpacked? A truncated block stops the scan.</returns>
bool WDTState::Create_Territories(void * data, int size)
{
	NumTerritories = 0;

	if (size < MIN_TERRITORY_DATA_SIZE) {
		return(false);
	}

	int pos = 0;
	while (pos < size) {
		if (pos + MIN_TERRITORY_DATA_SIZE > size) {
			return(false);
		}

		rc_ptr<WDTTerritory> terr = WDTTerritory::Create(); // RA2: the territory is made counted (TibSun: new WDTTerritory and a memset)

		terr->Index = NumTerritories;

		/*
		 * When 1 byte-sized variables are read, 128 is subtracted from them (so 228 is the same as 100).
		 * When this variable is part of a group that has a default value, a value of > 127 signifies
		 * that this is the default value, and it will be loaded into the according Min and Max fields, too.
		 * For example, if CliffsMin is read as 228, all of CliffsMin, CliffsMax and CliffsDefault will
		 * be equal to 100 (because 228 - 128 = 100). This means that Min and Max fields can be omitted,
		 * provided that they are followed by a default value of at least 128.
		 * NOTE: 2 and 4 byte-sized values are read in with their bytes reversed.
		 */

		/*
		 * Single Values
		 */
		terr->Seed = PEEK_AT(data, pos++) << 8;
		terr->Seed += PEEK_AT(data, pos++);
		terr->UnitCount = PEEK_AT(data, pos++) + 0x80; // RA2: adds 0x80 (TibSun: subtracts it)
		terr->TechLevel = PEEK_AT(data, pos++) + 0x80; // RA2: as above
		terr->StartingCredits = PEEK_AT(data, pos++) << 8;
		terr->StartingCredits += PEEK_AT(data, pos++);
		unsigned int packed = PEEK_AT(data, pos++) - 0x80; // RA2: one byte holds the map type in its low and the theater in its high nibble (TibSun: Biome)
		terr->MapType = packed & 0x0F; // RA2: as above
		terr->Theater = (packed >> 4) & 0x0F; // RA2: as above
		terr->Time = PEEK_AT(data, pos++) + 0x80; // RA2: adds 0x80
		terr->Width = PEEK_AT(data, pos++) + 0x80; // RA2: as above
		terr->Height = PEEK_AT(data, pos++) + 0x80; // RA2: as above
		terr->NumPlayers = PEEK_AT(data, pos++) + 0x80; // RA2: as above
		terr->Resources = PEEK_AT(data, pos++) - 0x80; // RA2: the resources (TibSun: Veinholes)

		/*
		 * Ranged Values
		 */
		terr->CliffsMin = WDT_Unbias(PEEK_AT(data, pos)); pos++; // RA2: every value of a triple only drops the high bit, in order
		terr->CliffsMax = WDT_Unbias(PEEK_AT(data, pos)); pos++; // RA2: as above
		terr->CliffsDefault = WDT_Unbias(PEEK_AT(data, pos)); pos++; // RA2: as above

		terr->AccessibilityMin = WDT_Unbias(PEEK_AT(data, pos)); pos++; // RA2: as above
		terr->AccessibilityMax = WDT_Unbias(PEEK_AT(data, pos)); pos++; // RA2: as above
		terr->AccessibilityDefault = WDT_Unbias(PEEK_AT(data, pos)); pos++; // RA2: as above

		terr->HillsMin = WDT_Unbias(PEEK_AT(data, pos)); pos++; // RA2: as above
		terr->HillsMax = WDT_Unbias(PEEK_AT(data, pos)); pos++; // RA2: as above
		terr->HillsDefault = WDT_Unbias(PEEK_AT(data, pos)); pos++; // RA2: as above

		terr->TiberiumAmountMin = WDT_Unbias(PEEK_AT(data, pos)); pos++; // RA2: as above
		terr->TiberiumAmountMax = WDT_Unbias(PEEK_AT(data, pos)); pos++; // RA2: as above
		terr->TiberiumAmountDefault = WDT_Unbias(PEEK_AT(data, pos)); pos++; // RA2: as above

		terr->TiberiumFieldsMin = WDT_Unbias(PEEK_AT(data, pos)); pos++; // RA2: as above
		terr->TiberiumFieldsMax = WDT_Unbias(PEEK_AT(data, pos)); pos++; // RA2: as above
		terr->TiberiumFieldsDefault = WDT_Unbias(PEEK_AT(data, pos)); pos++; // RA2: as above

		terr->WaterMin = WDT_Unbias(PEEK_AT(data, pos)); pos++; // RA2: as above
		terr->WaterMax = WDT_Unbias(PEEK_AT(data, pos)); pos++; // RA2: as above
		terr->WaterDefault = WDT_Unbias(PEEK_AT(data, pos)); pos++; // RA2: as above

		terr->VegetationMin = WDT_Unbias(PEEK_AT(data, pos)); pos++; // RA2: as above
		terr->VegetationMax = WDT_Unbias(PEEK_AT(data, pos)); pos++; // RA2: as above
		terr->VegetationDefault = WDT_Unbias(PEEK_AT(data, pos)); pos++; // RA2: as above

		terr->CitiesMin = WDT_Unbias(PEEK_AT(data, pos)); pos++; // RA2: as above
		terr->CitiesMax = WDT_Unbias(PEEK_AT(data, pos)); pos++; // RA2: as above
		terr->CitiesDefault = WDT_Unbias(PEEK_AT(data, pos)); pos++; // RA2: as above

		/*
		 * User Modifiable Values
		 */
		terr->UserModBooleans = PEEK_AT(data, pos++) << 24;
		terr->UserModBooleans += PEEK_AT(data, pos++) << 16;
		terr->UserModBooleans += PEEK_AT(data, pos++) << 8;
		terr->UserModBooleans += PEEK_AT(data, pos++);

		/*
		 * Boolean Values
		 */
		terr->Booleans = PEEK_AT(data, pos++) << 24;
		terr->Booleans += PEEK_AT(data, pos++) << 16;
		terr->Booleans += PEEK_AT(data, pos++) << 8;
		terr->Booleans += PEEK_AT(data, pos++);

		Territories.Add(terr);
		NumTerritories++;
	}
	return(true);
}


/// <summary>
/// Fetches the game options this territory is fought under.
/// This routine is used to set up the rules for the conflict over this territory. Options
/// the tour never varies are forced to their tour values rather than taken from the
/// territory.
/// </summary>
/// <returns>Returns with a newly allocated option block. The caller is responsible for
/// deleting it.</returns>
#ifdef RA2_NON_MATCHING // not exact: no row of the original matches it yet (unplaced)
GameOptionsType *WDTTerritory::Get_Game_Options(void)
{
	GameOptionsType *opts = new GameOptionsType;

	opts->Bases = Bases;
	opts->Credits = StartingCredits;
	opts->BridgeDestruction = BridgeDestruction;
	opts->Goodies = Goodies;
	opts->Ghosts = true;
	opts->UnitCount = UnitCount;
	opts->AIPlayers = false;
	opts->AlliesAllowed = AlliesAllowed;
	opts->HarvTruce = HarvTruce;
	opts->CTF = false;
	opts->FogOfWar = FogOfWar;
	opts->MCVRedeploy = MCVRedeploy;

	return(opts);
}
#endif


/// <summary>
/// Builds a printable listing of this campaign state.
/// This routine is used to examine the tour data that arrives from the ladder server. It
/// covers the campaign descriptions and identifiers, every territory in the cycle, and the
/// side that held each territory at each tick.
/// </summary>
/// <returns>Returns with a newly allocated string holding the listing. The caller is
/// responsible for deleting it.</returns>
#ifdef RA2_NON_MATCHING // not exact: no row of the original matches it yet (unplaced)
char *WDTState::To_String(void)
{
	int i;

	char *retstr = new char[100000];
	char *tempbuf = new char[2048];
	memset(retstr, 0, 100000);
	memset(tempbuf, 0, 2048);

	sprintf(tempbuf, "WDTState::To_String printout\n");
	strcat(retstr, tempbuf);
	sprintf(tempbuf, "----------------------------\n");
	strcat(retstr, tempbuf);
	sprintf(tempbuf, "ShortDesc: %s\n", ShortDesc);
	strcat(retstr, tempbuf);
	sprintf(tempbuf, "LongDesc: %s\n", LongDesc);
	strcat(retstr, tempbuf);
	sprintf(tempbuf, "MapID: %d\n", MapID);
	strcat(retstr, tempbuf);
	sprintf(tempbuf, "CycleID: %d\n", CycleID);
	strcat(retstr, tempbuf);
	sprintf(tempbuf, "NumTerritories: %d\n", NumTerritories);
	strcat(retstr, tempbuf);
	sprintf(tempbuf, "NumTicks: %d\n", NumTicks);
	strcat(retstr, tempbuf);
	sprintf(tempbuf, "TickTime: %d seconds\n", TickTime);
	strcat(retstr, tempbuf);

	for (i = 0; i < (int)NumTerritories; i++) {
		sprintf(tempbuf, "Territory #%d\n", i);
		strcat(retstr, tempbuf);
		sprintf(tempbuf, "-------------\n");
		strcat(retstr, tempbuf);
		sprintf(tempbuf, "%s", Territories[i]->To_String());
		strcat(retstr, tempbuf);
	}

	for (i = 0; i < (int)NumTicks; i++) {
		sprintf(tempbuf, "Tick #%d\n", i);
		strcat(retstr, tempbuf);
		sprintf(tempbuf, "--------\n");
		strcat(retstr, tempbuf);

		for (unsigned int k = 0; k < NumTerritories; k++) {
			sprintf(tempbuf, "Territory %d: ", k);
			strcat(retstr, tempbuf);

			if (OwnerHistory[i][k] == NOD) {
				sprintf(tempbuf, "Nod\n");
			} else if (OwnerHistory[i][k] == GDI) {
				sprintf(tempbuf, "GDI\n");
			} else {
				sprintf(tempbuf, "Contested\n");
			}
			strcat(retstr, tempbuf);

		}
	}

	delete [] tempbuf;

	return(retstr);
}
#endif


/// <summary>
/// Builds a printable listing of this territory's settings.
/// This routine is used to examine the tour data that arrives from the ladder server.
/// Every map generator setting and every game option flag is written out.
/// </summary>
/// <returns>Returns with a newly allocated string holding the listing. The caller is
/// responsible for deleting it.</returns>
#ifdef RA2_NON_MATCHING // not exact: no row of the original matches it yet (unplaced)
char *WDTTerritory::To_String(void)
{
	char *retbuf = new char[2048];
	char *tempbuf = new char[128];
	memset(retbuf, 0, 2048);
	memset(tempbuf, 0, 128);

	sprintf(tempbuf, "UnitCount\t\t%d\nTechLevel\t\t%d\n", UnitCount, TechLevel);
	strcat(retbuf, tempbuf);
	sprintf(tempbuf, "StartingCredits\t%d\nSeed\t\t\t%d\n", StartingCredits, Seed);
	strcat(retbuf, tempbuf);
	sprintf(tempbuf, "Width\t\t\t%d\nHeight\t\t\t%d\n", Width, Height);
	strcat(retbuf, tempbuf);
	sprintf(tempbuf, "NumPlayers\t\t%d\nBiome\t\t\t%d\n", NumPlayers, MapType); // RA2: the map type (TibSun: Biome)
	strcat(retbuf, tempbuf);
	sprintf(tempbuf, "Time\t\t\t%d\nCliffsMin\t\t%d\n", Time, CliffsMin);
	strcat(retbuf, tempbuf);
	sprintf(tempbuf, "CliffsMax\t\t%d\nHillsMin\t\t%d\n", CliffsMax, HillsMin);
	strcat(retbuf, tempbuf);
	sprintf(tempbuf, "HillsMax\t\t%d\nAccessibilityMin\t%d\n", HillsMax, AccessibilityMin);
	strcat(retbuf, tempbuf);
	sprintf(tempbuf, "AccessibilityMax\t%d\nWaterMin\t\t%d\n", AccessibilityMax, WaterMin);
	strcat(retbuf, tempbuf);
	sprintf(tempbuf, "WaterMax\t\t%d\nTiberiumAmountMin\t%d\n", WaterMax, TiberiumAmountMin);
	strcat(retbuf, tempbuf);
	sprintf(tempbuf, "TiberiumAmountMax\t%d\nTiberium_Fields_Min\t%d\n", TiberiumAmountMax, TiberiumFieldsMin);
	strcat(retbuf, tempbuf);
	sprintf(tempbuf, "TiberiumFieldsMax\t%d\nVegetationMin\t\t%d\n", TiberiumFieldsMax, VegetationMin);
	strcat(retbuf, tempbuf);
	sprintf(tempbuf, "VegetationMax\t\t%d\nCitiesMin\t\t%d\n", VegetationMax, CitiesMin);
	strcat(retbuf, tempbuf);
	sprintf(tempbuf, "CitiesMax\t\t%d\nTime transitions\t\t%d\n", CitiesMax, TimeTransitions);
	strcat(retbuf, tempbuf);
	sprintf(tempbuf, "Tiberium Creatures\t%d\nAllies\t\t\t%d\n", TiberiumCreatures, AlliesAllowed);
	strcat(retbuf, tempbuf);
	sprintf(tempbuf, "Harvester Truce\t\t%d\nBases\t\t\t%d\n", HarvTruce, Bases);
	strcat(retbuf, tempbuf);
	sprintf(tempbuf, "Re-deployable MCV\t%d\nFog of War\t\t%d\n", MCVRedeploy, FogOfWar);
	strcat(retbuf, tempbuf);
	sprintf(tempbuf, "Destroyable Bridges\t%d\nCrates\t\t\t%d\n", BridgeDestruction, Goodies);
	strcat(retbuf, tempbuf);
	sprintf(tempbuf, "Blue tiberium\t\t%d\n", BlueTiberium);
	strcat(retbuf, tempbuf);
	sprintf(tempbuf, "Veinholes\t\t%d\nCliffsDefault\t\t%d\n", Resources, CliffsDefault); // RA2: the resources (TibSun: Veinholes)
	strcat(retbuf, tempbuf);
	sprintf(tempbuf, "HillsDefault\t\t%d\nAccessibilityDefault\t\t%d\n", HillsDefault, AccessibilityDefault);
	strcat(retbuf, tempbuf);
	sprintf(tempbuf, "WaterDefault\t\t%d\nTiberiumAmountDefault\t\t%d\n", WaterDefault, TiberiumAmountDefault);
	strcat(retbuf, tempbuf);
	sprintf(tempbuf, "TiberiumFieldsDefault\t\t%d\nVegetationDefault\t\t%d\n", TiberiumFieldsDefault, VegetationDefault);
	strcat(retbuf, tempbuf);
	sprintf(tempbuf, "CitiesDefault\t\t%d\n", CitiesDefault);
	strcat(retbuf, tempbuf);
	delete [] tempbuf;

	return(retbuf);
}
#endif

