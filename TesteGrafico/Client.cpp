#include "stdafx.h"
#include "Client.h"
#include <minwinbase.h>
#include "ServerBootstrap.h"
#include <winuser.h>


Game gameShared;


ClientData data;
HANDLE hServerThread;
DWORD serverThreadId;

int APIENTRY wWinMain(_In_ HINSTANCE hInstance,
	_In_opt_ HINSTANCE hPrevInstance,
	_In_ LPWSTR    lpCmdLine,
	_In_ int       nCmdShow)
{
	UNREFERENCED_PARAMETER(hPrevInstance);
	UNREFERENCED_PARAMETER(lpCmdLine);

	// TODO: Place code here.
	//srand(time(NULL));

	


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
		600, 800,
		nullptr, nullptr, hInstance, nullptr);

	if (!hWnd)
	{
		return FALSE;
	}

	ShowWindow(hWnd, nCmdShow);
	UpdateWindow(hWnd);

	mainWindow = hWnd;


	if (NULL == (hServerThread = CreateThread(
		NULL,
		0,
		serverMain,
		&gameShared,
		0,
		&serverThreadId)))
	{
		MessageBox(hWnd, TEXT("Cannot start server thread"), TEXT("ERROR"), 0);
		return FALSE;
	}


	return TRUE;
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
	
	static HDC hdcNave;

	HDC hdc = NULL;

	ThreadGames threadGames;
	//threadGames.game = 
	//threadGames->gameShared;

	switch (message)
	{
	case WM_CREATE:

		LoadClientBitmaps(data);

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
	/*
	case WM_KEYDOWN:
		if (wParam == VK_SPACE)
		{
			EnemyFire();
			InvalidateRect(hWnd, NULL, FALSE);
		}
		if (wParam == VK_CONTROL)
		{
			CreateShot();
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

		*/
	case WM_PAINT:
		PAINTSTRUCT ps;

		hdc = BeginPaint(hWnd, &ps);


		// COPIA INFORMACAO DO 'DC' EM MEMORIA PARA O DISPLAY... 

		BitBlt(hdc, 0, 0, screenWidth, screenHeight, data.memDC, 0, 0, SRCCOPY);

		EndPaint(hWnd, &ps);
		break;

	case WM_DESTROY:
		DeleteDC(hdc);
		DeleteDC(hdcNave);
		DeleteDC(data.memDC);
		PostQuitMessage(0);
		break;
	default:
		return DefWindowProc(hWnd, message, wParam, lParam);
	}
	return 0;
}


//FUNCTIONS 

int LoadClientBitmaps(ClientData data)
{
	HDC _hdc;

	//GLOBAL
	data.ClientBitmaps.GlobalBitmap = LoadBitmap(hInst, MAKEINTRESOURCE(IDB_CLEANER));		//100*50

	//MISCELLANEOUS
	data.ClientBitmaps.Background = LoadBitmap(hInst, MAKEINTRESOURCE(IDB_CLIENTBKG));
	data.ClientBitmaps.Cleaner = LoadBitmap(hInst, MAKEINTRESOURCE(IDB_CLEANER));			//100*50

	//SHIPS
	data.ClientBitmaps.Basic = LoadBitmap(hInst, MAKEINTRESOURCE(IDB_BASICSHIP));			// 50*50
	data.ClientBitmaps.Elusive = LoadBitmap(hInst, MAKEINTRESOURCE(IDB_ELUSIVESHIP));

	//DEFENDER
	data.ClientBitmaps.Defender = LoadBitmap(hInst, MAKEINTRESOURCE(IDB_DEFENDERSHIP));		//100*50

	//PROJECTILES
	data.ClientBitmaps.Bomb = LoadBitmap(hInst, MAKEINTRESOURCE(IDB_BOMB));					//15*30
	data.ClientBitmaps.Shot = LoadBitmap(hInst, MAKEINTRESOURCE(IDB_SHOT));

	//SHIELDS
	data.ClientBitmaps.Shield100 = LoadBitmap(hInst, MAKEINTRESOURCE(IDB_SHIELD100));		//100*50
	data.ClientBitmaps.Shield66 = LoadBitmap(hInst, MAKEINTRESOURCE(IDB_SHIELD66));
	data.ClientBitmaps.Shield33 = LoadBitmap(hInst, MAKEINTRESOURCE(IDB_SHIELD33));

	//POWERUPS
	data.ClientBitmaps.PowerupShield = LoadBitmap(hInst, MAKEINTRESOURCE(IDB_POWSHIELD));	// 40*40
	data.ClientBitmaps.PowerupIce = LoadBitmap(hInst, MAKEINTRESOURCE(IDB_POWICE));
	data.ClientBitmaps.PowerupBattery = LoadBitmap(hInst, MAKEINTRESOURCE(IDB_POWBATTERY));
	data.ClientBitmaps.PowerupVelocity = LoadBitmap(hInst, MAKEINTRESOURCE(IDB_POWVELOCITY));
	data.ClientBitmaps.PowerupLife = LoadBitmap(hInst, MAKEINTRESOURCE(IDB_POWLIFE));
	data.ClientBitmaps.PowerupAlcohol = LoadBitmap(hInst, MAKEINTRESOURCE(IDB_POWALCOHOL));


	//GET SCREEN SIZE
	screenWidth = GetSystemMetrics(SM_CXSCREEN);
	screenHeight = GetSystemMetrics(SM_CYSCREEN);

	_hdc = GetDC(mainWindow);

	data.ClientBitmaps.GlobalBitmap = CreateCompatibleBitmap(_hdc, screenWidth, screenHeight);
	data.memDC = CreateCompatibleDC(_hdc);
	SelectObject(data.memDC, data.ClientBitmaps.GlobalBitmap);

	DeleteDC(_hdc);

	return 0;
}


int renderLocalMap(ClientData data, Game game)
{
	HDC _hdc = GetDC(mainWindow);

	HDC auxRenderDC = CreateCompatibleDC(_hdc);

	ReleaseDC(mainWindow, _hdc);

	SelectObject(auxRenderDC, data.ClientBitmaps.GlobalBitmap);

	RECT rectangle = { 0, 0, screenWidth, screenHeight };
	FillRect(data.memDC, &rectangle, CreatePatternBrush(LoadBitmap(hInst, MAKEINTRESOURCE(IDB_CLIENTBKG))));

	for (unsigned int i = 0; i < game.nBasicInvaders; i++)
	{
		data.ClientBitmaps.GlobalBitmap = data.ClientBitmaps.Basic;

		SelectObject(auxRenderDC, data.ClientBitmaps.GlobalBitmap);

		/*BitBlt(data.memDC,
				game.basicInvaders[i].box.position.x, game.basicInvaders[i].box.position.y,
				50, 50, auxRenderDC, 0, 0, SRCCOPY);*/

		GdiTransparentBlt(data.memDC, 
			game.basicInvaders[i].box.position.x, 
			game.basicInvaders[i].box.position.y, 
			BASIC_INVADER_BITMAP_WIDTH, BASIC_INVADER_BITMAP_HEIGHT,
			auxRenderDC, 0, 0,
			BASIC_INVADER_BITMAP_WIDTH, BASIC_INVADER_BITMAP_HEIGHT,
			(RGB(255, 255, 255)));
	}

	for (unsigned int i = 0; i < game.nElusiveInvaders; i++)
	{
		data.ClientBitmaps.GlobalBitmap = data.ClientBitmaps.Elusive;

		SelectObject(auxRenderDC, data.ClientBitmaps.GlobalBitmap);

		GdiTransparentBlt(data.memDC, 
			game.elusiveInvaders[i].box.position.x, 
			game.elusiveInvaders[i].box.position.y, 
			ELUSIVE_INVADER_BITMAP_WIDTH, ELUSIVE_INVADER_BITMAP_HEIGHT,
			auxRenderDC, 0, 0, 
			ELUSIVE_INVADER_BITMAP_WIDTH, ELUSIVE_INVADER_BITMAP_HEIGHT,
			(RGB(255, 255, 255)));
	}

	if (1 == game.activePowerup)
	{
		switch (game.powerUp.type)
		{
		case SHIELD:
			data.ClientBitmaps.GlobalBitmap = data.ClientBitmaps.PowerupShield;
			break;
		case VELOCITY:
			data.ClientBitmaps.GlobalBitmap = data.ClientBitmaps.PowerupVelocity;
			break;
		case ICE:
			data.ClientBitmaps.GlobalBitmap = data.ClientBitmaps.PowerupIce;
			break;
		case BATTERY:
			data.ClientBitmaps.GlobalBitmap = data.ClientBitmaps.PowerupBattery;
			break;
		case ALCOHOL:
			data.ClientBitmaps.GlobalBitmap = data.ClientBitmaps.PowerupAlcohol;
			break;
		case LIFE:
			data.ClientBitmaps.GlobalBitmap = data.ClientBitmaps.PowerupLife;
			break;
		}

		SelectObject(auxRenderDC, data.ClientBitmaps.GlobalBitmap);

		GdiTransparentBlt(data.memDC, 
			game.powerUp.box.position.x, 
			game.powerUp.box.position.y, 
			POWERUP_BITMAP_WIDTH, POWERUP_BITMAP_HEIGHT,
			auxRenderDC, 0, 0, 
			POWERUP_BITMAP_WIDTH, POWERUP_BITMAP_HEIGHT,
			(RGB(255, 255, 255)));
	}

	for (unsigned int i = 0; i < game.nBombs; i++)
	{
		data.ClientBitmaps.GlobalBitmap = data.ClientBitmaps.Bomb;

		SelectObject(auxRenderDC, data.ClientBitmaps.GlobalBitmap);

		GdiTransparentBlt(data.memDC, 
			game.bombs[i].box.position.x, 
			game.bombs[i].box.position.y, 
			BOMB_BITMAP_WIDTH, BOMB_BITMAP_HEIGHT,
			auxRenderDC, 0, 0, 
			BOMB_BITMAP_WIDTH, BOMB_BITMAP_HEIGHT,
			(RGB(255, 255, 255)));
	}

	for (unsigned int i = 0; i < game.nShots; i++)
	{
		data.ClientBitmaps.GlobalBitmap = data.ClientBitmaps.Shot;

		SelectObject(auxRenderDC, data.ClientBitmaps.GlobalBitmap);

		GdiTransparentBlt(data.memDC, 
			game.shots[i].box.position.x, 
			game.shots[i].box.position.y, 
			SHOT_BITMAP_WIDTH, SHOT_BITMAP_HEIGHT,
			auxRenderDC, 0, 0, 
			SHOT_BITMAP_WIDTH, SHOT_BITMAP_HEIGHT,
			(RGB(255, 255, 255)));
	}

	for (unsigned int i = 0; i < game.nDefenders; i++)
	{
		data.ClientBitmaps.GlobalBitmap = data.ClientBitmaps.Defender;

		SelectObject(auxRenderDC, data.ClientBitmaps.GlobalBitmap);

		GdiTransparentBlt(data.memDC, 
			game.defenders[i].box.position.x, 
			game.defenders[i].box.position.y, 
			DEFENDER_BITMAP_WIDTH, DEFENDER_BITMAP_HEIGHT, 
			auxRenderDC, 0, 0, 
			DEFENDER_BITMAP_WIDTH, DEFENDER_BITMAP_HEIGHT, 
			(RGB(255, 255, 255)));

	}

	for (unsigned int i = 0; i < game.nShields; i++)
	{
		if (3 == game.shields[i].lives)
			data.ClientBitmaps.GlobalBitmap = data.ClientBitmaps.Shield100;
		else
			if (2 == game.shields[i].lives)
				data.ClientBitmaps.GlobalBitmap = data.ClientBitmaps.Shield66;
			else
				if (1 == game.shields[i].lives)
					data.ClientBitmaps.GlobalBitmap = data.ClientBitmaps.Shield33;
				else //just to be sure
					data.ClientBitmaps.GlobalBitmap = data.ClientBitmaps.Cleaner;

		SelectObject(auxRenderDC, data.ClientBitmaps.GlobalBitmap);

		GdiTransparentBlt(data.memDC, 
			game.shields[i].box.position.x, 
			game.shields[i].box.position.y, 
			SHIELD_BITMAP_WIDTH, SHIELD_BITMAP_HEIGHT,
			auxRenderDC, 0, 0, 
			SHIELD_BITMAP_WIDTH, SHIELD_BITMAP_HEIGHT,
			(RGB(255, 255, 255)));

	}

	InvalidateRect(mainWindow, NULL, FALSE);

	DeleteDC(auxRenderDC);

	return 0;
}

DWORD WINAPI threadRenderMap(LPVOID * in)
{
	//ClientData *data = (ClientData *)in;
	Game game;

	while (TRUE)
	{
		Sleep(RENDER_MAP_TIME);

		CopyMemory(&game, &gameShared, sizeof(Game));

		renderLocalMap(data, game);
	}

	return 0;
}

