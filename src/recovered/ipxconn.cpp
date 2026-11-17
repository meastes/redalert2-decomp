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
// Function comment blocks from mischa85/RedAlert2 392e904 src/ipxconn.cpp (matching RA2 1.006 decomp), placed above our
// own notes; code unchanged.

// ipxconn.cpp: the original translation unit re-filed as one source (Route A): the accepted
// bodies of its canonical compile group in address order, under the unit's own header
// state (its switches and includes in their original order). The module map
// manifests/original_target/modules/ipxconn.json carries the addresses and the rows that
// stay in their own files.


#include "shared/shape_file.hpp"
#include "shared/unknown_geometry.hpp"
#include "shared/ipx_conn.hpp"
#include "shared/ipx_manager.hpp"
#include "shared/owner_talk.hpp"
#include "shared/unknown_player_entry.hpp"
#include "shared/session_state.hpp"


// Header-static dynamic initializers of the unit at 0x00523980: the shape_file.hpp empty rect and the
// shared/unknown_geometry.hpp constants (0x00523980-0x00523B61); the other
// drafts of this plan are emitted by these includes.


// Dynamic initializer (0x005239A0, _$E) of diagonal from the header(s) included
// by HeaderStatics_empty_rect_init_00523980.cpp; no source of its own.


// Dynamic initializer (0x005239D0, _$E) of b from the header(s) included
// by HeaderStatics_empty_rect_init_00523980.cpp; no source of its own.


// Dynamic initializer (0x005239F0, _$E) of c from the header(s) included
// by HeaderStatics_empty_rect_init_00523980.cpp; no source of its own.


// Dynamic initializer (0x00523A10, _$E) of angle45 from the header(s) included
// by HeaderStatics_empty_rect_init_00523980.cpp; no source of its own.


// Dynamic initializer (0x00523A30, _$E) of angle60 from the header(s) included
// by HeaderStatics_empty_rect_init_00523980.cpp; no source of its own.


// Dynamic initializer (0x00523A50, _$E) of angle90 from the header(s) included
// by HeaderStatics_empty_rect_init_00523980.cpp; no source of its own.


// Dynamic initializer (0x00523A70, _$E) of g from the header(s) included
// by HeaderStatics_empty_rect_init_00523980.cpp; no source of its own.


// Dynamic initializer (0x00523AB0, _$E) of h from the header(s) included
// by HeaderStatics_empty_rect_init_00523980.cpp; no source of its own.


// Dynamic initializer (0x00523AE0, _$E) of i from the header(s) included
// by HeaderStatics_empty_rect_init_00523980.cpp; no source of its own.


// Dynamic initializer (0x00523B10, _$E) of j from the header(s) included
// by HeaderStatics_empty_rect_init_00523980.cpp; no source of its own.


// Dynamic initializer (0x00523B40, _$E) of k from the header(s) included
// by HeaderStatics_empty_rect_init_00523980.cpp; no source of its own.


// Dynamic initializer (0x00523B50, _$E) of l from the header(s) included
// by HeaderStatics_empty_rect_init_00523980.cpp; no source of its own.


/***************************************************************************
 * IPXConnClass::IPXConnClass -- class constructor                         *
 *                                                                         *
 * INPUT:                                                                  *
 *      numsend         desired # of entries for the send queue            *
 *      numreceive      desired # of entries for the receive queue         *
 *      maxlen         max length of an application packet                 *
 *      magicnum         the packet "magic number" for this connection     *
 *      address         address of destination (NULL = no address)         *
 *      id               connection's unique numerical ID                  *
 *      name            connection's name                                  *
 *      extralen         max size of app-specific extra bytes (optional)   *
 *                                                                         *
 * OUTPUT:                                                                 *
 *      none.                                                              *
 *                                                                         *
 * WARNINGS:                                                               *
 *      none.                                                              *
 *                                                                         *
 * HISTORY:                                                                *
 *   12/20/1994 BR : Created.                                              *
 *=========================================================================*/
// (0x00523B70)
// A connection with a 2-tick retry, no retry limit and a 60-tick timeout,
// to the given address (sent directly to its node).

IPXConnClass::IPXConnClass(int numsend, int numreceive, int maxlen, unsigned short magicnum,
    IPXAddressClass *address, int id, wchar_t const *name, int extralen) :
    ConnectionClass(numsend, numreceive, maxlen, magicnum, 2, 0xFFFFFFFF, 60, extralen)
{
    NetNumType net;
    NetNodeType node;

    if (address) {
        Address = (*address);
    }
    ID = id;
    wcscpy(Name, name);
    Address.Get_Address(net, node);
    memcpy(ImmediateAddress, node, 6);
    unknown_60 = 0;
}


/***************************************************************************
 * IPXConnClass::Init -- hardware-specific initialization routine          *
 *                                                                         *
 * INPUT:                                                                  *
 *      none.                                                              *
 *                                                                         *
 * OUTPUT:                                                                 *
 *      none.                                                              *
 *                                                                         *
 * WARNINGS:                                                               *
 *      none.                                                              *
 *                                                                         *
 * HISTORY:                                                                *
 *   12/20/1994 BR : Created.                                              *
 *=========================================================================*/
// (0x00523C20, slot 1)

void IPXConnClass::Init()
{
    ConnectionClass::Init();
}


/***************************************************************************
 * IPXConnClass::Configure -- One-time initialization routine              *
 *                                                                         *
 * This routine sets up static members that are shared by all IPX          *
 * connections (ie those variables used by the Send/Listen/Broadcast       *
 * routines).                                                              *
 *                                                                         *
 * INPUT:                                                                  *
 *      socket            socket ID for sending & receiving                *
 *      conn_num            local IPX Connection Number (0 = not logged in)*
 *      listen_ecb         ptr to ECBType for listening                    *
 *      send_ecb            ptr to ECBType for sending                     *
 *      listen_header      ptr to IPXHeaderType for listening              *
 *      send_header         ptr to IPXHeaderType for sending               *
 *      listen_buf         ptr to buffer for listening                     *
 *      send_buf            ptr to buffer for sending                      *
 *      handler_rm_ptr      REAL-MODE pointer to event service routine     *
 *                        (high word = segment, low word = offset)         *
 *      maxpacketlen      max packet size to listen for                    *
 *                                                                         *
 * OUTPUT:                                                                 *
 *      none.                                                              *
 *                                                                         *
 * WARNINGS:                                                               *
 *      - All pointers must be protected-mode pointers, but must point to  *
 *        DOS real-mode memory (except the Handler segment/offset)         *
 *                                                                         *
 * HISTORY:                                                                *
 *   12/20/1994 BR : Created.                                              *
 *=========================================================================*/
// (0x00523C30)
// Keeps the shared socket settings and marks the class configured.

void __fastcall IPXConnClass::Configure(unsigned short socket, int conn_num, int arg3, int arg4, int arg5, int arg6, int arg7)
{
    Socket = socket;
    ConnectionNum = conn_num;
    unknown_a523c4 = arg3;
    unknown_a523e0 = arg4;
    unknown_a52398 = arg5;
    unknown_a52370 = arg6;
    unknown_a523d4 = arg7;
    Configured = 1;
}


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
unsigned short IPXConnClass::Socket;

/***************************************************************************
 * IPXConnClass::Start_Listening -- commands IPX to listen                 *
 *                                                                         *
 * This routine may be used to start listening in polled mode (if the      *
 * ECB's Event_Service_Routine is NULL), or in interrupt mode; it's        *
 * up to the caller to fill the ECB in.  If in polled mode, Listening      *
 * must be restarted every time a packet comes in.                         *
 *                                                                         *
 * INPUT:                                                                  *
 *      none.                                                              *
 *                                                                         *
 * OUTPUT:                                                                 *
 *      none.                                                              *
 *                                                                         *
 * WARNINGS:                                                               *
 *      - The ListenECB must have been properly filled in by the IPX Manager.*
 *      - Configure must be called before calling this routine.            *
 *                                                                         *
 * HISTORY:                                                                *
 *   12/16/1994 BR : Created.                                              *
 *=========================================================================*/
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


/***************************************************************************
 * IPXConnClass::Stop_Listening -- commands IPX to stop  listen            *
 *                                                                         *
 * INPUT:                                                                  *
 *      none.                                                              *
 *                                                                         *
 * OUTPUT:                                                                 *
 *      none.                                                              *
 *                                                                         *
 * WARNINGS:                                                               *
 *      - This routine MUST NOT be called if IPX is not listening already! *
 *                                                                         *
 * HISTORY:                                                                *
 *   12/16/1994 BR : Created.                                              *
 *=========================================================================*/
// (0x00523CF0)

int IPXConnClass::Stop_Listening()
{
    if (PacketTransport) {
        PacketTransport->Stop_Listening();
    }
    Listening = 0;
    return 1;
}


/***************************************************************************
 * IPXConnClass::Send -- sends a packet; invoked by SequencedConnection    *
 *                                                                         *
 * INPUT:                                                                  *
 *      buf         buffer to send                                         *
 *      buflen      length of buffer to send                               *
 *      extrabuf      (not used by this class)                             *
 *      extralen      (not used by this class)                             *
 *                                                                         *
 * OUTPUT:                                                                 *
 *      1 = OK, 0 = error                                                  *
 *                                                                         *
 * WARNINGS:                                                               *
 *      none.                                                              *
 *                                                                         *
 * HISTORY:                                                                *
 *   12/16/1994 BR : Created.                                              *
 *=========================================================================*/
// (0x00523D10, slot 9)
// Sends to this connection's address.

int IPXConnClass::Send(char *buf, int buflen, void *extrabuf, int extralen, bool flagged, unsigned short id)
{
    return Send_To(buf, buflen, &Address, 0, flagged, 0);
}


/***************************************************************************
 * IPXConnClass::Open_Socket -- opens communications socket                *
 *                                                                         *
 * INPUT:                                                                  *
 *      socket      desired socket ID number                               *
 *                                                                         *
 * OUTPUT:                                                                 *
 *      1 = OK, 0 = error                                                  *
 *                                                                         *
 * WARNINGS:                                                               *
 *      none.                                                              *
 *                                                                         *
 * HISTORY:                                                                *
 *   12/16/1994 BR : Created.                                              *
 *=========================================================================*/
// (0x00523D30)

void __fastcall IPXConnClass::Open_Socket(unsigned short socket)
{
    PacketTransport->Set_NetCard(unknown_a40b60);
    SocketOpen = PacketTransport->Open_Socket(socket);
}


/***************************************************************************
 * IPXConnClass::Close_Socket -- closes the socket                         *
 *                                                                         *
 * INPUT:                                                                  *
 *      socket      desired socket ID number                               *
 *                                                                         *
 * OUTPUT:                                                                 *
 *      none.                                                              *
 *                                                                         *
 * WARNINGS:                                                               *
 *      Calling this routine when the sockets aren't open may crash!       *
 *                                                                         *
 * HISTORY:                                                                *
 *   12/16/1994 BR : Created.                                              *
 *=========================================================================*/
// (0x00523D70)

void IPXConnClass::Close_Socket()
{
    PacketTransport->Close_Socket();
    SocketOpen = 0;
}


/***************************************************************************
 * IPXConnClass::Send_To -- sends the packet to the given address          *
 *                                                                         *
 * The "ImmediateAddress" field of the SendECB must be filled in with the  *
 * address of a bridge, or the node address of the destination if there    *
 * is no bridge.  The NETX call to find this address will always crash     *
 * if NETX isn't loaded (ConnectionNum is 0), so this case is trapped &    *
 * prevented.                                                              *
 * Also, if the address of this IPX connection is known when the           *
 * constructor is called, and Configure has been called, Get_Local_Target  *
 * is called to precompute the ImmediateAddress; this case is detected &   *
 * if the value is already computed, it's just memcpy'd over.              *
 *                                                                         *
 * INPUT:                                                                  *
 *      buf         buffer to send                                         *
 *      buflen      length of buffer                                       *
 *      address      Address to send to                                    *
 *      immed         ImmediateAddress value, NULL if none                 *
 *                                                                         *
 * OUTPUT:                                                                 *
 *      1 = OK, 0 = error                                                  *
 *                                                                         *
 * WARNINGS:                                                               *
 *      none.                                                              *
 *                                                                         *
 * HISTORY:                                                                *
 *   12/16/1994 BR : Created.                                              *
 *=========================================================================*/
// (0x00523D90, slot 10)
// Sends a packet to an address (through a bridge network when given). A
// forwarded packet gets the connection's forwarding target (no target: not
// sent) and goes to the forwarding address, carrying the forwarding port
// unless that address is the first node's. In the game-mode 4 session the
// port comes from the session player of the connection.

int IPXConnClass::Send_To(char *buf, int buflen, IPXAddressClass *address, unsigned char *immed, bool flagged, unsigned short id)
{
    IPXAddressClass dest;
    dest = *address;
    if (immed) {
        NetNumType net;
        NetNodeType node;
        address->Get_Address(net, node);
        memcpy(net, immed, 4);
        dest.Set_Address(net, node, 0);
    }

    unsigned short port = 0;
    CommHeaderType *header = (CommHeaderType *)buf;
    if (!flagged && unknown_flag_a3fba8) {
        if (header->unknown_03 == 0) {
            unsigned char target = OwnerTalk.Mask_From_ID(ID);
            if (!target) {
                return 0;
            }
            header->unknown_03 = target;
        }
        dest = unknown_forward_address_a3fbac;
        port = LoopbackPort;
        if (unknown_forward_address_a3fbb8 == unknown_players_a3fa7c[0]->Address
            && dest.Is_Same_Node(unknown_players_a3fa7c[0]->Address)) {
            header->unknown_0c = 0;
            memset(&header->unknown_08, 0, sizeof(header->unknown_08));
        } else {
            port = unknown_forward_port_a3fbc4;
            header->unknown_0c = LoopbackPort;
            memset(&header->unknown_08, 0, sizeof(header->unknown_08));
        }
    } else {
        header->unknown_0c = 0;
        memset(&header->unknown_08, 0, sizeof(header->unknown_08));
    }

    if (unknown_game_mode_a3d298 == 4 && !unknown_flag_a3fba8) {
        int index = Ipx.unknown_slot19(this);
        switch (index) {
            case -4:
                if ((unsigned short)unknown_forward_port_a3fbc4) {
                    port = unknown_forward_port_a3fbc4;
                }
                break;
            case -3:
            case -2:
            case -1:
                break;
            default:
                if (index == 0 && unknown_flag_a3fba8) {
                    if ((unsigned short)unknown_forward_port_a3fbc4) {
                        port = unknown_forward_port_a3fbc4;
                    }
                } else if (index >= 0 && index < Ipx.Num_Connections()) {
                    port = unknown_session_a3d298.Player_Port(Ipx.Connection_ID(index));
                }
                break;
        }
    }

    if (PacketTransport) {
        PacketTransport->WriteTo(buf, buflen, &dest, sizeof(dest), flagged, 0, port);
    }
    return 1;
}


/***************************************************************************
 * IPXConnClass::Broadcast -- broadcasts the given packet                  *
 *                                                                         *
 * INPUT:                                                                  *
 *      socket      desired socket ID number                               *
 *                                                                         *
 * OUTPUT:                                                                 *
 *      1 = OK, 0 = error                                                  *
 *                                                                         *
 * WARNINGS:                                                               *
 *      none.                                                              *
 *                                                                         *
 * HISTORY:                                                                *
 *   12/16/1994 BR : Created.                                              *
 *=========================================================================*/
// (0x00523F70)

int __fastcall IPXConnClass::Broadcast(char *buf, int buflen)
{
    PacketTransport->Broadcast(buf, buflen, 0);
    return 1;
}


// void *IPXConnClass::`scalar deleting destructor'(unsigned int flags) (0x00523F90)
// Compiler-generated with the vtable (the implicit destructor inlined). No source of its own.
