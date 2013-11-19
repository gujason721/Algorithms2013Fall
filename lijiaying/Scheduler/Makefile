all:	SHD

.PHONY:
	run

SHD:	main.c schedule.h task.h energy.h
	g++ main.c -o SHD -g

clean:
	rm SHD output

run:
	./SHD < ./test/testcase > output
