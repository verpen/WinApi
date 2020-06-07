
#include <stdafx.h>
#include <Windows.h>
#include <string>
#include <algorithm>
#include <float.h>
#include <vector> 
#include <commctrl.h>
#include <math.h>
#include <windowsx.h>
#include <string>
#include <stdio.h>
#include <cstdlib>
#include <ctime>

#pragma comment( lib, "comctl32.lib")

#pragma comment( linker, "/manifestdependency:\"type='win32' \
        name='Microsoft.Windows.Common-Controls' version='6.0.0.0' \
        processorArchitecture='*' publicKeyToken='6595b64144ccf1df' \
        language='*'\"")


#define HATCH 1023

#define NEHATCH 1024



HANDLE hThread1;
HANDLE hThread2;




DWORD WINAPI Thread1(LPVOID t);
DWORD WINAPI Thread2(LPVOID t);



BOOL thread1_working = false;
BOOL thread2_working = false;


int hatch;


DWORD WINAPI Thread1(LPVOID t) {
	while (true) {
		Sleep(2*1000);
		HDC hdc = GetDC((HWND)t);
		RECT Window;
		GetWindowRect((HWND)t, &Window);

		FillRect(hdc, &Window, CreateSolidBrush(RGB(180, 180, 180)));
		DrawText(hdc, L"Технический перерыв 20 мин", 27, &Window, DT_SINGLELINE | DT_CENTER | DT_VCENTER);
	}
	return 0;
}



DWORD WINAPI Thread2(LPVOID t) {
	PAINTSTRUCT ps;
	HDC hdc = GetDC((HWND)t);
	BeginPaint((HWND)t, &ps);
	RECT Window;
	GetWindowRect((HWND)t, &Window);
	srand((unsigned)time(0));
	SetBkColor(hdc, RGB(rand() % 155, rand() % 155, rand() % 155));
	int a = rand() % 255;
	HBRUSH brush = CreateHatchBrush(HS_BDIAGONAL, RGB(rand() % 155 + 100, rand() % 155 + 100, rand() % 155 + 100));
	switch (hatch) {
	case 1:
	{
		SetBkColor(hdc, RGB(175, 238, 238));
		brush = CreateHatchBrush(HS_BDIAGONAL, RGB(255, 255, 255));
		FillRect(hdc, &Window, brush);
		ReleaseDC((HWND)t, hdc);
		EndPaint((HWND)t, &ps);
		return 0;
	}
	case 0:
	{
		FillRect(hdc, &Window, CreateSolidBrush(RGB(175, 238, 238)));
		ReleaseDC((HWND)t, hdc);
		EndPaint((HWND)t, &ps);
		return 0;
	}
	}
}

LRESULT CALLBACK WndProc(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam)
{

	switch (uMsg) {

	case WM_PAINT:
	{

	}

	return 0;

	case WM_MOUSELEAVE:
	{

	}
	case WM_CTLCOLORSTATIC:
	{


	}
	return 0;
	case WM_COMMAND:
	{
		if (LOWORD(wParam) == HATCH) {
			hatch = true;
			hThread2 = CreateThread(NULL, 0, Thread2, hWnd, 0, NULL);
		}
		if (LOWORD(wParam) == NEHATCH) {
			hatch = false;

			hThread2 = CreateThread(NULL, 0, Thread2, hWnd, 0, NULL);
		}
		if (LOWORD(wParam) == 1000) {
			hThread1 = CreateThread(NULL, 0, Thread1, hWnd, 0, NULL);
		}
		if (LOWORD(wParam) == 1001) {
			TerminateThread(hThread1, 0);
		}
	}
	return 0;
	case WM_LBUTTONDOWN:
	{



	}
	return 0;
	case WM_RBUTTONDOWN:
	{

	}
	return 0;
	case WM_CREATE:
	{
		HDC hdc = GetDC(hWnd);
		HMENU hMenubar = CreateMenu();
		HMENU hMenu = CreateMenu();
		AppendMenu(hMenubar, MF_POPUP, (UINT_PTR)hMenu, L"Главное окно");
		AppendMenu(hMenu, MF_STRING | MF_POPUP, HATCH, L"Штриховка");
		AppendMenu(hMenu, MF_STRING | MF_POPUP, NEHATCH, L"Сплошное");
		HMENU hMenu1 = CreateMenu();
		AppendMenu(hMenubar, MF_POPUP, (UINT_PTR)hMenu1, L"Технический перерыв");
		AppendMenu(hMenu1, MF_STRING | MF_POPUP, 1000, L"Включить");
		AppendMenu(hMenu1, MF_STRING | MF_POPUP, 1001, L"Выключить");
		SetMenu(hWnd, hMenubar);
		RECT Window;
		GetWindowRect(hWnd, &Window);
		FillRect(hdc, &Window, CreateHatchBrush(HS_BDIAGONAL, RGB(123, 123, 123)));


	}
	return 0;
	case WM_DESTROY:
	{
		PostQuitMessage(0);
	}
	return 0;
	}
	return DefWindowProc(hWnd, uMsg, wParam, lParam);
};




int CALLBACK wWinMain(HINSTANCE hInstance, HINSTANCE, PWSTR szCmdLine, int nCmdShow) {
	MSG msg{};
	HWND hwnd{};
	WNDCLASSEX wc{ sizeof(WNDCLASSEX) };

	wc.cbClsExtra = 0;
	wc.cbWndExtra = 0;
	wc.hbrBackground = CreateHatchBrush(HS_BDIAGONAL, RGB(255, 255, 255));
	wc.hCursor = LoadCursor(nullptr, IDC_ARROW);
	wc.hIcon = LoadIcon(nullptr, IDI_APPLICATION);
	wc.hInstance = hInstance;

	wc.lpfnWndProc = WndProc;
	wc.lpszClassName = L"Class";
	wc.lpszMenuName = NULL;
	wc.style = CS_VREDRAW | CS_HREDRAW;
	LoadLibrary(TEXT("Msftedit.dll"));
	if (!RegisterClassEx(&wc)) {
		return EXIT_FAILURE;
	}
	hwnd = CreateWindow(wc.lpszClassName, L"Пенегина ЛР4", WS_OVERLAPPEDWINDOW, 0, 0, 500, 800, NULL, NULL, wc.hInstance, NULL);
	if (hwnd == INVALID_HANDLE_VALUE)
	{
		return EXIT_FAILURE;
	}
	ShowWindow(hwnd, nCmdShow);
	UpdateWindow(hwnd);

	while (GetMessage(&msg, nullptr, 0, 0)) {
		DispatchMessage(&msg);
		TranslateMessage(&msg);
	}
	return static_cast<int>(msg.wParam);
}