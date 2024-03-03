#include <windows.h>
#include <stdio.h>
#include <time.h>
#include <mmsystem.h>

#pragma comment(lib, "Winmm.lib")

#define ID_TIMER 1

LRESULT WINAPI ScreenSaverProc(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam)
{
    switch (msg)
    {
        case WM_CREATE:
            SetTimer(hWnd, ID_TIMER, 1000, NULL); // Timer fires every second
            return 0;

        case WM_TIMER:
        {
            time_t now;
            struct tm *info;
            time(&now);
            info = localtime(&now);

            // Calculate time remaining before sleep mode
            SYSTEM_POWER_STATUS powerStatus;
            GetSystemPowerStatus(&powerStatus);
            int timeRemaining = powerStatus.BatteryLifeTime / 60; // Convert seconds to minutes

            // Draw countdown on screen
            HDC hdc = GetDC(hWnd);
            SetBkColor(hdc, RGB(0, 0, 0)); // Black background
            SetTextColor(hdc, RGB(255, 0, 0)); // Red text
            char countdownStr[20];
            sprintf(countdownStr, "%02d:%02d:%02d", info->tm_hour, info->tm_min, timeRemaining);
            TextOut(hdc, 10, 10, countdownStr, strlen(countdownStr));
            ReleaseDC(hWnd, hdc);

            // Play .wav file when countdown reaches zero
            if (timeRemaining <= 0)
                PlaySound(TEXT("tick.wav"), NULL, SND_FILENAME | SND_ASYNC);

            return 0;
        }

        case WM_DESTROY:
            KillTimer(hWnd, ID_TIMER);
            PostQuitMessage(0);
            return 0;
    }
    return DefScreenSaverProc(hWnd, msg, wParam, lParam);
}

BOOL WINAPI ScreenSaverConfigureDialog(HWND hDlg, UINT message, WPARAM wParam, LPARAM lParam)
{
    return FALSE; // No configuration dialog
}

BOOL WINAPI RegisterDialogClasses(HANDLE hInst)
{
    return TRUE;
}

int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int nCmdShow)
{
    WNDCLASS wc;
    HWND hWnd;
    MSG msg;

    wc.style = CS_HREDRAW | CS_VREDRAW;
    wc.lpfnWndProc = ScreenSaverProc;
    wc.cbClsExtra = 0;
    wc.cbWndExtra = 0;
    wc.hInstance = hInstance;
    wc.hIcon = LoadIcon(NULL, IDI_APPLICATION);
    wc.hCursor = LoadCursor(NULL, IDC_ARROW);
    wc.hbrBackground = (HBRUSH)GetStockObject(BLACK_BRUSH);
    wc.lpszMenuName = NULL;
    wc.lpszClassName = "ScreenSaverClass";

    if (!RegisterClass(&wc))
        return 0;

    hWnd = CreateWindow(
        "ScreenSaverClass",        // class name
        "Screen Saver",            // window name
        WS_POPUP,                  // window style
        0,                         // horizontal position
        0,                         // vertical position
        GetSystemMetrics(SM_CXSCREEN), // screen width
        GetSystemMetrics(SM_CYSCREEN), // screen height
        NULL,                      // parent window
        NULL,                      // menu
        hInstance,                 // instance handle
        NULL);                     // additional application data

    if (!hWnd)
        return 0;

    ShowWindow(hWnd, SW_SHOW);

    while (GetMessage(&msg, NULL, 0, 0))
    {
        TranslateMessage(&msg);
        DispatchMessage(&msg);
    }

    return msg.wParam;
}
