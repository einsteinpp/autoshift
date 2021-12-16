#pragma once

#include <Windows.h>
#include <time.h>

DWORD keysToListen[] = {
	0x30,
	0x31,
	0x32,
	0x33,
	0x34,
	0x35,
	0x36,
	0x37,
	0x38,
	0x39,
	0xBA, // $£¤
	0xBB, // =+}
	0xDB  // )°]
};

typedef struct KeyState KeyState;
struct KeyState
{
    DWORD keyCode;
    clock_t begin;
};

LRESULT CALLBACK LowLevelKeyboardProc(int nCode, WPARAM wParam, LPARAM lParam);

BOOL IsInArray(DWORD array[], DWORD index);
