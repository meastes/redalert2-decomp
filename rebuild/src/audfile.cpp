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
struct AudioFileThreadInfo { // RA2: as above
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
struct AudioFileRecordHeader { // RA2: as above
	int Frame; // RA2: +0x00; the game frame it was recorded at
	unsigned Count; // RA2: +0x04; MessageLoopCount when it was recorded
	int Type; // RA2: +0x08; 0 for a window message, 1 for the cursor position
};

struct AudioFileCursorRecord { // RA2: as above
	AudioFileRecordHeader Header; // RA2: as above
	int X; // RA2: +0x0C
	int Y; // RA2: +0x10
};

struct AudioFileMessageRecord { // RA2: as above
	AudioFileRecordHeader Header; // RA2: as above
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

int __fastcall Audio_File_Init(int count, int size); // RA2: as above
DWORD WINAPI Audio_File_Thread(void * data); // RA2: as above
void __fastcall Audio_File_Shutdown(void); // RA2: as above
void __fastcall Add_Audio_File_Window(HWND window); // RA2: as above
void __fastcall Remove_Audio_File_Window(HWND window); // RA2: as above
int __fastcall Audio_File_Find(int id); // RA2: as above
HWND __fastcall Audio_File_Window(int index); // RA2: as above
int __fastcall Audio_File_Create_Stream(char const * name); // RA2: as above
int __fastcall Audio_File_Open_Stream(char const * name); // RA2: as above
void __fastcall Audio_File_Close_Stream(void); // RA2: as above
void __fastcall Audio_File_Flush(void); // RA2: as above

/// <summary>
/// Sets the recorder up: the two critical sections, the window table (count entries), the streamer thread, and the
/// ring buffer (at least 0x2800 bytes) with its two queues. Any failure shuts it down again and returns 0.
/// </summary>
int __fastcall Audio_File_Init(int count, int size) // RA2: 0x00522560
{
	InitializeCriticalSection(&AudioFileLock); // RA2: as above
	InitializeCriticalSection(&AudioFileCursorLock); // RA2: as above
	AudioFileLocksReady = 1; // RA2: as above

	if (count <= 0) { // RA2: as above
		Audio_File_Shutdown(); // RA2: as above
		return 0; // RA2: as above
	}
	if (size < 0x2800) { // RA2: as above
		size = 0x2800; // RA2: the ring buffer is never smaller
	}

	AudioFileWindows = (HWND *)operator new(count * sizeof(HWND)); // RA2: as above
	if (AudioFileWindows == NULL) { // RA2: as above
		Audio_File_Shutdown(); // RA2: as above
		return 0; // RA2: as above
	}
	memset(AudioFileWindows, 0, count * sizeof(HWND)); // RA2: as above
	AudioFileWindowCount = 0; // RA2: as above
	AudioFileWindowSlots = count; // RA2: as above

	AudioFileThread.Stop = 0; // RA2: as above
	AudioFileThread.Stopped = 0; // RA2: as above
	AudioFileThread.Running = 0; // RA2: as above
	AudioFileThread.Period = 100; // RA2: as above
	AudioFileThread.Passes = 0; // RA2: as above
	AudioServiceInfoInit(&AudioFileThread.Timing); // RA2: as above
	AudioServiceSetInterval(&AudioFileThread.Timing, AudioFileThread.Period); // RA2: as above
	AudioServiceSetMustServiceInterval(&AudioFileThread.Timing, AudioFileThread.Period * 4); // RA2: as above
	AudioServiceSetResetInterval(&AudioFileThread.Timing, AudioFileThread.Period * 4); // RA2: as above

	AudioFileThreadHandle = CreateThread(NULL, 0x1000, Audio_File_Thread, &AudioFileThread, 0, &AudioFileThreadId); // RA2: as above
	if (AudioFileThreadHandle == NULL) { // RA2: as above
		Audio_File_Shutdown(); // RA2: as above
		return 0; // RA2: as above
	}
	AudioFileThreadRunning = 1; // RA2: as above

	if (!SetThreadPriority(AudioFileThreadHandle, THREAD_PRIORITY_HIGHEST)) { // RA2: as above
		char buffer[1024]; // RA2: as above
		FormatMessageA(FORMAT_MESSAGE_FROM_SYSTEM, NULL, GetLastError(), 0, buffer, sizeof(buffer), NULL); // RA2: as above
		DebugString("Unable to change the priority of the Streamer Thread - %s\n", buffer); // RA2: as above
		Audio_File_Shutdown(); // RA2: as above
		return 0; // RA2: as above
	}

	AudioFileRing = STM_StreamCreate(); // RA2: as above
	if (AudioFileRing == NULL) { // RA2: as above
		Audio_File_Shutdown(); // RA2: as above
		return 0; // RA2: as above
	}
	if (!STM_StreamCreateSBuffers(AudioFileRing, 1, size, 0)) { // RA2: as above
		Audio_File_Shutdown(); // RA2: as above
		return 0; // RA2: as above
	}
	STM_StreamReset(AudioFileRing); // RA2: as above
	AudioFileReadQueue = STM_StreamAcquireAccess(AudioFileRing, vSTM_ACCESS_ID_IN); // RA2: as above
	AudioFileFlushQueue = STM_StreamAcquireAccess(AudioFileRing, vSTM_ACCESS_ID_OUT); // RA2: as above
	if (AudioFileReadQueue == NULL || AudioFileFlushQueue == NULL) { // RA2: as above
		Audio_File_Shutdown(); // RA2: as above
		return 0; // RA2: as above
	}

	AudioFileReady = 1; // RA2: as above
	memset(KeyStates, 0, sizeof(KeyStates)); // RA2: as above
	Add_Audio_File_Window(MainWindow); // RA2: the main window is window number 0 of the recording
	return 1; // RA2: as above
}

/// <summary>
/// The streamer thread: every Period milliseconds it either tops the ring buffer up from the file (playing back) or
/// writes half of it out to the file (recording), until it is asked to stop.
/// </summary>
DWORD WINAPI Audio_File_Thread(void * data) // RA2: 0x005227D0
{
	AudioFileThreadInfo * info = (AudioFileThreadInfo *)data; // RA2: as above

	info->Running = 1; // RA2: as above
	info->Stopped = 0; // RA2: as above
	while (!info->Stop) { // RA2: as above
		AudioServicePerform(&info->Timing, AudioGetTime()); // RA2: as above

		STM_STREAM * ring = AudioFileRing; // RA2: as above
		if (AudioFileRing != NULL && AudioFileMode != 0 && AudioFileStream != NULL) { // RA2: as above
			EnterCriticalSection(&AudioFileLock); // RA2: as above
			if (AudioFileMode == 2 && STM_StreamTotalBytesIn(ring) < (unsigned)STM_StreamTotalBytes(ring) >> 1) { // RA2: as above
				if (AudioFileReadQueue != NULL) { // RA2: as above
					int done; // RA2: as above
					if (STM_AccessCRTFileTransfer(AudioFileReadQueue, AudioFileStream, STM_StreamTotalBytesTillFull(ring), &done) != STM_OK) { // RA2: as above
						AudioFileFillFailed = 1; // RA2: a short read: the file is at its end
					}
				}
			} else if (AudioFileMode == 1 && STM_StreamTotalBytesTillFull(ring) < (unsigned)STM_StreamTotalBytes(ring) >> 1 && AudioFileFlushQueue != NULL) { // RA2: as above
				STM_AccessCRTFileTransfer(AudioFileFlushQueue, AudioFileStream, (unsigned)STM_StreamTotalBytes(ring) >> 1, NULL); // RA2: as above
				fflush(AudioFileStream); // RA2: as above
			}
			LeaveCriticalSection(&AudioFileLock); // RA2: as above
		}

		Sleep((DWORD)info->Period); // RA2: as above
		info->Passes++; // RA2: as above
	}
	info->Stopped = 1; // RA2: as above
	return 0; // RA2: as above
}

/// <summary>
/// Closes the recorder: flushes and closes the file, stops the thread, frees the window table, the queues, the ring
/// and the critical sections.
/// </summary>
void __fastcall Audio_File_Shutdown(void) // RA2: 0x00522930
{
	if (AudioFileReady) { // RA2: as above
		Audio_File_Flush(); // RA2: as above
		Audio_File_Close_Stream(); // RA2: as above
	}

	if (AudioFileWindows != NULL) { // RA2: as above
		delete [] AudioFileWindows; // RA2: as above
		AudioFileWindows = NULL; // RA2: as above
	}
	AudioFileWindowCount = 0; // RA2: as above

	if (AudioFileThreadRunning) { // RA2: as above
		AudioFileThread.Stop = 1; // RA2: as above
		while (!AudioFileThread.Stopped) { // RA2: as above
			Sleep(0); // RA2: waits for the thread to see the flag
		}
		WaitForSingleObject(AudioFileThreadHandle, 5000); // RA2: as above
		CloseHandle(AudioFileThreadHandle); // RA2: as above
		AudioFileThreadRunning = 0; // RA2: as above
	}

	if (AudioFileReadQueue != NULL) { // RA2: as above
		STM_AccessRelease(AudioFileReadQueue); // RA2: as above
		AudioFileReadQueue = NULL; // RA2: as above
	}
	if (AudioFileFlushQueue != NULL) { // RA2: as above
		STM_AccessRelease(AudioFileFlushQueue); // RA2: as above
		AudioFileFlushQueue = NULL; // RA2: as above
	}
	if (AudioFileRing != NULL) { // RA2: as above
		STM_StreamDestroy(AudioFileRing); // RA2: as above
		AudioFileRing = NULL; // RA2: as above
	}

	if (AudioFileLocksReady) { // RA2: as above
		DeleteCriticalSection(&AudioFileLock); // RA2: as above
		DeleteCriticalSection(&AudioFileCursorLock); // RA2: as above
		AudioFileLocksReady = 0; // RA2: as above
	}
	AudioFileReady = 0; // RA2: as above
}

/// <summary>
/// Puts a window in the table of windows that the recording knows, unless it is in there already.
/// </summary>
void __fastcall Add_Audio_File_Window(HWND window) // RA2: 0x00522B20
{
	if (AudioFileReady && window != NULL) { // RA2: as above
		HWND * free_slot = NULL; // RA2: as above
		HWND * slot = AudioFileWindows; // RA2: as above
		for (int index = 1; index < AudioFileWindowSlots; index++, slot++) { // RA2: as above
			if (*slot == NULL) { // RA2: as above
				if (free_slot == NULL) { // RA2: as above
					free_slot = slot; // RA2: as above
				}
			} else if (*slot == window) { // RA2: as above
				return; // RA2: as above
			}
		}
		if (free_slot != NULL) { // RA2: as above
			*free_slot = window; // RA2: as above
			AudioFileWindowCount++; // RA2: as above
		}
	}
}

/// <summary>
/// Takes a window out of the table again.
/// </summary>
void __fastcall Remove_Audio_File_Window(HWND window) // RA2: 0x00522B80
{
	if (AudioFileReady && window != NULL) { // RA2: as above
		HWND * slot = AudioFileWindows; // RA2: as above
		for (int index = 1; index < AudioFileWindowSlots; index++, slot++) { // RA2: as above
			if (*slot == window) { // RA2: as above
				*slot = NULL; // RA2: as above
				AudioFileWindowCount--; // RA2: as above
				return; // RA2: as above
			}
		}
	}
}

/// <summary>
/// Returns the index of a window in the table, or -1; the last answer is remembered.
/// </summary>
int __fastcall Audio_File_Find(int id) // RA2: 0x00522BD0
{
	if (AudioFileReady) { // RA2: as above
		HWND * slot = AudioFileWindows; // RA2: as above
		if (id == AudioFileFoundId) { // RA2: as above
			return AudioFileFoundIndex; // RA2: as above
		}
		int index = 0; // RA2: as above
		while (index < AudioFileWindowSlots) { // RA2: as above
			if ((int)*slot == id) { // RA2: as above
				AudioFileFoundId = id; // RA2: as above
				AudioFileFoundIndex = index; // RA2: as above
				return index; // RA2: as above
			}
			index++; // RA2: as above
			slot++; // RA2: as above
		}
	}
	return -1; // RA2: as above
}

/// <summary>
/// Returns the window with the given index in the table, or NULL.
/// </summary>
HWND __fastcall Audio_File_Window(int index) // RA2: 0x00522C20
{
	if (AudioFileReady && index >= 0 && index < AudioFileWindowSlots) { // RA2: as above
		return AudioFileWindows[index]; // RA2: as above
	}
	return NULL; // RA2: as above
}

/// <summary>
/// Creates the file a recording is written to ("wb") and switches to recording.
/// </summary>
int __fastcall Audio_File_Create_Stream(char const * name) // RA2: 0x00522C50
{
	if (AudioFileReady && AudioFileStream == NULL) { // RA2: as above
		EnterCriticalSection(&AudioFileLock); // RA2: as above
		AudioFileStream = fopen(name, "wb"); // RA2: as above
		if (AudioFileStream != NULL) { // RA2: as above
			AudioFileMode = 1; // RA2: as above
		}
		LeaveCriticalSection(&AudioFileLock); // RA2: as above
		return AudioFileStream != NULL; // RA2: as above
	}
	return false; // RA2: as above
}

/// <summary>
/// Opens a recording ("rb"), fills the ring buffer from it and switches to playing back.
/// </summary>
int __fastcall Audio_File_Open_Stream(char const * name) // RA2: 0x00522CB0
{
	if (AudioFileReady && AudioFileStream == NULL) { // RA2: as above
		EnterCriticalSection(&AudioFileLock); // RA2: as above
		AudioFileStream = fopen(name, "rb"); // RA2: as above
		AudioFileFillFailed = 0; // RA2: as above
		if (AudioFileStream != NULL) { // RA2: as above
			AudioFileMode = 2; // RA2: as above
			if (STM_AccessCRTFileTransfer(AudioFileReadQueue, AudioFileStream, STM_AccessTotalBytes(AudioFileReadQueue), NULL) != STM_OK) { // RA2: as above
				AudioFileFillFailed = 1; // RA2: as above
			}
		}
		LeaveCriticalSection(&AudioFileLock); // RA2: as above
		return AudioFileStream != NULL; // RA2: as above
	}
	return false; // RA2: as above
}

/// <summary>
/// Flushes and closes the file and switches the recorder off.
/// </summary>
void __fastcall Audio_File_Close_Stream(void) // RA2: 0x00522D50
{
	if (AudioFileReady) { // RA2: as above
		EnterCriticalSection(&AudioFileLock); // RA2: as above
		Audio_File_Flush(); // RA2: as above
		AudioFileMode = 0; // RA2: as above
		if (AudioFileStream != NULL) { // RA2: as above
			fclose(AudioFileStream); // RA2: as above
			AudioFileStream = NULL; // RA2: as above
		}
		LeaveCriticalSection(&AudioFileLock); // RA2: as above
	}
}

/// <summary>
/// While recording, writes everything that is in the ring buffer out to the file.
/// </summary>
void __fastcall Audio_File_Flush(void) // RA2: 0x00522E10
{
	if (AudioFileReady) { // RA2: as above
		EnterCriticalSection(&AudioFileLock); // RA2: as above
		if (AudioFileStream != NULL && AudioFileRing != NULL && AudioFileMode == 1 && AudioFileFlushQueue != NULL) { // RA2: as above
			STM_AccessCRTFileTransfer(AudioFileFlushQueue, AudioFileStream, STM_AccessTotalBytes(AudioFileFlushQueue), NULL); // RA2: as above
			fflush(AudioFileStream); // RA2: as above
		}
		LeaveCriticalSection(&AudioFileLock); // RA2: as above
	}
}

/// <summary>
/// Returns the recorder's mode: 0 off, 1 recording, 2 playing back.
/// </summary>
int Get_Input_Record_Mode(void) // RA2: 0x00522E80
{
	return AudioFileMode; // RA2: as above
}

/// <summary>
/// Waits for the next window message and returns 0 when it is WM_QUIT.
/// </summary>
int Get_Windows_Message(MSG * msg, HWND window, unsigned filter_min, unsigned filter_max) // RA2: 0x00522E90
{
	int result; // RA2: as above

	do { // RA2: as above
		result = Peek_Windows_Message(msg, window, filter_min, filter_max, PM_REMOVE); // RA2: as above
	} while (!result); // RA2: as above
	if (msg->message == WM_QUIT) { // RA2: as above
		result = 0; // RA2: as above
	}
	return result; // RA2: as above
}

/// <summary>
/// The game's window message peek. Recording, it writes the cursor position and every message of a game window to
/// the file; playing back, it takes the messages from the file instead (the escape key ends the playback).
/// </summary>
int Peek_Windows_Message(MSG * msg, HWND window, unsigned filter_min, unsigned filter_max, unsigned remove) // RA2: 0x00522ED0
{
	int result; // RA2: as above

	if (AudioFileMode == 1 && MouseCursor != NULL) { // RA2: as above
		POINT point; // RA2: as above
		GetCursorPos(&point); // RA2: as above
		AudioFileCursorX = point.x; // RA2: as above
		AudioFileCursorY = point.y; // RA2: as above
		MouseCursor->Convert_Coordinate(AudioFileCursorX, AudioFileCursorY); // RA2: as above
		if (AudioFileCursorX != AudioFileLastCursorX || AudioFileCursorY != AudioFileLastCursorY) { // RA2: as above
			AudioFileCursorRecord record; // RA2: as above
			record.Header.Type = 1; // RA2: as above
			record.Header.Count = MessageLoopCount; // RA2: as above
			record.Header.Frame = Frame; // RA2: as above
			EnterCriticalSection(&AudioFileCursorLock); // RA2: as above
			MouseCursor->Set_Mouse_Point(AudioFileCursorX, AudioFileCursorY); // RA2: as above
			record.X = AudioFileCursorX; // RA2: as above
			record.Y = AudioFileCursorY; // RA2: as above
			AudioFileLastCursorX = AudioFileCursorX; // RA2: as above
			AudioFileLastCursorY = AudioFileCursorY; // RA2: as above
			LeaveCriticalSection(&AudioFileCursorLock); // RA2: as above
			STM_AccessTransfer(AudioFileReadQueue, &record, sizeof(record)); // RA2: as above
		}
	}

	for (;;) { // RA2: as above
		if (AudioFileMode == 2) { // RA2: as above
			bool stopped = false; // RA2: as above
			while (PeekMessageA(msg, NULL, 0, 0, PM_REMOVE)) { // RA2: as above
				if (msg->message == WM_KEYDOWN && msg->wParam == VK_ESCAPE) { // RA2: as above
					Audio_File_Close_Stream(); // RA2: the escape key ends the playback
					stopped = true; // RA2: as above
					break; // RA2: as above
				}
				if (msg->message == WM_PAINT) { // RA2: as above
					DispatchMessageA(msg); // RA2: as above
				}
			}
			if (stopped) { // RA2: as above
				continue; // RA2: as above
			}
			for (;;) { // RA2: as above
				if (!AudioFileHeaderRead) { // RA2: as above
					unsigned available = STM_AccessTotalBytes(AudioFileFlushQueue); // RA2: as above
					if (available == 0) { // RA2: as above
						if (AudioFileFillFailed) { // RA2: as above
							Audio_File_Close_Stream(); // RA2: the file has run out
							stopped = true; // RA2: as above
							break; // RA2: as above
						}
						continue; // RA2: as above
					}
					if (available < sizeof(AudioFileRecordHeader)) { // RA2: as above
						continue; // RA2: as above
					}
					STM_AccessTransfer(AudioFileFlushQueue, &AudioFileNextHeader, sizeof(AudioFileRecordHeader)); // RA2: as above
					AudioFileHeaderRead = 1; // RA2: as above
				}
				if (AudioFileNextHeader.Count > (unsigned)MessageLoopCount) { // RA2: as above
					return 0; // RA2: not due yet
				}
				AudioFileHeaderRead = 0; // RA2: as above
				if (AudioFileNextHeader.Type == 1) { // RA2: as above
					POINT point; // RA2: as above
					STM_AccessTransfer(AudioFileFlushQueue, &point, sizeof(point)); // RA2: as above
					EnterCriticalSection(&AudioFileCursorLock); // RA2: as above
					AudioFileCursorX = point.x; // RA2: as above
					AudioFileCursorY = point.y; // RA2: as above
					if (MouseCursor != NULL) { // RA2: as above
						MouseCursor->Set_Mouse_Point(point.x, point.y); // RA2: as above
					}
					LeaveCriticalSection(&AudioFileCursorLock); // RA2: as above
					continue; // RA2: as above
				}
				STM_AccessTransfer(AudioFileFlushQueue, msg, sizeof(MSG)); // RA2: as above
				msg->hwnd = Audio_File_Window((int)msg->hwnd); // RA2: as above
				if (msg->hwnd != NULL) { // RA2: as above
					result = 1; // RA2: as above
					break; // RA2: as above
				}
			}
			if (stopped) { // RA2: as above
				continue; // RA2: as above
			}
		} else { // RA2: as above
			result = PeekMessageA(msg, window, filter_min, filter_max, remove); // RA2: as above
		}
		if (AudioFileMode != 1 || !result) { // RA2: as above
			return result; // RA2: as above
		}
		int index = Audio_File_Find((int)msg->hwnd); // RA2: as above
		if (index >= 0) { // RA2: as above
			AudioFileMessageRecord record; // RA2: as above
			record.Header.Count = MessageLoopCount; // RA2: as above
			record.Header.Type = 0; // RA2: as above
			record.Header.Frame = Frame; // RA2: as above
			record.Message = *msg; // RA2: as above
			record.Message.hwnd = (HWND)index; // RA2: as above
			STM_AccessTransfer(AudioFileReadQueue, &record, sizeof(record)); // RA2: as above
			return result; // RA2: as above
		}
		if (!remove) { // RA2: as above
			PeekMessageA(msg, window, filter_min, filter_max, PM_REMOVE); // RA2: a message of no game window is dropped
		}
	}
}

/// <summary>
/// Gives the cursor position that the recording has (or is playing back), while it is on.
/// </summary>
void __fastcall Audio_File_Get_Cursor(unsigned * x, unsigned * y) // RA2: 0x00523360
{
	if (AudioFileReady && AudioFileMode != 0) { // RA2: as above
		EnterCriticalSection(&AudioFileCursorLock); // RA2: as above
		*x = AudioFileCursorX; // RA2: as above
		*y = AudioFileCursorY; // RA2: as above
		LeaveCriticalSection(&AudioFileCursorLock); // RA2: as above
	}
}

/// <summary>
/// While recording or playing back, stores the state of a key.
/// </summary>
void Set_Key_State(unsigned key, bool release) // RA2: 0x005233A0
{
	if (AudioFileMode != 0) { // RA2: as above
		KeyStates[key & 0xFF] = release ? 0 : 0x8000; // RA2: as above
	}
}

/// <summary>
/// Returns the key state of the recording while it is on, else Windows' own.
/// </summary>
short Get_Key_State(unsigned key) // RA2: 0x005233D0
{
	if (AudioFileMode == 0) { // RA2: as above
		return GetKeyState(key); // RA2: as above
	}
	return KeyStates[key & 0xFF]; // RA2: as above
}

/// <summary>
/// The same for the asynchronous key state.
/// </summary>
short Get_Async_Key_State(unsigned key) // RA2: 0x005233F0
{
	if (AudioFileMode == 0) { // RA2: as above
		return GetAsyncKeyState(key); // RA2: as above
	}
	return KeyStates[key & 0xFF]; // RA2: as above
}
