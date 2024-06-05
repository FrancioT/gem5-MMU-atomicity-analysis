#include <stdio.h>
#include <atomic>
#include <pthread.h>
#include <gem5/m5ops.h>
#define DIM 1000

using namespace std;
long long prec_elements[DIM] = {0};
//atomic<long long> My_values[DIM] = {0};
long long my_values[DIM] = {0};
long long i = 0;

/*void do_stuff()
{
	int a=1;
	int b=2;
	for(int j=0; j<100; j++)
	{
		b = (a-b)*(a+b);
	}
}*/

void *producer(void *par)
{
	int checkpoint_counter = 0;
	for(i=0; i<DIM; i++)
	{
		if(i%100==0)
			printf("checkpoint %d\n", checkpoint_counter++);
		//	do_stuff();*/
		prec_elements[i] = i+1;
		//My_values[i].store(i+1, memory_order_release);
		my_values[i] = i+1;
	}
	return NULL;
}

void *consumer(void *par)
{
	long long counter=0;
	long long prec = -1;
	long long val = -1;
	long long index = 0;
	index = i;
	while(index < DIM)
	{
		val = my_values[index];
		//val = My_values[index].load(memory_order_acquire);
		prec = prec_elements[index];
		if(val > prec)
		{
			++counter;
			printf("prec_element  |  my_index:  %lld  |  %lld\n", prec, val);
		}
		index = i;
	}
	printf("Number of memory order inconsistency: %lld\n", counter);
	return NULL;
}

int main()
{
	pthread_t thread_1, thread_2;

	m5_switch_cpu();	
	pthread_create(&thread_1, NULL, producer, NULL);
	pthread_create(&thread_2, NULL, consumer, NULL);
	pthread_join(thread_1, NULL);
	pthread_join(thread_2, NULL);
	fflush(stdout);
//	m5_exit(0);
	return 0;
}
