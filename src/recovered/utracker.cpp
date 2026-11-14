// utracker.cpp: the original translation unit re-filed as one source (Route A): the accepted
// bodies of its canonical compile group in address order, under the unit's own header
// state (its switches and includes in their original order). The module map
// manifests/original_target/modules/utracker.json carries the addresses and the rows that
// stay in their own files.


#include "shared/unit_tracker.hpp"


// UnitTrackerClass::UnitTrackerClass() -- 0x0070C440

UnitTrackerClass::UnitTrackerClass()
{
    UnitCount = 512;
    Clear_Unit_Total();
    InNetworkFormat = 0;
}


// UnitTrackerClass::~UnitTrackerClass() -- 0x0070C480

UnitTrackerClass::~UnitTrackerClass()
{
}


// void UnitTrackerClass::Increment_Unit_Total(int unit_type) -- 0x0070C490

void UnitTrackerClass::Increment_Unit_Total(int unit_type)
{
    if (unit_type < UnitCount) {
        UnitTotals[unit_type]++;
    }
}


// void UnitTrackerClass::Decrement_Unit_Total(int unit_type) -- 0x0070C4B0

void UnitTrackerClass::Decrement_Unit_Total(int unit_type)
{
    if (unit_type < UnitCount) {
        UnitTotals[unit_type]--;
    }
}


// void UnitTrackerClass::Set_Unit_Count(int count) -- 0x0070C4D0
// Clears every entry, then limits the tracked types to count.

void UnitTrackerClass::Set_Unit_Count(int count)
{
    UnitCount = 512;
    Clear_Unit_Total();
    UnitCount = count;
}


// int UnitTrackerClass::Get_All_Totals_Sum() -- 0x0070C510

int UnitTrackerClass::Get_All_Totals_Sum()
{
    int total = 0;
    for (int index = 0; index < UnitCount; index++) {
        total += UnitTotals[index];
    }
    return total;
}


// int *UnitTrackerClass::Get_All_Totals() -- 0x0070C530

int *UnitTrackerClass::Get_All_Totals()
{
    return UnitTotals;
}


// void UnitTrackerClass::Clear_Unit_Total() -- 0x0070C540

void UnitTrackerClass::Clear_Unit_Total()
{
    for (int index = 0; index < UnitCount; index++) {
        UnitTotals[index] = 0;
    }
}


// void UnitTrackerClass::To_Network_Format() -- 0x0070C570
// Byte-swaps every count to network order once.

extern "C" unsigned long __stdcall htonl(unsigned long value);

void UnitTrackerClass::To_Network_Format()
{
    if (!InNetworkFormat) {
        for (int index = 0; index < UnitCount; index++) {
            UnitTotals[index] = htonl(UnitTotals[index]);
        }
    }
    InNetworkFormat = 1;
}


// void UnitTrackerClass::To_PC_Format() -- 0x0070C5C0
// Byte-swaps every count back to host order once.

extern "C" unsigned long __stdcall ntohl(unsigned long value);

void UnitTrackerClass::To_PC_Format()
{
    if (InNetworkFormat) {
        for (int index = 0; index < UnitCount; index++) {
            UnitTotals[index] = ntohl(UnitTotals[index]);
        }
    }
    InNetworkFormat = 0;
}
