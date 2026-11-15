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
// Function comment blocks from mischa85/RedAlert2 392e904 src/blowfish.cpp (matching RA2 1.006 decomp), placed above our
// own notes; code unchanged.

// blowfish.cpp: the original translation unit re-filed as one source (Route A): the accepted
// bodies of its canonical compile group in address order, under the unit's own header
// state (its switches and includes in their original order). The module map
// manifests/original_target/modules/blowfish.json carries the addresses and the rows that
// stay in their own files.


#include "shared/blowfish_engine.hpp"


/// <summary>
/// Constructor for the Blowfish engine.
/// This will construct the engine in an unkeyed state. No data can be processed until a
/// key has been submitted to it.
/// </summary>
/// <remarks>You must submit the key before calling the encrypt or decrypt routines.</remarks>
// (0x00435580)
// Starts the Blowfish COM server and obtains its interface; failures other
// than a missing interface are raised as COM errors.

BlowfishEngine::BlowfishEngine() :
    Interface(0)
{
    IUnknown *unknown;
    HRESULT result = CoCreateInstance(CLSID_Blowfish, 0, CLSCTX_ALL, IID_IUnknown, (void **)&unknown);
    if (SUCCEEDED(result)) {
        result = OleRun(unknown);
        if (SUCCEEDED(result)) {
            result = unknown->QueryInterface(IID_IBlowfish, (void **)&Interface);
        }
        unknown->Release();
    }
    if (FAILED(result) && result != E_NOINTERFACE) {
        _com_issue_error(result);
    }
}


/***********************************************************************************************
 * BlowfishEngine::~BlowfishEngine -- Destructor for the Blowfish engine.                      *
 *                                                                                             *
 *    This destructor will clear out the s-box tables so that even if the memory for the       *
 *    class remains, it will contain no compromising data.                                     *
 *                                                                                             *
 * INPUT:   none                                                                               *
 *                                                                                             *
 * OUTPUT:  none                                                                               *
 *                                                                                             *
 * WARNINGS:   none                                                                            *
 *                                                                                             *
 * HISTORY:                                                                                    *
 *   07/08/1996 JLB : Created.                                                                 *
 *=============================================================================================*/
// (0x004355F0)
// Releases the COM interface.

BlowfishEngine::~BlowfishEngine()
{
    if (Interface != 0) {
        Interface->Release();
    }
}


/***********************************************************************************************
 * BlowfishEngine::Submit_Key -- Submit a key that will allow data processing.                 *
 *                                                                                             *
 *    This routine must be called before any data can be encrypted or decrypted. This routine  *
 *    need only be called when the key is to be changed or set for the first time. Once the    *
 *    key has been set, the engine may be used to encrypt, decrypt, or both operations         *
 *    indefinitely. The key must be 56 bytes or less in length. This is necessary because      *
 *    any keys longer than that will not correctly affect the encryption process.              *
 *                                                                                             *
 *    If the key pointer is NULL, then the S-Box tables are reset to identity. This will       *
 *    mask the previous key setting. Use this method to clear the engine after processing in   *
 *    order to gain a measure of security.                                                     *
 *                                                                                             *
 * INPUT:   key      -- Pointer to the key data block.                                         *
 *                                                                                             *
 *          length   -- The length of the submitted key.                                       *
 *                                                                                             *
 * OUTPUT:  none                                                                               *
 *                                                                                             *
 * WARNINGS:   This is a time consuming process.                                               *
 *                                                                                             *
 * HISTORY:                                                                                    *
 *   04/14/1996 JLB : Created.                                                                 *
 *=============================================================================================*/
// (0x00435600)
// Passes the cipher key to the server.

void BlowfishEngine::Submit_Key(void const *key, int length)
{
    Server()->Submit_Key(length, (unsigned char const *)key);
}


/***********************************************************************************************
 * BlowfishEngine::Encrypt -- Encrypt an arbitrary block of data.                              *
 *                                                                                             *
 *    Use this routine to encrypt an arbitrary block of data. The block must be an even        *
 *    multiple of 8 bytes. Any bytes left over will not be encrypted. The 8 byte requirement   *
 *    is necessary because the underlying algorithm processes blocks in 8 byte chunks.         *
 *    Partial blocks are unrecoverable and useless.                                            *
 *                                                                                             *
 * INPUT:   plaintext-- Pointer to the data block to be encrypted.                             *
 *                                                                                             *
 *          length   -- The length of the data block.                                          *
 *                                                                                             *
 *          cyphertext- Pointer to the output buffer that will hold the encrypted data.        *
 *                                                                                             *
 * OUTPUT:  Returns with the actual number of bytes encrypted.                                 *
 *                                                                                             *
 * WARNINGS:   You must submit the key before calling this routine. This will only encrypt     *
 *             the plaintext in 8 byte increments. Modulo bytes left over are not processed.   *
 *                                                                                             *
 * HISTORY:                                                                                    *
 *   04/14/1996 JLB : Created.                                                                 *
 *=============================================================================================*/
// (0x00435630)
// Encrypts length bytes of whole blocks; returns length.

int BlowfishEngine::Encrypt(void const *plaintext, int length, void *cyphertext)
{
    Server()->Encrypt(length, (unsigned char const *)plaintext, (unsigned char *)cyphertext);
    return length;
}


/***********************************************************************************************
 * BlowfishEngine::Decrypt -- Decrypt an arbitrary block of data.                              *
 *                                                                                             *
 *    Use this routine to decrypt an arbitrary block of data. The block must be an even        *
 *    multiple of 8 bytes. Any bytes left over will not be decrypted. The 8 byte requirement   *
 *    is necessary because the underlying algorithm processes blocks in 8 byte chunks.         *
 *    Partial blocks are unrecoverable and useless.                                            *
 *                                                                                             *
 * INPUT:   cyphertext- Pointer to the data block to be decrypted.                             *
 *                                                                                             *
 *          length   -- The length of the data block.                                          *
 *                                                                                             *
 *          plaintext-- Pointer to the output buffer that will hold the decrypted data.        *
 *                                                                                             *
 * OUTPUT:  Returns with the actual number of bytes decrypted.                                 *
 *                                                                                             *
 * WARNINGS:   You must submit the key before calling this routine. This will only decrypt     *
 *             the cyphertext in 8 byte increments. Modulo bytes left over are not processed.  *
 *                                                                                             *
 * HISTORY:                                                                                    *
 *   04/14/1996 JLB : Created.                                                                 *
 *=============================================================================================*/
// (0x00435660)
// Decrypts length bytes of whole blocks; returns length.

int BlowfishEngine::Decrypt(void const *cyphertext, int length, void *plaintext)
{
    Server()->Decrypt(length, (unsigned char const *)cyphertext, (unsigned char *)plaintext);
    return length;
}
