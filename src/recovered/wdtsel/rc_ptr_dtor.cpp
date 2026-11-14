// 0x00730310: function not yet identified
//
// Body: 11 bytes (frozen inventory bound).
// Referenced from 5 sites (unconditional call).
// Ghidra prototype: void __fastcall rc_ptr_dtor_00730310(undefined4 *param_1)
//
// Types and names are Ghidra placeholders;
// rename and retype as the purpose becomes known.
//

namespace seed_00730310 {

extern void __fastcall rc_ptr_base_dtor_00639510(void);
extern "C" void* rc_ptr_vtable_007af27c;

void __fastcall rc_ptr_dtor_00730310(unsigned int *self)
{
    *self = (unsigned int)(&rc_ptr_vtable_007af27c);
    rc_ptr_base_dtor_00639510();
    return;
}

}  // namespace seed_00730310
