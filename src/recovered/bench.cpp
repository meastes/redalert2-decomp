// bench.cpp: the original translation unit re-filed as one source (Route A): the accepted
// bodies of its canonical compile group in address order, under the unit's own header
// state (its switches and includes in their original order). The module map
// manifests/original_target/modules/bench.json carries the addresses and the rows that
// stay in their own files.


#include "shared/shape_file.hpp"
#include "shared/bench.hpp"


// Header-static dynamic initializer of the unit at 0x0042F0A0 (the shape_file.hpp empty rect,
// 0x0084E998); the other drafts of this plan are the unit's Benchmark members.


// Benchmark::Benchmark (0x0042F0C0): the clock starts now, the statistics empty (Westwood's Bench.cpp).

Benchmark::Benchmark() :
    Average(0),
    Counter(0),
    TotalCount(0)
{
}


// Benchmark::Reset (0x0042F0F0): forgets the events.

void Benchmark::Reset()
{
    Average = 0;
    Counter = 0;
    TotalCount = 0;
}


// Benchmark::Begin (0x0042F100): restarts the clock, clearing the statistics first when asked (Westwood's
// Bench.cpp).

void Benchmark::Begin(bool reset)
{
    if (reset) Reset();
    Clock = 0;
}


// Benchmark::End (0x0042F140): adds the interval since Begin; once MAXIMUM_EVENT_COUNT (256) events are
// averaged, the oldest share decays instead of the count growing (Westwood's Bench.cpp).

void Benchmark::End()
{
    unsigned long value = Clock;
    if (Counter == 256) {
        Average -= Average / 256;
        Average += value;
    } else {
        Average += value;
        Counter++;
    }
    TotalCount++;
}


// Benchmark::Step (0x0042F1A0): adds the interval since Begin as one event; the event count.

unsigned long Benchmark::Step()
{
    unsigned long value = Clock;
    Average += value;
    Counter++;
    TotalCount++;
    return TotalCount;
}


// Benchmark::Value (0x0042F1E0): the average interval, 0 without events.

unsigned long Benchmark::Value() const
{
    if (Counter) {
        return Average / Counter;
    }
    return 0;
}
