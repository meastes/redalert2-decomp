// AnimTypeClass::AnimTypeClass(char const *ini_name) (0x00424E30)
// Default animation values, then registration in AnimTypes and the list at
// 0x00AC0CB0; a named type loads its image at once (slot 40, theater -1).
// Assumptions: the 0x00AC0CB0 list is declared as an ObjectClass list in
// shared/object.hpp (it also holds AnimTypes; element type unknown).
#include "shared/anim_type.hpp"

AnimTypeClass::AnimTypeClass(char const *ini_name) :
    ObjectTypeClass(ini_name),
    ArrayIndex(-1),
    Biggest(0),
    Damage(0),
    Rate(1),
    Start(0),
    LoopStart(0),
    LoopEnd(0),
    End(0),
    LoopCount(0),
    Next(0),
    DetailLevel(0),
    TranslucencyDetailLevel(0),
    RandomLoopDelayMin(0),
    RandomLoopDelayMax(0),
    RandomRateMin(0),
    RandomRateMax(0),
    Translucency(0),
    Spawns(0),
    SpawnCount(0),
    StartSound(-1),
    StopSound(-1),
    BounceAnim(0),
    ExpireAnim(0),
    TrailerAnim(0),
    TrailerSeperation(0),
    Elasticity(.8f),
    MinZVel(3.5),
    MaxZVel(3.5),
    MaxXYVel(15.0),
    Warhead(0),
    DamageRadius(0),
    TiberiumSpawnType(0),
    TiberiumSpreadRadius(0),
    YSortAdjust(0),
    YDrawOffset(0),
    ZAdjust(0),
    RunningFrames(0),
    IsFlamingGuy(false),
    IsVeins(false),
    IsMeteor(false),
    TiberiumChainReaction(false),
    IsTiberium(false),
    HideIfNoOre(false),
    Bouncer(false),
    Tiled(false),
    ShouldUseCellDrawer(true),
    UseNormalLight(false),
    DemandLoad(false),
    FreeAfterPlaying(false),
    IsAnimatedTiberium(false),
    AltPalette(false),
    Normalized(false),
    Layer(3),
    Flat(false),
    Translucent(false),
    Scorch(false),
    Flamer(false),
    Crater(false),
    ForceBigCraters(false),
    Sticky(false),
    PingPong(false),
    Reverse(false),
    Shadow(false),
    PsiWarning(false),
    ShouldFogRemove(true)
{
    AbstractClass::Create_ID();
    IsSentient = true;
    IsStealthy = true;
    IsSelectable = false;
    IsLegalTarget = false;
    IsInsignificant = true;
    IsImmune = true;
    IsFootprint = false;
    AnimTypes.Add(this);
    AbstractTypePtrTracker.Add(this);
    if (ini_name != 0) {
        AnimTypeClass::Load_Image(-1);
    }
    ArrayIndex = AnimTypes.ID(this);
}
