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
pthread_mutex_t mutex;
pthread_mutex_t print;
sem_t multiplex;


void *childFunc(void *threadid)
{

	long tid = (long) threadid;

	pthread_mutex_lock(&print);
	cout << "Child " << tid << " is ready to be cared for" << endl;
	pthread_mutex_unlock(&print);
	sem_post(&multiplex);

	pthread_exit(NULL);

}

void *parentFunc(void *threadid)
{

	long tid = (long) threadid;

	pthread_mutex_lock(&mutex);
	sem_wait(&multiplex);
	sem_wait(&multiplex);
	sem_wait(&multiplex);
	pthread_mutex_lock(&print);
	cout << "Parent " << tid << " is caring for three children" << endl;
	pthread_mutex_unlock(&print);
	pthread_mutex_unlock(&mutex);

	pthread_exit(NULL);

}

int main(int argc, char *argv[])
{
	//300 children & 100 parents
	pthread_t childThread[300];
	pthread_t parentThread[100];

	sem_init(&multiplex, 0, 3);

	int rc;
	long c, p;
	
	for(c=0; c<300; c++)
	{
	
		rc = pthread_create(&childThread[c], NULL, childFunc, (void *)c);
		if(rc)
		{
		
			printf("Error code is %d\n", rc);

			return 1;
		}
	
	}

	for(p=0; p<100; p++)
	{
	
		rc = pthread_create(&parentThread[p], NULL, parentFunc, (void *)p);
		if(rc)
		{
		
			printf ("Error code is %d\n", rc);

			return 1;
		
		}
	
	}

	for(c=0; c<300; c++)
	{
	
		pthread_join(childThread[c], 0);
	
	}

	for(p=0; p<100; p++)
	{
	
		pthread_join(parentThread[p], 0);
	
	}
	
	pthread_exit(NULL);
}
