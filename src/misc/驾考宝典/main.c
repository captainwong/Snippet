#if 1
#include <windows.h>
#include <stdio.h>



// 记录光标位置，移动光标到“上一题”，“下一题”，点击，移动光标到原位置
void click(int x, int y) 
{
    POINT pt;
    GetCursorPos(&pt);
    SetCursorPos(x, y);
    INPUT inputs[2] = { 0 };
    inputs[0].type = inputs[1].type = INPUT_MOUSE;
    inputs[0].mi.dwFlags = MOUSEEVENTF_LEFTDOWN;
    inputs[1].mi.dwFlags = MOUSEEVENTF_LEFTUP;
    SendInput(2, &inputs[0], sizeof(INPUT));
    SetCursorPos(pt.x, pt.y);
}

int wmain(int argc, wchar_t* argv[])
{
    if (!RegisterHotKey(NULL, 1, MOD_ALT | MOD_NOREPEAT, 'B'))
    {
        return 0;
    }

    if (!RegisterHotKey(NULL, 1, MOD_ALT | MOD_NOREPEAT, 'N'))
    {
        return 0;
    }

    wprintf(L"Hotkey 'alt+b' and 'alt+n' registered, using MOD_NOREPEAT flag\n");

    MSG msg;
    while (GetMessage(&msg, NULL, 0, 0)) {
        if (msg.message == WM_HOTKEY) {
            wprintf(L"WM_HOTKEY received, 0x%08X 0x%08X\n", msg.wParam, msg.lParam);
            //Sleep(500);
            //click();

            if (HIWORD(msg.lParam) == 'B') {
                click(900, 700);
            } else if (HIWORD(msg.lParam) == 'N') {
                click(1025, 700);
            }
        }
    }

    return 0;
}
#else

#include <windows.h>

#define MY_TITLE L"HotKey"

LRESULT CALLBACK WndProc(HWND, UINT, WPARAM, LPARAM);

//
//  FUNCTION: MyRegisterClass()
//
//  PURPOSE: Registers the window class.
//
ATOM MyRegisterClass(HINSTANCE hInstance)
{
    WNDCLASSEXW wcex;

    wcex.cbSize = sizeof(WNDCLASSEX);

    wcex.style = CS_HREDRAW | CS_VREDRAW;
    wcex.lpfnWndProc = WndProc;
    wcex.cbClsExtra = 0;
    wcex.cbWndExtra = 0;
    wcex.hInstance = hInstance;
    wcex.hIcon = NULL;
    wcex.hCursor = LoadCursor(NULL, IDC_ARROW);
    wcex.hbrBackground = (HBRUSH)(COLOR_WINDOW + 1);
    wcex.lpszMenuName = NULL;
    wcex.lpszClassName = MY_TITLE;
    wcex.hIconSm = NULL;

    return RegisterClassExW(&wcex);
}

int APIENTRY wWinMain(HINSTANCE inst, HINSTANCE prevInst, LPWSTR cmd, int show)
{
    MyRegisterClass(inst);

    HWND hwnd = CreateWindow(MY_TITLE, MY_TITLE, WS_OVERLAPPEDWINDOW, CW_USEDEFAULT, 0, CW_USEDEFAULT, 0, NULL, NULL, inst, NULL);
    if (!hwnd) {
        return 0;
    }

    ShowWindow(hwnd, SW_HIDE);
    UpdateWindow(hwnd);

    MSG msg;
    // Main message loop:
    while (GetMessage(&msg, NULL, 0, 0)) {        
        TranslateMessage(&msg);
        DispatchMessage(&msg);        
    }

    return (int)msg.wParam;
}

LRESULT CALLBACK WndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{
    switch (message) {

    case WM_DESTROY:
        PostQuitMessage(0);
        break;
    default:
        return DefWindowProc(hWnd, message, wParam, lParam);
    }
    return 0;
}
#endif