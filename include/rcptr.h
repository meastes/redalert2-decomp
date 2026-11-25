// SPDX-License-Identifier: GPL-3.0-or-later
// Copyright 2026 ra2-decomp contributors
#pragma once

class rc_object
{
	public:
		virtual void vt_0(void) = 0;
		virtual void vt_1(void) = 0;
		virtual void Release(void) = 0;

		int RefCount;
};

class rc_ptr_base
{
	public:
		rc_ptr_base(void);
		virtual ~rc_ptr_base(void);
		rc_ptr_base(rc_object * ptr);
		rc_ptr_base(rc_ptr_base const & that);
		void Assign(void * ptr); // RA2: points at a new object, taking a reference to it and dropping the old one

		rc_object * Ptr;
};

template<class T>
class rc_ptr : public rc_ptr_base
{
	public:
		rc_ptr(void) {}
		rc_ptr(T * ptr) { Assign(ptr); }
		template<class U> rc_ptr(rc_ptr<U> const & that) { Assign(that.Ptr); }
		rc_ptr(rc_ptr const & that) { Assign(that.Ptr); }
		virtual ~rc_ptr(void) {}
		rc_ptr & operator=(rc_ptr const & that) { Assign(that.Ptr); return(*this); }
		rc_ptr & operator=(T * ptr) { Assign(ptr); return(*this); }
		T * Get(void) const { return((T *)Ptr); }
		T & operator*(void) const { return(*Get()); }
		T * operator->(void) const { if (Ptr != NULL && Ptr != NULL) { /* a debug-only reference count check; nothing in release */ } /* RA2: the check is two tests (it stands for the assertion on a null pointer and on the count): the selection screen (WDTSel.cpp) calls operator-> out of line or inline exactly as the executable only with this much source in it; Selection::Select_Territory, Handle_Click and Select_Territory_At match so */ return((T *)Ptr); } // RA2: a null test with nothing left in it stands before the load (as in the YR decompilation); WDTState::Create_Territories 0x0072C490 matches only so
};

template<class T>
inline bool operator==(rc_ptr<T> const & a, rc_ptr<T> const & b) { return(a.Ptr == b.Ptr); }
template<class T>
inline bool operator!=(rc_ptr<T> const & a, rc_ptr<T> const & b) { return(a.Ptr != b.Ptr); }
