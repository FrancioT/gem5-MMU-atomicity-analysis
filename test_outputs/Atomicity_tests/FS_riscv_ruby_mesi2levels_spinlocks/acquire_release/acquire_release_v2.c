#include <stdio.h>
#include <atomic>
#include <pthread.h>
#include <gem5/m5ops.h>
#define DIM 500

using namespace std;
pthread_spinlock_t sp_lock;
int start = 0;
long long prec_elements[DIM] = {0};
//atomic<long long> My_values[DIM] = {0};
long long my_values[DIM] = {0};
unsigned int i = 0;

void do_stuff()
{
	int a=1;
	int b=2;
	for(int j=0; j<100; j++)
	{
		b = (a-b)*(a+b);
	}
}

void *producer(void *par)
{
	int local_start = -1;
	while(local_start != 1)
	{
		pthread_spin_lock(&sp_lock);
		if(local_start==-1)
		{
			local_start=0;
			++start;
		}
		if(start>=2)
			local_start=1;
		pthread_spin_unlock(&sp_lock);
	}
	printf("Inizio\n");
	
	for(i=0; i<DIM; i++)
	{
		do_stuff();
		prec_elements[i] = i/1.321;
		//My_values[i].store(i/1.321, memory_order_release);
		my_values[i] = i/1.321;    // oppure   my_values[i] = i*10.1234;
	}
	return NULL;
}

void *consumer(void *par)
{
	unsigned int counter=0;
	long long prec = -1;
	long long val = -1;
	unsigned int index = 0;
	int local_start = -1;
	while(local_start != 1)
	{
		pthread_spin_lock(&sp_lock);
		if(local_start==-1)
		{
			local_start=0;
			++start;
		}
		if(start>=2)
			local_start=1;
		pthread_spin_unlock(&sp_lock);
	}
	
	index = i;
	while(index < DIM)
	{
		val = my_values[index];
		//val = My_values[index].load(memory_order_acquire);
		prec = prec_elements[index];
		if(val!=0 && prec==0)
		{
			++counter;
			printf("prec_element  |  new_value:  %lld  |  %lld\n", prec, val);
		}
		index = i;
	}
	printf("Number of memory order inconsistency: %lld\n", counter);
	return NULL;
}

int main()
{
	pthread_t thread_1, thread_2;
	
	if(pthread_spin_init(&sp_lock, PTHREAD_PROCESS_PRIVATE) != 0)
	{
		printf("ERROR in initializing spin lock!\n");
		return -1;
	}

	m5_switch_cpu();	
	pthread_create(&thread_1, NULL, producer, NULL);
	pthread_create(&thread_2, NULL, consumer, NULL);
	pthread_join(thread_1, NULL);
	pthread_join(thread_2, NULL);
	fflush(stdout);
	if(pthread_spin_destroy(&sp_lock) != 0)
	{
		printf("ERROR in initializing spin lock!\n");
		return -1;
	}
//	m5_exit(0);
	return 0;
}
