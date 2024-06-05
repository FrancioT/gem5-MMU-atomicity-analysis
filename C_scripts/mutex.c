#include <stdio.h>
#include <pthread.h>
#include <stdlib.h>
#include <gem5/m5ops.h>
#define N_TIMES 10
#define N_INCREM 3

pthread_mutex_t my_mutex;
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
	
	//printf("Entro %d\n", increm);
	
	for(int i=0; i<N_TIMES; i++)
	{
		if(pthread_mutex_lock(&my_mutex)!=0)
			return (void *) -1;
		if(shared_value!=old_val)
			change++;
		for(int j=0; j<N_INCREM; j++)
		{
			shared_value = shared_value + increm;
		}
		old_val=shared_value;
//		shared_value += increm;
		if(pthread_mutex_unlock(&my_mutex)!=0)
			return (void *) -1;
	}
	
//	printf("Esco %d\n", increm);
	return 0;
}

int main()
{
	pthread_t thread_1, thread_2;
	int param_1= 1;
	int param_2= -1;
	void *ret;
	
	printf("Testing mutex...\n");
	if(pthread_mutex_init(&my_mutex, NULL)!=0 || pthread_spin_init(&sp_lock, PTHREAD_PROCESS_PRIVATE)!=0)
	{
		printf("ERROR in initializing!\n");
		return -1;
	}
	m5_switch_cpu();
	pthread_create(&thread_1, NULL, func, (void *)&param_1);
	pthread_create(&thread_2, NULL, func, (void *)&param_2);
	
	pthread_join(thread_1, &ret);
	if((int)ret!=0)
		printf("IL THREAD 1 HA FALLITO!\n");
	pthread_join(thread_2, &ret);
	if((int)ret!=0)
		printf("IL THREAD 2 HA FALLITO!\n");
	if(pthread_mutex_destroy(&my_mutex)!=0 || pthread_spin_destroy(&sp_lock)!=0)
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



