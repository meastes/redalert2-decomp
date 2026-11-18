
// ---------------------------------------------------------------------------------------------
// NON-EXACT NATIVE DRAFT (lane YW), appended to the ipxmgr unit: IPXManagerClass::Service,
// YR 0x00541820 (2,655 bytes, unmatched in ra2-decomp). From the RA2 tree's draft of its twin
// (src/recovered/ipxmgr.cpp, RA2 0x00525F60, 97.99) with gamemd.exe's members and globals: each
// waiting datagram goes to the global channel (its magic number), the lobby channel (with the
// packet router) or the connection it came from (the product id), then every channel and
// connection is serviced. LEFT OUT: the Internet game's branches (Session.Type == GAME_INTERNET:
// NAT port translation, a router's address change, a player's address learnt from an event):
// Westwood Online is not part of the native build (fixups.json), so that type never occurs.
// Behaviour code for the native build; never part of an exact build.
// ---------------------------------------------------------------------------------------------
extern WinsockInterfaceClass * DAT_00887628;   // the packet transport (PacketTransport in the RA2 tree)

int IPXManagerClass::Service(void)
{
	int result = 1;
	int packetlen;
	unsigned char receive_buffer[1024];
	char address_buffer[128];
	unsigned short port;
	IPXAddressClass address;

	if (DAT_00887628) {
		do {
			int receive_length = sizeof(receive_buffer);
			int address_length = sizeof(address_buffer);
			port = 0;
			packetlen = DAT_00887628->Read(receive_buffer, receive_length, address_buffer, address_length, port);
			if (!packetlen) {
				break;
			}
			CurDataBuf = (char *)receive_buffer;
			address = *(IPXAddressClass *)address_buffer;
			CommHeaderType *packet = (CommHeaderType *)CurDataBuf;
			if (packet->MagicNumber == GlobalChannel->Magic_Num()) {
				if (!FUN_00541680(packet, receive_buffer, packetlen, address_buffer, port)) {
					if (!GlobalChannel->Receive_Packet(packet, packetlen, &address, port)) {
						++ReceiveOverflows;
						FUN_004068E0("GlobalChannel recive buffer overflow %d\n", ReceiveOverflows);
						break;
					}
				}
			} else if (LobbyChannel && packet->MagicNumber == LobbyChannel->Magic_Num()) {
				if (Session.ViaPacketRouter && !LobbyChannel->Receive_Packet(packet, packetlen, &address, 0)) {
					++ReceiveOverflows;
					FUN_004068E0("RouterChannel recive buffer overflow %d\n", ReceiveOverflows);
					break;
				}
			} else if (packet->MagicNumber == ProductID
					&& !FUN_00541680(packet, receive_buffer, packetlen, address_buffer, port)) {
				if (Session.ViaPacketRouter && packet->Field3 == 0xFF) {
					if (packet->Code == ConnectionClass::PACKET_ACK) {
						MulticastChannel->Receive_Packet(packet, packetlen);
					} else if (NumConnections > 0 && !Connection[0]->Receive_Packet(packet, packetlen)) {
						++ReceiveOverflows;
						FUN_004068E0("Receive buffer overflow %d\n", ReceiveOverflows);
					}
				} else {
					int i;
					for (i = 0; i < NumConnections; i++) {
						if (Session.ViaPacketRouter ? OwnerTalk.Mask_From_ID(Connection[i]->ID) == packet->Field3
								: Connection[i]->Address == address) {
							break;
						}
					}
					if (i < NumConnections && !Connection[i]->Receive_Packet(packet, packetlen)) {
						++ReceiveOverflows;
						FUN_004068E0("Reecive buffer overflow %d\n", ReceiveOverflows);
					}
				}
			}
		} while (packetlen);
	}

	if (GlobalChannel && DAT_00887628 && !GlobalChannel->Service()) {
		GlobalChannel->Discard_Undeliverable_Packets();
		result = 0;
	}
	if (Session.ViaPacketRouter) {
		if (LobbyChannel) {
			if (!LobbyChannel->Service()) {
				LobbyChannel->Discard_Undeliverable_Packets();
				result = 0;
			}
			if (!Session.unknown_3098) {
				OwnerTalk.Handle_Router_Messages();
			}
		}
		if (MulticastChannel && !MulticastChannel->Service()) {
			FUN_004068E0("Multicast channel has gone bad\n");
			result = 0;
		}
	}
	if (DAT_00887628) {
		for (int i = 0; i < NumConnections; i++) {
			if (!Connection[i]->Service()) {
				result = 0;
				BadConnection = Connection[i]->ID;
				FUN_004068E0("Error - Connection %d has gone bad\n", BadConnection);
			}
		}
	}
	if (result) {
		BadConnection = CONNECTION_NONE;
	}
	return result;
}
