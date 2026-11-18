
// ---------------------------------------------------------------------------------------------
// NATIVE (lane YW, LAN), appended to the multiplayer unit: MultiplayerGameMode's base virtuals the YR
// tree leaves undefined, read from gamemd.exe's vtable (0x007EED60; each one a few bytes: a constant
// return, the by-value DynamicVectorClass parameters' destruction aside, which the native ABI leaves
// to the caller). The network screens and the game's start call them on the modes that do not
// override them. Behaviour code for the native build.
// ---------------------------------------------------------------------------------------------
bool MultiplayerGameMode::vt_entry_0C(unsigned a) { return true; }                                    // 0x005C0E50
void MultiplayerGameMode::vt_entry_20(void) {}                                                        // 0x005C0E60
void MultiplayerGameMode::vt_entry_24(void) {}                                                        // 0x005C0E70
void MultiplayerGameMode::vt_entry_38(unsigned a) {}                                                  // 0x005C0E80
bool MultiplayerGameMode::vt_entry_54(int index, DynamicVectorClass<int> picks) { return false; }     // 0x005C0EB0
bool MultiplayerGameMode::vt_entry_58(int index, DynamicVectorClass<int> picks, char const * map, char const * name) { return false; }   // 0x005C0E90
bool MultiplayerGameMode::vt_entry_5C(unsigned a, unsigned b, unsigned c) { return true; }            // 0x005C0ED0
bool MultiplayerGameMode::vt_entry_60(void) { return true; }                                          // 0x005D6320
bool MultiplayerGameMode::vt_entry_64(void) { return true; }                                          // 0x005D6330
bool MultiplayerGameMode::vt_entry_68(void) { return true; }                                          // 0x005C0EE0
void MultiplayerGameMode::vt_entry_74(unsigned a, unsigned b) {}                                      // 0x005C0EF0
void MultiplayerGameMode::vt_entry_78(HWND dialog) {}                                                 // 0x005C0F00
void MultiplayerGameMode::vt_entry_90(void) {}                                                        // 0x005C0F10
void MultiplayerGameMode::vt_entry_94(void) {}                                                        // 0x005C0F20
int MultiplayerGameMode::vt_entry_98(void) { return -1; }                                             // 0x005C0F30
void MultiplayerGameMode::vt_entry_9C(void) {}                                                        // 0x005C0F40
void MultiplayerGameMode::vt_entry_A0(bool playerWon) {}                                              // 0x005C0F50
void MultiplayerGameMode::vt_entry_A4(unsigned a) {}                                                  // 0x005C0F60
bool MultiplayerGameMode::vt_entry_A8(void) { return false; }                                         // 0x005C0F70
void MultiplayerGameMode::vt_entry_AC(void) {}                                                        // 0x005C0F80
void MultiplayerGameMode::vt_entry_B0(void) {}                                                        // 0x005C0F90
bool MultiplayerGameMode::vt_entry_B4(DynamicVectorClass<int> list, int * out) { return false; }      // 0x005C0FA0
int MultiplayerGameMode::vt_entry_B8(void) { return 0; }                                              // 0x005C0FC0
bool MultiplayerGameMode::vt_entry_BC(void) { return true; }                                          // 0x005C0FD0
