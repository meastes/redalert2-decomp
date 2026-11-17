// Dynamic initializer of the 256-entry event queue 0x00A35F30 (0x004D4730, _$E): 256
// empty events, then Count, Head and Tail cleared.
#include "shared/event.hpp"

QueueClass<EventClass, 256> MegaMissionQueue;
