// 0x00792ad4: function not yet identified
//
// Body: 17 bytes (frozen inventory bound).
// Referenced from 1 site (data).
// Ghidra prototype: void unknown_set_exception_filter_00792ad4(void)
//
// Types and names are Ghidra placeholders;
// rename and retype as the purpose becomes known.
//

namespace seed_00792ad4 {

extern void __cdecl CxxUnhandledExceptionFilter_00792a8e(void);
extern "C" __declspec(dllimport) unsigned int __stdcall SetUnhandledExceptionFilter(int);  // @ref __imp__SetUnhandledExceptionFilter@4=0079a13c
extern "C" unsigned int unknown_previous_filter_00b2b33c;

void  unknown_set_exception_filter_00792ad4(void)
{
    unknown_previous_filter_00b2b33c = SetUnhandledExceptionFilter((int)(CxxUnhandledExceptionFilter_00792a8e));
    return;
}

}  // namespace seed_00792ad4
