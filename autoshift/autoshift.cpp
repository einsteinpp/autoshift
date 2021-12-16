#include <stdio.h>
#include "constants.h"

KeyState activekey{ NULL, NULL };

INT WINAPI WinMain(
    _In_ HINSTANCE hinstance,
    _In_opt_ HINSTANCE hPrevInstance,
    _In_ LPSTR lpCmdLine,
    _In_ int nCmdShow)
{
    HHOOK hhkLowLevelKybd = SetWindowsHookEx(WH_KEYBOARD_LL, LowLevelKeyboardProc, 0, 0);
    MSG msg;

    while (!GetMessage(&msg, NULL, NULL, NULL)) {
        TranslateMessage(&msg);
        DispatchMessage(&msg);
    }

    UnhookWindowsHookEx(hhkLowLevelKybd);

    return 0;
}

LRESULT CALLBACK LowLevelKeyboardProc(int nCode, WPARAM wParam, LPARAM lParam)
{
    BOOL fEatKeystroke = FALSE;

    if (nCode == HC_ACTION)
    {
        switch (wParam)
        {
        case WM_KEYDOWN:
        case WM_SYSKEYDOWN:
        case WM_KEYUP:
        case WM_SYSKEYUP:
            PKBDLLHOOKSTRUCT p = (PKBDLLHOOKSTRUCT)lParam;
            if (fEatKeystroke = IsInArray(keysToListen, p->vkCode))
            {
                if (activekey.keyCode == NULL && ((wParam == WM_KEYDOWN) || (wParam == WM_SYSKEYDOWN)))
                {
                    activekey.keyCode = p->vkCode;
                    activekey.begin = clock();
                }
                else if (activekey.keyCode != NULL && ((wParam == WM_KEYUP) || (wParam == WM_SYSKEYUP)))
                {
                    clock_t end = clock();
                    double time_spent = ((double)end - (double)activekey.begin) / CLOCKS_PER_SEC;
                    if (time_spent > 0.175)
                    {
                        keybd_event(VK_CONTROL, 0, 0, 0);
                        keybd_event(VK_MENU, 0, 0, 0);
                        keybd_event(p->vkCode, 0, 0, 0);
                        keybd_event(p->vkCode, 0, KEYEVENTF_KEYUP, 0);
                        keybd_event(VK_CONTROL, 0, KEYEVENTF_KEYUP, 0);
                        keybd_event(VK_MENU, 0, KEYEVENTF_KEYUP, 0);
                    }
                    else {
                        keybd_event(p->vkCode, 0, 0, 0);
                        keybd_event(p->vkCode, 0, KEYEVENTF_KEYUP, 0);
                    }
                    activekey.begin = NULL;
                    activekey.keyCode = NULL;
                }
            }
            break;
        }
    }
    return (fEatKeystroke ? 1 : CallNextHookEx(NULL, nCode, wParam, lParam));
}

BOOL IsInArray(DWORD array[], DWORD index)
{
    int arraySize = sizeof(keysToListen) / sizeof(DWORD);

    for (int i = 0; i < arraySize; i++)
        if (index == array[i]) return TRUE;

    return FALSE;
}
