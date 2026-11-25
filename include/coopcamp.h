/*******************************************************************************
 *                                O P E N  T S
 *******************************************************************************
 * SPDX-License-Identifier: GPL-3.0-or-later
 * Copyright 2026 OpenTS contributors
 * Copyright 2026 ra2-decomp contributors
 *
 * See LICENSE.md for applicable additional terms and warranty disclaimers.
 ******************************************************************************/

#pragma once

#include "always.h"
#include "ccini.h"
#include "vector.h"

struct MPlayerScoreType;

/*
**	RA2: one cooperative campaign as CoopCamp.ini lists it: its title, how many maps it has, the
**	three variants of each map (260 characters each), the players' and the enemies' house
**	names per map, and the difficulty name of the computer players.
*/
struct CoopCampEntry
{
	int Index; // RA2: +0x00, the entry's position in the Campaigns section
	wchar_t * Name; // RA2: +0x04, the campaign's title from the string table
	signed char MapCount; // RA2: +0x08, NumberOfCampaignMaps
	char Pad9[3];
	char * Maps; // RA2: +0x0C, MapCount records of three 260 character map names
	DynamicVectorClass<char *> Players; // RA2: +0x10, the CampaignPlayer<n> house names
	DynamicVectorClass<char *> Enemies; // RA2: +0x28, the CampaignEnemy<n> house names
	char * AIName; // RA2: +0x40, CampaignAI
};

/*
**	RA2: the list of cooperative campaigns, read from CoopCamp.ini when the object is made.
*/
class CoopCampListClass
{
	public:
		CoopCampListClass(void);
		~CoopCampListClass(void);
		bool Get_Record(int index, CoopCampEntry * * record);
		bool Get_Count(int * count) const;
		bool Copy_Names(DynamicVectorClass<wchar_t *> & names);

		CCINIClass Ini; // RA2: +0x00, CoopCamp.ini
		CoopCampEntry * Entries; // RA2: +0x50
		int Count; // RA2: +0x54
		bool Initialized; // RA2: +0x58, false when CoopCamp.ini could not be read
};

/*
**	RA2: the state of a two player cooperative campaign: which campaign, which map of it comes
**	next, the maps each of the two players drew from the three variants, and the scores added
**	up over the maps played. It is saved in coopsave.ini under a section named after the two
**	player names.
*/
class CoopCampaignClass
{
	public:
		CoopCampaignClass(void);
		CoopCampaignClass(wchar_t const * first, wchar_t const * second);
		~CoopCampaignClass(void);

		void Set_Names(wchar_t const * first, wchar_t const * second);
		bool Next_Map(void);
		bool Get_Campaign_Type(int * value);
		bool Get_Map_Count(int * value);
		bool Get_Current_Map(int * value);
		bool Get_Player_Scores(DynamicVectorClass<MPlayerScoreType *> & scores, int * time);
		bool Clear_Saved_Campaign(void);
		bool Load_Saved_Campaign(void);
		bool Save_Campaign(void);
		bool Load_Campaign(int index);
		bool Add_Score(int index, int time, MPlayerScoreType first, MPlayerScoreType second);
		bool Set_Campaign_Info(CoopCampListClass const & campaigns, int index, DynamicVectorClass<int> choices, char const * second_name, char const * first_name);
		bool Get_Map_Name(char * * name, int index);
		bool Get_First_Player_Settings(int * house, int * color);
		bool Get_Second_Player_Settings(int * house, int * color);
		bool Has_Saved_Campaign(void);
		bool Saved_Map_Is_Undefined(char * map);
		bool Send_Campaign_Options(CoopCampListClass & campaigns);

		char FirstName[20]; // RA2: +0x00, the local player's name
		int FirstHouse; // RA2: +0x14, -2 until set
		int FirstColor; // RA2: +0x18, -2 until set
		char SecondName[20]; // RA2: +0x1C, the partner's name
		int SecondHouse; // RA2: +0x30, -2 until set
		int SecondColor; // RA2: +0x34, -2 until set
		int CurrentMap; // RA2: +0x38, -1 before the first map
		char * * MapNames; // RA2: +0x3C, one 260 character name per map
		int MapCount; // RA2: +0x40
		int CampaignType; // RA2: +0x44, the index in CoopCamp.ini, -1 until set
		int Kills[2]; // RA2: +0x48, per player
		int Built[2]; // RA2: +0x50
		int Lost[2]; // RA2: +0x58
		int Score[2]; // RA2: +0x60
		int Time; // RA2: +0x68, the time the campaign has taken so far
		bool Initialized; // RA2: +0x6C
};

int __fastcall Find_Country_Index(char const * name); // RA2: 0x004904A0, the index of a house type by given or ID name, -2 for "<random>"
