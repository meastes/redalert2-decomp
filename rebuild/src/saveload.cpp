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

/* $Header: /counterstrike/SAVELOAD.CPP 9     3/17/97 1:04a Steve_tall $ */
/***********************************************************************************************
 ***              C O N F I D E N T I A L  ---  W E S T W O O D  S T U D I O S               ***
 ***********************************************************************************************
 *                                                                                             *
 *                 Project Name : Command & Conquer                                            *
 *                                                                                             *
 *                    File Name : SAVELOAD.CPP                                                 *
 *                                                                                             *
 *                   Programmer : Joe L. Bostic                                                *
 *                                                                                             *
 *                   Start Date : August 23, 1994                                              *
 *                                                                                             *
 *                  Last Update : July 8, 1996 [JLB]                                           *
 *                                                                                             *
 *---------------------------------------------------------------------------------------------*
 * Functions:                                                                                  *
 *   Code_All_Pointers -- Code all pointers.                                                   *
 *   Decode_All_Pointers -- Decodes all pointers.                                              *
 *   Get_Savefile_Info -- gets description, scenario #, house                                  *
 *   Load_Game -- loads a saved game                                                           *
 *   Load_MPlayer_Values -- Loads multiplayer-specific values                                  *
 *   Load_Misc_Values -- loads miscellaneous variables                                         *
 *   MPlayer_Save_Message -- pops up a "saving..." message                                     *
 *   Put_All -- Store all save game data to the pipe.                                          *
 *   Reconcile_Players -- Reconciles loaded data with the 'Players' vector                     *
 *   Save_Game -- saves a game to disk                                                         *
 *   Save_MPlayer_Values -- Saves multiplayer-specific values                                  *
 *   Save_Misc_Values -- saves miscellaneous variables                                         *
 * - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - */

#define INCLUDE_COM
#include "always.h"
#include "rect.h" // RA2: the header statics, in the order of the unit's initializer table
#include "geometry.h"

#include "saveload.h"

#include "_logic.h"
#include "_map.h"
#include "_rect.h"
#include "_rules.h"
#include "_script.h"
#include "_tactica.h"
#include "_vanim.h"
#include "_warhead.h"
#include "_weapon.h"
#include "aircraft.h"
#include "airctype.h"
#include "aitrig.h"
#include "bomb.h" // RA2: as above
#include "bomblist.h" // RA2: as above
#include "capmgr.h" // RA2: as above
#include "kamikaze.h" // RA2: as above
#include "parasite.h" // RA2: as above
#include "radsite.h" // RA2: as above
#include "spawnmanager.h" // RA2: as above
#include "temporal.h" // RA2: as above
#include "theme.h" // RA2: as above
#include "tracker.h" // RA2: as above
#include "alphashp.h"
#include "anim.h"
#include "animtype.h"
#include "blight.h"
#include "building.h"
#include "builtype.h"
#include "bullet.h"
#include "bullettype.h"
#include "dbgprint.h"
#include "empulse.h"
#include "enviro.h"
#include "factory.h"
#include "fog.h"
#include "globals.h"
#include "goptions.h"
#include "houstype.h"
#include "ilinkstm.h"
#include "infantry.h"
#include "infatype.h"
#include "init.h"
#include "ion.h"
#include "loaddlg.h"
#include "light.h"
#include "logic.h"
#include "overlay.h"
#include "overtype.h"
#include "ovrlight.h"
#include "particle.h"
#include "partsys.h"
#include "progress.h" // RA2: Progress.Initialize in Load_Game
#include "psystype.h"
#include "ptype.h"
#include "revent.h"
#include "rules.h"
#include "savever.h"
#include "scenario.h"
#include "script.h"
#include "session.h"
#include "side.h"
#include "sidebar.h"
#include "smudtype.h"
#include "sun.h"
#include "super.h"
#include "suprtype.h"
#include "swizzle.h"
#include "tactical.h"
#include "taction.h"
#include "tag.h"
#include "tagtype.h"
#include "taskforc.h"
#include "team.h"
#include "teamtype.h"
#include "terrain.h"
#include "terrtype.h"
#include "tevent.h"
#include "tiberium.h"
#include "trigger.h"
#include "trigtype.h"
#include "tube.h"
#include "unit.h"
#include "unittype.h"
#include "vanim.h"
#include "vanimtype.h"
#include "vein.h"
#include "vox.h"
#include "warhead.h"
#include "wave.h"
#include "waypoint.h"
#include "weapon.h"
#include "ftimer.h"
#include "txtlabel2.h"



//#define	SAVE_BLOCK_SIZE	512
#define	SAVE_BLOCK_SIZE	4096
//#define	SAVE_BLOCK_SIZE	1024

/*
********************************** Defines **********************************
*/
#define	SAVEGAME_VERSION		(DESCRIP_MAX + 0x01000006)


unsigned int ExpectedGameVersion = LoadOptionsClass::GAMEVER_FS;


static int Reconcile_Players(void);

_COM_SMARTPTR_TYPEDEF(ILinkStream, __uuidof(ILinkStream));





/// <summary>
/// Loads a vector of persistent objects from the save game stream.
/// This routine reads the element count and then recreates each object through OLE. The
/// objects are not handed back -- each one reattaches itself to its own heap as it is
/// constructed, which is what refills the game's vectors.
/// RA2
/// </summary>
/// <returns>Returns with S_OK, or the failure code of the read that went wrong.</returns>
void __fastcall Free_Interface_Art(void); // RA2: 0x006F30F0, defined in txtlabel2.cpp
HRESULT __fastcall Static_Sounds_Save(IStream * stream); // RA2: 0x00714A90, defined in voc.cpp
HRESULT __fastcall Static_Sounds_Load(IStream * stream); // RA2: 0x00714B80, defined in voc.cpp
static bool Put_All(IStream *stream, int save_net); // RA2: defined after its caller

static bool Get_All(IStream *stream, bool save_net); // RA2: defined after its caller

HRESULT Load_Vector(IStream * stream) // RA2: Load_Vector at 0x654C40 matches only so
{
	int count;
	int index;
	LPVOID obj;

	HRESULT result = stream->Read(&count, sizeof(count), NULL);
	if (FAILED(result)) return(result); // RA2: as above
	for (index = 0; index < count; index++) {
		if (FAILED(result = OleLoadFromStream(stream, IID_IUnknown, &obj))) return(result); // RA2: as above
	}
	return(S_OK);
}





/// <summary>
/// Saves a vector of persistent objects to the save game stream.
/// This routine writes the element count and then streams out each object in turn through
/// its IPersistStream interface.
/// </summary>
/// <returns>Returns with S_OK, or the failure code of the first object that refused to
/// save.</returns>
template<class T>
inline HRESULT Save_Vector(IStream * stream, const DynamicVectorClass<T> &list) // RA2: as above
{
	int count = list.Count();
	HRESULT result = stream->Write(&count, sizeof(count), NULL);
	if (SUCCEEDED(result)) {
		for (int index = 0; index < count; index++) {
			LPPERSISTSTREAM lpPS = NULL;
			result = list[index]->QueryInterface(IID_IPersistStream, (LPVOID *)&lpPS);
			if (FAILED(result)) {
				return(result);
			}
			result = OleSaveToStream(lpPS, stream);
			if (FAILED(result)) {
				return(result);
			}
			result = lpPS->Release();
			if (FAILED(result)) {
				return(result);
			}
		}
		result = S_OK;
	}
	return(result);
}
/// <summary>
/// Builds a checksum over the whole of the game object state.
/// This routine walks the scenario and every object and type heap, folding each one's own
/// contribution into a single engine. It is used to compare the state held by two
/// machines in a networked game, so that a desynchronization can be spotted.
/// </summary>
/// <returns>Returns with the checksum engine holding the accumulated state.</returns>
CRCEngine Object_CRCs(void)
{
	int i;
	CRCEngine crc;
	Scen->Compute_CRC(crc);

#define DO_OBJ_CRC(VECTOR) \
	for (i = 0; i < VECTOR.Count(); i++) { \
		VECTOR[i]->Compute_CRC(crc); \
	} \

	DO_OBJ_CRC(HouseTypes);
	DO_OBJ_CRC(Houses);
	DO_OBJ_CRC(UnitTypes);
	DO_OBJ_CRC(Units);
	DO_OBJ_CRC(InfantryTypes);
	DO_OBJ_CRC(Infantry);
	DO_OBJ_CRC(BuildingTypes);
	DO_OBJ_CRC(Buildings);
	DO_OBJ_CRC(AircraftTypes);
	DO_OBJ_CRC(Aircraft);
	DO_OBJ_CRC(AITriggerTypes);
	DO_OBJ_CRC(Anims);
	DO_OBJ_CRC(AnimTypes);
	DO_OBJ_CRC(TaskForces);
	DO_OBJ_CRC(TeamTypes);
	DO_OBJ_CRC(Teams);
	DO_OBJ_CRC(ScriptTypes);
	DO_OBJ_CRC(Scripts);
	DO_OBJ_CRC(TagTypes);
	DO_OBJ_CRC(Tags);
	DO_OBJ_CRC(TriggerTypes);
	DO_OBJ_CRC(Triggers);
	DO_OBJ_CRC(Actions);
	DO_OBJ_CRC(Events);
	DO_OBJ_CRC(Factories);
	DO_OBJ_CRC(VoxelAnimTypes);
	DO_OBJ_CRC(VoxelAnims);
	DO_OBJ_CRC(Warheads);
	DO_OBJ_CRC(Weapons);
	DO_OBJ_CRC(ParticleTypes);
	DO_OBJ_CRC(Particles);
	DO_OBJ_CRC(ParticleSystems);
	DO_OBJ_CRC(ParticleSystemTypes);
	DO_OBJ_CRC(BulletTypes);
	DO_OBJ_CRC(Bullets);
	DO_OBJ_CRC(WaypointPaths);
	DO_OBJ_CRC(SmudgeTypes);
	DO_OBJ_CRC(OverlayTypes);
	DO_OBJ_CRC(LightSources);
	DO_OBJ_CRC(BuildingLights);
	DO_OBJ_CRC(Tubes);
	DO_OBJ_CRC(Sides);
	DO_OBJ_CRC(Tiberiums);
	DO_OBJ_CRC(EMPulseClass::EMPulses);
	DO_OBJ_CRC(SuperWeaponTypes);
	DO_OBJ_CRC(SuperWeapons);
	DO_OBJ_CRC(TerrainTypes);
	DO_OBJ_CRC(Terrains);

#undef DO_OBJ_CRC

	if (PlayerPtr != NULL) {
		crc(PlayerPtr->HeapID);
	}
	crc((int)Frame);
	crc(CurrentObject.Count());
	return(crc);
}

/***********************************************************************************************
 * Put_All -- Store all save game data to the pipe.                                            *
 *                                                                                             *
 *    This is the bulk processor of the game related save game data. All the game object       *
 *    and state data is stored to the pipe specified.                                          *
 *                                                                                             *
 * INPUT:   pipe  -- Reference to the pipe that will receive the save game data.               *
 *                                                                                             *
 * OUTPUT:  none                                                                               *
 *                                                                                             *
 * WARNINGS:   none                                                                            *
 *                                                                                             *
 * HISTORY:                                                                                    *
 *   07/08/1996 JLB : Created.                                                                 *
 *=============================================================================================*/

/***************************************************************************
 * Save_Game -- saves a game to disk                                       *
 *                                                                         *
 * Saving the Map:                                                         *
 *     DisplayClass::Save() invokes CellClass's Write() for every cell     *
 *     that needs to be saved.  A cell needs to be saved if it contains    *
 *     any special data at all, such as a TIcon, or an Occupier.           *
 *   The cell saves its own CellTrigger pointer, converted to a TARGET.    *
 *                                                                         *
 * Saving game objects:                                                    *
 *   - Any object stored in an ArrayOf class needs to be saved.  The ArrayOf*
 *     Save() routine invokes each object's Write() routine, if that       *
 *     object's IsActive is set.                                           *
 *                                                                         *
 * Saving the layers:                                                      *
 *   The Map's Layers (Ground, Air, etc) of things that are on the map,    *
 *     and the Logic's Layer of things to process both need to be saved.   *
 *     LayerClass::Save() writes the entire layer array to disk            *
 *                                                                         *
 * Saving the houses:                                                      *
 *   Each house needs to be saved, to record its Credits, Power, etc.      *
 *                                                                         *
 * Saving miscellaneous data:                                              *
 *   There are a lot of miscellaneous variables to save, such as the       *
 *     map's dimensions, the player's house, etc.                          *
 *                                                                         *
 * INPUT:                                                                  *
 *      id      numerical ID, for the file extension                       *
 *                                                                         *
 * OUTPUT:                                                                 *
 *      true = OK, false = error                                           *
 *                                                                         *
 * WARNINGS:                                                               *
 *      none.                                                              *
 *                                                                         *
 * HISTORY:                                                                *
 *   12/28/1994 BR : Created.                                              *
 *   02/27/1996 JLB : Uses simpler game control value save operation.      *
 *=========================================================================*/
bool Save_Game(const char *file_name, wchar_t const * descr, bool ) // RA2: wide description
{
	WCHAR name[64];

	// RA2: the other failure and progress messages are not compiled in

	DebugString("\nSAVING GAME [%s - %s]\n", file_name, descr);

	IStoragePtr storage; // RA2: declared before the name is converted

	MultiByteToWideChar(0,0, file_name, -1, name, sizeof(name)/sizeof(WCHAR));

	/*
	**	Open the file
	*/
	if (FAILED(StgCreateDocfile(name, STGM_CREATE|STGM_SHARE_EXCLUSIVE|STGM_READWRITE, 0, &storage))) {
		return(false);
	}


	/*
	**	Save the description, scenario #, and house
	**	(scenario # & house are saved separately from the actual Scenario &
	**	PlayerPtr globals for convenience; we can quickly find out which
	**	house & scenario this save-game file is for by reading these values.
	**	Also, PlayerPtr is stored in a coded form in Save_Misc_Values(),
	**	which may or may not be a HousesType number; so, saving 'house'
	**	here ensures we can always pull out the house for this file.)
	*/
	SaveVersionInfo info;
	info.Set_Internal_Version(ExpectedGameVersion);
	info.Set_Scenario_Description(descr);
	info.Set_Version(1);
	info.Set_Player_House(PlayerPtr->Class->UIName); // RA2: the house's wide UIName
	info.Set_Campaign_Number(Scen->Campaign);
	info.Set_Scenario_Number(Scen->Scenario);
	info.Set_Executable_Name("SUN.EXE");
	info.Set_Game_Type(Session.Type);
	FILETIME FileTime;
	CoFileTimeNow(&FileTime);
	info.Set_Last_Time(FileTime);
	info.Set_Start_Time(FileTime);
	info.Set_Play_Time(FileTime);

	/*
	**	Save the save-game version, for loading verification
	*/
	info.Save(storage); // RA2: the result of saving the version information is not checked

	IStreamPtr content;
	if (FAILED(storage->CreateStream(L"CONTENTS", STGM_CREATE|STGM_SHARE_EXCLUSIVE|STGM_READWRITE, 0, 0, &content))) {
		return(false);
	}

	ILinkStreamPtr link;
	link.CreateInstance(CLSID_CompressStream, NULL, CLSCTX_INPROC|CLSCTX_LOCAL_SERVER);
	if (FAILED(link->Link_Stream(content))) {
		DebugString("\t***** FAILED!\n"); // RA2: the only failure message, after linking the stream
		return(false);
	}
	IStreamPtr stream(link);

	/*
	**	Dump the save game data to the file. The data is compressed
	**	and then encrypted. The message digest is calculated in the
	**	process by using the data just as it is written to disk.
	*/
	bool res = Put_All(stream,0);

	if (FAILED(link->Unlink_Stream(NULL))) { // RA2: no debug messages around unlinking, releasing and committing
		return(false);
	}

	content.Release();

	if (FAILED(storage->Commit(0))) {
		return(false);
	}

	DebugString("SAVING GAME [%s] - Complete\n\n", file_name, descr);
	return(res);
}



/***************************************************************************
 * Reconcile_Players -- Reconciles loaded data with the 'Players' vector   *
 *                                                                         *
 * This function is for supporting loading a saved multiplayer game.       *
 * When the game is loaded, we have to figure out which house goes with    *
 * which entry in the Players vector.  We also have to figure out if       *
 * everyone who was originally in the game is still with us, and if not,   *
 * turn their stuff over to the computer.                                  *
 *                                                                         *
 * So, this function does the following:                                   *
 * - For every name in 'Players', makes sure that name is in the House     *
 *   array; if not, it's a fatal error.                                    *
 * - For every human-controlled house, makes sure there's a player         *
 *   with that name; if not, it turns that house over to the computer.     *
 * - Fills in the Player's house ID                                        *
 *                                                                         *
 * This assumes that each player MUST keep their name the same as it was   *
 * when the game was saved!  It's also assumed that the network            *
 * connections have not been formed yet, since Player[i]->Player.ID will   *
 * be invalid until this routine has been called.                          *
 *                                                                         *
 * INPUT:                                                                  *
 *      none.                                                              *
 *                                                                         *
 * OUTPUT:                                                                 *
 *      true = OK, false = error                                           *
 *                                                                         *
 * WARNINGS:                                                               *
 *      none.                                                              *
 *                                                                         *
 * HISTORY:                                                                *
 *   09/29/1995 BRR : Created.                                             *
 *=========================================================================*/
static int Reconcile_Players(void)
{
	#if 0
	int i;
	int found;
	HousesType house;
	HouseClass * housep;

	/*
	**	If there are no players, there's nothing to do.
	*/
	if (Session.Players.Count()==0)
		return(true);

	/*
	**	Make sure every name we're connected to can be found in a House
	*/
	for (i = 0; i < Session.Players.Count(); i++) {
		found = 0;
		for (house = HOUSE_MULTI1; house < HOUSE_MULTI1 +
			Session.MaxPlayers; house++) {

			housep = Houses[house];
			if (!housep) {
				continue;
			}

			if (!stricmp(Session.Players[i]->Name, housep->IniName)) {
				found = 1;
				break;
			}
		}
		if (!found)
			return(false);
	}

	//
	// Loop through all Houses; if we find a human-owned house that we're
	// not connected to, turn it over to the computer.
	//
	for (house = HOUSE_MULTI1; house < HOUSE_MULTI1 +
		Session.MaxPlayers; house++) {
		housep = Houses[house];
		if (!housep) {
			continue;
		}

		//
		// Skip this house if it wasn't human to start with.
		//
		if (!housep->IsHuman) {
			continue;
		}

		//
		// Try to find this name in the Players vector; if it's found, set
		// its ID to this house.
		//
		found = 0;
		for (i = 0; i < Session.Players.Count(); i++) {
			if (!stricmp(Session.Players[i]->Name, housep->IniName)) {
				found = 1;
				Session.Players[i]->Player.ID = house;
				break;
			}
		}

		/*
		**	If this name wasn't found, remove it
		*/
		if (!found) {

			/*
			**	Turn the player's house over to the computer's AI
			*/
			housep->IsHuman = false;
			housep->IsStarted = true;
//			housep->Smartness = IQ_MENSA;
			housep->IQ = Rule->MaxIQ;
			housep->IniName = Text_String(TXT_COMPUTER);

			Session.NumPlayers--;
		}
	}

	//
	// If all went well, our Session.NumPlayers value should now equal the value
	// from the saved game, minus any players we removed.
	//
	if (Session.NumPlayers == Session.Players.Count()) {
		return(true);
	} else {
		return(false);
	}
	#endif
}


static bool Put_All(IStream *stream, int save_net)
{
	// RA2: no "Saving ..." or failure messages are compiled in (only the two skirmish ones remain)
	/*
	**	Save the scenario global information.
	*/
	Scen->Save(stream);
	Environment.Save(stream);
	Rule->Save(stream);

	if (FAILED(Save_Vector(stream, AnimTypes))) {
		return(false);
	}

	/*
	**	Save the map.  The map must be saved first, since it saves the Theater.
	*/
	Map.Save(stream);

	if (FAILED(Save_Vector(stream, Tubes))) {
		return(false);
	}

	/*
	**	Save miscellaneous variables.
	*/
	if (FAILED(Save_Misc_Values(stream))) {
		return(false);
	}

	/*
	**	Save the Logic & Map layers
	*/
	Logic.Save(stream);

	if (FAILED(OleSaveToStream(TacticalMap, stream))) {
		return(false);
	}

	/*
	**	Save all game objects.  This code saves every object that's stored in a
	**	TFixedIHeap class.
	*/
	if (FAILED(Save_Vector(stream, HouseTypes))) {
		return(false);
	}
	if (FAILED(Save_Vector(stream, Houses))) {
		return(false);
	}
	if (FAILED(Save_Vector(stream, Units))) {
		return(false);
	}
	if (FAILED(Save_Vector(stream, UnitTypes))) {
		return(false);
	}
	if (FAILED(Save_Vector(stream, InfantryTypes))) {
		return(false);
	}
	if (FAILED(Save_Vector(stream, Infantry))) {
		return(false);
	}
	if (FAILED(Save_Vector(stream, BuildingTypes))) {
		return(false);
	}
	if (FAILED(Save_Vector(stream, Buildings))) {
		return(false);
	}
	if (FAILED(Save_Vector(stream, AircraftTypes))) {
		return(false);
	}
	if (FAILED(Save_Vector(stream, Aircraft))) {
		return(false);
	}
	if (FAILED(Save_Vector(stream, Anims))) {
		return(false);
	}
	if (FAILED(Save_Vector(stream, TaskForces))) {
		return(false);
	}
	if (FAILED(Save_Vector(stream, TeamTypes))) {
		return(false);
	}
	if (FAILED(Save_Vector(stream, Teams))) {
		return(false);
	}
	if (FAILED(Save_Vector(stream, ScriptTypes))) {
		return(false);
	}
	if (FAILED(Save_Vector(stream, Scripts))) {
		return(false);
	}
	if (FAILED(Save_Vector(stream, TagTypes))) {
		return(false);
	}
	if (FAILED(Save_Vector(stream, Tags))) {
		return(false);
	}
	if (FAILED(Save_Vector(stream, TriggerTypes))) {
		return(false);
	}
	if (FAILED(Save_Vector(stream, Triggers))) {
		return(false);
	}
	Save_Vector(stream, AITriggerTypes); // RA2: the result is not tested
	Save_Vector(stream, Actions); // RA2: as above
	if (FAILED(Save_Vector(stream, Events))) {
		return(false);
	}
	if (FAILED(Save_Vector(stream, Factories))) {
		return(false);
	}
	if (FAILED(Save_Vector(stream, VoxelAnimTypes))) {
		return(false);
	}
	if (FAILED(Save_Vector(stream, VoxelAnims))) {
		return(false);
	}
	if (FAILED(Save_Vector(stream, Warheads))) {
		return(false);
	}
	if (FAILED(Save_Vector(stream, Weapons))) {
		return(false);
	}
	if (FAILED(Save_Vector(stream, ParticleTypes))) {
		return(false);
	}
	if (FAILED(Save_Vector(stream, Particles))) {
		return(false);
	}
	if (FAILED(Save_Vector(stream, ParticleSystemTypes))) {
		return(false);
	}
	if (FAILED(Save_Vector(stream, ParticleSystems))) {
		return(false);
	}
	if (FAILED(Save_Vector(stream, BulletTypes))) {
		return(false);
	}
	if (FAILED(Save_Vector(stream, Bullets))) {
		return(false);
	}
	if (FAILED(Save_Vector(stream, WaypointPaths))) {
		return(false);
	}
	if (FAILED(Save_Vector(stream, SmudgeTypes))) {
		return(false);
	}
	if (FAILED(Save_Vector(stream, OverlayTypes))) {
		return(false);
	}
	if (FAILED(Save_Vector(stream, LightSources))) {
		return(false);
	}
	if (FAILED(Save_Vector(stream, BuildingLights))) {
		return(false);
	}
	if (FAILED(Save_Vector(stream, Sides))) {
		return(false);
	}
	if (FAILED(Save_Vector(stream, Tiberiums))) {
		return(false);
	}
	if (FAILED(Save_Vector(stream, EMPulseClass::EMPulses))) {
		return(false);
	}
	if (FAILED(Save_Vector(stream, SuperWeaponTypes))) {
		return(false);
	}
	if (FAILED(Save_Vector(stream, SuperWeapons))) {
		return(false);
	}
	{ // RA2: the timed super weapons are written as their raw pointers, after the count
		int count = TimedSuperWeapons.Count(); // RA2: as above
		HRESULT result = stream->Write(&count, sizeof(count), NULL); // RA2: as above
		if (SUCCEEDED(result)) { // RA2: as above
			for (int index = 0; index < count; index++) { // RA2: as above
				SuperClass * super = TimedSuperWeapons[index]; // RA2: as above
				result = stream->Write(&super, sizeof(super), NULL); // RA2: as above
				if (FAILED(result)) { // RA2: as above
					break; // RA2: as above
				}
			}
		}
		if (FAILED(result)) { // RA2: as above
			return(false); // RA2: as above
		}
	}
	if (FAILED(Save_Vector(stream, TerrainTypes))) {
		return(false);
	}
	if (FAILED(Save_Vector(stream, Terrains))) {
		return(false);
	}
	if (FAILED(Save_Vector(stream, FoggedObjectClass::FoggyObjects))) {
		return(false);
	}
	if (FAILED(Save_Vector(stream, AlphaShapes))) {
		return(false);
	}
	if (FAILED(Save_Vector(stream, Waves))) {
		return(false);
	}
	if (!VeinholeMonsterClass::Save_All(stream)) {
		return(false);
	}
	if (!RadarEventClass::Save(stream)) {
		return(false); // RA2: as above
	}
	if (FAILED(Save_Vector(stream, CaptureManagerArray))) { // RA2: the next ten steps are not in TibSun
		return(false); // RA2: as above
	}
	if (FAILED(Save_Vector(stream, Parasites))) { // RA2: as above
		return(false); // RA2: as above
	}
	if (FAILED(Save_Vector(stream, Temporals))) { // RA2: as above
		return(false); // RA2: as above
	}
	if (FAILED(Save_Vector(stream, SpawnManagers))) { // RA2: as above
		return(false); // RA2: as above
	}
	if (FAILED(AircraftTracker.Save(stream))) { // RA2: as above
		return(false); // RA2: as above
	}
	if (FAILED(Kamikazes.Save(stream))) { // RA2: as above
		return(false); // RA2: as above
	}
	if (FAILED(BombList.Save(stream))) { // RA2: as above
		return(false); // RA2: as above
	}
	if (FAILED(Save_Vector(stream, Bombs))) { // RA2: as above
		return(false); // RA2: as above
	}
	if (FAILED(Save_Vector(stream, RadSites))) { // RA2: as above
		return(false);
	}

	if (Session.Type == GAME_SKIRMISH) {
		DebugString("Writing Skirmish Session.Options\n");
		if (!Session.Options.Save(stream)) {
			return(false);
		}
	}

	if (FAILED(Static_Sounds_Save(stream))) { // RA2: the sounds, the speech and the theme close the save
		return(false); // RA2: as above
	}
	if (FAILED(Speech_Save(stream))) { // RA2: as above
		return(false); // RA2: as above
	}
	if (FAILED(Theme.Save(stream))) { // RA2: as above
		return(false); // RA2: as above
	}
	return(true);
}


/// <summary>
/// Restores all of the save game data from the stream.
/// This routine is the counterpart of Put_All. It tears down the current scenario, puts
/// back the addon, theater and rules that the game was saved under, rebuilds the display
/// surfaces to suit the saved options, and then recreates every object heap in the same
/// order they were written out.
/// </summary>
/// <returns>bool; Was the game state restored?</returns>



/***********************************************************************************************
 * Put_All -- Store all save game data to the pipe.                                            *
 *                                                                                             *
 *    This is the bulk processor of the game related save game data. All the game object       *
 *    and state data is stored to the pipe specified.                                          *
 *                                                                                             *
 * INPUT:   pipe  -- Reference to the pipe that will receive the save game data.               *
 *                                                                                             *
 * OUTPUT:  none                                                                               *
 *                                                                                             *
 * WARNINGS:   none                                                                            *
 *                                                                                             *
 * HISTORY:                                                                                    *
 *   07/08/1996 JLB : Created.                                                                 *
 *=============================================================================================*/



/***************************************************************************
 * Load_Game -- loads a saved game                                         *
 *                                                                         *
 * This routine loads the data in the same way it was saved out.           *
 *                                                                         *
 * Loading the Map:                                                        *
 *   - DisplayClass::Load() invokes CellClass's Load() for every cell      *
 *     that was saved.                                                     *
 * - The cell loads its own CellTrigger pointer.                           *
 *                                                                         *
 * Loading game objects:                                                   *
 * - IHeap's Load() routine loads the # of objects stored, and loads       *
 *   each object.                                                          *
 * - Triggers: Add themselves to the HouseTriggers if they're associated   *
 *   with a house                                                          *
 *                                                                         *
 * Loading the layers:                                                     *
 *     LayerClass::Load() reads the entire layer array to disk             *
 *                                                                         *
 * Loading the houses:                                                     *
 *   Each house is loaded in its entirety.                                 *
 *                                                                         *
 * Loading miscellaneous data:                                             *
 *   There are a lot of miscellaneous variables to load, such as the       *
 *     map's dimensions, the player's house, etc.                          *
 *                                                                         *
 * INPUT:                                                                  *
 *      id         numerical ID, for the file extension                    *
 *                                                                         *
 * OUTPUT:                                                                 *
 *      true = OK, false = error                                           *
 *                                                                         *
 * WARNINGS:                                                               *
 *      If this routine returns false, the entire game will be in an       *
 *      unknown state, so the scenario will have to be re-initialized.     *
 *                                                                         *
 * HISTORY:                                                                *
 *   12/28/1994 BR : Created.                                              *
 *   1/20/97  V.Grippi Added expansion CD check                            *
 *=========================================================================*/
bool Load_Game(const char *file_name)
{
	WCHAR name[64];

	DebugString("\nLOADING GAME [%s]\n", file_name);

	/*
	**	Read & discard the save-game's header info
	*/
	SaveVersionInfo info;
	if (!Get_Savefile_Info(file_name, &info)) {
		return(false);
	}

	Session.Type = (GameType)info.Get_Game_Type();
	Swizzler.Reset();

	/*
	**	Open the file
	*/
	IStoragePtr storage;
	MultiByteToWideChar(0,0,file_name, -1, name, (sizeof(name)/sizeof(WCHAR)));

	if (FAILED(StgOpenStorage(name, 0, STGM_SHARE_DENY_WRITE, 0, 0, &storage))) {
		return(false);
	}

	IStreamPtr content;
	if (FAILED(storage->OpenStream(L"CONTENTS", 0, STGM_SHARE_EXCLUSIVE, 0, &content))) {
		return(false);
	}

	IUnknown *pUnknown = NULL;
	ILinkStreamPtr link;
	link.CreateInstance(CLSID_CompressStream, pUnknown,CLSCTX_INPROC|CLSCTX_LOCAL_SERVER);
	if (FAILED(link->Link_Stream(content))) {
		//return(false);
	}
	IStreamPtr stream(link);

	Progress.Initialize(100.0, 0, NULL); // RA2: the progress screen is set up before the load, with no dialog window
	bool res = Get_All(stream, false);

	link->Unlink_Stream(NULL);

	Swizzler.Reset();

	/*
	**	Fixup any expediency data that can be inferred from the physical
	**	data loaded.
	*/
	Post_Load_Game();

	Map.Init_IO();
	Map.Activate(1);
	Map.Reposition_Sidebar();
	TiberiumClass::Init_Tiberium_Growth_System();
	TiberiumClass::Init_Tiberium_Spread_System();
	Map.Complete_Radar_Refresh();
	ScenarioActive = true;
	TacticalActive = true;
	Free_Interface_Art(); // RA2: the interface art is freed once the game is up (0x006F30F0)
	DebugString("LOADING GAME [%s] - Complete\n\n", file_name);
	return(true);
}



static bool Get_All(IStream *stream, bool save_net)
{
	Clear_Scenario();
	Scen->Load(stream);

	if (!Prep_For_Side(Scen->IsGDI ? SIDE_GDI : SIDE_NOD)) { // RA2: no required-addon check (RequiredAddOn is not in RA2's ScenarioClass)
		return(false);
	}

	Rect temp; // RA2: as above
	temp = Get_Title_Screen_Area(); // RA2: the area comes from the title screen's layout, not from VisibleRect and the sidebar's width

	{ // RA2: the surface rectangles live in their own block
		Rect sidebar(0, 0, SidebarWidth, VisibleRect.Height); // RA2: the surfaces are sized from SidebarWidth
		Rect tile(0, 0, temp.Width, VisibleRect.Height); // RA2: as above
		Rect composite(0, 0, temp.Width, VisibleRect.Height); // RA2: as above
		Allocate_Surfaces(VisibleRect, composite, tile, sidebar, false); // RA2: the arguments are built beforehand, in this order
	}

	Map.Set_View_Dimensions(temp);

	Environment.Load(stream);

	Init_Theater(Scen->Theater);

	Rule->Load(stream); // RA2: no art INI loading and no per-side speech preparation (SpeechSide is not in RA2's ScenarioClass)

	if (FAILED(Load_Vector(stream))) {	/// AnimTypes
		return(false);
	}

	Map.Load(stream);

	if (FAILED(Load_Vector(stream))) {	/// Tubes
		return(false);
	}

	if (FAILED(Load_Misc_Values(stream))) {
		return(false);
	}

	Map.Reset_All_Subzones();
	Logic.Load(stream);

	if (TacticalMap != NULL) {
		delete TacticalMap;
		TacticalMap = NULL;
	}
	Tactical * old_tactical;
	if (FAILED(OleLoadFromStream(stream, IID_IUnknown, (LPVOID *)&old_tactical))) {
		return(false);
	}

	if (FAILED(Load_Vector(stream))) {	/// HouseTypes
		return(false);
	}
	if (FAILED(Load_Vector(stream))) {	/// Houses
		return(false);
	}
	if (FAILED(Load_Vector(stream))) {	/// Units
		return(false);
	}
	if (FAILED(Load_Vector(stream))) {	/// UnitTypes
		return(false);
	}
	if (FAILED(Load_Vector(stream))) {	/// InfantryTypes
		return(false);
	}
	if (FAILED(Load_Vector(stream))) {	/// Infantry
		return(false);
	}
	if (FAILED(Load_Vector(stream))) {	/// BuildingTypes
		return(false);
	}
	if (FAILED(Load_Vector(stream))) {	/// Buildings
		return(false);
	}
	if (FAILED(Load_Vector(stream))) {	/// AircraftTypes
		return(false);
	}
	if (FAILED(Load_Vector(stream))) {	/// Aircraft
		return(false);
	}
	if (FAILED(Load_Vector(stream))) {	/// Anims
		return(false);
	}
	if (FAILED(Load_Vector(stream))) {	/// TaskForces
		return(false);
	}
	if (FAILED(Load_Vector(stream))) {	/// TeamTypes
		return(false);
	}
	if (FAILED(Load_Vector(stream))) {	/// Teams
		return(false);
	}
	if (FAILED(Load_Vector(stream))) {	/// ScriptTypes
		return(false);
	}
	if (FAILED(Load_Vector(stream))) {	/// Scripts
		return(false);
	}
	if (FAILED(Load_Vector(stream))) {	/// TagTypes
		return(false);
	}
	if (FAILED(Load_Vector(stream))) {	/// Tags
		return(false);
	}
	if (FAILED(Load_Vector(stream))) {	/// TriggerTypes
		return(false);
	}
	if (FAILED(Load_Vector(stream))) {	/// Triggers
		return(false);
	}
	if (FAILED(Load_Vector(stream))) {	/// AITriggerTypes
		return(false);
	}
	if (FAILED(Load_Vector(stream))) {	/// Actions
		return(false);
	}
	if (FAILED(Load_Vector(stream))) {	/// Events
		return(false);
	}
	if (FAILED(Load_Vector(stream))) {	/// Factories
		return(false);
	}
	if (FAILED(Load_Vector(stream))) {	/// VoxelAnimTypes
		return(false);
	}
	if (FAILED(Load_Vector(stream))) {	/// VoxelAnims
		return(false);
	}
	if (FAILED(Load_Vector(stream))) {	/// Warheads
		return(false);
	}
	if (FAILED(Load_Vector(stream))) {	/// Weapons
		return(false);
	}
	if (FAILED(Load_Vector(stream))) {	/// ParticleTypes
		return(false);
	}
	if (FAILED(Load_Vector(stream))) {	/// Particles
		return(false);
	}
	if (FAILED(Load_Vector(stream))) {	/// ParticleSystemTypes
		return(false);
	}
	if (FAILED(Load_Vector(stream))) {	/// ParticleSystems
		return(false);
	}
	if (FAILED(Load_Vector(stream))) {	/// BulletTypes
		return(false);
	}
	if (FAILED(Load_Vector(stream))) {	/// Bullets
		return(false);
	}
	if (FAILED(Load_Vector(stream))) {	/// WaypointPaths
		return(false);
	}
	if (FAILED(Load_Vector(stream))) {	/// SmudgeTypes
		return(false);
	}
	if (FAILED(Load_Vector(stream))) {	/// OverlayTypes
		return(false);
	}
	if (FAILED(Load_Vector(stream))) {	/// LightSources
		return(false);
	}
	if (FAILED(Load_Vector(stream))) {	/// BuildingLights
		return(false);
	}
	if (FAILED(Load_Vector(stream))) {	/// Sides
		return(false);
	}
	if (FAILED(Load_Vector(stream))) {	/// Tiberiums
		return(false);
	}
	if (FAILED(Load_Vector(stream))) {	/// EMPulseClass::EMPulses
		return(false);
	}
	if (FAILED(Load_Vector(stream))) {	/// SuperWeaponTypes
		return(false);
	}
	if (FAILED(Load_Vector(stream))) {	/// SuperWeapons
		return(false);
	}
	TimedSuperWeapons.Clear(); // RA2: the timed super weapons are read back as raw pointers and swizzled
	{
		int count; // RA2: as above
		SuperClass * super; // RA2: as above
		HRESULT result = stream->Read(&count, sizeof(count), NULL); // RA2: as above
		if (SUCCEEDED(result) && count > 0) { // RA2: as above
			int index; // RA2: as above
			for (index = 0; index < count; index++) { // RA2: as above
				result = stream->Read(&super, sizeof(super), NULL); // RA2: as above
				if (FAILED(result)) { // RA2: as above
					break; // RA2: as above
				}
				TimedSuperWeapons.Add(super); // RA2: as above
			}
			for (index = 0; index < count; index++) { // RA2: as above
				Swizzler.Swizzle((void **)&TimedSuperWeapons[index]); // RA2: as above
			}
		}
		if (FAILED(result)) { // RA2: as above
			return(false); // RA2: as above
		}
	}
	if (FAILED(Load_Vector(stream))) {	/// TerrainTypes
		return(false);
	}
	if (FAILED(Load_Vector(stream))) {	/// Terrains
		return(false);
	}
	if (FAILED(Load_Vector(stream))) {	/// FoggedObjectClass::FoggyObjects
		return(false);
	}
	if (FAILED(Load_Vector(stream))) {	/// AlphaShapes
		return(false);
	}
	if (FAILED(Load_Vector(stream))) {	/// Waves
		return(false);
	}
	if (!VeinholeMonsterClass::Load_All(stream)) {
		return(false);
	}
	if (!RadarEventClass::Load(stream)) {
		return(false);
	}
	if (FAILED(Load_Vector(stream))) {	/// CaptureManagerArray // RA2: as above
		return(false); // RA2: as above
	}
	if (FAILED(Load_Vector(stream))) {	/// Parasites // RA2: as above
		return(false); // RA2: as above
	}
	if (FAILED(Load_Vector(stream))) {	/// Temporals // RA2: as above
		return(false); // RA2: as above
	}
	if (FAILED(Load_Vector(stream))) {	/// SpawnManagers // RA2: as above
		return(false); // RA2: as above
	}
	AircraftTracker.Clear(); // RA2: as above
	if (FAILED(AircraftTracker.Load(stream))) { // RA2: as above
		return(false); // RA2: as above
	}
	Kamikazes.Clear(); // RA2: as above
	if (FAILED(Kamikazes.Load(stream))) { // RA2: as above
		return(false); // RA2: as above
	}
	BombList.Clear(); // RA2: as above
	if (FAILED(BombList.Load(stream))) { // RA2: as above
		return(false); // RA2: as above
	}
	if (FAILED(Load_Vector(stream))) {	/// Bombs // RA2: as above
		return(false); // RA2: as above
	}
	if (FAILED(Load_Vector(stream))) {	/// RadSites // RA2: as above
		return(false); // RA2: as above
	}

	if (Session.Type == GAME_SKIRMISH) {
		DebugString("Reading Skirmish Session.Options\n");
		if (!Session.Options.Load(stream)) {
			DebugString("\t***** FAILED!\n");
			return(false);
		}
	}

	if (FAILED(Static_Sounds_Load(stream))) { // RA2: as above
		return(false); // RA2: as above
	}
	if (FAILED(Speech_Load(stream))) { // RA2: as above
		return(false); // RA2: as above
	}
	if (FAILED(Theme.Load(stream))) { // RA2: as above
		return(false); // RA2: as above
	}

	Map.Flag_To_Redraw(GS_REDRAW_ALL);

	if (Session.Type == GAME_NORMAL) { // RA2: as above
		ScenarioSaveNameSet = true; // RA2: as above
	}

	return(true);
}





/***************************************************************************
 * Save_Misc_Values -- saves miscellaneous variables                       *
 *                                                                         *
 * INPUT:                                                                  *
 *      file      file to use for writing                                  *
 *                                                                         *
 * OUTPUT:                                                                 *
 *      true = success, false = failure                                    *
 *                                                                         *
 * WARNINGS:                                                               *
 *      none.                                                              *
 *                                                                         *
 * HISTORY:                                                                *
 *   12/29/1994 BR : Created.                                              *
 *   03/12/1996 JLB : Simplified.                                          *
 *=========================================================================*/
int Save_Misc_Values(IStream * stream)
{
	int i;
	int count;          // # ptrs in 'CurrentObject'
	//ObjectClass * ptr;  // for saving 'CurrentObject' ptrs
	HRESULT res;

	res = stream->Write(&GasSystem, sizeof(GasSystem), NULL);
	if (FAILED(res)) {
		return(res);
	}

	/*
	**	Player's House.
	*/
	res = stream->Write(&PlayerPtr, sizeof(PlayerPtr), NULL);
	if (FAILED(res)) {
		return(res);
	}

	/*
	**	Save frame #.
	*/
	res = stream->Write(&Frame, sizeof(Frame), NULL);
	if (FAILED(res)) {
		return(res);
	}

	/*
	**	Save currently-selected objects list.
	**	Save the # of ptrs in the list.
	*/
	count = CurrentObject.Count();
	res = stream->Write(&count, sizeof(count), NULL);
	if (FAILED(res)) {
		return(res);
	}

	/*
	**	Save the pointers.
	*/
	for (i = 0; i < count; i++) {
		//ptr = CurrentObject[i];
		res = stream->Write(&CurrentObject[i], sizeof(CurrentObject[i]), NULL);
		if (FAILED(res)) {
			return(res);
		}
	}

	res = stream->Write(&Ground, sizeof(Ground), NULL);
	if (FAILED(res)) {
		return(res);
	}

	IonStormClass::Save(stream);

	count = LogicTags.Count();
	res = stream->Write(&count, sizeof(count), NULL);
	if (FAILED(res)) {
		return(res);
	}

	for (i = 0; i < count; i++) {
		res = stream->Write(&LogicTags[i], sizeof(LogicTags[i]), NULL);
		if (FAILED(res)) {
			return(res);
		}
	}

	count = MapTags.Count();
	res = stream->Write(&count, sizeof(count), NULL);
	if (FAILED(res)) {
		return(res);
	}

	for (i = 0; i < count; i++) {
		res = stream->Write(&MapTags[i], sizeof(MapTags[i]), NULL);
		if (FAILED(res)) {
			return(res);
		}
	}

	res = stream->Write(&CrateShares, sizeof(CrateShares), NULL);
	if (FAILED(res)) {
		return(res);
	}

	res = stream->Write(&CrateAnims, sizeof(CrateAnims), NULL);
	if (FAILED(res)) {
		return(res);
	}

	res = stream->Write(&CrateData, sizeof(CrateData), NULL);
	if (FAILED(res)) {
		return(res);
	}

	res = stream->Write(&CrateWater, sizeof(CrateWater), NULL); // RA2: the crate water flags are saved after the crate data
	if (FAILED(res)) {
		return(res);
	}

	res = stream->Write(&MissionControl, sizeof(MissionControl), NULL);
	if (FAILED(res)) {
		return(res);
	}

	res = stream->Write(&UnlockInputFrame, sizeof(UnlockInputFrame), NULL); // RA2: the input unlock frame replaces the speech state
	if (FAILED(res)) {
		return(res);
	}

	return(S_OK);
}



/***********************************************************************************************
 * Load_Misc_Values -- Loads miscellaneous variables.                                          *
 *                                                                                             *
 * INPUT:   file  -- The file to load the misc values from.                                    *
 *                                                                                             *
 * OUTPUT:  Was the misc load process successful?                                              *
 *                                                                                             *
 * WARNINGS:   none                                                                            *
 *                                                                                             *
 * HISTORY:                                                                                    *
 *   06/24/1995 BRR : Created.                                                                 *
 *   03/12/1996 JLB : Simplified.                                                              *
 *=============================================================================================*/
int Load_Misc_Values(IStream * stream)
{
	int i;
	int count;					// # ptrs in 'CurrentObject'
	void * ptr;					// for loading ptrs
	HRESULT res;

	res = stream->Read(&GasSystem, sizeof(GasSystem), NULL);
	if (FAILED(res)) {
		return(res);
	}
	Swizzle_Pointer(&GasSystem);

	/*
	**	Player's House.
	*/
	res = stream->Read(&PlayerPtr, sizeof(PlayerPtr), NULL);
	if (FAILED(res)) {
		return(res);
	}
	Swizzle_Pointer(&PlayerPtr);

	/*
	**	Load frame #.
	*/
	res = stream->Read(&Frame, sizeof(Frame), NULL);
	if (FAILED(res)) {
		return(res);
	}

	/*
	**	Load currently-selected objects list.
	**	Load the # of ptrs in the list.
	*/
	res = stream->Read(&count, sizeof(count), NULL);
	if (FAILED(res)) {
		return(res);
	}

	/*
	**	Load the pointers.
	*/
	for (i = 0; i < count; i++) {
		res = stream->Read(&ptr, sizeof(ptr), NULL);
		if (FAILED(res)) {
			return(res);
		}
		CurrentObject.Add(*(ObjectClass **)&ptr);
	}
	for (i = 0; i < CurrentObject.Count(); i++) {
		Swizzle_Pointer(&CurrentObject[i]);
	}


	res = stream->Read(&Ground, sizeof(Ground), NULL); // RA2: the ground table is read in one piece, there is no old save game format
	if (FAILED(res)) {
		return(res);
	}

	IonStormClass::Load(stream);

	res = stream->Read(&count, sizeof(count), NULL);
	if (FAILED(res)) {
		return(res);
	}

	for (i = 0; i < count; i++) {
		res = stream->Read(&ptr, sizeof(ptr), NULL);
		if (FAILED(res)) {
			return(res);
		}
		LogicTags.Add(*(TagClass **)&ptr);
	}
	for (i = 0; i < LogicTags.Count(); i++) {
		Swizzle_Pointer(&LogicTags[i]);
	}

	res = stream->Read(&count, sizeof(count), NULL);
	if (FAILED(res)) {
		return(res);
	}

	for (i = 0; i < count; i++) {
		res = stream->Read(&ptr, sizeof(ptr), NULL);
		if (FAILED(res)) {
			return(res);
		}
		MapTags.Add(*(TagClass **)&ptr);
	}
	for (i = 0; i < MapTags.Count(); i++) {
		Swizzle_Pointer(&MapTags[i]);
	}

	res = stream->Read(&CrateShares, sizeof(CrateShares), NULL);
	if (FAILED(res)) {
		return(res);
	}

	res = stream->Read(&CrateAnims, sizeof(CrateAnims), NULL);
	if (FAILED(res)) {
		return(res);
	}

	res = stream->Read(&CrateData, sizeof(CrateData), NULL);
	if (FAILED(res)) {
		return(res);
	}

	res = stream->Read(&CrateWater, sizeof(CrateWater), NULL); // RA2: the crate water flags follow the crate data
	if (FAILED(res)) {
		return(res);
	}

	res = stream->Read(&MissionControl, sizeof(MissionControl), NULL);
	if (FAILED(res)) {
		return(res);
	}

	res = stream->Read(&UnlockInputFrame, sizeof(UnlockInputFrame), NULL); // RA2: the input unlock frame replaces the speech state
	if (FAILED(res)) {
		return(res);
	}

	return(S_OK);
}



/***************************************************************************
 * Get_Savefile_Info -- gets description, scenario #, house                *
 *                                                                         *
 * INPUT:                                                                  *
 *      id         numerical ID, for the file extension                    *
 *      buf      buffer to store description in                            *
 *      scenp      ptr to variable to hold scenario                        *
 *      housep   ptr to variable to hold house                             *
 *                                                                         *
 * OUTPUT:                                                                 *
 *      true = OK, false = error (save-game file invalid)                  *
 *                                                                         *
 * WARNINGS:                                                               *
 *      none.                                                              *
 *                                                                         *
 * HISTORY:                                                                *
 *   01/12/1995 BR : Created.                                              *
 *=========================================================================*/
bool Get_Savefile_Info(char const * name, SaveVersionInfo * info)
{
	IStoragePtr storage;
	WCHAR wname[64];

	MultiByteToWideChar(0, 0, name, -1, wname, sizeof(wname) / sizeof(WCHAR));

	HRESULT result = StgOpenStorage(wname, NULL, STGM_SHARE_EXCLUSIVE|STGM_READWRITE, NULL, 0, &storage);
	if (FAILED(result)) {
		return(false);
	}

	result = info->Load(storage);
	if (FAILED(result)) {
		return(false);
	}

	return(true);
}


/***************************************************************************
 * Reconcile_Players -- Reconciles loaded data with the 'Players' vector   *
 *                                                                         *
 * This function is for supporting loading a saved multiplayer game.       *
 * When the game is loaded, we have to figure out which house goes with    *
 * which entry in the Players vector.  We also have to figure out if       *
 * everyone who was originally in the game is still with us, and if not,   *
 * turn their stuff over to the computer.                                  *
 *                                                                         *
 * So, this function does the following:                                   *
 * - For every name in 'Players', makes sure that name is in the House     *
 *   array; if not, it's a fatal error.                                    *
 * - For every human-controlled house, makes sure there's a player         *
 *   with that name; if not, it turns that house over to the computer.     *
 * - Fills in the Player's house ID                                        *
 *                                                                         *
 * This assumes that each player MUST keep their name the same as it was   *
 * when the game was saved!  It's also assumed that the network            *
 * connections have not been formed yet, since Player[i]->Player.ID will   *
 * be invalid until this routine has been called.                          *
 *                                                                         *
 * INPUT:                                                                  *
 *      none.                                                              *
 *                                                                         *
 * OUTPUT:                                                                 *
 *      true = OK, false = error                                           *
 *                                                                         *
 * WARNINGS:                                                               *
 *      none.                                                              *
 *                                                                         *
 * HISTORY:                                                                *
 *   09/29/1995 BRR : Created.                                             *
 *=========================================================================*/
/// <summary>
/// RA2
/// </summary>
void Unused_Saveload_Stub(void) // RA2: at 0x658040, empty and unreferenced (name chosen here)
{
}


/***************************************************************************
 * MPlayer_Save_Message -- pops up a "saving..." message                   *
 *                                                                         *
 * INPUT:                                                                  *
 *      none.                                                              *
 *                                                                         *
 * OUTPUT:                                                                 *
 *      none.                                                              *
 *                                                                         *
 * WARNINGS:                                                               *
 *      none.                                                              *
 *                                                                         *
 * HISTORY:                                                                *
 *   10/30/1995 BRR : Created.                                             *
 *=========================================================================*/
void MPlayer_Save_Message(void)
{
	//char *txt = Text_String(
}

