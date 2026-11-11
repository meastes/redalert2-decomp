// Random2Class, Westwood's lagged-Fibonacci generator (TibSun random.h; names after mischa85/RedAlert2
// include/random.h). Declared apart from shared/random.hpp so a class header can hold one as a member:
// random.hpp's anonymous enums each take a compiler counter value and renumber a unit's _$E static
// initializers (merging it there flips 153 regions; Scout1284, modules.json).
#ifndef RA2_SHARED_RANDOM2_HPP
#define RA2_SHARED_RANDOM2_HPP

// A 250-entry XOR lagged-Fibonacci table seeded from a Random3Class (shared/random.hpp, which also
// holds the Pick_Random_Number template its ranged operator() expands).
class Random2Class {
public:
    Random2Class(unsigned seed = 0);

    operator int() { return operator()(); }
    int operator()();
    int operator()(int minval, int maxval);

    enum {
        SIGNIFICANT_BITS = 32
    };

    bool IsLocked;          // while set, the generator returns 0 without advancing (Clear_Scenario)

protected:
    int Index1;             // table entry updated next
    int Index2;             // table entry mixed in (103 ahead)
    int Table[250];
};

extern Random2Class unknown_random_839218;      // 0x00839218 the generator audio and techno use

#endif
