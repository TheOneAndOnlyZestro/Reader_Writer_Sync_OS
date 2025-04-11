#include <pthread.h>
#include <stdio.h>
#include <unistd.h>

#define THREAD_NUMBER 5

//Creating a global counter
static int count =0;

//Create mutex locks for threads
pthread_mutex_t m = PTHREAD_MUTEX_INITIALIZER;
pthread_cond_t c = PTHREAD_COND_INITIALIZER;

void* threadExample(void *args)
{
	unsigned int thread_id = *((unsigned int *)(args));

	switch(thread_id)
	{
		case 2:
			//We are dealing with thread 0
			//DECREMETNER: CONDITION-> count must be > 20 before decrementing
			//WE WILL USE CV

			pthread_mutex_lock(&m);
			while(count <= 20){
				//Then we want to wait for signal
				printf("WAITING.............\n");
				pthread_cond_wait(&c, &m);
			}
			count-=20;
			printf("COUNT -=20, COUNT=%d \n", count);
			pthread_mutex_unlock(&m);
			break;
		default:
			//We are dealing with thread 0
			//INCREMENTER
			pthread_mutex_lock(&m);
			count += 10;
			printf("COUNT+=10, COUNT=%d \n", count);
			pthread_mutex_unlock(&m);
			pthread_cond_signal(&c);
			sleep(1);
			break;

	}
}
int main()
{
	pthread_t pt[THREAD_NUMBER];
	
	pthread_mutex_init(&m, NULL);
	pthread_cond_init(&c, NULL);
	
	unsigned int thread_ids[THREAD_NUMBER];
	for(unsigned int i =0; i < THREAD_NUMBER;i++)
	{
		thread_ids[i] =i;
		if(pthread_create(&pt[i], NULL, threadExample, (void *)(&thread_ids[i]) ) != 0)
		{
			perror("Failed to Create Thread");
		}
	}
	for(unsigned int i =0; i < THREAD_NUMBER;i++)
	{
		pthread_join(pt[i], NULL);
	}

	pthread_mutex_destroy(&m);

	printf("Threads finished execution with count equal to: %d \n", count);

	return 0;

}
