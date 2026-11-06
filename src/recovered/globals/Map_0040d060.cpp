// MouseClass Map (0x008324E0): the game's screen object, defined in the unit at 0x0040CE70 after
// its header statics (ra2-decomp's _map.cpp `MouseClass Map;`, the YR layout twin). The
// definition emits the dynamic initializer (0x0040D060: the out-of-line MouseClass constructor
// and the atexit registration) and the atexit destructor (0x0040D080: ~MouseClass inline, with
// the sidebar strips' member destructors, up to the out-of-line ~RadarClass); MouseClass's
// vtable brings the deleting destructor (0x0040D160).
#include "shared/mouse_class.hpp"
// @ref ??0MouseClass@@QAE@XZ=0059f020
// @ref ??1?$CDTimerClass@VFrameTimerClass@@@@QAE@XZ=0040d1d0
// @ref ??1RadarClass@@UAE@XZ=0062f080
// @ref ??1UnknownBuildable30@@QAE@XZ=0040d130
// @ref ??8?$VectorClass@U?$HashObject@URadarTrackingStruct@@PAVTechnoClass@@@@@@UBE_NABV0@@Z=00634c40
// @ref ??8?$VectorClass@USubzoneConnectionStruct@@@@UBE_NABV0@@Z=0040b070
// @ref ??_7?$VectorClass@U?$HashObject@URadarTrackingStruct@@PAVTechnoClass@@@@@@6B@=007a8824
// @ref ??_7?$VectorClass@USubzoneConnectionStruct@@@@6B@=0079a76c
// @ref ??_7MouseClass@@6BINoticeSink@@@=0079a94c
// @ref ??_7MouseClass@@6BSidebarClass@@@=0079a954
// @ref ??_E?$DynamicVectorClass@U?$HashObject@URadarTrackingStruct@@PAVTechnoClass@@@@@@UAEPAXI@Z=00634f90
// @ref ??_ESubzoneTrackingStruct@@QAEPAXI@Z=0040afa0
// @ref ??_G?$VectorClass@U?$HashObject@URadarTrackingStruct@@PAVTechnoClass@@@@@@UAEPAXI@Z=00635040
// @ref ??_G?$VectorClass@USubzoneConnectionStruct@@@@UAEPAXI@Z=0040b230
// @ref ??_GMouseClass@@UAEPAXI@Z=0040d160
// @ref ??_H@YGXPAXIHP6EX0@Z@Z=0040aed0
// @ref ??_I@YGXPAXIHP6EX0@Z@Z=0040af00
// @ref ?AddRef@GScreenClass@@UAGKXZ=0040d100
// @ref ?Clear@?$VectorClass@U?$HashObject@URadarTrackingStruct@@PAVTechnoClass@@@@@@UAEXXZ=00634da0
// @ref ?Clear@?$VectorClass@USubzoneConnectionStruct@@@@UAEXXZ=0040b1a0
// @ref ?Fetch@?$VectorClass@U?$HashObject@URadarTrackingStruct@@PAVTechnoClass@@@@@@UBE?AU?$HashObject@URadarTrackingStruct@@PAVTechnoClass@@@@H@Z=00634c10
// @ref ?Fetch@?$VectorClass@USubzoneConnectionStruct@@@@UBE?AUSubzoneConnectionStruct@@H@Z=0040b050
// @ref ?ID@?$VectorClass@U?$HashObject@URadarTrackingStruct@@PAVTechnoClass@@@@@@UAEHABU?$HashObject@URadarTrackingStruct@@PAVTechnoClass@@@@@Z=00634dd0
// @ref ?ID@?$VectorClass@U?$HashObject@URadarTrackingStruct@@PAVTechnoClass@@@@@@UAEHPBU?$HashObject@URadarTrackingStruct@@PAVTechnoClass@@@@@Z=00634e20
// @ref ?ID@?$VectorClass@USubzoneConnectionStruct@@@@UAEHABUSubzoneConnectionStruct@@@Z=0040b1d0
// @ref ?ID@?$VectorClass@USubzoneConnectionStruct@@@@UAEHPBUSubzoneConnectionStruct@@@Z=0040b210
// @ref ?Init_IO@ScrollClass@@UAEXXZ=0040d140
// @ref ?Release@GScreenClass@@UAGKXZ=0040d110
// @ref ?Resize@?$VectorClass@U?$HashObject@URadarTrackingStruct@@PAVTechnoClass@@@@@@UAE_NHPBU?$HashObject@URadarTrackingStruct@@PAVTechnoClass@@@@@Z=00634ca0
// @ref ?Resize@?$VectorClass@USubzoneConnectionStruct@@@@UAE_NHPBUSubzoneConnectionStruct@@@Z=0040b0b0
// @ref ?unknown_slot40@MouseClass@@UAEHXZ=0040d150
// @ref _$E82=0040d080

MouseClass Map;
