/* Matthew Trotter
   Santa Claus Problem
   Elves can only wake Santa when three elves have problems
   After, if any other elves have problems, must wait for one to return
   If three elves have problem and 
 */

#include <pthread.h>
#include <semaphore.h>
#include <iostream>
#include <cstdio>
#include <cstdlib>
#include <unistd.h>
#include <stdlib.h>

using namespace std;

int elves = 0;

int reindeer = 0;

//init sems and muts
sem_t santaSem;
sem_t deerSem;
pthread_mutex_t elfTex;
pthread_mutex_t mutex;

void getHitched(long tid)
{

	pthread_mutex_lock(&mutex);
	cout << "deer " << tid << " is getting hitched" << endl;
	pthread_mutex_unlock(&mutex);
}

void prepareSleigh()
{
	pthread_mutex_lock(&mutex);
	cout << "Santa is preparing the sleigh" << endl;
	pthread_mutex_unlock(&mutex);
}

void getHelp(long tid)
{
	pthread_mutex_lock(&mutex);
	cout << "elf " << tid << " is getting help" << endl;
	pthread_mutex_unlock(&mutex);
}

void *deerFunc(void *threadid)
{
	long tid = (long)threadid;
	int randNum = rand() % 100 + 1;
	while(!(randNum >= 85))
	{
		sleep(1);
		randNum = rand() % 100 + 1;
	}

	pthread_mutex_lock(&mutex);
	cout << "deer " << tid << " has returned from the tropics" << endl;
	reindeer++;
	if(reindeer == 9)
	{
		sem_post(&santaSem);
	}
	pthread_mutex_unlock(&mutex);

	sem_wait(&deerSem);

	getHitched(tid);

	pthread_exit(NULL);
}

void helpElves()
{
	pthread_mutex_lock(&mutex);
	cout << "Santa is helping the elves" << endl;
	pthread_mutex_unlock(&mutex);
}

//elf thread function
void *elfFunc(void *threadid)
{
	long tid = (long)threadid;
	int randNum = rand() % 100 + 1;
	while(!(randNum >= 50))
	{
		sleep(1);
		randNum = rand() % 100 + 1;
	}
	pthread_mutex_lock(&elfTex);
	pthread_mutex_lock(&mutex);
	elves++;
	if(elves == 3)
	{
		sem_post(&santaSem);
	}
	else
	{
		pthread_mutex_unlock(&elfTex);
	}
	pthread_mutex_unlock(&mutex);

	getHelp(tid);

	pthread_mutex_lock(&mutex);
		elves--;
		if (elves == 0)
		{
			pthread_mutex_unlock(&elfTex);
		}
	pthread_mutex_unlock(&mutex);
	
	pthread_exit(NULL);
}

void *santaFunc(void *threadid)
{
	long tid = (long) threadid;
	int i;

	sem_wait(&santaSem);
	pthread_mutex_lock(&mutex);
	while(1 == 1){
	
		if(reindeer == 9){
			prepareSleigh();
			for(i=0; i<9; i++){
				sem_post(&deerSem);
			}
			pthread_exit(NULL);
		}else{
			if(elves == 3){
				helpElves();
			}
		}
		pthread_mutex_unlock(&mutex);
	}
}

int main(int argc, char *argv[])
{
	int rc;
	int numElves = 120, numDeer = 9, numSanta = 1;
	long et, dt, st=0;

	//init elf and deer threads
	pthread_t elfThreads[numElves];
	pthread_t deerThreads[numDeer];
	pthread_t santaThread[numSanta];

	sem_init(&santaSem, 0, 1);
	sem_init(&deerSem, 0, 1);

	rc = pthread_create(&santaThread[st], NULL, santaFunc, (void *)st);
	if(rc)
	{
		printf("Error code is %d\n", rc);

		return 1;
	}

	for(et=0; et<numElves; et++)
	{
	
		rc = pthread_create(&elfThreads[et], NULL, elfFunc, (void *)et);
		if(rc)
		{
		
			printf("Error code is %d\n", rc);

			return 1;
		}
	
	}

	for(dt=0; dt<numDeer; dt++)
	{
	
		rc = pthread_create(&deerThreads[dt], NULL, deerFunc, (void *)dt);
		if(rc)
		{
		
			printf("Error code is %d\n", rc);

			return 1;
		
		}
	
	}

	pthread_join(santaThread[0], 0);

	for(et=0; et<numElves; et++)
	{
	
		pthread_join(elfThreads[et], 0);
	
	}
	for(dt=0; dt<numDeer; dt++)
	{
	
		pthread_join(deerThreads[dt], 0);
	
	}
	
	pthread_exit(NULL);
}
