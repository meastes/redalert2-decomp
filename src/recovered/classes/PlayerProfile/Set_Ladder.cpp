// 007399F0: update PlayerProfile ladder statistics and its changed flag.
#include "shared/player_profile.hpp"
#include "shared/wol_ladder.hpp"
void PlayerProfile::Set_Ladder_007399f0(WOL::Ladder *entry)
{
    if(entry->unknown_1c != Rank) { Rank=entry->unknown_1c; IsDirty=true; }
    if(entry->unknown_10 != Points) { Points=entry->unknown_10; IsDirty=true; }
    if(entry->unknown_08 != Wins) { Wins=entry->unknown_08; IsDirty=true; }
    if(entry->unknown_0c != Losses) { Losses=entry->unknown_0c; IsDirty=true; }
    if(entry->unknown_20 != Disconnects) { Disconnects=entry->unknown_20; IsDirty=true; }
}
