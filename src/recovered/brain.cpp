/*******************************************************************************
 *                                O P E N  T S
 *******************************************************************************
 * SPDX-License-Identifier: GPL-3.0-or-later
 * Copyright 2026 OpenTS contributors
 *
 * See LICENSE.md for applicable additional terms and warranty disclaimers.
 ******************************************************************************/
// Function comment blocks from mischa85/RedAlert2 392e904 src/brain.cpp (matching RA2 1.006 decomp), placed above our
// own notes; code unchanged.

// brain.cpp: the original translation unit re-filed as one source (Route A): the accepted
// bodies of its canonical compile group in address order, under the unit's own header
// state (its switches and includes in their original order). The module map
// manifests/original_target/modules/brain.json carries the addresses and the rows that
// stay in their own files.


// Header-static dynamic initializers of the unit at 0x00437740:  the
// shared/unknown_geometry.hpp constants (0x00437740-0x00437901); the other
// drafts of this plan are emitted by these includes.
#include "shared/unknown_geometry.hpp"


// Dynamic initializer (0x00437770, _$E) of b from the header(s) included
// by HeaderStatics_diagonal_init_00437740.cpp; no source of its own.


// Dynamic initializer (0x00437790, _$E) of c from the header(s) included
// by HeaderStatics_diagonal_init_00437740.cpp; no source of its own.


// Dynamic initializer (0x004377B0, _$E) of angle45 from the header(s) included
// by HeaderStatics_diagonal_init_00437740.cpp; no source of its own.


// Dynamic initializer (0x004377D0, _$E) of angle60 from the header(s) included
// by HeaderStatics_diagonal_init_00437740.cpp; no source of its own.


// Dynamic initializer (0x004377F0, _$E) of angle90 from the header(s) included
// by HeaderStatics_diagonal_init_00437740.cpp; no source of its own.


// Dynamic initializer (0x00437810, _$E) of g from the header(s) included
// by HeaderStatics_diagonal_init_00437740.cpp; no source of its own.


// Dynamic initializer (0x00437850, _$E) of h from the header(s) included
// by HeaderStatics_diagonal_init_00437740.cpp; no source of its own.


// Dynamic initializer (0x00437880, _$E) of i from the header(s) included
// by HeaderStatics_diagonal_init_00437740.cpp; no source of its own.


// Dynamic initializer (0x004378B0, _$E) of j from the header(s) included
// by HeaderStatics_diagonal_init_00437740.cpp; no source of its own.


// Dynamic initializer (0x004378E0, _$E) of k from the header(s) included
// by HeaderStatics_diagonal_init_00437740.cpp; no source of its own.


// Dynamic initializer (0x004378F0, _$E) of l from the header(s) included
// by HeaderStatics_diagonal_init_00437740.cpp; no source of its own.


/// <summary>
/// Creates a neuron.
/// The neuron notes the frame it came into being on and starts out unattached -- a brain
/// adopts it later. It registers itself with the object trackers so that it can be
/// detached cleanly when the things it refers to go away.
/// </summary>
// (0x00437910)
// Joins the list 0x00AC0C18 and the abstract list 0x00AC0CB0.
#ifndef VECTOR_IMPLEMENT_ALL
#define VECTOR_IMPLEMENT_ALL
#endif
#include "shared/vector_class.hpp"
#include "shared/neuron.hpp"
#include "shared/object_lists.hpp"
#include "shared/cd_timer.hpp"

NeuronClass::NeuronClass() :
    unknown_24(0),
    unknown_28(0),
    Brain(0),
    CreationFrame(Frame)
{
    TeamPtrTracker.Add(this);
    AbstractTypePtrTracker.Add(this);
}


/// <summary>
/// Removes this neuron from the object trackers.
/// </summary>
// (0x00437A00)
// Leaves both lists.

NeuronClass::~NeuronClass()
{
    AbstractTypePtrTracker.Delete(this);
    TeamPtrTracker.Delete(this);
}


/// <summary>
/// Fetches the class identifier of this object.
/// This routine is used by the save game system so that it knows what kind of object to
/// construct when the stream is read back in.
/// </summary>
/// <param name="retval">Pointer to the place to store the class identifier.</param>
/// <returns>Returns with S_OK, or E_POINTER if no destination was supplied.</returns>
// (0x00437AC0, slot 3)

HRESULT __stdcall NeuronClass::GetClassID(GUID *clsid)
{
    if (!clsid) {
        return E_POINTER;
    }
    *clsid = CLSID_NeuronClass;
    return S_OK;
}


/// <summary>
/// Loads this neuron from the save game stream.
/// The object is rebuilt in place once the base class has read the raw image back over
/// it, and the pointers it carries are handed to the swizzler so that they can be
/// remapped onto wherever those objects now live.
/// </summary>
/// <returns>Returns with S_OK if the neuron was read successfully.</returns>
// (0x00437B00, slot 5)

HRESULT __stdcall NeuronClass::Load(IStream *stream)
{
    HRESULT result = AbstractClass::Load(stream);
    if (FAILED(result)) {
        return result;
    }
    new (this) NeuronClass(NoInitClass());
    Swizzle_Pointer(unknown_28);
    Swizzle_Pointer(unknown_24);
    Swizzle_Pointer(Brain);
    return S_OK;
}


/// <summary>
/// Saves this neuron to the save game stream.
/// </summary>
/// <param name="cleardirty">Should the dirty flag be cleared once the object is written?</param>
/// <returns>Returns with S_OK if the neuron was written successfully.</returns>
// (0x00437B70, slot 6)

HRESULT __stdcall NeuronClass::Save(IStream *stream, BOOL clear_dirty)
{
    HRESULT result = AbstractClass::Save(stream, clear_dirty);
    if (SUCCEEDED(result)) {
        result = S_OK;
    }
    return result;
}


/// <summary>
/// Folds this neuron's state into a running CRC.
/// The multiplayer sync check accumulates every object's state each frame; a CRC that
/// differs between machines means the simulations have drifted apart. A neuron
/// contributes its age, since its pointers are meaningless across machines.
/// </summary>
/// <param name="crc">The running CRC to fold this neuron's state into.</param>
// (0x00437B90, slot 13)
// Adds the neuron's age in frames.

void NeuronClass::Compute_CRC(WWCRCEngine &crc) const
{
    AbstractClass::Compute_CRC(crc);
    crc((long)(Frame - CreationFrame));
}


/// <summary>
/// Destroys the brain along with every neuron it owns.
/// </summary>
// (0x00437BC0)
// Deletes its neurons.

BrainClass::~BrainClass()
{
    Clear();
}


// void BrainClass::Clear() (0x00437C30)
// Deletes the neurons and empties the list.

void BrainClass::Clear()
{
    int count = Neurons.Count();
    for (int i = 0; i < count; i++) {
        delete Neurons[i];
        Neurons[i] = 0;
    }
    Neurons.Clear();
}


/// <summary>
/// Prepares the brain to hold neurons.
/// Any neurons the brain was already holding are thrown away before the new limits take
/// effect, so this routine can be used to start a brain over from scratch.
/// </summary>
/// <param name="min">The fewest neurons this brain is meant to hold.</param>
/// <param name="max">The most neurons this brain will accept.</param>
// (0x00437C70)

void BrainClass::Init(int value, int max_neurons)
{
    Clear();
    unknown_1c = value;
    MaxNeurons = max_neurons;
}


// bool BrainClass::Add(NeuronClass *neuron) (0x00437CC0)
// Adds the neuron while the brain is below its cap.

bool BrainClass::Add(NeuronClass *neuron)
{
    if (Neurons.Count() < MaxNeurons) {
        Neurons.Add(neuron);
        neuron->Brain = this;
        return true;
    }
    return false;
}


/// <summary>
/// Saves this brain to the save game stream.
/// The number of neurons goes out first so that the loader knows how many to expect,
/// followed by each neuron in turn.
/// </summary>
/// <param name="cleardirty">Should the neurons be marked clean once they are written?</param>
/// <returns>
/// Returns with S_OK when the brain was written, E_POINTER when no stream was supplied,
/// or the first failure code a neuron reported.
/// </returns>
// (0x00437D30)
// The neuron count, then each neuron.

HRESULT BrainClass::Save(IStream *stream, BOOL clear_dirty)
{
    if (!stream) {
        return E_POINTER;
    }
    int count = Neurons.Count();
    HRESULT result = stream->Write(&count, sizeof(count), 0);
    if (FAILED(result)) {
        return result;
    }
    for (int i = 0; i < count; i++) {
        result = Neurons[i]->Save(stream, clear_dirty);
        if (FAILED(result)) {
            return result;
        }
    }
    return S_OK;
}


/// <summary>
/// Reads this brain back from the save game stream.
/// A neuron is created and read back for each one the stream says was written, and each
/// is handed to the brain as it arrives.
/// </summary>
/// <returns>
/// Returns with S_OK when the brain was read, E_POINTER when no stream was supplied,
/// E_OUTOFMEMORY when a neuron could not be created, or the stream's own failure code.
/// </returns>
// (0x00437DA0)
#include "shared/class_factory.hpp"

HRESULT BrainClass::Load(IStream *stream)
{
    if (!stream) {
        return E_POINTER;
    }
    int count = 0;
    HRESULT result = stream->Read(&count, sizeof(count), 0);
    if (FAILED(result)) {
        return result;
    }
    for (int i = 0; i < count; i++) {
        NeuronClass *neuron = new NeuronClass;
        if (!neuron) {
            return E_OUTOFMEMORY;
        }
        result = neuron->Load(stream);
        if (FAILED(result)) {
            return result;
        }
        Add(neuron);
    }
    return S_OK;
}


// int NeuronClass::Kind_Of() const (0x00437F60, slot 11)
// Inline in shared/neuron.hpp; emitted with the vtable. No source of its own.


// int NeuronClass::Size_Of() const (0x00437F70, slot 12)
// Inline in shared/neuron.hpp; emitted with the vtable. No source of its own.


// void *NeuronClass::`scalar deleting destructor'(unsigned int flags) (0x00437F80)
// Compiler-generated with the vtable (the destructor inlined). No source of its own.


// void *BrainClass::`scalar deleting destructor'(unsigned int flags) (0x00438050)
// Compiler-generated with the vtable (the destructor inlined). No source of its own.


// Container members this unit emits by its own use of its lists (bodies: shared/vector_class.hpp):
// VectorClass<NeuronClass *>::Fetch (0x004380E0; VectorClass<NeuronClass *> vtable 0x0079cb54 slot 6).
// VectorClass<NeuronClass *>::operator equal (0x004380F0; VectorClass<NeuronClass *> vtable 0x0079cb54 slot 1).
// VectorClass<NeuronClass *>::Resize (0x00438130; VectorClass<NeuronClass *> vtable 0x0079cb54 slot 2).
// VectorClass<NeuronClass *>::Clear (0x004381E0; VectorClass<NeuronClass *> vtable 0x0079cb54 slot 3).
// VectorClass<NeuronClass *>::ID_object (0x00438210; VectorClass<NeuronClass *> vtable 0x0079cb54 slot 4).
// VectorClass<NeuronClass *>::ID_pointer (0x00438250; VectorClass<NeuronClass *> vtable 0x0079cb54 slot 5).


extern "C" unsigned char vtable_438270[]; extern "C" void __cdecl release_buffer_438270(void*); extern "C" void __cdecl release_object_438270(void*);
class Leaf_438270 { public: void* get(unsigned int flags); };
void* Leaf_438270::get(unsigned int flags) { unsigned int pointer = *(unsigned int*)((char*)this + 4); *(unsigned int*)this=(unsigned int)vtable_438270; if(pointer && *(unsigned char*)((char*)this + 13)){ release_buffer_438270((void*)pointer); *(unsigned int*)((char*)this + 4)=0; } *(unsigned char*)((char*)this + 13)=0; *(unsigned int*)((char*)this + 8)=0; if(flags & 1) release_object_438270(this); return this; }
