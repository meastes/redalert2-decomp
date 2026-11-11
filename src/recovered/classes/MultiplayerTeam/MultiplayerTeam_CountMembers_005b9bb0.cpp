// int MultiplayerTeam::CountMembers() const -- 0x005B9BB0
// The number of players whose team number is this team's.

int MultiplayerTeam::CountMembers() const
{
    int count = 0;
    for (int index = 0; index < unknown_players_a3fa7c.Count(); index++) {
        if (unknown_players_a3fa7c[index]->Team == Number) {
            count++;
        }
    }
    return count;
}
