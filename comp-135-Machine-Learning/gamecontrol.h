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