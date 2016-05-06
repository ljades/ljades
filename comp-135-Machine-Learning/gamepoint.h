// Louis Ades
// gamepoint.h
// Purpose: Control behavior of a single point on a backgammon board

#pragma once

#ifndef GAMEPOINT_H
#define GAMEPOINT_H

#define MAX_PER_POINT 15
#define WHITE -1
#define NEUTRAL 0
#define BLACK 1

class GamePoint {
public:
	GamePoint();
	GamePoint(const GamePoint &old);
	~GamePoint();
	int getNumCheckers ();
	int getControl ();
	bool addChecker (int color);
	bool popChecker ();
	void initPoint(int color, int ncheckers);
private:
	int numCheckers;
	int control;
};



#endif