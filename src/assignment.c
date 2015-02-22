#include "assignment.h"
int main(int argc, char const *argv[])
{
	pthread_t thread1, thread2;
	int rrThread, sjfThread;

	inputFlag = 0;threadReadyFlag = 0;outputFlag = 0, quit=0;
	
	pthread_cond_init(&inputCond,NULL);
	pthread_cond_init(&outputCond, NULL);
	pthread_cond_init(&countCond, NULL);
	pthread_mutex_init(&inputMutex, NULL);
	pthread_mutex_init(&outputMutex, NULL);
	pthread_mutex_init(&countMutex, NULL);
		
	rrThread = pthread_create( &thread1, NULL, roundRobin, NULL);
    sjfThread = pthread_create( &thread2, NULL, shortetJobFirst, NULL);


	/*****************************************
	WAIT UNTIL THREADS HAVE BEEN CREATED AND 
	STARTED UP
	*****************************************/
	pthread_mutex_lock(&countMutex);
	while(threadReadyFlag != 2)
	{
		pthread_cond_wait(&countCond, &countMutex);
	}
	threadReadyFlag = 0;
	pthread_mutex_unlock(&countMutex);

	/****************************************/
	printf("~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~\n");
	printf("  Welcome to Schedual Simulator 1998: GoTY Edition\n");
	printf("~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~\n");
	while(quit != 1)
	{
		/*****************************************
		LOCK THE INPUT AND HAVE THE USER INPUT IT
		*****************************************/
		pthread_mutex_lock(&inputMutex);
		do{
		printf("Input File Name:");
		scanf("%10s", inputBuffer);
		}while(!file_exist(inputBuffer) && !(strcmp(inputBuffer, "QUIT") == 0));
		
		inputFlag = 1;
		pthread_cond_broadcast(&inputCond);
		pthread_mutex_unlock(&inputMutex);
		/****************************************/
		if( !(strcmp(inputBuffer, "QUIT") == 0) )
		{
			/*****************************************
			WAIT FOR OUTPUT TO BE FILLED, THEN LOCK
			AND REMOVE THE DATA
			*****************************************/
			printf("/****************************************/\n");
			for(int ii=0; ii < 2; ii++)
			{
				//printf("For %d\n", ii);
				pthread_mutex_lock(&outputMutex);
				while(outputFlag != 1)
				{
					pthread_cond_wait(&outputCond, &outputMutex);
				}
				
				if(outputBuffer.j == 'S')
					printf("\tJob: Shortest Job First\n");
				else
					printf("\tJob: Round Robin\n");
				printf("\tAverage Wait Time: %.2f\n", outputBuffer.waitTime);
				printf("\tAverage Turn Time: %.2f\n", outputBuffer.turnTime);
				printf("/****************************************/\n");
				outputFlag = 0;
				pthread_cond_broadcast(&outputCond);
				pthread_mutex_unlock(&outputMutex);
			}
			/****************************************/
		 }
	 	else
	 	{
	 		quit = 1;
	 	}
	}
	pthread_join(thread1, NULL);
	pthread_join(thread2,NULL);
	return 0;
}
