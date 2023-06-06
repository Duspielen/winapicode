
#define _CRT_SECURE_NO_WARNINGS
#include <windows.h>
#include <cstdio>
#include "menu.h"

#define ID_EDIT 1

LRESULT CALLBACK WndProc(HWND, UINT, WPARAM, LPARAM);

HINSTANCE hInst;

static BOOL colorBack;

void FileMenu(HWND hwnd)
{
	HMENU hMenuFile = CreateMenu();
	HMENU hPopupMenuFile = CreatePopupMenu();
	HMENU hPopupMenuText = CreatePopupMenu();

	AppendMenu(hMenuFile, MF_STRING | MF_POPUP, (UINT)hPopupMenuFile, "&File");
	{
		AppendMenu(hPopupMenuFile, MF_STRING, OpenFunction, "Open");
		AppendMenu(hPopupMenuFile, MF_STRING, CloseFunction, "Close");
		AppendMenu(hPopupMenuFile, MF_STRING, ExitFunction, "Exit");
		AppendMenu(hPopupMenuFile, MF_STRING, InfoFunction, "Info");
	}

	AppendMenu(hMenuFile, MF_STRING | MF_POPUP, (UINT)hPopupMenuText, "&Text");
	{
		AppendMenu(hPopupMenuText, MF_STRING, ColFunction, "ColorT");
		AppendMenu(hPopupMenuText, MF_STRING, ZalFunction, "Zalivka");
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

	static BOOL colorText;
	static BOOL function;

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
		switch (wParam)
		{
		case OpenFunction:
		{
			MessageBox(hwnd, "Open", "", MB_OK);
			return 0;
		}

		case CloseFunction:
		{
			MessageBox(hwnd, "Close", "", MB_OK);
			return 0;
		}

		case ExitFunction:
		{
			exit(0);
			return 0;
		}

		case InfoFunction:
		{
			char buf[MAX_PATH];

			char path[MAX_PATH];
			char winDirectory[MAX_PATH];
			char systemDirectory[MAX_PATH];

			GetCurrentDirectory(sizeof(path), path);
			GetWindowsDirectory(winDirectory, MAX_PATH);
			GetSystemDirectory(systemDirectory, MAX_PATH);

			sprintf(buf, "Текущая директория: %s \nДиректория Window: %s\nСистемная директория: %s", path, winDirectory, systemDirectory);
			MessageBox(hwnd, buf, "Directory info", MB_OK);
			return 0;
		}

		case ColFunction:
		{
			colorText = TRUE;
			InvalidateRect(hEdit, NULL, TRUE);
			return 0;
		}

		case ZalFunction:
		{
			colorBack = TRUE;
			InvalidateRect(hEdit, NULL, TRUE);
			return 0;
		}

		default:
			break;
		}
		return 0;
	}

	case WM_CTLCOLOREDIT:
	{
		if (colorText && (HWND)lParam == hEdit)
		{
			SetTextColor((HDC)wParam, RGB(255, 0, 0));
			return 0;
		}

		if (colorBack && (HWND)lParam == hEdit)
		{
			SetBkColor((HDC)wParam, RGB(255, 0, 0));
			return 0;
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
