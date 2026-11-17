#define RA2_VECTOR_UNSIGNED_INDEX  // migration holdout: unit not yet re-rolled to VectorClass::operator[](int) (docs/HANDOFF.md)
// The WDTMap.ini builder 0x0072A880 expands VectorClass<TRect<int>>/<Centroid>::Resize inline, walks
// TibSun's VectorCursor (virtual operator bool and ++, out-of-class constructors) and uses TRect's
// Set and point members, GenericList's constructor 0x00490E80 and CDFileClass(Wstring const &).
#define VECTOR_IMPLEMENT_VECTOR_RESIZE
#define VECTOR_CURSOR_REFERENCE_FORM
#define VECTOR_CURSOR_VIRTUAL_OPERATORS
#define GENERIC_LIST_IMPLEMENT_CONSTRUCTOR
#define INDEX_DECLARE_CONSTRUCTOR
#define CD_FILE_DECLARE_WSTRING_CTOR
#define RA2_TRECT_POINT_MEMBERS
#define RA2_WDT_MAP_LOAD_LAYOUT
#define RA2_RECT_POINT_INLINE
#define CENTROID_DECLARE_CONSTRUCTOR
// Header-static dynamic initializers of the unit at 0x00728FD0: the shape_file.hpp empty rect and the
// shared/unknown_geometry.hpp constants (0x00728FD0-0x007291B1); the other
// drafts of this plan are emitted by these includes.
// The unit's vector members: Map's constructor expands the DynamicVectorClass constructors, its
// destructor the vector destructors, Get_Drawer Add (defined before the unit's first vector_class.hpp).
#define VECTOR_IMPLEMENT_DYNAMIC_CONSTRUCTOR
#define VECTOR_IMPLEMENT_VECTOR_ACCESSORS
#define VECTOR_IMPLEMENT_DYNAMIC_ACCESSORS
#define VECTOR_IMPLEMENT_DYNAMIC_ADD
// The unit defines WorldDominationTour::Centroid's members (0x0072A7C0, 0x0072A7F0).
#define CENTROID_DECLARE_MEMBERS
#include "shared/shape_file.hpp"
#include "shared/unknown_geometry.hpp"
