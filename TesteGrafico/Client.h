#pragma once

#include "resource.h"
#include "Common.h"

#define RENDER_MAP_TIME					40


#define MAX_LOADSTRING 100

// Global Variables:
HINSTANCE hInst;                                // current instance
WCHAR szTitle[MAX_LOADSTRING];                  // The title bar text
WCHAR szWindowClass[MAX_LOADSTRING];            // the main window class name


HWND mainWindow;

int screenWidth, screenHeight; //screen size

//Structures
typedef struct allBitmaps {
	HBITMAP GlobalBitmap;

	HBITMAP Background;
	HBITMAP Cleaner;

	HBITMAP Basic;
	HBITMAP Elusive;

	HBITMAP Defender;

	HBITMAP Bomb;
	HBITMAP Shot;

	HBITMAP Shield100;
	HBITMAP Shield66;
	HBITMAP Shield33;
	   
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



// Forward declarations of functions included in this code module:
ATOM                MyRegisterClass(HINSTANCE hInstance);
BOOL                InitInstance(HINSTANCE, int);
LRESULT CALLBACK    WndProc(HWND, UINT, WPARAM, LPARAM);
INT_PTR CALLBACK    About(HWND, UINT, WPARAM, LPARAM);

int renderLocalMap(ClientData data);
DWORD WINAPI threadRenderMap(LPVOID * param);
int LoadClientBitmaps(ClientData data);


typedef struct ThreadGames {
	Game * game;
	Game * gameShared;

} ThreadGames;