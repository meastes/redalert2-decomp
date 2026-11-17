
// ---------------------------------------------------------------------------------------------
// Native glue (lane Y2), appended to the ccini unit. CCINIClass::ReadCCFile (0x004741F0) is also
// declared as CCINIClass::Load(FileClass &, bool, bool) (ccini.h), the name its callers use; one
// function at one address, so the native link gives the definition both names.
// ---------------------------------------------------------------------------------------------
__asm__(".globl __ZN10CCINIClass4LoadER9FileClassbb\n"
        "__ZN10CCINIClass4LoadER9FileClassbb = __ZN10CCINIClass10ReadCCFileER9FileClassbb");
