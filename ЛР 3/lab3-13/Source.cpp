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
TCHAR callBegin[] = _T("�������� . . .");
TCHAR callEnd[] = _T("�����");

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

    if (!RegisterClassEx(&wcex)) // ���� �� ������� ������� ����� ����
    {
        MessageBox(NULL,
            _T("Call to RegisterClassEx failed!"),
            _T("Windows Desktop Guided Tour"),
            NULL);

        return 1;
    }

    hInst = hInstance;
    // �������� ����
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

    if (!hWnd) // ���� �� ������� ������� ����������
    {
        MessageBox(NULL,
            _T("Call to CreateWindow failed!"),
            _T("Windows Desktop Guided Tour"),
            NULL);

        return 1;
    }
    // �������� ����
    ShowWindow(hWnd,
        nCmdShow);
    UpdateWindow(hWnd);
    // ���� ��������� ���������
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

        if (isCall) // ���� ������
        {
            TextOut(hdc,
                5, 5,
                callBegin, _tcslen(callBegin));
            TextOut(hdc,
                100, 5,
                outTime, _tcslen(outTime));
        }

        if (isHang) // ���� �������� ������
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

        hContactList = CreateWindow( // �������� ������ ���������
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
        // ������� ������� � ������� � ���������� ���������
        SendMessage(hContactList, LB_ADDSTRING, 0,
            (LPARAM)(LPSTR)L"+79154545354 ��������");
        SendMessage(hContactList, LB_ADDSTRING, 0,
            (LPARAM)(LPSTR)L"+79154545666 ��������");
        SendMessage(hContactList, LB_ADDSTRING, 0,
            (LPARAM)(LPSTR)L"+78005553535 �����");

        // �������� ��������� ����� � ��������� � ��� ��������
        hCurrentName = CreateWindow(L"edit", L"��� ��������", WS_CHILD | WS_VISIBLE | WS_BORDER | ES_LEFT,
            10, 40, 200, 20, hWnd, (HMENU)CURRENT_NAME, hInst, 0);
        hCurrentPhone = CreateWindow(L"edit", L"�������", WS_CHILD | WS_VISIBLE | WS_BORDER | ES_LEFT,
            10, 70, 200, 20, hWnd, (HMENU)CURRENT_PHONE, hInst, 0);

        // �������� ������ ������ � ������ ������
        hHangBtn = CreateWindow(
            L"BUTTON",  // Predefined class; Unicode assumed 
            L"�����",      // Button text 
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
            L"���������",      // Button text 
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
        if (HIWORD(wParam) == LBN_SELCHANGE || HIWORD(wParam) == LBN_DBLCLK) // ������� �� �������� � ������
        {
            unsigned uSelectedItem; // ����� ��������� ������

            uSelectedItem = (int)SendMessage(hContactList, // ���������� ������ ���������� ������
                LB_GETCURSEL, 0, 0L);

            if (uSelectedItem != LB_ERR) // ���� ������ ���������� ����� ��������� ������
            {
                wchar_t buffer[256];
                SendMessage(hContactList, LB_GETTEXT, // ����������� ������ � ��������� ������ � �������� buffer
                    uSelectedItem, (LPARAM)buffer);

                // ������ ������ �� ���������� ������ � ����� �� bufer �������������
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
                // ����� ����������

                isPhone = true;
                isName = true;
                // ��������� ������ ����� �����
                SendMessage(hCurrentName, WM_SETTEXT, NULL, (LPARAM)name);
                SendMessage(hCurrentPhone, WM_SETTEXT, NULL, (LPARAM)phone);
                // ���������� ����� �����
                UpdateWindow(hCurrentName);
                UpdateWindow(hCurrentPhone);

            }
        }

        if (wParam == CALL_BTN) // ������ ����������
        {
            if (isName && isPhone) // ���� ������ �������
            {
                isCall = true;
                isHang = false;
                InvalidateRect(hWnd, NULL, TRUE); // ����������� ����
                SetTimer(hWnd, 1, 1000, NULL); // ��������� �������, ������ ������ �������
            }
            else MessageBox(hWnd,L"��������� ��������!", L"������!", NULL); // ���� ������� �� ������
        }

        if (wParam == HANG_BTN) // ������ ������
        {
            if (isName && isPhone) // ���� ������ �������
            {
                isHang = true;
                isCall = false;
                InvalidateRect(hWnd, NULL, TRUE); // ����������� ����
                KillTimer(hWnd, 1); // ��������� �������
                callSec = 0;
                callMin = 0;
            }
            else MessageBox(hWnd, L"��������� ��������!", L"������!", NULL); // ���� ������� �� ������
        }
        break;

     case WM_TIMER: // ����� ������ ������
         callSec++; // ���-�� ������ + 1

         if (callSec == 60) // ��������� ����� �����
         {
             callSec = 0;
             callMin++;
         }

         sec1 = callSec % 10; // ���������� ��� �������� ������ �������
         sec2 = callSec / 10;
         min1 = callMin % 10;
         min2 = callMin / 10;

         swprintf(outTime, L"%i%i:%i%i\0", min2, min1, sec2, sec1); // ����� �������
         InvalidateRect(hWnd, NULL, TRUE);  // ����������� ����
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