#include "scheduals.h"

void *roundRobin(){
	int sumOfWait=0,sumOfTurnaround=0,earlyStart = INT_MAX;;
	int ii,time,remain,a,b,r,f, timeQuantum, numOfProcess;
	double avgWaitTime, avgTurnTime;
	char fileName[11];
	LinkedList* jobQueue, *processing, *finished;
	LinkedListNode* currJob, *checkedJob;
			
	jobQueue = createLinkedList();
	processing = createLinkedList();
	finished = createLinkedList();
	

	/*****************************************
	TELL THE PARENT THAT IT IS READY BY 
	INCREMENTING THE READYCOUNT
	*****************************************/
	pthread_mutex_lock(&countMutex);
	threadReadyFlag = threadReadyFlag + 1;
	pthread_cond_broadcast(&countCond);
	pthread_mutex_unlock(&countMutex);
	/****************************************/
	
	
	while(quit!=1)
	{
		/*****************************************
		WAIT UNTIL THE PARENT HAS INPUT
		*****************************************/
		pthread_mutex_lock(&inputMutex);
		while(inputFlag != 1)
		{
			pthread_cond_wait(&inputCond, &inputMutex);
		}
		snprintf(fileName, 11, "%s", inputBuffer);
		inputFlag=2;
		pthread_cond_broadcast(&inputCond);
		pthread_mutex_unlock(&inputMutex);
		/****************************************/	
	
		if(!(strcmp(fileName, "QUIT") == 0) )
		{
			calcProcesses(fileName,&numOfProcess);
			remain=numOfProcess;
			loadLinkedList(fileName, jobQueue, numOfProcess, &timeQuantum, &earlyStart);
			time=earlyStart;
			
			/*Check at the beginging to see if the finished queue contais all jobs*/
			while( getLength(finished) != numOfProcess) 
			{
				/*remove a job from processing, if it has no jobs, check jobQueue
					and remove it from there instead*/
				if(getLength(processing)!=0)
					currJob = removeFirstElement(processing);
				else if(getLength(jobQueue) != 0){
					currJob = removeFirstElement(jobQueue);
					time = currJob->a;
				}
				
				/* Decrement the burst time by timeQuantum, if it is now 0
					put it in the finished queue */
				if(currJob->r <= timeQuantum && currJob->r >= 0){
					time += currJob->r;
					currJob->r = 0;
					a = currJob->a;
					b = currJob->b;
					r = currJob->r;
					f = time;
					insertNodeEnd(finished, a,b,r,f,currJob->pid);
				}	
				else if(currJob->r > timeQuantum){
					currJob->r -= timeQuantum;
					time += timeQuantum;
				}		
				/*	else check the rest of
					the jobs in the job queue to see if any have come whilst
					it was running and put them onto the processing queue */
					
				for(int jj=1;jj<=getLength(jobQueue); jj++){
					checkedJob = removeFirstElement(jobQueue);
					if(checkedJob->a <= time){
						a = checkedJob->a;
						b = checkedJob->b;
						r = checkedJob->r;
						f = checkedJob->f;
						insertNodeEnd(processing, a,b,r,f,checkedJob->pid);
					}
					else{
						a = checkedJob->a;
						b = checkedJob->b;
						r = checkedJob->r;
						f = checkedJob->f;
						insertNodeEnd(jobQueue, a,b,r,f,checkedJob->pid);
					}
				}
				/*then put the job that was being processed onto the 
					back of the processing queue */
				if(currJob->r != 0){
					a = currJob->a;
					b = currJob->b;
					r = currJob->r;
					f = currJob->f;
					insertNodeEnd(processing, a,b,r,f,currJob->pid);
				}			
			}
			
			/* go through the list and calculate the sum of wait
				and turnaround time */
			for(ii=0; ii<numOfProcess; ii++)
			{
				currJob = removeFirstElement(finished);
				sumOfWait += currJob->f - currJob->b - currJob->a;
				sumOfTurnaround += currJob->f - currJob->a;
			}
			avgWaitTime = ((double)sumOfWait)/((double)numOfProcess);
			avgTurnTime = ((double)sumOfTurnaround)/((double)numOfProcess);
			sumOfTurnaround = 0;
			sumOfWait = 0;

			/*****************************************
			LOCK THE OUTPUT AND INSERT THAT INTO THE
			BUFFER
			*****************************************/
			pthread_mutex_lock(&outputMutex);

			while(outputFlag != 0)
			{
				pthread_cond_wait(&outputCond, &outputMutex);
			}
			outputBuffer.turnTime = avgTurnTime;
			outputBuffer.waitTime = avgWaitTime;
			outputBuffer.j = 'R';
			outputFlag = 1;
			pthread_cond_broadcast(&outputCond);
			pthread_mutex_unlock(&outputMutex);
			/****************************************/
		}
	}
	return NULL;
}

void *shortetJobFirst()
{
 	char fileName[11];
	int ii, sum_burstTime=0, sumOfTurnaround = 0, sumOfWait = 0,earlyStart = INT_MAX; 
	int  time, smallest,remain, timeQuantum, numOfProcess;
	double avgWaitTime, avgTurnTime;
	
	/*****************************************
	TELL THE PARENT THAT IT IS READY BY 
	INCREMENTING THE READYCOUNT
	*****************************************/
	pthread_mutex_lock(&countMutex);
	threadReadyFlag = threadReadyFlag + 1;
	pthread_cond_broadcast(&countCond);
	pthread_mutex_unlock(&countMutex);
	/****************************************/	
	while(quit!=1)
	{
		/*****************************************
		WAIT UNTIL THE PARENT HAS INPUT
		*****************************************/
		pthread_mutex_lock(&inputMutex);
		while(inputFlag != 2)
		{
			pthread_cond_wait(&inputCond, &inputMutex);
		}

		snprintf(fileName, 11, "%s", inputBuffer);
		inputFlag = 0;
		pthread_cond_broadcast(&inputCond);
		pthread_mutex_unlock(&inputMutex);
		/****************************************/	
			
		if( !(strcmp(inputBuffer, "QUIT") == 0) )
		{
			earlyStart = INT_MAX;
			
			calcProcesses(fileName,&numOfProcess);
			int arriveTime[numOfProcess+1], burstTime[numOfProcess+1], burstCopy[numOfProcess+1];	
			
			
			loadFile(fileName, arriveTime, burstTime, &timeQuantum, &earlyStart);
			bubbleSort(numOfProcess, arriveTime, burstTime);
		 	
		 	memcpy(burstCopy, burstTime, (numOfProcess+1)*sizeof(int));
		 	/* Set the last array value (not an actual job) to INT_MAX
		 		so that the loops can find the smallest value */
		 	burstCopy[numOfProcess]=INT_MAX;
		 	arriveTime[numOfProcess]=INT_MAX;
			
			time=earlyStart;
			remain = numOfProcess;
		  	
		  	while(remain > 0)
		  	{
				smallest=numOfProcess;
				/* Find the next job with the smallest remaining burst time */
				for(ii=0;ii<numOfProcess;ii++)
				{
			  		if(arriveTime[ii]<=time && burstCopy[ii]>0 && burstCopy[ii]<burstCopy[smallest])
						smallest=ii;
				}
				
				/*If no jobs came in whilst the other was running, pick the
					next one with the shortest arrival time */
				if(smallest == numOfProcess)
				{
					for(ii=0;ii<numOfProcess;ii++)
					{
				  		if(burstCopy[ii]>0 && arriveTime[ii]<arriveTime[smallest])
							smallest = ii;
					}
					time = arriveTime[smallest];
				}
				remain--;
				/* re-calculate the sum of WT and TT and make the burst time
					of the current job =0 */
				sumOfTurnaround+=time+burstCopy[smallest]-arriveTime[smallest];	
				sumOfWait+=time-arriveTime[smallest];
				time+=burstCopy[smallest];	
				burstCopy[smallest]=0;
		  	}
			
		  	avgWaitTime = sumOfWait*1.0/numOfProcess;
		 	avgTurnTime = sumOfTurnaround*1.0/numOfProcess;
			sumOfTurnaround = 0;
			sumOfWait = 0;
			sum_burstTime = 0;
			numOfProcess = 0;
			/*****************************************
			LOCK THE OUTPUT AND INSERT THAT INTO THE
			BUFFER
			*****************************************/
			pthread_mutex_lock(&outputMutex);

			while(outputFlag != 0)
			{
				pthread_cond_wait(&outputCond, &outputMutex);
			}

			outputBuffer.turnTime = avgTurnTime;
			outputBuffer.waitTime = avgWaitTime;
			outputBuffer.j = 'S';

			outputFlag = 1;
			pthread_cond_broadcast(&outputCond);
			pthread_mutex_unlock(&outputMutex);
			/****************************************/
		}
	}
	return NULL;
}
