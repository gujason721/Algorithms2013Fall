#ifndef _SCHEDULE_H_
#define _SCHEDULE_H_
#include <assert.h>
#include "energy.h"

void initialize_task_list() 
{
	int i;
	for (i = 0; i < Priority_Num; i++) {
		tasklist[i].total_num = 0;
		tasklist[i].active_num = 0;
		tasklist[i].notactive_num = 0;
		tasklist[i].active = NULL;
		tasklist[i].notactive = NULL;
		tasklist[i].type = NORMAL;
		if (i < 40) {
			tasklist[i].type = HARD_RT;
		} else if (i < 100) {
			tasklist[i].type = SOFT_RT;
		}
	}
	/* 	CurTime		Task_id		Runtime		TimeLeft	Finished */
	//	printf ("CurTime\t\tTask_id\t\tRuntime\t\tTimeleft\t\tFinished?\n");
	printf ("curTime   state    runtime    OwnerOfCPU\n");
}


void put_task(struct task_struct* ts)
{
	/*
	 *
	 printf ("--> put task, task%d, type%d, starttime%d, deadline%d, runtime%d\n", ts->task_id, ts->type, ts->starttime, ts->deadline, ts->runtime);
	 *
	 * */
	if (ts->priority == 0) {
		switch(ts->type) {
			case 0:
				ts->priority = DEFAULT_PRIORITY_NORMAL;
				break;
			case 1:
				ts->priority = DEFAULT_PRIORITY_SOFTRT;
				break;
			case 2:
				ts->priority = DEFAULT_PRIORITY_HARDRT;
				break;
		}
	}
	if (ts->bare_ratio == 0) {
		ts->bare_ratio = DEFAULT_BARE_RATIO;
	}
	ts->time_runned = 0;
	ts->last_run_time_ratio = 1;

	struct task_list* tl = &tasklist[ts->priority];
	struct task_struct* t = tl->active;
	tl->active_num++;
	//	tl->active = ts;
	//	ts->next = ts;
	//	ts->prev = ts;
	if (t == NULL) {
		tl->active = ts;
		ts->next = ts;
		ts->prev = ts;
	} else {
		if (t->bare_ratio < ts->bare_ratio) {
			while ((t->next != NULL) && (t->bare_ratio < ts->bare_ratio)) {
				t = t->next;
			}
		}
		ts->next = t;
		ts->prev = t->prev;
		t->prev->next = ts;
		t->prev = ts;
		//		tl->active = ts;
	}
	//	printf ("<-- put task\n");
}




void delete_task(struct task_struct* ts)
{
	printf ("<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<Time%3d DEL [id:%3d] [%3d~%3d]",global_time, ts->task_id, ts->starttime, ts->deadline);
	switch (ts->type) {
		case 0:
			printf (" NML");
			break;
		case 1:
			printf (" SRT");
			break;
		case 2:
			printf (" HRT");
			break;
	}
	if (ts->deadline < global_time) {
		printf (" MISS \n");
		switch (ts->type) {
			case 0:
				nmlmt++;
				break;
			case 1:
				sfrtmt++;
				break;
			case 2:
				hdrtmt++;
				break;
		}
	} else {
		printf (" PASS \n");
	}
	assert(ts != NULL);
	struct task_list* tl = &tasklist[ts->priority];
	struct task_struct* t = tl->active;
	tl->active_num--;
	if (tl->active_num == 0) {
		tl->active = NULL;
		delete(ts);
		//		printf ("No left.");
		return;
	}
	ts->prev->next = ts->next;
	ts->next->prev = ts->prev;
	if (t == ts) {
		tl->active = ts->next;
	}
	delete(ts);
	return;
}



/*
 * THis is the main process for schedule
 * It checks the tasks which is ready to run
 * and pick up the one who has the most priority
 * And then give it some time to run.
 */
struct task_struct* pick_up_task()
{
	int i = 0;
	while (i < 140) {
		if (tasklist[i].active_num > 0) {
			break;
		}
		i++;
	}
	if (i == 140) {
		return NULL;
	}
	return tasklist[i].active;
}



int runtime(struct task_struct* ts)
{
	int t = ts->runtime;
	int i;
	if (ts->priority > 100) {
		i = (150 - ts->priority) / 10;
	} else {
		i = (100 - ts->priority) / 5 * ts->last_run_time_ratio * 1.5;
	}
	int j = continue_consume_time();
	if (i < t)
		t = i;
	if (j < t)
		t = j;
	if (t == 0) {
		t = 1;
	}
	return t;
}



void relax()
{
	//	printf ("--%dRelax, due to keep energy\n", global_time);
	printf ("%4d\t%4dR\t\t%4d\t\t----\n", global_time, 0, 1);
	//	printf ("%4d\t%4d\n", global_time, 0);
	keep_battery(0, 0, 1);
}



void schedule()
{
	struct task_struct* ts = pick_up_task();
	if (ts == NULL) {
		//		printf ("No task available at time %d\n", global_time);
		//		printf ("RN%dRelax, due to no tasks available\n", global_time);
		//		printf ("N%d\t%4d\t%d\n", 0, global_time, 1);
		printf ("%4d\t%4dN\t\t%4d\t\t----\n", global_time, 0, 1);
		keep_battery(0, 0, 1);	
		return;
	}
	int time = runtime(ts);
	if (time == -2) {
		/* only hard real time task can run */
		if (ts->type != 2) {
			relax();
			return;
		}
	}
	if (time == -1) {
		/* only real time tasks can run*/
		if (ts->type == 0) {
			relax();
			return;
		}
	}
	if (time == 0) {
		relax();
		return;
		//		printf ("!!BUG!!Reschedule!\n");
		//		schedule();
	}
	if (time < 0) {
		time = 1;
	}
	if (ts->type == 0) {
		/* As normal task makes contribution to energy,
		 * We give them a bonus*/
		time *= 2;
		if (time > ts->runtime) 
			time = ts->runtime;
		keep_battery(0, time, 0);
		printf ("%4d\t%4dB\t\t%4d\t\t%4d\n", global_time, 1, time, ts->task_id);
		//		int t = global_time;
		//		int t1 = time;
		//		while (--t1)
		//			printf ("%4d\t%4d\n", ++t, 1);
	} else {
		keep_battery(time, 0, 0);
		printf ("%4d\t%4dB\t\t%4d\t\t%4d\n", global_time, 2, time, ts->task_id);
		//		int t = global_time;
		//		int t1 = time;
		//		while (--t1)
		//			printf ("%4d\t%4d\n", ++t, 2);
	}
	ts->runtime -= time;

	/* 	CurTime		Task_id		Runtime		TimeLeft	Finished */
	//	printf ("B\t%4d\t%d\n", global_time, time);
	//	printf ("%4d\t%4d\n", global_time, 1);
	//	printf ("%d\t%4d\t%d\tB\n", 1, global_time, time);
	//	int t = global_time;
	//	int t1 = time;
	//	while (--t1)
	//		printf ("%4d\t%4d\n", ++t, 1);
	/*	printf ("%d\t\t\t%d", global_time, ts->task_id);
		ts->runtime -= time;
		switch (ts->type) {
		case 2:
		printf (" HRT\t\t");
		break;
		case 1:
		printf (" SRT\t\t");
		break;
		case 0:
		printf (" NML\t\t");
		break;
		}
		printf ("%d\t\t\t%d\t\t\t\t", time, ts->runtime);
		*/	if (ts->runtime == 0) {
			//		printf ("YES!");
			delete_task(ts);
			//		printf ("\n");
			//	} else {
			//		printf ("No!\n");
}
if (time > 1) {
	global_time += time - 1;
}
}


#endif
