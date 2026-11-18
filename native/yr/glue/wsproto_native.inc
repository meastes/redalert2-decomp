
// ---------------------------------------------------------------------------------------------
// NATIVE (lane YW, LAN), appended to the wsproto unit: WinsockInterfaceClass's packet CRC members.
// gamemd.exe's virtuals at 0x007B1FD0 and 0x007B2050 (slots 21 and 22, the RA2 tree's
// WinsockInterfaceClass::Build_Packet_CRC / Passes_CRC_Check) are matched above as free __stdcall
// functions of the packet (their code never reads `this`), while wspudp.cpp and wspipx.cpp call the
// members. Natively a member takes `this` first, so one function cannot carry both names (as an
// address alias would): the members call the matched functions. Its buffer type is this unit's copy
// of the class's WinsockBufferType (the same layout).
// ---------------------------------------------------------------------------------------------
void WinsockInterfaceClass::Build_Packet_CRC(WinsockInterfaceClass::WinsockBufferType *packet)
{
	::Build_Packet_CRC((::WinsockBufferType *)packet);
}

bool WinsockInterfaceClass::Passes_CRC_Check(WinsockInterfaceClass::WinsockBufferType *packet)
{
	return ::Passes_CRC_Check((::WinsockBufferType *)packet);
}
