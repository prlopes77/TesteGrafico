#pragma once
#include "Common.h"


#define BOMB_SPEED				35
#define SHOT_SPEED				35
#define BASIC_INVADERS_SPEED	30
#define ELUSIVE_INVADERS_SPEED	(BASIC_INVADERS_SPEED * 1.1) //10% faster than basic
#define POWERUP_SPEED			20
#define DEFENDER_SPEED			20

#define BASIC_INVADERS_FIRERATE		1000
#define ELUSIVE_INVADERS_FIRERATE	(BASIC_INVADERS_FIRERATE * 0.6)
#define DEFENDER_FIRERATE			500

#define BASIC_INVADERS_LIVES	1
#define ELUSIVE_INVADERS_LIVES	3
#define DEFENDER_INITIAL_LIVES	3
#define SHIELD_LIVES			3

#define POWERUP_REGEN_TIME		10000
#define POWERUP_TIMEOUT			7000		

#define STEP_ATT_X				1
#define STEP_ATT_Y				1
#define STEP_DEF_X				5
#define STEP_DEF_Y				5

#define WINDOW_WIDTH			600
#define WINDOW_HEIGHT			800

#define LIM_LEFT				0
#define LIM_TOP					0

#define LIM_BOTTOM				WINDOW_HEIGHT
#define LIM_RIGHT				WINDOW_WIDTH

#define LIM_TOP_DEFENDERS		(0.8 * LIM_BOTTOM)

#define SHIP_SPACING			25		//pixels
#define SHIELD_SPACING			50


void setupGame(ExtGame * sharedGame);

DWORD WINAPI moveBasicShips(LPVOID in);
DWORD WINAPI moveElusiveShips(LPVOID in);

DWORD WINAPI moveBombs(LPVOID in);
DWORD WINAPI powerupThread(LPVOID in);
DWORD WINAPI moveShots(LPVOID in);

typedef struct ThreadGames{
	ExtGame * game;
	ExtGame * gameShared;
	
} ThreadGames;
