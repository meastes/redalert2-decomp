// WeaponTypeClass: a weapon type (WeaponType.cpp, 0x00732CB0-0x007340FA;
// RTTI vtables 0x007AF470 primary (27 slots), 0x007AF444 IRTTITypeInfo at
// +4, 0x007AF468 INoticeSink, 0x007AF460 INoticeSource).
//
// Layout (from the constructor 0x00732EA0, the destructor 0x00733120,
// Compute_CRC 0x00733AB0 and Save 0x00733DF0; names are hypotheses):
//   +0x000  AbstractTypeClass (0x98)
//   +0x098  int unknown_98, +0x09C int Burst (1)
//   +0x0A0  BulletTypeClass *Projectile, +0x0A4 int, +0x0A8 int
//   +0x0AC  WarheadTypeClass *Warhead, +0x0B0 int, +0x0B4 int, +0x0B8 int
//   +0x0BC  TypeList<int> unknown_bc (vtable 0x0079D9F4)
//   +0x0D8  TypeList<AnimTypeClass *> Anim (vtable 0x007A3B3C)
//   +0x0F4  int, +0x0F8 int, +0x0FC..+0x116 bools (+0x10F, +0x113 true)
//   +0x118  int, +0x11C..+0x123 bools, +0x124 unsigned char (10),
//   +0x125..+0x12B bools, +0x12C int
//   size 0x130 (slot 12)
// Every type is added to WeaponTypes (0x00839B90) and removed when destroyed.
#ifndef RA2_SHARED_WEAPON_TYPE_HPP
#define RA2_SHARED_WEAPON_TYPE_HPP

#include "shared/bullet_type.hpp"
#include "shared/warhead_type.hpp"
#include "shared/rgb_class.hpp"

extern "C" GUID const CLSID_WeaponTypeClass;    // 0x007A1DA0

// Speed helpers (0x0047FCD0 returns a default speed for floating
// projectiles; 0x0047FB70 converts a range and speed; hypotheses).
double __cdecl Get_Floater_Gravity();
int __fastcall Calculate_Projectile_Speed(int range, double speed);
struct UnknownFacingDir;
// Whether a ballistic shot reaches: the arc direction for a horizontal
// distance and height difference at a speed (0x0047F8B0; TechnoClass slot
// 170; hypothesis).
bool __fastcall Calculate_Projectile_Pitch(bool arg1, int velocity, int distance, int height, double speed, UnknownFacingDir *dir);

// Armor kinds none, flak, plate, light, medium, heavy, wood, steel,
// concrete, special_1, special_2 (names 0x0079DE28).
enum ArmorType {
    ARMOR_NONE
};
// The armor type names (0x0079DE28: "none", "flak", ... "special_2").
extern char const *ArmorNames[11];
// The armor type named, or 0 (none) when unknown (name is a hypothesis).
ArmorType __fastcall Armor_From_Name(char const *name);                 // 0x00733A20

class WeaponTypeClass : public AbstractTypeClass {
public:
    WeaponTypeClass(char const *ini_name);                              // 0x00732EA0
    WeaponTypeClass(NoInitClass const &noinit);                         // 0x007330D0
    virtual ~WeaponTypeClass();                                         // 0x00733120

    virtual HRESULT __stdcall GetClassID(GUID *retval);                  // 0x00733C50 slot 3
    virtual HRESULT __stdcall Load(IStream *stream);                    // 0x00733C90 slot 5
    virtual HRESULT __stdcall Save(IStream *stream, BOOL cleardirty);  // 0x00733DF0 slot 6
    virtual int Kind_Of() const;                                        // 0x00733FE0 slot 11
    virtual int Size_Of() const;                                        // 0x00733FD0 slot 12
    virtual void Compute_CRC(WWCRCEngine &crc) const;                   // 0x00733AB0 slot 13
    virtual bool Read_INI(CCINIClass &ini);                             // 0x00733220 slot 25

    int Allowed_Threats() const;                                        // 0x00733A60 (projectile flags; callers or the whole of eax: Building/Unit slot 219)
    bool Is_Wall_Destroyer() const;                                     // 0x00733A90
    void Init_Max_Speed();                                              // 0x007339C0 (sets unknown_a8)
    int Get_Speed(int range) const;                                     // 0x00733F70
    static int __fastcall Index_From_Name(char const *name);            // 0x00733F30 (-1 if none)
    static WeaponTypeClass *__fastcall Find_Or_Make(char const *name);  // 0x00733EA0

    int AmbientDamage;                         // +0x098
    int Burst;                              // +0x09C
    BulletTypeClass *Projectile;            // +0x0A0
    int Attack;                         // +0x0A4
    int MaxSpeed;                         // +0x0A8
    WarheadTypeClass *Warhead;              // +0x0AC
    int ROF;                         // +0x0B0
    int Range;                         // +0x0B4
    int MinimumRange;                         // +0x0B8
    TypeList<int> unknown_bc;               // +0x0BC
    TypeList<AnimTypeClass *> Anim;         // +0x0D8
    AnimTypeClass *AssaultAnim;              // +0x0F4 "AssaultAnim"
    ParticleSystemTypeClass *AttachedParticleSystem;    // +0x0F8 "AttachedParticleSystem"
    RGBClass LaserInnerColor;                    // +0x0FC "LaserInnerColor"
    RGBClass LaserOuterColor;                    // +0x0FF "LaserOuterColor"
    RGBClass LaserOuterSpread;                   // +0x102 "LaserOuterSpread"
    bool UseFireParticles;                       // +0x105
    bool UseSparkParticles;                       // +0x106
    bool OmniFire;                       // +0x107
    bool DistributedWeaponFire;                       // +0x108
    bool IsRailgun;                       // +0x109
    bool IsLobber;                       // +0x10A
    bool IsBright;                       // +0x10B
    bool IsSonic;                       // +0x10C
    bool Spawner;                       // +0x10D
    bool LimboLaunch;                       // +0x10E
    bool DecloakToFire;                       // +0x10F
    bool CellRangefinding;                       // +0x110
    bool FireOnce;                       // +0x111
    bool NeverUse;                       // +0x112
    bool RevealOnFire;                       // +0x113
    bool TerrainFire;                       // +0x114
    bool SabotageCursor;                       // +0x115
    bool DisguiseFireOnly;                       // +0x116
    int DisguiseFakeBlinkTime;                        // +0x118
    bool Suicide;                       // +0x11C
    bool IsTurboBoosted;                       // +0x11D
    bool IsSupressed;                       // +0x11E
    bool IsCamera;                       // +0x11F
    bool IsElectric;                       // +0x120
    bool IsLaser;                       // +0x121
    bool IsBigLaser;                       // +0x122
    bool IsHouseColor;                       // +0x123
    char LaserDuration;                       // +0x124 "LaserDuration"
    bool IsIonSensitive;                       // +0x125
    bool AreaFire;                       // +0x126
    bool IsElectricBolt;                       // +0x127
    bool DrawBoltAsLaser;                       // +0x128
    bool IsAlternateColor;                       // +0x129
    bool IsRadBeam;                       // +0x12A
    bool IsRadEruption;                       // +0x12B
    int RadLevel;                        // +0x12C
};

// Every weapon type (0x00839B90; defined in the unit at 0x0040F720).
extern DynamicVectorClass<WeaponTypeClass *> Weapons;

#endif
