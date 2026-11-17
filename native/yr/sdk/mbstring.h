// <mbstring.h> for the native YR build (lane Y1): the one multibyte-string call the YR tree makes
// (OwnerDraw's edit-control caret, _mbsnccnt). The game's text is single-byte here (code page
// 1252), so a count of bytes is a count of characters (native/yr/glue/yr_crt.cpp).
#ifndef YR_NATIVE_MBSTRING_H
#define YR_NATIVE_MBSTRING_H
#include <stddef.h>
#ifdef __cplusplus
extern "C" {
#endif
size_t _mbsnccnt(const unsigned char *s, size_t n);
#ifdef __cplusplus
}
#endif
#endif
