// void ScenarioClass::Compute_CRC(WWCRCEngine &crc) const (0x00662E00)
// Adds the scenario's name, movies, timers and lighting settings to a CRC.
// @ref ??RWWCRCEngine@@QAEXPBD@Z=00493f90
// @ref ??RWWCRCEngine@@QAEXJ@Z=00493f30
// @ref ??RWWCRCEngine@@QAEXN@Z=00493f70
// @ref ??RWWCRCEngine@@QAEX_N@Z=00493e80
// @ref ??BSystemTimerClass@@QBEJXZ=0069a430
#include "shared/id_source.hpp"
#include "shared/wwcrc_engine.hpp"

void ScenarioClass::Compute_CRC(WWCRCEngine &crc) const
{
    crc(UIName);
    crc((long)IntroMovie);
    crc((long)BriefMovie);
    crc((long)WinMovie);
    crc((long)LoseMovie);
    crc((long)ActionMovie);
    crc((long)PostScoreMovie);
    crc((long)PreMapSelectMovie);
    crc((long)PlayerHouse);
    crc((long)Percent);
    crc((long)Theme);
    crc(CarryOverMoney);
    crc(MultiplayerOnly);
    crc(IsInheritTimer);
    crc((long)(CarryOverCap / 100));
    crc((long)BridgeCount);
    crc(EndOfGame);
    crc((long)Campaign);
    crc(SkipScore);
    crc(OneTimeOnly);
    crc(SkipMapSelect);
    crc(FillSilos);
    crc(IsIgnoreGlobalAITriggers);
    crc(TruckCrate);
    crc((long)Percent);
    crc((long)StartingDropships);
    crc((long)MissionTimer.Value());
    crc((long)ShroudTimer.Value());
    crc((long)FogTimer.Value());
    crc((long)IceGrowthTimer.Value());
    crc((long)VeinGrowthTimer.Value());
    crc((long)ElapsedTime.Time());
    crc((long)Difficulty);
    crc((long)CDifficulty);
    crc((long)Theater);
    crc((long)AmbientGround);
    crc((long)AmbientLevel);
    crc(FreeRadar);
    crc((long)IonAmbient);
    crc((long)IonRed);
    crc((long)IonGreen);
    crc((long)IonBlue);
    crc((long)IonGround);
    crc((long)IonLevel);
    crc((long)AmbientLight);
    crc((long)CurrentAmbientLight);
    crc((long)DesiredAmbientLight);
}
