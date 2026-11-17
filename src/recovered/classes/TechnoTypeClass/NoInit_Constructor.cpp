// TechnoTypeClass::TechnoTypeClass(NoInitClass const &noinit) (0x006DA6E0)
// Load path: only members with constructors of their own are set up (the
// particle-system lists, cameo names and recoil timing are rebuilt with
// their default constructors); the type rejoins TechnoTypes once.
#include "shared/techno_type.hpp"
#include "shared/house.hpp"

TechnoTypeClass::TechnoTypeClass(NoInitClass const &noinit) :
    ObjectTypeClass(noinit),
    VeteranAbilities(noinit),
    EliteAbilities(noinit),
    DebrisTypes(noinit),
    DebrisMaximums(noinit),
    unknown_390(0),
    Dock(noinit),
    VoiceSelect(noinit),
    VoiceMove(noinit),
    VoiceAttack(noinit),
    VoiceSpecialAttack(noinit),
    VoiceDie(noinit),
    VoiceFeedback(noinit),
    MoveSound(noinit),
    DieSound(noinit),
    DebrisAnims(noinit),
    Prerequisite(noinit),
    PrerequisiteOverride(noinit),
    Explosion(noinit),
    DestroyAnim(noinit)
{
    for (int index = 0; index < TechnoTypes.Count(); index++) {
        if (TechnoTypes[index] == this) {
            return;
        }
    }
    TechnoTypes.Add(this);
}
