// beacon.cpp: the original translation unit re-filed as one source (Route A): the accepted
// bodies of its canonical compile group in address order, under the unit's own header
// state (its switches and includes in their original order). The module map
// manifests/original_target/modules/beacon.json carries the addresses and the rows that
// stay in their own files.


#include "shared/shape_file.hpp"
#include "shared/unknown_geometry.hpp"
#define UNKNOWN_BEACONS_DECLARE_TEXT
#define UNKNOWN_BEACONS_DECLARE_MEMBERS
#include "shared/unknown_globals.hpp"
#include "shared/beacon.hpp"
#define SHAPE_FILE_EMPTY_RECT Rect(0, 0, 0, 0)
#include "shared/house.hpp"
#include "shared/tactical.hpp"
#include "shared/text_print.hpp"
#include "shared/palette.hpp"
#include "shared/cd_timer.hpp"
#include "shared/keyboard.hpp"
#include "shared/gadget.hpp"
#include "shared/mouse_class.hpp"
#include "shared/map_view.hpp"
#include "shared/raw_file_class.hpp"
#include "shared/cc_file_class.hpp"
#include "shared/mix_file_class.hpp"
#include "shared/anim_type.hpp"
#include "shared/debug_printf.hpp"
#include "shared/rect.hpp"
#include "shared/session_state.hpp"
#include "shared/string_table.hpp"
#include "shared/message_list.hpp"
#include "shared/vox_class.hpp"
#include "shared/sound_play.hpp"
#include "shared/rules.hpp"
#include "shared/ipx_manager.hpp"
#include "shared/unknown_player_entry.hpp"
#include "shared/radar_event.hpp"
#include "shared/main_loop.hpp"


// Header-static dynamic initializers of the unit at 0x0042D770: the shape_file.hpp empty rect and the
// shared/unknown_geometry.hpp constants (0x0042D770-0x0042D951); the other
// drafts of this plan are emitted by these includes.


// Dynamic initializer (0x0042D790, _$E) of diagonal from the header(s) included
// by HeaderStatics_empty_rect_init_0042d770.cpp; no source of its own.


// Dynamic initializer (0x0042D7C0, _$E) of b from the header(s) included
// by HeaderStatics_empty_rect_init_0042d770.cpp; no source of its own.


// Dynamic initializer (0x0042D7E0, _$E) of c from the header(s) included
// by HeaderStatics_empty_rect_init_0042d770.cpp; no source of its own.


// Dynamic initializer (0x0042D800, _$E) of angle45 from the header(s) included
// by HeaderStatics_empty_rect_init_0042d770.cpp; no source of its own.


// Dynamic initializer (0x0042D820, _$E) of angle60 from the header(s) included
// by HeaderStatics_empty_rect_init_0042d770.cpp; no source of its own.


// Dynamic initializer (0x0042D840, _$E) of angle90 from the header(s) included
// by HeaderStatics_empty_rect_init_0042d770.cpp; no source of its own.


// Dynamic initializer (0x0042D860, _$E) of g from the header(s) included
// by HeaderStatics_empty_rect_init_0042d770.cpp; no source of its own.


// Dynamic initializer (0x0042D8A0, _$E) of h from the header(s) included
// by HeaderStatics_empty_rect_init_0042d770.cpp; no source of its own.


// Dynamic initializer (0x0042D8D0, _$E) of i from the header(s) included
// by HeaderStatics_empty_rect_init_0042d770.cpp; no source of its own.


// Dynamic initializer (0x0042D900, _$E) of j from the header(s) included
// by HeaderStatics_empty_rect_init_0042d770.cpp; no source of its own.


// Dynamic initializer (0x0042D930, _$E) of k from the header(s) included
// by HeaderStatics_empty_rect_init_0042d770.cpp; no source of its own.


// Dynamic initializer (0x0042D940, _$E) of l from the header(s) included
// by HeaderStatics_empty_rect_init_0042d770.cpp; no source of its own.


// Definition of the beacon manager object (0x0084E8D0): its dynamic initializer (0x0042D960, the
// constructor, then atexit of the exit thunk) and the exit thunk (0x0042D990, the destructor).

BeaconManagerClass unknown_object_84e8d0;


// Exit thunk (0x0042D990, _$E41) of the beacon manager object: its destructor, atexit-registered by
// the dynamic initializer of 0x0042D960 (emitted by that draft; no source of its own).


// UnknownBeacon::UnknownBeacon (0x0042D9E0): the default position (the unit's unknown_geometry_l),
// placed and blinking flags clear, an empty text. Out of line here; 0x0042E370 expands it.

UnknownBeacon::UnknownBeacon()
{
    unknown_coord = unknown_geometry_l;
    unknown_flags &= 0xfc;
    for (int index = 0; index < 128; index++) {
        unknown_text.chars[index] = 0;
    }
}


extern "C" __declspec(dllimport) unsigned long __stdcall timeGetTime();
extern "C" unsigned int __cdecl wcslen(unsigned short const *string);
extern "C" unsigned short *__cdecl wcsrchr(unsigned short const *string, unsigned short character);   // 0x007835C5

void UnknownBeacon::Unknown_0042DA20(Surface *surface, Rect rect)
{
    ColorScheme *scheme = ColorSchemes[Houses[unknown_house]->ColorSchemeIndex];
    Point2D pixel;
    if (!unknown_tactical_83996c->Coord_To_Pixel(unknown_coord, pixel)) {
        return;
    }
    int half = unknown_beacon_cycle_84e93c / 2;
    CC_Draw_Shape(surface, scheme->Converter, unknown_beacon_shape_84e98c,
                  ((unknown_flags & 2) ? half : 0) + Frame % half, pixel, rect, 0x600, 0, 0, 0, 1000, 0, 0,
                  Point2D(0, 0));
    if (wcslen(unknown_text.chars) == 0) {
        return;
    }
    UnknownBeaconText text = unknown_text;
    if (unknown_flags & 2) {
        if (unknown_flag_a3dc89 && timeGetTime() % 500 < 250) {
            unsigned short *cursor = wcsrchr(text.chars, 0x5f);
            if (cursor && wcslen(cursor) == 1) {
                *cursor = 0x20;
            }
        }
    }
    if (!((unknown_flags & 2) && unknown_flag_a3dc89)) {
        unsigned short *cursor = wcsrchr(text.chars, 0x5f);
        if ((cursor && wcslen(cursor) == 1) || ((cursor = wcsrchr(text.chars, 0x20)) && wcslen(cursor) == 1)) {
            *cursor = 0;
        }
    }
    BitFontClass *font = BitFont;
    int width = 0;
    int height = 0;
    font->Get_Text_Area(text.chars, &width, &height, 200);
    Rect box(pixel.X - width / 2 - 4, pixel.Y + 32, width + 8, height + 8);
    RGBClass color = scheme->BaseColor.To_RGB();
    Rect fill = Intersect(box, rect, 0, 0);
    unsigned pixel_color = ((color.Red >> unknown_shift_852fbc) << unknown_shift_852fb8)
                         | ((color.Green >> unknown_shift_852fcc) << unknown_shift_852fc8)
                         | ((color.Blue >> unknown_shift_852fc4) << unknown_shift_852fc0);
    surface->Fill_Rect(fill, 0);
    surface->Draw_Rect(fill, pixel_color);
    font->Set_Clipping(true);
    tagRECT clip;
    clip.left = rect.X;
    clip.top = rect.Y;
    clip.right = rect.X + rect.Width - 1;
    clip.bottom = rect.Y + rect.Height - 1;
    font->Set_Clip_Box(&clip);
    font->Set_Color((unsigned short)pixel_color);
    BitText->Print_Box(font, surface, text.chars, box.X + 4, box.Y + 4, 200, box.Height, 0, 0, 0);
}


// 0x0042dd50: function not yet identified
//
// Body: 3 bytes (frozen inventory bound).
// Referenced from 0 sites.
// Ghidra prototype: void FUN_0042dd50(void)
//
// Types and names are Ghidra placeholders;
// rename and retype as the purpose becomes known.
//

namespace seed_0042dd50 {

void __stdcall FUN_0042dd50(int unused_0, int unused_1, int unused_2, int unused_3, int unused_4, int unused_5, int unused_6)
{
    return;
}

}  // namespace seed_0042dd50


// UnknownBeacon::Unknown_0042DD60 (0x0042DD60): takes a position other than the default one and a
// house below 8 (then marks it placed). Out of line here; 0x0042E370 expands it.
// The position is stored through a fresh CoordStruct built from the parameter's members: that temporary
// gives the original's order (default X loaded before the Y parameter, Z in esi, Y in edx); storing the
// parameter itself swaps the Y and Z registers.

void UnknownBeacon::Unknown_0042DD60(CoordStruct coord, int house)
{
    if (coord != unknown_geometry_l) {
        unknown_coord = CoordStruct(coord.X, coord.Y, coord.Z);
    }
    if (house < 8) {
        unknown_house = house;
        unknown_flags |= 1;
    }
}


// 0x0042ddb0: function not yet identified
//
// Body: 22 bytes (frozen inventory bound).
// Referenced from 0 sites.
// Ghidra prototype: void __thiscall FUN_0042ddb0(int param_1,byte param_2)
//
// Types and names are Ghidra placeholders;
// rename and retype as the purpose becomes known.
//

namespace seed_0042ddb0 {

struct Owner_0042ddb0 { void FUN_0042ddb0(unsigned char flag_value); };

void Owner_0042ddb0::FUN_0042ddb0(unsigned char flag_value)
{
    int object_address = (int)this;
    *(unsigned char *)(object_address + 0xc) = (flag_value & 1) << 1 | *(unsigned char *)(object_address + 0xc) & 0xfd;
    return;
}

}  // namespace seed_0042ddb0


// 0x0042ddd0: function not yet identified
//
// Body: 1 bytes (frozen inventory bound).
// Referenced from 0 sites.
// Ghidra prototype: void FUN_0042ddd0(void)
//
// Types and names are Ghidra placeholders;
// rename and retype as the purpose becomes known.
//

namespace seed_0042ddd0 {

void  FUN_0042ddd0(void)
{
    return;
}

}  // namespace seed_0042ddd0


// 0x0042dde0: function not yet identified
//
// Body: 5 bytes (frozen inventory bound).
// Referenced from 0 sites.
// Ghidra prototype: void __fastcall FUN_0042dde0(int param_1)
//
// Types and names are Ghidra placeholders;
// rename and retype as the purpose becomes known.
//

namespace seed_0042dde0 {

void __fastcall FUN_0042dde0(int object_address)
{
    *(unsigned char *)(object_address + 0xc) = *(unsigned char *)(object_address + 0xc) & 0xfe;
    return;
}

}  // namespace seed_0042dde0


// UnknownBeacon::Unknown_0042DDF0 (0x0042DDF0): clears the text and copies at most 127 characters of the
// new one (none: stays empty). Out of line here; 0x0042EC20 expands it.

void UnknownBeacon::Unknown_0042DDF0(unsigned short const *text)
{
    for (int index = 0; index < 128; index++) {
        unknown_text.chars[index] = 0;
    }
    if (text) {
        wcsncpy(unknown_text.chars, text, 127);
    }
}


// Radar blink marker and dirty pixels, original0042DE20. Field names remain hypotheses.

extern Rect unknown_rect_8336e8;
extern Rect unknown_radar_rect_833978;

void UnknownBeacon::Unknown_0042DE20(Surface *surface, Rect window, bool forced)
{
    int frame = Frame % unknown_object_84e8d0.unknown_7c;
    if (frame < unknown_object_84e8d0.unknown_78 + 1 || forced) {
        // The canonical radar interface reads only the X/Y prefix; Z is not read.
        Point2D pixel = Map.Coord_To_Radar_Pixel((Point2D const &)unknown_coord, true);
        if (frame < unknown_object_84e8d0.unknown_78 && !forced) {
            CC_Draw_Shape(surface, ColorSchemes[Houses[unknown_house]->ColorSchemeIndex]->Converter,
                unknown_radar_beacon_shape_84e990, frame, pixel, window, 0x600, 0, 0, 0, 1000, 0, 0, Point2D(0, 0));
        }
        int width = unknown_object_84e8d0.unknown_70;
        int height = unknown_object_84e8d0.unknown_74;
        Rect marker(pixel.X - width / 2, pixel.Y - height / 2, width, height);
        Rect dirty = marker + Point2D(unknown_radar_rect_833978.X, unknown_radar_rect_833978.Y);
        unknown_rect_8336e8 = Intersect(Union(unknown_rect_8336e8, dirty), unknown_radar_rect_833978);
        for (int y = marker.Y; y < marker.Y + marker.Height; ++y) {
            for (int x = marker.X; x < marker.X + marker.Width; ++x) {
                Point2D point(x, y);
                Map.Radar_Pixel(point);
            }
        }
    }
}


// UnknownBeacon::Unknown_0042E080 (0x0042E080): the beacon is placed by a house that is the player's
// ally both ways and not defeated (+0x13D).

bool UnknownBeacon::Unknown_0042E080()
{
    if ((unknown_flags & 1) && unknown_player_a35db4->Is_Ally(unknown_house)
        && Houses[unknown_house]->Is_Ally(unknown_player_a35db4) && !Houses[unknown_house]->unknown_13d) {
        return true;
    }
    return false;
}


// BeaconManagerClass::BeaconManagerClass (0x0042E0E0): no beacons, disabled, no radar blink cycle. Out of line
// here; the definition of unknown_object_84e8d0 (0x0042D960) expands it.

BeaconManagerClass::BeaconManagerClass()
{
    for (int house = 0; house < 8; house++) {
        for (int slot = 0; slot < 3; slot++) {
            unknown_00[house][slot] = 0;
        }
    }
    unknown_60 = 0;
    unknown_7c = 0;
}


// BeaconManagerClass::~BeaconManagerClass (0x0042E100): deletes every beacon (0x0042E150 expanded). Out of line
// here; the exit thunk of unknown_object_84e8d0 (0x0042D990) expands it.

BeaconManagerClass::~BeaconManagerClass()
{
    Unknown_0042E150();
}


// BeaconManagerClass::Unknown_0042E150 (0x0042E150): deletes every beacon and disables them.

void BeaconManagerClass::Unknown_0042E150()
{
    if (unknown_60) {
        for (int house = 0; house < 8; house++) {
            for (int slot = 0; slot < 3; slot++) {
                if (unknown_00[house][slot]) {
                    delete unknown_00[house][slot];
                    unknown_00[house][slot] = 0;
                }
            }
        }
        unknown_60 = 0;
    }
}


void BeaconManagerClass::Unknown_0042E1A0()
{
    RawFileClass beacon_file("PBEACON.SHP");
    unknown_beacon_shape_84e98c = (ShapeFileStruct const *)Load_Alloc_Data(beacon_file);
    if (unknown_beacon_shape_84e98c == 0) {
        unknown_beacon_shape_84e98c = (ShapeFileStruct const *)MixFileClass::Retrieve("PBEACON.SHP");
    }
    if (unknown_beacon_shape_84e98c != 0) {
        unknown_64 = unknown_beacon_shape_84e98c->Width;
        unknown_68 = unknown_beacon_shape_84e98c->Height;
        unknown_6c = unknown_beacon_shape_84e98c->FrameCount;
    } else {
        Debug_Printf("Failed to load BeaconArt");
    }
    RawFileClass radar_file("RDRBEACN.SHP");
    unknown_radar_beacon_shape_84e990 = (ShapeFileStruct const *)Load_Alloc_Data(radar_file);
    if (unknown_radar_beacon_shape_84e990 == 0) {
        unknown_radar_beacon_shape_84e990 = (ShapeFileStruct const *)MixFileClass::Retrieve("RDRBEACN.SHP");
    }
    if (unknown_radar_beacon_shape_84e990 != 0) {
        unknown_70 = unknown_radar_beacon_shape_84e990->Width;
        unknown_74 = unknown_radar_beacon_shape_84e990->Height;
        short frames = unknown_radar_beacon_shape_84e990->FrameCount;
        unknown_78 = frames;
        unknown_7c = frames << 2;
    } else {
        Debug_Printf("Failed to load RadarBeaconArt");
    }
}


void BeaconManagerClass::Unknown_0042E290(Surface *surface, Rect rect)
{
    if (unknown_60) {
        for (int house = 0; house < 8; house++) {
            for (int slot = 0; slot < 3; slot++) {
                UnknownBeacon *beacon = unknown_00[house][slot];
                if (beacon && (beacon->unknown_flags & 1) && unknown_player_a35db4->Is_Ally(beacon->unknown_house)
                    && Houses[beacon->unknown_house]->Is_Ally(unknown_player_a35db4)
                    && !Houses[beacon->unknown_house]->unknown_13d) {
                    beacon->Unknown_0042DA20(surface, rect);
                }
            }
        }
    }
}


// BeaconManagerClass::Unknown_0042E370 (0x0042E370): places a beacon of the house at the coordinate in the
// slot (-1: the first free one; none free: nothing). The player's own placement shows the help lines,
// plays EVA_BeaconPlaced and the placement sound and sends packet 0x20 to the other players; an
// allied one raises a radar event (kind 11) and EVA_BeaconDetected.

void BeaconManagerClass::Unknown_0042E370(int house, CoordStruct coord, int slot)
{
    if (!Unknown_0042E700(house) && slot == -1) {
        return;
    }
    UnknownBeacon *beacon = new UnknownBeacon;
    bool full = false;
    if (slot == -1) {
        full = true;
        int index;
        for (index = 0; index < 3; index++) {
            if (unknown_00[house][index] == 0) {
                full = false;
                break;
            }
        }
        if (!full) {
            slot = index;
            unknown_00[house][slot] = beacon;
        }
    } else {
        if (unknown_00[house][slot]) {
            delete unknown_00[house][slot];
            unknown_60--;
            unknown_00[house][slot] = beacon;
        } else {
            unknown_00[house][slot] = beacon;
        }
    }
    if (full) {
        delete beacon;
        return;
    }
    beacon->Unknown_0042DD60(coord, house);
    unknown_60++;
    if (unknown_session_a3d298.Type != 0 && unknown_session_a3d298.Type != 5) {
        if (house == unknown_player_a35db4->ArrayIndex) {
            unknown_session_a3d298.Messages.Add_Message(0, 0,
                StringTable::Load_String("TXT_BEACON_HELP1", 0, "D:\\Ra2\\Beacon.CPP", 811),
                unknown_player_a35db4->ColorSchemeIndex, 0x4046, 225, false);
            unknown_session_a3d298.Messages.Add_Message(0, 0,
                StringTable::Load_String("TXT_BEACON_HELP2", 0, "D:\\Ra2\\Beacon.CPP", 812),
                unknown_player_a35db4->ColorSchemeIndex, 0x4046, 225, false);
            Speech_Play_Name("EVA_BeaconPlaced", -1, -1);
            Voc_Play(Rules->PlaceBeaconSound, 1.0f, 0x2000, 0);
            UnknownBeaconPacket packet;
            packet.Type = 0x20;
            strcpy(packet.Name, Wide_To_Narrow_006fa120(unknown_players_a3fa7c[0]->Name));
            packet.X = coord.X;
            packet.Y = coord.Y;
            packet.Z = coord.Z;
            packet.Slot = (unsigned char)slot;
            packet.House = (unsigned char)house;
            for (int index = 1; index < unknown_players_a3fa7c.Count(); index++) {
                Debug_Printf("Sending beacon placement to %s\n", unknown_players_a3fa7c[index]);
                unknown_ipx_manager_a40978.Send_Global_Message(&packet, 0x3d, 1, &unknown_players_a3fa7c[index]->Address, 0, 0);
            }
        } else if (beacon->Unknown_0042E080()) {
            if (Unknown_0063B5B0(11, Cell(coord.X / 256, coord.Y / 256))) {
                Speech_Play_Name("EVA_BeaconDetected", -1, -1);
            }
        }
    }
}


// bool BeaconManagerClass::Unknown_0042E700(int house) (0x0042E700)
// Whether the house still has an empty one of its three slots (DisplayClass
// 0x0049CAC0 tests it before entering the +0x11B0 mode).

bool BeaconManagerClass::Unknown_0042E700(int house)
{
    for (int i = 0; i < 3; i++) {
        if (unknown_00[house][i] == 0) {
            return true;
        }
    }
    return false;
}


// Point2D __stdcall unknown_zero_point_0042e730() (0x0042E730)
// Returns the point (0, 0) through the hidden result pointer and pops it (ret 4). No callers: a
// __thiscall member returning a Point2D compiles to the same bytes, so the owner (the beacon
// manager's neighbours 0x0042E700/0x0042E740 suggest BeaconManagerClass) and convention are hypotheses.

Point2D __stdcall unknown_zero_point_0042e730()
{
    return Point2D(0, 0);
}


// BeaconManagerClass::Unknown_0042E740 (0x0042E740): selects the beacon within 128 of the point
// (unselecting everything else first); 0x0042E860 is expanded.

bool BeaconManagerClass::Unknown_0042E740(CoordStruct coord)
{
    UnknownBeacon *beacon = FindBeaconAt_0042e860(coord);
    if (beacon) {
        Unknown_00482BC0();
        beacon->unknown_flags |= 2;
        return true;
    }
    return false;
}


// void __fastcall Unknown_0042e830(BeaconManagerClass *beacons) (0x0042E830): when the beacons
// are enabled (+0x60), clears the blink bit (bit 1) of every beacon in the eight houses'
// three slots (Clear_Selected_Objects 0x00482BC0 calls it).

void __fastcall Unknown_0042e830(BeaconManagerClass *beacons)
{
    if (beacons->unknown_60 != 0) {
        for (int house = 0; house < 8; house++) {
            for (int slot = 0; slot < 3; slot++) {
                if (beacons->unknown_00[house][slot] != 0) {
                    beacons->unknown_00[house][slot]->unknown_flags &= ~2;
                }
            }
        }
    }
}


// BeaconManagerClass::FindBeaconAt_0042e860 (0x0042E860): the beacon within 128 of the point, else none.

UnknownBeacon *BeaconManagerClass::FindBeaconAt_0042e860(CoordStruct coord)
{
    if (unknown_60) {
        for (int house = 0; house < 8; house++) {
            for (int slot = 0; slot < 3; slot++) {
                if (unknown_00[house][slot] && Beacon_Distance(coord, unknown_00[house][slot]->unknown_coord) < 128) {
                    return unknown_00[house][slot];
                }
            }
        }
    }
    return 0;
}


// BeaconManagerClass::Unknown_0042E940 (0x0042E940): the house's beacon whose text is being edited
// (flag 2; house -1: any house's), else none.

UnknownBeacon *BeaconManagerClass::Unknown_0042E940(int house)
{
    if (unknown_60) {
        for (int index = 0; index < 8; index++) {
            for (int slot = 0; slot < 3; slot++) {
                UnknownBeacon *beacon = unknown_00[index][slot];
                if (beacon && (beacon->unknown_flags & 2)) {
                    if (house == -1) {
                        return beacon;
                    }
                    if (house == beacon->unknown_house) {
                        return beacon;
                    }
                }
            }
        }
    }
    return 0;
}


// BeaconManagerClass::Unknown_0042E990 (0x0042E990): removes the beacon in the house's slot (house and slot
// -1: the edited one). Cancelling the edit of another house's beacon only clears its flags; a visible
// beacon is erased from the radar first; the player's own removal sends packet 0x21 to the others.

void BeaconManagerClass::Unknown_0042E990(int house, int slot)
{
    if (unknown_60) {
        bool edited;
        UnknownBeacon *beacon;
        edited = house == -1 && slot == -1;
        if (edited) {
            beacon = Unknown_0042E940(-1);
        } else {
            beacon = unknown_00[house][slot];
        }
        if (beacon) {
            int owner = beacon->unknown_house;
            if (owner != unknown_player_a35db4->ArrayIndex && edited) {
                beacon->unknown_flags &= 0xfc;
                return;
            }
            if (beacon->Unknown_0042E080()) {
                beacon->Unknown_0042DE20(Map.unknown_1218, Map.unknown_1218->Get_Rect(), true);
            }
            delete beacon;
            int index;
            for (index = 0; index < 3; index++) {
                if (beacon == unknown_00[owner][index]) {
                    goto found;
                }
            }
            return;
found:
            unknown_00[owner][index] = 0;
            unknown_60--;
            if (unknown_session_a3d298.Type != 0 && unknown_session_a3d298.Type != 5
                && owner == unknown_player_a35db4->ArrayIndex && edited) {
                UnknownBeaconPacket packet;
                packet.Type = 0x21;
                strcpy(packet.Name, Wide_To_Narrow_006fa120(unknown_players_a3fa7c[0]->Name));
                packet.House = (unsigned char)owner;
                packet.Slot = (unsigned char)index;
                for (int player = 1; player < unknown_players_a3fa7c.Count(); player++) {
                    Debug_Printf("Sending beacon delete to %s\n", unknown_players_a3fa7c[player]);
                    unknown_ipx_manager_a40978.Send_Global_Message(&packet, 0x3d, 1, &unknown_players_a3fa7c[player]->Address, 0, 0);
                }
            }
        }
    }
}


// 0x0042ebe0: function not yet identified
//
// Body: 56 bytes (frozen inventory bound).
// Referenced from 1 site (unconditional call).
// Ghidra prototype: void __thiscall FUN_0042ebe0(int param_1,int param_2)
//
// Types and names are Ghidra placeholders;
// rename and retype as the purpose becomes known.
//

namespace seed_0042ebe0 {

struct Callee_0042e990 { void FUN_0042e990(int row_index, int column_index); };

struct Owner_0042ebe0 { void FUN_0042ebe0(int row_index); };

void Owner_0042ebe0::FUN_0042ebe0(int row_index)
{
    int object_address = (int)this;
    int column;
    int *entry_words;

    if (*(int *)(object_address + 0x60) != 0) {
        column = (int)(0);
        entry_words = (int *)(object_address + row_index * 0xc);
        do {
            if (*entry_words != 0) {
                ((Callee_0042e990*)(object_address))->FUN_0042e990((int)(row_index), (int)(column));
            }
            column = (int)(column + 1);
            entry_words = (int *)(entry_words + 1);
        } while (column < 3);
    }
    return;
}

}  // namespace seed_0042ebe0
