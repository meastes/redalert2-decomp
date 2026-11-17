// VectorClass<DiskID>::VectorClass(unsigned size, DiskID const *array) (0x00674AA0): the out-of-line copy MultiMission's
// constructors call for DiskIDs (the header body, as the explicit specialization multi_mission.hpp declares). Its own
// unit: defined in the MultiMission unit, VC6 inlines it into 0x00671C50, which the original calls.
// Container members stay undefined here: this text does not yet reproduce the original
// unit's list uses (shared/vector_class.hpp).
#define VECTOR_DECLARATIONS_ONLY
#include "shared/multi_mission.hpp"
template <>
VectorClass<DiskID>::VectorClass(unsigned size, DiskID const *array)
    : Vector(0), VectorMax(size), IsValid(true), IsAllocated(false)
{
    if (size) {
        if (array) {
            Vector = new ((void *)array) DiskID[size];
        } else {
            Vector = new DiskID[size];
            IsAllocated = true;
        }
    }
}
