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

/* $Header: /CounterStrike/OPTIONS.CPP 1     3/03/97 10:25a Joe_bostic $ */
/***********************************************************************************************
 ***              C O N F I D E N T I A L  ---  W E S T W O O D  S T U D I O S               ***
 ***********************************************************************************************
 *                                                                                             *
 *                 Project Name : Command & Conquer                                            *
 *                                                                                             *
 *                    File Name : OPTIONS.CPP                                                  *
 *                                                                                             *
 *                   Programmer : Joe L. Bostic                                                *
 *                                                                                             *
 *                   Start Date : June 8, 1994                                                 *
 *                                                                                             *
 *                  Last Update : July 30, 1996 [JLB]                                          *
 *                                                                                             *
 *---------------------------------------------------------------------------------------------*
 * Functions:                                                                                  *
 *   OptionsClass::Adjust_Palette -- Adjusts the palette according to the settings specified.  *
 *   OptionsClass::Fixup_Palette -- Adjusts the real palette to match the palette sliders.     *
 *   OptionsClass::Get_Brightness -- Fetches the current brightness setting.                   *
 *   OptionsClass::Get_Contrast -- Gets the current contrast setting.                          *
 *   OptionsClass::Get_Game_Speed -- Fetches the current game speed setting.                   *
 *   OptionsClass::Get_Saturation -- Fetches the current color setting.                        *
 *   OptionsClass::Get_Scroll_Rate -- Fetches the current scroll rate setting.                 *
 *   OptionsClass::Get_Tint -- Fetches the current tint setting.                               *
 *   OptionsClass::Load_Settings -- reads options settings from the INI file                   *
 *   OptionsClass::Normalize_Delay -- Normalizes delay factor to keep rate constant.           *
 *   OptionsClass::Normalize_Volume -- Convert to a real volume value.                         *
 *   OptionsClass::One_Time -- This performs any one time initialization for the options class.*
 *   OptionsClass::OptionsClass -- The default constructor for the options class.              *
 *   OptionsClass::Process -- Handles all the options graphic interface.                       *
 *   OptionsClass::Save_Settings -- writes options settings to the INI file                    *
 *   OptionsClass::Set -- Sets options based on current settings                               *
 *   OptionsClass::Set_Brightness -- Sets the brightness level to that specified.              *
 *   OptionsClass::Set_Contrast -- Sets the contrast to the value specified.                   *
 *   OptionsClass::Set_Game_Speed -- Sets the game speed as specified.                         *
 *   OptionsClass::Set_Repeat -- Controls the score repeat option.                             *
 *   OptionsClass::Set_Saturation -- Sets the color to the value specified.                    *
 *   OptionsClass::Set_Score_Volume -- Sets the global score volume to that specified.         *
 *   OptionsClass::Set_Scroll_Rate -- Sets the scroll rate as specified.                       *
 *   OptionsClass::Set_Shuffle -- Controls the play shuffle setting.                           *
 *   OptionsClass::Set_Sound_Volume -- Sets the sound effects volume level.                    *
 *   OptionsClass::Set_Tint -- Sets the tint setting.                                          *
 * - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - */

#include "always.h"
#include "geometry.h" // RA2: the header statics, in the order of the unit's initializer table
#include "rect.h"

#include "options.h"

#include "_command.h"
#include "_map.h"
#include "_rules.h"
#include "ccfile.h"
#include "ccrand.h"
#include "command.h"
#include "dbgprint.h"
#include "dsurface.h"
#include "globals.h"
#include "init.h"
#include "ipxmgr.h"
#include "keyboard.h"
#include "language\language.h"
#include "mouse.h"
#include "msgbox.h"
#include "ownrdraw.h"
#include "rules.h"
#include "session.h"
#include "stringtable.h" // RA2: for the hotkey dialog texts
#include "techno.h"
#include "theme.h"
#include "vector.h"
#include "vox.h"
#include "voc.h" // RA2: for the volume setters
#include "audlevel.h" // RA2: for the volume setters
#include "auddsnd.h" // RA2: for the volume setters

#include "diff.hh"


char const * const OptionsClass::HotkeyName = "WinHotkeys";


/// <summary>
/// RA2
/// </summary>
int __fastcall Hotkey_To_Key(int hotkey) // RA2: not in TibSun, at 0x005D87D0
{
	int key = hotkey & 0xFF;
	unsigned int mods = (hotkey >> 8) & 0xFF;
	if (mods & 4) {
		key |= KN_ALT_BIT;
	}
	if (mods & 2) {
		key |= KN_CTRL_BIT;
	}
	if (mods & 1) {
		key |= KN_SHIFT_BIT;
	}
	if (mods & 8) {
		key = 0;
	}
	return key;
}


/// <summary>
/// RA2
/// </summary>
int __fastcall Key_To_Hotkey(unsigned short key) // RA2: not in TibSun, at 0x005D8800
{
	int mods = 0;
	if (key & KN_ALT_BIT) {
		mods = 4;
	}
	if (key & KN_CTRL_BIT) {
		mods |= 2;
	}
	if (key & KN_SHIFT_BIT) {
		mods |= 1;
	}
	if (key & WWKEY_VK_BIT) {
		mods |= 8;
	}
	return (key & 0xFF) | (mods << 8);
}


/***********************************************************************************************
 * OptionsClass::OptionsClass -- The default constructor for the options class.                *
 *                                                                                             *
 *    This is the constructor for the options class. It handles setting up all the globals     *
 *    necessary for the options. This includes setting them to their default state.            *
 *                                                                                             *
 * INPUT:   none                                                                               *
 *                                                                                             *
 * OUTPUT:  none                                                                               *
 *                                                                                             *
 * WARNINGS:   none                                                                            *
 *                                                                                             *
 * HISTORY:                                                                                    *
 *   07/21/1994 JLB : Created.                                                                 *
 *=============================================================================================*/
OptionsClass::OptionsClass(void) :
	GameSpeed(3), // RA2: members are initialized in declaration order (0x5D8840)
	Difficulty(DIFF_NORMAL),
	ScrollMethod(0), // RA2: as above
	ScrollRate(3),
	AutoScroll(true),
	DetailLevel(2), // RA2: as above
	IsSidebarOnRight(true),
	SidebarCameoText(true),
	ActionLines(true),
	IsShowHidden(false), // RA2: +0x1B is cleared
	ToolTips(true),
	ScreenWidth(-1),
	ScreenHeight(-1),
	StretchMovies(false), // RA2: as above
	AllowHiResModes(false), // RA2: as above
	AllowVRAMSidebar(false), // RA2: +0x2A is cleared
	SoundVolume(.7f), // RA2: as above
	VoiceVolume(1.0f), // RA2: as above
	ScoreVolume(.5f), // RA2: as above
	IsScoreRepeat(false), // RA2: as above
	IsInGameMusic(true), // RA2: +0x39 is set
	IsScoreShuffle(false), // RA2: as above
	SoundLatency(9),
	Socket(0xFFFF), // RA2: 0xFFFF as an unsigned short
	NetCard(0),
	KeyForceMove1(KN_LALT),
	KeyForceMove2(KN_LALT),
	KeyForceAttack1(KN_LCTRL),
	KeyForceAttack2(KN_LCTRL),
	KeySelect1(KN_LSHIFT),
	KeySelect2(KN_LSHIFT),
	KeyQueueMove1(KN_Q),
	KeyQueueMove2(KN_Q)
{
	LastUnlockedSovMovie = -1; // RA2: both unlocked-movie indices start at -1
	LastUnlockedAllMovie = -1; // RA2: as above
	DestNet[0] = '\0';
}



/***********************************************************************************************
 * OptionsClass::One_Time -- This performs any one time initialization for the options class.  *
 *                                                                                             *
 *    This routine should be called only once and it will perform any initializations for the  *
 *    options class that is needed. This may include things like file loading and memory       *
 *    allocation.                                                                              *
 *                                                                                             *
 * INPUT:   none                                                                               *
 *                                                                                             *
 * OUTPUT:  none                                                                               *
 *                                                                                             *
 * WARNINGS:   Only call this routine once.                                                    *
 *                                                                                             *
 * HISTORY:                                                                                    *
 *   07/21/1994 JLB : Created.                                                                 *
 *=============================================================================================*/
void OptionsClass::One_Time(void)
{
}


/***********************************************************************************************
 * OptionsClass::Set_Shuffle -- Controls the play shuffle setting.                             *
 *                                                                                             *
 *    This routine will control the score shuffle flag. The setting to use is provided as      *
 *    a parameter. When shuffling is on, the score play order is scrambled.                    *
 *                                                                                             *
 * INPUT:   on -- Should the shuffle option be activated?                                      *
 *                                                                                             *
 * OUTPUT:  none                                                                               *
 *                                                                                             *
 * WARNINGS:   none                                                                            *
 *                                                                                             *
 * HISTORY:                                                                                    *
 *   01/19/1995 JLB : Created.                                                                 *
 *=============================================================================================*/
void OptionsClass::Set_Shuffle(bool on)
{
	IsScoreShuffle = on;
	Theme.Set_Shuffle(on);
	DebugString("ScoreShuffle is %s\n", IsScoreShuffle == true ? "ON" : "OFF");
}


/***********************************************************************************************
 * OptionsClass::Set_Repeat -- Controls the score repeat option.                               *
 *                                                                                             *
 *    This routine is used to control whether scores repeat or not. The setting to use for     *
 *    the repeat flag is provided as a parameter.                                              *
 *                                                                                             *
 * INPUT:   on -- Should the scores repeat?                                                    *
 *                                                                                             *
 * OUTPUT:  none                                                                               *
 *                                                                                             *
 * WARNINGS:   none                                                                            *
 *                                                                                             *
 * HISTORY:                                                                                    *
 *   01/19/1995 JLB : Created.                                                                 *
 *=============================================================================================*/
void OptionsClass::Set_Repeat(bool on)
{
	IsScoreRepeat = on;
	Theme.Set_Repeat(on);
	DebugString("ScoreRepeat is %s\n", IsScoreRepeat == true ? "ON" : "OFF");
}


/***********************************************************************************************
 * OptionsClass::Set_Score_Volume -- Sets the global score volume to that specified.           *
 *                                                                                             *
 *    This routine will set the global score volume to the value specified. The value ranges   *
 *    from zero to 255.                                                                        *
 *                                                                                             *
 * INPUT:   volume   -- The new volume setting to use for scores.                              *
 *                                                                                             *
 *          feedback -- Should a feedback sound effect be generated?                           *
 *                                                                                             *
 * OUTPUT:  none                                                                               *
 *                                                                                             *
 * WARNINGS:   none                                                                            *
 *                                                                                             *
 * HISTORY:                                                                                    *
 *   01/19/1995 JLB : Created.                                                                 *
 *=============================================================================================*/
void OptionsClass::Set_Score_Volume(float volume, bool feedback)
{
	ScoreVolume = MIN(volume, 1.0);
	AudioLevelSet(&AudioScoreAttribs->VolumeLevel, ScoreVolume * 16384.0f); // RA2: 0x5D8980 sets the score level, no feedback beep
	Theme.Set_Volume(ScoreVolume * 255.0);
	DebugString("ScoreVolume = %f\n", ScoreVolume); // RA2: Set_Score_Volume at 0x5D8980 has no Theme, Rule, GenericBeep and 1 more
}


/***********************************************************************************************
 * OptionsClass::Set_Sound_Volume -- Sets the sound effects volume level.                      *
 *                                                                                             *
 *    This routine will set the sound effect volume level as indicated. It can generate a      *
 *    sound effect for feedback purposes if desired. The volume setting can range from zero    *
 *    to 255. The value of 255 is the loudest.                                                 *
 *                                                                                             *
 * INPUT:   volume   -- The volume setting to use for the new value. 0 to 255.                 *
 *                                                                                             *
 *          feedback -- Should a feedback sound effect be generated?                           *
 *                                                                                             *
 * OUTPUT:  none                                                                               *
 *                                                                                             *
 * WARNINGS:   none                                                                            *
 *                                                                                             *
 * HISTORY:                                                                                    *
 *   01/19/1995 JLB : Created.                                                                 *
 *=============================================================================================*/
void OptionsClass::Set_Sound_Volume(float volume, bool feedback)
{
	SoundVolume = MIN(volume, 1.0);
	AudioLevelSet(&AudioSoundAttribs->VolumeLevel, SoundVolume * 16384.0f); // RA2: 0x5D89F0 sets the sound level
	if (feedback) {
		VocClass::PlayGlobal(Rule->GenericBeep, 1.0f, 0x2000); // RA2: as above, beeps through PlayGlobal
	}
	DebugString("SoundVolume = %f\n", SoundVolume);
}


/// <summary>
/// Sets the speech volume level.
/// This routine will set the volume that the EVA and taunt speech plays back at. It can
/// generate a feedback sound if desired -- a taunt while a game is in progress, and a plain
/// beep otherwise.
/// </summary>
/// <param name="volume">The volume setting to use for the new value. Zero to one.</param>
/// <param name="feedback">Should a feedback sound be generated?</param>
void OptionsClass::Set_Voice_Volume(float volume, bool feedback)
{
	VoiceVolume = MIN(volume, 1.0);
	Set_Speech_Volume(VoiceVolume * 255.0);
	AudioLevelSet(&AudioVoiceAttribs->VolumeLevel, VoiceVolume * 16384.0f); // RA2: 0x5D8A70 sets the voice level
	if (feedback) {
		VocClass::PlayGlobal(Rule->GenericBeep, VoiceVolume, 0x2000); // RA2: as above, beeps through PlayGlobal, no taunt
	}
	DebugString("VoiceVolume = %f\n", VoiceVolume);
}


/***********************************************************************************************
 * OptionsClass::Load_Settings -- reads options settings from the INI file                     *
 *                                                                                             *
 * INPUT:                                                                                      *
 *      none.                                                                                  *
 *                                                                                             *
 * OUTPUT:                                                                                     *
 *      none.                                                                                  *
 *                                                                                             *
 * WARNINGS:                                                                                   *
 *      none.                                                                                  *
 *                                                                                             *
 * HISTORY:                                                                                    *
 *   02/14/1995 BR : Created.                                                                  *
 *   07/03/1996 JLB : Reworked to use new INI handler.                                         *
 *   07/30/1996 JLB : Handles hotkeys.                                                         *
 *=============================================================================================*/
void OptionsClass::Load_Settings(void)
{
	DebugString("--------- Loading SUN.INI settings ---------------\n");

	/*
	**	Read in the Options values
	*/
	GameSpeed = ConfigINI.Get_Int("Options", "GameSpeed", GameSpeed);
	DebugString("GameSpeed = %d\n", GameSpeed);

	Difficulty = ConfigINI.Get_Int("Options", "Difficulty", Difficulty);
	Difficulty = Difficulty < 4 ? Difficulty : 4; // RA2: clamps with ?: (MIN/MAX do not match here)
	Difficulty = Difficulty > 0 ? Difficulty : 0; // RA2: as above
	DebugString("Difficulty = %d\n", Difficulty);

	ScrollMethod = ConfigINI.Get_Int("Options", "ScrollMethod", ScrollMethod);
	DebugString("ScrollMethod = %d\n", ScrollMethod);

	ScrollRate = ConfigINI.Get_Int("Options", "ScrollRate", ScrollRate);
	DebugString("ScrollRate = %d\n", ScrollRate);

	AutoScroll = ConfigINI.Get_Bool("Options", "AutoScroll", AutoScroll);
	DebugString("AutoScroll is %s\n", AutoScroll == true ? "ON" : "OFF");

	DetailLevel = ConfigINI.Get_Int("Options", "DetailLevel", DetailLevel);
	DetailLevel = DetailLevel < 2 ? DetailLevel : 2; // RA2: as above
	DetailLevel = DetailLevel > 0 ? DetailLevel : 0; // RA2: as above
	DebugString("DetailLevel = %d\n", DetailLevel);

	IsSidebarOnRight = true;
	DebugString("SideBar on %s\n", "RIGHT"); // RA2: the text is a constant

	SidebarCameoText = ConfigINI.Get_Bool("Options", "SidebarCameoText", SidebarCameoText);
	DebugString("Sidebar Text is %s\n", SidebarCameoText == true ? "ON" : "OFF");

	ActionLines = ConfigINI.Get_Bool("Options", "UnitActionLines", ActionLines);
	DebugString("ActionLines are %s\n", ActionLines == true ? "ON" : "OFF");

	IsShowHidden = ConfigINI.Get_Bool("Options", "ShowHidden", IsShowHidden); // RA2: [Options] ShowHidden
	DebugString("ShowHidden is %s\n", IsShowHidden == true ? "ON" : "OFF"); // RA2: as above

	ToolTips = ConfigINI.Get_Bool("Options", "ToolTips", ToolTips);
	DebugString("ToolTips are %s\n", ToolTips == true ? "ON" : "OFF");

	ScreenWidth = ConfigINI.Get_Int("Video", "ScreenWidth", ScreenWidth);
	ScreenHeight = ConfigINI.Get_Int("Video", "ScreenHeight", ScreenHeight);
	DebugString("Resolution = %d X %d\n", ScreenWidth, ScreenHeight);

	StretchMovies = ConfigINI.Get_Bool("Video", "StretchMovies", StretchMovies);
	StretchMovies = StretchMovies == true && DSurface::AllowStretchBlits == true;
	DebugString("StretchMovies is %s\n", StretchMovies == true ? "ON" : "OFF");

	AllowHiResModes = ConfigINI.Get_Bool("Video", "AllowHiResModes", AllowHiResModes);
	DebugString("AllowHiRes is %s\n", AllowHiResModes == true ? "ON" : "OFF");

	Debug_IngameModeChange = ConfigINI.Get_Bool("Video", "AllowModeToggle", Debug_IngameModeChange);

	AllowVRAMSidebar = ConfigINI.Get_Bool("Video", "AllowVRAMSidebar", AllowVRAMSidebar); // RA2: [Video] AllowVRAMSidebar

	Set_Sound_Volume(ConfigINI.Get_Float("Audio", "SoundVolume", SoundVolume), false);
	Set_Voice_Volume(ConfigINI.Get_Float("Audio", "VoiceVolume", VoiceVolume), false);
	Set_Score_Volume(ConfigINI.Get_Float("Audio", "ScoreVolume", ScoreVolume), false);

	Set_Repeat(ConfigINI.Get_Bool("Audio", "IsScoreRepeat", IsScoreRepeat));
	Set_Shuffle(ConfigINI.Get_Bool("Audio", "IsScoreShuffle", IsScoreShuffle));
	IsInGameMusic = ConfigINI.Get_Bool("Audio", "InGameMusic", IsInGameMusic); // RA2: [Audio] InGameMusic

	/*
	**	RA2: the unlocked movies are kept in [Network] NetID as two inverted numbers.
	*/
	LastUnlockedSovMovie = -1; // RA2: as above
	LastUnlockedAllMovie = -1; // RA2: as above
	wchar_t buffer[200]; // RA2: as above
	ConfigINI.Get_UnicodeString("Network", "NetID", L"", buffer, 200); // RA2: as above
	if (buffer[0] != 0) { // RA2: as above
		for (wchar_t * letter = buffer; *letter != 0; letter++) { // RA2: as above
			*letter = ~*letter; // RA2: as above
			if (*letter == 0) { // RA2: as above
				*letter = 0xFFFF; // RA2: as above
			}
		}
		wchar_t * token = wcstok(buffer, L" "); // RA2: as above
		if (token != NULL) { // RA2: as above
			LastUnlockedSovMovie = wcstol(token, NULL, 10) - 1; // RA2: as above
			token = wcstok(NULL, L" "); // RA2: as above
			if (token != NULL) { // RA2: as above
				LastUnlockedAllMovie = wcstol(token, NULL, 10) - 1; // RA2: as above
			}
		}
	}
	if (LastUnlockedSovMovie < -1 || LastUnlockedSovMovie >= 12) { // RA2: as above
		LastUnlockedSovMovie = -1; // RA2: as above
	}
	if (LastUnlockedAllMovie < -1 || LastUnlockedAllMovie >= 12) { // RA2: as above
		LastUnlockedAllMovie = -1; // RA2: as above
	}

	SoundLatency = ConfigINI.Get_Int("Audio", "SoundLatency", SoundLatency);
	DebugString("Emulated sound card latency default = %d\n", SoundLatency);

	Socket = ConfigINI.Get_Int("Network", "Socket", Socket);
	NetCard = ConfigINI.Get_Int("Network", "NetCard", NetCard);
	ConfigINI.Get_String("Network", "DestNet", "", DestNet, sizeof(DestNet));

	DebugString("--------- Complete -------------------------------\n");

	Map.Set_Cameo_Text(SidebarCameoText);
	TechnoClass::Set_Action_Lines(ActionLines);
	Setup_Dest_Net();
}



/***********************************************************************************************
 * OptionsClass::Save_Settings -- writes options settings to the INI file                      *
 *                                                                                             *
 * INPUT:                                                                                      *
 *      none.                                                                                  *
 *                                                                                             *
 * OUTPUT:                                                                                     *
 *      none.                                                                                  *
 *                                                                                             *
 * WARNINGS:                                                                                   *
 *      none.                                                                                  *
 *                                                                                             *
 * HISTORY:                                                                                    *
 *   02/14/1995 BR : Created.                                                                  *
 *   07/03/1996 JLB : Revamped and tightened up.                                               *
 *   07/30/1996 JLB : Handles hotkeys.                                                         *
 *=============================================================================================*/
void OptionsClass::Save_Settings (void)
{
	CCFileClass file("RA2.INI"); // RA2: the settings file is RA2.INI, not SUN.INI

	DebugString("Saving game settings\n");

	/*
	**	Save Options settings
	*/
	ConfigINI.Put_Int("Options", "GameSpeed", GameSpeed);
	ConfigINI.Put_Int("Options", "Difficulty", Difficulty);
	ConfigINI.Put_Int("Options", "ScrollMethod", ScrollMethod);
	ConfigINI.Put_Int("Options", "ScrollRate", ScrollRate);
	ConfigINI.Put_Bool("Options", "AutoScroll", AutoScroll);
	ConfigINI.Put_Int("Options", "DetailLevel", DetailLevel);
	ConfigINI.Put_Bool("Options", "SidebarCameoText", SidebarCameoText);
	ConfigINI.Put_Bool("Options", "UnitActionLines", ActionLines);
	ConfigINI.Put_Bool("Options", "ShowHidden", IsShowHidden); // RA2: [Options] ShowHidden is saved
	ConfigINI.Put_Bool("Options", "ToolTips", ToolTips);
	ConfigINI.Put_Int("Video", "ScreenWidth", ScreenWidth);
	ConfigINI.Put_Int("Video", "ScreenHeight", ScreenHeight);
	ConfigINI.Put_Bool("Video", "StretchMovies", StretchMovies);
	ConfigINI.Put_Float("Audio", "SoundVolume", SoundVolume);
	ConfigINI.Put_Float("Audio", "VoiceVolume", VoiceVolume);
	ConfigINI.Put_Float("Audio", "ScoreVolume", ScoreVolume);
	ConfigINI.Put_Bool("Audio", "IsScoreRepeat", IsScoreRepeat);
	ConfigINI.Put_Bool("Audio", "IsScoreShuffle", IsScoreShuffle);
	ConfigINI.Put_Int("Audio", "SoundLatency", SoundLatency);
	ConfigINI.Put_Bool("Audio", "InGameMusic", IsInGameMusic); // RA2: [Audio] InGameMusic is saved
	ConfigINI.Put_Int("Network", "Socket", Socket);
	ConfigINI.Put_Int("Network", "NetCard", NetCard);
	ConfigINI.Put_String("Network", "DestNet", DestNet);

	/*
	**	RA2: the unlocked movies go to [Network] NetID as two inverted numbers.
	*/
	wchar_t buffer[200]; // RA2: as above
	swprintf(buffer, L"%d %d", LastUnlockedSovMovie + 1, LastUnlockedAllMovie + 1); // RA2: as above
	for (wchar_t * letter = buffer; *letter != 0; letter++) { // RA2: as above
		*letter = ~*letter; // RA2: as above
		if (*letter == 0) { // RA2: as above
			*letter = 0xFFFF; // RA2: as above
		}
	}
	ConfigINI.Put_UnicodeString("Network", "NetID", buffer); // RA2: as above

	/*
	**	Write the INI data out to a file.
	*/
	ConfigINI.Save(file, false);

	Setup_Dest_Net();
}




/// <summary>
/// Sets up the network socket and destination address.
/// This routine is called once the settings have been read in. It puts the IPX socket into
/// service and, when a destination network was specified, arranges for the game to broadcast
/// across the bridge to that network as well.
/// </summary>
void OptionsClass::Setup_Dest_Net(void)
{
	if (Socket == 0xFFFF) {
		Ipx.Set_Socket(VIRGIN_SOCKET);
	} else if (Socket < 0x4000) {
		Ipx.Set_Socket(Socket + 0x4000);
	}

	int len = strlen(DestNet);
	if (len != 0) {
		char *string = new char[len + 1];
		strcpy(string, DestNet);

		/*
		**	Specify destination connection for network play
		*/
		NetNumType net;
		NetNodeType node;

		/*
		**	Scan the command-line string, pulling off each address piece
		*/
		int i = 0;
		char * p = strtok(string, ".");
		if (p != NULL) {
			while (p) {
				int x;

				sscanf(p, "%x", &x);			// convert from hex string to int
				if (i < 4) {
					net[i] = (char)x;			// fill NetNum
				} else {
					node[i-4] = (char)x;		// fill NetNode
				}
				i++;
				p = strtok(NULL, ".");
			}

			/*
			**	If all the address components were successfully read, fill in the
			**	BridgeNet with a broadcast address to the network across the bridge.
			*/
			if (i >= 4) {
				Session.IsBridge = true;
				memset(node, 0xff, sizeof(node));
				Session.BridgeNet = IPXAddressClass(net, node, 0); // RA2: IPXAddressClass has only the (net, node, port) constructor
			}
		}

	}
}


/***********************************************************************************************
 * OptionsClass::Set -- Sets options based on current settings                                 *
 *                                                                                             *
 * Use this routine to adjust the palette or sound settings after a fresh scenario load.       *
 * It assumes the values needed are already loaded into OptionsClass.                          *
 *                                                                                             *
 * INPUT:                                                                                      *
 *                                                                                             *
 * OUTPUT:                                                                                     *
 *                                                                                             *
 * WARNINGS:                                                                                   *
 *                                                                                             *
 * HISTORY:                                                                                    *
 *   06/24/1995 BRR : Created.                                                                 *
 *=============================================================================================*/
void OptionsClass::Set(void)
{
	Set_Sound_Volume(SoundVolume, false);
	Set_Voice_Volume(VoiceVolume, false);
	Set_Score_Volume(ScoreVolume, false);
	Set_Repeat(IsScoreRepeat);
	Set_Shuffle(IsScoreShuffle);
	Map.Set_Cameo_Text(SidebarCameoText);
	TechnoClass::Set_Action_Lines(ActionLines);
}


/***********************************************************************************************
 * OptionsClass::Normalize_Delay -- Normalizes delay factor to keep rate constant.             *
 *                                                                                             *
 *    This routine is used to adjust delay factors that MUST be synchronized on all machines   *
 *    but should maintain a speed as close to constant as possible. Building animations are    *
 *    a good example of this.                                                                  *
 *                                                                                             *
 * INPUT:   delay -- The normal delay factor.                                                  *
 *                                                                                             *
 * OUTPUT:  Returns with the delay to use that has been modified so that a reasonably constant *
 *          rate will result.                                                                  *
 *                                                                                             *
 * WARNINGS:   This calculation is crude due to the coarse resolution that a 1/15 second timer *
 *             allows.                                                                         *
 *                                                                                             *
 *             Use of this routine ASSUMES that the GameSpeed is synchronized on all machines. *
 *                                                                                             *
 * HISTORY:                                                                                    *
 *   06/18/1995 JLB : Created.                                                                 *
 *   06/30/1995 JLB : Handles low values in a more consistent manner.                          *
 *=============================================================================================*/
int OptionsClass::Normalize_Delay(int delay) const
{
	static int _adjust[][MAX_SPEED_SETTING + 1] = {
		{2,2,1,1,1,1,1,1},
		{3,3,3,2,2,2,1,1},
		{5,4,4,3,3,2,2,1},
		{7,6,5,4,4,4,3,2}
	};
	if (delay) {
		if (delay < 5) {
			delay = _adjust[delay-1][GameSpeed];
		} else {
			delay = ((delay * (MAX_SPEED_SETTING + 1)) / (GameSpeed+1));
		}
	}
	return(delay);
}


/***********************************************************************************************
 * OptionsClass::Normalize_Volume -- Convert to a real volume value.                           *
 *                                                                                             *
 *    This routine will take a relative volume value and convert it to the real volume value   *
 *    to use. This allows all the game volumes to be corrected to the correct global volume.   *
 *                                                                                             *
 * INPUT:   volume   -- Requested volume level.                                                *
 *                                                                                             *
 * OUTPUT:  Returns with the actual volume level to use.                                       *
 *                                                                                             *
 * WARNINGS:   none                                                                            *
 *                                                                                             *
 * HISTORY:                                                                                    *
 *   09/22/1995 JLB : Created.                                                                 *
 *=============================================================================================*/
int OptionsClass::Normalize_Volume(int volume) const
{
	return(volume * SoundVolume);
}


/*
 * Internal state-machine messages the hotkey-configuration dialog posts to
 * itself. wParam/lParam are unused; each just triggers the matching refresh.
 */
#define HKD_FILL_COMMANDS	(WM_USER + 100)		/// rebuild the command listbox for the selected category
#define HKD_SHOW_COMMAND	(WM_USER + 101)		/// refresh the description / assigned-key panel for the selected command
#define HKD_APPLY_HOTKEY	(WM_USER + 102)		/// assign the hotkey edit's key to the selected command
#define HKD_REINIT			(WM_USER + 103)		/// full refresh: repopulate the category combo and reset
#define HKD_ERROR_TEXT		1597					// RA2: 0x63D, the dialog's error line
#define HKD_OK_BUTTON		1670					// RA2: 0x686, the OK button
#define OD_GETTEXTW			(WM_USER + 179)		// RA2: 0x4B3, copies a control's wide caption (wParam = size, lParam = buffer)
#define OD_FINDSTRINGW		(WM_USER + 190)		// RA2: 0x4BE, finds a wide string in an owner-drawn combo box
#define OD_COMBOADDSTRINGW	(WM_USER + 194)		// RA2: 0x4C2, adds a wide string to an owner-drawn combo box
#define RA2_SOURCE "D:\\Ra2\\Options.CPP" // RA2: the file name RA2 passes to StringTable::LoadString
bool __fastcall Can_Use_Modified_Key(CommandClass const & command, unsigned int key); // RA2: 0x480B20, whether the command accepts the key with its modifiers
bool __fastcall Can_Use_Hotkey(unsigned int key); // RA2: 0x480B40, whether the key is free to be assigned
namespace OwnerDraw { void __fastcall Get_Keyboard_Key_String(unsigned short hotkey, wchar_t * buffer); } // RA2: 0x5FC600 (YR: UI::GetKeyboardKeyString)


/// <summary>
/// Handles the messages for the keyboard configuration dialog.
/// This routine drives the category, command and hotkey controls, and hands the reassigned
/// keys back to the hotkey command list. Accepting the dialog writes the assignments out to
/// KEYBOARD.INI; canceling puts the previous assignments back.
/// </summary>
/// <returns>Returns with TRUE if the message was consumed by this dialog.</returns>
BOOL CALLBACK Hotkey_Dialog_Proc(HWND window, UINT message, WPARAM wparam, LPARAM lparam)
{
	wchar_t buffer[64]; // RA2: wide text (0x5D97A0)
	int * retval;
	static int current_selection = -1;

	int result = OwnerDraw::Default_Dialog_Proc(window, message, wparam, lparam);
	if (result) {
		return(result);
	}

	retval = (int *)GetWindowLong(window, DWL_USER);

	switch (message) {
		case HKD_APPLY_HOTKEY: {
			HWND list_commands = GetDlgItem(window, IDC_KEY_COMMANDS);
			int selection = ListBox_GetCurSel(list_commands);
			if (selection != LB_ERR) {
				CommandClass * cmd = (CommandClass *)ListBox_GetItemData(list_commands, selection); // RA2: as above
				HWND hotkey = GetDlgItem(window, IDC_KEY_HOTKEY);
				int key = Hotkey_To_Key(SendMessage(hotkey, HKM_GETHOTKEY, 0, 0)); // RA2: the control's value is converted to a key number
				if (!Can_Use_Modified_Key(*cmd, key)) { // RA2: the command refuses the key with its modifiers
					HWND error = GetDlgItem(window, HKD_ERROR_TEXT); // RA2: error line
					SendMessage(error, OD_SETTEXTW, 0, (LPARAM)StringTable::LoadString("Error:CannotMap", NULL, RA2_SOURCE, 987)); // RA2: as above
				} else if (!Can_Use_Hotkey(key)) { // RA2: the key is reserved
					HWND error = GetDlgItem(window, HKD_ERROR_TEXT); // RA2: as above
					SendMessage(error, OD_SETTEXTW, 0, (LPARAM)StringTable::LoadString("Error:CannotRemap", NULL, RA2_SOURCE, 995)); // RA2: as above
					return(TRUE);
				} else {
					for (int i = 0; i < HotkeyCommands.Count(); i++) { // RA2: as above
						if (HotkeyCommands.Fetch_By_Position(i) == cmd) { // RA2: as above
							HotkeyCommands.Remove_Index(HotkeyCommands.Fetch_ID_By_Position(i)); // RA2: as above
							break; // RA2: as above
						}
					}
					if (key != 0) { // RA2: as above
						HotkeyCommands.Remove_Index(key); // RA2: as above
						HotkeyCommands.Add_Index(key, cmd); // RA2: as above
						return(TRUE); // RA2: as above
					}
				}
			}
			return(TRUE);
		}

		case HKD_SHOW_COMMAND: {
			HWND list_commands = GetDlgItem(window, IDC_KEY_COMMANDS);
			int selection = ListBox_GetCurSel(list_commands);
			if (selection != LB_ERR) {
				CommandClass const * cmd = (CommandClass const *)ListBox_GetItemData(list_commands, selection);
				HWND description = GetDlgItem(window, IDC_KEY_DESCRIPTION);
				SendMessage(description, OD_SETTEXTW, 0, (LPARAM)cmd->Get_Description()); // RA2: wide text through OD_SETTEXTW

				int key = 0;
				for (int i = 0; i < HotkeyCommands.Count(); i++) {
					if (HotkeyCommands.Fetch_By_Position(i) == cmd) {
						key = HotkeyCommands.Fetch_ID_By_Position(i);
						break;
					}
				}

				HWND key_label = GetDlgItem(window, IDC_KEY_CURRENT_SHORTCUT);
				OwnerDraw::Get_Keyboard_Key_String(Key_To_Hotkey(key), buffer); // RA2: as above
				SendMessage(key_label, OD_SETTEXTW, 0, (LPARAM)buffer); // RA2: as above

				HWND hotkey = GetDlgItem(window, IDC_KEY_HOTKEY);
				SendMessage(hotkey, HKM_SETHOTKEY, 0, 0);

				HWND hotkey_name = GetDlgItem(window, IDC_KEY_ASSIGNED_TO);
				SendMessage(hotkey_name, OD_SETTEXTW, 0, (LPARAM)L""); // RA2: as above
				return(TRUE);
			}
			return(TRUE);
		}

		case HKD_FILL_COMMANDS: {
			HWND cmb_category = GetDlgItem(window, IDC_KEY_CATEGORY);
			if (ComboBox_GetCurSel(cmb_category) != current_selection) {
				current_selection = ComboBox_GetCurSel(cmb_category);
				SendMessage(cmb_category, OD_GETTEXTW, 64, (LPARAM)buffer); // RA2: the category's wide caption
				HWND list_commands = GetDlgItem(window, IDC_KEY_COMMANDS);
				ListBox_ResetContent(list_commands);
				for (int i = 0; i < AllCommands.Count(); i++) {
					CommandClass const * cmd = AllCommands[i];
					if (_wcsicmp(cmd->Get_Category(), buffer) == 0) { // RA2: wide compare
						int index = SendMessage(list_commands, OD_ADDSTRINGW, 0, (LPARAM)cmd->Get_Display_Name()); // RA2: as above
						if (index != LB_ERR) {
							ListBox_SetItemData(list_commands, index, (LPARAM)cmd);
						}
					}
				}
				HWND description = GetDlgItem(window, IDC_KEY_DESCRIPTION);
				SendMessage(description, OD_SETTEXTW, 0, (LPARAM)L""); // RA2: as above
				HWND key_label = GetDlgItem(window, IDC_KEY_CURRENT_SHORTCUT); // RA2: the shortcut text is cleared too
				SendMessage(key_label, OD_SETTEXTW, 0, (LPARAM)L""); // RA2: as above
				HWND hotkey_name = GetDlgItem(window, IDC_KEY_ASSIGNED_TO); // RA2: as above
				SendMessage(hotkey_name, OD_SETTEXTW, 0, (LPARAM)L""); // RA2: as above
				ListBox_SetCurSel(hotkey_name, 0); // RA2: sent to the assigned-to control
				SendMessage(window, HKD_SHOW_COMMAND, 0, 0);
				return(TRUE);
			}
			return(TRUE);
		}

		case HKD_REINIT: {
			HWND cmb_category = GetDlgItem(window, IDC_KEY_CATEGORY);
			ComboBox_ResetContent(cmb_category);
			for (int i = 0; i < AllCommands.Count(); i++) {
				CommandClass const * cmd = AllCommands[i];
				if (SendMessage(cmb_category, OD_FINDSTRINGW, 0, (LPARAM)cmd->Get_Category()) == CB_ERR) { // RA2: wide find
					SendMessage(cmb_category, OD_COMBOADDSTRINGW, 0, (LPARAM)cmd->Get_Category()); // RA2: wide add
				}
			}
			ComboBox_SetCurSel(cmb_category, 0);
			current_selection = -1;
			SendMessage(window, HKD_FILL_COMMANDS, 0, 0); // RA2: as above
			return(TRUE);
		}

		case OD_SUBCLASSED: // RA2: message 0x497 refills the categories and lets the dialog go on
			SendMessage(window, HKD_REINIT, 0, 0);
			return(FALSE);

		case WM_COMMAND: // RA2: handled after the private messages (same code as TibSun's, moved)
			switch (LOWORD(wparam)) { // RA2: as above
				case HKD_OK_BUTTON: // RA2: control 0x686 is the OK button
					if (HIWORD(wparam) == BN_CLICKED) { // RA2: as above
						CCINIClass ini; // RA2: as above
						ini.Clear(); // RA2: as above

						for (int i = 0; i < HotkeyCommands.Count(); i++) { // RA2: as above
							CommandClass const * cmd = HotkeyCommands.Fetch_By_Position(i); // RA2: as above
							int key = HotkeyCommands.Fetch_ID_By_Position(i); // RA2: as above
							ini.Put_Int("Hotkey", cmd->Get_Unique_Name(), key); // RA2: as above
						}

						RawFileClass file("Keyboard.ini"); // RA2: as above
						ini.Save(file, false); // RA2: as above
						*retval = 1; // RA2: result 1
						return(TRUE); // RA2: as above
					}
					break; // RA2: as above

				case IDCANCEL: // RA2: as above
					if (HIWORD(wparam) == BN_CLICKED) { // RA2: as above
						Init_Hotkeys(); // RA2: as above
						*retval = 2;
						return(TRUE); // RA2: as above
					}
					break; // RA2: as above

				case IDC_KEY_COMMANDS: // RA2: as above
					if (HIWORD(wparam) == LBN_SELCHANGE) { // RA2: as above
						SendMessage(window, HKD_SHOW_COMMAND, 0, 0); // RA2: as above
						HWND hotkey = GetDlgItem(window, IDC_KEY_HOTKEY); // RA2: as above
						if (hotkey != NULL) { // RA2: as above
							SetFocus(hotkey); // RA2: as above
							return(TRUE); // RA2: as above
						}
					}
					break; // RA2: as above

				case IDC_KEY_ASSIGN: // RA2: as above
					SendMessage(window, HKD_APPLY_HOTKEY, 0, 0); // RA2: as above
					SendMessage(window, HKD_SHOW_COMMAND, 0, 0); // RA2: as above
					return(TRUE); // RA2: as above

				case IDC_KEY_HOTKEY: // RA2: as above
					if (HIWORD(wparam) == EN_CHANGE) { // RA2: as above
						HWND error = GetDlgItem(window, HKD_ERROR_TEXT); // RA2: the error line is cleared first
						SendMessage(error, OD_SETTEXTW, 0, (LPARAM)L""); // RA2: as above
						int key = Hotkey_To_Key(SendMessage((HWND)lparam, HKM_GETHOTKEY, 0, 0)); // RA2: as above
						wchar_t const * key_name; // RA2: wide text
						if (HotkeyCommands.Is_Present(key)) { // RA2: as above
							key_name = HotkeyCommands[key]->Get_Display_Name(); // RA2: as above
							if (key_name == NULL) { // RA2: as above
								key_name = L""; // RA2: as above
							}
						} else {
							key_name = L""; // RA2: as above
						}
						HWND hotkey_name = GetDlgItem(window, IDC_KEY_ASSIGNED_TO); // RA2: as above
						SendMessage(hotkey_name, OD_SETTEXTW, 0, (LPARAM)key_name); // RA2: as above
						return(TRUE); // RA2: as above
					}
					break; // RA2: as above

				case IDC_KEY_RESET_ALL: // RA2: as above
					if (HIWORD(wparam) == BN_CLICKED) { // RA2: as above
						DebugString("Deleting users KEYBOARD.INI\n"); // RA2: as above
						CCFileClass file("KEYBOARD.INI"); // RA2: as above
						file.Delete(); // RA2: as above
						Init_Hotkeys(); // RA2: as above
						SendMessage(window, HKD_REINIT, 0, 0); // RA2: as above
						return(TRUE); // RA2: as above
					}
					break; // RA2: as above

				case IDC_KEY_CATEGORY: // RA2: as above
					if (HIWORD(wparam) == CBN_SELCHANGE) { // RA2: as above
						SendMessage(window, HKD_FILL_COMMANDS, 0, 0); // RA2: as above
						return(TRUE); // RA2: as above
					}
					break; // RA2: as above
			}
			return(TRUE); // RA2: as above
	}

	return(FALSE);
}




/// <summary>
/// Displays the keyboard configuration dialog.
/// This routine brings up the hotkey assignment dialog and does not return until the player
/// dismisses it. The title screen is kept refreshed while the dialog is up outside of a
/// game.
/// </summary>
bool OptionsClass::Hotkey_Dialog(void)
{
	int res = -1; // RA2: Hotkey_Dialog at 0x5DA360 has no HWND

	HWND handle = OwnerDraw::Begin_Dialog(163, Hotkey_Dialog_Proc, 0); // RA2: Hotkey_Dialog at 0x5DA360 matches only so
	if (handle != NULL) {
		SetWindowLong(handle, DWL_USER, (LONG)&res);
		OwnerDraw::Display_Dialog(handle);

		while (res < 0) {
			if (OwnerDraw::Dialog_Message_Handler() == true) {
				res = 2;
			}
			if (!GameActive) {
				Title_Screen_Restore();
			}
		}
		OwnerDraw::End_Dialog(handle);
	}

	return(true);
}


/// <summary>
/// Fetches the tactical map scrolling method the player prefers.
/// </summary>
int OptionsClass::Get_Scroll_Method(void) const
{
	return(ScrollMethod);
}









/*
**	RA2: a campaign movie the movies dialog lists. Name is the movie file, Label the string table entry shown
**	and Side the campaign (-1 the intro, 0 Allied, 1 Soviet). The tables are 0x007E8248-0x007E8394.
*/
struct MovieEntry { // RA2: as above
	char const * Name; // RA2: +0x00, the file's base name (the lists end with a null name)
	char const * Label; // RA2: +0x04, the string table label
	int Side; // RA2: +0x08, the campaign
};

MovieEntry IntroMovieEntry = { "A00_F00E", "Name:IntroMovie", -1 }; // RA2: 0x007E8248, the intro movie (its name is the shared string at 0x007DB5F8)
MovieEntry AlliedMovieList[] = { // RA2: 0x007E8258, the Allied campaign's movies, ended by a null entry
	{ "A01_F00e", "Name:All01", 0 }, // RA2: as above
	{ "A02_F00e", "Name:All02", 0 }, // RA2: as above
	{ "A03_F00e", "Name:All03", 0 }, // RA2: as above
	{ "A04_F00e", "Name:All04", 0 }, // RA2: as above
	{ "A05_F00e", "Name:All05", 0 }, // RA2: as above
	{ "A06_F00e", "Name:All06", 0 }, // RA2: as above
	{ "A07_F00e", "Name:All07", 0 }, // RA2: as above
	{ "A08_F00e", "Name:All08", 0 }, // RA2: as above
	{ "A09_F00e", "Name:All09", 0 }, // RA2: as above
	{ "A10_F00e", "Name:All10", 0 }, // RA2: as above
	{ "A11_F00e", "Name:All11", 0 }, // RA2: as above
	{ "A12_F00e", "Name:All12", 0 }, // RA2: as above
	{ NULL, NULL, 0 } // RA2: as above
};
MovieEntry SovietMovieList[] = { // RA2: 0x007E82F8, the Soviet campaign's movies, ended by a null entry
	{ "S01_F00e", "Name:Sov01", 1 }, // RA2: as above
	{ "S02_F00e", "Name:Sov02", 1 }, // RA2: as above
	{ "S03_F00e", "Name:Sov03", 1 }, // RA2: as above
	{ "S04_F00e", "Name:Sov04", 1 }, // RA2: as above
	{ "S05_F00e", "Name:Sov05", 1 }, // RA2: as above
	{ "S06_F00e", "Name:Sov06", 1 }, // RA2: as above
	{ "S07_F00e", "Name:Sov07", 1 }, // RA2: as above
	{ "S08_F00e", "Name:Sov08", 1 }, // RA2: as above
	{ "S09_F00e", "Name:Sov09", 1 }, // RA2: as above
	{ "S10_F00e", "Name:Sov10", 1 }, // RA2: as above
	{ "S11_F00e", "Name:Sov11", 1 }, // RA2: as above
	{ "S12_F00e", "Name:Sov12", 1 }, // RA2: as above
	{ NULL, NULL, 0 } // RA2: as above
};


/// <summary>
/// Finds a movie in a list by its file name; -1 when it is not there.
/// </summary>
inline int Movie_Index(MovieEntry const * movies, char const * name) // RA2: not in TibSun
{
	for (int index = 0; movies->Name != NULL; index++, movies++) { // RA2: as above
		if (stricmp(name, movies->Name) == 0) { // RA2: as above
			return(index); // RA2: as above
		}
	}
	return(-1); // RA2: as above
}


/// <summary>
/// Notes that a campaign movie has been seen, so that the movies dialog lists it from now on.
/// </summary>
void OptionsClass::Unlock_Movie(char const * name) // RA2: 0x005DA3F0, not in TibSun
{
	int index = Movie_Index(SovietMovieList, name); // RA2: the Soviet list is searched first
	if (index > LastUnlockedSovMovie) { // RA2: as above
		LastUnlockedSovMovie = index; // RA2: as above
	}
	index = Movie_Index(AlliedMovieList, name); // RA2: then the Allied one
	if (index > LastUnlockedAllMovie) { // RA2: as above
		LastUnlockedAllMovie = index; // RA2: as above
	}
}


/// <summary>
/// Puts one movie's name into the list box, with the movie as the item's data.
/// </summary>
inline void Add_Movie(HWND list, MovieEntry const * movie) // RA2: not in TibSun
{
	if (movie->Name != NULL && movie->Label != NULL) { // RA2: as above
		int index = SendMessage(list, OD_ADDSTRINGW, 0, (LPARAM)StringTable::LoadString(movie->Label, NULL, RA2_SOURCE, 1248)); // RA2: as above
		if (index != LB_ERR) { // RA2: as above
			SendMessage(list, LB_SETITEMDATA, index, (LPARAM)movie); // RA2: as above
		}
	}
}


/// <summary>
/// Fills the movies dialog's list box with the intro and each campaign's movies that have been unlocked.
/// </summary>
void OptionsClass::Fill_Movie_List(HWND list) // RA2: 0x005DA470, not in TibSun
{
	Add_Movie(list, &IntroMovieEntry); // RA2: the intro is always listed
	if (LastUnlockedSovMovie >= 0) { // RA2: as above
		int count = LastUnlockedSovMovie + 1; // RA2: as above
		for (MovieEntry * movie = SovietMovieList; movie->Name != NULL && count > 0; movie++, count--) { // RA2: as above
			Add_Movie(list, movie); // RA2: as above
		}
	}
	if (LastUnlockedAllMovie >= 0) { // RA2: as above
		int count = LastUnlockedAllMovie + 1; // RA2: as above
		for (MovieEntry * movie = AlliedMovieList; movie->Name != NULL && count > 0; movie++, count--) { // RA2: as above
			Add_Movie(list, movie); // RA2: as above
		}
	}
}
