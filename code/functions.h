#ifndef FUNCTIONS_H
#define FUNCTIONS_H

#include <stdio.h>
#include <sys/stat.h>
#include <sys/types.h>
#include "LinkedList.h"

void calcProcesses(char*, int*);
void bubbleSort(int, int[], int[]);
int file_exist (char*);
void loadLinkedList(char*, LinkedList*, int, int *, int*);
void loadFile(char*, int[], int[], int*, int*);

#endif
