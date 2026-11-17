// void unknown_reset_globals_ab62e0() -- 0x00627330
// Purpose: clears two words (0x00AB62EC, 0x00AB62F0) and three flags
// (0x00AB62F4, 0x00AB62F5, 0x00AB62E0).
// Assumptions: the globals are unidentified.

extern int unknown_ab62ec;
extern int unknown_ab62f0;
extern bool unknown_ab62f4;
extern bool unknown_ab62f5;
extern bool unknown_ab62e0;

void unknown_reset_globals_ab62e0()
{
    unknown_ab62ec = 0;
    unknown_ab62f0 = 0;
    unknown_ab62f4 = false;
    unknown_ab62f5 = false;
    unknown_ab62e0 = false;
}
