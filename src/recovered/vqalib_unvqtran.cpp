// vqalib_unvqtran.cpp: the original translation unit re-filed as one source (Route A): the accepted
// bodies of its canonical compile group in address order, under the unit's own header
// state (its switches and includes in their original order). The module map
// manifests/original_target/modules/vqalib_unvqtran.json carries the addresses and the rows that
// stay in their own files.


#include "shared/vqa_player.hpp"


// SPDX-License-Identifier: GPL-3.0-or-later
// Copyright 2026 ra2-decomp contributors
// Copyright 2025 Electronic Arts Inc.; 2026 OpenTS contributors
// Derived from Electronic Arts source code; EA's GPL Section 7 terms apply (COPYING.md).
// From ra2-decomp (YR 0x007C4B80).

typedef signed int int32_t;
typedef unsigned char uint8_t;
typedef unsigned short uint16_t;
typedef unsigned int uint32_t;

void __cdecl UnVQ2_C0_4x4_KEY(unsigned char * codebook, unsigned char * pointers, unsigned char * buffer, unsigned long blocksperrow, unsigned long numrows, unsigned long bufwidth)
{
	uint16_t * src = (uint16_t *)pointers;
	uint8_t * dst = (uint8_t *)buffer;
	uint8_t * row_base = (uint8_t *)buffer;
	uint32_t blocks = 0;
	uint32_t total = numrows * blocksperrow;
	int r;
	uint32_t count;

	if (total > 0u) {

		while (1) {

			uint16_t word = *src;

			if ((word & 0x8000u) != 0) {

				int32_t op = word & 0xF000u;

				if (op <= 0xC000) {

					if (op != 0xC000) {

						switch (op) {

						case 0xB000: {
							count = ((uint32_t)word >> 8) & 0xFu;

							{
								uint32_t n = 4u * count;

								for (r = 0; r < 4; ++r) {
									for (uint32_t k = 0; k < n; ++k) {
										*dst++ = (uint8_t)word;
									}
									dst -= n;
									dst += bufwidth;
								}

								dst -= 4u * bufwidth;
							}

							dst += 4u * count;
							blocks += count;
							src += 1;
						} break;

						case 0xA000: {
							count = word & 0xFFFu;
							src += 1;
							uint16_t word2 = *src;

							if ((word2 & 0x8000u) != 0) {
								int sub = word2 & 0xC000;

								switch (sub) {

								case 0xC000: {
									const uint8_t * cb = codebook + ((uint32_t)(word2 & 0xFFFu) << 4);

									for (r = 0; r < 4; ++r) {
										uint32_t nn;
										for (nn = 0; nn < count; nn++) {
											for (int c = 0; c < 4; ++c) {
												*dst = *cb;
												++dst;
												++cb;
											}
											cb -= 4;
										}
										cb += 4;
										dst -= 4u * count;
										dst += bufwidth;
									}

									dst += 4u * count;
									dst -= 4u * bufwidth;
									src += 1;
									blocks += count;

								} break;

								case 0x8000: {
									{
										uint32_t n = 4u * count;

										for (r = 0; r < 4; ++r) {
											for (uint32_t k = 0; k < n; ++k) {
												*dst++ = (uint8_t)word2;
											}
											dst -= n;
											dst += bufwidth;
										}

										dst -= 4u * bufwidth;
									}

									dst += 4u * count;
									blocks += count;
									src += 1;

								} break;
								}

							} else {
								const uint8_t * cb = codebook + ((uint32_t)word2 << 4);

								for (r = 0; r < 4; ++r) {
									uint32_t nn;
									for (nn = 0; nn < count; nn++) {
										for (int c = 0; c < 4; ++c) {
											*dst++ = *cb++;
										}
										cb -= 4;
									}
									cb += 4;
									dst -= 4u * count;
									dst += bufwidth;
								}

								dst += 4u * count;
								dst -= 4u * bufwidth;
								src += 1;
								blocks += count;
							}

						} break;
						}

					} else {
						count = word & 0xFFFu;
						src += 1;
						for (uint32_t pairs = count >> 1; pairs != 0u; pairs--) {
							uint16_t word2 = *src;
							uint8_t hi = (uint8_t)(word2 >> 8);

							for (r = 0; r < 4; ++r) {
								for (int c = 0; c < 4; ++c) {
									*dst++ = hi;
								}
								dst -= 4u;
								dst += bufwidth;
							}
							dst -= 4u * bufwidth;
							dst += 4;

							uint8_t lo = (uint8_t)word2 & 0xFF;

							for (r = 0; r < 4; ++r) {
								for (int c = 0; c < 4; ++c) {
									*dst++ = lo;
								}
								dst -= 4u;
								dst += bufwidth;
							}
							dst -= 4u * bufwidth;
							dst += 4;

							src += 1;
						}

						blocks += count;
					}

				} else {

					switch (op) {

					case 0xF000: {
						for (r = 0; r < 4; ++r) {
							for (int c = 0; c < 4; ++c) {
								*dst++ = (uint8_t)word;
							}
							dst -= 4u;
							dst += bufwidth;
						}
						dst -= 4u * bufwidth;

						dst += 4;
						src += 1;
						blocks += 1;
					} break;

					case 0xE000: {
						const uint8_t * cb = codebook + ((uint32_t)(word & 0xFFFu) << 4);

						for (r = 0; r < 4; ++r) {
							for (int c = 0; c < 4; ++c) {
								*dst = *cb;
								++dst;
								++cb;
							}
							dst -= 4u;
							dst += bufwidth;
						}

						dst -= 4u * bufwidth;

						dst += 4u;
						src += 1;
						blocks += 1;

					} break;
					}
				}

			} else {
				const uint8_t * cb = codebook + ((uint32_t)*src << 4);

				for (r = 0; r < 4; ++r) {
					for (int c = 0; c < 4; ++c) {
						*dst++ = *cb++;
					}
					dst -= 4u;
					dst += bufwidth;
				}

				dst -= 4u * bufwidth;

				dst += 4u;
				src += 1;
				blocks += 1;
			}

			if (blocks % blocksperrow == 0u) {
				dst = row_base + 4u * bufwidth;
				row_base = dst;
			}

			if (blocks >= total) {
				return;
			}
		}
	}
}


// void __cdecl UnVQ2_C0_4x4_TRANS_HALF(unsigned char *codebook, unsigned char *pointers, unsigned char *buffer,
//                 unsigned long blocksperrow, unsigned long numrows, unsigned long bufwidth) -- 0x0077E1A0
// Purpose: the half-size form of UnVQ2_C0_4x4_TRANS: each 4x4 codebook block is drawn
// as a 2x2 block (every other pixel of its first two rows), skipping zero pixels; a
// block row wraps to the next two pixel rows after blocksperrow blocks.

void __cdecl UnVQ2_C0_4x4_TRANS_HALF(unsigned char *codebook, unsigned char *pointers, unsigned char *buffer,
                                unsigned long blocksperrow, unsigned long numrows, unsigned long bufwidth)
{
    register unsigned short *src = (unsigned short *)pointers;
    unsigned char *dst = buffer;
    unsigned long blocks = 0;
    unsigned long count;
    int r;
    unsigned char *rowstart = buffer;
    unsigned long total = blocksperrow * numrows;

    if (total > 0) {
        while (1) {
            unsigned short code = *src;
            if ((code & 0x8000) != 0) {
                long op = code & 0xF000;
                if (op <= 0xC000) {
                    if (op != 0xC000) {
                        switch (op) {
                        case 0xB000: {
                            count = ((unsigned long)code >> 8) & 0xF;
                            if ((unsigned char)code) {
                                unsigned long n = count * 2;
                                for (r = 0; r < 2; r++) {
                                    for (unsigned long k = 0; k < n; k++) {
                                        *dst++ = (unsigned char)code;
                                    }
                                    dst -= n;
                                    dst += bufwidth;
                                }
                                dst -= bufwidth * 2;
                            }
                            dst += count * 2;
                            blocks += count;
                            src++;
                        } break;

                        case 0xA000: {
                            src++;
                            unsigned short code2 = *src;
                            count = code & 0xFFF;
                            if ((code2 & 0x8000) != 0) {
                                int kind = code2 & 0xC000;
                                switch (kind) {
                                case 0xC000: {
                                    unsigned char *cb = codebook + ((unsigned long)(code2 & 0xFFF) << 4);
                                    for (r = 0; r < 2; r++) {
                                        unsigned long j;
                                        for (j = 0; j < count; j++) {
                                            for (int c = 0; c < 2; c++) {
                                                if (*cb) {
                                                    *dst = *cb;
                                                }
                                                dst++;
                                                cb += 2;
                                            }
                                            cb -= 4;
                                        }
                                        cb += 4;
                                        dst -= count * 2;
                                        dst += bufwidth;
                                    }
                                    dst += count * 2;
                                    dst -= bufwidth * 2;
                                    src++;
                                    blocks += count;
                                } break;

                                case 0x8000: {
                                    if ((unsigned char)code2) {
                                        unsigned long n = count * 2;
                                        for (r = 0; r < 2; r++) {
                                            for (unsigned long k = 0; k < n; k++) {
                                                *dst++ = (unsigned char)code2;
                                            }
                                            dst -= n;
                                            dst += bufwidth;
                                        }
                                        dst -= bufwidth * 2;
                                    }
                                    dst += count * 2;
                                    blocks += count;
                                    src++;
                                } break;
                                }
                            } else {
                                unsigned char *cb = codebook + ((unsigned long)code2 << 4);
                                for (r = 0; r < 2; r++) {
                                    unsigned long j;
                                    for (j = 0; j < count; j++) {
                                        for (int c = 0; c < 2; c++) {
                                            *dst++ = *cb;
                                            cb += 2;
                                        }
                                        cb -= 4;
                                    }
                                    cb += 4;
                                    dst -= count * 2;
                                    dst += bufwidth;
                                }
                                dst += count * 2;
                                dst -= bufwidth * 2;
                                src++;
                                blocks += count;
                            }
                        } break;
                        }
                    } else {
                        count = code & 0xFFF;
                        src++;
                        for (unsigned long pairs = count >> 1; pairs != 0; pairs--) {
                            unsigned short code2 = *src;
                            unsigned char left = (unsigned char)(code2 >> 8);
                            if (left != 0) {
                                for (r = 0; r < 2; r++) {
                                    for (int c = 0; c < 2; c++) {
                                        *dst++ = left;
                                    }
                                    dst -= 2;
                                    dst += bufwidth;
                                }
                                dst -= bufwidth * 2;
                            }
                            dst += 2;
                            if ((unsigned char)code2) {
                                for (r = 0; r < 2; r++) {
                                    for (int c = 0; c < 2; c++) {
                                        *dst++ = (unsigned char)code2;
                                    }
                                    dst -= 2;
                                    dst += bufwidth;
                                }
                                dst -= bufwidth * 2;
                            }
                            dst += 2;
                            src++;
                        }
                        blocks += count;
                    }
                } else {
                    switch (op) {
                    case 0xF000: {
                        if ((unsigned char)code) {
                            for (r = 0; r < 2; r++) {
                                for (int c = 0; c < 2; c++) {
                                    *dst++ = (unsigned char)code;
                                }
                                dst -= 2;
                                dst += bufwidth;
                            }
                            dst -= bufwidth * 2;
                        }
                        dst += 2;
                        src++;
                        blocks += 1;
                    } break;

                    case 0xE000: {
                        unsigned char *cb = codebook + ((unsigned long)(code & 0xFFF) << 4);
                        for (r = 0; r < 2; r++) {
                            for (int c = 0; c < 2; c++) {
                                if (*cb) {
                                    *dst = *cb;
                                }
                                dst++;
                                cb += 2;
                            }
                            dst -= 2;
                            dst += bufwidth;
                        }
                        dst -= bufwidth * 2;
                        dst += 2;
                        src++;
                        blocks += 1;
                    } break;
                    }
                }
            } else {
                unsigned char *cb = codebook + ((unsigned long)*src << 4);
                for (r = 0; r < 2; r++) {
                    for (int c = 0; c < 2; c++) {
                        *dst++ = *cb;
                        cb += 2;
                    }
                    dst -= 2;
                    dst += bufwidth;
                }
                dst -= bufwidth * 2;
                dst += 2;
                src++;
                blocks += 1;
            }

            if (blocks % blocksperrow == 0) {
                dst = rowstart + bufwidth * 2;
                rowstart = dst;
            }
            if (blocks >= total) {
                return;
            }
        }
    }
}


// void __cdecl UnVQ2_C0_4x2_TRANS(unsigned char *codebook, unsigned char *pointers, unsigned char *buffer,
//                 unsigned long blocksperrow, unsigned long numrows, unsigned long bufwidth) -- 0x0077E5B0
// Purpose: the 4x2-block form of UnVQ2_C0_4x4_TRANS (8-byte codebook entries, two pixel
// rows per block), skipping zero pixels of fills and codebook blocks.

void __cdecl UnVQ2_C0_4x2_TRANS(unsigned char *codebook, unsigned char *pointers, unsigned char *buffer,
                                unsigned long blocksperrow, unsigned long numrows, unsigned long bufwidth)
{
    register unsigned short *src = (unsigned short *)pointers;
    unsigned char *dst = buffer;
    unsigned long blocks = 0;
    unsigned long count;
    int r;
    unsigned char *rowstart = buffer;
    unsigned long total = blocksperrow * numrows;

    if (total > 0) {
        while (1) {
            unsigned short code = *src;
            if ((code & 0x8000) != 0) {
                long op = code & 0xF000;
                if (op <= 0xC000) {
                    if (op != 0xC000) {
                        switch (op) {
                        case 0xB000: {
                            count = ((unsigned long)code >> 8) & 0xF;
                            if ((unsigned char)code) {
                                unsigned long n = count * 4;
                                for (r = 0; r < 2; r++) {
                                    for (unsigned long k = 0; k < n; k++) {
                                        *dst++ = (unsigned char)code;
                                    }
                                    dst -= n;
                                    dst += bufwidth;
                                }
                                dst -= bufwidth * 2;
                            }
                            dst += count * 4;
                            blocks += count;
                            src++;
                        } break;

                        case 0xA000: {
                            src++;
                            unsigned short code2 = *src;
                            count = code & 0xFFF;
                            if ((code2 & 0x8000) != 0) {
                                int kind = code2 & 0xC000;
                                switch (kind) {
                                case 0xC000: {
                                    unsigned char *cb = codebook + ((unsigned long)(code2 & 0xFFF) << 3);
                                    for (r = 0; r < 2; r++) {
                                        unsigned long j;
                                        for (j = 0; j < count; j++) {
                                            for (int c = 0; c < 4; c++) {
                                                if (*cb) {
                                                    *dst = *cb;
                                                }
                                                dst++;
                                                cb++;
                                            }
                                            cb -= 4;
                                        }
                                        cb += 4;
                                        dst -= count * 4;
                                        dst += bufwidth;
                                    }
                                    dst += count * 4;
                                    dst -= bufwidth * 2;
                                    src++;
                                    blocks += count;
                                } break;

                                case 0x8000: {
                                    if ((unsigned char)code2) {
                                        unsigned long n = count * 4;
                                        for (r = 0; r < 2; r++) {
                                            for (unsigned long k = 0; k < n; k++) {
                                                *dst++ = (unsigned char)code2;
                                            }
                                            dst -= n;
                                            dst += bufwidth;
                                        }
                                        dst -= bufwidth * 2;
                                    }
                                    dst += count * 4;
                                    blocks += count;
                                    src++;
                                } break;
                                }
                            } else {
                                unsigned char *cb = codebook + ((unsigned long)code2 << 3);
                                for (r = 0; r < 2; r++) {
                                    unsigned long j;
                                    for (j = 0; j < count; j++) {
                                        for (int c = 0; c < 4; c++) {
                                            *dst++ = *cb++;
                                        }
                                        cb -= 4;
                                    }
                                    cb += 4;
                                    dst -= count * 4;
                                    dst += bufwidth;
                                }
                                dst += count * 4;
                                dst -= bufwidth * 2;
                                src++;
                                blocks += count;
                            }
                        } break;
                        }
                    } else {
                        count = code & 0xFFF;
                        src++;
                        for (unsigned long pairs = count >> 1; pairs != 0; pairs--) {
                            unsigned short code2 = *src;
                            unsigned char left = (unsigned char)(code2 >> 8);
                            if (left != 0) {
                                for (r = 0; r < 2; r++) {
                                    for (int c = 0; c < 4; c++) {
                                        *dst++ = left;
                                    }
                                    dst -= 4;
                                    dst += bufwidth;
                                }
                                dst -= bufwidth * 2;
                            }
                            dst += 4;
                            if ((unsigned char)code2) {
                                for (r = 0; r < 2; r++) {
                                    for (int c = 0; c < 4; c++) {
                                        *dst++ = (unsigned char)code2;
                                    }
                                    dst -= 4;
                                    dst += bufwidth;
                                }
                                dst -= bufwidth * 2;
                            }
                            dst += 4;
                            src++;
                        }
                        blocks += count;
                    }
                } else {
                    switch (op) {
                    case 0xF000: {
                        if ((unsigned char)code) {
                            for (r = 0; r < 2; r++) {
                                for (int c = 0; c < 4; c++) {
                                    *dst++ = (unsigned char)code;
                                }
                                dst -= 4;
                                dst += bufwidth;
                            }
                            dst -= bufwidth * 2;
                        }
                        dst += 4;
                        src++;
                        blocks += 1;
                    } break;

                    case 0xE000: {
                        unsigned char *cb = codebook + ((unsigned long)(code & 0xFFF) << 3);
                        for (r = 0; r < 2; r++) {
                            for (int c = 0; c < 4; c++) {
                                if (*cb) {
                                    *dst = *cb;
                                }
                                dst++;
                                cb++;
                            }
                            dst -= 4;
                            dst += bufwidth;
                        }
                        dst -= bufwidth * 2;
                        dst += 4;
                        src++;
                        blocks += 1;
                    } break;
                    }
                }
            } else {
                unsigned char *cb = codebook + ((unsigned long)*src << 3);
                for (r = 0; r < 2; r++) {
                    for (int c = 0; c < 4; c++) {
                        *dst++ = *cb++;
                    }
                    dst -= 4;
                    dst += bufwidth;
                }
                dst -= bufwidth * 2;
                dst += 4;
                src++;
                blocks += 1;
            }

            if (blocks % blocksperrow == 0) {
                dst = rowstart + bufwidth * 2;
                rowstart = dst;
            }
            if (blocks >= total) {
                return;
            }
        }
    }
}


// SPDX-License-Identifier: GPL-3.0-or-later
// Copyright 2026 ra2-decomp contributors
// Copyright 2025 Electronic Arts Inc.; 2026 OpenTS contributors
// Derived from Electronic Arts source code; EA's GPL Section 7 terms apply (COPYING.md).
// From ra2-decomp (YR 0x007C57D0).

typedef signed int int32_t;
typedef unsigned char uint8_t;
typedef unsigned short uint16_t;
typedef unsigned int uint32_t;

void __cdecl UnVQ2_C0_4x2_KEY(unsigned char * codebook, unsigned char * pointers, unsigned char * buffer, unsigned long blocksperrow, unsigned long numrows, unsigned long bufwidth)
{
	uint16_t * src = (uint16_t *)pointers;
	uint8_t * dst = (uint8_t *)buffer;
	uint8_t * row_base = (uint8_t *)buffer;
	uint32_t blocks = 0;
	uint32_t total = numrows * blocksperrow;
	int r;
	uint32_t count;

	if (total > 0u) {

		while (1) {

			uint16_t word = *src;

			if ((word & 0x8000u) != 0) {

				int32_t op = word & 0xF000u;

				if (op <= 0xC000) {

					if (op != 0xC000) {

						switch (op) {

						case 0xB000: {
							count = ((uint32_t)word >> 8) & 0xFu;

							{
								uint32_t n = 4u * count;

								for (r = 0; r < 2; ++r) {
									for (uint32_t k = 0; k < n; ++k) {
										*dst++ = (uint8_t)word;
									}
									dst -= n;
									dst += bufwidth;
								}

								dst -= 2u * bufwidth;
							}

							dst += 4u * count;
							blocks += count;
							src += 1;
						} break;

						case 0xA000: {
							count = word & 0xFFFu;
							src += 1;
							uint16_t word2 = *src;

							if ((word2 & 0x8000u) != 0) {
								int sub = word2 & 0xC000;

								switch (sub) {

								case 0xC000: {
									const uint8_t * cb = codebook + ((uint32_t)(word2 & 0xFFFu) << 3);

									for (r = 0; r < 2; ++r) {
										uint32_t nn;
										for (nn = 0; nn < count; nn++) {
											for (int c = 0; c < 4; ++c) {
												*dst = *cb;
												++dst;
												++cb;
											}
											cb -= 4;
										}
										cb += 4;
										dst -= 4u * count;
										dst += bufwidth;
									}

									dst += 4u * count;
									dst -= 2u * bufwidth;
									src += 1;
									blocks += count;

								} break;

								case 0x8000: {
									{
										uint32_t n = 4u * count;

										for (r = 0; r < 2; ++r) {
											for (uint32_t k = 0; k < n; ++k) {
												*dst++ = (uint8_t)word2;
											}
											dst -= n;
											dst += bufwidth;
										}

										dst -= 2u * bufwidth;
									}

									dst += 4u * count;
									blocks += count;
									src += 1;

								} break;
								}

							} else {
								const uint8_t * cb = codebook + ((uint32_t)word2 << 3);

								for (r = 0; r < 2; ++r) {
									uint32_t nn;
									for (nn = 0; nn < count; nn++) {
										for (int c = 0; c < 4; ++c) {
											*dst++ = *cb++;
										}
										cb -= 4;
									}
									cb += 4;
									dst -= 4u * count;
									dst += bufwidth;
								}

								dst += 4u * count;
								dst -= 2u * bufwidth;
								src += 1;
								blocks += count;
							}

						} break;
						}

					} else {
						count = word & 0xFFFu;
						src += 1;
						for (uint32_t pairs = count >> 1; pairs != 0u; pairs--) {
							uint16_t word2 = *src;
							uint8_t hi = (uint8_t)(word2 >> 8);

							for (r = 0; r < 2; ++r) {
								for (int c = 0; c < 4; ++c) {
									*dst++ = hi;
								}
								dst -= 4u;
								dst += bufwidth;
							}
							dst -= 2u * bufwidth;
							dst += 4;

							uint8_t lo = (uint8_t)word2 & 0xFF;

							for (r = 0; r < 2; ++r) {
								for (int c = 0; c < 4; ++c) {
									*dst++ = lo;
								}
								dst -= 4u;
								dst += bufwidth;
							}
							dst -= 2u * bufwidth;
							dst += 4;

							src += 1;
						}

						blocks += count;
					}

				} else {

					switch (op) {

					case 0xF000: {
						for (r = 0; r < 2; ++r) {
							for (int c = 0; c < 4; ++c) {
								*dst++ = (uint8_t)word;
							}
							dst -= 4u;
							dst += bufwidth;
						}
						dst -= 2u * bufwidth;

						dst += 4;
						src += 1;
						blocks += 1;
					} break;

					case 0xE000: {
						const uint8_t * cb = codebook + ((uint32_t)(word & 0xFFFu) << 3);

						for (r = 0; r < 2; ++r) {
							for (int c = 0; c < 4; ++c) {
								*dst = *cb;
								++dst;
								++cb;
							}
							dst -= 4u;
							dst += bufwidth;
						}

						dst -= 2u * bufwidth;

						dst += 4u;
						src += 1;
						blocks += 1;

					} break;
					}
				}

			} else {
				const uint8_t * cb = codebook + ((uint32_t)*src << 3);

				for (r = 0; r < 2; ++r) {
					for (int c = 0; c < 4; ++c) {
						*dst++ = *cb++;
					}
					dst -= 4u;
					dst += bufwidth;
				}

				dst -= 2u * bufwidth;

				dst += 4u;
				src += 1;
				blocks += 1;
			}

			if (blocks % blocksperrow == 0u) {
				dst = row_base + 2u * bufwidth;
				row_base = dst;
			}

			if (blocks >= total) {
				return;
			}
		}
	}
}
