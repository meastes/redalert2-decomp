// blowfish.cpp: the original translation unit re-filed as one source (Route A): the accepted
// bodies of its canonical compile group in address order, under the unit's own header
// state (its switches and includes in their original order). The module map
// manifests/original_target/modules/blowfish.json carries the addresses and the rows that
// stay in their own files.


#include "shared/blowfish_engine.hpp"


// BlowfishEngine::BlowfishEngine() (0x00435580)
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


// BlowfishEngine::~BlowfishEngine() (0x004355F0)
// Releases the COM interface.

BlowfishEngine::~BlowfishEngine()
{
    if (Interface != 0) {
        Interface->Release();
    }
}


// BlowfishEngine::Submit_Key(void const *key, int length) (0x00435600)
// Passes the cipher key to the server.

void BlowfishEngine::Submit_Key(void const *key, int length)
{
    Server()->Submit_Key(length, (unsigned char const *)key);
}


// BlowfishEngine::Encrypt(void const *plaintext, int length, void *cyphertext) (0x00435630)
// Encrypts length bytes of whole blocks; returns length.

int BlowfishEngine::Encrypt(void const *plaintext, int length, void *cyphertext)
{
    Server()->Encrypt(length, (unsigned char const *)plaintext, (unsigned char *)cyphertext);
    return length;
}


// BlowfishEngine::Decrypt(void const *cyphertext, int length, void *plaintext) (0x00435660)
// Decrypts length bytes of whole blocks; returns length.

int BlowfishEngine::Decrypt(void const *cyphertext, int length, void *plaintext)
{
    Server()->Decrypt(length, (unsigned char const *)cyphertext, (unsigned char *)plaintext);
    return length;
}
