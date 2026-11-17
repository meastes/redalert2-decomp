/*******************************************************************************
 *                                O P E N  T S
 *******************************************************************************
 * SPDX-License-Identifier: GPL-3.0-or-later
 * Copyright 2026 OpenTS contributors
 *
 * See LICENSE.md for applicable additional terms and warranty disclaimers.
 ******************************************************************************/

#pragma once

#include "coord.h"

#include "vector.h"

#include <unknwn.h>

class AnimClass; // RA2: the clouds and bolts
class HouseClass; // RA2: the owner of a storm

class IonStormClass
{
	public:
		static void Init(void); // RA2: 0x0051F2F0; also empties the three lists and gives the ambient light back
		static HRESULT Save(IStream * stream); // RA2: 0x0051F3F0; the state, the owner and the three lists
		static HRESULT Load(IStream * stream); // RA2: 0x0051F5E0

		static void Set_Ion_Storm_Active(bool active); // RA2: 0x0051F920
		static void Ion_Storm_Begin(int duration, int deferment, Cell cell, HouseClass * owner); // RA2: 0x0051F930, a storm aimed at a cell for a house
		static void Ion_Storm_End(void); // RA2: 0x0051FBB0; only asks the storm to end, the AI lets the last clouds play out first
		static void Ion_Storm_Stop(void); // RA2: 0x0051FBD0; the storm is over: the cell and the owner are given up and the light returns
		static int Is_Ion_Storm_Pending(void); // RA2: 0x0051FC30; a storm is running or still waiting out its deferment
		static bool Is_Ion_Storm_Active(void); // RA2: 0x0051FC50
		static void Lightning_Bolt(Cell cell); // RA2: 0x0051FC90; a cloud forms over a cell, to strike once it is half played
		static void Lightning_Strike(Coord coord); // RA2: 0x0051FE50; the bolt of a cloud lands: animation, thunder, damage and debris
		static void AI(void); // RA2: 0x00520200; the per frame logic (nuke flash, the lists, the clouds of a running storm, the deferment)
		static void Apply_Secondary_Effect(void); // RA2: 0x00520770; puts the lighting of a nuke flash or a running storm back (after a load)
		static void Start_Nuke_Flash(void); // RA2: 0x005208A0; the nuke lighting for 30 frames
		static void End_Nuke_Flash(void); // RA2: 0x00520980; back to the storm's or the scenario's lighting for 15 frames
		static void Set_Scheme_Lighting(int red, int green, int blue, bool ion_light); // RA2: 0x00520A80; the tint of the converters of the color scheme lists
		static void Set_Lighting(int red, int green, int blue, bool ion_light); // RA2: 0x00520B00; tints every tile drawer and color scheme (thousandths, -1 for the scenario's own light)
		static void Ion_Storm_Active_Message(void); // RA2: 0x00520BF0; tells the local player (Msg:LightningStormActive) that a storm is already running

	private:
		friend struct NukeFlash;

		static bool IsActive; // RA2: 0x00A51934
		static int Deferment; // RA2: 0x00A51938; frames before a deferred storm breaks
		static int NukeFlashState; // RA2: 0x00A5193C; 1 the nuke light is on, 2 it is fading out
		static HouseClass * Owner; // RA2: 0x00A51940
		static bool IsEnding; // RA2: 0x00A51944; the storm has been asked to end
		static int StartFrame; // RA2: 0x007DDB10
		static int Duration; // RA2: 0x007DDB14; -1 for a storm that never ends
		static int NukeFlashStart; // RA2: 0x007DDB18
		static int NukeFlashDelay; // RA2: 0x007DDB1C
		static Cell Coords; // RA2: 0x00A51878; the cell the storm is aimed at
		static DynamicVectorClass<AnimClass *> CloudsManifesting; // RA2: 0x00A51880; clouds that have yet to strike
		static DynamicVectorClass<AnimClass *> CloudsPresent; // RA2: 0x00A518C8
		static DynamicVectorClass<AnimClass *> BoltsPresent; // RA2: 0x00A51908
};

/*
**	RA2: the nuke flash is the storm's lighting state (0x0051FC60 and 0x0051FC70 read it).
*/
struct NukeFlash {
	static bool IsFadingIn(void); // RA2: 0x0051FC60
	static bool IsFadingOut(void); // RA2: 0x0051FC70
};
