// coopcamp.cpp: the original translation unit re-filed as one source (Route A): the accepted
// bodies of its canonical compile group in address order, under the unit's own header
// state (its switches and includes in their original order). The module map
// manifests/original_target/modules/coopcamp.json carries the addresses and the rows that
// stay in their own files.


#include "shared/quoted_printable.hpp"
#include "shared/crt_memory.hpp"
#define DESCRIPTION_LIST_INLINE_GET_RECORD
#define VECTOR_IMPLEMENT_VECTOR_ACCESSORS
#define VECTOR_IMPLEMENT_DYNAMIC_ACCESSORS
#define VECTOR_IMPLEMENT_VECTOR_DESTRUCTOR
#include "shared/coop_campaign.hpp"
#include "shared/crt_string.hpp"
#include "shared/mplayer_score_type.hpp"
#include "shared/id_source.hpp"
#include "shared/comm_buffer.hpp"
#define VECTOR_IMPLEMENT_DYNAMIC_ADD
#define VECTOR_IMPLEMENT_DYNAMIC_DELETE_INDEX
#include "shared/vector_class.hpp"
#define INDEX_DECLARE_CONSTRUCTOR
#define LIST_DECLARE_CONSTRUCTOR
#include "shared/cc_file_class.hpp"
#include "shared/ccini_class.hpp"
#include "shared/unknown_description_list.hpp"
#include "shared/multiplayer_game_mode.hpp"
#include "shared/random.hpp"
#include "shared/debug_printf.hpp"
#include "shared/lobby.hpp"


char __fastcall Hex_Digit_0048de60(int value)
{
    if(value<0 || value>15) return 0;
    if(value<10) return value+'0';
    return value+'7';
}


int __fastcall Hex_Value_0048de80(char value)
{
    if(value<='9' && value>='0') return value-'0';
    if(value<='f' && value>='a') return value-'a'+10;
    if(value<='F' && value>='A') return value-'A'+10;
    return 0;
}


// Quoted-printable conversion for campaign section names, using the shared output buffer.
extern "C" int __cdecl isalnum(int);
extern char unknown_encoded_buffer_00851170[];
char *__fastcall Quoted_Printable_Encode_0048dec0(char const *input)
{
    char const *cursor=input;
    int length;
    for(length=0; *cursor && length < 1021; ++length,++cursor) {
        if(!isalnum(*cursor)) {
            unknown_encoded_buffer_00851170[length] = '=';
            ++length;
            unknown_encoded_buffer_00851170[length] = Hex_Digit_0048de60(*cursor >> 4);
            ++length;
            unknown_encoded_buffer_00851170[length] = Hex_Digit_0048de60(*cursor & 15);
        } else unknown_encoded_buffer_00851170[length] = *cursor;
    }
    unknown_encoded_buffer_00851170[length]=0;
    return unknown_encoded_buffer_00851170;
}


extern char unknown_decoded_buffer_00851570[];
char *__fastcall Quoted_Printable_Decode_0048df70(char const *input)
{
    char *output=unknown_decoded_buffer_00851570;
    while(*input) {
        if(*input=='=') {
            if(!input[1]) break;
            *output=Hex_Value_0048de80(input[1]);
            ++input;
            if(input[1]) {
                *output <<= 4;
                *output |= Hex_Value_0048de80(input[1]);
                ++input;
            }
        } else *output=*input;
        ++input;
        ++output;
    }
    *output=0;
    return unknown_decoded_buffer_00851570;
}


CoopCampaignClass::CoopCampaignClass()
{
    FirstName[0]=0;
    SecondName[0]=0;
    FirstHouse=-2;
    SecondHouse=-2;
    FirstColor=-2;
    SecondColor=-2;

    MapNames=0;
    CampaignType=-1;
    Initialized=false;
    MapCount=0;
    CurrentMap=-1;
    Kills[0]=0;
    Kills[1]=0;
    Built[0]=0;
    Built[1]=0;
    Lost[0]=0;
    Lost[1]=0;
    Time=0;
    Score[0]=0;
    Score[1]=0;
}


char *__fastcall Wide_To_Narrow_006fa120(unsigned short const *string);
CoopCampaignClass::CoopCampaignClass(unsigned short const *first, unsigned short const *second)
{
    Set_Names(first,second);
    FirstHouse=-2;
    SecondHouse=-2;
    FirstColor=-2;
    SecondColor=-2;
    MapNames=0;
    CampaignType=-1;
    CurrentMap=-1;
    Initialized=false;
    MapCount=0;
    Kills[0]=0;
    Kills[1]=0;
    Built[0]=0;
    Built[1]=0;
    Lost[0]=0;
    Lost[1]=0;
    Time=0;
    Score[0]=0;
    Score[1]=0;
}


void __cdecl operator delete[](void *block);
CoopCampaignClass::~CoopCampaignClass()
{
    if(MapNames) {
        for(int i=0;i<MapCount;++i) {
            if(MapNames[i]) delete [] MapNames[i];
        }
        delete [] MapNames;
    }
}


char *__fastcall Wide_To_Narrow_006fa120(unsigned short const *string);
void CoopCampaignClass::Set_Names(unsigned short const *first, unsigned short const *second)
{
    if(first) strcpy(FirstName,Wide_To_Narrow_006fa120(first));
    else FirstName[0]=0;
    if(second) strcpy(SecondName,Wide_To_Narrow_006fa120(second));
    else SecondName[0]=0;
}


void __fastcall Collect_Coop_Player_Score_0048e210(MPlayerScoreType &output, bool skip_first);
char *__fastcall Wide_To_Narrow_006fa120(unsigned short const *string);
extern "C" int __cdecl sprintf(char *destination, char const *format, ...);
extern char const next_map_log_007d4f38[];
extern char const local_player_log_007d4f24[];
extern char const assume_local_player_log_007d4f08[];
extern char const game_time_log_007d4ed4[];
extern char const time_format_007d4ef8[];
inline void Format_Time(int seconds, char *buffer)
{
    if(buffer) {
        if(seconds/3600>99) seconds=359999;
        int hours=seconds/3600;
        seconds-=hours*3600;
        int minutes=seconds/60;
        seconds-=minutes*60;
        sprintf(buffer,time_format_007d4ef8,hours,minutes,seconds);
    }
}
bool CoopCampaignClass::Next_Map_0048e360()
{
    if(!Initialized) return false;
    Debug_Printf(next_map_log_007d4f38);
    MPlayerScoreType first;
    Collect_Coop_Player_Score_0048e210(first,false);
    MPlayerScoreType second;
    Collect_Coop_Player_Score_0048e210(second,true);
    if(strcmp(FirstName,Wide_To_Narrow_006fa120(first.Name))==0) {
        Debug_Printf(local_player_log_007d4f24,Wide_To_Narrow_006fa120(first.Name));
        Kills[0]+=first.Kills; Kills[1]+=second.Kills;
        Built[0]+=first.Built; Built[1]+=second.Built;
        Lost[0]+=first.Lost; Lost[1]+=second.Lost;
        Score[0]+=first.Score; Score[1]+=second.Score;
    } else {
        Debug_Printf(assume_local_player_log_007d4f08,Wide_To_Narrow_006fa120(second.Name));
        Kills[1]+=first.Kills; Kills[0]+=second.Kills;
        Built[1]+=first.Built; Built[0]+=second.Built;
        Lost[1]+=first.Lost; Lost[0]+=second.Lost;
        Score[1]+=first.Score; Score[0]+=second.Score;
    }
    char game_time[256];
    Format_Time(unknown_id_source_a3d290->Elapsed_Time().Time()/60,game_time);
    Time+=unknown_id_source_a3d290->Elapsed_Time().Time();
    char total_time[256];
    Format_Time(Time,total_time);
    Debug_Printf(game_time_log_007d4ed4,game_time,total_time);
    ++CurrentMap;
    return true;
}


bool CoopCampaignClass::Get_Campaign_Type_0048e6b0(int *value)
{
    if(!Initialized) return false;
    *value=CampaignType;
    return true;
}


bool CoopCampaignClass::Get_Map_Count_0048e6d0(int *value)
{
    if(!Initialized) return false;
    *value=MapCount;
    return true;
}


bool CoopCampaignClass::Get_Current_Map_0048e6f0(int *value)
{
    if(!Initialized) return false;
    *value=CurrentMap;
    return true;
}


void __fastcall Collect_Coop_Player_Score_0048e210(MPlayerScoreType &output, bool skip_first);
char *__fastcall Wide_To_Narrow_006fa120(unsigned short const *string);
bool CoopCampaignClass::Get_Player_Scores_0048e710(DynamicVectorClass<MPlayerScoreType *> &scores, int *time)
{
    if(!Initialized) return false;
    while(scores.Count()!=0) {
        delete scores[0];
        scores.Delete(0);
    }
    MPlayerScoreType *first=new MPlayerScoreType;
    Collect_Coop_Player_Score_0048e210(*first,false);
    MPlayerScoreType *second=new MPlayerScoreType;
    Collect_Coop_Player_Score_0048e210(*second,true);
    if(strcmp(FirstName,Wide_To_Narrow_006fa120(first->Name))!=0) {
        MPlayerScoreType *temp=first;
        first=second;
        second=temp;
    }
    for(int i=0;i<2;++i) {
        MPlayerScoreType *score=i==0 ? first : second;
        score->Kills=Kills[i];
        score->Built=Built[i];
        score->Lost=Lost[i];
        score->Winner=CurrentMap;
        score->Score=Score[i];
        scores.Add(score);
    }
    *time=Time;
    return true;
}


extern "C" int __cdecl isalnum(int value);
extern char unknown_encoded_buffer_00851170[];
extern char const campaign_save_filename_007d4f48[];
inline void Prepare_Campaign_Save_File(FileClass &file)
{
    file.Set_Name(campaign_save_filename_007d4f48);
    file.Open(FILE_RIGHTS_WRITE);
}
bool CoopCampaignClass::Clear_Saved_Campaign_0048e870()
{
    if(FirstName[0] && SecondName[0]) {
        CCFileClass saved(campaign_save_filename_007d4f48);
        CCINIClass ini;
        ini.Load(saved,false,false);
        saved.Close();
        char section[100];
        strcpy(section,Quoted_Printable_Encode_0048dec0(FirstName));
        strcat(section,Quoted_Printable_Encode_0048dec0(SecondName));
        CCFileClass output;
        Prepare_Campaign_Save_File(output);
        ini.Clear(section,0);
        ini.Save(output,false);
        output.Close();
    }
    return true;
}


extern char const campaign_save_filename_007d4f48[];
char *__fastcall Quoted_Printable_Encode_0048dec0(char const *input);
extern char const campaign_key_CurrentMap_007d4fec[];
extern char const campaign_key_Score_007d4fe4[];
extern char const campaign_key_CampaignType_007d4fd4[];
extern char const campaign_key_House1_007d4fcc[];
extern char const campaign_key_Color1_007d4fc4[];
extern char const campaign_key_House2_007d4fbc[];
extern char const campaign_key_Color2_007d4fb4[];
extern char const campaign_key_Kills1_007d4fac[];
extern char const campaign_key_Kills2_007d4fa4[];
extern char const campaign_key_Built1_007d4f9c[];
extern char const campaign_key_Built2_007d4f94[];
extern char const campaign_key_Lost1_007d4f8c[];
extern char const campaign_key_Lost2_007d4f84[];
extern char const campaign_key_Score1_007d4f7c[];
extern char const campaign_key_Score2_007d4f74[];
extern char const campaign_key_Time_007d4f6c[];
extern char const campaign_key_Map_format_007d4f64[];
extern char const campaign_key_Map_default_007d4f58[];
bool CoopCampaignClass::Load_Saved_Campaign_0048eb30()
{
    if(!FirstName[0] || !SecondName[0]) return false;
    char section[100];
    strcpy(section,Quoted_Printable_Encode_0048dec0(FirstName));
    strcat(section,Quoted_Printable_Encode_0048dec0(SecondName));
    CCFileClass saved(campaign_save_filename_007d4f48);
    CCINIClass ini;
    if(!ini.Load(saved,false,false)) return false;
    CurrentMap=ini.Get_Int(section,campaign_key_CurrentMap_007d4fec,0);
    ini.Get_Int(section,campaign_key_Score_007d4fe4,0);
    CampaignType=ini.Get_Int(section,campaign_key_CampaignType_007d4fd4,-1);
    FirstHouse=ini.Get_HousesType(section,campaign_key_House1_007d4fcc,-2);
    FirstColor=ini.Get_Int(section,campaign_key_Color1_007d4fc4,-2);
    SecondHouse=ini.Get_HousesType(section,campaign_key_House2_007d4fbc,-2);
    SecondColor=ini.Get_Int(section,campaign_key_Color2_007d4fb4,-2);
    Kills[0]=ini.Get_Int(section,campaign_key_Kills1_007d4fac,0);
    Kills[1]=ini.Get_Int(section,campaign_key_Kills2_007d4fa4,0);
    Built[0]=ini.Get_Int(section,campaign_key_Built1_007d4f9c,0);
    Built[1]=ini.Get_Int(section,campaign_key_Built2_007d4f94,0);
    Lost[0]=ini.Get_Int(section,campaign_key_Lost1_007d4f8c,0);
    Lost[1]=ini.Get_Int(section,campaign_key_Lost2_007d4f84,0);
    Score[0]=ini.Get_Int(section,campaign_key_Score1_007d4f7c,0);
    Score[1]=ini.Get_Int(section,campaign_key_Score2_007d4f74,0);
    Time=ini.Get_Int(section,campaign_key_Time_007d4f6c,0);
    UnknownDescriptionList_00490E00 *campaigns=new UnknownDescriptionList_00490E00;
    if(CampaignType>=0 && CampaignType<campaigns->Count) {
        UnknownDescriptionRecord_00490E00 const &record=campaigns->Entries[CampaignType];
        MapCount=(signed char)record.MapCount;
        MapNames=new char *[(signed char)record.MapCount];
        for(int i=0;i<MapCount;++i) {
            char key[32];
            sprintf(key,campaign_key_Map_format_007d4f64,i+1);
            MapNames[i]=new char[260];
            ini.Get_String(section,key,campaign_key_Map_default_007d4f58,MapNames[i],260);
        }
        delete campaigns;
        Initialized=true;
        return true;
    }
    MapCount=0;
    delete campaigns;
    return false;
}


extern char const campaign_save_filename_007d4f48[];
char *__fastcall Quoted_Printable_Encode_0048dec0(char const *input);
extern char const campaign_key_CurrentMap_007d4fec[];
extern char const campaign_key_Score_007d4fe4[];
extern char const campaign_key_CampaignType_007d4fd4[];
extern char const campaign_key_House1_007d4fcc[];
extern char const campaign_key_Color1_007d4fc4[];
extern char const campaign_key_House2_007d4fbc[];
extern char const campaign_key_Color2_007d4fb4[];
extern char const campaign_key_Kills1_007d4fac[];
extern char const campaign_key_Kills2_007d4fa4[];
extern char const campaign_key_Built1_007d4f9c[];
extern char const campaign_key_Built2_007d4f94[];
extern char const campaign_key_Lost1_007d4f8c[];
extern char const campaign_key_Lost2_007d4f84[];
extern char const campaign_key_Score1_007d4f7c[];
extern char const campaign_key_Score2_007d4f74[];
extern char const campaign_key_Time_007d4f6c[];
extern char const campaign_key_Map_format_007d4f64[];
extern char const campaign_key_Map_default_007d4f58[];
extern int unknown_player_slot_types_00a3d300[];
void Prepare_Campaign_Save_File(FileClass &file);
bool CoopCampaignClass::Save_Campaign_0048f040()
{
    if(Initialized && FirstName[0] && SecondName[0]) {
    UnknownDescriptionList_00490E00 *campaigns=new UnknownDescriptionList_00490E00;
    UnknownDescriptionRecord_00490E00 *record;
    if(!campaigns->Get_Record_00490C50(CampaignType,&record)) {
        delete campaigns;
        return false;
    }
    {
        CCFileClass saved(campaign_save_filename_007d4f48);
        CCINIClass ini;
        ini.Load(saved,false,false);
        saved.Close();
        char section[100];
        strcpy(section,Quoted_Printable_Encode_0048dec0(FirstName));
        strcat(section,Quoted_Printable_Encode_0048dec0(SecondName));
        CCFileClass output;
        Prepare_Campaign_Save_File(output);
        ini.Clear(section,0);
        ini.Put_Int(section,campaign_key_CurrentMap_007d4fec,CurrentMap);
        ini.Put_Int(section,campaign_key_CampaignType_007d4fd4,CampaignType);
        for(int i=0;i<(signed char)record->MapCount;++i) {
            char key[32];
            sprintf(key,campaign_key_Map_format_007d4f64,i+1);
            ini.Put_String(section,key,MapNames[i]);
        }
        if(unknown_players_a3fa7c.Count()) {
            ini.Put_HousesType(section,campaign_key_House1_007d4fcc,unknown_players_a3fa7c[0]->unknown_66e290());
            ini.Put_Int(section,campaign_key_Color1_007d4fc4,unknown_players_a3fa7c[0]->Get_Color_0066e2a0());
        }
        for(int player=1;player<unknown_players_a3fa7c.Count();++player) {
            if(unknown_player_slot_types_00a3d300[player-1]==-1) {
                ini.Put_HousesType(section,campaign_key_House2_007d4fbc,unknown_players_a3fa7c[player]->unknown_66e290());
                ini.Put_Int(section,campaign_key_Color2_007d4fb4,unknown_players_a3fa7c[player]->Get_Color_0066e2a0());
                break;
            }
        }
        ini.Put_Int(section,campaign_key_Time_007d4f6c,Time);
        ini.Put_Int(section,campaign_key_Kills1_007d4fac,Kills[0]);
        ini.Put_Int(section,campaign_key_Kills2_007d4fa4,Kills[1]);
        ini.Put_Int(section,campaign_key_Built1_007d4f9c,Built[0]);
        ini.Put_Int(section,campaign_key_Built2_007d4f94,Built[1]);
        ini.Put_Int(section,campaign_key_Lost1_007d4f8c,Lost[0]);
        ini.Put_Int(section,campaign_key_Lost2_007d4f84,Lost[1]);
        ini.Put_Int(section,campaign_key_Score1_007d4f7c,Score[0]);
        ini.Put_Int(section,campaign_key_Score2_007d4f74,Score[1]);
        ini.Save(output,false);
        output.Close();
    }
    delete campaigns;
    Initialized=true;
    return true;
    }
    return false;
}


extern "C" char *__cdecl strncpy(char *destination, char const *source, unsigned int count);
bool CoopCampaignClass::Load_Campaign_0048f590(int index)
{
    UnknownDescriptionList_00490E00 *campaigns=new UnknownDescriptionList_00490E00;
    if(index>=0 && index<campaigns->Count) {
        UnknownDescriptionRecord_00490E00 *record=&campaigns->Entries[index];
        Initialized=true;
        CurrentMap=-1;
        Kills[0]=0;
        Kills[1]=0;
        Built[0]=0;
        Built[1]=0;
        Score[0]=0;
        Score[1]=0;
        Lost[0]=0;
        Lost[1]=0;
        Time=0;
        if(CampaignType!=index) {
            if(MapNames) {
                for(int i=0;i<MapCount;++i) {
                    if(MapNames[i]) delete [] MapNames[i];
                }
                delete [] MapNames;
            }
            MapNames=0;
            MapCount=0;
            MapNames=new char *[(signed char)record->MapCount];
            for(int m=0;m<(signed char)record->MapCount;++m) {
                int choice=unknown_id_source_a3d290->Random()(0,2);
                MapNames[m]=new char[260];
                strncpy(MapNames[m],record->Maps+(m*3+choice)*260,259);
            }
            MapCount=(signed char)record->MapCount;
            CurrentMap=0;
        }
        CampaignType=index;
        delete campaigns;
        return true;
    }
    delete campaigns;
    return false;
}


bool CoopCampaignClass::Add_Score_0048f730(int index, int time, MPlayerScoreType first, MPlayerScoreType second)
{
    if(index>=0 && index<MapCount) {
        Kills[0]+=first.Kills;
        Kills[1]+=second.Kills;
        Built[0]+=first.Built;
        Built[1]+=second.Built;
        Lost[0]+=first.Lost;
        Lost[1]+=second.Lost;
        Score[0]+=first.Score;
        Score[1]+=second.Score;
        Time=time;
        CurrentMap=index;
        return true;
    }
    return false;
}


extern "C" char *__cdecl strncpy(char *destination, char const *source, unsigned int count);
extern char const unknown_set_campaign_log_007d4ff8[];
bool CoopCampaignClass::Set_Campaign_Info_0048f7d0(UnknownDescriptionList_00490E00 const &campaigns, int index, DynamicVectorClass<int> choices, char const *second_name, char const *first_name)
{
    if(index>=0 && index<campaigns.Count) {
        UnknownDescriptionRecord_00490E00 const *record=&campaigns.Entries[index];
        CurrentMap=-1;
        Kills[0]=0;
        Kills[1]=0;
        Built[0]=0;
        Built[1]=0;
        Lost[0]=0;
        Lost[1]=0;
        Score[0]=0;
        Score[1]=0;
        Time=0;
        if((signed char)record->MapCount!=choices.Count()) return false;
        CampaignType=index;
        Initialized=true;
        if(MapNames) {
            for(int i=0;i<MapCount;++i) {
                if(MapNames[i]) delete [] MapNames[i];
            }
            delete [] MapNames;
        }
        MapNames=0;
        MapCount=0;
        MapNames=new char *[choices.Count()];
        for(int m=0;m<choices.Count();++m) {
            MapNames[m]=new char[260];
            strncpy(MapNames[m],record->Maps+(m*3+choices[m])*260,259);
        }
        MapCount=choices.Count();
        CurrentMap=0;
        if(first_name) strcpy(FirstName,Quoted_Printable_Decode_0048df70(first_name));
        else FirstName[0]=0;
        if(second_name) strcpy(SecondName,Quoted_Printable_Decode_0048df70(second_name));
        else SecondName[0]=0;
        Debug_Printf(unknown_set_campaign_log_007d4ff8,CampaignType);
    }
    return Initialized;
}


bool CoopCampaignClass::Get_Map_Name_0048fb40(char **name,int index)
{
    bool result=false;
    *name=0;
    if(index==-1) index=CurrentMap;
    if(index<0 || index>=MapCount) index=-1;
    if(Initialized) {
        if(index!=-1) *name=MapNames[index];
        if(*name!=0) result=true;
    }
    return result;
}


bool CoopCampaignClass::Get_First_Player_Settings_0048fb90(int *house,int *color)
{
    if(Initialized) {
        *house=FirstHouse;
        *color=FirstColor;
        return true;
    }
    return false;
}


bool CoopCampaignClass::Get_Second_Player_Settings_0048fbc0(int *house,int *color)
{
    if(Initialized) {
        *house=SecondHouse;
        *color=SecondColor;
        return true;
    }
    return false;
}


extern char const campaign_save_filename_007d4f48[];
char *__fastcall Quoted_Printable_Encode_0048dec0(char const *input);
bool CoopCampaignClass::Has_Saved_Campaign_0048fbf0()
{
    if(FirstName[0] && SecondName[0]) {
    CCFileClass saved(campaign_save_filename_007d4f48);
    CCINIClass ini;
    if(!ini.Load(saved,false,false)) {
        saved.Close();
    } else {
    saved.Close();
    char section[100];
    strcpy(section,Quoted_Printable_Encode_0048dec0(FirstName));
    strcat(section,Quoted_Printable_Encode_0048dec0(SecondName));
    return ini.Find_Section(section)!=0;
    }
    }
    return false;
}


extern char const campaign_save_filename_007d4f48[];
char *__fastcall Quoted_Printable_Encode_0048dec0(char const *input);
extern char const campaign_key_CurrentMap_007d4fec[];
extern char const campaign_key_Score_007d4fe4[];
extern char const campaign_key_CampaignType_007d4fd4[];
extern char const campaign_key_House1_007d4fcc[];
extern char const campaign_key_Color1_007d4fc4[];
extern char const campaign_key_House2_007d4fbc[];
extern char const campaign_key_Color2_007d4fb4[];
extern char const campaign_key_Kills1_007d4fac[];
extern char const campaign_key_Kills2_007d4fa4[];
extern char const campaign_key_Built1_007d4f9c[];
extern char const campaign_key_Built2_007d4f94[];
extern char const campaign_key_Lost1_007d4f8c[];
extern char const campaign_key_Lost2_007d4f84[];
extern char const campaign_key_Score1_007d4f7c[];
extern char const campaign_key_Score2_007d4f74[];
extern char const campaign_key_Time_007d4f6c[];
extern char const campaign_key_Map_format_007d4f64[];
extern char const campaign_key_Map_default_007d4f58[];
bool CoopCampaignClass::Saved_Map_Is_Undefined_0048fe70(char *map)
{
    if(FirstName[0] && SecondName[0]) {
    CCFileClass saved(campaign_save_filename_007d4f48);
    CCINIClass ini;
    if(!ini.Load(saved,false,false)) {
        saved.Close();
    } else {
    saved.Close();
    char section[100];
    strcpy(section,Quoted_Printable_Encode_0048dec0(FirstName));
    strcat(section,Quoted_Printable_Encode_0048dec0(SecondName));
    int current=ini.Get_Int(section,campaign_key_CurrentMap_007d4fec,0);
    char key[32];
    sprintf(key,campaign_key_Map_format_007d4f64,current+1);
    ini.Get_String(section,key,campaign_key_Map_default_007d4f58,map,260);
    if(strcmp(map,campaign_key_Map_default_007d4f58)==0) return true;
    goto map_is_defined;
    }
    }
map_is_defined:
    return false;
}


extern "C" int __cdecl _stricmp(char const *first,char const *second);
char *__fastcall Quoted_Printable_Encode_0048dec0(char const *input);
extern char const campaign_options_format_007d5068[];
extern char const campaign_options_separator_007d5060[];
extern char const campaign_options_comma_007ce9c0[];
extern char const campaign_map_choice_format_007cfa6c[];
extern char const campaign_sending_log_007d5030[];
extern char const campaign_sent_log_007d5024[];
bool CoopCampaignClass::Send_Campaign_Options_00490150(UnknownDescriptionList_00490E00 &campaigns)
{
    if(!Initialized) return false;
    UnknownDescriptionRecord_00490E00 *record;
    if(!campaigns.Get_Record_00490C50(CampaignType,&record)) return false;
    char message[400];
    sprintf(message,campaign_options_format_007d5068,CampaignType,CurrentMap,Time,
            Kills[0],Built[0],Lost[0],Score[0],Kills[1],Built[1],Lost[1],Score[1]);
    for(int map=0;map<MapCount;++map) {
        int choice=0;
        for(int option=0;option<3;++option) {
            if(_stricmp(record->Maps+map*0x30c+option*260,MapNames[map])==0) {
                choice=option;
                break;
            }
        }
        char choice_text[16];
        sprintf(choice_text,campaign_map_choice_format_007cfa6c,choice);
        strcat(message,choice_text);
    }
    strcat(message,campaign_options_separator_007d5060);
    strcat(message,Quoted_Printable_Encode_0048dec0(FirstName));
    strcat(message,campaign_options_comma_007ce9c0);
    strcat(message,Quoted_Printable_Encode_0048dec0(SecondName));
    Debug_Printf(campaign_sending_log_007d5030,message);
    Send_Game_Options_005c3c60(message);
    Debug_Printf(campaign_sent_log_007d5024);
    return true;
}


extern "C" unsigned char vtable_491340[]; extern "C" void __cdecl release_buffer_491340(void*); extern "C" void __cdecl release_object_491340(void*);
class Leaf_491340 { public: void* get(unsigned int flags); };
void* Leaf_491340::get(unsigned int flags) { unsigned int pointer = *(unsigned int*)((char*)this + 4); *(unsigned int*)this=(unsigned int)vtable_491340; if(pointer && *(unsigned char*)((char*)this + 13)){ release_buffer_491340((void*)pointer); *(unsigned int*)((char*)this + 4)=0; } *(unsigned char*)((char*)this + 13)=0; *(unsigned int*)((char*)this + 8)=0; if(flags & 1) release_object_491340(this); return this; }


extern "C" unsigned char vtable_491390[]; extern "C" void __cdecl release_buffer_491390(void*); extern "C" void __cdecl release_object_491390(void*);
class Leaf_491390 { public: void* get(unsigned int flags); };
void* Leaf_491390::get(unsigned int flags) { unsigned int pointer = *(unsigned int*)((char*)this + 4); *(unsigned int*)this=(unsigned int)vtable_491390; if(pointer && *(unsigned char*)((char*)this + 13)){ release_buffer_491390((void*)pointer); *(unsigned int*)((char*)this + 4)=0; } *(unsigned char*)((char*)this + 13)=0; *(unsigned int*)((char*)this + 8)=0; if(flags & 1) release_object_491390(this); return this; }


extern "C" unsigned char table_4913e0[]; extern "C" void* __cdecl storage_4913e0(unsigned int);
class Storage_4913e0 { unsigned int table_word; unsigned int* items; unsigned int count; unsigned char valid; unsigned char owns; public: void* get(unsigned int requested,unsigned int* buffer); };
void* Storage_4913e0::get(unsigned int requested,unsigned int* buffer) { items=0; count=requested; valid=1; owns=0; table_word=(unsigned int)table_4913e0; if(requested) { if(buffer) { items=buffer; return this; } items=(unsigned int*)storage_4913e0(requested << 2); owns=1; } return this; }
