// char const *SelectTeamCommandClass::Get_Name() const -- 0x0051B280
// The command's key name, "TeamSelect_<n>" with the number, formatted into the shared name
// buffer (0x00A4149C).
#include "shared/command.hpp"
// @ref _sprintf=007820f4
// @ref ?unknown_command_name_a4149c@@3PADA=00a4149c
// @ref ??_C@_0O@GGJJ@TeamSelect_?$CFd?$AA@=007dca28

extern "C" int __cdecl sprintf(char *buffer, char const *format, ...);

char const *SelectTeamCommandClass::Get_Name() const
{
    sprintf(unknown_command_name_a4149c, "TeamSelect_%d", TeamNumber);
    return unknown_command_name_a4149c;
}
