#include <windows.h>
#include <math.h>

// объявление функций
LRESULT CALLBACK WndProc(HWND, UINT, WPARAM, LPARAM);
ATOM RegMyWindowClass(HINSTANCE, LPCTSTR);

#define Pi 3.14159265

double finSizeB = 3.0;
double finSizeGraphB = 5.0;

static double size;
static double dsize;
static double sizeGraph;
static double dsizeGraph;
static double finSize;
static double dfinSize;
static double finSizeGraph;
static double dfinSizeGraph;

static BOOL key;
static BOOL mouse;
static BOOL left, right;

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

    // вычисление координат центра экрана
    RECT screen_rect;
    GetWindowRect(GetDesktopWindow(), &screen_rect); // разрешение экрана
    int x = 20;
    int y = 10;

    // создание диалогового окна
    HWND hWnd = CreateWindow(lpzClass, TEXT("График y=x^2"),
        WS_VISIBLE | WS_SYSMENU | WS_MINIMIZEBOX,
        x, y, 300 * 5, 150 * 5, NULL, NULL,
        hInstance, NULL);

    // если окно не создано, описатель будет равен 0
    if (!hWnd)
        return 2;

    // цикл сообщений приложения
    MSG msg = { 0 };    // структура сообщения
    int iGetOk = 0;   // переменная состояния
    while ((iGetOk = GetMessage(&msg, NULL, 0, 0)) != 0) // цикл сообщений
    {
        if (iGetOk == -1) return 3;  // если GetMessage вернул ошибку - выход
        TranslateMessage(&msg);
        DispatchMessage(&msg);
    }

    return msg.wParam;  // возвращаем код завершения программы
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
    wcWindowClass.hCursor = LoadCursor(NULL, IDC_ARROW);
    // загрузка цвета окон
    wcWindowClass.hbrBackground = (HBRUSH)CreateSolidBrush(RGB(255, 255, 255));
    // белый фон
    return RegisterClass(&wcWindowClass);
    // регистрация класса
}

void DrawDiagram(HWND hwnd, HDC hdc)
{
    RECT rect;
    GetClientRect(hwnd, &rect);
    const int xVE = rect.right - rect.left;
    const int yVE = rect.bottom - rect.top;
    const int xWE = xVE;
    const int yWE = yVE;
    double nPixPerVolt = yVE / 1000.0;
    double nPixPerMs = xVE / 60.0;

    SetMapMode(hdc, MM_ISOTROPIC); //  устанавливает режим отображения указанного контекста устройства
    SetWindowExtEx(hdc, xWE, yWE, NULL); // устанавливает горизонтальные и вертикальные размеры окна для контекста устройства при помощи использования указанных значений
    SetViewportExtEx(hdc, xWE, yWE, NULL); // устанавливает горизонтальные и вертикальные размеры области просмотра для контекста устройства при помощи использования заданных значений
    SetViewportOrgEx(hdc, 10 * nPixPerMs, yWE / 2, NULL); // определяет, какая точка устройства отображает начало координат окна 

    const int tMin = 0;
    const int uMax = 400;
    const int uMin = -400;
    const int tMax = 40;
    
    int u = uMin;
    int xMin = tMin * nPixPerMs;
    int xMax = tMax * nPixPerMs;
    int t = tMin;
    int yMin = uMin * nPixPerVolt;
    int yMax = uMax * nPixPerVolt;

    // горизонтальная линия
    HPEN hPen1 = CreatePen(PS_SOLID, finSizeB, RGB(0, 0, 0));
    HPEN hPen2 = CreatePen(PS_SOLID, finSizeGraphB, RGB(225, 0, 0));

    if (key)
    {
        finSize = finSizeB + size;
        hPen1 = CreatePen(PS_SOLID, finSize, RGB(0, 0, 0));
        
        finSizeGraph = finSizeGraphB + sizeGraph;
        hPen2 = CreatePen(PS_SOLID, finSizeGraph, RGB(225, 0, 0));      
    }

    if (mouse)
    {
        if (left)
        {       
            finSize = finSizeB * size;
            hPen1 = CreatePen(PS_SOLID, finSize, RGB(0, 0, 0));

            finSizeGraph = finSizeGraphB * sizeGraph;
            hPen2 = CreatePen(PS_SOLID, finSizeGraph, RGB(225, 0, 0));
        }
           
        if (right)
        {
            finSize = finSizeB / size;
            hPen1 = CreatePen(PS_SOLID, finSize, RGB(0, 0, 0));

            finSizeGraph = finSizeGraphB / sizeGraph;
            hPen2 = CreatePen(PS_SOLID, finSizeGraph, RGB(225, 0, 0));
        }
    }
  
    SelectObject(hdc, hPen1);
    MoveToEx(hdc, 0, 310, NULL);
    LineTo(hdc, xMax, 310);

    // линия вертикальная
    MoveToEx(hdc, 495, yMin - 30, NULL);
    LineTo(hdc, 495, yMax + 50);

    // график 
    SelectObject(hdc, hPen2);
    int tStep = 1;
    double radianPerMs = 2 * Pi / 20;
    const double uAmplit = 450.00;
    t = tMin;
    MoveToEx(hdc, 300, -245, NULL);
    while (t <= 13) {
        u = (uAmplit - 10) * sin(t * radianPerMs);
        LineTo(hdc, (t + 15) * nPixPerMs, (u + 3) * nPixPerVolt);
        t += 1;
    }
}

// функция обработки сообщений
LRESULT CALLBACK WndProc(
    HWND hWnd,
    UINT message,
    WPARAM wParam,
    LPARAM lParam)
{
    static HDC hDC;
    PAINTSTRUCT ps;

    switch (message)
    {

    case WM_KEYDOWN:
    {
        switch (wParam)
        {
        case VK_LEFT:
        {
            //MessageBox(hWnd, "Message", "left key", MB_OK);
            key = true;
            mouse = false;
            size += 10.0;
            sizeGraph += 10.0;
            InvalidateRect(hWnd, NULL, TRUE);
            return 0;
        }

        case VK_RIGHT:
        {
            //MessageBox(hWnd, "Message", "right key", MB_OK);
            key = true;
            mouse = false;
            size -= 10;
            sizeGraph -= 10;
            InvalidateRect(hWnd, NULL, TRUE);
            return 0;
        }

        default:
            return 0;
        }
        return 0;
    }
   
    case WM_LBUTTONDOWN:
    {
        //MessageBox(hWnd, "Message", "left mouse", MB_OK);
        mouse = left = true; 
        key = right = false;
       
        size += 1.5;
        sizeGraph += 1.5;      
        InvalidateRect(hWnd, NULL, TRUE);
        return 0;
    }

    case WM_RBUTTONDOWN:
    {
        //MessageBox(hWnd, "Message", "right mouse", MB_OK);
        mouse = left = false;
        key = right = true;

        size -= 1.5;
        sizeGraph -= 1.5;
        InvalidateRect(hWnd, NULL, TRUE);
        return 0;
    }

    case WM_PAINT:
    {
        hDC = BeginPaint(hWnd, &ps);
        DrawDiagram(hWnd, hDC);
        EndPaint(hWnd, &ps);
        break;
    }

    case WM_DESTROY:
    {
        PostQuitMessage(0);
        break;
    } 

    default:
        return DefWindowProc(hWnd, message, wParam, lParam);
    }
    return 0;
}
