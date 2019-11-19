/* Matthew Trotter
 * The search-insert-delete problem
 * Only one inserter can insert at a time
 * A searchers may occur during an insert
 * A delete can only occur if no insert or search is happening
 */

#include <pthread.h>
#include <semaphore.h>
#include <iostream>
#include <cstdio>
#include <cstdlib>
#include <unistd.h>

using namespace std;

//init sems and muts
pthread_mutex_t insertMutex;
pthread_mutex_t noSearcher;
pthread_mutex_t noInserter;
pthread_mutex_t searchSwitch;
pthread_mutex_t insertSwitch;

//mutex for cout ops
pthread_mutex_t print;

//singly-linked list
struct Node {
	int data;
	struct Node *next;

};

struct Node *head = NULL, *ptr = head;
int length=0;

void insert(int new_data)
{

	struct Node* new_node = (struct Node*) malloc(sizeof(struct Node));
	new_node->data = new_data;
	new_node->next = head;
	head = new_node;
	ptr = head;
	length++;

}

void display()
{

	struct Node* ptr;
	ptr = head;
	while (ptr != NULL){
		cout << ptr->data << " ";
		ptr = ptr->next;
	}
	ptr = head;

}

void delFunc(int value)
{
	int i;
	
	for(i=0; i<length-1; i++){
	
		if(ptr->next->data == value)
		{
			ptr->next = ptr->next->next;
			cout << value << " deleted" << endl;
			i = length;
		}
		ptr = ptr->next;

	}
	ptr = head;

}

void *searchFunc(void *threadid)
{

	long tid = (long) threadid;
	int val, i;

	val = rand() % 10;
	
	pthread_mutex_lock(&noSearcher);
	pthread_mutex_lock(&print);
	cout << "Searcher " << tid << " is searching for value " << val << endl;
	pthread_mutex_unlock(&print);
	for(i = 0; i<length; i++)
	{
		if(ptr->data == val)
		{
			pthread_mutex_lock(&print);
			cout << "Searcher " << tid << " found " << val << endl;
			pthread_mutex_unlock(&print);
			i = length;
		}
		if(ptr->next != NULL)
			ptr = ptr->next;
	
	}
	ptr = head;
	pthread_mutex_unlock(&noSearcher);

	pthread_exit(NULL);

}

void *insertFunc(void *threadid)
{

	long tid = (long) threadid;
	int val;

	val = rand() % 10;

	pthread_mutex_lock(&noInserter);
	pthread_mutex_lock(&insertMutex);
	pthread_mutex_lock(&print);
	cout << "Inserter " << tid << " is inserting value " << val << endl;
	pthread_mutex_unlock(&print);
	insert(val);
	pthread_mutex_unlock(&insertMutex);
	pthread_mutex_unlock(&noInserter);

	pthread_exit(NULL);

}

void *deleteFunc(void *threadid)
{
	
	long tid = (long) threadid;
	int val;

	val = rand() % 10;

	pthread_mutex_lock(&noSearcher);
	pthread_mutex_lock(&noInserter);
	pthread_mutex_lock(&print);
	cout << "Deleter " << tid << " is deleting node " << val << endl;
	delFunc(val);
	pthread_mutex_unlock(&print);
	pthread_mutex_unlock(&noInserter);
	pthread_mutex_unlock(&noSearcher);

	pthread_exit(NULL);


}

int main(int argc, char *argv[])
{
	//100 inserters, 100 searchers, 10 deletes
	pthread_t insThread[100];
	pthread_t searchThread[100];
	pthread_t deleteThread[10];

	int rc;
	long ins, ser, del;
	
	for(ins=0; ins<100; ins++)
	{
	
		rc = pthread_create(&insThread[ins], NULL, insertFunc, (void *)ins);
		if(rc)
		{
		
			printf("Error code is %d\n", rc);

			return 1;
		}
	
	}

	for(ser=0; ser<100; ser++)
	{
	
		rc = pthread_create(&searchThread[ser], NULL, searchFunc, (void *)ser);
		if(rc)
		{
		
			printf ("Error code is %d\n", rc);

			return 1;
		
		}
	
	}

	for(del=0; del<10; del++)
	{
	
		rc = pthread_create(&deleteThread[del], NULL, deleteFunc, (void *)del);
		if(rc)
		{
		
			printf ("Error code is %d\n", rc);

			return 1;
		
		}
	
	}

	for(ins=0; ins<100; ins++)
	{
	
		pthread_join(insThread[ins], 0);
	
	}

	for(ser=0; ser<100; ser++)
	{
	
		pthread_join(searchThread[ser], 0);
	
	}

	for(del=0; del<10; del++)
	{
	
		pthread_join(deleteThread[del], 0);
	
	}
	
	pthread_exit(NULL);
}
