#define RA2_MAPGEN_SHAPE_MEMBER
#define RA2_COORD_AS_CELL
#define RA2_VECTOR_PUBLIC_COUNT
#define RA2_MAP_REGION_SPLIT
#define RA2_POINT_ADD_IN_PLACE
#define RA2_MAP_REGION_LIST
#define VECTOR_IMPLEMENT_DYNAMIC_DELETE_OBJECT
#define VECTOR_IMPLEMENT_VECTOR_DESTRUCTOR
#define VECTOR_IMPLEMENT_DYNAMIC_DELETE_INDEX
#define VECTOR_IMPLEMENT_DYNAMIC_SET_GROWTH_STEP
#define VECTOR_IMPLEMENT_VECTOR_ACCESSORS
#define VECTOR_IMPLEMENT_DYNAMIC_ACCESSORS
#define VECTOR_IMPLEMENT_DYNAMIC_CONSTRUCTOR
#define VECTOR_IMPLEMENT_DYNAMIC_ADD
#include "shared/map_region.hpp"
#include "shared/map.hpp"
#include "shared/radar.hpp"
// @ref ??_E?$DynamicVectorClass@VCell@@@@UAEPAXI@Z=0042d3e0
// @ref ??8?$VectorClass@VCell@@@@UBE_NABV0@@Z=0042ce10
// @ref ?Resize@?$DynamicVectorClass@VCell@@@@UAE_NHPBVCell@@@Z=0042d050
// @ref ?Clear@?$DynamicVectorClass@VCell@@@@UAEXXZ=0042cc10
// @ref ?ID@?$DynamicVectorClass@VCell@@@@UAEHABVCell@@@Z=0042d120
// @ref ?ID@?$DynamicVectorClass@VCell@@@@UAEHPBVCell@@@Z=0042cc40
// @ref ?Fetch@?$VectorClass@VCell@@@@UBE?AVCell@@H@Z=0042cc60
// @ref ??_E?$VectorClass@VCell@@@@UAEPAXI@Z=0042d390
// @ref ?Resize@?$VectorClass@VCell@@@@UAE_NHPBVCell@@@Z=0042cee0
// @ref ?Clear@?$VectorClass@VCell@@@@UAEXXZ=0042cfb0
// @ref ?ID@?$VectorClass@VCell@@@@UAEHABVCell@@@Z=0042cfe0
// @ref ?ID@?$VectorClass@VCell@@@@UAEHPBVCell@@@Z=0042d030
// @ref ??_E?$rc_ptr@VWDTTerritory@@@@UAEPAXI@Z=0051e600
// @ref ??_E?$VectorClass@H@@UAEPAXI@Z=0046da70
// @ref ??8?$VectorClass@H@@UBE_NABV0@@Z=0046d070
// @ref ?Resize@?$VectorClass@H@@UAE_NHPBH@Z=0046d0b0
// @ref ?Clear@?$VectorClass@H@@UAEXXZ=0046d160
// @ref ?ID@?$VectorClass@H@@UAEHABH@Z=0046d190
// @ref ?ID@?$VectorClass@H@@UAEHPBH@Z=0046d1d0
// @ref ?Fetch@?$VectorClass@H@@UBEHH@Z=0046ccd0
// @ref ??_E?$DynamicVectorClass@H@@UAEPAXI@Z=0046cf50
// @ref ?Resize@?$DynamicVectorClass@H@@UAE_NHPBH@Z=0046d250
// @ref ?Clear@?$DynamicVectorClass@H@@UAEXXZ=0046cc80
// @ref ?ID@?$DynamicVectorClass@H@@UAEHABH@Z=0046d350
// @ref ?ID@?$DynamicVectorClass@H@@UAEHPBH@Z=0046ccb0
// @ref ??_E?$VectorClass@VRect@@@@UAEPAXI@Z=0058ed40
// @ref ??8?$VectorClass@VRect@@@@UBE_NABV0@@Z=0058e530
// @ref ?Resize@?$VectorClass@VRect@@@@UAE_NHPBVRect@@@Z=0058e5a0
// @ref ?Clear@?$VectorClass@VRect@@@@UAEXXZ=0058e690
// @ref ?ID@?$VectorClass@VRect@@@@UAEHABVRect@@@Z=0058e6c0
// @ref ?ID@?$VectorClass@VRect@@@@UAEHPBVRect@@@Z=0058e720
// @ref ?Fetch@?$VectorClass@VRect@@@@UBE?AVRect@@H@Z=0058dde0
// @ref ??_E?$DynamicVectorClass@VRect@@@@UAEPAXI@Z=0058ed90
// @ref ?Resize@?$DynamicVectorClass@VRect@@@@UAE_NHPBVRect@@@Z=0058e740
// @ref ?Clear@?$DynamicVectorClass@VRect@@@@UAEXXZ=0058dd90
// @ref ?ID@?$DynamicVectorClass@VRect@@@@UAEHABVRect@@@Z=0058e830
// @ref ?ID@?$DynamicVectorClass@VRect@@@@UAEHPBVRect@@@Z=0058ddc0
// @ref ??_E?$VectorClass@PAVCellClass@@@@UAEPAXI@Z=0056ae40
// @ref ??8?$VectorClass@PAVCellClass@@@@UBE_NABV0@@Z=0056ba80
// @ref ?Resize@?$VectorClass@PAVCellClass@@@@UAE_NHPBQAVCellClass@@@Z=0056bac0
// @ref ?Clear@?$VectorClass@PAVCellClass@@@@UAEXXZ=0056bb70
// @ref ?ID@?$VectorClass@PAVCellClass@@@@UAEHABQAVCellClass@@@Z=0056bba0
// @ref ?ID@?$VectorClass@PAVCellClass@@@@UAEHPBQAVCellClass@@@Z=0056bbe0
// @ref ?Fetch@?$VectorClass@PAVCellClass@@@@UBEPAVCellClass@@H@Z=0056acd0
// @ref ??_E?$DynamicVectorClass@PAVCellClass@@@@UAEPAXI@Z=0058ede0
// @ref ?Resize@?$DynamicVectorClass@PAVCellClass@@@@UAE_NHPBQAVCellClass@@@Z=0058e880
// @ref ?Clear@?$DynamicVectorClass@PAVCellClass@@@@UAEXXZ=0058de10
// @ref ?ID@?$DynamicVectorClass@PAVCellClass@@@@UAEHABQAVCellClass@@@Z=0058e930
// @ref ?ID@?$DynamicVectorClass@PAVCellClass@@@@UAEHPBQAVCellClass@@@Z=0058de40
// @ref ??_E?$VectorClass@USubzoneConnectionStruct@@@@UAEPAXI@Z=0040b230
// @ref ??8?$VectorClass@USubzoneConnectionStruct@@@@UBE_NABV0@@Z=0040b070
// @ref ?Resize@?$VectorClass@USubzoneConnectionStruct@@@@UAE_NHPBUSubzoneConnectionStruct@@@Z=0040b0b0
// @ref ?Clear@?$VectorClass@USubzoneConnectionStruct@@@@UAEXXZ=0040b1a0
// @ref ?ID@?$VectorClass@USubzoneConnectionStruct@@@@UAEHABUSubzoneConnectionStruct@@@Z=0040b1d0
// @ref ?ID@?$VectorClass@USubzoneConnectionStruct@@@@UAEHPBUSubzoneConnectionStruct@@@Z=0040b210
// @ref ?Fetch@?$VectorClass@USubzoneConnectionStruct@@@@UBE?AUSubzoneConnectionStruct@@H@Z=0040b050
// Actual generic vector definitions required by the new constructed/append context.
// Header-static dynamic initializers of the unit at 0x0056D2D0:  the
// shared/unknown_geometry.hpp constants and unknown_static_rect.hpp
// (0x0056D2D0-0x0056D4B6); the other
// drafts of this plan are emitted by these includes.
#include "shared/unknown_geometry.hpp"
#include "shared/unknown_static_rect.hpp"
#define SHAPE_FILE_EMPTY_RECT unknown_static_rect
#include "shared/unknown_map_generator.hpp"
#include "shared/map_gaussian.hpp"
#include "shared/map_random.hpp"
#include "shared/house.hpp"
#include "shared/mouse_class.hpp"
#include "shared/isometric_tile_type.hpp"
#include "shared/map_view.hpp"
#include "shared/cell.hpp"
#include "shared/random2.hpp"
#include "shared/coord.hpp"
#include "shared/crt_math.hpp"
#include "shared/tileset_index.hpp"
#include "shared/building.hpp"
#include "shared/id_source.hpp"
#include "shared/fast_math.hpp"
#include "shared/house_type.hpp"
#include "shared/rules.hpp"
#include "shared/map_preview.hpp"
#include "shared/dialog_create.hpp"
#include "shared/raw_file_class.hpp"
#include "shared/unknown_globals.hpp"
#include "shared/map_seed.hpp"
#include "shared/win32_find.hpp"
#include "shared/options_dialog.hpp"
#include "shared/wdt_territory.hpp"
#include "shared/cc_file_class.hpp"
#include "shared/win32_api.hpp"
#include "shared/mouse.hpp"
#include "shared/keyboard.hpp"
#include "shared/progress_screen.hpp"
#include "shared/string_table.hpp"
#include "shared/crt_memory.hpp"
#include "shared/owner_draw_dialog_control.hpp"
#include "shared/anim_type.hpp"
#include "shared/reference_counted.hpp"
#include "shared/world_domination_tour.hpp"
#include "shared/session_state.hpp"
#define RA2_VEIN_HEAP_REMOVE_MIN_IMPLEMENTATION
#include "shared/tiberium.hpp"
#include "shared/debug_printf.hpp"
#include "shared/ccini_class.hpp"
#include "shared/tactical.hpp"
#include "shared/unit.hpp"
#include "shared/infantry.hpp"
#include "shared/terrain.hpp"
#include "shared/terrain_type.hpp"
#include "shared/overlay_type.hpp"
#include "shared/building_type_class.hpp"
#include "shared/smudge_type.hpp"
#include "shared/isometric_tile.hpp"
#include "shared/vector_class.hpp"
#include "shared/random.hpp"
#include "shared/vein_heap_core.hpp"
#include "shared/map_cell_view.hpp"
#include "shared/cell_class.hpp"
#include "shared/rect.hpp"
#include "shared/building_type_list.hpp"
#include "shared/infantry_type.hpp"
#include "shared/adjacent_cell.hpp"
#include "shared/dir_type.hpp"
#include "shared/map_generation_link.hpp"
#include "shared/map_generation_path.hpp"
#include "shared/event.hpp"
#include "shared/facing.hpp"
#include "shared/vein_heap.hpp"
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
// @ref _unknown_geometry_angle45=00a709f8
// @ref _unknown_geometry_angle60=00a709b8
// @ref _unknown_geometry_angle90=00a6fd48
// @ref _unknown_geometry_b=00a70a10
// @ref _unknown_geometry_c=00a70a00
// @ref _unknown_geometry_deg_to_rad=007a5b60
// @ref _unknown_geometry_diagonal=00a70098
// @ref _unknown_geometry_g=00a70a18
// @ref _unknown_geometry_h=00a709c0
// @ref _unknown_geometry_i=00a709f0
// @ref _unknown_geometry_j=00a70a08
// @ref _unknown_geometry_k=00a70090
// @ref _unknown_geometry_l=00a709e0
// @ref _unknown_geometry_sixty=007a5b58
// @ref _unknown_static_rect=00a70080
