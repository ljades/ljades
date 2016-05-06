#pragma once

#ifndef GAMEPLAYER_H
#define GAMEPLAYER_h

#include "gameboard.h"
#include <iostream>
#include <stdio.h>

#define L_RATE 0.0005
#define LAMBDA 0.7

#define HUMAN 0
#define AI_TYPE_A 1
#define AI_TYPE_B 2
#define MAX_NUM_ROLLS 4

#define OPTIMAL 0
#define EXPERIMENTAL 1
#define SEMIOPTIMAL 2

#define NUM_FEATS_A 28
#define NUM_FEATS_A2 52 //added feature for each column denoting if there's only one
							//checker there
#define NUM_FEATS_B 32 //added two features for each side: blockade strength and
							//danger of getting hit
#define NUM_FEATS_B2 56

#define WBAR 24
#define BBAR 25
#define WHOME 26
#define BHOME 27
#define WBLOCK 28
#define WHIT 29
#define BBLOCK 30
#define BHIT 31

#define WHITE_WIN -1000.0
#define BLACK_WIN 1000.0

struct movesetList {
	FromToMove moveset[MAX_NUM_ROLLS];
	double value;
	movesetList *next;
};

class Player {
public:
	Player();
	Player(int color, int type, int policy, char *fWeights);
	~Player();
	FromToMove getNextMove(int movesLeft, int rolls[MAX_NUM_ROLLS], GameBoard *board);
	void resetRolls(int rolls[MAX_NUM_ROLLS]);
	bool appendRoll(int rolls[MAX_NUM_ROLLS], int roll);
	bool useRoll(int rolls[MAX_NUM_ROLLS], int roll);
	int getPtype();
	void backpropagate(BoardList * pastBoards);
private:
	int pcolor;
	int ptype;
	int ppolicy;
	int numFeats;
	double *weights;
	char *weightsFileName;
	FromToMove chosenMoves[MAX_NUM_ROLLS];
	int numChosen;
	void resetMoves();
	bool appendMove(FromToMove move);
	FromToMove useMove();

	double getBoardVal(GameBoard *board);
	double getBoardVal(GameBoard *board, double *inputs);

	void selectMoves(int movesLeft, int rolls[MAX_NUM_ROLLS], GameBoard *board);
	movesetList *simMovesRecurse(int maxMovesLeft, int rolls[MAX_NUM_ROLLS], GameBoard *board, 
		movesetList *allFullMoves, FromToMove currMoveset[MAX_NUM_ROLLS], int currMovesLeft, int lastRoll);

	double getBasicBoardVal(GameBoard *board);
	double getBasicBoardVal(GameBoard *board, double *inputs);
	double getAdvBoardVal(GameBoard *board);
	double getAdvBoardVal(GameBoard *board, double *inputs);
};


#endif