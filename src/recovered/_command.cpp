// _command.cpp: the original translation unit re-filed as one source (Route A): the accepted
// bodies of its canonical compile group in address order, under the unit's own header
// state (its switches and includes in their original order). The module map
// manifests/original_target/modules/_command.json carries the addresses and the rows that
// stay in their own files.


// The command unit's globals: the hotkey index (initializer 0x0040BC80 zeroes
// unknown_hotkeys_832380 (0x00832380) and registers its atexit destructor
// 0x0040BCB0) and the registered commands (initializer 0x0040BCE0, atexit
// 0x0040BD20; vtables 0x0079A81C/0x0079A83C, RTTI DynamicVectorClass<CommandClass const *>).
// The list's template members are this unit's copies at 0x0040BD60-0x0040BFF0 (accepted
// as standalone members); its deleting destructors 0x0040C020/0x0040C070 are claimed here.
#ifndef VECTOR_IMPLEMENT_ALL
#define VECTOR_IMPLEMENT_ALL
#endif
#include "shared/command.hpp"

IndexClass<int, CommandClass *> unknown_hotkeys_832380;
DynamicVectorClass<CommandClass const *> unknown_commands_832358;


// atexit destructor of unknown_hotkeys_832380 (0x0040BCB0, _$E): emitted by
// the definition in unknown_hotkeys_832380_init_0040bc80.cpp; no source of its own.
