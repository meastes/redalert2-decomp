// Veinhole AI710250: table-inclusive bound1480+8 established before source.
#include "shared/veinhole.hpp"
#include "shared/map_cell_view.hpp"
#include "shared/particle_system.hpp"
#include "shared/id_source.hpp"
#include "shared/fast_math.hpp"
extern int vein_frames_7f9514[4][13];
extern int vein_transition_7f95e4[4],vein_loop_7f95f4[4];
extern int vein_step_7f9604[4],vein_stage_7f9614[4];
extern char vein_gas_7f96cc[];
extern double const vein_random_unit_79c280,vein_full_angle_79c9b8,vein_speed_79d558;
extern double const vein_idle_scale_7a5fe8,vein_idle_chance_7ae718,vein_attack_chance_7ae720;
// @ref ?vein_frames_7f9514@@3PAY0N@HA=007f9514
// @ref ?vein_transition_7f95e4@@3PAHA=007f95e4
// @ref ?vein_loop_7f95f4@@3PAHA=007f95f4
// @ref ?vein_step_7f9604@@3PAHA=007f9604
// @ref ?vein_stage_7f9614@@3PAHA=007f9614
// @ref ?vein_gas_7f96cc@@3PADA=007f96cc
// @ref ?vein_random_unit_79c280@@3NB=0079c280
// @ref ?vein_full_angle_79c9b8@@3NB=0079c9b8
// @ref ?vein_speed_79d558@@3NB=0079d558
// @ref ?vein_idle_scale_7a5fe8@@3NB=007a5fe8
// @ref ?vein_idle_chance_7ae718@@3NB=007ae718
// @ref ?vein_attack_chance_7ae720@@3NB=007ae720
// @ref ?Value@?$CDTimerClass@VFrameTimerClass@@@@QBEJXZ=00453eb0
// @ref ??0?$CDTimerClass@VFrameTimerClass@@@@QAE@J@Z=00463f80
inline double Vein_Random_Double(double low,double high)
{
    int sample=unknown_id_source_a3d290->Random()(0,0x7ffffffe);
    return (high-low)*((double)sample*vein_random_unit_79c280)+low;
}
void VeinholeMonsterClass::AI()
{
    // Same evidenced 0x18 layout, using the shared StageClass operation hierarchy.
    ((StageClass *)&Control)->Graphic_Logic();
    switch(CurrentState) {
    case 0:
        if(LogicTimer!=0) {
            if(DesiredState!=3) DesiredState=2;
        } else if(DesiredState!=3) {
            if(Map.Is_Something_Nearby(CellID,2)) DesiredState=1;
            else if(((StageClass *)&Control)->Fetch_Stage()==vein_transition_7f95e4[CurrentState]) {
                if((double)(abs(unknown_id_source_a3d290->Random()())%10000)*vein_idle_scale_7a5fe8<vein_idle_chance_7ae718) {
                    ((StageClass *)&Control)->Set_Stage(vein_loop_7f95f4[CurrentState]-1);
                    ((StageClass *)&Control)->Set_Step(-1);
                }
            }
        }
        break;
    case 1:
        if(LogicTimer!=0) {
            if(DesiredState!=3) DesiredState=2;
            ((StageClass *)&Control)->Set_Step(1);
            if(((StageClass *)&Control)->Fetch_Stage()<0) ((StageClass *)&Control)->Set_Stage(0);
        } else if(!Map.Is_Something_Nearby(CellID,2) && DesiredState==1) {
            DesiredState=0;
            ((StageClass *)&Control)->Set_Step(1);
            if(((StageClass *)&Control)->Fetch_Stage()<0) ((StageClass *)&Control)->Set_Stage(0);
        }
        break;
    case 2:
        if(((StageClass *)&Control)->Fetch_Stage()>=0 && vein_frames_7f9514[2][((StageClass *)&Control)->Fetch_Stage()]==11) {
            if(!IsToPuffGas) {
                CoordStruct base(CellID);
                CoordStruct position=CellID.As_Coord(Map.Get_Height_GL(base)+400);
                ParticleClass *particle=unknown_gas_cloud_system_a40d20->Spawn_Held_Particle(ParticleTypes[ParticleTypeClass::Index_Or_Make(vein_gas_7f96cc)],position);
                particle->unknown_b0.Z=12;
                double angle=Vein_Random_Double(0.0,1.0)*vein_full_angle_79c9b8;
                particle->unknown_b0.X=(int)(Fast_Cos(angle)*vein_speed_79d558);
                particle->unknown_b0.Y=(int)(Fast_Sin(angle)*vein_speed_79d558);
                IsToPuffGas=true;
            }
        } else IsToPuffGas=false;
        if(LogicTimer!=0 || IsToPuffGas) {
            if(((StageClass *)&Control)->Fetch_Stage()==vein_transition_7f95e4[CurrentState]) {
                if(Vein_Random_Double(0.0,1.0)<vein_attack_chance_7ae720) {
                    ((StageClass *)&Control)->Set_Stage(vein_loop_7f95f4[CurrentState]-1);
                    ((StageClass *)&Control)->Set_Step(-1);
                }
            }
        } else {
            if(DesiredState!=3) DesiredState=0;
            ((StageClass *)&Control)->Set_Step(-1);
        }
        break;
    case 3:
        if(((StageClass *)&Control)->Fetch_Stage()==vein_transition_7f95e4[3] && !IsDead) Destroy_Monster();
        break;
    }
    if(GrowthTimer==0) {
        if(!IsDead) {
            Grow();
            GrowthTimer=Rules->VeinholeGrowthRate;
        } else {
            Shrink();
            GrowthTimer=Rules->VeinholeShrinkRate;
        }
    }
    if(!IsDead) {
        if(((StageClass *)&Control)->Fetch_Stage()==vein_transition_7f95e4[CurrentState]) {
            if(DesiredState!=CurrentState) {
                CurrentState=DesiredState;
                ((StageClass *)&Control)->Set_Step(vein_step_7f9604[CurrentState]);
                ((StageClass *)&Control)->Set_Stage(vein_stage_7f9614[CurrentState]);
            } else ((StageClass *)&Control)->Set_Step(0);
        } else if(((StageClass *)&Control)->Fetch_Stage()!=-1) ShapeFrame=vein_frames_7f9514[CurrentState][((StageClass *)&Control)->Fetch_Stage()]-1;
        else ((StageClass *)&Control)->Set_Step(0);
    }
}

// @ref ??0StageTimerClass@@QAE@J@Z=00463f80
