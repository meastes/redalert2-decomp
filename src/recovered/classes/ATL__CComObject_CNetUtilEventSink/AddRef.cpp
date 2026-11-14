// 0x00765c90: function not yet identified
//
// Owner: ATL::CComObject<CNetUtilEventSink>, virtual slot 1 (vtable +0x4) (from RTTI/vtables).
// Body: 17 bytes (frozen inventory bound).
// Referenced from 1 site (data).
// Ghidra prototype: void AddRef_00765c90(int param_1)
//
// Types and names are Ghidra placeholders;
// rename and retype as the purpose becomes known.
//
#include "shared/win32_types.hpp"

namespace seed_00765c90 {

extern "C" __declspec(dllimport) unsigned int __stdcall InterlockedIncrement(int);  // @ref __imp__InterlockedIncrement@4=0079a1bc

void __stdcall AddRef_00765c90(int self)
{
    InterlockedIncrement((int)((LONG *)(self + 4)));
    return;
}

}  // namespace seed_00765c90
