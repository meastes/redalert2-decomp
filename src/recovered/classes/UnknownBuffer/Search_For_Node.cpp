// Voxel cache index: signed keys, lazy sorting and archive lookup.
#define RA2_UNIT_PLAN_ROUTE   // Unit.CPP unit: UnitClass::Plan_Route 0x00707F60 and MapClass::Unknown_005651E0 declarations
#define RA2_COORD_AS_CELL     // Unit.CPP unit: member Cell comparisons (YR twin Plan_Route shape)
#define RA2_BUILDING_LOCOMOTOR_ADDREF_DECL   // Unit.CPP unit: comip _AddRef declarations (UnitClass::Assign_Destination 0x007053F0)
// Not exact (RA2_NON_MATCHING) prelude: header switches the drafts below need (guard-on compile only)
#ifdef RA2_NON_MATCHING
#define RA2_COORD_LENGTH
#define RA2_COORD_PLUS_POINT2D
#endif
#include "shared/unknown_geometry.hpp"
#include "shared/unknown_static_rect.hpp"
#include "shared/unit.hpp"
// @ref ?Compare_Nodes_006d98d0@UnknownBuffer@@=006d98d0
// @ref ?Search_For_Node_006d9850@UnknownBuffer@@=006d9850
// @ref ?Is_Present_0070a350@UnknownBuffer@@=0070a350
// @ref ?Is_Archive_Same_0070a3f0@UnknownBuffer@@=0070a3f0
UnknownBuffer::NodeElement const *UnknownBuffer::Search_For_Node_006d9850(int const &id) const
    {
        if (unknown_04==0) return 0;
        if (!unknown_0c) {
            qsort(unknown_00,unknown_04,sizeof(NodeElement),Compare_Nodes_006d98d0);
            const_cast<UnknownBuffer *>(this)->unknown_10=0;
            const_cast<UnknownBuffer *>(this)->unknown_0c=true;
        }
        NodeElement target;
        target.ID=id;
        return Binary_Search(static_cast<NodeElement *>(unknown_00),unknown_04,target);
    }
