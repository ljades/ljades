#include "gamecontrol.h"

using namespace std;

GameControl::GameControl(int player1Type, int player2Type, char *inputA, char *inputB, bool viz) {
	wPlayer = Player(WHITE, player1Type, SEMIOPTIMAL, inputA);
	bPlayer = Player(BLACK, player2Type, SEMIOPTIMAL, inputB);
	board = new GameBoard;
	pastBoards = NULL;
	isViz = viz;

	srand(time(NULL));
}

GameControl::~GameControl() {
	delete board;
	BoardList *tmp;
	while (pastBoards != NULL) {
		tmp = pastBoards;
		pastBoards = pastBoards->next;
		delete tmp;
	}
}

int GameControl::startGame() {
	int i;
	int whoseTurn;
	int playerRolls[MAX_NUM_ROLLS];
	int movesLeft;
	bool isMovePossible = false;
	FromToMove nextMove;
	wPlayer.resetRolls(playerRolls);

	// Roll to see whose turn is first.
	int roll1 = diceRoll();
	int roll2 = diceRoll();
	while (roll1 == roll2) {
		roll1 = diceRoll();
		roll2 = diceRoll();
	}
	if (isViz) {
		cout << "WHITE rolled " << roll1 << ", BLACK rolled " << roll2 << ".\n";
	}
	if (roll1 > roll2) {
		whoseTurn = WHITE;
		if (isViz) {
			cout << "WHITE ";
		}
	}
	else {
		whoseTurn = BLACK;
		if (isViz) {
			cout << "BLACK ";
		}
	}
	if (isViz) {
		cout << "goes first\n.";
	}
	// Main game loop
	while (!(board->isBlackVictory()) && !(board->isWhiteVictory())) {
		wPlayer.appendRoll(playerRolls, roll1);
		wPlayer.appendRoll(playerRolls, roll2);
		// Check for doubles
		if (roll1 == roll2) {
			wPlayer.appendRoll(playerRolls, roll1);
			wPlayer.appendRoll(playerRolls, roll1);
			movesLeft = 4;
		}
		else {
			movesLeft = 2;
		}

		while (movesLeft) {
			if (isViz) {
				board->printBoard();
				cout << "Rolls remaining: ";
			}
			isMovePossible = false;
			for (i = 0; i < MAX_NUM_ROLLS; i++) {
				if (playerRolls[i]) {
					if (isViz) {
						cout << playerRolls[i] << " ";
					}
					isMovePossible = isMovePossible || board->isThereAvailableMove(whoseTurn, playerRolls[i]);
				}
			}
			if (isViz) {
				cout << endl;
			}
			if (!isMovePossible) {
				if (isViz) {
					cout << "No moves left available.\n";
				}
				break;
			}
			if (whoseTurn == WHITE) {
				nextMove = wPlayer.getNextMove(movesLeft, playerRolls, board);
			}
			else {
				nextMove = bPlayer.getNextMove(movesLeft, playerRolls, board);
			}
			if (wPlayer.useRoll(playerRolls, abs(nextMove.from - nextMove.to))) {
				if (board->movePiece(whoseTurn, nextMove.from, nextMove.to)) {
					movesLeft--;
				}
				else {
					if (isViz) {
						cout << abs(nextMove.from - nextMove.to) << ", Invalid move.\n";
					}
					wPlayer.appendRoll(playerRolls, abs(nextMove.from - nextMove.to));
				}
			}
			else {
				if (isViz) {
					cout << "You tried " << abs(nextMove.from - nextMove.to) <<", Not one of your available rolls.\n";
				}
			}
		}
		appendPastBoard(board);
		// switch whose turn it is
		if (whoseTurn == WHITE) {
			whoseTurn = BLACK;
			if (isViz) {
				cout << "Player BLACK ";
			}
		}
		else {
			whoseTurn = WHITE;
			if (isViz) {
				cout << "Player WHITE ";
			}
		}
		wPlayer.resetRolls(playerRolls);
		movesLeft = 0;
		roll1 = diceRoll();
		roll2 = diceRoll();
		if (isViz) {
			cout << "rolled " << roll1 << " and " << roll2 << ".\n";
		}

	}

	wPlayer.backpropagate(pastBoards);
	if (wPlayer.getPtype() != bPlayer.getPtype()) {
		bPlayer.backpropagate(pastBoards);
	}

	if (board->isBlackVictory()) {
		if (isViz) {
			cout << "BLACK wins.\n";
		}
		return BLACK;
	}
	else {
		if (isViz) {
			cout << "WHITE wins.\n";
		}
		return WHITE;
	}
}

int GameControl::diceRoll() {
	return (rand() % 6) + 1;
}


void GameControl::appendPastBoard(GameBoard *pBoard) {
	BoardList *pBoardNode = new BoardList;
	pBoardNode->board = GameBoard(*pBoard);
	pBoardNode->value = UNDEF_VAL;
	pBoardNode->next = pastBoards;
	pastBoards = pBoardNode;
}