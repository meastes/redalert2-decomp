// Westwood's Base64 tables (BASE64.CPP), read by Base64_Encode/Base64_Decode
// (0x0042D550/0x0042D640) and the Base64 pipes/straws (INI key blocks, the RSA
// public key). Adopted from the TibSun archive, code/base64.cpp (GPL-3.0-or-later
// with EA section 7 terms, THIRD_PARTY.md); compared privately with the pinned
// target at these addresses (equal). No bytes from the executable.
#define BAD 0xFE   // ignore this character in source data
#define END 0xFF   // premature end of input data (the pad)

// 0x0079C61C / 0x0079C620: the pad and the alphabet.
char const *unknown_base64_pad_79c61c = "=";
char const *unknown_base64_chars_79c620 = "ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz0123456789+/";

// 0x0079C624: the value of each character.
unsigned char unknown_base64_table_79c624[256] = {
	BAD,BAD,BAD,BAD,BAD,BAD,BAD,BAD,BAD,BAD,BAD,BAD,BAD,BAD,BAD,BAD,
	BAD,BAD,BAD,BAD,BAD,BAD,BAD,BAD,BAD,BAD,BAD,BAD,BAD,BAD,BAD,BAD,
	BAD,BAD,BAD,BAD,BAD,BAD,BAD,BAD,BAD,BAD,BAD,62,BAD,BAD,BAD,63,
	52,53,54,55,56,57,58,59,60,61,BAD,BAD,BAD,END,BAD,BAD,
	BAD,0,1,2,3,4,5,6,7,8,9,10,11,12,13,14,
	15,16,17,18,19,20,21,22,23,24,25,BAD,BAD,BAD,BAD,BAD,
	BAD,26,27,28,29,30,31,32,33,34,35,36,37,38,39,40,
	41,42,43,44,45,46,47,48,49,50,51,BAD,BAD,BAD,BAD,BAD,
	BAD,BAD,BAD,BAD,BAD,BAD,BAD,BAD,BAD,BAD,BAD,BAD,BAD,BAD,BAD,BAD,
	BAD,BAD,BAD,BAD,BAD,BAD,BAD,BAD,BAD,BAD,BAD,BAD,BAD,BAD,BAD,BAD,
	BAD,BAD,BAD,BAD,BAD,BAD,BAD,BAD,BAD,BAD,BAD,BAD,BAD,BAD,BAD,BAD,
	BAD,BAD,BAD,BAD,BAD,BAD,BAD,BAD,BAD,BAD,BAD,BAD,BAD,BAD,BAD,BAD,
	BAD,BAD,BAD,BAD,BAD,BAD,BAD,BAD,BAD,BAD,BAD,BAD,BAD,BAD,BAD,BAD,
	BAD,BAD,BAD,BAD,BAD,BAD,BAD,BAD,BAD,BAD,BAD,BAD,BAD,BAD,BAD,BAD,
	BAD,BAD,BAD,BAD,BAD,BAD,BAD,BAD,BAD,BAD,BAD,BAD,BAD,BAD,BAD,BAD,
	BAD,BAD,BAD,BAD,BAD,BAD,BAD,BAD,BAD,BAD,BAD,BAD,BAD,BAD,BAD,BAD
};
