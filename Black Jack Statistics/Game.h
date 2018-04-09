#pragma once
#include <stdio.h>
#include <time.h>
#include "Hand.h"

class Game
{
public:
	Game();
	Game(int num_decks, int max_rounds, int num_other_players, double bank_start, double bet_min, double cashout, bool trace);
	~Game();

	void start();
	void createDeck();
	void shuffleDeck();
	void printList(Hand*);
	void printList(Card*);
	void swapValues(Card*, Card*);
	void dealOut();
	int dealCard(Card**, Card**);
	int getValue(Card *);
	int getSoftHardValue(Card*);
	bool isSplitable(Card*);
	void HitStayDoubleSplit(Hand*);
	char getDecision(Hand*);
	void dealDealer();
	double payOut(Hand*);
	void recycleAllHands();
	void recycleHand(Hand*);
	void recycleCard(Card*);
	void addToBurn(Card*);
	void resetDeck();
	void deallocateDeck(Card*);

	int pull_count = 0, rounds_played = 0, num_decks, max_rounds, num_other_players;
	double bank_start, bank, bet_min, cashout;
	bool isDealerBJ = false;
	Card *deck = NULL, *dealer, *burn_pile = NULL;
	Hand player, *players;
	bool trace;

	const char even_chart[34][10] = {
		// HARD
		// 2    3    4    5    6    7    8    9    10   A
		{ 'H', 'H', 'H', 'H', 'H', 'H', 'H', 'H', 'H', 'H' }, // 5
		{ 'H', 'H', 'H', 'H', 'H', 'H', 'H', 'H', 'H', 'H' }, // 6
		{ 'H', 'H', 'H', 'H', 'H', 'H', 'H', 'H', 'H', 'H' }, // 7
		{ 'H', 'H', 'H', 'H', 'H', 'H', 'H', 'H', 'H', 'H' }, // 8
		{ 'H', 'D', 'D', 'D', 'D', 'H', 'H', 'H', 'H', 'H' }, // 9
		{ 'D', 'D', 'D', 'D', 'D', 'D', 'D', 'D', 'H', 'H' }, // 10
		{ 'D', 'D', 'D', 'D', 'D', 'D', 'D', 'D', 'D', 'D' }, // 11
		{ 'H', 'H', 'S', 'S', 'S', 'H', 'H', 'H', 'H', 'H' }, // 12
		{ 'S', 'S', 'S', 'S', 'S', 'H', 'H', 'H', 'H', 'H' }, // 13
		{ 'S', 'S', 'S', 'S', 'S', 'H', 'H', 'H', 'H', 'H' }, // 14
		{ 'S', 'S', 'S', 'S', 'S', 'H', 'H', 'H', 'H', 'H' }, // 15
		{ 'S', 'S', 'S', 'S', 'S', 'H', 'H', 'H', 'H', 'H' }, // 16
		{ 'S', 'S', 'S', 'S', 'S', 'S', 'S', 'S', 'S', 'S' }, // 17
		{ 'S', 'S', 'S', 'S', 'S', 'S', 'S', 'S', 'S', 'S' }, // 18
		{ 'S', 'S', 'S', 'S', 'S', 'S', 'S', 'S', 'S', 'S' }, // 19
		{ 'S', 'S', 'S', 'S', 'S', 'S', 'S', 'S', 'S', 'S' }, // 20
		// SOFT
		// 2    3    4    5    6    7    8    9    10   A
		{ 'H', 'H', 'H', 'D', 'D', 'H', 'H', 'H', 'H', 'H' }, // -13
		{ 'H', 'H', 'H', 'D', 'D', 'H', 'H', 'H', 'H', 'H' }, // -14
		{ 'H', 'H', 'D', 'D', 'D', 'H', 'H', 'H', 'H', 'H' }, // -15
		{ 'H', 'H', 'D', 'D', 'D', 'H', 'H', 'H', 'H', 'H' }, // -16
		{ 'H', 'D', 'D', 'D', 'D', 'H', 'H', 'H', 'H', 'H' }, // -17
		{ 'Y', 'Y', 'Y', 'Y', 'Y', 'S', 'S', 'H', 'H', 'H' }, // -18
		{ 'S', 'S', 'S', 'S', 'Y', 'S', 'S', 'S', 'S', 'S' }, // -19
		{ 'S', 'S', 'S', 'S', 'S', 'S', 'S', 'S', 'S', 'S' }, // -20
		// PAIR
		// 2    3    4    5    6    7    8    9    10   A
		{ 'P', 'P', 'P', 'P', 'P', 'P', 'H', 'H', 'H', 'H' }, // 2,2
		{ 'P', 'P', 'P', 'P', 'P', 'P', 'H', 'H', 'H', 'H' }, // 3,3
		{ 'H', 'H', 'H', 'P', 'P', 'H', 'H', 'H', 'H', 'H' }, // 4,4
		{ 'D', 'D', 'D', 'D', 'D', 'D', 'D', 'D', 'H', 'H' }, // 5,5
		{ 'P', 'P', 'P', 'P', 'P', 'H', 'H', 'H', 'H', 'H' }, // 6,6
		{ 'P', 'P', 'P', 'P', 'P', 'P', 'H', 'H', 'H', 'H' }, // 7,7	
		{ 'P', 'P', 'P', 'P', 'P', 'P', 'P', 'P', 'P', 'P' }, // 8,8
		{ 'P', 'P', 'P', 'P', 'P', 'S', 'P', 'P', 'S', 'S' }, // 9,9
		{ 'S', 'S', 'S', 'S', 'S', 'S', 'S', 'S', 'S', 'S' }, // 10,10
		{ 'P', 'P', 'P', 'P', 'P', 'P', 'P', 'P', 'P', 'P' }, // A,A
	};
	const char bitch_ass_chart[34][10] = {
		// 2    3    4    5    6    7    8    9    10   A
		{ 'H', 'H', 'H', 'H', 'H', 'H', 'H', 'H', 'H', 'H' }, // 5
		{ 'H', 'H', 'H', 'H', 'H', 'H', 'H', 'H', 'H', 'H' }, // 6
		{ 'H', 'H', 'H', 'H', 'H', 'H', 'H', 'H', 'H', 'H' }, // 7
		{ 'H', 'H', 'H', 'D', 'D', 'H', 'H', 'H', 'H', 'H' }, // 8
		{ 'D', 'D', 'D', 'D', 'D', 'H', 'H', 'H', 'H', 'H' }, // 9
		{ 'D', 'D', 'D', 'D', 'D', 'D', 'D', 'D', 'H', 'H' }, // 10
		{ 'D', 'D', 'D', 'D', 'D', 'D', 'D', 'D', 'D', 'D' }, // 11
		{ 'H', 'H', 'S', 'S', 'S', 'H', 'H', 'H', 'H', 'H' }, // 12
		{ 'S', 'S', 'S', 'S', 'S', 'H', 'H', 'H', 'H', 'H' }, // 13
		{ 'S', 'S', 'S', 'S', 'S', 'H', 'H', 'H', 'H', 'H' }, // 14
		{ 'S', 'S', 'S', 'S', 'S', 'H', 'H', 'H', 'H', 'H' }, // 15
		{ 'S', 'S', 'S', 'S', 'S', 'H', 'H', 'H', 'H', 'H' }, // 16
		{ 'S', 'S', 'S', 'S', 'S', 'S', 'S', 'S', 'S', 'S' }, // 17
		{ 'S', 'S', 'S', 'S', 'S', 'S', 'S', 'S', 'S', 'S' }, // 18
		{ 'S', 'S', 'S', 'S', 'S', 'S', 'S', 'S', 'S', 'S' }, // 19
		{ 'S', 'S', 'S', 'S', 'S', 'S', 'S', 'S', 'S', 'S' }, // 20
															  // SOFT
		// 2    3    4    5    6    7    8    9    10   A
		{ 'H', 'H', 'D', 'D', 'D', 'H', 'H', 'H', 'H', 'H' }, // -13
		{ 'H', 'H', 'D', 'D', 'D', 'H', 'H', 'H', 'H', 'H' }, // -14
		{ 'H', 'H', 'D', 'D', 'D', 'H', 'H', 'H', 'H', 'H' }, // -15
		{ 'H', 'H', 'D', 'D', 'D', 'H', 'H', 'H', 'H', 'H' }, // -16
		{ 'D', 'D', 'D', 'D', 'D', 'H', 'H', 'H', 'H', 'H' }, // -17
		{ 'S', 'Y', 'Y', 'Y', 'Y', 'S', 'S', 'H', 'H', 'S' }, // -18
		{ 'S', 'S', 'S', 'S', 'Y', 'S', 'S', 'S', 'S', 'S' }, // -19
		{ 'S', 'S', 'S', 'S', 'S', 'S', 'S', 'S', 'S', 'S' }, // -20
															  // PAIR
		// 2    3    4    5    6    7    8    9    10   A
		{ 'P', 'P', 'P', 'P', 'P', 'P', 'H', 'H', 'H', 'H' }, // 2,2
		{ 'P', 'P', 'P', 'P', 'P', 'P', 'P', 'H', 'H', 'H' }, // 3,3
		{ 'H', 'H', 'P', 'P', 'P', 'H', 'H', 'H', 'H', 'H' }, // 4,4
		{ 'D', 'D', 'D', 'D', 'D', 'D', 'D', 'D', 'H', 'H' }, // 5,5
		{ 'P', 'P', 'P', 'P', 'P', 'P', 'H', 'H', 'H', 'H' }, // 6,6
		{ 'P', 'P', 'P', 'P', 'P', 'P', 'P', 'H', 'S', 'H' }, // 7,7	
		{ 'P', 'P', 'P', 'P', 'P', 'P', 'P', 'P', 'P', 'P' }, // 8,8
		{ 'P', 'P', 'P', 'P', 'P', 'S', 'P', 'P', 'S', 'S' }, // 9,9
		{ 'S', 'S', 'S', 'S', 'S', 'S', 'S', 'S', 'S', 'S' }, // 10,10
		{ 'P', 'P', 'P', 'P', 'P', 'P', 'P', 'P', 'P', 'P' }, // A,A
	};
	const char faces[13][6] = { "Ace", "2", "3", "4", "5", "6", "7", "8", "9", "10", "Jack", "Queen", "King" };
	const char suits[4][9] = { "Spades", "Clubs", "Hearts", "Diamonds" };
};

