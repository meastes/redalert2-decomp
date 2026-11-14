// dropship.cpp: the original translation unit re-filed as one source (Route A): the accepted
// bodies of its canonical compile group in address order, under the unit's own header
// state (its switches and includes in their original order). The module map
// manifests/original_target/modules/dropship.json carries the addresses and the rows that
// stay in their own files.


#define RA2_VECTOR_UNSIGNED_INDEX  // migration holdout: unit not yet re-rolled to VectorClass::operator[](int) (docs/HANDOFF.md)
#define RA2_WDT_MAP_LOAD_LAYOUT
#define RA2_SHAPE_BUTTON_SHAPE_DATA
#define RA2_XMOUSE_WRAPPERS
#define SHAPE_FILE_EMPTY_RECT unknown_static_rect
#include "shared/unknown_geometry.hpp"
#include "shared/unknown_static_rect.hpp"
#include "shared/unknown_loadout.hpp"
#define VECTOR_IMPLEMENT_VECTOR_ACCESSORS
#define VECTOR_IMPLEMENT_DYNAMIC_ACCESSORS
#include "shared/techno_type.hpp"
#include "shared/type_list.hpp"
#include "shared/id_source.hpp"
#define VECTOR_IMPLEMENT_DYNAMIC_ADD
#define VECTOR_IMPLEMENT_DYNAMIC_DELETE_INDEX
#define VECTOR_IMPLEMENT_DYNAMIC_ID_OBJECT
#define VECTOR_IMPLEMENT_DYNAMIC_CONSTRUCTOR
#include "shared/cc_file_class.hpp"
#include "shared/mix_file_class.hpp"
#include "shared/surface.hpp"
#include "shared/convert.hpp"
#include "shared/palette.hpp"
#include "shared/surface_blit.hpp"
#include "shared/gadget.hpp"
#include "shared/blit.hpp"
#include "shared/house.hpp"
#include "shared/house_type.hpp"
#include "shared/infantry_type.hpp"
#include "shared/unit_type.hpp"
#include "shared/unknown_globals.hpp"
#include "shared/mouse.hpp"
#include "shared/keyboard.hpp"
#include "shared/ms_engine.hpp"
#include "shared/system_timer.hpp"
#include "shared/cd_timer.hpp"
#include "shared/fast_math.hpp"
#include "shared/string_table.hpp"
#include "shared/gscreen.hpp"
#include "shared/hicolor.hpp"
#include "shared/crt_string.hpp"
#include "shared/text_print.hpp"
#include "shared/dsurface.hpp"


// Header-static dynamic initializers of the unit at 0x004A6720:  the
// shared/unknown_geometry.hpp constants and unknown_static_rect.hpp
// (0x004A6720-0x004A6906); the other
// drafts of this plan are emitted by these includes.


// Dynamic initializer (0x004A6750, _$E) of b from the header(s) included
// by HeaderStatics_diagonal_init_004a6720.cpp; no source of its own.


// Dynamic initializer (0x004A6770, _$E) of c from the header(s) included
// by HeaderStatics_diagonal_init_004a6720.cpp; no source of its own.


// Dynamic initializer (0x004A6790, _$E) of angle45 from the header(s) included
// by HeaderStatics_diagonal_init_004a6720.cpp; no source of its own.


// Dynamic initializer (0x004A67B0, _$E) of angle60 from the header(s) included
// by HeaderStatics_diagonal_init_004a6720.cpp; no source of its own.


// Dynamic initializer (0x004A67D0, _$E) of angle90 from the header(s) included
// by HeaderStatics_diagonal_init_004a6720.cpp; no source of its own.


// Dynamic initializer (0x004A67F0, _$E) of g from the header(s) included
// by HeaderStatics_diagonal_init_004a6720.cpp; no source of its own.


// Dynamic initializer (0x004A6830, _$E) of h from the header(s) included
// by HeaderStatics_diagonal_init_004a6720.cpp; no source of its own.


// Dynamic initializer (0x004A6860, _$E) of i from the header(s) included
// by HeaderStatics_diagonal_init_004a6720.cpp; no source of its own.


// Dynamic initializer (0x004A6890, _$E) of j from the header(s) included
// by HeaderStatics_diagonal_init_004a6720.cpp; no source of its own.


// Dynamic initializer (0x004A68C0, _$E) of k from the header(s) included
// by HeaderStatics_diagonal_init_004a6720.cpp; no source of its own.


// Dynamic initializer (0x004A68D0, _$E) of l from the header(s) included
// by HeaderStatics_diagonal_init_004a6720.cpp; no source of its own.


// Dynamic initializer (0x004A68F0, _$E) of static_rect from the header(s) included
// by HeaderStatics_diagonal_init_004a6720.cpp; no source of its own.


DropshipLoadoutClass::DropshipLoadoutClass()
    : Timer(0x7fffffff), unknown_0c(false), Count(0), Cost(0)
{
}


bool DropshipLoadoutClass::Add_Type_004A6940(TechnoTypeClass *type)
{
    if (Count < 5) {
        int index = Find_Type_Index_004A6B50(type);
        if (index == -1 || unknown_id_source_a3d290->AllowableUnitCounts[index]
            < unknown_id_source_a3d290->AllowableUnitMaximums[index]) {
            Types[Count++] = type;
            Cost += type->Raw_Cost();
            if (index != -1) ++unknown_id_source_a3d290->AllowableUnitCounts[index];
            return true;
        }
    }
    return false;
}


bool DropshipLoadoutClass::Remove_Index_004A6A10(int index)
{
    if (Count <= 0 || index < 0 || index >= Count) return false;
    int scenarioIndex = Find_Type_Index_004A6B50(Types[index]);
    if (scenarioIndex != -1) --unknown_id_source_a3d290->AllowableUnitCounts[scenarioIndex];
    Cost -= Types[index]->Raw_Cost();
    for (int slot = index; slot < Count - 1; ++slot) {
        Types[slot] = Types[slot + 1];
    }
    Types[--Count] = 0;
    return true;
}


void DropshipLoadoutClass::Clear_004A6AE0()
{
    unknown_0c = false;
    for (int count = Count; count > 0; --count) Remove_Index_004A6A10(0);
    Count = 0;
    Cost = 0;
}


void DropshipLoadoutClass::Swizzle_Types_004A6B10()
{
    for (int index = 0; index < Count; ++index) {
        Swizzler.Swizzle((void **)&Types[index]);
    }
}


TechnoTypeClass *DropshipLoadoutClass::Get_Type_004A6B40(int index)
{
    return Types[index];
}


int __fastcall DropshipLoadoutClass::Find_Type_Index_004A6B50(TechnoTypeClass *type)
{
    TypeList<TechnoTypeClass *> &units = unknown_id_source_a3d290->AllowableUnits;
    if (!units.Count()) return -1;
    for (int index = 0; index < units.Count(); ++index) {
        if (units[index] == type && unknown_id_source_a3d290->AllowableUnitMaximums[index] != -1) return index;
    }
    return -1;
}


class Leaf_4a6930 { public: void get(); };
void Leaf_4a6930::get() {  }


// The dropship loadout screen (0x004A6B90): TibSun's Dropship_Screen (dropship.cpp), with RA2's
// four green lights, the depth buffer standing in for TibSun's alpha buffer and the loadouts
// stored in the player house (+0x5560).

extern "C" __declspec(dllimport) unsigned long __stdcall timeGetTime();
void __fastcall Draw_Updated_Counter_004a95c0(int current, int previous, Surface *background,
    Point2D &position, wchar_t const *prefix, bool centered, void *convert, class Blitter const *blitter);
void __fastcall Draw_Type_Details_004a97b0(Surface *surface, ConvertClass *drawer, TechnoTypeClass *techtype,
    bool allowed);


struct ButtonFadeEffect
{
    unsigned int StartTime;
    int CandidateIndex;
    float Alpha;
    float Direction;
    bool StopAtLow;

    ButtonFadeEffect(unsigned int start_time, int candidate_index, float alpha, float direction, bool stop_at_low) :
        StartTime(start_time),
        CandidateIndex(candidate_index),
        Alpha(alpha),
        Direction(direction),
        StopAtLow(stop_at_low)
    {
    }
};

struct CrossDissolveEffect
{
    Rect FromRect;
    Rect ToRect;
    Rect DestRect;
    BSurface *FromSurface;
    BSurface *ToSurface;
    int Alpha;
    unsigned int StartTime;
    Blitter *FromBlitter;
    Blitter *ToBlitter;
    bool WasDrawn;

    CrossDissolveEffect(Rect const &from_rect, Rect const &to_rect, Rect const &dest_rect, BSurface *from_surface, BSurface *to_surface, Blitter *from_blitter, Blitter *to_blitter) :
        FromRect(from_rect),
        ToRect(to_rect),
        DestRect(dest_rect),
        FromSurface(from_surface),
        ToSurface(to_surface),
        Alpha(0),
        StartTime(::timeGetTime()),
        FromBlitter(from_blitter),
        ToBlitter(to_blitter),
        WasDrawn(false)
    {
    }
};

void __fastcall Dropship_Screen_004A6B90()
{
    int i;
    int j;
    int k;

    int dropship_count = unknown_id_source_a3d290->StartingDropships;
    int money = unknown_player_a35db4->Available_Money();
    unsigned int money_display = 0;

    static const char *_drop_btn_names[] = {
        "DROP0001.SHP",
        "DROP0002.SHP",
        "DROP0003.SHP",
    };

    static const char *_green_btn_names[] = {
        "DGREEN1.SHP",
        "DGREEN2.SHP",
        "DGREEN3.SHP",
        "DGREEN4.SHP",
    };

    static const int _cameo_count = 8;
    static const int _cameo_x = 491;
    static const int _cameo_y = 25;
    static const int _cameo_width = 64;
    static const int _cameo_height = 48;
    static const int _cameo_gap_x = 4;
    static const int _cameo_gap_y = 2;
    static const int _up_x = 527;
    static const int _up_y = 229;
    static const int _down_x = 557;
    static const int _down_y = 229;
    static const int _up_index = 8;
    static const int _down_index = 9;
    static const int _slot_index = 10;
    static const int _slot_width = 64;
    static const int _slot_gap_x = 2;
    static const int _slot_x = 53;
    static const int _slot_height = 48;
    static const int _minimum_cost = 10;
    static const int _light_rate = 100;
    static const int _light_x = 371;
    static const int _fade_rate = 10;
    static const int _fade_low = 63;
    static const int _fade_scale = 70;
    static const int _loadout_rate = 250;
    static const int _loadout_x = 45;
    static const int _loadout_y = 0;
    static const int _dissolve_scale = 30;
    static const int _dissolve_rate = 7;
    static const int _money_x = 500;
    static const int _money_y = 385;
    static const int _money_rate = 50;
    static const int _click_delay = 300;
    static const int _button_width = 64;
    static const int _pilot_rate = 150;
    static const float _pilot_chance = 0.02f;
    static const int _pilot_x = 284;
    static const int _pilot_y = 151;
    static const int _pilot_delay = 5;

    static const int _loadout_slot_ys[3][3][5] = {
        {
            {69, 69, 119, 119, 119},
            {0, 0, 0, 0, 0},
            {0, 0, 0, 0, 0}
        },
        {
            {69, 69, 119, 119, 119},
            {209, 209, 259, 259, 259},
            {0, 0, 0, 0, 0}
        },
        {
            {39, 39, 89, 89, 89},
            {159, 159, 209, 209, 209},
            {279, 279, 329, 329, 329}
        }
    };

    static const int _green_light_ys[] = {
        10, 60, 110, 160
    };

    ShapeFileStruct *green_shapes[4];
    for (i = 0; i < 4; ++i) {
        CCFileClass file(_green_btn_names[i]);
        unsigned char *shape_buf = new unsigned char[file.Size()];
        file.Read(shape_buf, file.Size());
        green_shapes[i] = (ShapeFileStruct *)shape_buf;
    }

    ShapeFileStruct *loadout_shape = (ShapeFileStruct *)MixFileClass::Retrieve("LOADOUT.SHP");
    ShapeFileStruct *pilotlight_shape = (ShapeFileStruct *)MixFileClass::Retrieve("PILOTLIT.SHP");
    ShapeFileStruct *no_cameo_shape = (ShapeFileStruct *)MixFileClass::Retrieve("XXICON.SHP");

    CCFileClass drop_shape_file(_drop_btn_names[dropship_count - 1]);
    unsigned char *drop_shape_buf = new unsigned char[drop_shape_file.Size()];
    drop_shape_file.Read(drop_shape_buf, drop_shape_file.Size());
    ShapeFileStruct *drop_shape = (ShapeFileStruct *)drop_shape_buf;

    BSurface *background_surface = new BSurface(drop_shape->Get_Width(), drop_shape->Get_Height(), 1, (void *)drop_shape->Get_Frame_Data(0));

    int drop_height = drop_shape->Get_Height();
    int drop_width = drop_shape->Get_Width();
    Point2D screen_origin((unknown_surface_839954->Get_Width() - drop_width) / 2, (unknown_surface_839954->Get_Height() - drop_height) / 2);

    PaletteClass palette_data;
    memmove(&palette_data, MixFileClass::Retrieve("DROPSHIP.PAL"), sizeof(palette_data));
    for (i = 0; i < 256; ++i) {
        palette_data[i] = RGBClass(palette_data[i].Red * 4, palette_data[i].Green * 4, palette_data[i].Blue * 4);
    }

    ConvertClass *drawer_dropship = new ConvertClass(palette_data, unknown_palette_837e10, unknown_surface_839950, 53, false);
    ConvertClass *drawer_dropship_alpha = new ConvertClass(palette_data, unknown_palette_837e10, unknown_surface_839950, 1, false);

    memmove(&palette_data, MixFileClass::Retrieve("CAMEO.PAL"), sizeof(palette_data));
    for (i = 0; i < 256; ++i) {
        palette_data[i] = RGBClass(palette_data[i].Red * 4, palette_data[i].Green * 4, palette_data[i].Blue * 4);
    }
    ConvertClass *drawer_cameo = new ConvertClass(palette_data, unknown_palette_837e10, unknown_surface_839950, 1, false);

    Blitter *plain_blitter = drawer_dropship->Select_Blitter_00485AA0(0);
    Blitter *cameo_alpha_blitter = drawer_cameo->Select_Blitter_00485AA0(0x40);
    Blitter *drop_alpha_blitter = drawer_dropship_alpha->Select_Blitter_00485AA0(0x40);

    unknown_surface_839954->Fill(0);

    DynamicVectorClass<TechnoTypeClass *> candidates;
    int type_count = InfantryTypes.Count() + UnitTypes.Count();

    unsigned int ownable_mask = 1 << HouseTypes.ID(unknown_player_a35db4->Type);

    if (unknown_id_source_a3d290->AllowableUnits.Count() > 0) {
        for (i = 0; i < unknown_id_source_a3d290->AllowableUnits.Count(); ++i) {
            TechnoTypeClass *techtype = unknown_id_source_a3d290->AllowableUnits[i];
            if ((techtype->Owner & ownable_mask) && unknown_id_source_a3d290->AllowableUnitMaximums[i] != 0) {
                candidates.Add(techtype);
            }
        }
    } else {
        for (k = 0; k < type_count; ++k) {
            TechnoTypeClass *techtype = 0;
            if (k < InfantryTypes.Count()) {
                techtype = (TechnoTypeClass *)InfantryTypes[k];
            } else {
                techtype = (TechnoTypeClass *)UnitTypes[k - InfantryTypes.Count()];
            }

            if (techtype->TechLevel > unknown_player_a35db4->unknown_11c) {
                continue;
            }
            if (techtype->Kind_Of() == 16 && techtype->Category == 1) {
                continue;
            }
            if (techtype->TechLevel == -1) {
                continue;
            }
            if (techtype->Raw_Cost() <= 10) {
                continue;
            }
            if ((techtype->Owner & ownable_mask) == 0) {
                continue;
            }

            candidates.Add(techtype);
        }
    }

    int cameo_top = 0;
    bool can_scroll_up = false;
    int button_count = 8 + 2 + dropship_count * 5;
    bool can_scroll_down = candidates.Count() > 8;
    ShapeButtonClass **buttons = new ShapeButtonClass *[button_count];
    ToggleClass *button_list = 0;

    int id = 1;
    for (i = 0; i < 8; ++i) {
        ShapeButtonClass *button = new ShapeButtonClass(id, i < candidates.Count() ? (ShapeFileStruct const *)candidates[i]->unknown_slot38() : no_cameo_shape, screen_origin.X + 491 + (64 + 4) * (i % 2), screen_origin.Y + 25 + (48 + 2) * (i / 2), 0, 0, true);
        button->ShapeDrawer = unknown_drawer_8323b0;
        buttons[i] = button;
        if (i == 0) {
            button_list = buttons[0];
        } else {
            button->Add(*button_list);
        }
        ++id;
    }

    ShapeButtonClass *up_button = new ShapeButtonClass(id, MixFileClass::Retrieve("DROPUP.SHP"), screen_origin.X + 527, screen_origin.Y + 229, 0, 0, false);
    up_button->ShapeDrawer = drawer_dropship;
    up_button->IsSticky = true;
    buttons[8] = up_button;
    up_button->Add(*button_list);
    ++id;

    ShapeButtonClass *down_button = new ShapeButtonClass(id, MixFileClass::Retrieve("DROPDOWN.SHP"), screen_origin.X + 557, screen_origin.Y + 229, 0, 0, false);
    down_button->ShapeDrawer = drawer_dropship;
    down_button->IsSticky = true;
    buttons[9] = down_button;
    down_button->Add(*button_list);
    ++id;

    for (i = 0; i < dropship_count; ++i) {
        for (j = 0; j < 5; ++j) {
            int xpos = j * (64 + 2);
            ShapeButtonClass *button = new ShapeButtonClass(id, 0, screen_origin.X + 53 + (xpos < 2 * (64 + 2) ? xpos : xpos - 2 * (64 + 2)), screen_origin.Y + _loadout_slot_ys[dropship_count - 1][i][j], 0, 0, false);
            button->ShapeDrawer = unknown_drawer_8323b0;
            buttons[id - 1] = button;
            button->Add(*button_list);
            ++id;
        }
    }

    button_list->Turn_On();

    bool full_redraw = true;
    bool accept = false;
    bool recent_click = false;

    unsigned int light_frame[4] = {(unsigned)-1, (unsigned)-1, (unsigned)-1, (unsigned)-1};
    unsigned int light_start[4] = {0, 0, 0, 0};

    bool force_light_redraw = false;

    DynamicVectorClass<ButtonFadeEffect *> button_fades;

    unsigned int screen_start_time = ::timeGetTime();
    unsigned int money_display_time = screen_start_time;
    unsigned int last_input_time = 0;
    unsigned int pilot_start_time = 0;
    int pilot_frame = -1;

    CDTimerClass<SystemTimerClass> pilot_timer = 5;
    int loadout_anim_frame = 0;

    DynamicVectorClass<CrossDissolveEffect *> cross_dissolves;
    bool force_info_redraw = false;

    TechnoTypeClass *last_hover_type = 0;
    int last_hover_slot = -1;
    int selected_count = 0;

    DynamicVectorClass<TechnoTypeClass *> selections;

    Rect alpha_area = AlphaBuffer->Area;
    Hide_Mouse();
    delete AlphaBuffer;
    AlphaBuffer = 0;
    AlphaBuffer = new ABuffer(Rect(0, 0, unknown_surface_839954->Get_Width(), unknown_surface_839954->Get_Height()));
    Show_Mouse();

    unknown_surface_83995c = unknown_surface_839954;
    WWKeyboard->Clear();

    while (!accept) {
        bool redraw = false;

        if (unknown_redraw_8523dc) {
            unknown_redraw_8523dc = false;
            full_redraw = true;
        }

        if (full_redraw) {
            full_redraw = false;
            redraw = true;
            CC_Draw_Shape(unknown_surface_839954, drawer_dropship, drop_shape, 0, Point2D(0, 0), Rect(screen_origin.X, screen_origin.Y, drop_shape->Get_Width(), drop_shape->Get_Height()), 0, 0, 0, 0, 1000, 0, 0, Point2D(0, 0));

            for (i = 0; i < button_count; ++i) {
                int usage_index = -1;
                if (i + cameo_top < candidates.Count()) {
                    usage_index = DropshipLoadoutClass::Find_Type_Index_004A6B50(candidates[cameo_top + i]);
                }

                if (i >= 8 || usage_index == -1 || unknown_id_source_a3d290->AllowableUnitCounts[usage_index] < unknown_id_source_a3d290->AllowableUnitMaximums[usage_index]) {
                    AlphaBuffer->Clear_Region_00411090(127, Rect(screen_origin.X + buttons[i]->X, screen_origin.Y + buttons[i]->Y, 64, 48));
                } else {
                    AlphaBuffer->Clear_Region_00411090(63, Rect(screen_origin.X + buttons[i]->X, screen_origin.Y + buttons[i]->Y, 64, 48));
                }
                buttons[i]->Draw_Me(0);
            }
        }

        unknown_481fb0();

        int input = button_list->Input();
        if (!recent_click && (input & 0x8000) != 0) {
            input = input & ~0x8000;
            last_input_time = ::timeGetTime();

            if (input <= 8 && selected_count < dropship_count * 5) {
                int candidate_index = cameo_top + input - 1;
                if (candidate_index >= 0 && candidate_index < candidates.Count()) {
                    TechnoTypeClass *techtype = candidates[cameo_top + input - 1];

                    int usage_index = DropshipLoadoutClass::Find_Type_Index_004A6B50(techtype);
                    if (money - techtype->Raw_Cost() >= 0 && (usage_index == -1 || unknown_id_source_a3d290->AllowableUnitCounts[usage_index] < unknown_id_source_a3d290->AllowableUnitMaximums[usage_index])) {
                        buttons[10 + selected_count]->Set_Shape((ShapeFileStruct const *)techtype->unknown_slot38(), 0, 0);

                        ShapeFileStruct const *cameo_shape = (ShapeFileStruct const *)techtype->unknown_slot38();
                        ShapeButtonClass *slot_button = buttons[10 + selected_count];
                        BSurface *cameo_surface = new BSurface(cameo_shape->Get_Width(), cameo_shape->Get_Height(), 1, (void *)cameo_shape->Get_Frame_Data(0));

                        CrossDissolveEffect *dissolve_effect =
                            new CrossDissolveEffect(Rect(slot_button->X - screen_origin.X, slot_button->Y - screen_origin.Y, slot_button->Width, slot_button->Height),
                                                    Rect(0, 0, cameo_shape->Get_Width(), cameo_shape->Get_Height()),
                                                    Rect(slot_button->X, slot_button->Y, slot_button->Width, slot_button->Height),
                                                    background_surface,
                                                    cameo_surface,
                                                    drop_alpha_blitter,
                                                    cameo_alpha_blitter);
                        cross_dissolves.Add(dissolve_effect);

                        if (usage_index != -1) {
                            unknown_id_source_a3d290->AllowableUnitCounts[usage_index]++;
                        }
                        selections.Add(techtype);

                        redraw = true;
                        selected_count++;
                        recent_click = true;

                        int light_row = (input - 1) / 2;
                        if (light_frame[light_row] == (unsigned int)-1) {
                            light_frame[light_row] = 0;
                            light_start[light_row] = ::timeGetTime();
                        }

                        force_light_redraw = true;
                        for (j = 0; ; ++j) {
                            if (j >= button_fades.Count()) {
                                ButtonFadeEffect *fade = new ButtonFadeEffect(::timeGetTime(), candidate_index, 127.0f, -1.0f, false);
                                if (usage_index != -1 && unknown_id_source_a3d290->AllowableUnitCounts[usage_index] >= unknown_id_source_a3d290->AllowableUnitMaximums[usage_index]) {
                                    force_info_redraw = true;
                                    fade->StopAtLow = true;
                                }
                                button_fades.Add(fade);
                                break;
                            }
                            if (button_fades[j]->CandidateIndex == candidate_index) {
                                break;
                            }
                        }

                        money -= techtype->Raw_Cost();
                    }
                }
            } else if ((input == 9 && can_scroll_up) || (input == 10 && can_scroll_down)) {
                if (input == 9) {
                    cameo_top -= 2;
                } else {
                    cameo_top += 2;
                }

                for (i = 0; i < 8; ++i) {
                    int candidate_index = cameo_top + i;
                    buttons[i]->Set_Shape(candidate_index < candidates.Count() ? (ShapeFileStruct const *)candidates[candidate_index]->unknown_slot38() : no_cameo_shape, 0, 0);

                    int usage_index = -1;
                    if (candidate_index < candidates.Count()) {
                        usage_index = DropshipLoadoutClass::Find_Type_Index_004A6B50(candidates[candidate_index]);
                    }
                    if (usage_index == -1 || unknown_id_source_a3d290->AllowableUnitCounts[usage_index] < unknown_id_source_a3d290->AllowableUnitMaximums[usage_index]) {
                        AlphaBuffer->Clear_Region_00411090(127, Rect(buttons[i]->X, buttons[i]->Y, 64, 48));
                    } else {
                        AlphaBuffer->Clear_Region_00411090(63, Rect(buttons[i]->X, buttons[i]->Y, 64, 48));
                    }
                    buttons[i]->Draw_Me(1);
                }

                buttons[input - 1]->IsPressed = false;
                buttons[input - 1]->Draw_Me(1);

                can_scroll_up = cameo_top != 0;
                redraw = true;
                can_scroll_down = cameo_top < candidates.Count() - 8;
            } else if (input >= 11 && input < dropship_count * 5 + 11) {
                int remove_index = input - 11;
                if (remove_index < selections.Count()) {
                    int usage_index = DropshipLoadoutClass::Find_Type_Index_004A6B50(selections[remove_index]);
                    if (usage_index != -1) {
                        if (unknown_id_source_a3d290->AllowableUnitCounts[usage_index] >= unknown_id_source_a3d290->AllowableUnitMaximums[usage_index]) {
                            for (j = 0; j < candidates.Count(); ++j) {
                                if (candidates[j] == selections[remove_index]) {
                                    ButtonFadeEffect *fade = new ButtonFadeEffect(::timeGetTime(), j, 127.0f, -1.0f, false);
                                    fade->Direction = 1.0f;
                                    fade->Alpha = 63.0f;
                                    button_fades.Add(fade);
                                    break;
                                }
                            }
                        }
                        unknown_id_source_a3d290->AllowableUnitCounts[usage_index]--;
                    }

                    money += selections[remove_index]->Raw_Cost();
                    selections.Delete(remove_index);
                    redraw = true;

                    for (i = 0; i < selected_count - remove_index - 1; ++i) {
                        ShapeButtonClass *dest_slot = buttons[input + i - 1];

                        ShapeFileStruct const *old_shape = (ShapeFileStruct const *)dest_slot->Get_Shape_Data();
                        BSurface *old_surface = new BSurface(old_shape->Get_Width(), old_shape->Get_Height(), 1, (void *)old_shape->Get_Frame_Data(0));

                        dest_slot->Set_Shape(buttons[input + i]->Get_Shape_Data(), 0, 0);

                        ShapeFileStruct const *new_shape = (ShapeFileStruct const *)dest_slot->Get_Shape_Data();
                        BSurface *new_surface = new BSurface(new_shape->Get_Width(), new_shape->Get_Height(), 1, (void *)new_shape->Get_Frame_Data(0));

                        CrossDissolveEffect *dissolve_effect =
                            new CrossDissolveEffect(Rect(0, 0, dest_slot->Width, dest_slot->Height),
                                                    Rect(0, 0, dest_slot->Width, dest_slot->Height),
                                                    Rect(dest_slot->X, dest_slot->Y, dest_slot->Width, dest_slot->Height),
                                                    old_surface,
                                                    new_surface,
                                                    cameo_alpha_blitter,
                                                    cameo_alpha_blitter);
                        cross_dissolves.Add(dissolve_effect);
                    }

                    ShapeButtonClass *last_slot = buttons[selected_count + 9];
                    ShapeFileStruct const *last_shape = (ShapeFileStruct const *)last_slot->Get_Shape_Data();
                    BSurface *last_surface = new BSurface(last_shape->Get_Width(), last_shape->Get_Height(), 1, (void *)last_shape->Get_Frame_Data(0));

                    CrossDissolveEffect *dissolve_effect =
                        new CrossDissolveEffect(Rect(0, 0, last_shape->Get_Width(), last_shape->Get_Height()),
                                                Rect(last_slot->X - screen_origin.X, last_slot->Y - screen_origin.Y, last_slot->Width, last_slot->Height),
                                                Rect(last_slot->X, last_slot->Y, last_slot->Width, last_slot->Height),
                                                last_surface,
                                                background_surface,
                                                cameo_alpha_blitter,
                                                drop_alpha_blitter);
                    cross_dissolves.Add(dissolve_effect);

                    last_slot->Set_Shape(0, 0, 0);
                    selected_count--;
                }
            }
        } else if (input == 32) {
            accept = true;
        }

        if (buttons[8]->IsPressed && can_scroll_up && !recent_click) {
            buttons[8]->Draw_Me(0);
            redraw = true;
        }
        if (buttons[9]->IsPressed && can_scroll_down && !recent_click) {
            buttons[9]->Draw_Me(0);
            redraw = true;
        }

        for (i = 0; i < 4; ++i) {
            if (light_frame[i] != (unsigned int)-1) {
                int frame = light_frame[i];
                int next_frame = (::timeGetTime() - light_start[i]) / 100;
                if (next_frame != frame || force_light_redraw) {
                    int light_y = screen_origin.Y + _green_light_ys[i];
                    int light_x = screen_origin.X + 371;
                    ShapeFileStruct *shape = green_shapes[i];
                    if (next_frame > shape->Get_Count()) {
                        unknown_blit_49f3a0(
                            *unknown_surface_839954,
                            drawer_dropship,
                            *background_surface,
                            Rect(light_x - screen_origin.X, light_y - screen_origin.Y, shape->Get_Width(), shape->Get_Height()),
                            RectPoint2D(0, 0),
                            Rect(light_x, light_y, shape->Get_Width(), shape->Get_Height()),
                            0,
                            plain_blitter, 0, 0, 1000, 0);
                        light_frame[i] = (unsigned int)-1;
                    } else {
                        CC_Draw_Shape(
                            unknown_surface_839954,
                            drawer_dropship,
                            shape,
                            frame,
                            Point2D(0, 0),
                            Rect(light_x, light_y, shape->Get_Width(), shape->Get_Height()),
                            0, 0, 0, 0, 1000, 0, 0, Point2D(0, 0));
                        light_frame[i] = next_frame;
                    }
                    redraw = true;
                }
            }
        }

        for (i = button_fades.Count() - 1; i >= 0; --i) {
            ButtonFadeEffect *effect = button_fades[i];
            int alpha;
            if (effect->Direction < 0.0f) {
                alpha = 127 - (10 * ::timeGetTime() - 10 * effect->StartTime) / 70;
                if (alpha < 63) {
                    alpha = 63;
                    effect->Direction = 1.0f;
                    effect->StartTime = ::timeGetTime();
                }
            } else {
                alpha = (10 * ::timeGetTime() - 10 * effect->StartTime) / 70 + 63;
                if (alpha > 127) {
                    alpha = 127;
                }
            }

            float falpha = (float)alpha;
            if (falpha != effect->Alpha) {
                effect->Alpha = falpha;
                if (effect->CandidateIndex >= cameo_top && effect->CandidateIndex <= cameo_top + 8) {
                    int screen_index = effect->CandidateIndex - cameo_top;
                    AlphaBuffer->Clear_Region_00411090((unsigned short)falpha, Rect(screen_origin.X + 491 + (64 + 4) * (screen_index % 2), screen_origin.Y + 25 + (48 + 2) * (screen_index / 2), 64, 48));
                    buttons[screen_index]->Draw_Me(1);
                    redraw = true;
                }

                if (effect->Alpha == 127.0f || (effect->StopAtLow && effect->Alpha == 63.0f)) {
                    delete effect;
                    button_fades.Delete(i);
                }
            }
        }

        if (cross_dissolves.Count() != 0) {
            int dissolve_index = 0;
            if (dissolve_index < cross_dissolves.Count()) {
                int preceding_count = 0;
                do {
                    CrossDissolveEffect *effect = cross_dissolves[dissolve_index];
                    bool overlap_drawn = false;
                    bool draw_background = true;

                    j = dissolve_index - 1;
                    for (int scan = preceding_count; scan >= 1; --scan) {
                        CrossDissolveEffect *earlier = cross_dissolves[j];
                        if (earlier->DestRect == effect->DestRect) {
                            draw_background = false;
                            overlap_drawn = overlap_drawn || earlier->WasDrawn;
                        }
                        --j;
                    }

                    int alpha = 255 < (7 * ::timeGetTime() - 7 * effect->StartTime) / 30 ? 255 : (7 * ::timeGetTime() - 7 * effect->StartTime) / 30;

                    if (alpha != effect->Alpha || overlap_drawn) {
                        effect->Alpha = alpha;
                        if (draw_background) {
                            unknown_blit_49f3a0(*unknown_surface_839954, drawer_dropship, *effect->FromSurface, effect->FromRect, RectPoint2D(0, 0), effect->DestRect, 0, effect->FromBlitter, 0, 0, 1000, 0);
                        }
                        AlphaBuffer->Clear_Region_00411090(effect->Alpha, effect->DestRect);
                        unknown_blit_49f3a0(*unknown_surface_839954, drawer_cameo, *effect->ToSurface, effect->ToRect, RectPoint2D(0, 0), effect->DestRect, 0, effect->ToBlitter, 0, 0, 1000, 0);
                        effect->WasDrawn = true;
                        redraw = true;
                    }

                    if (alpha == 255) {
                        if (effect->FromSurface != background_surface) {
                            delete effect->FromSurface;
                        }
                        if (effect->ToSurface != background_surface) {
                            delete effect->ToSurface;
                        }
                        delete effect;
                        cross_dissolves.Delete(dissolve_index);
                        --dissolve_index;
                        --preceding_count;
                    }

                    ++dissolve_index;
                    ++preceding_count;
                } while (dissolve_index < cross_dissolves.Count());
            }
        }

        recent_click = (::timeGetTime() - last_input_time) < 300;

        int loadout_count = loadout_shape->Get_Count();
        int next_loadout_frame = ((::timeGetTime() - screen_start_time) / 250) % loadout_count;
        if (next_loadout_frame != loadout_anim_frame) {
            loadout_anim_frame = next_loadout_frame;
            CC_Draw_Shape(unknown_surface_839954, drawer_dropship, loadout_shape, loadout_anim_frame, Point2D(0, 0), Rect(screen_origin + Point2D(45, 0), loadout_shape->Get_Width(), loadout_shape->Get_Height()), 0, 0, 0, 0, 1000, 0, 0, Point2D(0, 0));
            redraw = true;
        }

        unsigned int now = ::timeGetTime();
        if ((unsigned int)money != money_display) {
            unsigned int elapsed = now - money_display_time;
            if (elapsed >= 50) {
                int previous_display = money_display;
                double distance = (double)abs(money - (int)money_display);
                double elapsed_time = (double)elapsed;
                double step = Fast_Sqrt(distance) * elapsed_time * 0.04 < distance ? Fast_Sqrt(distance) * elapsed_time * 0.04 : distance;

                int delta = (int)step;
                if (money < (int)money_display) {
                    delta = -delta;
                }

                money_display += delta;
                Point2D money_point = screen_origin + Point2D(500, 385);
                Draw_Updated_Counter_004a95c0(money_display, previous_display, background_surface, money_point, StringTable::Load_String("TXT_CREDITS_COLON", 0, "D:\\Ra2\\Dropship.cpp", 873), false, drawer_dropship, plain_blitter);
                redraw = true;
                money_display_time = now;
            }
        } else {
            money_display_time = now;
        }

        if (pilot_frame >= 0) {
            int next_pilot_frame = (::timeGetTime() - pilot_start_time) / 150;
            if (next_pilot_frame != pilot_frame) {
                if (next_pilot_frame < pilotlight_shape->Get_Count()) {
                    pilot_frame = next_pilot_frame;
                    CC_Draw_Shape(unknown_surface_839954, drawer_dropship, pilotlight_shape, pilot_frame, Point2D(0, 0), Rect(screen_origin.X + 284, screen_origin.Y + 151, pilotlight_shape->Get_Width(), pilotlight_shape->Get_Height()), 0, 0, 0, 0, 1000, 0, 0, Point2D(0, 0));
                    redraw = true;
                } else {
                    pilot_frame = -1;
                    unknown_blit_49f3a0(*unknown_surface_839954, drawer_dropship, *background_surface, Rect(284, 151, pilotlight_shape->Get_Width(), pilotlight_shape->Get_Height()), RectPoint2D(0, 0), Rect(screen_origin.X + 284, screen_origin.Y + 151, pilotlight_shape->Get_Width(), pilotlight_shape->Get_Height()), 0, plain_blitter, 0, 0, 1000, 0);
                    pilot_timer = 60;
                    redraw = true;
                }
            }
        } else {
            if (pilot_timer == 0) {
                if (Random_Pick(0, 0x7FFFFFFE) / (double)0x7FFFFFFE < _pilot_chance) {
                    pilot_frame = 0;
                    pilot_start_time = ::timeGetTime();
                    CC_Draw_Shape(unknown_surface_839954, drawer_dropship, pilotlight_shape, 0, Point2D(0, 0), Rect(screen_origin.X + 284, screen_origin.Y + 151, pilotlight_shape->Get_Width(), pilotlight_shape->Get_Height()), 0, 0, 0, 0, 1000, 0, 0, Point2D(0, 0));
                    redraw = true;
                }

                pilot_timer = 5;
            }
        }

        Point2D mouse_pos = Get_Mouse_Point();
        int hover_slot;
        int previous_hover_slot;
        TechnoTypeClass *hover_type = 0;

        int grid_x = screen_origin.X + 491;
        int grid_y = screen_origin.Y + 25;
        if (mouse_pos.X >= grid_x && mouse_pos.X < grid_x + 2 * (64 + 4) && mouse_pos.Y >= grid_y && mouse_pos.Y < grid_y + 4 * (48 + 2)) {
            int col = (mouse_pos.X - screen_origin.X - 491) / (64 + 4);
            int row = (mouse_pos.Y - screen_origin.Y - 25) / (48 + 2);
            int left = screen_origin.X + (64 + 4) * col + 491;
            int top = screen_origin.Y + (48 + 2) * row + 25;
            if (mouse_pos.X >= left
                && mouse_pos.X < left + 64
                && mouse_pos.Y >= top
                && mouse_pos.Y < top + 48
                && cameo_top + col + 2 * row < candidates.Count()) {
                hover_type = candidates[cameo_top + col + 2 * row];
                previous_hover_slot = last_hover_slot;
                hover_slot = col + 2 * row;
                last_hover_slot = hover_slot;
            } else {
                hover_slot = -1;
                previous_hover_slot = last_hover_slot;
                last_hover_slot = -1;
            }
        } else {
            hover_slot = -1;
            previous_hover_slot = last_hover_slot;
            last_hover_slot = -1;
        }

        bool hover_allowed;
        int usage_index = DropshipLoadoutClass::Find_Type_Index_004A6B50(hover_type);
        if (usage_index != -1) {
            hover_allowed = unknown_id_source_a3d290->AllowableUnitCounts[usage_index] < unknown_id_source_a3d290->AllowableUnitMaximums[usage_index];
        } else {
            hover_allowed = true;
        }

        if (hover_type != last_hover_type || force_info_redraw) {
            last_hover_type = hover_type;
            Draw_Type_Details_004a97b0(background_surface, drawer_dropship, hover_type, hover_allowed);
            redraw = true;
        }

        force_light_redraw = false;
        if (redraw) {
            if (hover_slot != previous_hover_slot && previous_hover_slot != -1) {
                buttons[previous_hover_slot]->Draw_Me(1);
            }

            if (hover_slot != -1) {
                int color = hover_allowed ? Build_Hicolor_Pixel(0, 255, 0) : Build_Hicolor_Pixel(255, 0, 0);
                unknown_surface_839954->Draw_Rect(Rect(screen_origin.X + 491 + (64 + 4) * (hover_slot % 2), screen_origin.Y + 25 + (48 + 2) * (hover_slot / 2), 64, 48), color);
            }

            GScreenClass::Blit(true, unknown_surface_839954, 0);
        }
    }

    for (i = 0; i < selections.Count(); ++i) {
        ((DropshipLoadoutClass *)&unknown_player_a35db4->unknown_5560[i / 5])->Add_Type_004A6940(selections[i]);
    }

    delete drawer_dropship;
    delete drawer_cameo;
    delete drawer_dropship_alpha;
    delete background_surface;
    delete [] drop_shape_buf;
    for (i = 0; i < 4; ++i) {
        delete green_shapes[i];
    }
    for (i = 0; i < button_count; ++i) {
        delete buttons[i];
    }
    delete [] buttons;

    Hide_Mouse();

    delete AlphaBuffer;
    AlphaBuffer = 0;
    AlphaBuffer = new ABuffer(alpha_area);

    if (DepthBuffer) {
        delete DepthBuffer;
        DepthBuffer = 0;
    }
    DepthBuffer = new ZBuffer(alpha_area);

    Show_Mouse();
}


extern wchar_t counter_current_text_00852a68[];
extern wchar_t counter_previous_text_00852ab8[];
extern wchar_t const counter_text_format_007d61ec[];
extern char const counter_scheme_007d61e4[];
ColorScheme *__fastcall Fetch_Scheme_By_Name(char const *name,int kind);
void __fastcall unknown_blit_49f3a0(Surface &dest,void *convert,Surface &source,Rect const &sourcerect,
    RectPoint2D const &destpoint,Rect const &destclip,unsigned char const *unknown1, class Blitter const *unknown2,int unknown3,int unknown4,int unknown5,int unknown6);
// The original merge retains the inclusive right/bottom adjustment.
inline Rect Merge_Counter_Areas(Rect const &first,Rect const &second)
{
    Rect area=first;
    if(area.X>second.X) {
        area.Width+=area.X-second.X;
        area.X=second.X;
    }
    if(area.Y>second.Y) {
        area.Height+=area.Y-second.Y;
        area.Y=second.Y;
    }
    if(area.X+area.Width<second.X+second.Width)
        area.Width=second.X+second.Width-area.X+1;
    if(area.Y+area.Height<second.Y+second.Height)
        area.Height=second.Y+second.Height-area.Y+1;
    return area;
}
// Restore the union of old/new text bounds, then print the updated value.
void __fastcall Draw_Updated_Counter_004a95c0(int current,int previous,Surface *background,
    Point2D &position,wchar_t const *prefix,bool centered,void *convert, class Blitter const *blitter)
{
    unsigned style=0x46;
    if(centered) style=0x146;
    BitFontClass *font=Font_For_Style(style);
    swprintf(counter_current_text_00852a68,counter_text_format_007d61ec,prefix,current);
    swprintf(counter_previous_text_00852ab8,counter_text_format_007d61ec,prefix,previous);
    int current_x;
    int previous_x;
    if(centered) {
        current_x=position.X-font->String_Pixel_Width(counter_current_text_00852a68,0)/2;
        previous_x=position.X-font->String_Pixel_Width(counter_previous_text_00852ab8,0)/2;
    } else {
        current_x=position.X;
        previous_x=position.X;
    }
    Rect current_area(current_x,position.Y,110,15);
    Rect previous_area(previous_x,position.Y,110,15);
    Rect screen_area=Merge_Counter_Areas(current_area,previous_area);
    Rect area=screen_area;
    area.X-=(unknown_surface_839954->Get_Width()-background->Get_Width())/2;
    area.Y-=(unknown_surface_839954->Get_Height()-background->Get_Height())/2;
    unknown_blit_49f3a0(*unknown_surface_839954,convert,*background,area,RectPoint2D(0,0),screen_area,0,blitter,0,0,1000,0);
    Fancy_Text_Printf(counter_current_text_00852a68,unknown_surface_839954,
        &unknown_surface_839954->Get_Rect(),&position,
        Fetch_Scheme_By_Name(counter_scheme_007d61e4,1),0,style);
}


extern wchar_t const type_name_format_007d627c[];
extern wchar_t const type_cost_format_007d6268[];
extern wchar_t const type_cost_unavailable_007d6254[];
extern wchar_t const type_armament_format_007d6238[];
extern wchar_t const type_armament_none_007d6218[];
extern wchar_t const type_armor_format_007d6204[];
extern char const type_green_scheme_007d61fc[];
extern char const type_red_scheme_007d61f8[];
ColorScheme *__fastcall Fetch_Scheme_By_Name(char const *name,int kind);
void __fastcall unknown_blit_49f3a0(Surface &dest,void *convert,Surface &source,Rect const &sourcerect,
    RectPoint2D const &destpoint,Rect const &destclip,unsigned char const *unknown1, class Blitter const *unknown2,int unknown3,int unknown4,int unknown5,int unknown6);
void __fastcall Draw_Type_Details_004a97b0(Surface *background, ConvertClass *converter,
    TechnoTypeClass *type, bool show_cost)
{
    RectPoint2D offset;
    {
    Rect source_area = Rect(450,300,190,80);
    offset = RectPoint2D((unknown_surface_839954->Get_Width()-background->Get_Width())/2,
                   (unknown_surface_839954->Get_Height()-background->Get_Height())/2);
    RectPoint2D top_left = RectPoint2D(450,300);
    Rect clip = Rect(top_left+offset,190,80);
    RectPoint2D zero(0,0);
    // Preserve the existing unresolved blit prototype's 32-bit payload spelling.
    unknown_blit_49f3a0(*unknown_surface_839954,converter,*background,source_area,
        zero,clip,0,converter->Select_Blitter_00485AA0(0),0,0,1000,0);
    }
    if (type) {
        wchar_t name[40];
        wchar_t cost[20];
        wchar_t armament[40];
        wchar_t armor[30];
        swprintf(name,type_name_format_007d627c,(char const *)type->Name);
        if (show_cost) swprintf(cost,type_cost_format_007d6268,type->Raw_Cost());
        else swprintf(cost,type_cost_unavailable_007d6254);
        if (type->Get_Weapon(0)->WeaponType)
            swprintf(armament,type_armament_format_007d6238,(char const *)type->Get_Weapon(0)->WeaponType->Name);
        else swprintf(armament,type_armament_none_007d6218);
        swprintf(armor,type_armor_format_007d6204,ArmorNames[type->unknown_9c]);
        ColorScheme *scheme;
        if(show_cost) scheme=Fetch_Scheme_By_Name(type_green_scheme_007d61fc,1);
        else scheme=Fetch_Scheme_By_Name(type_red_scheme_007d61f8,1);
        {
            Point2D position = Point2D(offset.X+450,offset.Y+300);
            Fancy_Text_Printf(name,unknown_surface_839954,&unknown_surface_839954->Get_Rect(),
                &position,scheme,0,0x46);
        }
        {
            Point2D position = Point2D(offset.X+450,offset.Y+315);
            Fancy_Text_Printf(armament,unknown_surface_839954,&unknown_surface_839954->Get_Rect(),
                &position,scheme,0,0x46);
        }
        {
            Point2D position = Point2D(offset.X+450,offset.Y+330);
            Fancy_Text_Printf(armor,unknown_surface_839954,&unknown_surface_839954->Get_Rect(),
                &position,scheme,0,0x46);
        }
        {
            Point2D position = Point2D(offset.X+450,offset.Y+345);
            Fancy_Text_Printf(cost,unknown_surface_839954,&unknown_surface_839954->Get_Rect(),
                &position,scheme,0,0x46);
        }
    }
}


extern "C" void __cdecl release_4a9b00(void*);
class Cleanup_4a9b00 { unsigned char pad0[4]; unsigned int* items; int count; unsigned char pad12[1]; unsigned char owns; unsigned char pad14[2]; int capacity; public: void get(); };
void Cleanup_4a9b00::get() { unsigned int* pointer=items; capacity=0; if(pointer && owns) { release_4a9b00(pointer); items=0; } owns=0; count=0; }


extern "C" void __cdecl release_4a9b60(void*);
class Cleanup_4a9b60 { unsigned char pad0[4]; unsigned int* items; int count; unsigned char pad12[1]; unsigned char owns; unsigned char pad14[2]; int capacity; public: void get(); };
void Cleanup_4a9b60::get() { unsigned int* pointer=items; capacity=0; if(pointer && owns) { release_4a9b60(pointer); items=0; } owns=0; count=0; }


extern "C" unsigned char vtable_4aa080[]; extern "C" void __cdecl release_buffer_4aa080(void*); extern "C" void __cdecl release_object_4aa080(void*);
class Leaf_4aa080 { public: void* get(unsigned int flags); };
void* Leaf_4aa080::get(unsigned int flags) { unsigned int pointer = *(unsigned int*)((char*)this + 4); *(unsigned int*)this=(unsigned int)vtable_4aa080; if(pointer && *(unsigned char*)((char*)this + 13)){ release_buffer_4aa080((void*)pointer); *(unsigned int*)((char*)this + 4)=0; } *(unsigned char*)((char*)this + 13)=0; *(unsigned int*)((char*)this + 8)=0; if(flags & 1) release_object_4aa080(this); return this; }


extern "C" unsigned char vtable_4aa0d0[]; extern "C" void __cdecl release_buffer_4aa0d0(void*); extern "C" void __cdecl release_object_4aa0d0(void*);
class Leaf_4aa0d0 { public: void* get(unsigned int flags); };
void* Leaf_4aa0d0::get(unsigned int flags) { unsigned int pointer = *(unsigned int*)((char*)this + 4); *(unsigned int*)this=(unsigned int)vtable_4aa0d0; if(pointer && *(unsigned char*)((char*)this + 13)){ release_buffer_4aa0d0((void*)pointer); *(unsigned int*)((char*)this + 4)=0; } *(unsigned char*)((char*)this + 13)=0; *(unsigned int*)((char*)this + 8)=0; if(flags & 1) release_object_4aa0d0(this); return this; }


extern "C" unsigned char vtable_4aa120[]; extern "C" void __cdecl release_buffer_4aa120(void*); extern "C" void __cdecl release_object_4aa120(void*);
class Leaf_4aa120 { public: void* get(unsigned int flags); };
void* Leaf_4aa120::get(unsigned int flags) { unsigned int pointer = *(unsigned int*)((char*)this + 4); *(unsigned int*)this=(unsigned int)vtable_4aa120; if(pointer && *(unsigned char*)((char*)this + 13)){ release_buffer_4aa120((void*)pointer); *(unsigned int*)((char*)this + 4)=0; } *(unsigned char*)((char*)this + 13)=0; *(unsigned int*)((char*)this + 8)=0; if(flags & 1) release_object_4aa120(this); return this; }


extern "C" unsigned char vtable_4aa170[]; extern "C" void __cdecl release_buffer_4aa170(void*); extern "C" void __cdecl release_object_4aa170(void*);
class Leaf_4aa170 { public: void* get(unsigned int flags); };
void* Leaf_4aa170::get(unsigned int flags) { unsigned int pointer = *(unsigned int*)((char*)this + 4); *(unsigned int*)this=(unsigned int)vtable_4aa170; if(pointer && *(unsigned char*)((char*)this + 13)){ release_buffer_4aa170((void*)pointer); *(unsigned int*)((char*)this + 4)=0; } *(unsigned char*)((char*)this + 13)=0; *(unsigned int*)((char*)this + 8)=0; if(flags & 1) release_object_4aa170(this); return this; }


extern "C" unsigned char vtable_4aa1c0[]; extern "C" void __cdecl release_buffer_4aa1c0(void*); extern "C" void __cdecl release_object_4aa1c0(void*);
class Leaf_4aa1c0 { public: void* get(unsigned int flags); };
void* Leaf_4aa1c0::get(unsigned int flags) { unsigned int pointer = *(unsigned int*)((char*)this + 4); *(unsigned int*)this=(unsigned int)vtable_4aa1c0; if(pointer && *(unsigned char*)((char*)this + 13)){ release_buffer_4aa1c0((void*)pointer); *(unsigned int*)((char*)this + 4)=0; } *(unsigned char*)((char*)this + 13)=0; *(unsigned int*)((char*)this + 8)=0; if(flags & 1) release_object_4aa1c0(this); return this; }


extern "C" unsigned char table_4aa210[]; extern "C" void* __cdecl storage_4aa210(unsigned int);
class Storage_4aa210 { unsigned int table_word; unsigned int* items; unsigned int count; unsigned char valid; unsigned char owns; public: void* get(unsigned int requested,unsigned int* buffer); };
void* Storage_4aa210::get(unsigned int requested,unsigned int* buffer) { items=0; count=requested; valid=1; owns=0; table_word=(unsigned int)table_4aa210; if(requested) { if(buffer) { items=buffer; return this; } items=(unsigned int*)storage_4aa210(requested << 2); owns=1; } return this; }


extern "C" unsigned char table_4aa260[]; extern "C" void* __cdecl storage_4aa260(unsigned int);
class Storage_4aa260 { unsigned int table_word; unsigned int* items; unsigned int count; unsigned char valid; unsigned char owns; public: void* get(unsigned int requested,unsigned int* buffer); };
void* Storage_4aa260::get(unsigned int requested,unsigned int* buffer) { items=0; count=requested; valid=1; owns=0; table_word=(unsigned int)table_4aa260; if(requested) { if(buffer) { items=buffer; return this; } items=(unsigned int*)storage_4aa260(requested << 2); owns=1; } return this; }
