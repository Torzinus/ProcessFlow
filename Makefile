all: processflow

processflow: main.o tasks.o
	gcc main.o tasks.o -o processflow

main.o: main.c tasks.h
	gcc -c main.c

tasks.o: tasks.c tasks.h
	gcc -c tasks.c

clean:
	rm -f *.o processflow

run: processflow
	./processflow