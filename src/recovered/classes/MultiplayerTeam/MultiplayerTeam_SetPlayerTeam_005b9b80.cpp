// bool MultiplayerTeam::SetPlayerTeam(int player) -- 0x005B9B80
// Slot 2: puts a valid player (virtual slot 1) on this team.

bool MultiplayerTeam::SetPlayerTeam(int player)
{
    if (!IsTeamIncluded(player)) {
        return false;
    }
    unknown_players_a3fa7c[player]->Team = Number;
    return true;
}
