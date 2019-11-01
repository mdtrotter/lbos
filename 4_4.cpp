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

//init sems and muts
sem_t sems[5];
pthread_mutex_t mutex1;
pthread_mutex_t mutex2;

void think(long tid)
{

	printf("philospher %d is thinking\n", tid);
	sleep(1);

}

void put_forks(long tid)
{

	pthread_mutex_lock(&mutex1);
	std::cout << "philosopher " << tid << " has placed down right fork " << tid << std::endl;
	sem_post(&sems[tid]);
	pthread_mutex_unlock(&mutex1);

	pthread_mutex_lock(&mutex1);
	std::cout << "philosopher " << tid << " has placed down left fork " << (tid+1)%5 << std:: endl;
	sem_post(&sems[(tid+1)%5]);
	pthread_mutex_unlock(&mutex1);

}

void eat(long tid)
{

	printf("philosopher %d is eating\n", tid);
	sleep(1);
	put_forks(tid);

}

void get_forks(long tid)
{
	sem_wait(&sems[tid]);
	std::cout << "philosopher " << tid << " has grabbed right fork " << tid << std::endl;
	sem_wait(&sems[(tid+1)%5]);
	std::cout << "philosopher " << tid << " has grabbed left fork " << ((tid+1)%5) << std::endl;
	pthread_mutex_unlock(&mutex1);
	eat(tid);

}

void check_forks(long tid)
{

	int valLeft, valRight;
	pthread_mutex_lock(&mutex1);
	sem_getvalue(&sems[tid], &valLeft);
	sem_getvalue(&sems[((tid+1)%5)], &valRight);
	if(valLeft == 1 && valRight == 1)
	{
	
		get_forks(tid);
	
	}else{
		if(valLeft == 0){
			std::cout << "philosopher " << tid << " needs fork " << tid << std::endl;
		}if(valRight == 0){
			std::cout << "philosopher " << tid << " needs fork " << (tid+1)%5 << std::endl;
		}
		std::cout << "philosopher " << tid << " will go hungry"<< std::endl;
		pthread_mutex_unlock(&mutex1);
	
	}

}

//main thread function
void *philFunc(void *threadid)
{
	for(int i=0; i<100; i++){
		long tid = (long)threadid;
		think(tid);
		check_forks(tid);
	}

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
