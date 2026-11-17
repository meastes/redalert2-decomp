// MultiplayerSiegeAttackerTeam::MultiplayerSiegeAttackerTeam() -- 0x005ABE60
// The siege attackers: team number 1 as well, named by the localized
// "MP:AttackerTeam" string.

MultiplayerSiegeAttackerTeam::MultiplayerSiegeAttackerTeam() :
    MultiplayerTeam(StringTable::Load_String("MP:AttackerTeam", 0, "D:\\Ra2\\MPSiegeTeam.cpp", 0x16), 1)
{
}
