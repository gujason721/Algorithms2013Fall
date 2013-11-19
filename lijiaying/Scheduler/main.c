#include <stdio.h>
#include <stdlib.h>
#include "task.h"
#include "schedule.h"

#include <assert.h>


struct task_struct* previous = NULL;
int taskid = 0;


int input_task()
{
	int num = 0;
	//	printf ("-->input task <globaltime:%d>\n", global_time);
	if (previous != NULL) {
		if (previous->starttime <= global_time) {
			num++;
			put_task(previous);
			printf (">>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>Time%3d ADD [id:%3d] [%3d~%3d]", 
					global_time, previous->task_id, previous->starttime, previous->deadline);
			switch (previous->type) {
				case 2:
					printf (" HRT\n");
					break;
				case 1:
					printf (" SRT\n");
					break;
				case 0:
					printf (" NML\n");
					break;
			}
			previous = NULL;
		} else {
			return num;
		}
	}
	struct task_struct* input_task;
	int type, starttime, deadline, runtime;
	while (true) {
		input_task = (struct task_struct*) malloc (sizeof (struct task_struct));
		if (scanf ("type:%d starttime:%d deadline:%d runtime:%d\n", &type, &starttime, &deadline, &runtime) == EOF) {
			//			printf ("\n");
			return -1;
		}
		input_task->task_id = taskid++;
		input_task->type = type;
		input_task->starttime = starttime;
		input_task->deadline = deadline;
		input_task->runtime = runtime;

		switch (input_task->type) {
			case 0:
				nml++;
				break;
			case 1:
				sfrt++;
				break;
			case 2:
				hdrt++;
				break;
		}
		

		if (global_time >= input_task->starttime) {
			num++;
			put_task(input_task);
			printf (">>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>Time%3d ADD [id:%3d] [%3d~%3d]", 
					global_time, input_task->task_id, input_task->starttime, input_task->deadline);
			switch (input_task->type) {
				case 2:
					printf (" HRT\n");
					break;
				case 1:
					printf (" SRT\n");
					break;
				case 0:
					printf (" NML\n");
					break;
			}
		}
		else {
			previous = input_task;
			return num;
		}
	}
	printf ("\n");
	return num;
}


void initialize_tasks()
{
	input_task();
	return;
}




int main()
{
	global_time = 0;
	initialize_battery();
	initialize_task_list();
	initialize_tasks();
	while (true) {
		schedule();
		global_time++;
		//		printf ("%d\t", global_time);
		input_task();
		//		if (input_task() < 0) {
		//			printf ("No additional tasks at time %d!\n", global_time);
		//		}
		if (global_time > 1500) 
			break;
	}
	printf ("\n-------------------------------\n");
	printf ("HDRT : %d miss %d ratio %f\n", hdrt, hdrtmt, (double)hdrtmt/hdrt);
	printf ("SFRT : %d miss %d ratio %f\n", sfrt, sfrtmt, (double)sfrtmt/sfrt);
	printf ("NOML : %d miss %d ratio %f\n", nml, nmlmt, (double)nmlmt/nml);
	return 0;
}

