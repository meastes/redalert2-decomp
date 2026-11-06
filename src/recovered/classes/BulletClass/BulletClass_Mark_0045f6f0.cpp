// bool BulletClass::Mark(int arg1) (0x0045F6F0)
#include "shared/bullet.hpp"

bool BulletClass::Mark(int arg1)
{
    return ObjectClass::Mark(arg1) ? true : false;
}
