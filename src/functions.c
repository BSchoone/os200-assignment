#include "functions.h"

void calcProcesses(char* name, int* numOfProcess){
	FILE *read = fopen(name, "r");
	int ii =0, x;

	fscanf(read, "%d", &x);	
	while (fscanf(read, "%d", &x) == 1){
		ii++;
	}
	fclose(read);
	*numOfProcess = (ii/2);
}
/***********************************************/
/*		A stable sort algorithm used to sort 
		the data before putting it into 
		a linkedlist							*/
/***********************************************/
void bubbleSort(int numOfProcess, int arriveTime[], int burstTime[])
{
	int length = numOfProcess - 1, sorted = 0, pass = 0, temp;

	while(sorted != 1){
		sorted = 1;
		for(int ii=0;ii<(length-pass);ii++)
		{
			if( arriveTime[ii] > arriveTime[ii+1])
			{
				temp = arriveTime[ii];
				arriveTime[ii] = arriveTime[ii+1];
				arriveTime[ii+1]=temp;
				
				temp = burstTime[ii];
				burstTime[ii] = burstTime[ii+1];
				burstTime[ii+1]=temp;
				sorted = 0;
			}
		}
		pass++;
	}
}

/***********************************************/
/*	Takes the sorted data and load it into
	the a linkedlist						   */
/***********************************************/

void loadLinkedList(char* name, LinkedList* jobQueue, int numOfProcess, int *timeQuantum, int* earlyStart)
{
	
	int arriveTime[numOfProcess], burstTime[numOfProcess];
	int a,b,r,f, ii=0;
	char pid;
	
	FILE *read = fopen(name, "r");
	int x, y, jj=0;
	*earlyStart = 999999;
	fscanf(read, "%d", timeQuantum);	
	while (fscanf(read, "%d %d", &x, &y) == 2){
		arriveTime[jj]=x;
		burstTime[jj]=y;
		
		if(x <= *earlyStart)
			*earlyStart=x;
		jj++;
	}
	fclose(read);
	
	bubbleSort(numOfProcess, arriveTime, burstTime);
	
	
	for(ii=0;ii<numOfProcess;ii++)
	{
		a = arriveTime[ii];
		b = burstTime[ii];
		r = b;
		f = 0;
		pid = 'A'+ ii;
		insertNodeEnd(jobQueue, a, b, r, f, pid);
	}
}

void loadFile(char* name, int arriveTime[], int burstTime[], int *timeQuantum, int* earlyStart)
{
	FILE *read = fopen(name, "r");
	int x=999999, y, jj=0;

	fscanf(read, "%d", timeQuantum);	
	while (fscanf(read, "%d %d", &x, &y) == 2){
		arriveTime[jj]=x;
		burstTime[jj]=y;
		
		if(x <= *earlyStart)
			*earlyStart=x;
		jj++;
	}
	fclose(read);
}

/***********************************************/
/*	Returns 1 or 0 depending on if the
	file exists or not						  */
/***********************************************/ 
int file_exist (char *filename)
{
  struct stat buffer;   
  return (stat(filename, &buffer) == 0);
}
