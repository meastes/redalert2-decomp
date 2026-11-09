// char const *AddTeamCommandClass::Get_Name() const -- 0x0051B370
// The command's key name, "TeamAddSelect_<n>" with the number, formatted into the shared name
// buffer (0x00A4149C).
#include "shared/command.hpp"
// @ref _sprintf=007820f4
// @ref ?unknown_command_name_a4149c@@3PADA=00a4149c
// @ref ??_C@_0BB@FNPD@TeamAddSelect_?$CFd?$AA@=007dca84

extern "C" int __cdecl sprintf(char *buffer, char const *format, ...);

char const *AddTeamCommandClass::Get_Name() const
{
    sprintf(unknown_command_name_a4149c, "TeamAddSelect_%d", TeamNumber);
    return unknown_command_name_a4149c;
}
