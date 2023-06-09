#define STRICT // строгая проверка типов переменных
#include <windows.h> // два файла с определениями, макросами
#include <windowsx.h> // и прототипами функций Windows
#include <cmath>

#define TIMER_ID 1

auto PI = 3.14156;

struct STAR {
    unsigned N;
    int x, y;
    unsigned r;
    COLORREF color;
};

LRESULT CALLBACK WndProc(HWND, UINT, WPARAM, LPARAM);

void ugol(HDC hDC, int x1, int y1, int x2, int y2, int x3, int y3, int n);

void star(HDC hDC, struct STAR str) {
    int rc = str.r / 3;
    double a1, a2, a3, d = 2. * PI / (str.N * 2);
    HPEN hPen = CreatePen(PS_SOLID, 3, str.color);

    SelectObject(hDC, hPen);
    for (int i = 1; i < str.N; i++) {
        a1 = 2 * PI * (i - 1) / str.N;
        a2 = a1 + d;
        a3 = a1 - d;
        ugol(hDC, str.x, str.y, str.x + str.r * sin(a1), str.y + str.r * cos(a1),
             str.x + rc * sin(a1), str.y + rc * cos(a2), 25);
        ugol(hDC, str.x, str.y, str.x + str.r * sin(a1), str.y + str.r * cos(a1),
             str.x + rc * sin(a3), str.y + rc * cos(a3), 25);
    }
}

void ugol(HDC hDC, int x1, int y1, int x2, int y2, int x3, int y3, int n)
{
    int n1 = n + 1;
    double a;
    int xx1, yy1, xx2, yy2;
    for (int i = 1; i <= n1; i++)
    {
        a = (double)(i - 1) / (double)n;
        xx1 = x1 + (x2 - x1) * a;
        yy1 = y1 + (y2 - y1) * a;
        xx2 = x2 + (x3 - x2) * a;
        yy2 = y2 + (y3 - y2) * a;
        MoveToEx(hDC, xx1, yy1, NULL);
        LineTo(hDC, xx2, yy2);
    }
}

HINSTANCE hInst; // Дескриптор экземпляра приложения
char ClassName[] = "Window"; // Название класса окна
char AppTitle[] = "Application Win32"; // Заголовок главного окна
static STAR str;

int WINAPI WinMain(
        HINSTANCE hInstance, // Дескриптор экземпляра приложения
        HINSTANCE, // В Win32 всегда равен NULL
        LPSTR, // Указатель на командную строку. Он позволяет
        // приложению получать данные из командной строки.
        int // Определяет, как приложение первоначально
        // отображается на дисплее
)
{

    WNDCLASS wc; // Структура для информации о классе окна
    MSG msg; // Структура для хранения сообщения
    HWND hWnd;

    if ((hWnd = FindWindow(ClassName, NULL)) != NULL)
    {
        if (IsIconic(hWnd)) ShowWindow(hWnd, SW_RESTORE);
        SetForegroundWindow(hWnd);
        // Найдена работающая копия - работа новой копии прекращается.
        return FALSE;
    }
    // --- Работающая копия не найдена - функция WinMain приступает к инициализации.
    // Заполнение структуры WNDCLASS для регистрации класса окна.
    memset(&wc, 0, sizeof(wc));
    wc.lpszClassName = ClassName; // Имя класса окон
    wc.lpfnWndProc = WndProc; // Адрес оконной функции
    wc.style = CS_HREDRAW | CS_VREDRAW; // Стиль класса окон
    wc.hInstance = hInst; // Экземпляр приложения
    wc.hIcon = LoadIcon(NULL, IDI_APPLICATION); // Пиктограмма для окон
    wc.hCursor = LoadCursor(NULL, IDC_ARROW); // Курсор мыши для окон
    wc.hbrBackground = GetStockBrush(WHITE_BRUSH); // Кисть для окон
    wc.lpszMenuName = NULL; // Ресурс меню окон
    wc.cbClsExtra = 0; // Дополнительная память
    wc.cbWndExtra = 0; // Дополнительная память
    // Pегистрация класса окна.
    RegisterClass(&wc);
    // Создаем главное окно приложения.
    hWnd = CreateWindow(
            ClassName, // Имя класса окон
            AppTitle, // Заголовок окна
            WS_OVERLAPPEDWINDOW, // Стиль окна
            10, // X-координаты
            10, // Y-координаты
            600, // Ширина окна
            500, // Высота окна
            NULL, // Дескриптор окна-родителя
            NULL, // Дескриптор меню окна
            hInst, // Дескриптор экземпляра приложения
            NULL); // Дополнительная информация
    if (!hWnd)
    {
        // Окно не создано, выдаем предупреждение.
        MessageBox(NULL, "Create: error", AppTitle, MB_OK | MB_ICONSTOP);
        return FALSE;
    }
    // Отображаем окно.
    ShowWindow(hWnd, SW_SHOWNORMAL);
    // Обновляем содержимое клиентской области окна.
    UpdateWindow(hWnd);
    // Запускаем цикл обработки очереди сообщений. Функция GetMessage получает
    // сообщение из очереди и выдает false при выборке из очереди сообщения
    while (GetMessage(&msg, NULL, 0, 0))
    {
        // Преобразование некоторых сообщений, полученных с помощью клавиатуры
        TranslateMessage(&msg);
        // Отправляем сообщение оконной процедуре
        DispatchMessage(&msg);
    }
    return 0;
}
// --- Функция окна
LRESULT CALLBACK WndProc(HWND hWnd, UINT msg,
                         WPARAM wParam, LPARAM lParam)
{
    const char* Str = "First Windows aplication";

    static RECT WindowSize;

    switch (msg)
    {

        case WM_CREATE:
        {
            SetTimer(hWnd, TIMER_ID, 5000, NULL); // создание таймера
            return 0;
        }

        case WM_TIMER:
        {
            str.N = rand() % 15 + 5;
            str.r = rand() % 200 + 20;
            str.x = rand() % ((WindowSize.right - WindowSize.left) - str.r) + str.r;
            str.y = rand() % ((WindowSize.bottom - WindowSize.top) - str.r) + str.r;
            str.color = RGB(rand() % 255, rand() % 255, rand() % 255);

            InvalidateRect(hWnd, NULL, TRUE);
            return 0;
        }
        case WM_PAINT:
        {
            PAINTSTRUCT ps;
            HDC hdc;
            GetWindowRect(hWnd, &WindowSize);


            hdc = BeginPaint(hWnd, &ps); // Получить контекст окна
            star(hdc, str); // рисование звезды
            EndPaint(hWnd, &ps); // Освободить контекст окна

            return 0;
        }

            // Нажата левая клавиша мыши в клиентской области окна.
        case WM_LBUTTONDOWN:
        {
            MessageBox(hWnd,
                       "pressed left button", "LEFT BUTTON",
                       MB_OK | MB_ICONINFORMATION);
            return 0;
        }

            // Нажата правая клавиша мыши в клиентской области окна.
        case WM_RBUTTONDOWN:
        {
            MessageBox(hWnd,
                       "pressed right button", "RIGHT BUTTON",
                       MB_OK | MB_ICONINFORMATION);
            return 0;
        }

            // Пользователь удалил окно.
        case WM_DESTROY:
        {
            // Если данная функция является оконной функцией главного окна, то
            // следует в очередь сообщений приложения послать сообщение WM_QUIT
            KillTimer((HWND)TIMER_ID, NULL); // удаление таймера
            PostQuitMessage(0);
            return 0;
        }
            // Необработанные сообщения передаем в стандартную
            // функцию обработки сообщений по умолчанию.
        default: return DefWindowProc(hWnd, msg, wParam, lParam);
    }
    return 0;
}
