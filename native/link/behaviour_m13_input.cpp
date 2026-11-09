// Behaviour rows (Scout1115, native M13; NOT EXACT, native/behaviour_rewrites.json):
// leaves the player's first clicks reach (ScrollClass 0x0066A400's WM_LBUTTONDOWN,
// the selection band, Clear_Selected_Objects 0x00482BC0), written from their
// seeds (src/recovered/<entry>.cpp) and the pass-6a decompiles, with the shared
// headers' member names instead of the VC6 offsets.
#include "shared/display.hpp"     // before map_view.hpp (full DisplayClass)
#include "shared/uicmnds.hpp"
#include "shared/tactical.hpp"
#include "shared/beacon.hpp"

extern int unknown_state_ac132c;      // 0x00AC132C (Unknown_Command_Experience 0x006F7EC0 reads it)
extern bool DAT_00ac133d;             // 0x00AC133D

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

// 0x006F6780 (seed: stores ECX at 0x00AC132C and clears the byte at 0x00AC1330).
void __fastcall Unknown_006f6780(unsigned int value)
{
    unknown_state_ac132c = (int)value;
    unknown_flag_ac1330 = false;
}

// 0x006F76B0 (seed: returns the byte at 0x00AC133D).
bool Unknown_006F76B0()
{
    return DAT_00ac133d;
}

// 0x0042E830 (seed): when the beacons are enabled (+0x60), clears the blink bit (bit 1 of
// +0x0C) of every beacon in the eight houses' three slots.
void __fastcall Unknown_0042e830(Unknown84E8D0 *receiver)
{
    if (receiver->unknown_60 != 0) {
        for (int house = 0; house < 8; ++house) {
            for (int slot = 0; slot < 3; ++slot) {
                UnknownBeacon *beacon = receiver->unknown_00[house][slot];
                if (beacon != 0) beacon->unknown_flags &= 0xFD;
            }
        }
    }
}

// 0x0049C490 DisplayClass slot 45 (seed): the left button went down at xy. With no
// mode active (+0x11AC, +0x11AE, +0x11AD clear, +0x11B4 -1) and nothing pending
// placement (+0x11A4), a selection band is anchored there (+0x11CC set, its start
// +0x11D0 and last point +0x11D8 both xy).
void DisplayClass::unknown_slot45(Point2D const &xy)
{
    if (!unknown_11ac && !unknown_11ae && !unknown_11ad && unknown_11b4 == -1 && unknown_11a4 == 0) {
        unknown_11cc = true;
        unknown_11d0 = xy;
        unknown_11d8 = xy;
    }
}

// 0x006F6770 (seed: clears the byte at 0x00AC1330); Mouse_Left_Release 0x0049BB20 calls
// it after a placement or selection click.
void Unknown_Click_End_006F6770()
{
    unknown_flag_ac1330 = false;
}

// 0x00614B50 (seed): an infantry action is allowed unless planning mode is on
// (0x00A767A4) and the action is 4.
extern bool unknown_flag_a767a4;
unsigned char __fastcall Infantry_Action_Allowed_00614b50(int action)
{
    if (unknown_flag_a767a4 && action == 4) return 0;
    return 1;
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
