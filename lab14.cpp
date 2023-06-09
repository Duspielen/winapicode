#define _CRT_SECURE_NO_WARNINGS 1
#include <windows.h>

DWORD WINAPI ThreadWorkA(LPVOID param);
DWORD WINAPI ThreadWorkB(LPVOID param);
DWORD WINAPI ThreadWorkC(LPVOID param);

LRESULT CALLBACK WndProc(HWND, UINT, WPARAM, LPARAM);

HINSTANCE hInst;

int counterA = 0;
int counterB = 0;
int counterC = 0;

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
	HDC hdc;
	PAINTSTRUCT ps;
	RECT Rect;

	HANDLE thread[3];

	thread[0] = CreateThread(
		NULL,
		0,
		ThreadWorkB,
		NULL,
		0,
		0);

	thread[1] = CreateThread(
		NULL,
		0,
		ThreadWorkA,
		NULL,
		0,
		0);

	thread[2] = CreateThread(
		NULL,
		0,
		ThreadWorkC,
		NULL,
		0,
		0);


	WaitForMultipleObjects(3, thread, TRUE, INFINITE);

	for (DWORD i = 0; i < 3; ++i)
		CloseHandle(thread[i]);

	switch (msg)
	{

		case WM_PAINT:
		{
			GetClientRect(hwnd, &Rect);
			hdc = BeginPaint(hwnd, &ps);

			char buf[100];
			char buf1[100];
			char buf2[100];

			_itoa(counterA, buf, 10);
			_itoa(counterB, buf1, 10);
			_itoa(counterC, buf2, 10);

			TextOut(hdc, 20, 20, "Thread A: count = ", 20);
			TextOut(hdc, 20, 60, "Thread B: count = ", 20);
			TextOut(hdc, 20, 100, "Thread C: count = ", 20);
			TextOut(hdc, 130, 20, buf, strlen(buf));
			TextOut(hdc, 130, 60, buf1, strlen(buf1));
			TextOut(hdc, 130, 100, buf2, strlen(buf2));

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

DWORD WINAPI ThreadWorkA(LPVOID param)
{
	while(counterA <= 100)
		counterA += 10;

	ExitThread(0);
}

DWORD WINAPI ThreadWorkB(LPVOID param)
{
	while (counterB <= 50)
	{
		counterB += 5;

	}

	ExitThread(0);
}

DWORD WINAPI ThreadWorkC(LPVOID param)
{
	while (counterC <= 50)
	{
		counterC += 90;
	}

	ExitThread(0);
}
