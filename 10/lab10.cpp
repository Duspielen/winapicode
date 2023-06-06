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

// объявление функций
LRESULT CALLBACK WndProc(HWND, UINT, WPARAM, LPARAM);
ATOM RegMyWindowClass(HINSTANCE, LPCTSTR);

// функция вхождений программы WinMain
int WINAPI WinMain(HINSTANCE hInstance,
	HINSTANCE         hPrevInstance,
	LPSTR             lpCmdLine,
	int               nCmdShow)
{

	// имя будущего класса
	LPCTSTR lpzClass = TEXT("Window");

	// регистрация класса
	if (!RegMyWindowClass(hInstance, lpzClass))
		return 1;

	int x = 100;
	int y = 100;

	BYTE szWindowName[80];
	BYTE szBuf[80];

	int cb = LoadString(hInstance, IDS_STRING104, (LPSTR)szWindowName, sizeof(szWindowName));
	wsprintf((LPSTR)szBuf, (LPSTR)szWindowName);

	// создание диалогового окна
	HWND hWnd = CreateWindow(lpzClass,
		(LPSTR)szBuf,
		WS_VISIBLE | WS_SYSMENU | WS_MINIMIZEBOX,
		x, y, 100 * 5, 50 * 5, NULL, NULL,
		hInstance, NULL);

	// если окно не создано
	if (!hWnd) return 2;

	// цикл сообщений приложения
	MSG msg = { 0 };    // структура сообщения
	
	while (GetMessage(&msg, NULL, 0, 0)) {
		if (!TranslateAccelerator(hWnd, hAccel, &msg)) {
			TranslateMessage(&msg);
			DispatchMessage(&msg);
		}
	}

	int iGetOk = 0;   // переменная состояния
	while ((iGetOk = GetMessage(&msg, NULL, 0, 0)) != 0) // цикл сообщений
	{
		if (iGetOk == -1) return 3;  // если GetMessage вернул ошибку - выход
		TranslateMessage(&msg);
		DispatchMessage(&msg);
	}
	return msg.wParam;  // возвращается код завершения программы
}

// функция регистрации класса окон
ATOM RegMyWindowClass(HINSTANCE hInst, LPCTSTR lpzClassName)
{
	WNDCLASS wcWindowClass = { 0 };
	// адрес ф-ции обработки сообщений
	wcWindowClass.lpfnWndProc = (WNDPROC)WndProc;
	// стиль окна
	wcWindowClass.style = CS_HREDRAW | CS_VREDRAW;
	// дискриптор экземпляра приложения
	wcWindowClass.hInstance = hInst;
	// название класса
	wcWindowClass.lpszClassName = lpzClassName;
	// загрузка курсора
	wcWindowClass.hCursor = LoadCursor(NULL, MAKEINTRESOURCE(IDB_PNG1));
	// курсор
	wcWindowClass.hIcon = LoadIcon(hInst, MAKEINTRESOURCE(IDI_ICON1));
	// иконка
	wcWindowClass.hbrBackground = (HBRUSH)CreateSolidBrush(RGB(255, 255, 255));
	// цвет фона экрана
	return RegisterClass(&wcWindowClass);
	// регистрация класса
}

// функция обработки сообщений
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

		// _itoa - преоброзвание целого числа в строку
		/*
		x, y
		Число, которое требуется преобразовать.

		buf, buf2
		Буфер, содержащий результат преобразования.

		10
		База, используемая для преобразования value, которая должна находиться в диапазоне от 2 до 36.
		*/

		InvalidateRect(hWnd, NULL, TRUE);
		// Функция InvalidateRect добавляет прямоугольник к обновляемому региону заданного окна. Обновляемый регион представляет часть рабочей области окна, которая должна быть перерисована.
		//	hWnd - дескриптор окна
		//	NULL - координаты прямоугольника
		//	TRUE-  состояние очистки

		return 0;
	}

	case WM_PAINT:
	{ 
		GetClientRect(hWnd, &Rect);//Получение информации о рабочей области окна
		hdc = BeginPaint(hWnd, &ps);//Получение хендла контекста устройства

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
