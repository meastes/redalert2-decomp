// void __fastcall unknown_store_timecode_0069be30(unsigned short const *text, unsigned *target) (0x0069BE30):
// stores the time parsed from a "H:M:S:F" string (unknown_parse_timecode, 0x0069BD60) in target.

unsigned __fastcall unknown_parse_timecode(unsigned short const *text);

void __fastcall unknown_store_timecode_0069be30(unsigned short const *text, unsigned *target)
{
    *target = unknown_parse_timecode(text);
}
