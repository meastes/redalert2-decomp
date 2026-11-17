"""Shift build: link one module where the linker puts it instead of at its original addresses.

The exact build places every accepted function as a fixed-address `.text$VA`
fragment and carries the original data sections as bytes. A shift build
(`build.py --shift-module NAME`) proves the module is an ordinary object:

- the module's fresh object is linked as it is, its sections renamed to
  `.text$z<NAME>`, `.data$z<NAME>`, `.rdata$z<NAME>` (LINK sorts `$` suffixes
  within an output section, so they land after the last fixed fragment and
  after the original data), with a padding block `.text$y<NAME>` ahead of its code;
- the original bytes of its code and data ranges are replaced by 0xCC fill in the
  scaffold, so nothing can run or read from the old addresses;
- every reference into the module from accepted fragments becomes a COFF
  relocation against the module's symbol (the placement container carries a
  symbol table for the first time), and every reference out of the module is
  satisfied by a symbol the scaffold defines at the original address;
- the linked image is given the original's data directories and section names
  so Windows (or Wine) can load and run it.

The result is not byte-exact and earns nothing; its receipt records where the
module landed and how every reference was resolved.
"""
import struct

DIR32, REL32 = 6, 20
SECTION_MAP = {'.text': '.text$z%s', '.data': '.data$z%s', '.rdata': '.rdata$z%s', '.rdata$r': '.rdata$z%s',
               '.bss': '.bss$z%s'}


def rename_sections(data, name):
    """Return the object with its allocatable sections renamed for the shift link.
    The string table must end the file (VC6 objects); names longer than eight bytes go there."""
    machine, count, _, symbols_at, symbol_count, optional, _ = struct.unpack_from('<HHIIIHH', data)
    if machine != 0x14c or optional:
        raise ValueError('Expected classic i386 COFF')
    strings_at = symbols_at + 18 * symbol_count
    length = struct.unpack_from('<I', data, strings_at)[0]
    if strings_at + length != len(data):
        raise ValueError('String table does not end the object')
    out = bytearray(data)
    strings = bytearray(data[strings_at:])
    for index in range(count):
        at = 20 + 40 * index
        raw = bytes(out[at:at + 8])
        current = raw.rstrip(b'\0').decode('ascii')
        if current.startswith('/'):
            offset = int(current[1:])
            end = strings.find(b'\0', offset)
            current = strings[offset:end].decode('ascii')
        if current not in SECTION_MAP:
            continue
        new = (SECTION_MAP[current] % name).encode('ascii')
        if len(new) <= 8:
            out[at:at + 8] = new.ljust(8, b'\0')
        else:
            out[at:at + 8] = ('/%d' % len(strings)).encode('ascii').ljust(8, b'\0')
            strings.extend(new + b'\0')
    struct.pack_into('<I', strings, 0, len(strings))
    return bytes(out[:strings_at]) + bytes(strings)


def write_container(parts, relocations):
    """Classic COFF of (name, bytes) sections, like build.write_coff, plus relocations:
    {section index (0-based): [(offset, symbol name, kind)]} against undefined external symbols."""
    if not 0 < len(parts) < 32768:
        raise ValueError('Unsupported classic COFF section count')
    names = sorted({symbol for relocs in relocations.values() for _, symbol, _ in relocs})
    symbol_index = {n: i for i, n in enumerate(names)}
    strings = bytearray(b'\0\0\0\0')
    headers, payload = [], bytearray()
    offset = 20 + 40 * len(parts)
    reloc_blobs = []
    for index, (name, data) in enumerate(parts):
        field = ('/' + str(len(strings))).encode().ljust(8, b'\0'); strings.extend(name.encode() + b'\0')
        relocs = relocations.get(index, [])
        reloc_blob = b''.join(struct.pack('<IIH', o, symbol_index[s], k) for o, s, k in relocs)
        if len(relocs) > 0xffff:
            raise ValueError('Too many relocations in one section')
        headers.append((field, len(data), offset + len(payload), len(relocs), reloc_blob))
        payload.extend(data)
    # Relocation tables follow the section payloads.
    table = bytearray(); fixed = []
    for field, size, raw_at, reloc_count, blob in headers:
        reloc_at = offset + len(payload) + len(table) if reloc_count else 0
        table.extend(blob)
        fixed.append(struct.pack('<8sIIIIIIHHI', field, 0, 0, size, raw_at, reloc_at, 0, reloc_count, 0, 0x60100020))
    symbols = bytearray()
    for n in names:
        if len(n) <= 8:
            raw = n.encode().ljust(8, b'\0')
        else:
            raw = struct.pack('<II', 0, len(strings)); strings.extend(n.encode() + b'\0')
        symbols.extend(struct.pack('<8sIhHBB', raw, 0, 0, 0, 2, 0))
    symbols_at = offset + len(payload) + len(table)
    struct.pack_into('<I', strings, 0, len(strings))
    header = struct.pack('<HHIIIHH', 0x14c, len(parts), 0, symbols_at, len(names), 0, 0)
    return header + b''.join(fixed) + bytes(payload) + bytes(table) + bytes(symbols) + bytes(strings)


def quoted(name):
    if any(c in name for c in '"\\\n'):
        raise ValueError('Unsupported symbol spelling: ' + name)
    return '"%s"' % name


def definition_lines(symbols, sections):
    """Assembler lines defining `symbols` (name -> original address) at their place in the scaffold:
    `sections` = [(asm section name, start va, end va)]."""
    lines = []
    for name, address in sorted(symbols.items(), key=lambda kv: kv[1]):
        for section, start, end in sections:
            if start <= address < end:
                lines += ['.globl %s' % quoted(name), '.set %s, %s + %d' % (quoted(name), section, address - start)]
                break
        else:
            raise ValueError('%s binds outside the image: %08x' % (name, address))
    return lines


def patch_for_run(linked, original):
    """The linked image with the original's data directories (imports, resources, IAT, ...) and
    its resource section named .rsrc again, so a loader finds them at their unchanged RVAs."""
    out = bytearray(linked)
    pe_at = struct.unpack_from('<I', out, 0x3c)[0]
    original_pe = struct.unpack_from('<I', original, 0x3c)[0]
    optional_at = pe_at + 24
    magic = struct.unpack_from('<H', out, optional_at)[0]
    if magic != 0x10b or struct.unpack_from('<H', original, original_pe + 24)[0] != 0x10b:
        raise ValueError('Expected PE32 images')
    directories = 96      # offset of the data directory array in a PE32 optional header
    out[optional_at + directories:optional_at + directories + 16 * 8] = \
        original[original_pe + 24 + directories:original_pe + 24 + directories + 16 * 8]
    count = struct.unpack_from('<H', out, pe_at + 6)[0]
    optional_size = struct.unpack_from('<H', out, pe_at + 20)[0]
    table = pe_at + 24 + optional_size
    for index in range(count):
        at = table + 40 * index
        if bytes(out[at:at + 8]).rstrip(b'\0') == b'.rawres':
            out[at:at + 8] = b'.rsrc'.ljust(8, b'\0')
    return bytes(out)
