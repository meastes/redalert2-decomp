// pk.cpp: the original translation unit re-filed as one source (Route A): the accepted
// bodies of its canonical compile group in address order, under the unit's own header
// state (its switches and includes in their original order). The module map
// manifests/original_target/modules/pk.json carries the addresses and the rows that
// stay in their own files.


#include "shared/pkey.hpp"
#include "shared/crt_memory.hpp"
#include "shared/crt_string.hpp"


// PKey::PKey(void const *exponent, void const *modulus) (0x0060ED40)
// Builds a key from the DER encodings of its exponent and modulus.

PKey::PKey(void const *exponent, void const *modulus)
{
    Modulus.DERDecode((unsigned char const *)modulus);
    Exponent.DERDecode((unsigned char const *)exponent);
    BitPrecision = Modulus.BitCount() - 1;
}


// PKey::Encode_Modulus(void *buffer) const (0x0060EDA0)
// Writes the DER encoding of the modulus; returns its size.

int PKey::Encode_Modulus(void *buffer) const
{
    if (buffer == 0) {
        return 0;
    }
    return Modulus.DEREncode((unsigned char *)buffer);
}


// PKey::Encode_Exponent(void *buffer) const (0x0060EDC0)
// Writes the DER encoding of the exponent; returns its size.

int PKey::Encode_Exponent(void *buffer) const
{
    if (buffer == 0) {
        return 0;
    }
    return Exponent.DEREncode((unsigned char *)buffer);
}


// PKey::Decode_Modulus(void *buffer) (0x0060EDE0)
// Reads the modulus from its DER encoding and updates the precision.

void PKey::Decode_Modulus(void *buffer)
{
    Modulus.DERDecode((unsigned char const *)buffer);
    BitPrecision = Modulus.BitCount() - 1;
}


// PKey::Decode_Exponent(void *buffer) (0x0060EE10)
// Reads the exponent from its DER encoding.

void PKey::Decode_Exponent(void *buffer)
{
    Exponent.DERDecode((unsigned char const *)buffer);
}


// PKey::Generate(Straw &random, int bits, PKey &fastkey, PKey &slowkey) (0x0060EE30)
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


// PKey::Encrypt(void const *source, int length, void *dest) const (0x0060F520)
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


// PKey::Decrypt(void const *source, int length, void *dest) const (0x0060F650)
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
