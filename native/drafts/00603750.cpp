// NON-EXACT DRAFT 0x603750 Unknown_00603750 (scheme table theater)
// entry 0x603750; exact-match score 70.35 (misses_index best); plan "draft, verify"
// origin lane: Scout1008 (build/class-work/Scout1008/w/st0.cpp)
// Recovered source text by our lanes/workers; never exact-match credit. Tracked by
// Scout1089 (native/drafts.json). Not original bytes.
// void __fastcall Unknown_00603750(int theater) (0x00603750; Init_Theater 0x00519E70): makes the table on first
// use and switches it to the theater (nothing when it is unchanged).

void __fastcall Unknown_00603750(int theater)
{
    SchemeTable *table = Scheme_Table();
    if (table->unknown_00 != theater) {
        table->Set_Theater(theater);
    }
}
