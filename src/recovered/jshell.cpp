// jshell.cpp: the original translation unit re-filed as one source (Route A): the accepted
// bodies of its canonical compile group in address order, under the unit's own header
// state (its switches and includes in their original order). The module map
// manifests/original_target/modules/jshell.json carries the addresses and the rows that
// stay in their own files.


#include "shared/shape_file.hpp"
#include "shared/jshell.hpp"
#include "shared/crt_stderr.hpp"
#include "shared/exit.hpp"
#include "shared/mono_class.hpp"


// Header-static dynamic initializer of the JShell unit at 0x0052EDE0: the shape_file.hpp empty rect
// (the unit's only .CRT$XCU entry); the other drafts of this plan follow it in the same file.


// Fatal (0x0052EE00): posts the main window's destroy, prints the message to stderr and the
// mono screen, then exits with code 1 (Westwood's JSHELL.CPP Fatal).

MonoClass *__fastcall Unknown_0059DA50(int page);       // 0x0059DA50

void __cdecl Fatal(char const *message, ...)
{
    Unknown_00691320();
    vfprintf(stderr, message, (char *)(&message + 1));
    Unknown_0059DA50(4)->Printf(message);
    Emergency_Exit(1);
}


// Set_Bit (0x0052EE40): sets or clears one bit of a packed bit array.

void __fastcall Set_Bit(void *array, int bit, int value)
{
    unsigned char mask = (unsigned char)(1 << (bit % 8));

    if (value != 0) {
        *((unsigned char *)array + (bit / 8)) |= mask;
    } else {
        *((unsigned char *)array + (bit / 8)) &= (unsigned char)~mask;
    }
}


// Get_Bit (0x0052EE90): one bit of a packed bit array as 0 or 1.

int __fastcall Get_Bit(void const *array, int bit)
{
    unsigned char mask = (unsigned char)(1 << (bit % 8));
    return (*((unsigned char *)array + (bit / 8)) & mask) != 0;
}


// First_True_Bit (0x0052EED0): index of the first set bit (the array must hold one).

int __fastcall First_True_Bit(void const *array)
{
    int index = 0;
    while (*((unsigned char *)array) == 0) {
        index++;
        array = ((char *)array) + 1;
    }

    int subindex;
    for (subindex = 0; subindex < 8; subindex++) {
        if (Get_Bit(array, subindex)) {
            break;
        }
    }
    return index * 8 + subindex;
}


// First_False_Bit (0x0052EF20): index of the first clear bit (the array must hold one).

int __fastcall First_False_Bit(void const *array)
{
    int index = 0;
    while (*((unsigned char *)array) == 0xFF) {
        index++;
        array = ((char *)array) + 1;
    }

    int subindex;
    for (subindex = 0; subindex < 8; subindex++) {
        if (!Get_Bit(array, subindex)) {
            break;
        }
    }
    return index * 8 + subindex;
}
