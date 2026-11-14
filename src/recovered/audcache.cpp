// audcache.cpp: the original translation unit re-filed as one source (Route A): the accepted
// bodies of its canonical compile group in address order, under the unit's own header
// state (its switches and includes in their original order). The module map
// manifests/original_target/modules/audcache.json carries the addresses and the rows that
// stay in their own files.


#include "shared/crt_memory.hpp"
#include "shared/audio.hpp"


// AudioBank *__fastcall Audio_Bank_Create(AudioBag *bag, int bytes, int entries, int block) -- 0x004019E0 (ret 8)
// Purpose: makes a bank with a pool of bytes / (block + 0x1C) pieces of
// block + 0x1C bytes and one of entries entries of 0x64 bytes. Destroys it
// again (0x00401B00, expanded inline) and returns 0 when a pool cannot be
// made.

void *__cdecl operator new(unsigned int size);
void __cdecl operator delete(void *block);

AudioBank *__fastcall Audio_Bank_Create(AudioBag *bag, int bytes, int entries, int block)
{
    AudioBank *bank = (AudioBank *)operator new(sizeof(AudioBank));
    memset(bank, 0, sizeof(AudioBank));
    bank->Bag = bag;
    bank->BlockSize = block;
    bank->Pieces = Audio_Pool_Create(bytes / (block + 0x1c), block + 0x1c);
    bank->Entries = Audio_Pool_Create(entries, 100);
    Audio_List_Init(&bank->List);
    if (bank->Pieces != 0 && bank->Entries != 0) {
        goto done;
    }
    Audio_Bank_Destroy(bank);
    bank = 0;
done:
    return bank;
}


// 0x00401fb0: function not yet identified
//
// Body: 4 bytes (frozen inventory bound).
// Referenced from 0 sites.
// Ghidra prototype: undefined4 __fastcall FUN_00401fb0(int param_1)
//
// Types and names are Ghidra placeholders;
// rename and retype as the purpose becomes known.
//

namespace seed_00401fb0 {

unsigned int __fastcall FUN_00401fb0(int object_address)
{
    return *(unsigned int *)(object_address + 0xc);
}

}  // namespace seed_00401fb0


// void __fastcall Audio_Bank_Destroy(AudioBank *bank) -- 0x00401B00
// Purpose: destroys every entry of the bank (0x00401FC0, expanded inline),
// then its pools and the bank.

void __cdecl operator delete(void *block);

void __fastcall Audio_Bank_Destroy(AudioBank *bank)
{
    AudioBankEntry *entry;
    while ((entry = (AudioBankEntry *)Audio_List_Next_Unchecked(&bank->List)) != 0) {
        Audio_Bank_Entry_Destroy(entry);
    }
    if (bank->Pieces != 0) {
        Audio_Pool_Destroy(bank->Pieces);
    }
    if (bank->Entries != 0) {
        Audio_Pool_Destroy(bank->Entries);
    }
    operator delete(bank);
}


// AudioBankEntry *__fastcall Audio_Bank_Find(AudioBank *bank, int id) -- 0x00401BB0
// Purpose: returns the bank's entry for the sound id that holds data, or 0.

AudioBankEntry *__fastcall Audio_Bank_Find(AudioBank *bank, int id)
{
    for (AudioBankEntry *entry = (AudioBankEntry *)bank->List.Next; entry != (AudioBankEntry *)&bank->List;
         entry = (AudioBankEntry *)entry->Next) {
        if (entry->Id == id && entry->Data != 0) {
            return entry;
        }
    }
    return 0;
}


// 0x00401be0: function not yet identified
//
// Body: 22 bytes (frozen inventory bound).
// Referenced from 0 sites.
// Ghidra prototype: void __fastcall FUN_00401be0(int param_1)
//
// Types and names are Ghidra placeholders;
// rename and retype as the purpose becomes known.
//

namespace seed_00401be0 {

void __fastcall FUN_00401be0(int bank_address)
{
    unsigned int *entry_words;

    for (entry_words = *(unsigned int **)(bank_address + 0xc); entry_words != (unsigned int *)(bank_address + 0xc);
            entry_words = (unsigned int *)*entry_words) {
        entry_words[6] = 0;
    }
    return;
}

}  // namespace seed_00401be0


// AudioBankEntry *Audio_Bank_Load(AudioBank *bank, int id) (0x00401C00)
// Returns the bank's entry for sound id, reading it from the bag when it is
// not cached: moves a cached entry to the head of the list; otherwise opens
// the sound in the bag, takes an entry (evicting the oldest unused one when
// the pool is empty) and reads the sound into pieces of the bank's block
// size, evicting entries while the piece pool is empty. Returns 0 when the
// sound cannot be opened or read.

AudioBankEntry *__fastcall Audio_Bank_Load(AudioBank *bank, int id)
{
    AudioBankEntry *entry = Audio_Bank_Find(bank, id);
    if (entry != 0) {
        Audio_List_Remove(entry);
        Audio_List_Add_Head(&bank->List, entry);
        return entry;
    }
    if (Audio_Bag_Open(bank->Bag, id)) {
        entry = (AudioBankEntry *)Audio_Pool_Alloc(bank->Entries);
        if (entry == 0) {
            Audio_Bank_Evict(bank);
            entry = (AudioBankEntry *)Audio_Pool_Alloc(bank->Entries);
            if (entry == 0) {
                Audio_Bag_Close(bank->Bag);
                return 0;
            }
        }
        entry->Id = id;
        entry->Owner = bank;
        Audio_List_Init_Node(entry);
        entry->unknown_0c = 0;
        Audio_Source_Reset(&entry->Source);
        Audio_Format_Init(&entry->Format);
        entry->Source.Format = &entry->Format;
        int remaining = bank->Bag->Remaining;
        for (;;) {
            if (remaining == 0) {
                Audio_Bag_Get_Format(bank->Bag, entry->Id, &entry->Format);
                Audio_List_Add_Head(&bank->List, entry);
                entry->Data = (void *)1;
                Audio_Bag_Close(bank->Bag);
                return entry;
            }
            int size = bank->BlockSize;
            if (size > remaining) {
                size = remaining;
            }
            AudioSourceInfo *piece = (AudioSourceInfo *)Audio_Pool_Alloc(bank->Pieces);
            while (piece == 0) {
                if (!Audio_Bank_Evict(bank)) {
                    goto failed;
                }
                piece = (AudioSourceInfo *)Audio_Pool_Alloc(bank->Pieces);
            }
            piece->Init((char *)piece + 0x1c, size);
            Audio_Source_Add_Piece(&entry->Source, piece);
            int read = Audio_Bag_Read(bank->Bag, (char *)piece + 0x1c, size);
            if (read != size) {
                break;
            }
            remaining -= read;
        }
failed:
        Audio_Bank_Entry_Destroy(entry);
    }
    return 0;
}


// int __fastcall Audio_Bank_Evict(AudioBank *bank) -- 0x00401EF0
// Purpose: destroys the last entry of the bank that is not in use
// (0x00401FC0, expanded inline). Returns 1, or 0 when every entry is in use.

int __fastcall Audio_Bank_Evict(AudioBank *bank)
{
    AudioBankEntry *entry = (AudioBankEntry *)Audio_List_Last(&bank->List);
    while (entry != 0) {
        if (entry->unknown_0c == 0) {
            Audio_Bank_Entry_Destroy(entry);
            return 1;
        }
        entry = (AudioBankEntry *)Audio_List_Last(entry);
    }
    return 0;
}


// void __fastcall AudioCountedList::Enter() -- 0x00401F80
// Purpose: counts one more user changing the list.

void __fastcall AudioCountedList::Enter()
{
    Busy++;
}


// void __fastcall AudioCountedList::Leave() -- 0x00401F90
// Purpose: counts one user less, never going below 0.

void __fastcall AudioCountedList::Leave()
{
    if (--Busy < 0) {
        Busy = 0;
    }
}


// void __fastcall Audio_Bank_Entry_Destroy(AudioBankEntry *entry) -- 0x00401FC0
// Purpose: unlinks the entry from its bank, gives its pieces back to the
// bank's piece pool and the entry to the entry pool.

void __fastcall Audio_Bank_Entry_Destroy(AudioBankEntry *entry)
{
    if (entry->Prev != entry) {
        Audio_List_Remove(entry);
    }
    AudioSourceInfo *piece;
    while ((piece = (AudioSourceInfo *)Audio_List_Next_Unchecked(&entry->Source.Pieces)) != 0) {
        Audio_List_Remove(piece);
        piece->Detach();
        Audio_Pool_Free(entry->Owner->Pieces, piece);
    }
    entry->Source.Finish();
    Audio_Pool_Free(entry->Owner->Entries, entry);
}


// 0x00402020: function not yet identified
//
// Body: 4 bytes (frozen inventory bound).
// Referenced from 0 sites.
// Ghidra prototype: undefined4 __fastcall FUN_00402020(int param_1)
//
// Types and names are Ghidra placeholders;
// rename and retype as the purpose becomes known.
//

namespace seed_00402020 {

unsigned int __fastcall FUN_00402020(int object_address)
{
    return *(unsigned int *)(object_address + 0x14);
}

}  // namespace seed_00402020


// AudioSource *AudioBankEntry::Get_Source() (0x00402030): the entry's sound (+0x1C).

AudioSource *__fastcall AudioBankEntry::Get_Source()
{
    return &Source;
}
