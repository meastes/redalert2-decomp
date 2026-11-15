/*******************************************************************************
 *                                O P E N  T S
 *******************************************************************************
 * SPDX-License-Identifier: GPL-3.0-or-later
 * Copyright 2025 Electronic Arts Inc.
 * Copyright 2026 OpenTS contributors
 *
 * Contains material derived from Electronic Arts source code.
 * Modified by OpenTS contributors, 2026.
 * EA's GPLv3 Section 7 additional terms and supplemental warranty
 * disclaimers apply; see LICENSE.md.
 ******************************************************************************/
// Function comment blocks from mischa85/RedAlert2 392e904 src/counter.cpp (matching RA2 1.006 decomp), placed above our
// own notes; code unchanged.

// counter.cpp: the original translation unit re-filed as one source (Route A): the accepted
// bodies of its canonical compile group in address order, under the unit's own header
// state (its switches and includes in their original order). The module map
// manifests/original_target/modules/counter.json carries the addresses and the rows that
// stay in their own files.


#define VECTOR_IMPLEMENT_VECTOR_CONSTRUCTOR
#define VECTOR_IMPLEMENT_VECTOR_DESTRUCTOR
#include "shared/counter_class.hpp"


/// <summary>
/// Constructor for the counter object.
/// The counter starts out empty and sizes itself as counters are tallied, so the
/// houses may keep a running total per object type without knowing how many types
/// there will be.
/// </summary>
CounterClass::CounterClass() : VectorClass<int>() {}


// Emitted by the shared vector template and CounterClass definition.


int CounterClass::Increment(int index) { if (Ensure_Index(index)) { ++Vector[index]; return Vector[index]; } return 0; }


int CounterClass::Decrement(int index) { if (Ensure_Index(index)) { --Vector[index]; return Vector[index]; } return 0; }


int CounterClass::Get(int index) { if (Ensure_Index(index)) return Vector[index]; return 0; }


void CounterClass::Clear() { for(int i=0;i<VectorMax;++i) Vector[i]=0; }


int CounterClass::Sum() const { int total=0;for(int i=0;i<VectorMax;++i) total+=Vector[i];return total; }


/// <summary>
/// Saves the counters to the save game stream.
/// </summary>
/// <returns>Returns with S_OK if the counters were written, otherwise with the error
/// code.</returns>
HRESULT CounterClass::Save(IStream *stream) {
 int count=VectorMax;
 HRESULT result=stream->Write(&count,4,0);
 if (FAILED(result)) return result;
 for(int i=0;i<count;++i) { result=stream->Write(&Vector[i],4,0);if (FAILED(result)) return result; }
 return 0;
}


/// <summary>
/// Loads the counters from the save game stream.
/// This routine will reconstruct the counter object before reading, so whatever it
/// was tallying beforehand is discarded.
/// </summary>
/// <returns>Returns with S_OK if the counters were read, otherwise with the error code.</returns>
HRESULT CounterClass::Load(IStream *stream) {
 new(this) CounterClass;
 int count;
 HRESULT result=stream->Read(&count,4,0);
 if (FAILED(result)) return result;
 if (!Resize(count)) return 8;
 for(int i=0;i<count;++i) { result=stream->Read(&Vector[i],4,0);if(FAILED(result))return result; }
 return 0;
}


bool CounterClass::Ensure_Index(int index) {
 int old_size=VectorMax;
 if(index<old_size) return true;
 if(Resize(index+10)) {
  for(int i=old_size;i<VectorMax;++i) Vector[i]=0;
  return true;
 }
 return false;
}


// Emitted by the shared vector template and CounterClass definition.
template class VectorClass<int>;
