#define _CRT_SECURE_NO_WARNINGS 1
#include <windows.h>
#include <string>
#include "resource.h"

#define TIMER_ID 1

int x, y;
char buf[100];
char buf2[100];

static HINSTANCE hInst;

HACCEL hAccel = LoadAccelerators(hInst, MAKEINTRESOURCE(IDR_ACCELERATOR5));

// ���������� �������
LRESULT CALLBACK WndProc(HWND, UINT, WPARAM, LPARAM);
ATOM RegMyWindowClass(HINSTANCE, LPCTSTR);

// ������� ��������� ��������� WinMain
int WINAPI WinMain(HINSTANCE hInstance,
	HINSTANCE         hPrevInstance,
	LPSTR             lpCmdLine,
	int               nCmdShow)
{

	// ��� �������� ������
	LPCTSTR lpzClass = TEXT("Window");

	// ����������� ������
	if (!RegMyWindowClass(hInstance, lpzClass))
		return 1;

	int x = 100;
	int y = 100;

	BYTE szWindowName[80];
	BYTE szBuf[80];

	int cb = LoadString(hInstance, IDS_STRING104, (LPSTR)szWindowName, sizeof(szWindowName));
	wsprintf((LPSTR)szBuf, (LPSTR)szWindowName);

	// �������� ����������� ����
	HWND hWnd = CreateWindow(lpzClass,
		(LPSTR)szBuf,
		WS_VISIBLE | WS_SYSMENU | WS_MINIMIZEBOX,
		x, y, 100 * 5, 50 * 5, NULL, NULL,
		hInstance, NULL);

	// ���� ���� �� �������
	if (!hWnd) return 2;

	// ���� ��������� ����������
	MSG msg = { 0 };    // ��������� ���������
	
	while (GetMessage(&msg, NULL, 0, 0)) {
		if (!TranslateAccelerator(hWnd, hAccel, &msg)) {
			TranslateMessage(&msg);
			DispatchMessage(&msg);
		}
	}

	int iGetOk = 0;   // ���������� ���������
	while ((iGetOk = GetMessage(&msg, NULL, 0, 0)) != 0) // ���� ���������
	{
		if (iGetOk == -1) return 3;  // ���� GetMessage ������ ������ - �����
		TranslateMessage(&msg);
		DispatchMessage(&msg);
	}
	return msg.wParam;  // ������������ ��� ���������� ���������
}

// ������� ����������� ������ ����
ATOM RegMyWindowClass(HINSTANCE hInst, LPCTSTR lpzClassName)
{
	WNDCLASS wcWindowClass = { 0 };
	// ����� �-��� ��������� ���������
	wcWindowClass.lpfnWndProc = (WNDPROC)WndProc;
	// ����� ����
	wcWindowClass.style = CS_HREDRAW | CS_VREDRAW;
	// ���������� ���������� ����������
	wcWindowClass.hInstance = hInst;
	// �������� ������
	wcWindowClass.lpszClassName = lpzClassName;
	// �������� �������
	wcWindowClass.hCursor = LoadCursor(NULL, MAKEINTRESOURCE(IDB_PNG1));
	// ������
	wcWindowClass.hIcon = LoadIcon(hInst, MAKEINTRESOURCE(IDI_ICON1));
	// ������
	wcWindowClass.hbrBackground = (HBRUSH)CreateSolidBrush(RGB(255, 255, 255));
	// ���� ���� ������
	return RegisterClass(&wcWindowClass);
	// ����������� ������
}

// ������� ��������� ���������
LRESULT CALLBACK WndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{
	HDC hdc, hCompatibleDC;
	PAINTSTRUCT ps;

	HANDLE hOldBitmap;

	static HANDLE hBitmap;
	static BITMAP Bitmap;

	RECT Rect;

	static int time;
	static int x, y;

	switch (message)
	{

	case WM_CREATE:
	{
		SetTimer(hWnd, TIMER_ID, time, NULL);
		return 0;
	}

	case WM_COMMAND:
	{
		switch (LOWORD(wParam)) 
		{
			case ID_ACCELERATOR40011:
			{
				//MessageBox(hWnd, "up key", "Message", NULL);
				time += 1000;
				InvalidateRect(hWnd, NULL, TRUE);
				return 0;
			}

			case ID_ACCELERATOR40012:
			{
				//MessageBox(hWnd, "down key", "Message", NULL);
				if(time > 0)
					time -= 1000;
				InvalidateRect(hWnd, NULL, TRUE);
				return 0;
			}

			default:
				return 0;
		}

		return 0;
	}

	case WM_MOUSEMOVE:
	{
		x = LOWORD(lParam);
		y = HIWORD(lParam);

		_itoa(x, buf, 10);
		_itoa(y, buf2, 10);

		// _itoa - ������������� ������ ����� � ������
		/*
		x, y
		�����, ������� ��������� �������������.

		buf, buf2
		�����, ���������� ��������� ��������������.

		10
		����, ������������ ��� �������������� value, ������� ������ ���������� � ��������� �� 2 �� 36.
		*/

		InvalidateRect(hWnd, NULL, TRUE);
		// ������� InvalidateRect ��������� ������������� � ������������ ������� ��������� ����. ����������� ������ ������������ ����� ������� ������� ����, ������� ������ ���� ������������.
		//	hWnd - ���������� ����
		//	NULL - ���������� ��������������
		//	TRUE-  ��������� �������

		return 0;
	}

	case WM_PAINT:
	{ 
		GetClientRect(hWnd, &Rect);//��������� ���������� � ������� ������� ����
		hdc = BeginPaint(hWnd, &ps);//��������� ������ ��������� ����������

		TextOut(hdc, 20, 0, "xPos = ", 10); // 20, 0,
		TextOut(hdc, 70, 0, buf, strlen(buf)); // 70, 0
		TextOut(hdc, 95, 0, ", yPos = ", 10); // 95, 0
		TextOut(hdc, 150, 0, buf2, strlen(buf2)); // 150, 0

		TextOut(hdc, 5, 60, "Timer = ", 10);

		char buf3[100];
		wsprintf(buf3, "%d", time);
		TextOut(hdc, 60, 60, buf3, strlen(buf3));

		EndPaint(hWnd, &ps);
		return 0;
	}

	case WM_DESTROY:
	{
		exit(0);
		return 0;
	}

	default:
		return DefWindowProc(hWnd, message, wParam, lParam);
	}
	return 0;
}