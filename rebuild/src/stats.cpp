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

/***********************************************************************************************
 ***              C O N F I D E N T I A L  ---  W E S T W O O D  S T U D I O S               ***
 ***********************************************************************************************
 *                                                                                             *
 *                 Project Name : Command & Conquer                                            *
 *                                                                                             *
 *                    File Name : WINSTUB.CPP                                                  *
 *                                                                                             *
 *                   Programmer : Steve Tall                                                   *
 *                                                                                             *
 *                   Start Date : 05/29/1996                                                   *
 *                                                                                             *
 *                  Last Update : May 29th 1996 [ST]                                           *
 *                                                                                             *
 *---------------------------------------------------------------------------------------------*
 * Overview:                                                                                   *
 *  Internet game statistics to collect and upload to the server                               *
 *                                                                                             *
 *                                                                                             *
 *---------------------------------------------------------------------------------------------*
 *                                                                                             *
 * Functions:                                                                                  *
 * - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - */


#include "always.h"
#include "geometry.h"
#include "rect.h"

#include "addon.h"
#include "aircraft.h"
#include "airctype.h"
#include "building.h"
#include "builtype.h"
#include "dbgprint.h"
#include "getcpu.h"
#include "globals.h"
#include "goptions.h"
#include "house.h"
#include "houstype.h"
#include "infantry.h"
#include "infatype.h"
#include "internet.h"
#include "misc.h"
#include "netshare.h"
#include "packet.h"
#include "session.h"
#include "stimer.h"
#include "timer.h"
#include "unit.h"
#include "unittype.h"
#include "win.h"
#include "wolapi.h"
#include "wonline.h"

#define FIELD_SHORT_GAME						"SHRT"
#define FIELD_SUPER_WEAPONS						"SUPR"
#define FIELD_GAME_MODE							"MODE"
#define FIELD_BUILD_OFF_ALLY_MCV				"BAMR"
#define FIELD_GAME_ID							"IDNO"
#define FIELD_START_CREDITS						"CRED"
#define FIELD_BASES								"BASE"
#define FIELD_TIBERIUM							"TIBR"
#define FIELD_CRATES							"CRAT"
#define FIELD_AI_PLAYERS						"AIPL"
#define FIELD_CAPTURE_THE_FLAG					"FLAG"
#define FIELD_START_UNIT_COUNT					"UNIT"
#define FIELD_TECH_LEVEL						"TECH"
#define FIELD_SCENARIO							"SCEN"
#define FIELD_START_TIME						"TIME"
#define FIELD_GAME_DURATION						"DURA"
#define FIELD_FRAME_RATE						"AFPS"
#define FIELD_SPEED_SETTING						"SPED"
#define FIELD_GAME_VERSION						"VERS"
#define FIELD_GAME_BUILD_DATE					"DATE"
#define FIELD_CPU_TYPE							"PROC"
#define FIELD_MEMORY							"MEMO"
#define FIELD_VIDEO_MEMORY						"VIDM"
#define FIELD_SHADOW_REGROWS					"SHAD"

#define FIELD_TOURNAMENT						"TRNY"

#define FIELD_GAME_SKU							"GSKU"

#define FIELD_STATS_PACKET_ID					"SPID"

#define FIELD_PLAYER_COUNT						"PLRS"
#define FIELD_WDT_TERRITORY						"WDTT"

#define FIELD_PINGS_RECEIVED					"PNGR"
#define FIELD_PINGS_SENT						"PNGS"

#define FIELD_GAME_FINISHED						"FINI"
#define FIELD_GAME_OUT_OF_SYNC					"OOSY"

// Note: These enums match those in the game results server code.
enum {
	COMPLETION_DISCONNECT = 1,
	COMPLETION_NO = 3,
	COMPLETION_QUIT = 4,
	COMPLETION_WON = 8,
	COMPLETION_LOST = 9,
};

/***************************************************************************
**	Internet specific globals
*/
BasicTimerClass<SystemTimerClass>	GameTimer;
int								GameEndTime;
bool								GameTimerInUse = false;
void								*PacketLater = NULL;

int __fastcall Get_Unit_Count(int *buf, int count);

#define GET_UNIT_TOTALS(tracker) (void*) player->tracker.Get_All_Totals(), Get_Unit_Count(player->tracker.Get_All_Totals(),player->tracker.Get_Unit_Count())*4

/***********************************************************************************************
 * Send_Statistics_To_Server -- sends internet game statistics to the Westeood server          *
 *                                                                                             *
 *                                                                                             *
 *                                                                                             *
 * INPUT:    Nothing                                                                           *
 *                                                                                             *
 * OUTPUT:   Nothing                                                                           *
 *                                                                                             *
 * WARNINGS: None                                                                              *
 *                                                                                             *
 * HISTORY:                                                                                    *
 *    5/29/96 12:38PM ST : Created                                                             *
 *=============================================================================================*/

void __fastcall Send_Statistics_Packet(bool alt_server)
{
//	DebugString( "Stats: Send_Statistics_Packet() called.\n" );
#ifndef INTERNET_OFF // Denzil 5/4/98

	PacketClass		stats;
	unsigned int 	playercount = 0;
	unsigned int 	statsid = 0;

	if (ObserverHouse != NULL && PlayerPtr == ObserverHouse) {
		return;
	}
	HouseClass		*player;
	static int		packet_size;
	unsigned int	i;
	void			*packet;
	unsigned int	temp;

	static char	field_player_handle[5] 				= { "NAM?" };
	static char	field_player_team[5] 				= { "SID?" };
	static char	field_player_t_team[5] 				= { "TID?" };
	static char	field_player_color[5]				= { "COL?" };
	static char	field_player_country[5]				= { "CTY?" };
	static char field_player_credits[5]				= { "CRD?" };
	static char field_player_units_left[5]			= { "UNL?" };
	static char field_player_infantry_left[5]		= { "INL?" };
	static char field_player_planes_left[5]			= { "PLL?" };
	static char field_player_buildings_left[5]		= { "BLL?" };
	static char field_player_units_bought[5] 		= { "UNB?" };
	static char field_player_infantry_bought[5] 	= { "INB?" };
	static char field_player_planes_bought[5]		= { "PLB?" };
	static char field_player_buildings_bought[5]	= { "BLB?" };
	static char field_player_units_killed[5] 		= { "UNK?" };
	static char field_player_infantry_killed[5] 	= { "INK?" };
	static char field_player_planes_killed[5]		= { "PLK?" };
	static char field_player_buildings_killed[5]	= { "BLK?" };
	static char field_player_buildings_captured[5]	= { "BLC?" };
	static char field_player_crates_found[5]		= { "CRA?" };
	static char field_player_harvested[5]			= { "HRV?" };
	static char field_player_completion[5]			= { "CMP?" };
	static char field_player_ip[5]					= { "IPA?" };
	static char field_player_clan[5]				= { "CID?" };
	static char field_player_lost_connection[5]		= { "LCN?" };

	DebugString("Sending game results.  SawCompletion=%d\n", Session.SawGameCompletion);

	/*
	**	Game ID. A unique game identifier assigned by WChat.
	*/
	stats.Add_Field(FIELD_GAME_ID, (unsigned int)GameID);

	/*
	**	Product SKU
	*/
	if (Session.Type == GAME_INTERNET && Online_Session_State == 2) {
		stats.Add_Field(FIELD_WDT_TERRITORY, Session.TournamentTerritory);
		if (alt_server) {
			stats.Add_Field(FIELD_GAME_SKU, (unsigned int)GameProduct);
		} else {
			stats.Add_Field(FIELD_GAME_SKU, (unsigned int)g_WDTGameSKU);
		}
	} else {
		stats.Add_Field(FIELD_GAME_SKU, (unsigned int)GameProduct);
	}

	/*
	**	Whether or not this was a tournament game.
	*/
	stats.Add_Field(FIELD_TOURNAMENT, (unsigned int)WestwoodOnline_Tournament);

	if (Session.Type == GAME_INTERNET && Online_Session_State == 2 && alt_server) {
		WestwoodOnline_Tournament = 0;
	}

	/*
	**
	*/
	stats.Add_Field(FIELD_GAME_OUT_OF_SYNC, (unsigned char)(Session.OutOfSync == true));

	/*
	**
	*/
	stats.Add_Field(FIELD_GAME_FINISHED, (unsigned char)(Session.SawGameCompletion == true));


	/*
	**	Game duration (seconds).
	*/
	stats.Add_Field (FIELD_GAME_DURATION, (unsigned int) GameEndTime/60);

	/*
	**	Start credits.
	*/
	stats.Add_Field(FIELD_START_CREDITS, (unsigned int)Session.Options.Credits);

	/*
	**	Bases (On/Off)
	**	Short game (On/Off)
	*/
	stats.Add_Field(FIELD_SHORT_GAME, (unsigned char)(Session.Options.ShortGame != false));

	/*
	**	Tiberium (On/Off)
	**	Superweapons (On/Off)
	*/
	stats.Add_Field(FIELD_SUPER_WEAPONS, (unsigned char)(Session.Options.SWAllowed != false));

	/*
	**	Game mode
	*/
	stats.Add_Field(FIELD_GAME_MODE, (unsigned int)Session.Options.MPModeIndex);

	/*
	**	Build off allies and MCV redeploy
	*/
	stats.Add_Field(FIELD_BUILD_OFF_ALLY_MCV, (unsigned int)(((Session.Options.BuildOffAlly == true) << 1) + (Session.Options.MCVRedeploy == true)));

	/*
	**	Crates (On/Off)
	*/
	stats.Add_Field(FIELD_CRATES, (unsigned char)(Session.Options.Goodies != false));

	/*
	**	AI Players (On/Off)
	*/
	stats.Add_Field(FIELD_AI_PLAYERS, (unsigned int)Session.Options.AIPlayers);

	/*
	**	Start unit count
	*/
	stats.Add_Field(FIELD_START_UNIT_COUNT, (unsigned int)Session.Options.UnitCount);

	/*
	**	Scenario
	*/
	stats.Add_Field(FIELD_SCENARIO, Session.ScenarioFileName);

	/*
	 * Pings sent
	 */
	stats.Add_Field(FIELD_PINGS_SENT, (unsigned int)g_PingsSent);

	/*
	 * Pings received
	 */
	stats.Add_Field(FIELD_PINGS_RECEIVED, (unsigned int)g_PingsReceived);

	/*
	**
	*/
	HouseClass *houses[MAX_PLAYERS];
	for (int h = 0; h < Houses.Count(); h++) {
		HouseClass *ptr = Houses[h];
		if (ptr->IsHuman && ptr != ObserverHouse) {
			houses[playercount] = ptr;
			playercount++;
		}
	}

	/*
	**
	*/
	for (i = 0; i < playercount; i++) {
		for (unsigned int j = 0; j < (playercount - i) - 1; j++) {
			HouseClass *ptr1 = houses[j+1];
			HouseClass *ptr2 = houses[j];
			if (strcmpi(ptr2->IniName, ptr1->IniName) > 0) {
				houses[j] = ptr1;
				houses[j+1] = ptr2;
			}
		}
	}

	/*
	**
	*/
	for (i = 1; i < playercount; i++) {
		HouseClass *ptr = houses[i];
		if (strcmpi(ptr->IniName, GameHostName) == 0) {
			HouseClass *tmp = houses[0];
			houses[0] = ptr;
			houses[i] = tmp;
		}
	}

	/*
	**
	*/
	for (i = 0; i < playercount; i++) {
		HouseClass *ptr = houses[i];
		if (strcmpi(ptr->IniName, PlayerNickname) == 0) {
			statsid = i;
			break;
		}
	}

	/*
	**	Number of players
	*/
	stats.Add_Field(FIELD_PLAYER_COUNT, (unsigned int)playercount);

	/*
	 * Statistics packet ID
	 */
	stats.Add_Field(FIELD_STATS_PACKET_ID, (unsigned int)statsid);

	/*
	 * Game completion status.
	 */
	int completion[MAX_PLAYERS];
	for (i = 0; i < playercount; i++) {
		if (!Session.SawGameCompletion) {
			completion[i] = (1 << COMPLETION_NO);
		} else {
			HouseClass * hptr = houses[i];
			if (hptr->LostConnection) {
				completion[i] = (1 << COMPLETION_DISCONNECT);
			} else if (hptr->IsGiverUpper || hptr->IsResigner) {
				completion[i] = (1 << COMPLETION_LOST) | (1 << COMPLETION_QUIT);
			} else if (hptr->IsDefeated) {
				completion[i] = (1 << COMPLETION_LOST);
			} else {
				completion[i] = (1 << COMPLETION_WON);
			}
		}
	}

	/*
	**	Game start time (GMT or Pacific?)
	*/
	stats.Add_Field (FIELD_START_TIME, (int) GameStartTime);

	/*
	**	Avg. frame rate.
	*/
	int divisor = GameEndTime / 60;
	if (divisor != 0) {
		stats.Add_Field (FIELD_FRAME_RATE, (unsigned int) Frame / (GameEndTime/60) );
	} else {
		stats.Add_Field (FIELD_FRAME_RATE, 0l);
	}

	/*
	**	CPU type
	*/
	stats.Add_Field (FIELD_CPU_TYPE, (char)CPUType);

	/*
	**	Memory
	*/
	MEMORYSTATUS	mem_info;
	mem_info.dwLength=sizeof(mem_info);
	GlobalMemoryStatus(&mem_info);
	stats.Add_Field (FIELD_MEMORY, (int)mem_info.dwTotalPhys);

	/*
	**	Video memory
	*/
	DDCAPS	video_capabilities;
	int		video_memory;

	if (DirectDrawObject){
		video_capabilities.dwSize = sizeof (video_capabilities);
		if (DD_OK == DirectDrawObject->GetCaps (&video_capabilities , NULL)){
			video_memory = video_capabilities.dwVidMemTotal;
			video_memory += 1024*1024 -1;
			video_memory &= 0xfff00000;
			stats.Add_Field (FIELD_VIDEO_MEMORY, (int) video_memory);
		}
	}

	/*
	**	Game speed setting.
	*/
	stats.Add_Field (FIELD_SPEED_SETTING, (char)Options.GameSpeed);

	/*
	 * Game version/build date
	 */
	char	version[128];
	wsprintf (version, "V%s", VerNum.Version_Name() );
	stats.Add_Field (FIELD_GAME_VERSION, (char*)version);

	char path_to_exe[280];
	FILETIME write_time;		//File time is 64 bits

	GetModuleFileName (ProgramInstance, path_to_exe, sizeof(path_to_exe));
	RawFileClass file;
	file.Set_Name(path_to_exe);
	file.Open();
	HANDLE handle = file.Get_File_Handle();

	if (handle != INVALID_HANDLE_VALUE) {
		if (GetFileTime (handle, NULL, NULL, &write_time)){
			write_time.dwLowDateTime = htonl (write_time.dwLowDateTime);
			write_time.dwHighDateTime = htonl (write_time.dwHighDateTime);
			stats.Add_Field (FIELD_GAME_BUILD_DATE, (void*)&write_time, sizeof (write_time));
		}
	}

	/*
	**	Build the player specific statistics
	**
	*/
	for (i = 0; i < playercount; i++) {
		player = houses[i];
		char digit = '0' + (char)i;

		/*
		**	Player handle.
		*/
		field_player_handle[3] = digit;
		stats.Add_Field (field_player_handle, (char*) player->IniName);

		/*
		 * Player IP Address.
		 */
		temp = player->IPAddress;
		field_player_ip[3] = digit;
		stats.Add_Field (field_player_ip, (unsigned int)(temp));

		/*
		 * Player clan.
		 */
		temp = player->SquadID;
		field_player_clan[3] = digit;
		stats.Add_Field (field_player_clan, (unsigned int)(temp));

		/*
		**	Player team. (NOD or GDI)
		*/
		field_player_team[3] = digit;
		stats.Add_Field (field_player_team, (unsigned int)player->ActLike);

		/*
		 * Player team.
		 */
		field_player_t_team[3] = digit;
		temp = i;
		if (WestwoodOnline_Tournament == 2) {
			temp = player->SquadID;
		}
		stats.Add_Field (field_player_t_team, (unsigned int)(temp));

		/*
		 * Player completion.
		 */
		field_player_completion[3] = digit;
		stats.Add_Field (field_player_completion, (unsigned int)(completion[i]));

		/*
		 * LCN
		 */
		field_player_lost_connection[3] = digit;
		bool lcn = false;
		if (player->LostConnection && !player->IsResigner) {
			lcn = true;
		}
		stats.Add_Field (field_player_lost_connection, (unsigned char)(lcn));

		/*
		**	Player color
		*/
		field_player_color[3] = digit;
		stats.Add_Field (field_player_color, (unsigned int) (player->Scheme));

		/*
		**	Player country.
		*/
		field_player_country[3] = digit;
		stats.Add_Field (field_player_country, (unsigned int)player->Class->House);

		/*
		**	Player end credits.
		*/
		field_player_credits[3] = digit;
		stats.Add_Field (field_player_credits, (int)player->Available_Money());

		/*
		**	Number of each unit/building type built
		*/
		field_player_infantry_bought[3] = digit;
		field_player_units_bought[3] = digit;
		field_player_planes_bought[3] = digit;
		field_player_buildings_bought[3] = digit;

		player->InfantryTotals.To_Network_Format();
		player->UnitTotals.To_Network_Format();
		player->AircraftTotals.To_Network_Format();
		player->BuildingTotals.To_Network_Format();

		stats.Add_Field (field_player_infantry_bought, GET_UNIT_TOTALS(InfantryTotals));
		stats.Add_Field (field_player_units_bought, GET_UNIT_TOTALS(UnitTotals));
		stats.Add_Field (field_player_planes_bought, GET_UNIT_TOTALS(AircraftTotals));
		stats.Add_Field (field_player_buildings_bought, GET_UNIT_TOTALS(BuildingTotals));

		player->InfantryTotals.To_PC_Format();
		player->UnitTotals.To_PC_Format();
		player->AircraftTotals.To_PC_Format();
		player->BuildingTotals.To_PC_Format();

		/*
		**	Clear out the counts and use the space to count up the current number of units/buildings
		*/
		player->InfantryTotals.Clear_Unit_Total();
		player->AircraftTotals.Clear_Unit_Total();
		player->UnitTotals.Clear_Unit_Total();
		player->BuildingTotals.Clear_Unit_Total();

		/*
		**	Number of units remaining to player
		*/
		int index;
		for (index = 0; index < Units.Count(); index++) {
			UnitClass const * unit = Units[index];
			if (player == unit->House) {
				player->UnitTotals.Increment_Unit_Total (unit->Class->HeapID);
			}
		}

		for (index = 0; index < Infantry.Count(); index++) {
			InfantryClass const * infantry = Infantry[index];
			if (player == infantry->House && !infantry->Class->IsCivilian) {
				player->InfantryTotals.Increment_Unit_Total (infantry->Class->HeapID);
			}
		}

		for (index = 0; index < Aircraft.Count(); index++) {
			AircraftClass const * aircraft = Aircraft[index];
			if (player == aircraft->House) {
				player->AircraftTotals.Increment_Unit_Total (aircraft->Class->HeapID);
			}
		}

		int count = player->Buildings.Count();
		for (index = 0; index < count; index++) {
			BuildingClass const * building = player->Buildings[index];
			player->BuildingTotals.Increment_Unit_Total (building->Class->HeapID);
		}

		player->InfantryTotals.To_Network_Format();
		player->UnitTotals.To_Network_Format();
		player->AircraftTotals.To_Network_Format();
		player->BuildingTotals.To_Network_Format();

		field_player_infantry_left[3] = digit;
		field_player_units_left[3] = digit;
		field_player_planes_left[3] = digit;
		field_player_buildings_left[3] = digit;

		stats.Add_Field (field_player_infantry_left, GET_UNIT_TOTALS(InfantryTotals));
		stats.Add_Field (field_player_units_left,GET_UNIT_TOTALS(UnitTotals));
		stats.Add_Field (field_player_planes_left, GET_UNIT_TOTALS(AircraftTotals));
		stats.Add_Field (field_player_buildings_left, GET_UNIT_TOTALS(BuildingTotals));

		/*
		**	Number of enemy units/buildings of each type destroyed.
		*/
		player->DestroyedInfantry.To_Network_Format();
		player->DestroyedUnits.To_Network_Format();
		player->DestroyedAircraft.To_Network_Format();
		player->DestroyedBuildings.To_Network_Format();

		field_player_infantry_killed[3] = digit;
		field_player_units_killed[3] = digit;
		field_player_planes_killed[3] = digit;
		field_player_buildings_killed[3] = digit;

		stats.Add_Field (field_player_infantry_killed, GET_UNIT_TOTALS(DestroyedInfantry) /* RA2: the count comes from the same tracker */);
		stats.Add_Field (field_player_units_killed,GET_UNIT_TOTALS(DestroyedUnits) /* RA2: the count comes from the same tracker */);
		stats.Add_Field (field_player_planes_killed, GET_UNIT_TOTALS(DestroyedAircraft) /* RA2: the count comes from the same tracker */);
		stats.Add_Field (field_player_buildings_killed, GET_UNIT_TOTALS(DestroyedBuildings) /* RA2: the count comes from the same tracker */);

		/*
		**	Number and type of enemy buildings captured
		*/
		field_player_buildings_captured[3] = digit;
		player->CapturedBuildings.To_Network_Format();
		stats.Add_Field (field_player_buildings_captured, GET_UNIT_TOTALS(CapturedBuildings));

		/*
		**	Number of crates discovered and their contents
		*/
		field_player_crates_found[3] = digit;
		player->TotalCrates.To_Network_Format();
		stats.Add_Field (field_player_crates_found, GET_UNIT_TOTALS(TotalCrates));

		/*
		**	Amount of tiberium turned into credits
		*/
		field_player_harvested[3] = digit;
		stats.Add_Field (field_player_harvested, (unsigned int) player->HarvestedCredits);
	}

	/*
	**	Create the comms packet to be sent
	*/
	packet = stats.Create_Comms_Packet(packet_size);

	/*
	**	Send it.....
	*/
	if (g_pNetUtil != NULL) {
		if (alt_server) {
			g_pNetUtil->RequestGameresSend(g_WDTServerHost, g_WDTServerPort, (unsigned char*)packet, packet_size);
		} else {
			g_pNetUtil->RequestGameresSend(g_GameServerHost, g_GameServerPort, (unsigned char*)packet, packet_size);
		}
	}

	/*
	**	Save it to disk as well so I can see it
	*/
//	RawFileClass anotherfile ("packet.net");
//	anotherfile.Write(packet, packet_size);
//DebugString( "Wrote out packet.net\n" );

	/*
	**	Tidy up
	*/
	delete [] packet;

	GameStatisticsPacketSent = true;
#endif // INTERNET_OFF
}


/// <summary>
/// Determines how much of a unit tracker's totals is worth sending.
/// This routine is used by the statistics packet builder to trim the unused tail off a
/// tracker's totals, so that only the meaningful head of the array is transmitted to the
/// server.
/// </summary>
/// <param name="buf">The tracker totals to examine.</param>
/// <param name="count">The number of entries held in the tracker totals.</param>
/// <returns>Returns with the entry count up to and including the last non-zero total.
/// Zero is returned if the tracker recorded nothing at all.</returns>
int __fastcall Get_Unit_Count(int *buf, int count)
{
	int n = 0;
	for (int i = 0; i < count; i++) {
		if (buf[i] != 0) {
			n = i + 1;
		}
	}
	return(n);
}


/// <summary>
/// Starts the game duration timer.
/// This routine is called as the game begins so that the statistics gathering code has a
/// running clock to measure the game against.
/// </summary>
void __fastcall Register_Game_Start_Time(void)
{
	GameTimer = 0;
	GameTimerInUse = true;
}


/// <summary>
/// Stops the game duration timer.
/// This routine is called as the game ends and records how long the game ran, so that the
/// statistics packet can report the duration and the average frame rate.
/// </summary>
void __fastcall Register_Game_End_Time(void)
{
	GameEndTime = GameTimer;
	GameTimerInUse = false;
}
