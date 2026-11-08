// Native data view (Scout1091, native M8): the sight scan's RadiusCount table
// (0x007A5908; TibSun MapClass::RadiusCount: 12 cumulative cell counts, ranges 0..11),
// read whole from the user's game.exe at run time. The data tables split it
// at 0x007A590C (unknown_cell_count_7a590c, TAction's iron curtain count), so
// the native storage of unknown_radius_count_7a5908 holds only element 0.
// Never original code or bytes.
namespace ra2port { bool original_bytes(unsigned int va, void *out, unsigned long n); }

int const *ra2port_radius_count_7a5908()
{
    static int table[12];   // ranges 0..11 (0x0054A550 clamps to 11)
    static bool loaded = false;
    if (!loaded) {
        loaded = ra2port::original_bytes(0x007A5908, table, sizeof table);
    }
    return table;
}
