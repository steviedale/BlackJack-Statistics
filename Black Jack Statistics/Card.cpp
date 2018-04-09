#include "Card.h"

Card::Card(int face, int suit) : face(face), suit(suit)
{
	// init value
	if (face == 0) value = 11;
	else if (face > 9) value = 10;
	else value = face + 1;
}

Card::~Card()
{
}