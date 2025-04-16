#include <stdio.h> //For normal I/O operations
#include <unistd.h> //For standard os operations like fork, sleep,etc
#include <pthread.h> //For handling Threads
#include <semaphore.h> //For semaphores
#include <stdlib.h> //Important funcitons

//Define constants
#define READER_COUNT 3
#define WRITER_COUNT 2

// Formatting Options
#define RESET       "\033[0m"
#define BOLD        "\033[1m"
#define UNDERLINE   "\033[4m"

// Foreground Colors
#define BLACK       "\033[30m"
#define RED         "\033[31m"
#define GREEN       "\033[32m"
#define YELLOW      "\033[33m"
#define BLUE        "\033[34m"
#define MAGENTA     "\033[35m"
#define CYAN        "\033[36m"
#define WHITE       "\033[37m"

// Background Colors
#define BG_BLACK    "\033[40m"
#define BG_RED      "\033[41m"
#define BG_GREEN    "\033[42m"
#define BG_YELLOW   "\033[43m"
#define BG_BLUE     "\033[44m"
#define BG_MAGENTA  "\033[45m"
#define BG_CYAN     "\033[46m"
#define BG_WHITE    "\033[47m"

//Global Variable
char data[1024]; 
unsigned int reader_count =0;
//Define Semaphores
sem_t reader_count_sem;
sem_t writer_sem;

//Writer job
void* writer(void* args)
{
	unsigned int id = *((unsigned int*)args);
	//While true
	while(1)
	{
		sem_wait(&writer_sem);

		//The Writing Process
		printf(BOLD GREEN "[INFO::<%d>] WRITING MESSAGE\n", id);
		sprintf(data, "Message from writer: (%s: %d)", "I am writer", rand() );
		sleep(1);
		sem_post(&writer_sem);
		sleep(1);
	}
}

//Reader Job
void* reader(void* args)
{
	unsigned int id = *((unsigned int*)args);
	//While true
	while(1)
	{
		sem_wait(&reader_count_sem);
		reader_count++;

		if(reader_count == 1)
		{
			//This is first reader so block writer (READER_PRIORITY SCENARIO)
			sem_wait(&writer_sem);
		}

		sem_post(&reader_count_sem);

		//Start reading
		printf(RED"[INFO::<%d>] Read Message -> [%s] \n", id,data);
		sleep(1);
		sem_wait(&reader_count_sem);

		reader_count--;

		if(reader_count == 0){
			//After the last reader gets out, the writer is able to write again (READER_PRIORITY SCENARIO)
			sem_post(&writer_sem);
		}
		sem_post(&reader_count_sem);
		sleep(1);
	}
}
int main()
{
	//Thread arrays
	pthread_t reader_threads[READER_COUNT];
	pthread_t writer_threads[WRITER_COUNT];
	
	//Initialize the semaphores
	sem_init(&reader_count_sem, 0, 1);
	sem_init(&writer_sem, 0, 1);
	
	//IDS
	unsigned int rids[READER_COUNT];
	unsigned int wids[WRITER_COUNT];
	//Initialize threads
	for(unsigned int i =0; i < READER_COUNT; i++)
	{
		rids[i] =i;
		pthread_create(&reader_threads[i], NULL, reader, &rids[i]);
	}
	
	for(unsigned int i =0; i < WRITER_COUNT; i++)
	{
		wids[i] =i;
		pthread_create(&writer_threads[i], NULL, writer, &rids[i]);
	}

	//join threads
	for(unsigned int i =0; i < READER_COUNT; i++)
	{
		pthread_join(reader_threads[i], NULL);
	}
	
	for(unsigned int i =0; i < WRITER_COUNT; i++)
	{
		pthread_join(writer_threads[i], NULL);
	}

	return 0;
}
