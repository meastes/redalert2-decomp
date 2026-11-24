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
	Detected = 0;
	Settings = 0;
	Delta = 0;
	ReadDelta = 0;
}

/// <summary>
/// Forgets what was detected, so the next Save_Detected runs the detection again.
/// </summary>
void FirewallHelperClass::Clear(void) // RA2: 0x005B9E70
{
	Detected = 0;
}

/// <summary>
/// Runs the detection when it has not run yet, and writes the result to RA2.INI when it is reliable.
/// </summary>
void FirewallHelperClass::Save_Detected(void) // RA2: 0x005B9E80
{
	bool detected = false;
	if (Detected == 0) {
		Detected = Detect(&detected);
		if (detected) {
			RawFileClass file("RA2.INI");
			ConfigINI.Put_Int("MultiPlayer", "FirewallSettings", Detected, 0);
			ConfigINI.Put_Int("MultiPlayer", "FirewallDelta", Delta, 0);
			ConfigINI.Save(file, false);
		}
	}
}

/// <summary>
/// Hands out the next port of the pool, stepping by the given amount; over the internet a port that cannot be opened is skipped.
/// </summary>
unsigned short FirewallHelperClass::Next_Port(int step) // RA2: 0x005B9F10
{
	unsigned short port;

	if (step == 0) {
		step = 1;
	}
	int tries = 255;
	do {
		Port += step;
		port = (unsigned short)Port;
		if (Port > 0xFFFF) {
			Port = 0x800;
		}
		if (PacketTransport && Session.Type == GAME_INTERNET) {
			if (!((UDPInterfaceClass *)PacketTransport)->Open_Port_Socket(port)) {
				continue;
			}
			((UDPInterfaceClass *)PacketTransport)->Close_Port_Socket(port);
		}
		return port;
	} while (tries-- != 0);
	return port;
}

/// <summary>
/// Sends one probe from a port to a mangler server and waits until it has gone out.
/// </summary>
bool FirewallHelperClass::Send_Probe(IPXAddressClass * address, unsigned short port, int packet_id, bool flag) // RA2: 0x005B9F90
{
	GlobalPacketType packet;

	packet.Command = NET_MANGLER_REQUEST;
	strcpy(packet.Name, "WW");
	((UDPInterfaceClass *)PacketTransport)->Select_Port_Socket(port);
	packet.ManglerData.OriginalPortNumber = port;
	packet.ManglerData.BlitzMe = (flag == true);
	int pending = Ipx.Global_Num_Send();
	DebugString("Sending from port %d to %s\n", (unsigned short)port, address->As_String(NULL));
	Ipx.Send_Global_Message(&packet, 0x38, 0, address, address->Get_Port(), packet_id);
	CDTimerClass<SystemTimerClass> timer(180);
	while (Ipx.Global_Num_Send() > pending || PacketTransport->OutBuffers.Count()) {
		if (timer() == 0) {
			break;
		}
		Ipx.Service();
		Call_Back();
	}
	((UDPInterfaceClass *)PacketTransport)->Select_Port_Socket(0);
	return true;
}

/// <summary>
/// Waits for the mangler's answer to a probe and returns the port it saw the probe come from, or 0 when none came in time.
/// </summary>
unsigned short FirewallHelperClass::Find_Mangled_Port(int packet_id, int time) // RA2: 0x005BA0D0
{
	int peek_packet = 0;
	IPXAddressClass address;
	GlobalPacketType packet;
	unsigned short product_id;
	int packet_len;
	unsigned long id;

	CDTimerClass<SystemTimerClass> timeout(60);
	if (time) {
		timeout = CDTimerClass<SystemTimerClass>(time);
	}
	while (timeout() != 0) {
		Ipx.Service();
		Call_Back();
		if (peek_packet >= Ipx.Global_Num_Receive()) {
			peek_packet = 0;
		}
		if (Ipx.Peek_Global_Message(peek_packet, &packet, &packet_len, &address, &product_id, &id)) {
			if (product_id == 0xAA03 && packet.Command == NET_MANGLER_RESPONSE) {
				int original_port = packet.ManglerData.OriginalPortNumber;
				if (packet_id == (int)id) {
					unsigned short mangled_port = packet.ManglerData.MangledPortNumber;
					DebugString("Mangler is seeing packets from port %d as coming from port %d\n", original_port, (unsigned int)mangled_port);
					Ipx.Mark_Global_Message_Read(peek_packet);
					return mangled_port;
				}
			}
		}
		peek_packet++;
	}
	return 0;
}

/// <summary>
/// Writes the detection result to RA2.INI.
/// </summary>
void FirewallHelperClass::Write_Settings(void) // RA2: 0x005BA210
{
	RawFileClass file("RA2.INI");
	ConfigINI.Put_Int("MultiPlayer", "FirewallSettings", Detected, 0);
	ConfigINI.Put_Int("MultiPlayer", "FirewallDelta", Delta, 0);
	ConfigINI.Save(file, false);
}

/// <summary>
/// Reads the saved detection result (what Detect found last time) from RA2.INI's [MultiPlayer] section.
/// </summary>
void FirewallHelperClass::Read_Settings(void) // RA2: 0x005BA280
{
	Settings = ConfigINI.Get_Int("MultiPlayer", "FirewallSettings", 0);
	ReadDelta = ConfigINI.Get_Int("MultiPlayer", "FirewallDelta", 1);
}

/// <summary>
/// Probes the mangler servers to find out how the router rewrites source ports. The result is a set of flags (1 no
/// translation, 2 the same port for every destination, 4 a different one, 8 the Netgear bug, 0x10 an absolute and
/// 0x20 a relative port delta, 0x40 a port that depends on the destination port); detected is set when it is reliable.
/// </summary>
int FirewallHelperClass::Detect(bool * detected) // RA2: 0x005BA2C0
{
	unsigned short mangler_port = 4321;
	unsigned long packet_id = 0x7F000000;

	static char _manglername[5][32] = { // RA2: 0x007E6334, the mangler servers tried when the server list gave none
		"mangler1.westwood.com",
		"mangler2.westwood.com",
		"mangler3.westwood.com",
		"mangler.westwood.com",
		""
	};

	*detected = false;
	unsigned char mangler_addresses[4][4];
	int num_mangler_addresses = 0;
	int behavior;
	IPXAddressClass manglers[4];
	CDTimerClass<SystemTimerClass> timeout;

	unsigned short source_ports[4];
	unsigned short mangled_ports[4];
	int delta;
	bool looks_good;
	int i;

	if (Session.PortNumberOverride) {
		behavior = 1;
		DebugString("Source port %d specified by user\n", Session.PortNumberOverride);
		if (Session.SendDelay) {
			behavior |= 8;
			DebugString("Netgear bug specified by command line or SendDelay flag\n");
		}
		return behavior;
	}

	timeout = 300;
	DebugString("About to call gethostbyname for the mangler address\n");
	int namenum = 0;

	do {
		char * mangler_name_ptr = &_manglername[namenum][0];
		if (ManglerCount > namenum) {
			mangler_name_ptr = ManglerAddresses[namenum];
			mangler_port = ManglerPorts[namenum];
			DebugString("Using mangler from servserv\n");
		}
		namenum++;

		if (strlen(mangler_name_ptr) == 0) {
			break;
		}

		char temp_name[256];
		strcpy(temp_name, mangler_name_ptr);
		hostent * host_info = gethostbyname(temp_name);

		if (!host_info) {
			DebugString("gethostbyname failed! Error code %d\n", WSAGetLastError());
			break;
		}

		bool found = false;
		for (i = 0; i < num_mangler_addresses; i++) {
			if (memcmp(mangler_addresses[i], &host_info->h_addr_list[0][0], 4) == 0) {
				found = true;
				break;
			}
		}
		if (!found) {
			int m = num_mangler_addresses++;
			memcpy(&mangler_addresses[m][0], &host_info->h_addr_list[0][0], 4);
			DebugString("Found mangler address at %d.%d.%d.%d\n", mangler_addresses[m][0], mangler_addresses[m][1], mangler_addresses[m][2], mangler_addresses[m][3]);
		}
	} while (num_mangler_addresses < 4 && timeout());

	if (num_mangler_addresses < 3) {
		return 1;
	}

	for (i = 0; i < num_mangler_addresses; i++) {
		NetNodeType addr;
		memcpy(addr, &mangler_addresses[i][0], 4);
		manglers[i].Set_Node(addr);
		manglers[i].Set_Port(mangler_port);
	}

	unsigned short port = Next_Port(0);
	if (!((UDPInterfaceClass *)PacketTransport)->Open_Port_Socket(port)) {
		return 1;
	}
	PacketTransport->Start_Listening();

	timeout = 360;
	unsigned short mangled_port = 0;
	while (timeout() && mangled_port == 0) {
		Send_Probe(&manglers[0], port, packet_id, false);
		mangled_port = Find_Mangled_Port(packet_id, 0);
	}

	if (mangled_port == 0 || mangled_port == port) {
		((UDPInterfaceClass *)PacketTransport)->Close_Port_Socket(port);
		if (mangled_port == port) {
			Delta = 0;
			*detected = true;
		}
		return 1;
	}

	timeout = 360;
	unsigned short second_ip_mangled_port = 0;
	while (timeout() && second_ip_mangled_port == 0) {
		Send_Probe(&manglers[1], port, packet_id + 1, false);
		second_ip_mangled_port = Find_Mangled_Port(packet_id + 1, 0);
	}

	((UDPInterfaceClass *)PacketTransport)->Close_Port_Socket(port);

	if (second_ip_mangled_port == 0 || second_ip_mangled_port == port) {
		return 1;
	}

	if (mangled_port == second_ip_mangled_port) {
		behavior = 2;
	} else {
		behavior = 4;
	}

	int try_again;
	for (try_again = 0; try_again < 3; try_again++) {
		memset(source_ports, 0, sizeof(source_ports));
		memset(mangled_ports, 0, sizeof(source_ports));

		for (i = 0; i < 4; i++) {
			source_ports[i] = Next_Port(i);
			if (!((UDPInterfaceClass *)PacketTransport)->Open_Port_Socket(source_ports[i])) {
				for (int j = 0; j < i; j++) {
					if (source_ports[j]) {
						((UDPInterfaceClass *)PacketTransport)->Close_Port_Socket(source_ports[j]);
					}
				}
				return behavior;
			}
		}
		PacketTransport->Start_Listening();

		int num_responses = 0;
		packet_id = packet_id + 10;
		timeout = 720;

		while (timeout() && num_responses < 4) {
			for (i = 0; i < 4; i++) {
				if (mangled_ports[i] == 0) {
					Send_Probe(&manglers[0], source_ports[i], packet_id + i, false);
					mangled_ports[i] = Find_Mangled_Port(packet_id + i, 0);
					if (mangled_ports[i] != 0) {
						num_responses++;
					}
				}
			}
		}

		for (int j = 0; j < i; j++) {
			if (source_ports[j]) {
				((UDPInterfaceClass *)PacketTransport)->Close_Port_Socket(source_ports[j]);
			}
		}

		if (num_responses < 4) {
			if (ReadDelta != 0 && Settings > 1) {
				Delta = ReadDelta;
			}
			return behavior;
		}

		bool relative_delta = false;
		looks_good = false;
		delta = Analyse_Ports(num_responses, source_ports, mangled_ports, relative_delta, looks_good);

		if (delta) {
			unsigned long addbehavior = 0;
			if (relative_delta) {
				addbehavior = 0x20;
			} else {
				addbehavior = 0x10;
			}
			addbehavior |= behavior;
			behavior = addbehavior;
			Delta = delta;
			if (looks_good) {
				*detected = true;
			}
			break;
		} else {
			if (ReadDelta != 0 && Settings > 1) {
				Delta = ReadDelta;
			}
		}
	}

	if ((behavior & 4) != 0) {
		if ((behavior & 0x10) != 0) {
			DebugString("Testing to see if the NAT mangles differently per destination port at the same IP\n");

			unsigned short port1 = Next_Port(0);
			if (!((UDPInterfaceClass *)PacketTransport)->Open_Port_Socket(port1)) {
				return behavior;
			}

			unsigned short port2 = Next_Port(0);
			if (!((UDPInterfaceClass *)PacketTransport)->Open_Port_Socket(port2)) {
				return behavior;
			}
			PacketTransport->Start_Listening();

			timeout = 240;
			mangled_port = 0;
			packet_id++;

			while (timeout() && mangled_port == 0) {
				Send_Probe(&manglers[0], port1, packet_id, false);
				mangled_port = Find_Mangled_Port(packet_id, 0);
			}

			if (mangled_port == 0) {
				((UDPInterfaceClass *)PacketTransport)->Close_Port_Socket(port1);
				((UDPInterfaceClass *)PacketTransport)->Close_Port_Socket(port2);
				return behavior;
			}

			IPXAddressClass addr = manglers[0];
			addr.Set_Port(addr.Get_Port() + 1);
			Send_Probe(&addr, port1, packet_id, false);
			Send_Probe(&addr, port1, packet_id, false);
			Send_Probe(&addr, port1, packet_id, false);

			packet_id++;
			unsigned short new_mangled_port = 0;
			timeout = 240;
			while (timeout() && new_mangled_port == 0) {
				Send_Probe(&manglers[0], port2, packet_id, false);
				new_mangled_port = Find_Mangled_Port(packet_id, 0);
			}

			if (new_mangled_port != 0) {
				if (new_mangled_port != mangled_port + Delta) {
					DebugString("NAT uses different source ports for different destination ports\n");
					behavior |= 0x40;
				} else {
					DebugString("NAT uses the same source port for different destination ports\n");
				}
			}
		} else {
			behavior |= 0x40;
		}
	}

	DebugString("Testing for Netgear bug\n");
	if (RouterType == 2 || Session.SendDelay) {
		behavior |= 8;
		DebugString("Netgear bug specified by command line or SendDelay flag\n");
	}
	return behavior;
}

/// <summary>
/// Sorts the mangled ports (with the ports they were sent from) and looks for the pattern the router allocates them
/// in; returns the delta between two allocations, or 0 when there is none.
/// </summary>
int FirewallHelperClass::Analyse_Ports(int num_ports, unsigned short * original_ports, unsigned short * mangled_ports, bool & relative_delta, bool & looks_good) // RA2: 0x005BAC50
{
	DebugString("Looking for port allocation pattern in original_ports %d, %d, %d, %d\n", original_ports[0], original_ports[1], original_ports[2], original_ports[3]);

	for (int x = 0; x < num_ports; x++) {
		for (int y = 0; y < num_ports - 1; y++) {
			if (mangled_ports[y] > mangled_ports[y + 1]) {
				int temp = mangled_ports[y];
				mangled_ports[y] = mangled_ports[y + 1];
				mangled_ports[y + 1] = temp;
				temp = original_ports[y];
				original_ports[y] = original_ports[y + 1];
				original_ports[y + 1] = temp;
			}
		}
	}

	if (mangled_ports[1] - mangled_ports[0] == 1) {
		if (mangled_ports[2] - mangled_ports[1] == 1) {
			if (mangled_ports[3] - mangled_ports[2] == 1) {
				DebugString("Incremental port allocation detected\n");
				relative_delta = false;
				looks_good = true;
				return 1;
			}
		}
	}

	if (mangled_ports[1] - mangled_ports[0] == 2) {
		if (mangled_ports[2] - mangled_ports[1] == 2) {
			if (mangled_ports[3] - mangled_ports[2] == 2) {
				DebugString("Semi-incremental port allocation detected\n");
				relative_delta = false;
				looks_good = true;
				return 2;
			}
		}
	}

	int diff1 = mangled_ports[1] - mangled_ports[0];
	int diff2 = mangled_ports[2] - mangled_ports[1];
	int diff3 = mangled_ports[3] - mangled_ports[2];

	if (diff1 == diff2 && diff2 == diff3) {
		DebugString("Looks good for absolute allocation sequence delta of %d\n", diff1);
		relative_delta = false;
		looks_good = true;
		return diff1;
	}

	if (diff1 == diff2) {
		DebugString("Probable absolute allocation sequence delta of %d\n", diff1);
		relative_delta = false;
		looks_good = false;
		return diff1;
	}

	if (diff2 == diff3) {
		DebugString("Probable absolute allocation sequence delta of %d\n", diff2);
		relative_delta = false;
		looks_good = false;
		return diff2;
	}

	for (int i = 0; i < num_ports; i++) {
		mangled_ports[i] -= original_ports[i];
	}

	diff1 = mangled_ports[1] - mangled_ports[0];
	diff2 = mangled_ports[2] - mangled_ports[1];
	diff3 = mangled_ports[3] - mangled_ports[2];

	if (diff1 == diff2 && diff2 == diff3) {
		DebugString("Looks good for a relative port range delta of %d\n", diff1);
		relative_delta = true;
		looks_good = true;
		return diff1;
	}

	if (diff1 == diff2 || diff1 == diff3) {
		DebugString("Detected probable broken relative port range delta of %d\n", diff1);
		relative_delta = true;
		looks_good = false;
		return diff1;
	}

	if (diff2 == diff3) {
		DebugString("Detected probable broken relative port range delta of %d\n", diff2);
		relative_delta = true;
		looks_good = false;
		return diff2;
	}

	looks_good = false;
	relative_delta = false;
	return 0;
}

/// <summary>
/// Rates a detection result by how awkward the connection it describes is (the higher, the worse).
/// </summary>
int FirewallHelperClass::Rank_A(int flags) // RA2: 0x005BAEE0
{
	int rank = 0;
	if (flags & 1) {
		rank = 1;
	}
	if (flags & 2) {
		rank += 2;
	}
	if (flags & 4) {
		rank += 3;
	}
	if (flags & 8) {
		rank += 10;
	}
	if (flags & 0x10) {
		rank++;
	}
	if (flags & 0x20) {
		rank += 2;
	}
	return rank;
}

/// <summary>
/// Rates a detection result by the number of connection attempts it takes (the higher, the more).
/// </summary>
int FirewallHelperClass::Rank_B(int flags) // RA2: 0x005BAF20
{
	int rank = 2;
	if (flags & 1) {
		rank = 3;
	}
	if (flags & 2) {
		rank++;
	}
	if (flags & 4) {
		rank++;
	}
	if (flags & 0x20) {
		rank += 5;
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
	CDTimerClass<SystemTimerClass> timeout;
	int return_port;

	int fw = Detected;
	if (fw == 0) {
		return source_port;
	}
	if (fw & 1) {
		return source_port;
	}

	unsigned short mangler_port = 4321;
	char mangler_name[256];
	strcpy(mangler_name, "mangler1.westwood.com");
	unsigned char maddress[4];

	if (ManglerCount > 0) {
		strcpy(mangler_name, ManglerAddresses[ManglerIndex]);
		mangler_port = ManglerPorts[ManglerIndex];
		DebugString("Using mangler from servserv\n");
	}

	DebugString("Firewall helper - About to call gethostbyname for %s\n", mangler_name);
	hostent * host_info = gethostbyname(mangler_name);
	if (!host_info) {
		DebugString("gethostbyname failed! Error code %d\n", WSAGetLastError());
		return source_port;
	}
	memcpy(maddress, &host_info->h_addr_list[0][0], 4);
	DebugString("Mangler address is %d.%d.%d.%d ; %d\n", maddress[0], maddress[1], maddress[2], maddress[3], (int)mangler_port);
	IPXAddressClass mangler_address;
	mangler_address.Set_Node(maddress);
	mangler_address.Set_Port(mangler_port);
	DebugString("Mangler address is %s\n", mangler_address.As_String(NULL));
	unsigned short port = 0;
	unsigned short open_port = 0;
	DebugString("fw = %08x\n", fw);

	int use_temp = fw & 4;
	if (use_temp) {
		port = Next_Port(0);
		open_port = port;
		if (!((UDPInterfaceClass *)PacketTransport)->Open_Port_Socket(port)) {
			return source_port;
		}
		PacketTransport->Start_Listening();
	}

	DebugString("FirewallHelper - Getting mangling for port %d\n", (int)port);

	timeout = 180;
	unsigned short mangled_port = 0;
	while (timeout() && mangled_port == 0) {
		Send_Probe(&mangler_address, port, _packet_id, false);
		mangled_port = Find_Mangled_Port(_packet_id, 30);
		DebugString("FirewallHelper - Mangled port = %d\n", (int)mangled_port);
	}

	_packet_id++;

	if (open_port) {
		((UDPInterfaceClass *)PacketTransport)->Close_Port_Socket(open_port);
	}

	if (mangled_port == 0) {
		DebugString("Couldn't find mangled port, returning source port\n");
		return source_port;
	}

	if (!use_temp) {
		DebugString("FirewallHelper - Returning next mangled port as %d\n", (int)mangled_port);
		return mangled_port;
	}

	if (fw & 0x10) {
		return_port = (int)mangled_port + Delta;
	} else if (Delta == 100) {
		return_port = mangled_port - port;
		return_port += Delta;
		return_port += source_port;
	} else {
		return_port = mangled_port / Delta;
		return_port = (return_port + 1) * Delta;
		return_port += source_port % Delta;
	}

	if (return_port > 65535) {
		return_port -= 65535;
	}
	if (return_port < 1024) {
		return_port += 1024;
	}

	DebugString("FirewallHelper - Returning next mangled port as %d\n", return_port);
	return (unsigned short)return_port;
}
