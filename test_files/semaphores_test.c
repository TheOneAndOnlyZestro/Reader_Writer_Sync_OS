#include <stdio.h>
#include <pthread.h>
#include <semaphore.h>
#include <unistd.h>
#define THREAD_NUM 5

static sem_t A_Done, B_Done, C_Done, D_Done;

void* jobA(void * args)
{
	printf("Running Thread A...............\n");
	sleep(1);
	
	//Finish the execution
	sem_post(&A_Done);
	sem_post(&A_Done);
	sem_post(&A_Done);
}

void* jobB(void* args)
{
	sem_wait(&A_Done);
	sleep(0.1);
	printf("Running Thread B...............\n");
	sleep(1);
	sem_post(&B_Done);
}
void* jobC(void* args)
{
	sem_wait(&A_Done);
	printf("Running Thread C...............\n");
	sleep(1);
	sem_post(&C_Done);

}
void* jobD(void* args)
{
	sem_wait(&A_Done);
	printf("Running Thread D...............\n");
	sleep(1);
	sem_post(&D_Done);

}

void* jobE(void* args)
{
	sem_wait(&D_Done);

	printf("Running Thread E...............\n");

}



int main()
{
	//Initialize Semaphore
	sem_init(&A_Done, 0,0);
	sem_init(&B_Done, 0,0);
	sem_init(&C_Done, 0,0);
	sem_init(&D_Done, 0,0);
	
	pthread_t thread_id[THREAD_NUM];
	

	pthread_create(&thread_id[0], NULL, jobA, NULL);
	pthread_create(&thread_id[1], NULL, jobB, NULL);
	pthread_create(&thread_id[2], NULL, jobC, NULL);
	pthread_create(&thread_id[3], NULL, jobD, NULL);
	pthread_create(&thread_id[4], NULL, jobE, NULL);


	pthread_join(thread_id[0], NULL);
	pthread_join(thread_id[1], NULL);
	pthread_join(thread_id[2], NULL);
	pthread_join(thread_id[3], NULL);
	pthread_join(thread_id[4], NULL);
	return 0;
}
