// pkpipe.cpp: the original translation unit re-filed as one source (Route A): the accepted
// bodies of its canonical compile group in address order, under the unit's own header
// state (its switches and includes in their original order). The module map
// manifests/original_target/modules/pkpipe.json carries the addresses and the rows that
// stay in their own files.


#include "shared/pk_pipe.hpp"
#include "shared/crt_string.hpp"


// PKPipe::PKPipe(CryptControl control, RandomStraw &rnd) (0x0060FA90)
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


// PKPipe::Put_To(Pipe *pipe) (0x0060FAF0, virtual slot 3)
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


// PKPipe::Key(PKey const *key) (0x0060FB40)
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


// PKPipe::Put(void const *source, int length) (0x0060FBA0, virtual slot 4)
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


// PKPipe::Encrypted_Key_Length() const (0x0060FD10)
// Size of the Blowfish key once encrypted with the key (0 without a key).

int PKPipe::Encrypted_Key_Length() const
{
    if (CipherKey == 0) {
        return 0;
    }
    return CipherKey->Block_Count(BLOWFISH_KEY_SIZE) * CipherKey->Crypt_Block_Size();
}


// PKPipe::Plain_Key_Length() const (0x0060FD40)
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
