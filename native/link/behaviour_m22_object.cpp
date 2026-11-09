// Behaviour-written bodies for native M22 (Scout1177): ObjectClass and other
// default virtual slots that the coverage ranking found in live vtables. NOT
// EXACT: each follows its pass-6a decompile (most are the base class's
// do-nothing or constant default).
#include "shared/object.hpp"
#include "shared/object_type.hpp"
#include "shared/techno.hpp"
#include "shared/house.hpp"

void ObjectClass::unknown_slot92(int arg1) { (void)arg1; }                 // 0x0041B310
void ObjectClass::unknown_slot74(int arg1) { (void)arg1; }                 // 0x0041B2F0
ObjectTypeClass *ObjectClass::unknown_slot50(bool arg1) { (void)arg1; return 0; }   // 0x0041B2D0
// 0x00423D40: the name of an object without a type ("No name" at 0x007CECB0).
wchar_t const *ObjectClass::unknown_slot35() { return L"No name"; }
int ObjectClass::unknown_slot102(UnknownCellClass *from, int facing, int *level, bool *ramp, UnknownCellClass *to)   // 0x00423E10
{
    (void)from; (void)facing; (void)level; (void)ramp; (void)to;
    return 0;
}
bool ObjectClass::unknown_slot24() { return false; }                       // 0x00423D10
int ObjectClass::unknown_slot25(bool arg1, intptr_t arg2) { (void)arg1; (void)arg2; return 0; }   // 0x00423D20
void ObjectClass::unknown_slot66(Point2D const &point, Rect const &window) { (void)point; (void)window; }   // 0x00423DD0
void ObjectClass::unknown_slot82(int arg1) { (void)arg1; }                 // 0x00423DE0
void ObjectClass::unknown_slot93(int arg1) { (void)arg1; }                 // 0x00423DF0
bool ObjectClass::unknown_slot111() { return false; }                      // 0x00423E20
bool ObjectClass::unknown_slot112() { return false; }                      // 0x00423E30
bool ObjectClass::unknown_slot113() { return false; }                      // 0x00423E40
bool ObjectClass::unknown_slot114() { return true; }                       // 0x00423E50
int ObjectTypeClass::unknown_slot28() const { return -1; }                 // 0x00426760
int TechnoClass::unknown_slot62(TechnoClass *techno) { (void)techno; return 0; }   // 0x006D4880

// 0x004F3160 (pass-6a decompile): sets +0x143. Reached by BuildingClass slots 52 and 53.
void HouseClass::unknown_func_4f3160()
{
    unknown_143 = true;
}

// TechnoClass defaults in live vtables (pass-6a decompiles).
bool TechnoClass::unknown_slot189() { return false; }                      // 0x00453BA0
void TechnoClass::unknown_slot194() { }                                    // 0x00453BB0
bool TechnoClass::unknown_slot195() { return false; }                      // 0x006CD6E0
bool TechnoClass::unknown_slot202() { return false; }                      // 0x00453BC0
bool TechnoClass::unknown_slot204() { return false; }                      // 0x0041B3D0
bool TechnoClass::unknown_slot224(int arg1) { (void)arg1; return false; }  // 0x00453BD0
void TechnoClass::unknown_slot240() { }                                    // 0x0041B400
void TechnoClass::unknown_slot256() { unknown_170 = false; }               // 0x0041B430 (clears +0x170)
ObjectTypeClass *ObjectClass::Class_Of() const { return 0; }               // 0x004CDDD0

#include "shared/foot.hpp"
void FootClass::unknown_slot280(intptr_t arg1) { (void)arg1; }                  // 0x004CD060
void FootClass::unknown_slot281(intptr_t arg1) { (void)arg1; }                  // 0x004CD070
