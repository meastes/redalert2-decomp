// Behaviour rows (Scout1125, native M15; NOT EXACT, native/behaviour_rewrites.json):
// leaves the load game reaches (Load_Game 0x00656890 -> Get_All: Clear_Scenario
// deletes every object and type first) that have no recovered body.
#include "shared/unknown_pair_second.hpp"
#include "shared/tiberium.hpp"

// 0x0059EBA0 (pass-6a decompile 0059eba0): frees the motion library's matrix
// array (+0x0C) and clears it. Reached when the voxel types are deleted.
UnknownPairSecond::~UnknownPairSecond()
{
    if (unknown_transforms != 0) {
        operator delete(unknown_transforms);
    }
    unknown_transforms = 0;
}

// 0x006EA9D0 (shared/tiberium.hpp: Kind_Of 0x2E, RTTI TiberiumClass).
int TiberiumClass::Kind_Of() const
{
    return 0x2E;
}
