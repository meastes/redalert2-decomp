// @ref ??_C@_06FOCG@INVISO?$AA@=007cecc0
// AnimClass AI 00421710: RA2 visibility, bounce effects, tiberium and frame lifecycle.
#include "shared/anim.hpp"
#include "shared/rules.hpp"
#include "shared/map_view.hpp"
#include "shared/cell.hpp"
#include "shared/overlay.hpp"
#include "shared/options.hpp"
#include "shared/id_source.hpp"
#include "shared/shape_file.hpp"
#include "shared/tactical.hpp"
#include "shared/crt_string.hpp"
#include "shared/fast_math.hpp"
// @ref ?OverlayTypes@@3V?$DynamicVectorClass@PAVOverlayTypeClass@@@@A=00a35de8
extern DynamicVectorClass<OverlayTypeClass *> OverlayTypes;
// @ref ?unknown_rect_839630@@3VRect@@A=00839630
extern Rect unknown_rect_839630;
bool __fastcall Building_Player_Detector_Covers_Cell_00438a60(HouseClass const *house, CellClass const *cell);
void AnimClass::AI()
{
    if (!unknown_178 && Class->StartSound!=-1)
        Voc_Update_At(Center_Coord(),&unknown_180);
    if (Class->IsFlamingGuy) {
        Flaming_Guy_AI();
        ObjectClass::AI();
    }
    if (Class->PsiWarning) {
        CellClass *cell=&Map[Center_Coord()];
        HouseClass *house=unknown_160;
        bool covered=Building_Player_Detector_Covers_Cell_00438a60(house,cell);
        unknown_17d=!covered;
    }
    if (Class==Rules->Behind) {
        if (!Options.ShowHidden) unknown_17d=true;
        else unknown_17d=false;
    }
    if (Class->HideIfNoOre) {
        AbstractClass *object=unknown_slot105();
        CellClass *cell=(CellClass *)object;
        if (!cell || !cell->Get_Tiberium_Value()) unknown_17d=true;
        else unknown_17d=false;
    }
    if (unknown_103 && unknown_104==Stage) unknown_103=false;
    if (unknown_174) {
        int result=unknown_slot116();
        if (result==2 || result==1) {
            CellClass &cell=Map[Position_Coord_004f3f40()];
            int land=cell.unknown_ec;
            bool water=land==2;
            int z=Position_Coord_004f3f40().Z;
            int height=Map.Get_Height_GL(Position_Coord_004f3f40())+unknown_geometry_j;
            bool bridge=z>=height;
            if (water && !bridge) {
                if (Class->IsMeteor) {
                    new AnimClass(Rules->SplashList[Rules->SplashList.Count()-1],Position_Coord_004f3f40()+CoordStruct(0,0,3),0,1,0x600,0);
                } else {
                    new AnimClass(Rules->unknown_09c,Position_Coord_004f3f40(),0,1,0x600,0);
                    new AnimClass(Rules->SplashList[0],Position_Coord_004f3f40()+CoordStruct(0,0,3),0,1,0x600,0);
                }
            } else {
                if (Class->ExpireAnim) {
                    Vector3 position=unknown_108.Position;
                    new AnimClass(Class->ExpireAnim,CoordStruct((int)position.X,(int)position.Y,(int)position.Z),0,1,0x2600,-30);
                    Unknown_0047E2C0(unknown_108.Unknown_00436F80(),(int)Class->Damage,0,Class->Warhead,1);
                    Unknown_0047F600(unknown_108.Unknown_00436F80(),(int)Class->Damage,Class->Warhead,0,0);
                }
            }
            if (!water || bridge) {
                CoordStruct coord=unknown_108.Unknown_00436F80();
                if (Class->Spawns && Class->SpawnCount>0) {
                    int first=Random_Pick(0,Class->SpawnCount);
                    int count=first+Random_Pick(0,Class->SpawnCount);
                    for (int i=0;i<count;i++)
                        new AnimClass(Class->Spawns,coord,0,1,0x600,0);
                }
                Rect update(0,0,0,0);
                if (Class->IsTiberium && !bridge) {
                    for (int x=-Class->TiberiumSpreadRadius;x<=Class->TiberiumSpreadRadius;x++) {
                        for (int y=-Class->TiberiumSpreadRadius;y<=Class->TiberiumSpreadRadius;y++) {
                            double sq=(double)x*x+(double)y*y;
                            int distance=(int)Fast_Sqrt(sq);
                            if (distance<=Class->TiberiumSpreadRadius) {
                                // Original uses x's facing table slot for every y iteration.
                                CellClass *cell=&Map[Cell(unknown_adjacent_851a70[x&7]+Cell(coord))];
                                bool germinate=cell->Can_Tiberium_Germinate(0);
                                if (germinate && Class->TiberiumSpawnType) {
                                    new OverlayClass(OverlayTypes[Class->TiberiumSpawnType->ArrayIndex+Random_Pick(0,3)],cell->Fetch_CellID(),-1);
                                    unsigned char data=(unsigned char)Random_Pick(0,2);
                                    cell->OverlayData=data;
                                    Rect overlay=cell->unknown_474eb0();
                                    overlay.Y-=unknown_rect_839630.Y;
                                    update=Union(update,overlay);
                                }
                            }
                        }
                    }
                    unknown_tactical_83996c->Register_Dirty_Area(update,false);
                }
            }
            unknown_slot60();
            return;
        }
    }
    if (unknown_7d && !unknown_17b) {
        if (Class->TrailerAnim && (Class->TrailerSeperation==1 || Frame%Class->TrailerSeperation==0))
            new AnimClass(Class->TrailerAnim,Center_Coord(),1,1,0x600,0);
    }
    if (Class==Rules->DropZoneAnim && Map[Center_Coord()].unknown_4717b0()) unknown_17b=true;
    if (unknown_17b) { unknown_slot60(); return; }
    if (unknown_17c) { unknown_17c=false; return; }
    if (LoopDelay) {
        --LoopDelay;
        if (!LoopDelay) unknown_func_4226d0();
    } else if (unknown_7d) {
        if (Class->IsVeins) unknown_func_422f50();
        if (Class->IsAnimatedTiberium) {
            int overlay=Map[Center_Coord()-CoordStruct(384,384,0)].OverlayIndex;
            if (overlay==-1 || OverlayTypes[overlay]->CellAnim!=Class) unknown_17b=true;
        }
        if (Class->End==-1) {
            Class->End=((ShapeFileStruct const *)Class->unknown_slot39())->Get_Count();
            if (Class->Shadow) Class->End/=2;
        }
        if (Class->LoopEnd==-1) Class->LoopEnd=Class->End;
        Mark(2);
        if (!unknown_17e && !unknown_102 && StageClass::Graphic_Logic()) {
            int stage=Stage;
            if (Class->Damage>0.0 && !unknown_174) {
                if (unknown_b4 && unknown_b4->Kind_Of()==36) unknown_168+=Class->Damage*5.0;
                else unknown_168+=Class->Damage;
                if (unknown_168>=1.0 && !unknown_178) {
                    int damage=(int)unknown_168;
                    unknown_168-=damage;
                    if (strcmp(Class->ID,"INVISO")==0)
                        Unknown_0047E2C0(Center_Coord(),damage,0,Rules->unknown_c74,1);
                    else Unknown_0047E2C0(Center_Coord(),damage,0,Rules->FlameDamage2,1);
                    if (!unknown_7d) return;
                }
            }
            if (Class->unknown_268 && Class->Start+stage==Class->unknown_268 && !unknown_174)
                unknown_func_4228f0();
            if (Class->PingPong) {
                if ((Loops<=1 && (stage>=Class->End || stage==0)) ||
                    (Loops>1 && (stage>=Class->LoopEnd-Class->Start || stage==Class->Start))) {
                    Step=-Step;
                    return;
                }
            }
            if ((Loops<=1 && stage>=Class->End) || (Loops>1 && stage>=Class->LoopEnd-Class->Start)
                || (Class->Shadow && stage>=Class->LoopEnd-Class->Start) || (Class->Reverse && stage<=Class->Start)) {
                if (Loops && Loops!=255) --Loops;
                if (Loops) {
                    if (Class->Reverse) Stage=Class->LoopEnd;
                    else Stage=Class->LoopStart-Class->Start;
                    if (Class->RandomLoopDelayMin || Class->RandomLoopDelayMax)
                        LoopDelay=Random_Pick(Class->RandomLoopDelayMin,Class->RandomLoopDelayMax);
                } else {
                    if (Class->Next) {
                        Class=Class->Next;
                        if (Class->End==-1) {
                            Class->End=((ShapeFileStruct const *)Class->unknown_slot39())->Get_Count();
                            if (Class->Shadow) Class->End/=2;
                        }
                        if (Class->LoopEnd==-1) Class->LoopEnd=Class->End;
                        unknown_17b=false;
                        Loops=(unsigned char)Class->LoopCount;
                        unknown_168=0.0;
                        int delay=Class->Rate;
                        if (Class->RandomRateMin || Class->RandomRateMax)
                            delay=Random_Pick(Class->RandomRateMin,Class->RandomRateMax);
                        if (Class->Normalized) StageClass::Set_Rate(Options.Normalize_Delay(delay));
                        else StageClass::Set_Rate(delay);
                        Stage=Class->Start;
                        unknown_func_4226d0();
                    } else {
                        unknown_159=true;
                        unknown_slot60();
                    }
                }
            }
        }
    }
}
