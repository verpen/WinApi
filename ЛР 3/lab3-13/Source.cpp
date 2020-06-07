#include "include.h"

#define CONTACT_LIST 1001
#define CURRENT_NAME 1002
#define CURRENT_PHONE 1003
#define HANG_BTN 1004
#define CALL_BTN 1005

static TCHAR szWindowClass[] = _T("DesktopApp");

static TCHAR szTitle[] = _T("Phone by Penegina");

HINSTANCE hInst;

HWND hContactList;
HWND hCurrentName;
HWND hCurrentPhone;
HWND hHangBtn;
HWND hWnd;

wchar_t phone[13];
wchar_t name[64];
wchar_t outTime[6];

int callSec = 0;
int callMin = 0;
int sec1 = callSec % 10;
int sec2 = callSec / 10;
int min1 = callMin % 10;
int min2 = callMin / 10;

bool isCall = false;
bool isHang = false;
bool isName = false;
bool isPhone = false;

TCHAR callInfo[] = _T("Phone by Penegina");
TCHAR callBegin[] = _T("Ожидание . . .");
TCHAR callEnd[] = _T("Сброс");

HDC hdc;
PAINTSTRUCT ps;

LRESULT CALLBACK WndProc(HWND, UINT, WPARAM, LPARAM);

int CALLBACK WinMain(
    _In_ HINSTANCE hInstance,
    _In_opt_ HINSTANCE hPrevInstance,
    _In_ LPSTR     lpCmdLine,
    _In_ int       nCmdShow
)
{
    WNDCLASSEX wcex;

    wcex.cbSize = sizeof(WNDCLASSEX);
    wcex.style = CS_HREDRAW | CS_VREDRAW;
    wcex.lpfnWndProc = WndProc;
    wcex.cbClsExtra = 0;
    wcex.cbWndExtra = 0;
    wcex.hInstance = hInstance;
    wcex.hIcon = LoadIcon(hInstance, IDI_APPLICATION);
    wcex.hCursor = LoadCursor(NULL, IDC_ARROW);
    wcex.hbrBackground = (HBRUSH)(COLOR_WINDOW + 1);
    wcex.lpszMenuName = NULL;
    wcex.lpszClassName = szWindowClass;
    wcex.hIconSm = LoadIcon(wcex.hInstance, IDI_APPLICATION);

    if (!RegisterClassEx(&wcex)) // если не удалось создать класс окна
    {
        MessageBox(NULL,
            _T("Call to RegisterClassEx failed!"),
            _T("Windows Desktop Guided Tour"),
            NULL);

        return 1;
    }

    hInst = hInstance;
    // Создание окна
    hWnd = CreateWindow(
        szWindowClass,
        szTitle,
        WS_OVERLAPPEDWINDOW,
        CW_USEDEFAULT, CW_USEDEFAULT,
        500, 500,
        NULL,
        NULL,
        hInstance,
        NULL
    );

    if (!hWnd) // если не удалось создать дескриптор
    {
        MessageBox(NULL,
            _T("Call to CreateWindow failed!"),
            _T("Windows Desktop Guided Tour"),
            NULL);

        return 1;
    }
    // открытие окна
    ShowWindow(hWnd,
        nCmdShow);
    UpdateWindow(hWnd);
    // цикл обработки сообщений
    MSG msg;
    while (GetMessage(&msg, NULL, 0, 0))
    {
        TranslateMessage(&msg);
        DispatchMessage(&msg);
    }

    return (int)msg.wParam;
}


LRESULT CALLBACK WndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{
    switch (message)
    {
    case WM_PAINT:
        hdc = BeginPaint(hWnd, &ps);

        if (isCall) // если звонок
        {
            TextOut(hdc,
                5, 5,
                callBegin, _tcslen(callBegin));
            TextOut(hdc,
                100, 5,
                outTime, _tcslen(outTime));
        }

        if (isHang) // если повесили трубку
        {
            TextOut(hdc,
                5, 5,
                callEnd, _tcslen(callEnd));
            TextOut(hdc,
                100, 5,
                outTime, _tcslen(outTime));
            isHang = false;
        }


        EndPaint(hWnd, &ps);
        break;

    case WM_CREATE:

        hContactList = CreateWindow( // создание списка абонентов
            _T("listbox"),
            NULL,
            WS_CHILD | WS_VISIBLE | LBS_STANDARD |
            LBS_NOTIFY,
            10,
            100,
            300,
            300,
            hWnd,
            (HMENU)CONTACT_LIST,
            hInst,
            NULL);
        // вставка строчек с именами и телефонами абонентов
        SendMessage(hContactList, LB_ADDSTRING, 0,
            (LPARAM)(LPSTR)L"+79154545354 Лужайкин");
        SendMessage(hContactList, LB_ADDSTRING, 0,
            (LPARAM)(LPSTR)L"+79154545666 Краснова");
        SendMessage(hContactList, LB_ADDSTRING, 0,
            (LPARAM)(LPSTR)L"+78005553535 Займы");

        // Создание текстовых полей с абонентом и его телефона
        hCurrentName = CreateWindow(L"edit", L"Имя абонента", WS_CHILD | WS_VISIBLE | WS_BORDER | ES_LEFT,
            10, 40, 200, 20, hWnd, (HMENU)CURRENT_NAME, hInst, 0);
        hCurrentPhone = CreateWindow(L"edit", L"Телефон", WS_CHILD | WS_VISIBLE | WS_BORDER | ES_LEFT,
            10, 70, 200, 20, hWnd, (HMENU)CURRENT_PHONE, hInst, 0);

        // Создание кнопок сброса и начала звонка
        hHangBtn = CreateWindow(
            L"BUTTON",  // Predefined class; Unicode assumed 
            L"Сброс",      // Button text 
            WS_TABSTOP | WS_VISIBLE | WS_CHILD | BS_DEFPUSHBUTTON,  // Styles 
            310,         // x position 
            40,         // y position 
            50,        // Button width
            50,        // Button height
            hWnd,     // Parent window
            (HMENU)HANG_BTN,
            hInst,
            NULL);      // Pointer not needed.

        hHangBtn = CreateWindow(
            L"BUTTON",  // Predefined class; Unicode assumed 
            L"Позвонить",      // Button text 
            WS_TABSTOP | WS_VISIBLE | WS_CHILD | BS_DEFPUSHBUTTON,  // Styles 
            220,         // x position 
            40,         // y position 
            80,        // Button width
            50,        // Button height
            hWnd,     // Parent window
            (HMENU)CALL_BTN,
            hInst,
            NULL);      // Pointer not needed.

        break;

     case WM_COMMAND:
        if (HIWORD(wParam) == LBN_SELCHANGE || HIWORD(wParam) == LBN_DBLCLK) // Нажатие на абонента в списке
        {
            unsigned uSelectedItem; // Номер выбранной ячейки

            uSelectedItem = (int)SendMessage(hContactList, // Присвоение номера выделенной ячейки
                LB_GETCURSEL, 0, 0L);

            if (uSelectedItem != LB_ERR) // Если смогли определить номер выбранной ячейки
            {
                wchar_t buffer[256];
                SendMessage(hContactList, LB_GETTEXT, // перемещение строки в выбранной ячейке в перенную buffer
                    uSelectedItem, (LPARAM)buffer);

                // Начало циклов по извлечению номера и имени из bufer поотдельности
                phone[12] = '\0';
                for (size_t i = 0; i < 12; i++)
                {
                    phone[i] = buffer[i];
                }
                size_t i = 13;
                while (buffer[i] != L'\0')
                {
                    name[i - 13] = buffer[i];
                    i++;
                }
                name[i-1] = '\0';
                // конец извлечения

                isPhone = true;
                isName = true;
                // изменение текста полей ввода
                SendMessage(hCurrentName, WM_SETTEXT, NULL, (LPARAM)name);
                SendMessage(hCurrentPhone, WM_SETTEXT, NULL, (LPARAM)phone);
                // обновление полей ввода
                UpdateWindow(hCurrentName);
                UpdateWindow(hCurrentPhone);

            }
        }

        if (wParam == CALL_BTN) // кнопка позвоночка
        {
            if (isName && isPhone) // если выбран абонент
            {
                isCall = true;
                isHang = false;
                InvalidateRect(hWnd, NULL, TRUE); // перерисовка окна
                SetTimer(hWnd, 1, 1000, NULL); // установка таймера, тикает каждую секунду
            }
            else MessageBox(hWnd,L"Ввыберите абонента!", L"Ошибка!", NULL); // если абонент не выбран
        }

        if (wParam == HANG_BTN) // кнопка сброса
        {
            if (isName && isPhone) // если выбран абонент
            {
                isHang = true;
                isCall = false;
                InvalidateRect(hWnd, NULL, TRUE); // перерисовка окна
                KillTimer(hWnd, 1); // остановка таймера
                callSec = 0;
                callMin = 0;
            }
            else MessageBox(hWnd, L"Ввыберите абонента!", L"Ошибка!", NULL); // если абонент не выбран
        }
        break;

     case WM_TIMER: // когда тикнул таймер
         callSec++; // кол-во секунд + 1

         if (callSec == 60) // изменение числа минут
         {
             callSec = 0;
             callMin++;
         }

         sec1 = callSec % 10; // переменные для создания текста времени
         sec2 = callSec / 10;
         min1 = callMin % 10;
         min2 = callMin / 10;

         swprintf(outTime, L"%i%i:%i%i\0", min2, min1, sec2, sec1); // вывод времени
         InvalidateRect(hWnd, NULL, TRUE);  // перерисовка окна
         break;

    case WM_DESTROY:
        PostQuitMessage(0);
        break;

    default:
        return DefWindowProc(hWnd, message, wParam, lParam);
        break;
    }
    return 0;
}