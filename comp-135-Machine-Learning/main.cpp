// Louis Ades
// main.cpp
// Purpose: Control backgammon gameplay and large-scale AI testing
// Note: It's in shambles right now as I accrued some technical debt creating test cases on the fly

#include "gamecontrol.h"
#include <fstream>

using namespace std;

int main() {
	ofstream myfile;
	myfile.open("gamestats.txt");/*
	for (int i = 0; i < 10; i++) {
		GameControl mControl(AI_TYPE_A, AI_TYPE_A, "featuresA10Self.txt", "featuresA10Self.txt", false);
		mControl.startGame();
	}
	cout << "A10 Completed\n";
	for (int i = 0; i < 10; i++) {
		GameControl mControl(AI_TYPE_B, AI_TYPE_B, "featuresB10Self.txt", "featuresB10Self.txt", false);
		mControl.startGame();
	}
	cout << "B10 Completed\n";


	for (int i = 0; i < 100; i++) {
		GameControl mControl(AI_TYPE_A, AI_TYPE_A, "featuresA100Self.txt", "featuresA100Self.txt", false);
		mControl.startGame();
	}
	cout << "A100 Completed\n";
	for (int i = 0; i < 100; i++) {
		GameControl mControl(AI_TYPE_B, AI_TYPE_B, "featuresB100Self.txt", "featuresB100Self.txt", false);
		mControl.startGame();
	}
	cout << "B100 Completed\n";


	for (int i = 0; i < 500; i++) {
		GameControl mControl(AI_TYPE_A, AI_TYPE_A, "featuresA500Self.txt", "featuresA500Self.txt", false);
		mControl.startGame();
	}
	cout << "A500 Completed\n";
	for (int i = 0; i < 500; i++) {
		GameControl mControl(AI_TYPE_B, AI_TYPE_B, "featuresB500Self.txt", "featuresB500Self.txt", false);
		mControl.startGame();
	}
	cout << "B500 Completed\n";
	
	int whiteTally = 0;
	int blackTally = 0;
	
	for (int i = 0; i < 200; i++) {
		GameControl mControl(AI_TYPE_A, AI_TYPE_B, "featuresAvAfter0.txt", "featuresBvAfter0.txt", false);
		if (mControl.startGame() == WHITE) {
			whiteTally++;
		}
		else {
			blackTally++;
		}
		if (i % 10 == 9) {
			myfile << "A: " << whiteTally << " B: " << blackTally << endl;
		}
	}
	myfile << "AB0 Completed\n";
	
	whiteTally = 0;
	blackTally = 0;
	for (int i = 0; i < 200; i++) {
		GameControl mControl(AI_TYPE_A, AI_TYPE_B, "featuresAvAfter10.txt", "featuresBvAfter10.txt", false);
		if (mControl.startGame() == WHITE) {
			whiteTally++;
		}
		else {
			blackTally++;
		}
		if (i % 10 == 9) {
			myfile << "A: " << whiteTally << " B: " << blackTally << endl;
		}
	}
	myfile << "AB10 Completed\n";

	whiteTally = 0;
	blackTally = 0;
	for (int i = 0; i < 200; i++) {
		GameControl mControl(AI_TYPE_A, AI_TYPE_B, "featuresAvAfter100.txt", "featuresBvAfter100.txt", false);
		if (mControl.startGame() == WHITE) {
			whiteTally++;
		}
		else {
			blackTally++;
		}
		if (i % 10 == 9) {
			myfile << "A: " << whiteTally << " B: " << blackTally << endl;
		}
	}
	myfile << "AB100 Completed\n";
	
	whiteTally = 0;
	blackTally = 0;
	for (int i = 0; i < 200; i++) {
		GameControl mControl(AI_TYPE_A, AI_TYPE_B, "featuresAvAfter500.txt", "featuresBvAfter500.txt", false);
		if (mControl.startGame() == WHITE) {
			whiteTally++;
		}
		else {
			blackTally++;
		}
		if (i % 10 == 9) {
			myfile << "A: " << whiteTally << " B: " << blackTally << endl;
		}
	}
	myfile << "AB500 Completed\n";
	/*
	whiteTally = 0;
	blackTally = 0;
	for (int i = 0; i < 200; i++) {
	GameControl mControl(AI_TYPE_A, AI_TYPE_B, "featuresAvAfter1000.txt", "featuresBvAfter1000.txt", false);
	if (mControl.startGame() == WHITE) {
	whiteTally++;
	}
	else {
	blackTally++;
	}
	if (i % 10 == 9) {
	myfile << "A: " << whiteTally << " B: " << blackTally << endl;
	}
	}
	myfile << "AB1000 Completed\n";*/
	
	
	for (int i = 0; i < 1000; i++) {
		GameControl mControl(AI_TYPE_A, AI_TYPE_A, "featuresA1000Self.txt", "featuresA1000Self.txt", false);
		mControl.startGame();
	}
	cout << "A1000 Completed\n";
	for (int i = 0; i < 1000; i++) {
		GameControl mControl(AI_TYPE_B, AI_TYPE_B, "featuresB1000Self.txt", "featuresB1000Self.txt", false);
		mControl.startGame();
	}
	cout << "B1000 Completed\n";
	myfile.close();
	getchar();
	return 0;
}