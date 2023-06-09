#include <windows.h>
#include "menu.h"

#define ID_EDIT 1
#define ID_OPEN 2
#define ID_CLOSE 3
#define ID_EXIT 4
#define ID_COLORT 5
#define ID_ZALIVKA 6

LRESULT CALLBACK WndProc(HWND, UINT, WPARAM, LPARAM);

HINSTANCE hInst;

void FileMenu(HWND hwnd)
{
    HMENU hMenuFile = CreateMenu();
    HMENU hPopupMenuFile = CreatePopupMenu();
    AppendMenu(hMenuFile, MF_STRING | MF_POPUP, (UINT_PTR)hPopupMenuFile, "&File");
    {
        AppendMenu(hPopupMenuFile, MF_STRING, ID_OPEN, "Open");
        AppendMenu(hPopupMenuFile, MF_STRING, ID_CLOSE, "Close");
        AppendMenu(hPopupMenuFile, MF_STRING, ID_EXIT, "Exit");
    }
    HMENU hPopupMenuText = CreatePopupMenu();
    AppendMenu(hMenuFile, MF_STRING | MF_POPUP, (UINT_PTR)hPopupMenuText, "&Text");
    {
        AppendMenu(hPopupMenuText, MF_STRING, ID_COLORT, "ColorT");
        AppendMenu(hPopupMenuText, MF_STRING, ID_ZALIVKA, "Zalivka");
    }
    SetMenu(hwnd, hMenuFile);
}

int WINAPI WinMain(
        HINSTANCE hInstance,
        HINSTANCE hPrevInstance,
        LPSTR lpszCmdLine,
        int nCmdShow)
{
    MSG msg;
    HWND hwnd;
    WNDCLASS wc;
    HMENU hMenu;
    HMENU hPopupMenuMAIN, hPopupMenuTEXT;

    wc.style = CS_HREDRAW | CS_VREDRAW;
    wc.cbClsExtra = wc.cbWndExtra = 0;
    wc.lpfnWndProc = WndProc;
    wc.hInstance = hInstance;
    wc.hIcon = LoadIcon(NULL, IDI_APPLICATION);
    wc.hCursor = LoadCursor(NULL, IDC_ARROW);
    wc.hbrBackground = (HBRUSH)(COLOR_WINDOW + 1);
    wc.lpszMenuName = (LPCTSTR)NULL;
    wc.lpszClassName = "WindowsClass";

    if (!RegisterClass(&wc))
    {
        MessageBox(NULL, "reg class", "Error", MB_OK);
        return 0;
    }

    hInst = hInstance;
    hwnd = CreateWindow(
            "WindowsClass",
            "Program No",
            WS_VISIBLE | WS_SYSMENU | WS_MINIMIZEBOX,
            500, 200,
            100 * 4, 50 * 6,
            0,
            0,
            hInstance,
            NULL);

    if (!hwnd)
    {
        MessageBox(NULL, "create window", "Error", MB_OK);
        return 0;
    }

    ShowWindow(hwnd, nCmdShow);
    UpdateWindow(hwnd);

    while (GetMessage(&msg, 0, 0, 0))
    {
        TranslateMessage(&msg);
        DispatchMessage(&msg);
    }
    return msg.wParam;
}

LRESULT CALLBACK WndProc(
        HWND hwnd,
        UINT msg,
        WPARAM wParam,
        LPARAM lParam)
{
    static HWND hEdit;
    static HWND hListBox;

    switch (msg)
    {
        case WM_CREATE:
        {
            hEdit = CreateWindow(
                    "edit",
                    NULL,
                    WS_CHILD | WS_VISIBLE | ES_MULTILINE,
                    0, 0,
                    192 * 2, 121 * 2,
                    hwnd,
                    (HMENU)ID_EDIT,
                    hInst,
                    NULL);
            InvalidateRect(hEdit, NULL, TRUE);

            FileMenu(hwnd);
            return 0;
        }

        case WM_COMMAND:
        {
            switch (LOWORD(wParam))
            {
                case ID_OPEN:
                {
                    MessageBox(hwnd, "Open", "", MB_OK);
                    return 0;
                }

                case ID_CLOSE:
                {
                    MessageBox(hwnd, "Close", "", MB_OK);
                    return 0;
                }

                case ID_EXIT:
                {
                    exit(0);
                    return 0;
                }

                case ID_COLORT:
                {
                    HBRUSH hBrush = CreateSolidBrush(RGB(255, 0, 0)); // Создаем кисть с желаемым цветом фона
                    SetClassLongPtr(hEdit, GCLP_HBRBACKGROUND, (LONG_PTR)hBrush); // Устанавливаем кисть в качестве фона

                    HFONT hFont = (HFONT)GetStockObject(DEFAULT_GUI_FONT); // Получаем стандартный шрифт
                    SendMessage(hEdit, WM_SETFONT, (WPARAM)hFont, TRUE); // Устанавливаем шрифт для текстового поля

                    InvalidateRect(hEdit, NULL, TRUE); // Перерисовываем текстовое поле
                    return 0;
                }

                case ID_ZALIVKA:
                {
                    MessageBox(hwnd, "Zalivka", "", MB_OK);
                    return 0;
                }

                default:
                    break;
            }
            return 0;
        }

        case WM_DESTROY:
        {
            PostQuitMessage(0);
            return 0;
        }
    }

    return DefWindowProc(hwnd, msg, wParam, lParam);
}
