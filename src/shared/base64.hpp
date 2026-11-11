// Westwood's Base64 text coding (wwlib) (module src/recovered/base64.cpp): the data is coded in groups of three bytes
// into four characters of the 64-character alphabet; the text is padded with the pad character.
#ifndef RA2_SHARED_BASE64_HPP
#define RA2_SHARED_BASE64_HPP

// Encodes slen bytes into at most dlen characters (a multiple of four is used); a terminating zero is written when
// room remains. Returns the number of characters produced.
int __fastcall Base64_Encode(void const *source, int slen, void *dest, int dlen);
// Decodes the text; characters outside the alphabet are skipped and the pad character ends the input.
// Returns the number of bytes produced.
int __fastcall Base64_Decode(void const *source, int slen, void *dest, int dlen);


#endif
