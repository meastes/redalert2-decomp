// bool MultiplayerBattleTeam::SetPlayerTeam(int unknown) -- 0x005A25D0
// Team slot 2: the MultiplayerTeam slot-2 test (0x005B9B80, called directly).

bool MultiplayerBattleTeam::SetPlayerTeam(int unknown)
{
    return MultiplayerTeam::SetPlayerTeam(unknown) ? true : false;
}
