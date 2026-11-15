/*******************************************************************************
 *                                O P E N  T S
 *******************************************************************************
 * SPDX-License-Identifier: GPL-3.0-or-later
 * Copyright 2025 Electronic Arts Inc.
 * Copyright 2026 OpenTS contributors
 *
 * Contains material derived from Electronic Arts source code.
 * Modified by OpenTS contributors, 2026.
 * EA's GPLv3 Section 7 additional terms and supplemental warranty
 * disclaimers apply; see LICENSE.md.
 ******************************************************************************/
// Function comment blocks from mischa85/RedAlert2 392e904 src/rndstraw.cpp (matching RA2 1.006 decomp), placed above our
// own notes; code unchanged.
// Parameter names from mischa85/RedAlert2 392e904 src/rndstraw.cpp where both define the function (names only).

// rndstraw.cpp: the original translation unit re-filed as one source (Route A): the accepted
// bodies of its canonical compile group in address order, under the unit's own header
// state (its switches and includes in their original order). The module map
// manifests/original_target/modules/rndstraw.json carries the addresses and the rows that
// stay in their own files.


#include "shared/random_straw.hpp"
#include "shared/crt_string.hpp"
#include "shared/sha.hpp"


/***********************************************************************************************
 * RandomStraw::RandomStraw -- Constructor for the random straw class.                         *
 *                                                                                             *
 *    This will initialize the random straw into a known state. The initial state is useless   *
 *    for cryptographic purposes. It must be seeded before it should be used.                  *
 *                                                                                             *
 * INPUT:   none                                                                               *
 *                                                                                             *
 * OUTPUT:  none                                                                               *
 *                                                                                             *
 * WARNINGS:   none                                                                            *
 *                                                                                             *
 * HISTORY:                                                                                    *
 *   07/10/1996 JLB : Created.                                                                 *
 *=============================================================================================*/
// (0x0063D2D0)
// Starts with an empty (all zero) seed.

RandomStraw::RandomStraw() :
    SeedBits(0),
    Current(0)
{
    Reset();
}


/***********************************************************************************************
 * RandomStraw::~RandomStraw -- Destructor for random straw class.                             *
 *                                                                                             *
 *    This destructor will clear out the seed data so that there won't be any sensitive        *
 *    information left over in the memory this object occupied.                                *
 *                                                                                             *
 * INPUT:   none                                                                               *
 *                                                                                             *
 * OUTPUT:  none                                                                               *
 *                                                                                             *
 * WARNINGS:   none                                                                            *
 *                                                                                             *
 * HISTORY:                                                                                    *
 *   07/10/1996 JLB : Created.                                                                 *
 *=============================================================================================*/
// (0x0063D320)
// Clears the seed so no random state is left in memory.

RandomStraw::~RandomStraw()
{
    Reset();
}


/***********************************************************************************************
 * RandomStraw::Reset -- Reset the data to known initial state.                                *
 *                                                                                             *
 *    This routine is functionally equivalent to performing an placement new on the object. It *
 *    clears out all the seed data.                                                            *
 *                                                                                             *
 * INPUT:   none                                                                               *
 *                                                                                             *
 * OUTPUT:  none                                                                               *
 *                                                                                             *
 * WARNINGS:   You must reseed it before fetching random numbers.                              *
 *                                                                                             *
 * HISTORY:                                                                                    *
 *   07/10/1996 JLB : Created.                                                                 *
 *=============================================================================================*/
// (0x0063D350)
// Discards the seed and all generator state.

void RandomStraw::Reset()
{
    SeedBits = 0;
    Current = 0;
    memset(Random, '\0', sizeof(Random));
}


/***********************************************************************************************
 * RandomStraw::Seed_Bits_Needed -- Fetches the number of seed bits needed.                    *
 *                                                                                             *
 *    This routine is used when seeding the random straw. Keep feeding random bits to this     *
 *    class until the Seed_Bits_Needed value returns zero. Random bits should be gathered from *
 *    sources external to the immediate program. Examples would be the system clock (good for  *
 *    a few bits), the delay between player keystrokes (good for a few bits per keystroke),    *
 *    etc. When gathering random bits from integers, use the low order bits (since they        *
 *    characteristically are less predictable and more 'random' than the others).              *
 *                                                                                             *
 * INPUT:   none                                                                               *
 *                                                                                             *
 * OUTPUT:  Returns with the number of bits required in order to fully seed this random        *
 *          number generator.                                                                  *
 *                                                                                             *
 * WARNINGS:   Even if this routine returns zero, you are still allowed and encouraged to feed *
 *             more random bits when the opportunity arrises.                                  *
 *                                                                                             *
 * HISTORY:                                                                                    *
 *   07/10/1996 JLB : Created.                                                                 *
 *=============================================================================================*/
// (0x0063D370)
// Seed bits still wanted before the generators are fully seeded.

int RandomStraw::Seed_Bits_Needed() const
{
    const int total = sizeof(Random) * 8;
    if (SeedBits < total) {
        return total - SeedBits;
    }
    return 0;
}


/***********************************************************************************************
 * RandomStraw::Seed_Bit -- Add a random bit to the accumulated seed value.                    *
 *                                                                                             *
 *    This routine should be called to feed a single random bit to the random straw object.    *
 *    You must feed as many bits as requested by the Seed_Bits_Needed() function. Submitting   *
 *    additional bits is not only allowed, but encouraged.                                     *
 *                                                                                             *
 * INPUT:   seed  -- The bit (lowest order bit) to feed to the random number seed.             *
 *                                                                                             *
 * OUTPUT:  none                                                                               *
 *                                                                                             *
 * WARNINGS:   none                                                                            *
 *                                                                                             *
 * HISTORY:                                                                                    *
 *   07/10/1996 JLB : Created.                                                                 *
 *=============================================================================================*/
// (0x0063D390)
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


/***********************************************************************************************
 * RandomStraw::Seed_Byte -- Submit 8 bits to the random number seed.                          *
 *                                                                                             *
 *    This will submit 8 bits (as specified) to the random number seed.                        *
 *                                                                                             *
 * INPUT:   seed  -- The seed bits to submit.                                                  *
 *                                                                                             *
 * OUTPUT:  none                                                                               *
 *                                                                                             *
 * WARNINGS:   none                                                                            *
 *                                                                                             *
 * HISTORY:                                                                                    *
 *   07/10/1996 JLB : Created.                                                                 *
 *=============================================================================================*/
// (0x0063D470)
// Seeds eight bits, low bit first.

void RandomStraw::Seed_Byte(char seed)
{
    for (int index = 0; index < 8; index++) {
        Seed_Bit(seed);
        seed >>= 1;
    }
}


/***********************************************************************************************
 * RandomStraw::Seed_Short -- Submit 16 bits to the random number seed.                        *
 *                                                                                             *
 *    This will submit 16 bits to the accumulated seed.                                        *
 *                                                                                             *
 * INPUT:   seed  -- The 16 bits to submit to the seed.                                        *
 *                                                                                             *
 * OUTPUT:  none                                                                               *
 *                                                                                             *
 * WARNINGS:   none                                                                            *
 *                                                                                             *
 * HISTORY:                                                                                    *
 *   07/10/1996 JLB : Created.                                                                 *
 *=============================================================================================*/
// (0x0063D580)
// Seeds sixteen bits, low bit first.

void RandomStraw::Seed_Short(short seed)
{
    for (int index = 0; index < 16; index++) {
        Seed_Bit(seed);
        seed >>= 1;
    }
}


/***********************************************************************************************
 * RandomStraw::Seed_Long -- Submit 32 bits to the random number seed.                         *
 *                                                                                             *
 *    This will submit a full 32 bits to the accumulated seed value.                           *
 *                                                                                             *
 * INPUT:   seed  -- The 32 bits to submit.                                                    *
 *                                                                                             *
 * OUTPUT:  none                                                                               *
 *                                                                                             *
 * WARNINGS:   none                                                                            *
 *                                                                                             *
 * HISTORY:                                                                                    *
 *   07/10/1996 JLB : Created.                                                                 *
 *=============================================================================================*/
// (0x0063D680)
// Seeds thirty-two bits, low bit first.

void RandomStraw::Seed_Long(long seed)
{
    for (int index = 0; index < 32; index++) {
        Seed_Bit(seed);
        seed >>= 1;
    }
}


/***********************************************************************************************
 * RandomStraw::Scramble_Seed -- Masks any coorelation between the seed bits.                  *
 *                                                                                             *
 *    This routine is called when a full set of seed bits has been accumulated. It will take   *
 *    the existing seed and scramble the bits. An effective way of doing this is to use the    *
 *    Secure Hash Algorithm. It is necessary to have this routine because there might be       *
 *    some coorelation in the seed bits. Even more important is that this routine will result  *
 *    in every bit of the seed having a scrambling effect on every other bit.                  *
 *                                                                                             *
 * INPUT:   none                                                                               *
 *                                                                                             *
 * OUTPUT:  none                                                                               *
 *                                                                                             *
 * WARNINGS:   none                                                                            *
 *                                                                                             *
 * HISTORY:                                                                                    *
 *   07/10/1996 JLB : Created.                                                                 *
 *=============================================================================================*/
// (0x0063D790)
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


/***********************************************************************************************
 * RandomStraw::Get -- Fetch random data.                                                      *
 *                                                                                             *
 *    This routine will fetch random data and store it into the buffer specified.              *
 *                                                                                             *
 * INPUT:   source   -- Pointer to the buffer to fill with random data.                        *
 *                                                                                             *
 *          length   -- The number of bytes to store into the buffer.                          *
 *                                                                                             *
 * OUTPUT:  Returns with the actual number of bytes stored into the buffer. This will always   *
 *          be the number of bytes requested since random numbers are unexhaustible.           *
 *                                                                                             *
 * WARNINGS:   none                                                                            *
 *                                                                                             *
 * HISTORY:                                                                                    *
 *   07/04/1996 JLB : Created.                                                                 *
 *   07/10/1996 JLB : Revamped to make cryptographically secure.                               *
 *=============================================================================================*/
// (0x0063D830, virtual slot 2)
// Fills the buffer with one byte from each generator in turn; returns the
// bytes delivered.

int RandomStraw::Get(void *source, int slen)
{
    if (source == 0 || slen < 1) {
        return Straw::Get(source, slen);
    }

    int total = 0;
    while (slen > 0) {
        *(char *)source = (char)Random[Current++]();
        Current = Current % (sizeof(Random) / sizeof(Random[0]));
        source = (char *)source + sizeof(char);
        slen--;
        total++;
    }
    return total;
}


// RandomStraw scalar deleting destructor (0x0063D890, virtual slot 0)
// Compiler generated with the RandomStraw vtable.
