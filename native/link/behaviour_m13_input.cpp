// Behaviour rows (Scout1115, native M13; NOT EXACT, native/behaviour_rewrites.json):
// leaves the player's first clicks reach (ScrollClass 0x0066A400's WM_LBUTTONDOWN,
// the selection band, Clear_Selected_Objects 0x00482BC0), written from their
// seeds (src/recovered/<entry>.cpp) and the pass-6a decompiles, with the shared
// headers' member names instead of the VC6 offsets.
#include "shared/display.hpp"     // before map_view.hpp (full DisplayClass)
#include "shared/uicmnds.hpp"
#include "shared/tactical.hpp"
#include "shared/beacon.hpp"


// 0x006AC7B0 (seed: returns this +0xDB0): the selection list behind 0x0083996C is the
// Tactical object there; its count is Tactical +0xDB0, the entries of the object pixel
// table 0x00ABE5F8.
int UnknownSelection::Count()
{
    return ((Tactical *)this)->unknown_db0;
}

// 0x006AC7C0 (seed: returns the first word of the 12-byte entry at 0x00ABE5F8 + 12 * index).
ObjectClass *UnknownSelection::Get(int index)
{
    return unknown_tactical_pixels_abe5f8[index].Object;
}







// 0x0041B360 (seed: returns 0), TechnoClass slot 153; a shared one-instruction body
// (the techno.hpp row says "kept copy elsewhere").
#include "shared/techno.hpp"
bool TechnoClass::unknown_slot153()
{
    return false;
}

// 0x00453BE0 (seed: an empty body, `ret 8`), TechnoClass slot 243; drawing a selected
// unit reaches it every frame.
void TechnoClass::unknown_slot243(bool arg1, int arg2)
{
}

// 0x006F58E0 (seed): shows a message (priority -1: 0xF0 frames) in the player's colour
// scheme (3 without a player) through the on-screen message list 0x00A3DC70
// (MessageListClass::Add_Message 0x005B4DA0, style 0x4046).
#include "shared/house.hpp"
void __fastcall Unknown_006F58E0(wchar_t const *text, int priority)
{
    if (priority == -1) priority = 0xF0;
    int color = 3;
    if (unknown_player_a35db4 != 0) color = unknown_player_a35db4->ColorSchemeIndex;
    unknown_messages_a3dc70.Add_Message(0, 0, text, color, 0x4046, priority, true);
}
