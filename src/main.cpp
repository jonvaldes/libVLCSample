#include <windows.h>
#include <stdio.h>
#include <stdlib.h>
#include <malloc.h>

#include "video.cpp"

static LRESULT CALLBACK windowProc(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam) {
    switch(uMsg) {
    case WM_KEYDOWN: {
        int ScanCode = HIWORD(lParam) & 0x1FF;
        switch(ScanCode) {

        case 0x14B: { //KEY_LEFT
            libvlc_time_t T = _libvlc_media_player_get_time(g_mediaPlayer);
            _libvlc_media_player_set_time(g_mediaPlayer, max(T - 10000, 0));

        } break;

        case 0x14D: { //KEY_RIGHT
            libvlc_time_t T = _libvlc_media_player_get_time(g_mediaPlayer);
            _libvlc_media_player_set_time(g_mediaPlayer, T + 10000);
        } break;

        case 0x01: { // KEY_ESC
            ExitProcess(0);
        }

        default: {
        }
        }
        return 0;
    }

    case WM_CLOSE: {
        ExitProcess(0);
    }
    }
    return DefWindowProc(hWnd, uMsg, wParam, lParam);
}

HWND CreateMainWindow() {
    WNDCLASS wndCls;
    ZeroMemory(&wndCls, sizeof(wndCls));
    wndCls.lpfnWndProc = (WNDPROC)windowProc;
    wndCls.hInstance = GetModuleHandleA(NULL);
    wndCls.style = CS_OWNDC | CS_HREDRAW | CS_VREDRAW;
    wndCls.lpszClassName = "SampleWND";
    RegisterClass(&wndCls);

    HWND Result = CreateWindow("SampleWND", "", WS_CAPTION | WS_CLIPSIBLINGS | WS_CLIPCHILDREN | WS_VISIBLE | WS_BORDER |
                                                    WS_SYSMENU | WS_THICKFRAME | WS_MAXIMIZEBOX | WS_MINIMIZEBOX,
                               0, 0, 0, 0, 0, 0, GetModuleHandleA(NULL), 0);
    SetWindowPos(Result, HWND_TOP, 100, 100, 1280, 745, 0);
    return Result;
}

int _stdcall WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int nCmdShow) {

    if(__argc != 2) {
        MessageBox(NULL, "Usage: main.exe <videofile>", "Wrong params", MB_OK);
        ExitProcess(0);
    }
    startVideo(CreateMainWindow(), __argv[1]);

    for(;;) {
        MSG msg = {0};
        while(PeekMessage(&msg, 0, 0, 0, PM_REMOVE)) {
            TranslateMessage(&msg);
            DispatchMessage(&msg);
        }

        Sleep(16);
    }
}
