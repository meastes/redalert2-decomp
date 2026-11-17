// MPCooperative::~MPCooperative() -- 0x005A2B50
// Deletes the campaign records, the selected campaign, the campaigns, the saved players and the
// score screen, then the members.
#define VECTOR_IMPLEMENT_VECTOR_ACCESSORS
#define VECTOR_IMPLEMENT_DYNAMIC_ACCESSORS
#define VECTOR_IMPLEMENT_VECTOR_DESTRUCTOR
#define VECTOR_IMPLEMENT_VECTOR_CLEAR
#include "shared/multiplayer_game_mode.hpp"
#include "shared/coop_campaign.hpp"
#include "shared/unknown_description_list.hpp"
#include "shared/campaign_score.hpp"

MPCooperative::~MPCooperative()
{
    if (unknown_campaigns_40) {
        delete unknown_campaigns_40;
        unknown_campaigns_40 = 0;
    }
    if (unknown_campaign_44) {
        delete unknown_campaign_44;
        unknown_campaign_44 = 0;
    }
    for (int i = unknown_campaigns_328.Count() - 1; i >= 0; i--) {
        delete unknown_campaigns_328[i];
    }
    unknown_campaigns_328.Clear();
    for (i = unknown_players_5c.Count() - 1; i >= 0; i--) {
        delete unknown_players_5c[i];
    }
    int length = unknown_players_5c.Length();
    unknown_players_5c.Clear();
    unknown_players_5c.Resize(length);
    if (unknown_score_54) {
        delete unknown_score_54;
        unknown_score_54 = 0;
    }
    unknown_04 = false;
}
