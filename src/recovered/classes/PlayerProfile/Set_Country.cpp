#include "shared/player_profile.hpp"
unsigned int PlayerProfile::Set_Country_007399b0(unsigned int country)
{
    if (country != (unsigned int)Country) {
        Country = country;
        IsDirty = true;
    }
    return country;
}
