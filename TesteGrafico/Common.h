#pragma once

#include <Windows.h>
#include <time.h>

#define MAX_BASIC_INVADERS			5
#define MAX_ELUSIVE_INVADERS		2
#define MAX_PROJECTILES				10
#define MAX_DEFENDERS				4
#define MAX_SHIELDS					5
#define MAX_POWERUPS				1
#define MAX_SHOTS					500
#define MAX_BOMBS					500


#define DEFENDER_WIDTH				100
#define DEFENDER_HEIGHT				50
#define BASIC_INVADER_WIDTH			50
#define BASIC_INVADER_HEIGHT		50
#define ELUSIVE_INVADER_WIDTH		50
#define ELUSIVE_INVADER_HEIGHT		50
#define SHOT_WIDTH					15
#define SHOT_HEIGHT					30
#define BOMB_WIDTH					15
#define BOMB_HEIGHT					30
#define POWERUP_WIDTH				40
#define POWERUP_HEIGHT				40
#define SHIELD_WIDTH				100
#define SHIELD_HEIGHT				50



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

typedef struct BasicInvader {
	Box box;
	Direction currentDirection;
	Direction lastDirection;
	Speed_T speed;
	Speed_T fireRate;
	Health_T lives;
} BasicInvader;

typedef struct ElusiveInvader {
	Box box;
	Direction currentDirection;
	Direction lastDirection;
	Speed_T speed;
	Speed_T fireRate;
	Health_T lives;
} ElusiveInvader;

typedef struct Projectile {
	Box box;
	Speed_T speed;
} Projectile;

typedef struct Defender {
	Box box;
	PlayerId_T player;
	Health_T lives;
	Speed_T speed;
	Speed_T fireRate;
} Defender;

typedef struct Shield {
	Box box;
	Health_T lives;
} Shield;

typedef struct PowerUp {
	Box box;
	unsigned int type;
	Speed_T speed;
} PowerUp;

typedef struct Game {
	Dimension gameMapDimension;

	BasicInvader basicInvaders[MAX_BASIC_INVADERS];
	Count_T nBasicInvaders;
	ElusiveInvader elusiveInvaders[MAX_ELUSIVE_INVADERS];
	Count_T nElusiveInvaders;
	Defender defenders[MAX_DEFENDERS];
	Count_T nDefenders;
	Shield shields[MAX_SHIELDS];
	Count_T nShields;
	PowerUp powerUp;
	Projectile shots[MAX_SHOTS];
	Count_T nShots;
	Projectile bombs[MAX_BOMBS];
	Count_T nBombs;

	BOOL activePowerup;
} Game;