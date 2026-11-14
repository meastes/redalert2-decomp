// Original0068E270: the shutdown exception shim usesECX/fastcall.
// WinMain preserves original explicit cast to the Windows stdcall callback type.
#include "shared/startup_exceptions.hpp"
int __fastcall Top_Level_Exception_Filter_0068e270(_EXCEPTION_POINTERS *information)
{
    return Handle_Exception_004b87f0(information->ExceptionRecord->ExceptionCode, information);
}
