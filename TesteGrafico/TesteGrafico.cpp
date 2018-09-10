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
#include <winuser.h>
#include <winuser.h>

#define MAX_BASIC_INVADERS 200
#define MAX_ELUSIVE_INVADERS 200
#define MAX_PROJECTILES 500

#define NBSHIPS 5
#define NESHIPS 2

int NSHOTS = 0;

int ACTIVEPOWERUP = 0;

#define LIM_ESQ 0
#define LIM_CIM 0
#define PASSO_X 1
#define PASSO_Y 1

#define WINDOW_WIDTH 600
#define WINDOW_HEIGHT 800

const int LIM_BAI = WINDOW_HEIGHT;
const int LIM_DIR = WINDOW_WIDTH; //GetSystemMetrics(SM_CXSCREEN);

#define SHIP_SPACING 25 //pixels

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
	int type; ////////////////////////////////////
} Box;

typedef struct BasicInvader {
	Box box;
	Direction currentDirection;
	Direction lastDirection;
} BasicInvader;

typedef struct ElusiveInvader {
	Box box;
	Direction currentDirection;
	Direction lastDirection;
} ElusiveInvader;

typedef struct Projectile {
	Box box;
	unsigned int speed;
} Projectile;

typedef struct PowerUp {
	Box box;
	int type;
} PowerUp;

/* An array of structures representing the basic invaders */
BasicInvader basicInvaders[MAX_BASIC_INVADERS];

/* An array of structures representing the elusive invaders */
ElusiveInvader elusiveInvaders[MAX_ELUSIVE_INVADERS];

Projectile projectiles[MAX_PROJECTILES];

PowerUp powerup;

typedef struct bitmaps{
	HBITMAP BitmapGlobal;

	HBITMAP Background;

	HBITMAP Basic;
	HBITMAP Elusive;
	HBITMAP InvaderProjectile;
	HBITMAP DefenderProjectile;

	HBITMAP PowerupShield;
	HBITMAP PowerupIce;
	HBITMAP PowerupBattery;
	HBITMAP PowerupVelocity;
	HBITMAP PowerupLife;
	HBITMAP PowerupAlcohol;

} Bitmaps;


typedef struct clientdata {
	HDC memDC;

	Bitmaps ClientBitmaps;

} ClientData;

ClientData data;

#define MAX_LOADSTRING 100

// Global Variables:
HINSTANCE hInst;                                // current instance
WCHAR szTitle[MAX_LOADSTRING];                  // The title bar text
WCHAR szWindowClass[MAX_LOADSTRING];            // the main window class name


int x, y;
HBRUSH bg = NULL;
HBITMAP auxBM = NULL;
int nX = 0, nY = 0; //screen size
int nJog = 0;

HWND janela_principal;



DWORD WINAPI threadRenderMap(LPVOID * param);

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
	srand(time(NULL));



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

	HWND hWnd = CreateWindowW(szWindowClass, 
		szTitle, 
		WS_OVERLAPPED | WS_CAPTION | WS_SYSMENU | WS_MINIMIZEBOX | WS_MAXIMIZEBOX,
		CW_USEDEFAULT, 0, 
		WINDOW_WIDTH, WINDOW_HEIGHT, 
		nullptr, nullptr, hInstance, nullptr);

	if (!hWnd)
	{
		return FALSE;
	}

	ShowWindow(hWnd, nCmdShow);
	UpdateWindow(hWnd);

	janela_principal = hWnd;

	return TRUE;
}

void * moveBasicShips(void * in)
{
	HWND hWnd = (HWND)in;

	while (1) {

		Sleep(10);
		for (int i = 0; i < NBSHIPS; i++)
		{
			switch (basicInvaders[i].currentDirection)
			{
			case Right:
				if (basicInvaders[i].box.position.x + basicInvaders[i].box.dimension.width + PASSO_X < LIM_DIR)
				{
					basicInvaders[i].box.position.x += PASSO_X;
					continue;
				}
				else
				{
					if (basicInvaders[i].box.position.y + (PASSO_Y * basicInvaders[i].box.dimension.height) < LIM_BAI)
					{
						basicInvaders[i].box.position.y += (PASSO_Y * basicInvaders[i].box.dimension.height);
						basicInvaders[i].lastDirection = basicInvaders[i].currentDirection;
						basicInvaders[i].currentDirection = Left;
						continue;
					}
					else
					{
						basicInvaders[i].box.position.x = LIM_ESQ;
						basicInvaders[i].box.position.y = LIM_CIM;
						basicInvaders[i].lastDirection = basicInvaders[i].currentDirection;
						basicInvaders[i].currentDirection = Right;
						continue;
					}
				}
				break;

			case Left:
				if (basicInvaders[i].box.position.x - PASSO_X > LIM_ESQ)
				{
					basicInvaders[i].box.position.x -= PASSO_X;
					continue;
				}
				else
				{
					if (basicInvaders[i].box.position.y + (PASSO_Y * basicInvaders[i].box.dimension.height) < LIM_BAI)
					{
						basicInvaders[i].box.position.y += (PASSO_Y * basicInvaders[i].box.dimension.height);
						basicInvaders[i].lastDirection = basicInvaders[i].currentDirection;
						basicInvaders[i].currentDirection = Right;
						continue;
					}
					else
					{
						basicInvaders[i].box.position.x = LIM_ESQ; //volta a posicao inicial
						basicInvaders[i].box.position.y = LIM_CIM;
						basicInvaders[i].lastDirection = basicInvaders[i].currentDirection;
						basicInvaders[i].currentDirection = Right;
						continue;
					}
				}
				break;
			}

		}
	}

}

void * moveElusiveShips(void * in)
{
	HWND hWnd = (HWND)in;
	
	while (1)
	{

		Sleep(10);
		for (int i = 0; i < NESHIPS; i++)
		{
			int delta = (rand() % 3) - 1;

			elusiveInvaders[i].currentDirection = (Direction)abs(((elusiveInvaders[i].currentDirection + delta) % NumberOfDirections));

			switch (elusiveInvaders[i].currentDirection)
			{
			case Up:
				if (elusiveInvaders[i].box.position.y - PASSO_Y > LIM_CIM)
				{
					elusiveInvaders[i].box.position.y -= PASSO_Y;
					continue;
				}
				break;
			case UpRight:
				if ((elusiveInvaders[i].box.position.y - PASSO_Y > LIM_CIM) && (elusiveInvaders[i].box.position.x + PASSO_X < LIM_DIR))
				{
					elusiveInvaders[i].box.position.y -= PASSO_Y;
					elusiveInvaders[i].box.position.x += PASSO_X;
					elusiveInvaders[i].lastDirection = elusiveInvaders[i].currentDirection;
					continue;
				}
				break;
			case Right:
				if (elusiveInvaders[i].box.position.y + PASSO_X < LIM_DIR)
				{
					elusiveInvaders[i].box.position.x += PASSO_X;
					elusiveInvaders[i].lastDirection = elusiveInvaders[i].currentDirection;
					continue;
				}
				break;
			case DownRight:
				if (elusiveInvaders[i].box.position.y + PASSO_Y < LIM_BAI && elusiveInvaders[i].box.position.x + PASSO_X < LIM_DIR)
				{
					elusiveInvaders[i].box.position.y += PASSO_Y;
					elusiveInvaders[i].box.position.x += PASSO_X;
					elusiveInvaders[i].lastDirection = elusiveInvaders[i].currentDirection;
					continue;
				}
				break;
			case Down:
				if (elusiveInvaders[i].box.position.y - PASSO_Y < LIM_BAI)
				{
					elusiveInvaders[i].box.position.y += PASSO_Y;
					elusiveInvaders[i].lastDirection = elusiveInvaders[i].currentDirection;
					continue;
				}
				break;
			case DownLeft:
				if (elusiveInvaders[i].box.position.y + PASSO_Y < LIM_BAI && elusiveInvaders[i].box.position.x - PASSO_X > LIM_ESQ)
				{
					elusiveInvaders[i].box.position.y += PASSO_Y;
					elusiveInvaders[i].box.position.x -= PASSO_X;
					elusiveInvaders[i].lastDirection = elusiveInvaders[i].currentDirection;
					continue;
				}
				break;
			case Left:
				if (elusiveInvaders[i].box.position.x - PASSO_X > LIM_ESQ)
				{
					elusiveInvaders[i].box.position.x -= PASSO_X;
					elusiveInvaders[i].lastDirection = elusiveInvaders[i].currentDirection;
					continue;
				}
				break;
			case UpLeft:
				if (elusiveInvaders[i].box.position.y - PASSO_Y > LIM_CIM && elusiveInvaders[i].box.position.x - PASSO_X > LIM_ESQ)
				{
					elusiveInvaders[i].box.position.y -= PASSO_Y;
					elusiveInvaders[i].box.position.x -= PASSO_X;
					elusiveInvaders[i].lastDirection = elusiveInvaders[i].currentDirection;
					continue;
				}
				break;
			}
		}
	}
}

void * powerupThread(void * in)
{
	while(1)
	{
		Sleep(2000);
		if(((rand() % 100) + 1) > 85)
		{
			ACTIVEPOWERUP = 1;

			powerup.box.position.x = WINDOW_WIDTH / 2;
			powerup.box.position.y = LIM_CIM;
			/*
			unsigned int pType = rand() * 75 + 1; //1 to 75

			if (pType >= 1 && pType <= 20) //common
				powerup.type = 1;
			else if (pType > 21 && pType <= 40)
				powerup.type = 2;
			else if (pType >= 41 && pType <= 50) //uncommon
				powerup.type = 3;
			else if (pType >= 51 && pType <= 60)
				powerup.type = 4;
			else if (pType >= 61 && pType <= 70)
				powerup.type = 5;
			else //if (pType >= 71 && pType <= 75) //rare
				powerup.type = 6;
				*/
			while (powerup.box.position.y < LIM_BAI)
			{
				Sleep(25);
				powerup.box.position.y += PASSO_Y;
			}

			ACTIVEPOWERUP = 0;
		}
	}
}

int LoadClientBitmaps()
{
	HDC _hdc;

	data.ClientBitmaps.Background = LoadBitmap(hInst, MAKEINTRESOURCE(IDB_CLIENTBKG));

	data.ClientBitmaps.Basic = LoadBitmap(hInst, MAKEINTRESOURCE(IDB_BASICSHIP));
	data.ClientBitmaps.Elusive = LoadBitmap(hInst, MAKEINTRESOURCE(IDB_ELUSIVESHIP));// 50*50

	data.ClientBitmaps.InvaderProjectile = LoadBitmap(hInst, MAKEINTRESOURCE(IDB_INVPROJ));
	//data.ClientBitmaps.PowerupShield = 
	data.ClientBitmaps.PowerupIce = LoadBitmap(hInst, MAKEINTRESOURCE(IDB_POWICE)); //40*40
	//data.ClientBitmaps.PowerupBattery
	//data.ClientBitmaps.PowerupVelocity
	//data.ClientBitmaps.PowerupLife
	//data.ClientBitmaps.PowerupAlcohol

	//data.ClientBitmaps.

	nX = GetSystemMetrics(SM_CXSCREEN);
	nY = GetSystemMetrics(SM_CYSCREEN);

	_hdc = GetDC(janela_principal);

	data.ClientBitmaps.BitmapGlobal = CreateCompatibleBitmap(_hdc, nX, nY);
	data.memDC = CreateCompatibleDC(_hdc);
	SelectObject(data.memDC, data.ClientBitmaps.BitmapGlobal);

	return 0;
}

void * WINAPI moveEnemyProjectiles(void * in)
{
	Projectile * shotPointer = (Projectile *)in;

	while(shotPointer->box.position.y < LIM_BAI)
	{
		Sleep(50);
		shotPointer->box.position.y += PASSO_Y;
	}
	NSHOTS--;
	return nullptr;
}

void EnemyFire() //enter
{
	BITMAP shotB;

	//Projectile shot = projectiles[0];
	GetObject(data.ClientBitmaps.InvaderProjectile, sizeof(shotB), &shotB);
	projectiles[0].box.dimension.width = shotB.bmWidth;
	projectiles[0].box.dimension.height = shotB.bmHeight;
	projectiles[0].box.position.x = 200;
	projectiles[0].box.position.y = LIM_CIM * projectiles[0].box.dimension.height * 2;
	projectiles[0].speed = 10;
	projectiles[0].box.type = 1; //TODO types of shots enum

	NSHOTS++;

	Projectile * shotPointer = &projectiles[0];
	
	CreateThread(NULL, 0, (LPTHREAD_START_ROUTINE)moveEnemyProjectiles, (LPVOID)shotPointer, 0, NULL);

	
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
LRESULT CALLBACK WndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{
	static BITMAP basicShip, elusiveShip;
	static HDC hdcNave;

	HDC hdc = NULL;

	switch (message)
	{
	case WM_CREATE:

		// OBTEM AS DIMENSOES DO DISPLAY... 
		bg = CreateSolidBrush(RGB(255, 128, 128));
		nX = GetSystemMetrics(SM_CXSCREEN);
		nY = GetSystemMetrics(SM_CYSCREEN);

		LoadClientBitmaps();
		/*
		hNave = (HBITMAP)LoadImage(GetModuleHandle(NULL), MAKEINTRESOURCE(IDB_BITMAP1), IMAGE_BITMAP, 0, 0, LR_DEFAULTSIZE);
		hdc = GetDC(hWnd);
		GetObject(hNave, sizeof(bmNave), &bmNave);
		hdcNave = CreateCompatibleDC(hdc);
		SelectObject(hdcNave, hNave);
		ReleaseDC(hWnd, hdc);

		
		// PREPARA 'BITMAP' E ASSOCIA A UM 'DC' EM MEMORIA... 
		hdc = GetDC(hWnd);
		auxDC = CreateCompatibleDC(hdc);
		auxBM = CreateCompatibleBitmap(hdc, nX, nY);
		SelectObject(auxDC, auxBM);
		SelectObject(auxDC, GetStockObject(GRAY_BRUSH));
		PatBlt(auxDC, 0, 0, nX, nY, PATCOPY);
		ReleaseDC(hWnd, hdc);
		*/

		for (int i = 0; i < NBSHIPS; i++)
		{
			BasicInvader nave;
			nave.box.type = 1; //TODO enum
			GetObject(data.ClientBitmaps.Basic, sizeof(basicShip), &basicShip);
			nave.box.dimension.width = basicShip.bmWidth;
			nave.box.dimension.height = basicShip.bmHeight;
			nave.box.position.x = ((i + 1) * nave.box.dimension.width) + ((i + 1) * SHIP_SPACING);
			nave.box.position.y = LIM_CIM;
			nave.currentDirection = Right;
			nave.lastDirection = nave.currentDirection;
			basicInvaders[i] = nave;

		}

		for(int i=0; i<NESHIPS; i++)
		{
			ElusiveInvader nave;
			nave.box.type = 2;
			GetObject(data.ClientBitmaps.Elusive, sizeof(elusiveShip), &elusiveShip);
			nave.box.dimension.height = nave.box.dimension.width = 1;
			nave.box.position.x = ((i + 1) * nave.box.dimension.width) + ((i + 1) * SHIP_SPACING);
			nave.box.position.y = LIM_CIM * nave.box.dimension.height;
			nave.currentDirection = (Direction)(rand() % NumberOfDirections);
			nave.lastDirection = nave.currentDirection;
			elusiveInvaders[i] = nave;

		}

		CreateThread(NULL, 0, (LPTHREAD_START_ROUTINE)moveBasicShips, (void *)hWnd, 0, NULL);
		CreateThread(NULL, 0, (LPTHREAD_START_ROUTINE)moveElusiveShips, (void *)hWnd, 0, NULL);
		CreateThread(NULL, 0, (LPTHREAD_START_ROUTINE)powerupThread, NULL, 0, NULL);
		CreateThread(NULL, 0, (LPTHREAD_START_ROUTINE)threadRenderMap, NULL, 0, NULL);

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
		if(wParam == VK_SPACE)
		{
			EnemyFire();
			InvalidateRect(hWnd, NULL, FALSE);
		}
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

		hdc = BeginPaint(hWnd, &ps);


		// COPIA INFORMACAO DO 'DC' EM MEMORIA PARA O DISPLAY... 

		BitBlt(hdc, 0, 0, nX, nY, data.memDC, 0, 0, SRCCOPY);

		EndPaint(hWnd, &ps);
		break;

	case WM_DESTROY:
		PostQuitMessage(0);
		DeleteDC(hdcNave);
		// LIBERTA RECURSO RELATIVOS AO 'BITMAP' E AO 'DC' EM MEMORIA... 
		DeleteObject(bg);
		DeleteObject(auxBM);
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


int renderLocalMap()
{
	HDC _hdc, auxRenderDC;

	_hdc = GetDC(janela_principal);

	auxRenderDC = CreateCompatibleDC(_hdc);

	ReleaseDC(janela_principal, _hdc);

	SelectObject(auxRenderDC, data.ClientBitmaps.BitmapGlobal);

	RECT rectangle = { 0, 0, nX, nY };
	FillRect(data.memDC, &rectangle, CreatePatternBrush(LoadBitmap(hInst, MAKEINTRESOURCE(IDB_CLIENTBKG))));

	for (int i = 0; i < NBSHIPS; i++)
	{
		//TODO if para selecionar bitmap - funcao que devolve o q quero
		data.ClientBitmaps.BitmapGlobal = data.ClientBitmaps.Basic;

		SelectObject(auxRenderDC, data.ClientBitmaps.BitmapGlobal);

		GdiTransparentBlt(data.memDC, basicInvaders[i].box.position.x, basicInvaders[i].box.position.y, 50, 50, auxRenderDC, 0, 0, 50, 50, (RGB(255, 255, 255)));
			
		/*
		BitBlt(
				data.memDC,
				basicInvaders[i].box.position.x,
				basicInvaders[i].box.position.y,
				50, 50, auxRenderDC, 0, 0, SRCCOPY);
		*/
	}

	for (int i = 0; i < NESHIPS; i++)
	{
		data.ClientBitmaps.BitmapGlobal = data.ClientBitmaps.Elusive;

		SelectObject(auxRenderDC, data.ClientBitmaps.BitmapGlobal);

		GdiTransparentBlt(data.memDC, elusiveInvaders[i].box.position.x, elusiveInvaders[i].box.position.y, 50, 50, auxRenderDC, 0, 0, 50, 50, (RGB(255, 255, 255)));

	}


	for (int i = 0; i < NSHOTS; i++)
	{
		data.ClientBitmaps.BitmapGlobal = data.ClientBitmaps.InvaderProjectile;

		SelectObject(auxRenderDC, data.ClientBitmaps.BitmapGlobal);

		GdiTransparentBlt(data.memDC, projectiles[i].box.position.x, projectiles[i].box.position.y, 22, 39, auxRenderDC, 0, 0, 22, 39, (RGB(255, 255, 255)));

	}

	if(1 == ACTIVEPOWERUP)
	{
		//TODO resto dos pups
		data.ClientBitmaps.BitmapGlobal = data.ClientBitmaps.PowerupIce;

		SelectObject(auxRenderDC, data.ClientBitmaps.BitmapGlobal);

		GdiTransparentBlt(data.memDC, powerup.box.position.x, powerup.box.position.y, 40, 40, auxRenderDC, 0, 0, 40, 40, (RGB(255, 255, 255)));

	}

	InvalidateRect(janela_principal, NULL, FALSE);

	DeleteDC(auxRenderDC);

	return 0;
}

DWORD WINAPI threadRenderMap(LPVOID * param)
{
	//while (!clientData.threads.renderMap.dieThread)
	{
		//WaitForSingleObject(clientData.events.startRendering, INFINITE);

		while (TRUE)//while (clientData.isGameRunning)
		{
			Sleep(42);

			renderLocalMap();
		}
	}

	return 0;
}

