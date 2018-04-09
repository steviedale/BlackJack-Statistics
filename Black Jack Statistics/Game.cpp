#include "Game.h"
#define CHART even_chart
#define BJ_RATIO 6/5

Game::Game() : Game(4, 100, 4, 100, 5, 1.5, false)
{
}
Game::Game(int num_decks, int max_rounds, int num_other_players, double bank_start, double bet_min, double cashout, bool trace) :
	num_decks(num_decks),
	max_rounds(max_rounds),
	num_other_players(num_other_players),
	bank_start(bank_start),
	bank(bank_start),
	bet_min(bet_min),
	cashout(cashout),
	trace(trace)
{
	start();
}
Game::~Game()
{
	resetDeck();
	deallocateDeck(deck);
}

void Game::start()
{
	createDeck();
	shuffleDeck();
	players = new Hand [num_other_players];
	
	while (bank >= bet_min && bank < bank_start*cashout && rounds_played < max_rounds)
	{
		
		if (pull_count > num_decks * 26)
			resetDeck();
	 	if (trace) printf("BANK: %.2lf\n", bank);
		dealOut();
		if (trace) printf("Dealer's front card is: %s of %s\n", faces[dealer->face], suits[dealer->suit]);

		if (getValue(dealer) == 21)
			isDealerBJ = true;
		if (getValue(player.hand) == 21)
			player.isBlackJack = true;
		if (isDealerBJ && !player.isBlackJack)
		{
			if (trace)
			{
				printf("Your Hand:\n");
				printList(player.hand);
				printf("Dealer has Black Jack!\n");
				printList(dealer);
			}
		}
		else
		{
			if (!isDealerBJ && player.isBlackJack)
			{
				if (trace) {
					printf("Your Hand:\n");
					printList(player.hand);
					printf("Black Jack!\n");
				}
			}
			else
				HitStayDoubleSplit(&player);
			
		}
		if (!isDealerBJ)
			for (int i = 0; i < num_other_players; ++i) {
				if (trace) printf("== Player: %d\n", i + 1);
				HitStayDoubleSplit((players + i));
			}
		
		dealDealer();

		if (trace) {
			printf("Dealer's Hand:\n");
			printList(dealer);
		}
		
		///////////////////////////////
		 if (trace)
			for (int i = 0; i < num_other_players; ++i)
			{
				printf("\nCom #%d", i);
				printList((players + i)->hand);
			}
		///////////////////////////////
		double bank_offset = payOut(&player);
		bank += bank_offset;
		recycleAllHands();
		++rounds_played;
	}
	if (trace) {
		printf("Rounds Played: %d\n", rounds_played);
		printf("Bank: %.2lf\n", bank);
	}
	/*//CHECK DECK////
	printf("DECK:\n");
	printList(deck);
	*/////////////////
}
void Game::deallocateDeck(Card *deck)
{
	if (deck->next != NULL)
		deallocateDeck(deck->next);
	free(deck);
}
void Game::createDeck()
{
	Card *previousCard = NULL;
	for (int i = 0; i < num_decks; ++i)
	{
		for (int s = 0; s < 4; ++s)
		{
			for (int f = 0; f < 13; ++f)
			{
				Card *temp = (Card*)malloc(sizeof(Card));
				if (temp == 0) printf("Out of memory!!");

				temp->suit = s;
				temp->face = f;

				if (f == 10 || f == 11 || f == 12)
					temp->value = 10;
				else if (f == 0)
					temp->value = 11;
				else
					temp->value = f + 1;

				if (i == 0 && s == 0 && f == 0)
					deck = temp;
				else
					previousCard->next = temp;
				// set last card's next to null
				if (i == num_decks - 1 && s == 3 && f == 12)
					temp->next = NULL;
				previousCard = temp;
			}
		}
	}
}
void Game::printList(Hand *hand)
{
	if (hand->split_1 == NULL)
		printList(hand->hand);
	else
	{
		printList(hand->split_1);
		printf("\nSplit\n");
		printList(hand->split_2);
	}
}
void Game::printList(Card *card)
{

	for (Card *c = card; c != NULL; c = c->next)
		printf("\t%s of %s\n", faces[c->face], suits[c->suit]);
	
}
void Game::shuffleDeck()
{
	Card *swap = NULL;
	for (Card *current = deck; current != NULL; current = current->next)
	{
		int swap_val = rand() % (num_decks * 52 - 1) + 1;
		swap = current;
		for (int i = 0; i < swap_val; ++i)
		{
			if (swap->next == NULL) {
				swap = deck;
			}
			else {
				swap = swap->next;
			}
		}
		swapValues(current, swap);
	}
}
void Game::swapValues(Card *card1, Card *card2)
{
	Card *temp = (Card*)malloc(sizeof(Card));
	temp->face = card2->face;
	temp->suit = card2->suit;
	temp->value = card2->value;
	card2->face = card1->face;
	card2->suit = card1->suit;
	card2->value = card1->value;
	card1->face = temp->face;
	card1->suit = temp->suit;
	card1->value = temp->value;
	free(temp);
}
void Game::dealOut()
{
	isDealerBJ = false;
	player.reset();
	dealer = NULL;
	// place bet
	bank -= bet_min;
	player.bet = bet_min;
	for (int i = 0; i < num_other_players; ++i)
		(players+i)->reset();
	// deal initial cards
	for (int i = 0; i < 2; ++i) {
		dealCard(&dealer, &deck);
		dealCard(&player.hand, &deck);
		for (int j = 0; j < num_other_players; ++j)
			dealCard(&(players + j)->hand, &deck);
	}

}
int Game::dealCard(Card **to, Card **from)
{
	if (*from == deck)
		++pull_count;
	if (*from == NULL)
	{
		printf("NULL FROM");
		return -1;
	}
	else
	{
		Card *temp1, *temp2;
		if (*to == NULL)
		{
			// temp1 = 2nd card in deck
			temp1 = (*from)->next;
			// hand = 1st card in deck
			*to = *from;
			// deck points to 2nd card
			*from = (*from)->next;
			(*to)->next = NULL;
		}
		else
		{
			temp1 = (*from)->next;
			temp2 = *to;
			*to = *from;
			(*to)->next = temp2;
			*from = temp1;
		}
	}
	return 1;
}
void Game::HitStayDoubleSplit(Hand *hand)
{
	char decision;
	if (trace) {
		printf("Your Hand:\n");
		printList(hand->hand);
	}
	if (getValue(hand->hand) == 21) {
		hand->isBlackJack = true;
		if (trace) printf("Black Jack!\n");
	}
	else {
		hand->isBlackJack = 0;
		do {
			decision = getDecision(hand);
			if (trace) printf("DECISION: ** %c **\n", decision);
			switch (decision)
			{
			case 'S':
				break;
			case 'H':
				dealCard(&hand->hand, &deck);
				break;
			case 'D':
				if (hand->hand->next->next == NULL)
				{
					dealCard(&hand->hand, &deck);
					if (hand == &player) {
						bank -= hand->bet;
					}
					hand->bet *= 2;
					decision = 'S';
				}
				else
					printf("You can't double down!\n");
				break;
			case 'P':
				if (isSplitable(hand->hand))
				{
					hand->split_1 = new Hand();
					hand->split_2 = new Hand();
					if (hand->split_2 == 0) printf("ERROR: Out of Memory!!\n");

					hand->split_1->reset();
					hand->split_2->reset();

					if (hand == &player)
						bank -= hand->bet;
					hand->split_1->bet = hand->bet;
					hand->split_2->bet = hand->bet;
					hand->bet = 0;

					dealCard(&hand->split_1->hand, &hand->hand);
					dealCard(&hand->split_1->hand, &deck);
					HitStayDoubleSplit(hand->split_1);

					dealCard(&hand->split_2->hand, &hand->hand);
					dealCard(&hand->split_2->hand, &deck);
					HitStayDoubleSplit(hand->split_2);
				}
				else
					printf("You can't split!\n");
				break;
			default:
				printf("Incorrect Input!\n");
			}
			if (decision == 'P') break;
			if (trace) {
				printf("Your Hand:\n");
				printList(hand->hand);
			}
		} while (decision != 'S' && getValue(hand->hand) < 20);
	}
}
char Game::getDecision(Hand *hands)
{
	Card *hand = hands->hand;
	char decision;
	int column, row, handValue;
	if (dealer->face > 8)
		column = 8;
	else if (dealer->face == 0)
		column = 9;
	else
		column = dealer->face - 1;

	if (isSplitable(hand) && hands->bet <= bank)
	{
		if (hand->face == 0)
			row = 33;
		else if (hand->face > 8)
			row = 32;
		else
			row = hand->face + 23;
	}
	else
	{
		handValue = getSoftHardValue(hand);
		// if A-A or 2-2, and can't split
		if (handValue == -12 || handValue == 4)
			row = 0;
		else if (handValue < 0)
			row = abs(handValue) + 3;
		else
			row = handValue - 5;
	}
	decision = CHART[row][column];
	// Check Double's
	if ((decision == 'D' || decision == 'Y') && (hand->next->next != NULL || hands->bet > bank))
	{
		if (decision == 'D')
			return 'H';
		else
			return 'S';
	}
	else if (decision == 'Y')
		return 'D';
	else
		return decision;
}
int Game::getSoftHardValue(Card *hand)
{
	int val = 0;
	int numElevens = 0;
	while (hand != NULL)
	{
		if (hand->value == 11)
			++numElevens;
		val += hand->value;
		while (val > 21 && numElevens > 0)
		{
			val -= 10;
			--numElevens;
		}
		hand = hand->next;
	}
	if (numElevens > 0)
		return -val;
	else
		return val;
}
int Game::getValue(Card *hand)
{
	int val = 0;
	int numAces = 0;
	while (hand != NULL)
	{
		if (hand->value == 11)
			++numAces;
		val += hand->value;
		while (val > 21 && numAces > 0)
		{
			val -= 10;
			--numAces;
		}
		hand = hand->next;
	}
	return val;
}
bool Game::isSplitable(Card *hand)
{
	if (hand->face == hand->next->face && hand->next->next == NULL)
		return true;
	else
		return false;

}
void Game::dealDealer()
{
	int value = getSoftHardValue(dealer);
	while (value < 17 && value > -18)
	{
		dealCard(&dealer, &deck);
		value = getSoftHardValue(dealer);
	}
}
double Game::payOut(Hand *hand)
{
	double moneyDistribute = 0;
	if (hand->split_1 == NULL)
	{
		int playerValue = getValue(hand->hand);
		int dealerValue = getValue(dealer);
		char WinLosePush;
		if (isDealerBJ && !hand->isBlackJack)
			WinLosePush = 'l';
		else if (!isDealerBJ && hand->isBlackJack)
			WinLosePush = 'b';
		else if (playerValue > 21)
			WinLosePush = 'l';
		else if (dealerValue == playerValue)
			WinLosePush = 'p';
		else if (dealerValue > 21)
			WinLosePush = 'w';
		else if (dealerValue < playerValue)
			WinLosePush = 'w';
		else if (dealerValue > playerValue)
			WinLosePush = 'l';
		else
			printf("Something went wrong...\n");

		if (WinLosePush == 'p')
			moneyDistribute = hand->bet;
		else if (WinLosePush == 'l')
			moneyDistribute = 0;
		else if (WinLosePush == 'w')
			moneyDistribute = 2*hand->bet;
		else if (WinLosePush == 'b')
			moneyDistribute = (1.0 + BJ_RATIO)*hand->bet;
		else
			printf("oops, not again\n");
		return moneyDistribute;
	}
	else
	{
		return payOut(hand->split_1) + payOut(hand->split_2);
	}
}
void Game::recycleAllHands()
{
	recycleCard(dealer);
	recycleHand(&player);
	for (int i = 0; i < num_other_players; ++i)
		recycleHand(players + i);
}
void Game::recycleHand(Hand *hand)
{
	if (hand->split_1 != NULL)
	{
		recycleHand(hand->split_1);
		recycleHand(hand->split_2);
	}
	else
		recycleCard(hand->hand);

}
void Game::recycleCard(Card *card)
{
	if (card->next != NULL)
		recycleCard(card->next);
	addToBurn(card);
}
void Game::addToBurn(Card *card)
{
	card->next = burn_pile;
	burn_pile = card;
}
void Game::resetDeck()
{
	if (trace) printf("RESETING DECK!\n");
	Card *temp = burn_pile;
	while (burn_pile != NULL)
	{
		burn_pile = temp->next;
		temp->next = deck;
		deck = temp;
		temp = burn_pile;
	}
	pull_count = 0;
	shuffleDeck();
}