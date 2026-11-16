// SPDX-License-Identifier: GPL-3.0-or-later
// One game in one executable (lane Y8; native/yr/README.md "iOS in one executable"): a game's
// engine object (ld -r of its objects) starts with this file compiled with SIDE=begin and ends with
// it compiled with SIDE=end, so its share of each section is bounded by known symbols: the data it
// resets for a fresh start (__data, __bss, __common) and the code whose exit handlers end with it
// (__text). ENGINE names the game (ra2, yr). Not game code.
#define PASTE(e, w, s) e##_##w##_##s
#define NAME(e, w, s) PASTE(e, w, s)
#define MARK(what) NAME(ENGINE, what, SIDE)

__attribute__((used, aligned(16))) int MARK(data) = 1;                                  // __DATA,__data
__attribute__((used, aligned(16), section("__DATA,__bss"))) int MARK(bss);              // __DATA,__bss
__attribute__((used, aligned(16), section("__DATA,__common"))) int MARK(common);        // __DATA,__common (ld -r's zero fill)
__attribute__((used, noinline)) void MARK(text)(void) { __asm__ volatile(""); }         // __TEXT,__text
