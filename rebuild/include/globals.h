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

#pragma once

#include "coord.h"
#include "ftimer.h"

#include "action.hh"
#include "anim.hh"
#include "armor.hh"
#include "crate.hh"
#include "ground.hh"
#include "house.hh"
#include "land.hh"
#include "mission.hh"
#include "source.hh"
#include "special.hh"
#include "speed.hh"
#include "theater.hh"
#include "vox.hh"

/*
**	Forward declarations.
*/
template<class T> class DynamicVectorClass;
template<class I, class T> class IndexClass;
template<class T, int> class QueueClass;
template<class T> class CDTimerClass;
class AITriggerTypeClass;
class AbstractTypeClass;
class AircraftClass;
class AircraftTypeClass;
class AnimClass;
class AnimTypeClass;
class BuildingClass;
class BuildingLightClass;
class BuildingTypeClass;
class BulletClass;
class BulletTypeClass;
class CampaignClass;
class FactoryClass;
class HouseClass;
class HouseTypeClass;
class InfantryClass;
class InfantryTypeClass;
class IsometricTileTypeClass;
class ObjectClass;
class OverlayClass;
class OverlayTypeClass;
class ParticleClass;
class ParticleSystemClass;
class ParticleSystemTypeClass;
class ParticleTypeClass;
class ScriptTypeClass;
class SideClass;
class SmudgeClass;
class SmudgeTypeClass;
class SuperClass;
class SuperWeaponTypeClass;
class TaskForceClass;
class TeamClass;
class TeamTypeClass;
class TechnoClass;
class TechnoTypeClass;
class TerrainClass;
class TerrainTypeClass;
class TriggerClass;
class TriggerTypeClass;
class TubeClass;
class UnitClass;
class UnitTypeClass;
class VoxelAnimTypeClass;
class WaveClass;
class MissionControlClass;
class RulesClass;
class TagClass;
class ScenarioClass;

class GetCDClass;
class RandomStraw;
class VersionClass;
class GameOptionsClass;
class ThemeClass;
class SessionClass;
class NullModemClass;
class IPXManagerClass;
class Buffer;
class TiberianSunClassFactory;
class SpecialClass;
class EventClass;
class FrameTimerClass;

extern bool ScenarioActive;
extern bool Debug_ForceScenario;
extern bool Debug_Create_Maps;
extern bool Debug_SpeedBuild;
extern bool Debug_Inert;
extern bool Debug_MotionCapture;
extern bool Debug_IngameModeChange;
extern bool Debug_Quiet;
extern bool Debug_Cheat;
extern bool Debug_Remap;
extern bool Debug_Flag;
extern bool Debug_Lose;
extern bool Debug_Map;
extern bool Debug_Win;
extern bool Debug_Icon;
extern bool Debug_Passable;
extern bool Debug_Unshroud;
extern bool Debug_Threat;
extern bool Debug_Find_Path;
extern bool Debug_Check_Map;
extern bool Debug_Playtest;
extern bool Debug_Trap_Check_Heap;
extern bool Debug_Modem_Dump;
extern bool Debug_Print_Events;
extern bool Debug_Console;

extern ParticleSystemClass *GasSystem;

extern int NewINIFormat;

/*
**	Dynamic global variables (these change or are initialized at run time).
*/
extern MissionControlClass			MissionControl[MISSION_COUNT];
extern IndexClass<int, char *> 		TutorialText;
extern GetCDClass					CDList;
extern int							MapTriggerID;
extern int							LogicTriggerID;
extern RandomStraw					CryptRandom;
extern ScenarioClass *				Scen;
extern VersionClass					VerNum;
extern bool							ScoresPresent;
extern bool							DrawShapeShadows;
extern int							TournamentTime;
extern bool							AllowVoice;
extern VoxType						SpeakQueue;
extern bool							PlayerWins;
extern bool							PlayerLoses;
extern bool							PlayerRestarts;
extern bool							PlayerAborts;
extern int							Frame;
extern VoxType						SpeechRecord[1];
extern void *						SpeechBuffer[1];
extern GameOptionsClass 			Options;
extern ThemeClass 					Theme;
extern SpecialClass 				Special;

/*
**	Game object allocation and tracking classes.
*/
extern DynamicVectorClass<AITriggerTypeClass *>			AITriggerTypes;
extern DynamicVectorClass<TriggerTypeClass *>			TriggerTypes;
extern DynamicVectorClass<TriggerClass *>				Triggers;
extern DynamicVectorClass<BuildingLightClass *>			BuildingLights;
extern DynamicVectorClass<ParticleSystemClass *>		ParticleSystems;
extern DynamicVectorClass<TubeClass *>					Tubes;
extern DynamicVectorClass<WaveClass *>					Waves;
extern DynamicVectorClass<BulletClass *>				Bullets;
extern DynamicVectorClass<OverlayClass *>				Overlays;
extern DynamicVectorClass<ParticleClass *>				Particles;
extern DynamicVectorClass<SmudgeClass *>				Smudges;
extern DynamicVectorClass<TerrainClass *>				Terrains;
extern DynamicVectorClass<InfantryClass *>				Infantry;
extern DynamicVectorClass<BuildingClass *>				Buildings;
extern DynamicVectorClass<AircraftClass *>				Aircraft;
extern DynamicVectorClass<UnitClass *>					Units;
extern DynamicVectorClass<HouseClass *>					Houses;
extern DynamicVectorClass<AnimClass *>					Anims;
extern DynamicVectorClass<TeamClass *>					Teams;
extern DynamicVectorClass<FactoryClass *>				Factories;
extern DynamicVectorClass<CampaignClass *>				Campaigns;
extern DynamicVectorClass<SideClass *>					Sides;
extern DynamicVectorClass<HouseTypeClass *>				HouseTypes;
extern DynamicVectorClass<TaskForceClass *>				TaskForces;
extern DynamicVectorClass<TeamTypeClass *>				TeamTypes;
extern DynamicVectorClass<ScriptTypeClass *>			ScriptTypes;
extern DynamicVectorClass<OverlayTypeClass *>			OverlayTypes;
extern DynamicVectorClass<SmudgeTypeClass *>			SmudgeTypes;
extern DynamicVectorClass<TerrainTypeClass *>			TerrainTypes;
extern DynamicVectorClass<BuildingTypeClass *>			BuildingTypes;
extern DynamicVectorClass<AircraftTypeClass *>			AircraftTypes;
extern DynamicVectorClass<UnitTypeClass *>				UnitTypes;
extern DynamicVectorClass<AnimTypeClass *>				AnimTypes;
extern DynamicVectorClass<InfantryTypeClass *>			InfantryTypes;
extern DynamicVectorClass<BulletTypeClass *>			BulletTypes;
extern DynamicVectorClass<VoxelAnimTypeClass *>			VoxelAnimTypes;
extern DynamicVectorClass<IsometricTileTypeClass *>		IsometricTileTypes;
extern DynamicVectorClass<ParticleTypeClass *>			ParticleTypes;
extern DynamicVectorClass<ParticleSystemTypeClass *>	ParticleSystemTypes;
extern DynamicVectorClass<SuperWeaponTypeClass *>		SuperWeaponTypes;
extern DynamicVectorClass<SuperClass *>					SuperWeapons;
extern DynamicVectorClass<TechnoClass *>				Technos;
extern DynamicVectorClass<TechnoTypeClass *>			TechnoTypes;
extern DynamicVectorClass<ObjectClass *>				Objects;
extern DynamicVectorClass<AbstractTypeClass *>			AbstractTypes;

extern DynamicVectorClass<AnimClass *>					MoveFlashes;

extern QueueClass<EventClass, MAX_EVENTS>				OutList;
extern QueueClass<EventClass, (MAX_EVENTS * 128)>		DoList; // RA2: 16384 events

#ifdef MIRROR_QUEUE
extern QueueClass<EventClass, (MAX_EVENTS * 2)>		MirrorList; // RA2: 256 events
#endif

extern DynamicVectorClass<ObjectClass *>				CurrentObject;
extern DynamicVectorClass<TagClass *>					LogicTags;
extern DynamicVectorClass<TagClass *>					MapTags;

/*
**	Miscellaneous globals.
*/
extern HousesType					Whom;

//extern _VQAConfig					AnimControl;
extern int							SpareTicks;
extern int							PathCount;
extern int							CellCount;
extern int							TargetScan;
extern int							SidebarRedraws;
//extern DMonoType					MonoPage;
extern bool							GameActive;
extern bool							SpecialFlag;
extern int							ScenarioInit;
extern HouseClass *					PlayerPtr;
extern HouseClass *					ObserverHouse;		// RA2: the house of an observing player
extern bool							ThumbWasActive;		// RA2: the sidebar thumb was open when the player was defeated
extern bool							DiplomacyChatFlags[8];	// RA2: the players chat messages go to
namespace Game { extern int PlayerColor; }				// RA2: the local player's color index

extern int							BuildLevel;
extern unsigned int				ScenarioCRC;

#ifdef _DEBUG
extern Cell 						CurrentCell;
#endif

extern SessionClass				Session;
extern NullModemClass			NullModem;
extern IPXManagerClass			Ipx;

#if (TIMING_FIX)
extern int										NewMaxAheadFrame1;
extern int										NewMaxAheadFrame2;
#endif

extern bool VisceroidsAsSnoBees;
extern bool Just4Fun;

extern int 							_dialog_count;
extern int							Seed;
extern int							CustomSeed;
extern bool							IgnoreInput;
extern bool							drag_select_aborted;

extern GroundType  				Ground[LAND_COUNT];

extern int LParam;

/*
**	Constant externs (data is not modified during game play).
*/
extern char const *							LandName[LAND_COUNT];
extern char const *							SpeedName[SPEED_COUNT];
extern double								CrateData[CRATE_COUNT];
extern char const * const					CrateNames[CRATE_COUNT];
extern int									CrateShares[CRATE_COUNT];
extern AnimType								CrateAnims[CRATE_COUNT];
extern char const * const					ArmorName[ARMOR_COUNT];
extern char const * const					ActionName[ACTION_COUNT];
extern Coord const 							StoppingCoordAbs[5];

extern SpecialDialogType	SpecialDialog;

extern int LogLevel;
extern unsigned int LogLevelTime[ MAX_LOG_LEVEL ];
extern unsigned int LogLastTime;

extern TheaterType LastTheater;

extern bool _special_dialog_flag;

/*
 * These used to be defined in the header, polluting every module. Moved to globals.cpp to not do that.
 */
extern const double ONE_RAD;
extern const double RAD_45;
extern const double RAD_60;
extern const double RAD_90;

extern const double CELL_LEPTON_DIAG;

extern const double ISO_TILE_SIZE;
#ifndef GEOMETRY_H // geometry.h defines these itself
const int ISO_TILE_PIXEL_W = 60; // RA2: fixed tile size, folded into the code
const int ISO_TILE_PIXEL_H = 30;

const int LEVEL_LEPTON_H = (int)(fastmath::tan(RAD_90 - RAD_60) * CELL_LEPTON_DIAG * 0.5); // RA2: computed in every module, as in the original
#endif
extern const int LEVEL_PIXEL_H;
extern const double CELL_SLOPE_ANGLE;
extern const double CELL_DIAG_SLOPE_ANGLE;
extern const int LEVEL_PIXEL_H_1;
extern const int BRIDGE_LEPTON_HEIGHT;


#ifndef GEOMETRY_NONE_DEFINED // geometry.h defines its own copies; redeclaring them extern after that makes VC6 register an empty initializer for each
extern const Cell CELL_NONE;
extern const Coord COORD_NONE;
#endif


extern char BuildNumber[64];
extern char BuildDate[64];

/************************************************************
**	Win32 specific externs
*/
extern int ReadyToQuit;							//Are we about to exit cleanly
extern bool IsOldSaveGame;
void Memory_Error_Handler(void);				//Memory error handler function

extern bool TacticalActive;
struct IDirect3DDevice2;
extern bool UseDirect3D; // RA2: a Direct3D device is in use
extern IDirect3DDevice2 * Direct3DDevice; // RA2
void Release_Direct3D(void); // RA2: releases the viewport and the device that Init_Direct3D (0x004B1340) created, at 0x004B1470

extern CDTimerClass<FrameTimerClass> TournamentTimer;

extern char Debug_ScenarioName[];

extern Buffer * UnkBuffer;

extern TiberianSunClassFactory TibSunFactory;

extern "C"{
	extern int FPU_FLAG;
}

extern bool CursorCheat; // RA2: toggled by the CursorCheat debug command; tooltips then show without delay

class CCINIClass;
class ShapeButtonClass;
class ShapeSet;
class MixFileClass;
class MultiplayerTeam;
// Declarations moved from the units that used them (one declaration per name).
class RGBClass;
class SchemeLibrary;
extern char * __fastcall Wide_To_Narrow(wchar_t const * wide); // RA2: 0x006FA120, a static buffer
extern wchar_t * __fastcall Narrow_To_Wide(char const * narrow); // RA2: 0x006FA180, a static buffer
extern RGBClass TooltipColor; // RA2: the outline colour of a tooltip
extern int ObserverMode; // RA2: 0x00A72C68, nonzero for an observer
extern void * PacketLater; // RA2: 0x00ABD530, cleared when an internet game starts
extern bool ConnectionLost; // RA2: 0x00ABD500 (defined in queue.cpp)
extern int ProbeDelay; // RA2: 0x007D333C, 1200 ticks between two port probes
extern bool g_ConnectionLost; // RA2: 0x00B2ABDA (wonline.cpp), the online connection is gone
extern unsigned long COMRefCount; // the COM objects alive (ULONG)
extern int ManglerCount; // RA2: 0x00B2AC34, how many mangler servers the WOL server list gave
extern int ManglerIndex; // RA2: 0x00B2AC38, the one picked from them at random
extern int RouterType; // RA2: 0x00B2AC30, 1 with -DLINK1, 2 with -NETGEAR
extern char ManglerAddresses[64][128]; // RA2: 0xB26CBC, WOL mangler server addresses (filled from the server list)
extern int ManglerPorts[64]; // RA2: 0xB291A4, their ports
extern int MuteSWLaunches; // RA2: 0x00A3D548, nonzero while the launch announcements are muted (as the other files declare it)
extern char const * const Difficulty_Labels[3]; // RA2: 0x007D8404, as init.cpp declares it
extern bool GameControlsShown; // RA2: 0x00A40D84, set by -SPEEDCONTROL, as init.cpp declares it
extern int const ScreenWidths[]; // RA2: {640, 800, 1024} at 0x007ADD68
extern int const ScreenHeights[]; // RA2: {480, 600, 768} at 0x007ADD74
extern bool ScenarioSaveNameSet; // RA2: 0x00A6EBD0, as in mainloop.cpp
extern bool InMainLoop; // RA2: as above
extern int ChatTarget; // RA2: 0x00A6EBE0; who a typed message goes to: 1 everybody, 2 allies, 3 one player, 0 nobody
extern char LastPagerName[]; // RA2: 0x00B2AC48; who paged last
extern unsigned int DebugTime; // RA2: 0x007EC498; milliseconds shown at the top of the screen, -1 for none
extern int LastNetFrameTimer; // RA2: 0x00A6EBCC; the net frame timer as Main_Loop first set it
extern int UnlockInputFrame; // RA2: 0x00AB9174; the frame input unlocks itself at
extern int ScrollKeys; // RA2: 0x00A6EBDC; the scroll direction keys now held
extern int ScrollSpeed; // RA2: 0x007DFB70
extern float ScrollBoost; // RA2: 0x007DFB74
extern int MapSizeX; // RA2: 0x008325D0
extern int MapSizeY; // RA2: 0x008325D4
extern int LobbyObserverSlot; // RA2: 0x007E6D90 (netdlg2.cpp), the lobby slot of the observer
extern int GameID; // RA2: 0x00B2A7FC, the game's number for the statistics (TXT_GAME_ID)
extern int GameProduct; // RA2: 0x00B2AB84, the product the statistics are sent for (set from g_GameSKU)
extern char PlayerNickname[]; // RA2: 0x00B2A6D8, the nickname of the local player, copied from g_NickName
extern int PlayerPings[8]; // RA2: 0x00B2A734, the ping of each player of the game
extern char Squad1Names[]; // RA2: 0x00B2A820, comma-separated names of the first fixed squad (session.cpp)
extern char Squad2Names[]; // RA2: 0x00B293D8, the second squad (session.cpp)
extern int g_GameSKU; // RA2: 0x007FD370 (wonline.cpp)
extern unsigned short primeTable[3511];
extern int SidebarMovieX; // RA2: 0x008336CC
extern int SidebarMovieY; // RA2: 0x008336D0
extern bool MovieAudioSuspended; // RA2: set while the movie has the audio suspended
extern "C" int __cdecl _wcsicmp(wchar_t const * left, wchar_t const * right); // RA2: the category compare
extern SchemeLibrary * TheSchemeLibrary; // RA2: 0x00A76414
extern bool IsReadingAudioFile; // RA2: set while the streamer reads from a file
extern int DeformPointXAdd;
extern int DeformPointYAdd;
extern int DeformPointWidth;
extern int DeformPointHeight;
extern bool Asm_Ripple_Deform_Points(int startpointx, int startpointy, int general_direction, bool forced);
extern int g_WDTGameSKU; // RA2: 0x00B2668C, 0x2300, the product of the tournament ladder, set at game start in wonline.cpp
extern long GameStartTime; // RA2: 0x00B2A638, time(0) when the game started
extern char GameHostName[]; // RA2: 0x00B2A808, the name of the first user of the channel, the host
extern char g_WDTServerHost[]; // RA2: 0x00B2AB00, the address of the second results server, from the server list
extern int g_WDTServerPort; // RA2: 0x00B28DD4, its port
extern int Online_Session_State; // RA2: 0x00A3D2A4, 2 in a tournament session (as in ownrdraw.cpp)
extern "C" unsigned int __cdecl _mbsnccnt(const unsigned char * string, unsigned int count);
extern int WideScreenThreshold; // RA2: 0x007ADD70
extern int Online_Session_Return; // RA2: 0x00A3D2A8, the state the online menu (4) returns to; cleared whenever the session ends
extern bool g_PlayingNetGame;
extern int WestwoodOnline_PortNumber;
extern bool IsCameoRefreshPending; // RA2: 0x8373C2; set when an infiltration changes the buildable cameos, the sidebar then refreshes them
extern bool BuildingRunAwayOnDamage; // RA2: 0x7CF670; when set, objects in radio contact within two cells of a building that is hit run away from it
extern bool const ActionLineDash[]; // RA2: 0x7F7130, the dash pattern of an action line
extern int TargetScanDelay; // RA2: a global no code sets, so the scan delay is 0
extern bool CrateWater[CRATE_COUNT]; // RA2: which crates may appear on water
extern int WolOptionsLocked; // RA2: 0x00B2AC40, the Internet game options are not accepted any more
extern bool LobbyDialogFlag; // RA2: 0x00A72D52, as netdlg2.cpp
extern void __fastcall Show_Player_Message(wchar_t const * text, int delay); // RA2: 0x006F58E0 (uicmnds); puts a text on the message list in the player's color, -1 for the default delay
extern int Cursor_Start_Frame_76(void); // RA2: 0x006F6760 (uicmnds)
extern int Cursor_Start_Frame_77(void); // RA2: 0x006F6750 (uicmnds)
extern bool AircraftRearmWhenEmpty; // RA2: a global at 0x83C4D4 that no code sets (false)
extern bool AircraftRearmWhenHalf; // RA2: a global at 0x83C4D5 that no code sets (false)
extern bool InputLockedBeforeHold; // RA2: 0x00AB6EF4
extern int SavedSpeechVolume; // RA2: 0x7F1DEC, set to full volume with EVA's level when a scenario is cleared
extern CCINIClass UIINI; // RA2: 0x00839860, UI.INI (init.cpp declares it the same way)
extern bool Net2JoinListEnabled; // RA2: 0x007E699C, the join dialog's game list accepts a selection, initially true
extern int SidebarWidth; // RA2: 0x007ADD80
extern void * BriefingButtonShape; // RA2: 0x00AB6350
extern void * OptionsButtonShape; // RA2: 0x00AB6318
extern bool IsBriefingButtonShapeLoaded; // RA2: 0x00AB6369
extern bool IsOptionsButtonShapeLoaded; // RA2: 0x00AB6368
extern char const * BriefingButtonShapeName; // RA2: 0x007EE064
extern char const * OptionsButtonShapeName; // RA2: 0x007EE068
extern ShapeButtonClass BriefingButton; // RA2: 0x00AB63E8
extern ShapeButtonClass OptionsButton; // RA2: 0x00AB6380
extern ShapeSet const * RadarFrameShape; // RA2: 0x00AC0FF0
extern ShapeSet const * RadarAnimShape; // RA2: 0x00AC1070
extern DynamicVectorClass<MixFileClass *> ScenarioMixFiles; // RA2: 0x837480, the .mmx archives opened for the multiplayer maps
extern bool PortsRead; // RA2: 0xAB9178, set once the port numbers have been read from RA2.INI
extern int SavedGameSpeed; // RA2: 0x7F3230, the game speed from before the game was left (-1 when none)
extern int Text_Color; // RA2: 0x00A733E4, the font color of the dialogs
extern MultiplayerTeam * __fastcall Create_Battle_Team(int number); // RA2: 0x005A2550, a team titled with its number (the MultiplayerTeam constructor at 0x005B9B20 with the number as the text and the index)
