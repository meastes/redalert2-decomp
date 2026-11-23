# VC6 matching notes

Practical rules learned while matching readable source against the original
with `diff_candidate.py`. Each was confirmed by a byte-exact result; they are
starting points for the next class, not guarantees.

## Types

- Westwood flags such as `opened` are `int`, not `bool` (`mov dword ptr` vs
  `mov byte ptr` stores). Class members that are flags are `bool`.
- A predicate whose callers test `al` returns `bool` (`Is_Open`,
  `Is_Available`); one whose callers test `eax` returns `int`.
- Stack bytes in `ret N` fix the parameter list; doubles and 64-bit values
  take 8 bytes.

## Expressions and statement order

- `x = c ? a : b` evaluates `a` first, then compares and conditionally loads
  `b`. Pick the operand order that reproduces the original's first load:
  `size >= room ? room : size` gives `mov edi, room; cmp size, room; jge`.
- Comparison operand order follows the source (`a > b` vs `b < a`).
- Assignment order is kept: `buffer += n; size -= n;` and the reverse
  compile differently. So does declaration-with-initializer order of locals.
- A value returned from two branches that ends in one register is often a
  parameter reused as the result (`size = total; ... return size;`).

## Control flow

- `if (c) { A } else { B }` lays out A first. When the original jumps far
  for the common case, the source tested the opposite condition first
  (`if (UseBuffer) { ... } return RawFileClass::Seek(...)`).
- A loop whose condition contains a ternary (directly or through an inline
  helper) is rotated: VC6 copies the condition to the bottom. An original that
  re-evaluates such a condition at the top with a `jmp` back was
  `while (true) { if (!cond) break; ... }`; `for`, `while (cond)` and
  `for (;; i++)` with an if-break all rotate (SidebarClass::Draw_It 0x0067CF80
  with the inline ternary StripClass::Max_Visible_Rows; the out-of-line
  0x006820A0 is an if/else).
- Two branch blocks ending in the same virtual call, with its vptr temporary in
  the same register where the eax/edx source-order rotation would alternate,
  were one shared trailing statement that VC6 tail-duplicated into each branch,
  not two calls (AircraftClass Enter 0x004193D0).

## Inlining and translation units

- Two out-of-line functions with identical bodies called from one inline
  expansion (or from the two element loops of one template) are two levels of
  one constructor chain: a wrapper and the constructor it forwards to
  (`D(x, y, z) : Base(x, y, z) {}`). Call sites that read the returned `this`
  to copy a temporary call the inner constructor directly; an inline-expanded
  constructor leaves an inner call whose result is ignored.
- In DynamicVectorClass<T>::Resize the placement `new (array) T[n]` loop
  expands one constructor level more than the heap `new T[n]` loop, whatever
  their source order. A one-level element constructor is called by the heap
  loop and expanded by the placement loop (ZoneConnectionClass 0x0056B330); a
  two-level chain gives an outer call in the heap loop and an inner call in the
  placement loop (WaypointClass 0x007256C0: 0x00434820, then 0x0041B630).
  VectorClass<T>::Resize compiled alone (0x007254F0) expands both levels.

- `/Ob2` inlines across functions of one translation unit, including
  functions defined later in it (`RawFileClass::Close` into `Is_Available`)
  and `inline` helpers with no out-of-line copy (`Raw_Seek`). Accept a class's
  methods as one plan so they compile as one unit, in address order.
- A qualified call (`RawFileClass::Open(...)`) is a direct, devirtualized
  call; an unqualified call on `this` stays virtual.
- An inlined base-class accessor (`RawFileClass::Is_Open` inside a derived
  method) appears as a direct member test; write the member test when the
  accessor has a separate accepted copy.
- A compiler-generated function (scalar deleting destructor) is emitted in
  every object file that needs the vtable; the linker kept the first copy.
  When the kept copy calls the destructor instead of inlining it, it came from
  another file; `#pragma auto_inline(off)` around the destructor reproduces
  it in the class's own unit (Straw, Pipe).
- Whether `/Ob2` expands a function defined later in the unit varies: it
  does in wwmous.cpp and MPMath (`mp_quo_digit`), but XSurface's 7-argument
  `Copy_From` (into 0x774F00) and DSurface's `Restore_Check` (into Lock and
  Unlock) only expand when declared `inline`. Marking a function `inline`
  can also suppress an out-of-line copy the original kept (wwmous.cpp
  `Unblock_Mouse`); an explicit trailing `return;` kept `Calc_Confining_Rect`
  from being expanded into the constructor.
  Found in the rebuild (lanes R15, R33): no rebuild case was decided by
  definition order. wonline's Set_Match_State 0x759DF0, an ordinary external
  definition, is expanded into SetPlayerAccepted, WOL_Wait_Callback and
  Start_Game_Now, which lie before it in address order; moving map.cpp's
  Get_Cell_Zone_Index and Get_Cell_Subzone_Index to the end of the file did
  not stop their expansion into Collapse_Cliff 0x562F50. Where a later
  definition stays a call, check the caller's inline budget and the callee's
  size against the auto-inline cap (Inline252) before its position.
- Found in the rebuild (lane R17): a file-`static` function is expanded into
  its caller where the original calls it out of line (reinf `_Create_Group`
  0x639DA0 into Do_Reinforcements); the same text non-static (main's form)
  stays a call and is exact. A called out-of-line copy of a one-caller helper
  is evidence that the original definition was external.
- Found in the rebuild (lane R20): declaring an explicit specialization of a
  class template member does not stop VC6 expanding the generic in-class
  definition at the call sites in that unit (BlitTransLucent50<unsigned short>
  ::BlitForward 0x487080, whose image copy is inline assembly); declare the
  member in the class and keep the generic body out of class.
- An uncalled out-of-line copy whose body other rows of the same unit expand comes from one ordinary (non-`inline`) external definition: /Ob2 auto-inlines it at its call sites, including callers defined earlier in the unit, and still emits its body at its place in definition order (the link keeps unreferenced code). Do not write an inline twin plus an out-of-line copy. Found in the rebuild (lane R33): wonline Set_Match_State 0x759DF0, Reset_Wait_Events 0x752930, Is_Tournament_Session and Select_WDT_Location lost their inline twins this way with no row lost, and the same mechanism is the likely explanation of main's uncalled byte-identical copies (DECISIONS "An uncalled out-of-line copy in the image whose body earlier rows expand"). It is the same rule as Loco54's never-called helpers in "Out-of-line helpers and global timers".
- Inline header members are emitted at the end of the unit; non-virtual
  accessors the original emits mid-unit were defined in the .cpp.
- `/Ob2` gives each caller one cumulative inline budget. The budget is
  max(floor, k x the caller's own size): plain statements, init-list entries
  and out-of-line calls in the caller all raise it. With 60 equal callees,
  24 + 0.73 per extra arithmetic statement are inlined; a simple store adds
  about 0.27 and an out-of-line call about 0.18. Small functions sit at the
  floor, so their own size does not matter. Later statements count as much as
  earlier ones (the size is the whole function's). Each site inlines when its
  cost still fits; it is not "stop at the first miss".
- **Depth-first, and a nested site pays for the sites still waiting above
  it** (Study152; replaces Inline17's "direct sites first"). VC6 decides call
  sites in source order; when it expands one it decides the sites inside that
  body next, before the caller's later sites. A direct site (depth 1) inlines
  when `cost <= remaining budget`. A site at depth d >= 2 inlines only when
  `cost * (1 + W) <= remaining budget`, where W counts the inline-candidate
  sites not yet decided in the enclosing bodies: a later direct site counts
  d-1 times, a later site in the parent body d-2 times, later siblings in the
  same body 0. Empty inline calls and inline calls that will be rejected as
  too large count in W; out-of-line, virtual and over-the-cap non-inline calls
  do not. Only accepted sites consume their cost, and free callees (Study37:
  at most two plain stores or one call) cost 0. So an empty call before the
  nested sites changes nothing (`E(); B();` = `B();`), one after takes the full
  penalty (`B(); E();`), and a large nested callee (a template constructor,
  Intersect) is pushed out by one or two later inline calls while a small one
  is not. Lab: `build/class-work/Study152/lab/` (a 3/6/12/24-store callee loses
  about its own cost per pending empty call; depth 3 pays 2x, depth 4 3x).
  Examples: WorldDominationTour::Map ctor 0x729330 (exact only with three
  member objects with inline constructors after Territories, which push the
  VectorClass<rc_ptr<Territory>> ctor 0x72BB10 out while Palettes' small
  vector ctor, decided last, stays inline); a DynamicVectorClass member's
  VectorClass base constructor, a helper's own callees, ToolTip() inside an
  inlined Tab helper. Examples:
  - FootClass ctor 0x4C2930 is byte-exact with one extra empty direct inline
    call anywhere in the body, at the start or after the two Adds (no natural
    source found yet). The same draft also has to drop `unknown_424(0)`.
  - A helper that wraps a call turns that call into a nested site. This is
    why MouseClass Load's `New_Cell_Table()` keeps the template ctor out of
    line. The helper works the same whether it is defined before or after the
    caller; VC6 does not "compile it first".
  - Swizzle_Pointer wrappers are direct sites. They spend budget that the
    nested sites would otherwise get.
- What costs budget: any direct call to an inline function, including an
  empty one; an empty constructor or destructor of a *local* object; a
  member constructor that has code. Empty *member* constructors (`Cell()
  {}`, `E() {}`, `~E() {}` members) cost nothing. A local with an empty
  destructor costs about as much as an empty call. A member array of a class
  with an empty constructor (`Point2D InlinePoints[6]`) is expensive: once
  the budget runs out, it becomes a `??_H` vector-constructor-iterator call
  that no original has.
- Individual size cap: a non-template function without `inline` is
  auto-inlined only while it is small: about 5 "statement + if" pairs in the
  lab. Writing `inline`, defining the function in its class, or making it any
  template member (in-class, out-of-class, with or without `inline`) lifts
  the cap to about 60-120 pairs. Spelling does not change the per-site cost.
  The definition's position (before or after the caller) changes neither the
  cap nor the cost. The earlier "a later-defined function only expands when
  declared inline" cases are this size cap.
- `#pragma inline_depth(1)` stops every nested expansion: FrameTimerClass
  `operator long` and VectorClass::Length become calls. It cannot explain a
  unit in which some nested sites inline.
  (Lab and the status of the open cases: `build/class-work/Inline17/`.)
- RA2's object `Load` functions fix
  up each pointer through an inline wrapper,
  `template<class T> inline HRESULT Swizzle_Pointer(T *&p) { return
  SwizzleManager.Swizzle((void **)&p); }`, one expansion per pointer; that
  pushes `IndexClass::Search_For_Node`/`Increase_Table_Size` out of line in
  Load while the constructor still inlines them (UnitClass::Load 47 → 98).
  The number of extra sites needed differs per Load (TerrainClass needs
  exactly one more, through an inline `Add_Object_Index` wrapper over
  `Add_Index`; FactoryClass needed only the swizzle wrapper). Empty member
  constructors do not count. Register choice is separate: a trailing real use
  of `this` changes which register holds it, empty inline calls do not.
- A bit read into a dead argument's stack slot (`mov dl,[esi+0x14]; and dl,1;
  mov [esp+arg],dl`) instead of `test byte ptr [esi+0x14],1` comes from a
  `bool` local declared *before* the condition and assigned inside it
  (`bool active; if (p) active = p->bit; if (!p || !active ...)`); a `bool`
  initialized inside a block does not reproduce it (AircraftClass slot 101).
  When the bit belongs to a member pointer, copy the pointer into a local
  first, inside the enclosing test (InfantryClass slot 219).
- Stack-slot assignment follows scope nesting, not declaration order:
  BuildingClass::Load only matches with its body inside `if
  (SUCCEEDED(result)) { int count; ... } return result;`.
- Access members through the real class, not a cast view of another type
  (`House_View(House)->...`): the extra cast changes register allocation.
- Call arguments are evaluated right to left; an inline helper's argument
  order decides which expression is computed first.
- A `bool` comparison result returned with a bare `setcc al` comes from
  `return x <= 0 ? true : false;`; the branchless "object if it is a
  building, else null" (`sub; neg; sbb; not; and`) from an early null return
  followed by a ternary (SidebarClass StripClass sort and filters).
- In an additive chain a ternary operand is evaluated first; a local assigned
  by `if/else` keeps the source order. SidebarClass's strip row count only
  matches as `int extra; if (flag) extra = 26; else extra = 18; return (y +
  height - top - extra - 7) / 50;` (13 functions fell to this one rule).
- With a local object that has a destructor (a COM smart pointer), the
  budget pressure that pushes its helper expansions out of line comes only
  from direct inline calls made while that object is alive, a second named
  local with a destructor (even an empty one), or an extra `return` path in
  its scope. Plain calls, temporaries, calls nested inside other inline
  functions and inline calls made before the object exists do not count. One
  or two extra sites push one expansion out of line, three or four push two
  (micro-tests in build/class-work/ComPtr/w/micro). The kind of pressure
  decides which expansion goes first: a second named local with a destructor
  pushes out a query's failure-path release, while extra inline calls, extra
  returns and dead comparisons push out the success path. The locomotor Saves
  behave as if one such local exists, InfantryClass slot 288 as if two do;
  no natural source for them is known yet (open).
- Under `__fastcall` a `float` parameter is never passed in a register, so
  the next integer parameter takes `edx`; with right-to-left evaluation the
  `mov edx, imm` then lands between the stack pushes. The sound call 0x713DB0
  is `void __fastcall (int sound, float volume, int priority, void *object)`;
  declaring the int before the float moves `mov edx` after both pushes.
- A two-dimensional table indexed as `table[row][col]` (`[][4]`) gives
  `lea eax,[edx+ecx*4]; mov eax,[eax*4+base]`; a flattened index gives a shift.
- `test` then `jbe` comes from an unsigned `> 0` test, not `!= 0`.
- A `__fastcall` argument passed through untouched keeps the original's `ecx`;
  many seeds miss that parameter entirely.
- `return value += v;` with a `float` return gives `fst` instead of `fstp`.
- A by-reference argument whose temporary is stored *after* `push &temp`
  comes from an rvalue argument (`List.ID(this)` inside a member, `&*node`);
  a named local or a plain parameter stores the temporary before the push.
- The order of the terms in a packed-pixel expression sets the order of the
  channel loads; try all six permutations when a pixel build misses.
- A function out of line at every call site in its unit but compiled in that
  unit is an ordinary non-`inline` function at its own address position; VC6
  still inlines it where its budget allows. `#pragma auto_inline(off)` is only
  for a function the original never inlines anywhere in the unit.
- Keep a local copy of a pointer passed by reference to a call
  (`T *pointer = item; list.Delete(pointer); delete item;`) to keep `item` in
  a callee-saved register across the call.
- 64-bit values stored as two halves (`unsigned __int64` local) reproduce the
  original's split store order; a tail `jmp` into a `this`-taking callee needs
  the callee declared `__fastcall` rather than as a member.
- A counter the original reloads around an increment (load, inc, store, and
  sometimes a reload after a test) is `volatile`.
- `return field = -1;` gives `or eax,-1` then the store.
- Evaluate a lookup into a local before a multi-argument `__fastcall` call,
  or it is evaluated after the stack arguments are pushed.
- Copy-initialize from an explicit temporary (`CellStruct spot =
  CellStruct(...)`) to get the original's build-a-temporary-then-copy
  pattern; direct construction or member assignment does not.
- Split chained constant multiplications (`x *= a; x *= b;`) to keep two
  `fmul`s; one expression folds the constants.
- An init-style function starting with `mov eax,ecx` is a constructor that
  returns `this`; `while (index--)` gives `mov esi,eax; dec esi; test eax`.
- Small functions defined *later* in the unit are still expanded inline
  (XSurface Copy_From, RadSiteClass::AI → Update_Light, Matrix3D Translate).
  Splitting a body into later-defined helpers keeps both the inline
  expansions and the out-of-line copies.
- Switch statements place their jump table and byte index table directly
  after the code; accept such a function with a plan `body_bytes` covering
  code and tables.
- Some out-of-line functions are never called (inlined everywhere): they have
  no entry references and are accepted with alignment-bounded entry evidence.

- A small helper written as an inline function and the same helper written as
  a `#define` macro compile differently: SHA's rotate only matches as an
  inline function.
- Pass-by-value structs change argument passing: RLE_Blit only matches when
  its last parameter is an 8-byte struct with a constructor (`BlitPair`), not
  two ints, an `__int64` or a 12-byte struct.
- A template member that the original implements in hand-written assembly is
  still instantiated generically by VC6 even when the specialization is only
  declared; the generic COMDAT is recorded as kept elsewhere (bound to the
  assembly entry).
- String literals are verified against the original bytes at their bound
  address; a literal whose text differs is refused even when the code matches
  (`"?:"`, not `"CD"`, in CDFileClass::Set_Search_Drives).

## Classes and virtual tables

- Overloaded virtual functions share the vtable position of the first
  overload, in reverse declaration order: declare each overload set
  last-slot-first (Surface).
- A class with an implicit destructor does not store its own vptr in the
  scalar deleting destructor; a user-declared empty `~Class() {}` does
  (BSurface).
- Westwood nests per-class control enums (`LCWPipe::CompControl`); the nesting
  is visible in constructor manglings.
- `memset` is an intrinsic (`rep stosd`) only when declared and listed in
  `#pragma intrinsic` (`shared/crt_string.hpp`, `shared/crt_memory.hpp`).

## Libraries and imports

- Win32 calls go through `__declspec(dllimport)` (`shared/win32_api.hpp`).
- `strcmp` is an intrinsic at `/O2` (expanded inline); `memmove` is a call.
- `operator new`, `operator delete`, `_strdup`, `free` and `memmove` are bound
  through `manifests/symbols.json`.

## Register allocation

Derived from controlled experiments with the real compiler (Regalloc27,
2026-10-27; lab tools in the ignored build/class-work/Regalloc27/).

- VC6 colours def-use webs (not source variables) greedily by priority; each
  takes the first of esi, edi, ebx, ebp not held by an interfering web already
  coloured, else a stack slot (reloaded into eax at each use inside a loop).
  Values not live across a call use eax/ecx/edx. Reusing a variable for
  disjoint values gives separate webs; a dead store is dropped.
- Priority is the reference count: a def or use is 1, `x += y` / `++x` is 1 in
  total, a use inside an if is about 0.5 (nested ifs do not compound), a use
  inside a loop about 1.4 (depth 1), 2.5 (depth 2), 5 (depth 3), minus a
  live-through penalty (see "Register priority is references minus live-through
  pressure" below, Scout1212). The instruction length of the range does not count.
- Ties go to the web spanning fewer basic blocks, then to the one *assigned*
  first. Declaration order and scope nesting alone change nothing; reordering
  independent initialisations does.
- Parameters are live from their first use, not from entry; `this` is defined
  at entry (`mov esi, ecx`) and wins ties. Copying a parameter into a local at
  entry moves its start earlier.
- `char`/`bool` locals live across calls can only use ebx (bl); a second one
  goes to the stack even with esi/edi free. `short` can use any register.
- A copy whose source dies is coalesced with it and their counts add
  (`for (int c = x; ...)` versus looping on `x` changes coalescing). A loop
  index unused in the body becomes a low-priority countdown temporary.
- Constant 0 is cached in a register only if an already-saved callee-saved
  register is free (it never causes a new push); its range runs to the last 0
  it could serve, including byte compares, so it can push a pointer out of ebx.
- A global esi/edi permutation means two long webs are tied or near-tied; a
  statement reorder that changes coalescing can flip all of it.
- Legitimate knobs: order of independent initialisations, a separate copy
  variable versus the variable itself, reading a field into a local once versus
  re-reading, conditional versus unconditional uses, char/bool versus int.
  Dummy uses, volatile, `register`, dead code and allocation-only casts are
  steering and are refused.

## Call-argument scheduling

Derived from controlled experiments with the real compiler (Sched29, 2026-10-27; lab in
the ignored build/class-work/Sched29/).

- Without scheduling (/G6) VC6 pushes arguments right to left, computes each address
  (`lea eax, local`) just before its push and loads ecx (this) last. /G5 moves an address
  `lea` up by one push to pair it and hoists argument memory loads; /G3, /G4, /GB and no
  /G schedule identically.
- In a `__fastcall` function (free or static member), after a callee-pops call (thiscall,
  virtual, stdcall or fastcall, even without arguments) the next call's register setups
  (`lea ecx, object`, `lea eax, &local`, `mov ecx/edx, imm`) come before all of its pushes:
  `call; lea ecx, file; push "ART.INI"; call ctor`; `lea eax, file; mov ecx, ArtINI;
  push 0; push 0; push eax`; `mov edx, flag; lea ecx, result; push getter; call Create`.
  Other callers push first. An `add esp` after a cdecl call has the same effect in every
  caller; a cdecl call without arguments does not. The convention of the function being
  compiled decides, not the callee's or an inlined function's.
- So a register-first setup in a free or static function without parameters (whose call
  sites cannot show the convention) is evidence that it is `__fastcall` (Read_Art_INI,
  Coop_Camp_Unique_ID, 0x6525F0, Conflict::Init_Options).
- A temporary built by an out-of-line constructor and passed to a `T const &` parameter is
  pushed as the constructor's return value (`push eax`) when the source casts explicitly
  (`Add(T(x))`) and as a fresh `lea` of the temporary when the conversion is implicit
  (`Add(x)`). The implicit form also enlarges the caller (more inline budget).
- Independent global stores are sunk towards the next call (between `mov ecx` and `call`);
  their source position before a call does not show in the order.

## Free and charged inline callees, block placement (Study37)

- Free and charged callees (Study37). A callee whose body is at most two plain stores, or a
  single call statement whose result is unused, is expanded at every site and costs no
  budget (it goes out of line only at the caller's absolute size cap). Anything larger is
  charged its whole size, not the excess: one more node (a discarded read, a discarded
  returned reference such as `{ Set(x, y); }` with `Cell &Set`) turns a free two-store
  constructor into one that runs out after 78% of a table. Three stores (CoordStruct, Rect)
  are always charged. An original out-of-line call to a tiny constructor means the original
  constructor was charged (Cell 0x0042AD30: Map tables 288/369, 241/309, 8/40, 7/42, and
  DynamicVectorClass<ZoneConnectionClass>::Resize 0x0056B330 calling it for To).
- The budget is proportional to the caller's size (a fixed fraction of equal sites inlines for
  any table length) up to an absolute cap past which the inlined count falls as the caller
  grows. A callee's cost is its own unexpanded IL; nested sites are decided depth-first and
  pay for the sites still pending above them (Study152, above), except free callees, which always expand.
- A dynamic initializer is `_$E(n+1): jmp _$E(n)` for large objects; small ones are expanded
  into the .CRT$XCU entry itself, which makes their constructor calls depth-2 sites with only
  the entry's floor budget (Map 0x0055B520: 8 of 40 constructions inline).
- Commutative operand order (an addition's operands, two virtual calls in an inlined
  `a * f() + b * g()`) and some schedules are tie-breaks that move when an earlier inline
  expansion in the same function creates more or fewer temporaries (a nested call's
  parameters). A shared inline-header change can therefore alter accepted functions that
  never use the changed path; check it with a whole-tree compile (Study37 w/impact.py).
- In an additive chain every if/else spelling of a conditional term gives the same canonical
  order `(a - c) - extra`; a ternary gives `(a - extra) - c`, and a ternary in a `for`
  condition makes VC6 rotate the loop (condition copied to the bottom).


- An early return guarded by one test stays inline (`jne cont; epilogue`); one guarded by a
  short-circuit chain (`if (p == 0 || !p->flag) return x;`) is laid out after the following
  code. The original's single inline return block reached by both tests (`test p; je R;
  ...; jne cont; R: epilogue`) comes from evaluating the condition into a bool first:
  `bool ok = p != 0 && p->flag; if (!ok) return x;` or `ok = false; if (p) ok = p->flag;
  if (!ok) return x;` (jump threading). TechnoClass 0x6D4180, slot 175 0x6C15D0.
- `return 0` right after a `test eax,eax` branch reuses eax instead of `xor eax,eax` unless the
  return block is shared: identical return blocks are tail-merged, and the shared copy keeps
  the xor.

## Stack frame layout (Study42)

Derived from controlled experiments with the real compiler (Study42, 2026-10-27; lab in
the ignored build/class-work/Study42/, `f.sh` prints a function's frame map).

- Frame variables (address-taken locals, objects, temporaries, spilled webs) are sorted by
  size ascending, then by the number of instructions referencing them in the final code (a
  loop does not weigh more), then (usually, not always) by whose last reference comes
  first. Each takes the first
  compatible slot: dead argument slots in the order the arguments died (objects of at most
  4 bytes; an 8-byte object only a dead double argument), then existing local slots newest
  first, else a new slot. 1-byte objects never share a local slot with wider ones. Local
  slots are laid out from the top of the frame down in reverse creation order; the frame
  is rounded to 4 with the padding at the bottom.
- An argument slot dies at its last read (a load into a register frees it; a copy
  `int t = a;` is forwarded to its use); a never-read argument's slot is never reused.
  No argument slot is reused at all when the local frame would be 112 bytes or more
  (corpus: 161 such functions, none reuses one).
- A variable whose address reaches a real call is live from the first call in its block (a
  temporary: in its statement) to the last one; every call counts as a use and a
  definition. So `g(a); g(b); int x, y; use2(&x, &y);` reuses only a's slot (b is read
  after the function's first call) while `g(a); g(b); { int x, y; ... }` reuses both.
  Other frame variables use exact liveness.
- Empty-class temporaries (`NoInitClass()`) are 1 byte: they are placed first and take the
  first-dead argument slot. HouseClass::Load matches only with BuildingClass::Load's
  `if (SUCCEEDED(result)) { int count; new (this) HouseClass(NoInitClass()); ... } return
  result;`: count's block then contains the temporary and count takes the stream slot.
- Fix a frame-only miss with block placement of declarations, statement boundaries and
  statement order (which argument dies first, which variable is referenced last), not with
  dummy references.
- Open: an original slot that is allocated but never referenced (0x712300's top slot;
  MapSizeGameOption::Describe leaving the dead territory slot unused and putting its
  operator+ temporary in a new slot) is a variable that existed at layout time and lost its
  references afterwards; no source form found yet.

## Block-wide scheduler state (Big49)

  placement miss late in a long block (a `mov ecx, reg` before or after the pushes, an `fstp`
  around them) can come from earlier statements whose code already matches. A value passed
  through a named local (`double v = x * 0.01; f(v);`) emits the same code as the inline
  argument but shifts that state; try locals for a run of similar statements (Write_Basic
  0x6625B0: all twelve [Lighting] values). Dummy statements that move it are steering.


## Named locals and the inline budget (Big49)

- Named locals count in the caller's inline budget though they emit nothing:
  `int value = ini.Get_Int(...); Field = value;` and `Field = ini.Get_Int(...);` compile to the
  same statement, but the first enlarges the caller. When a long reader (RulesClass::AI,
  Combat_Damage) inlines slightly fewer helpers than the original, put one kind of read
  (ints and leptons there) through locals; the band is narrow, so try per-kind sets, not
  arbitrary subsets. Apply one rule per function (every read of a kind), never individual
  statements picked to hit a count (owner ruling 2026-10-27, landed 5ccf5f93).
- VC6 numbers a unit's header-static initializers (`_$E`n) in order of first inclusion,
  so include order inside a unit with header statics is part of the match.

## Macro clamps and member accessors (Big51)

- VC6 compiles a parenthesized member `(x)` in a ternary differently from bare `x`: `x = (x) < c ?
  (x) : c` keeps a copy of the stored value (`fld st(0); fcomp`) and a `je/jmp` pair over the false arm,
  where `x = x < c ? x : c` gives `fcom` and an inverted branch. Westwood clamps are macros
  (`#define Min(a, b) ((a) < (b) ? (a) : (b))`); write them as the macro (Read_Basic 0x00661980,
  RulesClass::General DropPodAngle).
- An accessor that stands in for a member (`T &List() { return *(T *)((char *)this + 0x2270); }`) is
  a direct inline site at every use and spends the caller's budget; the original had a member. Declare
  container members with their real types (shared/vector_class_decl.hpp, shared/type_list_decl.hpp
  give the class declarations without the member definitions) (UnknownIDSource::Read_Basic).

## Switch layout and cross-jumping (Study50)

Derived from controlled experiments with the real compiler (Study50, 2026-10-27; lab in the
ignored build/class-work/Study50/, `lay.py` prints a listing's block layout with the cases
reaching each block).

- Dispatch: up to 3 case labels compile to an ascending compare chain (`dec eax; je`,
  `sub eax, delta; je`) with the default inline after it and the case blocks in descending value
  order whatever the source order. From 4 labels a jump table; it is two-level (byte index table
  plus target table) exactly when 3R > 4n + 12 (R = value range, n = case labels without
  default). Very sparse switches split at the median value. The target table of a two-level
  switch lists targets by smallest case value, default last.
- Jump-table case blocks follow source order; the default block goes after all of them wherever
  it stands; the code after the switch follows the last case (in source order) that breaks to it,
  and cases that return come after it. Partial common tails (`add esp, 4`, `call f`) are sunk into
  the block that falls into the join.
- Identical blocks (identical final code: a different scratch register prevents it, and scratch
  registers rotate after a block with fewer temporaries) are merged into one copy; the copy
  entered by fall-through stays (the first case after a dispatch counts as entered by
  fall-through); otherwise the later copy stays within one switch, and across two switches the
  later copy stays only when the block's tail matches the tail of the block that falls into the
  exit (else the earlier copy stays). Runs of identical consecutive blocks move together. The
  surviving block keeps the deleted copy's label.
- Which identical return a duplicate merges into depends on the other paths: TechnoClass slot
  175 0x6C15D0 sends case 6's `return -1` to the guard's return only with
  `case 0: if (c) return -1; return 0;` (with `break` to the shared `return 0` it joins case 0's).
- Two tests of the same values fold when they read the same variables (`if (s == 3 && t == 1)
  return; if (s != 1 ...)` loses `cmp t, 1`); through inline accessors (`Map.Closing() ||
  !Map.Active()`) both compares stay and the known constant is reused as a register
  (`cmp ecx, eax`). Rereading members folds too and leaves a dead load. (RTacticalClass::Action
  0x62FE30 with RadarClass 0x6331C0 and 0x633110 inline.)
- `d = d - limit + 1` becomes `mov r, 1; sub r, limit; add d, r` in every spelling; recomputing
  from the operands (`d = x - y - limit + 1` after `d = x - y`) gives `sub d, limit; inc d`. A
  named sum (`d = x + y; if (d > h) d -= h;`) gives `add; cmp; sub`; the recomputed
  `if (x + y > h) d = x + y - h;` can become `lea t, [y + x]; ...; sub y, h; add y, x`.
- Commutative operand order (which register an `add` writes) is a function-wide tie-break that
  unrelated temporaries move (0x62FE30's step-4 sum flips only when the slot-48
  `CoordStruct(cell.X * 256 + 128, ...)` temporary is removed); spelling, casts and local copies
  do not change it.
- A virtual call on a member of a named global object (`Map.List.Clear()`) is bound statically and
  inlined; through `this` or a reference it stays virtual. An original virtual call on a global's member
  comes from an inline member of the owner (`Map.Clear_Cells()` with `{ List.Clear(); }`), which also
  spends the caller's budget like the original (Read_Scenario_INI 0x0065EB40) (Big51).
- A no-argument function's convention shows only in its callers: in a `__fastcall` caller, the call
  after it loads ecx before the pushes only if it is callee-pops (`__fastcall`/thiscall) (Big51 ren2).
- Assignment through an inline `operator=(char const *)` evaluates the destination first; the original's
  temporary-first order comes from `x = FixedString<21>("...")`. Parallel arrays indexed from one base
  pointer (`[ebx-0x20]`, `[ebx]`, `[ebx+0x20]`) are members of one object (Assign_Houses 0x0065FBB0) (Big51).

## Small observations (Near48)

- A call that leaves ECX holding the caller's `this` (no `mov ecx` before it) is a free function, not a
  member: a stale ECX is not an argument (0x0051FC50).
- Repeating a member expression through a global pointer (`MissionTimer.Value()` twice) re-reads the
  fields; a reference local lets VC6 reuse them (0x006B0B20).
- Compares on a local int are jump-threaded (a passed test jumps straight to the call); the same member
  read repeated in each test is CSE'd but not threaded (0x006B1F10).

## Out-of-line helpers and global timers (Loco54, Scen55)

- Helpers the original keeps out of line but never calls (DriveClass Start_Driver, Stop_Driver, Hover/Fly
  Start_Drift, ...) are ordinary non-inline members defined in the unit: /Ob2 expands them at every use,
  and the standalone copy is emitted at its own address (Loco54).
- Functions returning a Matrix3D through a pointer keep the pointer form: `*matrix = transform; return
  matrix;`. Taking a base result as `Matrix3D m(*Base(&t, key))` reproduces the original's call of the
  copy constructor 0x0058FE10 (Locomotion Draw_Matrix family).
- Calling an inline timer member directly on a global (`FrameTimer.Value()`) lets VC6 drop the redundant
  -1 test inside it; a reference local (`CDTimerClass<..> &timer = FrameTimer; ...`) keeps it, as the
  original does (Post_Load_Game 0x0065D2C0).

## Members and temporaries (Loco56, Scen57)

- A zero-initialised `double` member is stored as two dword stores, which the scheduler reorders
  independently; a constructor whose init-list store order is off often has a member that is really a
  double (Fly +0x40, Rocket +0x48, Jumpjet +0x68/+0x70/+0x80 constructors).
- A `float` local holding a function result (`float ratio = Slope_Ratio();`) keeps the unfolded
  `fld 1.0; jmp; fcomp` comparison the original has (Drive Is_Level 0x0049FFC0).
- A virtual call on a temporary (`CD().Force_Available(...)`) is emitted as `call [vtable]`, not bound
  statically (Scen57).

## Constants, negations and temporaries (Big75)

- A constant cached in a callee-saved register (`mov ebx, 1`, later `mov eax, ebx`, `cmp eax, ebx`)
  comes from a variable initialised to it: `int players = 1; if (...) players = list.Count();`. The
  equivalent ternary loads the constant inline and never caches it (0x0065C7D0).
- `sete al; test al, al; sete al` for a negated comparison passed as a bool is a bool local negated at
  the use (`bool campaign = mode == 0; f(!campaign)`); `mode != 0`, `!(mode == 0)`, `!!mode` and
  casts all fold to one `setne` (0x0065C7D0).
- A named pointer local (`char *extension = &name[strlen(name) - 4];`) computes its value at its
  statement and moves a cached constant's live range; the same expression inline in a later call
  evaluates late (0x0065C7D0).
- Inline helpers fix the evaluation order of their arguments: `Cell operator-` built through the
  constructor computes Y before X, and a length helper that converts Y then X reproduces a distance sum
  whose inline spelling changes the register webs of the whole function (Create_Units 0x00660320).
- A zero-initialised accumulator declared before an inline `Clear()` gets its own register; declared
  after it, it shares the cached zero's register (Create_Units).
- `f(&CoordStruct(...))` builds the temporary after the later arguments are pushed; a named local
  built before the call stores ahead of the pushes.
- `volatile` is steering when it only moves registers. It is the original's source when the original
  performs the memory accesses only volatile semantics keep: `volatile int zero = 0; zero = 1 / zero;`
  stores the zero, reloads it, divides and stores the quotient (a deliberate fault,
  SwizzleManagerClass::Process_Tables 0x0069FC30; owner ruling 2026-10-28).
- A container method called through a pointer to its owner (`Rules->BaseUnit.ID(x)`) stays a virtual
  call (only a named global's member is devirtualised). An original that scans the container inline
  there used a non-virtual inline helper (`TypeList::Is_Present`).
- A CDTimerClass test through `operator long` leaves `Value()` out of line; `.Value()` inlines it
  (SpawnManagerClass::AI 0x00689C90, with the tail-duplicated trailing-call rule: 84.5 to exact).
- In a long compare-list function, the last `if (list) return X;` of each group stays inline in the
  original only when written `bool b = list; if (b) return X;` (applied to every group); otherwise VC6
  lays the return after the following code and cross-jumps equal returns. Equal single-value returns
  were one `||` list (OwnerDraw 0x005E0010, 0x005E0760).
- The bool-first rule extends to if/else: an if/else whose condition is a `||` list lays the else
  first unless the condition is a bool local. A bool `return a || b || ...` shares one `mov eax, 1`
  block across all lists (Big80).
- VC6 delays a callee-saved push until the register's first use, and saves it only around an if-body
  when that is its only use.
- A return the original sends to the function's final return block was an else-part return:
  `if (id != X) { ... } else return V;` (OwnerDraw 0x005DEFB0).
- For a parameterless function, `__fastcall` versus cdecl moves where the epilogue's `pop` is
  scheduled (Delete_All_Objects 0x00519960 is `__fastcall`).
- Separate stack slots for several temporaries mean they belong to one statement (MapClass 0x0055DEF0's
  redraw is one statement with an inline `Rect - TPoint2D`).
- A separate-unit plan cannot include headers that carry header statics; such functions go in their
  real unit.

## Lobby rules (Netshare183)

- VC6 strength-reduces two arrays onto one induction pointer only when both are members of one object; read
  both through that object (0x5CA540: AIColor at session +0x84 and the slot table at +0x2800).
- When /Ob2 inlines too much in a large body, write its repeated parts as file-static helpers; the budget then
  reproduces the original's inline set (0x5CA000 41-63 as one function, exact with helpers).
- A parameterless function declared `__fastcall` schedules a fastcall callee's ecx/edx loads before the pushes
  (0x5C86E0, 0x5C3040, accepted 0x4D1EA0); it did not help 0x5C67C0 or 0x5C34F0.
- A WideString temporary destroyed before the return copy is a block-scoped local (0x5CCF50).

## Westwood source idioms (published Westwood sources, 2026-10-28)

The published Red Alert and Renegade sources ([WESTWOOD_SOURCES.md](WESTWOOD_SOURCES.md))
show how the original code was written, which is what the rules above are reverse
engineering. Read them for shape; GPL code is never copied, and every shape is still
verified on the target.

- The recovered library headers already reproduce the wwlib text: VectorClass,
  IndexClass, GenericList, Straw/Pipe, Pick_Random_Number, TRect/TPoint2D and the
  timers match member for member. When a lane needs a container or library member it
  has not met, read the wwlib header first; its names and statement order are the
  original's. The RA2-only differences are listed in WESTWOOD_SOURCES.md.
- `wwlib/always.h` is included by every library unit. It defines
  `WWINLINE` as `__forceinline` under MSVC and uses it on `VectorClass<T>::operator[]`,
  `Length()`, the NoInit constructor and `RefCountClass::Add_Ref`/`Release_Ref`; it
  pulls in `visualc.h`, which sets `#pragma inline_depth(255)` (VC6's default is 8) and
  disables warnings 4514, 4244, 4056, 4710, 4355, 4097, 4505, 4711, 4511, 4512, 4100
  and 4786; it defines `NOMINMAX`, `MIN`/`MAX` as the usual double-evaluating macros
  beside template `min`/`max`, and `ARRAY_SIZE`. `bound.h` has the `Bound<T>` template
  (two ifs, not a nested ternary), `swap.h` a `swap<T>` with a default-constructed
  temporary, `incdec.h` the enum `++`/`--` operator templates. Tested (Study158): neither
  idiom changes any open case. `#pragma inline_depth(n)` only caps nesting (default 8,
  the 9th level is called); 255 leaves budget, W thresholds and whole-unit objects
  identical at depth 8 or less (MultiplayerGameMode and BuildingLight units compared byte
  for byte). `__forceinline` expands its site past the budget and its own W, costs no
  budget, still counts as a pending site in W for earlier nested sites, and does not lift
  the depth cap. Plain VectorClass `operator[]`/`Length()` and the empty NoInit
  constructor are already free callees (cost 0, counted in W the same way), so `WWINLINE`
  on them gives the same code as `inline`: no ruling is needed and `__forceinline` stays
  banned. Side observation: free one-call wrapper levels do not multiply W by depth (an
  X under 1, 4 or 7 free wrappers goes out at the same budget); Study152's per-depth
  multiplier was measured with costed levels.
- The MIN/MAX macro under "Macro clamps and member accessors" is wwlib's spelling
  (`MIN(a,b)`, `MAX(a,b)` in always.h); Red Alert's JSHELL.H had template `min`/`max`
  and `Bound` instead.
- Game-code idioms from Red Alert that the class lanes keep meeting: NoInit constructors
  pass `x` to every base and member that has one
  (`AircraftClass(NoInitClass const & x) : FootClass(x), Class(x), SightTimer(x) {}`);
  `Is_Infantry()`-style tests are inline RTTI compares in the header; each object class
  has `static void Init(void)`, an out-of-line pool `operator new(size_t)` beside an
  inline placement `operator new(size_t, void * ptr) {return(ptr);}` and a
  `Read_INI(CCINIClass &)`; nested constants are `enum {FLIGHT_LEVEL=256};`;
  parameter lists are `(void)` and accessors `const`; 1996 flags are
  `unsigned IsX:1` bitfields where RA2's are `int` members and `bool` returns (see
  Types). `Percent_Chance`, `Random_Pick` and `Sim_Random_Pick` are inline wrappers in
  INLINE.H over the scenario and simulation generators.

## Dialog chains (Modem185)

- Chat chain: write the leaf (Show_Chat), a caller with text (Send_Chat(text) calls Show_Chat) and the parameterless
  wrapper (Send_Chat() calls Send_Chat(text)); the dialog proc only gets the original's inline set in that form.
- A helper the original keeps out of line stays out when written with an explicit if/else into a local, while the
  same logic through a small inline accessor gets auto-inlined (Read_Color 0x597xxx vs Player_Color_RGB).
- Each checksum test written as clearing a `match` flag then `if (!match)` reproduces the original's branch layout.

## rc_ptr operator-> (RcPtr192)

- `rc_ptr<T>::operator->` carries an empty null test before the load (`if (Pointer == 0) {} return (T *)Pointer;`,
  a release-empty debug check). It keeps a second-level chain's `this` in a register (`add eax,8; mov ecx,[eax+4]`
  instead of a folded `[eax+0xC]`), keeps the dead `lea` of `(*cursor)->f()`, and charges inline budget, which is
  why operator-> COMDAT copies exist. Get() keeps the plain body; code the original compiles without the test reads
  through Get() (State ctor 0x730830). WDTSel unit: 17/19 drafts exact with it, 11/19 without.

## Out-of-line base vector constructors (Session200)

- When the original expands TypeList/DynamicVectorClass constructors inline but calls the VectorClass base
  constructor out of line, declare the VectorClass constructor without a definition in scope and define only the
  derived constructors inline as explicit specializations; any generic definition in scope (even after the caller)
  makes VC6 inline it. The out-of-line copy is its own row (0x674AF0 VectorClass<Wstring>).

## Waiting sites decide nested expansion (UICmnds206)

- Which depth-2 calls the original inlines is decided by the inline sites still pending later in the caller (W in
  the Study152 rule), not by definition order. UICmnds.cpp commands call the list rebuild 0x6F79C0 (depth 1, inlined);
  its builder calls (depth 2) stay calls only because the command's loop reads the list through
  DynamicVectorClass accessors (Count(), operator[]), each a pending site. Reading raw fields removes those sites and
  the builders expand. The out-of-line 0x6F79C0, with nothing pending, inlines both builders. One accessor too many
  pushes out a later expansion that must stay. Score such functions in the whole-population TU (UICmnds206 ws.py),
  never in a prefix compile.

## Local DynamicVectorClass vtables outside their home population (House208)

- A local `DynamicVectorClass<TechnoTypeClass *>` (vtables 0x7A0F44/0x7A0F64; RTTI says `T const *`) or
  `<BuildingTypeClass *>` (0x7A2F28/0x7A2F08) compiled outside the Factory population fails with "Unsupported
  allocatable data section" and its slot names bind to another copy. @ref both vtables and every slot name to the
  original vtable dwords (House208 refs.py generates the lines; refs_tt.txt, refs_bt.txt).

- (UICmnds210) A function called through a function-pointer type inlines through the constant pointer only when
  the parameter count matches the pointer type; return type and pointee type differences do not block it.

## _$E initializer numbering counts static data members (Session209)

- VC6 advances a unit's `_$E` initializer counter once per class static data member declaration the unit sees
  (static member functions and inline virtual destructors do not count). A header that pulls a class with static
  data (e.g. CDFileClass's search-list statics via cc_file_class.hpp) into a widely included header renumbers every
  unit's `_$E` symbols and the build fails "Function symbol ambiguous/missing: _$EN". Keep such includes out of
  shared headers that units include for unrelated declarations.
- Found in the rebuild (lane R20's countsens.py; owner landing, 2026-11-19): guarding or unguarding one function renumbers the `_$E` initializers after
  it, even for a function with no statics of its own (mapgen Create_Urban_Area 0x586B40). The renumbered rows stay
  exact once renamed, so rename a unit's initializer rows by their evidence (the .CRT$XCU table, then the calls and
  atexit registrations each makes: rebuild/tools/xcu.py) before reading a trial as a loss.

- (TAction211) A coordinate built through an inline `XYZ_Coord(x, y, z)` evaluates its arguments right to left
  (Z, Y, X) like any call; that, with `Cell spot = waypoint - Cell(size / 2, size / 2)`, reproduces the original's
  "Cell temporary" order in the reveal actions.

## Static over-the-cap functions count as pending sites (Queue220)

- In the Study152 budget, a later `static` function in the unit whose size is over the auto-inline cap still counts
  in W (a pending candidate site) at its call; an external function of any size does not, and a small external one
  is simply inlined. Queue_AI 0x623770 is exact only with Queue_AI_Multiplayer 0x623B00 defined `static` in the unit
  (as in Red Alert's QUEUE.CPP): land them together.

- Tail calls: after callee-saved pushes VC6 turns a final call into `pop; jmp` only when the callee is `__fastcall`; a
  cdecl or stdcall callee keeps `call; pop; ret` (Prog_End 0x6909E0 -> Kill_Mouse_Thread, Startup226).
- An `extern` declaration of a global placed after its dynamically initialised definition in the same unit makes VC6
  emit the initializer twice; include the declaring header before the definition (Startup226 header statics).
- String literals outside a unit's own pool (shared "%s", "INTRO", registry paths) need @ref lines: diff_candidate
  shows them resolved, but acceptance refuses the population with "Unsupported allocatable data section .data"; read the
  mangled names from the compiled object (Wonline239, Queue225).
- A class template member's out-of-line copy (e.g. WolDictionary<WideString,bool>::Shrink_Table 0x5CD9A0) is emitted only
  when the member is defined outside the class and some caller in the unit keeps a call to it; the accepted row source
  is an explicit instantiation (`template void X<...>::F();`), as for 0x51E760 (Netshare243).

## /Ob2 inline decision rule (Inline248, read from C2.DLL's P2 inline routine 0x1072D288 and confirmed in lab units)

- Every function has an IL size `s` (its own unexpanded body, call nodes included; lab: a constant global store ~6,
  an empty body ~12; a caller's S ~18 plus its statements).
- Root budget = max(1000, 2*S(caller)); a running total starting at S stops all further inlining past 35000.
- Found in the rebuild (lane R42): the cap's signature in a long dispatcher is a clean cut-over in source order. In
  TActionClass::operator() 0x6ADAE0 (7,752 B) the original expands every TAction handler up to
  TAction_BEGIN_AI_TRIGGERS and calls every later one out of line from TAction_STOP_AI_TRIGGERS (0x6B22B0) on; the
  rebuild's text cut over three handlers later (from SET_TEAM_INFANTRY_RATIO), so its earlier expansions total about
  three handlers' IL less than the original's. A cut-over that moves by whole sites while every expanded body matches
  points at the bodies before the cut (wordier in the original), not at the handlers after it.
- Sites are decided in evaluation order. N_i = inline-candidate sites in the same body from this one to the end,
  itself included (calls with IL available, including empty or later-rejected ones; not virtual or declared-only).
- Reject when s > 40 and s > budget (also depth limit, argument-count mismatch, the 35000 cap). Callees with s <= 40
  are free: always expanded, never charged.
- On accept: if s > 40 subtract s from the budget; then decide the callee's own body with budget / N_i (integer) at
  depth+1 and subtract what it consumed. The division compounds per level (Study152's cost*(1+W) was a linear
  approximation; the "waiting sites" rule is N_i).
- Practical: a tiny charged ctor out of line inside an inlined chain means the budget reaching that level was tiny;
  pending extra sites only divide one level, while each charged inline helper level subtracts its size and divides
  again. Out-of-line probe calls only raise S (the root budget). Measured sizes: CoordStruct ctor 41-52,
  CoordStruct::operator- 71-88. Lab tools: build/class-work/Inline248/lab/.
- Stack packing: wrapping a function's main part in a scope block (an if/else or a guarding if) decides VC6's frame
  slot sharing; a unit's .data must reproduce the original exactly, including statics between used ones (Scroll250).
- Auto-inline size cap (Inline252, lab with real CL): a function NOT declared inline is never auto-inlined above about
  175 IL units (27 constant stores expand, 28 do not; 23/24 with parameter stores), whatever the caller's budget; an
  `inline` function of 252 units still expands. So a callee the original keeps out of line with budget to spare is
  probably wordier in its source than ours. Unit sizes measured in a caller: global store 6, out-of-line call
  statement 4, explicit pointer cast 1; `T *x = new X; Add(x);` is 2 smaller than `Add(new X)` when Add takes T const&.
- Type-count sensitivity (Hdr306; replaces "header hygiene", Span268): a few functions pick between equal-cost
  instruction orders by VC6's internal allocation order, so any new declared type earlier in their unit (a new function
  signature, member or free, or a new function-pointer typedef; not comments, names or a reused signature) can flip them,
  non-monotonically (1-2 new types flipped 0x428370 at 7d270e22, 3-4 did not). Known victims: 0x428370 (path estimator,
  until 29d3eb48), BaseClass::Read_INI 0x42C460. The full-build refusal now names the victim row and first differing
  offset; reproduce by scoring that row in its concatenated population. Fixes, in order: land the header when the victim
  is exact with it (other landings move the count); give the victim's unit the real declarations its own code needs; a
  unit-local declaration in the one population that needs it. Never pad a header with dummy declarations (steering).
- Union members vs pointer casts (Event274): reading an event payload as `*(long*)&Data.Bytes[n]` instead of a union
  member (`Data.LongsCell.Long1`) adds indirect memory expressions that change operand order and scheduling much later
  in the same function (EventClass::Execute's TIMING sum). If a late expression resists every local rewrite, look at
  how earlier code in the function accesses memory.
- Dead stores (Tactical287): VC6 keeps two stores to an address-taken local when the second goes through an inline
  member (`rect.Y = a; rect += Point2D(0, b);`) but merges plain field writes; a "dead store" in the original points
  at an inline member operator. `Rect rect = Rect(x, y, 60, 30)` stores the constants through a register.
- Score in file context (OwnrDraw286): a draft scored as the first function of its file can allocate registers
  differently from the same draft after any other function; always score inside the population file in address order.
- MAX macro (Scenario289, Session275): Westwood's MAX(a,b) expansion repeats the expression (`a > b ? a : b`); a local or
  a template max compiles differently and does not match.
- __fastcall argument scheduling (Scenario291): in a __fastcall function, the register-argument setups after a
  callee-pops call are scheduled before the pushes; "/G5 scheduling ties" around calls in a free function often mean it
  is __fastcall. Also: a single new global `extern` in tag.hpp/id_source.hpp shifts a load pair in BaseClass::Read_INI
  0x42C460, while new function/member declarations do not - put new extern objects in a unit-local header.
- Measured IL sizes (Inline293 lab sweep): CoordStruct(int,int,int) 47-52; UnknownCoord434820(x,y,z) 41-46 (with inner
  ctor 89-94); XYZ_Coord 41-46; Cell_Center 47-52; old Coord_Snap 53-58; Union<Rect> 275-280 (Westwood's int() casts +6);
  TibSun Rect::Size() <= 40 (free). Arguments are decided before the call they feed (Cell_Center before Coord_Snap).
  Coord_Snap is the UnknownCoord434820 wrapper (out-of-line pair 0x434820 over 0x41B630), which puts the CoordStruct
  ctor one charged level deeper - that, plus TibSun's `pixel = f(coord); pixel -= Point2D(...)`, matches all six
  Tactical Render passes.

## Dead byte spill of an object flag (Cell351, 2026-10-31)
A spilled byte of an object's bit-0 flag that is tested once and then dead comes from `bool is_x; if (object != 0)
is_x = object->flag_bit0; if (object != 0 && is_x ...)`, not from register pressure. Seen in Cell 0x471660/0x47BB30,
TechnoClass Detach 0x6D2D10, 0x6F69F0; still open at 0x477B55, 0x477C87, 0x477EF8, 0x47805B, 0x47842A, 0x49B76B,
0x62A8A6, 0x62AB79, 0x6D70AF.

## Element loop versus memset (Cell360, 2026-10-31)
VC6 turns `for (i = 0; i < N; i++) a[i] = 0;` into `lea edi,a / mov ecx,N / rep stosd` (lea first), while memset emits
mov ecx first. When the original clears an array with lea edi before mov ecx, write the element loop.

## Calling convention of a no-argument callee (Netshare383, 2026-10-31)
Declaring a no-argument callee `__fastcall` changes the caller's scheduling even though the callee's bytes do not
change: after the call, a following tail call's register arguments are set before the epilogue pops, and the callee's
result is copied to its callee-saved register at once. Probe by overlaying the callee's declaration as __fastcall: a
shape-100 result whose only miss is the unresolved mangling means a rename spec to __fastcall fixes the caller. Fixed
netshare 0x5CB560 (98.97 over three lanes) and 0x5C34F0 (97.37). Other "scheduling tie around a call" plateaus may fall to it.

## A global read twice, through eax (Netshare390, 2026-10-31)
When the original loads a global HWND/pointer into eax, pushes a call's arguments and then copies eax into a callee-saved
register, the source is `local = global;` followed by an inline call that reads the same global itself (or a first call
that takes the global directly and later calls that take the local). VC6 merges the two loads into eax. Fixed netshare
0x5C67C0 and 0x5C71F0 (87.67, and its inventory bound was short). Stack-frame sharing follows the 2x rule (Netshare387):
VC6 reuses a slot for a non-overlapping object only when it is at most twice the slot's current size, so a frame that is
too large can need an earlier, larger buffer (0x5C7570: `char name[128]`, any 125-200 matches).

## Inline helper with a const-reference argument (Netshare390, 2026-10-31)
Eight repeated add-row/set-item-data pairs in 0x5CA6C0 come from one file-static inline helper. Passing the node pointer
by value scored 97.00; passing it as `NodeNameType * const &` is exact (the reference keeps the caller's load order).

## Calling-convention probe order (NearMiss392, 2026-10-31)
The __fastcall lever also applies to the function itself. When a free function's only miss is register-argument setup
(mov ecx/edx, lea ecx) after the pushes, or a push/mov ecx tie right after a call, probe before rewriting statements:
(1) the function's own convention as __fastcall; (2) each no-argument callee just before the tie as __fastcall; (3) a
free "object pointer" callee as a member (thiscall). Five of about twelve probes hit: 0x711E60 (callee 0x428AC0),
0x519E70 (TechnoTypeClass::Load_Palettes static __fastcall), 0x516E70 and 0x768C70 (self; 82 -> shape 100), 0x542460
(self plus 0x5DA360 as OptionsClass member). Missing @refs for string literals look like a convention miss on scores.
Follow-up (NearMiss395): four more self-__fastcall hits, all free no-argument functions (0x718060 voxel matrices,
0x6F5B10, 0x5C84E0 with its callee 0x5C8420, 0x683BF0). Ghidra cannot tell a no-argument __fastcall from __cdecl,
so every unaccepted no-argument free function is a candidate: probe the convention before anything else.

## Fragile 0x73FEE0 is $L-label renumbering (ConvSweep396, 2026-10-31)
Refresh_Wonline_Players 0x73FEE0 breaks (refused at +0x10A2) because a new declaration in a header it includes
renumbers its compiler `$L` labels: measured lobby.hpp +2, pcx_cache.hpp +7, owner_draw_dialog_control.hpp +1. Put
new declarations for those areas in new headers or behind a guard macro (pcx_cache.hpp PCX_CACHE_DECLARE_LOADS).
Also: populations marked separate_units (1640, 1643, 2179, 2795) need that flag in the plan or the composite compile
reports "accepted 0"; a joined unit cannot take a VECTOR_IMPLEMENT_*/guard macro defined after the header was first
included (move such functions to a separate-units bag); every string literal needs its own @ref in the plan.
The no-argument __fastcall lever pays on recorded near misses (8 of ~20), not on raw seeds (1 of 292).

## Inline budget in practice (InlineLab398, 2026-10-31)
- The Inline248 rule holds in every lab here; two corrections to lane folklore: (1) "small" callees are often
  charged, not free: the HEAD Cell(short,short) ctor is 41-42 IL units, the one-line 0x54FAF0 index 49-50, XY_Cell
  53-54, so a `<= 40` free callee is rarer than it looks; (2) s is the callee's unexpanded size, so a wrapper that
  only calls one large inline stays free and its callee is decided one level deeper.
- Measure before reshaping: lab/g2.py (Spend = k stores + b calls: 2-unit resolution) for callee sizes; in place,
  replace the callee by k stores (the "fk" sweep) to read the nested budget at each site, and add m empty inline
  calls before the final return to read N_i (the flip m gives R_i and N_i from two callees of known size).
- When the original keeps a charged callee out of line at early sites and expands it at a late one, the early
  sites see (R - s_parent)/N_i with a large N_i: the natural fix is one more charged level above it (a small
  inline accessor wrapping the call), not a wordier callee (0x565EB0: `Cell_Subzone(cell, layer)` around the
  clamped index; the callee would otherwise need ~120 units).
- When one of several identical statements has a different inline pattern, first write them all the same way:
  the budget alone often produces the difference (0x5629D0's fourth Get_Height_GL).
- Westwood's Cell is TibSun's `Cell : TPoint2D<short>` with `Cell(int x, int y) : BASECLASS(x, y) {}` and
  TPoint2D member operators returning `TPoint2D<T> const` built with `T(...)` casts: an operator then costs ~64 with
  a free nested base ctor, while the HEAD forms cost 80 or ~60 + a charged Cell ctor. This model makes
  Destroy_Low_Bridge_Span 0x556BF0 and the COMDAT 0x56AA40 byte-exact; adopting it needs ten accepted re-shapes
  (InlineLab398 NOTES "Whole-tree check").

## Auto-inline size cap versus budget (Map405, 2026-10-31)
A callee that is expanded although the original calls it may simply be under VC6's auto-inline size cap: the cap
applies to the callee's unexpanded IL, independent of the caller's budget (padding the callee with k 6-unit stores
flips it at a fixed k however many calls the caller gains) and of where the callee is defined. The natural fix is the
original's wordier body: Spring_Low_Bridge_Destroyed_EW 0x55EA50 goes out of line when its walk writes TibSun's
Is_Low_Bridge test out (`CellClass *cellptr = &Map[tmp]; overlay = cellptr->OverlayIndex; ... while (overlay >= 0x4a
&& overlay <= 0x65)`) instead of calling the bridge predicate. A hand-written `tmp + AdjacentCell[f]` costs ~16-20 IL
more than an Adjacent_Cell call; by-value vs reference, enum vs int, `!= 0` and early declarations cost nothing.
(macOS `seq 0 -1` prints "0 -1": guard k=0 in probe scripts.)

## Function-local statics in a shared .rdata (Iso416, 2026-11-01)
VC6 puts a function-local `static float const` into the unit's .rdata together with the header-static constants, so
one data section holds symbols the original placed apart. The audit verifies such a section per symbol (each bound by
@ref, tiling the section, bytes equal at its own original address) since 53f108001; bind every symbol, including the
local static's decorated `_?_name@?M@??func@Class@@...` name.

## Frame order in very large frames (Iso420, 2026-11-01)
The size-ascending slot order does not hold near esp: the most-referenced objects go first (in the disp8 range)
whatever their size (`ini`, 80 bytes / 93 refs, sits among the scalars in Read_Control_File 0x529870). Beyond that,
ties among equal-weight spills are unstable: independent of declaration order and names, but sensitive to which
temporaries are in the sort (removing one CDTimerClass temporary moves the other). Tools: Iso420 t2/fas.py, fcmp.py,
fref.py compare every frame variable's offset against the original.

## Macros versus inlined calls (Span427, 2026-11-01)
When a header expresses a helper as a macro, check whether the original was a function VC6 inlined: the audio event
code (0x403EB0-0x4068F0) is ordinary /Ob2 C calling Audio_Events_Clear / Audio_Event_Stop / Free_Stopped, which match
byte-exact as calls; the audio_event.hpp macros are not the original shape and their expansion is too big to inline
where the original does (Handle_Stop_Loop needs Stop written as Detach + Release_Samples calls).

## Join reload order follows definition order (Reinf437, 2026-11-01)
At a join, VC6 reloads spilled values in the order the variables were *defined*, latest first: Do_Reinforcements
0x639970 matched when `bool atwaypoint = false;` was defined before `bool drop_pod` and assigned later
(`atwaypoint = wp != -1;`); moving only the declaration changed nothing. Also: `while (true)` stays unrotated where
`for (;;)` gets rotated; a BN_CLICKED test written `(unsigned short)(wparam >> 16) == 0` gives `shr reg,16; jne`.

## "Handwritten assembly" claims need re-checking (Scout444, 2026-11-01)
Two functions our headers labelled handwritten assembly, LCW_Uncomp 0x534B50 and AnimFile::Decode_Frame 0x4249F0,
are Westwood C that compiled exact almost at once from the TibSun/Westwood source (`__fastcall` LCW decoder; TibSun
animfile.cpp). Before parking a row as assembly, compile the TibSun/Westwood C; only genuine inline-asm bodies
(LCW_Comp 0x534D40, XSurface::Fill_Rect 0x7743C0, BlitForward 0x4861C0/0x487080) stay out.

## Boolean conditional forms and bitfield order (Cell450, 2026-11-01)

- `flag = (x == 0) ? true : false;` and `flag = (x == 0);` are not the same to VC6. The plain comparison kept `x`
  live in a register and shifted every spill slot (Set_Under_Bridge 0x4732D0: 76.95 against exact with the
  conditional form).
- Consecutive bitfield assignments hoist their shift/mask temporaries in statement order: match the original's
  store order first.
- A member defined later in the same file still inlines into earlier callers (CellClass::Adjacent_Cell 0x476AC0
  into the bridge functions); do not move definitions to force or prevent it.

## if/else nesting changes stack packing (Mech459, 2026-11-01)

Early returns and the equivalent nested `if/else` blocks do not give the same frame. In Mech Movement_AI 0x5919A0,
wrapping TibSun's "no destination" case and the head-to-set path in `else` blocks cut VC6's locals from 0x6c to the
original 0x5c (+4.7 points). Keep the reference source's nesting before trying declaration-order changes. Also:
TibSun inline.h Direction written as `int x1 = coord1.X; int x2 = coord2.X;` reproduces the original's X-only stack
copies exactly.

## Float-returning helpers, switch-table bounds (Mix462, 2026-11-01)

- An inlined helper's return type moves FP-store scheduling even when no float store appears: Percent_Value 0x7200C0
  returns float (TibSun _Parse_Percentage), and only that gives WarheadTypeClass::Read_INI's fstp-after-pushes order.
- New rows that end in switch tables need `body_bytes` in the plan item; diff_candidate's score counts the candidate's
  tables as extra instructions, so judge such functions by their code lines. diff_candidate's default length is the
  inventory row: pass `--length` with the true extent before calling a function a miss (Loco276's "Ratio member breaks
  0x6F0530 (76.19)" was only a short bound; with --length 255 it is exact).

## Locomotor shapes (Teleport464, 2026-11-01)

- Two vtable calls in an if/else (`if (x != zero) slot59(x); else slot59(Object_Coord(LinkedTo));`) get the
  original's tail-merged `push; call [vtbl]`; a ternary argument does not.
- Named locals fix frame layout in long locomotor bodies: `CoordStruct target = center + offset;` before a member
  store, `Cell where = Coord_Cell(...); CellClass *cell = &Map[where];`, separately scoped pointer locals per block.
- The FootClass flag idiom `bool foot; if (o) foot = o->flag; ...` reproduces the byte spill `mov [esp+N], al`.
- (Egos466) An inline helper's body form changes the caller's branch layout when inlined, even if both forms are
  exact out of line: TibSun's `Y -= amount; if (Y < -20) return true; else return false;` for EgoClass::Advance
  0x4B3670 fixed the Egos scroll loop where a bool-local form did not.

## Named results move stores (NearMiss469, 2026-11-01)

- `int length = Get_String(...); if (length == 0)` lets the two inlined CRC temporaries share a frame slot (0x5014);
  `if (Get_String(...) == 0)` keeps them apart (0x5028) (INIClass::Get_Unicode 0x50EDF0).
- An inline helper that names its returned value (`Cell adjacent = ...; return Map[adjacent];`) moves the copy's store
  after the push and `this` setup at every inlined caller: a change to one helper body re-scores all its inliners.
- RA2 Direction-style code subtracts in ints and negates: `int dy = to.Y - from.Y; atan2((double)-dy, (double)dx)`
  gives the original's `sub; neg`.

## Switch cases do not share temporaries (Loco468, 2026-11-01)

Lab-confirmed (build/class-work/Loco468/lab/t1-t3.cpp): unnamed temporaries in different `case`s of one `switch` never
share a stack slot, while the same code as plain `if`s does, and temporaries inside inlined helpers share even within
a switch. In a big state-machine Process (Rocket 0x63DE90), routing each `LinkedTo->Center_Coord()`-style temporary
through a file-static helper (Ground_Distance, Direction_To, Distance_To) cut the frame from 0x9C-0xAC to the
original 0x84. A real arithmetic helper (`static inline int Square(int)`) adds inline sites and can push a later
constructor out of line, as the original did (CoordStruct ctor 0x41B630 in 0x63DE10's inlined copies).

## Merged calls and member-ctor store barriers (NearMiss474, 2026-11-01)

- Two `htons` calls in an if/else get merged by VC6 into one call, and that merge is what reads an int global as a
  16-bit word; a single call on an unsigned short local always loads a dword (UDPInterfaceClass 0x76CD40).
- VC6 never moves a store above an inlined member constructor: Point2D members act as a barrier for the zoom double's
  low-dword store in the Tactical ctor 0x6A24B0, while plain ints/bools let it rise to the top. Member types can be
  read off store order.

## Shared tails and hoisted loads (Rocket473, 2026-11-01)

- In a switch, two cases with identical endings get merged; the merge can pull post-switch code up behind the
  earlier case. Writing a test as `exploded = f(x); if (!exploded) {...}` instead of `if (f(x)) { exploded = true;
  break; }` kept the case-5 block apart (Rocket Process 0x63DE90).
- When an object pointer is chosen by if/else, VC6 loads a member first and then hoists the load both paths share; a
  single-use local in each branch restores the hoist, one local for every use spills it (labs Rocket473 lab/t6-t7n).

## Dialog procs (Opts476, 2026-11-02)

- VC6 lays out switch cases by value, not source order; source order still changes the inline budget.
- `HWND h = GetDlgItem(...)` as a named temporary, not a nested argument, fixes GetDlgItem call order and registers.
- Of two duplicate error blocks, VC6 keeps as the shared tail the one whose branch has an explicit `return`.
- (Near481) VC6 reorders ungrouped FP sums; only explicit grouping fixes product order: Closest_Object 0x5D5090
  matched with double locals y, z, x and `x * x + (y * y + z * z)`; none of 36 ungrouped orders did.

## Callee conventions and TibSun helpers (Wol483, 2026-11-02)

- A parameterless callee's calling convention moves the caller's epilogue scheduling: Wonline_Main 0x73A300's
  `xor eax,eax` lands before the pops only when unknown_close_all_dialogs 0x753F50 is declared __fastcall.
- Where TibSun calls SendDlgItemMessage and RA2 compiles GetDlgItem + SendMessageA, the original used an inlined
  helper (Send_Dlg_Item_Message); TibSun's inline accessors (MultiMission::Get_Official) fix load order.
- (Abuf490) Tail merging depends on the unit: identical masked/opaque case tails never merge in a one-function file
  but do merge after the unit's earlier functions in the joined population (unvq.cpp 0xA000 case). Standalone
  "tail-merge plateaus" can be scoring artefacts: always rescore joined (HALF 87.5 and 4x2 TRANS 88.2 became exact).

## DynamicVectorClass::ID loops on Count() (Port500, 2026-11-02)

Westwood's DYNAVEC.CPP and TibSun vector.h write `for (index = 0; index < Count(); index++)` in
DynamicVectorClass<T>::ID. HEAD had used `index < ActiveCount`, and that caused the long-standing "count/vector ecx/edx
swap in the inlined ID after an inlined Add" plateaus: PhoneEdit Dial/Add, the HouseTypeClass ctor and
RulesClass::Sides all went exact once ID called Count(). The 144 explicit-instance units still compile `ActiveCount`
under `#ifndef VECTOR_IMPLEMENT_DYNAMIC_ACCESSORS`, so they emit no Count COMDAT. Rescore any miss with an inlined
ID/Add register swap.

## How /Ob2 refuses an inline site (Plan512, 2026-11-02)

Lab-confirmed (build/class-work/Plan512/lab/L1-L9): each call site is decided on its own. A callee expands when its
own source body fits the caller's remaining allowance, and that allowance shrinks with every expansion already made.
Sites are processed in forward order (see "Nested inline sites" below for the exact rule): an inline call placed after a site, even an empty one, pushes that
site's inner callees out of line; the same call placed before it does nothing. The callee's body size is what counts
(named int locals and struct copies count: four named ints flipped Planning_Centered_Rect, two did not); padding the
caller or parent helpers does nothing. To refuse a helper where the original calls it, give that helper a larger,
still-exact body (named locals), then rescore every accepted row that uses it (Plan512 x/fam.py).

## Nested inline sites: the full /Ob2 rule, validated (InlineLab518, 2026-11-02)

Inline248's C2.DLL rule is complete; it also explains Plan512 ("reverse source order") and Plan515 ("~145 units,
nested formula open"). 88 fresh predicted lab cases, all exact at their flip points (build/class-work/InlineLab518
lab/h1.py, r1.py random 4-level trees, v2.cpp), plus in-place probes of Draw_Node 0x619870 predicted to the unit.
- Sizes: s(f) is f's own unexpanded IL; callee bodies never add to it. Root budget B = max(1000, 2*S(F)).
- Sites of a body = calls to functions with IL (inline, in-class, non-inline functions of the unit under the ~175
  auto-inline cap), in evaluation order (an argument before the call it feeds), a loop body once. Over-cap,
  declared-only and virtual calls are not sites and count nowhere.
- For site i with N_i = sites from i to the end of the same body (i included): refuse iff s > 40 and s > R (costs
  nothing, its body is never examined); else charge s if s > 40 (s <= 40 is free), then decide the callee's body
  with (R after the charge) // N_i and subtract whatever that consumed.
- So a depth-1 site depends only on earlier accepted depth-1 charges. A nested site depends on later sites of every
  enclosing body (they raise N) and on its parent's own size (charged before the division). An inline call placed
  after a parent starves the parent's callees; the same call placed before only lowers R if it is charged.
- Unit costs (2-unit resolution): empty body 12; constant store to a global array element 6, scalar global 4;
  parameter store 8; extra parameter 2; `f();` 4, `ext(1);` 6; `return k;` 2; named local `int a = k; use a` +6
  over the direct form; `if (g) store` 14; struct copy through a local 12, direct 6. operator== as
  `if (...) return 1; return 0;` 72, `return a && b && c;` 62 (int) / 60 (bool).
- Reading a plateau: if the original refuses a charged callee and then accepts later charged callees at the same
  depth whose sum exceeds that callee's size, the refused one is one level deeper in the original (a free wrapper,
  an inline accessor or a constructor chain), never a different statement order. Draw_Node: the out-of-line
  0x41B630 at both CoordStruct(-1,-1,-1) sites needs TibSun's `Coord(int x, int y, int z) : BASECLASS(x, y, z) {}`
  over TPoint3D<int> (RA2_COORD_TPOINT3D: wrapper 44, TPoint3D ctor 48 refused at depth 2, call bound with
  `@ref ??0?$TPoint3D@H@@QAE@HHH@Z=0041b630`), and Member_Owner/Planning_Techno_Color (refused at R = 828) need a
  colour helper; with the 62-unit operator== (also exact at 0x61A8B0) the whole call sequence matches. The planning
  population stays 168/168 with RA2_COORD_TPOINT3D and that operator==.
- Tools: InlineLab518 x/meas.py (callee size by a charged Spend flip), x/model.py + run.py (simulate a call tree and
- Applying the rule (Ob2Apply519): the IL size of an out-of-line callee is itself a lever. A byte-identical rewrite of 0x54FAB0 (its result held in a local through an if/else-if/else chain, +13 to 18 units over the two-return forms) moved where Collapse_Cliff 0x562F50's nested decisions flip.
- A delete of a class with an inline empty destructor adds IL but no code, in the nested-if form only; the && form emits a ??_G call (Free_Buildup 0x45E920). Measure a missing spend with Ob2Apply519/prb.py (remaining-budget probe for a joined function).
  compare with the compiler), x/inplace.py (many variants of one function in one unit compile), x/probe.py (the
  remaining budget at any source line of a real function).

## Loop tails and known-value threading (Scout526, 2026-11-02)
- **Separate loop exit (`jne top` plus its own return copy).** Write one shared `return x;` as the last statement of the loop body, reached by positive nesting. Example, from Get_Next_Port 0x5B9F10: `if (transport && mode == 4) { if (!Open) continue; Close; } return port;`. The exhausted path is a return after the `while (tries--)` loop. If the body has two separate early returns, VC6 tail-merges the exit into `je exit; jmp top`. The same loop is inlined in Get_Mangled_Port 0x5BAF50 and Detect 0x5BA2C0.
- **Known-value threading.** VC6 threads a failed `x == 1` sub-test straight past a following `x == 2` test. Where the original still makes the second comparison, assign `x` again between the two ifs (Get_Allocation_Delta 0x5BAC50).
- **Register plateau in repeated loops (codex1 r847, Collapse_Cliff 0x562F50).** Three identical register-allocation diffs, one per half-2 loop, went away when the loops shared one `row_width` local that each loop reloaded in the original's order. The /Ob2 inline decisions had already been fixed through the 0x54FAB0 IL-size lever (Ob2Apply519). Treat a repeated per-loop register diff as a sign of one shared local.

## Frame slots and unsigned loop ends (Scout531, 2026-11-02)
- Unused stack bytes can be the home slot of a struct whose other members stay in registers. In Render() 0x7178E0, a named `Rect rect` filled member-wise keeps X, Width and Height in registers and spills Y into the Rect's own slot. That reproduced the original's 0x34 frame.
- Loop strength reduction keeps a signed compare (`jl`) for every index form: int, unsigned, size_t, an unsigned bound, `!=` and `<=`. Only a pointer compare in the source gives `jb` (Convert_Voxel_Colors 0x716FC0: walk a pointer to each entry's middle byte, with the output pointer declared first).
- Stack-slot sharing: a shared slot can grow to at most twice its size. Making one 4-byte local a 6-byte struct let a 12-byte object share its slot, which gave Detect 0x5BA2C0's 0x188 frame. To diagnose slot problems, compile the joined unit with /FAs and read the frame map (Scout531 t/fa.sh, t/fab.sh).

## Late-day mechanisms (Scout528/530/532, 2026-11-02)
- **Coupled register choices (Detect 0x5BA2C0).** Two distant register diffs can be one effect. Writing `result |= relative ? 0x20 : 0x10;` as `int kind; if (relative) kind = 0x20; else kind = 0x10; result |= kind;` gives the same neg/sbb code but a different temporary order, and that fixed both diffs.
- **FP operand order.** VC6 re-sorts flat `+` chains, but parenthesising one product changes the order it picks (Matrix3D rows, Scout530; +4 to +20 points).
- **Extern re-emission.** VC6 emits a global's dynamic initializer again after every later `extern` redeclaration in the same file. Put such externs in a header the unit includes before the definition. Avoid widely shared headers: random2.hpp changed Pick_Lobby 0x744090.
- **Joined scoring.** A by-value return whose copy constructor is defined earlier in the population auto-inlines. operator*(Matrix3D, Matrix3D) scored 64 standalone and was exact joined, so always score such rows joined.
- **Jump thunks** compile from a one-line forwarding function. Rows that start inside the alignment pad before the true entry hide real functions (Scout528 t/early.py).
- **A member array of a global struct gets strength-reduced; a plain global array does not** (Multiplayer_Debug_Print 0x526D90: frames-behind as session +0x28EC, indexed by the same i as `this->Connection[i]`).
- **Init-list copy vs body assignment.** `List(other.List)` in the ctor init list differs from assigning in the body by two bytes in the clear loop (copy ctor 0x6025A0).
- **/Ob2 levers from the late passes.** A forwarding-only explicit specialization is a free site: later callers inline it and then call the refused callee. A typed `delete` on a class with an inline empty destructor adds a later root site, and one level less of inline depth can win back a refused sibling site (0x5EDB20).
- **Reference locals force a reload.** `int &slot = ActiveSlot; if (--slot < 0) slot = 31;` stops VC6 reusing the compare's load and gives the original `dec eax / jns` (LineTrail::AI 0x539530). The order of member stores then decides which local holds the node in ebp.
- **Guarding a declaration for one joined population.** Define the guard macro in that population's header-statics source. diff_candidate scores only the original's instruction span, so read `--full` when a probe adds calls after it.
- **Repeated bool tests merge.** VC6 always merges two consecutive tests of the same bool unless a different test sits between them, and its later passes do not fold that test away (Queue 0x6279D0, Scout536; a guard that only keeps the second test is steering, so it is a resume point, not a fix).
- **A named source local moves `this`** (Copy_Nodes_To 0x613550, Scout539): `TechnoClass *owner = Owner; PlanningMemberClass *source = node->Member_For(owner);` moved `this` from edi to esi, where it shares the register with the Owner load. This is another case of the coupled-register rule.
- **Inline refusal depends on the remaining budget.** Adding a free inline call after a site can push a nested ctor out of line (PaletteName::Load 0x603380, Scout539 hv/t1). The 40-unit threshold is not the deciding factor.
- **Near-tied register webs follow real use counts** (Scout540). Register ties between two webs, such as Get_Input's this/input and Add_Compressed_Events' index/storedsize, are decided by use counts after optimization. Declaration order, reference copies and dead initializers never break them; only a real change in uses does. If the bytes show no such use, the tie may be unreachable from source.
- **Check @refs before shape work.** MapSizeGameOption::Describe 0x728AB0 gained 3.6 points from a missing `?MapSize_Labels@WorldDominationTour@@3QBQBDB=007af140` @ref.
- **Rect copy-init vs direct init.** `Rect r = Rect(...)` keeps X/W/H in registers and stores them late; `Rect r(...)` stores them early. In Draw_Radial_Indicator 0x451BF0 that was worth 22 points (Scout543). Also bind FP constants by @ref: 0x79D168 is 1/2π and 0x79D160 is 3π/2.
- **Field-address registers in pointer loops** (Scout542). If a loop reads `p->Field` right after `p` is assigned, both on the loop-entry path and at the loop bottom, VC6 keeps `&p->Field` in its own register, and that one extra register shifts the whole function's allocation. Testing the field before the pointer copy and looping with do/while avoids it (LineTrail::Draw 0x5395C0, 65.64 -> 96.94).
- **Inline wrappers change evaluation order** (Scout541). Inline operands are evaluated before plain calls, and two inline operands are evaluated right to left. A temporary that is the destination of a `sub` tends to get ebx, while a read-only temporary gets edi (Get_Obstacle 0x4BBAF0).
- **Pushing ??_G out of line.** Two more /Ob2 sites after a nested `delete` make ??_GWideString go out of line. An explicit specialization defined after an earlier inline use is called out of line. VC6 emits nothing for an explicit specialization of an in-class member that nothing calls (control registry 0x5DEE00).
  - Found in the rebuild (lane R54, 2026-11-20): the same site falls out of line when an earlier charged sibling is wordier, with identical code. In the registry atexit 0x5DEE00, ~WinData expands Set_Wide_Text(0) before `delete WideText`; writing the setter Westwood-style (`if (Text != NULL) { delete [] Text; Text = NULL; }`, `if (text != NULL && *text != L'\0') Text = new wchar_t[wcslen(text) + 1];`) compiles to the same bytes everywhere but costs more IL, and ??_GWideWstring is then called (0x601430). The length-local and `operator new(... * sizeof(wchar_t))` spellings were too small; four dead-branch statements were enough (probe only). Look at the spelling of the sibling sites before the refused one, not only at the site count.
- **Double lvalue in `?:` is copied as integers** (Made_A_Kill 0x713360, Scout549). When a double lvalue is the false arm of `?:` (MIN), VC6 copies it as two dwords; an rvalue arm (an inline returning double, `+x`, `x * 1.0`) is reloaded with fld/fstp. TibSun's `MIN(Experience, Veteran_Cap())` form matched.
- **Growing the root's own body restores a site** (tracker 0x4128A0). Writing `edge.X = center.X + radius;` instead of `+=` enlarges the root, and since the root budget is twice its size this brought back an Append_Bin inline site that a bigger Shift_Bin had pushed out.
- **Inline wrappers as register levers (evaluation order and rotation).**
  - An inline argument is evaluated before a plain call. An identity inline `Rect const &Wall_Rect(Rect const &r) { return r; }` around the first Union argument gave the original's call order (Wall_Update 0x475960, Scout550). This form is a hypothesis: it is not TibSun-shaped, so replace it when a natural source shape turns up.
  - A file-local `static inline` wrapper around every SendMessageA call adds code-free temporaries that bring the scratch-register rotation into step (Fill_Port 0x66D5A0, Scout551). Dead or no-op statements never help, because VC6 drops them before it assigns registers.
- **A dead 3D temporary keeps its 12-byte frame slot** even when its Z stores are removed (Wide_Area_Damage 0x47F6E0, TibSun 3D-coordinate Distance, Scout552). An inline argument temporary bound to a const reference keeps its memory home and its stores, while a named struct local loses both (IonBlast AI).
- **Discarded results keep inline bodies.** `Union(a, b) - origin;` with the result discarded keeps the partial inlined Union body and supplies cheap inline sites that push other callees out of line (Wall_Update).
- **Rebuilt temporaries reorder struct stores** (UnknownBeacon 0x42DD60, Scout555). `dst = CoordStruct(src.X, src.Y, src.Z);` is not a no-op: it reorders the member webs, which moved Z to esi and Y to edx as in the original. The inline-wrapper lever was neutral there.
- **Guarded atexit means a class static** (Scout557, Scout559). An atexit that tests and sets a guard bit, or a thunk that only sets a guard bit, is the destructor callback for a class static data member whose type has a user-declared destructor (empty in Scout559's case). A plain global of the same type gets a bare `ret`. Declare such statics under a unit macro: unguarded, they renumber `_$E`/`_$S` in every includer. A guarded declaration takes one `_$E` number from the whole unit.
- **Small late levers** (Scout557, Scout560).
  - `Cell const &offset = table[i]; cell + offset` gives the cell.X-first add.
  - A bool local read before an unrelated store keeps identical stores in two branches from merging.
  - A static inline helper in an argument list is expanded before the argument pushes.
  - A rebuilt temporary used as an operator operand moves member-load choices across statements, not only within its own statement.
- **Final-pass levers** (Scout561-563).
  - A static inline bounds helper adds an /Ob2 site after a `new`, which pushes a nested VectorClass ctor out of line (Collect_Boundary_Cells 0x56ED10).
  - A member-wise difference stored Y, X, Z, from a copy-initialised rebuilt `from`, splits the loop-sum fix from the subtraction load order (Find_First_Obstacle 0x4BB890).
  - A lone float stored at frame offset 0 of a 12-byte slot is the X member of a 3D local; `Vector3 facing; facing.X = cos(...)` gave IonBlast AI the original 0xB8 frame (84.41 -> 96.30).
- **One-use float copies set x87 operand order** (IonBlast AI 0x521340, Scout566). In a commutative fmul of two memory floats, a float local used only once is loaded first. `float c2 = facing.X; float s2 = facing_sin;` just before each fabs test matched the original's operand order. VC6 propagates int copies away but keeps these float copies.
- **One-use float copies also move integer scheduling** (operator*(Matrix3D, Vector3) 0x591380, Scout572). Reading all nine rotation elements into one-use float locals placed `mov eax, ecx` where the original has it.
- **Comma-assigned pointer in a short-circuit condition** (IonBlast AI 0x521340, Scout571). When an expression is read twice in a condition, writing the second read as `(voxel = expr, !voxel->field)` moves that load's register, and the change carries into the next statement's pointer copy.
- **Push vs mov after a call** (PaletteName::Load, Scout574). The original puts `mov reg, eax` before `push imm` only when the call result lives in a register; ours pushes first. Resume point: find accepted matches that show the mov-first order.
- **Correction to the use-count note (Scout570).** The Add_Compressed_Events 0x627B60 esi/edi tie was broken by the order of independent initialisations, not by use counts. Moving `units = 0; whom = 0;` from the declarations to just after the memset changed where their register lifetimes start, and the function became exact. Try initialisation placement before concluding that a tie is unreachable.
- **One-use field copies stop alias reloads** (Execute_DoList / Process_Receive_Packet, Scout579). Copy a field that is read before several stores into a local used once (`int response = ...ResponseTime`). This removes reloads the compiler makes because it assumes the stores may alias, and can flip function-wide register choices.
- **Late joins change earlier drafts.** A helper accepted later and now joined into a row's unit can start auto-inlining into rows drafted before it; for example 0x62E5C0 now inlines into Process_Receive_Packet. Rescore old drafts against HEAD, and refuse the helper's inlining with an exact, larger rewrite landed in the same plan. Also: diff_candidate does not flag missing string @refs, so run the strict audit.
- **Single-member explicit instantiation emits nothing** under /O2, without a warning; `template class` emits every member. A template member therefore lands with its real caller (Scout580).
- **Byte-identical rewrites that add IL are a budget lever** (Options_Menu_Proc 0x4DD8A0, Scout582). Measure the missing /Ob2 budget with probe pads, then close the gap with natural forms that compile to the same bytes but carry more IL: TibSun store-then-override, nested ifs, named pointers, explicit `!= 0`. Never land the probe pads.

## Float chains and MAX macros (Scout585, 2026-11-03)
- VC6 sorts a flat floating-point `+` chain and reuses repeated subexpressions across it, so neither the term order nor an inline helper stops the reuse. A miss where the original recomputes a squared term (railgun 0x60C050) needs a different evidenced shape, not reordering.
- An expanded TibSun-style MAX macro (`a > b ? a : b` written out at the site) gives `add reg,-N` for a constant clamp, where a helper call gives cmp/branch forms.
- A local `Random2Class &rng` reference held across several random calls changes the register assignment (spark 0x60B670 62 -> 78.8).
- Matrix temporaries (Scout588): VC6 keeps the two Rotate_Vector temporaries separate when the Slope_Vector result is assigned, and shares the matrix temporaries when the slope matrix is assigned in its own statement.
- Including a header that defines VECTOR_IMPLEMENT_* renumbers a unit's guarded-static labels ($S42 -> $S64), and acceptance refuses the result as unbound .bss. Move a global the unit needs into a small header of its own instead (frame_rate.hpp for unknown_a6eb1c, ParticleClass 0x609D10).
- Import-call wrappers (Scout587/589): a code-free file-local `static inline` wrapper around an import changes which callee-saved register caches that import. Get_Item(HWND, int) around GetDlgItem matched 0x466A50 (98.98 -> exact; +4.4 on 0x634670, +13.8 on 0x6340F0). Send_Message around SendMessageA matched 0x596540 and Fill_Port 0x66D5A0. Wrap the import that appears in the tie, every site first, then single sites. The wrappers are neutral on loop-index ties, and in functions near the /Ob2 inline budget they make things worse. They are hypotheses, not Westwood shapes.
- Session members vs aliases (Scout591): reaching a member through the session object (`unknown_session_a3d298.Players`) or through its global alias changes alias reloads and the register choice in an inlined DVC Add. Try both.
- A branch that ends in `break` to a shared tail and a branch that repeats the tail and returns leave different blocks after VC6's tail merging (Net_Join_Dialog 0x5BD070: the guest path repeats the tail).
- A new parameter in a widely included prototype can change unrelated functions (lobby.hpp: Pick_Lobby 0x744090 and Print_CRCs 0x62A360 by 4 bytes each). Moving a prototype into its only caller is neutral.
- Command dispatch (Scout593, Get_Join_Responses 0x5C08A0, 83.7 -> exact): one if/else-if chain over the command lets VC6 keep the compared constants in callee-saved registers across the loop. Separate `if (cmd == X) { ...; continue; }` blocks give the same block shape but different constant registers.
- A dead counting loop (`if (!strcmp(...)) dups++;`, TibSun) keeps the loop and its dead stack store. `x = strcmp(...)` or an empty if removes the loop.
- Reading a bitfield once through the global alias and once through the session member gives the original's separate dword and byte loads.
- diff_candidate does not flag an unbound string literal; acceptance refuses it. Add @ref for every literal the original references (`","` 0x7CE9C0).
- Declaring an import `__declspec(dllimport)` (timeGetTime in Dropship_Screen 0x4A6B90) gives the original's `call [imp]` and register caching of the import pointer: +21 points. A plain prototype calls a thunk.
- Aggregate initialisers instead of memset for local arrays gave the original's stores (+3.3, 0x4A6B90).
- Classes whose RTTI mangling is PAV must be declared `class`, not `struct` (ButtonFadeEffect, CrossDissolveEffect).
- A function-local static with a destructor makes the unit emit its `$E` atexit thunk. Acceptance refuses an @ref binding of that thunk, so the thunk's row must be taken into the plan (0x5C01D0).
- A static inline accessor used as a sprintf argument is evaluated before the other pushes and moves that load into a callee-saved register (0x5BC940).
- Refused first inline site (Scout596, Skirmish_Command 0x6829F0, 78.89 -> exact): when the original calls a helper at one site and expands it at the others, and no IL probe at depth 1 refuses it, move that case into a static helper (depth 2) followed by enough free root sites (here code-free Get_Item wrappers) for the budget to refuse it. `#pragma auto_inline(off)` also refuses it, but breaks the other sites.
- A value read into a local before a constructor call (`mode = Session.Mode;` before `WideString message;`) is held in a callee-saved register across the call, which also fixes later frame-slot order.
- Nested call arguments: when the original pushes a constant after an inner call, make the inner call's result a temporary.
- A free inline function from a header that an earlier function in the unit expanded stays expandable; a member-call form avoids it (`cliprect.Intersect(Get_Rect())` instead of the free rect_intersect.hpp `Intersect(a, b)`, DSurface slot 13).
- A const-reference local to a field (`unsigned long const &received = rec->ReceiveTime;`) reorders the operand loads of the sum it feeds (0x524870, 98.50 -> exact).
- A cached constant register lives until its last source use. TibSun's `if (SUCCEEDED(result)) { ...; result = S_OK; } return result;` keeps the cached zero alive to the end, so it stops sharing a register with a pointer defined late. An early `if (FAILED(result)) return result;` frees it (TechnoTypeClass::Load 0x6DE260, 99.64 -> exact).
- SIB operand order inside an inlined callee is set at the call site, not in the callee: VC6 canonicalises the callee's address tree. Read_INI 0x44AB80 got [ecx+edi] by calling through a pointer copy (`BuildingClass *building = b; building->Turn_On();`).

## What counts as an /Ob2 site (Scout614, 2026-11-03; measurements in build/class-work/Scout614/NOTES.md)
- A site is any call of a user-written function that has IL in the unit, implicit calls included. Each of these is one site: an inline accessor or wrapper; an explicit temporary; a local declared without an initializer whose type has a user default ctor (`CoordStruct c;`); a user-written copy ctor or operator=, also in `T v = f();` and by-value arguments; a converting ctor (base-copy conversions included); a default argument that builds an object; an inline operator (`+=`, `==`). A local with a user dtor adds one site per dtor call, plus one for its ctor.
- None of these is a site: compiler-generated copies, assignments and by-value passing; derived-to-base conversions; field reads; declared-only calls; release `assert` (TibSun's compiles to `((void)0)`).
- An accessor in place of a field read adds exactly one free site and 2-6 root IL units. Most recorded "N sites short" misses come from the original calling user-written inline functions, mostly the position accessor (Position_Coord_004f3f40 / Object_Coord, TibSun Get_Coord/Center_Coord), where our source reads the field. Example: Aircraft 0x416420 under the ComPtr299 comip model, 63.48 -> exact by passing Position_Coord_004f3f40() instead of CoordStruct(Coord). A second family is user ctors, dtors and conversions of value types (Scout605's Coord crossings).
- Not every deficit is a site count. A deficit measured in IL units (Ship Draw_Matrix about 170) or a very large one (Unlimbo 60-70) points to a missing inline level instead.
- Scout614 also ruled out a user-written Vector3 copy for BuildingLight: the original's integer moves are the implicit copy.
- A function-local DynamicVectorClass makes the unit emit its vtable. Acceptance refuses that .rdata unless every slot of the DVC vtable and its base VectorClass vtable is bound by @ref, read from the original vtables (TechnoClass::AI: 0x79D0AC, 0x79D0CC).
- A local DynamicVectorClass whose stores come out as Vector, IsValid, vptr, GrowthStep, ActiveCount, IsAllocated, VectorMax is the ctor followed by an inlined `Clear()` (TibSun's `ActiveCount = 0; BASECLASS::Clear();`, its `if (Vector && IsAllocated)` folds away). VC6 keeps those stores in IL order. The Clear call costs one /Ob2 site, so another later root site has to go (UnitClass::Read_INI 0x706AE0).
- A 12-byte frame gap whose only spilled member is Y is a Rect built with the size calls as ctor arguments (`Rect rect(cell->X, cell->Y, type->Width(), type->Foundation_Height(false));`, DisplayClass 0x499390). The same gap is in 0x4991D0.
- Loop bound shape decides hoisting: `y < ymin + ysize` (with `int ysize = 2 * adj + rect.Height;`) is hoisted into the outer loop's preheader after the guard, which puts the bound in a register and leaves x in memory. A named `ymax` computes it before the guard (Passes_Proximity_Check 0x4991D0).
- TibSun's `Coord(int x, int y, int z = 0) : BASECLASS(x, y, z)` wrapper, built in place for offsets, adds one free /Ob2 site per use and fixes add operand order (Aircraft Fire_At 0x415790). A DVC-style example of Scout614's user-ctor site family.
- Read_INI 0x49CFD0: the surface size is `Get_Width() * Get_Height() * Bytes_Per_Pixel()`, and a CoordStruct built member by member (`homecoord.X = ...`) fixed a later loop's register choice. Including building_type.hpp instead of building_type_class.hpp pulled in an extra dynamic initializer that acceptance refuses.
- Dead frame bytes can be an unused member of a by-value temporary: Fire_At's 8 bytes are the Z of a 24-byte BulletVelocity built through TibSun's copy ctor.
- Braces are IL (Scout626): removing the braces from four one-statement ifs took Sidebar C850 from 168 to 162 /Ob2 units with identical code; an early return gave 158. Write braces as TibSun does when a budget is tight.
- A call refused inside an inlined copy in another function names the callee the source really calls: Recalc 0x6802D0's inlined C850 calls 0x6820F0, which reveals C850's own source.
- Free callees (40 IL units or less) are never refused, even at allowance 0, so a callee the original refuses somewhere is over 40.
- A local's scope decides whether it takes a dead parameter's stack slot: a Cell declared inside `if (best == 0) { ... }` reuses the dead `point` parameter slot, while one at function scope gets its own (Tactical 0x6AA990). Splitting a Point2D into two ints gives the original's scalar slots.

## The per-unit declaration counter (Scout627, 2026-11-03; measurements in build/class-work/Scout627/NOTES.md)
- VC6 numbers symbols, new function signatures and `$L` labels with one counter per translation unit, and some c2 tie-breaks follow that number mod 64. Each `struct X;`, `extern int x;` or `void f();` counts 1, and a declaration with a new signature counts 2. Names, comments, declaration order within a block, and anything declared after the row do not matter.
- Print_CRCs 0x62A360 (three reload-order sites; 20 of 64 offsets are good) and Refresh_Wonline_Players 0x73FEE0 (one site; 48 of 64 good) depend on the counter at their own function. Every declaration before them in the joined population moves them, so include order cannot harden them. When a header change breaks one, land a natural re-roll of the victim's own body in the same round. Check header copies first with build/class-work/Scout627/fragile_probe.py.
- Pick_Lobby 0x744090 depended on where INIClass and CCFileClass are declared relative to each other. Including ini_class.hpp and cc_file_class.hpp first in the wonline unit hardened it (9c4945b18).
- Static data members (CDFileClass's five) declared before a unit's initializers advance its `$E`/`$S` names. The plan's initializer symbol_prefix values must follow them, or acceptance refuses `.CRT$XCU`.
- Seed_Lake 0x57E100 (82.45 -> exact): the grow block as its own `if (success && origin == Cell(0,0)) {...}` after the whole success/else chain gives the original's layout (grow block after the return tail). The spread loop is `while (n < count) { if (node == 0 || !success) break; ... }`, not if plus do/while. An inlined helper written as one float expression (`return a*Rand()+b*s-c*t;`, no double casts or named locals) leaves the random fraction on the x87 stack as the original does.
- Empty inline functions (`static inline void f() {}`) are not /Ob2 sites, even 140 of them; a probe body needs at least one store to count (Scout637).
- A bool-returning accessor gives `shr; test cl,1`; the original's `and eax,1` out of line and `test ah,1` inline mean it returns an unsigned int bit (CellClass IsUnderBridge 0x4788D0).
- Two TibSun `min()` calls supplied the two later /Ob2 sites that make the original call operator+ 0x42ADD0 out of line (Repair_High_Bridge_Span 0x54B560). Fix the call sequence first, then the frame.
- A variable reused as a walk cell whose address goes to an out-of-line operator+ is not merged with a later copy's stack slot. A loop `if (flag) { if (++n >= 4) return; } else break;` gets its own epilogue copy (Scout640).

## Loop-invariant step values (Scout645, 2026-11-03; lab in build/class-work/Scout645/lab/)
- In a loop that stores to an address-taken local (a walk Cell), VC6 hoists the X/Y values of `AdjacentCell[step & 7]` into registers only when `step` is a `?:` of two constants; it tracks the select's value set. When `step` comes from an if/else diamond (`if (c) step = S; else step = E;`), or from a load, call or bitfield, it hoists only the address and re-reads both members on every pass. The diamond is if-converted after the loop optimizer, so it emits the same `neg/sbb/and/add` bytes as the `?:`; only the loop body tells them apart. `int step = E; if (c) step = S;` keeps a branch instead. This gave Destroy_Train_Bridge_Connections 0x553570 (61.84 -> exact) and Repair_Bridge 0x555A60 (63.60 -> 99.75). scan.py lists other original loops with a hoisted address.
- One named local re-read before each test breaks a this/parameter register tie (TechnoClass slot 174 0x6C1380, Scout644).
- VC6 reuses an incoming parameter's stack slot for a later temporary once the parameter is held in a register for the rest of the function (0x553000 `dir`, 0x5539B0 `cell`). If ours gives the temporary its own slot, the parameter is probably not kept in a register.
- Membership tests as plain loops with a `found` flag and the target on the left (`target == list[k]`) thread the original's branch; an inlined ID() compared with -1 does not. Reading `T *item = list[i];` before the value test in a loop over local vectors hoists both Vector pointers and stops VC6 turning the loop into a countdown (TechnoClass 0x6D4400, 72 -> exact).
- Commutative 16-bit Cell sum operand order flips with the function's count of named locals mod 8; position does not matter, a local that is copied away does not count, and declarations before the function have no effect (Scout648, Scout650). Survey an operand-order residue with a probe `Cell probeN;`, then find the natural local; unused probes must not land. `wchar_t *wname = ...` before each call fixed IPX Service's function-wide register rotation.

## The per-function operand tie (Scout651, 2026-11-03; lab in build/class-work/Scout651/lab/gen4..8)
- A commutative tie flips on one phase of key = T + offs mod 8, and the whole compile, frame and registers included, repeats with period 8 unless the /Ob2 budget moves. T counts parameters and referenced locals in any scope, c1 temporaries (a `(Cell)` cast, `f(a+b)`, assigning a Cell return), and one per distinct field of a local struct named directly (`walk.X`). T does not count never-referenced locals, statements, calls, statics, earlier declarations, or one-return inlines such as Facing_Add. offs comes only from inline expansions earlier in the source text; layout does not set it, so a block laid out after a statement can still be written after it. Survey with dead locals and dead inlines; dead probes must not land. The c2 structure behind mod 8 is not identified.
- `Delete(T(id, false))` builds the temporary in the argument and schedules its store as the original does; a named local with the same ctor does not (0x566C30).
- Two Cell-sum mechanisms (Scout652): X-type sites follow N mod 8; Y-type sites ignore N, dead probes and `(Cell)` casts and flip by parity along the function, with each preceding wrapper result copy. Symmetric edits to two families cannot flip only one; an asymmetric source is required (OverlayClass Mark 0x5DA970).

## Locomotion Process and While_Moving forms (Scout654, Hover 0x4FBFC0, 0x4FCBC0)
- A named `int const &result = ok;` used for the first test stops known-value threading of `ok` past later tests. Only a named reference does this; an inline with a const-ref parameter or a value copy does not.
- An X,Y-only copy of a moved Coord is a Point2D move step: `TPoint2D<int> moved = ...; coord.X = moved.X; coord.Y = moved.Y;`. The dead Z goes and the frame comes right.
- A helper that takes the exit coordinate by reference and reads `count` inside, before its call, shares the register the way the original does; the same read in the caller does not.
- An unreferenced row after a switch table needs its predecessor's extent on record, so it lands one round later, with its definition inline in the predecessor's draft for that round.
- Natural free /Ob2 sites that make As_Cell and operator== refuse inside Start: Under_Bridge, Abs, Object_Coord accessors and TibSun's Is_Complete_Turn with its operator- and operator>=. No pads were needed.

## Key probes, refined (Scout653)
- A k probe moves only sites after it, which narrows the earlier "position does not matter" claim. `int q; q;` is a pure T probe with no IL; `int q = 0` also moves S and hits budget flips.
- Copy-propagated locals add nothing to T; `(Cell)` casts do.
- A site stable over every k is an initialisation-order problem, not a key problem: try moving each `= 0` first (0x5629D0 went 82.86 to exact with `int found = 0;` before `xx`).

## Constant webs and Coord differences (Scout655, Ship 0x676F60)
- `(Object_Coord(LinkedTo) - dest_coord)` gives `mov esi,edx`; `movement + Object_Coord(LinkedTo)` gives `mov ebx,edx`. An unexplained register copy around a Coord difference is usually one more inline level.
- A cached constant (-1 in edi) is a conflict rule, not a type rule: compare types do not detach it, but a real pseudo at the difference peak or live across a call evicts it. Constants compete: a second zero use at the head caches zero in edi and moves -1 to ebx.
- TibSun's Point2DStruct: `Point2D(Point2DStruct const &)` built through a nested call reads Y first. `(double)X*(double)X...` Length gives the original FP order.

## Inline budget levers (Scout656)
- `Cell temp(first);` in a small local swap helper is a charged site; `Cell temp = first` is smaller and misses the window. `current.X++` instead of `+= Cell(1,0)` removes sites, lowering N so a larger early charge fits.
- An inline member-wise operator= makes a struct assignment a charged site with no code change. Static helpers are free sites that only raise N.
- The first nested accept can have zero margin: one added site anywhere later flips it, even a spend probe, so the charge must come from existing sites or helper sizes.
- One shared int across disjoint loops gives a shared frame slot; block-scoped locals do not. Naming a count before a call lets `add ebp,0x30` coalesce with the dying pointer. Two side steps written the same way (`facing - 4`) CSE as in the original.

## Frames, FP and statics (Scout657)
- Frame order follows object size: an 8-byte Point2D sorts before 12-byte objects, and a `CoordStruct delta` with Z dead ties with them (0x608440). An inline value helper such as TibSun's `Random_Double(a, b)` moves a `fild` temp into the first-dead argument home; a named int does not.
- Register setups before pushes in a free function mean `__fastcall`, even with no callers (0x53F490).
- Inserting a function with local statics renumbers later `$S` and `_$E` symbols, which shows up as "Unsupported .bss" and "Unaccounted code". Newly emitted inline slots need @refs too.
- To break FP product CSE, use casts in the first sum and fresh double locals in the second; casts or locals in both sums CSE the squares. `double angle = a; angle *= b; angle *= c;` keeps operand order and stops the `fst` forward.
- `dir += x` then `Normalize(particle->fc)`, not `Normalize(dir)`, gives fstp plus reload. `fld st0; fld mem; fcompp` needs a float-typed value and the limit read through an inline returning double.
- T-phase interaction: naming a local can fix a sum order and move a later tie; dropping another referenced local restores it (0x60C050).

## Loops and charge (Scout658)
- TibSun pointer walks can be VC6 strength reduction, so write the real index loop. A vector used in the loop is read before the guard; one used only in the guard sinks.
- A non-inline callee near the 175 cap gives a narrow IL window: more charge per site, and two more locals stop it expanding at all.
- A local helper taking the record and a const& gives a charge without a global operator= changing other rows. Naming values in a helper raises its size, so a later ctor site can be refused (0x565E20).
- `rowx += count; x = rowx` rotated three long webs in 0x563D80.

## Byte reads, blend loops, wparam (Scout659)
- A bool at a 4-aligned struct offset read through the struct object gives `mov ecx,[x]; and ecx,0xff`; the extern bool alias gives `xor; mov cl`; offset +1 is a plain byte read.
- Blend loops: compute `dest = &buffer[out]` inside the loop, write `int in = y*pitch + start_x` before the branch, and `out++; in++` at the bottom with `x` only as the counter. That puts the `lea` after the guard and gives the original induction order (0x68CF90 went 84 to 99.55).
- `unsigned short code = wparam >> 16; if (code == 0)` gives `shr; jne`, not `test reg,0xffff0000`. `int value = (unsigned short)(wparam >> 16)` plus a second `(unsigned short)(wparam >> 16) == 1` keeps wparam intact.
- Hypothesis: scratch registers rotate eax, ecx, edx and the rotation persists to the function end. Code-free temps did not move it, so adding a wrapper is not a reliable lever. Declaration order fixes store order but all six orders left frame slots unchanged on 0x6340F0.

## Y-sum parity and small forms (Scout660)
- Y sum parity ignores k probes. A named pop slot (`CellClass **slot = &stack[count]; cur = *slot;`) plus `visited[..]++` flips it, `= 1` does not; known-zero threading keeps the `++` store a plain `mov 1`.
- A register plus a write-through home can be two variables (zoneptr/zonerow, like TibSun's x/rowx). A pointer assigned before `1 << n` lands in edx because ecx holds the count. `Rect bounds(0, 0, w, w)` after the other inits delays `xor ebp`. `walk -= Cell(1, 0)` gives `dec word` plus a rewrite of the other coordinate. Writing Cell arithmetic through the real operator- raises the /Ob2 charge at the natural place.
- Rewriting a low draft to TibSun flow or correcting it against the disassembly pays more than variants (0x564290 +24, 0x569320 +35).

## Free sites and helper size (Scout661)
- TibSun's one-line `Get_Cell_Data` is IL <= 40, so it is a free /Ob2 site; a long cast form was charged at every use, and that alone flipped a loop Insert inline (0x586340). Inline decisions are monotone in the helper's size.
- `float &first_key = nodes[0].Key;` fixes store order, as in Create_Resource_Patch.
- An inlined copy can differ from the out-of-line row: Grow_Water's score copies use TibSun's RMG_RANDOM_DOUBLE2 order, while 0x582970 keeps `weight*heading+rand*jitter*unit`. A separate inline helper fixes the fxch family.

## Layout, loops and refusals (Scout662; tool Scout662/t/fa.sh + fm.py prints a frame map with reference counts)
- Frame slot order follows reference counts. A join statement sinks arm tails but drops the variable's frame refs by one.
- `switch (found) { case false: S; break; default: F; }` lays F first with S earlier in source; plain if/else and goto do not.
- `for (;;)` and `do ... while (true)` rotate the loop, `while (true)` does not; an inlined `if` in the head stops the rotation.
- Reading a field twice keeps a one-use pointer local alive while CSE still gives one load.
- One extra inline depth moves a nested refusal: an accessor at the same level as 0x54FA60 took 0x566340 to 18 of 18 calls.

## Threading and slot weight (Scout663)
- `if (placed && success)` with a named bool keeps `success` in memory, because threading would have to cross the placed test; it also fixes block layout.
- `while (queue.Count != 0) { if (queue.Count >= 1020) break; ... }`, then the drain loop, then `if (processed > 1) return true; return false;` gives the shared false tail through threading.
- A rotated top-register tie unmoved by k probes was init order: `int attempt = 0` after the random offset.
- Rows accepted in another population can expand through file-local `inline` definitions in the draft, with no allow_split.
- Stack slot order is weighted by references that survive to final code; code-free references do nothing.

## Cross-jump and callee-saved swaps (Scout664)
- Reading both coordinates of a const& into named ints before the arithmetic keeps Y in a callee-saved register and can flip a whole-function callee-saved swap (0x566340).
- In a cross-jump the first copy in layout stays unless a later one falls into the join. Scheduling runs after tail merge. A rotated loop, or a second call at the bottom, adds one frame ref (the copy's lea) without changing the tail. Loop-bottom tests known true fold before tail merge, so the join block stays empty.

## In-place Cell arithmetic (Scout665)
- `tail += Cell(a, b)` and `bias -= Cell(...)` in place of TibSun's field assigns fix eax/edx choices, the literal `mov word [m],0` and the zero CSE; the unused `Y += 0` keeps the temp live.
- `short offset = down ? forward : -forward` replaces TibSun's `backward` local. Recomputing `second.Y - first.Y` instead of naming height keeps abs in eax and gives `lea eax,[ecx+1]`.
- TibSun's int margins are needed on the Plan rows: Rect margins reach shape 100 with the wrong frame order, and int margins also flip the operand tie.
- Correction to the k-probe note: a referenced local (`Rect const &area = rect`) flipped a `[rect+8]/[rect]` load tie that k probes did not.

## Labels, duplicated tests and RMW (Scout666)
- A label's return block moves to just after its last `goto`, even one that folds. Gotos before the last site share one block; plain `return`s after it get inline epilogs.
- Source can duplicate a test: giving each distance arm its own `if (range > 0 && dist > range) return false;` took 0x6C5570 from 80 to 91 and fixed frame slots and ebp.
- 3D `fild` order depends on the key: `double y = c.Y` first, or any extra referenced local, gives Y,Z,X.
- `*x += y` uses three instructions when a scratch register is free and `add [m],r` when none is, so an RMW mismatch means a register web difference, not a statement form. High-byte tests always load a register, so a lone read can look like a CSE web.

## Refusals by counting, constants and self-copies (Scout667)
- To refuse a ctor, count the inline sites after it: survey with throwaway `Nop()` sites, then replace them with real one-line helpers, or move IL into helpers to lower the root budget. Real rows defined later in the unit still inline (Map_Clamped_Gaussian).
- `double const &h = data.f; data.f = h;` keeps a self-copy as integer moves; a plain self-assign vanishes.
- One shared `tries` int gives the shared frame slot. A named `position` before `Map[]` fixes an eax/edx tie.
- An implicit `DynamicVectorClass<double>` needs an @ref for each emitted COMDAT.
- A small `differing_bytes` can be a wrong constant binding the score does not show: 15.0 exists at both 0x7a5398 and 0x7a5ce8; cmp.py plus report.json find it.

## Scratch rotation and duplicated statements (Scout668, 0x6C9260)
- Scratch registers advance round robin through eax, ecx, edx; ecx is skipped when reserved for `this`, and al byte loads do not advance it. This confirms Scout659's hypothesis.
- Tail merge runs after register allocation, so a cross-jumped copy still advances the rotation. If an if arm ends in `jmp` and the register sequence after it is shifted by one, a statement is duplicated in both arms. That took Can_Fire from 82.58 to 96.77 with the land test in each arm.
- Which arm uses `return 5` and which `goto ILLEGAL` picks the surviving copy. Nesting the rest in `if (weapon != 0) { ... return 0; } CANT: return 6; ILLEGAL: return 5;` keeps CANT last with no folded goto.
- A memory `test byte [m],imm8` of a parameter changes the register piece of its later refs.

## Threading, tails and constant registers (Scout670)
- `int limit = f; if (f <= 0) {...} if (limit <= 0)` keeps both tests; two tests on `limit` thread away.
- An inlined real row shows up as one shared cross-jump tail. All four case tails merged only once a combined `||` return was split into two ifs.
- `while (true) { ...; k++; if (k >= 3) break; }` gives `jge out; jmp top`. A plain named temporary (`int id = over[i];`) moves the scratch rotation by one.
- 46 repeated `X = -1` stores put -1 in a register, and later uses go through it, including inline strlen; pushes alone do not.
- Dropping named locals lowers S and can stand in for one more inline site.

## Region and ramp forms (Scout669)
- `(second-Cell(span,0)).X-first.X-5` through operator- keeps the word subtracts on X-field rows; North also needs `bias.X =` rather than `bias +=`.
- `Rect rect2 = rect1` gives register copies. `for (; tries < 200; tries++) { if (result) break;` keeps a head test that do-while rotates away. A single flat do-while with `||` restores frame and index CSE.
- `if (fail) x = false; if (x && y) {...}` as two statements stops a cross-jump. Repeating `Is_Valid()` in both arms keeps a second test that nesting threads away.
- The block scope of one Cell can decide constant caching across a whole function, so test scope before constant probes: `Cell end` declared in each loop body took 0x587330 from 78.56 to 99.06.
- Unused file-local inlines are not budget sites. Count with Nop probes, then replace them with real helpers.

## Forward substitution and threaded flags (Scout672)
- Tree forward substitution is block-local and runs before threading. A local defined in the block before a known-result flag test stays a web, even though threading later merges the blocks: `bool valid = false; int first; if (a && b) { first = start.X; valid = true; } if (valid && end.X - first >= 6)` gives a right-operand-first `sub`. Helpers, casts and negations all normalize to one tree.
- Rotation model: pure eax/ecx/edx rotation where a register still live as a later operand's source is skipped.
- Twin arms need not share a form: each read order is what the rotation at that arm needs (0x582FD0).
- A read through inline `Get_Cell_Data(cell->MapCoords)` keeps the second Y read uncached and the cell pointer in ecx.

## Argument hoists, tails and local statics (Scout671)
- A call inside a ctor argument hoists the whole argument before the pushes; an inline wrapper hoists only the call (0x47C3A0 went 83.85 to 98.91).
- A row written out with loops can exceed the auto-inline cap; its form with helper calls inlines into later rows.
- `lowcolor = X` in each arm plus one `highcolor = lowcolor` after the if/else gives the shared tail; `highcolor = lowcolor = X` does not.
- In a function-local static table, leading constants stay in .data and everything from the first non-constant element on is stored by code.
- shape_file.hpp in a unit without an empty rect row needs SHAPE_FILE_EMPTY_RECT, or the unit gets an extra initializer and acceptance refuses .CRT$XCU.

## Statements first, doubles and literals (Scout673, 0x57ECF0)
- Fix statements first: Seed_River's frame and `this`/zero registers came right on their own, and slot packing followed reference counts.
- A hand-written copy of an inline shows as left-to-right evaluation and a plain variable pushed as ints.
- A second named double keeps a value on the x87 stack (`fst m; fld st(0)`). `fst dev; fadd lo` versus `fstp dev; fld lo; fadd dev` follows declaration order of dev relative to lo.
- A constant left of a compare after folding needs the row written `mean-deviation > maximum`. The same double at two addresses is an extern const double. A dword load before a byte store needs a named int.
- A 1 ulp wrong literal scores 100 but fails acceptance with "Unsupported allocatable data section"; check literals with Scout673's rd.py.

## Second-site inlining and facing ties (Scout674, 0x4F4070)
- A wrapper one level down moves a callee out of line at that site only; it was the only form that did (Scout642 had ruled out charged units there).
- `int n = 0` placed next to a loop init coalesces the zero with the index, which put the index in memory as in the original (73 to 96.57). Reusing locals across blocks fixes frame slots. A live named local holds a register across an arm, so dropping it matters.
- diff_candidate scores unbound string literals as fine but acceptance refuses their .data: rows with strings need the @ref, and /FAs gives the mangled name.
- Constant-facing tie: the first two distinct facings give `add dx,[global]`, the rest `add dx,[esp]`; with three distinct the set decides, and sets containing 0 or 7 stay all global.
- `T result(rect1)` is not the same as `T result = rect1`. A false epilog copy whose next statement is adjacent is removed; one whose next statement is far away stays.

## Case order and spills (Scout675)
- Switch source order follows TibSun (E, W, S, N on Waterfall), not numeric case order: reordering alone took a draft from 22.55 to 50.47.
- `mov byte [eax],0` beside a zero cached in ebp is forced, because ebp has no byte form; it is not a separate statement.
- A spilled pseudo shows as defs `lea ebx; mov [m],ebx` and reads `mov eax,[m]; add edi,eax`. If the original spills a pseudo ours keeps in registers, look for a statement diff first: Seed_River's equivalent came right only after its statements did.
- Arms must start with a differing statement, or identical X/Y stores hoist above the `if`. `for (int side = 2; side != 0; side--)` gives the `jmp` over the loop-top `xor`.

## Dead slots and statics (Scout676)
- `Cell x = Cell(0,0)`, unlike `Cell x(0,0)`, leaves a dead temp slot, which made the 0x4F03C0 frame size exact.
- Early Adjacent_Cell sites refuse op+ through R//N while late sites inline it, so in-function site order alone explains mixed call and inline sites.
- A function-local Cell table with an empty atexit thunk implies an element type with an empty dtor, built from Cell temps; the frame carries one temp per element.
- An inline helper taking double parameters fixes int-to-double evaluation order that an int-parameter helper gets wrong. A reused byte slot can be one bool reassigned.
- Gap: frame order follows sharing groups, not single-variable reference counts. The Study42 sort rule did not predict the 0x4F03C0 layout.

## Promoted temporaries and CSE rank (Scout677, 0x57FF20 from 58 to exact)
- `Rect pre(0,0,0,0)` then `pre = Rect(...)` in each arm makes a promoted temporary, which decides which field pseudo spills; field assigns do not.
- Repeating an expression instead of naming it makes a CSE temp that ranks differently from a named local, and that decides which pseudo wins the register. `x_offsets[direction / 2]` twice with no index local gives the `shl eax,2` CSE.
- `Rect a = src; a.X += 1; a.Width -= 2;` gives `inc` and `sub r,2`; ctor arguments give `add r,-2`. `center = Point2D(a, b)` fixes read order. `base + Cell(-1, 0)` and `base - Cell(1, 0)` differ.
- Rows ending in switch tables need table-inclusive body_bytes.

## Flag reuse and aliasing (Scout678, 0x4F2350 from 88 to exact)
- `flag = !(a && b && ...); if (flag) {B} else A;` places B after the next unconditional jmp and lets A fall into the join; plain `if (!cond) B else A` normalizes and does not move it. Scout678's scan.py lists matched rows with out-of-line arms that jump back.
- A store through a reference blocks later loads from scheduling above it; a direct this-member store does not.
- Aliasing is flow-insensitive: an address-taken local keeps store order against pointer loads even when the address is taken later.
- The threader sees through named bools, copies, inline helpers, joins and De Morgan forms; only a path with the flag unknown keeps a test.

## Frame maps and slot sharing (Scout680, 0x4F03C0 from 63 to exact)
- Scout680's fa.py (a /FAs frame variable map with ref counts and first references) and rm.py (raw original-to-ours slot map) replace blind frame hunting.
- Slot sharing groups reveal source objects. `Point2D delta` with field assigns replaced two ints and explained `neg; neg`: X stays in memory, Y is forward-substituted. Wrapping a body in `else { }` lets a local share a temp and changes a static's mangled scope. Function-block address-taken objects overlap everything in the block.
- Tied temp slots came out right once one temp's ref count was right. `Map[Adjacent_Cell(...)]` without a named cell fixed registers.
- A member rename must use mangling_rewrite: a plain `renames` entry leaves symbol_prefix stale.

## Second frame sort, ternaries and hoists (Scout679, 0x529870 and 0x6D4920)
- Above about 0x80 local bytes a second sort orders slots by density (summed refs of the sharing vars / size). All-tie behaviour: for each lo, mid = lo + (hi - lo) / 2, swap A[lo] and A[mid]; lab/sim5.py reproduces N=35, 36, 37 and 46. Its input is the first-phase order (size, refs, last ref), so a char array's declared size inside its 4-byte rounding changes key order with no code change (`suffix[10]` vs `[12]`: 95.09 to 99.75). Test frame ties that ignore declaration order with array sizes.
- A function-scope buffer overlaps a pre-loop temp, so two loop temps share a slot instead.
- A ternary of named int locals (`i < n ? full : empty`) stays a real conditional until const-prop, so the setcc runs before the arg temps; constant arms fold in place at the push (60 to 91).
- `dec` after a loop guard means a hoisted value: write `for (j = rows - 1; ...)` inside the outer loop, not a precomputed count.
- Commutative imul order can flip on an unrelated temp elsewhere in the function; source operand order does not reach it.
- diff_candidate shows nothing for what acceptance refuses unless bound: the `""` literal (.bss COMDAT), every string, vtables, ??_G, GenericNode and ??1INIClass COMDATs, and `_$E`/`$S` shifts in later rows. Scout679/cs.py lists code sections with no ref.

## Hidden results and budget sites (Scout681)
- `lea ecx,tmp; push y; push x; call` with no result pointer pushed is `__fastcall` with the hidden result in ecx and a by-value struct on the stack; it looks like a thiscall ctor.
- `bool techno; if (p != 0) { techno = p->IsTechno; } if (p != 0 && techno)` explains a dead byte store.
- Double locals inside an inline Length hoist ahead of the rest of the expression and give dot-before-sqrt order. An inline Percent_Chance reads the Rules value before calling Random.
- Coord_To_Pixel with its result ignored expands with the compares dead; a site's depth decides which nested callees stay out of line.
- Point2D-local sums (`Point2D offset(0,2); a += offset; b += offset;`) supply pending inline sites and emit nothing extra. Find the count with Nop probes, then use real statements.
- The Print_CRCs counter depends on which real declarations land with a header change: over one header, k=2, 4, 5, 7 extra decls were good and 1, 3, 6, 8 bad.

## Constant webs by threshold (Scout682, Ship 0x676F60)
- Constant webs compete by use count, not by a pseudo at the peak (this corrects Scout655): one more zero use at the tail makes `xor edi,edi` win, which flips the head immediate and the edge defs. A literal 0 one inline level down counts; nesting it through a helper leaves a local `xor edx,edx`.
- A named `CoordStruct partial(0,0,0)` is block-live, and that alone fixed three slot groups.
- `double x, y, z` in that order with `x*x + (y*y + z*z)` gives X,Y,Z fild order.
- `Point2D offset = ptr[..].Offset` direct-init reads X first; assignment reads Y first. The right operand of != is evaluated first, so `Cell here = pos.As_Cell(); if (here != head.As_Cell())` gives left-first order.
- Scout682's om.py maps each frame object to its original slot, with push depth corrected. scan.py runs a source toggle across all 8 T phases.

## Label locals and inlined members (Scout683)
- A 4-byte local sorted into the 8-byte group is a Point2D with a constant field (`Point2D offset(0, rect.Y + 1)`), and `pixel - offset` supplies sites.
- A separately named pointer plus a repeated expression gives a CSE temp, so the pointer spills at once.
- A charge of about 54 to 130 before a site splits depth-3 sites; it must sit where it does not disturb argument evaluation (a wrapper on Fancy_Text_Print is exact, one on CC_Draw_Shape is not).
- A member with "no callers" may be inlined: WM_CHAR is really `if (flag) Put_Char(wparam)`, which alone explains a last-site call. Check such members before adding pending sites.
- A duplicated case tail can be fall-through. Mixed `jl top; jmp tail` is a for-loop with a guard, not `while (true)`. TibSun's `-30 + pixel.X + 30` is really `pixel.X -= 30; point.X = pixel.X + 30`.

## Frame placement and address-taken order (Scout684, Ship 0x676F60 from 86 to 99)
- Frame objects are placed in descending last-ref order, newest compatible slot first. One named `Cell gate = c.As_Cell();` made a temp share a slot and fixed the frame size.
- An address-taken local's store sits ahead of later reads, forcing a copy: `Cell a0 = ...; Cell b0 = ...; Cell newcell = ...; if (a0 != b0)` keeps the reads first.
- T phase depends only on the total count, not where a local sits, and the distance between two sites matters, not just the total. Identifier renames change nothing.

## Tactical forms (Scout685)
- TibSun's `LONG x, y` with `(int &)` casts raises S just enough for one more inline site, which merged a whole block. `-1 - 15*h` puts the product in a fresh register: name both products before the second call.
- The field choice decides which operand `add` writes. A CSE reload on an edge means an operand-order tie made a word load available from a known dword; fixing the tie brings in the reload and the rotation together.
- A named pointer with an unnamed `ptr->Type` CSE fixed an arm-by-arm rotation; a named `type` did not. Slot sharing across sections exposed one reused pair (`start`, `dims`): 65 to 93.
- Nested expressions in an argument can refuse where a named `Cell cell = ...` expands.
- `bool inrange` set in both arms keeps `setle; test; je`, and the tails cross-jump. `bool clear = f() == 0; return clear;` gives `test; sete`; plain `return f() == 0` gives neg/sbb/inc.
- Right-associated `X*X + (Y*Y + Z*Z)` reaches the x87 order; named doubles and left-associated forms do not. Promoted double locals keep the same order under any spelling.

## Address-exposed joins and copy order (Scout686, Ship and Drive to exact)
- Y and Z reads hoist over an X store only when coordinates come through a local pointer; a repeated member gives the original order, as its CSE temp keeps the struct copy in order. A copy from a CSE temp also survives copy-prop (`lea eax; mov ecx,eax`).
- `mov esi,eax; test esi,esi` needs the join variable address-exposed: `T *object = cond ? a : b; T *&ref = object; T *occupier = ref;`. `T *&`, `T * const &`, `T **p = &o` all work; inline reference parameters fold away. `mov R,eax; test R,R` is the default only after calls.
- A `do` head with a same-condition bottom threads away before copy-prop. T phase is exactly mod 8.
- Scout686's mini.py with mi.sh compiles a small standalone file with /FAs in about 3 s; it reproduced both residues outside the 5.7 KB function. Reduce to a small test when a residue survives many variants.

## Y order and control flow (Scout687)
- Y operand order can follow control flow, not T: `if (range <= 0) continue;` plus a `for` gives scan-first; dropping the guard so the `for`'s rotated guard does the `jle` gives adj-first. T-only probes never reach Y.
- `Point2D point = Point2D(a, b)` keeps the point in the frame; `Point2D point(a, b)` promotes it.
- A named `CoordStruct coord = bptr->Coord;` made 0x6AA300 exact.

## Hoisted loads and early returns (Scout688, Start_Of_Move)
- A member load used in an expression with a call hoists over the call, and that web can take the register the original gives another value. A named reference (`CellClass const &here = Map[...]; here.x + (...)`) fixes it: 62 to 77.
- Inline sites go monotonically with N: one form at seven sites gives refused (ctor, Length), then refused (ctor), then none.
- A pure temporary shares a dead slot and can fix the frame; a named Cell never does.
- `if (!(a != b)) return false;` lays the block far and cross-jumps. `A || B` return goes far; `A && B` stays inline. `if (!ok && !same) { Y; return false; } T; return true;` gives one block order.
- Scout688's seg.py is a call-aligned segment diff that ignores frame offsets; difflib under-reports on misaligned rows.

## Depth budgets and traps (Scout689)
- TibSun's explicit Absolute plus minus can be a Coord_To_Pixel statement with Absolute refused at depth 2; a copy keeps the pixel out of memory.
- For "A in, B out" at depth, with m = b - s_parent, it needs m - s_B < s_A <= m/2. A bigger S can raise B above 1000; charging spend and adding N are not interchangeable.
- `extern Rect x;` after `Rect x(0,0,0,0);` in one TU re-emits a dynamic initializer (stray `_$E85`, refused as .CRT$XCU #2).
- Identical repeated global stores are removed as dead, so zero-use probes must write distinct array elements.
- 0x6A2900 is exact as TibSun's `Vector3 position(point.X, point.Y, 0.0)`, which explains a Z-first store.

## Threaded else and exposed tests (Scout690, Start_Of_Move twins)
- Dropping `else nextface = facing` lets threading produce a far `jle`.
- A stop copy without `xor` needs an address-exposed test: `bool found = ...; bool &result = found; if (!result) {... return found;}`. Plain `if (!found)` folds to a constant and merges with the other site.
- A two-arm `if (train) cando = 0; else if (crusher...) cando = 0;` fixed a join reload and cleared several rotation residues at once.
- `FootClass *unit = LinkedTo;` before a test explains a load ahead of the test. A scoped block around a named `CoordStruct center` fixes evaluation order and frame slot.
- A fild order of Y, Z, X comes from a rotated Length with declarations y, z, x. An inline chain's symbol depth counts toward T: a rotated helper needs the same depth as the plain one. Removing named locals is not always the inverse of adding probes.

## By-value parameters and Matrix3D (Scout692)
- A by-value parameter that is never exposed keeps fields in registers (`to.X` and `to.Y` in esi/edi): write `x = CoordStruct(to.X, to.Y, to.Z);` and `Map[CoordStruct(to)]`. Field-written copies give frame misses (Jumpjet Move_To 35 to 94).
- `f(CoordStruct(cell))` evaluates in place; `f(Cell_Coord(cell))` hoists before the pushes.
- Draw_Matrix: `Matrix3D transform` at function scope, `transform = transform * Get_Slope_Matrix() * basemtx * rotation;` keeps slope temps in the multiply statement. An empty `~Matrix3D() {}` stops the by-value slope helper from inlining.
- Frame order follows final-code refs: code-free references and dead objects never move it, a real extra ref does.

## Pointer copies, halving and missing defs (Scout691)
- A named `CoordStruct const &x = f(); Coord y = x;` is the only form that gives the pointer copy `mov edx,eax`; no inline parameter form does.
- Y/2 at dim+4 with Z at dim+8 means halving in place: `int x = dim.X / 2; dim.Y /= 2; int y = dim.Y;`.
- IL costs: `{}` adds none, a pointer double cast a little, a real call +4, a named local +6; a window can be 2 units wide.
- A reload on a single-predecessor edge means a missing def: `result = 5;` falling into `if (result == 5) return 5;` gives both the reload and `mov eax,5` through threading.
- `CoordStruct half = dim / 2` with an inline operator/ refuses only the second nested ctor, because the zero-divisor ctor folds.

## Declare-then-assign and slot lifetime (Scout693)
- `T r; r = f();` in an inlined helper fixes reload order across arms, and the out-of-line copy stays exact.
- Temporaries and distinct named Cells share one slot by linear lifetime. A repeated store to one slot means separate objects.
- A named copy of a returned struct adds a register copy and rotates following code; `a + (Map[c].f() - b)` removes it.
- A default-first pointer (`p = &A; if (...) p = &B;`) keeps the source live, so `p` cannot coalesce and gives a register compare.
- `Distance(Point2D(a), Point2D(b))` reads the right argument's coordinates before a virtual call on the left.

## FP past rep movs and layout (Scout694)
- A `float const &` parameter on only the first argument of an inline ctor helper exposes that temp, so its fstp stays after `rep movsd` while fld/fchs hoist.
- `fstp` then `fld Y, Z, X` then `fld st(n); fmul` comes from double locals declared y, z, x with `x*x + z*z + y*y`.
- A later row can inline only when its callee is one call deep. A false block follows the memory-last goto. Compare-chain switches lay out default, then cases descending.
- Scout694's crt.py lists $E/$S and init relocs that a local score does not show.

## Array sizes and timers (Scout696)
- A frame slot with no references can be an array's declared size inside its rounding: check accepted siblings first (`char options[132]` in 0x755180).
- Timer residues with a `mov ecx,esi; test ecx` shape come from a Value form that rereads DelayTime on the idle path; named, reference and inline-parameter result copies all fold.
- In a label group at function end, source order of the blocks does not change layout. Some sum ties are not T or offs ties: real statements added at the top did not move them.

## Selector locals and helper temps (Scout697)
- A named selector local can hold a frame slot: replacing it with `if ((a && A) || (!a && B)) { shape = a ? A : B; ... }` flipped two slots. That order ignored ref counts, last-ref, first-def, alignment and aggregate class.
- Moving two tests into an inline helper fixed frame groups; the density sort moved when one ref left a temp group. Temps in a helper appear block-lived (inferred).
- `xy + Point2D(o.X, o.Y)` reads the offset first; a copy-form `Point2D(o)` does not. Removing a named reference can save enough budget to keep sites inline.
- VC6 mangles a free `operator*` as `??D@YI...`; an @ref to it scores 100 locally but is refused at acceptance as unbound. rl2.py lists unbound relocations plan-wide.

## Web weight, narrowing and islands (Scout699)
- A copy local in one branch halves a web's weight and flips esi/edi without emitting code.
- Stepwise `x = a; x -= b; x -= 3` on an unsigned short keeps the 16-bit op on the first operand; `a - b - 3` narrows the wrong operand and hoists a push.
- A 12 B coord written only X,Y through a by-const-ref inline drops the Z store.
- VC6 moves an island ending in `jmp next` past the next barrier when a threaded xor reaches ELSE. A named struct of doubles with an inline member Length spills only the last value; scalars never spill.

## Hidden counters and regimes (Scout698, Dropship 0x4A6B90)
- ebp is never a base without a disp, so `[Data+ebp]` carries no order information.
- Some sum ties ignore T, self-assigns, plain calls and plain loads, and move only with statements that make a CSE or IV temp, even after the site: they behave like a hidden counter.
- Dead local count sets regimes almost regardless of position (0-9, 10-11, 12-20, 21+ on Dropship); probes inside one arm do not act like probes at the top.

## MIN spelled out and Point2D forms (Scout700)
- TibSun's MIN/MAX macro spelled out with the repeated expression differs from a named `last` (94 to exact).
- A zero constant takes the register of the web defined just after it unless the source assigns that web first.
- `Point2D p; p.X = q.X; p.Y = q.Y - v;` gives lea/sub and the original registers; `p = q; p.Y -= v` gives shl/neg; `p -= Point2D(0, v)` keeps lea but loses the registers.
- An ebp frame in an /O2 row plus a dword-compared global in a tight loop points to original inline asm: stop drafting.
- Low seed scores can be stale; redraft before parking.

## COM coordinates and prior rows (Scout702)
- Is_Moving_Here takes a plain CoordStruct: the conversion operator hoists X,Y,Z into three registers ahead of the test (39 to 85); `MapCoords.As_Coord()` made it exact.
- A named `bool flat = otype->DrawFlat` flipped three register webs. `Point2D point = offset() + xpoint` with `point -= clip.Top_Left()` gives the original register order.
- With /Ob2 a row in the same unit inlines at its call sites, so a missing out-of-line ctor at a site can point to an earlier row's form: one row can be exact in five forms that differ only at the inlined site. TU position does not decide this.
- `Rect rect = Rect(0,0,0,0)` versus direct init changes which temp shares a slot and splits the zero web.

## Named bools and joins (Scout703)
- `A && B` tests hoisted into named bools from named locals stop threading from dropping a useless compare. A `jne` from one chain into another chain's assign blocks is `if (Count() || p == 0 || p == 1 ...) { same chain }`, not two chains.
- A vptr reload after a nested virtual argument means a separate statement: within one expression tree VC6 CSEs `*this`.
- `mov eax,K; jmp` into `cmp [r],eax` is a join on a named value.
- Scout703's sh.py is a structural diff with registers and frame offsets normalised: the fastest first read of a new draft. tp.py + ts.sh run a T phase scan.

## First function of a TU (Scout701)
- VC6 does not tail-merge two identical tails in the first function of a TU; any function before it, even a trivial one, turns the merge on. Blank lines before it or functions after it change nothing. Check this for rows that differ only by a merged tail.
- Local constants count as locals for T: plain `const int` locals act like probes, `static const int` do not.

## Case joins and add destinations (Scout704)
- `mov eax,K` plus a backward `jmp` from a case body means source copies merged after register allocation, such as per-case `if (result < 5)` with one case falling into default; a join on one local cannot produce that layout.
- A statement adding `this` refs can flip this/cell in ebx/ebp, and any non-`this` IL in the loop flips it back, so the weight looks like refs over span.
- `else { if (!pressed) call }` gives a threaded jump past the call while keeping setne/test.
- X+Y is canonical in VC6: Y+X, split sums and named copies give the same code; the add dest flips only when an operand stays live past the add.
- A float length temp folded into its one use lifted Fire_At from 62 to 71.

## Pointer reads and negation pairs (Scout705)
- `CoordStruct base = house->BaseCenter; Coord_Distance(base, center)` gives `lea r,[member]; mov edx,r` pointer reads; the same copy written directly folds (62 to 78).
- `bool const &on = e; bool off = !on;` keeps a second flag test after a failed chain.
- The order of a virtual-call `==` pair flips once two `bool x = !y` values exist anywhere in the function; ints, compares and copies do not count.
- `double dy = -delta.Y;` fixed fstp order and a tail push order. A constant used inside an inlined body in a loop joins and hoists the web of the same constant in the outer function.

## Float lengths and evictions (Scout706)
- TibSun `TPoint3D<float>::Length` with `(double)X*(double)X` casts spills X and Y to the struct and keeps Z on the x87 stack. Sum order z,x,y comes from `double s = Z2; s += X2 + Y2;`. Unnamed int Length casts give the X2 and whole-sum FP CSEs on their own.
- A long pseudo is evicted around cold arms and reloaded later; reloads seem to add weight, so one eviction can cancel the next (inferred). TypeList::Pick turns off the count CSE, as its inline `this` hides the member read until after CSE.

## Reload order and first fit (Scout707)
- Loop-region reload order is the reverse of the symbols' first occurrence in the IL, not declaration order. A code-free reference placed before the other's definition flips it.
- `Point2D delta; delta.X = offset.X; delta.Y = offset.Y; int dy = -delta.Y;` took a row from 64 to 99.9.
- A temp's slot follows first fit: a block-lived named or `$S` result conflicts with the earlier temp and pulls an 8 B double into its 12 B slot.
- In /FAs listings a pointer load hoisted above a guard can mark an unmerged return path.

## Temps, call order and arm folds (Scout708)
- `CoordStruct x; if (a) x = f(); else x = g();` shares one return temp across arms; a ternary keeps two.
- `Random_Pick(0, X)` computes the Random `this` after the pushes; `Random()(0, X)` before.
- A named `float length = (float)sqrt(sum); return length;` inside an inline Length sinks the fstp past the next load.
- A member-style inline for a field read stops CSE across a virtual call (83 to 94).
- Per-arm `faceto = Dir_To_256(...)` keeps the `and eax,0xff` that a single assignment folds into movsx. `int facing = 0` before the first test gives a shared zero web.

## Reassignment and zero webs (Scout710)
- Reassigning one variable (`here = Map.f(here, ...)`) gives a `mov ax,dx` X copy and a shared slot; a new named Cell does not.
- Wrapping distance in inline helpers keeps Center temps block-lived; three distance sites in one `||` statement otherwise each hold their own temps (frame 0x80 to 0x54).
- `default: break;` on a switch can give the zero constant a register web across the whole function.
- Tail merging and a far earlier vptr register can flip on a local changed much later: treat these as the hidden-counter class.

## Dying locals and FP operands (Scout709)
- An add destination can come right when a call result is a dying local inside an inline: `static inline int Bridge_Height(coord, int &ground) { int height = call; ground = height; return height + j; }` gives `add eax,ecx`; direct forms stay flat.
- `double gravity = Rules->x; velocity.Z -= gravity;` gives `fld; fild; fsubp` on a Vector3 (89 to 99).
- An inline's return slot adds frame bytes and moves temps: the assigned form and the return-ignored form differ by the return slot.

## Real helpers and arm order (Scout711)
- The real Process_DD_Result with its early returns, called before `result == DD_OK`, lets threading produce far arms; hand expansion does not (76 to exact).
- `Rect area(r.left, r.top, r.right - r.left, r.bottom - r.top)` keeps a width read ahead of a flag test; a plain int forward-substitutes into the arm.
- Statement order inside an arm decides the `and` destination register. `if (flag) {arm} else {...}` lays the flag arm after the ret; `if (!flag)` does not.
- A loop written `if (cb) cb(); while (...) { if (cb) cb(); }` supplies the fourth zero use that hoists zero into a register (58 to exact).
- A spelled-out MAX with `shift = MAX(a); shift -= MAX(b);` closed a register swap. Writing `box.Y =` into a Rect local fixed a `push; sub` tie.

## Temps within one statement (Scout712)
- Two temps inside one statement or one inline conflict even when the first is used up before the second is born. A coord whose Z store is dead vanishes from code but still takes a 12 B slot.
- `CoordStruct(Point2D(call), height)` evaluates the Point2D first, against right-to-left; a by-const-ref helper keeps height first.
- A this/position ebx/ebp swap tracks final-code base refs only: dead reads, aliases and inline params do not count.
- `R|G|B` reassociation does not follow source order; it looks register-need driven.

## Phase scan first, long compare lists (Scout713, Scout714)
- A first-read tie (`mov cx,[K+ecx*4]` vs `mov cx,[eax]; add cx,[edx]`) unmoved by about 60 helper forms was a T-phase tie: run tp.py before more form variants. Rows parked before Scout651 may hide the same tie.
- A repeated return in the one arm that would fall into the tail moves the whole case past the ret, and the copies cross-jump.
- `(Cell)` casts cost frame slots. Declaring a loop counter before the value it is added to swaps which register the `add` writes.
- `bool c12 = list; if (c12)` sets arm layout for long compare lists. `x != old_x || y != old_y` after up-front copies threads away compares on unchanged fields. One if/else-if chain with one trailing call tail-duplicates it into each arm; a per-arm call merges with a `jmp`.

## Statement shapes and budget (Scout716)
- An inline's statement shape sets its IL size and so the /Ob2 budget of its callers; put a per-population macro around it when other units need the old shape.
- `if (p) delete p` where `delete p` was meant changes the tail register and the IL. `(style & (unsigned char)7) == (unsigned char)7` gives `and ecx; cmp cl`. `RGB()` folds before IL and adds no budget.
- Slot and reload ties can depend on whether a double local is an FP-register candidate.

## Add destinations and depth (Scout715)
- Apparent tail-merge copy differences can be add-destination ties on an inlined Add, with the merges following from them. Phase period can be 32 in local count, not 8, in a large function; a Point2D temp counts as 2K only for sites after it. Calls and labels do not move those fronts.
- Rows out of line at the first case of a huge function, when budget is ample, mean an inline wrapper around the statement.
- `bool const &x = obj->Bitfield;` gives a dead byte store. `CoordStruct(coord)` as a by-value argument hoists X, Y, Z ahead of `Checked()`. `jl` on a loop over a static table means an int index loop, not a pointer loop.

## comip levels and budget probes (Scout718)
- Real comip levels decide the inline budget: `_QueryInterface` with its else arm, GetIID, Attach and an inline `_Release`, with the dtor calling `_Release`. One inline source then gave expand-release, expand-ctor and call-both at three rows.
- `clsid == CLSID` through the shared GUID operator== counts as a site. Hand-expanded blocks can hide real calls: calling the real row took another row out of line and fixed three rows.
- Empty inline probes after a suspect site measure the budget shortfall in about three compiles (kp.py, thr.sh); never deliver them.

## Real helpers hiding in units (Scout717)
- A row spelling out a long inlined list may hide a real predicate seed in the same unit: calling it shrinks callers' IL below the auto-inline cap. 0x5ED180's missing budget was a real call to the unit's mask builder.
- A new row that other rows inline needs body_bytes in the plan, or the composite pass splits and those rows are missed.
- A charged helper level over `if (Count) Get(...)` keeps a first lookup out of line; a by-value key fixes key stores.

## Pending sites and op- (Scout719)
- A nested argument does not divide the budget: in `f(a - b)` the op- gets all of it. One pending inline site after op- in the same body sends it out of line; wrapper depth with single-site bodies does nothing.
- Temps inside an inline wrapper always go to shared locals. Case layout follows source order, which limits the earlier by-value note.

## Unit rows as inline sites (Scout720)
- Unit rows can hide as inline sites: 0x5E7230 and 0x5E7620 are tests expanded inside 0x5E7EB0, and in Get form they reproduce its mixed lookups.
- `T *const &found = record; if (found)` stops known-zero threading past a record test. A root-level `unsigned int key` holds its slot, so later keys take a new one.
- A missing first-lookup call can be pending sites: about 38 empty sites without a helper level, 5 to 8 with one. Row wordiness moves the charge at its inline sites.

## Rect members and FP order (Scout721)
- `Rect(-2,-2,3,3)+point` is an inline over the out-of-line sum and corner ctor. `rect.Intersect(x)` as a member supplies the 16 B result copy that a free call does not.
- Vector3 Length with `double z, y, x` locals loads the FP stack as Z,Y,X. A `double dist2` with a `(float)dist2` final use flips which operand gets loaded.
- `StageClass(noinit)` with a plain CDTimerClass member stays inline where a wrapper goes out of line.

## Later rows and real calls (Scout722)
- A row can inline a later-addressed row: 0x5EDB20 is really a callback enumeration plus a direct call of 0x5EDD30, and that depth division keeps a nested row a call.
- An arm that looks spelled out can be calls of real unit rows. Pending inline sites decide which rows go out of line.

## Double locals and by-value Rects (Scout723)
- `double angle` plus `double scaled = distance` inside inline Move_Coord fixes the frame by 8 B: the double makes a Y spill and a call result share a slot.
- An inline taking `Rect box` by value and intersecting inside gives crossed Rect slots with no named local.
- `Cell offset(ox, oy)` and fieldwise stores each fix only one add destination; a ctor for X plus a separate Y update fixes both.
- `test ah,N` where `test esi,imm` was expected means the test is on a masked value, not the raw one.

## Consumption before a dtor (Scout725)
- Missing IL can be consumption before a ??_G site rather than a bigger dtor: `int size = (wcslen(text) + 1) * 2;` plus a per-arm `next` kept the delete out of line, where an `if (p) { delete p; p = 0; }` form stopped the dtor inlining elsewhere.
- Rect ctor charges can be the root budget: which Rects are field-assigned reproduces the arm pattern. A `while (length && w > max) text[--length]` loop gives the hoisted pointer and rotation.

## Repeated calls and short rows (Scout726)
- A call repeated inside an inlined loop means the call was passed as an inline argument: `Get_Unit_Count(x.Get_All_Totals(), x.UnitCount)` re-evaluates per iteration. A row defined later in the file still inlines.
- Reusing `i` for an outer loop, rather than a separate named counter, can fix frame slot order (97.12 to 98.14).
- `Timer(0)` picks the out-of-line long ctor 0x4DE1E0 with no change to row 0x6682C0.
- `return Anims.ID(anim) == -1 ? true : false;` keeps Is_Anim_Done and Show_Credits exact where the plain comparison does not.
- A shared-header overlay can change a fragile row's inlining: guard new declarations under a macro defined only by the new source (RA2_STATS_UNIT) and run fragile_probe.
- Real functions hide behind short inventory rows; check accepted-coverage gaps (Scout726 gaps.py) and carry body_bytes plus bound_evidence.

## Charged forms of exact rows (Scout728)
- A free callee under the 40 IL limit becomes charged with one extra statement: `bool done = ...; return done;` gives identical row bytes but makes the inlined caller call it out of line. A call site missing from a caller of an already-exact row means looking for a wordier form of that row.
- Pending-site budget shows up as a region, not a count: Presentation needed a<=3 and a+b>=5 (a sites at the end, b between two named sites). One free wrapper per string fetch met it, and only `Fetch_Text(label, file, line)` kept the register ties; `(label, line)` gave 93.99.
- A block-scoped redeclaration of a local in the last else moved two frame slots. A `char *` local picked the original induction variable. `int id2 = Players[i]->ID` fixed a load order and about 50 diff rows.
- Build_Hicolor_Pixel with constant args: the original evaluates shifts r|b|g, not header order.
- Original bugs to keep: the fame test reads hallfame[i].score with i stale at 10.

## Strength reduction and reused Rects (Scout727)
- An uncollapsed `row - row_base` can be `for (int index = first; ...)` with `(index - first) * height` written twice: VC6 distributes and strength-reduces, one imul feeds two slots, no CSE of the subs. No row variable exists.
- `int length = wcslen(text); text_28 = new((length + 1) * 2);` is the Set_Wide_Text form that holds in all its users.

## Frame maps and web evidence (Scout730)
- Map real target slots against our variables (Scout730 fm.py, em.py) before permuting declarations: it finds split variables directly. One variable's last three refs were a separate Rect in another slot group, and fixing that moved the density sort.
- A function-scope Rect overlaps slots across switch cases, so a later 28-byte object skips it and takes the next-newest slot. Case scope gave no change; function scope gave the original's order.
- Stack reads from a Rect home are not proof of an escape. A non-escaped Rect with spilled X and Y has the same loads, but the allocator sees different webs: escaped reads leave temporaries that keep a value out of esi and give `lea`; spilled webs give in-place `add` and `mov cl`/`test cl,cl`.
- A zero constant web needs a use inside its range, not one that survives to final code. `master ? master->alternate_cd : false` in one arm threads a literal 0 and flips X/Y from ebp to zero. Dead `= 0` initialisers and uses after the arm's join do not count.
- A bare `return 0` cross-jumps to the epilogue of the last `return 0` written in the source, whatever the layout order, and case blocks stay in place when moved in the source. The block carrying the shared epilogue schedules separately. A single-site probe sweep (rz.py) found this where diff scores stalled at 99.91.
- `Size = Size >> 1` versus `old_size >> 1` swaps which copy gets the load.

## Reload order follows a location count (Scout729)
- Spill reloads at a join have an order. When all predecessors end in the same reload it appears to sink into the head block and the remaining reload cross-jumps, giving one head block and a lone `inc [mem]` at the tail (inferred from output).
- The flip follows a per-function count mod 32 of static memory locations named, read or written, dead reads included. Alias names for one address count separately: the key is symbol plus offset. Locals, copies, calls, new externs, folded constants, parameter reads and frame size do not count. Distinct immediates and strings move it irregularly. The commutative `|` order of Build_Hicolor_Pixel moved with the same probes.
- So a stubborn head block or `|` order can be a location count fault, not a loop-form fault: for(;;), while(1), do/while and goto were all flat.

## Dead conditional writes as evidence (Scout731)
- Some conditional writes survive early dead-code removal and change register choice with no code left in the output. In 0x5E9410, exactly two `if (kind == C) { x = x; }` give 100; one does nothing, three or more drop to a 49 regime. The target must be a threaded variable with a non-constant right side, under a condition on it. Constant right sides, empty ifs, dead reads and unconditional writes fail. Never deliver these; read one as a sign that real source has two more conditional defs of that variable, and look for those.

## Twin locals that do not exist (Scout732)
- A `linked` local in a twin may not exist. `LinkedTo->...` twice in one expression tree gives the same load, which does not CSE with earlier reads; a named local was CSE'd upward.
- `x = MIN(T, factor*T)` as the macro, with the member in every operand, kept a `fld`/`fstp` spill before the second call and a reload in the arms. An if/else or mixed ternary sinks the load past the call.
- `(0, x)` in a comparison forces a load before a later call: a quick probe for this fault, never delivered.
- A ctor out of line at one deep site while Cell conversions at that level stay inline is a level fault, not a size fault: the model (RA2_COORD_TPOINT3D plus coord.As_Cell()) had to change.
- An argument evaluated ahead of an outer call inside `new X(...)` is an inline accessor call.
- Inlined Query_From plus Attach gives the comip site with an explicit Release; a later site with the pointer known zero folds to `setl`/`dec`/`and` with no source change.
- Dropping `else` after an early `return` gives a second Cell local its own slot rather than overlapping the sibling scope's.

## Late-removed compares and loop forms (Scout734)
- A computed value stored and never used can be kept by a compare VC6 removes late. With one use `range` is forward-substituted and the store loses part of its expression; two uses keep the full value. Missing refs also shift frame slots.
- `for (; tries < 6; tries++)` with `break`s gives a plain `jl head`; `for(;;)` and `do/while` copied the head up to the first test.
- `flags & 0x100` tested twice stays a CSE temp and is not threaded; `int under = flags & 0x100` threads the second test away and spills.
- Dividing a Vector3 by a double gives fdivr with a qword zero and matches Z = 0.
- A by-value CoordStruct argument hoists X, Y, Z ahead of a virtual call evaluating the other operand.
- `!found ? fallback : Cell(x, y)` gives `lea eax` / `mov eax,[eax]`; through an inline it adds a second temp slot.
- `for (int j = i + 1; ...)` plus `i++` coalesced j and i (91.52 to 99.75). `i < start + count` rather than a named `end`, and named int offsets, fixed schedule ties.
- An inline ctor in a unit emits the vtable, so check_plans needs @refs for every slot; the failure reads "Unsupported allocatable data section".
- A ternary in an argument tail-duplicates the call.

## VectorCursor virtual operators (Scout733)
- TibSun VectorCursor has virtual `operator bool`, `++` and `--`, not inline wrappers over Valid/Next. That removes 6 pending root sites per use, so List() inlines and its charge sends the cursor ctor and operator= out of line. Took 0x72A880 from 62.65 to exact. `.Valid()` on a local object devirtualises and inlines, so it does not remove the sites. VECTOR_CURSOR_VIRTUAL_OPERATORS guards the form.
- `TRect::Set(0,0,0,0)` gives one zero web; a `TRect(0,0,0,0)` temp spreads zeros across registers (64.8 to 92.57).
- A by-value inline parameter gives a bitwise struct copy with the temp's address pushed first.
- A named block-scoped cursor leaves the ctor's eax return unused, where a temp feeds it to operator=.
- A plain vtable bound from an inline class needs @refs for every slot under the emitted manglings; the audit names only the vtable.
- Tools: Scout733 rp.py (remaining budget at a source line by binary search), cn.py (diff masking esp offsets and jump targets; raw score misleads when the frame shifts).

## Temps, slots and switch signedness (Scout735)
- `Setter(char const *)` through an implicit Wstring conversion gives each temp its own frame slot; a named Wstring shares one (90.30 to 98.25).
- `char const *a, *b` declared in a loop body overlaps a later inline's locals; at case scope they do not.
- A found path jumping past an assignment means `if (!remember) x = saved;`.
- `switch ((unsigned short)wparam)` gives signed `jg`; `wparam & 0xffff` gives `ja`.
- A temporary `(Point2D(a.X,a.Y) - Point2D(b.X,b.Y)).Length()` in an inline may overlap a caller slot; a named diff cannot (frame 0x34 to 0x2C).
- TibSun `Is_Complete_Turn(towards, by)`: the `by` const-ref temp is built before `towards` is evaluated.
- Nested `if (a) { if (b) ... }` produces a hoist that a combined `&&` does not.
- Repeated triples with depth-2 calls out of line are usually an inlined row. Check existing rows before spelling the code out.

## Cursor rows and pending sites (Scout736)
- A ctor refused at a budget of about 1596 must sit one inline level down: depth-one refusal is impossible there. A free wrapper returning the object makes its ctor out of line while later cursors stay inline.
- Postfix `cursor++` is a pending site; prefix is not. TibSun's postfix wrapper is a free site.
- Sites can have a two-sided window: 3 to 14 pending sites before the main loop both refused one inline and allowed the first of two dtors.
- Natural sites come from accessors: Get_Filename, Map::Get_Origin, Get_Play_Rect, Get_Back_Rect, INIClass::Load(file), Territory getters.
- tp.py T-phase scans also move the budget, so give the base a wider margin first.
- `(*cursor)->ID` compare followed by `T *item = *cursor;` gives the re-read of array and element (61.90 to exact).
- A hole fully covered by one normal row is skipped by gaps.py: use gaps2.py.

## __fastcall argument list order (Scout737)
- The first two dword arguments in list order take ecx/edx, and a by-value struct goes on the stack wherever it is listed. `(Point2D, Point2D, int, int, bool)` and `(int, int, Point2D, Point2D, bool)` share an ABI but differ in evaluation order. With register args listed after the structs they are set before the pushes, so struct dwords reload one at a time through eax. A call site that never loads edx means the same thing: every Planning `int unused` parameter was this.
- A local can sit in the stack home of an enregistered parameter; a "point written into the start slot" is a local, not a write to the parameter.
- Depth-2 refusals after several larger depth-1 expansions name an inlined row. A lab with extra sites at the end of the caller raises the budget rather than lowering it. Scout737 sb.py measures how many units of spend a refusal needs.
- `return f(a->x, a->y, a->z)` is free; naming the three locals first makes it charged.
- `bool f() { return a && b && c; }` emits `mov eax,1` / `xor eax,eax`, callers `test al,al`.
- An inline Length with `int x = X; int y = Y;` keeps an FP load order that a direct `this->` read flips.
- A function at the tail of a unit, such as Planning_Pixel, need not be an inline.

## Implicit operator= and zero webs (Scout738)
- Implicit CellClass::operator= stays an out-of-line call in a small caller and expands inline only in a large one (a budget effect). It copies each named bitfield with an xor/and sequence, every alternative of a union, and a struct of bitfields as one dword. Unnamed bits are not copied.
- A variable with a zero init and no re-init on its loop (`int i = 0; for (; i < n; i++)`) keeps zero in a register for neighbouring stores. `int y = 0` placed after a test rather than in the for gave a second zero web (75.3 to 80.25).
- `new` size order: `h * w * sizeof` gives `imul` first, then the lea chain.
- Copy loops by index give a biased pointer and a dec counter.

## Frame density and forwarders (Scout739)
- Frame slots sort by final refs divided by size, descending, 1-byte bools included, so bools interleave with 16-byte slots.
- An address-taken local at function scope is live from the function's first call. Moving an inline's `if {...}` to an early return puts its locals at function scope, where they conflict with a temp: that fixed a caller's frame without touching the caller.
- A body of just `return Wrapper(...)` always expands, even through an inner inline. A reference local plus a named result charges the budget and keeps direct argument order.
- Inline budgets look thiscall-sensitive: a member stayed out of line at the tail while a free helper with the same body inlined.
- Spilled variables get second-chance registers at first use after a death, and register ties flip when the refusal pattern changes: fix calls before chasing registers.

## Nested tests and row depth (Scout740)
- `if (a) { if (!b) {...} }` versus `if (a && !b)` around an inlined row changes the inline budget: `&&` at the last two of four sites took 0x685600 from 81 to 99.44.
- A byte-repeated bounds test with some calls to the same address is one inline at two depths: inline at depth 1, out of line one level down.
- Fetch_CellID() temporaries in a row explain separate frame slots when that row is inlined in a caller.
- `int ramp = ...; int base = ...; value = *table[ramp] + base;`: naming the right operand before the sum keeps the left operand's load order (75 to 98.49).
- A statement first in the then-arm and alone in the else arm is hoisted above the test with no else block, but it changes register allocation; a dead one does not.

## Free __fastcall and level helpers (Scout741)
- A "member" body at a tie, with its register setups after the pushes, goes exact as free `__fastcall`. It took zero-argument node members from 97 to exact; probe it first for any Planning member.
- A CoordStruct-derived type with a forwarding default ctor keeps ctor 0x41B630 out of line one level down, while == still reads the temp slot directly. An inline returning CoordStruct does not do this.
- Row-exact forms that refuse at depth 4: `Point2D pixel = f(coord); return pixel;`, `::Rect rect; rect = ...; return rect;`.
- A named local in a depth-1 accessor can flip the register regime: `PlanningBranchClass *branch = Branches[index];` took a row from 90 to 97. Naming `owner` before an argument call moved `lea` CSE back into ebx.
- Dead `int` probes and dead static reads moved inline budget here, not ties, so a jump in a tp.py scan may be a budget effect.

## Same-unit twins and record shapes (Scout742)
- Follow a same-unit TibSun source (smartdeform.cpp) closely before reasoning about registers. An apparent scalar-replacement fault was the record's real form: `Add(UndoInfoStruct(point, value))` with a Point2D member and assigning ctors keeps the point's home memory where the record inlines; plain X, Y, Value fields do not. Spread 58 to 99.49, Deform_RMG 65.88 to 99.52, and two Resize COMDATs came exact with it.
- A direct `cell->MapCoords` read differs from by-value Fetch_CellID(): the temp keeps X in a register, turning `sub word [slot],bp` into a register sub.
- `lat_rect - r.Top_Left()` with an inline Rect::operator- went 57 to exact; the out-of-line form spills the whole Rect and shifts the frame sort of unrelated slots.
- If/else and a ternary colour differently, and each can win in a different function. `if (Login(s)) return true; return false` keeps constant webs that `return Login(s)` loses (71 to 88).
- `if (c) SetFocus(GetDlgItem(w, a)); else SetFocus(GetDlgItem(w, b));` cross-jumps to `push a; jmp; push b`; a ternary argument does not.

## Refusal chains (Scout743)
- A temp in the by-value argument of an inlined callee overlaps that callee's locals; naming `CoordStruct coord = obj->Coord();` first took a frame from 0x44 to 0x38.
- A row refused at one site and expanded at another can be the same helper: a refused callee leaves its spend, so the next nested site expands.
- Fewer depth-1 sites after a parent let nested sites expand.
- `bool f() { return a == b; }` gives `mov eax,1` with callers on `test al,al`; `if (...) return true` gives `mov al,1`.
- `const &` aliases count toward the inline size and emit nothing. Do not use them as padding.
- A plan may replace a row in a seed population; a hand-written population needs allow_split.

## Calling convention and scheduling (Scout744)
- A function's own `__fastcall` changes /G5 scheduling across the body: `lea ecx; push eax` against `push eax; lea ecx`, `mov eax,1` before or after the pops. If only scheduling differs, probe the root's calling convention.
- Root-to-helper moves supply budget: moving k units costs 2k of root budget, plus k if charged early. Earlier sites out of line with later ones inline can be budget after all.
- Weight of a constant web drops when its uses move into a conditional block; a plain block keeps it.
- Reading a global directly in an inline, rather than through a local copy, allows threading of its range tests at the call site.
- A virtual slot called directly from a local object cannot sit in a helper taking that object by reference: evidence for keeping the loop in the root.

## Early returns and helper-owned locals (Scout746)
- `if (!a || !b) return 1; ...; Dialog next; return 1;`: the early return carries extra dtor copies, so vptr uses keep their register weight, then the copies cross-jump away after allocation. This fixes the regime that the conditional-block form loses.
- A refused inline whose result is never tested means the source discards the value: `add(x); return true;`, not `return add(x);`.
- A named local inside a small helper dies at the helper's end; a temporary in the caller gets a different order and slot.
- A WideString in the same block as an expanded inline's locals gets its own slot; `{ }` around it shares the slot.
- By-value WolArrayList arguments expand wlib's copy ctor into operator= into clear; the scalar deleting dtor is refused at depth 4 by pending-site budget.
- A zero register web reaches across a join only with `if (event != 6) {...} else { break; }` nesting; flat `if (event == 6) break;` drops it.
- Explicit `continue` right after a final `goto X` folds to `jne top` and pulls X adjacent.
- ms_message_box.hpp pulls shape_file.hpp's static unknown_empty_rect in population-4122, adding a .CRT$XCU initializer and renumbering `_$E210`.

## Real accessors as budget (Scout745)
- "Free sites" a row seems to need are usually the unit's real one-line accessors, sitting in seed rows. Spelling them out (Event_At, Stop_Type, Mission/Target, Clear_1c) made rows exact with no lab sites.
- `int planned = event != 0 ? (char)x : 0` gives `movsx; test eax,eax`; plain casts do not.
- `if (n == 15) return 2; return -1;` gives the sbb form and threads in an inlining caller; a ternary does not thread.
- `(object && techno) ? (TechnoClass *)object : 0` reproduces a direct jump past a Kind_Of ladder.
- shared/wwmouse.hpp adds a static initializer that local scoring does not show; use shared/mouse.hpp.
- At some sites empty sites do nothing but a real extra expansion flips the refusal: spend, not sites.

## Data symbol count, second confirmation (Scout748)
- Commutative `add a,b` and `lea [a+b]` operand order flips with the number of distinct data keys in the function (Scout748 read period 32; Scout749 found thresholds, not a period, up to +80 in OnChannelJoin, and position in the function does not matter). Stores, reads, address-of and strings count; function symbols and repeated symbols do not. A separate cluster follows function IL size. Dead arms of an inline with a folded condition count and emit no bytes.
- Stores before an inline memset keep their immediates: `x = 0; name[0] = 0; memset(...)` compiles with `rep stos` ahead of the stores; with the memset first the stores reuse eax/al (89 to 94, the twin's order).
- A byte-order 3,2,1,0 12-byte copy was a real by-value helper inlined, named local plus return.

## Inline depth limit and cross-jump tails (Scout747)
- Hard inline depth limit: depth 8 expands, depth 9 refuses, even the trivial placement new. Budget refusal only hits sites before the pending spend, so later sites out of line with earlier ones inline points to depth, not budget. Moving a callee one level deeper makes site 1 inline and later sites call.
- A plain function must be under the auto-inline size limit: a helper shrinks its IL and the row stays exact.
- `break` versus `return 0` at case ends decides which block keeps a cross-jump tail, and the outer `break` after an inner switch counts too (92.95 to exact on 7 KB). If a tail merges at the wrong end, grid these first.
- A lone reload that no flat form gives can be a real row inlined, with `result` from the row.
- `setne/test` came from TibSun's three-way `if (music && ...) else if (music) ... else if (!music)` with a bool local.
- `char *`-free point arrays copied as 12-byte structs have no ctor; a named double lands above the arrays at any scope (Scout749 EBolt).

## Threading and the auto-inline limit (Scout751)
- VC6 threads a bool test away when all its predecessors are constant, through inline levels and by-reference or by-value inline params. Ternary, `||`, do/while(0), goto and dead stores all still thread. Only an address-taken local stops it (`bool const &ok = H()`, `bool *p = &found`). So a surviving `xor al,al; test al,al` suggests an address-taken local that folds late.
- A row just over the auto-inline limit can drop under it through a real helper: a tail of `Close_Wonline_Wait(flags)`, or `BucketCount` read directly rather than Get_Size().
- `else` chain versus flat `if` on arms moves the cross-jump keeper and budget by a small unit.
- `calls.py` (target and candidate call sequences side by side) finds refusal patterns faster than score diffs.

## Compiled callees and web references (Scout750)
- A row compiled earlier in the unit inlines with its callee already expanded, so the callee can't be refused inside it. A fresh `inline` copy of the same body can be refused, emitting the callee at its original address.
- Flat early returns put a later local at function scope, stopping it sharing a slot with an inlined block's temp. A bare `{ }` does the reverse.
- Dead twin statements can keep a row over the auto-inline limit with its bytes unchanged.
- Whether a parameter or the zero web gets a register can come down to about two references.
- Refused inlines belonging to another population need their row moved in with allow_split, or they fail as "Unaccounted emitted code section bytes".

## Lifted helpers and hidden schedules (Scout752)
- A repeated loop lifted into a real helper inside a callee lowers that callee's cost and adds a nested spend. It took a row from 96.55 to exact and changed refusals in other callers.
- Free pending sites count, and wrapper sites weigh more than ctor and dtor sites. The budget tracks root size: 20 plain stores flip decisions.
- An `unresolved` entry in diff_candidate can hide a /G5 schedule difference: byte-compare the object (Scout752 sdiff.py) before planning.
- "Unaccounted emitted code section" names no symbol: Scout752 comdats.py lists emitted COMDATs to diff against a baseline.
- A row with zero callers and an inventory gap can be the real inline behind a repeated pattern; check gaps before spelling a helper out.

## Join reloads and vanishing compares (Scout753)
- Second-chance join rule (inferred from probes): jump-table edges keep a spilled variable's register mapping; an extra forward jump from a block without the mapping drops it at the join and the use reloads into a fresh register at the head block; a fallthrough predecessor without the mapping gets the reload at its own end.
- `T *target=attacker; if (attacker==0 && <global read>) target=0; if (target)...` leaves no bytes but keeps `target` a distinct web, so a later `attacker` use reads the home slot. The condition must be a global read: a member read folds early, and a parameter read maps that parameter into a register.
- The order of compensating reloads rotates with distinct static keys; dead reads at head and tail give the same result, so it is key count, not budget.

## Reference params and direct counts (Scout755)
- A reference param that folds directly (`In_Bounds(cell->MapCoords)`) reads through the object; a reference local reads through its pointer. VC6 does not CSE X and Y between the two. A `lea` of the field ahead of direct reads with re-reads after means this split (75.63 to exact).
- `cells.ActiveCount` versus Count(): direct member reads give `[base+count*4-4]` and `ActiveCount--` CSE'd with the index; Count() adds a copy web with `mov eax,edx` / `dec eax`.
- A repeated loop can be an inlined generator row (4-arg Clear_Region_Cells). `Fetch_CellID()` twice in one expression gives one temp slot per path; a named by-value local does not.
- `MAX(10, x)` as `10 > x ? 10 : x` gives `jge`; the reversed order gives `jg`.
- Spend position does not matter up to the first refused site.

## Later rows inline, and charged windows (Scout756)
- A same-unit row defined later still inlines, so many "local helper" copies can be real row calls.
- Refusal at a late site follows R/N: natural tail sites flip a next-to-last call, only charge flips the last site. Scout756 need.py measures the gap.
- A narrow window can come from one charged form: Get_Region with a named reference closed a 44-80 unit gap and refuses at depth 2 elsewhere.
- Moving a real expression into a helper moves budget even when the helper's charge is small.
- The real Range struct form of Map_Random_Range fixes fstp order when inlined.

## Case source order and site division (Scout754)
- Case source order is a first-class budget lever: layout stays by value for most orders, but moving one case last took a dialog from 78 to 88 in one step. Probe it before chasing IL.
- Grouping a callee under a free wrapper saves its whole charge. Spend after a site's last user is free for that user; a new site instead divides its budget.
- A small lab with a site through a free Mid behind a charged Lab measures any callee's IL to about 2 units.
- `while (BucketCount > Min)` versus `Get_Size() > Min` is a /G5 load-order flip visible in a COMDAT.
- A row emitted as a refused inline must carry its callee as an inline too.
- Two-level Remove_Head into Remove(T&,int): the free outer level lets the inner Get be refused while the two-arg stays accepted.

## By-value returns and global reads (Scout757)
- `Cell const &exitcell = f()`, binding a reference to a by-value return, gave better reads (75.48 to 77.37).
- A global read twice in one call (`Cell(Map.X, -Map.X)`) is CSE'd without field forwarding, so the temp's store and reload survive; a local copy forwards and loses them.
- `short x1, x2` in a Direction(Cell, Cell) helper gives the 16-bit load with a late movsx.
- A nested virtual call hoists the outer vptr read above the argument call; a named result moves it after the call.

## Wrapper levels and pending-site sides (Scout758)
- A depth-1 call can stay expanded however many sites are pending: an out-of-line first-site call means a wrapper level, and then pending sites flip it.
- Sites after a helper divide its budget; sites before only affect earlier refusals. Probe them separately.
- Moving a raw call into a helper shrinks root IL and so the budget: an empty probe and a real helper are not equivalent.
- `bool const &ok = A() ? B() : false;` keeps the surviving al test and puts B at depth 1.
- A back edge reload can cross-jump into the last pre-header load, so a jump landing mid pre-header points to reload order.
- A global `@ref _x=addr` in one row rebinds that symbol for every row in the unit and shows only as a single ~ call line elsewhere.

## Stack-object count and reused cells (Scout759)
- A stack-object count mod 4 flips late store forwarding and register webs far from the probe, up to 8 points on one row. A one-field struct temp counts 1; an int, a non-copy pointer or an unconstructed Cell counts 0.
- A surviving dead store, or `add word [slot]` followed by a movsx reload, can mean an address-taken function-scope variable reused across cases. `mov ecx,[eax]; mov ax,cx` has the same cause.
- A `Y - 0` from `Cell(1, 0)` keeps a temp's Y store where `Cell(x - 1, y)` copy-propagates it.
- An inline in a call's argument list moves its temps and constants ahead of the pushes, into callee-saved registers; naming it first does not.
- `if (a || b) X else Y` lays Y first and makes X last-written, so the tail keeper moves with source order while layout stays put.
- A real template inline (Random_Pick) reverses the load order compared with `Random()(a, b)` written out.
- A Cell whose last reference comes after y's takes y's slot. Temps inside one small inline share 12-byte slots; plain temps do not.

## Distance forms and tail webs (Scout760)
- A `bool const &` temp of a bitfield reproduces `and cl,1; mov [esp+N],cl`.
- Two distance forms can sit in one function: a by-value second parameter (`lea edi; mov edx,edi`, Z spilled) with out-of-line Length, and a const& difference with inline double-local Length.
- `CoordStruct scoord = source->Coord` beside a by-value `tcoord` gives a hoisted `lea` plus `add edx,0x88`; a reference local does not.
- A zero web can hinge on one zero use after a join; removing other zero uses leaves it. A coord tail web can flip on one reference near a threshold, and added references to the competing CSE do not move it.

## Loop weight and wrapper asymmetry (Scout762)
- Source loop nesting counts even when bytes do not change. `do { e = W(); if (e == K) P(); } while (e == K && !leave);` threads to the same code as a one-level `continue` form, but the extra depth raises W's weight, moving a zero web and a global's register far from the loop (90.64 to 95.75).
- An import called only through an inline wrapper reloads after a directly called one at a pre-header. A jump landing mid pre-header on a reload points to this asymmetry first; weight, declaration order, first reference, data keys and named locals did not move it.
- `mov [m],0` versus `xor edx,edx`: a by-value return helper forwards a temp field, an in-place temp keeps the immediate.
- Register webs need refs that survive to allocation. Threaded or folded tests add none; only a real loop level adds weight.
- diff_candidate can read 98.75 on an exact row when only switch table `$L` relocations differ: trust check_plans.

## Tail duplication and push depth (Scout761)
- Check push depth before calling a store dead: `[esp+0x1c]` under a live push was a function-level variable.
- One statement after an if/else can be tail-duplicated into each arm, each ending in `jmp`. A one-register rotation across arms points at this.
- `int i, j` reuse across loops fixes frame slot grouping.
- Object count is strictly mod 4 and position-independent; dead keys count per function only.

## Named call results and known cases (Scout763)
- Naming an int call result before a comparison (`int h = f(...); if (h < coord.Z)`) rather than calling inside the `if` kills a zero web and fixes temp slots and store order in the block.
- `Cell const &center = f()` moves Cell X and Y add order together; under that form dead-arm data keys flip single add sites with a period of about 32. Mixed X/Y orders are key dependent.
- Threaded outer loops do nothing: a loop level only counts with a real back edge at weighting time.
- `break` jumps from known cases thread past a following `if (result != 4)`.
- macOS: overlay folders hb and hB collide on this filesystem.

## Add orientation (Scout764)
- A trailing `mov mapped,reg` can be a spilled variable's second-chance register at a join, when the add has the other orientation. Add orientation is chosen regardless of source order and varies site by site with key and object counts; stack objects move key thresholds non-monotonically.
- Unequal forms of a repeated statement let one arm's register web move into the other arm.
- Reloading a variable at a loop head is decided at the join: without the mapping a scratch register stays free, which keeps a CSE alive.

## Twin inline templates and by-value helpers (Scout765)
- Check the twin's inline templates first: `Random_Pick` instead of `Random()(a, b)` at four sites flipped a whole tail register web (84.94 to 96.28, masked 0). Not universal.
- Separate block-scoped loop counters, as the twin writes them, fix slot grouping. Splitting `ittype` into `ittype1`, `ittype2` fixed ring add order.
- A separate search loop with `bool found`, whose test threads away, leaves the same code but changes which `return` tails merge (91.66 to exact). `if (index == Count)` keeps a real compare.
- A by-value helper parameter moves a temp's address use out of the statement, so the temp can share a dead slot: a frame 8 B short went exact.
- diff_candidate masks data: a wrong literal with an @ref can score 100.00 and only acceptance refuses it ("Unsupported allocatable data section"). Read bytes at the bound address.
- check_plans refuses a rename spec and source plans in one run. fragile_probe must run under the project Python; system python3 reads `fail` on every line.

## Frame sort model (Scout767 sim.py)
- The second frame sort is K&R qsort on -sum/size. Its input is slots ordered by size descending, then the head variable's refs ascending, then last ref ascending. This reproduces 23 of 25 slots on Explosion_Damage; use it to compute which ref change a slot swap needs.
- Length with z, y, x double locals reproduces the `fld st(0)` web with Z loaded first.
- A second assignment to a float can be a separate variable when its slot is reused by a temp.
- `lea eax; mov ecx,eax` appears with two read struct copies of one member.

## Callee conventions and threaded zero-caller rows (Scout766)
- A callee's own convention changes /G5 scheduling after the call: last stores stayed ahead of the epilogue pops only with a no-arg `__fastcall` callee. Probe callee conventions when an epilogue interleaves.
- Tail cross-jumps stop when arms end differently in source order.
- Scratch registers rotate with hidden temporaries: one named pointer local before a call cleared a 15-row rotation.
- A chain ending in `x == N ? a : 0` can be a threaded inline of a zero-caller row with one more arm.
- 0x73FEE0's bad declaration window is about -15 to +17 around that row.

## Named results and template direction (Scout768)
- Naming a call result before the next call flipped a callee-saved tie on two strength-reduced cursors (85 to exact). An extra plain call or a live read also flips it; dead reads and a conditional call do not.
- Calling one level down protects a head site from later spends; an alias helper for that call is not the same.
- Template comparison direction is per site: `a > b ? a : b` versus `a < b ? b : a` chooses `setle`/`setl` and `jg`/`jge` independently; argument order can fix a site without a second template.
- Slot grouping depends on the position of a variable's last ref: declaration order does not move it, but moving `++i` before a dtor does.

## One-reference colour edges (Scout769)
- A swap of two callee-saved colours can sit on a one-reference edge. Probe by replacing one reference with a global, site by site, or adding one read: this separated the fault from form changes in about 20 compiles.
- Reference sensitivity is live-range based: removing a variable's last use flips colours where removing earlier uses does not. Too few refs spills the variable to memory.
- Named copies after a threaded chain are coalesced; a copy between the chain and its test stops threading.
- Inlined rows can hide behind hand-expanded tails: check the unit for rows with the same statements.
- A wrong form can read high only because branch targets realigned: check loose and the row diffs.

## Tail refusal and blocks at colouring time (Scout770, Scout771)
- A last site gets all remaining budget, so added root statements only grow it. Wrappers that move real statements out of the root flip it naturally; a 2^n grid of wrappers and accessors at about 2 s per compile finds the state.
- Moving a wrapper boundary swaps two sites: refusing one frees the budget that expands another.
- A named by-value event, `EventClass join(...); f(join);`, reorders big frame slots and gives `lea esi` rather than `mov esi,eax` on the ctor result.
- Repeated stores that cross-jump to one instruction still exist as blocks at colouring time: an else-if chain and a `||` test give identical bytes but different register ties.
- diff_candidate can score 100.00 with a wrong binding; only check_plans shows it.

## Colouring-time blocks and return counts (Scout771)
- A `||` split into repeated arms produces no byte change but adds blocks before colouring, which can flip a tie. Probe with a lab `if (zz1) zz2 = 1;` in the span of one competing web. Scout769's one-reference probes may partly measure this.
- A third `return` sends an early return far and pins saves in the entry block; with two returns the early return stays inline with its own epilogue and saves sink below it.
- Mid-function `push ebx`/`pop ebx` is ordinary (244 accepted rows): treat it as an outcome.
- An int from an inline `if`/`return` at an argument position allocates differently from `x != 0` or `x ? 1 : 0`, which fold alike.
- A byte-typed non-bool parameter takes an int with no conversion; bool never does.
- diff_candidate shows truncated string names, and `@ref` with a truncated name scores 100 yet is refused at acceptance: read full names from the object's symbol table.

## Hidden accepted rows (Scout773)
- A parked row can hide an accepted row of the same unit behind hand-expanded code. Evidence: a pointer held in a callee-saved register across a call where the member would normally reload. This gave +16 points at once on a 3 KB row: sweep parked rows for it.
- `money >= cost` versus `cost <= money` keeps call order and sets `cmp` operand order and `jl`/`jg`.
- Declaration order of two spilled variables sets their reload order after `rep movs`, slots unchanged.
- `setcc; test al,al; jne epilogue` with no `mov al,1` means `return hit`, not `return true`. `sete/test` survives only while the bool is live on the false edge.
- Scout773 sbs.py: a difflib realign on register-normalised rows reads better than the aligned diff when blocks shift.

## Alternate forms of accepted rows (Scout772, Scout774)
- An accepted row can have a second exact form that matters only when inlined: a named `char *other = f(entry->Name);` kept 0x672F90 at 100 and took a caller from 89.14 to 94.42. When a row is inlined in a parked row, try alternate forms of the accepted row too.
- Right-to-left argument order explains `x++` scheduling: a wrapper reading the entry before `names++` fixed it.
- A default-ctor site after a by-value local acts as a pending site: it refuses a depth-4 callee and keeps a later depth-3 one.
- A charged site can flip a later site even when itself refused.
- Row-local 100.00 can hide breakage in callers: score caller rows before freezing a header-level form.

## This-key parity (Scout775)
- `[reg+reg]` orientation of a strength-reduced index can follow a parity-like tie on this-based memory references before the operand's first read; it does not follow T, data keys or declarations. Candidate for Exit_Object's add sites, untested.
- `int i = 0;` at the top joins the zero constant web with a loop index; `i = 0` after an inline memset splits them.
- A dead zero-trip loop still counts as a this-key where a folded arm does not.
- Inline expansion of a dtor at an early-return site can flip distant ties through its local reads: a confounder when reading orientation.
- Scout775 hid.py TARGET LEN lists accepted rows under 200 B whose normalised stream sits inside a target.

## Expression temp spills and byte arguments (Scout777)
- `if (W(current) < W(next))` with named int copies of both arguments, loaded before either call, gives the temp spill `mov edx,eax ... mov [slot],edx ... mov edx,[slot]; cmp eax,edx` (85 to 99); a named weight local does not.
- Passing a table int to an `unsigned char` param directly gives `mov dl,[table]`; a named int copy gives a dword load and `push eax`.

## Library templates and scratch rotation (Scout776)
- A hidden library template can be the missing spend: real ATL CComObject::CreateInstance and its folded FinalConstruct arm took a row from 73.93 to exact. Check for a real library wrapper before hunting labs.
- Moving real reset stores into a root helper before the first site flips later refusals by changing root size.
- A named read just before a call changes scratch rotation: allocation runs eax, ecx, edx, so a whole-function rotation can come from one hoisted read.
- `cond ? f(x) : ""` initialising a Wstring gives a copy ctor plus temp-flag dtors; slot order wants the implicit WideString conversion.
- Root sites move refusals monotonically: a deep site refused with a later shallower one accepted can't come from the root; the site lies between them.
- Moving a local class view into a header can renumber `_$S`/`_$E`, shown only at acceptance as "Unsupported allocatable data section .CRT$XCU".

## Loop exit values and member-wise statements (Scout778)
- When a pointer's exit value is computed in the inner guard (`add ebp,edx` before the loop), the source is `source[index]; index++`, not `*source++`.
- A named `int offset = ...` before an inner loop, used as `target[offset + column]`, gives `[ecx-2]` cursor code with the base calculated before the guard; a named pointer does not.
- `W/2, H/2, +X, +Y, +dx, +dy` order means member-wise Point2D statements.
- `mov edi,[home]` just before an `if` at the tail means a new variable, not the parameter reused.
- To load an array element before a virtual call, declare `int house = ...; int sum = ...` in that order.
- `if (name == 0 || *best == 0)` threads the first test past the second.
- Tool traps: `grep -v` on an @ref misses because `$` is an anchor, use -F; with two conflicting @ref lines the row-local score can read 100 while acceptance takes the first binding; a false inventory end shows as "expected_bytes 24".

## Inlined early returns and by-value homes (Scout779)
- `if (!g) return 0;` and `if (g) {...} return 0;` are byte-identical as a row; inlined, the first puts the failure path ahead of the arm (`jne far`), fixing a block layout that looked like a moved loop.
- A struct by-value param keeps param homes out of the frame pool; with int params a memory local takes an enregistered param's dead home. A target frame 16 B larger with a local in its own slot points to Point2D by value.
- `return a != 0 || b != 0;` at a tail gives `xor eax,eax` / `mov eax,1` next to `mov al,1` returns elsewhere.
- A real wrapper over the last statements moves IL out of the root and can refuse several earlier sites at once (60 to 85).
- `T *const &` on a wrapper param rereads the caller's memory local at each use with no copy register (87 to 97.82).
- The kept return tail tracks the last return in source; a keeper mid-function stays unexplained.

## comip bodies and temp contexts (Scout780)
- Giving comip's `_AddRef` a body lets budget alone call a pair of rows in one block and inline them in another (68.83 to 78.05 with Length).
- The generic `UnknownComPtr<IPiggyback>` gives the target's inline destructors with Release_Held; a specialization does not.
- `(contact==0 || proceed==true)` keeps a bool's store and `cmp byte,1`, because threading crosses only the contact test.
- Temps in `if` `&&` chains share a slot; temps in a value context or `?:` do not.

## Tail copies and the scratch cursor (Scout781)
- A single statement at a function end reached by fall-through is copied into every predecessor after allocation, so all copies share one register: the likely source of repeated final statements, including twin doubles (to 1664/1664).
- Scratch registers roughly follow a cursor across blocks: a block's first register depends on the previous block in final layout order, not source order. `if (!x) { ...; return; }` first can fix a block head.
- `else { Cell what ... }` lets a local share the first slot with earlier sibling arms.
- In tube blocks the count is read after the coordinate is built, and `CoordStruct(exit, 0)` in place keeps the Z immediate.

## Eviction forms and fild slots (Scout782)
- A one-call inline wrapper over a whole body keeps a pending callee out of line at both sites of an inlined member (70.80 to exact).
- Named `int` or `short` flags get the eviction form: a register at the arms and an edge reload. bool stays in memory in the same function.
- `(green)|(blue)` in that order evaluates blue first; `(blue)|(green)` or `|=` spills differently.
- A fild temp sits at +4 of an 8-byte double slot, explaining a one-dword frame hole next to it. A 12-byte temp can overlay 4-byte class-object slots.

## Depth refusals and slot maps (Scout783)
- A function auto-inlined at depth 2 stays a call at depth 3: one real wrapper level kept a static out of line where root pressure never did.
- fa.py with /FAs gives names per slot and vs.py gives variable to true slot with push depth removed: these found fixes on a 6.7 KB row. A top-level `coord` used by one arm plus a new local in the end block sets 12-B slot order; block-scoped `{ for (int i...) }` loops swap two 4-B slots.
- `Point2D delta` plus `double x, y` in a 2D distance puts dx and dy into a 12-B slot.
- `(b + a) / 2` versus `(a + b) / 2` flips one site's add orientation and register web.

## Dialog proc forms (Scout784)
- A whole-struct copy keeping a dead field store (`RECT area = client;`) can change who wins a register. Dead `Rect at = screen` fields appear as a surviving width store.
- An inline helper taking (parent, control) evaluates right to left.
- `(unsigned int const &)x` keys use x's own home; `(unsigned int)x` makes a temp slot.
- A helper-returned ternary as a call argument gives two tail-duplicated calls.
- `int result = -1; if (...) result = ...; return result;` gives the shared tail with eax set first.
- HIWORD's `(unsigned long)` cast gives `shr` with no `and`.
- A zero constant web ends where a variable initialised to 0 coalesces with it; declaring it earlier turns `cmp x,ebp` forms back into `test`.

## Written-out helpers and table scores (Scout785)
- A row's file-local inline helper can be written out with no byte change in that row, yet flip an esi/edi tie in callers that inline it (85.07 to exact).
- diff_candidate reads 91 or 95 on an exact row with a large table: score the code length only.
- "Unaccounted emitted code section bytes" can mean an out-of-line inline with no binding; Scout785 cov.py OBJ PLAN lists such sections.
- On aligned frames an unaccessed low dword is not a 0-ref variable, since 0-ref slots sort high. pm.py compares slot partitions independent of frame shift.

## Cross-jump exemplars (Scout786)
- Scout786 cj.py lists accepted rows with `cmp; jmp` landing on a jcc, which finds real source shapes for cross-jumps.
- Nested fall-through running to one `break` keeps this copy and the later copy jumps back; an explicit `break` keeps the later copy. `return 0` on only one of two copies blocks the merge; on both it merges to the later keeper.
- After a `Rect at = screen` struct copy, a field read through `at.Height` enregisters and drops its store, while a never-referenced field keeps its store.
- `at.Y += ...; at.Height = height;` keeps height live, so the at.Y load takes edx.
- Removing an explicit `ValidateRect; break` from an arm lets the tail copy fix that block's scratch register.

## Return registers and switch tails (Scout788)
- `mov eax,edi; pop...` at every return means `result = x; return result;`: conditional returns jump to one final block (46 to 56).
- Moving a large case last can leave its layout position unchanged while moving inline decisions. The after-switch tail is pulled up with its loop and stays attached to one case.
- A bool set true and only conditionally cleared gives the eviction reload `mov bl,[slot]`; a read-only copy folds.
- Register choice in a short block can be two things: a scratch cursor for temporaries and separate colouring for named values. A lab statement rotates only the first.

## Pending-site survey (Scout787)
- On a row whose extra instructions sit inside one inlined callee, add lab Nop sites, read the window (3-8, 5-8), then fill it with natural accessors: 87.40 and 85.26 went straight to exact.
- Inlined rows in a family supply budget a private draft had to fake.
- Dropping one inline `operator==` site in a loop frees budget for a ctor at an earlier site (94.38 to exact).
- `if (Y < -20) return true; return false;` keeps a row exact and flips `jns`/`jmp` to `js`/`jmp` in its inlined caller.

## Threading order and hoisted constants (Scout790)
- A kept `test r,r` after `add` means the block still had a label when the js peephole ran.
- A second `if (save)` stops threading only when its condition differs at IL level (`== true`, `& 1`); casts, `!!`, `!= 0`, wrappers and named copies all thread.
- Passing `&local` or a reference to an inline does not keep the local in memory once inlined.
- A constant hoist such as `mov bl,0x80` needs ebx already in use.

## Flag forms and regional colouring (Scout789)
- `bool forward = true; ... result = K; forward = false; break; ... if (forward) {...} return result;`: threading removes the test everywhere except the case with a non-constant flag, and the after-switch tail lands right after that case. TibSun's `char call_default` is this.
- `result = K; return result;` in one block folds to `xor eax,eax` or `or eax,-1`; in a later block it gives `mov eax,edi` tail copies.
- An inline boundary stops threading: `T *data = Get_Record(window); if (!data)` keeps the zero init and test; plain `Get(key, data); if (!data)` threads past it.
- Targets colour per region: a variable can hold a register only in one switch tree.

## Batch labs and store order (Scout791)
- Scout791 lab.py and lc.py compile hundreds of whole-body variants in one TU as vK functions and parse /FAs: 700+ variants of a 1.8 KB body in 34 s. Confirm hits with s.py.
- VC6 reorders stores within a block, so source store order is not preserved. A one-call `mov r,imm` hoists past exactly one neighbour in every form tried.
- Any `T &r` local inside a block moves a store ahead of memset setup, whichever store it aliases.
- In a sum with a folded if/else or ternary, the operand tree is canonicalised before source grouping: named copies and order are neutral.

## All-flag tails and cold import webs (Scout792)
- `result = K; call_default = false; break;` at every site with one final `return result` gives unfolded tails; a direct `return result` after a def in the same block elides the def into eax. Const prop runs before tail duplication.
- Replacing a by-value key `.Get((unsigned int)x, p)` with an inline Get_Record(x) removes a dedicated temp slot.
- `LRESULT r = -1; if (p) {... r = Send(...);} return r;` gives `or eax,-1; test; je epilogue`.
- `push K` in a branch before a shared call comes from two full call statements cross-jumped, not a ternary argument.
- A two-use import CSE web in one cold arm can flip a global edi/ebx colouring; cold reference counts do not.
- `if (node) return true; return false;` in a bool inline threads into `if (!Present(...))`; `return node != 0` keeps `setne`.

## Guard loops and zero stores (Scout793)
- `if (n>0) { i=0; do ... }` gives a literal guard compare on the zero web and stops a for loop evicting a memory `result` into a register.
- A zero store to a memory home right after `test eax,eax; jne` becomes `mov [m],eax` when no zero register is live.
- `i=0` bounds a zero web: a later zero use starts a second one. A zero web can depend on a store inside a loop that no longer uses it in final code.

## Neg forms and last-arm sites (Scout794)
- `x = call() == 0` in both arms of an if/else, with the next statement after the join, gives a cross-jumped `mov r,eax; neg; sbb; inc` and raises that variable's rank. A shared variable with `== 0` after the join gives `xor; test; sete`.
- `int v = f() != 1; g(v, 0)` gives `dec; push 0; neg; sbb; neg`.
- A case arm last in source supplies pending sites to earlier arms: paint last did with 2-4 sites what paint first needed 22 Nops for.
- `while (n) { if (w <= limit) break; ...; }` inside `if (n)` keeps a single bottom `test; jne`.
- Colour flips can come from register-count or graph effects, not weights: 100 extra reads were flat, dropping one named local flipped.

## Direct-member offsets and bound Unions (Scout795)
- `Point2D(x, y) - Point2D(radar.X, radar.Y)` with direct members gives X through a CSE'd `lea` and Y through `[this+off]`; `pixel -= r.Top_Left()` routes Y through the base pointer.
- A nested Union's sum tie flips when the inner result is bound to `T const &`; a by-value local adds copies and loses.
- An X-offset read through a named ref folds into the base web; a Y or Z read adds one web ref.
- First-attempt rows can hide wrong operator shapes: 73.96 to exact in a few variants.

## Position scans for colouring (Scout796)
- A lab 2-use local inserted after every statement (233 positions, about 12 minutes, state signature) finds which arm controls a global colouring when weights are flat; it found a natural fix in one pass.
- Named argument locals at a many-argument call can swap two global registers with no change in the arm's bytes; both locals were needed.

## Cross-jumped arms and induction bias (Scout797)
- A duplicated reachable else arm that gets cross-jumped still counts: it won a loop-hoisted register (82.63 to 98.70).
- Zero-store order for flags follows reference counts; a `flag = true` after an inner if/else counts once and VC6 duplicates it into both arms.
- Index-form entries bias the induction variable to the most-used field. `i = 0; if (i < count)` puts pointer inits after the guard and keeps the later count test, which `if (count > 0)` threads away.
- A named local receiving `a - zero_ext(b)` evaluates b first; the same subtraction in a condition evaluates a first.
- Direct struct members at distinct offsets let a store move past a load; pointer casts do not.

## Semantics before colour (Scout798)
- A higher-scoring draft can have wrong semantics: a variable reassigned in one arm was missed by five lanes. Read each arm's register writes against the target before trusting a score.
- Frame differences can be pure reference-count swaps.
- A position scan should cover 2+ use labs just after a call; a 1-use lab at the same spot differs.

## Member forms as spend (Scout799)
- Member `a.Intersect(b)` versus free `Intersect(a,b)` is a natural budget spend: one or two sites flip a later site's inline to a call, all three overshoot.
- Cell_Center (by-value Cell) under Coord_Snap has no ctor temp; Cell_Coord (const &) adds one plus frame and calls.
- An unused bool result of an inlined Coord_To_Pixel lets its nested pure inline vanish, so identical sites expand differently.

## Kept fst (Scout801)
- A block-scoped float stays unrounded on the FPU stack; inline helpers taking float, double or const&, and members of a local float struct (split into scalars), do not produce a kept `fst [m]` followed by stack reuse. A float read in a later block gives `fstp` then `fmul [m]`. `(float)` casts in a population row do not change it.

## Case-scoped buffers and dictionary members (Scout800)
- A large buffer declared inside one case block shares its slot with an inline temporary in the post-switch code; at function scope the frame grows by its size.
- Calling an inline WolDictionary member (Get_Value, Get_Pointer) leaves Key_Hash out of line as in the target; a hand-written Find inlines the hash.

## Frame alignment switch (Scout802, lab)
- A function that would get `and esp,-8` (double locals) drops to plain `sub esp` when one local aggregate with 4 or more scalar members (int[4], struct of four ints, short[4]) carries about 13 references. 13 references over only two members, two Point2D locals, a three-field CoordStruct (even at 19 references), four named scalars or address-taken scalars do not flip it; removing a call elsewhere can. When the target has no alignment, look for a real small array or 4-member struct local.

## Index loop versus pointer walk (Scout806)
- `for (int i = 0; i < count; i++) p[i] = f(p[i])` is strength-reduced to a biased pointer and down-counter initialised after the `jle` guard, and leaves a callee-saved register free for a longer-lived variable. A pointer walk with its pointer set before `for (i = count; i > 0; i--)` keeps the init before the guard and spills it.

## YR twin adoption (Scout803)
- Reading a member through a nested struct member (`a.b[i]`) instead of a plain member reverses the add operand order (0x630B70).
- A vector-constructor static array needs `@ref ??_H@YGXPAXIHP6EX0@Z@Z=0040aed0` plus a comment-only emission row for the element constructor (0x4B25E0); a function-local static with an empty destructor needs its `_$E` row (0x66A280).
- ra2-decomp's YRpp property reads (PositionCoord) map to our inline accessors such as Position_Coord_004f3f40(), which produce the visible CoordStruct temp.

## From the ra2-decomp guide (2026-11-06)
Credit: gibbo101/ra2-decomp AGENTS.md ("What has worked", "Known dead ends"), measured on YR gamemd.exe with VC6 SP3; compared with these notes in build/ra2decomp/rules-compare.md. Techniques new to these notes, as hypotheses: confirm on our target and add the address where one is seen. Where they disagree with an entry above (inline budget, declaration steering, rewording, out-of-line sites), the entry above stands.
- Declarations: varargs callees are `__cdecl`, and caller-popped pushes around an ECX call can belong to a following varargs call. A pushed double whose low dword is 0x60000000 is a widened float literal (write `1.4f`); an st0 returner is `double __cdecl`. `setne` on a call result means the callee returns `unsigned char`; `cmp; sbb; inc` marks an unsigned operand. `or ecx,-1; or eax,-1; push; push` is a by-value default argument such as `Point2D(-1,-1)`.
- Evaluation order: for double `a * b` the right operand loads first. `V[Count++] = x` stores the element first; an inline `operator[]` or `int n = Count; Count = n + 1; V[n] = x;` stores the count first. An instruction one place off around `?:` can need the shared operation moved into each arm.
- Branches: a sparse byte-index switch drops break-only cases and falls back to a compare chain; list every in-range case and send no-op cases by `goto` to a label after the switch.
- Frames: a small buffer declared in a loop body gives `push ecx`; at function scope it does not. Two frame objects pinned together are members of one local struct (steering here unless the struct is evidenced).
- Inlining: the last switch cases of a very large function can be scheduled less fully, and a leaner earlier case fixes the tail. Template inline helpers can crash VC6; write one overload per type. `member.Type::Type();` in a constructor body builds the member in statement order with no null test; placement new adds the test.
- Constructs: `(char)(x & 2) >> 1` keeps `and; shr`. A `(float)` cast on a double gives `fcomp dword [mem]`. A bare `ret` in a non-void function is an empty body with C4716 disabled. Inline helpers keep their null tests when the test is the `while` condition; a body `break` lets VC6 fold them. Old for-scope: give each loop variable its own name. A constructor storing base then derived vptr does its work in the base constructor. A guard re-tested at each use is a function-local static in an inline accessor. A self-jumping free `operator==` comes from a non-const member `operator==`. A run of header-static initializers starts a unit's address range; the run after the last function belongs to the next file. Byte-identical instances take symbols in COMDAT emission order. One address called with two argument types is two overloads the linker folded. Consecutive statics with destructors share a `$S` guard byte; a function definition between them starts a new byte. A pushed file and line is `#line N "path"`; a first-character test is `if (*name)`.
- Headers: MSVC never reuses a base's tail padding. VC6 has no `__declspec(align)` or anonymous unions; a 4-byte bool is BOOL. An override differing in constness adds a vtable slot. The linker folded every `return 0` body into `_purecall`, so a `_purecall` slot may be a real `return 0`. Files with asserts compiled out get `#define NDEBUG` before their includes. Dword reads of a short global happen only at offset 0 of an extern aggregate or a 4-aligned .bss definition (.bss ordered by a name hash). A member function writes `::Name` for a global the class shadows. Inline structors in shared headers of classes with vector members shift every includer's `$E`/`$S`; define them in the owning file.
- Dead ends: no parameter type turns pushed immediates into register pushes; `delete this` in a deleting destructor adds a virtual call and a null test.

## Declaration order of real locals (Scout810)
- VC6 numbers locals in declaration order; that order picks spill slots, the load order of register-held locals and the operand order of commutative adds between them. TrackBarHandler 0x5FB270: the twin's order 98.62, `maximum` and `thumb_pos` declared ahead of range/value/minimum 100. A permutation of real locals is natural source. Scout811 swept it over ten near misses at 86-99 (perm.py) and it moved none: where VC6's weight-based slot order decides, declaration order does not break the tie. Expect it to help only where weights are equal.

## Cell comparison form (Scout812)
- Member `Cell::operator==` and `TPoint2D::operator!=` written `X != r.X || Y != r.Y` (the YR twin's form) sit one inline level shallower than a free `operator!=(Cell,Cell) { return !(a == b); }`. The extra level shifts VC6's temporary numbering at every comparison, which flips each later DynamicVectorClass::Add's `Length()+GrowthStep` operand order and the tail merges after it (Plan_Route 0x707F60: 91.20 to 99.90 under RA2_COORD_AS_CELL). Check this on parked rows in populations using the free form.

## Compiler version (Scout812, Scout816)
- The original was compiled with VC6 SP3 (C2 build 8447; the project used RTM 8168 until 2026-11-06). The two back ends agree almost everywhere; known differences: SP3 keeps the count-store-first order in some inlined DynamicVectorClass::Add tails (Plan_Route 0x707F60), and SP3 emits an unsigned loop exit (`jb`) for a signed counter compared with an unsigned bound where RTM emitted `jl`; the original source there compares signed. A row that plateaued under RTM on a scheduling or register tie may simply need SP3.
- Scout820: reading a vector through the owning session object (`Session.Players`) instead of a global alias of the same vector flips the inlined Add's growth-sum operand order (0x760C00), the same nested-member rule. Reading a field directly instead of calling its inline accessor can free the budget a later inline expansion needs (Read_Game_Modes 0x5B8490).
- Found in the rebuild (lane R53, 2026-11-20): the operand order of an expanded vector member is not set by vector.h or by the access form. With one header and the same `Session.Players.Add` text, exact rows show both growth sums (Host_Game 0x7459A0 `add eax,ecx`; MPCooperative 0x5A5720 and 0x5A6040 `add ecx,eax`). The image's out-of-line copies (21 Add, 177 Resize rows, all exact under the TibSun/YR body) are uniform: every struct-element Resize copies through `lea ebx,[eax+edi]`. Only an expanded copy differs (WDT_Write_Map_Ini 0x72A880 `[edi+eax]`). A local reference to the vector or to Session, 1-8 unused int locals, and 1-4,096 declarations before the includes all leave both sites unchanged. Main reached the image with a separate global at the vector's address (Scout820). The rebuild cannot use that, because it would give one object two definitions. Work these rows as function-context ties (data-key count, temporary numbering), one function at a time, not as a header change.
- Scout824: reading a bitfield struct's bits (`Occupy.Building || .Vehicle || .Monolith`) instead of the byte mask (`& 0xE0`) still merges to one `test al,0xe0`, but the extra IL uses raise the base pointer's register weight and can move `this` to the original's register (Desired_Load_Dir 0x704730, esi to edi).
- Scout826: the twin's uninitialised locals plus a chained `y = x = 0;` before the switch fixed a zero-register rematerialisation (ebp) that initialised declarations could not (Calculated_Cell 0x49A700).
- Scout838 (C2.DLL root routine 0x1072E7FB): the root inline budget is 2*S clamped to [1000, 35000]; the running total starts at S and refuses sites past 35000. A callee's IL size is held in a signed 16-bit word, so lab spend probes above 32767 wrap negative and are always accepted.
- Scout855: moving one case body into a file-static helper that /Ob2 inlines (the twin's `Choose_Campaign()`) changes the register reloads at a later loop back-edge even though the inlined code is the same; a twin's file-static helpers are worth reproducing even when they inline away (Select_Game 0x5135F0).
- Scout854: a counter-fragile row can depend on the declaration numbers of the data it references (where that data's extern or its header is first declared in the unit), not on its own text or on rows near it. Putting the unit's includes together at the top of the first row, as the original .cpp has them, and using the real literal instead of a stand-in extern gave those declarations fixed early numbers and made Generate_Urban_Buildings 0x587CF0 stable.
- Scout857: an operand order can depend on a class's symbol index, i.e. how many declarations precede the class in the unit (GetObstacle 0x4BBAF0: a window about 9,600 declarations wide, reached in the original by its full SDK include set). Declarations after the class have no effect. Dummy declarations are steering; the natural fix is including the headers the original included.
- Scout857: a caller's loop shape in an accepted inlined helper (`for (i = 0; i < count; i++)` vs the twin's `for (i = 0; ; i++) { if (i >= count) return ...; }`) changes temporary numbering at the inlined site and flips a later add's operands in the caller (Try_Diagonal_Shortcut 0x428CF0 via Follow_Path 0x427060).
- Scout865: frame slots sort by density (references per byte) with ties broken by declaration order; an anonymous fild spill counts. Slot sharing follows the size and kind of temporaries the expression form creates (an 8-byte Point2D difference shares a call's return buffer where a 12-byte CoordStruct does not); nothing inside a loop shares with an address-taken object of the same loop.
- Scout863: a stand-in `extern double const` for a constant the original wrote as a literal (here 1/UINT_MAX, pooled at 0x7A5DD0) is a memory operand VC6 cannot fold, and it changes register reloads in the caller that inlines it. Check every extern-constant stand-in against the pooled __real literal at the same address.
- Scout870: with literal FP constants VC6 reassociates products (two constants fold into one, a negative constant turns an add into a reversed subtract, a constant can move to the end); explicit parentheses keep the written order. Which form the original used is a per-site shape choice (Angle_Score 0x582970 exact only parenthesised; Randomized_Cell_Distance rows exact only unparenthesised).
- Scout874: VC6 SP3 at /O2 drops unreferenced function-local `static const int`s but emits all of them, in declaration order, under /Z7 or /Zi. Runs of such constants in .rdata (Dropship's block at 0x7A0990) are evidence the unit was compiled with debug info; it did not change that function's code.
- Scout878 block relocation rule (labs in build/class-work/Scout878/): a run of blocks entered only by jumps and ending in an unconditional transfer, sitting between a block X ending in `jmp T` and T, is lifted and placed after the next unconditional transfer after T, so X falls into T; a run whose last block falls into T stays. Identical copies are merged before layout (the later survives), so a duplicated tail can trigger the rule; a copy differing by a propagated constant (`push 0`) is not merged. A goto target is laid out after its last goto. A flag tested after the if with `if (!flag) A else B` keeps the chain in place where `if (flag) B else A` relocates it. A spilled variable that forces a reload at a join makes an arm end in a jmp and get moved to the function end.
- Scout885 (measured IL, VC6 SP3): VectorClass ctor 138-140, DynamicVectorClass ctor 76-78, VectorClass::Clear 74, Add 142, Delete(T const &) 60, Delete(int) 102-108, Add_Head 182-194, VectorClass operator= 186-188, Is_Present 78; destructors, DynamicVectorClass::Clear, operator[], Count and Length are free (40 or under). Westwood's forms differ only by small body-text amounts.

## Symbol-count windows for register ties (Scout886, 2026-11-07)
A register tie between two equal-weight sums can follow the number of symbols created before one operand's local (locals, inline parameters/locals, and distinct global data keys incl. global struct fields and string literals; not `this->` fields, functions, double literals or immediates). AnimClass::AI 0x421710 flips on count mod 32 in [13,29]; symbols declared after the local do nothing. Measure with a dead-local calibration (Scout886 cal.py), then look for a real inline helper that supplies the count; dead probes never land.
Limits (Scout891): in some rows (Exit_Object) the window follows the total symbol count regardless of position, and two ties can want incompatible counts; in small functions extra symbols change the frame size first (Place_Tiberium), and some ties are not count-driven at all (0x5E9410). Calibrate before assuming the mechanism.

## Direct field reads change later sum grouping (Scout894, 2026-11-07)
Direct reads of a local struct's fields (`cell.X`, `cell.Y`) earlier in a function let VC6 reassociate a later `(X + Y) - b` into `(Y - b) + X`, losing a sum shared with a test; the same reads through inline member accessors do not. When a sum/operand-order miss sits after direct field reads, try the twin's accessor form (RTactical Action 0x62FE30).
Second case (Scout898, Infantry Scatter 0x504120): the same direct reads decide whether a later product reuses its operand's register (`imul edi,[g]` vs `mov eax,[g]; imul eax,edi`); the head of the function, not the product site, decides. Compile the twin text with its own headers and cut regions out to find which region decides.

## Early return vs wrapping block changes store scheduling (Scout901, 2026-11-07)
`if (p == 0) return X; <body>` and `if (p != NULL) { <body> } return X;` emit the same branches but schedule differently: with the early return VC6 kept copy stores in source order against later loads through `*p`; inside the wrapper block it moved a store past the loads (AI_Building 0x4E9660). Try the twin's block structure when a store order is the only miss.

## A user-declared base destructor makes a ctor site charged (Scout903, 2026-11-07)
Under /Ob2, a constructor of a class whose base has a user-declared destructor is a charged inline site; with no such destructor it is free. TechnoClass NoInit 0x6C1F10: eleven CDTimerClass members built on BasicTimerClass (user dtor) spend the budget, so the VectorClass ctors stay out of line as in the original. When ctors in an init list inline that the original calls, check the members' class hierarchy against TibSun/YR.

## An inlined callee's source form sets the caller's ties (Scout899, 2026-11-07)
When a helper the original never calls is inlined into a big caller, the helper's own source shape (locals, references, loop form) feeds the caller's symbol counts and register ties. Dropship_Screen 0x4A6B90's two ties were set by Find_Type_Index 0x4A6B50's form (inlined at five sites): re-rolling the callee (still exact on its own) fixed the caller. Before inventing sites in the caller, check which accepted rows are inlined into it and try their natural alternative forms.

## Westwood `return(expr);` parentheses in inline accessors (Scout913, 2026-11-07)
In inline accessors, Westwood's `return(expr);` and a bare `return expr;` can schedule the caller differently once inlined (Bullet Draw_It 0x4610D0: 100 vs 98.91, fixing two distant misses at once). Use the Westwood spelling from TibSun headers verbatim, parentheses included, before concluding an accessor shape is wrong.

## Merged vs separate empty case labels (Scout911, 2026-11-07)
`case a: case b: break;` and `case a: break; case b: break;` emit different jump tables: merged labels let VC6 thread both entries past a later known-value test, which keeps the switch variable in a register across the table; separate labels give each entry its own target (and, in Take_Damage 0x6CDC40, the original's reload). When a switch row misses only on a reload or table target, try splitting or merging empty labels as the twin writes them.
Refinement (Scout914): `return(expr);` changes scheduling only when expr already has the function's return type (a double sqrt returned as double, a `(double)(...)` cast); with an implicit conversion around expr the parentheses are inert. The effect is per site and can go either way, so copy TibSun header text site by site; never parenthesize a whole header.

## Field signedness decides immediate caching (Scout929, 2026-11-07)
A struct field's signedness can decide whether VC6 caches a test immediate in a byte register: AudioEvent +0x18 as signed `volatile int` lets an inlined loop hold 0x80 in bl (`mov bl,0x80; test bl,al`), unsigned does not. Header-bisect minimal TUs one declaration at a time; field types from the twin headers are evidence.

## Spill-slot assignment order (Scout944, 2026-11-07)
For equal-size spilled webs, VC6 creates frame slots in processing order (first web processed -> first new slot), and a later temporary joins the newest compatible slot. Among webs first defined inside the same loop, the earlier in-loop first definition is processed later (dead definitions count; definitions outside the loop do not). Use a /FAs frame map with ref counts (build/class-work/Scout944/fa.sh) before trying declaration shuffles.
Further (Scout946): moving declarations changed no slot in five rows (first definitions and weights decide); a dead initialisation of another variable adds no ref to the target web; address-taken locals in one block scope never share a slot even with disjoint live ranges, while compiler temporaries share with either.
Density (Scout969): frame slots are laid out sorted by density (total references divided by slot size), denser slots at lower addresses; the sort is not stable, so exact density ties fall to slot creation order. Count references per slot (build/class-work/Scout969/fa.py) before reordering source.

## Frame-slot model (Scout972 lab, 2026-11-07; build/class-work/Scout972/MODEL.md, lab/, lab.py)
Measured with VC6 SP3, superseding the partial rules above where they disagree:
- R1 processing order: frame variables (spilled webs, temps, address-taken locals) by size ascending, then refs descending, then earlier last reference. Declaration and first-definition order do not matter (40/40 and 30/30 random checks). Scout944's in-loop definition rule did not reproduce in isolation: for webs defined in a loop and read after it, the later use's argument evaluation order decides.
- R2 slot choice: first compatible of dead argument slots (first-dead first), then existing slots newest first, else a new slot.
- R3 size classes 1B/2B/4B/8B/>8B = 0..4; a variable joins a slot only if within one class of the slot's largest member (so 12-byte temps can only join slots holding an 8-byte member).
- R4 a local whose address reaches a real call is live over its enclosing block from the block's first call.
- R5 argument slots are not reused when the frame with reuse would be 112 bytes or more.
- R6 frames over 128 bytes keep pass-1 slots and re-sort them by (sum of member refs) / slot size, densest lowest; R7 the sort is the K&R qsort (middle pivot, Lomuto, strict <) over pass-1 creation order, which decides density ties.
- R8 placement runs from the frame top downward in reverse list order.
- R9-R10 `and esp,-8` is triggered by a non-address-taken double kept in memory with about 4+ uses; then double slots are 8-aligned and rounded up (4-byte holes).
R2 refinement (Scout978 lab): dead argument slots are offered in R1 order too (access size asc, refs desc, earlier last reference); "first-dead first" is only the last tie-break; a bool read as a dword sorts as 4 bytes; R3's class rule does not seem to apply to argument slots.
Further (Scout980 lab, build/class-work/Scout980/MODEL2.md): T1 tail merging runs after frame layout and the merged-away copy's references still count (a duplicated `return a;` tail adds a layout-time ref); folds before layout do not. T2 dead stores are removed before layout (no ref) but still change register-allocator order. T3 a reload group at a loop exit is ordered by the register allocator (priority, first appearance breaks exact ties), not by a frame tie-break.
T1 limit (Scout982): VC6 does not cross-jump multi-block tails that end in a return, so duplicating such a tail in source leaves extra code; only single-block identical tails merge.

## Shared epilogue and jump threading (Scout989, 2026-11-07)
VC6's shared return epilogue is the one ending the last block in the layout. A block can sit after the shared exit only when its join is jump-threaded past the next test (a flag known false on that path): `if (!p) {..flag = true..} else {B} if (flag) {R; return 0;} return 0;` puts B after the exit with its own epilogue copy, and early returns land after R. This refines the "last bare return written" rule, which holds only where VC6 does not thread or move blocks.
R4 refinement (Scout990 lab): an escaped local stays live past its block when a pointer variable is assigned inside that local's scope (from a call result or a load) and dereferenced later; it stays live up to the last such dereference. Passing the pointer to a call does not extend it; assigning it after the block does not.
Condition temporaries (Scout994): the 12-byte temporaries of an `&&` chain share one frame slot when the chain is an if condition, but interfere (one slot each) when the chain initialises a `bool x = ...;` declaration, with identical code; use the form that gives the original's slot count.

## Multi-path flag set: one statement gives the register form (Scout997)
A flag set on several paths (register on the set paths, memory home reloaded at the join) gets that form only from a single `if ((a || b) && !c) flag = true;` statement; two separate set statements leave it in memory, and term order matters. Seen at UnitClass::Assign_Destination 0x7053F0 (useplan).

## Header include order inside a joined unit (Scout1014)
A row that becomes a unit's first includer of a header moves where that header's include guard fires. Defines that later rows place before their includes (VECTOR_IMPLEMENT_*, accessor and delete-variant switches) then come too late, and a template member is only declared, so a neighbouring row calls it out of line (Destroy_Null_Connection 0x5CEBA0 68.12). When a new row breaks a neighbour, bisect its includes one header at a time before suspecting the inline budget; move the defines to the new first include point.

## new[] size reuse is matched only to `a[n - 1]` (Scout1019)
After `int *a = new int[n];`, VC6 reuses the computed `n*4` for an `a[n - 1]` index (any spelling of -1), not for -2 or -3, and never for a variable defined after the call. When the original recomputes the address after the call, name the value separately after the allocation (`int total = count + 3; ... a[total - 1]`) so it is a distinct value (OwnerDraw 0x5E5150).

## Memory-resident loop variables block strength reduction (Scout1020)
If the target's inner loop recomputes `base + index` in each arm and reloads base/offset with no pointer induction variable, the source variables were memory-resident (address-taken, struct fields or similar), not register candidates; a named local index is always strength-reduced to a stepping pointer (CacheBMP 0x68C040 diagnostic).

## Initialisation form changes the inline cost (Scout1024)
`Rect area = Rect(a, b, c, d);` and `Rect area(a, b, c, d);` cost differently under VC6's ~175-unit auto-inline cap: the copy-initialised form brought a by-value Point2D helper (0x536C70) under the cap where the direct form did not. If-return chains with no else and a trailing `return 0;` are cheaper than if/else chains, which restores a neighbour's inline decision when the budget is at its edge.

## Function IL size gates inner-loop reassociation (Scout1022)
A function's own pre-optimisation size (not the unit's, not code position) can decide whether an inner loop keeps `base + index` address arithmetic or strength-reduces to stepping pointers: CacheBMP 0x68C040 matches only within a narrow size window (calibrated with a dead `if (0)` block: 11-21 extra increments). When a loop's shape is right in a diagnostic padded variant, look for natural code-neutral source (inline helpers, accessors that fold) that the original carried; never deliver the padding.

## Frame layout counts final instructions, not IL references (Scout1039 lab)
Lab-verified (614/614 functions; build/class-work/Scout1039/lab): R1's reference count is the count of references left in the final instructions. A split read-modify-write (`p += 7` on a memory web: load/add/store) counts twice; a reload the allocator removed (reused from a register) and uses served from the def register do not count; memory webs get no loop weighting. Homes of split webs (stored at the def, reloaded at a loop exit) are ordered by the loop-exit reload order, which comes from the allocator: for non-constant definitions it is reverse definition order; it does not follow use counts. On real accepted bodies this still leaves R1 at about 77% of held-out tie functions (Scout1032/1039); not a 95% rule, so use it to read a target's frame, not to predict one.

## Split-web reload at an emptied if-join counts once per incoming edge (Scout1041 lab)
A register web with a frame home that needs a reload at an if-join with no statement of its own gets one reload per incoming edge at layout time (VC6 merges them into one `mov reg,[home]` afterwards), so the frame order uses store + edge count, not the 2 references the listing shows. Lab reading (14 shapes, build/class-work/Scout1041/lab c0-c29): one edge per then/else block ending at the join plus one for the condition-false exit; a short-circuit condition is one exit; an inner if after another statement adds an edge; a statement after the if (`if (A) S; T;`) removes the effect; loop exits count once unless the pre-test bypasses the reload block. Seen in 0x411D80 and 0x710BB0. Adds about 1 point on held-out ties; use it to explain a target's frame order.

## Register priority: what counts (Scout1047 lab)
Lab facts (build/class-work/Scout1047, q.py threshold labs): (1) a use that cross-jumping merges after allocation still counts in full (`if (G){k(1);h(B);} else {k(2);h(B);}` weighs like two uses); (2) uses removed before allocation do not count (a repeated test of an unmodified local, `else if` on the same value, dead stores and expressions), which is why neutral probes never flip ties; (3) loop temporaries that cross no call can hold esi/edi during allocation and end in eax/ecx/edx, leaving a callee-saved register unused and unpushed, so a "missing" competitor can push a named web down (a two-pointer `previous = cursor` loop pushes `this` off esi); (4) conditional weights depend on context (if/else arm ~0.5-0.67, `if (G) h(B);` without a call ~1.0, a call arm followed directly by another if ~1.4, nested if ~0.9, loop body 1-2). A greedy 3^depth-of-final-refs rule predicts only 46% of held-out functions (85% of forced pairs); use these facts to explain a tie, not to predict one.

## Inlined DynamicVectorClass::Add orientation is a whole-function count effect (Scout1065 lab)
Whether a growth site emits `add G,L` (sum in the GrowthStep register) or `add L,G` is not decided by element type, Add text, caller or local register pressure (756 corpus sites). Functions with 26 or fewer named locals (counting constant-folded, address-taken and inlined callees' locals; not unnamed temporaries) give G; from 27, the order depends on the number of distinct memory locations read, repeating every 32 (the same family as the mod-64 declaration counter). Stack-local vectors give L. The generic Add text in vector_class.hpp is the right single form (438/440 functions); do not change it tree-wide. Lab pairs: build/class-work/Scout1065/lab. When a row's last diff is the Add orientation, look for the natural local or read count the original had, not a different Add.

## A named constant local is propagated after reassociation (Scout1074)
With literal constants VC6 reassociates an index such as `row * 2 + ID * 60 + col` into `(row + ID*30)*2 + col`. When the count is a plain named local (`int columns = 2;`, used at its natural sites: the loop bound and both index expressions), the constant is propagated only after reassociation, giving the original's `a*2 + (b*k')*4` form and its spills. StripClass draw 0x67F800 became exact this way. The reverse also happens: a draft that keeps a constant in a register where the original uses immediates may have a named local the original did not.

## Division by a negative constant depends on the destination (Scout1078)
VC6 rewrites `x + c*h/-2` as `x - (|c|h)/2` when the result goes to a plain int local, but keeps the `shl / sub / neg / shl 1` form when it goes to a struct member (`top.Y += 30 * h / -2;`). Accepted Rectangular_To_Isometric 0x6A2740 keeps the neg form the same way (`iso.X += y*60/-2`). If a target shows the neg sequence, the original wrote into a member.

## Each return statement takes its own scratch-rotation step (Scout1082)
When several by-value struct returns differ only in the register used for the copy, the original probably had one shared return: each `return` is its own web in the eax/ecx/edx rotation. Storing each result into one local and `break`ing to a single `return local;` reproduced Pixel_To_Cell 0x6A6BD0's registers. Also: a struct copy's constant-zero member (`tmp.Z = coord.Z` where Z is 0) becomes a register web; a literal 0 lets VC6 cache the constant in a callee-saved register across a call for a later store of 0.

## Reading an operand into a local first fixes a subtraction tree's order (Scout1079)
VC6 canonicalises `a + b - c - (flag ? 26 : 18) - 7` into the `c`-first tree in every if-form spelling (48 leaf orders tried). Reading the strip top into a local before the 26/18 term is assigned (`int top = abcd64; int reserved; if (flag) reserved = 26; else reserved = 18; return (a + b - top - reserved - 7) / 50;`, YR twin Max_Visible_Cameos) gives the original's reserved-first order with the if-form registers; assigning `reserved` before `top` reverts it. Ternaries give the right tree but load the flag early (eax/edx swap). Sidebar slot 34 0x6819A0 became exact this way.

## What advances the scratch cursor (Scout1086 lab)
VC6 gives eax/ecx/edx to two kinds of temporaries. Cursor temps (single-use loads, new arithmetic results, array elements, one half of a struct copy) take the next free register after the previous cursor temp, eax -> ecx -> edx, skipping busy ones. First-free temps (a value tested by a branch, a value with two or more uses, a memory read-modify-write) take the first free register and do not move the cursor. No step: values loaded into fixed registers (call results, ecx/edx arguments, cdq), immediates, in-place results, locals across calls, register-variable `i--`, dead code, unused results, move-only inline bodies. The cursor runs in source order through the whole function and is not reset at calls or joins. To shift a later load without changing the listing, make an earlier load first-free (a second use or a branch test), or use a cross-jumped duplicate tail (still counts after merging). Held-out prediction ~68% per load; use it to explain a shift. Lab: build/class-work/Scout1086/lab.

## A flag-collapsed return keeps duplicated arms apart (Scout1099)
When several arms of the original end in the same `return expr;` but each copy uses a different scratch rotation (ecx/edx, eax/ecx, edx/eax), the original wrote the return once behind a flag: `bool rock = false;` ... each arm `if (Is_Rock) rock = true;` ... `if (rock) return rock_z + adj - 1;`. VC6 colours the single return block, then copies it into each arm after colouring (jump threading), so the arms keep distinct registers and do not merge. Separate `return` statements in each arm merge in ours instead. TechnoClass Get_Z_Adjust 0x6CFEA0 81.20 -> 91.41 this way.

## A named double local forces `and esp,-8`; a float local does not (Scout1099)
VC6 aligns the frame to 8 (`and esp,-8`, `this` moves out of ebp) when the function has a named `double` local. Declaring the local `float` removes the alignment; without /Op the value stays at full x87 precision and spills as an 8-byte compiler temporary (`fstp qword` then both dwords pushed), which is what the original shows. Compiler temporaries never trigger the alignment. By-const& facings, double-returning helpers and unnamed angles did not remove it. Walk 0x71E220 57.72 -> 74.48 (`float angle` in Move_Coord).

## Declaration-count ties are windows mod 64, not shifts (Scout1114)
Rows that flip with header declaration changes (Print_CRCs 0x62A360, AI_Build_Defense 0x4F1190, Sidebar 0x6809F0/0x6819A0) are register or reload ties between two symbol numbers compared mod 64: one a header-declared symbol the function uses, one local. Calibrate with K `struct P;` lines before the function (lab only): 0x4F1190 has one bad window (k 25-50 at +0x332), Print_CRCs three sites (26/64 good). Declarations before both symbols move one window edge, declarations between them move the other, so window width changes too and per-edit bookkeeping cannot predict a combination: measure each combination (build/class-work/Scout1114 combo.sh). A redeclaration counts 0, a new overload 2, a typedef 1, a template forward declaration 0. Held header edits land together when the combination keeps every row in its window (Scout1114 r1).
- Found in the rebuild (lane R20, DoType; lanes R21, R33): evidenced content can flip a row by count alone. do.hh's
  DoType grown from TibSun's 16 sequences to RA2's 38 flipped wonline Draw_Player_List 0x73FEE0 and sidebar
  SelectClass::Action 0x6809F0, which use none of the new values. The probe that tells a count flip from a content flip:
  add N unrelated declarations to the old header and record windows on both sides (over the 38-entry header: +1..+5
  Draw_Player_List off, +6..+9 both exact, +10..+11 Encode_Game_Options off, +12..+15 exact, +16..+17 Draw_Player_List
  off, +18..+23 exact). Such rows are parked as "exact form, count-sensitive" with their windows (DECISIONS, third part
  of the flip rulings) and come back by themselves: Draw_Player_List was exact again under wonline's new includes
  (lane R21) and again after its inline twins left (lane R33). `rebuild/tools/countsens.py` retries every listed row
  at a landing that changes a header; nothing is padded to move a count.
- Found in the rebuild (review session, 2026-11-19): weigh the probe by body, not by count. The compiler's counters
  advance by what a declaration creates (a new signature 2, a typedef 1, a redeclaration 0: Scout627, Scout1114;
  defined locals rather than declarations: Scout1296), so N bare `struct P;` lines and N evidenced members or
  enumerators are not the same move. Record a probe of matched weight where it is cheap before calling a flip
  count-only.
- Found in the rebuild (lanes R21, R22): rows that are not count-sensitive say so quickly. MapRegionClass::Build_North_Ramp
  0x573680 (1..30 probe declarations) and Run_Lobby_Session 0x743720 (0..60) kept the same difference at every count:
  their causes were a missing `short` local (ra2-decomp's twin) and an out-of-line ArrayList::get, so run the probe
  before spending time on header order.

## SEH (`__try`) functions lay out their frame by variable name (Scout1135)
In a function containing `__try`, VC6 does no density re-sort and no slot sharing: every named local gets its own slot (even register-allocated ones), and every statement temporary keeps a slot even when eliminated (each `x = Rect(..)` temp leaves 16 unreferenced bytes). Scopes stack top-down: function scope, nested blocks depth-first in source order, one group of parse-time temporaries, inlined functions' scopes (in inline order), then inlining temporaries. Within a scope, locals sort by a 16-bucket hash of the name, ascending from the top; within a bucket the later-declared variable sits higher; use order never matters. The bucket (Scout1137, 86/86 measured names plus random scopes) is `h = (h<<2) + (h>>4) + c` over every character as 32-bit unsigned, then `bucket = (h ^ (h>>16)) & 15`; `build/class-work/Scout1137/hash/bh.py NAME...` prints buckets and `bh.py -o` predicts a scope's top-down order. Statement temporaries always form one group below the whole named-scope tree, in creation order, never shared (all option variants); every `new` leaves a 4-byte temp (a pointer initialiser costs its named 4 plus that temp; corrected by Scout1139), `delete p` costs 8, `GetExceptionCode()` in a filter costs 4, and `T const &r = T(..)` puts its temp in the named scope. Named locals declared inside if/else branch blocks form nested scopes placed after the parent's whole list; each inlined function's own temps sit right after its own scope. So renaming a local is a real frame lever in SEH functions (WinMain 0x68E280: `retval` in bucket 12/13 placed the return code between path_to_exe and dir). The ordinary frame notes do not apply to SEH functions.

## x87 term order follows whole-function variable weight; flat + chains are re-ordered (Scout1169)
VC6 reorders an unparenthesised flat `+` chain into its own order: every permutation of `X*X+Y*Y+Z*Z` compiles identically, inline and out of line; parentheses stop the reordering. The x87 operand order of such terms follows how heavily each variable is used across the whole function, so two loops that use separate velocity objects can differ from an original that shares one: TibSun's single function-scope `TVelocity3D velocity; velocity.Set(0,0,0); Set_Yaw; Set_Speed; ...; Set_Pitch` per iteration fixed Shrapnel 0x462C50's operand orders, a hoisted vtable load and a scheduling tie at once. Declaration counts, dead locals and dead statements never changed these orders.

## A by-value Point2D cursor keeps two frame slots and an in-memory increment (Scout1198)
When the original keeps X and Y of a depth-buffer position in two adjacent frame slots and increments X in memory inside a pixel loop, the source walked a by-value `Point2D zpos(xstart, y - TacticalRect.Y)` with `zpos.X++` (WaveClass Draw_Laser 0x722CB0 and Draw_Sonic 0x7225B0 slow branches), not a per-pixel `Point2D(x, ypos)` temporary or an int `xpos`: VC6 merges an int position with the loop counter into one induction variable and the frame comes out 8 B short. Both rows' frames match with the cursor (0x40, 0x3c).

## Write `coord.As_Cell()` where the twin does, not `Cell(coord)` (Scout1204)
A coordinate-to-cell conversion through our `Cell(CoordStruct)` constructor and through TibSun `Coord::As_Cell()` produce
different code in a joined unit: Get_Obstacle 0x4BBAF0 sat at 97.80 for that one expression and went exact with two of its
three uses written as `As_Cell()` (TibSun point.h TPoint3D text under RA2_POINT3D_TIBSUN). In any near miss with a
coordinate-to-cell site, try `As_Cell()` first.

## An inlined accessor on a by-value temporary stops CSE of unrelated global loads (Scout1210)
Reading a returned-by-value RGBClass through its inline accessors (`line.Get_Red()`, RGB_DECLARE_ACCESSORS) makes the
temporary address-exposed, and VC6 then re-reads unrelated globals (the hicolor shift words at 0x852FB8...) after each
construction; direct field reads let it CSE them. Draw_Depth_Antialiased_Line 0x4AC650 re-reads `mov cx,[0x852fb8]` in all
four loops only with the accessor form.

## /Ob2 inline budget, measured costs (Scout1211)
Root budget 1000 IL units per caller; a site inlines while its cost fits the remaining budget divided among the inline sites from
it to the end of the body (N). Measured: DynamicVectorClass ctor 134; an empty `TypeList() {}` is itself charged (52-60);
CDTimer/TTimer(NoInit) ctor 52; a class-typed array (any length) 50; PauseTimer default ctor 56 (60 with TibSun's
`CDTimerClass(int set=0)`); `FixedString(NoInit) {}` 0. An address-only inline accessor in the body counts as a site (raises
N). The caller's own body size does not change the root budget. Lab tools: build/class-work/Scout1211/ (pat.py reads per-site
inline/refuse patterns; one score can hide different patterns).
Root budget is size-bound: B = max(1000, 2S), S the caller's own IL. Adding non-site IL (stores) to Draw_Placement_Cursor 0x473F20
makes Find_Object inline once S rises ~48 units, position-independent. Code-neutral evidenced spellings can supply IL: ra2-decomp's
`CoordStruct coord = CoordStruct(MapCoords, 0);` is byte-identical and adds 18-30 units (Scout1211).

## Register priority is references minus live-through pressure; a losing long web is split and its register lent (Scout1212 lab)
A web's colouring priority is its weighted reference count minus a penalty for the register candidates it is live
through: about 0.25 per other candidate referenced in a conditional block of its range, about 1.3 per candidate in a
depth-1 loop block (lab grids: B needs nA + K/2 uses to beat A past K `if (G) H = 1;` blocks, nA + 2.75K past K
`for` loops; straight-line calls or stores anywhere, 1 to 2,400 of them, never change it). The penalty is subtracted,
not divided (thr = nA + K/2 for nA = 2..8), so Chow-style size normalisation is the wrong model. Candidates that
count: named locals and inlined helpers' locals live in the region, loop counters, a repeated constant (`push 1` or
`H = 1` in several arms is one web spanning them), a tested value kept live into its arm; not one-reference
constants or plain global loads (lab L: `int t = Hk; h(t); h(t);` in eight arms costs 2 uses, `h(Hk); h(Hk);` costs 0, two locals cost 4). Colouring is greedy in that order; a web that finds no free register over its
whole range is split: pieces where a register is free take one (its frame home is written at the split), the rest
lives in memory with scratch reloads, and the freed register serves shorter webs and reloads in the busy region.
Minimal reproduction lab/K/k_2_00.cpp: `int endy = g(count)` as an outer loop bound with two uses after the loops
gets esi in prologue and epilogue only, esi is lent to a `yoff` reload and the inner counter inside the loop, the
tail compares `[endy]` from memory and `mov esi, ecx` restores it after the loop; the short in-loop `ypos` that
finds every register taken in its one region is spilled whole. This is the Draw_Sonic 0x7225B0 original (ebp lent
to xoff/yoff/ypos, endy restored from [esp+0x20]) and the AA line's `this` (esi for Lock/fail-Unlock, ecx for
Stride). When our draft keeps the long web in a register unsplit, its own form is not the lever (every long-web form
is inert): look for the candidates the original had live along that range (a local or helper temporary live across
the loop, a repeated constant, a value carried from a test into an arm) and for weaker in-loop webs in ours. A real
local live across a loop costs every web live through it about 1.3 per loop block, which is why one named local can
reorder long webs (the count-calibrated-locals effect).
Refinement (Scout1212 lab M): the penalty counts callee-saved candidates (values live across a call or a loop) referenced in any
block of the web's range, weighted by block frequency: about 1.3 per candidate in an unconditional block, 0.25 in a conditional
arm, more in loops. A local used once (scratch) or loads without a local cost 0; "straight-line code never counts" means plain
code, not candidates in straight-line blocks.

## MIN/MAX macro clamps versus if-clamps (Scout1212)
A `MIN(a,b)`/`MAX(a,b)` macro clamp keeps both operands as temporaries across the compare and stores the result once at the join;
`if (x >= b) x = b;` spills x at both definitions. The macro operands are callee-saved candidates that can push a loop bound below
the in-loop webs (Draw_Sonic 0x7225B0: four macro clamps make the original lend ebp to ypos and the xoff/yoff reloads; 42.65 -> 61.38).
When a target computes the clamp bound first and stores once, write the macro.

## One declaration, several exclusive loops: one frame symbol with summed references (Scout1212)
A variable declared once at an outer scope and assigned in several exclusive loops is one frame symbol whose reference count is the
sum over its webs; it is processed earlier and takes the first dead argument slot or an earlier local slot, and its later webs join
it. Per-loop declarations give separate low-count symbols that scatter. When the target's dead argument slot holds the same loop
variable from several branches, the source declared it once (Draw_Sonic 0x7225B0: y, zval, points declared at branch scope,
61.38 -> 68.12). A dead argument slot goes to the spilled web with the most memory references among compatible ones.

## Frame-slot sharing is path-based memory liveness; order is size class, refs, last reference, with an allocator residue (Scout1215 lab, 2026-11-10)
Measured on 440 accepted exact functions (small non-aligned frames; build/class-work/Scout1215 corpus.py/stats.py,
simulator fsim.py over the /FAs listing's CFG). Two symbols may share a slot when their memory live ranges (store
defines, load uses, byte-granular, computed over the control-flow graph) never overlap: webs used only in exclusive
branches share even when their instruction index ranges overlap (Draw_Sonic 0x7225B0: `right` with `dest`), so
linear first..last intervals and listing "last line" proxies are wrong models; use a liveness pass. Processing order
holds as Scout972 R1 states: size class ascending (95% of cross-class slot pairs), references descending within a
class (86%; 89% among register-web homes), earlier last reference on ties (80%; 75% among webs). Loop-weighted
counts are worse on every subset, and declaration order is not the key (39-43% on ties). The 11-14% residue is the
homes of split register webs (stored at the def, reloaded at loop exits), whose order the allocator sets
(Scout1039): Check_Map 0x569B60 orders mzone(12), from(5), this(7), file(6), which no final-count key gives. A
joiner takes the newest compatible slot (84% of web joiners; the rest are upstream order errors, not a different
choice). A by-value 8-byte struct passed to a by-value callee is scalar-replaced into two independent 4-byte webs
(the named symbol keeps one member, a `$T` the other, not adjacent; lab s8.cpp); a `double` stays one 8-byte symbol
and is processed after the whole 4-byte class; a cursor whose member is incremented in memory (`zpos.X++`) stays one
8-byte symbol. Dead argument slots have no proven rule yet (77 takers: newest-dead 18, lowest-address 16, both 16,
neither 27 under memory liveness of the parameter); the two hand-checked rows (Draw_Dashed_Line 0x773C30, Draw_Sonic)
fit "newest-dead compatible slot, parameter live while its value sits in a register". Use the simulator to read a
target's frame (which symbols the original grouped, which slot a web joined), not to predict one: on Draw_Sonic the
best key reproduces our own layout at 503/528 same-slot pairs and the original's at 482/528.

## Reloads merged from several edges count once per edge in the frame order (Scout1215 lab, 2026-11-10)
Refines Scout1041 and explains part of the "allocator residue" in frame-slot order. When a register web with a frame
home must be reloaded on several incoming edges of a join (the failed arms of an `&&` chain, skipped if-arms), VC6
inserts one reload per edge at layout time and merges them afterwards into one block that contains nothing but the
reload (`$L9490: mov ebx,[from]` falling into the join). The frame order uses the pre-merge count: such a reload counts
once per predecessor edge; a reload at the head of a block that carries a statement (a call, the loop increment, a
loop header) counts once. Check_Map 0x569B60: `from` has 5 listing references but 2 + 1 + 4 + 2 = 9 at layout, above
`this` (7) and `file` (6), giving the original order mzone, from, this, file; removing the fprintf arms one by one
(build/class-work/Scout1215/lab/CM) moves every variant exactly as the rule predicts. Isolated split webs otherwise
follow size class, refs desc, earlier last reference (204/204 lab functions); the loop-exit reload group is emitted in
reverse definition order with the last-defined web reloaded at its own later use, and webs without a pre-loop use are
reloaded at use in use order. On 440 accepted functions the per-edge count raises web order agreement from 74.0% to
75.1% (exact layouts 56 to 60 of 127); implemented as key `r1b` in build/class-work/Scout1215/fsim.py. Still open:
ties whose last reference is a loop-exit reload (Load_MFCI 0x77A4C0 orders by the last reference inside the loop) and
Draw_Sonic's third slot.
Addendum (Scout1215 lab W3): when two equal-reference webs are reloaded together after a loop, the one whose first use inside the
loop comes earlier takes the lower slot; definition order and post-loop use order never change it; webs only carried through the
loop are reloaded in reverse definition order; webs unused in the loop are reloaded at their uses in use order.

## x87 store scheduling against integer stores depends on code before the block (Scout1216)
Whether VC6 /G5 lets an x87 `fchs/fstp` chain slide past independent integer stores (e.g. a struct `rep movsd`) depends on code
before the block, periodically: each extra local defined before the block and consumed by a call (or a distinct global store) steps
the schedule through a cycle of three states; plain calls, loops, function size and code after the block are inert. Treat it like a
declaration-count window: find the original's state with evidenced forms, never with filler (Draw_Voxel 0x6FF690 / slot 311 0x7005B0).
Correction (Scout1216 unit 3): symbol numbering is not the counter (unused declarations are inert); the lab's period-3 stepping is the
scratch-register cursor choosing eax/ecx/edx for each copy. Once registers match, the residue is the scheduler sliding fstp/fld past
independent stores, perturbed hash-like by address-taken temporaries before the block (Draw_Voxel 0x6FF690 stays 99.54).

## The loop-exit restore group of lent webs is emitted in descending web weight (Scout1217 lab E)
At a loop exit where several lent webs are restored, the reloads run in descending web weight; a depth-2 use of an enclosing-loop
counter ranks it first; candidate counts in its range do not move it (Egos 0x4B37C0 frame/step order; build/class-work/Scout1217/lab).

## Candidate text for docs/VC6_MATCHING_NOTES.md (owner's call; from Scout1218, 2026-11-10)

### A narrowed subtraction chain's operand order is a whole-function property of its variables (Scout1218)
In `zval = (unsigned short)(MaxValue - a) - b - 2` VC6 picks which of `a`, `b` goes into the 16-bit first
subtraction (and which is folded as a word memory operand) by a whole-function property of the two variables, not
by the written order or by local register residence: Draw_Sonic 0x7225B0 writes the forward chain with zpix first
and the two backward chains with endy first; our draft gives starty first / endy first, and a named copy
`int position = zpix;` before the forward chain fixes the forward order but flips BOTH backward chains (where the
copy is unused). Declaration count (0-8 header `struct X;` lines) and the inline `Get_Scroll_Delta(int position)`
spelling (its parameter copy is substituted away for a plain variable argument, also with `return(...)` and an
unsigned member) do not move it. Integer twin of the x87 term-order note (Scout1169); when such a chain is the
last diff, look for the variable's other uses across the function, not at the chain.

### The original's third Draw_Sonic slot (Scout1218, refines Scout1212 "summed references" and Scout1215)
Draw_Sonic's slot 0x08 is opened by `xstop` declared once per Direction arm (4 + 4 = 8 references, processed after
y 13 and xoff 12); zpix (4) only joins it as the newest compatible slot. Together with per-loop `dest` (TibSun form,
4 references per web, so starty 7 opens 0x0c next) and the backward fast loop tail written in the forward loop's
statement order with signs flipped (`zptr -= zwidth; dest -= stride; zval++; points--;`), 68.12 -> 71.04, frame
slots 0x00-0x0c = the original's membership. Still open (strict refs-desc violations no count key gives): starty(7)
created before the equal-count forward xstop, a 4-5 reference backward web opening a slot before right(5), and
index8/zwidth opening one before surfptr/yoff; declaration order and position are inert.

## Narrowed chain order is an inventory tie, landed by an evidenced local elsewhere (Scout1218 round 2)
Lab (build/class-work/Scout1218/lab/C): for `(unsigned short)(M - a) - b - 2` the later-defined operand takes the
16-bit first subtraction; a named copy flips it and every other chain on that variable; use counts, address-taking,
`base_z` locals and every inline `Get_Scroll_Delta` signature are inert (a plain variable argument is substituted,
no parameter copy). With several chains in one function the order flips with the OTHER chains' spelling at
identical code (one `base_z` site vs two), i.e. a tie over the function's temporary inventory. Draw_Sonic
0x7225B0: TibSun's `int zy = y - TacticalRect.Y;` in both slow loops (feeding the `Point2D zpos` cursor) sets all
three chains to the original's order, 71.04 -> 79.12 (build/class-work/Scout1218/keep/sonic_H2_7912.cpp); TibSun's
full `int xpos; int zy;` cursor gives the chains but loses the slow loops' memory layout (58.03).

## A depth test written twice keeps the base in a register and dissolves a constant web (Scout1218 round 3, Draw_Tile 0x52C230)
In IsometricTileTypeClass::Draw_Tile's inner loops the original computes `BaseDepth + *SrcDepth` twice (TibSun
isotype.cpp 2386-2388: once in the `if (*DepthPtr >= (unsigned short)(...))` test, once in the store), not through a
`pixeldepth` local (2086 form): with the twice-written form VC6 loads BaseDepth into ebp per iteration and uses
immediates for the three `+= 2` pointer increments; with the local it folds BaseDepth as a memory operand and
materialises the constant 2 in ebp (`mov ebp, 2`). Loop 0x1094: bucket 0x1000 62/119 -> 99/119, draft 51.29 -> 53.49
(build/class-work/Scout1218/keep/draw_tile_U1_5349.cpp). When a loop shows a constant held in a callee-saved register
where the original uses immediates, look for a repeated sub-expression the original spells twice.

## Free-register choice order (Scout1217 labs, ~90 functions)
Webs needing a callee-saved register are coloured in priority order (weighted refs desc, definition order on exact ties); each takes
the first register free over its own range in the fixed order esi, edi, ebx, ebp. A freed register is reused in the same fixed
order (no LIFO/LRU free list, no preference for the previous occupant). A copy `B = A` does not coalesce by itself; a counter `i = 0`
after a constant-zero web shares the zero's register (and drops the xor) only when that register is the first free one over the loop.
A constant-zero web runs to its last use: a final `if (result)` on a spilled result compiles as `cmp eax, zeroreg` when the zero can
reach it. Reading rows: a register difference between builds is always an IL web-range or priority-order difference, never the
register name; list which earlier-coloured web holds each register over the mismatched web's range.

## Large-frame slot order: density, lea counts, IL last reference (Scout1214 labs)
In frames over 128 B the density key is total member references / slot size (lea address references count exactly like loads and
stores; accesses through a copied pointer register do not). Equal-size, equal-reference temporaries keep pass-1 creation order
(earlier IL last reference first; right-to-left argument evaluation, so `two(&A,&B)` and `two(&B,&A)` swap two 48-byte slots with
identical code) and are then scrambled by the K&R qsort (three slots A,B,C -> B,C,A). To fix a tie, reorder the operands or
statements carrying the tied temporaries' last references; never add references.

## Scratch-cursor refinements (Scout1218 round 10 lab; build/class-work/Scout1218/sc, lab_A/B/C, rd.py)
Extends Scout1086's rule (cursor temps advance eax -> ecx -> edx; shared or branch-tested values take first-free and do
not advance). Measured on the `h(GA); <construct>; h(GB);` shape, GB's register as readout (baseline ecx):
- No resets. The cursor runs on through a label with a backward goto, a for/do-while loop body, a switch (every case's
  temporaries count), an if/else (both arms count, in turn), a forward goto over a statement (the skipped statement's
  temporaries still count), a nested if and an early return (lab_A: 18 shapes, all exactly the summed count mod 3).
- Rotation, not last-freed: three single-use loads return the cursor to its start (rr3), a value held across other
  temporaries keeps its register while the cursor skips it (live2/3/4: `t = X` ... later uses) and the next free is
  taken in rotation order after the busy one.
- Allocation order is the IL order after optimisation, not the listing order: a copy `int t = X;` propagated to its
  use is allocated at the use (live3: X's load, first in source, is emitted last and takes the register after Y and Z);
  a value with two IL uses takes first-free even when one use is folded in place (live1/live4: `G = Y; ... Y + 1`
  puts Y in eax and does not advance).
- Steps per construct (lab_C, mod 3): ternary `X ? Y : Z` 2 (test first-free, two loads); `(short)/(char)` load 1;
  `X / Y` 0 (eax/edx precoloured), `X % 3` 1; `X * Y`, `-X`, `~X`, `X & 7`, `X >> 2`, `UX * 3` 1 each; `(X + Y) * Z` 2;
  `PA[X]` 0 (= 3), `*PA` and `PA[2]` 2, `PA + X` 0 (= 3), `&PA[X]` 0 (= 3), `PA[X] + PB[Y]` 0 (= 6); `&G` as an
  argument 0; `(double)X` 0, `(int)GD` 0 (ftol result), `hd(GD)` 2 (the two dword halves), `hd((double)X)` 0;
  `X > Y` 2, `X == 0` 1, `X && Y` 0, `(X > 3) ? 1 : 0` 1; `f(X)` and `h(X)` 1 (the argument load), `GV->vf()` 2
  (object and vtable loads), `GV->vg(X)` 0 (= 3); `GSTR = "lit"` 0; `GS = GS2` (8 bytes) 2; `GS.a = X` 1; `PA++` 0
  (memory RMW); `UX >>= 2` on a global 0 (memory RMW).
- Reading a one-step residue: the two compiles' prefixes are identical code, so the extra or missing step is a
  classification difference (a value single-use in one IL and shared in the other) or an IL-order difference (a
  propagated copy allocated at its use), never a reset. Draw_In_Area 0x41F1C0: the cursor simulated over the prefix
  lands on eax after `point.X`, which gives the original's ecx for r3.Y; ours has one more temporary in the r3 window;
  `Rect r3(point, 60, 30)` (TibSun TRect(TPoint2D const &, T, T); needs RA2_WDT_MAP_LOAD_LAYOUT) is byte-identical,
  named Point2D operands identical, r3 as an unnamed Intersect argument 32.43, r3 built before r1 35.84.

## Switch table kind follows the number of case groups (Scout1217)
VC6 picks a byte-indexed jump table or a direct table from the number of distinct case bodies (groups) in the source: in
DisplayClass Mouse_Left_Up 0x49B0F0, 34 bodies give a byte-indexed table and 40 or more a direct one; ungrouped bodies that are
identical cross-jump back into one block, so instruction counts stay equal. Note: diff_candidate decodes our jump tables as
instructions, so scores on switch-heavy rows are only roughly comparable (count with fa.py, which skips DD/DB).

## By-value struct temporaries in frame layout (Scout1214 labs)
`T __fastcall operator*(T const&, T const&)` returning a non-POD 48-byte T: ecx = hidden return, edx = first operand, a chain's
second operands pre-pushed right-to-left before the first call, the next product takes the result via `mov edx, eax` (a POD T pushes
the hidden pointer and copies every chained result into a second temporary, so POD labs are not faithful to Matrix3D). Layout-time
references are exactly the listing's esp-relative ones (one per lea or direct access; consuming a returned pointer adds nothing;
merged tails still count). Pass-1 order among equal temporaries follows the IL pre-push order: outermost second operand first, the
innermost last, then the hidden-return temporaries in call order.

## Spill-home creation order follows reload position, not colouring priority (Scout1218 round 14 lab; build/class-work/Scout1218/ft, gen2-gen7.py, fsim actual layouts)
Under heavy call-crossing pressure (six to eight long-lived locals), two whole-spilled webs with equal reference
counts take their slots in the order of their reloads in the code: swapping the use order swaps the slots
(dAB_uABBA vs dAB_uBAAB, three webs likewise), while swapping the definition order (the def stores) does not
(ctl_AB = ctl_BA), nor does a rename (A -> Z), nor doubled uses served by one reload (`h2(B, B)` twice), nor
candidates in a loop that only one web is live through (preA/preB c0-c3: both webs keep their order, only the
joinable loop temporaries change). So the frame pass does not see the allocator's priority: it orders by (size
class, references, reload position), consistent with Scout1215's "earlier last reference" (in these labs each web has
one reload, so first and last coincide). A load folded into a compare could not be forced under this pressure (the
compared web keeps a callee-saved register), so whether a folded operand counts as a reference stays untested.
Draw_Sonic 0x7225B0 consequence: the original's 5-reference group right (0x14), zwidth (0x18), surfptr (0x1c),
yoff (0x20) is exactly the last-reference order (0x4b4, 0x537, 0x60f, 0x620) once `zwidth` is one 5-reference symbol,
i.e. `int zwidth;` declared once at the branch scope like y/zval/points (u1zo, score-neutral until the other two
slots move). The two remaining violations (starty before the equal-count forward xstop; the dest/zptr/endy group
before right) are count-level in the original, not tie-level: both resolve at once if folded compare operands
(`cmp edx,[xstop]`, `cmp eax,[right]`) are not counted (xstop_f 6 < starty 7; right 4 < zptr 5), which is the lab
left to build with a spilled compared web.

## ebx is withheld from dword webs when byte webs need it across a call (Scout1217 labs F9/F10)
A dword web live across byte-typed temporaries that must survive a call (`unsigned char lo, hi` used across a call) is coloured into
ebp, not ebx; with int-typed temporaries stored as bytes the usual esi, edi, ebx, ebp order holds. ebx, the only callee-saved register
with byte halves, is reserved for the byte webs; byte temporaries outside the dword web's range do not matter.

## Which invariant global stays hoisted across a loop with calls (Scout1214 lab al1/al2)
With two loop-invariant global pointers and calls (__ftol) in the body, VC6 keeps exactly one hoisted and re-reads the other at each
use: the one whose wrap/step statement comes last in the loop body is kept. Stores of any width, pointers derived from either global
and member stores do not change the choice (not aliasing). Both stay hoisted only when one's pre-loop web reaches the loop without a
call in between.

## Invariant global loads across sibling loops are one function-wide web each (Scout1214 labs al6/al7)
Which loop-invariant global loads stay in registers per loop is decided by register-allocation priority on one function-wide web per
global, not by a hoisting budget; a global that loses is rematerialised from its address at each use (never a stack home). A priority
change anywhere (an extra reference in one loop) re-orders the competition in every loop. A per-use reload pattern in the original
means a competing web or a missing reference somewhere in the function, not in that loop's text.

## Candidate (round 17): store position across a branch is source order; `for` counters are two webs
- A global store that sits in the basic block after a wrap's `jb` (Draw_Tile 0a78 RowAlpha) cannot have been
  scheduled there: VC6 does not sink stores across branches, so the statement follows the wrap in source.
- `for (int n = *p; n > 0; --n)` compiles to an init temp (tested, in eax) plus a copy into the counter's home
  (`mov reg/mem, eax` right after the `jle`); `int n = *p; if (n > 0) do {...; --n;} while (n != 0);` is one web
  with no copy. Use the `for` form when the original shows the copy.
- Dead parameter slots are spill homes (Draw_Tile: image/run/row in cell_variation's 0x60, row in brightness's
  0x58). A reconstruction that names the parameter instead (TibSun isotype.cpp `- (unsigned char *)drawer`) is
  a calibration, not evidence of the source.

## Address webs: scratch if coloured before block temporaries, callee-saved if after (Scout1217 labs G/H)
A multi-block address web with no call in its range (lea of a member, byte reads, a store) gets the first free scratch register when
VC6 colours it before the block's temporaries, and a callee-saved register when it is coloured after them. With bl unavailable,
byte pieces move to other registers via `xor r16,r16; mov r8,r8` zero-extension.

## Candidate (round 17f): dead parameter slots as spill homes
VC6 (/O2 /G5, esp frames) reuses the stack slots of parameters that are already dead as homes for spilled
webs. The slots form a pool ordered by the parameter's death point (its last read in machine order; for
call arguments that is the push order, so the LAST argument of `h6(p0,...,p5)` dies first). Spilled webs,
ordered by earlier last reference (the same key the frame model uses), take the pool in that order, one dword
slot each whatever their size (a char spill occupies a whole parameter slot as BYTE PTR). A slot whose
parameter is still live when the web is created is skipped, and once the pool is exhausted fresh local slots
are allocated. Declaration order, name order and parameter position do not decide. Evidence:
build/class-work/Scout1218/ft/lab/d1.cpp, d2.cpp and their /FAs listings (out/d1.asm, out/d2.asm); Draw_Tile
0x52C230 shows the mechanism (image/run/row in cell_variation's 0x60, row in brightness's 0x58, col in fog's
0x6c, clip_right in height's 0x54, clip_bottom in subtile's 0x34, a product temp in fog's 0x6c) but its first
reuse (0x58 before 0x60) is not yet explained by the pure-spill rule.
Refinement (Scout1218 round 17g): a parameter's slot enters the dead-slot pool at its last slot read, or at the variable's death when
the parameter is assigned anywhere; a parameter rematerialised into a later expression never frees its slot.

## A pointer parameter is promoted whole or re-read per use; split webs come from eviction (Scout1217 lab P)
A pointer parameter used two or more times across calls takes a callee-saved register (one slot load, no home store) when one is free
over its whole range, at any distance. When all four are claimed over its range, even by short loop counters, it is re-read from its
slot into a scratch at every use; VC6 never fits a web into holes between shorter claimants. An original showing a parameter in ebx
with slot reloads at segment starts and holes where temporaries use ebx (Render 0x6A45A0 surface, Mouse_Left_Up 0x49B0F0 cell)
coloured the parameter before those temporaries and was evicted locally; ours colours the temporaries first. The lever is the
parameter's priority against the short claimants, not the spelling of its uses.
Refinement (Scout1217 labs P5/P6): VC6 does not promote a parameter web across a stretch between two consecutive references that
contains two or more loops whose counters live across calls; one such loop, a nested pair, call-free loops or loops outside the web's
range do not count, and any reference inside the stretch resets the count. The web is then re-read per use everywhere.

- **CSE'd member-address webs rank last among callee-saved candidates** (Scout1217 lab U, 2026-11-10; build/class-work/Scout1217/lab/U1.cpp). An address such as `&cell->MapCoords` passed by reference to a call and re-read later takes a callee-saved register only if one is free when it is coloured; otherwise it becomes a scratch with a frame home. If it lands in a callee-saved register in ours, look for which scalar lost instead (Find_Path_Regular 0x427370: ours forwards from_index's def register in ecx to its first use; the original re-reads it).

- **Stored scalars are forwarded unless address-exposed** (Scout1217 lab V, 2026-11-10; build/class-work/Scout1217/lab/genV*.py). A scalar with a frame home has its first use in a call-free stretch taken from the def register, whatever the cursor temporaries, joins, cdq or declaration form (plain, struct field, array element, reference). It is re-read from its home at that first use only when its address is taken somewhere in the function (e.g. `h(&F)`). So an original that re-reads a just-stored local in a call-free stretch has that local address-exposed (passed by pointer/reference, or a member of an address-taken object).
  - Limit (Scout1220 lab W, 2026-11-10; build/class-work/Scout1220/lab/genW*.py): the rule above holds for a first use outside a following loop. When the first use is inside a loop that follows the def, VC6 forwards the def register into the loop and reloads at the latch whether or not the variable is address-exposed; an original that re-reads there means the value did not reach the loop head in a register (in the lab only a call on the path did that).

- **Loop counter vs `this`, and split webs** (Scout1221 labs L1-L4, 2026-11-10; build/class-work/Scout1221/lab). A loop counter outranks `this` until `this` has about 13 references; one in-loop push of the counter is worth 3-4 `this` references; pre-loop conditional candidate blocks count 0.25 each. In-loop candidates never reorder two webs that both span the loop (the live-through penalty cancels); against a pre-loop callee-saved local, two in-loop candidates do lower `this`. A split web's register says nothing about its colouring order. When reading a swap, find the first differing register in the listing (often a short temporary earlier in the function) before tuning the named webs.

- **/Ob2 budget is one running sum per function; arms share it; nested helpers get a divided share** (Scout1219 arm lab, 2026-11-10; build/class-work/Scout1219/arm/). If/else arms are not budgeted separately: sites are charged in source order against one sum and nothing recovers after exhaustion. Root budget is exactly 1000 for a small caller (the sum must stay below it), B = max(1000, 2S) with S the function's own IL anywhere in the body, and the pending-site count does not divide the root budget. Parameterless bodies up to ~36 units are inlined free. Inside an inlined helper the share falls with the caller's pending sites m after the helper's site, about 2B/(m+2) with ~60 units per-site overhead. So "refused in the middle, accepted after" in an original means the refused sites sit inside an inline helper (BulletClass::AI 0x45F710's ballistic step).
  - Forwarding layers (Scout1219 unit 4, 2026-11-10; build/class-work/Scout1219/arm gen5-gen8): declaration style of an inline (in-class, out-of-class, template) is inert. A callee-side forwarding layer (a wrapper member whose body calls an inner inline, e.g. Coord(x,y,z) forwarding to TPoint3D<int>(x,y,z), coord.hpp 270-282) is inlined while its inner body is called out of line once the caller has about 20 or more pending charged sites. So a tiny member with a visible body called out of line at a function's first sites marks a forwarding layer in the callee, not an arm or budget effect in the caller.
  - Qualification (Scout1219 unit 5): the forwarding-layer reading holds only where the caller has many pending charged sites; BounceClass::AI 0x4370E0 calls tiny in-class Vector3 bodies (TibSun vector3.h:79-102, Renegade WWMath vector3.h:98-131; deferred COMDATs 0x437690-0x437710) at depth 1 with few pending sites, and no wrapper/budget/order/TU-count form reproduces it; mechanism unidentified. The 12 + 6a + 4b unit scale is not additive across body sizes (two 600-unit bodies inline, 950 + 50 refuses).

- **Called tiny bodies with a sibling that expands them = per-unit /Ob1** (Scout1219 unit 6, 2026-11-10; build/class-work/Scout1219/bu/x, arm/). Cost scale confirmed: a site of K dword stores costs 12 + 6K units against B in [984, 1000); a refused last site is emitted as a tail `jmp`, not a call (count both when reading refusals). In bounce.obj the accepted BounceClass::Coord 0x436F80 inlines CoordStruct(x,y,z) while BounceClass::AI calls 0x434820 at a fresh-budget site, so budget, arms, nesting, quotas and TU shape are excluded; BulletClass::AI 0x45F710 under /Ob1 has the original's call list exactly. Such units were likely compiled /Ob1 (a recorded option, canonical.RECORDED_OPTIONS), with inline marking in headers deciding expansion.
  - Bullet unit trial (Scout1219 unit 7): population-4423 does not hold under /Ob1. /Ob1 emits Speed/Horizontal_Speed COMDAT bodies the original keeps elsewhere, and an inline-marked function whose in-unit sites all expand gets no body, while the original keeps Create_Bullet 0x463990 and Set_Weapon_Type 0x463BA0 bodies AND expands them (only an unmarked auto-inlined function under /Ob2 does that). The original's facts split across both option sets; the unit stays /Ob2.
  - /Ob1 and header statics (Scout1219 unit 8): /Ob1 emits each header static's `_$E` initializer helper as its own COMDAT, which /Ob2 expands into the HeaderStatics row. An original unit with header statics and no such bodies was compiled /Ob2 (Bounce 4034, Bullet 4423, Particle 4059 all excluded). The recorded /Ob1 precedent (audio events) must be a unit without header statics. The Vector3/Coord/+= calls in those units are /Ob2 refusals whose cause is still unfound.

- **What counts as a pending site after an inlined helper** (Scout1221 budget lab, 2026-11-10; build/class-work/Scout1221/budget, 114 variants; refines the ~2B/(m+2) estimate above). An inlined helper's share is about R/(m+1), m = number of inline call sites after the helper's site in the caller, each counted once whatever its size or charge: charged inlines, tiny parameterless inlines (free but counted), inline accessors, Count(), operator[], operator==, inline parameterised ctors, by-value struct returns, ctor temporaries, the dtor of a temporary whose class has a user-declared dtor (`x = T(a,b)` counts two), `new T[n]` with an inline ctor, `delete[]` with a dtor, forwarding wrappers, smart-pointer operator->, a nested inline helper (one). Each counts once wherever it sits (if-arm, loop; one per arm). Not counted: out-of-line or virtual calls, implicit copy assignment/construction, POD copies, by-value struct arguments to out-of-line calls. So "the original refuses a helper's inner call" means RA2's text has more such sites after it than ours: look for accessors and temporaries where our text reads raw fields.
  - Default arguments (Scout1220 lab Y, 2026-11-10): a struct temporary supplied by a default argument (`P off = P(0, 0)`) counts as a pending site exactly like an explicit `P(0, 0)`; an int default counts nothing.

- **Helper structure decides nested inlining** (Scout1222, 2026-11-10; OwnerDraw 0x5E8720, commit 49d189887). An accessor level inside an inlined helper (e.g. `Get_Record(dialog)` wrapping a dictionary lookup) lowers the nested share at that lookup, so the original's "expand the outer, call Key_Hash/Get_Pointer inside" appears. Splitting a large unmarked helper into tiny real inline pieces lets /Ob2 auto-inline the now-small body (no `inline` keyword needed). New inline helpers or changed signatures add symbols before every later function in the unit, which can flip a declaration-window tie elsewhere (0x5FB270): fix with the real header redeclaration, not filler. Prefer the ra2-decomp twin's helper structure when one exists.

- **Regional callee-saved registers** (Scout1225 labs B-D, 2026-11-10; build/class-work/Scout1225/lab). A web confined to one conditional arm (if/else with a join) whose register is not in the prologue set gets `push r` at the arm's first use and `pop r` before the jump to the join; if a long web owns the register, the arm web evicts it locally (spill at the owner's def, reload at the arm's end). Webs live after the join take prologue registers. A register enters the prologue set with no visible web when a web was coloured in the first pass and later demoted to per-use re-reads (a parameter used in call-crossing loops on both arms). Rare: two verified accepted cases (Send_FrameSync 0x626500, Reset_Radar 0x631D40). UICmnds 0x6F7020/0x6F71E0: the original never gives `name` a first-pass register; ours does.
  - Latch refinement (Scout1226, 2026-11-10; Do_Chrono_Reinforcements 0x63A880): with ONE source latch (which VC6 may duplicate) the loop-entry def register is forwarded into the loop and reloaded at each latch copy; with two source latches the def is stored as an immediate and not forwarded.

- **Return-false exit sharing follows the tail** (Scout1228, 2026-11-10; Is_Clear_To_Build 0x4718B0; toy lab build/class-work/Scout1228/lab/gen3-5.py + rd4.py, 1 s per variant). A trailing `return true;` after the last test (reachable or not) puts VC6 in copy mode: every `return false` gets its own block. An if/else tail with nothing after it gives one shared exit that later sites jump back to; a final non-constant return places the shared exit late. The else-chain order matters (`else if (post || gate) {...} else {...}` keeps that arm's copies), and a joined `||` first arm places the following block after it. A byte flag tested inside a nested `if` moves its base register; written as one `&&` chain it takes the next free byte register.

- **Inlined if-form accessor with a constant argument reorders frame-slot creation at identical code** (Scout1239, 2026-11-11; Explosion_Damage 0x47E2C0). TibSun's `Map[cell].Cell_Occupier(true/false)` member with an `if (bridge) return AltOccupierPtr; return OccupierPtr;` body, at the rocker loop, broke an exact 14/14 frame-slot density tie (same 1,345 instructions): the inlined, constant-folded branch changes pass-1 slot creation order. The twins' ternary body was one instruction longer here. A lever for density ties.

- **Struct homes rank by the whole struct's size** (2026-11-11, Create_Preview 0x61D660, Scout1254). Frame slots are ordered by refs/size, not declaration or first reference. A struct whose other members stay in registers keeps a home sized for the whole struct, so a lone stored dword among 12/16-byte temps marks a promoted Point2D/Rect, and its unreferenced tail explains "extra" frame bytes with no references. A block-scoped address-taken Rect shares its slot with earlier temps; at function scope it is live from the first call. The prologue zero-init order follows the declaration order of register-resident locals (`minx, miny, maxx, maxy` vs TibSun's `minx, maxx, miny, maxy`). Values that look like spilled ints with no raw stores are usually struct members.

- **Declaration count in included headers flips commutative operand order elsewhere** (2026-11-11, Reposition_Sidebar 0x6819A0, Scout1250). Adding one to three member declarations, or merging two distinct names into one overloaded name, in a header the unit includes swaps the load order of `g1 + g2` (two global ints) in an unrelated function of the unit; same-length or distinct renames do not. Spelling, length and hashing are irrelevant: the count of declarations before the unit is what matters. Pure renames that keep names distinct are safe; overload merges and added/removed declarations in widely included headers need the header-users gate (apply_rename catches it). Overloads stay held unless the original evidently declared them as one name. Related: count-calibrated locals.

- **Constant-zero webs are bounded by loop regions** (2026-11-11, IPXManagerClass::Service 0x525F60, Scout1268 lab of 43 variants). Before-guard, preheader+loop+entry test and after-loop each form their own zero web, coloured first-free over its own range. A loop counter's `i = 0` is never a use of the preceding zero web, whatever the loop form or init spelling; placing the counter def before the zero uses ends the web there (compares become `test`, a memory `result` gets promoted). An original whose single `xor` serves both zero compares and a later counter is a colouring coincidence: look for an extra early web holding the zero web's first-free registers, not a respelled loop.

- **`imul reg,[local]` vs `mov r,[local]; imul r,reg`** (2026-11-11, Draw_Pixel_Effects 0x6A7E80, Scout1269 lab of 45 variants). A direct stack scalar is always folded as the memory operand, and the product coalesces with the other factor, which can flip a register tie and lets /G5 hoist the product. A load into a scratch register first is the shape of an indirect operand: a global, a pointer dereference, an inline member accessor read, `volatile`, or a member of a local struct that an inlined callee writes through its address. Frame homes group by size class, so a loaded 4-byte slot among scalars is a scalar. Do not use `volatile` to get the shape (fails the real-codebase test).

- **A loop-invariant scale written on the difference is hoisted late** (2026-11-11, Draw_A_Link_To 0x6D0990, Scout1275). `((i - phase) * 8)` is distributed over the induction variable and `phase*8` hoisted as a late temporary that loses the callee-saved race (frame home, reload at use); the same value as a source variable or `(phase << 3)` is an early web that wins a register. Also: `double arc = -(t*t); ... arc + 1.0` gives `fchs; fadd`, where every one-expression spelling folds to `fsubr`.

- **A block-copied struct parameter joins a loop-head read only with two head uses** (2026-11-11, Draw_A_Link_To 0x6D0990, Scout1275 unit 2). After `last = from` (a by-value struct, copied as an aggregate), VC6 joins the copy's load of a member with the loop-head read of the same member only when the head block uses that member twice. The join shows as an entry `jmp` past the head reload, in-place shifts and the original's store order. With a single use, whatever its IL position, the head read is a fresh load.

- **Conditional blocks before a variable's definition lower its colouring weight** (2026-11-11, Per_Cell_Process 0x500AA0, Scout1280 lab). On the path to a variable's definition, about every two or three conditional IL blocks cost it one reference's worth of priority. Unconditional calls before it, and conditional blocks after it on the other path, are neutral. Copies, casts, inline wrappers, reference aliases and redundant null tests add no references; a CSE-merged second source read counts twice. So a margin outside the block can come from an IL block that folds away in the bytes.

- **A refused return-value ctor is read through its returned pointer; a refused base ctor is not** (2026-11-11, health bar 0x6C3F00, Scout1285 lab). If the refused ctor directly builds an inlined function's return object (depth 1, for example `TPoint3D::operator/` returning `TPoint3D(...)`), every consumer reads the temporary through eax (`mov edx,eax; mov eax,[edx]; ...`). If the refused ctor is a base-subobject ctor inside an inlined derived ctor (depth 2, for example `CoordStruct::operator/` returning `CoordStruct(x,y,z)`), the consumer reads the $T slot. So an original reading a quotient through eax was written on the base type (TibSun `Point3D dim`). The nested return ctor is refused once about 6-7 root-level inline sites follow the site in a budget-1000 body. Empty inline calls and argument temporaries count; an operator+ with two temporaries counts 3; nested sites, sites textually before, and plain IL do not (plain IL raises the budget).

- **Declaration-count ties are inversions of IL creation order, with no symbol-level anchor** (2026-11-11, Scout1287 lab: K = 0..63 probe declarations over Print_CRCs, AI_Build_Defense and Reposition_Sidebar; 17 sweeps). Local names, local declaration order and the probe's kind are inert. The only lever is the IL creation order of the two tied items: swapping an operand pair gives the exact complement table, and moving the definitions shifts the window. No natural form is robust at every K: a header change moves the bad window and never removes it. One-object spellings (`R.X + R.Width`) are hash-consed across sites, so the first spelling decides every site. Template headers act as a pure +7 shift for Reposition_Sidebar, not for Print_CRCs.

- **Inlined helper parameters: by-value ints get their own home; `int &` is a pure alias** (2026-11-11, Tactical::Render 0x6A45A0, Scout1291 lab). A by-value `int` parameter of an inlined helper gets its own frame home and web, so a value threaded through two inlined helpers (`line = A(line); line = B(line);`) shows as two homes with a copy between them, where a single local instead splits at loop boundaries with stores at its definitions. An inlined `int &` parameter adds nothing. A second index read of a vector element inside one iteration is never a spelling under /O2 (fifteen forms all CSE): when the original shows one with base and index in registers, read it as the element web spilled and rematerialised.

- **The count unit for local-count windows is defined locals, not declarations** (2026-11-11, Exit_Object 0x440520, Scout1296 lab). Constructed objects (`Cell qN;`), constant-initialised scalars (`int qN = 0;`) and temporaries count. Uninitialised scalars, inline copy locals, unused labels, typedefs, empty inline calls and TU-level declarations do not. Exit_Object's windows are insensitive to header declaration counts, unlike Reposition_Sidebar's.

- **Under /GR the class hierarchy descriptor's ambiguous bit follows the compile path** (2026-11-13, lane D; docs/ORIGINAL_TARGET_BUILD.md "Fixed compile folder"). VC6 SP3 (C2.DLL 8447) writes bit 4 of a `??_R3` attributes word (1 against 5 for the same class, same source, same arguments) from memory that changes with the length of the compile folder and of the `/I` directory; TMP does not change the bit but changes the order of sections in some objects. Measured on tagtype under `build/laneD/<name>`: path lengths 94, 97-107, 109 give 1 and 95, 96, 108 give 5; terrtype flips at 97-98; campaign (5, as in the original), house and version did not flip over 94-109. Under the fixed layout (folder spelling 34 characters, include 32) every emitted descriptor of tagtype, campaign, terrtype, house and version equals the original's; that layout is not unique (31/33 and 32/33 also match; 33/32, 35/32 and 36/32 do not) and was picked by outcome over these five units, chosen once for every population, so a later /GR unit can still differ (it then keeps its RTTI from original bytes, listed with the field). Without /GR the path changes at most the order of sections in some objects (83 of 1,617), never their bytes. The object cache key names the layout. Widened sample (47 units, 51 multiple-inheritance classes): every candidate layout gets only 76-80% of them right, a different set each, and AbstractClass flips between units under one layout, so the bit follows per-compile memory, not the environment (its handling: DECISIONS "Class hierarchy descriptor ambiguous bit"). TMP length drives the section order of some objects.
- **The four fragile rows hold together at exactly one declaration count, and pure renames never move it** (2026-11-15, lane E1b fragile-row lab: `tools/original_target/fragile_lab.py` renames identifiers in a scratch tree and compiles only the four rows' populations in about 5 s; `build/lab/probe.sh` adds K `struct LabProbeN;` lines at the top of `shared/cd_timer.hpp`, which all four units include). Print_CRCs 0x62A360, WinMain 0x68E280, OwnerDraw 0x5E5150 and Reposition_Sidebar 0x6819A0 are exact together only at K = 0 among K = 0..63. Exact counts per row: OwnerDraw 33 (K = 0 and 31..63, one window), Print_CRCs 23, WinMain 18 and Reposition_Sidebar 16 (scattered, several tie sites each). So every net change in the declaration count before these functions breaks at least one of them. Name length and spelling do nothing: 44 of lane M1's 46 held names, renamed one at a time and then all together, keep all four rows exact. The two that flip rows change the count. Load_Alloc_Data (Unknown_Load_Shape_00495270) becomes a second overload of 0x00495230's name and flips OwnerDraw and Reposition_Sidebar. Technos (unknown_list_a40c20) merges an alias into an existing declaration and flips Reposition_Sidebar. Combined, they also flip Print_CRCs. Consequences:
  - A rename batch is safe for these rows when every new name is distinct and undeclared, or is the same entity's declaration with the same type. Overloads and merges need the lab or the gate.
  - No "natural form" makes these rows count-robust (as Scout1287 found): only K = 0 holds all four. A count-changing name lands only with an evidenced re-roll of every row it flips (DECISIONS row 23).

## Inline constructors and template members the original calls out of line (found in the rebuild, lanes R8-R42)

- Count the image's calls before moving a definition. `rebuild/tools/expansions.py HEADER OLD NEW SYMBOL ADDRESS`
  declares one inline function without a body for one run, compiles every unit and sets each exact row's source sites
  against the image's calls to the copy (lane R42). TPoint3D<int>(x, y, z) 0x41B630: 60 image calls in exact rows
  against 859 source sites, 799 expanded; Cell(int, int) 0x42AD30: 451 against 3,169, 2,718 expanded; TPoint2D
  ::operator== likewise. The in-class definitions are right: the out-of-line sites are per-caller budget outcomes, and
  a header change that stops the expansion at one parked row breaks hundreds of exact ones.
- An out-of-class template member reached through another inline expansion (DynamicVectorClass's constructor inside
  an inlined WaveClass(NoInitClass), Add, the copy constructor, GenericNode's constructor) is the usual parked
  difference: the original calls it, the rebuild expands it (WaveClass::Load 0x7221D0, Search_For_Tiberium_Weighted
  0x4CBB40, TeamTypeClass::Suggested_New_Team 0x6BED50, RulesClass::Initialize 0x6433A0; lanes R10-R26). It is the
  caller's budget, not the header form: Load with only the placement new expands every level, and two or three more
  Swizzle_Pointer calls ahead of the loops turn the constructor into a call as in the original (lane R25, scratch only).
  So the original's caller was wordier before that site; look for its missing statements or calls in the reference
  texts, never for padding.
- Found in the rebuild (lane F21, high): the missing sites of such a row can be a member grouping in the class, not a
  statement of the caller. WaveClass::Load 0x7221D0 (305 bytes off through six lanes) is exact with the twin's
  wave.h: ra2-decomp declares the six active wave points as one nested block (`struct ActiveWaveStruct { Point2D
  StartMiddle; .. } ActiveWave;`) where the archive has six Point2D members. Six empty Point2D constructors are free
  sites; the block's compiler-made default constructor is one site of its own ahead of AffectedCells in the expanded
  WaveClass(NoInitClass), and DynamicVectorClass<CellClass *>'s constructor 0x7246D0 is then called as in the image.
  When comparing a twin's header, compare the member declarations structurally (nesting, grouping), not only for
  added or missing members: lane R42 had recorded this header as "differs only by YR members".
- Found in the rebuild (lane F21, high): a never-referenced empty function in a unit is a lead for its parked rows. RA2
  emptied SuperClass::Deactivate_Firestorm(int, bool) but kept the definition (0x69E980, `ret 8`, at the archive's
  place between Place and Is_Charging) and its two calls in Discharged 0x69D090, which /Ob2 expands to nothing: no
  call in the image, but two sites' worth of root size. A draft that drops such calls "because the image does not make
  them" comes out a few call statements short of the budget and loses a late nested expansion (163 bytes there; exact
  with the archive's calls restored). Check the stack-argument count of an unreferenced `ret N` stub against the
  archive function defined at the same place in the unit before naming it a hook.
- Found in the rebuild (lane F21, high): a bool function returning `a || b` builds the result as 0/1 in eax (`xor eax,
  eax` / `mov eax, 1`), the same body an int function with `return(1)`/`return(0)` gives; callers then test `al`.
  IonStormClass::Is_Ion_Storm_Pending 0x51FC30: the int body with a second bool prototype for callers was never needed.
- Found in the rebuild (lane F21, high): `x == false` costs a root one IL unit more than `!x` with the same code, and
  one unit can decide the last site: TechnoClass(NoInitClass) 0x6C1F10 expands Technos.Add as the image does with
  `if (found == false)` and calls it with the references' `if (!found)` (93 bytes). Such a form is a hand-made
  variation when no reference has it (variation_candidates.csv), but it sizes the deficit: when one out-of-line call
  statement of padding flips a site, look for a one-token difference before a missing statement.
- Found in the rebuild (lane F21, high): an explicit specialization of a template member is an ordinary function for
  the inliner, so it falls under the auto-inline size cap that template members escape. map.cpp's `template<> bool
  VectorClass<PassabilityType>::Resize` (written so 0x56BCD0 has a body while its only emitter, Zone_Reset, is parked)
  makes the instantiated DynamicVectorClass<PassabilityType>::Resize 0x56BE10 a 64-byte call where the image expands
  the base Resize (176 B); without the specialization both come out exact from vector.h once Zone_Reset's guard is
  open. It is the techtype 0x6DF8C0 case in "Later inline sites of a long reader, and specializations left behind": the
  specialization leaves when the emitter opens.
- Tell the budget from depth or helper shape with a pad probe (`rebuild/tools/inline_probe.py --pad`): when a few
  expansions of padding at the top of the caller flip the site (Tactical::Draw_Fence_Placement 0x6A5D70: one
  Adjacent_Cell), it is the budget; when 0..12 never flip it (HoverLocomotionClass::Process 0x4FBFC0, reinf
  Do_Reinforcements 0x639970), the original reaches the callee through a different helper level or form (lanes R15, R17).
- In a chain of `||` tests that each build an offset, the last operand before the Center_Coord call is the one
  expanded: a sixth test in AircraftClass::Fire_At 0x415790 moves the expansion to it (lane R12).
- Found in the rebuild (lane F22, high): two texts with the same final instructions can share frame slots
  differently: a store repeated in both arms of an `if`/`else` is merged into one again, yet the layout changes
  (not isolated in a lab; the likely cause is the variable's reference count at layout time). WOL_Wait_Callback
  0x73E100 was 23 bytes of slot numbers only (601 = 601 instructions, every call agreeing) with its two lookup blocks
  written alike. ra2-decomp's twin writes the second block's link `if (last_user == NULL) { list = new_user; last_user
  = new_user; } else { last_user->next = new_user; last_user = new_user; }` and the first with the archive's single
  trailing assignment; the code is the same 601 instructions either way, and with the twin's form the image's
  sharing follows (both Wstring names in one slot, the ping handle with the first list).
  When a slot-only miss has two sibling blocks, read the twin for a statement repeated in both arms of one of them.
- Found in the rebuild (lane F22, high): which of two `memory + register` sums of a compare is loaded first in an
  expanded helper is a tie the caller's locals decide, not the helper's text. Union's `result.X+result.Width <
  rect2.X+rect2.Width` (trect.h, the same text in TibSun, ra2-decomp, mischa85 and Red Alert) loads rect2's sum first
  out of line (0x45EFE0) and wherever one sum is register plus register; where both are memory plus register the
  order moved with the caller's tail in BuildingTypeClass::Get_Draw_Rect 0x45E290 (ra2-decomp's named `Rect
  drawrect` filled member by member: the image; two int locals passed to a Rect temporary: the other order, 5 bytes;
  the archive's `Rect(x, y, rect.Width, rect.Height)`: 17). File-scope declaration counts (lane F9) and named ints
  added inside one arm (Damage_Low_Bridge_EW 0x55DEF0 probes, still 5 bytes) do not move it. A YR twin that is matched
  with the same text can show the other order at the same site (YR 0x57BCF0), so check the twin's image before
  taking its text as evidence for the order.
- Found in the rebuild (lane F22, high): two expansions of one member in one function can order the same subtraction
  chain differently, and no spelling of the member reaches the odd one. SidebarClass::Activate 0x67E210 expands the
  row count in both arms; the if arm, where a second chain shares Height and StripTop in registers, subtracts
  StripTop before the side's extra as Get_Row_Count 0x6820A0 does out of line; the else arm's lone chain subtracts
  the extra first. Seventeen member and open-coded spellings of the twin's shape and five spellings of the two row
  members leave our else arm in the out-of-line order (58 bytes, three instructions). Find which arm a chain
  difference is in from the calls around it before reading the dossier's hunk as the first site in source order.
- Found in the rebuild (lane F22, high): a helper whose out-of-line body is exact two ways is decided by its callers'
  sites. MapClass::Spring_Low_Bridge2_Destroyed_EW 0x55FB90 is the image with `while (Is_Low_Bridge2(tmp))` and with
  the range test written in place (`overlay = Map[tmp].Overlay; } while (overlay >= 0xCD && overlay <= 0xE8)`,
  ra2-decomp's shape for the first kind): the image expands the predicate inside it either way. But the predicate
  form is small enough to be expanded into Damage_Low_Bridge2_EW 0x55F020 (the predicate then left as a call, 34
  calls for 27, 1,214 bytes off), while the in-place form stays a call there as in the image (5 bytes left). When a
  draft shows a callee's inner calls where the image has one call to the callee, respell the callee's inner helper
  uses in place before spending the caller's budget.

## Literals, function statics and data order (found in the rebuild, lanes R10, R22, R25)

- VC6 folds `1.0 / UINT_MAX` to exactly 2^-32, not to the 1/4294967295 the image pools; write `1.0 / 4294967295.0`
  as main does (mapgen's RMG scale, lane R25; main's literal form for the same constant in Scout863). The row compare
  binds literals by address, so the wrong fold shows only in the data bytes the build compares.
- A unit's initialised constants, file-scope and function-local alike, are emitted in text order, one run per
  declaration group. A gap after a data symbol is first a declaration-order question: mapgen's 4 bytes after Init_Map's
  float `_waterfall_chance` closed when the variable moved to file scope just before Init_Map as ra2-decomp declares it
  (mapgen.cpp:3254), and Get_Angle_Score's weights declared in the image's order (random 2.5, length, angle) laid out as
  the image holds them (lane R25). The order of runs between functions can still differ from the image's (Init_Map's
  run before MapRegionClass's there), which a per-symbol compare does not depend on.
- One object holds one select-any copy per distinct literal. A second or third byte-identical constant inside a unit's
  function-statics block, read by one function and placed in the order of the functions that read the statics, is a
  named `static const` of that function (mapgen 0.7 at 0x7A5CB0 read by Seed_River, 0x7A5D70 read by
  Find_Dirt_Road_Tile; 15.0 at 0x7A5CE8, Create_Tiberium's `_distance_scale`; lane R22; DECISIONS "A byte-identical
  floating constant"). The plain literal binds to the pool copy, which may be one kept from an earlier object (15.0 at
  0x7A5398, also read at 0x530C24): a literal that lands on another unit's pool address is link-order evidence, and one
  that cannot (fly Process_Landing's 0.02 binds to 0x7A7F30, the original reads 0x7A0FF8) stays a named static until
  a binding is evidenced (lane R10).

## Select-any COMDAT copies and header-emitted data (found in the rebuild, lanes R7, R17, R19, R35, R43)

- The linker keeps one copy of each select-any COMDAT image-wide, from the first object in link order that defines it,
  and keeps it even under a link that kept unreferenced code. Where the kept copy sits is link-order evidence: palette's
  VectorClass<HSVClass> constructor 0x605DA0 and DynamicVectorClass<HSVClass> 0x6051F0 are palette's own object tail
  (lane R7); radsite's object, not ramfile's, emits the twelve RadSites vector members 0x637F90-0x638310 right after its
  last function (lane R17). A copy our object emits that the image holds elsewhere is the discarded duplicate: compare
  it with the kept one (DECISIONS, select-any second half).
- Template copies that only a parked caller instantiates disappear with its guard and come back with it: 72 of the
  156 rows exact with the guards open (7.6 KB) are held that way (lane R43, tail_holders_r43.csv); Dropship_Screen's
  effect-vector copies and Combat_Damage's two Smudge copies (lanes R19, R31) are examples.
- `__uuidof(I)` is its own select-any GUID COMDAT, separate from the `IID_I` .rdata copy: building's IPiggyback rows
  refer to 0x7CFC30, while IID_IPiggyback is 0x7A2050 (lane R19).
- System headers can emit zero-initialised select-any .bss objects no code references: wonline's ATL 3.0 headers
  emit CComApartment::ATL_CREATE_OBJECT (4 B) and CComModule::m_libid (16 B), which fill the hole 0xB2ACAC-0xB2ACBF
  exactly. Separate COMDATs are ordered by the linker, so the order inside the hole is our object's, a hypothesis; they
  are bound as a group (lane R35; DECISIONS "Unreferenced zero-initialised select-any COMDAT .bss objects").

## Adopted Tiberian Sun text and routines RA2 lacks (found in the rebuild, lane R45)

- Found in the rebuild (lane R45): routines Tiberian Sun has and RA2 lacks show up as unresolved calls in parked
  rows. Before writing a call against an adopted declaration, check the image's call targets at that site: drive and
  ship Start_Of_Move call RadioClass::HasAnyLink 0x636F10 where TibSun's text calls In_Radio_Contact, and
  VocClass::PlayGlobal 0x713DB0 where it calls Sound_Effect(VocType). `rebuild/tools/declared_never_defined.py`
  lists the declarations the tree carries with no RA2 body (rebuild/manifest/declared_never_defined.csv); a
  declaration leaves its header when its last user calls the image's routine instead (DECISIONS, "Members declared
  in adopted Tiberian Sun headers that RA2 never defines").

## Measuring a nested inline budget in the unit itself (found in the rebuild (Fable trial), lane FT1)

- Found in the rebuild (Fable trial): a callee's IL size can be read inside its own unit, with the unit's headers and
  spellings, by `rebuild/tools/il_lab.py`: a small root `{ Spend(); f; }` has the floor budget 1000, the inline
  `Spend` (constant stores: 12 empty, 6 per array element, 4 per scalar) is charged first, and f expands iff its
  size fits what is left. ownrdraw, 2026-11-20, to 2 units: Dictionary keyHash 48, a node's scalar deleting
  destructor 50, ~WinData 50, ??_GWideWstring 50, getPointer 132, WinData::operator= 132, Set_Wide_Text 116,
  NodeList::Remove_Node 140, shrink 248, remove(key, value) 378, remove(key) 62, Restore_Child 106. Sweeping the
  spend past f's own size lists the thresholds of f's nested sites in order, in f's own budget units.
- A compiler-generated scalar deleting destructor (`delete node` of a class with a non-trivial member) is an
  ordinary charged site (50 here, over the free limit of 40), with the implicit destructor and the member's
  destructor decided inside it: the image's `call ~WinData; push node; call operator delete` at one delete and a
  whole expanded destructor at another are the same source statement under two budgets.
- An accepted nested site charges its parent's remaining budget, so the states of one body are not ordered by "more
  budget, more expansion". Dictionary<K, WinData>::remove(key, value), as its body's budget grows: (A) the second
  delete calls ??_G; (B) from 744 it expands ??_G and calls ~WinData; (C) from 754 the first shrink's keyHash is
  accepted and its 48 units send the second delete back to a ??_G call; (D) from 792 both are expanded. The image's
  OwnerDraw::Combo_Drop_Proc 0x5EAD20 is state B, a 10-unit window; the rebuild's text sat one unit under it.
  The sums of the measured sizes miss these thresholds by 2 to 34 units once nested bodies are accepted: measure the
  thresholds, do not add sizes.
- Padding the top of a long root is not a ruler for a late site. Earlier nested sites of the root flip as its budget
  grows, so the budget reaching a late site is a sawtooth: in Combo_Drop_Proc 24 `GetFocus();` statements left the
  late remove in state A and 25 put it in D, while 16 to 18 global stores put it in C; state B was never reached that
  way. To measure the gap at a late nested site, shrink the nearest enclosing callee in a scratch copy instead
  (delete trailing statements that are not inline sites): only that site's budget moves, by the removed size over the
  product of the enclosing N. Removing Restore_Child's last call statement (6 units) reached state B, and removing
  all four trailing statements (about 44 units) stayed in it: with a divisor of about four between Restore_Child and
  the innermost body (inferred from these two points, not measured), the deficit is one or two units of that body.
- When a late site is within a unit or two of its window, look for spelling differences that cost single units in
  the callees above it before reshaping the root: an explicit cast is one unit, and a reference cast at every call
  site of one table (`table.remove((unsigned int const &)window)`) or inside a template member (`hashFunc((K &)key)`)
  repeats that unit in every caller and callee of the family.
- Found in the rebuild (Fable trial): a constructor with a long initializer list is a root whose member
  constructor sites share one budget of 2*S, each decided with what is left divided by the sites still to come.
  RulesClass::RulesClass 0x640B80 (533 entries, 83 vector members): the image expands TypeList's and
  DynamicVectorClass's constructors at every member, calls VectorClass's at 82 of them and expands it only at the
  last vector member, where few sites remain. The chain here is TypeList<int>() 58, DynamicVectorClass<int> 78,
  VectorClass<int>(int, T const *) 138 units; the rebuild's root is 7 to 12 units short at that last member (two
  probe stores in the body flip it, one does not, sixteen expand the member before it too).
- What does not move such a budget: the spelling of constant initializers. TibSun's and ra2-decomp's forms
  (`1.0/60.0`, `M_PI/2`, enumerators for `(CrateType)2`, `0` for `(char)0`) and integer literals for 66 double
  members (`TunnelSpeed(1)`) left the call list unchanged: constants are folded before the size is taken.
- What does: an implicit integral conversion of a parameter handed on to a base constructor is counted.
  `DynamicVectorClass(unsigned size, ...) : VectorClass<T>(size, array)` over `VectorClass(int size, ...)` is a unit or
  two larger than the same text with an `int` parameter, and over 83 chains that alone closes the RulesClass
  constructor (probe: exact, rules 152/156). It is not the answer: the signed parameter loses the same rows the
  unsigned VectorClass parameter loses (house 2, palette 1, smartdeform Commit_Deform_Grid, vein
  Build_Growth_Queue), and RA95, TibSun, ra2-decomp and mischa85 all declare it unsigned. Those rows fix the
  chain's size as it stands, so the constructor's missing 7 to 12 units are in its own root, not in the chain.
- Found in the rebuild (Fable trial): an inline wrapper spelled out by hand costs its site. The nested budget of a
  site is what is left divided by the inline sites from it to the end of the body, so a free inline call later in
  the body lowers the share of every nested site before it without emitting anything. Westwood's
  `Random_Pick(a, b)` (ccrand.h: `return(T(Scen->RandomNumber((int)a, (int)b)));`) is such a site; a draft that
  calls `Scen->RandomNumber(a, b)` directly compiles the same call and has none. MultiplayerGameMode::
  GenerateStartingUnits 0x5B8060: the image calls the infantry vector's Clear out of line at the early return and
  expands the unit vector's destructor right after it (the later site has fewer sites after it, so a larger share);
  three extra inline sites after the return (probe) reproduce it, and the two picks written through Random_Pick are
  exact. MapClass::Collapse_Cliff 0x562F50, hard on main (Ob2Apply519 reshaped a callee for it): its six draws
  through Random_Pick are exact with nothing else changed. When the image calls a small inline helper early in a
  body that our text expands, count the later statements that Westwood would have written through an inline
  wrapper (ccrand.h's Random_Pick, Sim_Random_Pick, Percent_Chance, Probability_Of) before reshaping anything.
- Found in the rebuild (Fable trial): the same holds for a unit's own out-of-line function whose body a draft
  spells out by hand. RulesClass::Initialize 0x6433A0 read [Maximums] inline with a static of its own while the unit
  defines Heap_Maximums; written as the call (which /Ob2 expands, as ra2-decomp's twin has it) the root is smaller
  and the later CCINIClass constructor calls GenericNode's constructor out of line, as the image does.
- A cheap direction test for a row whose image calls a helper that our text expands: add K empty inline calls
  (`static inline void Site(void) {}`) before the function's last statement in a scratch copy. If the image's call
  list appears at a small K, the original has that many more inline sites after the helper (or as much less budget):
  look for wrappers and for the unit's own functions written out by hand in the rest of the body. If no K helps,
  the cause is not a later site (a deeper wrapper level, the 35000 cap, or the text).

## Explicit zero initializers of class members in a constructor's budget (found in the rebuild (lane F2))

- Found in the rebuild (lane F2): a member whose default constructor zeroes it and the same member written out with
  zero arguments in the initializer list compile to the same stores, but they are different inline sites of the
  root. `RGBClass(void) : Red(0), Green(0), Blue(0) {}` at a member the list omits is an implicit site with no
  arguments; `RadColor(0, 0, 0)` is a call of the three-argument constructor whose three converted arguments are
  counted in the root's size S (budget 2*S). RulesClass::RulesClass 0x640B80 was 7 to 12 units short of expanding
  VectorClass<int>'s constructor at its last vector member (FillEarliestTeamProbability); ra2-decomp's twin lists
  `RadColor(0, 0, 0)`, `LineTrailColorOverride(0, 0, 0)` and `ChronoBeamColor(0, 0, 0)` where main's and mischa85's
  texts leave the three colours to the default constructor. With those three initializers the constructor is exact
  (7,768 B) and the two VectorClass constructor copies behind it (0x654960, 0x6549B0) bind; nothing else changed.
- Reading it from the image: zero stores to a class member in the middle of the list, in member order, where the
  member's type has both a zeroing default constructor and a value constructor, do not say which was written. When
  a long constructor is a few units short at a late site, compare the reference twins' initializer lists for
  members our list omits before measuring anything else; main matched this row only because its vector chain was
  a unit smaller per member (both size parameters unsigned), which hid the three missing initializers.

## Twin statements, inline predicates and charged probes (found in the rebuild (lane F2))

- Found in the rebuild (lane F2): before measuring a row that is a few budget units off, read the matched or
  near twin's text of the same function for calls our draft spells out by hand. Each of these was the whole cause:
  an inline predicate (`Road_Tile_Set(tile)` for `tile != ISOTILE_NONE`, seven sites in
  MapGeneratorClass::Generate_Rural_Roads 0x58A1B0, the K=2 trailing-site lead); inline.h's
  `Distance(coord1, coord2)` for `(a - b).Length()` (Explosion_Damage 0x47E2C0: the image's three calls of
  TPoint3D<int>'s constructor are operator-'s result inside the charged wrapper); `groups.Delete_All()` for
  `groups.Clear()` (the image's virtual Clear followed by Resize(len) is Delete_All's body); and a constructor's
  base-initializer form (`Coord(Point2D const & pt, LEPTON z) : BASECLASS(pt.X, pt.Y, z) {}` as ra2-decomp has it,
  against the TibSun archive's member assignments: the same stores when expanded, a call of TPoint3D<int>'s
  constructor where the budget is short, Tactical::Draw_Shroud 0x6A7820). That header form was rechecked over all 280
  includers of coord.h: no exact row lost, and Tactical::Draw_Tile_Shadows 0x6A8240, whose text already equalled its
  matched twin, became exact with it.
- A charged probe measures a deficit that out-of-line pads cannot. An inline function of constant stores
  (12 + 6 per array store, so 42 at the least: 40 and under is free) called at the top of the root charges that
  many units before every later site, whatever the root's budget; `GetFocus();` pads only raise 2*S, which is
  nothing while the root sits on the 1000 floor. Sweep the spend upward until the image's call list appears, then
  add pads to the smallest working spend: when a pad of 8 budget units takes the list away again the root is above
  the floor, and spend minus pads brackets the deficit (HouseTypeClass::Read_INI 0x4F9550: a spend of 42 gives the
  image's list, two pads lose it again, so the original has 27 to 34 fewer budget units at the second list reader).
  Trailing-site probes (K empty inline calls) must stand after the site in question; sites before it do not count
  in its N.
- il_lab.py read at a finer step lists a reader's states in order. TGet_TypeList<UnitTypeClass> in houstype (size
  216): the list's Add is called and both TypeList copies expanded from 756 to 811, Add expanded and the copies
  called from 812 to 959. The image's second reader is in the first state and ours in the second, one step apart:
  a deficit of a few units at the reader's own level, which is 27 to 34 at the root because the reader's share is
  what is left divided by the ten or so sites after it. TechnoTypeClass::Read_INI 0x6DAF60 shows the same family
  (five of six Get_VocType_List statements call VectorClass<int>::Clear in the image; a top spend of 192 fixes
  the first, 372 three of them).
- A callee of 41 to 46 units is not free. TPoint2D<int>::operator== measures 46 in infantry (il_lab), DirType::
  Direction 98 with the comparison nested at 144, Move_Coord 90, Coord(int, int, int) 42 to 44 and TPoint3D<int>'s
  constructor inside it 46 to 48. DirType::Direction's `if (coord1 == coord2) Facing = 0;` leaves a call whose
  result is dropped when the comparison is refused: the image has it in FlyLocomotionClass::Nearing_Target,
  HoverLocomotionClass::Process and Motion_AI and twice in InfantryClass::Start_Driver. In Start_Driver the image
  refuses the comparison at both statements while expanding everything inside Move_Coord beside it; no budget at
  the root gives that (30 trailing sites refuse the comparison and Coord's constructor together), so the
  original's Direction sits one inline level deeper there, as hover's Motion_AI already has it through a
  unit-local `Facing_To` (ra2-decomp's name). A plain `Facing_To(Center_Coord(), headto)` in infantry overshoots
  (DirType::Direction itself is called), so the wrapper's own shape is still open.
- Found in the rebuild (lane F2): the order in which an expanded TPoint3D<int>::Length converts its three members
  (fild X, Y, Z or Y, Z, X, with the products taken in the reverse order) is not fixed by Length's text. In
  Explosion_Damage three expansions of the same `(a - b).Length()` give Y, Z, X at the two cell sites and X, Y, Z
  at the third; the image has X, Y, Z at all three. Not the wrapper's spelling (Distance or Distance_To at any of
  the three), not the declaration count (12 extra function types), not the root budget (pads). A fourth Distance
  site later in the function (probe) turns the first inline sites to X, Y, Z, and removing the third leaves the
  first two as they were: the order follows the other expansions of Length in the function. Rows that carry a
  second Length with named locals (`Length_YZX`, the rocket's two Ground_Distance twins) are probably this effect.
- An adopted twin can bring a unit-local helper that the structure budget counts. AnimClass::Flaming_Guy_AI
  0x423120 is exact with ra2-decomp's `_Move_Coord` (y before x, a named result) where inline.h's Move_Coord gives a
  constructor temporary and x first; the image has the first shape there and the second in other units, and
  ra2-decomp keeps both bodies too. One name with two bodies is a ruling, not a draft: the row stays parked with
  the exact candidate recorded.
- The trailing-site count names the wrappers. InfantryClass::Assign_Destination 0x501CE0 answered the probe at K=3,
  and ra2-decomp's twin has exactly three inline calls our draft spelled out: `As_Unit(target)` and
  `As_Building(target)` (their cast.h: a null test and an RTTI test around the cast) and
  `Begin_Piggyback_Locomotion(Locomotion, piggy, walk)` (their piggyback.h). With the two headers adopted the row
  is exact. The same checked casts were already in the tree as unit-local helpers (team `_As_Cell`, unit
  `Unit_As_Cell`/`Unit_As_Building`/`Unit_As_Unit`, building `Techno_As_Foot`, written as conditional
  expressions); the shared if/return bodies replace all five with no row lost, so one shared definition serves
  both spellings' rows.
- ra2-decomp's unit sources carry their inline wrappers beside the functions that use them (39 `static inline`
  helpers in their wonline.cpp alone). Three wonline rows fell to them in minutes: Update_Buddy_Profile 0x7562B0
  (seventeen `Set_Text(window, text)` calls for hand-written SendMessage(0x4B2) lines), Poke_The_Host 0x750F60
  (`Current_Channel_Type()`, the guard's "one more inline call site"), and with them the out-of-line
  Dictionary<Wstring, Ladder>::getPointer under its own name. For a parked row with a twin, list the twin unit's
  `static inline` definitions above the function first (Connect_To_Server's twin has six: Strip_Server_Prefix,
  Pick_Locale, Set_Locale_Entry, Apply_Locale, Check_Locale, Wait_For_Event).
- A guard whose difference is only immediates of zero (`push 0` for `push 0x7FE6CC`) is unbound references, not
  text: open it and run `rebuild.py check UNIT --learn` (Pick_Lobby 0x744090, 19 bytes, five references).
- A function-local static of a newly exact row can strand the unit's data section: CellClass::Wall_Update's
  `_offsets` (0x7D2B58) is followed in the image by the statics of Closest_Free_Spot, which is parked, so the
  object lays Clear_Icon's tables 36 bytes early, pads the next one by 4, and the per-symbol data compare refuses
  the section. Such a row lands with the parked neighbour whose statics lie between, not alone.

## Later inline sites of a long reader, and specializations left behind (found in the rebuild (lane FH1))

- Found in the rebuild (lane FH1): in a long root an early nested site sits on a knife edge that every later inline
  site of the root moves. TechnoTypeClass::Read_INI 0x6DAF60 (12,659 B): the image calls VectorClass<int>::Clear
  for the temporaries of five of the six Voice list statements and the DynamicVectorClass<int> copy at +0x1A06,
  where the draft expanded them. The destructor's nested share is what is left divided by every inline site after
  it in the root (hundreds here), against a Clear of about 76 units (il_lab, read in houstype), so one site more
  or less later in the function flips a statement. A charged probe at the top needs 192 units for the first
  statement and 372 for three (lane F2); the same effect is ten inline sites the draft spelled
  by hand, all in ra2-decomp's twin text at the same places: the unit's own `_Scale_To_256` for the Speed= clamp
  (one site: the first Clear and the copy), the eight recoil statements as `setter(ini.Get_Int(..., getter()))`
  through TurretControl's accessors where main's coined Get_Recoil helpers had one site each (eight more sites:
  three Clears), and the unit's own out-of-line `Has_Turret_Weapons()` at the art section's turret test (the last).
  Count sites before measuring: list the twin's callee vocabulary against the draft's
  (`grep -o '[A-Za-z_:]*(' | sort | uniq -c`), and read every callee the twin has and the draft lacks.
- An accessor that Yuri's Revenge keeps out of line at the end of a unit, among the template copies, is the class's
  own inline member in RA2 (TurretControl's eight at 0x717A30-0x717AD0, the VoxelLibrary load test at 0x717AE0):
  the larger YR root ran out of budget and called them, RA2's expands them all and emits none.
- The scheduling of a member store against the next statement's loads says how the value arrived. With ccini.h's
  `Get_VocType(section, entry, (VocType)member)` each sound member was stored before the next default was loaded;
  the image loads the next default first and stores just before the call. ra2-decomp's unit-local
  `Read_Sound(ini, section, key, int fallback)` (int in, int out, no enum conversion at the store) gives the
  image's order at all twenty statements. The opposite case is HouseTypeClass::Read_INI 0x4F9550: the image
  stores Prefix before loading FirepowerBias, and `Prefix = prefix[0];` lets the store slide down; only a store
  through a reference keeps it (ra2-decomp's `char & p = Prefix; p = prefix[0];`; memcpy, a pointer cast and
  `*prefix` do not), which says the original assigned through something that took Prefix by reference.
- An explicit specialization written to emit one out-of-line template row while its only natural emitter was
  parked must leave when the emitter opens. techtype carried `template<> VectorClass<ParticleSystemTypeClass const *>
  ::operator=` so that 0x6DF8C0 had a body; a specialization defined after its first use is always called, so the
  TypeList copy constructor 0x6DF9E0 that Read_INI emits compiled with a call where the image expands the
  assignment (135 of 136 bytes off). Without the specialization the template's own member expands there and
  Read_INI's two out-of-line calls emit 0x6DF8C0. When a newly opened row's template copy differs by an inner call,
  search the unit for `template<>` first.
- Where an inline member's body lives can move a count-sensitive row elsewhere. TurretControl's eight accessors
  defined in the class flipped sidebar SelectClass::Action 0x6809F0 (4 bytes; the known count-sensitive row);
  declared in the class and defined `inline` in techtype.cpp, their only user, as ra2-decomp lays them out, 98
  includers keep every row and planmgr's parked count-sensitive Planning_Update 0x614010 is exact again.
- A coined stand-in class costs what its members cost, not what the original's did. HouseTypeClass::Read_INI
  0x4F9550 sat 35 to 42 budget units over the image at its unit list reader through several lanes (a charged 42 at the top
  was exact but for the probe; or one trailing site with 41 to 160 more root budget). ParentCountry was
  ra2-decomp's CountryName, a struct written to reproduce the stores; the tree's own TStringID<24> (IniName's type:
  same 25 bytes, same constructor code) has an operator= that returns by value, whose copy the compiler drops from
  the code but counts in the size, and with it the call list is the image's and the unit is whole (90 rows).
  When a root is a few tens of units off and holds an assignment or construction of a small helper class, ask
  whether the tree already has the real class for that member before measuring: il_lab there read
  CountryName(char const *) about 72 and its operator= 56, TypeList operator= 54 (DynamicVectorClass's 64,
  VectorClass's 188), copy constructor 62 (DVC copy 88), ~TypeList 50 (Clear 76), Add 142, TypeList() 58,
  Delete_Index 106. Name() is nearer the image than IniName in that root (IniName needs a spend of 150 to 204).
- A count-sensitive row can sit on a single count: with houstype.h one struct lighter, sidebar SelectClass::Action
  is 4 bytes off at +0 and exact at every probe from +1 to +16 declarations, while queue's parked 0x62A360 came
  back at the same change. Probe both directions and run countsens before deciding what a header change costs.

## Hand-written blocks in a reader's root, accessor conditionals and named-local ties (found in the rebuild (lane F2, high))

- Found in the rebuild (lane F2, high): a reader root that is a few units short at a late site can be missing root
  text, not carrying extra sites. RulesClass::Combat_Damage 0x646C60 called TGet_Class for its last particle system
  where the image expands it (9 to 16 budget units short); main had closed the gap with eighteen named int locals
  (`int x = ini.Get_Int(...); Member = x;`), and the rebuild then needed non-const type pointers on top. ra2-decomp's
  twin writes the two elite warheads out by hand (`{ char name[128]; WarheadTypeClass const * fallback = V3Warhead;
  V3EliteWarhead = ini.Get_String(...) ? WarheadTypeClass::Find_Or_Make(name) : fallback; }`): a block is the root's
  own text (it raises S, so 2*S) where a TGet_Class call is a charged site, and the expanded reader and the block
  compile to the same instructions, so the image cannot show which was written. Measured in the unit: direct
  assignments with both readers 549 lines off, one block 240, both blocks exact, both blocks with the named locals
  105 off the other way (an overshoot: the smudge and anim list readers expand too far). Try a twin's hand-written
  block against the clean direct text, not on top of a draft that was already padded. The `fallback` local does
  work: with the member named in the conditional the default is read after the call (10 lines).
- This is the reverse of the wrapper lesson (lane FH1): there the draft spelled out by hand what the original called
  through an inline; here the draft called an inline where the original spelled it out. Both show only in the
  budget. List the twin's blocks and its callee vocabulary before measuring either way.
- A conditional over two parameterless accessors is not the same tree as one over a flagged accessor.
  `x = bridge ? cell.Cell_Bridge_Occupier() : cell.Cell_Occupier();` leaves the value in eax on both arms and copies
  it at the join (`mov esi, eax`); `bridge ? cell.Cell_Occupier(true) : cell.Cell_Occupier(false)` (the inline's own
  conditional folded twice) and the if/else form load the destination register in each arm. The image has the first
  in the train block of both While_Moving copies (0x676F60, 0x4A1000) and in Explosion_Damage's rocker loop
  (0x47E2C0), where main used a reference alias (`ObjectClass *& ref`) or an inline member and an earlier lane an
  occupier local; ra2-decomp's twin of Explosion_Damage has `Cell_Bridge_Occupier() : Cell_Occupier(false)` at that
  place. When a join copies a register that both arms could have loaded directly, look for a second accessor of
  the same field before adding a local.
- `DirType(facing).As_Dir8()` and `Dir_Facing(facing)` differ only in what they read (Raw, the whole word, against
  Facing), and VC6 narrows both to the same dword load; but the order of that load against a neighbouring stack
  load differs. In While_Moving the track jump's `tnum` loads nextface first in the image: no spelling of the sum
  (operand order, `+=`, a named facing, an int nextface), no named-local, data-key or call-pad probe and no header
  declaration count moved it; As_Dir8 at that one statement did, while the turn test a few lines above is right
  through Dir_Facing. A swapped pair of loads next to an inlined DirType conversion is the accessor, not a tie.
- Porting an accepted body with refused forms into clean text: score each of the old forms alone, then all, then
  all but one, then pairs. While_Moving (5,085 bytes off as adopted text) was 26 lines off with all thirteen of
  main's forms and looked as if every one was needed (each removal cost 20 to 450 lines); with three of them
  replaced by clean equivalents (the accessor conditional, the locomotor's own `Incoming(c.As_Cell())` for main's
  `Cell gate`, the member read for main's `ttype`), three more could simply go (`cell` at the tube, the Coord copy
  in the distance, either of them with `target`), but not all three together: the function is exact in a window of
  its named-local count (unused Cell locals: +0 to +3 exact, +4, +5 and +7 off, +6, +8, +9 exact; one fewer off).
  Ship's text has a `target` local where Drive's has TibSun's `maxspeed`, the same count. Two differences that
  always appear together under unrelated single removals (here a push scheduled across the Adjacent_Cell sum, and
  a 16-bit Cell sum at the tube) are count ties of this kind, not properties of the removed form.
- What still holds both While_Moving copies: `UnitClass * unit = (UnitClass *)LinkedTo` in the train block (the
  image keeps LinkedTo in a callee-saved register across the loop's calls, which a member re-read cannot give),
  `Map[Coord(coord)]` (a copy of the chosen coordinate is built for the lookup) and `Cell here` at the arrival test
  (the unit's cell is evaluated first and compared on the left; swapping the test's operands changes the
  evaluation order). No reference has this function matched (ra2-decomp omits it, mischa85's is unmatched), so the
  exact text stays under its guard as a held variation.
- Named-local ties, measured (Explosion_Damage 0x47E2C0, 12 lines from exact): the operand order of a 16-bit Cell sum
  and the order in which two expanded Length sites convert X, Y and Z follow the function's count of named locals.
  The probe that counts is an unused reference (`Coord const & p = coord;`) or an unused class local declared at the
  top: unused ints are dropped and count nothing, a Point2D counts two, header declarations before the function
  (1 to 65) move nothing, and a class local declared in mid-function is an inline site that costs budget instead.
  The sum is right at +1, +9 and +17 (period eight); the conversions have windows without a period (+3..+7 and
  +14..+24; +6, +7 and +17..+24), so the text is exact at +17, or at +6 with the sum written table first. Main's
  accepted body had the count from its coined Length helper (three named doubles at each of three sites). A probe
  result of this kind names a deficit of named locals, not a form: nothing lands from it.
- A constructor's argument scheduling can count the inline member-constructor sites before it. TechnoTypeClass's
  constructor 0x6D9B40 is 15 bytes off: the two pushes and the lea of Prerequisite's VectorClass<int> constructor
  sit one member store early among ten constant stores. Measured with a scratch wrapper class in the header
  (restored): any one scalar member before +0x544 given a class type with an inline one-argument constructor makes
  the row exact, the first member of the list as well as the one next to the vector; a member after that point
  moves other stores, and two or more wrapped members overshoot. An explicit default initializer (`Prerequisite()`,
  as the TibSun archive's constructor of the same signature has it), the order of the initializer list and an
  inline function inside an initializer do nothing. Main matched it by making GuardRange a coined `Leptons` class;
  every member type before Prerequisite equals both reference headers, so which member it is stays open. When a
  long constructor differs only in where a later call's arguments are scheduled, count member-constructor sites
  with arguments before spending time on the stores next to the call.
- Look for the author's idiom in the same unit before calling a local coined. The train block of While_Moving needs
  LinkedTo in a local across the loop's calls; main wrote `FootClass * train = LinkedTo`. The TibSun archive's and
  ra2-decomp's Stop_Moving of the same unit test a train as `if (LinkedTo->TClass->IsTrain) { UnitClass *unit =
  (UnitClass *)LinkedTo; if (!unit->IsFollowing) {`; the same three statements in While_Moving, with the loop's
  Take_Damage through `unit` and everything else as the archive's While_Moving has it, are exact in both copies.
  It is still not reference text at that place (the archive's While_Moving has the combined test), so it is recorded
  as a variation with its support, not landed.

## Empty constructors as inline sites, the reader templates under the cap, index signedness and frame maps (found in the rebuild (lane F3, high))

- Found in the rebuild (lane F3, high): an explicit empty inline constructor is an inline site of every expression
  that builds a temporary of the class (`ModemHost().Dialog()`), free to expand but counted in N for every nested
  site before it; a class with no user-declared constructor has no site there. Select_Serial_Dialog 0x5CDF00
  (2,580 B): the image expands TTimerClass's conversion at the first four waits of the inlined `Test_Null_Modem`
  and calls its out-of-line copy (0x5CFDB0) at the later ones; the draft called it at all of them, and 32
  `GetFocus();` pads changed nothing (the root sits on the 1000 floor, the nested share is what is left over the
  sites still to come). ra2-decomp's headers have no `ModemHost(void){}`, `ModemGuest(void){}` or
  `SerialEdit(void) {}` (the archive's and mischa85's do); without the three the nested share is large enough,
  the identical twin's text is exact and the unit is whole. When a nested site is refused under a floor budget,
  compare the constructors of every temporary built after it against the twin's headers before anything else.
- A function its identical twin defines first in the unit and that the tree's draft has at the end of the file
  with its helper declared `inline` just above is not the same translation: /Ob2 expands a later-defined static
  function at an earlier call site, so the twin's order (the dialog first, the `static` probe forward-declared and
  defined after Modem_Signoff, as the archive has it) costs nothing and keeps the file in address order.
- The reader templates and the two budget regimes. findmake.h's `!= 0` (the archive's and ra2-decomp's form) adds 2
  units to TGet_Class (64 against 62, il_lab in rules) and to each list reader. RulesClass::General 0x647F40 is not
  on its root budget but on the 35000 running-total cap: its last TypeList<int> temporary is destroyed fully
  expanded, and every `GetFocus();` pad past eight (4 units each) makes it call one level more (VectorClass<int>::
  Clear, then ~VectorClass, ~DynamicVectorClass, ~TypeList, 36 units a level). A pad raises S and so the running
  total: under the cap more root text means less expansion at the last sites, the reverse of the budget regime, and
  a charged probe at the top spends from the total too, so only removing text measures the other direction (one
  Get_Int statement fewer restores the image's list with `!= 0`). With `!= 0` General is 32 to 35 units over,
  Combat_Damage (budget regime, window -40 to +16 units without it) 16 to 32 short and Audio_Visual_Rules 9 to 15
  pads short; in TGet_TypeList alone it leaves General and Combat_Damage exact. Nothing landed: no reference has
  text for the three roots that pays for it.
- Reading the cap regime: pads that take expansions away in clean steps, each step one level of the same destructor
  chain, are the cap; a sawtooth or a gain is the budget.
- The signedness of VectorClass<T>::operator[]'s index is a global property of a function's code, not of the indexed
  sites. Dropship_Screen 0x4A6B90 (10,797 B) and WDT_Write_Map_Ini 0x72A880 (1,652 B) are exact but for bindings
  when the operator indexes unsigned (`unsigned` or `unsigned long` parameter, or `Vector[(unsigned)index]` in an
  int-parameter body; `long` changes nothing): five operand-order sites in the first (two `[index*4 + Vector]`
  loads, a reload register, an `add` for a `lea`, `cameo_top + (col + 2*row)`), one lea of the expanded Resize copy
  loop in the second, which no index operator touches. Red Alert 1's VECTOR.H declares `unsigned index` (and
  unsigned sizes); the archive, ra2-decomp, mischa85 and Renegade declare int. Over all 272 includers the unsigned
  parameter loses 24 exact rows in ten units (eleven of them inline-budget flips, a conversion per site) and gains
  these two; the body cast loses twelve in the same units. So the header stays int, and what the two functions
  have instead is open: not converted arguments as such (a scratch `U(unsigned)` accessor at all 63 expansions of
  Dropship_Screen gives a third state, at one to sixteen sites nothing), not dead reference locals (0 to 40), dead
  inline calls or declaration counts.
- A block-local loop index is a frame symbol of its own. Dropship_Screen walked the cross dissolves with the
  function's `i` (the archive's text); ra2-decomp's twin declares `int dissolve_index = 0;` in the block, and with
  it every stack slot of the 1.7 KB frame is the image's (222 differing lines to 34). When a large frame is a
  permutation of the image's and one slot short or long, look for a loop the twin walks with its own index before
  reordering declarations.
- Mapping a frame without a simulator. When the masked instruction sequences are equal (esp offsets replaced by a
  token before diffing), compile the unit with `/FAs /Fa<path>` through rebuild.compile_unit: the listing names every
  frame symbol with its offset and prints each reference as `sym$[esp+N]`, so N minus frame minus saved registers
  is the push depth of that instruction, and the image's slot of the same symbol is its raw offset on the aligned
  line minus that depth. ScoreClass::Presentation 0x663FE0 (7,782 B; equal but for one operand order and a frame of
  0x69C against 0x6A0): the image has two 8-byte Point2D slots where ours has one and five 4-byte slots where ours
  has six; `index` shares with efic01shape and `total` with logo01shape in the image, the other way round in ours,
  and `drawer` sits alone in the top scalar slot. The twin's text gives 0x6A8 for YR under its own headers and
  0x69C under ours, so the missing symbol is in a header inline, not in the function's text.
- Found in the rebuild (lane F3, high): for a near-matched twin, take the twin's whole function and map its names by
  rule before touching the tree's draft. BuildingClass::Do_MISSION_REPAIR 0x447260 (4,503 B) had been edited by
  hand toward the twin and sat 4,259 bytes off (a frame of 0x48 for 0x28 from two unscoped CLSID locals, `Anims[]`
  tests for the inline `Anim_Active`, a flag where the twin's loop uses `continue`); the twin's text with eight
  rule-based renames compiled first time and was one byte off, a call of the const `Radio_Link` (through the
  twin's `Contact_With_Whom()`) where the image calls the non-const one: the residue names the one RA2 difference.
  saveload's Save_Game and nulldlg's Select_Serial_Dialog fell the same way.
- A result held in one function-level local is root text. Put_All 0x6558B0: `res = Save_Vector(stream, X); if
  (FAILED(res))` (ra2-decomp's twin) against `if (FAILED(Save_Vector(stream, X)))` (the archive) is the same code
  and about four units more per statement, three more expanded list saves over 55 statements; the twin's
  Save_Vector body (locals first, early returns, `if (FAILED(result = ...)) return(result);`) is smaller than the
  archive's nested form by two more. The last list save needs eight pads of root, which seven or eight further
  `res` statements give: a count, so the exact text is held as a variation. A larger YR root (three more saves)
  is why the twin's own text has the room there: budget forms do not transfer between the two games' roots.
- The linkage of a small member is read from the whole image, not from one unit: count the direct calls of its
  out-of-line copy in every row. AbstractClass::Is_Techno has one copy (0x6F8A70, in uicmnds) and exactly one call
  in the executable, so it is the class's inline member (as ra2-decomp's abstract.h has it) that one root refused;
  the tree had it out of line with unit-local stand-ins elsewhere. Inline, CellClass::Goodie_Check 0x476CC0
  (6,672 B) has the image's call list at once (the five expansions also move N for Adjacent_Cell and two
  TPoint3D constructors). The body must be the tree's (`bool flag = AbstractFlags & 1; return(flag);`): the twin's
  `unsigned char flag ... return(flag != 0)` is six bytes off in the out-of-line copy. The cost of the correct
  linkage is that the copy has no emitter until its one caller (uicmnds 0x6F6D00) is exact.
- An inline body in a header moves the count that the count-sensitive rows sit on, by its shape and not by its
  presence. With Is_Techno defined in the class as `if (this == NULL) return(false); ...; return(...)` (two
  returns, with or without the `flag` local) queue's Print_CRCs 0x62A360 (4 bytes) and mapgen's Create_Urban_Area
  flip across 220 includers and nothing else does; as the single expression `return(this != NULL &&
  (AbstractFlags & 1) != 0);` neither flips, but Goodie_Check's five expansions are then not the image's. The
  two-return shape is the evidenced one, so the flips are count flips by evidenced content (the review's rule),
  not something to steer with the body.
- A masked diff that shows only a table offset is hiding a jump. Labels print as `L`, so a conditional jump to a
  different target is invisible until its encoding changes length: Goodie_Check was four bytes short with every
  line equal, the switch tables four bytes early. Decode both byte streams and compare instruction sizes: the first
  difference was a near `je` to the end of the switch where ours had a short one over the next statement (the
  reveal crate plays its sound inside the player test in RA2).

## Uncalled function-pointer helpers are inline sites; a constant test expands through them (found in the rebuild (lane F4, high))

- Found in the rebuild (lane F4, high): a unit's out-of-line helper that takes a function pointer and that the image
  neither calls nor takes the address of is used only through its expansions. uicmnds has four of them in a row
  (For_Each_Techno 0x6F6B90, For_Each_Techno_If 0x6F6BC0, All_Technos_Pass 0x6F6C10, Any_Techno_Passes 0x6F6C50): a
  scan of every `E8`/`E9` rel32 and every imm32 of .text finds no reference to any of them (the link kept
  unreferenced code). Every "count; for; element; test; act" loop of the unit is then a candidate site of one of
  them, and writing the loop open-coded gives the same instructions with a different inline budget.
- VC6 expands a call through a function pointer when the pointer is a constant at the site: after the helper is
  expanded with `(TechnoTest)Is_Unselected_Combatant` as its argument the inner `test(techno, argument)` is itself an
  inline site (il_lab in uicmnds: Is_Owned_Active, 44 units, expands inside Collect_Technos when its address is
  written at the call). Through a local (`TechnoTest test = (TechnoTest)Is_Owned_Active; Collect_Technos(test, 0)`)
  it is not a site: the call stays, and constant propagation later makes it direct (`call 0x6F6FE0`). So one function
  can hold both pictures: a direct call of a test whose pointer went through a local, and a loop with a test's body
  and no call, where the function was named at the site.
- The signature is in the budget, not in the loop. Select_All_Combatants 0x6F6D00 (725 B) was 98 bytes off at one
  site with both loops open-coded: the image expands Get_Selection_Scope_Text (116 units) and then calls
  Show_Player_Message (84 units) out of line, ours expanded both. The loops written as the unit's own helpers
  (`Any_Techno_Passes(list, (TechnoTest)Is_Unselected_Combatant, 0)` and `For_Each_Techno_If(list,
  (TechnoTest)Is_Selectable_Combatant, Select_Techno, 0, 0)`) charge the helpers and their tests and leave less than
  84 at the last site: exact, with the same loop instructions. Either helper alone does not do it (the final loop
  alone: 613 bytes off; the test loop alone: the same 98). Sizes read with il_lab in uicmnds: Show_Player_Message 84,
  Get_Selection_Scope_Text 116, Collect_Technos 128, Collect_Visible_Technos 148, DynamicVectorClass's (unsigned,
  T const *) constructor 80, AbstractClass::Is_Techno with its statement body 48, a ra2-decomp-style
  Has_Unselected_Combatants 112.
- The Yuri's Revenge twin shows the other side of it: there the final loop calls a real function of the helper's
  pointer shape directly (0x7325C0, `int __fastcall (TechnoClass *, int unused)`; ra2-decomp wrote the loop out and
  named the function Is_Selectable_Combatant). That is the same source with the test refused (YR's test is larger
  by CanBeSelectedNow), left as a direct call by constant propagation and so emitted. A twin's "real function with an
  unused parameter called from a hand-written loop" points at a pointer-taking helper in our unit.
- Such a test or action has no copy in the image when every site expands it: it is declared `inline` (the rebuild
  ruling "a function our object emits that the image lacks takes the linkage that does not emit it"); VC6 emits no
  body for an inline function whose address was only ever an argument that the expansion consumed.

## Image order tells in-class virtuals from out-of-line ones; fitting a budget from sizes (found in the rebuild (lane F5, high))

- A unit's select-any functions (virtuals defined in the class, `??_G`) lie after its ordinary functions in the
  image, in object order, although the object holds them right after the first constructor. So the tail of a
  unit's range reads off which virtuals the header defines: overtype, smudtype, airctype and terrtype end with
  Fetch_RTTI, Fetch_Object_Size, Fetch_Heap_ID, `??_G` (all three in the class, declaration order), while unittype
  ends with Find_Or_Make, Fetch_Object_Size 0x70B5D0, Fetch_RTTI 0x70B5E0, Fetch_Heap_ID 0x70B5F0, `??_G`. That
  order allows two forms only: all three out of line in that order (the TibSun archive's unittype.cpp ends with
  exactly these definitions) or Fetch_Object_Size out of line and the other two in the class (ra2-decomp). One of
  the two in the class alone (Fetch_RTTI) would put it after Fetch_Heap_ID. A virtual has no direct call, so
  nothing else in the image separates the two forms; an inventory difference of this kind moves no parked row.
- Defining two declared virtuals of a class in the class can flip a row that no unrelated probe moves: with
  UnitTypeClass::Fetch_RTTI and Fetch_Heap_ID both in the class, UnitClass::Draw_It 0x700E70 is 387 bytes off
  (either one alone: exact), while on the unchanged header 0..16 plain declarations and up to six unrelated
  in-class virtual bodies, inline functions or inline members leave it exact. Such a row does not meet the
  count-sensitive rule; it is the row that refuses the header form (it also refused lane F4's inline Is_Techno).
- Fitting a budget instead of searching forms: read the sizes of every callee of the row with il_lab (rocket:
  Pitch_To_Target 126, TPoint3D operator- 72, the 3-argument constructor 48 decided with half the rest, Length
  64, ObjectClass::Distance 46), write each disputed nest's states as intervals of the budget it receives (the
  cruise test: 182 and more expands constructor and Length, 166..181 calls Length, 134..165 calls the constructor
  instead; the dive's Pitch_To_Target: 168 and more expands the constructor), then place three probes: K empty
  inline sites before the last statement, one charged spend at the top, one charged spend just before the later
  nest. Their flip points give the remaining budget and the site count of each nest, and so what the original
  must differ by: here "about 170..213 units less before the cruise test, or one more site after the dive and
  1..47 units less". The non-monotone K response (K=1 fixes one nest, K=2 fixes the other and breaks the first)
  is the signature of two nests wanting different divisors: it rules out trailing sites alone.
- Pass-through named locals are budget: each costs 6 units of the root's size and so 12 of its budget once the
  root is past 500. In RocketLocomotionClass::Process three of them (`int distance`, `float rate`, the tail's
  `Coord coord`) are byte-neutral, and only without them does one more trailing site give the exact function.
  Remove byte-neutral aliases before measuring a budget.
- A coined wrapper that takes the object and calls Center_Coord() inside may be a real member: the cruise test's
  `Distance_To(LinkedTo, dest)` is ObjectClass::Distance(Coord const &) (TibSun's object.h), byte for byte. Each
  such temporary lies in an inlined body in the original: written at the site, the same call costs 8 bytes of
  frame per temporary (the slots are no longer shared).
- A probe helper that replaces statements also shrinks the root: an inline absolute value of the dive's pitch
  difference (one site in the right place) removes three statements from Process and overshoots to the K=2 state.
  Judge a candidate site by the exact function, not by the site count it adds.
- The same trailing-site reading holds in a second unit: BuildingClass::Captured 0x444660 (3,359 B) is the
  original's instruction for instruction with two or three empty inline sites before its last return (nine bytes
  left: three unbound literals), and one is not enough. Two shared-header explanations were tested and fail on
  building's own exact rows: a larger Length (through a named double result) and three other bodies for
  ObjectClass::Distance(Coord const &) each flip exact rows there and do not give Captured's calls. The missing
  sites are in each function's own text.
- A lead, not a finding: the trailing-site count equals the `new` plus `delete` expressions after the nest in
  rocket Process (1) and Captured (2), and those of the whole function in mapgen Generate_Rural_Roads (2; their
  place against its nest not checked); it fails in infantry Assign_Destination (0 against 3). Routing `new` through an inline pass-through operator new (scratch only) makes
  rocket Process exact; nothing shows what a `delete` would carry. Test it on a row with several of each before
  believing it.

## A missing trailing inline site can be a real accessor of the reference headers (found in the rebuild (lane F6, high))

- BuildingClass::Captured 0x444660 (3,359 B) lacked two inline sites before its last return (lane F5's probe).
  They are `oldowner->Production_Status_Changed(); newowner->Production_Status_Changed();`:
  `HouseClass::Production_Status_Changed(void) {IsRecalcNeeded = true;}` is a member of the TibSun archive's
  house.h (after Update_Production_Mode) and of ra2-decomp's house.h, which the rebuild's header did not carry.
  The archive's own Captured stores the flag directly at that place, and ra2-decomp calls the member once
  (temporal.cpp) beside three direct stores, so the member's use is read off the caller's budget, not off a
  reference text: a free (40 units or less) accessor expands to the very store, the standalone bytes are the
  same either way, and only the count of sites after the disputed nest differs.
- Recipe once a trailing-site probe gives K: list the statements after the nest that write or read one member
  through a pointer, and search the reference headers for an inline member whose whole body is that statement
  (`grep -n '{.*IsRecalcNeeded = true' include/*.h`). K real accessors used K times is the candidate; judge it
  by the exact function.
- Sites before the nest do not count: two or four empty inline sites at the top of Captured change nothing, two
  before the last return give the exact function. One charged spend at the top flips the same nest between 252
  and 312 units, so "two trailing sites" and "about 250..310 units spent earlier" are the same reading here.
- The natural spellings are byte-neutral in Captured: the greet loop through `reestablish[i]->` three times,
  `reestablish.Add(new Reestablish(...))`, the archive's `::Distance(contact->Center_Coord(), ...)`.
  `!contact->Is_Foot()` in place of the flag test is 16 bytes longer (the null test stays).
- One inline member added to a widely included class moves the declaration count like about two plain
  declarations: with Production_Status_Changed in house.h, OwnerDraw::Trackbar_Proc 0x5FB270's exact windows
  move from +0 +5 +6 +16 to +3 +4, wonline Draw_Player_List 0x73FEE0's from +0 +1 +3 +16 to +1 +16, and queue
  Print_CRCs 0x62A360 (parked count-sensitive by the inline Is_Techno) comes back exact. A function parked
  under its guard also leaves the count: wonline Pick_Lobby 0x744090 flips with the member in and is exact again
  once Draw_Player_List, earlier in the unit, is guarded. Run countsens.py after the guards are in, not before.
- A cast that names the member's own type is no code: ra2-decomp's `((unsigned char)AbstractFlags >> 2) & 1` in
  ObjectClass::Is_Foot leaves all 174 units that include object.h unchanged (AbstractFlags is an unsigned char).
- A draft that is "one site off" may rest on coined helpers: TechnoClass::Draw_Post_Render 0x6C2C40 is 16
  instructions off (the talk bubble timer's Value expanded, the original calls it) only with the unit-local
  Mouse_Hover_Flag / Techno_Is_Selected readers of main's text; with ra2-decomp's plain
  `IsMouseHovering && !IsSelected` the register allocation differs through the whole function (ebp for `this`).
  Check the helpers a draft uses before reading its bucket as an inline-budget question.
## A dispatcher whose budget runs out, handler sizes at the free limit, and budget windows read with pads (found in the rebuild (lane F7, high))

- TActionClass::operator() 0x6ADAE0 (7,752 B) is not a 35000-cap cut-over (lane R42's reading above). After
  TAction_STOP_AI_TRIGGERS the image still expands PLAY_INGAME_MOVIE, ACTIVATE_FIRESTORM, DEACTIVATE_FIRESTORM,
  PAUSE_SCENARIO and PLAY_INGAME_MOVIE_PAUSE and calls every other handler: the free handlers (40 units or less)
  go on expanding and the charged ones are refused, which is the root's budget running out. The whole instruction
  diff of the rebuild's text is the four handlers it expands and the image calls (STOP_AI_TRIGGERS 58,
  SET_AI_TRIGGER_TEAM_RATIO 50, SET_TEAM_AIRCRAFT_RATIO 50, SET_TEAM_INFANTRY_RATIO 50 units); nothing else differs.
- Handler sizes in the unit (il_lab.py, `lab_a->TAction_X(lab_h, lab_o, lab_t, *lab_c);` with
  `#define private public`, step 2; 85 handlers in one five-second run). Free: TOGGLE_TRAIN_CARGO, VEIN/TIB/ICE_GROWTH,
  ANNOUNCE_WIN/LOSE, FORCE_END, CREEP_SHADOW, PLAY_SPEECH, CHANGE_ZOOM, LOCK_INPUT, UNLOCK_INPUT, RESHROUD,
  SET_AMBIENT_STEP/RATE, PLAY_INGAME_MOVIE, ACTIVATE_FIRESTORM, PAUSE_SCENARIO. A handler of one member call with two
  arguments and `return(true)` is 42, just over the limit (SET_GLOBAL, CLEAR_GLOBAL, SET_LOCAL, CLEAR_LOCAL,
  PLAY_SOUND, PLAY_MUSIC, ION_STORM_STOP; PLAY_MOVIE and ALL_HUNT 44, ION_LIGHTNING_STRIKE 46, PREFERRED_TARGET 48).
  Others: DESTROY_TEAM 50, REINFORCEMENTS 52, RADAR_EVENT 54, BEGIN_PRODUCTION/FIRE_SALE/AUTOCREATE/
  BEGIN_AI_TRIGGERS 58, CREATE_TEAM/SET_AMBIENT_LIGHT 60, REDUCE_TIBERIUM/START_TIMER/STOP_TIMER/ION_STORM_START 62,
  REINFORCEMENTS_SPECIAL 64, LIGHT_SMALL/MEDIUM/LARGE 66, ZOOM_OUT 74, MAKE_ALLY/MAKE_ENEMY 82, ADD_TIMER 86,
  ZOOM_IN 92, ION_CANNON 104, PLAY_SOUND_AT 106, 1_SPECIAL/DISABLE_TRIGGER 120, REVEAL_ALL 122, FORCE_TRIGGER 124,
  TEXT_TRIGGER/SET_TIMER 126, ALL_CHANGE_HOUSE 130, GO_BERZERK 140, SELL/TURN_OFF/TURN_ON_ATTACHED 142,
  WAKEUP_ALL_SLEEP/HARMLESS 144, FULL_SPECIAL 146, CENTER_VIEWPOINT/DESTROY_TRIGGER/DESTROY_TAG 148,
  CHANGE_HOUSE/SUB_TIMER 152, PLAY_SOUND_RANDOM 158, SET_TIMER_TEXT 160, WAKEUP_GROUP 164, LIGHTNING_STORM 166,
  BASE_BUILDING and CHANGE_SPOTLIGHT_BEHAVIOR 172. With so many bodies within a few units of 40, one extra node in
  a free handler charges its whole size.
- The dispatcher's deficit, read with pads. `GetFocus();` statements (4 units each) spread over six small handlers
  after BASE_BUILDING (SET_GLOBAL, CLEAR_GLOBAL, SET_LOCAL, CLEAR_LOCAL, WIN, LOSE): 44 to 52 of them (176 to 208
  units) give the image's call list with no other site moved; 40 leave SET_AI_TRIGGER_TEAM_RATIO expanded and 56
  refuse BEGIN_AI_TRIGGERS. So the budget left at the cut is 208 to 221 units here and under 46 in the image
  (ION_LIGHTNING_STRIKE, 46, is refused there too). The same pads in the first handler are not equivalent: 40 units
  are tolerated, 80 refuse the nested Coord::As_Cell in BASE_BUILDING (the image expands it and calls
  Cell(int, int) 0x42AD30), so at most about 40 to 79 of the missing units lie before BASE_BUILDING. Put pads where
  the hypothesis puts the wordiness; a pad host must stay under the size cap (a padded SUB_TIMER goes out of line).
- BASE_BUILDING with the archive's and ra2-decomp's `BuildingClass *building = house->ConYards[0];` local is called
  out of line even with 60 extra call statements in the root (480 units of budget): the non-inline size cap (about
  175), not the budget. The tree's one-statement form (172 units) stands.
- A real member spelled out by hand is not always the answer. LIGHT_SMALL/MEDIUM/LARGE call
  `Waypoint_As_Coord()` in the archive and in ra2-decomp's twins (their YR dispatcher 0x6DD8B0 is matched); the tree
  writes `Scen->Get_Waypoint_Coord(Data.Value)`. Both are exact standalone. Waypoint_As_Coord is 48 units and the
  handler through it 56 (66 by hand), so each expansion charges 104 against 66: three of them move the cut two
  handlers toward the image's but refuse BASE_BUILDING's As_Cell. Sixteen units more in the root (four call
  statements) bring As_Cell back with the cut still two handlers late. Read together: if the original has the
  member there, its root is some 16 units larger and its handlers after BASE_BUILDING 62 to 158 units wordier
  still. No reference text supplies either; TActionClass::Get_Text (0x6B2F20, free) in SET_TIMER_TEXT in place of
  ra2-decomp's `Text[0] ? Text : NULL` loses the handler's own row (127 bytes) and changes nothing in the
  dispatcher; the root without the archive's unused `Cell waypoint; Coord coord;` (ra2-decomp's matched root has
  neither) moves the cut one handler.
- Init_Game 0x5116A0 (4,420 B): the budget window at the SOUND.INI load-failure return. `GetFocus();` at the top of
  the root (8 budget units each): 0 to 3 nothing; 4 expands eva_ini's CCINIClass constructor instead (the
  constructor is smaller than ~CCFileClass); 5 to 11 the image's call list; 12 the constructor again; 13 and more
  expand the EVA.INI failure path's destructors too. With four pads, removing one `(void *)` cast of Bootstrap's
  palette loads is enough: the image has 33 or 34 to about 95 budget units more than the rebuild at that site. All
  sixteen memmove casts of the archive's text weigh three pads (24 units): an explicit pointer cast in an expanded
  callee is about 1.5 units.
- What does not supply them: the archive's own calls that the draft spells out or drops (Init_Heaps(),
  Init_Threads() as empty statics, Anim_Init() for its two statements, Init_Color_Remaps() for the colour scheme
  store) are byte-neutral and two or three units worse (with four pads three casts must go, not one); main's
  hand-written `CD().Force_Available(RequiredCD)` for the archive's inline CD::Force_Available changes nothing
  measurable (one pad is the resolution). main's exact text reaches the window by hand-spelling (no casts, fields
  for the RGB accessors, no CD wrappers).
- The nearest evidenced form: ra2-decomp's twin (their YR Init_Game 0x52BA60 is in their manifest of matched
  functions) writes Bootstrap's palette loads without the archive's casts, `memmove(&pal[0], <load>, sizeof(pal))`,
  and stores `VoxelDrawSystem::EnableLighting`/`EnableZBuffer` directly. In the RA2 draft the cast-free loads are
  those 24 units (the call list then needs two pads instead of five) and the direct stores add nothing; 8 to 10
  budget units are still missing after them, and no reference text supplies those. Not landed: the row stays
  parked, the archive's casts stay in the draft.
- A reference header form can be refuted by exact rows. ra2-decomp's `WOLOptionsClass(void) : Options(0),
  IsValid(false) { Load(); }` (all sixteen users here construct and then call Load) loses wonline Login_Wonline
  0x742FD0 (1,782 bytes differ) and Host_Game 0x7459A0 (947), and gains nothing in init, startup, wolstart or
  wolpersonainformation. The explicit `options.Load();` after an empty-bodied constructor is RA2's form as far as
  those two rows show.
- A row "exact with every guard open" may need one body before it, any body. SidebarClass::Reposition_Sidebar
  0x6819A0 (1,314 B): 2 bytes off with its own guard open; exact with its guard and Activate's (0x67E210) or its
  guard and the unplaced TibSun Abandon_Production draft; 4 bytes with SidebarClass::AI's guard, 6 with all but
  that one. It is a declaration-count row that returns when Activate lands, and Activate has no bound on main.

## Count-sensitive rows flip one change at a time and return under sets of changes (found in the rebuild (lane F8, high))
Found in the rebuild (lane F8, high): queue Print_CRCs 0x62A360 (11,223 B) and wonline Encode_Game_Options 0x73CCB0
(797 B) lose 4 to 12 and 4 to 6 bytes with almost any single evidenced header change and are exact again under the
right set of changes. Windows are read with `struct P;` lines put on one existing line (no line moves), at two
places: the first line of the unit (before every header) and the function's own line (after every header).
- Encode_Game_Options: the only difference at every count is the frame order of three equal-weight locals
  (`int address = 0; int port = 0; int ping = 0;`, zeroed before the slot loop, reloaded after it). The image has
  address at [esp+2Ch], port at +28h, ping at +24h; the off form rotates them (address +24h, port +2Ch, ping +28h;
  a 4-byte form at window edges) and the three reloads after the loop follow the slots. First line of the unit:
  exact at +0, +22..+38, +41..+62 (40 of 64; period 64 confirmed at +64..+66). Own line: exact only at -1, +0,
  +38, +40 (4 of 64). Declarations between the headers and the function move it far more than declarations before
  both (Scout1114's two edges); a late declaration of weight 1 always flips it.
- Print_CRCs: three independent sites, each the order of two spill reloads after the long fprintf at the foot of a
  per-house object loop (`mov edi,[esp+74h]` / `mov eax,[esp+44h]` swapped, 4 bytes a site). First line of the
  unit: exact at +0 +3 +4 +7 +8 +9 +16 +42 +56 +57 +60 +63 (12 of 64). Own line: exact at +0 +3 +4 +5 +7 +8 +9
  +16..+21 +23 +56 +57 +60 +63 (18 of 64).
- Probe weights on these rows: `struct P;` 1; a plain prototype `void f(void);` 2 a line (Print_CRCs on its own
  line: structs +1 4 B, +2 8 B, +3 exact, +4 exact; prototypes +1 8 B, +2 exact, +3 8 B); a new member declaration
  2; an in-class virtual body turned into a declaration moves Encode_Game_Options' own-line window by 4 the other
  way (ObjectClass::Docking_Coord: exact at +3 +4 afterwards) and leaves Print_CRCs exact.
- A prototype left above a definition bears count. `void Encode_Game_Options(char * out);` stood directly above
  the definition in wonline.cpp since the row's guard came off (wonline.h already declares the function; its
  comment said the definition was not exact). It weighs 2, not the 0 Scout1114 gives a redeclaration: with it the
  row is exact at -1/+0 on its own line, without it at +1/+2. Remove such a line in the landing that measures the
  row, never by itself.
- Single changes flip, sets restore. Six held changes, each evidenced, each costing at least one of the two rows
  alone: D ObjectClass::Docking_Coord out of line; S the SelectClass member declaration after Set_Owner (ra2-decomp's
  place); H ScoutStruct's copy constructor (ra2-decomp's _house.h); C OwnerDraw's ColumnData copy constructor
  declared in the class; P the five never-defined CCINIClass Put_ declarations removed; R the stale prototype
  removed. Of the 63 non-empty subsets both rows are exact under exactly seven: S+C, H+C, D+S+H, C+P+R, D+S+P+R,
  D+H+P+R and all six; no subset of five. On the full set the windows are again narrow (own line:
  Encode_Game_Options exact at +0 only of +0..+6, Print_CRCs at +0 +5 +6).
- A set that suits two rows can cost a third: check every count-sensitive row of the tree on each candidate set.
  sidebar SelectClass::Action 0x6809F0 (3,083 B, the first DoType row) loses 4 bytes with P alone and under every
  set that holds P (C+P+R, D+S+P+R, D+H+P+R, all six), and stays exact under S+C, H+C and D+S+H. The sets that
  keep all three rows are therefore S+C, H+C and D+S+H, and they exclude one another (D+S+H+C costs both rows of
  the pair): C and D meet only with P and R, and P costs Action.
- So held header changes are measured as a set and landed as a set: apply each subset of the delivered texts to
  the headers, check the fragile rows, land the largest subset that keeps them (Scout1114 r1's rule for held
  header edits, applied to evidenced content only; nothing is added, removed or reordered for the count, and a
  set that cannot be found parks the rows under the DoType precedent). The weights depend on the delivered text
  and its place: two new free-function prototypes appended to txtlabel2.h and ownrdraw.h moved neither row here,
  while the worker's placement of the same two flipped both.
- Reference forms do not harden Encode_Game_Options. ra2-decomp's matched YR twin (0x77E430) has the same named
  local for the maximum players (`int value = Session.ScenarioMaxPlayers;`), evidence for the tree's flagged local
  (505 bytes off without it). Its `description` local leaves the same 6 bytes at +0..+5; its
  `memcpy(&address, node, sizeof(address))` compiles to the tree's bytes with the same window (-1, +0); its loop
  text (slot_name, player_name, stricmp written out) is 143 bytes off at every count.
- Landed as a set (lane O10, owner ruling): with the workers' delivered texts the set's fragile rows changed again. All six with r2459's Blend_Rect line after Resize_Dialog_To_Screen cost Encode_Game_Options; the six alone cost SelectClass::Action and loaddlg LoadOptionsClass::Dialog 0x53B770 (9 B, off only when D, H, C and P are all in); the six with codex1 r2486's form (Inflate_Rect in rect.h, Blend_Rect in ownrdraw.h, four unit-local copies removed) keep the pair's Encode_Game_Options, Action and Dialog and cost Print_CRCs and Pick_Lobby 0x744090, both with DoType windows on both header states (probes at the end of rect.h), so those two park. A fourth row's measurement on old texts says little about the final set: measure the final texts together.

## A name local in place of an accessor removes the pending sites that keep a nested template a call (found in the rebuild (lane F9, high))
Found in the rebuild (lane F9, high): AnimTypeClass::Read_INI 0x425690 and the out-of-line TFind_Or_Make<AnimTypeClass>
0x426890 (128 B), and what holds the 92 rows that are exact only with their guards open.
- The image's Read_INI expands TGet_Class and AnimTypeClass::Find_Or_Make at "Next" and "Spawns" and calls
  TFind_Or_Make<AnimTypeClass> out of line at both. The tree read the section name once
  (`char const * section = Name();`) and passed the local to its 63 INI calls; with that text both TGet_Class and a
  direct TFind_Or_Make expand the template at both sites (2,060 bytes off), which is why the unit carried a coined
  static inline lookup and an external stand-in for the instance. With the archive's and ra2-decomp's text,
  `Name()` at every call, the 63 pending accessor sites keep the depth-3 TFind_Or_Make a call, the instance is
  emitted and both rows are exact from findmake.h (the UICmnds206 rule: the sites still pending later in the caller
  decide a nested expansion, not definition order; Find_Or_Make itself is defined after Read_INI and still
  expands there).
- Recipe: when a root expands a helper the image calls only through a coined stand-in, list the inline accessors
  the reference text calls per statement that the draft has folded into one local (`Name()`, `Count()`,
  `Length()`); restore them before trying budget pads. A free accessor emits the same bytes as the local (the
  compiler keeps `this + 0x24` in a register either way) and only its pending sites differ.
- A guard labelled "no independent bound yet" can hide an exact function. Main records its never-matched functions
  with their extent in manifests/original_target/non_exact.json and no catalog row; of the 67 such guards (31 units)
  opened alone and compared with the image over that extent, one was exact as it stood: MapGeneratorClass's
  constructor 0x577010 (404 B, main's best 86.79). Its bound is the row every frozen pass holds plus the 12-byte
  NOP gap row of seeded-pass5-aligned, between two exact rows of the unit (`frozen_bound` in units.json, the
  session 0x673090 precedent). Re-run the comparison after header landings; the nearest now are
  MapClass::Damage_Low_Bridge_EW/_NS (5 bytes each of 1,302) and radar Update_Diplomacy_Row 0x634670 (6 bytes).
- What holds an "exact with the guards open" row is almost never the row. Of 92 rows in 20 units, 2 are variation
  candidates whose guard could come off as it stands (drive While_Moving, saveload Put_All), 1 is a
  declaration-count row (sidebar Reposition_Sidebar), and 89 are template or inline copies that only a parent
  which is not exact emits; no unit showed a COMDAT-order or instance-move cause. The bytes at stake are the
  parent's: read the bucket by parent (`rebuild/tools/guardprobe.py UNIT`), not by row.
- One Union site, three functions. The whole difference of builtype Get_Draw_Rect 0x45E290 and of
  MapClass::Damage_Low_Bridge_EW 0x55DEF0 and _NS 0x55E410 is the same five bytes: which right edge of the
  expanded Union's `result.X+result.Width < rect2.X+rect2.Width` is loaded first (rect2's in the image).
  Union's text is the archive's and ra2-decomp's and its out-of-line copy 0x45EFE0 is exact; a header form that
  turned the compare (lane R52) was worse.
- Dropship_Screen 0x4A6B90's five operand-order sites do not move with file-scope declarations: 0 to 66 probe
  structs on the function's own line or on the first line of the unit leave all 34 differing lines as they are.
  A row with operand-order sites is not thereby a declaration-count row; sweep one full period before parking it
  as one.
- WDT_Write_Map_Ini 0x72A880 sits on an inline-budget edge: two dead locals at its top, or the uncalled
  Territory_Of_Color 0x72A870 used for its two colour comparisons, expand the VectorCursor constructor that the
  image calls. A dead-local survey of its one lea tie reads the budget, not the tie.

## The global tie counts signedness conversions, not index operators (found in the rebuild (lane F10, high))
Found in the rebuild (lane F10, high): Dropship_Screen 0x4A6B90 (10,797 B) and WDT_Write_Map_Ini 0x72A880 (1,652 B),
the two rows that are exact but for bindings when VectorClass<T>::operator[] indexes unsigned (lane F3; codex1
r2491 to r2495). Nothing landed: the form is a perturbation, not the text.
- What the unsigned index does is add one same-size signed/unsigned conversion to the function's IL per expansion,
  and the five sites answer to how many such conversions the whole function holds, wherever they are. Measured on
  Dropship_Screen with a scratch accessor `UX(int index) {return(Vector[(unsigned)index]);}` (never landed) put at
  chosen sites: no single site matters (each of the 33 direct sites can be left out of the full conversion with no
  change; converting only the two scroll-loop `candidates[]` sites and the hover site, the sites that differ,
  changes nothing). The states come in a fixed order as conversions are added: the Get_Data `add` for `lea` first
  (14 direct sites, or the nested ones in Add/Delete_Index/ID and Get_Allowable_Index), then
  `cameo_top + (col + 2*row)` (nested plus 14 direct), then the two `[index*4 + Vector]` loads and the reload
  register together (nested plus 25 direct), and past that window the function leaves again (all 63 expansions plus
  21 raw sites: 64 bytes off; 33 direct plus 18 raw: frame change).
- The unit is not the operator. `buttons[(unsigned)(i)]` on the raw pointer arrays moves the same counter (12 raw
  sites give the first state, all 33 the second, 33 direct plus 12 raw the exact one), and so does one cast on a
  constant in ordinary arithmetic (`(unsigned)_cameo_count + 2 + ...` or `(unsigned long)`, one step short of the
  window, completes it). What does not count: `(long)`, an enum cast (the archive's `KeyNumType input`), pointer
  casts, a comma expression, a dead local, ten dead locals, `(int)(unsigned)index` (folded), an unsigned parameter
  with `Vector[(int)index]`, `*(Vector + index)`, `((void)0);` or Renegade's compiled-out assert before the return.
  `(__int64)index` reaches the second state only. Return types count too: `unsigned` on ShapeSet::Get_Width and
  Get_Height (scratch; 46 uses in the function, no other byte moves) gives the first state. Weights are not uniform
  across sites (raw sites move it further than operator sites, return conversions less), so read thresholds on one
  kind of site at a time.
- WDT_Write_Map_Ini is the same counter at small scale: its one lea (the expanded Resize copy loop) turns when the
  VectorCursor dereference and the two `[index]` sites whose index is already an `unsigned` variable index
  unsigned together (four expansions; the dereference alone, the three direct sites alone, or the dereference
  with `centroids[i]` do nothing).
- Evidence against the header form. Yuri's Revenge 0x4B6C30 (ra2-decomp: exact with `operator[](int)`) has the same
  three statements in the order the int header gives (`[eax + ebx]`, `[edi + ebx]`, `lea eax, [edx + ecx]`; read
  from the user's installed gamemd.exe). The archive, ra2-decomp, mischa85 and Renegade declare int; only Red Alert
  1 declares unsigned. Every twin and archive statement that differs from the tree's inside the function
  (`AlphaBuffer->Bounds`, `(unsigned short)falpha`, the two spelled-out MINs, `techtype->Level` without the cast,
  `delete []`, the typed shape buffer, `KeyNumType input`) leaves all 34 lines as they are; the archive's
  `!Count() ? -1 :` guard and `if (a || b) a = true;` change bytes. So Red Alert 2's own text holds some 45 or more
  signedness conversions that the tree's does not (types of values the function mixes, not one statement), and no
  reference shows where. Not its own locals: each of the function's 81 `int` and `static const int` declarations
  made unsigned alone either changes bytes beyond the ties (38, the image fixes them signed) or changes nothing (43). Before spending on a row with operand-order ties, count how the candidate fix changes
  unsigned arithmetic: a fix that works only by adding conversions everywhere is this counter.
- Reading it: `rebuild/tools/tailmap.compile_text` on a scratch copy with the guard open, one edit list per variant,
  about one second a compile; sweep the number of converted sites in source order from both ends before reading any
  single site as the cause.

## A mixed-order member sum comes from a table-first helper called through a reference local, and equal locals tie by object, not by count (found in the rebuild (lane F12, high))
Found in the rebuild (lane F12, high): the register_order bucket of `rebuild/manifest/tail_buckets.csv`, one lane-hour.
- A two-member sum whose members take opposite operand orders is not an operand spelling. MapClass::Destroy_Bridge2_Near
  0x556520 and Destroy_Low_Bridge_Near 0x557090 (4 bytes each) add the deck walk's X as table + cell and its Y as
  cell + table. `Adjacent_Cell(from, step)` (inline.h: cell + table) gives cell-first for both; the direct sum
  `AdjacentCell[step & 7] + walk` gives table-first for both and also turns the later `(table + walk) + table`
  statement (one-object spellings are shared across sites). Only the twin's statement gives the mix: a file-local
  `static inline Cell Adjacent_Cell_L(Cell const &cell, FacingType facing) {return (AdjacentCell[(int)facing & 7] + cell);}`
  called as `Cell const & from = walk; walk = Adjacent_Cell_L(from, step);` (ra2-decomp src/map.cpp,
  DestroyConcreteBridgeAt and DestroyWoodBridgeAt). Without the reference local (`walk = Adjacent_Cell_L(walk, step)`)
  the frame changes (0x30 for 0x34) and 1,250 bytes differ. Read a mixed sum as "a helper with the other operand order
  at this one site", and look for it in the twin unit before trying spellings.
- Two locals with equal uses, each loaded from a global, tie by the objects they are loaded from. netshare
  Set_Slot_Names 0x5C8DD0 (4 bytes): `open` and `closed` are read from the fixed lobby entries. With the entries as
  one array (`LobbyEntries[0]`, `LobbyEntries[1]`, mischa85's form) the declaration order sets the load order and the
  later-declared local takes ebx, so the image's pair (open loaded first, into ebx) cannot be written: either the
  loads or the registers are exchanged. With the entries as separate objects (main's lobby.hpp and ra2-decomp's
  DAT_00AC119C.. both declare seven pointers) the twin's order (open, then closed) is exact, in either declaration
  order of the two globals. The tie does not move with file-scope declarations: 0 to 63 probe structs before the
  function leave the 4 bytes as they are. A register exchange between two locals loaded from neighbouring elements of
  one array is a question about the array, not about the function.
  Not landed: with the seven entries declared separately tree-wide, netshare keeps every row and gains this one, but
  wonline's CChatEventSink::OnPrivateGameOptions 0x761C50 (2,360 B), which reads two of the entries, loses one
  `lea ebx, [eax + edi]` operand order in its user-list walk at every count tried, and Draw_Player_List 0x73FEE0
  flips by count (the patch is kept in private/dossiers/f12_lobby_entries_separate.patch). One function's tie under
  the array form and another's under the separate form: settle the reader of two entries first.
- A unit-local explicit specialisation of a template member reaches every later site of that type in the unit.
  wonline's Add_Test_Player_Ol_Cruddy 0x7513E0, Add_Test_Player_Potato 0x7514D0 and Add_Test_Player_Potato_Too
  0x7515B0 (2 bytes each: the image's growth sum is `add ecx, eax`, in the GrowthStep register) have main's and the
  twin's text word for word, do not move with 0 to 63 probe structs, and do not move with a standalone vector object
  in place of `Session.Players`. The cause is 3,500 lines above them: `template<> inline bool
  DynamicVectorClass<NodeNameType *>::Add` (the named `new_length` form, lane R21, written for Host_Game 0x7459A0's
  one growth sum, which the image has the other way round). Without it the three are exact and Host_Game is 8 bytes
  off (`add ecx, eax` for the image's `add eax, ecx`); main matched all four with one Add, so the image's two
  orientations are the whole-function count of the Scout1065 note, not two bodies. When a small function's inlined
  template member differs and its text is the reference's, grep the unit for `template<>` before sweeping counts.
  Not changed here: parking Host_Game also unemits three rows only it references (Close_Wait_Window 0x74D780, the
  WonlineStringDialogControl constructor 0x765AA0, CreateGameDialogControl's deleting destructor 0x765AC0), 2,361 B
  for 719 B; link2.cpp and objtype.cpp carry the same kind of specialisation. Apply_Endpoint_Map 0x75C570 (2 bytes,
  the reverse orientation on a `DynamicVectorClass<MultiMission *>` with no specialisation) keeps its bytes at every
  one of 0 to 63 probe structs.
- A flat floating-point sum handed to a double parameter is sorted before the argument slot is reserved, so the
  place of `sub esp, 8` does not answer to the sum's text. TechnoClass::Railgun_Beam_Damage 0x6D6ED0 (19 bytes): the
  image reserves the slot after the third product, ours after the second; `x*x + z*z + y*y`, `x*x + y*y + z*z`,
  `y*y + z*z + x*x`, the twin's `y*y + (x*x + z*z)`, a named `double sum` and a float-returning root all give the
  same bytes (reading the members without the three double locals changes the loads instead), and so do the
  helper declared `__cdecl`, the helper placed as main has it, and 0 to 63 probe structs before the function.
  Main's accepted text is the same helper; what differs is outside the helper.
- Twelve commutative sums over one table do not map one to one onto their spellings. LaserDrawClass::Draw_It
  0x5334F0: with the function statics masked the tree's text is one load pair from the image, in the second
  antialiased glow line. Arguments are evaluated right to left, so the end sum is the first in the code; read the
  frame offsets of the two points before naming a site (the first reading here had them reversed, and the sums
  looked cross-coupled). Four sums are inert (both of the second multiplying line and of the first shaded line; the
  last shaded line's pair moves together); all 256 orders of the other eight were compiled and none is exact, ten
  tying at one exchanged load pair. For the differing line, `start + g, end + g` has the start sum right and the
  end sum's loads exchanged; `start + g, g + end` (the twin's order for the line) has the end sum right and the
  start sum's loads exchanged; `g + start, g + end` is another body, 16 bytes shorter. Named points for the line change the frame, so the image passes temporaries. What is left under
  the twin's order is one commutation of a sum whose operands are both loads: lane F10's global tie, not a
  spelling. A masked diff (statics normalised) is the first step for a row whose first difference is an unbound
  static, and a full search of the live sums costs about a second a pattern.

## A bool function that returns an `||` chain sets eax, and its callers test al; inline shares measured by padding the callee (found in the rebuild (lane F14, high))
Found in the rebuild (lane F14, high): two inline_call rows worked from prepared dossiers, one lane-hour.
- **`return a || b || ...;` in a `bool` function ends `xor eax, eax` / `mov eax, 1`; `if (...) return true; return false;` ends `xor al, al` / `mov al, 1`.** ownrdraw's Is_Listed_Kind 0x5E0DA0 (and Is_Listed_Dialog_Kind 0x5E8590) return through eax while every caller in the image tests al (0x5E08ED, 0x5E491B, 0x5E9614; 0x5E834B, 0x5E8424, 0x5E84FF); Kind_Test 0x5DF660, the if/return form, returns through al. One `bool` prototype with the chain as the return expression gives both the eax body and the al tests: no int body behind a char prototype, no `(char)` cast at the caller. `(chain) ? TRUE : FALSE` gives the same bytes; `(chain) != 0`, a BOOL local, a bool local set in an if, and nested `?:` do not.
- **Enable_Animated_Control 0x5E4780 is ra2-decomp's text with RA2's id lists, apart from one call.** With the twin's Get_Dialog_Type -> Get_Data -> getPointer levels the first lookup expands getPointer and calls keyHash 0x602020, and the two Is_Plain_Control sites and Set_Control_Disabled expand both, as the image has it (il_lab in the unit: Get_Dialog_Type about 42, Get_Data 48, getPointer 132, keyHash 48, Is_Plain_Control 88, Is_Listed_Kind 95, Is_Flag_Control_A 185, Is_Quick_Control_B 88, Is_Quick_Control_D 216, Is_Listed_Dialog_Kind 165, Kind_Test over the cap). What is left is Is_Listed_Kind: the image calls it out of line at all three of its call sites and ours expands it. No budget explains that: at the root it would need less than 95 units left where the later sites still get 180 each, and inside Is_Flag_Control_A the first lookup's share (at least 180 a site) and the helper's (under 95 after its own 185) cannot both hold for any site count. With the predicate kept from expansion (a declared-only name as the twin has it, or `#pragma auto_inline(off)`, both measured as probes only) the row is exact. The four places where the image shows the list expanded (Control_Height, Control_Kind, Control_Flags, Is_Owner_Tested_Control) are one explicit list in the twin too, so nothing in the image need be an expansion of the predicate. Placement: each of the three kind predicates lies directly after its lowest-address caller (Kind_Test 0x5DF660 after Control_Height, Is_Listed_Kind after Is_Tested_Control, Is_Listed_Dialog_Kind 0x5E8590 after Init_Control_Animation 0x5E8130), which is where VC6 puts an on-demand copy of an inline function; but Is_Listed_Kind declared `inline` and defined at the end of the file is still expanded here (probe).
- **Measuring a callee body's share in place.** Pad the expanded helper with k distinct array-element stores (6 units each; repeated stores to one scalar do not count) and watch its nested sites flip: the k of each flip, set against il_lab's thresholds for the helper alone, gives the share y = (budget left - s) / N at k = 0 and N from the spacing of the flips (6/N per store). strtable LoadFile 0x6F8E30: ours gives the header helper about 385 units (N = 3: the helper, Strip_Line, GetLanguageName; about 1,290 left at its site); the image's pattern (the failing exit calls ~CCFileClass 0x51AD20, the other exit expands it down to ~BufferIOFileClass) needs 155 to 185, which is N = 7. ra2-decomp's `static inline _Read_Header` form (header declared first, early return) changes nothing here: both forms sit in the same state. Main reached N = 7 with four coined wrapper sites (Open_File once, Close_File three times). Declaring Read_Text_Entry and Read_Text_File file-static raised N by one only.

## A tiny comparison kept out of line at the top of a function is a missing accessor level (found in the rebuild (lane F11, high))

- The image keeps five calls to `TPoint2D<int>::operator==` (0x4BFB60, 46 units) in four functions: infantry
  Start_Driver 0x504BC0 (twice), fly Nearing_Target 0x4BE720, hover Process 0x4FBFC0 and hover Motion_AI 0x4FDAF0.
  Each discards the result (the `Facing = 0` store of `DirType::Direction(Coord, Coord)` is dead) and each follows a
  virtual `Center_Coord()` of the object asked. At a function's first statement the root budget is whole, so a
  46-unit callee cannot be refused one level down: `(1000 - 98) / N` stays above 46 until N reaches 20.
- The missing level is Red Alert's inline member, `DirType Direction(COORDINATE coord) const {return
  ::Direction(Center_Coord(), coord);}` (ABSTRACT.H:115; INFANTRY.CPP:2089 calls it as `Direction(headto)+DIR_S`).
  As `ObjectClass::Direction(Coord const & coord) const {return(DirType().Direction(Center_Coord(), coord));}` it
  measures 58 units (il_lab, infantry), `DirType::Direction` 98 inside it and `operator==` 46 inside that: alone the
  comparison is a call for budgets of 156..201, with `+ DirType` after it 254..345 (the wrapper's body is divided
  by the sites that follow it in the caller).
- With the member hover Motion_AI is exact without ra2-decomp's unit-local `Facing_To` wrapper, anim stays whole
  without its `_Direction(object, to)` wrapper (the member with the object as a parameter), and the comparison
  sites of hover Process, fly Nearing_Target and both statements of Start_Driver are the image's. Start_Driver's
  first `DirType::Direction` was then 4 units short (`(1000 - 58) / 10 = 94` against 98): one inline site too many
  after it. The archive's `Clear_Occupy_Bit(PositionCoord)` reads a property (`Get_Coord()`, an inline site);
  ra2-decomp's twin has `Clear_Occupy_Bit(Coord(Position))`, a plain copy and no site, and the row is exact
  (817 B, infantry whole). Three spellings of the member's body (temporary, named local returned, the named local's
  call returned) give the same lists.
- A property read is an inline site: when a budget is one site out, look for `PositionCoord`-style property reads
  after the disputed call and at the twin's spelling of the same statement.
- hover Process 0x4FBFC0 after the member: the expanded `Start()` still expands `Coord::As_Cell` (50 units) and
  `TPoint2D<short>::operator==` where the image calls both; charged 48-unit pads at the top of the root give
  As_Cell a call at 3 and the image's whole call list at 6, so the image has 240..288 units less there.
- ra2-decomp's reading (the template's `operator==` declared only, defined at the end of six units, `Point2D` and
  `Cell` with their own) is not RA2's: the image holds ordinary out-of-line copies of
  `TPoint2D<short>::operator==` (0x4FEF20), `operator!=` (0x58DB50) and `TPoint3D<int>::operator==` (0x61A8B0), and
  with the declaration alone Start_Driver still calls `TPoint3D<int>(int, int, int)` where the image expands it.
- The second tiny callee of Nearing_Target, 0x4BFB30 (`abs(Facing) >= abs(rvalue.Facing)`), takes `this` in ecx and
  one stack argument: it is a member, as ra2-decomp's face.h has it (`bool operator >= (DirType const & rvalue)
  const`), not the archive's free inline (which /Gr would pass in ecx and edx). The form shows only where the
  image keeps the call.
- Recipe: when a callee of 40..60 units is a call at a function's first statement, list every image site of that
  callee (`rebuild/tools/disasm.py` of each caller) and read what the sites share before the call; a shared
  argument expression is the body of an accessor. Look for it in Red Alert's headers as well as the archive's: the
  archive's callers can be spelled one level flatter than the source was.
- Coord_Scatter 0x491750: the lock branch is ra2-decomp's spelled-out expression in the image (`X - (X & 255) +
  128`), not `Coord_Snap`; eight natural spellings of it leave `TPoint3D<int>(int, int, int)` expanded inside
  `Move_Coord`, so the level ra2-decomp supplies with its pass-through `_Move_Coord` is still unexplained (Red
  Alert's COORD.CPP has an out-of-line `Coord_Move` at that place; the image has none in coord's range).

## Counting sites with empty probes, and a callee too small to refuse (found in the rebuild (lane F13, high))

- Count sites before measuring charge. An empty `static inline void Probe_Site(void) {}` called k times late in
  the root adds k sites and no charge (measurement only, never landed). hover Process 0x4FBFC0: k = 0 expands
  `Coord::As_Cell` inside the expanded `Start()` and calls `Cell(int, int)`; k = 1 calls `As_Cell` and expands the
  cell comparison; k = 2 calls both, the image's list. Lane F11's charged pads (240..288 units at the top) and
  two sites after `Start()` are the same shortfall; the site count is the smaller statement of it.
- One of the two sites was a conversion the siblings spell out: `Point2D(a).Distance_To(Point2D(b))` (jumpjet,
  aircraft, object, techno, missile) against the archive's `Distance_To(HeadToCoord)` in hover. An explicit
  temporary of the argument's own class is an inline site; an implicit base-class reference is none.
- A wrong virtual hides in a draft that is far off for other reasons: hover Process called `Docking_Coord`
  (slot 0xA4) where the image calls slot 0x4C, `AbstractClass::Destination_Coord(TechnoClass *)`. Compare the
  slot offsets of every virtual call in the diff before reading register differences.
- A two-coordinate level changes operand order where no flat spelling does. hover Process reads `HeadToCoord.Y`
  into ebx before the position's temporary and `HeadToCoord.X` after it; twelve flat spellings of the 2D
  distance do not, and a probe `(Coord const & from, Coord const & to) {return(Point2D(from).Distance_To(
  Point2D(to)));}` gives the site exactly. No reference has such a function (main coined one per unit, in hover
  and in fly); the row stays parked.
- An X-and-Y-only copy of a coordinate before `Facing.Current()`, with only X and Y stored back, is a 2D twin of
  `Move_Coord` over `Point2D` (hover Process; frame 0x4C and the block order come right with a probe twin). Not
  in the archive, ra2-decomp or mischa85.
- A callee of 40 units or less is never refused, so an out-of-line call to one is a statement about its body.
  0x4BFB30 (`abs(Facing) >= abs(rvalue.Facing)`, 36 B) has one caller in the image, fly Nearing_Target, and sits
  among the refused inline copies at the end of fly's object. il_lab (fly): `DirType::operator>=` with the
  references' one-line body is free (expanded at every budget, also inside `Is_Complete_Turn`, 68 units, beside
  `operator-`, about 52), so neither pads nor sites can make it a call. Bodies with the same meaning and more
  nodes (`if (...) return(true); return(false);`, two named ints) are charged, and then three more sites after
  the call give the image's call. The original's body was therefore wordier than both references'; which
  wording is not evidenced, so nothing landed.
- To tell a free callee from a charged one with `il_lab.py`: put the callee's caller in the spec (here
  `Lab_B = Lab_D1.Is_Complete_Turn(Lab_D2, Lab_D3);`) and read the range just above the caller's own size; a
  callee that never appears as a call there is free.
- Pads that flip nothing mean the root is not the level that refuses. particle Gas_Behavior_AI 0x608BB0: the
  image calls `Coord(int, int, int)` twice, `Vector3::operator+=`, `Vector3(float, float, float)` and
  `Vector3::operator*=`. Charged pads at the top of the root: up to 1092 units flip nothing, 1812 gives the
  image's calls inside `Slope_Vector` with the three root sites still expanded, 2292 refuses `Slope_Vector`
  itself. No pad refuses three small root sites while a large callee decided after them still expands.
- The same statements in two functions with the same refused callees are one inline function. Spark_Behavior_AI
  0x609540 holds Gas_Behavior_AI's RA2 block word for word (`old_coord`, `position += velocity`, `coord2`, the
  bridge and blocked tests) and the image makes the same out-of-line calls in both; Railgun_Behavior_AI, which
  has `Coord pos(...)` and `PrecisePosition += velocity` at its root, expands both. A probe level around the block (results by reference) makes
  `TPoint3D<int>(int, int, int)` a call inside both `Coord` constructors, and with 8 to 16 empty sites after it
  `operator+=`, `operator*=` and the second `Coord` constructor are calls: the image's family, not yet its list.
  The archive's text is flat in both functions, so the level is RA2's own and has no reference name. When a
  parked function's budget cannot be explained at the root, search the unit for a second function with the same
  statements before measuring further.
- Scratch compiles through `tailmap.compile_text` share one source path per unit; run variants of one unit one
  after another, never in parallel (parallel runs silently score each other's text).
- Probe sites must stand after the disputed call to count (N runs from the call to the end); before it only a
  charged pad moves anything. mapgen Create_Region_From_Cell 0x56E100: empty sites at the top change nothing,
  a pad of 48 units makes `TPoint2D<short>::operator!=` a call and one of 144 gives the image's whole call list
  (`VectorClass<Cell>::Clear` from the destructor at both returns), 300 overshoots: the image has about
  100..190 units more charged before the returns. A pad sweep is not monotonic past its window, so step it
  finely (48 units) before reading a bracket.
- Fewer free sites late can mean less budget later: the expansions between them and the end divide by a
  smaller N, take more, and leave less for the last sites. Create_Region_From_Cell's pop with the count read
  as a member (`cells[cells.ActiveCount - 1]`, probe only: the member is protected) instead of through
  `Count()` gives the image's loop head and makes `operator!=` near the end a call (1024 -> 555 bytes off).
  A shared index where the image computes `count - 1` twice (`[ecx+edx*4-4]`, then `lea eax, [edx-1]`) is the
  sign that the image did not go through the accessor.

## A family of small members is one coupled budget system: adopt the twin's forms for all of it, and read free limits before reshaping (found in the rebuild (lane F15, high))

- Found in the rebuild (lane F15, high): a row that is only expansions of small members (planmgr
  Planning_Draw_Remembered 0x618090: a colour, `Frame_Lead_Box`, `Frame_Lead_Box_Outer`) is decided by the IL
  sizes of the whole chain under it, and every member of that chain is also a root or a site of other exact rows.
  `rebuild/tools/il_lab.py` over the chain gives the sizes in one compile (Planning_Pixel 56, both frames 96,
  Lead_Box_Rect 62, Lead_Box_Rect2 54, Lead_Rect 72, Lead_Pixel 60, Lead_Coord 58, the centered rectangle 82,
  Planning_Inflate 112, Planning_Coord_Pixel 68); with them the rule of the "Nested inline sites" section predicts
  every threshold il_lab prints: the first frame expands both inflates iff its body gets
  Lead_Box_Rect + 2 * (Lead_Rect + Planning_Inflate) = 430, and it got (1000 - 56 - 96) / 2 = 424.
- The standalone rows hold each size to a unit or two, so no single form can be moved: Frame_Lead_Box as a root
  refuses the centered rectangle with 80 left against 82, Draw_Lead_Box refuses Planning_Coord_Pixel with 66 left
  against 68. A lighter Lead_Rect or Lead_Box_Rect alone fixes the first frame and breaks two standalone rows.
- ra2-decomp's texts of the same family (YR, all matched there) differ from the tree's in six places, and only
  the six together satisfy every row here: Lead_Coord through `Get_Packet()` (an out-of-line accessor of the unit
  is a site: 58 -> 62), the centered rectangle's `Point2D corner; corner = center;` (82 -> 92), Lead_Rect
  `return Planning_Centered_Rect(pixel, 8);` (72 -> 64), Lead_Box_Rect `Rect rect = Lead_Rect();` (62 -> 54), the
  pixel packer with three `unsigned char` locals for the shift counts (56 -> 76), and a file-local `Shrink_Rect`
  in place of `Planning_Inflate(rect, -1)` in the three lead box members (the same instructions, a site of 96
  against 112). Any five
  leave a row off. When a twin family is matched as a whole, adopt its forms as a whole before measuring a single
  one: the twin's sizes are a consistent solution of the same system.
- A draft that calls a sibling the image expands may be a mapping error, not a budget one: the twin's
  FUN_0063D480 is the filled dot (Planning_Fill_Dot 0x61A210), and the draft of Planning_Draw_Filled_Techno
  0x618C00 called the outline dot at eight sites. With the right callee, the twin's `RGB_Pixel` wrapper (one more
  level over the three-int Build_Hicolor_Pixel keeps 0x432D90 a call) and the twin's Fill_Dot text (68 units;
  main's alias locals made it 98) 51 of 53 direct calls agree.
- Measuring the rest with a charged spend at the top of the root (an inline of 12 + 6k units of global array
  stores; `rebuild/tools/inline_probe.py --source` aligns the calls): spends of 40 or less are free and only raise
  the root by the call statement, so the first usable point is 42. Here 42 reproduces every call and 48 loses the
  late `Reached(0)` (44 units): the text is 16 to 34 units richer than the image before the fifth dot. Sizes just
  over the free limit (Has_Planned_Nodes 44, Owner_Is_Mission 42, Should_Draw_Active_Action 46, Reached 44) are
  where a one-unit difference in a header (a member's type, a cast) changes a whole charge; list them first.
- A dword zeroed at entry and never read again is a temporary-construction flag whose condition folded
  (wdtsel Selection::Select_Territory_At 0x72E650): `c ? f() : T()` builds f's result in a temporary,
  copy-constructs the destination and destroys the temporary, and keeps the flag's entry store when c is
  constant. No spelling of a plain initialisation gives the copy (direct, functional cast, reference then copy,
  declare then assign: 174 to 180 bytes). Here only main's `true ?` reaches it and no independent reference has the
  condition, so the row and its dependent Handle_Click 0x72E430 are on the variation list; look for an inline
  predicate or a parameter with a constant argument before calling such a flag unreachable.
- A draft that spells out a test the unit has a member for loses the member's locals: map
  Pick_Random_Tile_Variant 0x55D640 was one register swap from the image with the diamond test written as four
  comparisons, and exact with the matched twin's `Map.In_Radar(cell)` (expanded; the member reads the cell into
  x and y first). The twin's other forms with the test spelled out stayed at 694 bytes.

## A budget a few units long can sit in how a small struct is copied; depth 9 and the wlib lists (found in the rebuild (lane F16, high))

- Found in the rebuild (lane F16, high): planmgr Planning_Draw_Filled_Techno 0x618C00 was two call decisions from
  the image with the matched twin's text (lane F15). Before changing a callee, test whether the body is already
  right: make the disputed callee free with a probe form (here `Reached` as one `&&` expression; its own row then
  differs, so the form is not its text) and give the other the size the window asks for. Every instruction then
  agreed except the expanded test itself, so only the budget was open and no statement had to move.
- Map the window with probe stores inside a callee that is expanded once before the disputed sites (scalar
  global stores, 4 units each, in `Has_Planned_Nodes`): 24 units and up give the image's calls, 16 to 20 flip a
  third site (`Lead_Pixel`) first. A top-of-root spend cannot reach under 42 (40 or less is free), a callee's
  body can.
- The root's own size counts twice when 2 * S is over 1000: a root 12 units smaller is 24 units of budget. The
  twin copied the path start field by field (`Point2D source; source.X = center.X; source.Y = center.Y;`);
  declared and assigned whole (`Point2D source; source = center;`) is the same instructions, a smaller root and
  the image's call list: exact (2,076 B). Other spellings of the same copy are each a different result:
  `Point2D source = center;` leaves `Reached` a call and `Point2D source(center.X, center.Y);` expands
  `Lead_Pixel`. When a budget is a few units long and the callees are the twin's, list the root's struct copies
  and coordinate locals before the callees.
- Declared-then-assigned rectangles cost 8 to 10 units each over initialised ones (Planning_Fill_Dot 68, 76,
  86), with the same bytes for the function's own row; a sibling's form (Planning_Draw_Dot is declared then
  assigned, 78) is not evidence for the other (Fill_Dot at 76 or 86 breaks the filled renderer).
- wlib's `ArrayList` in this tree is the archive's text unchanged (mischa85's is the same file, ra2-decomp uses
  the archive's), and both references' `Dictionary` takes keys as `K &`; the tree's `K const &` exists for
  ownrdraw's `unsigned int` keys, where ra2-decomp keys all three tables by `HWND` with one hash function (the
  image has one, 0x5DE970, bound here under three names). With `K &` wonline's lookups compile and decide the
  same, so the const is not what expands `keyHash` in CChatEventSink::OnFind 0x762F70.
- `growVector` is 170 units and `shrinkVector` 174 (il_lab, wonline, `#define private public` in the spec's
  `pre`): both just under the auto-inline limit, so they expand wherever the budget allows and a root a few call
  statements larger flips them (OnFind expands `shrinkVector` with three more call statements).
  `#pragma auto_inline(off)` around a template member's definition in a header does nothing (the members are
  instantiated where the pragma no longer holds); to probe "never expanded", change the member's size instead.
- Tell a frame miss from an inline miss before working a "calls agree" row: disassemble both, replace every
  `[esp + n]` and the frame size by one token and compare. planmgr Planning_Draw_Techno 0x6182F0 (1,665 bytes
  counted) is then the image instruction for instruction; all of the difference is displacement sizes from a
  frame of 0x178 against 0x174. Its three flags show the shape: the image keeps has_nodes and the attack-move
  flag in one dword (0x12, 0x13), ours gives each its own. Where a declaration stands (function top, block, or
  apart from its assignment) does not move them, and every statement form that changes the frame here does it by
  changing which members expand.
- Wonline_Main_Dialog_Proc 0x7469C0: the image expands the placement new of `ArrayList<int>::add` and calls
  `growVector` twice beside it at four sites. Under main's helper chain the placement new is at depth 9 (a call,
  as "Inline depth limit" says); one helper level less, in eight combinations, expands the placement new and
  `growVector` with it, the budget there being ample. So the image has the two `growVector` calls one level below
  the placement new of the same `add`, which the archive's `add` cannot give and no reference has (main's
  `Make_Room`): the row is on the variation list.

## Look inside the expanded callee before the root; a doubled property read; a second Length site sized by root pads (found in the rebuild (lane F17, high))

- A call difference reported at an offset of the root can lie inside a callee the root expands. BuildingLightClass::AI
  0x433970 showed `TPoint3D<int>(int, int, int)` 0x41B630 called and Length expanded at +0x503; that offset is not the
  follow test but `Detection_Radius` -> `Sweep_Stage`, both expanded there. Sweep_Stage read the distance as
  `Center_Coord().Distance_To(Owner->Center_Coord())`; ra2-decomp's twin and the archive's object.h have
  `Distance_To(Owner)` (`ObjectClass::Distance_To` -> `Distance(Coord const &)` -> `TPoint3D::Distance_To`). The two
  levels leave Sweep_Stage 0x434610 and Detection_Radius 0x434530 the same bytes on their own rows and leave the
  constructor a call three levels down in AI: exact (1,818 B) with AI's text untouched. When a parked root expands
  small functions of its own unit that are exact rows, compare their texts with the twin's first: a form that is
  neutral for the callee's own row is not neutral for its expansion.
- A register that holds 0 through a function (`xor ebp, ebp` early, `cmp eax, ebp` for `test eax, eax`) is what is left
  when another value has lost that register. In FootClass::AI 0x4C8FF0 the image keeps `&Locomotion` in ebx and has
  no zero register; the draft had no spare register because `TClass->MoveSound[random % (unsigned)TClass->
  MoveSound.Count()]` reads the `TClass` property twice (two virtual `Techno_Type_Class` calls, the image has one).
  The archive's `TypeList<T>::Pick(int index)` (typelist.h) takes the list once: `TClass->MoveSound.Pick(
  NonCriticalRandomNumber())` is exact (2,184 B). Count indirect calls too: a "calls identical" verdict made from
  direct calls alone missed the extra `call [eax + 0x80]`. A property written twice in one expression is two calls;
  look for the member that takes the object once (`Pick`, `Random_Pick`) before a helper.
- BuildingClass::Take_Damage 0x43EE70: the frame (0x98 for 0x8C) was two spellings. The archive sums the occupy
  offset first, `Cell(*offset++) + Get_Cell()` (the image copies `*offset` before the virtual call), and both
  distances go through `Distance_To(contact)`: the image's two sites share their temporaries, which only the same
  member chain at both gives (16 combinations of `Distance_To`, `Distance(Coord)`, `::Distance(a, b)` and
  `Coord::Distance_To` compiled; the others add one or two Coord temporaries to the frame). One site is left: the
  image expands Length at the second site and calls it at the first. K out-of-line call statements before the
  final return flip the second site alone between K=32 and K=36, and the function is then the image's, the
  expanded Length converting Y, Z, X as the image does (lane F2's note: the order follows the function's other
  expansions, here it follows the right call list). A flip that needs more than thirty call statements reads either
  as 264 to 288 units of root budget or as the root crossing the 1000 floor only there (2 * S passes 1000 near
  K=33). Three more probes tell them apart: the site does not answer to what the root spent before it (it stays
  a call without the first distance, without `contacts.Add` and without `Delete_All`), and with one inline site
  less after it (`Coord(Position)` for the `Get_Coord()` property at the sound, the same bytes) the flip moves to
  K=17..24. So the root is above the floor, a site after the disputed one is worth 12 to 16 call statements, and
  the image is about three sites lighter after the second distance or 132 to 144 units larger at the root. Pads
  at the end of the root and a site count after the disputed call, taken together, give both numbers in six
  compiles; `Scen->RandomNumber` in place of an expanded `Random_Pick` is told from it by the argument order
  (the expanded helper's call comes before the outer call's pushes).
- `if (x < 0 || x > 3) x = 0;` on an int and the archive's `switch` over the four values with a default compile to
  the same object here (the signed pair `jl` / `jle`), and the same inline decisions: reinf's `_Create_Paradrop` has
  the archive's Create_Air_Reinforcement as its ancestor (reinf.cpp:841), whose switch is the form main's text has,
  and it does not move `Coord(Cell const &, int)` 0x61EA80 in Do_Reinforcements 0x639970. Out-of-line call
  statements inside the paradrop (4 units each) leave the constructor a call from ten on and not at nine: the
  original's paradrop body is 37 to 40 units larger than ours, one size class, not a cast or two.

## A block two functions share may be another unit's inline; a spelled-out one-liner against a nested one; a named count against two Count() sites (found in the rebuild (lane F18, high))

- Pull the references before fitting a level by measurement. The level lane F13 measured in particle
  Gas_Behavior_AI 0x608BB0 is `Ballistic_Move(Vector3 & position, Vector3 & velocity, double gravity, double
  elasticity, double max_velocity)`, ra2-decomp's `include/ballistic.h` (their commit f5e9025, 2026-11-21): the
  move of BounceClass::AI, shared by bounce and particle. With it Gas_Behavior_AI is exact at the first compile
  (`Ballistic_Move(position, velocity, Rule->Gravity, 1.0, 0.0)`), the image's five calls included, and the
  archive's `Slope_Vector` has no user left in RA2.
- The out-of-line copies after a unit's last function name the inlines it was first to refuse. After
  BounceClass::AI 0x4370E0 the image has `Vector3(float, float, float)` 0x437690, `operator*=` 0x4376B0,
  `operator+=` 0x4376E0 and `Length` 0x437710: bounce's object emits all four, and they are the calls particle
  makes. A parked function that calls such copies shares an inline with the unit that owns them.
- A nested one-liner and its spelled-out twin differ at a refused site. `Vector3::Length` as the archive has it
  (`sqrt(Length2())`) is expanded at the clamp in BounceClass::AI with `Length2` refused inside it (a call the
  image does not have); as ra2-decomp's vector3.h has it (`sqrt(X*X + Y*Y + Z*Z)`) the site is refused whole
  and calls the 45-byte copy, the image's call. An outer level expanded with its inner level called, where the
  image calls the outer, means the outer has no inner level. All 140 includers of vector3.h keep their rows.
- An unnamed temporary takes a slot an expanded inline left free; a named local never does. Spark_Behavior_AI
  0x609540 with the archive's `Point3D new_position(...)` has a frame of 0x130; with `Set_Coord(Coord(position.X,
  position.Y, position.Z))` the Coord sits in the slot of the helper's `previous_coord` (frame 0x124, the
  image's) and its base constructor is the image's call to `TPoint3D<int>(int, int, int)` 0x41B630 (the Coord
  constructor expanded, the base refused). What is left there is slot order and registers only (694 bytes; the
  image has `ground_height` above `velocity` and `bridge_height` in ebx), and no natural spelling of the root's
  five statements moved it; the row is not sensitive to declaration counts either.
- A named count is not two `Count()` sites. mapgen Create_Region_From_Cell 0x56E100: `int count = cells.Count();
  Cell cell = cells[count - 1]; cells.Delete_Index(count - 1);` gives what lane F13's probe with the protected
  member gave (`[ecx+edx*4-4]`, then `count - 1` computed again; `operator!=` a call; 1024 -> 555 bytes), so no
  accessor or friend is behind that sign. Create_Zone_Region 0x575D20 in the same file, exact with `Count()`
  written twice, has the shared index in the image: a shared index means two accessor sites, an index folded
  into the address and computed again means one named value.
- A lighter twin form can move a draft away from the image's call list while it lowers the byte count.
  ra2-decomp's Recount_Cells (`Cell empty_cell; empty_cell.X = 0; empty_cell.Y = 0;`) with the named count
  reads 538 against 555, but expands `operator!=` where the image calls it; the for-statement form of the count
  does the same. Read the call list before the count.
- A sweep of every draft parked only for lack of a bound is cheap (64 guards, one compile each, under three
  minutes): open the guard alone, compare over the frozen pass's row. None was exact on 2026-11-22; the two
  closest are map 0x55DEF0 and 0x55E410 (5 bytes each at the pass's size). Eight pass rows do not hold the
  whole function (a 61-byte row before a 3,955-byte gap at mapgen 0x56EF20): those need another bound, the
  frozen-bound precedent does not reach them.
- `pgrep -f NAME` in a wait loop matches the loop's own shell (its command line holds NAME); write the pattern
  as `[N]AME`.

## A frame miss with every call agreeing is a callee's block scope; a by-value helper parameter is a second copy; site counts from empty probes (found in the rebuild (lane F20, high))

Found in the rebuild (lane F20, high):

- Map a "calls agree, frame differs" row slot by slot before changing text. Compile the scratch unit with
  `/FAs /Fa<path>`: every frame reference in the listing reads `_name$[esp+K]`, K being the frame size plus the
  push depth, so the listing gives each object's name and the depth at every instruction. Pair the listing with
  the image instruction for instruction (the mnemonics are equal when the calls agree) and subtract the depth
  from the image's `[esp + n]`: the result is a table of our objects with our slot and the image's slot, the
  reference count and the source lines of each (lane tool: build/r/f20/fm.py, 100 lines).
- planmgr Planning_Draw_Techno 0x6182F0 (frame 0x178 against 0x174): the table showed has_nodes and the
  attack-move flag at 0x12 and 0x13 in the image because the 16-byte slot above them holds 47 references
  (2.94 a byte, under the flag's 3) where ours held 49 (3.06, so the flag sorted above the slot and took a
  dword). Frames over 128 bytes sort slots by references per byte, and two byte objects share a dword only when
  they are neighbours in that order: a one-dword frame difference can be two references in a 16-byte slot.
- Which objects share a slot is liveness, and for the locals of an expanded callee the scope they had in the
  callee decides it. A callee written `if (c) { T a; ... }` gives its locals the block's life; written
  `if (!c) return; T a; ...` they are locals of the whole callee and, expanded, live across the caller's
  enclosing block: they then conflict with the caller's long-lived values and take slots of their own, while a
  return-slot temporary of the same expansion (statement life) still shares. Both forms are the same bytes for
  the callee's own row, so the callee's row cannot choose between them; only a function that expands it can.
  Planning_Draw_Midpoint 0x61A2F0 with the early return gives Planning_Draw_Techno the image's slot for every
  object below its tail. (Scout739 noted the same move in the main tree; the slot table is what finds the
  callee.) Owner ruling, 2026-11-22: ra2-decomp's twin of the helper writes the block, so the early return is a
  form against the twin and the row is held on variation_candidates.csv, whatever the caller's frame says.
- The early return is 2 units smaller than the block (118 against 120, il_lab), and two expansions of it freed
  enough for PlanningNodeClass::Get_Event_For to expand at the tail (frame 0x104). The answer was not a wordier
  helper but the tail's own form: the image has a second 0x70-byte EventClass copy there, which main's text
  (`EventClass event = node->Get_Event_For(techno); if (event.Get_Event_Frame() != -1)`) only reproduces while
  Get_Event_For is refused. The sibling renderer's line from ra2-decomp's twin,
  `int frame = Planned_Waypoint_Frame(node->Get_Event_For(techno));` with the helper taking the order by value,
  makes the copy the helper's parameter and is exact over the early return (the tail lines themselves would
  land flagged; the early return under them is what holds the row). An object the image keeps that the text
  gets only through a refused call is evidence for a by-value parameter at that place; look at what the sibling
  function's twin writes there before sizing anything. A split statement in the helper (`Rect view; view =
  ...;`, 8 to 10 units) also gave the image's bytes and was refused first: against the twin's text of the
  helper and chosen for the budget (the Reposition_Sidebar precedent); a sibling's split form in our own tree
  is not evidence. Three exact routes are on the variation row; none is the twin's helper text.
- A constructor's member initialisers are inline sites decided before the body, and a nested site in one of
  them gets the budget divided by every candidate site from there to the end of the body. wdtsel
  Selection::Selection 0x72D560 keeps `VectorClass<MSSfxEntry *>(int, T const *)` 0x7304C0 out of line inside
  the expanded DynamicVectorClass constructor of its last member: empty inline probe calls before the last
  statement flip it at three or four (one, two and six or more do not), so the image's body has three or four
  more inline candidate sites than the adopted text. ra2-decomp's file-static
  `Load_Selection_Text(label, line)` (its wdtsel.cpp:232, used by its matched Select_Territory) at the two
  string sites supplies them: with it, and Map::Load's third parameter as `bool`, the constructor is the image
  but for the binding of 0x7304C0. Select_Territory 0x72ED80 stays exact with the helper at its four tooltip
  sites, as the twin has it. Landed flagged (owner ruling, 2026-11-22): the helper is a twin unit-local helper
  at Select_Territory (DECISIONS row 85, twin_helpers.csv); its two uses in the constructor have no twin text
  and carry the judgment flag. When a count of missing sites comes out of empty probes, look for a file-static
  helper in the twin's unit before any accessor: main reached this row through two coined accessors and its
  own vector type.
- An unmasked `push eax` of a `bool` result into a parameter is a `bool` parameter; `and eax, 0xff` before the
  push means ours declares it `int`. Where the callee never reads the parameter its own row cannot tell (Map::Load
  0x729510): the caller's push is the only evidence.
- wonline Buddy_Dialog_Proc 0x758750 (1,728 bytes, two sites: `remove(Wstring &, int)` called where the image
  expands it into `get` and `remove(int)`, four levels down through Refresh_Buddy_Status,
  Service_Buddy_Find_Queue and removeHead): out-of-line probe calls at the top of the root (8 units of root
  budget each) do not find a window. One, two: no change; four: seven other sites move; eight: one of the two
  sites expands and nothing else moves; sixteen and more: the ladder block and four more sites move. The two
  sites sit at different shares of one budget, so a single root spend cannot answer both; the next thing to
  measure is each expanded Refresh_Buddy_Status copy on its own (N_i at the timer case and at the refresh
  button).

## A twin text newer than the dossier is the first thing to try; missing nested calls are the root's other inline sites (found in the rebuild (lane F24, high))

Found in the rebuild (lane F24, high):

- Before working a parked row from its dossier, look at the reference checkouts' history for the function
  (`git log -L<start>,<end>:src/<unit>.cpp` in private/reference/ra2-decomp, or its "Record the Nth set" commits):
  ra2-decomp lands matched Yuri's Revenge twins daily, and a dossier or twin table written before such a commit
  still ranks hand-made leads. map MapClass::Reset_Subzone 0x563D80 (1,287 bytes) and techno
  TechnoClass::Draw_Link_Line 0x6D0990 (1,059 bytes) were exact on the first compile of twin texts two days old.
- Reset_Subzone shows what "the image calls a nested constructor, an operator= and a Clear that ours expands"
  means when the statement itself is right. `track->Add(SubzoneTrackingStruct())` is the same in the archive's
  text and in the twin's; under the archive's text VC6 expands the struct's constructor down to
  VectorClass's, the struct's operator= and the temporary's Clear, and under the twin's it keeps
  DynamicVectorClass<SubzoneConnectionStruct>(unsigned, T const *), SubzoneTrackingStruct::operator= and
  VectorClass::Clear as calls, as the image does. The twin differs only elsewhere in the root: `set->Clear()` for
  the open-coded bucket loop, `Rect bounds(0, 0, w, w)`, `Cell(X, Y)` at two uses, `Cell_Region(Cell(X, Y))`, a
  bucket reference, `SubzoneConnectionStruct conn(id)`. A lead that says "write the assignment through the outer
  type" is then the wrong edit: count the root's accessor and constructor sites against the twin first.
- A row on the variation list can still have an ordinary route once its twin is matched: map MapClass::Zone_Span
  0x54F250 (1,386 bytes; main reached it only through a threaded Add specialisation with a dummy flag) is exact with
  the twin's text, which fills one `ObjectType connection` local by hand and calls the two-argument
  `Add_Object(bucket_index, object)`. Check the twin's history before treating a variation row as closed.
- Vector3::Cross_Product's two forms differ in budget, not only in code: in techno Railgun_Beam_Damage 0x6D6ED0 the
  three-argument form (`Vector3 cross; Vector3::Cross_Product(off, delta, &cross);`) gives the image's
  floating-point order where the by-value form leaves 19 bytes, and it also frees enough of the root's budget that
  the local vector's destructor at the return is expanded where the image calls it (0x40CCB0). Two things were
  being traded against each other in one statement; the row needs the three-argument form plus the charged sites
  the original has elsewhere.
- Sizing a missing-site count with discarded accessor statements: in Railgun_Beam_Damage, with the twin's whole text,
  one or two extra `coord.As_Cell();` statements ahead of the loop (scratch probes, never a landing form) make the
  function exact, and three or more break it again: the window is two sites wide, so the original has one or two
  more charged sites than the twin's Yuri's Revenge text. A member-returning accessor does not count (an if/else
  over Cell_Bridge_Occupier() and Cell_Occupier() in place of Cell_Occupier(bool) changes nothing), and an extra
  level inside an already expanded helper did not count here either (Percent_Chance written through Random_Pick,
  at either site or both). Probe with a callee that computes something, and expect the window to be narrow.
## Two sites that answer a root spend in the wrong order; a spent root that still expands a distance; bind a parked draft's template copies first (found in the rebuild (lane F23, high))

- When two sites of the same nested callee differ from the image, probe the root before choosing forms, and read
  the order in which they flip. wdtsel Do_Target_Anim 0x72E780: the image expands `~CDFileClass` inside the first
  `CCFileClass file` destructor (+0x285) and calls it (0x51AD10) at the second (+0x38F); ours calls it at both.
  Out-of-line call statements at the top (8 units of root budget each): one, two: no change; four: the second
  site expands and the first stays a call; eight: both expand. The later site has fewer candidate sites after
  it, so it gets the larger share and flips first: no root spend gives "first expanded, second called". That
  pattern needs more charge between the two sites or more candidate sites after the second, so look at the
  callees expanded between them and at the statements after the second, not at the function's total. Opening
  the first statement's inline (`Remove_Target_Anims()` written out as the archive has it) overshoots at once:
  the teardown's cursor constructor and `operator->` expand where the image calls them, so that level is right.
  Measured afterwards: the pattern is two fewer candidate sites between the destructors and a slightly wordier
  root. With the second file's name read without its inline accessor at both places (main's direct member read;
  the member is private here, probed with access opened) one to six pad statements give the image's pair and
  none does not; the archive's `if (anim != NULL) { delete anim; }` in place of the pads leaves 2 bytes, the X
  operands of the target sum. One accessor site less (a named reference) is not enough, and passing `target.X`
  directly together with the member `Target` makes the root less wordy and loses the pair again. A redundant
  test the archive writes before a `delete` changes no code and still counts in the root's size.
- A root that refuses small charged members can still show a fully expanded distance later. BulletClass::AI
  0x45F710: from the ballistic branch on the image expands Coord's constructor but calls TPoint3D's inside it
  (+0xCE2), calls `TVelocity3D::operator+=` (0x4642A0), the next `Coord(int, int, int)` whole (0x434820) and
  Vector3's constructor and `operator*=`; the two distances after that (+0x1212, +0x163A) are expanded
  completely, differences stored member by member with no `operator-` or constructor call. By the decision rule
  a spent root refuses `operator-` inside `Distance_To`, so those two are not the archive's `Distance_To`
  chain: they are written out in the body or go through a helper whose pieces are all free (HouseClass::
  Where_To_Place_Building's cloak-generator distance has the same shape). Size such a site by what the root
  refused just before it before restoring the archive's accessor there.
- The archive's accessor level at an early site can be right and still score worse until the spend before it
  is right. The same function's rate-of-turn test with `Center_Coord().Distance_To(target_coord)` (archive
  bullet.cpp:457) turns the site from all-expanded into Length called and the constructor expanded; the image
  has the constructor called and Length expanded, which the same text gives once nine `TPoint3D<int>(0, 0, 0);`
  pads stand at the top (one, two, four and six: no change). So the text before the test is seven to nine
  constructor expansions cheaper than the image's; the accessor goes back in together with whatever supplies
  that, not alone.
- Before reading a parked draft's byte count, bind the template copies only it calls. With its guard open the
  unit's object emits them; where a copy is byte-identical to the bound row at the address the image calls
  (wdtsel: `VectorClass<MSAnim *>(int, T const *)` 0x730470 and `VectorCursor<MSSfxEntry *>(V &, int)` 0x730340,
  both rows on main, the first under a seed name), the name goes to symbols.csv with that evidence and the
  draft's remaining difference is code alone (six hunks became three).
- HouseClass::Where_To_Place_Building 0x4F03C0: with main's body in the tree's names the frame size and every
  call agree and the miss is one place in a tie: the sixteen Cell temporaries of the two static tables and the
  `base_cell + offset + scan_offsets[facing]` temporary take the same slots in another order, most of them one
  place off (image 0x80, 0x5C, 0xB4, 0xB0, 0x7C ..; ours 0x80, 0x58, 0xB4, 0xB0, 0x5C ..). Copy-initialised
  Cells and Coords change the frame size and score worse. A shifted tie order with the same slot set points at
  one temporary's reference count or creation place (the Scout680 rule), which a /FAs variable map shows
  directly; spellings of the statements do not reach it.

## A 16-bit global is loaded as a dword only where the compiler knows its place: extern scalars stay 16-bit, extern array elements widen (found in the rebuild (lane F25, high))

Found in the rebuild (lane F25, high):

- Under /G5 VC6 reads an `unsigned short` global with a 32-bit load (`mov eax, dword ptr [g]`, then
  `and r, 0xffff` or a 32-bit `and`) when it may read four bytes there, and with a 16-bit load (`mov ax, word
  ptr [g]`, zero-extended as `xor r, r` / `mov r16, ax`) when it may not. What decides is the declaration the
  unit sees, not the use: (a) an `extern unsigned short g;` scalar is never widened; (b) element 0 of an
  `extern unsigned short g[];` array is widened (element 1, two bytes in, is not); (c) a scalar the unit itself
  defines with an initialiser is widened when it lies at a multiple of four in the unit's data and not when it
  lies two bytes in; (d) defined without an initialiser the first two masks lost seven ownrdraw rows (measured as rows, not
  load by load).
- So the load width of one global across the image tells how it is declared. Survey every reference to the
  address (raw search for the four address bytes, the 0x66 prefix before the opcode): OwnerDraw's three colour
  masks at 0xA763DC / DE / E0 are read as dwords at DC and E0 only inside ownrdraw's own functions (20 sites),
  never at DE, and never in egos (Show_Credits) or srfcache, which only expand the inline Blend_Color. That is
  three scalars defined together with initialisers in ownrdraw.cpp and declared `extern` in the header; the
  tree's two-entry array for the first two (main's form, mischa85's copy of it) reproduced ownrdraw's rows but
  widened the DC load in every other unit that expands Blend_Color.
- egos Show_Credits 0x4B37C0 (2,924 B; main's best 99.54 in ten tries, "frame, register, order"): the 22
  differing instructions were all this one load in the two expanded Blend_Color copies (and the scheduling that
  follows from it). With the three scalars the function is exact as it stood; ownrdraw keeps 229 rows, srfcache
  16, and the other 39 includers of ownrdraw.h do not move. The `(unsigned int)` cast main carried on the red
  mask in Shade_Edges' two halving expressions does nothing and is gone.
- When a parked row's residue is `mov r, dword ptr [g]` + `and r, 0xffff` against a 16-bit load of the same
  address (or the reverse), look at the declaration of g before the expression: array or scalar, defined here or
  extern, initialised or not.
## A product of two calls follows the callees' declaration order; an inline's return form reschedules the caller's other blocks; test a header form on every exact includer before choosing it (found in the rebuild (lane F26, high))

Found in the rebuild (lane F26, high):

- When `f(a) * g(b)` has two calls as operands, VC6 SP3 does not follow the written order: it evaluates the
  callee that was declared later first, stores it, and multiplies the other into it. building
  BuildingClass::Do_MISSION_MISSILE 0x448400: the take-off velocity `TVelocity3D<double>(DirType(DIR_N),
  DirType(DIR_N), 10)` expands `fastmath::sin(yaw) * fastmath::cos(pitch) * magnitude`; with `sin` declared before
  `cos` (the archive's fastmath.h) the object calls cos first, and the Dir256 constructor, which writes the same
  product as `cos(..) * sin(..)`, compiles to the same bytes. With `cos` declared before `sin` (ra2-decomp's
  fastmath.h) both call sin first, as the image does. So a diff that is only "two calls of a commutative
  expression swapped" is settled by the order of the two declarations in their header, not by the expression;
  compare the declaration order with the twin's header before rewriting any statement. All 286 includers of
  fastmath.h were unchanged by the swap, so it costs nothing elsewhere.
- The same class of diff inside an expanded inline (a commuted `fld st(0)` / `fmul [mem]` pair on the first of
  three `X *= ratio` statements) followed the inline's own named values: `T current = Speed(); T scale = speed /
  current;` (ra2-decomp's Set_Speed) against the archive's one `T speed_ratio = speed / Speed();`. Two named
  doubles where the archive has one is a different operand order for the first use only.
- An inline's return form can reschedule a block of the caller that does not contain it. In the same function one
  `push` of the second Create_Bullet's arguments sat on the wrong side of a load (10 bytes); neither argument
  spelling nor the callee's kind (member against the references' free function) moved it. The block also holds
  the expanded `::Direction(Coord, Coord)` a few statements earlier, and that inline's last statement decided it:

  | Direction(Coord, Coord) returns | Do_MISSION_MISSILE | HouseClass::Which_Zone 0x4EAD40 |
  | --- | --- | --- |
  | `return(fastmath::atan2(..));` (archive) | 10 bytes | exact |
  | `double angle = ..; return(DirType(angle));` (ra2-decomp inline.h) | exact | 16 bytes |
  | `double angle = ..; return(angle);` | 6 bytes | 16 bytes |
  | `return(DirType(fastmath::atan2(..)));` (ra2-decomp disklaser.cpp's copy of the body) | exact | exact |

  The conversion written out is what the missile function needs and the named double is what Which_Zone refuses
  (an `fild` of its expanded Distance moves, again in a block before the Direction call). Two lessons: a
  scheduling tie next to an expanded inline is a question about that inline's text, and a header form that fixes
  one function is not known until every exact includer has been checked with it: the twin's own header form
  would have traded a 362-byte exact row for the new one. Probing with one-instruction stores at the head of the
  block did not show a clean count here (each added store moved several instructions), so the table above, not a
  store count, is the measurement.
- Seen while probing, cause not looked into: a scratch copy of the inline placed late in building.cpp refused
  `return(DirType(x));` ("cast to function type is illegal") and took `return DirType(x);`; the parenthesised
  form compiles at the header's own place.

## A destructor called at an early return and expanded at the last one is the site count, not the spend; the missing site can be a cast helper (found in the rebuild (lane F27, high))

Found in the rebuild (lane F27, high):

- TechnoClass::Railgun_Beam_Damage 0x6D6ED0 (1,521 B) holds a DynamicVectorClass local. The image calls
  VectorClass<ObjectClass *>::Clear 0x40CCB0 at the early return inside the loop and expands the same Clear
  at the last return. Both destructors are free and identical, so the spend up to each cannot explain it
  alone: a nested body is decided with (root budget left) / N, N the inline sites from that one to the end
  of the root, itself included. At the last return N is 1; at the early return N counts every site of the
  function's tail. "Called early, expanded late" therefore reads as a budget that is just under
  N x size(Clear) at the early return.
- Two probes tell the two readings apart in one compile each (scratch only, never landing forms): an empty
  inline function called once after the loop (one more site, no spend) and a few constant stores added
  inside a charged helper ahead of the return (spend, no site). Here both flipped the call: one site after
  the early return, or 1 to 4 IL units before it (stores of 4 and 6 units inside the length helper). Lane
  F24's reading "one or two charged As_Cell sites short" was the same window from its far end; the near
  end is four units, so a row like this is close to any small header difference.
- The missing site was the fallback target's cast. The archive writes `dynamic_cast<ObjectClass *>(abstract)`
  there; RA2 has the object flag test, which ra2-decomp's twin text writes out
  (`abstract != NULL && (abstract->AbstractFlags & 2) ? static_cast<ObjectClass *>(abstract) : NULL`) and
  which is, word for word, the body ra2-decomp's object.h gives the archive's As_Object(AbstractClass *).
  `ObjectClass *obj = As_Object(abstract);` is one inline site, expands to the same bytes, and makes the
  function exact. Recipe (lane F6's, for casts): when a trailing-site probe gives one site, look for a
  spelled-out null-and-flag test after the disputed nest and for the reference header's inline whose whole
  body is that expression (cast.h As_Techno / As_Foot, object.h As_Object).
- Before reading a spend off a helper's spelling, check that the helper is charged at all: a callee of 40
  units or less is free and its size never reaches the root (the twin's Is_Techno spelling changes nothing
  here, lane F24's probe); only charged callees and the site count move this kind of row.
- A charged spend that leaves no code: `inline void Pad(int x) {if (x) {a[0]=0; a[1]=0; a[2]=0; a[3]=0; a[4]=0;}}`
  called as `Pad(0);` is charged its whole IL size (it is over 40 units) and folds to nothing, so a probed
  function can come out exact and the rest of its diff can be read. MapRegionClass::Create_Region_From_Cell
  0x56E100: two or three such spends leave the last return's Clear expanded, four bring a Cell constructor
  out of line; the image's spend lies in a window about 40 units wide between them. At a last return the
  divisor is 1: a destructor called there is always spend, never sites.
- "The first of eleven like statements calls, the rest expand" (MapGeneratorClass::Read_RMG_INI 0x579960,
  `list = ini->Get_IntList(section, name, list);`) is the same division read from the other end: each
  statement's nested operator= gets root / (sites left), which grows as the sites run out. Four to seven
  more call statements in the root (8 budget units each, the root being over the 1000 floor) reproduce every
  decision of the image; three or eight do not. A row like that is a root-length row.
- The floating-point order of the same function (the square root's argument reserved after the third
  product) needs the three-argument Vector3::Cross_Product into declared vectors and the twin's sum order
  y * y + (x * x + z * z); the by-value cross products give the other order whatever the helper says, and
  the tree's x, z, y sum leaves two bytes (two double slots exchanged).

## An argument held across a later argument's call was computed in an earlier statement; of two member stores before a call the first is the one deferred (found in the rebuild (lane F28, high))

- Arguments are evaluated last first and each result is pushed at once: `f(fmt, g(a), g(a))` compiles to call,
  push, call, push. Where the image has call, `mov ebp, eax`, call, `push eax`, `push ebp`, the value pushed
  second was computed before the argument list began: a named value in an earlier statement. Wait_For_Players_To_Load
  0x65C520 (676 B): the first name's conversion named (`char const * narrow = Wide_To_Narrow(name);`) takes the
  parked draft from 201 to 77 bytes, and the drop loop's entry test turns from `cmp eax, 1` into the image's
  `cmp eax, ebx` (the slot's register) with it. Its comma step is in the image's order too: `inc ebx`, `inc esi`
  is `slot++, i++`.
- Two member assignments to a local whose address the next call takes (`probe.X = a; probe.Y = b; f(probe, true)`):
  when both values are loads, VC6 stores the second assignment's member before the argument pushes and defers the
  first one's store until after them. So the member stored after the pushes was assigned first in the source.
  MapClass::Subzone_Span 0x564290: the right-hand probes store X early and Y late, which is `probe.Y = y;
  probe.X = end_x;`; the archive's `probe = Cell(x, y)` is a temporary plus a dword copy the image does not have.
- A register variable that has a home slot is reloaded from it after each block that borrows its register
  (Subzone_Span: seed in ebp, the scan position in edi; the reload sits at the join, or at a loop's latch when
  the loop body borrows the register and the loop top reads the variable). One store after the loop that changes
  the variable means one variable: a second store of the same register there is a second variable holding a copy
  (the archive's `begin_x = x`).
- A relocation-masked instruction diff (unbound fields given a marker base, absolute addresses folded, jumps as
  labels) reads the register and layout residue of an unbound parked draft before any binding work; byte counts
  of such drafts mostly count relocation fields.

## Diagnostic count sweeps (flipsweep), a plain function the caller will not take whatever its pool, and a twin's bool return that moves the frame (found in the rebuild (lane O47, high))

- `rebuild/tools/flipsweep.py UNIT ENTRY --func TEXT --kind local|store|call|expr|stmt --at start|end|before:RE|after:RE
  --max K` (idea from ra2-decomp's tools/flipsweep.py, commit 3f4505e, GPL; rewritten for rebuild.py) adds 0..K copies
  of one diagnostic statement to a scratch copy (guards around the function opened) and prints the k at which the set
  of differing lines changes. A flip is a target for natural source, never a fix; the probes never land.
- ra2-decomp's note (1e9ffea) that a scheduling tie inside one block can follow the block's instruction count before
  scheduling was confirmed here on Draw_Health_Bar 0x6C3F00: the twin's YR text carries `offset.Y +=
  TClass->PixelSelectionBracketDelta;`, which RA2 lacks; without it the last pip loop's Y sum loads its operands in the
  other order (4 bytes), and a folding `+= 0` there (diagnostic only) gives the image. No natural RA2 statement found.
- Damage_Low_Bridge_EW/NS, Damage_Low_Bridge2_EW/NS (5 B each): the tie is a register choice between the two right-edge
  sums of the first expanded Union. Measured with the probes: deleting any one Damage_Low_Bridge_Piece call statement
  gives the image; moving that statement above the Union (same pool, same site total, fewer sites after the Union)
  does not; deleting only the call node and keeping its argument temporaries does not; deleting only the argument
  temporaries (or writing `Cell(cell.X - 1, cell.Y)`) tips the second Union into expansion (1302 -> 1424..1440 B); one
  dead inline probe anywhere is neutral and two tip it; duplicating call statements keeps ours. So the function sits
  one notch from the second Union's expansion in both directions, and the tie needs the pool and the site total to
  fall together, which no evidenced text does (TibSun, ra2-decomp's matched YR twin 0x57BCF0 and mischa85 all have the
  tree's statements; YR's differs only in `Kill_Illegal_Occupiers(kill)`). It is not the "nested budget / sites after
  the call" split: moving the statement keeps the miss.
- Default_Dialog_Proc 0x5FFB00: the image expands the plain function Show_Dialog at both WM_INITDIALOG sites; ours
  never does, even with 12 extra dead locals in the caller (pool not the limit), while cutting any three statements
  from Show_Dialog lets it in. So Show_Dialog's own size is over the plain-function cap by about three call statements.
  The twin's forms (the EnumChildWindows one-line wrappers Assign_All_Control_Sounds / Assign_Button_Kinds /
  Init_Control_Animations, no `using` line, no cast at Set_Control_Art, Get_Data at WM_PAINT, a constructed size
  point) keep Show_Dialog exact but do not lower it enough. WinData is already flat in this tree, so ra2-decomp's
  PrepareDialogChildControls lever (a nested struct member costs one node more) has nothing to remove here.
- A twin's bool declaration of a predicate whose body returns `cond && cond` still compiles to `mov eax, 1` in the
  callee but makes callers test `al`; in Draw_Health_Bar it also took 4 bytes off the frame. Is_Red_Health /
  Is_Yellow_Health declared bool with the twin's one-line bodies keep object (111/111) and building (288/290)
  unchanged and drop building's two `(char)` casts; patch kept in lane scratch, not landed (object.h needs the full
  includer gates).
## A coined unit-local specialisation costs every later site; the whole twin text before any part of it; scratch registers step with each named local and wrapper; an empty inline call is a site probe (found in the rebuild (lane F29, high))

Found in the rebuild (lane F29, high):

- wonline carried `template<> inline bool DynamicVectorClass<NodeNameType *>::Add` (a named new_length form no
  reference has, written for Host_Game 0x7459A0's one growth sum). It turned the sum at every later
  Session.Players.Add of the unit: OnUserList 0x760C00 (30 bytes), the three test players (2 bytes each) and the
  three sites of OnChannelJoin 0x75F530. Removed, those are exact with vector.h's Add and unchanged text, and
  Host_Game is exact again with the host's address built in place, the statements main's exact text has there,
  where the tree had `User_To_IPX_Address(user)` expanded: the same bytes, another count. When a small
  count-sensitive row is fixed by a unit-local form, check what main's accepted text spells out in that function
  before coining anything; a written-out block and an expanded helper are not the same count.
- Take the twin's text whole before judging it. RadarDiplomacy::Update_Diplomacy_Row 0x634670 was 6 bytes off
  with a hand-tuned draft; each single statement of the twin tried alone looked far worse (202 bytes by
  position, the callee-saved registers exchanged), the whole twin text left 26 bytes of scratch registers, and
  one more named local in the twin's own form made it exact. A low twin score is not a text problem either:
  TechnoClass::Rock 0x6D5F40 (0.59: a Yuri's Revenge parameter and float constants) and RadBeam's two step
  routines (0.72: a degree factor that is a variable of the file, not a literal) were exact on the first or
  second compile of the twin's statements.
- Scratch registers: a table value read inside a call's argument is a temporary that takes the next register
  of the eax, ecx, edx rotation; the same value in a named local takes eax over its index. Every inline
  wrapper call or named local ahead of a site moves the rotation of the temporaries after it by one step. A
  draft whose temporaries are one register off from some point on wants one more (or one fewer) named value or
  wrapper before that point; look at how the twin names the neighbouring values first.
- A do-while search helper (`i = 0; if (count > 0) do {...} while (++i < count)`) peels its first pass when it
  is expanded after a path on which the result is already known null; the plain for loop keeps one loop
  (Planning_Left_Press 0x617340, 847 to 17 bytes). Held as a variation: the twin has the do-while and every
  credited row is exact with either.
- Probing inline budget splits in scratch (never landing forms): `inline void NopSite(void) {}` called k times is
  k more inline sites with no spend; F27's `Pad(0)` is spend with no code. The two move opposite ways and tell
  which the image has: netshare Scenario_DlgProc 0x5C69E0 needs five or six more sites between its second and
  third list (the second then calls the vector constructor and expands Clear, the third expands
  Select_List_Item_By_Data, as the image); the ScenarioClass NoInit constructor 0x65B780 needs one site or one
  spend in its body; planmgr Planning_Execute 0x614BA0 needs fewer sites near the end of the root (the selected
  node stored directly in place of its setter lets Planning_New_Token expand). Compare the direct-call lists of
  the two listings, not the byte count: a budget row's count by position is meaningless.
- An image block order that differs from the draft's case order is the source order: rewriting
  Scenario_DlgProc and OwnerDraw::Default_Dialog_Proc in the twin's case order made every call up to the first
  budget decision the image's. A big ordinary function the image expands twice and still emits (Show_Dialog
  0x5FF7E0 in Default_Dialog_Proc) is not `inline`: the keyword expands both sites but drops the body.
- OnChannelJoin's last 17 bytes are a commutative operand order inside the expanded ArrayList<User>::add
  (`lea eax, [edx + ecx]` against `add ecx, edx`); the same header text gives the other order in OnUserFlags.
  It moves with the number of memory references in the function, not with locals, types or position in the
  file: 29 to 33 more load-and-store statements after the site give every instruction of the image, 16 before
  it flip the growth sums of the three Add sites the wrong way. No reference form found; a row to revisit when
  one of its inlined callees changes text.
- A parameter type change in a class header (MultiplayerGameMode slot 0x0C to `WideWstring *`) renames a
  vtable slot's symbol: graft every includer of the header, not only the units whose sources changed, or the
  whole-file build refuses the vtable's data section.

## An operand-order tie that counts the function's distinct static addresses, dead ones included; read a three-site budget with every call in the filter (found in the rebuild (lane F32, high))

Found in the rebuild (lane F32, high); nothing landed, the probes are diagnostic only:

- CChatEventSink::OnChannelJoin 0x75F530 (17 bytes: `add ecx, edx` against `lea eax, [edx + ecx]` in the expanded
  ArrayList<User>::add). F29's "memory references" are distinct static addresses named in the function's own text,
  and they need no code: k dead reads `ProbeM[k];` of an extern array give every instruction of the image at
  k = 45..47, 61..67, 93..95 and 125..127 and leave 2 to 14 lines elsewhere above 44. One unit per distinct address
  (also under `if (0)`; a block-local initialised from one about two; a string literal about a third); the same
  address again, reads through a pointer parameter, locals, empty blocks and constant casts count nothing. The
  counter is the function's own: the same reads in the next member function, every other guarded body of the unit
  compiled, and 1 to 64 `struct P;` lines at the unit's first line or just above the class do not move it. So
  before reading such a tie as a declaration window, put code-free reads of distinct globals in the function and
  in its neighbour: only the first moves this kind.
- A code-free probe is the cleaner ruler for it: F29's load-and-store statements gave 29..33 and added bytes each
  (they seem to weigh about a unit and a half); dead reads give the thresholds with the listing otherwise equal, so "exact" can be
  read off the diff.
- netshare Scenario_DlgProc 0x5C69E0: when a budget row has several sites of one helper, put every call the
  helper can make in the probe's filter (constructor of each level, Clear, the plain callee). With the vector
  constructors in it, no site count at one place gives the image: sites after the mode list turn the random map
  site with or before it, sites at the top only expand the last select, spends take the last site first. The image
  has the middle site poorest (constructor called, Clear expanded) and the draft has it richest, which a root-wide
  count cannot do; the difference is in that case's own text.
- An accepted text on main is a reference for a helper's form: Planning_Pick_Node 0x616070 on main is the plain
  for loop the rebuild held as coined for Planning_Left_Press 0x617340. Look at main's text of the function a
  twin's file-local helper stands for before listing the plain form as a variation.
- Scheduling inside an expanded copy that differs from the same body standalone (Planning_Refuse's `push 0`
  against `mov edx, 0x2000`) was not moved by counts (locals, stores), by the callee's text, by nesting or by
  cutting the root down to one copy: every copy expanded in that root has the other order.

## A matched twin of the same size is taken whole before any operand search; an operand-order tie can follow the function's expression count, dead expressions included (found in the rebuild (lane F31, high))

- LaserDrawClass::Draw_It 0x5334F0 (1,906 B) stood at 12 bytes through four lanes and a 256-order operand search of
  the tree's draft. ra2-decomp's matched Yuri's Revenge twin has the same 1,906 bytes; its text taken whole was exact on
  the first compile. It differs in three places at once (no named direction before `As_Facing()`, the outer colour
  tested member by member instead of through `operator==` on a temporary, and its own operand order at each glow sum),
  and no single one of them reads as the cause from the diff. When a twin is matched and has the image's size, compile
  its whole text under the tree's names before reading the residue. A relocation-masked diff of zero hunks with
  unbound function statics is an exact row waiting for bindings: name the atexit thunk rows as the object names them
  (the `_$E` counter moves when a guarded function is opened), then `check --learn` binds the statics.
- Damage_Low_Bridge_EW 0x55DEF0 (5 bytes, one load order in the expanded Union): the order follows how many expression
  nodes the function holds after inlining, counting ones the optimiser later deletes. In place of one deleted
  Damage_Low_Bridge_Piece statement, a dead inline that only copies two members keeps the image's order; the same
  inline with one subtraction gives ours; `Cell(cell.X - 1, 0)` (one load, one subtraction) keeps the image. Dead
  scalar assignments (`kill = true`) do not count. The text is three to four simple call statements over the
  threshold. The inline decision for the later Unions is a second, separate measure that moves the other way (calls
  and no-op statements add room, expanded sites spend it), so a row can sit in a narrow window of both. Probe with dead
  inline helpers of graded size (never landing forms): the smallest helper that flips the order back sizes the gap.
- `&Map[Cell(cell.X, cell.Y - 1)]` and `&Map[cell - Cell(0, 1)]` compile to the same lookup instructions; they differ
  only in inline sites and nodes. Passing `cell - Cell(1, 0)` to a `Cell const &` parameter is two temporaries (the
  operator's TPoint2D<short> and the converted Cell, a dword copy); `Cell(cell.X - 1, cell.Y)` is one.
- A variable that is a copy of another (`begin_x = x`) can take the register with a home slot while the source
  variable's first web is coalesced into it; the image then reloads that register from the copy's home after each block
  that borrows it and the source needs no home (MapClass::Subzone_Span 0x564290). The top-of-function stores come in
  declaration order, which tells whether such a copy is declared at all.

## An out-of-line constructor the image calls is the source form; site probes placed between two decisions; callee-saved registers follow use counts (found in the rebuild (lane F34, high))

Found in the rebuild (lane F34, high); nothing landed as bytes, the probes are diagnostic only:

- UnitClass::Tunnel_AI 0x6FAA30 and InfantryClass::Tunnel_AI 0x5025C0: the "DirType::Direction calls out of line" were
  never Direction: the callee 0x423EF0 is RA2's own `DirType(Coord const & from, Coord const & to)` constructor
  (anim.cpp). `DirType direction = DirType(Center_Coord(), LastTubeCoord);` gives the call and the image's copy of
  the temporary. When a draft expands a helper where the image calls something, look up what the callee is before
  reading it as a budget miss.
- A value the image holds in a scratch-saved register across exactly one call and then drops was not a named local:
  `(Map.Get_Height_GL(exit) - Map.Get_Height_GL(enter)) / height_step` evaluates the right operand first and keeps it
  in ebx; the named `enter_height` took the frame slot and pushed `distance` into the register.
- A self-copy of a member the statement does not change (`mov eax, [edi+8]` ... `mov [ebp+8], eax`) is an
  assignment from a sum (`a = a + Coord(dx, dy, 0)`), not `+=`.
- Where a probe's empty inline sites go matters as much as how many: in Tunnel_AI sites at the end of the root starve
  the later operator+ before they starve the first distance's Length (both have them after), while sites between the
  two decisions lower only the first one's share. Put the probe between the decision that must fall and the one that
  must stand. Five or more there give every instruction of the image; the frame then tells whether the real sites own
  locals (eight bytes short here, so they do).
- BuildingClass::Do_MISSION_GUARD 0x445620: one to five empty sites after Distance_To, or about 100 units of charged
  inline before it, are exact byte for byte. Checks made on the way: a non-inline function of the unit is a site only
  under the auto-inline cap (Clear_Weapons_Factory_Bib is not until cut to six statements; GetOccupantCount is);
  `DebugString` is only an empty inline where dbgprint.h's stub wins, in building.cpp it is the real 0x4068F0;
  Red Alert's `::Distance(Center_Coord(), coord)` member form is cheaper than TibSun's Distance_To level, so it expands
  more, not less.
- Callee-saved registers go by use count: Net2SetHouseAndColor 0x5BCCF0 keeps the search key in ebp through its loop
  and loads `house` at first use only when `house` has one use fewer than the twin's three (diagnostic); in
  Wait_For_Players_To_Load 0x65C520 the image's esi, edi, ebp are i, name, held value in falling use count. When two
  register parameters swap homes, count the uses of each in the draft against what the image needs before trying
  shapes; a use the image does not show (a debug stub's argument, an assert) is the usual missing one.

## A callee the image calls from among a unit's inline copies is a refused header inline, reached through an inline wrapper; no debug stub stands behind the empty-site rows (found in the rebuild (lane F35, high))

Found in the rebuild (lane F35, high). The lane tested one question across the rows that are exact once code-free inline
sites are added (Do_MISSION_GUARD 0x445620, both Tunnel_AI, the ScenarioClass NoInit constructor 0x65B780, netshare
Scenario_DlgProc, Damage_Low_Bridge, OnChannelJoin): did Westwood's text hold inline calls that compile to nothing and
that the references dropped?

- No debug or validation construct was found. Searched in the TibSun archive: `assert` is `((void)0)` (assert.h:28) and
  `BStart`/`BEnd` are empty macros (_bench.h:24), so neither is a site; `Mono_Printf` is `Mono.Printf`; the empty inlines
  are `DebugString`/`DebugStringNoPrefix` (dbgprint.h:22, variadic, so never expanded: in RA2 every call pushes its
  arguments and calls 0x4068F0), `Last_Error_Text`, `CargoClass::AI`, `StageClass::AI`, `BuildingClass::Rotation_AI`,
  the type classes' `Init(TheaterType) {}` and `TerrainTypeClass::One_Time`; none is called in the counterpart of a
  listed row. The archive marks discarded results where its image showed a call (unit.cpp:346 `Get_Coord()`, which the
  Tunnel_AI draft already has and which is code-free in RA2 because Get_Coord is no longer virtual; building.cpp:3786
  and 5929, techno.cpp:2110, infantry.cpp:4245 `height = height`), all in other functions. ra2-decomp's and mischa85's
  headers hold no further stub; mischa85's Tunnel_AI is the archive's text unmatched and ra2-decomp has none. Do not
  repeat this search.
- What was found is a wrapper level. 0x423EF0, which the tree carries as a constructor `DirType(Coord, Coord)` defined in
  anim.cpp, is the out-of-line copy of face.h's inline `DirType::Direction(Coord const &, Coord const &)`: it lies among
  anim's inline copies (after `??_GAnimClass`, before a CDTimerClass template constructor), anim's object emits that
  member byte for byte without the constructor (Flaming_Guy_AI's refused site), and the image calls it seven times, never
  at root level. A root-level `temp.Direction(a, b)` always expands (one to eight empty sites after it do not refuse it);
  through Red Alert's inline `ObjectClass::Direction(Coord const &)` (object.h, ABSTRACT.H:115) it is two levels down and
  is refused as the image has it. So where the image calls a function that sits among a unit's inline copies, look for
  the inline wrapper that puts the site one level deeper before writing an out-of-line form: `Direction(LastTubeCoord)`
  in both Tunnel_AI and `LinkedTo->Direction(coord)` in fly Nearing_Target give the image's calls of 0x423EF0. In
  UnitClass::Tunnel_AI the wrapper accounts for three of lane F34's five empty sites: two are left, anywhere between the
  first direction and Adjacent_Cell.
- The constructor is not removed here: without its declaration face.h is ra2-decomp's face.h line for line, and of the
  212 units that include it only queue changes, where Print_CRCs 0x62A360 (11,223 B, declaration-count sensitive) flips.
  Recorded for the owner: the row can be bound to `?Direction@DirType@@QAEAAV1@ABVCoord@@0@Z` (`check anim --learn` does
  it) and the constructor dropped from face.h and anim.cpp whenever Print_CRCs' window allows.
- A frame map from the compiler's own listing: compile the scratch unit with `/FAs` and an absolute `/Fa` path
  (build/r/f35/m.py `--fa`); the listing names every frame reference (`_distance$[esp+120]`), and when a probe makes the
  instructions equal one for one, pairing listing, object and image line by line gives the image's slot for each of our
  named locals and temporaries (build/r/f35/frame.py). For UnitClass::Tunnel_AI it shows the 8 missing bytes are not a
  missing local: the image keeps the two `Cell_Coord()` return temporaries in slots of their own (ours shares them with
  the other temporaries and with the tube's exit), puts `coord` in the exit's slot and the vector of occupiers above it.
  Empty sites that own a temporary (a discarded `Get_Coord()`, a by-value or const-reference parameter, a local) do not
  add frame; a file-local inline around each `Map[x].Cell_Coord()` gives 0x70 with another layout and too little
  budget. The frame is a packing order, not a count.
- Checked and ruled out for Do_MISSION_GUARD: ra2-decomp's spelling of Is_Techno (`unsigned char flag`, `flag != 0`)
  changes nothing; an inline `Assign_Mission(mission)` over `QueueMission(mission, false)` (199 of 212 sites pass false)
  has no out-of-line copy anywhere in the image and is more likely a default argument, which is no site; radio.h's
  Transmit_Message forms are three virtuals in RA2, no inline wrapper. For the ScenarioClass NoInit constructor the image
  default-constructs RandomNumber, PauseTimer and the three type lists, so the missing site is not one of their `(x)`
  initialisers, and SpecialClass has no NoInit constructor in any reference. For Nearing_Target the refused
  `operator>=` (0x4BFB30, its only call in the image) does not answer to empty sites at three places (1 to 8).

## A twin matched since the last map is the first compile; a helper's nested call is told from a root call by spend probes (found in the rebuild (lane F36, high))

Found in the rebuild (lane F36, high):

- CellClass::Reduce_Tiberium 0x475DB0 (556 B) stood at 397 bytes with a draft that reused the parameter as the result.
  ra2-decomp matched its identical Yuri's Revenge twin a few hours after the queue's twin map was made; the twin's whole
  text (a named `reducer` set on both arms, one return) was exact on the first compile. Before working a queue, diff
  ra2-decomp's data/manifest.csv between the map's commit and its head (`git diff MAPREV HEAD -- data/manifest.csv`):
  every added line is a row to compile first.
- ParticleSystemClass::Smoke_AI 0x60BB70: the image calls DynamicVectorClass<ParticleClass *>::Add at two sites and
  expands it at the third. Spend probes tell where such a call lives: with `new` and Add written in the root, no number
  of charged pads makes Add a call (a depth-1 site is expanded whatever is left); through the unit's plain member
  Spawn_Particle, which the compiler expands on its own, the nested Add becomes a call with 20 to 28 pads ahead or 12 or
  more empty sites after, exactly the image's split. A result the image keeps in a memory local, passes by address and
  then copies into a second variable is that helper's local and its return value. Look for the unit's own small
  members (here Spawn_Particle and Spawn_Held_Particle, which the exact Spark_AI and Railgun_AI already use) before
  reading "called early, expanded late" as a budget of the root's own text.
- A `shr reg, n; and reg, 1` flag read stored to a byte temporary before its test is the body of an Is_Techno-style
  accessor (`unsigned char flag = (AbstractFlags >> n) & 1; return(flag != 0);`); `test byte ptr [..], mask` after a null
  test is cast.h's As_Foot form. The two are different helpers in the same function.
- OwnerDraw::Owner_Button_Proc 0x5F0320 (same size, 87 masked hunks) has one cause at its first instruction: which of
  ebx, edi, esi take the previous procedure, the origin's width and its height. The zero register of WM_PAINT, the
  slot shared by colour and shape and the frame number held in eax all follow from ebx being free there. The origin
  constructor, declaration orders and extra uses did not move the three.
- order_search.py needs a units.json row: for a no-row function add an image-derived row in the working copy, run it,
  and restore units.json.

## A record fetched into its own pointer against a second pointer; a new twin text is pulled before anything else; one reference more or less decides who gets the callee-saved register (found in the rebuild (lane F33, high))

Found in the rebuild (lane F33, high):

- Check the reference pull first. Of the lane's exact rows, three were on the first compile of a twin text
  ra2-decomp had matched the same night: map Repair_Bridge 0x555A60 (2,743 B, a group-A row that was exact in main
  only with coined helpers), cell Reduce_Tiberium 0x475DB0 (556 B) and, with RA2's own statements kept,
  netdlg2 Net2SetHouseAndColor 0x5BCCF0 (403 B). The twin list a pull prints covers only rows that have a
  tail_buckets entry: read the twin project's log since the last map as well (Reduce_Tiberium was not on the list).
  A twin's commit can also change a helper's form: the one that matched Planning_Draw_Techno's twin gave the
  midpoint helper the early return that three held routes here had needed.
- `WinData * record = NULL; getPointer(window, &record); EditRecord * data = (EditRecord *)record;` is not the
  same code as the twin's `WinData * data = NULL; getPointer(window, &data);`. With the pointer's own address
  taken, VC6 copies a member through a scratch register before pushing it (`mov ecx, eax; push ecx`), keeps a
  decremented member in a callee-saved register beside the pushed copy, and steps the scratch rotation of the
  pushes after it. OwnerDraw::New_Edit_Proc 0x5F22A0 (2,215 B) was three such sites from the image, nine forms
  of the argument expressions changed nothing, and the twin's declaration alone made it exact. A view struct
  cast from a fetched record is a second pointer: name the record's fields in its own struct instead.
- A header change that only names members (three members where a reserved array stood) still moves the
  declaration count: wonline Encode_Game_Options turned, and ownrdraw Trackbar_Proc came back. Probe the turned
  row on both header states with forward declarations at the end of the changed header (lane tool
  build/r/f33/cs.py) before parking it; run countsens after, not before, the guard changes of the same tip.
- Which of two values takes the one callee-saved register left is a matter of single references. In
  Net2SetHouseAndColor the image keeps `who` in ebp through the search loop and loads `house` into ebp after it;
  ours gave ebp to `house` for the whole function and read `who` from its home slot inside the loop. Replacing
  any one of house's three references (probe) gave the image's choice, so the two weights were one reference
  apart; the twin's `node` local naming the slot inside the loop as well (a looked-up value, flagged) made the
  function exact. Read the first instructions after the prologue: `mov [home], edx; mov ebp, ecx` against
  `mov ebp, edx; mov [home], ecx` says which parameter lost.
- The same question is open on three rows where nothing but the callee-saved registers differ, and none of them
  is a count (dead locals and expressions leave them): netdlg2 MPlayer_Host_Dialog_Proc 0x5BEFE0 (61 bytes: message and
  lparam exchange ebx and edi; the pair turns when lparam is not passed in the set-up case's call, and with the
  order of the two point assignments, but not with one to four more reads of either), scenario
  Wait_For_Players_To_Load 0x65C520 (77 bytes: i, name and the held conversion one step round esi, edi, ebp) and
  ownrdraw Owner_Button_Proc 0x5F0320 (the previous procedure and the origin's width and height one step round).
  Adding uses at the top of a function does not move such a pair; removing the one use that keeps a value alive
  across a long block does.
- A pip loop's sum whose two operands are loads commutes with the references to the local's member, not with the
  spelling of the sum: TechnoClass::Draw_Health_Bar 0x6C3F00 loads `xpoint.Y` before `offset.Y` only when
  `offset.Y` is referred to by member somewhere ahead of the single Point2D sum (a folding `offset.Y += 0`
  anywhere before the loop, or Yuri's Revenge's `offset.Y += delta`). The archive's two member statements for the
  point are such references and are its text for this loop.
- An operand of a row sum that comes out of an inline of its own with control flow is evaluated first and
  subtracted first; the same value as an if/else local of the function is a leaf and is ordered after the globals;
  as a conditional expression it is moved below a later test. SidebarClass::AI 0x67DC90 has all three signs
  (the side's extra subtracted before StripTop, the flag worked out before Scroll's TopIndex test) and is exact
  only with such a helper, which no reference has (held). The term order written in the source sum moves nothing.
- A `default: return(0);` inside an inner switch keeps that switch's own epilogue; `break` shares the outer one
  (eight instructions in MPlayer_Host_Dialog_Proc). A value pushed from a global where a draft passes a constant, or a
  flag passed in cl to a function a draft calls through its no-argument wrapper, are read off the call sites
  before any register work.

## A twin's helper forms come with its function text; every text of a unit carrying one source line is one function body; a count window can come back with evidenced forms (found in the rebuild (lane F37, high))

Found in the rebuild (lane F37, high), in planmgr:

- ra2-decomp's new planning matches name the helper forms they rest on ("real forms"). The node search takes the
  coordinate before the id (`Planning_Find_Node(int house, Coord coord, int id)`, by-value `Coord`, `Lead_At(coord)`,
  `Get_Id()`): the same register convention, another argument evaluation order at every caller, and small enough to
  be expanded where the image expands it. The tree's `CoordStruct` form with its `Coord(coord)` copy was too large for
  that, which is why main needed an inline copy of the search. When a twin text lands in a later commit, read the
  commit's other hunks first: the helper changes are the content.
- With the twin's search, `Lead_At` (`Coord c; c = Lead_Coord(); return c == coord;`) and `Planning_Remove_Node`
  (`next->Rebuild()`, `Member_Owner`), Planning_Update 0x614010, parked as "exact form, count-sensitive" since lane
  F27, is exact again (countsens.py took the guard off). The window moved with the search or Remove_Node form, not with
  `Lead_At` alone and not with the prototype that moved. Evidenced forms are not picked for a window, but each one that
  lands is a reason to run countsens.py on the unit.
- Planning_Execute 0x614BA0 went from 516 to 272 bytes in the twin's whole text and is exact in two diagnostic states:
  (a) the selected node stored directly plus one to six empty inline sites anywhere between `Commit_Plans` and the
  delete arm's search (after the search or in the last arm they do not help); (b) the text as it stands with
  ra2-decomp's `Member_Owner` (`return member->Get_Owner();`). Form (b) costs Planning_Add_Order and
  Planning_Connect_Selection (their last decided site expands where the image calls it), so the family's budget is
  still one or two pending sites short somewhere the three functions share; `Member_Owner` with a named local is the
  heavier form the other rows need.
- Every `StringTable::LoadString` in the unit's code (15 functions, 20 sites) pushes the same file and line
  (PlanMgr.cpp, 3231). `__LINE__` is the line of one function body, so all of them are expansions of one function,
  Planning_Message 0x616FB0, and ra2-decomp's `LoadString` written in place inside its refusal helper cannot be the
  author's text even though it compiles to the same out-of-line bytes. As a budget form it differs: in place the
  message body is charged to the root (Planning_Add_Order's twin text is then exact with the tree's `Member_Owner`,
  and Planning_Update loses its third refused site), nested it is charged to the helper's share. A line number shared
  by several call sites is direct evidence of the nesting; check it before adopting an in-place form from a reference.
- Tool notes: build/r/f37/tryall.py compares every row of a unit for a set of substituted bodies and reports the rows
  that change; pair2.py pairs the unit's functions with the twin's by size sequence and lists the pairs whose
  statement skeletons differ (75 of 214 in planmgr; most are names and literals, a dozen are real form differences:
  Member_Owner, Order_Kind, Event_Result, Refuse_Order by reference, Selection_Node_State, the draw helpers).
## A function the image expands and still emits is an ordinary one under the size limit, and the limit counts its own text: scalars against array elements, wrappers against written-out calls (found in the rebuild (lane F38, high))

Found in the rebuild (lane F38, high). The lane asked when VC6 SP3 both expands a function at a site and emits its
body, for the rows where ours either calls it or expands it and loses the row (lab units under the project flags,
build/r/f38/emit.py, emit2.py, cap.py, cap2.py, cap3.py).

- Expanded and emitted: (a) an ordinary external function, member or free, under the size limit of a function not
  declared inline; (b) an `inline` function any declaration of which carries `extern` (`extern void f(int);` then
  `inline void f(int) {...}`, or `extern inline`): no size limit, body emitted at its place; (c) an inline whose address
  is taken; (d) `__declspec(dllexport)`; (e) an inline with a refused site. Expanded and dropped: a prototype without
  `extern` followed by an inline definition, `inline` in the class with a plain definition, a call ahead of the inline
  definition, a static member, a friend, `extern "C"`, a namespace. A `static` function with one call site is expanded
  whatever its size and dropped; with two sites it is held to the limit. Nothing else moves the limit: /G5 or /G6, /GX,
  /GR, /Zi, /Oa, /Ow, /Op, /Oy-, /Gy, `#pragma auto_inline(on)`, `#pragma optimize`.
- So (b) is the only form that lifts the limit and keeps the body, and no reference has it for any row here: it stays
  a variation. The rows are (a): the function's text was heavier than the original's.
- The limit is measured on the function's own text before anything in it is expanded: four calls of an inline of eight
  stores cost what four calls cost. With a constant store to a global array element as 6 units (26 fit, 27 do not,
  about 170 units): a call statement with no argument 4, one argument 7, two 9.5, three 12.5; `if (f(x)) { f(x); }`
  23; `D[i] = S;` 7 for a scalar global S, 8 for a const scalar, 10 for an array element with a constant index, a
  dereferenced array or a struct member. Casts cost nothing. So the levers are, in this order: a twin's one-call wrapper where
  the text has the three-argument call written out; a scalar where the text reads `Table[0]`; const.
- Default_Dialog_Proc 0x5FFB00 (1,482 B, exact): Show_Dialog 0x5FF7E0 was over the limit by the three
  `EnumChildWindows(dialog, proc, 0)` calls and the two reads `ScreenWidths[0]`, `ScreenHeights[0]`. ra2-decomp's
  UI::RegisterWindow has `FUN_0060AF40(window)` style wrappers (in this tree Assign_All_Control_Sounds,
  Assign_Button_Kinds, Init_Control_Animations, each with its own row) and two scalars; with exactly those it expands
  at both sites and keeps its row. One unit decides: const scalars leave it a call. With the function in, the twin's
  case order and the twin's lookup wrappers gave every decision: the tested `Get_Data` at WM_PAINT is a charged site,
  which leaves the pending slide's lookup a call; the twin's `Find_Data` (this tree's Get_Record) is under the free
  limit of 40 where Control_Record is 42, so it expands in the second copy's Set_Dialog_Kind. When a twin uses two
  lookup wrappers, which one stands at which site is part of its text.
- A value fetched by an inline wrapper is a plain copy and may stay in a callee-saved register across calls; the same
  record fetched through `getPointer(key, &record)` in the function itself is an escaped local and is reloaded after
  every call. The image's expanded Slide_Pending_Dialog keeps the record in edi: the wrapper form, although both give
  the same bytes in its own row.
- StripClass::Recalc 0x6802D0 stays parked. Update_Column_Availability 0x67CA50 measures 196 to 198 units against
  about 170. Ruled out with its own row kept: First_Used_Column() in it (72 units, charged, and no loop is under the
  free limit), a one-line Get_Doubled_Row_Count (free: the member's own row expands two calls more), early returns,
  no braces, one loop variable, the shutter shape as a scalar with or without ra2-decomp's `((short *)shape)[3]`
  (6 units of the 25 or so; TibSun has four separate shape statics where this tree has one SidebarShapes array).
  Forced in (inline, probe only) Recalc is 22 instruction lines off, so the member's missing form is still unknown.


## A twin draft is measured with every statement of the twin back in; a statement form can cross the free inline size; functions the image never calls must each have a use (found in the rebuild (lane F39, high))

Found in the rebuild (lane F39, high), in planmgr:

- Planning_Draw_Techno 0x6182F0 (2,230 B) had been parked for three lanes as "the twin's text less its separate
  source point". With ra2-decomp's Planning_Draw_Dot (both Rects initialised: 604 to 440 bytes) and the dropped
  statements back (`Point2D source; source.X = center.X; source.Y = center.Y;`, `source = action;`, the last block
  drawing from `source`) it is exact on the first compile. The two decisions that were left (a conversion nested in
  Lead_Pixel refused, Get_Event_For expanded) were the root's budget: a root this large has B = 2S, so every
  statement of the root's own text is two units of budget, and a named `EventClass` copy in the last block (probe)
  moved the same two decisions. When a draft is "the twin's text less X", put X back before reading its budget, and
  read a nested refusal in a large root as root text missing, not as a site count.
- The held variation for the same row (the guard frame test written in place) would have left Planning_Guard_Frame
  0x61A290 without a single use. A function of the unit that the image never calls (lane tool build/r/f39/uncalled.py:
  no rel32 call or jump and no data pointer to it) exists only as expansions, so each one needs a use in the source,
  and a form that removes the last use is ruled out by the image. The same list names real helpers a draft may be
  missing: Planning_Add_To_Nodes 0x615B60 has no call in our source or in the twin's.
- A statement form that compiles to the same bytes can cross the 40-unit free size. Planning_Recount 0x616D00 with
  `PlanningUnitCounts[house]++;` is free; with `PlanningUnitCounts[house] = PlanningUnitCounts[house] + 1;` (also
  with a named value, or under `bool valid = ...; if (valid)`) it measures 48 and is charged. That one charge is
  what ra2-decomp's whole Planning_Add_Order text needs here (Last_Node is then called, as in the image), with the
  twin's by-reference Planning_Order_Kind, Planning_Event_Result and Planning_Refuse_Order: every row of the unit
  exact, Planning_Update included. Held on variation_candidates.csv (no reference has the form for this function; its
  only effect is the budget), with a ready patch; it would replace main's three coined helpers in that function.
  Found by adding two scalar stores to each helper the root expands (build/r/f39/heavy.py) and reading the root's
  call list against the image's (cs.sh): the one helper whose extra weight gives the image's list is the one sitting
  just under the free size.
- Sizes measured in the unit itself (build/r/f39/size.py: k charged code-free pads, then the call; pad
  `inline void Pad(int x) {if (x) {n array stores, m scalar stores}}` measures 6n + 4m + 23, free up to 40; root
  budget 1000): Member_Owner 48 with its named owner local, 44 in ra2-decomp's `return member->Get_Owner();`,
  Last_Node 61, Node_At 141. Calibrate the pads first by counting how many of each size a root accepts; assumed
  sizes were off by one unit per pad and hid a 22-unit error.
- Planning_Execute 0x614BA0 (272 B) read against the image call by call: the first search copy expands Lead_At and
  calls Member_Owner, the second calls Lead_At and expands Member_Owner, the root expands its own Member_Owner and
  calls Node_At. Ours differs only inside the second copy: its nested Member_Owner (48) is refused, and the 48 units
  left over let Node_At in. The nested share is (R - s)//N, N the sites from the search to the end of the root, and
  it is exact with one site fewer after the search and the sites before it unchanged (probe: the selected node stored
  directly plus one empty site ahead of the delete arm). Lighter text ahead of the search (6 to 12 units) does not
  move it. With the 44-unit Member_Owner the share fits here, but the same form expands the search's Member_Owner in
  Planning_Add_Order, where the image calls it, in main's text and in the twin's alike.
- A twin text that is exact for its own row can still cost a caller: Planning_Close_Loop in ra2-decomp's text
  (Set_Loop, the unselect loop written out) is exact and small enough to be expanded in Planning_Path_Add, where the
  image calls it (353 bytes). Test a helper's twin text against every row that reaches it, not its own row.


## A written-out flag block in a twin is a plain function of the unit expanded; two missing sites can be a reference's accessors; read "N empty sites" rows as sizes first (found in the rebuild (lane F41, high))

Found in the rebuild (lane F41, high). The lane re-read the "N more empty inline sites" rows with the calibrated size model
(lane tools in build/r/f41: v.py applies named edit sets to any unit, header edits included and restored, and prints the
call list against the image's with a frame-insensitive diff; callers.py lists the image's call sites of an address by row;
fmap.py lists a listing's frame objects with reference counts).

- BuildingClass::Do_MISSION_GUARD 0x445620 (788 B, exact). The root is over the 1000 floor, so its own text sets the budget
  (B = 2S): cutting about 28 units of root text, before or after the decision, refused TPoint3D<int>::Length as the image
  does, where earlier lanes had read "one to five empty sites after Distance_To". The twin's block
  `bool idle = false; if (A && B) idle = true; else if (C) idle = true; if (idle)` is the expansion of
  BuildingClass::Fetch_Super_Weapon 0x452800, a plain function of the unit under the auto-inline size, and the TibSun
  archive has the call at that point (`Fetch_Super_Weapon() == SUPER_NONE`): the root's text is lighter and the helper is a
  charged site ahead of the decision. A flag set in two arms and tested once is how a decompiled twin shows an expanded
  bool or enum function; find the function by listing the small image functions that call the same callee with the same
  test (callers.py), then look for the archive's call. A plain function defined later in the file is expanded like one
  defined before.
- Two coined forms were exact for the same row before the evidenced one was found (an inline one-argument wrapper over
  `QueueMission(mission, false)`, an inline helper holding the test). Exact under a probe is not evidence of the form: at
  every false site of building.cpp the wrapper flips three exact rows.
- UnitClass::Tunnel_AI 0x6FAA30 and InfantryClass::Tunnel_AI 0x5025C0: the two sites lane F35 counted are ra2-decomp's tube
  accessors (`Cell Get_Exit() const`, `int Get_Count() const`, declared in its tube.h, out of line in Yuri's Revenge, no
  copy in RA2's image, so inline here). ra2-decomp's hover statement
  `(Map.Get_Height_GL(tube->Get_Exit()) - ...) / tube->Get_Count()` gives the image's copied cells, the refused Length,
  every instruction and the frame size in one statement, where the archive's block with a reused `tube_cell` gave the
  instructions only with probe sites. By-value accessors make the cell temporaries share the height step's slot;
  reference-returning ones leave the frame 8 bytes short. Casts such as `(Cell)tunnel->Exit` and `Cell(tube->Exit)` in an
  exact row are the trace of a by-value accessor: look in the references' headers for a declared member before reading
  the sites as unknown. What is left in both rows is slot sharing among 12-byte objects (see the log rows).
- In a root over the floor an empty probe site is not neutral: each call statement adds 4 units of text, 8 of budget. Six
  probe sites after `speed` in the infantry row raised the budget and expanded Move_Coord's constructors; the same six at
  the end of the root gave the image. Place count probes where they do not sit ahead of the decisions they should starve,
  and read a count that works only at the end as "sites late or less text", not as a number of sites.
- A missing spend is told from a missing site with graded pads in the first initialiser or statement: the ScenarioClass
  NoInit constructor 0x65B780 is exact with any charged pad of 41 to 251 there and unchanged by free pads of 23 to 35, so
  it lacks a spend, not a site. Weighting each helper by one and two scalar stores (4 and 8 units) finds the helper just
  under the free 40: CDTimerClass's NoInit constructor is 33 to 36, and six of them charged are the image's calls. The
  Yuri's Revenge twin gets the same spend from two class arrays RA2 does not have, so a twin that matches there can still
  be one spend short here: compare the member lists before the text.
- A plain member the image never calls (no rel32 call or jump) and still emits was expanded at every use. For
  SidebarClass::First_Used_Column 0x681980 (70 units, charged) and Update_Column_Availability 0x67CA50 (198 against about
  170) that leaves a contradiction the size model does not resolve with any reference form: the standalone member has no
  room for a 70-unit charge and Recalc needs the member 26 units lighter. rebuild/tools/il_lab.py measures a plain member
  when the scratch copy declares it inline (build/r/f41/sbvar.py).
- Three sites of one helper in a large root (netshare Scenario_DlgProc): with a = sites between the first two, b between
  the last two, the image's "middle refused, outer two expanded" needs spends between S2 and S3 below T times b and room at
  S1, which pads and count probes at three places each confirm (a pad between S1 and S2 refuses S3 first; sites anywhere
  between S2 and S3 behave alike). Write the inequalities before sweeping counts.
