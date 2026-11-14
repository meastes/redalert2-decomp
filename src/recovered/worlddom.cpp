// worlddom.cpp: the original translation unit re-filed as one source (Route A): the accepted
// bodies of its canonical compile group in address order, under the unit's own header
// state (its switches and includes in their original order). The module map
// manifests/original_target/modules/worlddom.json carries the addresses and the rows that
// stay in their own files.


#define VECTOR_IMPLEMENT_VECTOR_ACCESSORS
#include "shared/shape_file.hpp"
#include "shared/unknown_geometry.hpp"
#include "shared/wdt_voices.hpp"
#include "shared/wstring.hpp"
#include "shared/world_domination_tour.hpp"
#include "shared/wdt_faction_select.hpp"
#include "shared/surface.hpp"
#include "shared/rect.hpp"
#include "shared/win32_api.hpp"
#include "shared/unknown_globals.hpp"
#include "shared/campaign_score.hpp"
#include "shared/ms_message_box.hpp"
#include "shared/skirmish.hpp"
#include "shared/string_table.hpp"
#include "shared/wide_string.hpp"
#include "shared/cc_file_class.hpp"
#include "shared/ms_anim.hpp"
#include "shared/wdt_selection.hpp"
#include "shared/wdt_campaign_layout.hpp"
#include "shared/mix_file_class.hpp"
#include "shared/cd.hpp"
#include "shared/lobby.hpp"
#include "shared/main_opt.hpp"
#include "shared/options.hpp"
#include "shared/play_movie.hpp"
#include "shared/unknown_map_generator.hpp"
#include "shared/crt_string.hpp"
#include "shared/wol_sink.hpp"
#include "shared/reference_counted.hpp"


// Header-static dynamic initializers of the unit at 0x007683B0: the shape_file.hpp empty rect and the
// shared/unknown_geometry.hpp constants (0x007683B0-0x00768591); the other
// drafts of this plan are emitted by these includes. The unit sees the inline VectorClass accessors
// (Get_WDT_Territory_00769690 expands operator[]).


// Dynamic initializer (0x007683D0, _$E) of diagonal from the header(s) included
// by HeaderStatics_empty_rect_init_007683b0.cpp; no source of its own.


// Dynamic initializer (0x00768400, _$E) of b from the header(s) included
// by HeaderStatics_empty_rect_init_007683b0.cpp; no source of its own.


// Dynamic initializer (0x00768420, _$E) of c from the header(s) included
// by HeaderStatics_empty_rect_init_007683b0.cpp; no source of its own.


// Dynamic initializer (0x00768440, _$E) of angle45 from the header(s) included
// by HeaderStatics_empty_rect_init_007683b0.cpp; no source of its own.


// Dynamic initializer (0x00768460, _$E) of angle60 from the header(s) included
// by HeaderStatics_empty_rect_init_007683b0.cpp; no source of its own.


// Dynamic initializer (0x00768480, _$E) of angle90 from the header(s) included
// by HeaderStatics_empty_rect_init_007683b0.cpp; no source of its own.


// Dynamic initializer (0x007684A0, _$E) of g from the header(s) included
// by HeaderStatics_empty_rect_init_007683b0.cpp; no source of its own.


// Dynamic initializer (0x007684E0, _$E) of h from the header(s) included
// by HeaderStatics_empty_rect_init_007683b0.cpp; no source of its own.


// Dynamic initializer (0x00768510, _$E) of i from the header(s) included
// by HeaderStatics_empty_rect_init_007683b0.cpp; no source of its own.


// Dynamic initializer (0x00768540, _$E) of j from the header(s) included
// by HeaderStatics_empty_rect_init_007683b0.cpp; no source of its own.


// Dynamic initializer (0x00768570, _$E) of k from the header(s) included
// by HeaderStatics_empty_rect_init_007683b0.cpp; no source of its own.


// Dynamic initializer (0x00768580, _$E) of l from the header(s) included
// by HeaderStatics_empty_rect_init_007683b0.cpp; no source of its own.


// Wstring const &__fastcall Faction_Name(int faction) -- 0x007685A0
// The faction name of a faction number ("DIS", "ALL", "SOV", else "xx").

// Never inlined by the original (Theater_Choice_Menu 0x00768DD0 calls it).
#pragma auto_inline(off)
Wstring const &__fastcall Faction_Name(int faction)
{
    static Wstring dis("DIS");
    static Wstring all("ALL");
    static Wstring sov("SOV");
    switch (faction) {
    case 1:
        return dis;
    case 2:
        return all;
    case 3:
        return sov;
    default:
        static Wstring unknown("xx");
        return unknown;
    }
}
#pragma auto_inline(on)


// Atexit destructor thunk (0x00768690, _$E51) of the function static unknown in Faction_Name;
// emitted by the Faction_Name draft; no source of its own.


// Atexit destructor thunk (0x007686A0, _$E50) of the function static sov in Faction_Name;
// emitted by the Faction_Name draft; no source of its own.


// Atexit destructor thunk (0x007686B0, _$E49) of the function static all in Faction_Name;
// emitted by the Faction_Name draft; no source of its own.


// Atexit destructor thunk (0x007686C0, _$E48) of the function static dis in Faction_Name;
// emitted by the Faction_Name draft; no source of its own.


// Wstring const &__fastcall Faction_Code(int faction) -- 0x007686D0
// The two-letter code of a faction ("NA", "EU", "AS", else "XX").

Wstring const &__fastcall Faction_Code(int faction)
{
    static Wstring na("NA");
    static Wstring eu("EU");
    static Wstring as("AS");
    switch (faction) {
    case 1:
        return na;
    case 2:
        return eu;
    case 3:
        return as;
    default:
        static Wstring unknown("XX");
        return unknown;
    }
}


// Atexit destructor thunk (0x007687C0, _$E56) of the function static unknown in Faction_Code;
// emitted by the Faction_Code draft; no source of its own.


// Atexit destructor thunk (0x007687D0, _$E55) of the function static as in Faction_Code;
// emitted by the Faction_Code draft; no source of its own.


// Atexit destructor thunk (0x007687E0, _$E54) of the function static eu in Faction_Code;
// emitted by the Faction_Code draft; no source of its own.


// Atexit destructor thunk (0x007687F0, _$E53) of the function static na in Faction_Code;
// emitted by the Faction_Code draft; no source of its own.


// 0x00769730: function not yet identified
//
// Body: 3 bytes (frozen inventory bound).
// Referenced from 0 sites.
// Ghidra prototype: undefined1 unknown_return_true_00769730(void)
//
// Types and names are Ghidra placeholders;
// rename and retype as the purpose becomes known.
//

namespace seed_00769730 {

unsigned char  unknown_return_true_00769730(void)
{
    return 1;
}

}  // namespace seed_00769730


// WorldDominationTour::FactionSelectDialogControl::~FactionSelectDialogControl() -- 0x00768800
// Deletes the three faction images; the deleting destructor 0x00769740 inlines it.

namespace WorldDominationTour {

FactionSelectDialogControl::~FactionSelectDialogControl()
{
    delete NoneImage;
    delete AlliedImage;
    delete SovietImage;
}

}


// void WorldDominationTour::FactionSelectDialogControl::Mouse_Move_00768840(HWND window, Point2D const &point)
// (0x00768840): picks the image and selection under the point (the allied and soviet map areas) and
// repaints the window when the selection changes.

namespace WorldDominationTour {

void FactionSelectDialogControl::Mouse_Move_00768840(HWND window, Point2D const &point)
{
    if (unknown_slot2()) {
        return;
    }
    Current = NoneImage;
    int selection = 0;
    if (Rect(16, 20, 348, 116).Is_Point_Within(point) || Rect(130, 100, 170, 116).Is_Point_Within(point) || Rect(175, 216, 70, 46).Is_Point_Within(point)) {
        Current = AlliedImage;
        selection = 2;
    } else if (Rect(252, 180, 214, 202).Is_Point_Within(point) || Rect(212, 262, 284, 160).Is_Point_Within(point)) {
        Current = SovietImage;
        selection = 3;
    }
    if (Selection != selection) {
        Selection = selection;
        InvalidateRect(window, 0, FALSE);
    }
}

}


// void WorldDominationTour::FactionSelectDialogControl::Draw_00768940(int unknown) -- 0x00768940
// Clears the current image's area of the background surface and copies the image there.

namespace WorldDominationTour {

void FactionSelectDialogControl::Draw_00768940(int unknown)
{
    if (unknown_surface_839958 != 0 && Current != 0) {
        Rect rect = Current->Get_Rect();
        unknown_surface_839958->Fill_Rect(rect, 0);
        unknown_surface_839958->Copy_From(rect, *Current, rect, false, true);
    }
}

}


// int __stdcall Faction_Select_Dialog_Proc_007689c0(HWND window, UINT message, WPARAM wparam, LPARAM lparam)
// (0x007689C0): the World Domination Tour faction dialog's procedure. WM_INITDIALOG keeps the
// FactionSelectDialogControl as the DWL_USER value; then the common handler 0x005FFB00. The control
// tracks the cursor (0x84), paints its image, ends the dialog on button 0x686 (-1) or a click on a
// faction (1), and answers the tool tip request (0x4E9) for the faction under the cursor.


int __stdcall Faction_Select_Dialog_Proc_007689c0(HWND window, UINT message, WPARAM wparam, LPARAM lparam)
{
    if (message == 0x110 && lparam != 0) {
        SetWindowLongA(window, 8, ((UnknownDialogCreateParam *)lparam)->Param);
    }
    int result = unknown_dialog_proc_5ffb00(window, message, wparam, lparam);
    if (result != 0) {
        return result;
    }
    WorldDominationTour::FactionSelectDialogControl *control = (WorldDominationTour::FactionSelectDialogControl *)GetWindowLongA(window, 8);
    switch (message) {
        case 0x84: {
            Point2D point(lparam & 0xFFFF, (unsigned int)lparam >> 16);
            control->Mouse_Move_00768840(window, point);
            return 0;
        }
        case 0x2B:
            Unknown_005FE380(lparam);
            return 1;
        case 0x0F:
            Unknown_005FEE70(window);
            control->Draw_00768940(0);
            unknown_dialog_5e5f20(window);
            ValidateRect(window, 0);
            return 1;
        case 0x14:
            return 1;
        case 0x111:
            if ((unsigned short)wparam == 0x686) {
                control->Set_Window_006010f0((unsigned int)window, (HWND)-1);
                return 0;
            }
            break;
        case 0x497:
            return 0;
        case 0x4E9: {
            ToolTipTextRequest *request = (ToolTipTextRequest *)lparam;
            if (request->Window == window) {
                wchar_t const *text;
                switch (control->Selection) {
                    case 2:
                        text = StringTable::Load_String("WDT:FactionSelectAlliesToolTip", 0, "D:\\Ra2\\WorldDom.cpp", 176);
                        break;
                    case 3:
                        text = StringTable::Load_String("WDT:FactionSelectSovietToolTip", 0, "D:\\Ra2\\WorldDom.cpp", 179);
                        break;
                    default:
                        text = StringTable::Load_String("WDT:FactionSelectToolTip", 0, "D:\\Ra2\\WorldDom.cpp", 182);
                        break;
                }
                WideString string(text);
                request->Text = string;
            }
            return 0;
        }
        case 0x202:
            if (control->Selection != 0) {
                control->Set_Window_006010f0((unsigned int)window, (HWND)1);
            }
            break;
    }
    return 0;
}


// int __fastcall Faction_Select_Dialog_00768c70() -- 0x00768C70
// Runs the World Domination faction choice dialog 0x125 with a FactionSelectDialogControl (whose inline
// constructor loads the three small faction images; out of line at 0x00769790) and returns its selection.

int __fastcall Run_Control_Dialog_006011a0(int id, DLGPROC procedure, OwnerDraw::DialogControl *control);

namespace WorldDominationTour {

// Loads one image file into image (the constructor skips images already set).
static void Load_Image(Surface *&image, char const *filename)
{
    CCFileClass file(filename);
    image = unknown_load_pcx_60d110(&file, 0, 0, 0);
}

inline FactionSelectDialogControl::FactionSelectDialogControl() :
    Selection(0),
    NoneImage(0),
    AlliedImage(0),
    SovietImage(0)
{
    if (NoneImage == 0) {
        Load_Image(NoneImage, "WDTFSBSm.pcx");
    }
    if (AlliedImage == 0) {
        Load_Image(AlliedImage, "WDTFSAsm.pcx");
    }
    if (SovietImage == 0) {
        Load_Image(SovietImage, "WDTFSSsm.pcx");
    }
    Current = NoneImage;
}

}

using namespace WorldDominationTour;

int __stdcall Faction_Select_Dialog_Proc_007689c0(HWND window, UINT message, WPARAM wparam, LPARAM lparam);

int __fastcall Faction_Select_Dialog_00768c70()
{
    unknown_5159b0(0, true);
    FactionSelectDialogControl control;
    Run_Control_Dialog_006011a0(0x125, Faction_Select_Dialog_Proc_007689c0, &control);
    return control.Selection;
}


// int __fastcall Theater_Choice_Menu(int faction) (0x00768DD0): runs the WDTChoice.ini
// menu section "TheaterChoice<faction name>Menu" and returns its choice 1-3 (0 for none or
// cancel). Names are hypotheses; faction stays in ecx for Faction_Name. The menu type and
// its loader 0x004DE260 / run 0x004DE9E0 are declared here until their unit is recovered.

Wstring const &__fastcall Faction_Name(int faction);

class GraphicMenu {
public:
    virtual ~GraphicMenu();
    static GraphicMenu *__fastcall Load(Wstring const &file, Wstring const &section);
    int Run();
};

int __fastcall Theater_Choice_Menu(int faction)
{
    char buffer[128];
    sprintf(buffer, "TheaterChoice%sMenu", Faction_Name(faction));
    GraphicMenu *menu = GraphicMenu::Load("WDTChoice.ini", buffer);
    if (!menu) {
        return 0;
    }
    int choice = menu->Run();
    delete menu;
    switch (choice) {
        case 1: return 1;
        case 2: return 2;
        case 3: return 3;
    }
    return 0;
}


// int __fastcall WDT_Setup_Game_00768e90(bool first_time) (0x00768E90): sets up a World Domination
// Tour game (TibSun WDT_Setup_Game). Offers to drop to 640x480, mounts WDT.MIX, WDTVOX.MIX and
// Local.MIX, then runs the tour states until a mission is ready (6) or the player leaves (0):
// 1 faction choice, 2 theater movie, 3 theater choice, 4 cycle request and conflict choice,
// 5 the server's answer. Names are hypotheses.

using namespace WorldDominationTour;

int __fastcall Faction_Select_Dialog_00768c70();
int __fastcall Theater_Choice_Menu(int faction);
bool __fastcall Request_WDT_Cycle_00751df0();
int __fastcall Unknown_00759e20();

extern int WDT_Chosen_Side_b2ada4;
extern int WDT_Chosen_Theater_b2ada8;
extern int wol_max_players_b2abdc;
rc_ptr<Campaign> const &__fastcall Current_WDT_Campaign_00769670();
rc_ptr<Campaign> const &__fastcall Previous_WDT_Campaign_00769680();

int __fastcall WDT_Setup_Game_00768e90(bool first_time)
{
    if (unknown_8523e0 > 640) {
        WideString text(StringTable::Load_String("WOL:WDTResolutionQuery", 0, "D:\\Ra2\\WorldDom.cpp", 385));
        if (Message_Box_005c3230(text.Get(), 4, Unknown_0073e100, true) != 6) {
            return false;
        }
        if (!Test_Display_Mode(640, 480)) {
            return false;
        }
        Options.ScreenWidth = 640;
        Options.ScreenHeight = 480;
        Options.Save_Settings();
    }

    MixFileClass *wdtmix = 0;
    MixFileClass *wdtvoxmix = 0;
    MixFileClass *localmix = 0;

    if (CCFileClass("WDT.MIX").Is_Available()) {
        wdtmix = new MixFileClass("WDT.MIX", &FastKey);
    }
    if (CCFileClass("WDTVOX.MIX").Is_Available()) {
        wdtvoxmix = new MixFileClass("WDTVOX.MIX", &FastKey);
    }
    if (CCFileClass("Local.MIX").Is_Available()) {
        localmix = new MixFileClass("Local.MIX", &FastKey);
    }

    if (wdtmix != 0 && localmix != 0) {
        int state = first_time ? 4 : 1;
        while (state != 0 && state != 6) {
            switch (state) {
                case 1:
                    WDT_Chosen_Side_b2ada4 = Faction_Select_Dialog_00768c70();
                    state = WDT_Chosen_Side_b2ada4 != 0 ? 4 : 0;
                    break;
                case 2: {
                    char name[128];
                    sprintf(name, "WDTTheater%02.VQA", WDT_Chosen_Side_b2ada4);
                    if (CCFileClass(name).Is_Available()) {
                        Play_Movie(name, -1, false, true, false, false);
                    }
                    state = 3;
                    break;
                }
                case 3:
                    WDT_Chosen_Theater_b2ada8 = Theater_Choice_Menu(WDT_Chosen_Side_b2ada4);
                    state = WDT_Chosen_Theater_b2ada8 != 0 ? 4 : 1;
                    break;
                case 4: {
                    if (!Request_WDT_Cycle_00751df0()) {
                        state = 0;
                        break;
                    }
                    rc_ptr<Campaign> campaign = Current_WDT_Campaign_00769670();
                    if (campaign.Get() == 0) {
                        return false;
                    }
                    campaign->Set_Player_Faction(WDT_Chosen_Side_b2ada4);
                    if (campaign->ID_Differs()) {
                        rc_ptr<Campaign> previous = Previous_WDT_Campaign_00769680();
                        if (previous.Get() != 0 && !previous->ID_Differs()) {
                            Selection::Show(previous, WDT_Chosen_Theater_b2ada8);
                        }
                    }
                    unknown_a3d3bc = WDT_Chosen_Side_b2ada4 == 3;
                    if (Selection::Choose_Conflict(campaign, WDT_Chosen_Theater_b2ada8)) {
                        wol_max_players_b2abdc = 0;
                        RandomMapGen.SeedData.Apply_WDT_Territory_00578bc0();
                        state = 5;
                    } else {
                        state = 1;
                    }
                    break;
                }
                case 5:
                    if (Unknown_00759e20() == 1) {
                        state = 6;
                    } else {
                        state = 4;
                    }
                    break;
            }
        }
        delete wdtmix;
        delete localmix;
        delete wdtvoxmix;
        return state == 6;
    }

    delete wdtmix;
    delete localmix;
    delete wdtvoxmix;
    return false;
}


// 0x00769390: function not yet identified
//
// Body: 22 bytes (frozen inventory bound).
// Referenced from 1 site (data).
// Ghidra prototype: void unknown_init_block_00b2ad60_00769390(void)
// Callees with accepted source:
//   unknown_free_block_00b2ad60_007693b0 -> src/recovered/007693b0.cpp
//   atexit_0078298a -> src/recovered/0078298a.cpp
//
// Types and names are Ghidra placeholders;
// rename and retype as the purpose becomes known.
//

namespace seed_00769390 {

extern void __cdecl unknown_free_block_00b2ad60_007693b0(void);
extern unsigned int  atexit_0078298a(unsigned int);
extern "C" unsigned int unknown_wdt_block_00b2ad60;

void  unknown_init_block_00b2ad60_00769390(void)
{
    unknown_wdt_block_00b2ad60 = 0;
    atexit_0078298a((unsigned int)(unknown_free_block_00b2ad60_007693b0));
    return;
}

}  // namespace seed_00769390


// 0x007693b0: function not yet identified
//
// Body: 29 bytes (frozen inventory bound).
// Referenced from 1 site (data).
// Ghidra prototype: void unknown_free_block_00b2ad60_007693b0(void)
// Callees with accepted source:
//   operator_delete_00781d3b -> src/recovered/00781d3b.cpp
//
// Types and names are Ghidra placeholders;
// rename and retype as the purpose becomes known.
//

namespace seed_007693b0 {

extern void  operator_delete_00781d3b(unsigned int);
extern "C" unsigned int unknown_wdt_block_00b2ad60;

void  unknown_free_block_00b2ad60_007693b0(void)
{
    if (unknown_wdt_block_00b2ad60 != 0) {
        operator_delete_00781d3b((unsigned int)(unknown_wdt_block_00b2ad60));
        unknown_wdt_block_00b2ad60 = 0;
    }
    return;
}

}  // namespace seed_007693b0


// 0x007693d0: function not yet identified
//
// Body: 22 bytes (frozen inventory bound).
// Referenced from 1 site (data).
// Ghidra prototype: void unknown_init_block_00b2ad70_007693d0(void)
// Callees with accepted source:
//   unknown_free_block_00b2ad70_007693f0 -> src/recovered/007693f0.cpp
//   atexit_0078298a -> src/recovered/0078298a.cpp
//
// Types and names are Ghidra placeholders;
// rename and retype as the purpose becomes known.
//

namespace seed_007693d0 {

extern void __cdecl unknown_free_block_00b2ad70_007693f0(void);
extern unsigned int  atexit_0078298a(unsigned int);
extern "C" unsigned int unknown_wdt_block_00b2ad70;

void  unknown_init_block_00b2ad70_007693d0(void)
{
    unknown_wdt_block_00b2ad70 = 0;
    atexit_0078298a((unsigned int)(unknown_free_block_00b2ad70_007693f0));
    return;
}

}  // namespace seed_007693d0


// 0x007693f0: function not yet identified
//
// Body: 29 bytes (frozen inventory bound).
// Referenced from 1 site (data).
// Ghidra prototype: void unknown_free_block_00b2ad70_007693f0(void)
// Callees with accepted source:
//   operator_delete_00781d3b -> src/recovered/00781d3b.cpp
//
// Types and names are Ghidra placeholders;
// rename and retype as the purpose becomes known.
//

namespace seed_007693f0 {

extern void  operator_delete_00781d3b(unsigned int);
extern "C" unsigned int unknown_wdt_block_00b2ad70;

void  unknown_free_block_00b2ad70_007693f0(void)
{
    if (unknown_wdt_block_00b2ad70 != 0) {
        operator_delete_00781d3b((unsigned int)(unknown_wdt_block_00b2ad70));
        unknown_wdt_block_00b2ad70 = 0;
    }
    return;
}

}  // namespace seed_007693f0


class Leaf_769510 { public: unsigned int get(); };
unsigned int Leaf_769510::get() { return 0xb2ad68; }


// void __fastcall WDT_Store_Current_00769520(rc_ptr<WDTState> const &state) (0x00769520): keeps the
// current cycle's tour state from the WOL callback and builds the player's campaign from it (TibSun
// WDT_Start_New_Campaign).

extern rc_ptr<WDTState> WDT_Current_State_b2ad68;
extern rc_ptr<WorldDominationTour::Campaign> WDT_Current_Campaign_b2ad78;

void __fastcall WDT_Store_Current_00769520(rc_ptr<WDTState> const &state)
{
    WDT_Current_State_b2ad68 = state;
    WDT_Current_Campaign_b2ad78 = WorldDominationTour::Campaign::Create(unknown_wstring_b26cb8,
        WorldDominationTour::CampaignProperties::Create(state));
}


// 0x00769590: function not yet identified
//
// Body: 6 bytes (frozen inventory bound).
// Referenced from 0 sites.
// Ghidra prototype: undefined * WDT_Previous_State_00769590(void)
//
// Types and names are Ghidra placeholders;
// rename and retype as the purpose becomes known.
//

namespace seed_00769590 {

extern "C" unsigned int WDT_Previous_State_00b2ad80;

unsigned char *  WDT_Previous_State_00769590(void)
{
    return (unsigned char *)(&WDT_Previous_State_00b2ad80);
}

}  // namespace seed_00769590


// void __fastcall WDT_Store_Previous_007695a0(rc_ptr<WDTState> const &state) (0x007695A0): keeps the
// previous cycle's tour state from the WOL callback and builds that campaign from it (TibSun
// WDT_Resume_Campaign).

extern rc_ptr<WDTState> WDT_Previous_State_b2ad80;
extern rc_ptr<WorldDominationTour::Campaign> WDT_Previous_Campaign_b2ad88;

void __fastcall WDT_Store_Previous_007695a0(rc_ptr<WDTState> const &state)
{
    WDT_Previous_State_b2ad80 = state;
    WDT_Previous_Campaign_b2ad88 = WorldDominationTour::Campaign::Create(unknown_wstring_b26cb8,
        WorldDominationTour::CampaignProperties::Create(state));
}


namespace WorldDominationTour { class Campaign; }
extern rc_ptr<WorldDominationTour::Campaign> current_campaign_00b2ad78;
rc_ptr<WorldDominationTour::Campaign> const &__fastcall Current_WDT_Campaign_00769670()
{
    return current_campaign_00b2ad78;
}


// 0x00769610: function not yet identified
//
// Body: 6 bytes (frozen inventory bound).
// Referenced from 0 sites.
// Ghidra prototype: undefined4 unknown_get_block_00b2ad60_00769610(void)
//
// Types and names are Ghidra placeholders;
// rename and retype as the purpose becomes known.
//

namespace seed_00769610 {

extern "C" unsigned int unknown_wdt_block_00b2ad60;

unsigned int  unknown_get_block_00b2ad60_00769610(void)
{
    return unknown_wdt_block_00b2ad60;
}

}  // namespace seed_00769610


// 0x00769620: function not yet identified
//
// Body: 29 bytes (frozen inventory bound).
// Referenced from 0 sites.
// Ghidra prototype: void __fastcall unknown_set_block_00b2ad60_00769620(undefined4 param_1)
// Callees with accepted source:
//   operator_delete_00781d3b -> src/recovered/00781d3b.cpp
//
// Types and names are Ghidra placeholders;
// rename and retype as the purpose becomes known.
//

namespace seed_00769620 {

extern void  operator_delete_00781d3b(unsigned int);
extern "C" unsigned int unknown_wdt_block_00b2ad60;

void __fastcall unknown_set_block_00b2ad60_00769620(unsigned int block)
{
    if (unknown_wdt_block_00b2ad60 != 0) {
        operator_delete_00781d3b((unsigned int)(unknown_wdt_block_00b2ad60));
    }
    unknown_wdt_block_00b2ad60 = block;
    return;
}

}  // namespace seed_00769620


// 0x00769640: function not yet identified
//
// Body: 6 bytes (frozen inventory bound).
// Referenced from 0 sites.
// Ghidra prototype: undefined4 unknown_get_block_00b2ad70_00769640(void)
//
// Types and names are Ghidra placeholders;
// rename and retype as the purpose becomes known.
//

namespace seed_00769640 {

extern "C" unsigned int unknown_wdt_block_00b2ad70;

unsigned int  unknown_get_block_00b2ad70_00769640(void)
{
    return unknown_wdt_block_00b2ad70;
}

}  // namespace seed_00769640


// 0x00769650: function not yet identified
//
// Body: 29 bytes (frozen inventory bound).
// Referenced from 0 sites.
// Ghidra prototype: void __fastcall unknown_set_block_00b2ad70_00769650(undefined4 param_1)
// Callees with accepted source:
//   operator_delete_00781d3b -> src/recovered/00781d3b.cpp
//
// Types and names are Ghidra placeholders;
// rename and retype as the purpose becomes known.
//

namespace seed_00769650 {

extern void  operator_delete_00781d3b(unsigned int);
extern "C" unsigned int unknown_wdt_block_00b2ad70;

void __fastcall unknown_set_block_00b2ad70_00769650(unsigned int block)
{
    if (unknown_wdt_block_00b2ad70 != 0) {
        operator_delete_00781d3b((unsigned int)(unknown_wdt_block_00b2ad70));
    }
    unknown_wdt_block_00b2ad70 = block;
    return;
}

}  // namespace seed_00769650


// rc_ptr<Campaign> const &__fastcall Previous_WDT_Campaign_00769680() (0x00769680): the campaign the
// WOL callbacks stored as the previous cycle's (TibSun WDT_Get_Resumed_Campaign).
namespace WorldDominationTour { class Campaign; }
extern rc_ptr<WorldDominationTour::Campaign> WDT_Previous_Campaign_b2ad88;

rc_ptr<WorldDominationTour::Campaign> const &__fastcall Previous_WDT_Campaign_00769680()
{
    return WDT_Previous_Campaign_b2ad88;
}


// rc_ptr<WDTTerritory> __fastcall Get_WDT_Territory_00769690(int index) (0x00769690): the territory of
// the current campaign's conflict index, or an empty pointer (TibSun WDT_Get_Territory).

using namespace WorldDominationTour;

rc_ptr<Campaign> const &__fastcall Current_WDT_Campaign_00769670();

rc_ptr<WDTTerritory> __fastcall Get_WDT_Territory_00769690(int index)
{
    ConflictCursor cursor = Current_WDT_Campaign_00769670()->Find_Conflict(index);
    return cursor ? (*cursor)->Territory : rc_ptr<WDTTerritory>();
}


// FactionSelectDialogControl deleting destructor (0x00769740, ??_G, vtable slot 0); emitted by the
// FactionSelectDialogControl destructor draft (0x00768800); no source of its own.
