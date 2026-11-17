// Dynamic initializer of a global UnknownObjectPair (0x004D4620, _$E): the inline
// constructor clears both pointers, then the atexit destructor (0x004D4640, the following
// _$E draft, emitted by this definition) runs ~UnknownObjectPair 0x005D4A60. Owning unit:
// the global-object block around 0x004D4500.
#include "shared/object_type.hpp"

UnknownObjectPair DropPodVoxel;
