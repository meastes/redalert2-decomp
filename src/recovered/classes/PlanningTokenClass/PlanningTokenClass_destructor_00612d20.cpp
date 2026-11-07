#define RA2_COORD_EQUAL_MEMBER
// PlanningTokenClass::~PlanningTokenClass() (0x00612D20)
// Clears the techno's token slot, unlists the token and drops its path.
#include "shared/planning.hpp"
// @ref ??_GPlanningNodeClass@@QAEPAXI@Z=0061a750

PlanningTokenClass::~PlanningTokenClass()
{
    if (Owner != 0) {
        Owner->unknown_6d1420(0);
    }
    Planning_Unlist_Token(this);
    Cancel_Owner();
}
