// The measured frame rate the detail levels compare against (0x00A6EB1C;
// LogicClass's light and particle detail test 0x0053D840 and the draw
// tests read it; name hypothesis). Declaration only, so including it adds
// no compiler-numbered statics.
#ifndef RA2_SHARED_FRAME_RATE_HPP
#define RA2_SHARED_FRAME_RATE_HPP

extern unsigned int FPSCounter_CurrentFrameRate;     // 0x00A6EB1C

#endif
