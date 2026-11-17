# Unidentified

Accepted, byte-exact sources whose functions have no evidenced name yet. They keep an address in
their file names because nothing else identifies them; every other source file under src/ is named
without an address. Each file still records its own row in the catalog, and the per-file reason is
in manifests/original_target/address_named_kept.json. A file leaves this folder when a name is
evidenced (TibSun, ra2-decomp, mischa85 names.csv or RTTI) and the rename passes the exact build.

## seed_views (37 files)

Seed views: small functions recovered from the Ghidra seed as stand-alone views. No reference table
binds a real name at their addresses and no unit compile emits them, so neither the function nor
its class is known. Names are not invented (docs/DECISIONS.md, address-free file names).

## uncalled_copies (16 files)

Functions with no caller anywhere in the image, each byte-identical to another copy in the same unit
(owner ruling 4, 2026-11-16). With no caller there is nothing to name them after, and the copies
cannot be told apart by content. The address is the only distinction.

## multimission_instances (16 files)

Members of two VectorClass<MultiMission *> / DynamicVectorClass<MultiMission *> instances (vtables
0x007A7388 in netshare and 0x007A95C4 in session). Their RTTI names are identical, so the member
name alone cannot tell the two copies apart (owner ruling 3, 2026-11-16). The member is named in
each file; the address says which instance it belongs to.
