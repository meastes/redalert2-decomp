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
// Function comment blocks from mischa85/RedAlert2 392e904 src/pkstraw.cpp (matching RA2 1.006 decomp), placed above our
// own notes; code unchanged.
// Parameter names from mischa85/RedAlert2 392e904 src/pkstraw.cpp where both define the function (names only).

// pkstraw.cpp: the original translation unit re-filed as one source (Route A): the accepted
// bodies of its canonical compile group in address order, under the unit's own header
// state (its switches and includes in their original order). The module map
// manifests/original_target/modules/pkstraw.json carries the addresses and the rows that
// stay in their own files.


#include "shared/pk_straw.hpp"
#include "shared/crt_string.hpp"


/***********************************************************************************************
 * PKStraw::PKStraw -- Initialize the public key straw object.                                 *
 *                                                                                             *
 *    This constructs the public key straw object. The operation to perform (encrypt or        *
 *    decrypt) as well as a random number generator must be provided.                          *
 *                                                                                             *
 * INPUT:   control  -- What operation to perform on the data. Pass in either ENCRYPT or       *
 *                      DECRYPT.                                                               *
 *                                                                                             *
 *          rnd      -- Reference to a random number straw that is used internally to          *
 *                      generate the sub-key. The final strength of the cipher depends on      *
 *                      quality of this random number generator.                               *
 *                                                                                             *
 * OUTPUT:  none                                                                               *
 *                                                                                             *
 * WARNINGS:   none                                                                            *
 *                                                                                             *
 * HISTORY:                                                                                    *
 *   07/11/1996 JLB : Created.                                                                 *
 *=============================================================================================*/
// (0x0060FE30)
// The embedded Blowfish straw works in the same direction and is the
// straw this one draws from; the key exchange starts with the first data.

PKStraw::PKStraw(CryptControl control, RandomStraw &rnd) :
    IsGettingKey(true),
    Rand(rnd),
    BF((control == ENCRYPT) ? BlowStraw::ENCRYPT : BlowStraw::DECRYPT),
    Control(control),
    CipherKey(0),
    Counter(0),
    BytesLeft(0)
{
    Get_From(BF);
}


/***********************************************************************************************
 * PKStraw::Get_From -- Chains one straw to another.                                           *
 *                                                                                             *
 *    This routine handles the special case of this straw object in that there is an           *
 *    embedded blowfish straw segment. It must be chained on correctly.                        *
 *                                                                                             *
 * INPUT:   straw -- Pointer to the straw segment that this segment is to receive data from.   *
 *                                                                                             *
 * OUTPUT:  none                                                                               *
 *                                                                                             *
 * WARNINGS:   none                                                                            *
 *                                                                                             *
 * HISTORY:                                                                                    *
 *   07/11/1996 JLB : Created.                                                                 *
 *=============================================================================================*/
// (0x0060FEA0, virtual slot 1)
// Connects the input: the Blowfish stage draws from straw and this straw
// draws from the Blowfish stage. The source straw records this straw (not
// the Blowfish stage) as the one drawing from it.

void PKStraw::Get_From(Straw *straw)
{
    if (BF.ChainTo != straw) {
        if (straw != 0 && straw->ChainFrom != 0) {
            straw->ChainFrom->Get_From(0);
            straw->ChainFrom = 0;
        }

        if (BF.ChainTo != 0) {
            BF.ChainTo->ChainFrom = 0;
        }

        BF.ChainTo = straw;
        BF.ChainFrom = this;
        ChainTo = &BF;
        if (straw != 0) {
            straw->ChainFrom = this;
        }
    }
}


/***********************************************************************************************
 * PKStraw::Key -- Assign a key to the cipher process straw.                                   *
 *                                                                                             *
 *    This routine will assign the key (or NULL if the current key is to be removed) to the    *
 *    cipher stream process. When a key has been assigned, encryption or decryption will       *
 *    take place. In the absence (NULL key pointer) of a key, the data passes through          *
 *    unchanged.                                                                               *
 *                                                                                             *
 * INPUT:   key   -- Pointer to the key to assign to the stream. If the key pointer is NULL,   *
 *                   then this causes the cipher stream to stop processing the data and will   *
 *                   pass the data through unchanged.                                          *
 *                                                                                             *
 * OUTPUT:  none                                                                               *
 *                                                                                             *
 * WARNINGS:   Be sure that the key passed to this routine is the opposite key to that used    *
 *             to process the stream originally (when decrypting).                             *
 *                                                                                             *
 * HISTORY:                                                                                    *
 *   07/08/1996 JLB : Created.                                                                 *
 *=============================================================================================*/
// (0x0060FEF0)
// Sets the key and restarts the key exchange.

void PKStraw::Key(PKey const *key)
{
    CipherKey = key;
    if (key != 0) {
        IsGettingKey = true;
    }
    Counter = 0;
    BytesLeft = 0;
}


/***********************************************************************************************
 * PKStraw::Get -- Fetch data and process it accordingly.                                      *
 *                                                                                             *
 *    This routine will fetch the number of bytes requested. If a valid key has been assigned  *
 *    to this stream, then the data will be processed as it passes through.                    *
 *                                                                                             *
 * INPUT:   source   -- Pointer to the buffer that will hold the requested data.               *
 *                                                                                             *
 *          length   -- The number of data bytes requested.                                    *
 *                                                                                             *
 * OUTPUT:  Returns with the actual number of data bytes stored to the destination buffer. If  *
 *          this number is less than that requested, then it indicates that the data source    *
 *          has been exhausted.                                                                *
 *                                                                                             *
 * WARNINGS:   none                                                                            *
 *                                                                                             *
 * HISTORY:                                                                                    *
 *   07/08/1996 JLB : Created.                                                                 *
 *=============================================================================================*/
// (0x0060FF10, virtual slot 2)
// Before the first data: decrypting, reads the encrypted Blowfish key,
// decrypts it with the private key and keys the Blowfish stage;
// encrypting, makes a random Blowfish key, encrypts it with the public key
// for output and keys the Blowfish stage. The encrypted key is handed out
// first, then data drawn through the Blowfish stage. Returns the bytes
// delivered (0 if the encrypted key could not be read).

int PKStraw::Get(void *source, int length)
{
    if (source == 0 || length < 1 || CipherKey == 0) {
        return Straw::Get(source, length);
    }

    int total = 0;

    if (IsGettingKey) {
        if (Control == DECRYPT) {
            char cbuffer[MAX_KEY_BLOCK_SIZE];
            int got = Straw::Get(cbuffer, Encrypted_Key_Length());
            if (got != Encrypted_Key_Length()) {
                return 0;
            }

            CipherKey->Decrypt(cbuffer, got, Buffer);
            BF.Key(Buffer, BLOWFISH_KEY_SIZE);
        } else {
            char cbuffer[MAX_KEY_BLOCK_SIZE];
            memset(cbuffer, '\0', sizeof(cbuffer));
            Rand.Get(cbuffer, BLOWFISH_KEY_SIZE);

            Counter = BytesLeft = CipherKey->Encrypt(cbuffer, Plain_Key_Length(), Buffer);
            BF.Key(cbuffer, BLOWFISH_KEY_SIZE);
        }
        IsGettingKey = false;
    }

    if (BytesLeft > 0) {
        int count = (length < BytesLeft) ? length : BytesLeft;
        memmove(source, &Buffer[Counter - BytesLeft], count);
        source = (char *)source + count;
        BytesLeft -= count;
        length -= count;
        total += count;
    }

    total += Straw::Get(source, length);
    return total;
}


/***********************************************************************************************
 * PKStraw::Encrypted_Key_Length -- Fetch the encrypted key length.                            *
 *                                                                                             *
 *    This returns the total number of bytes (after encryption) that the blowfish key will     *
 *    consume. It should be possible to get a block of this size, then pass it to the          *
 *    public key decrypter and the result will be the full blowfish key.                       *
 *                                                                                             *
 * INPUT:   none                                                                               *
 *                                                                                             *
 * OUTPUT:  Returns with the number of bytes that the encrypted blowfish key required.         *
 *                                                                                             *
 * WARNINGS:   none                                                                            *
 *                                                                                             *
 * HISTORY:                                                                                    *
 *   07/11/1996 JLB : Created.                                                                 *
 *=============================================================================================*/
// (0x006100E0)
// Size of the Blowfish key once encrypted with the key (0 without a key).

int PKStraw::Encrypted_Key_Length() const
{
    if (CipherKey == 0) {
        return 0;
    }
    return CipherKey->Block_Count(BLOWFISH_KEY_SIZE) * CipherKey->Crypt_Block_Size();
}


/***********************************************************************************************
 * PKStraw::Plain_Key_Length -- Returns the number of bytes to encrypt key.                    *
 *                                                                                             *
 *    This is the number of plain (unencrypted) bytes that the blowfish key will take up. This *
 *    is actually the number of plain blocks minimum that can contain the full blowfish        *
 *    key. The public key cryptography system encrypts in whole blocks only.                   *
 *                                                                                             *
 * INPUT:   none                                                                               *
 *                                                                                             *
 * OUTPUT:  Returns with the total number of bytes that will contain the full blowfish key     *
 *          and still be an even block size for the public key cryptography process.           *
 *                                                                                             *
 * WARNINGS:   This value is probably be larger than the actual blowfish key length.           *
 *                                                                                             *
 * HISTORY:                                                                                    *
 *   07/11/1996 JLB : Created.                                                                 *
 *=============================================================================================*/
// (0x00610110)
// Size of the Blowfish key padded to whole plaintext blocks of the key
// (0 without a key).

int PKStraw::Plain_Key_Length() const
{
    if (CipherKey == 0) {
        return 0;
    }
    return CipherKey->Block_Count(BLOWFISH_KEY_SIZE) * CipherKey->Plain_Block_Size();
}


// PKStraw::Get_From(Straw &straw) (0x00610140, virtual slot 3)
// Inline in the class declaration; emitted with the PKStraw vtable.


// PKStraw scalar deleting destructor (0x00610150, virtual slot 0)
// Compiler generated; destroys the embedded BlowStraw.
