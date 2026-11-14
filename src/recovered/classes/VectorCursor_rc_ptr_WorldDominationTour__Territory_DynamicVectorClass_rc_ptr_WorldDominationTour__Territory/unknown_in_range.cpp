// 0x00730390: function not yet identified
//
// Owner: VectorCursor<rc_ptr<WorldDominationTour::Territory>, DynamicVectorClass<rc_ptr<WorldDominationTour::Territory>>>, virtual slot 1 (vtable +0x4) (from RTTI/vtables).
// Body: 14 bytes (frozen inventory bound).
// Referenced from 4 sites (computed call, data).
// Ghidra prototype: bool __fastcall unknown_in_range_00730390(int param_1)
//
// Types and names are Ghidra placeholders;
// rename and retype as the purpose becomes known.
//

namespace seed_00730390 {

bool __fastcall unknown_in_range_00730390(int self)
{
    return *(unsigned int *)(self + 8) < *(unsigned int *)(*(int *)(self + 4) + 0x10);
}

}  // namespace seed_00730390
