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
// Function comment blocks from mischa85/RedAlert2 392e904 src/pk.cpp (matching RA2 1.006 decomp), placed above our
// own notes; code unchanged.

// pk.cpp: the original translation unit re-filed as one source (Route A): the accepted
// bodies of its canonical compile group in address order, under the unit's own header
// state (its switches and includes in their original order). The module map
// manifests/original_target/modules/pk.json carries the addresses and the rows that
// stay in their own files.


#include "shared/pkey.hpp"
#include "shared/crt_memory.hpp"
#include "shared/crt_string.hpp"


/***********************************************************************************************
 * PKey::PKey -- Construct a key using encoded strings.                                        *
 *                                                                                             *
 *    This constructor will construct a key based on the encoded strings supplied.             *
 *                                                                                             *
 * INPUT:   exponent -- The encoded string for the exponent portion of the key.                *
 *                                                                                             *
 *          modulus  -- The encoded string for the modulus portion of the key.                 *
 *                                                                                             *
 * OUTPUT:  none                                                                               *
 *                                                                                             *
 * WARNINGS:   none                                                                            *
 *                                                                                             *
 * HISTORY:                                                                                    *
 *   07/08/1996 JLB : Created.                                                                 *
 *=============================================================================================*/
// (0x0060ED40)
// Builds a key from the DER encodings of its exponent and modulus.

PKey::PKey(void const *exponent, void const *modulus)
{
    Modulus.DERDecode((unsigned char const *)modulus);
    Exponent.DERDecode((unsigned char const *)exponent);
    BitPrecision = Modulus.BitCount() - 1;
}


/***********************************************************************************************
 * PKey::Encode_Modulus -- Encode the modulus portion of the key.                              *
 *                                                                                             *
 *    This will store the modulus portion of the key into a buffer. The number of bytes        *
 *    stored into the buffer depends on the value of the key.                                  *
 *                                                                                             *
 * INPUT:   buffer   -- Pointer to the buffer that will hold the encoded modulus value.        *
 *                                                                                             *
 * OUTPUT:  Returns with the number of bytes stored to the buffer.                             *
 *                                                                                             *
 * WARNINGS:   Be sure that the buffer can hold the encoded bytes. This is normally around the *
 *             same size as the Crypt_Block_Size() (plus a byte or two).                       *
 *                                                                                             *
 * HISTORY:                                                                                    *
 *   07/08/1996 JLB : Created.                                                                 *
 *=============================================================================================*/
// (0x0060EDA0)
// Writes the DER encoding of the modulus; returns its size.

int PKey::Encode_Modulus(void *buffer) const
{
    if (buffer == 0) {
        return 0;
    }
    return Modulus.DEREncode((unsigned char *)buffer);
}


/***********************************************************************************************
 * PKey::Encode_Exponent -- Encode the exponent portion of the key into a buffer.              *
 *                                                                                             *
 *    This routine will encode the exponent portion of the key. This is only necessary for the *
 *    slow key since the fast key always has an exponent of 65537.                             *
 *                                                                                             *
 * INPUT:   buffer   -- Pointer to the buffer that will be filled with the encoded exponent.   *
 *                                                                                             *
 * OUTPUT:  Returns with the nuber of bytes stored into the buffer.                            *
 *                                                                                             *
 * WARNINGS:   Be sure the buffer is big enough to hold the encoded exponent. Usually this is  *
 *             about the same size as the Crypt_Block_Size (plus a byte or two).               *
 *                                                                                             *
 * HISTORY:                                                                                    *
 *   07/08/1996 JLB : Created.                                                                 *
 *=============================================================================================*/
// (0x0060EDC0)
// Writes the DER encoding of the exponent; returns its size.

int PKey::Encode_Exponent(void *buffer) const
{
    if (buffer == 0) {
        return 0;
    }
    return Exponent.DEREncode((unsigned char *)buffer);
}


/***********************************************************************************************
 * PKey::Decode_Modulus -- Decodes the modulus value back into the key.                        *
 *                                                                                             *
 *    This is the counterpart to the Encode_Modulus() function. It will initialize the         *
 *    modulus portion of the key with the encoded data supplied.                               *
 *                                                                                             *
 * INPUT:   buffer   -- Pointer to the buffer that holds the previously encoded modulus value. *
 *                                                                                             *
 * OUTPUT:  void                                                                               *
 *                                                                                             *
 * WARNINGS:   void                                                                            *
 *                                                                                             *
 * HISTORY:                                                                                    *
 *   07/08/1996 JLB : Created.                                                                 *
 *=============================================================================================*/
// (0x0060EDE0)
// Reads the modulus from its DER encoding and updates the precision.

void PKey::Decode_Modulus(void *buffer)
{
    Modulus.DERDecode((unsigned char const *)buffer);
    BitPrecision = Modulus.BitCount() - 1;
}


/***********************************************************************************************
 * PKey::Decode_Exponent -- Decodes the exponent back into the key.                            *
 *                                                                                             *
 *    This is the counterpart to the Encode_Exponent function. It will decode a previously     *
 *    encoded exponent portion back into the key.                                              *
 *                                                                                             *
 * INPUT:   buffer   -- Pointer to the buffer that holds the encoded exponent value.           *
 *                                                                                             *
 * OUTPUT:  none                                                                               *
 *                                                                                             *
 * WARNINGS:   none                                                                            *
 *                                                                                             *
 * HISTORY:                                                                                    *
 *   07/08/1996 JLB : Created.                                                                 *
 *=============================================================================================*/
// (0x0060EE10)
// Reads the exponent from its DER encoding.

void PKey::Decode_Exponent(void *buffer)
{
    Exponent.DERDecode((unsigned char const *)buffer);
}


/***********************************************************************************************
 * PKey::Generate -- Generate a public and private key.                                        *
 *                                                                                             *
 *    Public key cryptography relies on having two paired keys. The key used to encrypt        *
 *    data must be decrypted by using the other key. Which key designated as the public or     *
 *    private key is arbitrary. However, one is faster than the other. Use the faster key for  *
 *    the more common operation.                                                               *
 *                                                                                             *
 * INPUT:   random   -- Reference to a source of random data.                                  *
 *                                                                                             *
 *          bits     -- The number of bits to use for key generation. Use a number greater     *
 *                      than 16 but less than 2048. The ideal bit size is one that is evenly   *
 *                      divisible by 8 and then add one. Practical numbers range from 65 to    *
 *                      1025 bits.                                                             *
 *                                                                                             *
 *          fastkey  -- Reference to the key that has fast encryption/decryption properties.   *
 *                                                                                             *
 *          slowkey  -- Reference to the mate key of the other.                                *
 *                                                                                             *
 * OUTPUT:  none                                                                               *
 *                                                                                             *
 * WARNINGS:   This routine can take a very long time. It can take MINUTES to generate a       *
 *             1024 bit key (even on a Pentium Pro 200Mghz machine).                           *
 *                                                                                             *
 * HISTORY:                                                                                    *
 *   07/05/1996 JLB : Created.                                                                 *
 *   07/10/1996 JLB : Must supply source of random data.                                       *
 *=============================================================================================*/
// (0x0060EE30)
// Makes an RSA key pair: two safe primes p and q, public exponent 65537
// (fastkey) and its inverse modulo (p-1)(q-1) (slowkey), both over n = pq.
// Repeats until a random test block survives an encrypt/decrypt round trip.
// Takes its first two arguments in ECX/EDX (__fastcall).

extern "C" int __cdecl rand();

void __fastcall PKey::Generate(Straw &random, int bits, PKey &fastkey, PKey &slowkey)
{
    char plain[256];
    char crypt[256];

    do {
        BigInt p = Generate_Prime(random, bits, &p);
        BigInt q = Generate_Prime(random, bits, &q);
        BigInt e = 65537L;
        BigInt n = BigInt::Unsigned_Mult(p, q);
        BigInt pqmin = BigInt::Unsigned_Mult(p - (unsigned short)1, q - (unsigned short)1);
        BigInt d = e.Inverse(pqmin);

        fastkey.Exponent = e;
        fastkey.Modulus = n;
        fastkey.BitPrecision = n.BitCount() - 1;

        slowkey.Exponent = d;
        slowkey.Modulus = n;
        slowkey.BitPrecision = fastkey.BitPrecision;

        for (int index = 0; index < fastkey.Plain_Block_Size(); index++) {
            plain[index] = (char)rand();
        }
        fastkey.Encrypt(plain, fastkey.Plain_Block_Size(), crypt);
        slowkey.Decrypt(crypt, slowkey.Crypt_Block_Size(), crypt);
    } while (memcmp(plain, crypt, fastkey.Plain_Block_Size()) != 0);
}


/***********************************************************************************************
 * PKey::Encrypt -- Encrypt blocks of plaintext.                                               *
 *                                                                                             *
 *    This routine will encrypt the supplied plaintext into cyphertext by processing the input *
 *    in block. The source is processed in whole blocks. Partial blocks are not supported by   *
 *    public key cryptography.                                                                 *
 *                                                                                             *
 * INPUT:   source   -- Pointer to the source plaintext that will be encrypted.                *
 *                                                                                             *
 *          length   -- The length of the plaintext to encrypt.                                *
 *                                                                                             *
 *          dest     -- Pointer to the buffer that will hold the encrypted data.               *
 *                                                                                             *
 * OUTPUT:  Returns with the number of cypher text bytes placed into the destination buffer.   *
 *                                                                                             *
 * WARNINGS:   Be sure that the destination buffer is big enough to hold the output.           *
 *                                                                                             *
 * HISTORY:                                                                                    *
 *   07/05/1996 JLB : Created.                                                                 *
 *=============================================================================================*/
// (0x0060F520)
// Raises each whole plaintext block to the exponent modulo the modulus;
// returns the cyphertext bytes written.

int PKey::Encrypt(void const *source, int length, void *dest) const
{
    int total = 0;

    while (length >= Plain_Block_Size()) {
        BigInt temp = 0;
        memmove(&temp, source, Plain_Block_Size());
        temp = temp.exp_b_mod_c(Exponent, Modulus);

        memmove(dest, &temp, Crypt_Block_Size());
        length -= Plain_Block_Size();
        source = (char const *)source + Plain_Block_Size();
        dest = (char *)dest + Crypt_Block_Size();
        total += Crypt_Block_Size();
    }

    return total;
}


/***********************************************************************************************
 * PKey::Decrypt -- Decrypt supplied cyphertext into its original plaintext.                   *
 *                                                                                             *
 *    This routine will process the supplied cyphertext by breaking it up into blocks and      *
 *    then decrypting each block in turn. The block size is dependant upon the key. By NOT     *
 *    embedding this information into the cypher data, it makes the encryption more secure.    *
 *                                                                                             *
 * INPUT:   source   -- Pointer to the cypher text to be decrypted.                            *
 *                                                                                             *
 *          length   -- The number of cypher text bytes supplied to this routine.              *
 *                                                                                             *
 *          dest     -- Pointer to the buffer to hold the plaintext.                           *
 *                                                                                             *
 * OUTPUT:  Returns with the number of plaintext bytes output to the destination buffer.       *
 *                                                                                             *
 * WARNINGS:   Only whole blocks are processed. If the source has any partial block sized      *
 *             data, then it will be left unprocessed.                                         *
 *                                                                                             *
 * HISTORY:                                                                                    *
 *   07/05/1996 JLB : Created.                                                                 *
 *=============================================================================================*/
// (0x0060F650)
// Raises each whole cyphertext block to the exponent modulo the modulus;
// returns the plaintext bytes written.

int PKey::Decrypt(void const *source, int length, void *dest) const
{
    int total = 0;
    BigInt temp;

    while (length >= Crypt_Block_Size()) {
        temp = 0;
        memmove(&temp, source, Crypt_Block_Size());
        temp = temp.exp_b_mod_c(Exponent, Modulus);

        memmove(dest, &temp, Plain_Block_Size());
        length -= Crypt_Block_Size();
        source = (char const *)source + Crypt_Block_Size();
        dest = (char *)dest + Plain_Block_Size();
        total += Plain_Block_Size();
    }

    return total;
}


// Int<64>::operator-(unsigned short) const (0x0060F7A0)
// Template member defined inline in shared/int.hpp; the BigInt instance
// was emitted out of line in PK.cpp (called from PKey::Generate).


// Int<64>::Inverse(Int const &modulus) const (0x0060F800)
// Template member defined inline in shared/int.hpp; the BigInt instance
// was emitted out of line in PK.cpp (called from PKey::Generate).


// Int<64>::Randomize(Straw &, Int const &, Int const &) (0x0060F850)
// Template member defined inline in shared/int.hpp; the BigInt instance
// was emitted out of line in PK.cpp (called from PKey::Generate).


// Int<64>::operator+=(Int const &) (0x0060F880)
// Template member defined inline in shared/int.hpp; the BigInt instance
// was emitted out of line in PK.cpp (called from PKey::Generate).


// Int<64>::operator+(unsigned short) const (0x0060F8A0)
// Template member defined inline in shared/int.hpp; the BigInt instance
// was emitted out of line in PK.cpp (called from PKey::Generate).


// Int<64>::operator*(unsigned short) const (0x0060F900)
// Template member defined inline in shared/int.hpp; the BigInt instance
// was emitted out of line in PK.cpp (called from PKey::Generate).


// Int<64>::operator<<(int) const (0x0060F950)
// Template member defined inline in shared/int.hpp; the BigInt instance
// was emitted out of line in PK.cpp (called from PKey::Generate).


// Int<64>::operator>(Int const &) const (0x0060F9A0)
// Template member defined inline in shared/int.hpp; the BigInt instance
// was emitted out of line in PK.cpp (called from PKey::Generate).


// RemainderTable<BigInt>::RemainderTable(BigInt const &) (0x0060F9C0)
// Template member defined inline in shared/int.hpp; the BigInt instance
// was emitted out of line in PK.cpp (called from PKey::Generate).


// RemainderTable<BigInt>::Increment(unsigned short) (0x0060FA20)
// Template member defined inline in shared/int.hpp; the BigInt instance
// was emitted out of line in PK.cpp (called from PKey::Generate).
