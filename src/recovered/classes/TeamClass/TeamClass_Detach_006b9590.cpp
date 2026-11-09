// void TeamClass::Detach(AbstractClass *target, bool all) (0x006B9590, slot 10): forgets an
// object that leaves the game: the tag, the first member (only when `all`; the member's
// next team member at FootClass +0x4C8 takes its place), the owner, the script, the
// type and the six swizzled references.
#include "shared/team.hpp"
#include "shared/foot.hpp"

void TeamClass::Detach(AbstractClass *target, bool all)
{
    if (target == Tag) {
        Tag = 0;
    }
    if (target == Member && all) {
        Member = (FootClass *)Member->unknown_4c8;
    }
    if (target == Owner) {
        Owner = 0;
    }
    if (target == Script) {
        Script = 0;
    }
    if (unknown_40 == target) {
        unknown_40 = 0;
    }
    if (unknown_3c == target) {
        unknown_3c = 0;
    }
    if (unknown_34 == target) {
        unknown_34 = 0;
    }
    if (Class == target) {
        Class = 0;
    }
    if (unknown_38 == target) {
        unknown_38 = 0;
    }
    if (unknown_44 == target) {
        unknown_44 = 0;
    }
    if (unknown_30 == target) {
        unknown_30 = 0;
    }
}
