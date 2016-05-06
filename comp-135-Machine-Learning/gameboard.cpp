#include "gameboard.h"

#include <iostream>
using namespace std;

GameBoard::GameBoard() {
	whiteOnBar = 0;
	blackOnBar = 0;
	whiteInHome = 0;
	blackInHome = 0;
	points[23].initPoint(WHITE, 2);
	points[18].initPoint(BLACK, 5);
	points[16].initPoint(BLACK, 3);
	points[12].initPoint(WHITE, 5);
	points[11].initPoint(BLACK, 5);
	points[7].initPoint(WHITE, 3);
	points[5].initPoint(WHITE, 5);
	points[0].initPoint(BLACK, 2);
}

// Copy constructor
GameBoard::GameBoard(const GameBoard &old) {
	whiteOnBar = old.whiteOnBar;
	blackOnBar = old.blackOnBar;
	whiteInHome = old.whiteInHome;
	blackInHome = old.blackInHome;
	for (int i = 0; i < NUM_POINTS; i++) {
		points[i] = GamePoint(old.points[i]);
	}
}

GameBoard::~GameBoard() {

}

bool GameBoard::isBlackVictory() {
	return blackInHome == CHECKERS_PER_TEAM;
}

bool GameBoard::isWhiteVictory() {
	return whiteInHome == CHECKERS_PER_TEAM;
}

// Takes in an instruction to move a game piece. Return true if move was successful,
// false otherwise
bool GameBoard::movePiece(int color, int fromPoint, int toPoint) {
	if (isMovePossible(color, fromPoint, toPoint)) {
		if (!capturePiece(color, fromPoint, toPoint)) {
			// check if escape from bar
			if (color == WHITE && fromPoint == WHITEBAR_POS) {
				whiteOnBar--;
			} else if (color == BLACK && fromPoint == BLACKBAR_POS) {
				blackOnBar--;
			} else {
				points[fromPoint].popChecker();
			}

			// check if going home
			if (color == WHITE && toPoint <= BLACKBAR_POS) {
				whiteInHome++;
			} else if (color == BLACK && toPoint >= WHITEBAR_POS) {
				blackInHome++;
			} else {
				points[toPoint].addChecker(color);
			}
		}
		return true;
	}
	else {
		return false;
	}
}



bool GameBoard::isMovePossible(int color, int fromPoint, int toPoint) {
	if (isBlackVictory() || isWhiteVictory()) {
		return false;
	}
	int i;

	// out of range
	if (fromPoint < BLACKBAR_POS || fromPoint > WHITEBAR_POS || 
		toPoint <= BLACKBAR_POS - MAX_ROLL || toPoint >= WHITEBAR_POS + MAX_ROLL) {
		return false;
	}

	// wrong direction
	if (color == NEUTRAL ||
		(color == WHITE && fromPoint - toPoint <= 0) ||
		(color == BLACK && fromPoint - toPoint >= 0)) {
		return false;
	}

	// are your own pieces on the bar?
	if (color == WHITE) {
		if (whiteOnBar) {
			if (fromPoint != WHITEBAR_POS) {
				return false;
			}
		}
		else {
			if (fromPoint == WHITEBAR_POS) {
				return false;
			}
		}
	}
	else { //color == BLACK
		if (blackOnBar) {
			if (fromPoint != BLACKBAR_POS) {
				return false;
			}
		}
		else {
			if (fromPoint == BLACKBAR_POS) {
				return false;
			}
		}
	}
	// fromPoint not the right color, toPoint blocked
	if ((fromPoint >= WHITE_END && fromPoint <= BLACK_END) && color != points[fromPoint].getControl()) {
		return false;
	}
	if (toPoint > BLACKBAR_POS && toPoint < WHITEBAR_POS &&
		(color != points[toPoint].getControl() && points[toPoint].getNumCheckers() > 1)) {
		return false;
	}

	if (toPoint <= BLACKBAR_POS && !isHomeValid(WHITE)) {
		return false;
	} else if (toPoint >= WHITEBAR_POS && !isHomeValid(BLACK)) {
		return false;
	}

	// if none of these checks are violated, move is possible
	return true;
}



// Assume move possibility was checked already, given the context in which this function is
// used.
bool GameBoard::capturePiece(int color, int fromPoint, int toPoint) {
	if (toPoint <= BLACKBAR_POS || toPoint >= WHITEBAR_POS) {
		return false;
	}
	if (color != points[toPoint].getControl() && points[toPoint].getNumCheckers() == 1) {
		if (color == WHITE) {
			if (fromPoint == WHITEBAR_POS) {
				whiteOnBar--;
			} else {
				points[fromPoint].popChecker();
			}
			points[toPoint].popChecker();
			blackOnBar++;
			points[toPoint].addChecker(color);
		} else if (color == BLACK) {
			if (fromPoint == BLACKBAR_POS) {
				blackOnBar--;
			} else {
				points[fromPoint].popChecker();
			}
			points[toPoint].popChecker();
			whiteOnBar++;
			points[toPoint].addChecker(color);
		}
		return true;
	}
	return false;
}




//TODO: isHomeValid
bool GameBoard::isHomeValid (int color) {
	int i;
	for (i = WHITE_END + MAX_ROLL; i <= BLACK_END - MAX_ROLL; i++) {
		if (points[i].getControl() == color) {
			return false;
		}
	}
	if (color == WHITE) {
		if (whiteOnBar) {
			return false;
		}
		for (i = WHITEBAR_POS - MAX_ROLL; i < WHITEBAR_POS; i++) {
			if (points[i].getControl() == color) {
				return false;
			}
		}
	}
	if (color == BLACK) {
		if (blackOnBar) {
			return false;
		}
		for (i = WHITE_END; i < WHITE_END + MAX_ROLL; i++) {
			if (points[i].getControl() == color) {
				return false;
			}
		}
	}
	return true;
}


// Deprecated function, not needed
int GameBoard::getNumMoves(int color, int singleRoll) {
	int i;
	int sumOfMoves = 0;
	if (color == WHITE) {
		if (whiteOnBar) {
			if (isMovePossible(color, WHITEBAR_POS, WHITEBAR_POS - singleRoll)) {
				return 1;
			}
			return 0;
		}
		for (i = BLACK_END; i > BLACKBAR_POS; i--) {
			if (isMovePossible(color, i, i - singleRoll)) {
				sumOfMoves++;
			}
		}
	} else if (color == BLACK) {
		if (blackOnBar) {
			if (isMovePossible(color, BLACKBAR_POS, BLACKBAR_POS + singleRoll)) {
				return 1;
			}
			return 0;
		}
		for (i = WHITE_END; i < WHITEBAR_POS; i++) {
			if (isMovePossible(color, i, i + singleRoll)) {
				sumOfMoves++;
			}
		}
	}

	return sumOfMoves;
}

// Returns a list of possible moves given whose turn it is and a dice roll.
ftMoveList *GameBoard::getAllMoves(int color, int singleRoll, ftMoveList *moves) {
	int i;
	ftMoveList *tmpMove;
	if (color == WHITE) {
		if (whiteOnBar) {
			if (isMovePossible(color, WHITEBAR_POS, WHITEBAR_POS - singleRoll)) {
				tmpMove = new ftMoveList;
				tmpMove->move.from = WHITEBAR_POS;
				tmpMove->move.to = WHITEBAR_POS - singleRoll;
				tmpMove->next = moves;
				moves = tmpMove;
			}
		}
		else {
			for (i = BLACK_END; i > BLACKBAR_POS; i--) {
				if (isMovePossible(color, i, i - singleRoll)) {
					tmpMove = new ftMoveList;
					tmpMove->move.from = i;
					tmpMove->move.to = i - singleRoll;
					tmpMove->next = moves;
					moves = tmpMove;
				}
			}
		}
	}
	else if (color == BLACK) {
		if (blackOnBar) {
			if (isMovePossible(color, BLACKBAR_POS, BLACKBAR_POS + singleRoll)) {
				tmpMove = new ftMoveList;
				tmpMove->move.from = BLACKBAR_POS;
				tmpMove->move.to = BLACKBAR_POS + singleRoll;
				tmpMove->next = moves;
				moves = tmpMove;
			}
		}
		else {
			for (i = WHITE_END; i < WHITEBAR_POS; i++) {
				if (isMovePossible(color, i, i + singleRoll)) {
					tmpMove = new ftMoveList;
					tmpMove->move.from = i;
					tmpMove->move.to = i + singleRoll;
					tmpMove->next = moves;
					moves = tmpMove;
				}
			}
		}
	}

	return moves;
}

// Returns true if there is any avaiable move to make with the given dice roll,
// false otherwise
bool GameBoard::isThereAvailableMove(int color, int singleRoll) {
	int i;
	if (color == WHITE) {
		if (whiteOnBar) {
			if (isMovePossible(color, WHITEBAR_POS, WHITEBAR_POS - singleRoll)) {
				return true;
			}
			return false;
		}
		for (i = BLACK_END; i > BLACKBAR_POS; i--) {
			if (isMovePossible(color, i, i - singleRoll)) {
				return true;
			}
		}
	}
	else if (color == BLACK) {
		if (blackOnBar) {
			if (isMovePossible(color, BLACKBAR_POS, BLACKBAR_POS + singleRoll)) {
				return true;
			}
			return false;
		}
		for (i = WHITE_END; i < WHITEBAR_POS; i++) {
			if (isMovePossible(color, i, i + singleRoll)) {
				return true;
			}
		}
	}

	return false;
}

int GameBoard::getNumOnBar(int color) {
	if (color == WHITE) {
		return whiteOnBar;
	} else if (color == BLACK) {
		return blackOnBar;
	}
	else {
		return 0;
	}
}

int GameBoard::getWhiteInHome() {
	return whiteInHome;
}

int GameBoard::getBlackInHome() {
	return blackInHome;
}

GamePoint *GameBoard::getPoints() {
	return points;
}

void GameBoard::printBoard() {
	visualizer.printBoard(points, whiteOnBar, blackOnBar, whiteInHome, blackInHome);
}
