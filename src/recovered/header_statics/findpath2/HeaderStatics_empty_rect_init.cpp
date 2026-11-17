#define RA2_SDK_HEADERS
// The unit's coordinate is TibSun's Coord on TPoint3D with point.h's member operators (shared/coord.hpp), and
// Coord::As_Cell (Find_First_Obstacle_004bb890, from mischa85 findpath2.cpp).
#define RA2_COORD_TPOINT3D
#define RA2_POINT3D_TIBSUN
#define RA2_COORD_AS_CELL
#define RA2_COORD_AS_CELL_NO_CASTS
// Get_Obstacle_004bbaf0 calls CellClass 0x0047CF10 as a member (shared/cell_class.hpp).
#define RA2_CELL_DECLARE_BRIDGE_ADJUSTED_HEIGHT
// Unit converted to the pinned SDK headers (RA2_SDK_HEADERS, docs/DECISIONS.md). Include order of the original
// findpath2.cpp:6-8 (always.h, rect.h, cell.h; mischa85 src/findpath2.cpp, ra2-decomp src/findpath2.cpp):
#include "shared/always.hpp"
#include "shared/rect.hpp"
#include "shared/cell_class.hpp"
// The screen chain cell_class.hpp brings in (RA2_SDK_HEADERS) makes the unit emit these template members and
// helpers; each is the original's copy compiled in another unit:
// @ref ??_E?$DynamicVectorClass@U?$HashObject@URadarTrackingStruct@@PAVTechnoClass@@@@@@UAEPAXI@Z=00634f90
// @ref ??_G?$VectorClass@U?$HashObject@URadarTrackingStruct@@PAVTechnoClass@@@@@@UAEPAXI@Z=00635040
// @ref ??8?$VectorClass@U?$HashObject@URadarTrackingStruct@@PAVTechnoClass@@@@@@UBE_NABV0@@Z=00634c40
// @ref ?Resize@?$VectorClass@U?$HashObject@URadarTrackingStruct@@PAVTechnoClass@@@@@@UAE_NHPBU?$HashObject@URadarTrackingStruct@@PAVTechnoClass@@@@@Z=00634ca0
// @ref ?Clear@?$VectorClass@U?$HashObject@URadarTrackingStruct@@PAVTechnoClass@@@@@@UAEXXZ=00634da0
// @ref ?ID@?$VectorClass@U?$HashObject@URadarTrackingStruct@@PAVTechnoClass@@@@@@UAEHABU?$HashObject@URadarTrackingStruct@@PAVTechnoClass@@@@@Z=00634dd0
// @ref ?ID@?$VectorClass@U?$HashObject@URadarTrackingStruct@@PAVTechnoClass@@@@@@UAEHPBU?$HashObject@URadarTrackingStruct@@PAVTechnoClass@@@@@Z=00634e20
// @ref ?Fetch@?$VectorClass@U?$HashObject@URadarTrackingStruct@@PAVTechnoClass@@@@@@UBE?AU?$HashObject@URadarTrackingStruct@@PAVTechnoClass@@@@H@Z=00634c10
// @ref ??_ESubzoneTrackingStruct@@QAEPAXI@Z=0040afa0
// @ref ??_G?$VectorClass@USubzoneConnectionStruct@@@@UAEPAXI@Z=0040b230
// @ref ??8?$VectorClass@USubzoneConnectionStruct@@@@UBE_NABV0@@Z=0040b070
// @ref ?Resize@?$VectorClass@USubzoneConnectionStruct@@@@UAE_NHPBUSubzoneConnectionStruct@@@Z=0040b0b0
// @ref ?Clear@?$VectorClass@USubzoneConnectionStruct@@@@UAEXXZ=0040b1a0
// @ref ?ID@?$VectorClass@USubzoneConnectionStruct@@@@UAEHABUSubzoneConnectionStruct@@@Z=0040b1d0
// @ref ?ID@?$VectorClass@USubzoneConnectionStruct@@@@UAEHPBUSubzoneConnectionStruct@@@Z=0040b210
// @ref ?Fetch@?$VectorClass@USubzoneConnectionStruct@@@@UBE?AUSubzoneConnectionStruct@@H@Z=0040b050
// Header-static dynamic initializers of the unit at 0x004BB6A0: the shape_file.hpp empty rect and the
// shared/unknown_geometry.hpp constants (0x004BB6A0-0x004BB881); the other
// drafts of this plan are emitted by these includes.
#include "shared/shape_file.hpp"
#include "shared/unknown_geometry.hpp"
// @ref ?unknown_math_4ba520@@YANN@Z=004ba520
// @ref ?unknown_math_4ba5b0@@YANN@Z=004ba5b0
// @ref ?unknown_sqrt_4ba410@@YANN@Z=004ba410
// @ref __CIpow=007821b0
// @ref __ftol=0077f110
// @ref __real@8@3ff78000000000000000=0079a730
// @ref __real@8@3ffe8000000000000000=0079a728
// @ref __real@8@3fff8000000000000000=0079a708
// @ref __real@8@40008000000000000000=0079a6f8
// @ref __real@8@4004b400000000000000=0079a710
// @ref __real@8@4004f000000000000000=0079a718
// @ref __real@8@4005b400000000000000=0079a720
// @ref __real@8@40078000000000000000=0079a700
// @ref _unknown_empty_rect=00865d40
// @ref _unknown_geometry_angle45=00865d88
// @ref _unknown_geometry_angle60=00865d60
// @ref _unknown_geometry_angle90=00865d38
// @ref _unknown_geometry_b=00865da0
// @ref _unknown_geometry_c=00865d90
// @ref _unknown_geometry_deg_to_rad=007a0fd0
// @ref _unknown_geometry_diagonal=00865d58
// @ref _unknown_geometry_g=00865da8
// @ref _unknown_geometry_h=00865d68
// @ref _unknown_geometry_i=00865d80
// @ref _unknown_geometry_j=00865d98
// @ref _unknown_geometry_k=00865d50
// @ref _unknown_geometry_l=00865d70
// @ref _unknown_geometry_sixty=007a0fc8
