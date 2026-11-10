// Native M25 link closure row (Scout1202). NOT EXACT: behaviour from the seed.

// 0x00770330 WideString::Release (seed src/recovered/00770330.cpp: frees the text and
// clears the pointer, as the destructor 0x0076FA50 does). Called by the wide-text
// readers 0x00600E50/0x00600F10/0x00600FD0 when a control has no text. Declared there
// on a local two-member view of the class (Buffer at +0x00), declared the same way here.
class WideString {
public:
    void Release();
    unsigned short *Buffer;
};

void WideString::Release()
{
    if (Buffer != 0) {
        delete Buffer;
    }
    Buffer = 0;
}

