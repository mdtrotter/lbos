/* Matthew Trotter
 * Dining Philosophers Program
 * -Only one philosopher can hold a fork at a time
 * -It must be impossible for a deadlock to occur
 * -It must be impossible for a philosopher to starve waiting for a fork
 * -It must be possible for more than one philosopher to eat at the same time
 */

#include <pthread.h>
#include <semaphore.h>
#include <iostream>
#include <cstdio>
#include <cstdlib>
#include <unistd.h>

int elves = 12, reindeer = 9;

//init sems and muts
sem_t santaSem;
sem_t deerSem;
pthread_mutex_t elfTex;
pthread_mutex_t mutex1;

void *deerFunc(void *threadid)
{

	pthread_exit(NULL);

}

//elf thread function
void *elfFunc(void *threadid)
{
	pthread_exit(NULL);

}

int main(int argc, char *argv[])
{
	//init elf and deer threads
	pthread_t elfThreads[elves];
	pthread_t deerThreads[reindeer];

	int rc;
	long et, dt;
	
	for(et=0; et<elves; et++)
	{
	
		rc = pthread_create(&elfThreads[et], NULL, elfFunc, (void *)et);
		if(rc)
		{
		
			printf("Error code is %d\n", rc);

			return 1;
		}
	
	}

	for(dt=0; dt<reindeer; dt++)
	{
	
		rc = pthread_create(&deerThreads[dt], NULL, deerFunc, (void *)dt);
		if(rc)
		{
		
			printf("Error code is %d\n", rc);

			return 1;
		
		}
	
	}

	for(et=0; et<elves; et++)
	{
	
		pthread_join(elfThreads[et], 0);
	
	}
	for(dt=0; dt<reindeer; dt++)
	{
	
		pthread_join(deerThreads[dt], 0);
	
	}
	
	pthread_exit(NULL);
}
