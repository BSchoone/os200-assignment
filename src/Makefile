CFLAGS = -std=c99 -Wall -g
CC = gcc
OBJ = assignment.o scheduals.o LinkedList.o functions.o

assignment: $(OBJ)
	$(CC) -o assignment -pthread $(OBJ)
		
assignment.o: assignment.c assignment.h scheduals.h functions.h LinkedList.h
	$(CC) $(CFLAGS) -c assignment.c 
	
scheduals.o: scheduals.c scheduals.h functions.h LinkedList.h
	$(CC) $(CFLAGS) -c scheduals.c

functions.o: functions.c functions.h LinkedList.h
	$(CC) $(CFLAGS) -c functions.c
	
LinkedList.o: LinkedList.c LinkedList.h
	$(CC) $(CFLAGS) -c LinkedList.c
	 
clean: 
	rm -f assignment $(OBJ)
	rm -f *~
