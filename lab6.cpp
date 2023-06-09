#include <windows.h>

#define STRICT 1
#define ID_LIST 1
#define ID_BUTTON 2
#define ID_BUTTON_CANCEL 2

LRESULT CALLBACK WndProc(HWND, UINT, WPARAM, LPARAM);

HINSTANCE hInst;

int WINAPI WinMain(
	HINSTANCE hInstance,
	HINSTANCE hPrevInstance,
	LPSTR lpszCmdLine,
	int nCmdShow)
{
	MSG msg;
	HWND hwnd;
	WNDCLASS wc;

	//регистрация окна
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
	//создание окна
	hwnd = CreateWindow(
		"WindowsClass",
		"Select...",
		WS_VISIBLE | WS_SYSMENU | WS_MINIMIZEBOX,
		500, 200,
		100 * 3, 50 * 5,
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
	static HWND hListBox;
	static HWND hButton;
	static HWND hButtonCancel;

	WIN32_FIND_DATA wfd;
	HANDLE f;

	f = FindFirstFile(LPCSTR("D:/*"), &wfd);  // путь к файлу

	switch (msg)
	{
	case WM_CREATE:
	{
		// создание листа
		hListBox = CreateWindow(
			"listBox",
			NULL,
			WS_CHILD | WS_VISIBLE | LBS_STANDARD ,
			30, 30,
			230, 100,
			hwnd,
			(HMENU)ID_LIST,
			hInst,
			NULL);

		// поиск файла
		if (INVALID_HANDLE_VALUE != f)
		{
			do
			{
				SendMessage(hListBox, LB_ADDSTRING, 0, (LPARAM)&wfd.cFileName[0]);
			} while (FindNextFile(f, &wfd));
			FindClose(f);
		}

		InvalidateRect(hListBox, NULL, TRUE);
		//кнопки
		hButton = CreateWindow(
			"button",
			"OK",
			WS_CHILD | WS_VISIBLE | BS_PUSHBUTTON,
			30, 150,
			50, 20,
			hwnd,
			(HMENU)ID_BUTTON,
			hInst,
			NULL);

		hButtonCancel = CreateWindow(
			"button",
			"Cancel",
			WS_CHILD | WS_VISIBLE | BS_PUSHBUTTON,
			200, 150,
			50, 20,
			hwnd,
			(HMENU)ID_BUTTON_CANCEL,
			hInst,
			NULL);

		return 0;
	}

	case WM_SETFOCUS:
	{
		SetFocus(hListBox);
		return 0;
	}

	case WM_COMMAND:
	{
		if (wParam == ID_LIST)
		{
			if (HIWORD(lParam) == (unsigned)LBN_ERRSPACE)
			{
				MessageBox(hwnd, "low memory", "Dialog", MB_OK);
			}

			else if (HIWORD(lParam) == LBN_DBLCLK)
			{
				int uSelectedItem;
				char Buffer[256];
				uSelectedItem = (int)SendMessage(hListBox, LB_GETCURSEL, 0, 0L);

				if (uSelectedItem != LB_ERR)
				{
					SendMessage(hListBox, LB_GETTEXT, uSelectedItem, (LPARAM)Buffer);
					MessageBox(hwnd, (LPSTR)Buffer, "Dialog", MB_OK);
				}
			}
			else if (HIWORD(lParam) == LBN_SELCANCEL)
			{
				int uSelectedItem, nSize;
				char Buffer[256];
				HDC hdc;

				uSelectedItem = (int)SendMessage(hListBox, LB_GETCURSEL, 0, 0L);

				if (uSelectedItem != LB_ERR)
				{
					SendMessage(hListBox, LB_GETTEXT, uSelectedItem, (LPARAM)Buffer);
					hdc = GetDC(hwnd);
					nSize = lstrlen(Buffer);
					TextOut(hdc, 250, 60, (LPSTR)" ", 25);
					TextOut(hdc, 250, 60, (LPSTR)Buffer, 25);
					ReleaseDC(hwnd, hdc);
				}
			}
		}
		//обработка кнопки
		else if (wParam == ID_BUTTON)
		{
			int uSelectItem;
			char Buffer[256];
			uSelectItem = (int)SendMessage(hListBox, LB_GETCURSEL, 0, 0L);
			if (uSelectItem != LB_ERR)
			{
				SendMessage(hListBox, LB_GETTEXT, uSelectItem, (LPARAM)Buffer);
				MessageBox(hwnd, (LPSTR)Buffer, "", MB_OK);
			}
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
