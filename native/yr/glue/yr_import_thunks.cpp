// The import thunks the YR tree calls by address (lane Y2): gamemd.exe's `jmp [import]` stubs at
// 0x007C897A.. (functions.csv names each one's import), declared by the YR units as FUN_007C8xxx.
// Natively each is the port's function of that import.
#include <stdint.h>
struct HWND__;
struct hostent;
extern "C" {
void *ImmGetContext(HWND__ *);
long ImmGetCompositionStringA(void *, unsigned long, void *, unsigned long);
unsigned long ImmGetCandidateListA(void *, unsigned long, void *, unsigned long);
int ImmNotifyIME(void *, unsigned long, unsigned long, unsigned long);
int ImmSetOpenStatus(void *, int);
int WSAGetLastError(void);
}

int FUN_007C897A(void) { return WSAGetLastError(); }                                    // WSAGetLastError
hostent *FUN_007C8980(char const *name) { return ra2_gethostbyname(name); }  // gethostbyname (yr_crt.h; Winsock 1.1 layout)
void *FUN_007C8A1C(HWND__ *, void *) { return nullptr; }                                // ImmAssociateContext: no IME natively
void *FUN_007C8A22(HWND__ *window) { return ImmGetContext(window); }                    // ImmGetContext
int FUN_007C8A28(void *context, int action, int index, int value) { return ImmNotifyIME(context, action, index, value); }
unsigned int FUN_007C8A2E(void *context, unsigned int index, void *list, unsigned int length) { return ImmGetCandidateListA(context, index, list, length); }
int FUN_007C8A34(void *context, unsigned int index, void *buffer, unsigned int length) { return ImmGetCompositionStringA(context, index, buffer, length); }
int FUN_007C8A3A(void *context, int open) { return ImmSetOpenStatus(context, open); }
