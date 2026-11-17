#include "shared/cc_file_class.hpp"
extern char const expansion_data_name_007d7eac[];
// No direct callers found; no-argument fastcall convention is an exact-code hypothesis.
bool __fastcall Expansion_Data_Available_004b8970()
{
    CCFileClass file(expansion_data_name_007d7eac);
    return file.Is_Available(0);
}
