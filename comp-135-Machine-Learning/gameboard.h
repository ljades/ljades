// Louis Ades
// gameboard.h
// Purpose: control state of a game board for backgammon. Includes
//			move validation and retrieving lists of all possible moves.

#pragma once
#ifndef GAMEBOARD_H
#define GAMEBOARD_H

#include "gamepoint.h"
#include "gameviz.h"

#define PAIR 2
#define MAX_ROLL 6

#define NUM_POINTS 24
#define BLACKBAR_POS -1
#define WHITEBAR_POS 24
#define WHITE_END 0
#define BLACK_END 23
#define CHECKERS_PER_TEAM 15

#define UNDEF_VAL -999999999.0

struct FromToMove {
	int from;
	int to;
};

struct ftMoveList {
	FromToMove move;
	ftMoveList *next;
};

class GameBoard {
public:
	GameBoard();
	GameBoard(const GameBoard &old);
	~GameBoard();
	bool isBlackVictory ();
	bool isWhiteVictory ();
	bool movePiece(int color, int fromPoint, int toPoint);
	int getNumMoves(int color, int singleRoll);
	ftMoveList *getAllMoves(int color, int singleRoll, ftMoveList *moves);
	bool isThereAvailableMove(int color, int singleRoll);
	bool isMovePossible(int color, int fromPoint, int toPoint);
	int getNumOnBar(int color);
	int getWhiteInHome();
	int getBlackInHome();
	GamePoint *getPoints();
	void printBoard();
private:
	GamePoint points[NUM_POINTS];
	int whiteOnBar;
	int blackOnBar;
	int whiteInHome;
	int blackInHome;
	GameViz visualizer;
	bool capturePiece(int color, int fromPoint, int toPoint);
	bool isHomeValid(int color); //TODO
};

struct BoardList {
	GameBoard board;
	double value;
	BoardList *next;
};

#endif