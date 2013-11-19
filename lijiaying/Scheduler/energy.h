#ifndef _ENERGY_H_
#define _ENERGY_H_
#include <stdio.h>
#include <stdlib.h>

/*
 * best usage:
 * 	High: 60%	120ms	12
 * 	Low:  40%	800ms	8
 */
#define CIRCLE 40
#define SOFT_THRETHOLD 3/5
#define NORAML_THRETHOLD 2/5

//#define best_relax_time BTY.best_relax_time
//#define left_energy BTY.left_energy
extern int global_time;


struct battery {
	int latest_energy;
//	int consumed_energy;
//	void (*left_energy)(struct battery*);
	void (*left_energy)();

	int consume_time;
	int relax_time;

	//	int last_left_time;
//	int (*best_relax_time)(struct battery*);
	int (*best_relax_time)();
};


/*
 * We assume the battery can be devided into 1 000 000 pieces
 */
struct battery BTY;



void BTY_left_energy()
	//int* last_energy, int consume_time, int relax_time)
{
	struct battery* bty = &BTY;
	int energy = bty->latest_energy;
	/*
	 * Battery function:
	 * b>c    m = a - b + c*0.4;
	 * b<= c  m = a - b + b * 0.4 + (c-b)*0.3
	 * 		  m > a*0.8 ===> m = a * 0.8
	 */
	bty->latest_energy = energy - bty->consume_time;
	if (bty->latest_energy <= 0) {
		bty->latest_energy = 0;
		return;
	}
	if (bty->relax_time < bty->consume_time) {
		bty->latest_energy += bty->relax_time * 0.4;
	} else {
		bty->latest_energy += bty->consume_time * 0.4 + (bty->relax_time - bty->consume_time) * 0.3;
		if (bty->latest_energy > energy * 0.8) {
			bty->latest_energy = energy * 0.8;
		}
	}
	return;
}



int BTY_best_relax_time()
	//int* left_energy, int consume_energy, int last_left_time)
{
	struct battery* bty = &BTY;
	if (bty->consume_time == 0) {
		bty->relax_time = 0;
		return 0;
	}
	return bty->consume_time - bty->relax_time;
}



int continue_consume_time()
{
	struct battery* bty = &BTY;
//	return 20;
	/* 
	 * 80% is a thredhold, danger!
	 * only hard real time can work above 70%
	 * 60% is worked for soft real time task
	 */
	if (bty->consume_time >= CIRCLE * SOFT_THRETHOLD) {
		return -2;
	}
	if (bty->consume_time >= CIRCLE * NORAML_THRETHOLD) {
		return -1;
	}
	return (CIRCLE * SOFT_THRETHOLD - bty->consume_time) * 4 / 3;
}




int keep_battery(int used_time_full, int used_time_half, int relax_time)
{
	struct battery* bty = &BTY;
//	if (global_time % 100 == 0) {
	bty->consume_time += used_time_full;
	bty->consume_time += used_time_half / 2;

	bty->relax_time += relax_time;
	if (bty->consume_time + bty->relax_time >= 20) {
		bty->left_energy();
		bty->consume_time = 0;
		bty->relax_time = 0;
	}
	return 0;
}



void initialize_battery()
{
	struct battery* bty = &BTY;
	bty->latest_energy = 1000000;
//	BTY.last_left_time = 0;
	bty->consume_time = 0;
	bty->relax_time = 0;
	bty->left_energy = BTY_left_energy;
	bty->best_relax_time = BTY_best_relax_time;
}


#endif
