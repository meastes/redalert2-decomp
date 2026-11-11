// bool MultiplayerBattleTeam::SetPlayerTeam(int unknown) -- 0x005A25D0
// Team slot 2: the MultiplayerTeam slot-2 test (0x005B9B80, called directly).
// @ref ?SetPlayerTeam@MultiplayerTeam@@UAE_NH@Z=005b9b80

bool MultiplayerBattleTeam::SetPlayerTeam(int unknown)
{
    return MultiplayerTeam::SetPlayerTeam(unknown) ? true : false;
}
