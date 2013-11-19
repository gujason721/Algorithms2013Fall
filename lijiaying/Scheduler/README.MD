Scheduler:
 Compile and run:
 	Just input the following in the shell:
		$ make
		$ make run



 Input comes from ./test/[filename]
	For the input file, there are many lines, 
	and the format in every line is like the following:
		type:0 starttime:0 deadline:68 runtime:6
	In the test folder, there are two sample file for test.
	You can generate your own input files and put them in test folder.

  Output wil be put ./output
  	There are four kind of lines in the output file	
  	1>
		6	   0N		   1		----
  		21	   1B		   3		   1
  		83	   0R		   1		----
	These show the state of the CPU,
	[global time]  [battery voltage][Busy/Relax/No tasks]	[duration]	[task id]

	2>
		>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>Time 72 ADD [id: 20] [ 71~152] NML
		<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<Time 72 DEL [id: 14] [ 59~125] SRT PASS 
	These lines show the event that tasks come in or terminate.
	The string in these line can explain themselves...

	3>
		-------------------------------
		HDRT : 11 miss 0 ratio 0.000000
		SFRT : 31 miss 12 ratio 0.387097
		NOML : 177 miss 101 ratio 0.570621
	These rows are statistical information about our scheduler.

Recommend setting:
	GCC 4.6.*
	Architecture x64
