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

struct GlobalPacketType;
class IPXAddressClass;

bool Net2Callback(void);
void Net2DisplayUsers(void);
bool Net2Init_Network(void);
void Net2Shutdown_Network(void);
void Net2EncodeGameopt(char *out);
void Net2SetAccept(char const *who, int status); // RA2: the name is const
int Net2GetAccept(char const *who); // RA2: the name is const
bool Decrypt_Serial(char *buffer); // RA2: 0x005BCE90, reads the registry itself
bool Net2ReadyToGo(int load_game);
bool Process_Global_Packet(GlobalPacketType *packet, IPXAddressClass *address);
void Net2DisplayGameList(void);
void Net2ServiceGameList(void);
