// counter.cpp: the original translation unit re-filed as one source (Route A): the accepted
// bodies of its canonical compile group in address order, under the unit's own header
// state (its switches and includes in their original order). The module map
// manifests/original_target/modules/counter.json carries the addresses and the rows that
// stay in their own files.


#define VECTOR_IMPLEMENT_VECTOR_CONSTRUCTOR
#define VECTOR_IMPLEMENT_VECTOR_DESTRUCTOR
#include "shared/counter_class.hpp"


CounterClass::CounterClass() : VectorClass<int>() {}


// Emitted by the shared vector template and CounterClass definition.


int CounterClass::Increment(int index) { if (Ensure_Index(index)) { ++Vector[index]; return Vector[index]; } return 0; }


int CounterClass::Decrement(int index) { if (Ensure_Index(index)) { --Vector[index]; return Vector[index]; } return 0; }


int CounterClass::Get(int index) { if (Ensure_Index(index)) return Vector[index]; return 0; }


void CounterClass::Clear() { for(int i=0;i<VectorMax;++i) Vector[i]=0; }


int CounterClass::Sum() const { int total=0;for(int i=0;i<VectorMax;++i) total+=Vector[i];return total; }


HRESULT CounterClass::Save(IStream *stream) {
 int count=VectorMax;
 HRESULT result=stream->Write(&count,4,0);
 if (FAILED(result)) return result;
 for(int i=0;i<count;++i) { result=stream->Write(&Vector[i],4,0);if (FAILED(result)) return result; }
 return 0;
}


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
