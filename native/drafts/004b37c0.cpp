// NON-EXACT DRAFT 0x4B37C0 FUN_004b37c0
// entry 0x4B37C0; exact-match score 99.54 (misses_index best); plan "draft as-is"
// origin lane: Scout912 (build/class-work/Scout912/w/eg_f1.cpp)
// Recovered source text by our lanes/workers; never exact-match credit. Tracked by
// Scout1061 (native/drafts.json). Not original bytes.
// @ref ??_C@_04KPAE@Blue?$AA@=007d649c
// @ref ??_C@_07JBC@CREDITS?$AA@=007d64a4
// @ref ??_C@_0BA@PGBN@D?3?2Ra2?2Egos?4CPP?$AA@=007d64ac
// @ref ??_C@_1BI@NKIG@?$AA?$HL?$AAB?$AAa?$AAd?$AA?5?$AAL?$AAa?$AAb?$AAe?$AAl?$AA?$HN?$AA?$AA@=007d64bc
// @ref ??_C@_1CA@OMJE@?$AA?$HL?$AAM?$AAi?$AAs?$AAs?$AAi?$AAn?$AAg?$AA?5?$AAL?$AAa?$AAb?$AAe?$AAl?$AA?$HN?$AA?$AA@=007d64d4
// @ref ??_C@_13ECNF@?$AA?5?$AA?$AA@=007d64f4
// @ref ??_C@_05DKMC@Green?$AA@=007d61fc
// @ref ??_C@_09EEKE@LightGold?$AA@=007d64f8
// @ref ??_C@_0M@ONIL@CREDITS?4TXT?$AA@=007d6504
// Credits scroller004B37C0; RA2 statements derived from the original, reference names are hypotheses.
#include "shared/ego.hpp"
#include "shared/always.hpp"
#include "shared/cc_file_class.hpp"
#include "shared/dsurface.hpp"
#include "shared/text_print.hpp"
#include "shared/string_table.hpp"
#include "shared/theme.hpp"
#include "shared/options.hpp"
#include "shared/keyboard.hpp"
#include "shared/movie_globals.hpp"
#include "shared/t_timer.hpp"
#include "shared/system_timer.hpp"
#include "shared/owner_draw_dialog_control.hpp"
extern DynamicVectorClass<EgoClass *> unknown_EgoClass_list_00855930;
extern char const *unknown_scheme_name_007d83b4;
extern bool unknown_surfaces_restored_008523dc;
extern bool unknown_flag_851cb0;
extern bool unknown_flag_a40d28;
extern Surface *unknown_surface_853118;
extern int unknown_8523e0,unknown_8523e4;
extern TTimerClass<SystemTimerClass> TickCount;
extern wchar_t labels_0085311c[];
extern int credits_scroll_speed_007d6430;
ColorScheme *__fastcall Fetch_Scheme_By_Name(char const *,int);
// @ref ?unknown_scheme_name_007d83b4@@3PBDB=007d83b4
// @ref ?unknown_surfaces_restored_008523dc@@3_NA=008523dc
// @ref ?labels_0085311c@@3PAGA=0085311c
// @ref ?credits_scroll_speed_007d6430@@3HA=007d6430

// Label substitutions are expanded into the unit's original shared wide buffer.
static wchar_t const *Expand_Labels_004b37c0(char const *text)
{
    wchar_t *out=labels_0085311c;
    *out=0;
    while (*text) {
        if (*text=='{') {
            unsigned length=0;
            ++text;
            char const *start=text;
            while (*text && *text!='}') { ++length; ++text; }
            wchar_t const *replacement;
            if (!length) replacement=L"{Missing Label}";
            else if (length>31) replacement=L"{Bad Label}";
            else {
                char label[32];
                char *lptr;
                char guard=*start;
                for (text=start,lptr=label; guard && *text!='}'; guard=text[1],++text) {
                    if (*text!=' ' && *text!='\t') *lptr++=*text;
                }
                *lptr=0;
                if (*text=='}') ++text;
                replacement=StringTable::Load_String(label,0,"D:\\Ra2\\Egos.CPP",284);
            }
            if (replacement) { wcscpy(out,replacement); out+=wcslen(replacement); }
        } else *out++=*text++ & 0xff;
    }
    *out=0;
    return labels_0085311c;
}
void __fastcall Show_Who_Was_Responsible_004b37c0()
{
    unknown_surface_83995c=unknown_surface_839958;
    CCFileClass file("CREDITS.TXT");
    if (file.Is_Available()) {
        char *credits=new char[file.Size()+1];
        file.Read(credits,file.Size());
        unknown_scheme_name_007d83b4="Green";
        Fancy_Text_Printf(L" ",unknown_surface_83995c,&unknown_surface_83995c->Get_Rect(),&Point2D(0,0),Fetch_Scheme_By_Name("LightGold",1),0,0x148);
        int length=file.Size();
        int line=0,column=0;
        char *cursor=credits;
        int y=unknown_surface_839954->Get_Height()+2;
        int lastline=-2;
        do {
            char ch=*cursor++;
            --length;
            switch (ch) {
            case 13: ++line; y+=16; column=0; break;
            case 10: break;
            case 32: ++column; break;
            case 9: column=(column+8)&~7; break;
            default: {
                char last=ch;
                char *start=cursor-1;
                char *parse=cursor-1;
                bool done=false;
                do {
                    ch=*parse++;
                    switch (ch) {
                    case 9: case 10: case 13: done=true; break;
                    case 32: if (last==32) done=true; break;
                    }
                    if (parse>=cursor+length) done=true;
                    last=ch;
                } while (!done);
                if (parse>=cursor+length) break;
                if (parse[-2]==32) --parse;
                unsigned flags=0x48;
                if (column>3 && column<8) flags|=0x100;
                else if (column>8) flags|=0x200;
                if (column>3 && column<8 && line-lastline>1) flags|=0x80;
                else flags|=0x20;
                lastline=line;
                char saved=parse[-1];
                parse[-1]=0;
                EgoClass *ego=new EgoClass(0,y,Expand_Labels_004b37c0(start),flags);
                unknown_EgoClass_list_00855930.Add(ego);
                parse[-1]=saved;
                cursor=parse;
                column+=parse-start;
                length-=parse-start-1;
                if (ch==13) { ++line; y+=16; column=0; }
                else if (ch==9) column=(column+7)&~7;
                break;
            }
            }
        } while (length>0);
        Theme.Stop();
        unknown_surface_839954->Fill(0);
        Unknown_004E0C90(true,unknown_surface_839954,0);
        unknown_surface_853118=new DSurface(unknown_int_839648,unknown_int_83964c,false);
        unknown_surface_853118->Copy_From(*unknown_surface_839954,false,true);
        WWKeyboard->Clear();
        float oldvolume=Options.ScoreVolume;
        unknown_surface_83995c=unknown_surface_839954;
        if ((double)oldvolume==0.0) Options.Set_Score_Volume(0.4f,false);
        Theme.Queue_Song(Theme.From_Name("CREDITS"));
        int time=TickCount.Time();
        int frame=0,picture_frame=0,slide=0;
        bool fresh=false;
        int key=0;
        WWMouse->Hide_Mouse();
        while (unknown_EgoClass_list_00855930.Count()) {
            ++frame;
            if (unknown_surfaces_restored_008523dc) {
                unknown_surfaces_restored_008523dc=false;
                fresh=true;
                if (!unknown_flag_851cb0) {
                    unknown_surface_839950->Fill(0);
                    unknown_surface_853118->Fill(0);
                }
                WWMouse->Show_Mouse();
                WWMouse->Hide_Mouse();
            }
            ++picture_frame;
            if (picture_frame>200 && slide<16) { ++slide; picture_frame=0; }
            int i;
            for (i=unknown_EgoClass_list_00855930.Count()-1;i>=0;--i) {
                if (unknown_EgoClass_list_00855930[i]->Advance(2)) {
                    delete unknown_EgoClass_list_00855930[i];
                    unknown_EgoClass_list_00855930.Delete(i);
                    break;
                }
            }
            Rect dest(0,32,unknown_8523e0,unknown_8523e4-66);
            Rect source(0,34,unknown_8523e0,unknown_8523e4-66);
            Rect top(0,0,unknown_8523e0,34);
            Rect bottom(0,unknown_8523e4-34,unknown_8523e0,34);
            unknown_surface_853118->Fill_Rect(top,0);
            unknown_surface_853118->Fill_Rect(bottom,0);
            for (i=unknown_EgoClass_list_00855930.Count()-1;i>=0;--i) unknown_EgoClass_list_00855930[i]->Draw(fresh);
            int step=0;
            unsigned short *pixels=(unsigned short *)unknown_surface_853118->Lock();
            if (pixels) {
                step=unknown_surface_853118->Stride()/2;
                for (int fade_y=0;fade_y<32;fade_y++) {
                    int alpha=min(256-fade_y*8,255);
                    int first=step*fade_y;
                    int last=step*(unknown_8523e4-fade_y-1);
                    for (int x=0;x<unknown_8523e0;++x) {
                        (pixels+first)[x]=OwnerDraw::Blend_Color((pixels+first)[x],0,alpha);
                        (pixels+last)[x]=OwnerDraw::Blend_Color((pixels+last)[x],0,alpha);
                    }
                }
                unknown_surface_853118->Unlock();
            }
            if (unknown_flag_a40d28) {
                if (fresh) unknown_surface_839950->Copy_From(*unknown_surface_853118,false,true);
                else {
                    unknown_surface_839950->Copy_From(top,*unknown_surface_853118,top,false,true);
                    unknown_surface_839950->Copy_From(dest,*unknown_surface_839950,source,false,true);
                    unknown_surface_839950->Copy_From(bottom,*unknown_surface_853118,bottom,false,true);
                }
            }
            fresh=false;
            Service_Game_IO_00481fb0();
            while (TickCount.Time()-time<frame*credits_scroll_speed_007d6430 && !WWKeyboard->Check()) {}
            key=0;
            if (WWKeyboard->Check()) { key=WWKeyboard->Get(); if (key==27) break; }
        }
        while (picture_frame<=187) {
            if (picture_frame<150 && picture_frame>47) picture_frame=150;
            ++frame;
            ++picture_frame;
            Service_Game_IO_00481fb0();
            while (TickCount.Time()-time<frame*credits_scroll_speed_007d6430 && !WWKeyboard->Check()) {}
        }
        unknown_surface_839954->Fill(0);
        Unknown_004E0C90(true,unknown_surface_839954,0);
        WWMouse->Show_Mouse();
        unknown_scheme_name_007d83b4="Blue";
        Theme.Stop();
        Options.Set_Score_Volume(oldvolume,false);
        delete unknown_surface_853118;
        delete [] credits;
        while (unknown_EgoClass_list_00855930.Count()>0) {
            delete unknown_EgoClass_list_00855930[0];
            unknown_EgoClass_list_00855930.Delete(0);
        }
        unknown_EgoClass_list_00855930.Clear();
    }
}
