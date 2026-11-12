// Westwood's pseudo-random number generators (TibSun random.h; names after mischa85/RedAlert2
// include/random.h; the module is src/recovered/random.cpp).
//
// RandomClass: 32-bit linear congruential generator returning 15 bits.
// Random2Class: 250-entry XOR lagged-Fibonacci table.
// Random3Class: counter-based generator using the "psdes" mixing function with the Mix1/Mix2
//   tables.
// The ranged operator()(min, max) of each class expands the Pick_Random_Number template inline.
//
// Known differences from the template header, each kept because the template form changes other
// units' code (manifests/original_target/modules.json): the template precedes the classes;
// Random2Class spells TABLE_SIZE as 250; units other than the module see RandomClass's constructor
// inline.
#ifndef RA2_SHARED_RANDOM_HPP
#define RA2_SHARED_RANDOM_HPP

// A 250-entry XOR lagged-Fibonacci table seeded from a Random3Class; its ranged operator() expands
// the Pick_Random_Number template.
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

// A random number in [minval, maxval] drawn with rejection sampling from the
// generator's low bits.
template<class T>
int Pick_Random_Number(T &generator, int minval, int maxval)
{
    if (minval == maxval) {
        return minval;
    }

    if (minval > maxval) {
        int temp = minval;
        minval = maxval;
        maxval = temp;
    }

    int magnitude = maxval - minval;
    int highbit = T::SIGNIFICANT_BITS - 1;
    while ((magnitude & (1 << highbit)) == 0 && highbit > 0) {
        highbit--;
    }

    int mask = ~((~0L) << (highbit + 1));
    int pick = magnitude + 1;
    while (pick > magnitude) {
        pick = generator() & mask;
    }
    return pick + minval;
}

class RandomClass {
public:
#ifdef RA2_RANDOM_UNIT
    RandomClass(unsigned seed = 0);
#else
    RandomClass(unsigned seed = 0) : Seed(seed) {}
#endif

    operator int() { return operator()(); }
    int operator()();
    int operator()(int minval, int maxval);

    enum {
        SIGNIFICANT_BITS = 15
    };

protected:
    unsigned int Seed;

    enum {
        MULT_CONSTANT = 0x41C64E6D,
        ADD_CONSTANT = 0x00003039,
        THROW_AWAY_BITS = 10
    };
};

class Random3Class {
public:
    Random3Class(unsigned seed1 = 0, unsigned seed2 = 0);

    operator int() { return operator()(); }
    int operator()();
    int operator()(int minval, int maxval);

    enum {
        TABLE_SIZE = 20,
        SIGNIFICANT_BITS = 32
    };

protected:
    static int Mix1[TABLE_SIZE];
    static int Mix2[TABLE_SIZE];
    int Seed;
    int Index;
};

#endif
