#include <stdio.h>
#include <pthread.h>
#include <stdlib.h>
#include <semaphore.h>
#include <gem5/m5ops.h>
#define N_TIMES 100
#define N_INCREM 3

sem_t my_sem;
pthread_spinlock_t sp_lock;
unsigned int shared_value = 0;
int start = 0;
int change = 0;

void *func(void *par)
{
	int increm = *((int *)par);
	unsigned int old_val = 0;
	int local_start = 0;
	
	while(!local_start)
	{
		pthread_spin_lock(&sp_lock);
		if(start!=0 && start!=increm)
			local_start=1;
		start = increm;
		pthread_spin_unlock(&sp_lock);
	}
	
	for(int i=0; i<N_TIMES; i++)
	{
		sem_wait(&my_sem);
		if(shared_value!=old_val)
			change++;
		for(int j=0; j<N_INCREM; j++)
		{
			shared_value = shared_value + increm;
		}
		old_val=shared_value;
//		shared_value += increm;
		sem_post(&my_sem);
	}
	return NULL;
}

int main()
{
	pthread_t thread_1, thread_2;
	int param_1= 1;
	int param_2= -1;
	
	printf("Testing semaphore...\n");
	if(sem_init(&my_sem, 0, 1)!=0 || pthread_spin_init(&sp_lock, PTHREAD_PROCESS_PRIVATE)!=0)
	{
		printf("ERROR in initializing!\n");
		return -1;
	}
	m5_switch_cpu();
	pthread_create(&thread_1, NULL, func, (void *)&param_1);
	pthread_create(&thread_2, NULL, func, (void *)&param_2);
	
	pthread_join(thread_1, NULL);
	pthread_join(thread_2, NULL);
	if(sem_destroy(&my_sem)!=0 || pthread_spin_destroy(&sp_lock)!=0)
	{
		printf("ERROR in cleaning!\n");
		return -1;
	}
	printf("Number of changes in thread lock owning: %d\n", change);
	printf("Final value: %d\n", shared_value);
	fflush(stdout);
//	m5_exit(0);
	return 0;
}



