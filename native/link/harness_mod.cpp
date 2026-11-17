// Skirmish harness: what the mod tests read (lane C16, mod support; README "Mods"). Test
// scaffolding, never game behaviour. At the first game frame (harness_input.cpp):
//   RA2_NATIVE_HARNESS_DUMP_RULES=<file>  the game's own RULES.INI as the game reads it
//                                         (CCFileClass: a loose file first, else its MIX) is
//                                         written to <file>, for a test to make a mod from;
//   RA2_NATIVE_HARNESS_COST=<ID>[,<ID>]   each vehicle type's cost as the rules loaded it
//                                         (TechnoTypeClass::Raw_Cost), logged "harness-cost ID N".
#include "shared/cc_file_class.hpp"
#include "shared/unit_type.hpp"
#include "shared/techno_type.hpp"
#include <stdio.h>
#include <stdlib.h>

namespace {
bool Same(char const *a, char const *b, int n)
{
    for (int i = 0; i < n; ++i) {
        char x = a[i] >= 'a' && a[i] <= 'z' ? a[i] - 32 : a[i];
        char y = b[i] >= 'a' && b[i] <= 'z' ? b[i] - 32 : b[i];
        if (x != y) return false;
        if (!x) return true;
    }
    return b[n] == 0;
}
}  // namespace

void Native_Harness_Rules()
{
    if (char const *path = getenv("RA2_NATIVE_HARNESS_DUMP_RULES")) {
        CCFileClass file("RULES.INI");
        long size = file.Is_Available() ? file.Size() : 0;
        if (size > 0 && file.Open()) {
            char *buffer = (char *)malloc(size);
            long got = file.Read(buffer, size);
            file.Close();
            if (FILE *out = fopen(path, "wb")) {
                fwrite(buffer, 1, got, out);
                fclose(out);
                fprintf(stderr, "harness-rules: %ld bytes of RULES.INI written\n", got);
            }
            free(buffer);
        } else {
            fprintf(stderr, "harness-rules: RULES.INI not available\n");
        }
    }
    if (char const *ids = getenv("RA2_NATIVE_HARNESS_COST")) {
        while (*ids) {
            int n = 0;
            while (ids[n] && ids[n] != ',') ++n;
            for (int i = 0; i < UnitTypes.Count(); ++i) {
                UnitTypeClass *type = UnitTypes[i];
                if (type && Same(ids, ((TechnoTypeClass *)type)->ID.Buffer, n))
                    fprintf(stderr, "harness-cost %s %d\n", ((TechnoTypeClass *)type)->ID.Buffer, ((TechnoTypeClass *)type)->Raw_Cost());
            }
            ids += n;
            if (*ids == ',') ++ids;
        }
    }
}
