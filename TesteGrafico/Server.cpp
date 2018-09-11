#include "stdafx.h"
#include "Server.h"
#include <minwinbase.h>
#include "ServerBootstrap.h"


DWORD WINAPI serverMain(LPVOID in)
{
	ExtGame *sharedGame = (ExtGame *)in;
	setupGame(sharedGame);


	return 0;
}

void setupGame(ExtGame * gameShared)
{
	ExtGame game;


	game.nDefenders = MAX_DEFENDERS;
	game.nBasicInvaders = MAX_BASIC_INVADERS;
	game.nElusiveInvaders = MAX_ELUSIVE_INVADERS;
	game.nShields = MAX_SHIELDS;
	game.nShots = 0;
	game.nBombs = 0; //missing initial values


	for (unsigned int i = 0; i < game.nBasicInvaders; i++)
	{
		ExtBasicInvader nave;

		nave.box.dimension.width = BASIC_INVADER_BITMAP_WIDTH;
		nave.box.dimension.height = BASIC_INVADER_BITMAP_HEIGHT;
		nave.box.position.x = ((i + 1) * nave.box.dimension.width) + ((i + 1) * SHIP_SPACING);
		nave.box.position.y = LIM_TOP;
		nave.currentDirection = Right;
		nave.lastDirection = nave.currentDirection;
		nave.speed = BASIC_INVADERS_SPEED;
		nave.fireRate = BASIC_INVADERS_FIRERATE;
		nave.lives = BASIC_INVADERS_LIVES;

		game.basicInvaders[i] = nave;
	}

	for (unsigned int i = 0; i < game.nElusiveInvaders; i++)
	{
		ExtElusiveInvader nave;

		nave.box.dimension.width = ELUSIVE_INVADER_BITMAP_WIDTH;
		nave.box.dimension.height = ELUSIVE_INVADER_BITMAP_HEIGHT;
		nave.box.position.x = ((i + 1) * nave.box.dimension.width) + ((i + 1) * SHIP_SPACING);
		nave.box.position.y = LIM_TOP * nave.box.dimension.height;
		nave.currentDirection = (Direction) (rand() % NumberOfDirections);
		nave.lastDirection = nave.currentDirection;
		nave.speed = ELUSIVE_INVADERS_SPEED;
		nave.fireRate = ELUSIVE_INVADERS_FIRERATE;
		nave.lives = ELUSIVE_INVADERS_LIVES;

		game.elusiveInvaders[i] = nave;
	}

	//defender

	for (unsigned int i = 0; i < game.nDefenders; i++)
	{
		ExtDefender defender;

		defender.box.dimension.width = DEFENDER_BITMAP_WIDTH;
		defender.box.dimension.height = DEFENDER_BITMAP_HEIGHT;
		defender.box.position.x = ((i + 1) * (game.defenders[i].box.dimension.width)) + (i + 1) * SHIP_SPACING;
		defender.box.position.y = LIM_BOTTOM - 3 * game.defenders[i].box.dimension.height;
		defender.speed = DEFENDER_SPEED;
		defender.lives = DEFENDER_INITIAL_LIVES;
		defender.fireRate = DEFENDER_FIRERATE;
		// TODO defender.player = 

		game.defenders[i] = defender;
	}

	for (unsigned int i = 0; i < game.nShields; i++)
	{
		ExtShield shield;

		shield.box.dimension.width = SHIELD_BITMAP_WIDTH;
		shield.box.dimension.height = SHIELD_BITMAP_HEIGHT;
		shield.box.position.x = ((i + 1) * game.shields[i].box.dimension.width) + ((i + 1) * SHIELD_SPACING);;
		shield.box.position.y = 0.81 * LIM_BOTTOM;
		shield.lives = SHIELD_LIVES;

		game.shields[i] = shield;
	}

	CopyMemory(gameShared, &game, sizeof(ExtGame));

	ThreadGames threadGames;
	threadGames.gameShared = gameShared;
	threadGames.game = &game;

	HANDLE threadHandles[5];

	threadHandles[0] = CreateThread(NULL, 0, (LPTHREAD_START_ROUTINE)moveBasicShips, (LPVOID)&threadGames, 0, NULL);
	threadHandles[1] = CreateThread(NULL, 0, (LPTHREAD_START_ROUTINE)moveElusiveShips, (LPVOID)&threadGames, 0, NULL);

	threadHandles[2] = CreateThread(NULL, 0, (LPTHREAD_START_ROUTINE)moveBombs, (LPVOID)&threadGames, 0, NULL);
	threadHandles[3] = CreateThread(NULL, 0, (LPTHREAD_START_ROUTINE)moveShots, (LPVOID)&threadGames, 0, NULL);

	threadHandles[4] = CreateThread(NULL, 0, (LPTHREAD_START_ROUTINE)powerupThread, (LPVOID)&threadGames, 0, NULL);

	WaitForMultipleObjects(5, threadHandles, TRUE, INFINITE);

}

DWORD WINAPI moveBasicShips(LPVOID in)
{
	ThreadGames *threadGames = (ThreadGames *)in;
	ExtGame *game = threadGames->game;
	ExtGame *gameShared = threadGames->gameShared;

	while (1) {


		for (unsigned int i = 0; i < game->nBasicInvaders; i++)
		{
			switch (game->basicInvaders[i].currentDirection)
			{
			case Right:
				if (game->basicInvaders[i].box.position.x + game->basicInvaders[i].box.dimension.width + STEP_ATT_X < LIM_RIGHT)
				{
					game->basicInvaders[i].box.position.x += STEP_ATT_X;
					continue;
				}
				else
				{
					if (game->basicInvaders[i].box.position.y + (STEP_ATT_Y * game->basicInvaders[i].box.dimension.height) < LIM_BOTTOM)
					{
						game->basicInvaders[i].box.position.y += (STEP_ATT_Y * game->basicInvaders[i].box.dimension.height);
						game->basicInvaders[i].lastDirection = game->basicInvaders[i].currentDirection;
						game->basicInvaders[i].currentDirection = Left;
						continue;
					}
					else
					{
						game->basicInvaders[i].box.position.x = LIM_LEFT;
						game->basicInvaders[i].box.position.y = LIM_TOP;
						game->basicInvaders[i].lastDirection = game->basicInvaders[i].currentDirection;
						game->basicInvaders[i].currentDirection = Right;
						continue;
					}
				}
				break;

			case Left:
				if (game->basicInvaders[i].box.position.x - STEP_ATT_X > LIM_LEFT)
				{
					game->basicInvaders[i].box.position.x -= STEP_ATT_X;
					continue;
				}
				else
				{
					if (game->basicInvaders[i].box.position.y + (STEP_ATT_Y * game->basicInvaders[i].box.dimension.height) < LIM_BOTTOM)
					{
						game->basicInvaders[i].box.position.y += (STEP_ATT_Y * game->basicInvaders[i].box.dimension.height);
						game->basicInvaders[i].lastDirection = game->basicInvaders[i].currentDirection;
						game->basicInvaders[i].currentDirection = Right;
						continue;
					}
					else
					{
						game->basicInvaders[i].box.position.x = LIM_LEFT; //back to initial position
						game->basicInvaders[i].box.position.y = LIM_TOP;
						game->basicInvaders[i].lastDirection = game->basicInvaders[i].currentDirection;
						game->basicInvaders[i].currentDirection = Right;
						continue;
					}
				}
				break;
			}

		}
		CopyMemory(gameShared, game, sizeof(ExtGame));

		Sleep(BASIC_INVADERS_SPEED);
	}

}

DWORD WINAPI moveElusiveShips(LPVOID in)
{

	ThreadGames *threadGames = (ThreadGames *)in;
	ExtGame *game = threadGames->game;
	ExtGame *gameShared = threadGames->gameShared;

	while (1)
	{

		for (unsigned int i = 0; i < game->nElusiveInvaders; i++)
		{
			if (rand() % 100 < 82)
				game->elusiveInvaders[i].currentDirection = game->elusiveInvaders[i].lastDirection;
			else
			{
				int delta = (rand() % 3) - 1;
				game->elusiveInvaders[i].currentDirection = (Direction)abs(((game->elusiveInvaders[i].currentDirection + delta) % NumberOfDirections));
			}

			switch (game->elusiveInvaders[i].currentDirection)
			{
			case Up:
				if ((game->elusiveInvaders[i].box.position.y - STEP_ATT_Y) > LIM_TOP)
				{
					game->elusiveInvaders[i].box.position.y -= STEP_ATT_Y;
					game->elusiveInvaders[i].lastDirection = game->elusiveInvaders[i].currentDirection;
					//continue;
				}
				else
				{
					game->elusiveInvaders[i].box.position.y += STEP_ATT_Y;
					game->elusiveInvaders[i].lastDirection = game->elusiveInvaders[i].currentDirection;
					//continue;
				}
				break;
			case UpRight:
				if ((game->elusiveInvaders[i].box.position.y - STEP_ATT_Y > LIM_TOP) && (game->elusiveInvaders[i].box.position.x + STEP_ATT_X < LIM_RIGHT))
				{
					game->elusiveInvaders[i].box.position.y -= STEP_ATT_Y;
					game->elusiveInvaders[i].box.position.x += STEP_ATT_X;
					game->elusiveInvaders[i].lastDirection = game->elusiveInvaders[i].currentDirection;
					//continue;
				}
				else
				{
					game->elusiveInvaders[i].box.position.y += STEP_ATT_Y;
					game->elusiveInvaders[i].box.position.x -= STEP_ATT_X;
					game->elusiveInvaders[i].lastDirection = game->elusiveInvaders[i].currentDirection;
					//continue;
				}
				break;
			case Right:
				if (game->elusiveInvaders[i].box.position.x + STEP_ATT_X < LIM_RIGHT)
				{
					game->elusiveInvaders[i].box.position.x += STEP_ATT_X;
					game->elusiveInvaders[i].lastDirection = game->elusiveInvaders[i].currentDirection;
					//continue;
				}
				else
				{
					game->elusiveInvaders[i].box.position.x -= STEP_ATT_X;
					game->elusiveInvaders[i].lastDirection = game->elusiveInvaders[i].currentDirection;
					//continue;
				}
				break;
			case DownRight:
				if (game->elusiveInvaders[i].box.position.y + STEP_ATT_Y < LIM_BOTTOM && game->elusiveInvaders[i].box.position.x + STEP_ATT_X < LIM_RIGHT)
				{
					game->elusiveInvaders[i].box.position.y += STEP_ATT_Y;
					game->elusiveInvaders[i].box.position.x += STEP_ATT_X;
					game->elusiveInvaders[i].lastDirection = game->elusiveInvaders[i].currentDirection;
					//continue;
				}
				else
				{
					game->elusiveInvaders[i].box.position.y -= STEP_ATT_Y;
					game->elusiveInvaders[i].box.position.x -= STEP_ATT_X;
					game->elusiveInvaders[i].lastDirection = game->elusiveInvaders[i].currentDirection;
					//continue;
				}
				break;
			case Down:
				if (game->elusiveInvaders[i].box.position.y + STEP_ATT_Y < LIM_BOTTOM)
				{
					game->elusiveInvaders[i].box.position.y += STEP_ATT_Y;
					game->elusiveInvaders[i].lastDirection = game->elusiveInvaders[i].currentDirection;
					//continue;
				}
				else
				{
					game->elusiveInvaders[i].box.position.y -= STEP_ATT_Y;
					game->elusiveInvaders[i].lastDirection = game->elusiveInvaders[i].currentDirection;
					//continue;
				}
				break;
			case DownLeft:
				if (game->elusiveInvaders[i].box.position.y + STEP_ATT_Y < LIM_BOTTOM && game->elusiveInvaders[i].box.position.x - STEP_ATT_X > LIM_LEFT)
				{
					game->elusiveInvaders[i].box.position.y += STEP_ATT_Y;
					game->elusiveInvaders[i].box.position.x -= STEP_ATT_X;
					game->elusiveInvaders[i].lastDirection = game->elusiveInvaders[i].currentDirection;
					//continue;
				}
				else
				{
					game->elusiveInvaders[i].box.position.y -= STEP_ATT_Y;
					game->elusiveInvaders[i].box.position.x += STEP_ATT_X;
					game->elusiveInvaders[i].lastDirection = game->elusiveInvaders[i].currentDirection;
					//continue;
				}
				break;
			case Left:
				if (game->elusiveInvaders[i].box.position.x - STEP_ATT_X > LIM_LEFT)
				{
					game->elusiveInvaders[i].box.position.x -= STEP_ATT_X;
					game->elusiveInvaders[i].lastDirection = game->elusiveInvaders[i].currentDirection;
					//continue;
				}
				else
				{
					game->elusiveInvaders[i].box.position.x += STEP_ATT_X;
					game->elusiveInvaders[i].lastDirection = game->elusiveInvaders[i].currentDirection;
					//continue;
				}
				break;
			case UpLeft:
				if (game->elusiveInvaders[i].box.position.y - STEP_ATT_Y > LIM_TOP && game->elusiveInvaders[i].box.position.x - STEP_ATT_X > LIM_LEFT)
				{
					game->elusiveInvaders[i].box.position.y -= STEP_ATT_Y;
					game->elusiveInvaders[i].box.position.x -= STEP_ATT_X;
					game->elusiveInvaders[i].lastDirection = game->elusiveInvaders[i].currentDirection;
					//continue;
				}
				else
				{
					game->elusiveInvaders[i].box.position.y += STEP_ATT_Y;
					game->elusiveInvaders[i].box.position.x += STEP_ATT_X;
					game->elusiveInvaders[i].lastDirection = game->elusiveInvaders[i].currentDirection;
					//continue;
				}
				break;
			}
		}

		CopyMemory(gameShared, game, sizeof(ExtGame));

		Sleep(ELUSIVE_INVADERS_SPEED);
	}
}


DWORD WINAPI powerupThread(LPVOID in)
{

	ThreadGames *threadGames = (ThreadGames *)in;
	ExtGame *game = threadGames->game;
	ExtGame *gameShared = threadGames->gameShared;

	while (TRUE)
	{
		Sleep(POWERUP_REGEN_TIME);

		if (((rand() % 100) + 1) > 25)
		{
			game->activePowerup = TRUE;

			game->powerUp.box.position.x = WINDOW_WIDTH / 2;
			game->powerUp.box.position.y = LIM_TOP;

			//srand(time(NULL));
			int pType = (rand() % 75 + 1); //1 to 75

			if (pType >= 1 && pType < 20) //common
				game->powerUp.type = SHIELD;
			else if (pType >= 21 && pType <= 40)
				game->powerUp.type = VELOCITY;
			else if (pType >= 41 && pType <= 50) //uncommon
				game->powerUp.type = ICE;
			else if (pType >= 51 && pType <= 60)
				game->powerUp.type = BATTERY;
			else if (pType >= 61 && pType <= 70)
				game->powerUp.type = ALCOHOL;
			else //if (pType >= 71 && pType <= 75) //rare
				game->powerUp.type = LIFE;

			CopyMemory(gameShared, game, sizeof(ExtGame));

			while (game->powerUp.box.position.y < LIM_BOTTOM)
			{
				Sleep(POWERUP_SPEED);
				game->powerUp.box.position.y += STEP_ATT_Y;

				CopyMemory(gameShared, game, sizeof(ExtGame));
			}

			game->activePowerup = FALSE;
			CopyMemory(gameShared, game, sizeof(ExtGame));
		}
	}
}

DWORD WINAPI moveBombs(LPVOID in)
{
	//ExtGame *game = (ExtGame *)in;
	//
	//for (unsigned int i = 0; i < game->nBombs; i++)
	//{
	//	//GetObject(data.ClientBitmaps.Bomb, sizeof(shotB), &shotB);
	//	game->bombs[i].box.dimension.width = 15;
	//	game->bombs[i].box.dimension.height = 30;
	//	game->bombs[i].box.position.x = i * 50;
	//	game->bombs[i].box.position.y = LIM_TOP * game->bombs[i].box.dimension.height * 2;
	//	game->bombs[i].speed = 10;


	//}
	///*
	//for (int i = 0; i < game->nBombs; i++)
	//{
	//	while (shotPointer->box.position.y < LIM_BOTTOM)
	//	{
	//		Sleep(BOMB_SPEED);
	//		shotPointer->box.position.y += STEP_ATT_Y;
	//	}
	//}
	//*/
	return 0;
}

DWORD WINAPI moveShots(LPVOID in)
{
	ExtGame *game = (ExtGame *)in;

	/*while (1)
	{
		for (unsigned int i = 0; i < game->nShots; i++)
		{
			game->shots[i].box.dimension.width = 15;
			game->shots[i].box.dimension.height = 30;
			game->shots[i].box.position.x = i * 50;
			game->shots[i].box.position.y = LIM_TOP * game->shots[i].box.dimension.height * 2;
			game->shots[i].speed = 10;
		}

	}
*/

	return 0;
}