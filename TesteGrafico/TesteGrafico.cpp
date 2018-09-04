// TesteGrafico.cpp : Defines the entry point for the application.
//

#include "stdafx.h"
#include "TesteGrafico.h"
#include <stdio.h>

#include <time.h>
#include <stdlib.h>
#include <stdio.h>
#include <Windows.h>
#include <tchar.h>
#include <io.h>
#include <fcntl.h>

#define MAX_BASIC_INVADERS 200
#define MAX_ELUSIVE_INVADERS 200

#define NSHIPS 1

#define LIM_ESQ 0
#define LIM_DIR 31
#define LIM_CIM 0
#define LIM_BAI 21
#define PASSO_X 1
#define PASSO_Y 1

enum Direction {
	Up,
	UpRight,
	Right,
	DownRight,
	Down,
	DownLeft,
	Left,
	UpLeft,
	NumberOfDirections
};

typedef struct Point {
	unsigned int x;
	unsigned int y;
} Point;

typedef struct Point Position;

typedef struct Dimension {
	unsigned int width;
	unsigned int height;
} Dimension;

typedef struct Box {
	Position position;
	Dimension dimension;
} Box;

typedef struct BasicInvader {
	Box box;
	Direction currentDirection;
	Direction lastDirection;
	char simb;
} BasicInvader;

typedef struct ElusiveInvader {
	Box box;
	Direction currentDirection;
	Direction lastDirection;
	char simb;
} ElusiveInvader;

typedef struct Projectile {
	Box box;
	unsigned int speed;
} Projectile;

/* An array of structures representing the basic invaders */
BasicInvader basicInvaders[MAX_BASIC_INVADERS];

/* An array of structures representing the elusive invaders */
ElusiveInvader elusiveInvaders[MAX_ELUSIVE_INVADERS];







#define MAX_LOADSTRING 100

// Global Variables:
HINSTANCE hInst;                                // current instance
WCHAR szTitle[MAX_LOADSTRING];                  // The title bar text
WCHAR szWindowClass[MAX_LOADSTRING];            // the main window class name


int x, y;
HDC hdc = NULL, auxDC = NULL;
HBRUSH bg = NULL;
HBITMAP auxBM = NULL;
int nX = 0, nY = 0; //screen size
int nJog = 0;


HBITMAP hNave = NULL;




// Forward declarations of functions included in this code module:
ATOM                MyRegisterClass(HINSTANCE hInstance);
BOOL                InitInstance(HINSTANCE, int);
LRESULT CALLBACK    WndProc(HWND, UINT, WPARAM, LPARAM);
INT_PTR CALLBACK    About(HWND, UINT, WPARAM, LPARAM);

int APIENTRY wWinMain(_In_ HINSTANCE hInstance,
	_In_opt_ HINSTANCE hPrevInstance,
	_In_ LPWSTR    lpCmdLine,
	_In_ int       nCmdShow)
{
	UNREFERENCED_PARAMETER(hPrevInstance);
	UNREFERENCED_PARAMETER(lpCmdLine);

	// TODO: Place code here.

	// Initialize global strings
	LoadStringW(hInstance, IDS_APP_TITLE, szTitle, MAX_LOADSTRING);
	LoadStringW(hInstance, IDC_TESTEGRAFICO, szWindowClass, MAX_LOADSTRING);
	MyRegisterClass(hInstance);

	// Perform application initialization:
	if (!InitInstance(hInstance, nCmdShow))
	{
		return FALSE;
	}

	HACCEL hAccelTable = LoadAccelerators(hInstance, MAKEINTRESOURCE(IDC_TESTEGRAFICO));

	MSG msg;

	// Main message loop:
	while (GetMessage(&msg, nullptr, 0, 0))
	{
		if (!TranslateAccelerator(msg.hwnd, hAccelTable, &msg))
		{
			TranslateMessage(&msg);
			DispatchMessage(&msg);
		}
	}

	return (int)msg.wParam;
}


void gotoxy(int x, int y) {
	static HANDLE hStdout = NULL;
	COORD coord;
	coord.X = x;
	coord.Y = y;
	if (!hStdout)
		hStdout = GetStdHandle(STD_OUTPUT_HANDLE);

	SetConsoleCursorPosition(hStdout, coord);
}

//
//  FUNCTION: MyRegisterClass()
//
//  PURPOSE: Registers the window class.
//
ATOM MyRegisterClass(HINSTANCE hInstance)
{
	WNDCLASSEXW wcex;

	wcex.cbSize = sizeof(WNDCLASSEX);

	wcex.style = CS_HREDRAW | CS_VREDRAW;
	wcex.lpfnWndProc = WndProc;
	wcex.cbClsExtra = 0;
	wcex.cbWndExtra = 0;
	wcex.hInstance = hInstance;
	wcex.hIcon = LoadIcon(hInstance, MAKEINTRESOURCE(IDI_TESTEGRAFICO));
	wcex.hCursor = LoadCursor(nullptr, IDC_ARROW);
	wcex.hbrBackground = (HBRUSH)(COLOR_WINDOW + 1);
	wcex.lpszMenuName = MAKEINTRESOURCEW(IDC_TESTEGRAFICO);
	wcex.lpszClassName = szWindowClass;
	wcex.hIconSm = LoadIcon(wcex.hInstance, MAKEINTRESOURCE(IDI_SMALL));

	return RegisterClassExW(&wcex);
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
	hInst = hInstance; // Store instance handle in our global variable

	HWND hWnd = CreateWindowW(szWindowClass, szTitle, WS_OVERLAPPEDWINDOW,
		CW_USEDEFAULT, 0, CW_USEDEFAULT, 0, nullptr, nullptr, hInstance, nullptr);

	if (!hWnd)
	{
		return FALSE;
	}

	ShowWindow(hWnd, nCmdShow);
	UpdateWindow(hWnd);

	return TRUE;
}

//
//  FUNCTION: WndProc(HWND, UINT, WPARAM, LPARAM)
//
//  PURPOSE:  Processes messages for the main window.
//
//  WM_COMMAND  - process the application menu
//  WM_PAINT    - Paint the main window
//  WM_DESTROY  - post a quit message and return
//
//
void * move(void * in)
{
	HWND hWnd = (HWND)in;

	//OutputDebugString(TEXT("o x mexeu"));
	//Sleep(5);


	while (1) {

		Sleep(100);
		for (int i = 0; i < NSHIPS; i++)
		{
			gotoxy(basicInvaders[i].box.position.x, basicInvaders[i].box.position.y);

			switch (basicInvaders[i].currentDirection)
			{
			case Right:
				if (basicInvaders[i].box.position.x + PASSO_X < LIM_DIR)
				{
					gotoxy(basicInvaders[i].box.position.x - PASSO_X, basicInvaders[i].box.position.y);
					
					basicInvaders[i].box.position.x += PASSO_X;
					continue;
				}
				else
				{
					if (basicInvaders[i].box.position.y + PASSO_Y < LIM_BAI)
					{
						basicInvaders[i].box.position.y += PASSO_Y;
						basicInvaders[i].lastDirection = basicInvaders[i].currentDirection;
						basicInvaders[i].currentDirection = Left;
						continue;
					}
					else
					{
						gotoxy(basicInvaders[i].box.position.x, basicInvaders[i].box.position.y - PASSO_X);
						
						basicInvaders[i].box.position.x = 1;
						basicInvaders[i].box.position.y = 1;
						basicInvaders[i].lastDirection = basicInvaders[i].currentDirection;
						basicInvaders[i].currentDirection = Right;
						continue;
					}
				}
				break;

			case Left:
				if (basicInvaders[i].box.position.x - PASSO_X > LIM_ESQ)
				{
					gotoxy(basicInvaders[i].box.position.x + PASSO_X, basicInvaders[i].box.position.y);
					
					basicInvaders[i].box.position.x -= PASSO_X;
					continue;
				}
				else
				{
					if (basicInvaders[i].box.position.y + PASSO_Y < LIM_BAI)
					{
						basicInvaders[i].box.position.y += PASSO_Y;
						basicInvaders[i].lastDirection = basicInvaders[i].currentDirection;
						basicInvaders[i].currentDirection = Right;
						continue;
					}
					else
					{
						basicInvaders[i].box.position.x = 1; //volta a posicao inicial
						basicInvaders[i].box.position.y = 1;
						basicInvaders[i].lastDirection = basicInvaders[i].currentDirection;
						basicInvaders[i].currentDirection = Right;
						continue;
					}
				}
				break;
			}


		}
		InvalidateRect(hWnd, NULL, FALSE);
	}

}


LRESULT CALLBACK WndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{

	static BITMAP bmNave;
	static HDC hdcNave;

	switch (message)
	{
	case WM_CREATE:

		hNave = (HBITMAP)LoadImage(GetModuleHandle(NULL), MAKEINTRESOURCE(IDB_BITMAP1), IMAGE_BITMAP, 0, 0, LR_DEFAULTSIZE);
		hdc = GetDC(hWnd);
		GetObject(hNave, sizeof(bmNave), &bmNave);
		hdcNave = CreateCompatibleDC(hdc);
		SelectObject(hdcNave, hNave);
		ReleaseDC(hWnd, hdc);

		// OBTEM AS DIMENSOES DO DISPLAY... 
		bg = CreateSolidBrush(RGB(255, 128, 128));
		nX = GetSystemMetrics(SM_CXSCREEN);
		nY = GetSystemMetrics(SM_CYSCREEN);

		// PREPARA 'BITMAP' E ASSOCIA A UM 'DC' EM MEMORIA... 
		hdc = GetDC(hWnd);
		auxDC = CreateCompatibleDC(hdc);
		auxBM = CreateCompatibleBitmap(hdc, nX, nY);
		SelectObject(auxDC, auxBM);
		SelectObject(auxDC, GetStockObject(GRAY_BRUSH));
		PatBlt(auxDC, 0, 0, nX, nY, PATCOPY);
		ReleaseDC(hWnd, hdc);


		for (int i = 0; i < NSHIPS; i++)
		{
			BasicInvader nave;
			nave.box.dimension.height = nave.box.dimension.width = 1;
			nave.simb = '*';
			nave.box.position.x = i + 2;
			nave.box.position.y = 1;
			nave.currentDirection = Right;
			nave.lastDirection = nave.currentDirection;
			basicInvaders[i] = nave;
		}

		CreateThread(NULL, 0, (LPTHREAD_START_ROUTINE)move, (void *)hWnd, 0, NULL);

		break;

	case WM_COMMAND:
	{
		int wmId = LOWORD(wParam);
		// Parse the menu selections:
		switch (wmId)
		{
		case IDM_ABOUT:
			DialogBox(hInst, MAKEINTRESOURCE(IDD_ABOUTBOX), hWnd, About);
			break;
		case IDM_EXIT:
			DestroyWindow(hWnd);
			break;
		default:
			return DefWindowProc(hWnd, message, wParam, lParam);
		}
	}
	break;

	case WM_KEYDOWN:
		if (wParam == VK_RIGHT)
		{
			x += 1;
			InvalidateRect(hWnd, NULL, FALSE);
		}
		if (wParam == VK_LEFT)
		{
			x -= 1;
			InvalidateRect(hWnd, NULL, FALSE);
		}
		if (wParam == VK_UP)
		{
			y -= 1;
			InvalidateRect(hWnd, NULL, FALSE);
		}
		if (wParam == VK_DOWN)
		{
			y += 1;
			InvalidateRect(hWnd, NULL, FALSE);
		}
		break;


	case WM_PAINT:
		PAINTSTRUCT ps;


		// DESENHA NO 'DC' EM MEMORIA... 
		for(int i=0;i<NSHIPS; i++)
		{
			//PatBlt(auxDC, 0, 0, nX, nY, PATCOPY);
			//BitBlt(auxDC, x, y, bmNave.bmWidth, bmNave.bmHeight, hdcNave, 0, 0, SRCCOPY);

			PatBlt(auxDC, 0, 0, nX, nY, PATCOPY);

			BitBlt(auxDC, 
				basicInvaders[i].box.position.x, 
				basicInvaders[i].box.position.y, 
				bmNave.bmWidth, 
				bmNave.bmHeight, 
				hdcNave, 0, 0, SRCCOPY);
		}


		// COPIA INFORMACAO DO 'DC' EM MEMORIA PARA O DISPLAY... 
		hdc = BeginPaint(hWnd, &ps);
		BitBlt(hdc, 0, 0, nX, nY, auxDC, 0, 0, SRCCOPY);
		EndPaint(hWnd, &ps);
		break;

	case WM_DESTROY:
		PostQuitMessage(0);
		DeleteObject(hNave);
		DeleteDC(hdcNave);
		// LIBERTA RECURSO RELATIVOS AO 'BITMAP' E AO 'DC' EM MEMORIA... 
		DeleteObject(bg);
		DeleteObject(auxBM);
		DeleteDC(auxDC);
		break;
	default:
		return DefWindowProc(hWnd, message, wParam, lParam);
	}
	return 0;
}

// Message handler for about box.
INT_PTR CALLBACK About(HWND hDlg, UINT message, WPARAM wParam, LPARAM lParam)
{
	UNREFERENCED_PARAMETER(lParam);
	switch (message)
	{
	case WM_INITDIALOG:
		return (INT_PTR)TRUE;

	case WM_COMMAND:
		if (LOWORD(wParam) == IDOK || LOWORD(wParam) == IDCANCEL)
		{
			EndDialog(hDlg, LOWORD(wParam));
			return (INT_PTR)TRUE;
		}
		break;
	}
	return (INT_PTR)FALSE;
}
