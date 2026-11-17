// bool MultiplayerBattleTeam::IsTeamIncluded(int unknown) -- 0x005A25B0
// Team slot 1: the MultiplayerTeam slot-1 test (0x005B9B60, called directly).
// @ref ?IsTeamIncluded@MultiplayerTeam@@UAE_NH@Z=005b9b60

bool MultiplayerBattleTeam::IsTeamIncluded(int unknown)
{
    return MultiplayerTeam::IsTeamIncluded(unknown) ? true : false;
}
