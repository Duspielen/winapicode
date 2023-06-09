#define _CRT_SECURE_NO_WARNINGS

#include <windows.h>

BOOL RegClass(WNDPROC, HINSTANCE, LPCSTR, UINT);

LRESULT CALLBACK WndProcMain(HWND, UINT, WPARAM, LPARAM);
LRESULT CALLBACK WndProcPopup(HWND, UINT, WPARAM, LPARAM);

LPCSTR lpszNameClass = "MyClassMain";
LPCSTR lpszNameClassPopup = "MyClassPoppup";
char szTitle[] = "Window";
HINSTANCE hInstance;

int WINAPI WinMain(HINSTANCE hInst, HINSTANCE hPrevInstance, LPSTR lpszCmdLine, int nCmdShow) // главная функция
{
    MSG msg;
    HWND hwnd;
    hInstance = hInst;
    if (!RegClass(WndProcMain, hInstance, lpszNameClass, COLOR_DESKTOP))
        return FALSE;
    if (!RegClass(WndProcPopup, hInstance, lpszNameClassPopup, COLOR_BTNFACE))
        return FALSE;
    hwnd = CreateWindow(lpszNameClass, szTitle, WS_OVERLAPPEDWINDOW | WS_VISIBLE,
        CW_USEDEFAULT, 0, CW_USEDEFAULT, 0, 0, 0, hInstance, 0);
    if (!hwnd)
        return FALSE;
    ShowWindow(hwnd, nCmdShow);
    while (GetMessage(&msg, 0, 0, 0))  DispatchMessage(&msg);
    return msg.wParam;

}
BOOL RegClass(WNDPROC wproc, HINSTANCE hInstance, LPCSTR lpszClassName, UINT hbrBackground) // регистрация окна
{
    WNDCLASS new_class;
    new_class.cbClsExtra = new_class.cbWndExtra = 0;
    new_class.style = CS_VREDRAW | CS_HREDRAW;
    new_class.lpszMenuName = NULL;
    new_class.lpszClassName = lpszClassName;
    new_class.hbrBackground = (HBRUSH)(hbrBackground + 1);
    new_class.hCursor = LoadCursor(NULL, IDC_ARROW);
    new_class.hInstance = hInstance;
    new_class.hIcon = LoadIcon(NULL, IDI_APPLICATION);
    new_class.lpfnWndProc = wproc;
    return (RegisterClass(&new_class) != 0);
}
LRESULT CALLBACK WndProcPopup(HWND hWnd, UINT messages, WPARAM wParam, LPARAM lParam) // обработчик событыий
{
    return DefWindowProc(hWnd, messages, wParam, lParam); // обработка каждого сообщения

}
LRESULT CALLBACK WndProcMain(HWND hWnd, UINT messages, WPARAM wParam, LPARAM lParam) // обработчик событыий
{
    static short cxClient, cyClient, yStep, xStep;
    static short left, top, Width, Height;
    static HWND hwnds[5];
    switch (messages)
    {
    case WM_CREATE:
    {
        yStep = GetSystemMetrics(SM_CYCAPTION);
        return 0;
    }
    case WM_MOVE:
    {
        left = LOWORD(lParam);
        top = HIWORD(lParam);
        for (short j = 0; j < 5; j++)
            if (IsWindow(hwnds[j]))
                MoveWindow(hwnds[j],
                    left + cxClient - Width - xStep * j,
                    top + cyClient - Height - yStep * j,
                    Width, Height, TRUE);
        return 0;
    }
    case WM_SIZE:
    {
        cxClient = LOWORD(lParam);
        cyClient = HIWORD(lParam);
        Width = cxClient / 2;
        Height = cyClient - 4 * yStep;
        xStep = (cxClient - Width) / 4;
        for (short j = 0; j < 5; j++)
            if (IsWindow(hwnds[j]))
                MoveWindow(hwnds[j],
                    left + cxClient - Width - xStep * j,
                    top + cyClient - Height - yStep * j,
                    Width, Height, TRUE);
        return 0;
    }
    case WM_LBUTTONDOWN:
    {
        short j;
        for (j = 0; (j < 5 && IsWindow(hwnds[j])); j++);
        if (j > 4)
            return 0;
        char str[20];
        _itoa(j + 1, str, 10);
        strcat(str, " Window");
        hwnds[j] = CreateWindow(lpszNameClassPopup, str, WS_POPUPWINDOW | WS_CAPTION | WS_VISIBLE,
            left + cxClient - Width - xStep * j, top + cyClient - Height - yStep * j,
            Width, Height, hWnd, 0, hInstance, NULL);
        if (j > 0)
        {
            SetWindowPos(hwnds[j], hwnds[j - 1], 0, 0, Width, Height, SWP_NOMOVE);
            SetForegroundWindow(hwnds[0]);

        } return 0;
    }
    case WM_DESTROY:
    {
        PostQuitMessage(0);
        return 0;
    }
    }
    return DefWindowProc(hWnd, messages, wParam, lParam);
}
