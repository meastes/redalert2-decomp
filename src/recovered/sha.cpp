// sha.cpp: the original translation unit re-filed as one source (Route A): the accepted
// bodies of its canonical compile group in address order, under the unit's own header
// state (its switches and includes in their original order). The module map
// manifests/original_target/modules/sha.json carries the addresses and the rows that
// stay in their own files.


#include "shared/sha.hpp"
#include "shared/crt_memory.hpp"
#define Reverse_LONG(a) ((((a) >> 24) & 0x000000FFL) | (((a) >> 8) & 0x0000FF00L) | (((a) << 8) & 0x00FF0000L) | (((a) << 24) & 0xFF000000L))


// SHAEngine::Process_Partial(void const *&data, long &length) (0x00674BF0)
// Stages source bytes in Partial when a block is already partly staged or
// fewer than a block's bytes remain, advancing data and length; hashes
// Partial once it holds a whole block.

void SHAEngine::Process_Partial(void const *&data, long &length)
{
    if (length == 0 || data == 0) {
        return;
    }

    if (PartialCount == 0 && length >= SRC_BLOCK_SIZE) {
        return;
    }

    int add_count = ((int)length < (int)SRC_BLOCK_SIZE - PartialCount) ? (int)length : (int)SRC_BLOCK_SIZE - PartialCount;
    memcpy(&Partial[PartialCount], data, add_count);
    data = ((char const *)data) + add_count;
    PartialCount += add_count;
    length -= add_count;

    if (PartialCount == SRC_BLOCK_SIZE) {
        Process_Block(&Partial[0], Acc);
        Length += (long)SRC_BLOCK_SIZE;
        PartialCount = 0;
    }
}


// SHAEngine::Hash(void const *data, long length) (0x00674C90)
// Adds data to the digest: completes a staged block, hashes whole blocks
// straight from the source and stages the remainder.

void SHAEngine::Hash(void const *data, long length)
{
    IsCached = false;

    Process_Partial(data, length);
    if (length == 0) {
        return;
    }

    long blocks = length / SRC_BLOCK_SIZE;
    long const *source = (long const *)data;
    for (int bcount = 0; bcount < blocks; bcount++) {
        Process_Block(source, Acc);
        Length += (long)SRC_BLOCK_SIZE;
        source += SRC_BLOCK_SIZE / sizeof(long);
        length -= (long)SRC_BLOCK_SIZE;
    }

    data = source;
    Process_Partial(data, length);
}


// SHAEngine::Result(void *result) const (0x00674D10)
// Pads a copy of the staged data with the end bit and the bit length,
// hashes it into a copy of the accumulator and returns the big-endian
// digest (20 bytes). A cached digest is copied out first but always
// recomputed (original behaviour).


int SHAEngine::Result(void *result) const
{
    if (IsCached) {
        memcpy(result, &FinalResult, sizeof(FinalResult));
    }

    long length = Length + PartialCount;
    int partialcount = PartialCount;
    char partial[SRC_BLOCK_SIZE];
    memcpy(partial, Partial, sizeof(Partial));

    // End the data with a 1 bit.
    partial[partialcount] = (char)0x80;

    // Without room for the 8-byte length, finish this block first.
    SHADigest acc = Acc;
    if ((SRC_BLOCK_SIZE - partialcount) < 9) {
        if (partialcount + 1 < SRC_BLOCK_SIZE) {
            memset(&partial[partialcount + 1], '\0', SRC_BLOCK_SIZE - (partialcount + 1));
        }
        Process_Block(&partial[0], acc);
        partialcount = 0;
    } else {
        partialcount++;
    }

    // The bit length (low 32 bits) ends the last block, big-endian.
    memset(&partial[partialcount], '\0', SRC_BLOCK_SIZE - partialcount);
    *(long *)(&partial[SRC_BLOCK_SIZE - 4]) = Reverse_LONG((length * 8));
    Process_Block(&partial[0], acc);

    memcpy((char *)&FinalResult, &acc, sizeof(acc));
    for (int index = 0; index < sizeof(FinalResult) / sizeof(long); index++) {
        (long &)FinalResult.Long[index] = Reverse_LONG(FinalResult.Long[index]);
    }
    (bool &)IsCached = true;
    memcpy(result, &FinalResult, sizeof(FinalResult));
    return sizeof(FinalResult);
}


// SHAEngine::Process_Block(void const *source, SHADigest &acc) const (0x00674E60)
// One SHA-1 compression: expands the big-endian 16-word block to 80 words
// and runs the 80 steps, adding the result into acc.

// Rotates a left by n bits (an inline function, not a macro: that decides
// how the compiler orders the step sum).
inline unsigned long Rotate_Left(unsigned long a, int n) { return (a << n) | (a >> (32 - n)); }

void SHAEngine::Process_Block(void const *source, SHADigest &acc) const
{
    long block[PROC_BLOCK_SIZE / sizeof(long)];

    long const *data = (long const *)source;
    unsigned a;
    for (a = 0; a < SRC_BLOCK_SIZE / sizeof(long); a++) {
        block[a] = Reverse_LONG(data[a]);
    }

    for (a = SRC_BLOCK_SIZE / sizeof(long); a < PROC_BLOCK_SIZE / sizeof(long); a++) {
        unsigned long value = block[a - 3] ^ block[a - 8] ^ block[a - 14] ^ block[a - 16];
        block[a] = (long)Rotate_Left(value, 1);
    }

    SHADigest alt = acc;
    for (a = 0; a < PROC_BLOCK_SIZE / sizeof(long); a++) {
        unsigned long temp = Rotate_Left(alt.Long[0], 5) + Do_Function(a, alt.Long[1], alt.Long[2], alt.Long[3])
                           + alt.Long[4] + block[a] + Get_Constant(a);
        alt.Long[4] = alt.Long[3];
        alt.Long[3] = alt.Long[2];
        alt.Long[2] = Rotate_Left(alt.Long[1], 30);
        alt.Long[1] = alt.Long[0];
        alt.Long[0] = temp;
    }

    acc.Long[0] += alt.Long[0];
    acc.Long[1] += alt.Long[1];
    acc.Long[2] += alt.Long[2];
    acc.Long[3] += alt.Long[3];
    acc.Long[4] += alt.Long[4];
}
