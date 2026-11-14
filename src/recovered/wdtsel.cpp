// wdtsel.cpp: the original translation unit re-filed as one source (Route A): the accepted
// bodies of its canonical compile group in address order, under the unit's own header
// state (its switches and includes in their original order). The module map
// manifests/original_target/modules/wdtsel.json carries the addresses and the rows that
// stay in their own files.


// Header-static dynamic initializers of the unit at 0x0072D370: the shape_file.hpp empty rect and the
// shared/unknown_geometry.hpp constants (0x0072D370-0x0072D551); the other
// drafts of this plan are emitted by these includes.
#include "shared/shape_file.hpp"
#include "shared/unknown_geometry.hpp"


// Dynamic initializer (0x0072D390, _$E) of diagonal from the header(s) included
// by HeaderStatics_empty_rect_init_0072d370.cpp; no source of its own.


// Dynamic initializer (0x0072D3C0, _$E) of b from the header(s) included
// by HeaderStatics_empty_rect_init_0072d370.cpp; no source of its own.


// Dynamic initializer (0x0072D3E0, _$E) of c from the header(s) included
// by HeaderStatics_empty_rect_init_0072d370.cpp; no source of its own.


// Dynamic initializer (0x0072D400, _$E) of angle45 from the header(s) included
// by HeaderStatics_empty_rect_init_0072d370.cpp; no source of its own.


// Dynamic initializer (0x0072D420, _$E) of angle60 from the header(s) included
// by HeaderStatics_empty_rect_init_0072d370.cpp; no source of its own.


// Dynamic initializer (0x0072D440, _$E) of angle90 from the header(s) included
// by HeaderStatics_empty_rect_init_0072d370.cpp; no source of its own.


// Dynamic initializer (0x0072D460, _$E) of g from the header(s) included
// by HeaderStatics_empty_rect_init_0072d370.cpp; no source of its own.


// Dynamic initializer (0x0072D4A0, _$E) of h from the header(s) included
// by HeaderStatics_empty_rect_init_0072d370.cpp; no source of its own.


// Dynamic initializer (0x0072D4D0, _$E) of i from the header(s) included
// by HeaderStatics_empty_rect_init_0072d370.cpp; no source of its own.


// Dynamic initializer (0x0072D500, _$E) of j from the header(s) included
// by HeaderStatics_empty_rect_init_0072d370.cpp; no source of its own.


// Dynamic initializer (0x0072D530, _$E) of k from the header(s) included
// by HeaderStatics_empty_rect_init_0072d370.cpp; no source of its own.


// Dynamic initializer (0x0072D540, _$E) of l from the header(s) included
// by HeaderStatics_empty_rect_init_0072d370.cpp; no source of its own.


// Selection::Selection(rc_ptr<Campaign> const &campaign, int faction) (0x0072D560)
#ifndef VECTOR_IMPLEMENT_ALL
#define VECTOR_IMPLEMENT_ALL
#endif
#include "shared/wdt_selection.hpp"
#include "shared/text_print.hpp"
#include "shared/hicolor.hpp"
#include "shared/cc_file_class.hpp"
#include "shared/string_table.hpp"
extern int unknown_color_a733e4;
namespace WorldDominationTour {
Selection::Selection(rc_ptr<Campaign> const &campaign, int faction) :
    TheCampaign(campaign),
    TheVoices(campaign->Player_Faction()),
    Escaped(false),
    LastMouse(-1, -1),
    unknown_450(campaign->Properties->Last_Conflict()),
    unknown_454(-1),
    TheMap(Map::Create(campaign->Properties->unknown_14(), faction, campaign->Player_Faction())),
    unknown_460(-1),
    TargetPalette(0),
    ZoomingTarget(0),
    Target(0),
    TargetDividingFrame(32),
    Description(0),
    Tooltip(0),
    unknown_484(new MSFont(false)),
    unknown_488(State::Create()),
    ThemeName((char const *)0)
{
    int color = unknown_color_a733e4;
    int pixel = Build_Hicolor_Pixel((unsigned char)color, (unsigned char)((unsigned short)color >> 8), (unsigned char)(color >> 16));
    OldFontColor = BitFont->Set_Color(pixel) & 0xFFFF;
    Add_Anim(TheVoices.Create_Anim());
    CCFileClass file("WDTMAP.INI");
    INIClass ini;
    ini.Load(file);
    Load_Voices(ini, "VoiceOvers");
    Load_Sounds(ini, "SOUNDS");
    Wstring layout;
    ini.Get_String("LAYOUTS", Int_To_Wstring(campaign->Properties->unknown_72d2a0()), "", layout);
    Load_Layout(ini, layout);
    TheMap->Load(ini, this, campaign->Progress_Differs(faction));
    WideString play_text(StringTable::Load_String("GUI:PlayWDT", 0, "D:\\Ra2\\WDTSel.cpp", 157));
    MSBitPrintAnim *play = new MSBitPrintAnim(play_text, TheMap->Get_Play_Rect(), true);
    play->Set_Unknown_30(true);
    Add_Anim(play);
    WideString back_text(StringTable::Load_String("TXT_BACK", 0, "D:\\Ra2\\WDTSel.cpp", 164));
    MSBitPrintAnim *back = new MSBitPrintAnim(back_text, TheMap->Get_Back_Rect(), true);
    back->Set_Unknown_30(true);
    Add_Anim(back);
    Start_Theme();
}
}


// Selection::~Selection() (0x0072D990)
#ifndef VECTOR_IMPLEMENT_ALL
#define VECTOR_IMPLEMENT_ALL
#endif
template<> VectorCursor<MSSfxEntry *, DynamicVectorClass<MSSfxEntry *> >::VectorCursor(
    DynamicVectorClass<MSSfxEntry *> &vector, unsigned index);

namespace WorldDominationTour {
Selection::~Selection()
{
    Clear_Screen();
    unknown_464.Set(0);
    Remove_Territory_Anims();
    Remove_Territory_Anims_2();
    for (SfxCursor cursor(Sounds); cursor; ++cursor) {
        delete *cursor;
    }
    delete TargetPalette;
    delete ZoomingTarget;
    delete Target;
    delete unknown_484;
    BitFont->Set_Color(OldFontColor);
}
}


// void Selection::Load_Voices(INIClass &ini, Wstring const &section) (0x0072DC20)
#ifndef VECTOR_IMPLEMENT_ALL
#define VECTOR_IMPLEMENT_ALL
#endif
namespace WorldDominationTour {
void Selection::Load_Voices(INIClass &ini, Wstring const &section)
{
    TheVoices.Load(ini, section);
    WinningThreshold = ini.Get_Int(section, "WinningThreshold", WinningThreshold);
    MegaWinningThreshold = ini.Get_Int(section, "MegaWinningThreshold", MegaWinningThreshold);
    LosingThreshold = ini.Get_Int(section, "LosingThreshold", LosingThreshold);
    MegaLosingThreshold = ini.Get_Int(section, "MegaLosingThreshold", MegaLosingThreshold);
}
}


// void Selection::Load_Sounds(INIClass &ini, Wstring const &section) (0x0072DD10)
#ifndef VECTOR_IMPLEMENT_ALL
#define VECTOR_IMPLEMENT_ALL
#endif
namespace WorldDominationTour {
void Selection::Load_Sounds(INIClass &ini, Wstring const &section)
{
    Wstring value;
    int count = ini.Entry_Count(section);
    for (int index = 0; index < count; index++) {
        Wstring entry = ini.Get_Entry(section, index);
        if (entry != 0 && ini.Get_String(section, entry, (char const *)0, value) > 0) {
            if (entry == "Theme") {
                ThemeName = value;
                break;
            }
            MSSfxEntry *sound = new MSSfxEntry(entry, value);
            if (sound->Name == 0) {
                delete sound;
            } else if (sound != 0) {
                Sounds.Add(sound);
            }
        }
    }
}
}


// void Selection::Load_Layout(INIClass &ini, Wstring const &section) (0x0072DEA0)
#ifndef VECTOR_IMPLEMENT_ALL
#define VECTOR_IMPLEMENT_ALL
#endif
namespace WorldDominationTour {
void Selection::Load_Layout(INIClass &ini, Wstring const &section)
{
    Wstring value;
    Wstring faction(Faction_Name(TheCampaign->Player_Faction()));
    if (ini.Get_String(section, faction + "ZoomingTarget", (char const *)0, value) > 0) {
        ZoomingTarget = new MSAnimEntry(value);
    }
    ini.Get_String(section, faction + "TargetPalette", (char const *)0, value);
    TargetPalette = unknown_load_drawer_5ac560(value);
    ini.Get_String(section, faction + "Target", (char const *)0, value);
    Target = new MSAnimEntry(value);
    TargetDividingFrame = ini.Get_Int(section, faction + "TargetDividingFrame", 0x20);
}
}


// rc_ptr<Territory> Selection::Choose_Territory() (0x0072E050)
#ifndef VECTOR_IMPLEMENT_ALL
#define VECTOR_IMPLEMENT_ALL
#endif
#include "shared/lobby.hpp"
namespace WorldDominationTour {
rc_ptr<Territory> Selection::Choose_Territory()
{
    Show_Campaign();
    bool canceled = false;
    bool selected = false;
    WWKeyboard->Clear();
    do {
        Unknown_0073e100();
        Check_Focus();
        Point2D point(WWMouse->Get_Mouse_X(), WWMouse->Get_Mouse_Y());
        if (WWKeyboard->Check()) {
            int key = WWKeyboard->Get();
            if (key == 1) {
                Click(point, selected);
            } else if (key == 0x801) {
                TheMap->Set_Unknown_74(this, false);
                if (Region(point) == 2) {
                    Play_Sound("Canceled");
                    canceled = true;
                }
            }
        } else if (point != LastMouse) {
            LastMouse = point;
            int region = Region(point);
            if (unknown_454 != region) {
                switch (unknown_454) {
                case 1:
                    Leave_Territory();
                    break;
                case 2:
                    Uncancel();
                    break;
                }
            }
            switch (region) {
            case 1:
                if (unknown_454 != 1) {
                    Play_Map_Enter();
                }
                Hover_2(point);
                break;
            case 2:
                if (unknown_454 != 2) {
                    Cancel();
                }
                break;
            }
            unknown_454 = region;
        }
        Run(1);
    } while (!canceled && !selected);
    return selected ? unknown_464 : rc_ptr<Territory>();
}
}


// void Selection::Click(Point2D const &point, bool &selected) (0x0072E430)
// Hover_At is a hypothesis: Hover's second Get_Territory stays out of line here, which needs one more
// inline level (or one pending site) over Hover.
#ifndef VECTOR_IMPLEMENT_ALL
#define VECTOR_IMPLEMENT_ALL
#endif
namespace WorldDominationTour {
// Never inlined: Choose_Territory 0x0072E050 calls it.
#pragma auto_inline(off)
void Selection::Click(Point2D const &point, bool &selected)
{
    switch (Region(point)) {
    case 2:
        TheMap->Set_Unknown_74(this, true);
        Cancel();
        break;
    case 1:
        TheMap->Set_Unknown_74(this, false);
        selected = Hover_At(point);
        if (selected) {
            Play_Sound("ConflictSelected");
        }
        break;
    }
}
#pragma auto_inline(on)
}


// bool Selection::Hover(Point2D const &point) (0x0072E650): refreshes the hovered territory, then selects it.
// DEAD CONSTRUCT, owner ruling: `true ?` accounts for bytes in the target (the zeroed temporary-construction
// flag at entry and the copy into territory through the out-of-line default ctor plus Set). The real source's
// condition is unknown; a plain initialisation gives 39.22.
#ifndef VECTOR_IMPLEMENT_ALL
#define VECTOR_IMPLEMENT_ALL
#endif
namespace WorldDominationTour {
bool Selection::Hover(Point2D const &point)
{
    Hover_2(point);
    rc_ptr<Territory> territory = true ? TheMap->Get_Territory(unknown_460) : rc_ptr<Territory>();
    return Select_Territory(territory);
}
}


// void Selection::Hover_2(Point2D const &point) (0x0072E720)
#ifndef VECTOR_IMPLEMENT_ALL
#define VECTOR_IMPLEMENT_ALL
#endif
namespace WorldDominationTour {
void Selection::Hover_2(Point2D const &point)
{
    int index = TheMap->Territory_At(point);
    if (index != unknown_460) {
        unknown_460 = index;
        Select_Territory(TheMap->Get_Territory(index));
    }
}
}


// void Selection::Show_State(rc_ptr<State> const &state, bool unknown2, bool won) (0x0072E780): moves the
// map to a new state, updating each territory's owner overlay, then (unknown2) plays the zoom and throb
// target animations on the disputed territories (TibSun Selection::Do_Target_Anim).
// The cursor walks follow TibSun wdtsel.cpp: postfix ++, virtual operator bool.
// Walk_Territories is a hypothesis: the main walk's cursor constructor sits one inline level down
// (out of line here, inline in the later anim walks), and no spelling at depth one refuses it.
#ifndef VECTOR_IMPLEMENT_ALL
#define VECTOR_IMPLEMENT_ALL
#endif
#include "shared/ms_anim.hpp"
namespace WorldDominationTour {
inline TerritoryCursor Walk_Territories(TerritoryList &list) { return TerritoryCursor(list); }
typedef DynamicVectorClass<MSAnim *> AnimList;
typedef VectorCursor<MSAnim *, AnimList> AnimCursor;
void Selection::Show_State(rc_ptr<State> const &state, bool unknown2, bool won)
{
    Remove_Territory_Anims();
    AnimList fades;
    AnimList shapes;
    for (TerritoryCursor cursor = Walk_Territories(TheMap->Territories); cursor; cursor++) {
        int index = (*cursor)->Get_Index();
        int old_owner = unknown_488->Owner_Of(index);
        int owner = state->Owner_Of(index);
        if (owner != old_owner) {
            Check_Escape();
            (*cursor)->Set_Owner_State(owner, this, !Escaped || won);
        }
        if (unknown2 && owner == 1) {
            Point2D point = TheMap->Get_Origin() + (*cursor)->Get_Target();
            int x = point.X;
            int y = point.Y;
            if (ZoomingTarget != 0) {
                CCFileClass file(ZoomingTarget->Get_Filename());
                if (file.Is_Available()) {
                    MSFadeAnim *fade = new MSFadeAnim(ZoomingTarget->Get_Filename(), x, y, TargetPalette, 3, 0x200, &Anims);
                    fades.Add(fade);
                }
            }
            CCFileClass file(Target->Name);
            if (file.Is_Available()) {
                MSShapeAnim *shape = new MSShapeAnim(Target->Name, x, y, TargetPalette, 5, true, 0x200);
                int frame = TargetDividingFrame - 1;
                shape->Set_End_Frame(frame);
                shape->Set_Start_Frame(0);
                shape->Set_Frame(0);
                (*cursor)->Set_unknown_2c((int)shape);
                shapes.Add(shape);
            }
        }
    }
    AnimCursor fade(fades);
    for (AnimCursor shape(shapes); shape; shape++) {
        Check_Escape();
        if (fade) {
            if (Escaped) {
                MSAnim *anim = *fade;
                delete anim;
            } else {
                Play_Sound("TargetZoom");
                Add_Anim(*fade);
                Wait_For(*fade, 300000);
            }
            fade++;
        }
        Add_Anim(*shape);
    }
    unknown_488 = state;
}
}


WideString __fastcall operator+(unsigned short const *left, WideString const &right);

namespace WorldDominationTour {
bool Selection::Select_Territory(rc_ptr<Territory> const &territory)
{
    bool selected = unknown_50.Get() != 0;
    if (territory.Get() == unknown_464.Get()) return selected;
    if (unknown_464.Get()) {
        if (unknown_50.Get()) unknown_50.Set(0);
        unknown_464->Show_Highlight(false, this);
        MSShapeAnim *target = (MSShapeAnim *)unknown_464->Get_unknown_2c();
        if (target) {
            target->Set_End_Frame(TargetDividingFrame - 1);
            target->Set_Start_Frame(0);
            target->Set_Frame(0);
        }
    }
    unknown_464 = territory;
    if (!unknown_464.Get()) {
        Show_Campaign();
        TheVoices.Say_List(1, false, 30);
        return selected;
    }
    selected = false;
    WideString description;
    WideString options;
    int index = unknown_464->Get_Index();
    int owner = unknown_488->Owner_Of(index);
    int kind = 2;
    if (owner != 1 && owner != 0) kind = TheCampaign->Player_Faction() != owner;
    TheVoices.Say(3, kind, false, false, 30);
    switch (owner) {
    case 1: Set_Tooltip(StringTable::Load_String("WDT:TerritorySelDispToolTip", 0, "D:\\Ra2\\WDTSel.cpp", 0x2b5)); break;
    case 2: Set_Tooltip(StringTable::Load_String("WDT:AlliedHeldTooltip", 0, "D:\\Ra2\\WDTSel.cpp", 0x2ad)); break;
    case 3: Set_Tooltip(StringTable::Load_String("WDT:SovietHeldTooltip", 0, "D:\\Ra2\\WDTSel.cpp", 0x2b1)); break;
    default: Set_Tooltip(StringTable::Load_String("WDT:TerritorySelectToolTip", 0, "D:\\Ra2\\WDTSel.cpp", 0x2b9)); break;
    }
    unknown_464->Show_Highlight(true, this);
    if (TheCampaign->Properties->Last_Conflict() == unknown_450) {
        ConflictCursor cursor(TheCampaign->Conflicts);
        while (cursor) {
            rc_ptr<Conflict> const &conflict = *cursor;
            Conflict const *current = conflict.operator->();
            int id = unknown_464->Get_Index();
            if (current->Is_Territory(id)) break;
            cursor++;
        }
        if (cursor) {
            selected = true;
            unknown_50 = *cursor;
            description = (*cursor)->Describe_Options();
            options = description;
            MSShapeAnim *target = (MSShapeAnim *)territory->Get_unknown_2c();
            if (target) {
                target->Set_End_Frame(-1);
                target->Set_Start_Frame(TargetDividingFrame);
                target->Set_Frame(TargetDividingFrame);
            }
        }
    }
    WideString text = StringTable::Load_String("WDT:TerritoryPrefix", 0, "D:\\Ra2\\WDTSel.cpp", 0x2d7)
        + unknown_464->Name + StringTable::Load_String("WDT:TerritorySuffix", 0, "D:\\Ra2\\WDTSel.cpp", 0x2d7);
    if (options.Length() > 0) text += description;
    Set_Description(text, 0);
    return selected;
}
}


// void Selection::Remove_Territory_Anims() (0x0072F180)
#ifndef VECTOR_IMPLEMENT_ALL
#define VECTOR_IMPLEMENT_ALL
#endif
namespace WorldDominationTour {
void Selection::Remove_Territory_Anims()
{
    for (TerritoryCursor cursor(TheMap->Territories); cursor; ++cursor) {
        MSAnim *anim = (MSAnim *)(*cursor)->Take_unknown_2c();
        if (anim != 0) {
            Remove_Anim(anim);
        }
    }
}
}


// void Selection::Remove_Territory_Anims_2() (0x0072F200; same body as 0x0072F180)
#ifndef VECTOR_IMPLEMENT_ALL
#define VECTOR_IMPLEMENT_ALL
#endif
namespace WorldDominationTour {
void Selection::Remove_Territory_Anims_2()
{
    for (TerritoryCursor cursor(TheMap->Territories); cursor; ++cursor) {
        MSAnim *anim = (MSAnim *)(*cursor)->Take_unknown_2c();
        if (anim != 0) {
            Remove_Anim(anim);
        }
    }
}
}


// void Selection::Play_Map_Enter() (0x0072F280)
#ifndef VECTOR_IMPLEMENT_ALL
#define VECTOR_IMPLEMENT_ALL
#endif
namespace WorldDominationTour {
void Selection::Play_Map_Enter()
{
    Play_Sound("MapEnter");
}
}


// void Selection::Leave_Territory() (0x0072F340)
#ifndef VECTOR_IMPLEMENT_ALL
#define VECTOR_IMPLEMENT_ALL
#endif
namespace WorldDominationTour {
void Selection::Leave_Territory()
{
    Play_Sound("MapLeave");
    unknown_460 = -1;
    rc_ptr<Territory> none;
    Select_Territory(none);
}
}


// void Selection::Cancel() (0x0072F430)
#ifndef VECTOR_IMPLEMENT_ALL
#define VECTOR_IMPLEMENT_ALL
#endif
namespace WorldDominationTour {
void Selection::Cancel()
{
    if (!TheMap->unknown_64.unknown_4) {
        TheMap->Set_Unknown_68(this, true);
        Play_Sound("MouseCancel");
        Set_Tooltip(StringTable::Load_String("WOL:WDTTerritoryBack", 0, "D:\\Ra2\\WDTSel.cpp", 894));
    }
}
}


// void Selection::Uncancel() (0x0072F540)
#ifndef VECTOR_IMPLEMENT_ALL
#define VECTOR_IMPLEMENT_ALL
#endif
namespace WorldDominationTour {
void Selection::Uncancel()
{
    if (TheMap->unknown_64.unknown_4) {
        TheMap->Set_Unknown_68(this, false);
        Set_Tooltip(StringTable::Load_String("WDT:TerritorySelectToolTip", 0, "D:\\Ra2\\WDTSel.cpp", 916));
    }
}
}


// void Selection::Show_Progress(int faction) (0x0072F5A0)
#ifndef VECTOR_IMPLEMENT_ALL
#define VECTOR_IMPLEMENT_ALL
#endif
namespace WorldDominationTour {
void Selection::Show_Progress(int faction)
{
    Set_Description(0, 0);
    int last = TheCampaign->Properties->Last_Conflict();
    bool won = true;
    int day = TheCampaign->Last_Day(faction);
    if (last - day > 8) {
        day = last - 8;
    }
    if (day != last) {
        won = !Show_Days(day, last);
    } else {
        bool mega = false;
        int result = Get_Result(last, mega);
        TheVoices.Say(2, result, mega, true, 0);
    }
    Set_Description(0, 0);
    Check_Escape();
    Show_State(TheCampaign->TheHistory->Get_State(last), true, won);
    TheCampaign->Save_Progress(faction);
}
}


// int Selection::Get_Result(int index, bool &mega) (0x0072F6E0)
#ifndef VECTOR_IMPLEMENT_ALL
#define VECTOR_IMPLEMENT_ALL
#endif
namespace WorldDominationTour {
int Selection::Get_Result(int index, bool &mega)
{
    rc_ptr<State> state = TheCampaign->TheHistory->Get_State(index);
    if (state.Get() != 0) {
        int owned = state->Count_Owned(TheCampaign->PlayerFaction);
        if (owned <= LosingThreshold) {
            mega = owned <= MegaLosingThreshold;
            return 1;
        }
        if (owned >= WinningThreshold) {
            mega = owned >= MegaWinningThreshold;
            return 0;
        }
    }
    mega = false;
    return 2;
}
}


// bool Selection::Animate_Days(int from, int to) (0x0072F7A0)
#ifndef VECTOR_IMPLEMENT_ALL
#define VECTOR_IMPLEMENT_ALL
#endif
WideString __fastcall unknown_number_text_width_771130(int number, int width);
namespace WorldDominationTour {
bool Selection::Animate_Days(int from, int to)
{
    bool first = true;
    bool first_state = true;
    unsigned length = 0;
    for (int day = from; day <= to; day++) {
        Check_Escape();
        if (Escaped) {
            break;
        }
        WideString text(StringTable::Load_String("WDT:ReviewingHistory", 0, "D:\\Ra2\\WDTSel.cpp", 995));
        text += L"\n";
        if (first) {
            length = text.Length();
        }
        text += WideString(StringTable::Load_String("WDT:ReviewingHistoryDay", 0, "D:\\Ra2\\WDTSel.cpp", 1003)) + WideString(L" ") + unknown_number_text_width_771130(day + 1, 2);
        Set_Description(text, first ? 0 : length);
        Show_State(TheCampaign->TheHistory->Get_State(day), false, first_state);
        first = false;
        first_state = false;
        Wait_For(Description, 300000);
        int count = 5;
        while (!Escaped && count--) {
            Run(5);
            Check_Escape();
        }
        if (Escaped) {
            break;
        }
    }
    return first;
}
}


// void Selection::Show_Result(int faction) (0x0072FA00)
#ifndef VECTOR_IMPLEMENT_ALL
#define VECTOR_IMPLEMENT_ALL
#endif
namespace WorldDominationTour {
void Selection::Show_Result(int faction)
{
    Set_Description(L"", 0);
    int last = TheCampaign->Properties->Last_Conflict();
    int day = TheCampaign->Last_Day(faction);
    if (last - day > 8) {
        day = last - 8;
    }
    rc_ptr<State> state = TheCampaign->TheHistory->Get_State(day);
    if (state.Get() != 0) {
        int owned = state->Count_Owned(TheCampaign->Player_Faction());
        if (owned >= MegaWinningThreshold) {
            TheVoices.Say(0, 0, owned >= 30, true, 0);
        } else if (owned <= MegaLosingThreshold) {
            TheVoices.Say(0, 1, owned <= 0, true, 0);
        } else {
            TheVoices.Say(0, 2, true, true, 0);
        }
        Animate_Days(day, last);
        while (TheVoices.Is_Speaking() && !Escaped) {
            Run(5);
            Check_Escape();
        }
    }
}
}


// bool Selection::Show_Days(int from, int to) (0x0072FB60)
#ifndef VECTOR_IMPLEMENT_ALL
#define VECTOR_IMPLEMENT_ALL
#endif
namespace WorldDominationTour {
bool Selection::Show_Days(int from, int to)
{
    rc_ptr<State> before = TheCampaign->TheHistory->Get_State(from);
    int old_count = before->Count_Owned(TheCampaign->Player_Faction());
    rc_ptr<State> after = TheCampaign->TheHistory->Get_State(to);
    int new_count = after->Count_Owned(TheCampaign->Player_Faction());
    int change = new_count - old_count;
    int kind;
    bool mega;
    if (change < -1) {
        kind = 1;
        mega = change < -3 || new_count <= MegaLosingThreshold;
    } else if (change > 1) {
        kind = 0;
        mega = change > 3 || new_count >= MegaWinningThreshold;
    } else {
        kind = 2;
        mega = false;
        mega = Get_Result(to, mega) == 1 && mega;
    }
    TheVoices.Say(1, kind, mega, true, 0);
    return !Animate_Days(from, to);
}
}


// void Selection::Check_Escape() (0x0072FC80)
#ifndef VECTOR_IMPLEMENT_ALL
#define VECTOR_IMPLEMENT_ALL
#endif
namespace WorldDominationTour {
void Selection::Check_Escape()
{
    Check_Focus();
    while (WWKeyboard->Check()) {
        if (WWKeyboard->Get() == 0x1B) {
            Escaped = true;
        }
    }
}
}


// void Selection::Set_Description(WideString const &text, int unknown2) (0x0072FCD0)
#ifndef VECTOR_IMPLEMENT_ALL
#define VECTOR_IMPLEMENT_ALL
#endif
namespace WorldDominationTour {
void Selection::Set_Description(WideString const &text, int unknown2)
{
    if (Description != 0) {
        Remove_Anim(Description);
        unknown_surface_839954->Copy_From(TheMap->DescriptionRect, *unknown_surface_839958, TheMap->DescriptionRect, false, true);
        Add_Rect(TheMap->DescriptionRect);
        Description = 0;
    }
    if (text.Length() > 0) {
        WideString copy(text);
        Description = new MSBitPrintAnim(copy, TheMap->DescriptionRect, true);
        Description->Set_Unknown_30(true);
        Add_Anim(Description);
    }
}
}


// void Selection::Set_Tooltip(WideString const &text) (0x0072FDA0)
#ifndef VECTOR_IMPLEMENT_ALL
#define VECTOR_IMPLEMENT_ALL
#endif
namespace WorldDominationTour {
void Selection::Set_Tooltip(WideString const &text)
{
    Rect rect = TheMap->TooltipRect;
    if (Tooltip != 0) {
        Remove_Anim(Tooltip);
        unknown_surface_839954->Copy_From(rect, *unknown_surface_839958, rect, false, true);
        Add_Rect(rect);
        Tooltip = 0;
    }
    if (text.Length() > 0) {
        WideString copy(text);
        Tooltip = new MSBitPrintAnim(copy, rect, false);
        Tooltip->Set_Unknown_30(true);
        Add_Anim(Tooltip);
    }
}
}


// void Selection::Play_Sound(Wstring const &name) (0x0072FE90)
#ifndef VECTOR_IMPLEMENT_ALL
#define VECTOR_IMPLEMENT_ALL
#endif
namespace WorldDominationTour {
void Selection::Play_Sound(Wstring const &name)
{
    if (name != 0) {
        SfxCursor cursor(Sounds);
        while (cursor && name == (*cursor)->Name) {
            ++cursor;
        }
        if (cursor) {
            (*cursor)->Play();
        }
    }
}
}


// void Selection::Show_Campaign() (0x0072FF30)
#ifndef VECTOR_IMPLEMENT_ALL
#define VECTOR_IMPLEMENT_ALL
#endif
namespace WorldDominationTour {
void Selection::Show_Campaign()
{
    Set_Description(TheCampaign->Properties->LongDescription, 0);
    WideString tip(StringTable::Load_String("WDT:TerritorySelectToolTip", 0, "D:\\Ra2\\WDTSel.cpp", 1230));
    Set_Tooltip(tip);
}
}


// void __fastcall Selection::Show(rc_ptr<Campaign> const &campaign, int faction) (0x0072FF80)
#ifndef VECTOR_IMPLEMENT_ALL
#define VECTOR_IMPLEMENT_ALL
#endif
namespace WorldDominationTour {
void __fastcall Selection::Show(rc_ptr<Campaign> const &campaign, int faction)
{
    WWKeyboard->Clear();
    Selection selection(campaign, faction);
    selection.Show_Result(faction);
}
}


// bool __fastcall Selection::Choose_Conflict(rc_ptr<Campaign> const &campaign, int faction) (0x0072FFC0)
#ifndef VECTOR_IMPLEMENT_ALL
#define VECTOR_IMPLEMENT_ALL
#endif
#include "shared/options_dialog.hpp"
#include "shared/session_state.hpp"
namespace WorldDominationTour {
bool __fastcall Selection::Choose_Conflict(rc_ptr<Campaign> const &campaign, int faction)
{
    if (!campaign.Get()) {
        return false;
    }
    Selection selection(campaign, faction);
    selection.Show_Campaign();
    selection.Show_Progress(faction);
    rc_ptr<Territory> territory = selection.Choose_Territory();
    bool chosen = territory.Get() != 0;
    if (chosen) {
        unknown_session_a3d2a0 = territory->Index;
        rc_ptr<WDTTerritory> settings = Unknown_Territory_00769690(unknown_session_a3d2a0);
        if (settings.Get()) {
            unknown_session_a3d298.UnitCount = settings->UnitCount;
            unknown_house_setting_7d88d8 = settings->TechLevel;
            unknown_session_a3d298.unknown_24 = settings->unknown_10;
            unknown_session_a3d298.unknown_a4 = (settings->OptionFlags & 0x4) != 0;
            unknown_session_a3d298.unknown_a5 = false;
            unknown_session_a3d298.unknown_bases_20 = (settings->OptionFlags & 0x10) != 0;
            unknown_session_a3d298.unknown_a8 = (settings->OptionFlags & 0x20) != 0;
            unknown_session_a3d298.unknown_fog_a7 = false;
            unknown_session_a3d298.unknown_28 = true;
            unknown_session_a3d298.unknown_crates_29 = (settings->OptionFlags & 0x100) != 0;
            unknown_session_a3d298.unknown_2a = (settings->OptionFlags & 0x400) != 0;
            unknown_session_a3d298.unknown_2b = (settings->OptionFlags & 0x200) != 0;
            unknown_session_a3d298.unknown_2c = false;
            unknown_session_a3d298.AICount = 0;
            unknown_session_a3d298.unknown_difficulty_40 = 1;
        } else {
            chosen = false;
        }
    }
    return chosen;
}
}


// void Selection::Start_Theme() (0x00730160)
#ifndef VECTOR_IMPLEMENT_ALL
#define VECTOR_IMPLEMENT_ALL
#endif
namespace WorldDominationTour {
void Selection::Start_Theme()
{
    unknown_flag_a40d44 = false;
    if (ThemeName != 0) {
        Theme.Play_Song(Theme.From_Name(ThemeName.Get()));
        Theme.IsRepeat = true;
    }
    WWMouse->Hide_Mouse();
    unknown_surface_839958->Fill(0);
    unknown_surface_839954->Fill(0);
    unknown_surface_839950->Copy_From(*unknown_surface_839954, false, true);
    WWKeyboard->Clear();
    TheMap->unknown_72a620(this, &TheVoices);
    while (WWMouse->Get_Mouse_State() < 0) {
        WWMouse->Show_Mouse();
    }
}
}


// void Selection::Clear_Screen() (0x00730230)
#ifndef VECTOR_IMPLEMENT_ALL
#define VECTOR_IMPLEMENT_ALL
#endif
namespace WorldDominationTour {
void Selection::Clear_Screen()
{
    WWMouse->Hide_Mouse();
    Theme.Queue_Song(-3);
    unknown_surface_839958->Fill(0);
    unknown_surface_839954->Fill(0);
    while (WWMouse->Get_Mouse_State() < 0) {
        WWMouse->Show_Mouse();
    }
}
}


// Map::Get_Territory(int), 007302B0: the inline header member emitted naturally
// by the complete WDTSel.cpp population. No separate handwritten body.
#include "shared/wdt_map.hpp"


// VectorCursor<MSSfxEntry*,DynamicVectorClass<MSSfxEntry*> >00730340.
// Concrete cursor identity comes from vptr007AF37C and the accepted Selection sound walks.
template<>
VectorCursor<MSSfxEntry *, DynamicVectorClass<MSSfxEntry *> >::VectorCursor(
    DynamicVectorClass<MSSfxEntry *> &vector, unsigned index)
    : Vector(&vector), Index(index)
{
}


extern "C" void __cdecl release_730290(void*);
class Leaf_730290 { public: void cleanup();void* get(unsigned int flags); };
void* Leaf_730290::get(unsigned int flags) { cleanup(); if (flags & 1) release_730290(this); return this; }


class Leaf_7302d0 { public: unsigned int get(); };
unsigned int Leaf_7302d0::get() { unsigned int self = (unsigned int)this; unsigned int result; unsigned int spare; result = *(unsigned int*)(self + 4); return result; }


class Call_7302e0_0 {public: unsigned int target();};
extern "C" unsigned char data_7302e0_7af27c[];
class Call_7302e0_1 {public: unsigned int target(unsigned int p0);};
class Leaf_7302e0 { public: unsigned int get(unsigned int a1); };
unsigned int Leaf_7302e0::get(unsigned int a1) { unsigned int r0 = ((Call_7302e0_0*)((unsigned int)this))->target(); *(unsigned int*)(((unsigned int)this)) = (unsigned int)data_7302e0_7af27c; unsigned int r1 = ((Call_7302e0_1*)((unsigned int)this))->target((unsigned int)(*(unsigned int*)((a1) + 4))); return (unsigned int)this; }


class Leaf_730320 { public: unsigned int get(); };
unsigned int Leaf_730320::get() { unsigned int self = (unsigned int)this; unsigned int result; unsigned int spare; result = *(unsigned int*)(self + 4); return result; }


class Leaf_730330 { public: unsigned int get(); };
unsigned int Leaf_730330::get() { unsigned int self = (unsigned int)this; unsigned int result; unsigned int spare; result = *(unsigned int*)(self + 4); return result; }


// VectorCursor<rc_ptr<WorldDominationTour::Territory>, DynamicVectorClass<...>>::VectorCursor(V &, unsigned)
// (0x00730370): the out-of-line copy of the inline constructor (shared/vector_cursor.hpp) that this
// unit's territory walks call (Selection destructor 0x0072D990); no source of its own.


// VectorCursor<MSSfxEntry*,DynamicVectorClass<MSSfxEntry*> > deleting destructor007303B0.
// Naturally emitted by the real sound-cursor uses in Selection::Play_Sound and its callers.
// No handwritten compiler-helper implementation.


class Leaf_730410 { public: unsigned int get(); };
unsigned int Leaf_730410::get() { unsigned int self = (unsigned int)this; unsigned int result; unsigned int spare; result = self; *(unsigned int*)(result + 8) = (*(unsigned int*)(result + 8)) + 1; return result; }


class Leaf_730420 { public: unsigned int get(); };
unsigned int Leaf_730420::get() { unsigned int self = (unsigned int)this; unsigned int result; unsigned int spare; result = self; *(unsigned int*)(result + 8) = (*(unsigned int*)(result + 8)) - 1; return result; }


class Leaf_730430 { public: unsigned int get(); };
unsigned int Leaf_730430::get() { unsigned int self = (unsigned int)this; unsigned int result; unsigned int spare; result = self; *(unsigned int*)(result + 8) = (*(unsigned int*)(result + 8)) + 1; return result; }


class Leaf_730440 { public: unsigned int get(); };
unsigned int Leaf_730440::get() { unsigned int self = (unsigned int)this; unsigned int result; unsigned int spare; result = self; *(unsigned int*)(result + 8) = (*(unsigned int*)(result + 8)) - 1; return result; }


class Leaf_730450 { public: unsigned int get(); };
unsigned int Leaf_730450::get() { unsigned int self = (unsigned int)this; unsigned int result; unsigned int spare; result = self; *(unsigned int*)(result + 8) = (*(unsigned int*)(result + 8)) + 1; return result; }


class Leaf_730460 { public: unsigned int get(); };
unsigned int Leaf_730460::get() { unsigned int self = (unsigned int)this; unsigned int result; unsigned int spare; result = self; *(unsigned int*)(result + 8) = (*(unsigned int*)(result + 8)) - 1; return result; }


extern "C" unsigned char table_730470[]; extern "C" void* __cdecl storage_730470(unsigned int);
class Storage_730470 { unsigned int table_word; unsigned int* items; unsigned int count; unsigned char valid; unsigned char owns; public: void* get(unsigned int requested,unsigned int* buffer); };
void* Storage_730470::get(unsigned int requested,unsigned int* buffer) { items=0; count=requested; valid=1; owns=0; table_word=(unsigned int)table_730470; if(requested) { if(buffer) { items=buffer; return this; } items=(unsigned int*)storage_730470(requested << 2); owns=1; } return this; }


extern "C" unsigned char table_7304c0[]; extern "C" void* __cdecl storage_7304c0(unsigned int);
class Storage_7304c0 { unsigned int table_word; unsigned int* items; unsigned int count; unsigned char valid; unsigned char owns; public: void* get(unsigned int requested,unsigned int* buffer); };
void* Storage_7304c0::get(unsigned int requested,unsigned int* buffer) { items=0; count=requested; valid=1; owns=0; table_word=(unsigned int)table_7304c0; if(requested) { if(buffer) { items=buffer; return this; } items=(unsigned int*)storage_7304c0(requested << 2); owns=1; } return this; }
