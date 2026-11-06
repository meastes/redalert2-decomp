// bool AnimClass::Mark(int arg1) (0x00421500)
// The ObjectClass result as a truth value.
#include "shared/anim.hpp"
// @ref ?Mark@ObjectClass@@UAE_NH@Z=005d4160

bool AnimClass::Mark(int arg1)
{
    return ObjectClass::Mark(arg1) ? true : false;
}
