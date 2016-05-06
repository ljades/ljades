#pragma once

#ifndef GAMEVIZ_H
#define GAMEVIZ_H

#include "gamepoint.h"
#include <iostream>
#include <string>

class GameViz {
public:
	GameViz();
	~GameViz();
	void printBoard(GamePoint *points, int whiteBar, int blackBar, int whiteHome, int blackHome);


};


#endif