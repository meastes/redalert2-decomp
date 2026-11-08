// NON-EXACT DRAFT 0x468F90 CCFileClass::CCFileClass(Wstring const &)
// entry 0x468F90; exact-match score 78.57 (/O2 /Ob2 /G5; 11 of 17 instructions: check the bound); plan "draft, verify"
// origin lane: Scout1076 (the accepted CCFileClass(char const *) 0x00468F50 with the name from
// Wstring::Peek_Buffer 0x0076E730, as the row's code calls it)
// Recovered source text by our lanes/workers; never exact-match credit. Tracked by
// native/drafts.json. Not original bytes.
#include "shared/wstring.hpp"

CCFileClass::CCFileClass(Wstring const &filename) :
    Position(0)
{
    CDFileClass::Set_Name(filename.Peek_Buffer());
}
