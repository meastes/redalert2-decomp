// mapsel.cpp: the original translation unit re-filed as one source (Route A): the accepted
// bodies of its canonical compile group in address order, under the unit's own header
// state (its switches and includes in their original order). The module map
// manifests/original_target/modules/mapsel.json carries the addresses and the rows that
// stay in their own files.


#include "shared/shape_file.hpp"
#include "shared/unknown_geometry.hpp"
#include "shared/map_select.hpp"
#include "shared/id_source.hpp"
#include "shared/ww_message_box.hpp"
#include "shared/string_table.hpp"
#include "shared/debug_printf.hpp"
#include "shared/house_type.hpp"
#include "shared/side.hpp"
#include "shared/theme.hpp"
#include "shared/dsurface.hpp"


// Header-static dynamic initializers of the unit at 0x0058F320: the shape_file.hpp empty rect and the
// shared/unknown_geometry.hpp constants (0x0058F320-0x0058F501); the other
// drafts of this plan are emitted by these includes.


// Dynamic initializer (0x0058F340, _$E) of diagonal from the header(s) included
// by HeaderStatics_empty_rect_init_0058f320.cpp; no source of its own.


// Dynamic initializer (0x0058F370, _$E) of b from the header(s) included
// by HeaderStatics_empty_rect_init_0058f320.cpp; no source of its own.


// Dynamic initializer (0x0058F390, _$E) of c from the header(s) included
// by HeaderStatics_empty_rect_init_0058f320.cpp; no source of its own.


// Dynamic initializer (0x0058F3B0, _$E) of angle45 from the header(s) included
// by HeaderStatics_empty_rect_init_0058f320.cpp; no source of its own.


// Dynamic initializer (0x0058F3D0, _$E) of angle60 from the header(s) included
// by HeaderStatics_empty_rect_init_0058f320.cpp; no source of its own.


// Dynamic initializer (0x0058F3F0, _$E) of angle90 from the header(s) included
// by HeaderStatics_empty_rect_init_0058f320.cpp; no source of its own.


// Dynamic initializer (0x0058F410, _$E) of g from the header(s) included
// by HeaderStatics_empty_rect_init_0058f320.cpp; no source of its own.


// Dynamic initializer (0x0058F450, _$E) of h from the header(s) included
// by HeaderStatics_empty_rect_init_0058f320.cpp; no source of its own.


// Dynamic initializer (0x0058F480, _$E) of i from the header(s) included
// by HeaderStatics_empty_rect_init_0058f320.cpp; no source of its own.


// Dynamic initializer (0x0058F4B0, _$E) of j from the header(s) included
// by HeaderStatics_empty_rect_init_0058f320.cpp; no source of its own.


// Dynamic initializer (0x0058F4E0, _$E) of k from the header(s) included
// by HeaderStatics_empty_rect_init_0058f320.cpp; no source of its own.


// Dynamic initializer (0x0058F4F0, _$E) of l from the header(s) included
// by HeaderStatics_empty_rect_init_0058f320.cpp; no source of its own.


extern "C" unsigned int __cdecl strlen(char const *);
extern char const unknown_mapselect_file_7e1bd4[];
extern char const unknown_mapselect_failure_7e1bc0[];
extern char const unknown_txt_ok_7db8f4[];
extern unsigned short const unknown_blank_caption_7d64f4[];
extern char const unknown_mapselect_format_7e1b98[];
char const *__fastcall Map_Selection_0058f510(ScenarioClass *scenario)
{
    if(!MapSelect().Presentation_0058f900(scenario)) {
        WWMessageBox(unknown_blank_caption_7d64f4).Process(
            StringTable::Load_String(unknown_mapselect_failure_7e1bc0,0,unknown_mapselect_file_7e1bd4,134),
            StringTable::Load_String(unknown_txt_ok_7db8f4,0,unknown_mapselect_file_7e1bd4,134),0,0,false);
        return 0;
    }
    Debug_Printf(unknown_mapselect_format_7e1b98,scenario->FileName,scenario->unknown_2304);
    return strlen(scenario->FileName)==0?0:scenario->FileName;
}


extern char const unknown_mapselect_file_7e1bd4[];
extern char const unknown_mapselect_failure_7e1bc0[];
extern char const unknown_txt_ok_7db8f4[];
extern unsigned short const unknown_blank_caption_7d64f4[];
char const *__fastcall Map_Select_Advance_0058f620(ScenarioClass *scenario,char const *name)
{
    if(!MapSelect().Advance_Progression_0058f720(scenario,name)) {
        WWMessageBox(unknown_blank_caption_7d64f4).Process(
            StringTable::Load_String(unknown_mapselect_failure_7e1bc0,0,unknown_mapselect_file_7e1bd4,152),
            StringTable::Load_String(unknown_txt_ok_7db8f4,0,unknown_mapselect_file_7e1bd4,152),0,0,false);
        return 0;
    }
    return name;
}


extern char const unknown_mapselect_invalid_side_7e1c44[];
extern char const unknown_mapselect_side_7e1c30[];
extern char const unknown_mapselect_invalid_choices_7e1c04[];
extern char const unknown_mapselect_invalid_stage_7e1be8[];
extern char const unknown_mapselect_empty_83c55c[];
bool MapSelect::Advance_Progression_0058f720(ScenarioClass *scenario,Wstring const &name)
{
    if(!scenario || name==(char const *)0)return false;
    SideClass *side=Sides[HouseTypes[scenario->unknown_11fc]->Side];
    if(!side) {
        Debug_Printf(unknown_mapselect_invalid_side_7e1c44);
        return false;
    }
    Debug_Printf(unknown_mapselect_side_7e1c30,(char const *)side->ID);
    bool failed;
    {
        Wstring sideName((char const *)side->ID);
        failed=!Choices.Load(sideName);
    }
    if(failed) {
        Choices.Clear();
        Debug_Printf(unknown_mapselect_invalid_choices_7e1c04);
        return false;
    }
    MapStage *stage=Choices.Get_Stage(scenario->unknown_2304);
    if(!stage) {
        Choices.Clear();
        Debug_Printf(unknown_mapselect_invalid_stage_7e1be8);
        return false;
    }
    scenario->Set_Scenario_Name_0065e730(unknown_mapselect_empty_83c55c);
    for(int index=0;index<stage->Selections.Count();++index) {
        MapSelection *selection=stage->Selections[index];
        if(selection) {
            MapStage *next=Choices.Find_Stage(selection->Name);
            Wstring candidate(next->Scenario);
            candidate.To_Lower();
            Wstring requested(name);
            requested.To_Lower();
            if(next && candidate==requested) {
                scenario->Set_Scenario_Name_0065e730(next->Scenario.Peek_Buffer());
                scenario->unknown_2304=(unsigned short)Choices.Stage_Index(next);
                break;
            }
        }
    }
    Choices.Clear();
    return strlen(scenario->FileName)>0;
}


extern char unknown_map_select_a6ebd0;
extern char const unknown_map_select_stage_7e1c78[];
extern char const unknown_map_select_theme_7e1c70[];
extern char const unknown_map_select_selected_7e1c60[];
bool MapSelect::Presentation_0058f900(ScenarioClass *scenario)
{
    if(!Init_0058fa20(scenario))return false;
    unknown_map_select_a6ebd0=0;
    MapStage *stage;
    {
        Wstring name(scenario->FileName);
        stage=Choices.Find_Stage_04(name);
    }
    Debug_Printf(unknown_map_select_stage_7e1c78,stage->Name.Buffer);
    if(stage->MapVQ!=(char const *)0) {
        Theme.Play_Song(Theme.From_Name(unknown_map_select_theme_7e1c70));
        char const *selected=stage->Selections[0]->Name.Peek_Buffer();
        if(selected) {
            Debug_Printf(unknown_map_select_selected_7e1c60,selected);
            {
                Wstring name(selected);
                stage=Choices.Find_Stage(name);
            }
            Wstring scenarioName(stage->Scenario);
            if(scenarioName.Length()!=0) {
                scenario->Set_Scenario_Name_0065e730(scenarioName.Get());
                scenario->unknown_2304=(unsigned short)Choices.Stage_Index(stage);
            }
        }
    }
    Deinit_0058fc60();
    return true;
}


extern char const unknown_mapselect_invalid_scenario_7e1cb4[];
extern char const unknown_mapselect_invalid_side_7e1c44[];
extern char const unknown_mapselect_side_7e1c30[];
extern char const unknown_mapselect_invalid_choices_7e1c04[];
extern char const unknown_mapselect_invalid_stage_7e1be8[];
extern char const unknown_mapselect_invalid_font_7e1c90[];
bool MapSelect::Init_0058fa20(ScenarioClass *scenario)
{
    AnimDrawer=0;
    OverlayDrawer=0;
    Font=0;
    if(!scenario) {
        Debug_Printf(unknown_mapselect_invalid_scenario_7e1cb4);
        return false;
    }
    SideClass *side=Sides[HouseTypes[scenario->unknown_11fc]->Side];
    if(!side) {
        Debug_Printf(unknown_mapselect_invalid_side_7e1c44);
        return false;
    }
    Debug_Printf(unknown_mapselect_side_7e1c30,(char const *)side->ID);
    if(!Choices.Load((char const *)side->ID)) {
        Choices.Clear();
        Debug_Printf(unknown_mapselect_invalid_choices_7e1c04);
        return false;
    }
    MapStage *stage=Choices.Find_Stage_04(scenario->FileName);
    if(!stage) {
        Debug_Printf(unknown_mapselect_invalid_stage_7e1be8);
        Deinit_0058fc60();
        return false;
    }
    Font=new MSFont(true);
    if(!Font) {
        Debug_Printf(unknown_mapselect_invalid_font_7e1c90);
        Deinit_0058fc60();
        return false;
    }
    XOffset=(unknown_surface_839954->Get_Width()-640)/2;
    YOffset=(unknown_surface_839954->Get_Height()-400)/2;
    Rect const *textBounds=&Choices.TextRect;
    TextRect=*textBounds;
    TextRect.X+=XOffset;
    TextRect.Y+=YOffset;
    int index=0;
    do {
        WideString description(stage->Description);
        unknown_wrap_text_5ae9a0(description,Font,TextRect.Width);
        stage=Choices.Get_Stage(++index);
    } while(stage);
    VoiceName=(char const *)0;
    VoiceTimer=0;
    VoiceHandle=-1;
    return true;
}


// Original keeps cleanup out of line in this unit: calls 0058fa0c, 0058fb5b.
#pragma auto_inline(off)
void MapSelect::Deinit_0058fc60()
{
    if(Font) { delete Font; Font=0; }
    if(OverlayDrawer) { delete OverlayDrawer; OverlayDrawer=0; }
    if(AnimDrawer) { delete AnimDrawer; AnimDrawer=0; }
    Choices.Clear();
}

#pragma auto_inline(on)


// RA2's idle slot retains the queued-voice clock query but no playback action.
// The value is discarded; querying the real system time remains observable.
void MapSelect::Unknown_Slot2()
{
    if(VoiceName!=(char const *)0) {
        (void)VoiceTimer.Value();
    }
}


void MapSelect::Play_Sound_0058fd00(Wstring const &name)
{
    if(name!=0) {
        MSSfxEntry *sound=Choices.Find_Sound(name);
        if(sound) sound->Play();
    }
}


void MapSelect::Queue_Voice_0058fd30(Wstring const &name,int delay)
{
    VoiceName=name;
    VoiceTimer=CDTimerClass<SystemTimerClass>(delay);
}


// The compiler emits MapSelect's implicit virtual destructor from the
// temporary in Map_Selection_0058f510; no handwritten deletion scaffold.


// 0x0058fcf0: function not yet identified
//
// Body: 5 bytes (frozen inventory bound).
// Referenced from 0 sites.
// Ghidra prototype: undefined4 FUN_0058fcf0(void)
//
// Types and names are Ghidra placeholders;
// rename and retype as the purpose becomes known.
//

namespace seed_0058fcf0 {

unsigned int __stdcall FUN_0058fcf0(int unused_0)
{
    return 0;
}

}  // namespace seed_0058fcf0


// 0x0058fd70: function not yet identified
//
// Body: 3 bytes (frozen inventory bound).
// Referenced from 0 sites.
// Ghidra prototype: void FUN_0058fd70(void)
//
// Types and names are Ghidra placeholders;
// rename and retype as the purpose becomes known.
//

namespace seed_0058fd70 {

void __stdcall FUN_0058fd70(int unused_0)
{
    return;
}

}  // namespace seed_0058fd70


// 0x0058fd80: function not yet identified
//
// Body: 3 bytes (frozen inventory bound).
// Referenced from 0 sites.
// Ghidra prototype: void FUN_0058fd80(void)
//
// Types and names are Ghidra placeholders;
// rename and retype as the purpose becomes known.
//

namespace seed_0058fd80 {

void __stdcall FUN_0058fd80(int unused_0)
{
    return;
}

}  // namespace seed_0058fd80


class Leaf_58fd90 { public: void get(unsigned int unused_argument); };
void Leaf_58fd90::get(unsigned int unused_argument) { unsigned int unused_this_address = (unsigned int)this; unsigned int unused_result; unsigned int unused_value; }
