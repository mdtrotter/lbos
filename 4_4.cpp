/* Matthew Trotter
 * Dining Philosophers Program
 * -Only one philosopher can hold a fork at a time
 * -It must be impossible for a deadlock to occur
 * -It must be impossible for a philosopher to starve waiting for a fork
 * -It must be possible for more than one philosopher to eat at the same time
 */

#include <pthread.h>
#include <semaphore.h>
#include <cstdio>
#include <cstdlib>
#include <unistd.h>

//init sems and muts
sem_t sems[5];
pthread_mutex_t mutex1;
pthread_mutex_t mutex2;

void think(long tid)
{

	printf("philospher %d is thinking\n", tid);

}

void get_forks(long tid)
{
	pthread_mutex_lock(&mutex1);
	sem_wait(&sems[tid]);
	printf("philosopher %d has grabbed fork %d\n", tid, tid);
	pthread_mutex_unlock(&mutex1);
	sleep(1);
	sem_wait(&sems[(tid+1)%5]);
	printf("philosopher %d has grabbed fork %d\n", tid, ((tid+1)%5));

}

void eat(long tid)
{

	printf("philosopher %d is eating\n", tid);

}

void put_forks(long tid)
{

	sem_post(&sems[tid]);
	printf("philosopher %d has placed down fork %d\n", tid, tid);
	sleep(1);
	sem_post(&sems[(tid+1)%5]);
	printf("philosopher %d has placed down fork %d\n", tid, ((tid+1)%5));

}

//main thread function
void *philFunc(void *threadid)
{

	long tid = (long)threadid;
	//think(tid);
	get_forks(tid);
	//eat(tid);
	put_forks(tid);

	pthread_exit(NULL);

}

int main(int argc, char *argv[])
{
	//five philosophers
	pthread_t threads[5];
	
	//five forks (represented as semaphores)
	for(int i=0; i<5; i++)
	{
	
		sem_init(&sems[i], 1, 1);
	
	}

	int rc;
	long t;
	
	for(t=0; t<5; t++)
	{
	
		rc = pthread_create(&threads[t], NULL, philFunc, (void *)t);
		if(rc)
		{
		
			printf("Error code is %d\n", rc);

			return 1;
		}
	
	}

	for(t=0; t<5; t++)
	{
	
		pthread_join(threads[t], 0);
	
	}
	
	pthread_exit(NULL);
}
