#include "gamepoint.h"

GamePoint::GamePoint() {
	numCheckers = 0;
	control = NEUTRAL;
}

GamePoint::GamePoint(const GamePoint &old) {
	numCheckers = old.numCheckers;
	control = old.control;
}

GamePoint::~GamePoint() {

}

int GamePoint::getNumCheckers() {
	return numCheckers;
}

int GamePoint::getControl() {
	return control;
}

// Note: This function does not handle capturing checkers. That is handed from the gameboard.
// To capture a checker, you must first pop it, then push the new one.
bool GamePoint::addChecker(int color) {
	if (color == NEUTRAL) {
		return false;
	}
	if (control == NEUTRAL) {
		numCheckers = 1;
		control = color;
	} else {
		if (control != color) {
			return false;
		}
		numCheckers++;
	}
	return true;
}

void GamePoint::initPoint(int color, int ncheckers) {
	control = color;
	numCheckers = ncheckers;
}

bool GamePoint::popChecker() {
	if (numCheckers <= 0) {
		return false;
	}
	numCheckers--;
	if (numCheckers == 0) {
		control = NEUTRAL;
	}
	return true;
}