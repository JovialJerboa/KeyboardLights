// KeyboardLights.cpp : Defines the entry point for the application.
//

#pragma region Includes
#include "stdafx.h"
#include "KeyboardLights.h"
#include "About.h"
#include "SysTray.h"
#pragma endregion

#define MAX_LOADSTRING 100

// Global Variables:
TCHAR szTitle[MAX_LOADSTRING];					// The title bar text
TCHAR szWindowClass[MAX_LOADSTRING];			// the main window class name

namespace Global
{
	HINSTANCE hInst;								// current instance
}
using namespace About;
using namespace SysTray;

static UINT_PTR mainTimer = NULL;

// Forward declarations of functions included in this code module:
ATOM				MyRegisterClass(HINSTANCE hInstance);
BOOL				InitInstance(HINSTANCE, int);
LRESULT CALLBACK	WndProc(HWND, UINT, WPARAM, LPARAM);
VOID	CALLBACK	TimerProc(HWND hwnd,UINT uMsg,UINT_PTR idEvent,DWORD dwTime);


int APIENTRY _tWinMain(_In_ HINSTANCE hInstance,
                     _In_opt_ HINSTANCE hPrevInstance,
                     _In_ LPTSTR    lpCmdLine,
                     _In_ int       nCmdShow)
{
	UNREFERENCED_PARAMETER(hPrevInstance);
	UNREFERENCED_PARAMETER(lpCmdLine);

 	// TODO: Place code here.
	MSG msg;
	HACCEL hAccelTable;

	// Initialize global strings
	LoadString(hInstance, IDS_APP_TITLE, szTitle, MAX_LOADSTRING);
	LoadString(hInstance, IDC_KEYBOARDLIGHTS, szWindowClass, MAX_LOADSTRING);
	MyRegisterClass(hInstance);

	// Perform application initialization:
	if (!InitInstance (hInstance, nCmdShow))
	{
		return FALSE;
	}

	hAccelTable = LoadAccelerators(hInstance, MAKEINTRESOURCE(IDC_KEYBOARDLIGHTS));

	// Main message loop:
	while (GetMessage(&msg, NULL, 0, 0))
	{
		if (!TranslateAccelerator(msg.hwnd, hAccelTable, &msg))
		{
			TranslateMessage(&msg);
			DispatchMessage(&msg);
		}
	}

	return (int) msg.wParam;
}


//
//  FUNCTION: MyRegisterClass()
//
//  PURPOSE: Registers the window class.
//
ATOM MyRegisterClass(HINSTANCE hInstance)
{
	WNDCLASSEX wcex;

	wcex.cbSize = sizeof(WNDCLASSEX);

	wcex.style			= CS_HREDRAW | CS_VREDRAW;
	wcex.lpfnWndProc	= WndProc;
	wcex.cbClsExtra		= 0;
	wcex.cbWndExtra		= 0;
	wcex.hInstance		= hInstance;
	wcex.hIcon			= LoadIcon(hInstance, MAKEINTRESOURCE(IDI_KEYBOARDLIGHTS));
	wcex.hCursor		= LoadCursor(NULL, IDC_ARROW);
	wcex.hbrBackground	= (HBRUSH)(COLOR_WINDOW+1);
	wcex.lpszMenuName	= MAKEINTRESOURCE(IDC_KEYBOARDLIGHTS);
	wcex.lpszClassName	= szWindowClass;
	wcex.hIconSm		= LoadIcon(wcex.hInstance, MAKEINTRESOURCE(IDI_SMALL));

	return RegisterClassEx(&wcex);
}

//
//   FUNCTION: InitInstance(HINSTANCE, int)
//
//   PURPOSE: Saves instance handle and creates main window
//
//   COMMENTS:
//
//        In this function, we save the instance handle in a global variable and
//        create and display the main program window.
//
BOOL InitInstance(HINSTANCE hInstance, int nCmdShow)
{
	HWND hWnd;

	Global::hInst = hInstance; // Store instance handle in our global variable

	hWnd = CreateWindow(szWindowClass, szTitle, WS_OVERLAPPEDWINDOW,	CW_USEDEFAULT, 0, CW_USEDEFAULT, 0, NULL, NULL, hInstance, NULL);
	//ShowWindow(hWnd, nCmdShow);
	//UpdateWindow(hWnd);
	CreateSysTray(hWnd);

	// Set up timer
	mainTimer = SetTimer(hWnd, 0, USER_TIMER_MINIMUM, TimerProc);

	return TRUE;
}

BOOL ExitInstance(HWND hWnd)
{
	RemoveSysTray(hWnd);
	return TRUE;
}

//
//  FUNCTION: WndProc(HWND, UINT, WPARAM, LPARAM)
//
//  PURPOSE:  Processes messages for the main window.
//
//  WM_COMMAND	- process the application menu
//  WM_PAINT	- Paint the main window
//  WM_DESTROY	- post a quit message and return
//
//
LRESULT CALLBACK WndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{
	int wmId, wmEvent;
	PAINTSTRUCT ps;
	HDC hdc;

	switch (message)
	{
	case WM_COMMAND:
		wmId    = LOWORD(wParam);
		wmEvent = HIWORD(wParam);
		// Parse the menu selections:
		switch (wmId)
		{
		case IDM_ABOUT:
			DialogBox(Global::hInst, MAKEINTRESOURCE(IDD_ABOUTBOX), hWnd, AboutProc);
			break;
		case IDM_EXIT:
			DestroyWindow(hWnd);
			break;
		default:
			return DefWindowProc(hWnd, message, wParam, lParam);
		}
		break;
	case WM_PAINT:
		hdc = BeginPaint(hWnd, &ps);
		EndPaint(hWnd, &ps);
		break;
	case WM_DESTROY:
		ExitInstance(hWnd);
		PostQuitMessage(0);
		break;
	case APP_MSG_TRAY:
		switch (LOWORD(lParam))
		{
			case WM_CONTEXTMENU:
			{
				HMENU hMenu = LoadMenu(Global::hInst, MAKEINTRESOURCE(IDR_SYSTRAY_MENU));
				POINT pos;

				HMENU hSubMenu = ::GetSubMenu(hMenu, 0);

				GetCursorPos(&pos);
				SetForegroundWindow(hWnd);
				TrackPopupMenu(hSubMenu, 0, pos.x, pos.y, 0, hWnd, 0);

				DestroyMenu(hMenu);
				DestroyMenu(hSubMenu);
			}
			break;
		}
	default:
		return DefWindowProc(hWnd, message, wParam, lParam);
	}
	return 0;
}

VOID CALLBACK TimerProc(HWND hWnd,UINT uMsg,UINT_PTR idEvent,DWORD dwTime)
{
	ChangeSysTrayIcon(hWnd);
	mainTimer = SetTimer(hWnd, 0, USER_TIMER_MINIMUM, TimerProc);
}
