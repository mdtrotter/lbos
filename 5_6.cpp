/* Matthew Trotter
 * H20 problem
 * - Oxygen waits for two hydrogen
 */

#include <pthread.h>
#include <semaphore.h>
#include <iostream>
#include <cstdio>
#include <cstdlib>
#include <unistd.h>

using namespace std;

//init sems and muts
int oxygen = 0;
int hydrogen = 0;
pthread_mutex_t mutex;
pthread_barrier_t bar;
sem_t hydroSem;
sem_t oxySem;

//hydrogen thread function
void *hydroFunc(void *threadid)
{
	long tid = (long) threadid;
	int i;

	pthread_mutex_lock(&mutex);
	hydrogen++;
	if(hydrogen >= 2 && oxygen >= 1)
	{
	
		for(i=0; i<2; i++){
			sem_post(&hydroSem);
			hydrogen--;
		}

		sem_post(&oxySem);
		oxygen--;
	
	}else{
		pthread_mutex_unlock(&mutex);
	}
	sem_wait(&hydroSem);
	cout << "hydrogen " << tid << " is bonding" << endl;

	pthread_barrier_wait(&bar);

	pthread_exit(NULL);

}

//oxygen thread function
void *oxyFunc(void *threadid)
{
	long tid = (long) threadid;
	int i;

	pthread_mutex_lock(&mutex);
	oxygen++;
	if( hydrogen >= 2)
	{
	
		for(i = 0; i<2; i++)
		{
			sem_post(&hydroSem);
			hydrogen--;
		}

		sem_post(&oxySem);
		oxygen--;
	
	}else{
		pthread_mutex_unlock(&mutex);
	}

	sem_wait(&oxySem);
	cout << "oxygen " << tid << " is bonding" << endl;

	pthread_barrier_wait(&bar);

	pthread_mutex_unlock(&mutex);

	pthread_exit(NULL);

}

int main(int argc, char *argv[])
{
	//50 oxy / 100 hydro
	pthread_t oxyThread[50];
	pthread_t hydroThread[100];

	sem_init(&oxySem, 0, 1);
	sem_init(&hydroSem, 0, 2);

	pthread_barrier_init(&bar, NULL, 3);

	int rc;
	long oxy, hydro;
	
	for(oxy=0; oxy<50; oxy++)
	{
	
		rc = pthread_create(&oxyThread[oxy], NULL, oxyFunc, (void *)oxy);
		if(rc)
		{
		
			printf("Error code is %d\n", rc);

			return 1;
		}
	
	}

	for(hydro=0; hydro<100; hydro++)
	{
	
		rc = pthread_create(&hydroThread[hydro], NULL, hydroFunc, (void *)hydro);
		if(rc)
		{
		
			printf ("Error code is %d\n", rc);

			return 1;
		
		}
	
	}

	for(oxy=0; oxy<50; oxy++)
	{
	
		pthread_join(oxyThread[oxy], 0);
	
	}

	for(hydro=0; hydro<100; hydro++)
	{
	
		pthread_join(hydroThread[hydro], 0);
	
	}
	
	pthread_exit(NULL);
}
