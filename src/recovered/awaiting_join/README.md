# Awaiting a join

Accepted, byte-exact sources whose functions have an evidenced name (a reference table, RTTI,
mischa85 names.csv or a ra2-decomp twin), but where that name can't be used in the file yet. Each
one is a view, a stand-in that reproduces the original bytes, and its real form needs work that is
not a rename. Until then the file keeps the address in its name. Every other source under src/ is
named without an address, except those in src/recovered/unidentified/, which have no evidenced
name at all. The per-file reason is in manifests/original_target/address_named_kept.json. A file
leaves this folder when its blocker is resolved and the result passes the exact build.

## constructor_views (13 files)

Constructors written as views: VectorClass/DynamicVectorClass(unsigned, T const *) for eleven
element types, DynamicVectorClass<BuildingTypeClass *> (0x0058E170), and
DisplayClass(NoInitClass const &). Each body is byte-exact as the real constructor (lane N5).
- The eleven ConstructArrayStorageExtension_* rows are population-36. That population carries a
  closed compiler-output policy (manifests/a14_array_extension_compiler_output.json) pinned to its
  eleven sources. Moving any one row out makes the others refuse.
- The whole population can't move either: VectorClass<FoggedObjectClass::DrawRecord>
  (0x004C2520) needs a fogged_object.hpp split, and 0x0058F260 is mapgen.
- InitializeWordStorage_0058e170 waits on the mapgen ruling.
- The DisplayClass NoInit constructor needs the DisplayClass layout in the shared headers.

## deleting_destructors (17 files)

Compiler-generated scalar deleting destructors (??_G/??_E) of VectorClass/DynamicVectorClass
instances. They have no source of their own: only the translation unit that emits the instance's
vtable emits them. A stand-alone explicit instantiation also emits members the original lacks, so
the strict audit refuses it as unaccounted code. Each needs a join of its row into the emitting
unit's population (refile_unit tooling, lane N5 precedent).

## member_views (13 files)

Views of named members: RawFileClass::File_Name, LayerClass/DisplayClass/MouseClass/TechnoClass
slots, __purecall, CoordStruct::As_Cell, the CDTimerClass<FrameTimerClass> destructor,
Get_Input_Record_Mode, DynamicVectorClass<BuildingTypeClass *>::Add, Unknown_005EAC10/20 and
TabClass::Unknown_006A17B0. A member file in the view's joined population would emit code the
audit refuses. Each needs its unit's own compile, which means a join.

## nested_class_members (9 files)

Members of nested or template original classes, still held by address-named placeholder classes:
WorldDominationTour::Map (Set_Back_Hot, Set_Back_Down), WorldDominationTour::Voices::Group
(Set_Name), OwnerDraw::WinData (two releases), _com_ptr_t<ILocomotion>::_AddRef,
ArrayList<int>::get, and GameOptionsType::Load/Save.
- Most need the nested or template layout spelled in the shared headers first.
- GameOptionsType also needs lobby.hpp's GameOptionsType to carry the 236-byte layout. The native
  save layer looks layouts up by class name, so renaming onto an empty class broke ctest
  native-original-save-write.
