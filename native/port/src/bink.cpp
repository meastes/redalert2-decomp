// binkw32 for the native port: the Bink container and its audio (Scout1142,
// native M18; header and timing Scout1133/Scout1138, M16/M17). Clean-room:
// written from the format's public description, no third-party code or tables.
//
// The game's movie code (BinkClass 0x0042FF30.., BinkMovieHandle 0x005A1A80,
// MSBinkAnim) runs as in the original: BinkOpen reads the movie (by name, or
// from the mix file handle at its current position under BINKFILEHANDLE
// 0x00800000) and returns a handle with the SDK's leading fields
// (shared/movie.hpp BinkHandleSize: Width, Height, Frames, FrameNum,
// LastFrameNum, FrameRate, FrameRateDiv); frames advance with
// BinkNextFrame/BinkGoto, and BinkWait answers from a clock at the header's
// rate (frame n is due (n - 1) * FrameRateDiv / FrameRate seconds after the
// start; BinkPause stops it).
//
// Container: a 44-byte header ('BIK' + revision, size - 8, frames, largest
// frame, -, width, height, rate, rate divisor, video flags, audio tracks), per
// track a u32 (largest decoded packet), then per track u16 sample rate + u16
// flags (0x4000 16-bit, 0x2000 stereo, 0x1000 DCT transform), then per track
// a u32 id; then frames + 1 u32 frame offsets (bit 0 marks a key frame). A
// frame is, per audio track, a u32 packet size and the packet, then the video.
//
// Audio: BinkDoFrame decodes the first track's packet of the current frame and
// streams it through the port mixer (ra2port::audio_stream_*), when the game
// set a sound system (BinkSetSoundSystem with its DirectSound). A packet is a
// u32 sample count, then 32-bit aligned blocks read LSB first: per (decoded)
// channel two coefficients as 29-bit floats (5-bit exponent, 23-bit mantissa,
// sign), one 8-bit quantizer index per critical band (Zwicker's Bark band
// edges), then the coefficients from 2 up in runs (a run flag and a 4-bit run
// length code, else 8; a 4-bit width; per coefficient width bits and a sign),
// inverse transformed (real inverse DFT; DCT-III for DCT tracks) and overlapped
// with the previous block's tail over a sixteenth of the block. The RDFT
// variant decodes stereo as one interleaved channel at twice the rate.
//
// Video (Scout1153, M19; port/src/bink_video.cpp): BinkDoFrame decodes the
// current frame's video (from the last key frame after a jump) and
// BinkCopyToBuffer converts it to the destination's 5:6:5 pixels. The decoder
// needs fixed tables the format description does not give: read from the
// installed BINKW32.DLL (bink_video.cpp; the default since Scout1161, M20, by the
// user's ruling); without them BinkCopyToBuffer writes a black frame.
// RA2_NATIVE_MOVIE_SKIP=1 (test runs) keeps M16's pace,
// one frame per decode pass, and plays no movie sound.
// RA2_NATIVE_BINK_DUMP=<prefix> (diagnostics) writes every 30th decoded frame
// (and the first) as <prefix>-<movie>-<frame>.bmp.
// RA2_NATIVE_BINK_WAV=<file> (diagnostics) also writes each movie's decoded
// sound to <file> as a WAV (the last movie's).
#include "ra2port/win32_port.hpp"

#include <chrono>
#include <math.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <vector>

using namespace ra2port;

namespace ra2port {
struct BinkVideo;
BinkVideo *bink_video_open(int width, int height);
void bink_video_close(BinkVideo *v);
bool bink_video_decode(BinkVideo *v, unsigned char const *data, size_t size);
void bink_video_copy_565(BinkVideo *v, void *dest, int pitch, unsigned height, unsigned x, unsigned y);
}

extern "C" {
// port/src/kernel32.cpp
HANDLE CreateFileA(const char *name, DWORD access, DWORD share, void *security, DWORD disposition, DWORD flags, HANDLE templ);
BOOL ReadFile(HANDLE handle, void *buffer, DWORD size, DWORD *read_bytes, void *overlapped);
BOOL CloseHandle(HANDLE handle);
}

namespace {

unsigned le32(unsigned char const *p) { return p[0] | p[1] << 8 | p[2] << 16 | (unsigned)p[3] << 24; }
unsigned le16(unsigned char const *p) { return p[0] | p[1] << 8; }

// Bits read least significant first from a byte buffer.
struct BitReader {
    unsigned char const *data;
    size_t bits;     // total
    size_t pos;      // read so far
    unsigned get(int n)
    {
        unsigned v = 0;
        for (int i = 0; i < n; ++i, ++pos) {
            if (pos < bits && (data[pos >> 3] >> (pos & 7) & 1)) v |= 1u << i;
        }
        return v;
    }
    bool bit() { return get(1) != 0; }
    size_t left() const { return pos < bits ? bits - pos : 0; }
    void align32() { pos = (pos + 31) & ~(size_t)31; }
};

void fft(std::vector<double> &re, std::vector<double> &im, bool inverse)
{
    size_t n = re.size();
    for (size_t i = 1, j = 0; i < n; ++i) {
        size_t bit = n >> 1;
        for (; j & bit; bit >>= 1) j ^= bit;
        j |= bit;
        if (i < j) { double t = re[i]; re[i] = re[j]; re[j] = t; t = im[i]; im[i] = im[j]; im[j] = t; }
    }
    for (size_t len = 2; len <= n; len <<= 1) {
        double a = (inverse ? 2.0 : -2.0) * M_PI / (double)len;
        for (size_t i = 0; i < n; i += len) {
            for (size_t k = 0; k < len / 2; ++k) {
                double wr = cos(a * (double)k), wi = sin(a * (double)k);
                double xr = re[i + k + len / 2] * wr - im[i + k + len / 2] * wi;
                double xi = re[i + k + len / 2] * wi + im[i + k + len / 2] * wr;
                re[i + k + len / 2] = re[i + k] - xr;
                im[i + k + len / 2] = im[i + k] - xi;
                re[i + k] += xr;
                im[i + k] += xi;
            }
        }
    }
}

// One Bink audio track's decoder.
struct BinkAudio {
    bool dct = false;
    bool revision_b = false;
    int channels = 1;          // decoded channels (1 for the interleaved RDFT variant)
    int out_channels = 1;      // channels of the stream
    int rate = 22050;          // stream rate
    int frame_len = 0, overlap = 0, bands = 0;
    int band_edge[26];
    float root = 0;
    float quant_table[96];
    bool first = true;
    std::vector<float> coeffs[2], previous[2];

    void init(int sample_rate, unsigned flags, bool rev_b)
    {
        static const int bark[25] = {100, 200, 300, 400, 510, 630, 770, 920, 1080, 1270, 1480, 1720, 2000,
                                     2320, 2700, 3150, 3700, 4400, 5300, 6400, 7700, 9500, 12000, 15500, 24500};
        dct = (flags & 0x1000) != 0;
        revision_b = rev_b;
        out_channels = (flags & 0x2000) ? 2 : 1;
        rate = sample_rate;
        int bits = sample_rate < 22050 ? 9 : sample_rate < 44100 ? 10 : 11;
        int decode_rate = sample_rate;
        if (dct) {
            channels = out_channels;
        } else {
            channels = 1;                       // stereo arrives interleaved
            decode_rate *= out_channels;
            if (!revision_b && out_channels == 2) bits += 1;
        }
        frame_len = 1 << bits;
        overlap = frame_len / 16;
        int half = (decode_rate + 1) / 2;
        int i = 1;
        for (; i < 25; ++i) {
            if (half <= bark[i - 1]) break;
        }
        bands = i;
        band_edge[0] = 2;
        for (int b = 1; b < bands; ++b) band_edge[b] = (int)((long long)bark[b - 1] * frame_len / half) & ~1;
        band_edge[bands] = frame_len;
        root = dct ? (float)(frame_len / sqrt((double)frame_len)) : (float)(2.0 / sqrt((double)frame_len));
        for (int q = 0; q < 96; ++q) quant_table[q] = expf((float)q * 0.15289164787221953823f) * root;
        for (int c = 0; c < 2; ++c) {
            coeffs[c].assign((size_t)frame_len, 0.0f);
            previous[c].assign((size_t)overlap, 0.0f);
        }
        first = true;
    }

    static float read_float(BitReader &br)
    {
        int power = (int)br.get(5);
        float f = ldexpf((float)br.get(23), power - 23);
        return br.bit() ? -f : f;
    }

    // The inverse transform of coeffs[c] in place (time samples).
    void inverse(int c)
    {
        std::vector<float> &x = coeffs[c];
        size_t n = (size_t)frame_len;
        if (dct) {
            // DCT-III: y[t] = X0 / 2 + sum X_k cos(pi k (2t + 1) / 2N), through a 2N-point DFT.
            std::vector<double> re(2 * n, 0.0), im(2 * n, 0.0);
            for (size_t k = 0; k < n; ++k) {
                double a = M_PI * (double)k / (double)(2 * n);
                double v = k == 0 ? x[0] * 0.5 : x[k];
                re[k] = v * cos(a);
                im[k] = v * sin(a);
            }
            fft(re, im, true);
            for (size_t t = 0; t < n; ++t) x[t] = (float)re[t];
        } else {
            // Packed spectrum: [0] DC, [1] Nyquist, then (re, -im) of bins 1 .. N/2 - 1 (the
            // imaginary sign measured: the overlap of consecutive blocks correlates 0.74 with
            // it, 0.14 without, in the intro logo movie).
            std::vector<double> re(n, 0.0), im(n, 0.0);
            re[0] = x[0];
            re[n / 2] = x[1];
            for (size_t k = 1; k < n / 2; ++k) {
                re[k] = x[2 * k];
                im[k] = -x[2 * k + 1];
                re[n - k] = x[2 * k];
                im[n - k] = x[2 * k + 1];
            }
            fft(re, im, true);
            for (size_t t = 0; t < n; ++t) x[t] = (float)(re[t] * 0.5);
        }
    }

    // One block: appends (frame_len - overlap) samples per stream channel to out.
    bool block(BitReader &br, std::vector<short> &out)
    {
        static const int run_length[16] = {2, 3, 4, 5, 6, 8, 9, 10, 11, 12, 13, 14, 15, 16, 32, 64};
        if (dct) br.get(2);
        for (int c = 0; c < channels; ++c) {
            std::vector<float> &x = coeffs[c];
            if (revision_b) {
                unsigned a = br.get(32), b = br.get(32);
                float fa, fb;
                memcpy(&fa, &a, 4);
                memcpy(&fb, &b, 4);
                x[0] = fa * root;
                x[1] = fb * root;
            } else {
                x[0] = read_float(br) * root;
                x[1] = read_float(br) * root;
            }
            float quant[26];
            for (int b = 0; b < bands; ++b) {
                unsigned v = br.get(8);
                quant[b] = quant_table[v < 95 ? v : 95];
            }
            int k = 0;
            float q = quant[0];
            int i = 2;
            while (i < frame_len) {
                int j;
                if (revision_b) {
                    j = i + 16;
                } else if (br.bit()) {
                    j = i + run_length[br.get(4)] * 8;
                } else {
                    j = i + 8;
                }
                if (j > frame_len) j = frame_len;
                int width = (int)br.get(4);
                if (width == 0) {
                    for (; i < j; ++i) x[i] = 0.0f;
                    while (k < bands && band_edge[k] < i) q = quant[k++];
                } else {
                    for (; i < j; ++i) {
                        if (k <= bands && band_edge[k] == i) q = quant[k++];
                        int v = (int)br.get(width);
                        x[i] = v ? (br.bit() ? -q * (float)v : q * (float)v) : 0.0f;
                    }
                }
                if (br.left() == 0 && i < frame_len) return false;
            }
            if (dct) x[0] /= 0.5f;
            inverse(c);
        }
        int keep = frame_len - overlap;
        for (int c = 0; c < channels; ++c) {
            std::vector<float> &x = coeffs[c];
            if (!first) {
                int count = overlap * channels;
                for (int i = 0, j = c; i < overlap; ++i, j += channels) {
                    x[i] = (previous[c][i] * (float)(count - j) + x[i] * (float)j) / (float)count;
                }
            }
            memcpy(previous[c].data(), &x[keep], (size_t)overlap * sizeof(float));
        }
        first = false;
        for (int i = 0; i < keep; ++i) {
            for (int c = 0; c < channels; ++c) {
                float v = coeffs[c][i];
                out.push_back((short)(v > 32767.0f ? 32767 : v < -32768.0f ? -32768 : lrintf(v)));
            }
        }
        return true;
    }

    // A packet (after its u32 size): the u32 sample count, then blocks.
    void packet(unsigned char const *data, size_t size, std::vector<short> &out)
    {
        if (size < 4) return;
        BitReader br = {data, size * 8, 32};
        while (br.left() > 0) {
            if (!block(br, out)) break;
            br.align32();
        }
    }
};

struct NativeBink {
    int Width;
    int Height;
    unsigned Frames;
    unsigned FrameNum;
    unsigned LastFrameNum;
    unsigned FrameRate;
    unsigned FrameRateDiv;
    unsigned reserved[16];   // the SDK handle is larger; nothing reads past +0x1C
    bool held;               // native (RA2_NATIVE_MOVIE_SKIP): the next frame is not due until BinkWait was asked once more
    double start;            // native: clock time of frame 1 (seconds)
    double paused_at;        // native: clock time BinkPause stopped it (0 while running)
    // native: the movie
    std::vector<unsigned char> *file;
    std::vector<unsigned> *offsets;  // frames + 1 file offsets (key flag cleared)
    unsigned tracks;
    BinkAudio *audio;                // the first track, or null
    void *stream;                    // its port voice, or null
    unsigned decoded;                // the last frame whose sound was decoded (0 none)
    std::vector<short> *wav;         // RA2_NATIVE_BINK_WAV
    std::vector<unsigned char> *keys;  // per frame: 1 for a key frame
    BinkVideo *video;                // the video decoder, or null (no tables)
    unsigned video_decoded;          // the last frame whose video was decoded (0 none)
    int movie_number;                // RA2_NATIVE_BINK_DUMP file names
    bool full_movie;                 // counted in full_movies (movie_playing)
};
// Movies open that look like a cutscene (lane I, ra2port::movie_playing): with sound and
// at least 400 pixels wide. The menus' Bink animations are silent (the main menu's
// 632x570 background) and the radar's in-game movies are small.
int full_movies = 0;
const char *last_error = "";
bool sound_system = false;           // BinkSetSoundSystem was given a DirectSound
int volume = 32768;

// The game's clock (stands still while the host pause holds the game, Scout1247).
double now()
{
    return (double)ra2port::game_clock_ns() / 1e9;
}
bool skip_movies()
{
    static int skip = -1;
    if (skip < 0) {
        const char *v = getenv("RA2_NATIVE_MOVIE_SKIP");
        skip = v && *v && *v != '0';
    }
    return skip != 0;
}
// Seconds from frame 1 to frame n at the header's rate.
double frame_time(NativeBink const *bink, unsigned n)
{
    return n > 1 ? (double)(n - 1) * bink->FrameRateDiv / bink->FrameRate : 0.0;
}
// Bytes per pixel of a BINKSURFACE type (flags' low bits): 8P, 24, 24R, 32, 32R,
// 32A, 32RA, 4444, 5551, 555, 565, 655, 664, YUY2, UYVY.
int surface_bytes(unsigned flags)
{
    unsigned type = flags & 0x0F;
    if (type == 0) return 1;
    if (type <= 2) return 3;
    if (type <= 6) return 4;
    if (type <= 14) return 2;
    return 0;
}

bool read_all(HANDLE file, unsigned char *to, size_t size)
{
    while (size) {
        DWORD got = 0, want = size > 0x40000000 ? 0x40000000 : (DWORD)size;
        if (!ReadFile(file, to, want, &got, nullptr) || got == 0) return false;
        to += got;
        size -= got;
    }
    return true;
}

void write_wav(const char *path, std::vector<short> const &pcm, int rate, int channels)
{
    FILE *f = fopen(path, "wb");
    if (!f) return;
    unsigned data = (unsigned)(pcm.size() * 2);
    unsigned char h[44] = {'R', 'I', 'F', 'F', 0, 0, 0, 0, 'W', 'A', 'V', 'E', 'f', 'm', 't', ' ', 16, 0, 0, 0, 1, 0};
    auto put32 = [&](int at, unsigned v) { h[at] = v; h[at + 1] = v >> 8; h[at + 2] = v >> 16; h[at + 3] = v >> 24; };
    put32(4, 36 + data);
    h[22] = (unsigned char)channels;
    put32(24, (unsigned)rate);
    put32(28, (unsigned)(rate * channels * 2));
    h[32] = (unsigned char)(channels * 2);
    h[34] = 16;
    memcpy(h + 36, "data", 4);
    put32(40, data);
    fwrite(h, 1, 44, f);
    fwrite(pcm.data(), 2, pcm.size(), f);
    fclose(f);
}

void write_bmp565(const char *path, NativeBink *bink)
{
    int w = bink->Width, h = bink->Height;
    std::vector<unsigned short> pixels((size_t)w * h);
    bink_video_copy_565(bink->video, pixels.data(), w * 2, (unsigned)h, 0, 0);
    FILE *f = fopen(path, "wb");
    if (!f) return;
    int row = (w * 3 + 3) & ~3;
    unsigned size = 54 + (unsigned)row * h;
    unsigned char head[54] = {'B', 'M'};
    auto put32 = [&](int at, unsigned v) { for (int i = 0; i < 4; ++i) head[at + i] = (unsigned char)(v >> (8 * i)); };
    put32(2, size); put32(10, 54); put32(14, 40); put32(18, (unsigned)w); put32(22, (unsigned)h);
    head[26] = 1; head[28] = 24;
    fwrite(head, 1, 54, f);
    std::vector<unsigned char> line((size_t)row);
    for (int y = h - 1; y >= 0; --y) {
        for (int x = 0; x < w; ++x) {
            unsigned p = pixels[(size_t)y * w + x];
            line[x * 3 + 0] = (unsigned char)((p & 0x1F) << 3);
            line[x * 3 + 1] = (unsigned char)(((p >> 5) & 0x3F) << 2);
            line[x * 3 + 2] = (unsigned char)((p >> 11) << 3);
        }
        fwrite(line.data(), 1, line.size(), f);
    }
    fclose(f);
}

// The video data of frame n (1-based): after the audio tracks' packets.
bool video_data(NativeBink *bink, unsigned n, unsigned char const **data, size_t *size)
{
    std::vector<unsigned char> &file = *bink->file;
    size_t at = (*bink->offsets)[n - 1], end = (*bink->offsets)[n];
    if (end > file.size() || at > end) return false;
    for (unsigned t = 0; t < bink->tracks; ++t) {
        if (at + 4 > end) return false;
        at += 4 + (size_t)le32(&file[at]);
    }
    if (at > end) return false;
    *data = &file[at];
    *size = end - at;
    return true;
}

// Decodes the video of frame n once; after a jump from the last key frame up to n.
void decode_video(NativeBink *bink, unsigned n)
{
    if (!bink->video || n == 0 || n > bink->Frames || n == bink->video_decoded) return;
    unsigned from = n;
    if (n != bink->video_decoded + 1) {
        while (from > 1 && !(*bink->keys)[from - 1]) --from;
    }
    static const char *dump = getenv("RA2_NATIVE_BINK_DUMP");
    for (unsigned k = from; k <= n; ++k) {
        unsigned char const *data;
        size_t size;
        if (!video_data(bink, k, &data, &size)) break;
        bink_video_decode(bink->video, data, size);
        if (dump && (k == 1 || k % 30 == 0)) {
            char path[512];
            snprintf(path, sizeof path, "%s-%02d-%04u.bmp", dump, bink->movie_number, k);
            write_bmp565(path, bink);
        }
    }
    bink->video_decoded = n;
}

// Decodes the sound of frame n (1-based) once, in order; a jump restarts the overlap.
void decode_sound(NativeBink *bink, unsigned n)
{
    if (!bink->audio || n == 0 || n > bink->Frames || n == bink->decoded) return;
    if (n != bink->decoded + 1) bink->audio->first = true;
    bink->decoded = n;
    std::vector<unsigned char> &file = *bink->file;
    size_t at = (*bink->offsets)[n - 1], end = (*bink->offsets)[n];
    if (end > file.size() || at + 4 > end) return;
    size_t size = le32(&file[at]);
    if (at + 4 + size > end) return;
    std::vector<short> pcm;
    bink->audio->packet(&file[at + 4], size, pcm);
    if (pcm.empty()) return;
    int frames = (int)(pcm.size() / bink->audio->out_channels);
    if (bink->stream) audio_stream_write(bink->stream, pcm.data(), frames);
    if (bink->wav) bink->wav->insert(bink->wav->end(), pcm.begin(), pcm.end());
}
}  // namespace

namespace ra2port {
bool movie_playing() { return full_movies > 0; }
}

extern "C" {

void *BinkOpen(const char *name, DWORD flags)
{
    unsigned char header[44];
    HANDLE file;
    bool own = !(flags & 0x00800000u);
    if (own) {
        file = CreateFileA(name, 0x80000000u, 1, nullptr, 3, 0, nullptr);
        if (file == RA2_INVALID_HANDLE) { last_error = "file not found"; return nullptr; }
    } else {
        file = (HANDLE)name;
    }
    bool ok = read_all(file, header, sizeof header) && header[0] == 'B' && header[1] == 'I' && header[2] == 'K';
    std::vector<unsigned char> *data = nullptr;
    if (ok) {
        size_t size = (size_t)le32(header + 4) + 8;
        if (size < sizeof header || size > ((size_t)1 << 31)) {
            ok = false;
        } else {
            data = new std::vector<unsigned char>(size);
            memcpy(data->data(), header, sizeof header);
            ok = read_all(file, data->data() + sizeof header, size - sizeof header);
        }
    }
    if (own) CloseHandle(file);
    if (!ok) {
        delete data;
        last_error = "not a Bink file";
        trace("BinkOpen: not a Bink file");
        return nullptr;
    }
    NativeBink *bink = (NativeBink *)calloc(1, sizeof(NativeBink));
    bink->Frames = le32(header + 8);
    bink->Width = (int)le32(header + 20);
    bink->Height = (int)le32(header + 24);
    bink->FrameRate = le32(header + 28);
    bink->FrameRateDiv = le32(header + 32);
    if (bink->FrameRateDiv == 0) bink->FrameRateDiv = 1;
    if (bink->FrameRate < bink->FrameRateDiv) bink->FrameRate = bink->FrameRateDiv;
    bink->FrameNum = 1;
    bink->start = now();
    bink->file = data;
    bink->tracks = le32(header + 40);
    std::vector<unsigned char> &f = *data;
    size_t at = 44 + (size_t)bink->tracks * 4;
    unsigned rate = 0, aflags = 0;
    if (bink->tracks > 256 || at + bink->tracks * 8 > f.size()) bink->tracks = 0;
    if (bink->tracks) {
        rate = le16(&f[at]);
        aflags = le16(&f[at + 2]);
    }
    at += (size_t)bink->tracks * 8;
    bink->offsets = new std::vector<unsigned>;
    bink->keys = new std::vector<unsigned char>;
    if (at + ((size_t)bink->Frames + 1) * 4 <= f.size()) {
        for (unsigned i = 0; i <= bink->Frames; ++i) {
            bink->offsets->push_back(le32(&f[at + i * 4]) & ~1u);
            bink->keys->push_back(le32(&f[at + i * 4]) & 1u);
        }
    } else {
        bink->Frames = 0;
    }
    if (bink->tracks && rate && bink->Frames) {
        bink->audio = new BinkAudio;
        bink->audio->init((int)rate, aflags, header[3] == 'b');
        if (sound_system && !skip_movies()) {
            bink->stream = audio_stream_open(bink->audio->rate, bink->audio->out_channels);
            audio_stream_volume(bink->stream, volume);
        }
        if (getenv("RA2_NATIVE_BINK_WAV")) bink->wav = new std::vector<short>;
    }
    static int movies = 0;
    bink->movie_number = ++movies;
    if (bink->Frames && (!skip_movies() || getenv("RA2_NATIVE_BINK_DUMP"))) bink->video = bink_video_open(bink->Width, bink->Height);
    if (bink->tracks > 0 && bink->Width >= 400) {
        bink->full_movie = true;
        ++full_movies;
    }
    trace("BinkOpen: '%c' %dx%d, %u frames at %u/%u fps, %u audio tracks (%u Hz, flags 0x%X)%s", header[3],
          bink->Width, bink->Height, bink->Frames, bink->FrameRate, bink->FrameRateDiv, bink->tracks, rate, aflags,
          bink->stream ? ", sound" : "");
    return bink;
}
void BinkClose(void *handle)
{
    NativeBink *bink = (NativeBink *)handle;
    if (!bink) return;
    if (bink->full_movie) --full_movies;
    if (bink->stream) audio_stream_close(bink->stream);
    if (bink->wav) {
        if (!bink->wav->empty()) write_wav(getenv("RA2_NATIVE_BINK_WAV"), *bink->wav, bink->audio->rate, bink->audio->out_channels);
        delete bink->wav;
    }
    delete bink->audio;
    bink_video_close(bink->video);
    delete bink->keys;
    delete bink->offsets;
    delete bink->file;
    free(bink);
}
// The decoded frame at x, y of the destination (5:6:5), clipped to its height;
// black without a decoder (no tables) or for other surface types.
int BinkCopyToBuffer(void *handle, void *dest, int pitch, unsigned height, unsigned x, unsigned y, unsigned flags)
{
    NativeBink *bink = (NativeBink *)handle;
    int bytes = surface_bytes(flags);
    if (!bink || !dest || pitch <= 0 || bytes == 0) return 0;
    if (bink->video && bink->video_decoded && (flags & 0xF) == 10) {
        bink_video_copy_565(bink->video, dest, pitch, height, x, y);
        return 0;
    }
    for (unsigned row = 0; row < (unsigned)bink->Height && y + row < height; ++row) {
        memset((unsigned char *)dest + (size_t)(y + row) * (unsigned)pitch + (size_t)x * bytes, 0,
               (size_t)bink->Width * bytes);
    }
    return 0;
}
int BinkDDSurfaceType(void *) { return 10; }   // BINKSURFACE565: the port's DirectDraw surfaces
// Decodes the current frame: its sound and its video.
int BinkDoFrame(void *handle)
{
    NativeBink *bink = (NativeBink *)handle;
    if (bink && (bink->stream || bink->wav)) decode_sound(bink, bink->FrameNum);
    if (bink && bink->video) decode_video(bink, bink->FrameNum);
    return 0;
}
const char *BinkGetError(void) { return last_error; }
int BinkGoto(void *handle, unsigned frame, int)
{
    NativeBink *bink = (NativeBink *)handle;
    if (bink) {
        bink->LastFrameNum = bink->FrameNum;
        bink->FrameNum = frame ? frame : 1;
        bink->start = now() - frame_time(bink, bink->FrameNum);
        if (bink->paused_at) bink->paused_at = now();
    }
    return 0;
}
void BinkNextFrame(void *handle)
{
    NativeBink *bink = (NativeBink *)handle;
    if (bink) { bink->LastFrameNum = bink->FrameNum; ++bink->FrameNum; bink->held = true; }
}
int BinkOpenDirectSound(uintptr_t) { return 1; }
// The game passes BinkOpenDirectSound and its DirectSound object (BinkClass::Open
// 0x0042FF30) when sound is on: movies then play their sound through the port mixer.
int BinkSetSoundSystem(void *, uintptr_t direct_sound)
{
    sound_system = direct_sound != 0;
    return sound_system ? 1 : 0;
}
// Stops (nonzero) or restarts the movie's clock and sound; returns the paused state.
int BinkPause(void *handle, int pause)
{
    NativeBink *bink = (NativeBink *)handle;
    if (!bink) return 0;
    if (pause && !bink->paused_at) {
        bink->paused_at = now();
        audio_stream_pause(bink->stream, true);
    } else if (!pause && bink->paused_at) {
        bink->start += now() - bink->paused_at;
        bink->paused_at = 0;
        audio_stream_pause(bink->stream, false);
    }
    return bink->paused_at != 0;
}
void BinkSetVolume(void *handle, int value)
{
    NativeBink *bink = (NativeBink *)handle;
    volume = value;
    if (bink && bink->stream) audio_stream_volume(bink->stream, value);
}
// Nonzero while the next frame is not due: frame FrameNum is due frame_time
// after the start (paused movies wait). Under RA2_NATIVE_MOVIE_SKIP each frame
// is due on the BinkWait after the one that follows BinkNextFrame: the decode
// loops (BinkClass 0x00430600: `do { ... BinkNextFrame } while (BinkWait() == 0)`)
// show one frame per pass, as fast as the caller loops.
int BinkWait(void *handle)
{
    NativeBink *bink = (NativeBink *)handle;
    if (!bink) return 0;
    if (skip_movies()) {
        if (bink->held) { bink->held = false; return 1; }
        return 0;
    }
    if (bink->paused_at) return 1;
    return now() - bink->start < frame_time(bink, bink->FrameNum) ? 1 : 0;
}

}  // extern "C"
