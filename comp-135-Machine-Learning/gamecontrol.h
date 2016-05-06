// Louis Ades
// gamecontrol.h
// Purpose: Serve as a central control for a game of backgammon.
//			Calls for players to make moves, changes the board accordingly,
//			terminates when the game is over.

#pragma once

#ifndef GAMECONTROL_H
#define GAMECONTROL_H

#include "gameboard.h"
#include "gameplayer.h"
#include <stdlib.h>
#include <time.h>


class GameControl {
public:
	GameControl(int player1Type, int player2Type, char *inputA, char *inputB, bool viz);
	~GameControl();
	int startGame();
	int diceRoll();
private:
	Player wPlayer;
	Player bPlayer;
	bool isViz;
	GameBoard *board;
	BoardList *pastBoards;
	void appendPastBoard(GameBoard *pBoard);
};


#endif