#include <stdio.h>
#include <unistd.h>
#include <assert.h>
#include <pthread.h>
#include <stdlib.h>

#define MAX_SIZE 60
#define CYCLES 100

// ANSI color codes
#define RED     "\x1B[31m"
#define GREEN   "\x1B[32m"
#define YELLOW  "\x1B[33m"
#define BLUE    "\x1B[34m"
#define MAGENTA "\x1B[35m"
#define CYAN    "\x1B[36m"
#define WHITE   "\x1B[37m"
#define RESET   "\x1B[0m"

//Queue Global State
static char* data[MAX_SIZE];
static unsigned int head = 0;
static unsigned int tail = 0;
static unsigned int count = 0; 

//Mutex and CV values
static pthread_cond_t empty, fill;
static pthread_mutex_t m;
static int terminate = 0;
void push(char* value)
{
	if(tail == MAX_SIZE)
	{
		tail = 0;
	}

	data[tail] = value;
	tail++;
	count++;
}

char* pop()
{
	assert(data[head] != NULL);

	if(head == MAX_SIZE)
	{
		head = 0;
	}
	char* value = data[head];
	head++;
	count--;
	return value;
}

void* producer(void *args)
{
	//This is a producer who adds to a queue
	for(unsigned int i =0; i < CYCLES;i++){
		
		pthread_mutex_lock(&m);
		while(count == MAX_SIZE)
		{
			//To exit safely
			if(terminate == 1)
			{
				return NULL;
			}
			//we want to wait for signal
			pthread_cond_wait(&fill, &m);
			printf(GREEN"PRODUCER IS WAITING FOR CONSUMER TO CONSUME SOME \n"RESET);
			
		}
		//add element
		char* msg = (char*)malloc(sizeof(char) * 100);
		snprintf(msg,100, "MESSAGE: %d", i);

		push(msg);

		printf(GREEN"PRODUCER PRODUCED NEW MSG, MESSAGE=(%s) \n"RESET, msg);
		pthread_cond_signal(&empty);
		pthread_mutex_unlock(&m);
	}
}

void* consumer(void* args)
{
	pthread_mutex_lock(&m);
	while(count == 0)
	{
		//Consumer is Waiting for the queue to fill up
		pthread_cond_wait(&empty, &m);
		printf(RED"CONSUMER IS WAITING FOR PRODUCER TO PRODUCE SOME \n"RESET);
	}

	//remove element
	char* msg = pop();
	printf(RED"MESSAGE CONSUMED, MESSAGE=(%s) \n"RESET, msg);
	
	free(msg);

	pthread_cond_signal(&fill);
	pthread_mutex_unlock(&m);
}
int main(int argc, char** argv)
{
	pthread_mutex_init(&m, NULL);
	pthread_cond_init(&empty, NULL);
	pthread_cond_init(&fill, NULL);

	pthread_t prod;
	
	int thread_count;
	
	assert(argc == 2);
	assert(argv[1] != NULL);

	sscanf(argv[1], "%d", &thread_count);
	pthread_t *cons = (pthread_t *)malloc(sizeof(pthread_t) * thread_count);

	pthread_create(&prod, NULL, producer, NULL);

	for(unsigned int i =0; i < thread_count;i++)
	{
		pthread_create(&cons[i], NULL, consumer, NULL);
	}


	for(unsigned int i=0; i < thread_count;i++)
	{
		pthread_join(cons[i], NULL);
	}
	
	terminate =1;
	pthread_cond_broadcast(&fill);

	pthread_join(prod, NULL);
	//Clean up
	pthread_mutex_destroy(&m);
	pthread_cond_destroy(&empty);
	pthread_cond_destroy(&fill);

}
