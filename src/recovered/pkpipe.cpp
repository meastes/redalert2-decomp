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
// Function comment blocks from mischa85/RedAlert2 392e904 src/pkpipe.cpp (matching RA2 1.006 decomp), placed above our
// own notes; code unchanged.

// pkpipe.cpp: the original translation unit re-filed as one source (Route A): the accepted
// bodies of its canonical compile group in address order, under the unit's own header
// state (its switches and includes in their original order). The module map
// manifests/original_target/modules/pkpipe.json carries the addresses and the rows that
// stay in their own files.


#include "shared/pk_pipe.hpp"
#include "shared/crt_string.hpp"


/***********************************************************************************************
 * PKPipe::PKPipe -- Constructor for the public key pipe object.                               *
 *                                                                                             *
 *    This will construct the public key pipe object.                                          *
 *                                                                                             *
 * INPUT:   control  -- The method used to process the data flow (encrypt or decrypt).         *
 *                                                                                             *
 *          rnd      -- Reference to a random number generate used to create the internal      *
 *                      blowfish key.                                                          *
 *                                                                                             *
 * OUTPUT:  none                                                                               *
 *                                                                                             *
 * WARNINGS:   none                                                                            *
 *                                                                                             *
 * HISTORY:                                                                                    *
 *   07/11/1996 JLB : Created.                                                                 *
 *=============================================================================================*/
// (0x0060FA90)
// The embedded Blowfish pipe works in the same direction; the key exchange
// starts with the first data.

PKPipe::PKPipe(CryptControl control, RandomStraw &rnd) :
    IsGettingKey(true),
    Rand(rnd),
    BF((control == ENCRYPT) ? BlowPipe::ENCRYPT : BlowPipe::DECRYPT),
    Control(control),
    CipherKey(0),
    Counter(0),
    BytesLeft(0)
{
}


/***********************************************************************************************
 * PKPipe::Put_To -- Chains one pipe to another.                                               *
 *                                                                                             *
 *    This handles linking of one pipe to this pipe. Data will flow from this PKPipe to the    *
 *    pipe segment specified. Special handling is done so that piping actually flows to the    *
 *    embedded blowfish pipe and then flows to the designated pipe.                            *
 *                                                                                             *
 * INPUT:   pipe  -- Pointer to the pipe that this pipe segment is to send data to.            *
 *                                                                                             *
 * OUTPUT:  none                                                                               *
 *                                                                                             *
 * WARNINGS:   none                                                                            *
 *                                                                                             *
 * HISTORY:                                                                                    *
 *   07/12/1996 JLB : Created.                                                                 *
 *=============================================================================================*/
// (0x0060FAF0, virtual slot 3)
// Connects the output: this pipe feeds its Blowfish stage, which feeds pipe.

void PKPipe::Put_To(Pipe *pipe)
{
    if (BF.ChainTo != pipe) {
        if (pipe != 0 && pipe->ChainFrom != 0) {
            pipe->ChainFrom->Put_To(0);
            pipe->ChainFrom = 0;
        }

        if (BF.ChainTo != 0) {
            BF.ChainTo->ChainFrom = 0;
        }

        BF.ChainTo = pipe;
        if (pipe != 0) {
            pipe->ChainFrom = &BF;
        }
        BF.ChainFrom = this;
        ChainTo = &BF;
    }
}


/***********************************************************************************************
 * PKPipe::Key -- Submit a key to enable processing of data flow.                              *
 *                                                                                             *
 *    This routine must be called with a valid key pointer in order for encryption/description *
 *    to be performed on the data stream. Prior to calling this routine or after calling this  *
 *    routine with a NULL pointer, the data stream will pass through this pipe without         *
 *    modification.                                                                            *
 *                                                                                             *
 * INPUT:   key   -- Pointer to the key to use for processing. Pass NULL if process is to be   *
 *                   terminated.                                                               *
 *                                                                                             *
 * OUTPUT:  none                                                                               *
 *                                                                                             *
 * WARNINGS:   none                                                                            *
 *                                                                                             *
 * HISTORY:                                                                                    *
 *   07/07/1996 JLB : Created.                                                                 *
 *=============================================================================================*/
// (0x0060FB40)
// Sets the key and restarts the key exchange; a null key flushes the pipe
// and turns encryption off. Decrypting, the encrypted Blowfish key length
// is expected first.

void PKPipe::Key(PKey const *key)
{
    if (key == 0) {
        Flush();
        IsGettingKey = false;
    }
    CipherKey = key;

    if (CipherKey != 0) {
        IsGettingKey = true;
        if (Control == DECRYPT) {
            Counter = BytesLeft = Encrypted_Key_Length();
        }
    }
}


/***********************************************************************************************
 * PKPipe::Put -- Submit data to the pipe for processing.                                      *
 *                                                                                             *
 *    This routine (if processing as been enabled by a previous key submission) will           *
 *    encrypt or decrypt the data stream that passes through it. When encrypting, the data     *
 *    stream will increase in size by about 10% (bit it varies according to the key used).     *
 *                                                                                             *
 * INPUT:   source   -- Pointer to the data to be submitted to the pipe stream.                *
 *                                                                                             *
 *          length   -- The number of bytes submitted.                                         *
 *                                                                                             *
 * OUTPUT:  Returns with the actual number of byte output at the final end of the pipe.        *
 *                                                                                             *
 * WARNINGS:   none                                                                            *
 *                                                                                             *
 * HISTORY:                                                                                    *
 *   07/07/1996 JLB : Created.                                                                 *
 *=============================================================================================*/
// (0x0060FBA0, virtual slot 4)
// Before the first data: encrypting, makes a random Blowfish key, sends it
// encrypted with the public key and keys the Blowfish stage; decrypting,
// gathers the encrypted Blowfish key, decrypts it with the private key and
// keys the Blowfish stage. All other data goes to the Blowfish stage.
// Returns the bytes passed down the chain.

int PKPipe::Put(void const *source, int length)
{
    if (source == 0 || length < 1 || CipherKey == 0) {
        return Pipe::Put(source, length);
    }

    int total = 0;

    if (IsGettingKey) {
        if (Control == ENCRYPT) {
            char buffer[MAX_KEY_BLOCK_SIZE];
            memset(buffer, '\0', sizeof(buffer));
            Rand.Get(buffer, BLOWFISH_KEY_SIZE);
            int didput = CipherKey->Encrypt(buffer, Plain_Key_Length(), Buffer);
            total += Pipe::Put(Buffer, didput);
            BF.Key(buffer, BLOWFISH_KEY_SIZE);
            IsGettingKey = false;
        } else {
            int toget = (BytesLeft < length) ? BytesLeft : length;
            memmove(&Buffer[Counter - BytesLeft], source, toget);
            length -= toget;
            BytesLeft -= toget;
            source = (char const *)source + toget;

            if (BytesLeft == 0) {
                char buffer[MAX_KEY_BLOCK_SIZE];
                CipherKey->Decrypt(Buffer, Counter, buffer);
                BF.Key(buffer, BLOWFISH_KEY_SIZE);
                IsGettingKey = false;
            }
        }
    }

    total += Pipe::Put(source, length);
    return total;
}


/***********************************************************************************************
 * PKPipe::Encrypted_Key_Length -- Fetch the encrypted key length.                             *
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
// (0x0060FD10)
// Size of the Blowfish key once encrypted with the key (0 without a key).

int PKPipe::Encrypted_Key_Length() const
{
    if (CipherKey == 0) {
        return 0;
    }
    return CipherKey->Block_Count(BLOWFISH_KEY_SIZE) * CipherKey->Crypt_Block_Size();
}


/***********************************************************************************************
 * PKPipe::Plain_Key_Length -- Returns the number of bytes to encrypt key.                     *
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
// (0x0060FD40)
// Size of the Blowfish key padded to whole plaintext blocks of the key
// (0 without a key).

int PKPipe::Plain_Key_Length() const
{
    if (CipherKey == 0) {
        return 0;
    }
    return CipherKey->Block_Count(BLOWFISH_KEY_SIZE) * CipherKey->Plain_Block_Size();
}


// BlowPipe scalar deleting destructor (0x0060FD70, BlowPipe virtual slot 0)
// Compiler generated in PKPipe.cpp, which embeds a BlowPipe; expands the
// inline BlowPipe destructor.


// PKPipe::Put_To(Pipe &pipe) (0x0060FDC0, virtual slot 5)
// Inline in the class declaration; emitted with the PKPipe vtable.


// PKPipe scalar deleting destructor (0x0060FDD0, virtual slot 0)
// Compiler generated; destroys the embedded BlowPipe.
