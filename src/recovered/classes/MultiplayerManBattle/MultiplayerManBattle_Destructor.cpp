// void __fastcall MultiplayerManBattle_Destructor_005a74f0(unsigned int *object_words) (0x005A74F0): stores
// the MultiplayerManBattle vtable and runs the MultiplayerGameMode destructor (0x005B6E20).

namespace seed_005a74f0 {

extern void __fastcall MultiplayerGameMode_Destructor_005b6e20(void);
extern "C" void* MultiplayerManBattle_VTable_007a69dc;

void __fastcall MultiplayerManBattle_Destructor_005a74f0(unsigned int *object_words)
{
    *object_words = (unsigned int)(&MultiplayerManBattle_VTable_007a69dc);
    MultiplayerGameMode_Destructor_005b6e20();
    return;
}

}  // namespace seed_005a74f0
