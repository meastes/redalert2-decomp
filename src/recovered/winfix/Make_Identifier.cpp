// char const *__fastcall Make_Identifier(char const *name, int number) -- 0x00736D40
// Purpose: formats "<name><number>" (or just the number without a name)
// into the shared buffer at 0x00B25EF0 and returns it.
// Assumptions: what the names are (WOL area) is unknown.

extern "C" __declspec(dllimport) int __cdecl wsprintfA(char *buffer, char const *format, ...);
extern char unknown_buffer_b25ef0[];

char const *__fastcall Make_Identifier(char const *name, int number)
{
    if (name != 0) {
        wsprintfA(unknown_buffer_b25ef0, "%s%d", name, number);
        return unknown_buffer_b25ef0;
    }
    wsprintfA(unknown_buffer_b25ef0, "%d", number);
    return unknown_buffer_b25ef0;
}
