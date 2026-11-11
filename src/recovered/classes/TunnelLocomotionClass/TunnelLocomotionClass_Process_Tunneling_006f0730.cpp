// Tunnel state4: choose a surface exit, or advance toward the underground destination.
#include "shared/tunnel_locomotion.hpp"
#include "shared/fast_math.hpp"
#include "shared/unknown_geometry.hpp"
extern "C" double const tunnel_radian_scale;
// @ref _tunnel_radian_scale=0079b620
static inline UnknownFacingDir Tunnel_Direction_To(CoordStruct const &from, CoordStruct const &to)
{
 Point2D delta(to.X-from.X,to.Y-from.Y);
 double angle=Fast_Atan2((double)-delta.Y,(double)delta.X);
 return UnknownFacingDir((unsigned short)(int)((angle-1.5707963267948966)*-10430.060040584269));
}
static inline CoordStruct Tunnel_Move_Coord(CoordStruct const &start, UnknownFacingDir const &dir, int distance)
{
 Point2D point(start.X,start.Y);
 double radians=((short)dir.Raw-0x3fff)*tunnel_radian_scale;
 return CoordStruct((int)(point.X+Fast_Cos(radians)*(double)distance),
                    (int)(point.Y-Fast_Sin(radians)*(double)distance),start.Z);
}
void TunnelLocomotionClass::Process_Tunneling()
{
 CoordStruct coord=LinkedTo->Coord;
 if((Point2D(DestinationCoord.X,DestinationCoord.Y)-Point2D(coord.X,coord.Y)).Length()<20) {
  coord=DestinationCoord;
  coord.Z=-256;
  if(LinkedTo->Can_Enter_Cell(&Map[CoordStruct(LinkedTo->Coord)],-1,-1,0,true) && Map.Unknown_0055A7F0(CoordStruct(LinkedTo->Coord))) {
   Cell cell=LinkedTo->unknown_slot104();
   cell=Map.Nearby_Location(cell,1,Map.Zone(cell,0,false),0,false,Point2D(1,1),false,false,true,true,Cell(0,0),0);
   if(cell==unknown_geometry_k) cell=Map.Nearby_Location(LinkedTo->unknown_slot104(),1,-1,6,false,Point2D(1,1),false,false,true,true,Cell(0,0),0);
   if(cell==unknown_geometry_k) {
    LinkedTo->Take_Damage(&LinkedTo->Strength,0,Rules->unknown_c74,0,true,true);
    return;
   }
   Move_To(CoordStruct(cell,0));
  } else {
   if(LinkedTo->Get_Height_AGL()>-50) Voc_Play_At(Rules->unknown_16c,CoordStruct(LinkedTo->Coord),0);
   unknown_18=5;
   LinkedTo->Mark(1);
   IsUnderground=false;
  }
 } else {
  coord=Tunnel_Move_Coord(coord,Tunnel_Direction_To(coord,DestinationCoord),19);
 }
 LinkedTo->Set_Coord(coord);
}
// @ref _unknown_geometry_k=00ac0f10
// @ref __real@8@40039800000000000000=007adca8
// @ref ?Fast_Atan2@@YAMNN@Z=004ba600
// @ref ?Fast_Sin@@YANN@Z=004ba480
// @ref ?Fast_Cos@@YANN@Z=004ba4d0
