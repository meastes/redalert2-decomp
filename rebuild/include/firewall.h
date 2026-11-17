/*******************************************************************************
 * SPDX-License-Identifier: GPL-3.0-or-later
 * Copyright 2026 RedAlert2 contributors
 ******************************************************************************/

#pragma once

class IPXAddressClass;

/*
**	RA2: the firewall helper (the global at 0xA71B18): detects how the router rewrites source ports and works out
**	which port a peer behind it will be seen on.
*/
class FirewallHelperClass
{
	public:
		FirewallHelperClass(void); // RA2: 0x005B9E60
		void Clear(void); // RA2: 0x005B9E70, forgets the detection result
		void Save_Detected(void); // RA2: 0x005B9E80, detects when nothing was detected yet and saves a reliable result
		unsigned short Next_Port(int step); // RA2: 0x005B9F10, the next port of the pool
		bool Send_Probe(IPXAddressClass * address, unsigned short port, int packet_id, bool flag); // RA2: 0x005B9F90, a probe to a mangler server
		unsigned short Find_Mangled_Port(int packet_id, int time); // RA2: 0x005BA0D0, the port the mangler saw a probe come from
		void Write_Settings(void); // RA2: 0x005BA210, writes the detection result to RA2.INI
		void Read_Settings(void); // RA2: 0x005BA280, reads the detection result from RA2.INI
		int Detect(bool * detected); // RA2: 0x005BA2C0, probes the mangler servers; the result is a set of flags
		int Analyse_Ports(int num_ports, unsigned short * original_ports, unsigned short * mangled_ports, bool & relative_delta, bool & looks_good); // RA2: 0x005BAC50, the pattern the router allocates ports in
		int Rank_A(int flags); // RA2: 0x005BAEE0, how awkward the connection a result describes is
		int Rank_B(int flags); // RA2: 0x005BAF20, how many connection attempts it takes
		unsigned short Get_Next_Mangled_Source_Port(unsigned short source_port); // RA2: 0x005BAF50, the port a peer will be seen on

		int Detected; // RA2: +0x00
		int Settings; // RA2: +0x04
		int Delta; // RA2: +0x08
		int ReadDelta; // RA2: +0x0C
		int Port; // RA2: +0x10, the port pool (0xA71B28)
};

// Declarations moved from the units that used them (one declaration per name).
extern FirewallHelperClass FirewallHelper; // RA2: 0x00A71B18, as session.cpp
