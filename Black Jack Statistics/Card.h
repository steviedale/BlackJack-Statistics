#include <stdlib.h>
class Card
{
public:
	int face;
	int suit;
	int value;
	Card *next;
	Card(int, int);
	~Card();
};

