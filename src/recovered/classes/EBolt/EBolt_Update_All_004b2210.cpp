#include "shared/ebolt.hpp"
// @ref _unknown_geometry_l=00853028
// The no-argument fastcall convention is an exact-code hypothesis.
void __fastcall EBolt::Update_All_004B2210()
{
 for(int i=unknown_EBolt_list_00853050.Count()-1;i>=0;--i) {
  EBolt *bolt=unknown_EBolt_list_00853050[i];
  if(bolt->Owner) {
   CoordStruct coord=bolt->Owner->unknown_slot43(bolt->unknown_24);
   if(coord!=unknown_geometry_l) bolt->Start=coord;
  }
  bolt->Draw_004B2170();
  if(!bolt->unknown_28) {
   unknown_EBolt_list_00853050.Delete(bolt);
   delete bolt;
  }
 }
}
