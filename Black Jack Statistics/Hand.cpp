#include "Hand.h"

Hand::Hand()
{
	reset();
}


Hand::~Hand()
{
}

void Hand::reset()
{
	isBlackJack = false;
	hand = NULL;
	split_1 = NULL;
	split_2 = NULL;
}
