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

#include "data.h"
#include "msanim.h"
#include "msengine.h"
#include "rcptr.h" // RA2: the tour keeps its territories and states as counted pointers
#include "rcptr.h"
#include "refcount.h"
#include "stringtable.h" // RA2: the option texts are loaded by label
#include "widewstring.h"
#include "wstring.h"

class MSFont;
class MSAnimEntry;
class MSSfxEntry;
class INIClass;
struct GameOptionsType;
template<class T> class DynamicVectorClass;

#define MAX_WDT_TERRITORIES 100

class Surface;
class ConvertClass;
class Wstring;
void __fastcall WDT_Write_Map_Ini(Wstring const & prefix, Wstring const & filename); // RA2: 0x0072A880; a development tool that finds the territory targets in a click map and writes them to WDTMap.ini
Wstring const & __fastcall Faction_Name(int faction); // RA2: 0x007685A0; the name a faction is called in the INI files
Wstring const & __fastcall Faction_Code(int faction); // RA2: 0x007686D0; the short code of a faction that the LastDay<code> keys of WDTHist.INI end in (YR: WDT_Country_Name)

class WDTTerritory : public ReferenceCounted { // RA2: ReferenceCounted
	public:
		WDTTerritory(void);
		virtual ~WDTTerritory(void);

		static rc_ptr<WDTTerritory> __fastcall Create(void); // RA2: 0x0072BDC0; a new territory, counted

		bool operator==(const WDTTerritory &that) const { return(Index == that.Index); }
		bool operator!=(const WDTTerritory &that) const { return(Index != that.Index); }

		GameOptionsType *Get_Game_Options(void);

		char *To_String(void);

	public:
		/*
		 * This is the territory's position in the tour's territory list, and the number
		 * the ladder server and the click map both know it by.
		 */
		int Index;

		/*
		 * This is the seed the battlefield is generated from, so that every player fighting
		 * over this territory is given the same map. If zero, a seed is picked at random.
		 */
		unsigned short Seed;

		/*
		 * This is the number of vehicles each player starts the battle with (1 - 10).
		 */
		unsigned char UnitCount;

		/*
		 * This is the highest build level the players may reach in the battle (1 - 10),
		 * which is what limits how advanced their armies are allowed to become.
		 */
		unsigned char TechLevel;

		/*
		 * This is the number of credits each player starts the battle with.
		 */
		unsigned short StartingCredits;

		/*
		 * These are the dimensions of the battlefield, expressed as a size step (0 - 3)
		 * rather than in cells. Together they name the shape of map the player is told to
		 * expect -- tiny, narrow, tall, huge and so on.
		 */
		unsigned char Width;
		unsigned char Height;

		/*
		 * This is the number of players the battle over this territory is fought between.
		 * Only two and four are used, which the front end offers as 1on1 and 2on2.
		 */
		unsigned char NumPlayers;

		/*
		 * This is the amount of ore and gems the battlefield is seeded with (0 - 3).
		 */
		int Resources; // RA2: +0x18, replaces Veinholes

		/*
		 * This specifies the kind of map the battlefield is generated as (0 - 4).
		 */
		unsigned char MapType; // RA2: +0x1C, replaces Biome

		/*
		 * This specifies the theater the battlefield is generated in.
		 */
		unsigned char Theater; // RA2: +0x1D, replaces Time

		/*
		 * This specifies the time of day the battle is lit by.
		 */
		unsigned char Time; // RA2: +0x1E

		/*
		 * These are the limits the tour places on how much of the battlefield is broken up
		 * by cliffs, along with the amount it starts out with (0 - 100).
		 */
		unsigned char CliffsMin;
		unsigned char CliffsMax;
		unsigned char CliffsDefault;

		/*
		 * These are the limits the tour places on how freely units may cross the
		 * battlefield, along with the openness the ground starts out with (0 - 100).
		 */
		unsigned char AccessibilityMin;
		unsigned char AccessibilityMax;
		unsigned char AccessibilityDefault;

		/*
		 * These are the limits the tour places on how hilly the battlefield is, along with
		 * the amount of high ground it starts out with (0 - 100).
		 */
		unsigned char HillsMin;
		unsigned char HillsMax;
		unsigned char HillsDefault;

		/*
		 * These are the limits the tour places on how much water the battlefield holds,
		 * along with the amount it starts out with (0 - 100).
		 */
		unsigned char WaterMin;
		unsigned char WaterMax;
		unsigned char WaterDefault;

		/*
		 * These are the limits the tour places on how rich the battlefield's tiberium is,
		 * along with the amount it starts out with (0 - 100).
		 */
		unsigned char TiberiumAmountMin;
		unsigned char TiberiumAmountMax;
		unsigned char TiberiumAmountDefault;

		/*
		 * These are the limits the tour places on how widely the battlefield's tiberium is
		 * spread about, along with the layout it starts out with (0 - 100).
		 */
		unsigned char TiberiumFieldsMin;
		unsigned char TiberiumFieldsMax;
		unsigned char TiberiumFieldsDefault;

		/*
		 * These are the limits the tour places on how much tree cover the battlefield has,
		 * along with the amount it starts out with (0 - 100).
		 */
		unsigned char VegetationMin;
		unsigned char VegetationMax;
		unsigned char VegetationDefault;

		/*
		 * These are the limits the tour places on how built up the battlefield is, along
		 * with the number of civilian settlements it starts out with (0 - 100).
		 */
		unsigned char CitiesMin;
		unsigned char CitiesMax;
		unsigned char CitiesDefault;
		union {
			struct {
				/*
				 * Does the battlefield's lighting shift with the time of day?
				 */
				unsigned int TimeTransitions : 1;

				/*
				 * Do tiberium lifeforms roam the battlefield?
				 */
				unsigned int TiberiumCreatures : 1;

				/*
				 * May the players form alliances during the battle?
				 */
				unsigned int AlliesAllowed : 1;

				/*
				 * Are harvesters protected from being fired upon?
				 */
				unsigned int HarvTruce : 1;

				/*
				 * May the players build bases, or fight the battle out with the units
				 * they start with?
				 */
				unsigned int Bases : 1;

				/*
				 * May a deployed construction yard pack itself back up?
				 */
				unsigned int MCVRedeploy : 1;

				/*
				 * Does explored ground fade back into fog once nothing of the player's
				 * can see it any longer?
				 */
				unsigned int FogOfWar : 1;

				/*
				 * May bridges be brought down by weapon fire?
				 */
				unsigned int BridgeDestruction : 1;

				/*
				 * Are crates scattered across the battlefield?
				 */
				unsigned int Goodies : 1;

				/*
				 * Does the battlefield's tiberium include the richer blue strain as
				 * well as the green?
				 */
				unsigned int BlueTiberium : 1;

				/*
				 * Is a player defeated the moment his last building falls?
				 */
				unsigned int ShortGame : 1;

				/*
				 * Are engineers reduced to damaging a healthy building rather than
				 * capturing it outright?
				 */
				unsigned int CrapEngineer : 1;
			};

			/*
			 * This is the whole set of game option flags as one word, which is how the ladder
			 * server sends them down and how each option's bitmask picks out its own.
			 */
			unsigned int Booleans;
		};
		union {
			struct {
				/*
				 * These flags say which of the territory's settings the player is
				 * left free to change before the battle begins. A setting whose flag
				 * is clear is fixed by the tour, and its control in the map generator
				 * and game option dialogs is greyed out.
				 */
				unsigned int UserModTimeTransitions : 1;
				unsigned int UserModTiberiumCreatures : 1;
				unsigned int UserModAlliances : 1;
				unsigned int UserModHarvesterTruce : 1;
				unsigned int UserModBases : 1;
				unsigned int UserModMCVRedeploy : 1;
				unsigned int UserModFogOfWar : 1;
				unsigned int UserModBridgeDestruction : 1;
				unsigned int UserModCrates : 1;
				unsigned int UserModBlueTiberium : 1;
				unsigned int UserModShortGame : 1;
				unsigned int UserModCrapEngineer : 1;
				unsigned int UserModBiome : 1;
				unsigned int UserModTime : 1;
				unsigned int UserModNumPlayers : 1;
				unsigned int UserModUnitCount : 1;
				unsigned int UserModTechLevel : 1;
				unsigned int UserModCredits : 1;
				unsigned int UserModCliffs : 1;
				unsigned int UserModAccessability : 1;
				unsigned int UserModHills : 1;
				unsigned int UserModTiberiumAmount : 1;
				unsigned int UserModTiberiumFields : 1;
				unsigned int UserModWater : 1;
				unsigned int UserModVegetation : 1;
				unsigned int UserModCities : 1;
				unsigned int UserModWidth : 1;
				unsigned int UserModHeight : 1;
				unsigned int UserModSeed : 1;
				unsigned int UserModVeinholeMonsters : 1;
			};

			/*
			 * This is the whole set of user modifiable flags as one word, which is how the
			 * ladder server sends them down and how each option's bitmask picks out its own.
			 */
			unsigned int UserModBooleans;
		};
};

enum {
	/// the minimum size the data WDTState::Create_Territories requires to be read
	MIN_TERRITORY_DATA_SIZE = size_of(WDTTerritory, Seed)
							+ size_of(WDTTerritory, UnitCount)
							+ size_of(WDTTerritory, TechLevel)
							+ size_of(WDTTerritory, StartingCredits)
							+ size_of(WDTTerritory, MapType) // RA2: was Biome
							+ size_of(WDTTerritory, Theater) // RA2: was Time
							+ size_of(WDTTerritory, Width)
							+ size_of(WDTTerritory, Height)
							+ size_of(WDTTerritory, NumPlayers)
							+ 1 // RA2: the resources byte of the packet (Resources is an int in RA2)
							+ size_of(WDTTerritory, CliffsDefault)
							+ size_of(WDTTerritory, AccessibilityDefault)
							+ size_of(WDTTerritory, HillsDefault)
							+ size_of(WDTTerritory, TiberiumAmountDefault)
							+ size_of(WDTTerritory, TiberiumFieldsDefault)
							+ size_of(WDTTerritory, WaterDefault)
							+ size_of(WDTTerritory, VegetationDefault)
							+ size_of(WDTTerritory, CitiesDefault)
							+ size_of(WDTTerritory, Booleans)
							+ size_of(WDTTerritory, UserModBooleans),
};

class WDTState : public ReferenceCounted { // RA2: ReferenceCounted
	public:
		WDTState(void);
		WDTState(void *data);
		virtual ~WDTState(void);

		static rc_ptr<WDTState> __fastcall Create(void *data); // RA2: 0x0072BEB0; a new state read from the packet, counted

		bool Has_Owner_History(void);
		bool Is_Previous_Cycle(void);

		bool Create_History(void *data, int size);
		bool Create_Territories(void *data, int size);

		char *To_String(void);

		enum State {
			UNASSIGNED = -1,
			CONTESTED,
			GDI,
			NOD,
		};

	public:
		/*
		 * This is the one line description of the tour cycle that the ladder server sent
		 * down, used wherever the campaign must be named in passing.
		 */
		char *ShortDesc;

		/*
		 * This is the full description of the tour cycle that the ladder server sent down.
		 * It is the caption the selection screen wears while no territory is picked out.
		 */
		char *LongDesc;

		/*
		 * This is the address of the web page the ladder server advertises for this cycle,
		 * where the standings are published.
		 */
		char *WebURL;

		/*
		 * This specifies which of the tour's world maps the cycle is fought over, and so
		 * which region artwork and territory list the selection screen loads.
		 */
		unsigned char MapID;

		/*
		 * This identifies the tour cycle. A campaign that remembers a different cycle has
		 * been left behind by the tour and is started over rather than resumed.
		 */
		unsigned int CycleID;

		/*
		 * This is the number of ticks a full tour cycle runs for.
		 */
		unsigned int CycleLength;

		/*
		 * This is the number of territories being fought over in this cycle.
		 */
		unsigned int NumTerritories;

		/*
		 * This is the number of ticks the cycle has reached, and so the number of rows the
		 * ownership history holds. The last of them is the state of the world right now.
		 */
		unsigned int NumTicks;

		/*
		 * This is the length of a tour tick, expressed in seconds. A report on a cycle that
		 * has already finished carries no timing of its own.
		 */
		unsigned int TickTime;

		/*
		 * This is the list of territory records for the cycle, each carrying the
		 * battlefield and the game options that the fight over that territory is
		 * settled by.
		 */
		DynamicVectorClass<rc_ptr<WDTTerritory> > Territories; // RA2: counted pointers

		/*
		 * This records who held each territory at each tick of the cycle -- a row per
		 * tick, an entry per territory. The selection screen replays it to show the player
		 * how the map changed hands while he was away.
		 */
		unsigned char **OwnerHistory;

		/*
		 * If this state is a report on the cycle that has already ended, then this flag
		 * will be true. Such a report brings the finished ownership history alone -- it has
		 * no territory records and no tick timing.
		 */
		bool IsPreviousCycle;
};


namespace WorldDominationTour
{
	enum Outcome {
		OUTCOME_WIN,
		OUTCOME_LOSE,
		OUTCOME_DRAW,
		OUTCOME_COUNT
	};

	class State;
	class History;
	class CampaignProperties;
	class Conflict;
	class Territory;
	class Campaign;

	/*
	 * RA2: the classes of the tour are counted objects (ReferenceCounted) kept in rc_ptr, made by a
	 * static Create. The screen of WDTSel.cpp only reads them, so what is declared is what it uses.
	 */

	/*
	 * RA2: the owner of every territory at one tick, read from the tour state (WDTState) of the
	 * campaign; Owners is NULL for the empty state. Territory owners are 1 (disputed), 2 and 3 (the
	 * two sides), 0 for none.
	 */
	class State : public ReferenceCounted {
		public:
			State(void); // RA2: 0x007307F0
			State(rc_ptr<WDTState> const & tour, unsigned int tick); // RA2: 0x00730830
			virtual ~State(void); // RA2: 0x007308E0
			static rc_ptr<State> __fastcall Create(rc_ptr<WDTState> const & tour, int tick); // RA2: 0x00730700
			static rc_ptr<State> __fastcall Create(void); // RA2: 0x00730790; the empty state
			int Owner_Of(unsigned int terr) const; // RA2: 0x00730890
			int Count_Owned(int owner) const; // RA2: 0x00730900; counts the territories of owner 2 and 3 only

		public:
			rc_ptr<WDTState> TourState; // RA2: +0x08
			unsigned char * Owners; // RA2: +0x10; one byte per territory, from the tour state's list for the tick
	};

	/*
	 * RA2: the tour state's ownership history, one State per tick.
	 */
	class History : public ReferenceCounted {
		public:
			History(rc_ptr<WDTState> const & tour); // RA2: 0x00728EC0
			virtual ~History(void); // RA2: 0x00728F40
			static rc_ptr<History> __fastcall Create(rc_ptr<WDTState> const & tour); // RA2: 0x00728E50
			rc_ptr<State> Get_State(int tick) const; // RA2: 0x00728F00
			int Is_Tick_Valid(unsigned int tick) const; // RA2: 0x00728F20

		public:
			rc_ptr<WDTState> TourState; // RA2: +0x08
	};

	/*
	 * RA2: one battle of the campaign, a territory of the tour state.
	 */
	class Conflict : public ReferenceCounted {
		public:
			Conflict(rc_ptr<WDTTerritory> const & terr); // RA2: 0x00727360
			virtual ~Conflict(void); // RA2: 0x007273C0
			static rc_ptr<Conflict> __fastcall Create(rc_ptr<WDTTerritory> const & terr); // RA2: 0x007272F0
			bool operator==(Conflict const & that) const; // RA2: 0x007273A0
			bool Is_Territory_Set(void) const; // RA2: 0x007273E0
			int Get_Territory_Index(void) const; // RA2: 0x007273F0; the territory's number (+0x08), 0 without one
			bool Is_Territory(int index) const { return(Get_Territory_Index() == index); } // RA2: inline in Selection::Select_Territory
			static void __fastcall Init_Game_Options(void); // RA2: 0x00727480
			static void Deinit_Game_Options(void); // RA2: 0x00727920
			WideWstring Describe_Options(void) const; // RA2: 0x00727960

		public:
			rc_ptr<WDTTerritory> Territory; // RA2: +0x08
	};

	typedef DynamicVectorClass<rc_ptr<Conflict> > CONFLICT_LIST; // RA2: counted pointers
	typedef VectorCursor<rc_ptr<Conflict>, CONFLICT_LIST> CONFLICT_ITER;

	/*
	 * RA2: the descriptions of the tour state the campaign belongs to.
	 */
	class CampaignProperties : public ReferenceCounted {
		public:
			CampaignProperties(rc_ptr<WDTState> const & tour); // RA2: 0x0072D1F0
			virtual ~CampaignProperties(void); // RA2: 0x0072D1C0
			static rc_ptr<CampaignProperties> __fastcall Create(rc_ptr<WDTState> const & tour); // RA2: 0x0072D180

			int Get_Cycle_ID(void) const; // RA2: 0x0072D290; 0 without a state
			int Get_Layout(void) const; // RA2: 0x0072D2A0; the key into [LAYOUTS]
			int Get_Map_ID(void) const; // RA2: 0x0072D2B0; 1 without a state
			int Get_Current_Tick(void) const; // RA2: 0x0072D2D0; the state's conflict count less one
			Wstring Get_Web_URL(void) const; // RA2: 0x0072D2E0; empty without a state

		public:
			rc_ptr<WDTState> TourState; // RA2: +0x08
			WideWstring ShortDescription; // RA2: +0x10
			WideWstring LongDescription; // RA2: +0x14
	};

	/*
	 * RA2: the progress of one campaign, kept in WDTHist.INI under its name.
	 */
	class Campaign : public ReferenceCounted {
		public:
			Campaign(Wstring const & name, rc_ptr<CampaignProperties> const & props); // RA2: 0x00725F40
			virtual ~Campaign(void); // RA2: 0x007262E0
			static rc_ptr<Campaign> __fastcall Create(Wstring const & name, rc_ptr<CampaignProperties> const & props); // RA2: 0x00725EE0
			bool Is_Different_Cycle(void) const; // RA2: 0x00726350
			bool Is_Different(int faction) const; // RA2: 0x00726370; the saved progress of the faction differs
			int Get_Last_Day(int faction) const; // RA2: 0x007263A0
			void Save_Progress(int faction); // RA2: 0x007263B0
			void Reset_Progress(void); // RA2: 0x00726580; zeroes the cycle and every faction's last day in WDTHist.INI
			CONFLICT_ITER Get_Conflict_Iterator(int index); // RA2: 0x00726730; the first conflict over that territory
			void Set_Faction(int faction); // RA2: 0x007267C0; keeps the side and writes PlayerFaction
			void Load_INI(INIClass & ini); // RA2: 0x00726930; loads WDTHist.INI, creating it when missing
			int Get_Player_Faction(void) const { return(PlayerFaction); }

		public:
			rc_ptr<CampaignProperties> Properties; // RA2: +0x08
			rc_ptr<History> CycleHistory; // RA2: +0x10
			CONFLICT_LIST Conflicts; // RA2: +0x18
			int CycleID; // RA2: +0x30
			int LastDay[4]; // RA2: +0x34
			int PlayerFaction; // RA2: +0x44
			Wstring Name; // RA2: +0x48
	};

	/*
	 * RA2: one territory of the map, made by Create from a section of WDTMap.ini.
	 */
	class Territory : public ReferenceCounted {
		public:
			Territory(int id, INIClass const & ini, Wstring const & section, Wstring const & shape_prefix, Wstring const & name_prefix); // RA2: 0x00730BE0
			virtual ~Territory(void); // RA2: 0x00730F80
			static rc_ptr<Territory> __fastcall Create(int index, INIClass const & ini, Wstring const & section, Wstring const & shape_prefix, Wstring const & name_prefix); // RA2: 0x00730B80 (WDTTerr.cpp)
			void Create_Owner_Anim(MSEngine * engine, ConvertClass * drawer, Point2D const & offset); // RA2: 0x00730E00
			void Create_Highlight_Anim(MSEngine * engine, ConvertClass * drawer, Point2D const & offset); // RA2: 0x00730EC0
			void Set_Owner_State(int state, MSEngine * engine, bool wait); // RA2: 0x00730FB0
			void Set_Highlight(bool active, MSEngine * engine); // RA2: 0x00731120
			void Set_Target_Anim(MSAnim * anim); // RA2: 0x00731190
			MSAnim * Remove_Target_Anim(void); // RA2: 0x007311A0; returns the animation and clears it
			Point2D const & Get_Target(void) const { return(Target); }
			int Get_ID(void) const { return(ID); }
			MSAnim * Get_Target_Anim(void) const { return(TargetAnim); }

		public:
			int ID; // RA2: +0x08
			WideWstring Name; // RA2: +0x0C
			Point2D Target; // RA2: +0x10; where the target marker is pinned, relative to the map origin
			Point2D Origin; // RA2: +0x18
			Wstring ShapeName; // RA2: +0x20
			MSOverlayAnim * OwnerAnim; // RA2: +0x24
			MSAnim * HighlightAnim; // RA2: +0x28
			MSAnim * TargetAnim; // RA2: +0x2C
	};

	typedef DynamicVectorClass<rc_ptr<Territory> > TERRITORY_LIST;
	typedef VectorCursor<rc_ptr<Territory>, TERRITORY_LIST> TERRITORY_ITER;
	rc_ptr<Territory> __fastcall Find_Territory(TERRITORY_LIST & territories, int index); // RA2: 0x007311B0 (WDTTerr.cpp)

	class Voices; // RA2: Map::Start_Intro takes the voice player

	/*
	 * RA2: the map is a counted object made by Create. Load reads the map's section of WDTMap.ini
	 * for the screen size in use: the background and the animations, the click map in which every
	 * pixel holds the number of the territory it belongs to, the rectangles the texts go in, the back
	 * button, and the territories with their overlays.
	 */
	class Map : public ReferenceCounted {
		public:
			/*
			 * RA2: a palette file of the map and the drawer made from it (Get_Drawer keeps them).
			 */
			class AnimationPalette {
				public:
					AnimationPalette(Wstring const & name, ConvertClass * drawer) : Name(name), Drawer(drawer) {}
					~AnimationPalette(void); // RA2: 0x0072A600

					Wstring Name; // RA2: +0x00
					ConvertClass * Drawer; // RA2: +0x04
			};

			/*
			 * RA2: the back button's overlay, whether it is down (+0x68), and the frames for its states.
			 */
			struct BackButtonOverlay {
				BackButtonOverlay(void) : Anim(0), IsDown(false) {}
				MSOverlayAnim * Anim; // RA2: +0x64
				bool IsDown; // RA2: +0x68
			};
			struct BackButtonFrames {
				BackButtonFrames(void) : Normal(0), Depressed(0), IsHot(false) {}
				int Normal; // RA2: +0x6C
				int Depressed; // RA2: +0x70
				bool IsHot; // RA2: +0x74; the mouse is over the button
			};

			Map(int map, int faction, int player_faction); // RA2: 0x00729330
			virtual ~Map(void); // RA2: 0x0072AF30 (scalar deleting), 0x007293F0
			static rc_ptr<Map> __fastcall Create(int map, int faction, int player_faction); // RA2: 0x007292E0
			void Load(INIClass & ini, MSEngine * engine, bool differs); // RA2: 0x00729510; bool: the selection screen pushes Campaign::Is_Different's result unmasked
			MSAnim * Load_Anim(INIClass & ini, Wstring const & section, Wstring const & entry, ConvertClass * drawer); // RA2: 0x0072A280; "file,x,y[,rate[,palette]]"
			ConvertClass * Get_Drawer(Wstring const & name); // RA2: 0x0072A510; the drawer of a palette file, loaded on first use
			void Start_Intro(MSEngine * engine, Voices * voices); // RA2: 0x0072A620
			int Territory_At(Point2D const & point) const; // RA2: 0x0072A660; the territory under a point, -1 for none
			void Set_Back_Hot(MSEngine * engine, bool hot); // RA2: 0x0072A6D0
			void Set_Back_Down(MSEngine * engine, bool down); // RA2: 0x0072A720
			int Is_Over_Back(Point2D const & point) const; // RA2: 0x0072A770; inside the back button's rectangle (+0x54)
			rc_ptr<Territory> Get_Territory(int index) { return(Find_Territory(Territories, index)); } // RA2: the territory with this number; inline (an out-of-line copy is 0x007302B0 in WDTSel.cpp)

		public:
			int MapNumber; // RA2: +0x08; the entry of [MAPS]
			int Faction; // RA2: +0x0C
			int PlayerFaction; // RA2: +0x10; the faction whose section is read
			ConvertClass * OverlayDrawer; // RA2: +0x14
			MSAnim * OpeningAnim; // RA2: +0x18; the "Opening" movie
			MSAnim * BackgroundAnim; // RA2: +0x1C
			MSAnim * HelpBarAnim; // RA2: +0x20
			Surface * ClickMap; // RA2: +0x24; each pixel holds a territory's number
			Wstring OverlayPrefix; // RA2: +0x28
			TERRITORY_LIST Territories; // RA2: +0x2C
			Point2D Origin; // RA2: +0x44; where the map begins on the screen
			Point2D HelpBarOrigin; // RA2: +0x4C
			Rect BackButtonRect; // RA2: +0x54
			BackButtonOverlay BackButton; // RA2: +0x64
			BackButtonFrames BackFrames; // RA2: +0x6C
			ConvertClass * BackButtonDrawer; // RA2: +0x78
			Rect DescriptionRect; // RA2: +0x7C
			Rect TooltipRect; // RA2: +0x8C
			Rect PlayRect; // RA2: +0x9C
			Rect BackRect; // RA2: +0xAC
			DynamicVectorClass<AnimationPalette *> Palettes; // RA2: +0xBC
	};

	class Centroid {
		public:
			Centroid(void); // RA2: 0x0072A7B0
			Centroid const & operator+=(Point2D const & pt); // RA2: 0x0072A7C0
			Point2D Center_Point(void) const; // RA2: 0x0072A7F0
			bool operator==(Centroid const & that) const; // RA2: 0x0072A840
			bool operator!=(Centroid const & that) const { return(!(*this == that)); }

		public:
			Point2D Position; // RA2: the sum of the points
			int Count; // RA2: how many were added
	};

	typedef VectorClass<Centroid> CENTROID_LIST;
	typedef VectorCursor<Centroid, CENTROID_LIST> CENTROID_ITER;

	/// Game Options

	enum GameOptionEnum {
		WDT_GAME_OPT_PRIORITY,		/// shown in the first summary pass
		WDT_GAME_OPT_NORMAL,		/// shown in the second summary pass
		WDT_GAME_OPT_USER_MOD,		/// user modifiable, excluded from the summary
	};

	enum GameOptionRangeEnum {
		WDT_GAME_OPT_RANGE_VERY_LOW,
		WDT_GAME_OPT_RANGE_LOW,
		WDT_GAME_OPT_RANGE_HIGH,
		WDT_GAME_OPT_RANGE_VERY_HIGH,
		WDT_GAME_OPT_RANGE_MIDDLE,
	};

	/*
	 * RA2: the battle options are counted objects. Get_Display_Priority says how an option is shown for a
	 * territory (WDT_GAME_OPT_PRIORITY first, WDT_GAME_OPT_USER_MOD never), Get_String appends its text to the
	 * text so far. Their classes are WDTGameOptions.cpp's (0x00728480 on); Conflict::Init_Game_Options makes
	 * the shared list of them.
	 */
	class GameOption : public ReferenceCounted {
		public:
			GameOption(unsigned int bitmask) : // RA2: inline
				Bitmask(bitmask)
			{
			}

			virtual ~GameOption(void); // RA2: 0x00728670
			virtual int Get_Display_Priority(rc_ptr<WDTTerritory> const & territory) const = 0; // RA2: vtable +0xC
			virtual WideWstring Get_String(rc_ptr<WDTTerritory> const & territory, WideWstring const & prefix) const = 0; // RA2: vtable +0x10
			bool Is_User_Modifiable(rc_ptr<WDTTerritory> const & territory) const; // RA2: 0x00728680; false

		public:
			unsigned int Bitmask; // RA2: +0x08; this option's bit in the used mask of a description
	};

	/*
	 * RA2: TibSun's option whose value falls into one of five bands. RA2 never makes one (no constructor, no
	 * table of virtual functions); its members are in the executable (0x00728690-0x007287C6), which gives the
	 * layout: a label whose text is completed by the band number, and the four limits of the bands.
	 */
	class RangedGameOption : public GameOption {
		public:
			virtual int Get_Display_Priority(rc_ptr<WDTTerritory> const & territory) const override; // RA2: 0x007287B0
			virtual WideWstring Get_String(rc_ptr<WDTTerritory> const & territory, WideWstring const & prefix) const override; // RA2: 0x00728690
			virtual int Get_Range(rc_ptr<WDTTerritory> const & territory) const = 0; // RA2: vtable +0x14, the band of the territory's value
			int In_Range_Of(unsigned int value) const; // RA2: 0x00728770; the band of a value

		public:
			int Reserved; // RA2: +0x0C, not read by this unit
			Wstring Label; // RA2: +0x10; the label of the text, completed by the band number
			unsigned int Range0; // RA2: +0x14
			unsigned int Range1; // RA2: +0x18
			unsigned int Range2; // RA2: +0x1C
			unsigned int Range3; // RA2: +0x20
	};

	/*
	 * RA2: an on/off option, from the bit of its Bitmask in the territory's flags.
	 */
	class FlagGameOption : public GameOption {
		public:
			FlagGameOption(unsigned int bitmask, Wstring off_string, int off_value, Wstring on_string, int on_value); // RA2: 0x00728B90
			static rc_ptr<FlagGameOption> __fastcall Create(unsigned int bitmask, Wstring off_string, int off_value, Wstring on_string, int on_value); // RA2: 0x007287D0
			virtual int Get_Display_Priority(rc_ptr<WDTTerritory> const & territory) const override; // RA2: 0x00728850
			virtual WideWstring Get_String(rc_ptr<WDTTerritory> const & territory, WideWstring const & prefix) const override; // RA2: 0x00728870

		public:
			int OffValue; // RA2: +0x0C
			int OnValue; // RA2: +0x10
			Wstring OnString; // RA2: +0x14
			Wstring OffString; // RA2: +0x18
	};

	/*
	 * RA2: TibSun's option that restricts the battle to two or four players (TibSun's name; the executable has
	 * no type information for it). RA2 never makes one either (0x00728970, 0x00728980 only).
	 */
	class NumberOfPlayersGameOption : public GameOption {
		public:
			virtual int Get_Display_Priority(rc_ptr<WDTTerritory> const & territory) const override; // RA2: 0x00728970
			virtual WideWstring Get_String(rc_ptr<WDTTerritory> const & territory, WideWstring const & prefix) const override; // RA2: 0x00728980
	};

	/*
	 * RA2: the option that tells the size of the map.
	 */
	class MapSizeGameOption : public GameOption {
		public:
			MapSizeGameOption(unsigned int bitmask) :
				GameOption(bitmask)
			{
			}
			static rc_ptr<MapSizeGameOption> __fastcall Create(unsigned int bitmask); // RA2: 0x00728A60
			virtual int Get_Display_Priority(rc_ptr<WDTTerritory> const & territory) const override; // RA2: 0x00728B70
			virtual WideWstring Get_String(rc_ptr<WDTTerritory> const & territory, WideWstring const & prefix) const override; // RA2: 0x00728AB0
	};

	/*
	 * RA2: an option whose value is a number read from the territory through Getter and printed through the
	 * format text of its label. The members are expanded where they are used (WDTConflict.cpp).
	 */
	template<typename T>
	class ValueGameOption : public GameOption {
		public:
			typedef int (__fastcall * GetterType)(WDTTerritory const * territory);

			ValueGameOption(unsigned int bitmask, GetterType getter, Wstring name, int fallback) :
				GameOption(bitmask),
				Fallback(fallback),
				Getter(getter),
				Name(name)
			{
			}

			static rc_ptr<ValueGameOption<T> > __fastcall Create(unsigned int bitmask, GetterType getter, Wstring name, int fallback)
			{
				return(rc_ptr<ValueGameOption<T> >(new ValueGameOption<T>(bitmask, getter, name, fallback)));
			}

			virtual int Get_Display_Priority(rc_ptr<WDTTerritory> const & territory) const override
			{
				if (Is_User_Modifiable(territory)) {
					return(WDT_GAME_OPT_USER_MOD);
				}
				return(Fallback);
			}

			virtual WideWstring Get_String(rc_ptr<WDTTerritory> const & territory, WideWstring const & prefix) const override
			{
				wchar_t buffer[64];
				Wstring number = Wstring_FromInt(Getter(territory.Get()));
				WideWstring format(StringTable::LoadString(Name.text(), NULL, "D:\\Ra2\\wdtgameoptions.h", 161));
				swprintf(buffer, format.get(), number.text());
				if (prefix.length() != 0) {
					WideWstring separator(L", ");
					prefix + separator + buffer;
				}
				return(WideWstring(buffer));
			}

		public:
			int Fallback; // RA2: +0x0C; the priority of a setting the player cannot change
			GetterType Getter; // RA2: +0x10
			Wstring Name; // RA2: +0x14; the label of the format text
	};

	typedef DynamicVectorClass<rc_ptr<GameOption> > WDT_GAME_OPTION_LIST;
	typedef VectorCursor<rc_ptr<GameOption>, WDT_GAME_OPTION_LIST> WDT_GAME_OPTION_ITER;
	extern WDT_GAME_OPTION_LIST WDTGameOptionList; // RA2: 0x00B256C0; the shared list of battle options, filled by Conflict::Init_Game_Options
	int __fastcall WDT_Get_Unit_Count(WDTTerritory const * territory); // RA2: 0x00727B70; the getter of the WDT:UnitCount option
	int __fastcall WDT_Get_Tech_Level(WDTTerritory const * territory); // RA2: 0x00727B80; the getter of the WDT:TechLevel option

	typedef DynamicVectorClass<unsigned int> VOICEINDEX_LIST;

	class Voices
	{
		public:
			class Anim : public MSAnim {
				public:
					Anim(Voices *voices); // RA2: 0x00732AA0
					virtual ~Anim(void); // RA2: 0x00732AE0

					virtual void Set_Active(bool active); // RA2: 0x00732AF0
					virtual void Pause(void); // RA2: 0x00732B50
					virtual void Resume(void); // RA2: 0x00732BA0
					virtual bool Advance(Surface * surface, Rect & rect); // RA2: 0x00732BC0
					virtual void Redraw(Surface * surface, Rect const * rect=NULL); // RA2: 0x00732BD0
					virtual Rect Get_Rect(void) const; // RA2: 0x00732BE0
					virtual bool Has_Finished(void) const; // RA2: 0x00732C10
					virtual void Restore(Rect const & rect); // RA2: 0x00732C20

				public:
					Voices * VoicePlayer; // RA2: +0x1C
			};

			/*
			 * RA2: one group of announcements: the key prefix it is read under and, for each of
			 * the three results, the plain and the emphatic line numbers.
			 */
			struct Group {
				Group(void); // RA2: 0x00732060
				void Load(INIClass & ini, Wstring const & section, int faction); // RA2: 0x007320C0
				void Load(INIClass & ini, Wstring const & section, int faction, int kind, Wstring const & key); // RA2: 0x00732300
				bool Pick(int faction, int kind, Wstring & name) const; // RA2: 0x00732160
				bool Pick_Emphasis(int faction, int kind, Wstring & name) const; // RA2: 0x00732230

				void Set_Name(Wstring const & name); // RA2: 0x007320B0; names the group (expanded in the Voices constructor too)

				Wstring Name; // RA2: +0x00
				DynamicVectorClass<unsigned int> Lines[6]; // RA2: +0x04; plain at [kind * 2], emphatic at [kind * 2 + 1]
			};

			enum VoiceCategoryType {
				VOICECAT_OLD_CYCLE,
				VOICECAT_HISTORY,
				VOICECAT_STATUS,
				VOICECAT_TERRITORY,
				VOICECAT_GAME,
				VOICECAT_EVACUATE,
				VOICECAT_COUNT
			};

			Voices(int faction); // RA2: 0x007314C0
			~Voices(void); // RA2: 0x00731680
			void Load(INIClass & ini, Wstring const & section); // RA2: 0x007317A0
			void Load_List(INIClass & ini, Wstring const & section, int index, Wstring const & key); // RA2: 0x00731AA0; one comma separated list, <faction><key>
			bool Pick(int group, int kind, Wstring & name) const; // RA2: 0x00731BC0; the speech name of an ordinary line
			bool Pick_Emphasis(int group, int kind, Wstring & name) const; // RA2: 0x00731CA0
			bool Pick_List(int index, Wstring & name) const; // RA2: 0x00731D80
			static bool __fastcall Pick(int faction, DynamicVectorClass<unsigned int> const & lines, Wstring & name); // RA2: 0x00731E60; a random line of a list, false for an empty one
			static bool __fastcall Make_Name(int faction, int line, Wstring & name); // RA2: 0x00731F30; "WWD-Soviet<NN>" for faction 3, else "WWD_Allied<NN>"
			static void __fastcall Parse_List(Wstring const & text, DynamicVectorClass<unsigned int> & lines); // RA2: 0x00731FC0
			void Update(void); // RA2: 0x00732870; once the delay is over and nothing plays, drops the line said and plays the next
			void Trim(void); // RA2: 0x00732960; drops the queued lines past the counted ones
			void Stop(void); // RA2: 0x007329F0
			void Say(int group, int kind, bool emphasis, bool counted, int delay); // RA2: 0x00732510
			void Say_List(int index, bool counted, int delay); // RA2: 0x00732700; says one of the voice lists read from the INI (the startup list is 0)
			bool Is_Speaking(void); // RA2: 0x00732A40
			Anim * Create_Anim(void); // RA2: 0x00732A50

		public:
			int Faction; // RA2: +0x000
			Group Groups[6]; // RA2: +0x004
			DynamicVectorClass<unsigned int> Lists[2]; // RA2: +0x37C; startup and territory select
			CDTimerClass<SystemTimerClass> Timer; // RA2: +0x3AC
			bool Idle; // RA2: +0x3B8; nothing is being said
			int Pending; // RA2: +0x3BC
			DynamicVectorClass<Wstring> Queue; // RA2: +0x3C0; the speech names
			void * Stream; // RA2: +0x3D8; the audio stream
	};

	/*
	 * RA2: the screen where the player picks the next territory to attack (WDTSel.cpp). It is a
	 * stack object (0x4B0 bytes) made by Show and Choose_Conflict.
	 */
	class Selection : public MSEngine {
		typedef DynamicVectorClass<MSSfxEntry *> SFX_LIST;
		typedef VectorCursor<MSSfxEntry *, SFX_LIST> SFX_ITER;

		public:
			Selection(rc_ptr<Campaign> const & campaign, int faction); // RA2: 0x0072D560
			virtual ~Selection(void); // RA2: 0x0072D990 (scalar deleting 0x00730290)

			static void __fastcall Show(rc_ptr<Campaign> const & campaign, int faction); // RA2: 0x0072FF80
			static bool __fastcall Choose_Conflict(rc_ptr<Campaign> const & campaign, int faction); // RA2: 0x0072FFC0

			void Init_Voices(INIClass & ini, Wstring const & section); // RA2: 0x0072DC20
			void Init_Sounds(INIClass & ini, Wstring const & section); // RA2: 0x0072DD10
			void Init_Target(INIClass & ini, Wstring const & section); // RA2: 0x0072DEA0
			rc_ptr<Territory> Pick_Territory(void); // RA2: 0x0072E050
			void Handle_Click(Point2D const & point, bool & selected); // RA2: 0x0072E430
			bool Select_Territory_At(Point2D const & point); // RA2: 0x0072E650
			void Update_Hovered_Territory(Point2D const & point); // RA2: 0x0072E720
			void Do_Target_Anim(rc_ptr<State> const & state, bool show_targets, bool silent); // RA2: 0x0072E780
			bool Select_Territory(rc_ptr<Territory> const & territory); // RA2: 0x0072ED80
			void Remove_Target_Anims(void); // RA2: 0x0072F180
			void Remove_Target_Anims2(void); // RA2: 0x0072F200; an exact duplicate
			void On_Map_Enter(void); // RA2: 0x0072F280
			void On_Map_Leave(void); // RA2: 0x0072F340
			void On_Cancel_Enter(void); // RA2: 0x0072F430
			void On_Cancel_Leave(void); // RA2: 0x0072F540
			void Do_Target_Selection(int faction); // RA2: 0x0072F5A0
			int Get_Outcome(int tick, bool & mega); // RA2: 0x0072F6E0; 0 won, 1 lost, 2 neither
			bool Present_Ticks(int tick_from, int tick_to); // RA2: 0x0072F7A0
			void Presentation(int faction); // RA2: 0x0072FA00
			bool Present_History(int tick_from, int tick_to); // RA2: 0x0072FB60
			void Check_For_Breakout(void); // RA2: 0x0072FC80
			void Set_Text(WideWstring const & text, int start_delay); // RA2: 0x0072FCD0
			void Set_Tooltip(WideWstring const & text); // RA2: 0x0072FDA0
			void Play_SFX(Wstring const & name); // RA2: 0x0072FE90
			void Set_Long_Description(void); // RA2: 0x0072FF30
			void Start(void); // RA2: 0x00730160
			void End(void); // RA2: 0x00730230

			void Add_Sfx_Entry(MSSfxEntry * entry) { if (entry != NULL) SfxEntries.Add(entry); }

		public:
			rc_ptr<Campaign> TourCampaign; // RA2: +0x48; the campaign the screen presents
			rc_ptr<Conflict> SelectedConflict; // RA2: +0x50; the conflict waiting on the selected territory
			Voices VoicePlayer; // RA2: +0x58
			int WinningThreshold; // RA2: +0x434
			int LosingThreshold; // RA2: +0x438
			int MegaWinningThreshold; // RA2: +0x43C
			int MegaLosingThreshold; // RA2: +0x440
			bool Brokeout; // RA2: +0x444; Escape was pressed
			Point2D LastPoint; // RA2: +0x448
			int StartTick; // RA2: +0x450; the last conflict of the campaign
			int LastHoverZone; // RA2: +0x454
			rc_ptr<Map> WorldMap; // RA2: +0x458
			unsigned int HoveredTerritoryID; // RA2: +0x460
			rc_ptr<Territory> MousedTerritory; // RA2: +0x464
			ConvertClass * TargetDrawer; // RA2: +0x46C
			MSAnimEntry * ZoomingTarget; // RA2: +0x470
			MSAnimEntry * ThrobbingTarget; // RA2: +0x474
			int ThrobbingTargetDividingFrame; // RA2: +0x478
			MSBitPrintAnim * TextAnim; // RA2: +0x47C; the description
			MSBitPrintAnim * TooltipAnim; // RA2: +0x480
			MSFont * Font; // RA2: +0x484
			rc_ptr<State> CurrentState; // RA2: +0x488
			Wstring ThemeName; // RA2: +0x490; the "Theme" entry of [SOUNDS]
			SFX_LIST SfxEntries; // RA2: +0x494
			int TextColor; // RA2: +0x4AC; the font color to put back
	};

};

enum WDTStateRequestEnum {
	WDT_REQ_STATE_EVERYTHING = 3,
	WDT_REQ_STATE_LAST_CYCLE = 5,
};
