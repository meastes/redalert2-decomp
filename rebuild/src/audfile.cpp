// SPDX-License-Identifier: GPL-3.0-or-later
// Copyright 2026 ra2-decomp contributors
// Copyright 2025 Electronic Arts Inc.
// Derived from Electronic Arts source code; EA's GPL Section 7 terms apply (COPYING.md).
#include "always.h" // RA2: the includes this file needs
#include <windows.h> // RA2: the critical sections, the thread and the window messages
#include <stdio.h> // RA2: the stream file: fopen, fflush, fclose
#include <string.h> // RA2: memset
#include "audstrm.h" // RA2: the STM_ ring buffer the stream file feeds
#include "audsvc.h" // RA2: AudioServiceInfo, the thread's timing record
#include "audtimer.h" // RA2: AudioGetTime
#include "dbgprint.h" // RA2: DebugString
#include "ftimer.h" // RA2: Frame
#include "_xmouse.h" // RA2: MouseCursor
#include "win.h" // RA2: MainWindow
#include "msgloop.h" // RA2: Peek_Windows_Message, Get_Windows_Message, Get_Input_Record_Mode, MessageLoopCount
#include "keyboard.h" // RA2: Set_Key_State, Get_Key_State, Get_Async_Key_State

/*
**	RA2: the input recorder. The executable calls these functions "audio file" after the STM_ ring buffer they use,
**	but what they stream is a recording of the window messages and of the mouse cursor, written while the game
**	runs (mode 1) and read back and replayed by Peek_Windows_Message (mode 2). A thread named "Streamer Thread" moves
**	the data between the ring buffer and the file. The windows of the game are numbered (their index in a table) in the
**	recording, so that it does not depend on the window handles of one run.
*/

/*
**	RA2: the thread's state (0x00A521E0, 0x60 bytes behind the stop flag, passed to the thread as its parameter).
*/
struct AudioFileThreadInfo {
	volatile int Stop; // RA2: +0x00; set to ask the thread to stop; volatile because the thread re-reads it: the executable tests it in memory after the stores that start the thread
	volatile int Passes; // RA2: +0x04; loops the thread has made; volatile: the executable clears it last in Audio_File_Init, after the period
	volatile int Stopped; // RA2: +0x08; set by the thread when it has stopped; volatile: Audio_File_Shutdown waits on it
	int Unknown_0C; // RA2: +0x0C
	volatile TimeStamp Period; // RA2: +0x10; milliseconds between two loops (100); volatile: the executable reads both halves of it for every use
	int Running; // RA2: +0x18; set by the thread when it has started
	int Unknown_1C; // RA2: +0x1C
	AudioServiceInfo Timing; // RA2: +0x20; the service timing of the thread
};

/*
**	RA2: the header in front of every record in the file.
*/
struct AudioFileRecordHeader {
	int Frame; // RA2: +0x00; the game frame it was recorded at
	unsigned Count; // RA2: +0x04; MessageLoopCount when it was recorded
	int Type; // RA2: +0x08; 0 for a window message, 1 for the cursor position
};

struct AudioFileCursorRecord {
		AudioFileRecordHeader Header;
	int X; // RA2: +0x0C
	int Y; // RA2: +0x10
};

struct AudioFileMessageRecord {
		AudioFileRecordHeader Header;
	MSG Message; // RA2: +0x0C; its window is the index of the window in the table
};

static AudioFileRecordHeader AudioFileNextHeader; // RA2: 0x00A521D0; the header read ahead while playing back
static int AudioFileWindowSlots; // RA2: 0x00A521DC; entries of the window table
static AudioFileThreadInfo AudioFileThread; // RA2: 0x00A521E0
static HANDLE AudioFileThreadHandle; // RA2: 0x00A52260
static DWORD AudioFileThreadId; // RA2: 0x00A52264
static CRITICAL_SECTION AudioFileCursorLock; // RA2: 0x00A52268; guards the cursor position
static CRITICAL_SECTION AudioFileLock; // RA2: 0x00A52280; guards the stream file
static int AudioFileCursorX; // RA2: 0x00A5229C
static int AudioFileCursorY; // RA2: 0x00A522A0
static int AudioFileLastCursorX; // RA2: 0x00A522A4; the last position that was recorded
static int AudioFileLastCursorY; // RA2: 0x00A522A8
static int AudioFileMode; // RA2: 0x00A522AC; 0 off, 1 recording, 2 playing back
static int AudioFileReady; // RA2: 0x00A522B0; the recorder has been set up
static HWND * AudioFileWindows; // RA2: 0x00A522B4; the window table
static int AudioFileWindowCount; // RA2: 0x00A522B8; windows in the table
static int AudioFileLocksReady; // RA2: 0x00A522BC; the critical sections exist
static int AudioFileThreadRunning; // RA2: 0x00A522C0
static STM_STREAM * AudioFileRing; // RA2: 0x00A522C4
static STM_ACCESS * AudioFileReadQueue; // RA2: 0x00A522C8; the side that data goes into the ring from
static STM_ACCESS * AudioFileFlushQueue; // RA2: 0x00A522CC; the side that data comes out of the ring from
static FILE * AudioFileStream; // RA2: 0x00A522D0
static int AudioFileFillFailed; // RA2: 0x00A522D4; the first fill of the ring failed
static int AudioFileFoundId; // RA2: 0x00A522D8; the window Audio_File_Find found last
static int AudioFileFoundIndex; // RA2: 0x00A522DC; and its index
static int AudioFileHeaderRead; // RA2: 0x00A522E0; AudioFileNextHeader has been read
static short KeyStates[256]; // RA2: 0x00A51FD0; the key states of the recording, read back by Get_Key_State

int __fastcall Audio_File_Init(int count, int size);
DWORD WINAPI Audio_File_Thread(void * data);
void __fastcall Audio_File_Shutdown(void);
void __fastcall Add_Audio_File_Window(HWND window);
void __fastcall Remove_Audio_File_Window(HWND window);
int __fastcall Audio_File_Find(int id);
HWND __fastcall Audio_File_Window(int index);
int __fastcall Audio_File_Create_Stream(char const * name);
int __fastcall Audio_File_Open_Stream(char const * name);
void __fastcall Audio_File_Close_Stream(void);
void __fastcall Audio_File_Flush(void);

/// <summary>
/// Sets the recorder up: the two critical sections, the window table (count entries), the streamer thread, and the
/// ring buffer (at least 0x2800 bytes) with its two queues. Any failure shuts it down again and returns 0.
/// </summary>
int __fastcall Audio_File_Init(int count, int size) // RA2: 0x00522560
{
	InitializeCriticalSection(&AudioFileLock);
	InitializeCriticalSection(&AudioFileCursorLock);
	AudioFileLocksReady = 1;

	if (count <= 0) {
		Audio_File_Shutdown();
		return 0;
	}
	if (size < 0x2800) {
		size = 0x2800; // RA2: the ring buffer is never smaller
	}

	AudioFileWindows = (HWND *)operator new(count * sizeof(HWND));
	if (AudioFileWindows == NULL) {
		Audio_File_Shutdown();
		return 0;
	}
	memset(AudioFileWindows, 0, count * sizeof(HWND));
	AudioFileWindowCount = 0;
	AudioFileWindowSlots = count;

	AudioFileThread.Stop = 0;
	AudioFileThread.Stopped = 0;
	AudioFileThread.Running = 0;
	AudioFileThread.Period = 100;
	AudioFileThread.Passes = 0;
	AudioServiceInfoInit(&AudioFileThread.Timing);
	AudioServiceSetInterval(&AudioFileThread.Timing, AudioFileThread.Period);
	AudioServiceSetMustServiceInterval(&AudioFileThread.Timing, AudioFileThread.Period * 4);
	AudioServiceSetResetInterval(&AudioFileThread.Timing, AudioFileThread.Period * 4);

	AudioFileThreadHandle = CreateThread(NULL, 0x1000, Audio_File_Thread, &AudioFileThread, 0, &AudioFileThreadId);
	if (AudioFileThreadHandle == NULL) {
		Audio_File_Shutdown();
		return 0;
	}
	AudioFileThreadRunning = 1;

	if (!SetThreadPriority(AudioFileThreadHandle, THREAD_PRIORITY_HIGHEST)) {
		char buffer[1024];
		FormatMessageA(FORMAT_MESSAGE_FROM_SYSTEM, NULL, GetLastError(), 0, buffer, sizeof(buffer), NULL);
		DebugString("Unable to change the priority of the Streamer Thread - %s\n", buffer);
		Audio_File_Shutdown();
		return 0;
	}

	AudioFileRing = STM_StreamCreate();
	if (AudioFileRing == NULL) {
		Audio_File_Shutdown();
		return 0;
	}
	if (!STM_StreamCreateSBuffers(AudioFileRing, 1, size, 0)) {
		Audio_File_Shutdown();
		return 0;
	}
	STM_StreamReset(AudioFileRing);
	AudioFileReadQueue = STM_StreamAcquireAccess(AudioFileRing, vSTM_ACCESS_ID_IN);
	AudioFileFlushQueue = STM_StreamAcquireAccess(AudioFileRing, vSTM_ACCESS_ID_OUT);
	if (AudioFileReadQueue == NULL || AudioFileFlushQueue == NULL) {
		Audio_File_Shutdown();
		return 0;
	}

	AudioFileReady = 1;
	memset(KeyStates, 0, sizeof(KeyStates));
	Add_Audio_File_Window(MainWindow); // RA2: the main window is window number 0 of the recording
	return 1;
}

/// <summary>
/// The streamer thread: every Period milliseconds it either tops the ring buffer up from the file (playing back) or
/// writes half of it out to the file (recording), until it is asked to stop.
/// </summary>
DWORD WINAPI Audio_File_Thread(void * data) // RA2: 0x005227D0
{
	AudioFileThreadInfo * info = (AudioFileThreadInfo *)data;

	info->Running = 1;
	info->Stopped = 0;
	while (!info->Stop) {
		AudioServicePerform(&info->Timing, AudioGetTime());

		STM_STREAM * ring = AudioFileRing;
		if (AudioFileRing != NULL && AudioFileMode != 0 && AudioFileStream != NULL) {
			EnterCriticalSection(&AudioFileLock);
			if (AudioFileMode == 2 && STM_StreamTotalBytesIn(ring) < (unsigned)STM_StreamTotalBytes(ring) >> 1) {
				if (AudioFileReadQueue != NULL) {
					int done;
					if (STM_AccessCRTFileTransfer(AudioFileReadQueue, AudioFileStream, STM_StreamTotalBytesTillFull(ring), &done) != STM_OK) {
						AudioFileFillFailed = 1; // RA2: a short read: the file is at its end
					}
				}
			} else if (AudioFileMode == 1 && STM_StreamTotalBytesTillFull(ring) < (unsigned)STM_StreamTotalBytes(ring) >> 1 && AudioFileFlushQueue != NULL) {
				STM_AccessCRTFileTransfer(AudioFileFlushQueue, AudioFileStream, (unsigned)STM_StreamTotalBytes(ring) >> 1, NULL);
				fflush(AudioFileStream);
			}
			LeaveCriticalSection(&AudioFileLock);
		}

		Sleep((DWORD)info->Period);
		info->Passes++;
	}
	info->Stopped = 1;
	return 0;
}

/// <summary>
/// Closes the recorder: flushes and closes the file, stops the thread, frees the window table, the queues, the ring
/// and the critical sections.
/// </summary>
void __fastcall Audio_File_Shutdown(void) // RA2: 0x00522930
{
	if (AudioFileReady) {
		Audio_File_Flush();
		Audio_File_Close_Stream();
	}

	if (AudioFileWindows != NULL) {
		delete[] AudioFileWindows;
		AudioFileWindows = NULL;
	}
	AudioFileWindowCount = 0;

	if (AudioFileThreadRunning) {
		AudioFileThread.Stop = 1;
		while (!AudioFileThread.Stopped) {
			Sleep(0); // RA2: waits for the thread to see the flag
		}
		WaitForSingleObject(AudioFileThreadHandle, 5000);
		CloseHandle(AudioFileThreadHandle);
		AudioFileThreadRunning = 0;
	}

	if (AudioFileReadQueue != NULL) {
		STM_AccessRelease(AudioFileReadQueue);
		AudioFileReadQueue = NULL;
	}
	if (AudioFileFlushQueue != NULL) {
		STM_AccessRelease(AudioFileFlushQueue);
		AudioFileFlushQueue = NULL;
	}
	if (AudioFileRing != NULL) {
		STM_StreamDestroy(AudioFileRing);
		AudioFileRing = NULL;
	}

	if (AudioFileLocksReady) {
		DeleteCriticalSection(&AudioFileLock);
		DeleteCriticalSection(&AudioFileCursorLock);
		AudioFileLocksReady = 0;
	}
	AudioFileReady = 0;
}

/// <summary>
/// Puts a window in the table of windows that the recording knows, unless it is in there already.
/// </summary>
void __fastcall Add_Audio_File_Window(HWND window) // RA2: 0x00522B20
{
	if (AudioFileReady && window != NULL) {
		HWND * free_slot = NULL;
		HWND * slot = AudioFileWindows;
		for (int index = 1; index < AudioFileWindowSlots; index++, slot++) {
			if (*slot == NULL) {
				if (free_slot == NULL) {
					free_slot = slot;
				}
			} else if (*slot == window) {
				return;
			}
		}
		if (free_slot != NULL) {
			*free_slot = window;
			AudioFileWindowCount++;
		}
	}
}

/// <summary>
/// Takes a window out of the table again.
/// </summary>
void __fastcall Remove_Audio_File_Window(HWND window) // RA2: 0x00522B80
{
	if (AudioFileReady && window != NULL) {
		HWND * slot = AudioFileWindows;
		for (int index = 1; index < AudioFileWindowSlots; index++, slot++) {
			if (*slot == window) {
				*slot = NULL;
				AudioFileWindowCount--;
				return;
			}
		}
	}
}

/// <summary>
/// Returns the index of a window in the table, or -1; the last answer is remembered.
/// </summary>
int __fastcall Audio_File_Find(int id) // RA2: 0x00522BD0
{
	if (AudioFileReady) {
		HWND * slot = AudioFileWindows;
		if (id == AudioFileFoundId) {
			return AudioFileFoundIndex;
		}
		int index = 0;
		while (index < AudioFileWindowSlots) {
			if ((int)*slot == id) {
				AudioFileFoundId = id;
				AudioFileFoundIndex = index;
				return index;
			}
			index++;
			slot++;
		}
	}
	return -1;
}

/// <summary>
/// Returns the window with the given index in the table, or NULL.
/// </summary>
HWND __fastcall Audio_File_Window(int index) // RA2: 0x00522C20
{
	if (AudioFileReady && index >= 0 && index < AudioFileWindowSlots) {
		return AudioFileWindows[index];
	}
	return NULL;
}

/// <summary>
/// Creates the file a recording is written to ("wb") and switches to recording.
/// </summary>
int __fastcall Audio_File_Create_Stream(char const * name) // RA2: 0x00522C50
{
	if (AudioFileReady && AudioFileStream == NULL) {
		EnterCriticalSection(&AudioFileLock);
		AudioFileStream = fopen(name, "wb");
		if (AudioFileStream != NULL) {
			AudioFileMode = 1;
		}
		LeaveCriticalSection(&AudioFileLock);
		return AudioFileStream != NULL;
	}
	return false;
}

/// <summary>
/// Opens a recording ("rb"), fills the ring buffer from it and switches to playing back.
/// </summary>
int __fastcall Audio_File_Open_Stream(char const * name) // RA2: 0x00522CB0
{
	if (AudioFileReady && AudioFileStream == NULL) {
		EnterCriticalSection(&AudioFileLock);
		AudioFileStream = fopen(name, "rb");
		AudioFileFillFailed = 0;
		if (AudioFileStream != NULL) {
			AudioFileMode = 2;
			if (STM_AccessCRTFileTransfer(AudioFileReadQueue, AudioFileStream, STM_AccessTotalBytes(AudioFileReadQueue), NULL) != STM_OK) {
				AudioFileFillFailed = 1;
			}
		}
		LeaveCriticalSection(&AudioFileLock);
		return AudioFileStream != NULL;
	}
	return false;
}

/// <summary>
/// Flushes and closes the file and switches the recorder off.
/// </summary>
void __fastcall Audio_File_Close_Stream(void) // RA2: 0x00522D50
{
	if (AudioFileReady) {
		EnterCriticalSection(&AudioFileLock);
		Audio_File_Flush();
		AudioFileMode = 0;
		if (AudioFileStream != NULL) {
			fclose(AudioFileStream);
			AudioFileStream = NULL;
		}
		LeaveCriticalSection(&AudioFileLock);
	}
}

/// <summary>
/// While recording, writes everything that is in the ring buffer out to the file.
/// </summary>
void __fastcall Audio_File_Flush(void) // RA2: 0x00522E10
{
	if (AudioFileReady) {
		EnterCriticalSection(&AudioFileLock);
		if (AudioFileStream != NULL && AudioFileRing != NULL && AudioFileMode == 1 && AudioFileFlushQueue != NULL) {
			STM_AccessCRTFileTransfer(AudioFileFlushQueue, AudioFileStream, STM_AccessTotalBytes(AudioFileFlushQueue), NULL);
			fflush(AudioFileStream);
		}
		LeaveCriticalSection(&AudioFileLock);
	}
}

/// <summary>
/// Returns the recorder's mode: 0 off, 1 recording, 2 playing back.
/// </summary>
int Get_Input_Record_Mode(void) // RA2: 0x00522E80
{
	return AudioFileMode;
}

/// <summary>
/// Waits for the next window message and returns 0 when it is WM_QUIT.
/// </summary>
int Get_Windows_Message(MSG * msg, HWND window, unsigned filter_min, unsigned filter_max) // RA2: 0x00522E90
{
	int result;

	do {
		result = Peek_Windows_Message(msg, window, filter_min, filter_max, PM_REMOVE);
	} while (!result);
	if (msg->message == WM_QUIT) {
		result = 0;
	}
	return result;
}

/// <summary>
/// The game's window message peek. Recording, it writes the cursor position and every message of a game window to
/// the file; playing back, it takes the messages from the file instead (the escape key ends the playback).
/// </summary>
int Peek_Windows_Message(MSG * msg, HWND window, unsigned filter_min, unsigned filter_max, unsigned remove) // RA2: 0x00522ED0
{
	int result;

	if (AudioFileMode == 1 && MouseCursor != NULL) {
		POINT point;
		GetCursorPos(&point);
		AudioFileCursorX = point.x;
		AudioFileCursorY = point.y;
		MouseCursor->Convert_Coordinate(AudioFileCursorX, AudioFileCursorY);
		if (AudioFileCursorX != AudioFileLastCursorX || AudioFileCursorY != AudioFileLastCursorY) {
			AudioFileCursorRecord record;
			record.Header.Type = 1;
			record.Header.Count = MessageLoopCount;
			record.Header.Frame = Frame;
			EnterCriticalSection(&AudioFileCursorLock);
			MouseCursor->Set_Mouse_Point(AudioFileCursorX, AudioFileCursorY);
			record.X = AudioFileCursorX;
			record.Y = AudioFileCursorY;
			AudioFileLastCursorX = AudioFileCursorX;
			AudioFileLastCursorY = AudioFileCursorY;
			LeaveCriticalSection(&AudioFileCursorLock);
			STM_AccessTransfer(AudioFileReadQueue, &record, sizeof(record));
		}
	}

	for (;;) {
		if (AudioFileMode == 2) {
			bool stopped = false;
			while (PeekMessageA(msg, NULL, 0, 0, PM_REMOVE)) {
				if (msg->message == WM_KEYDOWN && msg->wParam == VK_ESCAPE) {
					Audio_File_Close_Stream(); // RA2: the escape key ends the playback
					stopped = true;
					break;
				}
				if (msg->message == WM_PAINT) {
					DispatchMessageA(msg);
				}
			}
			if (stopped) {
				continue;
			}
			for (;;) {
				if (!AudioFileHeaderRead) {
					unsigned available = STM_AccessTotalBytes(AudioFileFlushQueue);
					if (available == 0) {
						if (AudioFileFillFailed) {
							Audio_File_Close_Stream(); // RA2: the file has run out
							stopped = true;
							break;
						}
						continue;
					}
					if (available < sizeof(AudioFileRecordHeader)) {
						continue;
					}
					STM_AccessTransfer(AudioFileFlushQueue, &AudioFileNextHeader, sizeof(AudioFileRecordHeader));
					AudioFileHeaderRead = 1;
				}
				if (AudioFileNextHeader.Count > (unsigned)MessageLoopCount) {
					return 0; // RA2: not due yet
				}
				AudioFileHeaderRead = 0;
				if (AudioFileNextHeader.Type == 1) {
					POINT point;
					STM_AccessTransfer(AudioFileFlushQueue, &point, sizeof(point));
					EnterCriticalSection(&AudioFileCursorLock);
					AudioFileCursorX = point.x;
					AudioFileCursorY = point.y;
					if (MouseCursor != NULL) {
						MouseCursor->Set_Mouse_Point(point.x, point.y);
					}
					LeaveCriticalSection(&AudioFileCursorLock);
					continue;
				}
				STM_AccessTransfer(AudioFileFlushQueue, msg, sizeof(MSG));
				msg->hwnd = Audio_File_Window((int)msg->hwnd);
				if (msg->hwnd != NULL) {
					result = 1;
					break;
				}
			}
			if (stopped) {
				continue;
			}
		} else {
			result = PeekMessageA(msg, window, filter_min, filter_max, remove);
		}
		if (AudioFileMode != 1 || !result) {
			return result;
		}
		int index = Audio_File_Find((int)msg->hwnd);
		if (index >= 0) {
			AudioFileMessageRecord record;
			record.Header.Count = MessageLoopCount;
			record.Header.Type = 0;
			record.Header.Frame = Frame;
			record.Message = *msg;
			record.Message.hwnd = (HWND)index;
			STM_AccessTransfer(AudioFileReadQueue, &record, sizeof(record));
			return result;
		}
		if (!remove) {
			PeekMessageA(msg, window, filter_min, filter_max, PM_REMOVE); // RA2: a message of no game window is dropped
		}
	}
}

/// <summary>
/// Gives the cursor position that the recording has (or is playing back), while it is on.
/// </summary>
void __fastcall Audio_File_Get_Cursor(unsigned * x, unsigned * y) // RA2: 0x00523360
{
	if (AudioFileReady && AudioFileMode != 0) {
		EnterCriticalSection(&AudioFileCursorLock);
		*x = AudioFileCursorX;
		*y = AudioFileCursorY;
		LeaveCriticalSection(&AudioFileCursorLock);
	}
}

/// <summary>
/// While recording or playing back, stores the state of a key.
/// </summary>
void Set_Key_State(unsigned key, bool release) // RA2: 0x005233A0
{
	if (AudioFileMode != 0) {
		KeyStates[key & 0xFF] = release ? 0 : 0x8000;
	}
}

/// <summary>
/// Returns the key state of the recording while it is on, else Windows' own.
/// </summary>
short Get_Key_State(unsigned key) // RA2: 0x005233D0
{
	if (AudioFileMode == 0) {
		return GetKeyState(key);
	}
	return KeyStates[key & 0xFF];
}

/// <summary>
/// The same for the asynchronous key state.
/// </summary>
short Get_Async_Key_State(unsigned key) // RA2: 0x005233F0
{
	if (AudioFileMode == 0) {
		return GetAsyncKeyState(key);
	}
	return KeyStates[key & 0xFF];
}
