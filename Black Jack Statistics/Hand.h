#include "Card.h"

class Hand
{
public:
	double bet;
	bool isBlackJack;
	Card *hand;
	Hand *split_1, *split_2;

	Hand();
	~Hand();
	void reset();
};

