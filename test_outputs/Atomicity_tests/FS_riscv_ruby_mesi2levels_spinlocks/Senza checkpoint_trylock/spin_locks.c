#include <stdio.h>
#include <pthread.h>
#include <stdlib.h>
#include <time.h>
#include <gem5/m5ops.h>
#define DIM 5000
#define B_DIM 256

pthread_spinlock_t sp_lock;
int vect[DIM]={0};
int curr_index=0;
int change = 0;

void do_stuff(char buffer[], int dim)
{
	for(int i=0; i<dim; i++)
		buffer[i]=i;
	return;
}

void *func(void *par)
{
	char buffer[B_DIM] = {0};
	int val = *((int *)par);
	int lock_mine = 0;
	int while_flag=1;
	while(while_flag)
	{
		if(!lock_mine)
		{
			do_stuff(buffer, rand()%B_DIM);
			if(pthread_spin_trylock(&sp_lock) != 0)
				continue;
			
			lock_mine=1;
			if(curr_index!=0)	// check how many times there was a change in the ownership of the spinlock
				if(vect[curr_index-1]!=val)
					++change;
			if(curr_index>=DIM)    // if the array was already filled by the other thread, stop
			{
				lock_mine=0;
				pthread_spin_unlock(&sp_lock);
				while_flag=0;
				continue;
			}
		}
		
		if(curr_index%10==9)
		{
			vect[curr_index] = val;
			++curr_index;
			lock_mine = 0;
			pthread_spin_unlock(&sp_lock);
		}
		else if(lock_mine)
		{
			vect[curr_index] = val;
			++curr_index;
		}
	}
	return NULL;
}

int main()
{
	pthread_t thread_1, thread_2;
	int param_1= 1;
	int param_2= 2;
	srand(time(NULL));
	
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
/*	if(pthread_spin_destroy(&sp_lock) != 0)
	{
		printf("ERROR in initializing spin lock!\n");
		return -1;
	}
	int chunk_size=1;
	for(int i=1; i<DIM; i++)
	{
		if(vect[i]==vect[i-1])
			++chunk_size;
		else
		{
			printf("%d   %d\n", vect[i-1], chunk_size);
			chunk_size=1;
		}
	}*/
	printf("Number of changes in thread lock owning: %d\n", change);
	fflush(stdout);
	m5_exit(0);
	return 0;
}



