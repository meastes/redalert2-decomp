
// ---------------------------------------------------------------------------------------------
// NON-EXACT NATIVE DRAFT (lane Y3), appended to the palette unit: FUN_006267A0, YR 0x006267A0
// (504 bytes, unmatched in ra2-decomp): the colour-scheme library's theater change. From
// gamemd.exe's code: the library is made on first use (the unit's Library()); when the theater
// differs from the library's mode, the mode is set and every scheme set in the library is
// generated again for it (Generate by the key's name, the old set released, the new one kept in
// the node). Behaviour code for the native build; never part of an exact build.
// ---------------------------------------------------------------------------------------------
void __fastcall FUN_006267A0(TheaterType theater)
{
	SchemeLibrary *library = Library();
	if (library->Mode == theater) {
		return;
	}
	library->Mode = theater;
	HashIterator it;
	it.BucketIndex = 0;
	it.InBucketIndex = 0;
	it.OutOfBuckets = false;
	for (HashString *key = Next_Key(library->Table, &it); key != NULL; key = Next_Key(library->Table, &it)) {
		SchemeNode **node = library->Table->Find(*key);
		DynamicVectorClass<ColorScheme *> *schemes = library->Generate((char const *)key);
		if (node != NULL) {
			(*node)->~SchemeNode();
			(*node)->Schemes = schemes;
		}
	}
}
