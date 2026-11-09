// PRIVATE complete145-body+15-padding animation factory; now compiled under approved declarations.
// Original first stack argument is attachment; second is name (proved from original prologue and loads).
// The first argument is the owner house (SuperClass::Place 0x0069D850 passes House; the +0x160 setter
// 0x00422690 keeps it); the receiver is any AbstractClass (Place passes a cell).
#include "shared/anim.hpp"
#include "shared/anim_type.hpp"
// @ref ?From_Name@AnimTypeClass@@=00425640
// @ref ??0AnimClass@@=00420110
// @ref ?unknown_func_422680@AnimClass@@=00422680
// @ref ?unknown_func_422690@AnimClass@@=00422690
AnimClass *__fastcall Create_Animation_At_Object_00438b80(HouseClass *house,AbstractClass *object,
    AbstractClass *attachment,char const *name)
{
    int index=AnimTypeClass::From_Name(name);
    AnimTypeClass *type=AnimTypes[index];
    CoordStruct position=object->Center_Coord();
    AnimClass *animation=new AnimClass(type,position,0,1,0x600,0);
    animation->unknown_func_422680(attachment);
    animation->unknown_func_422690(house);
    animation->unknown_17d=true;
    return animation;
}
