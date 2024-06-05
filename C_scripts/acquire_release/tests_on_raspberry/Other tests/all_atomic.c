#include <stdio.h>
#include <atomic>
#include <pthread.h>
#define DIM 1000000L

using namespace std;
atomic<long long> prec_elements[DIM] = {0};
atomic<long long> my_values[DIM] = {0};
atomic<long long> i = 0;

void *producer(void *par)
{
	long long n_times = *((long long *)par);
	for(i.store(0, memory_order_relaxed); i.load(memory_order_relaxed)<n_times; i.fetch_add(1, memory_order_relaxed))
	{
		prec_elements[i.load(memory_order_relaxed)].store(i.load(memory_order_relaxed)+1, memory_order_relaxed);
		my_values[i.load(memory_order_relaxed)].store(i.load(memory_order_relaxed)+1, memory_order_relaxed);
	}
	return NULL;
}

void *consumer(void *par)
{
	long long n_times = *((long long *)par);
	long long counter=0;
	long long prec = -1;
	long long val = -1;
	long long index = 0;
	index = i.load(memory_order_relaxed);
	while(index < n_times)
	{
		val = my_values[index].load(memory_order_relaxed);
		prec = prec_elements[index].load(memory_order_relaxed);
		if(val > prec)
		{
			++counter;
			printf("prec_element  |  my_index:  %lld  |  %lld\n", prec, val);
		}
		index = i.load(memory_order_relaxed);
	}
	printf("Number of memory order inconsistency: %lld\n", counter);
	return NULL;
}

int main()
{
	pthread_t thread_1, thread_2;
	long long param = DIM;
	
	pthread_create(&thread_1, NULL, producer, (void *)&param);
	pthread_create(&thread_2, NULL, consumer, (void *)&param);
	pthread_join(thread_1, NULL);
	pthread_join(thread_2, NULL);
	
	for(int j=0; j<100; j++)
	{
		for(i.store(0, memory_order_relaxed); i.load(memory_order_relaxed)<DIM; i.fetch_add(1, memory_order_relaxed))
		{
			prec_elements[i.load(memory_order_relaxed)].store(0, memory_order_relaxed);
			my_values[i.load(memory_order_relaxed)].store(0, memory_order_relaxed);
		}
		i.store(0, memory_order_relaxed);
		pthread_create(&thread_1, NULL, producer, (void *)&param);
	        pthread_create(&thread_2, NULL, consumer, (void *)&param);
	        pthread_join(thread_1, NULL);
	        pthread_join(thread_2, NULL);
	}
	return 0;
}


