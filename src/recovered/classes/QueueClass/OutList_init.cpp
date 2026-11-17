// Dynamic initializer of OutList (0x004D46D0, _$E; Tiberian Sun's globals.cpp): the
// queue's inline constructor builds its 128 empty events and then clears Count, Head
// and Tail (no destructor, so no atexit).
#include "shared/event.hpp"

QueueClass<EventClass, 128> OutList;
