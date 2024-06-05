#include <stdio.h>
#include <atomic>
#include <pthread.h>
#include <string.h>
#include <gem5/m5ops.h>
#define DIM 50000L

using namespace std;
long long prec_elements[DIM] = {0};
atomic<long long> My_values[DIM] = {0};
long long my_values[DIM] = {0};
long long i = 0;

void *producer_noMB(void *par)
{
	long long n_times = *((long long *)par);
	for(i=0; i<n_times; i++)
	{
		prec_elements[i] = i+1;
		my_values[i] = i+1;
	}
	return NULL;
}

void *consumer_noMB(void *par)
{
	long long n_times = *((long long *)par);
	long long counter=0;
	long long prec = -1;
	long long val = -1;
	long long index = 0;
	index = i;
	while(index < n_times)
	{
		val = my_values[index];
		prec = prec_elements[index];
		if(val > prec)
			++counter;
		index = i;
	}
	printf("Number of memory order inconsistency: %d\n", counter);
	return NULL;
}


void *producer_withMB(void *par)
{
	long long n_times = *((long long *)par);
	for(i=0; i<n_times; i++)
	{
		prec_elements[i] = i+1;
		My_values[i].store(i, memory_order_release);
	}
	return NULL;
}

void *consumer_withMB(void *par)
{
	long long n_times = *((long long *)par);
	long long counter=0;
	long long prec = -1;
	long long val = -1;
	long long index = 0;
	while(i < n_times)
	{
		index = i;
		val = My_values[index].load(memory_order_acquire);
		prec = prec_elements[index];
		if(val > prec)
			++counter;
	}
	printf("Number of memory order inconsistency: %d\n\n", counter);
	return NULL;
}

int main()
{
	pthread_t thread_1, thread_2;
	long long param = DIM;

	m5_switch_cpu();	
	pthread_create(&thread_1, NULL, producer_noMB, (void *)&param);
	pthread_create(&thread_2, NULL, consumer_noMB, (void *)&param);
	pthread_join(thread_1, NULL);
	pthread_join(thread_2, NULL);
	
	memset(prec_elements, 0, sizeof(prec_elements));
	i=0;
	pthread_create(&thread_1, NULL, producer_withMB, (void *)&param);
	pthread_create(&thread_2, NULL, consumer_withMB, (void *)&param);
	pthread_join(thread_1, NULL);
	pthread_join(thread_2, NULL);
	fflush(stdout);
//	m5_exit(0);
	return 0;
}
