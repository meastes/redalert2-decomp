// SPDX-License-Identifier: GPL-3.0-or-later
// Copyright 2026 ra2-decomp contributors
// Copyright 2025 Electronic Arts Inc.; 2026 OpenTS contributors
// Derived from Electronic Arts source code; EA's GPL Section 7 terms apply (COPYING.md).
// Changed to RA2 1.006's layout by RedAlert2 contributors, 2026.
#pragma once

#include "ccfile.h"
#include "connect.h"
#include "event.h"
#include "house.h"
#include "ipxaddr.h"
#include "msglist.h"
#include "special.h"
#include "sun.h"
#include "typelist.h"
#include "version.h"
#include "wstring.h"
#include "win.h"

#include "dialog.hh"
#include "diff.hh"
#include "diskid.hh"

class AircraftClass;
class AnimClass;
class BuildingClass;
class BulletClass;
class InfantryClass;
class UnitClass;
class PhoneEntryClass;
class CellClass;
class INIClass;
class MultiplayerGameMode;
class CCINIClass;

#define	MPLAYER_BUILD_LEVEL_MAX		10
#define	MAX_MPLAYER_COLORS			8

#define	MAX_IPX_PACKET_SIZE			558
#define	MAX_SERIAL_PACKET_SIZE		256

#include "mplayerscore.h"

#define	MAX_MULTI_NAMES	8

#define MODEM_MIN_MAX_AHEAD			5
#define NETWORK_MIN_MAX_AHEAD		2

#define DEFAULT_FRAME_SEND_RATE		5

#define	PORTBUF_MAX					64
#define	IRQBUF_MAX					3
#define	BAUDBUF_MAX					7
#define	INITSTRBUF_MAX				41
#define	CWAITSTRBUF_MAX				16
#define	CREDITSBUF_MAX				5
#define	PACKET_TIMING_TIMEOUT		40
#define	MODEM_NAME_MAX				PORTBUF_MAX - 1

#define SERIAL_MAX					23
#define ENCRYPTION_STRING_LENGTH	128

enum GameType {
	GAME_NORMAL,
	GAME_MODEM,
	GAME_NULL_MODEM,
	GAME_IPX,
	GAME_INTERNET,
	GAME_SKIRMISH,
	GAME_WDT,
	GAME_SERIAL_MODEM,
};

enum DetectPortType {
	PORT_VALID = 0,
	PORT_INVALID,
	PORT_IRQ_INUSE
};

enum DialStatusType {
	DIAL_CONNECTED			= 0,
	DIAL_NO_CARRIER,
	DIAL_BUSY,
	DIAL_ERROR,
	DIAL_NO_DIAL_TONE,
	DIAL_TIMEOUT,
	DIAL_CANCELED
};

enum DialMethodType {
	DIAL_TOUCH_TONE = 0,
	DIAL_PULSE,
	DIAL_METHODS
};

enum CallWaitStringType {
	CALL_WAIT_TONE_1 = 0,
	CALL_WAIT_TONE_2,
	CALL_WAIT_PULSE,
	CALL_WAIT_CUSTOM,
	CALL_WAIT_STRINGS_NUM
};

enum ModemGameType {
	MODEM_NULL_HOST = 0,
	MODEM_NULL_JOIN,
	MODEM_DIALER,
	MODEM_ANSWERER
};

enum SerialCommandType {
	SERIAL_CONNECT			= 100,
	SERIAL_GAME_OPTIONS,
	SERIAL_SIGN_OFF,
	SERIAL_GO,
	SERIAL_MESSAGE,
	SERIAL_TIMING,
	SERIAL_SCORE_SCREEN,
	SERIAL_LOADGAME,
	SERIAL_PROGRESS_REPORT,
	SERIAL_LAST_COMMAND,
	SERIAL_REQ_SCENARIO,
	SERIAL_FILE_INFO,
	SERIAL_FILE_CHUNK,
	SERIAL_FILE_INFO_ACK,
	SERIAL_READY_TO_GO,
	SERIAL_NO_SCENARIO,
	SERIAL_ACCEPT_OPTIONS,
	SERIAL_PREVIEW_MODE,
	SERIAL_PREVIEW_ACK,
	SERIAL_REQ_PREVIEW,
	SERIAL_TAUNT, // RA2: 0x78, a taunt (the packed byte follows the sender's ID)
};

enum NetCommandType {
	NET_QUERY_GAME,
	NET_ANSWER_GAME,
	NET_QUERY_PLAYER,
	NET_ANSWER_PLAYER,
	NET_CHAT_ANNOUNCE,
	NET_CHAT_REQUEST,
	NET_QUERY_JOIN,
	NET_CONFIRM_JOIN,
	NET_REJECT_JOIN,
	NET_GAME_OPTIONS,
	NET_SIGN_OFF,
	NET_GO,
	NET_MESSAGE,
	NET_PING,
	NET_LOADGAME,
	NET_PROGRESS_REPORT,
	NET_REQ_SCENARIO,
	NET_FILE_INFO,
	NET_FILE_CHUNK,
	NET_READY_TO_GO,
	NET_NO_SCENARIO,
	NET_FILE_INFO_ACK,
	NET_COMMAND_16,
	NET_PUB_GAMEOPT,
	NET_PRIV_GAMEOPT,
	NET_PREVIEW_MODE,
	NET_PREVIEW_ACK,
	NET_REQ_PREVIEW,
	NET_PROPOSE_KICK,

	NET_IM_STILL_HERE = 39,
	NET_COMMAND_28 = 40, // RA2: 0x28, the host reminds the joiner to accept (Handle_Start_Reminder, 0x005C3C20)
	NET_PORT_PROBE = 41,
	NET_MANGLER_REQUEST = 43,
	NET_MANGLER_RESPONSE = 44,
	NET_PORT_UNREACHABLE = 47,
};

struct SerialSettingsType {
	int Port;
	int IRQ;
	int Baud;
	DialMethodType DialMethod;
	int InitStringIndex;
	int CallWaitStringIndex;
	int Compression;
	int ErrorCorrection;
	char CallWaitString[ CWAITSTRBUF_MAX ];
	char ModemName [ MODEM_NAME_MAX ];
};

#pragma pack(1)

struct NodeNameType {
	wchar_t Name[20];
		IPXAddressClass Address;
	union {
		struct {
			unsigned char IsOpen;
			unsigned int LastTime;
		} Game;
		struct {
			char Serial[SERIAL_MAX];
			int House;
			int : 32;
			int Color;
			int : 32;
			int : 32; // RA2: three ints more than TS (YR has seven); ID is at +0x5F, the rest is not verified
			int ID;
			int ProcessTime;
			int Status;
			int SquadID;
		} Player;
		struct {
			unsigned int LastTime;
			unsigned char LastChance;
			int Color;
		} Chat;
		struct {
			char SerialPad[SERIAL_MAX];
			int Country; // RA2: +0x4B; Get_Country/Set_Country (0x0066E290, 0x0066E2D0) read and write it with InitialCountry
			int InitialCountry; // RA2: +0x4F, -2 while the country is still "random"
			int ColorIdx; // RA2: +0x53 (0x0066E2A0)
			int InitialColor; // RA2: +0x57, -2 while the color is still "random"
			int SpectatorFlag; // RA2: +0x5B
			int HouseIndex;
			int Time;
			int Unknown77;
			int Clan;
			int Unknown7F;
			unsigned short Port;
		} Slot;
	};

	int Get_Country(void);
	int Get_Color(void);
	int Get_Color_Or_Random(void);
	void Set_Country(int value);
	void Set_Color(int value);
};

struct SerialPacketType {
	SerialCommandType Command;
	char Name[MPLAYER_NAME_MAX];
	unsigned char ID;
	union {
		struct {
			HousesType House;
			char Color;
			unsigned int MinVersion;
			unsigned int MaxVersion;
			wchar_t Scenario[DESCRIP_MAX];
			unsigned int Credits;
			unsigned int IsBases		: 1;
			unsigned int IsBridgeDestruction : 1;
			unsigned int IsGoodies	: 1;
			unsigned int IsGhosties	: 1;
			unsigned int OfficialScenario : 1;
			int CheatCheck;
			unsigned char BuildLevel;
			unsigned char UnitCount;
			unsigned char AIPlayers;
			unsigned char AIDifficulty;
			unsigned int IsUnknownFlag1 : 1;
			unsigned int IsHarvTruce : 1;
			unsigned int IsMCVRedeploy : 1;
			unsigned int IsFogOfWar : 1;
			unsigned int IsShortGame : 1;
			unsigned int IsSWAllowed : 1;
			unsigned int IsBuildOffAlly : 1;
			unsigned int IsCrapEngineers : 1;
			int Seed;
			SpecialClass Special;
			unsigned int ResponseTime;
			unsigned int FileLength;
			unsigned char GameSpeed;
			char ShortFileName[13];
			unsigned char FileDigest[32];

			int AICheatCheck;
			int ArtCheatCheck;
			int BuildNumber;
		} ScenarioInfo;

		struct {
			wchar_t Message[MAX_MESSAGE_LENGTH];
			char Color;
		} Message;
		struct {
			int Color;
		} Chat;

		struct {
			int Percent;
		} Progress;

		char Taunt; // RA2: +0x19, the packed taunt byte of a SERIAL_TAUNT packet (Play_Taunt)
	};
};

#define MAX_SEND_FILE_PACKET_SIZE (MAX_SERIAL_PACKET_SIZE - 64) // RA2: parenthesised, it is a divisor in Receive_Remote_File
#pragma pack(push, 1)
struct RemoteFileTransferType {
	SerialCommandType	Command;
	char				Name[MPLAYER_NAME_MAX];
	unsigned short 	BlockNumber;
	unsigned short		BlockLength;

	unsigned char 		RawData	[MAX_SEND_FILE_PACKET_SIZE]; // RA2: 192 data bytes, the packet is 220 bytes (the stack slot of Send_Remote_File shows it)
};
#pragma pack(pop)

struct GlobalPacketType {
	NetCommandType Command;
	char Name[MPLAYER_NAME_MAX];
	char Serial[SERIAL_MAX];
	union {
		struct {
			unsigned int IsOpen		: 1;
			unsigned int IsFirestorm : 1;
		} GameInfo;
		struct {
			int House;
			int Color;
			unsigned int NameCRC;
			unsigned int MinVersion;
			unsigned int MaxVersion;
			int CheatCheck;
			int AICheatCheck;
			int ArtCheatCheck;
			unsigned int BuildNumber;
			int Slot;
			unsigned int Flag;
		} PlayerInfo;
		struct {
			#if 0
			char Scenario[DESCRIP_MAX];
			unsigned int Credits;
			unsigned int IsBases		: 1;
			unsigned int IsTiberium	: 1;
			unsigned int IsGoodies	: 1;
			unsigned int IsGhosties	: 1;
			unsigned int OfficialScenario :1;
			unsigned char BuildLevel;
			unsigned char UnitCount;
			unsigned char AIPlayers;
			int Seed;
			SpecialClass Special;
			unsigned int GameSpeed;
			unsigned int Version;
			#endif
			char pad[0xAF - 0x2F];
			unsigned int FileLength;
			char ShortFileName[13];
			unsigned char FileDigest[32];

		} ScenarioInfo;
		struct {
			unsigned char Recipients;
			char Buf[(MAX_MESSAGE_LENGTH) * sizeof(wchar_t)];
			int Color;
			unsigned int NameCRC;
		} Message;
		struct {
			wchar_t Names[(0x1C7 - 0x2F) / 2]; // RA2: the slot names, written by Get_Slot_Names (0x005C8CF0)
		} Slots; // RA2: command NET_COMMAND_16
		struct {
			int OneWay;
		} ResponseTime;
		struct {
			int Why;
		} Reject;
		struct {
			unsigned int ID;
			int Color;
		} Chat;

		struct {
			int Percent;
		} Progress;

		struct {
			unsigned int KickeeID;
			unsigned int KickerID;
		} Kick;

		struct {
			unsigned char Recipients;
			unsigned char ID;
			int Color;
			unsigned int NameCRC;
		} Taunt;

		struct {
			int X;
			int Y;
			int Z;
			char ID;
			char House;
		} Beacon;

		struct {
			char ID;
			char House;
			wchar_t Text[128];
		} BeaconText;

		struct {
			char Buf[400];
			int Color;
			unsigned int NameCRC;
		} Options;

		struct {
			int ID; // RA2: +0x2F, the sender's player ID
			char Address[sizeof(IPXAddressClass)]; // RA2: +0x33, its address with the packet router's port (command NET_IM_STILL_HERE)
		} StillHere;

		struct {
			unsigned short MangledPortNumber;
			unsigned char MangledAddress[4];
			unsigned short OriginalPortNumber;
			unsigned char BlitzMe;
		} ManglerData;
	};
};
#pragma pack()

struct TrapObjectType {
	union {
		AircraftClass *Aircraft;
		AnimClass *Anim;
		BuildingClass *Building;
		BulletClass *Bullet;
		InfantryClass *Infantry;
		UnitClass *Unit;
		void *All;
	} Ptr;
};

class MultiMission
{
	public:
		MultiMission(INIClass const & ini, char const * filename);
		MultiMission(char const * filename, wchar_t const * description, char const * digest, bool official, char const * game_modes, int min_players, int max_players);

		void Set_Description(wchar_t const * description);
		void Set_Filename(char const * filename);
		void Set_Digest(char const * digest);
		void Set_Official(bool official);
		wchar_t const * Description(void) const {return(ScenarioDescription);}
		char const * Get_Filename(void) const {return(Filename);}
		char const * Get_Digest(void) const {return(Digest);}
		bool Get_Official(void) const { return(IsOfficial); }

		bool Is_Random_Map(void) const;
		bool Supports_Game_Mode(Wstring & mode) const;
		bool Is_On_CD(DiskID cd) const;
		DiskID On_Which_CD(void) const;

	friend class SessionClass;
	friend class MPCooperative; // RA2: vt_entry_94 reads MaxPlayers of the next map (0x005A5720)

	private:
		wchar_t ScenarioDescription[DESCRIP_MAX];
		char Filename[_MAX_PATH];
		char Digest[32];
		bool IsOfficial;

		int MinPlayers;
		int MaxPlayers;

		TypeList<DiskID> Disks;
		DynamicVectorClass<Wstring> GameModes;
};

// One computer player's setup in a skirmish or multiplayer game, by player slot.
struct AISlotsType {
	int Difficulties[8];
	int Countries[8];
	int Colors[8];
	// RA2: no Starts or Allies; the game options string sends difficulty, country and color per AI
};

struct GameOptionsType {
	int			MPModeIndex;
	int 		ScenarioIndex;
	bool 		Bases;
	int 		Credits;
	bool		BridgeDestruction;
	bool 		Goodies;
	bool		ShortGame;
	bool		SWAllowed;
	bool		BuildOffAlly;
	int			GameSpeed;
	bool		CrapEngineers;
	bool 		Ghosts;
	int 		UnitCount;
	int 		AIPlayers;
	DiffType	AIDifficulty;
	AISlotsType	AISlots;
	bool		AlliesAllowed;
	bool		HarvTruce;
	bool		CTF;
	bool		FogOfWar;
	bool		MCVRedeploy;
	wchar_t		ScenarioDescription [45];

	void Reset_AI(void); // RA2: 0x00672810, no computer players and their country and color undecided (-1)
	bool Save(IStream * stream) const;
	bool Load(IStream * stream);

};

struct SessionOptionsClass {
	SessionOptionsClass(void);
	void Read(CCINIClass & ini, char const * section, int default_slot, int default_first_slot);
	void Write(CCINIClass & ini, char const * section);

	int MPGameMode;
	int ScenIndex;
	int GameSpeed;
	int Credits;
	int UnitCount;
	bool ShortGame;
	bool SuperWeaponsAllowed;
	bool BuildOffAlly;
	bool MCVRepacks;
	bool CratesAppear;
	int SlotData[8][3];
};

struct MPStatsType {
	char Name[64];
	int MaxRoundTrip;
	int Resends;
	int Lost;
	int PercentLost;
	int MaxAvgRoundTrip;
	int FrameSyncStalls;
	int CommandCountStalls;
	IPXAddressClass Address;
};

class SessionClass
{

	public:
		unsigned short Player_Port(int connection_id); // RA2: the port of the player on the connection

		SessionClass(void);
		~SessionClass(void);

		void One_Time(void);
		void Init(void);

		void Read_MultiPlayer_Settings (void);
		void Write_MultiPlayer_Settings (void);
		void Write_Chat_Settings(void);
		void Read_Scenario_Descriptions (void);
		void Free_Scenario_Descriptions(void);
		void Load_Call_Wait_Strings(void);

		int Create_Connections(void);
		bool Am_I_Master(HouseClass * who = NULL);
		unsigned int Compute_Unique_ID(void);
		void Update_Progress(int percent);
		void Init_Fixed_Alliances(void);
		void Trap_Object(void);
		int Color_Index_To_Scheme(int id);
		bool Is_Random_Map(void) const; // RA2: 0x00672140
		bool Is_Color_Used(int color); // RA2: 0x006728D0, a player or computer player already has the color
		int Random_Side(void); // RA2: 0x00672940
		int Random_Color(void); // RA2: 0x00672980
		int Chosen_House(void) const; // RA2: 0x006729F0
		int Chosen_Color(void) const; // RA2: 0x00672A10
		void Set_Side(int side, bool random); // RA2: 0x00672A30
		void Set_Color(int color, bool random); // RA2: 0x00672AE0
		void Assign_Random_Choices(void); // RA2: 0x00672BB0
		void Resume(void); // RA2: 0x672DE0; back into the game (the counterpart of Leave_Game)
		void Leave_Game(void); // RA2: 0x672E70
		void Remove_Player_Port(int index); // RA2: 0x00672F20, drops the port of the player at index
		int Player_To_Slot(int index); // RA2: 0x00672F90
		int Name_To_Slot(char const * name); // RA2: 0x00672FF0
		int Slot_To_Player(int slot); // RA2: 0x00673030
		bool Is_In_Game(void) const; // RA2: 0x672F10

		bool Log_To_File(FILE *out);

		GameType Type;
		MultiplayerGameMode * MPGameMode;
		int TournamentTerritory; // RA2: +0x08, index of the tournament territory being fought over
		int TournamentState; // RA2: +0x0C, 2 while a tournament game is played
		int unknown_10;

		CommProtocolType CommProtocol;

		GameOptionsType Options;

		unsigned int UniqueID;

		char Handle[MPLAYER_NAME_MAX];
		int PrefColor;
		int ColorIdx;
		int House;
		int idxSide2;
		int Color;
		int Color2;
		int Side;
		int Side2;

		SessionOptionsClass Skirmish;
		SessionOptionsClass LAN;
		SessionOptionsClass WOL;

		BOOL MultiplayerObserver;
		int Unknown_304;
		bool WOLLimitResolution;

		int LastNicknameSlot;

		int MaxPlayers;
		int NumPlayers;

		unsigned int MaxAhead;
		unsigned int FrameSendRate;

		int			DesiredFrameRate;
		int			ProcessTimer;
		int			ProcessTicks;
		int			ProcessFrames;
		int			MaxMaxAhead;
		int			PrecalcMaxAhead;
		int			PrecalcDesiredFrameRate;

		MPStatsType ConnectionStats[MAX_PLAYERS];

		bool EnableMultiplayerDebug;
		bool DrawMPDebugStats;
		char field_67E;
		char field_67F;

		int LoadGame;
		int EmergencySave;

		bool SquadAlliances;
		bool SawGameCompletion;
		bool OutOfSync;
		char field_68B;

		unsigned int PlayingAgainstVersion;

		DynamicVectorClass<MultiMission *> Scenarios;

		char ScenarioFileName[_MAX_FNAME+_MAX_EXT+2];
		char ScenarioDigest [32+2];
		unsigned int ScenarioFileLength;
		bool ScenarioIsOfficial;
		int ScenarioMaxPlayers; // RA2: +0x884, how many players the scenario takes (the debug text of Is_Channel_Full names it Session.ScenarioMaxPlayers)
		char ScenarioRequests[MAX_PLAYERS];
		int  RequestCount;

		IPXAddressClass	HostAddress;

		DynamicVectorClass<GlobalPacketType *> KickProposals;
		int KickVoteCount[MAX_PLAYERS];
		int KickVoteWho[MAX_PLAYERS][MAX_PLAYERS];

		MessageListClass Messages;
		IPXAddressClass MessageAddress;

		bool PlayerChatEnabled[8];
		bool LANTaunts;
		bool WOLTaunts;

		wchar_t LastMessage[MAX_MESSAGE_LENGTH];
		unsigned WWChat		: 1;
		bool LANScrollText;
		bool WOLScrollText;

		MPlayerScoreType Score[MAX_MULTI_NAMES];
		int GamesPlayed;
		int NumScores;
		int Winner;
		int CurGame;

		static char Descriptions[100][40];
		static int CountMin[2];
		static int CountMax[2];

		static char const * GlobalPacketNames[];
		static char const * SerialPacketNames[];

		CCFileClass RecordFile;
		unsigned Record				: 1;
		unsigned Play				 	: 1;
		unsigned Attract			 	: 1;

		int IsBridge;
		IPXAddressClass BridgeNet;
		bool NetStealth;
		bool NetProtect;
		bool NetOpen;
		wchar_t GameName[MPLAYER_NAME_MAX];
		GlobalPacketType GPacket;
		int GPacketlen;
		IPXAddressClass GAddress;
		unsigned short GProductID;
		char MetaPacket[MAX_IPX_PACKET_SIZE];
		int MetaSize;
		DynamicVectorClass <NodeNameType *> Chat;
		DynamicVectorClass <NodeNameType *> Games;
		DynamicVectorClass <NodeNameType *> Players;

		int LocalSlot; // RA2: +0x27FC, the slot of the local player: Seat_First_Player (0x005C8CD0) sets it, Reset_Slots (0x005C8A90) clears it to -1
		NodeNameType * Slots[8];
		bool IsStartReminded;	// RA2: +0x2878, set while the host's start reminder stands (0x005C3C20 sets, 0x005C3C40 clears)

		int Suspended;

		unsigned ModemService		: 1;
		int CurPhoneIdx;
		SerialSettingsType SerialDefaults;
		ModemGameType ModemType;
		DynamicVectorClass<PhoneEntryClass *> PhoneBook;
		DynamicVectorClass <char *> InitStrings;
		DynamicVectorClass <wchar_t const *> CallWaitStrings;
		static char const * DialMethodCheck[ DIAL_METHODS ];
		static char const * const CallWaitStringIDs[ CALL_WAIT_STRINGS_NUM ];

		int PlayerLatency[MAX_PLAYERS];
		int LatencyFudge;

		bool ViaPacketRouter;
		unsigned char unknown_2969[3];
		IPXAddressClass PacketRouterAddress;
		IPXAddressClass PacketRouterAddressTranslate;
		unsigned short PacketRouterPort;
		unsigned short : 16;
		IPXAddressClass LocalAddress;
		IPXAddressClass SlotAddresses[MAX_PLAYERS];
		int WolPlayerState[MAX_PLAYERS]; // RA2: +0x299C, cleared by Select_Game before the Internet game dialogs; name from that use only
		int PeerProbeData[64][2]; // RA2: +0x29BC, cleared by Start_Network_Session
		int ConnectionResults[64][2]; // RA2: +0x2BBC, set to -1 by Start_Network_Session (YR: ConnectionResults)
		int ConnectionResultCount; // RA2: +0x2DBC, cleared by Start_Network_Session
		unsigned short PeerPorts[MAX_PLAYERS]; // RA2: +0x2DC0, cleared by Start_Network_Session
		char NatPeerNames[MAX_PLAYERS][36]; // RA2: +0x2DD0, names of the players behind the local player's NAT (Allocate_Player_Ports)
		bool SendDelay; // RA2: [MultiPlayer] SendDelay of RA2.INI
		unsigned char PeerAttemptBlock[0x23]; // RA2: +0x2EF1, three bytes and then eight ints (+0x2EF4) that Connect_Player_Pairs clears; what else they hold is not known
		int KeepAliveSlot[MAX_PLAYERS]; // RA2: +0x2F14, the slot of each peer whose keepalive (NET_PING) is due (Queue_Peer_Timeout, Retry_Requests)
		CDTimerClass<SystemTimerClass> KeepAliveTimer[MAX_PLAYERS]; // RA2: +0x2F34, the countdown of each (1200 ticks)
		int KeepAliveCount; // RA2: +0x2F94, how many keepalives wait
		int PortBase;
		int ForcePortBase;
		int PlayerPortBase[MAX_PLAYERS]; // RA2: +0x2FA0, the port each player listens on (Address.Set_Port reads it)
		bool RotatePortBase; // RA2: +0x2FC0, Start_Network_Session moves PortBase by 5 once when it is set
		int PortNumberOverride; // RA2: [MultiPlayer] PortNumberOverride of RA2.INI, the source port the user sets (0 none)
		wchar_t MasterPlayerName[MPLAYER_NAME_MAX + 1];
		int PlayerLastTime[8]; // RA2: +0x2FF4, set to the tick count when an Internet game starts (Select_Game)
		bool PortProbeDue; // RA2: +0x3014, a port probe of the router is due (Conquer tests it, Start_Network_Session clears it)
		unsigned char : 8;
		unsigned short RouterProbePort; // RA2: +0x3016, cleared by Start_Network_Session
		unsigned short RouterProbePort2; // RA2: +0x3018, cleared by Select_Game before the Internet game dialogs
		unsigned short : 16;
		int MasterPlayerID;
		bool RouterProbeActive; // RA2: +0x3020, cleared by Start_Network_Session
		unsigned short PlayerPorts[MAX_PLAYERS];
		unsigned char unknown_308A[2];
		int SlowestResponse; // RA2: +0x3034, the largest response time seen
		bool UseRouter; // RA2: +0x3038, cleared by -NOROUTER (Parse_Command_Line); the Internet game tests it
		int LastTimingID; // RA2: +0x303C, the id of the last timing report (-1 none)
		bool AboutToExitActive; // RA2: +0x3040, set while an ABOUT_TO_EXIT event is processed (Execute)
		unsigned int RouterResponse; // RA2: +0x3044, the router response time (default 20), shown as "Rout resp"
		int ExitEventPending; // RA2: +0x3048, an EXIT event waits to be moved to ExitEventFrame (Queue)
		int ExitEventFrame; // RA2: +0x304C, the frame the EXIT event moves to
		int ExitEventPlayer; // RA2: +0x3050, the player that exits (-1 none)

		int TrapFrame;
		RTTIType TrapObjType;
		TrapObjectType TrapObject;
		Coord TrapCoord;
		TargetClass TrapTarget;
		CellClass * TrapCell;
		int TrapCheckHeap;
		int TrapPrintCRC;

		bool CurrentlyInGame;
};

extern SessionClass Session;

void Leave_Game_Hook(void);

void Draw_Multiplayer_Debug(bool flag); // RA2: 0x005419E0, draws the multiplayer debug text when Session.EnableMultiplayerDebug is set

struct SessionPortAllocator : public SessionClass { // RA2: home of Allocate_Player_Ports (0x00673090), no data: same code as a SessionClass member
	void Allocate_Player_Ports(void); // RA2: as above
};

