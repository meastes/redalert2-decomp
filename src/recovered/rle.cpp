// Westwood rle.cpp in one source (twins: mischa85/RedAlert2 src/rle.cpp, TibSun rle.cpp).

#include "shared/crt_memory.hpp"
#include "shared/rle_engine.hpp"


// int RLEEngine::Compress(void const *source, void *dest, int length) const --  (TS rle.cpp)
// Copies source to dest replacing each run of zero bytes (at most 255) by a zero and the run length;
// returns the coded length (dest may be null to only measure it).
#ifndef MIN
#define MIN(a, b) (((a) < (b)) ? (a) : (b))
#endif

int RLEEngine::Compress(void const *source, void *dest, int length) const
{
    int outlen = 0;
    unsigned char const *sptr = (unsigned char const *)source;
    unsigned char *dptr = (unsigned char *)dest;

    while (length > 0) {
        if (*sptr == '\0') {
            int runcount = 0;
            while (sptr[runcount] == '\0' && runcount <= length) {
                runcount++;
            }
            runcount = MIN(runcount, 255);
            if (dptr != 0) {
                *dptr++ = '\0';
                *dptr++ = (unsigned char)runcount;
            }
            outlen += 2;
            sptr += runcount;
            length -= runcount;
        } else {
            if (dptr != 0) {
                *dptr++ = *sptr++;
            } else {
                sptr++;
            }
            outlen++;
            length--;
        }
    }
    return outlen;
}


// int RLEEngine::Line_Compress(void const *source, void *dest, int length) const --  (TS rle.cpp)
// Compress behind a 16-bit size word (the coded length including the word).

int RLEEngine::Line_Compress(void const *source, void *dest, int length) const
{
    if (dest != 0) {
        unsigned short *sizeptr = (unsigned short *)dest;
        int complen = Compress(source, sizeptr + 1, length) + sizeof(short);
        *sizeptr = (unsigned short)complen;
        return complen;
    }
    return Compress(source, 0, length) + sizeof(short);
}


// int RLEEngine::Decompress(void const *source, void *dest, int length) const --  (TS rle.cpp)
// Expands Compress's coding of length source bytes into dest; returns the bytes written.

int RLEEngine::Decompress(void const *source, void *dest, int length) const
{
    unsigned char *dptr = (unsigned char *)dest;
    unsigned char const *sptr = (unsigned char const *)source;

    while (length > 0) {
        unsigned char value = *sptr++;
        length--;
        if (value == '\0') {
            int outlen = *sptr++;
            length--;
            if (outlen > 0) {
                memset(dptr, '\0', outlen);
                dptr += outlen;
            }
        } else {
            *dptr++ = value;
        }
    }
    return dptr - (unsigned char const *)dest;
}


// int RLEEngine::Line_Decompress(void const *source, void *dest) const --  (TS rle.cpp)
// Decompress of a line written by Line_Compress (its size word gives the length).

int RLEEngine::Line_Decompress(void const *source, void *dest) const
{
    unsigned short const *sptr = (unsigned short const *)source;
    int datalen = *sptr++;
    return Decompress(sptr, dest, datalen - sizeof(short));
}
