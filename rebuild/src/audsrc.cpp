// SPDX-License-Identifier: GPL-3.0-or-later
// Copyright 2026 ra2-decomp contributors
// Copyright 2025 Electronic Arts Inc.
// Derived from Electronic Arts source code; EA's GPL Section 7 terms apply (COPYING.md).
#include "always.h"
#include <windows.h>
#include <mmsystem.h>
#include <mmreg.h>
#include "audsrc.h"
#include "wwfile.h"
#include <stdlib.h>
#include <string.h>







AudioSample * __fastcall AudioCreateSample(unsigned bytes)
{
	AudioSample * sample;

	sample = (AudioSample *)operator new(sizeof(AudioSample));
	memset(sample, 0, sizeof(AudioSample));
	AudioSampleInit(sample);
	sample->Bytes = bytes;

	if (!(sample->Data = (char *)operator new(sample->Bytes))) {
		if (sample) {
			AudioSampleDestroy(sample);
		}
		return NULL;
	}

	return sample;
}

void __fastcall AudioSampleDestroy(AudioSample * sample)
{
	if (sample->Data) {
		operator delete(sample->Data);
	}
	operator delete(sample);
}

void __fastcall AudioSampleInit(AudioSample * sample)
{
	sample->Data = NULL;
	sample->Bytes = 0;
	sample->Format = NULL;
	sample->Attribs = NULL;
	ListInit(&sample->Frames);
}

void __fastcall AudioSampleAddFrame(AudioSample * sample, AudioFrame * frame)
{
	ListNodeInsert(&sample->Frames, &frame->nd);
	sample->Bytes += frame->Bytes;
	frame->sample = sample;
}

AudioFrame * __fastcall AudioSampleFirstFrame(AudioSample * sample)
{
	return (AudioFrame *)ListNodeNext(&sample->Frames);
}

void __fastcall AudioSampleDeinit(AudioSample * sample)
{
}

int __fastcall AudioFormatBytes(AudioFormat * format, TimeStamp time)
{
	return format->BytesPerSecond * (int)time / SECONDS(1);
}

TimeStamp __fastcall AudioFormatTime(AudioFormat * format, int bytes)
{
	if (format->BytesPerSecond) {
		return (((TimeStamp)bytes) * (TimeStamp)SECONDS(1)) / (TimeStamp)format->BytesPerSecond;
	}

	return 0;
}

void __fastcall AudioFormatInit(AudioFormat * format)
{
	memset(format, 0, sizeof(AudioFormat));
}

void __fastcall AudioFormatUpdate(AudioFormat * format)
{
	format->BytesPerSecond = format->Channels * format->SampleWidth * format->Rate;
	if (format->Compression == AUDIO_COMPRESS_IMA_ADPCM || format->Compression == AUDIO_COMPRESS_MS_ADPCM) {
		format->BytesPerSecond >>= 2;
	}
}

void __fastcall AudioFrameInit(AudioFrame * frame, void * data, int bytes)
{
	frame->Bytes = bytes;
	frame->Data = data;
	ListNodeInit(&frame->nd);
	frame->sample = NULL;
}

void __fastcall AudioFrameDeinit(AudioFrame * frame)
{
	frame->sample = NULL;
}

int __fastcall AudioFormatReadWaveFile(FileClass * file, AudioFormat * format, int * bytes)
{
	WAVEFORMATEX * wformat = NULL;
	int result = 0;

	if (bytes) {
		*bytes = 0;
	}

	if (file) {
		struct {
				unsigned long form;
				unsigned long length;
				unsigned long type;
		} rh;

		file->Seek(0, SEEK_SET);

		if (file->Read(&rh, sizeof(rh)) == sizeof(rh) && rh.form == FOURCC_RIFF && rh.type == mmioFOURCC('W', 'A', 'V', 'E')) {
			struct {
					unsigned long type;
					unsigned long length;
			} chunk;

			while (file->Read(&chunk, sizeof(chunk)) == sizeof(chunk)) {
				switch (chunk.type) {
					case mmioFOURCC('f', 'm', 't', ' '):
					if (chunk.length < 0x12) {
						wformat = (WAVEFORMATEX *)malloc(0x12);
						memset(wformat, 0, 0x12);
					} else {
						wformat = (WAVEFORMATEX *)malloc(chunk.length);
						memset(wformat, 0, chunk.length);
					}
					file->Read(wformat, chunk.length);
					wformat->cbSize = (unsigned short)chunk.length;
					continue;
					case mmioFOURCC('d', 'a', 't', 'a'):
					break;
				default:
						file->Seek(chunk.length, SEEK_CUR);
					continue;
				}

				*bytes = chunk.length;
				if (wformat) {
					if (wformat->wFormatTag == WAVE_FORMAT_IMA_ADPCM) {
						format->BlockSize = wformat->nBlockAlign;
						format->Compression = AUDIO_COMPRESS_IMA_ADPCM;
						format->SampleWidth = 2;
					} else if (wformat->wFormatTag == WAVE_FORMAT_PCM) {
						format->Compression = AUDIO_COMPRESS_NONE;
						format->SampleWidth = wformat->wBitsPerSample / 8;
					} else {
						break;
					}
					format->Channels = wformat->nChannels;
					format->BytesPerSecond = wformat->nAvgBytesPerSec;
					format->Rate = wformat->nSamplesPerSec;
					format->Flags = mAUDIO_FORMAT_PCM;
					AudioFormatUpdate(format);
					result = 1;
				}
				break;
			}
		}
	}

	if (wformat) {
		free(wformat);
	}
	return result;
}

