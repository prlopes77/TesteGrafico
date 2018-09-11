#pragma once

#include <Windows.h>
#include <time.h>

#define MAX_BASIC_INVADERS				5
#define MAX_ELUSIVE_INVADERS			2
#define MAX_PROJECTILES					10
#define MAX_DEFENDERS					4
#define MAX_SHIELDS						5
#define MAX_POWERUPS					1
#define MAX_SHOTS						500
#define MAX_BOMBS						500


#define DEFENDER_BITMAP_WIDTH			100
#define DEFENDER_BITMAP_HEIGHT			50
#define BASIC_INVADER_BITMAP_WIDTH		50
#define BASIC_INVADER_BITMAP_HEIGHT		50
#define ELUSIVE_INVADER_BITMAP_WIDTH	50
#define ELUSIVE_INVADER_BITMAP_HEIGHT	50
#define SHOT_BITMAP_WIDTH				15
#define SHOT_BITMAP_HEIGHT				30
#define BOMB_BITMAP_WIDTH				15
#define BOMB_BITMAP_HEIGHT				30
#define POWERUP_BITMAP_WIDTH			40
#define POWERUP_BITMAP_HEIGHT			40
#define SHIELD_BITMAP_WIDTH				100
#define SHIELD_BITMAP_HEIGHT			50

#define RENDER_MAP_TIME					40



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
typedef unsigned int Speed_T;

typedef struct Dimension {
	unsigned int width;
	unsigned int height;
} Dimension;

typedef struct Box {
	Position position;
	Dimension dimension;
} Box;

typedef struct ExtBasicInvader {
	Box box;
	Direction currentDirection;
	Direction lastDirection;
	Speed_T speed;
	Speed_T fireRate;
	Health_T lives;
} ExtBasicInvader;

typedef struct ExtElusiveInvader {
	Box box;
	Direction currentDirection;
	Direction lastDirection;
	Speed_T speed;
	Speed_T fireRate;
	Health_T lives;
} ExtElusiveInvader;

typedef struct ExtProjectile {
	Box box;
	Speed_T speed;
} ExtProjectile;

typedef struct ExtDefender {
	Box box;
	PlayerId_T player;
	Health_T lives;
	Speed_T speed;
	Speed_T fireRate;
} ExtDefender;

typedef struct ExtShield {
	Box box;
	Health_T lives;
} ExtShield;

typedef struct ExtPowerUp {
	Box box;
	unsigned int type;
	Speed_T speed;
} ExtPowerUp;

typedef struct ExtGame {
	Dimension gameMapDimension;

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

	BOOL activePowerup;
} ExtGame;