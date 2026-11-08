// CCFileClass::CCFileClass(Wstring const &) (0x00468F90)
// A named file from a string's text (MSPCXAnim, the game modes' INI). Draft: Scout1076.
#include "shared/wstring.hpp"

CCFileClass::CCFileClass(Wstring const &filename) :
    Position(0)
{
    CDFileClass::Set_Name(filename.Peek_Buffer());
}
