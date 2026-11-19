/*******************************************************************************
 *                                O P E N  T S
 *******************************************************************************
 * SPDX-License-Identifier: GPL-3.0-or-later
 * Copyright 2026 OpenTS contributors
 *
 * Contains material derived from Electronic Arts source code.
 * Modified by OpenTS contributors, 2026.
 * EA's GPLv3 Section 7 additional terms and supplemental warranty
 * disclaimers apply; see LICENSE.md.
 ******************************************************************************/

/*
**	RA2: the firewall helper: finds out how the player's router (NAT) rewrites the source ports of UDP packets
**	by sending probes to the mangler servers, and works out which port a peer behind it will be seen on.
**	TibSun has no such file; the code is the executable's from 0x005B9C50.
*/

#include "geometry.h" // RA2: the geometry constants come before the rectangle of rect.h (the executable initializes them first)
#include "always.h" // RA2: the includes this file needs
#include <string.h> // RA2: the includes this file needs
#include "_rules.h" // RA2: the includes this file needs
#include "_timer.h" // RA2: the includes this file needs
#include "conquer.h" // RA2: the includes this file needs
#include "dbgprint.h" // RA2: the includes this file needs
#include "globals.h" // RA2: the includes this file needs
#include "ipxmgr.h" // RA2: the includes this file needs
#include "rawfile.h" // RA2: the includes this file needs
#include "session.h" // RA2: the includes this file needs
#include "stimer.h" // RA2: the includes this file needs
#include "timer.h" // RA2: the includes this file needs
#include "wsproto.h" // RA2: the includes this file needs
#include "wspudp.h" // RA2: the includes this file needs
#include "firewall.h" // RA2: FirewallHelperClass
#include "rect.h" // RA2: the includes this file needs



FirewallHelperClass FirewallHelper; // RA2: 0x00A71B18, its initializer is the last one before the first function (0x005B9E40)

/// <summary>
/// Starts with nothing detected and the settings not read yet.
/// </summary>
FirewallHelperClass::FirewallHelperClass(void) // RA2: 0x005B9E60
{
	Detected = 0; // RA2: as above
	Settings = 0; // RA2: as above
	Delta = 0; // RA2: as above
	ReadDelta = 0; // RA2: as above
}

/// <summary>
/// Forgets what was detected, so the next Save_Detected runs the detection again.
/// </summary>
void FirewallHelperClass::Clear(void) // RA2: 0x005B9E70
{
	Detected = 0; // RA2: as above
}

/// <summary>
/// Runs the detection when it has not run yet, and writes the result to RA2.INI when it is reliable.
/// </summary>
void FirewallHelperClass::Save_Detected(void) // RA2: 0x005B9E80
{
	bool detected = false; // RA2: as above
	if (Detected == 0) { // RA2: as above
		Detected = Detect(&detected); // RA2: as above
		if (detected) { // RA2: as above
			RawFileClass file("RA2.INI"); // RA2: as above
			ConfigINI.Put_Int("MultiPlayer", "FirewallSettings", Detected, 0); // RA2: as above
			ConfigINI.Put_Int("MultiPlayer", "FirewallDelta", Delta, 0); // RA2: as above
			ConfigINI.Save(file, false); // RA2: as above
		}
	}
}

/// <summary>
/// Hands out the next port of the pool, stepping by the given amount; over the internet a port that cannot be opened is skipped.
/// </summary>
unsigned short FirewallHelperClass::Next_Port(int step) // RA2: 0x005B9F10
{
	unsigned short port; // RA2: as above

	if (step == 0) { // RA2: as above
		step = 1; // RA2: as above
	}
	int tries = 255; // RA2: as above
	do {
		Port += step; // RA2: as above
		port = (unsigned short)Port; // RA2: as above
		if (Port > 0xFFFF) { // RA2: as above
			Port = 0x800; // RA2: as above
		}
		if (PacketTransport && Session.Type == GAME_INTERNET) { // RA2: as above
			if (!((UDPInterfaceClass *)PacketTransport)->Open_Port_Socket(port)) { // RA2: as above
				continue; // RA2: as above
			}
			((UDPInterfaceClass *)PacketTransport)->Close_Port_Socket(port); // RA2: as above
		}
		return port;
	} while (tries-- != 0); // RA2: as above
	return port;
}

/// <summary>
/// Sends one probe from a port to a mangler server and waits until it has gone out.
/// </summary>
bool FirewallHelperClass::Send_Probe(IPXAddressClass * address, unsigned short port, int packet_id, bool flag) // RA2: 0x005B9F90
{
	GlobalPacketType packet; // RA2: as above

	packet.Command = NET_MANGLER_REQUEST; // RA2: as above
	strcpy(packet.Name, "WW"); // RA2: as above
	((UDPInterfaceClass *)PacketTransport)->Select_Port_Socket(port); // RA2: as above
	packet.ManglerData.OriginalPortNumber = port; // RA2: as above
	packet.ManglerData.BlitzMe = (flag == true); // RA2: as above
	int pending = Ipx.Global_Num_Send(); // RA2: as above
	DebugString("Sending from port %d to %s\n", (unsigned short)port, address->As_String(NULL)); // RA2: as above
	Ipx.Send_Global_Message(&packet, 0x38, 0, address, address->Get_Port(), packet_id); // RA2: as above
	CDTimerClass<SystemTimerClass> timer(180); // RA2: as above
	while (Ipx.Global_Num_Send() > pending || PacketTransport->OutBuffers.Count()) { // RA2: as above
		if (timer() == 0) { // RA2: as above
			break;
		}
		Ipx.Service(); // RA2: as above
		Call_Back(); // RA2: as above
	}
	((UDPInterfaceClass *)PacketTransport)->Select_Port_Socket(0); // RA2: as above
	return true;
}

/// <summary>
/// Waits for the mangler's answer to a probe and returns the port it saw the probe come from, or 0 when none came in time.
/// </summary>
unsigned short FirewallHelperClass::Find_Mangled_Port(int packet_id, int time) // RA2: 0x005BA0D0
{
	int peek_packet = 0; // RA2: as above
	IPXAddressClass address; // RA2: as above
	GlobalPacketType packet; // RA2: as above
	unsigned short product_id; // RA2: as above
	int packet_len; // RA2: as above
	unsigned long id; // RA2: as above

	CDTimerClass<SystemTimerClass> timeout(60); // RA2: as above
	if (time) { // RA2: as above
		timeout = CDTimerClass<SystemTimerClass>(time); // RA2: as above
	}
	while (timeout() != 0) { // RA2: as above
		Ipx.Service(); // RA2: as above
		Call_Back(); // RA2: as above
		if (peek_packet >= Ipx.Global_Num_Receive()) { // RA2: as above
			peek_packet = 0; // RA2: as above
		}
		if (Ipx.Peek_Global_Message(peek_packet, &packet, &packet_len, &address, &product_id, &id)) { // RA2: as above
			if (product_id == 0xAA03 && packet.Command == NET_MANGLER_RESPONSE) { // RA2: as above
				int original_port = packet.ManglerData.OriginalPortNumber; // RA2: as above
				if (packet_id == (int)id) { // RA2: as above
					unsigned short mangled_port = packet.ManglerData.MangledPortNumber; // RA2: as above
					DebugString("Mangler is seeing packets from port %d as coming from port %d\n", original_port, (unsigned int)mangled_port); // RA2: as above
					Ipx.Mark_Global_Message_Read(peek_packet); // RA2: as above
					return mangled_port;
				}
			}
		}
		peek_packet++; // RA2: as above
	}
	return 0;
}

/// <summary>
/// Writes the detection result to RA2.INI.
/// </summary>
void FirewallHelperClass::Write_Settings(void) // RA2: 0x005BA210
{
	RawFileClass file("RA2.INI"); // RA2: as above
	ConfigINI.Put_Int("MultiPlayer", "FirewallSettings", Detected, 0); // RA2: as above
	ConfigINI.Put_Int("MultiPlayer", "FirewallDelta", Delta, 0); // RA2: as above
	ConfigINI.Save(file, false); // RA2: as above
}

/// <summary>
/// Reads the saved detection result (what Detect found last time) from RA2.INI's [MultiPlayer] section.
/// </summary>
void FirewallHelperClass::Read_Settings(void) // RA2: 0x005BA280
{
	Settings = ConfigINI.Get_Int("MultiPlayer", "FirewallSettings", 0); // RA2: as above
	ReadDelta = ConfigINI.Get_Int("MultiPlayer", "FirewallDelta", 1); // RA2: as above
}

/// <summary>
/// Probes the mangler servers to find out how the router rewrites source ports. The result is a set of flags (1 no
/// translation, 2 the same port for every destination, 4 a different one, 8 the Netgear bug, 0x10 an absolute and
/// 0x20 a relative port delta, 0x40 a port that depends on the destination port); detected is set when it is reliable.
/// </summary>
int FirewallHelperClass::Detect(bool * detected) // RA2: 0x005BA2C0
{
	unsigned short mangler_port = 4321; // RA2: as above
	unsigned long packet_id = 0x7F000000; // RA2: as above

	static char _manglername[5][32] = { // RA2: 0x007E6334, the mangler servers tried when the server list gave none
		"mangler1.westwood.com", // RA2: as above
		"mangler2.westwood.com", // RA2: as above
		"mangler3.westwood.com", // RA2: as above
		"mangler.westwood.com", // RA2: as above
		"" // RA2: as above
	};

	*detected = false; // RA2: as above
	unsigned char mangler_addresses[4][4]; // RA2: as above
	int num_mangler_addresses = 0; // RA2: as above
	int behavior; // RA2: as above
	IPXAddressClass manglers[4]; // RA2: as above
	CDTimerClass<SystemTimerClass> timeout; // RA2: as above

	unsigned short source_ports[4]; // RA2: as above
	unsigned short mangled_ports[4]; // RA2: as above
	int delta; // RA2: as above
	bool looks_good; // RA2: as above
	int i; // RA2: as above

	if (Session.PortNumberOverride) { // RA2: as above
		behavior = 1; // RA2: as above
		DebugString("Source port %d specified by user\n", Session.PortNumberOverride); // RA2: as above
		if (Session.SendDelay) { // RA2: as above
			behavior |= 8; // RA2: as above
			DebugString("Netgear bug specified by command line or SendDelay flag\n"); // RA2: as above
		}
		return behavior;
	}

	timeout = 300; // RA2: as above
	DebugString("About to call gethostbyname for the mangler address\n"); // RA2: as above
	int namenum = 0; // RA2: as above

	do {
		char * mangler_name_ptr = &_manglername[namenum][0]; // RA2: as above
		if (ManglerCount > namenum) { // RA2: as above
			mangler_name_ptr = ManglerAddresses[namenum]; // RA2: as above
			mangler_port = ManglerPorts[namenum]; // RA2: as above
			DebugString("Using mangler from servserv\n"); // RA2: as above
		}
		namenum++; // RA2: as above

		if (strlen(mangler_name_ptr) == 0) { // RA2: as above
			break;
		}

		char temp_name[256]; // RA2: as above
		strcpy(temp_name, mangler_name_ptr); // RA2: as above
		hostent * host_info = gethostbyname(temp_name); // RA2: as above

		if (!host_info) { // RA2: as above
			DebugString("gethostbyname failed! Error code %d\n", WSAGetLastError()); // RA2: as above
			break;
		}

		bool found = false; // RA2: as above
		for (i = 0; i < num_mangler_addresses; i++) { // RA2: as above
			if (memcmp(mangler_addresses[i], &host_info->h_addr_list[0][0], 4) == 0) { // RA2: as above
				found = true; // RA2: as above
				break;
			}
		}
		if (!found) { // RA2: as above
			int m = num_mangler_addresses++; // RA2: as above
			memcpy(&mangler_addresses[m][0], &host_info->h_addr_list[0][0], 4); // RA2: as above
			DebugString("Found mangler address at %d.%d.%d.%d\n", mangler_addresses[m][0], mangler_addresses[m][1], mangler_addresses[m][2], mangler_addresses[m][3]); // RA2: as above
		}
	} while (num_mangler_addresses < 4 && timeout()); // RA2: as above

	if (num_mangler_addresses < 3) { // RA2: as above
		return 1;
	}

	for (i = 0; i < num_mangler_addresses; i++) { // RA2: as above
		NetNodeType addr; // RA2: as above
		memcpy(addr, &mangler_addresses[i][0], 4); // RA2: as above
		manglers[i].Set_Node(addr); // RA2: as above
		manglers[i].Set_Port(mangler_port); // RA2: as above
	}

	unsigned short port = Next_Port(0); // RA2: as above
	if (!((UDPInterfaceClass *)PacketTransport)->Open_Port_Socket(port)) { // RA2: as above
		return 1;
	}
	PacketTransport->Start_Listening(); // RA2: as above

	timeout = 360; // RA2: as above
	unsigned short mangled_port = 0; // RA2: as above
	while (timeout() && mangled_port == 0) { // RA2: as above
		Send_Probe(&manglers[0], port, packet_id, false); // RA2: as above
		mangled_port = Find_Mangled_Port(packet_id, 0); // RA2: as above
	}

	if (mangled_port == 0 || mangled_port == port) { // RA2: as above
		((UDPInterfaceClass *)PacketTransport)->Close_Port_Socket(port); // RA2: as above
		if (mangled_port == port) { // RA2: as above
			Delta = 0; // RA2: as above
			*detected = true; // RA2: as above
		}
		return 1;
	}

	timeout = 360; // RA2: as above
	unsigned short second_ip_mangled_port = 0; // RA2: as above
	while (timeout() && second_ip_mangled_port == 0) { // RA2: as above
		Send_Probe(&manglers[1], port, packet_id + 1, false); // RA2: as above
		second_ip_mangled_port = Find_Mangled_Port(packet_id + 1, 0); // RA2: as above
	}

	((UDPInterfaceClass *)PacketTransport)->Close_Port_Socket(port); // RA2: as above

	if (second_ip_mangled_port == 0 || second_ip_mangled_port == port) { // RA2: as above
		return 1;
	}

	if (mangled_port == second_ip_mangled_port) { // RA2: as above
		behavior = 2; // RA2: as above
	} else {
		behavior = 4; // RA2: as above
	}

	int try_again; // RA2: as above
	for (try_again = 0; try_again < 3; try_again++) { // RA2: as above
		memset(source_ports, 0, sizeof(source_ports)); // RA2: as above
		memset(mangled_ports, 0, sizeof(source_ports)); // RA2: as above

		for (i = 0; i < 4; i++) { // RA2: as above
			source_ports[i] = Next_Port(i); // RA2: as above
			if (!((UDPInterfaceClass *)PacketTransport)->Open_Port_Socket(source_ports[i])) { // RA2: as above
				for (int j = 0; j < i; j++) { // RA2: as above
					if (source_ports[j]) { // RA2: as above
						((UDPInterfaceClass *)PacketTransport)->Close_Port_Socket(source_ports[j]); // RA2: as above
					}
				}
				return behavior;
			}
		}
		PacketTransport->Start_Listening(); // RA2: as above

		int num_responses = 0; // RA2: as above
		packet_id = packet_id + 10; // RA2: as above
		timeout = 720; // RA2: as above

		while (timeout() && num_responses < 4) { // RA2: as above
			for (i = 0; i < 4; i++) { // RA2: as above
				if (mangled_ports[i] == 0) { // RA2: as above
					Send_Probe(&manglers[0], source_ports[i], packet_id + i, false); // RA2: as above
					mangled_ports[i] = Find_Mangled_Port(packet_id + i, 0); // RA2: as above
					if (mangled_ports[i] != 0) { // RA2: as above
						num_responses++; // RA2: as above
					}
				}
			}
		}

		for (int j = 0; j < i; j++) { // RA2: as above
			if (source_ports[j]) { // RA2: as above
				((UDPInterfaceClass *)PacketTransport)->Close_Port_Socket(source_ports[j]); // RA2: as above
			}
		}

		if (num_responses < 4) { // RA2: as above
			if (ReadDelta != 0 && Settings > 1) { // RA2: as above
				Delta = ReadDelta; // RA2: as above
			}
			return behavior;
		}

		bool relative_delta = false; // RA2: as above
		looks_good = false; // RA2: as above
		delta = Analyse_Ports(num_responses, source_ports, mangled_ports, relative_delta, looks_good); // RA2: as above

		if (delta) { // RA2: as above
			unsigned long addbehavior = 0; // RA2: as above
			if (relative_delta) { // RA2: as above
				addbehavior = 0x20; // RA2: as above
			} else {
				addbehavior = 0x10; // RA2: as above
			}
			addbehavior |= behavior; // RA2: as above
			behavior = addbehavior; // RA2: as above
			Delta = delta; // RA2: as above
			if (looks_good) { // RA2: as above
				*detected = true; // RA2: as above
			}
			break;
		} else {
			if (ReadDelta != 0 && Settings > 1) { // RA2: as above
				Delta = ReadDelta; // RA2: as above
			}
		}
	}

	if ((behavior & 4) != 0) { // RA2: as above
		if ((behavior & 0x10) != 0) { // RA2: as above
			DebugString("Testing to see if the NAT mangles differently per destination port at the same IP\n"); // RA2: as above

			unsigned short port1 = Next_Port(0); // RA2: as above
			if (!((UDPInterfaceClass *)PacketTransport)->Open_Port_Socket(port1)) { // RA2: as above
				return behavior;
			}

			unsigned short port2 = Next_Port(0); // RA2: as above
			if (!((UDPInterfaceClass *)PacketTransport)->Open_Port_Socket(port2)) { // RA2: as above
				return behavior;
			}
			PacketTransport->Start_Listening(); // RA2: as above

			timeout = 240; // RA2: as above
			mangled_port = 0; // RA2: as above
			packet_id++; // RA2: as above

			while (timeout() && mangled_port == 0) { // RA2: as above
				Send_Probe(&manglers[0], port1, packet_id, false); // RA2: as above
				mangled_port = Find_Mangled_Port(packet_id, 0); // RA2: as above
			}

			if (mangled_port == 0) { // RA2: as above
				((UDPInterfaceClass *)PacketTransport)->Close_Port_Socket(port1); // RA2: as above
				((UDPInterfaceClass *)PacketTransport)->Close_Port_Socket(port2); // RA2: as above
				return behavior;
			}

			IPXAddressClass addr = manglers[0]; // RA2: as above
			addr.Set_Port(addr.Get_Port() + 1); // RA2: as above
			Send_Probe(&addr, port1, packet_id, false); // RA2: as above
			Send_Probe(&addr, port1, packet_id, false); // RA2: as above
			Send_Probe(&addr, port1, packet_id, false); // RA2: as above

			packet_id++; // RA2: as above
			unsigned short new_mangled_port = 0; // RA2: as above
			timeout = 240; // RA2: as above
			while (timeout() && new_mangled_port == 0) { // RA2: as above
				Send_Probe(&manglers[0], port2, packet_id, false); // RA2: as above
				new_mangled_port = Find_Mangled_Port(packet_id, 0); // RA2: as above
			}

			if (new_mangled_port != 0) { // RA2: as above
				if (new_mangled_port != mangled_port + Delta) { // RA2: as above
					DebugString("NAT uses different source ports for different destination ports\n"); // RA2: as above
					behavior |= 0x40; // RA2: as above
				} else {
					DebugString("NAT uses the same source port for different destination ports\n"); // RA2: as above
				}
			}
		} else {
			behavior |= 0x40; // RA2: as above
		}
	}

	DebugString("Testing for Netgear bug\n"); // RA2: as above
	if (RouterType == 2 || Session.SendDelay) { // RA2: as above
		behavior |= 8; // RA2: as above
		DebugString("Netgear bug specified by command line or SendDelay flag\n"); // RA2: as above
	}
	return behavior;
}

/// <summary>
/// Sorts the mangled ports (with the ports they were sent from) and looks for the pattern the router allocates them
/// in; returns the delta between two allocations, or 0 when there is none.
/// </summary>
int FirewallHelperClass::Analyse_Ports(int num_ports, unsigned short * original_ports, unsigned short * mangled_ports, bool & relative_delta, bool & looks_good) // RA2: 0x005BAC50
{
	DebugString("Looking for port allocation pattern in original_ports %d, %d, %d, %d\n", original_ports[0], original_ports[1], original_ports[2], original_ports[3]); // RA2: as above

	for (int x = 0; x < num_ports; x++) { // RA2: as above
		for (int y = 0; y < num_ports - 1; y++) { // RA2: as above
			if (mangled_ports[y] > mangled_ports[y + 1]) { // RA2: as above
				int temp = mangled_ports[y]; // RA2: as above
				mangled_ports[y] = mangled_ports[y + 1]; // RA2: as above
				mangled_ports[y + 1] = temp; // RA2: as above
				temp = original_ports[y]; // RA2: as above
				original_ports[y] = original_ports[y + 1]; // RA2: as above
				original_ports[y + 1] = temp; // RA2: as above
			}
		}
	}

	if (mangled_ports[1] - mangled_ports[0] == 1) { // RA2: as above
		if (mangled_ports[2] - mangled_ports[1] == 1) { // RA2: as above
			if (mangled_ports[3] - mangled_ports[2] == 1) { // RA2: as above
				DebugString("Incremental port allocation detected\n"); // RA2: as above
				relative_delta = false; // RA2: as above
				looks_good = true; // RA2: as above
				return 1;
			}
		}
	}

	if (mangled_ports[1] - mangled_ports[0] == 2) { // RA2: as above
		if (mangled_ports[2] - mangled_ports[1] == 2) { // RA2: as above
			if (mangled_ports[3] - mangled_ports[2] == 2) { // RA2: as above
				DebugString("Semi-incremental port allocation detected\n"); // RA2: as above
				relative_delta = false; // RA2: as above
				looks_good = true; // RA2: as above
				return 2;
			}
		}
	}

	int diff1 = mangled_ports[1] - mangled_ports[0]; // RA2: as above
	int diff2 = mangled_ports[2] - mangled_ports[1]; // RA2: as above
	int diff3 = mangled_ports[3] - mangled_ports[2]; // RA2: as above

	if (diff1 == diff2 && diff2 == diff3) { // RA2: as above
		DebugString("Looks good for absolute allocation sequence delta of %d\n", diff1); // RA2: as above
		relative_delta = false; // RA2: as above
		looks_good = true; // RA2: as above
		return diff1;
	}

	if (diff1 == diff2) { // RA2: as above
		DebugString("Probable absolute allocation sequence delta of %d\n", diff1); // RA2: as above
		relative_delta = false; // RA2: as above
		looks_good = false; // RA2: as above
		return diff1;
	}

	if (diff2 == diff3) { // RA2: as above
		DebugString("Probable absolute allocation sequence delta of %d\n", diff2); // RA2: as above
		relative_delta = false; // RA2: as above
		looks_good = false; // RA2: as above
		return diff2;
	}

	for (int i = 0; i < num_ports; i++) { // RA2: as above
		mangled_ports[i] -= original_ports[i]; // RA2: as above
	}

	diff1 = mangled_ports[1] - mangled_ports[0]; // RA2: as above
	diff2 = mangled_ports[2] - mangled_ports[1]; // RA2: as above
	diff3 = mangled_ports[3] - mangled_ports[2]; // RA2: as above

	if (diff1 == diff2 && diff2 == diff3) { // RA2: as above
		DebugString("Looks good for a relative port range delta of %d\n", diff1); // RA2: as above
		relative_delta = true; // RA2: as above
		looks_good = true; // RA2: as above
		return diff1;
	}

	if (diff1 == diff2 || diff1 == diff3) { // RA2: as above
		DebugString("Detected probable broken relative port range delta of %d\n", diff1); // RA2: as above
		relative_delta = true; // RA2: as above
		looks_good = false; // RA2: as above
		return diff1;
	}

	if (diff2 == diff3) { // RA2: as above
		DebugString("Detected probable broken relative port range delta of %d\n", diff2); // RA2: as above
		relative_delta = true; // RA2: as above
		looks_good = false; // RA2: as above
		return diff2;
	}

	looks_good = false; // RA2: as above
	relative_delta = false; // RA2: as above
	return 0;
}

/// <summary>
/// Rates a detection result by how awkward the connection it describes is (the higher, the worse).
/// </summary>
int FirewallHelperClass::Rank_A(int flags) // RA2: 0x005BAEE0
{
	int rank = 0; // RA2: as above
	if (flags & 1) { // RA2: as above
		rank = 1; // RA2: as above
	}
	if (flags & 2) { // RA2: as above
		rank += 2; // RA2: as above
	}
	if (flags & 4) { // RA2: as above
		rank += 3; // RA2: as above
	}
	if (flags & 8) { // RA2: as above
		rank += 10; // RA2: as above
	}
	if (flags & 0x10) { // RA2: as above
		rank++; // RA2: as above
	}
	if (flags & 0x20) { // RA2: as above
		rank += 2; // RA2: as above
	}
	return rank;
}

/// <summary>
/// Rates a detection result by the number of connection attempts it takes (the higher, the more).
/// </summary>
int FirewallHelperClass::Rank_B(int flags) // RA2: 0x005BAF20
{
	int rank = 2; // RA2: as above
	if (flags & 1) { // RA2: as above
		rank = 3; // RA2: as above
	}
	if (flags & 2) { // RA2: as above
		rank++; // RA2: as above
	}
	if (flags & 4) { // RA2: as above
		rank++; // RA2: as above
	}
	if (flags & 0x20) { // RA2: as above
		rank += 5; // RA2: as above
	}
	return rank;
}

/// <summary>
/// Works out the port a peer will be seen on for a connection from the given source port, from the detection result
/// and a probe to one mangler server.
/// </summary>
unsigned short FirewallHelperClass::Get_Next_Mangled_Source_Port(unsigned short source_port) // RA2: 0x005BAF50
{
	static unsigned long _packet_id = 0x7F100000; // RA2: 0x007E63D4, the packet ID of the next probe
	CDTimerClass<SystemTimerClass> timeout; // RA2: as above
	int return_port; // RA2: as above

	int fw = Detected; // RA2: as above
	if (fw == 0) { // RA2: as above
		return source_port;
	}
	if (fw & 1) { // RA2: as above
		return source_port;
	}

	unsigned short mangler_port = 4321; // RA2: as above
	char mangler_name[256]; // RA2: as above
	strcpy(mangler_name, "mangler1.westwood.com"); // RA2: as above
	unsigned char maddress[4]; // RA2: as above

	if (ManglerCount > 0) { // RA2: as above
		strcpy(mangler_name, ManglerAddresses[ManglerIndex]); // RA2: as above
		mangler_port = ManglerPorts[ManglerIndex]; // RA2: as above
		DebugString("Using mangler from servserv\n"); // RA2: as above
	}

	DebugString("Firewall helper - About to call gethostbyname for %s\n", mangler_name); // RA2: as above
	hostent * host_info = gethostbyname(mangler_name); // RA2: as above
	if (!host_info) { // RA2: as above
		DebugString("gethostbyname failed! Error code %d\n", WSAGetLastError()); // RA2: as above
		return source_port;
	}
	memcpy(maddress, &host_info->h_addr_list[0][0], 4); // RA2: as above
	DebugString("Mangler address is %d.%d.%d.%d ; %d\n", maddress[0], maddress[1], maddress[2], maddress[3], (int)mangler_port); // RA2: as above
	IPXAddressClass mangler_address; // RA2: as above
	mangler_address.Set_Node(maddress); // RA2: as above
	mangler_address.Set_Port(mangler_port); // RA2: as above
	DebugString("Mangler address is %s\n", mangler_address.As_String(NULL)); // RA2: as above
	unsigned short port = 0; // RA2: as above
	unsigned short open_port = 0; // RA2: as above
	DebugString("fw = %08x\n", fw); // RA2: as above

	int use_temp = fw & 4; // RA2: as above
	if (use_temp) { // RA2: as above
		port = Next_Port(0); // RA2: as above
		open_port = port; // RA2: as above
		if (!((UDPInterfaceClass *)PacketTransport)->Open_Port_Socket(port)) { // RA2: as above
			return source_port;
		}
		PacketTransport->Start_Listening(); // RA2: as above
	}

	DebugString("FirewallHelper - Getting mangling for port %d\n", (int)port); // RA2: as above

	timeout = 180; // RA2: as above
	unsigned short mangled_port = 0; // RA2: as above
	while (timeout() && mangled_port == 0) { // RA2: as above
		Send_Probe(&mangler_address, port, _packet_id, false); // RA2: as above
		mangled_port = Find_Mangled_Port(_packet_id, 30); // RA2: as above
		DebugString("FirewallHelper - Mangled port = %d\n", (int)mangled_port); // RA2: as above
	}

	_packet_id++; // RA2: as above

	if (open_port) { // RA2: as above
		((UDPInterfaceClass *)PacketTransport)->Close_Port_Socket(open_port); // RA2: as above
	}

	if (mangled_port == 0) { // RA2: as above
		DebugString("Couldn't find mangled port, returning source port\n"); // RA2: as above
		return source_port;
	}

	if (!use_temp) { // RA2: as above
		DebugString("FirewallHelper - Returning next mangled port as %d\n", (int)mangled_port); // RA2: as above
		return mangled_port;
	}

	if (fw & 0x10) { // RA2: as above
		return_port = (int)mangled_port + Delta; // RA2: as above
	} else if (Delta == 100) { // RA2: as above
		return_port = mangled_port - port; // RA2: as above
		return_port += Delta; // RA2: as above
		return_port += source_port; // RA2: as above
	} else {
		return_port = mangled_port / Delta; // RA2: as above
		return_port = (return_port + 1) * Delta; // RA2: as above
		return_port += source_port % Delta; // RA2: as above
	}

	if (return_port > 65535) { // RA2: as above
		return_port -= 65535; // RA2: as above
	}
	if (return_port < 1024) { // RA2: as above
		return_port += 1024; // RA2: as above
	}

	DebugString("FirewallHelper - Returning next mangled port as %d\n", return_port); // RA2: as above
	return (unsigned short)return_port;
}
