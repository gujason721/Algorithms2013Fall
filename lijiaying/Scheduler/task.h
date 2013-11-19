#ifndef _TASK_H_
#define _TASK_H_

#include <stdio.h>
#include <stdlib.h>


#define Priority_Num	140
#define DEFAULT_PRIORITY_NORMAL 120
#define DEFAULT_PRIORITY_SOFTRT 80
#define DEFAULT_PRIORITY_HARDRT 20
#define DEFAULT_BARE_RATIO 10

// global_time used as the global time of the virtual CPU
static int global_time = 0;

// This part is used to get some statistical information of the schedule
// The num of tasks of each kind
int hdrt = 0;
int sfrt = 0;
int nml = 0;

// The timie out task num
int hdrtmt = 0;
int sfrtmt = 0;
int nmlmt = 0;



/*
 * Every bit as a priority, which correpond to the priority queues
 * | 31....0 | 63...32 | 95...64 | 127...96 | 159...139...128|
 */
int Priority_map[5] = {0, 0, 0, 0, 0};

int set_priority_map(int i)
{
	if (i >= Priority_Num)
		return -1;
	int j = i / 32;
	int k = 31 - i % 32;
	Priority_map[j] |= (1 << k);
	return 0;
}
int clear_priority_map(int i)
{
	if (i >= Priority_Num)
		return -1;
	int j = i / 32;
	int k = 31 - i % 32;
	Priority_map[j] &= ~(0 << k);
	return 0;
}
/*
 * start is first priority we want to start.
 * Because we may want to find the task in a given range of priority....
 * We set "start"
 */
int find_unempty_priority(int start)
{
	if (start >= 140) {
		return -1;
	}
	int i = start / 32;
	int j = start % 32;
	while (i < 5) {
		if (Priority_map[i] << j != 0) {
			while (j < 32) {
				if (Priority_map[i] | (1 << (31 - j)) != 0) {
					if (i * 32 + j < Priority_Num) {
						return i * 32 + j;
					}
					else {
						return -1;
					}
				}
				j++;
			}
		} else {
			i++;
			j = 0;
		}
	}
	return -1;
}




/*
 * One main list for all the process
 * Each node represents a priority, the processes 
 * in on one list are of the same priority.
 * Every priority has two list, active and non-active
 * When scheduling, pick up the highest priority from avtive list.
 */

struct tasklist;


// // hard realtime, soft realtime, normal 
// // for the hard realtime, we use the greedy algorithm 
// // for the soft realtime, we run them as soon as possible, 
// // 							the constrain is hard realtimg running 
// // 							another one constains is to void normal task hungrey 
// // 							we set the ratio to be 20% for normal task 
// // for the normal ones, we need them to simulate the curve as good as possible
//enum TASKTYPE {NORMAL, SOFT_RT, HARD_RT};
#define NORMAL 0
#define SOFT_RT 1
#define HARD_RT 2


struct task_struct{
	int task_id;
	//	enum TASKTYPE type;
	int type;
	int starttime;
	int deadline;
	int runtime;
	int priority;
	int bare_ratio;

	int time_runned;
	float last_run_time_ratio;
	/* Reserved, for we make an adjudgement to priority.
	 * If last_run_time_ratio < 1, we will put it to a higher
	 * priority, so that the next time, it can overbeat 
	 * tasks with some more tasks.
	 * And its running time also get a bonus, which depends
	 * on its priority and ratio.
	 */

	struct task_struct* prev;
	struct task_struct* next;

	struct task_list* tl;
};



struct task_list{
	//	enum TASKTYPE type;				
	int type;
	struct task_struct* active;
	struct task_struct* notactive;
	int total_num;
	int active_num;
	int notactive_num;
};

struct task_list tasklist[Priority_Num];

#endif
