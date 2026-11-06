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

// Adapted from TibSun ipxconn.cpp to the recovered RA2 Open/Close_Socket interface.
#include "shared/ipx_conn.hpp"
unsigned short IPXConnClass::Socket;

int IPXConnClass::Start_Listening(void)
{
	/*
	**	Open the socket.
	*/
	Open_Socket(Socket);
	if (!SocketOpen)
		return(false);

	/*
	**	start listening on the socket.
	*/
	if ( PacketTransport->Start_Listening () ) {
		Listening =1;
		return(true);
	} else {
		Close_Socket();
		SocketOpen = false;
		return(false);
	}
}
