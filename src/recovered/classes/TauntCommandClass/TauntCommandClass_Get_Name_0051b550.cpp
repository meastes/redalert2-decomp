// char const *TauntCommandClass::Get_Name() const -- 0x0051B550
// The command's key name, "Taunt_<n>" with the number, formatted into the shared name
// buffer (0x00A4149C).
#include "shared/command.hpp"
// @ref _sprintf=007820f4
// @ref ?unknown_command_name_a4149c@@3PADA=00a4149c
// @ref ??_C@_08JAP@Taunt_?$CFd?$AA@=007dcb48

extern "C" int __cdecl sprintf(char *buffer, char const *format, ...);

char const *TauntCommandClass::Get_Name() const
{
    sprintf(unknown_command_name_a4149c, "Taunt_%d", TauntNumber);
    return unknown_command_name_a4149c;
}
