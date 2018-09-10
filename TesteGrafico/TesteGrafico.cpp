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

#define MAX_BASIC_INVADERS 200
#define MAX_ELUSIVE_INVADERS 200
#define MAX_PROJECTILES 500
#define MAX_DEFENDERS 1
#define MAX_SHIELDS 5
#define MAX_POWERUPS 1
#define MAX_SHOTS 500
#define MAX_BOMBS 500

#define NBSHIPS 5
#define NESHIPS 1

int NSHOTS = 0;

int ACTIVEPOWERUP = 0;

#define LIM_ESQ 0
#define LIM_CIM 0

#define STEP_ATT_X 1
#define STEP_ATT_Y 1
#define STEP_DEF_X 5
#define STEP_DEF_Y 5

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

enum PowerUpTypes
{
	SHIELD,
	VELOCITY,
	ICE,
	BATTERY,
	ALCOHOL,
	LIFE
};

typedef struct Point {
	  int x;
	  int y;
} Point;

typedef struct Point Position;

typedef unsigned int Count_T;

typedef unsigned int Health_T;
typedef unsigned int PlayerId_T;

typedef struct Dimension {
	unsigned int width;
	unsigned int height;
} Dimension;

typedef struct Box {
	Position position;
	Dimension dimension;
	int type; ////////////////////////////////////
} Box;

typedef struct ExtBasicInvader {
	Box box;
	Direction currentDirection;
	Direction lastDirection;
} ExtBasicInvader;

typedef struct ExtElusiveInvader {
	Box box;
	Direction currentDirection;
	Direction lastDirection;
} ExtElusiveInvader;

typedef struct ExtProjectile {
	Box box;
	unsigned int speed;
} ExtProjectile;

typedef struct ExtDefender {
	Box box;
	PlayerId_T player;
	unsigned int lives;
} ExtDefender;

typedef struct ExtShield {
	Box box;
	Health_T health;
} ExtShield;

typedef struct ExtPowerUp {
	Box box;
	int type;
} ExtPowerUp;


typedef struct ExtGame {
	Dimension dimension;
	ExtBasicInvader basicInvaders[MAX_BASIC_INVADERS];
	Count_T nBasicInvaders;
	ExtElusiveInvader elusiveInvaders[MAX_ELUSIVE_INVADERS];
	Count_T nElusiveInvaders;
	ExtDefender defenders[MAX_DEFENDERS];
	Count_T nDefenders;
	ExtShield shields[MAX_SHIELDS];
	Count_T nShields;
	ExtPowerUp powerUp;
	ExtProjectile shots[MAX_SHOTS];
	Count_T nShots;
	ExtProjectile bombs[MAX_BOMBS];
	Count_T nBombs;
} ExtGame;

typedef struct bitmaps {
	HBITMAP BitmapGlobal;

	HBITMAP Background;

	HBITMAP Basic;
	HBITMAP Elusive;

	HBITMAP Defender;

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




ExtGame game;




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
	wcex.hbrBackground = CreatePatternBrush(data.ClientBitmaps.Background); //(HBRUSH)(COLOR_WINDOW + 1);
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
			switch (game.basicInvaders[i].currentDirection)
			{
			case Right:
				if (game.basicInvaders[i].box.position.x + game.basicInvaders[i].box.dimension.width + STEP_ATT_X < LIM_DIR)
				{
					game.basicInvaders[i].box.position.x += STEP_ATT_X;
					continue;
				}
				else
				{
					if (game.basicInvaders[i].box.position.y + (STEP_ATT_Y * game.basicInvaders[i].box.dimension.height) < LIM_BAI)
					{
						game.basicInvaders[i].box.position.y += (STEP_ATT_Y * game.basicInvaders[i].box.dimension.height);
						game.basicInvaders[i].lastDirection = game.basicInvaders[i].currentDirection;
						game.basicInvaders[i].currentDirection = Left;
						continue;
					}
					else
					{
						game.basicInvaders[i].box.position.x = LIM_ESQ;
						game.basicInvaders[i].box.position.y = LIM_CIM;
						game.basicInvaders[i].lastDirection = game.basicInvaders[i].currentDirection;
						game.basicInvaders[i].currentDirection = Right;
						continue;
					}
				}
				break;

			case Left:
				if (game.basicInvaders[i].box.position.x - STEP_ATT_X > LIM_ESQ)
				{
					game.basicInvaders[i].box.position.x -= STEP_ATT_X;
					continue;
				}
				else
				{
					if (game.basicInvaders[i].box.position.y + (STEP_ATT_Y * game.basicInvaders[i].box.dimension.height) < LIM_BAI)
					{
						game.basicInvaders[i].box.position.y += (STEP_ATT_Y * game.basicInvaders[i].box.dimension.height);
						game.basicInvaders[i].lastDirection = game.basicInvaders[i].currentDirection;
						game.basicInvaders[i].currentDirection = Right;
						continue;
					}
					else
					{
						game.basicInvaders[i].box.position.x = LIM_ESQ; //volta a posicao inicial
						game.basicInvaders[i].box.position.y = LIM_CIM;
						game.basicInvaders[i].lastDirection = game.basicInvaders[i].currentDirection;
						game.basicInvaders[i].currentDirection = Right;
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
	while (1)
	{

		Sleep(20);
		for (int i = 0; i < NESHIPS; i++)
		{
			if (rand() % 100 < 82)
				game.elusiveInvaders[i].currentDirection = game.elusiveInvaders[i].lastDirection;
			else
			{
				int delta = (rand() % 3) - 1;
				game.elusiveInvaders[i].currentDirection = (Direction)abs(((game.elusiveInvaders[i].currentDirection + delta) % NumberOfDirections));
			}

			switch (game.elusiveInvaders[i].currentDirection)
			{
			case Up:
				if ((game.elusiveInvaders[i].box.position.y - STEP_ATT_Y) > LIM_CIM)
				{
					game.elusiveInvaders[i].box.position.y -= STEP_ATT_Y;
					game.elusiveInvaders[i].lastDirection = game.elusiveInvaders[i].currentDirection;
					//continue;
				}
				else
				{
					game.elusiveInvaders[i].box.position.y += STEP_ATT_Y;
					game.elusiveInvaders[i].lastDirection = game.elusiveInvaders[i].currentDirection;
					//continue;
				}
				break;
			case UpRight:
				if ((game.elusiveInvaders[i].box.position.y - STEP_ATT_Y > LIM_CIM) && (game.elusiveInvaders[i].box.position.x + STEP_ATT_X < LIM_DIR))
				{
					game.elusiveInvaders[i].box.position.y -= STEP_ATT_Y;
					game.elusiveInvaders[i].box.position.x += STEP_ATT_X;
					game.elusiveInvaders[i].lastDirection = game.elusiveInvaders[i].currentDirection;
					//continue;
				}
				else
				{
					game.elusiveInvaders[i].box.position.y += STEP_ATT_Y;
					game.elusiveInvaders[i].box.position.x -= STEP_ATT_X;
					game.elusiveInvaders[i].lastDirection = game.elusiveInvaders[i].currentDirection;
					//continue;
				}
				break;
			case Right:
				if (game.elusiveInvaders[i].box.position.x + STEP_ATT_X < LIM_DIR)
				{
					game.elusiveInvaders[i].box.position.x += STEP_ATT_X;
					game.elusiveInvaders[i].lastDirection = game.elusiveInvaders[i].currentDirection;
					//continue;
				}
				else
				{
					game.elusiveInvaders[i].box.position.x -= STEP_ATT_X;
					game.elusiveInvaders[i].lastDirection = game.elusiveInvaders[i].currentDirection;
					//continue;
				}
				break;
			case DownRight:
				if (game.elusiveInvaders[i].box.position.y + STEP_ATT_Y < LIM_BAI && game.elusiveInvaders[i].box.position.x + STEP_ATT_X < LIM_DIR)
				{
					game.elusiveInvaders[i].box.position.y += STEP_ATT_Y;
					game.elusiveInvaders[i].box.position.x += STEP_ATT_X;
					game.elusiveInvaders[i].lastDirection = game.elusiveInvaders[i].currentDirection;
					//continue;
				}
				else
				{
					game.elusiveInvaders[i].box.position.y -= STEP_ATT_Y;
					game.elusiveInvaders[i].box.position.x -= STEP_ATT_X;
					game.elusiveInvaders[i].lastDirection = game.elusiveInvaders[i].currentDirection;
					//continue;
				}
				break;
			case Down:
				if (game.elusiveInvaders[i].box.position.y + STEP_ATT_Y < LIM_BAI)
				{
					game.elusiveInvaders[i].box.position.y += STEP_ATT_Y;
					game.elusiveInvaders[i].lastDirection = game.elusiveInvaders[i].currentDirection;
					//continue;
				}
				else
				{
					game.elusiveInvaders[i].box.position.y -= STEP_ATT_Y;
					game.elusiveInvaders[i].lastDirection = game.elusiveInvaders[i].currentDirection;
					//continue;
				}
				break;
			case DownLeft:
				if (game.elusiveInvaders[i].box.position.y + STEP_ATT_Y < LIM_BAI && game.elusiveInvaders[i].box.position.x - STEP_ATT_X > LIM_ESQ)
				{
					game.elusiveInvaders[i].box.position.y += STEP_ATT_Y;
					game.elusiveInvaders[i].box.position.x -= STEP_ATT_X;
					game.elusiveInvaders[i].lastDirection = game.elusiveInvaders[i].currentDirection;
					//continue;
				}
				else
				{
					game.elusiveInvaders[i].box.position.y -= STEP_ATT_Y;
					game.elusiveInvaders[i].box.position.x += STEP_ATT_X;
					game.elusiveInvaders[i].lastDirection = game.elusiveInvaders[i].currentDirection;
					//continue;
				}
				break;
			case Left:
				if (game.elusiveInvaders[i].box.position.x - STEP_ATT_X > LIM_ESQ)
				{
					game.elusiveInvaders[i].box.position.x -= STEP_ATT_X;
					game.elusiveInvaders[i].lastDirection = game.elusiveInvaders[i].currentDirection;
					//continue;
				}
				else
				{
					game.elusiveInvaders[i].box.position.x += STEP_ATT_X;
					game.elusiveInvaders[i].lastDirection = game.elusiveInvaders[i].currentDirection;
					//continue;
				}
				break;
			case UpLeft:
				if (game.elusiveInvaders[i].box.position.y - STEP_ATT_Y > LIM_CIM && game.elusiveInvaders[i].box.position.x - STEP_ATT_X > LIM_ESQ)
				{
					game.elusiveInvaders[i].box.position.y -= STEP_ATT_Y;
					game.elusiveInvaders[i].box.position.x -= STEP_ATT_X;
					game.elusiveInvaders[i].lastDirection = game.elusiveInvaders[i].currentDirection;
					//continue;
				}
				else
				{
					game.elusiveInvaders[i].box.position.y += STEP_ATT_Y;
					game.elusiveInvaders[i].box.position.x += STEP_ATT_X;
					game.elusiveInvaders[i].lastDirection = game.elusiveInvaders[i].currentDirection;
					//continue;
				}
				break;
			}
		}
	}
}

void * powerupThread(void * in)
{
	while (1)
	{
		Sleep(10000);
		if (((rand() % 100) + 1) > 25)
		{
			ACTIVEPOWERUP = 1;

			game.powerUp.box.position.x = WINDOW_WIDTH / 2;
			game.powerUp.box.position.y = LIM_CIM;

			//srand(time(NULL));
			int pType = (rand() % 75 + 1); //1 to 75

			if (pType >= 1 && pType < 20) //common
				game.powerUp.type = SHIELD;
			else if (pType >= 21 && pType <= 40)
				game.powerUp.type = VELOCITY;
			else if (pType >= 41 && pType <= 50) //uncommon
				game.powerUp.type = ICE;
			else if (pType >= 51 && pType <= 60)
				game.powerUp.type = BATTERY;
			else if (pType >= 61 && pType <= 70)
				game.powerUp.type = ALCOHOL;
			else //if (pType >= 71 && pType <= 75) //rare
				game.powerUp.type = LIFE;

			while (game.powerUp.box.position.y < LIM_BAI)
			{
				Sleep(10);
				game.powerUp.box.position.y += STEP_ATT_Y;
			}

			ACTIVEPOWERUP = 0;
		}
	}
}

int UnloadClientBitmaps()
{
	CloseHandle(data.ClientBitmaps.BitmapGlobal);
	CloseHandle(data.ClientBitmaps.Background);
	CloseHandle(data.ClientBitmaps.Basic);
	CloseHandle(data.ClientBitmaps.Elusive);
	CloseHandle(data.ClientBitmaps.Defender);
	CloseHandle(data.ClientBitmaps.InvaderProjectile);
	CloseHandle(data.ClientBitmaps.PowerupShield);
	CloseHandle(data.ClientBitmaps.PowerupAlcohol);
	CloseHandle(data.ClientBitmaps.PowerupVelocity);
	CloseHandle(data.ClientBitmaps.PowerupBattery);
	CloseHandle(data.ClientBitmaps.PowerupIce);
	CloseHandle(data.ClientBitmaps.PowerupLife);

	return 0;
}

int LoadClientBitmaps()
{
	HDC _hdc;

	//miscellaneous
	data.ClientBitmaps.Background = LoadBitmap(hInst, MAKEINTRESOURCE(IDB_CLIENTBKG));

	//SHIPS
	data.ClientBitmaps.Basic = LoadBitmap(hInst, MAKEINTRESOURCE(IDB_BASICSHIP));          // 50*50
	data.ClientBitmaps.Elusive = LoadBitmap(hInst, MAKEINTRESOURCE(IDB_ELUSIVESHIP));

	data.ClientBitmaps.Defender = LoadBitmap(hInst, MAKEINTRESOURCE(IDB_DEFENDERSHIP));

	//PROJECTILES
	data.ClientBitmaps.InvaderProjectile = LoadBitmap(hInst, MAKEINTRESOURCE(IDB_INVPROJ));
	//data.ClientBitmaps.DefenderProjectile


	//POWERUPS
	data.ClientBitmaps.PowerupShield = LoadBitmap(hInst, MAKEINTRESOURCE(IDB_POWSHIELD));  // 40*40
	data.ClientBitmaps.PowerupIce = LoadBitmap(hInst, MAKEINTRESOURCE(IDB_POWICE));
	data.ClientBitmaps.PowerupBattery = LoadBitmap(hInst, MAKEINTRESOURCE(IDB_POWBATTERY));
	data.ClientBitmaps.PowerupVelocity = LoadBitmap(hInst, MAKEINTRESOURCE(IDB_POWVELOCITY));
	data.ClientBitmaps.PowerupLife = LoadBitmap(hInst, MAKEINTRESOURCE(IDB_POWLIFE));
	data.ClientBitmaps.PowerupAlcohol = LoadBitmap(hInst, MAKEINTRESOURCE(IDB_POWALCOHOL));

	//data.ClientBitmaps.

	nX = GetSystemMetrics(SM_CXSCREEN);
	nY = GetSystemMetrics(SM_CYSCREEN);

	_hdc = GetDC(janela_principal);

	data.ClientBitmaps.BitmapGlobal = CreateCompatibleBitmap(_hdc, nX, nY);
	data.memDC = CreateCompatibleDC(_hdc);
	SelectObject(data.memDC, data.ClientBitmaps.BitmapGlobal);

	DeleteDC(_hdc);

	return 0;
}

void * moveEnemyProjectiles(void * in)
{
	ExtProjectile * shotPointer = (ExtProjectile *)in;

	while (shotPointer->box.position.y < LIM_BAI)
	{
		Sleep(50);
		shotPointer->box.position.y += STEP_ATT_Y;
	}
	NSHOTS--;
	return in;
}

void EnemyFire() //space
{
	BITMAP shotB;

	for (int i = 0; i < game.nBombs; i++)
	{
		GetObject(data.ClientBitmaps.InvaderProjectile, sizeof(shotB), &shotB);
		game.bombs[i].box.dimension.width = shotB.bmWidth;
		game.bombs[i].box.dimension.height = shotB.bmHeight;
		game.bombs[i].box.position.x = i * 50;
		game.bombs[i].box.position.y = LIM_CIM * game.bombs[i].box.dimension.height * 2;
		game.bombs[i].speed = 10;
		game.bombs[i].box.type = 1; //TODO types of shots enum

		NSHOTS++;
	}

	
	CreateThread(NULL, 0, (LPTHREAD_START_ROUTINE)moveEnemyProjectiles, (LPVOID)game.bombs, 0, NULL);


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
	static BITMAP basicShip, elusiveShip, defenderShip;
	static HDC hdcNave;

	HDC hdc = NULL;

	switch (message)
	{
	case WM_CREATE:

		// OBTEM AS DIMENSOES DO DISPLAY... 
		bg = CreateSolidBrush(RGB(255, 128, 128));
		nX = GetSystemMetrics(SM_CXSCREEN);
		nY = GetSystemMetrics(SM_CYSCREEN);

		game.nDefenders = MAX_DEFENDERS;
		game.nBombs = MAX_BOMBS;
		game.nBasicInvaders = MAX_BASIC_INVADERS;
		game.nElusiveInvaders = MAX_ELUSIVE_INVADERS;
		game.nShields = MAX_SHIELDS;
		game.nShots = MAX_SHOTS;

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

		for (int i = 0; i < game.nBasicInvaders; i++)
		{
			ExtBasicInvader nave;
			nave.box.type = 1; //TODO enum
			GetObject(data.ClientBitmaps.Basic, sizeof(basicShip), &basicShip);
			nave.box.dimension.width = basicShip.bmWidth;
			nave.box.dimension.height = basicShip.bmHeight;
			nave.box.position.x = ((i + 1) * nave.box.dimension.width) + ((i + 1) * SHIP_SPACING);
			nave.box.position.y = LIM_CIM;
			nave.currentDirection = Right;
			nave.lastDirection = nave.currentDirection;
			game.basicInvaders[i] = nave;

		}

		for (int i = 0; i < NESHIPS; i++)
		{
			ExtElusiveInvader nave;
			nave.box.type = 2;
			GetObject(data.ClientBitmaps.Elusive, sizeof(elusiveShip), &elusiveShip);
			//nave.box.dimension.height = nave.box.dimension.width = 1;
			nave.box.dimension.width = elusiveShip.bmWidth;
			nave.box.dimension.height = elusiveShip.bmHeight;
			nave.box.position.x = ((i + 1) * nave.box.dimension.width) + ((i + 1) * SHIP_SPACING);
			nave.box.position.y = LIM_CIM * nave.box.dimension.height;
			nave.currentDirection = (Direction)(rand() % NumberOfDirections);
			nave.lastDirection = nave.currentDirection;
			game.elusiveInvaders[i] = nave;

		}

		//defender
		GetObject(data.ClientBitmaps.Defender, sizeof(defenderShip), &defenderShip);
		game.defenders[0].box.dimension.width = defenderShip.bmWidth;
		game.defenders[0].box.dimension.height = defenderShip.bmHeight;
		game.defenders[0].box.position.x = (game.defenders[0].box.dimension.width * 2) + SHIP_SPACING;
		game.defenders[0].box.position.y = LIM_BAI - 3 * game.defenders[0].box.dimension.height;

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
		if (wParam == VK_SPACE)
		{
			EnemyFire();
			InvalidateRect(hWnd, NULL, FALSE);
		}
		if (wParam == VK_RIGHT)
		{
			game.defenders[0].box.position.x += STEP_DEF_X;
			InvalidateRect(hWnd, NULL, FALSE);
		}
		if (wParam == VK_LEFT)
		{
			game.defenders[0].box.position.x -= STEP_DEF_X;
			InvalidateRect(hWnd, NULL, FALSE);
		}
		if (wParam == VK_UP)
		{
			game.defenders[0].box.position.y -= STEP_DEF_Y;
			InvalidateRect(hWnd, NULL, FALSE);
		}
		if (wParam == VK_DOWN)
		{
			game.defenders[0].box.position.y += STEP_DEF_Y;
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
		UnloadClientBitmaps();
		DeleteDC(hdc);
		DeleteDC(hdcNave);
		DeleteDC(data.memDC);
		DeleteObject(bg);
		DeleteObject(auxBM);
		PostQuitMessage(0);
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

		GdiTransparentBlt(data.memDC, game.basicInvaders[i].box.position.x, game.basicInvaders[i].box.position.y, 50, 50, auxRenderDC, 0, 0, 50, 50, (RGB(255, 255, 255)));

		/*
		BitBlt(
				data.memDC,
				game.basicInvaders[i].box.position.x,
				game.basicInvaders[i].box.position.y,
				50, 50, auxRenderDC, 0, 0, SRCCOPY);
		*/
	}

	for (int i = 0; i < NESHIPS; i++)
	{
		data.ClientBitmaps.BitmapGlobal = data.ClientBitmaps.Elusive;

		SelectObject(auxRenderDC, data.ClientBitmaps.BitmapGlobal);

		GdiTransparentBlt(data.memDC, game.elusiveInvaders[i].box.position.x, game.elusiveInvaders[i].box.position.y, 50, 50, auxRenderDC, 0, 0, 50, 50, (RGB(255, 255, 255)));

	}


	for (int i = 0; i < NSHOTS; i++)
	{
		data.ClientBitmaps.BitmapGlobal = data.ClientBitmaps.InvaderProjectile;

		SelectObject(auxRenderDC, data.ClientBitmaps.BitmapGlobal);

		GdiTransparentBlt(data.memDC, game.bombs[i].box.position.x, game.bombs[i].box.position.y, 22, 39, auxRenderDC, 0, 0, 22, 39, (RGB(255, 255, 255)));

	}

	if (1 == ACTIVEPOWERUP)
	{

		switch (game.powerUp.type)
		{
		case SHIELD:

			data.ClientBitmaps.BitmapGlobal = data.ClientBitmaps.PowerupShield;
			break;

		case VELOCITY:

			data.ClientBitmaps.BitmapGlobal = data.ClientBitmaps.PowerupVelocity;
			break;

		case ICE:

			data.ClientBitmaps.BitmapGlobal = data.ClientBitmaps.PowerupIce;
			break;

		case BATTERY:

			data.ClientBitmaps.BitmapGlobal = data.ClientBitmaps.PowerupBattery;
			break;

		case ALCOHOL:

			data.ClientBitmaps.BitmapGlobal = data.ClientBitmaps.PowerupAlcohol;
			break;

		case LIFE:

			data.ClientBitmaps.BitmapGlobal = data.ClientBitmaps.PowerupLife;
			break;

		}

		SelectObject(auxRenderDC, data.ClientBitmaps.BitmapGlobal);

		GdiTransparentBlt(data.memDC, game.powerUp.box.position.x, game.powerUp.box.position.y, 40, 40, auxRenderDC, 0, 0, 40, 40, (RGB(255, 255, 255)));

	}

	for (int i = 0; i < MAX_DEFENDERS; i++)
	{
		data.ClientBitmaps.BitmapGlobal = data.ClientBitmaps.Defender;

		SelectObject(auxRenderDC, data.ClientBitmaps.BitmapGlobal);

		GdiTransparentBlt(data.memDC, game.defenders[i].box.position.x, game.defenders[i].box.position.y, 100, 50, auxRenderDC, 0, 0, 100, 50, (RGB(255, 255, 255)));

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

