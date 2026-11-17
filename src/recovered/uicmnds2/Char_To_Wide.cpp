// void __fastcall Char_To_Wide_006fa0c0(unsigned short *dst, char const *src, int limit) (0x006FA0C0)
// Widens a narrow string into dst, stopping at the terminator or after limit
// characters (-1: no limit); always terminates dst (the inverse of 0x006FA0F0).

void __fastcall Char_To_Wide_006fa0c0(unsigned short *dst, char const *src, int limit)
{
    while (*src != 0 && limit != 0) {
        *dst++ = (unsigned char)*src++;
        if (limit != -1) {
            limit--;
        }
    }
    *dst = 0;
}
