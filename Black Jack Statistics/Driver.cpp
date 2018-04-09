#include "Game.h"
#include <iostream>
#include <chrono>

#define NUM_GAMES 100000
#define NUM_CYCLES 10
#define MIN_BET 10
#define MAX_HANDS 10
#define IS_PRINT false

using namespace std;
using namespace std::chrono;

void printLongGameStats(int);
void printSingleHandStats(int);

void main() {
	high_resolution_clock::time_point t1 = high_resolution_clock::now();
	srand(time(0));
	
	printSingleHandStats(NUM_CYCLES);
	
	high_resolution_clock::time_point t2 = high_resolution_clock::now();
	auto duration = duration_cast<milliseconds>(t2 - t1).count();
	printf("T_exec: %.2f\n", (double)duration/1000);
	system("pause");
}
void printLongGameStats(int NumCycles) {
	long float netEarnings = 0, ratio;
	int n175 = 0, n150 = 0, n125 = 0, n100 = 0, n75 = 0, n50 = 0, n25 = 0, n0 = 0, nU = 0, nL = 0,
		rounds = 0;
	double p175, p150, p125, p100, p75, p50, p25, p0, pU, pL;
	Game *game;
	for (int i = 0; i < NUM_GAMES; ++i)
	{
		game = new Game(2, 1, 0, 100, MIN_BET, 5, IS_PRINT);
		netEarnings += game->bank - game->bank_start;

		ratio = game->bank / game->bank_start;

		if (ratio >= 2) ++nU;
		else if (game->bank < game->bet_min) ++nL;

		if (ratio >= 1.75) ++n175;
		else if (ratio >= 1.5) ++n150;
		else if (ratio >= 1.25) ++n125;
		else if (ratio >= 1) ++n100;
		else if (ratio >= 0.75) ++n75;
		else if (ratio >= 0.5) ++n50;
		else if (ratio >= 0.25) ++n25;
		else ++n0;

		rounds += game->rounds_played;
		delete game;
	}
	cout << "TESTING: " << n100 << endl;

	rounds = rounds / NUM_GAMES;

	cout << rounds << '\t';
	printf("%.4f\t\t", netEarnings / NUM_GAMES);
	cout << nL << '\t';
	cout << nU << "\t";
	cout << n0 << '\t';
	cout << n25 << '\t';
	cout << n50 << '\t';
	cout << n75 << '\t';
	cout << n100 << '\t';
	cout << n125 << '\t';
	cout << n150 << '\t';
	cout << n175 << "\n\t\t\t";

	pU = 100 * (float)nU / NUM_GAMES;
	pL = 100 * (float)nL / NUM_GAMES;
	p175 = 100 * (float)n175 / NUM_GAMES;
	p150 = 100 * (float)n150 / NUM_GAMES;
	p125 = 100 * (float)n125 / NUM_GAMES;
	p100 = 100 * (float)n100 / NUM_GAMES;
	p75 = 100 * (float)n75 / NUM_GAMES;
	p50 = 100 * (float)n50 / NUM_GAMES;
	p25 = 100 * (float)n25 / NUM_GAMES;
	p0 = 100 * (float)n0 / NUM_GAMES;

	printf("%.1f\t", pL);
	printf("%.1f\t", pU);
	printf("%.1f\t", p0);
	printf("%.1f\t", p25);
	printf("%.1f\t", p50);
	printf("%.1f\t", p75);
	printf("%.1f\t", p100);
	printf("%.1f\t", p125);
	printf("%.1f\t", p150);
	printf("%.1f\n", p175);
}


void printSingleHandStats(int NumCycles)
{
	long float net_earnings = 0, percent_earnings;
	int lost_25_to_50 = 0, lost_0_to_25 = 0, won_0_to_25 = 0, lost_50_to_75 = 0, lost_75_to_all = 0, won_25_to_50 = 0, won_50_to_75 = 0, won_75_to_100 = 0, won_100_to_up = 0, error = 0;
	Game *game;
	for (int i = 0; i < NUM_GAMES; ++i)
	{
		game = new Game(2, MAX_HANDS, 1, 1000, MIN_BET, 5, IS_PRINT);
		percent_earnings = ((float)game->bank - (float)game->bank_start)/(float)(game->rounds_played*MIN_BET);
		net_earnings += percent_earnings;

		if (percent_earnings <= -0.75) ++lost_75_to_all;
		else if (percent_earnings <= -0.5)++lost_50_to_75;
		else if (percent_earnings <= -0.25)++lost_25_to_50;
		else if (percent_earnings <= 0)++lost_0_to_25;
		else if (percent_earnings <= 0.25)++won_0_to_25;
		else if (percent_earnings <= 0.5)++won_25_to_50;
		else if (percent_earnings <= 0.75)++won_50_to_75;
		else if (percent_earnings <= 1.0)++won_75_to_100;
		else ++won_100_to_up;

		delete game;
	}
	net_earnings /= NUM_GAMES;
	cout << "Avg. game earnings: " << net_earnings << endl;
	cout << "[-100,\t" << "[-75,\t" << "[-50,\t" << "[-25,\t" << "[0,\t" << "[25,\t" << "[50,\t" << "[75,\t" << "[100,\n";
	cout << " -75] \t" << " -50]\t" << " -25]\t" << " 0 ]\t" << " 25]\t" << " 50]\t" << " 75]\t" << "100]\t" << " UP]\n";
	printf("%.2f\t%.2f\t%.2f\t%.2f\t%.2f\t%.2f\t%.2f\t%.2f\t%.2f\t\n", 
		(float)lost_75_to_all / NUM_GAMES, 
		(float)lost_50_to_75 / NUM_GAMES, 
		(float)lost_25_to_50 / NUM_GAMES, 
		(float)lost_0_to_25 / NUM_GAMES, 
		(float)won_0_to_25 / NUM_GAMES, 
		(float)won_25_to_50 / NUM_GAMES, 
		(float)won_50_to_75 / NUM_GAMES, 
		(float)won_75_to_100 / NUM_GAMES, 
		(float)won_100_to_up / NUM_GAMES);						
}