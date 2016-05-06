#include "gameplayer.h"

using namespace std;

Player::Player() {
	pcolor = WHITE;
	ptype = HUMAN;
}

Player::Player(int color, int type, int policy, char *fWeights) {
	FILE * weightsFile;
	double temp = 0.0;
	pcolor = color;
	ptype = type;
	ppolicy = policy;
	weightsFileName = fWeights;
	numChosen = 0;
	resetMoves();

	if (ptype == AI_TYPE_A) {
		numFeats = NUM_FEATS_A2;
		weights = new double[NUM_FEATS_A2];
	}
	else if (ptype == AI_TYPE_B) {
		numFeats = NUM_FEATS_B2;
		weights = new double[NUM_FEATS_B2];
	}
	else {
		numFeats = 0;
		weights = NULL;
	}

	for (int i = 0; i < numFeats; i++) {
		weights[i] = 0.001;
	}
	errno_t err = fopen_s(&weightsFile, weightsFileName, "r");
	if (err == 0) {
		for (int i = 0; i < numFeats; i++) {
			if (fscanf_s(weightsFile, "%lf", &temp) != EOF) {
				weights[i] = temp;
			}
			else {
				break;
			}
		}
		fclose(weightsFile);
	}
}

Player::~Player() {
	//delete [] weights;
}

FromToMove Player::getNextMove(int movesLeft, int rolls[MAX_NUM_ROLLS], GameBoard *board) {
	FromToMove nextMove;
	int rollsCopy[MAX_NUM_ROLLS];
	if (ptype == HUMAN) {
		cout << "Player ";
		if (pcolor == WHITE) {
			cout << "WHITE";
		}
		else {
			cout << "BLACK";
		}
		cout << ", please enter next move as two numbers, the point where\n you're moving from"
			"and the point you're moving towards.\n For WHITE home, enter -1, and for BLACK home,"
			"enter 24:\n";
		cout << ">>";
		cin >> nextMove.from >> nextMove.to;
	}
	else if (ptype > HUMAN) {
		if (numChosen) {
			return useMove();
		}
		else {
			for (int i = 0; i < MAX_NUM_ROLLS; i++) {
				rollsCopy[i] = rolls[i];
			}
			selectMoves(movesLeft, rollsCopy, board);

			return useMove();
		}
	}
	else {
		cout << "Should only be valid player types for now.";
		exit(1);
	}
	return nextMove;
}

void Player::selectMoves(int movesLeft, int rolls[MAX_NUM_ROLLS], GameBoard *board) {
	ftMoveList *startingMoves = NULL;
	ftMoveList *tmpMoveList = NULL;
	movesetList *allFullMoves = NULL;
	movesetList *tmpMovesetList = NULL;
	movesetList *prevMovesetList = NULL;
	FromToMove currMoveset[MAX_NUM_ROLLS];
	bool needOptimal;


	int i, numMoves, policyIndex;
	double sumVals, meanVals, maxVal, policyValIndex, currSum;
	FromToMove maxMoveset[MAX_NUM_ROLLS];
	for (i = 0; i < MAX_NUM_ROLLS; i++) {
		currMoveset[i].from = 0;
		currMoveset[i].to = 0;
		maxMoveset[i].from = 0;
		maxMoveset[i].to = 0;
	}

	allFullMoves = simMovesRecurse(movesLeft, rolls, board, allFullMoves, currMoveset, movesLeft, 0);
	

	// Possibly second half-ply feature selection
	
	// Stats step
	tmpMovesetList = allFullMoves;
	sumVals = 0;
	numMoves = 0;
	maxVal = WHITE_WIN;
	while (tmpMovesetList != NULL) {
		sumVals += tmpMovesetList->value;
		numMoves++;
		if (tmpMovesetList->value > maxVal) {
			maxVal = tmpMovesetList->value;
			for (i = 0; i < MAX_NUM_ROLLS; i++) {
				maxMoveset[i].from = tmpMovesetList->moveset[i].from;
				maxMoveset[i].to = tmpMovesetList->moveset[i].to;
			}
		}
		tmpMovesetList = tmpMovesetList->next;
	}
	meanVals = sumVals / (double)numMoves;
	
	//Filter step
	if (ppolicy != OPTIMAL) {
		tmpMovesetList = allFullMoves;
		while (tmpMovesetList != NULL) {
			if (tmpMovesetList->value < meanVals && tmpMovesetList->next != NULL) {
				sumVals -= tmpMovesetList->value;
				numMoves--;
				if (tmpMovesetList == allFullMoves) {
					allFullMoves = allFullMoves->next;
					delete tmpMovesetList;
					tmpMovesetList = allFullMoves;
				}
				else {
					tmpMovesetList = tmpMovesetList->next;
					delete prevMovesetList->next;
					prevMovesetList->next = tmpMovesetList;
				}
			}
			else {
				prevMovesetList = tmpMovesetList;
				tmpMovesetList = tmpMovesetList->next;
			}
		}
	}

	// Select moves based on policy
	if (ppolicy == OPTIMAL) {
		resetMoves();
		for (i = 0; i < MAX_NUM_ROLLS; i++) {
			appendMove(maxMoveset[i]);
		}
	}
	else if (ppolicy == EXPERIMENTAL) {
		policyIndex = rand() % numMoves;
		tmpMovesetList = allFullMoves;
		while (tmpMovesetList != NULL) {
			if (!policyIndex) {
				break;
			}
			policyIndex--;
			tmpMovesetList = tmpMovesetList->next;
		}
		resetMoves();
		for (i = 0; i < MAX_NUM_ROLLS; i++) {
			appendMove(tmpMovesetList->moveset[i]);
		}
	}
	else if (ppolicy == SEMIOPTIMAL) {
		policyValIndex = (((double)rand()) / ((double)RAND_MAX));
		tmpMovesetList = allFullMoves;
		currSum = 0.0;
		needOptimal = false;
		while (tmpMovesetList != NULL) {
			if (sumVals != 0) {
				currSum += tmpMovesetList->value / ((double)sumVals);
			}
			if (policyValIndex < currSum) {
				break;
			}
			if (tmpMovesetList->next == NULL) {
				needOptimal = true;
				break;
			}
			tmpMovesetList = tmpMovesetList->next;
		}
		resetMoves();
		if (needOptimal) {
			for (i = 0; i < MAX_NUM_ROLLS; i++) {
				appendMove(maxMoveset[i]);
			}
		}
		else {
			for (i = 0; i < MAX_NUM_ROLLS; i++) {
				appendMove(tmpMovesetList->moveset[i]);
			}
		}
	}

	// delete movesetlist
	while (allFullMoves != NULL) {
		tmpMovesetList = allFullMoves;
		allFullMoves = allFullMoves->next;
		delete tmpMovesetList;
	}
	
}

movesetList *Player::simMovesRecurse(int maxMovesLeft, int rolls[MAX_NUM_ROLLS], GameBoard *board, 
	movesetList *allFullMoves, FromToMove currMoveset[MAX_NUM_ROLLS], int currMovesLeft, int lastRoll) {
	ftMoveList *startingMoves = NULL;
	ftMoveList *tmpMoveList = NULL;
	ftMoveList *prevMoveList = NULL;
	movesetList *tmpMovesetList = NULL;
	GameBoard boardCopy;
	bool isMovePossible;
	int i;

	isMovePossible = false;
	for (i = 0; i < MAX_NUM_ROLLS; i++) {
		if (rolls[i]) {
			isMovePossible = isMovePossible || board->isThereAvailableMove(pcolor, rolls[i]);
		}
	}
	if (!currMovesLeft || !isMovePossible) {
		tmpMovesetList = new movesetList;
		for (i = 0; i < MAX_NUM_ROLLS; i++) {
			tmpMovesetList->moveset[i].from = currMoveset[i].from;
			tmpMovesetList->moveset[i].to = currMoveset[i].to;
		}
		tmpMovesetList->value = getBoardVal(board) * (double)pcolor;
		tmpMovesetList->next = allFullMoves;
		allFullMoves = tmpMovesetList;
		return allFullMoves;
	}
	else {
		for (i = 0; i < MAX_NUM_ROLLS; i++) {
			startingMoves = board->getAllMoves(pcolor, rolls[i], startingMoves);
		}
		tmpMoveList = startingMoves;
		while (tmpMoveList != NULL) {
			boardCopy = GameBoard(*board);
			lastRoll = abs(tmpMoveList->move.from - tmpMoveList->move.to);
			if (useRoll(rolls, lastRoll)
				&& boardCopy.movePiece(pcolor, tmpMoveList->move.from, tmpMoveList->move.to)) {
				currMoveset[maxMovesLeft - currMovesLeft].from = tmpMoveList->move.from;
				currMoveset[maxMovesLeft - currMovesLeft].to = tmpMoveList->move.to;
				allFullMoves = simMovesRecurse(maxMovesLeft, rolls, &boardCopy, allFullMoves, 
					currMoveset, currMovesLeft - 1, lastRoll);
				appendRoll(rolls, lastRoll);
				currMoveset[maxMovesLeft - currMovesLeft].from = 0;
				currMoveset[maxMovesLeft - currMovesLeft].to = 0;
			}
			else {
				cout << "Bad move? Something went wrong.";
				cin >> i;
				exit(1);
				return NULL;
			}
			prevMoveList = tmpMoveList;
			tmpMoveList = tmpMoveList->next;
			delete prevMoveList;
		}
		return allFullMoves;
	}
}


void Player::resetRolls(int rolls[MAX_NUM_ROLLS]) {
	int i;
	for (i = 0; i < MAX_NUM_ROLLS; i++) {
		rolls[i] = 0;
	}
	return;
}

bool Player::appendRoll(int rolls[MAX_NUM_ROLLS], int roll) {
	int i;
	for (i = 0; i < MAX_NUM_ROLLS; i++) {
		if (rolls[i] == 0) {
			rolls[i] = roll;
			return true;
		}
	}
	return false;
}

bool Player::useRoll(int rolls[MAX_NUM_ROLLS], int roll) {
	int i;
	for (i = 0; i < MAX_NUM_ROLLS; i++) {
		if (rolls[i] == roll) {
			rolls[i] = 0;
			return true;
		}
	}
	return false;
}



void Player::resetMoves() {
	int i;
	for (i = 0; i < MAX_NUM_ROLLS; i++) {
		chosenMoves[i].from = 0;
		chosenMoves[i].to = 0;
	}
	numChosen = 0;
	return;
}

bool Player::appendMove(FromToMove move) {
	int i;
	if (move.from == 0 && move.to == 0) {
		return false;
	}
	for (i = 0; i < MAX_NUM_ROLLS; i++) {
		if (chosenMoves[i].from == 0 && chosenMoves[i].to == 0) {
			chosenMoves[i] = move;
			numChosen++;
			return true;
		}
	}
	return false;
}

FromToMove Player::useMove() {
	int i;
	FromToMove temp;
	temp.from = 0;
	temp.to = 0;
	if (numChosen == 0) {
		return temp;
	}
	for (i = 0; i < MAX_NUM_ROLLS; i++) {
		if (chosenMoves[i].from != 0 || chosenMoves[i].to != 0) {
			temp = chosenMoves[i];
			chosenMoves[i].from = 0;
			chosenMoves[i].to = 0;
			numChosen--;
			return temp;
		}
	}
	return temp;
}




double Player::getBoardVal(GameBoard *board) {
	if (board->isBlackVictory()) {
		return BLACK_WIN;
	}
	else if (board->isWhiteVictory()) {
		return WHITE_WIN;
	}
	if (ptype == AI_TYPE_A) {
		return getBasicBoardVal(board);
	}
	else if (ptype == AI_TYPE_B) {
		return getAdvBoardVal(board);
	}

	return 0.0;
}


// Overloaded version for SGD
double Player::getBoardVal(GameBoard *board, double *inputs) {
	if (board->isBlackVictory()) {
		return BLACK_WIN;
	}
	else if (board->isWhiteVictory()) {
		return WHITE_WIN;
	}
	if (ptype == AI_TYPE_A) {
		return getBasicBoardVal(board, inputs);
	}
	else if (ptype == AI_TYPE_B) {
		return getAdvBoardVal(board, inputs);
	}

	return 0.0;
}


double Player::getBasicBoardVal(GameBoard *board) {
	GamePoint *boardPoints = board->getPoints();
	double totalVal = 0.0;
	int i;
	for (i = 0; i < NUM_POINTS; i++) {
		totalVal += weights[i] * (double)(boardPoints[i].getNumCheckers() * boardPoints[i].getControl());
		if (boardPoints[i].getNumCheckers() == 1) {
			totalVal += weights[i + NUM_FEATS_A] * (double)(1 * boardPoints[i].getControl());
		}
	}
	totalVal += weights[WBAR] * (double)(board->getNumOnBar(WHITE));
	totalVal += weights[BBAR] * (double)(board->getNumOnBar(BLACK));
	totalVal += weights[WHOME] * (double)(board->getWhiteInHome());
	totalVal += weights[BHOME] * (double)(board->getBlackInHome());
	return totalVal;
}


// Overloaded version for SGD
double Player::getBasicBoardVal(GameBoard *board, double *inputs) {
	GamePoint *boardPoints = board->getPoints();
	double totalVal = 0.0;
	int i;
	for (i = 0; i < NUM_POINTS; i++) {
		inputs[i] = (double)(boardPoints[i].getNumCheckers() * boardPoints[i].getControl());
		totalVal += weights[i] * inputs[i];
		if (boardPoints[i].getNumCheckers() == 1) {
			inputs[i + NUM_FEATS_A] = (double)(1 * boardPoints[i].getControl());
			totalVal += weights[i + NUM_FEATS_A] * inputs[i + NUM_FEATS_A];
		}
	}
	inputs[WBAR] = (double)(board->getNumOnBar(WHITE));
	inputs[BBAR] = (double)(board->getNumOnBar(BLACK));
	inputs[WHOME] = (double)(board->getWhiteInHome());
	inputs[BHOME] = (double)(board->getBlackInHome());
	totalVal += weights[WBAR] * inputs[WBAR];
	totalVal += weights[BBAR] * inputs[BBAR];
	totalVal += weights[WHOME] * inputs[WHOME];
	totalVal += weights[BHOME] * inputs[BHOME];
	return totalVal;
}

double Player::getAdvBoardVal(GameBoard *board) {
	GamePoint *boardPoints = board->getPoints();
	double totalVal = 0.0;
	int blockAcc, currAcc;

	int i;
	for (i = 0; i < NUM_POINTS; i++) {
		totalVal += weights[i] * (double)(boardPoints[i].getNumCheckers() * boardPoints[i].getControl());
		if (boardPoints[i].getNumCheckers() == 1) {
			totalVal += weights[i + NUM_FEATS_B] * (double)(1 * boardPoints[i].getControl());
		}
	}
	totalVal += weights[WBAR] * (double)(board->getNumOnBar(WHITE));
	totalVal += weights[BBAR] * (double)(board->getNumOnBar(BLACK));
	totalVal += weights[WHOME] * (double)(board->getWhiteInHome());
	totalVal += weights[BHOME] * (double)(board->getBlackInHome());

	// Handpicked feature detection
	// White blockade
	blockAcc = 0;
	currAcc = 0;
	for (i = 0; i < NUM_POINTS; i++) {
		if (boardPoints[i].getControl() == WHITE && boardPoints[i].getNumCheckers() > 1) {
			currAcc += 2;
			blockAcc += currAcc;
		}
		else {
			if (currAcc) {
				currAcc -= 1;
			}
		}
	}
	totalVal += weights[WBLOCK] * (double)(blockAcc);

	// Black blockade
	blockAcc = 0;
	currAcc = 0;
	for (i = 0; i < NUM_POINTS; i++) {
		if (boardPoints[i].getControl() == BLACK && boardPoints[i].getNumCheckers() > 1) {
			currAcc += 2;
			blockAcc += currAcc;
		}
		else {
			if (currAcc) {
				currAcc -= 1;
			}
		}
	}
	totalVal += weights[BBLOCK] * (double)(blockAcc);

	// White getting hit
	blockAcc = 0; // reuse this as hit accumulation
	currAcc = 0;
	for (i = NUM_POINTS - 1; i >= 0; i--) {
		if (boardPoints[i].getControl() == WHITE && boardPoints[i].getNumCheckers() == 1) {
			currAcc += 7;
			if (i < 6 && board->getNumOnBar(BLACK)) {
				blockAcc += 1;
			}
		}
		if (boardPoints[i].getControl() == BLACK && currAcc) {
			blockAcc += (currAcc / 7) + 1;
		}
		if (currAcc) {
			currAcc--;
		}
	}
	totalVal += weights[WHIT] * (double)(blockAcc);

	// Black getting hit
	blockAcc = 0; // reuse this as hit accumulation
	currAcc = 0;
	for (i = 0; i < NUM_POINTS; i++) {
		if (boardPoints[i].getControl() == BLACK && boardPoints[i].getNumCheckers() == 1) {
			currAcc += 7;
			if (i > 17 && board->getNumOnBar(WHITE)) {
				blockAcc += 1;
			}
		}
		if (boardPoints[i].getControl() == WHITE && currAcc) {
			blockAcc += (currAcc / 7) + 1;
		}
		if (currAcc) {
			currAcc--;
		}
	}
	totalVal += weights[BHIT] * (double)(blockAcc);

	return totalVal;
}

// Overloaded version for SGD
double Player::getAdvBoardVal(GameBoard *board, double *inputs) {
	GamePoint *boardPoints = board->getPoints();
	double totalVal = 0.0;
	int blockAcc, currAcc;

	int i;
	for (i = 0; i < NUM_POINTS; i++) {
		inputs[i] = (double)(boardPoints[i].getNumCheckers() * boardPoints[i].getControl());
		totalVal += weights[i] * inputs[i];
		if (boardPoints[i].getNumCheckers() == 1) {
			inputs[i + NUM_FEATS_B] = (double)(1 * boardPoints[i].getControl());
			totalVal += weights[i + NUM_FEATS_B] * inputs[i + NUM_FEATS_B];
		}
	}
	inputs[WBAR] = (double)(board->getNumOnBar(WHITE));
	inputs[BBAR] = (double)(board->getNumOnBar(BLACK));
	inputs[WHOME] = (double)(board->getWhiteInHome());
	inputs[BHOME] = (double)(board->getBlackInHome());
	totalVal += weights[WBAR] * inputs[WBAR];
	totalVal += weights[BBAR] * inputs[BBAR];
	totalVal += weights[WHOME] * inputs[WHOME];
	totalVal += weights[BHOME] * inputs[BHOME];

	// Handpicked feature detection
	// White blockade
	blockAcc = 0;
	currAcc = 0;
	for (i = 0; i < NUM_POINTS; i++) {
		if (boardPoints[i].getControl() == WHITE && boardPoints[i].getNumCheckers() > 1) {
			currAcc += 2;
			blockAcc += currAcc;
		}
		else {
			if (currAcc) {
				currAcc -= 1;
			}
		}
	}
	inputs[WBLOCK] = (double)(blockAcc);
	totalVal += weights[WBLOCK] * inputs[WBLOCK];

	// Black blockade
	blockAcc = 0;
	currAcc = 0;
	for (i = 0; i < NUM_POINTS; i++) {
		if (boardPoints[i].getControl() == BLACK && boardPoints[i].getNumCheckers() > 1) {
			currAcc += 2;
			blockAcc += currAcc;
		}
		else {
			if (currAcc) {
				currAcc -= 1;
			}
		}
	}
	inputs[BBLOCK] = (double)(blockAcc);
	totalVal += weights[BBLOCK] * inputs[BBLOCK];

	// White getting hit
	blockAcc = 0; // reuse this as hit accumulation
	currAcc = 0;
	for (i = NUM_POINTS - 1; i >= 0; i--) {
		if (boardPoints[i].getControl() == WHITE && boardPoints[i].getNumCheckers() == 1) {
			currAcc += 7;
			if (i < 6 && board->getNumOnBar(BLACK)) {
				blockAcc += 1;
			}
		}
		if (boardPoints[i].getControl() == BLACK && currAcc) {
			blockAcc += (currAcc / 7) + 1;
		}
		if (currAcc) {
			currAcc--;
		}
	}
	inputs[WHIT] = (double)(blockAcc);
	totalVal += weights[WHIT] * inputs[WHIT];

	// Black getting hit
	blockAcc = 0; // reuse this as hit accumulation
	currAcc = 0;
	for (i = 0; i < NUM_POINTS; i++) {
		if (boardPoints[i].getControl() == BLACK && boardPoints[i].getNumCheckers() == 1) {
			currAcc += 7;
			if (i > 17 && board->getNumOnBar(WHITE)) {
				blockAcc += 1;
			}
		}
		if (boardPoints[i].getControl() == WHITE && currAcc) {
			blockAcc += (currAcc / 7) + 1;
		}
		if (currAcc) {
			currAcc--;
		}
	}
	inputs[BHIT] = (double)(blockAcc);
	totalVal += weights[BHIT] * inputs[BHIT];

	return totalVal;
}

int Player::getPtype() {
	return ptype;
}


void Player::backpropagate(BoardList * pastBoards) {
	FILE * weightsFile;
	BoardList * tmp = pastBoards; // initial board is definitive
	BoardList * prev = NULL;
	double *inputs = new double[numFeats];
	double prevTarget = getBoardVal(&(tmp->board));
	double nextTarget = 0.0;
	double nextActual = 0.0;
	int i;

	if (tmp == NULL) {
		return;
	}

	

	while (tmp->next != NULL) {
		prev = tmp;
		tmp = tmp->next;
		nextTarget = LAMBDA * prevTarget;
		prevTarget = nextTarget;
		for (i = 0; i < numFeats; i++) {
			inputs[i] = 0.0;
		}
		nextActual = getBoardVal(&(tmp->board), inputs);
		for (i = 0; i < numFeats; i++) {
			weights[i] += L_RATE*(nextTarget - nextActual)*inputs[i]; // Stochastic Gradient Descent
		}
	}

	errno_t err = fopen_s(&weightsFile, weightsFileName, "w");
	if (err == 0) {
		for (int i = 0; i < numFeats; i++) {
			fprintf(weightsFile, "%.3f\n", weights[i]);
		}
		fclose(weightsFile);
	}

	return;
}

