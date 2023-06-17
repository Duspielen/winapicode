#include <Windows.h>
#include <vector>
#include <string>
#include <sstream>

#define ID_FORMAT_PARAGRAPH 1001

LRESULT CALLBACK WndProc(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam);
std::string FormatParagraphs(const std::string& text); // Function declaration

int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int nCmdShow)
{
    // Регистрация класса окна
    const char* className = "TextEditorClass";

    WNDCLASSEX wcex = {};
    wcex.cbSize = sizeof(WNDCLASSEX);
    wcex.lpfnWndProc = WndProc;
    wcex.hInstance = hInstance;
    wcex.lpszClassName = className;

    RegisterClassEx(&wcex);

    // Создание окна
    HWND hWnd = CreateWindow(className, "Text Editor", WS_OVERLAPPEDWINDOW, CW_USEDEFAULT, CW_USEDEFAULT,
                             800, 600, nullptr, nullptr, hInstance, nullptr);

    if (!hWnd)
    {
        return -1;
    }

    // Отображение окна
    ShowWindow(hWnd, nCmdShow);
    UpdateWindow(hWnd);

    // Цикл обработки сообщений
    MSG msg = {};
    while (GetMessage(&msg, nullptr, 0, 0))
    {
        TranslateMessage(&msg);
        DispatchMessage(&msg);
    }

    return static_cast<int>(msg.wParam);
}

LRESULT CALLBACK WndProc(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam)
{
    static HWND hEdit;

    switch (msg)
    {
        case WM_CREATE:
        {
            // Создание текстового поля
            hEdit = CreateWindowEx(WS_EX_CLIENTEDGE, "EDIT", nullptr, WS_CHILD | WS_VISIBLE | WS_VSCROLL |
                                                                      ES_MULTILINE | ES_AUTOVSCROLL, 0, 0, 784, 561, hWnd, nullptr, GetModuleHandle(nullptr), nullptr);
            break;
        }
        case WM_COMMAND:
        {
            if (LOWORD(wParam) == ID_FORMAT_PARAGRAPH)
            {
                // Получение текста из текстового поля
                int length = GetWindowTextLength(hEdit) + 1;
                std::vector<char> buffer(length);
                GetWindowText(hEdit, buffer.data(), length);

                // Форматирование абзацев
                std::string text(buffer.begin(), buffer.end());
                std::string formattedText = FormatParagraphs(text);

                // Установка отформатированного текста в текстовое поле
                SetWindowText(hEdit, formattedText.c_str());
            }
            break;
        }
        case WM_SIZE:
        {
            // Изменение размеров текстового поля при изменении размеров окна
            RECT rect;
            GetClientRect(hWnd, &rect);
            MoveWindow(hEdit, 0, 0, rect.right, rect.bottom, TRUE);
            break;
        }
        case WM_DESTROY:
        {
            PostQuitMessage(0);
            break;
        }
        default:
            return DefWindowProc(hWnd, msg, wParam, lParam);
    }

    return 0;
}

std::string FormatParagraphs(const std::string& text)
{
    std::vector<std::string> paragraphs;
    std::string paragraph;
    std::istringstream iss(text);

    // Разделение текста на абзацы
    for (std::string line; std::getline(iss, line); )
    {
        if (!line.empty())
        {
            paragraph += line + "\n";
        }
        else if (!paragraph.empty())
        {
            paragraphs.push_back(paragraph);
            paragraph.clear();
        }
    }

    if (!paragraph.empty())
    {
        paragraphs.push_back(paragraph);
    }

    // Форматирование абзацев
    std::string formattedText;

    for (const auto& para : paragraphs)
    {
        std::vector<std::string> lines;
        std::string line;
        std::istringstream iss(para);

        // Разделение абзаца на строки
        for (std::string word; iss >> word; )
        {
            if (line.size() + word.size() <= 63)
            {
                line += word + " ";
            }
            else
            {
                lines.push_back(line);
                line = word + " ";
            }
        }

        if (!line.empty())
        {
            lines.push_back(line);
        }

        // Соединение строк с добавлением пробелов
        std::string formattedParagraph;

        for (const auto& line : lines)
        {
            formattedParagraph += line;
        }

        formattedText += formattedParagraph + "\n";
    }

    return formattedText;
}
k
