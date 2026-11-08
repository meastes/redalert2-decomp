// Reads the firewall settings and delta from [MultiPlayer] of the INI object 0x00839740 (0x005BA280;
// Read_MultiPlayer_Settings calls it once). Draft: Scout1076.
#include "shared/ccini_class.hpp"
#include "shared/firewall_settings.hpp"
// @ref ?Get_Int@INIClass@@QBEHPBD0H@Z=0050d7f0
// @ref ?unknown_ini_839740@@3VCCINIClass@@A=00839740
// @ref ??_C@_0M@MLEF@MultiPlayer?$AA@=007dbce4
// @ref ??_C@_0BB@DHMB@FirewallSettings?$AA@=007e63ec
// @ref ??_C@_0O@OMOI@FirewallDelta?$AA@=007e63dc

void FirewallSettings::Load_005ba280()
{
    unknown_04 = unknown_ini_839740.Get_Int("MultiPlayer", "FirewallSettings", 0);
    unknown_0c = unknown_ini_839740.Get_Int("MultiPlayer", "FirewallDelta", 1);
}
