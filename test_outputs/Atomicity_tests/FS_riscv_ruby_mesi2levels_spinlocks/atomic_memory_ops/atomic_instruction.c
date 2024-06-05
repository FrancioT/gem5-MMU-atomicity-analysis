#include <stdio.h>
#include <stdlib.h>
#include <stdatomic.h>
#include <gem5/m5ops.h>
#include <pthread.h>
#define N_TIMES 20

pthread_spinlock_t sp_lock;
atomic_int my_index = ATOMIC_VAR_INIT(0);
//int my_index=0;
int start = 0;

void *func(void *par)
{
	int increm = *((int *)par);
	int n_times = *(((int *)par)+1);
	int local_start = 0;
	
	while(!local_start)
	{
		pthread_spin_lock(&sp_lock);
		if(start!=0 && start!=increm)
			local_start=1;
		start = increm;
		pthread_spin_unlock(&sp_lock);
	}
	
	for(int i=0; i<n_times; i++)
	{
		atomic_fetch_add(&my_index, increm);
		//my_index = my_index+increm;
	}
	return NULL;
}

int main()
{
	pthread_t thread_1, thread_2;
	int param_1[]={1, N_TIMES};
	int param_2[]={-1, N_TIMES};
	
	if(pthread_spin_init(&sp_lock, PTHREAD_PROCESS_PRIVATE) != 0)
	{
		printf("ERROR in initializing spin lock!\n");
		return -1;
	}
	m5_switch_cpu();
	pthread_create(&thread_1, NULL, func, (void *)&param_1);
	pthread_create(&thread_2, NULL, func, (void *)&param_2);
	pthread_join(thread_1, NULL);
	pthread_join(thread_2, NULL);
	if(pthread_spin_destroy(&sp_lock) != 0)
	{
		printf("ERROR in initializing spin lock!\n");
		return -1;
	}
	printf("%d\n", my_index);
	return 0;
}



