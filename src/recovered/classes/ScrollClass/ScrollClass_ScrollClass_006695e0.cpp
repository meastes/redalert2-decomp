// ScrollClass::ScrollClass() (0x006695E0)
// Clears the scroll state; +0x4F29 starts true.
#include "shared/scroll.hpp"
// @ref ??0TabClass@@QAE@XZ=006a06e0
// @ref ??_7ScrollClass@@6BSidebarClass@@@=007a9444
// @ref ??_7ScrollClass@@6BINoticeSink@@@=007a943c
// @ref ?RevealFogShroud@RadarClass@@UAE_NABVCell@@PAVHouseClass@@_N@Z=0062fca0
// @ref ?unknown_slot38@DisplayClass@@UAE_NABVCell@@PAVHouseClass@@@Z=0049a0f0
// @ref ?unknown_slot39@DisplayClass@@UAE_NABVCell@@PAVHouseClass@@@Z=0049a370
// @ref ?Mouse_Left_Held@DisplayClass@@UAEXABUPoint2D@@@Z=0049c500
// @ref ?INoticeSink_Unknown@TabClass@@UAE_NHH@Z=006a2020
// @ref ?AddRef@GScreenClass@@UAGKXZ=0040d100
// @ref ?Release@GScreenClass@@UAGKXZ=0040d110
// @ref ??_EScrollClass@@UAEPAXI@Z=0066ac60
// @ref ?Init_IO@ScrollClass@@UAEXXZ=0040d140
// @ref ?Get_Input@GScreenClass@@UAEXAAH00@Z=004e09d0
// @ref ?Draw_It@TabClass@@UAEX_N@Z=006a12d0
// @ref __purecall=004b8960
// @ref ?Detach@MapClass@@UAEXPAVAbstractClass@@_N@Z=00559d80
// @ref ?unknown_slot27@MapClass@@UAEXXZ=0054e300
// @ref ?Set_Map_Dimensions@RadarClass@@UAEXXZ=00630300
// @ref ?Set_Local_Dimensions@RadarClass@@UAEXABVRect@@@Z=00630840
// @ref ?Load@SidebarClass@@UAEJPAUIStream@@@Z=00682290
// @ref ?Save@SidebarClass@@UAEJPAUIStream@@@Z=006822a0
// @ref ?unknown_slot32@DisplayClass@@UAEXXZ=0049cfd0
// @ref ?Help_Text@TabClass@@UAEPBGH@Z=006a20b0
// @ref ?Reposition_Sidebar@SidebarClass@@UAEXXZ=006819a0
// @ref ?unknown_slot42@DisplayClass@@UAEXABVRect@@@Z=00498c80
// @ref ?unknown_slot43@DisplayClass@@UAEXXZ=0040d120
// @ref ?Mouse_Left_Press@DisplayClass@@UAEXABUPoint2D@@@Z=0049c490
// @ref ?unknown_slot46@DisplayClass@@UAEXABVCell@@_NPAVObjectClass@@H1@Z=0049b0f0
// @ref ?Mouse_Left_Release@DisplayClass@@UAEXABUCoordStruct@@ABVCell@@PAVObjectClass@@H_N@Z=0049bb20
// @ref ?Init_For_House@TabClass@@UAEXXZ=006a0b70
// @ref ?unknown_slot54@TabClass@@UAE_NH@Z=006a0da0

ScrollClass::ScrollClass()
    : unknown_4f18(0), unknown_4f1c(false), unknown_4f20(0, 0), unknown_4f28(false),
      unknown_4f29(true), unknown_4f2a(false)
{
}
