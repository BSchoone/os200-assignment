#ifndef SCHEDUALS_H
#define SCHEDUALS_H

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <limits.h>
#include <pthread.h>
#include "functions.h"
#include "LinkedList.h"

typedef struct OutputBuffer{char j; double waitTime; double turnTime;} OutputBuffer;

pthread_cond_t inputCond;
pthread_cond_t outputCond;
pthread_cond_t countCond;
pthread_mutex_t inputMutex;
pthread_mutex_t outputMutex;
pthread_mutex_t countMutex;

int inputFlag, outputFlag,threadReadyFlag, quit;
char inputBuffer[11];
OutputBuffer outputBuffer;

void *roundRobin();
void *shortetJobFirst();

#endif
