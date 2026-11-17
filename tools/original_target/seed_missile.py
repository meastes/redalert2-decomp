"""Typed scratch seed for the frozen Mission_Missile export."""
import hashlib

from seed_guarded_cfg import X, guarded_graph, require

ENTRY = 0x448400
RAW_SHA256 = "ad9e1de97dc43792b86be286d337aeb7ff47356448cad1ad73f5f25b862b0fd5"
BODY_SHA256 = "b923ac8ad005911e88c9dbeaf7e326a973931f9d6d4d9485e51ee51c42855a38"

PROVIDERS = {
    0x443d60: ('?unknown_443d60@BuildingClass@@QAEXH@Z', 4),
    0x548070: ('??AMapClass@@QAEAAVCellClass@@ABVCell@@@Z', 4),
    0x463990: ('?Create_Bullet@@YIPAVBulletClass@@PAVBulletTypeClass@@PAVAbstractClass@@PAVTechnoClass@@HPAVWarheadTypeClass@@H_N@Z', 20),
    0x420110: ('??0AnimClass@@QAE@PBVAnimTypeClass@@ABUCoordStruct@@HHHH@Z', 24),
    0x4ba410: ('?Fast_Sqrt@@YAMN@Z', 0),
    0x4ba600: ('?Fast_Atan2@@YAMNN@Z', 0),
    0x4ba4d0: ('?Fast_Cos@@YANN@Z', 0),
    0x4ba480: ('?Fast_Sin@@YANN@Z', 0),
    0x4616b0: ('?unknown_slot118@BulletClass@@UAE_NABUCoordStruct@@ABU?$TVelocity3D@N@@@Z', 8),
}

def verify_native(inventory, repository):
    import catalog_io
    from seed_capture_receivers import Receivers
    from verify_manifest import Audit

    audit = Audit(repository, "private/original-target-20261020/inputs/game.exe")
    graph = guarded_graph(audit, inventory, ENTRY)
    require(graph["body_sha256"] == BODY_SHA256, "Frozen missile body differs")
    require([switch["bound"] for switch in graph["switches"]] == [5, 4],
            "Missile dispatch shapes differ")
    catalog = catalog_io.load(catalog_io.locate(repository), references=False)
    receivers = Receivers(repository, catalog, audit, inventory, graph, None)
    for target, (symbol, cleanup) in PROVIDERS.items():
        require(receivers.accepted(target)["symbol"] == symbol and receivers.cleanup(target) == cleanup,
                "Missile provider ABI differs")
    family = receivers.family(("input", 0, "BulletClass"), 118)
    require(family["pop"] == 8 and {row["symbol"] for row in family["targets"]} == {PROVIDERS[0x4616b0][0]},
            "Missile launch slot ABI differs")
    nodes = graph["nodes"]
    for at, target in ((0x448447, 0x443d60), (0x448468, 0x548070), (0x4487bf, 0x548070)):
        ins = nodes[at]
        require(ins.mnemonic == "call" and ins.operands[0].type == X.X86_OP_IMM
                and ins.operands[0].imm == target, "Missile call packet differs")
    require(nodes[0x448ea7].op_str == "eax, [esp + 0x18]" and
            nodes[0x448eab].op_str == "ecx, [esp + 0x54]" and
            nodes[0x448eb3].mnemonic == "call" and
            nodes[0x448eb3].op_str == "dword ptr [edx + 0x1d8]", "Missile launch arguments differ")


def optional_missile_seed(raw, inventory, repository):
    if hashlib.sha256(raw.encode()).hexdigest() != RAW_SHA256:
        return None
    try:
        verify_native(inventory, repository)
    except (ValueError, KeyError, TypeError, OSError):
        return None
    return CANDIDATE


CANDIDATE = """// Scratch hypothesis for BuildingClass::Do_MISSION_MISSILE at 0x00448400.
// Current canonical velocity and destination-cell types. Source must still match in its full unit.
#define RA2_FACING_LEFT90
#define RA2_FACING_DIFFERENCE
// Nuclear and EMP launch missions. Recovered against the base RA2 image.
#include "shared/building.hpp"
#include "shared/house.hpp"
#include "shared/gscreen.hpp"
#include "shared/building_map.hpp"
#include "shared/cell.hpp"
#include "shared/anim.hpp"
#include "shared/anim_type.hpp"
#include "shared/bullet.hpp"
#include "shared/weapon_type.hpp"
#include "shared/super_weapon_type.hpp"
#include "shared/unknown_geometry.hpp"
#include "shared/fast_math.hpp"
#include "shared/sound_play.hpp"
#include "shared/random2.hpp"

extern char const Missile_Target_Anim_Name_007cfa20[];
extern char const EMP_Pulse_Anim_Name_007cfa14[];
// @ref ?Missile_Target_Anim_Name_007cfa20@@=007cfa20
// @ref ?EMP_Pulse_Anim_Name_007cfa14@@=007cfa14
// @ref ?From_Name@AnimTypeClass@@=00425640
// @ref ?Anim_From_Name@@=00420cd0
// @ref ?Create_Bullet@@=00463990
// @ref ?Set_Weapon_Type@BulletClass@@=00463ba0
// @ref ?unknown_sin_4ba480@@=004ba480
// @ref ?unknown_cos_4ba4d0@@=004ba4d0
// @ref ?Fast_Atan2@@=004ba600

// Actual contiguous target-cell fields; same offsets and four-byte representations.
// Reference names and grouping remain qualified until the separate field migration gate.
inline double Missile_Direction_Radians_00448400(UnknownFacingDir const &direction)
{
    return ((short)direction.Raw-16383)*-9.587672516830327e-05;
}
inline UnknownFacingDir Missile_Direction_From_Radians_00448400(double radians)
{
    return UnknownFacingDir((unsigned short)(int)((radians-1.5707963267948966)*-10430.060040584269));
}
inline bool operator<(UnknownFacingDir const &left,UnknownFacingDir const &right)
{
    return abs((short)left.Raw)<abs((short)right.Raw);
}
// Source hypothesis: the reference's real two- and three-dimensional velocity
// classes. Their doubles map to the existing flat BulletVelocity ABI view.
// No shared class, declaration or layout is changed by this private model.
template <class T> class MissileVelocity2D_00448400 {
public:
    MissileVelocity2D_00448400() {}
    MissileVelocity2D_00448400(T x,T y) : X(x),Y(y) {}
    T Speed() const { return (T)Fast_Sqrt(X*X+Y*Y); }
    void Ensure_Nonzero() { if (X==0 && Y==0) X=100; }
    void Set_Yaw(UnknownFacingDir const &direction) {
        Ensure_Nonzero();
        T magnitude=Speed();
        X=magnitude*unknown_cos_4ba4d0(Missile_Direction_Radians_00448400(direction));
        Y=-(magnitude*unknown_sin_4ba480(Missile_Direction_Radians_00448400(direction)));
    }
    T X,Y;
};
template <class T> class MissileVelocity3D_00448400 : public MissileVelocity2D_00448400<T> {
    typedef MissileVelocity2D_00448400<T> PlanarBase;
public:
    MissileVelocity3D_00448400() {}
    MissileVelocity3D_00448400(T x,T y,T z) : PlanarBase(MissileVelocity2D_00448400<T>(x,y)),Z(z) {}
    MissileVelocity3D_00448400(UnknownFacingDir const &yaw,UnknownFacingDir const &pitch,T magnitude) {
        T vertical_heading=unknown_sin_4ba480(Missile_Direction_Radians_00448400(yaw));
        T pitch_projection=unknown_cos_4ba4d0(Missile_Direction_Radians_00448400(pitch));
        T y=vertical_heading*pitch_projection*magnitude;
        T x=unknown_cos_4ba4d0(Missile_Direction_Radians_00448400(yaw))*unknown_cos_4ba4d0(Missile_Direction_Radians_00448400(pitch))*magnitude;
        PlanarBase horizontal(x,y);
        static_cast<PlanarBase &>(*this)=horizontal;
        Z=unknown_sin_4ba480(Missile_Direction_Radians_00448400(pitch))*magnitude;
    }
    void Set(T x,T y,T z) { this->X=x; this->Y=y; Z=z; }
    T Speed() const { return (T)Fast_Sqrt(this->X*this->X+this->Y*this->Y+Z*Z); }
    void Ensure_Nonzero() { if (this->X==0 && this->Y==0 && Z==0) this->X=100; }
    void Set_Speed(T magnitude) {
        Ensure_Nonzero();
        T scale=magnitude/Speed();
        T scaled_x=scale;
        scaled_x*=this->X;
        this->X=scaled_x;
        this->Y*=scale;
        Z*=scale;
    }
    UnknownFacingDir Pitch() const {
        return Missile_Direction_From_Radians_00448400(Fast_Atan2(Z,PlanarBase::Speed()));
    }
    void Set_Pitch(UnknownFacingDir const &direction) {
        double old_angle=Missile_Direction_Radians_00448400(Pitch());
        T magnitude=Speed();
        if (old_angle!=0) {
            this->X/=unknown_cos_4ba4d0(old_angle);
            this->Y/=unknown_cos_4ba4d0(old_angle);
        }
        this->X*=unknown_cos_4ba4d0(Missile_Direction_Radians_00448400(direction));
        this->Y*=unknown_cos_4ba4d0(Missile_Direction_Radians_00448400(direction));
        Z=magnitude*unknown_sin_4ba480(Missile_Direction_Radians_00448400(direction));
    }
    T Z;
};

// A cell-centre constructor over the existing three-int coordinate view.
// This models the evidenced Coord/Point3D constructor chain locally.
struct MissileCoord_00448400 : public CoordStruct {
    MissileCoord_00448400(Cell const &cell,int z=0) : CoordStruct(cell.X*256+128,cell.Y*256+128,z) {}
};

int BuildingClass::Do_MISSION_MISSILE()
{
    if (Type->NukeSilo) {
        switch (unknown_a8) {
        case 0: {
            unknown_5ad=false;
            unknown_443d60(2);
            unknown_a8=1;
            CellClass *target=&Map[House->NukeDest];
            HouseClass *house=House;
            int typeIndex=AnimTypeClass::From_Name(Missile_Target_Anim_Name_007cfa20);
            AnimTypeClass *type=AnimTypes[typeIndex];
            CoordStruct coord=target->Center_Coord();
            AnimClass *anim=new AnimClass(type,coord,0,1,0x600,0);
            anim->unknown_func_422680(0);
            anim->unknown_func_422690(house);
            anim->unknown_17d=true;
            unknown_444=anim;
            // The original deliberately falls through both following states.
        }
        case 1:
            if (unknown_5ad) { unknown_443d60(4); unknown_a8=2; }
        case 2: {
            Center_Coord();
            AbstractClass *target=reinterpret_cast<AbstractClass *>(&Map[House->NukeDest]);
            WeaponTypeClass *weapon=SuperWeaponTypes[unknown_4cc]->WeaponType;
            WarheadTypeClass *warhead=weapon->Warhead;
            int damage=weapon->unknown_a4;
            BulletTypeClass *projectile=weapon->Projectile;
            BulletClass *bullet=Create_Bullet(projectile,target,this,damage,warhead,255,true);
            if (bullet) {
                bullet->Set_Weapon_Type(weapon);
                if (unknown_444) { static_cast<AnimClass *>(unknown_444)->unknown_func_422680(bullet); unknown_444=0; }
                bullet->unknown_slot52();
                CoordStruct launch=unknown_slot43(0);
                MissileVelocity3D_00448400<double> preview(UnknownFacingDir(0), UnknownFacingDir(0), 10.0);
                if (!bullet->unknown_slot118(launch, TVelocity3D<double>(preview.X, preview.Y, preview.Z))) { delete bullet; unknown_a8=3; return 1; }
                AnimClass *takeoff=new AnimClass(Rules->NukeTakeOff,launch,0,1,0x600,0);
                takeoff->ZAdjust=-100;
                unknown_a8=3;
            }
            return 1;
        }
        case 3:
            unknown_443d60(5);
            unknown_a8=4;
            return 6;
        case 4:
            unknown_443d60(1);
            unknown_slot116(5,false);
            return 60;
        }
    }
    if (unknown_4cc!=-1) {
        unknown_slot221(reinterpret_cast<AbstractClass *>(&Map[House->NukeDest]),0);
        unknown_slot116(5,false);
    }
    if (Type->EMPulseCannon) {
        switch (unknown_a8) {
        case 0: {
            UnknownFacingDir aim=unknown_slot285(reinterpret_cast<AbstractClass *>(&Map[House->EMPDest]));
            UnknownFacingDir pitch=unknown_slot170(reinterpret_cast<AbstractClass *>(&Map[House->EMPDest]));
            if (UnknownFacingDir(0)<(unknown_2e0.Current()-aim)) unknown_2e0.Set_Desired(aim);
            else if (UnknownFacingDir(0)<(unknown_2c8.Current()-pitch)) unknown_2c8.Set_Desired(pitch);
            else unknown_a8=1;
            return 1;
        }
        case 1:
            new AnimClass(AnimTypes[Anim_From_Name(EMP_Pulse_Anim_Name_007cfa14)],unknown_slot43(0),0,1,0x600,0);
            unknown_a8=2;
            return 32;
        case 3:
            unknown_2c8.Set_Desired(UnknownFacingDir(0x4000));
            unknown_443d60(1);
            unknown_slot116(5,false);
            return 60;
        case 2: {
            WeaponTypeClass *weapon=unknown_slot231(0)->WeaponType;
            CellClass *target=&Map[House->EMPDest];
            CellClass *target2=&Map[House->EMPDest];
            if (Debug_Map || House->EMPDest==*reinterpret_cast<Cell const *>(&unknown_84ed10)) {
                unknown_443d60(1);
                unknown_slot116(5,false);
                return 60;
            }
            unknown_2c8.Set_Desired(unknown_slot170(reinterpret_cast<AbstractClass *>(target)));
            CoordStruct fire=unknown_slot43(0);
            Map.Get_Height_GL(MissileCoord_00448400(House->EMPDest));
            CoordStruct destination=MissileCoord_00448400(House->EMPDest);
            UnknownFacingDir yaw=Missile_Direction_From_Radians_00448400(Fast_Atan2((double)fire.Y-destination.Y,(double)destination.X-fire.X));
            int speed=Unknown_0047FB70(unknown_5d4c00(reinterpret_cast<AbstractClass *>(target)),Rules->unknown_1320);
            BulletClass *bullet=Create_Bullet(weapon->Projectile,reinterpret_cast<AbstractClass *>(target),this,weapon->unknown_a4,weapon->Warhead,speed*3/4,weapon->unknown_10b);
            TVelocity3D<double> velocity(0.0, 0.0, 0.0);
            velocity.Set_Yaw(yaw);
            velocity.Set_Speed((double)speed);
            CoordStruct turret=unknown_slot181(0);
            CoordStruct displacement=target2->Center_Coord()-turret;
            int distance2=displacement.X*displacement.X+displacement.Y*displacement.Y;
            UnknownFacingDir pitch;
            bool valid=Unknown_0047F8B0(unknown_6d7d20(0),speed,(int)Fast_Sqrt(distance2),displacement.Z,Rules->unknown_1320,&pitch);
            if (!valid) {
                valid=Unknown_0047F8B0(unknown_6d7d20(0),speed*10/8,(int)Fast_Sqrt(distance2),displacement.Z,Rules->unknown_1320,&pitch);
                if (!valid) { pitch=UnknownFacingDir(64000); valid=true; }
            }
            double horizontal_speed = Fast_Sqrt(velocity.Y * velocity.Y + velocity.X * velocity.X);
            UnknownFacingDir old_pitch = Missile_Direction_From_Radians_00448400(
                Fast_Atan2(velocity.Z, horizontal_speed));
            double old_angle = Missile_Direction_Radians_00448400(old_pitch);
            double magnitude = Fast_Sqrt((velocity.Y * velocity.Y + velocity.Z * velocity.Z)
                                         + velocity.X * velocity.X);
            if (old_angle != 0.0) {
                velocity.X /= Fast_Cos(old_angle);
                velocity.Y /= Fast_Cos(old_angle);
            }
            velocity.X *= Fast_Cos(Missile_Direction_Radians_00448400(pitch));
            velocity.Y *= Fast_Cos(Missile_Direction_Radians_00448400(pitch));
            velocity.Z = magnitude * Fast_Sin(Missile_Direction_Radians_00448400(pitch));
            if (valid && bullet->unknown_slot118(turret, velocity)) {
                if (unknown_slot232() && unknown_slot32()->TurretRecoil) {
                    unknown_330.unknown_6d8b90();
                    unknown_350.unknown_6d8b90();
                }
                unsigned int draw=(unsigned int)unknown_random_839218();
                int sound=weapon->unknown_bc[draw%(unsigned int)weapon->unknown_bc.Count()];
                Voc_Play_At(sound,unknown_slot43(0),0);
                unknown_a8=3;
                return 1;
            }
            bullet->Release();
            unknown_443d60(1);
            unknown_slot116(5,false);
            return 60;
        }
        }
    }
    return (int)(Control()->unknown_10*900.0);
}
"""
