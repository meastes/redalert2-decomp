// pkstraw.cpp: the original translation unit re-filed as one source (Route A): the accepted
// bodies of its canonical compile group in address order, under the unit's own header
// state (its switches and includes in their original order). The module map
// manifests/original_target/modules/pkstraw.json carries the addresses and the rows that
// stay in their own files.


#include "shared/pk_straw.hpp"
#include "shared/crt_string.hpp"


// PKStraw::PKStraw(CryptControl control, RandomStraw &rnd) (0x0060FE30)
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


// PKStraw::Get_From(Straw *straw) (0x0060FEA0, virtual slot 1)
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


// PKStraw::Key(PKey const *key) (0x0060FEF0)
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


// PKStraw::Get(void *buffer, int length) (0x0060FF10, virtual slot 2)
// Before the first data: decrypting, reads the encrypted Blowfish key,
// decrypts it with the private key and keys the Blowfish stage;
// encrypting, makes a random Blowfish key, encrypts it with the public key
// for output and keys the Blowfish stage. The encrypted key is handed out
// first, then data drawn through the Blowfish stage. Returns the bytes
// delivered (0 if the encrypted key could not be read).

int PKStraw::Get(void *buffer, int length)
{
    if (buffer == 0 || length < 1 || CipherKey == 0) {
        return Straw::Get(buffer, length);
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
        memmove(buffer, &Buffer[Counter - BytesLeft], count);
        buffer = (char *)buffer + count;
        BytesLeft -= count;
        length -= count;
        total += count;
    }

    total += Straw::Get(buffer, length);
    return total;
}


// PKStraw::Encrypted_Key_Length() const (0x006100E0)
// Size of the Blowfish key once encrypted with the key (0 without a key).

int PKStraw::Encrypted_Key_Length() const
{
    if (CipherKey == 0) {
        return 0;
    }
    return CipherKey->Block_Count(BLOWFISH_KEY_SIZE) * CipherKey->Crypt_Block_Size();
}


// PKStraw::Plain_Key_Length() const (0x00610110)
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
