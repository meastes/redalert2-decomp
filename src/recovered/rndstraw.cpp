// rndstraw.cpp: the original translation unit re-filed as one source (Route A): the accepted
// bodies of its canonical compile group in address order, under the unit's own header
// state (its switches and includes in their original order). The module map
// manifests/original_target/modules/rndstraw.json carries the addresses and the rows that
// stay in their own files.


#include "shared/random_straw.hpp"
#include "shared/crt_string.hpp"
#include "shared/sha.hpp"


// RandomStraw::RandomStraw() (0x0063D2D0)
// Starts with an empty (all zero) seed.

RandomStraw::RandomStraw() :
    SeedBits(0),
    Current(0)
{
    Reset();
}


// RandomStraw::~RandomStraw() (0x0063D320)
// Clears the seed so no random state is left in memory.

RandomStraw::~RandomStraw()
{
    Reset();
}


// RandomStraw::Reset() (0x0063D350)
// Discards the seed and all generator state.

void RandomStraw::Reset()
{
    SeedBits = 0;
    Current = 0;
    memset(Random, '\0', sizeof(Random));
}


// RandomStraw::Seed_Bits_Needed() const (0x0063D370)
// Seed bits still wanted before the generators are fully seeded.

int RandomStraw::Seed_Bits_Needed() const
{
    const int total = sizeof(Random) * 8;
    if (SeedBits < total) {
        return total - SeedBits;
    }
    return 0;
}


// RandomStraw::Seed_Bit(int seed) (0x0063D390)
// Mixes the low bit of seed into the next seed bit; once every bit of the
// generator state has been seeded, the state is scrambled.

void RandomStraw::Seed_Bit(int seed)
{
    char *ptr = ((char *)&Random[0]) + ((SeedBits / 8) % sizeof(Random));
    if (seed & 0x01) {
        *ptr ^= (char)(1 << (SeedBits & 0x07));
    }
    SeedBits++;
    if (SeedBits == (sizeof(Random) * 8)) {
        Scramble_Seed();
    }
}


// RandomStraw::Seed_Byte(char seed) (0x0063D470)
// Seeds eight bits, low bit first.

void RandomStraw::Seed_Byte(char seed)
{
    for (int index = 0; index < 8; index++) {
        Seed_Bit(seed);
        seed >>= 1;
    }
}


// RandomStraw::Seed_Short(short seed) (0x0063D580)
// Seeds sixteen bits, low bit first.

void RandomStraw::Seed_Short(short seed)
{
    for (int index = 0; index < 16; index++) {
        Seed_Bit(seed);
        seed >>= 1;
    }
}


// RandomStraw::Seed_Long(long seed) (0x0063D680)
// Seeds thirty-two bits, low bit first.

void RandomStraw::Seed_Long(long seed)
{
    for (int index = 0; index < 32; index++) {
        Seed_Bit(seed);
        seed >>= 1;
    }
}


// RandomStraw::Scramble_Seed() (0x0063D790)
// Replaces each byte of the generator state, in turn, with the start of the
// SHA-1 digest of the whole state so far.

void RandomStraw::Scramble_Seed()
{
    SHAEngine sha;
    for (unsigned index = 0; index < sizeof(Random); index++) {
        char digest[20];
        sha.Hash(&Random[0], sizeof(Random));
        sha.Result(digest);

        int tocopy = (sizeof(Random) - index > sizeof(digest)) ? sizeof(digest) : sizeof(Random) - index;
        memmove(((char *)&Random[0]) + index, digest, tocopy);
    }
}


// RandomStraw::Get(void *buffer, int length) (0x0063D830, virtual slot 2)
// Fills the buffer with one byte from each generator in turn; returns the
// bytes delivered.

int RandomStraw::Get(void *buffer, int length)
{
    if (buffer == 0 || length < 1) {
        return Straw::Get(buffer, length);
    }

    int total = 0;
    while (length > 0) {
        *(char *)buffer = (char)Random[Current++]();
        Current = Current % (sizeof(Random) / sizeof(Random[0]));
        buffer = (char *)buffer + sizeof(char);
        length--;
        total++;
    }
    return total;
}


// RandomStraw scalar deleting destructor (0x0063D890, virtual slot 0)
// Compiler generated with the RandomStraw vtable.
