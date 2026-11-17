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

/*************************************************************************************
 **   C O N F I D E N T I A L --- W E S T W O O D    S T U D I O S                  **
 *************************************************************************************
 *                                                                                   *
 *                 Project Name : Command & Conquer - Red Alert                      *
 *                                                                                   *
 *                    File Name : SENDFILE.CPP                                       *
 *                                                                                   *
 *                   Programmer : Steve Tall                                         *
 *                                                                                   *
 *                   Start Date : Audust 20th, 1996                                  *
 *                                                                                   *
 *                  Last Update : August 20th, 1996 [ST]                             *
 *                                                                                   *
 *-----------------------------------------------------------------------------------*
 * Overview:                                                                         *
 *                                                                                   *
 *  Functions for scenario file transfer between machines                            *
 *                                                                                   *
 *-----------------------------------------------------------------------------------*
 * Functions:                                                                        *
 *                                                                                   *
 *                                                                                   *
 * - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - */

#include "always.h" // RA2: as above

#include "cd.h" // RA2: as above
#include "ccfile.h" // RA2: CCFileClass
#include "conquer.h" // RA2: as above
#include "dbgprint.h" // RA2: as above
#include "globals.h" // RA2: as above
#include "ini.h" // RA2: as above
#include "ipxmgr.h" // RA2: as above
#include "msgloop.h" // RA2: as above
#include "nullmgr.h" // RA2: as above
#include "progress.h" // RA2: as above
#include "rawfile.h" // RA2: RawFileClass
#include "sendfile.h" // RA2: the declarations of the file
#include "session.h" // RA2: as above
#include "stimer.h" // RA2: as above
#include "wolapi.h" // RA2: User, for the owner of an Internet channel
#undef LoadString // RA2: the Windows macro hides StringTable::LoadString
#include "stringtable.h" // RA2: the progress caption comes from the string table

User * Find_Owner_User(void); // RA2: 0x0074DA60, the channel user that owns the channel, NULL if none (WOL)
char * __fastcall Wide_To_Narrow(wchar_t const * text); // RA2: 0x006FA120, a static buffer
wchar_t * __fastcall Narrow_To_Wide(char const * narrow); // RA2: 0x006FA180, a static buffer
int __fastcall Get_Timeout(int gametype); // RA2: 0x0066AEA0, defined below
class Surface; // RA2: for Darken_Surface
void __fastcall Darken_Surface(Surface * surface, bool update); // RA2: 0x005159B0, dims the screen behind the dialog, defined in init.cpp

#define RESPONSE_TIMEOUT	TIMER_MINUTE // RA2: as above
#define FILE_PACKET_BUFFER_SIZE	0x1E7 // RA2: the receive buffer of Receive_Remote_File

namespace OwnerDraw { // RA2: WideToCharString lives in the OwnerDraw namespace (0x006FA0F0, as modemgst.cpp)
void __fastcall WideToCharString(char * dest, wchar_t const * src, int count); // RA2: as above
}


/***********************************************************************************************
 * Get_Scenario_File_From_Host -- Initiates download of scenario file from game host           *
 *                                                                                             *
 *                                                                                             *
 *                                                                                             *
 * INPUT:    ptr to buffer to copy file name into                                              *
 *           game type - 0 for modem/null modem, 1 otherwise                                   *
 *                                                                                             *
 * OUTPUT:   true if file sucessfully downloaded                                               *
 *                                                                                             *
 * WARNINGS: None                                                                              *
 *                                                                                             *
 * HISTORY:                                                                                    *
 *    8/22/96 3:06PM ST : Created                                                              *
 *=============================================================================================*/
/// <summary>
/// How long a file transfer waits for the next packet, in timer ticks. The debug texts of the executable call it GetTimeout. Internet games wait three times as long.
/// </summary>
int __fastcall Get_Timeout(int gametype) // RA2: 0x0066AEA0, not in TibSun
{
	//DebugString ("RA95 - In Get_Scenario_From_Host\n");
	switch (gametype) { // RA2: as above
	case GAME_IPX: // RA2: 3
		DebugString("GetTimeout: GAME_IPX (%d)\n", RESPONSE_TIMEOUT); // RA2: as above
		return(RESPONSE_TIMEOUT); // RA2: as above
	case GAME_INTERNET: // RA2: 4
		DebugString("GetTimeout: GAME_INTERNET (%d)\n", RESPONSE_TIMEOUT * 3); // RA2: as above
		return(RESPONSE_TIMEOUT * 3); // RA2: as above
	}
	DebugString("GetTimeout: RESPONSE_TIMEOUT(%d)\n", RESPONSE_TIMEOUT); // RA2: as above
	return(RESPONSE_TIMEOUT); // RA2: as above
}


/// <summary>
/// Asks the game host for the scenario file and starts the download. Sends the request (modem: SERIAL_REQ_SCENARIO, network: NET_REQ_SCENARIO with the player's name), waits for the host's file info packet, acknowledges it and hands over to Receive_Remote_File. Every wait gives up after RESPONSE_TIMEOUT.
/// </summary>
bool __fastcall Get_File_From_Host(char *return_name, int gametype, bool show_progress) // RA2: 0x0066AEF0, a fastcall, the same signature as TibSun
{
	unsigned int file_length = 0; // RA2: as above

	SerialPacketType 	send_packet; // RA2: as above
	SerialPacketType 	receive_packet; // RA2: as above
	GlobalPacketType	net_send_packet; // RA2: as above
	GlobalPacketType	net_receive_packet; // RA2: as above
	int					packet_len; // RA2: an int, the length is passed on to the Get_Message functions
	unsigned short		product_id; // RA2: as above

	IPXAddressClass	sender_address; // RA2: as above

	DebugString("Getting file from host\n"); // RA2: as above

	CDTimerClass<SystemTimerClass> response_timer;		// timeout timer for waiting for responses // RA2: as above

	DebugString("Requesting file download\n"); // RA2: as above

	response_timer = RESPONSE_TIMEOUT; // RA2: as above

	/*
	**	Send the scenario request using guaranteed delivery.
	*/
	if (gametype == GAME_MODEM || gametype == GAME_NULL_MODEM) { // RA2: as above
		memset ((void*)&send_packet, 0, sizeof (send_packet)); // RA2: as above
		send_packet.Command = SERIAL_REQ_SCENARIO; // RA2: as above
		NullModem.Send_Message (&send_packet, sizeof(send_packet), 1); // RA2: as above
		while (NullModem.Num_Send() > 0 && response_timer) { // RA2: as above
			Call_Back(); // RA2: as above
		}
	} else {
		memset ((void*)&net_send_packet, 0, sizeof (net_send_packet)); // RA2: as above
		net_send_packet.Command = NET_REQ_SCENARIO; // RA2: as above
		OwnerDraw::WideToCharString(net_send_packet.Name, Session.Players[0]->Name, -1); // RA2: the request carries the player's name, the host answers by name in an Internet game
		if (Session.Type == GAME_IPX) { // RA2: a LAN game sends to the host's address, an Internet game to the channel owner's name
			Ipx.Send_Global_Message (&net_send_packet, sizeof (net_send_packet), // RA2: as above
				1, &(Session.HostAddress), 0, 0); // RA2: port and packet ID zero
		} else {
			User * host = Find_Owner_User(); // RA2: as above
			if (host != NULL) { // RA2: as above
				Ipx.Send_Global_Message_To_Name(&net_send_packet, sizeof(net_send_packet), 1, Narrow_To_Wide((char const *)host->name)); // RA2: as above
			}
		}
		while (Ipx.Global_Num_Send() > 0 && response_timer) { // RA2: as above
			Call_Back(); // RA2: as above
		}
	}

	//DebugString ("RA95 - Waiting for response from host\n");

	if (!response_timer) { // RA2: as above
		DebugString("Timeout waiting for REQ_SCENARIO packet send\n"); // RA2: as above
		return(false); // RA2: as above
	}

	/*
	**	Wait for host to respond with a file info packet
	*/
	response_timer = RESPONSE_TIMEOUT; // RA2: as above
	if (gametype == GAME_MODEM || gametype == GAME_NULL_MODEM){ // RA2: as above
		do { // RA2: as above
			Call_Back(); // RA2: as above

			if (NullModem.Get_Message ((void*)&receive_packet, &packet_len) > 0 && receive_packet.Command == SERIAL_FILE_INFO) { // RA2: one condition
				strcpy (return_name, receive_packet.ScenarioInfo.ShortFileName); // RA2: as above
				file_length = receive_packet.ScenarioInfo.FileLength; // RA2: as above
				DebugString("Host responded with file info\n"); // RA2: as above
				DebugString("File name is %s\n", return_name); // RA2: as above
				break; // RA2: as above
			}
		} while (response_timer); // RA2: as above
	} else {
		do { // RA2: as above
			Call_Back(); // RA2: as above
			int receive_packet_length = sizeof(net_receive_packet); // RA2: as above
			if (Ipx.Get_Global_Message(&net_receive_packet, &receive_packet_length, &sender_address, &product_id)) { // RA2: as above
				bool from_host = sender_address.Is_Same_Node(Session.HostAddress); // RA2: a LAN game accepts the host's node only
				if (Session.Type == GAME_INTERNET) { // RA2: an Internet game accepts the channel owner's name only
					User * host = Find_Owner_User(); // RA2: as above
					from_host = host != NULL && _stricmp((char const *)host->name, net_receive_packet.Name) == 0; // RA2: as above
				}
				if (net_receive_packet.Command == NET_FILE_INFO && from_host) { // RA2: as above
					strcpy (return_name, net_receive_packet.ScenarioInfo.ShortFileName); // RA2: as above
					file_length = net_receive_packet.ScenarioInfo.FileLength; // RA2: as above
					DebugString("Host responded with file info\n"); // RA2: as above
					DebugString("File name is %s\n", return_name); // RA2: as above
					break; // RA2: as above
				}
			}
		} while (response_timer); // RA2: as above
	}


	/*
	**	If we timed out then something horrible has happened to the other player so just
	**	return failure.
	*/
	if (!response_timer) { // RA2: as above
		DebugString("Timeout waiting for host to respond to file info request\n"); // RA2: as above
		return(false); // RA2: as above
	}

	/*
	**	Acknowledge the file info packet.
	*/
	DebugString("Sending file info received ack\n"); // RA2: as above

	response_timer = RESPONSE_TIMEOUT; // RA2: as above
	if (gametype == GAME_MODEM || gametype == GAME_NULL_MODEM) { // RA2: as above
		memset ((void*)&send_packet, 0, sizeof (send_packet)); // RA2: as above
		send_packet.Command = SERIAL_FILE_INFO_ACK; // RA2: as above
		NullModem.Send_Message (&send_packet, sizeof(send_packet), 1); // RA2: as above
		while (NullModem.Num_Send() > 0 && response_timer) { // RA2: as above
			Call_Back(); // RA2: as above
		}
	} else {
		memset ((void*)&net_send_packet, 0, sizeof (net_send_packet)); // RA2: as above
		net_send_packet.Command = NET_FILE_INFO_ACK; // RA2: as above
		strcpy(net_send_packet.Name, Wide_To_Narrow(Session.Players[0]->Name)); // RA2: the ack carries the player's name
		if (Session.Type == GAME_INTERNET) { // RA2: as above
			User * host = Find_Owner_User(); // RA2: as above
			if (host != NULL) { // RA2: as above
				Ipx.Send_Global_Message_To_Name(&net_send_packet, sizeof(net_send_packet), 1, Narrow_To_Wide((char const *)host->name)); // RA2: as above
			}
		} else {
			Ipx.Send_Global_Message (&net_send_packet, sizeof (net_send_packet), // RA2: as above
				1, &(Session.HostAddress), 0, 0); // RA2: as above
		}
		while (Ipx.Global_Num_Send() > 0 && response_timer) { // RA2: as above
			Call_Back(); // RA2: as above
		}
	}

	if (!response_timer) { // RA2: as above
		DebugString("Timeout waiting for FILE_INFO_ACK send\n"); // RA2: as above
		return(false); // RA2: as above
	}

//	DebugString( "about to download '%s'\n", return_name );
	bool result = Receive_Remote_File(return_name, file_length, gametype, show_progress); // RA2: as above
	DebugString("Receive_Remote_File returned %s\n", result ? "true" : "false"); // RA2: as above
	return(result); // RA2: as above
}


/***********************************************************************************************
 * Receive_Remote_File -- Handles incoming file download packets from the game host            *
 *                                                                                             *
 *                                                                                             *
 *                                                                                             *
 * INPUT:    file name to save as                                                              *
 *           length of file to expect                                                          *
 *           game type - 0 for modem/null modem, 1 otherwise                                   *
 *                                                                                             *
 * OUTPUT:   true if file downloaded was completed                                             *
 *                                                                                             *
 * WARNINGS: This fuction can modify the file name passed in                                   *
 *                                                                                             *
 * HISTORY:                                                                                    *
 *   8/22/96 3:07PM ST : Created                                                               *
 *=============================================================================================*/
/// <summary>
/// Receives the scenario file the host sends in chunks of 192 bytes, collects them in a buffer by block number and writes the file when the timeout runs out or all blocks are in. A partial file is deleted again. Shows a progress bar when asked.
/// </summary>
bool __fastcall Receive_Remote_File(char *file_name, unsigned int file_length, int gametype, bool show_progress) // RA2: 0x0066B520, the game type and the progress flag; the old header declaration had a CRC argument instead
{
	CDTimerClass<SystemTimerClass> response_timer;		// timeout timer for waiting for responses // RA2: as above
	IPXAddressClass	sender_address; // RA2: as above
	unsigned short		product_id; // RA2: as above
	int					packet_len; // RA2: as above
	bool return_code = false; // RA2: as above
	RemoteFileTransferType * receive_packet = (RemoteFileTransferType *)new char[FILE_PACKET_BUFFER_SIZE]; // RA2: a buffer of 487 bytes, big enough for any packet of the transfer

	DebugString("Receiving download of file %s\n", file_name); // RA2: as above
	RawFileClass save_file(file_name); // RA2: as above
	if (save_file.Is_Available()) { // RA2: as above
		save_file.Delete(); // RA2: as above
	}
	save_file.Open(FileClass::WRITE); // RA2: as above

	int total_blocks = file_length / MAX_SEND_FILE_PACKET_SIZE; // RA2: 192 data bytes per chunk
	if (file_length % MAX_SEND_FILE_PACKET_SIZE) { // RA2: as above
		total_blocks++; // RA2: as above
	}
	char * received = new char[total_blocks]; // RA2: as above
	memset(received, 0, total_blocks); // RA2: as above
	int blocks_received = 0; // RA2: as above
	char * buffer = new char[file_length]; // RA2: as above

	if (show_progress) { // RA2: as above
		Progress.Initialize(100.0, 1, NULL); // RA2: as above
		Progress.Set_Graphic_Data("PROGBARM.SHP", NULL, NULL); // RA2: as above
		Progress.Set_Layout(Point2D(-1, -1), StringTable::LoadString("GUI:MapTransfer", NULL, "D:\\Ra2\\SendFile.CPP", 358), false, true); // RA2: as above
		Darken_Surface(NULL, true); // RA2: as above
		Progress.Display_Progress(Point2D(-1, -1)); // RA2: as above
		Progress.Set_Progress_Percent(0, 0.0, Point2D(-1, -1)); // RA2: as above
	}
	if (Session.Type == GAME_INTERNET) { // RA2: an Internet game transfers with slower retry timing
		Ipx.Save_Timing(); // RA2: as above
		Ipx.Set_External_Timing(120, -1, 1200); // RA2: as above
	}

	/*
	**	Wait for all the blocks to arrive
	*/
	response_timer = Get_Timeout(gametype); // RA2: as above
	int percent = 0; // RA2: as above
	do { // RA2: as above
		if (gametype == GAME_MODEM || gametype == GAME_NULL_MODEM) { // RA2: as above
			Windows_Message_Handler(); // RA2: as above
			Call_Back(); // RA2: as above
			if (NullModem.Get_Message(receive_packet, &packet_len) > 0 && receive_packet->Command == SERIAL_FILE_CHUNK) { // RA2: as above
				if (received[receive_packet->BlockNumber] == 0) { // RA2: as above
					received[receive_packet->BlockNumber] = 1; // RA2: as above
					blocks_received++; // RA2: as above
					percent += 100; // RA2: as above
					response_timer = RESPONSE_TIMEOUT / 2; // RA2: a received chunk restarts the wait at half a minute
					DebugString("Received file chunk %d\n", receive_packet->BlockNumber); // RA2: as above
					memcpy(buffer + receive_packet->BlockNumber * MAX_SEND_FILE_PACKET_SIZE, receive_packet->RawData, receive_packet->BlockLength); // RA2: as above
					if (show_progress) { // RA2: as above
						Progress.Set_Progress_Percent(0, percent / total_blocks, Point2D(-1, -1)); // RA2: as above
					}
					if (blocks_received == total_blocks) { // RA2: as above
						if (show_progress) { // RA2: as above
							Progress.Set_Progress_Percent(0, 100.0, Point2D(-1, -1)); // RA2: as above
						}
						return_code = true; // RA2: as above
						break; // RA2: as above
					}
				}
			}
		} else {
			Windows_Message_Handler(); // RA2: as above
			Call_Back(); // RA2: as above
			int receive_packet_length = FILE_PACKET_BUFFER_SIZE; // RA2: as above
			if (Ipx.Get_Global_Message(receive_packet, &receive_packet_length, &sender_address, &product_id)) { // RA2: as above
				bool from_host = sender_address.Is_Same_Node(Session.HostAddress); // RA2: as above
				if (Session.Type == GAME_INTERNET) { // RA2: as above
					User * host = Find_Owner_User(); // RA2: as above
					from_host = host != NULL && _stricmp((char const *)host->name, receive_packet->Name) == 0; // RA2: as above
				}
				if (receive_packet->Command == (SerialCommandType)NET_FILE_CHUNK && from_host) { // RA2: as above
					if (received[receive_packet->BlockNumber] == 0) { // RA2: as above
						received[receive_packet->BlockNumber] = 1; // RA2: as above
						blocks_received++; // RA2: as above
						percent += 100; // RA2: as above
						response_timer = Get_Timeout(gametype) / 2; // RA2: as above
						DebugString("Received file chunk %d/%d with %d seconds left\n", receive_packet->BlockNumber, total_blocks, (long)response_timer); // RA2: as above
						memcpy(buffer + receive_packet->BlockNumber * MAX_SEND_FILE_PACKET_SIZE, receive_packet->RawData, receive_packet->BlockLength); // RA2: as above
						if (show_progress) { // RA2: as above
							Progress.Set_Progress_Percent(0, percent / total_blocks, Point2D(-1, -1)); // RA2: as above
						}
						if (blocks_received == total_blocks) { // RA2: as above
							return_code = true; // RA2: as above
							if (show_progress) { // RA2: as above
								Progress.Set_Progress_Percent(0, 100.0, Point2D(-1, -1)); // RA2: as above
							}
							break; // RA2: as above
						}
					}
				}
			}
		}
	} while (response_timer); // RA2: as above

	if (Session.Type == GAME_INTERNET) { // RA2: as above
		Ipx.Restore_Timing(); // RA2: as above
	}
	save_file.Write(buffer, file_length); // RA2: as above
	delete [] buffer; // RA2: as above
	delete [] received; // RA2: as above
	if (return_code) { // RA2: as above
		DebugString("File download completed OK\n"); // RA2: as above
	} else {
		DebugString("File download failed to complete\n"); // RA2: as above
	}
	save_file.Close(); // RA2: as above
	DebugString("Download file closed\n"); // RA2: as above
	if (!return_code && save_file.Is_Available()) { // RA2: as above
		save_file.Delete(); // RA2: as above
		DebugString("Partial file deleted\n"); // RA2: as above
	}
	delete [] (char *)receive_packet; // RA2: as above
	return(return_code); // RA2: as above
}


/***********************************************************************************************
 * Send_Remote_File -- Sends a file to game clients                                            *
 *                                                                                             *
 *                                                                                             *
 *                                                                                             *
 * INPUT:    File name                                                                         *
 *                                                                                             *
 * OUTPUT:   true if file transfer was successfully completed                                  *
 *           game type - 0 for modem/null modem, 1 otherwise                                   *
 *                                                                                             *
 * WARNINGS: None                                                                              *
 *                                                                                             *
 * HISTORY:                                                                                    *
 *    8/22/96 3:09PM ST : Created                                                              *
 *=============================================================================================*/
/// <summary>
/// Sends a scenario file to the clients that asked for it: first the file info (name and length), then, after the acknowledgements, the file in chunks of 192 bytes. Over a modem the chunks go to the other machine; on a network to the players that requested the scenario (send_to_all: to every other player). Shows a progress bar when asked.
/// </summary>
bool __fastcall Send_Remote_File ( char const *file_name, int gametype, bool send_to_all, bool show_progress ) // RA2: 0x0066BB20, a fastcall; TibSun has the same four parameters
{
	CDTimerClass<SystemTimerClass> response_timer;		// timeout timer for waiting for responses // RA2: as above
	int percent = 0; // RA2: as above
	bool return_code = false; // RA2: as above
	SerialPacketType file_info; // RA2: as above
	SerialPacketType receive_packet; // RA2: as above
	GlobalPacketType net_file_info; // RA2: as above
	GlobalPacketType net_receive_packet; // RA2: as above
	RemoteFileTransferType send_packet; // RA2: as above
	int packet_len; // RA2: as above
	unsigned short product_id; // RA2: as above

	DebugString("Send file %s to clients\n", file_name); // RA2: as above
	CCFileClass file(file_name); // RA2: as above
	if (!file.Is_Available()) { // RA2: as above
		return(false); // RA2: as above
	}
	int file_length = file.Size(); // RA2: as above

	response_timer = RESPONSE_TIMEOUT; // RA2: as above

	DebugString("Sending file info to clients\n"); // RA2: as above

	/*
	**	Send the file info to the remote machine(s)
	*/
	if (gametype == GAME_MODEM || gametype == GAME_NULL_MODEM) { // RA2: as above
		file_info.Command = SERIAL_FILE_INFO; // RA2: as above
		strcpy(file_info.ScenarioInfo.ShortFileName, file_name); // RA2: as above
		file_info.ScenarioInfo.FileLength = file_length; // RA2: as above
		NullModem.Send_Message(&file_info, sizeof(file_info), 1); // RA2: as above
		while (NullModem.Num_Send() > 0 && response_timer) { // RA2: as above
			Call_Back(); // RA2: as above
		}
	} else {
		net_file_info.Command = NET_FILE_INFO; // RA2: as above
		strcpy(net_file_info.ScenarioInfo.ShortFileName, file_name); // RA2: as above
		net_file_info.ScenarioInfo.FileLength = file_length; // RA2: as above
		strcpy(net_file_info.Name, Wide_To_Narrow(Session.Players[0]->Name)); // RA2: the packet carries the host's name
		if (send_to_all) { // RA2: every other player gets the file info
			for (int i = 1; i < Session.Players.Count(); i++) { // RA2: as above
				do { // RA2: as above
					Call_Back(); // RA2: as above
				} while (!Ipx.Send_Global_Message_To_Name(&net_file_info, sizeof(net_file_info), 1, Session.Players[i]->Name) && response_timer); // RA2: as above
			}
		} else { // RA2: only the players that requested the scenario
			for (int i = 0; i < Session.RequestCount; i++) { // RA2: as above
				do { // RA2: as above
					Call_Back(); // RA2: as above
					DebugString("Sending file info packet to player %d\n", Session.Players[Session.ScenarioRequests[i]]->Name); // RA2: as above
				} while (!Ipx.Send_Global_Message_To_Name(&net_file_info, sizeof(net_file_info), 1, Session.Players[Session.ScenarioRequests[i]]->Name) && response_timer); // RA2: as above
			}
		}

		while (Ipx.Global_Num_Send() > 0 && response_timer) { // RA2: as above
			Call_Back(); // RA2: as above
		}
	}
	if (!response_timer) { // RA2: as above
		DebugString("File info send timed out\n"); // RA2: as above
		return(false); // RA2: as above
	}

	/*
	**	Wait for the acknowledgements of the file info.
	*/
	IPXAddressClass sender_address; // RA2: as above
	int acks = 0; // RA2: as above
	int clients = send_to_all ? Session.Players.Count() - 1 : Session.RequestCount; // RA2: as above
	response_timer = RESPONSE_TIMEOUT; // RA2: as above

	if (gametype == GAME_MODEM || gametype == GAME_NULL_MODEM) { // RA2: as above
		do { // RA2: as above
			Call_Back(); // RA2: as above
			if (NullModem.Get_Message(&receive_packet, &packet_len) > 0 && receive_packet.Command == SERIAL_FILE_INFO_ACK) { // RA2: as above
				DebugString("Client responded with file info ack\n"); // RA2: as above
				break; // RA2: as above
			}
		} while (response_timer && acks < clients); // RA2: as above
	} else {
		do { // RA2: as above
			Call_Back(); // RA2: as above
			int receive_packet_length = sizeof(net_receive_packet); // RA2: as above
			if (Ipx.Get_Global_Message(&net_receive_packet, &receive_packet_length, &sender_address, &product_id) && net_receive_packet.Command == NET_FILE_INFO_ACK) { // RA2: as above
				acks++; // RA2: as above
			}
		} while (response_timer && acks < clients); // RA2: as above
	}

	int total_blocks = (file_length + MAX_SEND_FILE_PACKET_SIZE - 1) / MAX_SEND_FILE_PACKET_SIZE; // RA2: as above
	file.Open(FileClass::READ); // RA2: as above
	int block_number = 0; // RA2: as above
	if (show_progress) { // RA2: as above
		Progress.Initialize(100.0, 1, NULL); // RA2: as above
		Progress.Set_Graphic_Data("PROGBARM.SHP", NULL, NULL); // RA2: as above
		Progress.Set_Layout(Point2D(-1, -1), StringTable::LoadString("GUI:MapTransfer", NULL, "D:\\Ra2\\SendFile.CPP", 659), false, true); // RA2: as above
		Darken_Surface(NULL, true); // RA2: as above
		Progress.Display_Progress(Point2D(-1, -1)); // RA2: as above
		Progress.Set_Progress_Percent(0, 0.0, Point2D(-1, -1)); // RA2: as above
	}
	if (Session.Type == GAME_INTERNET) { // RA2: an Internet game transfers with slower retry timing
		Ipx.Save_Timing(); // RA2: as above
		Ipx.Set_External_Timing(240, -1, 1200); // RA2: as above
	}

	response_timer = Get_Timeout(gametype); // RA2: as above
	percent = 0; // RA2: as above
	while (response_timer) { // RA2: as above
		if (gametype == GAME_MODEM || gametype == GAME_NULL_MODEM) { // RA2: as above
			Windows_Message_Handler(); // RA2: as above
			Call_Back(); // RA2: as above
			if (block_number < total_blocks) { // RA2: as above
				if (NullModem.Num_Send() < 2) { // RA2: as above
					send_packet.Command = SERIAL_FILE_CHUNK; // RA2: as above
					send_packet.BlockNumber = block_number; // RA2: as above
					send_packet.BlockLength = file_length; // RA2: as above
					if (file_length >= MAX_SEND_FILE_PACKET_SIZE) { // RA2: as above
						send_packet.BlockLength = MAX_SEND_FILE_PACKET_SIZE; // RA2: as above
					}
					file_length -= send_packet.BlockLength; // RA2: as above
					if (file.Read(send_packet.RawData, send_packet.BlockLength) == send_packet.BlockLength) { // RA2: as above
						DebugString("Sending file chunk %d/%d, %d sec left\n", block_number, total_blocks, (long)response_timer); // RA2: as above
						NullModem.Send_Message(&send_packet, sizeof(send_packet), 1); // RA2: as above
					}
					block_number++; // RA2: as above
					percent += 100; // RA2: as above
					if (show_progress) { // RA2: as above
						Progress.Set_Progress_Percent(0, percent / total_blocks, Point2D(-1, -1)); // RA2: as above
					}

				}
			} else if (NullModem.Num_Send() == 0) { // RA2: as above
				return_code = true; // RA2: as above
				if (show_progress) { // RA2: as above
					Progress.Set_Progress_Percent(0, 100.0, Point2D(-1, -1)); // RA2: as above
				}
				break; // RA2: as above
			}
		} else {
			Windows_Message_Handler(); // RA2: as above
			Call_Back(); // RA2: as above
			if (block_number < total_blocks) { // RA2: as above
				if (Ipx.Global_Num_Send() < 10) { // RA2: as above
					send_packet.Command = (SerialCommandType)NET_FILE_CHUNK; // RA2: the chunk travels in a RemoteFileTransferType over the network too
					send_packet.BlockNumber = block_number; // RA2: as above
					send_packet.BlockLength = file_length; // RA2: as above
					if (file_length >= MAX_SEND_FILE_PACKET_SIZE) { // RA2: as above
						send_packet.BlockLength = MAX_SEND_FILE_PACKET_SIZE; // RA2: as above
					}
					strcpy(send_packet.Name, Wide_To_Narrow(Session.Players[0]->Name)); // RA2: as above
					file_length -= send_packet.BlockLength; // RA2: as above
					if (file.Read(send_packet.RawData, send_packet.BlockLength) == send_packet.BlockLength) { // RA2: as above
						DebugString("Sending file chunk %d/%d, %d sec left\n", block_number, total_blocks, (long)response_timer); // RA2: as above
						if (send_to_all) { // RA2: as above
							for (int i = 1; i < Session.Players.Count(); i++) { // RA2: as above
								int sent; // RA2: as above
								do { // RA2: as above
									sent = Ipx.Send_Global_Message_To_Name(&send_packet, sizeof(send_packet), 1, Session.Players[i]->Name); // RA2: as above
									Call_Back(); // RA2: as above
								} while (!sent); // RA2: as above
							}
						} else { // RA2: as above
							for (int i = 0; i < Session.RequestCount; i++) { // RA2: as above
								int sent; // RA2: as above
								do { // RA2: as above
									sent = Ipx.Send_Global_Message_To_Name(&send_packet, sizeof(send_packet), 1, Session.Players[Session.ScenarioRequests[i]]->Name); // RA2: as above
									Call_Back(); // RA2: as above
								} while (!sent); // RA2: as above
							}
						}
					}

					block_number++; // RA2: as above
					percent += 100; // RA2: as above
					if (show_progress) { // RA2: as above
						Progress.Set_Progress_Percent(0, percent / total_blocks, Point2D(-1, -1)); // RA2: as above
					}

				}
			} else if (Ipx.Global_Num_Send() == 0) { // RA2: as above
				if (Ipx.Global_Num_Send() == 0) { // RA2: tested twice
					DebugString("We sent the file properly with %d seconds left!\n", (long)response_timer); // RA2: as above
					return_code = true; // RA2: as above
					if (show_progress) { // RA2: as above
						Progress.Set_Progress_Percent(0, 100.0, Point2D(-1, -1)); // RA2: as above
					}
					break; // RA2: as above
				}
			}
		}

	}
	if (Session.Type == GAME_INTERNET) { // RA2: as above
		Ipx.Restore_Timing(); // RA2: as above
	}
	DebugString("File upload completed - %s\n", return_code ? "success" : "failure"); // RA2: as above
	return(return_code); // RA2: as above
}


/***********************************************************************************************
 * Find_Local_Scenario -- finds the file name of the scenario with matching attributes         *
 *                                                                                             *
 *                                                                                             *
 *                                                                                             *
 * INPUT:    ptr to Scenario description                                                       *
 *           ptr to Scenario filename to fix up                                                *
 *           length of file for trivial rejection of scenario files                            *
 *           ptr to digest. Digests must match.                                                *
 *                                                                                             *
 *                                                                                             *
 * OUTPUT:   true if scenario is available locally                                             *
 *                                                                                             *
 * WARNINGS: We need to reject files that don't match exactly because scenarios with the same  *
 *           description can exist on both machines but have different contents. For example   *
 *           there will be lots of scenarios called 'my map' and 'crap' and 'aaaaaa'.          *
 *                                                                                             *
 * HISTORY:                                                                                    *
 *    8/23/96 12:36PM ST : Created                                                             *
 *=============================================================================================*/
/// <summary>
/// Finds the scenario the host named among the local scenarios: same file name, the file available (the CD is asked for when check_cd is set), same length and, when a digest is given, the same digest. Copies the local file name into the buffer and returns true on a match.
/// </summary>
bool __fastcall Find_Local_Scenario (char *filename, unsigned int length, char *digest, bool official, bool checkcd) // RA2: 0x0066C5B0
{
//FILE *fp;
//fp = fopen("findscen.txt","wt");
//DebugString("looking for local scenario: description = %s, name=%s, length=%d, digest=%s, official=%d\n", description, filename, length, digest, official);
	if (_stricmp(filename, "RandMap.Sed") != 0) { // RA2: strcmpi in TibSun, the random map is never a local scenario
		for (int index = 0; index < Session.Scenarios.Count(); index++) { // RA2: as above
			if (_stricmp(Session.Scenarios[index]->Get_Filename(), filename) == 0) { // RA2: a case-insensitive compare
				CCFileClass file (Session.Scenarios[index]->Get_Filename()); // RA2: as above

				if (!file.Is_Available() && checkcd == true) { // RA2: as above
					MultiMission *mm = Session.Scenarios[index]; // RA2: as above
					if (!mm->Is_On_CD(CD::Get_Current_Disk(60))) { // RA2: a timeout of 60
						CD::Set_Required_Disk(mm->On_Which_CD()); // RA2: as above
					}
					Session.Suspended++; // RA2: as above
					if (!CD::Force_Available()) { // RA2: as above
						Session.Suspended--; // RA2: as above
						return(false); // RA2: as above
					}
					Session.Suspended--; // RA2: as above
				}

				/*
				**	Possible rejection on the basis of availability.
				*/
				if (file.Is_Available() && (unsigned)file.Size() == length) { // RA2: one condition
					INIClass ini; // RA2: as above
					ini.Load(file, false); // RA2: as above
					char digest_buffer[32]; // RA2: as above
					ini.Get_String ("Digest", "1", "No digest here mate. Nope.", digest_buffer, sizeof (digest_buffer) ); // RA2: as above
					if (digest == NULL || *digest == '\0' || !strcmp (digest, digest_buffer)) { // RA2: as above
						strcpy (filename, Session.Scenarios[index]->Get_Filename()); // RA2: as above
						return(true); // RA2: as above
					}
				}
	//			else
	//				DebugString("file not available '%s'.\n", Session.Scenarios[index]->Get_Filename());

			}
		}
	}
//DebugString("failed match.\n");
	/*
	**	Couldnt find the scenario locally. Return failure.
	*/
	return(false); // RA2: as above
}
