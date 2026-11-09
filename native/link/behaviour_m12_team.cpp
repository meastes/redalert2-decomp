// Behaviour row (Scout1111, native M12; NOT EXACT, native/behaviour_rewrites.json):
// TeamClass::Detach (slot 10, 0x006B9590; seed src/recovered/classes/TeamClass/006b9590.cpp):
// forgets an object that leaves the game: the tag, the first member (when `all`;
// the member's next team member, FootClass +0x4C8, takes its place), the owner, the
// script and the six swizzled references, in the seed's order. The computer
// player's teams reach it on '#5' once the harness gives the game an opponent.
#include "shared/team.hpp"
#include "shared/foot.hpp"

void TeamClass::Detach(AbstractClass *target, bool all)
{
    void *object = target;
    if (object == (void *)Tag) Tag = 0;
    if (object == (void *)Member && all) Member = (FootClass *)Member->unknown_4c8;
    if (object == (void *)Owner) Owner = 0;
    if (object == (void *)Script) Script = 0;
    if (unknown_40 == target) unknown_40 = 0;
    if (unknown_3c == target) unknown_3c = 0;
    if (unknown_34 == target) unknown_34 = 0;
    if (object == (void *)Class) Class = 0;
    if (unknown_38 == target) unknown_38 = 0;
    if (unknown_44 == target) unknown_44 = 0;
    if (unknown_30 == target) unknown_30 = 0;
}
