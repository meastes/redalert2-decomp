// bomb.cpp: the original translation unit re-filed as one source (Route A): the accepted
// bodies of its canonical compile group in address order, under the unit's own header
// state (its switches and includes in their original order). The module map
// manifests/original_target/modules/bomb.json carries the addresses and the rows that
// stay in their own files.


// The Bomb unit calls MapClass 0x00556520/0x00557090 (BombClass::Unknown_00435D30).
#define RA2_MAP_BOMB_BRIDGE
#define BOMB_LIST_DECLARE_LOAD
// Header-static dynamic initializers of the unit at 0x00435970 and the
// unit's BombClass list; the other drafts of this plan are emitted by this file.
#include "shared/unknown_geometry.hpp"
#include "shared/unknown_static_rect.hpp"
#ifndef VECTOR_IMPLEMENT_ALL
#define VECTOR_IMPLEMENT_ALL
#endif
#include "shared/vector_class.hpp"

#include "shared/object.hpp"

// The unit's list of BombClass objects (0x0084EB78; name is a hypothesis).
DynamicVectorClass<BombClass *> unknown_BombClass_list_0084eb78;


// Dynamic initializer (0x004359A0, _$E) emitted by HeaderStatics_diagonal_init_00435970.cpp; no source of its own.


// Dynamic initializer (0x004359C0, _$E) emitted by HeaderStatics_diagonal_init_00435970.cpp; no source of its own.


// Dynamic initializer (0x004359E0, _$E) emitted by HeaderStatics_diagonal_init_00435970.cpp; no source of its own.


// Dynamic initializer (0x00435A00, _$E) emitted by HeaderStatics_diagonal_init_00435970.cpp; no source of its own.


// Dynamic initializer (0x00435A20, _$E) emitted by HeaderStatics_diagonal_init_00435970.cpp; no source of its own.


// Dynamic initializer (0x00435A40, _$E) emitted by HeaderStatics_diagonal_init_00435970.cpp; no source of its own.


// Dynamic initializer (0x00435A80, _$E) emitted by HeaderStatics_diagonal_init_00435970.cpp; no source of its own.


// Dynamic initializer (0x00435AB0, _$E) emitted by HeaderStatics_diagonal_init_00435970.cpp; no source of its own.


// Dynamic initializer (0x00435AE0, _$E) emitted by HeaderStatics_diagonal_init_00435970.cpp; no source of its own.


// Dynamic initializer (0x00435B10, _$E) emitted by HeaderStatics_diagonal_init_00435970.cpp; no source of its own.


// Dynamic initializer (0x00435B20, _$E) emitted by HeaderStatics_diagonal_init_00435970.cpp; no source of its own.


// Dynamic initializer (0x00435B40, _$E) emitted by HeaderStatics_diagonal_init_00435970.cpp; no source of its own.


// Dynamic initializer (0x00435B60, _$E) emitted by HeaderStatics_diagonal_init_00435970.cpp; no source of its own.


// Dynamic initializer (0x00435BA0, _$E) emitted by HeaderStatics_diagonal_init_00435970.cpp; no source of its own.


// BombClass::BombClass() (0x00435BE0): an unplanted bomb with the ticking
// sound from Rules; added to the bomb list.

BombClass::BombClass() :
    unknown_54(0)
{
    unknown_28 = 0;
    Target = 0;
    unknown_58 = false;
    unknown_3c.Init();
    unknown_50 = Rules->BombTickingSound;
    unknown_BombClass_list_0084eb78.Add(this);
}


// BombClass::~BombClass() (0x00435C80): stops the ticking sound and leaves the
// bomb list.

BombClass::~BombClass()
{
    unknown_3c.Unknown_00405C10();
    unknown_BombClass_list_0084eb78.Delete(this);
}


// BombClass::BombClass(NoInitClass const &noinit) (0x00435D00): the vtables only.

BombClass::BombClass(NoInitClass const &noinit) :
    AbstractClass(noinit)
{
}


// void BombClass::Unknown_00435D30() (0x00435D30): sets the bomb off at once. Unless it sits on nothing or is
// spent, the target loses the bomb; a target with +0x76 set only marks the bomb spent; otherwise the Ivan
// explosion hits the target's coordinate (IvanDamage/IvanWarhead from the planting unit) with its damage
// animation, and a bridge repair hut target makes the map destroy the bridge by it (0x00557090 when a cell within
// two holds a bridge tile or a bridge overlay 0x4A-0x65, else 0x00556520). Then it lets go of its owner, target,
// house and sound. Map's 0x00556520/0x00557090 declarations are guarded by RA2_MAP_BOMB_BRIDGE (set by the unit's
// header-statics source).
#include "shared/anim.hpp"
#include "shared/cell.hpp"
#include "shared/map_view.hpp"
#include "shared/rules.hpp"
#include "shared/building_class.hpp"
#include "shared/tileset_index.hpp"

// The land type of the cell under coord.
static inline int Land_At(CoordStruct const &coord)
{
    return Map[Cell(coord)].unknown_ec;
}

void BombClass::Unknown_00435D30()
{
    if (Target == 0 || unknown_58) {
        return;
    }
    bool flag = Target->unknown_76;
    Target->unknown_38 = 0;
    if (flag) {
        Target->unknown_68 = false;
        unknown_58 = true;
    } else {
        Target->unknown_68 = false;
        unknown_58 = true;
        CoordStruct coord = Target->Coord;
        Unknown_0047E2C0(coord, Rules->IvanDamage, Owner, Rules->IvanWarhead, true);
        new AnimClass(Select_Damage_Anim(Rules->IvanDamage, Rules->IvanWarhead, Land_At(coord), &coord),
            coord, 0, 1, 0x2600, Get_Explosion_Z_0047fcc0(coord));
        if (Target != 0 && Target->Kind_Of() == 6 && ((BuildingClass *)Target)->Type->BridgeRepairHut) {
            bool bridge = false;
            for (int y = -2; y < 3; y++) {
                for (int x = -2; x < 3; x++) {
                    int tile = Map[Target->unknown_slot104() + Cell(x, y)].IsoTileIndex;
                    int overlay = Map[Target->unknown_slot104() + Cell(x, y)].OverlayIndex;
                    if ((tile >= unknown_tileset_a6cb68 && tile < unknown_tileset_a6cb68 + 16) || (overlay >= 0x4A && overlay <= 0x65)) {
                        bridge = true;
                    }
                }
            }
            if (bridge) {
                Map.Unknown_00557090(Target->unknown_slot104());
            } else {
                Map.Unknown_00556520(Target->unknown_slot104());
            }
        }
    }
    Owner = 0;
    Target = 0;
    unknown_28 = 0;
    unknown_3c.Unknown_00405FE0();
    unknown_54 = 0;
}


// Unknown435FB0Owner::unknown_435fb0() (0x00435FB0)
// Detaches the target at +0x2C (clearing two of its fields), then resets the owner and its +0x3C sub-object.
struct Unknown435FB0Target {
    char unknown_pad[0x38];
    int unknown_38;
    char unknown_pad2[0x2c];
    char unknown_68;
};
struct Unknown435FB0Sub { void unknown_405fe0(); };
struct Unknown435FB0Owner {
    void unknown_435fb0();
    char unknown_pad0[0x24];
    int unknown_24;
    int unknown_28;
    Unknown435FB0Target *unknown_2c;
    char unknown_pad1[0xc];
    Unknown435FB0Sub unknown_3c;
    char unknown_pad2[0x14];
    int unknown_54;
    char unknown_58;
};
void Unknown435FB0Owner::unknown_435fb0()
{
    if (unknown_2c != 0) {
        unknown_2c->unknown_38 = 0;
        unknown_2c->unknown_68 = 0;
    }
    unknown_58 = 1;
    unknown_24 = 0;
    unknown_2c = 0;
    unknown_28 = 0;
    unknown_3c.unknown_405fe0();
    unknown_54 = 0;
}


class Leaf_435ff0 { public: unsigned int get(); };
unsigned int Leaf_435ff0::get() { return *(unsigned int*)((char*)this + 0x30); }


// int BombClass::Unknown_00436000() const (0x00436000): the frame of the
// bomb's flickering icon: two per sixth of IvanTimedDelay since it was
// planted, plus one in the second half of each IvanIconFlickerRate period,
// at most 11; 12 for a type 1 bomb.

int BombClass::Unknown_00436000() const
{
    if (Type == 1) {
        return 12;
    }
    int rate = Rules->IvanIconFlickerRate;
    int frame = (Frame - unknown_34) / (Rules->IvanTimedDelay / 6) * 2;
    if (Frame % (rate * 2) >= rate) {
        frame++;
    }
    if (frame >= 12) {
        frame = 11;
    }
    return frame;
}


// bool BombClass::Unknown_00436070() const (0x00436070): whether a timed bomb
// (type 0) has reached its frame without being disarmed.

bool BombClass::Unknown_00436070() const
{
    if (Type == 0 && Frame > unknown_38 && !unknown_58) {
        return true;
    }
    return false;
}


// void BombClass::Compute_CRC(WWCRCEngine &crc) const (0x00436090)
#include "shared/wwcrc_engine.hpp"
#include "shared/house.hpp"
#include "shared/techno.hpp"

void BombClass::Compute_CRC(WWCRCEngine &crc) const
{
    AbstractClass::Compute_CRC(crc);
    if (unknown_28 != 0) {
        crc(unknown_28->Fetch_ID());
    }
    if (Owner != 0) {
        crc(Owner->Fetch_ID());
    }
    if (Target != 0) {
        crc(Target->Fetch_ID());
    }
    crc(unknown_58);
}


// HRESULT __stdcall BombClass::Load(IStream *stream) (0x00436140): restores
// the bomb in place, restarts its sound object and swizzles the house, owner
// and target.

HRESULT __stdcall BombClass::Load(IStream *stream)
{
    HRESULT result = AbstractClass::Load(stream);
    if (SUCCEEDED(result)) {
        new (this) BombClass(NoInitClass());
        unknown_3c.Init();
        unknown_54 = 0;
        Swizzler.Swizzle((void **)&unknown_28);
        Swizzler.Swizzle((void **)&Owner);
        Swizzler.Swizzle((void **)&Target);
    }
    return result;
}


void BombListClass::Unknown_004361F0()
{
    for(int index=Bombs.Count()-1;index>=0;--index) {
        BombClass *bomb=Bombs[index];
        if(bomb==0) Bombs.Delete(index);
        else if(bomb->Target==0) {
            delete bomb;
            Bombs.Delete(index);
        }
    }
    for(index=Bombs.Count()-1;index>=0;--index) {
        BombClass *bomb=Bombs[index];
        if(bomb->unknown_50!=-1) {
            if(bomb->Target->unknown_76) {
                bomb->unknown_3c.Unknown_00405D50();
                bomb->unknown_54=0;
            } else if(bomb->unknown_54==0) {
                Voc_Play_At(bomb->unknown_50,bomb->Target->Position_Coord_004f3f40(),&bomb->unknown_3c);
                bomb->unknown_54=1;
            } else {
                Voc_Update_At(bomb->Target->Position_Coord_004f3f40(),&bomb->unknown_3c);
            }
        }
    }
    if(unknown_30>0) {
        --unknown_30;
        return;
    }
    unknown_30=45;
    for(index=Bombs.Count()-1;index>=0;--index) {
        BombClass *bomb=Bombs[index];
        bool visible=false;
        bool previous=bomb->Target->unknown_68;
        if(bomb->unknown_28->Is_Player_Control()) visible=true;
        else {
            int scan=0;
            if(unknown_18.Count()>0) {
                do {
                    TechnoClass *detector=unknown_18[scan];
                    if(detector->Owning_House()->Is_Player_Control()) {
                        CoordStruct delta=detector->Center_Coord()-bomb->Target->Center_Coord();
                        double z=delta.Z;
                        double y=delta.Y;
                        double x=delta.X;
                        int distance=(int)unknown_sqrt_4ba410(x*x+y*y+z*z);
                        if(distance<detector->Techno_Type_Class()->BombSight*256) {
                            visible=true;
                            break;
                        }
                    }
                } while(++scan<unknown_18.Count());
            }
        }
        bomb->Target->unknown_68=visible;
        if(visible!=previous) bomb->Target->unknown_75=true;
    }
}

// External references checked against complete original body operands.


class Target_4361d0 { public: unsigned int Save_004100c0(unsigned int target_object, unsigned int target_stream, unsigned int target_clear_dirty); };
class Leaf_4361d0 { public: unsigned int get(unsigned int object_address, unsigned int stream_address, unsigned int clear_dirty); };
unsigned int Leaf_4361d0::get(unsigned int object_address, unsigned int stream_address, unsigned int clear_dirty) { return ((Target_4361d0*)(stream_address))->Save_004100c0((unsigned int)(object_address), (unsigned int)(stream_address), (unsigned int)(clear_dirty)); }


// 0x00436460: function not yet identified
//
// Body: 1 bytes (frozen inventory bound).
// Referenced from 0 sites.
// Ghidra prototype: void FUN_00436460(void)
//
// Types and names are Ghidra placeholders;
// rename and retype as the purpose becomes known.
//

namespace seed_00436460 {

void  FUN_00436460(void)
{
    return;
}

}  // namespace seed_00436460


// void BombListClass::Add_Bomb(TechnoClass *owner, ObjectClass *target) (0x00436470):
// an infantry (kind 0x0F) plants a timed bomb on a target that has none: the
// bomb goes off IvanTimedDelay frames from now; BombAttachSound plays at the
// target for the owner's house when it is the player's (0x004F52B0).

void BombListClass::Add_Bomb(TechnoClass *owner, ObjectClass *target)
{
    if (owner != 0 && owner->Kind_Of() == 0x0F && target != 0 && target->unknown_38 == 0) {
        BombClass *bomb = new BombClass;
        bomb->unknown_28 = owner->Owning_House();
        bomb->Owner = owner;
        bomb->Target = target;
        bomb->unknown_34 = Frame;
        bomb->unknown_38 = Rules->IvanTimedDelay + bomb->unknown_34;
        bomb->Type = 0;
        target->unknown_38 = bomb;
        Bombs.Add(bomb);
        unknown_30 = 1;
        if (Rules->BombAttachSound != -1 && owner->House->Is_Player_Control()) {
            CoordStruct coord = target->Coord;
            Voc_Play_At(Rules->BombAttachSound, coord, 0);
        }
    }
}


// void BombListClass::Unknown_00436630(BombClass *bomb) (0x00436630): Bombs.Add.

void BombListClass::Unknown_00436630(BombClass *bomb)
{
    Bombs.Add(bomb);
}


// void BombListClass::Unknown_00436680(TechnoClass *techno) (0x00436680):
// joins the BombSight list (TechnoClass slot 53).

void BombListClass::Unknown_00436680(TechnoClass *techno)
{
    unknown_18.Add(techno);
}


// void BombListClass::Unknown_004366D0(TechnoClass *techno) (0x004366D0):
// leaves the BombSight list (TechnoClass slot 52).

void BombListClass::Unknown_004366D0(TechnoClass *techno)
{
    unknown_18.Delete(techno);
}


// void BombListClass::Clear() (0x00436710): empties both lists, keeping
// their allocated sizes, and resets +0x30 to 45.

void BombListClass::Clear()
{
    int length = Bombs.Length();
    unknown_30 = 45;
    Bombs.Clear();
    Bombs.Resize(length);
    DynamicVectorClass<TechnoClass *> &list = unknown_18;
    length = list.Length();
    list.Clear();
    list.Resize(length);
}


// void BombListClass::Detach(AbstractClass *target) (0x00436750): drops
// target from the bombs and from the owner of every bomb.

void BombListClass::Detach(AbstractClass *target)
{
    Bombs.Delete((BombClass *)target);
    for (int index = 0; index < Bombs.Count(); index++) {
        BombClass *bomb = Bombs[index];
        if (bomb != 0 && bomb->Owner == target) {
            bomb->Owner = 0;
        }
    }
}


// HRESULT BombListClass::Save(IStream *stream) (0x004367C0): both lists as
// a count and their pointers.

HRESULT BombListClass::Save(IStream *stream)
{
    int count = Bombs.Count();
    HRESULT result = stream->Write(&count, sizeof(count), 0);
    if (FAILED(result)) {
        return result;
    }
    for (int index = 0; index < count; index++) {
        result = stream->Write(&Bombs[index], sizeof(BombClass *), 0);
        if (FAILED(result)) {
            return result;
        }
    }
    count = unknown_18.Count();
    result = stream->Write(&count, sizeof(count), 0);
    if (FAILED(result)) {
        return result;
    }
    for (index = 0; index < count; index++) {
        result = stream->Write(&unknown_18[index], sizeof(TechnoClass *), 0);
        if (FAILED(result)) {
            return result;
        }
    }
    return result;
}


HRESULT BombListClass::Load(IStream *stream)
{
    BombClass *bomb;
    TechnoClass *techno;
    int count;
    HRESULT result=stream->Read(&count,sizeof(count),0);
    if(FAILED(result)) return result;
    for(int index=0;index<count;++index) {
        result=stream->Read(&bomb,sizeof(bomb),0);
        if(FAILED(result)) return result;
        Bombs.Add(bomb);
    }
    for(index=0;index<count;++index) Swizzle_Pointer(Bombs[index]);
    result=stream->Read(&count,sizeof(count),0);
    if(FAILED(result)) return result;
    for(index=0;index<count;++index) {
        result=stream->Read(&techno,sizeof(techno),0);
        if(FAILED(result)) return result;
        unknown_18.Add(techno);
    }
    for(index=0;index<count;++index) Swizzle_Pointer(unknown_18[index]);
    return result;
}

// External references checked against complete original body operands.


class Leaf_4369d0 { public: unsigned int get(); };
unsigned int Leaf_4369d0::get() { return 0x5c; }


class Leaf_4369e0 { public: unsigned int get(); };
unsigned int Leaf_4369e0::get() { return 0x44; }


// BombClass scalar deleting destructor (0x004369F0): compiler generated (the destructor expanded inline); no source of its own.
