// Native stand-ins for YR functions cut from the generated units (lane Y1; native/yr/fixups.json).
// NOT EXACT and not original code.

// Dump_Exception_Info, YR 0x004C85E0 (facing.cpp): wrote the x86 registers and a stack dump to
// except.txt for the crash dialog. Natively the port's fault handler reports a fault (the RA2
// native tree does the same); Exception_Handler still runs and calls this.
struct _EXCEPTION_POINTERS;
void Dump_Exception_Info(_EXCEPTION_POINTERS *) {}
