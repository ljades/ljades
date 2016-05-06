#include "gameviz.h"

using namespace std;

GameViz::GameViz() {

}

GameViz::~GameViz() {

}

string ptos(GamePoint point) {
	if (point.getControl() == WHITE) {
		return to_string(point.getNumCheckers()) + "w";
	}
	else if (point.getControl() == BLACK) {
		return to_string(point.getNumCheckers()) + "b";
	}
	return "  ";
}

//TODO finish visualization of game board
//TODO fix for more than 10 checkers on a point
void GameViz::printBoard(GamePoint *points, int whiteBar, int blackBar, int whiteHome, int blackHome) {
	cout << "____________________________________________________\n";
	cout << "|                       |   |                        |\n";
	cout << "|12  13  14  15  16  17 |   | 18  19  20  21  22  23 |\n";
	cout << "|                       |   |                        |\n";
	cout << "|" << ptos(points[12]) << "  " << ptos(points[13])
		<< "  " << ptos(points[14]) << "  " << ptos(points[15])
		<< "  " << ptos(points[16]) << "  " << ptos(points[17])
		<< " |   | " << ptos(points[18]) << "  " << ptos(points[19])
		<< "  " << ptos(points[20]) << "  " << ptos(points[21])
		<< "  " << ptos(points[22]) << "  " << ptos(points[23]) << " |\n";
	cout << "|                       | " << to_string(whiteBar) << "w|                        |\n";
	cout << "|                       |   |                        |\n";
	cout << "|                       |BAR|                        |\n";
	cout << "|                       |   |                        |\n";
	cout << "|                       | " << to_string(blackBar) << "b|                        |\n";
	cout << "|" << ptos(points[11]) << "  " << ptos(points[10])
		<< "  " << ptos(points[9]) << "  " << ptos(points[8])
		<< "  " << ptos(points[7]) << "  " << ptos(points[6])
		<< " |   | " <<  ptos(points[5]) << "  " << ptos(points[4])
		<< "  " << ptos(points[3]) << "  " << ptos(points[2])
		<< "  " << ptos(points[1]) << "  " << ptos(points[0]) << " |\n";
	cout << "|                       |   |                        |\n";
	cout << "|11  10   9   8   7   6 |   |  5   4   3   2   1   0 |\n";
	cout << "|_______________________|___|________________________|\n";
	return;
}


