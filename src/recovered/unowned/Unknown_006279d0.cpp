// Saved repeated-megamission unit lists; original006279D0.
#include "shared/queue.hpp"
#include "shared/crt_memory.hpp"
#include "shared/debug_printf.hpp"
extern char const queue_cache_debug_007ecbcc[];
// @ref ?queue_cache_debug_007ecbcc@@3QBDB=007ecbcc
// @ref ?unknown_megamission_lists_a76ca8@@3PAY0CIA@DA=00a76ca8
bool __fastcall Unknown_006279D0(unsigned char *count, int *size, char *whom, char *units, bool save)
{
    bool matched = false;
    if (*count > 1) {
        for (int i = 0; i < 8; ++i) {
            int saved_count = unknown_megamission_counts_aabdb0[i];
            if (*count == saved_count &&
                memcmp(unknown_megamission_lists_a76ca8[i], whom, 5) == 0 &&
                memcmp(unknown_megamission_lists_a76ca8[i] + 5, units,
                    saved_count * 5 - 5) == 0) {
                if (save) *size += 5 - *count * 5;
                matched = true;
                if (save) {
                    *count = 0;
                    whom[6] = (char)i;
                }
                goto finished;
            }
        }
    }
    if (save && *count > 1) {
        int oldest = -1;
        int greatest_age = -1;
        for (int slot = 0; slot < 8; ++slot) {
            if (unknown_megamission_frames_aabe94[slot] == 0) {
                oldest = slot;
                break;
            }
            int age = Frame - unknown_megamission_frames_aabe94[slot];
            if (age > greatest_age) {
                greatest_age = age;
                oldest = slot;
            }
        }
        if (oldest == -1) oldest = 0;
        Debug_Printf(queue_cache_debug_007ecbcc, *count, oldest);
        unknown_megamission_frames_aabe94[oldest] = Frame;
        memcpy(unknown_megamission_lists_a76ca8[oldest], whom, 5);
        memcpy(unknown_megamission_lists_a76ca8[oldest] + 5, units, *count * 5 - 5);
        unknown_megamission_counts_aabdb0[oldest] = *count;
        whom[6] = (char)oldest;
    }
finished:
    return matched;
}
